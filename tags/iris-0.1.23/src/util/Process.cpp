/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "Process.hpp"

#include <sys/types.h>
#include <sys/wait.h>

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

namespace IRIS
{
//
// Constructor
//
ProcPipe::ProcPipe()
{
	InitFD();
}

//
// Initialize fds
//
void ProcPipe::InitFD()
{
	iPid = 0;
	for (INT_32 iPos = 0; iPos < 2; ++iPos) { aIn[iPos] = aOut[iPos] = aErr[iPos] = -1; }
}

//
// Close fds
//
void ProcPipe::CloseFD()
{
	iPid = 0;
	for (INT_32 iPos = 0; iPos < 2; ++iPos)
	{
		if (aIn[iPos]  != -1) { close(aIn[iPos]);  }
		if (aOut[iPos] != -1) { close(aOut[iPos]); }
		if (aErr[iPos] != -1) { close(aErr[iPos]); }

		aIn[iPos] = aOut[iPos] = aErr[iPos] = -1;
	}
}

//
// Open pipe
//
INT_32 ProcPipe::Open(CCHAR_P  szProgram)
{
	if (aIn[0] != -1) { return -1; }

	InitFD();

	if (pipe(aIn)  < 0) { CloseFD(); return -1; }
	if (pipe(aOut) < 0) { CloseFD(); return -1; }
	if (pipe(aErr) < 0) { CloseFD(); return -1; }

	iPid = vfork();
	if (iPid == -1) { CloseFD(); return -1; }

	// Child
	if (iPid == 0)
	{
		if (aIn[0] != fileno(stdin))
		{
			dup2(aIn[0], fileno(stdin));
			close(aIn[0]);
		}
		close(aIn[1]);

		if (aOut[1] != fileno(stdout))
		{
			dup2(aOut[1], fileno(stdout));
			close(aOut[1]);
		}
		close(aOut[0]);

		if (aErr[1] != fileno(stderr))
		{
			dup2(aErr[1], fileno(stderr));
			close(aErr[1]);
		}
		close(aErr[0]);

		if (execl("/bin/sh", "sh", "-c", szProgram, (void *)0) == -1)
		{
			fprintf(stderr, "ERROR: %s (%d)\n", strerror(errno), errno);
		}
		exit(EX_SOFTWARE);
	}

	close(aIn[0]);
	close(aOut[1]);
	close(aErr[1]);
	aIn[0] = aOut[1] = aErr[1] = -1;

return 0;
}

//
// Close pipe
//
INT_32 ProcPipe::Close()
{
	if (aIn[0] == -1) { return -1; }

	sigset_t oOldMask, oNewMask;
	sigemptyset(&oNewMask);
	sigaddset(&oNewMask, SIGINT);
	sigaddset(&oNewMask, SIGQUIT);
	sigaddset(&oNewMask, SIGHUP);
	sigprocmask(SIG_BLOCK, &oNewMask, &oOldMask);

	pid_t iChildPid = 0;
	int   iStatus   = 0;
	do
	{
		iChildPid = waitpid(iPid, &iStatus, 0);
	} while (iChildPid == -1 && errno == EINTR);

	sigprocmask(SIG_SETMASK, &oOldMask, NULL);

	close(aIn[1]);
	close(aOut[0]);
	close(aErr[0]);
	aIn[1] = aOut[0] = aErr[0] = -1;

	if (iChildPid == -1) { return -1; }

	if (WIFEXITED(iStatus)) { return 0; }

return WEXITSTATUS(iStatus);
}

//
// Write data to stdout stream
//
INT_64 ProcPipe::Write(const void    * vData,
                       const INT_64    iSize) { return write(aIn[1], vData, iSize); }

//
// Read data from stdin stream
//
INT_64 ProcPipe::Read(void          * vData,
                      const INT_64    iSize) { return read(aOut[0], vData, iSize); }

//
// Read data from stderr stream
//
INT_64 ProcPipe::ReadErr(void          * vData,
                         const INT_64    iSize) { return read(aErr[0], vData, iSize); }

//
// Destructor
//
ProcPipe::~ProcPipe() throw()
{
	Close();
}

} // namespace IRIS
// End.
