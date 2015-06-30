/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "BlockAllocator.hpp"

#include <stdio.h>
#include <sysexits.h>

using namespace IRIS;

int main(void)
{

	{
		BlockAllocator oBlockAllocator(128, 1024);

		void * vData1 = oBlockAllocator.Allocate(110);
		oBlockAllocator.Deallocate(vData1);
	}

	{
		BlockAllocator oBlockAllocator(128, 1024);

		void * vData1 = oBlockAllocator.Allocate(110);
		void * vData2 = oBlockAllocator.Allocate(112);

		oBlockAllocator.Deallocate(vData2);
		oBlockAllocator.Deallocate(vData1);
	}

	{
		BlockAllocator oBlockAllocator(128, 1024);

		void * vData1 = oBlockAllocator.Allocate(110);
		void * vData2 = oBlockAllocator.Allocate(112);
		void * vData3 = oBlockAllocator.Allocate(114);
		void * vData4 = oBlockAllocator.Allocate(116);
		void * vData5 = oBlockAllocator.Allocate(118);

		oBlockAllocator.Deallocate(vData2);
		oBlockAllocator.Deallocate(vData4);
		oBlockAllocator.Deallocate(vData3);

		oBlockAllocator.Deallocate(vData5);
		oBlockAllocator.Deallocate(vData1);

	}

	{
		BlockAllocator oBlockAllocator(128, 8);

		void * vData1 = oBlockAllocator.Allocate(110);
		void * vData2 = oBlockAllocator.Allocate(112);
		void * vData3 = oBlockAllocator.Allocate(114);
		void * vData4 = oBlockAllocator.Allocate(116);
		void * vData5 = oBlockAllocator.Allocate(118);

		oBlockAllocator.Deallocate(vData2);
		oBlockAllocator.Deallocate(vData4);
		oBlockAllocator.Deallocate(vData3);

		oBlockAllocator.Deallocate(vData5);
		oBlockAllocator.Deallocate(vData1);
	}

	{
		BlockAllocator oBlockAllocator(128, 8);
		void * vData1 = oBlockAllocator.Allocate(1100);
		fprintf(stderr, "%p\n", vData1);
	}

return 0;
}
// End.
