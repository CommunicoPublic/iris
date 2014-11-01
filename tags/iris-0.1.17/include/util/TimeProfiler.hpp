/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _TIME_PROFILER_HPP__
#define _TIME_PROFILER_HPP__ 1

/**
  @file TimeProfiler.hpp
  @brief Time profiler for measueraments of execution time
*/

// C Includes
#include <sys/time.h>
#include <string.h>
// Linux stupidity
#ifdef linux
  #include <time.h>
#endif

// Local includes
#include "Types.h"
#include "STLMap.hpp"
#include "STLString.hpp"

namespace IRIS
{

/**
  @class TimeProfiler TimeProfiler.hpp <TimeProfiler.hpp>
  @brief Time profiler for meausurements of execution time
*/
class TimeProfiler
{
public:
	/**
	  @brief Constructor
	*/
	TimeProfiler();

	/**
	  @brief Start point of event
	  @param sEventName - name of event
	  @return 0 if this event is new event and -1 if this event exist
	*/
	INT_32 StartEvent(CCHAR_P sEventName);

	/**
	  @brief End point of event (overloaded method)
	  @param sEventName - name of event
	  @return full time of event execution in seconds
	*/
	W_FLOAT EndEvent(CCHAR_P sEventName);

	/**
	  @brief Get total time of event execution
	  @param sEventName - name of event
	  @return full time of event execution in milliseconds
	*/
	W_FLOAT CheckEvent(CCHAR_P sEventName);

	/**
	  @brief Delete event from event list
	  @param sEventName - name of event
	*/
	void DeleteEvent(CCHAR_P sEventName);

	/**
	  @brief Clear event list
	*/
	void Clear();

	/**
	  @brief A destructor
	*/
	~TimeProfiler() throw();

private:
	/**
	  @struct TimeDelta TimeProfiler.hpp <TimeProfiler.hpp>
	  @brief time interval
	*/
	struct TimeDelta
	{
		/** Start time of event */
		struct timespec   tm_begin;
		/** End time of event   */
		struct timespec   tm_end;
		/** Duration of event   */
		struct timespec   tm_delta;

		/**
		  @brief Constructor
		*/
		TimeDelta();
	};

	/**
	  @typedef Hash<TokenHash_t, TimeDelta> TimeHash
	*/
	typedef STLW::map<STLW::string, TimeDelta> TimeHash;

	/** Index hash          */
	TimeHash       oTimeHash;
};

} // namespace IRIS
#endif // _TIME_PROFILER_HPP__
// End.
