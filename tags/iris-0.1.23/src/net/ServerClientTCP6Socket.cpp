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

#include "ServerClientTCP6Socket.hpp"

namespace IRIS
{

//
// Constructor
//
ServerClientTCP6Socket::ServerClientTCP6Socket(const INT_32   iISocket,
                                               CCHAR_P        szIIP,
                                               const UINT_32  iIPort)
{
	iSocket = iISocket;
	szIP    = strdup(szIIP);
	iPort   = iIPort;
}

//
// Get client IP
//
CCHAR_P ServerClientTCP6Socket::GetIP() const { return szIP; }

//
// Get client IP
//
UINT_32 ServerClientTCP6Socket::GetPort() const { return iPort; }

//
// Open socket
//
TCPSocket::State ServerClientTCP6Socket::Open()
{
	;;
return OK;
}

//
// Close socket
//
TCPSocket::State ServerClientTCP6Socket::Close()
{
	if (iSocket == -1) { return OK; }

	close(iSocket);
	free(szIP);
	szIP = NULL;
	iSocket = -1;

return OK;
}

//
// Get address family
//
TCPSocket::AddressFamily ServerClientTCP6Socket::GetAddressFamily() const { return INET6; }

//
// Destructor
//
ServerClientTCP6Socket::~ServerClientTCP6Socket() throw()
{
	Close();
}

} // namespace IRIS
// End.
