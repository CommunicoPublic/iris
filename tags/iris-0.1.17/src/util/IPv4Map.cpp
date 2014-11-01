/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "IPv4Map.hpp"

namespace IRIS
{

//
// Sorting comparator
//
bool IPv4Map::IPRangeLess::operator() (const IPRange  & oX,
                                       const IPRange  & oY) const { return oX.start_ip < oY.start_ip; }

//
// A destructor
//
IPv4Map::~IPv4Map() throw() { ;; }

//
// Add network
//
IPMap::State IPv4Map::AddNet(const STLW::string  & sSubnet)
{
	IPRange oIPRange;
	IPMap::State oRC = ParseNetwork(sSubnet, oIPRange);
	if (oRC != IPMap::OK) { return oRC; }

return AddRange(oIPRange);
}

//
// Add range of addresses
//
IPMap::State IPv4Map::AddRange(const STLW::string  & sStartIP,
                               const STLW::string  & sEndIP)
{
	IPRange oIPRange;
	IPMap::State oRC = ParseAddress(sStartIP, oIPRange.start_ip);
	if (oRC != IPMap::OK) { return oRC; }

	oRC = ParseAddress(sStartIP, oIPRange.end_ip);
	if (oRC != IPMap::OK) { return oRC; }

	if (oIPRange.start_ip > oIPRange.end_ip) { return IPMap::ERROR; }

return AddRange(oIPRange);
}

//
// Add range of addresses
//
IPMap::State IPv4Map::AddRange(IPRange  & oIPRange)
{
	IPRangeMap::const_iterator  itoRange = oRange.lower_bound(oIPRange);
	if (!oRange.empty()) { --itoRange; }

	while (itoRange != oRange.end())
	{
		if ((oIPRange.start_ip >= itoRange -> start_ip && oIPRange.start_ip <= itoRange -> end_ip) ||
		    (oIPRange.end_ip >= itoRange -> start_ip && oIPRange.end_ip <= itoRange -> end_ip))
		{
			return IPMap::ERROR;
		}

		if (oIPRange.start_ip < itoRange -> start_ip && oIPRange.end_ip < itoRange -> start_ip) { break; }

		++itoRange;
	}
	oRange.insert(oIPRange);

return IPMap::OK;
}

//
// Check IP address
//
IPMap::State IPv4Map::CheckIP(const STLW::string & sAddress) const
{
	IPRange oIPRange;
	IPMap::State oRC = ParseAddress(sAddress, oIPRange.start_ip);
	if (oRC != IPMap::OK) { return oRC; }

	IPRangeMap::const_iterator  itoRange = oRange.lower_bound(oIPRange);
	if (!oRange.empty()) { --itoRange; }
	while (itoRange != oRange.end())
	{
		if (oIPRange.start_ip >= itoRange -> start_ip && oIPRange.start_ip <= itoRange -> end_ip)
		{
			return IPMap::OK;
		}

		if (oIPRange.start_ip < itoRange -> start_ip && oIPRange.start_ip < itoRange -> end_ip) { break; }

		++itoRange;
	}

return IPMap::ERROR;
}

//
// Parse IPv4 network
//
IPMap::State IPv4Map::ParseNetwork(const STLW::string  & sSubnet,
                                   IPRange             & oIPRange)
{
	if (sSubnet.empty()) { return IPMap::ERROR; }

	oIPRange.start_ip = 0;
	oIPRange.end_ip   = 0;

	UINT_32 iOctetNum = 0;
	STLW::string::const_iterator itsSubnet = sSubnet.begin();
	for (; iOctetNum < 4; ++iOctetNum)
	{
		UINT_32 iOctet = 0;
		for(;;)
		{
			if (*itsSubnet >= '0' && *itsSubnet <= '9') { iOctet = iOctet * 10 + (*itsSubnet - '0'); }
			else { break; }
			++itsSubnet;
		}

		if (iOctet > 255) { return IPMap::ERROR; }
		oIPRange.start_ip = (oIPRange.start_ip << 8) + iOctet;

		if (*itsSubnet != '.') { break; }
		++itsSubnet;
	}
	oIPRange.end_ip = oIPRange.start_ip = oIPRange.start_ip << (8 * (3 - iOctetNum));

	// Subnet?
	if (*itsSubnet == '/')
	{
		UINT_32 iBits = 0;
		++itsSubnet;
		for(;;)
		{
			if (*itsSubnet >= '0' && *itsSubnet <= '9') { iBits = iBits * 10 + (*itsSubnet - '0'); }
			else { break; }
			++itsSubnet;
		}
		if (iBits > 32) { return IPMap::ERROR; }
		oIPRange.end_ip = oIPRange.start_ip | (0xFFFFFFFFUL >> iBits);
	}

return IPMap::OK;
}

//
// Parse IPv4 address
//
IPMap::State IPv4Map::ParseAddress(const STLW::string  & sAddress,
                                   UINT_32             & iIP)
{
	if (sAddress.empty()) { return IPMap::ERROR; }

	UINT_32 iOctetNum = 0;
	STLW::string::const_iterator itsAddress = sAddress.begin();
	for (; iOctetNum < 4; ++iOctetNum)
	{
		UINT_32 iOctet = 0;
		for(;;)
		{
			if (*itsAddress >= '0' && *itsAddress <= '9') { iOctet = iOctet * 10 + (*itsAddress - '0'); }
			else { break; }
			++itsAddress;
		}

		if (iOctet > 255) { return IPMap::ERROR; }
		iIP = (iIP << 8) + iOctet;

		if (*itsAddress != '.') { break; }
		++itsAddress;
	}

	if (iOctetNum != 3) { return IPMap::ERROR; }

return IPMap::OK;
}

} // namespace IRIS
// End.
