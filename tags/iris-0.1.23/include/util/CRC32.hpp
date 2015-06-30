/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _CRC32_HPP__
#define _CRC32_HPP__ 1

/**
  @file CRC32.hpp
  @brief Calculate CRC32 sum
*/

#include "Types.h"
#include "STLString.hpp"

namespace IRIS
{

/**
  @class CRC32 CRC32.hpp <CRC32.hpp>
  @brief Calculate CRC32 sum
*/
class CRC32
{
public:
	/**
	  @brief Constructor
	  @param iSeed - initial seed for crc32 algo
	*/
	CRC32(const UINT_32 iSeed = 0xFFFFFFFFUL);

	/**
	  @brief Input operator <<
	  @param sData - source data, STL string
	  @return Reference to self
	*/
	CRC32 & operator<<(const STLW::string & sData);

	/**
	  @brief Input operator <<
	  @param szData - source data, asciz string
	  @return Reference to self
	*/
	CRC32 & operator<<(CCHAR_P szData);

	/**
	  @brief Input operator <<
	  @param f - reference to function
	  @return Reference to self
	*/
	CRC32 & operator<<(CRC32 & (*f)(CRC32 & oCRC32));

	/**
	  @brief Update digest
	  @param sData - source data, STL string
	  @return Reference to self
	*/
	CRC32 & Update(const STLW::string  & sData);

	/**
	  @brief Update digest
	  @param sData - source data, pointer
	  @param iLength - source data length
	  @return Reference to self
	*/
	CRC32 & Update(CCHAR_P sData, const UINT_64 iLength);

	/**
	  @brief Update digest
	  @param sData - source data, pointer
	  @param iLength - source data length
	  @return Reference to self
	*/
	CRC32 & Update(UCCHAR_P sData, const UINT_64 iLength);

	/**
	  @brief Get CRC32 value
	  @return CRC32 value
	*/
	UINT_32 AsInt();

	/**
	  @brief Cast to integer
	*/
	operator UINT_32() const;

	/**
	  @brief Destructor
	*/
	~CRC32() throw();

	/**
	  @brief Get CRC32 value
	  @param sData - source data, STL string
	  @param iSeed - initial seed for crc32 algo
	  @return CRC32 value
	*/
	static UINT_32 GetCRC32(const STLW::string  & sData,
	                        const UINT_32         iSeed = 0xFFFFFFFFUL);

	/**
	  @brief Get CRC32 value
	  @param sData - source data, pointer
	  @param iLength - source data length
	  @param iSeed - initial seed for crc32 algo
	  @return CRC32 value
	*/
	static UINT_32 GetCRC32(CCHAR_P        sData,
	                        const UINT_64  iLength,
	                        const UINT_32  iSeed = 0xFFFFFFFFUL);

	/**
	  @brief Get CRC32 value
	  @param sData - source data, pointer
	  @param iLength - source data length
	  @param iSeed - initial seed for crc32 algo
	  @return CRC32 value
	*/
	static UINT_32 GetCRC32(UCCHAR_P       sData,
	                        const UINT_64  iLength,
	                        const UINT_32  iSeed = 0xFFFFFFFFUL);
private:
	// Does not exist
	CRC32(const CRC32  & oRhs);
	CRC32 & operator=(const CRC32  & oRhs);

	/** Initial seed */
	const UINT_32  iSeed;
	/** Calculated value */
	UINT_32        iCRC32;

	/** CRC32 table */
	static const UINT_32 aCRC32Tab[];
};

} // namespace IRIS
#endif // _CRC32_HPP__
// End.
