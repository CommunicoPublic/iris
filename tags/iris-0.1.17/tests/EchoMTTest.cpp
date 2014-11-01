/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include <ev.h>

#include "EventLoop.hpp"
#include "IOAsyncWatcher.hpp"
#include "IOWatcher.hpp"
#include "Mutex.hpp"
#include "ThreadGroup.hpp"
#include "STLQueue.hpp"
#include "STLString.hpp"
#include "STLSstream.hpp"

#ifndef C_NETWORK_THREADS
#define C_NETWORK_THREADS 10
#endif // C_NETWORK_THREADS

namespace IRIS
{
//
// Create new listener
//
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

// FWD
struct EVThreadContext;

class EVAsyncWatcher:
  public IOAsyncWatcher
{
public:
	/**
	  @brief Constructor
	  @param pIContext - context
	*/
	EVAsyncWatcher(EVThreadContext  * pIContext);

	/**
	  @brief Watcher callback
	  @param iSocket - socket to watch
	  @param iEvents - list of events
	*/
	void Callback(const UINT_32  iREvents);

	/**
	  @brief A destructor
	*/
	~EVAsyncWatcher() throw();
private:
	// Does not exist
	EVAsyncWatcher(const EVAsyncWatcher  & oRhs);
	EVAsyncWatcher& operator=(const EVAsyncWatcher  & oRhs);

	EVThreadContext  * pContext;
};


struct EVMainContext
{
	/** Thread context mutex     */
	Mutex                  mutex;
	/** Main loop                */
	EventLoop              main_loop;
	/** Client sockets           */
	STLW::queue<INT_32>    client_sockets;
};

/**
  @class EVThreadContext EchoTest.cpp <EchoTest.cpp>
  @brief Event thread context
*/
struct EVThreadContext
{
public:
	/** Main context */
	EVMainContext     & main_context;
	/** Thread loop              */
	EventLoop           thread_loop;
	/** Event watcher            */
	EVAsyncWatcher      event_watcher;
	/** Number of active clients */
	UINT_32             clients;

	/**
	  @brief Constructor
	*/
	EVThreadContext(EVMainContext  & oMainContext);
private:
	friend class EVThreadWorker;
	// Does not exist
	EVThreadContext(const EVThreadContext  & oRhs);
	EVThreadContext& operator=(const EVThreadContext  & oRhs);

	/**
	  @brief A destructor
	*/
	~EVThreadContext() throw();
};

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
	ClientWatcher(EVThreadContext  * pIContext);

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
	/** Thread context */
	EVThreadContext  * pContext;

	/** Flag, set to true, if all request data readed  */
	bool bRequestOK;
	/** Data buffer    */
	STLW::string       sDataBuffer;
};


//
// Constructor
//
EVAsyncWatcher::EVAsyncWatcher(EVThreadContext  * pIContext): IOAsyncWatcher(&(pIContext -> thread_loop)),
                                                              pContext(pIContext)
{
	StartWatch();
}

//
// Watcher callback
//
void EVAsyncWatcher::Callback(const UINT_32  iREvents)
{
	MutexLocker oLocker(pContext -> main_context.mutex);
fprintf(stderr, "EVAsyncWatcher::Callback: %d\n", INT_32(pContext -> main_context.client_sockets.size()));
	while (!pContext -> main_context.client_sockets.empty())
	{
		++(pContext -> clients);
		const INT_32 iClientSocket = pContext -> main_context.client_sockets.front();
		pContext -> main_context.client_sockets.pop();

		ClientWatcher * pClientWatcher = new ClientWatcher(pContext);
		pClientWatcher -> SetEvents(iClientSocket, IOWatcher::READ | IOWatcher::WRITE);
		pClientWatcher -> StartWatch();
	}
}

//
// A destructor
//
EVAsyncWatcher::~EVAsyncWatcher() throw() { ;; }

//
// Constructor
//
EVThreadContext::EVThreadContext(EVMainContext  & oMainContext): main_context(oMainContext),
                                                                 event_watcher(this)
{
	;;
}

//
// Destructor
//
EVThreadContext::~EVThreadContext() throw() { ;; }

//
// Constructor
//
ClientWatcher::ClientWatcher(EVThreadContext  * pIContext): IOWatcher(&(pIContext -> thread_loop)),
                                                            pContext(pIContext) { ;; }

