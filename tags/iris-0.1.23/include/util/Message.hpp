/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MESSAGE_HPP__
#define _MESSAGE_HPP__ 1

/**
  @file Message.hpp
  @brief Message for queue
*/

// Local includes
#include "Object.hpp"

namespace IRIS
{

/**
  @class Message Message.hpp <Message.hpp>
  @brief Message for queue
*/
class Message:
  public Object
{
public:
	virtual UINT_64 GetPriority() const = 0;

	virtual UINT_64 GetEventFlags() const = 0;

	virtual ~Message() throw();
};

} // namespace IRIS
#endif // _MESSAGE_HPP__
// End.
