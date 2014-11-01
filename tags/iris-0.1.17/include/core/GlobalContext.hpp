/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _GLOBAL_CONTEXT_HPP__
#define _GLOBAL_CONTEXT_HPP__ 1

/**
  @file GlobalContext.hpp
  @brief Iris global context
*/

#include "MainConfig.hpp"

namespace IRIS
{
// FWD
class Logger;
class FileHandleCache;

/**
  @class GlobalContext GlobalContext.hpp <GlobalContext.hpp>
  @brief Iris global context
*/
struct GlobalContext
{
	enum ShutdownType { NONE, IMMEDIATE, TRANSACTIONAL };

	/** Main configuration     */
	MainConfig            config;
	/** Object loader          */
	ObjectLoader          loader;
	/** Object factory         */
	ObjectFactory         factory;
	/** File handle cache      */
	FileHandleCache     * file_handle_cache;
	/** Global context logger  */
	Logger              * error_log;
	/** Global context logger  */
	Logger              * transfer_log;
	/** Shutdown type          */
	ShutdownType          shutdown_type;

	/**
	  @brief Constructor
	*/
	GlobalContext();
private:
	// Does not exist
	GlobalContext(const GlobalContext  & oRhs);
	GlobalContext& operator=(const GlobalContext  & oRhs);
};

} // namespace IRIS
#endif // _GLOBAL_CONTEXT_HPP__
// End.

