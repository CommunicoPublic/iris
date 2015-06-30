/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _DEBUG_HPP__
#define _DEBUG_HPP__ 1

/**
  @file Debug.hpp
  @brief Various debugging functions
*/

// Local includes
#include "Types.h"
#include "STLString.hpp"

namespace IRIS
{

/**
  @class Debug Debug.hpp <Debug.hpp>
  @brief Various debugging functions
*/
class Debug
{
public:
	/**
	  @brief Print error description
	  @param iRC - unix error code
	  @return Human-readable error description
	*/
	static STLW::string PrintableError(const INT_32  iRC);

	/**
	  @brief Check for unrecoverable error
	  @param szWhere - Where error was uccured
	*/
	static void Abort(CCHAR_P szWhere);

	/**
	  @brief Check for unrecoverable error
	  @param szWhere - Where error was uccured
	  @param iRC - unix error code
	*/
	static void CheckUnrecoverable(CCHAR_P szWhere, const INT_32 iRC);
};

} // namespace IRIS
#endif // _DEBUG_HPP__
// End.
