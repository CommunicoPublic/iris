/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MESSAGE_DIGEST_HPP__
#define _MESSAGE_DIGEST_HPP__ 1

/**
  @file MessageDigest.hpp
  @brief Message-Digest Algorithms
*/

#include "Types.h"
#include "STLString.hpp"

#ifdef AS_MD5_WITHOUT_OPENSSL
    #include <md5.h>
    #define MD5_Init    MD5Init
    #define MD5_Update  MD5Update
    #define MD5_Final   MD5Final
#else
    #include <openssl/md5.h>
#endif

namespace IRIS
{

/**
  @class MD5Hash MessageDigest.hpp <MessageDigest.hpp>
  @brief MD5 hash support
*/
class MD5Hash
{
public:
	MD5Hash();

	MD5Hash & operator<<(const STLW::string & sData);
	MD5Hash & operator<<(CCHAR_P szData);
	MD5Hash & operator<<(const UINT_64 iData);

	MD5Hash & operator<<(MD5Hash & (*f)(MD5Hash & oMD5Hash));

	/**
	  @brief Update digest
	  @param sData - source data, STL string
	  @return Reference to self
	*/
	MD5Hash & Update(const STLW::string  & sData);

	/**
	  @brief Update digest
	  @param sData - source data, pointer
	  @param iLength - source data length
	  @return Reference to self
	*/
	MD5Hash & Update(CCHAR_P sData, const UINT_64 iLength);

	/**
	  @brief Update digest
	  @param sData - source data, pointer
	  @param iLength - source data length
	  @return Reference to self
	*/
	MD5Hash & Update(UCCHAR_P sData, const UINT_64 iLength);

	/**
	  @brief Update digest
	  @param iData - source data, unsigned integer
	  @return Reference to self
	*/
	MD5Hash & Update(const UINT_64 iData);

	/**
	  @brief Get Hex-encoded string
	  @param bUc - if set to true, use uppercase letters
	  @return Hex-encoded string
	*/
	STLW::string AsHex(const bool  bUc = false);

	/**
	  @brief Get BINARY string
	  @return BINARY string
	*/
	STLW::string AsBin();

	/**
	  @brief Destructor
	*/
	~MD5Hash() throw();

	/**
	  @brief Get MD5 digest as Hex-encoded string
	  @param sData - source data, STL string
	  @param bUc - if set to true, use uppercase letters
	  @return Hex-encoded string
	*/
	static STLW::string MD5Hex(const STLW::string  & sData,
	                           const bool            bUc = false);

	/**
	  @brief Get MD5 digest as Hex-encoded string
	  @param sData - source data, pointer
	  @param iLength - source data length
	  @param bUc - if set to true, use uppercase letters
	  @return Hex-encoded string
	*/
	static STLW::string MD5Hex(CCHAR_P        sData,
	                           const UINT_64  iLength,
	                           const bool     bUc = false);

	/**
	  @brief Get MD5 digest as Hex-encoded string
	  @param sData - source data, pointer
	  @param iLength - source data length
	  @param bUc - if set to true, use uppercase letters
	  @return Hex-encoded string
	*/
	static STLW::string MD5Hex(UCCHAR_P       sData,
	                           const UINT_64  iLength,
	                           const bool     bUc = false);


	/**
	  @brief Get MD5 digest as binary string
	  @param sData - source data, STL string
	  @return BINARY string
	*/
	static STLW::string MD5Bin(const STLW::string  & sData);

	/**
	  @brief Get MD5 digest as binary string
	  @param sData - source data, pointer
	  @param iLength - source data length
	  @return BINARY string
	*/
	static STLW::string MD5Bin(CCHAR_P        sData,
	                           const UINT_64  iLength);

	/**
	  @brief Get MD5 digest as binary string
	  @param sData - source data, pointer
	  @param iLength - source data length
	  @return BINARY string
	*/
	static STLW::string MD5Bin(UCCHAR_P       sData,
	                           const UINT_64  iLength);
private:
	// Does not exist
	MD5Hash(const MD5Hash  & oRhs);
	MD5Hash & operator=(const MD5Hash  & oRhs);

	/** MD5Hash Context */
	MD5_CTX oMD5Context;
};

} // namespace IRIS
#endif // _MESSAGE_DIGEST_HPP__
// End.
