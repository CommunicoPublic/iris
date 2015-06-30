/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

/**
  @file IrisEchoHandler.hpp
  @brief Iris Control Protocol handler
*/

// C++ Includes
#include <GlobalContext.hpp>
#include <HandlerConfig.hpp>
#include <Logger.hpp>
#include <LoggerObject.hpp>

#include "IrisEchoEventWatcher.hpp"
#include "IrisEchoHandler.hpp"

namespace IRIS_MOD
{

//
// A constructor
//
IrisEchoHandler::IrisEchoHandler(): oIrisEchoSignalHandler(oWorkerContext),
                                    pThreadGroup(NULL)
{
	;;
}

//
// A destructor
//
IrisEchoHandler::~IrisEchoHandler() throw()
{
	;;
}

//
// Initialize module
//
INT_32 IrisEchoHandler::InitModule(IRIS::GlobalContext    & oGlobalContext,
                                   const IRIS::VariantNC  & oConfig,
                                   IRIS::SignalHandler    & oSigHandler,
                                   IRIS::Logger           & oLogger)
{
	using namespace IRIS;
	// Create worker context
	oWorkerContext.shutdown = false;
	oWorkerContext.threads  = 0;

	// Create signal handler
	oSigHandler.RegisterHandler(SIGINT,  &oIrisEchoSignalHandler);
	oSigHandler.RegisterHandler(SIGTERM, &oIrisEchoSignalHandler);

	oLogger.Notice("Echo module started");

return 0;
}

//
// Initialize handler
//
INT_32 IrisEchoHandler::InitHandler(IRIS::GlobalContext        & oGlobalContext,
                                    const IRIS::HandlerConfig  & oHandlerConfig,
                                    IRIS::Logger               & oLogger)
{
	// Create thread group
	pThreadGroup = new IrisEchoThreadGroup;

	// Create set of threads
	for (UINT_32 iPos = 0; iPos < oHandlerConfig.start_threads; ++iPos)
	{
		pThreadGroup -> CreateThread(new IrisEchoThreadWorker(oWorkerContext));
		++oWorkerContext.threads;
		oLogger.Notice("Echo thread %d started", oWorkerContext.threads);
	}

	const STLW::string sLogger = oHandlerConfig.logger.empty() ? oGlobalContext.config.logger_type : oHandlerConfig.logger;
	// Get logger from factory for main process
	pLoggerObject = static_cast<IRIS::LoggerObject *>(oGlobalContext.factory.GetObject("Logger/" + sLogger));
	if (pLoggerObject == NULL)
	{
		oLogger.Emerg("Can't get logger `%s` for %s", sLogger.c_str(), GetObjectName());
		return -1;
	}

	oLogger.Notice("Echo module initialized");

return 0;
}

//
// Handle new connection
//
IRIS::NetworkEventWatcher  * IrisEchoHandler::NewConnection(IRIS::AcceptLoopContext  & oAcceptLoopContext,
                                                            IRIS::ServiceConfig      & oServiceConfig,
                                                            IRIS::ClientTCPSocket    * pSocket)
{
	return new IrisEchoEventWatcher(oWorkerContext, oAcceptLoopContext, oServiceConfig, *pLoggerObject, pSocket);
}

//
// Destroy handler
//
INT_32 IrisEchoHandler::DestroyHandler(IRIS::GlobalContext  & oGlobalContext,
                                       IRIS::Logger         & oLogger)
{
	oLogger.Notice("Shutting down Echo handler");

	// Fire up shutdown flag
	oWorkerContext.shutdown = true;
	// Shut down threads
	while (oWorkerContext.threads != 0)
	{
		oLogger.Notice("Stopping echo handler thread %d", oWorkerContext.threads);
		// Send signal
		oWorkerContext.cond.Signal();

		// Wait for thread
		oWorkerContext.shutdown_cond.Wait();
	}

	// Destroy thread pool
	delete pThreadGroup;

	oLogger.Notice("Echo module stopped");
return 0;
}

//
// Destroy module
//
INT_32 IrisEchoHandler::DestroyModule(IRIS::GlobalContext  & oGlobalContext,
                                      IRIS::SignalHandler  & oSigHandler,
                                      IRIS::Logger         & oLogger)
{
	// Create signal handler
	oSigHandler.RemoveHandler(SIGINT,  &oIrisEchoSignalHandler);
	oSigHandler.RemoveHandler(SIGTERM, &oIrisEchoSignalHandler);

	oLogger.Notice("Echo module destroyed");
return 0;
}

//
// Get object type
//
CCHAR_P IrisEchoHandler::GetObjectType() const { return "Handler"; }

//
// Get object name
//
CCHAR_P IrisEchoHandler::GetObjectName() const { return "EchoHandler"; }

} // namespace IRIS_MOD
// End.
