/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _UNICODE_UTILS_HPP__
#define _UNICODE_UTILS_HPP__ 1

/**
  @file UnicodeUtils.hpp
  @brief Unicode utilities
*/

// Local includes
#include "CharTables.hpp"

#include <stdio.h>

namespace IRIS
{

/**
  @class Unicode UnicodeUtils.hpp <UnicodeUtils.hpp>
  @brief Unicode support
*/
class Unicode
{
public:

	/**
	  @brief Convert wide character to UTF-8
	  @param iUCS - Wide character
	  @param itsUTF8 - iterator or pointer to first octet of UTF-8 buffer. Buffer MUST contain at least 6 octets
	  @param iUCS - Wide character [out]
	  @return Length of character in octets, -1 - otherwise
	*/
	template <typename T> static INT_32 WideToUTF8(UINT_32    iUCS,
	                                               T        & itsUTF8);

	/**
	  @brief Convert UTF-8 character to wide
	  @param itsIUTF8 - iterator or pointer to first octet of UTF-8 character
	  @param iBufferLength - size of buffer
	  @param iUCS - Wide character [out]
	  @return Length of character in octets, -1 - otherwise
	*/
	template <typename T> static INT_32 UTF8ToWide(T             & itsIUTF8,
	                                               const UINT_64   iBufferLength,
	                                               UINT_32       & iUCS);

	/**
	  @brief Convert UTF-8 character to wide
	  @param itsIUTF8 - iterator or pointer to first octet of UTF-8 character
	  @param itsIUTF8End - iterator or pointer to last octet in buffer
	  @param iUCS - Wide character [out]
	  @return Length of character in octets, -1 - otherwise
	*/
	template <typename T> static INT_32 UTF8ToWide(T             & itsIUTF8,
	                                               const T       & itsIUTF8End,
	                                               UINT_32       & iUCS);

	/**
	  @brief Get length of UTF-8 character
	  @param itsIUTF8 - iterator or pointer to first octet of UTF-8 character
	  @param iBufferLength - size of buffer
	  @return Length of string in UTF-8 characters, -1 - otherwise
	*/
	template <typename T> static INT_32 UTF8Length(T              itsIUTF8,
	                                               const UINT_64  iBufferLength);

	/**
	  @brief Get length of UTF-8 character
	  @param itsIUTF8 - iterator or pointer to first octet of UTF-8 character
	  @param itsIUTF8End - iterator or pointer to last octet in buffer
	  @return Length of string in UTF-8 characters, -1 - otherwise
	*/
	template <typename T> static INT_32 UTF8Length(T          itsIUTF8,
	                                               const T  & itsIUTF8End);

	/**
	  @brief Compare two strings
	*/
	template <typename T> static INT_32 Compare(T        itsFirst,
	                                            UINT_64  iFirstLength,
	                                            T        itsSecond,
	                                            UINT_64  iSecondLength);

	/**
	  @brief Compare two strings
	*/
	template <typename T> static INT_32 Compare(const T  itsFirst,
	                                            const T  itsFirstEnd,
	                                            const T  itsSecond,
	                                            const T  itsSecondEnd);

	/**
	  @brief Compare two strings, ignore case
	*/
	template <typename T> static INT_32 CompareIgnoreCase(const T  itsFirst,
	                                                      UINT_64  iFirstLength,
	                                                      const T  itsSecond,
	                                                      UINT_64  iSecondLength);

	/**
	  @brief Compare two strings, ignore case
	*/
	template <typename T> static INT_32 CompareIgnoreCase(const T  itsFirst,
	                                                      const T  itsFirstEnd,
	                                                      const T  itsSecond,
	                                                      const T  itsSecondEnd);

	/**
	  @brief Find substring, ignore case
	*/
	template <typename T> static INT_32 Find(T        itsFirst,
	                                         UINT_64  iFirstLength,
	                                         const T  itsSecond,
	                                         UINT_64  iSecondLength);

	/**
	  @brief Find substring, ignore case
	*/
	template <typename T> static INT_32 Find(const T  itsFirst,
	                                         const T  itsFirstEnd,
	                                         const T  itsSecond,
	                                         const T  itsSecondEnd);

	/**
	  @brief Find substring, ignore case
	*/
	template <typename T> static INT_32 FindIgnoreCase(T        itsFirst,
	                                                   UINT_64  iFirstLength,
	                                                   const T  itsSecond,
	                                                   UINT_64  iSecondLength);

	/**
	  @brief Find substring, ignore case
	*/
	template <typename T> static INT_32 FindIgnoreCase(const T  itsFirst,
	                                                   const T  itsFirstEnd,
	                                                   const T  itsSecond,
	                                                   const T  itsSecondEnd);

	/**
	  @brief  Make a UTF8 string lowercase
	*/
	static INT_32 Lowercase(CCHAR_P    szSrc,
	                        UINT_64    iSrcLen,
	                        CHAR_P   & szDst,
	                        UINT_64  & iDstLen);

	/**
	  @brief  Make a UTF8 string uppercase
	*/
	static INT_32 Uppercase(CCHAR_P    szSrc,
	                        UINT_64    iSrcLen,
	                        CHAR_P   & szDst,
	                        UINT_64  & iDstLen);
private:

};

#include "UnicodeUtils.tcc"

} // namespace IRIS
#endif // _SYSTEM_VARS_HPP__
// End.
