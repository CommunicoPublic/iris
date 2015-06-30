/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "StringBuffer.hpp"
#include "VariantDumperJSON.hpp"

#include <stdio.h>

namespace IRIS
{

//
// Escape JSON string value
//
static void EscapeJSONString(const STLW::string  & sSource,
                             BaseStringBuffer    * pStringBuffer)
{
	pStringBuffer -> Store('"');
	STLW::string::const_iterator itsSource = sSource.begin();
	while(itsSource != sSource.end())
	{
		const UCHAR_8 chTMP = *itsSource;
		bool bHandleOther = false;
		switch(chTMP)
		{
			case '\'':
				pStringBuffer -> Store("\\'");
				break;
			case '"':
				pStringBuffer -> Store("\\\"");
				break;
			case '\\':
				pStringBuffer -> Store("\\\\");
				break;
			case '/':
				pStringBuffer -> Store("\\/");
				break;
			case '\b':
				pStringBuffer -> Store("\\b");
				break;
			case '\f':
				pStringBuffer -> Store("\\f");
				break;
			case '\n':
				pStringBuffer -> Store("\\n");
				break;
			case '\r':
				pStringBuffer -> Store("\\r");
				break;
			case '\t':
				pStringBuffer -> Store("\\t");
				break;
			case '\v':
				pStringBuffer -> Store("\\v");
				break;
			case '<':
				pStringBuffer -> Store("\\u003c");
				break;
			case '>':
				pStringBuffer -> Store("\\u003e");
				break;
			default:
				bHandleOther = true;
		}
		if (bHandleOther)
		{
			if (chTMP < ' ')
			{
				CHAR_8 szTMP[7];
				snprintf(szTMP, 7, "\\u%04x", chTMP);
				pStringBuffer -> Store(szTMP);
			}
			else
			{
				pStringBuffer -> Store(chTMP);
			}
		}
		++itsSource;
	}
	pStringBuffer -> Store('"');
}

//
// Constructor
//
VariantDumperJSON::VariantDumperJSON(BaseStringBuffer  * pIStringBuffer): pStringBuffer(pIStringBuffer)
{
	;;
}

//
// Add integer element
//
VariantDumper::State VariantDumperJSON::AddElement(const INT_64  iValue)
{
	CHAR_8 szBuffer[1024];
	snprintf(szBuffer, 1023, "%lli", (long long)(iValue));
	pStringBuffer -> Store(szBuffer);

return VariantDumper::OK;
}

//
// Add unsigned integer element
//
VariantDumper::State VariantDumperJSON::AddElement(const UINT_64  iValue)
{
	CHAR_8 szBuffer[1024];
	snprintf(szBuffer, 1023, "%llu", (long long unsigned)(iValue));
	pStringBuffer -> Store(szBuffer);

return VariantDumper::OK;
}

//
// Add IEEE 754 element
//
VariantDumper::State VariantDumperJSON::AddElement(const W_FLOAT  dValue)
{
	CHAR_8 szBuffer[1024];
	snprintf(szBuffer, 1023, "%.*G", 12, dValue);
	pStringBuffer -> Store(szBuffer);

return VariantDumper::OK;
}

//
// Add string element
//
VariantDumper::State VariantDumperJSON::AddElement(const STLW::string & sValue)
{
	EscapeJSONString(sValue, pStringBuffer);

return VariantDumper::OK;
}

//
// Start adding array
//
VariantDumper::State VariantDumperJSON::StartArray(const UINT_64 iElements)
{
	pStringBuffer -> Store('[');

return VariantDumper::OK;
}

//
// Start adding array element
//
VariantDumper::State VariantDumperJSON::StartElement(const UINT_64 iPos, const bool bIslastElement)
{
	;;
return VariantDumper::OK;
}

//
// End adding array element
//
VariantDumper::State VariantDumperJSON::EndElement(const bool bIslastElement)
{
	if (!bIslastElement) { pStringBuffer -> Store(','); }

return VariantDumper::OK;
}

//
// End adding array
//
VariantDumper::State VariantDumperJSON::EndArray()
{
	pStringBuffer -> Store(']');

return VariantDumper::OK;
}

//
// Start adding map
//
VariantDumper::State VariantDumperJSON::StartMap(const UINT_64 iPairs)
{
	pStringBuffer -> Store('{');

return VariantDumper::OK;
}

//
// Start adding map key -> value pair
//
VariantDumper::State VariantDumperJSON::StartPair(const STLW::string & sKey, const bool bIslastElement)
{
	EscapeJSONString(sKey, pStringBuffer);
	pStringBuffer -> Store(':');

return VariantDumper::OK;
}

//
// End adding map
//
VariantDumper::State VariantDumperJSON::EndPair(const bool bIslastElement)
{
	if (!bIslastElement) { pStringBuffer -> Store(','); }

return VariantDumper::OK;
}

//
// End adding map
//
VariantDumper::State VariantDumperJSON::EndMap()
{
	pStringBuffer -> Store('}');

return VariantDumper::OK;
}

//
// Destructor
//
VariantDumperJSON::~VariantDumperJSON() throw()
{
	;;
}

} // namespace IRIS
// End.
