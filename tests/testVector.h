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
//File: testVector.h
//Content: test for the module Vector
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FMPT_TESTVECTOR_H
#define FMPT_TESTVECTOR_H

#include <cppunit/extensions/HelperMacros.h>

//build the class where define the test methods
class TestVector : public CppUnit::TestFixture
{
    //establish the test suit of Test
    CPPUNIT_TEST_SUITE(TestVector);

    //add test methods
    CPPUNIT_TEST(testBuildAndDestroy);
    CPPUNIT_TEST(testSetDimension);
    CPPUNIT_TEST(testCopy);
    CPPUNIT_TEST(testAcum);
    CPPUNIT_TEST(testPow2);
    CPPUNIT_TEST(testSqrt);

    //finish the process
    CPPUNIT_TEST_SUITE_END();

public:
    //overide setUp(), init data, etc
    void setUp();
    //overide tearDown(), free allocated memory, etc
    void tearDown();

    //test methods
    void testBuildAndDestroy();
    void testSetDimension();
    void testCopy();
    void testAcum();
    void testPow2();
    void testSqrt();
};

#endif // FMPT_TESTVECTOR_H