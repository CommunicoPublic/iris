/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MUTEX_HPP__
#define _MUTEX_HPP__ 1

/**
  @file Mutex.hpp
  @brief Mutual exclusion class
*/
#include "Types.h"

#include <pthread.h>

namespace IRIS
{

/**
  @class Mutex Mutex.hpp <Mutex.hpp>
  @brief Mutual exclusion class
*/
class Mutex
{
public:
	enum State { OK, NO, DEADLOCK, LOCKED };
	/**
	  @brief Constructor
	*/
	Mutex(const bool bRecursive = false);

	/**
	  @brief Lock mutex
	  @return State, one of OK, DEADLOCK
	*/
	State Lock();

	/**
	  @brief Unlock mutex
	  @return State, one of OK, NO
	*/
	State Unlock();

	/**
	  @brief Try to lock mutex
	  @return State, one of OK, LOCKED
	*/
	State TryLock();

	/**
	  @brief A destructor
	*/
	~Mutex() throw();
private:
	// Does not exist
	Mutex(const Mutex & oRhs);
	Mutex & operator=(const Mutex & oRhs);

	pthread_mutexattr_t  oAttributes;
	pthread_mutex_t      oMutex;
};

/**
  @class MutexLocker Mutex.hpp <Mutex.hpp>
  @brief Auyo-locker for mutual exclusion class
*/
class MutexLocker
{
public:
	MutexLocker(Mutex & oIMutex);

	~MutexLocker() throw();
private:
	Mutex  & oMutex;
	// Does not exist
	MutexLocker(const MutexLocker & oRhs);
	MutexLocker & operator=(const MutexLocker & oRhs);
};

/**
  @class MutexLocker Mutex.hpp <Mutex.hpp>
  @brief Auto-locker for mutual exclusion class; lock mutex at exit
*/
class MutexLockerOnExit
{
public:
	MutexLockerOnExit(Mutex & oIMutex);

	~MutexLockerOnExit() throw();
private:
	Mutex  & oMutex;
	// Does not exist
	MutexLockerOnExit(const MutexLockerOnExit & oRhs);
	MutexLockerOnExit & operator=(const MutexLockerOnExit & oRhs);
};

} // namespace IRIS
#endif // _MUTEX_HPP__
// End.
