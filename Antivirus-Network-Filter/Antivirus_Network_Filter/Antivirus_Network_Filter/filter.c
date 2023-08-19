
#include "filter.h"

NDIS_FILTER_PARTIAL_CHARACTERISTICS DefaultChars = {
	{ 0, 0, 0 },
	0,
	FilterSendNetBufferLists,
	FilterSendNetBufferListsComplete,
	NULL,
	FilterReceiveNetBufferLists,
	FilterReturnNetBufferLists
};

_Use_decl_annotations_
VOID check(PNET_BUFFER_LIST NetBufferLists)
{
	PNET_BUFFER NetBuffer = NetBufferLists->FirstNetBuffer;
	PNDF_ETH_HEADER pEthHeader = (PNDF_ETH_HEADER)NdisGetDataBuffer(NetBuffer, sizeof(NDF_ETH_HEADER), NULL, 1, 0);
	ULONG DataOffset = 0;

	KdPrint(("eth type: %x mac: src %x-%x-%x-%x-%x-%x dst %x-%x-%x-%x-%x-%x\n", RtlUshortByteSwap(pEthHeader->Type),
		pEthHeader->Src[0], pEthHeader->Src[1], pEthHeader->Src[2], pEthHeader->Src[3], pEthHeader->Src[4], pEthHeader->Src[5],
		pEthHeader->Dst[0], pEthHeader->Dst[1], pEthHeader->Dst[2], pEthHeader->Dst[3], pEthHeader->Dst[4], pEthHeader->Dst[5]));
	if (pEthHeader->Type == RtlUshortByteSwap(ETHERTYPE_IP4))
	{

		NdisAdvanceNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), FALSE, NULL);
		PNDF_IPV4_HEADER pIPv4Header = (PNDF_IPV4_HEADER)NdisGetDataBuffer(NetBuffer, sizeof(NDF_IPV4_HEADER), NULL, 1, 0);

		UNREFERENCED_PARAMETER(pIPv4Header);
		DataOffset = sizeof(NDF_ETH_HEADER);
		KdPrint(("IPv4 src: %d.%d.%d.%d dst: %d.%d.%d.%d\n",
			pIPv4Header->SrcIp[0], pIPv4Header->SrcIp[1], pIPv4Header->SrcIp[2], pIPv4Header->SrcIp[3],
			pIPv4Header->DstIp[0], pIPv4Header->DstIp[1], pIPv4Header->DstIp[2], pIPv4Header->DstIp[3]));
		NdisRetreatNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), 0, NULL);

	}
	else if (pEthHeader->Type == RtlUshortByteSwap(ETHERTYPE_IP6))
	{

		NdisAdvanceNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), FALSE, NULL);
		PNDF_IPV6_HEADER pIPv6Header = (PNDF_IPV6_HEADER)NdisGetDataBuffer(NetBuffer, sizeof(NDF_IPV6_HEADER), NULL, 1, 0);
		DataOffset = sizeof(NDF_ETH_HEADER);
		pIPv6Header = (PNDF_IPV6_HEADER)NdisGetDataBuffer(NetBuffer, sizeof(NDF_IPV6_HEADER), NULL, 1, 0);
		KdPrint(("IPv6 src: %x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x dst: %x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x\n",
			pIPv6Header->SrcAddress[0], pIPv6Header->SrcAddress[1], pIPv6Header->SrcAddress[2], pIPv6Header->SrcAddress[3],
			pIPv6Header->SrcAddress[4], pIPv6Header->SrcAddress[5], pIPv6Header->SrcAddress[6], pIPv6Header->SrcAddress[7],
			pIPv6Header->SrcAddress[8], pIPv6Header->SrcAddress[9], pIPv6Header->SrcAddress[10], pIPv6Header->SrcAddress[11],
			pIPv6Header->SrcAddress[12], pIPv6Header->SrcAddress[13], pIPv6Header->SrcAddress[14], pIPv6Header->SrcAddress[15],
			pIPv6Header->DstAddress[0], pIPv6Header->DstAddress[1], pIPv6Header->DstAddress[2], pIPv6Header->DstAddress[3],
			pIPv6Header->DstAddress[4], pIPv6Header->DstAddress[5], pIPv6Header->DstAddress[6], pIPv6Header->DstAddress[7],
			pIPv6Header->DstAddress[8], pIPv6Header->DstAddress[9], pIPv6Header->DstAddress[10], pIPv6Header->DstAddress[11],
			pIPv6Header->DstAddress[12], pIPv6Header->DstAddress[13], pIPv6Header->DstAddress[14], pIPv6Header->DstAddress[15]));
		NdisRetreatNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), 0, NULL);
	}
	else if (pEthHeader->Type == RtlUshortByteSwap(ETHERTYPE_ARP))
	{
		NdisAdvanceNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), FALSE, NULL);
		PNDF_ARP_HEADER pARPHeader = (PNDF_ARP_HEADER)NdisGetDataBuffer(NetBuffer, sizeof(NDF_ARP_HEADER), NULL, 1, 0);
		DataOffset = sizeof(NDF_ETH_HEADER);
		UNREFERENCED_PARAMETER(pARPHeader);
		KdPrint(("arp srcIp: %d.%d.%d.%d dstIp: %d.%d.%d.%d srcMac: %x-%x-%x-%x-%x-%x dstMac: %x-%x-%x-%x-%x-%x\n",
			pARPHeader->SrcIp[0], pARPHeader->SrcIp[1], pARPHeader->SrcIp[2], pARPHeader->SrcIp[3],
			pARPHeader->DstIp[0], pARPHeader->DstIp[1], pARPHeader->DstIp[2], pARPHeader->DstIp[3],
			pARPHeader->SrcMac[0], pARPHeader->SrcMac[1], pARPHeader->SrcMac[2], pARPHeader->SrcMac[3], pARPHeader->SrcMac[4], pARPHeader->SrcMac[5],
			pARPHeader->DstMac[0], pARPHeader->DstMac[1], pARPHeader->DstMac[2], pARPHeader->DstMac[3], pARPHeader->DstMac[4], pARPHeader->DstMac[5]
			));
		NdisRetreatNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), 0, NULL);

	}
}
_Use_decl_annotations_
NDIS_STATUS FilterRegisterOptions(NDIS_HANDLE  NdisFilterDriverHandle, NDIS_HANDLE  FilterDriverContext) {
	KdPrint(("===>FilterRegisterOptions\n"));

	NT_ASSERT(NdisFilterDriverHandle == FilterDriverHandle);
	NT_ASSERT(FilterDriverContext == (NDIS_HANDLE)FilterDriverObject);

	if ((NdisFilterDriverHandle != (NDIS_HANDLE)FilterDriverHandle) || (FilterDriverContext != (NDIS_HANDLE)FilterDriverObject))
	{
		return NDIS_STATUS_INVALID_PARAMETER;
	}


	return NDIS_STATUS_SUCCESS;
}


