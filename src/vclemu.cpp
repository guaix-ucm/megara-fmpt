// Copyright (c) 2014-2015 Isaac Morales Durán. All rights reserved.
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
//File: vclemu.cpp
//Content: VCL emulator
//Last update: 14/02/2015
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "vclemu.h"
#include "Exceptions.h"

#include <cmath>
#include <climits> //PATH_MAX
#include <limits> //std::numeric_limits
#include <stdlib.h> //rands, rand
#include <sstream> //ostringstream
#include <stdlib.h> //strtod
#include <unistd.h> //getcwd
#include <sys/stat.h> //stat, S_ISDIR
#include <locale.h> //struct lconv, localeconv()
#include <cstring> //strlen
#include <stdio.h> //printf, scanf

//---------------------------------------------------------------------------

//lee elvalor del separador decimal
char get_decimal_separator(void)
{
    struct lconv *lc;
    lc = localeconv();
    char *s = lc->decimal_point;

    if(strlen(s) != 1)
        throw exception();

    return s[0];
}

//inicializa la semilla de los números pseudoaleatorios
void randomize(void)
{
    srand(time(NULL));
}
//genera un número aleatorio en [0, max)
unsigned int random(unsigned int max) {
    //genera el número en [0, RAND_MAX)
    unsigned int n = rand();
    //normaliza el número
    double x = double(n)/double(RAND_MAX);
    //obtiene el número en [0, max)
    x *= max;
    //toma la parte entera
    n = floor(x);
    //devuelve la parte entera
    return n;

    //para realizar la portabilidad a otro sistema,
    //deberán sustituirse:
    //  rand    ->  qrand   (en stdlib.h)
    //  floor   ->  qFloor  (en Qt/Core/qmat.h)
}

//---------------------------------------------------------------------------
//AnsiString

//MÉTODOS PÚBLICOS:

//construye una AnsiString por defecto
AnsiString::AnsiString(void) : str()
{
}
//construye un AnsiString a partir de una char
AnsiString::AnsiString(const char c) : str()
{
    str = c;
}
//construye un AnsiString a partir de una char*
AnsiString::AnsiString(const char *cs) : str(cs)
{
}
//construye un AnsiString a partir de una string
AnsiString::AnsiString(const string& t_str)
{
    str = t_str;
}
/*#//construye una AnsiString a partir de una QString
AnsiString::AnsiString(const QString& QS) : str(QS.toLocal8Bit())
{
}*/

//construye una AnsiString a partir de un int
AnsiString::AnsiString(int x)
{
    *this = IntToStr(x);
}
//construye una AnsiString a partir de un double
AnsiString::AnsiString(double x)
{
    *this = FloatToStr(x);
}

//contatena dos cadenas
AnsiString AnsiString::operator+(const AnsiString& S) const
{
    return AnsiString(str + S.str);
}
//contatena dos cadenas
AnsiString AnsiString::operator+(const string& t_str) const
{
    return AnsiString(str + t_str);
}
//concatena una cadena a esta
AnsiString& AnsiString::operator+=(const AnsiString& S)
{
    str += S.str;
    return *this;
}
//concatena un caracter a esta cadena
AnsiString& AnsiString::operator+=(const char& c)
{
    str += c;
    return *this;
}
//copia una AnsiString
AnsiString& AnsiString::operator=(const AnsiString& S)
{
    str = S.str;
    return *this;
}
//copia una string
AnsiString& AnsiString::operator=(const string& t_str)
{
    str = t_str;
    return *this;
}
//copia una char* (no la ñade, sino que la copia)
AnsiString& AnsiString::operator=(const char *chars)
{
    //el puntero chars debería apuntar a un caracter construido
    if(chars == NULL)
        throw EImproperArgument("pointer chars should point to built char");

    str = chars;
    return *this;
}
//determina si una cadena es igual a esta
bool AnsiString::operator==(const AnsiString& S) const
{
    if(str == S.str)
        return true;
    return false;
}
//determina si una cadena es desigual a esta
bool AnsiString::operator!=(const AnsiString& S) const
{
    if(str == S.str)
        return false;
    return true;
}

