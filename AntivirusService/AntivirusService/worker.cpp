
#include <Windows.h>
#include <iostream>
#include "worker.h"
void PfWorkerStatusPrint()
{

	std::cout << "PfWokrerStatusPrint()\n";

}
void PfWorkerInit(void *lpPoolData, void *lpThreadData)
{
	WORKER_DATA *wd = (WORKER_DATA*)lpThreadData;

	wprintf(L"Worker Initializing");
	wd->hWriteCompletionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (wd->hWriteCompletionEvent == NULL) {
		wprintf(L"Unable to create write completion event in worker");
	}

}


void
PfWorkerDestroy(void *lpPoolData, void *lpThreadData)
{
	WORKER_DATA *wd = (WORKER_DATA*)lpThreadData;

	wprintf(L"Worker stopping");

	CloseHandle(wd->hWriteCompletionEvent);
}


void PfWorkerWork(void *lpPoolData, void *lpThreadData, void *lpTaskData, bool bCancel)
{
	if (bCancel) return;

	POOL_DATA *pd = (POOL_DATA*)lpPoolData;
	WORKER_DATA *wd = (WORKER_DATA*)lpThreadData;
	ANTIVIRUS_REQUEST *req = (ANTIVIRUS_REQUEST*)lpTaskData;

	HANDLE hSelf = GetCurrentThread();

	wprintf(L"Worker starting");

	// Map the event type from kernel mode to an api event type
	DWORD dwApiEventId = PROCFILTER_EVENT_NONE;
	switch (req->Event) {
	case CREATEPROCESS: dwApiEventId = PROCFILTER_EVENT_PROCESS_CREATE; break;
	case TERMINATEPROCESS: dwApiEventId = PROCFILTER_EVENT_PROCESS_TERMINATE; break;
	case CREATETHREAD: dwApiEventId = PROCFILTER_EVENT_THREAD_CREATE; break;
	case TERMINATETHREAD: dwApiEventId = PROCFILTER_EVENT_THREAD_TERMINATE; break;
	case LOADIMAGE: dwApiEventId = PROCFILTER_EVENT_IMAGE_LOAD; break;
	}
	// Build the header portion of the response
	ANTIVIRUS_RESPONSE response;
	ZeroMemory(&response, sizeof(ANTIVIRUS_RESPONSE));
	response.dwProcessId = req->PID;
	response.EventType = req->Event;
	response.dwThreadId = req->ThreadID;
	response.lpImageBase = req->ImageBase;

	// Handle the event according to its type
	if (req->Event == CREATEPROCESS) {
		Scan(req->Event, SCAN_PROCESSCREATION, pd->hSharedDriverHandle, wd->hWriteCompletionEvent, req->PID, req->ParentPID, req->lpszFileName, NULL, wd->lpvScanDataArray);

	}
	else if (req->Event == TERMINATEPROCESS) {
		Scan(req->Event, SCAN_PROCESSTERMINATION, pd->hSharedDriverHandle, wd->hWriteCompletionEvent, req->PID, 0, req->lpszFileName, NULL, wd->lpvScanDataArray);

	}
	else if (req->Event == CREATETHREAD) {

		DriverSendResponse(pd->hSharedDriverHandle, wd->hWriteCompletionEvent, &response);
	}
	else if (req->Event == TERMINATETHREAD) {

	}
	else if (req->Event == LOADIMAGE) {

	}
}