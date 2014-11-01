/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _CLIENT_TCP_SSL_SOCKET_HPP__
#define _CLIENT_TCP_SSL_SOCKET_HPP__ 1

#include "ClientTCPSocket.hpp"

namespace IRIS
{

/**
  @class ClientTCPSSLSocket ClientTCPSSLSocket.hpp <ClientTCPSSLSocket.hpp>
  @brief Base client TCP socket class
*/
class ClientTCPSSLSocket:
  public ClientTCPSocket
{
public:
	/**
	  @brief Constructor
	*/
	ClientTCPSSLSocket();

	/**
	  @brief Switch to secure layer
	*/
	virtual void StartTLS() = 0;

	/**
	  @brief Switch to insecure layer
	*/
	virtual void StopTLS() = 0;

	/**
	  @brief Check SSL was handshaked
	*/
	virtual bool Handshaked() const = 0;

	/**
	  @brief start client-requested connection renegatiation
	*/
	virtual void Renegotiate() = 0;

	/**
	  @brief Read from socket
	  @param vBuffer - destination buffer
	  @param iBufferSize - max buffer size
	  @param iReadBytes - number of bytes actually read
	  @return OK - if success, ERROR - if some error occured, TIMEOUT - if timeout reached
	*/
	virtual TCPSocket::State Read(void           * vBuffer,
	                              const UINT_64    iBufferSize,
	                              UINT_64        & iReadBytes);

	/**
	  @brief Write to socket
	  @param vBuffer - source buffer
	  @param iBufferSize - buffer size
	  @param iWroteBytes - number of written bytes
	  @return OK - if success, ERROR - if some error occured, TIMEOUT - if timeout reached
	*/
	virtual TCPSocket::State Write(const void     * vBuffer,
	                               const UINT_64    iBufferSize,
	                               UINT_64        & iWroteBytes);

	/**
	  @brief Check socket support SSL/TLS
	  @return true, it socket supports SSL/TLS
	*/
	virtual bool IsSSL() const;

	/**
	  @brief A virtual destructor
	*/
	virtual ~ClientTCPSSLSocket() throw();

protected:
	// Does not exist
	ClientTCPSSLSocket(const ClientTCPSSLSocket & oRhs);
	ClientTCPSSLSocket  & operator=(const ClientTCPSSLSocket & oRhs);
};

} // namespace IRIS
#endif // _CLIENT_TCP_SSL_SOCKET_HPP__
// End.
