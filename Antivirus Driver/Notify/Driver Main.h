#pragma once

#include <ntddk.h>
#include <ntstrsafe.h>
#include <ndis.h>


#define _DEVICE_NAME (L"\\Device\\AntivirusDevice")
#define _DOS_DEVICE_NAME (L"\\DosDevices\\AntivirusDevice")
#define MINISPY_PORT_NAME L"\\MiniSpyPort"
#define IOCTL_CONFIGURE CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2048, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ADD_IPV4 CTL_CODE(FILE_DEVICE_UNKNOWN ,2050, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REMOVE_IPV4 CTL_CODE(FILE_DEVICE_UNKNOWN, 2051, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ADD_IPV6 CTL_CODE(FILE_DEVICE_UNKNOWN, 2052, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REMOVE_IPV6 CTL_CODE(FILE_DEVICE_UNKNOWN, 2053, METHOD_BUFFERED, FILE_ANY_ACCESS)





#define FILTER_FRIENDLY_NAME        L"Antivirus1"
#define FILTER_UNIQUE_NAME          L"{e6dd4321-f7nn-745k-lwqa-12b492189efb}"
#define FILTER_SERVICE_NAME         L"Antivirus"






typedef enum {
	DONOTHING,
	CREATEPROCESS,
	TERMINATEPROCESS,
	CREATETHREAD,
	TERMINATETHREAD,
	LOADIMAGE,
	NUM_EVENTTYPES
} Events;




typedef struct antivirus_request {
	DWORD dwRequestSize;      
	DWORD dwEventType;        
	DWORD dwProcessId;       
	DWORD dwParentProcessId; 
	DWORD dwThreadId;        
	void *lpImageBase;       
	WCHAR szFileName[1];     
	WCHAR *packet;


} ANTIVIRUS_REQUEST;

#define ANTIVIRUS_REQUEST_SIZE (sizeof(ANTIVIRUS_REQUEST) + ((UNICODE_STRING_MAX_CHARS + 14) * sizeof(WCHAR))) 

typedef struct antivirus_response {
	DWORD EventType;
	DWORD dwProcessId;
	DWORD dwThreadId;
	void *lpImageBase;
	BOOLEAN  bBlock;



} ANTIVIRUS_RESPONSE;

typedef struct antivirus_configuration {
	DWORD dwReqSize;
	DWORD dwMAXReqSize;
	BOOLEAN bDenyProcessCreationOnFailedScan;
	BOOLEAN bWantThreadEvents;
	BOOLEAN bWantImageLoadEvents;

	BOOLEAN ProtectProcess;
	BOOLEAN WantNetworkFilter;
} ANTIVIURS_CONFIGURATION;


typedef struct _QUEUED_WRITE {
	LIST_ENTRY           entry;              
	DWORD                dwEventType;        
	HANDLE               hPid;               
	HANDLE               hTid;               
	void                *lpImageBase;        
	BOOLEAN                 bResponseReceived; 
	ANTIVIURS_CONFIGURATION  response;           
	KEVENT              *pkeCompletionEvent; 

	WCHAR *ipv4;
	WCHAR *ipv6;
	WCHAR *Packet;

}QUEUED_WRITE;

typedef struct _QUEUED_READ {
	LIST_ENTRY            entry;         
	ANTIVIRUS_REQUEST    request;        
	const UNICODE_STRING *pImageFileName; 
} QUEUED_READ;



typedef struct _wdm_device_control {
	BOOLEAN                     bDeviceInUse;
	BOOLEAN                     bDeviceConfigured;
	ULONG                    ServiceProcessID;
	LIST_ENTRY               WriteList;
	LIST_ENTRY               ReadList;
	ULONG                    nInProgress;
	ANTIVIURS_CONFIGURATION config[1];
	struct {
		PIRP       pPendingReadIrp;
		KSPIN_LOCK Lock;
		KIRQL      IRQLevel;
	} data[1];
} WDM_DEVICE_CONTROL;















DRIVER_INITIALIZE DriverEntry;
NDIS_FILTER_DRIVER_CHARACTERISTICS      FChars;

VOID CancelIoRoutine(PDEVICE_OBJECT pdo, PIRP Irp);