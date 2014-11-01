/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>
//#include <sys/cpuset.h>
#include <ev.h>

#include "EventLoop.hpp"
#include "IOWatcher.hpp"
#include "STLString.hpp"
#include "STLSstream.hpp"
namespace IRIS
{

/**
  @class ClientWatcher EchoTest.cpp <EchoTest.cpp>
  @brief Client events watcher
*/
class ClientWatcher:
  public IOWatcher
{
public:
	/**
	  @brief Constructor
	  @param pIEventLoop - event loop object
	*/
	ClientWatcher(EventLoop  * pEventLoop);

	/**
	  @brief Watcher callback
	  @param iSocket - socket to watch
	  @param iEvents - list of events
	*/
	void Callback(const INT_32   iSocket,
	              const UINT_32  iREvents);

	/**
	  @brief A destructor
	*/
	~ClientWatcher() throw();
private:
	STLW::string    sDataBuffer;
	bool            bRequestOK;
};

//
// Constructor
//
ClientWatcher::ClientWatcher(EventLoop  * pEventLoop): IOWatcher(pEventLoop) { ;; }

//
// Watcher callback
//
void ClientWatcher::Callback(const INT_32   iSocket,
                             const UINT_32  iREvents)
{
	if (iREvents & IOWatcher::READ)
	{
		CHAR_8 szBuffer[10];
		const ssize_t iReadBytes = recv(iSocket, szBuffer, 10, 0);
		if (iReadBytes <= 0)
		{
			if (iReadBytes < 0) { fprintf(stderr, "Callback/recv() failed errno=%i (%s)\n",  errno, strerror(errno)); }

			fprintf(stderr, "Closing socket %d\n", iSocket);
			StopWatch();
			close(iSocket);
			delete this;
			return;
		}
//		fprintf(stderr, "Read %d byte(s)\n", INT_32(iReadBytes));
		sDataBuffer.append(szBuffer, iReadBytes);

		bRequestOK = (sDataBuffer.find("\r\n\r\n") != STLW::string::npos);
	}
/*
	if (sDataBuffer.empty()) { SetEvents(iSocket, IOWatcher::READ); }
	else
	{
		if (iREvents & IOWatcher::WRITE)
		{
			const ssize_t iSentBytes = send(iSocket, sDataBuffer.data(), sDataBuffer.size(), MSG_DONTWAIT);
			fprintf(stderr, "Sent %d byte(s)\n", INT_32(iSentBytes));
			sDataBuffer.erase();
		}

		SetEvents(iSocket, IOWatcher::READ | IOWatcher::WRITE);
	}
*/

	if (!bRequestOK)
	{
		SetEvents(iSocket, IOWatcher::READ);
	}
	else
	{
		if (iREvents & IOWatcher::WRITE)
		{
			STLW::stringstream sStream;
			sStream << "HTTP/1.1 200 OK\r\n"
			           "Server: libev\r\n"
			           "Content-Type: text/plain\r\n"
			           "Connection: close\r\n"
//			           "Content-Length: " << sDataBuffer.size() << "\r\n"
			           "\r\n"
			        << sDataBuffer;

			sDataBuffer = sStream.str();
			const ssize_t iSentBytes = send(iSocket, sDataBuffer.data(), sDataBuffer.size(), MSG_DONTWAIT);
fprintf(stderr, "Wrote %d\n", INT_32(iSentBytes));
			sDataBuffer.erase();
			bRequestOK = false;
			StopWatch();
//			shutdown(iSocket, SHUT_RDWR);
			close(iSocket);
fprintf(stderr, "Closing socket %d\n", iSocket);

			delete this;
			return;
		}

		SetEvents(iSocket, IOWatcher::READ | IOWatcher::WRITE);
	}
}

//
// Destructor
//
ClientWatcher::~ClientWatcher() throw()
{
	;;
}

/**
  @class ClientWatcher EchoTest.cpp <EchoTest.cpp>
  @brief Server socket events watcher
*/
class AcceptWatcher:
  public IOWatcher
{
public:
	/**
	  @brief Constructor
	  @param pIEventLoop - event loop object
	*/
	AcceptWatcher(EventLoop  * pEventLoop);

	/**
	  @brief Watcher callback
	  @param iSocket - socket to watch
	  @param iEvents - list of events
	*/
	void Callback(const INT_32   iSocket,
	              const UINT_32  iREvents);

	/**
	  @brief A destructor
	*/
	~AcceptWatcher() throw();
private:
};

//
// Constructor
//
AcceptWatcher::AcceptWatcher(EventLoop  * pEventLoop): IOWatcher(pEventLoop) { ;; }

//
// Watcher callback
//
void AcceptWatcher::Callback(const INT_32   iSocket,
                             const UINT_32  iREvents)
{
//fprintf(stderr, "Callback socket %d events %d\n", iSocket, iREvents);
	if (iREvents != IOWatcher::READ) { return; }

	INT_32 iClientSocket = accept(iSocket, NULL, NULL);
	if (iClientSocket == -1 )
	{
		if( errno != EAGAIN && errno != EWOULDBLOCK)
		{
			fprintf(stderr, "accept() failed errno=%i (%s)",  errno, strerror(errno));
			exit(EX_SOFTWARE);
		}
		return;
	}
fprintf(stderr, "Accepted a client %d\n", iClientSocket);

	int iTMP = 1;
	setsockopt(iClientSocket, SOL_SOCKET, SO_REUSEADDR, &iTMP, sizeof(int));
#ifdef SO_REUSEPORT
	iTMP = 1;
	setsockopt(iClientSocket, SOL_SOCKET, SO_REUSEPORT, &iTMP, sizeof(int));
#endif
	iTMP = 1;
	setsockopt(iClientSocket, SOL_SOCKET, SO_KEEPALIVE, &iTMP, sizeof(int));

	INT_32 iFlags = fcntl(iClientSocket, F_GETFL);
	iFlags |= O_NONBLOCK;
	fcntl(iClientSocket, F_SETFL, iFlags);

	struct linger sLinger;
	sLinger.l_onoff  = 1;
	sLinger.l_linger = 2;
	setsockopt(iSocket, SOL_SOCKET, SO_LINGER, &sLinger, sizeof(struct linger));

	ClientWatcher * pClientWatcher = new ClientWatcher(pEventLoop);
	pClientWatcher -> SetEvents(iClientSocket, IOWatcher::READ | IOWatcher::WRITE);
	pClientWatcher -> StartWatch();
}

static INT_32 MakeSocket()
{
	// Setup a unix socket listener.
	INT_32 iSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (iSocket == -1)
	{
		fprintf(stderr, "socket: %d %s\n", errno, strerror(errno));
		return -1;
	}

	INT_32 iFlags = fcntl(iSocket, F_GETFL);
	iFlags |= O_NONBLOCK;
	fcntl(iSocket, F_SETFL, iFlags);

	struct sockaddr_in oSockAddr;
	oSockAddr.sin_family = AF_INET;
	oSockAddr.sin_port = htons(10001);
	oSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(iSocket, (struct sockaddr *) &oSockAddr, sizeof (oSockAddr)) < 0)
	{
		fprintf(stderr, "bind: %d %s\n", errno, strerror(errno));
		return -1;
	}

