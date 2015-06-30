/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _PROCESS_HPP__
#define _PROCESS_HPP__ 1

/**
  @file Allocator.hpp
  @brief Base abstract class for memory pools
*/
#include "Types.h"

namespace IRIS
{
/**
  @class Allocator Allocator.hpp <Allocator.hpp>
  @brief Base class for memory pools
*/
class ProcPipe
{
public:
	/**
	  @brief Constructor
	*/
	ProcPipe();

	/**
	  @brief Open pipe
	*/
	INT_32 Open(CCHAR_P  szProgram);

	/**
	  @brief Write data to stdout stream
	*/
	INT_64 Write(const void    * vData,
	             const INT_64    iSize);

	/**
	  @brief Read data from stdin stream
	*/
	INT_64 Read(void          * vData,
	            const INT_64    iSize);

	/**
	  @brief Read data from stderr stream
	*/
	INT_64 ReadErr(void          * vData,
	               const INT_64    iSize);

	/**
	  @brief Close pipe
	*/
	INT_32 Close();

	/**
	  @brief Destructor
	*/
	~ProcPipe() throw();
private:
	ProcPipe(const ProcPipe & oRhs);
	ProcPipe& operator=(const ProcPipe & oRhs);

	/** Standard input stream  */
	INT_32   aIn[2];
	/** Standard output stream */
	INT_32   aOut[2];
	/** Standard error stream  */
	INT_32   aErr[2];
	/** Child pid              */
	pid_t    iPid;

	/**
	  @brief Initialize fds
	*/
	void InitFD();

	/**
	  @brief Close fds
	*/
	void CloseFD();
};

} // namespace IRIS
#endif // _PROCESS_HPP__
// End.
