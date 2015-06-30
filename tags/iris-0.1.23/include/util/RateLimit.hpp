/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _RATE_LIMIT_HPP__
#define _RATE_LIMIT_HPP__ 1

/**
  @file RateLimit.hpp
  @brief Leaky Bucket algorithm
*/
#include "RateLimitStorage.hpp"

namespace IRIS
{

/**
  @class RateLimit RateLimit.hpp <RateLimit.hpp>
  @brief Check rate limit
*/
class RateLimit
{
public:
	enum Interval { PER_SECOND, PER_MINUTE, PER_HOUR };

	/**
	  @brief Constructor
	  @param pIStorage - Rate limit storage
	*/
	RateLimit(RateLimitStorage  * pIStorage);

	/**
	  @brief Check & set rate limit
	  @param sKey - key name
	  @param dPerSecond - number of requests per second/minute/hour
	  @param dBurst - determines how large bursts can be before exceeds the rate limit
	  @param iEvents - number of events
	*/
	bool SetLimit(const STLW::string  & sKey,
	              const W_FLOAT         dPerSecond,
	              const W_FLOAT         dBurst,
	              const UINT_32         iEvents = 1);

	/**
	  @brief A destructor
	*/
	~RateLimit() throw();
private:
	// Does not exist
	RateLimit(const RateLimit  & oRhs);
	RateLimit& operator=(const RateLimit  & oRhs);

	/** Rate limit storage */
	RateLimitStorage  * pStorage;

	/**
	  @brief Check time interval
	  @param dPerInterval - number of events per interval
	  @param dBurst - determines how large bursts can be before exceeds the rate limit
	  @param dATime - access time
	  @param dBucket - bucket data
	  @param iAttempts - number of attempts
	  @param oInterval - type of interval
	  @return true, if rate limit not exceed, false - otherwise
	*/
	static bool CheckInterval(const W_FLOAT                 dPerInterval,
	                          const W_FLOAT                 dBurst,
	                          W_FLOAT                     & dATime,
	                          W_FLOAT                     & dBucket,
	                          const INT_32                  iAttempts,
	                          const RateLimit::Interval     oInterval = PER_SECOND);
};

} // namespace IRIS
#endif // _RATE_LIMIT_HPP__
// End.
