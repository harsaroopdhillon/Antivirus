#include "reg.h"



LPCWSTR GetTransactionNotifyClassString(_In_ ULONG TransactionNotifcation);

LPCWSTR GetNotifyClassString(_In_ REG_NOTIFY_CLASS NotifyClass);
extern PLARGE_INTEGER Cookie;

extern HANDLE TransactionManager ;
BOOLEAN CheckReg(PUNICODE_STRING Key) {
	PUNICODE_STRING ProtectKey = NULL;
	RtlInitUnicodeString(ProtectKey, MYREGKEY);
	if (RtlEqualUnicodeString(ProtectKey, Key, TRUE)) {
		return TRUE;
	}
	return FALSE;
}

NTSTATUS GetContextString(PVOID Obj, PCUNICODE_STRING lpszStringResult) {
	NTSTATUS st;
	if (Obj != NULL) { // cannot pass to this routine...
		if ((st = CmCallbackGetKeyObjectID(Cookie, Obj, 0, &lpszStringResult)) == STATUS_SUCCESS) {
			st = STATUS_SUCCESS;
			return st;
		}
	}
	// windows 8 check here....
	st = STATUS_UNSUCCESSFUL;
	return st;
}
NTSTATUS Callback(_In_ PVOID CallbackContext, _In_opt_ PVOID Argument1, _In_opt_ PVOID Argument2)