_Use_decl_annotations_
NDIS_STATUS FilterAttach(NDIS_HANDLE NdisFilterHandle, NDIS_HANDLE FilterDriverContext, PNDIS_FILTER_ATTACH_PARAMETERS AttachParameters)
{
	PMS_FILTER              pFilter = NULL;
	NDIS_STATUS             Status = NDIS_STATUS_SUCCESS;
	NDIS_FILTER_ATTRIBUTES  FilterAttributes;
	ULONG                   Size;


	KdPrint(("===>FilterAttach: NdisFilterHandle %p\n", NdisFilterHandle));

	NT_ASSERT(FilterDriverContext == (NDIS_HANDLE)FilterDriverObject);
	if (FilterDriverContext != (NDIS_HANDLE)FilterDriverObject)
	{
		Status = NDIS_STATUS_INVALID_PARAMETER;
		return Status;
		//break;
	}

	if ((AttachParameters->MiniportMediaType != NdisMedium802_3) && (AttachParameters->MiniportMediaType != NdisMediumWan) && (AttachParameters->MiniportMediaType != NdisMediumWirelessWan))
	{
		KdPrint(("\n"));

		Status = NDIS_STATUS_INVALID_PARAMETER;
		return Status;
		//break;
	}

	Size = sizeof(MS_FILTER) + AttachParameters->FilterModuleGuidName->Length + AttachParameters->BaseMiniportInstanceName->Length + AttachParameters->BaseMiniportName->Length;

	pFilter = (PMS_FILTER)NdisAllocateMemoryWithTagPriority(NdisFilterHandle, Size, FILTER_ALLOC_TAG, LowPoolPriority);
	if (pFilter == NULL)
	{
		KdPrint(("\n"));
		Status = NDIS_STATUS_RESOURCES;
		return Status;
		//break;
	}

	NdisZeroMemory(pFilter, sizeof(MS_FILTER));

	pFilter->FilterModuleName.Length = pFilter->FilterModuleName.MaximumLength = AttachParameters->FilterModuleGuidName->Length;
	pFilter->FilterModuleName.Buffer = (PWSTR)((PUCHAR)pFilter + sizeof(MS_FILTER));
	NdisMoveMemory(pFilter->FilterModuleName.Buffer, AttachParameters->FilterModuleGuidName->Buffer, pFilter->FilterModuleName.Length);



	pFilter->MiniportFriendlyName.Length = pFilter->MiniportFriendlyName.MaximumLength = AttachParameters->BaseMiniportInstanceName->Length;
	pFilter->MiniportFriendlyName.Buffer = (PWSTR)((PUCHAR)pFilter->FilterModuleName.Buffer + pFilter->FilterModuleName.Length);
	NdisMoveMemory(pFilter->MiniportFriendlyName.Buffer, AttachParameters->BaseMiniportInstanceName->Buffer, pFilter->MiniportFriendlyName.Length);


	pFilter->MiniportName.Length = pFilter->MiniportName.MaximumLength = AttachParameters->BaseMiniportName->Length;
	pFilter->MiniportName.Buffer = (PWSTR)((PUCHAR)pFilter->MiniportFriendlyName.Buffer + pFilter->MiniportFriendlyName.Length);
	NdisMoveMemory(pFilter->MiniportName.Buffer, AttachParameters->BaseMiniportName->Buffer, pFilter->MiniportName.Length);

	pFilter->MiniportIfIndex = AttachParameters->BaseMiniportIfIndex;

	pFilter->TrackReceives = TRUE;
	pFilter->TrackSends = TRUE;
	pFilter->FilterHandle = NdisFilterHandle;


	NdisZeroMemory(&FilterAttributes, sizeof(NDIS_FILTER_ATTRIBUTES));
	FilterAttributes.Header.Revision = NDIS_FILTER_ATTRIBUTES_REVISION_1;
	FilterAttributes.Header.Size = sizeof(NDIS_FILTER_ATTRIBUTES);
	FilterAttributes.Header.Type = NDIS_OBJECT_TYPE_FILTER_ATTRIBUTES;
	FilterAttributes.Flags = 0;

	NDIS_DECLARE_FILTER_MODULE_CONTEXT(MS_FILTER);
	Status = NdisFSetAttributes(NdisFilterHandle, pFilter, &FilterAttributes);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		KdPrint(("\n"));
		return Status;
		//break;
	}

	pFilter->State = FilterPaused;

	NdisAcquireSpinLock(&FilterListLock);
	InsertHeadList(&FilterModuleList, &pFilter->FilterModuleLink);
	NdisReleaseSpinLock(&FilterListLock);


	if (Status != NDIS_STATUS_SUCCESS)
	{
		if (pFilter != NULL)
		{
			NdisFreeMemory(pFilter, 0, 0);
		}
	}

	return Status;
}

_Use_decl_annotations_
NDIS_STATUS FilterPause(NDIS_HANDLE FilterModuleContext, PNDIS_FILTER_PAUSE_PARAMETERS PauseParameters) {
	PMS_FILTER          pFilter = (PMS_FILTER)(FilterModuleContext);
	NDIS_STATUS         Status;

	UNREFERENCED_PARAMETER(PauseParameters);

	KdPrint(("===>NDISFilterDriver FilterPause: FilterInstance %p\n", FilterModuleContext));
	NT_ASSERT(pFilter->State == FilterRunning);

	NdisAcquireSpinLock(&pFilter->Lock);
	pFilter->State = FilterPausing;
	NdisReleaseSpinLock(&pFilter->Lock);

	Status = NDIS_STATUS_SUCCESS;

	pFilter->State = FilterPaused;

	return Status;
}

