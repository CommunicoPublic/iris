/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "MainLoopEventWatcher.hpp"
#include "AcceptLoopContext.hpp"
#include "ClientTCPSocket.hpp"
#include "Debug.hpp"
#include "GlobalContext.hpp"
#include "Logger.hpp"
#include "MainLoopContext.hpp"
#include "ServerTCPSocket.hpp"
#include "STLSstream.hpp"

namespace IRIS
{

//
// Constructor
//
MainLoopEventWatcher::MainLoopEventWatcher(MainLoopContext                    & oIMainContext,
                                           STLW::vector<AcceptLoopContext *>  & vIContextes,
                                           ServerTCPSocket                    * pIServerSocket,
                                           ServiceConfig                      * pIServiceConfig): IOWatcher(&oIMainContext.main_loop),
                                                                                                  oMainContext(oIMainContext),
                                                                                                  vContextes(vIContextes),
                                                                                                  pServerSocket(pIServerSocket),
                                                                                                  pServiceConfig(pIServiceConfig)
{
	// Watch only for READ event
	SetEvents(pServerSocket -> GetSocket(), IOWatcher::READ);
	StartWatch();
}

//
// Watcher callback
//
void MainLoopEventWatcher::Callback(const INT_32   iSocket,
                                    const UINT_32  iREvents)
{
	if (iREvents != IOWatcher::READ)
	{
		oMainContext.global_context.error_log -> Emerg("Invalid event mask for MainLoopEventWatcher: need 0x%08X, got 0x%08X)", IOWatcher::READ, iREvents);
		return;
	}

	MainLoopContext::EventEntry oEventEntry;
	oEventEntry.service_config = pServiceConfig;

	// Accept new connection
	TCPSocket::State oRC = pServerSocket -> Accept(oEventEntry.socket);
	if (oRC != TCPSocket::OK)
	{
		const UINT_32 iRC = pServerSocket -> GetError();
		oMainContext.global_context.error_log -> Error("Accept failed: %s (error code %d)", Debug::PrintableError(iRC).c_str(), iRC);
		return;
	}

	oEventEntry.socket -> SetMode(TCPSocket::NON_BLOCKING);
	oEventEntry.socket -> SetMode(TCPSocket::CLOSE_ON_EXEC);

	oEventEntry.socket -> SetFlags(TCPSocket::KEEPALIVE,     1);
	oEventEntry.socket -> SetFlags(TCPSocket::REUSE_ADDRESS, 1);
	oEventEntry.socket -> SetFlags(TCPSocket::NODELAY,       1);

	// Put socket to the queue
	CRITICAL
	{
		MutexLocker oLocker(oMainContext.mutex);

		bool bDenyConnection = false;
		const STLW::string sClientIP = oEventEntry.socket -> GetIP();

		// ALLOW_DENY
		if (pServiceConfig -> allow_deny_order == ServiceConfig::ALLOW_DENY)
		{
			if (pServiceConfig -> ipv4_allowed_networks.CheckIP(sClientIP) != IPMap::OK)
			{
				bDenyConnection = (pServiceConfig -> ipv4_denied_networks.CheckIP(sClientIP) == IPMap::OK);
			}
		}
		// DENY_ALLOW
		else
		{
			if (pServiceConfig -> ipv4_denied_networks.CheckIP(sClientIP) == IPMap::OK)
			{
				bDenyConnection = true;
			}
		}

		if (bDenyConnection)
		{
			oMainContext.global_context.error_log -> Error("Access to service `%s` for client `%s`, denied by server configuration", pServiceConfig -> name.c_str(),
			                                                                                                                         oEventEntry.socket -> GetIP());
			oMainContext.global_context.transfer_log -> Error("Access to service `%s` for client `%s`, denied by server configuration", pServiceConfig -> name.c_str(),
			                                                                                                                            oEventEntry.socket -> GetIP());

			delete oEventEntry.socket;
			return;
		}

		// Check number of active connections
		if (pServiceConfig -> max_clients != 0)
		{
			++(pServiceConfig -> active_clients);
			oMainContext.global_context.error_log -> Debug("Active clients %u", pServiceConfig -> active_clients);

			if (pServiceConfig -> active_clients == pServiceConfig -> max_clients)
			{
				oMainContext.global_context.error_log -> Error("MaxClients reached for service `%s`, refusing client %s:%d", pServiceConfig -> name.c_str(),
				                                                                                                             oEventEntry.socket -> GetIP(),
				                                                                                                             oEventEntry.socket -> GetPort());
				oMainContext.global_context.transfer_log -> Error("MaxClients reached for service `%s`, refusing client %s:%d", pServiceConfig -> name.c_str(),
				                                                                                                                oEventEntry.socket -> GetIP(),
				                                                                                                                oEventEntry.socket -> GetPort());

				delete oEventEntry.socket;
				--(pServiceConfig -> active_clients);
				return;
			}
		}

		oMainContext.client_sockets.push(oEventEntry);
		// Find most underloaded thread
		UINT_32 iMinClients    = 0;
		UINT_32 iMinClientsPos = 0;
		UINT_32 iTotalClients  = 0;
		for (UINT_32 iPos = 0; iPos < vContextes.size(); ++iPos)
		{
			AcceptLoopContext * pContext = vContextes[iPos];
			if (iMinClients >= pContext -> clients.Size())
			{
				iMinClients = pContext -> clients.Size();
				iMinClientsPos = iPos;
			}
			iTotalClients += pContext -> clients.Size();
		}

		/*
		 * We don't need mutex here because we have only one main process
		 */

		// Notify network thread
		AcceptLoopContext * pContext = vContextes[iMinClientsPos];
		pContext -> accept_signal_watcher.SendSignal();

		oMainContext.global_context.error_log -> Debug("Socket %d -> slot %d", iSocket, iMinClientsPos);
	}
}

//
// Destructor
//
MainLoopEventWatcher::~MainLoopEventWatcher() throw() { ;; }

} // namespace IRIS
// End.
