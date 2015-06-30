/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "MainLoopContext.hpp"

namespace IRIS
{

//
// Constructor
//
MainLoopContext::MainLoopContext(GlobalContext  & oIGlobalContext): global_context(oIGlobalContext),
                                                                    stop_event_watcher(&main_loop)
{
	;;
}

} // namespace IRIS
// End.
