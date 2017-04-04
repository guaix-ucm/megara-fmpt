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
//File: testStrPR.cpp
//Content: test for the module StrPR
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testStrPR.h"
#include "../src/Constants.h" //M_2PI
//#include "cppunit_assert_emulator.h" //Qt only.

#include <cmath> //abs
#include <vector>
//#include <stdlib.h>
#include <limits> //std::numeric_limits

#include "../src/StrPR.h"

using namespace Strings;
//using namespace Models;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestStrPR);

//overide setUp(), init data, etc
void TestStrPR::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestStrPR::tearDown() {
}

//---------------------------------------------------------------------------

void TestStrPR::test_Text()
{
    //---------------------------------------------------------------------------
    //Funciones para texto:

    //intenta leer una palabra desde la posición i de una cadena de texto
    AnsiString S = "hello word";
    AnsiString Word;
    int i = 6;
    StrReadWord(Word, S, i);
    if(Word != "word" || S != "hello word" || i != 11) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //intenta leer una cadena desde la posición i de una cadena de texto
    //incluyendo las comillas simples delimitadoras
    S = " 'hello word'";
    AnsiString String;
    i = 1;
    StrReadString(String, S, i);
    if(String != "'hello word'" || S != " 'hello word'" || i != 14) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //intenta leer una cadena desde la posición i de una cadena de texto
    //que se encuentra delimitada por dos caracteres determinados
    i = 1;
    S = " \"hello word\"";
    char c1 = '\"';
    char c2 = '\"';
    String = "";
    StrReadStringBetweenChars(String, S, i, c1, c2);
    if(String != "hello word" || S != " \"hello word\"" || i != 14 || c1 != '\"' || c2 != '\"') {
        CPPUNIT_ASSERT(false);
        return;
    }

    //traduce una cadena entre dos caracteres a la misma cadena sin los caracteres
    S = " \"hello word\"";
    c1 = '\"';
    c2 = '\"';
    String = "";
    String = StrBetweenCharsToStr(S, c1, c2);
    if(String != "hello word" || S != " \"hello word\"" || c1 != '\"' || c2 != '\"') {
        CPPUNIT_ASSERT(false);
        return;
    }

    //lee una subcadena SubS desde la posición indicada de una cadena S
    //hasta el caracter delimitador (d) que se indique
    char d = 'd';
    i = 1;
    S = "hello word";
    AnsiString SubS;
    StrReadStringToChar(SubS, S, i, d);
    if(SubS != "hello wor" || S != "hello word" || i != 10 || d != 'd') {
        CPPUNIT_ASSERT(false);
        return;
    }

    //intenta recorrer una cadena de texto (entre comillas simples)
    //desde la posición i de una cadena de texto
    S = "  'hello word'";
    i = 1;
    StrTravelString(S, i);
    if(S != "  'hello word'" || i != 14) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //intenta leer una etiqueta desde
    //la posición indicada de una cadena de texto
    i = 6;
    S = "hello word";
    AnsiString Label = "word";
    AnsiString Reading;
    StrReadLabel(Reading, Label, S, i);
    if(Reading != "word" || Label != "word" || S != "hello word" || i != 11) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //recorre una etiqueta a partir de
    //la posición indicada de una cadena de texto
    i = 6;
    S = "hello word";
    Label = "word";
    StrTravelLabel(Label, S, i);
    if(Label != "word" || S != "hello word" || i != 11) {
        CPPUNIT_ASSERT(false);
        return;
    }

    /*//trabel a label from
//the indicated position of a string
//if it is not possible travel all lavel, return false
bool strTravelLabel_(const string& label, const string& str, unsigned int& i);

//trabel a label from
//the indicated position of a string
//if it is not possible travel all lavel, return false
void strTravelLabel(const string& label, const string& str, unsigned int& i);

//ADVERTENCIA: las funciones de lectura de etiquetas no pueden ser empleadas
//con cadenas que empiecen por caracteres separadores.

//busca una sentencia de asignación a partir de
//la posición indicada de una cadena de texto
//void StrReadAssign(AnsiString &Ident, const AnsiString &S, int &i);
//lee de S una cadena hasta que encuentra ';'
//void StrReadSentence(AnsiString &Sentence, const AnsiString &S, int &i);

//segrega la subcadena desde la posición indicada hasta el final
//si encuentra un caracter no separador lanza una excepción EImproperArgument
//i quedará indicando a la posúltima posición
//void StrReadToEnd(AnsiString &SubS, const AnsiString &S, int &i);
//atraviesa la subcadena desde la posición indicada hasta el final
//si encuentra un caracter no separador lanza una excepción EImproperArgument
//i quedará indicando a la posúltima posición
void StrTravelToEnd(const AnsiString&, int&);
//recorre una cadena desde la posición indicada
//hasta que encuentra un caracter no separador
void StrTravelSeparatorsIfAny(const AnsiString&, int&);
//atraviesa uno o más separadores
void StrTravelSeparators(const AnsiString&, int&);

//travel one or more separators
void strTravelSeparators(const string& str, unsigned int& i);

//travel string from the indicated position
//to find a nonseparator character
void strTravelSeparatorsIfAny(const string&, unsigned int&);
*/

    CPPUNIT_ASSERT(true);
}

