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
//File: testActuator.cpp
//Content: test for the module Actuator
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testActuator.h"
#include "../src/Constants.h" //M_2PI
//#include "cppunit_assert_emulator.h" //Qt only.

#include <cmath> //abs
#include <vector>
//#include <stdlib.h>
#include <limits> //std::numeric_limits

#include "../src/Actuator.h"

using namespace Mathematics;
using namespace Models;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestActuator);

//overide setUp(), init data, etc
void TestActuator::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestActuator::tearDown() {
}

//---------------------------------------------------------------------------

void TestActuator::test_()
{
    for(int i=0; i<1000; i++) {
        try {
            //build an object
            TDoublePoint P0(0, 0);
            TActuator *A = new TActuator(1, P0);

            //free the dinamic memory
            delete A;

        } catch(...) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //construye un RP por defecto y le asigna (theta_1, theta___3)
    TDoublePoint P0(0, 0);
    TActuator A(1, P0);
    A.setQuantify_(false);
    A.getArm()->setQuantify___(false);
    A.setAnglesRadians(M_PI/2, M_PI);

    //obtiene la posición numérica del punto P3
    double x3, y3;
    x3 = A.getArm()->getP3().x;
    y3 = A.getArm()->getP3().y;

    //calcula la posición teórica del punto P3
    double x3_ = 0;
    double y3_ = - A.getL01() - A.getArm()->getL13();

    //compara la coordenada x
    if((x3_ - x3) > ERR_NUM)
        CPPUNIT_ASSERT(false);

    //compara la coordenada y
    if((y3_ - y3) > ERR_NUM)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
