#include <ntddk.h>


#define MYREGKEY L"\\REGISTRY\\MACHINE\\Software\\Antivirus1"

#define MAX_ALTITUDE_BUFFER_LENGTH 10
#ifndef _REGFLTR
#define REGFLTR



#define REGFLTR_CONTEXT_POOL_TAG          '0tfR'
#define REGFLTR_CAPTURE_POOL_TAG          '1tfR'


HANDLE g_RootKey;



ULONG g_MajorVersion;
ULONG g_MinorVersion;



BOOLEAN g_RMCreated;


BOOLEAN g_IsWin8OrGreater;


#define MAX_CALLBACK_CTX_ENTRIES            10
FAST_MUTEX g_CallbackCtxListLock;

LIST_ENTRY g_CallbackCtxListHead;

//
// Count of entries in list
//
USHORT g_NumCallbackCtxListEntries;

//
// Context data structure for the transaction callback RMCallback
//

typedef struct _RMCALLBACK_CONTEXT {

	//
	// A bit mask of all transaction notifications types that the RM Callback is 
	// notified of.
	//
	ULONG Notification;

	//
	// The handle to an enlistment
	//
	HANDLE Enlistment;

} RMCALLBACK_CONTEXT, *PRMCALLBACK_CONTEXT;


//
// The context data structure for the registry callback. It will be passed 
// to the callback function every time it is called. 
//

typedef struct _CALLBACK_CONTEXT {

	//
	// List of callback contexts currently active
	//
	LIST_ENTRY CallbackCtxList;


	//
	// Records the current ProcessId to filter out registry operation from
	// other processes.
	//
	HANDLE ProcessId;

	//
	// Records the altitude that the callback was registered at
	//
	UNICODE_STRING Altitude;
	WCHAR AltitudeBuffer[MAX_ALTITUDE_BUFFER_LENGTH];

	//
	// Records the cookie returned by the registry when the callback was 
	// registered
	//
	LARGE_INTEGER Cookie;

	//
	// A pointer to the context for the transaction callback. 
	// Used to enlist on a transaction. Only used in the transaction samples.
	//
	PRMCALLBACK_CONTEXT RMCallbackCtx;

	//
	// These fields record information for verifying the behavior of the
	// certain samples. They are not used in all samples
	//

	//
	// Number of times the RegNtCallbackObjectContextCleanup 
	// notification was received
	//
	LONG ContextCleanupCount;

	//
	// Number of times the callback saw a notification with the call or
	// object context set correctly.
	//
	LONG NotificationWithContextCount;

	//
	// Number of times callback saw a notirication without call or without
	// object context set correctly
	//
	LONG NotificationWithNoContextCount;

	//
	// Number of pre-notifications received
	//
	LONG PreNotificationCount;

	//
	// Number of post-notifications received
	//
	LONG PostNotificationCount;

} CALLBACK_CONTEXT, *PCALLBACK_CONTEXT;


//
// The registry and transaction callback routines
//

EX_CALLBACK_FUNCTION Callback;

#endif