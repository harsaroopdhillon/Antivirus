#pragma once

#pragma warning(push)
#pragma warning(disable:4055)
#pragma warning(pop)
#include <ntifs.h>
#include <ntddk.h>
#include <ntstrsafe.h>
#include <wdmsec.h>
#include <wdm.h>
#include "Driver Main.h"
#include "process.h"
#include "reg.h"
#include "precomp.h"
#include "minispy.h"


PDEVICE_OBJECT g_pDeviceObject = NULL;
NDIS_STRING ServiceName = RTL_CONSTANT_STRING(FILTER_SERVICE_NAME);
NDIS_STRING UniqueName = RTL_CONSTANT_STRING(FILTER_UNIQUE_NAME);
NDIS_STRING FriendlyName = RTL_CONSTANT_STRING(FILTER_FRIENDLY_NAME);


PCALLBACK_CONTEXT CallbackCtx = NULL;
PLARGE_INTEGER Cookie = { 0 };

/*
* SDDL_DEVOBJ_SYS_ALL_ADM_ALL
* "D:P(A;;GA;;;SY)(A;;GA;;;BA)"
* SDDL_DEVOBJ_SYS_ALL_ADM_ALL allows the kernel, system, and administrator complete control over the device. No other users may access the device.
*/
DECLARE_CONST_UNICODE_STRING(xSDDL_DEVOBJ_SYS_ALL_ADM_ALL, L"D:P(A;;GA;;;SY)(A;;GA;;;BA)");


VOID UnloadDriver(IN PDRIVER_OBJECT pdo);

void CompleteReadIrp(PIRP pReadIrp, const QUEUED_READ *QueuedRead);




void InitateKernelCleanup(WDM_DEVICE_CONTROL *WDM)
{
	if (!WDM->bDeviceInUse) return;

	WDM->bDeviceInUse = FALSE;
	WDM->ServiceProcessID = 0;
	WDM->bDeviceConfigured = FALSE;
	PLIST_ENTRY entry = NULL;
	while ((entry = RemoveHeadList(&WDM->ReadList)) != &WDM->ReadList) {
		QUEUED_READ *QueuedRead = CONTAINING_RECORD(entry, QUEUED_READ, entry);
		UNREFERENCED_PARAMETER(QueuedRead);
	}
	while ((entry = RemoveHeadList(&WDM->WriteList)) != &WDM->WriteList) {
		if (WDM->nInProgress > 0) WDM->nInProgress -= 1;
		KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);
		QUEUED_WRITE *pwPendingWrite = CONTAINING_RECORD(entry, QUEUED_WRITE, entry);
		pwPendingWrite->bResponseReceived = FALSE;
		KeSetEvent(pwPendingWrite->pkeCompletionEvent, IO_NO_INCREMENT, FALSE);
		KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
	}

	PIRP pReadIrp = NULL;
	if (WDM->data->pPendingReadIrp) {
		pReadIrp = WDM->data->pPendingReadIrp;
		WDM->data->pPendingReadIrp = NULL;
		IoSetCancelRoutine(pReadIrp, NULL);
	}
	PIRP pPendingReadIrp = pReadIrp;
	WDM->nInProgress = 0;
	KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);

	if (pPendingReadIrp) {
		pPendingReadIrp->IoStatus.Information = 0;
		pPendingReadIrp->IoStatus.Status = STATUS_INVALID_HANDLE;
		IoCompleteRequest(pPendingReadIrp, IO_NO_INCREMENT);
	}

	KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
}


NTSTATUS IRP_Unknown(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Irp);

	return STATUS_NOT_SUPPORTED;
}


NTSTATUS IRP_CreateFile(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)DeviceObject->DeviceExtension;

	ULONG CallerPID = IoGetRequestorProcessId(Irp);
	
	BOOLEAN bDriverOpened = FALSE;
	KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
	if (!WDM->bDeviceInUse) {
		WDM->bDeviceInUse = TRUE;
		WDM->ServiceProcessID = CallerPID;
		bDriverOpened = TRUE;
	}
	KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);

	if (bDriverOpened) {
		KdPrint(("CreateFile() was called by: %08X\n", ServiceProcessID));
	}

	NTSTATUS rv = bDriverOpened ? STATUS_SUCCESS : STATUS_SHARING_VIOLATION;

	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = rv;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return rv;
}


