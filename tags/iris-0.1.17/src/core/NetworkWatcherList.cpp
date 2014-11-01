/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

#include "Debug.hpp"
#include "NetworkEventWatcher.hpp"
#include "NetworkWatcherList.hpp"
#include "SystemVars.hpp"
#include "STLVector.hpp"

#include <stdio.h>

namespace IRIS
{

//
// Constructor
//
NetworkWatcherList::NetworkWatcherList(): oMutex(true) { ;; }

//
// Add new watcher
//
NetworkWatcherList::State NetworkWatcherList::AddWatcher(NetworkEventWatcher  * pWatcher)
{
	CRITICAL
	{
		MutexLocker oLocker(oMutex);

		// Check duplicates
		InactiveWatcherMap::iterator itmInactiveWatcherMap = mInactiveWatcherMap.find(pWatcher);
		if (itmInactiveWatcherMap != mInactiveWatcherMap.end()) { return EXIST; }

		// Check duplicates
		ActiveWatcherMap::iterator itmActiveWatcherMap = mActiveWatcherMap.find(pWatcher);
		if (itmActiveWatcherMap != mActiveWatcherMap.end()) { return EXIST; }

		mInactiveWatcherMap.insert(pWatcher);
	}

return OK;
}

//
// Inactivaye watcher
//
NetworkWatcherList::State NetworkWatcherList::ActivateWatcher(NetworkEventWatcher  * pWatcher,
                                                              const W_FLOAT          dTimeout)
{
	CRITICAL
	{
		MutexLocker oLocker(oMutex);

		// Check active watchers, do not activate active watcher
		ActiveWatcherMap::iterator itmActiveWatcherMap = mActiveWatcherMap.find(pWatcher);
		if (itmActiveWatcherMap != mActiveWatcherMap.end()) { return ERROR; }

		// Check inactive watchers
		InactiveWatcherMap::iterator itmInactiveWatcherMap = mInactiveWatcherMap.find(pWatcher);
		if (itmInactiveWatcherMap == mInactiveWatcherMap.end()) { return NOT_FOUND; }

		TimeoutMap::iterator itmTimeoutRecords = mTimeoutRecords.find(dTimeout);
		if (itmTimeoutRecords == mTimeoutRecords.end())
		{
			TimeoutList oTimeoutList;
			itmTimeoutRecords = mTimeoutRecords.insert(STLW::pair<W_FLOAT, TimeoutList>(dTimeout, oTimeoutList)).first;
		}

		const SystemVars::Time oTimeNow = SystemVars::GetTime();
		const W_FLOAT dTimeNow = oTimeNow.sec + 1E-9 * oTimeNow.nsec;

		TimeoutRec oTimeoutRec = { dTimeout, dTimeNow, pWatcher };
		itmTimeoutRecords -> second.push_back(oTimeoutRec);
		TimeoutList::iterator itoTimeoutList = itmTimeoutRecords -> second.end();
		--itoTimeoutList;

		mActiveWatcherMap[pWatcher] = itoTimeoutList;
		mInactiveWatcherMap.erase(itmInactiveWatcherMap);
	}

return OK;
}

//
// Inactivate watcher
//
NetworkWatcherList::State NetworkWatcherList::InactivateWatcher(NetworkEventWatcher  * pWatcher)
{
	CRITICAL
	{
		MutexLocker oLocker(oMutex);

		// Check inactive watchers
		InactiveWatcherMap::iterator itmInactiveWatcherMap = mInactiveWatcherMap.find(pWatcher);
		if (itmInactiveWatcherMap != mInactiveWatcherMap.end()) { return ERROR; }

		// Check active watchers, do not activate active watcher
		ActiveWatcherMap::iterator itmActiveWatcherMap = mActiveWatcherMap.find(pWatcher);
		if (itmActiveWatcherMap == mActiveWatcherMap.end()) { return NOT_FOUND; }

		// Get timeout
		const W_FLOAT dTimeout = itmActiveWatcherMap -> second -> timeout;

		// Remove from timeout list
		TimeoutMap::iterator itmTimeoutRecords = mTimeoutRecords.find(dTimeout);
		if (itmTimeoutRecords == mTimeoutRecords.end()) { return ERROR; }

		itmTimeoutRecords -> second.erase(itmActiveWatcherMap -> second);
		if (itmTimeoutRecords -> second.empty()) { mTimeoutRecords.erase(itmTimeoutRecords); }

		mInactiveWatcherMap.insert(pWatcher);
		mActiveWatcherMap.erase(itmActiveWatcherMap);
	}

return OK;
}

//
// Check watcher existence
//
bool NetworkWatcherList::WatcherExist(NetworkEventWatcher  * pWatcher) const
{
	MutexLocker oLocker(oMutex);

return (mInactiveWatcherMap.find(pWatcher) != mInactiveWatcherMap.end() || mActiveWatcherMap.find(pWatcher) != mActiveWatcherMap.end());
}

//
// Remove watcher from all lists
//
NetworkWatcherList::State NetworkWatcherList::DeleteWatcher(NetworkEventWatcher  * pWatcher)
{
	CRITICAL
	{
		MutexLocker oLocker(oMutex);

		// Check inactive watchers, simple case
		InactiveWatcherMap::iterator itmInactiveWatcherMap = mInactiveWatcherMap.find(pWatcher);
		if (itmInactiveWatcherMap != mInactiveWatcherMap.end())
		{
			mInactiveWatcherMap.erase(itmInactiveWatcherMap);
			return OK;
		}

		// Check active watchers, complex case
		ActiveWatcherMap::iterator itmActiveWatcherMap = mActiveWatcherMap.find(pWatcher);
		if (itmActiveWatcherMap == mActiveWatcherMap.end()) { return NOT_FOUND; }

		// Get timeout
		const W_FLOAT dTimeout = itmActiveWatcherMap -> second -> timeout;

		// Remove from timeout list
		TimeoutMap::iterator itmTimeoutRecords = mTimeoutRecords.find(dTimeout);
		if (itmTimeoutRecords == mTimeoutRecords.end()) { return ERROR; }

		itmTimeoutRecords -> second.erase(itmActiveWatcherMap -> second);
		if (itmTimeoutRecords -> second.empty()) { mTimeoutRecords.erase(itmTimeoutRecords); }

		mActiveWatcherMap.erase(itmActiveWatcherMap);
	}

return OK;
}

//
// get number of watchers (active & inactive)
//
UINT_32 NetworkWatcherList::Size() const
{
	MutexLocker oLocker(oMutex);

return mActiveWatcherMap.size() + mInactiveWatcherMap.size();
}

//
// Delete all watchers
//
void NetworkWatcherList::DeleteAllWatchers()
{
	CRITICAL
	{
		MutexLocker oLocker(oMutex);

		STLW::vector<NetworkEventWatcher *>  vContextes;

		{ TimeoutMap mTMP; mTimeoutRecords.swap(mTMP); }

		ActiveWatcherMap::iterator itmActiveWatcherMap = mActiveWatcherMap.begin();
		while (itmActiveWatcherMap != mActiveWatcherMap.end())
		{
			vContextes.push_back(itmActiveWatcherMap -> first);
			++itmActiveWatcherMap;
		}

		{ ActiveWatcherMap mTMP; mActiveWatcherMap.swap(mTMP); }

		InactiveWatcherMap::iterator itmInactiveWatcherMap = mInactiveWatcherMap.begin();
		while (itmInactiveWatcherMap != mInactiveWatcherMap.end())
		{
			vContextes.push_back(*itmInactiveWatcherMap);
			++itmInactiveWatcherMap;
		}

		{ InactiveWatcherMap mTMP; mInactiveWatcherMap.swap(mTMP); }

		// Shutdown all contextes
		STLW::vector<NetworkEventWatcher *>::iterator itvContextes = vContextes.begin();
		while (itvContextes != vContextes.end())
		{
			(*itvContextes) -> ShutdownContext();
			++itvContextes;
		}
	}
}

//
// Check timeouts
//
void NetworkWatcherList::CheckTimeout(STLW::vector<NetworkEventWatcher *>  & vWatchers)
{
	CRITICAL
	{
		MutexLocker oLocker(oMutex);

		const SystemVars::Time oTimeNow = SystemVars::GetTime();
		const W_FLOAT dTimeNow = oTimeNow.sec + 1E-9 * oTimeNow.nsec;

		TimeoutMap::iterator itmTimeoutRecords = mTimeoutRecords.begin();
		while (itmTimeoutRecords != mTimeoutRecords.end())
		{
			const W_FLOAT dMaxTime = dTimeNow - itmTimeoutRecords -> first;

			TimeoutList & oTimeoutList = itmTimeoutRecords -> second;
			TimeoutList::iterator itoTimeoutList = oTimeoutList.begin();
			while (itoTimeoutList != oTimeoutList.end())
			{
				if (itoTimeoutList -> last_ts > dMaxTime) { break; }

				// Move watcher from active list to inactive
				NetworkEventWatcher  * pWatcher = itoTimeoutList -> watcher;
				mActiveWatcherMap.erase(pWatcher);
				mInactiveWatcherMap.insert(pWatcher);

				vWatchers.push_back(pWatcher);
				++itoTimeoutList;
			}

			oTimeoutList.erase(oTimeoutList.begin(), itoTimeoutList);
			++itmTimeoutRecords;
		}
	}
}

//
// Destructor
//
NetworkWatcherList::~NetworkWatcherList() throw()
{
	try
	{
		DeleteAllWatchers();
	}
	catch(STLW::exception  & e)
	{
		Debug::Abort("NetworkWatcherList::~NetworkWatcherList/Exception");
	}
	catch(...)
	{
		Debug::Abort("NetworkWatcherList::~NetworkWatcherList/Unknown exception");
	}
}

} // namespace IRIS
// End.
