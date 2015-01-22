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
//Archivo: PLVector.h
//Contenido: lista de punteros a vector
//Última actualización: 17/03/2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef PLVECTOR_H
#define PLVECTOR_H

#include "Vector.h"
#include "PointersList.h"

//---------------------------------------------------------------------------

//espacio de nombres de funciones y clases de objetos
//relacionados con listas, vectores y matrices
namespace Lists {

//---------------------------------------------------------------------------
//clase TPLVector
//---------------------------------------------------------------------------

//TPLVector: listas de punteros a vector

//clase lista de punteros a vector
template <class T> class TPLVector : public TPointersList<TVector<T> > {
protected:
public:
    //constructor general
    TPLVector(int _Capacity=8,
              int ( *_CompareVectors)(const TVector<T>*, const TVector<T>*)=NULL,
              double ( *_EvaluateVector)(const TVector<T>*)=NULL,
              void ( *_AssignValue)(TVector<T>*, double)=NULL) :
        TPointersList<TVector<T> >(_Capacity, _CompareVectors,
                                   _EvaluateVector, _AssignValue,
                                   TVector<T>::PrintVector, TVector<T>::ReadVector) {;}

    //suma del nº de elementos de cada vector
    int getTotalCount(void) const;

    //redimensiona el buffer de todas las filas
    void SetColCapacity(int);
    //redimensiona los buffers de la lista de listas
    void SetCapacitys(int RowCapacity, int ColCapacity);

    //redimensiona todas las filas
    void SetColCount(int);
    //redimensiona la lista de listas
    void SetCounts(int RowCount, int ColCount);

    //redimensiona la lista de listas
    //inicializando las nuevas variables a cero
    void SetDimensions(int RowCount, int ColCount);
    //flag indicador de que todas las filas tienen el mismo nº de elementos
    bool getEqualDimensions(void) const;

    //añade cada elemento de un vector al final de cad alista
    void AddColumn(TVector<T> &v);

    //copia los vectores de una PLV
    TPLVector<T>& operator=(const TPLVector<T>&);

    //asigna el mismo valor a todos los elementos
    void SetAll(T x);
    //asigna a cada elemento una muestra de la v.a.--> U[xmin, xmax]
    void Random(T xmin, T xmax);
    /*
    //Compara los vectores omónimos de una PLV
    bool operator==(const TPLVector<T>&) const;
    bool operator!=(const TPLVector<T>&) const;*/
    //busca la primera ocurrencia de la componente. Si no lo encuentra
    //devuelve falso e (i, j)=(Count, Last->Count)
    bool Find(T item, int &i, int &j) const;

    //calcula el vector media
    void GetVectorAve(TVector<double> &v);
    //obtiene el vector mínima
    void GetVectorMin(TVector<double> &v);
    //obtiene el vector máxima
    void GetVectorMax(TVector<double> &v);
    //obtiene el vector varianza
    void GetVectorVar(TVector<double> &v);
/*
    //lista de vectores en formato texto con delimitadores
    //  '{', ',' y '}' para listas
    //  '(', ',' y ')' para vectores
    AnsiString getText(void) const;
    void setText(const AnsiString&);*/
};

//---------------------------------------------------------------------------
//MÉTODOS PROTEGIDOS:
//---------------------------------------------------------------------------
template <class T> int TPLVector<T>::getTotalCount(void) const
{
    int acum=0;

    for(int i=0; i<this->getCount(); i++)
        acum += this->Items[i]->getCount();

    return acum;
}

template <class T> bool TPLVector<T>::getEqualDimensions(void) const
{
    //el nº de componentes de una lista inexistente no tiene sentido
    if(this->getCount() < 1)
        throw EImproperCall("empty list");

    //si encuentra alguna lista de distinta longitud devuelve falso
    for(int i=1; i<this->getCount(); i++)
        if(this->Items[i]->getCount() != this->Items[0]->getCount())
            return false;

    return true;
}

//---------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//redimensiona el buffer de todas las filas
template <class T> void TPLVector<T>::SetColCapacity(int ColCapacity)
{
    //las dimensiones deben ser positivas
    if(ColCapacity < 0)
        throw EImproperArgument("negative ColCapacity");

    //redimensiona el buffer de todas las filas
    for(int i=0; i<this->getCount(); i++)
        this->Items[i]->setCapacity(ColCapacity);
}

//redimensiona los buffers de la lista de listas
template <class T> void TPLVector<T>::SetCapacitys(int RowCapacity,
                                                   int ColCapacity)
{
    //las dimensiones deben ser positivas
    if(RowCapacity<0 || ColCapacity<0)
        throw EImproperArgument("dimensions should be positives");

    //redimensiona el buffer de la lista de listas
    this->setCapacity(RowCapacity);

    //redimensiona el buffer de todas las filas
    for(int i=0; i<this->getCount(); i++)
        this->Items[i]->setCapacity(ColCapacity);
}

//redimensiona todas las filas
template <class T> void TPLVector<T>::SetColCount(int ColCount)
{
    //las dimensiones deben ser positivas
    if(ColCount < 0)
        throw EImproperArgument("dimensions should be positives");

    //redimensiona todas las filas
    for(int i=0; i<this->getCount(); i++)
        this->Items[i]->setCount(ColCount);
}

//redimensiona la lista de listas
template <class T> void TPLVector<T>::SetCounts(int RowCount, int ColCount)
{
    //las dimensiones deben ser positivas
    if(RowCount<0 || ColCount<0)
        throw EImproperArgument("dimensions should be positives");

    if(RowCount < this->getCount()) { //menos filas
        //libera las filas sobrantes
        this->setCount(RowCount);

        //redimensiona todas las filas restantes
        for(int i=0; i<this->getCount(); i++)
            this->Items[i]->setCount(ColCount);
    } else { //mas filas
        //redimensiona todas las filas
        int i;
        for(i=0; i<this->getCount(); i++)
            this->Items[i]->setCount(ColCount);
        //i queda indicando a la primera nueva fila

        //reserva y redimensiona las nuevas filas
        this->setCount(RowCount);
        for(i; i<this->getCount(); i++)
            this->Items[i]->setCount(ColCount);
    }
}

//redimensiona la lista de listas inicializando las nuevas variables a cero
template <class T> void TPLVector<T>::SetDimensions(int RowCount, int ColCount)
{
    //las dimensiones deben ser positivas
    if(RowCount<0 || ColCount<0)
        throw EImproperArgument("dimensions should be positives");

    if(RowCount < this->getCount()) { //menos filas
        //libera las filas sobrantes
        this->setCount(RowCount);

        //redimensiona las filas restantes
        //e inicializa a cero los nuevos parámetros
        for(int i=0; i<this->getCount(); i++) {
            //indica al primer nuevo parámetro
            int j = this->Items[i]->getCount();
            //redimensiona el vector indicado
            this->Items[i]->setCount(ColCount);
            //inicializa a cero los nuevos parámetros
            for(j; j<ColCount; j++)
                (*this->Items[i])[j] = 0;
        }
    } else { //mas o igual filas
        //redimensiona las viejas filas
        int i;
        for(i=0; i<this->getCount(); i++) {
            //indica al primer nuevo parámetro
            int j = this->Items[i]->getCount();
            //redimensiona el vector
            this->Items[i]->setCount(ColCount);
            //inicializa a cero los nuevos parámetros
            for(j; j<ColCount; j++)
                (*this->Items[i])[j] = 0;
        }
        //i queda indicando a la primera nueva fila

        //reserva las nuevas filas, las redimensiona
        //y las inicializa a cero
        this->setCount(RowCount); //reserva las nuevas filas
        for(i; i<this->getCount(); i++) {
            //redimensiona el vector
            this->Items[i]->setCount(ColCount);
            //inicializa a cero los nuevos parámetros
            this->Items[i]->SetAll(0);
        }
    }
}

//---------------------------------------------------------------------------
/*
//copia los elemenetos de una lista
template <class T> void TPLVector<T>::Copy(const TPLVector<T> &L)
{
        //redimensiona la lista
        Count = L.Count;

        //copia los elementos
        for(int i=0; i<getCount(); i++)
                *(Items[i]) = L[i];
}
  */

//añade cada elemento de un vector al final de cad alista
template <class T> void TPLVector<T>::AddColumn(TVector<T> &v)
{
    //la dimensión del vector v debe ser igual al número de columnas
    if(v.Count != this->getCount())
        throw EImproperArgument("dimension vector v should be equal row number");

    //por cada fila
    for(int i=0; i<this->getCount(); i++)
        this->Items[i]->Add(v[i]); //añade el elemento indicado al finald el vector
}

//copia los vectores de una PLV
template <class T> TPLVector<T>& TPLVector<T>::operator=(
        const TPLVector<T> &PLV)
{
    this->Clear(); //destruye todos los elementos de la lista
    this->setCount(PLV.getCount()); //redimensiona la lista

    //copia los elementos
    for(int i=0; i<this->getCount(); i++)
        *(this->Items[i]) = PLV[i];

    //devuelve una referencia a esta PLV para
    //permitir la anidación de asignaciones
    return *this;
}

//asigna el mismo valor a todos los elementos
template <class T> void TPLVector<T>::SetAll(T x)
{
    for(int i=0; i<this->getCount(); i++)
        for(int j=0; j<this->Items[i]->Count; j++)
            this->Items[i]->Set(j, x);
}

//asigna a cada elemento una muestra de la v.a.--> U[xmin, xmax]
template <class T> void TPLVector<T>::Random(T xmin, T xmax)
{
    //el intervalo debe ser positivo
    if(xmin > xmax)
        throw EImproperArgument("xmin should be less or equal xmax");

    //asigna un nº aleatorio a cada elemento
    int i, j;
    for(i=0; i<this->getCount(); i++)
        for(j=0; j<this->Items[i]->Count; j++)
            (*this->Items[i])[j] = RandomUniform(xmin, xmax);
}
//---------------------------------------------------------------------------
/*//compara los vectores de una PLV
template <class T> bool TPLVector<T>::operator==(const TPLVector<T> &L) const
{
    //si no tienen el mismo nº de vectores devuelve falso
    if(Count != L.Count)
            return false;

    //si encuentra algún vector distinto devuelve falso
    for(int i=0; i<getCount(); i++)
                if(Items[i]!=NULL || L[i]!=NULL) { //si ambos no son nulos
                        if(Items[i]==NULL || L.Get(i)==NULL) //si solo lo es uno
                                return false; //indica que no son iguales
                        //si ambos no son nulos compara los vectores
                        if((*(Items[i])!=L[i])) //si difieren
                            return false; //indica que no son iguales
                }

    return true; //indica que no ´se han encontrado diferencias
}
template <class T> bool TPLVector<T>::operator!=(const TPLVector<T> &L) const
{
    if(*this == L)
                return false;
        else
                return true;
}               */
//busca la primera ocurrencia del elemento
//si no la encuentra devuelve falso e (i, j)=(Count, Items[Count-1]->Count)
template <class T> bool TPLVector<T>::Find(T item, int &i, int &j) const
{
    //recorre secuencialmente la lista en busca del puntero a elemento
    for(i=0; i<this->getCount(); i++) {
        //si no hay vector no puede encontrarlo
        if(this->Items[i] == NULL)
            return false;
        //busca el elemento en el vector
        for(j=0; j<this->Items[i]->Count; j++)
            if(item == this->Items[i]->Get(j))
                return true;
    }

    return false; //indica que no lo ha encontrado
}

//calcula el vector media
template <class T> void TPLVector<T>::GetVectorAve(TVector<double> &v)
{
    TVector<T> *pv;

    //inicializa el vector de medias
    v.Clear();
    //suma las componentes de los vectores al vector de medias
    for(int i=0; i<this->getCount(); i++) {
        //apunta el vector indicado para facilitar su acceso
        pv = this->Items[i];
        //completa con ceros hasta alcanzar la longitud del vector de turno
        while(v.getCount() < pv->getCount())
            v.Add(0);
        //suma las componentes del vector indicado
        for(int j=0; j<pv->Count; j++)
            v[j] += pv->Get(j);
    }
    //divide las componentes del vector de medias entre el número de vectores
    v /= this->getCount();
}
//obtiene el vector mínima
template <class T> void TPLVector<T>::GetVectorMin(TVector<double> &v)
{
    TVector<T> *pv;
    int J;
    T x;

    //inicializa el vector de mínimas
    v.Clear();
    //por cada vector de la lista
    for(int i=0; i<this->getCount(); i++) {
        //apunta el vector indicado para facilitar su acceso
        pv = this->Items[i];
        //determina la longitud del vector más corto
        J = min(v.getCount(), pv->getCount());
        //por cada par de componentes
        for(int j=0; j<J; j++) {
            //asigna la componente del nuevo vector para facilitar su acceso
            x = pv->Get(j);
            //si la nueva componente es menor que la de referencia
            if(x < v[j])
                v[j] = x; //asigna la nueva componente
        }
        //añade las nuevas componentes que no tiene comparación posible
        while(v.getCount() < pv->getCount())
            v.Add(pv->Get(v.getCount()));
    }
}
//obtiene el vector máxima
template <class T> void TPLVector<T>::GetVectorMax(TVector<double> &v)
{
    TVector<T> *pv;
    int J;
    T x;

    //inicializa el vector de máximas
    v.Clear();
    //por cada vector de la lista
    for(int i=0; i<this->getCount(); i++) {
        //apunta el vector indicado para facilitar su acceso
        pv = this->Items[i];
        //determina la longitud del vector más corto
        J = min(v.getCount(), pv->getCount());
        //por cada par de componentes
        for(int j=0; j<J; j++) {
            //asigna la componente del nuevo vector para facilitar su acceso
            x = pv->Get(j);
            //si la nueva componente es mayor que la de referencia
            if(x > v[j])
                v[j] = x; //asigna la nueva componente
        }
        //añade las nuevas componentes que no tiene comparación posible
        while(v.getCount() < pv->getCount())
            v.Add(pv->Get(v.getCount()));
    }
}
//obtiene el vector varianza
template <class T> void TPLVector<T>::GetVectorVar(TVector<double> &v)
{
    TVector<T> *pv;
    TVector<double> ave;
    int J;

    //calcula el vector de medias
    GetVectorAve(ave);
    //inicializa el vector de varianzas
    v.Clear();
    //por cada vector de la lista
    for(int i=0; i<this->getCount(); i++) {
        //apunta el vector indicado para facilitar su acceso
        pv = this->Items[i];
        //completa con ceros hasta alcanzar la longitud del vector de turno
        while(v.getCount() < pv->getCount())
            v.Add(0);
        //suma las desviaciones al cuadrado del vector indicado
        for(int j=0; j<pv->Count; j++)
            v[j] += pow(pv->Get(j) - ave[j], 2.);
    }
    //divide las componentes del vector de varianzas entre el número de vectores
    v /= this->getCount();
}

//---------------------------------------------------------------------------

} //namespace Lists

//---------------------------------------------------------------------------
#endif // PLVECTOR_H