_Use_decl_annotations_
NDIS_STATUS FilterRestart(NDIS_HANDLE FilterModuleContext, PNDIS_FILTER_RESTART_PARAMETERS RestartParameters)
{
	NDIS_STATUS     Status;
	PMS_FILTER      pFilter = (PMS_FILTER)FilterModuleContext;
	NDIS_HANDLE     ConfigurationHandle = NULL;


	PNDIS_RESTART_GENERAL_ATTRIBUTES NdisGeneralAttributes;
	PNDIS_RESTART_ATTRIBUTES         NdisRestartAttributes;
	NDIS_CONFIGURATION_OBJECT        ConfigObject;

	KdPrint(("===>FilterRestart:   FilterModuleContext %p\n", FilterModuleContext));

	ASSERT(pFilter->State == FilterPaused);

	ConfigObject.Header.Type = NDIS_OBJECT_TYPE_CONFIGURATION_OBJECT;
	ConfigObject.Header.Revision = NDIS_CONFIGURATION_OBJECT_REVISION_1;
	ConfigObject.Header.Size = sizeof(NDIS_CONFIGURATION_OBJECT);
	ConfigObject.NdisHandle = FilterDriverHandle;
	ConfigObject.Flags = 0;

	Status = NdisOpenConfigurationEx(&ConfigObject, &ConfigurationHandle);
	if (Status != NDIS_STATUS_SUCCESS)
	{

	}


	if (Status == NDIS_STATUS_SUCCESS)
	{
		NdisCloseConfiguration(ConfigurationHandle);
	}

	NdisRestartAttributes = RestartParameters->RestartAttributes;

	if (NdisRestartAttributes != NULL)
	{
		PNDIS_RESTART_ATTRIBUTES   NextAttributes;

		ASSERT(NdisRestartAttributes->Oid == OID_GEN_MINIPORT_RESTART_ATTRIBUTES);

		NdisGeneralAttributes = (PNDIS_RESTART_GENERAL_ATTRIBUTES)NdisRestartAttributes->Data;

		NdisGeneralAttributes->LookaheadSize = 128;

		NextAttributes = NdisRestartAttributes->Next;

		while (NextAttributes != NULL)
		{

			NextAttributes = NextAttributes->Next;
		}




	}


	pFilter->State = FilterRunning;


	Status = NDIS_STATUS_SUCCESS;


	if (Status != NDIS_STATUS_SUCCESS)
	{
		pFilter->State = FilterPaused;
	}


	return Status;
}


_Use_decl_annotations_
VOID FilterDetach(NDIS_HANDLE     FilterModuleContext)
{
	PMS_FILTER                  pFilter = (PMS_FILTER)FilterModuleContext;
	BOOLEAN                      bFalse = FALSE;


	KdPrint(("===>FilterDetach:    FilterInstance %p\n", FilterModuleContext));


	ASSERT(pFilter->State == FilterPaused);


	if (pFilter->FilterName.Buffer != NULL)
	{
		FILTER_FREE_MEM(pFilter->FilterName.Buffer);
	}


	FILTER_ACQUIRE_LOCK(&FilterListLock, bFalse);
	RemoveEntryList(&pFilter->FilterModuleLink);
	FILTER_RELEASE_LOCK(&FilterListLock, bFalse);


	FILTER_FREE_MEM(pFilter);

	KdPrint(("<===FilterDetach Successfully\n"));
	return;
}

_Use_decl_annotations_
NDIS_STATUS FilterOidRequest(NDIS_HANDLE FilterModuleContext, PNDIS_OID_REQUEST Request) {
	PMS_FILTER              pFilter = (PMS_FILTER)FilterModuleContext;
	NDIS_STATUS             Status;
	PNDIS_OID_REQUEST       ClonedRequest = NULL;
	BOOLEAN                 bSubmitted = FALSE;
	PFILTER_REQUEST_CONTEXT Context;
	BOOLEAN                 bFalse = FALSE;


	KdPrint(("===>FilterOidRequest: Request %p.\n", Request));


	do
	{
		Status = NdisAllocateCloneOidRequest(pFilter->FilterHandle,
			Request,
			FILTER_TAG,
			&ClonedRequest);
		if (Status != NDIS_STATUS_SUCCESS)
		{
			KdPrint(("FilerOidRequest: Cannot Clone Request\n"));
			break;
		}

		Context = (PFILTER_REQUEST_CONTEXT)(&ClonedRequest->SourceReserved[0]);
		*Context = Request;

		bSubmitted = TRUE;

		ClonedRequest->RequestId = Request->RequestId;

		pFilter->PendingOidRequest = ClonedRequest;


		Status = NdisFOidRequest(pFilter->FilterHandle, ClonedRequest);

		if (Status != NDIS_STATUS_PENDING)
		{


			FilterOidRequestComplete(pFilter, ClonedRequest, Status);
			Status = NDIS_STATUS_PENDING;
		}



	} while (bFalse);

	if (bSubmitted == FALSE)
	{
		switch (Request->RequestType)
		{
		case NdisRequestMethod:
			Request->DATA.METHOD_INFORMATION.BytesRead = 0;
			Request->DATA.METHOD_INFORMATION.BytesNeeded = 0;
			Request->DATA.METHOD_INFORMATION.BytesWritten = 0;
			break;

		case NdisRequestSetInformation:
			Request->DATA.SET_INFORMATION.BytesRead = 0;
			Request->DATA.SET_INFORMATION.BytesNeeded = 0;
			break;

		case NdisRequestQueryInformation:
		case NdisRequestQueryStatistics:
		default:
			Request->DATA.QUERY_INFORMATION.BytesWritten = 0;
			Request->DATA.QUERY_INFORMATION.BytesNeeded = 0;
			break;
		}

	}
	KdPrint(("<===FilterOidRequest: Status %8x.\n", Status));

	return Status;

}

_Use_decl_annotations_
VOID FilterCancelOidRequest(NDIS_HANDLE FilterModuleContext, PVOID RequestId) {
	PMS_FILTER                          pFilter = (PMS_FILTER)FilterModuleContext;
	PNDIS_OID_REQUEST                   Request = NULL;
	PFILTER_REQUEST_CONTEXT             Context;
	PNDIS_OID_REQUEST                   OriginalRequest = NULL;
	BOOLEAN                             bFalse = FALSE;

	FILTER_ACQUIRE_LOCK(&pFilter->Lock, bFalse);

	Request = pFilter->PendingOidRequest;

	if (Request != NULL)
	{
		Context = (PFILTER_REQUEST_CONTEXT)(&Request->SourceReserved[0]);

		OriginalRequest = (*Context);
	}

	if ((OriginalRequest != NULL) && (OriginalRequest->RequestId == RequestId))
	{
		FILTER_RELEASE_LOCK(&pFilter->Lock, bFalse);

		NdisFCancelOidRequest(pFilter->FilterHandle, RequestId);
	}
	else
	{
		FILTER_RELEASE_LOCK(&pFilter->Lock, bFalse);
	}


}

