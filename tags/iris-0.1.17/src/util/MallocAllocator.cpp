/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "MallocAllocator.hpp"

#include <stdlib.h>

namespace IRIS
{

//
// Consrtructor
//
MallocAllocator::MallocAllocator() { ;; }

//
// Allocate memory block
//
void * MallocAllocator::Allocate(const UINT_64 iSize) { return malloc(iSize); }

//
// Free memory block
//
void MallocAllocator::Deallocate(void * vAllocated) { free(vAllocated); }

//
// Destrcutor
//
MallocAllocator::~MallocAllocator() throw() { ;; }

} // namespace IRIS
// End.
