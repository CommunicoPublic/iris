/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "NetworkLoggerContext.hpp"

#include <stdio.h>

namespace IRIS
{

//
// Constructor
//
NetworkLoggerContext::NetworkLoggerContext() { ;; }

//
// Set parameter
//
void NetworkLoggerContext::SetParam(const STLW::string  & sParam,
                                    const INT_64          iValue)
{
	CHAR_8 szBuffer[1024];
	snprintf(szBuffer, 1023, "%lli", (long long)(iValue));
	mParams[sParam] = szBuffer;
}

//
// Set parameter
//
void NetworkLoggerContext::SetParam(const STLW::string  & sParam,
                                    const UINT_64         iValue)
{
	CHAR_8 szBuffer[1024];
	snprintf(szBuffer, 1023, "%lli", (long long unsigned)(iValue));
	mParams[sParam] = szBuffer;
}

//
// Set parameter
//
void NetworkLoggerContext::SetParam(const STLW::string  & sParam,
                                    const W_FLOAT         dValue)
{
	CHAR_8 szBuffer[1024];
	snprintf(szBuffer, 1023, "%.*G", 12, dValue);
	mParams[sParam] = szBuffer;
}

//
// Set parameter
//
void NetworkLoggerContext::SetParam(const STLW::string  & sParam,
                                    const STLW::string  & sValue)
{
	mParams[sParam] = sValue;
}

//
// Get parameter value for logger
//
void NetworkLoggerContext::GetParam(const STLW::string  & sParam,
                                    STLW::string        & sValue) const
{
	sValue = mParams[sParam];
}

//
// A destructor
//
NetworkLoggerContext::~NetworkLoggerContext() throw() { ;; }

} // namespace IRIS
// End.
