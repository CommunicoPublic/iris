/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "RateLimit.hpp"
#include "SystemVars.hpp"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include <STLMap.hpp>

using namespace IRIS;

/**
  @class RateLimitStorage RateLimitStorage.hpp <RateLimitStorage.hpp>
  @brief storage for rate limits
*/
class RateLimitStorageTest:
  public RateLimitStorage
{
public:
	RateLimitStorageTest();

	/**
	  @brief Get rate limits from storage
	  @param sKey - key
	  @param dATime - access time
	  @param dBucket - bucket data
	  @return Unique transaction identifier
	*/
	UINT_64 Get(const STLW::string  & sKey,
	            W_FLOAT             & dATime,
	            W_FLOAT             & dBucket);

	/**
	  @brief Store rate limit to storage
	  @param sKey - key
	  @param dATime - access time
	  @param iUnique - unique identifier
	  @return true, if transaction commited, false - if any other transaction is in progress
	*/
	bool Set(const STLW::string  & sKey,
	         const W_FLOAT         dATime,
	         const W_FLOAT         dBucket,
	         const UINT_64         iUnique);

	/**
	  @brief A destructor
	*/
	~RateLimitStorageTest() throw();
private:
	struct RLData
	{
		W_FLOAT    atime;
		W_FLOAT    bucket;
		UINT_64    unique;
	};

	STLW::map<STLW::string, RLData> mRL;
};

//
// Constructor
//
RateLimitStorageTest::RateLimitStorageTest() { ;; }

//
// Get rate limits from storage
//
UINT_64 RateLimitStorageTest::Get(const STLW::string  & sKey,
                                  W_FLOAT             & dATime,
                                  W_FLOAT             & dBucket)
{
	STLW::map<STLW::string, RLData>::const_iterator itmRL = mRL.find(sKey);
	if (itmRL == mRL.end()) { return 0; }

	dATime  = itmRL -> second.atime;
	dBucket = itmRL -> second.bucket;

return itmRL -> second.unique;
}

//
// Store rate limit to storage
//
bool RateLimitStorageTest::Set(const STLW::string  & sKey,
                               const W_FLOAT         dATime,
                               const W_FLOAT         dBucket,
                               const UINT_64         iUnique)
{
	STLW::map<STLW::string, RLData>::iterator itmRL = mRL.find(sKey);
	if (itmRL == mRL.end() || iUnique == 0)
	{
		RLData oRLData;
		oRLData.atime  = dATime;
		oRLData.bucket = dBucket;
		oRLData.unique = random();

		mRL[sKey] = oRLData;
		return true;
	}

	if (itmRL -> second.unique != iUnique) { return false; }

	itmRL -> second.atime  = dATime;
	itmRL -> second.bucket = dBucket;
	itmRL -> second.unique = random();

return true;
}

//
// A destructor
//
RateLimitStorageTest::~RateLimitStorageTest() throw() { ;; }

int main(int argc, char ** argv)
{
	RateLimitStorageTest oRateLimitStorageTest;
	RateLimit oRateLimit(&oRateLimitStorageTest);

	if (argc != 3)
	{
		fprintf(stderr, "usage: %s per_second burst\n", argv[0]);
		return EX_USAGE;
	}

	UINT_32 iPerSecond = atoi(argv[1]);
	UINT_32 iBurst = atoi(argv[2]);

	// Get time
	SystemVars::Time oTime = SystemVars::GetTime();
	W_FLOAT dTimeStart = oTime.sec + 1E-9 * oTime.nsec;

	UINT_32 iPassed   = 0;
	UINT_32 iRejected = 0;
	for (UINT_32 iPos = 0; iPos < 50; ++iPos)
	{
		const bool bPassed = oRateLimit.SetLimit("somekey", iPerSecond, iBurst);
		if (bPassed) { ++iPassed;   }
		else         { ++iRejected; }

		fprintf(stdout, "Set limit %s\n", bPassed ? "OK":"NO");
		SystemVars::Sleep(50000);
	}

	oTime = SystemVars::GetTime();
	W_FLOAT dTimeEnd = oTime.sec + 1E-9 * oTime.nsec;
	fprintf(stdout, "Completed in %f seconds; %d passed, %d rejected\n", dTimeEnd - dTimeStart, iPassed, iRejected);
	fprintf(stdout, "Per second %d, burst %d; average %f\n", iPerSecond, iBurst, 1.0 * iPassed / (dTimeEnd - dTimeStart));

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
