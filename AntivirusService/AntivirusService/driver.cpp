
#include <iostream>
#include <Windows.h>
#include "header.h"
#include "thread.h"
#include "worker.h"
#include "config.h"
HANDLE DriverThread = NULL, ThreadingEvent = NULL, DriverHandle = INVALID_HANDLE_VALUE;

SC_HANDLE ServiceControlManager = NULL, DriverService = NULL;

DWORD WINAPI DriverServiceThread(VOID *arg);

VOID LoadKernelDriver();
VOID UnloadKernelDriver();


BOOL StopService(SC_HANDLE hService)
{

	BOOL rv = FALSE;

	SERVICE_STATUS ServiceSt;
	if (!ControlService(hService, SERVICE_CONTROL_STOP, &ServiceSt)) {
		std::cout << "StopService() failed with getlasterror: " << GetLastError() << '\n';
	}

	DWORD dwBase = GetTickCount();
	while ((rv = QueryServiceStatus(hService, &ServiceSt)) && ServiceSt.dwCurrentState != SERVICE_STOPPED) {
		Sleep(20);
	}

	if (rv && ServiceSt.dwCurrentState == SERVICE_STOPPED) {
		rv = TRUE;
	}

	return rv;
}

VOID InitiateDriver()
{
	ThreadingEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (ThreadingEvent == NULL) {
		std::cout << "failed to create stop thread event in driver serice\n";
	}

	LoadKernelDriver();

	DWORD dwThreadId = 0;
	DriverThread = CreateThread(NULL, 0, DriverServiceThread, 0, 0, &dwThreadId);
	if (DriverThread == NULL) {
		std::cout << "failed to create driver service thread\n";
	}
}


VOID ShutDownDriver()
{
	if (!SetEvent(ThreadingEvent)) {
		std::cout << "SetEvent() Failed in ShutdownDriver()\n";
	}
	if (DriverHandle != INVALID_HANDLE_VALUE) {
		CancelIoEx(DriverHandle, NULL);
	}
	WaitForSingleObject(DriverThread, INFINITE);
	UnloadKernelDriver();
}


BOOL DriverInstall()
{
	SC_HANDLE ServiceControlManager = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
	if (!ServiceControlManager) return FALSE;

	BOOL rv = FALSE;
	WCHAR szDriverPath[MAX_PATH + 7] = { '\0' };
	wsprintfW(szDriverPath, L"%ls", L"C:\\Windows\\System32\\MyDriverLocation.sys");
	SC_HANDLE ProceServiceStNotification = CreateServiceW(ServiceControlManager, DriverName, DriverDisplayName, SERVICE_START | SERVICE_STOP | DELETE, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, szDriverPath, 0, 0, 0, 0, 0);

	if (ProceServiceStNotification) {
		rv = TRUE;
		CloseServiceHandle(ProceServiceStNotification);
	}
	else if (GetLastError() == ERROR_SERVICE_EXISTS) {
		rv = TRUE;
	}

	CloseServiceHandle(ServiceControlManager);

	return rv;
}


BOOL DriverUninstall()
{
	SC_HANDLE ServiceControlManager = OpenSCManagerW(0, 0, SC_MANAGER_ALL_ACCESS);
	if (!ServiceControlManager) return FALSE;

	BOOL rv = TRUE;
	SC_HANDLE hDriverService = OpenServiceW(ServiceControlManager, DriverName, SERVICE_START | SERVICE_STOP | DELETE | SERVICE_QUERY_STATUS);
	if (hDriverService) {
		rv = StopService(hDriverService) && DeleteService(hDriverService);
	}
	else {
		rv = TRUE;
	}

	CloseServiceHandle(ServiceControlManager);

	return rv;
}


