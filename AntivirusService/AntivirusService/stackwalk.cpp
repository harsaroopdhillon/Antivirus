

#include <Windows.h>
#include <dbghelp.h>
// this will not work on some stuff !
BOOL WalkStack(CONTEXT &ctx, HANDLE hProcess, HANDLE hThread) {
	CRITICAL_SECTION Cts;
	EnterCriticalSection(&Cts);

	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);


	LeaveCriticalSection(&Cts);

}