{

	NTSTATUS Status = STATUS_SUCCESS;
	REG_NOTIFY_CLASS NotifyClass;
	PCALLBACK_CONTEXT CallbackCtx;
	PUNICODE_STRING RegistryKey = NULL;

	CallbackCtx = (PCALLBACK_CONTEXT)CallbackContext;
	NotifyClass = (REG_NOTIFY_CLASS)(ULONG_PTR)Argument1;

	
	if (CallbackCtx->ProcessId == PsGetCurrentProcessId()) {
		return STATUS_UNSUCCESSFUL;
	}

	KdPrint(("\tCallback: Altitude-%S, NotifyClass-%S.", CallbackCtx->AltitudeBuffer, GetNotifyClassString(NotifyClass)));


	if (Argument2 == NULL) {

		

		KdPrint(("\tCallback: Argument 2 unexpectedly 0. Filter will abort and return success."));
		return STATUS_SUCCESS;
	}
	
	PREG_CREATE_KEY_INFORMATION_V1 PreCreateInfov1;

	PREG_CREATE_KEY_INFORMATION PreCreateInfo;
	PREG_SET_VALUE_KEY_INFORMATION PreSetValueInfo;
	PREG_DELETE_KEY_INFORMATION PreDeleteKeyInfo;
	PREG_DELETE_VALUE_KEY_INFORMATION PreDeleteValueInfo;
	PREG_SET_INFORMATION_KEY_INFORMATION PreSetInfoKeyInfo;
	PREG_RENAME_KEY_INFORMATION PreRenameKeyInfo;
	PREG_ENUMERATE_KEY_INFORMATION PreEnumKeyInfo;
	PREG_ENUMERATE_VALUE_KEY_INFORMATION PreEnumValueKeyInfo;
	PREG_QUERY_KEY_INFORMATION PreQueryKeyInfo;
	PREG_QUERY_VALUE_KEY_INFORMATION PreQueryValueKeyInfo;
	PREG_QUERY_MULTIPLE_VALUE_KEY_INFORMATION PreQueryMultiValueKeyInfo;
	PREG_PRE_OPEN_KEY_INFORMATION PreOpenKeyInfo;
	PREG_KEY_HANDLE_CLOSE_INFORMATION PreHandleCloseInfo;
	PREG_FLUSH_KEY_INFORMATION PreFlushKeyInfo;
	PREG_LOAD_KEY_INFORMATION PreLoadKeyInfo;
	PREG_UNLOAD_KEY_INFORMATION PreUnloadKeyInfo;
	PREG_QUERY_KEY_SECURITY_INFORMATION PreQueryKeySecInfo;
	PREG_SET_KEY_SECURITY_INFORMATION PreSetKeySecInfo;
	//REG_CALLBACK_CONTEXT_CLEANUP_INFORMATION ???
	PREG_RESTORE_KEY_INFORMATION PreRestoreKeyInfo;
	PREG_SAVE_KEY_INFORMATION PreSaveKeyInfo;
	PREG_REPLACE_KEY_INFORMATION PreReplaceKeyInfo;
	PREG_QUERY_KEY_NAME PreQueryKeyName;

	
	switch (NotifyClass) {
		/////// normal
	case RegNtPreCreateKey:
		PreCreateInfo = (PREG_CREATE_KEY_INFORMATION)Argument2;
		if (!CheckReg(PreCreateInfo->CompleteName)) {
			// add to list entry to be sent to usermode.
		}
		else {
			Status = STATUS_ACCESS_DENIED;
		}
		break;
	case RegNtPreCreateKeyEx:

		PreCreateInfov1 = (PREG_CREATE_KEY_INFORMATION_V1)Argument2;
		if (!CheckReg(PreCreateInfov1->CompleteName)) {
			// add to list entry to be sent to usermode.
		}
		else {
			Status = STATUS_ACCESS_DENIED;
		}
		break;
	case RegNtPreSetValueKey:
		PreSetValueInfo = (PREG_SET_VALUE_KEY_INFORMATION)Argument2;

		GetContextString(PreSetValueInfo->Object, RegistryKey);
		break;
	case RegNtPreDeleteKey:
		PreDeleteKeyInfo = (PREG_DELETE_KEY_INFORMATION)Argument2;
		// deleting registry key not specific value
		GetContextString(PreDeleteKeyInfo->Object, RegistryKey);
		
		break;
	case RegNtPreDeleteValueKey:
		PreDeleteValueInfo = (PREG_DELETE_VALUE_KEY_INFORMATION)Argument2;
		GetContextString(PreDeleteValueInfo->Object, RegistryKey);
		break;
	case RegNtPreSetInformationKey:
		PreSetInfoKeyInfo = (PREG_SET_INFORMATION_KEY_INFORMATION)Argument2;
		GetContextString(PreSetInfoKeyInfo->Object, RegistryKey);
		break;
	case RegNtPreRenameKey:
		PreRenameKeyInfo = (PREG_RENAME_KEY_INFORMATION)Argument2;
		GetContextString(PreRenameKeyInfo->Object, RegistryKey);
		break;
	case RegNtPreEnumerateKey:
		PreEnumKeyInfo = (PREG_ENUMERATE_KEY_INFORMATION)Argument2;
		break;
	case RegNtPreEnumerateValueKey:
		PreEnumValueKeyInfo = (PREG_ENUMERATE_VALUE_KEY_INFORMATION)Argument2;
		break;
	case RegNtPreQueryKey:
		PreQueryKeyInfo = (PREG_QUERY_KEY_INFORMATION)Argument2;
		break;
	case RegNtPreQueryValueKey:
		PreQueryValueKeyInfo = (PREG_QUERY_VALUE_KEY_INFORMATION)Argument2;
		break;

	case RegNtPreQueryMultipleValueKey:
		PreQueryMultiValueKeyInfo = (PREG_QUERY_MULTIPLE_VALUE_KEY_INFORMATION)Argument2;
		break;

	case RegNtPreOpenKeyEx:
	case RegNtPreOpenKey:
		PreOpenKeyInfo = (PREG_PRE_OPEN_KEY_INFORMATION)Argument2;
		if (!CheckReg(PreOpenKeyInfo->CompleteName)) {
			// add to list entry to be sent to usermode.
		}
		else {
			Status = STATUS_ACCESS_DENIED;
		}
		break;
	case RegNtPreKeyHandleClose:
		PreHandleCloseInfo = (PREG_KEY_HANDLE_CLOSE_INFORMATION)Argument2;
		break;
	case RegNtPreFlushKey:
		PreFlushKeyInfo = (PREG_FLUSH_KEY_INFORMATION)Argument2;
		break;
	case RegNtPreLoadKey:
		PreLoadKeyInfo = (PREG_LOAD_KEY_INFORMATION)Argument2;
		break;
	case RegNtPreUnLoadKey:
		PreUnloadKeyInfo = (PREG_UNLOAD_KEY_INFORMATION)Argument2;

		break;
	case RegNtPreQueryKeySecurity:
		PreQueryKeySecInfo = (PREG_QUERY_KEY_SECURITY_INFORMATION)Argument2;
		break;
	case RegNtPreSetKeySecurity:
		PreSetKeySecInfo = (PREG_SET_KEY_SECURITY_INFORMATION)Argument2;
		break;

		//REG_CALLBACK_CONTEXT_CLEANUP_INFORMATION ???
	case RegNtPreRestoreKey:
		PreRestoreKeyInfo = (PREG_RESTORE_KEY_INFORMATION)Argument2;
		break;
	case RegNtPreSaveKey:
		PreSaveKeyInfo = (PREG_SAVE_KEY_INFORMATION)Argument2;
		break;
	case RegNtPreReplaceKey:
		PreReplaceKeyInfo = (PREG_REPLACE_KEY_INFORMATION)Argument2;
		break;
	case RegNtPreQueryKeyName:
		PreQueryKeyName = (PREG_QUERY_KEY_NAME)Argument2;
		break;
		/////// normal


	default:
		break;
	}


	return Status;

}


