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
//File: testItemsList.cpp
//Content: test for the module ItemsList
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testItemsList.h"
#include "../src/Constants.h" //M_2PI
//#include "cppunit_assert_emulator.h" //Qt only.

#include <cmath> //abs
#include <vector>
//#include <stdlib.h>
#include <limits> //std::numeric_limits

#include "../src/ItemsList.h"

using namespace Lists;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestItemsList);

//overide setUp(), init data, etc
void TestItemsList::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestItemsList::tearDown() {
}

//---------------------------------------------------------------------------

void TestItemsList::test_()
{
    for(int i=0; i<10000; i++) {
        try {
            //build an object
            TItemsList<int> *L = new TItemsList<int>();

            //free the dinamic memory
            delete L;

        } catch(...) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    CPPUNIT_ASSERT(true);
}

