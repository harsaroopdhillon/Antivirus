#include <ntddk.h>
#include <ntstrsafe.h>
#include "Driver Main.h"



BOOLEAN Event(DWORD EventType, PCUNICODE_STRING ImageFileName, HANDLE Pid, HANDLE ParentPid, HANDLE ThreadId, void *lpImageBase, ANTIVIRUS_RESPONSE *o_response);
DWORD HandleToPid(HANDLE hPid);

const __declspec(selectany) UNICODE_STRING g_Null = RTL_CONSTANT_STRING(L"*NULL*");
VOID ProcessCreationCallBack(_Inout_ PEPROCESS Process, _In_ HANDLE ProcessId, _In_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo);
VOID ThreadCreationCallBack(IN HANDLE ProcessId, IN HANDLE ThreadId, IN BOOLEAN Create);

VOID ImageCreationCallBack(__in_opt PUNICODE_STRING PartialImageName, __in HANDLE  ProcessId, __in PIMAGE_INFO  ImageInfo);
void CompleteReadIrp(PIRP pReadIrp, const QUEUED_READ *prPendingRead);




#define MEMORY_TAG1 'PCFT' // "PCFT"