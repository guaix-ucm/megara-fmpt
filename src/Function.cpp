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
///@file Function.cpp
///@brief contonuous function of continuous time defined in sample points
///@author Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Function.h"
#include "PointersList.h"
#include "TextFile.h"

#include <algorithm> //std::min, std::max

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
void TFunction::setPeriodic(bool Periodic)
{
    //la activación de la periodicidad requiere que todos los puntos
    //de muestreo se encuentren en el ciclo principal [0, T)
    if(!p_Periodic && Periodic && p_Count>0 && (p_X.getFirst()<0 || p_T<=p_X.getLast()))
        throw EImproperCall("sample point dont belong to [0, T)");

    //si no hay puntos de muestreo siempre se podrá activar la periodicidad

    //asigna el nuevo valor
    p_Periodic = Periodic;
}

//Establece o cambia el periodo T. Si la función es periódica,
//todos los puntos de muestreo deberán estar en el ciclo principal [0, T).
void TFunction::setT(double T)
{
    //si la función es periódica, todos los puntos de muestreo deberán estar
    //en el ciclo principal [0, T)
    if(p_Periodic && p_Count && (p_X.getFirst()<0 || T<=p_X.getLast()))
        throw EImproperArgument("sample point dont belong to [0, T)");

    p_T = T;
}

//---------------------------------------------------------------------------
//ACCESO A LOS PUNTOS DE MUESTREO:

//asigna los vectores de abcisas y ordenadas
void TFunction::Set(const TVector<double>& X, const TVector<double>& Y)
{
    //comprueba las precondiciones
    if(X.getCount() != Y.getCount())
        throw EImproperArgument("number of abcises should be equalto thenumber of ordinates");
    if(!X.getIncreasing())
        throw EImproperArgument("the abcises should besorted increasing");
    for(int i=1; i<X.getCount(); i++)
        if(X[i] == X[i-1])
            throw EImproperArgument("should not be two abcises equals");

    //asigna los vectores y actualiza el número de puntos
    p_X = X;
    p_Y = Y;
    p_Count = X.getCount();
}

double TFunction::getX(int i) const
{
    if(p_Periodic) {
        //la función periódica debe contener
        //almenos un punto fundamental
        if(p_Count < 1)
            throw EImproperCall("empty function");

        //transplanta i al ciclo principal
        int ncycles = (int)floor(double(i)/p_Count);
        i -= ncycles*p_Count;

        //devuelve la abcisa del punto (virtual) indicado
        return p_X[i] + p_T*ncycles;
    } else { //si es aperiódica
        //debe indicar un punto fundamental existente
        if(i<0 || p_Count<=i)
            throw EImproperArgument("index i out bounds");

        //devuelve la abcisa del punto indicado
        return p_X[i];
    }
}

void TFunction::setX(int i, double x)
{
    //para tener sentido descriptivo, la abcisa debe ser finita
    if(x<=-DBL_MAX || DBL_MAX<=x)
        throw EImproperArgument("x should be finite");

    //si la función es periódica
    if(p_Periodic) {
        //la función periódica debe contener
        //almenos un punto fundamental
        if(p_Count < 1)
            throw EImproperCall("empty function");

        //las abcisas deben quedar ordenadas
        //en orden creciente y no coincidir
        if(x<=p_X[i-1] || p_X[i+1]<=x)
            throw EImproperArgument("improper value x");

        //transplanta la abcisa al ciclo principal
        int ncycles = (int)floor(double(i)/p_Count);
        i -= ncycles*p_Count; //transplanta i al ciclo principal
        x -= p_T*ncycles; //transplanta x al ciclo principal
    }
    //si la función es aperiódica
    else {
        //debe indicar un punto fundamental existente
        if(i<0 || p_Count<=i)
            throw EImproperArgument("index i out bounds");

        //las abcisas deben quedar ordenadas
        //en orden creciente y no coincidir
        if(i <= 0) { //si indica al primer punto
            //x debe ser menor que la abcisa del segundo
            if(x >= p_X[1])
                throw EImproperArgument("imrpoper value x");
        } else if(i < p_Count-1) { //si indica un punto intermedio
            //x debe estar entre las abcisas de los puntos contiguos
            if(x<=p_X[i-1] || p_X[i+1]<=x)
                throw EImproperArgument("improper value x");
        } else { //si indica al último punto
            //x debe ser mayor que la abcisa del penúltimo
            if(x <= p_X[i-1])
                throw EImproperArgument("improper value x");
        }
    }

    p_X[i] = x; //asigna la abcisa al punto indicado
}

