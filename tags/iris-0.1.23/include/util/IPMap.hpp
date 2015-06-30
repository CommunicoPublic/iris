/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _IP_MAP_HPP__
#define _IP_MAP_HPP__ 1

/**
  @file IPMap.hpp
  @brief IP address map
*/

#include "Types.h"
#include "STLString.hpp"

namespace IRIS
{
/**
  @struct IPMap IPMap.hpp <IPMap.hpp>
  @brief IP address map
*/
class IPMap
{
public:
	enum State { OK, ERROR };

	/**
	  @brief Add network
	  @param sSubnet - network with subnet or IP address
	*/
	virtual State AddNet(const STLW::string  & sSubnet) = 0;

	/**
	  @brief Add range of addresses
	  @param sStartIP - first IP address in range
	  @param sEndIP - last IP address in range
	*/
	virtual State AddRange(const STLW::string  & sStartIP,
	                       const STLW::string  & sEndIP) = 0;

	/**
	  @brief Check IP address
	  @param sAddress - IP address to check
	*/
	virtual State CheckIP(const STLW::string & sAddress) const = 0;

	/**
	  @brief A virtual destructor
	*/
	virtual ~IPMap() throw();
};

} // namespace IRIS
#endif // _IP_MAP_HPP__
// End.
