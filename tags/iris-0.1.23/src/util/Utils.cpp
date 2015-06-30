/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "Utils.hpp"

namespace IRIS
{

//
// Convert string to 64-bit unsigned int
//
UINT_64 Util::StrToUInt(CCHAR_P sData, UINT_64 iDataSize)
{
	UINT_64 iResult = 0;
	for(UINT_64 iPos = 0; iPos < iDataSize; ++iPos)
	{
		const UCHAR_8 ucTMP = *sData;
		if (!(ucTMP >= '0' && ucTMP <= '9')) { break; }
		iResult = iResult * 10 + ucTMP - '0';
		++sData;
	}
return iResult;
}

//
// Convert string to 64-bit int
//
INT_64 Util::StrToInt(CCHAR_P sData, UINT_64 iDataSize)
{
	INT_64 iResult = 0;
	bool bNegative = false;
	if (iDataSize > 0 && *sData == '-')
	{
		bNegative = true;
		++sData;
		--iDataSize;
	}

	for(UINT_64 iPos = 0; iPos < iDataSize; ++iPos)
	{
		const UCHAR_8 ucTMP = *sData;
		if (!(ucTMP >= '0' && ucTMP <= '9')) { break; }
		iResult = iResult * 10 + ucTMP - '0';
		++sData;
	}
return bNegative ? -iResult : iResult;
}

} // namespace IRIS
// End.
