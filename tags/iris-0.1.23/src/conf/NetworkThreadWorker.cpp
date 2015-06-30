/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "NetworkThreadWorker.hpp"

namespace IRIS
{
//
// Constructor
//
NetworkThreadData::NetworkThreadData(Mutex            & oMutex,
                                     FileHandleCache  & oFileHandleCache,
                                     GlobalContext    & oGlobalContext): mutex(oMutex),
                                                                         file_cache(oFileHandleCache),
                                                                         global_context(oGlobalContext) { ;; }

} // namespace IRIS
// End.

