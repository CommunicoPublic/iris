/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SOCKET_HPP__
#define _SOCKET_HPP__ 1

#include "STLString.hpp"
#include "TCPSocket.hpp"

/**
  @file Socket.hpp
  @brief Generic socket implementation
*/

namespace IRIS
{

// FWD
class ClientTCPSocket;

class SocketBuffer
{
public:
	SocketBuffer(const UINT_32 iIBufferSize = (1 << 16));

	CHAR_8 & operator[](const UINT_32 iPos);

	CHAR_P Data();

	UINT_32 Size() const;

	~SocketBuffer() throw();
private:
	// Does not exist
	SocketBuffer(const SocketBuffer & oRhs);
	SocketBuffer & operator=(const SocketBuffer & oRhs);

	CHAR_P         sBuffer;
	const UINT_32  iBufferSize;
};

/**
  @class Socket Socket.hpp <Socket.hpp>
  @brief IMAP i/o socket routines
*/
class Socket
{
public:
	/**
	  @brief Constructor
	*/
	Socket(SocketBuffer     * pIRSocketBuffer,
	       SocketBuffer     * pIWSocketBuffer,
	       ClientTCPSocket  * pISocket);

	/**
	  Read one line from socket
	*/
	TCPSocket::State ReadLine(STLW::string & sResult);

	/**
	  @brief Read block of data with specified length
	*/
	TCPSocket::State ReadData(const UINT_32    iDataLength,
	                          STLW::string   & sResult);

	Socket & operator<<(const UINT_32 iData);
	Socket & operator<<(const STLW::string & sData);
	Socket & operator<<(CCHAR_P szData);

	Socket & operator>>(const UINT_32 iData);
	Socket & operator>>(const STLW::string & sData);
	Socket & operator>>(CCHAR_P szData);

	Socket & operator>>(Socket & (*f)(Socket & oSocket));
	Socket & operator<<(Socket & (*f)(Socket & oSocket));

	static Socket & endl(Socket & oSocket);
	static Socket & flush(Socket & oSocket);
	static Socket & reset(Socket & oSocket);

	/**
	  @brief A destructor
	*/
	~Socket() throw();

	void Reset();
private:
	// Does not exist
	Socket(const Socket & oRhs);
	Socket & operator=(const Socket & oRhs);

	/** Socket buffer for reading  */
	SocketBuffer     * pRSocketBuffer;
	/** Socket buffer for writing  */
	SocketBuffer     * pWSocketBuffer;

	/** Low level socket   */
	ClientTCPSocket  * pSocket;
	/** Buffer for i/o     */
	CHAR_P             sRBuffer;
	/** Buffer size        */
	UINT_32            iRBufferSize;

	/** Buffer for i/o     */
	CHAR_P             sWBuffer;
	/** Buffer size        */
	UINT_32            iWBufferSize;

	/** Previous search position */
	UINT_32            iRPrevSearchPos;

	/** Position in buffer       */
	UINT_32            iRBufferPos;
	/** Position in buffer       */
	UINT_32            iWBufferPos;

	void CopyData(CCHAR_P        sData,
	              const UINT_32  iDataLen);

};

} // namespace IRIS
#endif // _SOCKET_HPP__
// End.