//
// Watcher callback
//
void ClientWatcher::Callback(const INT_32   iSocket,
                             const UINT_32  iREvents)
{
	if (iREvents & IOWatcher::ERROR)
	{
		fprintf(stderr, "ClientWatcher::ERROR: IOWatcher::ERROR/Closing socket %d\n", iSocket);
		StopWatch();
		close(iSocket);

		MutexLocker oLocker(pContext -> main_context.mutex);
		--(pContext -> clients);
		delete this;
		return;
	}

	if (iREvents & IOWatcher::READ)
	{
		CHAR_8 szBuffer[10];
		const ssize_t iReadBytes = recv(iSocket, szBuffer, 10, 0);
		if (iReadBytes <= 0)
		{
			fprintf(stderr, "ClientWatcher::ERROR: IOWatcher::READ/Closing socket %d\n", iSocket);
			StopWatch();
			close(iSocket);

			MutexLocker oLocker(pContext -> main_context.mutex);
			--(pContext -> clients);
			delete this;
			return;
		}
//		fprintf(stderr, "Read %d byte(s)\n", INT_32(iReadBytes));
		sDataBuffer.append(szBuffer, iReadBytes);

		bRequestOK = (sDataBuffer.find("\r\n\r\n") != STLW::string::npos);
	}

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
			           "Content-Type: text/html\r\n"
			           "Connection: close\r\n"
			           "Content-Length: " << sDataBuffer.size() << "\r\n\r\n"
			        << sDataBuffer;

			sDataBuffer = sStream.str();
			/*const ssize_t iSentBytes = */send(iSocket, sDataBuffer.data(), sDataBuffer.size(), MSG_DONTWAIT);
			sDataBuffer.erase();
			bRequestOK = false;

			StopWatch();
			close(iSocket);
fprintf(stderr, "Closing socket %d\n", iSocket);
			MutexLocker oLocker(pContext -> main_context.mutex);
			--(pContext -> clients);
			delete this;
			return;
		}

		SetEvents(iSocket, IOWatcher::READ | IOWatcher::WRITE);
	}
}

//
// Destructor
//
ClientWatcher::~ClientWatcher() throw() { ;; }

/**
  @class EVThreadWorker EchoTest.cpp <EchoTest.cpp>
  @brief Event thread worker
*/
class EVThreadWorker
{
public:
	/**
	  @brief Constructor
	*/
	EVThreadWorker(EVThreadContext * pIContext,
	               const INT_32      iICPUNum = -1);

	/**
	  @brioef Execute job
	*/
	void Run() throw();

	/**
	  @brief A destructor
	*/
	~EVThreadWorker() throw();
private:
	// Does not exist
	EVThreadWorker(const EVThreadWorker  & oRhs);
	EVThreadWorker& operator=(const EVThreadWorker  & oRhs);

	/** Thread context     */
	EVThreadContext   * pContext;
	/** CPU Number         */
	const INT_32        iCPUNum;
};

//
// Constructor
//
EVThreadWorker::EVThreadWorker(EVThreadContext  * pIContext,
                               const INT_32       iICPUNum): pContext(pIContext),
                                                             iCPUNum(iICPUNum) { ;; }

//
// Execute job
//
void EVThreadWorker::Run() throw()
{
	if (iCPUNum >= 0)
	{
		fprintf(stderr, "Thread started. Binding to CPU#%d\n", iCPUNum);
#if (linux)
		const INT_32 iMaxCores = sysconf(_SC_NPROCESSORS_ONLN);
		if (iCPUNum > iMaxCores) { return; }
		pthread_t oCurrentThread = pthread_self();

#if(__FreeBSD__)
		cpuset_t * oCPUSet = cpuset_create();
		cpuid_t iCPUId = iCPUNum;
		cpuset_set(iCPUId, oCPUSet);
		const INT_32 iRC = pthread_setaffinity_np(oCurrentThread, sizeof(cpu_set_t), &oCPUSet);
		cpuset_destroy(oCPUSet);
#elif(linux) // __FreeBSD__
		cpu_set_t oCPUSet;
		CPU_ZERO(&oCPUSet);
		CPU_SET(iCPUNum, &oCPUSet);
		const INT_32 iRC = pthread_setaffinity_np(oCurrentThread, sizeof(oCPUSet), &oCPUSet);
#endif // linux

		if (iRC != 0) { fprintf(stderr, "FAILED\n"); }
#endif // linux || __FreeBSD__
	}
	else
	{
		fprintf(stderr, "Thread started.\n");
	}

	// Start loop
	pContext -> thread_loop.Loop();
	fprintf(stderr, "Thread loop end\n");
}