NTSTATUS IRP_Close(IN PDEVICE_OBJECT DeviceObject, IN IRP* Irp)
{

	KdPrint(("Entering IRP_Close()\n"));

	WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)DeviceObject->DeviceExtension;
	KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
	InitateKernelCleanup(WDM);
	KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);

	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = STATUS_SUCCESS;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


NTSTATUS IRP_Shutdown(IN PDEVICE_OBJECT DeviceObject, IN IRP* Irp)
{

	KdPrint(("Entering IRP_Shutdown()\n"));

	WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)DeviceObject->DeviceExtension;
	KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
	InitateKernelCleanup(WDM);
	KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);

	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = STATUS_SUCCESS;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


NTSTATUS IRP_DeviceIoControl(IN PDEVICE_OBJECT DeviceObject, IN IRP* Irp)
{

	KdPrint(("Entering IRP_DeviceIoControl()\n"));

	ULONG_PTR Information = 0;
	NTSTATUS rv = STATUS_INVALID_PARAMETER;


	



	IO_STACK_LOCATION* IrpSp = IoGetCurrentIrpStackLocation(Irp);
	if (IrpSp) {
		ULONG IoControlCode = IrpSp->Parameters.DeviceIoControl.IoControlCode;

		PVOID Buffer = Irp->AssociatedIrp.SystemBuffer;
		ULONG BufferLength = IrpSp->Parameters.DeviceIoControl.OutputBufferLength;
		ULONG InputBufferLength = IrpSp->Parameters.DeviceIoControl.InputBufferLength;
		Irp->IoStatus.Information = 0;

		switch(IoControlCode) {


			
		case IOCTL_CONFIGURE:
		{
			WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)DeviceObject->DeviceExtension;

			if (Buffer && BufferLength == sizeof(ANTIVIURS_CONFIGURATION)) {
				KdPrint(("IRP_DeviceIoControl()\n"));
				KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
				BOOLEAN DeviceConfigSt = WDM->bDeviceConfigured, bCurrentConfigStatus = FALSE;
				if (!DeviceConfigSt) {
					RtlCopyMemory(WDM->config, Buffer, sizeof(ANTIVIURS_CONFIGURATION));
					if (WDM->config->dwReqSize == sizeof(ANTIVIRUS_REQUEST)) {
						if (WDM->config->dwMAXReqSize == ANTIVIRUS_REQUEST_SIZE) {
							KdPrint(("Set Up Configuration\n"));
							WDM->bDeviceConfigured = TRUE;
							bCurrentConfigStatus = TRUE;
						}
					}
				}
				KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);

				if (!DeviceConfigSt && bCurrentConfigStatus) {
					rv = STATUS_SUCCESS;
				}
				else {
					rv = STATUS_UNSUCCESSFUL;
				}
			}
		}
			break;

			case IOCTL_ADD_IPV4:
			{
				WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)DeviceObject->DeviceExtension;

				KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
				KdPrint(("Adding IPV4 Rule to firewall!\n"));
				if (InputBufferLength != sizeof(RULE_IPV4))
				{
					rv = NDIS_STATUS_FAILURE;
					//failed we still have a lock lets reelase it...

					KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);
					break;
				}
				PRULE_IPV4 pRuleIPv4 = ExAllocatePoolWithTag(NonPagedPool, sizeof(RULE_IPV4), 'Ipv4');
				RtlCopyMemory(pRuleIPv4, Buffer, InputBufferLength);
				//	AddIPv4Rule(FilterDeviceExtension, pRuleIPv4);
				KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);
			}
				break;

			case IOCTL_ADD_IPV6:
			{
				WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)DeviceObject->DeviceExtension;

				KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
				KdPrint(("Adding IPV6 Rule!\n"));
				InputBufferLength = IrpSp->Parameters.DeviceIoControl.InputBufferLength;
				if (InputBufferLength != sizeof(RULE_IPV6))
				{
					rv = NDIS_STATUS_FAILURE;
					KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);
					break;
				}
				// POOL is not freed until later!
				PRULE_IPV6 pRuleIPv6 = ExAllocatePool(NonPagedPool, sizeof(RULE_IPV6));
				RtlCopyMemory(pRuleIPv6, Buffer, InputBufferLength);
				//	AddIPv6Rule(FilterDeviceExtension, pRuleIPv6);	
				KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);
			}
				break;
				

			case IOCTL_REMOVE_IPV4:
			{
				WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)DeviceObject->DeviceExtension;

				KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
				KdPrint(("Deleting IPV4 rule to firewall!\n"));
				InputBufferLength = IrpSp->Parameters.DeviceIoControl.InputBufferLength;
				if (InputBufferLength != sizeof(ULONG))
				{
					rv = NDIS_STATUS_FAILURE;

					KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);
					break;
				}
				//				ULONG IdIPv4 = *((PULONG)(Buffer));
						//		DelIPv4Rule(FilterDeviceExtension, IdIPv4);
				KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);
			}
				break;

			case IOCTL_REMOVE_IPV6:
			{
				WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)DeviceObject->DeviceExtension;

				KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
				KdPrint(("removing IPV4\n"));
				InputBufferLength = IrpSp->Parameters.DeviceIoControl.InputBufferLength;
				if (InputBufferLength != sizeof(ULONG))
				{
					rv = NDIS_STATUS_FAILURE;

					KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);
					break;
				}
				//				ULONG IdIPv6 = *((PULONG)(Buffer));
						//		DelIPv6Rule(FilterDeviceExtension, IdIPv6);
				KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);
			}
				break;


			default:
				break;
			


		}

	}

	Irp->IoStatus.Information = Information;
	Irp->IoStatus.Status = rv;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return rv;
}

