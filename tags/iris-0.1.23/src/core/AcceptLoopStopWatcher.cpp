/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "AcceptLoopStopWatcher.hpp"
#include "AcceptLoopContext.hpp"
#include "Debug.hpp"
#include "Exception.hpp"
#include "GlobalContext.hpp"
#include "MainLoopContext.hpp"
#include "NetworkEventWatcher.hpp"

namespace IRIS
{

//
// Constructor
//
AcceptLoopStopWatcher::AcceptLoopStopWatcher(AcceptLoopContext  & oIAcceptLoopContext): IOAsyncWatcher(&(oIAcceptLoopContext.GetEventLoop())),
                                                                                                         oAcceptLoopContext(oIAcceptLoopContext)
{
	StartWatch();
}

//
// Watcher callback
//
void AcceptLoopStopWatcher::Callback(const UINT_32  iREvents)
{
	try
	{
		oAcceptLoopContext.Shutdown();
	}
	catch(UnixException  & e)
	{
		Debug::CheckUnrecoverable("AcceptLoopStopWatcher::Callback/UnixException", e.err_no());
	}
	catch(...)
	{
		Debug::Abort("AcceptLoopStopWatcher::Callback/Unknown exception");
	}
}

//
// A destructor
//
AcceptLoopStopWatcher::~AcceptLoopStopWatcher() throw() { ;; }

} // namespace IRIS
// End.
