/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "StringBuffer.hpp"
#include "Variant.hpp"
#include "VariantDumperJSON.hpp"
#include "JSONParser.hpp"

#include <stdio.h>
#include <string.h>
#include <sysexits.h>

int main(int argc, char ** argv)
{
	using namespace IRIS;

	if (argc != 2)
	{
		fprintf(stderr, "usage: %s JSON\n", argv[0]);
		return EX_USAGE;
	}

	Variant oResult;
	JSONParser<Variant> oParser(oResult);
	oParser.Parse(argv[1], strlen(argv[1]));

	STLW::string sResult;
	StringBuffer oBuffer(sResult);
	VariantDumperJSON oDumper(&oBuffer);

	oResult.Dump(&oDumper);
	oBuffer.Commit();

	fprintf(stdout, "%s\n", sResult.c_str());

return EX_OK;
}
// End.
