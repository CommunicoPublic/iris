/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _IP_V4_MAP_HPP__
#define _IP_V4_MAP_HPP__ 1

/**
  @file IPMap.hpp
  @brief IP address map
*/

#include "IPMap.hpp"
#include "STLSet.hpp"

namespace IRIS
{
/**
  @struct IPv4Map IPv4Map.hpp <IPv4Map.hpp>
  @brief IPv4 address map
*/
class IPv4Map:
  public IPMap
{
public:

	/**
	  @brief A virtual destructor
	*/
	~IPv4Map() throw();

	/**
	  @brief Add network
	  @param sSubnet - network with subnet or IP address
	*/
	IPMap::State AddNet(const STLW::string  & sSubnet);

	/**
	  @brief Add range of addresses
	  @param sStartIP - first IP address in range
	  @param sEndIP - last IP address in range
	*/
	IPMap::State AddRange(const STLW::string  & sStartIP,
	                      const STLW::string  & sEndIP);

	/**
	  @brief Check IP address
	  @param sAddress - IP address to check
	*/
	IPMap::State CheckIP(const STLW::string & sAddress) const;
private:

	/**
	  @struct IPv4Map IPv4Map.hpp <IPv4Map.hpp>
	  @brief IPv4-to-Location map
	*/
	struct IPRange
	{
		/** First IP address in range */
		UINT_32  start_ip;
		/** Last IP address in range */
		UINT_32  end_ip;

	};

	/**
	  @struct IPv4Map IPv4Map.hpp <IPv4Map.hpp>
	  @brief Sorting comparator
	*/
	struct IPRangeLess
	{
		bool operator() (const IPRange  & oX,
		                 const IPRange  & oY) const;
	};

	typedef STLW::set<IPRange, IPRangeLess>  IPRangeMap;
	/** IP range */
	IPRangeMap  oRange;

	/**
	  @brief Add range of addresses
	*/
	IPMap::State AddRange(IPRange  & oIPRange);

	/**
	  @brief Parse IPv4 network
	  @param sSubnet - IPv4 network in format AA.BB.CC.DD/NN
	  @param oIPRange - ip address range
	*/
	static IPMap::State ParseNetwork(const STLW::string  & sSubnet,
	                                 IPRange             & oIPRange);

	/**
	  @brief Parse IPv4 address
	  @param sAddress - IP address
	  @param iIP - ip address [out]
	*/
	static IPMap::State ParseAddress(const STLW::string  & sAddress,
	                                 UINT_32             & iIP);
};

} // namespace IRIS
#endif // _IP_V4_MAP_HPP__
// End.
