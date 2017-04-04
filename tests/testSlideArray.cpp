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
//File: testSlideArray.cpp
//Content: test for the module SlideArray
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testSlideArray.h"
#include "../src/Constants.h" //M_2PI
//#include "cppunit_assert_emulator.h" //Qt only.

#include <cmath> //abs
#include <vector>
//#include <stdlib.h>
#include <limits> //std::numeric_limits

#include "../src/SlideArray.h"

//using namespace Mathematics;
using namespace Lists;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestSlideArray);

//overide setUp(), init data, etc
void TestSlideArray::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestSlideArray::tearDown() {
}

//---------------------------------------------------------------------------

void TestSlideArray::test_()
{
    for(int i=0; i<10000; i++) {
        try {
            //build a slide array
            TSlideArray<int> *SL = new TSlideArray<int>();

            //add 100 items to the array
            for(int j=0; j<100; j++)
                SL->AddLast(j);
            
            //check the value of each item
            for(int j=0; j<100; j++)
                if(SL->Get(j) != j) {
                    CPPUNIT_ASSERT(false);
                    return;
                }

            //free the dinamic memory
            delete SL;

        } catch(...) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }


/*    //----------------------------------
    //NewFirst

    TSlideArray<int> SA;
    int N = 100000;

    for(int i=0; i<N; i++) {
        SA.NewFirst(1);
        SA.setFirst(i);
    }

    for(int i=0; i<N; i++) 
        if(SA.Get(i) != N-1-i) {
            CPPUNIT_ASSERT(false);
            return;
        }

    //----------------------------------
    //NewLast

    TSlideArray<int> SA;
    int N = 100000;

    for(int i=0; i<N; i++) {
        SA.NewLast(1);
        SA.setLast(i);
    }

    for(int i=0; i<N; i++) 
        if(SA.Get(i) != i) {
            CPPUNIT_ASSERT(false);
            return;
        }
*/
    CPPUNIT_ASSERT(true);
}
