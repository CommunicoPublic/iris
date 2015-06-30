/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _ACCEPT_LOOP_WORKER_HPP__
#define _ACCEPT_LOOP_WORKER_HPP__ 1

/**
  @file AcceptLoopWorker.hpp
  @brief Iris network thread which accepts connections of clients
*/

#include "Types.h"
#include "SSL.hpp"

namespace IRIS
{
// FWD
class AcceptLoopContext;
class Logger;

/**
  @class AcceptLoopWorker AcceptLoopWorker.hpp <AcceptLoopWorker.hpp>
  @brief Event thread worker
*/
class AcceptLoopWorker
{
public:
	/**
	  @brief Constructor
	*/
	AcceptLoopWorker(AcceptLoopContext  * pIAcceptLoopContext,
	                 const INT_32         iICPUNum = -1);

	/**
	  @brief Execute job
	*/
	void Run() throw();

	/**
	  @brief A destructor
	*/
	~AcceptLoopWorker() throw();
private:
	// Does not exist
	AcceptLoopWorker(const AcceptLoopWorker  & oRhs);
	AcceptLoopWorker& operator=(const AcceptLoopWorker  & oRhs);

	/** Thread context       */
	AcceptLoopContext  * pAcceptLoopContext;
	/** CPU Number           */
	const INT_32         iCPUNum;
};

} // namespace IRIS
#endif // _ACCEPT_LOOP_WORKER_HPP__
// End.
