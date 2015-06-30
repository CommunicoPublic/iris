/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _MESSAGE_QUEUE_HPP__
#define _MESSAGE_QUEUE_HPP__ 1

/**
  @file MessageQueue.hpp
  @brief Base message queue class
*/

// Local includes

#include "Types.h"

namespace IRIS
{
// FWD
class Message;
class MessageSubscriber;

/**
  @class MessageQueue MessageQueue.hpp <MessageQueue.hpp>
  @brief Message queue class
*/
class MessageQueue
{
public:
	enum State { OK, NO, EMPTY };

	/**
	  @brief Add message to queue
	  @param pMessage - message object
	  @return Result state, one of OK, NO
	*/
	virtual State AddMessage(Message  * pMessage) = 0;

	/**
	  @brief Subscribe to the event
	  @param pSubscriber - Object-subscriber
	  @param iEventsMap - Id of event to subscribe
	  @return Result state, one of OK, NO
	*/
	virtual State Subscribe(MessageSubscriber  * pSubscriber,
	                         const UINT_64        iEventsMap) = 0;

	/**
	  @brief Remove top element and notify all subscribers
	  @return Result state, one of OK, EMPTY
	*/
	virtual MessageQueue::State Pop() = 0;

	/**
	  @brief Get queue length
	  @return elements in queue
	*/
	virtual UINT_64 GetQueueLength() const = 0;

	/**
	  @brief A virtual destructor
	*/
	virtual ~MessageQueue() throw();

private:

};

} // namespace IRIS
#endif // _MESSAGE_QUEUE_HPP__
// End.
