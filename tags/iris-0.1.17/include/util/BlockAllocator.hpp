/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _BLOCK_ALLOCATOR_HPP__
#define _BLOCK_ALLOCATOR_HPP__ 1

/**
  @file BlockAllocator.hpp
  @brief Block-aligned vemory pool for objects
*/

#include "AVLTree.hpp"
#include "FixedSizeAllocator.hpp"

namespace IRIS
{

/**
  @class BlockAllocator BlockAllocator.hpp <BlockAllocator.hpp>
  @brief Memory pool for fixed size objects
*/
class BlockAllocator:
  public Allocator
{
public:
	/**
	  @brief Constructor
	  @param iIObjectSize - object size
	  @param iIMaxObjects - max. number of objects in pool
	*/
	BlockAllocator(const UINT_64 iIBlockSize,
	               const UINT_64 iIMaxBlocks);

	/**
	  @brief Allocate chunk
	*/
	void * Allocate(const UINT_64 iSize);

	/**
	  @brief Deallocate chunk
	*/
	void Deallocate(void * vData);

	/**
	  @brif Destructor
	*/
	~BlockAllocator() throw();

private:
	// Does not exist
	BlockAllocator(const BlockAllocator  & oRhs);
	BlockAllocator& operator=(const BlockAllocator  & oRhs);

	/**
	  @struct SizeIndexRecord BlockAllocator.hpp <BlockAllocator.hpp>
	  @brief Record for block-by-size index
	*/
	struct SizeIndexRecord
	{
		/** Block size     */
		UINT_64    blocks;
		/** Block offset   */
		UINT_64    offset;
	};

	/**
	  @struct SizeIndexRecordCMP BlockAllocator.hpp <BlockAllocator.hpp>
	  @brief Comparator for block-by-size index
	*/
	struct SizeIndexRecordCMP
	{
		bool operator()(const SizeIndexRecord  & oX,
		                const SizeIndexRecord  & oY) const
		{
			if (oX.blocks == oY.blocks) { return oX.offset < oY.offset; }

		return oX.blocks < oY.blocks;
		}
	};

	typedef AVLTree<SizeIndexRecord, SizeIndexRecordCMP, FixedSizeAllocator> TSizeIndex;

	/** Block size                */
	const UINT_64         iBlockSize;
	/** Max. number of blocks     */
	const UINT_64         iMaxBlocks;
	/** Memory block              */
	void                * vData;
	/** AVL tree memory allocator */
	FixedSizeAllocator    oFreeIndexAllocator;
	/** Comparator                */
	SizeIndexRecordCMP    oCMP;
	/** Size index                */
	TSizeIndex            oFreeIndex;

	/**
	  @brief Get header size
	*/
	static UINT_64 HeaderSize();

	/**
	  @brief Get number of blocks
	*/
	UINT_64 GetBlocks(const UINT_64  iSize);

	/**
	  @brief Get block size
	*/
	void GetPrevBlockSize(const UINT_64    iOffset,
	                      UINT_64        & iPrevSize);
	/**
	  @brief Store block size
	*/
	void StorePrevBlockSize(const UINT_64  iOffset,
	                        const UINT_64  iPrevSize);

	/**
	  @brief Get block size
	*/
	void GetBlockSize(const UINT_64    iOffset,
	                  UINT_64        & iSize);
	/**
	  @brief Store block size
	*/
	void StoreBlockSize(const UINT_64  iOffset,
	                    const UINT_64  iSize);

	/**
	  @brief Get memory block offset
	*/
	void * GetBlockOffset(const UINT_64  iStartBlock);

	/**
	  @brief Get memory block offset
	*/
	UINT_64 GetBlockOffset(void * vBlock);
};
} // namespace IRIS
#endif // _BLOCK_ALLOCATOR_HPP__
// End.
