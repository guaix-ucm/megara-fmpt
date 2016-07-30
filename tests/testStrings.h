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
//File: testStrings.h
//Content: test for the module Strings
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FMPT_TESTStrings_H
#define FMPT_TESTStrings_H

#include <cppunit/extensions/HelperMacros.h>

//build the class where define the test methods
class TestStrings : public CppUnit::TestFixture
{
    //establish the test suit of Test
    CPPUNIT_TEST_SUITE(TestStrings);

    //add test methods
    CPPUNIT_TEST(test_isprint);
    CPPUNIT_TEST(test_islatinprint);
    CPPUNIT_TEST(test_StrIsUsualSeparator);
    CPPUNIT_TEST(test_StrNextChar);
    CPPUNIT_TEST(test_StrPreviousChar);
    CPPUNIT_TEST(test_StrSearch);
    CPPUNIT_TEST(test_strNextChar);
    CPPUNIT_TEST(test_strPreviousChar);
    CPPUNIT_TEST(test_StrFlip);
    CPPUNIT_TEST(test_StrIsPrintable);
    CPPUNIT_TEST(test_StrIsntPrintable);
    CPPUNIT_TEST(test_StrIsntString);
    CPPUNIT_TEST(test_StrSplitAssign);
    CPPUNIT_TEST(test_StrExtension);
    CPPUNIT_TEST(test_StrCopy);
    CPPUNIT_TEST(test_strreplace);
    CPPUNIT_TEST(test_StrReplace);
    CPPUNIT_TEST(test_StrCountLines);
    CPPUNIT_TEST(test_strPositionToCoordinates);
    CPPUNIT_TEST(test_thereIsntEndline);
    CPPUNIT_TEST(test_StrDivideInLines);
/*    CPPUNIT_TEST(test_StrDivideInWords);
    CPPUNIT_TEST(test_StrSplit);
    CPPUNIT_TEST(test_StrTrim);
    CPPUNIT_TEST(test_StrTrim);
    CPPUNIT_TEST(test_strTrim);
    CPPUNIT_TEST(test_strTrim);
    CPPUNIT_TEST(test_StrFirstChars);
    CPPUNIT_TEST(test_StrFirstChars);
    CPPUNIT_TEST(test_StrSearchFirstNonseparatorChar);
    CPPUNIT_TEST(test_strFirstNonseparatorChar);
    CPPUNIT_TEST(test_StrFill);
    CPPUNIT_TEST(test_strInsertChar);
    CPPUNIT_TEST(test_StrNotHasSameWords);
    CPPUNIT_TEST(test_StrMultiIndent);
    CPPUNIT_TEST(test_StrIndent);
    CPPUNIT_TEST(test_StrFormat);
    CPPUNIT_TEST(test_StrCutDecimals);*/

    //finish the process
    CPPUNIT_TEST_SUITE_END();

public:
    //overide setUp(), init data, etc
    void setUp();
    //overide tearDown(), free allocated memory, etc
    void tearDown();

    //TEST METHODS:

    //determina si un caracter es imprimible
    void test_isprint();

    //caracteres imprimibles de ISO8859-1 (ISO Latin 1 ó Alfabeto Latino nº1):
    //      anglosajones: [32, 126] (los mismos que en código ASCII)
    //      latinos: [160, 255] (lo mismos que en unicode)

    //La función isprint(c) determina si c está en [32, 126]

    //determina si un caracter ISO8859-1 es imprimible latino
    void test_islatinprint();// {if(c<160) return false; else return true;}

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
    void test_StrIsUsualSeparator();

    //en las siguientes funciones se usa el término imprimible para
    //referirse a un caracter imprimible anglosajón o imprimible latino

    //busca en S, a partir de i, el próximo caracter imprimible distinto de espacio
    //si no lo encuentra devuelve la posúltima posición
    void test_StrNextChar();
    //busca en S, a partir de i, el caracter previo imprimible distinto de espacio
    //si no lo encuentrsa devuelve la preprimera posición
    void test_StrPreviousChar();
    //busca en S la primera ocurrencia de c a partir de i
    void test_StrSearch();

    //busca en str, a partir de i, el próximo caracter imprimible distinto de espacio
    //si no lo encuentra devuelve la posúltima posición
    void test_strNextChar();
    //busca en str, a partir de i, el caracter previo imprimible distinto de espacio
    //si no lo encuentrsa devuelve la preprimera posición
    void test_strPreviousChar();

