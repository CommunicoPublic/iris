/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _VARIANT_DUMPER_JSON__
#define _VARIANT_DUMPER_JSON__ 1

/**
  @file VariantDumperJSON.hpp
  @brief Dump Variant data type to JSON object
*/

#include "VariantDumper.hpp"

namespace IRIS
{
// FWD
class StringBuffer;

/**
  @brief VariantDumperJSON VariantDumperJSON.hpp <VariantDumperJSON.hpp>
  @brief Dump Variant data type to JSON object
*/
class VariantDumperJSON:
  public VariantDumper
{
public:
	VariantDumperJSON(BaseStringBuffer  * pStringBuffer);

	/**
	  @brief Add integer element
	  @param iValue - element value
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	VariantDumper::State AddElement(const INT_64  iValue);

	/**
	  @brief Add IEEE 754 element
	  @param dValue - element value
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
	  @param iPairs - number of elements in map
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
	~VariantDumperJSON() throw();
private:
	// Does not exist
	VariantDumperJSON(const VariantDumperJSON  & oRhs);
	VariantDumperJSON& operator=(const VariantDumperJSON  & oRhs);

	/** Stream for dump */
	BaseStringBuffer  * pStringBuffer;
};

} // namespace IRIS
#endif // _VARIANT_DUMPER_JSON__
// End.
