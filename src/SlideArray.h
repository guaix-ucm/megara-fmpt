//---------------------------------------------------------------------------
//Archivo: SlideArray.h
//Contenido: lista de elementos genéricos
//Última actualización: 23/05/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef SLIDEARRAY_H
#define SLIDEARRAY_H

#include "Exceptions.h"

//---------------------------------------------------------------------------

//espacio de nombres de funciones y clases de objetos
//relacionados con listas, vectores y matrices
namespace Lists {

//---------------------------------------------------------------------------
//clase TSlideArray
//---------------------------------------------------------------------------

//TslideArray define arrays deslizantes
//autoredimensionables de Count elementos tipo T. Los arrays deslizantes
//permiten la adición, borrado y apilado de elementos en los límites de
//un array de una forma rápida y eficiente. Además reduce a la mitad los
//tiempos de inserción y borrado.

//clase array deslizante
template <class T> class TSlideArray {
protected:
    int Capacity;
    int Count;

    int ifirst; //índice al primer elemento reservado
    int ilast; //índice al último elemento reservado

public:
    T *Items; //aray de elementos

    //número de elementos del array
    int getCapacity(void) const {return Capacity;}
    void setCapacity(int);
    //número de elementos reservados del array
    int getCount(void) const {return Count;}

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
    TSlideArray(int _Capacity=8, int _Count=0);
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

template <class T> void TSlideArray<T>::setCapacity(int _Capacity)
{
    //el número de elementos Capacity no debe ser menor que el número de elementos reservados Count
    if(_Capacity < Count)
        throw EImproperArgument("items number Capacity should not be less reserved items number Count");

    //construye un nuevo array
    T *NewItems = new T[_Capacity];

    //copia los elementos en el nuevo array
    if(Count) {
        int i=ifirst; //índice para Items
        int j=0; //índice para NewItems
        if(ifirst <= ilast)
            for(; i<=ilast; i++)
                NewItems[j++] = Items[i];
        else {
            for(; i<Capacity; i++)
                NewItems[j++] = Items[i];
            for(i=0; i<=ilast; i++)
                NewItems[j++] = Items[i];
        }
    }

    //apunta el nuevo array
    delete Items;
    Items = NewItems;
    Capacity = _Capacity;

    //actualiza los límites de la ventana de reserva
    ifirst = 0;
    if(Count)
        ilast = Count - 1;
    else
        ilast = Capacity - 1;
}

template <class T> inline const T &TSlideArray<T>::getFirst(void) const
{
    //el número de elementos reservados Count debería ser al menos uno
    if(Count < 1)
        throw EImproperCall("reserved items number Count chould be one almost");

    return Items[ifirst];
}
template <class T> inline void TSlideArray<T>::setFirst(T &item)
{
    //el número de elementos reservados Count debería ser al menos uno
    if(Count < 1)
        throw EImproperCall("reserved items number Count chould be one almost");

    Items[ifirst] = item;
}

template <class T> inline const T &TSlideArray<T>::getLast(void) const
{
    //el número de elementos reservados Count debería ser al menos uno
    if(Count < 1)
        throw EImproperCall("reserved items number Count chould be one almost");

    return Items[ilast];
}
template <class T> inline void TSlideArray<T>::setLast(T &item)
{
    //el número de elementos reservados Count debería ser al menos uno
    if(Count < 1)
        throw EImproperCall("reserved items number Count chould be one almost");

    Items[ilast] = item;
}

//---------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//construye un array deslizante
template <class T> TSlideArray<T>::TSlideArray(int _Capacity, int _Count) :
        ifirst(0)
{
    //el número de elementos Capacity debería ser no menor que uno
    if(_Capacity < 1)
        throw EImproperArgument("items number Capacity should be not less one");

    //el número de elementos reservados Count debería estar en [0, Capacity]
    if(_Count<0 || _Capacity<_Count)
        throw EImproperArgument("reserved items number Count should be in [0, Capacity]");

    //reserva los elementos
    Items = new T[_Capacity];
    Capacity = _Capacity;
    Count = _Count;

    //inicializa ilast
    if(_Count)
        ilast = _Count - 1;
    else
        ilast = _Capacity - 1;
}

//construye un clon de un array deslizante
template <class T> TSlideArray<T>::TSlideArray(TSlideArray &SA)
{
    //copia las propiedades
    Capacity = SA.Capacity;
    Count = SA.Count;
    ifirst = SA.ifirst;
    ilast = SA.ilast;

    //reserva el array
    Items = new T[Capacity];

    //copia los elementos
    for(int i=0; i<Capacity; i++)
        Items[i] = SA.Items[i];
}

//---------------------------------------------------------------------------
//ACCESO A ELEMENTOS:

//lee el elemento reservado indicado
template <class T> inline const T &TSlideArray<T>::Get(int i) const
{
    //el índice i debe indicar un elemento reservado del array deslizante
    if(i<0 || Count<=i)
        throw EImproperArgument("index i should indicate an reserved item in slide array");

    //determina la posición del elemento
    i += ifirst;
    if(i >= Capacity)
        i -= Capacity;

    return Items[i];
}
//escribe el elemento reservado indicado
template <class T> inline void TSlideArray<T>::Set(int i, const T &item)
{
    //el índice i debe indicar un elemento reservado del array deslizante
    if(i<0 || Count<=i)
        throw EImproperArgument("index i should indicate an reserved item in slide array");

    //determina la posición del elemento
    i += ifirst;
    if(i >= Capacity)
        i -= Capacity;

    Items[i] = item;
}

//accede al elemento reservado indicado para escritura
template <class T> inline T& TSlideArray<T>::operator[](int i)
{
    //el índice i debe indicar un elemento reservado del array deslizante
    if(i<0 || Count<=i)
        throw EImproperArgument("index i should indicate an reserved item in slide array");

    //determina la posición del elemento
    i += ifirst;
    if(i >= Capacity)
        i -= Capacity;

    return Items[i];
}
//accede al elemento reservado indicado para lectura
template <class T> inline const T& TSlideArray<T>::operator[](int i) const
{
    //el índice i debe indicar un elemento reservado del array deslizante
    if(i<0 || Count<=i)
        throw EImproperArgument("index i should indicate an reserved item in slide array");

    //determina la posición del elemento
    i += ifirst;
    if(i >= Capacity)
        i -= Capacity;

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
    int _Count = Count + N;
    if(_Count > Capacity)
        setCapacity(2*_Count);

    //reserva los elementos
    Count = _Count;

    //despalza ifirst N elementos a la izquierda
    ifirst -= N;
    if(ifirst < 0)
        ifirst += Capacity;
}
//borra N elementos reservados por el principio
template <class T> inline void TSlideArray<T>::DelFirst(int N)
{
    //el número de elementos a borrar N no debería ser mayor que el número de elementos de la array Count
    if(N > Count)
        throw EImproperArgument("number items to delete N should not be upper array items number Count");

    //avanza ifirst n pasos
    ifirst += N;
    if(ifirst >= Capacity)
        ifirst -= Capacity;

    //descuenta los elementos
    Count -= N;
}

//añade N elementos reservados por el final
template <class T> inline void TSlideArray<T>::NewLast(int N)
{
    //el número de elementos a añadir N debería ser no menor que uno
    if(N < 1)
        throw EImproperArgument("number items to add N should be not less one");

    //si se desborda la capacidad redimensiona
    int NewCount = Count + N;
    if(NewCount > Capacity)
        setCapacity(2*NewCount);

    //reserva los elementos
    Count = NewCount;

    //desplaza ilast N elementos a la derecha
    ilast += N;
    if(ilast >= Capacity)
        ilast -= Capacity;
}
//borra N elementos reservados por el final
template <class T> inline void TSlideArray<T>::DelLast(int N)
{
    //el número de elementos a borrar N no debería ser mayor que el número de elementos de la array Count
    if(N > Count)
        throw EImproperArgument("number items to delete N should not be upper array items number Count");

    //retrocede ilast n pasos
    ilast -= N;
    if(ilast < 0)
        ilast += Capacity;

    //descuenta los elementos
    Count -= N;
}

//desliza la ventana de reserva N posiciones a la izquierda
template <class T> inline void TSlideArray<T>::SlideLeft(int N)
{
    //debe desplazar almenos una posición
    if(N < 1)
        throw EImproperArgument("n < 1");

    //desplaza ifirst a la izda
    ifirst -= N;
    if(ifirst < 0)
        ifirst += (Capacity - 1 - ifirst)/Capacity*Capacity;

    //desplaza ilast a la izda
    ilast -= N;
    if(ilast < 0)
        ilast += (Capacity - 1 - ilast)/Capacity*Capacity;
}
//desliza la ventana de reserva N posiciones a la derecha
template <class T> inline void TSlideArray<T>::SlideRight(int N)
{
    //debe desplazar almenos una posición
    if(N < 1)
        throw EImproperArgument("n < 1");

    //desplaza ifirst a la dcha
    ifirst += N;
    if(ifirst >= Capacity)
        ifirst %= Capacity;

    //desplaza ilast a la dcha
    ilast += N;
    if(ilast >= Capacity)
        ilast %= Capacity;
}

//añade un elemento por el final
template <class T> void TSlideArray<T>::AddLast(const T &item)
{
    //reserva un elemento por el final
    NewLast();
    //asigna el elemento
    Set(Count-1, item);
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
    if(i<0 || Count<i)
        throw EImproperArgument("index i should indicate a position in [0, Count]");

    //NOTA: cuando inserta debe poder insertar hasta en la posúltima posición.

    //si la posición está en la primera mitad
    if(i <= Count/2) {
        //reserva un elemento por el principio
        NewFirst();
        //Desplaza los elementos
        for(int j=0; j<i; j++)
            Set(j, Get(j+1));
    } else { //el elemento está en la segunda mitad
        //reserva un elemento por el final
        NewLast();
        //Desplaza los elementos
        for(int j=Count-1; j>i; j--)
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
    if(Capacity != SA.Capacity) {
        delete Items;
        Items = new T[SA.Capacity];
    }

    //copia las propiedades
    Capacity = SA.Capacity;
    Count = SA.Count;
    ifirst = SA.ifirst;
    ilast = SA.ilast;

    //copia los elementos
    for(int i=0; i<Capacity; i++)
        Items[i] = SA.Items[i];
}
//equipara el número de elementos reservados del array y los copia
template <class T> void TSlideArray<T>::Copy(const TSlideArray& SA)
{
    //equipara el número de elementos reservados
    if(Count < SA.Count)
            DelLast(SA.Count - Count);
    else if(Count > SA.Count)
            NewLast(Count - SA.Count);

    //copia los elementos reservados
    for(int i=0; i<Count; i++)
        Set(i, SA[i]);
}

//---------------------------------------------------------------------------

} //namespace Lists

//---------------------------------------------------------------------------
#endif // SLIDEARRAY_H

