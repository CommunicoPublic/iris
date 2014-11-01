/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "NetworkEventWatcher.hpp"

namespace IRIS
{
//
// Constructor
//
NetworkEventWatcher::NetworkEventWatcher(EventLoop  * pIEventLoop): IOWatcher(pIEventLoop), iInCritical(0) { ;; }

//
// Set critical section flag
//
void NetworkEventWatcher::SetCritical() { ++iInCritical; }

//
// Check critical section flag
//
bool NetworkEventWatcher::InCritical() const { return (iInCritical > 0); }

//
// Set critical section flag
//
void NetworkEventWatcher::ResetCritical() { --iInCritical; }

//
// Destructor
//
NetworkEventWatcher::~NetworkEventWatcher() throw() { ;; }

} // namespace IRIS
// End.
