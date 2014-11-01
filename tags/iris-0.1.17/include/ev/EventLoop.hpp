/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _EVENT_LOOP_HPP__
#define _EVENT_LOOP_HPP__ 1

#include <ev.h>

namespace IRIS
{
/**
  @class EventLoop EventLoop.hpp <EventLoop.hpp>
  @brief LibEv Event loop
*/
class EventLoop
{
public:
	/**
	  @brief Constructor
	*/
	EventLoop();

	/**
	  @brief Start loop
	*/
	void Loop();

	/**
	  @brief Stop loop
	*/
	void UnLoop();

	/**
	  @brief A destructor
	*/
	~EventLoop() throw();
private:
	friend class EventTimer;
	friend class IOWatcher;
	friend class IOAsyncWatcher;

	// Does bot exist
	EventLoop(const EventLoop  & oRhs);
	EventLoop& operator=(const EventLoop  & oRhs);

	/** LibEv event loop */
	struct ev_loop   * pLoop;
};
} // namespace IRIS
#endif // _EVENT_LOOP_HPP__
// End.
