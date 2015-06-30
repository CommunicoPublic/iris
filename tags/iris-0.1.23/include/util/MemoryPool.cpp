#include <STLMap.hpp>

#include <stdio.h>

class LRUMemoryPool
{
public:
	LRUMemoryPool(const UINT_64 iBlockSize);
private:
	LRUMemoryPool(const LRUMemoryPool  & oRhs);
	LRUMemoryPool& operator=(const LRUMemoryPool  & oRhs);

	struct PoolRecord
	{
		UINT_32  pool_id;
		UINT_32  block_count;
	};

	typedef STLW::map<void *, PoolRecord>  TPoolRecord;
};

int main(void)
{
	LRUMemoryPool oLRUMemoryPool(128);
return EX_OK;
}
// End.
