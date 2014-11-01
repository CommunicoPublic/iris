/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MAIN_LOOP_STOP_WATCHER_HPP__
#define _MAIN_LOOP_STOP_WATCHER_HPP__ 1

/**
  @file MainLoopStopWatcher.hpp
  @brief Watcher for asyncronous stop server event
*/

#include "IOAsyncWatcher.hpp"

namespace IRIS
{

/**
  @class MainLoopStopWatcher MainLoopStopWatcher.hpp <MainLoopStopWatcher.hpp>
  @brief Watcher for asyncronous stop server event
*/
class MainLoopStopWatcher:
  public IOAsyncWatcher
{
public:
	/**
	  @brief Constructor
	  @param pILoop - event loop
	*/
	MainLoopStopWatcher(EventLoop  * pILoop);

	/**
	  @brief Watcher callback
	  @param iREvents - list of events
	*/
	void Callback(const UINT_32  iREvents);

	/**
	  @brief A destructor
	*/
	~MainLoopStopWatcher() throw();
private:
	// Does not exist
	MainLoopStopWatcher(const MainLoopStopWatcher  & oRhs);
	MainLoopStopWatcher& operator=(const MainLoopStopWatcher  & oRhs);

	/** Event loop */
	EventLoop  * pLoop;
};

} // namespace IRIS
#endif // _MAIN_LOOP_STOP_WATCHER_HPP__
// End.
