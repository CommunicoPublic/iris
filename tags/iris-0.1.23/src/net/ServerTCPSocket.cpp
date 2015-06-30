/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include <sys/types.h>
#include <sys/socket.h>

#include <errno.h>

#include "ServerTCPSocket.hpp"

namespace IRIS
{

//
// Constructor
//
ServerTCPSocket::ServerTCPSocket() { ;; }

//
// Listen for connections
//
TCPSocket::State ServerTCPSocket::Listen(const INT_32 iBacklog)
{
	// Listen for connections on a socket
	if (listen(iSocket, iBacklog) == -1)
	{
		SetError(errno);
		return UNIX_ERROR;
	}
return OK;
}

//
// Check socket support SSL/TLS
//
bool ServerTCPSocket::IsSSL() const { return false; }

//
// Destructor
//
ServerTCPSocket::~ServerTCPSocket() throw()
{
	;;
}

} // namespace IRIS
// End.
