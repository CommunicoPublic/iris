/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _LRU_CACHE_HPP__
#define _LRU_CACHE_HPP__ 1

#include "AVLTree.hpp"
#include "FixedSizeAllocator.hpp"
#include "STLAlgorithm.hpp"
#include "LRUList.hpp"

#include <stdio.h>

/**
  @file LRUCache.hpp
  @brief Last recently use element cache
*/

namespace IRIS
{

/**
  @class NullCacheEvictionCallback LRUCache.hpp <LRUCache.hpp>
  @brief Empty eviction callback, default class for EvictionCallback template argument
*/
template <typename K, typename V>class NullCacheEvictionCallback
{
public:
	/**
	  @brief Eviction hook
	  @param oK - Key to evict
	  @param oV - Value to evict
	*/
	void Evict(const K  & oK,
	           V        & oV)
	{
		;;
	}
};

/**
  @class NullCacheRestoreCallback LRUCache.hpp <LRUCache.hpp>
  @brief Empty object restore callback, default class for RestoreCallback template argument
*/
template <typename K, typename V>class NullCacheRestoreCallback
{
public:
	/**
	  @brief Eviction hook
	  @param oK - Key to evict
	  @param oV - Value to evict
	*/
	bool Restore(const K  & oK,
	             V        & oV)
	{
		return false;
	}
};

/**
@struct LRUCacheUsageStats LRUCache.hpp <LRUCache.hpp>
@brief Cache usage stats
*/
struct LRUCacheUsageStats
{
	/** Number of cache hits      */
	UINT_64  hits;
	/** Number of cache misses    */
	UINT_64  misses;
	/** Number of insertions      */
	UINT_64  insertions;
	/** Number of deletions       */
	UINT_64  deletions;
	/** Number of releases        */
	UINT_64  releases;
	/** Number of cache evictions */
	UINT_64  evictions;
	/** Number of cache restores  */
	UINT_64  restores;
	/** Number of errors          */
	UINT_64  errors;

	inline LRUCacheUsageStats(): hits(0),
	                             misses(0),
	                             insertions(0),
	                             deletions(0),
	                             releases(0),
	                             evictions(0),
	                             restores(0),
	                             errors(0) { ;; }
};

/**
  @class LRUCache LRUCache.hpp <LRUCache.hpp>
  @brief Last recently use element cache
*/
template<typename K,
         typename V,
         class EvictionCallback = NullCacheEvictionCallback<K, V>,
         class RestoreCallback = NullCacheRestoreCallback<K, V>,
         class Compare = STLW::less<K> >
  class LRUCache
{
private:
	// Does not exist
	LRUCache(const LRUCache  & oRhs);
	LRUCache& operator=(const LRUCache  & oRhs);

	/**
	  @struct CacheRecord LRUCache.hpp <LRUCache.hpp>
	  @brief Last recently used list record
	*/
	struct ListRecord
	{
		/** Key               */
		K          key;
		/** Value                      */
		V          value;
		/** Number of active instances */
		UINT_64    use_count;
	};

	typedef LRUList<ListRecord, FixedSizeAllocator> TLRUList;

	/**
	  @struct CacheRecord LRUCache.hpp <LRUCache.hpp>
	  @brief Last recently used element cache
	*/
	struct CacheRecord
	{
		/** Key               */
		K                             key;
		/** List element      */
		typename TLRUList::Iterator   it;
	};

	/**
	  @struct LRUListLess LRUCache.hpp <LRUCache.hpp>
	  @brief Cache element comparator
	*/
	struct LRUListLess
	{
		/** Comparator */
		Compare    oCMP;

		const bool operator()(const CacheRecord  & oX,
		                      const CacheRecord  & oY) const
		{
			return oCMP(oX.key, oY.key);
		}
	};

	typedef AVLTree<CacheRecord, LRUListLess, FixedSizeAllocator> TCacheTree;

	/** Number of max. elements in cache */
	const UINT_64             iMaxElements;

	/** Element eviction callback        */
	EvictionCallback        * pEvictionCallback;
	/** Element restore callback         */
	RestoreCallback         * pRestoreCallback;

	/** Memory allocator                 */
	FixedSizeAllocator        oListAllocator;
	/** Memory allocator                 */
	FixedSizeAllocator        oAVLAllocator;

	/** Comparator                       */
	LRUListLess               oLRUListLess;

	/** LRU list                         */
	TLRUList                  oLRUList;
	/** Data storage                     */
	TCacheTree                oCacheTree;
	/** Cache usage stats                */
	LRUCacheUsageStats        oUsageStats;

public:
	enum State { OK, ERROR, NOT_FOUND, DUPLICATE_FOUND };

	/**
	  @brief Constructor
	  @param iIMaxElements - Max. number of elements in cache
	  @param pICallback - eviction callback
	*/
	LRUCache(const UINT_64         iIMaxElements,
	         EvictionCallback    * pIEvictionCallback = NULL,
	         RestoreCallback     * pIRestoreCallback  = NULL): iMaxElements(iIMaxElements),

	                                                           pEvictionCallback(pIEvictionCallback),
	                                                           pRestoreCallback(pIRestoreCallback),

	                                                           oListAllocator(TLRUList::NodeSize(), iMaxElements),
	                                                           oAVLAllocator(TCacheTree::NodeSize(), iMaxElements),

	                                                           oLRUList(&oListAllocator),
	                                                           oCacheTree(&oLRUListLess, &oAVLAllocator) { ;; }