	if (listen(iSocket, 128) < 0)
	{
		fprintf(stderr, "listen: %d %s\n", errno, strerror(errno));
		return -1;
	}

	int iTMP = 1;
	setsockopt(iSocket, SOL_SOCKET, SO_REUSEADDR, &iTMP, sizeof(int));
#ifdef SO_REUSEPORT
	iTMP = 1;
	setsockopt(iSocket, SOL_SOCKET, SO_REUSEPORT, &iTMP, sizeof(int));
#endif
	iTMP = 1;
	setsockopt(iSocket, SOL_SOCKET, SO_KEEPALIVE, &iTMP, sizeof(int));

	struct linger sLinger;
	sLinger.l_onoff  = 1;
	sLinger.l_linger = 2;
	setsockopt(iSocket, SOL_SOCKET, SO_LINGER, &sLinger, sizeof(struct linger));

	fprintf(stderr, "MakeSocket: %d\n", iSocket);
return iSocket;
}

//
// Destructor
//
AcceptWatcher::~AcceptWatcher() throw() { ;; }

} // namespace IRIS

int main(void)
{
	using namespace IRIS;
	EventLoop  oLoop;
	AcceptWatcher oAcceptWatcher(&oLoop);
	INT_32 iServerSocket = MakeSocket();
	if (iServerSocket == -1) { return EX_SOFTWARE; }

	oAcceptWatcher.SetEvents(iServerSocket, IOWatcher::READ);
	oAcceptWatcher.StartWatch();
	oLoop.Loop();

	close(iServerSocket);

return EX_OK;
}
// End.
