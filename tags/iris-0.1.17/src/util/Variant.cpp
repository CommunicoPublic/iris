/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "UnicodeUtils.hpp"
#include "Variant.hpp"

namespace IRIS
{

//
// Compare two strings ingnore case
//
bool NoCaseCmp::operator()(const STLW::string oX, const STLW::string & oY) const
{
	return Unicode::CompareIgnoreCase(oX.data(), oX.size(), oY.data(), oY.size()) < 0;
}

} // namespace IRIS
// End.
