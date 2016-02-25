
#include "testScalars.h"
#include "../src/Constants.h" //M_2PI

#include <cmath> //abs
#include <vector>

#include "../src/Scalars.h"

using namespace Mathematics;

CPPUNIT_TEST_SUITE_REGISTRATION(TestScalars);

//overide setUp(), init data, etc
void TestScalars::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestScalars::tearDown() {
}

//---------------------------------------------------------------------------

void TestScalars::testFactorial()
{
    try {
        Factorial(-1);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    if(Factorial(0) != 1)
        CPPUNIT_ASSERT(false);

    if(Factorial(1) != 1)
        CPPUNIT_ASSERT(false);

    if(Factorial(2) != 2)
        CPPUNIT_ASSERT(false);

    if(Factorial(3) != 6)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testBinomial()
{
    //Binomial(m, n) = Factorial(m)/(Factorial(n)*Factorial(m-n))

    try {
        Binomial(-1, 0);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    try {
        Binomial(0, -1);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    try {
        Binomial(-1, -1);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    if(Binomial(0, 0) != 1)
        CPPUNIT_ASSERT(false);

    if(Binomial(1, 0) != 1)
        CPPUNIT_ASSERT(false);
    if(Binomial(1, 1) != 1)
        CPPUNIT_ASSERT(false);

    if(Binomial(2, 0) != 1)
        CPPUNIT_ASSERT(false);
    if(Binomial(2, 1) != 2)
        CPPUNIT_ASSERT(false);
    if(Binomial(2, 2) != 1)
        CPPUNIT_ASSERT(false);

    if(Binomial(3, 0) != 1)
        CPPUNIT_ASSERT(false);
    if(Binomial(3, 1) != 3)
        CPPUNIT_ASSERT(false);
    if(Binomial(3, 2) != 3)
        CPPUNIT_ASSERT(false);
    if(Binomial(3, 3) != 1)
        CPPUNIT_ASSERT(false);

    if(Binomial(4, 0) != 1)
        CPPUNIT_ASSERT(false);
    if(Binomial(4, 1) != 4)
        CPPUNIT_ASSERT(false);
    if(Binomial(4, 2) != 6)
        CPPUNIT_ASSERT(false);
    if(Binomial(4, 3) != 4)
        CPPUNIT_ASSERT(false);
    if(Binomial(4, 4) != 1)
        CPPUNIT_ASSERT(false);

    if(Binomial(5, 0) != 1)
        CPPUNIT_ASSERT(false);
    if(Binomial(5, 1) != 5)
        CPPUNIT_ASSERT(false);
    if(Binomial(5, 2) != 10)
        CPPUNIT_ASSERT(false);
    if(Binomial(5, 3) != 10)
        CPPUNIT_ASSERT(false);
    if(Binomial(5, 4) != 5)
        CPPUNIT_ASSERT(false);
    if(Binomial(5, 5) != 1)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testLog()
{
    //log(a, x) = log(x)/log(a)

    try {
        Log(0, 1);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }
    try {
        Log(1, 0);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }
    try {
        Log(0, 0);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    try {
        Log(1, 1);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }
    try {
        Log(1, 2);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    if(Log(2, 1) != 0)
        CPPUNIT_ASSERT(false);
    if(Log(2, 2) != 1)
        CPPUNIT_ASSERT(false);
    if(Log(2, 4) != 2)
        CPPUNIT_ASSERT(false);

    if(Log(3, 1) != 0)
        CPPUNIT_ASSERT(false);
    if(Log(3, 3) != 1)
        CPPUNIT_ASSERT(false);
    if(Log(3, 9) != 2)
        CPPUNIT_ASSERT(false);
    if(Log(3, 27) != 3)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testLog2()
{
    try {
        Log2(-1);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }
    try {
        Log2(0);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    if(Log2(1) != 0)
        CPPUNIT_ASSERT(false);
    if(Log2(2) != 1)
        CPPUNIT_ASSERT(false);
    if(Log2(4) != 2)
        CPPUNIT_ASSERT(false);
    if(Log2(8) != 3)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testSign()
{
    if(Sign(-1) != -1)
        CPPUNIT_ASSERT(false);

    if(Sign(0) != 1)
        CPPUNIT_ASSERT(false);

    if(Sign(1) != 1)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testAbs()
{
    if(Abs(-2) != 2)
        CPPUNIT_ASSERT(false);
    if(Abs(-1) != 1)
        CPPUNIT_ASSERT(false);
    if(Abs(0) != 0)
        CPPUNIT_ASSERT(false);
    if(Abs(1) != 1)
        CPPUNIT_ASSERT(false);
    if(Abs(2) != 2)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}

void TestScalars::testMin()
{
    if(Min(-1, -10) != -10)
        CPPUNIT_ASSERT(false);
    if(Min(-1, 0) != -1)
        CPPUNIT_ASSERT(false);
    if(Min(0, -10) != -10)
        CPPUNIT_ASSERT(false);

    if(Min(-1.1, -10.1) != -10.1)
        CPPUNIT_ASSERT(false);
    if(Min(-1.1, 0.) != -1.1)
        CPPUNIT_ASSERT(false);
    if(Min(0., -10.1) != -10.1)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testMax()
{
    if(Max(-1, -10) != -1)
        CPPUNIT_ASSERT(false);
    if(Max(-1, 0) != 0)
        CPPUNIT_ASSERT(false);
    if(Max(0, -10) != 0)
        CPPUNIT_ASSERT(false);

    if(Max(-1.1, -10.1) != -1.1)
        CPPUNIT_ASSERT(false);
    if(Max(-1.1, 0.) != 0)
        CPPUNIT_ASSERT(false);
    if(Max(0., -10.1) != 0)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}

void TestScalars::testLimitDomain()
{
    try {
        LimitDomain(0, 1, -1);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    if(LimitDomain(0, -1, 1) != 0)
        CPPUNIT_ASSERT(false);

    if(LimitDomain(-2, -1, 1) != -1)
        CPPUNIT_ASSERT(false);

    if(LimitDomain(2, -1, 1) != 1)
        CPPUNIT_ASSERT(false);

    try {
        LimitDomain(0., 1., -1.);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    if(LimitDomain(0., -1., 1.) != 0)
        CPPUNIT_ASSERT(false);

    if(LimitDomain(-2., -1., 1.) != -1)
        CPPUNIT_ASSERT(false);

    if(LimitDomain(2., -1., 1.) != 1)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}

void TestScalars::testRound()
{
    //determina el entero n más próximo tq [n-0.5 <= x < n+0.5)

    if(Round(-0.501) != -1)
        CPPUNIT_ASSERT(false);

    if(Round(-0.5) != 0)
        CPPUNIT_ASSERT(false);

    if(Round(0.499) != 0)
        CPPUNIT_ASSERT(false);

    if(Round(0.5) != 1)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}

void TestScalars::testNear()
{
    //      Near(x) = Floor(Abs(x))*Sign(x)

    if(Near(-2) != 2)
        CPPUNIT_ASSERT(false);

    if(Near(-1.9) != 1)
        CPPUNIT_ASSERT(false);

    if(Near(-1) != 1)
        CPPUNIT_ASSERT(false);

    if(Near(-0.9) != 0)
        CPPUNIT_ASSERT(false);

    if(Near(2) != 2)
        CPPUNIT_ASSERT(false);

    if(Near(1.9) != 1)
        CPPUNIT_ASSERT(false);

    if(Near(1) != 1)
        CPPUNIT_ASSERT(false);

    if(Near(0.9) != 0)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testFar()
{
    //      Far(x) = Ceil(Abs(x))*Sign(x)

    if(Far(-2) != 2)
        CPPUNIT_ASSERT(false);

    if(Far(-1.1) != 2)
        CPPUNIT_ASSERT(false);

    if(Far(-1) != 1)
        CPPUNIT_ASSERT(false);

    if(Far(-0.1) != 1)
        CPPUNIT_ASSERT(false);

    if(Far(-0) != 0)
        CPPUNIT_ASSERT(false);

    if(Far(2) != 2)
        CPPUNIT_ASSERT(false);

    if(Far(1.1) != 2)
        CPPUNIT_ASSERT(false);

    if(Far(1) != 1)
        CPPUNIT_ASSERT(false);

    if(Far(0.1) != 1)
        CPPUNIT_ASSERT(false);

    if(Far(0) != 0)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testRandomReal()
{
    //genera un número real aleatorio con distribución uniforme en [0, 1]

    vector<double> v;

    for(int i=0; i<1000000; i++) {
        double x = RandomReal();
        v.push_back(x);
        if(x<0 || 1<x)
            CPPUNIT_ASSERT(false);
    }

    bool allequals = true;
    double ave = v[0];
    for(int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals)
        CPPUNIT_ASSERT(false);

    ave = ave/double(v.size());
    if(abs(ave - 0.5) > 0.1)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testRandomUniformDouble()
{
    //genera un número real aleatorio con distribución uniforme en [x1, x2]

    try {
        RandomUniform(1., -1.);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    try {
        RandomUniform(0., 0.);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    //-----------------------------------------------------------------

    double x1 = -10;
    double x2 = -5;

    vector<double> v;

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x)
            CPPUNIT_ASSERT(false);
    }

    bool allequals = true;
    double ave = v[0];
    for(int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals)
        CPPUNIT_ASSERT(false);

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1))
        CPPUNIT_ASSERT(false);

    //-----------------------------------------------------------------

    x1 = -10;
    x2 = 10;

    v.clear();

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x)
            CPPUNIT_ASSERT(false);
    }

    allequals = true;
    ave = v[0];
    for(int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals)
        CPPUNIT_ASSERT(false);

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1))
        CPPUNIT_ASSERT(false);

    //-----------------------------------------------------------------

    x1 = -5;
    x2 = 1;

    v.clear();

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x)
            CPPUNIT_ASSERT(false);
    }

    allequals = true;
    ave = v[0];
    for(int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals)
        CPPUNIT_ASSERT(false);

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1))
        CPPUNIT_ASSERT(false);

    //-----------------------------------------------------------------

    x1 = 1;
    x2 = 7;

    v.clear();

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x)
            CPPUNIT_ASSERT(false);
    }

    allequals = true;
    ave = v[0];
    for(int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals)
        CPPUNIT_ASSERT(false);

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1))
        CPPUNIT_ASSERT(false);

    //-----------------------------------------------------------------

    CPPUNIT_ASSERT(true);
}
void TestScalars::testRandomUniformInteger()
{
    //genera un número entero aleatorio con distribución uniforme en [x1, x2]

    try {
        RandomUniform(1, -1);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    try {
        RandomUniform(0, 0);
        CPPUNIT_ASSERT(false);
    } catch(...) {
        //do nothing
    }

    //-----------------------------------------------------------------

    int x1 = -10;
    int x2 = -5;

    vector<int> v;

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x)
            CPPUNIT_ASSERT(false);
    }

    bool allequals = true;
    double ave = double(v[0]);
    for(int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals)
        CPPUNIT_ASSERT(false);

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1))
        CPPUNIT_ASSERT(false);

    //-----------------------------------------------------------------

    x1 = -10;
    x2 = 10;

    v.clear();

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x)
            CPPUNIT_ASSERT(false);
    }

    allequals = true;
    ave = double(v[0]);
    for(int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals)
        CPPUNIT_ASSERT(false);

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1))
        CPPUNIT_ASSERT(false);

    //-----------------------------------------------------------------

    x1 = -5;
    x2 = 1;

    v.clear();

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x)
            CPPUNIT_ASSERT(false);
    }

    allequals = true;
    ave = double(v[0]);
    for(int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals)
        CPPUNIT_ASSERT(false);

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1))
        CPPUNIT_ASSERT(false);

    //-----------------------------------------------------------------

    x1 = 1;
    x2 = 7;

    v.clear();

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(x1, x2);
        v.push_back(x);
        if(x<x1 || x2<x)
            CPPUNIT_ASSERT(false);
    }

    allequals = true;
    ave = double(v[0]);
    for(int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
        ave += v[i];
    }
    if(allequals)
        CPPUNIT_ASSERT(false);

    ave = ave/double(v.size());
    if(abs(ave - (x1 + x2)/2.) > 0.1*(x2 - x1))
        CPPUNIT_ASSERT(false);

    //-----------------------------------------------------------------

    CPPUNIT_ASSERT(true);
}
void TestScalars::testArgPos()
{
    //desplaza theta al intervalo [0, M_2PI)
    //una distancia múltiplo de M_2PI

    vector<int> v;

    for(int i=0; i<1000000; i++) {
        double x = RandomUniform(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
        x = ArgPos(x);
        v.push_back(x);
        if(x<0 || M_2PI<=x)
            CPPUNIT_ASSERT(false);
    }

    bool allequals = true;
    for(int i=1; i<v.size(); i++) {
        if(v[0] != v[i])
            allequals = false;
    }
    if(allequals)
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}

void TestScalars::testNumerical()
{
    //determina si un tipo es numérico

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
            !Numerical(typeid(float))  || !Numerical(typeid(double))  || !Numerical(typeid(long double)))
        CPPUNIT_ASSERT(false);

    if(Numerical(typeid(string)) || Numerical(typeid(void)) || Numerical(typeid(vector<int>)) || Numerical(typeid(type_info)))
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
void TestScalars::testFloatingPoint()
{
    //determina si un tipo es de punto flotante

    if(!FloatingPoint(typeid(float)) || !FloatingPoint(typeid(double)) || !FloatingPoint(typeid(long double)))
        CPPUNIT_ASSERT(false);

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
            FloatingPoint(typeid(unsigned long))  || FloatingPoint(typeid(unsigned long int)))
        CPPUNIT_ASSERT(false);

    if(FloatingPoint(typeid(string)) || FloatingPoint(typeid(void)) || FloatingPoint(typeid(vector<int>)) || FloatingPoint(typeid(type_info)))
        CPPUNIT_ASSERT(false);

    CPPUNIT_ASSERT(true);
}
