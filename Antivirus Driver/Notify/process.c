
#include <ntifs.h>
#include "Driver Main.h"
#include "process.h"
#include <ntddk.h>
#include <wdm.h>
#include <ntstrsafe.h>
#include <wdmsec.h>
extern PDEVICE_OBJECT g_pDeviceObject;
DWORD HandleToPid(HANDLE hPid)
{
	return ((ULONG_PTR)hPid) & -1;
}


BOOLEAN Event(DWORD EventType, PCUNICODE_STRING ImageFileName, HANDLE Pid, HANDLE ParentPid, HANDLE ThreadId, void *lpImageBase, ANTIVIRUS_RESPONSE *o_response)
{
	KdPrint(("Entering Event()\n"));

	BOOLEAN rv = FALSE;
	WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)g_pDeviceObject->DeviceExtension;

	QUEUED_READ QueuedRead = { 0 };
	QueuedRead.request.dwEventType = EventType;
	QueuedRead.request.dwProcessId = HandleToPid(Pid);
	QueuedRead.request.dwThreadId = HandleToPid(ThreadId);
	QueuedRead.request.lpImageBase = lpImageBase;
	QueuedRead.request.dwParentProcessId = HandleToPid(ParentPid);
	QueuedRead.request.szFileName[0] = L'\0';
	QueuedRead.pImageFileName = ImageFileName;

	QUEUED_WRITE QueuedWrite = { 0 };

	KEVENT CompleteWriteEvent;
	KeInitializeEvent(&CompleteWriteEvent, NotificationEvent, FALSE);
	QueuedWrite.pkeCompletionEvent = &CompleteWriteEvent;
	QueuedWrite.dwEventType = EventType;
	QueuedWrite.hPid = Pid;
	QueuedWrite.hTid = ThreadId;
	QueuedWrite.lpImageBase = lpImageBase;

	KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
	IRP *pReadIrp = NULL;
	BOOLEAN bDeviceInUse = WDM->bDeviceInUse;
	ULONG nInProgress = 0;
	if (bDeviceInUse) {
		PIRP ReadIrp = NULL;
		if (WDM->data->pPendingReadIrp) {
			ReadIrp = WDM->data->pPendingReadIrp;
			WDM->data->pPendingReadIrp = NULL;
			IoSetCancelRoutine(ReadIrp, NULL);
		}
		pReadIrp = ReadIrp;
		if (pReadIrp) {
			WDM->nInProgress += 1;
			nInProgress = WDM->nInProgress;
		}
		else {
			InsertTailList(&WDM->ReadList, &QueuedRead.entry);
		}

		InsertTailList(&WDM->WriteList, &QueuedWrite.entry);
	}
	KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);

	if (pReadIrp) {
		KdPrint(("CompleteReadIrp()\n"));
		CompleteReadIrp(pReadIrp, &QueuedRead);
	}

	if (!bDeviceInUse) {
		KdPrint(("Device is not in use\n"));
		return rv;
	}


	KdPrint(("Waiting on user-land write: %u in progress in userspace...\n", nInProgress));
	KeWaitForSingleObject(&CompleteWriteEvent, Executive, KernelMode, FALSE, NULL);
	KdPrint(("Received user-land write\n"));

	if (QueuedWrite.bResponseReceived) {
		if (o_response) RtlCopyMemory(o_response, &QueuedWrite.response, sizeof(ANTIVIRUS_RESPONSE));

		rv = TRUE;
	}

	return rv;
}



















