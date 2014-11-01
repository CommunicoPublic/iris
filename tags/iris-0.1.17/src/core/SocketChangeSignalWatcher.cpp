/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "SocketChangeSignalWatcher.hpp"
#include "AcceptLoopContext.hpp"
#include "Debug.hpp"
#include "Exception.hpp"
#include "MainLoopContext.hpp"
#include "NetworkEventWatcher.hpp"
#include "Mutex.hpp"

#include <stdio.h>

namespace IRIS
{

//
// Constructor
//
SocketChangeSignalWatcher::SocketChangeSignalWatcher(AcceptLoopContext  * pIContext): IOAsyncWatcher(&(pIContext -> thread_loop)),
                                                                                      pContext(pIContext)
{
	StartWatch();
}

//
// Watcher callback
//
void SocketChangeSignalWatcher::Callback(const UINT_32  iREvents)
{
	CRITICAL
	{
		MutexLocker oLocker(pContext -> mutex);

		STLW::queue<AcceptLoopContext::EventEntry> & oQueue = pContext -> socket_state_queue;
		while(!oQueue.empty())
		{
			AcceptLoopContext::EventEntry  & oEventEntry = oQueue.front();
			if (oEventEntry.events == IOWatcher::DISCONNECT)
			{
				oEventEntry.network_watcher -> ShutdownContext();
			}
			else
			{
				if (pContext -> clients.WatcherExist(oEventEntry.network_watcher))
				{
					oEventEntry.network_watcher -> SetEvents(oEventEntry.events);
				}
			}
			oQueue.pop();
		}
	}
}

//
// A destructor
//
SocketChangeSignalWatcher::~SocketChangeSignalWatcher() throw() { ;; }

} // namespace IRIS
// End.
