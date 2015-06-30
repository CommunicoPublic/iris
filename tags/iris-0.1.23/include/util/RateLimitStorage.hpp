/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _RATE_LIMIT_STORAGE_HPP__
#define _RATE_LIMIT_STORAGE_HPP__ 1

/**
  @file RateLimitStorage.hpp
  @brief Leaky Bucket algorithm storage
*/

#include "STLString.hpp"
#include "Types.h"

namespace IRIS
{
/**
  @class RateLimitStorage RateLimitStorage.hpp <RateLimitStorage.hpp>
  @brief storage for rate limits
*/
class RateLimitStorage
{
public:
	/**
	  @brief Get rate limits from storage
	  @param sKey - key
	  @param dATime - access time
	  @param dBucket - bucket data
	  @return Unique transaction identifier
	*/
	virtual UINT_64 Get(const STLW::string  & sKey,
	                    W_FLOAT             & dATime,
	                    W_FLOAT             & dBucket) = 0;

	/**
	  @brief Store rate limit to storage
	  @param sKey - key
	  @param dATime - access time
	  @param dBucket - bucket data
	  @param iUnique - unique identifier
	  @return true, if transaction commited, false - if any other transaction is in progress
	*/
	virtual bool Set(const STLW::string  & sKey,
	                 const W_FLOAT         dATime,
	                 const W_FLOAT         dBucket,
	                 const UINT_64         iUnique) = 0;

	/**
	  @brief A destructor
	*/
	virtual ~RateLimitStorage() throw();
private:
};

} // namespace IRIS
#endif // _RATE_LIMIT_STORAGE_HPP__
// End.
