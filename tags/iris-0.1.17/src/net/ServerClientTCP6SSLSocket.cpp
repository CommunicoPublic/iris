/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

#include <sys/types.h>
#include <sys/socket.h>

#include <openssl/ssl.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ServerClientTCP6SSLSocket.hpp"

namespace IRIS
{

//
// Constructor
//
ServerClientTCP6SSLSocket::ServerClientTCP6SSLSocket(const bool      bIStartTLS,
                                                     const INT_32    iISocket,
                                                     CCHAR_P         szIIP,
                                                     const UINT_32   iIPort): ServerClientTCPSSLSocket(bIStartTLS, iISocket)
{
	szIP    = strdup(szIIP);
	iPort   = iIPort;
}

//
// Get client IP
//
CCHAR_P ServerClientTCP6SSLSocket::GetIP() const { return szIP; }

//
// Get client IP
//
UINT_32 ServerClientTCP6SSLSocket::GetPort() const { return iPort; }

//
// Get address family
//
TCPSocket::AddressFamily ServerClientTCP6SSLSocket::GetAddressFamily() const { return INET6; }

//
// Destructor
//
ServerClientTCP6SSLSocket::~ServerClientTCP6SSLSocket() throw()
{
	Close();
	free(szIP);
}

} // namespace IRIS
// End.
