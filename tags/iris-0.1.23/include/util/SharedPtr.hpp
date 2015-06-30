#ifndef _SHARED_PTR_HPP__
#define _SHARED_PTR_HPP__ 1

#include "Types.h"

namespace IRIS
{
/**
  @class SharedPtr SharedPtr.hpp <SharedPtr.hpp>
  @brief Shared pointer implementation
*/
template <typename T>class SharedPtr
{
public:
	/**
	  @brief Constructor
	  @param pT - pointer to share
	*/
	SharedPtr(T  * pT)
	{
		pImpl = new _SharedPtr;
		pImpl -> refcount = 1;
		pImpl -> data     = pT;
	}

	/**
	  @brief Copy constructor
	  @param oRhs - object to copy
	*/
	SharedPtr(const SharedPtr<T>  & oRhs)
	{
		pImpl = oRhs.pImpl;
		Share();
	}

	/**
	  @brief Copy operator
	  @param oRhs - object to copy
	  @return reference to self
	*/
	SharedPtr& operator=(const SharedPtr<T>  & oRhs)
	{
		if (this == &oRhs) { return *this; }

		Unshare();
		pImpl = oRhs.pImpl;
		Share();

	return *this;
	}

	/**
	  @brief Get pointer to the object
	*/
	T * operator ->() { return pImpl -> data; }

	/**
	  @brief Dereference object
	*/
	T & operator*() { return *(pImpl -> data); }

	/**
	  @brief Type cast operator
	*/
	operator T*() { return pImpl -> data; }

	/**
	  @brief Get pointer to the object
	*/
	const T * operator ->() const { return pImpl -> data; }

	/**
	  @brief Dereference object
	*/
	const T & operator*() const { return *(pImpl -> data); }

	/**
	  @brief Type cast operator
	*/
	operator const T*() const { return pImpl -> data; }

	/**
	  @brief Type cast operator
	*/
	operator bool () { return pImpl -> data != NULL; }

	/**
	  @brief A destructor
	*/
	~SharedPtr() throw() { Unshare(); }

private:
	/**
	  @brief Shared pointer PIMPL
	*/
	struct _SharedPtr
	{
		/** Reference count */
		UINT_32   refcount;
		/** Object          	*/
		T       * data;
	};
	/** Pointer implementation */
	_SharedPtr  * pImpl;

	/**
	  @brief Share pointer
	*/
	void Share() { ++(pImpl -> refcount); }

	/**
	  @brief Unshare pointer
	*/
	void Unshare() throw()
	{
		--(pImpl -> refcount);
		if (pImpl -> refcount == 0)
		{
			delete pImpl -> data;
			delete pImpl;
		}
	}
};
} // namespace IRIS
#endif // _SHARED_PTR_HPP__
// End.
