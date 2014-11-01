/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "SystemVars.hpp"

// C Includes
#include <sys/param.h>
#include <sys/time.h>
#include <string.h>
// Linux stupidity
#ifdef linux
  #include <time.h>
#endif
#ifdef sun
  #include <netdb.h>
#endif
#include <grp.h>
#include <pwd.h>

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

namespace IRIS
{

//
// Get system time
//
SystemVars::Time SystemVars::GetTime()
{
	struct timespec oSysTime;
#ifdef linux
	clock_gettime(CLOCK_REALTIME, &oSysTime);
#elif defined(sun) || defined(_AIX) || defined(__OpenBSD__) || defined(__NetBSD__)
	clock_gettime(CLOCK_REALTIME, &oSysTime);
#else
	clock_gettime(CLOCK_REALTIME_PRECISE, &oSysTime);
#endif
	SystemVars::Time oTime = { UINT_64(oSysTime.tv_sec), UINT_64(oSysTime.tv_nsec) };

return oTime;
}

//
// Get host name
//
STLW::string SystemVars::GetHostname()
{
	char szHostName[MAXHOSTNAMELEN + 1];

	gethostname(szHostName, sizeof(szHostName));

return szHostName;
}

//
// Sleep
//
UINT_64 SystemVars::Sleep(const UINT_64 iMicroseconds)
{
	;;
return usleep(iMicroseconds);
}

//
// Get unix error code
//
UINT_32 SystemVars::ErrNo() { return errno; }

//
// Get system user data
//
SystemVars::SystemUser SystemVars::GetSystemUserData(CCHAR_P  szUserName)
{
	SystemUser oSystemUser = { -1, -1, ""};
	if (szUserName == NULL) { return oSystemUser; }

	struct passwd    oUserRec;
	struct passwd  * pUser = NULL;
	INT_32          iBufferLen = 1024;
	CHAR_P          sBuffer = (CHAR_P)malloc(iBufferLen);
	for (;;)
	{
		INT_32 iRC = getpwnam_r(szUserName, &oUserRec, sBuffer, iBufferLen, &pUser);
		if (iRC == 0 && pUser != NULL) { break; }
		if (iRC != ERANGE)
		{
			free(sBuffer);
			return oSystemUser;
		}

		iBufferLen <<= 1;
		CHAR_P sTMP = (CHAR_P)realloc(sBuffer, iBufferLen);
		if (sTMP != NULL) { sBuffer = sTMP; }
		else
		{
			free(sBuffer);
			return oSystemUser;
		}
	}
	oSystemUser.uid     = pUser -> pw_uid;
	oSystemUser.gid     = pUser -> pw_gid;
	oSystemUser.homedir = pUser -> pw_dir;
	free(sBuffer);

return oSystemUser;
}

//
// Set user Id
//
INT_64 SystemVars::SetUID(INT_64  iUserId)
{
	;;
return setuid(iUserId);
}

//
// Get user Id
//
INT_64 SystemVars::GetUID(CCHAR_P  szUserName)
{
	if (szUserName == NULL) { return getuid(); }

	struct passwd    oUserRec;
	struct passwd  * pUser = NULL;
	INT_32          iBufferLen = 1024;
	CHAR_P          sBuffer = (CHAR_P)malloc(iBufferLen);
	for (;;)
	{
		INT_32 iRC = getpwnam_r(szUserName, &oUserRec, sBuffer, iBufferLen, &pUser);
		if (iRC == 0 && pUser != NULL) { break; }
		if (iRC != ERANGE)
		{
			free(sBuffer);
			return -1;
		}

		iBufferLen <<= 1;
		CHAR_P sTMP = (CHAR_P)realloc(sBuffer, iBufferLen);
		if (sTMP != NULL) { sBuffer = sTMP; }
		else
		{
			free(sBuffer);
			return -1;
		}
	}
	INT_64 iTMP = pUser -> pw_uid;
	free(sBuffer);
return iTMP;
}

//
// Set group Id
//
INT_64 SystemVars::SetGID(INT_64  iGroupId)
{
	;;
return setgid(iGroupId);
}

//
// Get group Id
//
INT_64 SystemVars::GetGID(CCHAR_P  szGroupName)
{
	if (szGroupName == NULL || *szGroupName == '\0') { return getgid(); }

	struct group    oGroup;
	struct group  * pGrp = NULL;
	INT_32          iBufferLen = 1024;
	CHAR_P          sBuffer = (CHAR_P)malloc(iBufferLen);
	for (;;)
	{
		INT_32 iRC = getgrnam_r(szGroupName, &oGroup, sBuffer, iBufferLen, &pGrp);
		if (iRC == 0 && pGrp != NULL) { break; }
		if (iRC != ERANGE)
		{
			free(sBuffer);
			return -1;
		}

		iBufferLen <<= 1;
		CHAR_P sTMP = (CHAR_P)realloc(sBuffer, iBufferLen);
		if (sTMP != NULL) { sBuffer = sTMP; }
		else
		{
			free(sBuffer);
			return -1;
		}
	}
	INT_64 iTMP = pGrp -> gr_gid;
	free(sBuffer);
return iTMP;
}

//
// Get group name
//
STLW::string SystemVars::GetGroupNam(const UINT_64  iGroupId)
{
        struct group    oGroup;
        struct group  * pGrp;
        INT_32          iBufferLen = 1024;
        CHAR_P          sBuffer = (CHAR_P)malloc(iBufferLen);
        while (getgrgid_r(iGroupId, &oGroup, sBuffer, iBufferLen, &pGrp) == ERANGE)
        {
                iBufferLen <<= 1;
                CHAR_P sTMP = (CHAR_P)realloc(sBuffer, iBufferLen);
                if (sTMP != NULL) { sBuffer = sTMP; }
                else
                {
                        free(sBuffer);
                        return "";
                }
        }
        STLW::string sTMP = pGrp -> gr_name;
        free(sBuffer);
return sTMP;
}

} // namespace IRIS
// End.