//lectura del caracter indicado en cadenas no constantes
char& AnsiString::operator[](int i)
{
    //i debe indicar un caracter existente
    if(i<1 || Length()<i)
        throw Exception("index i out bounds");

    return str[i-1];
}
//lectura del caracter indicado en cadenas constantes
const char& AnsiString::operator[](int i) const
{
    //i debe indicar un caracter existente
    if(i<1 || str.length()<(unsigned int)i)
        throw EImproperArgument("index i out bounds");

    return str[i-1];
}

//cambia la longitud de una cadena
void AnsiString::SetLength(int n)
{
    try {
        str.resize(n);
    } catch(...) {
        throw;
    }
}

//insert a char in a position of the string
//if index i not indicate a position in the string S
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
    if(i<0 || Length()+1<i)
        throw EImproperArgument("index i shall indicates a position in the string");

    for(int i=1; i<=S.Length(); i++) {
        char c = S[i];
        str.insert(i, sizeof(typeof(c)), c);
    }
}

//obtiene la subcadena en el intervalo [offset, offset+count]
AnsiString AnsiString::SubString(int offset, int count) const
{
    //el índice offset debería indicar a una posiciíon de la cadena
    if(offset<1 || Length()<offset)
        throw EImproperArgument("index offset should indicate a position in the string");

    //el índice count debería indicar a una posiciíon de la cadena
    if(count<1 || Length()<count)
        throw EImproperArgument("index count should indicate a position in the string Src");

    AnsiString Dest;
    offset--;
    for(int i=0; i<count; i++)
        Dest += str[offset+i];

    return Dest;
}

//convierte la cadena a double
double AnsiString::ToDouble() const
{
    try {
        stringstream ss(str);
        ss.precision(std::numeric_limits<double>::digits10);
        double value;
        bool ok = (ss >> value);
        if(!ok) {
            throw EImproperArgument(AnsiString("can't convert string to double"));
        }

        //WARNING: if precision is upper to digits10 decimals, conversión to
        //string can introduce numerial error for somevalues.Example:
        //  value       ->  string
        //  87.035553	->  "87.03555299999999"
        //  -70.35		->  "-70.34999999999999"

        return value;

    } catch(...) {
            throw;
    }
}
//convierte la cadena a entero
int AnsiString::ToInt() const
{
    try {
        istringstream ss(str);
        ss.precision(std::numeric_limits<double>::digits10);
        int value;
        bool ok = (ss >> value);
        if(!ok) {
            throw EImproperArgument(AnsiString("can't convert string to int"));
        }

        //WARNING: if precision is upper to digits10 decimals, conversión to
        //string can introduce numerial error for somevalues.Example:
        //  value       ->  string
        //  87.035553	->  "87.03555299999999"
        //  -70.35		->  "-70.34999999999999"

        return value;

    } catch(...) {
            throw;
    }
}

//No sabemos como configurar un stringstream para que funcione en base hexadecimal.
//Por eso vamos a comentar las funciones de conversión en base hexadecimal
//hasta que sean necesarias.

//convierte la cadena a entero
//en base hexadecimal
int AnsiString::ToHex() const
{
    try {
        int value = (int)strtol(str.c_str(), NULL, 16);
        return value;

    } catch(...) {
            throw;
    }
}
/*#
//convierte la cadena a entero
//en base hexadecimal
uint AnsiString::ToUHex() const
{
    //indica a la primera posición
    uint i = 0;
    //descarta los separadores iniciales
    while(str[i]==' ' || str[i]=='\t')
        i++;
    //descarta los ceros iniciales
    while(str[i]=='0')
        i++;

    //copia el resto de la cadena en una QString
    QString QS;
    for(; i<str.length(); i++)
        QS += str[i];

    //intenta traducir a int
    bool ok;
    uint n = QS.toUInt(&ok, 16);

    //si no ha tenido éxito
    if(!ok)
        throw Exception(str+" not contains an integer in hexadecimal base");

    //devuelve el resultado
    return n;
}
*/
//---------------------------------------------------------------------------
//FUNCIONES DE CONVERSIÓN:

