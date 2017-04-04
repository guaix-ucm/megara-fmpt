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
//File: testScalars.cpp
//Content: test for the module Scalars
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testScalars.h"
#include "../src/Constants.h" //M_2PI
#include "../src/Scalars.h"
//#include "cppunit_assert_emulator.h" //Qt only.

#include <cmath> //abs
#include <vector>
//#include <stdlib.h>
#include <limits> //std::numeric_limits

using namespace Mathematics;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestScalars);

//overide setUp(), init data, etc
void TestScalars::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestScalars::tearDown() {
}

//---------------------------------------------------------------------------

void TestScalars::test_ceil()
{
    if(ceil(0.01) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(ceil(-0.99) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }
    CPPUNIT_ASSERT(true);
}

void TestScalars::test_floor()
{
    if(floor(0.99) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(floor(-0.01) != -1) {
        CPPUNIT_ASSERT(false);
        return;
    }
    CPPUNIT_ASSERT(true);
}

void TestScalars::test_Factorial()
{
    try {
        Factorial(-1);
        CPPUNIT_ASSERT(false);
        return;

    } catch(...) {
        //do nothing
    }

    if(Factorial(0) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Factorial(1) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Factorial(2) != 2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Factorial(3) != 6) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
void TestScalars::test_Binomial()
{
    //Binomial(m, n) = Factorial(m)/(Factorial(n)*Factorial(m-n))

    try {
        Binomial(-1, 0);
        CPPUNIT_ASSERT(false);
        return;

    } catch(...) {
        //do nothing
    }

    try {
        Binomial(0, -1);
        CPPUNIT_ASSERT(false);
        return;

    } catch(...) {
        //do nothing
    }

    try {
        Binomial(-1, -1);
        CPPUNIT_ASSERT(false);
        return;

    } catch(...) {
        //do nothing
    }

    if(Binomial(0, 0) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(1, 0) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(1, 1) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(2, 0) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(2, 1) != 2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(2, 2) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(3, 0) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(3, 1) != 3) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(3, 2) != 3) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(3, 3) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(4, 0) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(4, 1) != 4) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(4, 2) != 6) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(4, 3) != 4) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(4, 4) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(5, 0) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(5, 1) != 5) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(5, 2) != 10) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(5, 3) != 10) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(5, 4) != 5) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Binomial(5, 5) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
