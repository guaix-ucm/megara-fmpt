//---------------------------------------------------------------------------
//Archivo: vclemu.h
//Contenido: emulador de VCL
//Última actualización: 03/10/2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef VCLEMU_H
#define VCLEMU_H

//#include <QCoreApplication> //std::exception
#include <string>
//##include <QColor>

//---------------------------------------------------------------------------

//Límites numéricos:
//
//en values.h:
#define DSIGNIF                 53 //número de bits de la mantisa de un double
//#define MAXINT                  0x7fffffff //valor máximo para un entero
#define MAXDOUBLE               1.7976931348623158E+308 //valor máximo para un double
//#define MINDOUBLE               2.2250738585072014E-308 //valor mínimo (positivo) para un double
//con std:
//  numeric_limits<int>::max()      -
//  numeric_limits<double>::min()   -
//  numeric_limits<double>:_max()   -
//
//Encabezado necesario: <limits>
//
//Problema, el valor máximo de un double no se puede imprimir con
//QString::toDouble(), produciéndose un error.
//Solución, tomar los valores extremos como +/- INF.

//define un separador decimal
extern char DecimalSeparator;

//clase pantalla
class TScreen {
public:
    int Width;
    int Height;
    double PixelsPerInch;

    TScreen(void) : Width(1024), Height(768), PixelsPerInch(91.43) {;}
};

//define una pantalla
extern TScreen Screen;

//debe existir otro modo de acceeder a estas propiedades.

//inicializa la semilla de los números pseudoaleatorios
void randomize(void);
//genera un número aleatorio en [0, max)
unsigned int random(unsigned int max);

//---------------------------------------------------------------------------
//AnsiString

using namespace std; //string

//clase AnsiString
class AnsiString {
public:
    //cadena contenedora
    string str;

    //MÉTODOS PÚBLICOS:

    //construye una AnsiString por defecto
    AnsiString(void);
    //construye un AnsiString a partir de una char
    AnsiString(const char);
    //construye un AnsiString a partir de una char*
    AnsiString(const char*);
    //construye un AnsiString a partir de una string
    AnsiString(const string&);
    //construye una AnsiString a partir de una QString
    //#AnsiString(const QString&);

    //construye una AnsiString a partir de un int
    AnsiString(int);
    //construye una AnsiString a partir de un double
    AnsiString(double);

    //contatena dos cadenas
    AnsiString operator+(const AnsiString&) const;
    //contatena dos cadenas
    AnsiString operator+(const string&) const;
    //concatena una cadena a esta
    AnsiString& operator+=(const AnsiString&);
    //concatena un caracter a esta cadena
    AnsiString& operator+=(const char&);
    //copia una AnsiString
    AnsiString& operator=(const AnsiString&);
    //copia una char* (no la ñade, sino que la copia)
    AnsiString& operator=(const char*);
    //determina si una cadena es igual a esta
    bool operator==(const AnsiString&) const;
    //determina si una cadena es desigual a esta
    bool operator!=(const AnsiString&) const;

    //lectura del caracter indicado en cadenas no constantes
    char& operator[](int i);
    //lectura del caracter indicado en cadenas constantes
    const char& operator[](int i) const;

    //puntero al primer caracteer de la cadena
    const char *c_str(void) const {return str.c_str();}
    //determina la longitud de una cadena
    int Length() const {return str.length();}
    //cambia la longitud de una cadena
    void SetLength(int);

    //obtiene la subcadena en el intervalo [offset, offset+count]
    //si offset o count no indican una posición de la cadena Src:
    //  lanza EImproperArgument
    AnsiString SubString(int offset, int count) const;

    //convierte la cadena a double
    double ToDouble() const;
    //convierte la cadena a entero
    int ToInt() const;
    //convierte la cadena a entero
    //en base hexadecimal
    int ToHex() const;
/*#    //convierte la cadena a entero sin signo
    //en base hexadecimal
    uint ToUHex() const;*/
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
    Exception(AnsiString _Message) : exception(), Message(_Message) {;}
    //libera la memoria
    ~Exception() throw() {;}

    //accede al mensaje
    const char* what() const throw() {return Message.str.c_str();}
};

//---------------------------------------------------------------------------
//FUNCIONES DE CONVERSIÓN:

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
AnsiString IntToHex(int n, int fig);
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
//FUNCIONES DEL SISTEMA DE ARCHIVOS:

//construye una ruta de directorios
int mkpath(const string& dir);

//obtiene la ruta del directorio actual
AnsiString GetCurrentDir(void);
//construye un directorio
void ForceDirectories(const AnsiString&);

//---------------------------------------------------------------------------
//TStrings

//clase array de cadenas de texto
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

//variables globales para contener las cadenas correspondientes a true y false
//según StrToBool y BoolToStr
extern TStrings TrueBoolStrs;
extern TStrings FalseBoolStrs;

//convierte de Boolean a AnsiString
AnsiString __fastcall BoolToStr(bool B, bool UseBoolStrs=false);
//convierte de AnsiString a Boolean
bool __fastcall StrToBool(const AnsiString S);

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

struct TPoint
{
    int  x;
    int  y;

    TPoint() {}
    TPoint(int _x, int _y) : x(_x), y(_y) {}
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
