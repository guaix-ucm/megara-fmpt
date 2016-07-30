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
//File: testExceptions.cpp
//Content: test for the module Exceptions
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testExceptions.h"
#include "../src/Exceptions.h"
//#include "cppunit_assert_emulator.h" //Qt only.

#include <iostream>
#include <limits.h> //INT_MAX
#include <float.h> //DBL_MAX

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestExceptions);

//overide setUp(), init data, etc
void TestExceptions::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestExceptions::tearDown() {
}

//---------------------------------------------------------------------------

void TestExceptions::test_()
{
    //EImproperCall
    try {
        try {
            throw EImproperCall("message 1");
            CPPUNIT_ASSERT(false);
            return;
        }
        catch(Exception& E) {
            E.Message.Insert(1, "message 2: ");
            throw;
        }
        CPPUNIT_ASSERT(false);
        return;
    }
    catch(Exception& E) {
        if(typeid(E) != typeid(EImproperCall)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(E.Message.str != "message 2: message 1") {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //EImproperArgument
    try {
        try {
            throw EImproperArgument("message 1");
            CPPUNIT_ASSERT(false);
            return;
        }
        catch(Exception& E) {
            E.Message.Insert(1, "message 2: ");
            throw;
        }
        CPPUNIT_ASSERT(false);
        return;
    }
    catch(Exception& E) {
        if(typeid(E) != typeid(EImproperArgument)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(E.Message.str != "message 2: message 1") {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //EImproperSetting
    try {
        try {
            throw EImproperSetting("message 1");
            CPPUNIT_ASSERT(false);
            return;
        }
        catch(Exception& E) {
            E.Message.Insert(1, "message 2: ");
            throw;
        }
        CPPUNIT_ASSERT(false);
        return;
    }
    catch(Exception& E) {
        if(typeid(E) != typeid(EImproperSetting)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(E.Message.str != "message 2: message 1") {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //ECantComplete
    try {
        try {
            throw ECantComplete("message 1");
            CPPUNIT_ASSERT(false);
            return;
        }
        catch(Exception& E) {
            E.Message.Insert(1, "message 2: ");
            throw;
        }
        CPPUNIT_ASSERT(false);
        return;
    }
    catch(Exception& E) {
        if(typeid(E) != typeid(ECantComplete)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(E.Message.str != "message 2: message 1") {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //EComplete
    try {
        try {
            throw EComplete("message 1");
            CPPUNIT_ASSERT(false);
            return;
        }
        catch(Exception& E) {
            E.Message.Insert(1, "message 2: ");
            throw;
        }
        CPPUNIT_ASSERT(false);
        return;
    }
    catch(Exception& E) {
        if(typeid(E) != typeid(EComplete)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(E.Message.str != "message 2: message 1") {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //ECantFindSolution
    try {
        try {
            throw ECantFindSolution("message 1");
            CPPUNIT_ASSERT(false);
            return;
        }
        catch(Exception& E) {
            E.Message.Insert(1, "message 2: ");
            throw;
        }
        CPPUNIT_ASSERT(false);
        return;
    }
    catch(Exception& E) {
        if(typeid(E) != typeid(ECantFindSolution)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(E.Message.str != "message 2: message 1") {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //EImproperFileLoadedValue
    try {
        try {
            throw EImproperFileLoadedValue("message 1");
            CPPUNIT_ASSERT(false);
            return;
        }
        catch(Exception& E) {
            E.Message.Insert(1, "message 2: ");
            throw;
        }
        CPPUNIT_ASSERT(false);
        return;
    }
    catch(Exception& E) {
        if(typeid(E) != typeid(EImproperFileLoadedValue)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(E.Message.str != "message 2: message 1") {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //EImpossibleError
    try {
        try {
            throw EImpossibleError("message 1");
            CPPUNIT_ASSERT(false);
            return;
        }
        catch(Exception& E) {
            E.Message.Insert(1, "message 2: ");
            throw;
        }
        CPPUNIT_ASSERT(false);
        return;
    }
    catch(Exception& E) {
        if(typeid(E) != typeid(EImpossibleError)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(E.Message.str != "message 2: message 1") {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    CPPUNIT_ASSERT(true);
}
