/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _EXCEPTION_HPP__
#define _EXCEPTION_HPP__ 1

/**
  @file Exception.hpp
  @brief Exceptions
*/

#include "Types.h"
#include "STLException.hpp"

namespace IRIS
{

/**
  @class UnrecoverableException Exception.hpp <Exception.hpp>
  @brief Unrecoverable exception class
*/
class UnrecoverableException:
  public STLW::exception
{
public:
	/**
	  @brief Constructor
	  @param sIText - Human-readable error description
	*/
	UnrecoverableException(CCHAR_P sIText);

	/**
	  @brief Copy constructor
	  @param oRhs - object to copy
	*/
	UnrecoverableException(const UnrecoverableException  & oRhs);

	/**
	  @brief Copy operator=
	  @param oRhs - object to copy
	*/
	UnrecoverableException & operator=(const UnrecoverableException  & oRhs);

	/**
	  @brief Get description of the exception.
	  @return Human-readable error description
	*/
	virtual const char* what() const throw();

	/**
	  @brief A destructor
	*/
	virtual ~UnrecoverableException() throw();
private:
	/** Human-readable error description */
	CHAR_P   sText;
};

/**
  @class UnixException Exception.hpp <Exception.hpp>
  @brief Unix exception class
*/
class UnixException:
  public STLW::exception
{
public:
	/**
	  @brief Constructor
	  @param sIText - Human-readable error description
	  @param iIErrno -nix error code
	*/
	UnixException(CCHAR_P sIText, const INT_32 iIErrno);

	/**
	  @brief Copy constructor
	  @param oRhs - object to copy
	*/
	UnixException(const UnixException  & oRhs);

	/**
	  @brief Copy operator=
	  @param oRhs - object to copy
	*/
	UnixException & operator=(const UnixException  & oRhs);

	/**
	  @brief Get description of the exception.
	  @return Human-readable error description
	*/
	virtual const char* what() const throw();

	/**
	  @brief Get error code
	  @return Unix error code
	*/
	virtual INT_32 err_no() const throw();

	/**
	  @brief A destructor
	*/
	virtual ~UnixException() throw();
private:
	/** Human-readable error description */
	CHAR_P   sText;
	/** Error code                       */
	INT_32   iErrno;
};

} // namespace IRIS
#endif // _EXCEPTION_HPP__
// End.
