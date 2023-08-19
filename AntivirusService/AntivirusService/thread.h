

#pragma once

#include <Windows.h>

typedef struct threadpool THREADPOOL;

THREADPOOL* ThreadPoolAlloc(
	const DWORD dwNumChannels,
	void(*initfn)(void *lpPoolData, void *lpThreadData),
	void(*workfn)(void *lpPoolData, void *lpThreadData, void *lpTaskData, bool bCancel),
	void(*destroyfn)(void *lpPoolData, void *lpThreadData),
	void *lpPoolData,
	DWORD dwThreadDataSize);

void ThreadPoolFree(THREADPOOL *tp);


#define CHANNEL_NONE 0
bool ThreadPoolPost(THREADPOOL *tp, DWORD dwChannel, bool bWait, HANDLE hStopEvent, void *lpTaskData);