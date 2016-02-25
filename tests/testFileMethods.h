// Copyright (c) 2014-2015 Isaac Morales Durán. All rights reserved.
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
//Archivo: testFileMethods.cpp
//Contenido: test para FileMethods
//Última actualización: 12/02/2015
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FMPT_TESTFILEMETHODS_H
#define FMPT_TESTFILEMETHODS_H

#include "../src/FileMethods.h"

using namespace Models;

#include <cppunit/extensions/HelperMacros.h>

//build the class where define the test methods
class TestFileMethods: public CppUnit::TestFixture
{
    //establish the test suit of Test
    CPPUNIT_TEST_SUITE(TestFileMethods);

    //add test methods
    CPPUNIT_TEST(test_readInstanceFromDir_RP);
    CPPUNIT_TEST(test_writeInstanceToDir_RPL);
    CPPUNIT_TEST(test_readInstanceFromDir_RPL);
    CPPUNIT_TEST(test_writeInstanceToDir_EAL);
    CPPUNIT_TEST(test_readInstanceFromDir_EAL);
    CPPUNIT_TEST(test_writeInstanceToDir_FMM);
    CPPUNIT_TEST(test_readInstanceFromDir_FMM);

    //Although they can be added several tweets, running make distcheck
    //all test of each class will be counted as single one.

    //finish the process
    CPPUNIT_TEST_SUITE_END();

public:
    //REMEMBER:
    //When you execute 'make distcheck', test are executed in the path:
    //  .../megarafmpt/megara-fmpt-3.0.7/_build/tests
    //During execution of tests the release is not installed.
    //For get a perfect match of regenerated values and loaded values,
    //the floating point values of the instance must be expressed rounded
    //with 14 decimals or less.


    //overide setUp(), init data, etc
    void setUp();
    //overide tearDown(), free allocated memory, etc
    void tearDown();

protected:

    //test methods for functions of the FileMethods:

    void test_readInstanceFromDir_RP();

    void test_writeInstanceToDir_RPL();
    void test_readInstanceFromDir_RPL();

    void test_writeInstanceToDir_EAL();
    void test_readInstanceFromDir_EAL();

    void test_writeInstanceToDir_FMM();
    void test_readInstanceFromDir_FMM();
/*
    void test_writeInstanceToDir_FCM();
    void test_readInstanceFromDir_FCM();*/
};

#endif // FMPT_TESTFILEMETHODS_H