//
// Destructor
//
EVThreadWorker::~EVThreadWorker() throw()
{
	delete pContext;
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
	AcceptWatcher(EVMainContext                    & oMainContext,
	              STLW::vector<EVThreadContext *>  & vIContextes);

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
	// Does not exist
	AcceptWatcher(const AcceptWatcher  & oRhs);
	AcceptWatcher& operator=(const AcceptWatcher  & oRhs);

	/** Main Context      */
	EVMainContext                    & oMainContext;
	/** List of contextes */
	STLW::vector<EVThreadContext *>  & vContextes;
};

//
// Constructor
//
AcceptWatcher::AcceptWatcher(EVMainContext                    & oIMainContext,
                             STLW::vector<EVThreadContext *>  & vIContextes): IOWatcher(&oIMainContext.main_loop),
                                                                              oMainContext(oIMainContext),
                                                                              vContextes(vIContextes) { ;; }

//
// Watcher callback
//
void AcceptWatcher::Callback(const INT_32   iSocket,
                             const UINT_32  iREvents)
{
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
	fprintf(stderr, "\nAccepted a client %d\n", iClientSocket);

	int iTMP = 1;
	setsockopt(iClientSocket, SOL_SOCKET, SO_REUSEADDR, &iTMP, sizeof(int));
#ifdef SO_REUSEPORT
	iTMP = 1;
	setsockopt(iClientSocket, SOL_SOCKET, SO_REUSEPORT, &iTMP, sizeof(int));
#endif
	iTMP = 1;
	setsockopt(iClientSocket, SOL_SOCKET, SO_KEEPALIVE, &iTMP, sizeof(int));

	struct linger sLinger;
	sLinger.l_onoff  = 1;
	sLinger.l_linger = 2;
	setsockopt(iSocket, SOL_SOCKET, SO_LINGER, &sLinger, sizeof(struct linger));

	INT_32 iFlags = fcntl(iClientSocket, F_GETFL);
	iFlags |= O_NONBLOCK;
	fcntl(iClientSocket, F_SETFL, iFlags);

	// Put socket to the queue
	MutexLocker oLocker(oMainContext.mutex);
	oMainContext.client_sockets.push(iClientSocket);

	// Find most underloaded thread
	UINT_32 iMinClients    = 0;
	UINT_32 iMinClientsPos = 0;
fprintf(stderr, "Slots: ");
	for (UINT_32 iPos = 0; iPos < vContextes.size(); ++iPos)
	{
		EVThreadContext * pContext = vContextes[iPos];
fprintf(stderr, "%d ", pContext -> clients);
		if (iMinClients >= pContext -> clients)
		{
			iMinClients = pContext -> clients;
			iMinClientsPos = iPos;
		}
	}
fprintf(stderr, "; ok on slot %d\n", iMinClientsPos);

	// Notify thread
	EVThreadContext * pContext = vContextes[iMinClientsPos];
	pContext -> event_watcher.SendSignal();
}

//
// Destructor
//
AcceptWatcher::~AcceptWatcher() throw() { ;; }

} // namespace IRIS

#ifndef C_NETWORK_THREADS
#define C_NETWORK_THREADS 10
#endif // C_NETWORK_THREADS

int main(void)
{
	using namespace IRIS;

	// Create server socket
	INT_32 iServerSocket = MakeSocket();
	if (iServerSocket == -1) { return EX_SOFTWARE; }

	INT_32 iProcNum = 4;
#ifdef _SC_NPROCESSORS_ONLN
	iProcNum = sysconf(_SC_NPROCESSORS_ONLN);
	fprintf(stderr, "Found %d CPU(s)\n", iProcNum);
#endif
	// Create group of threads
	ThreadGroup<EVThreadWorker> oThreadGroup;

	// Create main context
	EVMainContext  oMainContext;
	// Contextes for thread watchers
	STLW::vector<EVThreadContext *> vContextes;
	for(INT_32 iPos = 0; iPos < iProcNum; ++iPos)
	{
		EVThreadContext * pContext = new EVThreadContext(oMainContext);
		vContextes.push_back(pContext);
		oThreadGroup.CreateThread(new EVThreadWorker(pContext, iPos));
	}

	// Create accept watcher
	AcceptWatcher oAcceptWatcher(oMainContext, vContextes);

	// Start waitching for server socket
	oAcceptWatcher.SetEvents(iServerSocket, IOWatcher::READ);
	oAcceptWatcher.StartWatch();
	oMainContext.main_loop.Loop();

	close(iServerSocket);

return EX_OK;
}
// End.

