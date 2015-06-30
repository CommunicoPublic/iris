/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

#include "IOWatcher.hpp"
#include "EventLoop.hpp"

#include <stdio.h>

namespace IRIS
{

const UINT_32 IOWatcher::READ    = 0x00000001;
const UINT_32 IOWatcher::WRITE   = 0x00000002;
const UINT_32 IOWatcher::ERROR   = 0x10000000;
const UINT_32 IOWatcher::REMOVE  = 0x20000000;

//
// Constructor
//
IOWatcher::IOWatcher(EventLoop  * pIEventLoop): pEventLoop(pIEventLoop),
                                                bWatchStarted(false)
{
	/*
	 * http://lists.schmorp.de/pipermail/libev/2008q4/000441.html
	 */
	ev_io  * pEventIOStupidFix = &oEvIO;
	ev_init(pEventIOStupidFix, IOWatcher::WatcherCallback);
	oEvIO.data = this;
}

//
// Set events
//
void IOWatcher::SetEvents(const INT_32   iSocket,
                          const UINT_32  iEvents)
{
	iIOEvents = iEvents;

	int iEV = 0;

	if (iEvents & READ)  { iEV |= EV_READ;  }
	if (iEvents & WRITE) { iEV |= EV_WRITE; }
	if (iEvents & ERROR) { iEV |= EV_ERROR; }

	ev_io_set(&oEvIO, iSocket, iEV);
}


//
// Get watching events
//
UINT_32 IOWatcher::GetEvents() const { return iIOEvents; }

//
// Watcher callback
//
void IOWatcher::WatcherCallback(struct ev_loop  * pLoop,
                                struct ev_io    * pWatcher,
                                int               iREV)
{
	IOWatcher * pThis = (IOWatcher *)pWatcher -> data;

	UINT_32 iREvents = 0;
	if (iREV & EV_READ)  { iREvents |= READ;  }
	if (iREV & EV_WRITE) { iREvents |= WRITE; }
	if (iREV & EV_ERROR) { iREvents |= ERROR; }

	pThis -> Callback(pWatcher -> fd, iREvents);
}

//
// Start watch
//
void IOWatcher::StartWatch()
{
	if (bWatchStarted) { return; }
	ev_io_start(pEventLoop -> pLoop, &oEvIO);
	bWatchStarted = true;
}

//
// Stop watch
//
void IOWatcher::StopWatch()
{
	if (!bWatchStarted) { return; }
	ev_io_stop(pEventLoop -> pLoop, &oEvIO);
	bWatchStarted = false;
}

//
// Destructor
//
IOWatcher::~IOWatcher() throw() { StopWatch(); }

} // namespace IRIS
// End.
