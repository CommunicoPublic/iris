/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "Debug.hpp"
#include "Exception.hpp"
#include "RWLock.hpp"

#include <errno.h>

namespace IRIS
{

//
// Constructor
//
RWLock::RWLock()
{
	const INT_32 iRC = pthread_rwlock_init(&oRWLock, NULL);

	if (iRC != 0) { throw UnixException("RWLock::RWLock()", iRC); }
}

//
// Lock r/w lock for reading
//
RWLock::State RWLock::RLock()
{
	const INT_32 iRC = pthread_rwlock_rdlock(&oRWLock);
	if (iRC == DEADLOCK) { return NO; }

	if (iRC != 0) { throw UnixException("RWLock::RLock()", iRC); }

return OK;
}

//
// Try to lock r/w lock for reading
//
RWLock::State RWLock::TryRLock()
{
	const INT_32 iRC = pthread_rwlock_tryrdlock(&oRWLock);
	if (iRC == EBUSY) { return LOCKED; }

	if (iRC == DEADLOCK) { return NO; }

	if (iRC != 0) { throw UnixException("RWLock::TryRLock()", iRC); }

return OK;
}

//
// Lock r/w lock for writing
//
RWLock::State RWLock::WLock()
{
	const INT_32 iRC = pthread_rwlock_wrlock(&oRWLock);
	if (iRC == DEADLOCK) { return NO; }

	if (iRC != 0) { throw UnixException("RWLock::WLock()", iRC); }

return OK;
}

//
// Try to lock r/w lock for writing
//
RWLock::State RWLock::TryWLock()
{
	const INT_32 iRC = pthread_rwlock_trywrlock(&oRWLock);
	if (iRC == EBUSY) { return LOCKED; }

	if (iRC == DEADLOCK) { return NO; }

	if (iRC != 0) { throw UnixException("RWLock::TryWLock()", iRC); }

return OK;
}

//
// Unlock r/w lock for writing
//
RWLock::State RWLock::Unlock()
{
	const INT_32 iRC = pthread_rwlock_unlock(&oRWLock);
	if (iRC == EPERM) { return NO; }

	if (iRC != 0) { throw UnixException("RWLock::Unlock()", iRC); }

return OK;
}

//
// A destructor
//
RWLock::~RWLock() throw()
{
	const INT_32 iRC = pthread_rwlock_destroy(&oRWLock);

	// Check for fatal unrecoverable error
	Debug::CheckUnrecoverable("RWLock::~RWLock()", iRC);
}

//
// Constructor
//
RWLockRLocker::RWLockRLocker(RWLock & oIRWLock): oRWLock(oIRWLock) { oRWLock.RLock(); }

//
// Destructor
//
RWLockRLocker::~RWLockRLocker() throw()
{
	try
	{
		if (oRWLock.Unlock() != RWLock::OK) { Debug::CheckUnrecoverable("RWLockRLocker::~RWLockRLocker()/Unlock", EPERM); }
	}
	catch(UnixException & e) { Debug::CheckUnrecoverable("RWLockRLocker::~RWLockRLocker()/Unlock", e.err_no()); }
	catch(...)               { Debug::CheckUnrecoverable("RWLockRLocker::~RWLockRLocker()/...", EFAULT); }
}

//
// Constructor
//
RWLockWLocker::RWLockWLocker(RWLock & oIRWLock): oRWLock(oIRWLock) { oRWLock.WLock(); }

//
// Destructor
//
RWLockWLocker::~RWLockWLocker() throw()
{
	try
	{
		if (oRWLock.Unlock() != RWLock::OK) { Debug::CheckUnrecoverable("RWLockWLocker::~RWLockWLocker()/Unlock", EPERM); }
	}
	catch(UnixException & e) { Debug::CheckUnrecoverable("RWLockWLocker::~RWLockWLocker()/Unlock", e.err_no()); }
	catch(...)               { Debug::CheckUnrecoverable("RWLockWLocker::~RWLockWLocker()/...", EFAULT); }
}

} // namespace IRIS
// End.
