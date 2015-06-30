/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _OBJECT_HPP__
#define _OBJECT_HPP__ 1

/**
  @file Object.hpp
  @brief Iiris server base object class
*/
#include "Types.h"

namespace IRIS
{

/**
  @class Object Object.hpp <Object.hpp>
  @brief Iris server base object class
*/
class Object
{
public:
	/**
	  @brief Get object type
	  @return Human-readable type of object
	*/
	virtual CCHAR_P GetObjectType() const = 0;

	/**
	  @brief Get object name
	  @return Human-readable name of object
	*/
	virtual CCHAR_P GetObjectName() const = 0;

	/**
	  @brief A virtual destructor
	*/
	virtual ~Object() throw() { ;; }
};

} // namespace IRIS
#endif // _OBJECT_HPP__
// End.
