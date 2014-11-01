/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "CRC32.hpp"

#include <stdio.h>
#include <string.h>
#include <sysexits.h>

using namespace IRIS;

int main(int argc, char ** argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "usage: %s string CRC32\n", argv[0]);
		return EX_USAGE;
	}

	CRC32 oCRC32;
	oCRC32 << argv[1];
	UINT_32 iResult = oCRC32.AsInt();
	fprintf(stdout, "`%s` -> `0x%08X`\n", argv[1], iResult);
	unsigned int iArgv2 = 0;
	sscanf(argv[2], "%x", &iArgv2);
	if (iResult != iArgv2)
	{
		fprintf(stdout, "`0x%08X` != `0x%08X`\n", iResult, iArgv2);
		return EX_SOFTWARE;
	}

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
