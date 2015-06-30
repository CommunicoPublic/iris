/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "FixedSizeAllocator.hpp"
#include "TimeProfiler.hpp"
#include <STLVector.hpp>
#include <STLAlgorithm.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

using namespace IRIS;

static const UINT_32 C_ITERATIONS    = 10000;

//static const UINT_32 C_START_SIZE    = 128;
static const UINT_32 C_START_SIZE    = 128;

int main(void)
{

	UINT_32 iSize = C_START_SIZE;

	fprintf(stderr, "size allocator malloc\n");
	for (;;)
	{
		TimeProfiler oTimeProfiler;
		W_FLOAT dTimeDelta1 = 0.0;
		W_FLOAT dTimeDelta2 = 0.0;
		{
			FixedSizeAllocator oFixedSizeAllocator(iSize, 100000);

			oTimeProfiler.StartEvent("FixedSizeAllocator");
			for (UINT_32 iPos = 0; iPos < C_ITERATIONS; ++iPos)
			{
				void * vV1 = oFixedSizeAllocator.Allocate(iSize);
//				memset(vV1, 'X', iSize);

				void * vV2 = oFixedSizeAllocator.Allocate(iSize);
//				memset(vV2, 'X', iSize);

				void * vV3 = oFixedSizeAllocator.Allocate(iSize);
//				memset(vV3, 'X', iSize);

				void * vV4 = oFixedSizeAllocator.Allocate(iSize);
//				memset(vV4, 'X', iSize);

				void * vV5 = oFixedSizeAllocator.Allocate(iSize);
//				memset(vV5, 'X', iSize);

				oFixedSizeAllocator.Deallocate(vV2);
				oFixedSizeAllocator.Deallocate(vV4);

				void * vV6 = oFixedSizeAllocator.Allocate(iSize);
//				memset(vV6, 'X', iSize);

				void * vV7 = oFixedSizeAllocator.Allocate(iSize);
//				memset(vV7, 'X', iSize);

				oFixedSizeAllocator.Deallocate(vV1);
				oFixedSizeAllocator.Deallocate(vV3);
				oFixedSizeAllocator.Deallocate(vV5);
				oFixedSizeAllocator.Deallocate(vV6);
				oFixedSizeAllocator.Deallocate(vV7);
			}
			dTimeDelta1 = oTimeProfiler.EndEvent("FixedSizeAllocator");
		}

		{
			oTimeProfiler.StartEvent("Malloc/Free");
			for (UINT_32 iPos = 0; iPos < C_ITERATIONS; ++iPos)
			{
				void * vV1 = malloc(iSize);
//				memset(vV1, 'X', iSize);

				void * vV2 = malloc(iSize);
//				memset(vV2, 'X', iSize);

				void * vV3 = malloc(iSize);
//				memset(vV3, 'X', iSize);

				void * vV4 = malloc(iSize);
//				memset(vV4, 'X', iSize);

				void * vV5 = malloc(iSize);
//				memset(vV5, 'X', iSize);

				free(vV2);
				free(vV4);

				void * vV6 = malloc(iSize);
//				memset(vV6, 'X', iSize);

				void * vV7 = malloc(iSize);
//				memset(vV7, 'X', iSize);

				free(vV1);
				free(vV3);
				free(vV5);
				free(vV6);
				free(vV7);
			}
			dTimeDelta2 = oTimeProfiler.EndEvent("Malloc/Free");
		}
		fprintf(stderr, "%u %f %f\n", iSize, dTimeDelta1, dTimeDelta2);

		iSize <<= 1;
		// 64 MBytes
		if (iSize == (128 << 20)) { break; }
	}

	iSize = C_START_SIZE;
	fprintf(stderr, "size objects allocator malloc\n");
	for(;;)
	{
		TimeProfiler oTimeProfiler;
		W_FLOAT dTimeDelta1 = 0.0;
		W_FLOAT dTimeDelta2 = 0.0;
		const UINT_32 iMaxBytes = 128 * 5000000;
		const UINT_32 iMaxObjects = iMaxBytes / iSize;

		{
			FixedSizeAllocator oFixedSizeAllocator(iSize, 5000000);
			STLW::vector<void *> vBlocks(iMaxObjects);

			oTimeProfiler.StartEvent("FixedSizeAllocator");
			for (UINT_32 iPos = 0; iPos < iMaxObjects; ++iPos)
			{
				vBlocks.push_back(oFixedSizeAllocator.Allocate(iSize));
			}

			// Shuffle
			STLW::random_shuffle(vBlocks.begin(), vBlocks.end());

			for (UINT_32 iPos = 0; iPos < iMaxObjects; ++iPos)
			{
				oFixedSizeAllocator.Deallocate(vBlocks[iPos]);
			}
			dTimeDelta1 = oTimeProfiler.EndEvent("FixedSizeAllocator");
		}

		{
			STLW::vector<void *> vBlocks(iMaxObjects);

			oTimeProfiler.StartEvent("Malloc/Free");
			for (UINT_32 iPos = 0; iPos < iMaxObjects; ++iPos)
			{
				vBlocks.push_back(malloc(iSize));
			}

			// Shuffle
			STLW::random_shuffle(vBlocks.begin(), vBlocks.end());

			for (UINT_32 iPos = 0; iPos < iMaxObjects; ++iPos)
			{
				free(vBlocks[iPos]);
			}

			dTimeDelta2 = oTimeProfiler.EndEvent("Malloc/Free");
		}

		fprintf(stderr, "%u %u %f %f\n", iSize, iMaxObjects, dTimeDelta1, dTimeDelta2);

		iSize <<= 1;
		// 64 MBytes
		if (iSize == (128 << 20)) { break; }
	}

return 0;
}
// End.
