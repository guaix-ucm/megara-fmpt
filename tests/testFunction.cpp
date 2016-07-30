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
//File: testFunction.cpp
//Content: test for the module Function
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testFunction.h"
#include "../src/Function.h"
#include "../src/Constants.h" //M_2PI
#include "../src/FiberMOSModelConstants.h" //MEGAMARA_SB1
//#include "cppunit_assert_emulator.h" //Qt only.

#include <iostream>
#include <limits.h> //INT_MAX
#include <float.h> //DBL_MAX

using namespace Operators;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestFunction);

//overide setUp(), init data, etc
void TestFunction::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestFunction::tearDown() {
}

//---------------------------------------------------------------------------

void TestFunction::test_()
{
    TFunction F;

    //añade los puntos de la función de compresión
    F.Add(-M_2PI, -double(MEGARA_SB1));
    F.Add(0., 0.);
    F.Add(M_2PI, double(MEGARA_SB1));
    F.Add(2*M_2PI, double(2*MEGARA_SB1));

    if(F.getX(0) != -M_2PI || F.getY(0) != -double(MEGARA_SB1) ||
            F.getX(1) != 0 || F.getY(1) != 0 ||
            F.getX(2) != M_2PI || F.getY(2) != double(MEGARA_SB1) ||
            F.getX(3) != 2*M_2PI || F.getY(3) != 2*double(MEGARA_SB1)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    TFunction G;
    G.Inverse(F);

    if(G.getY(0) != -M_2PI || G.getX(0) != -double(MEGARA_SB1) ||
            G.getY(1) != 0 || G.getX(1) != 0 ||
            G.getY(2) != M_2PI || G.getX(2) != double(MEGARA_SB1) ||
            G.getY(3) != 2*M_2PI || G.getX(3) != 2*double(MEGARA_SB1)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(fabs(F.Image(-M_PI) + double(MEGARA_SB1)/2) > ERR_NUM) {
        CPPUNIT_ASSERT(false);
        return;
    }

    string str;
    str += "-6.28318530717959   -194953.8462\r\n";
    str += "0                   0\r\n";
    str += "6.28318530717959    194953.8462\r\n";
    str += "12.5663706143592    389907.6924";

    TFunction F_;
    F_.setTableText(str);

    if(F_.getTableText() != str) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

