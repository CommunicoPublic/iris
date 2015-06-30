/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MEMORY_POOL_HPP__
#define _MEMORY_POOL_HPP__ 1

#include <Types.h>
#include <STLMap.hpp>
#include <STLSet.hpp>

#include <stdio.h>
#include <sysexits.h>

class MemoryPool
{
public:
	MemoryPool(const UINT_64 iIBlockSize,
	              const UINT_64 iIMaxBlocks);

	void * Allocate(const UINT_64 iSize);

	void Free(void * vData);

	~MemoryPool() throw();

private:
	// Does not exist
	MemoryPool(const MemoryPool  & oRhs);
	MemoryPool& operator=(const MemoryPool  & oRhs);

	struct SizeIndexRecord
	{
		/** Block size     */
		UINT_64    blocks;
		/** Block data     */
		void     * offset;
	};

	struct SizeIndexRecordCMP
	{
		bool operator()(const SizeIndexRecord  & oX,
		                const SizeIndexRecord  & oY) const
		{
			if (oX.blocks == oY.blocks) { return oX.offset < oY.offset; }

		return oX.blocks < oY.blocks;
		}
	};

	typedef STLW::set<SizeIndexRecord, SizeIndexRecordCMP>  TSizeIndex;

	struct VoidCMP
	{
		bool operator()(const void  * vX,
		                const void  * vY) const { return (UCHAR_P)vX < (UCHAR_P)vY; }
	};

	typedef STLW::map<void *, UINT_64, VoidCMP>  TMemoryMap;

	/** Block size                */
	const UINT_64    iBlockSize;
	/** Max. number of blocks     */
	const UINT_64    iMaxBlocks;
	/** Memory block              */
	void           * vData;
	/** Size index                */
	TSizeIndex       oUsedIndex;
	/** Size index                */
	TSizeIndex       oFreeIndex;
	/** Used memory map           */
	TMemoryMap       mUsedMemoryMap;
	/** Free memory map           */
	TMemoryMap       mFreeMemoryMap;

	/**
	  @brief Get number of blocks
	*/
	UINT_64 GetBlocks(const UINT_64  iSize);
};

//
// Constructor
//
MemoryPool::MemoryPool(const UINT_64  iIBlockSize,
                             const UINT_64  iIMaxBlocks): iBlockSize(iIBlockSize),
                                                          iMaxBlocks(iIMaxBlocks)
{
	// Allocate memory on init
	vData = ::operator new(iBlockSize * iMaxBlocks);

	SizeIndexRecord oFreeIndexRecord = { iMaxBlocks, vData };

	oFreeIndex.insert(oFreeIndexRecord);
	mFreeMemoryMap[vData] = iMaxBlocks;

fprintf(stderr, "Store block in free block index { %p, %llu }\n", (void*)oFreeIndexRecord.offset, (long long unsigned)oFreeIndexRecord.blocks);
}

//
// Allocate memory from pool
//
void * MemoryPool::Allocate(const UINT_64 iSize)
{
	// No free space?
	if (oFreeIndex.empty()) { return NULL; }

	// Get number of blocks
	const UINT_64 iBlocks = GetBlocks(iSize);

	// Search for first suitable block
	SizeIndexRecord oSearchIndexRecord = { iBlocks,  NULL };
	TSizeIndex::iterator itoFreeIndex = oFreeIndex.upper_bound(oSearchIndexRecord);

	// Check block with equal size
	if (itoFreeIndex != oFreeIndex.begin())
	{
		TSizeIndex::iterator itoTMP = itoFreeIndex;
		--itoTMP;
		if (itoTMP -> blocks == iBlocks) { itoFreeIndex = itoTMP; }
	}

	// No available memory
	if (itoFreeIndex == oFreeIndex.end()) { return NULL; }

	// Index record
	SizeIndexRecord oIndexRecord = *itoFreeIndex;
	// Remove from free index
	oFreeIndex.erase(itoFreeIndex);

	// Remove record from free memory map
	mFreeMemoryMap.erase(oIndexRecord.offset);

	// Split block
	if (oIndexRecord.blocks > iBlocks)
	{
fprintf(stderr, "Split block\n");
		// Save block info
		SizeIndexRecord oFreeIndexRecord = { oIndexRecord.blocks - iBlocks, (UCHAR_P)oIndexRecord.offset + iBlocks * iBlockSize };

		oFreeIndex.insert(oFreeIndexRecord);
		mFreeMemoryMap[oFreeIndexRecord.offset] = oFreeIndexRecord.blocks;
fprintf(stderr, "Store block in free block index { %p, %llu }\n", (void*)oFreeIndexRecord.offset, (long long unsigned)oFreeIndexRecord.blocks);
	}
	else
	{
fprintf(stderr, "Reuse block\n");
	}

	oIndexRecord.blocks = iBlocks;
	// Store in used index
	oUsedIndex.insert(oIndexRecord);
	mUsedMemoryMap[oIndexRecord.offset] = oIndexRecord.blocks;

fprintf(stderr, "Store block in used block index { %p, %llu }\n", (void*)oIndexRecord.offset, (long long unsigned)oIndexRecord.blocks);

return oIndexRecord.offset;
}

