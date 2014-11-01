/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "IOAsyncWatcher.hpp"
#include "EventLoop.hpp"

namespace IRIS
{

//
// Constructor
//
IOAsyncWatcher::IOAsyncWatcher(EventLoop  * pIEventLoop): pEventLoop(pIEventLoop),
                                                          bWatchStarted(false)
{
	/*
	 * http://lists.schmorp.de/pipermail/libev/2008q4/000441.html
	 */
	ev_async  * pEventAsyncStupidFix = &oEvAsync;
	ev_async_init(pEventAsyncStupidFix, IOAsyncWatcher::WatcherCallback);
	oEvAsync.data = this;
}

//
// Watcher callback
//
void IOAsyncWatcher::WatcherCallback(struct ev_loop   * pLoop,
                                     struct ev_async  * pWatcher,
                                     int                iREV)
{
	IOAsyncWatcher * pThis = (IOAsyncWatcher *)pWatcher -> data;

	pThis -> Callback(iREV);
}

//
// Start watch
//
void IOAsyncWatcher::StartWatch()
{
	if (bWatchStarted) { return; }
	ev_async_start(pEventLoop -> pLoop, &oEvAsync);
}

//
// Stop watch
//
void IOAsyncWatcher::StopWatch()
{
	if (!bWatchStarted) { return; }
	ev_async_stop(pEventLoop -> pLoop, &oEvAsync);
}

//
// Send signal
//
void IOAsyncWatcher::SendSignal()
{
	ev_async_send(pEventLoop -> pLoop, &oEvAsync);
}

//
// Destructor
//
IOAsyncWatcher::~IOAsyncWatcher() throw() { StopWatch(); }

} // namespace IRIS
// End.
