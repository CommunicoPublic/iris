/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <errno.h>
#include <fcntl.h>
#include <poll.h>

#include "TCPSocket.hpp"

namespace IRIS
{

//
// Constructor
//
TCPSocket::TCPSocket(const INT_32 iISocket): iSocket(iISocket), iErrNo(0) { ;; }

//
// Get error code
//
INT_32 TCPSocket::GetError() const { return iErrNo; }

//
// Clone socket descriptor
//
INT_32 TCPSocket::CloneSocket() { return iSocket; }

//
// Get socket descriptor
//
INT_32 TCPSocket::GetSocket() { return iSocket; }

//
// Set socket operating mode (blocking or non-blocking)
//
TCPSocket::State TCPSocket::SetMode(const SocketMode  eMode)
{
	const INT_32 iSocketFlags = fcntl(iSocket, F_GETFL, 0);

	switch (eMode)
	{
		// Set the socket nonblocking
		case NON_BLOCKING:
			fcntl(iSocket, F_SETFL, iSocketFlags | O_NONBLOCK);
			break;
		// Set the socket blocking
		case BLOCKING:
			fcntl(iSocket, F_SETFL, iSocketFlags & (~O_NONBLOCK));
			break;
		// Close socket on exec fro child process
		case CLOSE_ON_EXEC:
			fcntl(iSocket, F_SETFL, iSocketFlags | FD_CLOEXEC);
			break;
		// Not close socket on exec fro child process
		case NOT_CLOSE_ON_EXEC:
			fcntl(iSocket, F_SETFL, iSocketFlags & (~FD_CLOEXEC));
			break;
	}

return OK;
}

//
// Set socket flags
//
TCPSocket::State TCPSocket::SetFlags(const SocketFlags  eFlag, const INT_32 iFlagValue)
{
	INT_32 iRC = 0;
	int iValue = iFlagValue;
	switch(eFlag)
	{
		case NODELAY:
			iRC = setsockopt(iSocket, IPPROTO_TCP, TCP_NODELAY, &iValue, sizeof(iValue));
			break;
		case KEEPALIVE:
			iRC = setsockopt(iSocket, SOL_SOCKET, SO_KEEPALIVE, &iValue, sizeof(iValue));
			break;
		case REUSE_ADDRESS:
			iRC = setsockopt(iSocket, SOL_SOCKET, SO_REUSEADDR, &iValue, sizeof(iValue));
			break;
		case REUSE_PORT:
#ifdef SO_REUSEPORT
			iRC = setsockopt(iSocket, SOL_SOCKET, SO_REUSEPORT, &iValue, sizeof(iValue));
#endif
			break;
		case LINGER:
			{
				struct linger sLinger;
				if (iValue != 0)
				{
					sLinger.l_onoff  = 1;
					sLinger.l_linger = iValue;
				}
				else
				{
					sLinger.l_onoff  = 0;
					sLinger.l_linger = 0;
				}
				iRC = setsockopt(iSocket, SOL_SOCKET, SO_LINGER, &sLinger, sizeof(struct linger));
			}
			break;
	}

	if (iRC == -1)
	{
		iErrNo = errno;
		return UNIX_ERROR;
	}

return OK;
}

//
// Store error code
//
void TCPSocket::SetError(const INT_32 iIErrNo) { iErrNo = iIErrNo; }

//
// Destructor
//
TCPSocket::~TCPSocket() throw() { ;; }

} // namespace IRIS
// End.
