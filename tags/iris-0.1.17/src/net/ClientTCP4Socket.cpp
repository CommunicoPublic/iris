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

#include "ClientTCP4Socket.hpp"
#include "Resolver.hpp"

namespace IRIS
{

//
// Constructor
//
ClientTCP4Socket::ClientTCP4Socket(CCHAR_P        szIHost,
                                   const UINT_32  iIPort): szHost(NULL),
                                                           szIP(NULL),
                                                           iPort(iIPort),
                                                           bCloseAtExit(true)
{
	szHost = strdup(szIHost);
}

//
// Get client IP
//
CCHAR_P ClientTCP4Socket::GetIP() const { return szIP; }

//
// Get client Port
//
UINT_32 ClientTCP4Socket::GetPort() const { return iPort; }

//
// Open socket
//
TCPSocket::State ClientTCP4Socket::Open()
{
	iSocket = socket(PF_INET, SOCK_STREAM, 0);
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
TCPSocket::State ClientTCP4Socket::Connect()
{
	struct sockaddr_in oSockAddr;
	memset(&oSockAddr, 0, sizeof(struct sockaddr_in));

	oSockAddr.sin_family = PF_INET;
	oSockAddr.sin_port   = htons(iPort);

	if (szIP != NULL) { free(szIP); }
	// Resolve host name
	if (Resolver::Resolve4Hostname(szHost, szIP) != Resolver::OK)
	{
		SetError(errno);
		return TCPSocket::UNIX_ERROR;
	}

	if (inet_aton(szIP, &oSockAddr.sin_addr) == 0)
	{
		SetError(errno);
		return TCPSocket::UNIX_ERROR;
	}

	// Connect to server
	INT_32 iRC = connect(iSocket, (struct sockaddr*)&oSockAddr, sizeof(struct sockaddr_in));
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
TCPSocket::State ClientTCP4Socket::Close()
{
	if (iSocket == -1) { return TCPSocket::OK; }

	close(iSocket);

	iSocket = -1;

return TCPSocket::OK;
}

//
// Clone socket descriptor
//
INT_32 ClientTCP4Socket::CloneSocket()
{
	bCloseAtExit = false;

return iSocket;
}

//
// Get address family
//
TCPSocket::AddressFamily ClientTCP4Socket::GetAddressFamily() const { return INET; }

//
// Destructor
//
ClientTCP4Socket::~ClientTCP4Socket() throw()
{
	if (bCloseAtExit) { Close(); }
	free(szHost);
	free(szIP);
}

} // namespace IRIS
// End.
