/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

#include "EventLoop.hpp"

#ifndef EVBREAK_ALL
#define EVBREAK_ALL EVUNLOOP_ALL
#endif // EVBREAK_ALL

namespace IRIS
{

//
// Constructor
//
EventLoop::EventLoop() { pLoop = ev_loop_new(EVFLAG_AUTO); }

//
// Start loop
//
void EventLoop::Loop() { ev_loop(pLoop, 0); }

//
// End loop
//
void EventLoop::UnLoop() { ev_unloop(pLoop, EVBREAK_ALL); }

//
// Destructor
//
EventLoop::~EventLoop() throw() { ev_loop_destroy(pLoop); }

} // namespace IRIS
// End.
