#include <Windows.h>
#include "service.h"

void Scan(DWORD dwEventType, int dScanContext, HANDLE hDriver, HANDLE hWriteCompletionEvent, DWORD dwProcessId, DWORD dwParentProcessId, WCHAR *lpszFileName, void *lpImageBase, void *lpvScanDataArray);
