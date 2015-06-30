/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _OBJECT_FACTORY_HPP__
#define _OBJECT_FACTORY_HPP__ 1

/**
  @file ObjectFactory.hpp
  @brief Object factory class
*/

#include "AbstractFactory.hpp"
#include "DLLibraryLoader.hpp"
#include "Object.hpp"

namespace IRIS
{

typedef Loader<Object>                                    ObjectLoader;
typedef AbstractFactory<STLW::string, Object, NoCaseCmp>  ObjectFactory;

} // namespace IRIS
#endif // _OBJECT_FACTORY_HPP__
// End.