//translate from double to string
string floattostr(double value)
{
    stringstream ss;

    ss.precision(std::numeric_limits<double>::digits10);
    bool ok = (ss << value);
    if(!ok) {
        throw EImproperArgument(AnsiString("can't convert double to string"));
    }

    //WARNING: if precision is upper to digits10 decimals, conversión to
    //string can introduce numerial error for somevalues.Example:
    //  value       ->  string
    //  87.035553	->  "87.03555299999999"
    //  -70.35		->  "-70.34999999999999"

    return ss.str();
}
//translate from int to string
string inttostr(int value)
{
    stringstream ss;

    bool ok = (ss << value);
    if(!ok) {
        throw EImproperArgument(AnsiString("can't convert int to string"));
    }

    return ss.str();
}

//translate from string to double
double strtofloat(const string& str)
{
    try {
        stringstream ss(str);
        ss.precision(std::numeric_limits<double>::digits10);
        double value;
        bool ok = (ss >> value);
        if(!ok) {
            throw EImproperArgument(AnsiString("can't convert string to double"));
        }

        //WARNING: if precision is upper to digits10 decimals, conversión to
        //string can introduce numerial error for somevalues.Example:
        //  value       ->  string
        //  87.035553	->  "87.03555299999999"
        //  -70.35		->  "-70.34999999999999"

        return value;

    } catch(...) {
            throw;
    }
}
//translate from string to int
int strtoint(const string& str)
{
    try {
        istringstream ss(str);
        ss.precision(std::numeric_limits<double>::digits10);
        int value;
        bool ok = (ss >> value);
        if(!ok) {
            throw EImproperArgument(AnsiString("can't convert string to int"));
        }

        //WARNING: if precision is upper to digits10 decimals, conversión to
        //string can introduce numerial error for somevalues.Example:
        //  value       ->  string
        //  87.035553	->  "87.03555299999999"
        //  -70.35		->  "-70.34999999999999"

        return value;

    } catch(...) {
            throw;
    }
}
//translate from string to bool
bool strtobool(const string& str)
{
    try {
        istringstream ss(str);
        ss.precision(std::numeric_limits<double>::digits10);
        bool value;
        bool ok = (ss >> value);
        if(!ok) {
            throw EImproperArgument(AnsiString("can't convert string to bool"));
        }

        return value;

    } catch(...) {
            throw;
    }
}

double StrToFloat(const AnsiString& S)
{
    return S.ToDouble();
}
//traduce de double a AnsiString
AnsiString FloatToStr(double value)
{
    AnsiString S(floattostr(value));
    return S;
}

//traduce de AnsiString a integer
int StrToInt(const AnsiString& S)
{
    return S.ToInt();
}
//traduce de integer a AnsiString
AnsiString IntToStr(int value)
{
    AnsiString S(inttostr(value));
    return S;
}

