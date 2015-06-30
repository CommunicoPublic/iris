/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _NETWORK_LOGGER_CONTEXT_HPP__
#define _NETWORK_LOGGER_CONTEXT_HPP__ 1

/**
  @file NetworkLoggerContext.hpp
  @brief Network logger data
*/

// Local includes
#include "LoggerContext.hpp"
#include "STLMap.hpp"

namespace IRIS
{

/**
  @class NetworkLoggerContext NetworkLoggerContext.hpp <LoggerContext.hpp>
  @brief Network logger data
*/
class NetworkLoggerContext:
  public LoggerContext
{
public:
	/**
	  @brief Constructor
	*/
	NetworkLoggerContext();

	/**
	  @brief Set parameter
	  @param sParam - parameter name
	  @param iParam - parameter value
	*/
	void SetParam(const STLW::string  & sParam,
	              const INT_64          iParam);

	/**
	  @brief Set parameter
	  @param sParam - parameter name
	  @param iParam - parameter value
	*/
	void SetParam(const STLW::string  & sParam,
	              const UINT_64         iParam);

	/**
	  @brief Set parameter
	  @param sParam - parameter name
	  @param dParam - parameter value
	*/
	void SetParam(const STLW::string  & sParam,
	              const W_FLOAT         dParam);

	/**
	  @brief Set parameter
	  @param sParam - parameter name
	  @param sValue - parameter value
	*/
	void SetParam(const STLW::string  & sParam,
	              const STLW::string  & sValue);
	/**
	  @brief Get parameter value for logger
	  @param sParam - parameter name
	  @param sValue - parameter value [out]
	*/
	void GetParam(const STLW::string  & sParam,
	              STLW::string        & sValue) const;

	/**
	  @brief A destructor
	*/
	~NetworkLoggerContext() throw();
private:
	// Does not exist
	NetworkLoggerContext(const NetworkLoggerContext  & oRhs);
	NetworkLoggerContext& operator=(const NetworkLoggerContext  & oRhs);
	/** Logger parameters */
	mutable STLW::map<STLW::string, STLW::string>    mParams;
};

} // namespace IRIS
#endif // _NETWORK_LOGGER_CONTEXT_HPP__
// End.
