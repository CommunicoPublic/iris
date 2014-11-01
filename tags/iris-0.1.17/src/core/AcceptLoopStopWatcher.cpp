/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "AcceptLoopStopWatcher.hpp"
#include "AcceptLoopContext.hpp"
#include "Debug.hpp"
#include "Exception.hpp"
#include "GlobalContext.hpp"
#include "MainLoopContext.hpp"
#include "NetworkEventWatcher.hpp"

namespace IRIS
{

//
// Constructor
//
AcceptLoopStopWatcher::AcceptLoopStopWatcher(AcceptLoopContext  * pIContext): IOAsyncWatcher(&(pIContext -> thread_loop)),
                                                                              pContext(pIContext)
{
	StartWatch();
}

//
// Watcher callback
//
void AcceptLoopStopWatcher::Callback(const UINT_32  iREvents)
{
	try
	{
		// Check shutdown type
		GlobalContext & oGlobalContext = pContext -> main_context.global_context;
		if (oGlobalContext.shutdown_type == GlobalContext::IMMEDIATE)
		{
			pContext -> thread_loop.UnLoop();
			CRITICAL
			{
				MutexLocker oLocker(pContext -> mutex);

				// Destroy all active contextes
				pContext -> clients.DeleteAllWatchers();
			}
		}
		else if (oGlobalContext.shutdown_type == GlobalContext::TRANSACTIONAL)
		{
			CRITICAL
			{
				MutexLocker oLocker(pContext -> mutex);
				if (pContext -> clients.Size() == 0)
				{
					pContext -> thread_loop.UnLoop();
				}
			}
		}
	}
	catch(UnixException  & e)
	{
		Debug::CheckUnrecoverable("AcceptLoopStopWatcher::Callback/UnixException", e.err_no());
	}
	catch(...)
	{
		Debug::Abort("AcceptLoopStopWatcher::Callback/Unknown exception");
	}
}

//
// A destructor
//
AcceptLoopStopWatcher::~AcceptLoopStopWatcher() throw() { ;; }

} // namespace IRIS
// End.
