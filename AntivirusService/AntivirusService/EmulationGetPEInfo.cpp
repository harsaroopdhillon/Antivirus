#include "emulate.h"



extern TImportNameResolver ImportNameResolver;
BOOL InitatePEFile(uc_engine *uc, LPCWSTR FilePath, PSPEImage PEImage) {
	HANDLE hFile = CreateFileW(FilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (!hFile) {
		return TRUE;
	}
	SetFilePointer(hFile, 0, 0, FILE_END);

	size_t FileSize = GetFileSize(hFile, 0);
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);

	UCHAR *FileMem = (UCHAR *)HeapAlloc(GetProcessHeap(), 0, FileSize);
	RtlZeroMemory(FileMem, FileSize);


	ReadFile(hFile, FileMem, FileSize, NULL, NULL);
	CloseHandle(hFile);


	PIMAGE_DOS_HEADER pVDosHeader = (PIMAGE_DOS_HEADER)FileMem;
	PIMAGE_NT_HEADERS32 pVNTHeaders = (PIMAGE_NT_HEADERS32)(((BYTE *)pVDosHeader) + (pVDosHeader->e_lfanew));
	UCHAR *VImage = (UCHAR *)VirtualAlloc(NULL, pVNTHeaders->OptionalHeader.SizeOfImage, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	RtlZeroMemory(VImage, pVNTHeaders->OptionalHeader.SizeOfImage);
	UINT64 ImageBase = pVNTHeaders->OptionalHeader.ImageBase;
	UINT64 EP = pVNTHeaders->OptionalHeader.AddressOfEntryPoint;
	UINT ImageSize = pVNTHeaders->OptionalHeader.SizeOfImage;

	PIMAGE_SECTION_HEADER psectionheader = (PIMAGE_SECTION_HEADER)(pVNTHeaders + 1);
	for (UINT i = 0; i < pVNTHeaders->FileHeader.NumberOfSections; i++) {
		UINT64 VA = psectionheader->VirtualAddress;
		UINT64 VASize = psectionheader->Misc.VirtualSize;

		UINT64 RawOffset = psectionheader->PointerToRawData;
		UINT64 RawSize = psectionheader->SizeOfRawData;

		RtlCopyMemory(VImage + VA, FileMem + RawOffset, (size_t)RawSize);

		if (!i) {
			RtlCopyMemory(VImage, FileMem, (size_t)RawOffset);
		}

		psectionheader++;
	}

	PIMAGE_DOS_HEADER pVDosHeaderVImage = (PIMAGE_DOS_HEADER)VImage;
	PIMAGE_NT_HEADERS32 pVNTHeadersVImage = (PIMAGE_NT_HEADERS32)(((BYTE *)pVDosHeaderVImage) + (pVDosHeaderVImage->e_lfanew));

	psectionheader = (PIMAGE_SECTION_HEADER)(pVNTHeadersVImage + 1);
	for (UINT i = 0; i < pVNTHeadersVImage->FileHeader.NumberOfSections; i++) {
		psectionheader[i].PointerToRawData = psectionheader[i].VirtualAddress;
		psectionheader[i].SizeOfRawData = psectionheader[i].Misc.VirtualSize;
	}

	UINT delta = (DWORD_PTR)((LPBYTE)VImage);

	PIMAGE_DATA_DIRECTORY  datadir = (PIMAGE_DATA_DIRECTORY)(&pVNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]);

	if (datadir->VirtualAddress != 0) {
		PIMAGE_BASE_RELOCATION reloc = (PIMAGE_BASE_RELOCATION)(VImage + datadir->VirtualAddress);

		while (reloc->VirtualAddress != 0) {
			if (reloc->SizeOfBlock >= sizeof(IMAGE_BASE_RELOCATION))
			{
				DWORD relocDescNb = (reloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
				LPWORD relocDescList = (LPWORD)((LPBYTE)reloc + sizeof(IMAGE_BASE_RELOCATION));

				for (UINT i = 0; i < relocDescNb; i++)
				{
					if (relocDescList[i] > 0)
					{
						DWORD_PTR *p = (DWORD_PTR *)(VImage + (reloc->VirtualAddress + (0x0FFF & (relocDescList[i]))));

						*p -= pVNTHeaders->OptionalHeader.ImageBase;
						*p += delta;
					}
				}
			}
			reloc = (PIMAGE_BASE_RELOCATION)((LPBYTE)reloc + reloc->SizeOfBlock);
		}
	}

	pVNTHeadersVImage->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress = 0;
	pVNTHeadersVImage->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size = 0;

	uc_err err = uc_mem_map_ptr(uc, (UINT64)VImage, pVNTHeaders->OptionalHeader.SizeOfImage, UC_PROT_ALL, VImage);
	if (err) {
		return TRUE;
	}

	PIMAGE_IMPORT_DESCRIPTOR PImportDscrtr = (PIMAGE_IMPORT_DESCRIPTOR)& VImage[pVNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress];

	for (; PImportDscrtr->Name != 0; PImportDscrtr++) {
		DWORD *PtrImport;
		CHAR *ImportNameRawRel;
		std::string ImportLibraryName;
		DWORD IATRVAPtr;
		DWORD FirstThunkRVA;

		ImportLibraryName = (CHAR *)VImage + PImportDscrtr->Name;

		HMODULE hDLL = LoadLibrary(ImportLibraryName.c_str());
		if (!hDLL) {
			return FALSE;
		}


		HANDLE hModSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

		if (hModSnap == INVALID_HANDLE_VALUE) {
			CloseHandle(hDLL);
			free(FileMem);
			return FALSE;
		}

		MODULEENTRY32 me;
		UINT ModuleSize = 0;
		me.dwSize = sizeof(me);

		if (!Module32First(hModSnap, &me)) {
			CloseHandle(hModSnap);
			CloseHandle(hDLL);

			free(FileMem);
			return FALSE;
		}

		do
		{
			if (me.hModule == hDLL)
			{

				ModuleSize = me.modBaseSize;
				break;
			}
		} while (Module32Next(hModSnap, &me));


		DWORD AllAccess = PAGE_EXECUTE_READWRITE;
		DWORD OldAccess = 0;

		MEMORY_BASIC_INFORMATION mbi;
		SIZE_T len = VirtualQuery(hDLL, &mbi, sizeof(mbi));
		UC_MapMemory(uc, hDLL, FALSE);

		if (PImportDscrtr->TimeDateStamp == 0) {
			IATRVAPtr = PImportDscrtr->FirstThunk;
		}
		else {
			IATRVAPtr = PImportDscrtr->OriginalFirstThunk;
		}

		PtrImport = (DWORD *)(VImage + IATRVAPtr);
		FirstThunkRVA = PImportDscrtr->FirstThunk;

		UCHAR Int3 = 0xCC;
		for (UINT i = 0; PtrImport[i] != 0; i++) {
			if (PtrImport[i] & IMAGE_ORDINAL_FLAG32) {
				FARPROC Proc = GetProcAddress(hDLL, LPCSTR(PtrImport[i] & 0xFFFF));
				*((DWORD *)(VImage + FirstThunkRVA)) = (DWORD)Proc;


				std::string Import = ImportLibraryName + "_!_" + std::to_string(PtrImport[i] & 0xFFFF);
				ImportNameResolver[(UINT64)Proc] = Import;
			}
			else {
				ImportNameRawRel = (CHAR *)(VImage + PtrImport[i]) + 2;
				FARPROC Proc = GetProcAddress(hDLL, ImportNameRawRel);
				*((DWORD *)(VImage + FirstThunkRVA)) = (DWORD)Proc;

				std::string Import = ImportLibraryName + "!" + ImportNameRawRel;
				ImportNameResolver[(UINT64)Proc] = Import;
			}
			IATRVAPtr += 4;
			FirstThunkRVA += 4;
		}
		CloseHandle(hModSnap);
	}

	PEImage->ImageBase = (UINT64)VImage;
	PEImage->ImageSize = pVNTHeaders->OptionalHeader.SizeOfImage;
	PEImage->EntryPoint = EP + (UINT64)VImage;
	PEImage->VirtualImage = VImage;


	HeapFree(GetProcessHeap(), 0, FileMem);

	return true;
}
