/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "EventLoop.hpp"
#include "MainLoopStopWatcher.hpp"

namespace IRIS
{

//
// Constructor
//
MainLoopStopWatcher::MainLoopStopWatcher(EventLoop  * pILoop): IOAsyncWatcher(pILoop),
                                                               pLoop(pILoop)
{
	StartWatch();
}

//
// Watcher callback
//
void MainLoopStopWatcher::Callback(const UINT_32  iREvents)
{
	pLoop -> UnLoop();
}

//
// A destructor
//
MainLoopStopWatcher::~MainLoopStopWatcher() throw() { ;; }

} // namespace IRIS
// End.
