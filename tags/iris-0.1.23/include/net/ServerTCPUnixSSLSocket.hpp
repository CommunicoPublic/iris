/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SERVER_TCP_UNIX_SSL_SOCKET_HPP__
#define _SERVER_TCP_UNIX_SSL_SOCKET_HPP__ 1

#include "ServerTCPSSLSocket.hpp"

namespace IRIS
{

/**
  @class ServerTCPUnixSSLSocket TCP4Socket.hpp <TCP4Socket.hpp>
  @brief IPv4 server socket implementation
*/
class ServerTCPUnixSSLSocket:
  public ServerTCPSSLSocket
{
public:
	/**
	  @brief Constructor
	  @param szIHost - host name or IP address
	  @param iIPort - port
	*/
	ServerTCPUnixSSLSocket(const bool       bIStartTLS,
	                       CCHAR_P          szIPath,
	                       const UINT_32    iIMode);

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
	virtual ~ServerTCPUnixSSLSocket() throw();

private:
	// Does not exist
	ServerTCPUnixSSLSocket(const ServerTCPUnixSSLSocket & oRhs);
	ServerTCPUnixSSLSocket & operator=(const ServerTCPUnixSSLSocket & oRhs);

	/** Socket path          */
	CHAR_P        szPath;
	/** Socket mode          */
	UINT_32       iMode;
};

} // namespace IRIS
#endif // _SERVER_TCP_UNIX_SSL_SOCKET_HPP__
// End.
