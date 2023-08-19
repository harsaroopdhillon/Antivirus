#pragma comment(lib, "wdm.lib")
#pragma warning(suppress: 6262)

#include <initguid.h>
#include "filesys.h"
#include <ntifs.h>
#include <ntstrsafe.h>
#include <wsk.h>


CONST FLT_OPERATION_REGISTRATION Callbacks[] = {
	{ IRP_MJ_CREATE, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_CREATE_NAMED_PIPE, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_CLOSE, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_READ, 0, SpyPreOperationCallback,0 },
	{ IRP_MJ_WRITE, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_QUERY_INFORMATION, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_SET_INFORMATION, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_QUERY_EA, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_SET_EA, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_FLUSH_BUFFERS, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_QUERY_VOLUME_INFORMATION, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_SET_VOLUME_INFORMATION, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_DIRECTORY_CONTROL, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_FILE_SYSTEM_CONTROL, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_DEVICE_CONTROL, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_INTERNAL_DEVICE_CONTROL, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_LOCK_CONTROL, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_CLEANUP, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_CREATE_MAILSLOT, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_QUERY_SECURITY, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_SET_SECURITY, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_QUERY_QUOTA, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_SET_QUOTA, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_PNP, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_ACQUIRE_FOR_SECTION_SYNCHRONIZATION, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_RELEASE_FOR_SECTION_SYNCHRONIZATION, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_ACQUIRE_FOR_MOD_WRITE, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_RELEASE_FOR_MOD_WRITE, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_ACQUIRE_FOR_CC_FLUSH, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_RELEASE_FOR_CC_FLUSH, 0, SpyPreOperationCallback, 0 },

	{ IRP_MJ_FAST_IO_CHECK_IF_POSSIBLE, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_NETWORK_QUERY_OPEN, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_MDL_READ, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_MDL_READ_COMPLETE, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_PREPARE_MDL_WRITE, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_MDL_WRITE_COMPLETE, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_VOLUME_MOUNT, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_VOLUME_DISMOUNT, 0, SpyPreOperationCallback, 0 },
	{ IRP_MJ_OPERATION_END }
};

CONST FLT_REGISTRATION FilterRegistration = {

	sizeof(FLT_REGISTRATION),
	FLT_REGISTRATION_VERSION,
#if (NTDDI_VERSION >= NTDDI_WIN8) 
	FLTFL_REGISTRATION_SUPPORT_NPFS_MSFS,
#else
	0,
#endif 

	0,
	Callbacks,

	NULL,

	NULL,
	SpyQueryTeardown,
	NULL,
	NULL,

	NULL,
	NULL,
	NULL,



	0

};

PRECORD_LIST
SpyAllocateBuffer(
	_Out_ PULONG RecordType
) {
	PVOID newBuffer;
	ULONG newRecordType = RECORD_TYPE_NORMAL;

	//
	//  See if we have room to allocate more buffers
	//

	if (MiniSpyData.RecordsAllocated < MiniSpyData.MaxRecordsToAllocate) {

		InterlockedIncrement(&MiniSpyData.RecordsAllocated);

		newBuffer = ExAllocateFromNPagedLookasideList(&MiniSpyData.FreeBufferList);

		if (newBuffer == NULL) {

			//
			//  We failed to allocate the memory.  Decrement our global count
			//  and return what type of memory we have.
			//

			InterlockedDecrement(&MiniSpyData.RecordsAllocated);

			newRecordType = RECORD_TYPE_FLAG_OUT_OF_MEMORY;
		}

	}
	else {

		//
		//  No more room to allocate memory, return we didn't get a buffer
		//  and why.
		//

		newRecordType = RECORD_TYPE_FLAG_EXCEED_MEMORY_ALLOWANCE;
		newBuffer = NULL;
	}

	*RecordType = newRecordType;
	return newBuffer;
}
PRECORD_LIST SpyNewRecord(VOID) {
	ULONG initialRecordType = 0;
	PRECORD_LIST newRecord = SpyAllocateBuffer(&initialRecordType);
	//VOID* newBuffer = NULL;
	ULONG newRecordType = RECORD_TYPE_NORMAL;
	if (MiniSpyData.RecordsAllocated < MiniSpyData.MaxRecordsToAllocate) {
		InterlockedIncrement(&MiniSpyData.RecordsAllocated);
		newRecord = ExAllocateFromNPagedLookasideList(&MiniSpyData.FreeBufferList);
		if (newRecord == NULL) {
			InterlockedDecrement(&MiniSpyData.RecordsAllocated);
			newRecordType = RECORD_TYPE_FLAG_OUT_OF_MEMORY;
		}
	}
	else {
		newRecordType = RECORD_TYPE_FLAG_EXCEED_MEMORY_ALLOWANCE;
		newRecord = NULL;
	}
	if (newRecord == NULL) {
		if (!InterlockedExchange(&MiniSpyData.StaticBufferInUse, TRUE)) {
			newRecord = (PRECORD_LIST)MiniSpyData.OutOfMemoryBuffer;
			initialRecordType |= RECORD_TYPE_FLAG_STATIC;
		}
	}
	if (newRecord != NULL) {
		newRecord->LogRecord.RecordType = initialRecordType;
		newRecord->LogRecord.Length = sizeof(LOG_RECORD);
		newRecord->LogRecord.SequenceNumber = InterlockedIncrement(&MiniSpyData.LogSequenceNumber);
		RtlZeroMemory(&newRecord->LogRecord.Data, sizeof(RECORD_DATA));
	}
	return newRecord;
}


