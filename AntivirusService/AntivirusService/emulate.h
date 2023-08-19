#pragma once
#include <unicorn/unicorn.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <set>
#include <map>
#include <string> 


#include <Windows.h>
#include <Tlhelp32.h>







#pragma pack(push, 1)
struct SegmentDescriptor {
	union {
		struct {
			USHORT limit0;
			USHORT base0;
			UCHAR base1;
			UCHAR type : 4;
			UCHAR system : 1;      /* S flag */
			UCHAR dpl : 2;
			UCHAR present : 1;     /* P flag */
			UCHAR limit1 : 4;
			UCHAR avail : 1;
			UCHAR is_64_code : 1;  /* L flag */
			UCHAR db : 1;          /* DB flag */
			UCHAR granularity : 1; /* G flag */
			UCHAR base2;
		};
		UINT64 desc;
	};
};
#pragma pack(pop)





typedef std::map<UINT64, std::string> TImportNameResolver;


typedef struct {
	UINT64 EntryPoint;
	UINT64 ImageBase;
	UINT64 ImageSize;
	UCHAR *VirtualImage;
} *PSPEImage, SPEImage;


BOOL InitatePEFile(uc_engine *uc, LPCWSTR FilePath, PSPEImage PEImage);

VOID UC_MapMemory(uc_engine *uc, HMODULE hmod, BOOL Write);