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

struct TestMap
{
	CCHAR_P       src;
	UINT_32       src_len;
};

static TestMap szTestMap[] =
{
	{ "1234567890abcdefghijklmnopqrstuvwxyz", sizeof("1234567890abcdefghijklmnopqrstuvwxyz") },
	{ "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ", sizeof("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ") },
	{ "1234567890АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ", sizeof("1234567890АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ") },
	{ "1234567890абвгдеёжзийклмнопрстуфхцчшщъыьэюя", sizeof("1234567890абвгдеёжзийклмнопрстуфхцчшщъыьэюя") },
	{ "العربية",          sizeof("العربية")          },
	{ "Žemaitėška",       sizeof("Žemaitėška")       },
	{ "Català",           sizeof("Català")           },
	{ "Español",          sizeof("Español")          },
	{ "Français",         sizeof("Français")         },
	{ "日本語",            sizeof("日本語")           },
	{ "Lietuvių",         sizeof("Lietuvių")         },
	{ "Norsk (bokmål)",   sizeof("Norsk (bokmål)")   },
	{ "Português",        sizeof("Português")        },
	{ "Slovenčina",       sizeof("Slovenčina")       },
	{ "Slovenščina",      sizeof("Slovenščina")      },
	{ "Тоҷикӣ",           sizeof("Тоҷикӣ")           },
	{ "Türkçe",           sizeof("Türkçe")           },
	{ "Українська",       sizeof("Українська")       },
	{ "Vèneto",           sizeof("Vèneto")           },
	{ "Volapük",          sizeof("Volapük")          },
	{ "Herzliche Grüße",  sizeof("Herzliche Grüße")  },
	{ "Сердечный привет", sizeof("Сердечный привет") },
	{ "Серде\200чный привет", sizeof("Серде\200чный привет") },
	{ "F\300\200oo",      sizeof("F\300\200oo")      },
	{ "F\300\200",        sizeof("F\300\200")        },
	{ NULL, 0 }
};

struct TestCompareMap
{
	CCHAR_P       src1;
	UINT_32       src1_len;

	CCHAR_P       src2;
	UINT_32       src2_len;
};

static TestCompareMap szTestCompareMap[] =
{
	{"foo",  sizeof("foo"),  "foo1", sizeof("foo1") },
	{"foo",  sizeof("foo"),  "foo",  sizeof("foo")  },
	{"Foo",  sizeof("Foo"),  "foO",  sizeof("foO")  },
	{"foo1", sizeof("foo1"), "foo",  sizeof("foo")  },

	{"абв",  sizeof("абв"),  "абг",  sizeof("абг")  },
	{"абг",  sizeof("абг"),  "абв",  sizeof("абв")  },

	{"Абв",  sizeof("Абв"),  "абв",  sizeof("абв")  },
	{"Абг",  sizeof("Абг"),  "аБв",  sizeof("аБв")  },
	// Invalid sequences
	{"Аi\200бг",  sizeof("А\200бг"),  "Аi\200бг",  sizeof("А\200бг"),  },
	{"Аi\200бг",  sizeof("А\200бг"),  "Аi\200бг1",  sizeof("А\200бг1"),  },

	{ NULL, 0, NULL, 0 }
};

static CCHAR_P GetSign(const INT_32 iResult)
{
	if (iResult > 0) { return ".GT."; }

	if (iResult < 0) { return ".LT."; }

return ".EQ.";
}

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

	fprintf(F, "Compare with case check:\n");
	INT_32    iPos = 0;
	while(szTestCompareMap[iPos].src1!= NULL)
	{
		fprintf(F, "`%.*s` %s `%.*s`\n", szTestCompareMap[iPos].src1_len -1, szTestCompareMap[iPos].src1,
		                                 GetSign(Unicode::Compare(szTestCompareMap[iPos].src1, szTestCompareMap[iPos].src1_len -1, szTestCompareMap[iPos].src2, szTestCompareMap[iPos].src2_len -1)),
		                                 szTestCompareMap[iPos].src2_len -1, szTestCompareMap[iPos].src2);
		++iPos;
	}

	fprintf(F, "Compare ignore case check:\n");
	iPos = 0;
	while(szTestCompareMap[iPos].src1!= NULL)
	{
		fprintf(F, "`%.*s` %s `%.*s`\n", szTestCompareMap[iPos].src1_len -1, szTestCompareMap[iPos].src1,
		                                 GetSign(Unicode::CompareIgnoreCase(szTestCompareMap[iPos].src1, szTestCompareMap[iPos].src1_len -1, szTestCompareMap[iPos].src2, szTestCompareMap[iPos].src2_len -1)),
		                                 szTestCompareMap[iPos].src2_len -1, szTestCompareMap[iPos].src2);
		++iPos;
	}

	fprintf(F, "Make strings uppercase:\n");
	iPos = 0;
	while(szTestMap[iPos].src!= NULL)
	{
		CHAR_P  szResult = NULL;
		UINT_64 iResultLen = 0;
		const INT_32 iRC = Unicode::Uppercase(szTestMap[iPos].src, szTestMap[iPos].src_len -1, szResult, iResultLen);
		if (iRC == 0)
		{
			fprintf(F, "`%.*s` -> `%.*s`\n", szTestMap[iPos].src_len -1, szTestMap[iPos].src,
			                                 INT_32(iResultLen), szResult);
			free(szResult);
		}
		else
		{
			fprintf(F, "`%.*s` -> ERROR\n", szTestMap[iPos].src_len -1, szTestMap[iPos].src);
		}
		++iPos;
	}

	fprintf(F, "Make strings lowercase:\n");
	iPos = 0;
	while(szTestMap[iPos].src!= NULL)
	{
		CHAR_P  szResult = NULL;
		UINT_64 iResultLen = 0;
		const INT_32 iRC = Unicode::Lowercase(szTestMap[iPos].src, szTestMap[iPos].src_len -1, szResult, iResultLen);
		if (iRC == 0)
		{
			fprintf(F, "`%.*s` -> `%.*s`\n", szTestMap[iPos].src_len -1, szTestMap[iPos].src,
			                                 INT_32(iResultLen), szResult);
			free(szResult);
		}
		else
		{
			fprintf(F, "`%.*s` -> ERROR\n", szTestMap[iPos].src_len -1, szTestMap[iPos].src);
		}
		++iPos;
	}

	if (argc == 2) { fclose(F); }

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
