/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _IRIS_ECHO_WORKER_CONTEXT_HPP__
#define _IRIS_ECHO_WORKER_CONTEXT_HPP__ 1

/**
  @file IrisEchoWorkerContext.hpp
  @brief Iris Echo module thread context
*/

// C++ Includes
#include <ConditionVariable.hpp>
#include <Mutex.hpp>
#include <STLQueue.hpp>

namespace IRIS
{
// FWD
struct AcceptLoopContext;
}

namespace IRIS_MOD
{
// FWD
class IrisEchoEventWatcher;

/**
  @class IrisEchoWorkerContext IrisEchoWorkerContext.hpp <IrisEchoWorkerContext.hpp>
  @brief Iris Echo module thread context
*/
struct IrisEchoWorkerContext
{
	struct Task
	{
		/** Loop context            */
		IRIS::AcceptLoopContext  & accept_loop_context;
		/** Event watcher           */
		IrisEchoEventWatcher     & event_watcher;
	};

	/** Task queue mutex                    */
	IRIS::Mutex          mutex;
	/** Queue of tasks for echo handlers    */
	STLW::queue<Task>    task_queue;

	/** Task condition variable             */
	IRIS::Condition      cond;
	/** Shutdown thread condition variable  */
	IRIS::Condition      shutdown_cond;

	/** Shutdown thread flag                */
	bool                 shutdown;
	/** Number of active threads            */
	UINT_32              threads;
};

} // namespace IRIS_MOD
#endif // _IRIS_ECHO_WORKER_CONTEXT_HPP__
// End.
