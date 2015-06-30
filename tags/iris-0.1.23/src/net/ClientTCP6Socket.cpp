/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>
#include <netdb.h>
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ClientTCP6Socket.hpp"
#include "Resolver.hpp"

namespace IRIS
{

//
// Constructor
//
ClientTCP6Socket::ClientTCP6Socket(CCHAR_P        szIHost,
                                   const UINT_32  iIPort): ClientTCPSocket(false),
                                                           szHost(NULL),
                                                           szIP(NULL),
                                                           iPort(iIPort),
                                                           bCloseAtExit(true)
{
	szHost = strdup(szIHost);
}

//
// Get client IP
//
CCHAR_P ClientTCP6Socket::GetIP() const { return szIP; }

//
// Get client Port
//
UINT_32 ClientTCP6Socket::GetPort() const { return iPort; }

//
// Open socket
//
TCPSocket::State ClientTCP6Socket::Open()
{
	iSocket = socket(PF_INET6, SOCK_STREAM, 0);
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
TCPSocket::State ClientTCP6Socket::Connect()
{
	struct sockaddr_in6 oSockAddr;
	memset(&oSockAddr, 0, sizeof(struct sockaddr_in6));

	oSockAddr.sin6_family = PF_INET6;
	oSockAddr.sin6_port   = htons(iPort);

	if (szIP != NULL) { free(szIP); }
	// Resolve host name
	if (Resolver::Resolve6Hostname(szHost, szIP) != Resolver::OK)
	{
		SetError(errno);
		return TCPSocket::UNIX_ERROR;
	}

	if (inet_pton(PF_INET6, szIP, &oSockAddr.sin6_addr) == 0)
	{
		SetError(errno);
		return TCPSocket::UNIX_ERROR;
	}

	// Connect to server
	INT_32 iRC = connect(iSocket, (struct sockaddr*)&oSockAddr, sizeof(struct sockaddr_in6));
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
TCPSocket::State ClientTCP6Socket::Close()
{
	if (iSocket == -1) { return TCPSocket::OK; }

	close(iSocket);

	iSocket = -1;

return TCPSocket::OK;
}

//
// Clone socket descriptor
//
INT_32 ClientTCP6Socket::CloneSocket()
{
	bCloseAtExit = false;

return iSocket;
}

//
// Get address family
//
TCPSocket::AddressFamily ClientTCP6Socket::GetAddressFamily() const { return INET6; }

//
// Destructor
//
ClientTCP6Socket::~ClientTCP6Socket() throw()
{
	if (bCloseAtExit) { Close(); }
	free(szHost);
	free(szIP);
}

} // namespace IRIS
// End.
