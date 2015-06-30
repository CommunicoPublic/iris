/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _THREAD_POOL_TCC__
#define _THREAD_POOL_TCC__ 1

namespace IRIS
{

static const INT_32 C_THREAD_NOT_READY = -1;
static const INT_32 C_THREAD_READY     = 0;

/**
  @brief Thread context
*/
template <typename ThreadWorker,
          typename ThreadData,
          typename TaskData> struct ThreadPool<ThreadWorker,
                                               ThreadData,
                                               TaskData>::ThreadContext
{
	/** Pointer to main context               */
	TGlobalContext          * global_context;
	/** Self thread                           */
	pthread_t                 self;

	/** Thread ready flag                     */
	bool                      init_ok;

	/** Self position in list of free threads */
	UINT_32                   self_free_pos;
	/** Self position in list of busy threads */
	UINT_32                   self_busy_pos;

	/** Max. requests per thread              */
	UINT_32                   max_requests_per_thread;
	/** Thread data                           */
	TaskData                  task_data;
};

/**
  @brief Global pool context
*/
template <typename ThreadWorker,
          typename ThreadData,
          typename TaskData> struct ThreadPool<ThreadWorker,
                                               ThreadData,
                                               TaskData>::GlobalContext
{
	/** Nax. number of threads      */
	UINT_32                        max_threads;
	/** Nax. number of free threads */
	UINT_32                        max_free_threads;
	/** Threads to start            */
	UINT_32                        start_threads;
	/** Number of thread to spawn   */
	UINT_32                        spawn_threads;
	/** Max. requests per thread    */
	UINT_32                        max_requests_per_thread;

	/** Thread attributes           */
	pthread_attr_t                 thread_attributes;

	/** Thread initialization mutex  */
	Mutex                          thread_mutex;

	/** Initialization semapthore    */
	Semaphore                      init_sem;

	/** Accept flag                  */
	INT_32                         accept_ok;
	/** Condition for accept         */
	ConditionVariable              accept_cond;

	/** Accept flag                  */
	INT_32                         accept_rev_ok;
	/** Condition for accept         */
	ConditionVariable              accept_rev_cond;

	/** Number of free threads       */
	UINT_32                        free_threads_count;
	/** Number of busy threads       */
	UINT_32                        busy_threads_count;

	/** List of free threads         */
	TThreadContext              ** free_threads;
	/** List of busy threads         */
	TThreadContext              ** busy_threads;

	/** Thread wiorker object        */
	ThreadWorker                 * thread_worker;
	/** Current task data            */
	TaskData                       task_data;
	/** Shutdown flag                */
	bool                           shutdown_thread;
};

//
// Constructor
//
template <typename ThreadWorker,
          typename ThreadData,
          typename TaskData> ThreadPool<ThreadWorker,
                                        ThreadData,
                                        TaskData>::ThreadPool(const UINT_32    iMaxThreads,
                                                              const UINT_32    iMaxFreeThreads,
                                                              const UINT_32    iStartThreads,
                                                              const UINT_32    iSpawnThreads,
                                                              const UINT_32    iMaxRequestsPerThread,
                                                              ThreadWorker   * pThreadWorker)
{
	// Create new context
	pGlobalContext = new TGlobalContext;

	pGlobalContext -> max_threads             = iMaxThreads;
	pGlobalContext -> max_free_threads        = iMaxFreeThreads;
	pGlobalContext -> start_threads           = iStartThreads < iMaxThreads ? iStartThreads : iMaxThreads;
	pGlobalContext -> spawn_threads           = iSpawnThreads;
	pGlobalContext -> max_requests_per_thread = iMaxRequestsPerThread;
	pGlobalContext -> free_threads_count      = 0;
	pGlobalContext -> busy_threads_count      = 0;

	// Thread attributes
	pthread_attr_init(&(pGlobalContext -> thread_attributes));
	pthread_attr_setdetachstate(&pGlobalContext -> thread_attributes, PTHREAD_CREATE_JOINABLE);

	// List of free threads
	pGlobalContext -> free_threads = new TThreadContext *[iMaxThreads];
	// List of busy threads
	pGlobalContext -> busy_threads = new TThreadContext *[iMaxThreads];

	for (UINT_32 iPos = 0; iPos < iMaxThreads; ++iPos)
	{
		pGlobalContext -> free_threads[iPos] = NULL;
		pGlobalContext -> busy_threads[iPos] = NULL;
	}

	// Thread worker
	pGlobalContext -> thread_worker      = pThreadWorker;

	// Shutdown flag
	pGlobalContext -> shutdown_thread    = false;

	// Thread is not ready
	pGlobalContext -> accept_ok          = C_THREAD_NOT_READY;

	// Create set of threads
	for (UINT_32 iI = 0; iI < pGlobalContext -> start_threads; ++iI)
	{
		// Lock mutex
		MutexLocker oMutexLocker(pGlobalContext -> thread_mutex);
		CreateThread(true);
	}
}

//
// Thread function
//
template <typename ThreadWorker,
          typename ThreadData,
          typename TaskData> void * ThreadPool<ThreadWorker,
                                               ThreadData,
                                               TaskData>::ThreadWorkerFunc(void * vContext)
{
	TThreadContext  * pThreadContext = (TThreadContext *)vContext;
	TGlobalContext  * pGlobalContext = pThreadContext -> global_context;

	// Internal thread data
	ThreadData oThreadData;
	// Start thread
	pGlobalContext -> thread_worker -> StartThread(&oThreadData);

	UINT_32 iThreadRequests = 0;
	for (;;)
	{
		// Lock accept mutex
		pGlobalContext -> accept_cond.Lock();
		if (pThreadContext -> init_ok != C_THREAD_READY)
		{
			pThreadContext -> init_ok = C_THREAD_READY;
			pGlobalContext -> init_sem.Post();
		}

		// Wait for accept condition
		while (pGlobalContext -> accept_ok != C_THREAD_READY) { pGlobalContext -> accept_cond.Wait(); }
		pGlobalContext -> accept_ok = C_THREAD_NOT_READY;
//fprintf(stderr, "accept_ok\n");
		pGlobalContext -> accept_rev_cond.Lock();
		pGlobalContext -> accept_cond.Unlock();

		CRITICAL
		{
			MutexLocker oMutexLocker(pGlobalContext -> thread_mutex);

			// Move thread from free to busy list
			pThreadContext -> task_data  = pGlobalContext -> task_data;
			pGlobalContext -> task_data  = TaskData();

			--pGlobalContext -> free_threads_count;
fprintf(stderr, "ThreadWorkerFunc: free: %d, busy: %d\n", pGlobalContext -> free_threads_count, pGlobalContext -> busy_threads_count);
			if (pGlobalContext -> free_threads_count != pThreadContext -> self_free_pos)
			{
				TThreadContext  * pTopThreadContext = pGlobalContext -> free_threads[pGlobalContext -> free_threads_count];
				pGlobalContext -> free_threads[pThreadContext -> self_free_pos] = pTopThreadContext;

				pTopThreadContext -> self_free_pos = pThreadContext -> self_free_pos;
			}

			pGlobalContext -> free_threads[pGlobalContext -> free_threads_count] = NULL;

			if (pGlobalContext -> shutdown_thread)
			{
fprintf(stderr, "ThreadWorkerFunc/Shutting down: got shutdown signal\n");
				pGlobalContext -> accept_rev_ok = C_THREAD_READY;
				pGlobalContext -> accept_rev_cond.Notify();
				pGlobalContext -> accept_rev_cond.Unlock();

				break;
			}
			++pGlobalContext -> busy_threads_count;
		}
//fprintf(stderr, "Sending notification to thread pool owner...\n");
		pGlobalContext -> accept_rev_ok = C_THREAD_READY;
		pGlobalContext -> accept_rev_cond.Notify();
		pGlobalContext -> accept_rev_cond.Unlock();

		try
		{
			// Do some useful work
			pGlobalContext -> thread_worker -> Run(&oThreadData, pThreadContext -> task_data);
		}
		catch(...) { ;; }

//fprintf(stderr, "Executing job... OK\n");
		// Clear data
		pThreadContext -> task_data = TaskData();

		// Move thread from busy to free list
		CRITICAL
		{
			MutexLocker oMutexLocker(pGlobalContext -> thread_mutex);
			--pGlobalContext -> busy_threads_count;

			// End of work; do not put this thread in list of free threads
			if (pGlobalContext -> max_requests_per_thread != 0)
			{
				++iThreadRequests;
				if (iThreadRequests > pGlobalContext -> max_requests_per_thread)
				{
fprintf(stderr, "ThreadWorkerFunc/Shutting down: max requests per thread reached\n");
					break;
				}
			}

			// Check max_free_threads
			if (pGlobalContext -> free_threads_count > pGlobalContext -> max_free_threads)
			{
fprintf(stderr, "ThreadWorkerFunc/Shutting down: free threads(%d) > max. free threads(%d)\n", pGlobalContext -> free_threads_count, pGlobalContext -> max_free_threads);
				break;
			}

			pGlobalContext -> free_threads[pGlobalContext -> free_threads_count] = pThreadContext;
			++pGlobalContext -> free_threads_count;
		}
//fprintf(stderr, "Accept condition\n");
	}

	// End thread
	pGlobalContext -> thread_worker -> EndThread(&oThreadData);
	// Clear data
	delete pThreadContext;

fprintf(stderr, "ThreadWorkerFunc: END\n");
return NULL;
}

//
// Create new thread
//
template <typename ThreadWorker,
          typename ThreadData,
          typename TaskData> ThreadPoolStatus ThreadPool<ThreadWorker,
                                                          ThreadData,
                                                          TaskData>::CreateThread(const bool bThrowException)
{
	if ((pGlobalContext -> free_threads_count + pGlobalContext -> busy_threads_count) > pGlobalContext -> max_threads) { return NO_FREE_THREADS; }

	// Create new thread context
	TThreadContext * pThreadContext           = new TThreadContext;
	// Set pointer to global context
	pThreadContext -> global_context          = pGlobalContext;
	// Set max. requests per thread
	pThreadContext -> max_requests_per_thread = pGlobalContext -> max_requests_per_thread;
	// Store position in list of free threads
	pThreadContext -> self_free_pos           = pGlobalContext -> free_threads_count;
	// Thread if not ready
	pThreadContext -> init_ok                 = C_THREAD_NOT_READY;

	// Add to tail
	pGlobalContext -> free_threads[pGlobalContext -> free_threads_count] = pThreadContext;

	++pGlobalContext -> free_threads_count;

	// Wait for initialization of thread
	pGlobalContext -> init_sem.Wait();

	// Create new thread
	const INT_32 iRC = pthread_create(&(pThreadContext -> self), &pGlobalContext -> thread_attributes, ThreadWorkerFunc, pThreadContext);
	if (iRC != 0)
	{
		--pGlobalContext -> free_threads_count;
		pGlobalContext -> free_threads[pGlobalContext -> free_threads_count] = NULL;
		delete pThreadContext;

		pGlobalContext -> init_sem.Post();

		// Possible unrecoverable?
		if (bThrowException) { throw UnixException("ThreadPool/CreateThread/pthread_create", iRC); }

		return THR_UNIX_ERROR;
	}

	// Wait for thread
	pGlobalContext -> init_sem.Wait();
	pGlobalContext -> init_sem.Post();

fprintf(stderr, "OK, thread created and started\n");

return THR_OK;
}

//
// Handle some work
//
template <typename ThreadWorker,
          typename ThreadData,
          typename TaskData> ThreadPoolStatus ThreadPool<ThreadWorker,
                                                          ThreadData,
                                                          TaskData>::HandleTask(TaskData  oTaskData)
{

	CRITICAL
	{
		// Lock mutex
		MutexLocker oMutexLocker(pGlobalContext -> thread_mutex);

		// Create new thread, if need
		if (pGlobalContext -> free_threads_count == 0)
		{
			for (UINT_32 iI = 0; iI < pGlobalContext -> spawn_threads; ++iI)
			{
				ThreadPoolStatus eStatus = CreateThread();
				if (eStatus != THR_OK) { return eStatus; }
			}
		}

		pGlobalContext -> task_data = oTaskData;
	}

	// Start job
	pGlobalContext -> accept_cond.Lock();
	pGlobalContext -> accept_rev_cond.Lock();

	pGlobalContext -> accept_ok = C_THREAD_READY;
	pGlobalContext -> accept_cond.Notify();
	pGlobalContext -> accept_cond.Unlock();
//fprintf(stderr, "HandleTask -> accept\n");

	while (pGlobalContext -> accept_rev_ok != C_THREAD_READY) { pGlobalContext -> accept_rev_cond.Wait(); }
	pGlobalContext -> accept_rev_ok = C_THREAD_NOT_READY;
	pGlobalContext -> accept_rev_cond.Unlock();
//fprintf(stderr, "HandleTask <- accept\n");

return THR_OK;
}

//
// Shutdown thread pool
//
template <typename ThreadWorker,
          typename ThreadData,
          typename TaskData> ThreadPoolStatus ThreadPool<ThreadWorker,
                                                          ThreadData,
                                                          TaskData>::ShutdownPool(TaskData  oTaskData)
{
	CRITICAL
	{
		// Lock mutex
		MutexLocker oMutexLocker(pGlobalContext -> thread_mutex);
		pGlobalContext -> shutdown_thread = 1;
	}
fprintf(stderr, "Shutting down %d thread(s): ", pGlobalContext -> free_threads_count + pGlobalContext -> busy_threads_count);
	for(;;)
	{
		// Start job
		pGlobalContext -> accept_cond.Lock();
		pGlobalContext -> accept_rev_cond.Lock();

		pGlobalContext -> accept_ok = C_THREAD_READY;
		pGlobalContext -> accept_cond.Notify();
		pGlobalContext -> accept_cond.Unlock();
//fprintf(stderr, "HandleTask -> accept for shutdown\n");

		while (pGlobalContext -> accept_rev_ok != C_THREAD_READY) { pGlobalContext -> accept_rev_cond.Wait(); }
		pGlobalContext -> accept_rev_ok = C_THREAD_NOT_READY;
		pGlobalContext -> accept_rev_cond.Unlock();
//fprintf(stderr, "HandleTask <- accept for shutdown\n");

		// Wait for busy threads
		sleep(1);
		CRITICAL
		{
			// Lock mutex
			MutexLocker oMutexLocker(pGlobalContext -> thread_mutex);
			if (pGlobalContext -> free_threads_count == 0 && pGlobalContext -> busy_threads_count == 0) { break; }
fprintf(stderr, "%d ", pGlobalContext -> free_threads_count + pGlobalContext -> busy_threads_count);
		}
	}

return THR_OK;
}

//
// Destructor
//
template <typename ThreadWorker,
          typename ThreadData,
          typename TaskData> ThreadPool<ThreadWorker,
                                        ThreadData,
                                        TaskData>::~ThreadPool() throw()
{
	fprintf(stderr, "~ThreadPool\n");

	// List of free threads
	delete pGlobalContext -> free_threads;
	// List of busy threads
	delete pGlobalContext -> busy_threads;
}

} // namespace IRIS
#endif // _THREAD_POOL_TCC__
// End.
