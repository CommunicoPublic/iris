/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _VARIANT_DUMPER_HPP__
#define _VARIANT_DUMPER_HPP__ 1

/**
  @file VariantDumper.hpp
  @brief Variant object dumper
*/

// Local includes
#include "STLString.hpp"
#include "Types.h"

namespace IRIS
{


/**
  @class VariantDumper VariantDumper.hpp <VariantDumper.hpp>
  @brief Variant object dumper
*/
class VariantDumper
{
public:
	enum State { OK, ERROR };

	/**
	  @brief Add integer element
	  @param iValue - element value
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	virtual State AddElement(const INT_64  iValue) = 0;

	/**
	  @brief Add unsigned integer element
	  @param iValue - element value
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	virtual State AddElement(const UINT_64  iValue) = 0;

	/**
	  @brief Add IEEE 754 element
	  @param dValue - element value
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	virtual State AddElement(const W_FLOAT  dValue) = 0;

	/**
	  @brief Add string element
	  @param sValue - element value
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	virtual State AddElement(const STLW::string & sValue) = 0;

	/**
	  @brief Start adding array
	  @param iElements - number of elements
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	virtual State StartArray(const UINT_64 iElements) = 0;

	/**
	  @brief Start adding array element
	  @param iPos - element position
	  @param bIslastElement - true, if element is last array element
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	virtual State StartElement(const UINT_64 iPos, const bool bIslastElement) = 0;

	/**
	  @brief End adding array element
	  @param bIslastElement - true, if element is last array element
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	virtual State EndElement(const bool bIslastElement) = 0;

	/**
	  @brief End adding array
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	virtual State EndArray() = 0;

	/**
	  @brief Start adding map
	  @param iPairs - number of elements in map
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	virtual State StartMap(const UINT_64 iPairs) = 0;

	/**
	  @brief Start adding map key -> value pair
	  @param sKey - key name
	  @param bIslastElement - true, if element is last map element
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	virtual State StartPair(const STLW::string & sKey, const bool bIslastElement) = 0;

	/**
	  @brief End adding map key -> value pair
	  @param bIslastElement - true, if element is last map element
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	virtual State EndPair(const bool bIslastElement) = 0;

	/**
	  @brief End adding map
	  @return Dumper state, one of [ OK, ERROR ]
	*/
	virtual State EndMap() = 0;

	/**
	  @brief Destructor
	*/
	virtual ~VariantDumper() throw();
private:

};

} // namespace IRIS
#endif // _VARIANT_DUMPER_HPP__
// End.
