#include <ntddk.h>
#include "Driver Main.h"
NTSTATUS PreRegistryCallback(_In_ PVOID CallbackContext, _In_ REG_NOTIFY_CLASS Argument1, _Inout_ PVOID Argument2)
{
	UNREFERENCED_PARAMETER(CallbackContext);
	UNREFERENCED_PARAMETER(Argument2);

	switch (Argument1)
	{
	case RegNtPreDeleteKey:
		// this is REG_DELETE_KEY_INFORMATION
		break;
	case RegNtPreSetValueKey:
		break;
	case RegNtPreDeleteValueKey:
		break;
	case RegNtPreSetInformationKey:
		break;
	case RegNtPreRenameKey:
		break;
	case RegNtEnumerateKey:
		break;
	case RegNtPreEnumerateValueKey:
		break;
	case RegNtPreQueryKey:
		break;
	case RegNtPreQueryValueKey:
		break;
	case RegNtPreQueryMultipleValueKey:
		break;


	case RegNtPreCreateKeyEx:
		break;
	case RegNtPreOpenKeyEx:
		break;
	case RegNtPreFlushKey:
		break;
	case RegNtKeyHandleClose:
		break;
	case RegNtPreLoadKey:
		break;
	case RegNtPreUnLoadKey:
		break;
	case RegNtPreQueryKeySecurity:
		break;
	case RegNtPreSetKeySecurity:
		break;
	case RegNtPreRestoreKey:
		break;
	case RegNtPreSaveKey:
		break;
	case RegNtPreReplaceKey:
		break;
	case RegNtPreQueryKeyName:
		break;
	default:
		break;
	}

	return STATUS_SUCCESS;
}

// who cares about POST??!?!?!