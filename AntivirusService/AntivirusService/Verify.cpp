#include <Windows.h>

#include <SoftPub.h>


#pragma comment(lib, "wintrust.lib")
bool CheckPeSig(const WCHAR *lpszFileName, bool bCheckRevocations)
{
	bool rv = false;

	WINTRUST_FILE_INFO WITFIO;
	RtlZeroMemory(&WITFIO, sizeof(WINTRUST_FILE_INFO));
	WITFIO.cbStruct = sizeof(WINTRUST_FILE_INFO);
	WITFIO.pcwszFilePath = lpszFileName;

	GUID WINTrustGuid = WINTRUST_ACTION_GENERIC_VERIFY_V2;
	WINTRUST_DATA WITDATA;
	RtlZeroMemory(&WITDATA, sizeof(WINTRUST_DATA));
	WITDATA.cbStruct = sizeof(WINTRUST_DATA);
	WITDATA.dwUIChoice = WTD_UI_NONE;
	WITDATA.fdwRevocationChecks = bCheckRevocations ? WTD_REVOKE_WHOLECHAIN : WTD_REVOKE_NONE;
	WITDATA.dwUnionChoice = WTD_CHOICE_FILE;
	WITDATA.dwStateAction = WTD_STATEACTION_VERIFY;
	WITDATA.pFile = &WITFIO;

	LONG lStatus = WinVerifyTrust((HWND)INVALID_HANDLE_VALUE, &WINTrustGuid, &WITDATA);
	if (lStatus == 0) {
		rv = true;
	}
	WITDATA.dwStateAction = WTD_STATEACTION_CLOSE;
	WinVerifyTrust((HWND)INVALID_HANDLE_VALUE, &WINTrustGuid, &WITDATA);

	return rv;
}