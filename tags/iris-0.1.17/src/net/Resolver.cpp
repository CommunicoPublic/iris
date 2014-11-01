/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>
#include <netdb.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Resolver.hpp"

namespace IRIS
{

//
// Resolve host name
//
#if defined(__linux__) || defined(__OpenBSD__) || defined(__NetBSD__)
Resolver::State Resolver::Resolve4Hostname(CHAR_P szHost, CHAR_P & szIP)
{
	struct addrinfo    oHints;
	struct addrinfo  * aRes;
	memset(&oHints, 0, sizeof(oHints));

	// TODO is that correct? it will work only with numeric names
	oHints.ai_flags    = 0;//AI_NUMERICSERV | AI_NUMERICHOST;
	oHints.ai_socktype = SOCK_STREAM;
	oHints.ai_family   = AF_INET;

	if (getaddrinfo(szHost, "imap", &oHints, &aRes) != 0) { return UNIX_ERROR; }

	CHAR_8 sAddress[INET6_ADDRSTRLEN];

	struct sockaddr_in * pSockAddr = (struct sockaddr_in *)(aRes -> ai_addr);
	inet_ntop(AF_INET, &(pSockAddr -> sin_addr), sAddress, sizeof(sAddress));
	szIP = strdup(sAddress);

	freeaddrinfo(aRes);

return OK;
}
#else
Resolver::State Resolver::Resolve4Hostname(CHAR_P szHost, CHAR_P & szIP)
{
	// Is the IP address?
	if (inet_addr(szHost) != (in_addr_t)(-1))
	{
		szIP = strdup(szHost);
		return OK;
	}

	int iErrNo = 0;
	struct hostent * oHostEnt = getipnodebyname(szHost, PF_INET, AI_DEFAULT, &iErrNo);
	if (oHostEnt == NULL) { return UNIX_ERROR; }

	szIP = strdup(inet_ntoa(*(struct in_addr *)(oHostEnt -> h_addr)));

	freehostent(oHostEnt);

return OK;
}
#endif

//
// Resolve host name
//
#if defined(__linux__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
Resolver::State Resolver::Resolve6Hostname(CHAR_P szHost, CHAR_P & szIP)
{
	// Is the IP address?
	struct sockaddr_in6 oSockAddr;
	if (inet_pton(PF_INET6, szHost, &oSockAddr.sin6_addr) != 0)
	{
		szIP = strdup(szHost);
		return OK;
	}

	struct addrinfo    oHints;
	struct addrinfo  * aRes;
	memset(&oHints, 0, sizeof(oHints));

	// TODO is that correct? it will work only with numeric names
	oHints.ai_flags    = AI_V4MAPPED | AI_ADDRCONFIG;//AI_NUMERICSERV | AI_NUMERICHOST;
	oHints.ai_socktype = SOCK_STREAM;
	oHints.ai_family   = AF_INET6;

	INT_32 iRC = getaddrinfo(szHost, NULL, &oHints, &aRes);
	if (iRC != 0) { return UNIX_ERROR; }

	CHAR_8 sAddress[INET6_ADDRSTRLEN];

	struct sockaddr_in6 * pSockAddr = (struct sockaddr_in6 *)(aRes -> ai_addr);
	inet_ntop(AF_INET6, &(pSockAddr -> sin6_addr), sAddress, sizeof(sAddress));

	szIP = strdup(sAddress);

	freeaddrinfo(aRes);

return OK;
}

#else
Resolver::State Resolver::Resolve6Hostname(CHAR_P szHost, CHAR_P & szIP)
{
	// Is the IP address?
	struct sockaddr_in6 oSockAddr;
	if (inet_pton(PF_INET6, szHost, &oSockAddr.sin6_addr) != 0)
	{
		szIP = strdup(szHost);
		return OK;
	}

	int iErrNo = 0;
	struct hostent * oHostEnt = getipnodebyname(szHost, PF_INET6, AI_DEFAULT, &iErrNo);
	if (oHostEnt == NULL) { return UNIX_ERROR; }

	CHAR_8 sBuffer[64];
	// 0000:0000:0000:0000:0000:0000:0000:0000
	if (inet_ntop(PF_INET6, *(struct in_addr *)(oHostEnt -> h_addr), sBuffer, 64) == NULL)
	{
		freehostent(oHostEnt);
		return UNIX_ERROR;
	}

	szIP = strdup(sBuffer);

	freehostent(oHostEnt);

return OK;
}
#endif

} // namespace IRIS
// End.
