/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "LoadableObject.hpp"

#include <stdio.h>

namespace TEST
{

/**
  @class TestLibrary TestLibrary.cpp <TestLibrary.cpp>
  @brief Loadable object
*/
class TestLibrary:
  public IRIS::Object
{
public:
	/**
	  @brief A constructor
	*/
	TestLibrary();

	/**
	  @brief Get object type
	  @return Human-readable type of object
	*/
	CCHAR_P GetObjectType() const;

	/**
	  @brief Get object name
	  @return Human-readable name of object
	*/
	CCHAR_P GetObjectName() const;

	/**
	  @brief A destructor
	*/
	~TestLibrary() throw();
};

EXPORT_HANDLER(TestLibrary)

//
// A constructor
//
TestLibrary::TestLibrary()
{
	fprintf(stdout, "TestLibrary::TestLibrary()\n");
}

//
// Get object type
//
CCHAR_P TestLibrary::GetObjectType() const
{
	fprintf(stdout, "TestLibrary::GetObjectType()\n");
return "TestLibrary";
}

//
// Get object name
//
CCHAR_P TestLibrary::GetObjectName() const
{
	fprintf(stdout, "TestLibrary::GetObjectName()\n");
return "TestLibrary";
}

//
// A virtual destructor
TestLibrary::~TestLibrary() throw()
{
	fprintf(stdout, "TestLibrary::~TestLibrary()\n");
}

} // namespace TEST
// End.
