#pragma once
#include <Windows.h>


#define DriverName (L"Antivirus Driver")
#define DriverDisplayName (L"Antivirus Driver 1")

#define _DEVICE_PATH (L"\\\\.\\AntivirusDevice")
#define PROCFILTER_STATUS_NUM_PENDING_EVENT_TYPES 1024


#define _DEVICE_NAME (L"\\Device\\AntivirusDevice")
#define _DOS_DEVICE_NAME (L"\\DosDevices\\AntivirusDevice")

#define IOCTL_CONFIGURE CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2048, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ADD_IPV4 CTL_CODE(FILE_DEVICE_UNKNOWN ,2050, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REMOVE_IPV4 CTL_CODE(FILE_DEVICE_UNKNOWN, 2051, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ADD_IPV6 CTL_CODE(FILE_DEVICE_UNKNOWN, 2052, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REMOVE_IPV6 CTL_CODE(FILE_DEVICE_UNKNOWN, 2053, METHOD_BUFFERED, FILE_ANY_ACCESS)






typedef enum {
	DONOTHING,
	CREATEPROCESS,
	TERMINATEPROCESS,
	CREATETHREAD,
	TERMINATETHREAD,
	LOADIMAGE,
	NUM_EVENTTYPES
} Events;

typedef enum {
	IPV4 = 0x0800,
	ARP = 0x0806,
	IPV6 = 0x86DD
} NetworkTypes;


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





BOOL DriverSendResponse(HANDLE hDriver, HANDLE hWriteCompletionEvent, const ANTIVIRUS_RESPONSE *response);