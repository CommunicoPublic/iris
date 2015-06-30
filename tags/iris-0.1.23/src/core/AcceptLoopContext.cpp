/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "AcceptLoopContext.hpp"
#include "GlobalContext.hpp"
#include "MainLoopContext.hpp"
#include "NetworkEventWatcher.hpp"

namespace IRIS
{

//
// Constructor
//
AcceptLoopContext::AcceptLoopContext(MainLoopContext  & oIMainLoopContext): oMainLoopContext(oIMainLoopContext),
                                                                            oAcceptSignalWatcher(*this),
                                                                            oStopSignalWatcher(*this),
                                                                            oSocketChangeSignalWatcher(*this),
                                                                            oNetworkEventTimer(*this)
{
	;;
}

//
// Inactivate watcher
//
void AcceptLoopContext::InactivateWatcher(NetworkEventWatcher  * pWatcher)
{
	oClients.InactivateWatcher(pWatcher);
}

//
// Activate watcher
//
void AcceptLoopContext::ActivateWatcher(NetworkEventWatcher  * pWatcher,
                                        const W_FLOAT          dTimeout)
{
	oClients.ActivateWatcher(pWatcher, dTimeout);
}

//
// Add event
//
void AcceptLoopContext::AddEvent(const EventEntry  & oEventEntry)
{
	CRITICAL
	{
		MutexLocker oLocker(oMutex);

		oSocketStateQueue.push(oEventEntry);
		oSocketChangeSignalWatcher.SendSignal();
	}
}

//
// Get number of clients
//
UINT_32 AcceptLoopContext::GetClientsNum() const
{
	CRITICAL
	{
		MutexLocker oLocker(oMutex);

		return oClients.Size();
	}

// Should never happened
return 0;
}

//
// Get main loop context
//
MainLoopContext & AcceptLoopContext::GetMainLoopContext() { return oMainLoopContext; }

//
// Notify context about new connection
//
void AcceptLoopContext::NotifyNewConnection() { oAcceptSignalWatcher.SendSignal(); }

//
// Notify context about shutdown
//
void AcceptLoopContext::NotifyShutdown() { oStopSignalWatcher.SendSignal(); }

//
// Handle timeouts
//
void AcceptLoopContext::HandleTimeouts()
{
	STLW::vector<NetworkEventWatcher *> vWatchers;
	CRITICAL
	{
		MutexLocker oLocker(oMutex);

		// Check socket state BEFORE timeouts
		while(!oSocketStateQueue.empty())
		{
			AcceptLoopContext::EventEntry oEventEntry = oSocketStateQueue.front();

			if (oClients.WatcherExist(oEventEntry.watcher))
			{
				// Disconnect
				if (oEventEntry.events == IOWatcher::REMOVE)
				{
					--(oEventEntry.watcher -> oServiceConfig.active_clients);
					// Remove watcher from list
					oClients.RemoveWatcher(oEventEntry.watcher);
					// Delete watcher
					delete oEventEntry.watcher;

					if (oMainLoopContext.global_context.shutdown_type != GlobalContext::NONE && oClients.Size() == 0) { oEventLoop.UnLoop(); }
				}
				else
				{
					oEventEntry.watcher -> SetEvents(oEventEntry.events);
				}
			}

			oSocketStateQueue.pop();
		}

		// Then check timeouts
		oClients.CheckTimeout(vWatchers);
	}

	// Handle I/O timeouts
	STLW::vector<NetworkEventWatcher *>::iterator itvWatchers = vWatchers.begin();
	while (itvWatchers != vWatchers.end())
	{
		(*itvWatchers) -> IOTimeout();
		++itvWatchers;
	}
}

//
// Handle events
//
void AcceptLoopContext::HandleEvents()
{
	CRITICAL
	{
		MutexLocker oLocker(oMutex);

		while(!oSocketStateQueue.empty())
		{
			AcceptLoopContext::EventEntry  & oEventEntry = oSocketStateQueue.front();

			if (oClients.WatcherExist(oEventEntry.watcher))
			{
				// Disconnect
				if (oEventEntry.events == IOWatcher::REMOVE)
				{
					--(oEventEntry.watcher -> oServiceConfig.active_clients);
					// Remove watcher from list
					oClients.RemoveWatcher(oEventEntry.watcher);
					// Delete watcher
					delete oEventEntry.watcher;

					if (oMainLoopContext.global_context.shutdown_type != GlobalContext::NONE && oClients.Size() == 0) { oEventLoop.UnLoop(); }
				}
				else
				{
					oEventEntry.watcher -> SetEvents(oEventEntry.events);
				}
			}

			oSocketStateQueue.pop();
		}
	}
}

//
// Add watcher
//
void AcceptLoopContext::AddWatcher(NetworkEventWatcher  * pWatcher)
{
	CRITICAL
	{
		MutexLocker oLocker(oMutex);
		oClients.AddWatcher(pWatcher);
	}
}

//
// Shutdown server
//
void AcceptLoopContext::Shutdown()
{
	STLW::vector<NetworkEventWatcher *> vWatchers;
	CRITICAL
	{
		MutexLocker oLocker(oMutex);

		if (oMainLoopContext.global_context.shutdown_type == GlobalContext::IMMEDIATE)
		{
			oEventLoop.UnLoop();

			// Remove all active contextes
			oClients.RemoveAllWatchers(vWatchers);
		}
		else if (oMainLoopContext.global_context.shutdown_type == GlobalContext::TRANSACTIONAL)
		{
			if (oClients.Size() == 0)
			{
				oEventLoop.UnLoop();
			}
		}
	}

	// Handle I/O timeouts
	STLW::vector<NetworkEventWatcher *>::iterator itvWatchers = vWatchers.begin();
	while (itvWatchers != vWatchers.end())
	{
		(*itvWatchers) -> ShutdownContext();
		++itvWatchers;
	}
}

//
// Get event loop
//
EventLoop & AcceptLoopContext::GetEventLoop() { return oEventLoop; }

#ifdef IRIS_TLS_SUPPORT
//
// Initialize SSL structures
//
INT_32 AcceptLoopContext::SetupSSL() { return oAcceptSignalWatcher.SetupSSL(); }
#endif // IRIS_TLS_SUPPORT

//
// Destructor
//
AcceptLoopContext::~AcceptLoopContext() throw() { ;; }

} // namespace IRIS
// End.
