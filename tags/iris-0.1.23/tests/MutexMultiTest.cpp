/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes

#include "MutexMultiLocker.hpp"
#include "Debug.hpp"

#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

using namespace IRIS;

struct ThreadContext
{
	MutexMultiLocker<UINT_32>   mutex_locker;
};

// Thread function
static void * ThreadFunction(void * pContext)
{
	ThreadContext * pThreadContext = (ThreadContext *)pContext;

	pThreadContext -> mutex_locker.Lock(1);
	for (INT_32 iPos = 0; iPos < 5; ++ iPos)
	{
		fprintf(stderr, "Thread %llu handling job\n", (long long unsigned)(pthread_self()));
		sleep(1);
	}
	pThreadContext -> mutex_locker.Unlock(1);
return pContext;
}

#define MAX_THREADS 3

int main(int argc, char ** argv)
{
	ThreadContext oContext;

	pthread_attr_t        oAttrs;
	pthread_attr_init(&oAttrs);
	pthread_attr_setdetachstate(&oAttrs, PTHREAD_CREATE_JOINABLE);

	pthread_t aThreads[MAX_THREADS];

	// Create set of threads
	printf("Creating %d threads\n", MAX_THREADS);
	INT_32 iPos;
	for (iPos = 0; iPos < MAX_THREADS; ++iPos)
	{
		INT_32 iRC = pthread_create(&aThreads[iPos], &oAttrs, ThreadFunction, &oContext);
		if (iRC != 0)
		{
			fprintf(stderr, "FATAL: %s", Debug::PrintableError(iRC).c_str());
			return EX_SOFTWARE;
		}
	}

	// Wait for results
	printf("Wait for results\n");
	for (iPos = 0; iPos < MAX_THREADS; ++iPos)
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