NTSTATUS IRP_Read(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	NTSTATUS rv = STATUS_SUCCESS;
	ULONG_PTR dwBytesRead = 0;

	WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)DeviceObject->DeviceExtension;

	PIO_STACK_LOCATION pIoStackIrp = IoGetCurrentIrpStackLocation(Irp);
	if (pIoStackIrp) {
		PVOID pBuffer = Irp->AssociatedIrp.SystemBuffer;
		ULONG ulBufferSize = pIoStackIrp->Parameters.Read.Length;
		//validate this is a correct buffer size.
		if (pBuffer && ulBufferSize == ANTIVIRUS_REQUEST_SIZE) {
			QUEUED_READ *QueuedRead = NULL;

			KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
			if (WDM->bDeviceInUse) {
				PLIST_ENTRY entry = RemoveHeadList(&WDM->ReadList);
				if (entry != &WDM->ReadList) {
					QueuedRead = CONTAINING_RECORD(entry, QUEUED_READ, entry);
					WDM->nInProgress += 1;
				}
				else {
					if (!WDM->data->pPendingReadIrp) { 
						IoSetCancelRoutine(Irp, CancelIoRoutine);
						IoMarkIrpPending(Irp);
						WDM->data->pPendingReadIrp = Irp;
						KdPrint(("Marking Read IRP as STATUS_PENDING\n"));
						rv = STATUS_PENDING;
					}
					else {
						KdPrint(("IRP_Read(): Occurred out of sequence\n"));
						rv = STATUS_REQUEST_OUT_OF_SEQUENCE;
					}
				}
			}
			else {
				rv = STATUS_DEVICE_NOT_CONNECTED;
			}
			KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);

			if (QueuedRead) {
				CompleteReadIrp(Irp, QueuedRead);
				return STATUS_SUCCESS;
			}
		}
		else {
			KdPrint(("WriteFile() to the Driver Received an invalid BufferLength\n"));
			rv = STATUS_INVALID_PARAMETER;
		}
	}

	if (rv != STATUS_PENDING) {
		Irp->IoStatus.Information = dwBytesRead;
		Irp->IoStatus.Status = rv;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
	}

	return rv;
}


