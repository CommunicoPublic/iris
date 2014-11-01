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
	// Prevent multiple execution
	if (bSignalCatched)
	{
		oMainContext.global_context.error_log -> Warn("Shutdown in progress");
		return;
	}

	switch (iSignal)
	{
		// Shutdown server immediatly
		case SIGINT:
			{
				bSignalCatched = true;
				oMainContext.global_context.error_log -> Info("Closing all socket queues");
				// Stop watching for server sockets
				oMainContext.stop_event_watcher.SendSignal();
				// Stop watching for all client sockets
				STLW::vector<AcceptLoopContext *>::iterator itvContextes = vContextes.begin();
				while (itvContextes != vContextes.end())
				{
					(*itvContextes) -> stop_signal_watcher.SendSignal();
					++itvContextes;
				}
			}
			break;

		// Commit all transactions, then shut down
		case SIGTERM:
			{
				bSignalCatched = true;
				oMainContext.global_context.error_log -> Info("Closing server sockets");
				// Stop watching for server sockets
				oMainContext.stop_event_watcher.SendSignal();

				// Stop watching for all client sockets
				STLW::vector<AcceptLoopContext *>::iterator itvContextes = vContextes.begin();
				while (itvContextes != vContextes.end())
				{
					(*itvContextes) -> stop_signal_watcher.SendSignal();
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
