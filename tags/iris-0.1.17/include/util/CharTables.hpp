/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _CHAR_TABLES_HPP__
#define _CHAR_TABLES_HPP__ 1

/*
  @file CharTables.hpp
  @brief Character translation tables & accessors

  Unicode uppercase to lowercase conversion mapping table
  http://publib.boulder.ibm.com/infocenter/systems/index.jsp?topic=/nls/rbagslowtoupmaptable.htm&tocNode=int_39939

  Unicode lowercase to uppercase conversion mapping table
  http://publib.boulder.ibm.com/infocenter/systems/index.jsp?topic=/nls/rbagslowtoupmaptable.htm&tocNode=int_39940
*/

// Local includes
#include "Types.h"

namespace IRIS
{

/**
  @class CharTable CharTables.hpp <CharTables.hpp>
  @brief Unicode characters tables & utilities
*/
class CharTable
{
public:

	/**
	  @brief Convert an upper-case letter to the corresponding lower-case letter.
	  @param iChar - source character
	  @return Lowercase analog
	*/
	static UINT_32 Lowercase(const UINT_32 iChar);

	/**
	  @brief Convert an lower-case letter to the corresponding upper-case letter.
	  @param iChar - source character
	  @return Uppercase analog
	*/
	static UINT_32 Uppercase(const UINT_32 iChar);

	/**
	  @brief Get sorting order for specified char.
	  @param iChar - source character
	  @return Sorting order, integer, suitable for std::sort
	*/
	static UINT_32 LowercaseSortOrder(UINT_32 iChar);

	/**
	  @brief Get sorting order for specified char.
	  @param iChar - source character
	  @return Sorting order
	  @return Sorting order, integer, suitable for std::sort
	*/
	static UINT_32 UppercaseSortOrder(UINT_32 iChar);

private:
	/**
	  @class UCSTransMap CharTables.hpp <CharTables.hpp>
	  @brief Unicode translation map
	*/
	struct UCSTransMap
	{
		/** Source character                  */
		UINT_32    sc;
		/** Destination character             */
		UINT_32    dc;
		/** Sorting order                     */
		UINT_32    sortorder;
		/** Source character description      */
		CCHAR_P    desc_sc;
		/** Destination character description */
		CCHAR_P    desc_dc;
	};

	/** Upper-to-lower case translation map */
	static UCSTransMap mUCSU2L[];
	/** Lower-to-upper case translation map */
	static UCSTransMap mUCSL2U[];
};

} // namespace IRIS
#endif // _CHAR_TABLES_HPP__
// End.
