/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _ACCEPT_LOOP_STOP_WATCHER_HPP__
#define _ACCEPT_LOOP_STOP_WATCHER_HPP__ 1

/**
  @file AcceptLoopStopWatcher.hpp
  @brief Watcher for asyncronous stop server event (for network thread)
*/

#include "IOAsyncWatcher.hpp"

namespace IRIS
{
// FWD
class AcceptLoopContext;

/**
  @class AcceptLoopStopWatcher AcceptLoopStopWatcher.hpp <AcceptLoopStopWatcher.hpp>
  @brief Watcher for asyncronous stop server event (for network thread)
*/
class AcceptLoopStopWatcher:
  public IOAsyncWatcher
{
public:
	/**
	  @brief Constructor
	  @param pIAcceptLoopContext - accept loop event context
	*/
	AcceptLoopStopWatcher(AcceptLoopContext  & oIAcceptLoopContext);

	/**
	  @brief Watcher callback
	  @param iREvents - list of events
	*/
	void Callback(const UINT_32  iREvents);

	/**
	  @brief A destructor
	*/
	~AcceptLoopStopWatcher() throw();
private:
	// Does not exist
	AcceptLoopStopWatcher(const AcceptLoopStopWatcher  & oRhs);
	AcceptLoopStopWatcher& operator=(const AcceptLoopStopWatcher  & oRhs);

	/** Event context */
	AcceptLoopContext  & oAcceptLoopContext;
};

} // namespace IRIS
#endif // _ACCEPT_LOOP_STOP_WATCHER_HPP__
// End.
