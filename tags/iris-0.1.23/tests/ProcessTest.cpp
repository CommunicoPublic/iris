/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "Process.hpp"

#include <stdio.h>
#include <string.h>
#include <sysexits.h>

using namespace IRIS;

int main(int argc, char ** argv)
{
	ProcPipe oProcPipe;
	oProcPipe.Open("./process-test2 arg1 arg2 arg3");

	CCHAR_P szLine  = "Hello, World!\n";
	INT_64 iLineLen = strlen(szLine);
	INT_64 iBytes   = oProcPipe.Write(szLine, iLineLen);
	fprintf(stderr, "`%s` (%lld) -> %lld\n", szLine, (long long int)iLineLen, (long long int)iBytes);

	CHAR_8 szBuf[4096];
	for (;;)
	{
		iBytes = oProcPipe.Read(szBuf, 4096);
		if (iBytes == 0) { break; }
		fprintf(stderr, "%.*s (%lld)\n", INT_32(iBytes), szBuf, (long long int)iBytes);
	}

	iBytes = oProcPipe.ReadErr(szBuf, 4096);
	fprintf(stderr, "%.*s (%lld)\n", INT_32(iBytes), szBuf, (long long int)iBytes);

	oProcPipe.Close();

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
