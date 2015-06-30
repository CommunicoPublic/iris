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

	if (argc == 1) { return EX_OK; }

	TAVLTree oAVLTree;
	UINT_32 iMax = 0;

	for (INT_32 iPos = 1; iPos < argc; ++iPos)
	{
		UINT_32 iVal = atoi(argv[iPos]);
		if (iMax < iVal) { iMax = iVal; }

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
	fprintf(stderr, "===========================\n");
	fprintf(stdout, "%s\n", sStream.str().c_str());
	fprintf(stderr, "===========================\n");

	fprintf(stderr, "Find: ");
	for (UINT_32 iElt = 0; iElt <= iMax + 1; ++iElt)
	{
		INT_32 * pT = oAVLTree.Find(iElt);
		if (pT != NULL) { fprintf(stderr, "(%2u %2u) ", iElt, *pT); }
		else            { fprintf(stderr, "(%2u  -) ", iElt);      }
	}
	fprintf(stderr, "\n");

	fprintf(stderr, "LowerBound: ");
	for (UINT_32 iElt = 0; iElt <= iMax + 1; ++iElt)
	{
		INT_32 * pT = oAVLTree.LowerBound(iElt);
		if (pT != NULL) { fprintf(stderr, "(%2u %2u) ", iElt, *pT); }
		else            { fprintf(stderr, "(%2u  -) ", iElt);      }
	}
	fprintf(stderr, "\n");

	fprintf(stderr, "Infimum:    ");
	for (UINT_32 iElt = 0; iElt <= iMax + 1; ++iElt)
	{
		INT_32 * pT = oAVLTree.Infimum(iElt);
		if (pT != NULL) { fprintf(stderr, "(%2u %2u) ", iElt, *pT); }
		else            { fprintf(stderr, "(%2u  -) ", iElt);      }
	}
	fprintf(stderr, "\n");

	fprintf(stderr, "Supremum:   ");
	for (UINT_32 iElt = 0; iElt <= iMax + 1; ++iElt)
	{
		INT_32 * pT = oAVLTree.Supremum(iElt);
		if (pT != NULL) { fprintf(stderr, "(%2u %2u) ", iElt, *pT); }
		else            { fprintf(stderr, "(%2u  -) ", iElt);      }
	}
	fprintf(stderr, "\n");

	fprintf(stderr, "UpperBound: ");
	for (UINT_32 iElt = 0; iElt <= iMax + 1; ++iElt)
	{
		INT_32 * pT = oAVLTree.UpperBound(iElt);
		if (pT != NULL) { fprintf(stderr, "(%2u %2u) ", iElt, *pT); }
		else            { fprintf(stderr, "(%2u  -) ", iElt);      }
	}
	fprintf(stderr, "\n");

return EX_OK;
}
// End.
