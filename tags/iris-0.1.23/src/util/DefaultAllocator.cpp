/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "DefaultAllocator.hpp"

#include <stdlib.h>

namespace IRIS
{

//
// Consrtructor
//
DefaultAllocator::DefaultAllocator() { ;; }

//
// Allocate memory block
//
void * DefaultAllocator::Allocate(const UINT_64 iSize) { return malloc(iSize); }

//
// Free memory block
//
void DefaultAllocator::Deallocate(void * vAllocated) { free(vAllocated); }

//
// Destrcutor
//
DefaultAllocator::~DefaultAllocator() throw() { ;; }

} // namespace IRIS
// End.
