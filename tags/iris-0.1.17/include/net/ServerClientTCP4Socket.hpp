/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SERVER_CLIENT_TCP4_SOCKET_HPP__
#define _SERVER_CLIENT_TCP4_SOCKET_HPP__ 1

#include "ClientTCPSocket.hpp"

namespace IRIS
{

/**
  @class ServerClientTCP4Socket ServerClientTCP4Socket.hpp <ServerClientTCP4Socket.hpp>
  @brief Base server-client TCP socket class
*/
class ServerClientTCP4Socket:
  public ClientTCPSocket
{
public:
	/**
	  @brief Constructor
	*/
	ServerClientTCP4Socket(const INT_32   iISocket,
	                       CCHAR_P        szIIP,
	                       const UINT_32  iIPort);

	/**
	  @brief Get client IP
	*/
	CCHAR_P GetIP() const;

	/**
	  @brief Get client Port
	*/
	UINT_32 GetPort() const;

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
	virtual ~ServerClientTCP4Socket() throw();

protected:
	// Does not exist
	ServerClientTCP4Socket(const ServerClientTCP4Socket & oRhs);
	ServerClientTCP4Socket  & operator=(const ServerClientTCP4Socket & oRhs);

	/** Client IP   */
	CHAR_P   szIP;
	/** Client Port */
	UINT_32  iPort;
};

} // namespace IRIS
#endif // _SERVER_CLIENT_TCP4_SOCKET_HPP__
// End.
