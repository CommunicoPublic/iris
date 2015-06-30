/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _CONDITION_VARIABLE_HPP__
#define _CONDITION_VARIABLE_HPP__ 1

/**
  @file ConditionVariable.hpp
  @brief Thread condition variable class
*/
#include "Types.h"

#include <pthread.h>

namespace IRIS
{

/**
  @class ConditionVariable ConditionVariable.hpp <ConditionVariable.hpp>
  @brief Thread condition variable class
*/
class ConditionVariable
{
public:
	enum State { OK, NO, NOT_LOCKED, DEADLOCK };

	/**
	  @brief Constructor
	*/
	ConditionVariable();

	/**
	  @brief Lock mutex
	  @return State, one of OK, DEADLOCK
	*/
	State Lock();

	/**
	  @brief Unock mutex
	  @return State, one of OK, DEADLOCK
	*/
	State Unlock();

	/**
	  @brief Wait for condition
	  @return State, one of OK, NO
	*/
	State Wait();

	/**
	  @brief Send signal
	  @return State, one of OK, LOCKED
	*/
	State Notify();

	/**
	  @brief A destructor
	*/
	~ConditionVariable() throw();
private:
	// Does not exist
	ConditionVariable(const ConditionVariable & oRhs);
	ConditionVariable & operator=(const ConditionVariable & oRhs);

	/** Condition mutex */
	pthread_mutex_t  oMutex;
	/** Condition variable */
	pthread_cond_t   oCondition;
};

/**
  @file ConditionVariable.hpp
  @brief Thread condition variable class
*/
class Condition
{
public:
	/**
	  @brief Constructor
	*/
	Condition();

	/**
	  @brief Send signal to other thread
	*/
	void Signal();

	/**
	  @brief Wait for signal from thread
	*/
	void Wait();

	/**
	  @brief A destructor
	*/
	~Condition() throw();
private:
	// Does not exist
	Condition(const Condition & oRhs);
	Condition & operator=(const Condition & oRhs);

	/** Task condition variable         */
	ConditionVariable  oCondVar;
	/** Task condition variable flag    */
	bool               bFlag;
};

} // namespace IRIS
#endif // _CONDITION_VARIABLE_HPP__
// End.
