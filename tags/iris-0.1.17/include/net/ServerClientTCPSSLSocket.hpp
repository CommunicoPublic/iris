/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SERVER_CLIENT_TCP_SSL_SOCKET_HPP__
#define _SERVER_CLIENT_TCP_SSL_SOCKET_HPP__ 1

#include "ClientTCPSSLSocket.hpp"

typedef struct ssl_ctx_st SSL_CTX;
typedef struct ssl_st SSL;

namespace IRIS
{

/**
  @class ServerClientTCPSSLSocket ServerClientTCPSSLSocket.hpp <ServerClientTCPSSLSocket.hpp>
  @brief Base client TCP socket class
*/
class ServerClientTCPSSLSocket:
  public ClientTCPSSLSocket
{
public:

	/**
	  @brief Constructor
	*/
	ServerClientTCPSSLSocket(const bool      bIStartTLS,
	                         const INT_32    iISocket);

	/**
	  @brief Set SSL context
	*/
	virtual void SetSSLContext(SSL_CTX  * pISSLContext);

	/**
	  @brief Open socket
	*/
	virtual TCPSocket::State Open();

	/**
	  @brief Close socket
	*/
	virtual TCPSocket::State Close();

	/**
	  @brief Switch to secure layer
	*/
	virtual void StartTLS();

	/**
	  @brief Switch to insecure layer
	*/
	virtual void StopTLS();

	/**
	  @brief Check SSL was handshaked
	*/
	virtual bool Handshaked() const;

	/**
	  @brief start client-requested connection renegatiation
	*/
	virtual void Renegotiate();

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
	  @brief A virtual destructor
	*/
	virtual ~ServerClientTCPSSLSocket() throw();

protected:
	// Does not exist
	ServerClientTCPSSLSocket(const ServerClientTCPSSLSocket & oRhs);
	ServerClientTCPSSLSocket  & operator=(const ServerClientTCPSSLSocket & oRhs);

	/** SSL context */
	SSL_CTX       * pSSLContext;
	/** Enable STARTTLS/STLS */
	const bool      bStartTLS;

	/** SSL object  */
	SSL           * pSSL;
	/** SSL was handshaked */
	bool            bHandshaked;
	/** Renegotiation request from client */
	bool            bRenegotiate;

	/**
	  @brief Start SSL handshake
	*/
	TCPSocket::State DoSSLHandshake();
};

} // namespace IRIS
#endif // _SERVER_CLIENT_TCP_SSL_SOCKET_HPP__
// End.
