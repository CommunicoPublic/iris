/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SERVER_TCP_SSL_SOCKET_HPP__
#define _SERVER_TCP_SSL_SOCKET_HPP__ 1

#include "ServerTCPSocket.hpp"

typedef struct ssl_ctx_st SSL_CTX;

namespace IRIS
{
// FWD
class ClientTCPSSLSocket;

/**
  @class ServerTCPSSLSocket ServerTCPSSLSocket.hpp <ServerTCPSSLSocket.hpp>
  @brief Base server TCP socket class
*/
class ServerTCPSSLSocket:
  public ServerTCPSocket
{
public:
	/**
	  @brief Constructor
	*/
	ServerTCPSSLSocket(const bool    bIStartTLS);

	/**
	  @brief Check socket support SSL/TLS
	  @return true, it socket supports SSL/TLS
	*/
	virtual bool IsSSL() const;

	/**
	  @brief A virtual destructor
	*/
	virtual ~ServerTCPSSLSocket() throw();

protected:
	/** Enable upgrading to secure socket layer */
	const bool    bStartTLS;

private:
	// Does not exist
	ServerTCPSSLSocket(const ServerTCPSSLSocket & oRhs);
	ServerTCPSSLSocket &operator=(const ServerTCPSSLSocket & oRhs);
};

} // namespace IRIS
#endif // _SERVER_TCP_SSL_SOCKET_HPP__
// End.


