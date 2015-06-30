/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "AcceptLoopContext.hpp"
#include "AcceptLoopWorker.hpp"
#include "GlobalContext.hpp"
#include "Logger.hpp"
#include "MainLoopContext.hpp"
#include "NetworkEventTimer.hpp"

#include <unistd.h>

#ifdef __FreeBSD__
#include <pthread_np.h>
#endif

#if (sun)
#include <sys/types.h>
#include <sys/processor.h>
#include <sys/procset.h>
#endif

namespace IRIS
{

//
// Constructor
//
AcceptLoopWorker::AcceptLoopWorker(AcceptLoopContext  * pIAcceptLoopContext,
                                   const INT_32         iICPUNum): pAcceptLoopContext(pIAcceptLoopContext),
                                                                   iCPUNum(iICPUNum) { ;; }

//
// Execute job
//
void AcceptLoopWorker::Run() throw()
{
#ifdef IRIS_TLS_SUPPORT
	pAcceptLoopContext -> SetupSSL();
#endif // IRIS_TLS_SUPPORT

	GlobalContext & oGlobalContext = pAcceptLoopContext -> GetMainLoopContext().global_context;

	if (iCPUNum < 0)
	{
		oGlobalContext.error_log -> Info("Network thread started");
	}
	else
	{
		oGlobalContext.error_log -> Info("Network thread started. Binding to CPU#%d", iCPUNum);

#if (linux || __FreeBSD__ || __NetBSD__ || sun)
		const INT_32 iMaxCores = sysconf(_SC_NPROCESSORS_ONLN);
		if (iCPUNum > iMaxCores)
		{
			oGlobalContext.error_log -> Emerg("Only %d CPU(s) available. Can't bind to CPU number %d", iMaxCores, iCPUNum);
			return;
		}
#if !(sun)
		pthread_t oCurrentThread = pthread_self();
#endif

#if (sun)
		processorid_t iCPUId = iCPUNum;
		INT_32 iRC = processor_bind(P_LWPID, P_MYID, iCPUId, NULL);
		if (iRC < 0)
		{
			 oGlobalContext.error_log -> Warning("Can't bind to CPU#%d: error is %d", iCPUNum, errno);
		}
#elif(__NetBSD__)
		cpuset_t  * pCPUSet = cpuset_create();
		if (pCPUSet == NULL)
		{
			oGlobalContext.error_log -> Emerg("Can't create CPU set");
			return;
		}
		cpuset_zero(pCPUSet);
		cpuid_t iCPUId = iCPUNum;
		cpuset_set(iCPUId, pCPUSet);
		const INT_32 iRC = pthread_setaffinity_np(oCurrentThread, cpuset_size(pCPUSet), pCPUSet);
		cpuset_destroy(pCPUSet);
		if (iRC != 0)
		{
			if (errno != EPERM)
			{
				oGlobalContext.error_log -> Warning("Can't bind to CPU#%d: error is %d", iCPUNum, errno);
			}
			else
			{
				oGlobalContext.error_log -> Warning("Can't bind to CPU#%d: Permission denied. You should enable this operation by setting sysctl variable security.models.extensions.user_set_cpu_affinity to 1", iCPUNum);
			}
		}
#else
#if(__FreeBSD__)
		cpuset_t  oCPUSet;
#elif(linux) // __FreeBSD__
		cpu_set_t oCPUSet;
#endif // linux
		CPU_ZERO(&oCPUSet);
		CPU_SET(iCPUNum, &oCPUSet);
		const INT_32 iRC = pthread_setaffinity_np(oCurrentThread, sizeof(oCPUSet), &oCPUSet);
		if (iRC != 0)
		{
			oGlobalContext.error_log -> Warning("Can't bind to CPU#%d", iCPUNum);
		}
#endif // linux || __FreeBSD__
#endif
	}

	// Start loop
	pAcceptLoopContext -> GetEventLoop().Loop();

	// All done
	oGlobalContext.error_log -> Info("Network thread stopped");
}

//
// Destructor
//
AcceptLoopWorker::~AcceptLoopWorker() throw()
{
	delete pAcceptLoopContext;
}

} // namespace IRIS
// End.
