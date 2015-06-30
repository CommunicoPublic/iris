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

#include "ServerTCP4Socket.hpp"
#include "ServerClientTCP4Socket.hpp"
#include "Resolver.hpp"

namespace IRIS
{

//
// Constructor
//
ServerTCP4Socket::ServerTCP4Socket(CCHAR_P        szIHost,
                                   const UINT_32  iIPort): szHost(NULL),
                                                           szIP(NULL),
                                                           iPort(iIPort)
{
	szHost = strdup(szIHost);
}

//
// Open socket
//
TCPSocket::State ServerTCP4Socket::Open()
{
	iSocket = socket(PF_INET, SOCK_STREAM, 0);
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
TCPSocket::State ServerTCP4Socket::Close()
{
	close(iSocket);
	iSocket = -1;

return OK;
}

//
// Bind to the address
//
TCPSocket::State ServerTCP4Socket::Bind()
{
	struct sockaddr_in oSockAddr;
	memset(&oSockAddr, 0, sizeof(struct sockaddr_in));

	oSockAddr.sin_family = PF_INET;
	oSockAddr.sin_port   = htons(iPort);

	if (szIP != NULL) { free(szIP); }

	if (szHost && *szHost == '*')
	{
		oSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		// Resolve host name
		if (Resolver::Resolve4Hostname(szHost, szIP) != Resolver::OK)
		{
			SetError(errno);
			return UNIX_ERROR;
		}

		if (inet_aton(szIP, &oSockAddr.sin_addr) == 0)
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
TCPSocket::State ServerTCP4Socket::Accept(ClientTCPSocket *  & pServerClientSocket)
{
	pServerClientSocket = NULL;

	struct sockaddr_in sSocketAddress;
	socklen_t sSocketAddressLen = sizeof(sSocketAddress);
	INT_32 iClientSocket = accept(iSocket, (struct sockaddr *)&sSocketAddress, &sSocketAddressLen);
	if (iClientSocket == -1)
	{
		SetError(errno);
		return UNIX_ERROR;
	}

	CHAR_8 sBuffer[16];
	if (inet_ntop(PF_INET, &sSocketAddress.sin_addr, sBuffer, 16) == NULL)
	{
		close(iClientSocket);
		SetError(errno);
		return UNIX_ERROR;
	}

	pServerClientSocket = new ServerClientTCP4Socket(iClientSocket, sBuffer, ntohs(sSocketAddress.sin_port));

return OK;
}

//
// Get address family
//
TCPSocket::AddressFamily ServerTCP4Socket::GetAddressFamily() const { return INET; }

//
// Destructor
//
ServerTCP4Socket::~ServerTCP4Socket() throw()
{
	Close();
	free(szIP);
	free(szHost);
}

} // namespace IRIS
// End.