VOID ProcessCreationCallBack(_Inout_ PEPROCESS Process, _In_ HANDLE ProcessId, _In_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo)
{
	UNREFERENCED_PARAMETER(Process);

	WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)g_pDeviceObject->DeviceExtension;
	KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
	BOOLEAN bDeviceInUse = WDM->bDeviceInUse;
	HANDLE hDeviceOwnerPid = (HANDLE)WDM->ServiceProcessID;
	BOOLEAN bDeviceConfigured = WDM->bDeviceConfigured;
	KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);
	if (!bDeviceInUse) {
		KdPrint(("Process 0x%p will not be scanned! There is no service in use!\n", ProcessId));
		return;
	}
	else if (!bDeviceConfigured) {
		KdPrint(("No Device is in use yet!\n"));
		return;
	}
	else if (hDeviceOwnerPid == ProcessId || (CreateInfo && hDeviceOwnerPid != NULL && hDeviceOwnerPid == CreateInfo->CreatingThreadId.UniqueProcess)) {
		KdPrint(("This is our own process.. or a process created from our process! We will not scan!\n"))	
		return;
	}

	PUNICODE_STRING AllocatedImageFileName = NULL;


	BOOLEAN allocprocessimagename = FALSE;
	PUNICODE_STRING FileNImage = NULL;
	PEPROCESS process = NULL;
	if (NT_SUCCESS(PsLookupProcessByProcessId(ProcessId, &process))) {
		if (NT_SUCCESS(SeLocateProcessImageName(process, &FileNImage))) {
			AllocatedImageFileName = FileNImage;
			allocprocessimagename = TRUE;
		}
		ObDereferenceObject(process);
	}

	if (!allocprocessimagename) {
		KdPrint(("SeLocateProcessImageName() failed!\n"));
		return;
	}
	PCUNICODE_STRING ImageFileName = AllocatedImageFileName;

	if (CreateInfo) { 
		KdPrint(("Process is being created With a process ID of : %p  Called: %wZ\n", ProcessId, ImageFileName));
		ANTIVIRUS_RESPONSE response;
		RtlZeroMemory(&response, sizeof(ANTIVIRUS_RESPONSE));
		if (Event(CREATEPROCESS, ImageFileName, ProcessId, CreateInfo->ParentProcessId, NULL, NULL, &response)) {
			KdPrint(("New process scanned: %p %wZ\n", ProcessId, ImageFileName));
			if (response.bBlock) {
				// virus!
				CreateInfo->CreationStatus = STATUS_VIRUS_INFECTED;
			}
		}
		else {
			KdPrint(("Scanning process id of %p failed!\n", ProcessId));
			if (WDM->config->bDenyProcessCreationOnFailedScan) {
				CreateInfo->CreationStatus = STATUS_UNSUCCESSFUL;
			}
		}
	}
	else { 
		KdPrint(("Process exiting: 0x%p %wZ\n", ProcessId, ImageFileName));

		if (Event(TERMINATEPROCESS, ImageFileName, ProcessId, NULL, NULL, NULL, NULL)) {
			KdPrint(("Terminating process scanned: 0x%p %wZ\n", ProcessId, ImageFileName));
		}
		else {
			KdPrint(("Terminating process scan failed: 0x%p %wZ\n", ProcessId, ImageFileName));
		}
	}
	KdPrint(("End of Process Callback!\n"));
	
	ExFreePool(AllocatedImageFileName);
}

VOID ThreadCreationCallBack(IN HANDLE ProcessId, IN HANDLE ThreadId, IN BOOLEAN Create)
{
	WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)g_pDeviceObject->DeviceExtension;
	KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
	BOOLEAN bDeviceInUse = WDM->bDeviceInUse;
	HANDLE hDeviceOwnerPid = (HANDLE)WDM->ServiceProcessID;
	BOOLEAN bDeviceConfigured = WDM->bDeviceConfigured;
	KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);
	if (!bDeviceInUse) {
		KdPrint(("Process 0x%p will not be scanned! There is no service in use!\n", ProcessId));
		return;
	}
	else if (!bDeviceConfigured) {
		KdPrint(("No Device is in use yet!\n"));
		return;
	}
	else if (hDeviceOwnerPid == ProcessId || !ProcessId) {
		KdPrint(("This is a thread created in our own process...Or it possibly is a system thread creation... We will not scan!\n"))	
		return;
	}

	Event(Create ? CREATETHREAD : TERMINATETHREAD, NULL, ProcessId, Create ? PsGetCurrentProcessId() : NULL, ThreadId, NULL, NULL);
}


