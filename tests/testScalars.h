// Copyright (c) 2012-2016 Isaac Morales Durán. All rights reserved.
// Institute of Astrophysics of Andalusia, IAA-CSIC
//
// This file is part of FMPT (Fiber MOS Positioning Tools)
//
// FMPT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//---------------------------------------------------------------------------
//File: testScalars.h
//Content: test for the module Scalars
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

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

//    CPPUNIT_TEST(test_frexp10);
    CPPUNIT_TEST(test_incmin);

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

    void testLimitDomain();

    void testRound();
    void testNear();
    void testFar();

    void testRandomReal();
    void testRandomUniformDouble();
    void testRandomUniformInteger();
    void testArgPos();

    void testNumerical();
    void testFloatingPoint();

//    void test_frexp10();
    void test_incmin();
};

#endif // FMPT_TESTSCALARS_H
