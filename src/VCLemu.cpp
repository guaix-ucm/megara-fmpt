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
///@file VCLemu.cpp
///@brief emulation of some function and classes of VCL
///@author Isaac Morales Durán
//---------------------------------------------------------------------------

#include "VCLemu.h"
#include "Exceptions.h"
#include "Strings.h"
#include "StrPR.h"

#include <cstring> //strlen
#include <stdlib.h> //srand, rand, RAND_MAX, atoi, strtod, strtol
#include <cmath>
//#include <typeinfo> //typeof
#include <sys/stat.h> //mkdir, stat, S_ISDIR
#include <dirent.h> //DIR
#include <stdio.h> //printf, scanf, sprintf, sscanf
#include <unistd.h> //getcwd, rmdir
#include <climits> //PATH_MAX
#include <locale.h> //struct lconv, localeconv()

#include <sstream> //stringstream

//WARNING:in GCC 2.95 not must be included:
//sstring: because it is not linked the dynamic version of libstdc++.
//iostream: because it is not used ios::fixed since the sstream is
//not included.
//string: because the following functions of the namespace std,
//aren't available in these version 2.95 of GCC:
//Convert from strings:
//  stoi    Convert string to integer (function template )
//  stol    Convert string to long int (function template )
//  stoul   Convert string to unsigned integer (function template )
//  stoll   Convert string to long long (function template )
//  stoull  Convert string to unsigned long long (function template )
//  stof    Convert string to float (function template )
//  stod    Convert string to double (function template )
//  stold   Convert string to long double (function template )
//Convert to strings:
//  to_string   Convert numerical value to string (function )
//  to_wstring  Convert numerical value to wide string (function )

using namespace std;
using namespace Strings;

//---------------------------------------------------------------------------

//get the decimal separator value
char get_decimal_separator(void)
{
    struct lconv *lc;
    lc = localeconv();
    char *s = lc->decimal_point;

    if(strlen(s) != 1)
        throw exception();

    return s[0];
}

//initialize the random seed according to the time
void randomize(void)
{
    srand(time(NULL));
}
//generate a random number in [0, max)
unsigned int random(unsigned int max) {
    //generate the number in [0, RAND_MAX)
    unsigned int n = rand();
    //normalize the number
    double x = double(n)/double(RAND_MAX);
    //project the number in [0, max)
    x *= max;
    //get the integer part
    n = (unsigned int)floor(x);
    //return the integer part
    return n;

    //for perform portability to other system,
    //it must be replaced:
    //  rand    ->  qrand   (in stdlib.h)
    //  floor   ->  qFloor  (in Qt/Core/qmat.h)
}

//---------------------------------------------------------------------------
//class AnsiString:

//PUBLIC METHODS:

//build an AnsiString by default
AnsiString::AnsiString(void) : str()
{
}
//build an AnsiString from an AnsiString
AnsiString::AnsiString(const AnsiString& S)
{
    str = S.str;
}

//build an AnsiString from a char
AnsiString::AnsiString(const char c) : str()
{
    str = c;
}
//build an AnsiString from a c string
AnsiString::AnsiString(const char *c_str) : str(c_str)
{
}
//build an AnsiString from a string
AnsiString::AnsiString(const string& t_str)
{
    str = t_str;
}
//build an AnsiString from a QString
//AnsiString::AnsiString(const QString& QS) : str(QS.toLocal8Bit())
//{
//}

//build an AnsiString from an integer
AnsiString::AnsiString(int x)
{
    *this = IntToStr(x);
}
//build an AnsiString from a double
AnsiString::AnsiString(double x)
{
    *this = FloatToStr(x);
}

