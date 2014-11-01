/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes

#include "RWLock.hpp"
#include "Debug.hpp"

#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

using namespace IRIS;

struct ThreadContext
{
	RWLock      lock;
};

// Thread function
static void * ThreadFunction1(void * pContext)
{
	ThreadContext * pThreadContext = (ThreadContext *)pContext;

	RWLockRLocker oLocker(pThreadContext -> lock);
	for (INT_32 iPos = 0; iPos < 5; ++ iPos)
	{
		fprintf(stderr, "Thread %p handling job\n", (void *)(pthread_self()));
		sleep(1);
	}
return pContext;
}

// Thread function
static void * ThreadFunction2(void * pContext)
{
	ThreadContext * pThreadContext = (ThreadContext *)pContext;

	RWLockWLocker oLocker(pThreadContext -> lock);
	for (INT_32 iPos = 0; iPos < 5; ++ iPos)
	{
		fprintf(stderr, "Thread %p handling job\n", (void *)(pthread_self()));
		sleep(1);
	}
return pContext;
}

#define MAX_THREADS 2

int main(int argc, char ** argv)
{
	ThreadContext oContext;

	pthread_attr_t        oAttrs;
	pthread_attr_init(&oAttrs);
	pthread_attr_setdetachstate(&oAttrs, PTHREAD_CREATE_JOINABLE);

	pthread_t aThreads[MAX_THREADS];

	// Create set of threads
	printf("Creating %d threads\n", MAX_THREADS);
	INT_32 iRC = pthread_create(&aThreads[0], &oAttrs, ThreadFunction1, &oContext);
	if (iRC != 0)
	{
		fprintf(stderr, "FATAL: %s", Debug::PrintableError(iRC).c_str());
		return EX_SOFTWARE;
	}

	iRC = pthread_create(&aThreads[1], &oAttrs, ThreadFunction2, &oContext);
	if (iRC != 0)
	{
		fprintf(stderr, "FATAL: %s", Debug::PrintableError(iRC).c_str());
		return EX_SOFTWARE;
	}

	// Wait for results
	printf("Wait for results\n");
	for (INT_32 iPos = 0; iPos < MAX_THREADS; ++iPos)
	{
		INT_32 iRC = pthread_join(aThreads[iPos], NULL);
		if (iRC != 0)
		{
			fprintf(stderr, "FATAL: %s", Debug::PrintableError(iRC).c_str());
			return EX_SOFTWARE;
		}
	}

	fprintf(stderr, "Cleanup and exit\n");
	pthread_attr_destroy(&oAttrs);

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
