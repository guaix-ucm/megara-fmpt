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
/// @file StrPR.h
/// @brief functions for print and read text strings
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef STRPR_H
#define STRPR_H

#include "Vectors.h" //TDoublePoint

//##include <QDateTime>

//---------------------------------------------------------------------------

using namespace Mathematics;

//espacio de nombres de funciones y clases de objetos
//relacionados con cadenas de texto
namespace Strings {

//--------------------------------------------------------------------------
//Functiones de conversión entre tipos numéricos y cadena de texto:

//Convierte un valor en punto flotante de double a AnsiString:
//      AnsiString = FloatToStr(double);
//esta función admite valores de x tan altos como se quiera, incluido MAXDOUBLE
//cuyo valor es 1.79769313486232E308

//Convierte un valor en punto flotante de AnsiString a double:
//      double = StrToFloat(AnsiString);
//esta función no admite valores todos los valores más altos para un double,
//de modo que si invoca a la función con la cadena "1.79769313486232E308",
//lanzará una excepción EOverflow. Además admite más decimales en la mantisa
//que el valor devuelto por FloatToStr; el valor "1.797693134862315807E308"
//tiene 5 decimales más y no produce error de desbordamiento.

//Conclusión:
//cuando se invoca a StrToFloat con una adena de texto conteniendo
//un valor en punto flotante mayor que "1,797693134862315807E308":
//      x = StrToFloat("1,79769313486232E308");
//sucede lo siguiente:
//      - La sentencia no llega a devolver ningún valor, con lo que la variable
//        x conservará su valor previo.
//      - Si el programa invoca a alguna de las funciones de conversión
//        StrToFloat o FloatToStr, o reaiza una operación aritmética:
//              · el sistema lanzará un excepción EOverflow en la sentencia,
//                que podrá ser capturada mediante un bloque catch.
//                Dicha excepción, según el debugger, debría contener
//                el mensaje 'Floating point overflow', pero realmente
//                contiene el mensaje 'EOverflow'.
//                Como la sentencia será interrumpida con una excepción no
//                legará a realizarse.
//      - Si el programa no invoca a StrToFloat ni FloatToStr ni realiza
//        ninguna operación aritmética:
//              · cuando termine de ejecutar todo y se devuelva el control
//                al programa principal, el sistema lanzará una excepción
//                EOverflow, esta vez si, con el mensaje
//                'Floating point overflow', qus será capturada por el bloque
//                catch general del programa.

//Como las funciones StrToFloat y FloatToStr no son simétricas, alguna de las
//dos deberá producir una excepción al intentar convertir alguno de los valores
//producidos por la otra.
//Es por esto que es preferible capturar las exepciones en todas
//ls invocaciones de StrToFloat, y no intentar programar una función
//simétrica de FloatToStr que no de nunca error.

//Resumen de la conslusión:
//La función StrToFloat produce efectos laterales que pueden ser capturados
//invoando a StrToFloat, FloatToStr o alguna función aritmética.

//Una posible solución:
//try {
//        x = StrToFloat(S);

//        //captura el efecto lateral cuando S contiene
//        //un valor mayor que "1,797693134862315807E308"
//        double aux = 0;
//        aux + aux;

//        //también podría hacerse invocando FloatToStr(0) pero seguramente
//        //sería más costoso.

//} catch(EOverflow &E) {
//        //indica que: la cadena de texto S contiene
//        //un valor en punto flotante fuera del rango de StrToFloat
//        throw EImproperArgument("string "S" contain a
//              floating point value out of range of StrToFloat");
//} catch(...) {
//        throw;
//}

//---------------------------------------------------------------------------
//Funciones para texto:

//intenta leer una palabra desde la posición i de una cadena de texto
void StrReadWord(AnsiString &Word, const AnsiString S, int &i);

//intenta leer una cadena desde la posición i de una cadena de texto
//incluyendo las comillas simples delimitadoras
void StrReadString(AnsiString &String, const AnsiString &S, int &i);
//intenta leer una cadena desde la posición i de una cadena de texto
//que se encuentra delimitada por dos caracteres determinados
void StrReadStringBetweenChars(AnsiString &String, const AnsiString &S, int &i,
        char c1='\"', char c2='\"');
//traduce una cadena entre dos caracteres a la misma cadena sin los caracteres
AnsiString StrBetweenCharsToStr(const AnsiString &S,
        char c1='\"', char c2='\"');
//lee una subcadena SubS desde la posición indicada de una cadena S
//hasta el caracter delimitador (d) que se indique
void StrReadStringToChar(AnsiString &SubS, const AnsiString &S, int &i, char d);
//intenta recorrer una cadena de texto (entre comillas simples)
//desde la posición i de una cadena de texto
void StrTravelString(const AnsiString &S, int &i);

//intenta leer una etiqueta desde
//la posición indicada de una cadena de texto
void StrReadLabel(AnsiString &Reading, const AnsiString &Label,
        const AnsiString &S, int &i);
//recorre una etiqueta a partir de
//la posición indicada de una cadena de texto
void StrTravelLabel(const AnsiString &Label, const AnsiString &S, int &i);

//trabel a label from
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

/*//segrega la subcadena desde la posición indicada hasta el final
//si encuentra un caracter no separador lanza una excepción EImproperArgument
//i quedará indicando a la posúltima posición
//void StrReadToEnd(AnsiString &SubS, const AnsiString &S, int &i);
//atraviesa la subcadena desde la posición indicada hasta el final
//si encuentra un caracter no separador lanza una excepción EImproperArgument
//i quedará indicando a la posúltima posición
void StrTravelToEnd(const AnsiString&, int&);*/
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

//---------------------------------------------------------------------------
//CONVERSION FUNCTIONS:


//WARNING: conversion functions bassed in sscanf implies:
//- conversión is stoped when a unrecognizable figure is found,
//  and it is not throug an exception;
//- too large numbers are converted to larg primitive types,
//  and it is truncated its most significant part of longint.

//read an integer in the indicated position of a string
void strReadIntStr(string& dst, const string& src, unsigned int& i);
//translate from string to int
int strToInt(const string&);
//read an integer in the indicated position of a string
void strReadInt(int& value, const string& src, unsigned int& i);

//read an integer in the indicated position of an AnsiString
void StrReadIntStr(AnsiString& D, const AnsiString& S, int& i);
//read an integer in the indicated position of an AnsiString
void StrReadInt(int& value, const AnsiString& S, int &i);

//read an integer in the indicated position of a string
//in hexadecimal format
void strReadHexStr(string& dst, const string& src, unsigned int& i);
//translate from string to int
//in hexadecimal format
int hexToInt(const string&);
//read an integer in the indicated position of a string
//in hexadecimal format
void strReadHex(int& value, const string& src, unsigned int& i);

//read an integer in the indicated position of an AnsiString
//in hexadecimal format
void StrReadHexStr(AnsiString& D, const AnsiString& S, int& i);
//read an integer in the indicated position of an AnsiString
//in hexadecimal format
void StrReadHex(int& value, const AnsiString& S, int &i);

//read a float in the indicated position of a string
void strReadFloatStr(string& dst, const string& src, unsigned int& i);
//translate from string to double
double strToFloat(const string&);
//read a float in the indicated position of a string
void strReadFloat(double& value, const string& src, unsigned int& i);

//print a double in an AnsiString
void StrPrintFloat(AnsiString& S, double value);
//read a float in the indicated position of an AnsiString
void StrReadFloatStr(AnsiString& D, const AnsiString& S, int &i);
//read a float in the indicated position of an AnsiString
void StrReadFloat(double& value, const AnsiString& S, int &i);

//read a bool in the indicated position of a string
void strReadBoolStr(string& dst, const string& src, unsigned int& i);
//translate from string to bool
bool strToBool(const string&);
//read a bool in the indicated position of a string
void strReadBool(bool& value, const string& src, unsigned int& i);

//print a bool in an AnsiString
void StrPrintBool(AnsiString& S, bool value);
//read a bool in the indicated position of an AnsiString
void StrReadBoolStr(AnsiString& D, const AnsiString& S, int &i);
//read a bool in the indicated position of an AnsiString
void StrReadBool(bool& value, const AnsiString& S, int &i);

//imprime un punto en una cadena de texto
void StrPrintPoint(AnsiString &S, const TPoint& P);
//compara dos punto
//valores devueltos:
//      -1: si x1<x2 || (x1==x2 && y1<y2)
//      1: si x1>x2 || (x1==x2 && y1>y2)
//      0: si x1==x2 && y1==y2
int  CompareDPoints(const TDoublePoint *P1, const TDoublePoint *P2);
//imprime un punto al final de una cadena de texto
void StrPrintDPoint(AnsiString &S, const TDoublePoint *P);
//intenta leer un punto a partir de la posición i de una cadena de texto
//e intenta convertirlo a TDoublePoint mediante StrToFloat
void StrReadDPoint(TDoublePoint *P, const AnsiString &S, int &i);
//convierte un valor punto de formato texto a TDoublePoint
//si S contiene algo más que un valor punto
//lanza una excepción EImproperArgument
TDoublePoint StrToDPoint(const AnsiString &S);

//intenta leer un rectángulo a partir de la posición i de una cadena de texto
//e intenta convertirlo a TDoubleRect mediante StrToFloat
void StrReadDRect(TDoubleRect *R, const AnsiString &S, int &i);
//convierte un valor rectángulo de formato texto a TDoubleRect
//si S contiene algo más que un valor rectángulo
//lanza una excepción EImproperArgument
TDoubleRect StrToDRect(const AnsiString &S);

//ADVERTENCIA: para incorporar las funciones de impresión y lectura de
//TDoublePoint a dicha estructura, habría que trasladar dicha estrucutar
//a una posición posterior a la declaración de FloatToStr.

//imprime dos enteros en una cadena indentados a 12 espacios
AnsiString IntToRowStr(int n1, int n2);

//NOTA: las siguientes funciones funcionan correctamente pero han sido aisladas
//para excluirlas del código posterior:
/*
//imprime en S las palabras de SL entre c1 y c2 separadas por s;
//s puede ser un simple espacio
void StrPrintSecuency (AnsiString &S, TStringList *SL,
    char c1='{', char c2='}', const AnsiString s=", ");
//escudriña S en busca de una lista delimitada por c1 y c2
//cuyas palabras están separadas por d
//si no la encuentra lanza una excepción EImproperArgument
void ReadList (TStringList *SL, const AnsiString &S,
    char c1='{', char c2='}', char d=',');
//escudriña S en busca de una lista delimitada por c1 y c2
//cuyas palabras están separadas por espacios
//si no la encuentra lanza una excepción EImproperArgument
void ReadVector (TStringList *SL, const AnsiString &S,
    char c1='(', char c2=')');  */

//---------------------------------------------------------------------------
//FUNCTIONS FOR DATE-TIME:

//translate from time_t to string in format en_EN
string strfromtime_t(time_t, string format = "%a, %d/%m/%Y %H:%M:%S");

//for get the current date-time in format time_t:
//  time_t t = time(NULL);

//Function:
//  time_t time(time_t *time);
//returns the current calendar time of the system in number of seconds
//elapsed since January 1, 1970. If the system has no time, .1 is returned.

//Para traducir de QString a QDateTime, existen los métodos:
//  QDateTime	fromString(const QString & string, Qt::DateFormat format = Qt::TextDate)
//  QDateTime	fromString(const QString & string, const QString & format)
//Sin embargo, el formato "yyyy/mm/dd hh:mm:ss" especificado mediante una QString,
//no resulta efectivo, devolviendo la función la fecha-hora inválida. Por eso es
//necesario especificar el formato mediante Qt::ISODate, el cual corresponde a:
//  "yyyy-mm-ddThh:mm:ss".

//Por otra parte TDateTime mantenía homología con double.
//En el caso de QDteTime, la homología se mantiene con uint.
//TDateTime era convertido automáticamente a double y viceversa.
//En el caso de QDateTime existen las funciones siguientes:
//  void	setTime_t(uint seconds)
//  uint	toTime_t() const
//  QDateTime	fromTime_t(uint seconds)
//  QDateTime	fromTime_t(uint seconds, Qt::TimeSpec spec, int offsetSeconds = 0)
//  QDateTime	fromTime_t(uint seconds, const QTimeZone & timeZone)
/*#
//lee la fecha-hora en una cadena de texto a partir de la posición indicada
//en el formato "dd/mm/yyyy hh:mm:ss" en formato de 24h sin AM/PM
//si S no contiene un dato válido, lanza EConvertError
void StrReadDateTime(QDateTime &DT,
        const AnsiString &S, int &i);

//convierte un valor de fecha-hora de formato texto a QDateTime
//si S contiene algo más que un valor de fehca-hora
//lanza una excepción EImproperArgument
double StrToDateTime_(const AnsiString &S);
*/
//---------------------------------------------------------------------------

} //namespace Strings

//---------------------------------------------------------------------------
#endif //STRPR_H
