/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _EVENT_TIMER_HPP__
#define _EVENT_TIMER_HPP__ 1

#include <ev.h>

#include "Types.h"

namespace IRIS
{
// FWD
class EventLoop;

/**
  @class EventTimer EventTimer.hpp <EventTimer.hpp>
  @brief Event watcher
*/
class EventTimer
{
public:
	/**
	  @brief Constructor
	  @param pIEventLoop - event loop object
	  @param dTimeout - Timer timeout
	  @param dRepeat - Repreat time interval
	*/
	EventTimer(EventLoop      * pIEventLoop,
	           const W_FLOAT    dTimeout,
	           const W_FLOAT    dRepeat);

	/**
	  @brief Watcher callback
	  @param iREvents - list of events
	*/
	virtual void Callback(const UINT_32  iREvents) = 0;

	/**
	  @brief Start watching for events
	*/
	void StartWatch();

	/**
	  @brief Stop watching for events
	*/
	void StopWatch();

	/**
	  @brief Virtual destructor
	*/
	virtual ~EventTimer() throw();
private:
	// Does not exist
	EventTimer(const EventTimer  & oRhs);
	EventTimer& operator=(const EventTimer  & oRhs);

protected:
	/** Event loop   */
	EventLoop  * pEventLoop;
private:
	/** Indicates that watching started */
	bool      bWatchStarted;
	/** LibEV data                      */
	ev_timer  oEvTimer;

	/**
	  @brief LibEV watcher callback
	  @param pLoop - LibEv event loop
	  @param pWatcher - LibEV watcher object
	  @param iREV - list of events
	*/
	static void WatcherCallback(struct ev_loop   * pLoop,
	                            struct ev_timer  * pWatcher,
	                            int                iREV);
};

} // namespace IRIS
#endif // _EVENT_TIMER_HPP__
// End.