VOID LoadKernelDriver()
{
	ServiceControlManager = OpenSCManagerW(0, 0, SC_MANAGER_ALL_ACCESS);
	if (!ServiceControlManager) {
		std::cout << "OpenSCManager() faied in LoadKerenlDriver()\n";
	}

	DriverService = OpenServiceW(ServiceControlManager, DriverName, SERVICE_START | SERVICE_STOP | DELETE | SERVICE_QUERY_STATUS);
	if (DriverService) {
		if (!StopService(DriverService)) {
			std::cout << "failed to stop pre-exisitng DriverService\n";
		}
		else {
			std::cout << "Kernel Driver was loaded!\n";
		}
	}
	else {
		std::cout << "OpenService() Failed in LoadKerenlDriver()\n";
	}

	BOOL rv = StartService(DriverService, 0, NULL);
	DWORD dwLastError = GetLastError();
	if (!rv) {
		if (dwLastError == ERROR_INVALID_IMAGE_HASH) {
			// should check, maybe someone/something is tryna fuck us :)
		}
		std::cout << "StartService() failed with getlasterror() code: " << GetLastError << '\n';
	}

	DriverHandle = CreateFileW(_DEVICE_PATH, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (DriverHandle == INVALID_HANDLE_VALUE) {
		DWORD dwLastError = GetLastError();
		std::cout << "failed to open driver, getlasterror() returned: " << dwLastError << '\n';

	}

	ANTIVIURS_CONFIGURATION yc;
	RtlZeroMemory(&yc, sizeof(ANTIVIURS_CONFIGURATION));
	yc.dwReqSize = sizeof(ANTIVIRUS_REQUEST);
	yc.dwMAXReqSize = ANTIVIRUS_REQUEST_SIZE;

	yc.bWantThreadEvents = TRUE;
	yc.bWantImageLoadEvents = TRUE;

	wprintf(L"yc.bWantThreadEvents = %s", yc.bWantThreadEvents ? L"TRUE" : L"FALSE");
	wprintf(L"yc.bWantImageLoadEvents = %s", yc.bWantImageLoadEvents ? L"TRUE" : L"FALSE");


	HANDLE hDeviceIoControlEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (!hDeviceIoControlEvent) wprintf(L"Unable to create event for DeviceIoControl()");
	OVERLAPPED overlapped;
	RtlZeroMemory(&overlapped, sizeof(OVERLAPPED));
	overlapped.hEvent = hDeviceIoControlEvent;
	// lets publish config
	rv = DeviceIoControl(DriverHandle, IOCTL_CONFIGURE, &yc, sizeof(ANTIVIURS_CONFIGURATION), NULL, 0, NULL, &overlapped);
	if (!rv && GetLastError() == ERROR_IO_PENDING) {
		DWORD dwBytesTransferred = 0;
		if (!GetOverlappedResult(DriverHandle, &overlapped, &dwBytesTransferred, TRUE)) {
			std::cout << "GetOverlappedResult() failed in LoadKernelDriver() with getlasterror code: " << GetLastError() << '\n';
		}
	}
	else if (!rv) {
		std::cout << "DeviceIoControl() failed in LoadKernelDriver() with getlast error code: ", GetLastError() << '\n';
	}

	CloseHandle(hDeviceIoControlEvent);

	std::cout << "Successfuly started the driver\n";
}


VOID UnloadKernelDriver()
{
	if (DriverService) {
		StopService(DriverService);
		CloseServiceHandle(DriverService);
		DriverService = NULL;
	}

	if (ServiceControlManager) {
		CloseServiceHandle(ServiceControlManager);
	}
	ServiceControlManager = NULL;
}



DWORD WINAPI DriverServiceThread(VOID *arg)
{

	POOL_DATA pd;
	RtlZeroMemory(&pd, sizeof(POOL_DATA));
	pd.hSharedDriverHandle = DriverHandle;
	const DWORD dwNumChannels = NUM_EVENTTYPES - 1;
	THREADPOOL *tp = ThreadPoolAlloc(dwNumChannels, PfWorkerInit, PfWorkerWork, PfWorkerDestroy, &pd, sizeof(WORKER_DATA));
	if (!tp) wprintf(L"Unable to allocate threadpool");

	// Create the read file event for use with overlapped I/O
	HANDLE hReadFileEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hReadFileEvent == NULL) wprintf(L"Unable to create read file event");

	DWORD dwNumConsecutiveZeroReads = 0;

	// Allocate memory for the buffer to be read from the kernel
	ANTIVIRUS_REQUEST *req = (ANTIVIRUS_REQUEST*)HeapAlloc(GetProcessHeap(), 0, ANTIVIRUS_REQUEST_SIZE);
	while (TRUE) {
		if (WaitForSingleObject(ThreadingEvent, 0) == WAIT_OBJECT_0) break;

		// Read request from driver using synch/asynch calls according to https://support.microsoft.com/en-us/kb/156932
		DWORD dwBytesRead = 0;
		OVERLAPPED overlapped;
		RtlZeroMemory(&overlapped, sizeof(OVERLAPPED));
		ResetEvent(hReadFileEvent);
		overlapped.hEvent = hReadFileEvent;
		BOOL rv = ReadFile(DriverHandle, req, ANTIVIRUS_REQUEST_SIZE, &dwBytesRead, &overlapped);
		DWORD dwLastError = GetLastError();
		if (rv) {
			// SucceServiceStfully completed a synchronous read, do nothing
			if (!dwBytesRead) wprintf(L"Read zero-sized packet from driver (sync)");
		}
		else if (dwLastError == ERROR_IO_PENDING) {


			DWORD dwNumberOfBytesTransferred = 0;
			if (!GetOverlappedResult(DriverHandle, &overlapped, &dwNumberOfBytesTransferred, TRUE)) {
				dwLastError = GetLastError();
				if (dwLastError == ERROR_OPERATION_ABORTED || dwLastError == ERROR_INVALID_HANDLE) break;
				// Cancel the pending IO to ensure the IO operation does not complete after this function ends
				// and the result is stored to an invalid location
				CancelIo(DriverHandle);
				wprintf(L"GetOverlappedResult() failure in reader: %d", dwLastError);
			}
			dwLastError = GetLastError(); // Always ERROR_IO_PENDING here, even after succeServiceStful GetOverlappedResult() call.
			dwBytesRead = dwNumberOfBytesTransferred;
			if (!dwBytesRead) wprintf(L"Read zero-sized packet from driver (async)");
		}
		else if (dwLastError == ERROR_OPERATION_ABORTED || dwLastError == ERROR_INVALID_HANDLE) {
			break;
		}
		else {
			wprintf(L"Unable to read data from driver: %d ", dwLastError);
		}
		wprintf(L"Read event from driver: PID:%u Event:%u", req->PID, req->Event);

		// Validate the size of data read
		if (dwBytesRead == 0) {
			// For safety, make sure that the communications with the driver haven't failed permanently. If all reads
			// are getting zeroed, many proceServiceStes will be hung waiting for a procfilter result that will never happen.
			const DWORD dwMaxConsecutiveZeroReads = 20;
			dwNumConsecutiveZeroReads += 1;
			if (dwNumConsecutiveZeroReads >= dwMaxConsecutiveZeroReads) {
				wprintf(L"Exceeded %u consecutive zero-sized reads from driver", dwMaxConsecutiveZeroReads);
			}
			wprintf(L"Read zero-sized packet from driver");
			continue;
		}
		dwNumConsecutiveZeroReads = 0;
		if (dwBytesRead < sizeof(ANTIVIRUS_REQUEST_SIZE) || dwBytesRead > ANTIVIRUS_REQUEST_SIZE) {
			wprintf(L"Read invalid size from driver device: %u < %u || %u > %u  ReadFile:%hs Errorvode:%d",
				dwBytesRead, sizeof(ANTIVIRUS_REQUEST_SIZE), dwBytesRead, ANTIVIRUS_REQUEST_SIZE, rv ? "TRUE" : "FALSE", dwLastError);
		}
		if (dwBytesRead != req->ANTIVIRUSREQSIZE) {
			wprintf(L"Read partial packet from driver device: Read:%u PacketSize:%u", dwBytesRead, req->ANTIVIRUSREQSIZE);
		}
		// MAKE THREADING
		// Post a copy of the retrieved data to a worker thread
		wprintf(L"Posting work task to worker");
		// Allocate memory for the task data, the structure of which includes only the header portion of the procfilter request,
		// so allocate only the exact size needed
		WORKER_TASK_DATA *wtd = (WORKER_TASK_DATA*)HeapAlloc(GetProcessHeap(), 0, sizeof(WORKER_TASK_DATA) + (dwBytesRead - sizeof(ANTIVIRUS_REQUEST_SIZE)));
		if (!wtd) wprintf(L"Memory allocation failure for ProcFilter request");
		memcpy(&wtd->AVReq, req, dwBytesRead);
		wprintf(L"Posting to threadpool: PID:%u Event:%u", req->PID, req->Event);
		if (ThreadPoolPost(tp, req->Event, FALSE, ThreadingEvent, wtd)) {
			wprintf(L"Posted work task to worker");
		}
		else {
			wprintf(L"Failed to post task to worker");
			wprintf(L"Failed to post task to worker");
			HeapFree(GetProcessHeap(), 0, wtd);
		}
	}

	HeapFree(GetProcessHeap(), 0, req);

	ThreadPoolFree(tp);

	CloseHandle(hReadFileEvent);

	CloseHandle(DriverHandle);
	DriverHandle = INVALID_HANDLE_VALUE;

	return 0;
}

