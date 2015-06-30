/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _IO_WATCHER_HPP__
#define _IO_WATCHER_HPP__ 1

#include <ev.h>

#include "Types.h"

namespace IRIS
{
// FWD
class EventLoop;

/**
  @class IOWatcher IOWatcher.hpp <IOWatcher.hpp>
  @brief Event watcher
*/
class IOWatcher
{
public:
	/** Error event          */
	static const UINT_32  ERROR;
	/** Read event           */
	static const UINT_32  READ;
	/** Write event          */
	static const UINT_32  WRITE;
	/** Disconnection event  */
	static const UINT_32  REMOVE;

	/**
	  @brief Constructor
	  @param pIEventLoop - event loop object
	*/
	IOWatcher(EventLoop  * pIEventLoop);

	/**
	  @brief Watcher callback
	  @param iSocket - socket to watch
	  @param iREvents - list of events
	*/
	virtual void Callback(const INT_32   iSocket,
	                      const UINT_32  iREvents) = 0;

	/**
	  @brief Start watching for events
	*/
	virtual void StartWatch();

	/**
	  @brief Stop watching for events
	*/
	virtual void StopWatch();

	/**
	  @brief Set watching events
	  @param iSocket - socket to watch
	  @param iEvents - list of events
	*/
	virtual void SetEvents(const INT_32   iSocket,
	                       const UINT_32  iEvents);

	/**
	  @brief Get watching events
	*/
	virtual UINT_32 GetEvents() const;

	/**
	  @brief Virtual destructor
	*/
	virtual ~IOWatcher() throw();
private:
	// Does not exist
	IOWatcher(const IOWatcher  & oRhs);
	IOWatcher& operator=(const IOWatcher  & oRhs);

protected:
	/** Event loop   */
	EventLoop  * pEventLoop;
private:
	/** Indicates that watching started */
	bool         bWatchStarted;
	/** LibEV data                      */
	ev_io        oEvIO;
	/** List of events                  */
	UINT_32      iIOEvents;

	/**
	  @brief LibEV watcher callback
	  @param pLoop - LibEv event loop
	  @param pWatcher - LibEV watcher object
	  @param iREV - list of events
	*/
	static void WatcherCallback(struct ev_loop  * pLoop,
	                            struct ev_io    * pWatcher,
	                            int               iREV);
};

} // namespace IRIS
#endif // _IO_WATCHER_HPP__
// End.
