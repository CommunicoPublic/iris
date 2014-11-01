/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include "IrisConfig.h"
#include "Debug.hpp"
#include "FileUtils.hpp"
#include "Logger.hpp"
#include "MainHelpers.hpp"
#include "SystemVars.hpp"

namespace IRIS
{

#ifndef C_PID_LENGTH
#define C_PID_LENGTH 1024
#endif // C_PID_LENGTH

//
// Daemonize process
//
INT_32 MainHelpers::Daemonize(CCHAR_P    szPidFile,
                              Logger   & oLogger)
{
	// Flush all streams before calling fork
	fflush(stdout);
	fflush(stderr);

	pid_t iPid = fork();
	switch (iPid)
	{
		// Error?
		case -1:
			oLogger.Emerg("Can't fork: %s, error code %d", strerror(errno), errno);
			return EX_SOFTWARE;

		case 0:
			setsid();
			iPid = fork();
			if      (iPid > 0) { exit(EX_OK); }
			else if (iPid == -1)
			{
				oLogger.Emerg("Can't fork: %s, error code %d", strerror(errno), errno);
				return EX_SOFTWARE;
			}
			break;
		default:
			exit(EX_OK);
	}

	// Only child here
	File::ChDir("/");
	// Write PID to file
	FILE * F = fopen(szPidFile, "w");
	if (F == NULL)
	{
		oLogger.Emerg("Can't open PID file `%s`: %s, error code %d", szPidFile, strerror(errno), errno);
		return EX_SOFTWARE;
	}

	CHAR_8 szPID[C_PID_LENGTH + 1];
	snprintf(szPID, C_PID_LENGTH, "%d\n", INT_32(getpid()));

	oLogger.Info("Pid file is: `%s`", szPidFile);

	const UINT_32 iWriteBytes = strlen(szPID);
	const size_t iWroteBytes = fwrite(szPID, sizeof(char), iWriteBytes, F);
	fclose(F);

	if (iWriteBytes != iWroteBytes)
	{
		oLogger.Emerg("Can't write PID to file `%s`", szPidFile);
		return EX_SOFTWARE;
	}

	// Reopen streams
	if (freopen("/dev/zero", "r", stdin) == NULL) { oLogger.Emerg("Can't reopen stdin to /dev/zero"); return EX_SOFTWARE; }
	if (freopen("/dev/null", "a", stdout) == NULL) { oLogger.Emerg("Can't reopen stdout to /dev/null"); return EX_SOFTWARE; }
	if (freopen("/dev/null", "a", stderr) == NULL) { oLogger.Emerg("Can't reopen stderr to /dev/null"); return EX_SOFTWARE; }

return EX_OK;
}

//
// Set priority and switch to unprivileged user
//
INT_32 MainHelpers::UnixSetup(const MainConfig  & oMainConfig,
                              Logger            & oLogger)
{
	// Set proper UID/GID
	if (getuid() == 0)
	{
		// Change priority, if need
		if (oMainConfig.priority != 0)
		{
			oLogger.Info("Changing base priority to %d", UINT_32(oMainConfig.priority));
			if (nice(oMainConfig.priority) == -1)
			{
				const INT_32 iErrNo = SystemVars::ErrNo();
				oLogger.Warn("Can't change base priority to %d: %s error code %d; continue anyway", UINT_32(oMainConfig.priority), Debug::PrintableError(iErrNo).c_str(), iErrNo);
			}
		}

		oLogger.Info("Switching to user/group %d:%d", UINT_32(oMainConfig.gid), UINT_32(oMainConfig.uid));
		if (oMainConfig.uid == 0)
		{
#ifdef IRIS_BIG_SECURITY_HOLE
			oLogger.Warn("Iris was compiled with _BIG_SECURITY_HOLE option. This option is potentially harmful and you should use it ONLY AT YOUR OWN RISK.");
#else
			oLogger.Emerg("Iris server won't work from superuser account (root) without recomiling with _BIG_SECURITY_HOLE option.");
			return EX_SOFTWARE;
#endif
		}
		else
		{
			if (setgid(oMainConfig.gid) == -1)
			{
				const INT_32 iErrNo = SystemVars::ErrNo();
				oLogger.Emerg("Can't set group id to %d: %s error code %d", INT_32(oMainConfig.gid), Debug::PrintableError(iErrNo).c_str(), iErrNo);
				return EX_SOFTWARE;
			}

			if (setuid(oMainConfig.uid)  == -1)
			{
				const INT_32 iErrNo = SystemVars::ErrNo();
				oLogger.Emerg("Can't set user id to %d: %s, error code %d", INT_32(oMainConfig.uid), Debug::PrintableError(iErrNo).c_str(), iErrNo);
				return EX_SOFTWARE;
			}
		}
	}
	oLogger.Info("Setting priority and switching to unprivileged user completed");

return EX_OK;
}

} // namespace IRIS
// End.
