//---------------------------------------------------------------------------
//Archivo: Scalars.h
//Contenido: funciones escalares
//Última actualización: 07/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef SCALARS_H
#define SCALARS_H

#include "vclemu.h"
#include "Exceptions.h" //excepciones no predefinidas

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

/*comentado por dar error:
    multiple definition of `Mathematics::MinN(int, ...)'

//determina el mínimo de N argumentos
double MinN(int N, ...);
//determina el máximo de N argumentos
double MaxN(int N, ...);
*/

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

//NOTA: __rtti permite determinar el tipo de objeto en tiempo de ejecución.

//determina la mantisa y el exponente en base 10
double frexp10(double x, int& exponent10);
//determina el nº de cifras decimales significativas de x en base 10
int nsigfig(double x);
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

/*#//---------------------------------------------------------------------------
//Funciones de colores:

//QColor dispone de los siguientes métodos:
//extrae las componentes RGB de un color
//  void	getRgb(int * r, int * g, int * b, int * a = 0) const
//asigna las compnentes RGB a un color
//  void	setRgb(int r, int g, int b, int a = 255)

//extrae las componentes RGB de un color
void ColorToRGB(unsigned char &R, unsigned char &G, unsigned char &B,
    QColor Color);
//asigna las compnentes RGB a un color
void RGBToColor(QColor &Color,
    unsigned char R, unsigned char G, unsigned char B);

//determina la luminancia de un color L = R+G+B
int Luminance(QColor Color);

//multiplica las coomponentes de un color por un factor de escala
//si alguna de las componentes resultantes excede de 0xFF le asigna 0xFF
QColor HighlightColor(QColor Color, double f);

//resalta un color al máximo manteniendo las proporciones entre sus componentes
QColor HighlightColor(QColor Color);

//obtiene el color complementario de un color
QColor ComplementaryColor(QColor Color);
*/
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

/*//determina el mínimo de N argumentos
double MinN(int N, ...)
{
    //el número de argumentos N debería ser al menos uno
    if(N < 1)
        throw EImproperArgument(AnsiString("number arguments N should be one almost"));

    //abre la lista de argumentos
    va_list ap; //puntero a argumento
    va_start(ap, N); //apunta al siguiente argumento a N

    //inicializa el mínimo con el primer argumento
    double min = va_arg(ap, double);

    //por ada argumento posterior
    double arg;
    for (int i=1; i<N; i++) {
        //extrae el valor del argumento
    arg = va_arg(ap, double);
        //si el argumento es menor que el mínimo
        if(arg < min)
            min = arg; //actualiza el mínimo
    }

    //cierra la lista de argumentos
    va_end(ap);

    return min;
}
//determina el máximo de N argumentos
double MaxN(int N, ...)
{
    //el número de argumentos N debería ser al menos uno
    if(N < 1)
        throw EImproperArgument("number arguments N should be one almost");

    //abre la lista de argumentos
    va_list ap; //puntero a argumento
    va_start(ap, N); //apunta al siguiente argumento a N

    //inicializa el mínimo con el primer argumento
    double max = va_arg(ap, double);

    //por ada argumento posterior
    double arg;
    for (int i=1; i<N; i++) {
        //extrae el valor del argumento
    arg = va_arg(ap, double);
        //si el argumento es mayor que el máximo
        if(arg > max)
            max = arg; //actualiza el máximo
    }

    //cierra la lista de argumentos
    va_end(ap);

    return max;
}
*/
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
    int Id;