//traduce de AnsiString a integer
//en base hexadecimal
int HexToInt(const AnsiString& S)
{
    return S.ToHex();
}
//traduce de integer a AnsiString
//en base hexadecimal
AnsiString IntToHex(intptr_t n, int fig)
{
    //el número de cifras debe ser sizeof(n)
    if(fig != 2*sizeof(n))
        throw EImproperArgument(AnsiString("number of figures n should be ")+IntToStr(sizeof(n)));

    AnsiString S;
    S.SetLength(8);
    // FIXME: itoa is not standard
    //itoa(n, (char*)S.c_str(), 16);
    return S;
//#    return AnsiString(QString::number(n, 16).toUpper());
}
/*#
//traduce de AnsiString a unsigned integer
//en base hexadecimal
uint HexToUInt(const AnsiString& S)
{
    return S.ToUHex();
}
//traduce de unsigned integer a AnsiString
//en base hexadecimal
AnsiString UIntToHex(uint n, int fig)
{
    //el número de cifras debe ser sizeof(n)
    if(fig != 2*sizeof(n))
        throw EImproperArgument(AnsiString("number of figures n should be ")+IntToStr(sizeof(n)));

//#    return AnsiString(QString::number(n, 16).toUpper());
}

//traduce de QColor a AnsiString
AnsiString ColorToString(QColor color)
{
    QString QS = color.name(QColor::HexArgb);
    string str(QS.toLatin1());
    return AnsiString(str);

    //Otra forma de traducir:

    //return UIntToHex(uint(Color.rgba()), 8);

    //QRgb QColor::rgba() const
    //Returns the RGB value of the color, including its alpha.
    //For an invalid color, the alpha value of the returned color
    //is unspecified.

    //typedef QRgb
    //An ARGB quadruplet on the format #AARRGGBB, equivalent to an
    //unsigned int.
    //The type also holds a value for the alpha-channel. The default
    //alpha channel is ff, i.e opaque. For more information, see the
    //Alpha-Blended Drawing section.
}
//traduce de AnsiString a QColor
QColor StringToColor(const AnsiString& S)
{
    try {
        QString QS(S.c_str());
        QColor color;
        color.setNamedColor(QS);

        //Otra forma de traducir:

        //traduce de AnsiString a QColor
        //uint n = HexToUInt(S);
        //QRgb Rgb = QRgb(n);
        //QColor t_Color;
        //_Color.setRgba(Rgb);

        //asigna el nuevo color
        //Color = t_Color;

        return color;

    } catch(...) {
        throw;
    }
}
*/
//---------------------------------------------------------------------------
//FUNCIONES DEL SISTEMA DE ARCHIVOS:

//construye una ruta de directorios
int mkpath(const string& path)
{
    //si la ruta está vacía
    if(path.length() <= 0)
        //indica que la ruta no puede ser construida
        return 1;

    //si la ruta no es válida
        //indica que la ruta no puede ser construida
//        return 1;

    //ERROR: hace falta una función para comrpobar
    //si una ruta de directorios es válida. Este error
    //tendrá el efecto de que los dir3ectorios serán
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

//obtiene la ruta del directorio actual
string getCurrentDir(void)
{
    char cwd[PATH_MAX];

    if(getcwd(cwd, PATH_MAX) != NULL)
        return string(cwd);
    else
        throw Exception("error calling funtion getcwd");
}
//construye un directorio
void ForceDirectories(const AnsiString& S)
{
    try {
        mkpath(S.c_str());

     } catch(...) {
        throw;
    }

/*#//    QString QS(S.c_str());
    QDir Dir;
    Dir.mkpath(S.c_str());*/
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

//accede a la cadena de texto idnicada
AnsiString& TStrings::operator[](int i)
{
    //el índice i debería indicar a una cadena de texto de la lista
    if(i<0 || Count<=i)
        throw EImproperArgument("index i should indicate to AnsiString of the list");

    return *(Items[i]);
}
//accede a la cadena de texto idnicada
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
        //sindica que las listas no son iguales
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
        //sindica que las listas son diferentes
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
//TrueBoolStrs and FalseBoolStrs:

//inicializa las variables globales
TStrings TrueBoolStrs;
TStrings FalseBoolStrs;

//---------------------------------------------------------------------------
//TStringList:

//asigna una cadena de texto descomponiéndola en líneas
void TStringList::setText(const AnsiString& S)
{
    throw Exception("falta por definir");
}

//añade una copia de una cadena de texto al final de la lista
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
