/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "AcceptLoopContext.hpp"
#include "GlobalContext.hpp"
#include "MainLoopContext.hpp"

namespace IRIS
{

//
// Constructor
//
AcceptLoopContext::AcceptLoopContext(MainLoopContext  & oMainContext): mutex(true),
                                                                       main_context(oMainContext),
                                                                       accept_signal_watcher(this),
                                                                       stop_signal_watcher(this),
                                                                       socket_change_signal_watcher(this)
{
	;;
}

//
// Delete watcher
//
void AcceptLoopContext::DeleteWatcher(NetworkEventWatcher  * pWatcher)
{
	this -> clients.DeleteWatcher(pWatcher);
	CRITICAL
	{
		MutexLocker oLocker(this -> mutex);
		if (this -> main_context.global_context.shutdown_type != GlobalContext::NONE && clients.Size() == 0)
		{
			thread_loop.UnLoop();
		}
	}
}

//
// Destructor
//
AcceptLoopContext::~AcceptLoopContext() throw() { ;; }

} // namespace IRIS
// End.
