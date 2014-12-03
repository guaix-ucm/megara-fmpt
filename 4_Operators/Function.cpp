//---------------------------------------------------------------------------
//Archivo: Function.cpp
//Contenido: función continua de tiempo continuo definida en puntos de muestreo
//Última actualización: 24/06/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Function.h"
#include "..\3_Lists\PointersList.h"
#include "..\2_Strings\TextFile.h"

//---------------------------------------------------------------------------

using namespace Lists;
using namespace Strings;

//espacio de nombres de operadores matemáticos
namespace Operators {

//---------------------------------------------------------------------------

//instanciación de TVector<int> para que se reconozcan sus métodos miembro
//en los métodos de esa clase al pasar vectores de enteros como argumento
static TVector<int> VI(1);

//activa o desactiva la periodicidad afectando al los métodos de evaluación
//de la función.
void TFunction::setPeriodic(bool _Periodic)
{
    //la activación de la periodicidad requiere que todos los puntos
    //de muestreo se encuentren en el ciclo principal [0, T)
    if(!Periodic && Count && (X.getFirst()<0 || T<=X.getLast()))
        throw EImproperCall("sample point dont belong to [0, T)");

    //si no hay puntos de muestreo siempre se podrá activar la periodicidad

    Periodic = _Periodic;
}

//Establece o cambia el periodo T. Si la función es periódica,
//todos los puntos de muestreo deberán estar en el ciclo principal [0, T).
void TFunction::setT(double _T)
{
    //si la función es periódica, todos los puntos de muestreo deberán estar
    //en el ciclo principal [0, T)
    if(Periodic && Count && (X.getFirst()<0 || T<=X.getLast()))
        throw EImproperArgument("sample point dont belong to [0, T)");

    T = _T;
}

//---------------------------------------------------------------------------
//ACCESO A LOS PUNTOS DE MUESTREO:

double TFunction::getX(int i) const
{
    if(Periodic) {
        //la función periódica debe contener
        //almenos un punto fundamental
        if(Count < 1)
            throw EImproperCall("empty function");

        //transplanta i al ciclo principal
        int ncycles = floor(double(i)/Count);
        i -= ncycles*Count;

        //devuelve la abcisa del punto (virtual) indicado
        return X[i] + T*ncycles;
    } else { //si es aperiódica
        //debe indicar un punto fundamental existente
        if(i<0 || Count<=i)
            throw EImproperArgument("index i out bounds");

        //devuelve la abcisa del punto indicado
        return X[i];
    }
}

void TFunction::setX(int i, double x)
{
    //para tener sentido descriptivo, la abcisa debe ser finita
    if(x<=-MAXDOUBLE || MAXDOUBLE<=x)
        throw EImproperArgument("x should be finite");

    if(Periodic) {
        //la función periódica debe contener
        //almenos un punto fundamental
        if(Count < 1)
            throw EImproperCall("empty function");

        //las abcisas deben quedar ordenadas
        //en orden creciente y no coincidir
        if(x<=X[i-1] || X[i+1]<=x)
            throw EImproperArgument("improper value x");

        //transplanta la abcisa al ciclo principal
        int ncycles = floor(double(i)/Count);
        i -= ncycles*Count; //transplanta i al ciclo principal
        x -= T*ncycles; //transplanta x al ciclo principal
    } else { //si es aperiódica
        //debe indicar un punto fundamental existente
        if(i<0 || Count<=i)
            throw EImproperArgument("index i out bounds");

        //las abcisas deben quedar ordenadas
        //en orden creciente y no coincidir
        if(i <= 0) { //si indica al primer punto
            //x debe ser menor que la abcisa del segundo
            if(x >= X[1])
                throw EImproperArgument("imrpoper value x");
        } else if(i < Count-1) { //si indica un punto intermedio
            //x debe estar entre las abcisas de los puntos contiguos
            if(x<=X[i-1] || X[i+1]<=x)
                throw EImproperArgument("improper value x");
        } else { //si indica al último punto
            //x debe ser mayor que la abcisa del penúltimo
            if(x <= X[i-1])
                throw EImproperArgument("improper value x");
        }
    }

    X[i] = x; //asigna la abcisa al punto indicado
}

double TFunction::getY(int i) const
{
    if(Periodic) {
        //la función periódica debe contener
        //almenos un punto fundamental
        if(Count < 1)
            throw EImproperCall("empty function");

        //transplanta i al ciclo principal
        int ncycles = floor(double(i)/Count);
        i -= ncycles*Count;
    } else { //si es aperiódica
        //debe indicar un punto fundamental existente
        if(i<0 || Count<=i)
            throw EImproperArgument("index i out bounds");
    }

    return Y[i]; //devuelve la ordenada del punto indicado
}

void TFunction::setY(int i, double y)
{
    if(Periodic) {
        //la función periódica debe contener
        //almenos un punto fundamental
        if(Count < 1)
            throw EImproperCall("empty function");

        //transplanta i al ciclo principal
        int ncycles = floor(double(i)/Count);
        i -= ncycles*Count;
    } else { //si es aperiódica
        //debe indicar un punto fundamental existente
        if(i<0 || Count<=i)
            throw EImproperArgument("index i out bounds");
    }

    Y[i] = y; //asigna la ordenada al punto indicado
}

TDoublePoint TFunction::getP(int i) const
{
    TDoublePoint p;
    if(Periodic) {
        int ncycles = floor(double(i)/Count);
        i -= ncycles*Count; //transplanta i al ciclo principal
        p.x = X[i] + T*ncycles;
        p.y = Y[i];
        return p;
    } else { //si es aperiódica
        //debe indicar un punto de muestreo existente
        if(i<0 || Count<=i)
            throw EImproperArgument("index i out bounds");

        p.x = X[i];
        p.y = Y[i];
        return p;
    }
}

void TFunction::setP(int i, TDoublePoint p)
{
    if(Periodic) {
        //X[i] debe estar entre X[i-1] y X[i+1]
        if(p.x<=X[i-1] || X[i+1]<=p.x)
            throw EImproperArgument(
                    "X[i] should be in (X[i-1], X[i+1])");

        //transplanta la abcisa al ciclo principal
        int ncycles = floor(double(i)/Count);
        i -= ncycles*Count; //transplanta i al ciclo principal
        p.x -= T*ncycles; //transplanta x al ciclo principal
    } else { //si es aperiódica
        //debe indicar un punto de muestreo existente
        if(i<0 || Count<=i)
            throw EImproperArgument("index i out bounds");

        //verifica que x tiene un valor permitido
        if(i <= 0) { //i==0
            //X[0] debe ser menor que X[1]
            if(p.x >= X[1])
                throw EImproperArgument(
                        "X(0) should be less X(1)");
        } else if(i < Count-1) { //1<=i<=Count-2
            //X[i] debe estar entre X[i-1] y X[i+1]
            if(p.x<=X[i-1] || X[i+1]<=p.x)
                throw EImproperArgument(
                        "X[i] should be in (X[i-1], X[i+1])");
        } else { //i==Count-1
            if(p.x <= X[i-1])
                throw EImproperArgument(
                        "X[Count-1] should be upper X[Count-2]"
                        );
        }
    }

    //asigna el punto
    X[i] = p.x;
    Y[i] = p.y;
}

//--------------------------------------------------------------------------
//PROPIEDADES EN FORMATO TEXTO:

AnsiString TFunction::getPointsText(void) const
{
    AnsiString S = "{";

    if(Count > 0) {
        S += AnsiString("(")+FloatToStr(X[0])+AnsiString(", ")+FloatToStr(Y[0])+AnsiString(")");
        for(int i=1; i<Count; i++)
            S += AnsiString(", (")+FloatToStr(X[i])+AnsiString(", ")+FloatToStr(Y[i])+AnsiString(")");
    }

    S += "}";

    return S;
}

void TFunction::setPointsText(const AnsiString &S)
{
    try {
        int i = 1;
        ReadPoints(this, S, i);
    } catch(...) {
        throw;
    }
}

AnsiString TFunction::getTableText(void) const
{
    AnsiString S1, S2;

    //si hay algún punto
    if(Count > 0) {
        //transforma el par indicado a formato texto
        S1 = FloatToStr(X.getFirst());
        while(S1.Length() < 20)
            S1 += " ";
        S1 += FloatToStr(Y.getFirst());

        //añade el par a la tabla
        S2 += S1;

        //por cada punto adicional de muestreo
        for(int i=1; i<Count; i++) {
            //transforma el par indicado a formato texto
            S1 = FloatToStr(X[i]);
            while(S1.Length() < 20)
                S1 += " ";
            S1 += FloatToStr(Y[i]);

            //añade el par a la tabla
            S2 += "\r\n";
            S2 += S1;
        }
    }

    return S2;
}
void TFunction::setTableText(const AnsiString &S)
{
    try {
        int i = 1;
        ReadTable(this, S, i);
    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//imprime los puntos de una función en una cadena
void __fastcall TFunction::PrintPoints(AnsiString &S, const TFunction *F)
{
    //el puntero F deberí apauntar a una función contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built function");

    S += F->getPointsText();
}
//lee una cadena en busca de los puntos de un afunción
void __fastcall TFunction::ReadPoints(TFunction *F, const AnsiString &S,int &i)
{
    //el puntero F deberí apauntar a una función contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built function");

    //NOTA: se procederá leyendo una lista de pares (x, y) y asignándolos
    //a una función tampón para verificar que las abcisas no coiniden.

    try {
        //escanea una lista de pares de la cadena
        TPointersList<TPair> Pairs(64, NULL, NULL, NULL, NULL, TPair::Read);
        TPointersList<TPair>::ReadList(&Pairs, S, i);

        //asigna los pares a una función tampón
        TFunction _F;
        for(int i=0; i<Pairs.getCount(); i++)
            _F.Add(Pairs[i].x, Pairs[i].y);

        //copia la función
        *F = _F;

    } catch(...) {
        throw;
    }
}

//imprime los puntos de una función en una cadena
void __fastcall TFunction::PrintTable(AnsiString &S, const TFunction *F)
{
    //el puntero F deberí apauntar a una función contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built function");

    S += F->getTableText();
}
//lee una cadena en busca de los puntos de un afunción
void __fastcall TFunction::ReadTable(TFunction *F, const AnsiString &S,
                                     int &i)
{
    //el puntero F debería apauntar a una función contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built function");

    //el índice i deberí indicar a una caracter de la cadena S
    if(i<0 || S.Length()<i)
        throw EImproperArgument("index i should indicate an character in the string S");

    try {
        //variables tampón
        TVector<double> _X, _Y;

        //intenta leer la tabla
        StrReadTable(_X, _Y, S, i);

        //Nótese que las abcisas no tienen por que estar ordenadas.

        //el vector de abcisas no debe tener valores repetidos
        for(int i=0; i<_X.getCount(); i++)
            for(int j=i+1; j<_X.getCount(); j++)
                if(_X[i] == _X[j])
                    //indica que la abcisa X[i] no debería ser igual a la abcisa X[j]
                    throw EImproperArgument(AnsiString("abcissa X[")+IntToStr(i)+AnsiString("] should not be equal to abcissa X[")+IntToStr(j)+AnsiString("]"));

        //asigna los nuevos valores
        F->X = _X;
        F->Y = _Y;
    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//MÉTODOS CONTRUCCIÓN COPIA Y DESTRUCCIÓN:

//constructor de funciones aperiódicas
TFunction::TFunction(int Capacity) :
    /*Periodic(false), T(0),*/
    Count(0), X(1), Y(1)
{
    //Debe especificar capacidad para uno o más puntos
    if(Capacity < 1)
        throw EImproperArgument("Capacity should be upper zero");

    T = 0;
    Periodic = false;

    X.setCapacity(Capacity);
    Y.setCapacity(Capacity);
}

//Constructor de funciones T-periódicas
TFunction::TFunction(double _T, int Capacity) :
    /*Periodic(true), T(T),*/
    Count(0), X(1), Y(1)
{
    //El periodo debe ser positivo
    if(Capacity<1 || _T<=0)
        throw EImproperArgument("Capacity should be upper zero");

    T = _T;
    Periodic = true;

    X.setCapacity(Capacity);
    Y.setCapacity(Capacity);
}

//copia una función
void TFunction::Copy(const TFunction *F)
{
    //el puntro F debería pauntar a una función contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built function");

    //copia las propiedades
    Periodic = F->Periodic;
    T = F->T;
    Count = F->Count;
    X = F->X;
    Y = F->Y;
    Label = F->Label;
}
void TFunction::Copy(const TFunction& F)
{
    //copia las propiedades
    Periodic = F.Periodic;
    T = F.T;
    Count = F.Count;
    X = F.X;
    Y = F.Y;
    Label = F.Label;
}

//clona una función
void TFunction::Clone(const TFunction *F)
{
    //el puntro F debería pauntar a una función contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built function");

    //copia las propiedades
    Periodic = F->Periodic;
    T = F->T;
    Count = F->Count;
    X.Clone(F->X);
    Y.Clone(F->Y);
    Label = F->Label;
}
void TFunction::Clone(const TFunction &F)
{
    //copia las propiedades
    Periodic = F.Periodic;
    T = F.T;
    Count = F.Count;
    X.Clone(F.X);
    Y.Clone(F.Y);
    Label = F.Label;
}

//construye un clon de una función
TFunction::TFunction(const TFunction *F)
{
    //el puntro F debería pauntar a una función contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built function");

    //copia las propiedades
    Clone(F);
}

//--------------------------------------------------------------------------

//borra todos los puntos de muestreo
void TFunction::Clear(void)
{
    X.Clear();
    Y.Clear();
    Count = 0;
}

//añade un nuevo punto devolviendo la posición de inserción
int TFunction::Add(double x, double y)
{
    int i; //posición de inserción
    int ncycles=0; //índice al ciclo correspondiente a x

    //el dominio de definición de una función periódica es todo R
    if(Periodic)
        //Transplanta x al ciclo principal [0, T)
        if(x<0 || T<=x) {
            //transplanta x al ciclo principal
            ncycles = floor(x/T);
            x -= T*ncycles;
        }

    if(Count) { //si hay algún punto
        //la coordenada x no deberá coincidir;
        //busca la primera coincidencia de x en X
        i = X.Search(x); //si no hay ninguno retorna i=Count

        if(i < Count) //si hay alguna coincidencia de x en X
            throw EImproperArgument("coincident stored x");

        //inserta el punto t.q. X queda ordenado
        i = X.InsertInc(x);
        Y.Insert(i, y);

    } else { //si la función está vacía
        //lo incluye directamente
        X.Add(x);
        Y.Add(y);

        i = Count; //captura la posición de inserción
    }

    Count++; //contabiliza el nuevo punto

    return i + Count*ncycles; //devuelve la posición de inserción
}

//empuja el último punto
void TFunction::PushLast(double x, double y)
{
    //El dominio de definición de una función periódica es todo R
    if(Periodic)
        //Transplanta x al ciclo principal [0, T)
        if(x<0 || T<=x)
            x -= T*floor(x/T);

    if(Count) {
        //debe estar más allá del último punto
        if(x <= X.getLast())
            throw EImproperArgument(
                    "point (x, y) should be posterior");

        X.PushLast(x);
        Y.PushLast(y);
    } else //si la función está vacía no se puede empujar
        throw EImproperCall("empty function");
}

//libera los priemros puntos
void TFunction::DelFirst(int n)
{
    //debe liberar puntos reservados
    if(n<1 || Count<n)
        throw EImproperArgument(
                "number items n should be in [1, Count]");

    X.DelFirst(n);
    Y.DelFirst(n);
    Count -= n;
}

//elimina el punto i-ésimo p.a. [0, Count-1] (si aperiódica)
//ó p.a. (-infinity, infinity) (si periódica
void TFunction::Delete(int i)
{
    int ncycles;
    if(Periodic) { //si es periódica
        //transplanta i al ciclo principal [0, Count-1]
        ncycles = floor(double(i)/Count);
        i -= Count*ncycles;
    } else
        //si es aperiódica el dominio se restringe a [0, Count-1]
        if(i<0 || Count<=i)
            throw EImproperArgument("index i out bounds");

    X.Delete(i);
    Y.Delete(i);
    Count--;
}

//--------------------------------------------------------------------------

//Genera n puntos con distribuxión uniforme en el intervalo (ymin, ymax).
//Si la función es periódica, equiespaciados en el itnervalo [0, T);
//y si es aperiódica, en los puntos X = {0, 1, 2, ..., n-1}.
void TFunction::Random(int n, double ymin, double ymax)
{
    if(Periodic) {
        double deltax = T/n;
        for(int i=0; i<n; i++)
            Add(i*deltax, RandomUniform(ymin, ymax));
    } else
        for(int i=0; i<n; i++)
            Add(double(i), RandomUniform(ymin, ymax));
}

//Genera n puntos con imagen igual a cero.
//Si la función es periódica, equiespaciados en el itnervalo [0, T);
//y si es aperiódica, en los puntos X = {0, 1, 2, ..., n-1}
void TFunction::Zeros(int n)
{
    if(Periodic) {
        double deltax = T/n;
        for(int i=0; i<n; i++)
            Add(i*deltax, 0);
    } else
        for(int i=0; i<n; i++)
            Add(double(i), 0);
}

//--------------------------------------------------------------------------

/*TFunction& TFunction::operator=(TFunction &F)
{
        __Periodic = F.Periodic;
        __T = F.T;
        __Count = F.Count;
    __X = F.__X;
    __Y = F.__Y;

        //no vacia los vectores porque podrían asignarse a sí mismos

        return *this;
}
  */
TFunction& TFunction::operator=(const TFunction &F)
{
    Periodic = F.Periodic;
    T = F.T;
    Count = F.Count;
    X = F.X;
    Y = F.Y;

    //no vacia los vectores porque podrían asignarse a sí mismos

    return *this;
}

//compara con una función
bool TFunction::operator==(const TFunction &F) const
{
    if(Periodic!=F.Periodic || T!=F.T || Count!=F.Count)
        return false;

    for(int i=0; i<Count; i++)
        if(X[i]!=F.X[i] || Y[i]!=F.Y[i])
            return false;

    return true;
}

//compara con una función
bool TFunction::operator!=(const TFunction &F) const
{
    if(*this == F)
        return false;
    else
        return true;
}

//--------------------------------------------------------------------------

//ordena las muestras de la función de menor a mayor ordenada
//aplicando el algoritmo quick sort
void TFunction::SortInc(void)
{
    if(Y.getCount() > 2)
        Y.SortInc(0, Y.getCount()-1);
}

//--------------------------------------------------------------------------
//EVALUACIÓN DE ASPECTOS DE LA FUNCIÓN:

//devuelve verdadero si x p.a. dominio de x
bool TFunction::BelongToDomain(double x) const
{
    //si la función está vacía el dominio está vacío
    if(Count < 1)
        return false;

    //si no es periódica el dominio es [X.getFirst(), X.getLast()]
    if(!Periodic && (x<X.getFirst() || X.getLast()<x))
        return false;

    return true;
}

//Evalúa la función en x
double TFunction::Image(double x) const
{
    //Para poder evaluarla, la función debe contener almenos un punto
    if(Count<1)
        throw EImproperArgument("empty function");

    if(Periodic)
        //transplanta x al ciclo principal [0, T)
        x -= T*floor(x/T);
    else
        //la evaluación está restringida al dominio de definción
        if(!BelongToDomain(x))
            throw EImproperArgument("x dont belong to Domain");

    //busca el primer X[i]>=x
    int i = X.FirstUpperOrEqualTo(x);
    //si es periódica, i indicará un punto en [0, Count]
    //si no, i índicará un punto en [0, Count-1]

    //determina los vértices del segmento que contiene el punto
    double x1, y1; //coordenadas del punto origen del segmento
    double x2, y2; //coordenadas del punto final del segmento
    if(i<1) {
        x1 = X.getLast() - T; y1 = Y.getLast(); //el último del ciclo previo
        x2 = X.getFirst(); y2 = Y.getFirst(); //el primero del ciclo principal
    } else if(Count<=i) {
        x1 = X.getLast(); y1 = Y.getLast(); //el último del ciclo principal
        x2 = X.getFirst() + T; y2 = Y.getFirst(); //el primero del ciclo post.
    } else {
        x1 = X[i-1]; y1 = Y[i-1]; //p1 es el punto previo
        x2 = X[i]; y2 = Y[i]; //p2 es el punto indicado
    }

    //interpola x entre p1 y p2
    return y1 + (x - x1)*(y2 - y1)/(x2 - x1);
}

//busca el punto (virtual) que contiene la abcisa x
bool TFunction::Find(double x, int &i) const
{
    int ncycles=0; //índice al ciclo al que pertenece x
    //El dominio de definición de una función periódica es todo R
    if(Periodic) //si es periódica...
        if(x<0 || T<=x) { //...y no está en el ciclo principal
            //Transplanta x al ciclo principal [0, T)
            ncycles = floor(x/T);
            x -= T*ncycles;
        }

    //busca un punto de abcisa x
    i = X.Search(x);

    if(i < Count) { //si lo encuentra
        //desplaza i al ciclo de x
        i += Count*ncycles;
        return true; //indica que lo ha encontrado
    } else
        return false; //indica que no lo ha encontrado
}

//determina el tipo de monotonía de la función
//      -1: monótona decreciente;
//      0: constante;
//      1: monótona creciente;
//      2: no monótona.
//si la función está vacía lanza una excepción EImproperCall
int TFunction::Monotony(void) const
{
    //la función no debe estar vacía
    if(Count == 0)
        throw EImproperArgument("funtion should not be empty");

    //estado de análisis
    //      -1: monótona decreciente;
    //      0: constante;
    //      1: monótona creciente;
    //      2: no monótona.
    int monotony=0;

    //paracada ordenada de la función
    for(int i=1; i<Count; i++) {
        switch(monotony) {
        case -1: //monótona decreciente
            if(Y[i] > Y[i]-1)
                return 2; //indica que no es monótoma
            break;
        case 1: //monótona creciente
            if(Y[i] < Y[i]-1)
                return 2; //indica que no es monótona
            break;
        case 0: //constante
            if(Y[i] < Y[i-1])
                monotony = -1;
            else if(Y[i] > Y[i-1])
                monotony = 1;
            break;
        }
    }

    return monotony;
}

//Determina el intervalo [i1, i2] conteniendo los índices a los vértices
//de los segmentos completamente contenidos en [x1, x2].
//Si la función no tiene ningún segmento completamente contenido en [x1, x2]
//este método devuelve false.
bool TFunction::InnerSegments(double x1, double x2, int& i1, int& i2) const
{
    int n1; //nº de ciclo donde se encuentra x1 (solo si periodica)
    int n2; //nº de ciclo donde se encuentra x2 (solo si periodica)

    //si es periódica
    if(Periodic) {
        //lleva x1 al ciclo principàl
        n1 = floor(x1/T);
        x1 -= n1*T;
        //lleva x2 al ciclo principal
        n2 = floor(x2/T);
        x2 -= n2*T;
    }

    //j1 al primer punto p.a. [x1, x2]
    int j1 = X.FirstUpperOrEqualTo(x1); //si X[j]<x1 p.t. j, dev. Count
    //si el primer punto está antes que x1
    if(!Periodic && j1>=X.getCount())
        return false; //no hay puntos internos

    //j2 al último punto p.a. [x1, x2]
    int j2 = X.LastLowerOrEqualTo(x2); //si x2<X[i] p.t. i, devolverá -1
    //si el último punto está después que x2
    if(!Periodic && j2<0)
        return false; //no hay puntos internos

    //si es periódica deben restaurarse los ciclos
    if(Periodic) {
        i1 = j1 + n1*Count; //restaura los ciclos y apunta al pto previo
        i2 = j2 + n2*Count + 1; //rest. los ciclos y apta al pto post.
    } else { //si es aperiódica
        i1 = j1;
        i2 = j2;
    }

    return true;
}

//Determina el intervalo [i1, i2] conteniendo los índices a los vértices
//de los segmentos extremos que tienen almenos una parte en (x1, x2).
//Si la función no tiene segmentos con almenos una parte en (x1, x2),
//este método devuelve false
bool TFunction::IntrusiveSegments(double x1, double x2, int& i1, int& i2) const
{
    int n1; //nº de ciclo donde se encuentra x1 (solo si periodica)
    int n2; //nº de ciclo donde se encuentra x2 (solo si periodica)

    //si es periódica
    if(Periodic) {
        //lleva x1 al ciclo principàl
        n1 = floor(x1/T);
        x1 -= n1*T;
        //lleva x2 al ciclo principal
        n2 = floor(x2/T);
        x2 -= n2*T;
    }

    //j1 al primer punto p.a. (x1, x2)
    int j1 = X.FirstUpperTo(x1); //si X[j]<x1 p.t. j, devolverá Count
    //si el primer punto está en x1 o antes
    if(!Periodic && j1>=Count)
        return false; //no hay puntos internos

    //j2 al último punto p.a. (x1, x2)
    int j2 = X.LastLowerTo(x2); //si x2<X[i] p.t. i, devolverá -1
    //si el último punto está en x2 o después
    if(!Periodic && j2<0)
        return false; //no hay puntos internos

    //si es periódica deben restaurarse los ciclos
    //y apuntarse a los puntos previo y posterior a j1 y j2
    if(Periodic) {
        //se restauran los ciclos
        i1 = j1 + n1*Count - 1; //y se apunta al punto previo
        i2 = j2 + n2*Count + 1; //t se apunta al punto posterior
    } else { //si es aperiódica puediera no haber punto previo/post. a j1/j2
        i1 = Max(0, j1 - 1); //apunta al pto previo en caso de haberlo
        i2 = Min(X.getCount() - 1, j2 + 1); //apunta al pto post. si lo hay
    }

    return true;
}

//determina el intervalo [i1, i2] conteniendo los índices a los vértices
//de los segmentos que tienen almenos un punto en el intervalo [x1, x2]
//si la función no tiene segmentos con almenos un punto en [x1, x2]
//esta función devuelve false
bool TFunction::AdjacentSegments(double x1, double x2, int& i1, int& i2) const
{
    int n1; //nº de ciclo donde se encuentra x1 (solo si periodica)
    int n2; //nº de ciclo donde se encuentra x2 (solo si periodica)

    //si es periódica
    if(Periodic) {
        //lleva x1 al ciclo principàl
        n1 = floor(x1/T);
        x1 -= n1*T;
        //lleva x2 al ciclo principal
        n2 = floor(x2/T);
        x2 -= n2*T;
    }

    //j1 al primer punto p.a. [x1, x2]
    int j1 = X.FirstUpperOrEqualTo(x1); //si X[j]<x1 p.t. j, dev. Count
    //si el primer punto está antes que x1
    if(!Periodic && j1>=Count)
        return false; //no hay puntos internos

    //j2 al último punto p.a. [x1, x2]
    int j2 = X.LastLowerOrEqualTo(x2); //si x2<X[i] p.t. i, devolverá -1
    //si el último punto está después que x2
    if(!Periodic && j2<0)
        return false; //no hay puntos internos

    //si es periódica siempre habrá punto previo y posterior
    if(Periodic) {
        //se restauran los ciclos
        i1 = j1 + n1*Count - 1; //y se apunta al punto previo
        i2 = j2 + n2*Count + 1; //y se aputna al punto posterior
    } else { //si es aperiódica puediera no haber punto previo/post. a j1/j2
        i1 = Max(0, j1 - 1); //apunta al pto previo en caso de haberlo
        i2 = Min(X.getCount()-1, j2+1); //apunta al pto post. si lo hay
    }

    return true;
}

//determina la secuencia de índices a los puntos de inflexión
//de la función, pertenecientes al intervalo [x1,x2]
void TFunction::InflexPoints (double x1, double x2, TVector<int> &V) const
{
    //Busca los máximos y mínimos locales así como los puntos donde
    //la función deje de ser constante respecto de los puntos previos
    //En el caso de que la función sea constante no tendrá máximos
    //ni mínimos y V se devolverá vacío

    int i1; //índice al primer punto menor (en sentido estricto) que x1
    int i2; //índice al primer punto mayor (en sentido estricto) que x2

    AdjacentSegments(x1, x2, i1, i2);

    V.Clear(); //vacía V
    if(i2-i1>=2) { //si hay almenos dos segmentos [x1, x2]
        double anterior; //deltay del segmento (x[i-1], x[i])
        double siguiente; //deltay del segmento (x[i], x[i+1])

        anterior = Y[i1 + 1] - Y[i1];
        for(int i=i1+1; i<i2; i++) {
            siguiente = Y[i+1] - Y[i];
            //si es punto de inflexión lo incorpora a V
            if((anterior==0 && siguiente!=0) ||
                    (anterior!=0 && siguiente==0) ||
                    (anterior>0 && siguiente<0) ||
                    (anterior<0 && siguiente>0))
                V.Add(i);
            anterior = siguiente;
        }
    }
}

//determina la secuencia de puntos de inflexión
//de la función, pertenecientes al intervalo [x1, x2]
void TFunction::InflexPoints(double x1, double x2, TVector<double> &V) const
{
    //Busca los máximos y mínimos locales así como los puntos donde
    //la función deje ser constante respecto de los puntos previos.
    //En el caso de que la función sea constante no tendrá máximos
    //ni mínimos y V se devolverá vacío.

    int i1; //índice al primer punto menor (en sentido estricto) que x1
    int i2; //índice al primer punto mayor (en sentido estricto) que x2

    AdjacentSegments(x1, x2, i1, i2);

    V.Clear(); //vacía V
    if(i2-i1>=2) { //si hay almenos dos segmentos [x1, x2]
        double anterior; //deltay del segmento (x[i-1], x[i])
        double siguiente; //deltay del segmento (x[i], x[i+1])

        anterior = Y[i1 + 1] - Y[i1];
        for(int i=i1+1; i<i2; i++) {
            siguiente = Y[i+1] - Y[i];
            //si es punto de inflexión lo incorpora a V
            if((anterior==0 && siguiente!=0) ||
                    (anterior!=0 && siguiente==0) ||
                    (anterior>0 && siguiente<0) ||
                    (anterior<0 && siguiente>0))
                V.Add(X[i]);
            anterior = siguiente;
        }
    }
}

//--------------------------------------------------------------------------
//OPERACIONES CON LA FUNCIÓN:

//asigna la función inversa de F
void TFunction::Inverse(const TFunction &F)
{
    //determina el tipo de monotonía
    int monotony = F.Monotony();

    //si la función F contiene más de una muestra no debe ser constante
    if(F.Count > 1 && monotony==0)
        throw EImproperArgument("if function F contain most one sample, should not be constant");

    //la función F no debe ser no monótona
    if(monotony<-1 || 1<monotony)
        throw EImproperArgument("function F should not be not monotone");

    double x; //variable auxiliar

    //vacia la función en congruencia con la situación inciial
    Clear();

    //si hay alguna muestra
    if(F.Count > 0)
        //añade la primera muestra transpuesta
        Add(F.getYFirst(), F.getXFirst());

    //por cada muestra sucesiva
    for(int i=1; i<F.Count; i++) {
        //lee la abcisa
        x = F.Y[i];

        //si la abcisa coincide conla previa
        if(x == getXLast())
            //actualiza laordenada
            setYLast(F.X[i]);
        else //si es una nueva abcisa
            //añadelacoordenada transpuesta
            Add(x, F.X[i]);
    }
}

//asigna la función normalizada de F
void TFunction::Normalized(const TFunction &F)
{
    //vacia la función en congruencia con el estado inicial
    Clear();

    //si la función contiene alguna muestra
    if(F.Count > 0) {
        //determina el valor máximo de F
        double ymax = F.getYMax();

        //por cada punto de muestreo
        for(int i=0; i<F.Count; i++)
            //añade el punto d emuestreo normalizado
            Add(F.X[i], F.Y[i]/ymax);
    }
}

//actualiza las ordenadas
//      Y[i] = Min(Y[i], F.Y[i])
//las abcisas debén coincidir
void TFunction::OrdinatesMin(TFunction &F)
{
    //el número de puntos F.Count debería ser igual al ´número de puntos Count
    if(F.Count != Count)
        throw EImproperArgument("point number F.Count should be equal to point number Count");

    //por cada punto de la función
    for(int i=0; i<Count; i++)
        //la abcisa F.X[i] debería ser igual a la abcisa X[i]
        if(F.X[i] != X[i])
            throw EImproperArgument("abcise F.X[i] should be equal to abcise X[i]");

    //por cada punto de la función
    for(int i=0; i<Count; i++)
        //asigna la ordenada mínima
        Y[i] = Min(Y[i], F.Y[i]);
}
//actualiza las ordenadas
//      Y[i] = Y[i]*((n-1)/n) + F.Y[i]/n
//las abcisas debén coincidir
void TFunction::OrdinatesAve(TFunction &F, int n)
{
    //el número de puntos F.Count debería ser igual al ´número de puntos Count
    if(F.Count != Count)
        throw EImproperArgument("point number F.Count should be equal to point number Count");

    //por cada punto de la función
    for(int i=0; i<Count; i++)
        //la abcisa F.X[i] debería ser igual a la abcisa X[i]
        if(F.X[i] != X[i])
            throw EImproperArgument("abcise F.X[i] should be equal to abcise X[i]");

    //elnúmero de muestra n debe ser al menos dos
    if(n < 2)
        throw EImproperArgument("samplenumber n should be almost two");

    //por cada punto de la función
    for(int i=0; i<Count; i++)
        //asigna la ordenada mínima
        Y[i] = Y[i]*((n-1)/n) + F.Y[i]/n;
}
//actualiza las ordenadas
//      Y[i] = Max(Y[i], F.Y[i])
//las abcisas debén coincidir
void TFunction::OrdinatesMax(TFunction &F)
{
    //el número de puntos F.Count debería ser igual al ´número de puntos Count
    if(F.Count != Count)
        throw EImproperArgument("point number F.Count should be equal to point number Count");

    //por cada punto de la función
    for(int i=0; i<Count; i++)
        //la abcisa F.X[i] debería ser igual a la abcisa X[i]
        if(F.X[i] != X[i])
            throw EImproperArgument("abcise F.X[i] should be equal to abcise X[i]");

    //por cada punto de la función
    for(int i=0; i<Count; i++)
        //asigna la ordenada máxima
        Y[i] = Max(Y[i], F.Y[i]);
}


//escribelafunciónenelarchivofilename
void TFunction::WriteToFile(char *filename)
{
    TTextFile TF;

    try {
        //abre el archivo para adición
        TF.Open(filename, (char*)"w");

        //imprime la cadena en el archivo
        AnsiString S = getPointsText();
        TF.Print(S);

        //cierra el archivo
        TF.Close();
    }
    catch(Exception &E) { //si falla
        //asegura el cierre del archivo para evitar la pérdida de datos
        if(TF.getIsOpen())
            TF.Close();

        //relanza la excepción que deberá ser
        //capturada en la secuencia de guardado
        throw Exception(E);
    }
}

//---------------------------------------------------------------------------

} //namespace Operators

//--------------------------------------------------------------------------

