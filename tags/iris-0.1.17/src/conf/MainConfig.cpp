/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

#include "MainConfig.hpp"
#include "ConfigHelper.hpp"
#include "FileUtils.hpp"
#include "Logger.hpp"
#include "ModuleConfig.hpp"
#include "SystemVars.hpp"
#include "UnicodeUtils.hpp"

#include <unistd.h>

namespace IRIS
{
template <typename T>MainConfig::State GetDriverConfig(const VariantNC     & oData,
                                                       const STLW::string  & sBranchName,
                                                       STLW::vector<T>     & oDriverConfig,
                                                       Logger              & oLogger)
{
	VariantNC::ConstIterator itoData = oData.Find(sBranchName);
	if (itoData == oData.End())
	{
		oLogger.Emerg("Can't find branch `%s`, Iris need at least one", sBranchName.c_str());
		return MainConfig::CONFIG_ERROR;
	}

	const VariantNC  & oConfig = itoData -> second;
	if (oConfig.Type() != VariantNC::MAP)
	{
		oLogger.Emerg("Invalid type of branch `%s`", sBranchName.c_str());
		return MainConfig::CONFIG_ERROR;
	}

	VariantNC::ConstIterator itoConfig = oConfig.Begin();
	while (itoConfig != oConfig.End())
	{
		T oTConfig;
		oTConfig.type = sBranchName;
		oTConfig.name = itoConfig -> first;
		const typename T::State oRC = T::GetConfig(itoConfig -> second, oTConfig, oLogger);
		if (oRC != T::OK) { return MainConfig::CONFIG_ERROR; }
		oDriverConfig.push_back(oTConfig);
		++itoConfig;
	}

return MainConfig::OK;
}

//
// Get configuration from variant data type
//
MainConfig::State MainConfig::GetConfig(const VariantNC  & oData,
                                        MainConfig       & oMainConfig,
                                        Logger           & oLogger)
{
	// Get Main configuration branch
	VariantNC::ConstIterator itModule = oData.Find("module");
	if (itModule == oData.End())
	{
		oLogger.Emerg("Can't find branch `module`");
		return CONFIG_ERROR;
	}

	VariantNC::ConstIterator itMain = itModule -> second.Find("Main");
	if (itMain == itModule -> second.End())
	{
		oLogger.Emerg("Can't find branch `module/Main`");
		return CONFIG_ERROR;
	}

	// Main configuration branch
	const VariantNC  & oMain = itMain -> second;

	// User and group
	ConfigHelper::State oConfRC = ConfigHelper::ParseUserGroup(oMain["UserGroup"],
	                                                           oMainConfig.user,
	                                                           oMainConfig.uid,
	                                                           oMainConfig.group,
	                                                           oMainConfig.gid);
	switch(oConfRC)
	{
		case ConfigHelper::NOT_NEED:
			oLogger.Warn("Parameter `module/Main/UserGroup` is useful only if Iris starting from root user");
			break;

		case ConfigHelper::CONFIG_ERROR:
			oLogger.Emerg("Parameter `module/Main/UserGroup` should be in format `UserGroup  user [group]`");
			return CONFIG_ERROR;

		case ConfigHelper::NO_SUCH_USER:
			oLogger.Emerg("No such user: `%s`!", oMainConfig.user.c_str());
			return CONFIG_ERROR;

		case ConfigHelper::NO_SUCH_GROUP:
			oLogger.Emerg("No such group: `%s`!", oMainConfig.group.c_str());
			return CONFIG_ERROR;

		default:
			;;
	}

	// Get include dirs
	ConfigHelper::GetDirList(oMain["IncludeDirs"], oMainConfig.include_dirs);

	// Get libexec dirs
	ConfigHelper::GetDirList(oMain["LibexecDirs"], oMainConfig.libexec_dirs);
	if (oMainConfig.libexec_dirs.empty())
	{
		oLogger.Emerg("Configuration is broken: parameter Module/Main/LibexecDirs not set");
		return CONFIG_ERROR;
	}

	// Main logger subsystem
	oMainConfig.logger_type.assign(oMain["Logger"]);
	if (oMainConfig.logger_type.empty())
	{
		oLogger.Emerg("Configuration is broken: parameter Module/Main/Logger not set");
		return CONFIG_ERROR;
	}

	oMainConfig.bind_network_threads_to_cpu = false;
	const STLW::string sNetworkThreads = oMain["NetworkThreads"];
	if (Unicode::CompareIgnoreCase(sNetworkThreads.data(), sNetworkThreads.size(), "CPU", sizeof("CPU") - 1) == 0)
	{
		oMainConfig.network_threads = 0;
#ifdef _SC_NPROCESSORS_ONLN
		oMainConfig.network_threads = sysconf(_SC_NPROCESSORS_ONLN);
		oLogger.Info("Found %d CPU(s)", oMainConfig.network_threads);
		oMainConfig.bind_network_threads_to_cpu = true;
#endif
		//Check number of network threads
		if (oMainConfig.network_threads == 0)
		{
			oLogger.Emerg("Configuration is broken: Can't get number of CPU(s), please specify parameter Module/Main/NetworkThreads manually");
			return CONFIG_ERROR;
		}
	}
	else
	{
		oMainConfig.network_threads = oMain["NetworkThreads"];
		//Check number of network threads
		if (oMainConfig.network_threads == 0)
		{
			oLogger.Emerg("Configuration is broken: parameter Module/Main/NetworkThreads not set");
			return CONFIG_ERROR;
		}
	}

	// Pid file
	oMainConfig.pid_file  = oMain["PidFile"].Str();
	if (oMainConfig.pid_file.empty())
	{
		oLogger.Emerg("Configuration is broken: parameter Module/Main/PidFile not set");
		return CONFIG_ERROR;
	}

	// Process priority
	oMainConfig.priority  = oMain["Priority"];
	// Max number of opened files
	oMainConfig.file_cache_size = oMain["LRUFileCacheSize"];
	if (oMainConfig.file_cache_size < MIN_FILE_CACHE_SIZE)
	{
		oMainConfig.file_cache_size = MIN_FILE_CACHE_SIZE;
		oLogger.Warn("Parameter `Module/Main/LRUFileCacheSize` not found, falling back to default `%d`", oMainConfig.file_cache_size);
	}
	// Host name
	oMainConfig.hostname.assign(oMain["Hostname"]);
	if (oMainConfig.hostname.empty())
	{
		oMainConfig.hostname = SystemVars::GetHostname();
		oLogger.Warn("Parameter `Module/Main/Hostname` not found, falling back to default `%s`", oMainConfig.hostname.c_str());
	}

	VariantNC oModules = oData;
	oModules["Module"].Erase("Main");

	// Modules
	State oRC = GetDriverConfig<ModuleConfig>(oModules, "Module", oMainConfig.modules, oLogger);
	if (oRC != OK) { return oRC; }

	// Loggers
	oRC = GetDriverConfig<ModuleConfig>(oData, "Logger", oMainConfig.loggers, oLogger);
	if (oRC != OK) { return oRC; }

	// Module loading order
	oMainConfig.load_order = oMain["ModuleLoadOrder"];
	if (oMainConfig.load_order.empty() || oMainConfig.load_order[0].empty())
	{
		oLogger.Warn("Module/Main/ModuleLoadOrder not found, no additional modules will be load");
	}
	else
	{
		VariantNC oDeniedNames;
		oDeniedNames["Handler"] = 1;
		oDeniedNames["Logger"]  = 1;
		oDeniedNames["Module"]  = 1;
		oDeniedNames["Service"] = 1;

		STLW::vector<STLW::string>::iterator itvLoadOrder = oMainConfig.load_order.begin();
		while(itvLoadOrder != oMainConfig.load_order.end())
		{
			if (!oDeniedNames.Exists(*itvLoadOrder) && !itvLoadOrder -> empty())
			{
				// Handlers
				oRC = GetDriverConfig<ModuleConfig>(oData, *itvLoadOrder, oMainConfig.modules, oLogger);
				if (oRC != OK) { return oRC; }
			}
			++itvLoadOrder;
		}
	}

	// List of active services
	oMainConfig.load_services = oMain["Services"];
	if (oMainConfig.load_services.empty() || oMainConfig.load_services[0].empty())
	{
		oLogger.Warn("Module/Main/Services not found, no available network services");
		return CONFIG_ERROR;
	}

	// Load only specified services
	VariantNC oServices;
	STLW::vector<STLW::string>::const_iterator itService = oMainConfig.load_services.begin();
	while(itService != oMainConfig.load_services.end())
	{
		oServices[*itService] = 1;
		++itService;
	}

	// Handlers
	oRC = GetDriverConfig<HandlerConfig>(oData, "Handler", oMainConfig.handlers, oLogger);
	if (oRC != OK) { return oRC; }

	// Services
	VariantNC::ConstIterator itoData = oData.Find("Service");
	if (itoData == oData.End())
	{
		oLogger.Emerg("Can't find branch `Service`");
		return CONFIG_ERROR;
	}
	const VariantNC  & oService = itoData -> second;
	if (oService.Type() != VariantNC::MAP)
	{
		oLogger.Emerg("Invalid type of branch `Service`");
		return MainConfig::CONFIG_ERROR;
	}

	VariantNC::ConstIterator itoService = oService.Begin();
	while (itoService != oService.End())
	{
		if (!oServices.Exists(itoService -> first))
		{
			oLogger.Warn("Service `%s` disabled in configuration", itoService -> first.c_str());
		}
		else
		{
			oLogger.Info("Loading configuration for service `%s`", itoService -> first.c_str());
			ServiceConfig  oServiceConfig;
			oServiceConfig.name = itoService -> first;

			const ServiceConfig::State oRC = ServiceConfig::GetConfig(itoService -> second, oServiceConfig, oLogger);
			if (oRC != ServiceConfig::OK) { return MainConfig::CONFIG_ERROR; }

			oMainConfig.services.push_back(oServiceConfig);
		}
		++itoService;
	}

	if (oMainConfig.services.empty())
	{
		oLogger.Emerg("No network services found, can't continue");
		return MainConfig::CONFIG_ERROR;
	}

	oMainConfig.raw_config = oData;

return OK;
}

} // namespace IRIS
// End.
