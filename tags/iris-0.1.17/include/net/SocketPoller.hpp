/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SOCKET_POLLER_HPP__
#define _SOCKET_POLLER_HPP__ 1

#include "TCPSocket.hpp"

#include <sys/socket.h>

#include <errno.h>
#include <fcntl.h>
#include <poll.h>

namespace IRIS
{
// FWD
class TCPSocket;

/**
  @class TCPSocket TCPSocket.hpp <TCPSocket.hpp>
  @brief Base client TCP socket class
*/
template <typename T>class SocketPoller
{
public:
	struct SocketData
	{
	 	T            ident;
		TCPSocket  * socket;
		UINT_32      flags;
	};

	/**
	  @brief Constructor
	*/
	SocketPoller(const UINT_32 iIMaxSockets);

	/**
	  @brief Add socket
	  @return OK, always success
	*/
	TCPSocket::State AddSocket(TCPSocket      * pSocket,
	                           const T        & iIdent,
	                           const UINT_32    iFlags);

	/**
	  @brief Poll sockets
	  @return , always success
	*/
	TCPSocket::State Poll(const UINT_32    iTimeoout);

	/**
	  @brief Get list of sockets
	  @return NULL-terminated array with ready for i/o sockets
	*/
	SocketData * GetReadySockets(UINT_32  & iReadySockets);

	/**
	  @brief Get error
	  @return errno value
	*/
	INT_32 GetError() const;

	/**
	  @brief A destructor
	*/
	~SocketPoller() throw();

protected:
	// Does not exist
	SocketPoller(const SocketPoller & oRhs);
	SocketPoller &operator=(const SocketPoller & oRhs);

	/** Max number of sockets */
	const UINT_32 iMaxSockets;

	/** Used sockets          */
	UINT_32 iUsedSockets;

	/** List of all sockets   */
	SocketData  * aSockets;

	/** List of rerady sockets   */
	UINT_32       iReadySocketsNum;
	/** List of rerady sockets   */
	SocketData  * aReadySockets;

	struct pollfd  * aPollSockets;

	/** Error code  */
	INT_32 iErrNo;
};

} // namespace IRIS

#include "SocketPoller.tcc"

#endif // _SOCKET_POLLER_HPP__
// End.
