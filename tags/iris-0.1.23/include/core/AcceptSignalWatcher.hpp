/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _ACCEPT_SIGNAL_WATCHER_HPP__
#define _ACCEPT_SIGNAL_WATCHER_HPP__ 1

/**
  @file AcceptSignalWatcher.hpp
  @brief Watcher for accept signal from main loop
*/

#include "IOAsyncWatcher.hpp"

#ifdef IRIS_TLS_SUPPORT
    #include "SSL.hpp"
    #include "STLMap.hpp"
#endif // IRIS_TLS_SUPPORT

namespace IRIS
{
// FWD
class AcceptLoopContext;

/**
  @class AcceptSignalWatcher AcceptSignalWatcher.hpp <AcceptSignalWatcher.hpp>
  @brief Watcher for accept signal from main loop
*/
class AcceptSignalWatcher:
  public IOAsyncWatcher
{
public:
	/**
	  @brief Constructor
	  @param pIContext - accept thread context
	*/
	AcceptSignalWatcher(AcceptLoopContext  & pIAcceptLoopContext);

	/**
	  @brief Watcher callback
	  @param iREvents - list of events
	*/
	void Callback(const UINT_32  iREvents);

	/**
	  @brief A destructor
	*/
	~AcceptSignalWatcher() throw();
private:
	// Does not exist
	AcceptSignalWatcher(const AcceptSignalWatcher  & oRhs);
	AcceptSignalWatcher& operator=(const AcceptSignalWatcher  & oRhs);

	/** Thread context       */
	AcceptLoopContext  & oAcceptLoopContext;

#ifdef IRIS_TLS_SUPPORT
	friend class AcceptLoopContext;

	/**
	  @brief Setup SSL
	*/
	INT_32 SetupSSL();

	/** SSL core routines */
	SSLCore                               oSSLCore;
	/** SSL contextes map */
	STLW::map<STLW::string, SSL_CTX *>    mSSLServiceMap;
#endif // IRIS_TLS_SUPPORT

};

} // namespace IRIS
#endif // _ACCEPT_SIGNAL_WATCHER_HPP__
// End.
