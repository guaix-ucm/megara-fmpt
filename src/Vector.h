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
///@file Vector.h
///@brief template for vector of numerical types
///@author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef VECTOR_H
#define VECTOR_H

#include "ItemsList.h"
#include "Scalars.h"

//---------------------------------------------------------------------------

using namespace Mathematics;

//espacio de nombres de funciones y clases de objetos
//relacionados con listas, vectores y matrices
namespace Lists {

//---------------------------------------------------------------------------
//clase TVector
//---------------------------------------------------------------------------

//TVector define una TItemList con funciones externas de tipo numérico

//clase vector de elementos numéricos
template <class T> class TVector : public TItemsList<T> {
protected:
    //PROPIEDADES DE ACCESO A ELEMENTOS:

    //MÉTODOS ESTÁTICOS DE INTERFAZ:

    //compara dos elementos
    static int  FCompare(T, T);
    //evalúa un elemento
    static double  FEvaluate(T);
    //asigna un valor a un elemento
    static void  FAssign(T&, double);

    //imprime un elemento en punto entero
    static void  FPrintInt(AnsiString&, T);
    //imprime un elemento en punto floatante
    static void  FPrintFloat(AnsiString&, T);

    //escudriña un elemento en punto entero
    static void  FReadInt(T&, const AnsiString&, int&);
    //escudriña un elemento en punto flotante
    static void  FReadFloat(T&, const AnsiString&, int&);

    //PROPIEDADES EN FORMATO TEXTO:

public:
/*    //PROPIEDADES DE ACCESO A ELEMENTOS:

    inline const T getFirst(void) const;
    inline void setFirst(const T);
    inline const T getLast(void) const;
    inline void setLast(const T);

    //Los métodos son redefinidos para que los valores sean
    //pasados y devueltos por valor. Como se trata de una lista
    //de elementos primitivos, ya no es conveniente pasarlos por referencia.
*/
    //PROPIEDADES EN FORMATO TEXTO:

    //componentes del vector separadas por un espacio en base decimal
    AnsiString getRowDecText(void);
    //componentes del vector separadas por un espacio en base hexadecimal
    AnsiString getRowHexText(void);

    //MÉTODOS ESTÁTICOS DE INTERFACE:

    static void  PrintVector(AnsiString &S, const TVector<T> *V) {
        try {
            PrintList(S, V);
        } catch(...) {
            throw;
        }
    }
    static void  ReadVector(TVector<T> *V, const AnsiString &S, int &i) {
        try {
            TItemsList<T>::ReadList(V, S, i);
        } catch(...) {
            throw;
        }
    }

    //NOTA: define las funcines PrintVector y ReadVector con el objeto
    //de poder pasar las funciones PrintList y ReadList como argumentos
    //de tipo de tipo TVector (y no de tipo TItemList) sin necesidad
    //de aplicar un cast a las funciones.

    //MÉTODOS PÚBLICOS:

    //construye un vector con la capacidad inicial indicada
    TVector(int Capacity=8);

    //constructor de copia
    TVector(TVector *);

    void SetDimension(int); //redimensiona el vector y completa con ceros

    //copia las componentes de un vector
    TVector<T>& operator=(const TVector<T>&);

    //acumula las componentes de un vector
    void Acum(const TItemsList<T>&);
    //asigna las componentes al cuadrado de un vector
    void Pow2(const TItemsList<T>&);

    //acumula las componentes del vector
    void Acum(void);
    //eleva al cuadrado las componentes del vector
    void Pow2(void);
    //aplica la raiz cuadrada a las componentes del vector
    void Sqrt(void);
};

//lee una tabla de pares (x, y) en punto flotante
void  StrReadTable(TVector<double> &X, TVector<double> &Y,
                             const AnsiString &S, int &i);

//ADVERTENCIA: si pasa un objeto tipo TVector<T> por valor
//se producirá un error cuando el vector que se le pase
//tenga más de 8 elementos, ya que el ovjeto es construido
//con el número de elementos por defecto y el vector
//que se le pase es copiado byte a byte, no permitiendo
//el redimensionado automático.

//---------------------------------------------------------------------------
//MÉTODOS PROTEGIDOS:

//compara dos elementos
template <class T> int  TVector<T>::FCompare(T item1,
                                                       T item2)
{
    /*      //los elementos deben ser distintos de NaN
        if(item1<-MAXDOUBLE || MAXDOUBLE<item1 ||
                item2<-MAXDOUBLE || MAXDOUBLE<item2)
                throw EImproperArgument("can't compare NaN value");
  */
    if(item1 < item2)
        return -1;
    else if(item1 > item2)
        return 1;
    else
        return 0;
}
//evalúa un elemento
template <class T> double  TVector<T>::FEvaluate(T item)
{
    return double(item);
}
//asigna un valor a un elemento
template <class T> void  TVector<T>::FAssign(T &item, double value)
{
    item = (T)value;
}

//imprime un elemento
template <class T> void  TVector<T>::FPrintInt(AnsiString &S,
                                                         T item)
{
    S += IntToStr((int)item);
}
//imprime un elemento
template <class T> void  TVector<T>::FPrintFloat(AnsiString &S,
                                                           T item)
{
    S += FloatToStr((double)item);
}

//escudriña un texto
template <class T> void  TVector<T>::FReadInt(T &item,
                                                        const AnsiString &S, int &i)
{
    int aux;
    StrReadInt(aux, S, i);
    item = (T)aux;
}
//escudriña un texto
template <class T> void  TVector<T>::FReadFloat(T &item,
                                                          const AnsiString &S, int &i)
{
    double aux;
    StrReadFloat(aux, S, i);
    item = (T)aux;
}

/*//---------------------------------------------------------------------------
//PROPIEDADES DE ACCESO A ELEMENTOS:

template <class T> inline const T TVector<T>::getFirst(void) const
{
    //el número de elementos de la lista getCount debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("list items number getCount should be one almost");

    return Items.getFirst();
}
template <class T> inline void TVector<T>::setFirst(const T item)
{
    //el número de elementos de la lista getCount debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("list items number getCount should be one almost");

    Items.First = item;
}

template <class T> inline const T TVector<T>::getLast(void) const
{
    //el número de elementos de la lista getCount debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("list items number getCount should be one almost");

    return Items.getLast();
}
template <class T> inline void TVector<T>::setLast(const T item)
{
    //el número de elementos de la lista getCount debería ser al menos uno
    if(getCount < 1)
        throw EImproperCall("list items number getCount should be one almost");

    Items.Last = item;
}
*/
//---------------------------------------------------------------------------
//PROPIEDADES EN FORMATO TEXTO:

template <class T> AnsiString TVector<T>::getRowDecText(void)
{
    AnsiString S;

    //imprime los bytes de datos
    for(int i=0; i<this->getCount(); i++)
        S += IntToStr(this->Items[i])+" ";

    return S;
}
template <class T> AnsiString TVector<T>::getRowHexText(void)
{
    AnsiString S;

    //imprime los bytes de datos
    for(int i=0; i<this->getCount(); i++)
        S += IntToHex(this->Items[i], 2*sizeof(T))+" ";

    return S;
}

//---------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//construye un vector con la capacidad inicial indicada
template <class T> TVector<T>::TVector(int Capacity) :
    TItemsList<T>(Capacity)
{
    //el tipo T debería ser numérico
    if(!Numerical(typeid(T)))
        throw EImproperCall("type T should be numerical");

    this->Compare = TVector<T>::FCompare;
    this->Evaluate = TVector<T>::FEvaluate;
    this->Assign = TVector<T>::FAssign;

    if(FloatingPoint(typeid(T))) {
        this->Print = TVector<T>::FPrintFloat;
        this->Read = TVector<T>::FReadFloat;
    } else {
        this->Print = TVector<T>::FPrintInt;
        this->Read = TVector<T>::FReadInt;
    }
}

//constructor de copia
template <class T> TVector<T>::TVector(TVector *V)
//se construye aquí el array deslizante por defecto
{
    //el puntero V debería apuntar a un vector construido
    if(V == NULL)
        throw EImproperArgument("V should point to built vector");

    //clona el array deslizante
    this->Items.Clone(V->Items);

    //apunta a las mismas funciones externas
    this->Compare = V->Compare;
    this->Evaluate = V->Evaluate;
    this->Assign = V->Assign;
    this->Print = V->Print;
    this->Read = V->Read;
}

//redimensiona el vector y completa con ceros
template <class T> void TVector<T>::SetDimension(int Dimension)
{
    //la dimensión debe ser no negativa
    if(Dimension < 0)
        throw EImproperArgument("Dimension should be non-negative");

    int i = this->getCount(); //indica al primer nuevo parámetro
    this->setCount(Dimension); //redimensiona el vector
//    for(; i<this->getCount(); i++) //a cada nuevo parámetro
    for(; i<Dimension; i++) //a cada nuevo parámetro
        this->Items[i] = 0; //le asigna cero

    //ERROR 01-02-2016: estaba haciendo el bucle desde getCount() <getCount(),
    //con loq ue saltaba warning de no usado:
    //In file included from Geometry.cpp:30:
    //./Vector.h:323:9: warning: expression result unused [-Wunused-value]
    //    for(; i<this->getCount(); i++) //a cada nuevo parámetro

}

//copia los elemeentos de una lista
template <class T> TVector<T>& TVector<T>::operator=(
        const TVector<T> &v)
{
    //redimensiona el vector
    if(v.getCount() < this->getCount())
        this->Items.DelLast(this->getCount() - v.getCount());
    else if(v.getCount() > this->getCount())
        this->Items.NewLast(v.getCount() - this->getCount());

    //copia las componentes
    for(int i=0; i<this->getCount(); i++)
        this->Items[i] = v[i];

    return *this;
}

//acumula las componentes de un vector
template <class T> void TVector<T>::Acum(const TItemsList<T> &v)
{
    //equipara la longitud del vector
    this->setCount(v.getCount());

    //si el vector no tiene ninguna componente
    if(this->getCount() < 1)
        return; //no hace nada más

    //asigna la primera componente
    this->Items.First = v.First;
    //por cada una de las siguientes componentes
    for(int i=1; i<this->getCount(); i++)
        this->Items[i] = this->Items[i-1] + v[i];
}
//asigna las componentes al cuadrado de un vector
template <class T> void TVector<T>::Pow2(const TItemsList<T> &v)
{
    //equipara la longitud del vector
    this->setCount(v.getCount());

    //si el vector no tiene ninguna componente
    if(this->getCount() < 1)
        return; //no hace nada más

    //por cada componente del vector
    for(int i=0; i<this->getCount(); i++)
        //asigna la componente al cuadrado del vector v
        this->Items[i] = pow(v[i], 2.);
}

//acumula las componentes del vector
template <class T> void TVector<T>::Acum(void)
{
    //por cada componente posterior a la primera
    for(int i=1; i<this->getCount(); i++)
        //suma la componente previa a la actual
        this->Items[i] = this->Items[i-1] + this->Items[i];
}
//eleva al cuadrado las componentes del vector
template <class T> void TVector<T>::Pow2(void)
{
    //por cada componente del vector
    for(int i=0; i<this->getCount(); i++)
        //eleva al cuadrado la componente
        this->Items[i] = pow(this->Items[i], 2.);
}
//aplica la raiz cuadrada a las componentes del vector
template <class T> void TVector<T>::Sqrt(void)
{
    //por cada componente del vector
    for(int i=0; i<this->getCount(); i++)
        //aplica la razi cuadrada a la componente
        this->Items[i] = sqrt(this->Items[i]);
}

//---------------------------------------------------------------------------

} //namespace Lists

//---------------------------------------------------------------------------
#endif // VECTOR_H
