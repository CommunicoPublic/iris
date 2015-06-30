/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _STRING_BUFFER_HPP__
#define _STRING_BUFFER_HPP__ 1

/**
  @file StringBuffer.hpp
  @brief String buffer
*/

#include <Types.h>
#include <STLString.hpp>
#include <STLSstream.hpp>

#ifndef C_STRING_BUFFER_LEN
#define C_STRING_BUFFER_LEN 1024
#endif // C_STRING_BUFFER_LEN

namespace IRIS
{
// FWD
class DataBuffer;

/**
  @brief BaseStringBuffer StringBuffer.hpp <StringBuffer.hpp>
  @brief String buffer, base class
*/
class BaseStringBuffer
{
public:
	/**
	  @brief Constructor
	*/
	BaseStringBuffer();

	/**
	  @brief Store one char
	  @param chData - char to store
	*/
	void Store(const UCHAR_8 chData);

	/**
	  @brief Store asciz string
	  @param szData - Data to store
	*/
	void Store(CCHAR_P szData);

	/**
	  @brief Store data buffer
	  @param szData - Data to store
	  @param iDataLen - Data length
	*/
	void Store(CCHAR_P szData, UINT_64 iDataLen);

	/**
	  @brief Dump data buffer
	*/
	void Commit();

	/**
	  @brief A destructor
	*/
	virtual ~BaseStringBuffer() throw();
private:
	// Does not exist
	BaseStringBuffer(const BaseStringBuffer  & oRhs);
	BaseStringBuffer& operator=(const BaseStringBuffer  & oRhs);

	/** Dump data to string */
	void RealDumpBuffer();

protected:
	/** Temp buffer           */
	CHAR_8          sBuffer[C_STRING_BUFFER_LEN];
	/** Buffer position       */
	UINT_32         iBufferPos;

	/** Dump data to string */
	virtual void DumpBuffer() = 0;
};

/**
  @brief BaseStringBuffer StringBuffer.hpp <StringBuffer.hpp>
  @brief String buffer
*/
class StringBuffer:
  public BaseStringBuffer
{
public:
	/**
	  @brief Constructor
	*/
	StringBuffer(STLW::string  & sIResult);

	/**
	  @brief A destructor
	*/
	virtual ~StringBuffer() throw();
private:
	/** Result string         */
	STLW::string  & sResult;

	/** Dump data to string */
	void DumpBuffer();
};

/**
  @brief StringStreamBuffer StringBuffer.hpp <StringBuffer.hpp>
  @brief Stringstream buffer
*/
class StringStreamBuffer:
  public BaseStringBuffer
{
public:
	/**
	  @brief Constructor
	*/
	StringStreamBuffer(STLW::stringstream  & sIResult);

	/**
	  @brief A destructor
	*/
	virtual ~StringStreamBuffer() throw();
private:
	/** Result string         */
	STLW::stringstream  & sResult;

	/** Dump data to string */
	void DumpBuffer();
};

/**
  @brief StringDataBuffer StringBuffer.hpp <StringBuffer.hpp>
  @brief Stringstream buffer
*/
class StringDataBuffer:
  public BaseStringBuffer
{
public:
	/**
	  @brief Constructor
	*/
	StringDataBuffer(DataBuffer  & oIDataBuffer);

	/**
	  @brief A destructor
	*/
	virtual ~StringDataBuffer() throw();
private:
	/** Result data buffer       */
	DataBuffer  & oDataBuffer;

	/** Dump data to Data buffer */
	void DumpBuffer();
};

} // namespace IRIS
#endif // _STRING_BUFFER_HPP__
// End.
