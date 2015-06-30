/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "BlockAllocator.hpp"

#include <new>
#include <stdlib.h>
#include <string.h>

namespace IRIS
{

//
// Get header size
//
UINT_64 BlockAllocator::HeaderSize() { return 16; }

//
// Get block size
//
void BlockAllocator::GetPrevBlockSize(const UINT_64    iOffset,
                                      UINT_64        & iPrevSize)
{
	memcpy(&iPrevSize, (UCHAR_P)vData + iOffset * iBlockSize, 8);
}

//
// Store block size
//
void BlockAllocator::StorePrevBlockSize(const UINT_64    iOffset,
                                        const UINT_64    iPrevSize)
{
	memcpy((UCHAR_P)vData + iOffset * iBlockSize, &iPrevSize, 8);
}

//
// Get block size
//
void BlockAllocator::GetBlockSize(const UINT_64    iOffset,
                                  UINT_64        & iSize)
{
	memcpy(&iSize, (UCHAR_P)vData + iOffset * iBlockSize + 8, 8);
}

//
// Store block size
//
void BlockAllocator::StoreBlockSize(const UINT_64    iOffset,
                                    const UINT_64    iSize)
{
	memcpy((UCHAR_P)vData + iOffset * iBlockSize + 8, &iSize, 8);
}

//
// Get memory block offset
//
void * BlockAllocator::GetBlockOffset(const UINT_64  iStartBlock)
{
	return (UCHAR_P)(UCHAR_P)vData + iStartBlock * iBlockSize + BlockAllocator::HeaderSize();
}

//
// Get memory block offset
//
UINT_64 BlockAllocator::GetBlockOffset(void * vBlock)
{
return ((UCHAR_P)vBlock - BlockAllocator::HeaderSize() - (UCHAR_P)vData) / iBlockSize;
}

//
// Constructor
//
BlockAllocator::BlockAllocator(const UINT_64  iIBlockSize,
                               const UINT_64  iIMaxBlocks): iBlockSize(iIBlockSize),
                                                            iMaxBlocks(iIMaxBlocks),
                                                            oFreeIndexAllocator(TSizeIndex::NodeSize(), iMaxBlocks),
                                                            oFreeIndex(&oCMP, &oFreeIndexAllocator)
{
	// Allocate memory on init
	vData = malloc(iBlockSize * iMaxBlocks);
	if (vData == NULL) { throw std::bad_alloc(); }
	memset(vData, 0, iBlockSize * iMaxBlocks);

	// Store previous blocks size. Previous block is empty
	StorePrevBlockSize(0, 0);
	StoreBlockSize(0, iMaxBlocks);

	SizeIndexRecord oFreeIndexRecord = { iMaxBlocks, 0 };
	oFreeIndex.Insert(oFreeIndexRecord);
}

//
// Allocate memory from pool
//
void * BlockAllocator::Allocate(const UINT_64 iSize)
{
	// No free space?
	if (oFreeIndex.Empty()) { return NULL; }

	// Get number of blocks
	const UINT_64 iBlocks = GetBlocks(iSize);

	// Search for first suitable block
	SizeIndexRecord oSearchIndexRecord = { iBlocks,  0 };
	SizeIndexRecord * pFoundRecord = oFreeIndex.Supremum(oSearchIndexRecord);

	// No available memory
	if (pFoundRecord == NULL) { return NULL; }

	// Index record
	SizeIndexRecord oIndexRecord = *pFoundRecord;
	// Remove from free index
	oFreeIndex.Remove(oIndexRecord);

	// Split block
	if (iBlocks < oIndexRecord.blocks)
	{
		// New block offset
		const UINT_64 iNewBlockOffset = oIndexRecord.offset + iBlocks;
		const UINT_64 iNewBlocks      = oIndexRecord.blocks - iBlocks;

		// Save new and previous block size
		StorePrevBlockSize(iNewBlockOffset, iBlocks);
		StoreBlockSize(iNewBlockOffset, iNewBlocks);

		// Save block info
		SizeIndexRecord oFreeIndexRecord = { iNewBlocks, iNewBlockOffset };
		oFreeIndex.Insert(oFreeIndexRecord);

		// Modify next block size for previous block
		const UINT_64 iNextBlockOffset = oIndexRecord.blocks + oIndexRecord.offset;
		if (iNextBlockOffset < iMaxBlocks)
		{
			// Save previous block size
			StorePrevBlockSize(iNextBlockOffset, iNewBlocks);
		}
	}
	else { ;; } // Reuse block, nothing to do

	// Save previous block size
	StoreBlockSize(oIndexRecord.offset, iBlocks);

return GetBlockOffset(oIndexRecord.offset);
}

//
// Free memory
//
void BlockAllocator::Deallocate(void * vData)
{
	// Nothing to do
	if (vData == NULL) { return; }

	UINT_64 iCurrBlockOffset = GetBlockOffset(vData);

	UINT_64 iCurrBlockSize     = 0;
	GetBlockSize(iCurrBlockOffset, iCurrBlockSize);

	UINT_64 iPrevBlockSize = 0;
	GetPrevBlockSize(iCurrBlockOffset, iPrevBlockSize);

	UINT_64 iNewFreeBlockOffset = iCurrBlockOffset;
	UINT_64 iNewFreeBlockSize   = iCurrBlockSize;

	// Check left neighbour
	if (iPrevBlockSize != 0)
	{
		// Can erase?
		const SizeIndexRecord oFreeIndexRecord = { iPrevBlockSize, iCurrBlockOffset - iPrevBlockSize };
		bool bRemoved = false;
		oFreeIndex.Remove(oFreeIndexRecord, bRemoved);
		if (bRemoved)
		{
			iNewFreeBlockOffset = oFreeIndexRecord.offset;
			iNewFreeBlockSize   += iPrevBlockSize;
		}
	}

	// Check left neighbour
	if (iCurrBlockOffset + iCurrBlockSize < iMaxBlocks)
	{
		UINT_64 iNextBlocks = 0;
		// Get size of next block
		GetBlockSize(iCurrBlockOffset + iCurrBlockSize, iNextBlocks);
		// Can erase?
		const SizeIndexRecord oFreeIndexRecord = { iNextBlocks, iCurrBlockOffset + iCurrBlockSize };
		bool bRemoved = false;
		oFreeIndex.Remove(oFreeIndexRecord, bRemoved);
		if (bRemoved)
		{
			iNewFreeBlockSize += iNextBlocks;
		}
	}

	// Add to free index
	const SizeIndexRecord oFreeIndexRecord = { iNewFreeBlockSize, iNewFreeBlockOffset };
	oFreeIndex.Insert(oFreeIndexRecord);

	StoreBlockSize(iNewFreeBlockOffset, iNewFreeBlockSize);
	if (iNewFreeBlockSize + iNewFreeBlockOffset < iMaxBlocks)
	{
		StorePrevBlockSize(iNewFreeBlockSize + iNewFreeBlockOffset, iNewFreeBlockSize);
	}
}

//
// Get number of blocks
//
UINT_64 BlockAllocator::GetBlocks(const UINT_64  iSize)
{
	const UINT_64 iRealSize = iSize + BlockAllocator::HeaderSize();

return (iRealSize / iBlockSize) + ((iRealSize % iBlockSize) ? 1 : 0);
}

//
// Destructor
//
BlockAllocator::~BlockAllocator() throw()
{
	free(vData);
}

} // namespace IRIS
// End.
