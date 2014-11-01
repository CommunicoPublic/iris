/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _LOG_PRIORITIES_HPP__
#define _LOG_PRIORITIES_HPP__ 1

/**
  @file LogPriorities.hpp
  @brief Priority codes for logger subsystem
*/

#include "Types.h"

namespace IRIS
{

/**
  @namespace IRIS::LogPriority
  @brief List of log priorities
*/
namespace LogPriority
{
	enum LogPriorities { LOG_EMERG   = 0, // System is unusable
	                     LOG_ALERT   = 1, // Action must be taken immediately
	                     LOG_CRIT    = 2, // Critical conditions
	                     LOG_ERROR   = 3, // Error conditions
	                     LOG_WARNING = 4, // Warning conditions
	                     LOG_NOTICE  = 5, // Normal but significant condition
	                     LOG_INFO    = 6, // Informational
	                     LOG_DEBUG   = 7  // Debug-level messages
	                    };

	CCHAR_P GetPrintableState(const LogPriorities  & ePriority);
}

} // namespace IRIS
#endif // _LOG_PRIORITIES_HPP__
// End.
