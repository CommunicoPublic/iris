/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SERVER_TCP4_SSL_SOCKET_HPP__
#define _SERVER_TCP4_SSL_SOCKET_HPP__ 1

#include "ServerTCPSSLSocket.hpp"

namespace IRIS
{

/**
  @class ServerTCP4SSLSocket TCP4Socket.hpp <TCP4Socket.hpp>
  @brief IPv4 server socket implementation
*/
class ServerTCP4SSLSocket:
  public ServerTCPSSLSocket
{
public:
	/**
	  @brief Constructor
	  @param szIHost - host name or IP address
	  @param iIPort - port
	*/
	ServerTCP4SSLSocket(const bool       bIStartTLS,
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
	virtual ~ServerTCP4SSLSocket() throw();

private:
	// Does not exist
	ServerTCP4SSLSocket(const ServerTCP4SSLSocket & oRhs);
	ServerTCP4SSLSocket & operator=(const ServerTCP4SSLSocket & oRhs);

	/** Server Hostname      */
	CHAR_P        szHost;
	/** Server IP            */
	CHAR_P        szIP;
	/** Port                 */
	UINT_32       iPort;
};

} // namespace IRIS
#endif // _SERVER_TCP4_SSL_SOCKET_HPP__
// End.
