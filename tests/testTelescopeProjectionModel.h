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
//File: testTelescopeProjectionModel.h
//Content: test for the module TelescopeProjectionModel
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FMPT_TESTTelescopeProjectionModel_H
#define FMPT_TESTTelescopeProjectionModel_H

#include <cppunit/extensions/HelperMacros.h>

//build the class where define the test methods
class TestTelescopeProjectionModel : public CppUnit::TestFixture
{
    //establish the test suit of Test
    CPPUNIT_TEST_SUITE(TestTelescopeProjectionModel);

    //add test methods
    CPPUNIT_TEST(test_ProjectionCircle);
    CPPUNIT_TEST(test_FocalPlane);
    CPPUNIT_TEST(test_TelescopeProjectionModel);

    //finish the process
    CPPUNIT_TEST_SUITE_END();

public:
    //overide setUp(), init data, etc
    void setUp();
    //overide tearDown(), free allocated memory, etc
    void tearDown();

    //test methods
    void test_ProjectionCircle();
    void test_FocalPlane();
    void test_TelescopeProjectionModel();
};

#endif // FMPT_TESTTelescopeProjectionModel_H
