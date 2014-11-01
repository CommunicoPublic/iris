/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _STL_FUNCTIONAL_HPP__
#define _STL_FUNCTIONAL_HPP__ 1

/**
  @file STLFunctional.hpp
  @brief Wrapper for STL algorithms
*/
#include <functional>
#include <algorithm>

#ifndef STLW
    #ifndef NO_STL_STD_NS_PREFIX
        #define STLW std
    #else
        #define STLW
    #endif // NO_STL_STD_NS_PREFIX
#endif // STLW

#endif // _STL_FUNCTIONAL_HPP__
// End.
