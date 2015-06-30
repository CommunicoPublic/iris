/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _LOADABLE_OBJECT_HPP__
#define _LOADABLE_OBJECT_HPP__ 1

/**
  @file LoadableObject.hpp
  @brief Iris server loadable object
*/
#include "Object.hpp"

/**
  @brief Magic string
*/
#define C_INIT_SYM_PREFIX "_module_init"

/**
  @brief Export a class
*/
#define EXPORT_HANDLER(T) extern "C" { IRIS::Object * T##_module_init(void) { try { return new T(); } catch (...) { return NULL; } return NULL; } }

#endif // _LOADABLE_OBJECT_HPP__
// End.