public:
    //PROPIEDADES DE LA VARIABLE IDENTIFICADA:

    //número de identificación de la variable
    //debe ser un valor no negativo
    //valor por defecto: 0
    int getId(void) const {return Id;} void setId(const int);
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
    static void  Assign(TIdDouble &ID, double _Value);
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
    static void  Assign(TIdDouble *ID, double _Value);
    //imprime un elemento en una cadena
    static void  Print(AnsiString &S, const TIdDouble *ID);
    //escudriña una cadena en busca de un elemento
    static void  Read(TIdDouble *ID, const AnsiString &S, int &i);

    //MÉTODOS PÚBLICOS:

    //construye una variable tipo double identificada
    //con las propiedades por defecto
    TIdDouble(void) : Id(0), Value(0) {;}
    //construye una variable tipo double identificada
    //con el identificador indicado
    TIdDouble(int _Id) : Value(0) {
        try {
            setId(_Id);
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
    double lmin;
    double lmax;

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
    double getlmin(void) const {return lmin;}
    void setlmin(double);
    //límite superior del intervalo
    //debe ser mayor o igual que el límite inferior
    double getlmax(void) const {return lmax;}
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
    TInterval(TIntervalType _IntervalType=itOpen, double _lmin=0, double _lmax=0);

    //asigna los límites del intervalo simultaneamente
    void SetLimits(double _lmin, double _lmax);
};
/* comentado para evitar tener que traducirlo
//---------------------------------------------------------------------------
//TPairIntegers

//clase par de enteros
class TPairIntegers {
    //PROPIEDADES EN FORMATO TEXTO:

    AnsiString GetxText(void) const; void SetxText(const AnsiString&);
    AnsiString GetyText(void) const; void SetyText(const AnsiString&);

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    AnsiString GetText(void) const;
    void SetText(const AnsiString&);

    AnsiString GetRowText(void) const;
    void SetRowText(const AnsiString&);

public:
    //primer valor del par
    //valor por defecto: 0
    int x;
    //segundo valor del par
    //valor por defecto: 0
    int y;

    //PROPIEDADES EN FORMATO TEXTO:

    __property AnsiString xText = {read=GetxText, write=SetxText};
    __property AnsiString yText = {read=GetyText, write=SetyText};

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    //par de enteros en formato texto
    __property AnsiString Text = {read=GetText, write=SetText};
    //par de enteros en formato fila de texto
    __property AnsiString RowText = {read=GetRowText, write=SetRowText};

    //FUNCIONES ESTÁTICAS:

    //función de impresión de elementos tipo TPairIntegers
    static void  Print(AnsiString &S, const TPairIntegers *PI);
    //función de lectura de elementos tipo TPairIntegers
    static void  Read(TPairIntegers *PI,
        const AnsiString &S, int &i);

    //función de impresión de elementos tipo TPairIntegers
    static void  PrintRow(AnsiString &S, const TPairIntegers *PI);
    //función de lectura de elementos tipo TPairIntegers
    static void  ReadRow(TPairIntegers *PI,
        const AnsiString &S, int &i);

    //MÉTODOS PÚBLICOS:

    //construye un par de enteros por defecto
    TPairIntegers(void);
    //contruye un par de enteros con los valores indicados
    TPairIntegers(int _x, int _y);

    //copia las propiedades de un par
    TPairIntegers& operator=(const TPairIntegers &PI);
};
*/
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
    TPair(double _x=0, double _y=0);

    //asigna simultaneamente los valores al par
    void SetValues(double _x, double _y);
};
/* comentado para evitar tener que traducirlo
//---------------------------------------------------------------------------
//TPairIB

//Clase par (integer, boolean):
class TPairIB {
    //PROPIEDADES EN FORMATO TEXTO:

    AnsiString GetxText(void) const; void SetxText(const AnsiString&);
    AnsiString GetyText(void) const; void SetyText(const AnsiString&);

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    AnsiString GetText(void) const;
    void SetText(const AnsiString&);

    AnsiString GetRowText(void) const;
    void SetRowText(const AnsiString&);

public:
    //ETIQUETAS DE LAS PROPIEDADES DE DEFINICIÓN:

    //etiqueta delavariable entera
    //valor por defecto: "x"
    static AnsiString xLabel;
    //etiqueta de la variable lógica
    //valor por defecto: "y"
    static AnsiString yLabel;

    //PROPIEDADES DEL GRUPO:

    //variable entera
    //valor por defecto: 0
    int x;
    //variable double
    //valor por defecto: false
    bool y;

    //PROPIEDADES EN FORMATO TEXTO:

    __property AnsiString xText = {read=GetxText, write=SetxText};
    __property AnsiString yText = {read=GetyText, write=SetyText};

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    //par en formato texto
    __property AnsiString Text = {read=GetText, write=SetText};
    //par en formato fila de texto
    __property AnsiString RowText = {read=GetRowText, write=SetRowText};

    //FUNCIONES ESTÁTICAS DE COMPARACIÓN:

    //compara la variable x
    //      si P1->x < P2->x devuelve -1
    //      si P1->x > P2->x devuelve 1
    //      si P1->x == P2->x devuelve 0
    static int  Comparex(const TPairIB *P1, const TPairIB *P2);

    //FUNCIONES ESTÁTICAS DE CONJUNTOS DE PROPIEDADES:

    //imprime los valores de las propiedades de la clase
    //en formato texto entre paréntesis
    static void  Print(AnsiString &S, const TPairIB *P);
    //lee los valores de las etiquetas de las propiedades de la clase
    //en formato textoentre paréntesis
    static void  Read(TPairIB *P,
        const AnsiString &S, int &i);

    //imprime los valores de las propiedades de la clase
    //en formato fila de texto
    static void  PrintRow(AnsiString &S, const TPairIB *P);
    //lee los valores de las etiquetas de las propiedades de la clase
    //en formato fila de texto
    static void  ReadRow(TPairIB *P,
        const AnsiString &S, int &i);

    //obtiene las etiquetas de las propiedades de la clase
    //en formato texto entre paréntesis
    static AnsiString GetLabels(void);
    //obtiene las etiquetas de las propiedades de la clase
    //en formato fila de texto
    static AnsiString GetRowLabels(void);
    //atraviesa las etiquetas de la clase
    //en formato fila de texto
    static void  TravelRowLabels(const AnsiString &S,
        int &i);

    //MÉTODOS PÚBLICOS:

    //construye un par pordefecto
    TPairIB(void);
    //contruye un par con los valores indicados
    TPairIB(int _x, int _y);

    //copia las propiedades de un par
    TPairIB& operator=(const TPairIB&);
};

//---------------------------------------------------------------------------
//TPairID

//Clase par (integer, double):
class TPairID {
    //PROPIEDADES EN FORMATO TEXTO:

    AnsiString GetxText(void) const; void SetxText(const AnsiString&);
    AnsiString GetyText(void) const; void SetyText(const AnsiString&);

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    AnsiString GetText(void) const;
    void SetText(const AnsiString&);

    AnsiString GetRowText(void) const;
    void SetRowText(const AnsiString&);

public:
    //ETIQUETAS DE LAS PROPIEDADES:

    //etiqueta delavariable entera
    //valor por defecto: "x"
    static AnsiString xLabel;
    //etiqueta de la variable double
    //valor por defecto: "y"
    static AnsiString yLabel;

    //PROPIEDADES:

    //variable entera
    //valor por defecto: 0
    int x;
    //variable double
    //valor por defecto: 0
    double y;

    //PROPIEDADES EN FORMATO TEXTO:

    __property AnsiString xText = {read=GetxText, write=SetxText};
    __property AnsiString yText = {read=GetyText, write=SetyText};

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    //par en formato texto
    __property AnsiString Text = {read=GetText, write=SetText};
    //par en formato fila de texto
    __property AnsiString RowText = {read=GetRowText, write=SetRowText};

    //FUNCIONES ESTÁTICAS DE COMPARACIÓN:

    //compara la variable x
    //      si P1->x < P2->x devuelve -1
    //      si P1->x > P2->x devuelve 1
    //      si P1->x == P2->x devuelve 0
    static int  Comparex(const TPairID *P1, const TPairID *P2);

    //FUNCIONES ESTÁTICAS DE CONJUNTOS DE PROPIEDADES:

    //imprime los valores de las propiedades de la clase
    //en formato texto entre paréntesis
    static void  Print(AnsiString &S, const TPairID *P);
    //lee los valores de las etiquetas de las propiedades de la clase
    //en formato textoentre paréntesis
    static void  Read(TPairID *P,
        const AnsiString &S, int &i);

    //imprime los valores de las propiedades de la clase
    //en formato fila de texto
    static void  PrintRow(AnsiString &S, const TPairID *P);
    //lee los valores de las etiquetas de las propiedades de la clase
    //en formato fila de texto
    static void  ReadRow(TPairID *P,
        const AnsiString &S, int &i);

    //obtiene las etiquetas de las propiedades de la clase
    //en formato texto entre paréntesis
    static AnsiString GetLabels(void);
    //obtiene las etiquetas de las propiedades de la clase
    //en formato fila de texto
    static AnsiString GetRowLabels(void);
    //atraviesa las etiquetas de la clase
    //en formato fila de texto
    static void  TravelRowLabels(const AnsiString &S,
        int &i);

    //MÉTODOS PÚBLICOS:

    //construye un objeto de la clase
    //con los valores por defecto
    TPairID(void);
    //construye un objeto de la clase
    //con los valores indicados
    TPairID(int _x, int _y);

    //copia las propiedades de un objeto de la clase
    TPairID& operator=(const TPairID&);
};

//---------------------------------------------------------------------------
//TPairIS

//Clase par (integer, AnsiString):
class TPairIS {
    //PROPIEDADES EN FORMATO TEXTO:

    AnsiString GetxText(void) const; void SetxText(const AnsiString&);
    AnsiString GetyText(void) const; void SetyText(const AnsiString&);

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    AnsiString GetText(void) const;
    void SetText(const AnsiString&);

    AnsiString GetRowText(void) const;
    void SetRowText(const AnsiString&);

public:
    //ETIQUETAS DE LAS PROPIEDADES DE DEFINICIÓN:

    //etiqueta delavariable entera
    //valor por defecto: "x"
    static AnsiString xLabel;
    //etiqueta de la variable double
    //valor por defecto: "y"
    static AnsiString yLabel;

    //PROPIEDADES DEL GRUPO:

    //variable entera
    //valor por defecto: 0
    int x;
    //variable AnsiString
    //valor por defecto: ""
    AnsiString y;

    //PROPIEDADES EN FORMATO TEXTO:

    __property AnsiString xText = {read=GetxText, write=SetxText};
    __property AnsiString yText = {read=GetyText, write=SetyText};

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    //par en formato texto
    __property AnsiString Text = {read=GetText, write=SetText};
    //par en formato fila de texto
    __property AnsiString RowText = {read=GetRowText, write=SetRowText};

    //FUNCIONES ESTÁTICAS DE COMPARACIÓN:

    //compara la variable x
    //      si P1->x < P2->x devuelve -1
    //      si P1->x > P2->x devuelve 1
    //      si P1->x == P2->x devuelve 0
    static int  Comparex(const TPairIS *P1, const TPairIS *P2);

    //FUNCIONES ESTÁTICAS DE CONJUNTOS DE PROPIEDADES:

    //obtiene las etiquetas de las propiedades de definición
    //en formato (xLabel, yLabel)
    static AnsiString GetLabels(void);

    //función de impresión de pares
    //en formato (x, y)
    static void  Print(AnsiString &S, const TPairIS *P);
    //función de lectura de pares
    //en formato (x, y)
    static void  Read(TPairIS *P, const AnsiString &S, int &i);

    //obtiene las etiquetas
    //en formato fila de texto
    static AnsiString GetRowLabels(void);
    //función de atravesar las etiquetas
    //en formato fila de texto
    static void  TravelRowLabels(const AnsiString &S, int &i);

    //función de impresión de pares
    //en formato fila de texto
    static void  PrintRow(AnsiString &S, const TPairIS *P);
    //función de lectura de pares
    //en formato fila de texto
    static void  ReadRow(TPairIS *P, const AnsiString &S, int &i);

    //MÉTODOS PÚBLICOS:

    //construye un par por defecto
    TPairIS(void);
    //contruye un par con los valores indicados
    TPairIS(int _x, int _y);

    //copia las propiedades de un par
    TPairIS& operator=(const TPairIS&);
};
*/
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
    TPairTD(T *_P=NULL, double _x=0) : P(_P), x(_x) {;}
};

//---------------------------------------------------------------------------
//TTernIntegers

//clase terna de enteros
class TTernIntegers {
    //PROPIEDADES EN FORMATO TEXTO:

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

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
    TTernIntegers(int _x, int _y, int _z);

    //copia las propiedades de una terna
    TTernIntegers& operator=(const TTernIntegers &TI);
};

//---------------------------------------------------------------------------

} //namespace Mathematics

//---------------------------------------------------------------------------
#endif //SCALARS_H
