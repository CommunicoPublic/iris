/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SERVER_TCP_SOCKET_HPP__
#define _SERVER_TCP_SOCKET_HPP__ 1

#include "TCPSocket.hpp"

namespace IRIS
{
// FWD
class ClientTCPSocket;

/**
  @class ServerTCPSocket ServerTCPSocket.hpp <ServerTCPSocket.hpp>
  @brief Base server TCP socket class
*/
class ServerTCPSocket:
  public TCPSocket
{
public:
	/**
	  @brief Constructor
	*/
	ServerTCPSocket();

	/**
	  @brief Bind to the interface
	*/
	virtual TCPSocket::State Bind() = 0;

	/**
	  @brief Listen for connections
	*/
	virtual TCPSocket::State Listen(const INT_32 iBacklog);

	/**
	  @brief Accept connection and get socket descriptor
	*/
	virtual TCPSocket::State Accept(ClientTCPSocket *  & pServerClientSocket) = 0;

	/**
	  @brief Check socket support SSL/TLS
	  @return true, it socket supports SSL/TLS
	*/
	virtual bool IsSSL() const;

	/**
	  @brief A virtual destructor
	*/
	virtual ~ServerTCPSocket() throw();

protected:
	// Does not exist
	ServerTCPSocket(const ServerTCPSocket & oRhs);
	ServerTCPSocket &operator=(const ServerTCPSocket & oRhs);
};

} // namespace IRIS
#endif // _SERVER_TCP_SOCKET_HPP__
// End.


