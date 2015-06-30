/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "IrisEchoEventWatcher.hpp"
#include "IrisEchoThreadWorker.hpp"
#include "IrisEchoWorkerContext.hpp"

#include <AcceptLoopContext.hpp>

namespace IRIS_MOD
{

//
// Constructor
//
IrisEchoThreadWorker::IrisEchoThreadWorker(IrisEchoWorkerContext  & oIWorkerContext): oWorkerContext(oIWorkerContext)
{
	;;
}

//
// Shutdown thread worker
//
void IrisEchoThreadWorker::Shutdown()
{
	using namespace IRIS;

	CRITICAL
	{
		MutexLocker oLocker(oWorkerContext.mutex);
		--oWorkerContext.threads;
	}

	oWorkerContext.shutdown_cond.Signal();
}

//
// Execute jobs
//
void IrisEchoThreadWorker::Run()
{
	using namespace IRIS;
	for (;;)
	{
		// Check queue size
		bool bQueueEmpty = false;
		CRITICAL
		{
			MutexLocker oLocker(oWorkerContext.mutex);
			bQueueEmpty = oWorkerContext.task_queue.empty();
		}

		// Wait for task(s)
		if (bQueueEmpty) { oWorkerContext.cond.Wait(); }

		// Check shutdown flag
		if (oWorkerContext.shutdown) { Shutdown(); return; }

		// List of active tasks
		STLW::queue<IrisEchoWorkerContext::Task> oTasks;

		// Fetch pending task(s)
		CRITICAL
		{
			MutexLocker oLocker(oWorkerContext.mutex);
			while (!oWorkerContext.task_queue.empty())
			{
				IrisEchoWorkerContext::Task  & oTask = oWorkerContext.task_queue.front();
				oTasks.push(oTask);
				oWorkerContext.task_queue.pop();
			}
		}

		// Handle task(s)
		while (!oTasks.empty())
		{
			IrisEchoWorkerContext::Task  & oTask = oTasks.front();

			// Get request buffer
			IRIS::DataBuffer & oRequestBuffer  = oTask.event_watcher.GetInputDataBuffer();
			// Get response buffer
			IRIS::DataBuffer & oResponseBuffer = oTask.event_watcher.GetOutputDataBuffer();
			// Set status
			oTask.event_watcher.GetLoggerContext() -> SetParam("status",  "OK");

			oResponseBuffer.Assign("Request: ");
			oResponseBuffer.Append(oRequestBuffer.Data(), oRequestBuffer.Size());

			// Fire up WRITE event & send response
			oTask.event_watcher.SetIOState(IrisEchoEventWatcher::WRITE);

			AcceptLoopContext::EventEntry oEventEntry = { &oTask.event_watcher, IOWatcher::WRITE };
			oTask.accept_loop_context.AddEvent(oEventEntry);

			// Reset critical state
			oTask.event_watcher.ResetCritical();

			oTasks.pop();
		}
	}
}

//
// Destructor
//
IrisEchoThreadWorker::~IrisEchoThreadWorker() throw()
{
	;;
}

} // namespace IRIS_MOD
// End.
