/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _IO_ASYNC_WATCHER_HPP__
#define _IO_ASYNC_WATCHER_HPP__ 1

#include <ev.h>

#include "Types.h"

namespace IRIS
{
// FWD
class EventLoop;

/**
  @class IOAsyncWatcher IOAsyncWatcher.hpp <IOAsyncWatcher.hpp>
  @brief Event watcher
*/
class IOAsyncWatcher
{
public:
	/**
	  @brief Constructor
	  @param pIEventLoop - event loop object
	*/
	IOAsyncWatcher(EventLoop  * pIEventLoop);

	/**
	  @brief Watcher callback
	  @param iREvents - list of events
	*/
	virtual void Callback(const UINT_32  iREvents) = 0;

	/**
	  @brief Start watching for events
	*/
	void StartWatch();

	/**
	  @brief Stop watching for events
	*/
	void StopWatch();

	/**
	  @brief Send signal
	*/
	void SendSignal();

	/**
	  @brief Virtual destructor
	*/
	virtual ~IOAsyncWatcher() throw();
private:
	// Does not exist
	IOAsyncWatcher(const IOAsyncWatcher  & oRhs);
	IOAsyncWatcher& operator=(const IOAsyncWatcher  & oRhs);

protected:
	/** Event loop   */
	EventLoop  * pEventLoop;
private:
	/** Indicates that watching started */
	bool         bWatchStarted;
	/** LibEV data                      */
	ev_async     oEvAsync;

	/**
	  @brief LibEV watcher callback
	  @param pLoop - LibEv event loop
	  @param pEvAsync - LibEV asyncronous object
	  @param iREV - list of events
	*/
	static void WatcherCallback(struct ev_loop   * pLoop,
	                            struct ev_async  * pEvAsync,
	                            int                iREV);
};

} // namespace IRIS
#endif // _IO_ASYNC_WATCHER_HPP__
// End.
