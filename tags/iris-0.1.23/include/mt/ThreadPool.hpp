/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _THREAD_POOL_HPP__
#define _THREAD_POOL_HPP__ 1

#include "ConditionVariable.hpp"
#include "Exception.hpp"
#include "Mutex.hpp"
#include "Semaphore.hpp"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

namespace IRIS
{

enum ThreadPoolStatus { THR_OK = 0, THR_UNIX_ERROR = -1, NO_FREE_THREADS = -2 };

/**
  @class template <typename ThreadWorker, typename TaskData>class ThreadPool  ThreadPool.hpp <ThreadPool.hpp>
  @brief POSIX thread pool for C++
*/
template <typename ThreadWorker,
          typename ThreadData,
          typename TaskData> class ThreadPool
{
public:
	/**
	  @brief Constructor
	  @param iMaxThreads     - max. number of threads in pool
	  @param iMaxFreeThreads - max. number of free threads in pool
	  @param iStartThreads   - initial number of threads in pool
	  @param iSpawnThreads   - number of threads to spawn
	  @param iMaxRequestsPerThread - max. number of requests per one thread
	  @param pThreadWorker   - thread worker
	*/
	ThreadPool(const UINT_32    iMaxThreads,
	           const UINT_32    iMaxFreeThreads,
	           const UINT_32    iStartThreads,
	           const UINT_32    iSpawnThreads,
	           const UINT_32    iMaxRequestsPerThread,
	           ThreadWorker   * pThreadWorker);

	/**
	  @brief Handle some work
	  @param oTaskData - task data
	*/
	ThreadPoolStatus HandleTask(TaskData oTaskData);

	/**
	  @brief Shutdown pool
	  @param oTaskData - task data
	*/
	ThreadPoolStatus ShutdownPool(TaskData  oTaskData);

	/**
	  @brief A destructor
	*/
	~ThreadPool() throw();
private:
	// Does not exist
	ThreadPool();
	ThreadPool(const ThreadPool & oRhs);
	ThreadPool & operator=(const ThreadPool & oRhs);

	// FWD
	struct GlobalContext;
	struct ThreadContext;

	/**
	  @typedef typename ThreadPool<ThreadWorker, ThreadData, TaskData>::GlobalContext TGlobalContext
	  @brief Global thread pool context
	*/
	typedef typename ThreadPool<ThreadWorker, ThreadData, TaskData>::GlobalContext TGlobalContext;

	/**
	  @typedef typename ThreadPool<ThreadWorker, ThreadData, TaskData>::ThreadContext TThreadContext
	  @brief Thread context
	*/
	typedef typename ThreadPool<ThreadWorker, ThreadData, TaskData>::ThreadContext TThreadContext;

	/** Thread pool context, PIMPL  */
	GlobalContext  * pGlobalContext;

	/**
	  @brief Create new thread
	  @return Status of operation
	*/
	ThreadPoolStatus CreateThread(const bool bThrowException = false);

	/**
	  @brief Thread function
	*/
	static void * ThreadWorkerFunc(void * vContext);
};

} // namespace IRIS

#include "ThreadPool.tcc"

#endif // _THREAD_POOL_HPP__
// End.
