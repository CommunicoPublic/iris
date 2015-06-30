/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

//
// Convert wide character to UTF-8
//
template <typename T>INT_32 Unicode::WideToUTF8(UINT_32   iUCS,
                                                T       & itsUTF8)
{
	INT_32   iCharLength = 0;
	UCHAR_8  sUTF8Prefix = 0;

	// ASCII characters.
	if ((iUCS & ~0x0000007F) == 0)
	{
		// Modified UTF-8, special case
		if (iUCS == 0)
		{
			*itsUTF8 = CHAR_8(0xC0);
			++itsUTF8;

			*itsUTF8 = CHAR_8(0x80);
			++itsUTF8;
			return 2;
		}

		// Check for TAB, CR and LF
		if (iUCS < ' ' && iUCS != '\t' && iUCS != '\n' && iUCS != '\r') { return -1; }

		*itsUTF8 = (char)iUCS;
		++itsUTF8;
		return 1;
	}

	if      ((iUCS & ~0x000007FF) == 0)
	{
		sUTF8Prefix = 0xC0; // 11000000b
		iCharLength = 2;
	}
	else if ((iUCS & ~0x0000FFFF) == 0)
	{
		sUTF8Prefix = 0xE0; // 11100000b
		iCharLength = 3;
	}
	else if ((iUCS & ~0x001FFFFF) == 0)
	{
		sUTF8Prefix = 0xF0; // 11110000b
		iCharLength = 4;
	}
	else if ((iUCS & ~0x03FFFFFF) == 0)
	{
		sUTF8Prefix = 0xF8; // 11111000b
		iCharLength = 5;
	}
	else if ((iUCS & ~0x7FFFFFFF) == 0)
	{
		sUTF8Prefix = 0xFC; // 11111100b
		iCharLength = 6;
	}
	// Incorrect multibyte character
	else { return -1; }

	/*
	 * Convert UCS character to UTF8. Split value in 6-bit chunks and
	 * move to UTF8 string
	 */
	UCHAR_8 aTMP[6];
	for (INT_32 iPos = iCharLength - 1; iPos > 0; --iPos)
	{
		aTMP[iPos] = (iUCS & 0x0000003F) | 0x80;
		iUCS >>= 6;
	}
	// UTF8 prefix, special case
	aTMP[0] = (iUCS & 0x000000FF) | sUTF8Prefix;

	// Move data
	for (INT_32 iPos = 0; iPos < iCharLength; ++iPos) { *itsUTF8++ = aTMP[iPos]; }

// Return size of UTF8 character
return iCharLength;
}

//
// Convert UTF-8 character to wide
//
template <typename T>INT_32 Unicode::UTF8ToWide(T             & itsIUTF8,
                                                const UINT_64   iBufferLength,
                                                UINT_32       & iUCS)
{
	T itsUTF8 = itsIUTF8;

	// No data
	if (iBufferLength == 0) { return 0; }

	// Determine size of UTF8 character & check "shortest form" of char

	UCHAR_8 ucPrefix = (UCHAR_8)(*itsUTF8);

	UINT_32  iCharLength = 0;
	UINT_32  uBoundary   = 0;

	// 10000000b  & 00000000b
	if ((ucPrefix & 0x80) == 0)
	{
		ucPrefix    = 0x7F;
		iCharLength = 1;
		uBoundary   = 0;
	}
	// 11100000b & 11000000b
	else if ((ucPrefix & 0xE0) == 0xC0)
	{
		ucPrefix    = 0x1F;
		iCharLength = 2;
		uBoundary   = 0x80;
	}
	// 11110000b & 11100000b
	else if ((ucPrefix & 0xF0) == 0xE0)
	{
		ucPrefix    = 0x0F;
		iCharLength = 3;
		uBoundary   = 0x00000800;
	}
	// 11111000b & 11110000b
	else if ((ucPrefix & 0xF8) == 0xF0)
	{
		ucPrefix    = 0x07;
		iCharLength = 4;
		uBoundary   = 0x00010000;
	}
	// 11111100b & 11111000b
	else if ((ucPrefix & 0xFC) == 0xF8)
	{
		ucPrefix    = 0x03;
		iCharLength = 5;
		uBoundary   = 0x00200000;
	}
	// 11111110b & 11111100b
	else if ((ucPrefix & 0xFE) == 0xFC)
	{
		ucPrefix    = 0x01;
		iCharLength = 6;
		uBoundary   = 0x04000000;
	}
	// Invalid UTF8 sequence
	else { return -1; }

	// Invalid UTF8 sequence
	if (iBufferLength < iCharLength) { return -1; }

	// Special case for first character
	iUCS = (UCHAR_8)(*itsUTF8) & ucPrefix;
	++itsUTF8;
	for (UINT_32 iPos = 1; iPos < iCharLength; ++iPos)
	{
		// Incorect characters in middle of string
		if (((*itsUTF8) & 0xC0) != 0x80) { return -1; }

		// Join value from 6-bit chunks
		iUCS <<= 6;
		iUCS |= (*itsUTF8) & 0x3F;
		++itsUTF8;
	}

	// Check boundary
	if (iUCS < uBoundary)
	{
		// Modified UTF-8, special case
		if (!(iUCS == 0 && uBoundary == 0x80))
		{
			// Not a "shortest form" of char
			return -1;
		}
	}

	itsIUTF8 = itsUTF8;

return iCharLength;
}

