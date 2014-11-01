/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "Debug.hpp"
#include "Exception.hpp"
#include "Mutex.hpp"

#include <errno.h>
#include <stdlib.h>

namespace IRIS
{

//
// Constructor
//
Mutex::Mutex(const bool bRecursive)
{
	INT_32 iRC = pthread_mutexattr_init(&oAttributes);
	if (iRC != 0) { throw UnixException("Mutex::Mutex()", iRC); }
	if (bRecursive) { pthread_mutexattr_settype(&oAttributes, PTHREAD_MUTEX_RECURSIVE); }

	iRC = pthread_mutex_init(&oMutex, &oAttributes);
	if (iRC != 0)
	{
		pthread_mutexattr_destroy(&oAttributes);
		throw UnixException("Mutex::Mutex()", iRC);
	}
}

//
// Lock mutex
//
Mutex::State Mutex::Lock()
{
	const INT_32 iRC = pthread_mutex_lock(&oMutex);
	if (iRC == EDEADLK) { return DEADLOCK; }

	if (iRC != 0) { throw UnixException("Mutex::Lock()", iRC); }

return OK;
}

//
// Unlock mutex
//
Mutex::State Mutex::Unlock()
{
	const INT_32 iRC = pthread_mutex_unlock(&oMutex);
	if (iRC == EPERM) { return NO; }

	if (iRC != 0) { throw UnixException("Mutex::Unlock()", iRC); }

return OK;
}

//
// Try to lock mutex
//
Mutex::State Mutex::TryLock()
{
	const INT_32 iRC = pthread_mutex_trylock(&oMutex);
	if (iRC == EBUSY) { return LOCKED; }

	if (iRC != 0) { throw UnixException("Mutex::TryLock()", iRC); }

return OK;
}

//
// A destructor
//
Mutex::~Mutex() throw()
{
	INT_32 iRC = pthread_mutex_destroy(&oMutex);
	// Check for fatal unrecoverable error
	Debug::CheckUnrecoverable("Mutex::~Mutex()", iRC);

	iRC = pthread_mutexattr_destroy(&oAttributes);
	Debug::CheckUnrecoverable("Mutex::~Mutex()", iRC);
}

//
// Constructor
//
MutexLocker::MutexLocker(Mutex & oIMutex): oMutex(oIMutex)
{
	if (oMutex.Lock() != Mutex::OK)
	{
		Debug::CheckUnrecoverable("MutexLocker::MutexLocker()", EDEADLK);
	}
}

//
// Destructor
//
MutexLocker::~MutexLocker() throw()
{
	try
	{
		if (oMutex.Unlock() != Mutex::OK) { Debug::CheckUnrecoverable("MutexLocker::~MutexLocker()/Unlock", EPERM); }
	}
	catch(UnixException & e) { Debug::CheckUnrecoverable("MutexLocker::~MutexLocker()/Unlock", e.err_no()); }
	catch(...)               { Debug::CheckUnrecoverable("MutexLocker::~MutexLocker()/...", EFAULT); }
}

//
// Constructor
//
MutexLockerOnExit::MutexLockerOnExit(Mutex & oIMutex): oMutex(oIMutex) { ;; }

//
// Destructor
//
MutexLockerOnExit::~MutexLockerOnExit() throw()
{
	try
	{
		if (oMutex.Lock() != Mutex::OK) { Debug::CheckUnrecoverable("MutexLockerOnExit::~MutexLockerOnExit()/Unlock", EDEADLK); }
	}
	catch(UnixException & e) { Debug::CheckUnrecoverable("MutexLockerOnExit::~MutexLockerOnExit()/Lock", e.err_no()); }
	catch(...)               { Debug::CheckUnrecoverable("MutexLockerOnExit::~MutexLockerOnExit()/...", EFAULT); }
}

} // namespace IRIS
// End.