double TFunction::getY(int i) const
{
    //si la función es periódica
    if(p_Periodic) {
        //la función periódica debe contener
        //almenos un punto fundamental
        if(p_Count < 1)
            throw EImproperCall("empty function");

        //transplanta i al ciclo principal
        int ncycles = (int)floor(double(i)/p_Count);
        i -= ncycles*p_Count;
    }
    //si la función es periódica
    else {
        //debe indicar un punto fundamental existente
        if(i<0 || p_Count<=i)
            throw EImproperArgument("index i out bounds");
    }

    return p_Y[i]; //devuelve la ordenada del punto indicado
}

void TFunction::setY(int i, double y)
{
    //si la función es periódica
    if(p_Periodic) {
        //la función periódica debe contener
        //almenos un punto fundamental
        if(p_Count < 1)
            throw EImproperCall("empty function");

        //transplanta i al ciclo principal
        int ncycles = (int)floor(double(i)/p_Count);
        i -= ncycles*p_Count;
    }
    //si la función es periódica
    else {
        //debe indicar un punto fundamental existente
        if(i<0 || p_Count<=i)
            throw EImproperArgument("index i out bounds");
    }

    p_Y[i] = y; //asigna la ordenada al punto indicado
}

TDoublePoint TFunction::getP(int i) const
{
    TDoublePoint p;
    //si lafunción es periódica
    if(p_Periodic) {
        //determina elnúmero de ciclos
        int ncycles = (int)floor(double(i)/p_Count);
        //transplanta i alciclo principal
        i -= ncycles*p_Count;
        //obtiene los valores en el ciclo correspondiente
        p.x = p_X[i] + p_T*ncycles;
        p.y = p_Y[i];
        //devuelvelos valores
        return p;

    }
    //si la función es periódica
    else {
        //debe indicar un punto fundamental existente
        if(i<0 || p_Count<=i)
            throw EImproperArgument("index i out bounds");

        p.x = p_X[i];
        p.y = p_Y[i];
        return p;
    }
}

void TFunction::setP(int i, TDoublePoint p)
{
    //si la función es periódica
    if(p_Periodic) {
        //X[i] debe estar entre X[i-1] y X[i+1]
        if(p.x<=p_X[i-1] || p_X[i+1]<=p.x)
            throw EImproperArgument(
                    "X[i] should be in (X[i-1], X[i+1])");

        //transplanta la abcisa al ciclo principal
        int ncycles = (int)floor(double(i)/p_Count);
        i -= ncycles*p_Count; //transplanta i al ciclo principal
        p.x -= p_T*ncycles; //transplanta x al ciclo principal
    }
    //si la función es periódica
    else {
        //debe indicar un punto de muestreo existente
        if(i<0 || p_Count<=i)
            throw EImproperArgument("index i out bounds");

        //verifica que x tiene un valor permitido
        if(i <= 0) { //i==0
            //X[0] debe ser menor que X[1]
            if(p.x >= p_X[1])
                throw EImproperArgument(
                        "X(0) should be less X(1)");
        } else if(i < p_Count-1) { //1<=i<=Count-2
            //X[i] debe estar entre X[i-1] y X[i+1]
            if(p.x<=p_X[i-1] || p_X[i+1]<=p.x)
                throw EImproperArgument(
                        "X[i] should be in (X[i-1], X[i+1])");
        } else { //i==Count-1
            if(p.x <= p_X[i-1])
                throw EImproperArgument(
                        "X[Count-1] should be upper X[Count-2]"
                        );
        }
    }

    //asigna el punto
    p_X[i] = p.x;
    p_Y[i] = p.y;
}

//--------------------------------------------------------------------------
//PROPIEDADES EN FORMATO TEXTO:

AnsiString TFunction::getPointsText(void) const
{
    AnsiString S = "{";

    if(p_Count > 0) {
        S += AnsiString("(")+FloatToStr(p_X[0])+AnsiString(", ")+FloatToStr(p_Y[0])+AnsiString(")");
        for(int i=1; i<p_Count; i++)
            S += AnsiString(", (")+FloatToStr(p_X[i])+AnsiString(", ")+FloatToStr(p_Y[i])+AnsiString(")");
    }

    S += "}";

    return S;
}