//
// Convert UTF-8 character to wide
//
template <typename T>INT_32 Unicode::UTF8ToWide(T             & itsIUTF8,
                                                const T       & itsIUTF8End,
                                                UINT_32       & iUCS)
{
	return Unicode::UTF8ToWide(itsIUTF8, itsIUTF8End - itsIUTF8, iUCS);
}

//
// Get length of string
//
template <typename T> INT_32 Unicode::UTF8Length(T              itsIUTF8,
	                                         const UINT_64  iBufferLength)
{
	UINT_64 iDataLen = iBufferLength;
	INT_32 iLength = 0;
	for(;;)
	{
		UINT_32 iUCS = 0;
		const INT_64 iCharLen = Unicode::UTF8ToWide(itsIUTF8, iDataLen, iUCS);
		if (iCharLen < 0) { return -1; }
		if (iCharLen == 0) { return iLength; }

		iDataLen -= iCharLen;
		++iLength;
	}
// Make compiler happy
return 0;
}

//
// Get length of string
//
template <typename T> INT_32 Unicode::UTF8Length(T              itsIUTF8,
	                                         const T       & itsIUTF8End)
{
	return Unicode::UTF8Length(itsIUTF8, itsIUTF8End - itsIUTF8);
}

//
// Compare two utf-8 strings
//
template <typename T> INT_32 Unicode::Compare(T        itsFirst,
                                              UINT_64  iFirstLength,
                                              T        itsSecond,
                                              UINT_64  iSecondLength)
{
	for(;;)
	{
		// if 1-st string is empty and second has wrong symbol at the beginning
		// then we must ensure -1 is returned (and vice versa)
		if (iFirstLength == 0 || iSecondLength == 0)
		{
			const INT_32 iResult = iFirstLength - iSecondLength;
			if (iResult < 0) { return -1; }
			if (iResult > 0) { return  1; }

			return 0;
		}

		// Convert symbol from UTF8 to UCS
		UINT_32 iUCS1 = 0;
		const INT_32 iCharLen1 = Unicode::UTF8ToWide(itsFirst, iFirstLength, iUCS1);

		// Convert symbol from UTF8 to UCS
		UINT_32 iUCS2 = 0;
		const INT_32 iCharLen2 = Unicode::UTF8ToWide(itsSecond, iSecondLength, iUCS2);

		/*
		 * both strings have wrong char at the same position - treat them as equal
		 * otherwise everything is wrong e.g. let X = 'some wrong string' then X < X which is wrong
		 */
		if (iCharLen1 == -1 && iCharLen2 == -1) { return 0; }
		// Incorrect input character  in first string
		if (iCharLen1 == -1) { return -2; }
		// Incorrect input character in second string
		if (iCharLen2 == -1) { return 2; }

		iFirstLength -= iCharLen1;
		iSecondLength -= iCharLen2;

		if (iUCS1 != iUCS2)
		{
			if (iUCS1 < iUCS2) { return -1; }

			return 1;
		}
	}

// Make compiler happy
return 0;
}

//
// Compare two utf-8 strings
//
template <typename T> INT_32 Unicode::Compare(const T  itsFirst,
                                              const T  itsFirstEnd,
                                              const T  itsSecond,
                                              const T  itsSecondEnd)
{
	return Unicode::Compare(itsFirst, itsFirstEnd - itsFirst, itsSecondEnd, itsSecond);
}

