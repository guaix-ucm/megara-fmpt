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
//File: SlideArray.h
//Content: template for slide array
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef SLIDEARRAY_H
#define SLIDEARRAY_H

#include "Exceptions.h"

//---------------------------------------------------------------------------

//namespace for lists
namespace Lists {

//---------------------------------------------------------------------------
//class TSlideArray
//---------------------------------------------------------------------------

//TslideArray define arrays deslizantes
//autoredimensionables de Count elementos tipo T. Los arrays deslizantes
//permiten la adición, borrado y apilado de elementos en los límites de
//un array de una forma rápida y eficiente. Además reduce a la mitad los
//tiempos de inserción y borrado.

//class slide array
template <class T> class TSlideArray {
protected:
    int p_Capacity;
    int p_Count;

    int p_ifirst; //índice al primer elemento reservado
    int p_ilast; //índice al último elemento reservado

public:
    T *Items; //aray de elementos

    //número de elementos del array
    int getCapacity(void) const {return p_Capacity;}
    void setCapacity(int);
    //número de elementos reservados del array
    int getCount(void) const {return p_Count;}

    //accede al primer elemento reservado
    //      First = ITems[ifirs]
    inline const T &getFirst(void) const;
    inline void setFirst(T&);
    //accede al último elemento reservado
    //      Last = ITems[ilast]
    inline const T &getLast(void) const;
    inline void setLast(T&);

        //MÉTODOS PÚBLICOS:

    //construye un array deslizante
    TSlideArray(int Capacity=8, int Count=0);
    //construye un clon de un array deslizante
    TSlideArray(TSlideArray &SA);
    //libera la memoria ocupada por el array deslizante
    ~TSlideArray() {delete Items;}

    //ACCESO A ELEMENTOS:

    //lee el elemento reservado indicado
    inline const T &Get(int i) const;
    //escribe el elemento reservado indicado
    inline void Set(int i, const T&);

    //accede al elemento reservado indicado en arrays no constantes
    inline T& operator[](int i);
    //accede al elemento reservado indicado en arrays constantes
    inline const T& operator[](int i) const;

    //NOTA: si la lista es constante, se invocará a la función constante
    //que devuelve una referencia a constante impidiendo su modificación.

    //REDIMENSIONAMIENTO Y DESLIZAMIENTO:

    //añade N elementos reservados por el principio
    inline void NewFirst(int N=1);
    //borra N elementos reservados por el principio
    inline void DelFirst(int N=1);

    //añade N elementos reservados por el final
    inline void NewLast(int N=1);
    //borra N elementos reservados por el final
    inline void DelLast(int N=1);

    //desliza la ventana de resrva N posiciones a la izquierda
    inline void SlideLeft(int N=1);
    //desliza la ventana de reserva N posiciones a la derecha
    inline void SlideRight(int N=1);

    //añade un elemento por el final
    void AddLast(const T &item);
    //añade un elemento por el principio
    void AddFirst(const T &item);
    //inserta un elemento en la posición indicada
    void Insert(int i, const T &item);

    //COPIADO:

    //copia todas las propiedades del array incluidos sus elementos
    void Clone(const TSlideArray &);
    //equipara el número de elementos reservados del array y los copia
    void Copy(const TSlideArray&);
};

//---------------------------------------------------------------------------
//Métodos de la clase TSlideArray:
//---------------------------------------------------------------------------

template <class T> void TSlideArray<T>::setCapacity(int Capacity)
{
    //el número de elementos Capacity no debe ser menor que el número de elementos reservados Count
    if(Capacity < p_Count)
        throw EImproperArgument("items number Capacity should not be less reserved items number Count");

    //construye un nuevo array
    T *NewItems = new T[Capacity];

    //copia los elementos en el nuevo array
    if(p_Count) {
        int i = p_ifirst; //índice para Items
        int j=0; //índice para NewItems
        if(p_ifirst <= p_ilast)
            for(; i<=p_ilast; i++)
                NewItems[j++] = Items[i];
        else {
            for(; i<p_Capacity; i++)
                NewItems[j++] = Items[i];
            for(i=0; i<=p_ilast; i++)
                NewItems[j++] = Items[i];
        }
    }

    //apunta el nuevo array
    delete Items;
    Items = NewItems;
    p_Capacity = Capacity;

    //actualiza los límites de la ventana de reserva
    p_ifirst = 0;
    if(p_Count)
        p_ilast = p_Count - 1;
    else
        p_ilast = p_Capacity - 1;
}

template <class T> inline const T &TSlideArray<T>::getFirst(void) const
{
    //el número de elementos reservados Count debería ser al menos uno
    if(p_Count < 1)
        throw EImproperCall("reserved items number Count chould be one almost");

    return Items[p_ifirst];
}
template <class T> inline void TSlideArray<T>::setFirst(T &item)
{
    //el número de elementos reservados Count debería ser al menos uno
    if(p_Count < 1)
        throw EImproperCall("reserved items number Count chould be one almost");

    Items[p_ifirst] = item;
}

template <class T> inline const T &TSlideArray<T>::getLast(void) const
{
    //el número de elementos reservados Count debería ser al menos uno
    if(p_Count < 1)
        throw EImproperCall("reserved items number Count chould be one almost");

    return Items[p_ilast];
}
template <class T> inline void TSlideArray<T>::setLast(T &item)
{
    //el número de elementos reservados Count debería ser al menos uno
    if(p_Count < 1)
        throw EImproperCall("reserved items number Count chould be one almost");

    Items[p_ilast] = item;
}

//---------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//construye un array deslizante
template <class T> TSlideArray<T>::TSlideArray(int Capacity, int Count) :
        p_ifirst(0)
{
    //el número de elementos Capacity debería ser no menor que uno
    if(Capacity < 1)
        throw EImproperArgument("items number Capacity should be not less one");

    //el número de elementos reservados Count debería estar en [0, Capacity]
    if(Count<0 || Capacity<Count)
        throw EImproperArgument("reserved items number Count should be in [0, Capacity]");

    //reserva los elementos
    Items = new T[Capacity];
    p_Capacity = Capacity;
    p_Count = Count;

    //inicializa ilast
    if(Count)
        p_ilast = Count - 1;
    else
        p_ilast = Capacity - 1;
}

//construye un clon de un array deslizante
template <class T> TSlideArray<T>::TSlideArray(TSlideArray &SA)
{
    //copia las propiedades
    p_Capacity = SA.p_Capacity;
    p_Count = SA.p_Count;
    p_ifirst = SA.p_ifirst;
    p_ilast = SA.p_ilast;

    //reserva el array
    Items = new T[p_Capacity];

    //copia los elementos
    for(int i=0; i<p_Capacity; i++)
        Items[i] = SA.Items[i];
}

//---------------------------------------------------------------------------
//ACCESO A ELEMENTOS:

//lee el elemento reservado indicado
template <class T> inline const T &TSlideArray<T>::Get(int i) const
{
    //el índice i debe indicar un elemento reservado del array deslizante
    if(i<0 || p_Count<=i)
        throw EImproperArgument("index i should indicate an reserved item in slide array");

    //determina la posición del elemento
    i += p_ifirst;
    if(i >= p_Capacity)
        i -= p_Capacity;

    return Items[i];
}
//escribe el elemento reservado indicado
template <class T> inline void TSlideArray<T>::Set(int i, const T &item)
{
    //el índice i debe indicar un elemento reservado del array deslizante
    if(i<0 || p_Count<=i)
        throw EImproperArgument("index i should indicate an reserved item in slide array");

    //determina la posición del elemento
    i += p_ifirst;
    if(i >= p_Capacity)
        i -= p_Capacity;

    Items[i] = item;
}

//accede al elemento reservado indicado para escritura
template <class T> inline T& TSlideArray<T>::operator[](int i)
{
    //el índice i debe indicar un elemento reservado del array deslizante
    if(i<0 || p_Count<=i)
        throw EImproperArgument("index i should indicate an reserved item in slide array");

    //determina la posición del elemento
    i += p_ifirst;
    if(i >= p_Capacity)
        i -= p_Capacity;

    return Items[i];
}
//accede al elemento reservado indicado para lectura
template <class T> inline const T& TSlideArray<T>::operator[](int i) const
{
    //el índice i debe indicar un elemento reservado del array deslizante
    if(i<0 || p_Count<=i)
        throw EImproperArgument("index i should indicate an reserved item in slide array");

    //determina la posición del elemento
    i += p_ifirst;
    if(i >= p_Capacity)
        i -= p_Capacity;

    return Items[i];
}

//---------------------------------------------------------------------------
//DIMENSIONAMIENTO Y DESLIZAMEINTO:

//añade N elementos reservados por el principio
template <class T> inline void TSlideArray<T>::NewFirst(int N)
{
    //el número de elementos a añadir N debería ser no menor que uno
    if(N < 1)
        throw EImproperArgument("number items to add N should be not less one");

    //si se desborda la capacidad redimensiona
    int Count = p_Count + N;
    if(Count > p_Capacity)
        setCapacity(2*Count);

    //reserva los elementos
    p_Count = Count;

    //despalza ifirst N elementos a la izquierda
    p_ifirst -= N;
    if(p_ifirst < 0)
        p_ifirst += p_Capacity;
}
//borra N elementos reservados por el principio
template <class T> inline void TSlideArray<T>::DelFirst(int N)
{
    //el número de elementos a borrar N no debería ser mayor que el número de elementos de la array Count
    if(N > p_Count)
        throw EImproperArgument("number items to delete N should not be upper array items number Count");

    //avanza ifirst n pasos
    p_ifirst += N;
    if(p_ifirst >= p_Capacity)
        p_ifirst -= p_Capacity;

    //descuenta los elementos
    p_Count -= N;
}

//añade N elementos reservados por el final
template <class T> inline void TSlideArray<T>::NewLast(int N)
{
    //el número de elementos a añadir N debería ser no menor que uno
    if(N < 1)
        throw EImproperArgument("number items to add N should be not less one");

    //si se desborda la capacidad redimensiona
    int NewCount = p_Count + N;
    if(NewCount > p_Capacity)
        setCapacity(2*NewCount);

    //reserva los elementos
    p_Count = NewCount;

    //desplaza ilast N elementos a la derecha
    p_ilast += N;
    if(p_ilast >= p_Capacity)
        p_ilast -= p_Capacity;
}
//borra N elementos reservados por el final
template <class T> inline void TSlideArray<T>::DelLast(int N)
{
    //el número de elementos a borrar N no debería ser mayor que el número de elementos de la array Count
    if(N > p_Count)
        throw EImproperArgument("number items to delete N should not be upper array items number Count");

    //retrocede ilast n pasos
    p_ilast -= N;
    if(p_ilast < 0)
        p_ilast += p_Capacity;

    //descuenta los elementos
    p_Count -= N;
}

//desliza la ventana de reserva N posiciones a la izquierda
template <class T> inline void TSlideArray<T>::SlideLeft(int N)
{
    //debe desplazar almenos una posición
    if(N < 1)
        throw EImproperArgument("n < 1");

    //desplaza ifirst a la izda
    p_ifirst -= N;
    if(p_ifirst < 0)
        p_ifirst += (p_Capacity - 1 - p_ifirst)/p_Capacity*p_Capacity;

    //desplaza ilast a la izda
    p_ilast -= N;
    if(p_ilast < 0)
        p_ilast += (p_Capacity - 1 - p_ilast)/p_Capacity*p_Capacity;
}
//desliza la ventana de reserva N posiciones a la derecha
template <class T> inline void TSlideArray<T>::SlideRight(int N)
{
    //debe desplazar almenos una posición
    if(N < 1)
        throw EImproperArgument("n < 1");

    //desplaza ifirst a la dcha
    p_ifirst += N;
    if(p_ifirst >= p_Capacity)
        p_ifirst %= p_Capacity;

    //desplaza ilast a la dcha
    p_ilast += N;
    if(p_ilast >= p_Capacity)
        p_ilast %= p_Capacity;
}

//añade un elemento por el final
template <class T> void TSlideArray<T>::AddLast(const T &item)
{
    //reserva un elemento por el final
    NewLast();
    //asigna el elemento
    Set(p_Count-1, item);
}
//añade un elemento por el principio
template <class T> void TSlideArray<T>::AddFirst(const T &item)
{
    //reserva un elemento por el principio
    NewFirst();
    //apunta el elemento
    Set(0, item);
}
//inserta un elemento en la posición indicada
template <class T> void TSlideArray<T>::Insert(int i, const T &item)
{
    //el índice i debería apuntar a una posición en [0, Count]
    if(i<0 || p_Count<i)
        throw EImproperArgument("index i should indicate a position in [0, Count]");

    //NOTA: cuando inserta debe poder insertar hasta en la posúltima posición.

    //si la posición está en la primera mitad
    if(i <= p_Count/2) {
        //reserva un elemento por el principio
        NewFirst();
        //Desplaza los elementos
        for(int j=0; j<i; j++)
            Set(j, Get(j+1));
    } else { //el elemento está en la segunda mitad
        //reserva un elemento por el final
        NewLast();
        //Desplaza los elementos
        for(int j=p_Count-1; j>i; j--)
            Set(j, Get(j-1));
    }

    //asigna el elemento
    Set(i, item);
}

//--------------------------------------------------------------------------
//COPIADO:

//copia todas las propiedades del array incluidos sus elementos
template <class T> void TSlideArray<T>::Clone(const TSlideArray& SA)
{
    //cambia la longitud del array
    if(p_Capacity != SA.p_Capacity) {
        delete Items;
        Items = new T[SA.p_Capacity];
    }

    //copia las propiedades
    p_Capacity = SA.p_Capacity;
    p_Count = SA.p_Count;
    p_ifirst = SA.p_ifirst;
    p_ilast = SA.p_ilast;

    //copia los elementos
    for(int i=0; i<p_Capacity; i++)
        Items[i] = SA.Items[i];
}
//equipara el número de elementos reservados del array y los copia
template <class T> void TSlideArray<T>::Copy(const TSlideArray& SA)
{
    //equipara el número de elementos reservados
    if(p_Count < SA.p_Count)
            DelLast(SA.p_Count - p_Count);
    else if(p_Count > SA.p_Count)
            NewLast(p_Count - SA.p_Count);

    //copia los elementos reservados
    for(int i=0; i<p_Count; i++)
        Set(i, SA[i]);
}

//---------------------------------------------------------------------------

} //namespace Lists

//---------------------------------------------------------------------------
#endif // SLIDEARRAY_H

