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
struct AcceptLoopContext
{
public:
	struct EventEntry
	{
		/** Network event watcher  */
		NetworkEventWatcher  * network_watcher;
		/** List of events         */
		UINT_32                events;
	};

	/** Accept thread context mutex         */
	Mutex                             mutex;
	/** Main context                        */
	MainLoopContext                 & main_context;
	/** Thread loop                         */
	EventLoop                         thread_loop;
	/** Accept event watcher                */
	AcceptSignalWatcher               accept_signal_watcher;
	/** Shutdown Event watcher              */
	AcceptLoopStopWatcher             stop_signal_watcher;
	/** Change socket state event watcher   */
	SocketChangeSignalWatcher         socket_change_signal_watcher;
	/** Set of active clients               */
	NetworkWatcherList                clients;
	/** Socket state change queue           */
	STLW::queue<EventEntry>           socket_state_queue;

	/**
	  @brief Delete watcher
	*/
	void DeleteWatcher(NetworkEventWatcher  * pWatcher);

	/**
	  @brief Constructor
	*/
	AcceptLoopContext(MainLoopContext  & oMainContext);
private:
	friend class AcceptLoopWorker;
	// Does not exist
	AcceptLoopContext(const AcceptLoopContext  & oRhs);
	AcceptLoopContext& operator=(const AcceptLoopContext  & oRhs);

	/**
	  @brief A destructor
	*/
	~AcceptLoopContext() throw();
};

} // namespace IRIS
#endif // _ACCEPT_LOOP_CONTEXT_HPP__
// End.
