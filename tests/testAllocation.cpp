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
//File: testAllocation.cpp
//Content: test for the module Allocation
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testAllocation.h"
#include "../src/Allocation.h"
//#include "cppunit_assert_emulator.h" //Qt only.

#include <iostream>
#include <limits.h> //INT_MAX
#include <float.h> //DBL_MAX

using namespace Models;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestAllocation);

//overide setUp(), init data, etc
void TestAllocation::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestAllocation::tearDown() {
}

//---------------------------------------------------------------------------

void TestAllocation::test_()
{
    //BUILDING AND DESTROYING METHODS:

    for(int i=0; i<10; i++) {
        TRoboticPositioner RP = new TRoboticPositioner();
        TAllocation *A = new TAllocation(&RP, 0, 0);
        //destroy a TAllocation
        //if thereisn't a built target point
        //  throw an exception EImproperCall
        delete A;
    }

    TRoboticPositioner RP1(1, TDoublePoint(0, 0));
    TRoboticPositioner RP2(2, TDoublePoint(20, 20));
    TAllocation A1(&RP1, TDoublePoint(0, 0));
    TAllocation A2(&RP2, 20, 20);

    if(A1.getRP() != &RP1 || A1.PP != TDoublePoint(0, 0)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(A2.getRP() != &RP2 || A2.PP != TDoublePoint(20, 20)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //------------------------------------------------------------------
    //STATIC METHODS:

    //compare the identifiers of the RPs attached to two target points
    if(TAllocation::CompareIds(&A1, &A2) != -1) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //this method shall be pointed in a pointer list
    //to allow the functioning of shorting and comparing methods

    //get the labels of the properties
    //in row text format
    if(TAllocation::GetIdPPLabelsRow() != "Id\tx\ty") {
        CPPUNIT_ASSERT(false);
        return;
    }

    try {
        //travel the labels of the properties
        //in a text string from the position i
        AnsiString S("x \r\nId\tx\ty");
        int i = 2;
        TAllocation::TravelLabels(S, i);

        if(i != 11) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }
    catch(...) {
        CPPUNIT_ASSERT(false);
        return;
    }

    try {
        //travel the labels of the properties
        //in a text string from the position i
        AnsiString S("x \r\n\t 4 \t 1.123   4.55 xxx");
        int i = 2;
        //read the values of the properties
        //in a text string from the position i
        int Id;
        double x, y;
        TAllocation::ReadSeparated(Id, x, y, S, i);

        if(i != 23 || Id != 4 || x != 1.123 || y != 4.55) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }
    catch(...) {
        CPPUNIT_ASSERT(false);
        return;
    }


    //print the properties of an allocation in a string
    //in row format
    AnsiString S = "x ";
    TAllocation::PrintRow(S, &A1);
    if(S != "x 1\t0\t0") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //CHECKING METHODS:

    //determines if the target point is out of the domain
    //of the point P3 of the attached RP
    double r_3max = A1.getRP()->getActuator()->getr_3max();
    A1.PP = TDoublePoint(0, r_3max);
    if(A1.IsOutDomainP3()) {
        CPPUNIT_ASSERT(false);
        return;
    }
    A1.PP = TDoublePoint(0, r_3max + ERR_NUM + ERR_NUM);
    if(!A1.IsOutDomainP3()) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //determines if the target point is in the secure area
    //of the point P3 of the attached RP
    A1.PP = TDoublePoint(0, 0);
    if(!A1.IsInSafeAreaP3()) {
        CPPUNIT_ASSERT(false);
        return;
    }
    A1.PP = TDoublePoint(0, r_3max + ERR_NUM + ERR_NUM);
    if(A1.IsInSafeAreaP3()) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //MOTION METHODS:

    //assign the point P3o of the attacheed RP to the point PP
    A1.SetP3o();
    if(A1.PP != A1.getRP()->getActuator()->getP3o()) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //assign the point P3 of the attacheed RP to the point PP
    RP1.getActuator()->setAnglesRadians(M_PI, M_PI);
    TDoublePoint P3 = RP1.getActuator()->getArm()->getP3();
    A1.SetP3();
    if(A1.PP != P3) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //randomize the point PP with uniform distribution
    //in the domain of the point P3 of its attached RP
    for(int i=0; i<10; i++) {
        A1.RandomizePP();
        if(Mod(A1.PP - RP1.getActuator()->getP0()) > r_3max + ERR_NUM) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //There are three ways to randomize a point in the domain of a RP:
    //  - Randomizing the angular positions of their rotors in their
    //    respective domains;
    //  - Randomizing the point in polar coordinates respect S1 with
    //    uniform distribution in the radial interval. Iterating the
    //    process until the point is in the domain of the RP.
    //  - Randomizing the point in Cartesian coordinates respect S0,
    //    in the Cartesian insterval of the domain of the RP, until
    //    thepoint is inthedomain of the RP.
    //Themethod Ramize, implement the las way.

    //assign the point PP to the point P3 of its attached RP
    //and return the distance from the stable position to the target point
    //if the the point PP isn't on the domain of its attached RP:
    //  throw an exception EImpropercall

    for(int i=0; i<10; i++) {
        RP1.getActuator()->randomizeP3();
        A1.MoveToPP();
        double d = Mod(A1.PP - RP1.getActuator()->getArm()->getP3());
        if(d > 0.0005) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //NOTE: method MoveToPP will move the rotors of the RP to the
    //positions corresponding to the P3 is positioned on the PP.
    //When the quantification of the rotors is enabled, will be quantified
    //first the rotor 1, and after the rotor 2, in any case, uncheking
    //which is the neares stable point.

    CPPUNIT_ASSERT(true);
}