//concatenate two AnsiStrings
AnsiString AnsiString::operator+(const AnsiString& S) const
{
    return AnsiString(str + S.str);
}
//concatenate an AnsiString and a string
AnsiString AnsiString::operator+(const string& t_str) const
{
    return AnsiString(str + t_str);
}
//concatenate an AnsiString to this AnsiString
AnsiString& AnsiString::operator+=(const AnsiString& S)
{
    str += S.str;
    return *this;
}
//concatenate a char to this AnsiString
AnsiString& AnsiString::operator+=(const char& c)
{
    str += c;
    return *this;
}
//copy an AnsiString
AnsiString& AnsiString::operator=(const AnsiString& S)
{
    str = S.str;
    return *this;
}
//copy a string
AnsiString& AnsiString::operator=(const string& t_str)
{
    str = t_str;
    return *this;
}
//copy a c string (not add it, copy it)
AnsiString& AnsiString::operator=(const char *c_str)
{
    //check the precondition
    if(c_str == NULL)
        throw EImproperArgument("pointer c_str should point to built char");

    str = c_str;
    return *this;
}
//determine if an AnsiString is equal to this AnsiString
bool AnsiString::operator==(const AnsiString& S) const
{
    if(str == S.str)
        return true;
    return false;
}
//determine if an AnsiString is unequal to this AnsiString
bool AnsiString::operator!=(const AnsiString& S) const
{
    if(str == S.str)
        return false;
    return true;
}

//access to the indexed char in non constant AnsiStrings
char& AnsiString::operator[](int i)
{
    //check the precondition
    if(i<1 || Length()<i)
        throw Exception("index i should indicate a char of the string");

    return str[i-1];
}
//access to the indexed char in constant AnsiStrings
const char& AnsiString::operator[](int i) const
{
    //check the precondition
    if(i<1 || str.length()<(unsigned int)i)
        throw Exception("index i should indicate a char of the string");

    return str[i-1];
}

//set the length of the AnsiString
void AnsiString::SetLength(int n)
{
    try {
        str.resize(n);
    }
    catch(...) {
        throw;
    }
}

//insert a char in a position of the string
//if index i not indicate a position in the string
//  throw an exception EImproperArgument
void AnsiString::Insert(int i, char c)
{
    //check the precondition
    if(i<0 || Length()+1<i)
        throw EImproperArgument("index i shall indicates a position in the string");

    str.insert(i-1, sizeof(typeof(c)), c);
}
//insert a substring in a position of the string
void AnsiString::Insert(int i, const AnsiString& S)
{
    //check the precondition
    if(i<1 || Length()<i)
        throw EImproperArgument("index i should indicate a char of the string");

    //Nothe that string::insert not allow insert in postlast position (string.length()).

    //insert all chars of S.str in the position i-1 of S.str
    for(unsigned int j=0; j<S.str.length(); j++) {
        char c = S.str[j];
        str.insert(i-1+j, sizeof(c), c);
    }
}

//get the substring in the interval [offset, offset + count]
//if offset or offset + count not indicates a position in the AnsiString:
//  throw EImproperArgument
AnsiString AnsiString::SubString(int offset, int count) const
{
    //check the preconditions
    if(offset<1 || Length()<offset)
        throw EImproperArgument("index offset should indicate a char of the string");
    if(offset + count < 1 || Length() + 1 < offset + count)
        throw EImproperArgument("index offset + count should indicate a position in the string");

    AnsiString Dest;
    offset--;
    for(int i=0; i<count; i++)
        Dest += str[offset + i];

    return Dest;
}

//convetrt the string to double
double AnsiString::ToDouble() const
{
    return strToFloat(str);
}
//convert the string to integer
int AnsiString::ToInt() const
{
    return strToInt(str);
}

//convert the string to integer
//in hexadecimal
int AnsiString::ToHex() const
{
    return hexToInt(str);
}

//---------------------------------------------------------------------------
//CONVERSION FUNCTIONS:

