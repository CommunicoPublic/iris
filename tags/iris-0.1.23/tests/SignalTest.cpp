/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "SignalHandler.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <stdio.h>
#include <sysexits.h>
#include <unistd.h>

using namespace IRIS;

class TestSignalHandler:
  public UserSignalHandler
{
public:
	/**
	  @brief A signal handler
	  @param iSignal - signal number
	*/
	void Handler(INT_32 iSignal);

	/**
	  @brief A Virtual Destructor
	*/
	~TestSignalHandler() throw();
};

//
// A signal handler
//
void TestSignalHandler::Handler(const INT_32 iSignal)
{
	switch (iSignal)
	{
		case SIGCHLD:
			{
				int iStatus;
				pid_t iKilledPid = 0;
				while ((iKilledPid = waitpid(-1, &iStatus, WNOHANG)) > 0)
				{
					if      (WIFEXITED(iStatus))   { fprintf(stdout, "%d catched SIGCHIHLD. Pid is %d, status %d\n",           INT_32(getpid()), INT_32(iKilledPid), WEXITSTATUS(iStatus)); }
					else if (WIFSIGNALED(iStatus)) { fprintf(stdout, "%d catched SIGCHIHLD. Pid is %d, killed by signal %d\n", INT_32(getpid()), INT_32(iKilledPid), WTERMSIG(iStatus));    }
				}
			}
			break;

		case SIGINT:
			fprintf(stdout, "%d catched SIGINT\n", INT_32(getpid()));
			break;

		case SIGTERM:
			fprintf(stdout, "%d catched SIGTERM\n", INT_32(getpid()));
			break;

		default:
			fprintf(stdout, "%d unknown signal catched: %d\n", INT_32(getpid()), iSignal);
	}
}

//
// A Destructor
//
TestSignalHandler::~TestSignalHandler() throw() { ;; }

int main(int argc, char ** argv)
{
	// Create Signal handler singletone
	SignalHandler & oSigHandler = SignalHandler::Instance();

	TestSignalHandler oTestSignalHandler;

	oSigHandler.RegisterHandler(SIGCHLD, &oTestSignalHandler);
	oSigHandler.RegisterHandler(SIGINT,  &oTestSignalHandler);
	oSigHandler.RegisterHandler(SIGTERM, &oTestSignalHandler);
	oSigHandler.RegisterHandler(SIGUSR1, &oTestSignalHandler);

	pid_t iPid1 = fork();
	if (iPid1 == 0) { sleep(3); fprintf(stderr, "%d exited\n", INT_32(getpid())); return EX_OK;       }

	pid_t iPid2 = fork();
	if (iPid2 == 0) { sleep(3); fprintf(stderr, "%d exited\n", INT_32(getpid())); return EX_CONFIG;   }

	pid_t iPid3 = fork();
	if (iPid3 == 0) { sleep(3); fprintf(stderr, "%d exited\n", INT_32(getpid())); return EX_SOFTWARE; }

	pid_t iPid4 = fork();
	if (iPid4 == 0) { sleep(1); fprintf(stderr, "%d exited\n", INT_32(getpid())); return EX_OK;       }

	// Kamikaze
	pid_t iPid5 = fork();
	if (iPid5 == 0) { sleep(1); raise(SIGUSR2); return EX_OK; }

	fprintf(stderr, "%d created processes: %d %d %d %d %d\n", INT_32(getpid()), INT_32(iPid1), INT_32(iPid2), INT_32(iPid3), INT_32(iPid4), INT_32(iPid5));

	// Send to first  child SIGINT
	fprintf(stderr, "%d => SIGINT  => %d\n", INT_32(getpid()), INT_32(iPid1));
	kill(iPid1, SIGINT);

	// Send to second child SIGTERM
	fprintf(stderr, "%d => SIGTERM => %d\n", INT_32(getpid()), INT_32(iPid2));
	kill(iPid2, SIGTERM);

	// Send to third  child SIGUSR1
	fprintf(stderr, "%d => SIGUSR1 => %d\n", INT_32(getpid()), INT_32(iPid3));
	kill(iPid3, SIGUSR1);

	// Wait fourth child
	fprintf(stderr, "%d waiting for %d\n\n", INT_32(getpid()), INT_32(iPid4));

	while (sleep(5) != 0) { ;; }

	fprintf(stderr, "All done: %d\n", INT_32(getpid()));

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
