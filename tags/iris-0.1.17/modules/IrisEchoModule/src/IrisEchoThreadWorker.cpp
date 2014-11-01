/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "IrisEchoThreadWorker.hpp"
#include "IrisEchoEventWatcher.hpp"
#include "IrisEchoWorkerContext.hpp"

#include <AcceptLoopContext.hpp>
#include <DataBuffer.hpp>
#include <JSONParser.hpp>
#include <StringBuffer.hpp>
#include <TimeProfiler.hpp>
#include <Variant.hpp>
#include <VariantDumperJSON.hpp>

#include <stdio.h>
#include <unistd.h>


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
		if (oWorkerContext.shutdown)
		{
			CRITICAL
			{
				MutexLocker oLocker(oWorkerContext.mutex);
				--oWorkerContext.threads;
			}

			Shutdown();
			return;
		}

		// List of active tasks
		STLW::vector<IrisEchoWorkerContext::Task> vTasks;

		// Fetch pending task(s)
		CRITICAL
		{
			MutexLocker oLocker(oWorkerContext.mutex);
			vTasks.reserve(oWorkerContext.task_queue.size());
			while (!oWorkerContext.task_queue.empty())
			{
				IrisEchoWorkerContext::Task  & oTask = oWorkerContext.task_queue.front();
				vTasks.push_back(oTask);
				oWorkerContext.task_queue.pop();
			}
		}

		// Handle task(s)
		STLW::vector<IrisEchoWorkerContext::Task>::iterator itvTasks = vTasks.begin();
		while (itvTasks != vTasks.end())
		{
			IrisEchoWorkerContext::Task  & oTask = *itvTasks;

			// Get request buffer
			IRIS::DataBuffer & oRequestBuffer  = oTask.watcher -> GetInputDataBuffer();
			// Get response buffer
			IRIS::DataBuffer & oResponseBuffer = oTask.watcher -> GetOutputDataBuffer();
			// Set status
			oTask.watcher -> GetLoggerContext() -> SetParam("status",  "OK");

			oResponseBuffer.Assign("Request: ");
			oResponseBuffer.Append(oRequestBuffer.Data(), oRequestBuffer.Size());

			// Fire up WRITE event & send response
			CRITICAL
			{
				MutexLocker oLocker(oTask.context -> mutex);
				oTask.watcher -> SetIOState(IrisEchoEventWatcher::WRITE);

				AcceptLoopContext::EventEntry oEventEntry = { oTask.watcher, IOWatcher::WRITE };
				oTask.context -> socket_state_queue.push(oEventEntry);
				oTask.context -> socket_change_signal_watcher.SendSignal();
			}

			// Reset critical state
			oTask.watcher -> ResetCritical();
			++itvTasks;
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
