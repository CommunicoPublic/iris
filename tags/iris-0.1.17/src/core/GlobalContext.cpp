/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "GlobalContext.hpp"
#include "LoadableObject.hpp"

namespace IRIS
{

//
// Constructor
//
GlobalContext::GlobalContext(): loader(C_INIT_SYM_PREFIX),
                                shutdown_type(NONE)
{
	;;
}

} // namespace IRIS
// End.
