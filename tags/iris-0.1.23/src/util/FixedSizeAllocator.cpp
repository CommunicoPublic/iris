/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "FixedSizeAllocator.hpp"

#include <new>
#include <stdlib.h>
#include <stdio.h>

namespace IRIS
{

//
// Consrtructor
//
FixedSizeAllocator::FixedSizeAllocator(const UINT_32  iIObjectSize,
                                       const UINT_32  iIMaxObjects): iObjectSize(iIObjectSize),
                                                                     iMaxObjects(iIMaxObjects),
                                                                     iUsedObjects(0)
{
	vData = malloc(iObjectSize * iMaxObjects);
	if (vData == NULL) { throw std::bad_alloc(); }

	aObjectIndex = (UINT_32 *)malloc(iMaxObjects * sizeof(UINT_32));
	if (aObjectIndex == NULL)
	{
		free(vData);
		throw std::bad_alloc();
	}

	for (UINT_32 iPos = 0; iPos < iMaxObjects; ++iPos) { aObjectIndex[iPos] = iPos; }
}

//
// Allocate memory block
//
void * FixedSizeAllocator::Allocate(const UINT_64  iSize)
{
	if (iSize != iObjectSize) { return NULL; }

	if (iUsedObjects == iMaxObjects) { return NULL; }

	const UINT_32 iObjectId = aObjectIndex[iUsedObjects];
	++iUsedObjects;
//fprintf(stderr, "Allocate/iUsedObjects = %u\n", UINT_32(iUsedObjects));
return ((CHAR_P)vData + iObjectId * iObjectSize);
}

//
// Free memory block
//
void FixedSizeAllocator::Deallocate(void * vAllocated)
{
	if (vAllocated == NULL) { return; }

	const UINT_32 iObjectId = GetObjectId(vAllocated);
	aObjectIndex[--iUsedObjects] = iObjectId;
//fprintf(stderr, "Deallocate/iUsedObjects = %u\n", UINT_32(iUsedObjects));
}

//
// Get object id by
//
UINT_32 FixedSizeAllocator::GetObjectId(const void * vObjectAddr)
{
	return ((CCHAR_P)vObjectAddr - (CCHAR_P)vData) / iObjectSize;
}

//
// Destrcutor
//
FixedSizeAllocator::~FixedSizeAllocator() throw()
{
	free(vData);
	free(aObjectIndex);
}

} // namespace IRIS
// End.
