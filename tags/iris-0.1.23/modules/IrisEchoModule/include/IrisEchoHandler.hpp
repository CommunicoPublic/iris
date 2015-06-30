/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _IRIS_ECHO_HANDLER_HPP__
#define _IRIS_ECHO_HANDLER_HPP__ 1

/**
  @file IrisEchoHandler.hpp
  @brief Iris Control Protocol handler
*/

// C++ Includes
#include <LoadableObject.hpp>
#include <NetworkHandler.hpp>
#include <ThreadGroup.hpp>
#include <Variant.hpp>

#include "IrisEchoThreadWorker.hpp"
#include "IrisEchoSignalHandler.hpp"
#include "IrisEchoWorkerContext.hpp"

namespace IRIS
{
// FWD
class AcceptLoopContext;
class ClientTCPSocket;
class GlobalContext;
class HandlerConfig;
class Logger;
class LoggerObject;
class NetworkEventWatcher;
class ServiceConfig;
} // namespace IRIS

/**
  @namespace IRIS_MOD
  @brief Iris handler namespace
*/
namespace IRIS_MOD
{

/**
  @class IrisEchoHandler IrisEchoHandler.cpp <IrisEchoHandler.cpp>
  @brief Loadable object; echo protocol handler
*/
class IrisEchoHandler:
 public IRIS::NetworkHandler
{
public:
	/**
	  @brief A constructor
	*/
	IrisEchoHandler();

	/**
	  @brief A destructor
	*/
	~IrisEchoHandler() throw();
private:
	// Does not exist
	IrisEchoHandler(const IrisEchoHandler  & oRhs);
	IrisEchoHandler& operator=(const IrisEchoHandler  & oRhs);

	typedef IRIS::ThreadGroup<IrisEchoThreadWorker> IrisEchoThreadGroup;

	/** Thread worker context      */
	IrisEchoWorkerContext    oWorkerContext;
	/** Signal handler             */
	IrisEchoSignalHandler    oIrisEchoSignalHandler;
	/** Logger object              */
	IRIS::LoggerObject     * pLoggerObject;
	/** Group of threads-executors */
	IrisEchoThreadGroup    * pThreadGroup;

	/**
	  @brief Initialize module
	  @param oGlobalContext - global server context
	  @param oConfig - configuration data
	  @param oSigHandler - signal handler
	  @param oLogger - temporary logger
	*/
	INT_32 InitModule(IRIS::GlobalContext    & oGlobalContext,
	                  const IRIS::VariantNC  & oConfig,
	                  IRIS::SignalHandler    & oSigHandler,
	                  IRIS::Logger           & oLogger);

	/**
	  @brief Initialize handler
	  @param oGlobalContext - global execution context
	  @param oHandlerConfig - handler configuration data
	  @param oLogger - logger object
	*/
	INT_32 InitHandler(IRIS::GlobalContext        & oGlobalContext,
	                   const IRIS::HandlerConfig  & oHandlerConfig,
	                   IRIS::Logger               & oLogger);

	/**
	  @brief Handle new connection
	  @param pContext - Thread loop context
	  @param pServiceConfig - Service configuration
	  @param pSocket - Client TCP connection
	  @return Pointer to NetworkEventWatcher object
	*/
	IRIS::NetworkEventWatcher  * NewConnection(IRIS::AcceptLoopContext  & oContext,
	                                           IRIS::ServiceConfig      & oServiceConfig,
	                                           IRIS::ClientTCPSocket    * pSocket);

	/*
	  @brief Destroy handler
	  @param oGlobalContext - global execution context
	  @param oLogger - logger object
	  @return 0 - if success, -1 - otherwise
	*/
	INT_32 DestroyHandler(IRIS::GlobalContext  & oGlobalContext,
	                      IRIS::Logger         & oLogger);

	/**
	  @brief Destroy module
	  @param oGlobalContext - global execution context
	  @param oLogger - logger object
	  @return 0 - if success, -1 - otherwise
	*/
	INT_32 DestroyModule(IRIS::GlobalContext  & oGlobalContext,
	                     IRIS::SignalHandler  & oSigHandler,
	                     IRIS::Logger         & oLogger);

	/**
	  @brief Get object type
	  @return Human-readable type of object
	*/
	CCHAR_P GetObjectType() const;

	/**
	  @brief Get object name
	  @return Human-readable name of object
	*/
	CCHAR_P GetObjectName() const;
};

EXPORT_HANDLER(IrisEchoHandler)

} // namespace IRIS_MOD
#endif // _IRIS_ECHO_HANDLER_HPP__
// End.
