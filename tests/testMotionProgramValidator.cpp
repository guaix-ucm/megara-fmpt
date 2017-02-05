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
//File: testMotionProgramValidator.cpp
//Content: test for the module MotionProgramValidator
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testMotionProgramValidator.h"
#include "../src/TextFile.h"
#include "../src/MotionProgramValidator.h"
#include "../src/FileMethods.h"
//#include "cppunit_assert_emulator.h" //Qt only.

#include <iostream>
#include <limits.h> //INT_MAX
#include <float.h> //DBL_MAX

using namespace Models;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestMotionProgramValidator);

//build and initialize the properties
TestMotionProgramValidator::TestMotionProgramValidator(void) :
    PP(), DP(), FMM(), MPV(&FMM)
{
}

//overide setUp(), init data, etc
void TestMotionProgramValidator::setUp() {
    try {
        //load a PP from a sample file
        string path_PP = "../../../data/Samples/example_generatePairPPDP_offline/PP-FMPT-from-megara-cb0.txt";
        //string path_PP = "../megarafmpt/data/Samples/example_generatePairPPDP_offline/PP-FMPT-from-megara-cb0.txt"; //Qt only
        string str;
        strReadFromFile(str, path_PP);
        TMotionProgram PP;
        PP.setText(str);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "loading positioning program: ");
        throw;
    }

    try {
        //load a DP from a sample file
        string path_DP = "../../../data/Samples/example_generatePairPPDP_offline/DP-FMPT-from-megara-cb0.txt";
        //string path_DP = "../megarafmpt/data/Samples/example_generatePairPPDP_offline/DP-FMPT-from-megara-cb0.txt"; //Qt only
        string str;
        strReadFromFile(str, path_DP);
        TMotionProgram DP;
        DP.setText(str);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "loading depositioning program: ");
        throw;
    }

    try {
        //load the FMM Instance
        string dir_FMM = "../../../data/Models/MEGARA_FiberMOSModel_Instance";
        //string dir_FMM = "../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"; //Qt only
        readInstanceFromDir(FMM, dir_FMM);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "loading instance: ");
        throw;
    }

    //built a MPV attached to the FMM
    TMotionProgramValidator MPV(&FMM);
}

//overide tearDown(), free allocated memory, etc
void TestMotionProgramValidator::tearDown() {
}

//---------------------------------------------------------------------------

void TestMotionProgramValidator::test_validateMotionProgram()
{
    try {
        //validate the PP
        FMM.RPL.setPurpose(pValPP);
        bool PPvalid = MPV.validateMotionProgram(PP);
        if(!PPvalid) {
            CPPUNIT_ASSERT(false);
            return;
        }
        //validate the DP
        FMM.RPL.setPurpose(pValDP);
        bool DPvalid = MPV.validateMotionProgram(DP);
        if(!DPvalid) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }
    catch(...) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
void TestMotionProgramValidator::test_checkPairPPDP()
{
    try {
        //check the pair (PP, DP)
        bool valid = MPV.checkPairPPDP(PP, DP);
        if(!valid) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }
    catch(...) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
