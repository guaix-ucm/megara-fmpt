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
//File: testMotionProgramGenerator.cpp
//Content: test for the module MotionProgramGenerator
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testMotionProgramGenerator.h"
#include "../src/MotionProgramGenerator.h"
//#include "cppunit_assert_emulator.h" //Qt only.

#include <iostream>
#include <limits.h> //INT_MAX
#include <float.h> //DBL_MAX

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestMotionProgramGenerator);

//overide setUp(), init data, etc
void TestMotionProgramGenerator::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestMotionProgramGenerator::tearDown() {
}

//---------------------------------------------------------------------------

void TestMotionProgramGenerator::test_generatePairPPDP()
{
    //build a simple FMM
    TFiberMOSModel FMM;
    FMM.RPL.Add(new TRoboticPositioner(2, TDoublePoint(-60.3, 87.035553)));
    FMM.RPL.Add(new TRoboticPositioner(4, TDoublePoint(-40.2, 87.035553)));
    FMM.assimilate();

    //set an allocation for each RP
    TMotionProgramGenerator MPG(&FMM);
    MPG.Add(new TAllocation(FMM.RPL[0], -60.3, 97.035553));
    MPG.Add(new TAllocation(FMM.RPL[1], -40.2, 77.035553));

    //move the RPs to the more closer stable position to the allocated projection points
    MPG.MoveToTargetP3();

    //captures the observing positions of the RPs in a PPA list
    TPairPositionAnglesList OPL;
    FMM.RPL.getPositions(OPL);

    //check the OPL
    string str = OPL.getText().str;
    if(str != "Id\tp_1\tp___3\r\n2\t129703\t10156\r\n4\t32226\t10156") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //segregates the operative outsider RPs
    TRoboticPositionerList Outsiders;
    FMM.RPL.segregateOperativeOutsiders(Outsiders);

    //check the ooperative outsider list
    str = Outsiders.getText().str;
    if(str != "{2, 4}") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //generate the parking program
    bool PPvalid, DPvalid;
    TRoboticPositionerList Collided;
    TRoboticPositionerList Obstructed;
    TMotionProgram PP, DP;
    MPG.generatePairPPDP(PPvalid, DPvalid, Collided, Obstructed, PP, DP, Outsiders);

    //check that the generated MPs are not empty
    if(PP.getText().str.length() <= 0 || DP.getText().str.length() <= 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //validate the generated MPs
    FMM.RPL.moveToOrigins();
    FMM.RPL.setPurpose(pValPP);
    bool PPvalid_ = MPG.validateMotionProgram(PP);
    FMM.RPL.setPurpose(pValDP);
    bool DPvalid_ = MPG.validateMotionProgram(DP);

    //check the flag of validation
    if(PPvalid != PPvalid_ || DPvalid != DPvalid_) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //check that the generated MPs are valid
    if(!PPvalid || !DPvalid) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void TestMotionProgramGenerator::test_generateParkingProgram()
{
    //build a simple FMM
    TFiberMOSModel FMM;
    FMM.RPL.Add(new TRoboticPositioner(2, TDoublePoint(-60.3, 87.035553)));
    FMM.RPL.Add(new TRoboticPositioner(4, TDoublePoint(-40.2, 87.035553)));
    FMM.assimilate();

    //set an allocation for each RP
    TMotionProgramGenerator MPG(&FMM);
    MPG.Add(new TAllocation(FMM.RPL[0], -60.3, 97.035553));
    MPG.Add(new TAllocation(FMM.RPL[1], -40.2, 77.035553));

    //move the RPs to the more closer stable position to the allocated projection points
    MPG.MoveToTargetP3();

    //captures the starting positions of the RPs in a PPA list
    TPairPositionAnglesList SPL;
    FMM.RPL.getPositions(SPL);

    //check the SPL
    string str = SPL.getText().str;
    if(str != "Id\tp_1\tp___3\r\n2\t129703\t10156\r\n4\t32226\t10156") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //segregates the operative outsider RPs
    TRoboticPositionerList Outsiders;
    FMM.RPL.segregateOperativeOutsiders(Outsiders);

    //check the ooperative outsider list
    str = Outsiders.getText().str;
    if(str != "{2, 4}") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //generate the parking program
    TRoboticPositionerList Collided;
    TRoboticPositionerList Obstructed;
    TMotionProgram ParkingProgram;
    bool valid = MPG.generateParkingProgram(Collided, Obstructed, ParkingProgram, Outsiders);

    //check that the generated MP is not empty
    if(ParkingProgram.getText().str.length() <= 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //validate the generated MP
    MPG.MoveToTargetP3();
    FMM.RPL.setPurpose(pValParPro);
    bool valid_ = MPG.validateMotionProgram(ParkingProgram);

    //check the flag of validation
    if(valid != valid_) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //check that the generated MP is valid
    if(!valid) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
void TestMotionProgramGenerator::test_generatePairPPDP_online()
{
    //build a simple FMM
    TFiberMOSModel FMM;
    FMM.RPL.Add(new TRoboticPositioner(2, TDoublePoint(-60.3, 87.035553)));
    FMM.RPL.Add(new TRoboticPositioner(4, TDoublePoint(-40.2, 87.035553)));
    FMM.assimilate();

    //set an allocation for each RP
    vector<double> p_1s;
    vector<double> p___3s;
    p_1s.push_back(129703);
    p_1s.push_back(32226);
    p___3s.push_back(10156);
    p___3s.push_back(10156);

    //build a vector of Ids
    vector<int> Ids;

    //generate the parking program
    TRoboticPositionerList Collided;
    TRoboticPositionerList Obstructed;
    TMotionProgram PP, DP;
    bool valid = generatePairPPDP_online(PP, DP,
                                         FMM, p_1s, p___3s, Ids);

    //check that the generated MP is not empty
    if(PP.getText().str.length() <= 0 || DP.getText().str.length() <= 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //validate the generated MP
    TMotionProgramValidator MPV(&FMM);
    FMM.RPL.moveToOrigins();
    FMM.RPL.setPurpose(pValPP);
    bool PPvalid_ = MPV.validateMotionProgram(PP);
    FMM.RPL.setPurpose(pValDP);
    bool DPvalid_ = MPV.validateMotionProgram(DP);

    //check the flag of validation
    if(valid != PPvalid_ || valid != DPvalid_) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //check that the generated MP is valid
    if(!valid) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void TestMotionProgramGenerator::test_generateParkingProgram_online()
{
    //build a simple FMM
    TFiberMOSModel FMM;
    FMM.RPL.Add(new TRoboticPositioner(2, TDoublePoint(-60.3, 87.035553)));
    FMM.RPL.Add(new TRoboticPositioner(4, TDoublePoint(-40.2, 87.035553)));
    FMM.assimilate();

    //set an allocation for each RP
    vector<double> p_1s;
    vector<double> p___3s;
    p_1s.push_back(129703);
    p_1s.push_back(32226);
    p___3s.push_back(10156);
    p___3s.push_back(10156);

    //build a vector of Ids
    vector<int> Ids;

    //generate the parking program
    TRoboticPositionerList Collided;
    TRoboticPositionerList Obstructed;
    TMotionProgram ParkingProgram;
    bool valid = generateParkingProgram_online(ParkingProgram,
                                               FMM, p_1s, p___3s, Ids);

    //check that the generated MP is not empty
    if(ParkingProgram.getText().str.length() <= 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //validate the generated MP
    TMotionProgramValidator MPV(&FMM);
    FMM.RPL[0]->getActuator()->setAnglesSteps(p_1s[0], p___3s[0]);
    FMM.RPL[0]->getActuator()->setAnglesSteps(p_1s[1], p___3s[1]);
    FMM.RPL.setPurpose(pValParPro);
    bool valid_ = MPV.validateMotionProgram(ParkingProgram);

    //check the flag of validation
    if(valid != valid_) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //check that the generated MP is valid
    if(!valid) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

