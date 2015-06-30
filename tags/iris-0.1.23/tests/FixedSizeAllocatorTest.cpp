/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "FixedSizeAllocator.hpp"

#include <stdio.h>
#include <sysexits.h>

using namespace IRIS;

int main(void)
{
	FixedSizeAllocator oFixedSizeAllocator(128, 1000);

	void * vV1 = oFixedSizeAllocator.Allocate(128);
	fprintf(stderr, "Allocate %p\n", vV1);

	void * vV2 = oFixedSizeAllocator.Allocate(128);
	fprintf(stderr, "Allocate %p\n", vV2);

	void * vV3 = oFixedSizeAllocator.Allocate(128);
	fprintf(stderr, "Allocate %p\n", vV3);

	void * vV4 = oFixedSizeAllocator.Allocate(128);
	fprintf(stderr, "Allocate %p\n", vV4);

	void * vV5 = oFixedSizeAllocator.Allocate(128);
	fprintf(stderr, "Allocate %p\n", vV5);

	oFixedSizeAllocator.Deallocate(vV2);
	fprintf(stderr, "Deallocate %p\n", vV2);
	oFixedSizeAllocator.Deallocate(vV4);
	fprintf(stderr, "Deallocate %p\n", vV4);

	void * vV6 = oFixedSizeAllocator.Allocate(128);
	fprintf(stderr, "Allocate %p\n", vV6);

	void * vV7 = oFixedSizeAllocator.Allocate(128);
	fprintf(stderr, "Allocate %p\n", vV7);

	oFixedSizeAllocator.Deallocate(vV1);
	oFixedSizeAllocator.Deallocate(vV3);
	oFixedSizeAllocator.Deallocate(vV5);
	oFixedSizeAllocator.Deallocate(vV6);
	oFixedSizeAllocator.Deallocate(vV7);

return 0;
}
// End.
