/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "Debug.hpp"
#include "Exception.hpp"
#include "Semaphore.hpp"

#include <errno.h>

namespace IRIS
{

//
// Constructor
//
Semaphore::Semaphore(const UINT_32 iValue)
{
	const INT_32 iRC = sem_init(&oSemaphore, 0, iValue);

	if (iRC != 0) { throw UnixException("Semaphore::Semaphore()", errno); }
}

//
// Lock r/w lock for reading
//
Semaphore::State Semaphore::Wait()
{
	const INT_32 iRC = sem_wait(&oSemaphore);

	if (iRC != 0) { throw UnixException("Semaphore::Wait()", errno); }

return OK;
}

//
// Try to lock r/w lock for reading
//
Semaphore::State Semaphore::TryWait()
{
	const INT_32 iRC = sem_trywait(&oSemaphore);
	if (errno == EBUSY) { return LOCKED; }

	if (iRC != 0) { throw UnixException("Semaphore::TryWait()", errno); }

return OK;
}

//
// Lock r/w lock for writing
//
Semaphore::State Semaphore::Post()
{
	const INT_32 iRC = sem_post(&oSemaphore);

	if (iRC != 0) { throw UnixException("Semaphore::Post()", errno); }

return OK;
}

//
// A destructor
//
Semaphore::~Semaphore() throw()
{
	const INT_32 iRC = sem_destroy(&oSemaphore);
	if (iRC != 0)
	{
		// Check for fatal unrecoverable error
		Debug::CheckUnrecoverable("Semaphore::~Semaphore()", errno);
	}
}

} // namespace IRIS
// End.
