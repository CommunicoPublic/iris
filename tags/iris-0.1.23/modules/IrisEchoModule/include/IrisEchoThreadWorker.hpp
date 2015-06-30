/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _IRIS_ECHO_THREAD_WORKER_HPP__
#define _IRIS_ECHO_THREAD_WORKER_HPP__ 1

/**
  @file IrisEchoThreadWorker.hpp
  @brief Iris Echo protocol thread worker
*/

namespace IRIS_MOD
{
// FWD
class IrisEchoWorkerContext;

/**
  @class IrisEchoThreadWorker IrisEchoThreadWorker.hpp <IrisEchoThreadWorker.hpp>
  @brief Iris echo protocol thread worker
*/
class IrisEchoThreadWorker
{
public:
	/**
	  @brief Constructor
	  @@param oIWorkerContext - worker context
	*/
	IrisEchoThreadWorker(IrisEchoWorkerContext  & oIWorkerContext);

	/**
	  @brief Execute jobs
	*/
	void Run();

	/**
	  @brief Destructor
	*/
	~IrisEchoThreadWorker() throw();

private:
	// Does not exist
	IrisEchoThreadWorker(const IrisEchoThreadWorker  & oRhs);
	IrisEchoThreadWorker& operator=(const IrisEchoThreadWorker  & oRhs);

	/** Worker context               */
	IrisEchoWorkerContext  & oWorkerContext;

	/**
	  @brief Shutdown thread worker
	*/
	void Shutdown();
};

} // namespace IRIS_MOD
#endif // _IRIS_ECHO_THREAD_WORKER_HPP__
// End.
