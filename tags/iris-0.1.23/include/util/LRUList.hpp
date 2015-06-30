/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _LRU_LIST_HPP__
#define _LRU_LIST_HPP__ 1

#include <MallocAllocator.hpp>
#include <new>

namespace IRIS
{
/**
  @class LRUList LRUList.hpp <LRUList.hpp>
  @brief Last recently use list
*/
template<typename T, typename ALLOC = MallocAllocator>class LRUList
{
private:
	struct ListElem
	{
		/** Next Iterator     */
		ListElem  * next;
		/** Previous Iterator */
		ListElem  * prev;
		/** Iterator          */
		T           item;

		/**
		  @brief Constructor
		*/
		ListElem(const T & oT): next(NULL),
		                        prev(NULL),
		                        item(oT)  { ;; }
	};

	/** Allocator           */
	ALLOC       * pAlloc;
	/** First Iterator      */
	ListElem    * pFirst;
	/** Last Iterator       */
	ListElem    * pLast;
	/** Number of elements  */
	UINT_64       iElements;
public:
	/**
	  @brief Get internal Iterator size for memory allocator
	*/
	static UINT_64 NodeSize() { return sizeof(ListElem); }

	/**
	  @brief Constructor
	  @param pIAlloc - Allocator object
	*/
	LRUList(ALLOC    * pIAlloc = NULL): pAlloc(pIAlloc),
	                                    pFirst(NULL),
	                                    pLast(NULL),
	                                    iElements(0) { ;; }

	/**
	  @brief Constructor
	  @param pIAlloc - Allocator object
	*/
	class Iterator
	{
	public:
		Iterator(): pListElem(NULL) { ;; }

		/**
		  @brief Type cast operator
		*/
		operator T&() { return pListElem -> item; }

		/**
		  @brief Type cast operator
		*/
		Iterator & operator=(const T & oT)
		{
			pListElem -> item = oT;
		return *this;
		}

		/**
		  @brief Pre-increment operator ++
		*/
		Iterator & operator++()
		{
			pListElem = pListElem -> next;
		return *this;
		}

		/**
		  @brief Post-increment operator ++
		*/
		Iterator operator++(int)
		{
			Iterator oTMP(pListElem);
			pListElem = pListElem -> next;
		return oTMP;
		}

		/**
		  @brief Access operator
		  @return Pair of key => value
		*/
		T * operator->() { return &(pListElem -> item); }

		/**
		  @brief Comparison operator
		  @param oRhs - object to compare
		  @return true if objects are equal
		*/
		bool operator ==(const Iterator & oRhs) { return pListElem == oRhs.pListElem; }

		/**
		  @brief Comparison operator
		  @param oRhs - object to compare
		  @return true if objects are NOT equal
		*/
		bool operator !=(const Iterator & oRhs) { return pListElem != oRhs.pListElem; }

		/**
		  @brief A destructor
		*/
		~Iterator() throw() { ;; }
	private:
		friend class LRUList;

		/**
		  @brief Constructor
		*/
		Iterator(ListElem  * pIListElem): pListElem(pIListElem) { ;; }

		/** Iterator */
		ListElem  * pListElem;
	};

	/**
	  @brief Get iterator pointed to start of list
	*/
	Iterator Begin() { return Iterator(pFirst); }

	/**
	  @brief Get iterator pointed to end of list
	*/
	Iterator End() { return Iterator(); }

	/**
	  @brief Check list is empty
	*/
	bool Empty() const { return iElements == 0; }

	/**
	  @brief Get number of elements
	*/
	UINT_64 Size() const { return iElements; }

	/**
	  @brief Add Iterator to the list
	  @param oT - Iterator to add
	*/
	Iterator Add(const T & oT)
	{
		void * vData = pAlloc -> Allocate(sizeof(ListElem));
		ListElem * pElem = new(vData)ListElem(oT);

		// Empty list
		if (pLast == NULL)
		{
			pFirst = pLast = pElem;
		}
		// Add to the end of list
		else
		{
			pElem -> prev = pLast;

			pLast -> next = pElem;
			pLast = pElem;
		}

		++iElements;
	return Iterator(pElem);
	}

	/**
	  @brief Remove Iterator from list
	  @param oIterator - Iterator to remove
	*/
	void Remove(const Iterator  & oIterator)
	{
		ListElem * pElem = oIterator.pListElem;

		--iElements;
		if (iElements == 0)
		{
			pElem -> ~ListElem();
			pAlloc -> Deallocate(pElem);
			pFirst = pLast = NULL;
			return;
		}

		ListElem  * pPrev = pElem -> prev;
		ListElem  * pNext = pElem -> next;

		// Remove first element
		if (pFirst == pElem)
		{
			pFirst = pNext;
			if (pNext) { pNext -> prev = NULL; }
		}
		// Remove last element
		else if (pLast == pElem)
		{
			pLast = pPrev;
			if (pPrev) { pPrev -> next = NULL; }
		}
		// Remove middle element
		else
		{
			if (pPrev) { pPrev -> next = pNext; }
			if (pNext) { pNext -> prev = pPrev; }
		}

		pElem -> ~ListElem();
		pAlloc -> Deallocate(pElem);
	}

	/**
	  @brief Reset list
	*/
	void Reset()
	{
		ListElem * pRec = pFirst;
		for(;;)
		{
			if (pRec == NULL) { break; }
			ListElem * pTMP = pRec;

			pRec = pRec -> next;

			pTMP -> ~ListElem();
			pAlloc -> Deallocate(pTMP);
		}
		iElements = 0;
		pFirst = pLast = NULL;
	}

	/**
	  @brief Move element on top
	*/
	void MoveOnTop(Iterator  & oIterator)
	{
		ListElem * pElem = oIterator.pListElem;

		// Nothing to do, element already on top
		if (pLast == pElem) { return; }

		ListElem  * pPrev = pElem -> prev;
		ListElem  * pNext = pElem -> next;

		// Move first element to top of the list
		if (pFirst == pElem)
		{
			pFirst = pNext;
			if (pNext) { pNext -> prev = NULL; }
		}
		// Move middle element to top of the list
		else
		{
			if (pPrev) { pPrev -> next = pNext; }
			if (pNext) { pNext -> prev = pPrev; }
		}

		pElem -> prev = pLast;
		pElem -> next = NULL;
		pLast -> next = pElem;
		pLast = pElem;
	}

	/**
	  @brief A destructor
	*/
	~LRUList() throw()
	{
		Reset();
	}

	/**
	  @brief Print list
	*/
	void Print()
	{
		ListElem * pRec = pFirst;
		fprintf(stderr, "Chain:");
		for(;;)
		{
			if (pRec == NULL) { break; }
			fprintf(stderr, " { %p %p %p }", (void*)pRec -> prev, (void*)pRec, (void*)pRec -> next);

			pRec = pRec -> next;

		}
		fprintf(stderr, " | first %p last %p\n", (void*)pFirst, (void*)pLast);
	}
};

} // namespace IRIS
#endif // _LRU_LIST_HPP__
// End.
