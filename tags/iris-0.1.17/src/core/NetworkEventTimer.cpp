/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

#include "AcceptLoopContext.hpp"
#include "MainLoopContext.hpp"
#include "NetworkEventTimer.hpp"
#include "NetworkEventWatcher.hpp"

#include <stdio.h>

namespace IRIS
{

//
// Constructor
//
NetworkEventTimer::NetworkEventTimer(AcceptLoopContext  * pIContext): EventTimer(&(pIContext -> thread_loop),
                                                                      pIContext -> main_context.timeout_check_interval,
                                                                      pIContext -> main_context.timeout_check_interval),
                                                                      pContext(pIContext)
{
	if (pIContext -> main_context.timeout_check_interval != -1) { StartWatch(); }
}

//
// Watcher callback
//
void NetworkEventTimer::Callback(const UINT_32  iREvents)
{
	STLW::vector<NetworkEventWatcher *> vWatchers;
	CRITICAL
	{
		MutexLocker oLocker(pContext -> mutex);

		// Check socket state BEFORE timeouts
		STLW::queue<AcceptLoopContext::EventEntry> & oQueue = pContext -> socket_state_queue;
		while(!oQueue.empty())
		{
			AcceptLoopContext::EventEntry oEventEntry = oQueue.front();
			oQueue.pop();

			oEventEntry.network_watcher -> SetEvents(oEventEntry.events);
		}

		// Then check timeouts
		pContext -> clients.CheckTimeout(vWatchers);

		// Handle I/O timeouts
		STLW::vector<NetworkEventWatcher *>::iterator itvWatchers = vWatchers.begin();
		while (itvWatchers != vWatchers.end())
		{
			(*itvWatchers) -> IOTimeout();
			++itvWatchers;
		}
	}
}

//
// A destructor
//
NetworkEventTimer::~NetworkEventTimer() throw()
{
	;;
}

} // namespace IRIS
// End.
