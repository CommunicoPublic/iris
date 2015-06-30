/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "ConditionVariable.hpp"
#include "Debug.hpp"
#include "Exception.hpp"

#include <errno.h>
#include <stdio.h>

namespace IRIS
{

//
// Constructor
//
ConditionVariable::ConditionVariable()
{
	INT_32 iRC = pthread_mutex_init(&oMutex, NULL);
	if (iRC != 0) { throw UnixException("ConditionVariable::ConditionVariable()/Mutex", iRC); }

	iRC = pthread_cond_init(&oCondition, NULL);
	if (iRC != 0)
	{
		pthread_mutex_destroy(&oMutex);
		throw UnixException("ConditionVariable::ConditionVariable()/CondVar", iRC);
	}
}

//
// Lock mutex
//
ConditionVariable::State ConditionVariable::Lock()
{
	const INT_32 iRC = pthread_mutex_lock(&oMutex);

	if (iRC == EDEADLK) { return DEADLOCK; }

	if (iRC != 0) { throw UnixException("ConditionVariable::Lock()", iRC); }

return OK;
}

//
// Lock mutex
//
ConditionVariable::State ConditionVariable::Unlock()
{
	const INT_32 iRC = pthread_mutex_unlock(&oMutex);

	if (iRC == EPERM) { return NO; }

	if (iRC != 0) { throw UnixException("ConditionVariable::Unlock()", iRC); }

return OK;
}

//
// Wait for signal
//
ConditionVariable::State ConditionVariable::Wait()
{
	const INT_32 iRC = pthread_cond_wait(&oCondition, &oMutex);

	if (iRC == EPERM) { return NOT_LOCKED; }

	if (iRC != 0) { throw UnixException("ConditionVariable::Wait()", iRC); }

return OK;
}

//
// Send signal
//
ConditionVariable::State ConditionVariable::Notify()
{
	const INT_32 iRC = pthread_cond_signal(&oCondition);

	if (iRC == EINVAL) { return NO; }

	// Yhis should *never* happened
	if (iRC != 0) { throw UnixException("ConditionVariable::Notify()", iRC); }

return OK;
}

//
// A destructor
//
ConditionVariable::~ConditionVariable() throw()
{
	const INT_32 iRCC = pthread_cond_destroy(&oCondition);
	const INT_32 iRCM = pthread_mutex_destroy(&oMutex);

	// Check for fatal unrecoverable error
	Debug::CheckUnrecoverable("ConditionVariable::~ConditionVariable()/CondVar", iRCC);
	Debug::CheckUnrecoverable("ConditionVariable::~ConditionVariable()/Mutex", iRCM);
}

//
// Constructor
//
Condition::Condition(): bFlag(false) { ;; }

//
// Send signal to other thread
//
void Condition::Signal()
{
	oCondVar.Lock();
	bFlag = true;
	oCondVar.Notify();
	oCondVar.Unlock();
}

//
// Wait for signal from thread
//
void Condition::Wait()
{
	oCondVar.Lock();
	while (!bFlag) { oCondVar.Wait(); }
	bFlag = false;
	oCondVar.Unlock();
}

//
// A destructor
//
Condition::~Condition() throw() { ;; }

} // namespace IRIS
// End.
