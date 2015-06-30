/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include <sys/types.h>
#include <sys/socket.h>

#include <errno.h>

#include "ServerTCPSSLSocket.hpp"

namespace IRIS
{

//
// Constructor
//
ServerTCPSSLSocket::ServerTCPSSLSocket(const bool  bIStartTLS): bStartTLS(bIStartTLS) { ;; }

//
// Check socket support SSL/TLS
//
bool ServerTCPSSLSocket::IsSSL() const { return true; }

//
// Destructor
//
ServerTCPSSLSocket::~ServerTCPSSLSocket() throw()
{
	;;
}

} // namespace IRIS
// End.
