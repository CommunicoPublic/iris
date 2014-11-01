/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/un.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>
#include <netdb.h>
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ClientTCPUnixSocket.hpp"

namespace IRIS
{

//
// Constructor
//
ClientTCPUnixSocket::ClientTCPUnixSocket(CCHAR_P        szIPath,
                                         const UINT_32  iIMode): ClientTCPSocket(false),
                                                                 szPath(NULL),
                                                                 iMode(iIMode),
                                                                 bCloseAtExit(true)
{
	szPath = strdup(szIPath);
}

//
// Get client IP
//
CCHAR_P ClientTCPUnixSocket::GetPath() const { return szPath; }

//
// Open socket
//
TCPSocket::State ClientTCPUnixSocket::Open()
{
	iSocket = socket(PF_UNIX, SOCK_STREAM, 0);
	if (iSocket == -1)
	{
		SetError(errno);
		return TCPSocket::UNIX_ERROR;
	}

return TCPSocket::OK;
}

//
// Connect to the server
//
TCPSocket::State ClientTCPUnixSocket::Connect()
{
	struct sockaddr_un oSockAddr;
	memset(&oSockAddr, 0, sizeof(struct sockaddr_un));

	oSockAddr.sun_family = PF_UNIX;

	strncpy(oSockAddr.sun_path, szPath, sizeof(oSockAddr.sun_path));

	// Connect to server
	INT_32 iRC = connect(iSocket, (struct sockaddr*)&oSockAddr, sizeof(struct sockaddr_un));
	if (iRC == -1 && errno != EINPROGRESS)
	{
		SetError(errno);
		Close();
		return TCPSocket::UNIX_ERROR;
	}

	// Check error
	int iValue = 0;
	socklen_t lErrorLength = sizeof(iValue);
	iRC = getsockopt(iSocket, SOL_SOCKET, SO_ERROR, &iValue, &lErrorLength);
	if (iRC == 0 && iValue != 0)
	{
		SetError(iValue);
		Close();
		return TCPSocket::UNIX_ERROR;
	}

return TCPSocket::OK;
}

//
// Disconnect from server
//
TCPSocket::State ClientTCPUnixSocket::Close()
{
	if (iSocket == -1) { return TCPSocket::OK; }

	close(iSocket);

	iSocket = -1;

return TCPSocket::OK;
}

//
// Clone socket descriptor
//
INT_32 ClientTCPUnixSocket::CloneSocket()
{
	bCloseAtExit = false;

return iSocket;
}

//
// Get address family
//
TCPSocket::AddressFamily ClientTCPUnixSocket::GetAddressFamily() const { return UNIX; }

//
// Destructor
//
ClientTCPUnixSocket::~ClientTCPUnixSocket() throw()
{
	if (bCloseAtExit) { Close(); }
	free(szPath);
}

} // namespace IRIS
// End.
