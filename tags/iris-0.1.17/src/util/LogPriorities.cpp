/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "LogPriorities.hpp"

namespace IRIS
{

namespace LogPriority
{
	CCHAR_P GetPrintableState(const LogPriorities & ePriority)
	{
		switch (ePriority)
		{
			case LOG_EMERG:    return "emerg";
			case LOG_ALERT:    return "alert";
			case LOG_CRIT:     return "crit";
			case LOG_ERROR:    return "error";
			case LOG_WARNING:  return "warning";
			case LOG_NOTICE:   return "notice";
			case LOG_INFO:     return "info";
			case LOG_DEBUG:    return "debug";
			default:
				return "unknown";
		}
	}
}

} // namespace IRIS
// End.
