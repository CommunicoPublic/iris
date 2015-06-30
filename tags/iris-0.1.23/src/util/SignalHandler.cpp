/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include <SignalHandler.hpp>

namespace IRIS
{

/** User handler Factory */
STLW::vector<UserSignalHandler *> SignalHandler::mHandler[C_MAX_SIGNAL_NUM];

/** Signal Action */
struct sigaction SignalHandler::sSA;

//
// Private Constructor
//
SignalHandler::SignalHandler()
{
	sSA.sa_handler = Handler;
	sigemptyset(&sSA.sa_mask);
	sSA.sa_flags = 0;
}

//
// Create instance
//
SignalHandler & SignalHandler::Instance()
{
	static SignalHandler oHanlder;

return oHanlder;
}

//
// Register user Handler
//
INT_32 SignalHandler::RegisterHandler(const INT_32 iSignal, UserSignalHandler * pHandler)
{
	if (iSignal > C_MAX_SIGNAL_NUM || iSignal < 0) { return -1; }

	if (!sigismember(&sSA.sa_mask, iSignal)) { sigaction(iSignal, &sSA, NULL); }

	mHandler[iSignal].push_back(pHandler);

return 0;
}

//
// Register user Handler
//
INT_32 SignalHandler::RemoveHandler(const INT_32 iSignal, UserSignalHandler * pHandler)
{
	if (iSignal > C_MAX_SIGNAL_NUM || iSignal < 0) { return -1; }

	STLW::vector<UserSignalHandler *>::iterator itV = mHandler[iSignal].begin();
	const STLW::vector<UserSignalHandler *>::iterator itVEnd = mHandler[iSignal].end();
	while (itV != itVEnd)
	{
		if (pHandler == *itV) { mHandler[iSignal].erase(itV); return 0; }
		itV++;
	}

return -1;
}

//
// Static C Handler
//
void SignalHandler::Handler(INT_32 iSignal)
{
	try
	{
		STLW::vector<UserSignalHandler *>::iterator itV = mHandler[iSignal].begin();
		const STLW::vector<UserSignalHandler *>::iterator itVEnd = mHandler[iSignal].end();
		while (itV != itVEnd)
		{
			try { (*itV) -> Handler(iSignal); }
			catch(...) { ;; }
			itV++;
		}
	}
	catch(...) { ;; }
}

//
// Remove all signal handlers
//
void SignalHandler::Clear()
{
	sigemptyset(&sSA.sa_mask);

	for(UINT_32 iSignal = 0; iSignal < C_MAX_SIGNAL_NUM; ++iSignal)
	{
		std::vector<UserSignalHandler *> vTMP;
		mHandler[iSignal].swap(vTMP);
	}
}

} // namespace IRIS
// End.
