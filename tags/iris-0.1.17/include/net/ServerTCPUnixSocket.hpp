/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SERVER_TCP_UNIX_SOCKET_HPP__
#define _SERVER_TCP_UNIX_SOCKET_HPP__ 1

#include "ServerTCPSocket.hpp"

namespace IRIS
{

/**
  @class ServerTCPUnixSocket TCP4Socket.hpp <TCP4Socket.hpp>
  @brief IPv4 server socket implementation
*/
class ServerTCPUnixSocket:
  public ServerTCPSocket
{
public:
	/**
	  @brief Constructor
	  @param szIHost - host name or IP address
	  @param iIPort - port
	*/
	ServerTCPUnixSocket(CCHAR_P        szPath,
	                    const UINT_32  iIMode);

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
	virtual ~ServerTCPUnixSocket() throw();

private:
	// Does not exist
	ServerTCPUnixSocket(const ServerTCPUnixSocket & oRhs);
	ServerTCPUnixSocket & operator=(const ServerTCPUnixSocket & oRhs);

	/** Socket path          */
	CHAR_P        szPath;
	/** Socvket mode         */
	UINT_32       iMode;
};

} // namespace IRIS
#endif // _SERVER_TCP_UNIX_SOCKET_HPP__
// End.