NTSTATUS IRP_Write(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{

	KdPrint(("IRP_Write() called\n"));

	NTSTATUS rv = STATUS_SUCCESS;
	ULONG_PTR dwBytesWritten = 0;

	WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)DeviceObject->DeviceExtension;

	PIO_STACK_LOCATION pIoStackIrp = IoGetCurrentIrpStackLocation(Irp);
	if (pIoStackIrp) {

		PVOID pBuffer = Irp->AssociatedIrp.SystemBuffer;
		ULONG ulBufferSize = pIoStackIrp->Parameters.Write.Length;
		if (pBuffer && ulBufferSize == sizeof(ANTIVIRUS_RESPONSE)) {

			ANTIVIRUS_RESPONSE *response = (ANTIVIRUS_RESPONSE*)pBuffer;

			KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
			PLIST_ENTRY entry = NULL;
			BOOLEAN bDeviceInUse = WDM->bDeviceInUse;
			if (bDeviceInUse) {
				PLIST_ENTRY current = WDM->WriteList.Flink;
				ULONG i = 0;
				while (current != &WDM->WriteList && i < WDM->nInProgress) {
					QUEUED_WRITE *QueuedWrite = CONTAINING_RECORD(current, QUEUED_WRITE, entry);

					if (QueuedWrite->dwEventType == response->EventType) {
						BOOLEAN RemoveEntry = FALSE;

						if (QueuedWrite->dwEventType == CREATEPROCESS || QueuedWrite->dwEventType == TERMINATEPROCESS) {
							if (HandleToPid(QueuedWrite->hPid) == response->dwProcessId){
								RemoveEntry = TRUE;
							}
						}
						else if (QueuedWrite->dwEventType == CREATETHREAD || QueuedWrite->dwEventType == TERMINATETHREAD) {
							if (HandleToPid(QueuedWrite->hPid) == response->dwProcessId && HandleToPid(QueuedWrite->hTid) == response->dwThreadId){
								RemoveEntry = TRUE;
							}
						}
						else if (QueuedWrite->dwEventType == LOADIMAGE) {
							if (HandleToPid(QueuedWrite->hPid) == response->dwProcessId && QueuedWrite->lpImageBase == response->lpImageBase){
								RemoveEntry = TRUE;
							}
						}

						if (RemoveEntry) {
							RemoveEntryList(current);
							entry = current;
							WDM->nInProgress -= 1;
							break;
						}
					}

					current = current->Flink;
					++i;
				}
			}
			KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);

			if (!bDeviceInUse) {
				rv = STATUS_PIPE_NOT_AVAILABLE;
			}
			else if (entry) {

				QUEUED_WRITE *QueuedWrite = CONTAINING_RECORD(entry, QUEUED_WRITE, entry);
				QueuedWrite->bResponseReceived = TRUE;
				RtlCopyMemory(&QueuedWrite->response, response, sizeof(ANTIVIRUS_RESPONSE));
				KeSetEvent(QueuedWrite->pkeCompletionEvent, IO_NO_INCREMENT, FALSE);
				dwBytesWritten = sizeof(ANTIVIRUS_RESPONSE);
				KdPrint(("Completing IRP\n"));
				rv = STATUS_SUCCESS;
			}
			else {
				KdPrint(("IPR_WRITE(), NTSTATUS: STATUS_REQUEST_OUT_OF_SEQUENCE\n"));
				KdBreakPoint();
				rv = STATUS_REQUEST_OUT_OF_SEQUENCE;
			}
		}
		else {
			KdPrint(("IRP_Write() failed!\n"));
			rv = STATUS_INVALID_PARAMETER;
		}
	}

	Irp->IoStatus.Information = dwBytesWritten;
	Irp->IoStatus.Status = rv;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return rv;
}



VOID CancelIoRoutine(PDEVICE_OBJECT pdo, PIRP Irp)
{
	KdPrint(("Entering CancelIoRoutine()\n"));
	KIRQL cancelIrql = Irp->CancelIrql;
	IoReleaseCancelSpinLock(cancelIrql);

	WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)pdo->DeviceExtension;
	KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
	if (WDM->data->pPendingReadIrp == Irp) {
		WDM->data->pPendingReadIrp = NULL;
	}
	KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);
	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = STATUS_CANCELLED;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);
}





