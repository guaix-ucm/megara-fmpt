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
//File: vclemu.h
//Content: VCL emulator
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef VCLEMU_H
#define VCLEMU_H

#include <stdint.h>
#include <string>
#include <limits> //std::numeric_limits

using namespace std; //string

//---------------------------------------------------------------------------

//Numeric limits:
//Value                     VCL             STL
//                          <values.h>      <limits>
//53                        DSIGNIF         std::numeric_limits<double>::digits
//15                        -               std::numeric_limits<double>::digits10
//0x7fffffff                MAXINT          std::numeric_limits<int>::max()
//1.7976931348623158E+308   MAXDOUBLE       std::numeric_limits<double>::max()
//
//WARNING: maximun value of a double can not be translated to text using
//QString::toDouble(), generating an error.
//this is the reasom why is prefferible use stringstream or ostringstream.

//Other conversion functin in Qt:
//  QString::number(x, 'g', 18);
//  QString::number(n, 10);

//get the decimal separator value
char get_decimal_separator(void);

//The decimal separator value is assigned using:
//  char* setlocale( int category, const char* locale);
//Parameters:
//  - category: locale category identifier, one of the LC_xxx macros. May be 0.
//  - locale: system-specific locale identifier.
//    Can be "" for the user-preferred locale or "C" for the minimal locale.
//Return value:
//  - Pointer to a narrow null-terminated string identifying the C locale
//    after applying the changes, if any, or null pointer on failure.
//  - A copy of the returned string along with the category used in this call
//    to std::setlocale may be used later in the program to restore the locale
//    back to the state at the end of this call.
//For set '.' how decimal separator:
//    setlocale(LC_NUMERIC, "C");

//initialize the random seed according to the time
void randomize(void);
//generate a random number in [0, max)
unsigned int random(unsigned int max);

//---------------------------------------------------------------------------
//AnsiString

//class AnsiString
class AnsiString {
public:
    //contained string
    string str;

    //PUBLIC METHODS:

    //build an AnsiString by default
    AnsiString(void);
    //build an AnsiString from a char
    AnsiString(const char);
    //build an AnsiString from a c string
    AnsiString(const char*);
    //build an AnsiString from a string
    AnsiString(const string&);
    //build an AnsiString from a QString
    //AnsiString(const QString&);

    //build an AnsiString from an integer
    AnsiString(int);
    //build an AnsiString from a double
    AnsiString(double);

    //concatenate two AnsiStrings
    AnsiString operator+(const AnsiString&) const;
    //concatenate an AnsiString and a string
    AnsiString operator+(const string&) const;
    //concatenate an AnsiString to this AnsiString
    AnsiString& operator+=(const AnsiString&);
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

    //access to the indexed char in non constant AnsiStrings
    char& operator[](int i);
    //access to the indexed char in constant AnsiStrings
    const char& operator[](int i) const;

    //In AnsiString the indexed access start in 1, not in 0.

    //get a pointer to the first char of the AnsiString
    const char *c_str(void) const {return str.c_str();}
    //get the number of shars of the AnsiString
    int Length() const {return str.length();}
    //set the length of the AnsiString
    void SetLength(int);

    //insert a char in a position of the string
    //if index i not indicate a position in the string S
    //  throw an exception EImproperArgument
    void Insert(int i, char c);
    //insert a substring in a position of the string
    void Insert(int i, const AnsiString&);

    //get the substring in the interval [offset, offset+count]
    //if offset or offset+count not indicates a position in the AnsiString:
    //  throw EImproperArgument
    AnsiString SubString(int offset, int count) const;

    //convetrt the string to double
    double ToDouble() const;
    //convert the string to integer
    int ToInt() const;

    //convert the string to integer
    //in hexadecimal
    int ToHex() const;
    /*#
    //convert the string to unsigned integer
    //in hexadecimal
    uint ToUHex() const;*/

    //No sabemos como configurar un stringstream para que funcione en base hexadecimal.
    //Por eso vamos a comentar las funciones de conversión en base hexadecimal
    //hasta que sean necesarias.
};

//---------------------------------------------------------------------------
//Exception

//ADVERTENCIA: Exception debe derivarse de exception para
//permitir su captura unificada.

//invocación indebida de método o función
class Exception : public exception {
public:
    //mensaje de la excepción
    AnsiString Message;

    //construye una Exeption
    Exception(AnsiString t_Message) : exception(), Message(t_Message) {;}
    //libera la memoria
    ~Exception() throw() {;}

    //accede al mensaje
    const char* what() const throw() {return Message.str.c_str();}
};

//---------------------------------------------------------------------------
//CONVERSION FUNCTIONS:

//The more large integer has 13 characters,
//including the sign.
//The more large double has 23 caracteres,
//including the sign, the exponent and the sign of the exponent.

//translate from double to string
string floattostr(double value);
string floattostr_fixed(double value, int precision=std::numeric_limits<double>::digits10);
//translate from int to string
string inttostr(int value);

//translate from string to double
double strtofloat(const string&);
//translate from string to int
int strtoint(const string&);
//translate from string to bool
bool strtobool(const string&);

//traduce de AnsiString a double
double StrToFloat(const AnsiString& S);
//traduce de double a AnsiString
AnsiString FloatToStr(double x);

