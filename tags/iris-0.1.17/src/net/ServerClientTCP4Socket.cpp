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

#include "ServerClientTCP4Socket.hpp"

namespace IRIS
{

//
// Constructor
//
ServerClientTCP4Socket::ServerClientTCP4Socket(const INT_32   iISocket,
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
CCHAR_P ServerClientTCP4Socket::GetIP() const { return szIP; }

//
// Get client IP
//
UINT_32 ServerClientTCP4Socket::GetPort() const { return iPort; }

//
// Open socket
//
TCPSocket::State ServerClientTCP4Socket::Open()
{
	;;
return OK;
}

//
// Close socket
//
TCPSocket::State ServerClientTCP4Socket::Close()
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
TCPSocket::AddressFamily ServerClientTCP4Socket::GetAddressFamily() const { return INET; }

//
// Destructor
//
ServerClientTCP4Socket::~ServerClientTCP4Socket() throw()
{
	Close();
}

} // namespace IRIS
// End.
