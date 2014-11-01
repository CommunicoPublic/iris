/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MAIN_LOOP_SIGNAL_HANDLER_HPP__
#define _MAIN_LOOP_SIGNAL_HANDLER_HPP__ 1

/**
  @file MainLoopSignalHandler.hpp
  @brief Main loop signal handler
*/

// Local includes
#include "SignalHandler.hpp"

namespace IRIS
{
// FWD
struct MainLoopContext;
class AcceptLoopContext;
/**
  @class MainLoopSignalHandler MainLoopSignalHandler.hpp <MainLoopSignalHandler.hpp>
  @brief Main loop signal handler
*/
class MainLoopSignalHandler:
  public UserSignalHandler
{
public:
	/**
	  @brief Constructor
	  @param oIMainContext - main thread context
	  @param vIContextes - list of accept thread contextes
	*/
	MainLoopSignalHandler(MainLoopContext                    & oIMainContext,
	                      STLW::vector<AcceptLoopContext *>  & vIContextes);

	/**
	  @brief A signal handler
	  @param iSignal - signal number
	*/
	void Handler(INT_32 iSignal);

	/**
	  @brief A destructor
	*/
	~MainLoopSignalHandler() throw();
private:
	/** Main Context                       */
	MainLoopContext                    & oMainContext;
	/** List of contextes                  */
	STLW::vector<AcceptLoopContext *>  & vContextes;
	/** Multiple execution prteventor      */
	bool                                 bSignalCatched;
};

} // namespace IRIS
#endif // _MAIN_LOOP_SIGNAL_HANDLER_HPP__
// End.
