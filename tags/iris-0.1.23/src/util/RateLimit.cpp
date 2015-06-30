/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "RateLimit.hpp"
#include "SystemVars.hpp"

namespace IRIS
{

//
// Constructor
//
RateLimit::RateLimit(RateLimitStorage  * pIStorage): pStorage(pIStorage) { ;; }

//
// Leaky bucket algorithm
//
bool RateLimit::CheckInterval(const W_FLOAT                dPerInterval,
                              const W_FLOAT                dBurst,
                              W_FLOAT                    & dATime,
                              W_FLOAT                    & dBucket,
                              const INT_32                 iAttempts,
                              const RateLimit::Interval    oInterval)
{
	// Get time
	const SystemVars::Time oTime = SystemVars::GetTime();

	// Time
	W_FLOAT dTimeNow   = oTime.sec + 1E-9 * oTime.nsec;
	if      (oInterval == PER_MINUTE) { dTimeNow /= 60;   }
	else if (oInterval == PER_HOUR)   { dTimeNow /= 3600; }

	const W_FLOAT dTimeDiff  = dTimeNow - dATime;

	bool bCheckResult = true;
	// Leaky bucked is on?
	if (dPerInterval != 0)
	{
		// Leak since last time we visited
		const W_FLOAT dTMP = (dTimeDiff * dPerInterval);

		if (dBucket > dTMP) { dBucket -= dTMP; }
		else                { dBucket = 0;     }

		// Bucket overflowed?
		if ((dBucket + iAttempts) > dBurst) { bCheckResult = false; }
		// Fill bucket
		else                                { dBucket += iAttempts; }
	}

	dATime = dTimeNow;

return bCheckResult;
}

//
// Check rate limit
//
bool RateLimit::SetLimit(const STLW::string  & sKey,
                         const W_FLOAT         dPerSecond,
                         const W_FLOAT         dBurst,
                         const UINT_32         iEvents)
{
	bool bCheckResult = true;
	for (;;)
	{
		W_FLOAT dATime  = 0.0;
		W_FLOAT dBucket = 0.0;
		UINT_64 iUnique = pStorage -> Get(sKey, dATime, dBucket);
		if (iUnique == 0)
		{
			dATime  = 0;
			dBucket = 1;
		}

		bCheckResult = CheckInterval(dPerSecond, dBurst, dATime, dBucket, iEvents);

		if (pStorage -> Set(sKey, dATime, dBucket, iUnique)) { break; }
	}

return bCheckResult;
}

//
// A destructor
//
RateLimit::~RateLimit() throw() { ;; }

} // namespace IRIS
// End.
