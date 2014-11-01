/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "STLString.hpp"
#include "UnicodeUtils.hpp"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

using namespace IRIS;

static void PrintHex(FILE * F, CCHAR_P szData, const UINT_64 iDataLen)
{
	for (UINT_64 iPos = 0; iPos < iDataLen; ++iPos)
	{
		fprintf(F, "0x%2X ", UCHAR_8(*szData));
		szData++;
	}
	fprintf(F, "\n");
}

static void IterateString(FILE * F, CCHAR_P szData, UINT_64 iDataLen, STLW::string & sResultString)
{
	STLW::string::iterator itsResultString = sResultString.begin();
	for(;;)
	{
		UINT_32 iUCS = 0;
		const INT_64 iCharLen = Unicode::UTF8ToWide(szData, iDataLen, iUCS);
		if (iCharLen <= 0) { break; }

		Unicode::WideToUTF8(iUCS, itsResultString);

		iDataLen -= iCharLen;
		fprintf(F, "0x%X ", iUCS);
	}
	fprintf(F, "\n");
}

int main(int argc, char ** argv)
{
	if (argc != 2 && argc != 3)
	{
		fprintf(stderr, "usage: %s string [file]\n", argv[0]);
		return EX_USAGE;
	}

	FILE * F = stdout;
	if (argc == 3)
	{
		F = fopen(argv[2], "w");
		if (F == NULL)
		{
			fprintf(stderr, "FATAL: Can't open `%s` for writing\n", argv[2]);
			return EX_SOFTWARE;
		}
	}

	const STLW::string sString(argv[1]);

	fprintf(F, "Length: %d\n", Unicode::UTF8Length(sString.data(), sString.size()));
	PrintHex(F, sString.data(), sString.size());

	STLW::string sResultString(sString.size(), ' ');
	IterateString(F, sString.data(), sString.size(), sResultString);

	PrintHex(F, sResultString.data(), sResultString.size());

	if (sResultString != sString)
	{
		fprintf(F, "ERROR: `%s` != `%s`\n", sString.c_str(), sResultString.c_str());
		return EX_SOFTWARE;
	}

	if (argc == 3) { fclose(F); }

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
