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
//Archivo: Scalars.h
//Contenido: scalar functions
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef SCALARS_H
#define SCALARS_H

#include "vclemu.h"
#include "Exceptions.h"

#include <typeinfo> //namespace std
#include <ctime> //clock_t

//---------------------------------------------------------------------------

using namespace std; //type_info

//espacio de nombres de clases y funciones matemáticas
namespace Mathematics {

//---------------------------------------------------------------------------
//Retardo:

//provoca un retardo de tiempo en clocks ocupando la CPU
//en SOs que no son de tiempo real 1 clock = 1 ms
void Delay(clock_t t);

//Función alternativa:
//      procedure Sleep(ATime: cardinal);
//      Parameters
//      ATime: cardinal
//      The number of milliseconds to sleep.
//      Suspends the current thread for the specified number of milliseconds.

//---------------------------------------------------------------------------
//Functiones escalares:

int Factorial(int n); //n(n-1)(n-2)···1; 0! = 1
int Binomial(int m, int n); //Binomio de newton
double Log(double a, double x); //logaritmo en base a de x
double Log2(double x); //Logaritmo en base dos
template <class T> T Sign(T x); //Función signo
template <class T> T Abs(T x); //Valor absoluto de un tipo numérico desconocido

//determina el mínimo de dos argumentos
template <class T> T Min(T x1, T x2);
//determina el máximo de dos argumentos
template <class T> T Max(T x1, T x2);

//ADVERTENCIAS:
//      Si se define una plantilla con un número indefinido de
//      argumentos, el compilador no podrá reconocer el tipo de dato:
//          template <class T> Min(int N, ...);
//          template <class T> Max(int N, ...);
//      es por ello que se definirán las funciones para tipos concretos.
//
//      Los argumentos indefinidos no pueden ser referencias
//      ya que las mismas deben ser inicializadas en la declaración.
//
//      Si pasa argumentos no double a las funciones:
//          double MinN(int N, ...);
//          double MaxN(int N, ...);
//      no darán error y no funcinarán correctamente. Debe prestarse especial
//      atención al paso de constantes, las cuales deberán tener todas el
//      punto decimal en el caso de funciones para tipos en punto flotante.

//determina el nº más próximo más próximo a x
//perteneciente al dominio [xmin, xmax]
template <class T> T LimitDomain(T x, T xmin, T xmax);

//determina el entero n más próximo tq [n-0.5 <= x < n+0.5)
int Round(float x);
//determina el entero más próximo al origen de coordenadas
//a una distancia inferior a la unidad del valor indicado
//      Near(x) = Floor(Abs(x))*Sign(x)
double Near(double x);
//determina el entero más lejano al origen de coordenadas
//a una distancia inferior a la unidad del valor indicado
//      Far(x) = Ceil(Abs(x))*Sign(x)
double Far(double x);

//genera un número real aleatorio con distribución uniforme en [0, 1]
double RandomReal(void);
//genera un número real aleatorio con distribución uniforme en [x1, x2]
double RandomUniform(double x1, double x2);
//genera un número entero aleatorio con distribución uniforme en [n1, n2]
int RandomUniform(int n1, int n2);
//desplaza theta al intervalo [0, M_2PI)
//una distancia múltiplo de M_2PI
double ArgPos(double theta);

//determina si un tipo es numérico
bool Numerical(const type_info& t);
//determina si un tipo es de punto flotante
bool FloatingPoint(const type_info& t);

//---------------------------------------------------------------------------
//Funciones de manipulación de tipos numéricos:

//determina el incremento mínimo de un double
//correspondiente a incrementar su mantisa una unidad
double incmin(double x);

//Incremento mínimo de la mantisa de un double:
//      normalmente la mantisa de un double tiene DSIGNIF+1=54 bits
//      y su incremento mínimo corresponde a:
//          1/pow(2., 54.) = 5.55111512312578E-17
//Incremento mínimo de un double 'x':
//          pow(2., xexp)/pow(2., 54.)
//donde xexp es el exponente binario de x.

//---------------------------------------------------------------------------
//Plantillas de funciones matemáticas:

//función signo
template <class T> T Sign(T x)
{
    if(x>=0)
        return 1;
    else
        return -1;
}

//valor absoluto
template <class T> T Abs(T x)
{
    //el arguemnto x debe ser de tipo numérico
    if(!Numerical(typeid(T)))
        throw EImproperArgument("argument x should be of a numerical type");

    if(x<0)
        return T(-1*x);

    return x;
}

//determina el mínimo de dos argumentos
template <class T> T Min(T x1, T x2)
{
    //los argumentos deben ser de tipo numérico
    if(!Numerical(typeid(T)))
        throw EImproperArgument("arguments should be of a numerical type");

    if(x1 <= x2)
        return x1;

    return x2;
}
//determina el máximo de dos argumentos
template <class T> T Max(T x1, T x2)
{
    //los argumentos deben ser de tipo numérico
    if(!Numerical(typeid(T)))
        throw EImproperArgument("arguments should be of a numerical type");

    if(x1 >= x2)
        return x1;

    return x2;
}

//determina el nº más próximo más próximo a x
//perteneciente al dominio [xmin, xmax]
template <class T> T LimitDomain(T x, T xmin, T xmax)
{
    //el argumento x debería ser tipo numérico
    if(!Numerical(typeid(T)))
        throw EImproperArgument("argument x should be of a numerical type");

    //el límite inferior debe ser menor que el límite superior
    if(xmax < xmin)
        throw EImproperArgument("lower mimit xmin shguld be less upper limit xmax");

    if(x<xmin)
        return xmin;

    if(xmax<x)
        return xmax;

    return x;
}

//---------------------------------------------------------------------------
//TIdDouble:
//---------------------------------------------------------------------------

//variable tipo double identificada
class TIdDouble {
protected:
    int p_Id;

public:
    //PROPIEDADES DE LA VARIABLE IDENTIFICADA:

