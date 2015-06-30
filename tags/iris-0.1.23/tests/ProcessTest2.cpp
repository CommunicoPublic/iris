/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "Types.h"

#include <stdio.h>
#include <string.h>
#include <sysexits.h>

static void Reverse(CHAR_P szLine)
{
	const INT_32 iLen = strlen(szLine);
	for (INT_32 iPos = 0; iPos < iLen / 2; ++iPos)
	{
		CHAR_8 chTMP = szLine[iPos];
		szLine[iPos] = szLine[iLen - iPos - 1];
		szLine[iLen - iPos - 1] = chTMP;
	}
}

int main(int argc, char ** argv)
{
	fprintf(stdout, "Progname: %s, args %d\n", argv[0], argc - 1);

	for(INT_32 iPos = 1; iPos < argc; ++iPos)
	{
		fprintf(stdout, "Arg[%d] = %s\n", iPos - 1, argv[iPos]);
	}
	fprintf(stdout, "\n");

	fprintf(stderr, "This is error message");

	CHAR_8 szLine[4096];
	if (fgets(szLine, 4096, stdin) != NULL)
	{
		Reverse(szLine);
		puts(szLine);
	}

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
