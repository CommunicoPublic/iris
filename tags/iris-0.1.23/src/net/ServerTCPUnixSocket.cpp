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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>

#include "ServerTCPUnixSocket.hpp"
#include "ServerClientTCPUnixSocket.hpp"

namespace IRIS
{

//
// Constructor
//
ServerTCPUnixSocket::ServerTCPUnixSocket(CCHAR_P        szIPath,
                                         const UINT_32  iIMode): szPath(NULL),
                                                                 iMode(iIMode)
{
	szPath = strdup(szIPath);
}

//
// Open socket
//
TCPSocket::State ServerTCPUnixSocket::Open()
{
	iSocket = socket(PF_UNIX, SOCK_STREAM, 0);
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
TCPSocket::State ServerTCPUnixSocket::Close()
{
	close(iSocket);
	iSocket = -1;
	unlink(szPath);

return OK;
}

//
// Bind to the address
//
TCPSocket::State ServerTCPUnixSocket::Bind()
{
	struct sockaddr_un oSockAddr;
	memset(&oSockAddr, 0, sizeof(struct sockaddr_un));

	oSockAddr.sun_family = PF_UNIX;

	strncpy(oSockAddr.sun_path, szPath, sizeof(oSockAddr.sun_path));
	unlink(oSockAddr.sun_path);

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
TCPSocket::State ServerTCPUnixSocket::Accept(ClientTCPSocket *  & pServerClientSocket)
{
	pServerClientSocket = NULL;

	struct sockaddr_un sSocketAddress;
	socklen_t sSocketAddressLen = sizeof(sSocketAddress);
	INT_32 iClientSocket = accept(iSocket, (struct sockaddr *)&sSocketAddress, &sSocketAddressLen);

	if (iClientSocket == -1)
	{
		SetError(errno);
		return UNIX_ERROR;
	}

	pServerClientSocket = new ServerClientTCPUnixSocket(iClientSocket, szPath);

return OK;
}

//
// Get address family
//
TCPSocket::AddressFamily ServerTCPUnixSocket::GetAddressFamily() const { return UNIX; }

//
// Destructor
//
ServerTCPUnixSocket::~ServerTCPUnixSocket() throw()
{
	Close();
	unlink(szPath);
	free(szPath);
}

} // namespace IRIS
// End.
