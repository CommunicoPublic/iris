/*
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "ObjectId.hpp"

#include <uuid/uuid.h>
#include <string.h>
#include <stdio.h>

namespace IRIS
{
//
// Generate unique id
//
ObjectId ObjectId::Generate()
{
	uuid_t oUUID;
	uuid_generate_random(oUUID);

return ObjectId(oUUID);
}

//
// Constructor
//
ObjectId::ObjectId(const UINT_64  iHi,
                   const UINT_64  iLo) { aData[0] = iHi; aData[1] = iLo; }

//
// Constructor
//
ObjectId::ObjectId(UCCHAR_P  aUUID)
{
	memcpy(&aData[0], aUUID, 16);
}

//
// Compare two objects
//
const bool ObjectId::operator <(const ObjectId & oObjectId) const
{
	return ((aData[0] < oObjectId.aData[0]) ||
	        (aData[0] == oObjectId.aData[0] && aData[1] < oObjectId.aData[1]));
}

//
// Compare two objects
//
const bool ObjectId::operator >(const ObjectId & oObjectId) const { return oObjectId.operator<(*this); }

//
// Compare two objects
//
const bool ObjectId::operator ==(const ObjectId & oObjectId) const { return (! this -> operator <(oObjectId) && ! oObjectId.operator <(*this)); }

//
// Compare two objects
//
const bool ObjectId::operator !=(const ObjectId & oObjectId) const { return !(this -> operator ==(oObjectId)); }

//
// Compare two objects
//
const bool ObjectId::operator >=(const ObjectId & oObjectId) const { return (! this -> operator <(oObjectId)); }

//
// Compare two objects
//
const bool ObjectId::operator <=(const ObjectId & oObjectId) const { return (! this -> operator >(oObjectId)); }

//
// Check object is null
//
const bool ObjectId::IsNull() const { return aData[0] == 0 && aData[1] == 0; }

//
// Dump object
//
void ObjectId::Dump(CHAR_P sBuffer) const
{
	snprintf(sBuffer, 33, "%016llX%016llX", (long long unsigned)aData[0], (long long unsigned)aData[1]);
}

//
// Restore object
//
void ObjectId::Restore(CCHAR_P sBuffer)
{
	sscanf(sBuffer, "%016llX%016llX", (long long unsigned *)&aData[0], (long long unsigned *)&aData[1]);
}

//
// Get higher 64 bits
//
const UINT_64 ObjectId::GetHi() const { return aData[0]; }

//
// Get lower 64 bits
//
const UINT_64 ObjectId::GetLo() const { return aData[1]; }

} // namespace IRIS
// End.
