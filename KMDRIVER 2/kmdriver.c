#include <nttdk.h>

/*
* https://www.cs.colorado.edu/~main/cs1300-old/include/ddk/ntapi.h
*/



typedef struct _KSERVICE_TABLE_DESCRIPTOR{
    ULONG_PTR *Base;
    ULONG * Count;
    ULONG Limit;
    UCHAR *Number;
} KSERVICE_TABLE_DESCRIPTOR, *PKSERVICE_TABLE_DESCRIPTOR;
KSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable;


NTSTATUS AnalyzeEPROCESS(PEPROCESS eProc){
	NTSTATUS st = STATUS_FAILURE;
	/*
	* now that we have EPROCESS walk VadRoot...
	* _HANDLE_TABLE...
	*/

	KdPrint(("eProc->SeAuditProcessCreationInfo->ImageFileName->Name: %s\n", eProc->SeAuditProcessCreationInfo->ImageFileName->Name));

	KdPrint(("File Name: %s\n", eProc->ImageFileName));
	KdPrint(("ImageBaseAddress 0%2x\n", eProc->Peb->ImageBaseAddress));
	KdPrint(("ImageSubsystem: %u\n", eProc->Peb->ImageSubsystem));
	KdPrint(("ImageSubsystemMajorVersion: %u\n", eProc->Peb->ImageSubsystemMajorVersion));
	KdPrint(("ImageSubsystemMinorVersion: %u\n", eProc->Peb->ImageSubsystemMinorVersion));
	KdPrint(("ImageProcessAffinityMask: %u\n", eProc->Peb->ImageProcessAffinityMask));
	KdPrint(("Command Line Arguments: %s\n", eProc->Peb->ProcessParameters->CommandLine));
	KdPrint(("DllPath: %s\n", eProc->Peb->ProcessParameters->DllPath));
	KdPrint(("ImagePathName : %s\n", eProc->Peb->ProcessParameters->ImagePathName));
	if(NT_SUCCESS(st) && eProc.VadRoot != 0){
		MMVAD_SHORT * VAD = NULL;
		MM_AVL_TABLE VadRoot = eProc->VadRoot;
		RtlEnumerateGenericTableWithoutSplayingAvl()
		// compare right child crap etc... so on..

	}
}


typedef struct _SYSTEM_MODULE_INFORMATION_ENTRY {
	ULONG	 Unknown1;
	ULONG	 Unknown2;
	PVOID  Base;
	ULONG  Size;
	ULONG  Flags;
	USHORT  Index;
	USHORT	NameLength;
	USHORT  LoadCount;
	USHORT  PathLength;
	CHAR  ImageName[256];
} SYSTEM_MODULE_INFORMATION_ENTRY, *PSYSTEM_MODULE_INFORMATION_ENTRY;

