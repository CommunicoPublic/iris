/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MAIN_LOOP_EVENT_WATCHER_HPP__
#define _MAIN_LOOP_EVENT_WATCHER_HPP__ 1

/**
  @file MainLoopEventWatcher.hpp
  @brief Main loop event watcher

  Accepts network connections and put them to the accept queue.
*/
#include "IOWatcher.hpp"
#include "STLVector.hpp"

namespace IRIS
{
// FWD
struct AcceptLoopContext;
struct MainLoopContext;
class ServerTCPSocket;
class ServiceConfig;

/**
  @class MainLoopEventWatcher MainLoopEventWatcher.hpp <MainLoopEventWatcher.hpp>
  @brief Main loop event watcher

  Accepts network connections and put them to the accept queue.
*/
class MainLoopEventWatcher:
  public IOWatcher
{
public:
	/**
	  @brief Constructor
	  @param oMainContext - Main context
	  @param vContextes - List of contextes for event threads
	  @param pServerSocket - server socket
	  @param pServiceConfig - service configuration
	*/
	MainLoopEventWatcher(MainLoopContext                    & oMainContext,
	                     STLW::vector<AcceptLoopContext *>  & vContextes,
	                     ServerTCPSocket                    * pServerSocket,
	                     ServiceConfig                      * pServiceConfig);

	/**
	  @brief Watcher callback
	  @param iSocket - socket to watch
	  @param iREvents - list of events
	*/
	void Callback(const INT_32   iSocket,
	              const UINT_32  iREvents);

	/**
	  @brief A destructor
	*/
	~MainLoopEventWatcher() throw();
private:
	// Does not exist
	MainLoopEventWatcher(const MainLoopEventWatcher  & oRhs);
	MainLoopEventWatcher& operator=(const MainLoopEventWatcher  & oRhs);

	/** Main Context      */
	MainLoopContext                    & oMainContext;
	/** List of contextes */
	STLW::vector<AcceptLoopContext *>  & vContextes;
	/** Server socket     */
	ServerTCPSocket                     * pServerSocket;
	/** Service config    */
	ServiceConfig                       * pServiceConfig;
};

} // namespace IRIS
#endif // _MAIN_LOOP_EVENT_WATCHER_HPP__
// End.