_Use_decl_annotations_
VOID FilterOidRequestComplete(NDIS_HANDLE FilterModuleContext, PNDIS_OID_REQUEST Request, NDIS_STATUS Status)
{
	PMS_FILTER                          pFilter = (PMS_FILTER)FilterModuleContext;
	PNDIS_OID_REQUEST                   OriginalRequest;
	PFILTER_REQUEST_CONTEXT             Context;
	BOOLEAN                             bFalse = FALSE;

	KdPrint(("===>FilterOidRequestComplete, Request %p.\n", Request));

	Context = (PFILTER_REQUEST_CONTEXT)(&Request->SourceReserved[0]);
	OriginalRequest = (*Context);

	if (OriginalRequest == NULL)
	{
		filterInternalRequestComplete(pFilter, Request, Status);
		return;
	}



	FILTER_ACQUIRE_LOCK(&pFilter->Lock, bFalse);

	ASSERT(pFilter->PendingOidRequest == Request);
	pFilter->PendingOidRequest = NULL;

	FILTER_RELEASE_LOCK(&pFilter->Lock, bFalse);


	switch (Request->RequestType)
	{
	case NdisRequestMethod:
		OriginalRequest->DATA.METHOD_INFORMATION.OutputBufferLength = Request->DATA.METHOD_INFORMATION.OutputBufferLength;
		OriginalRequest->DATA.METHOD_INFORMATION.BytesRead = Request->DATA.METHOD_INFORMATION.BytesRead;
		OriginalRequest->DATA.METHOD_INFORMATION.BytesNeeded = Request->DATA.METHOD_INFORMATION.BytesNeeded;
		OriginalRequest->DATA.METHOD_INFORMATION.BytesWritten = Request->DATA.METHOD_INFORMATION.BytesWritten;
		break;

	case NdisRequestSetInformation:
		OriginalRequest->DATA.SET_INFORMATION.BytesRead = Request->DATA.SET_INFORMATION.BytesRead;
		OriginalRequest->DATA.SET_INFORMATION.BytesNeeded = Request->DATA.SET_INFORMATION.BytesNeeded;
		break;

	case NdisRequestQueryInformation:
	case NdisRequestQueryStatistics:
	default:
		OriginalRequest->DATA.QUERY_INFORMATION.BytesWritten = Request->DATA.QUERY_INFORMATION.BytesWritten;
		OriginalRequest->DATA.QUERY_INFORMATION.BytesNeeded = Request->DATA.QUERY_INFORMATION.BytesNeeded;
		break;
	}


	(*Context) = NULL;

	NdisFreeCloneOidRequest(pFilter->FilterHandle, Request);

	NdisFOidRequestComplete(pFilter->FilterHandle, OriginalRequest, Status);

	KdPrint(("<===FilterOidRequestComplete.\n"));
}


_Use_decl_annotations_
VOID FilterStatus(NDIS_HANDLE FilterModuleContext, PNDIS_STATUS_INDICATION StatusIndication)
{
	PMS_FILTER              pFilter = (PMS_FILTER)FilterModuleContext;

	KdPrint(("===>FilterStatus, IndicateStatus = %8x.\n", StatusIndication->StatusCode));



	NdisFIndicateStatus(pFilter->FilterHandle, StatusIndication);


	KdPrint(("<===FilterStatus.\n"));

}

_Use_decl_annotations_
VOID FilterDevicePnPEventNotify(NDIS_HANDLE FilterModuleContext, PNET_DEVICE_PNP_EVENT NetDevicePnPEvent)
{
	PMS_FILTER             pFilter = (PMS_FILTER)FilterModuleContext;
	NDIS_DEVICE_PNP_EVENT  DevicePnPEvent = NetDevicePnPEvent->DevicePnPEvent;

	KdPrint(("===>FilterDevicePnPEventNotify: NetPnPEvent = %p.\n", NetDevicePnPEvent));


	switch (DevicePnPEvent)
	{

	case NdisDevicePnPEventQueryRemoved:
	case NdisDevicePnPEventRemoved:
	case NdisDevicePnPEventSurpriseRemoved:
	case NdisDevicePnPEventQueryStopped:
	case NdisDevicePnPEventStopped:
	case NdisDevicePnPEventPowerProfileChanged:
	case NdisDevicePnPEventFilterListChanged:

		break;

	default:
		KdPrint(("FilterDevicePnPEventNotify: Invalid event.\n"));

		return;
		break;
	}

	NdisFDevicePnPEventNotify(pFilter->FilterHandle, NetDevicePnPEvent);

	KdPrint(("<===FilterDevicePnPEventNotify\n"));

}

_Use_decl_annotations_
NDIS_STATUS FilterNetPnPEvent(NDIS_HANDLE FilterModuleContext, PNET_PNP_EVENT_NOTIFICATION NetPnPEventNotification)
{
	PMS_FILTER                pFilter = (PMS_FILTER)FilterModuleContext;
	NDIS_STATUS               Status = NDIS_STATUS_SUCCESS;

	Status = NdisFNetPnPEvent(pFilter->FilterHandle, NetPnPEventNotification);

	return Status;
}

_Use_decl_annotations_
VOID FilterSendNetBufferListsComplete(NDIS_HANDLE FilterModuleContext, PNET_BUFFER_LIST NetBufferLists, ULONG SendCompleteFlags)
{
	DbgPrint("FilterSendNetBufferListsComplete\n");
	check(NetBufferLists);
	PMS_FILTER         pFilter = (PMS_FILTER)FilterModuleContext;
	ULONG              NumOfSendCompletes = 0;
	BOOLEAN            DispatchLevel;
	PNET_BUFFER_LIST   CurrNbl;

	KdPrint(("===>SendNBLComplete, NetBufferList: %p.\n", NetBufferLists));



	if (pFilter->TrackSends)
	{
		CurrNbl = NetBufferLists;
		while (CurrNbl)
		{
			NumOfSendCompletes++;
			CurrNbl = NET_BUFFER_LIST_NEXT_NBL(CurrNbl);

		}
		DispatchLevel = NDIS_TEST_SEND_AT_DISPATCH_LEVEL(SendCompleteFlags);
		FILTER_ACQUIRE_LOCK(&pFilter->Lock, DispatchLevel);
		pFilter->OutstandingSends -= NumOfSendCompletes;
		FILTER_LOG_SEND_REF(2, pFilter, PrevNbl, pFilter->OutstandingSends);
		FILTER_RELEASE_LOCK(&pFilter->Lock, DispatchLevel);
	}


	NdisFSendNetBufferListsComplete(pFilter->FilterHandle, NetBufferLists, SendCompleteFlags);

	KdPrint(("<===SendNBLComplete.\n"));
}

