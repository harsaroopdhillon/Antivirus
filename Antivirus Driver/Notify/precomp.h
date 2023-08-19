#pragma warning(disable:28930)
#pragma warning(disable:28931)
#pragma warning(disable:4201)  
#include <ndis.h>

#define __FILENUMBER    'PNPF'


#define LINKNAME_STRING             L"\\DosDevices\\NDISFilterDriver"
#define NTDEVICE_STRING             L"\\Device\\NDISFilterDriver"

#define FILTER_REQUEST_ID          'RTLF'
#define FILTER_ALLOC_TAG           'tliF'
#define FILTER_TAG                 'dnTF'
#define FILTER_MAJOR_NDIS_VERSION   6
#define FILTER_MINOR_NDIS_VERSION   0

#define ETHERTYPE_IP4 0x0800
#define ETHERTYPE_ARP 0x0806
#define ETHERTYPE_IP6 0x86DD


#define   FILTER_LOG_RCV_REF(_O, _Instance, _NetBufferList, _Ref)


#define   FILTER_LOG_SEND_REF(_O, _Instance, _NetBufferList, _Ref)



#define FILTER_ALLOC_MEM(_NdisHandle, _Size)     \
    NdisAllocateMemoryWithTagPriority(_NdisHandle, _Size, FILTER_ALLOC_TAG, LowPoolPriority)

#define FILTER_FREE_MEM(_pMem)    NdisFreeMemory(_pMem, 0, 0)

#define FILTER_INIT_LOCK(_pLock)      NdisAllocateSpinLock(_pLock)

#define FILTER_FREE_LOCK(_pLock)      NdisFreeSpinLock(_pLock)

#define FILTER_ACQUIRE_LOCK(_pLock, DispatchLevel)              \
    {                                                           \
        if (DispatchLevel)                                      \
        {                                                       \
            NdisDprAcquireSpinLock(_pLock);                     \
        }                                                       \
        else                                                    \
        {                                                       \
            NdisAcquireSpinLock(_pLock);                        \
        }                                                       \
    }

#define FILTER_RELEASE_LOCK(_pLock, DispatchLevel)              \
    {                                                           \
        if (DispatchLevel)                                      \
        {                                                       \
            NdisDprReleaseSpinLock(_pLock);                     \
        }                                                       \
        else                                                    \
        {                                                       \
            NdisReleaseSpinLock(_pLock);                        \
        }                                                       \
    }


#define NET_BUFFER_LIST_LINK_TO_ENTRY(_pNBL)    ((PQUEUE_ENTRY)(NET_BUFFER_LIST_NEXT_NBL(_pNBL)))
#define ENTRY_TO_NET_BUFFER_LIST(_pEnt)         (CONTAINING_RECORD((_pEnt), NET_BUFFER_LIST, Next))

#define InitializeQueueHeader(_QueueHeader)             \
{                                                       \
    (_QueueHeader)->Head = (_QueueHeader)->Tail = NULL; \
}

#define IsQueueEmpty(_QueueHeader)      ((_QueueHeader)->Head == NULL)

#define RemoveHeadQueue(_QueueHeader)                   \
    (_QueueHeader)->Head;                               \
    {                                                   \
        PQUEUE_ENTRY pNext;                             \
        ASSERT((_QueueHeader)->Head);                   \
        pNext = (_QueueHeader)->Head->Next;             \
        (_QueueHeader)->Head = pNext;                   \
        if (pNext == NULL)                              \
            (_QueueHeader)->Tail = NULL;                \
    }

#define InsertHeadQueue(_QueueHeader, _QueueEntry)                  \
    {                                                               \
        ((PQUEUE_ENTRY)(_QueueEntry))->Next = (_QueueHeader)->Head; \
        (_QueueHeader)->Head = (PQUEUE_ENTRY)(_QueueEntry);         \
        if ((_QueueHeader)->Tail == NULL)                           \
            (_QueueHeader)->Tail = (PQUEUE_ENTRY)(_QueueEntry);     \
    }

#define InsertTailQueue(_QueueHeader, _QueueEntry)                      \
    {                                                                   \
        ((PQUEUE_ENTRY)(_QueueEntry))->Next = NULL;                     \
        if ((_QueueHeader)->Tail)                                       \
            (_QueueHeader)->Tail->Next = (PQUEUE_ENTRY)(_QueueEntry);   \
        else                                                            \
            (_QueueHeader)->Head = (PQUEUE_ENTRY)(_QueueEntry);         \
        (_QueueHeader)->Tail = (PQUEUE_ENTRY)(_QueueEntry);             \
    }


#define FILTER_READY_TO_PAUSE(_Filter)      \
    ((_Filter)->State == FilterPausing)



typedef struct _NDF_ETH_HEADER
{
	UCHAR Dst[6];
	UCHAR Src[6];
	USHORT Type;
} NDF_ETH_HEADER, *PNDF_ETH_HEADER;

