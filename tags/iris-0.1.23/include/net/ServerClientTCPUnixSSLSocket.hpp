/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SERVER_CLIENT_TCP_UNIX_SSL_SOCKET_HPP__
#define _SERVER_CLIENT_TCP_UNIX_SSL_SOCKET_HPP__ 1

#include "ServerClientTCPSSLSocket.hpp"

namespace IRIS
{

/**
  @class ServerClientTCPUnixSSLSocket ServerClientTCPUnixSSLSocket.hpp <ServerClientTCPUnixSSLSocket.hpp>
  @brief Base server-client TCP socket class
*/
class ServerClientTCPUnixSSLSocket:
  public ServerClientTCPSSLSocket
{
public:
	/**
	  @brief Constructor
	*/
	ServerClientTCPUnixSSLSocket(const bool      bIStartTLS,
	                             const INT_32    iISocket,
	                             CCHAR_P         szPath);

	/**
	  @brief Get client IP
	*/
	CCHAR_P GetPath() const;

	/**
	  @brief Get address family
	  @return One of { INET, INET6, UNIX }
	*/
	AddressFamily GetAddressFamily() const;

	/**
	  @brief A virtual destructor
	*/
	virtual ~ServerClientTCPUnixSSLSocket() throw();

protected:
	// Does not exist
	ServerClientTCPUnixSSLSocket(const ServerClientTCPUnixSSLSocket & oRhs);
	ServerClientTCPUnixSSLSocket  & operator=(const ServerClientTCPUnixSSLSocket & oRhs);

	/** Socket path */
	CHAR_P         szPath;
};

} // namespace IRIS
#endif // _SERVER_CLIENT_TCP_UNIX_SSL_SOCKET_HPP__
// End.