BOOL DriverSendResponse(HANDLE hDriver, HANDLE hWriteCompletionEvent, const ANTIVIRUS_RESPONSE *response)
{
	BOOL rv = FALSE;
	DWORD dwBytesWritten = 0;
	OVERLAPPED overlapped;
	RtlZeroMemory(&overlapped, sizeof(OVERLAPPED));
	ResetEvent(hWriteCompletionEvent);
	overlapped.hEvent = hWriteCompletionEvent;
	std::cout << "Sending data to driver with a processid of : " << response->dwProcessId << " To handle the event: " << response->EventType << '\n';
	rv = WriteFile(hDriver, response, sizeof(ANTIVIRUS_RESPONSE), &dwBytesWritten, &overlapped);
	wprintf(L"Data sent to driver");
	if (rv) {
		rv = TRUE;
	}
	else {
		DWORD dwLastError = GetLastError();
		if (dwLastError == ERROR_IO_PENDING) {
			DWORD dwBytesTransferred = 0;
			if (GetOverlappedResult(hDriver, &overlapped, &dwBytesTransferred, TRUE)) {
				dwBytesWritten = dwBytesTransferred;
				rv = TRUE;
			}
			else {
				dwLastError = GetLastError();
				if (dwLastError != ERROR_OPERATION_ABORTED && dwLastError != ERROR_INVALID_HANDLE) {
					CancelIo(hDriver);
					std::cout << "Writing data to the driver failed with getlasterror(): " << dwLastError << '\n';
				}
			}
		}
		else {
			std::cout << "Writing data to the driver failed! with getlasterror: " << dwLastError << '\n';
		}
	}

	if (rv && dwBytesWritten != sizeof(ANTIVIRUS_RESPONSE)) {
		std::cout << "Invalid data was written to the driver! Sie of data we wanted to send: " << sizeof(ANTIVIRUS_RESPONSE) << " sizeof what was sent: " << dwBytesWritten << '\n';
	}

	else {
		std::cout << "WriteFile() failed to write data to the kernel\n";
	}

	return rv;
}








