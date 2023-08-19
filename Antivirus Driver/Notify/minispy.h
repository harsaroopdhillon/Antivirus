#pragma prefast(disable:__WARNING_ENCODE_MEMBER_FUNCTION_POINTER, "Not valid for kernel mode drivers")
#pragma warning(push)
#pragma warning(disable:4200) 
#include <suppress.h>
#include <fltKernel.h>
#include <ntddk.h>
#ifndef __MINISPY_H__
#define __MINISPY_H__
#define SPY_TAG 'ypSM'


#define IRP_MJ_ACQUIRE_FOR_SECTION_SYNCHRONIZATION  ((UCHAR)-1)
#define IRP_MJ_RELEASE_FOR_SECTION_SYNCHRONIZATION  ((UCHAR)-2)
#define IRP_MJ_ACQUIRE_FOR_MOD_WRITE                ((UCHAR)-3)
#define IRP_MJ_RELEASE_FOR_MOD_WRITE                ((UCHAR)-4)
#define IRP_MJ_ACQUIRE_FOR_CC_FLUSH                 ((UCHAR)-5)
#define IRP_MJ_RELEASE_FOR_CC_FLUSH                 ((UCHAR)-6)
#define IRP_MJ_NOTIFY_STREAM_FO_CREATION            ((UCHAR)-7)
#define IRP_MJ_FAST_IO_CHECK_IF_POSSIBLE            ((UCHAR)-13)
#define IRP_MJ_NETWORK_QUERY_OPEN                   ((UCHAR)-14)
#define IRP_MJ_MDL_READ                             ((UCHAR)-15)
#define IRP_MJ_MDL_READ_COMPLETE                    ((UCHAR)-16)
#define IRP_MJ_PREPARE_MDL_WRITE                    ((UCHAR)-17)
#define IRP_MJ_MDL_WRITE_COMPLETE                   ((UCHAR)-18)
#define IRP_MJ_VOLUME_MOUNT                         ((UCHAR)-19)
#define IRP_MJ_VOLUME_DISMOUNT                      ((UCHAR)-20)



#define MAX_NAME_SPACE ROUND_TO_SIZE( (RECORD_SIZE - sizeof(RECORD_LIST)), sizeof( PVOID ))


#define MAX_NAME_SPACE_LESS_NULL (MAX_NAME_SPACE - sizeof(UNICODE_NULL))
#define MAX_NAME_WCHARS_LESS_NULL MAX_NAME_SPACE_LESS_NULL / sizeof(WCHAR)

#define REMAINING_NAME_SPACE(LogRecord) \
    (NT_ASSERT((LogRecord)->Length >= sizeof(LOG_RECORD)), \
     (USHORT)(MAX_NAME_SPACE - ((LogRecord)->Length - sizeof(LOG_RECORD))))

#define MAX_LOG_RECORD_LENGTH  (RECORD_SIZE - FIELD_OFFSET( RECORD_LIST, LogRecord ))



#define RECORD_SIZE     1024


#define RECORD_TYPE_NORMAL                       0x00000000
#define RECORD_TYPE_FILETAG                      0x00000004

#define RECORD_TYPE_FLAG_STATIC                  0x80000000
#define RECORD_TYPE_FLAG_EXCEED_MEMORY_ALLOWANCE 0x20000000
#define RECORD_TYPE_FLAG_OUT_OF_MEMORY           0x10000000
#define RECORD_TYPE_FLAG_MASK                    0xffff0000


typedef _Return_type_success_(return >= 0) LONG NTSTATUS;


typedef struct _RECORD_DATA {

	LARGE_INTEGER OriginatingTime;
	LARGE_INTEGER CompletionTime;

	ULONG_PTR DeviceObject;
	ULONG_PTR FileObject;
	ULONG_PTR Transaction;

	ULONG_PTR ProcessId;
	ULONG_PTR ThreadId;

	ULONG_PTR Information;

	NTSTATUS Status;

	ULONG IrpFlags;
	ULONG Flags;

	UCHAR CallbackMajorId;
	UCHAR CallbackMinorId;
	UCHAR Reserved[2];      

	PVOID Arg1;
	PVOID Arg2;
	PVOID Arg3;
	PVOID Arg4;
	PVOID Arg5;
	LARGE_INTEGER Arg6;

	ULONG EcpCount;
	ULONG KnownEcpMask;

} RECORD_DATA, *PRECORD_DATA;

#pragma warning(push)
#pragma warning(disable:4200)

typedef struct _LOG_RECORD {


	ULONG Length;           // Length of log record.  This Does not include
	ULONG SequenceNumber;   // space used by other members of RECORD_LIST

	ULONG RecordType;       // The type of log record this is.
	ULONG Reserved;         // For alignment on IA64

	RECORD_DATA Data;
	WCHAR Name[];          

} LOG_RECORD, *PLOG_RECORD;

#pragma warning(pop)


typedef struct _RECORD_LIST {

	LIST_ENTRY List;


	LOG_RECORD LogRecord;

} RECORD_LIST, *PRECORD_LIST;


typedef enum _MINISPY_COMMAND {

	GetMiniSpyLog

} MINISPY_COMMAND;


