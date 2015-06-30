/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _TCP_SOCKET_HPP__
#define _TCP_SOCKET_HPP__ 1

/**
  @file TCPSocket.hpp
  @brief Generic TCP socket, base class
*/
#include "Types.h"

namespace IRIS
{

/**
  @class TCPSocket TCPSocket.hpp <TCPSocket.hpp>
  @brief Base client TCP socket class
*/
class TCPSocket
{
public:
	/** Address family, see socket(2) */
	enum AddressFamily { INET, INET6, UNIX };

	/** Socket options, see getsockopt(2) */
	enum SocketMode { BLOCKING, NON_BLOCKING, CLOSE_ON_EXEC, NOT_CLOSE_ON_EXEC };

	/** Socket options, see getsockopt(2) */
	enum SocketFlags { KEEPALIVE, REUSE_PORT, REUSE_ADDRESS, LINGER, NODELAY };

	/** Socket state                      */
	enum State { OK, UNIX_ERROR, END_OF_DATA, TIMEOUT, NEED_READ, NEED_WRITE };

	/**
	  @brief Constructor
	*/
	TCPSocket(const INT_32 iISocket = -1);

	/**
	  @brief Open socket
	  @return OK, always success
	*/
	virtual State Open() = 0;

	/**
	  @brief Close socket
	  @return OK, always success
	*/
	virtual State Close() = 0;

	/**
	  @brief Get error
	  @return errno value
	*/
	virtual INT_32 GetError() const;

	/**
	  @brief Clone socket descriptor
	  @return socket descriptor
	*/
	virtual INT_32 CloneSocket();

	/**
	  @brief Get socket descriptor
	  @return socket descriptor
	*/
	virtual INT_32 GetSocket();

	/**
	  @brief Set socket operating mode (blocking or non-blocking)
	  @return OK - if success, SERVER_ERROR
	*/
	virtual State SetMode(const SocketMode  eMode);

	/**
	  @brief Set socket flags
	*/
	virtual State SetFlags(const SocketFlags  eFlag,
	                       const INT_32       iFlagValue = 0);

	/**
	  @brief Get address family
	  @return One of { INET, INET6, UNIX }
	*/
	virtual AddressFamily GetAddressFamily() const = 0;

	/**
	  @brief Check socket support SSL/TLS
	  @return true, it socket supports SSL/TLS
	*/
	virtual bool IsSSL() const = 0;

	/**
	  @brief A virtual destructor
	*/
	virtual ~TCPSocket() throw();

private:
	// Does not exist
	TCPSocket(const TCPSocket & oRhs);
	TCPSocket &operator=(const TCPSocket & oRhs);

protected:
	/** TCP socket  */
	INT_32  iSocket;
	/** Error code  */
	INT_32  iErrNo;

	/**
	  @brief Set error
	  @param iErrNo - error code
	*/
	void SetError(const INT_32 iErrNo);
};

} // namespace IRIS
#endif // _TCP_SOCKET_HPP__
// End.
