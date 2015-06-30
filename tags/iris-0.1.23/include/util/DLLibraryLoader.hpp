/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _DL_LIBRARY_LOADER_HPP__
#define _DL_LIBRARY_LOADER_HPP__ 1

/**
  @file DLLibraryLoader.hpp
  @brief Object loader
*/

#include "Types.h"
#include "STLMap.hpp"
#include "STLString.hpp"

#include <dlfcn.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

namespace IRIS
{

/**
  @class DLLibraryLoader DLLibraryLoader.hpp <DLLibraryLoader.hpp>
  @brief Object loader
*/
template<typename T>class Loader
{
public:
	/**
	  @brief Constructor
	  @param szILibraryPrefix - Library loader prefix
	*/
	Loader(CCHAR_P  szILibraryPrefix);

	/**
	  @brief Load library from disk
	  @param szLibraryName - path to the library
	*/
	INT_32 LoadLibrary(CCHAR_P szLibraryName);

	/**
	  @brief Get pointer to the object
	  @param szLibraryName - dynamic library file name
	  @param szClassName - class name
	  @return pointer to the object
	*/
	T * GetObject(CCHAR_P szLibraryName, CCHAR_P szClassName);

	/**
	  @brief Get object with type check
	  @param szLibraryName - dynamic library file name
	  @param szClassName - class name
	  @return pointer to the object
	*/
	template <typename TT> TT * GetCastedObject(CCHAR_P szLibraryName, CCHAR_P szClassName)
	{
		return dynamic_cast<TT *>(GetObject(szLibraryName, szClassName));
	}

	/**
	  @brief A destructor
	*/
	~Loader() throw();

private:
	Loader();
	Loader(const Loader<T>  & oRhs);
	Loader & operator=(const Loader<T>  & oRhs);

	typedef T * ((*InitPtr)());

	/**
	  @struct HandleInfo
	  @brief Dynamic-loaded library hadnle storage
	*/
	struct HandleInfo
	{
		/** DLL handle          */
		void        * handle;
		/** Number of references */
		UINT_32       refcount;
	};

	/** Library loader prefix               */
	STLW::string                         sLibraryPrefix;
	/** Library name-to-dl handle map       */
	STLW::map<STLW::string, HandleInfo>  mLibraryMap;

	/**
	  @brief Check library handle in cache
	  @param szLibraryName - library file name
	  @return 0 - if found, -1 - otherwise
	*/
	HandleInfo * CheckLibraryHandle(CCHAR_P szLibraryName);

	/**
	  @brief Load library
	  @param szLibraryName - library file name
	*/
	void * DoLoadLibrary(CCHAR_P szLibraryName);
};

//
// Constructor
//
template <typename T>Loader<T>::Loader(CCHAR_P szILibraryPrefix): sLibraryPrefix(szILibraryPrefix)
{
	;;
}

//
// Load library from disk
//
template <typename T>INT_32 Loader<T>::LoadLibrary(CCHAR_P szLibraryName)
{
	// Check library in cache
	HandleInfo * pHandleInfo = CheckLibraryHandle(szLibraryName);

	// Found library
	if (pHandleInfo != NULL) { return -1; }

	// Try to load library
	if (DoLoadLibrary(szLibraryName) == NULL) { return -1; }

	pHandleInfo = CheckLibraryHandle(szLibraryName);
	++(pHandleInfo -> refcount);

return 0;
}

//
// Get pointer to the object
//
template <typename T> T * Loader<T>::GetObject(CCHAR_P szLibraryName, CCHAR_P szClassName)
{
	// Check cache
	HandleInfo * pHandleInfo = CheckLibraryHandle(szLibraryName);
	// Load library
	if (pHandleInfo == NULL)
	{
		DoLoadLibrary(szLibraryName);
		pHandleInfo = CheckLibraryHandle(szLibraryName);
	}

	// Check error code
	if (pHandleInfo == NULL) { return NULL; }

	STLW::string sInitHandler(szClassName);
	sInitHandler.append(sLibraryPrefix);

	// Initialize class
#if defined(linux) || defined(sun) || defined(_AIX) || defined(__OpenBSD__) || defined(__NetBSD__)
	// Brain-damaged stupidity
	void * vPtr = dlsym(pHandleInfo -> handle, sInitHandler.c_str());
	InitPtr vVInitPtr;
	memcpy((void *)(&vVInitPtr), (void *)(&vPtr), sizeof(void *));
#else
	InitPtr vVInitPtr = (InitPtr)dlfunc(pHandleInfo -> handle, sInitHandler.c_str());
#endif

	if (vVInitPtr == NULL)
	{
		STLW::string sTMP("Cannot open library \"");
		sTMP.append(szLibraryName);
		sTMP.append("\": ");
		sTMP.append(dlerror());
		fprintf(stderr, "ERROR: %s\n", sTMP.data());
//		throw UnixException(sTMP.c_str(), errno);
		return NULL;
	}

	// Number of references
	++(pHandleInfo -> refcount);

	// Create object
	T * pObject = ((*vVInitPtr)());

return pObject;
}

//
// Check library handle in cache
//
template <typename T> typename Loader<T>::HandleInfo * Loader<T>::CheckLibraryHandle(CCHAR_P szLibraryName)
{
	typename STLW::map<STLW::string, HandleInfo>::iterator itmLibraryMap = mLibraryMap.find(szLibraryName);
	if (itmLibraryMap != mLibraryMap.end()) { return &(itmLibraryMap -> second); }

return NULL;
}

//
// Load library
//
template <typename T>void * Loader<T>::DoLoadLibrary(CCHAR_P szLibraryName)
{
	HandleInfo oHandleInfo;
	// Okay, try to load function
	oHandleInfo.handle   = dlopen(szLibraryName, RTLD_NOW | RTLD_GLOBAL);
	oHandleInfo.refcount = 0;
	// Error?
	if (oHandleInfo.handle == NULL)
	{
		STLW::string sTMP("Cannot open library \"");
		sTMP.append(szLibraryName);
		sTMP.append("\": ");
		sTMP.append(dlerror());
		fprintf(stderr, "ERROR: %s\n", sTMP.data());
//		throw UnixException(sTMP.c_str(), errno);
		return NULL;
	}

	mLibraryMap[szLibraryName] = oHandleInfo;

return oHandleInfo.handle;
}

//
// A destructor
//
template <typename T>Loader<T>::~Loader() throw()
{
#ifdef linux
	typename STLW::map<STLW::string, HandleInfo>::iterator itmLibraryMap = mLibraryMap.begin();
	while (itmLibraryMap != mLibraryMap.end())
	{
		for(UINT_32 iI = 0; iI < itmLibraryMap -> second.refcount; ++iI)
		{
			dlclose(itmLibraryMap -> second.handle);
		}

		++itmLibraryMap;
	}
#endif
}

} // namespace IRIS
#endif // _DL_LIBRARY_LOADER_HPP__
// End.