//translate from double to string
string floattostr(double value)
{
    stringstream ss;

    //convert to string
    ss.precision(DBL_DIG);
    bool ok = bool(ss << value);
    if(!ok)
        throw EImproperArgument(AnsiString("can't convert double to string"));
    return ss.str();

    //Othe way to do this:
    //  char c_str[308];
    //  string format = "%."+inttostr(DBL_DIG)+"lf";
    //  string format = "%lf";
    //  sprintf(c_str, format.c_str(), value);
    //  return string(c_str);
    //But this method print decimal zeros on the right.

    //WARNING: if precision is upper to digits10 decimals, conversión to
    //string can introduce numerial error for somevalues.Example:
    //  value       ->  string
    //  87.035553	->  "87.03555299999999"
    //  -70.35		->  "-70.34999999999999"
}
//translate from double to string
string floattostr_fixed(double value, int precision)
{
    //check the precondition
    if(precision<0 || DBL_DIG<precision)
        throw EImproperArgument("precision should be in [0, "+inttostr(DBL_DIG)+"]");

    //convert to string
    char c_str[330];
    string format = "%."+inttostr(precision)+"lf";
    sprintf(c_str, format.c_str(), value);
    return string(c_str);
    //But this method print decimal zeros on the right.

    //Othe way to do this:
    //  stringstream ss;
    //  ss.precision(precision);
    //  bool ok = (ss << value);
    //  if(!ok)
    //      throw EImproperArgument(AnsiString("can't convert double to string"));
    //  return ss.str();
    //But in this metthod, precission is the number of significant figures.

    //WARNING: if precision is upper to digits10 decimals, conversión to
    //string can introduce numerial error for somevalues.Example:
    //  value       ->  string
    //  87.035553	->  "87.03555299999999"
    //  -70.35		->  "-70.34999999999999"
}
//translate from int to string
string inttostr(int value)
{
    stringstream ss;
    bool ok = bool(ss << value);
    if(!ok)
        throw EImproperArgument(AnsiString("can't convert int to string"));
    return ss.str();

    //Other way to do this:
    //  char c_str[12];
    //  sprintf(c_str, "%d", value);
    //  return string(c_str);
}
//translate from unsigned int to string
string uinttostr(unsigned int value)
{
    //convert to string
    stringstream ss;
    bool ok = bool(ss << value);
    if(!ok)
        throw EImproperArgument(AnsiString("can't convert int to string"));
    return ss.str();

    //Other way to do this:
    //  char c_str[12];
    //  sprintf(c_str, "%d", value);
    //  return string(c_str);
    //But printf print "-1" for the UINT_MAX.
}

//translate from AnsiString to int
int StrToInt(const AnsiString& S)
{
    return strToInt(S.str);
}
//translate from integer to AnsiString
AnsiString IntToStr(int value)
{
    AnsiString S(inttostr(value));
    return S;
}

//translate from AnsiString to int
//in hexadecimal format
int HexToInt(const AnsiString& S)
{
    return hexToInt(S.str);
}
//translate from int to hexadecimal AnsiString
AnsiString IntToHex(intptr_t n)
{
    AnsiString S;
    S.SetLength(2*sizeof(n)); //Type intptr_t have 8 bytes.
    sprintf((char*)S.c_str(), "%x", (unsigned int)n);
    S.SetLength(strlen(S.c_str()));
    return S;

    //FIXME: itoa is not standard:
    //  itoa(n_, (char*)S.c_str(), 16);

    //Other way to do this in Qt:
    //  return AnsiString(QString::number(n, 16).toUpper());
}

//translate from AnsiString to double
double StrToFloat(const AnsiString& S)
{
    return strToFloat(S.str);
}
//translate from double to AnsiString
AnsiString FloatToStr(double value)
{
    AnsiString S(floattostr(value));
    return S;
}

