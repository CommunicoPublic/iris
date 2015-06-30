/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _NETWORK_EVENT_TIMER_HPP__
#define _NETWORK_EVENT_TIMER_HPP__ 1

/**
  @file NetworkEventTimer.hpp
  @brief Iris network watcher which work with connected client
*/

#include "EventTimer.hpp"

namespace IRIS
{
// FWD
class AcceptLoopContext;

/**
  @class NetworkEventTimer NetworkEventTimer.hpp <NetworkEventTimer.hpp>
  @brief Client events watcher
*/
class NetworkEventTimer:
  public EventTimer
{
public:
	/**
	  @brief Constructor
	  @param pIContext - accept event context
	*/
	NetworkEventTimer(AcceptLoopContext  & oIAcceptLoopContext);

	/**
	  @brief Watcher callback
	  @param iREvents - list of events
	*/
	void Callback(const UINT_32  iREvents);

	/**
	  @brief A destructor
	*/
	virtual ~NetworkEventTimer() throw();
protected:
	// Does not exist
	NetworkEventTimer(const NetworkEventTimer  & oRhs);
	NetworkEventTimer& operator=(const NetworkEventTimer  & oRhs);

	/** Event context     */
	AcceptLoopContext  & oAcceptLoopContext;
};

} // namespace IRIS
#endif // _NETWORK_EVENT_TIMER_HPP__
// End.
