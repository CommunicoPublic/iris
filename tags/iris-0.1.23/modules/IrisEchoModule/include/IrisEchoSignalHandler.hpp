/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _IRIS_ECHO_SIGNAL_HANDLER_HPP__
#define _IRIS_ECHO_SIGNAL_HANDLER_HPP__ 1

/**
  @file IrisEchoSignalHandler.hpp
  @brief Iris Echo protocol signal handler
*/

#include <SignalHandler.hpp>

namespace IRIS_MOD
{
// FWD
class IrisEchoWorkerContext;

/**
  @class IrisEchoSignalHandler IrisEchoSignalHandler.hpp <IrisEchoSignalHandler.hpp>
  @brief Signal handler implementation
*/
class IrisEchoSignalHandler:
  public IRIS::UserSignalHandler
{
public:
	/**
	  @brief Constructor
	  @param oIWorkerContext - thread context
	*/
	IrisEchoSignalHandler(IrisEchoWorkerContext  & oIWorkerContext);

	/**
	  @brief A signal handler
	*/
	void Handler(INT_32 iSignal);

	/**
	  @brief A destructor
	*/
	~IrisEchoSignalHandler() throw();
private:
	// Does not exist
	IrisEchoSignalHandler(const IrisEchoSignalHandler  & oRhs);
	IrisEchoSignalHandler& operator=(const IrisEchoSignalHandler  & oRhs);

	/** Thread worker context */
	IrisEchoWorkerContext  & oWorkerContext;
};

} // namespace IRIS_MOD
#endif // _IRIS_ECHO_SIGNAL_HANDLER_HPP__
// End.
