/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _LOGGER_CONTEXT_HPP__
#define _LOGGER_CONTEXT_HPP__ 1

/**
  @file LoggerContext.hpp
  @brief Base class for thread context
*/

// Local includes
#include "Types.h"
#include "STLString.hpp"

namespace IRIS
{

/**
  @class LoggerContext LoggerContext.hpp <LoggerContext.hpp>
  @brief Base class for logger context
*/
class LoggerContext
{
public:
	/**
	  @brief Set parameter
	*/
	virtual void SetParam(const STLW::string  & sParam,
	                      const INT_64          iParam) = 0;

	/**
	  @brief Set parameter
	*/
	virtual void SetParam(const STLW::string  & sParam,
	                      const W_FLOAT         dParam) = 0;

	/**
	  @brief Set parameter
	*/
	virtual void SetParam(const STLW::string  & sParam,
	                      const STLW::string  & sValue) = 0;

	/**
	  @brief Get parameter value for logger
	*/
	virtual void GetParam(const STLW::string  & sParam,
	                            STLW::string        & sValue) const = 0;

	/**
	  @brief Virtual destructor
	*/
	virtual ~LoggerContext() throw();
};

} // namespace IRIS
#endif // _LOGGER_CONTEXT_HPP__
// End.
