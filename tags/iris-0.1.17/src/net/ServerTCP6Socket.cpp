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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>

#include "ServerTCP6Socket.hpp"
#include "ServerClientTCP6Socket.hpp"
#include "Resolver.hpp"

namespace IRIS
{

//
// Constructor
//
ServerTCP6Socket::ServerTCP6Socket(CCHAR_P        szIHost,
                                   const UINT_32  iIPort): szHost(NULL),
                                                           szIP(NULL),
                                                           iPort(iIPort)
{
	szHost = strdup(szIHost);
}

//
// Open socket
//
TCPSocket::State ServerTCP6Socket::Open()
{
	iSocket = socket(PF_INET6, SOCK_STREAM, 0);
	if (iSocket == -1)
	{
		SetError(errno);
		return UNIX_ERROR;
	}

return OK;
}

//
// Close socket
//
TCPSocket::State ServerTCP6Socket::Close()
{
	close(iSocket);
	iSocket = -1;

return OK;
}

//
// Bind to the address
//
TCPSocket::State ServerTCP6Socket::Bind()
{
	struct sockaddr_in6 oSockAddr;
	memset(&oSockAddr, 0, sizeof(struct sockaddr_in6));

	oSockAddr.sin6_family = PF_INET6;
	oSockAddr.sin6_port   = htons(iPort);

	if (szIP != NULL) { free(szIP); }

	if (szHost && *szHost == '*')
	{
		oSockAddr.sin6_addr = in6addr_any;
	}
	else
	{
		// Resolve host name
		if (Resolver::Resolve6Hostname(szHost, szIP) != Resolver::OK)
		{
			SetError(errno);
			return UNIX_ERROR;
		}

		if (inet_pton(PF_INET6, szIP, &oSockAddr.sin6_addr) == 0)
		{
			SetError(errno);
			return UNIX_ERROR;
		}
	}

	// Bind to interface
	if (bind(iSocket, (struct sockaddr *)(&oSockAddr), sizeof(oSockAddr)) == -1)
	{
		SetError(errno);
		return UNIX_ERROR;
	}

return OK;
}

//
// Accept connection and get socket descriptor
//
TCPSocket::State ServerTCP6Socket::Accept(ClientTCPSocket *  & pServerClientSocket)
{
	pServerClientSocket = NULL;

	struct sockaddr_in6 sSocketAddress;
	socklen_t sSocketAddressLen = sizeof(sSocketAddress);
	INT_32 iClientSocket = accept(iSocket, (struct sockaddr *)&sSocketAddress, &sSocketAddressLen);
	if (iClientSocket == -1)
	{
		SetError(errno);
		return UNIX_ERROR;
	}

	CHAR_8 sBuffer[64];
	// 0000:0000:0000:0000:0000:0000:0000:0000
	if (inet_ntop(PF_INET6, &sSocketAddress.sin6_addr, sBuffer, 64) == NULL)
	{
		close(iClientSocket);
		SetError(errno);
		return UNIX_ERROR;
	}

	pServerClientSocket = new ServerClientTCP6Socket(iClientSocket, sBuffer, ntohs(sSocketAddress.sin6_port));

return OK;
}

//
// Get address family
//
TCPSocket::AddressFamily ServerTCP6Socket::GetAddressFamily() const { return INET6; }

//
// Destructor
//
ServerTCP6Socket::~ServerTCP6Socket() throw()
{
	Close();
	free(szIP);
	free(szHost);
}

} // namespace IRIS
// End.
