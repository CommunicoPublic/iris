/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "Socket.hpp"

#include "STLSstream.hpp"
#include "ClientTCPSocket.hpp"

#include <stdlib.h>
#include <string.h>

namespace IRIS
{

//
// Constructor
//
SocketBuffer::SocketBuffer(const UINT_32 iIBufferSize): iBufferSize(iIBufferSize) { sBuffer = (CHAR_P)malloc(iBufferSize); }

//
// Get reference to the element of buffer
//
CHAR_8 & SocketBuffer::operator[](const UINT_32 iPos) { return sBuffer[iPos]; }

//
// Get pointer to data
//
CHAR_P SocketBuffer::Data() { return sBuffer; }

//
// Get size of buffer
//
UINT_32 SocketBuffer::Size() const { return iBufferSize; }

//
// Destructor
//
SocketBuffer::~SocketBuffer() throw() { free(sBuffer); }

//
// Constructor
//
Socket::Socket(SocketBuffer     * pIRSocketBuffer,
               SocketBuffer     * pIWSocketBuffer,
               ClientTCPSocket  * pISocket): pRSocketBuffer(pIRSocketBuffer),
                                             pWSocketBuffer(pIWSocketBuffer),
                                             pSocket(pISocket),
                                             sRBuffer(pRSocketBuffer -> Data()),
                                             iRBufferSize(pRSocketBuffer -> Size()),
                                             sWBuffer(pWSocketBuffer -> Data()),
                                             iWBufferSize(pWSocketBuffer -> Size()),
                                             iRPrevSearchPos(0),
                                             iRBufferPos(0),
                                             iWBufferPos(0)
{
	;;
}

//
// Read one line from socket
//
TCPSocket::State Socket::ReadLine(STLW::string & sResult)
{
	bool bLastIsCR = false;

	sResult.erase();
	for (;;)
	{
		// Find "\r?\n"
		for(UINT_32 iPos = iRPrevSearchPos; iPos < iRBufferPos; ++iPos)
		{
			// "\n" ?
			if (sRBuffer[iPos] == '\n')
			{
				UINT_32 iEndPos = iPos;
				// "\r" ?
				if (iPos > 0 && sRBuffer[iPos - 1] == '\r') { --iEndPos; }

				// Special case
				if (bLastIsCR && iPos != 0)
				{
					sResult.append("\r", 1);
					bLastIsCR = false;
				}

				++iPos;

				sResult.append(sRBuffer + iRPrevSearchPos, iEndPos - iRPrevSearchPos);

				iRPrevSearchPos = iPos;
				return TCPSocket::OK;
			}
		}

		// No free space in buffer, flush
		if (iRBufferPos == iRBufferSize)
		{
			if (sRBuffer[iRBufferPos - 1] == '\r') { bLastIsCR = true; --iRBufferPos; }
			sResult.append(sRBuffer + iRPrevSearchPos, iRBufferPos - iRPrevSearchPos);
			iRBufferPos     = 0;
			iRPrevSearchPos = 0;
		}

		// Read data, if "\r?\n" not found
		UINT_64 iReadBytes = 0;
		pSocket -> Read(sRBuffer + iRBufferPos, iRBufferSize - iRBufferPos, iReadBytes);

		// EOF?
		if (iReadBytes == 0) { break; }
		iRBufferPos += iReadBytes;
	}

return TCPSocket::END_OF_DATA;
}

//
// Read block of data with specified length
//
TCPSocket::State Socket::ReadData(const UINT_32    iDataLength,
                                  STLW::string   & sResult)
{
	INT_32 iNeedToRead = iDataLength;
	sResult.erase();
	for (;;)
	{
		const INT_32 iAvailableBytes = iRBufferPos - iRPrevSearchPos;
		if (iAvailableBytes > iNeedToRead)
		{
			sResult.append(sRBuffer + iRPrevSearchPos, iNeedToRead);
			iRPrevSearchPos += iNeedToRead;
			return TCPSocket::OK;
		}

		if (iRBufferPos == iRBufferSize)
		{
			sResult.append(sRBuffer + iRPrevSearchPos, iAvailableBytes);

			iNeedToRead -= iAvailableBytes;

			iRBufferPos     = 0;
			iRPrevSearchPos = 0;
		}

		// Read data
		UINT_64 iReadBytes = 0;
		pSocket -> Read(sRBuffer + iRBufferPos, iRBufferSize - iRBufferPos, iReadBytes);

		// EOF?
		if (iReadBytes == 0) { break; }
		iRBufferPos += iReadBytes;
	}

return TCPSocket::END_OF_DATA;
}

//
// Store unsigned integer
//
Socket & Socket::operator<<(const UINT_32 iData)
{
	STLW::stringstream sStream;
	sStream << iData;

	const STLW::string sData = sStream.str();

	CopyData(sData.data(), sData.size());

return *this;
}

//
// Store string
//
Socket & Socket::operator<<(const STLW::string & sData)
{
	CopyData(sData.data(), sData.size());

return *this;
}

//
// Store ASCIZ string
//
Socket & Socket::operator<<(CCHAR_P szData)
{
	CopyData(szData, strlen(szData));

return *this;
}

//
// Reset data buffer
//
void Socket::Reset()
{
	iWBufferPos = 0;
}

//
// Send command to server
//
Socket & Socket::endl(Socket & oSocket)
{
	oSocket.sWBuffer[oSocket.iWBufferPos++] = '\r';
	oSocket.sWBuffer[oSocket.iWBufferPos++] = '\n';

return oSocket;
}

//
// Send command to server
//
Socket & Socket::flush(Socket & oSocket)
{
//	oSocket.pSocket -> Write(oSocket.sWBuffer, oSocket.iWBufferPos);

	// Reset buffer
	oSocket.Reset();

return oSocket;
}

//
// Send command to server
//
Socket & Socket::reset(Socket & oSocket)
{
	// Reset buffer
	oSocket.Reset();

return oSocket;
}

//
// Adapter for stream-like manipulators
//
Socket & Socket::operator<<(Socket & (*f)(Socket & oSocket)) { return f(*this); }

//
// Copy data and write to socket
//
void Socket::CopyData(CCHAR_P        sData,
                      const UINT_32  iDataLen)
{
	UINT_32 iBytes = iWBufferSize - iWBufferPos;
	if (iBytes > iDataLen)
	{
		memcpy(sWBuffer + iWBufferPos, sData, iDataLen);
		iWBufferPos += iDataLen;
	}
	else
	{
//		pSocket -> Write(sWBuffer, iWBufferPos);
//		pSocket -> Write(sData, iDataLen);
		iWBufferPos = 0;
	}
}

//
// A destructor
//
Socket::~Socket() throw() { ;; }

} // namespace IRIS
// End.
