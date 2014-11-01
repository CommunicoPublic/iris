/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _ALLOCATOR_HPP__
#define _ALLOCATOR_HPP__ 1

/**
  @file Allocator.hpp
  @brief Base abstract class for memory pools
*/
#include "Types.h"

namespace IRIS
{
/**
  @class Allocator Allocator.hpp <Allocator.hpp>
  @brief Base class for memory pools
*/
class Allocator
{
public:
	/**
	  @brief Allocate chunk
	  @prram iSize - chunk size
	*/
	virtual void * Allocate(const UINT_64 iSize) = 0;

	/**
	  @brief Deallocate chunk
	*/
	virtual void Deallocate(void * vAllocated) = 0;

	/**
	  @brif Destructor
	*/
	virtual ~Allocator() throw();
private:

};
} // namespace IRIS
#endif // _ALLOCATOR_HPP__
// End.
