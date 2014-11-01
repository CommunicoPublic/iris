/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MODULE_HPP__
#define _MODULE_HPP__ 1

/**
  @file Module.hpp
  @brief Base class class for loadable modules
*/
#include "Object.hpp"
#include "Variant.hpp"

namespace IRIS
{
// FWD
class GlobalContext;
class Logger;
class SignalHandler;

/**
  @class Module Module.hpp <Module.hpp>
  @brief Base class class for loadable modules
*/
class Module:
  public Object
{
public:

	/**
	  @brief Initialize module
	  @param oGlobalContext - global execution context
	  @param oConfig - configuration data
	  @param oSigHandler - signal handler
	  @param oLogger - logger object
	*/
	virtual INT_32 InitModule(GlobalContext    & oGlobalContext,
	                          const VariantNC  & oConfig,
	                          SignalHandler    & oSigHandler,
	                          Logger           & oLogger) = 0;

	/**
	  @brief Destroy module
	  @param oGlobalContext - global execution context
	  @param oSigHandler - signal handler
	  @param oLogger - logger object
	*/
	virtual INT_32 DestroyModule(GlobalContext  & oGlobalContext,
	                             SignalHandler  & oSigHandler,
	                             Logger         & oLogger) = 0;

	/**
	  @brief A virtual destructor
	*/
	virtual ~Module() throw() { ;; }
};

} // namespace IRIS
#endif // _MODULE_HPP__
// End.