//Convierte un valor lógico a un AnsiString.
//      Si UseBoolStrs==true, B==true y TrueBoolStrs[0] no está definido
//lanza una excepción EImproperCall.
//Si UseBoolStrs==true, B==false y FalseBoolStrs[0] no está definido
//      lanza una excepción EImproperCall.
AnsiString  BoolToStr(bool B, bool UseBoolStrs)
{
    if(UseBoolStrs) {
        if(B)
            return AnsiString("True");
        else
            return AnsiString("False");
    }
    else {
        if(B)
            return AnsiString("1");
        else
            return AnsiString("0");
    }
}
//translate from AnsiString to bool
bool StrToBool(const AnsiString& S)
{
    return strToBool(S.str);
}

//---------------------------------------------------------------------------
//FUNCTIONS OF THE FILE SYSTEM:

//build a directory path
int mkpath(const string& path)
{
    //si la ruta está vacía
    if(path.length() <= 0)
        //indica que la ruta no puede ser construida
        return 1;

    //si la ruta no es válida
    //indica que la ruta no puede ser construida
    //        return 1;

    //ERROR: hace falta una función para comprobar
    //si una ruta de directorios es válida. Este error
    //tendrá el efecto de que los directorios serán
    //constuidos de izquierda a derecha hasta que dejen
    //de ser válidos.

    //intenta contruir el directorio
    bool cant_make = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    //si el directorio no puede ser construido
    if(cant_make) {
        //descompone la ruta en la supra-ruta y el directorio actual
        string upper_path;
        int i = path.length() - 1;
        while(i>=0 && path.c_str()[i]!='/') {
            i--;
        }
        for(int j=0; j<i; j++)
            upper_path += path[j];

        //intenta construir la supra-ruta
        cant_make = mkpath(upper_path);

        //si la supra-ruta no puede ser construida
        if(cant_make)
            //indica que la ruta no puede ser consturida
            return 1;
        else { //si la supra-ruta ha sido construida
            //intenta construir el directorio actual
            cant_make = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            //indica el efecto de la construcción
            return cant_make;
        }
    }
    //si el directorio ha sido construido
    else
        //indica que la ruta ha sido construida
        return cant_make;


    /*    //el puntero dir debería apuntar a una cadena construida
    if(dir == NULL)
        return 1;

    //si el directorio existe o solo queda el directorio raiz
    struct stat sb;
    if((stat(dir, &sb)==0 && S_ISDIR(sb.st_mode)) ||
            (strlen(dir)==3 && dir[2]=='/'))
        //indica que ha terminado el trabajo con éxito
        //(para el nivel en que se encuentre)
        return 0;

    //si la path o el directorio no existe

    //construye la subpath contenedora
    char *sp = strdup(dir);
    char *dn = strstr(sp, "/");
    mkpath(dn);

    //construye el directorio actual
    //cuando la subpath contenedora ya está construida
    return mkdir(dir);*/
}
//force the erase of a directory and all their content
int rmpath(string& path)
{
    //lee los atributos de la entrada
    struct stat sb;
    int result = stat(path.c_str(), &sb);

    //si no ha podido leer los atributos de la entrada
    if(result != 0)
        //indica que no ha podido realizar la funcion
        return 1;

    //si la ruta se refiere a un directorio
    if(S_ISDIR(sb.st_mode)) {

        DIR *dp; //puntero a directorio
        struct dirent *ep; //puntero a entrada (de directorio)

        //intenta abrir el directorio
        dp = opendir(path.c_str());

        //si no puede abrir el directorio
        if (dp == NULL)
            //indica que no puede realizar la función
            return 2;

        //intenta obtener un puntero a la próxima entrada
        ep = readdir(dp);

        //mientras haya una nueva entrada
        while (ep != NULL) {
            //---------------------

            //compone la ruta de la entrada
            string d_name = string(ep->d_name);
            string entry;
            if(path[path.length()-1] != '/')
                entry = path+"/"+d_name;
            else
                entry = path+d_name;

            //lee los atributos de la entrada
            result = stat(entry.c_str(), &sb);

            //si no ha podido leer los atributos de la entrada
            if(result != 0)
                //indica que no ha podido realizar la funcion
                return 3;

            //si la entrada no es "." ni ".."
            if(d_name!="." && d_name!="..") {
                //si la entrada es un directorio
                if(S_ISDIR(sb.st_mode)) {
                    //intenta borrar el subdirectorio y todo su contenido
                    result = rmpath(entry);
                    //si no puede borrar el subdirectorio
                    if(result != 0)
                        //indica que no puede realizar la funcion
                        return 4;
                }
                //si la entrada es un archivo
                else
                    //borra la entrada
                    result = remove(entry.c_str());

                //si no ha podido borrar la entrada
                if(result != 0)
                    //indica que no ha podido realizar la funcion
                    return 5;
            }

            //---------------------

            //intenta obtener un puntero a la próxima entrada
            ep = readdir(dp);
        }
        (void) closedir (dp);
    }

    //borra la entrada correspondiente a la ruta
    result = remove(path.c_str());

    //si no ha podido borrar la entrada
    if(result != 0)
        //indica que no ha podido realizar la funcion
        return 5;

    //indica que ha podido realizar la función
    return 0;
}