_Use_decl_annotations_
VOID FilterSendNetBufferLists(NDIS_HANDLE FilterModuleContext, PNET_BUFFER_LIST NetBufferLists, NDIS_PORT_NUMBER PortNumber, ULONG SendFlags)
{
	DbgPrint("FilterSendNetBufferLists\n");
	check(NetBufferLists);
	PMS_FILTER          pFilter = (PMS_FILTER)FilterModuleContext;
	PNET_BUFFER_LIST    CurrNbl;
	BOOLEAN             DispatchLevel;
	BOOLEAN             bFalse = FALSE;

	KdPrint(("===>SendNetBufferList: NBL = %p.\n", NetBufferLists));

	do
	{

		DispatchLevel = NDIS_TEST_SEND_AT_DISPATCH_LEVEL(SendFlags);
		if (pFilter->TrackSends)
		{
			FILTER_ACQUIRE_LOCK(&pFilter->Lock, DispatchLevel);
			CurrNbl = NetBufferLists;
			while (CurrNbl)
			{
				pFilter->OutstandingSends++;
				FILTER_LOG_SEND_REF(1, pFilter, CurrNbl, pFilter->OutstandingSends);

				CurrNbl = NET_BUFFER_LIST_NEXT_NBL(CurrNbl);
			}
			FILTER_RELEASE_LOCK(&pFilter->Lock, DispatchLevel);
		}



		KIRQL OldIrql;
		KeAcquireSpinLock(&pFilterDeviceExtension->QLock, &OldIrql);
		BOOLEAN IsValid = CheckSentPacket(NetBufferLists->FirstNetBuffer);
		KeReleaseSpinLock(&pFilterDeviceExtension->QLock, OldIrql);
		if (IsValid)
			NdisFSendNetBufferLists(pFilter->FilterHandle, NetBufferLists, PortNumber, SendFlags);
		else
			NdisFSendNetBufferListsComplete(pFilter->FilterHandle, NetBufferLists, SendFlags);
	} while (bFalse);

	KdPrint(("<===SendNetBufferList. \n"));
}

_Use_decl_annotations_
VOID FilterReturnNetBufferLists(NDIS_HANDLE FilterModuleContext, PNET_BUFFER_LIST  NetBufferLists, ULONG ReturnFlags) {
	DbgPrint("FilterReturnNetBufferLists\n");
	check(NetBufferLists);
	PMS_FILTER          pFilter = (PMS_FILTER)FilterModuleContext;
	PNET_BUFFER_LIST    CurrNbl = NetBufferLists;
	UINT                NumOfNetBufferLists = 0;
	BOOLEAN             DispatchLevel;
	ULONG               Ref;

	KdPrint(("===>ReturnNetBufferLists, NetBufferLists is %p.\n", NetBufferLists));


	if (pFilter->TrackReceives)
	{
		while (CurrNbl)
		{
			NumOfNetBufferLists++;
			CurrNbl = NET_BUFFER_LIST_NEXT_NBL(CurrNbl);
		}
	}

	NdisFReturnNetBufferLists(pFilter->FilterHandle, NetBufferLists, ReturnFlags);

	if (pFilter->TrackReceives)
	{
		DispatchLevel = NDIS_TEST_RETURN_AT_DISPATCH_LEVEL(ReturnFlags);
		FILTER_ACQUIRE_LOCK(&pFilter->Lock, DispatchLevel);

		pFilter->OutstandingRcvs -= NumOfNetBufferLists;
		Ref = pFilter->OutstandingRcvs;
		FILTER_LOG_RCV_REF(3, pFilter, NetBufferLists, Ref);
		FILTER_RELEASE_LOCK(&pFilter->Lock, DispatchLevel);
	}


	KdPrint(("<===ReturnNetBufferLists.\n"));


}


_Use_decl_annotations_
VOID
FilterReceiveNetBufferLists(
	NDIS_HANDLE         FilterModuleContext,
	PNET_BUFFER_LIST    NetBufferLists,
	NDIS_PORT_NUMBER    PortNumber,
	ULONG               NumberOfNetBufferLists,
	ULONG               ReceiveFlags
)
{
	KdPrint(("FilterReceiveNetBufferLists\n"));
	PMS_FILTER          pFilter = (PMS_FILTER)FilterModuleContext;
	check(NetBufferLists);
	BOOLEAN             DispatchLevel;
	ULONG               Ref;
	BOOLEAN             bFalse = FALSE;

	KdPrint(("===>ReceiveNetBufferList: NetBufferLists = %p.\n", NetBufferLists));
	do
	{

		DispatchLevel = NDIS_TEST_RECEIVE_AT_DISPATCH_LEVEL(ReceiveFlags);

		ASSERT(NumberOfNetBufferLists >= 1);



		if (pFilter->TrackReceives)
		{
			FILTER_ACQUIRE_LOCK(&pFilter->Lock, DispatchLevel);
			pFilter->OutstandingRcvs += NumberOfNetBufferLists;
			Ref = pFilter->OutstandingRcvs;

			FILTER_LOG_RCV_REF(1, pFilter, NetBufferLists, Ref);
			FILTER_RELEASE_LOCK(&pFilter->Lock, DispatchLevel);
		}

		KIRQL OldIrql;
		KeAcquireSpinLock(&pFilterDeviceExtension->QLock, &OldIrql);
		BOOLEAN IsValid = CheckReceivePacket(NetBufferLists->FirstNetBuffer);
		KeReleaseSpinLock(&pFilterDeviceExtension->QLock, OldIrql);

		if (IsValid)
			NdisFIndicateReceiveNetBufferLists(
				pFilter->FilterHandle,
				NetBufferLists,
				PortNumber,
				NumberOfNetBufferLists,
				ReceiveFlags);
		else
			NdisFReturnNetBufferLists(pFilter->FilterHandle, NetBufferLists, ReceiveFlags);


		if (NDIS_TEST_RECEIVE_CANNOT_PEND(ReceiveFlags) &&
			pFilter->TrackReceives)
		{
			FILTER_ACQUIRE_LOCK(&pFilter->Lock, DispatchLevel);
			pFilter->OutstandingRcvs -= NumberOfNetBufferLists;
			Ref = pFilter->OutstandingRcvs;
			FILTER_LOG_RCV_REF(2, pFilter, NetBufferLists, Ref);
			FILTER_RELEASE_LOCK(&pFilter->Lock, DispatchLevel);
		}

	} while (bFalse);

	KdPrint(("<===ReceiveNetBufferList: Flags = %8x.\n", ReceiveFlags));

}


_Use_decl_annotations_
VOID FilterCancelSendNetBufferLists(
	NDIS_HANDLE             FilterModuleContext,
	PVOID                   CancelId
)
{
	PMS_FILTER  pFilter = (PMS_FILTER)FilterModuleContext;

	NdisFCancelSendNetBufferLists(pFilter->FilterHandle, CancelId);
}


