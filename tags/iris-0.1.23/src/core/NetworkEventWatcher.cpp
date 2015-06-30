/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "AcceptLoopContext.hpp"
#include "NetworkEventWatcher.hpp"

namespace IRIS
{
//
// Constructor
//
NetworkEventWatcher::NetworkEventWatcher(AcceptLoopContext  & oIAcceptLoopContext,
                                         ServiceConfig      & oIServiceConfig): IOWatcher(&oIAcceptLoopContext.GetEventLoop()),
                                                                                oAcceptLoopContext(oIAcceptLoopContext),
                                                                                oServiceConfig(oIServiceConfig),
                                                                                iInCritical(0) { ;; }

//
// Set critical section flag
//
INT_32 NetworkEventWatcher::SetCritical()
{
	CRITICAL
	{
		MutexLocker oLocker(oAcceptLoopContext.oMutex);
		++iInCritical;
		return iInCritical;
	}
return 0;
}

//
// Check critical section flag
//
bool NetworkEventWatcher::InCritical() const
{
	CRITICAL
	{
		MutexLocker oLocker(oAcceptLoopContext.oMutex);

		return iInCritical != 0;
	}
return false;
}

//
// Reset critical section flag
//
INT_32 NetworkEventWatcher::ResetCritical()
{
	CRITICAL
	{
		MutexLocker oLocker(oAcceptLoopContext.oMutex);

		--iInCritical;
		return iInCritical;
	}
return 0;
}

//
// Destructor
//
NetworkEventWatcher::~NetworkEventWatcher() throw() { ;; }

} // namespace IRIS
// End.
