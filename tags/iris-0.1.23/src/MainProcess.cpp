/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "AcceptLoopContext.hpp"
#include "AcceptLoopWorker.hpp"
#include "ClientTCPSocket.hpp"
#include "Debug.hpp"
#include "FileUtils.hpp"
#include "GlobalContext.hpp"
#include "Logger.hpp"
#include "LoggerObject.hpp"
#include "LoadableObject.hpp"

#include "MainLoopContext.hpp"
#include "MainLoopEventWatcher.hpp"
#include "MainLoopSignalHandler.hpp"

#include "MainProcess.hpp"
#include "NetworkHandler.hpp"
#include "NetworkThreadWorker.hpp"
#include "ServerTCP4Socket.hpp"
#include "ServerTCP6Socket.hpp"
#include "ServerTCPUnixSocket.hpp"

#ifdef IRIS_TLS_SUPPORT
    #include "ServerTCP4SSLSocket.hpp"
    #include "ServerTCP6SSLSocket.hpp"
    #include "ServerTCPUnixSSLSocket.hpp"
#endif

#include "SocketPoller.hpp"
#include "ThreadGroup.hpp"
#include "UnicodeUtils.hpp"

namespace IRIS
{

//
// Constructor
//
MainProcess::MainProcess(SignalHandler    & oISigHandler,
                         GlobalContext    & oIGlobalContext): oSigHandler(oISigHandler),
                                                              oGlobalContext(oIGlobalContext)
{
	;;
}

//
// Find file in specified list of directories
//
INT_32 MainProcess::FindFile(const STLW::vector<STLW::string>  & vDirectories,
                             const STLW::string                & sFilename,
                             STLW::string                      & sFullpath)
{
	STLW::vector<STLW::string>::const_iterator itvDirectories = vDirectories.begin();
	while (itvDirectories != vDirectories.end())
	{
		sFullpath.erase();
		const STLW::string & sDir(*itvDirectories);
		if (sDir.size())
		{
			sFullpath.assign(sDir);
			if (sDir[sDir.size() - 1] != '/') { sFullpath.append(1, '/'); }
		}

		sFullpath.append(sFilename);

		const INT_64 iFileHandle = File::Open(sFullpath.c_str(), OpenMode::READ);
		if (iFileHandle != -1)
		{
			File::Close(iFileHandle);
			return 0;
		}

		++itvDirectories;
	}
return -1;
}

//
// Load modules
//
INT_32 MainProcess::LoadModule(const STLW::string                & sModuleType,
                               const STLW::string                & sModuleName,
                               const STLW::string                & sLibrary,
                               const STLW::string                & sDriver,
                               const VariantNC                   & oModuleConfig,
                               const STLW::vector<STLW::string>  & vLibexecDirs,
                               Logger                            & oLogger)
{
	oLogger.Info("Opening `%s/%s` from library `%s`", sModuleType.c_str(), sModuleName.c_str(), sLibrary.c_str());
	STLW::string sFullpath;
	INT_32 iRC = FindFile(vLibexecDirs, sLibrary, sFullpath);
	if (iRC == -1)
	{
		STLW::string sDirList;
		STLW::vector<STLW::string>::const_iterator itvLibexecDirs = vLibexecDirs.begin();
		for (;;)
		{
			sDirList += "`" + *itvLibexecDirs + "` ";
			++itvLibexecDirs;
			if (itvLibexecDirs == vLibexecDirs.end()) { break; }
			sDirList += ", ";
		}
		oLogger.Emerg("Can't find library `%s` in LibexecDirs(%s) ", sLibrary.c_str(), sDirList.c_str());
		return -1;
	}
	oLogger.Info("Library `%s` found here: `%s`", sLibrary.c_str(), sFullpath.c_str());

	Object * pObject = oGlobalContext.loader.GetObject(sFullpath.c_str(), sDriver.c_str());
	if (pObject == NULL)
	{
		oLogger.Emerg("Can't load object `%s` from file `%s`", sDriver.c_str(), sFullpath.c_str());
		return -1;
	}

	// Check type of object
	const STLW::string sObjectType = pObject -> GetObjectType();
	if (Unicode::CompareIgnoreCase(sObjectType.data(), sObjectType.size(), sModuleType.data(), sModuleType.size()) != 0)
	{
		oLogger.Emerg("Need type `%s`, but object `%s` loaded from file `%s` has type `%s`", sModuleType.c_str(), pObject -> GetObjectName(), sFullpath.c_str(), pObject -> GetObjectType());
		delete pObject;
		return -1;
	}

	oLogger.Info("Object `%s` with type `%s` loaded from file `%s`", pObject -> GetObjectName(), pObject -> GetObjectType(), sFullpath.c_str());

	// Initialize module
	iRC = static_cast<Module *>(pObject) -> InitModule(oGlobalContext, oModuleConfig, oSigHandler, oLogger);
	if (iRC != 0)
	{
		oLogger.Emerg("Can't initialize module `%s` from file `%s`", sModuleName.c_str(), sFullpath.c_str());
		delete pObject;
		return -1;
	}

	const STLW::string sFullName = sModuleType + '/' + sModuleName;
	vModules.push_back(sFullName);
	// Store object in factory
	if(oGlobalContext.factory.AddObject(sFullName, pObject) == NULL)
	{
		oLogger.Emerg("Can't add module `%s` from file `%s`", sModuleName.c_str(), sFullpath.c_str());
		static_cast<Module *>(pObject) -> DestroyModule(oGlobalContext, oSigHandler, oLogger);
		delete pObject;
		return -1;
	}

return 0;
}

//
// Setup server: create sockets, load modules, etc
//
INT_32 MainProcess::Setup(Logger  & oLogger,
                          bool    & bLoggerStarted)
{
	// Initialize loggers, before of all other modules
	oLogger.Info("Starting logger subsystem");
	STLW::vector<ModuleConfig>::const_iterator itvLoggers = oGlobalContext.config.loggers.begin();
	while (itvLoggers != oGlobalContext.config.loggers.end())
	{
		const INT_32 iRC = LoadModule(itvLoggers -> type,
		                              itvLoggers -> name,
		                              itvLoggers -> library,
		                              itvLoggers -> driver,
		                              itvLoggers -> params,
		                              oGlobalContext.config.libexec_dirs,
		                              oLogger);
		if (iRC != 0) { return iRC; }
		oLogger.Info("Logger `%s` started", itvLoggers -> name.c_str());
		++itvLoggers;
	}

	// Get logger from factory for main process
	Object * pObject = oGlobalContext.factory.GetObject("Logger/" + oGlobalContext.config.logger_type);
	if (pObject == NULL)
	{
		oLogger.Emerg("Can't get main process logger `%s`", oGlobalContext.config.logger_type.c_str());
		return -1;
	}

	oLogger.Info("Logger subsystem started");

	// Start logger subsystem
	oGlobalContext.error_log  = static_cast<LoggerObject *>(pObject) -> GetErrorLog(NULL);
	oGlobalContext.error_log -> Notice("Log started");

	oGlobalContext.transfer_log = static_cast<LoggerObject *>(pObject) -> GetCustomLog(NULL);
	oGlobalContext.transfer_log -> Notice("Log started");

	bLoggerStarted = true;

	// Load modules
	oGlobalContext.error_log -> Info("Starting modules");
	STLW::vector<ModuleConfig>::const_iterator itvModules = oGlobalContext.config.modules.begin();
	while(itvModules != oGlobalContext.config.modules.end())
	{
		const INT_32 iRC = LoadModule(itvModules -> type,
		                              itvModules -> name,
		                              itvModules -> library,
		                              itvModules -> driver,
		                              itvModules -> params,
		                              oGlobalContext.config.libexec_dirs,
		                             *oGlobalContext.error_log);
		if (iRC != 0) { return iRC; }

		++itvModules;
	}
	oGlobalContext.error_log -> Info("All modules started");

return 0;
}

//
// Initialize handlers
//
INT_32 MainProcess::InitHandlers()
{
	// Load handlers
	oGlobalContext.error_log -> Info("Starting handlers");
	STLW::vector<HandlerConfig>::const_iterator itvHandlers = oGlobalContext.config.handlers.begin();
	while (itvHandlers != oGlobalContext.config.handlers.end())
	{
		const INT_32 iRC = LoadModule(itvHandlers -> type,
		                              itvHandlers -> name,
		                              itvHandlers -> library,
		                              itvHandlers -> driver,
		                              itvHandlers -> params,
		                              oGlobalContext.config.libexec_dirs,
		                              *oGlobalContext.error_log);
		if (iRC != 0) { return iRC; }

		oGlobalContext.error_log -> Info("Handler `%s` started", itvHandlers -> name.c_str());

		++itvHandlers;
	}

	// Initialize handlers
	itvHandlers = oGlobalContext.config.handlers.begin();
	while (itvHandlers != oGlobalContext.config.handlers.end())
	{
		// Initialize handler
		Handler * pHandler = static_cast<Handler *>(oGlobalContext.factory.GetObject(itvHandlers -> type + '/' + itvHandlers -> name));
		const INT_32 iRC = pHandler -> InitHandler(oGlobalContext, *itvHandlers, *oGlobalContext.error_log);
		if (iRC != 0) { return iRC; }

		oGlobalContext.error_log -> Info("Handler `%s` initialized", itvHandlers -> name.c_str());

		++itvHandlers;
	}
	oGlobalContext.error_log -> Info("All handlers started");

	oGlobalContext.error_log -> Info("Adding handlers to listeners");

	STLW::vector<ServiceConfig>::iterator itvServices = oGlobalContext.config.services.begin();
	while (itvServices != oGlobalContext.config.services.end())
	{
		// Get handler from factory
		Object * pObject = oGlobalContext.factory.GetObject("Handler/" + itvServices -> handler_name);
		if (pObject == NULL)
		{
			oGlobalContext.error_log -> Emerg("Can't find handler `%s`", itvServices -> handler_name.c_str());
			return -1;
		}
		itvServices -> handler = pObject;
		++itvServices;
	}
	oGlobalContext.error_log -> Info("Adding handlers to listeners finished");

return 0;
}

//
// Setup network connections
//
INT_32 MainProcess::SetupNetwork()
{
	// Create enetwork listeners
	oGlobalContext.error_log -> Info("Creating listeners");

	STLW::vector<ServiceConfig>::iterator itvServices = oGlobalContext.config.services.begin();
	while (itvServices != oGlobalContext.config.services.end())
	{
		MainProcess::MainSocketContext oSocketContext;
		oSocketContext.service_config = &(*itvServices);

		itvServices -> handler = NULL;

		STLW::vector<ServiceConfig::ListenInterface>::const_iterator itvListen = itvServices -> listen.begin();
		while(itvListen != itvServices -> listen.end())
		{
			STLW::stringstream sAddressStream;
			// Create socket
			switch(itvListen -> af)
			{
				case TCPSocket::INET:
					sAddressStream << "tcp:" << itvListen -> address << ":" << itvListen -> port;
					oGlobalContext.error_log -> Notice("Listening service `%s` with handler `%s` on `tcp:%s:%d`", itvServices -> name.c_str(),
					                                                                                              itvServices -> handler_name.c_str(),
					                                                                                              itvListen -> address.c_str(),
					                                                                                              itvListen -> port);
#ifdef IRIS_TLS_SUPPORT
					if (itvServices -> enable_ssl_tls)
					{
						oSocketContext.socket = new ServerTCP4SSLSocket(itvServices -> start_tls,
						                                                itvListen -> address.c_str(),
						                                                itvListen -> port);
					}
					else
#endif // IRIS_TLS_SUPPORT
					{
						oSocketContext.socket = new ServerTCP4Socket(itvListen -> address.c_str(), itvListen -> port);
					}

					break;

				case TCPSocket::INET6:
					sAddressStream << "tcp6:" << itvListen -> address << ":" << itvListen -> port;
					oGlobalContext.error_log -> Notice("Listening service `%s` with handler `%s` on `tcp6:%s:%d`", itvServices -> name.c_str(),
					                                                                                               itvServices -> handler_name.c_str(),
					                                                                                               itvListen -> address.c_str(),
					                                                                                               itvListen -> port);
#ifdef IRIS_TLS_SUPPORT
					if (itvServices -> enable_ssl_tls)
					{
						oSocketContext.socket = new ServerTCP6SSLSocket(itvServices -> start_tls,
						                                                itvListen -> address.c_str(),
						                                                itvListen -> port);
					}
					else
#endif // IRIS_TLS_SUPPORT
					{
						oSocketContext.socket = new ServerTCP6Socket(itvListen -> address.c_str(), itvListen -> port);
					}

					break;

				case TCPSocket::UNIX:
					sAddressStream << "unix:" << itvListen -> address << ":" << itvListen -> port;
					oGlobalContext.error_log -> Notice("Listening service `%s` with handler `%s` on `unix:%s:%04o`", itvServices -> name.c_str(),
					                                                                                                 itvServices -> handler_name.c_str(),
					                                                                                                 itvListen -> address.c_str(),
					                                                                                                 itvListen -> port);
#ifdef IRIS_TLS_SUPPORT
					if (itvServices -> enable_ssl_tls)
					{
						oSocketContext.socket = new ServerTCPUnixSSLSocket(itvServices -> start_tls,
						                                                   itvListen -> address.c_str(),
						                                                   itvListen -> port);
					}
					else
#endif // IRIS_TLS_SUPPORT
					{
						oSocketContext.socket = new ServerTCPUnixSocket(itvListen -> address.c_str(), itvListen -> port);
					}
					break;
			}

			if (oSocketContext.socket == NULL)
			{
				oGlobalContext.error_log -> Emerg("Can't create socket `%s`: %s", sAddressStream.str().c_str());
				return -1;
			}
			else
			{
				TCPSocket::State oRC = oSocketContext.socket -> Open();
				if (oRC != TCPSocket::OK)
				{
					oGlobalContext.error_log -> Emerg("Can't open listener on `%s`: %s",
					                                  sAddressStream.str().c_str(),
					                                  Debug::PrintableError(oSocketContext.socket -> GetError()).c_str());
					delete oSocketContext.socket;
					return -1;
				}

				oSocketContext.socket -> SetMode(TCPSocket::NON_BLOCKING);
				oSocketContext.socket -> SetMode(TCPSocket::CLOSE_ON_EXEC);

				oSocketContext.socket -> SetFlags(TCPSocket::KEEPALIVE, 1);
				oSocketContext.socket -> SetFlags(TCPSocket::REUSE_ADDRESS, 1);

				oRC = oSocketContext.socket -> Bind();
				if (oRC != TCPSocket::OK)
				{
					oGlobalContext.error_log -> Emerg("Can't bind to interface `%s`: %s",
					                                  sAddressStream.str().c_str(),
					                                  Debug::PrintableError(oSocketContext.socket -> GetError()).c_str());
					delete oSocketContext.socket;
					return -1;
				}

				oRC = oSocketContext.socket -> Listen(itvServices -> listen_queue);
				if (oRC != TCPSocket::OK)
				{
					oGlobalContext.error_log -> Emerg("Can't listen on `%s`: %s",
					                                  sAddressStream.str().c_str(),
					                                  Debug::PrintableError(oSocketContext.socket -> GetError()).c_str());
					delete oSocketContext.socket;
					return -1;
				}

				vListeners.push_back(oSocketContext);
			}
			++itvListen;
		}
		++itvServices;
	}

return 0;
}

//
// Run main process
//
INT_32 MainProcess::Run()
{
	// Create main event context
	MainLoopContext  oMainContext(oGlobalContext);

	// Check number of listeners
	Listeners::iterator itvListeners = vListeners.begin();
	if (itvListeners == vListeners.end())
	{
		oGlobalContext.error_log -> Emerg("No listeners found");
		return -1;
	}

	// Find smallest timeout
	oMainContext.timeout_check_interval = itvListeners -> service_config -> io_timeout;
	while(itvListeners != vListeners.end())
	{
		if (oMainContext.timeout_check_interval > itvListeners -> service_config -> io_timeout)
		{
			oMainContext.timeout_check_interval = itvListeners -> service_config -> io_timeout;
		}
		++itvListeners;
	}

	// Need double precision
	if (oMainContext.timeout_check_interval != -1) { oMainContext.timeout_check_interval /= 2; }
	oGlobalContext.error_log -> Info("Timeout check interval set to %f sec", oMainContext.timeout_check_interval);

	// Contextes for thread watchers
	STLW::vector<AcceptLoopContext *> vContextes;

	// Shutdown accept sockets when SIGTERM catched and shutdown all sockets when SIGINT catched
	MainLoopSignalHandler oMainLoopSignalHandler(oMainContext, vContextes);
	oSigHandler.RegisterHandler(SIGINT,  &oMainLoopSignalHandler);
	oSigHandler.RegisterHandler(SIGTERM, &oMainLoopSignalHandler);

	// Create group of threads with network listeners
	ThreadGroup<AcceptLoopWorker> oThreadGroup;
	// Create threads
	for(UINT_32 iPos = 0; iPos < oGlobalContext.config.network_threads; ++iPos)
	{
		AcceptLoopContext * pAcceptLoopContext = new AcceptLoopContext(oMainContext);
		vContextes.push_back(pAcceptLoopContext);

		INT_32 iCPUNum = -1;
		if (oGlobalContext.config.bind_network_threads_to_cpu) { iCPUNum = iPos; }
		oThreadGroup.CreateThread(new AcceptLoopWorker(pAcceptLoopContext, iCPUNum));
	}

	// Create watcher one per each server socket
	STLW::vector<MainLoopEventWatcher *> vWatchers;
	itvListeners = vListeners.begin();
	while(itvListeners != vListeners.end())
	{
		vWatchers.push_back(new MainLoopEventWatcher(oMainContext,
		                                             vContextes,
		                                             itvListeners -> socket,
		                                             itvListeners -> service_config));
		++itvListeners;
	}

	// Wait for events
	oGlobalContext.error_log -> Info("Staring main loop");
	oMainContext.main_loop.Loop();
	oGlobalContext.error_log -> Info("Shutting down all watchers");

	// Clear garbage and exit
	STLW::vector<MainLoopEventWatcher *>::iterator itvWatchers = vWatchers.begin();
	while (itvWatchers != vWatchers.end())
	{
		delete *itvWatchers;
		++itvWatchers;
	}

	// Remove signal handlers
	oSigHandler.RemoveHandler(SIGINT,  &oMainLoopSignalHandler);
	oSigHandler.RemoveHandler(SIGTERM, &oMainLoopSignalHandler);

	oGlobalContext.error_log -> Info("Waiting for network threads");
return 0;
}

//
// Shutdown sockets, etc
//
INT_32 MainProcess::Shutdown()
{
	oGlobalContext.error_log -> Info("Shutting down listeners");
	Listeners::iterator itvListeners = vListeners.begin();
	while(itvListeners != vListeners.end())
	{
		delete itvListeners -> socket;
		++itvListeners;
	}

	// Destroy all handlers
	oGlobalContext.error_log -> Info("Stopping handlers");
	STLW::vector<HandlerConfig>::const_reverse_iterator itvHandlers = oGlobalContext.config.handlers.rbegin();
	while (itvHandlers != oGlobalContext.config.handlers.rend())
	{
		// Stop handler
		Handler * pHandler = static_cast<Handler *>(oGlobalContext.factory.GetObject(itvHandlers -> type + '/' + itvHandlers -> name));
		if (pHandler == NULL)
		{
			oGlobalContext.error_log -> Info("Can't stop handler `%s`", itvHandlers -> name.c_str());
		}
		else
		{
			oGlobalContext.error_log -> Info("Stopping handler `%s`", itvHandlers -> name.c_str());
			INT_32 iRC = pHandler -> DestroyHandler(oGlobalContext, *oGlobalContext.error_log);
			if (iRC != 0) { return iRC; }

			oGlobalContext.error_log -> Info("Handler `%s` stopped", itvHandlers -> name.c_str());
		}

		++itvHandlers;
	}
	oGlobalContext.error_log -> Info("All handlers stopped");

	// Destroy all modules
	oGlobalContext.error_log -> Info("Stopping modules");
	STLW::vector<STLW::string>::const_reverse_iterator itvModules = vModules.rbegin();
	while (itvModules != vModules.rend())
	{
		oGlobalContext.error_log -> Info("Stopping module `%s`", itvModules -> c_str());

		// Stop module
		Module * pModule = static_cast<Module *>(oGlobalContext.factory.GetObject(*itvModules));
		INT_32 iRC = pModule -> DestroyModule(oGlobalContext, oSigHandler, *oGlobalContext.error_log);
		if (iRC != 0) { return iRC; }

		oGlobalContext.error_log -> Info("Handler `%s` stopped", itvModules -> c_str());

		++itvModules;
	}
	oGlobalContext.error_log -> Info("All modules stopped");

return 0;
}

//
// A destructor
//
MainProcess::~MainProcess() throw()
{
	;;
}

} // namespace IRIS
// End.