LPCWSTR GetNotifyClassString(_In_ REG_NOTIFY_CLASS NotifyClass)
{
	switch (NotifyClass) {
	case RegNtPreDeleteKey:                 return L"RegNtPreDeleteKey";
	case RegNtPreSetValueKey:               return L"RegNtPreSetValueKey";
	case RegNtPreDeleteValueKey:            return L"RegNtPreDeleteValueKey";
	case RegNtPreSetInformationKey:         return L"RegNtPreSetInformationKey";
	case RegNtPreRenameKey:                 return L"RegNtPreRenameKey";
	case RegNtPreEnumerateKey:              return L"RegNtPreEnumerateKey";
	case RegNtPreEnumerateValueKey:         return L"RegNtPreEnumerateValueKey";
	case RegNtPreQueryKey:                  return L"RegNtPreQueryKey";
	case RegNtPreQueryValueKey:             return L"RegNtPreQueryValueKey";
	case RegNtPreQueryMultipleValueKey:     return L"RegNtPreQueryMultipleValueKey";
	case RegNtPreKeyHandleClose:            return L"RegNtPreKeyHandleClose";
	case RegNtPreCreateKeyEx:               return L"RegNtPreCreateKeyEx";
	case RegNtPreOpenKeyEx:                 return L"RegNtPreOpenKeyEx";
	case RegNtPreFlushKey:                  return L"RegNtPreFlushKey";
	case RegNtPreLoadKey:                   return L"RegNtPreLoadKey";
	case RegNtPreUnLoadKey:                 return L"RegNtPreUnLoadKey";
	case RegNtPreQueryKeySecurity:          return L"RegNtPreQueryKeySecurity";
	case RegNtPreSetKeySecurity:            return L"RegNtPreSetKeySecurity";
	case RegNtPreRestoreKey:                return L"RegNtPreRestoreKey";
	case RegNtPreSaveKey:                   return L"RegNtPreSaveKey";
	case RegNtPreReplaceKey:                return L"RegNtPreReplaceKey";

	case RegNtPostDeleteKey:                return L"RegNtPostDeleteKey";
	case RegNtPostSetValueKey:              return L"RegNtPostSetValueKey";
	case RegNtPostDeleteValueKey:           return L"RegNtPostDeleteValueKey";
	case RegNtPostSetInformationKey:        return L"RegNtPostSetInformationKey";
	case RegNtPostRenameKey:                return L"RegNtPostRenameKey";
	case RegNtPostEnumerateKey:             return L"RegNtPostEnumerateKey";
	case RegNtPostEnumerateValueKey:        return L"RegNtPostEnumerateValueKey";
	case RegNtPostQueryKey:                 return L"RegNtPostQueryKey";
	case RegNtPostQueryValueKey:            return L"RegNtPostQueryValueKey";
	case RegNtPostQueryMultipleValueKey:    return L"RegNtPostQueryMultipleValueKey";
	case RegNtPostKeyHandleClose:           return L"RegNtPostKeyHandleClose";
	case RegNtPostCreateKeyEx:              return L"RegNtPostCreateKeyEx";
	case RegNtPostOpenKeyEx:                return L"RegNtPostOpenKeyEx";
	case RegNtPostFlushKey:                 return L"RegNtPostFlushKey";
	case RegNtPostLoadKey:                  return L"RegNtPostLoadKey";
	case RegNtPostUnLoadKey:                return L"RegNtPostUnLoadKey";
	case RegNtPostQueryKeySecurity:         return L"RegNtPostQueryKeySecurity";
	case RegNtPostSetKeySecurity:           return L"RegNtPostSetKeySecurity";
	case RegNtPostRestoreKey:               return L"RegNtPostRestoreKey";
	case RegNtPostSaveKey:                  return L"RegNtPostSaveKey";
	case RegNtPostReplaceKey:               return L"RegNtPostReplaceKey";

	case RegNtCallbackObjectContextCleanup: return L"RegNtCallbackObjectContextCleanup";

	default:
		return L"Unsupported REG_NOTIFY_CLASS";
	}
}

