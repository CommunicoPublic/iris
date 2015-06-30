/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "AcceptSignalWatcher.hpp"
#include "AcceptLoopContext.hpp"
#include "GlobalContext.hpp"
#include "Logger.hpp"
#include "MainLoopContext.hpp"
#include "NetworkHandler.hpp"
#include "NetworkEventWatcher.hpp"
#include "ServiceConfig.hpp"
#include "ServerClientTCPSSLSocket.hpp"

namespace IRIS
{

//
// Constructor
//
AcceptSignalWatcher::AcceptSignalWatcher(AcceptLoopContext  & oIAcceptLoopContext): IOAsyncWatcher(&(oIAcceptLoopContext.GetEventLoop())),
                                                                                    oAcceptLoopContext(oIAcceptLoopContext)
{
	StartWatch();
}

#ifdef IRIS_TLS_SUPPORT
//
// Setup SSL
//
INT_32 AcceptSignalWatcher::SetupSSL()
{
	GlobalContext & oGlobalContext = oAcceptLoopContext.GetMainLoopContext().global_context;

	// Setup SSL for all services
	STLW::vector<ServiceConfig>::iterator itvServices = oGlobalContext.config.services.begin();
	while (itvServices != oGlobalContext.config.services.end())
	{
		// Check for SSL support
		if (itvServices -> enable_ssl_tls)
		{
			SSL_CTX * pSSLContext = NULL;
			for (UINT_32 iPos = 0; iPos < itvServices -> tls_cert_file.size(); ++iPos)
			{
				oGlobalContext.error_log -> Debug("Adding certificate { `%s` `%s` } for service `%s`",
				                                  itvServices -> tls_cert_file[iPos].c_str(),
				                                  itvServices -> tls_key_file[iPos].c_str(),
				                                  itvServices -> name.c_str());

				pSSLContext = oSSLCore.AddContext(itvServices -> tls_cert_file[iPos],
				                                  itvServices -> tls_key_file[iPos],
				                                  itvServices -> dh_file,
				                                  itvServices -> ciphers,
				                                  itvServices -> name,
				                                  itvServices -> use_tls,
				                                  itvServices -> prefer_server_ciphers,
				                                  *oGlobalContext.error_log);
				if (pSSLContext == NULL)
				{
					oGlobalContext.error_log -> Error("Setup of SSL/TLS data for service `%s` aborted", itvServices -> name.c_str());
					return -1;
				}
			}

			mSSLServiceMap[itvServices -> name] = pSSLContext;
		}
		++itvServices;
	}

return 0;
}
#endif // IRIS_TLS_SUPPORT

//
// Watcher callback
//
void AcceptSignalWatcher::Callback(const UINT_32  iREvents)
{
	MainLoopContext & oMainLoopContext = oAcceptLoopContext.GetMainLoopContext();

	STLW::vector<MainLoopContext::EventEntry> vClients;
	CRITICAL
	{
		MutexLocker oLocker(oMainLoopContext.mutex);

		// Copy all pending connections to internal vector
		STLW::queue<MainLoopContext::EventEntry> & oClients = oMainLoopContext.client_sockets;

		vClients.reserve(oClients.size());
		while (!oClients.empty())
		{
			MainLoopContext::EventEntry  & oEntry = oClients.front();
			vClients.push_back(oEntry);
			oClients.pop();
		}
	}

	// Handle all pending connections
	STLW::vector<MainLoopContext::EventEntry>::iterator itvClients = vClients.begin();
	while (itvClients != vClients.end())
	{
		MainLoopContext::EventEntry  & oEntry = *itvClients;

		// Get network handler
		NetworkHandler * pHandler = static_cast<NetworkHandler *>(oEntry.service_config -> handler);

#ifdef IRIS_TLS_SUPPORT
		// Setup SSL connection, if need
		STLW::map<STLW::string, SSL_CTX *>::iterator itmSSLServiceMap = mSSLServiceMap.find(oEntry.service_config -> name);
		if (itmSSLServiceMap != mSSLServiceMap.end() && oEntry.socket -> IsSSL())
		{
			dynamic_cast<ServerClientTCPSSLSocket *>(oEntry.socket) -> SetSSLContext(itmSSLServiceMap -> second);
		}
#endif // IRIS_TLS_SUPPORT

		// Create new connection watcher
		NetworkEventWatcher * pNetworkWatcher = pHandler -> NewConnection(oAcceptLoopContext, *oEntry.service_config, oEntry.socket);
		// Add new watcher
		oAcceptLoopContext.AddWatcher(pNetworkWatcher);
		// And start watch
		pNetworkWatcher -> StartWatch();

		++itvClients;
	}
}

//
// A destructor
//
AcceptSignalWatcher::~AcceptSignalWatcher() throw() { ;; }

} // namespace IRIS
// End.
