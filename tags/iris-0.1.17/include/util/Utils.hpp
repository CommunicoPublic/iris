/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _UTILS_HPP__
#define _UTILS_HPP__ 1

/**
  @file Utils.hpp
  @brief Various helpful utilities
*/

#include "Types.h"

namespace IRIS
{

/**
  @class Util Utils.hpp <Utils.hpp>
  @brief Various helpful utilities
*/
class Util
{
public:
	/**
	  @brief Convert string to 64-bit unsigned int
	*/
	static UINT_64 StrToUInt(CCHAR_P  sData,
	                         UINT_64  iDataSize);

	/**
	  @brief Convert string to 64-bit int
	*/
	static INT_64 StrToInt(CCHAR_P  sData,
	                       UINT_64  iDataSize);

private:

};

} // namespace IRIS
#endif // _UTILS_HPP__
// End.
