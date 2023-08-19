/*

This file contains all the PE file information related code.


*/


#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <imagehlp.h>
#include "PE.h"

typedef struct _PEInfo {
	LPCWSTR lpszFileName;
	std::vector<LPSTR> IAT;
	std::vector<LPSTR> ImportTableDLL;
	BOOL IatPresent;
	DWORD IatEntryPoint;
	std::vector<LPSTR> EAT;
	BOOL EatPresent;
	DWORD EatEntryPoint;
	BOOL ValidPe;
	DWORD EntryPointAddress;
	WORD Type;
	DWORD ImageSize;
	DWORD ImageBase;

} PEInfo, *PPEInfo;

typedef enum{
	MapFileView,
	AlreadyMapped
}FileMapForm;

DWORD RvaToOffset(IMAGE_NT_HEADERS * pNtHdr, DWORD dwRVA)
{
	WORD wSections;
	PIMAGE_SECTION_HEADER pSectionHdr;

	pSectionHdr = IMAGE_FIRST_SECTION(pNtHdr);
	wSections = pNtHdr->FileHeader.NumberOfSections;

	for (size_t i = 0; i < wSections; i++)
	{
		if (pSectionHdr->VirtualAddress <= dwRVA)
			if ((pSectionHdr->VirtualAddress + pSectionHdr->Misc.VirtualSize) > dwRVA)
			{
				dwRVA -= pSectionHdr->VirtualAddress;
				dwRVA += pSectionHdr->PointerToRawData;

				return (dwRVA);
			}

		pSectionHdr++;
	}

	return 0;
}
BOOL GetPEInfo(PEInfo &PeInfo, FileMapForm Form)
{
	PIMAGE_DOS_HEADER ImgDosHeader;


	PIMAGE_DATA_DIRECTORY ImgDataDirectory;
	PIMAGE_IMPORT_DESCRIPTOR ImgImportDescriptor;
	PIMAGE_THUNK_DATA ImgFirstThunk;
	PIMAGE_THUNK_DATA ImgThunkData;
	PIMAGE_IMPORT_BY_NAME ImgImportName;
	PIMAGE_SECTION_HEADER SectionHeader;

	HANDLE file = NULL, FileMap = NULL;
	LPVOID MappedView = NULL;
	BOOL rv = FALSE;

	if(Form == MapFileView){
		file = CreateFileW(PeInfo.lpszFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE) {
			std::cout << "error, failed to open file for reading..." << '\n';
			return rv;
		}
		FileMap = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
		if (!FileMap) {
			std::cout << "Error CreateFileMapping() failed to map file for reading..." << '\n';

			CloseHandle(file);
			return rv;
		}
		MappedView = MapViewOfFile(FileMap, FILE_MAP_READ, 0, 0, 0);
		if (!MappedView) {
			std::cout << "Error MapViewOfFile() failed!" << '\n';

			CloseHandle(FileMap);
			CloseHandle(file);
			return rv;
		}
	}
	else if(Form == AlreadyMapped){
		//ReadProcessMemory of remote PID
		
	}

	ImgDosHeader = (PIMAGE_DOS_HEADER)MappedView;
	if (ImgDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		std::cout << "File is not a valid!" << '\n';
		PeInfo.ValidPe = FALSE;
		UnmapViewOfFile(MappedView);
		CloseHandle(FileMap);
		CloseHandle(file);
		return rv;
	}
	else {
		std::cout << "Valid PE!" << '\n';
		PeInfo.ValidPe = TRUE;
	}

	PIMAGE_NT_HEADERS NtImgHeader = (PIMAGE_NT_HEADERS)((char*)ImgDosHeader + ImgDosHeader->e_lfanew);

	if (NtImgHeader->Signature != IMAGE_NT_SIGNATURE) {
		std::cout << "File is not a PE file!" << '\n';
		PeInfo.ValidPe = FALSE;
		UnmapViewOfFile(MappedView);
		CloseHandle(FileMap);
		CloseHandle(file);
		return rv;
	}
	else {

		PeInfo.Type = NtImgHeader->OptionalHeader.Subsystem;
	}
	PeInfo.ImageSize = NtImgHeader->OptionalHeader.SizeOfImage;
	PeInfo.EntryPointAddress = NtImgHeader->OptionalHeader.AddressOfEntryPoint;
	PeInfo.ImageBase = NtImgHeader->OptionalHeader.ImageBase;
	ImgDataDirectory = &NtImgHeader->OptionalHeader.DataDirectory[1];
	ImgImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((char*)ImgDosHeader + RvaToOffset(NtImgHeader, ImgDataDirectory->VirtualAddress));
	ImgThunkData = (PIMAGE_THUNK_DATA)((char*)ImgDosHeader + RvaToOffset(NtImgHeader, ImgImportDescriptor->OriginalFirstThunk));

	SectionHeader = IMAGE_FIRST_SECTION(NtImgHeader);

	PeInfo.IatEntryPoint = (DWORD)((ImgDataDirectory - SectionHeader->VirtualAddress) + SectionHeader->PointerToRawData);
	if (NtImgHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size != 0) {
		do {
			ImgThunkData = (PIMAGE_THUNK_DATA)((char*)ImgDosHeader + RvaToOffset(NtImgHeader, ImgImportDescriptor->OriginalFirstThunk));

			ImgFirstThunk = (PIMAGE_THUNK_DATA)((char*)ImgDosHeader + RvaToOffset(NtImgHeader, ImgImportDescriptor->FirstThunk));

			while (ImgThunkData->u1.AddressOfData != 0)
			{
				ImgImportName = (PIMAGE_IMPORT_BY_NAME)((char*)ImgDosHeader + RvaToOffset(NtImgHeader, ImgThunkData->u1.AddressOfData));
				LPSTR dll = ((char*)ImgDosHeader + RvaToOffset(NtImgHeader, ImgImportDescriptor->Name));

				PeInfo.ImportTableDLL.push_back(dll);
				PeInfo.IAT.push_back(ImgImportName->Name);

				ImgThunkData++;
				ImgFirstThunk++;
			}
			ImgImportDescriptor++;
		} while (ImgImportDescriptor->OriginalFirstThunk != 0);
	}
	else {
		std::cout << "There is no import directory for the file : " << PeInfo.lpszFileName << '\n';
	}








	PIMAGE_EXPORT_DIRECTORY ImgExportDirectory;



	if (NtImgHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size != 0)
	{
		ImgExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((DWORD_PTR)ImgDosHeader + RvaToOffset(NtImgHeader, NtImgHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress));
		std::cout << "Library Name   :" << '\n';
		while (ImgExportDirectory->Name != NULL)
		{
			PeInfo.EAT.push_back((char*)((DWORD_PTR)ImgDosHeader + RvaToOffset(NtImgHeader, ImgExportDirectory->Name)));
			ImgExportDirectory++;

		}

	}
	else
	{
		std::cout << "No Export Table!\n";
	}


	PIMAGE_RESOURCE_DIRECTORY ImgResourceDirectory;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY ImgResourceDirectoryEntry;
	if (NtImgHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size != 0) {
		ImgResourceDirectory = (PIMAGE_RESOURCE_DIRECTORY)((DWORD_PTR)ImgDosHeader + RvaToOffset(NtImgHeader, NtImgHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress));
		ImgResourceDirectoryEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(ImgResourceDirectory + 1);
		for (unsigned int i = 0; i < ImgResourceDirectory->NumberOfIdEntries; i++) {
			if (ImgResourceDirectoryEntry->Id == (WORD)RT_VERSION) {
				std::cout << "The name of the resource is: " << ImgResourceDirectoryEntry->Name << '\n';

			}
		}

	}
	else {
		std::cout << "No resources found!\n";
	}

	PIMAGE_DELAYLOAD_DESCRIPTOR ImgDirectoryDelayImport;
	if(NtImgHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size != 0){
		DWORD DelayLoadSize = NtImgHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size / sizeof(IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT);
		ImgDirectoryDelayImport = (PIMAGE_DIRECTORY_ENTRY_DELAY_IMPORT)((DWORD_PTR)ImgDosHeader + RvaToOffset(NtImgHeader, NtImgHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress));
		while(DelayLoadSize->DllNameRVA != NULL){
			PIMAGE_BOUND_IMPORT_DESCRIPTOR ImgBoundImportDescriptor;
			ImgBoundImportDescriptor = (PIMAGE_BOUND_IMPORT_DESCRIPTOR)((DWORD_PTR)ImgDosHeader + RvaToOffset(NtImgHeader, BoundImportAddressTableRVA));
			for(unsigned int i = 0; i < ImgBoundImportDescriptor->NumberOfModuleForwarderRefs; i++){


			}

			DelayLoadSize++;
		}
	}


	PIMAGE_DEBUG_DIRECTORY ImgDebugDirectory;
	DWORD DebugImageSize = NtImgHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size / sizeof(IMAGE_DEBUG_DIRECTORY);
	DWORD DebugVirtualAddr = NtImgHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress;
	if (DebugImageSize != 0 && DebugVirtualAddr != 0 && DebugImageSize > sizeof(IMAGE_DEBUG_DIRECTORY)) {
		std::cout << "The DebugDirecotry is not empty!!!\n";

		ImgDebugDirectory = (PIMAGE_DEBUG_DIRECTORY)((DWORD_PTR)ImgDosHeader + RvaToOffset(NtImgHeader, DebugVirtualAddr));

		DWORD debugtype = ImgDebugDirectory->Type;

		for (unsigned int i = 0; i < DebugImageSize; i++, ImgDebugDirectory++) {
			if (ImgDebugDirectory->SizeOfData != 0) {

				struct CV_HEADER
				{
					DWORD Sig;
					DWORD Offset;
				};
				struct CV_INFO_PDB20 {
					CV_HEADER CVHEADER;
					DWORD Sig;
					DWORD Age;
					BYTE Pdb[1];

				};

				struct CV_INFO_PDB70 {
					DWORD CVSig;
					GUID Sig;
					DWORD Age;
					BYTE Pdb[1];
				};
				switch (debugtype) {
				case IMAGE_DEBUG_TYPE_MISC:
				case IMAGE_DEBUG_TYPE_CODEVIEW:
				{
					DWORD CVSig = *(DWORD*)ImgDosHeader + ImgDebugDirectory->PointerToRawData;
					if (CVSig == '01BN') {
						// if this is the sig then, it is PDB v2.0
						CV_INFO_PDB20 *CVInfoPdb20 = (CV_INFO_PDB20*)CVSig;
						std::cout << "Age of PDB v2.0 : " << CVInfoPdb20->Age << '\n';
						std::cout << "The name of the PDB is : " << CVInfoPdb20->Pdb << '\n';
					}
					else if (CVSig == 'SDSR') {
						// if this is the sig then, it is PDB v7.0
						CV_INFO_PDB70 *CVInfoPdb20 = (CV_INFO_PDB70*)CVSig;
						std::cout << "Age of PDB 7.0 : " << CVInfoPdb20->Age << '\n';
						std::cout << "The name of the PDB is : " << CVInfoPdb20->Pdb << '\n';
					}
					else {
						std::cout << "The debug data format is some other !!!\n";
					}

					break;
				}
				default:
					break;


				}
			}
		}

	}
	else {
		std::cout << " There is not debug directory\n";
	}
	DWORD dwCertsCount = 0;

	WIN_CERTIFICATE *pCert = NULL;

	ImageEnumerateCertificates(file, CERT_SECTION_TYPE_ANY, &dwCertsCount, NULL, NULL);

	DWORD *dwIndices = (DWORD*)HeapAlloc(GetProcessHeap(), 0, dwCertsCount + 1);
	if (ImageEnumerateCertificates(file, CERT_SECTION_TYPE_ANY, &dwCertsCount, dwIndices, dwCertsCount) && dwCertsCount)
	{
		for (DWORD i = 0; i < dwCertsCount; i++)
		{
			DWORD dwCertLen = 0;
			pCert = NULL;
			if (ImageGetCertificateData(file, dwIndices[i], NULL, &dwCertLen))
			{
				pCert = (LPWIN_CERTIFICATE)HeapAlloc(GetProcessHeap(), 0, dwCertLen);
				if (ImageGetCertificateData(file, dwIndices[i], pCert, &dwCertLen))
				{
					std::cout << pCert->bCertificate << '\n';

				}
				HeapFree(GetProcessHeap(), 0, pCert);
			}

		}
	}
	HeapFree(GetProcessHeap(), 0, dwIndices);


	/*

	handle enclave data... this is new to windows 10
	*/


	/*
	everything below is if the file is .NET
	*/
	PIMAGE_COR20_HEADER Cor20Header;
	DWORD Cor20HeaderLen = NtImgHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size;
	Cor20Header = (PIMAGE_COR20_HEADER)ImgDosHeader + RvaToOffset(NtImgHeader, NtImgHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress);

	if (Cor20HeaderLen != 0 && Cor20Header != 0) {
		// neither is 0
		std::cout << "Cor20headerLen EntryPointToken: " << Cor20Header->Flags << '\n';

	}


	if(FileMapForm == MapFileView){

		UnmapViewOfFile(MappedView);
		CloseHandle(FileMap);
		CloseHandle(file);
	}
	else if(FileMapForm == AlreadyMapped){

	}

	std::cout << "Done " << '\n';
	return rv;
}



// this is a sample "test" of how to scan a PE to extractr information.
int main() {
	LPCWSTR File = L"C:\\Users\\Administrator\\source\\repos\\JavascriptEmulation\\Release\\JavascriptEmulation.exe";

	PEInfo PeInfo = { 0 };
	PeInfo.lpszFileName = File;
	GetPEInfo(PeInfo);
	std::cout << "The Address to the EntryPoint is" << PeInfo.EntryPointAddress << '\n';

	getchar();
	return 0;
}