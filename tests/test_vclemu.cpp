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
//File: test_vclemu.cpp
//Content: test for the module vclemu
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "test_vclemu.h"
#include "../src/vclemu.h"

#include <iostream>

//using namespace std;

void test_get_decimal_separator()
{
    setlocale(LC_NUMERIC, "C");
    char c = get_decimal_separator();

    if(c != '.') {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void test_random()
{
    int x;
    int min = std::numeric_limits<int>::max();
    int max = -std::numeric_limits<int>::max();

    for(int i=0; i<1000; i++) {
        x = random(10);

        //chek if the number is in the interval [0, 1000)
        if(x<0 || 1000<=x)
            cout << "test_random fail 1" << endl;

        //actualize the limits
        if(x < min)
            min = x;
        if(x > max)
            max = x;
    }

    if(min != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(max != 9) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void test_AnsiString()
{
    //build an AnsiString by default
    AnsiString S1;

    //build an AnsiString from a char
    AnsiString S2('m');
    if(S2.str != "m") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //build an AnsiString from a c string
    string str = "hola mundo";
    AnsiString S3(str.c_str());
    if(S3.str != "hola mundo") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //build an AnsiString from a string
    AnsiString S4(str);
    if(S4.str != "hola mundo") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //build an AnsiString from an integer
    int n = std::numeric_limits<int>::max();
    AnsiString S5(n);
    if(S5.str != "2147483647") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //build an AnsiString from a double
    double x = std::numeric_limits<double>::max();
    AnsiString S6(x);
    if(S6.str != "1.79769313486232e+308") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //concatenate two AnsiStrings
    AnsiString S7 = S2 + S3 + S6;
    if(S7.str != "mhola mundo1.79769313486232e+308") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(S2.str != "m") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(S3.str != "hola mundo") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(S6.str != "1.79769313486232e+308") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //concatenate an AnsiString and a string
    AnsiString S8 = S2 + str;
    if(S8.str != "mhola mundo") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(S2.str != "m") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(str != "hola mundo") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //concatenate an AnsiString to this AnsiString
    S2 += S3;
    if(S2.str != "mhola mundo") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(S3.str != "hola mundo") {
        CPPUNIT_ASSERT(false);
        return;
    }

    /*
    //concatenate a char to this AnsiString
    AnsiString& operator+=(const char&);
    //copy an AnsiString
    AnsiString& operator=(const AnsiString&);
    //copy a string
    AnsiString& operator=(const string& str);
    //copy a c string (not add it, copy it)
    AnsiString& operator=(const char*);
    //determine if an AnsiString is equal to this AnsiString
    bool operator==(const AnsiString&) const;
    //determine if an AnsiString is unequal to this AnsiString
    bool operator!=(const AnsiString&) const;
  */

    CPPUNIT_ASSERT(true);
}