//get the path of the current directory
string getCurrentDir(void)
{
    char cwd[PATH_MAX];

    if(getcwd(cwd, PATH_MAX) != NULL)
        return string(cwd);
    else
        throw Exception("error calling funtion getcwd");
}
//force the building of a path of directories
void ForceDirectories(const AnsiString& S)
{
    try {
        mkpath(S.c_str());
    }
    catch(...) {
        throw;
    }

    //Other way to make this:
    //    QDir Dir;
    //    Dir.mkpath(S.c_str());
}

//split a path
void splitpath(string& parent_path, string& filename, const string& path)
{
    int i = path.length() - 1;
    while(i>=0 && path[i] != '/')
        i--;

    parent_path = "";
    if(i >= 0)
        parent_path = path.substr(0, i);

    filename = "";
    int aux = path.length() - 1;
    if(i < aux) {
        int count = aux - i;
        filename = path.substr(i+1, count);
    }

    //WARNING: in GCC 2.95 it is not recognized:
    //    string::clear()
}
//determine if a path correspond to a existing file
bool isfile(const string& path)
{
    //read the attributes of the path
    struct stat sb;
    int result = stat(path.c_str(), &sb);

    //chack that has read the attributes
    if(result != 0)
        return false;

    //check that it is not a directory
    if(S_ISDIR(sb.st_mode))
        return false;

    return false;
}

//extract the filename without extension
string stem(string path)
{
    string dir, filename;
    splitpath(dir, filename, path);
    int i = filename.rfind('.');
    if(i >= 0)
        return filename.substr(0, i);
    return filename;
}

//---------------------------------------------------------------------------
//TStrings

//capacidad del array de cadenas
void TStrings::setCapacity(int t_Capacity)
{
    //Capacity debería ser no negativa
    if(t_Capacity < 0)
        throw EImproperArgument("Capacity debería ser no nonnegative");

    //si la capacidad es menor que el número de elementos
    if(t_Capacity < Count) {
        //construye un array con la nueva capacidad
        AnsiString **t_Items = new AnsiString*[t_Capacity];
        //copia los elementos
        int i;
        for(i=0; i<t_Capacity; i++)
            t_Items[i] = Items[i];
        //libera los elementos sobrantes
        for(; i<Count; i++)
            delete Items[i];
        //libera el array antiguo
        delete Items;
        //asigna el array nuevo
        Items = t_Items;
        Count = Capacity = t_Capacity;
    }
    //si no, si la capacidad es mayor que el número de elementos
    else if(t_Capacity >= Count) {
        //construye un array con la nueva capacidad
        AnsiString **t_Items = new AnsiString*[t_Capacity];
        //copia los elementos
        int i;
        for(i=0; i<Count; i++)
            t_Items[i] = Items[i];
        //libera el array antiguo
        delete Items;
        //asigna el array nuevo
        Items = t_Items;
        Capacity = t_Capacity;
    }
}

