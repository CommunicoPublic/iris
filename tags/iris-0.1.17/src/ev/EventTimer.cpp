/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

#include "EventTimer.hpp"
#include "EventLoop.hpp"

#include <stdio.h>

namespace IRIS
{

//
// Constructor
//
EventTimer::EventTimer(EventLoop      * pIEventLoop,
                       const W_FLOAT    dTimeout,
                       const W_FLOAT    dRepeat): pEventLoop(pIEventLoop),
                                                  bWatchStarted(false)
{
	/*
	 * http://lists.schmorp.de/pipermail/libev/2008q4/000441.html
	 */
	ev_timer  * pEventTimerStupidFix = &oEvTimer;
	ev_timer_init(pEventTimerStupidFix, EventTimer::WatcherCallback, dTimeout, dRepeat);
	oEvTimer.data = this;
}

//
// Watcher callback
//
void EventTimer::WatcherCallback(struct ev_loop   * pLoop,
                                 struct ev_timer  * pWatcher,
                                 int                iREV)
{
	EventTimer * pThis = (EventTimer *)pWatcher -> data;

	pThis -> Callback(iREV);
}

//
// Start watch
//
void EventTimer::StartWatch()
{
	if (bWatchStarted) { return; }
	ev_timer_start(pEventLoop -> pLoop, &oEvTimer);
	bWatchStarted = true;
}

//
// Stop watch
//
void EventTimer::StopWatch()
{
	if (!bWatchStarted) { return; }
	ev_timer_stop(pEventLoop -> pLoop, &oEvTimer);
	bWatchStarted = false;
}

//
// Destructor
//
EventTimer::~EventTimer() throw() { StopWatch(); }

} // namespace IRIS
// End.
