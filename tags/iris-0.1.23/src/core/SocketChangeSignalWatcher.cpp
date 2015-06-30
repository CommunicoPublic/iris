/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "SocketChangeSignalWatcher.hpp"
#include "AcceptLoopContext.hpp"
#include "Debug.hpp"
#include "Exception.hpp"
#include "MainLoopContext.hpp"
#include "NetworkEventWatcher.hpp"
#include "Mutex.hpp"

#include <stdio.h>

namespace IRIS
{

//
// Constructor
//
SocketChangeSignalWatcher::SocketChangeSignalWatcher(AcceptLoopContext  & oIAcceptLoopContext): IOAsyncWatcher(&(oIAcceptLoopContext.GetEventLoop())),
                                                                                                oAcceptLoopContext(oIAcceptLoopContext)
{
	StartWatch();
}

//
// Watcher callback
//
void SocketChangeSignalWatcher::Callback(const UINT_32  iREvents) { oAcceptLoopContext.HandleEvents(); }

//
// A destructor
//
SocketChangeSignalWatcher::~SocketChangeSignalWatcher() throw() { ;; }

} // namespace IRIS
// End.