VOID SpyFreeRecord(_In_ PRECORD_LIST Record)
{
	if (FlagOn(Record->LogRecord.RecordType, RECORD_TYPE_FLAG_STATIC)) {
		NT_ASSERT(MiniSpyData.StaticBufferInUse);
		MiniSpyData.StaticBufferInUse = FALSE;
	}
	else {
		InterlockedDecrement(&MiniSpyData.RecordsAllocated);
		ExFreeToNPagedLookasideList(&MiniSpyData.FreeBufferList, Record);
	}
}


VOID SpyBuildEcpDataString(_In_ PRECORD_LIST RecordList, _Inout_ PUNICODE_STRING EcpData, _In_reads_(NumKnownEcps) PVOID * ContextPointers)
{
	ULONG knownCount = 0;
	SHORT wcharsCopied = 0;
	PRECORD_DATA recordData = &RecordList->LogRecord.Data;
	PWCHAR printPointer = EcpData->Buffer;
	TCHAR addressBuffer[ADDRESS_STRING_BUFFER_SIZE];
	ULONG addressBufferLen;
	LONG addressConvStatus;
	PAGED_CODE();
	NT_ASSERT(NULL != ContextPointers);

	wcharsCopied = (SHORT)RtlStringCchPrintfW(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"[%d ECPs:", recordData->EcpCount);

	if (recordData->KnownEcpMask & ECP_TYPE_FLAG_OPLOCK_KEY) {
		POPLOCK_KEY_ECP_CONTEXT oplockEcpContext = NULL;
		LPGUID oplockKeyGuid;
		UNICODE_STRING oplockKeyGuidString;
		knownCount++;
		oplockEcpContext = (POPLOCK_KEY_ECP_CONTEXT)ContextPointers[EcpOplockKey];
		NT_ASSERT(NULL != oplockEcpContext);
		oplockKeyGuid = &oplockEcpContext->OplockKey;
		if (NT_SUCCESS(RtlStringFromGUID(oplockKeyGuid,
			&oplockKeyGuidString))) {
			wcharsCopied = (SHORT)RtlStringCchPrintfW(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"%s OPLOCK KEY: %wZ;", printPointer, &oplockKeyGuidString);
			RtlFreeUnicodeString(&oplockKeyGuidString);
		}
		else {
			wcharsCopied = (SHORT)RtlStringCchPrintfW(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"%s INVALID OPLOCK KEY;", printPointer);
		}
	}


	if (recordData->KnownEcpMask & ECP_TYPE_FLAG_NFS) {
		PNFS_OPEN_ECP_CONTEXT nfsEcpContext = NULL;
		PUNICODE_STRING nfsShareNameString;
		PSOCKADDR_STORAGE_NFS nfsClientSocketAddr;

		knownCount++;

		nfsEcpContext = (PNFS_OPEN_ECP_CONTEXT)ContextPointers[EcpNfsOpen];

		NT_ASSERT(NULL != nfsEcpContext);

		nfsShareNameString = nfsEcpContext->ExportAlias;
		nfsClientSocketAddr = nfsEcpContext->ClientSocketAddress;


		if (nfsShareNameString) {

			wcharsCopied = (SHORT)RtlStringCchPrintfW(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"%s NFS SHARE NAME: %wZ,", printPointer, nfsShareNameString);
		}

		NT_ASSERT(nfsClientSocketAddr != NULL);

		addressConvStatus = STATUS_INVALID_PARAMETER;
		addressBufferLen = ADDRESS_STRING_BUFFER_SIZE;

		if (nfsClientSocketAddr->ss_family == AF_INET) {

			PSOCKADDR_IN ipv4SocketAddr = (PSOCKADDR_IN)nfsClientSocketAddr;


			addressConvStatus = RtlIpv4AddressToStringEx(&ipv4SocketAddr->sin_addr, ipv4SocketAddr->sin_port, addressBuffer, &addressBufferLen);

		}
		else if (nfsClientSocketAddr->ss_family == AF_INET6) {

			PSOCKADDR_IN6 ipv6SocketAddr = (PSOCKADDR_IN6)nfsClientSocketAddr;


			addressConvStatus = RtlIpv6AddressToStringEx(&ipv6SocketAddr->sin6_addr, 0, ipv6SocketAddr->sin6_port, addressBuffer, &addressBufferLen);
		}


		if ((STATUS_INVALID_PARAMETER != addressConvStatus) && (addressBufferLen > 0)) {
			wcharsCopied = (SHORT)RtlStringCchPrintfW(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"%s NFS SOCKET ADDR: %S;", printPointer, addressBuffer);

		}
		else {
			wcharsCopied = (SHORT)RtlStringCchPrintfW(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"%s NFS INVALID SOCKET ADDR;", printPointer);
		}
	}


	if (recordData->KnownEcpMask & ECP_TYPE_FLAG_SRV) {

		PSRV_OPEN_ECP_CONTEXT srvEcpContext = NULL;
		PUNICODE_STRING srvShareNameString;
		PSOCKADDR_STORAGE_NFS srvClientSocketAddr;

		knownCount++;


		srvEcpContext = (PSRV_OPEN_ECP_CONTEXT)ContextPointers[EcpSrvOpen];

		NT_ASSERT(NULL != srvEcpContext);

		srvShareNameString = srvEcpContext->ShareName;
		srvClientSocketAddr = srvEcpContext->SocketAddress;

		if (srvShareNameString) {
			wcharsCopied = (SHORT)RtlStringCchPrintfW(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"%s SRV SHARE NAME: %wZ,", printPointer, srvShareNameString);
		}

		NT_ASSERT(srvClientSocketAddr != NULL);
		addressConvStatus = STATUS_INVALID_PARAMETER;
		addressBufferLen = ADDRESS_STRING_BUFFER_SIZE;

		if (srvClientSocketAddr->ss_family == AF_INET) {
			PSOCKADDR_IN ipv4SocketAddr = (PSOCKADDR_IN)srvClientSocketAddr;
			addressConvStatus = RtlIpv4AddressToStringEx(&ipv4SocketAddr->sin_addr, ipv4SocketAddr->sin_port, addressBuffer, &addressBufferLen);
		}
		else if (srvClientSocketAddr->ss_family == AF_INET6) {
			PSOCKADDR_IN6 ipv6SocketAddr = (PSOCKADDR_IN6)srvClientSocketAddr;
			addressConvStatus = RtlIpv6AddressToStringEx(&ipv6SocketAddr->sin6_addr, 0, ipv6SocketAddr->sin6_port, addressBuffer, &addressBufferLen);
		}
		if ((STATUS_INVALID_PARAMETER != addressConvStatus) && (addressBufferLen > 0)) {
			wcharsCopied = (SHORT)RtlStringCchPrintfW(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"%s SRV SOCKET ADDR: %S;", printPointer, addressBuffer);
		}
		else {
			wcharsCopied = (SHORT)RtlStringCchPrintfW(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"%s SRV INVALID SOCKET ADDR;", printPointer);
		}
		wcharsCopied = (SHORT)RtlStringCchPrintfW(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"%s SRV FLAGS: %s%s%s;", printPointer, (srvEcpContext->OplockBlockState) ? L"B" : L"-", (srvEcpContext->OplockAppState) ? L"A" : L"-", (srvEcpContext->OplockFinalState) ? L"F" : L"-");
	}


	if (recordData->KnownEcpMask & ECP_TYPE_FLAG_PREFETCH) {
		knownCount++;
		wcharsCopied = (SHORT)RtlStringCchPrintfW(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"%s PREFETCH;", printPointer);
	}
	if (knownCount < recordData->EcpCount) {
		wcharsCopied = (SHORT)RtlStringCchPrintfW(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"%s %d unknown ECPs]", printPointer, recordData->EcpCount - knownCount);
	}
	else {
		wcharsCopied = (SHORT)RtlStringCchPrintfW(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"%s]", printPointer);
	}



	if (wcharsCopied >= 0) {
		EcpData->Length = wcharsCopied * sizeof(WCHAR);
	}
	else {
		EcpData->Length = (MAX_NAME_SPACE - sizeof(UNICODE_NULL));
		EcpData->Buffer[(MAX_NAME_SPACE - sizeof(UNICODE_NULL))] = UNICODE_NULL;
	}
}

