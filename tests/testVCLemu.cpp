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
//File: testVCLemu.cpp
//Content: test for the module VCLemu
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testVCLemu.h"
#include "../src/VCLemu.h"
//#include "cppunit_assert_emulator.h" //Qt only.

#include <iostream>
#include <limits.h> //INT_MAX
#include <float.h> //DBL_MAX

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestVCLemu);

//overide setUp(), init data, etc
void TestVCLemu::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestVCLemu::tearDown() {
}

//---------------------------------------------------------------------------

void TestVCLemu::test_get_decimal_separator()
{
    setlocale(LC_NUMERIC, "C");
    char c = get_decimal_separator();

    if(c != '.') {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void TestVCLemu::test_random()
{
    int x;
    int min = INT_MAX;
    int max = -INT_MAX;

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

void TestVCLemu::test_AnsiString()
{
    //-----------------------------------------------------------------------

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
    int n = 2147483647;
    AnsiString S5(n);
    if(S5.str != "2147483647") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //build an AnsiString from a double
    double x = 1.7976931348623158E+308;
    AnsiString S6(x);
    if(S6.str != "1.79769313486232e+308") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //-----------------------------------------------------------------------

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


    //concatenate a char to this AnsiString
    S2 += 'c';
    if(S2.str != "mhola mundoc") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //copy an AnsiString
    S2 = S3;
    if(S2.str != "hola mundo") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //copy a string
    S2 = "sociologist are antropologist that understand mathematics";
    if(S2.str != "sociologist are antropologist that understand mathematics") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //copy a c string (not add it, copy it)
    S2 = str.c_str();
    if(S2.str != "hola mundo") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(str != "hola mundo") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //determine if an AnsiString is equal to this AnsiString
    if(S1 == S2) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(S1.str != "") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(S2.str != "hola mundo") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //determine if an AnsiString is unequal to this AnsiString
    if(S2 != S3) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(S2.str != "hola mundo") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(S3.str != "hola mundo") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //-----------------------------------------------------------------------

    //access to the indexed char in non constant AnsiStrings
    try {
        S1[0];
        CPPUNIT_ASSERT(false);
        return;
    } catch(...) {
        //do nothing
    }
    try {
        S1[1];
        CPPUNIT_ASSERT(false);
        return;
    } catch(...) {
        //do nothing
    }
    try {
        S2[11];
        CPPUNIT_ASSERT(false);
        return;
    } catch(...) {
        //do nothing
    }
    if(S2[1] != 'h') {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(S2[5] != ' ') {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(S2[6] != 'm') {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(S2[10] != 'o') {
        CPPUNIT_ASSERT(false);
        return;
    }
    S2[10] = 'i';
    if(S2[10] != 'i') {
        CPPUNIT_ASSERT(false);
        return;
    }

    //access to the indexed char in constant AnsiStrings
    const AnsiString S0("bit");
    if(S0[2] != 'i') {
        CPPUNIT_ASSERT(false);
        return;
    }

    //-----------------------------------------------------------------------

    //get a pointer to the first char of the AnsiString
    if(S0.c_str() != S0.str.c_str()) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //get the number of chars of the AnsiString
    if(S0.Length() != (int)S0.str.length()) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(S1.Length() != (int)S1.str.length()) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(S2.Length() != (int)S2.str.length()) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //set the length of the AnsiString
    S1.SetLength(5);
    if(S1.str.length() != 5) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //insert a char in a position of the string
    //if index i not indicate a position in the string S
    //  throw an exception EImproperArgument
    S2.Insert(5, 's');
    if(S2.str != "holas mundi") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //insert a substring in a position of the string
    S2.Insert(1, "esto es ");
    if(S2.str != "esto es holas mundi") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //get the substring in the interval [offset, offset+count]
    //if offset or offset+count not indicates a position in the AnsiString:
    //  throw EImproperArgument
    if(S3.SubString(6, 5).str != "mundo") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //convetrt the string to double
    S1 = "1.7976931348623158E+308";
    if(S1.ToDouble() != 1.7976931348623158E+308) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //convert the string to integer
    S1 = "2147483647";
    if(S1.ToInt() != 2147483647) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //convert the string to integer
    //in hexadecimal
    S1 = "7ffFffff";
    if(S1.ToHex() != INT_MAX) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //-----------------------------------------------------------------------

    CPPUNIT_ASSERT(true);
}

void TestVCLemu::test_Exception()
{
    try {
        try {
            throw Exception("message 1");
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
        if(E.Message.str != "message 2: message 1") {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    CPPUNIT_ASSERT(true);
}

void TestVCLemu::test_ConversionFunctions()
{
    //translate from double to string
    string str = floattostr(1.7976931348623158E+308);
    if(str != "1.79769313486232e+308") {
        CPPUNIT_ASSERT(false);
        return;
    }
    str = floattostr_fixed(-3.14159265358979);
    if(str != "-3.141592653589790") {
        CPPUNIT_ASSERT(false);
        return;
    }
    str = floattostr_fixed(-3.14159265358979, 5);
    if(str != "-3.14159") {
        CPPUNIT_ASSERT(false);
        return;
    }
    //translate from int to string
    str = inttostr(2147483647);
    if(str != "2147483647") {
        CPPUNIT_ASSERT(false);
        return;
    }
    //translate from unsigned int to string
    str = uinttostr(4294967295);
    if(str != "4294967295") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //traduce de double a AnsiString
    AnsiString S = FloatToStr(1.7976931348623158E+308);
    if(S != "1.79769313486232e+308") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //traduce de integer a AnsiString
    S = IntToStr(2147483647);
    if(S != "2147483647") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //traduce de AnsiString a integer
    //en base hexadecimal
    int n = HexToInt("7fffffff");
    if(n != 0x7fffffff) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //traduce de integer a AnsiString
    //en base hexadecimal
    n = 0x7fffffff;
    S = IntToHex(intptr_t(n));
    if(S != "7fffffff") {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void TestVCLemu::test_Strings()
{
    //contruye una lista de cadenas por defecto
    for(int i=0; i<10000; i++) {
        TStrings *Strings = new TStrings;
        delete Strings;
    }

    //añade una copia de una cadena de texto al final de la lista
    TStrings Strings;
    string str = "string 1";
    Strings.Add(AnsiString(str));
    str = "string 2";
    Strings.Add(AnsiString(str));
    str = "string 3";
    Strings.Add(AnsiString(str));
    str.clear();

    //accede a la cadena de texto indicada
    if(Strings[0] != "string 1") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(Strings[1] != "string 2") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(Strings[2] != "string 3") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //añade una copia de una cadena de texto al final de la lista
    str = "En algún lugar de la mancha, de cuyo nombre no quiero acordarme...";
    Strings.Add(str.c_str());
    str.clear();
    if(Strings[3] != "En algún lugar de la mancha, de cuyo nombre no quiero acordarme...") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //inserta una copia de una cadena de texto en la posición indicada
    for(int i=0; i<100; i++) {
        Strings.Clear();

        str = "string 1";
        Strings.Add(AnsiString(str));
        str = "string 3";
        Strings.Add(AnsiString(str));

        str = "string 2";
        Strings.Insert(1, str);
        if(Strings.getCount() != 3) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(Strings[0] != "string 1") {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(Strings[1] != "string 2") {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(Strings[2] != "string 3") {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //accede a la cadena de texto constante indicada
    const TStrings& CStrings = Strings;
    if(CStrings[0] != "string 1") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(CStrings[1] != "string 2") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(CStrings[2] != "string 3") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //determina si dos listas de cadenas tienen todas las cadenas iguales
    if(!(CStrings == Strings)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //determina si dos listas de cadenas tienen alguna cadena desigual
    if(CStrings != Strings) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

void TestVCLemu::test_StringList()
{
    //contruye una lista de cadenas por defecto
    for(int i=0; i<10000; i++) {
        TStringList *StringList = new TStringList;
        delete StringList;
    }

    //añade una copia de una cadena de texto al final de la lista
    TStringList StringList;
    string str = "string 1";
    StringList.Add(AnsiString(str));
    str = "string 2";
    StringList.Add(AnsiString(str));
    str = "string 3";
    StringList.Add(AnsiString(str));
    str.clear();

    //accede a la cadena de texto indicada
    if(StringList.Strings[0] != "string 1") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(StringList.Strings[1] != "string 2") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(StringList.Strings[2] != "string 3") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //añade una copia de una cadena de texto al final de la lista
    str = "En algún lugar de la mancha, de cuyo nombre no quiero acordarme...";
    StringList.Add(str.c_str());
    str.clear();
    if(StringList.Strings[3] != "En algún lugar de la mancha, de cuyo nombre no quiero acordarme...") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //inserta una copia de una cadena de texto en la posición indicada
    for(int i=0; i<100; i++) {
        StringList.Clear();

        str = "string 1";
        StringList.Add(AnsiString(str));
        str = "string 3";
        StringList.Add(AnsiString(str));

        str = "string 2";
        StringList.Strings.Insert(1, str);
        if(StringList.getCount() != 3) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(StringList.Strings[0] != "string 1") {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(StringList.Strings[1] != "string 2") {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(StringList.Strings[2] != "string 3") {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //accede a la cadena de texto constante indicada
    const TStringList& CStringList = StringList;
    if(CStringList.Strings[0] != "string 1") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(CStringList.Strings[1] != "string 2") {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(CStringList.Strings[2] != "string 3") {
        CPPUNIT_ASSERT(false);
        return;
    }

    //determina si dos listas de cadenas tienen todas las cadenas iguales
    if(!(CStringList.Strings == StringList.Strings)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //determina si dos listas de cadenas tienen alguna cadena desigual
    if(CStringList.Strings != StringList.Strings) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}
