/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "Logger.hpp"

#include <stdio.h>
#include <stdlib.h>

namespace IRIS
{

const INT_32 Logger::C_START_MESSAGE_LEN = 512;

//
// Constructor
//
Logger::Logger(const LogPriority::LogPriorities  eIBasePriority): eBasePriority(eIBasePriority) { ;; }

//
// Change base priority
//
void Logger::SetPriority(const LogPriority::LogPriorities  eNewPriority) { eBasePriority = eNewPriority; }

//
// Log message with formatted parameters
//
Logger::State Logger::LogMessage(const LogPriority::LogPriorities  ePriority,
                                 CCHAR_P                           szFormat, ...)
{
	va_list aArgList;
	va_start(aArgList, szFormat);
	State eRC = LogMessage(ePriority, szFormat, aArgList);
	va_end(aArgList);

return eRC;
}

//
// Log message with formatted parameters
//
Logger::State Logger::LogMessage(const LogPriority::LogPriorities  ePriority,
                                 CCHAR_P                           szFormat,
                                 va_list                           aArgList)
{
	// Message not logged, incorrect priority level
	if (ePriority > eBasePriority) { return LOGIC_ERROR; }

	{
		va_list vaTMP;
		va_copy(vaTMP, aArgList);

		CHAR_8 szStaticBuffer[C_START_MESSAGE_LEN];
		// Format value
		const INT_32 iFmtLen = vsnprintf(szStaticBuffer, C_START_MESSAGE_LEN - 1, szFormat, vaTMP);
		if (iFmtLen < C_START_MESSAGE_LEN)
		{
			// Write to logger
			return WriteLog(ePriority, szStaticBuffer, iFmtLen);
		}
	}

	INT_32 iResultLen = C_START_MESSAGE_LEN;
	CHAR_P szResult   = NULL;
	for(;;)
	{
		va_list vaTMP;
		va_copy(vaTMP, aArgList);

		// TODO: Improve performance
		iResultLen <<= 1;
		szResult = (CHAR_P)malloc(iResultLen);
		// Format value
		const INT_32 iFmtLen = vsnprintf(szResult, iResultLen - 1, szFormat, vaTMP);
		if (iFmtLen < iResultLen)
		{
			// Write to logger
			State eRC = WriteLog(ePriority, szResult, iFmtLen);
			free(szResult);
			return eRC;
		}

		free(szResult);
	}
// Never happened
return OK;
}

//
// Log message with formatted parameters and LogPriority::LOG_EMERG priority
//
Logger::State Logger::Emerg(CCHAR_P szFormat, ...) { va_list aTMP; va_start(aTMP, szFormat); Logger::State eRC = LogMessage(LogPriority::LOG_EMERG, szFormat, aTMP); va_end(aTMP); return eRC; }

//
// Log message with formatted parameters and LogPriority::LOG_ALERT priority
//
Logger::State Logger::Alert(CCHAR_P szFormat, ...) { va_list aTMP; va_start(aTMP, szFormat); Logger::State eRC = LogMessage(LogPriority::LOG_ALERT, szFormat, aTMP); va_end(aTMP); return eRC; }

//
// Log message with formatted parameters and LogPriority::LOG_CRIT priority
//
Logger::State Logger::Crit(CCHAR_P szFormat, ...) { va_list aTMP; va_start(aTMP, szFormat); Logger::State eRC = LogMessage(LogPriority::LOG_CRIT, szFormat, aTMP); va_end(aTMP); return eRC; }

//
// Log message with formatted parameters and LogPriority::LOG_ERR priority
//
Logger::State Logger::Err(CCHAR_P szFormat, ...) { va_list aTMP; va_start(aTMP, szFormat); Logger::State eRC = LogMessage(LogPriority::LOG_ERROR, szFormat, aTMP); va_end(aTMP); return eRC; }

//
// Log message with formatted parameters and LogPriority::LOG_ERR priority
//
Logger::State Logger::Error(CCHAR_P szFormat, ...) { va_list aTMP; va_start(aTMP, szFormat); Logger::State eRC = LogMessage(LogPriority::LOG_ERROR, szFormat, aTMP); va_end(aTMP); return eRC; }

//
// Log message with formatted parameters and LogPriority::LOG_WARNING priority
//
Logger::State Logger::Warn(CCHAR_P szFormat, ...) { va_list aTMP; va_start(aTMP, szFormat); Logger::State eRC = LogMessage(LogPriority::LOG_WARNING, szFormat, aTMP); va_end(aTMP); return eRC; }

//
// Log message with formatted parameters and LogPriority::LOG_WARNING priority
//
Logger::State Logger::Warning(CCHAR_P szFormat, ...) { va_list aTMP; va_start(aTMP, szFormat); Logger::State eRC = LogMessage(LogPriority::LOG_WARNING, szFormat, aTMP); va_end(aTMP); return eRC; }

//
// Log message with formatted parameters and LogPriority::LOG_NOTICE priority
//
Logger::State Logger::Notice(CCHAR_P szFormat, ...) { va_list aTMP; va_start(aTMP, szFormat); Logger::State eRC = LogMessage(LogPriority::LOG_NOTICE, szFormat, aTMP); va_end(aTMP); return eRC; }

//
// Log message with formatted parameters and LogPriority::LOG_INFO priority
//
Logger::State Logger::Info(CCHAR_P szFormat, ...) { va_list aTMP; va_start(aTMP, szFormat); Logger::State eRC = LogMessage(LogPriority::LOG_INFO, szFormat, aTMP); va_end(aTMP); return eRC; }

//
// Log message with formatted parameters and LogPriority::LOG_DEBUG priority
//
Logger::State Logger::Debug(CCHAR_P szFormat, ...) { va_list aTMP; va_start(aTMP, szFormat); Logger::State eRC = LogMessage(LogPriority::LOG_DEBUG, szFormat, aTMP); va_end(aTMP); return eRC; }

//
// A destructor
//
Logger::~Logger() throw() { ;; }

} // namespace IRIS
// End.
