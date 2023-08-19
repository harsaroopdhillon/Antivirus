#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <wchar.h>
#include <stdio.h>
#include <psapi.h>
#include "worker.h"
#include "service.h"
#include "config.h"
#include "header.h"
#include "thread.h"
#include <shlwapi.h>
#include "scan.h"

void Scan(DWORD dwEventType, int dScanContext, HANDLE hDriver, HANDLE hWriteCompletionEvent, DWORD dwProcessId, DWORD dwParentProcessId, WCHAR *lpszFileName, void *lpImageBase, void *lpvScanDataArray)
{
	if (!lpszFileName) return;

	BOOL bBlock = FALSE;
	BOOL bLog = FALSE;
	BOOL bQuarantine = FALSE;
	SCAN_RESULT FileScanResult;
	RtlZeroMemory(&FileScanResult, sizeof(SCAN_RESULT));
	{
		if (FileScanResult.bScanSuccessful) {
			bBlock |= FileScanResult.bBlock;
			bLog |= FileScanResult.bLog;
			bQuarantine |= FileScanResult.bQuarantine;
		}

	}
	SCAN_RESULT MemoryScanResult;
	RtlZeroMemory(&MemoryScanResult, sizeof(SCAN_RESULT));
	{
		if (MemoryScanResult.bScanSuccessful) {
			bBlock |= MemoryScanResult.bBlock;
			bLog |= MemoryScanResult.bLog;
			bQuarantine |= MemoryScanResult.bQuarantine;
		}
	}
	if (bQuarantine) {

	}
	BOOL BlockProcess = FALSE;
	if (dScanContext == SCAN_PROCESSCREATION || dScanContext == SCAN_PROCESSTERMINATION) {
		ANTIVIRUS_RESPONSE response;
		RtlZeroMemory(&response, sizeof(ANTIVIRUS_RESPONSE));
		response.EventType = dwEventType;
		response.dwProcessId = dwProcessId;
		if (dScanContext == SCAN_PROCESSCREATION) {
			if (!FileScanResult.bScanSuccessful || !MemoryScanResult.bScanSuccessful) bBlock = TRUE;

			if (bBlock) {
				response.bBlock = TRUE;
			}
		}
		if (DriverSendResponse(hDriver, hWriteCompletionEvent, &response)) {
			if (dScanContext == SCAN_PROCESSCREATION) BlockProcess = TRUE;
		}
	}
	else if (dScanContext == SCAN_IMAGELOAD) {
		ANTIVIRUS_RESPONSE response;
		RtlZeroMemory(&response, sizeof(ANTIVIRUS_RESPONSE));
		response.EventType = dwEventType;
		response.dwProcessId = dwProcessId;
		response.lpImageBase = lpImageBase;
		DriverSendResponse(hDriver, hWriteCompletionEvent, &response);
	}

	if (dScanContext == SCAN_PROCESSCREATION) {

	}
	else if (dScanContext == SCAN_PROCESSTERMINATION) {

	}
	else if (dScanContext == SCAN_PERIODICALLY) {

	}
	else if (dScanContext == SCAN_IMAGELOAD) {
		WCHAR *lpszProcessName = NULL;
		DWORD dwProcessSize = sizeof(WCHAR) * (MAX_PATH + 1);
		HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
		if (hProc) {
			lpszProcessName = (WCHAR*)HeapAlloc(GetProcessHeap(), 0, dwProcessSize);
			if (!QueryFullProcessImageNameW(hProc, PROCESS_NAME_NATIVE, lpszProcessName, &dwProcessSize)) {
				HeapFree(GetProcessHeap(), 0, lpszProcessName);
				lpszProcessName = NULL;
			}
			CloseHandle(hProc);
		}
		// terminate process here

		if (lpszProcessName) {
			HeapFree(GetProcessHeap(), 0, lpszProcessName);
		}
	}
}