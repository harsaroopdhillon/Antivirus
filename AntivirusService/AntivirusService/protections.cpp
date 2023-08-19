/*
Memory Page Protectrion Checking

*/
#include <Windows.h>
#include<winternl.h>
typedef struct _MMVAD_FLAGS
{
	ULONG CommitCharge : 19;
	ULONG NoChange : 1;
	ULONG VadType : 3;
	ULONG MemCommit : 1;
	ULONG Protection : 5;
	ULONG Spare : 2;
	ULONG PrivateMemory : 1;
} MMVAD_FLAGS, *PMMVAD_FLAGS;
// bit fields/^

typedef enum _MI_VAD_TYPE
{
	VadNone = 0,
	VadDevicePhysicalMemory = 1,
	VadImageMap = 2,
	VadAwe = 3,
	VadWriteWatch = 4,
	VadLargePages = 5,
	VadRotatePhysical = 6,
	VadLargePageSection = 7
} MI_VAD_TYPE;
BOOL InjectionVAD() {
	//walk the virtual address descriptor to get Modules etc...



}