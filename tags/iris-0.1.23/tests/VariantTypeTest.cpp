/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "Variant.hpp"
#include "VariantDumper.hpp"
#include "STLSstream.hpp"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

using namespace IRIS;

class TestVariantDumper:
  public VariantDumper
{
public:
	TestVariantDumper(STLW::stringstream  & sIStream);

	/**
	  @brief Add integer element
	  @param iValue - element value
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	VariantDumper::State AddElement(const INT_64  iValue);

	/**
	  @brief Add unsigned integer element
	  @param iValue - element value
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	VariantDumper::State AddElement(const UINT_64  iValue);

	/**
	  @brief Add IEEE 754 element
	  @param sValue - element value
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	VariantDumper::State AddElement(const W_FLOAT  dValue);

	/**
	  @brief Add string element
	  @param sValue - element value
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	VariantDumper::State AddElement(const STLW::string & sValue);

	/**
	  @brief Start adding array
	  @param iElements - number of elements
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	VariantDumper::State StartArray(const UINT_64 iElements);

	/**
	  @brief Start adding array element
	  @param iPos - element position
	  @param bIslastElement - true, if element is last array element
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	VariantDumper::State StartElement(const UINT_64 iPos, const bool bIslastElement);

	/**
	  @brief End adding array element
	  @param bIslastElement - true, if element is last array element
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	VariantDumper::State EndElement(const bool bIslastElement);

	/**
	  @brief End adding array
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	VariantDumper::State EndArray();

	/**
	  @brief Start adding map
	  @param iElements - number of elements
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	VariantDumper::State StartMap(const UINT_64 iPairs);

	/**
	  @brief Start adding map key -> value pair
	  @param sKey - key name
	  @param bIslastElement - true, if element is last map element
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	VariantDumper::State StartPair(const STLW::string & sKey, const bool bIslastElement);

	/**
	  @brief End adding map key -> value pair
	  @param bIslastElement - true, if element is last map element
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	VariantDumper::State EndPair(const bool bIslastElement);

	/**
	  @brief End adding map
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	VariantDumper::State EndMap();

	/**
	  @brief Destructor
	*/
	~TestVariantDumper() throw();
private:
	STLW::stringstream  & sStream;
	UINT_64               iLevel;
};

//
// Constructor
//
TestVariantDumper::TestVariantDumper(STLW::stringstream  & sIStream): sStream(sIStream), iLevel(0)
{
	;;
}

//
// Add integer element
//
VariantDumper::State TestVariantDumper::AddElement(const INT_64  iValue)
{
	sStream << iValue;
return VariantDumper::OK;
}

//
// Add integer element
//
VariantDumper::State TestVariantDumper::AddElement(const UINT_64  iValue)
{
        sStream << iValue;
return VariantDumper::OK;
}

//
// Add IEEE 754 element
//
VariantDumper::State TestVariantDumper::AddElement(const W_FLOAT  dValue)
{
	sStream << dValue;
return VariantDumper::OK;
}

//
// Add string element
//
VariantDumper::State TestVariantDumper::AddElement(const STLW::string & sValue)
{
	sStream << sValue;
return VariantDumper::OK;
}

//
// Start adding array
//
VariantDumper::State TestVariantDumper::StartArray(const UINT_64 iElements)
{
	++iLevel;
	sStream << "ARRAY(" << iElements << ") [";
return VariantDumper::OK;
}

//
// Start adding array element
//
VariantDumper::State TestVariantDumper::StartElement(const UINT_64 iPos, const bool bIslastElement)
{
	sStream << iPos << " : ";
return VariantDumper::OK;
}

//
// End adding array element
//
VariantDumper::State TestVariantDumper::EndElement(const bool bIslastElement)
{
	if (!bIslastElement) { sStream << ", "; }
return VariantDumper::OK;
}

//
// End adding array
//
VariantDumper::State TestVariantDumper::EndArray()
{
	--iLevel;
	sStream << "]";
return VariantDumper::OK;
}

//
// Start adding map
//
VariantDumper::State TestVariantDumper::StartMap(const UINT_64 iPairs)
{
	++iLevel;
	sStream << "MAP(" << iPairs << ") {\n";
return VariantDumper::OK;
}

//
// Start adding map key -> value pair
//
VariantDumper::State TestVariantDumper::StartPair(const STLW::string & sKey, const bool bIslastElement)
{
	++iLevel;
	sStream  << STLW::string(iLevel * 4, ' ') << sKey << " => ";
return VariantDumper::OK;
}

//
// End adding map
//
VariantDumper::State TestVariantDumper::EndPair(const bool bIslastElement)
{
	--iLevel;
	if (!bIslastElement) { sStream << ";\n"; }
return VariantDumper::OK;
}

//
// End adding map
//
VariantDumper::State TestVariantDumper::EndMap()
{
	sStream << "\n" << STLW::string(iLevel * 4, ' ') << "}";
	--iLevel;
return VariantDumper::OK;
}

//
// Destructor
//
TestVariantDumper::~TestVariantDumper() throw()
{
	;;
}

int main(void)
{
	Variant oVariant;

	const UINT_64 iMaxUINT = UINT_64(-1);

	oVariant["max_uint"]  = iMaxUINT;
	oVariant["int"]       = INT_64(iMaxUINT);

	STLW::stringstream sStream;
	TestVariantDumper oDumper(sStream);
	sStream << "\noVariant = ";
	oVariant.Dump(&oDumper);

	fprintf(stderr, "%s\n", sStream.str().c_str());

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