//traduce de AnsiString a integer
int StrToInt(const AnsiString& S);
//traduce de integer a AnsiString
AnsiString IntToStr(int n);

//traduce de AnsiString a integer
//en base hexadecimal
int HexToInt(const AnsiString& S);
//traduce de integer a AnsiString
//en base hexadecimal
AnsiString IntToHex(intptr_t n, int fig);
/*#
//traduce de AnsiString a unsigned integer
//en base hexadecimal
uint HexToUInt(const AnsiString& S);
//traduce de unsigned integer a AnsiString
//en base hexadecimal
AnsiString UIntToHex(uint n, int fig);

//traduce de QColor a AnsiString
AnsiString ColorToString(QColor);
//traduce de AnsiString a QColor
QColor StringToColor(const AnsiString&);
*/

//RECUERDE: cuidado con definir el cuerpo de las funciones en el header.
//Hay riesgo de error de declaración múltiple.

//---------------------------------------------------------------------------
//FUNCTIONS OF THE FILE SYSTEM:

//construye una ruta de directorios
int mkpath(const string& dir);
//borra un directorio y todo su contenido
int rmpath(string& path);

//obtiene la ruta del directorio actual
string getCurrentDir(void);
//construye un directorio
void ForceDirectories(const AnsiString&);

//---------------------------------------------------------------------------
//TStrings

//class array of AnsiStrings
class TStrings {
    //array de cadenas de texto
    AnsiString **Items;
    //capacidad del array de cadenas
    int Capacity;
    //número de cadenas de texto
    //dbe ser no negativo
    //valor por defecto: 0
    int Count;

public:
    //capacidad del array de cadenas
    int getCapacity(void) const {return Capacity;}
    void setCapacity(int);
    //número de cadenas de texto
    int getCount(void) const {return Count;}
    void setCount(int);

    //contruye una lista de cadenas por defecto
    TStrings(void);

    //accede a la cadena de texto indicada
    AnsiString& operator[](int i);
    //accede a la cadena de texto indicada
    const AnsiString& operator[](int i) const;
    //determina si dos listas de cadenas tienen todas las cadenas iguales
    bool operator==(const TStrings&) const;
    //determina si dos listas de cadenas tienen alguna cadena desigual
    bool operator!=(const TStrings&) const;

    //añade una copia de una cadena de texto al final de la lista
    void Add(const AnsiString&);
    //añade una copia de una cadena de texto al final de la lista
    void Add(const char*);
    //inserta una copia de una cadena de texto en la posición indicada
    void Insert(int i, const AnsiString&);
    //vacía la lista
    void Clear(void);
};

//---------------------------------------------------------------------------
//TrueBoolStrs and FalseBoolStrs:

//variables globales para contener las cadenas correspondientes a true y false
//según StrToBool y BoolToStr
extern TStrings TrueBoolStrs;
extern TStrings FalseBoolStrs;

//---------------------------------------------------------------------------
//TStringList

class TStringList {
public:
    TStrings Strings;

    //número de cadenas de texto
    int getCount(void) const {return Strings.getCount();}
    //asigna una cadena de texto descomponiéndola en líneas
    void setText(const AnsiString&);

    //construye una lista de cadenas de texto por defecto
    TStringList(void) : Strings() {;}

    //añade una copia de una cadena de texto al final de la lista
    void Add(const AnsiString&);
    //añade una copia de una cadena de texto al final de la lista
    void Add(const char*);

    //borra la lista
    void Clear(void) {Strings.Clear();}
};

//---------------------------------------------------------------------------
//TDateTime

class TDateTime {
public:
    //tiempo en días relativo a 1899/12/30 12:00:00
    double t;

    //contruye una fecha-hora por defecto
    TDateTime(void) : t(0) {;}
};

//---------------------------------------------------------------------------
//TPoint

struct TPoint
{
    int  x;
    int  y;

    TPoint() {}
    TPoint(int t_x, int t_y) : x(t_x), y(t_y) {}
    TPoint(TPoint& pt)
    {
        x = pt.x;
        y = pt.y;
    }
/*    operator POINT() const
    {
        TPoint pt;
        pt.x = x;
        pt.y = y;
        return pt;
    }*/
};

//---------------------------------------------------------------------------
//TRect

struct TRect
{
    int left;
    int top;
    int right;
    int bottom;

    TRect() {}
    TRect(const TPoint& TL, const TPoint& BR) { left=TL.x; top=TL.y; right=BR.x; bottom=BR.y; }
    TRect(int l, int t, int r, int b) { left=l; top=t; right=r; bottom=b; }
    TRect(TRect& r)
    {
        left    = r.left;
        top     = r.top;
        right   = r.right;
        bottom  = r.bottom;
    }
    int Width () const { return right  - left; }
    int Height() const { return bottom - top ; }

    bool operator ==(const TRect& rc) const
    {
        return left ==  rc.left  && top==rc.top &&
                right == rc.right && bottom==rc.bottom;
    }
    bool operator !=(const TRect& rc) const
    {  return !(rc==*this); }
};

//---------------------------------------------------------------------------
#endif // VCLEMU_H
