/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MUTEX_MULTI_LOCKER_HPP__
#define _MUTEX_MULTI_LOCKER_HPP__ 1

#include "Mutex.hpp"
#include "STLMap.hpp"

#include <stdio.h>

namespace IRIS
{

/**
  @class MutexMultiLocker MutexMultiLocker.hpp <MutexMultiLocker.hpp>
  @brief Lock mutex by specified object id
*/
template <typename K>class MutexMultiLocker
{
public:
	/**
	  @brief Constructor
	*/
	MutexMultiLocker();

	/**
	  @brief Lock mutex for specified key
	*/
	void Lock(const K  & oK);

	/**
	  @brief Unlock mutex for specified key
	*/
	void Unlock(const K  & oK);

	/**
	  @brief Unlock mutex
	*/
	~MutexMultiLocker() throw();

private:
	// Does not exist
	MutexMultiLocker(const MutexMultiLocker  & oRhs);
	MutexMultiLocker& operator=(const MutexMultiLocker  & oRhs);

	struct MapRec
	{
		/** Number of references */
		UINT_32    refcount;
		/** Mutex                */
		Mutex    * mutex;
	};
	typedef STLW::map<K, MapRec> LockMap;

	/** Mutex                  */
	Mutex    oMutex;
	/** List of locked mutexes */
	LockMap  mLockMap;
};

//
// Constructor
//
template <typename K>MutexMultiLocker<K>::MutexMultiLocker()
{
	;;
}

//
// Constructor
//
template <typename K>void MutexMultiLocker<K>::Lock(const K  & oK)
{
	Mutex * pMutex = NULL;
	CRITICAL
	{
		MutexLocker oLocker(oMutex);

		typename LockMap::iterator itmLockMap = mLockMap.find(oK);
		if (itmLockMap == mLockMap.end())
		{
			MapRec oMapRec = { 1, new Mutex };
			oMapRec.mutex -> Lock();
			mLockMap[oK] = oMapRec;
fprintf(stderr, "Lock: %u, create mutex, refcount: %u\n", oK, oMapRec.refcount);
			return;
		}

		MapRec & oMapRec = itmLockMap -> second;
		++oMapRec.refcount;
fprintf(stderr, "Lock: %u refcount: %u\n", oK,oMapRec.refcount);
		pMutex = oMapRec.mutex;
	}
	pMutex -> Lock();
}

//
// Constructor
//
template <typename K>void MutexMultiLocker<K>::Unlock(const K  & oK)
{
	CRITICAL
	{
		MutexLocker oLocker(oMutex);

		typename LockMap::iterator itmLockMap = mLockMap.find(oK);
		if (itmLockMap == mLockMap.end()) { return; }

		MapRec & oMapRec = itmLockMap -> second;
		oMapRec.mutex -> Unlock();
fprintf(stderr, "Unlock: %u refcount: %u\n", oK,oMapRec.refcount);
		--oMapRec.refcount;
		if (oMapRec.refcount != 0) { return; }
fprintf(stderr, "Unlock: %u refcount: %u\n", oK,oMapRec.refcount);
		delete oMapRec.mutex;
		mLockMap.erase(itmLockMap);
	}
}

//
// Destructor
//
template <typename K>MutexMultiLocker<K>::~MutexMultiLocker() throw()
{
	typename LockMap::iterator itmLockMap = mLockMap.begin();
	while (itmLockMap != mLockMap.end())
	{
		MapRec & oMapRec = itmLockMap -> second;
		delete oMapRec.mutex;

		++itmLockMap;
	}
}

} // namespace IRIS
#endif // _MUTEX_MULTI_LOCKER_HPP__
// End.
