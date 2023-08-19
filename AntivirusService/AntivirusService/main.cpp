#include <iostream>
#include <Windows.h>
#include <wchar.h>
#include "scan.h"
#define SECOND 1000
WCHAR * SERVICE = (WCHAR*)L"AntivirusService";
HANDLE hService = NULL;
SERVICE_STATUS ServiceSt;
SERVICE_STATUS_HANDLE hServiceSt = 0;
extern BOOL DriverInstall();
extern VOID InitiateDriver();
extern VOID ShutDownDriver();

VOID WINAPI lpServiceHandler(DWORD ServiceControlCode) {
	switch (ServiceControlCode) {

	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
	{
		ServiceSt.dwCurrentState = SERVICE_STOP_PENDING;
		ServiceSt.dwWaitHint = 60 * SECOND;
		if (!SetServiceStatus(hServiceSt, &ServiceSt)) {
			std::cout << "SetServiceStatus Failed... with get last error: " << GetLastError() << '\n';
		}
		SetEvent(hService);
		break;
	}
	default:
		break;
	}

}
void WINAPI ServiceMain(DWORD argc, WCHAR *argv[]) {
	RtlZeroMemory(&ServiceSt, sizeof(SERVICE_STATUS));
	ServiceSt.dwCheckPoint = 0;
	ServiceSt.dwControlsAccepted = 0;
	ServiceSt.dwCurrentState = SERVICE_STOPPED;
	ServiceSt.dwServiceSpecificExitCode = 0;
	ServiceSt.dwServiceType = SERVICE_WIN32;
	ServiceSt.dwWaitHint = 0;
	ServiceSt.dwWin32ExitCode = 0;
	hServiceSt = RegisterServiceCtrlHandlerW(SERVICE, lpServiceHandler);
	if (hServiceSt) {
		ServiceSt.dwCurrentState = SERVICE_START_PENDING;
		SetServiceStatus(hServiceSt, &ServiceSt);


		ServiceSt.dwCurrentState = SERVICE_RUNNING;
		ServiceSt.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
		SetServiceStatus(hServiceSt, &ServiceSt);
		
		do {
			InitiateDriver();

			do {
				

						SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN);
						
						SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_END);
			
			} while ((WaitForSingleObject(hService, 0)) != WAIT_OBJECT_0 || WAIT_FAILED);

			ShutDownDriver();
		} while ((WaitForSingleObject(hService, 0)) != WAIT_OBJECT_0 || WAIT_FAILED);

		
		ServiceSt.dwControlsAccepted &= ~(SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		ServiceSt.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hServiceSt, &ServiceSt);
	}
	else {
		std::cout << "RegisterServiceCtrlHandlerW () failed with getlasterror() " << GetLastError() << '\n';
	}
	return;
}

int abc(int argc, WCHAR *argv[]) {

	DriverInstall();
	const SERVICE_TABLE_ENTRYW STE[] = { 
		{ SERVICE, ServiceMain},
		{ NULL, NULL}

	};
	hService = CreateEvent(0, TRUE, FALSE, 0);
	StartServiceCtrlDispatcherW(STE);
	return 0;
}