//
// Compare two utf-8 strings
//
template <typename T> INT_32 Unicode::CompareIgnoreCase(T        itsFirst,
                                                        UINT_64  iFirstLength,
                                                        T        itsSecond,
                                                        UINT_64  iSecondLength)
{
	for(;;)
	{
		// if 1-st string is empty and second has wrong symbol at the beginning
		// then we must ensure -1 is returned (and vice versa)
		if (iFirstLength == 0 || iSecondLength == 0)
		{
			const INT_32 iResult = iFirstLength - iSecondLength;
			if (iResult < 0) { return -1; }
			if (iResult > 0) { return  1; }

			return 0;
		}

		// Convert symbol from UTF8 to UCS
		UINT_32 iUCS1 = 0;
		INT_32 iCharLen1 = Unicode::UTF8ToWide(itsFirst, iFirstLength, iUCS1);

		// Convert symbol from UTF8 to UCS
		UINT_32 iUCS2 = 0;
		INT_32 iCharLen2 = Unicode::UTF8ToWide(itsSecond, iSecondLength, iUCS2);

		/*
		 * both strings have wrong char at the same position - treat them as equal
		 * otherwise everything is wrong e.g. let X = 'some wrong string' then X < X which is wrong
		 */
		if (iCharLen1 == -1 && iCharLen2 == -1)
		{
			iCharLen1 = iCharLen2 = 1;
			iUCS1 = iUCS2 = 0;
		}

		// Incorrect input character  in first string
		if (iCharLen1 == -1) { return -2; }
		// Incorrect input character in second string
		if (iCharLen2 == -1) { return 2; }

		iFirstLength -= iCharLen1;
		iSecondLength -= iCharLen2;

		if (iUCS1 != iUCS2)
		{
			// Lowercase all
			iUCS1 = CharTable::Lowercase(iUCS1);
			iUCS2 = CharTable::Lowercase(iUCS2);

			// Check sorting order
			if (iUCS1 != iUCS2)
			{
				iUCS1 = CharTable::LowercaseSortOrder(iUCS1);
				iUCS2 = CharTable::LowercaseSortOrder(iUCS2);

				if (iUCS1 < iUCS2) { return -1; }

				return 1;
			}
		}
	}

// Make compiler happy
return 0;
}

//
// Compare two utf-8 strings
//
template <typename T> INT_32 Unicode::CompareIgnoreCase(const T  itsFirst,
                                                        const T  itsFirstEnd,
                                                        const T  itsSecond,
                                                        const T  itsSecondEnd)
{
	return Unicode::CompareIgnoreCase(itsFirst, itsFirstEnd - itsFirst, itsSecondEnd, itsSecond);
}

//
//  Find substring, ignore case
//
template <typename T> INT_32 Unicode::Find(T        itsFirst,
                                           UINT_64  iFirstLength,
                                           const T  itsSecond,
                                           UINT_64  iSecondLength)
{
	INT_32 iPos = 0;
	for(;;)
	{
		if (iFirstLength < iSecondLength) { return -1; }
		const INT_32 iRC = Unicode::Compare(itsFirst, iSecondLength, itsSecond, iSecondLength);
		if (iRC == 0) { return iPos; }

		UINT_32 iUCS1 = 0;
		INT_32 iCharLen1 = Unicode::UTF8ToWide(itsFirst, iFirstLength, iUCS1);
		if (iCharLen1 == -1) { iCharLen1 = 1; }

		iFirstLength -= iCharLen1;

		iPos += iCharLen1;
	}

return -1;
}

//
//  Find substring, ignore case
//
template <typename T> INT_32 Unicode::Find(const T  itsFirst,
                                           const T  itsFirstEnd,
                                           const T  itsSecond,
                                           const T  itsSecondEnd)
{
        return Unicode::Find(itsFirst, itsFirstEnd - itsFirst, itsSecondEnd, itsSecond);
}

//
//  Find substring, ignore case
//
template <typename T> INT_32 Unicode::FindIgnoreCase(T        itsFirst,
                                                     UINT_64  iFirstLength,
                                                     const T  itsSecond,
                                                     UINT_64  iSecondLength)
{
	INT_32 iPos = 0;
	for(;;)
	{
		if (iFirstLength < iSecondLength) { return -1; }
		const INT_32 iRC = Unicode::CompareIgnoreCase(itsFirst, iSecondLength, itsSecond, iSecondLength);
		if (iRC == 0) { return iPos; }

		UINT_32 iUCS1 = 0;
		INT_32 iCharLen1 = Unicode::UTF8ToWide(itsFirst, iFirstLength, iUCS1);
		if (iCharLen1 == -1) { iCharLen1 = 1; }

		iFirstLength -= iCharLen1;

		iPos += iCharLen1;
	}
return -1;
}

//
//  Find substring, ignore case
//
template <typename T> INT_32 Unicode::FindIgnoreCase(const T  itsFirst,
                                                     const T  itsFirstEnd,
                                                     const T  itsSecond,
                                                     const T  itsSecondEnd)
{
        return Unicode::FindIgnoreCase(itsFirst, itsFirstEnd - itsFirst, itsSecondEnd, itsSecond);
}

// End.
