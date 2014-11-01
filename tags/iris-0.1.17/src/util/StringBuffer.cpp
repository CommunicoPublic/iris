/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "DataBuffer.hpp"
#include "StringBuffer.hpp"
namespace IRIS
{

//
// Constructor
//
BaseStringBuffer::BaseStringBuffer(): iBufferPos(0)
{
	;;
}

//
// Store one char
//
void BaseStringBuffer::Store(const UCHAR_8 chData)
{
	sBuffer[iBufferPos++] = chData;
	if (iBufferPos == C_STRING_BUFFER_LEN) { RealDumpBuffer(); }
}

//
// Store ASCIZ string
//
void BaseStringBuffer::Store(CCHAR_P szData)
{
	while(*szData != '\0')
	{
		sBuffer[iBufferPos++] = *szData;
		if (iBufferPos == C_STRING_BUFFER_LEN) { RealDumpBuffer(); }
		++szData;
	}
}

//
// Store data
//
void BaseStringBuffer::Store(CCHAR_P szData, UINT_64 iDataLen)
{
	for(UINT_64 iPos = 0; iPos < iDataLen; ++iPos)
	{
		sBuffer[iBufferPos++] = *szData;
		if (iBufferPos == C_STRING_BUFFER_LEN) { RealDumpBuffer(); }
		++szData;
	}
}

//
// Dump data buffer
//
void BaseStringBuffer::RealDumpBuffer()
{
	if (iBufferPos != 0) { DumpBuffer(); }
	iBufferPos = 0;
}

//
// Dump data buffer
//
void BaseStringBuffer::Commit() { RealDumpBuffer(); }

//
// Destructor
//
BaseStringBuffer::~BaseStringBuffer() throw()
{
	;;
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class StringBuffer
//

//
// Constructor
//
StringBuffer::StringBuffer(STLW::string  & sIResult): sResult(sIResult) { ;; }

//
// Dump data to string
//
void StringBuffer::DumpBuffer() { sResult.append(sBuffer, iBufferPos); }

//
// Destructor
//
StringBuffer::~StringBuffer() throw() { Commit(); }

// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class StringStreamBuffer
//

//
// Constructor
//
StringStreamBuffer::StringStreamBuffer(STLW::stringstream  & sIResult): sResult(sIResult) { ;; }

//
// Dump data to string
//
void StringStreamBuffer::DumpBuffer() { sResult << STLW::string(sBuffer, iBufferPos); }

//
// Destructor
//
StringStreamBuffer::~StringStreamBuffer() throw() { Commit(); }

// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class StringDataBuffer
//

//
// Constructor
//
StringDataBuffer::StringDataBuffer(DataBuffer  & oIDataBuffer): oDataBuffer(oIDataBuffer) { ;; }

//
// Dump data to string
//
void StringDataBuffer::DumpBuffer() { oDataBuffer.Append(sBuffer, iBufferPos); }

//
// Destructor
//
StringDataBuffer::~StringDataBuffer() throw() { Commit(); }

} // namespace IRIS
//
