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
/// @file Function.h
/// @brief contonuous function of continuous time defined in sample points
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FUNCTION_H
#define FUNCTION_H

#include "Vector.h"

//---------------------------------------------------------------------------

using namespace Lists;

//espacio de nombres de operadores matemáticos
namespace Operators {

//---------------------------------------------------------------------------
//class TFunction {...}; define una clase de objeto compuesta de una lista de
//puntos de una función continua de tiempo continuo, periódica o aperiódica,
//cuya evaluación se obtiene por interpolación lineal.
//
//al especificar uno o más puntos
//la función quedará definida en todo su dominio de definición:
//Dom{x} = [X.Min, X.Max] para funciones aperiódicas.
//Dom{x} = [MINDOUBLE, MAXDOUBLE] para funciones T-periódicas.
//solo puede representar funciones continuas, por lo que no está permitido
//añadir dos puntos con la misma abcisa.
//
//La función periódica define el ciclo principal mediante los puntos de
//muestreo en el intervalo [0, T) y los puntos de muestreo se repiten
//T-periódicamente pudiendo ser accedidos mediante los métodos X(i), Y(i) y
//P(i).
//---------------------------------------------------------------------------

class TFunction {
protected:
    bool p_Periodic; //true: T-periódica; false: aperiódica
    double p_T; //periodo de la función periódica

    //Es conveniente disponer las coordenadas de los puntos de muestreo en
    //vectores separados para poder acceder a las propiedades de cada
    //vector, sean: Min, Max, First, Last, etc;
    //o para facilitar la adición ordenada.
    //Tambien conviene disponer una variable p_Count que sirva de referente
    //del tamaño primitivo en los métodos que añaden o borran puntos de
    //muestreo.

    int p_Count; //nº de puntos específicos de la función
    TVector<double> p_X; //vector de ordenadas
    TVector<double> p_Y; //vector de abcisas

public:
    AnsiString Label; //texto descriptivo de la función
    bool getPeriodic(void) const {return p_Periodic;}
    void setPeriodic(bool);
    double getT(void) const {return p_T;}
    void setT(double);
    int getCount(void) const {return p_Count;}

    //ACCESO A LOS PUNTOS DE MUESTREO:

    //asigna los vectores de abcisas y ordenadas
    void Set(const TVector<double>& X, const TVector<double>& Y);

    //abcisa indicada
    double getX(int) const;
    void setX(int, double);
    //ordenada indicada
    double getY(int) const;
    void setY(int, double);
    //par (x, y) indicado
    TDoublePoint getP(int) const;
    void setP(int, TDoublePoint);

    //primera abcisa
    double getXFirst(void) const {return p_X.getFirst();}
    void setXFirst(double XFirst) {p_X.setFirst(XFirst);}
    //úñtima abcisa
    double getXLast(void) const {return p_X.getLast();}
    void setXLast(double XLast) {p_X.setLast(XLast);}
    //primera ordenada
    double getYFirst(void) const {return p_Y.getFirst();}
    void setYFirst(double YFirst) {p_Y.setFirst(YFirst);}
    //última ordenada
    double getYLast(void) const {return p_Y.getLast();}
    void setYLast(double YLast) {p_Y.setLast(YLast);}
    //odenada mínima
    double getYMin(void) const {return p_Y.getMin();}
    //ordenada máxima
    double getYMax(void) const {return p_Y.getMax();}
    //media de las ordenadas
    double getYAve(void) const {return p_Y.getAve();}

    //PROPIEDADES EN FORMATO TEXTO:

    //vector de abcisas en formato texto
    AnsiString getXText(void) const {return p_X.getText();}
    //vector de ordenadas en formato texto
    AnsiString getYText(void) const {return p_Y.getText();}

    //puntoa de la función en formato texto
    //{(x2 y2)...(xCount yCount)}
    AnsiString getPointsText(void) const;
    void setPointsText(const AnsiString&);
    //puntos de la función en formato tabla de texto
    AnsiString getTableText(void) const;
    void setTableText(const AnsiString&);

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    AnsiString getAllText(void) const;

    //MÉTODOS ESTÁTICOS:

    //imprime los puntos de una función en una cadena
    static void  PrintPoints(AnsiString &S, const TFunction *F);
    //lee una cadena en busca de los puntos de una función
    static void  ReadPoints(TFunction *F, const AnsiString &S,
                                      int &i);

    //imprime los puntos de una función en una cadena
    static void  PrintTable(AnsiString &S, const TFunction *F);
    //lee una cadena en busca de los puntos de una función
    static void  ReadTable(TFunction *F, const AnsiString &S,
                                     int &i);

    //MÉTODOS DE CONTRUCCIÓN COPIA Y DESTRUCCIÓN:

