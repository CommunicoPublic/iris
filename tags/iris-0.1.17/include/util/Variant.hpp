/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _VARIANT_HPP__
#define _VARIANT_HPP__ 1

/**
  @file Variant.hpp
  @brief Variant type
*/

// Local includes
#include "STLMap.hpp"
#include "STLSstream.hpp"
#include "STLVector.hpp"
#include "VariantDumper.hpp"

namespace IRIS
{
// FWD
class VariantDumper;

/**
  @class BasicVariant BasicVariant.hpp <BasicVariant.hpp>
  @brief variant class for configuration, etc
*/
template<typename Compare = STLW::less<STLW::string> >class BasicVariant
{
public:
	/** @enum eBasicVariantType - type of object */
	enum eBasicVariantType { INT, FLOAT, STRING, ARRAY, MAP };

	/**
	  @brief Constructor
	  @param eType - type of value, one of [STRING, ARRAY, MAP], default - STRING
	*/
	BasicVariant(const BasicVariant<Compare>::eBasicVariantType  & eType = STRING);

	/**
	  @brief Copy constructor
	  @param oRhs - object to copy
	*/
	BasicVariant(const BasicVariant<Compare>  & oRhs);

	/**
	  @brief Copy operator
	  @param oRhs - object to copy
	  @return reference to self
	*/
	BasicVariant<Compare> &operator=(const BasicVariant<Compare>  & oRhs);

	/**
	  @brief Type cast constructor
	  @param iValue - integer value
	*/
	BasicVariant(const INT_32  iValue);

	/**
	  @brief Type cast constructor
	  @param iValue - integer value
	*/
	BasicVariant(const UINT_32  iValue);

	/**
	  @brief Type cast constructor
	  @param iValue - integer value
	*/
	BasicVariant(const INT_64  iValue);

	/**
	  @brief Type cast constructor
	  @param iValue - integer value
	*/
	BasicVariant(const UINT_64  iValue);

	/**
	  @brief Type cast constructor
	  @param dValue - IEEE 754 value
	*/
	BasicVariant(const W_FLOAT  dValue);

	/**
	  @brief Type cast constructor
	  @param sValue - string
	*/
	BasicVariant(const STLW::string  & sValue);

	/**
	  @brief Cast to specified type
	*/
	template <typename T> BasicVariant(const STLW::vector<T> & vData)
	{
		{
			STLW::string  sTMPValue;
			STLW::vector<BasicVariant>  vTMPValue;
			STLW::map<STLW::string, BasicVariant, Compare>  mTMPValue;

			sValue.swap(sTMPValue);
			vValue.swap(vTMPValue);
			mValue.swap(mTMPValue);
		}

		eType = ARRAY;
		vValue.reserve(vData.size());
		typename STLW::vector<T>::const_iterator itvData = vData.begin();
		while (itvData != vData.end())
		{
			vValue.push_back(BasicVariant<Compare>(*itvData));
			++itvData;
		}
	}

	/**
	  @brief Cast to specified type
	*/
	template <typename T> BasicVariant(const STLW::map<STLW::string, T> & mData)
	{
		{
			STLW::string  sTMPValue;
			STLW::vector<BasicVariant>  vTMPValue;
			STLW::map<STLW::string, BasicVariant, Compare>  mTMPValue;

			sValue.swap(sTMPValue);
			vValue.swap(vTMPValue);
			mValue.swap(mTMPValue);
		}

		eType = MAP;
		typename STLW::map<STLW::string, T>::const_iterator itmData = mData.begin();
		while (itmData != mData.end())
		{
			mValue[itmData -> first] = BasicVariant<Compare>(itmData -> second);
			++itmData;
		}
	}

	/**
	  @brief Type cast operator=
	  @param sValue - string
	  @return reference to self
	*/
	BasicVariant<Compare> &operator=(const STLW::string   & sValue);

	/**
	  @brief Type cast operator=
	  @param iValue - 32-bit integer
	  @return reference to self
	*/
	BasicVariant<Compare> &operator=(const INT_32  iValue);

	/**
	  @brief Type cast operator=
	  @param iValue - unsigned 32-bit integer
	  @return reference to self
	*/
	BasicVariant<Compare> &operator=(const UINT_32  iValue);

	/**
	  @brief Type cast operator=
	  @param iValue - 64-bit integer
	  @return reference to self
	*/
	BasicVariant<Compare> &operator=(const INT_64  iValue);

	/**
	  @brief Type cast operator=
	  @param iValue - unsigned 64-bit integer
	  @return reference to self
	*/
	BasicVariant<Compare> &operator=(const UINT_64  iValue);

	/**
	  @brief Type cast operator=
	  @param dValue - double
	  @return reference to self
	*/
	BasicVariant<Compare> &operator=(const W_FLOAT  dValue);

	/**
	  @brief Cast to specified type
	*/
	template <typename T> BasicVariant<Compare> & operator=(const STLW::vector<T> & vData)
	{
		{
			STLW::string  sTMPValue;
			STLW::vector<BasicVariant>  vTMPValue;
			STLW::map<STLW::string, BasicVariant, Compare>  mTMPValue;

			sValue.swap(sTMPValue);
			vValue.swap(vTMPValue);
			mValue.swap(mTMPValue);
		}

		eType = ARRAY;
		vValue.reserve(vData.size());
		typename STLW::vector<T>::const_iterator itvData = vData.begin();
		while (itvData != vData.end())
		{
			vValue.push_back(BasicVariant<Compare>(*itvData));
			++itvData;
		}

	return *this;
	}

	/**
	  @brief Cast to specified type
	*/
	template <typename T> BasicVariant<Compare> & operator=(const STLW::map<STLW::string, T> & mData)
	{
		{
			STLW::string  sTMPValue;
			STLW::vector<BasicVariant>  vTMPValue;
			STLW::map<STLW::string, BasicVariant, Compare>  mTMPValue;

			sValue.swap(sTMPValue);
			vValue.swap(vTMPValue);
			mValue.swap(mTMPValue);
		}

		eType = MAP;
		typename STLW::map<STLW::string, T>::const_iterator itmData = mData.begin();
		while (itmData != mData.end())
		{
			mValue[itmData -> first] = BasicVariant<Compare>(T(itmData -> second));
			++itmData;
		}

	return *this;
	}

	/**
	  @brief Access operator for ARRAY
	  @param iPos - Position of object
	  @return reference to object
	*/
	BasicVariant & operator[](const UINT_64 iPos);

	/**
	  @brief Access operator for ARRAY
	  @param iPos - Position of object
	  @return reference to object
	*/
	BasicVariant & operator[](const INT_32 iPos) { return operator[](UINT_64(iPos)); }

	/**
	  @brief Access operator for ARRAY
	  @param iPos - Position of object
	  @return reference to object
	*/
	BasicVariant & operator[](const UINT_32 iPos) { return operator[](UINT_64(iPos)); }

	/**
	  @brief Access operator for ARRAY
	  @param iPos - Position of object
	  @return reference to object
	*/
	BasicVariant & operator[](const INT_64 iPos) { return operator[](UINT_64(iPos)); }

	/**
	  @brief Access operator for ARRAY, constant version
	  @param iPos - Position of object
	  @return reference to object
	*/
	const BasicVariant & operator[](const UINT_64 iPos) const;

	/**
	  @brief Access operator for ARRAY, constant version
	  @param iPos - Position of object
	  @return reference to object
	*/
	const BasicVariant & operator[](const INT_32 iPos) const { return operator[](UINT_64(iPos)); }

	/**
	  @brief Access operator for ARRAY, constant version
	  @param iPos - Position of object
	  @return reference to object
	*/
	const BasicVariant & operator[](const UINT_32 iPos) const { return operator[](UINT_64(iPos)); }

	/**
	  @brief Access operator for ARRAY, constant version
	  @param iPos - Position of object
	  @return reference to object
	*/
	const BasicVariant & operator[](const INT_64 iPos) const { return operator[](UINT_64(iPos)); }

	/**
	  @brief Access operator for MAP
	  @param sKey - Map key
	  @return reference to object
	*/
	BasicVariant & operator[](const STLW::string & sKey);

	/**
	  @brief Access operator for MAP
	  @param sKey - Map key
	  @return reference to object
	*/
	BasicVariant & operator[](CCHAR_P sKey) { return operator[](STLW::string(sKey)); }

	/**
	  @brief Access operator for MAP, constant version
	  @param sKey - Map key
	  @return reference to object
	*/
	const BasicVariant & operator[](const STLW::string & sKey) const;

	/**
	  @brief Access operator for MAP, constant version
	  @param sKey - Map key
	  @return reference to object
	*/
	const BasicVariant & operator[](CCHAR_P sKey) const { return operator[](STLW::string(sKey)); }

	/**
	  @brief Push element to the end of ARRAY
	*/
	void PushBack(const BasicVariant<Compare>  & oRhs);

	/**
	  @brief Push element to the end of ARRAY
	*/
	void PushBack(const STLW::string  & sData);

	/**
	  @brief Check existence of record
	*/
	bool Exists(const STLW::string & sKey) const;

	/**
	  @brief Get value as string
	*/
	STLW::string Str() const;

	/**
	  @brief Cast to string
	*/
	operator STLW::string() const { return Str(); }

	/**
	  @brief Get value as integer
	*/
	INT_64 Int() const;

	/**
	  @brief Cast to integer
	*/
	operator INT_32() const { return Int(); }

	/**
	  @brief Cast to integer
	*/
	operator UINT_32() const { return Int(); }

	/**
	  @brief Cast to integer
	*/
	operator INT_64() const { return Int(); }

	/**
	  @brief Cast to integer
	*/
	operator UINT_64() const { return Int(); }

	/**
	  @brief Cast to specified type
	*/
	template <typename T> operator STLW::vector<T>() const
	{
		STLW::vector<T> vData;
		if      (eType == STRING) { vData.push_back( this -> operator T() ); }
		else if (eType == ARRAY)
		{
			typename STLW::vector<BasicVariant<Compare> >::const_iterator itvValue = vValue.begin();
			while (itvValue != vValue.end())
			{
				vData.push_back( itvValue -> operator T() );
				++itvValue;
			}
		}
	return vData;
	}

	/**
	  @brief Cast to specified type
	*/
	template <typename T> operator STLW::map<STLW::string, T>() const
	{
		STLW::map<STLW::string, T> mData;
		if (eType == MAP)
		{
			typename STLW::map<STLW::string, BasicVariant<Compare> >::const_iterator itmValue = mValue.begin();
			while (itmValue != mValue.end())
			{
				mData[itmValue -> first] = itmValue -> second.operator T();
				++itmValue;
			}
		}
	return mData;
	}

	/**
	  @brief Get value as IEEE 764 value
	*/
	W_FLOAT Float() const;

	/**
	  @brief Cast to IEEE 764
	*/
	operator W_FLOAT() const { return Float(); };

	/**
	  @brief Get object size, elements for ARRAY and MAP, bytes - for STRING
	*/
	UINT_64 Size() const;

	/**
	  @brief Get type of object
	*/
	eBasicVariantType Type() const;

	/**
	  @brief Dump object to string
	*/
	void Dump(VariantDumper * pDumper) const;

	typedef typename STLW::map<STLW::string, BasicVariant<Compare>, Compare>::iterator Iterator;
	typedef typename STLW::map<STLW::string, BasicVariant<Compare>, Compare>::const_iterator ConstIterator;

	/**
	  @brief Return iterator to beginning
	  @return Iterator to beginning
	*/
	Iterator  Begin();

	/**
	  @brief Return iterator to end
	  @return Iterator to end
	*/
	Iterator  End();

	/**
	  @brief Return constant iterator to beginning
	  @return Constant iterator to beginning
	*/
	const ConstIterator  Begin() const;

	/**
	  @brief Return constant iterator to end
	  @return Constant iterator to end
	*/
	const ConstIterator  End() const;

	/**
	  @brief Erase element from map
	  @brief sKey - key to find
	  @return true, if element was found, false - otherwise
	*/
	bool Erase(const STLW::string & sKey);

	/**
	  @brief Erase element from array
	  @param iPos - Position of object
	  @return true, if element was found, false - otherwise
	*/
	bool Erase(const UINT_64 iPos);

	/**
	  @brief Find element in map
	  @brief sKey - key to find
	  @return Iterator pointed to element, or end, if not found
	*/
	Iterator  Find(const STLW::string & sKey);

	/**
	  @brief Find element in map
	  @brief sKey - key to find
	  @return Iterator pointed to element, or end, if not found
	*/
	ConstIterator  Find(const STLW::string & sKey) const;

	/**
	  @brief Merge two BasicVariant objects
	  @param oSrc - source object
	*/
	void Merge(const BasicVariant  & oSrc);

	/**
	  @brief Get difference between two BasicVariant objects
	  @param oSrc - source object
	*/
	BasicVariant Diff(const BasicVariant  & oSrc);

	/**
	  @brief Destructor
	*/
	~BasicVariant() throw();
public:
	/** Type of value  */
	eBasicVariantType                     eType;
	/** Integer value  */
	INT_64                                iValue;
	/** IEEE 754 value */
	W_FLOAT                               dValue;
	/** String value   */
	STLW::string                          sValue;
	/** Array value    */
	STLW::vector<BasicVariant<Compare> >  vValue;
	/** Subtree value  */
	STLW::map<STLW::string, BasicVariant<Compare>, Compare> mValue;

	/** Return empty object */
	static const BasicVariant<Compare> oDummy;

	/**
	  @brief Merge two BasicVariant objects
	  @param oDst - destination object
	  @param oSrc - source object
	*/
	static void MergeRec(BasicVariant<Compare>        & oDst,
	                     const BasicVariant<Compare>  & oSrc);

	/**
	  @brief Get difference between two BasicVariant objects
	  @param oDst - destination object
	  @param oSrc - source object
	  @param oResult - diff result object
	  @param bChanged - true, if oResult contains changes
	*/
	static void DiffRec(const BasicVariant<Compare>  & oDst,
	                    const BasicVariant<Compare>  & oSrc,
	                    BasicVariant<Compare>        & oResult,
	                    bool                    & bChanged);
};

typedef BasicVariant<STLW::less<STLW::string> >  Variant;

struct NoCaseCmp
{
	bool operator()(const STLW::string oX, const STLW::string & oY) const;
};

typedef BasicVariant<NoCaseCmp>  VariantNC;

} // namespace IRIS

#include "Variant.tcc"

#endif // _VARIANT_HPP__
// End.