_Use_decl_annotations_
NDIS_STATUS FilterSetModuleOptions(NDIS_HANDLE FilterModuleContext)
{
	PMS_FILTER                               pFilter = (PMS_FILTER)FilterModuleContext;
	NDIS_FILTER_PARTIAL_CHARACTERISTICS      OptionalHandlers;
	NDIS_STATUS                              Status = NDIS_STATUS_SUCCESS;
	BOOLEAN                                  bFalse = FALSE;

	if (bFalse)
	{
		UINT      i;


		pFilter->CallsRestart++;

		i = pFilter->CallsRestart % 8;

		pFilter->TrackReceives = TRUE;
		pFilter->TrackSends = TRUE;

		NdisMoveMemory(&OptionalHandlers, &DefaultChars, sizeof(OptionalHandlers));
		OptionalHandlers.Header.Type = NDIS_OBJECT_TYPE_FILTER_PARTIAL_CHARACTERISTICS;
		OptionalHandlers.Header.Size = sizeof(OptionalHandlers);
		switch (i)
		{

		case 0:
			OptionalHandlers.ReceiveNetBufferListsHandler = NULL;
			pFilter->TrackReceives = FALSE;
			break;

		case 1:

			OptionalHandlers.ReturnNetBufferListsHandler = NULL;
			pFilter->TrackReceives = FALSE;
			break;

		case 2:
			OptionalHandlers.SendNetBufferListsHandler = NULL;
			pFilter->TrackSends = FALSE;
			break;

		case 3:
			OptionalHandlers.SendNetBufferListsCompleteHandler = NULL;
			pFilter->TrackSends = FALSE;
			break;

		case 4:
			OptionalHandlers.ReceiveNetBufferListsHandler = NULL;
			OptionalHandlers.ReturnNetBufferListsHandler = NULL;
			break;

		case 5:
			OptionalHandlers.SendNetBufferListsHandler = NULL;
			OptionalHandlers.SendNetBufferListsCompleteHandler = NULL;
			break;

		case 6:

			OptionalHandlers.ReceiveNetBufferListsHandler = NULL;
			OptionalHandlers.ReturnNetBufferListsHandler = NULL;
			OptionalHandlers.SendNetBufferListsHandler = NULL;
			OptionalHandlers.SendNetBufferListsCompleteHandler = NULL;
			break;

		case 7:
			break;
		}
		Status = NdisSetOptionalHandlers(pFilter->FilterHandle, (PNDIS_DRIVER_OPTIONAL_HANDLERS)&OptionalHandlers);
	}
	return Status;
}



VOID
filterInternalRequestComplete(
	_In_ NDIS_HANDLE                  FilterModuleContext,
	_In_ PNDIS_OID_REQUEST            NdisRequest,
	_In_ NDIS_STATUS                  Status
)
{
	PFILTER_REQUEST              FilterRequest;


	UNREFERENCED_PARAMETER(FilterModuleContext);
	FilterRequest = CONTAINING_RECORD(NdisRequest, FILTER_REQUEST, Request);

	FilterRequest->Status = Status;
	NdisSetEvent(&FilterRequest->ReqEvent);
}

BOOLEAN CheckSentPacket(PNET_BUFFER NetBuffer)
{
	// net buffer should have PNDIS_PACKET in the stack
	PRULES_LISTS RulesLists = (PRULES_LISTS)pFilterDeviceExtension->FilterRules;
	if (!RulesLists->IsActive)
	{
		return TRUE;
	}
	PNDF_ETH_HEADER pEthHeader;
	PNDF_IPV4_HEADER pIPv4Header;
	PNDF_IPV6_HEADER pIPv6Header;
	PNDF_ARP_HEADER pArpHeader;
	ULONG DataOffset = 0;
	pEthHeader = (PNDF_ETH_HEADER)NdisGetDataBuffer(NetBuffer, sizeof(NDF_ETH_HEADER), NULL, 1, 0);
	USHORT ProtocolType = RtlUshortByteSwap(pEthHeader->Type);
	BOOLEAN IsValid = FALSE;
	switch (ProtocolType)
	{
	case ETHERTYPE_IP4:
		NdisAdvanceNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), FALSE, NULL);
		DataOffset = sizeof(NDF_ETH_HEADER);
		pIPv4Header = (PNDF_IPV4_HEADER)NdisGetDataBuffer(NetBuffer, sizeof(NDF_IPV4_HEADER), NULL, 1, 0);
		IsValid = CheckSentIPv4Header(pIPv4Header);
		NdisRetreatNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), 0, NULL);
		break;
	case ETHERTYPE_IP6:
		NdisAdvanceNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), FALSE, NULL);
		DataOffset = sizeof(NDF_ETH_HEADER);
		pIPv6Header = (PNDF_IPV6_HEADER)NdisGetDataBuffer(NetBuffer, sizeof(NDF_IPV6_HEADER), NULL, 1, 0);
		IsValid = CheckSentIPv6Header(pIPv6Header);
		NdisRetreatNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), 0, NULL);
		break;
	case ETHERTYPE_ARP:
		NdisAdvanceNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), FALSE, NULL);
		DataOffset = sizeof(NDF_ETH_HEADER);
		pArpHeader = (PNDF_ARP_HEADER)NdisGetDataBuffer(NetBuffer, sizeof(NDF_ARP_HEADER), NULL, 1, 0);
		IsValid = CheckSentArpHeader(pArpHeader);
		NdisRetreatNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), 0, NULL);
		break;
	}
	return IsValid;
}

BOOLEAN CheckSentIPv4Header(PNDF_IPV4_HEADER IPv4Header)
{
	PRULES_LISTS RulesLists = (PRULES_LISTS)pFilterDeviceExtension->FilterRules;
	PRULE_IPV4 CurrentRule = (PRULE_IPV4)RulesLists->FirstRuleIPv4;
	BOOLEAN IsValid = FALSE;

	while (CurrentRule != NULL)
	{
		BOOLEAN InRange = TRUE;

		for (int i = 0; i < 4; i++)
		{
			BOOLEAN tmp1 = (IPv4Header->DstIp[i] >= CurrentRule->Begin[i]);
			BOOLEAN tmp2 = (IPv4Header->DstIp[i] <= CurrentRule->End[i]);
			InRange = tmp1 && tmp2;
		}
		IsValid = InRange;
		if (IsValid)
			break;
		CurrentRule = (PRULE_IPV4)CurrentRule->Next;
	}

	return IsValid;
}

