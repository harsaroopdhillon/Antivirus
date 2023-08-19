/*
post tasksa etc to the kerenl mode driver ... this is only for the Process Creation etc...

*/

#pragma once
#include <Windows.h>
#include "service.h"
#include "header.h"
#include "thread.h"
#include "config.h"
#include "scan.h"

typedef struct pool_data POOL_DATA;
struct pool_data {
	HANDLE hSharedDriverHandle;           // The driver handled shared between workers where results are written
};

typedef struct worker_data WORKER_DATA;
struct worker_data {
	HANDLE hWriteCompletionEvent;       // Used for overlapped writes to kernel

	void *lpvScanDataArray;             // The data array used by plugins that contains match data
};
typedef struct worker_task_data WORKER_TASK_DATA;
struct worker_task_data {
	ANTIVIRUS_REQUEST AVReq;
};
void PfWorkerInit(void *lpPoolData, void *lpThreadData);
void PfWorkerDestroy(void *lpPoolData, void *lpThreadData);
void PfWorkerWork(void *lpPoolData, void *lpThreadData, void *lpTaskData, bool bCancel);

void PfWorkerStatsInit();
void PfWorkerStatusPrint();