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
//File: testBarrier.cpp
//Content: test for the module Barrier
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testBarrier.h"
#include "../src/Barrier.h"
#include "../src/Constants.h" //ERR_NUM
#include "../src/FiberMOSModelConstants.h"
#include "../src/Arm.h"
//#include "cppunit_assert_emulator.h" //Qt only.

#include <iostream>
#include <limits.h> //INT_MAX
#include <float.h> //DBL_MAX

using namespace Models;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestBarrier);

//overide setUp(), init data, etc
void TestBarrier::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestBarrier::tearDown() {
}

//---------------------------------------------------------------------------

void TestBarrier::test_()
{
    //build a barrier by default
    TBarrier B1(TDoublePoint(0, 0));

    //check if has all default values
    if(B1.dontHasAllDefaultValuesActuator()) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //check the contour
    if(B1.getContour_().getText().str != MEGARA_Contour_Text) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //----------------------------------------------------

    //set the contour of the EA
    B1.setContour_Text(MEGARA_LIFU_Contour_Text);
    B1.setSPM(MEGARA_Eo*B1.getr_max() + MEGARA_Ep);

    //check if has all default values
    if(B1.dontHasAllDefaultValuesEA()) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //check the contour
    if(B1.getContour_().getText().str != MEGARA_LIFU_Contour_Text) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //----------------------------------------------------

    //set the contour of the RP-Actuator
    B1.setContour_Text(MEGARA_Contour_Text);
    B1.setSPM(MEGARA_Eo*B1.getr_max() + MEGARA_Ep);

    //check if has all default values
    if(B1.dontHasAllDefaultValuesActuator()) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //check the contour
    if(B1.getContour_().getText().str != MEGARA_Contour_Text) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //----------------------------------------------------

    //clone the barrier
    TBarrier B2(&B1);
    if(B1 != B2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //move the barriers
    double x0 = B1.getr_max() + B1.getSPM();
    B1.setP0(TDoublePoint(-x0, 0));
    B2.set(TDoublePoint(x0, 0), M_PI);

    //-------------------------------------------------------------------
    //METHODS FOR DETECT COLLISIONS:

    //calculate the minimun distance with a barrier
    double D = B1.distanceMin(&B2);
    double Df = D - B1.getSPM() - B2.getSPM();

    //check the free distance
    if(fabs(Df) > ERR_NUM) {
        CPPUNIT_ASSERT(false);
        return;
    }

/*    //calculate the minimun distance with an arm
    x0 = 5.807111 + 2.35 + 0.195723055049871;
    TArm A(TDoublePoint(x0, 0));
    D = B1.distanceMin(&A);
    Df = D - B1.getSPM() - A.getSPM();

    //check the free distance
    if(fabs(Df) > ERR_NUM) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //If the contours are so far that can not be in collision,
    //the returned minimun distance will be DBL_MAX.

    //determine f there is collision with a barrier
    B2.setP0(TDoublePoint(B2.getP0().x + ERR_NUM, B2.getP0().y));
    bool b = B1.collides(&B2);
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //determine f there is collision with an arm
    A.setP1(TDoublePoint(A.getP1().x + ERR_NUM, A.getP1().y));
    b = B1.collides(&A);
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //determine if a point within the barrier
    b = B1.covers(TDoublePoint(-B1.getSPM(), 0));
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }
    b = B1.covers(TDoublePoint(ERR_NUM, 0));
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }
*/
    //----------------------------------------------------

    CPPUNIT_ASSERT(true);
}