    //número de identificación de la variable
    //debe ser un valor no negativo
    //valor por defecto: 0
    int getId(void) const {return p_Id;} void setId(const int);
    //valor de la variable
    //valor por defecto: 0
    double Value;

    //PROPIEDADES EN FORMATO TEXTO:

    AnsiString getIdText(void) const; void setIdText(const AnsiString &S);
    AnsiString getValueText(void) const; void setValueText(const AnsiString &S);
    AnsiString getText(void) const; void setText(const AnsiString &S);

    //MÉTODOS ESTÁTICOS PARA LISTAS:

    //compara dos elementos
    static int  Compare(TIdDouble ID1, TIdDouble ID2);
    //evalúa un elemento
    static double  Evaluate(TIdDouble ID);
    //asigna un valor a un elemento
    static void  Assign(TIdDouble &ID, double Value);
    //imprime un elemento en una cadena
    static void  Print(AnsiString &S, TIdDouble ID);
    //escudriña una cadena en busca de un elemento
    static void  Read(TIdDouble &ID, const AnsiString &S, int &i);

    //MÉTODOS ESTÁTICOS PARA LISTAS DE PUNTEROS:

    //compara dos elementos
    static int  Compare(const TIdDouble *ID1,
        const TIdDouble *ID2);
    //evalúa un elemento
    static double  Evaluate(const TIdDouble *ID);
    //asigna un valor a un elemento
    static void  Assign(TIdDouble *ID, double Value);
    //imprime un elemento en una cadena
    static void  Print(AnsiString &S, const TIdDouble *ID);
    //escudriña una cadena en busca de un elemento
    static void  Read(TIdDouble *ID, const AnsiString &S, int &i);

    //MÉTODOS PÚBLICOS:

    //construye una variable tipo double identificada
    //con las propiedades por defecto
    TIdDouble(void) : p_Id(0), Value(0) {;}
    //construye una variable tipo double identificada
    //con el identificador indicado
    TIdDouble(int Id) : Value(0) {
        try {
            setId(Id);
        } catch(...) {
            throw;
        }
    }

