/*
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

#include "DataBuffer.hpp"
#include "LineBuffer.hpp"

#include <string.h>

namespace IRIS
{

//
// Constructor
//
LineBuffer::LineBuffer(const UINT_32 iIBufferLen): iBufferLen(iIBufferLen)
{
	pCurrentBuffer = pFirstBuffer = pLastBuffer = new Buffer;
	iCurrentBufPos = 0;
	pFirstFreeBuffer = NULL;

	pCurrentBuffer -> data      = new CHAR_8[iBufferLen];
	pCurrentBuffer -> data_size = 0;
	pCurrentBuffer -> next      = NULL;
}

//
// Add asciz string
//
void LineBuffer::AddData(CCHAR_P  sData) { AddData(sData, strlen(sData)); }

//
// Add data
//
void LineBuffer::AddData(CCHAR_P        sData,
                         const UINT_32  iDataLen)
{
	if (iDataLen == 0) { return; }

	UINT_32 iBytesLeft = iDataLen;
	for(;;)
	{
		// Get available buffer size
		UINT_32 iCopyBytes = iBufferLen - pLastBuffer -> data_size;
		// Create new buffer
		if (iCopyBytes == 0)
		{
			Buffer * pTMP = pFirstFreeBuffer;
			if (pFirstFreeBuffer != NULL)
			{
				pFirstFreeBuffer = pFirstFreeBuffer -> next;
			}
			else
			{
				pTMP = new Buffer;
				pTMP -> data = new CHAR_8[iBufferLen];
			}

			pTMP -> data_size = 0;
			pTMP -> next      = NULL;

			pLastBuffer -> next = pTMP;
			pLastBuffer = pTMP;

			iCopyBytes = iBufferLen;
		}

		if (iCopyBytes > iBytesLeft) { iCopyBytes = iBytesLeft; }

		// Copy data
		memcpy(pLastBuffer -> data + pLastBuffer -> data_size,
		       sData + iDataLen - iBytesLeft,
		       iCopyBytes);

		pLastBuffer -> data_size += iCopyBytes;

		// Adjust positions
		iBytesLeft -= iCopyBytes;
		if (iBytesLeft == 0) { break; }
	}
}

//
// Get one line
//
bool LineBuffer::GetLine(DataBuffer  & oLine)
{
	oLine.Clear();
	bool bEOLFound = false;
	UINT_32 iStartPos = iCurrentBufPos;
	UINT_32 iPos      = iCurrentBufPos;
	Buffer * pSearchBuffer = pCurrentBuffer;
	for (;;)
	{
		if (iPos != pSearchBuffer -> data_size)
		{
			for (;;)
			{
				if (pSearchBuffer -> data[iPos] == '\n')
				{
					bEOLFound = true;
					break;
				}

				++iPos;
				if (iPos == pSearchBuffer -> data_size) { break; }
			}

			oLine.Append(pSearchBuffer -> data + iStartPos, iPos - iStartPos);
			if (bEOLFound) { break; }
		}
		iStartPos = iPos = 0;
		pSearchBuffer = pSearchBuffer -> next;
		if (pSearchBuffer == NULL) { break; }
	}

	if (!bEOLFound)
	{
		oLine.Clear();
		return false;
	}

	++iPos;
	pCurrentBuffer = pSearchBuffer;
	iCurrentBufPos = iPos;

	// Clear garbage
	Buffer * pBuffer = pFirstBuffer;
	if (pBuffer != pCurrentBuffer)
	{
		if (pFirstFreeBuffer == NULL)
		{
			pFirstFreeBuffer = pBuffer;
			pBuffer = pBuffer -> next;

			pFirstFreeBuffer -> next = NULL;
			pFirstFreeBuffer -> data_size = 0;
		}

		while (pBuffer != pCurrentBuffer)
		{
			Buffer * pTMP = pFirstFreeBuffer;
			pFirstFreeBuffer = pBuffer;
			pBuffer = pBuffer -> next;

			pFirstFreeBuffer -> next = pTMP;
			pFirstFreeBuffer -> data_size = 0;
		}
	}

	pFirstBuffer = pCurrentBuffer;

return true;
}

//
// Clear all data from buffer
//
void LineBuffer::Clear()
{
	// Leave at least one buffer
	if (pFirstBuffer -> next == NULL) { return; }

	Buffer * pBuffer = pFirstBuffer -> next;
	pFirstBuffer -> next = NULL;
	pFirstBuffer -> data_size = 0;

	while (pBuffer != NULL)
	{
		Buffer * pTMP = pFirstFreeBuffer;
		pFirstFreeBuffer = pBuffer;
		pBuffer = pBuffer -> next;

		pFirstFreeBuffer -> next = pTMP;
		pFirstFreeBuffer -> data_size = 0;
	}

	pCurrentBuffer = pLastBuffer = pFirstBuffer;
	iCurrentBufPos = 0;
}

//
// Destructor
//
LineBuffer::~LineBuffer() throw()
{
	Buffer * pBuffer = pFirstBuffer;
	while (pBuffer != NULL)
	{
		Buffer * pTMP = pBuffer -> next;
		delete [] pBuffer -> data;
		delete pBuffer;
		pBuffer = pTMP;
	}

	pBuffer = pFirstFreeBuffer;
	while (pBuffer != NULL)
	{
		Buffer * pTMP = pBuffer -> next;
		delete [] pBuffer -> data;
		delete pBuffer;
		pBuffer = pTMP;
	}
}

} // namespace IRIS
// End.
