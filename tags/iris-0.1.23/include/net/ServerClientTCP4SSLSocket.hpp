/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SERVER_CLIENT_TCP4_SSL_SOCKET_HPP__
#define _SERVER_CLIENT_TCP4_SSL_SOCKET_HPP__ 1

#include "ServerClientTCPSSLSocket.hpp"

namespace IRIS
{

/**
  @class ServerClientTCP4SSLSocket ServerClientTCP4SSLSocket.hpp <ServerClientTCP4SSLSocket.hpp>
  @brief Base server-client TCP socket class
*/
class ServerClientTCP4SSLSocket:
  public ServerClientTCPSSLSocket
{
public:
	/**
	  @brief Constructor
	*/
	ServerClientTCP4SSLSocket(const bool      bIStartTLS,
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
	virtual ~ServerClientTCP4SSLSocket() throw();

protected:
	// Does not exist
	ServerClientTCP4SSLSocket(const ServerClientTCP4SSLSocket & oRhs);
	ServerClientTCP4SSLSocket  & operator=(const ServerClientTCP4SSLSocket & oRhs);

	/** Client IP   */
	CHAR_P     szIP;
	/** Client Port */
	UINT_32    iPort;
};

} // namespace IRIS
#endif // _SERVER_CLIENT_TCP4_SSL_SOCKET_HPP__
// End.
