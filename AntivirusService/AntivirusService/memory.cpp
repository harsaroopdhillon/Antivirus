/*


Hi there! Our micropatches are applied in-memory, so the binaries remain untouched. We're not using Detours but code based on mHook.

*/
#include <Windows.h>
#include <Psapi.h>
#include <exception>
#include <iostream>
#include <TlHelp32.h>
#include <vector>
#include <map>
#include <mutex>
#include <winternl.h>
#pragma comment(lib, "ntdll.lib")

bool ReadProcessPeb(PEB *lpPeb, HANDLE dwProcessId)
{


	
	bool rv = false;

	if (dwProcessId) {
		PROCESS_BASIC_INFORMATION Pbi;
		DWORD dwSize = 0;
		DWORD dwStatus = NtQueryInformationProcess(dwProcessId, ProcessBasicInformation, &Pbi, sizeof(PROCESS_BASIC_INFORMATION), &dwSize);
		if (NT_SUCCESS(dwStatus) && dwSize == sizeof(PROCESS_BASIC_INFORMATION)) {
			void *lpRemotePeb = Pbi.PebBaseAddress;
			SIZE_T dwBytesRead = 0;
			if (lpRemotePeb && ReadProcessMemory(dwProcessId, lpRemotePeb, lpPeb, sizeof(PEB), &dwBytesRead) && dwBytesRead == sizeof(PEB)) {
				rv = true;
			}
		}
	}

	return rv;
}
bool R_ReadProcessMemory(const void *lpvRemotePointer, void *lpszDestination, DWORD dwDestinationSize, HANDLE dwProcessId)
{
	bool rv = false;


	if (dwProcessId) {
		SIZE_T dwBytesRead = 0;
		rv = ReadProcessMemory(dwProcessId, lpvRemotePointer, lpszDestination, dwDestinationSize, &dwBytesRead) && dwBytesRead == dwDestinationSize;
	}

	return rv;
}

bool DumpMemory2File(const WCHAR *lpszFileName, HANDLE dwProcessId)
{
	PEB Peb;
	PEB_LDR_DATA LoaderData;
	LIST_ENTRY Link;
	LDR_DATA_TABLE_ENTRY LoaderDataTableEntry;
	bool rv = true;

	if (ReadProcessPeb(&Peb, dwProcessId) &&
		R_ReadProcessMemory(Peb.Ldr, &LoaderData, sizeof(LoaderData), dwProcessId) &&
		R_ReadProcessMemory(LoaderData.InMemoryOrderModuleList.Flink, &Link, sizeof(Link), dwProcessId) &&
		R_ReadProcessMemory(CONTAINING_RECORD(Link.Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks), &LoaderDataTableEntry, sizeof(LoaderDataTableEntry), dwProcessId)) {

		SYSTEM_INFO si;
		GetSystemInfo(&si);
		DWORD dwPageSize = si.dwPageSize;
		BYTE *lpRemoteBaseAddress = (BYTE*)LoaderDataTableEntry.DllBase;

		HANDLE hMemoryDump = CreateFileW(lpszFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, 0, NULL);
		if (hMemoryDump != INVALID_HANDLE_VALUE) {
			BYTE *lpbaCurrentPage = (BYTE*)_alloca(dwPageSize);
			DWORD dwBytesWritten = 0;
			size_t nPages = 0;
			BOOL rc = FALSE;
			while (R_ReadProcessMemory(&lpRemoteBaseAddress[nPages * dwPageSize], lpbaCurrentPage, dwPageSize, dwProcessId) &&
				(rc = WriteFile(hMemoryDump, lpbaCurrentPage, dwPageSize, &dwBytesWritten, NULL)) == TRUE && dwBytesWritten == dwPageSize) {
				std::cout << lpbaCurrentPage << '\n';
				++nPages;
			}
			if(rc == TRUE && dwBytesWritten == 0) {
				rv = true;
			}

			CloseHandle(hMemoryDump);
		}
	}

	return rv;
}
bool DumpMemory(HANDLE dwProcessId, std::vector<BYTE*>& data, std::vector<SIZE_T>& datasize)
{
	PEB Peb;
	PEB_LDR_DATA LoaderData;
	LIST_ENTRY Link;
	LDR_DATA_TABLE_ENTRY LoaderDataTableEntry;
	bool rv = true;

	if (ReadProcessPeb(&Peb, dwProcessId) &&
		R_ReadProcessMemory(Peb.Ldr, &LoaderData, sizeof(LoaderData), dwProcessId) &&
		R_ReadProcessMemory(LoaderData.InMemoryOrderModuleList.Flink, &Link, sizeof(Link), dwProcessId) &&
		R_ReadProcessMemory(CONTAINING_RECORD(Link.Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks), &LoaderDataTableEntry, sizeof(LoaderDataTableEntry), dwProcessId)) {

		SYSTEM_INFO si;
		GetSystemInfo(&si);
		DWORD PageSize = si.dwPageSize;
		BYTE *BaseAddress = (BYTE*)LoaderDataTableEntry.DllBase;

			BYTE *lpbaCurrentPage = (BYTE*)HeapAlloc(GetProcessHeap(), 0, PageSize + 1);
			RtlZeroMemory(lpbaCurrentPage, PageSize);
			size_t exepages = 0;
			while (R_ReadProcessMemory(&BaseAddress[exepages * PageSize], lpbaCurrentPage, PageSize, dwProcessId)) {
				
				++exepages;
			}
			data.push_back(lpbaCurrentPage);

			datasize.push_back(PageSize);
			HeapFree(GetProcessHeap(), 0, lpbaCurrentPage);

	}
	return rv;
}

bool Dump() {
	std::vector<BYTE*> dumpeddata;
	std::vector<SIZE_T> datasize;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		return false;
	}
	if (Process32First(snapshot, &entry)) {

		do {
			HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

			if (hp) {
				std::cout << "Opened Process " << entry.th32ProcessID << '\n';
				DumpMemory(hp, dumpeddata, datasize);
				//processidz.push_back(entry.th32ProcessID);
				CloseHandle(hp);
			}
			else {
				std::cout << "Could Not Open Process " << entry.th32ProcessID << '\n';
			}

		} while (Process32Next(snapshot, &entry));
	}
	for (auto oi : dumpeddata) {
		std::cout << oi;
	}
	return true;
}

