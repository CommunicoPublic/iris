/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MALLOC_ALLOCATOR_HPP__
#define _MALLOC_ALLOCATOR_HPP__ 1

/**
  @file MallocAllocator.hpp
  @brief Standard allocator
*/

#include "Allocator.hpp"

namespace IRIS
{
/**
  @class MallocAllocator MallocAllocator.hpp <MallocAllocator.hpp>
  @brief Standard allocator
*/
class MallocAllocator:
  public Allocator
{
public:
	/**
	  @brief Constructor
	*/
	MallocAllocator();

	/**
	  @brief Allocate chunk
	  @prram iSize - chunk size
	*/
	void * Allocate(const UINT_64 iSize);

	/**
	  @brief Deallocate chunk
	*/
	void Deallocate(void * vAllocated);

	/**
	  @brif Destructor
	*/
	~MallocAllocator() throw();
private:

};
} // namespace IRIS
#endif // _MALLOC_ALLOCATOR_HPP__
// End.
