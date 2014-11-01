/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "ConfigHelper.hpp"
#include "HandlerConfig.hpp"
#include "Logger.hpp"

namespace IRIS
{

//
// Get configuration from variant data type
//
HandlerConfig::State HandlerConfig::GetConfig(const VariantNC  & oData,
                                              HandlerConfig    & oHandlerConfig,
                                              Logger           & oLogger)
{
	oHandlerConfig.driver.assign(oData["Driver"]);
	if (oHandlerConfig.driver.empty())
	{
		oLogger.Emerg("Parameter `%s/%s/Driver` not set", oHandlerConfig.type.c_str(), oHandlerConfig.name.c_str());
		return CONFIG_ERROR;
	}
	oHandlerConfig.library.assign(oData["Library"]);
	if (oHandlerConfig.library.empty())
	{
		oLogger.Emerg("Parameter `%s/%s/Library` not set", oHandlerConfig.type.c_str(), oHandlerConfig.name.c_str());
		return CONFIG_ERROR;
	}

	if (ConfigHelper::ParseBool(oData["DisableThreads"], false))
	{
		oHandlerConfig.start_threads = 0;
		oLogger.Notice("Ignoring `%s/%s/SStartThreads` branch", oHandlerConfig.type.c_str(), oHandlerConfig.name.c_str());
	}
	else
	{
		// Number of threads created on startup
		oHandlerConfig.start_threads = oData["StartThreads"];
		if (oHandlerConfig.start_threads == 0)
		{
			oLogger.Emerg("Parameter `%s/%s/StartThreads` not set", oHandlerConfig.type.c_str(), oHandlerConfig.name.c_str());
			return CONFIG_ERROR;
		}
	}
	// Logger name
	oHandlerConfig.logger.assign(oData["Logger"]);

	// Store parameters
	oHandlerConfig.params = oData;
return OK;
}

} // namespace IRIS
// End.