    //copia una variable tipo double identificada
    TIdDouble &operator=(const TIdDouble &ID);
};

//---------------------------------------------------------------------------
//TInterval:
//---------------------------------------------------------------------------

//tipo de intervalo:
//      itOpen: abierto ()
//      itLeft: cerrado por la izquierda [)
//      itRight: cerrado por la derecha (]
//      itClose: cerrado []
enum TIntervalType {itOpen, itLeft, itRight, itClose};

class TInterval {
    double p_lmin;
    double p_lmax;

public:
    //tipo de intervalo:
    //      itOpen: abierto ()
    //      itLeft: cerrado por la izquierda [)
    //      itRight: cerrado por la derecha (]
    //      itClose: cerrado []
    //valor por devecto: itOpen
    TIntervalType IntervalType;

    //límite inferior del intervalo
    //debe ser menor o igual que el límite superior
    double getlmin(void) const {return p_lmin;}
    void setlmin(double);
    //límite superior del intervalo
    //debe ser mayor o igual que el límite inferior
    double getlmax(void) const {return p_lmax;}
    void setlmax(double);

    //intervalo en formato texto
    AnsiString getText(void); void setText(const AnsiString&);

    //MÉTODOS ESTÁTICOS:

    //imprime un intervalo en una cadena de texto
    static void  Print(AnsiString &S, const TInterval *I);
    //escanea una cadena de texto en busca de un intervalo
    static void  Read(TInterval *I, const AnsiString &S, int &i);

    //MÉTODOS PÚBLICOS:

    //construye un intervalo
    TInterval(TIntervalType IntervalType=itOpen,
              double lmin=0, double lmax=0);

    //asigna los límites del intervalo simultaneamente
    void SetLimits(double lmin, double lmax);
};

//---------------------------------------------------------------------------
//TPair:

//par de variables en punto flotante
class TPair {
public:
    //primera variable del par
    double x;
    //segunda variable del par
    double y;

    //par en formato texto
    AnsiString getText(void); void setText(const AnsiString&);

    //MÉTODOS ESTÁTICOS:

    //imprime un par en una cadena de texto
    static void  Print(AnsiString &S, const TPair *P);
    //escanea una cadena de texto en busca de un par
    static void  Read(TPair *P, const AnsiString &S, int &i);

    //MÉTODOS PÚBLICOS:

    //construye un par
    TPair(double x=0, double y=0);

    //asigna simultaneamente los valores al par
    void SetValues(double x, double y);
};

//---------------------------------------------------------------------------
//TPairTD

//clase par (T*, double)
template <class T> class TPairTD {
//    T *__P;

public:
    //puntero a un objeto de tipo T
    //valor por defecto: NULL
    T *P;
    //variable de tipo double
    //valor por defecto: 0
    double x;

    //MÉTODOS ESTÁTICOS:

    //compara el valor de x de dos pares
    static int  Comparex(const TPairTD *P1, const TPairTD *P2) {
        //el puntero P1 debería apuntar a un par (T*, double) construido
        if(P1 == NULL)
            throw EImproperArgument("pointer P1 should point to built pair (T*, double)");
        //el puntero P2 debería apuntar a un par (T*, double) construido
        if(P2 == NULL)
            throw EImproperArgument("pointer P2 should point to built pair (T*, double)");

        //compara el valor de x y devuelve el resultado correspondiente
        if(P1->x < P2->x)
            return -1;
        if(P1->x > P2->x)
            return 1;
        return 0;
    }
    //compara el valor absoluto de x de dos pares
    static int  CompareAbsx(const TPairTD *P1, const TPairTD *P2) {
        //el puntero P1 debería apuntar a un par (T*, double) construido
        if(P1 == NULL)
            throw EImproperArgument("pointer P1 should point to built pair (T*, double)");
        //el puntero P2 debería apuntar a un par (T*, double) construido
        if(P2 == NULL)
            throw EImproperArgument("pointer P2 should point to built pair (T*, double)");

        //calcula el valor absoluto de cada x
        double x1 = Abs(P1->x);
        double x2 = Abs(P2->x);
        //compara el valor de x y devuelve el resultado correspondiente
        if(x1 < x2)
            return -1;
        if(x1 > x2)
            return 1;
        return 0;
    }
    //imprime el valor de x al final de una cadena de texto
    static void  Printx(AnsiString &S, const TPairTD *P) {
        //el puntero P debería apuntar a un par (T*, double) construido
        if(P == NULL)
            throw EImproperArgument("pointer P should point to built pair (T*, double)");

        //imprime el valor de x
        S += FloatToStr(P->x);
    }

    //MÉTODOS PÚBLICOS:

    //construye un par (T *P, double x)
    TPairTD(T *t_P=NULL, double t_x=0) : P(t_P), x(t_x) {;}
};

//---------------------------------------------------------------------------
//TTernIntegers

//clase terna de enteros
class TTernIntegers {
public:
    //ETIQUETAS DE LAS PROPIEDADES:

