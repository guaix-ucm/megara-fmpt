
#include "testScalars.h"

#include "../src/Scalars.h"

using namespace Mathematics;

CPPUNIT_TEST_SUITE_REGISTRATION(TestScalars);

//overide setUp(), init data, etc
void TestScalars::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestScalars::tearDown() {
}

//---------------------------------------------------------------------------

void TestScalars::testFactorial()
{
    try {
        Factorial(-1);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    if(Factorial(0) != 1)
        CPPUNIT_ASSERT(false);

    if(Factorial(1) != 1)
        CPPUNIT_ASSERT(false);

    if(Factorial(2) != 2)
        CPPUNIT_ASSERT(false);

    if(Factorial(3) != 6)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testBinomial()
{
    //Binomial(m, n) = Factorial(m)/(Factorial(n)*Factorial(m-n))

    try {
        Binomial(-1, 0);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    try {
        Binomial(0, -1);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    try {
        Binomial(-1, -1);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    if(Binomial(0, 0) != 1)
        CPPUNIT_ASSERT(false);

    if(Binomial(1, 0) != 1)
        CPPUNIT_ASSERT(false);
    if(Binomial(1, 1) != 1)
        CPPUNIT_ASSERT(false);

    if(Binomial(2, 0) != 1)
        CPPUNIT_ASSERT(false);
    if(Binomial(2, 1) != 2)
        CPPUNIT_ASSERT(false);
    if(Binomial(2, 2) != 1)
        CPPUNIT_ASSERT(false);

    if(Binomial(3, 0) != 1)
        CPPUNIT_ASSERT(false);
    if(Binomial(3, 1) != 3)
        CPPUNIT_ASSERT(false);
    if(Binomial(3, 2) != 3)
        CPPUNIT_ASSERT(false);
    if(Binomial(3, 3) != 1)
        CPPUNIT_ASSERT(false);

    if(Binomial(4, 0) != 1)
        CPPUNIT_ASSERT(false);
    if(Binomial(4, 1) != 4)
        CPPUNIT_ASSERT(false);
    if(Binomial(4, 2) != 6)
        CPPUNIT_ASSERT(false);
    if(Binomial(4, 3) != 4)
        CPPUNIT_ASSERT(false);
    if(Binomial(4, 4) != 1)
        CPPUNIT_ASSERT(false);

    if(Binomial(5, 0) != 1)
        CPPUNIT_ASSERT(false);
    if(Binomial(5, 1) != 5)
        CPPUNIT_ASSERT(false);
    if(Binomial(5, 2) != 10)
        CPPUNIT_ASSERT(false);
    if(Binomial(5, 3) != 10)
        CPPUNIT_ASSERT(false);
    if(Binomial(5, 4) != 5)
        CPPUNIT_ASSERT(false);
    if(Binomial(5, 5) != 1)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testLog()
{
    //log(a, x) = log(x)/log(a)

    try {
        Log(0, 1);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }
    try {
        Log(1, 0);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }
    try {
        Log(0, 0);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    try {
        Log(1, 1);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }
    try {
        Log(1, 2);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    if(Log(2, 1) != 0)
        CPPUNIT_ASSERT(false);
    if(Log(2, 2) != 1)
        CPPUNIT_ASSERT(false);
    if(Log(2, 4) != 2)
        CPPUNIT_ASSERT(false);

    if(Log(3, 1) != 0)
        CPPUNIT_ASSERT(false);
    if(Log(3, 3) != 1)
        CPPUNIT_ASSERT(false);
    if(Log(3, 9) != 2)
        CPPUNIT_ASSERT(false);
    if(Log(3, 27) != 3)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testLog2()
{
    try {
        Log2(-1);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }
    try {
        Log2(0);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    if(Log2(1) != 0)
        CPPUNIT_ASSERT(false);
    if(Log2(2) != 1)
        CPPUNIT_ASSERT(false);
    if(Log2(4) != 2)
        CPPUNIT_ASSERT(false);
    if(Log2(8) != 3)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testSign()
{
    if(Sign(-1) != -1)
        CPPUNIT_ASSERT(false);

    if(Sign(0) != 1)
        CPPUNIT_ASSERT(false);

    if(Sign(1) != 1)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testAbs()
{
    if(Abs(-2) != 2)
        CPPUNIT_ASSERT(false);
    if(Abs(-1) != 1)
        CPPUNIT_ASSERT(false);
    if(Abs(0) != 0)
        CPPUNIT_ASSERT(false);
    if(Abs(1) != 1)
        CPPUNIT_ASSERT(false);
    if(Abs(2) != 2)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}

void TestScalars::testMin()
{
    if(Min(-1, -10) != -10)
        CPPUNIT_ASSERT(false);
    if(Min(-1, 0) != -1)
        CPPUNIT_ASSERT(false);
    if(Min(0, -10) != -10)
        CPPUNIT_ASSERT(false);

    if(Min(-1.1, -10.1) != -10.1)
        CPPUNIT_ASSERT(false);
    if(Min(-1.1, 0.) != -1.1)
        CPPUNIT_ASSERT(false);
    if(Min(0., -10.1) != -10.1)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testMax()
{
    if(Max(-1, -10) != -1)
        CPPUNIT_ASSERT(false);
    if(Max(-1, 0) != 0)
        CPPUNIT_ASSERT(false);
    if(Max(0, -10) != 0)
        CPPUNIT_ASSERT(false);

    if(Max(-1.1, -10.1) != -1.1)
        CPPUNIT_ASSERT(false);
    if(Max(-1.1, 0.) != 0)
        CPPUNIT_ASSERT(false);
    if(Max(0., -10.1) != 0)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}

