/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SERVER_CLIENT_TCP_UNIX_SOCKET_HPP__
#define _SERVER_CLIENT_TCP_UNIX_SOCKET_HPP__ 1

#include "ClientTCPUnixSocket.hpp"

namespace IRIS
{

/**
  @class ServerClientTCPUnixSocket ServerClientTCPUnixSocket.hpp <ServerClientTCPUnixSocket.hpp>
  @brief Base server-client TCP socket class
*/
class ServerClientTCPUnixSocket:
  public ClientTCPSocket
{
public:
	/**
	  @brief Constructor
	*/
	ServerClientTCPUnixSocket(const INT_32   iISocket,
	                          CCHAR_P        szIPath);

	/**
	  @brief Get client path
	*/
	CCHAR_P GetPath() const;

	/**
	  @brief Open socket
	  @return OK, always success
	*/
	TCPSocket::State Open();

	/**
	  @brief Close socket
	  @return OK, always success
	*/
	TCPSocket::State Close();

	/**
	  @brief Get address family
	  @return One of { INET, INET6, UNIX }
	*/
	AddressFamily GetAddressFamily() const;

	/**
	  @brief A virtual destructor
	*/
	virtual ~ServerClientTCPUnixSocket() throw();

protected:
	// Does not exist
	ServerClientTCPUnixSocket(const ServerClientTCPUnixSocket & oRhs);
	ServerClientTCPUnixSocket  & operator=(const ServerClientTCPUnixSocket & oRhs);

	/** Socket path */
	CHAR_P        szPath;
};

} // namespace IRIS
#endif // _SERVER_CLIENT_TCP_UNIX_SOCKET_HPP__
// End.
