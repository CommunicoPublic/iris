/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _NETWORK_HANDLER_HPP__
#define _NETWORK_HANDLER_HPP__ 1

/**
  @file NetworkHandler.hpp
  @brief Base class class for network handlers
*/
#include "Handler.hpp"

namespace IRIS
{
// FWD
class AcceptLoopContext;
class ServiceConfig;
class ClientTCPSocket;
class NetworkEventWatcher;

/**
  @class NetworkHandler NetworkHandler.hpp <NetworkHandler.hpp>
  @brief Base class class for network handlers
*/
class NetworkHandler:
  public Handler
{
public:
	/**
	  @brief Handle new connection
	  @param pContext - Thread loop context
	  @param pServiceConfig - Service configuration
	  @param pSocket - Client TCP connection
	  @return Pointer to NetworkEventWatcher object
	*/
	virtual NetworkEventWatcher  * NewConnection(AcceptLoopContext  & pAcceptLoopContext,
	                                             ServiceConfig      & oServiceConfig,
	                                             ClientTCPSocket    * pSocket) = 0;

	/**
	  @brief A virtual destructor
	*/
	virtual ~NetworkHandler() throw();
};

} // namespace IRIS
#endif // _NETWORK_HANDLER_HPP__
// End.
