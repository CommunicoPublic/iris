/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _HANDLER_OBJECT_HPP__
#define _HANDLER_OBJECT_HPP__ 1

/**
  @file Handler.hpp
  @brief Base class class for handler
*/

#include "Module.hpp"

namespace IRIS
{
// FWD
class GlobalContext;
class HandlerConfig;
class Logger;

/**
  @class Handler Handler.hpp <Handler.hpp>
  @brief Base class class for handler
*/
class Handler:
  public Module
{
public:

	/**
	  @brief Initialize handler
	  @param oGlobalContext - global execution context
	  @param oHandlerConfig - handler configuration data
	  @param oLogger - logger object
	*/
	virtual INT_32 InitHandler(GlobalContext        & oGlobalContext,
	                           const HandlerConfig  & oHandlerConfig,
	                           Logger               & oLogger) = 0;

	/**
	  @brief Destroy handler
	  @param oGlobalContext - global execution context
	  @param oLogger - logger object
	*/
	virtual INT_32 DestroyHandler(GlobalContext  & oGlobalContext,
	                              Logger         & oLogger) = 0;

	/**
	  @brief A virtual destructor
	*/
	virtual ~Handler() throw() { ;; }
};

} // namespace IRIS
#endif // _HANDLER_OBJECT_HPP__
// End.
