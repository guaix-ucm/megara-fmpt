// Copyright (c) 2012-2017 Isaac Morales Durán. All rights reserved.
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
//File: TestOutputsPairPPDP.h
//Content: test for the module Outputs
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef TEST_Outputs_H
#define TEST_Outputs_H

#include <cppunit/extensions/HelperMacros.h>

//build the class where define the test methods
class TestOutputsPairPPDP : public CppUnit::TestFixture
{
    //establish the test suit of Test
    CPPUNIT_TEST_SUITE(TestOutputsPairPPDP);

    //add test methods
    CPPUNIT_TEST(test_);

    //finish the process
    CPPUNIT_TEST_SUITE_END();

public:
    //overide setUp(), init data, etc
    void setUp();
    //overide tearDown(), free allocated memory, etc
    void tearDown();

    //test methods
    void test_();
};

#endif // TEST_Outputs_H
