/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _NETWORK_EVENT_WATCHER_HPP__
#define _NETWORK_EVENT_WATCHER_HPP__ 1

/**
  @file NetworkEventWatcher.hpp
  @brief Iris network watcher which work with connected client
*/

#include "IOWatcher.hpp"
#include "STLString.hpp"

namespace IRIS
{
// FWD
class AcceptLoopContext;
class ServiceConfig;
class ClientTCPSocket;

/**
  @class NetworkEventWatcher NetworkEventWatcher.hpp <NetworkEventWatcher.hpp>
  @brief Client events watcher
*/
class NetworkEventWatcher:
  public IOWatcher
{
public:
	/**
	  @brief Constructor
	  @param pIEventLoop - event loop object
	*/
	NetworkEventWatcher(EventLoop  * pIEventLoop);

	/**
	  @brief Set events for watcher
	  @param iEvents - list of events
	*/
	virtual void SetEvents(const UINT_32  iEvents) = 0;

	/**
	  @brief Watcher callback
	  @param iSocket - socket to watch
	  @param iREvents - list of events
	*/
	virtual void Callback(const INT_32   iSocket,
	                      const UINT_32  iREvents) = 0;

	/**
	  @brief Input/Output timeout callback
	*/
	virtual void IOTimeout() = 0;

	/**
	  @brief Shutdown context
	*/
	virtual void ShutdownContext() = 0;

	/**
	  @brief Set critical section flag
	*/
	virtual void SetCritical();

	/**
	  @brief Check critical section flag
	*/
	virtual bool InCritical() const;

	/**
	  @brief Reset critical section flag
	*/
	virtual void ResetCritical();

	/**
	  @brief A destructor
	*/
	virtual ~NetworkEventWatcher() throw();

protected:
	// Does not exist
	NetworkEventWatcher(const NetworkEventWatcher  & oRhs);
	NetworkEventWatcher& operator=(const NetworkEventWatcher  & oRhs);

	/** Critical section flag */
	INT_64     iInCritical;
};

} // namespace IRIS
#endif // _NETWORK_EVENT_WATCHER_HPP__
// End.
