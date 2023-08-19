/*
PE file is not the same as disk to the one in memory
*/
#include <Windows.h>
BOOL CompareSectionHeaders(PIMAGE_SECTION_HEADER DiskFile, PIMAGE_SECTION_HEADER MemoryFile, unsigned int sections) {

	for (unsigned int section = 0; section < sections; section++) {
		if(
			
			(RtlCompareMemory(DiskFile[section].Name, MemoryFile[section].Name, 8) != 8) ||
			(DiskFile[section].VirtualAddress != MemoryFile[section].VirtualAddress) ||
			(DiskFile[section].SizeOfRawData != MemoryFile[section].SizeOfRawData) ||
			(DiskFile[section].Characteristics != MemoryFile[section].Characteristics)
			
			){


			//
			// file has changed?!?
			//
			return TRUE;
		}
	}
	return FALSE;


}


BOOL CompareOptionalHeaders(PIMAGE_OPTIONAL_HEADER DiskFile, PIMAGE_OPTIONAL_HEADER MemoryFile) {
	if (
		(DiskFile->CheckSum != MemoryFile->CheckSum)||
		(DiskFile->AddressOfEntryPoint != MemoryFile->AddressOfEntryPoint) ||
		(DiskFile->BaseOfCode != MemoryFile->BaseOfCode) ||
		(DiskFile->BaseOfData != MemoryFile->BaseOfData)||
		(DiskFile->SizeOfUninitializedData != MemoryFile->SizeOfUninitializedData) ||
		(DiskFile->SizeOfImage != MemoryFile->SizeOfImage) 
	){


		// welp this is malware!
		return TRUE;
	}

	return FALSE;

}

BOOL CompareNtHeaders(PIMAGE_NT_HEADERS DiskFile, PIMAGE_NT_HEADERS MemoryFile) {

	if (
		(DiskFile->FileHeader.TimeDateStamp != MemoryFile->FileHeader.TimeDateStamp) ||
		(DiskFile->FileHeader.SizeOfOptionalHeader != MemoryFile->FileHeader.SizeOfOptionalHeader) ||
		(DiskFile->FileHeader.Characteristics != MemoryFile->FileHeader.Characteristics)
		) {
		return TRUE;
	}

	return FALSE;
}



BOOL CompareDiskFileToMemory(PIMAGE_NT_HEADERS DiskFileNtHdrs, PIMAGE_NT_HEADERS MemoryFileNtHdrs, PIMAGE_SECTION_HEADER DiskFileSectionHdrs, PIMAGE_SECTION_HEADER MemoryFileSectionHdrs, unsigned int sections) {

	if (CompareNtHeaders(DiskFileNtHdrs, MemoryFileNtHdrs) || CompareOptionalHeaders(&DiskFileNtHdrs->OptionalHeader, &MemoryFileNtHdrs->OptionalHeader) || CompareSectionHeaders(DiskFileSectionHdrs, MemoryFileSectionHdrs, sections) ){
		return TRUE;
	}
	return FALSE;
}