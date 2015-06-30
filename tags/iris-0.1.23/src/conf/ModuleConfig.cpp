/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "ConfigHelper.hpp"
#include "ModuleConfig.hpp"
#include "Logger.hpp"

namespace IRIS
{

//
// Get configuration from variant data type
//
ModuleConfig::State ModuleConfig::GetConfig(const VariantNC  & oData,
                                            ModuleConfig     & oModuleConfig,
                                            Logger           & oLogger)
{
	oModuleConfig.driver.assign(oData["Driver"]);
	if (oModuleConfig.driver.empty())
	{
		oLogger.Emerg("Parameter `%s/%s/Driver` not set", oModuleConfig.type.c_str(), oModuleConfig.name.c_str());
		return CONFIG_ERROR;
	}
	oModuleConfig.library.assign(oData["Library"]);
	if (oModuleConfig.library.empty())
	{
		oLogger.Emerg("Parameter `%s/%s/Library` not set", oModuleConfig.type.c_str(), oModuleConfig.name.c_str());
		return CONFIG_ERROR;
	}

	// Store parameters
	oModuleConfig.params = oData;
return OK;
}

} // namespace IRIS
// End.
