/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "DataBuffer.hpp"
#include "TimeProfiler.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

using namespace IRIS;

static CCHAR_P szConfession = "Confessio atque poenitentia est sacramentum Ecclesiae Catholicae Romanae et ritus aliarum ecclesiarum Christianarum.";

#define C_BIG_DATA (1 << 24)

int main(int argc, char ** argv)
{
	DataBuffer oBuffer;

	CCHAR_P sIt = szConfession;
	INT_32 iPos = 1;
	while (*sIt != 0)
	{
		oBuffer.Append(*sIt);

		if (iPos % 12 == 0) { fprintf(stdout, "%.*s\n", INT_32(oBuffer.Size()), oBuffer.Data()); }
		++sIt;
		++iPos;
	}
	fprintf(stdout, "%.*s\n", INT_32(oBuffer.Size()), oBuffer.Data());

	TimeProfiler oTimeProfiler;
	oTimeProfiler.StartEvent("Create Buffer");
	CHAR_P szTMP = new CHAR_8[C_BIG_DATA];
	for (UINT_32 iPos = 0; iPos < C_BIG_DATA; ++iPos) { szTMP[iPos] = random() % 20 + '0'; }
	W_FLOAT dTimeDelta = oTimeProfiler.EndEvent("Create Buffer");
	fprintf(stderr, "Create Buffer: %f\n", dTimeDelta);

	DataBuffer oBuffer1;
	oTimeProfiler.StartEvent("Add Char");
	for (UINT_32 iPos = 0; iPos < C_BIG_DATA; ++iPos) { oBuffer1.Append(szTMP[iPos]); }
	W_FLOAT dCharTimeDelta = oTimeProfiler.EndEvent("Add Char");
	fprintf(stderr, "Add Char: %f\n", dCharTimeDelta);

	DataBuffer oBuffer2;
	oTimeProfiler.StartEvent("Add Block");
	oBuffer2.Append(szTMP, C_BIG_DATA);
	W_FLOAT dBlockTimeDelta = oTimeProfiler.EndEvent("Add Block");
	fprintf(stderr, "Add Block: %f\n", dBlockTimeDelta);
	fprintf(stderr, "Char/Block: %f\n", dCharTimeDelta / dBlockTimeDelta);
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
