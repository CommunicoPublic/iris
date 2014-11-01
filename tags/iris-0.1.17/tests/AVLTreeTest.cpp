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

using namespace IRIS;

int main(int argc, char ** argv)
{
	typedef AVLTree<INT_32> TAVLTree;

	TAVLTree oAVLTree;

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
	fprintf(stdout, "===========================\n");
	fprintf(stdout, "%s\n", sStream.str().c_str());
	fprintf(stdout, "===========================\n");

return 0;
}
// End.
