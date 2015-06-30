/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "UnicodeUtils.hpp"

#include <stdlib.h>

namespace IRIS
{

/**
  @var typedef unsigned int ((*CaseFn)(unsigned int));
*/
typedef UINT_32 ((*CaseFn)(UINT_32));

//
// Make a UTF8 string upper or lowercase
//
static INT_32 ChangeCase(CCHAR_P    szSrc,
                         UINT_64    iSrcLen,
                         CHAR_P   & szDst,
                         UINT_64  & iDstLen,
                         CaseFn     pCaseFn)
{
	// Make buffer safe for overflow
	iDstLen = iSrcLen;

	szDst = (CHAR_P)malloc(sizeof(CHAR_8) * iDstLen);
	if (szDst == NULL) { return -3; }

	UINT_64  iRealDstLen = 0;
	for(;;)
	{
		UINT_32 iUCS = 0;
		// Convert symbol from UTF8 to UCS
		const INT_32 iCharLen = Unicode::UTF8ToWide(szSrc, iSrcLen, iUCS);
		// Incorrect input character
		if (iCharLen == -1)
		{
			free(szDst);
			return -1;
		}
		iSrcLen -= iCharLen;

		// Change case
		iUCS = ((*pCaseFn)(iUCS));

		CHAR_P szTMPDst = szDst + iRealDstLen;
		// Convert symbol from UCS to UTF8
		const INT_32 iProcessedCharLen = Unicode::WideToUTF8(iUCS, szTMPDst);
		// Incorrect lowercase character; I hope, this should NEVER happened
		if (iProcessedCharLen == -1)
		{
			free(szDst);
			return -2;
		}
		// Store processed size
		iRealDstLen += iProcessedCharLen;

		// Check free memory in destination buffer
		if (iRealDstLen == iDstLen)
		{
			CHAR_P sReallocated = (CHAR_P)realloc(szDst, sizeof(CHAR_8) * iRealDstLen);
			// Cannot reallocate memory
			if (sReallocated == NULL)
			{
				free(szDst);
				return -3;
			}
			szDst = sReallocated;
		}

		// Exit condition
		if (iSrcLen == 0) { break; }
	}

	iDstLen = iRealDstLen;

return 0;
}

//
// Make a UTF8 string lowercase
//
INT_32 Unicode::Lowercase(CCHAR_P    szSrc,
                          UINT_64    iSrcLen,
                          CHAR_P   & szDst,
                          UINT_64  & iDstLen)
{

return ChangeCase(szSrc, iSrcLen, szDst, iDstLen, CharTable::Lowercase);
}

//
//  Make a UTF8 string uppercase
//
INT_32 Unicode::Uppercase(CCHAR_P    szSrc,
                          UINT_64    iSrcLen,
                          CHAR_P   & szDst,
                          UINT_64  & iDstLen)
{

return ChangeCase(szSrc, iSrcLen, szDst, iDstLen, CharTable::Uppercase);
}

} // namespace IRIS
// End.