NTSTATUS DriverEntry(IN PDRIVER_OBJECT pdo, IN PUNICODE_STRING Registry)
{

	UNREFERENCED_PARAMETER(Registry);
	

	SECURITY_DESCRIPTOR *sd = { 0 };
	OBJECT_ATTRIBUTES oa = { 0 };

	UNICODE_STRING Device;
	UNICODE_STRING SymbolicDeviceName;
	UNICODE_STRING FileSystemName;

	RtlInitUnicodeString(&Device, _DEVICE_NAME);
	RtlInitUnicodeString(&SymbolicDeviceName, _DOS_DEVICE_NAME);
	RtlInitUnicodeString(&FileSystemName, MINISPY_PORT_NAME);
	InitializeObjectAttributes(&oa,	&FileSystemName, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, sd);

	DEVICE_OBJECT *DeviceObj = NULL;
	NTSTATUS rv = STATUS_UNSUCCESSFUL;

	rv = WdmlibIoCreateDeviceSecure(pdo, sizeof(WDM_DEVICE_CONTROL), &Device, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &xSDDL_DEVOBJ_SYS_ALL_ADM_ALL, NULL, &DeviceObj);
	if (rv != STATUS_SUCCESS) {
		KdPrint(("WdmlibIoCreateDeviceSecure() Failed!\n"));
		return rv;
	}


	rv = IoCreateSymbolicLink(&SymbolicDeviceName, &Device);
	if (rv != STATUS_SUCCESS) {
		KdPrint(("IoCreateSymbolicLink() Failed\n"));
		IoDeleteDevice(DeviceObj);
		return rv;
	}

	WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)DeviceObj->DeviceExtension;
	
	RtlSecureZeroMemory(WDM, sizeof(WDM_DEVICE_CONTROL));
	InitializeListHead(&WDM->WriteList);
	InitializeListHead(&WDM->ReadList);
	InitializeListHead(&g_CallbackCtxListHead);

	KeInitializeSpinLock(&WDM->data->Lock);

	__try {

		for (SIZE_T i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; ++i) {
			pdo->MajorFunction[i] = IRP_Unknown;
		}

		KdPrint(("Registering driver to recieve IRPs!\n"));
		pdo->DriverUnload = UnloadDriver;
		pdo->MajorFunction[IRP_MJ_CREATE] = IRP_CreateFile;
		pdo->MajorFunction[IRP_MJ_CLOSE] = IRP_Close;
		pdo->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IRP_DeviceIoControl;
		pdo->MajorFunction[IRP_MJ_READ] = IRP_Read;
		pdo->MajorFunction[IRP_MJ_WRITE] = IRP_Write;
		pdo->MajorFunction[IRP_MJ_SHUTDOWN] = IRP_Shutdown;
		rv = IoRegisterShutdownNotification(DeviceObj);
		if(rv != STATUS_SUCCESS){
			KdPrint(("IoRegisterShutdownNotification() failed!\n"));
		}

		DeviceObj->Flags |= DO_BUFFERED_IO;
		
		if ((rv = CmRegisterCallbackEx(Callback, (PUNICODE_STRING)L"380000", pdo, CallbackCtx, Cookie, 0)) != STATUS_SUCCESS) {

		}
		else if ((rv = PsSetCreateProcessNotifyRoutineEx(ProcessCreationCallBack, FALSE)) != STATUS_SUCCESS) {
				KdPrint(("PsSetCreateProcessNotifyRoutineEx() failed\n"));
		}
		/*else if ((rv = ObRegisterCallbacks(callbackreg, reghandle)) != STATUS_SUCCESS) {
			PsSetCreateProcessNotifyRoutineEx(ProcessCreationCallBack, TRUE);
			KdPrint(("Failed to protect the driver and process!!!!\n"));
			
		}
		*/
		
		else if ((rv = PsSetCreateThreadNotifyRoutine(ThreadCreationCallBack)) != STATUS_SUCCESS) {
			
			//ObUnRegisterCallbacks(reghandle);
			KdPrint(("PsSetCreateThreadNotifyRoutine() failed! we will now try to unregister the previous calls.\n"));
			if(PsSetCreateProcessNotifyRoutineEx(ProcessCreationCallBack, TRUE) != STATUS_SUCCESS){
				KdPrint(("Unregistering PsSetCreateProcessNotifyRoutineEx() failed! \n"));
			}
		}

		else if ((rv = PsSetLoadImageNotifyRoutine(ImageCreationCallBack)) != STATUS_SUCCESS) {

			KdPrint(("PsSetLoadImageNotifyRoutine() failed! we will now try to unregister the previous calls.\n"));
			if(PsSetCreateProcessNotifyRoutineEx(ProcessCreationCallBack, TRUE) != STATUS_SUCCESS){
				KdPrint(("Unregistering PsSetCreateProcessNotifyRoutineEx() failed! \n"));
			}
			if(PsRemoveCreateThreadNotifyRoutine(ThreadCreationCallBack) != STATUS_SUCCESS){
				KdPrint(("PsRemoveCreateThreadNotifyRoutine() failed!\n"));
			}
		}

		else {
			KdPrint(("Added create process, create thread, and image load notification routines\n"));
			rv = STATUS_SUCCESS;
		}

			KdPrint(("Attempting to intialize File system filter!\n"));
			MiniSpyData.LogSequenceNumber = 0;
			MiniSpyData.RecordsAllocated = 0;
			MiniSpyData.NameQueryMethod = FLT_FILE_NAME_QUERY_ALWAYS_ALLOW_CACHE_LOOKUP;

			InitializeListHead(&MiniSpyData.OutputBufferList);
			KeInitializeSpinLock(&MiniSpyData.OutputBufferLock);

			ExInitializeNPagedLookasideList(&MiniSpyData.FreeBufferList, NULL, NULL, POOL_NX_ALLOCATION, RECORD_SIZE, SPY_TAG, 0);

			if ((rv = FltRegisterFilter(pdo, &FilterRegistration, &MiniSpyData.Filter)) != STATUS_SUCCESS) {
				KdPrint(("FltRegisterFilter() failed! we will now try to unregister the previous calls.\n"));
				if(PsSetCreateProcessNotifyRoutineEx(ProcessCreationCallBack, TRUE) != STATUS_SUCCESS){
					KdPrint(("Unregistering PsSetCreateProcessNotifyRoutineEx() failed! \n"));
				}
				if(PsRemoveCreateThreadNotifyRoutine(ThreadCreationCallBack) != STATUS_SUCCESS){
					KdPrint(("PsRemoveCreateThreadNotifyRoutine() failed!\n"));
				}
			}


			if ((rv = FltBuildDefaultSecurityDescriptor(&sd, FLT_PORT_ALL_ACCESS)) != STATUS_SUCCESS) {
				KdPrint(("FltBuildDefaultSecurityDescriptor() Failed\n"));
				return rv;
			}




	if(FltCreateCommunicationPort(MiniSpyData.Filter,	&MiniSpyData.ServerPort, &oa, NULL, SpyConnect, SpyDisconnect, SpyMessage, 1) != STATUS_SUCCESS){
				KdPrint(("FltCreateCommunicationPort() failed\n"));

			}

			FltFreeSecurityDescriptor(sd);

			if ((rv = FltStartFiltering(MiniSpyData.Filter)) != STATUS_SUCCESS) {
				KdPrint(("FltCreateCommunicationPort() Failed\n"));
				return rv;
			}


		
			ExInitializeFastMutex(&g_CallbackCtxListLock);
			g_NumCallbackCtxListEntries = 0;








			NdisZeroMemory(&FChars, sizeof(NDIS_FILTER_DRIVER_CHARACTERISTICS));
			FChars.Header.Type = NDIS_OBJECT_TYPE_FILTER_DRIVER_CHARACTERISTICS;
			FChars.Header.Size = sizeof(NDIS_FILTER_DRIVER_CHARACTERISTICS);
#if NDIS_SUPPORT_NDIS61
			FChars.Header.Revision = NDIS_FILTER_CHARACTERISTICS_REVISION_2;
#else
			FChars.Header.Revision = NDIS_FILTER_CHARACTERISTICS_REVISION_1;
#endif
			FChars.MajorNdisVersion = FILTER_MAJOR_NDIS_VERSION;
			FChars.MinorNdisVersion = FILTER_MINOR_NDIS_VERSION;
			FChars.MajorDriverVersion = 1;
			FChars.MinorDriverVersion = 0;
			FChars.Flags = 0;

			FChars.FriendlyName = FriendlyName;
			FChars.UniqueName = UniqueName;
			FChars.ServiceName = ServiceName;

			FChars.SetOptionsHandler = FilterRegisterOptions;
			FChars.AttachHandler = FilterAttach;
			FChars.DetachHandler = FilterDetach;
			FChars.RestartHandler = FilterRestart;
			FChars.PauseHandler = FilterPause;
			FChars.SetFilterModuleOptionsHandler = FilterSetModuleOptions;
			FChars.OidRequestHandler = FilterOidRequest;
			FChars.OidRequestCompleteHandler = FilterOidRequestComplete;
			FChars.CancelOidRequestHandler = FilterCancelOidRequest;

			FChars.SendNetBufferListsHandler = FilterSendNetBufferLists;
			FChars.ReturnNetBufferListsHandler = FilterReturnNetBufferLists;
			FChars.SendNetBufferListsCompleteHandler = FilterSendNetBufferListsComplete;
			FChars.ReceiveNetBufferListsHandler = FilterReceiveNetBufferLists;
			FChars.DevicePnPEventNotifyHandler = FilterDevicePnPEventNotify;
			FChars.NetPnPEventHandler = FilterNetPnPEvent;
			FChars.StatusHandler = FilterStatus;
			FChars.CancelSendNetBufferListsHandler = FilterCancelSendNetBufferLists;


			FilterDriverHandle = NULL;

			NdisAllocateSpinLock(&FilterListLock);

			InitializeListHead(&FilterModuleList);

/*			if ((rv = NdisRegisterFilterDriver(pdo, &FChars, &FilterDriverHandle)) != NDIS_STATUS_SUCCESS)
			{
				KdPrint(("Register filter driver failed.\n"));
				return rv;
			}
			*/
			rv = NDISFilterDriverRegisterDevice(&pFilterDeviceExtension, DeviceObj);

			if (rv != NDIS_STATUS_SUCCESS)
			{
//				NdisDeregisterFilterDriver(FilterDriverHandle);
				NdisFreeSpinLock(&FilterListLock);
				KdPrint(("Register device for the filter driver failed.\n"));
				return rv;
			}



	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		KdPrint(("Exception in DriverEntry()\n"));
		rv = STATUS_NONCONTINUABLE_EXCEPTION;
	}

	if (rv == STATUS_SUCCESS) {
		g_pDeviceObject = DeviceObj;
	}
	else {
		UNREFERENCED_PARAMETER(WDM);
		IoDeleteSymbolicLink(&SymbolicDeviceName);
		IoDeleteDevice(DeviceObj);
	}

	return rv;
}


