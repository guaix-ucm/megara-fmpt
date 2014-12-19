#ifndef FMPT_GTEST_H
#define FMPT_GTEST_H

#include <cppunit/extensions/HelperMacros.h>

class GTest: public CppUnit::TestFixture
{
 CPPUNIT_TEST_SUITE(GTest);
 CPPUNIT_TEST(testSimple);
 CPPUNIT_TEST_SUITE_END();

public:
  void setUp();

  void tearDown();

  void testSimple();

};



#endif // FMPT_GTEST_H