//
// Free memory
//
void MemoryPool::Free(void * vData)
{
fprintf(stderr, "Free: %p\n", vData);
	// Check block is allocated
	TMemoryMap::iterator itmUsedMemoryMap = mUsedMemoryMap.find(vData);
	if (itmUsedMemoryMap == mUsedMemoryMap.end()) { return; }

	// Remove from used index
	const SizeIndexRecord oUsedIndexRecord = { itmUsedMemoryMap -> second, itmUsedMemoryMap -> first };
	oUsedIndex.erase(oUsedIndexRecord);

	// Store in free memory map
	STLW::pair<TMemoryMap::iterator, bool> oPair = mFreeMemoryMap.insert(STLW::pair<void *, UINT_64>(oUsedIndexRecord.offset, oUsedIndexRecord.blocks));

	// Impossible happened?
	if (!oPair.second) { return; }

	// Check blocks are joinable
	TMemoryMap::iterator itmFreeMemoryMap = oPair.first;
	void     * vUsedData       = itmFreeMemoryMap -> first;
	UINT_64    iUsedBlockCount = itmFreeMemoryMap -> second;

	TMemoryMap::iterator itmJoinRight = itmFreeMemoryMap;
	TMemoryMap::iterator itmJoinLeft  = itmFreeMemoryMap;

	// Check right neighbour
	bool bJoinBlocks = false;
	TMemoryMap::iterator itmRightNeighbour = itmFreeMemoryMap;
	++itmRightNeighbour;
	if (itmRightNeighbour != mFreeMemoryMap.end())
	{
		const void * vRightOffset = (UCHAR_P)vUsedData + iUsedBlockCount * iBlockSize;
		if (itmRightNeighbour -> first == vRightOffset)
		{
fprintf(stderr, "Join with right block\n");
			// Increase block size
			iUsedBlockCount += itmRightNeighbour -> second;

			// Remove from free index
			const SizeIndexRecord oFreeIndexRecord = { itmRightNeighbour -> second, itmRightNeighbour -> first };
			oFreeIndex.erase(oFreeIndexRecord);

			itmJoinRight = itmRightNeighbour;
			bJoinBlocks = true;
		}
	}

	// Check left neighbour
	if (itmFreeMemoryMap != mFreeMemoryMap.begin())
	{
		TMemoryMap::iterator itmLeftNeighbour = itmFreeMemoryMap;
		--itmLeftNeighbour;

		const void * vBlockOffset = (UCHAR_P)itmLeftNeighbour -> first + itmLeftNeighbour -> second * iBlockSize;
		if (vUsedData == vBlockOffset)
		{
fprintf(stderr, "Join with left block\n");
			// Increase block size
			vUsedData        = itmLeftNeighbour -> first;
			iUsedBlockCount += itmRightNeighbour -> second;

			// Remove from free index
			const SizeIndexRecord oFreeIndexRecord = { itmLeftNeighbour -> second, itmLeftNeighbour -> first };
			oFreeIndex.erase(oFreeIndexRecord);

			itmJoinLeft = itmLeftNeighbour;
			bJoinBlocks = true;
		}
	}

	// Join blocks
	if (bJoinBlocks)
	{
		mFreeMemoryMap.erase(itmJoinLeft, itmJoinRight);
		mFreeMemoryMap[vUsedData] = iUsedBlockCount;
	}

	// Add to free index
	const SizeIndexRecord oFreeIndexRecord = { iUsedBlockCount, vUsedData };
	oFreeIndex.insert(oFreeIndexRecord);

fprintf(stderr, "Free/OK\n");
}

//
// Get number of blocks
//
UINT_64 MemoryPool::GetBlocks(const UINT_64  iSize)
{
	return (iSize / iBlockSize) + ((iSize % iBlockSize) ? 1 : 0);
}

//
// Destructor
//
MemoryPool::~MemoryPool() throw()
{
	::operator delete(vData);
}

int main(void)
{
	{
		MemoryPool oMemoryPool(128, 1024);

		void * vData1 = oMemoryPool.Allocate(129);
		void * vData2 = oMemoryPool.Allocate(130);
		void * vData3 = oMemoryPool.Allocate(131);
		void * vData4 = oMemoryPool.Allocate(132);
		void * vData5 = oMemoryPool.Allocate(133);

		oMemoryPool.Free(vData2);
		oMemoryPool.Free(vData4);
		oMemoryPool.Free(vData3);

		oMemoryPool.Free(vData5);
		oMemoryPool.Free(vData1);

	}
/*
	{
		MemoryPool oMemoryPool(128, 1024);

		void * vData1 = oMemoryPool.Allocate(129);
		void * vData2 = oMemoryPool.Allocate(129);
		void * vData3 = oMemoryPool.Allocate(129);
		void * vData4 = oMemoryPool.Allocate(129);
		void * vData5 = oMemoryPool.Allocate(129);

		oMemoryPool.Free(vData2);
		oMemoryPool.Free(vData4);

		void * vData6 = oMemoryPool.Allocate(129);
		void * vData7 = oMemoryPool.Allocate(129);

		oMemoryPool.Free(vData1);
		oMemoryPool.Free(vData3);
		oMemoryPool.Free(vData5);
		oMemoryPool.Free(vData6);
		oMemoryPool.Free(vData7);
	}
*/
return EX_OK;
}
// End.