void TestStrPR::test_Conversion()
{
    //---------------------------------------------------------------------------
    //CONVERSION FUNCTIONS:


    //WARNING: conversion functions bassed in sscanf implies:
    //- conversión is stoped when a unrecognizable figure is found,
    //  and it is not throug an exception;
    //- too large numbers are converted to larg primitive types,
    //  and it is truncated its most significant part of longint.

    //read an integer in the indicated position of a string
    unsigned int i = 0;
    string dst;
    strReadIntStr(dst, "2147483647", i);
    if(dst != "2147483647" || i != 10) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //translate from string to int
    int n = strToInt("2147483647");
    if(n != 2147483647) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //read an integer in the indicated position of a string
    i = 0;
    n = 0;
    strReadInt(n, "2147483647", i);
    if(n != 2147483647 || i != 10) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //read an integer in the indicated position of an AnsiString
    int i_ = 1;
    AnsiString D;
    StrReadIntStr(D, "2147483647", i_);
    if(D != "2147483647" || i_ != 11) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //read an integer in the indicated position of an AnsiString
    i_ = 1;
    n = 0;
    StrReadInt(n, "2147483647", i_);
    if(n != 2147483647 || i_ != 11) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //read an integer in the indicated position of a string
    //in hexadecimal format
    i = 0;
    dst = "";
    strReadHexStr(dst, "7fffffff", i);
    if(dst != "7fffffff" || i != 8) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //translate from string to int
    //in hexadecimal format
    n = hexToInt("7fffffff");
    if(n != 0x7fffffff) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //read an integer in the indicated position of a string
    //in hexadecimal format
    i = 0;
    n = 0;
    strReadHex(n, "7fffffff", i);
    if(n != 0x7fffffff || i != 8) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //read an integer in the indicated position of an AnsiString
    //in hexadecimal format
    i_ = 1;
    D = "";
    StrReadHexStr(D, "7fffffff", i_);
    if(D != "7fffffff" || i_ != 9) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //read an integer in the indicated position of an AnsiString
    //in hexadecimal format
    i_ = 1;
    n = 0;
    StrReadHex(n, "7fffffff", i_);
    if(n != 0x7fffffff || i_ != 9) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //read a float in the indicated position of a string
    i = 0;
    dst = "";
    strReadFloatStr(dst, "1.7976931348623158E+308", i);
    if(dst != "1.7976931348623158E+308" || i != 23) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //translate from string to double
    double x = strToFloat("1.7976931348623158E+308");
    if(x != 1.7976931348623158E+308) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //translate from string to double
    x = strToFloat("-4.74");
    if(x != -4.74) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //read a float in the indicated position of a string
    i = 0;
    x = 0;
    strReadFloat(x, "1.7976931348623158E+308", i);
    if(x != 1.7976931348623158E+308 || i != 23) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //print a double in an AnsiString
    x = 1.7976931348623158E+308;
    AnsiString S;
    StrPrintFloat(S, x);
    if(S != "1.79769313486232e+308" || x != 1.7976931348623158E+308) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //read a float in the indicated position of an AnsiString
    i_ = 1;
    D = "";
    StrReadFloatStr(D, "1.7976931348623158E+308", i_);
    if(D != "1.7976931348623158E+308" || i_ != 24) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //read a float in the indicated position of an AnsiString
    i_ = 1;
    x = 0;
    StrReadFloat(x, "1.7976931348623158E+308", i_);
    if(x != 1.7976931348623158E+308 || i_ != 24) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //read a bool in the indicated position of a string
    i = 0;
    dst = "";
    strReadBoolStr(dst, "1", i);
    if(dst != "1" || i != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //translate from string to bool
    bool b = strToBool("1");
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //read a bool in the indicated position of a string
    i = 0;
    b = false;
    strReadBool(b, "1", i);
    if(b != true || i != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //print a bool in an AnsiString
    S = "";
    StrPrintBool(S, true);
    if(S != "1") {
        CPPUNIT_ASSERT(false);
        return;
    }
    //read a bool in the indicated position of an AnsiString
    i_ = 1;
    D = "";
    StrReadBoolStr(D, "1", i_);
    if(D != "1" || i_ != 2) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //read a bool in the indicated position of an AnsiString
    i_ = 1;
    b = false;
    StrReadBool(b, "1", i_);
    if(b != true || i_ != 2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //imprime un punto en una cadena de texto
    S = "";
    StrPrintPoint(S, TPoint(-2147483647, 2147483647));
    if(S != "{-2147483647, 2147483647}") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //compara dos punto
    //valores devueltos:
    //      -1: si x1<x2 || (x1==x2 && y1<y2)
    //      1: si x1>x2 || (x1==x2 && y1>y2)
    //      0: si x1==x2 && y1==y2
    TDoublePoint P1(1, 1);
    TDoublePoint P2(0, 0);
    int result = CompareDPoints(&P1, &P2);
    if(result <= 0) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P1 = TDoublePoint(0, 1);
    result = CompareDPoints(&P1, &P2);
    if(result <= 0) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P1 = TDoublePoint(1, 0);
    result = CompareDPoints(&P1, &P2);
    if(result <= 0) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P1 = TDoublePoint(0, 0);
    result = CompareDPoints(&P1, &P2);
    if(result != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P1 = TDoublePoint(-1, -1);
    result = CompareDPoints(&P1, &P2);
    if(result >= 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //imprime un punto al final de una cadena de texto
    TDoublePoint P(1.7976931348623158E+308, 1.7976931348623158E+308);
    S = "";
    StrPrintDPoint(S, &P);
    if(S != "{1.79769313486232e+308, 1.79769313486232e+308}") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //intenta leer un punto a partir de la posición i de una cadena de texto
    //e intenta convertirlo a TDoublePoint mediante StrToFloat
    i_ = 1;
    S = "{-1.7976931348623158E+308, 1.7976931348623158E+308}";
    P = TDoublePoint(0, 0);
    StrReadDPoint(&P, S, i_);
    if(P.x != -1.7976931348623157E+308 || P.y != 1.7976931348623157E+308) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //convierte un valor punto de formato texto a TDoublePoint
    //si S contiene algo más que un valor punto
    //lanza una excepción EImproperArgument
    S = "{-1.7976931348623158E+308, 1.7976931348623158E+308}";
    P = StrToDPoint(S);
    if(P.x != -1.7976931348623157E+308 || P.y != 1.7976931348623157E+308) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //intenta leer un rectángulo a partir de la posición i de una cadena de texto
    //e intenta convertirlo a TDoubleRect mediante StrToFloat
    i_ = 1;
    S = "{-1.5, -1.1, 1.7, 0.8}";
    TDoubleRect R;
    StrReadDRect(&R, S, i_);
    if(R.Left != -1.5 || R.Right != 1.7 || R.Bottom != -1.1 || R.Top != 0.8) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //convierte un valor rectángulo de formato texto a TDoubleRect
    //si S contiene algo más que un valor rectángulo
    //lanza una excepción EImproperArgument
    R = StrToDRect("{-1.5, -1.1, 1.7, 0.8}");
    if(R.Left != -1.5 || R.Right != 1.7 || R.Bottom != -1.1 || R.Top != 0.8) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //ADVERTENCIA: para incorporar las funciones de impresión y lectura de
    //TDoublePoint a dicha estructura, habría que trasladar dicha estrucutar
    //a una posición posterior a la declaración de FloatToStr.

    //imprime dos enteros en una cadena indentados a 12 espacios
    S = IntToRowStr(-2147483647, 2147483647);
    if(S != "-2147483647 2147483647") {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

