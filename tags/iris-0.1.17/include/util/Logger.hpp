/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _LOGGER_HPP__
#define _LOGGER_HPP__ 1

/**
  @file Logger.hpp
  @brief Logger subsystem, base class
*/
#include "LogPriorities.hpp"

#include <stdarg.h>

namespace IRIS
{

/**
  @class Logger Logger.hpp <Logger.hpp>
  @brief Logger subsystem
*/
class Logger
{
public:
	enum State { OK, ERROR, LOGIC_ERROR };

	static const INT_32 C_START_MESSAGE_LEN;

	/**
	  @brief Constructor
	  @param eIBasePriority - base priority
	*/
	Logger(const LogPriority::LogPriorities  eIBasePriority = LogPriority::LOG_WARNING);

	/**
	  @brief Change base priority
	  @param eNewPriority - new base priority
	*/
	void SetPriority(const LogPriority::LogPriorities  eNewPriority);

	/**
	  @brief Write message to log file
	  @param ePriority - priority level
	  @param szString - message to store in file
	  @param iStringLen - string length
	  @return 0 - if success, -1 - otherwise
	*/
	virtual State WriteLog(const LogPriority::LogPriorities  ePriority,
	                       CCHAR_P                           szString,
	                       const UINT_32                     iStringLen) = 0;

	/**
	  @brief log message with formatted parameters
	  @param ePriority - priority level
	  @param szFormat - output format
	  @return 0 - if success, -1 - otherwise
	*/
	virtual State LogMessage(const LogPriority::LogPriorities  ePriority,
	                         CCHAR_P                           szFormat, ...);

	/**
	  @brief log message with formatted parameters
	  @param ePriority - priority level
	  @param szFormat - output format
	  @param aArgList - list of arguments
	  @return 0 - if success, -1 - otherwise
	*/
	virtual State LogMessage(const LogPriority::LogPriorities  ePriority,
	                         CCHAR_P                           szFormat,
	                         va_list                           aArgList);

	/**
	  @brief log message with formatted parameters and LOG_EMERG priority
	  @param szFormat - output format
	  @return 0 - if success, -1 - otherwise
	*/
	State Emerg(CCHAR_P szFormat, ...);

	/**
	  @brief log message with formatted parameters and LOG_ALERT priority
	  @param szFormat - output format
	  @return 0 - if success, -1 - otherwise
	*/
	State Alert(CCHAR_P szFormat, ...);

	/**
	  @brief log message with formatted parameters and LOG_CRIT priority
	  @param szFormat - output format
	  @return 0 - if success, -1 - otherwise
	*/
	State Crit(CCHAR_P szFormat, ...);

	/**
	  @brief log message with formatted parameters and LOG_ERR priority
	  @param szFormat - output format
	  @return 0 - if success, -1 - otherwise
	*/
	State Err(CCHAR_P szFormat, ...);

	/**
	  @brief log message with formatted parameters and LOG_ERR priority
	  @param szFormat - output format
	  @return 0 - if success, -1 - otherwise
	*/
	State Error(CCHAR_P szFormat, ...);

	/**
	  @brief log message with formatted parameters and LOG_WARNING priority
	  @param szFormat - output format
	  @return 0 - if success, -1 - otherwise
	*/
	State Warn(CCHAR_P szFormat, ...);

	/**
	  @brief log message with formatted parameters and LOG_WARNING priority
	  @param szFormat - output format
	  @return 0 - if success, -1 - otherwise
	*/
	State Warning(CCHAR_P szFormat, ...);

	/**
	  @brief log message with formatted parameters and LOG_NOTICE priority
	  @param szFormat - output format
	  @return 0 - if success, -1 - otherwise
	*/
	State Notice(CCHAR_P szFormat, ...);

	/**
	  @brief log message with formatted parameters and LOG_INFO priority
	  @param szFormat - output format
	  @return 0 - if success, -1 - otherwise
	*/
	State Info(CCHAR_P szFormat, ...);

	/**
	  @brief log message with formatted parameters and LOG_DEBUG priority
	  @param szFormat - output format
	  @return 0 - if success, -1 - otherwise
	*/
	State Debug(CCHAR_P szFormat, ...);

	/**
	  @brief A destructor
	*/
	virtual ~Logger() throw();

protected:
	/** Base priority   */
	LogPriority::LogPriorities  eBasePriority;
};

} // namespace IRIS
#endif // _LOGGER_HPP__
// End.
