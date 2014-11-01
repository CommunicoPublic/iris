/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

// C++ Includes
#include "AVLTree.hpp"
#include "FixedSizeAllocator.hpp"

using namespace IRIS;

int main(int argc, char ** argv)
{
	typedef AVLTree<INT_32, STLW::greater<INT_32>, FixedSizeAllocator> TAVLTree;

	STLW::greater<INT_32> oGreater;
	FixedSizeAllocator oAllocator(TAVLTree::NodeSize(), 1000);

	TAVLTree oAVLTree(&oGreater, &oAllocator);

	for (INT_32 iPos = 1; iPos < argc; ++iPos)
	{
		INT_32 iVal = atoi(argv[iPos]);

		if (!oAVLTree.Find(iVal))
		{
			oAVLTree.Insert(iVal);
		}
		else
		{
			oAVLTree.Remove(iVal);
		}
	}

	STLW::stringstream sStream;
	oAVLTree.Print(sStream);
	fprintf(stderr, "Size: %llu\n", (long long unsigned)oAVLTree.Size());
	fprintf(stderr, "===========================\n");
	fprintf(stdout, "%s\n", sStream.str().c_str());
	fprintf(stderr, "===========================\n");

return 0;
}
// End.
