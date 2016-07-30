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
//File: VCLemu.h
//Content: emulation of some function and classes of VCL
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef VCLEMU_H
#define VCLEMU_H

#include <stdint.h>
#include <string>
#include <float.h> //DBL_MANT_DIG, DBL_DIG, DBL_MAX
#include <exception> //class eception

using namespace std; //string

//---------------------------------------------------------------------------

//Numeric limits:
//Value                     VCL         C libraries     STL
//                          <values.h>  <float.h>       <limits>
//53                        DSIGNIF     DBL_MANT_DIG    std::numeric_limits<double>::digits
//15                        -           DBL_DIG         std::numeric_limits<double>::digits10
//1.7976931348623158E+308   MAXDOUBLE   DBL_MAX         std::numeric_limits<double>::max()
//10                        -           INT_DIG         std::numeric_limits<int>::digits10
//0x7fffffff                MAXINT      INT_MAX         std::numeric_limits<int>::max()
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
    //get the number of chars of the AnsiString
    int Length() const {return (int)str.length();}
    //set the length of the AnsiString
    void SetLength(int);

    //Nothe that:
    //  string::length is unsigned int
    //  AnsiString::Length is int
    //It is for keep compabiliti with integer index.
    //Integer index must be integer for can be negative.

    //insert a char in a position of the string
    //if index i not indicate a position in the string
    //  throw an exception EImproperArgument
    void Insert(int i, char c);
    //insert a substring in a position of the string
    void Insert(int i, const AnsiString&);

    //get the substring in the interval [offset, offset + count]
    //if offset or offset + count not indicates a position in the AnsiString:
    //  throw EImproperArgument
    AnsiString SubString(int offset, int count) const;

    //convetrt the string to double
    double ToDouble() const;
    //convert the string to integer
    int ToInt() const;

    //convert the string to integer
    //in hexadecimal
    int ToHex() const;
};

//---------------------------------------------------------------------------
//Exception

//WARNING: all exceptions must be derivated from exception
//to allow the unified capture.

//general exception sith a message
class Exception : public exception {
public:
    //menssage of the exception
    AnsiString Message;

    //build an objecto of the class
    Exception(AnsiString t_Message) : exception(), Message(t_Message) {;}
    //free the dinamic memory
    ~Exception() throw() {;}

    //access to the message
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
string floattostr_fixed(double value, int precision=DBL_DIG);
//translate from int to string
string inttostr(int);
//translate from unsigned int to string
string uinttostr(unsigned int);

//Note that inttostr(unsigned int) would produce ambiguous call.

//translate from AnsiString to int
int StrToInt(const AnsiString& S);
//translate from integer to AnsiString
AnsiString IntToStr(int n);

//translate from AnsiString to int
//in hexadecimal format
int HexToInt(const AnsiString& S);
//translate from int to hexadecimal AnsiString
AnsiString IntToHex(intptr_t n);

//About intptr_t:
//Integer type capable of holding a value converted from a void pointer
//and then be converted back to that type with a value that compares equal
//to the original pointer.
//Optional: These typedefs may not be defined in some library implementations.
//Type intptr_t have 8 bytes. And usually it is associated with the use of
//reinterpret_cat<>, whose use is prefferible avoid,
//being prefferible make a ordinary cast.

//translate from AnsiString to double
double StrToFloat(const AnsiString& S);
//translate from double to AnsiString
AnsiString FloatToStr(double x);

//translate from AnsiString to bool
bool StrToBool(const AnsiString& S);
//translate from bool to AnsiString
AnsiString  BoolToStr(bool B, bool UseBoolStrs = false);

// "1" || "true" || "True"   => false
// "0" || "false" || "False" => false
// B && UseBoolStrs       => "True"
// !B && UseBoolStrs      => "False"
// B && !UseBoolStrs      => "1"
// !B && !UseBoolStrs     => "0"

//REMEMBER: take care defining the body of the functions in the header,
//because there is risk of multiple declaration.

//---------------------------------------------------------------------------
//FUNCTIONS OF THE FILE SYSTEM:

//build a directory path
int mkpath(const string& dir);
//force the erase of a directory and all theri content
int rmpath(string& path);

//get the path of the current directory
string getCurrentDir(void);
//force the building of a path of directories
void ForceDirectories(const AnsiString&);

//---------------------------------------------------------------------------
//class TStrings: an array of AnsiStrings

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
    //accede a la cadena de texto constante indicada
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
//class TStringList

class TStringList {
public:
    TStrings Strings;

    //number of text strings
    int getCount(void) const {return Strings.getCount();}
    //assign a text string decomposing it in lines
    void setText(const AnsiString&);

    //build a string list by default
    TStringList(void) : Strings() {;}

    //add a copy of a text string by the end of the list
    void Add(const AnsiString&);
    void Add(const char*);

    //clear the string list
    void Clear(void) {Strings.Clear();}
};

//---------------------------------------------------------------------------
//class TDateTime

class TDateTime {
public:
    //time in days from 1899/12/30 12:00:00
    double t;

    //build an object of the class by default
    TDateTime(void) : t(0) {;}
};

//---------------------------------------------------------------------------
//class TPoint

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
    //operator POINT() const
    //{
    //    TPoint pt;
    //    pt.x = x;
    //    pt.y = y;
    //    return pt;
    //}
};

//---------------------------------------------------------------------------
//class TRect

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