typedef struct _COMMAND_MESSAGE {
	MINISPY_COMMAND Command;
	ULONG Reserved;  // Alignment on IA64
	UCHAR Data[];
} COMMAND_MESSAGE, *PCOMMAND_MESSAGE;

#pragma warning(pop)


#ifndef Add2Ptr
#define Add2Ptr(P,I) ((PVOID)((PUCHAR)(P) + (I)))
#endif

#ifndef ROUND_TO_SIZE
#define ROUND_TO_SIZE(_length, _alignment)    \
            (((_length) + ((_alignment)-1)) & ~((_alignment) - 1))
#endif

#ifndef FlagOn
#define FlagOn(_F,_SF)        ((_F) & (_SF))
#endif

#endif /* __MINISPY_H__ */




NTSTATUS
SpyMessage(
	_In_ PVOID ConnectionCookie,
	_In_reads_bytes_opt_(InputBufferSize) PVOID InputBuffer,
	_In_ ULONG InputBufferSize,
	_Out_writes_bytes_to_opt_(OutputBufferSize, *ReturnOutputBufferLength) PVOID OutputBuffer,
	_In_ ULONG OutputBufferSize,
	_Out_ PULONG ReturnOutputBufferLength
);

NTSTATUS
SpyConnect(
	_In_ PFLT_PORT ClientPort,
	_In_ PVOID ServerPortCookie,
	_In_reads_bytes_(SizeOfContext) PVOID ConnectionContext,
	_In_ ULONG SizeOfContext,
	_Flt_ConnectionCookie_Outptr_ PVOID *ConnectionCookie
);

VOID
SpyDisconnect(
	_In_opt_ PVOID ConnectionCookie
);




#define ECP_TYPE_FLAG_PREFETCH                   0x00000001


#define ECP_TYPE_FLAG_OPLOCK_KEY                 0x00000002
#define ECP_TYPE_FLAG_NFS                        0x00000004
#define ECP_TYPE_FLAG_SRV                        0x00000008


#define ADDRESS_STRING_BUFFER_SIZE          64


typedef enum _ECP_TYPE {

	EcpPrefetchOpen,
	EcpOplockKey,
	EcpNfsOpen,
	EcpSrvOpen,

	NumKnownEcps

} ECP_TYPE;






#define SPY_DEBUG_PARSE_NAMES   0x00000001


extern const FLT_REGISTRATION FilterRegistration;

FLT_PREOP_CALLBACK_STATUS
SpyPreOperationCallback(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID *CompletionContext
);


NTSTATUS
SpyQueryTeardown(
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
);



LONG
SpyExceptionFilter(
	_In_ PEXCEPTION_POINTERS ExceptionPointer,
	_In_ BOOLEAN AccessingUserBuffer
);


VOID
SpyFreeBuffer(
	_In_ PVOID Buffer
);

PRECORD_LIST
SpyNewRecord(
	VOID
);

VOID
SpyFreeRecord(
	_In_ PRECORD_LIST Record
);

VOID
SpyParseEcps(
	_In_ PFLT_CALLBACK_DATA Data,
	_Inout_ PRECORD_LIST RecordList,
	_Inout_ PUNICODE_STRING EcpData
);

VOID
SpyBuildEcpDataString(
	_In_ PRECORD_LIST RecordList,
	_Inout_ PUNICODE_STRING EcpData,
	_In_reads_(NumKnownEcps) PVOID * ContextPointers
);

VOID
SpySetRecordNameAndEcpData(
	_Inout_ PLOG_RECORD LogRecord,
	_In_ PUNICODE_STRING Name,
	_In_opt_ PUNICODE_STRING EcpData
);


VOID
SpyLogPreOperationData(
	_In_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Inout_ PRECORD_LIST RecordList
);

NTSTATUS SpyGetLog(_Out_writes_bytes_to_(OutputBufferLength, *ReturnOutputBufferLength) PUCHAR OutputBuffer, _In_ ULONG OutputBufferLength, _Out_ PULONG ReturnOutputBufferLength);
VOID
SpyLogPostOperationData(
	_In_ PFLT_CALLBACK_DATA Data,
	_Inout_ PRECORD_LIST RecordList
);

VOID
SpyLogTransactionNotify(
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Inout_ PRECORD_LIST RecordList,
	_In_ ULONG TransactionNotification
);

VOID
SpyLog(
	_In_ PRECORD_LIST RecordList
);

VOID
SpyEmptyOutputBufferList(
	VOID
);








typedef struct _FILESYSTEM_FILTER_DATA {
	PFLT_FILTER Filter;
	PFLT_PORT ServerPort;
	PFLT_PORT ClientPort;
	KSPIN_LOCK OutputBufferLock;
	LIST_ENTRY OutputBufferList;
	NPAGED_LOOKASIDE_LIST FreeBufferList;
	LONG MaxRecordsToAllocate;
	__volatile LONG RecordsAllocated;
	__volatile LONG StaticBufferInUse;
	PVOID OutOfMemoryBuffer[1024 / sizeof(PVOID)];
	__volatile LONG LogSequenceNumber;
	ULONG NameQueryMethod;
	ULONG DebugFlags;

} FILESYSTEM_FILTER_DATA, *PFILESYSTEM_FILTER_DATA;


FILESYSTEM_FILTER_DATA MiniSpyData;