void TFunction::setPointsText(const AnsiString &S)
{
    int i = 1; //initialize the index to the first position of the string

    try {
        //read the point in a tampon variable
        TFunction F(this);
        ReadPoints(&F, S, i);

        //look for unexpected text
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text: "+StrFirstChars(S.SubString(i, S.Length() - i + 1)).str);

        //clone the tampon variable
        Clone(F);

    } catch(Exception& E) {
        E.Message.Insert(1, "setting point in text format: ");
        throw;
    }
}

AnsiString TFunction::getTableText(void) const
{
    AnsiString S1, S2;

    //si hay algún punto
    if(p_Count > 0) {
        //transforma el par indicado a formato texto
        S1 = FloatToStr(p_X.getFirst());
        while(S1.Length() < 20)
            S1 += " ";
        S1 += FloatToStr(p_Y.getFirst());

        //añade el par a la tabla
        S2 += S1;

        //por cada punto adicional de muestreo
        for(int i=1; i<p_Count; i++) {
            //transforma el par indicado a formato texto
            S1 = FloatToStr(p_X[i]);
            while(S1.Length() < 20)
                S1 += " ";
            S1 += FloatToStr(p_Y[i]);

            //añade el par a la tabla
            S2 += "\r\n";
            S2 += S1;
        }
    }

    return S2;
}
void TFunction::setTableText(const AnsiString &S)
{
    int i = 1; //initialize the index to the first char of the string
    try {
        //read the table in a tampon variable
        TFunction F(this);
        ReadTable(&F, S, i);

        //look for unexpected text
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text: "+StrFirstChars(S.SubString(i, S.Length() - i + 1)).str);

        //set the tanpon variable
        Copy(&F);
    }
    catch(Exception& E) {
        unsigned int row, col;
        strPositionToCoordinates(row, col, S.str, i-1);
        E.Message.Insert(1, "setting table to function in row "+inttostr(row)+" and column "+inttostr(col)+": ");
        throw;
    }
}

//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TFunction::getAllText(void) const
{
    string str = "R/W:";
    str += "\r\n    Label = \"" + Label.str + "\"";
    str += "\r\n    Periodic = " + BoolToStr(p_Periodic).str;
    str += "\r\n    T = " + floattostr(p_T);
    str += "\r\n    Count = " + inttostr(getCount());
    str += "\r\n    XAddress: " + IntToHex(intptr_t(&p_X)).str;
    str += "\r\n    YAddress: " + IntToHex(intptr_t(&p_Y)).str;

    str += "\r\n    XFirst = " + floattostr(getXFirst());
    str += "\r\n    XLast = " + floattostr(getXLast());
    str += "\r\n    YFirst = " + floattostr(getYFirst());
    str += "\r\n    YLast = " + floattostr(getYLast());

    str += "\r\nR:";
    str += "\r\n    YMin = " + floattostr(getYMin());
    str += "\r\n    YMax = " + floattostr(getYMax());
    str += "\r\n    YAve = " + floattostr(getYAve());

    return AnsiString(str);
}

//---------------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//imprime los puntos de una función en una cadena
void  TFunction::PrintPoints(AnsiString &S, const TFunction *F)
{
    //comprueba las precondiciones
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built function");

    S += F->getPointsText();
}
//lee una cadena en busca de los puntos de un afunción
void  TFunction::ReadPoints(TFunction *F, const AnsiString &S,int &i)
{
    //comprueba las precondiciones
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built function");

    //NOTA: se procederá leyendo una lista de pares (x, y) y asignándolos
    //a una función tampón para verificar que las abcisas no coiniden.

    try {
        //escanea una lista de pares de la cadena
        TPointersList<TPair> Pairs(64, NULL, NULL, NULL, NULL, TPair::Read);
        TPointersList<TPair>::ReadList(&Pairs, S, i);

        //asigna los pares a una función tampón
        TFunction t_F;
        for(int i=0; i<Pairs.getCount(); i++)
            t_F.Add(Pairs[i].x, Pairs[i].y);

        //copia la función
        *F = t_F;

    } catch(...) {
        throw;
    }
}

