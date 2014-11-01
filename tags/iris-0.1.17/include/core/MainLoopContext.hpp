/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MAIN_LOOP_CONTEXT_HPP__
#define _MAIN_LOOP_CONTEXT_HPP__ 1

/**
  @file MainLoopContext.hpp
  @brief Main network thread context
*/
#include "EventLoop.hpp"
#include "MainLoopStopWatcher.hpp"
#include "Mutex.hpp"
#include "STLMap.hpp"
#include "STLQueue.hpp"

namespace IRIS
{
// FWD
struct GlobalContext;
struct ServiceConfig;
class ClientTCPSocket;
class ServerTCPSocket;
class ServiceConfig;

/**
  @struct MainLoopContext MainLoopContext.hpp <MainLoopContext.hpp>
  @brief Main network thread context
*/
struct MainLoopContext
{
	/**
	  @struct EventEntry MainLoopContext.hpp <MainLoopContext.hpp>
	  @brief Event data: service configuration and client socket
	*/
	struct EventEntry
	{
		/** Service configuration   */
		ServiceConfig     * service_config;
		/** Connected client socket */
		ClientTCPSocket   * socket;
	};

	/** Thread context mutex     */
	Mutex                      mutex;
	/** Global context           */
	GlobalContext            & global_context;
	/** Main loop                */
	EventLoop                  main_loop;
	/** Client sockets queue     */
	STLW::queue<EventEntry>    client_sockets;
	/** Watch for event and then stop main loop  */
	MainLoopStopWatcher        stop_event_watcher;
	/** Socket timeout check interval            */
	W_FLOAT                    timeout_check_interval;

	/**
	  @brief Constructor
	  @param oIGlobalContext - Global server context
	*/
	MainLoopContext(GlobalContext  & oIGlobalContext);

private:
	// Does not exist
	MainLoopContext(const MainLoopContext  & oRhs);
	MainLoopContext& operator=(const MainLoopContext  & oRhs);
};

} // namespace IRIS
#endif // _MAIN_LOOP_CONTEXT_HPP__
// End.
