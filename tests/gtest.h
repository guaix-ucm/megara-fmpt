#ifndef FMPT_GTEST_H
#define FMPT_GTEST_H

#include <cppunit/extensions/HelperMacros.h>

class GTest: public CppUnit::TestFixture
{
  //establish the test suit of GTest
  CPPUNIT_TEST_SUITE(GTest);
  //add test method testSimple
  CPPUNIT_TEST(testSimple);
  //finish the process
  CPPUNIT_TEST_SUITE_END();

public:
  //overide setUp(), init data etc
  void setUp();
  //overide tearDown(), free allocated memory, etc
  void tearDown();

protected:
  //test method testSimple
  void testSimple();

};



#endif // FMPT_GTEST_H
