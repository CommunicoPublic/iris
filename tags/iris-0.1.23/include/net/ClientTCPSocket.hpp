/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _CLIENT_TCP_SOCKET_HPP__
#define _CLIENT_TCP_SOCKET_HPP__ 1

#include "TCPSocket.hpp"

namespace IRIS
{

/**
  @class ClientTCPSocket ClientTCPSocket.hpp <ClientTCPSocket.hpp>
  @brief Base client TCP socket class
*/
class ClientTCPSocket:
  public TCPSocket
{
public:
	/**
	  @brief Constructor
	*/
	ClientTCPSocket(const bool bICheckTCPInfo = true);

	/**
	  @brief Get client IP
	*/
	virtual CCHAR_P GetPath() const;

	/**
	  @brief Get client IP
	*/
	virtual CCHAR_P GetIP() const;

	/**
	  @brief Get client Port
	*/
	virtual UINT_32 GetPort() const;

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
	virtual ~ClientTCPSocket() throw();

protected:
	// Does not exist
	ClientTCPSocket(const ClientTCPSocket & oRhs);
	ClientTCPSocket  & operator=(const ClientTCPSocket & oRhs);

	/** Check TCP info flag, if applicable */
	const bool   bCheckTCPInfo;
};

} // namespace IRIS
#endif // _CLIENT_TCP_SOCKET_HPP__
// End.
