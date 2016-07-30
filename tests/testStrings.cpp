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
//File: testStrings.cpp
//Content: test for the module Strings
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testStrings.h"
#include "../src/Constants.h" //M_2PI
//#include "cppunit_assert_emulator.h" //Qt only.

#include <cmath> //abs
#include <vector>
//#include <stdlib.h>
#include <limits> //std::numeric_limits

#include "../src/Strings.h"

using namespace Strings;

CPPUNIT_TEST_SUITE_REGISTRATION(TestStrings);

//overide setUp(), init data, etc
void TestStrings::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestStrings::tearDown() {
}

//---------------------------------------------------------------------------
//TEST METHODS:

//caracteres imprimibles de ISO8859-1 (ISO Latin 1 ó Alfabeto Latino nº1):
//      anglosajones: [32, 126] (los mismos que en código ASCII)
//      latinos: [160, 255] (lo mismos que en unicode)

//La función isprint(c) determina si c está en [32, 126]

//determina si un caracter ISO8859-1 es imprimible latino
void TestStrings::test_islatinprint()
{
    try {
        if(islatinprint(char(0))) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(islatinprint(char(159))) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(!islatinprint(char(160))) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(!islatinprint(char(255))) {
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

//caracteres separadores de ISO8859-1:
//Dec   Hex     Abrev   Repr    AT      Descripción
//9     09      HT      HT      ^I      Tabulación horizontal
//10    0A      LF      LF      ^J      Line feed (salto de línea)
//11    0B      VT      YT      ^K      Tabulación Vertical
//12    0C      FF      FF      ^L      Form feed (salto de página)
//13    0D      CR      CR      ^M      Carriage return (retorno de carro)
//28    1C      FS      FS      ^\      File Separator
//29    1D      GS      GS      ^]      Group Separator
//30    1E      RS      RS      ^^      Record Separator
//31    1F      US      US      ^_      Unit Separator
//32    20                              espacio
//160   A0                              ESPACIO DE "NO RUPTURA"

//caracteres separadores de ISO8859-1
//que usualmente forman parte de un docuemnto de texto:
//Dec   Hex     C. de Form. C   Descripción
//9     09      '       '       Tabulación horizontal
//10    0A      '\n'            Line feed (salto de línea)
//13    0D      '\r'            Carriage return (retorno de carro)
//32    20      ' '             espacio

//el caracter de salto de linea y retorno de carro siempre aparecen
//juntos en un documento de texto escrito en el siguiente orden: "\r\n"

//determina si la cadena contiene un separador de los usualmente escritos
void TestStrings::test_StrIsUsualSeparator()
{
    try {
        if(StrIsUsualSeparator("")) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(!StrIsUsualSeparator("\t")) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(!StrIsUsualSeparator(" ")) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(!StrIsUsualSeparator("\r\n")) {
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

//en las siguientes funciones se usa el término imprimible para
//referirse a un caracter imprimible anglosajón o imprimible latino

//busca en S, a partir de i, el próximo caracter imprimible distinto de espacio
//si no lo encuentra devuelve la posúltima posición
void TestStrings::test_StrNextChar()
{
    try {
        string str = " \t \r\n  texto   \t \r\n ";

        int i = 0;
        int p = strNextChar(str, i);
        if(p != 7) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 7;
        p = strNextChar(str, i);
        if(p != 7) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 11;
        p = strNextChar(str, i);
        if(p != 11) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 12;
        p = strNextChar(str, i);
        if(p != 20) {
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
//busca en S, a partir de i, el caracter previo imprimible distinto de espacio
//si no lo encuentrsa devuelve la preprimera posición
void TestStrings::test_StrPreviousChar()
{
    try {
        string str = " \t \r\n  texto   \t \r\n ";

        int i = 20;
        int p = StrPreviousChar(str, i);
        if(p != 12) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 13;
        p = StrPreviousChar(str, i);
        if(p != 12) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 12;
        p = StrPreviousChar(str, i);
        if(p != 12) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 8;
        p = StrPreviousChar(str, i);
        if(p != 8) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 7;
        p = StrPreviousChar(str, i);
        if(p != 0) {
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
//busca en S la primera ocurrencia de c a partir de i
void TestStrings::test_StrSearch()
{
    try {
        string str = " \t \r\n  texto   \t \r\n ";

        int i = 1;
        int p = StrSearch(str, 't', i);
        if(p != 8) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 8;
        p = StrSearch(str, 't', i);
        if(p != 8) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 9;
        p = StrSearch(str, 't', i);
        if(p != 11) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 12;
        p = StrSearch(str, 't', i);
        if(p != 21) {
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

//busca en str, a partir de i, el próximo caracter imprimible distinto de espacio
//si no lo encuentra devuelve la posúltima posición
void TestStrings::test_strNextChar()
{
    try {
        string str = " \t \r\n  texto   \t \r\n ";
        int i = 1;
        int p = StrNextChar(str, i);
        if(p != 8) {
            CPPUNIT_ASSERT(false);
            return;
        }
        i = 8;
        p = StrNextChar(str, i);
        if(p != 8) {
            CPPUNIT_ASSERT(false);
            return;
        }
        i = 12;
        p = StrNextChar(str, i);
        if(p != 12) {
            CPPUNIT_ASSERT(false);
            return;
        }
        i = 13;
        p = StrNextChar(str, i);
        if(p != 21) {
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
//busca en str, a partir de i, el caracter previo imprimible distinto de espacio
//si no lo encuentrsa devuelve la preprimera posición
void TestStrings::test_strPreviousChar()
{
    try {
        string str = " \t \r\n  texto   \t \r\n ";

        int i = 19;
        int p = strPreviousChar(str, i);
        if(p != 11) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 12;
        p = strPreviousChar(str, i);
        if(p != 11) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 11;
        p = strPreviousChar(str, i);
        if(p != 11) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 7;
        p = strPreviousChar(str, i);
        if(p != 7) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 6;
        p = strPreviousChar(str, i);
        if(p != -1) {
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

//invierte el orden de los elementos de una cadena
void TestStrings::test_StrFlip()
{
    try {
        AnsiString S = "texto 1";
        StrFlip(S);
        if(S.str != "1 otxet") {
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
//determina si un caracter es imprimible
void TestStrings::test_isprint()
{
    try {
        if((bool)isprint(char(0)) != false) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if((bool)isprint(char(31)) != false) {
            CPPUNIT_ASSERT(false);
            return;
        }

        if((bool)isprint(char(32)) != true) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if((bool)isprint(char(126)) != true) {
            CPPUNIT_ASSERT(false);
            return;
        }

        if((bool)isprint(char(127)) != false) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if((bool)isprint(char(255)) != false) {
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
//determina si una cadena contiene solo caracteres imprimibles
//y delimitadores usuales
void TestStrings::test_StrIsPrintable()
{
    try {
        string str = "line1\r\nline2 ";

        //caracteres imprimibles de ISO8859-1 (ISO Latin 1 ó Alfabeto Latino nº1):
        //  anglosajones: [32, 126] (los mismos que en código ASCII)
        //  latinos: [160, 255] (lo mismos que en unicode)

        //[0, 31]: not printable
        str[12] = char(0);
        if(StrIsPrintable(str)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        str[12] = char(31);
        if(StrIsPrintable(str)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //[32, 126]: printable
        str[12] = char(32);
        if(!StrIsPrintable(str)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        str[12] = char(126);
        if(!StrIsPrintable(str)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //[127, 159]: not printable
        str[12] = char(127);
        if(StrIsPrintable(str)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        str[12] = char(159);
        if(StrIsPrintable(str)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //[160, 255]: printable
        str[12] = char(160);
        if(!StrIsPrintable(str)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        str[12] = char(255);
        if(!StrIsPrintable(str)) {
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
//determina si una cadena contiene caracteres no imprimibles
//ni delimitadores usuales
void TestStrings::test_StrIsntPrintable()
{
    try {
        string str = "line1\r\nline2 ";

        //caracteres imprimibles de ISO8859-1 (ISO Latin 1 ó Alfabeto Latino nº1):
        //  anglosajones: [32, 126] (los mismos que en código ASCII)
        //  latinos: [160, 255] (lo mismos que en unicode)

        //char(0) is the string terminator '\0'
        //si str+char(0) is prontable

        //[0, 31]: not printable
        str[12] = char(0);
        if(!StrIsntPrintable(str)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        str[12] = char(31);
        if(!StrIsntPrintable(str)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //[32, 126]: printable
        str[12] = char(32);
        if(StrIsntPrintable(str)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        str[12] = char(126);
        if(StrIsntPrintable(str)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //[127, 159]: not printable
        str[12] = char(127);
        if(!StrIsntPrintable(str)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        str[12] = char(159);
        if(!StrIsntPrintable(str)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //[160, 255]: printable
        str[12] = char(160);
        if(StrIsntPrintable(str)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        str[12] = char(255);
        if(StrIsntPrintable(str)) {
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
//determina si una cadena no está entre comillas simples
void TestStrings::test_StrIsntString()
{
    try {
        if(!StrIsntString("'string 1")) {
            CPPUNIT_ASSERT(false);
            return;
        }
        if(StrIsntString("'string 1\r\n'")) {
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
//sustituye las secuencias de espacios y tabuladores por un solo espacio
//AnsiString& StrTighten(AnsiString &S);
//Segrega en Ident la subcadena que precede al primer '=' en S
//y en Value la subcadena que le sucede; si tiene éxito devuelve true
//y si S no contiene un caracter '=' devuelve false
void TestStrings::test_StrSplitAssign()
{
    try {
        AnsiString S;
        bool b;
        AnsiString Ident, Value;

        S = "";
        b = StrSplitAssign(Ident, Value, S);
        if(b != false || Ident != "" || Value != "") {
            CPPUNIT_ASSERT(false);
            return;
        }

        S = "ident = value";
        b = StrSplitAssign(Ident, Value, S);
        if(b != true || Ident != "ident" || Value != "value") {
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
//separa la última extensión de un nombre de archivo
//si no tiene extensión devuelve la cadena vacía
void TestStrings::test_StrExtension()
{
    try {
        string filename = "filename.ext1.ext2. ext3";
        AnsiString Ext = StrExtension(filename);
        if(Ext != ". ext3") {
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
//copia el intervalo [i1, i2], de la cadena origen en la cadena destino
//la cadena de destino perderá su contenido original
void TestStrings::test_StrCopy()
{
    try {
        AnsiString S = "hello world";
        AnsiString D;
        StrCopy(D, S, 7, 11);
        if(D != "world") {
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

//sustituye las ocurrencias de una cadena a reemplazar por
//una cadena sustituta
void TestStrings::test_strreplace()
{
    try {
        string str = "En algún lugar de la Mancha, de cuyo nombre no quiero acordarme...";
        string str_to_replace = " no quiero acordarme";
        string str_substitute = "... ¡Ah pues mira!, si me acuerdo";
        strreplace(str, str_to_replace, str_substitute);
        if(str != "En algún lugar de la Mancha, de cuyo nombre... ¡Ah pues mira!, si me acuerdo...") {
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
//sustituye las ocurrencias de una cadena a reemplazar
//por una cadena sustituta
void TestStrings::test_StrReplace()
{
    try {
        AnsiString str = "En algún lugar de la Mancha, de cuyo nombre no quiero acordarme...";
        AnsiString str_to_replace = " no quiero acordarme";
        AnsiString str_substitute = "... ¡Ah pues mira!, si me acuerdo";
        StrReplace(str, str_to_replace, str_substitute);
        if(str != "En algún lugar de la Mancha, de cuyo nombre... ¡Ah pues mira!, si me acuerdo...") {
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
//cuenta las líneas de texto contenidas en una cadena de texto imprimible
//si la cadena no es imprimible lanza una excepción EImproperArgument
void TestStrings::test_StrCountLines()
{
    try {
        string str;
        int n;

        str = "";
        n = StrCountLines(str);
        if(n != 1) {
            CPPUNIT_ASSERT(false);
            return;
        }

        str = "\r";
        n = StrCountLines(str);
        if(n != 1) {
            CPPUNIT_ASSERT(false);
            return;
        }

        str = "\r\r\r\n";
        n = StrCountLines(str);
        if(n != 2) {
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
//determine the coordinates (row, col) from the position i of a string
void TestStrings::test_strPositionToCoordinates()
{
    try {
        string str = "line 1\r\n\tline 2\r\r\n";
        unsigned int i, row, col;

        i = 0;
        strPositionToCoordinates(row, col, str, i);
        if(row != 1 || col != 1) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 5; //"line 1"
        strPositionToCoordinates(row, col, str, i);
        if(row != 1 || col != 6) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 6; //"line 1\r"
        strPositionToCoordinates(row, col, str, i);
        if(row != 1 || col != 7) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 7; //"line 1\r\n"
        strPositionToCoordinates(row, col, str, i);
        if(row != 2 || col != 0) {
            CPPUNIT_ASSERT(false);
            return;
        }

        i = 8; //"line 1\r\n\t"
        strPositionToCoordinates(row, col, str, i);
        if(row != 2 || col != 1) {
            CPPUNIT_ASSERT(false);
            return;
        }
        i = 15; //"line 1\r\n\tline 2\r"
        strPositionToCoordinates(row, col, str, i);
        if(row != 2 || col != 8) {
            CPPUNIT_ASSERT(false);
            return;
        }
        i = 16; //"line 1\r\n\tline 2\r\r"
        strPositionToCoordinates(row, col, str, i);
        if(row != 2 || col != 9) {
            CPPUNIT_ASSERT(false);
            return;
        }
        i = 17; //"line 1\r\n\tline 2\r\r\n"
        strPositionToCoordinates(row, col, str, i);
        if(row != 3 || col != 0) {
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
//determines if there is the substring "\r\n" in the position i of a string
void TestStrings::test_thereIsntEndline()
{
    try {
        string str = "";
        bool b = thereIsntEndline(str, 0);
        if(b != true) {
            CPPUNIT_ASSERT(false);
            return;
        }

        str = " testo 1 \r \r\n texto 2";

        b = thereIsntEndline(str, 9);
        if(b != true) {
            CPPUNIT_ASSERT(false);
            return;
        }

        b = thereIsntEndline(str, 10);
        if(b != true) {
            CPPUNIT_ASSERT(false);
            return;
        }

        b = thereIsntEndline(str, 11);
        if(b != false) {
            CPPUNIT_ASSERT(false);
            return;
        }

        b = thereIsntEndline(str, 12);
        if(b != true) {
            CPPUNIT_ASSERT(false);
            return;
        }

    } catch(...) {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

//divide una cadena por cada "\r\n" que encuentre
void TestStrings::test_StrDivideInLines()
{
    string str;
    TStrings Strings;
    StrDivideInLines(Strings, str);

    if(Strings.getCount() != 1 || Strings[0] != "") {
        CPPUNIT_ASSERT(false);
        return;
    }

    str = "\r\n";
    StrDivideInLines(Strings, str);

    if(Strings.getCount() != 2 || Strings[0] != "" || Strings[1] != "") {
        CPPUNIT_ASSERT(false);
        return;
    }

    str = " linea \r 1 \r\n";
    StrDivideInLines(Strings, str);

    if(Strings.getCount() != 2 || Strings[0] != " linea \r 1 " || Strings[1] != "") {
        CPPUNIT_ASSERT(false);
        return;
    }

    str = " linea \r 1 \r\n\t";
    StrDivideInLines(Strings, str);

    if(Strings.getCount() != 2 || Strings[0] != " linea \r 1 " || Strings[1] != "\t") {
        CPPUNIT_ASSERT(false);
        return;
    }

    str = " linea \r 1 \r\n\r\n";
    StrDivideInLines(Strings, str);

    if(Strings.getCount() != 3 || Strings[0] != " linea \r 1 " || Strings[1] != "" || Strings[2] != "") {
        CPPUNIT_ASSERT(false);
        return;
    }

    CPPUNIT_ASSERT(true);
}

