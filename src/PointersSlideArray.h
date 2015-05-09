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
//Archivo: PointersSlideArray.h
//Contenido: array deslizante de punteros
//Última actualización: 27/06/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef POINTERSSLIDEARRAY_H
#define POINTERSSLIDEARRAY_H

#include "Exceptions.h"

//---------------------------------------------------------------------------

//espacio de nombres de funciones y clases de objetos
//relacionados con listas, vectores y matrices
namespace Lists {

//---------------------------------------------------------------------------
//clase TPointersSlideArray
//---------------------------------------------------------------------------

//TslideArray define arrays deslizantes
//autoredimensionables de Count elementos tipo T. Los arrays deslizantes
//permiten la adición, borrado y apilado de elementos en los límites de
//un array de una forma rápida y eficiente. Además reduce a la mitad los
//tiempos de inserción y borrado.

//clase array deslizante de punteros
template <class T> class TPointersSlideArray {
protected:
    int p_Capacity;
    int p_Count;

    int p_ifirst; //índice al primer elemento reservado
    int p_ilast; //índice al último elemento reservado

public:
    T **Items; //aray de punteros a elementos

    //número de elementos reservados del array
    int getCapacity(void) const {return p_Capacity;}
    void setCapacity(int);
    //número de elementos del array
    int getCount(void) const {return p_Count;}

    //accede al primer elemento
    //      First = Items[ifirs]
    inline T *getFirst(void) const;
    inline void setFirst(T*);
    //accede al último elemento
    //      Last = Items[ilast]
    inline T *getLast(void) const;
    inline void setLast(T*);

    //MÉTODOS PÚBLICOS:

    //construye un array deslizante
    TPointersSlideArray(int Capacity=8, int Count=0);
    //cotruye un clon de un array deslizante
    TPointersSlideArray(TPointersSlideArray<T> &SA);
    //libera la memoria ocupada por el array deslizante
    ~TPointersSlideArray();

    //determina si una posición indicada del array está reservada
    bool PositionIsReserved(int i);

    //ACCESO A ELEMENTOS (MEDIANTE EL PUNTERO):

    //lee el puntero indicado
    inline T *Get(int i);
    //lee el puntero indicado
    inline const T *Get(int i) const;
    //escribe el puntero indicado
    inline void Set(int i, const T*);

    //accede al elemento indicado en arrays no constantes
    inline T *&operator[](int i);
    //accede al elemento indicado en arrays constantes
    inline const T *&operator[](int i) const;

    //NOTA: si la lista es constante, se invocará a la función constante
    //que devuelve una referencia a constante impidiendo su modificación.

    //RESERVA Y LIBERACIÓN DE PUNTEROS:

    //reserva N punteros por el principio
    inline void ResFirst(int N=1);
    //libera N punteros por el principio
    inline void LibFirst(int N=1);

    //reserva N punteros por el final
    inline void ResLast(int N=1);
    //libera N punteros por el final
    inline void LibLast(int N=1);

    //CONSTRUCCIÓN Y DESTRUCCION DE ELEMENTOS:

    //añade N elementos por el principio
    inline void NewFirst(int N=1);
    //borra N elementos por el principio
    inline void DelFirst(int N=1);

    //añade N elementos por el final
    inline void NewLast(int N=1);
    //borra N elementos por el final
    inline void DelLast(int N=1);

    //DESLIZAMIENTO:

    //desliza el array N posiciones a la izquierda
    inline void SlideLeft(int N=1);
    //desliza el array N posiciones a la derecha
    inline void SlideRight(int N=1);

    //ADICIÓN E INSERCIÓN:

    //añade un elemento por el final
    void AddLast(const T *item);
    //añade un elemento por el principio
    void AddFirst(const T *item);
    //inserta un elemento en la posición indicada
    void Insert(int i, const T *item);

    //Modo de uso de los métodos:
    //      SlideLeft;
    //      SlideRight.
    //En rotación:
    //      apuntar los elemento que se van a desbordar
    //      deslizar el array de putneros
    //      apuntar los elementos en los nuevos puntneros
    //Enpilas FIFO:
    //      es posible reciclar los eleemntos desbordados o destruirlos,
    //      deslizar el array y apuntar otros nuevos.

    //Este modo de uso deja a responsabilidad del programador
    //la manipulación de los punteros del array. Lo cual es aceptable
    //porque esta clase ha sido definida para incluir un objeto
    //dentro de la clase TPointersList.

    //COPIADO:

    //equipara todas las propiedades del array y
    //copia los elementos de un array deslizante
    void Clone(const TPointersSlideArray&);
    //equipara el número de elementos del array y
    //copia los elementos de un array deslizante
    void Copy(const TPointersSlideArray&);
};

//---------------------------------------------------------------------------
//Métodos de la clase TPointersSlideArray:
//---------------------------------------------------------------------------

template <class T> void TPointersSlideArray<T>::setCapacity(int Capacity)
{
    //el número de elementos reservados Capacity no debe ser menor que el número de elementos Count
    if(Capacity < getCount())
        throw EImproperArgument("reserved items number Capacity should not be less number items Count");

    //reserva un nuevo array
    T **NewItems = new T*[Capacity];

    //copia los elementos en el nuevo array
    if(getCount() > 0) {
        int i=p_ifirst; //índice para Items
        int j=0; //índice para NewItems
        if(p_ifirst <= p_ilast)
            for(; i<=p_ilast; i++)
                NewItems[j++] = Items[i];
        else {
            for(; i<getCapacity(); i++)
                NewItems[j++] = Items[i];
            for(i=0; i<=p_ilast; i++)
                NewItems[j++] = Items[i];
        }
    }

    //apunta el nuevo array
    delete Items;
    Items = NewItems;
    p_Capacity = Capacity;

    p_ifirst = 0;
    if(getCount() > 0)
        p_ilast = getCount() - 1;
    else
        p_ilast = getCapacity() - 1;
}

template <class T> inline T *TPointersSlideArray<T>::getFirst(void) const
{
    //el número de elementos del array Count debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("array items number Count chould be one almost");

    return Items[p_ifirst];
}
template <class T> inline void TPointersSlideArray<T>::setFirst(T *item)
{
    //el número de elementos del array Count debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("array items number Count chould be one almost");

    Items[p_ifirst] = item;
}

template <class T> inline T *TPointersSlideArray<T>::getLast(void) const
{
    //el número de elementos del array Count debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("array items number Count chould be one almost");

    return Items[p_ilast];
}
template <class T> inline void TPointersSlideArray<T>::setLast(T *item)
{
    //el número de elementos del array Count debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("array items number Count chould be one almost");

    Items[p_ilast] = item;
}

//---------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//construye un array deslizante
template <class T> TPointersSlideArray<T>::TPointersSlideArray(int Capacity, int Count) :
    p_ifirst(0)
{
    //el número de elementos reservados Capacity debería ser no menor que uno
    if(Capacity < 1)
        throw EImproperArgument("reserved items number should be not less one");

    //el número de elementos del array Count debería estar en [0, Capacity]
    if(Count<0 || Capacity<Count)
        throw EImproperArgument("number items Count should be in [0, Capacity]");

    //reserva los elementos
    Items = new T*[Capacity];
    p_Capacity = Capacity;
    p_Count = Count;

    //si hay algún elemento
    if(Count > 0) {
        //inicializa ilast
        p_ilast = Count - 1;
        //contruye los elementos
        for(int i=0; i<getCount(); i++)
            Items[i] = new T;
    } else
        //inicializa ilast
        p_ilast = Capacity - 1;
}

//cotruye un clon de un array deslizante
template <class T> TPointersSlideArray<T>::TPointersSlideArray(TPointersSlideArray<T> &SA)
{
    //copia las propiedades
    p_Capacity = SA.p_Capacity;
    p_Count = SA.p_Count;
    p_ifirst = SA.p_ifirst;
    p_ilast = SA.p_ilast;

    //reserva el array
    Items = new T*[getCapacity()];

    //CLONA LOS OBJETOS APUNTADOS:

    T *item;
    //por cada posición del array
    for(int i=0; i<getCapacity(); i++) {
        //si la posición está reservada
        if(PositionIsReserved(i)) {
            //clona el objeto apuntado
            item = new T(SA.Items[i]);
            //aputa el objeto en el array
            Items[i] = item;
        } else
            //copia el puntero
            Items[i] = SA.Items[i];
    }
}

//libera la memoria ocupada por el array deslizante
template <class T> TPointersSlideArray<T>::~TPointersSlideArray()
{
    //NOTA: se supone que todos los punteros apuntan a objetos construidos
    //porque la clase no deja añadir punteros nulos.

    //destruye los elementos apuntados
    for(int i=0; i<getCount(); i++) {
        T *item = Get(i);
        if(item != NULL)
            delete item;
    }

    //destruye el array de punteros
    delete Items;
}

//determina si una posición indicada del array está reservada
template <class T> bool TPointersSlideArray<T>::PositionIsReserved(int i)
{
    //el índice i debe indicar una posición del array
    if(i<0 || getCapacity()<=i)
        throw EImproperArgument("index i should indicate a position in the array");

    //si no hay elementos
    if(getCount() <= 0)
        return false; //la posición indicada no está reservada

    //si el intervalo de reserva no está partido
    if(p_ifirst <= p_ilast)
        if(i<p_ifirst || p_ilast<i) //si está fuera del intervalo
            return false; //la posición no está reservada
        else //si está dentro del intervalo
            return true; //la posición está reservada
    else //si el intervalo está partido
        if(i<=p_ilast || p_ifirst<=i) //si está dentro de alguna de las partes
            return true; //la posición está reservada
        else //si está fuera de las partes
            return false; //la posición no está reservada
}

//---------------------------------------------------------------------------
//ACCESO A ELEMENTOS (MEDIANTE EL PUNTERO):

//lee el puntero indicado
template <class T> inline T *TPointersSlideArray<T>::Get(int i)
{
    //el índice i debe indicar un elemento del array deslizante
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i should indicate an element in slide array");

    //determina la posición del elemento
    i += p_ifirst;
    if(i >= getCapacity())
        i -= getCapacity();

    return Items[i];
}
//lee el puntero indicado
template <class T> inline const T *TPointersSlideArray<T>::Get(int i) const
{
    //el índice i debe indicar un elemento del array deslizante
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i should indicate an element in slide array");

    //determina la posición del elemento
    i += p_ifirst;
    if(i >= getCapacity())
        i -= getCapacity();

    return Items[i];
}
//escribe el puntero indicado
template <class T> inline void TPointersSlideArray<T>::Set(int i, const T *item)
{
    //el índice i debe indicar un elemento del array deslizante
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i should indicate an element in slide array");

    //determina la posición del elemento
    i += p_ifirst;
    if(i >= getCapacity())
        i -= getCapacity();

    Items[i] = (T*)item;
}

//accede al elemento indicado para escritura
template <class T> inline T *&TPointersSlideArray<T>::operator[](int i)
{
    //el índice i debe indicar un elemento del array deslizante
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i should indicate an element in slide array");

    //determina la posición del elemento
    i += p_ifirst;
    if(i >= getCapacity())
        i -= getCapacity();

    return Items[i];
}
//accede al elemento indicado para lectura
template <class T> inline const T *&TPointersSlideArray<T>::operator[](int i) const
{
    //el índice i debe indicar un elemento del array deslizante
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i should indicate an element in slide array");

    //determina la posición del elemento
    i += p_ifirst;
    if(i >= getCapacity())
        i -= getCapacity();

    return (const T *&)Items[i];
}

//---------------------------------------------------------------------------
//RESERVA Y IBERACIÓN DE PUNTEROS:

//reserva N punteros por el principio
template <class T> inline void TPointersSlideArray<T>::ResFirst(int N)
{
    //el número de elementos a añadir N debería ser no menor que uno
    if(N < 1)
        throw EImproperArgument("number items to add N should be not less one");

    //si se desborda la capacidad redimensiona
    int Count = getCount() + N;
    if(Count > getCapacity())
        setCapacity(2*Count);

    //reserva los elementos
    p_Count = Count;

    //despalza ifirst N elementos a la izquierda
    p_ifirst -= N;
    if(p_ifirst < 0)
        p_ifirst += getCapacity();
}
//libera N punteros por el principio
template <class T> inline void TPointersSlideArray<T>::LibFirst(int N)
{
    //el número de elementos a liberar N no debería ser mayor que el número de elementos de la array Count
    if(N > getCount())
        throw EImproperArgument("number items to liberate N should not be upper array items number Count");

    //avanza ifirst n pasos
    p_ifirst += N;
    if(p_ifirst >= getCapacity())
        p_ifirst -= getCapacity();

    //descuenta los elementos
    p_Count -= N;
}

//reserva N punteros por el final
template <class T> inline void TPointersSlideArray<T>::ResLast(int N)
{
    //el número de elementos a añadir N debería ser no menor que uno
    if(N < 1)
        throw EImproperArgument("number items to add N should be not less one");

    //si se desborda la capacidad redimensiona
    int NewCount = getCount() + N;
    if(NewCount > getCapacity())
        setCapacity(2*NewCount);

    //reserva los elementos
    p_Count = NewCount;

    //desplaza ilast N elementos a la derecha
    p_ilast += N;
    if(p_ilast >= getCapacity())
        p_ilast -= getCapacity();
}
//libera N punteros por el final
template <class T> inline void TPointersSlideArray<T>::LibLast(int N)
{
    //el número de elementos a liberar N no debería ser mayor que el número de elementos de la array Count
    if(N > getCount())
        throw EImproperArgument("number items to liberate N should not be upper array items number Count");

    //retrocede ilast n pasos
    p_ilast -= N;
    if(p_ilast < 0)
        p_ilast += getCapacity();

    //descuenta los elementos
    p_Count -= N;
}

//---------------------------------------------------------------------------
//CONSTRUCCION Y DESTRUCCION DE ELEMENTOS:

//añade N elementos por el principio
template <class T> inline void TPointersSlideArray<T>::NewFirst(int N)
{
    //reserva N punteros por el principio
    try {
        ResFirst(N);
    } catch(...) {
        throw;
    }

    //construye y apunta los nuevos elementos
    for(int i=0; i<N; i++)
        Set(i, new T);
}
//borra N elementos por el principio
template <class T> inline void TPointersSlideArray<T>::DelFirst(int N)
{
    //el número de elementos a borrar N no debería ser mayor que el número de elementos de la array Count
    if(N > getCount())
        throw EImproperArgument("number items to delete N should not be upper array items number Count");

    //destruye los elementos apuntados
    for(int i=0; i<N; i++)
        delete Get(i);

    //libera los punteros
    LibFirst(N);
}

//añade N elementos por el final
template <class T> inline void TPointersSlideArray<T>::NewLast(int N)
{
    //reserva N punteros por el final
    try {
        ResLast(N);
    } catch(...) {
        throw;
    }

    //construye y apunta los nuevos elementos
    for(int i=getCount()-N; i<getCount(); i++)
        Set(i, new T);
}
//borra N elementos por el final
template <class T> inline void TPointersSlideArray<T>::DelLast(int N)
{
    //el número de elementos a borrar N no debería ser mayor que el número de elementos de la array Count
    if(N > getCount())
        throw EImproperArgument("number items to delete N should not be upper array items number Count");

    //destruye los elementos apuntados
    for(int i=getCount()-N; i<getCount(); i++)
        delete Get(i);

    //libera los punteros
    LibLast(N);
}

//--------------------------------------------------------------------------
//DESLIZAMIENTO:

//desliza el array N posiciones a la izquierda
template <class T> inline void TPointersSlideArray<T>::SlideLeft(int N)
{
    //debe desplazar almenos una posición
    if(N < 1)
        throw EImproperArgument("n < 1");

    //desplaza ifirst a la izda
    p_ifirst -= N;
    if(p_ifirst < 0)
        p_ifirst += (getCapacity() - 1 - p_ifirst)/getCapacity()*getCapacity();

    //desplaza ilast a la izda
    p_ilast -= N;
    if(p_ilast < 0)
        p_ilast += (getCapacity() - 1 - p_ilast)/getCapacity()*getCapacity();
}
//desliza el array N posiciones a la derecha
template <class T> inline void TPointersSlideArray<T>::SlideRight(int N)
{
    //debe desplazar almenos una posición
    if(N < 1)
        throw EImproperArgument("n < 1");

    //desplaza ifirst a la dcha
    p_ifirst += N;
    if(p_ifirst >= getCapacity())
        p_ifirst %= getCapacity();

    //desplaza ilast a la dcha
    p_ilast += N;
    if(p_ilast >= getCapacity())
        p_ilast %= getCapacity();
}

//--------------------------------------------------------------------------
//ADICIÓN E INSERCIÓN DE ELEMENTOS:

//añade un elemento por el final
template <class T> void TPointersSlideArray<T>::AddLast(const T *item)
{
    //el puntero item debería apuntar a un elemento contruido
    if(item == NULL)
        throw EImproperArgument("pointer item should point to built item");

    //reserva un puntero por el final
    ResLast();
    //apunta el elemento
    Set(getCount()-1, item);
}
//añade un elemento por el principio
template <class T> void TPointersSlideArray<T>::AddFirst(const T *item)
{
    //el puntero item debería apuntar a un elemento contruido
    if(item == NULL)
        throw EImproperArgument("pointer item should point to built item");

    //reserva un puntero por el principio
    ResFirst();
    //apunta el elemento
    Set(0, item);
}
//inserta un elemento en la posición indicada
template <class T> void TPointersSlideArray<T>::Insert(int i, const T *item)
{
    //el índice i debería apuntar a una posición en [0, Count]
    if(i<0 || getCount()<i)
        throw EImproperArgument("index i should indicate a position in [0, Count]");

    //NOTA: cuando inserta debe poder insertar hasta en la posúltima posición.

    //el puntero item debería apuntar a un elemento construido
    if(item == NULL)
        throw EImproperArgument("pointer item should point to built item");

    //si la posición está en la primera mitad
    if(i <= getCount()/2) {
        //reserva un puntero por el principio
        ResFirst();
        //Desplaza los elementos
        for(int j=0; j<i; j++)
            Set(j, Get(j+1));
    } else { //el elemento está en la segunda mitad
        //reserva un puntero por el final
        ResLast();
        //Desplaza los elementos
        for(int j=getCount()-1; j>i; j--)
            Set(j, Get(j-1));
    }

    //apunta el elemento
    Set(i, item);
}

//--------------------------------------------------------------------------
//COPIADO:

//copia todas las propiedades del array incluidos sus elementos
template <class T> void TPointersSlideArray<T>::Clone(const TPointersSlideArray &SA)
{
    //destruye los elementos del array
    T *item;
    for(int i=0; i<getCount(); i++)
        delete Get(i);

    //redimensiona el array en caso necesario
    if(getCapacity() != SA.getCapacity()) {
        delete Items;
        Items = new T*[SA.getCapacity()];
    }

    //copia las propiedades
    p_Capacity = SA.p_Capacity;
    p_Count = SA.p_Count;
    p_ifirst = SA.p_ifirst;
    p_ilast = SA.p_ilast;

    //CLONA LOS OBJETOS APUNTADOS:

    //por cada posición del array
    for(int i=0; i<getCapacity(); i++) {
        //si la posición está reservada
        if(PositionIsReserved(i)) {
            T *aux = SA.Items[i];
            //clona el objeto apuntado
            item = new T(aux);

            //ADVERTENCIA: al constructor de copia dela clase T
            //debe pasarseel objetoy no un puntero al objeto,
            //ya que hay algunas clases de objetos que tienen
            //constructor de copia porreferencia pero no por
            //puntero. Ese es  el caso de AnsiString, p.e.

            //ERROR: lasentencia 'item = new T(*(SA.Items[i]));'
            //produjo el error difícil de detectar consistente
            //en que al no haber un constructor de la clase T
            //por referencia, el compilador estaba adoptando
            //la clase TItemList<TFiberPositioner> como la clase T,
            //en lugar de la clase TFiberPositioner. Detectar
            //este error mellevó todo el día (24/06/2013).

            //apunta el clon
            Items[i] = item;
        } else
            //copia el puntero idicado
            Items[i] = SA.Items[i];
    }
}
//equipara el número de elementos reservados de la array y
//copia los elementos de una array deslizante
template <class T> void TPointersSlideArray<T>::Copy(const TPointersSlideArray &SA)
{
    T *item;

    //equipara el número de elementos reservados
    //        Count = SA.Count;
    if(getCount() < SA.Count)
        DelLast(SA.getCount() - getCount());
    else if(getCount() > SA.getCount())
        NewLast(getCount() - SA.getCount());

    //por cada elemento de la ventana de reserva
    for(int i=0; i<getCount(); i++) {
        /*                //clona el objeto apuntado
                item = new T(SA[i]);
                //apunta el clon
                Set(i, item);*/
        //copia el objeto indicado
        *Get(i) = *(SA.Get(i));
    }
}

//---------------------------------------------------------------------------

} //namespace Lists

//---------------------------------------------------------------------------
#endif // POINTERSSLIDEARRAY_H