VOID ImageCreationCallBack(__in_opt PUNICODE_STRING PartialImageName, __in HANDLE  ProcessId, __in PIMAGE_INFO  ImageInfo)
{
	KdPrint(("ImageCreationCallBack() entered\n"));

	WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)g_pDeviceObject->DeviceExtension;
	KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
	BOOLEAN bDeviceInUse = WDM->bDeviceInUse;
	HANDLE hDeviceOwnerPid = (HANDLE)WDM->ServiceProcessID;
	BOOLEAN bDeviceConfigured = WDM->bDeviceConfigured;
	BOOLEAN bWantImageLoadEvents = WDM->config->bWantImageLoadEvents;
	KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);
	if (!bDeviceInUse) {
		KdPrint(("Not processing image load in process 0x%p; user-land process is not using this driver (is the ProcFilter service running?)\n", ProcessId));
		return;
	}
	else if (!ProcessId) {
		KdPrint(("Ignoring image load event for kernel module\n"));
		return;
	}
	else if (!bDeviceConfigured) {
		KdPrint(("Ignoring image load event; device not yet configured\n"));
		return;
	}
	else if (hDeviceOwnerPid == ProcessId) {
		KdPrint(("Ignoring image load event related to the process that has the YARA scan device open (0x%p)\n", ProcessId));
		return;
	}
	else if (!bWantImageLoadEvents) {
		return;
	}


	BOOLEAN bTerminateProcess = FALSE;
	ANTIVIRUS_RESPONSE response;
	KdPrint(("OnLoadImage(): IRQL:%d ApcsDisabled:%d AllApcsDisabled:%d\n", KeGetCurrentIrql(), KeAreApcsDisabled(), KeAreAllApcsDisabled()));
	if (!KeAreAllApcsDisabled() && ImageInfo->ExtendedInfoPresent) {
		void *HeapPointer = NULL;
		IMAGE_INFO_EX *ImageInfoEx = CONTAINING_RECORD(ImageInfo, IMAGE_INFO_EX, ImageInfo);
		if (ImageInfoEx->FileObject) {
			unsigned char StackBuffer[1024];
			ULONG Length = sizeof(StackBuffer);
			OBJECT_NAME_INFORMATION *pObjectNameInfo = (OBJECT_NAME_INFORMATION*)StackBuffer;
			KdPrint(("Calling ObQueryNameString(): IRQL: %d\n", KeGetCurrentIrql()));

			NTSTATUS rc = ObQueryNameString(ImageInfoEx->FileObject, pObjectNameInfo, Length, &Length);
			if (rc == STATUS_INFO_LENGTH_MISMATCH && Length > 0) {
				KdPrint(("Allocating heap memory\n"));
				HeapPointer = ExAllocatePoolWithTag(NonPagedPool, Length, MEMORY_TAG1);
				if (HeapPointer) {
					pObjectNameInfo = (OBJECT_NAME_INFORMATION*)HeapPointer;
					rc = ObQueryNameString(ImageInfoEx->FileObject, pObjectNameInfo, Length, &Length);
				}
			}

			if (rc == STATUS_SUCCESS && pObjectNameInfo->Name.Buffer) {
				KdPrint(("Sending Image load event to userspace\n"));
				bTerminateProcess = Event(LOADIMAGE, &pObjectNameInfo->Name, ProcessId, NULL, NULL, ImageInfo->ImageBase, &response) && response.bBlock;
				KdPrint(("Userspace image load event returned\n"));
			}
		}
	}
	else if (PartialImageName) {
		// Generate an event that only includes the partial file name
		bTerminateProcess = Event(LOADIMAGE, PartialImageName, ProcessId, NULL, NULL, ImageInfo->ImageBase, &response) && response.bBlock;
	}

	if (bTerminateProcess) ZwTerminateProcess(ProcessId, STATUS_VIRUS_INFECTED);

	KdPrint(("OnLoadImage() exited\n"));
}


void CompleteReadIrp(PIRP pReadIrp, const QUEUED_READ *QueuedRead)
{
	ANTIVIRUS_REQUEST *AntivirusRequest = (ANTIVIRUS_REQUEST*)pReadIrp->AssociatedIrp.SystemBuffer;
	RtlCopyMemory(AntivirusRequest, &QueuedRead->request, sizeof(ANTIVIRUS_REQUEST));
	ULONG uAntivirusRequest = sizeof(ANTIVIRUS_REQUEST);
	if (QueuedRead->pImageFileName && QueuedRead->pImageFileName->Buffer) {
		ULONG dwExtraCharacters = 0;
		RtlCopyMemory(&AntivirusRequest->szFileName[dwExtraCharacters], L"\\\\?\\GLOBALROOT", 14 * sizeof(WCHAR));
		dwExtraCharacters += 14;
		RtlCopyMemory(&AntivirusRequest->szFileName[dwExtraCharacters], QueuedRead->pImageFileName->Buffer, QueuedRead->pImageFileName->Length);
		dwExtraCharacters += QueuedRead->pImageFileName->Length / sizeof(WCHAR);
		AntivirusRequest->szFileName[dwExtraCharacters] = L'\0';

		uAntivirusRequest += dwExtraCharacters * sizeof(WCHAR);
	}
	AntivirusRequest->dwRequestSize = uAntivirusRequest;

	KdPrint(("Completing Read Request! Sending %u bytes\n", uAntivirusRequest));

	if (uAntivirusRequest < sizeof(ANTIVIRUS_REQUEST)) {
		KeBugCheck(DRIVER_VIOLATION);
	}
	if (uAntivirusRequest > ANTIVIRUS_REQUEST_SIZE) {
		KeBugCheck(DRIVER_OVERRAN_STACK_BUFFER);
	}
	pReadIrp->IoStatus.Information = uAntivirusRequest;
	pReadIrp->IoStatus.Status = STATUS_SUCCESS;

	IoCompleteRequest(pReadIrp, IO_NO_INCREMENT);
}


