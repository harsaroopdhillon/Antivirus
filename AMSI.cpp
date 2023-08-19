/*

*/
#pragma comment(lib, "amsi.lib")
#include <windows.h>
#include <amsi.h>


#define APPNAME RTL_CONSTANT_STRING("Test1")
HAMSICONTEXT *amsiContext = NULL;
HAMSICONTEXT *session = NULL;
	
BOOL InitalizeAMSI(){
	if(AmsiInitialize(APPNAME, NULL, &amsiContext) != S_OK){
		return FALSE;
	}
	if(AmsiOpenSession(amsiContext, session) != S_OK){
		AmsiUninitialize(amsiContext);
		return FALSE;
	}
	return TRUE;
}

VOID UnInitalizeAMSI(){
	AmsiCloseSession(amsiContext, session);
	AmsiUninitialize(amsiContext);

}

/*
 * return TRUE on Malware or FALSE on clean
*/
BOOL AMSIScanString(LPCWSTR lpszString, LPCWSTR optString){
	AMSI_RESULT Result = 0;
	if(AmsiScanString(amsiContext, lspzString, optString, session, &Result) != S_OK){

		return FALSE;
	}
	return AmsiResultIsMalware(Result);
}

/*
 * return TRUE on Malware or FALSE on clean
*/

BOOL AMSIScanBuffer(PVOID lpszBuf, ULONG uLen, LPCWSTR optString){
	AMSI_RESULT Result = 0;
	if(AmsiScanBuffer(amsiContext, lpszBuf, uLen, optString, sesson, &Result) != S_OK){
		return FALSE;
	}
	return AmsiResultIsMalware(Result);
}