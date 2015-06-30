/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MAIN_SIGNAL_HANDLER_HPP__
#define _MAIN_SIGNAL_HANDLER_HPP__ 1

/**
  @file MainSignalHandler.hpp
  @brief Unix signal handlers for main process
*/

// Local includes
#include "SignalHandler.hpp"

namespace IRIS
{
// FWD
struct GlobalContext;

/**
  @class MainSignalHandler MainSignalHandlers.hpp <MainSignalHandlers.hpp>
  @brief Signal handler implementation
*/
class MainSignalHandler:
  public UserSignalHandler
{
public:
	/**
	  @brief Constructor
	  @param oIGlobalContext - global thread context
	*/
	MainSignalHandler(GlobalContext  & oIGlobalContext);

	/**
	  @brief A signal handler
	*/
	void Handler(INT_32 iSignal);

	/**
	  @brief A destructor
	*/
	~MainSignalHandler() throw();
private:
	/** Global server context */
	GlobalContext  & oGlobalContext;
};

} // namespace IRIS
#endif // _MAIN_SIGNAL_HANDLER_HPP__
// End.