VOID SpyParseEcps(_In_ PFLT_CALLBACK_DATA Data, _Inout_ PRECORD_LIST RecordList, _Inout_ PUNICODE_STRING EcpData)
{
	NTSTATUS status;
	PECP_LIST ecpList;
	PRECORD_DATA recordData = &RecordList->LogRecord.Data;
	PVOID ecpContext = NULL;
	GUID ecpGuid = { 0 };
	ULONG ecpContextSize = 0;
	ULONG ecpFlag;
	PVOID contextPointers[NumKnownEcps];
	UCHAR offset = 0;

	PAGED_CODE();

	RtlZeroMemory(contextPointers, sizeof(PVOID) * NumKnownEcps);

	status = FltGetEcpListFromCallbackData(MiniSpyData.Filter, Data, &ecpList);

	if (NT_SUCCESS(status) && (NULL != ecpList)) {

		while (NT_SUCCESS(FltGetNextExtraCreateParameter(MiniSpyData.Filter, ecpList, ecpContext, (LPGUID)&ecpGuid, &ecpContext, &ecpContextSize))) {

			ecpFlag = 0;

			if (IsEqualGUID(&GUID_ECP_PREFETCH_OPEN, &ecpGuid)) {
				ecpFlag = ECP_TYPE_FLAG_PREFETCH;
				offset = EcpPrefetchOpen;
			}


			else if (IsEqualGUID(&GUID_ECP_OPLOCK_KEY, &ecpGuid)) {

				ecpFlag = ECP_TYPE_FLAG_OPLOCK_KEY;
				offset = EcpOplockKey;

			}
			else if (IsEqualGUID(&GUID_ECP_NFS_OPEN, &ecpGuid)) {

				ecpFlag = ECP_TYPE_FLAG_NFS;
				offset = EcpNfsOpen;

			}
			else if (IsEqualGUID(&GUID_ECP_SRV_OPEN, &ecpGuid)) {

				ecpFlag = ECP_TYPE_FLAG_SRV;
				offset = EcpSrvOpen;
			}

			if ((0 != ecpFlag) &&
				!FltIsEcpFromUserMode(MiniSpyData.Filter, ecpContext)) {

				FLT_ASSERT(!FlagOn(recordData->KnownEcpMask, ecpFlag));

				recordData->KnownEcpMask |= ecpFlag;

				contextPointers[offset] = ecpContext;
			}

			recordData->EcpCount++;
		}

		if (0 < recordData->EcpCount) {

			SpyBuildEcpDataString(RecordList, EcpData, contextPointers);
		}
	}
}