	/**
	  @brief Insert new element in cache
	  @param oK - Element key
	  @param oV - Element value
	  @return Execution state, one of (OK, ERROR, DUPLICATE_FOUND)
	*/
	State Insert(const K  & oK,
	             const V  & oV)
	{
		// Check duplicate
		typename TLRUList::Iterator oIt;
		const CacheRecord oFindRec = {  oK, oIt };
		if (oCacheTree.Find(oFindRec)) { ++oUsageStats.errors; return DUPLICATE_FOUND; }

		// Evict from cache
		State oRC = Evict();
		if (oRC != OK) { return oRC; }

		ListRecord oListRecord = { oK, oV, 1 };
		oIt = oLRUList.Add(oListRecord);

		// New record
		CacheRecord oNewRec = { oK, oIt };
		// Insert new record
		if (oCacheTree.Insert(oNewRec)) { ++oUsageStats.errors; return DUPLICATE_FOUND; }

		++oUsageStats.insertions;
	return OK;
	}

	/**
	  @brief Get the element from cache
	  @param oK - Element key
	  @param oV - Element value [out]
	  @return Execution state, one of (OK, NOT_FOUND)
	*/
	State Get(const K  & oK,
	          V        & oV)
	{
		typename TLRUList::Iterator oIt;
		const CacheRecord oFindRec = { oK, oIt };
		CacheRecord * pRec = oCacheTree.Find(oFindRec);
		if (pRec == NULL)
		{
			++oUsageStats.misses;
			if (pRestoreCallback != NULL && pRestoreCallback -> Restore(oK, oV))
			{
				++oUsageStats.restores;
				return Insert(oK, oV);
			}
			return NOT_FOUND;
		}

		oIt = pRec -> it;

		// Return value
		oV = oIt -> value;
		++oIt -> use_count;

		// Move element on top of the list
		oLRUList.MoveOnTop(oIt);

		++oUsageStats.hits;
	return OK;
	}

	/**
	  @brief Release cache element
	  @param oK - Element key
	  @return Execution state, one of (OK, ERROR, NOT_FOUND)
	*/
	State Release(const K  & oK)
	{
		typename TLRUList::Iterator oIt;
		const CacheRecord oFindRec = { oK, oIt };
		CacheRecord * pRec = oCacheTree.Find(oFindRec);
		if (pRec == NULL) { ++oUsageStats.errors; return NOT_FOUND; }

		oIt = pRec -> it;

		if (oIt -> use_count == 1) { ++oUsageStats.errors; return ERROR; }
		--oIt -> use_count;

		++oUsageStats.releases;
	return OK;
	}

	/**
	  @brief Remove element from cache
	  @param oK - Element key
	  @return Execution state, one of (OK, NOT_FOUND)
	*/
	State Remove(const K  & oK)
	{
		typename TLRUList::Iterator oIt;
		const CacheRecord oFindRec = { oK, oIt };
		CacheRecord * pRec = oCacheTree.Find(oFindRec);
		if (pRec == NULL) { ++oUsageStats.errors; return NOT_FOUND; }

		if (pEvictionCallback != NULL)
		{
			pEvictionCallback -> Evict(pRec -> key, pRec -> it -> value);
			++oUsageStats.evictions;
		}

		oLRUList.Remove(pRec -> it);
		oCacheTree.Remove(oFindRec);
		++oUsageStats.deletions;

	return OK;
	}

	/**
	  @brief Reset cache
	  @return Execution state, one of (OK, ERROR)
	*/
	State Reset()
	{
		typename TLRUList::Iterator oIt = oLRUList.Begin();
		while (oIt != oLRUList.End())
		{
			if (oIt -> use_count != 1) { ++oUsageStats.errors; return ERROR; }
			++oIt;
		}

		oIt = oLRUList.Begin();
		while (oIt != oLRUList.End())
		{
			if (pEvictionCallback != NULL)
			{
				pEvictionCallback -> Evict(oIt -> key, oIt -> value);
				++oUsageStats.evictions;
			}

			const CacheRecord oRemoveRec = { oIt -> key, oIt };
			oCacheTree.Remove(oRemoveRec);
			++oUsageStats.deletions;
			++oIt;
		}

		// Reset list
		oLRUList.Reset();
		return OK;
	}

	/**
	  @brief Get cache usage stats
	*/
	LRUCacheUsageStats GetStats() const { return oUsageStats; }

	/**
	  @brief Print list
	*/
	void Print()
	{
		oLRUList.Print();
	}

private:

	/**
	  @brief Evict first unused element from cache
	  @return Execution state, one of (OK, ERROR)
	*/
	State Evict()
	{
		if (oLRUList.Size() < iMaxElements) { return OK; }

		typename TLRUList::Iterator oIt = oLRUList.Begin();
		while (oIt != oLRUList.End())
		{
			if (oIt -> use_count == 1)
			{
				if (pEvictionCallback != NULL)
				{
					pEvictionCallback -> Evict(oIt -> key, oIt -> value);
					++oUsageStats.evictions;
				}
				const CacheRecord oRemoveRec = { oIt -> key, oIt };
				oCacheTree.Remove(oRemoveRec);
				oLRUList.Remove(oIt);
				++oUsageStats.deletions;
				return OK;
			}
			++oIt;
		}
		++oUsageStats.errors;
	return ERROR;
	}
};

} // namespace IRIS
#endif // _LRU_CACHE_HPP__
// End.
