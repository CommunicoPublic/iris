/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _RESOLVER_HPP__
#define _RESOLVER_HPP__ 1

#include "Types.h"

namespace IRIS
{

/**
  @class Resolver Resolver.hpp <Resolver.hpp>
  @brief Host name resolver
*/
class Resolver
{
public:
	enum State { OK, UNIX_ERROR };
	/**
	  @brief Resolve IPv4 host name
	  @param szHost - host name
	  @param szIP - IP address
	  @return OK, always success
	*/
	static State Resolve4Hostname(CHAR_P szHost, CHAR_P & szIP);

	/**
	  @brief Resolve IPv6 host name
	  @param szHost - host name
	  @param szIP - IP address
	  @return OK, always success
	*/
	static State Resolve6Hostname(CHAR_P szHost, CHAR_P & szIP);
private:

};

} // namespace IRIS
#endif // _RESOLVER_HPP__
// End.