VOID SpySetRecordNameAndEcpData(_Inout_ PLOG_RECORD LogRecord, _In_ PUNICODE_STRING Name, _In_opt_ PUNICODE_STRING EcpData)
{

	PWCHAR printPointer = (PWCHAR)LogRecord->Name;
	SHORT wcharsCopied;
	USHORT stringLength;

	FLT_ASSERT(NULL != Name);

	if (NULL != EcpData) {
		wcharsCopied = (SHORT)_snwprintf(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"%wZ %wZ", Name, EcpData);

	}
	else {
		wcharsCopied = (SHORT)_snwprintf(printPointer, MAX_NAME_WCHARS_LESS_NULL, L"%wZ", Name);
	}

	if (wcharsCopied >= 0) {

		stringLength = wcharsCopied * sizeof(WCHAR);

	}
	else {

		stringLength = MAX_NAME_SPACE_LESS_NULL;
		printPointer[MAX_NAME_WCHARS_LESS_NULL] = UNICODE_NULL;
	}

	LogRecord->Length = ROUND_TO_SIZE((LogRecord->Length + stringLength + sizeof(UNICODE_NULL)), sizeof(PVOID));

	FLT_ASSERT(LogRecord->Length <= MAX_LOG_RECORD_LENGTH);
}


VOID SpyLogPreOperationData(_In_ PFLT_CALLBACK_DATA Data, _In_ PCFLT_RELATED_OBJECTS FltObjects, _Inout_ PRECORD_LIST RecordList)
{
	PRECORD_DATA recordData = &RecordList->LogRecord.Data;
	PDEVICE_OBJECT devObj;
	NTSTATUS status;

	status = FltGetDeviceObject(FltObjects->Volume, &devObj);
	if (NT_SUCCESS(status)) {

		ObDereferenceObject(devObj);

	}
	else {

		devObj = NULL;
	}

	recordData->CallbackMajorId = Data->Iopb->MajorFunction;
	recordData->CallbackMinorId = Data->Iopb->MinorFunction;
	recordData->IrpFlags = Data->Iopb->IrpFlags;
	recordData->Flags = Data->Flags;

	recordData->DeviceObject = (ULONG_PTR)devObj;
	recordData->FileObject = (ULONG_PTR)FltObjects->FileObject;
	recordData->Transaction = (ULONG_PTR)FltObjects->Transaction;
	recordData->ProcessId = (ULONG_PTR)PsGetCurrentProcessId();
	recordData->ThreadId = (ULONG_PTR)PsGetCurrentThreadId();

	recordData->Arg1 = Data->Iopb->Parameters.Others.Argument1;
	recordData->Arg2 = Data->Iopb->Parameters.Others.Argument2;
	recordData->Arg3 = Data->Iopb->Parameters.Others.Argument3;
	recordData->Arg4 = Data->Iopb->Parameters.Others.Argument4;
	recordData->Arg5 = Data->Iopb->Parameters.Others.Argument5;
	recordData->Arg6.QuadPart = Data->Iopb->Parameters.Others.Argument6.QuadPart;

	KeQuerySystemTime(&recordData->OriginatingTime);
}


