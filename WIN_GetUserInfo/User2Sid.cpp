#include "User2Sid.h"
bool user2sid(LPWSTR csUsername, PSID Sid)
{
	UCHAR buffer2[2048];
	UCHAR buffer3[4];
	DWORD length = 900;
	LPCTSTR lpAccountName = csUsername;
	LPDWORD cbSid = &length;
	LPTSTR ReferencedDomainName = (LPTSTR)buffer2;
	LPDWORD cbReferencedDomainName = &length;
	PSID_NAME_USE peUse = (PSID_NAME_USE)buffer3;
	if (LookupAccountName(NULL, lpAccountName, Sid, cbSid,
		ReferencedDomainName, cbReferencedDomainName,
		peUse))
	{
		return 1;
	}
	return 0;
}