BOOLEAN CheckSentIPv6Header(PNDF_IPV6_HEADER IPv6Header)
{
	PRULES_LISTS RulesLists = (PRULES_LISTS)pFilterDeviceExtension->FilterRules;
	PRULE_IPV4 CurrentRule = (PRULE_IPV4)RulesLists->FirstRuleIPv6;
	BOOLEAN IsValid = FALSE;

	while (CurrentRule != NULL)
	{
		BOOLEAN InRange = TRUE;
		for (int i = 0; i < 16; i++)
		{
			BOOLEAN tmp1 = (IPv6Header->DstAddress[i] >= CurrentRule->Begin[i]);
			BOOLEAN tmp2 = (IPv6Header->DstAddress[i] <= CurrentRule->End[i]);
			InRange = tmp1 && tmp2;
		}
		IsValid = InRange;
		if (IsValid)
			break;
		CurrentRule = (PRULE_IPV4)CurrentRule->Next;
	}

	return IsValid;
}

BOOLEAN CheckSentArpHeader(PNDF_ARP_HEADER ArpHeader)
{
	PRULES_LISTS RulesLists = (PRULES_LISTS)pFilterDeviceExtension->FilterRules;
	PRULE_IPV4 CurrentRule = (PRULE_IPV4)RulesLists->FirstRuleIPv4;
	BOOLEAN IsValid = FALSE;

	while (CurrentRule != NULL)
	{
		BOOLEAN InRange = TRUE;
		for (int i = 0; i < 4; i++)
		{
			BOOLEAN tmp1 = (ArpHeader->DstIp[i] >= CurrentRule->Begin[i]);
			BOOLEAN tmp2 = (ArpHeader->DstIp[i] <= CurrentRule->End[i]);
			InRange = tmp1 && tmp2;
		}
		IsValid = InRange;
		if (IsValid)
			break;
		CurrentRule = (PRULE_IPV4)CurrentRule->Next;
	}

	return IsValid;
}

BOOLEAN CheckReceivePacket(PNET_BUFFER NetBuffer)
{
	PRULES_LISTS RulesLists = (PRULES_LISTS)pFilterDeviceExtension->FilterRules;
	if (!RulesLists->IsActive)
	{
		return TRUE;
	}
	PNDF_ETH_HEADER pEthHeader;
	PNDF_IPV4_HEADER pIPv4Header;
	PNDF_IPV6_HEADER pIPv6Header;
	PNDF_ARP_HEADER pArpHeader;
	ULONG DataOffset = 0;
	pEthHeader = (PNDF_ETH_HEADER)NdisGetDataBuffer(NetBuffer, sizeof(NDF_ETH_HEADER), NULL, 1, 0);
	USHORT ProtocolType = RtlUshortByteSwap(pEthHeader->Type);
	BOOLEAN IsValid = FALSE;
	switch (ProtocolType)
	{
	case ETHERTYPE_IP4:
		NdisAdvanceNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), FALSE, NULL);
		DataOffset = sizeof(NDF_ETH_HEADER);
		pIPv4Header = (PNDF_IPV4_HEADER)NdisGetDataBuffer(NetBuffer, sizeof(NDF_IPV4_HEADER), NULL, 1, 0);
		IsValid = CheckReceiveIPv4Header(pIPv4Header);
		NdisRetreatNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), 0, NULL);
		break;
	case ETHERTYPE_IP6:
		NdisAdvanceNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), FALSE, NULL);
		DataOffset = sizeof(NDF_ETH_HEADER);
		pIPv6Header = (PNDF_IPV6_HEADER)NdisGetDataBuffer(NetBuffer, sizeof(NDF_IPV6_HEADER), NULL, 1, 0);
		IsValid = CheckReceiveIPv6Header(pIPv6Header);
		NdisRetreatNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), 0, NULL);
		break;
	case ETHERTYPE_ARP:
		NdisAdvanceNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), FALSE, NULL);
		DataOffset = sizeof(NDF_ETH_HEADER);
		pArpHeader = (PNDF_ARP_HEADER)NdisGetDataBuffer(NetBuffer, sizeof(NDF_ARP_HEADER), NULL, 1, 0);
		IsValid = CheckReceiveArpHeader(pArpHeader);
		NdisRetreatNetBufferDataStart(NetBuffer, sizeof(NDF_ETH_HEADER), 0, NULL);
		break;
	}
	return IsValid;
}

BOOLEAN CheckReceiveIPv4Header(PNDF_IPV4_HEADER IPv4Header)
{
	PRULES_LISTS RulesLists = (PRULES_LISTS)pFilterDeviceExtension->FilterRules;
	PRULE_IPV4 CurrentRule = (PRULE_IPV4)RulesLists->FirstRuleIPv4;
	BOOLEAN IsValid = FALSE;

	while (CurrentRule != NULL)
	{
		BOOLEAN InRange = TRUE;
		for (size_t i = 0; i < 4; i++)
		{
			BOOLEAN tmp1 = (IPv4Header->SrcIp[i] >= CurrentRule->Begin[i]);
			BOOLEAN tmp2 = (IPv4Header->SrcIp[i] <= CurrentRule->End[i]);
			InRange = tmp1 && tmp2;
		}
		IsValid = InRange;
		if (IsValid)
			break;
		CurrentRule = (PRULE_IPV4)CurrentRule->Next;
	}

	return IsValid;
}

BOOLEAN CheckReceiveIPv6Header(PNDF_IPV6_HEADER IPv6Header)
{
	PRULES_LISTS RulesLists = (PRULES_LISTS)pFilterDeviceExtension->FilterRules;
	PRULE_IPV4 CurrentRule = (PRULE_IPV4)RulesLists->FirstRuleIPv6;
	BOOLEAN IsValid = FALSE;

	while (CurrentRule != NULL)
	{
		BOOLEAN InRange = TRUE;
		for (int i = 0; i < 16; i++)
		{
			BOOLEAN tmp1 = (IPv6Header->SrcAddress[i] >= CurrentRule->Begin[i]);
			BOOLEAN tmp2 = (IPv6Header->SrcAddress[i] <= CurrentRule->End[i]);
			InRange = tmp1 && tmp2;
		}
		IsValid = InRange;
		if (IsValid)
			break;
		CurrentRule = (PRULE_IPV4)CurrentRule->Next;
	}

	return IsValid;
}

BOOLEAN CheckReceiveArpHeader(PNDF_ARP_HEADER ArpHeader)
{
	PRULES_LISTS RulesLists = (PRULES_LISTS)pFilterDeviceExtension->FilterRules;
	PRULE_IPV4 CurrentRule = (PRULE_IPV4)RulesLists->FirstRuleIPv4;
	BOOLEAN IsValid = FALSE;

	while (CurrentRule != NULL)
	{
		BOOLEAN InRange = TRUE;
		for (size_t i = 0; i < 4; i++)
		{
			BOOLEAN tmp1 = (ArpHeader->SrcIp[i] >= CurrentRule->Begin[i]);
			BOOLEAN tmp2 = (ArpHeader->SrcIp[i] <= CurrentRule->End[i]);
			InRange = tmp1 && tmp2;
		}
		IsValid = InRange;
		if (IsValid)
			break;
		CurrentRule = (PRULE_IPV4)CurrentRule->Next;
	}

	return IsValid;
}





