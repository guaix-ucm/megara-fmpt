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

//WARNING: for pass the test of FileMethods, the FMM instance
//not hould contains non-significant zeros:
//	wrong: 3.141592653589790
//	wrong: 03.14159265358979
//	wrong: 03.141592653589790
//	right: 3.14159265358979
//If the instance has non-significant zeros, the FMPT will work fine,
//but the test of FileMethods will not be OK.

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
    CPPUNIT_TEST(test_writeInstanceToDir_EAL);
    CPPUNIT_TEST(test_readInstanceFromDir_EAL);
    CPPUNIT_TEST(test_writeInstanceToDir_RPL);
    CPPUNIT_TEST(test_readInstanceFromDir_RPL);
    CPPUNIT_TEST(test_writeInstanceToDir_FMM);
    CPPUNIT_TEST(test_readInstanceFromDir_FMM);

    //Although they can be added several tweets, running make distcheck
    //all test of each class will be counted as single one.

    //finish the process
    CPPUNIT_TEST_SUITE_END();

public:
    //REMEMBER:
    //During execution of tests the release is not installed, but is built
    //a directory './megara-fmpt-3.3.0'.

    //When you execute 'make distcheck', test are executed in the path:
    //  './megara-fmpt-3.3.0/_build/tests'
    //So the data are can be found in the temporal directory:
    //  '../../data'

    //For get a perfect match of regenerated values and loaded values,
    //the floating point values of the instance must be expressed rounded
    //with 14 decimals or less.


    //overide setUp(), init data, etc
    void setUp();
    //overide tearDown(), free allocated memory, etc
    void tearDown();

    //test methods
    void test_readInstanceFromDir_RP();
    void test_writeInstanceToDir_EAL();
    void test_readInstanceFromDir_EAL();
    void test_writeInstanceToDir_RPL();
    void test_readInstanceFromDir_RPL();
    void test_writeInstanceToDir_FMM();
    void test_readInstanceFromDir_FMM();
};

#endif // FMPT_TESTFILEMETHODS_H