typedef struct _NDF_IPV4_HEADER
{
	ULONG HeaderInfo[2];
	INT TTL : 8;
	INT Protocol : 8;
	INT HeaderChecksum : 16;
	UCHAR SrcIp[4];
	UCHAR DstIp[4];
} NDF_IPV4_HEADER, *PNDF_IPV4_HEADER;

typedef struct _NDF_TCP_HEADER
{
	INT SrcPort : 16;
	INT DstPort : 16;
	INT SequenceNumber : 32;
	INT ApprovalNumber : 32;
	ULONG HeaderInfo[2];
} NDF_TCP_HEADER, *PNDF_TCP_HEADER;

typedef struct _NDF_IPV6_HEADER
{
	ULONG HeaderInfo[2];
	UCHAR SrcAddress[16];
	UCHAR DstAddress[16];
} NDF_IPV6_HEADER, *PNDF_IPV6_HEADER;

typedef struct _NDF_ARP_HEADER
{
	ULONG HeaderInfo[2];
	UCHAR SrcMac[6];
	UCHAR SrcIp[4];
	UCHAR DstMac[6];
	UCHAR DstIp[4];
} NDF_ARP_HEADER, *PNDF_ARP_HEADER;

typedef struct _RULE_IPV4
{
	ULONG Id;
	UCHAR Begin[4];
	UCHAR End[4];
	PVOID Next;
} RULE_IPV4, *PRULE_IPV4;

typedef struct _RULE_IPV6
{
	ULONG Id;
	UCHAR Begin[16];
	UCHAR End[16];
	PVOID Next;
} RULE_IPV6, *PRULE_IPV6;

typedef struct _RULE_ETH
{
	ULONG Id;
	UCHAR Addr[6];
} RULE_ETH, *PRULE_ETH;

typedef struct _RULES_LISTS
{
	BOOLEAN IsActive;
	PVOID FirstRuleIPv4;
	PVOID FirstRuleIPv6;
} RULES_LISTS, *PRULES_LISTS;


typedef struct _QUEUE_ENTRY
{
	struct _QUEUE_ENTRY * Next;
}QUEUE_ENTRY, *PQUEUE_ENTRY;

typedef struct _QUEUE_HEADER
{
	PQUEUE_ENTRY     Head;
	PQUEUE_ENTRY     Tail;
} QUEUE_HEADER, PQUEUE_HEADER;

typedef enum _FILTER_STATE
{
	FilterStateUnspecified,
	FilterInitialized,
	FilterPausing,
	FilterPaused,
	FilterRunning,
	FilterRestarting,
	FilterDetaching
} FILTER_STATE;


typedef struct _FILTER_REQUEST
{
	NDIS_OID_REQUEST       Request;
	NDIS_EVENT             ReqEvent;
	NDIS_STATUS            Status;
} FILTER_REQUEST, *PFILTER_REQUEST;

typedef struct _MS_FILTER
{
	LIST_ENTRY                     FilterModuleLink;
	ULONG                           RefCount;

	NDIS_HANDLE                     FilterHandle;
	NDIS_STRING                     FilterModuleName;
	NDIS_STRING                     MiniportFriendlyName;
	NDIS_STRING                     MiniportName;
	NET_IFINDEX                     MiniportIfIndex;

	NDIS_STATUS                     Status;
	NDIS_EVENT                      Event;
	ULONG                           BackFillSize;
	NDIS_SPIN_LOCK                     Lock;  

	FILTER_STATE                    State;  
	ULONG                           OutstandingSends;
	ULONG                           OutstandingRequest;
	ULONG                           OutstandingRcvs;
	NDIS_SPIN_LOCK                     SendLock;
	NDIS_SPIN_LOCK                     RcvLock;
	QUEUE_HEADER                    SendNBLQueue;
	QUEUE_HEADER                    RcvNBLQueue;


	NDIS_STRING                     FilterName;
	ULONG                           CallsRestart;
	BOOLEAN                         TrackReceives;
	BOOLEAN                         TrackSends;


	PNDIS_OID_REQUEST               PendingOidRequest;

}MS_FILTER, *PMS_FILTER;


typedef struct _FILTER_DEVICE_EXTENSION
{
	ULONG            Signature;
	NDIS_HANDLE      Handle;
	PVOID			 FilterRules;
	KSPIN_LOCK       QLock;
} FILTER_DEVICE_EXTENSION, *PFILTER_DEVICE_EXTENSION;



typedef struct _FL_NDIS_FILTER_LIST
{
	LIST_ENTRY              Link;
	NDIS_HANDLE             ContextHandle;
	NDIS_STRING             FilterInstanceName;
} FL_NDIS_FILTER_LIST, *PFL_NDIS_FILTER_LIST;

typedef struct _NDIS_OID_REQUEST *FILTER_REQUEST_CONTEXT, **PFILTER_REQUEST_CONTEXT;

