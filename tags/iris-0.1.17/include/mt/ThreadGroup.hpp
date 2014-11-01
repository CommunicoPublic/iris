/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _THREAD_GROUP_HPP__
#define _THREAD_GROUP_HPP__ 1

/**
  @file ThreadGroup.hpp
  @brief Thread group
*/

#include "Mutex.hpp"
#include "STLVector.hpp"

#include <pthread.h>

namespace IRIS
{

/**
  @class ThreadGroup ThreadGroup.hpp <ThreadGroup.hpp>
  @brief Group of threads
*/
template <typename WRK>class ThreadGroup
{
public:
	/**
	  @brief Constructor
	*/
	ThreadGroup();

	/**
	  @brief Create new thread
	  @param pWorker - thread worker
	*/
	INT_32 CreateThread(WRK * pWorker);

	/**
	  @brief A destructor
	*/
	~ThreadGroup() throw();
private:

	struct ThreadContext
	{
		/** Thread data   */
		pthread_t    self;
		/** Thread worker */
		WRK        * worker;
	};

	typedef STLW::vector<ThreadContext *>  TThrWorkers;

	/** Mutex for operatins with vector */
	Mutex           oMutex;
	/** Thread attrubutes               */
	pthread_attr_t  oAttr;
	/** All thread workers              */
	TThrWorkers     vWorkers;

	static void * ThreadWorkerFunc(void  * vWorker);
};

//
// Constructor
//
template <typename WRK>
  ThreadGroup<WRK>::ThreadGroup()
{
	pthread_attr_init(&oAttr);
	pthread_attr_setdetachstate(&oAttr, PTHREAD_CREATE_JOINABLE);
}

//
// Create new thread
//
template <typename WRK>
  INT_32 ThreadGroup<WRK>::CreateThread(WRK  * pWorker)
{
	MutexLocker oMutexLocker(oMutex);
	ThreadContext  * pContext = new ThreadContext;
	pContext -> worker = pWorker;

	const INT_32 iRC = pthread_create(&(pContext -> self), &oAttr, ThreadWorkerFunc, (void *)pWorker);
	if (iRC == -1) { return iRC; }

	vWorkers.push_back(pContext);

return iRC;
}

//
// Thread worker function
//
template <typename WRK>
  void * ThreadGroup<WRK>::ThreadWorkerFunc(void  * vWorker)
{
	WRK * pWorker = static_cast<WRK *>(vWorker);
	try
	{
		pWorker -> Run();
	}
	catch(...)
	{
		return (void*)(-1);
	}
return NULL;
}

//
// Destructor
//
template <typename WRK>
  ThreadGroup<WRK>::~ThreadGroup() throw()
{
	MutexLocker oMutexLocker(oMutex);
	typename TThrWorkers::iterator itvWorkers = vWorkers.begin();
	while (itvWorkers != vWorkers.end())
	{
		void * vRetVal = NULL;
		ThreadContext  * pContext = *itvWorkers;

		pthread_join(pContext -> self, &vRetVal);

		delete pContext -> worker;
		delete pContext;
		++itvWorkers;
	}
}

} // namespace IRIS
#endif // _THREAD_GROUP_HPP__
// End.