VOID SpyLog(_In_ PRECORD_LIST RecordList)
{
	KIRQL oldIrql;

	KeAcquireSpinLock(&MiniSpyData.OutputBufferLock, &oldIrql);
	InsertTailList(&MiniSpyData.OutputBufferList, &RecordList->List);
	KeReleaseSpinLock(&MiniSpyData.OutputBufferLock, oldIrql);
}


NTSTATUS SpyGetLog(_Out_writes_bytes_to_(OutputBufferLength, *ReturnOutputBufferLength) PUCHAR OutputBuffer, _In_ ULONG OutputBufferLength, _Out_ PULONG ReturnOutputBufferLength)
{
	PLIST_ENTRY pList;
	ULONG bytesWritten = 0;
	PLOG_RECORD pLogRecord;
	NTSTATUS status = STATUS_NO_MORE_ENTRIES;
	PRECORD_LIST pRecordList;
	KIRQL oldIrql;
	BOOLEAN recordsAvailable = FALSE;

	KeAcquireSpinLock(&MiniSpyData.OutputBufferLock, &oldIrql);

	while (!IsListEmpty(&MiniSpyData.OutputBufferList) && (OutputBufferLength > 0)) {


		recordsAvailable = TRUE;


		pList = RemoveHeadList(&MiniSpyData.OutputBufferList);

		pRecordList = CONTAINING_RECORD(pList, RECORD_LIST, List);

		pLogRecord = &pRecordList->LogRecord;


		if (REMAINING_NAME_SPACE(pLogRecord) == MAX_NAME_SPACE) {


			pLogRecord->Length += ROUND_TO_SIZE(sizeof(UNICODE_NULL), sizeof(PVOID));
			pLogRecord->Name[0] = UNICODE_NULL;
		}

		if (OutputBufferLength < pLogRecord->Length) {

			InsertHeadList(&MiniSpyData.OutputBufferList, pList);
			break;
		}

		KeReleaseSpinLock(&MiniSpyData.OutputBufferLock, oldIrql);

		try {
			RtlCopyMemory(OutputBuffer, pLogRecord, pLogRecord->Length);
		} except(EXCEPTION_EXECUTE_HANDLER) {


			KeAcquireSpinLock(&MiniSpyData.OutputBufferLock, &oldIrql);
			InsertHeadList(&MiniSpyData.OutputBufferList, pList);
			KeReleaseSpinLock(&MiniSpyData.OutputBufferLock, oldIrql);

			return GetExceptionCode();

		}

		bytesWritten += pLogRecord->Length;

		OutputBufferLength -= pLogRecord->Length;

		OutputBuffer += pLogRecord->Length;

		SpyFreeRecord(pRecordList);


		KeAcquireSpinLock(&MiniSpyData.OutputBufferLock, &oldIrql);
	}

	KeReleaseSpinLock(&MiniSpyData.OutputBufferLock, oldIrql);


	if ((bytesWritten == 0) && recordsAvailable) {


		status = STATUS_BUFFER_TOO_SMALL;

	}
	else if (bytesWritten > 0) {

		status = STATUS_SUCCESS;
	}

	*ReturnOutputBufferLength = bytesWritten;

	return status;
}


