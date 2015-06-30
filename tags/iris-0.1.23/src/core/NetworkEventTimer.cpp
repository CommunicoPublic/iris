/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

#include "AcceptLoopContext.hpp"
#include "MainLoopContext.hpp"
#include "NetworkEventTimer.hpp"
#include "NetworkEventWatcher.hpp"

#include <stdio.h>

namespace IRIS
{

//
// Constructor
//
NetworkEventTimer::NetworkEventTimer(AcceptLoopContext  & oIAcceptLoopContext): EventTimer(&(oIAcceptLoopContext.GetEventLoop()),
                                                                                oIAcceptLoopContext.GetMainLoopContext().timeout_check_interval,
                                                                                oIAcceptLoopContext.GetMainLoopContext().timeout_check_interval),
                                                                                oAcceptLoopContext(oIAcceptLoopContext)
{
	if (oAcceptLoopContext.GetMainLoopContext().timeout_check_interval != -1) { StartWatch(); }
}

//
// Watcher callback
//
void NetworkEventTimer::Callback(const UINT_32  iREvents) { oAcceptLoopContext.HandleTimeouts(); }

//
// A destructor
//
NetworkEventTimer::~NetworkEventTimer() throw()
{
	;;
}

} // namespace IRIS
// End.
