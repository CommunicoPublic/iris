/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SERVER_CLIENT_TCP6_SSL_SOCKET_HPP__
#define _SERVER_CLIENT_TCP6_SSL_SOCKET_HPP__ 1

#include "ServerClientTCPSSLSocket.hpp"

namespace IRIS
{

/**
  @class ServerClientTCP6SSLSocket ServerClientTCP6SSLSocket.hpp <ServerClientTCP6SSLSocket.hpp>
  @brief Base server-client TCP socket class
*/
class ServerClientTCP6SSLSocket:
  public ServerClientTCPSSLSocket
{
public:
	/**
	  @brief Constructor
	*/
	ServerClientTCP6SSLSocket(const bool      bIStartTLS,
	                          const INT_32    iISocket,
	                          CCHAR_P         szIIP,
	                          const UINT_32   iIPort);

	/**
	  @brief Get client IP
	*/
	CCHAR_P GetIP() const;

	/**
	  @brief Get client Port
	*/
	UINT_32 GetPort() const;

	/**
	  @brief Get address family
	  @return One of { INET, INET6, UNIX }
	*/
	AddressFamily GetAddressFamily() const;

	/**
	  @brief A virtual destructor
	*/
	virtual ~ServerClientTCP6SSLSocket() throw();

protected:
	// Does not exist
	ServerClientTCP6SSLSocket(const ServerClientTCP6SSLSocket & oRhs);
	ServerClientTCP6SSLSocket  & operator=(const ServerClientTCP6SSLSocket & oRhs);

	/** Client IP   */
	CHAR_P     szIP;
	/** Client Port */
	UINT_32    iPort;
};

} // namespace IRIS
#endif // _SERVER_CLIENT_TCP6_SSL_SOCKET_HPP__
// End.
