/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _DEFAULT_ALLOCATOR_HPP__
#define _DEFAULT_ALLOCATOR_HPP__ 1

/**
  @file DefaultAllocator.hpp
  @brief Standard allocator
*/

#include "Types.h"

namespace IRIS
{
/**
  @class DefaultAllocator DefaultAllocator.hpp <DefaultAllocator.hpp>
  @brief Standard allocator
*/
class DefaultAllocator
{
public:
	/**
	  @brief Constructor
	*/
	DefaultAllocator();

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
	~DefaultAllocator() throw();
private:

};
} // namespace IRIS
#endif // _DEFAULT_ALLOCATOR_HPP__
// End.
