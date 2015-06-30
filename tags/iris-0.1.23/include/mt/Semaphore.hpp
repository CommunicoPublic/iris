/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SEMAPHORE_HPP__
#define _SEMAPHORE_HPP__ 1

/**
  @file Semaphore.hpp
  @brief POSIX semaphore implementation
*/
#include "Types.h"

#include <semaphore.h>

namespace IRIS
{

/**
  @class Semaphore Semaphore.hpp <Semaphore.hpp>
  @brief Semaphore implementation
*/
class Semaphore
{
public:
	enum State { OK, NO, LOCKED };

	/**
	  @brief Constructor
	  @param iValue - Initial semaphore value
	*/
	Semaphore(const UINT_32 iValue = 1);

	/**
	  @brief Lock semaphore
	  @return State, one of OK, NO
	*/
	State Wait();

	/**
	  @brief Try to lock semaphore
	  @return State, one of OK, LOCKED
	*/
	State TryWait();

	/**
	  @brief Unlock semapthore
	  @return State, one of OK, NO
	*/
	State Post();

	/**
	  @brief A destructor
	*/
	~Semaphore() throw();
private:
	// Does not exist
	Semaphore(const Semaphore & oRhs);
	Semaphore & operator=(const Semaphore & oRhs);

	/** POSIX semaphore */
	sem_t    oSemaphore;
};

} // namespace IRIS
#endif // _SEMAPHORE_HPP__
// End.
