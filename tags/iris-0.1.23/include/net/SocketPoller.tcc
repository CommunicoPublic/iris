/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

namespace IRIS
{

//
// Constructor
//
template<typename T>SocketPoller<T>::SocketPoller(const UINT_32 iIMaxSockets): iMaxSockets(iIMaxSockets), iUsedSockets(0), iErrNo(0)
{
	aSockets      = new SocketData[iMaxSockets];
	aReadySockets = new SocketData[iMaxSockets + 1];
	aPollSockets  = new struct pollfd[iMaxSockets];

	for (UINT_32 iPos = 0; iPos < iMaxSockets; ++iPos)
	{
		aSockets[iPos].socket      = NULL;
		aReadySockets[iPos].socket = NULL;
	}

	aReadySockets[iMaxSockets].socket = NULL;
}

//
// Add socket
//
template<typename T>TCPSocket::State SocketPoller<T>::AddSocket(TCPSocket      * pSocket,
                                                                const T        & oIdent,
                                                                const UINT_32    iFlags)
{
	if (iUsedSockets == iMaxSockets) { return TCPSocket::UNIX_ERROR; }

	aSockets[iUsedSockets].ident   = oIdent;
	aSockets[iUsedSockets].socket  = pSocket;
	aSockets[iUsedSockets].flags   = iFlags;

	aPollSockets[iUsedSockets].fd      = pSocket -> GetSocket();
	aPollSockets[iUsedSockets].revents = 0;
	aPollSockets[iUsedSockets].events  = iFlags;

	iUsedSockets++;

return TCPSocket::OK;
}

//
// Poll sockets
//
template<typename T>TCPSocket::State SocketPoller<T>::Poll(const UINT_32    iTimeout)
{
	INT_32 iRC = 0;
	// Timeout, milliseconds
	do
	{
		iRC = poll(aPollSockets, iUsedSockets, iTimeout);
	} while(iRC == -1 && errno == EINTR);

	if (iRC < 0)
	{
		iErrNo = errno;
		return TCPSocket::UNIX_ERROR;
	}

	if (iRC == 0)
	{
		iErrNo = ETIMEDOUT;
		return TCPSocket::TIMEOUT;
	}

	UINT_32 iReadyPos = 0;
	for (UINT_32 iPos = 0; iPos < iUsedSockets; ++ iPos)
	{
		if (aPollSockets[iPos].events & aPollSockets[iPos].revents)
		{
			aReadySockets[iReadyPos] = aSockets[iPos];
			aReadySockets[iReadyPos].flags = aPollSockets[iPos].revents;
			++iReadyPos;
		}
	}
	aReadySockets[iReadyPos].socket = NULL;

	iReadySocketsNum = iReadyPos;

return TCPSocket::OK;
}

//
// Get list of sockets
//
template<typename T> typename SocketPoller<T>::SocketData * SocketPoller<T>::GetReadySockets(UINT_32  & iReadySockets)
{
	iReadySockets = iReadySocketsNum;
return aReadySockets;
}

//
// Get system error
//
template<typename T>INT_32 SocketPoller<T>::GetError() const { return iErrNo; }

//
// A destructor
//
template<typename T>SocketPoller<T>::~SocketPoller() throw()
{
	delete [] aPollSockets;
	delete [] aReadySockets;
	delete [] aSockets;
}

} // namespace IRIS
// End.
