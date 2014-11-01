/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "ConditionVariable.hpp"
#include "Debug.hpp"
#include "Mutex.hpp"
#include "STLSstream.hpp"
#include "STLString.hpp"

#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

using namespace IRIS;

#define C_MAX_ITERATIONS 10

struct ThreadContext
{
	Mutex      mutex;
	Condition  cond;
	Condition  cond_res;
	int        data;
};


// Thread function
static void * Producer(void * pContext)
{
	ThreadContext * pThreadContext = (ThreadContext *)pContext;

	fprintf(stderr, "Starting Producer: %p\n", (void *)pthread_self());
	for (;;)
	{
		// Rpepare data
		CRITICAL
		{
			MutexLocker oLocker(pThreadContext -> mutex);
			fprintf(stderr, "Producer: %p -> %d\n", (void *)pthread_self(), pThreadContext -> data);
			++pThreadContext -> data;
		}
		// Notify thread
		pThreadContext -> cond.Signal();

		// Wait for results
		pThreadContext -> cond_res.Wait();

		if (pThreadContext -> data == C_MAX_ITERATIONS) { break; }
	}
	fprintf(stderr, "Ending Producer: %p\n", (void *)pthread_self());
return pContext;
}

// Thread function
static void * Consumer(void * pContext)
{
	ThreadContext * pThreadContext = (ThreadContext *)pContext;
	fprintf(stderr, "Starting Consumer: %p\n", (void *)pthread_self());

	for (;;)
	{
		pThreadContext -> cond.Wait();

		CRITICAL
		{
			MutexLocker oLocker(pThreadContext -> mutex);
			fprintf(stderr, "Consumer: %p -> %d\n", (void *)pthread_self(), pThreadContext -> data);
		}

		pThreadContext -> cond_res.Signal();

		if (pThreadContext -> data == C_MAX_ITERATIONS) { break; }
	}
	fprintf(stderr, "Ending Consumer: %p\n", (void *)pthread_self());
return pContext;
}

int main(void)
{
	ThreadContext oThreadContext;
	oThreadContext.data = 0;

	pthread_attr_t        oAttrs;
	pthread_attr_init(&oAttrs);
	pthread_attr_setdetachstate(&oAttrs, PTHREAD_CREATE_JOINABLE);

	pthread_t aThreads[2];

	pthread_create(&aThreads[0], &oAttrs, Consumer, &oThreadContext);
	pthread_create(&aThreads[1], &oAttrs, Producer, &oThreadContext);

	// Wait for threads
	for (int iPos = 0; iPos < 2; ++iPos)
	{
		int iRC = pthread_join(aThreads[iPos], NULL);
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
}

// End.
