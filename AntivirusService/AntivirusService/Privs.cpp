
#include "Privs.h"
bool
IsAdmin()
{
	bool rv = false;

	BOOL bIsAdmin = FALSE;

	SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
	PSID AdminGroup;
	if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &AdminGroup)) {
		if (CheckTokenMembership(NULL, AdminGroup, &bIsAdmin) && bIsAdmin) {
			rv = true;
		}
		FreeSid(AdminGroup);
	}

	return rv;
}
bool getperms() {
	TOKEN_PRIVILEGES	priv;
	HANDLE				hThis, hToken;
	LUID				luid;

	hThis = GetCurrentProcess();
	OpenProcessToken(hThis, TOKEN_ADJUST_PRIVILEGES, &hToken);
	LookupPrivilegeValue(0, "SeDebugPrivilege", &luid);
	priv.PrivilegeCount = 1;
	priv.Privileges[0].Luid = luid;
	priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, false, &priv, 0, 0, 0);
	CloseHandle(hToken);
	CloseHandle(hThis);
}

bool
IsElevated(HANDLE hProcess, bool *lpbIsElevated)
{
	bool rv = false;
	HANDLE hTokenHandle = NULL;
	if (OpenProcessToken(hProcess, TOKEN_QUERY, &hTokenHandle)) {
		TOKEN_ELEVATION teTokenElevation;
		DWORD dwBytesStored = 0;
		if (GetTokenInformation(hTokenHandle, TokenElevation, &teTokenElevation, sizeof(TOKEN_ELEVATION), &dwBytesStored)) {
			if (dwBytesStored >= sizeof(TOKEN_ELEVATION)) {
				rv = true;
				if (lpbIsElevated) *lpbIsElevated = teTokenElevation.TokenIsElevated ? true : false;
			}
		}

		CloseHandle(hTokenHandle);
	}

	return rv;
}