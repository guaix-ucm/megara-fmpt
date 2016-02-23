#ifndef FMPT_TESTSCALARS_H
#define FMPT_TESTSCALARS_H

#include <cppunit/extensions/HelperMacros.h>

//build the class where define the test methods
class TestScalars: public CppUnit::TestFixture
{
    //establish the test suit of Test
    CPPUNIT_TEST_SUITE(TestScalars);

    //add test methods
    CPPUNIT_TEST(testFactorial);
    CPPUNIT_TEST(testBinomial);
    CPPUNIT_TEST(testLog);
    CPPUNIT_TEST(testLog2);
    CPPUNIT_TEST(testSign);
    CPPUNIT_TEST(testAbs);

    CPPUNIT_TEST(testMin);
    CPPUNIT_TEST(testMax);

    //finish the process
    CPPUNIT_TEST_SUITE_END();

public:
    //overide setUp(), init data, etc
    void setUp();
    //overide tearDown(), free allocated memory, etc
    void tearDown();

protected:
    void testFactorial();
    void testBinomial();
    void testLog();
    void testLog2();
    void testSign();
    void testAbs();

    void testMin();
    void testMax();
};

#endif // FMPT_TESTSCALARS_H
