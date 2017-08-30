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
//File: testPositionerCenter.cpp
//Content: test for the module PositionerCenter
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testPositionerCenter.h"
#include "../src/TextFile.h"
#include "../src/PositionerCenter.h"
#include "../src/FiberMOSModel.h"
//#include "cppunit_assert_emulator.h" //Qt only.

#include <iostream>
#include <limits.h> //INT_MAX
#include <float.h> //DBL_MAX

using namespace Models;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestPositionerCenter);

//overide setUp(), init data, etc
void TestPositionerCenter::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestPositionerCenter::tearDown() {
}

//---------------------------------------------------------------------------

void TestPositionerCenter::test_()
{
    try {
        //LOAD SETTINGS FROM FILES:

        //load the positioner center table from a file
        string path = "../data/Models/positionerCenters_theoretical_92RPs.txt";
        ///string path = "../megarafmpt/data/Models/positionerCenters_theoretical_92RPs.txt"; //Qt only
        string str;
        strReadFromFile(str, path);
        TPositionerCenterList PCL;
        PCL.setTableText(str);

        //MAKE THE OPERATIONS:

        //assimilates the table in a Fiber MOS Model
        TFiberMOSModel FMM;
        for(int i=0; i<PCL.getCount(); i++) {
            TPositionerCenter PC = PCL[i];
            int j = FMM.RPL.searchId(PC.Id);
            if(j < FMM.RPL.getCount()) {
                TRoboticPositioner *RP = FMM.RPL[j];
                RP->getActuator()->setP0(TDoublePoint(PC.x0, PC.y0));
            } else {
                int Id = FMM.RPL.searchFirstFreeId(1);
                TDoublePoint P0;
                P0.x = PC.x0;
                P0.y = PC.y0;
                TRoboticPositioner *RP = new TRoboticPositioner(Id, P0);
                FMM.RPL.Add(RP);
            }
        }
        FMM.assimilate();
    }
    catch(...) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

