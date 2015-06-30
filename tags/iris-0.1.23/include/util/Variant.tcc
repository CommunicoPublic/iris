/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include <stdio.h>
#include <stdlib.h>

namespace IRIS
{

//
// Constructor
//
template <typename Compare>BasicVariant<Compare>::BasicVariant(const BasicVariant<Compare>::eBasicVariantType  & eIType): eType(eIType)
{
	;;
}

//
// Copy constructor
//
template <typename Compare>BasicVariant<Compare>::BasicVariant(const BasicVariant<Compare>  & oRhs)
{
	eType = oRhs.eType;

	switch(eType)
	{
		case INT:
			iValue = oRhs.iValue;
			break;
		case UINT:
			iValue = oRhs.iValue;
			break;
		case FLOAT:
			dValue = oRhs.dValue;
			break;
		case STRING:
			sValue = oRhs.sValue;
			break;
		case ARRAY:
			vValue = oRhs.vValue;
			break;
		case MAP:
			mValue = oRhs.mValue;
			break;
	}
}

//
// Copy operator
//
template <typename Compare> BasicVariant<Compare> & BasicVariant<Compare>::operator=(const BasicVariant  & oRhs)
{
	if (&oRhs == this) { return *this; }

	eType = oRhs.eType;

	{
		STLW::string  sTMPValue;
		STLW::vector<BasicVariant>  vTMPValue;
		STLW::map<STLW::string, BasicVariant, Compare>  mTMPValue;

		sValue.swap(sTMPValue);
		vValue.swap(vTMPValue);
		mValue.swap(mTMPValue);
	}

	switch(eType)
	{
		case INT:
			iValue = oRhs.iValue;
			break;
		case UINT:
			iValue = oRhs.iValue;
			break;
		case FLOAT:
			dValue = oRhs.dValue;
			break;
		case STRING:
			sValue = oRhs.sValue;
			break;
		case ARRAY:
			vValue = oRhs.vValue;
			break;
		case MAP:
			mValue = oRhs.mValue;
			break;
	}

return *this;
}

//
// Type cast constructor
//
template <typename Compare>BasicVariant<Compare>::BasicVariant(const INT_32  iIValue): eType(INT), iValue(iIValue)
{
	;;
}

//
// Type cast constructor
//
template <typename Compare>BasicVariant<Compare>::BasicVariant(const UINT_32  iIValue): eType(UINT), iValue(iIValue)
{
	;;
}

//
// Type cast constructor
//
template <typename Compare>BasicVariant<Compare>::BasicVariant(const INT_64  iIValue): eType(INT), iValue(iIValue)
{
	;;
}

//
// Type cast constructor
//
template <typename Compare>BasicVariant<Compare>::BasicVariant(const UINT_64  iIValue): eType(UINT), iValue(iIValue)
{
	;;
}

//
// Type cast constructor
//
template <typename Compare>BasicVariant<Compare>::BasicVariant(const W_FLOAT  dIValue): eType(FLOAT), dValue(dIValue)
{
	;;
}

//
// Type cast constructor
//
template <typename Compare>BasicVariant<Compare>::BasicVariant(const STLW::string & sIValue): eType(STRING), sValue(sIValue)
{
	;;
}

//
// Type cast operator=
//
template <typename Compare>BasicVariant<Compare> & BasicVariant<Compare>::operator=(const STLW::string   & sIValue)
{
	eType = STRING;
	sValue = sIValue;

return *this;
}

//
// Type cast operator=
//
template <typename Compare>BasicVariant<Compare> & BasicVariant<Compare>::operator=(const INT_32  iIValue)
{
	eType = INT;
	iValue = iIValue;

return *this;
}

//
// Type cast operator=
//
template <typename Compare>BasicVariant<Compare> & BasicVariant<Compare>::operator=(const UINT_32  iIValue)
{
	eType = UINT;
	iValue = iIValue;

return *this;
}

//
// Type cast operator=
//
template <typename Compare>BasicVariant<Compare> & BasicVariant<Compare>::operator=(const INT_64  iIValue)
{
	eType = INT;
	iValue = iIValue;

return *this;
}

//
// Type cast operator=
//
template <typename Compare>BasicVariant<Compare> & BasicVariant<Compare>::operator=(const UINT_64  iIValue)
{
	eType = UINT;
	iValue = iIValue;

return *this;
}

//
// Type cast operator=
//
template <typename Compare>BasicVariant<Compare> & BasicVariant<Compare>::operator=(const W_FLOAT  dIValue)
{
	eType = FLOAT;
	dValue = dIValue;

return *this;
}

//
// Access operator for ARRAY
//
template <typename Compare>BasicVariant<Compare> & BasicVariant<Compare>::operator[](const UINT_64 iPos)
{
	if (eType != ARRAY)
	{
		STLW::string  sTMPValue;
		STLW::vector<BasicVariant>  vTMPValue;
		STLW::map<STLW::string, BasicVariant<Compare>, Compare>  mTMPValue;

		sValue.swap(sTMPValue);
		vValue.swap(vTMPValue);
		mValue.swap(mTMPValue);

		eType = ARRAY;
	}

	const UINT_64 iSize = vValue.size();

	if      (iPos == iSize) { vValue.push_back(BasicVariant()); }
	else if (iPos > iSize)  { vValue.resize(iPos + 1); }

return vValue[iPos];
}

//
// Access operator for ARRAY, constant version
//
template <typename Compare>const BasicVariant<Compare> & BasicVariant<Compare>::operator[](const UINT_64 iPos) const
{
	const UINT_64 iSize = vValue.size();
	if (eType != ARRAY || iPos >= iSize) { return oDummy; }

return vValue[iPos];
}

//
// Access operator for MAP
//
template <typename Compare>BasicVariant<Compare> & BasicVariant<Compare>::operator[](const STLW::string & sKey)
{
	if (eType != MAP)
	{
		STLW::string  sTMPValue;
		STLW::vector<BasicVariant>  vTMPValue;
		STLW::map<STLW::string, BasicVariant<Compare>, Compare>  mTMPValue;

		sValue.swap(sTMPValue);
		vValue.swap(vTMPValue);
		mValue.swap(mTMPValue);

		eType = MAP;
	}

	typename STLW::map<STLW::string, BasicVariant<Compare>, Compare>::iterator itmValue = mValue.find(sKey);
	if (itmValue == mValue.end()) { return mValue[sKey]; }

return itmValue -> second;
}

//
// Access operator for MAP, constant version
//
template <typename Compare>const BasicVariant<Compare> & BasicVariant<Compare>::operator[](const STLW::string & sKey) const
{
	typename STLW::map<STLW::string, BasicVariant<Compare>, Compare>::const_iterator itmValue = mValue.find(sKey);
	if (itmValue == mValue.end()) { return oDummy; }

return itmValue -> second;
}

//
// Push element to the end of ARRAY
//
template <typename Compare>void BasicVariant<Compare>::PushBack(const BasicVariant  & oData)
{
	if (eType != ARRAY)
	{
		STLW::string  sTMPValue;
		STLW::vector<BasicVariant>  vTMPValue;
		STLW::map<STLW::string, BasicVariant<Compare>, Compare>  mTMPValue;

		sValue.swap(sTMPValue);
		vValue.swap(vTMPValue);
		mValue.swap(mTMPValue);

		eType = ARRAY;
	}

	vValue.push_back(oData);
}

//
// Push string to the end of ARRAY
//
template <typename Compare>void BasicVariant<Compare>::PushBack(const STLW::string  & sData)
{
	if (eType != ARRAY)
	{
		STLW::string  sTMPValue;
		STLW::vector<BasicVariant>  vTMPValue;
		STLW::map<STLW::string, BasicVariant<Compare>, Compare>  mTMPValue;

		sValue.swap(sTMPValue);
		vValue.swap(vTMPValue);
		mValue.swap(mTMPValue);

		eType = ARRAY;
	}

	vValue.push_back(sData);
}

//
// Check existence of record
//
template <typename Compare>bool BasicVariant<Compare>::Exists(const STLW::string & sKey) const
{
	if (eType != MAP) { return false; }

return (mValue.find(sKey) != mValue.end());
}

//
// Dump object to string
//
template <typename Compare>void BasicVariant<Compare>::Dump(VariantDumper  * pDumper) const
{
	switch(eType)
	{
		case INT:
			pDumper -> AddElement(iValue);
			break;
		case UINT:
			pDumper -> AddElement(UINT_64(iValue));
			break;
		case FLOAT:
			pDumper -> AddElement(dValue);
			break;
		case STRING:
			pDumper -> AddElement(sValue);
			break;
		case ARRAY:
			{
				pDumper -> StartArray(vValue.size());
				typename STLW::vector<BasicVariant>::const_iterator itvValue = vValue.begin();
				if (itvValue != vValue.end())
				{
					UINT_64 iPos = 0;
					const UINT_64 iLastPos = vValue.size() - 1;
					for (;;)
					{
						const bool bIslastElement = (iPos == iLastPos);
						pDumper -> StartElement(iPos, bIslastElement);
						itvValue -> Dump(pDumper);
						pDumper -> EndElement(bIslastElement);
						++iPos;
						++itvValue;

						if (itvValue == vValue.end()) { break; }
					}
				}
				pDumper -> EndArray();
			}
			break;
		case MAP:
			{
				pDumper -> StartMap(mValue.size());
				typename STLW::map<STLW::string, BasicVariant<Compare>, Compare>::const_iterator itmValue = mValue.begin();
				if (itmValue != mValue.end())
				{
					UINT_64 iPos = 0;
					const UINT_64 iLastPos = mValue.size() - 1;
					for (;;)
					{
						const bool bIslastElement = (iPos == iLastPos);
						pDumper -> StartPair(itmValue -> first, bIslastElement);
						itmValue -> second.Dump(pDumper);
						pDumper -> EndPair(bIslastElement);
						++iPos;
						++itmValue;

						if (itmValue == mValue.end()) { break; }
					}
				}
				pDumper -> EndMap();
			}
			break;
	}
}

//
// Get value as string
//
template <typename Compare>STLW::string BasicVariant<Compare>::Str() const
{
	switch(eType)
	{
		case INT:
			{
				CHAR_8 szBuffer[1024];
				snprintf(szBuffer, 1023, "%lli", (long long)(iValue));
				return szBuffer;
			}
			break;
		case UINT:
			{
				CHAR_8 szBuffer[1024];
				snprintf(szBuffer, 1023, "%llu", (long long unsigned)(iValue));
				return szBuffer;
			}
			break;
		case FLOAT:
			{
				CHAR_8 szBuffer[1024];
				snprintf(szBuffer, 1023, "%.*G", 12, dValue);
				return szBuffer;
			}
			break;
		case STRING:
			return sValue;
			break;
		case ARRAY:
			{
				CHAR_8 szBuffer[1024];
				snprintf(szBuffer, 1023, "ARRAY(%p)", (void*)&vValue);
				return szBuffer;
			}
			break;
		case MAP:
			{
				CHAR_8 szBuffer[1024];
				snprintf(szBuffer, 1023, "HASH(%p)", (void*)&mValue);
				return szBuffer;
			}
	}
return "";
}

//
// Get value as integer
//
template <typename Compare>INT_64 BasicVariant<Compare>::Int() const
{
	if (eType == INT)    { return iValue; }
	if (eType == UINT)   { return iValue; }
	if (eType == FLOAT)  { return dValue; }
	if (eType == STRING) { return atoll(sValue.c_str()); }
return 0;
}

//
// Get value as unsigned integer
//
template <typename Compare>UINT_64 BasicVariant<Compare>::Uint() const
{
	if (eType == INT)    { return UINT_64(iValue); }
	if (eType == UINT)   { return UINT_64(iValue); }
	if (eType == FLOAT)  { return dValue; }
	if (eType == STRING) { return atoll(sValue.c_str()); }
return 0;
}


//
// Get value as IEEE 764 value
//
template <typename Compare>W_FLOAT BasicVariant<Compare>::Float() const
{
	if (eType == INT)    { return iValue; }
	if (eType == UINT)   { return UINT_64(iValue); }
	if (eType == FLOAT)  { return dValue; }
	if (eType == STRING) { return atof(sValue.c_str()); }

return 0.0;
}

//
// Get object size, elements for ARRAY and MAP, bytes - for STRING
//
template <typename Compare>UINT_64 BasicVariant<Compare>::Size() const
{
	switch(eType)
	{
		case STRING:
			return sValue.size();
		case ARRAY:
			return vValue.size();
		case MAP:
			return mValue.size();
		default:
			return 0;
	}
return 0;
}

//
// Get type of object
//
template <typename Compare>typename BasicVariant<Compare>::eBasicVariantType BasicVariant<Compare>::Type() const { return eType; }

//
// Return iterator to beginning
//
template <typename Compare>typename BasicVariant<Compare>::Iterator  BasicVariant<Compare>::Begin() { return mValue.begin(); }

//
// Return iterator to end
//
template <typename Compare>typename BasicVariant<Compare>::Iterator  BasicVariant<Compare>::End() { return mValue.end(); }

//
// Return constant iterator to beginning
//
template <typename Compare>const typename BasicVariant<Compare>::ConstIterator  BasicVariant<Compare>::Begin() const { return mValue.begin(); }

//
// Return constant iterator to end
//
template <typename Compare>const typename BasicVariant<Compare>::ConstIterator  BasicVariant<Compare>::End() const { return mValue.end(); }

//
// Erase element from map
//
template <typename Compare>bool BasicVariant<Compare>::Erase(const STLW::string & sKey)
{
	if (eType != MAP) { return false; }

return (mValue.erase(sKey) != 0);
}

//
// Erase element from array
//
template <typename Compare>bool BasicVariant<Compare>::Erase(const UINT_64 iPos)
{
	if (eType != ARRAY || iPos >= vValue.size()) { return false; }

	vValue.erase(vValue.begin() + iPos);

return true;
}

//
// Find element in map
//
template <typename Compare> typename BasicVariant<Compare>::Iterator  BasicVariant<Compare>::Find(const STLW::string & sKey)
{
	if (eType != MAP)
	{
		STLW::string  sTMPValue;
		STLW::vector<BasicVariant>  vTMPValue;
		STLW::map<STLW::string, BasicVariant<Compare>, Compare>  mTMPValue;

		sValue.swap(sTMPValue);
		vValue.swap(vTMPValue);
		mValue.swap(mTMPValue);

		eType = MAP;
	}

return mValue.find(sKey);
}

//
// Find element in map
//
template <typename Compare> typename BasicVariant<Compare>::ConstIterator  BasicVariant<Compare>::Find(const STLW::string & sKey) const { return mValue.find(sKey); }

//
// Merge two BasicVariant objects
//
template <typename Compare>void BasicVariant<Compare>::Merge(const BasicVariant<Compare>  & oSrc)
{
	MergeRec(*this, oSrc);
}

//
// Get difference between two BasicVariant objects
//
template <typename Compare>BasicVariant<Compare> BasicVariant<Compare>::Diff(const BasicVariant<Compare>  & oSrc)
{
	BasicVariant<Compare> oResult;

	bool  bChanged = false;
	DiffRec(*this, oSrc, oResult, bChanged);

return oResult;
}

//
// Merge two BasicVariant objects
//
template <typename Compare>void BasicVariant<Compare>::MergeRec(BasicVariant<Compare>        & oDst,
                                                                const BasicVariant<Compare>  & oSrc)
{
	if      (oSrc.Type() == BasicVariant<Compare>::STRING)
	{
		oDst = oSrc;
	}
	else if (oSrc.Type() == BasicVariant<Compare>::ARRAY)
	{
		if (oDst.Type() != BasicVariant<Compare>::ARRAY) { oDst = oSrc; return; }

		for(UINT_64 iPos = 0; iPos < oSrc.Size(); ++iPos)
		{
			MergeRec(oDst[iPos], oSrc[iPos]);
		}
	}
	else if (oSrc.Type() == BasicVariant<Compare>::MAP)
	{
		if (oDst.Type() != BasicVariant<Compare>::MAP) { oDst = oSrc; return; }

		typename BasicVariant::ConstIterator itoSrc = oSrc.Begin();
		while (itoSrc != oSrc.End())
		{
			MergeRec(oDst[itoSrc -> first], itoSrc -> second);
			++itoSrc;
		}
	}
}

//
// Get difference between two BasicVariant objects
//
template <typename Compare>void BasicVariant<Compare>::DiffRec(const BasicVariant<Compare>  & oDst,
                                                               const BasicVariant<Compare>  & oSrc,
                                                               BasicVariant<Compare>        & oResult,
                                                               bool                         & bChanged)
{
	if      (oSrc.Type() == BasicVariant<Compare>::STRING)
	{
		if (oDst.Str() != oSrc.Str())
		{
			bChanged = true;
			oResult  = oSrc;
		}
		else
		{
			bChanged = false;
		}
	}
	else if (oSrc.Type() == BasicVariant<Compare>::ARRAY)
	{
		if (oDst.Type() != BasicVariant<Compare>::ARRAY)
		{
			oResult = oSrc;
			bChanged = true;
			return;
		}

		for(UINT_64 iPos = 0; iPos < oSrc.Size(); ++iPos)
		{
			bool    bTMPChanged = false;
			BasicVariant<Compare> oTMPResult;

			DiffRec(oDst[iPos], oSrc[iPos], oTMPResult, bTMPChanged);
			if (bTMPChanged)
			{
				bChanged = bTMPChanged;
				oResult.PushBack(oTMPResult);
			}
		}
	}
	else if (oSrc.Type() == BasicVariant<Compare>::MAP)
	{
		if (oDst.Type() != BasicVariant<Compare>::MAP)
		{
			oResult = oSrc;
			bChanged = true;
			return;
		}

		typename BasicVariant<Compare>::ConstIterator itoSrc = oSrc.Begin();
		while (itoSrc != oSrc.End())
		{
			bool    bTMPChanged = false;
			BasicVariant<Compare> oTMPResult;

			DiffRec(oDst[itoSrc -> first], itoSrc -> second, oTMPResult, bTMPChanged);
			if (bTMPChanged)
			{
				bChanged = bTMPChanged;
				oResult[itoSrc -> first] = oTMPResult;
			}

			++itoSrc;
		}
	}
}

//
// Destructor
//
template <typename Compare>BasicVariant<Compare>::~BasicVariant() throw()
{
	;;
}

template<typename Compare>BasicVariant<Compare> const BasicVariant<Compare>::oDummy = BasicVariant<Compare>();

} // namespace IRIS
// End.
