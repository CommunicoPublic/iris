/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "MessageDigest.hpp"

#include <stdio.h>
#include <string.h>
#include <sysexits.h>

using namespace IRIS;

int main(int argc, char ** argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "usage: %s string MD5\n", argv[0]);
		return EX_USAGE;
	}

	MD5Hash oHash;
	oHash << argv[1];
	STLW::string sResult = oHash.AsHex();
	fprintf(stdout, "`%s` -> `%s`\n", argv[1], sResult.c_str());
	if (sResult != argv[2])
	{
		fprintf(stdout, "`%s` != `%s`\n", sResult.c_str(), argv[2]);
		return EX_SOFTWARE;
	}

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
