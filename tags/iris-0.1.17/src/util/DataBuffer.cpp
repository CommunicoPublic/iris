/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "DataBuffer.hpp"

#include <stdlib.h>
#include <string.h>

namespace IRIS
{

//
// Initial buffer size
//
const UINT_64 DataBuffer::BUFSIZE = 131072;
const UINT_64 DataBuffer::npos = (UINT_64)(-1);

//
// Constructor
//
DataBuffer::DataBuffer(const UINT_64 iInitialSize): sData(NULL), iUsedSize(0), iSize(iInitialSize)
{
	if (iSize != 0) { sData = (CHAR_P)malloc(iSize); }
}

//
// Reserve specified number of bytes
//
DataBuffer & DataBuffer::Reserve(const UINT_64  iBytes)
{
	if (iBytes > iSize)
	{
		const UINT_64 iNewSize = iBytes;
		CHAR_P sTMP = (CHAR_P)realloc(sData, iNewSize);
		if (sTMP == NULL)
		{
			// Handle error
			return *this;
		}
		iSize = iNewSize;
		sData = sTMP;
	}
return *this;
}

//
// Force size of buffer to specified value
//
UINT_64 DataBuffer::SetSize(const UINT_64  iNewSize)
{
	if (iNewSize > iSize) { return DataBuffer::npos; }
	iUsedSize = iNewSize;

return iNewSize;
}

//
// Append a character to the buffer
//
DataBuffer & DataBuffer::Assign(CHAR_8  chData)
{
	Clear();

return Append(chData);
}

//
// Append data buffer
//
DataBuffer  & DataBuffer::Assign(const void     * vBuffer,
                                 const UINT_64    iBufferSize)
{
	Clear();

return Append(vBuffer, iBufferSize);
}

//
// Append ASCIZ string
//
DataBuffer  & DataBuffer::Assign(CCHAR_P  szBuffer)
{
	Clear();

return Append(szBuffer);
}

//
// Append a character to the buffer
//
DataBuffer  & DataBuffer::Append(CHAR_8  chData)
{
	if (iUsedSize == iSize)
	{
		const UINT_64 iNewSize = iSize * 2;
		CHAR_P sTMP = (CHAR_P)realloc(sData, iNewSize);
		if (sTMP == NULL)
		{
			// Handle error
			return *this;
		}
		iSize = iNewSize;
		sData = sTMP;
	}
	sData[iUsedSize++] = chData;
return *this;
}

//
// Append data buffer
//
DataBuffer  & DataBuffer::Append(const void     * vBuffer,
                                 const UINT_64    iBufferSize)
{
	const UINT_64 iNewSize = iUsedSize + iBufferSize;
	if (iNewSize > iSize)
	{
		CHAR_P sTMP = (CHAR_P)realloc(sData, iNewSize);
		if (sTMP == NULL)
		{
			// Handle error
			return *this;
		}
		iSize = iNewSize;
		sData = sTMP;
	}
	memcpy(sData + iUsedSize, vBuffer, iBufferSize);
	iUsedSize = iNewSize;
return *this;
}

//
// Append ASCIZ string
//
DataBuffer  & DataBuffer::Append(CCHAR_P  szBuffer)
{
	return Append((void*)szBuffer, strlen(szBuffer));
}

//
// Get data buffer
//
CCHAR_P DataBuffer::Data(const UINT_64  iStartPos) const { return sData + iStartPos; }

//
// Find character
//
UINT_64 DataBuffer::Find(const UCHAR_8  chChar,
                         const UINT_64  iStartPos)
{
	if (iStartPos >= iUsedSize) { return DataBuffer::npos; }

	for (UINT_64 iPos = iStartPos; iPos < iUsedSize; ++iPos)
	{
		if (sData[iPos] == chChar) { return iPos; }
	}

return DataBuffer::npos;
}

//
// Get data size
//
UINT_64 DataBuffer::Size() const { return iUsedSize; }

//
// Get reference to specified element
//
CHAR_8 & DataBuffer::operator[](const UINT_64 iPos) { return sData[iPos]; }

//
// Get reference to specified element
//
const CHAR_8 & DataBuffer::operator[](const UINT_64 iPos) const { return sData[iPos]; }

//
// Clear buffer
//
void DataBuffer::Clear() { iUsedSize = 0; }

//
// Cut buffer
//
void DataBuffer::Cut(const UINT_64 iCutBytes)
{
	if (iCutBytes <= iUsedSize) { iUsedSize -= iCutBytes; }
}

//
// Constructor
//
DataBuffer::~DataBuffer() throw() { free(sData); }

} // namespace IRIS
// End.
