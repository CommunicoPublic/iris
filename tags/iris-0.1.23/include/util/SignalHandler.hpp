/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SIGNAL_HANDLER_HPP__
#define _SIGNAL_HANDLER_HPP__ 1

/**
  @file SignalHandler.hpp
  @brief C++-style Signal handler
*/

#include "Types.h"
#include "STLVector.hpp"

#include <signal.h>

#define C_MAX_SIGNAL_NUM 64

namespace IRIS
{

/**
  @class UserSignalHandler SignalHandler.hpp <SignalHandler.hpp>
  @brief Pure abstract class.
*/
class UserSignalHandler
{
public:
	/**
	  @brief A signal handler
	  @param iSignal - signal number
	*/
	virtual void Handler(const INT_32 iSignal) = 0;
	/**
	  @brief A Virtual Destructor
	*/
	virtual ~UserSignalHandler() throw() { ;; }
};

/**
  @class SignalHandler SignalHandler.hpp <SignalHandler.hpp>
  @brief Signal Handler. Singleton.
*/
class SignalHandler
{
public:
	/**
	  @brief Register user Handler
	  @param iSignal - signal
	  @param pHandler - pointer to the handler
	*/
	INT_32 RegisterHandler(const INT_32 iSignal, UserSignalHandler * pHandler);

	/**
	  @brief Remove handler from factory
	  @param iSignal - signal
	  @param pHandler - pointer to the handler
	*/
	INT_32 RemoveHandler(const INT_32 iSignal, UserSignalHandler * pHandler);

	/**
	  @brief Create singleton instance
	*/
	static SignalHandler & Instance();

	/**
	  @brief Remove all signal handlers
	*/
	void Clear();

private:
	/** User handler Factory */
	static STLW::vector<UserSignalHandler *> mHandler[C_MAX_SIGNAL_NUM];

	/** Signal Action */
	static struct sigaction sSA;

	// Does not exist
	SignalHandler(const SignalHandler & oSignalHandler);
	SignalHandler & operator =(const SignalHandler & oSignalHandler);

	/**
	  @brief Signal Handler
	  @param iSignal - signal
	*/
	static void Handler(INT_32 iSignal);

	/** Private Constructor */
	SignalHandler();
};

} // namespace IRIS
#endif // _SIGNAL_HANDLER_HPP__
// End.
