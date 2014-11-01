/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _STL_ALGORITHM_HPP__
#define _STL_ALGORITHM_HPP__ 1

/**
  @file STLAlgorithm.hpp
  @brief Wrapper for STL algorithm
*/
#include <algorithm>

#ifndef STLW
    #ifndef NO_STL_STD_NS_PREFIX
        #define STLW std
    #else
        #define STLW
    #endif // NO_STL_STD_NS_PREFIX
#endif // STLW

#endif // _STL_ALGORITHM_HPP__
// End.