//número de cadenas de texto
void TStrings::setCount(int t_Count)
{
    //Count debería ser no negativo
    if(t_Count < 0)
        throw EImproperArgument("Count should be nonnegative");

    //si el número de elementos excede la capacidad
    if(t_Count > Capacity)
        //redimensiona el array
        setCapacity(t_Count);

    //si el número de elementos es menor que el original
    if(t_Count < Count) {
        //borra los elementos sobrantes
        for(int i=t_Count; i<Count; i++)
            delete Items[i];
        //actualiza el número de elementos
        Count = t_Count;
    }
    //si no, si el número de elementos es mayor que el original
    else if(t_Count > Count) {
        //construye los elementos nuevos por defecto
        for(int i=Count; i<t_Count; i++)
            Items[i] = new AnsiString();
        //actualiza el número de elementos
        Count = t_Count;
    }
    //si el número de elementos es igual que el original no hace nada
}

//contruye una lista de cadenas por defecto
TStrings::TStrings(void) : Capacity(8), Count(0)
{
    //reserva punteros para las Capacity cadenas
    Items = new AnsiString*[Capacity];
}
//libera la memoria dinámica
TStrings::~TStrings()
{
    //libera cada AnsiString que ha sido reservado mediante new
    for(int i=0; i<Count; i++)
        delete Items[i];
}
//clona una lista de cadenas
void TStrings::Clone(const TStrings& Strings)
{
    //libera todas las cadenas
    for(int i=0; i<Count; i++)
        delete Items[i];
    //actualiza el número de elementos
    Capacity = Strings.getCapacity();
    Count = Strings.getCount();
    //reserva punteros para las Capacity cadenas
    Items = new AnsiString*[Capacity];
    //clona cada cadena
    for(int i=0; i<Count; i++)
        Items[i] = new AnsiString(Strings[i]);
}
//copia una lista de cadenas
void TStrings::Copy(const TStrings& Strings)
{
    //match the number of strings
    setCount(Strings.getCount());

    //copy the strings;
    for(int i=0; i<Count; i++)
        *(Items[i]) = Strings[i];
}
TStrings& TStrings::operator=(const TStrings& Strings)
{
    //match the number of strings
    setCount(Strings.getCount());

    //copy the strings;
    for(int i=0; i<Count; i++)
        *(Items[i]) = Strings[i];

    return *this;
}

//accede a la cadena de texto idnicada
AnsiString& TStrings::operator[](int i)
{
    //el índice i debería indicar a una cadena de texto de la lista
    if(i<0 || Count<=i)
        throw EImproperArgument("index i should indicate to AnsiString of the list");

    return *(Items[i]);
}
//accede a la cadena de texto constante idnicada
const AnsiString& TStrings::operator[](int i) const
{
    //el índice i debería indicar a una cadena de texto de la lista
    if(i<0 || Count<=i)
        throw EImproperArgument("index i should indicate to AnsiString of the list");

    return *(Items[i]);
}
//determina si dos listas de cadenas tienen todas las cadenas iguales
bool TStrings::operator==(const TStrings& Strings) const
{
    //si el número de cadenas difiere
    if(Strings.Count != Count)
        //indica que las listas no son iguales
        return false;

    //por cada cadena de la lista
    for(int i=0; i<Count; i++)
        //si las cadenas indicadas difieren entre si
        if(*(Items[i]) != Strings[i])
            //indica que las listas no son iguales
            return false;

    //indica que no se encontraron diferencias
    return true;
}
//determina si dos listas de cadenas tienen alguna cadena desigual
bool TStrings::operator!=(const TStrings& Strings) const
{
    //si el número de cadenas difiere
    if(Strings.Count != Count)
        //indica que las listas son diferentes
        return true;

    //por cada cadena de la lista
    for(int i=0; i<Count; i++)
        //si las cadenas indicadas difieren entre si
        if(*(Items[i]) != Strings[i])
            //indica que las listas son diferentes
            return true;

    //indica que no se encontraron diferencias
    return false;
}

