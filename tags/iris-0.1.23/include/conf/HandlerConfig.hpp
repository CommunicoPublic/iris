/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _HANDLER_CONFIG_HPP__
#define _HANDLER_CONFIG_HPP__ 1

/**
  @file HandlerConfig.hpp
  @brief Iris service handler configuration data
*/

#include "Variant.hpp"

namespace IRIS
{
// FWD
class Logger;

/**
  @class HandlerConfig HandlerConfig.hpp <HandlerConfig.hpp>
  @brief Iris service handler configuration data
*/
struct HandlerConfig
{
	enum State { OK, ERROR, CONFIG_ERROR };

	/** Handler type                                */
	STLW::string    type;
	/** Handler name                                */
	STLW::string    name;
	/** Library name                                */
	STLW::string    library;
	/** Driver name                                 */
	STLW::string    driver;
	/** Number of threads created on startup        */
	UINT_32         start_threads;
	/** Logger object                               */
	STLW::string    logger;
	/** List of configuration parameters            */
	VariantNC       params;

	/**
	  @brief Get configuration from variant data type
	*/
	static State  GetConfig(const VariantNC  & oData,
	                        HandlerConfig    & oConfig,
	                        Logger           & oLogger);
};

} // namespace IRIS
#endif // _HANDLER_CONFIG_HPP__
// End.

