/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

#include "AcceptLoopContext.hpp"
#include "GlobalContext.hpp"
#include "Logger.hpp"
#include "MainLoopContext.hpp"
#include "MainLoopSignalHandler.hpp"

namespace IRIS
{

//
// Constructor
//
MainLoopSignalHandler::MainLoopSignalHandler(MainLoopContext                    & oIMainContext,
                                             STLW::vector<AcceptLoopContext *>  & vIContextes): oMainContext(oIMainContext),
                                                                                                vContextes(vIContextes),
                                                                                                bSignalCatched(false)
{
	;;
}

//
// A signal handler
//
void MainLoopSignalHandler::Handler(const INT_32 iSignal)
{
	Logger & oErrorLogger = *oMainContext.global_context.error_log;

	// Prevent multiple execution
	if (bSignalCatched)
	{
		oErrorLogger.Warn("Shutdown in progress");
		return;
	}

	switch (iSignal)
	{
		// Shutdown server immediately
		case SIGINT:
			{
				bSignalCatched = true;
				oErrorLogger.Info("Closing all socket queues");
				// Stop watching for server sockets
				oMainContext.stop_event_watcher.SendSignal();

				// Stop watching for all client sockets
				STLW::vector<AcceptLoopContext *>::iterator itvContextes = vContextes.begin();
				while (itvContextes != vContextes.end())
				{
					(*itvContextes) -> NotifyShutdown();
					++itvContextes;
				}
			}
			break;

		// Commit all transactions, then shut down
		case SIGTERM:
			{
				bSignalCatched = true;
				oErrorLogger.Info("Closing server sockets");
				// Stop watching for server sockets
				oMainContext.stop_event_watcher.SendSignal();

				// Stop watching for all client sockets
				STLW::vector<AcceptLoopContext *>::iterator itvContextes = vContextes.begin();
				while (itvContextes != vContextes.end())
				{
					(*itvContextes) -> NotifyShutdown();
					++itvContextes;
				}
			}
			break;
	}
}

//
// Destructor
//
MainLoopSignalHandler::~MainLoopSignalHandler() throw() { ;; }

} // namespace IRIS
// End.