//añade una copia de una cadena de texto al final de la lista
void TStrings::Add(const AnsiString& S)
{
    //reserva espacio para la nueva cadena
    setCount(Count+1);
    //copia la nueva cadena
    *(Items[Count-1]) = S;
}
void TStrings::Add(const char *chars)
{
    //el puntero chars debería apuntar a un caracter construido
    if(chars == NULL)
        throw EImproperArgument("pointer chars should point to built char");

    //reserva espacio para la nueva cadena
    setCount(Count+1);
    //copia la nueva cadena
    *(Items[Count-1]) = chars;
}

//inserta una copia de una cadena de texto en la posición indicada
void TStrings::Insert(int i, const AnsiString& S)
{
    //el índice i debería indicar a una cadena de texto de la lista
    if(i<0 || Count<=i)
        throw EImproperArgument("index i should indicate to AnsiString of the list");

    //reserva espacio para la nueva cadena
    setCount(Count+1);
    //desplaza las cadenas a partir de la posición i
    for(int j=Count-1; j>i; j--)
        Items[j] = Items[j-1];
    //inserta un clon de la nueva cadena
    Items[i] = new AnsiString(S);
}

//vacía la lista
void TStrings::Clear(void)
{
    //libera todas las cadenas
    for(int i=0; i<Count; i++)
        delete Items[i];
    //actualiza el número de elementos
    Count = 0;
}

//---------------------------------------------------------------------------
//class TStringList:

//assign a text string decomposing it in lines
void TStringList::setText(const AnsiString& S)
{
    throw Exception("TBD");
}

//add a copy of a text string by the end of the list
void TStringList::Add(const AnsiString& S)
{
    //reserva espacio para la nueva cadena
    Strings.setCount(Strings.getCount()+1);
    //copia la nueva cadena
    Strings[Strings.getCount()-1] = S;
}
void TStringList::Add(const char *chars)
{
    //el puntero chars debería apuntar a un caracter construido
    if(chars == NULL)
        throw EImproperArgument("pointer chars should point to built char");

    //reserva espacio para la nueva cadena
    Strings.setCount(Strings.getCount()+1);
    //copia la nueva cadena
    Strings[Strings.getCount()-1] = chars;
}

//---------------------------------------------------------------------------
//KEYBOARD:

//struct termios initial_setting;
//struct termios new_setting;
//int Keyboard::peek_character = -1;

void Keyboard::initNonCannonical(void)
{
    tcgetattr(0, &initial_setting);
    new_setting = initial_setting;
    new_setting.c_lflag &= ~ICANON;
    new_setting.c_lflag &= ~ECHO;
    new_setting.c_lflag &= ~ISIG;
    new_setting.c_cc[VMIN] = 1;
    new_setting.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_setting);
}

void Keyboard::restoreSetting(void)
{
    tcsetattr(0, TCSANOW, &initial_setting);
}

int Keyboard::kbhit(void)
{
    if(peek_character != -1)
        return 1;

    new_setting.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &new_setting);

    char ch;
    ssize_t nread = read(0, &ch, 1);

    new_setting.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_setting);

    if(nread > 0) {
        peek_character = ch;
        return nread;
    }
    return 0;
}

char Keyboard::getch(void)
{
    char ch;

    if(peek_character != 1) {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }

    ssize_t nread = read(0, &ch, 1);
    if(nread != 1)
        throw EImpossibleError("error using ssize_t read(int__fd, void*, size_t__nbytes)");

    return ch;
}

//---------------------------------------------------------------------------
