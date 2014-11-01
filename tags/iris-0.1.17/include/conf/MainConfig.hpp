/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MAIN_CONFIG_HPP__
#define _MAIN_CONFIG_HPP__ 1

/**
  @file MainConfig.hpp
  @brief Iris Main configuration data
*/

#include "HandlerConfig.hpp"
#include "ModuleConfig.hpp"
#include "ServiceConfig.hpp"

#ifndef MIN_FILE_CACHE_SIZE
#define MIN_FILE_CACHE_SIZE  10240
#endif // MIN_FILE_CACHE_SIZE

namespace IRIS
{

/**
  @class MainConfig MainConfig.hpp <MainConfig.hpp>
  @brief Iris Main configuration data
*/
struct MainConfig
{
	enum State { OK, ERROR, CONFIG_ERROR };
	enum DebugLevel { NO_DEBUG, DEBUG_MIN, DEBUG_STD, DEBUG_MAX };

	/** List of directories
	    with includes                   */
	STLW::vector<STLW::string>   include_dirs;
	/** List of directories
	    with loadable modules & plugins */
	STLW::vector<STLW::string>   libexec_dirs;
	/** User name                       */
	STLW::string                 user;
	/** User Id                         */
	INT_64                       uid;
	/** Group name                      */
	STLW::string                 group;
	/** Group Id                        */
	INT_64                       gid;
	/** Pid file                        */
	STLW::string                 pid_file;
	/** Host name                       */
	STLW::string                 hostname;
	/** Number of network threads       */
	UINT_32                      network_threads;
	/** Number of network threads       */
	bool                         bind_network_threads_to_cpu;
	/** Base process priority           */
	INT_32                       priority;
	/** Logger type                     */
	STLW::string                 logger_type;
	/** Debug level                     */
	DebugLevel                   debug_level;
	/** Max. number of files in cache   */
	UINT_32                      file_cache_size;
	/** Daemonize, if need              */
	bool                         daemonize;
	/** Moduile loading order           */
	STLW::vector<STLW::string>   load_order;
	/** List of active services         */
	STLW::vector<STLW::string>   load_services;
	/** Available modules               */
	STLW::vector<ModuleConfig>   modules;
	/** List of services                */
	STLW::vector<ServiceConfig>  services;
	/** Available handlers              */
	STLW::vector<HandlerConfig>  handlers;
	/** Available loggers               */
	STLW::vector<ModuleConfig>   loggers;
	/** Raw config data                 */
	VariantNC                    raw_config;

	/**
	  @brief Get configuration from variant data type
	*/
	static State  GetConfig(const VariantNC  & oData,
	                        MainConfig       & oMainConfig,
	                        Logger           & oLogger);
};

} // namespace IRIS
#endif // _MAIN_CONFIG_HPP__
// End.

