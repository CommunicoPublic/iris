/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MESSAGE_SUBSCRIBER_HPP__
#define _MESSAGE_SUBSCRIBER_HPP__ 1

/**
  @file MessageSubscriber.hpp
  @brief Message queue subscriber
*/

// Local includes
#include "Object.hpp"

namespace IRIS
{

// FWD
class Message;

/**
  @class MessageSubscriber MessageSubscriber.hpp <MessageSubscriber.hpp>
  @brief Message queue subscriber
*/
class MessageSubscriber:
  public Object
{
public:
	virtual void ReceiveMessage(const Message  * pMessage,
	                            const UINT_64    iResultMap) = 0;

	virtual ~MessageSubscriber() throw();
};

} // namespace IRIS
#endif // _MESSAGE_SUBSCRIBER_HPP__
// End.
