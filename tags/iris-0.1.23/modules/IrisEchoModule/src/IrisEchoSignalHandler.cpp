/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "IrisEchoSignalHandler.hpp"
#include "IrisEchoWorkerContext.hpp"

namespace IRIS_MOD
{

//
// Constructor
//
IrisEchoSignalHandler::IrisEchoSignalHandler(IrisEchoWorkerContext  & oIWorkerContext): oWorkerContext(oIWorkerContext) { ;; }

//
// A signal handler
//
void IrisEchoSignalHandler::Handler(INT_32 iSignal)
{
	;;
}

//
// A destructor
//
IrisEchoSignalHandler::~IrisEchoSignalHandler() throw()
{
	;;
}

} // namespace IRIS_MOD
// End.
