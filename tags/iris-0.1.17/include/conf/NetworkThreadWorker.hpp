/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _NETWORK_THREAD_WORKER_HPP__
#define _NETWORK_THREAD_WORKER_HPP__ 1

/**
  @file NetworkThreadWorker.hpp
  @brief Iris network thread classes
*/

#include "ThreadPool.hpp"

namespace IRIS
{
// FWD
class Object;
// FWD
class FileHandleCache;
class GlobalContext;

/**
  @struct NetworkThreadData NetworkThreadWorker.hpp <NetworkThreadWorker.hpp>
  @brief Network thread data, need for NetworkThreadContext
*/
struct NetworkThreadData
{
	/** Mutex for data access */
	Mutex            & mutex;
	/** File handle cache     */
	FileHandleCache  & file_cache;
	/** Global server context */
	GlobalContext    & global_context;
	/** Constructor           */
	NetworkThreadData(Mutex            & oMutex,
	                  FileHandleCache  & oFileHandleCache,
	                  GlobalContext    & oGlobalContext);
};

class NetworkThreadContext
{
public:
	virtual ~NetworkThreadContext() throw();
};

class NetworkThreadWorker
{
public:

	virtual INT_32 StartThread(NetworkThreadData  ** pNetworkThreadData) = 0;

	virtual INT_32 Run(NetworkThreadData  ** pNetworkThreadData,
	                   NetworkThreadContext * pNetworkThreadContext) = 0;

	virtual INT_32 EndThread(NetworkThreadData  ** pNetworkThreadData) = 0;

	virtual ~NetworkThreadWorker() throw();
};

typedef ThreadPool<NetworkThreadWorker, NetworkThreadData *, NetworkThreadContext *> NetworkThreadPool;

} // namespace IRIS
#endif // _NETWORK_THREAD_WORKER_HPP__
// End.
