/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "MessageDigest.hpp"
#include "STLSstream.hpp"

#include <string.h>

namespace IRIS
{

//
// Constructor
//
MD5Hash::MD5Hash()
{
	MD5_Init(&oMD5Context);
}

// Store string
//
MD5Hash & MD5Hash::operator<<(const STLW::string & sData)
{
	Update(sData.data(), sData.size());

return *this;
}

//
// Store ASCIZ string
//
MD5Hash & MD5Hash::operator<<(CCHAR_P szData)
{
	Update(szData, strlen(szData));

return *this;
}

//
// Store unsigned integer
//
MD5Hash & MD5Hash::operator<<(const UINT_64 iData)
{
	Update(iData);

return *this;
}

//
// Adapter for stream-like manipulators
//
MD5Hash & MD5Hash::operator<<(MD5Hash & (*f)(MD5Hash & oMD5Hash)) { return f(*this); }

//
// Update digest
//
MD5Hash & MD5Hash::Update(const STLW::string  & sData) { return Update(sData.data(), sData.size()); }

//
// Update digest
//
MD5Hash & MD5Hash::Update(CCHAR_P sData, const UINT_64 iLength)
{
	MD5_Update(&oMD5Context, (const unsigned char *)sData, iLength);

return *this;
}

//
// Update digest
//
MD5Hash & MD5Hash::Update(const UINT_64  iData)
{
	STLW::stringstream sStream;

	sStream << iData;

	const STLW::string sData(sStream.str());

return Update(sData.data(), sData.size());
}


//
// Update digest
//
MD5Hash & MD5Hash::Update(UCCHAR_P sData, const UINT_64 iLength)
{
	MD5_Update(&oMD5Context, sData, iLength);

return *this;
}

//
// Get Hex-encoded string
//
STLW::string MD5Hash::AsBin()
{
	unsigned char sDigest[16];
	MD5_Final(sDigest, &oMD5Context);

	MD5_Init(&oMD5Context);

return STLW::string((char *)sDigest, 16);
}

//
// Get MD5 digest as Hex-encoded string
//
STLW::string MD5Hash::AsHex(const bool  bUc)
{
	static CHAR_8  aHexL[] = "0123456789abcdef";
	static CHAR_8  aHexU[] = "0123456789ABCDEF";

	CHAR_8         szMD5[32];
	unsigned char  sDigest[16];

	MD5_Final(sDigest, &oMD5Context);

	if (bUc)
	{
		UINT_32 iJ = 0;
		for (UINT_32 iI = 0; iI < 16; ++iI)
		{
			szMD5[ iJ++] = aHexU[ (sDigest[iI] >> 4 )& 0x0F ];
			szMD5[ iJ++] = aHexU[  sDigest[iI]       & 0x0F ];
		}
	}
	else
	{
		UINT_32 iJ = 0;
		for (UINT_32 iI = 0; iI < 16; ++iI)
		{
			szMD5[ iJ++] = aHexL[ (sDigest[iI] >> 4 )& 0x0F ];
			szMD5[ iJ++] = aHexL[  sDigest[iI]       & 0x0F ];
		}
	}

	MD5_Init(&oMD5Context);

return STLW::string(szMD5, 32);
}

//
// Destructor
//
MD5Hash::~MD5Hash() throw() { ;; }

//
// Get MD5 digest as Hex-encoded string
//
STLW::string MD5Hash::MD5Hex(const STLW::string  & sData, const bool  bUc) { return MD5Hex(UCCHAR_P(sData.data()), sData.size(), bUc); }

//
// Get MD5 digest as Hex-encoded string
//
STLW::string MD5Hash::MD5Hex(CCHAR_P sData, const UINT_64 iLength, const bool  bUc)  { return MD5Hex(UCCHAR_P(sData), iLength, bUc); }

//
// Get MD5 digest as Hex-encoded string
//
STLW::string MD5Hash::MD5Hex(UCCHAR_P sData, const UINT_64 iLength, const bool  bUc)
{

	static CHAR_8  aHexL[] = "0123456789abcdef";
	static CHAR_8  aHexU[] = "0123456789ABCDEF";

	CHAR_8         szMD5[32];
	unsigned char  sDigest[16];

	MD5_CTX oMD5Context;
	MD5_Init(&oMD5Context);
	MD5_Update(&oMD5Context, sData, iLength);
	MD5_Final(sDigest, &oMD5Context);

	if (bUc)
	{
		UINT_32 iJ = 0;
		for (UINT_32 iI = 0; iI < 16; ++iI)
		{
			szMD5[ iJ++] = aHexU[ (sDigest[iI] >> 4 )& 0x0F ];
			szMD5[ iJ++] = aHexU[  sDigest[iI]       & 0x0F ];
		}
	}
	else
	{
		UINT_32 iJ = 0;
		for (UINT_32 iI = 0; iI < 16; ++iI)
		{
			szMD5[ iJ++] = aHexL[ (sDigest[iI] >> 4 )& 0x0F ];
			szMD5[ iJ++] = aHexL[  sDigest[iI]       & 0x0F ];
		}
	}

return STLW::string(szMD5, 32);
}

//
// Get MD5 digest as Hex-encoded string
//
STLW::string MD5Hash::MD5Bin(const STLW::string  & sData) { return MD5Bin(UCCHAR_P(sData.data()), sData.size()); }

//
// Get MD5 digest as Hex-encoded string
//
STLW::string MD5Hash::MD5Bin(CCHAR_P sData, const UINT_64 iLength)  { return MD5Bin(UCCHAR_P(sData), iLength); }

//
// Get MD5 digest as Hex-encoded string
//
STLW::string MD5Hash::MD5Bin(UCCHAR_P sData, const UINT_64 iLength)
{
	unsigned char  sDigest[16];

	MD5_CTX oMD5Context;
	MD5_Init(&oMD5Context);
	MD5_Update(&oMD5Context, sData, iLength);
	MD5_Final(sDigest, &oMD5Context);

return STLW::string(CCHAR_P(sDigest), 16);
}

} // namespace IRIS
// End.
