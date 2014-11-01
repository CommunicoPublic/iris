/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _STL_EXCEPTION_HPP__
#define _STL_EXCEPTION_HPP__ 1

/**
  @file STLException.hpp
  @brief Wrapper for STL exceptions
*/
#include <exception>
#include <stdexcept>

#ifndef STLW
    #ifndef NO_STL_STD_NS_PREFIX
        #define STLW std
    #else
        #define STLW
    #endif // NO_STL_STD_NS_PREFIX
#endif // STLW

#endif // _STL_EXCEPTION_HPP__
// End.