    //construye una función aperiódica
    TFunction(int Capacity=128);
    //construye una función T-periódica
    TFunction(double T, int Capacity=128);

    //copia una función
    void Copy(const TFunction*);
    void Copy(const TFunction&);

    //clona una función
    void Clone(const TFunction*);
    void Clone(const TFunction&);

    //construye un clon de una función
    TFunction(const TFunction*);

    //GESTIÓN DE LA LISTA DE PUNTOS DE MUESTREO:

    void Clear(void); //borra los puntos y restablece Count=0
    //añade un nuevo punto devolviendo la posición de inserción
    int Add(double x, double y);
    int Add(TDoublePoint P) {return Add(P.x, P.y);}
    void PushLast(double x, double y); //empuja el último punto
    void DelFirst(int n); //borra los primeros puntos
    void Delete(int i); //borra el punto indicado

    //genera n puntos con distribuxión uniforme en el intervalo (ymin, ymax)
    //si la función es periódica equiespaciados en el itnervalo [0, T). Y si
    //es aperiódica en los puntos X = {0, 1, 2, ..., n-1}
    void Random(int n, double ymin, double ymax);
    //genera n puntos con y = 0
    //si la función es periódica equiespaciados en el intervalo [0, T)
    //si es aperiódica en los puntos X = {0, 1, 2, ..., n-1}
    void Zeros(int n);

    TFunction& operator=(const TFunction &F); //copia una función
    bool operator==(const TFunction &F) const; //compara con una función
    bool operator!=(const TFunction &F) const; //compara con una función

    //ordena las muestras de la función de menor a mayor ordenada
    //aplicando el algoritmo quick sort al intervalo [first, last]
    void SortInc(void);

    //EVALUACIÓN DE ASPECTOS DE LA FUNCIÓN:

    //devuelve verdadero si x p.a. dominio de definición de la función
    bool BelongToDomain(double x) const;
    //retorna la imagen de x p.a. Dom{x}
    double Image(double x) const;
    //busca el punto (virtual) que contiene la abcisa x
    bool Find(double x, int &i) const;

    //determina el tipo de monotonía de la función
    //      -1: monótona decreciente;
    //      0: constante;
    //      1: monótona creciente;
    //      2: no monótona.
    //si la función está vacía lanza una excepción EImproperCall
    int Monotony(void) const;

    //determina el intervalo [i1, i2] conteniendo los índices a los vértices
    //de los segmentos completamente contenidos en [x1, x2]
    //si la función no tiene ningún segmento
    //completamente contenido en [x1, x2], devuelve false
    bool InnerSegments(double x1, double x2, int& i1, int& i2) const;
    //determina el intervalo [i1, i2] conteniendo los índices a los vértices
    //de los segmentos extremos que tienen almenos una parte en (x1, x2)
    //si la función no tiene segmentos con almenos una parte en (x1, x2)
    //esta función devuelve false
    bool IntrusiveSegments(double x1, double x2, int& i1, int& i2) const;
    //determina el intervalo [i1, i2] conteniendo los índices a los vértices
    //de los segmentos que tienen almenos un punto en el intervalo [x1, x2]
    //si la función no tiene segmentos con almenos un punto en [x1, x2],
    //esta función devuelve false
    bool AdjacentSegments(double x1, double x2, int& i1, int& i2) const;

    //determina la secuencia de índices a los puntos de inflexión
    //de la función, pertenecientes al itnervalo [x1, x2]
    void InflexPoints(double x1, double x2, TVector<int> &V) const;
    //determina la secuencia de puntos de inflexión
    //de la función, pertenecientes al itnervalo [x1, x2]
    void InflexPoints(double x1, double x2, TVector<double> &V) const;

    //OPERACIONES CON LA FUNCIÓN:

    //determina si la función tiene inversa
    bool haventInverse(void) const;
    //asigna la función inversa de F
    void Inverse(const TFunction &F);
    //asigna la función normalizada de F
    void Normalized(const TFunction &F);

    //actualiza las ordenadas
    //      Y[i] = min(Y[i], F.Y[i])
    //las abcisas debén coincidir
    void OrdinatesMin(TFunction &F);
    //actualiza las ordenadas
    //      Y[i] = Y[i]*((n-1)/n) + F.Y[i]/n
    //las abcisas debén coincidir
    void OrdinatesAve(TFunction &F, int n);
    //actualiza las ordenadas
    //      Y[i] = max(Y[i], F.Y[i])
    //las abcisas debén coincidir
    void OrdinatesMax(TFunction &F);
};

//---------------------------------------------------------------------------

} //namespace Operators

//---------------------------------------------------------------------------
#endif // FUNCTION_H
