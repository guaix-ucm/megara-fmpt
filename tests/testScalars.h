#ifndef FMPT_TESTSCALARS_H
#define FMPT_TESTSCALARS_H

#include <cppunit/extensions/HelperMacros.h>

//build the class where define the test methods
class TestScalars: public CppUnit::TestFixture
{
    //establish the test suit of Test
    CPPUNIT_TEST_SUITE(TestScalars);

    //add test methods
    CPPUNIT_TEST(testFactorial);
    CPPUNIT_TEST(testBinomial);
    CPPUNIT_TEST(testLog);
    CPPUNIT_TEST(testLog2);
    CPPUNIT_TEST(testSign);
    CPPUNIT_TEST(testAbs);

    CPPUNIT_TEST(testMin);
    CPPUNIT_TEST(testMax);

    //finish the process
    CPPUNIT_TEST_SUITE_END();

public:
    //overide setUp(), init data, etc
    void setUp();
    //overide tearDown(), free allocated memory, etc
    void tearDown();

protected:
    void testFactorial();
    void testBinomial();
    void testLog();
    void testLog2();
    void testSign();
    void testAbs();

    void testMin();
    void testMax();

    void testLimitDomain();

    void testRound();
    void testNear();
    void testFar();

    void testRandomReal();
    void testRandomUniformDouble();
    void testRandomUniformInteger();
    void testArgPos();

    void testNumerical();
    void testFloatingPoint();

    //---------------------------------------------------------------------------
    //Funciones de manipulación de tipos numéricos:
/*
    //determina la mantisa y el exponente en base 10
    double frexp10(double x, int& exponent10);
    //determina el nº de cifras decimales significativas de x en base 10
    int nsigfig(double x);
    //determina el incremento mínimo de un double
    //correspondiente a incrementar su mantisa una unidad
    double incmin(double x);

    //Incremento mínimo de la mantisa de un double:
    //      normalmente la mantisa de un double tiene DSIGNIF+1=54 bits
    //      y su incremento mínimo corresponde a:
    //          1/pow(2., 54.) = 5.55111512312578E-17
    //Incremento mínimo de un double 'x':
    //          pow(2., xexp)/pow(2., 54.)
    //donde xexp es el exponente binario de x.*/
};

#endif // FMPT_TESTSCALARS_H
