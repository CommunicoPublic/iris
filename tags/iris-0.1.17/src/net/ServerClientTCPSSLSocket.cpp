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

#include "ServerClientTCPSSLSocket.hpp"

namespace IRIS
{

//
// Constructor
//
ServerClientTCPSSLSocket::ServerClientTCPSSLSocket(const bool      bIStartTLS,
                                                   const INT_32    iISocket):
                                                                              pSSLContext(NULL),
                                                                              bStartTLS(bIStartTLS),
                                                                              pSSL(NULL),
                                                                              bHandshaked(false),
                                                                              bRenegotiate(false)
{
	iSocket = iISocket;
}

//
// Set SSL context
//
void ServerClientTCPSSLSocket::SetSSLContext(SSL_CTX  * pISSLContext)
{
	pSSLContext = pISSLContext;

	if (bStartTLS) { StartTLS(); }
}

//
// Open socket
//
TCPSocket::State ServerClientTCPSSLSocket::Open()
{
	;;
return OK;
}

//
// Check SSL was handshaked
//
bool ServerClientTCPSSLSocket::Handshaked() const { return bHandshaked; }

//
// Start client-requested connection renegatiation
//
void ServerClientTCPSSLSocket::Renegotiate() { bRenegotiate = true; }

//
// Switch to secure layer
//
void ServerClientTCPSSLSocket::StartTLS()
{
	if (pSSL != NULL || pSSLContext == NULL) { return; }

	pSSL = SSL_new(pSSLContext);

	long iMode = SSL_MODE_ENABLE_PARTIAL_WRITE;
#ifdef SSL_MODE_RELEASE_BUFFERS
	iMode |= SSL_MODE_RELEASE_BUFFERS;
#endif
	SSL_set_mode(pSSL, iMode);
	SSL_set_accept_state(pSSL);
	SSL_set_fd(pSSL, iSocket);
	SSL_set_app_data(pSSL, this);
}

//
// Switch to secure layer
//
void ServerClientTCPSSLSocket::StopTLS()
{
	if (pSSL == NULL) { return; }

	SSL_set_shutdown(pSSL, SSL_SENT_SHUTDOWN);
	SSL_free(pSSL);
	pSSL = NULL;
}

//
// Start SSL handshake
//
TCPSocket::State ServerClientTCPSSLSocket::DoSSLHandshake()
{
	// Do SSL handshake
	bHandshaked = false;

	const INT_32 iHandshake = SSL_do_handshake(pSSL);
	if (iHandshake == 1)
	{
		bHandshaked = true;
		return OK;
	}

	const INT_32 iHandshakeError = SSL_get_error(pSSL, iHandshake);

	// Need to read data from socket
	if (iHandshakeError == SSL_ERROR_WANT_READ) { return NEED_READ; }


	// Need to write data to socket
	if (iHandshakeError == SSL_ERROR_WANT_WRITE) { return NEED_WRITE; }

	StopTLS();
	Close();

	// Connection closed (in handshake)
	if (iHandshakeError == SSL_ERROR_ZERO_RETURN) { return END_OF_DATA; }

// Unexpected SSL error (in handshake)
return UNIX_ERROR;
}

//
// Read from socket
//
TCPSocket::State ServerClientTCPSSLSocket::Read(void           * vBuffer,
                                                const UINT_64    iBufferSize,
                                                UINT_64        & iReadBytes)
{
	// No SSL
	if (pSSL == NULL) { return ClientTCPSSLSocket::Read(vBuffer, iBufferSize, iReadBytes); }

	// Do SSL handshake
	if (!bHandshaked)
	{
		TCPSocket::State oRC = DoSSLHandshake();
		if (oRC == OK) { return NEED_READ; }

		return oRC;
	}

	// Read data
	const INT_32 iSSLReadBytes = SSL_read(pSSL, vBuffer, iBufferSize);

	// Fix CVE-2009-3555. Disable renegotiation if started by client.
	if (bRenegotiate)
	{
		StopTLS();
		Close();
		return UNIX_ERROR;
	}

	// All done
	if (iSSLReadBytes > 0)
	{
		iReadBytes = iSSLReadBytes;
		return OK;
	}

	// Check error
	const INT_32 iSSLError = SSL_get_error(pSSL, iSSLReadBytes);

	// Wait for socket and write SSL data
	if (iSSLError == SSL_ERROR_WANT_WRITE) { return NEED_WRITE; }

	// Wait for socket and read SSL data
	if (iSSLError == SSL_ERROR_WANT_READ) { return NEED_READ; }

	// Stop TLS and close socket
	StopTLS();
	Close();

	// Connection closed
	if (iSSLError == SSL_ERROR_ZERO_RETURN) { return END_OF_DATA; }

// Any other error
return UNIX_ERROR;
}

//
// Write to socket
//
TCPSocket::State ServerClientTCPSSLSocket::Write(const void     * vBuffer,
                                                 const UINT_64    iBufferSize,
                                                 UINT_64        & iWroteBytes)
{
	// No SSL
	if (pSSL == NULL) { return ClientTCPSSLSocket::Write(vBuffer, iBufferSize, iWroteBytes); }

	// Do SSL handshake
	if (!bHandshaked)
	{
		TCPSocket::State oRC = DoSSLHandshake();
		if (oRC == OK) { return NEED_WRITE; }

		return oRC;
	}

	// Write data
	const INT_32 iSSLWroteBytes = SSL_write(pSSL, vBuffer, iBufferSize);

	// All done
	if (iSSLWroteBytes > 0)
	{
		iWroteBytes = iSSLWroteBytes;
		return OK;
	}

	const INT_32 iSSLError = SSL_get_error(pSSL, iSSLWroteBytes);

	// Wait for socket and write SSL data
	if (iSSLError == SSL_ERROR_WANT_WRITE) { return NEED_WRITE; }

	// Wait for socket and read SSL data
	if (iSSLError == SSL_ERROR_WANT_READ) { return NEED_READ; }

	// Handle fatal SSL error
	StopTLS();
	Close();

	// Connection closed
	if (iSSLError == SSL_ERROR_ZERO_RETURN) { return END_OF_DATA; }

return UNIX_ERROR;
}

//
// Close socket
//
TCPSocket::State ServerClientTCPSSLSocket::Close()
{
	if (iSocket == -1) { return OK; }

	StopTLS();

	close(iSocket);
	iSocket = -1;

return OK;
}

//
// Destructor
//
ServerClientTCPSSLSocket::~ServerClientTCPSSLSocket() throw()
{
	Close();
}

} // namespace IRIS
// End.