_Use_decl_annotations_
VOID UnloadDriver(IN PDRIVER_OBJECT DriverObject)
{
	KdPrint(("DriverUnload()\n"));

	WDM_DEVICE_CONTROL *WDM = (WDM_DEVICE_CONTROL*)DriverObject->DeviceObject->DeviceExtension;
	KeAcquireSpinLock(&WDM->data->Lock, &WDM->data->IRQLevel);
	InitateKernelCleanup(WDM);
	KeReleaseSpinLock(&WDM->data->Lock, WDM->data->IRQLevel);

	if (PsSetCreateProcessNotifyRoutineEx(ProcessCreationCallBack, TRUE) == STATUS_SUCCESS) {

		KdPrint(("Unregistering PsSetCreateProcessNotifyRoutineEx()\n"));
	}
	else {
		KdPrint(("failed to Unregister PsSetCreateProcessNotifyRoutineEx()\n"));
	}

	if (PsRemoveCreateThreadNotifyRoutine(ThreadCreationCallBack) == STATUS_SUCCESS) {
		KdPrint(("Unregistering PsRemoveCreateThreadNotifyRoutine()\n"));
	}
	else {
		KdPrint(("failed to Unregister PsRemoveCreateThreadNotifyRoutine()\n"));
	}

	
	if (PsRemoveLoadImageNotifyRoutine(ImageCreationCallBack) == STATUS_SUCCESS) {
		KdPrint(("unregistering PsRemoveLoadImageNotifyRoutine()\n"));
	}
	else {
		KdPrint(("failed to unregister PsRemoveLoadImageNotifyRoutine()\n"));
	}

	
	if (NULL != MiniSpyData.ServerPort) {
		FltCloseCommunicationPort(MiniSpyData.ServerPort);
		KdPrint(("Closing File System Filter\n"));
	}

	if (NULL != MiniSpyData.Filter) {
		FltUnregisterFilter(MiniSpyData.Filter);
		KdPrint(("Unregistering File System Filter\n"));
	}

	SpyEmptyOutputBufferList();

	ExDeleteNPagedLookasideList(&MiniSpyData.FreeBufferList);
	

		
	NdisFDeregisterFilterDriver(FilterDriverHandle);
	NdisFreeSpinLock(&FilterListLock);


	UNREFERENCED_PARAMETER(WDM);

	UNICODE_STRING SymbolicDeviceName;
	RtlInitUnicodeString(&SymbolicDeviceName, _DOS_DEVICE_NAME);
	IoDeleteSymbolicLink(&SymbolicDeviceName);

	if (DriverObject->DeviceObject) {
		KdPrint(("IoDeleteDevice()\n"));
		IoDeleteDevice(DriverObject->DeviceObject);
	}

	KdPrint(("Unloading the Driver\n"));
}



