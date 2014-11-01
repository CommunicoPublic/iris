/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "GlobalContext.hpp"
#include "Logger.hpp"
#include "MainSignalHandler.hpp"

#include <sys/types.h>
#include <unistd.h>

namespace IRIS
{

//
// Constructor
//
MainSignalHandler::MainSignalHandler(GlobalContext  & oIGlobalContext): oGlobalContext(oIGlobalContext) { ;; }

//
// A signal handler
//
void MainSignalHandler::Handler(const INT_32 iSignal)
{
	switch (iSignal)
	{
		// Ignore it
		case SIGPIPE:
			;;
			break;

		// Shutdown server immediatly
		case SIGINT:
			oGlobalContext.shutdown_type = GlobalContext::IMMEDIATE;
			oGlobalContext.error_log -> Notice("%d catched SIGINT. Shutting down immediately", INT_32(getpid()));
			break;

		// Commit all transactions, then shut down
		case SIGTERM:
			oGlobalContext.shutdown_type = GlobalContext::TRANSACTIONAL;
			oGlobalContext.error_log -> Notice("%d catched SIGTERM. Commit all transactions and shut down", INT_32(getpid()));
			break;

		// Re-open logs, TBD
		case SIGUSR1:
			oGlobalContext.error_log -> Notice("%d catched SIGUSR1. Reopening all logs", INT_32(getpid()));
			break;

		default:
			oGlobalContext.error_log -> Notice("Unknown signal (%d) catched: %d", INT_32(getpid()), iSignal);
	}
}

//
// Destructor
//
MainSignalHandler::~MainSignalHandler() throw() { ;; }

} // namespace IRIS
// End.
