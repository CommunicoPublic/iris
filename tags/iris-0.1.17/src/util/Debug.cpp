/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "Debug.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace IRIS
{

//
// Last words
//
static CCHAR_P LastWords()
{
	static CCHAR_P aPanic[] = { "Oink-Oink!!!",
	                            "Drink to me, drink to my health. You know I can't drink anymore.",
	                            "I found Rome brick, I leave it marble.",
	                            "Oh, do not cry - be good children and we will all meet in heaven.",
	                            "\\|/ ____ \\|/\n"
	                            "\"@'/ ,. \\`@\"\n"
	                            "/_| \\__/ |_\\\n"
	                            "   \\__U_/\n"
	                          };
	const UINT_32 iPos = (random() % (sizeof(aPanic) / sizeof(*aPanic)));

return aPanic[iPos];
}

//
// Check for unrecoverable error
//
void Debug::Abort(CCHAR_P szWhere)
{
	fprintf(stderr, "%s: aborting program\n%s\n", szWhere, LastWords());
	fflush(stderr);
	abort();
}

//
// Print error description
//
STLW::string Debug::PrintableError(const INT_32  iRC)
{
	if (iRC == 0) { return "no error"; }

	char szErrorBuf[1024 + 1];
// Linux brain-damaged multithread design
#if !linux || ((_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE)
	strerror_r(iRC, szErrorBuf, 1024);
	char * szError = szErrorBuf;
#else
	char * szError = strerror_r(iRC, szErrorBuf, 1024);
#endif
return szError;
}

//
// Check for unrecoverable error
//
void Debug::CheckUnrecoverable(CCHAR_P szWhere, const INT_32 iRC)
{
	if (iRC == 0) { return; }

	char szErrorBuf[1024 + 1];
// Linux brain-damaged multithread design
#if !linux || ((_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE)
	strerror_r(iRC, szErrorBuf, 1024);
	char * szError = szErrorBuf;
#else
	char * szError = strerror_r(iRC, szErrorBuf, 1024);
#endif

#ifdef IRIS_DIE_ON_UNRECOVERABLE
	fprintf(stderr, "%s: %s, error code is %d, aborting program\n%s\n", szWhere, szError, iRC, LastWords());
	fflush(stderr);
	abort();
#else
	fprintf(stderr, "%s: %s, error code is %d\n%s\n", szWhere, szError, iRC, LastWords());
	fflush(stderr);
#endif // _DIE_ON_UNRECOVERABLE
}

} // namespace IRIS
// End.
