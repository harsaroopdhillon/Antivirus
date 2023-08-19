
#include <stdio.h>
/*
	L"IsDebuggerPresent",
	L"CheckRemoteDebuggerPresent", 
	L"NtQueryInformationProcess", 
	L"NtSetInformationThread", 
	L"NtQueryObject", 
	L"UnhandledExceptionFilter", 
	L"OpenProcess",
	L"NtClose", 
	L"BlockInput",
	L"OutputDebugString", 
	L"GetTickCount", 
	L"GetSystemTime", 
	L"KiGetTickCount",
	L"QueryPerformaceCounter",
	L"timeGetTime", 
	L"TickCount",
	L"TickCountMultiplier",
	L"SetThreadContext",
	L"GetThreadContext",
	L"RtlQueryProcessDebugInformation"
	*/
int main(){
	printf("IsDebuggerPresent");
	printf("CheckRemoteDebuggerPresent");
	printf("NtSetInformationThread");
	printf("RtlQueryProcessDebugInformation");
	printf("QueryPerformaceCounter");
	return 0;
}