/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "UnicodeUtils.hpp"

#include "STLString.hpp"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

using namespace IRIS;

struct TestFindMap
{
	CCHAR_P       src1;
	UINT_32       src1_len;

	CCHAR_P       src2;
	UINT_32       src2_len;
};

static TestFindMap szTestFindMap[] =
{
	{"foo bar bazoola",  sizeof("foo bar bazoola"),  "foo", sizeof("foo") },
	{"foo bar bazoola",  sizeof("foo bar bazoola"),  "fOo", sizeof("fOo") },
	{"foo bar bazoola",  sizeof("foo bar bazoola"),  "foo1", sizeof("foo1") },
	{"Позвони мне, позвони",  sizeof("Позвони мне, позвони"),  "мне", sizeof("мне") },
	{"Позвони мне, позвони",  sizeof("Позвони мне, позвони"),  "Мне", sizeof("Мне") },
	{ NULL, 0, NULL, 0 }
};

int main(int argc, char ** argv)
{
	FILE * F = stdout;
	if (argc == 2)
	{
		F = fopen(argv[1], "w");
		if (F == NULL)
		{
			fprintf(stderr, "FATAL: Can't open `%s` for writing\n", argv[1]);
			return EX_SOFTWARE;
		}
	}

	fprintf(F, "Find with case check:\n");
	INT_32    iPos = 0;
	while(szTestFindMap[iPos].src1!= NULL)
	{
		fprintf(F, "`%.*s` %d `%.*s`\n", szTestFindMap[iPos].src1_len -1, szTestFindMap[iPos].src1,
		                                 Unicode::Find(szTestFindMap[iPos].src1, szTestFindMap[iPos].src1_len -1, szTestFindMap[iPos].src2, szTestFindMap[iPos].src2_len -1),
		                                 szTestFindMap[iPos].src2_len -1, szTestFindMap[iPos].src2);
		++iPos;
	}

	fprintf(F, "Find ignore case check:\n");
	iPos = 0;
	while(szTestFindMap[iPos].src1!= NULL)
	{
		fprintf(F, "`%.*s` %d `%.*s`\n", szTestFindMap[iPos].src1_len -1, szTestFindMap[iPos].src1,
		                                 Unicode::FindIgnoreCase(szTestFindMap[iPos].src1, szTestFindMap[iPos].src1_len -1, szTestFindMap[iPos].src2, szTestFindMap[iPos].src2_len -1),
		                                 szTestFindMap[iPos].src2_len -1, szTestFindMap[iPos].src2);
		++iPos;
	}

	if (argc == 2) { fclose(F); }

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
