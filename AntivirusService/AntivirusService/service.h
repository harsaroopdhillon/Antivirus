// Register the current process as a service.
// starts the kernel mode driver nd etc.
// TODO: add other IOCTL calls and etc crap

#pragma once

#define NOMINMAX
#include <Windows.h>
#include <winternl.h>

#include <stdarg.h>
#include <stdBOOL.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
SCAN_PROCESSCREATION,
SCAN_PROCESSTERMINATION,
SCAN_PERIODICALLY,
SCAN_IMAGELOAD,
SCAN_TYPES
} ScanTypes;

#pragma pack(push, 1)

	typedef struct scan_result SCAN_RESULT;
	struct scan_result {
		BOOL  bScanSuccessful;            // Was the scan successful?
		BOOL  bRuleMatched;               // Did any YARA rule match?
		BOOL  bBlock;                     // Should the current scan target be blocked?
		BOOL  bLog;                       // Should the current scan target be logged?
		BOOL  bQuarantine;                // Should the current scan target be quarantined?
		BYTE  bUnusedPadding[3];          // Align following struct members to a 4-byte boundary
		WCHAR szMatchedRuleNames[256];    // |-delimited list of rules that matched
		WCHAR szBlockRuleNames[256];      // |-delimited list of rules that specified to block the process
		WCHAR szQuarantineRuleNames[256]; // |-delimited list of rules that specified to quarantine the process
		WCHAR szError[512];               // An error string if bScanSuccessful is false
	};
#pragma pack(pop)


#ifdef __cplusplus
}
#endif