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
//File: testFMOSA.cpp
//Content: test for the module FMOSA
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testFMOSA.h"
#include "../src/FMOSA.h"
//#include "cppunit_assert_emulator.h" //Qt only.

#include <iostream>
#include <limits.h> //INT_MAX
#include <float.h> //DBL_MAX

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestFMOSA);

//overide setUp(), init data, etc
void TestFMOSA::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestFMOSA::tearDown() {
}

//---------------------------------------------------------------------------

void TestFMOSA::test_()
{
/*    TFMOSA FMOSA1;

    //build a string with the content of a little FMOSA file
    string str1;
    str1 = "# Id| Ra| Dec| Pos";
    str1 += "\r\n@@SOB@@";
    str1 += "\r\n0| 15.027879| 45.000422| 0.060858";
    str1 += "\r\n@@EOB@@";
    str1 += "\r\n#      Name             RA         Dec    Mag        Type         Pr  Bid Pid   X(mm)     Y(mm)  Enabled      Comment";
    str1 += "\r\n@@SOS@@";
    str1 += "\r\ns:751               |15.027787|45.017925|20.67|SOURCE          |6 |0  |1  |-0.137909 |51.932865 |1|foo comment         ";
    str1 += "\r\n                    |15.003376|45.028110|     |UNKNOWN         |  |   |2  |-51.296480|82.217116 |1|                    ";
    str1 += "\r\ns:397               |15.039336|45.029960|18.02|SOURCE          |0 |0  |3  |24.118230 |87.619726 |1|foo comment         ";
    str1 += "\r\n@@EOS@@";

    //set the string in a FMOSA
    unsigned int Bid;
    FMOSA1.setTableText(Bid, str1);

    //clone the FMOSA
    TFMOSA FMOSA2;
    FMOSA2.Clone(FMOSA1);
    if(FMOSA1.getCount() != FMOSA2.getCount()) {
        CPPUNIT_ASSERT(false);
        return;
    }
    for(int i=0; i<FMOSA1.getCount(); i++)
        if(FMOSA1[i] != FMOSA2[i]) {
            CPPUNIT_ASSERT(false);
            return;
        }

    //check a nonallocated item
    TObservingSource *OS = FMOSA2.GetPointer(1);
    if("" != OS->Name ||
            15.003376 != OS->RA ||
            45.028110 != OS->Dec ||
            0 != OS->Mag ||
            ptUNKNOWN != OS->Type ||
            0 != OS->Pr ||
            0 != OS->Bid ||
            2 != OS->Pid ||
            -51.296480 != OS->X ||
            82.217116 != OS->Y ||
            true != OS->Enabled ||
            "" != OS->Comment ||
            false != OS->there_is_Mag ||
            false != OS->there_is_Pr ||
            false != OS->there_is_Bid) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //check an allocated item
    OS = FMOSA2.GetPointer(2);
    if("s:397" != OS->Name ||
            15.039336 != OS->RA ||
            45.029960 != OS->Dec ||
            18.02 != OS->Mag ||
            ptSOURCE != OS->Type ||
            0 != OS->Pr ||
            0 != OS->Bid ||
            3 != OS->Pid ||
            24.118230 != OS->X ||
            87.619726 != OS->Y ||
            true != OS->Enabled ||
            "foo comment" != OS->Comment ||
            true != OS->there_is_Mag ||
            true != OS->there_is_Pr ||
            true != OS->there_is_Bid) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //get the clon FMOSA in text format
    string str2;
    FMOSA2.getTableText(str2); //<----en floattostr_fixed(value, precision), precision parece ser el número de cifras significativas, y no de decimales
    if(str1 != str2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //get the allocations
    TRoboticPositionerList RPL;
    RPL.Add(new TRoboticPositioner(1, TDoublePoint(0, 52.221332)));
    RPL.Add(new TRoboticPositioner(2, TDoublePoint(-60.3, 87.035553)));
    RPL.Add(new TRoboticPositioner(3, TDoublePoint(20.1, 87.035553)));
    TAllocationList AL(&RPL);
    FMOSA1.getAllocations(AL);
    TAllocation *A;
    A= AL[0];
    OS = FMOSA1.GetPointer(0);
    if(A->getRP()->getActuator()->getId() != (signed int)OS->Pid ||
            A->PP.x != OS->X || A->PP.y != OS->Y ||
            A->PP.Type != OS->Type ||
            A->PP.Priority != OS->Pr) {
        CPPUNIT_ASSERT(false);
        return;
    }
    A = AL[1];
    OS = FMOSA1.GetPointer(2);
    if(A->getRP()->getActuator()->getId() != (signed int)OS->Pid ||
            A->PP.x != OS->X || A->PP.y != OS->Y ||
            A->PP.Type != OS->Type ||
            A->PP.Priority != OS->Pr) {
        CPPUNIT_ASSERT(false);
        return;
    }
*/
    CPPUNIT_ASSERT(true);
}

