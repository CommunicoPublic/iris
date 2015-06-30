/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "TimeProfiler.hpp"

#include <errno.h>

namespace IRIS
{

//
// Constructor
//
TimeProfiler::TimeDelta::TimeDelta()
{
	tm_begin.tv_sec = tm_begin.tv_nsec = tm_end.tv_sec = tm_end.tv_nsec = tm_delta.tv_sec = tm_delta.tv_nsec = 0;
}

//
// Constructor
//
TimeProfiler::TimeProfiler() { ;; }

//
// Start point of event
//
INT_32 TimeProfiler::StartEvent(CCHAR_P szEventName)
{
	STLW::string sEventName(szEventName);

	TimeHash::iterator ithData = oTimeHash.find(sEventName);
	// New time point
	if (ithData == oTimeHash.end())
	{
		STLW::pair<TimeHash::iterator, bool> oPair = oTimeHash.insert(STLW::pair<STLW::string, TimeDelta>(sEventName, TimeDelta()));

		TimeDelta & oTimeDelta = oPair.first -> second;
#ifdef linux
		clock_gettime(CLOCK_MONOTONIC_RAW, &oTimeDelta.tm_end);
#elif defined(sun) || defined(_AIX) || defined(__OpenBSD__) || defined(__NetBSD__)
		clock_gettime(CLOCK_REALTIME, &oTimeDelta.tm_end);
#else
		clock_gettime(CLOCK_UPTIME_PRECISE, &oTimeDelta.tm_end);
#endif
		// Delta == 0
		oTimeDelta.tm_begin = oTimeDelta.tm_end;

		oTimeDelta.tm_delta.tv_sec  = 0;
		oTimeDelta.tm_delta.tv_nsec = 0;

		return 0;
	}

	TimeDelta oTimeDelta;
#ifdef linux
	clock_gettime(CLOCK_MONOTONIC_RAW, &oTimeDelta.tm_end);
#elif defined(sun) || defined(_AIX) || defined(__OpenBSD__) || defined(__NetBSD__)
	clock_gettime(CLOCK_REALTIME, &oTimeDelta.tm_end);
#else
	clock_gettime(CLOCK_UPTIME_PRECISE, &oTimeDelta.tm_end);
#endif
	ithData -> second.tm_begin = ithData -> second.tm_end;
	ithData -> second.tm_end   = oTimeDelta.tm_end;

return -1;
}

//
// End point of event
//
W_FLOAT TimeProfiler::EndEvent(CCHAR_P sEventName)
{
	TimeDelta oTimeDelta;
#ifdef linux
	clock_gettime(CLOCK_MONOTONIC_RAW, &oTimeDelta.tm_end);
#elif defined(sun) || defined(_AIX) || defined(__OpenBSD__) || defined(__NetBSD__)
	clock_gettime(CLOCK_REALTIME, &oTimeDelta.tm_end);
#else
	clock_gettime(CLOCK_UPTIME_PRECISE, &oTimeDelta.tm_end);
#endif
	TimeHash::iterator ithData = oTimeHash.find(sEventName);
	if (ithData != oTimeHash.end())
	{
		const INT_64 lTDSec  = oTimeDelta.tm_end.tv_sec  - ithData -> second.tm_end.tv_sec;
		const INT_64 lTDNSec = oTimeDelta.tm_end.tv_nsec - ithData -> second.tm_end.tv_nsec;

		ithData -> second.tm_delta.tv_sec  += lTDSec;
		ithData -> second.tm_delta.tv_nsec += lTDNSec;

		ithData -> second.tm_begin = ithData -> second.tm_end;
		ithData -> second.tm_end   = oTimeDelta.tm_end;

		return (lTDSec + lTDNSec / 1000000000.0);
	}

return -1.0;
}

//
// Get total time of event execution
//
W_FLOAT TimeProfiler::CheckEvent(CCHAR_P sEventName)
{
	TimeHash::iterator ithData = oTimeHash.find(sEventName);
	if (ithData != oTimeHash.end())
	{
		return (ithData -> second.tm_delta.tv_sec + ithData -> second.tm_delta.tv_nsec / 1000000000.0);
	}

return -1.0;
}

//
// Delete event from event list
//
void TimeProfiler::DeleteEvent(CCHAR_P sEventName)
{
	oTimeHash.erase(sEventName);
}

//
// Clear event list
//
void TimeProfiler::Clear()
{
	if (oTimeHash.size() != 0) { oTimeHash.clear(); }
}

//
// A destructor
//
TimeProfiler::~TimeProfiler() throw() { ;; }

} // namespace IRIS
// End.