_IRQL_requires_max_(DISPATCH_LEVEL)
PMS_FILTER filterFindFilterModule(_In_reads_bytes_(BufferLength) PUCHAR Buffer, _In_ ULONG BufferLength)
{

	PMS_FILTER              pFilter;
	PLIST_ENTRY             Link;
	BOOLEAN                  bFalse = FALSE;

	FILTER_ACQUIRE_LOCK(&FilterListLock, bFalse);

	Link = FilterModuleList.Flink;

	while (Link != &FilterModuleList)
	{
		pFilter = CONTAINING_RECORD(Link, MS_FILTER, FilterModuleLink);

		if (BufferLength >= pFilter->FilterModuleName.Length)
		{
			if (NdisEqualMemory(Buffer, pFilter->FilterModuleName.Buffer, pFilter->FilterModuleName.Length))
			{
				FILTER_RELEASE_LOCK(&FilterListLock, bFalse);
				return pFilter;
			}
		}

		Link = Link->Flink;
	}

	FILTER_RELEASE_LOCK(&FilterListLock, bFalse);
	return NULL;
}


_IRQL_requires_max_(PASSIVE_LEVEL)
NDIS_STATUS NDISFilterDriverRegisterDevice(OUT PFILTER_DEVICE_EXTENSION *Extension, PDEVICE_OBJECT DeviceObject)
{
	NDIS_STATUS            Status = NDIS_STATUS_SUCCESS;
	PFILTER_DEVICE_EXTENSION        FilterDeviceExtension;
	PDRIVER_OBJECT                  DriverObject;

	{
		*Extension = FilterDeviceExtension = NdisGetDeviceReservedExtension(DeviceObject);

		FilterDeviceExtension->Signature = 'FTDR';
		FilterDeviceExtension->Handle = FilterDriverHandle;
		KeInitializeSpinLock(&FilterDeviceExtension->QLock);
		PRULES_LISTS FilterRules = ExAllocatePool(PagedPool, sizeof(RULES_LISTS));
		FilterRules->IsActive = FALSE;
		FilterRules->FirstRuleIPv4 = NULL;
		FilterRules->FirstRuleIPv6 = NULL;
		FilterDeviceExtension->FilterRules = FilterRules;

		//
		// Workaround NDIS bug
		//
		DriverObject = (PDRIVER_OBJECT)FilterDriverObject;
	}


	KdPrint(("<==NDISFilterDriverRegisterDevice: %x\n", Status));

	return (Status);

}



VOID AddIPv4Rule(IN PFILTER_DEVICE_EXTENSION FilterDeviceExtension, IN PRULE_IPV4 Rule)
{
	DbgPrint("AddIPv4Rule\n");
	KdPrint(("Id: %lu Begin:%d.%d.%d.%d End:%d.%d.%d.%d\n",
		Rule->Id, Rule->Begin[0], Rule->Begin[1], Rule->Begin[2], Rule->Begin[3], Rule->End[0], Rule->End[1], Rule->End[2], Rule->End[3]));
	PRULES_LISTS RulesLists = FilterDeviceExtension->FilterRules;
	PRULE_IPV4 List = RulesLists->FirstRuleIPv4;
	Rule->Next = List;
	RulesLists->FirstRuleIPv4 = Rule;

}

VOID DelIPv4Rule(IN PFILTER_DEVICE_EXTENSION FilterDeviceExtension, IN ULONG Id)
{
	DbgPrint("DelIPv4Rule\n");
	DbgPrint("Id: %lu\n", Id);
	PRULES_LISTS RulesLists = FilterDeviceExtension->FilterRules;
	PRULE_IPV4 CurrentRule = RulesLists->FirstRuleIPv4;

	while (CurrentRule != NULL && CurrentRule->Id == Id)
	{
		RulesLists->FirstRuleIPv4 = CurrentRule->Next;
		ExFreePool(CurrentRule);
		CurrentRule = RulesLists->FirstRuleIPv4;
	}

	if (CurrentRule == NULL)
	{
		return;
	}

	PRULE_IPV4 PreviousRule = CurrentRule;
	CurrentRule = CurrentRule->Next;

	while (CurrentRule != NULL)
	{
		if (CurrentRule->Id == Id)
		{
			PreviousRule->Next = CurrentRule->Next;
			ExFreePool(CurrentRule);
		}
		PreviousRule = CurrentRule;
		CurrentRule = CurrentRule->Next;
	}
}

VOID AddIPv6Rule(IN PFILTER_DEVICE_EXTENSION FilterDeviceExtension, IN PRULE_IPV6 Rule)
{
	PRULES_LISTS RulesLists = FilterDeviceExtension->FilterRules;
	PRULE_IPV6 List = RulesLists->FirstRuleIPv6;
	Rule->Next = List;
	RulesLists->FirstRuleIPv6 = Rule;
}

VOID DelIPv6Rule(IN PFILTER_DEVICE_EXTENSION FilterDeviceExtension, IN ULONG Id)
{
	PRULES_LISTS RulesLists = FilterDeviceExtension->FilterRules;
	PRULE_IPV6 CurrentRule = RulesLists->FirstRuleIPv6;

	while (CurrentRule != NULL && CurrentRule->Id == Id)
	{
		RulesLists->FirstRuleIPv6 = CurrentRule->Next;
		ExFreePool(CurrentRule);
		CurrentRule = RulesLists->FirstRuleIPv4;
	}

	if (CurrentRule == NULL)
	{
		return;
	}

	PRULE_IPV6 PreviousRule = CurrentRule;
	CurrentRule = CurrentRule->Next;

	while (CurrentRule != NULL)
	{
		if (CurrentRule->Id == Id)
		{
			PreviousRule->Next = CurrentRule->Next;
			ExFreePool(CurrentRule);
		}
		PreviousRule = CurrentRule;
		CurrentRule = CurrentRule->Next;
	}
}

VOID ActivateRules(IN PFILTER_DEVICE_EXTENSION FilterDeviceExtension)
{
	PRULES_LISTS RulesLists = FilterDeviceExtension->FilterRules;
	RulesLists->IsActive = TRUE;
}

VOID DeactivateRules(IN PFILTER_DEVICE_EXTENSION FilterDeviceExtension)
{
	PRULES_LISTS RulesLists = FilterDeviceExtension->FilterRules;
	RulesLists->IsActive = FALSE;
}
