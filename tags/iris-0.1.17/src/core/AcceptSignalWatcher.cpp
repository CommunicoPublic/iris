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
AcceptSignalWatcher::AcceptSignalWatcher(AcceptLoopContext  * pIContext): IOAsyncWatcher(&(pIContext -> thread_loop)),
                                                                          pContext(pIContext)
{
	StartWatch();
}

#ifdef IRIS_TLS_SUPPORT
//
// Setup SSL
//
INT_32 AcceptSignalWatcher::SetupSSL()
{
	STLW::vector<ServiceConfig> & oServiceConfig = pContext -> main_context.global_context.config.services;

	STLW::vector<ServiceConfig>::iterator itvServices = oServiceConfig.begin();
	while (itvServices != oServiceConfig.end())
	{
		// Check for SSL support
		if (itvServices -> enable_ssl_tls)
		{
			SSL_CTX * pSSLContext = NULL;
			for (UINT_32 iPos = 0; iPos < itvServices -> tls_cert_file.size(); ++iPos)
			{
				pContext -> main_context.global_context.error_log -> Debug("Adding certificate { `%s` `%s` } for service `%s`",
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
				                                  *(pContext -> main_context.global_context.error_log));
				if (pSSLContext == NULL)
				{
					pContext -> main_context.global_context.error_log -> Error("Setup of SSL/TLS data for service `%s` aborted", itvServices -> name.c_str());
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
	STLW::vector<MainLoopContext::EventEntry> vClients;
	CRITICAL
	{
		MutexLocker oLocker(pContext -> main_context.mutex);

		// Copy all pending connections to internal vector
		STLW::queue<MainLoopContext::EventEntry> & oClients = pContext -> main_context.client_sockets;
		vClients.reserve(oClients.size());
		while (!oClients.empty())
		{
			MainLoopContext::EventEntry  & oEntry = oClients.front();
			vClients.push_back(oEntry);
			oClients.pop();
		}
	}

	// Handle all new pending connections
	STLW::vector<MainLoopContext::EventEntry>::iterator itvClients = vClients.begin();
	while (itvClients != vClients.end())
	{
		MainLoopContext::EventEntry  & oEntry = *itvClients;

		// Get network handler
		NetworkHandler * pHandler = static_cast<NetworkHandler *>(oEntry.service_config -> handler);

		// Setup SSL connection
		STLW::map<STLW::string, SSL_CTX *>::iterator itmSSLServiceMap = mSSLServiceMap.find(oEntry.service_config -> name);
		if (itmSSLServiceMap != mSSLServiceMap.end() && oEntry.socket -> IsSSL())
		{
			dynamic_cast<ServerClientTCPSSLSocket *>(oEntry.socket) -> SetSSLContext(itmSSLServiceMap -> second);
		}

		// Create new connection watcher
		NetworkEventWatcher * pNetworkWatcher = pHandler -> NewConnection(pContext, oEntry.service_config, oEntry.socket);

		// Add new watcher
		pContext -> clients.AddWatcher(pNetworkWatcher);

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
