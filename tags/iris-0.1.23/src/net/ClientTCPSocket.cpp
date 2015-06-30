/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#if defined(__FreeBSD__) || defined(sun)
#include <netinet/tcp_fsm.h>
#endif
#include <arpa/inet.h>

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "ClientTCPSocket.hpp"

namespace IRIS
{

//
// Constructor
//
ClientTCPSocket::ClientTCPSocket(const bool bICheckTCPInfo): bCheckTCPInfo(bICheckTCPInfo) { ;; }

//
// Get client IP
//
CCHAR_P ClientTCPSocket::GetPath() const { return NULL; }

//
// Get client IP
//
CCHAR_P ClientTCPSocket::GetIP() const { return NULL; }

//
// Get client Port
//
UINT_32 ClientTCPSocket::GetPort() const { return 0; }

//
// Read data from socket
//
TCPSocket::State ClientTCPSocket::Read(void           * vBuffer,
                                       const UINT_64    iBufferSize,
                                       UINT_64        & iReadBytes)
{
	INT_32 iFlags = 0;
#ifdef MSG_NOSIGNAL
	iFlags = MSG_NOSIGNAL;
#endif // MSG_NOSIGNAL

	iReadBytes = 0;
	ssize_t iRealReadBytes = 0;

	do
	{
		iRealReadBytes = recv(iSocket, vBuffer, iBufferSize, iFlags);
	} while(iRealReadBytes == -1 && errno == EINTR);

	if (iRealReadBytes >= 0)
	{
		iReadBytes = iRealReadBytes;
		return TCPSocket::OK;
	}

	// Error?
	iErrNo = errno;

return TCPSocket::UNIX_ERROR;
}

//
// Write data to the socket
//
TCPSocket::State ClientTCPSocket::Write(const void     * vBuffer,
                                        const UINT_64    iBufferSize,
                                        UINT_64        & iWroteBytes)
{
#ifdef TCP_INFO
	if (bCheckTCPInfo)
	{
		struct tcp_info oTCPInfo;
		socklen_t iTCPInfoLen = sizeof(oTCPInfo);

		if (getsockopt(iSocket, IPPROTO_TCP, TCP_INFO, &oTCPInfo, &iTCPInfoLen) == -1)
		{
			iErrNo = errno;
			return TCPSocket::UNIX_ERROR;
		}
#if defined(__FreeBSD__) || defined(sun)
		if (oTCPInfo.tcpi_state != TCPS_ESTABLISHED)
#else
		if (oTCPInfo.tcpi_state != TCP_ESTABLISHED)
#endif
		{
			iErrNo = EPIPE;
			return TCPSocket::UNIX_ERROR;
		}
	}
#endif

	INT_32 iFlags = 0;
#ifdef MSG_NOSIGNAL
	iFlags = MSG_NOSIGNAL;
#endif // MSG_NOSIGNAL

	ssize_t iTMP = 0;
	do
	{
		iTMP = send(iSocket, (CHAR_P)vBuffer, iBufferSize, iFlags);
	} while(iTMP == -1 && errno == EINTR);

	if (iTMP == -1 || iTMP == 0)
	{
		iErrNo = errno;
		return TCPSocket::UNIX_ERROR;
	}

	iWroteBytes = iTMP;

return TCPSocket::OK;
}

//
// Check socket support SSL/TLS
//
bool ClientTCPSocket::IsSSL() const { return false; }

//
// Destructor
//
ClientTCPSocket::~ClientTCPSocket() throw() { ;; }

} // namespace IRIS
// End.
