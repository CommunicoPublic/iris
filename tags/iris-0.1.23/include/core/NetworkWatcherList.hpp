/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _NETWORK_WATCHER_LIST_HPP__
#define _NETWORK_WATCHER_LIST_HPP__ 1

/**
  @file NetworkWatcherList.hpp
  @brief Iris network watcher timeout handler
*/

#include "Mutex.hpp"
#include "STLList.hpp"
#include "STLMap.hpp"
#include "STLSet.hpp"
#include "STLVector.hpp"

namespace IRIS
{
// FWD
class IOAsyncWatcher;
class NetworkEventWatcher;

/**
  @class NetworkWatcherList NetworkWatcherList.hpp <NetworkWatcherList.hpp>
  @brief Iris network watcher timeout handler
*/
class NetworkWatcherList
{
public:
	enum State { OK, ERROR, EXIST, NOT_FOUND };

	/**
	  @brief Constructor
	*/
	NetworkWatcherList();

	/**
	  @brief Add watcher to inactive list
	  @param pWatcher - connection watcher context
	*/
	State AddWatcher(NetworkEventWatcher  * pWatcher);

	/**
	  @brief Move watcher from inactive list to active list
	  @param pWatcher - connection watcher context
	  @param dTimeout - timeout for context
	*/
	State ActivateWatcher(NetworkEventWatcher  * pWatcher,
	                      const W_FLOAT          dTimeout);

	/**
	  @brief Remove watcher from active list and move in ti inactive list
	  @param pWatcher - connection watcher context
	*/
	State InactivateWatcher(NetworkEventWatcher  * pWatcher);

	/**
	  @brief Remove watcher from both(active & inactive) lists
	  @param pWatcher - connection watcher context
	*/
	State RemoveWatcher(NetworkEventWatcher  * pWatcher);

	/**
	  @brief get number of watchers (active & inactive)
	*/
	UINT_32 Size() const;

	/**
	  @brief Remove all watchers
	*/
	void RemoveAllWatchers(STLW::vector<NetworkEventWatcher *>  & vContextes);

	/**
	  @brief Check timeouts
	  @param vWatchers - list of watchers to check
	*/
	void CheckTimeout(STLW::vector<NetworkEventWatcher *>  & vWatchers);

	/**
	  @brief Check watcher existence
	  @param pWatcher - connection watcher context
	*/
	bool WatcherExist(NetworkEventWatcher  * pWatcher) const;

	/**
	  @brief Destructor
	*/
	~NetworkWatcherList() throw();
private:
	// Does not exist
	NetworkWatcherList(const NetworkWatcherList  & oRhhs);
	NetworkWatcherList& operator=(const NetworkWatcherList  & oRhhs);

	/**
	  @struct TimeoutRec NetworkWatcherList.hpp <NetworkWatcherList.hpp>
	  @brief Timeout record
	*/
	struct TimeoutRec
	{
		/** Timeout value  */
		W_FLOAT                timeout;
		/** Last event     */
		W_FLOAT                last_ts;
		/** Socket context */
		NetworkEventWatcher  * watcher;
	};

	typedef STLW::list<TimeoutRec>           TimeoutList;
	typedef STLW::map<W_FLOAT, TimeoutList>  TimeoutMap;

	typedef STLW::map<NetworkEventWatcher *, TimeoutList::iterator>  ActiveWatcherMap;
	typedef STLW::set<NetworkEventWatcher *> InactiveWatcherMap;

	/** List of records with context timeouts */
	TimeoutMap            mTimeoutRecords;
	/** List of active watchers               */
	ActiveWatcherMap      mActiveWatcherMap;
	/** List of inactive watchers             */
	InactiveWatcherMap    mInactiveWatcherMap;
};

} // namespace IRIS
#endif // _NETWORK_WATCHER_LIST_HPP__
// End.
