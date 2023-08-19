#define _CRT_SECURE_NO_WARNINGS
#include "emulate.h"

#include <vector>






IMAGE_SECTION_HEADER *SectionArray = NULL;

enum Movement
{
	Beginning = 1,
	Current = 2, 
	Ending = 3,
};

static ULONGLONG CurrentPosition;
static ULONGLONG FileSize;

std::vector<BYTE> Data;
BOOL Read(BYTE* &lpszBuf, ULONG &dwBuf, ULONG *& dwSize)
{
	if (lpszBuf == NULL || dwBuf == 0) return FALSE;
	ULONGLONG dwCopy;
	if (CurrentPosition + dwBuf > FileSize) {
		dwCopy = (ULONGLONG)(FileSize - CurrentPosition);
	}
	else {
		dwCopy = dwBuf;
	}
	if (dwSize) *dwSize = (ULONG)dwCopy;
	if (dwCopy == 0) return E_NOT_VALID_STATE;
	memcpy(lpszBuf, &Data[(size_t)CurrentPosition], (size_t)dwCopy);
	CurrentPosition += dwCopy;
	return TRUE;
}
BOOL Seek(ULARGE_INTEGER * &Position, const LARGE_INTEGER &MoveDistance, const Movement &Move)
{
	static ULONGLONG NewPosition;
	switch (Move)
	{
	case Beginning:
		NewPosition = MoveDistance.QuadPart;
		break;
	case Current:
		NewPosition = CurrentPosition + MoveDistance.QuadPart;
		break;
	case Ending:
		NewPosition = FileSize + MoveDistance.QuadPart;
		break;
	default:
		return FALSE;
	}
	if (NewPosition > FileSize) return FALSE;
	CurrentPosition = NewPosition;
	if (Position)
		Position->QuadPart = CurrentPosition;
	return TRUE;
}


BOOL ReadAt(const LARGE_INTEGER &MoveDistance, const Movement &Move, BYTE* &lpszBuf, ULONG &dwBuf, ULONG * dwSize)
{
	ULARGE_INTEGER *Position = NULL;
	BOOL hr = Seek(Position, MoveDistance, Move);
	if (!hr) return hr;
	return Read(lpszBuf, dwBuf, dwSize);
}

DWORD Align(DWORD str1, DWORD str2) {

	return (str1 + (str2 - 1)) & ~(str2 - 1);
}










void main() {
	uc_engine *uc;
	uc_err err;
	err = uc_open(UC_ARCH_X86, UC_MODE_32, &uc);
	if (err) {
		printf("error returned %u: %s", err, uc_strerror(err));
		getchar();
	}



	LPCWSTR FilePath = L"C:/Users/Administrator/source/repos/JavascriptEmulation/Release/JavaScriptEmulation.exe";



	HANDLE hFile = CreateFileW(FilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (!hFile) {
		return ;
	}
	SetFilePointer(hFile, 0, 0, FILE_END);
	ULONG dwSize = 0;
	ULONG FileSize = GetFileSize(hFile, 0);
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);

	UCHAR *lpszFileMem = (UCHAR *)HeapAlloc(GetProcessHeap(), 0, FileSize + 1);
	RtlZeroMemory(lpszFileMem, FileSize);


	ReadFile(hFile, lpszFileMem, FileSize, NULL, NULL);
	CloseHandle(hFile);

	LARGE_INTEGER li = { 0 };
	PIMAGE_DOS_HEADER ImgDosHeader = (PIMAGE_DOS_HEADER)lpszFileMem;
	PIMAGE_NT_HEADERS32 ImgNtHeaders = (PIMAGE_NT_HEADERS32)(((BYTE *)ImgDosHeader) + (ImgDosHeader->e_lfanew));
	PIMAGE_SECTION_HEADER section = (PIMAGE_SECTION_HEADER)(ImgNtHeaders + 1);
	UINT64 ImageBase = ImgNtHeaders->OptionalHeader.ImageBase;
	UINT64 EntryPoint = ImgNtHeaders->OptionalHeader.AddressOfEntryPoint;
	UINT ImageSize = ImgNtHeaders->OptionalHeader.SizeOfImage;
	DWORD SecAlignment = ImgNtHeaders->OptionalHeader.SectionAlignment;
	DWORD StackReserve = ImgNtHeaders->OptionalHeader.SizeOfStackReserve;


	err = uc_mem_map(uc, ImageBase, ImageSize, UC_PROT_ALL);
	if (err) {
		printf("uc_mem_map returned %u: %s", err, uc_strerror(err));
		getchar();
	}
	err = uc_mem_write(uc, ImageBase, lpszFileMem, ImageSize);
	if (err) {
		printf("uc_mem_write returned %u: %s", err, uc_strerror(err));
		getchar();
	}
	DWORD SizeOfImage = Align(ImageSize, SecAlignment);



}