/*
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _OBJECT_ID_HPP__
#define _OBJECT_ID_HPP__ 1

#include <Types.h>

namespace IRIS
{
/**
  @class ObjectId ObjectId.hpp <ObjectId.hpp>
  @brief Object Id
*/
class ObjectId
{
public:
	/**
	  @brief generate unique object Id
	*/
	static ObjectId Generate();

	/**
	  @brief Constructor
	*/
	explicit ObjectId(const UINT_64  iHi = 0,
	                  const UINT_64  iLo = 0);

	/**
	  @brief Compare two objects
	*/
	const bool operator <(const ObjectId & oObjectId) const;

	/**
	  @brief Compare two objects
	*/
	const bool operator >(const ObjectId & oObjectId) const;

	/**
	  @brief Compare two objects
	*/
	const bool operator >=(const ObjectId & oObjectId) const;

	/**
	  @brief Compare two objects
	*/
	const bool operator <=(const ObjectId & oObjectId) const;

	/**
	  @brief Compare two objects
	*/
	const bool operator ==(const ObjectId & oObjectId) const;

	/**
	  @brief Compare two objects
	*/
	const bool operator !=(const ObjectId & oObjectId) const;

	/**
	  @brief Check object is null
	*/
	const bool IsNull() const;

	/**
	  @brief Dump object
	*/
	void Dump(CHAR_P sBuffer) const;

	/**
	  @brief Restore object
	*/
	void Restore(CCHAR_P sBuffer);

	/**
	  @brief Get higher 64 bits
	*/
	const UINT_64 GetHi() const;

	/**
	  @brief Get lower 64 bits
	*/
	const UINT_64 GetLo() const;

private:
	/**
	  @brief Constructor
	*/
	explicit ObjectId(UCCHAR_P  aUUID);

	/**
	  @brief ObjectId
	*/
	UINT_64  aData[2];
};

} // namespace IRIS
#endif // _OBJECT_ID_HPP__
// End.

