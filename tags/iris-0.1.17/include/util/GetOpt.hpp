/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _GET_OPT_HPP__
#define _GET_OPT_HPP__ 1

/**
  @file GetOpt.hpp
  @brief Get command-line options
*/
#include "Types.h"

#define C_NO_ARG    0
#define C_HAS_ARG   1
#define C_OPT_ARG   2

namespace IRIS
{

/**
  @class GetOpt GetOpt.hpp <GetOpt.hpp>
  @brief  List of command-line options
*/
struct Options
{
	/** Short option name */
	UCHAR_8     short_opt;
	/** Long option name  */
	CCHAR_P     long_opt;
	/** Argument          */
	UINT_32     has_arg;
};

/**
  @fn INT_32 GetOpt(const Options * aOptions, UINT_32 & iArgNum, const UINT_32 & iMaxArgNum, CHAR_P * aArgList, CCHAR_P & szArgValue)
  @brief Parse command-line options
  @param aOptions - array with options properties
  @param iArgNum - current argument number
  @param iMaxArgNum - number of arguments
  @param aArgList - list of arguments
  @param szArgValue - argument value
*/
INT_32 GetOpt(const Options  * aOptions,
              UINT_32        & iArgNum,
              const UINT_32    iMaxArgNum,
              CHAR_P         * aArgList,
              CCHAR_P        & szArgValue);

} // namespace IRIS
#endif // _GET_OPT_HPP__
// End.
