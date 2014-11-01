/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

/**
  @file IrisEchoEventWatcher.hpp
  @brief Iris echo protocol event watcher
*/

// C++ Includes
#include <DataBuffer.hpp>
#include <Logger.hpp>
#include <NetworkEventWatcher.hpp>
#include <NetworkLoggerContext.hpp>
#include <TCPSocket.hpp>

namespace IRIS
{
struct AcceptLoopContext;
struct ServiceConfig;
struct ClientTCPSocket;
class LoggerObject;
} // namespace IRIS

namespace IRIS_MOD
{
class IrisEchoWorkerContext;
/**
  @class IrisEchoEventWatcher IrisEchoEventWatcher.hpp <IrisEchoEventWatcher.hpp>
  @brief Iris echo protocol event watcher
*/
class IrisEchoEventWatcher:
  public IRIS::NetworkEventWatcher
{
public:
	enum IOState { READ, WRITE };

	/**
	  @brief Constructor
	  @param pWorkerContext - thread worker context
	  @param pIContext - Accept loop context
	  @param pIServiceConfig - Service configuration
	  @param pLoggerObject - logger module
	  @param pISocket - Client socket
	*/
	IrisEchoEventWatcher(IrisEchoWorkerContext    * pWorkerContext,
	                     IRIS::AcceptLoopContext  * pIContext,
	                     IRIS::ServiceConfig      * pIServiceConfig,
	                     IRIS::LoggerObject       * pLoggerObject,
	                     IRIS::ClientTCPSocket    * pISocket);

	/**
	  @brief Set critical section flag
	*/
	void SetCritical();

	/**
	  @brief Reset critical section flag
	*/
	void ResetCritical();

	/**
	  @brief Get request data
	*/
	IRIS::DataBuffer &  GetInputDataBuffer();

	/**
	  @brief Store response data
	*/
	IRIS::DataBuffer &  GetOutputDataBuffer();

	/**
	  @brief Get error logger
	*/
	IRIS::Logger * GetErrorLog();

	/**
	  @brief Get transfer logger
	*/
	IRIS::Logger * GetTransferLog();

	/**
	  @brief Get logger context
	*/
	IRIS::NetworkLoggerContext * GetLoggerContext();

	/**
	  @brief Set I/O state
	*/
	void SetIOState(const IOState & eIIOState);

	/**
	  @brief A destructor
	*/
	~IrisEchoEventWatcher() throw();
private:
	// Does not exist
	IrisEchoEventWatcher(const IrisEchoEventWatcher  & oRhs);
	IrisEchoEventWatcher& operator=(const IrisEchoEventWatcher  & oRhs);

	/** Echo worker thread context */
	IrisEchoWorkerContext       * pWorkerContext;
	/** Accept loop context       */
	IRIS::AcceptLoopContext     * pAcceptContext;
	/** Service configuration     */
	IRIS::ServiceConfig         * pServiceConfig;
	/** Client socket             */
	IRIS::ClientTCPSocket       * pSocket;
	/** Logger context            */
	IRIS::NetworkLoggerContext    oLoggerContext;
	/** Error Log                 */
	IRIS::Logger                * pErrorLog;
	/** Custom log                */
	IRIS::Logger                * pTransferLog;
	/** Shutdown flag             */
	bool                          bShutdown;
	/** Temp. buffer              */
	CHAR_8                      * szBuffer;
	/** Input data buffer         */
	IRIS::DataBuffer              oInDataBuffer;
	/** Output data buffer        */
	IRIS::DataBuffer              oOutDataBuffer;
	/** Input data length         */
	UINT_64                       iSentBytes;
	/** Context I/O state         */
	IOState                       eIOState;
	/** Socket state              */
	IRIS::TCPSocket::State        eSocketState;

	/**
	  @brief Start watching for events
	*/
	void StartWatch();

	/**
	  @brief Set events for watcher
	  @param iEvents - list of events
	*/
	void SetEvents(const UINT_32  iEvents);

	/**
	  @brief Watcher callback
	  @param iSocket - socket to watch
	  @param iREvents - list of events
	*/
	void Callback(const INT_32   iSocket,
	              const UINT_32  iREvents);

	/**
	  @brief Input/Output timeout callback
	*/
	void IOTimeout();

	/**
	  @brief Shutdown context
	*/
	void ShutdownContext();
};

} // namespace IRIS_MOD
// End.
