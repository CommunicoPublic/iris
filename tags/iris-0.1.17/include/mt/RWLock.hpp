/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _RW_LOCK_HPP__
#define _RW_LOCK_HPP__ 1

/**
  @file RWLock.hpp
  @brief Read/write lock
*/
#include "Types.h"

#include <pthread.h>

namespace IRIS
{

/**
  @class RWLock RWLock.hpp <RWLock.hpp>
  @brief Read/write lock
*/
class RWLock
{
public:
	enum State { OK, NO, DEADLOCK, LOCKED };

	/**
	  @brief Constructor
	*/
	RWLock();

	/**
	  @brief Lock r/w lock for reading
	  @return eMutexState, one of OK, DEADLOCK
	*/
	State RLock();

	/**
	  @brief Lock r/w lock for reading
	  @return eMutexState, one of OK, LOCKED
	*/
	State TryRLock();

	/**
	  @brief Lock r/w lock for writing
	  @return eMutexState, one of OK, DEADLOCK
	*/
	State WLock();

	/**
	  @brief Lock r/w lock for writing
	  @return eMutexState, one of OK, LOCKED
	*/
	State TryWLock();

	/**
	  @brief Unlock r/w lock
	  @return eMutexState, one of OK, NO
	*/
	State Unlock();

	/**
	  @brief A destructor
	*/
	~RWLock() throw();
private:
	// Does not exist
	RWLock(const RWLock & oRhs);
	RWLock & operator=(const RWLock & oRhs);

	pthread_rwlockattr_t  oAttributes;
	pthread_rwlock_t      oRWLock;
};

/**
  @class MutexLocker Mutex.hpp <Mutex.hpp>
  @brief Auto-locker for mutual exclusion class
*/
class RWLockRLocker
{
public:
	RWLockRLocker(RWLock & oIRWLock);

	~RWLockRLocker() throw();
private:
	RWLock  & oRWLock;
	// Does not exist
	RWLockRLocker(const RWLockRLocker & oRhs);
	RWLockRLocker & operator=(const RWLockRLocker & oRhs);
};

/**
  @class MutexLocker Mutex.hpp <Mutex.hpp>
  @brief Auto-locker for mutual exclusion class
*/
class RWLockWLocker
{
public:
	RWLockWLocker(RWLock & oIRWLock);

	~RWLockWLocker() throw();
private:
	RWLock  & oRWLock;
	// Does not exist
	RWLockWLocker(const RWLockWLocker & oRhs);
	RWLockWLocker & operator=(const RWLockWLocker & oRhs);
};

} // namespace IRIS
#endif // _RW_LOCK_HPP__
// End.