    //invierte el orden de los elementos de una cadena
    void test_StrFlip();
    //determina si una cadena contiene solo caracteres imprimibles
    //y delimitadores usuales
    void test_StrIsPrintable();
    //determina si una cadena contiene caracteres no imprimibles
    //ni delimitadores usuales
    void test_StrIsntPrintable();
    //determina si una cadena no está entre comillas simples
    void test_StrIsntString();
    //sustituye las secuencias de espacios y tabuladores por un solo espacio
    //AnsiString& StrTighten(AnsiString &S);
    //Segrega en Ident la subcadena que precede al primer '=' en S
    //y en Value la subcadena que le sucede; si tiene éxito devuelve true
    //y si S no contiene un caracter '=' devuelve false
    void test_StrSplitAssign();
    //separa la última extensión de un nombre de archivo
    //si no tiene extensión devuelve la cadena vacía
    void test_StrExtension();
    //copia el intervalo [i1, i2], de la cadena origen en la cadena destino
    //la cadena de destino perderá su contenido original
    void test_StrCopy();

    //sustituye las ocurrencias de una cadena a reemplazar por
    //una cadena sustituta
    void test_strreplace();
    //sustituye las ocurrencias de una cadena a reemplazar
    //por una cadena sustituta
    void test_StrReplace();
    //cuenta las líneas de texto contenidas en una cadena de texto imprimible
    //si la cadena no es imprimible lanza una excepción EImproperArgument
    void test_StrCountLines();
    //determine the coordinates (row, col) from the position i of a string
    void test_strPositionToCoordinates();
    //determines if there is the substring "\r\n" in the position i of a string
    void test_thereIsntEndline();
    //divide una cadena por cada "\r\n" que encuentre
    void test_StrDivideInLines();
/*    //divide una cadena en palabras
    //obtiene cada palabra hasta cada secuencia de espacios o tabuladores
    void test_StrDivideInWords();
    void test_StrDivideInWords();

    //un párrafo puede dividirse en líneas mediante TStrings::SetText(String)
    //este método es heredado por la clase hija TStringList
    //como es lógico es preferible el uso de dicho método a la función
    //StrDivideInLines(Lines, String)

    //separate the words between chars
    void test_StrSplit();

    //elimina los espacios y caracteres de control marginales de una cadena
    //la cadena de destino D puede ser la misma cadena fuente S
    void test_StrTrim();
    //elimina los espacios y caracteres de control marginales de una cadena
    void test_StrTrim();

    //la función Trim(const AnsiString &) elimina los espacios marginales
    //de una cadena y quizás también algunos caracteres de control
    //sin embargo no elimina al menos el caracter '\n'
    //por eso se define la función StrTrim(AnsiString&) que
    //si elimina todos los caracteres de control marginales

    //elimina los espacios y caracteres de control marginales de una cadena
    //la cadena de destino dest puede ser la misma cadena fuente src
    void test_strTrim();
    //elimina los espacios y caracteres de control marginales de una cadena
    void test_strTrim();

    //Si la longitud de la cadena es mayor que el número de caracteres indicado,
    //sustituye la parte sobrante de la cadena por "...".
    //La cadena de destino D, puede ser la misma que la cadena fuente S.
    void test_StrFirstChars();
    void test_StrFirstChars();

    //search and return the index to first non separator char in a text string
    void test_StrSearchFirstNonseparatorChar();
    //search and return the first non separator char in a text string
    void test_strFirstNonseparatorChar();

    //rellena una cadena con caracteres hasta que alcance la longitud indicada
    void test_StrFill();
    //insert a char in the indicated position of a text string
    //to get the intended length
    void test_strInsertChar();

    //ERROR: usar en StrFill la 'L' como nombre de variable provoca el error:
    //      Undefined symbol 'AnsiString'

    //determina si dos cadenas contienen las mismas palabras
    void test_StrNotHasSameWords();

    //añade la cadena que se indique al comienzo de cada línea de texto
    void test_StrMultiIndent();
    //añade la cadena que se indique al comienzo de cada línea de texto
    void test_StrIndent();

    //Las siguientes funciones representan con un punto decimal (no una coma)
    //los números reales en formato cadena.
    //Conviene usar un punto y no una coma para poder representar
    //números reales separados por comas.
    //Para usar estas funciones debe configurarse el sistema realizando
    //la siguiente asignación:
    //      DecimalSeparator = '.'
    //En Windows XP el valor por defecto de DecimalSeparator es una coma.

    //reconoce formatos numéricos en una cadena:
    //      'r'->real; 'b': binario; 'z'->entero; 'o'->otro
    //Devuelve:
    //	'r': si el número tiene un punto decimal, aunque se encuentre al final.
    //	'b': si no hay punto y todas las cifras se encuentran en [0, 1].
    //	'z': si no hay punto y hay alguna cifra mayor que 1.
    //	'o': niguna de las anteriores.
    void test_StrFormat();
    //Limita a n el número de decimales de una cadena que rerpesenta un nº real
    void test_StrCutDecimals();*/
};

#endif // FMPT_TESTStrings_H