void TestScalars::test_Log()
{
    //log(a, x) = log(x)/log(a)

    try {
        Log(0, 1);
        CPPUNIT_ASSERT(false);
        return;

    } catch(...) {
        //do nothing
    }

    try {
        Log(1, 0);
        CPPUNIT_ASSERT(false);
        return;

    } catch(...) {
        //do nothing
    }

    try {
        Log(0, 0);
        CPPUNIT_ASSERT(false);
        return;

    } catch(...) {
        //do nothing
    }

    try {
        Log(1, 1);
        CPPUNIT_ASSERT(false);
        return;

    } catch(...) {
        //do nothing
    }

    try {
        Log(1, 2);
        CPPUNIT_ASSERT(false);
        return;

    } catch(...) {
        //do nothing
    }

    if(Log(2, 1) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Log(2, 2) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Log(2, 4) != 2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Log(3, 1) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Log(3, 3) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Log(3, 9) != 2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Log(3, 27) != 3) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
void TestScalars::test_Log2()
{
    try {
        Log2(-1);
        CPPUNIT_ASSERT(false);
        return;

    } catch(...) {
        //do nothing
    }

    try {
        Log2(0);
        CPPUNIT_ASSERT(false);
        return;

    } catch(...) {
        //do nothing
    }

    if(Log2(1) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Log2(2) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Log2(4) != 2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Log2(8) != 3) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
void TestScalars::test_Sign()
{
    if(Sign(-1) != -1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Sign(0) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Sign(1) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
void TestScalars::test_Abs()
{
    if(Abs(-2) != 2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Abs(-1) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Abs(0) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Abs(1) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Abs(2) != 2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void TestScalars::test_min()
{
    if(min(-1, -10) != -10) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(min(-1, 0) != -1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(min(0, -10) != -10) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(min(-1.1, -10.1) != -10.1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(min(-1.1, 0.) != -1.1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(min(0., -10.1) != -10.1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
void TestScalars::test_max()
{
    if(max(-1, -10) != -1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(max(-1, 0) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(max(0, -10) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(max(-1.1, -10.1) != -1.1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(max(-1.1, 0.) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(max(0., -10.1) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void TestScalars::test_LimitDomain()
{
    try {
        LimitDomain(0, 1, -1);
        CPPUNIT_ASSERT(false);
        return;

    } catch(...) {
        //do nothing
    }

    if(LimitDomain(0, -1, 1) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(LimitDomain(-2, -1, 1) != -1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(LimitDomain(2, -1, 1) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    try {
        LimitDomain(0., 1., -1.);
        CPPUNIT_ASSERT(false);
        return;

    } catch(...) {
        //do nothing
    }

    if(LimitDomain(0., -1., 1.) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(LimitDomain(-2., -1., 1.) != -1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(LimitDomain(2., -1., 1.) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void TestScalars::test_Round()
{
    //determine the integer n nearest s.t. [n-0.5 <= x < n+0.5)

    if(Round(-0.501) != -1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Round(-0.5) != -1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Round(0.499) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Round(0.5) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void TestScalars::test_Near()
{
    //Near(x) = Floor(Abs(x))*Sign(x)

    if(Near(-2) != -2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Near(-1.9) != -1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Near(-1) != -1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Near(-0.9) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Near(2) != 2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Near(1.9) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Near(1) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Near(0.9) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
void TestScalars::test_Far()
{
    //Far(x) = ceil(Abs(x))*Sign(x)

    if(Far(-2) != -2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Far(-1.1) != -2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Far(-1) != -1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Far(-0.1) != -1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Far(-0) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Far(2) != 2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Far(1.1) != 2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Far(1) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Far(0.1) != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Far(0) != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
void TestScalars::test_RandomReal()
{
    //genera un número real aleatorio con distribución uniforme en [0, 1]

    vector<double> v;

    for(int i=0; i<1000000; i++) {
        double x = RandomReal();
        v.push_back(x);
        if(x<0 || 1<x) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    bool allequals = true;
    double ave = v[0];
    for(unsigned int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals) {
        CPPUNIT_ASSERT(false);
        return;
    }

    ave = ave/double(v.size());
    if(abs(ave - 0.5) > 0.1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
void TestScalars::test_RandomUniformDouble()
{
    //generate a random real number with uniform distribution in [x1, x2]

    try {
        RandomUniform(1., -1.);
    }
    catch(...) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(RandomUniform(0.5, 0.5) != 0.5) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //-----------------------------------------------------------------

    double x1 = -10;
    double x2 = -5;

    vector<double> v;

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    bool allequals = true;
    double ave = v[0];
    for(unsigned int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals) {
        CPPUNIT_ASSERT(false);
        return;
    }

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //-----------------------------------------------------------------

    x1 = -10;
    x2 = 10;

    v.clear();

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    allequals = true;
    ave = v[0];
    for(unsigned int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals) {
        CPPUNIT_ASSERT(false);
        return;
    }

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //-----------------------------------------------------------------

    x1 = -5;
    x2 = 1;

    v.clear();

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    allequals = true;
    ave = v[0];
    for(unsigned int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals) {
        CPPUNIT_ASSERT(false);
        return;
    }

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //-----------------------------------------------------------------

    x1 = 1;
    x2 = 7;

    v.clear();

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    allequals = true;
    ave = v[0];
    for(unsigned int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals) {
        CPPUNIT_ASSERT(false);
        return;
    }

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //-----------------------------------------------------------------

    CPPUNIT_ASSERT(true);
}
void TestScalars::test_RandomUniformInteger()
{
    //genera un número entero aleatorio con distribución uniforme en [x1, x2]

    try {
        RandomUniform(1, -1);
    }
    catch(...) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(RandomUniform(7, 7) != 7) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //-----------------------------------------------------------------

    int x1 = -10;
    int x2 = -5;

    vector<int> v;

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    bool allequals = true;
    double ave = double(v[0]);
    for(unsigned int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals) {
        CPPUNIT_ASSERT(false);
        return;
    }

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //-----------------------------------------------------------------

    x1 = -10;
    x2 = 10;

    v.clear();

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    allequals = true;
    ave = double(v[0]);
    for(unsigned int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals) {
        CPPUNIT_ASSERT(false);
        return;
    }

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //-----------------------------------------------------------------

    x1 = -5;
    x2 = 1;

    v.clear();

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    allequals = true;
    ave = double(v[0]);
    for(unsigned int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals) {
        CPPUNIT_ASSERT(false);
        return;
    }

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //-----------------------------------------------------------------

    x1 = 1;
    x2 = 7;

    v.clear();

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    allequals = true;
    ave = double(v[0]);
    for(unsigned int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals) {
        CPPUNIT_ASSERT(false);
        return;
    }

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //-----------------------------------------------------------------

    CPPUNIT_ASSERT(true);
}
void TestScalars::test_ArgPos()
{
    //translate theta to the interval [0, M_2PI)
    //a distance multiple of M_2PI

    vector<int> v;

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(-1000000*M_2PI, 1000000*M_2PI);
        x = ArgPos(x);
        v.push_back(x);
        if(x<0 || M_2PI<=x) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    bool all_equals = true;
    for(unsigned int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            all_equals = false;
    }
    if(all_equals) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void TestScalars::test_Numerical()
{
    //determine if a type is numerical

    if(!Numerical(typeid(char))  || !Numerical(typeid(signed char))  ||
            !Numerical(typeid(unsigned char))  ||
            !Numerical(typeid(signed char))  ||
            !Numerical(typeid(int))  || !Numerical(typeid(signed int))  ||
            !Numerical(typeid(unsigned))  || !Numerical(typeid(unsigned int))  ||
            !Numerical(typeid(short))  || !Numerical(typeid(short int))  ||
            !Numerical(typeid(signed short int))  ||
            !Numerical(typeid(unsigned short))  ||
            !Numerical(typeid(unsigned short int))  ||
            !Numerical(typeid(long))  || !Numerical(typeid(long int))  ||
            !Numerical(typeid(signed long int))  ||
            !Numerical(typeid(unsigned long))  || !Numerical(typeid(unsigned long int))  ||
            !Numerical(typeid(float))  || !Numerical(typeid(double))  || !Numerical(typeid(long double))) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(Numerical(typeid(string)) || Numerical(typeid(void)) || Numerical(typeid(vector<int>)) || Numerical(typeid(type_info))) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
void TestScalars::test_FloatingPoint()
{
    //determine if a type is of floating point

    if(!FloatingPoint(typeid(float)) || !FloatingPoint(typeid(double)) || !FloatingPoint(typeid(long double))) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(FloatingPoint(typeid(char))  || FloatingPoint(typeid(signed char))  ||
            FloatingPoint(typeid(unsigned char))  ||
            FloatingPoint(typeid(signed char))  ||
            FloatingPoint(typeid(int))  || FloatingPoint(typeid(signed int))  ||
            FloatingPoint(typeid(unsigned))  || FloatingPoint(typeid(unsigned int))  ||
            FloatingPoint(typeid(short))  || FloatingPoint(typeid(short int))  ||
            FloatingPoint(typeid(signed short int))  ||
            FloatingPoint(typeid(unsigned short))  ||
            FloatingPoint(typeid(unsigned short int))  ||
            FloatingPoint(typeid(long))  || FloatingPoint(typeid(long int))  ||
            FloatingPoint(typeid(signed long int))  ||
            FloatingPoint(typeid(unsigned long))  || FloatingPoint(typeid(unsigned long int))) {
        CPPUNIT_ASSERT(false);
        return;
    }

    if(FloatingPoint(typeid(string)) || FloatingPoint(typeid(void)) || FloatingPoint(typeid(vector<int>)) || FloatingPoint(typeid(type_info))) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

/*void TestScalars::test_frexp10()
{
    //calculates the mantissa and the exponent of a floating point number

    //VALUES UPPER ZERO:

    double x = 314.15926535897932; //17 sig fig
    int exp;
    double mantissa = frexp10(x, exp);
    if(mantissa != 0.31415926535897932 && exp != 3)
        CPPUNIT_ASSERT(false);

    x = 31.415926535897932; //17 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != 0.31415926535897932 && exp != 2)
        CPPUNIT_ASSERT(false);

    x = 3.1415926535897932; //17 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != 0.31415926535897932 && exp != 1)
        CPPUNIT_ASSERT(false);

    x = 1.0000000000000001; //17 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != 0.10000000000000001 && exp != 1)
        CPPUNIT_ASSERT(false);

    x = 1; //1 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != 0.1 && exp != 1)
        CPPUNIT_ASSERT(false);

    x = 0.9999999999999999; //16 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != 0.9999999999999999 && exp != 0)
        CPPUNIT_ASSERT(false);

    x = 0.1; //1 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != 0.1 && exp != 0)
        CPPUNIT_ASSERT(false);

    x = 0.0999999999999999; //15 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != 0.999999999999999 && exp != -1)
        CPPUNIT_ASSERT(false);

    x = 0.0000000000000000000000000000000010000000000000001; //17 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != 0.10000000000000001 && exp != -32)
        CPPUNIT_ASSERT(false);

    x = 0.000000000000000000000000000000001; //1 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != 0.1 && exp != -32)
        CPPUNIT_ASSERT(false);

    //VALUE EQUAL TO ZERO:

    x = 0;
    mantissa = frexp10(x, exp);
    if(mantissa != 0 && exp != 0)
        CPPUNIT_ASSERT(false);

    //VALUES LESS ZERO:

    x = -314.15926535897932; //17 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != -0.31415926535897932 && exp != 3)
        CPPUNIT_ASSERT(false);

    x = -31.415926535897932; //17 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != -0.31415926535897932 && exp != 2)
        CPPUNIT_ASSERT(false);

    x = -3.1415926535897932; //17 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != -0.31415926535897932 && exp != 1)
        CPPUNIT_ASSERT(false);

    x = -1.0000000000000001; //17 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != -0.10000000000000001 && exp != 1)
        CPPUNIT_ASSERT(false);

    x = -1; //1 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != -0.1 && exp != 1)
        CPPUNIT_ASSERT(false);

    x = -0.9999999999999999; //16 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != -0.9999999999999999 && exp != 0)
        CPPUNIT_ASSERT(false);

    x = -0.1; //1 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != -0.1 && exp != 0)
        CPPUNIT_ASSERT(false);

    x = -0.0999999999999999; //15 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != -0.999999999999999 && exp != -1)
        CPPUNIT_ASSERT(false);

    x = -0.0000000000000000000000000000000010000000000000001; //17 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != -0.10000000000000001 && exp != -32)
        CPPUNIT_ASSERT(false);

    x = -0.000000000000000000000000000000001; //1 sig fig
    mantissa = frexp10(x, exp);
    if(mantissa != -0.1 && exp != -32)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}*/
void TestScalars::test_incmin()
{
    //Incremento mínimo de la mantisa de un double:
    //      normalmente la mantisa de un double tiene DSIGNIF+1=54 bits
    //      y su incremento mínimo corresponde a:
    //          1/pow(2., 54.) = 5.55111512312578E-17
    //Incremento mínimo de un double 'x':
    //          pow(2., xexp)/pow(2., 54.)
    //donde xexp es el exponente binario de x.

    //asigna un valor a x
    double x = 3.14159265358979;

    //determina el exponente de x
    int xexp;
    frexp(x, &xexp);
    //calcula el incremento mínimo de x
    double y = pow(2, double(xexp))/pow(2, double(std::numeric_limits<double>::digits+1));

    if(incmin(x) != y) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //asigna un valor a x
    x = 2.718281828182818;

    //determina el exponente de x
    frexp(x, &xexp);
    //calcula el incremento mínimo de x
    y = pow(2, double(xexp))/pow(2, double(std::numeric_limits<double>::digits+1));

    if(incmin(x) != y) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
