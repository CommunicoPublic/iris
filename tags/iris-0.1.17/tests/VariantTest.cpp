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
	  @brief Start adding map
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
	Variant oVariant1;
	oVariant1["bar"]["x"]= "4";
	oVariant1["bar"]["y"]= "5";
	oVariant1["bar"]["z"]= "6";
	oVariant1["foo"][1]["bar"]= "1";
	oVariant1["foo"][3]["bar"]= "2";
	oVariant1["foo"][5]["bar"]= "3";

	Variant oVariant2;
	oVariant2["bar"]["y"]["p"] = "8";
	oVariant2["bar"]["z"]= "9";
	oVariant2["foo"][2]["bar"]= "4";
	oVariant2["foo"][3]["bar"]= "5";
	oVariant2["foo"][4]["bar"]= "6";

	STLW::map<STLW::string, UINT_32> mData;
	mData["baz"] = 1;
	mData["boo"] = 10;

	Variant oVariant3;
	oVariant3["map"] = mData;

	STLW::vector<UINT_32> vData;
	vData.push_back(100);
	vData.push_back(200);

	oVariant3["vector"] = vData;

	const STLW::map<STLW::string, STLW::string> mData1 = oVariant3["map"];
	const STLW::vector<STLW::string> vData1 = oVariant3["vector"];

	STLW::stringstream sStream;
	STLW::map<STLW::string, STLW::string>::const_iterator itmData1 = mData1.begin();
	sStream << "{";
	if (itmData1 != mData1.end())
	{
		for(;;)
		{
			sStream << itmData1 -> first << ":" << itmData1 -> second;
			++itmData1;

			if (itmData1 == mData1.end()) { break; }
			sStream << ", ";
		}
	}
	sStream << "}\n[";

	STLW::vector<STLW::string>::const_iterator itvData1 = vData1.begin();
	if (itvData1 != vData1.end())
	{
		for(;;)
		{
			sStream << *itvData1;
			++itvData1;

			if (itvData1 == vData1.end()) { break; }
			sStream << ", ";
		}
	}
	sStream << "]\n";

	TestVariantDumper oDumper(sStream);
	sStream << "\noVariant1 = ";
	oVariant1.Dump(&oDumper);

	sStream << "\noVariant2 = ";
	oVariant2.Dump(&oDumper);

	sStream << "\noVariant3  = ";
	oVariant3.Dump(&oDumper);

	Variant oVariant4 = oVariant1.Diff(oVariant2);
	sStream << "\nDiff = ";
	oVariant4.Dump(&oDumper);

	oVariant1.Merge(oVariant2);

	sStream << "\nMerge = ";
	oVariant1.Dump(&oDumper);

	fprintf(stderr, "%s\n", sStream.str().c_str());

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
