/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MAIN_PROCESS_HPP__
#define _MAIN_PROCESS_HPP__ 1

/**
  @file MainProcess.hpp
  @brief Iris main process
*/

#include "Types.h"
#include "SSL.hpp"
#include "STLString.hpp"
#include "STLVector.hpp"

namespace IRIS
{
// FWD
class FileHandleCache;
class GlobalContext;
class ServerTCPSocket;
class SignalHandler;
class Logger;

/**
  @struct MainProcess MainProcess.hpp <MainProcess.hpp>
  @brief Main module
*/
class MainProcess
{
public:
	/**
	  @brief Constructor
	  @param oISigHandler - signal handler
	  @param oIFileHandleCache - file handle cache
	  @param oIGlobalContext - global context object
	*/
	MainProcess(SignalHandler    & oISigHandler,
	            GlobalContext    & oIGlobalContext);

	/**
	  @brief Setup server: create sockets, load modules, etc
	  @param oLogger - temporary logger object
	  @param bLoggerStarted - True, if external logger started and initialized [out]
	  @return 0 - if success, -1 - otherwise
	*/
	INT_32 Setup(Logger  & oLogger,
	             bool    & bLoggerStarted);

	/**
	  @brief Setup network connections
	  @return 0 - if success, -1 - otherwise
	*/
	INT_32 SetupNetwork();

	/**
	  @brief Initialize handlers
	*/
	INT_32 InitHandlers();

	/**
	  @brief Run main process
	  @return 0 - if success, -1 - otherwise
	*/
	INT_32 Run();

	/**
	  @brief Shutdown sockets, etc
	  @return 0 - if success, -1 - otherwise
	*/
	INT_32 Shutdown();

	/**
	  @brief A destructor
	*/
	~MainProcess() throw();
private:
	/**
	  @struct MainSocketContext MainProcess.hpp <MainProcess.hpp>
	  @brief Socket context
	*/
	struct MainSocketContext
	{
		/** TCP socket            */
		ServerTCPSocket  * socket;
		/** Service configuration */
		ServiceConfig    * service_config;
	};

	typedef STLW::vector<MainSocketContext>   Listeners;

	/** Signal handler        */
	SignalHandler               & oSigHandler;
	/** Global context object */
	GlobalContext               & oGlobalContext;
	/** List of sockets       */
	Listeners                     vListeners;
	/** List of modules       */
	STLW::vector<STLW::string>    vModules;

	/**
	  @brief Find file in specified list of directories
	*/
	INT_32 FindFile(const STLW::vector<STLW::string>  & vDirectories,
	                const STLW::string                & sFilename,
	                STLW::string                      & sFullpath);

	/**
	  @brief Load modules
	*/
	INT_32 LoadModule(const STLW::string                & sModuleType,
	                  const STLW::string                & sModuleName,
	                  const STLW::string                & sLibrary,
	                  const STLW::string                & sDriver,
	                  const VariantNC                   & oModuleConfig,
	                  const STLW::vector<STLW::string>  & vLibexecDirs,
	                  Logger                            & oLogger);
};

} // namespace IRIS
#endif // _MAIN_PROCESS_HPP__
// End.
