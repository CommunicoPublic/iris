/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _ABSTRACT_FACTORY_HPP__
#define _ABSTRACT_FACTORY_HPP__ 1

/**
  @file AbstractFactory.hpp
  @brief Abstract factory class
*/

// Local includes
#include "Variant.hpp"

namespace IRIS
{
/**
  @class AbstractFactory AbstractFactory.hpp <AbstractFactory.hpp>
  @brief Abstract factory class
*/
template<typename KeyType,
         typename BaseType,
         typename Compare = STLW::less<KeyType> >class AbstractFactory
{
public:
	/**
	  @brief Constructor
	*/
	AbstractFactory();

	/**
	  @brief Add object to the factory
	  @param sObjectName - object name
	  @param pObject - object to add
	  @return Result of operation (NULL - if duplicate found, pObject - if success)
	*/
	BaseType * AddObject(const KeyType  & sObjectName,
	                     BaseType       * pObject);

	/**
	  @brief Get object from factory
	  @param sObjectName - object name
	  @return Result of operation
	*/
	BaseType * GetObject(const KeyType  & sObjectName);

	/**
	  @brief A destructor
	*/
	~AbstractFactory() throw();
private:
	AbstractFactory(const AbstractFactory  & oRhs);
	AbstractFactory& operator=(const AbstractFactory  & oRhs);

	/** Data  */
	STLW::map<KeyType, BaseType *, Compare>   mFactory;
};

} // namespace IRIS

#include "AbstractFactory.tcc"

#endif // _ABSTRACT_FACTORY_HPP__
// End.