typedef struct _SYSTEM_MODULE_INFORMATION {
	ULONG  Count;
  SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

#define RootkitTAG 'abes'

typedef struct _KSERVICE_TABLE_DESCRIPTOR{
    ULONG_PTR *Base;
    ULONG * Count;
    #ifdef _WIN64 
    ULONGLONG Limit
    #else
    ULONG Limit;
    #endif
    UCHAR *Number;
} KSERVICE_TABLE_DESCRIPTOR, *PKSERVICE_TABLE_DESCRIPTOR;
KSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable;



PKSERVICE_TABLE_DESCRIPTOR SSDTfind(PVOID BaseAddress, ULONG kernelSize)
{
    static KSERVICE_TABLE_DESCRIPTOR* SSDT = 0;
    if(!SSDT)
    {
#ifndef _WIN64
        //x86 code
        UNICODE_STRING routineName;
        RtlInitUnicodeString(&routineName, L"KeServiceDescriptorTable");
        SSDT = (KSERVICE_TABLE_DESCRIPTOR*)MmGetSystemRoutineAddress(&routineName);
#else
        //x64 code
        if(BaseAddress == 0 || kernelSize == 0)
            return NULL;

        // Find KiSystemServiceStart
        const unsigned char KiSystemServiceStartPattern[] = { 0x8B, 0xF8, 0xC1, 0xEF, 0x07, 0x83, 0xE7, 0x20, 0x25, 0xFF, 0x0F, 0x00, 0x00 };
        const ULONG signatureSize = sizeof(KiSystemServiceStartPattern);
        bool found = false;
        ULONG KiSSSOffset;
        for(KiSSSOffset = 0; KiSSSOffset < kernelSize - signatureSize; KiSSSOffset++)
        {
            if(RtlCompareMemory(((unsigned char*)BaseAddress + KiSSSOffset), KiSystemServiceStartPattern, signatureSize) == signatureSize)
            {
                found = true;
                break;
            }
        }
        if(!found)
            return NULL;


        ULONG_PTR address = BaseAddress + KiSSSOffset + signatureSize;
        LONG relativeOffset = 0;
        if((*(unsigned char*)address == 0x4c) &&
                (*(unsigned char*)(address + 1) == 0x8d) &&
                (*(unsigned char*)(address + 2) == 0x15))
        {
            relativeOffset = *(LONG*)(address + 3);
        }
        if(relativeOffset == 0)
            return NULL;

        SSDT = (KSERVICE_TABLE_DESCRIPTOR*)(address + relativeOffset + 7);
#endif
    }
    return SSDT;
}




NTSTATUS RootkitDetect(){
    NTSTATUS rv;
 	ULONG ulNeededSize;

    if((rv = ZwQuerySystemInformation(SystemModuleInformation, &ulNeededSize, 0, &ulNeededSize)) == NT_STATUS_INFO_LENGTH_MISMATCH && ulNeededSize != 0){
    PSYSTEM_MODULE_INFORMATION pModules = ExAllocatePoolWithTag(PagedPool, ulNeededSize, RootkitTAG);
    if(pModules != NULL && (rv = ZwQuerySystemInformation(SystemModuleInformation, pModules, ulNeededSize, 0)) == NT_SUCCESSpModules != NULL && ){
	    if(pModules != NULL){
    	    PSYSTEM_MODULE_INFORMATION_ENTRY SystemModInfoEntry = &pModules->Module[0];
			PKSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable = SSDTfind(SystemModInfoEntry->Base, SystemModInfoEntry->Size)
    		ULONG uTableStart = SystemModInfoEntry->Base, uTableEnd = SystemModInfoEntry->Base + SystemModInfoEntry->Size;
 	 		DbgPrint("KeServiceDescriptorTable->Limit: %ul\n", (ULONG)KeServiceDescriptorTable->Limit);
    			for(ULONG i = 0; i < KeServiceDescriptorTable->Limit; i++){
        			DbgPrint("Syscall #%d is located at %08lX.\n", i, KeServiceDescriptorTable->Number[i]);
        			if((ULONG)KeServiceDescriptorTable->Number[i] < uTableStart || (ULONG)KeServiceDescriptorTable->Number[i] > uTableEnd){
        	    		DbgPrint("  Syscall #%d at %08lX is hooked!!\n", KeServiceDescriptorTable->Number[i]);
    	    		}
	    		}
        	ExFreePoolWithTag(pModules, RootkitTAG);
        	pModules = NULL;
    		}
    		else{
        		DbgPrint("Failed to get module list! Exiting...\n");
    		}
		}     
	}
    return STATUS_SUCCESS;
}








/*
* Detects EOP
*/
PRTL_AVL_TABLE Table = { 0 }; 

#define TableTag 'ErPl'
typedef struct _SystemTokenData{
	PACCESS_TOKEN Token;
	DWORD ProcessID;
} SystemTokenData, *PSystemTokenData;

typedef struct _NormalProcessTokenData{
	PACCESS_TOKEN Token;
	DWORD ProcessID;
} NormalProcessTokenData, *PNormalProcessTokenData;


NTSTATUS GetToken(PEPROCESS Process){

}
RTL_GENERIC_COMPARE_RESULTS TokenCompareRoutine(IN struct _RTL_AVL_TABLE *Table, IN PVOID FirstStruct, IN PVOID SecondStruct){
	NormalProcessTokenData ProcessToken = (PNormalProcessTokenData)FirstStruct;
	SystemTokenData SystemToken = (PSystemTokenData)SecondStruct;
	SIZE_T sSecondStruct = sizeof(SecondStruct);
	if(RtlCompareMemory(FirstStruct, SecondStruct, sSecondStruct) == sSecondStruct){
		
	}

}
PVOID TokenAllocateRoutine(IN struct _RTL_AVL_TABLE *Table, IN CLONG ByteSize){
	UNREFERNCED_PARAMETER(Table);
	if(ByteSize != NULL){
		return ExAllocatePoolWithTag(PagedPool, ByteSize, TableTag);
	}
	return NULL;
}
VOID TokenFreeRoutine(IN struct _RTL_AVL_TABLE *Table, IN PVOID Buffer){
	UNREFERNCED_PARAMETER(Table);
	if(Buffer != NULL){
		return ExFreePoolWithTag(Buffer, TableTag);
	}
	return;
}
NTSTATUS DetectEOP(PEPROCESS Process){
	Table = ExAllocatePoolWithTag(PagedPool, sizeof(RLT_AVL_TABLE) + 1, TableTag);
	if(Table != NULL){
		RtlInitializeGenericTableAvl(Table, TokenCompareRoutine, TokenAllocateRoutine, TokenFreeRoutine, NULL);

		ExFreePoolWithTag(Table, TableTag);
	}
}








#include <nttdk.h>

/*
* https://www.cs.colorado.edu/~main/cs1300-old/include/ddk/ntapi.h
*/
typedef struct _SYSTEM_PROCESSES {
    ULONG  NextEntryDelta;
    ULONG  ThreadCount;
    ULONG  Reserved1[6];
    LARGE_INTEGER  CreateTime;
    LARGE_INTEGER  UserTime;
    LARGE_INTEGER  KernelTime;
    UNICODE_STRING  ProcessName;
    KPRIORITY  BasePriority;
    ULONG  ProcessId;
    ULONG  InheritedFromProcessId;
    ULONG  HandleCount;
    ULONG  Reserved2[2];
    VM_COUNTERS  VmCounters;
    IO_COUNTERS  IoCounters;
    SYSTEM_THREADS  Threads[1];
} SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;



#define SystemInfoTag 'EfGl'
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pdo, IN PUNICODE_STRING pus){
    // Create Device using PDO later..
    UNREFERNCED_PARAMETER(pus);
    
    pus->DriverUnload = DriverUnload;
    NTSTATUS st = STATUS_FAILURE;
    SYSTEM_PROCESSES *sysp = { 0 };
    PULONG uLen = 0 ;
    st = ZwQuerySystemInformation(SystemProcessesAndThreadsInformation, NULL, uLen, &uLen);
    if(st == NT_STATUS_INFO_LENGTH_MISMATCH && uLen != NULL){
        
            BYTE *lpBuf = ExAllocatePoolWithTag(PagedPool, uLen, SystemInfoTag);
            if(lpBuf != NULL){
            // now that we have the size lets do the actual call
                st = ZwQuerySystemInformation(SystemProcessesAndThreadsInformation, lpBuf, uLen, &uLen);
                if(NT_SUCCESS(st)){
                    sysp = (SYSTEM_PROCESSES *)lpBuf;
                    do{

                        OBJECT_ATTRIBUTES ObjAttr = { 0 };
                        CLIENT_ID ClientId = { 0 };
                        HANDLE hProcess = NULL;
                        
                        ObjAttr.Length = sizeof(OBJECT_ATTRIBUTES);
                        ObjAttr.RootDirectory = NULL;
                        ObjAttr.ObjectName = NULL;
                        ObjAttr.Attributes = OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE;
                        ObjAttr.SecurityDescriptor = NULL;
                        ObjAttr.SecurityQualityOfService = NULL;

                        ClientId.UniqueThread = NULL;   
                        ClientId.UniqueProcess = (VOID*)sysp->ProcessID;
                        
                        st = ZwOpenProcess(&hProcess, GENERIC_READ, &ObjAttr, &ClientId);
                        if(hProcess != NULL){
                            if(NT_SUCCESS(st)){
                            	PEPROCESS process = NULL;
                            	if((st = PsLookupProcessByProcessId(sysp->ProcessID, &process))){
                                	KdPrint(("ProcessID opened!: %u\n", sysp->ProcessID));
                                	AnalyzePEB(hProcess);
                                	AnalyzeEPROCESS(hProcess);
                                	SSDTCheck(hProcess);
                            		ObfDereferenceObject(process);
                            	}
                            }
                            st = ZwClose(hProcess);

                        }
                        else{
                            KdPrint(("ZwOpenProcess Failed for ProcessID: %u\n ", sysp->ProcessID));
                        }
                        sysp = (SYSTEM_PROCESSES *)(((BYTE*)sysp) + sysp->NextEntryDelta);
                    } while(sysp);
                }
                ExFreePoolWithTag(lpBuf, SystemInfoTag);
            }
    }
}