FILTER_SET_OPTIONS FilterRegisterOptions;
FILTER_ATTACH FilterAttach;
FILTER_DETACH FilterDetach;
DRIVER_UNLOAD FilterUnload;
FILTER_RESTART FilterRestart;
FILTER_PAUSE FilterPause;
FILTER_OID_REQUEST FilterOidRequest;
FILTER_CANCEL_OID_REQUEST FilterCancelOidRequest;
FILTER_STATUS FilterStatus;
FILTER_DEVICE_PNP_EVENT_NOTIFY FilterDevicePnPEventNotify;
FILTER_NET_PNP_EVENT FilterNetPnPEvent;
FILTER_OID_REQUEST_COMPLETE FilterOidRequestComplete;
FILTER_SEND_NET_BUFFER_LISTS FilterSendNetBufferLists;
FILTER_RETURN_NET_BUFFER_LISTS FilterReturnNetBufferLists;
FILTER_SEND_NET_BUFFER_LISTS_COMPLETE FilterSendNetBufferListsComplete;
FILTER_RECEIVE_NET_BUFFER_LISTS FilterReceiveNetBufferLists;
FILTER_CANCEL_SEND_NET_BUFFER_LISTS FilterCancelSendNetBufferLists;
FILTER_SET_MODULE_OPTIONS FilterSetModuleOptions;
DRIVER_DISPATCH NDISFilterDriverDispatch;
DRIVER_DISPATCH NDISFilterDriverDeviceIoControl;
NDIS_HANDLE         FilterDriverHandle;
NDIS_HANDLE         FilterDriverObject;
NDIS_SPIN_LOCK         FilterListLock;
PFILTER_DEVICE_EXTENSION pFilterDeviceExtension;
LIST_ENTRY          FilterModuleList;
extern NDIS_HANDLE         FilterDriverHandle;
extern NDIS_HANDLE         FilterDriverObject;
extern NDIS_HANDLE         NdisFilterDeviceHandle;
extern NDIS_SPIN_LOCK FilterListLock;
extern LIST_ENTRY          FilterModuleList;




_IRQL_requires_max_(PASSIVE_LEVEL)
NDIS_STATUS NDISFilterDriverRegisterDevice( OUT PFILTER_DEVICE_EXTENSION *Extension, PDEVICE_OBJECT DeviceObject);

_IRQL_requires_max_(PASSIVE_LEVEL)
VOID NDISFilterDriverDeregisterDevice(VOID );


_IRQL_requires_max_(DISPATCH_LEVEL)
PMS_FILTER filterFindFilterModule( _In_reads_bytes_(BufferLength) PUCHAR Buffer, _In_ ULONG BufferLength);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS filterDoInternalRequest(_In_ PMS_FILTER FilterModuleContext, _In_ NDIS_REQUEST_TYPE RequestType, _In_ NDIS_OID Oid, _Inout_updates_bytes_to_(InformationBufferLength, *pBytesProcessed) PVOID InformationBuffer, _In_ ULONG InformationBufferLength, _In_opt_ ULONG OutputBufferLength, _In_ ULONG MethodId, _Out_ PULONG pBytesProcessed);

VOID filterInternalRequestComplete(_In_ NDIS_HANDLE FilterModuleContext, _In_ PNDIS_OID_REQUEST NdisRequest,_In_ NDIS_STATUS Status);


BOOLEAN CheckSentPacket(PNET_BUFFER NetBuffer);
BOOLEAN CheckSentIPv4Header(PNDF_IPV4_HEADER IPv4Header);
BOOLEAN CheckSentIPv6Header(PNDF_IPV6_HEADER IPv6Header);
BOOLEAN CheckSentArpHeader(PNDF_ARP_HEADER ArpHeader);
BOOLEAN CheckReceivePacket(PNET_BUFFER NetBuffer);
BOOLEAN CheckReceiveIPv4Header(PNDF_IPV4_HEADER IPv4Header);
BOOLEAN CheckReceiveIPv6Header(PNDF_IPV6_HEADER IPv6Header);
BOOLEAN CheckReceiveArpHeader(PNDF_ARP_HEADER ArpHeader);


VOID AddIPv4Rule(IN PFILTER_DEVICE_EXTENSION FilterDeviceExtension, IN PRULE_IPV4 Rule);
VOID DelIPv4Rule(IN PFILTER_DEVICE_EXTENSION FilterDeviceExtension, IN ULONG Id);
VOID AddIPv6Rule(IN PFILTER_DEVICE_EXTENSION FilterDeviceExtension, IN PRULE_IPV6 Rule);
VOID DelIPv6Rule(IN PFILTER_DEVICE_EXTENSION FilterDeviceExtension, IN ULONG Id);
VOID ActivateRules(IN PFILTER_DEVICE_EXTENSION FilterDeviceExtension);
VOID DeactivateRules(IN PFILTER_DEVICE_EXTENSION FilterDeviceExtension);
PMS_FILTER filterFindFilterModule(_In_reads_bytes_(BufferLength) PUCHAR Buffer, _In_ ULONG BufferLength);


