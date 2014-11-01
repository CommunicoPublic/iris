/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _UNIT_TEST_HPP__
#define _UNIT_TEST_HPP__ 1

/**
  @file UnitTest.hpp
  @brief Unit tests
*/
#include <stdarg.h>

#include "Types.h"
#include "STLString.hpp"
#include "STLMap.hpp"

#define TEST_CLASS_NAME(TestCaseName, TestName) TestCaseName##TestName##Test

#define TEST_START(TestCaseName, TestName) \
class TEST_CLASS_NAME(TestCaseName, TestName): \
  public IRIS::BaseTest \
{ \
public: \
	TEST_CLASS_NAME(TestCaseName, TestName)(); \
	INT_32 RunTests(IRIS::BaseTestCollector  * pCollector); \
	~TEST_CLASS_NAME(TestCaseName, TestName)() throw(); \
private: \
	TEST_CLASS_NAME(TestCaseName, TestName)(const TEST_CLASS_NAME(TestCaseName, TestName)  & oRhs); \
	TEST_CLASS_NAME(TestCaseName, TestName)& operator=(const TEST_CLASS_NAME(TestCaseName, TestName)  & oRhs); \
}; \
TEST_CLASS_NAME(TestCaseName, TestName)::TEST_CLASS_NAME(TestCaseName, TestName)() { ;; } \
TEST_CLASS_NAME(TestCaseName, TestName)::~TEST_CLASS_NAME(TestCaseName, TestName)() throw() { ;; } \
int TEST_CLASS_NAME(TestCaseName, TestName)::RunTests(IRIS::BaseTestCollector  * pCollector)

#define EXPECT_EQ(Name, Expected, Given) { pCollector -> StartTest(#Name); pCollector -> EndTest(#Name, IRIS::BaseTestCollector::EQ, (Expected) == (Given)); }
#define EXPECT_TRUE(Name, Given)         { pCollector -> StartTest(#Name); pCollector -> EndTest(#Name, IRIS::BaseTestCollector::TRUE, Given); }
#define EXPECT_FALSE(Name, Given)        { pCollector -> StartTest(#Name); pCollector -> EndTest(#Name, IRIS::BaseTestCollector::FALSE, !(Given)); }
#define TEST_RUN(TestCaseName, TestName, TestCollector) TEST_CLASS_NAME(TestCaseName, TestName)().RunTests(TestCollector)
#define TEST_DEPENDS_ON(Name, ...) { AddTest(#Name,

namespace IRIS
{
/**
  @class BaseTestCollector UnitTest.hpp <UnitTest.hpp
  @brief collector resultwf Base class for unit tests
*/
class BaseTestCollector
{
public:
	enum Condition { NE, EQ, TRUE, FALSE };

	/**
	  @brief Start test
	  @param sTestName - test name
	*/
	virtual void StartTest(CCHAR_P sTestName) = 0;

	/**
	  @brief End test
	  @param sEventName - test name
	  @param oCondition - test condition
	  @param bResult - test result (true, if passed, false, if failed)
	*/
	virtual void EndTest(CCHAR_P sEventName, const Condition  oCondition, const bool bResult) = 0;

	/**
	  @brief A destructor
	*/
	virtual ~BaseTestCollector() throw();
};

/**
  @class BaseTest UnitTest.hpp <UnitTest.hpp>
  @brief Base class for unit tests
*/
class BaseTest
{
public:
	virtual void RunTests(BaseTestCollector  * pCollector) = 0;

	/**
	  @brief A destructor
	*/
	virtual ~BaseTest() throw();

protected:

	/**
	  @brief Add test to testsuite
	*/
	void AddTest(CCHAR_P szName, ...);

private:

	struct Test
	{
		bool                test_result;
		STLW::set<Test *>   deps;
	};
	STLW::set<Test *>    oTestSuite;
};


void BaseTest::AddTest(CCHAR_P szName, ...)
{
/*
	int i,val,greater;
	va_list vl;
	va_start(vl, szName);
greater=va_arg(vl,int);
	for (i=1;i<amount;i++)
	{
	    val=va_arg(vl,int);
	    greater=(greater>val)?greater:val;
  }
  va_end(vl);
  return greater; */
}
} // n
int HelloWorld(int i)
{
	fprintf(stderr, "HelloWorld\n");
return i;
}

TEST_START(FirstTest, HelloWorld)
{
	EXPECT_EQ(HelloWorld, 1, HelloWorld(1))
}

class TestCollector:
  public IRIS::BaseTestCollector
{
public:
	void StartTest(const char * sEventName)
	{
		fprintf(stdout, "%s: ", sEventName);
	}

	void EndTest(const char * sEventName, const IRIS::BaseTestCollector::eCondition, const bool bResult)
	{
		fprintf(stdout, "%s\n", bResult ? "OK":"FAILED");
	}
        virtual ~TestCollector() throw() { ;; }

private:
};

int main(void)
{
	TestCollector  oTestCollector;
	TEST_RUN(FirstTest, HelloWorld, &oTestCollector);
}
