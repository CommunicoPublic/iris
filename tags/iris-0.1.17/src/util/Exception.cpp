/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "Exception.hpp"

#include <stdlib.h>
#include <string.h>

namespace IRIS
{

//
// Constructor
//
UnrecoverableException::UnrecoverableException(CCHAR_P sIText): sText(strdup(sIText)) { ;; }

//
// Copy constructor
//
UnrecoverableException::UnrecoverableException(const UnrecoverableException & oRhs): sText(strdup(oRhs.sText)) { ;; }

//
// Copy constructor
//
UnrecoverableException & UnrecoverableException::operator=(const UnrecoverableException & oRhs)
{
	if (this != &oRhs) { sText = strdup(oRhs.sText); }

return *this;
}

//
// Get description of the exception.
//
const char * UnrecoverableException::what() const throw() { return sText; }

//
// Destructor
//
UnrecoverableException::~UnrecoverableException() throw() { free(sText); }

//
// Copy constructor
//
UnixException::UnixException(const UnixException & oRhs): sText(strdup(oRhs.sText)), iErrno(oRhs.iErrno) { ;; }

//
// Copy constructor
//
UnixException & UnixException::operator=(const UnixException & oRhs)
{
	if (this != &oRhs)
	{
		sText = strdup(oRhs.sText);
		iErrno = oRhs.iErrno;
	}

return *this;
}

//
// Constructor
//
UnixException::UnixException(CCHAR_P sIText, const INT_32 iIErrno): sText(strdup(sIText)), iErrno(iIErrno) { ;; }

//
// Get description of the exception.
//
const char * UnixException::what() const throw() { return sText; }

//
// Get error code
//
INT_32 UnixException::err_no() const throw() { return iErrno; }

//
// Destructor
//
UnixException::~UnixException() throw() { free(sText); }

} // namespace IRIS
// End.
