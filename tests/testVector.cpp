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
//File: testVector.cpp
//Content: test for the module Vector
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testVector.h"
#include "../src/Constants.h" //M_2PI
//#include "cppunit_assert_emulator.h" //Qt only.

#include <cmath> //abs, sqrt
#include <vector>
//#include <stdlib.h>
#include <limits> //std::numeric_limits

#include "../src/Vector.h"

using namespace Lists;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestVector);

//overide setUp(), init data, etc
void TestVector::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestVector::tearDown() {
}

//---------------------------------------------------------------------------

void TestVector::testBuildAndDestroy()
{
    for(int i=0; i<10000; i++) {
        try {
            //build an object
            TVector<int> *V = new TVector<int>();

            //free the dinamic memory
            delete V;

        } catch(...) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    CPPUNIT_ASSERT(true);
}

void TestVector::testSetDimension()
{
    TVector<double> V;

    V.SetDimension(11);
    if(V.getCount() != 11) {
        CPPUNIT_ASSERT(false);
        return;
    }
    for(int i=0; i<V.getCount(); i++)
        if(V[i] != 0.) {
            CPPUNIT_ASSERT(false);
            return;
        }

    V.SetDimension(3);
    if(V.getCount() != 3) {
        CPPUNIT_ASSERT(false);
        return;
    }
    for(int i=0; i<V.getCount(); i++)
        if(V[i] != 0.) {
            CPPUNIT_ASSERT(false);
            return;
        }

    V.SetDimension(0);
    if(V.getCount() != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void TestVector::testCopy()
{
    TVector<int> V1;

    V1.setCount(3);
    V1[0] = 7;
    V1[0] = 3;
    V1[0] = 0;

    TVector<int> V2 = V1;

    if(V2.getCount() != V1.getCount()) {
        CPPUNIT_ASSERT(false);
        return;
    }
    for(int i=0; i<V1.getCount(); i++)
        if(V1[i] != V2[i]) {
            CPPUNIT_ASSERT(false);
            return;
        }

    CPPUNIT_ASSERT(true);
}

void TestVector::testAcum()
{
    TVector<int> V;

    V.setCount(3);
    V[0] = 7;
    V[1] = 3;
    V[2] = 0;

    V.Acum();

    if(V.getCount() != 3) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(V[0] != 7) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(V[1] != 10) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(V[2] != 10) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void TestVector::testPow2()
{
    TVector<int> V;

    V.setCount(3);
    V[0] = 7;
    V[1] = 3;
    V[2] = 0;

    V.Pow2();

    if(V.getCount() != 3) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(V[0] != 49) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(V[1] != 9) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(V[2] != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void TestVector::testSqrt()
{
    TVector<double> V;

    V.setCount(3);
    V[0] = 7.;
    V[1] = 3.;
    V[2] = 0.;

    double v0 = sqrt(7.);
    double v1 = sqrt(3.);
    double v2 = sqrt(0.);

    V.Sqrt();

    if(V.getCount() != 3) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(V[0] != v0) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(V[1] != v1) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(V[2] != v2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
