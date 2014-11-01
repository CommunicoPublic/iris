/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SOCKET_CHANGE_SIGNAL_WATCHER_HPP__
#define _SOCKET_CHANGE_SIGNAL_WATCHER_HPP__ 1

/**
  @file SocketChangeSignalWatcher.hpp
  @brief Watcher for asyncronous events
*/

#include "IOAsyncWatcher.hpp"
#include "IOWatcher.hpp"

namespace IRIS
{
// FWD
class AcceptLoopContext;

/**
  @class SocketChangeSignalWatcher SocketChangeSignalWatcher.hpp <SocketChangeSignalWatcher.hpp>
  @brief Watcher for asyncronous events
*/
class SocketChangeSignalWatcher:
  public IOAsyncWatcher
{
public:
	/**
	  @brief Constructor
	  @param pIContext - event context
	*/
	SocketChangeSignalWatcher(AcceptLoopContext  * pIContext);

	/**
	  @brief Watcher callback
	  @param iREvents - list of events
	*/
	void Callback(const UINT_32  iREvents);

	/**
	  @brief A destructor
	*/
	~SocketChangeSignalWatcher() throw();
private:
	// Does not exist
	SocketChangeSignalWatcher(const SocketChangeSignalWatcher  & oRhs);
	SocketChangeSignalWatcher& operator=(const SocketChangeSignalWatcher  & oRhs);

	/** Event context */
	AcceptLoopContext  * pContext;
};

} // namespace IRIS
#endif // _SOCKET_CHANGE_SIGNAL_WATCHER_HPP__
// End.
