/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "LRUCache.hpp"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

using namespace IRIS;

class RestoreCallback
{
public:
	bool Restore(const INT_32  & iKey,
	                   INT_32  & iVal)
	{
		fprintf(stdout, "Restore %d\n", iKey);
		return false;
	}
};

class EvictionCallback
{
public:
	void Evict(const INT_32 iKey, const INT_32 & iVal) { fprintf(stdout, "Evict %d-%d\n", iKey, iVal); }
};

typedef LRUCache<INT_32, INT_32, EvictionCallback, RestoreCallback>  TCache;

static void StateToText(CCHAR_P szAction, const TCache::State  & oRC)
{
	fprintf(stdout, "%s: ", szAction);
	switch(oRC)
	{
		case TCache::OK:
			fprintf(stdout, "OK\n");
			break;
		case TCache::ERROR:
			fprintf(stdout, "ERROR\n");
			break;
		case TCache::NOT_FOUND:
			fprintf(stdout, "NOT_FOUND\n");
			break;
		case TCache::DUPLICATE_FOUND:
			fprintf(stdout, "DUPLICATE_FOUND\n");
			break;
	};
}

int main(int argc, char ** argv)
{
	EvictionCallback oCallback;

	TCache oCache(5, &oCallback);

	TCache::State oRC = oCache.Insert(1, 2);
	StateToText("Insert 1, 2", oRC);

	oCache.Print();

	oRC = oCache.Insert(2, 3);
	StateToText("Insert 2, 3", oRC);

	oCache.Print();

	oRC = oCache.Insert(3, 4);
	StateToText("Insert 3, 4", oRC);

	oCache.Print();

	oRC = oCache.Insert(3, 4);
	StateToText("Insert 3, 4", oRC);

	oCache.Print();

	INT_32 iVal = 0;
	oRC = oCache.Get(3, iVal);
	StateToText("Get 3", oRC);
	fprintf(stdout, "Got %d\n", iVal);

	oCache.Print();

	oRC = oCache.Get(5, iVal);
	StateToText("Get 5", oRC);

	oCache.Print();

	oRC = oCache.Get(1, iVal);
	fprintf(stdout, "Got %d\n", iVal);
	StateToText("Get 1", oRC);

	oCache.Print();


	oRC = oCache.Get(3, iVal);
	fprintf(stdout, "Got %d\n", iVal);
	StateToText("Get 3", oRC);

	oCache.Print();

	oRC = oCache.Release(1);
	StateToText("Release 1", oRC);

	oCache.Print();

	oRC = oCache.Release(3);
	StateToText("Release 3", oRC);

	oCache.Print();

	oRC = oCache.Release(3);
	StateToText("Release 3", oRC);

	oCache.Print();

	oRC = oCache.Release(3);
	StateToText("Release 3", oRC);

	oCache.Print();

	oRC = oCache.Insert(4, 5);
	StateToText("Insert 4, 5", oRC);

	oCache.Print();

	oRC = oCache.Get(4, iVal);
	fprintf(stdout, "Got %d\n", iVal);
	StateToText("Get 4", oRC);

	oRC = oCache.Release(4);
	StateToText("Release 4", oRC);

	oCache.Print();

	oRC = oCache.Release(4);
	StateToText("Release 4", oRC);

	oCache.Print();

	oRC = oCache.Get(4, iVal);
	fprintf(stdout, "Got %d\n", iVal);
	StateToText("Get 4", oRC);

	oCache.Print();

	oRC = oCache.Insert(5, 6);
	StateToText("Insert 5, 6", oRC);

	oCache.Print();

	oRC = oCache.Insert(6, 7);
	StateToText("Insert 6, 7", oRC);

	oCache.Print();

	oRC = oCache.Insert(7, 8);
	StateToText("Insert 7, 8", oRC);

	oCache.Print();

	oRC = oCache.Insert(8, 9);
	StateToText("Insert 8, 9", oRC);

	oCache.Print();

	oRC = oCache.Insert(9, 10);
	StateToText("Insert 9, 10", oRC);

	for (UINT_32 iPos = 4; iPos < 10; ++iPos)
	{
		oRC = oCache.Get(iPos, iVal);
		fprintf(stdout, "Got %d\n", iVal);
		StateToText("Get", oRC);
	}

	oCache.Print();
	oCache.Reset();

	oCache.Print();

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