    //etiqueta de la variable entera x
    //valor por defecto: "x"
    static AnsiString xLabel;
    //etiqueta de la variable entera y
    //valor por defecto: "y"
    static AnsiString yLabel;
    //etiqueta de la variable entera z
    //valor por defecto: "z"
    static AnsiString zLabel;

    //PROPIEDADES:

    //primer valor de la terna
    //valor por defecto: 0
    int x;
    //segundo valor de la terna
    //valor por defecto: 0
    int y;
    //tercer valor de la terna
    //valor por defecto: 0
    int z;

    //PROPIEDADES EN FORMATO TEXTO:

    AnsiString getxText(void) const; void setxText(const AnsiString&);
    AnsiString getyText(void) const; void setyText(const AnsiString&);
    AnsiString getzText(void) const; void setzText(const AnsiString&);

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    //terna de enteros en formato texto
    AnsiString getText(void) const;
    void setText(const AnsiString&);
    //terna de enteros en formato fila de texto
    AnsiString getRowText(void) const;
    void setRowText(const AnsiString&);

    //FUNCIONES ESTÁTICAS:

    //compara la propiedad x de dos ternas TI1 y TI2
    //si TI1->x < TI2.x
    //      devuelve -1
    //si TI1->x > TI2.x
    //      devuelve 1
    //si TI1->x == TI2.x
    //      devuelve 0
    static int  Comparexs(const TTernIntegers *TI1,
        const TTernIntegers *TI2);

    //obtiene las etiquetas en formato texto:
    //      (xLabel, yLabel, zLabel)
    static AnsiString GetLabels(void);
    //función de impresión de elementos tipo TTernIntegers
    static void  Print(AnsiString &S, const TTernIntegers *TI);
    //función de lectura de elementos tipo TTernIntegers
    static void  Read(TTernIntegers *TI,
        const AnsiString &S, int &i);

    //obtiene las etiquetas en formato fila de texto:
    //      xLabel+"\t"+yLabel+"\t"+zLabel
    static AnsiString GetLabelsRow(void);
    //función de impresión de elementos tipo TTernIntegers
    static void  PrintRow(AnsiString &S, const TTernIntegers *TI);
    //función de lectura de elementos tipo TTernIntegers
    static void  ReadRow(TTernIntegers *TI,
        const AnsiString &S, int &i);

    //MÉTODOS PÚBLICOS:

    //construye un terna de enteros por defecto
    TTernIntegers(void);
    //contruye un terna de enteros con los valores indicados
    TTernIntegers(int x, int y, int z);

    //copia las propiedades de una terna
    TTernIntegers& operator=(const TTernIntegers &TI);
};

//---------------------------------------------------------------------------

} //namespace Mathematics

//---------------------------------------------------------------------------
#endif //SCALARS_H
