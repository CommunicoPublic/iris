/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */


/**
  @file AbstractFactory.tcc
  @brief Abstract factory class implementation
*/

namespace IRIS
{

//
// Constructor
//
template<typename KeyType,
         typename BaseType,
         typename Compare>AbstractFactory<KeyType, BaseType, Compare>::AbstractFactory()
{
	;;
}

//
// Add object to the factory
//
template<typename KeyType,
         typename BaseType,
         typename Compare>BaseType * AbstractFactory<KeyType, BaseType, Compare>::AddObject(const KeyType  & sObjectName,
                                                                                            BaseType       * pObject)
{
	typename STLW::map<KeyType, BaseType *, Compare>::iterator itmFactory = mFactory.find(sObjectName);
	if (itmFactory != mFactory.end()) { return NULL; }

	mFactory[sObjectName] = pObject;
return pObject;
}

//
// Get object from factory
//
template<typename KeyType,
         typename BaseType,
         typename Compare>BaseType * AbstractFactory<KeyType, BaseType, Compare>::GetObject(const KeyType  & sObjectName)
{
	typename STLW::map<KeyType, BaseType *, Compare>::iterator itmFactory = mFactory.find(sObjectName);
	if (itmFactory == mFactory.end()) { return NULL; }

return itmFactory -> second;
}

//
// A destructor
//
template<typename KeyType,
         typename BaseType,
         typename Compare>AbstractFactory<KeyType, BaseType, Compare>::~AbstractFactory() throw()
{
	typename STLW::map<KeyType, BaseType *, Compare>::iterator itmFactory = mFactory.begin();
	while(itmFactory != mFactory.end())
	{
		delete itmFactory -> second;
		++itmFactory;
	}
}

} // namespace IRIS
// End.
