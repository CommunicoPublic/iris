/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MODULE_CONFIG_HPP__
#define _MODULE_CONFIG_HPP__ 1

/**
  @file ModuleConfig.hpp
  @brief Iris service handler configuration data
*/

#include "Variant.hpp"

namespace IRIS
{
// FWD
class Logger;

/**
  @class ModuleConfig ModuleConfig.hpp <ModuleConfig.hpp>
  @brief Iris loadable module configuration data
*/
struct ModuleConfig
{
	enum State { OK, ERROR, CONFIG_ERROR };

	/** Module type                                */
	STLW::string    type;
	/** Module name                                */
	STLW::string    name;
	/** Library name                                */
	STLW::string    library;
	/** Driver name                                 */
	STLW::string    driver;
	/** List of configuration parameters            */
	VariantNC       params;

	/**
	  @brief Get configuration from variant data type
	*/
	static State  GetConfig(const VariantNC  & oData,
	                        ModuleConfig     & oConfig,
	                        Logger           & oLogger);
};

} // namespace IRIS
#endif // _MODULE_CONFIG_HPP__
// End.

