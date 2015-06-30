/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SCOPED_PTR_HPP__
#define _SCOPED_PTR_HPP__ 1

#include "Types.h"

namespace IRIS
{

template <typename T> class ScopedPtr
{
public:
	/**
	  @brief Constructor
	  @param pIObj - pointer to object
	*/
	ScopedPtr(T  * pIObj = NULL): pObj(pIObj) { ;; }

	/**
	  @brief Type cast constructor
	*/
	ScopedPtr & operator =(T  * pIObj)
	{
		if (pObj != NULL) { delete pObj; }
		pObj = pIObj;
	return *this;
	}

	/**
	  @brief Compare two pointers
	*/
	bool operator ==(const void * vPtr) { return pObj == vPtr; }

	/**
	  @brief Get pointer to the object
	*/
	T * operator->() { return pObj; }

	/**
	  @brief Get reference to the object
	*/
	T & operator*() { return *pObj; }

	/**
	  @brief Type cast operator
	*/
	operator T*() { return pObj; }

	/**
	  @brief A destructor
	*/
	~ScopedPtr() throw() { delete pObj; }
private:
	// Does not exist
	ScopedPtr(const ScopedPtr  & oRhs);
	ScopedPtr& operator =(const ScopedPtr  & oRhs);

	/** Pointer to the object */
	T  * pObj;
};

} // namespace IRIS
#endif // _SCOPED_PTR_HPP__
// End.