//imprime los puntos de una función en una cadena
void  TFunction::PrintTable(AnsiString &S, const TFunction *F)
{
    //el puntero F deberí apauntar a una función contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built function");

    S += F->getTableText();
}
//lee una cadena en busca de los puntos de una función
void  TFunction::ReadTable(TFunction *F, const AnsiString &S,
                           int &i)
{
    //comprueba las precondiciones
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built function");
    if(i<0 || S.Length()<i)
        throw EImproperArgument("index i should indicate an character in the string S");

    try {
        //lee la tabla en variables tampón
        TVector<double> X, Y;
        StrReadTable(X, Y, S, i);
        //Nótese que las abcisas no tienen por que estar ordenadas.

        //comprueba las precondiciones
        for(int i=0; i<X.getCount(); i++)
            for(int j=i+1; j<X.getCount(); j++)
                if(X[i] == X[j])
                    throw EImproperArgument(AnsiString("abcissa X[")+IntToStr(i)+AnsiString("] should not be equal to abcissa X[")+IntToStr(j)+AnsiString("]"));

        //asigna los nuevos valores
        F->Set(X, Y);

    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//MÉTODOS CONTRUCCIÓN COPIA Y DESTRUCCIÓN:

//constructor de funciones aperiódicas
TFunction::TFunction(int Capacity) :
    p_Periodic(false), p_T(0),
    p_Count(0), p_X(1), p_Y(1)
{
    //comprueba las precondiciones
    if(Capacity < 1)
        throw EImproperArgument("Capacity should be upper zero");

    p_X.setCapacity(Capacity);
    p_Y.setCapacity(Capacity);
}

//Constructor de funciones T-periódicas
TFunction::TFunction(double T, int Capacity) :
    p_Periodic(true),
    p_Count(0), p_X(1), p_Y(1)
{
    //El periodo debe ser positivo
    if(Capacity < 1 || T <= 0)
        throw EImproperArgument("Capacity should be upper zero");

    p_T = T;

    p_X.setCapacity(Capacity);
    p_Y.setCapacity(Capacity);
}

//copia una función
void TFunction::Copy(const TFunction *F)
{
    //el puntro F debería pauntar a una función contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built function");

    //copia las propiedades
    Label = F->Label;
    p_Periodic = F->p_Periodic;
    p_T = F->p_T;
    p_Count = F->p_Count;
    p_X = F->p_X;
    p_Y = F->p_Y;
}
void TFunction::Copy(const TFunction& F)
{
    //copia las propiedades
    Label = F.Label;
    p_Periodic = F.p_Periodic;
    p_T = F.p_T;
    p_Count = F.p_Count;
    p_X = F.p_X;
    p_Y = F.p_Y;
}

//clona una función
void TFunction::Clone(const TFunction *F)
{
    //el puntro F debería pauntar a una función contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built function");

    //copia las propiedades
    Label = F->Label;
    p_Periodic = F->p_Periodic;
    p_T = F->p_T;
    p_Count = F->p_Count;
    p_X.Clone(F->p_X);
    p_Y.Clone(F->p_Y);
}
void TFunction::Clone(const TFunction &F)
{
    //copia las propiedades
    Label = F.Label;
    p_Periodic = F.p_Periodic;
    p_T = F.p_T;
    p_Count = F.p_Count;
    p_X.Clone(F.p_X);
    p_Y.Clone(F.p_Y);
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
    p_X.Clear();
    p_Y.Clear();
    p_Count = 0;
}

//añade un nuevo punto devolviendo la posición de inserción
int TFunction::Add(double x, double y)
{
    int i; //posición de inserción
    int ncycles=0; //índice al ciclo correspondiente a x

    //el dominio de definición de una función periódica es todo R
    if(p_Periodic)
        //Transplanta x al ciclo principal [0, T)
        if(x<0 || p_T<=x) {
            //transplanta x al ciclo principal
            ncycles = (int)floor(x/p_T);
            x -= p_T*ncycles;
        }

    if(p_Count) { //si hay algún punto
        //la coordenada x no deberá coincidir;
        //busca la primera coincidencia de x en X
        i = p_X.Search(x); //si no hay ninguno retorna i=Count

        if(i < p_Count) //si hay alguna coincidencia de x en X
            throw EImproperArgument("coincident stored x");

        //inserta el punto t.q. X queda ordenado
        i = p_X.InsertInc(x);
        p_Y.Insert(i, y);

    } else { //si la función está vacía
        //lo incluye directamente
        p_X.Add(x);
        p_Y.Add(y);

        i = p_Count; //captura la posición de inserción
    }

    p_Count++; //contabiliza el nuevo punto

    return i + p_Count*ncycles; //devuelve la posición de inserción
}

//empuja el último punto
void TFunction::PushLast(double x, double y)
{
    //El dominio de definición de una función periódica es todo R
    if(p_Periodic)
        //Transplanta x al ciclo principal [0, T)
        if(x<0 || p_T<=x)
            x -= p_T*floor(x/p_T);

    if(p_Count) {
        //debe estar más allá del último punto
        if(x <= p_X.getLast())
            throw EImproperArgument("point (x, y) should be posterior");

        p_X.PushLast(x);
        p_Y.PushLast(y);
    }
    else //si la función está vacía no se puede empujar
        throw EImproperCall("empty function");
}

//libera los priemros puntos
void TFunction::DelFirst(int n)
{
    //comprueba las precondiciones
    if(n<1 || p_Count<n)
        throw EImproperArgument("number items n should be in [1, Count]");

    p_X.DelFirst(n);
    p_Y.DelFirst(n);
    p_Count -= n;
}

//elimina el punto i-ésimo p.a. [0, Count-1] (si aperiódica)
//ó p.a. (-infinity, infinity) (si periódica
void TFunction::Delete(int i)
{
    int ncycles;
    if(p_Periodic) { //si es periódica
        //transplanta i al ciclo principal [0, Count-1]
        ncycles = (int)floor(double(i)/p_Count);
        i -= p_Count*ncycles;
    } else
        //si es aperiódica el dominio se restringe a [0, Count-1]
        if(i<0 || p_Count<=i)
            throw EImproperArgument("index i out bounds");

    p_X.Delete(i);
    p_Y.Delete(i);
    p_Count--;
}

//--------------------------------------------------------------------------

//Genera n puntos con distribuxión uniforme en el intervalo (ymin, ymax).
//Si la función es periódica, equiespaciados en el itnervalo [0, T);
//y si es aperiódica, en los puntos X = {0, 1, 2, ..., n-1}.
void TFunction::Random(int n, double ymin, double ymax)
{
    if(p_Periodic) {
        double deltax = p_T/n;
        for(int i=0; i<n; i++)
            Add(i*deltax, RandomUniform(ymin, ymax));
    }
    else
        for(int i=0; i<n; i++)
            Add(double(i), RandomUniform(ymin, ymax));
}

//Genera n puntos con imagen igual a cero.
//Si la función es periódica, equiespaciados en el itnervalo [0, T);
//y si es aperiódica, en los puntos X = {0, 1, 2, ..., n-1}
void TFunction::Zeros(int n)
{
    if(p_Periodic) {
        double deltax = p_T/n;
        for(int i=0; i<n; i++)
            Add(i*deltax, 0);
    } else
        for(int i=0; i<n; i++)
            Add(double(i), 0);
}

//--------------------------------------------------------------------------

TFunction& TFunction::operator=(const TFunction &F)
{
    p_Periodic = F.p_Periodic;
    p_T = F.p_T;
    p_Count = F.p_Count;
    p_X = F.p_X;
    p_Y = F.p_Y;

    //no vacia los vectores porque podrían asignarse a sí mismos

    return *this;
}

//compara con una función
bool TFunction::operator==(const TFunction &F) const
{
    if(p_Periodic!=F.p_Periodic || p_T!=F.p_T || p_Count!=F.p_Count)
        return false;

    for(int i=0; i<p_Count; i++)
        if(p_X[i]!=F.p_X[i] || p_Y[i]!=F.p_Y[i])
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
    if(p_Y.getCount() > 2)
        p_Y.SortInc(0, p_Y.getCount()-1);
}

//--------------------------------------------------------------------------
//EVALUACIÓN DE ASPECTOS DE LA FUNCIÓN:

//devuelve verdadero si x p.a. dominio de x
bool TFunction::BelongToDomain(double x) const
{
    //si la función está vacía el dominio está vacío
    if(p_Count < 1)
        return false;

    //si no es periódica el dominio es [X.getFirst(), X.getLast()]
    if(!p_Periodic && (x<p_X.getFirst() || p_X.getLast()<x))
        return false;

    return true;
}

//Evalúa la función en x
double TFunction::Image(double x) const
{
    //Para poder evaluarla, la función debe contener almenos un punto
    if(p_Count<1)
        throw EImproperArgument("empty function");

    if(p_Periodic)
        //transplanta x al ciclo principal [0, T)
        x -= p_T*floor(x/p_T);
    else
        //la evaluación está restringida al dominio de definción
        if(!BelongToDomain(x))
            throw EImproperArgument("x dont belong to Domain");

    //busca el primer X[i]>=x
    int i = p_X.FirstUpperOrEqualTo(x);
    //si es periódica, i indicará un punto en [0, Count]
    //si no, i índicará un punto en [0, Count-1]

    //determina los vértices del segmento que contiene el punto
    double x1, y1; //coordenadas del punto origen del segmento
    double x2, y2; //coordenadas del punto final del segmento
    if(i<1) {
        x1 = p_X.getLast() - p_T; y1 = p_Y.getLast(); //el último del ciclo previo
        x2 = p_X.getFirst(); y2 = p_Y.getFirst(); //el primero del ciclo principal
    } else if(p_Count<=i) {
        x1 = p_X.getLast(); y1 = p_Y.getLast(); //el último del ciclo principal
        x2 = p_X.getFirst() + p_T; y2 = p_Y.getFirst(); //el primero del ciclo post.
    } else {
        x1 = p_X[i-1]; y1 = p_Y[i-1]; //p1 es el punto previo
        x2 = p_X[i]; y2 = p_Y[i]; //p2 es el punto indicado
    }

    //interpola x entre p1 y p2
    return y1 + (x - x1)*(y2 - y1)/(x2 - x1);
}

//busca el punto (virtual) que contiene la abcisa x
bool TFunction::Find(double x, int &i) const
{
    int ncycles=0; //índice al ciclo al que pertenece x
    //El dominio de definición de una función periódica es todo R
    if(p_Periodic) //si es periódica...
        if(x<0 || p_T<=x) { //...y no está en el ciclo principal
            //Transplanta x al ciclo principal [0, T)
            ncycles = (int)floor(x/p_T);
            x -= p_T*ncycles;
        }

    //busca un punto de abcisa x
    i = p_X.Search(x);

    if(i < p_Count) { //si lo encuentra
        //desplaza i al ciclo de x
        i += p_Count*ncycles;
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
    //comprueba las precondiciones
    if(p_Count == 0)
        throw EImproperArgument("funtion should not be empty");

    //estado de análisis
    //      -1: monótona decreciente;
    //      0: constante;
    //      1: monótona creciente;
    //      2: no monótona.
    int monotony=0;

    //paracada ordenada de la función
    for(int i=1; i<p_Count; i++) {
        switch(monotony) {
        case -1: //monótona decreciente
            if(p_Y[i] > p_Y[i]-1)
                return 2; //indica que no es monótoma
            break;
        case 1: //monótona creciente
            if(p_Y[i] < p_Y[i]-1)
                return 2; //indica que no es monótona
            break;
        case 0: //constante
            if(p_Y[i] < p_Y[i-1])
                monotony = -1;
            else if(p_Y[i] > p_Y[i-1])
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
    if(p_Periodic) {
        //lleva x1 al ciclo principàl
        n1 = (int)floor(x1/p_T);
        x1 -= n1*p_T;
        //lleva x2 al ciclo principal
        n2 = (int)floor(x2/p_T);
        x2 -= n2*p_T;
    }

    //j1 al primer punto p.a. [x1, x2]
    int j1 = p_X.FirstUpperOrEqualTo(x1); //si X[j]<x1 p.t. j, dev. Count
    //si el primer punto está antes que x1
    if(!p_Periodic && j1>=p_X.getCount())
        return false; //no hay puntos internos

    //j2 al último punto p.a. [x1, x2]
    int j2 = p_X.LastLowerOrEqualTo(x2); //si x2<X[i] p.t. i, devolverá -1
    //si el último punto está después que x2
    if(!p_Periodic && j2<0)
        return false; //no hay puntos internos

    //si es periódica deben restaurarse los ciclos
    if(p_Periodic) {
        i1 = j1 + n1*p_Count; //restaura los ciclos y apunta al pto previo
        i2 = j2 + n2*p_Count + 1; //rest. los ciclos y apta al pto post.
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
    if(p_Periodic) {
        //lleva x1 al ciclo principàl
        n1 = (int)floor(x1/p_T);
        x1 -= n1*p_T;
        //lleva x2 al ciclo principal
        n2 = (int)floor(x2/p_T);
        x2 -= n2*p_T;
    }

    //j1 al primer punto p.a. (x1, x2)
    int j1 = p_X.FirstUpperTo(x1); //si X[j]<x1 p.t. j, devolverá Count
    //si el primer punto está en x1 o antes
    if(!p_Periodic && j1>=p_Count)
        return false; //no hay puntos internos

    //j2 al último punto p.a. (x1, x2)
    int j2 = p_X.LastLowerTo(x2); //si x2<X[i] p.t. i, devolverá -1
    //si el último punto está en x2 o después
    if(!p_Periodic && j2<0)
        return false; //no hay puntos internos

    //si es periódica deben restaurarse los ciclos
    //y apuntarse a los puntos previo y posterior a j1 y j2
    if(p_Periodic) {
        //se restauran los ciclos
        i1 = j1 + n1*p_Count - 1; //y se apunta al punto previo
        i2 = j2 + n2*p_Count + 1; //t se apunta al punto posterior
    } else { //si es aperiódica puediera no haber punto previo/post. a j1/j2
        i1 = max(0, j1 - 1); //apunta al pto previo en caso de haberlo
        i2 = min(p_X.getCount() - 1, j2 + 1); //apunta al pto post. si lo hay
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
    if(p_Periodic) {
        //lleva x1 al ciclo principàl
        n1 = (int)floor(x1/p_T);
        x1 -= n1*p_T;
        //lleva x2 al ciclo principal
        n2 = (int)floor(x2/p_T);
        x2 -= n2*p_T;
    }

    //j1 al primer punto p.a. [x1, x2]
    int j1 = p_X.FirstUpperOrEqualTo(x1); //si X[j]<x1 p.t. j, dev. Count
    //si el primer punto está antes que x1
    if(!p_Periodic && j1>=p_Count)
        return false; //no hay puntos internos

    //j2 al último punto p.a. [x1, x2]
    int j2 = p_X.LastLowerOrEqualTo(x2); //si x2<X[i] p.t. i, devolverá -1
    //si el último punto está después que x2
    if(!p_Periodic && j2<0)
        return false; //no hay puntos internos

    //si es periódica siempre habrá punto previo y posterior
    if(p_Periodic) {
        //se restauran los ciclos
        i1 = j1 + n1*p_Count - 1; //y se apunta al punto previo
        i2 = j2 + n2*p_Count + 1; //y se aputna al punto posterior
    } else { //si es aperiódica puediera no haber punto previo/post. a j1/j2
        i1 = max(0, j1 - 1); //apunta al pto previo en caso de haberlo
        i2 = min(p_X.getCount()-1, j2+1); //apunta al pto post. si lo hay
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

        anterior = p_Y[i1 + 1] - p_Y[i1];
        for(int i=i1+1; i<i2; i++) {
            siguiente = p_Y[i+1] - p_Y[i];
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

        anterior = p_Y[i1 + 1] - p_Y[i1];
        for(int i=i1+1; i<i2; i++) {
            siguiente = p_Y[i+1] - p_Y[i];
            //si es punto de inflexión lo incorpora a V
            if((anterior==0 && siguiente!=0) ||
                    (anterior!=0 && siguiente==0) ||
                    (anterior>0 && siguiente<0) ||
                    (anterior<0 && siguiente>0))
                V.Add(p_X[i]);
            anterior = siguiente;
        }
    }
}

//--------------------------------------------------------------------------
//OPERACIONES CON LA FUNCIÓN:

//determina si la función tiene inversa
bool TFunction::haventInverse(void) const
{
    //determina el tipo de monotonía
    int monotony = Monotony();

    //if function F contain most one sample, should not be constant
    if(p_Count > 1 && monotony==0)
        return true;

    //function F should not be not monotone
    if(monotony<-1 || 1<monotony)
        return true;

    //indicates that the function have inverse
    return false;
}
//asigna la función inversa de F
void TFunction::Inverse(const TFunction &F)
{
    //determina el tipo de monotonía
    int monotony = F.Monotony();

    //comprueba las precondiciones
    if(F.p_Count > 1 && monotony==0)
        throw EImproperArgument("if function F contain most one sample, should not be constant");
    if(monotony<-1 || 1<monotony)
        throw EImproperArgument("function F should not be not monotone");

    double x; //variable auxiliar

    //vacia la función en congruencia con la situación inciial
    Clear();

    //si hay alguna muestra
    if(F.p_Count > 0)
        //añade la primera muestra transpuesta
        Add(F.getYFirst(), F.getXFirst());

    //por cada muestra sucesiva
    for(int i=1; i<F.p_Count; i++) {
        //lee la abcisa
        x = F.p_Y[i];

        //si la abcisa coincide conla previa
        if(x == getXLast())
            //actualiza laordenada
            setYLast(F.p_X[i]);
        else //si es una nueva abcisa
            //añadelacoordenada transpuesta
            Add(x, F.p_X[i]);
    }
}

//asigna la función normalizada de F
void TFunction::Normalized(const TFunction &F)
{
    //vacia la función en congruencia con el estado inicial
    Clear();

    //si la función contiene alguna muestra
    if(F.p_Count > 0) {
        //determina el valor máximo de F
        double ymax = F.getYMax();

        //por cada punto de muestreo
        for(int i=0; i<F.p_Count; i++)
            //añade el punto d emuestreo normalizado
            Add(F.p_X[i], F.p_Y[i]/ymax);
    }
}

//actualiza las ordenadas
//      Y[i] = min(Y[i], F.Y[i])
//las abcisas debén coincidir
void TFunction::OrdinatesMin(TFunction &F)
{
    //comprueba las precondiciones
    if(F.p_Count != p_Count)
        throw EImproperArgument("point number F.Count should be equal to point number Count");

    //por cada punto de la función
    for(int i=0; i<p_Count; i++)
        //la abcisa F.X[i] debería ser igual a la abcisa X[i]
        if(F.p_X[i] != p_X[i])
            throw EImproperArgument("abcise F.X[i] should be equal to abcise X[i]");

    //por cada punto de la función
    for(int i=0; i<p_Count; i++)
        //asigna la ordenada mínima
        p_Y[i] = min(p_Y[i], F.p_Y[i]);
}
//actualiza las ordenadas
//      Y[i] = Y[i]*((n-1)/n) + F.Y[i]/n
//las abcisas debén coincidir
void TFunction::OrdinatesAve(TFunction &F, int n)
{
    //comprueba las precondiciones
    if(F.p_Count != p_Count)
        throw EImproperArgument("point number F.Count should be equal to point number Count");

    //por cada punto de la función
    for(int i=0; i<p_Count; i++)
        //la abcisa F.X[i] debería ser igual a la abcisa X[i]
        if(F.p_X[i] != p_X[i])
            throw EImproperArgument("abcise F.X[i] should be equal to abcise X[i]");

    //elnúmero de muestra n debe ser al menos dos
    if(n < 2)
        throw EImproperArgument("samplenumber n should be almost two");

    //por cada punto de la función
    for(int i=0; i<p_Count; i++)
        //asigna la ordenada mínima
        p_Y[i] = p_Y[i]*((n-1)/n) + F.p_Y[i]/n;
}
//actualiza las ordenadas
//      Y[i] = max(Y[i], F.Y[i])
//las abcisas debén coincidir
void TFunction::OrdinatesMax(TFunction &F)
{
    //comprueba las precondiciones
    if(F.p_Count != p_Count)
        throw EImproperArgument("point number F.Count should be equal to point number Count");

    //por cada punto de la función
    for(int i=0; i<p_Count; i++)
        //la abcisa F.X[i] debería ser igual a la abcisa X[i]
        if(F.p_X[i] != p_X[i])
            throw EImproperArgument("abcise F.X[i] should be equal to abcise X[i]");

    //por cada punto de la función
    for(int i=0; i<p_Count; i++)
        //asigna la ordenada máxima
        p_Y[i] = max(p_Y[i], F.p_Y[i]);
}

//---------------------------------------------------------------------------

} //namespace Operators

//--------------------------------------------------------------------------

