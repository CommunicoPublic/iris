/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _LOGGER_OBJECT_HPP__
#define _LOGGER_OBJECT_HPP__ 1

/**
  @file LoggerObject.hpp
  @brief Parametrized logger
*/
#include "Module.hpp"

namespace IRIS
{
// FWD
class LoggerContext;

/**
  @class LoggerObject LoggerObject.hpp <LoggerObject.hpp>
  @brief Parametrized logger
*/
class LoggerObject:
  public Module
{
public:
	/**
	  @brief Get error log
	*/
	virtual Logger * GetErrorLog(const LoggerContext * pLoggerContext) = 0;

	/**
	  Get custom log
	*/
	virtual Logger * GetCustomLog(const LoggerContext * pLoggerContext) = 0;

	/**
	  @brief A virtual destructor
	*/
	virtual ~LoggerObject() throw() { ;; }
};

} // namespace IRIS
#endif // _LOGGER_OBJECT_HPP__
// End.
