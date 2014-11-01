/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SERVER_TCP6_SSL_SOCKET_HPP__
#define _SERVER_TCP6_SSL_SOCKET_HPP__ 1

#include "ServerTCPSSLSocket.hpp"

namespace IRIS
{

/**
  @class ServerTCP6SSLSocket TCP6Socket.hpp <TCP6Socket.hpp>
  @brief IPv4 server socket implementation
*/
class ServerTCP6SSLSocket:
  public ServerTCPSSLSocket
{
public:
	/**
	  @brief Constructor
	  @param szIHost - host name or IP address
	  @param iIPort - port
	*/
	ServerTCP6SSLSocket(const bool       bIStartTLS,
	                    CCHAR_P          szIHost,
	                    const UINT_32    iIPort);

	/**
	  @brief Open socket
	*/
	TCPSocket::State Open();

	/**
	  @brief Close socket
	  @return OK, always success
	*/
	TCPSocket::State Close();

	/**
	  @brief Bind to the address
	*/
	TCPSocket::State Bind();

	/**
	  @brief Accept connection and get socket descriptor
	*/
	TCPSocket::State Accept(ClientTCPSocket *  & pServerClientSocket);

	/**
	  @brief Get address family
	  @return One of { INET, INET6, UNIX }
	*/
	AddressFamily GetAddressFamily() const;

	/**
	  @brief A destructor
	*/
	virtual ~ServerTCP6SSLSocket() throw();

private:
	// Does not exist
	ServerTCP6SSLSocket(const ServerTCP6SSLSocket & oRhs);
	ServerTCP6SSLSocket & operator=(const ServerTCP6SSLSocket & oRhs);

	/** Server Hostname      */
	CHAR_P        szHost;
	/** Server IP            */
	CHAR_P        szIP;
	/** Port                 */
	UINT_32       iPort;
};

} // namespace IRIS
#endif // _SERVER_TCP6_SSL_SOCKET_HPP__
// End.
