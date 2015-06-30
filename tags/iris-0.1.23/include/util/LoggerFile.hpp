/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _LOGGER_FILE_HPP__
#define _LOGGER_FILE_HPP__ 1

/**
  @file LoggerFile.hpp
  @brief Logger subsystem; write messages to file
*/

#include "Logger.hpp"

#include <stdio.h>

namespace IRIS
{

/**
  @class LoggerFile LoggerFile.hpp <LoggerFile.hpp>
  @brief Logger subsystem; write messages to file
*/
class LoggerFile:
  public Logger
{
public:
	/**
	  @brief A constructor
	  @param oIF - log file descriptor
	*/
	LoggerFile(FILE * oIF);

	/**
	  @brief Write message to log file
	  @param ePriority - priority level
	  @param szString - message to store in file
	  @param iStringLen - string length
	  @return 0- if success, -1 - if any error occured
	*/
	Logger::State WriteLog(const LogPriority::LogPriorities  ePriority,
	                       CCHAR_P                           szString,
	                       const UINT_32                     iStringLen);

	/**
	  @brief A destructor
	*/
	~LoggerFile() throw();
private:
	/** Log file handle */
	FILE     * F;
};

} // namespace IRIS
#endif // _LOGGER_FILE_HPP__
// End.
