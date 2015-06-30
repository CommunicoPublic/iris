/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _ACCEPT_LOOP_CONTEXT_HPP__
#define _ACCEPT_LOOP_CONTEXT_HPP__ 1

/**
  @file AcceptLoopContext.hpp
  @brief Iris network thread context
*/

#include "AcceptSignalWatcher.hpp"
#include "AcceptLoopStopWatcher.hpp"
#include "EventLoop.hpp"
#include "Mutex.hpp"
#include "NetworkEventTimer.hpp"
#include "NetworkWatcherList.hpp"
#include "SocketChangeSignalWatcher.hpp"

#include "STLQueue.hpp"
#include "STLSet.hpp"

namespace IRIS
{
// FWD
class MainLoopContext;
class NetworkEventWatcher;
/**
  @class AcceptLoopContext AcceptLoopContext.hpp <AcceptLoopContext.hpp>
  @brief Accept event context
*/
class AcceptLoopContext
{
public:
	struct EventEntry
	{
		/** Network event watcher  */
		NetworkEventWatcher  * watcher;
		/** List of events         */
		UINT_32                events;
	};

	/**
	  @brief Constructor
	*/
	AcceptLoopContext(MainLoopContext  & oMainContext);

	/**
	  @brief Add event
	*/
	void AddEvent(const EventEntry & oEventEntry);

	/**
	  @brief Get main loop context
	*/
	MainLoopContext & GetMainLoopContext();

	/**
	  @brief Activate watcher
	*/
	void ActivateWatcher(NetworkEventWatcher  * pWatcher,
	                     const W_FLOAT          dTimeout);

	/**
	  @brief Inactivate watcher
	*/
	void InactivateWatcher(NetworkEventWatcher  * pWatcher);

	/**
	  @brief Notify context about new connection
	*/
	void NotifyNewConnection();

	/**
	  @brief Notify context about shutdown
	*/
	void NotifyShutdown();

	/**
	  @beirf Handle timeouts
	*/
	void HandleTimeouts();

	/**
	  @brief Handle events
	*/
	void HandleEvents();

	/**
	  @brief Add watcher
	*/
	void AddWatcher(NetworkEventWatcher  * pWatcher);

	/**
	  @brief Delete watcher
	*/
	void DeleteWatcher(NetworkEventWatcher  * pWatcher);

	/**
	  @brief Get number of clients
	*/
	UINT_32 GetClientsNum() const;

	/**
	  @brief A destructor
	*/
	~AcceptLoopContext() throw();

private:
	friend class AcceptLoopWorker;
	friend class AcceptSignalWatcher;
	friend class AcceptLoopStopWatcher;
	friend class NetworkEventTimer;
	friend class NetworkEventWatcher;
	friend class SocketChangeSignalWatcher;

	// Does not exist
	AcceptLoopContext(const AcceptLoopContext  & oRhs);
	AcceptLoopContext& operator=(const AcceptLoopContext  & oRhs);

	/** Accept thread context mutex         */
	mutable Mutex                oMutex;
	/** Main context                        */
	MainLoopContext            & oMainLoopContext;
	/** Thread loop                         */
	EventLoop                    oEventLoop;
	/** Socket state change queue           */
	STLW::queue<EventEntry>      oSocketStateQueue;

	/** Set of active clients               */
	NetworkWatcherList           oClients;
	/** Accept event watcher                */
	AcceptSignalWatcher          oAcceptSignalWatcher;
	/** Shutdown Event watcher              */
	AcceptLoopStopWatcher        oStopSignalWatcher;
	/** Change socket state event watcher   */
	SocketChangeSignalWatcher    oSocketChangeSignalWatcher;
	/** Timeout handler                     */
	NetworkEventTimer            oNetworkEventTimer;

	/**
	  @brief Get event loop
	*/
	EventLoop & GetEventLoop();

	/**
	  @brief Shutdown server
	*/
	void Shutdown();

#ifdef IRIS_TLS_SUPPORT
	/**
	  @brief Initialize SSL structures
	*/
	INT_32 SetupSSL();
#endif
};

} // namespace IRIS
#endif // _ACCEPT_LOOP_CONTEXT_HPP__
// End.