VOID SpyEmptyOutputBufferList(VOID)
{
	PLIST_ENTRY pList;
	PRECORD_LIST pRecordList;
	KIRQL oldIrql;

	KeAcquireSpinLock(&MiniSpyData.OutputBufferLock, &oldIrql);

	while (!IsListEmpty(&MiniSpyData.OutputBufferList)) {

		pList = RemoveHeadList(&MiniSpyData.OutputBufferList);
		KeReleaseSpinLock(&MiniSpyData.OutputBufferLock, oldIrql);

		pRecordList = CONTAINING_RECORD(pList, RECORD_LIST, List);

		SpyFreeRecord(pRecordList);

		KeAcquireSpinLock(&MiniSpyData.OutputBufferLock, &oldIrql);
	}

	KeReleaseSpinLock(&MiniSpyData.OutputBufferLock, oldIrql);
}




NTSTATUS SpyConnect(_In_ PFLT_PORT ClientPort, _In_ PVOID ServerPortCookie, _In_reads_bytes_(SizeOfContext) PVOID ConnectionContext, _In_ ULONG SizeOfContext, _Flt_ConnectionCookie_Outptr_ PVOID *ConnectionCookie)
{

	PAGED_CODE();

	UNREFERENCED_PARAMETER(ServerPortCookie);
	UNREFERENCED_PARAMETER(ConnectionContext);
	UNREFERENCED_PARAMETER(SizeOfContext);
	UNREFERENCED_PARAMETER(ConnectionCookie);

	NT_ASSERT(MiniSpyData.ClientPort == NULL);
	MiniSpyData.ClientPort = ClientPort;
	return STATUS_SUCCESS;
}


VOID SpyDisconnect(_In_opt_ PVOID ConnectionCookie)
{

	PAGED_CODE();

	UNREFERENCED_PARAMETER(ConnectionCookie);

	FltCloseClientPort(MiniSpyData.Filter, &MiniSpyData.ClientPort);
}




NTSTATUS SpyQueryTeardown(_In_ PCFLT_RELATED_OBJECTS FltObjects, _In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(Flags);
	PAGED_CODE();
	return STATUS_SUCCESS;
}


