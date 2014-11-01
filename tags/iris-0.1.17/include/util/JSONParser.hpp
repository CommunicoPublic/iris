/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _JSON_PARSER_HPP__
#define _JSON_PARSER_HPP__ 1

#include "StringBuffer.hpp"
#include "STLVector.hpp"
#include "UnicodeUtils.hpp"

#include <stdio.h>
#include <stdlib.h>

namespace IRIS
{

/**
  @brief JSONParser JSONParser.hpp <JSONParser.hpp>
  @brief JSON parser
*/
template<typename T>class JSONParser
{
public:
	/**
	  @brief Constructor
	  @param oIResult - result collector
	*/
	JSONParser(T  & oIResult);

	/**
	  @brief Parse data
	  @param szString - Data to parse
	  @param iStringLen - Data length
	  @return 0 - if success, -1 - otherwise
	*/
	INT_32 Parse(CCHAR_P        szString,
	             const UINT_64  iStringLen);

	/**
	  @brief Init parser
	*/
	void Init();

	/**
	  @brief Parse chunk of data
	  @param szString - Data to store
	  @param iStringLen - Data length
	  @param bIsLastChunk - if true, this is last chunk of data stream
	  @return 0 - if success, -1 - otherwise
	*/
	INT_32 ParseChunk(CCHAR_P        szString,
	                  const UINT_64  iStringLen,
	                  const bool     bIsLastChunk);

	/**
	  @brief A destructor
	*/
	~JSONParser() throw();
private:
	// Does not exist
	JSONParser(const JSONParser  & oRhs);
	JSONParser& operator=(const JSONParser  & oRhs);

	/** Result data object                  */
	T                          & oResult;
	/** Current stack value                 */
	T                          * pCurrVal;
	/** Internal stack                      */
	STLW::vector<T *>            vStack;
	/** Hex-encoded digit                   */
	UINT_32                      iXDigit;
	/** Found fractional value              */
	bool                         bFoundFrac;
	/** String data                         */
	STLW::string                 sTMP;
	/** String Buffer                       */
	StringBuffer                 oStringBuffer;

	int cs, top;
	/** Internal ragel stack                */
	STLW::vector<int>  stack;
};

#include "JSONParser.tcc"

} // namespace IRIS
#endif // _JSON_PARSER_HPP__
// End.
