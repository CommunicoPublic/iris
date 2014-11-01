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
fprintf(stdout, "= Insert ===================\n");
	for (INT_32 iPos = 1; iPos < argc; ++iPos)
	{
		const INT_32 iVal = atoi(argv[iPos]);

		if (!oAVLTree.Insert(iVal)) { fprintf(stdout, "%u ok\n", iVal); }
		else                        { fprintf(stdout, "%u duplicate found\n", iVal); }

	}

	STLW::stringstream sStream;
	oAVLTree.Print(sStream);
	fprintf(stdout, "===========================\n");
	fprintf(stdout, "%s\n", sStream.str().c_str());
	fprintf(stdout, "===========================\n");

/*	for (INT_32 iPos = 1; iPos < argc; iPos += 2)
	{
		const INT_32 iVal = atoi(argv[iPos]);

		bool bRemoved = false;
		oAVLTree.Remove(iVal, bRemoved);
		if (bRemoved) { fprintf(stdout, "%u removed\n", iVal); }
		else          { fprintf(stdout, "%u not found\n", iVal); }

	}

	STLW::stringstream sStream;
	oAVLTree.Print(sStream);
	fprintf(stdout, "===========================\n");
	fprintf(stdout, "%s\n", sStream.str().c_str());
	fprintf(stdout, "===========================\n");

*/
fprintf(stdout, "= Find =====================\n");
	for (INT_32 iPos = 1; iPos < argc; ++iPos)
	{
		const INT_32 iVal = atoi(argv[iPos]);

		if (oAVLTree.Find(iVal) == NULL) { fprintf(stdout, "%u not found\n", iVal); }
	}

fprintf(stdout, "= Remove ===================\n");
	for (INT_32 iPos = 1; iPos < argc; ++iPos)
	{
		const INT_32 iVal = atoi(argv[iPos]);

		bool bRemoved = false;
		oAVLTree.Remove(iVal, bRemoved);
		if (bRemoved) { fprintf(stdout, "%u removed\n", iVal); }
		else          { fprintf(stdout, "%u not found\n", iVal); }

	}

	sStream.str() = "";
	oAVLTree.Print(sStream);
	fprintf(stdout, "===========================\n");
	fprintf(stdout, "%s\n", sStream.str().c_str());
	fprintf(stdout, "===========================\n");

return 0;
}
// End.
