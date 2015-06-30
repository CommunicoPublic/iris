/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include <AcceptLoopContext.hpp>
#include <ClientTCPSocket.hpp>
#include <Debug.hpp>
#include <LoggerObject.hpp>
#include <ServiceConfig.hpp>

#include "IrisEchoEventWatcher.hpp"
#include "IrisEchoWorkerContext.hpp"

namespace IRIS_MOD
{

//
// Constructor
//
IrisEchoEventWatcher::IrisEchoEventWatcher(IrisEchoWorkerContext    & oIWorkerContext,
                                           IRIS::AcceptLoopContext  & oIAcceptLoopContext,
                                           IRIS::ServiceConfig      & oIServiceConfig,
                                           IRIS::LoggerObject       & oLoggerObject,
                                           IRIS::ClientTCPSocket    * pISocket): NetworkEventWatcher(oIAcceptLoopContext, oIServiceConfig),
                                                                                 oWorkerContext(oIWorkerContext),
                                                                                 pSocket(pISocket),
                                                                                 bShutdown(false),
                                                                                 iSentBytes(0),
                                                                                 eIOState(READ)
{
	szBuffer = new CHAR_8[oServiceConfig.recv_buffer_size];

	oLoggerContext.SetParam("client.port", INT_64(pSocket -> GetPort()));
	oLoggerContext.SetParam("client.ip",   pSocket -> GetIP());

	// Error log
	pErrorLog    = oLoggerObject.GetErrorLog(&oLoggerContext);
	// Custom log
	pTransferLog = oLoggerObject.GetCustomLog(&oLoggerContext);

	// Client connected
	pTransferLog -> Notice("Client connected");

}

//
// Start watching for events
//
void IrisEchoEventWatcher::StartWatch()
{
	if (eIOState == READ) { SetEvents(IRIS::IOWatcher::READ);  }
	else                  { SetEvents(IRIS::IOWatcher::WRITE); }
}

//
// Set events for watcher
//
void IrisEchoEventWatcher::SetEvents(const UINT_32  iEvents)
{
	using namespace IRIS;

	IOWatcher::StopWatch();
	IOWatcher::SetEvents(pSocket -> GetSocket(), iEvents);
	IOWatcher::StartWatch();

	oAcceptLoopContext.ActivateWatcher(this, oServiceConfig.io_timeout);
}

//
// Watcher callback
//
void IrisEchoEventWatcher::Callback(const INT_32   iSocket,
                                    const UINT_32  iREvents)
{
	using namespace IRIS;

	// Handle error
	if (iREvents & IOWatcher::ERROR)
	{
		pErrorLog -> Error("Internal error, closing connection");
		ShutdownContext();
		return;
	}

	// Inactivate timeout watcher
	oAcceptLoopContext.InactivateWatcher(this);

	// Read data from socket
	if (eIOState == READ && (((iREvents & IOWatcher::WRITE) && (eSocketState == TCPSocket::NEED_WRITE)) ||
	                          (iREvents & IOWatcher::READ)))
	{
		// Read data from socket
		UINT_64 iReadBytes = 0;
		eSocketState = pSocket -> Read(szBuffer, oServiceConfig.recv_buffer_size, iReadBytes);
		if (eSocketState == TCPSocket::NEED_READ)
		{
			SetEvents(IOWatcher::READ);
			return;
		}
		else if (eSocketState == TCPSocket::NEED_WRITE)
		{
			SetEvents(IOWatcher::WRITE);
			return;
		}
		else if (eSocketState != TCPSocket::OK)
		{
			pErrorLog -> Error("Can't read data from socket: %s", Debug::PrintableError(pSocket -> GetError()).c_str());
			ShutdownContext();
			return;
		}

		// Client closed connection
		if (eSocketState == TCPSocket::OK && iReadBytes == 0)
		{
			ShutdownContext();
			return;
		}

		// Append data to the buffer
		oInDataBuffer.Append(szBuffer, iReadBytes);

		// Stop watching for events
		IOWatcher::StopWatch();

		// Handle work in separated thread
		// Send task to handler
		oWorkerContext.mutex.Lock();
		SetCritical();
		IrisEchoWorkerContext::Task oTask  = { oAcceptLoopContext, *this };
		oWorkerContext.task_queue.push(oTask);
		oWorkerContext.mutex.Unlock();
		// Notify thread
		oWorkerContext.cond.Signal();

		iSentBytes = 0;
	}

	// Write data to the socket
	if (eIOState == WRITE && (((iREvents & IOWatcher::READ) && (eSocketState == TCPSocket::NEED_READ)) ||
	                           (iREvents & IOWatcher::WRITE)))
	{
		UINT_64 iBytesLeft = oOutDataBuffer.Size() - iSentBytes;
		if (iBytesLeft != 0)
		{
			UINT_64 iWroteNow = 0;
			CCHAR_P vData     = oOutDataBuffer.Data() + iSentBytes;

			eSocketState = pSocket -> Write(vData, iBytesLeft, iWroteNow);
			if (eSocketState == TCPSocket::NEED_READ)
			{
				SetEvents(IOWatcher::READ);
				return;
			}
			else if (eSocketState == TCPSocket::NEED_WRITE)
			{
				SetEvents(IOWatcher::WRITE);
				return;
			}
			else if (eSocketState != TCPSocket::OK)
			{
				pErrorLog -> Error("Can't write data to socket: %s", Debug::PrintableError(pSocket -> GetError()).c_str());
				ShutdownContext();
				return;
			}

			iSentBytes += iWroteNow;
			iBytesLeft = oOutDataBuffer.Size() - iSentBytes;

			// Re-activate timeout watcher
			oAcceptLoopContext.ActivateWatcher(this, oServiceConfig.io_timeout);
		}

		// Clear garbage after all jobs are completed
		if (iBytesLeft != 0)
		{
			SetEvents(IOWatcher::WRITE);
		}
		else
		{
			oInDataBuffer.Clear();
			oOutDataBuffer.Clear();

			eIOState = READ;
			SetEvents(IOWatcher::READ);
		}
	}
}

//
// Get request data
//
IRIS::DataBuffer &  IrisEchoEventWatcher::GetInputDataBuffer() { return oInDataBuffer; }

//
// Store response data
//
IRIS::DataBuffer &  IrisEchoEventWatcher::GetOutputDataBuffer() { return oOutDataBuffer; }

//
// Input/Output timeout callback
//
void IrisEchoEventWatcher::IOTimeout()
{
	pErrorLog -> Warn("Timeout reached, closing connection");
	ShutdownContext();
}

//
// Set critical section flag
//
INT_32 IrisEchoEventWatcher::SetCritical() { return NetworkEventWatcher::SetCritical(); }

//
// Reset critical section flag
//
INT_32 IrisEchoEventWatcher::ResetCritical()
{
	using namespace IRIS;

	const INT_32 iCriticalSections = NetworkEventWatcher::ResetCritical() == 0;
	bool bInCritical = iCriticalSections != 0;

	if (bShutdown && !bInCritical)
	{
		// Inactivate timeout watcher
		oAcceptLoopContext.InactivateWatcher(this);

		// Remove context
		AcceptLoopContext::EventEntry oEventEntry = { this, IOWatcher::REMOVE };
		oAcceptLoopContext.AddEvent(oEventEntry);
	}
return iCriticalSections;
}

//
// Shutdown context
//
void IrisEchoEventWatcher::ShutdownContext()
{
	using namespace IRIS;

	// Inactivate timeout watcher
	oAcceptLoopContext.InactivateWatcher(this);

	// On first call we need to remove context from clients list
	if (!bShutdown) { bShutdown = true; }

	// Check critical state
	// If we can delete context, do it
	if (!InCritical())
	{
		AcceptLoopContext::EventEntry oEventEntry = { this, IOWatcher::REMOVE };
		oAcceptLoopContext.AddEvent(oEventEntry);
	}
}

//
// Get error logger
//
IRIS::Logger * IrisEchoEventWatcher::GetErrorLog() { return pErrorLog; }

//
// Get transfer logger
//
IRIS::Logger * IrisEchoEventWatcher::GetTransferLog() { return pTransferLog; }

//
// Get logger context
//
IRIS::NetworkLoggerContext * IrisEchoEventWatcher::GetLoggerContext() { return &oLoggerContext; }

//
// Set I/O state
//
void IrisEchoEventWatcher::SetIOState(const IrisEchoEventWatcher::IOState &  eIIOState) { eIOState = eIIOState; }

//
// A destructor
//
IrisEchoEventWatcher::~IrisEchoEventWatcher() throw()
{
	IOWatcher::StopWatch();
	pSocket -> Close();

	delete [] szBuffer;
	delete pSocket;

	delete pErrorLog;
	delete pTransferLog;
}

} // namespace IRIS_MOD
// End.
