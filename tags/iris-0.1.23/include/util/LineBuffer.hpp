/*
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _LINE_BUFFER_HPP__
#define _LINE_BUFFER_HPP__ 1

#include "Types.h"

namespace IRIS
{
// FWD
class DataBuffer;

/**
  @class LineBuffer LineBuffer.hpp <LineBuffer.hpp>
  @brief  Growable data buffer
*/
class LineBuffer
{
public:
	/**
	  @brief Constructor
	  @param iIBufferLen - Internal buffer size
	*/
	LineBuffer(const UINT_32 iIBufferLen);

	/**
	  @brief Add asciz string
	  @param sData - data to add
	*/
	void AddData(CCHAR_P  sData);

	/**
	  @brief Add data
	  @param sData - data to add
	  @param iDataLen - data length
	*/
	void AddData(CCHAR_P        sData,
	             const UINT_32  iDataLen);

	/**
	  @brief Get one line from buffer
	  @param sLine - one line [out]
	  @param bWithLineFeed - get line with line feed
	  @return false, if no available data in buffer
	*/
	bool GetLine(DataBuffer  & oLine,
	             const bool    bWithLineFeed = false);

	/**
	  @brief Clear all data from buffer
	*/
	void Clear();

	/**
	  @brief Destructor
	*/
	~LineBuffer() throw();
private:
	// Does not exist
	LineBuffer(const LineBuffer  & oRhs);
	LineBuffer& operator=(const LineBuffer  & oRhs);

	struct Buffer
	{
		/** Data storage               */
		CHAR_P     data;
		/** Data size                  */
		UINT_32    data_size;
		/** Pointer to the next buffer */
		Buffer   * next;
	};

	/** Internal buffer size    */
	const UINT_32 iBufferLen;
	/** Pointer to first buffer */
	Buffer       * pFirstBuffer;
	/** Pointer to last buffer  */
	Buffer       * pLastBuffer;

	/** Current buffer for reading */
	Buffer       * pCurrentBuffer;
	/** Position in buffer         */
	UINT_32        iCurrentBufPos;

	/** First free buffer          */
	Buffer       * pFirstFreeBuffer;
};

} // namespace IRIS
#endif // _LINE_BUFFER_HPP__
// End.
