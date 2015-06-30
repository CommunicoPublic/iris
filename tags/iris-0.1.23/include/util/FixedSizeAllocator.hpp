/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _FIXED_SIZE_ALLOCATOR_HPP__
#define _FIXED_SIZE_ALLOCATOR_HPP__ 1

/**
  @file FixedSizeAllocator.hpp
  @brief Memory pool for fixed size objects
*/

#include "Allocator.hpp"

namespace IRIS
{
/**
  @class FixedSizeAllocator FixedSizeAllocator.hpp <FixedSizeAllocator.hpp>
  @brief Memory pool for fixed size objects
*/
class FixedSizeAllocator:
  public Allocator
{
public:
	/**
	  @brief Constructor
	  @param iIObjectSize - object size
	  @param iIMaxObjects - max. number of objects in pool
	*/
	FixedSizeAllocator(const UINT_32  iIObjectSize,
	                   const UINT_32  iIMaxObjects);

	/**
	  @brief Allocate chunk
	*/
	void * Allocate(const UINT_64  iSize);

	/**
	  @brief Deallocate chunk
	*/
	void Deallocate(void * vAllocated);

	/**
	  @brif Destructor
	*/
	~FixedSizeAllocator() throw();
private:
	// Does not exist
	FixedSizeAllocator(const FixedSizeAllocator  & oRhs);
	FixedSizeAllocator& operator=(const FixedSizeAllocator  & oRhs);

	/** Object size            */
	const UINT_32    iObjectSize;
	/** Max number of Objects  */
	const UINT_32    iMaxObjects;
	/** Used number of Objects */
	UINT_32          iUsedObjects;
	/** Free Objects index     */
	UINT_32        * aObjectIndex;
	/** Data Object            */
	void           * vData;

	/**
	  @brief Get Object Id by address
	*/
	UINT_32 GetObjectId(const void * vObjectAddr);
};
} // namespace IRIS
#endif // _FIXED_SIZE_ALLOCATOR_HPP__
// End.
