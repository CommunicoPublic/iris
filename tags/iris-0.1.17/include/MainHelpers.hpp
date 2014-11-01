/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MAIN_HELPERS_HPP__
#define _MAIN_HELPERS_HPP__ 1

/**
  @file MainHelpers.hpp
  @brief Iris main process heplers
*/

#include "MainConfig.hpp"

namespace IRIS
{
class Logger;

/**
  @class GlobalContext GlobalContext.hpp <GlobalContext.hpp>
  @brief Iris global context
*/
class MainHelpers
{
public:

	/**
	  @brief Daemonize process
	*/
	static INT_32 Daemonize(CCHAR_P    szPidFile,
	                        Logger   & oLogger);

	/**
	  @brief Set priority and switch to unprivileged user
	*/
	static INT_32 UnixSetup(const MainConfig  & oMainConfig,
	                        Logger            & oLogger);
};

} // namespace IRIS
#endif // _MAIN_HELPERS_HPP__
// End.

