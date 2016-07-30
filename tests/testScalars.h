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
    CPPUNIT_TEST(test_ceil);
    CPPUNIT_TEST(test_floor);
    //----------------------
    CPPUNIT_TEST(test_Factorial);
    CPPUNIT_TEST(test_Binomial);
    CPPUNIT_TEST(test_Log);
    CPPUNIT_TEST(test_Log2);
    CPPUNIT_TEST(test_Sign);
    CPPUNIT_TEST(test_Abs);
    //----------------------
    CPPUNIT_TEST(test_min);
    CPPUNIT_TEST(test_max);
    //----------------------
    CPPUNIT_TEST(test_LimitDomain);
    //----------------------
    CPPUNIT_TEST(test_Round);
    CPPUNIT_TEST(test_Near);
    CPPUNIT_TEST(test_Far);
    //----------------------
    CPPUNIT_TEST(test_RandomReal);
    CPPUNIT_TEST(test_RandomUniformDouble);
    CPPUNIT_TEST(test_RandomUniformInteger);
    CPPUNIT_TEST(test_ArgPos);
    //----------------------
    CPPUNIT_TEST(test_Numerical);
    CPPUNIT_TEST(test_FloatingPoint);
    //----------------------
    CPPUNIT_TEST(test_incmin);

    //finish the process
    CPPUNIT_TEST_SUITE_END();

public:
    //overide setUp(), init data, etc
    void setUp();
    //overide tearDown(), free allocated memory, etc
    void tearDown();

    //test methods
    void test_ceil();
    void test_floor();
    //----------------------
    void test_Factorial();
    void test_Binomial();
    void test_Log();
    void test_Log2();
    void test_Sign();
    void test_Abs();
    //----------------------
    void test_min();
    void test_max();
    //----------------------
    void test_LimitDomain();
    //----------------------
    void test_Round();
    void test_Near();
    void test_Far();
    //----------------------
    void test_RandomReal();
    void test_RandomUniformDouble();
    void test_RandomUniformInteger();
    void test_ArgPos();
    //----------------------
    void test_Numerical();
    void test_FloatingPoint();
    //----------------------
    void test_incmin();
};

#endif // FMPT_TESTSCALARS_H
