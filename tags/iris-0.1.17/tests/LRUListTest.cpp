/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

// C++ Includes
#include "LRUList.hpp"

using namespace IRIS;

int main(void)
{
	typedef LRUList<INT_32> TIntList;

	fprintf(stderr, "#1\n");
	{
		TIntList oList;
		oList.Print();

		TIntList::Iterator oIt1 = oList.Add(1);
		oList.Print();

		oList.Remove(oIt1);
		oList.Print();
	}

	fprintf(stderr, "#2\n");
	{
		TIntList oList;
		oList.Print();

		TIntList::Iterator oIt1 = oList.Add(1);
		oList.Print();

		TIntList::Iterator oIt2 = oList.Add(2);
		oList.Print();

		oList.Remove(oIt2);
		oList.Print();

		oList.Remove(oIt1);
		oList.Print();
	}

	fprintf(stderr, "#3\n");
	{
		TIntList oList;
		oList.Print();

		TIntList::Iterator oIt1 = oList.Add(1);
		oList.Print();

		TIntList::Iterator oIt2 = oList.Add(2);
		oList.Print();

		oList.Remove(oIt1);
		oList.Print();

		oList.Remove(oIt2);
		oList.Print();
	}

	fprintf(stderr, "#4\n");
	{
		TIntList oList;
		oList.Print();

		TIntList::Iterator oIt1 = oList.Add(1);
		oList.Print();

		TIntList::Iterator oIt2 = oList.Add(2);
		oList.Print();

		TIntList::Iterator oIt3 = oList.Add(3);
		oList.Print();

		TIntList::Iterator oIt4 = oList.Add(4);
		oList.Print();

		oList.Remove(oIt3);
		oList.Print();

		oList.Remove(oIt1);
		oList.Print();

		oList.Remove(oIt2);
		oList.Print();

		oList.Remove(oIt4);
		oList.Print();
	}

	fprintf(stderr, "#5\n");
	{
		TIntList oList;
		oList.Print();

		TIntList::Iterator oIt1 = oList.Add(1);
		oList.Print();

		TIntList::Iterator oIt2 = oList.Add(2);
		oList.Print();

		TIntList::Iterator oIt3 = oList.Add(3);
		oList.Print();
	}

	fprintf(stderr, "#6\n");
	{
		TIntList oList;
		oList.Print();

		TIntList::Iterator oIt1 = oList.Add(1);
		oList.Print();

		oIt1 = 7;
		fprintf(stderr, "Value: %i\n", INT_32(oIt1));

		oList.Remove(oIt1);
		oList.Print();
	}

	fprintf(stderr, "#7\n");
	{
		TIntList oList;
		for (UINT_32 iElt = 0; iElt < 10; ++iElt)
		{
			oList.Add(iElt);
		}
		oList.Print();
		TIntList::Iterator oIt = oList.Begin();
		while (oIt != oList.End())
		{
			fprintf(stderr, "Value: %i\n", INT_32(oIt));
			++oIt;
		}
	}

	fprintf(stderr, "#8\n");
	{
		TIntList oList;
		UINT_32 iElt = 0;
		TIntList::Iterator oLast;
		for (; iElt < 10; ++iElt) { oLast = oList.Add(iElt); }

		TIntList::Iterator oIt = oList.Begin();
		while (oIt != oList.End())
		{
			fprintf(stderr, "Value: %i\n", INT_32(oIt));
			++oIt;
		}

		oList.Print();

		// Move last element
		oList.MoveOnTop(oLast);
		oList.Print();

		// Move first element on top
		oIt = oList.Begin();
		oList.MoveOnTop(oIt);

		oList.Print();

		oIt = oList.Begin();
		++oIt;
		oList.MoveOnTop(oIt);

		oList.Print();

		oIt = oList.Begin();
		while (oIt != oList.End())
		{
			fprintf(stderr, "Value: %i\n", INT_32(oIt));
			++oIt;
		}
	}

return 0;
}
// End.
