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
//File: testRoboticPositionerList.cpp
//Content: test for the module RoboticPositionerList
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testRoboticPositionerList3.h"
#include "../src/Constants.h" //M_2PI
//#include "cppunit_assert_emulator.h" //Qt only.

#include <cmath> //abs
#include <vector>
//#include <stdlib.h>
#include <limits> //std::numeric_limits

#include "../src/RoboticPositionerList3.h"

//using namespace Mathematics;
using namespace Lists;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestRoboticPositionerList);

//overide setUp(), init data, etc
void TestRoboticPositionerList::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestRoboticPositionerList::tearDown() {
}

//---------------------------------------------------------------------------

void TestRoboticPositionerList::test_()
{
    for(int i=0; i<100; i++) {
        try {
            //build an object
            TRoboticPositionerList *RPL = new TRoboticPositionerList();

            //free the dinamic memory
            delete RPL;

        } catch(...) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    CPPUNIT_ASSERT(true);
}