NTSTATUS SpyMessage(_In_ PVOID ConnectionCookie, _In_reads_bytes_opt_(InputBufferSize) PVOID InputBuffer, _In_ ULONG InputBufferSize, _Out_writes_bytes_to_opt_(OutputBufferSize, *ReturnOutputBufferLength) PVOID OutputBuffer, _In_ ULONG OutputBufferSize, _Out_ PULONG ReturnOutputBufferLength)
{
	MINISPY_COMMAND command;
	NTSTATUS rv = STATUS_UNSUCCESSFUL;

	PAGED_CODE();
	UNREFERENCED_PARAMETER(ConnectionCookie);
	if ((InputBuffer != NULL) && (InputBufferSize >= (FIELD_OFFSET(COMMAND_MESSAGE, Command) + sizeof(MINISPY_COMMAND)))) {
		try {

			command = ((PCOMMAND_MESSAGE)InputBuffer)->Command;

		} except(EXCEPTION_EXECUTE_HANDLER) {

			return GetExceptionCode();
		}

		switch (command) {
		case GetMiniSpyLog:
			if ((OutputBuffer == NULL) || (OutputBufferSize == 0)) {
				rv = STATUS_INVALID_PARAMETER;
				break;
			}
#if defined(_WIN64)
			if (IoIs32bitProcess(NULL)) {
				if (!IS_ALIGNED(OutputBuffer, sizeof(ULONG))) {
					rv = STATUS_DATATYPE_MISALIGNMENT;
					break;
				}
			}
			else {
#endif
				if (!IS_ALIGNED(OutputBuffer, sizeof(PVOID))) {
					rv = STATUS_DATATYPE_MISALIGNMENT;
					break;
				}
#if defined(_WIN64)
			}
#endif
			rv = SpyGetLog(OutputBuffer, OutputBufferSize, ReturnOutputBufferLength);
			break;
		default:
			rv = STATUS_INVALID_PARAMETER;
			break;
		}
	}
	else {
		rv = STATUS_INVALID_PARAMETER;
	}
	return rv;
}



FLT_PREOP_CALLBACK_STATUS SpyPreOperationCallback(_Inout_ PFLT_CALLBACK_DATA Data, _In_ PCFLT_RELATED_OBJECTS FltObjects, _Flt_CompletionContext_Outptr_ PVOID *CompletionContext)
{
	UNREFERENCED_PARAMETER(CompletionContext);
	PRECORD_LIST recordList;
	PFLT_FILE_NAME_INFORMATION nameInfo = NULL;
	UNICODE_STRING defaultName;
	PUNICODE_STRING nameToUse;
	NTSTATUS rv = STATUS_UNSUCCESSFUL;
	PUNICODE_STRING ecpDataToUse = NULL;
	UNICODE_STRING ecpData;
	WCHAR ecpDataBuffer[MAX_NAME_SPACE / sizeof(WCHAR)];
	WCHAR name[MAX_NAME_SPACE / sizeof(WCHAR)];
	recordList = SpyNewRecord();
	if (recordList) {
		if (FltObjects->FileObject != NULL) {
			rv = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | MiniSpyData.NameQueryMethod, &nameInfo);
		}
		else {
			rv = STATUS_UNSUCCESSFUL;
		}
		if (NT_SUCCESS(rv)) {
			nameToUse = &nameInfo->Name;
			if (FlagOn(MiniSpyData.DebugFlags, SPY_DEBUG_PARSE_NAMES)) {
				FltParseFileNameInformation(nameInfo);
			}
		}
		else {
			PFLT_FILE_NAME_INFORMATION lnameInfo = { 0 };
			if (FltObjects->FileObject != NULL) {
				rv = FltGetFileNameInformation(Data, FLT_FILE_NAME_OPENED | FLT_FILE_NAME_QUERY_ALWAYS_ALLOW_CACHE_LOOKUP, &lnameInfo);
				if (NT_SUCCESS(rv)) {
					RtlStringCchPrintfW(name, sizeof(name) / sizeof(WCHAR), L" %wZ", &lnameInfo->Name);
					FltReleaseFileNameInformation(lnameInfo);
				}
				else {
					RtlStringCchPrintfW(name, sizeof(name) / sizeof(WCHAR), L"<NO NAME: OpenedStatus=%08x>", rv);
				}
			}
			else {
				RtlStringCchPrintfW(name, sizeof(name) / sizeof(WCHAR), L"<NO NAME>");
			}
			name[(sizeof(name) / sizeof(WCHAR)) - 1] = L'\0';
			RtlInitUnicodeString(&defaultName, name);
			nameToUse = &defaultName;
		}
		if (Data->Iopb->MajorFunction == IRP_MJ_CREATE) {
			RtlInitEmptyUnicodeString(&ecpData, ecpDataBuffer, MAX_NAME_SPACE / sizeof(WCHAR));
			SpyParseEcps(Data, recordList, &ecpData);
			ecpDataToUse = &ecpData;
		}
		SpySetRecordNameAndEcpData(&(recordList->LogRecord), nameToUse, ecpDataToUse);
		if (NULL != nameInfo) {
			FltReleaseFileNameInformation(nameInfo);
		}
		SpyLogPreOperationData(Data, FltObjects, recordList);
	}
	return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}


