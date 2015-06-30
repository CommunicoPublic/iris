/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "ServerClientTCPUnixSocket.hpp"

namespace IRIS
{

//
// Constructor
//
ServerClientTCPUnixSocket::ServerClientTCPUnixSocket(const INT_32   iISocket,
                                                     CCHAR_P        szIPath)
{
	iSocket = iISocket;
	szPath  = strdup(szIPath);
}

//
// Open socket
//
TCPSocket::State ServerClientTCPUnixSocket::Open()
{
	;;
return OK;
}

//
// Close socket
//
TCPSocket::State ServerClientTCPUnixSocket::Close()
{
	if (iSocket == -1) { return OK; }

	close(iSocket);
	free(szPath);

	iSocket = -1;

return OK;
}

//
// Get client IP
//
CCHAR_P ServerClientTCPUnixSocket::GetPath() const { return szPath; }

//
// Get address family
//
TCPSocket::AddressFamily ServerClientTCPUnixSocket::GetAddressFamily() const { return UNIX; }

//
// Destructor
//
ServerClientTCPUnixSocket::~ServerClientTCPUnixSocket() throw()
{
	Close();
}

} // namespace IRIS
// End.
