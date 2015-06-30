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

#include "ServerClientTCPUnixSSLSocket.hpp"

namespace IRIS
{

//
// Constructor
//
ServerClientTCPUnixSSLSocket::ServerClientTCPUnixSSLSocket(const bool      bIStartTLS,
                                                           const INT_32    iISocket,
                                                           CCHAR_P         szIPath): ServerClientTCPSSLSocket(bIStartTLS, iISocket)
{
	szPath = strdup(szIPath);
}

//
// Get client IP
//
CCHAR_P ServerClientTCPUnixSSLSocket::GetPath() const { return szPath; }

//
// Get address family
//
TCPSocket::AddressFamily ServerClientTCPUnixSSLSocket::GetAddressFamily() const { return UNIX; }

//
// Destructor
//
ServerClientTCPUnixSSLSocket::~ServerClientTCPUnixSSLSocket() throw()
{
	Close();
	free(szPath);
}

} // namespace IRIS
// End.
