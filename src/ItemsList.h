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
//File: ItemsList.h
//Content: template for item list
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ITEMSLIST_H
#define ITEMSLIST_H

#include "SlideArray.h" //SlideArray
#include "Strings.h" //namespace Strings
#include "StrPR.h" //namespace Strings
#include "Scalars.h" //RandomUniform

#include <typeinfo> //namespace std

//---------------------------------------------------------------------------

using namespace std; //type_info
using namespace Strings; //StrTravelToEnd
using namespace Mathematics; //RandomUniform

//namespace for lists
namespace Lists {

//---------------------------------------------------------------------------
//class TItemsList
//---------------------------------------------------------------------------

//clase lista de elementos genéricos
template <class T> class TItemsList /*#: public QObject*/ {
protected:
    //------------------------------------------------------------------
    //PROPIEDADES INDEPENDIENTES DE LAS FUNCIONES EXTERNAS
    //------------------------------------------------------------------

    //ELEMENTOS:

    //array deslizante de elementos
    TSlideArray<T> Items;


public:
    //------------------------------------------------------------------
    //PROPIEDADES INDEPENDIENTES DE LAS FUNCIONES EXTERNAS
    //------------------------------------------------------------------

    //tipo de los elementos
    const type_info& getType(void) const {return typeid(T);}

    //PROPIEDADES DE DIMENSIONAMIENTO:

    //número de elementos reservados de la lista
    int getCapacity(void) const {return Items.getCapacity();}
    void setCapacity(int);
    //númer de elementos de la lista
    int getCount(void) const {return Items.getCount();}
    void setCount(int);

    //PROPIEDADES DE ACCESO A ELEMENTOS:

    //accede al primer elemento
    inline const T &getFirst(void) const;
    inline void setFirst(T&);
    //accede al último elemento
    inline const T &getLast(void) const;
    inline void setLast(T&);

    //------------------------------------------------------------------
    //PROPIEDADES DEPENDIENTES DE LAS FUNCIONES EXTERNAS
    //------------------------------------------------------------------

    //FUNCIONES EXTERNAS:

    //función de comparación de elementos tipo T
    int ( *Compare)(const T item1, const T item2);
    //puntero a función de evaluación de elementos tipo T
    double ( *Evaluate)(T item);
    //puntero a función de asignación de elementos tipo T
    void ( *Assign)(T &item, double x);
    //puntero a la función de impresión de elementos tipo T
    void ( *Print)(AnsiString &S, const T item);
    //Puntero a la función de lectura de elementos tipo T
    void ( *Read)(T &item, const AnsiString &S, int &i);

    //no se especifica un método de evaluación de elemento apuntado,
    //porque si el tipo T fuera entero se confundiría

    //ADVERTENCIA: si el tipo T es un punero, la función Read
    //deberá construir el elemento que devuelve.

    //FUNCIONES ESTÁTICAS:

    //imprime una lista en una cadena de texto
    //en formato de fila de texto
    static void  PrintRow(AnsiString &S,
                                    const TItemsList<T> *L);
    //imprime una lista en una cadena de texto
    //en formato de columna de texto
    static void  PrintColumn(AnsiString &S,
                                       const TItemsList<T> *L);
    //lee una lista de una cadena de texto
    //en formato de fila o columna de texto
    static void  ReadSeparated(TItemsList<T> *L,
                                         const AnsiString &S, int &i);
    //lee una lista de una cadena de texto
    //en formato de fila o columna de texto
    //para los elementos de la lista previamente construidos
    static void  ReadSeparatedForBuiltItems(TItemsList<T> *L,
                                                      const AnsiString &S, int &i);

    //imprime una lista en una cadena de texto
    //en formato de lista de texto
    static void  PrintList(AnsiString &S,
                                     const TItemsList<T> *L);
    //lee una lista de una cadena de texto
    //en formato de lista de texto
    static void  ReadList(TItemsList<T> *L,
                                    const AnsiString &S, int &i);
    //lee una lista de una cadena de texto
    //en formato de lista de texto
    //para los elementos de la lista previamente construidos
    static void  ReadListForBuiltItems(TItemsList<T> *L,
                                                 const AnsiString &S, int &i);

    //NOTA: las funciones PrintList y ReadList deberán ser apuntadas
    //en los punteros Print y Read de listas de listas.

    //COMPARACIÓN ELEMENTOS:

    //ADVERTENCIA:
    //No se han definido funciones de búsqueda que utilicen
    //los operadores de comparación aritméticos, por que eso
    //limitaría el tipo de objetos para los que pueden contruirse
    //listas, a aquellos que tengan dichos operadores definidos.

    //índice al primer elemento mínimo
    int getFirstMin(void) const;
    //índice al último elemento de máximo valor
    int getLastMax(void) const;

    //flag de ordenación creciente
    bool getIncreasing(void) const;
    //flag de ordenación decreciente
    bool getDecreasing(void) const;

    //EVALUACIÓN DE ELEMENTOS:

    //puntero a función de comparación de elementos tipo T
    double getMin(void) const; //mínimo
    double getMax(void) const; //máximo
    double getSum(void) const; //sumatorio
    double getAve(void) const; //media
    double getVar(void) const; //varianza

    //PROPIEDADES EN FORMATO TEXTO:

    //número de elementos
    AnsiString getCapacityText(void) const;
    void setCapacityText(const AnsiString&);
    //número de elementos
    AnsiString getCountText(void) const;
    void setCountText(const AnsiString&);

    //elemento indicado
    AnsiString getItemsText(int) const;
    void setItemsText(int, const AnsiString&);
    //primer elemento
    AnsiString getFirstText(void) const;
    void setFirstText(const AnsiString&);
    //último elemento
    AnsiString getLastText(void) const;
    void setLastText(const AnsiString&);

    //lista en formato texto con delimitadores '{', ',' y '}'
    AnsiString getText(void) const;
    void setText(const AnsiString&);
    //lista en formato fila
    //elementos de la lista separado por tabuladores '\t'
    AnsiString getRowText(void) const;
    void setRowText(const AnsiString&);
    //lista en formato columna
    //una lina de texto terminada en "\r\n" por cada elemento de la lista
    AnsiString getColumnText(void) const;
    void setColumnText(const AnsiString&);

    //------------------------------------------------------------------
    //MÉTODOS INDEPENDIENTES DE LAS FUNCIONES EXTERNAS:
    //------------------------------------------------------------------

    //MÉTODOS PÚBLICOS:

    //constructor general
    TItemsList(int Capacity=8,
               int ( *Compare)(T, T)=NULL,
               double ( *Evaluate)(T)=NULL,
               void ( *Assign)(T&, double)=NULL,
               void ( *Print)(AnsiString&, T)=NULL,
               void ( *Read)(T&, const AnsiString&, int&)=NULL);

    //ADVERTENCIA: declarar los argumentos por referencia constantes es
    //necesario para evitar las advertencias cuando son pasadas variables
    //"Temporary", y no se quiere que salten las "Warnings".

    //ADVERTENCIA: no es posible contruir una TPointersList<T>,
    // donde T es una clase abstracta, de modo que las listas de
    //punteros a clase abstracta deberán ser del tipo TItemsList<T*>.

    //construye un clon de una lista de elementos
    TItemsList(const TItemsList*);
    TItemsList(const TItemsList&);

    //clona una lista de elementos
    void Clone(const TItemsList<T>&);

    //libera la memoria ocupada por la lista de elementos
    ~TItemsList();

    //ADVERTENCIA: ~TItemsList no destruye los lementos apuntados
    //en una lista de punteros.

    //MÉTODOS DE DIMENSIONAMIENTO:

    //añade elementos al principio de la lista
    void NewFirst(int N=1);
    //añade elementos al final de la lista
    void NewLast(int N=1);
    //borra elementos al principio de la lista
    void DelFirst(int N=1);
    //borra elementos al final de la lista
    void DelLast(int N=1);

    //MÉTODOS DE ACCESO:

    //NOTA: el intervalo de indexación corresponde a: [0, getCount()-1].

    //ADVERTENCIA: en listas de punteros, la sobreescritura o eliminación
    //de un item puede ocasionar el descolgamiento de un objeto

    //lectura del elemento indicado en listas no constantes
    inline T &Get(int);
    //lectura elemento indicado en listas constantes
    inline const T &Get(int) const;
    //escritura del elemento indicado
    inline void Set(int i, const T &item);
    //lectura del elemento indicado en listas no constantes
    inline T& operator[](int i);
    //lectura del elemento indicado en listas constantes
    inline const T& operator[](int i) const;

    //MÉTODOS DE ADICIÓN:

    //añade un elemento al final de la lista
    inline void Add(const T&);
    //añade los elementos de una lista
    void Add(TItemsList<T>&);
    //añade los elementos [i1, i2] de L en esta lista
    void Add(TItemsList<T>& L, int i1, int i2);
    //inserta un elemento al principio de la lista
    void InsertFirst(T&);
    //inserta un elemento en la posición indicada
    void Insert(int, const T&);

    //apila por el principio según la disciplina FIFO
    void PushFirst(T&);
    //apila por el final según la disciplina FIFO
    void PushLast(T&);

    //MÉTODOS DE BORRADO:

    //borra la posición indicada
    void Delete(int i);
    //vacía la lista
    void Clear(void) {Items.DelLast(getCount());}
    //destruye los objetos apuntados por una lista de punteros
//    friend void Destroy(TItemsList<T*> &L);

    //MÉTODOS DE COPIA:

    //copia los elementos de una lista
    void Copy(const TItemsList<T>& L);
    //copia los elementos de una lista
    TItemsList<T>& operator=(const TItemsList<T>& L);

    //RECUERDE: el operador de asignación = no se hereda

    //copia los elementos del intervalo indicado de una lista
    void Copy(const TItemsList<T>& L, int i1, int i2);

    //MÉTODOS DE TRANSPOSICIÓN:

    //invierte el orden de los elementos de la lista
    void Invert(void);
    //desordena los elementos de la lista
    void Randomize(void);

    //------------------------------------------------------------------
    //MÉTODOS DEPENDIENTES DE LAS FUNCIONES EXTERNAS:
    //------------------------------------------------------------------

    //MÉTODOS DE COMPARACIÓN:

    //ADVERTENCIA:
    //No se han definido funciones de búsuqeda que utilicen
    //los operadores de comparación aritméticos, por que eso
    //limitaría el tipo de objetos para los que pueden contruirse
    //listas, a aquellos que tengan dichos operadores definidos.

    //compara dos listas
    bool operator==(const TItemsList<T>&) const;
    bool operator!=(const TItemsList<T>&) const;
    //busca un elemento y si no lo encuentra devuelve getCount
    int Search(const T&) const;

    //devuelve un índice al primer elemento mayor que
    int FirstUpperTo(const T&) const;
    //devuelve un índice al primer elemento mayor o igual que
    int FirstUpperOrEqualTo(const T&) const;
    //devuelve un índice al primer elemento menor que
    int LastLowerTo(const T&) const;
    //retorna un índice al primer elemento menor o igual que
    int LastLowerOrEqualTo(const T&) const;

    //ordena de menor a mayor en el intervalo [first, last]
    void SortInc(int first, int last);
    //ordena de mayor a menor en el intervalo [first, last]
    void SortDec(int first, int last);
    //ordena de menor a mayor en el intervalo [0, getCount()-1]
    void SortInc(void);
    //ordena de mayor a menor en el intervalo [0, getCount()-1]
    void SortDec(void);
    //inserta en orden creciente
    //devuelve un índice a la posición de inserción
    int InsertInc(const T&);
    //inserta en orden decreciente
    //devuelve un índice a la posición de inserción
    int InsertDec(const T&);

    //MÉTODOS DE EVALUACIÓN:

    //suma de potencias
    double Pow(int) const;
    //producto interno con un vector
    double Dot(const TItemsList<T>&) const;

    //MÉTODOS DE ASIGNACIÓN:

    //asigna el mismo valor a todas las componentes
    void SetAll(double x);
    //asigna valores equiespaciados en [xmin, (getCount()-1) deltax]
    void SetEquidist(double xmin, double deltax);
    //asigna las particiones del segmento [xmin, xmax] en (getCount() - 1) partes
    void SetPartitions(double xmin, double xmax);
    //asigna valores con distribución uniforme en [xmin, xmax]
    void Random(double xmin, double xmax);

    //suma los valores de las componentes homónimas de un vector
    TItemsList<T>& operator+=(const TItemsList<T>&);
    //resta los valores de las componentes omónimas de un vector
    TItemsList<T>& operator-=(const TItemsList<T>&);
    //multiplica los valores por un escalar
    TItemsList<T>& operator*=(double t);
    //divide los valores por un escalar
    TItemsList<T>& operator/=(double t);
};

//ADVERTENCIA: si pasa un objeto tipo TItemsList<T> por valor
//se producirá un error cuando la lista que se le pase
//tenga más de 8 elementos, ya que el ovjeto es construido
//con el número de elementos por defecto y la lista
//que se le pase es copiado byte a byte, no permitiendo
//el redimensionado automático.

//Nomenclatura recomendada:
//      lista de elementos tipo TItem:              TItemL
//      lista de punteros a elementos tipo TItem:   TItemLP

//---------------------------------------------------------------------------
//FUNCIONES ESTÁTICAS:

//imprime una lista en una cadena de texto
//en formato de fila de texto
template <class T> void  TItemsList<T>::PrintRow(AnsiString &S,
                                                           const TItemsList<T> *L)
{
    if(L == NULL)
        throw EImproperArgument("pointer L should point to built based pointer list");

    S += L->getRowText();
}
//imprime una lista en una cadena de texto
//en formato de columna de texto
template <class T> void  TItemsList<T>::PrintColumn(AnsiString &S,
                                                              const TItemsList<T> *L)
{
    if(L == NULL)
        throw EImproperArgument("pointer L should point to built item list");

    S += L->getColumnText;
}
//lee una lista de una cadena de texto
//en formato de fila de texto
template <class T> void  TItemsList<T>::ReadSeparated(TItemsList<T> *L,
                                                      const AnsiString &S, int &i)
{
    //comprueba las precondiciones
    if(L == NULL)
        throw EImproperArgument("pointer L should point to built based pointer list");
    if(L->Read == NULL)
        throw EImproperCall("pointer Read should point to reading function");
    if(i<=0 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a posicion in the string S");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //estado de la máquina de estados
    //      0: esperando próximo elemento o fin de cadena
    //      1: fin de cadena encontrada y lista de elementos leída con éxito
    int status = 0;

    //variables tampón
    T Item;
    TSlideArray<T> Items;

    //avanza el índice hasta el comienzo de la lista
    StrTravelSeparatorsIfAny(S, i);

    do { //repite
        //si no ha llegado al final de la cadena
        if(i <= S.Length())
            try {
            //lee el próximo elemento
            L->Read(Item, S, i);
            //añade el elemento a la lista tampón
            Items.AddLast(Item);
            //avanza el índice hasta el próximo caracter no separador
            StrTravelSeparatorsIfAny(S, i);
        } catch(Exception& E) {
            E.Message.Insert(1, "reading separated item list in text format: ");
            throw;
        }
        else //si ha llegado al final de la cadena
        //indica que ha llegado al final de la cadena
        status++;
        //mientras no se haya leido la lista con éxito
    } while(status < 1);

    //copia las variables tampón
    L->Clear();
    for(int j=0; j<Items.getCount(); j++)
        L->Add(Items[j]);
}
//lee una lista de una cadena de texto
//en formato de fila de texto
//para los elementos de la lista previamente construidos
template <class T> void  TItemsList<T>::ReadSeparatedForBuiltItems(TItemsList<T> *L,
                                                                             const AnsiString &S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el puntero Read debería apuntar a una función de lectura
    if(L->Read == NULL)
        throw EImproperCall("pointer Read should point to reading function");

    //el índice i debería indicar a alguna posición de la cadena de texto S
    if(i<0 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a posicion in the string S");

    //si la lista no contiene elementos
    if(L->getCount() < 1)
        return; //no lee nada

    //Aunque la cadena contuviera una lista de lementos legibles,
    //esta función solo intenta leer por cada elemento de la lista.

    //------------------------------------------------------------------
    //APLICA LA FUNCIÓN DE LECTURA A CADA ELEMENTO DE LA LISTA:

    //lista tampón
    TSlideArray<T> Items(L->Items); //clona los elementos de la lista

    try {
        //por cada elemento de la lista
        for(int j=0; j<Items.getCount(); j++)
            //aplica la función de lectura al elemento
            L->Read(Items[j], S, i);
    }
    catch(...) {
        throw;
    }

    //clona la lista tampón
    L->Items.Clone(Items);
}

//imprime una lista en una cadena de texto
//en formato de lista de texto
template <class T> void  TItemsList<T>::PrintList(AnsiString &S,
                                                            const TItemsList<T> *L)
{
    //el puntero L debería apuntar a una lista de construida
    if(L == NULL)
        throw EImproperArgument("pointer L should point to built list");

    //la lista L debería tener una función de impresión apuntada
    if(L->Print == NULL)
        throw EImproperCall("list L should have a pointed Print function");

    S += "{";

    if(L->getCount() > 0) {
        L->Print(S, L->Items.getFirst());
        for(int i=1; i<L->getCount(); i++) {
            S += ", ";
            L->Print(S, L->Items[i]);
        }
    }
    S += "}";
}
//lee una lista de una cadena de texto
//en formato de lista de texto
template <class T> void  TItemsList<T>::ReadList(TItemsList<T> *L,
                                                           const AnsiString &S, int &i)
{
    //el puntero L debería apuntar a una lista construida
    if(L == NULL)
        throw EImproperArgument("pointer L should point to built list");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //la lista L debería tener una función de lectura apuntada
    if(L->Read == NULL)
        throw EImproperCall("list L should have a pointed Read function");

    //el índice i debería indicar a una posición de la adena de texto S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("index i should indicate a character of the string S");

    //COMPRUEBA EL FORMATO DE LA LISTA:

    char c; //caracter S[i]
    //estado de la máquina de estados
    //      0: esperando ' ', '\r', o '{'
    //      1: esperando ' ', '\r', elemento o '}'
    //      2: esperando ' ', '\r', ',' o '}'
    //      3: esperando ' ', '\r' o elemento
    //      4: lista convertida con éxito
    int status = 0;
    //lista tampón
    TSlideArray<T> Items;
    //elemento auxiliar
    T Item;

    //si indica a la posúltima posición de la cadena
    if(i > S.Length())
        //indica que no ha encontrado el caracter de inicio de lista
        throw EImproperArgument("'{' not found");

    do { //repite
        //asigna el caracter indicado para facilitar su acceso
        c = S[i];

        //reacciona según el estado y el caracter
        switch(status) {
        case 0: //esperando ' ' o '{'
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("'{' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("'{' not found");
                break;
            case '{':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("Item or '}' not found");
                status++;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ' ' or '{'"));
            }
            break;
        case 1: //esperando ' ', elemento o '}'
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("'}' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("'}' not found");
                break;
            case '}':
                i++;
                status = 4;
                break;
            default:
                try {
                Items.NewLast();
                L->Read(Item, S, i);
                Items.setLast(Item);
                if(i > S.Length())
                    throw EImproperArgument("',' or '}' not found");
                status++;
            } catch(EImproperArgument &E) {
                    throw EImproperArgument(E.Message+AnsiString(" in item ")+IntToStr(Items.getCount()));
                } catch(...) {
                throw;
            }
            }
            break;
        case 2: //esperando ' ', ',' o '}'
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("',' or '}' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("',' or '}' not found");
                break;
            case ',':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("Item not found");
                status++;
                break;
            case '}':
                i++;
                status = 4;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ' ', ',' or '}'"));
            }
            break;
        case 3: //esperando ' ' o elemento
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("Item not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("Item not found");
                break;
            default:
                try {
                Items.NewLast();
                L->Read(Item, S, i);
                Items.setLast(Item);
                if(i > S.Length())
                    throw EImproperArgument("',' or '}' not found");
                status--;
            } catch(EImproperArgument &E) {
                    throw EImproperArgument(E.Message+AnsiString(" in item ")+IntToStr(Items.getCount()));
                } catch(...) {
                throw;
            }
            }
            break;
        }
        //mientras la lista no esté convertida con éxito
    } while(status < 4);

    //ASIGNA LOS ELEMENTOS:

    L->setCount(Items.getCount());
    for(int i=0; i<Items.getCount(); i++)
        L->Set(i, Items[i]);
}
//lee una lista de una cadena de texto
//en formato de lista de texto
//para los elementos de la lista previamente construidos
template <class T> void  TItemsList<T>::ReadListForBuiltItems(TItemsList<T> *L,
                                                                        const AnsiString &S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caraacter por defecto.

    //el puntero L debería apuntar a una lista basada en puntero construida
    if(L == NULL)
        throw EImproperArgument("pointer L should point to built based pointer list");

    //la lista L debería tener una función de lectura apuntada
    if(L->Read == NULL)
        throw EImproperCall("list L should have a pointed reading function");

    //el índice i debería indicar a una posición de la adena de texto S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("index i should indicate a character of the string S");

    //si la lista no contiene elementos
    if(L->getCount() < 1)
        return; //no lee nada

    //Aunque la cadena contuviera una lista de lementos legibles,
    //esta función solo intenta leer por cada elemento de la lista.

    //------------------------------------------------------------------
    //APLICA LA FUNCIÓN DE LECTURA A CADA ELEMENTO DE LA LISTA:

    //lista tampón
    TSlideArray<T> Items(L->Items); //clona los elementos de la lista
    //puntero a elemento
    T *Item;

    //LEE EL PRIMER ELEMENTO DE LA LISTA:

    try {
        //busca el primer elemento construido
        int j=0;
        while(j<Items.getCount() && Items[j]==NULL)
            j++;
        //si no ha encontrado un elemento consturido
        if(j >= Items.getCount())
            return; //no lee nada
        //asigna el puntero indicado para facilitar su acceso
        Item = &(Items[j]);
        //atraviesa el caracter delimitador de inicio de lista
        StrTravelLabel("{", S, i);
        //aplica la función de lectura al elemento
        L->Read(Item, S, i);

        //por cada puntero adicional de la lista
        for(; j<Items.getCount(); j++) {
            //apunta el elemento indicado para facilitar su acceso
            Item = &(Items[j]);
            //siel puntero apunta a un elemento construido
            if(Item != NULL) {
                //atraviesa el caracter delimitador de lista
                StrTravelLabel(",", S, i);
                //aplica la función de lectura al elemento
                L->Read(Item, S, i);
            }
        }

        //atraviesa el caracter delimitador de fin de lista
        StrTravelLabel("}", S, i);

    } catch(...) {
        throw;
    }

    //clona la lista tampón
    L->Items->Clone(Items);
}

//---------------------------------------------------------------------------
//FUNCIONES FRIEND:

//ADVERTENCIA: el uso del operador de indexación debe restringirse en
//las funciones friend porque está sujeto a redefinición en las clases derivadas

//ADVERTENCIA: lafunción Destroy conviene invocarlo usando el SRO:
//      Lists::Destroy(List);
//para evitar conflictos con los métodos loscales de la clase
//desde la cual es invocado.

//destruye los objetos apuntados por los punteros de una lista
template <class T> void Destroy(TItemsList<T*> &L)
{
    //destruye los objetos de la lista
    for(int i=0; i<L.getCount(); i++)
        if(L.Get(i) != NULL)
            delete L.Get(i);

    L.Clear(); //vacia la lista
}

//---------------------------------------------------------------------------
//PROPIEDADES DE DIMENSIONAMIENTO:

//redimensiona el buffer
template <class T> void TItemsList<T>::setCapacity(int Capacity)
{
    try {
        //redimensiona la capacidad del array deslizante
        if(Capacity != getCapacity())
            Items.setCapacity(Capacity);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting Capacity: ");
        throw;
    }
}

//redimensiona la lista reservando o liberando elementos por la dcha
template <class T> void TItemsList<T>::setCount(int Count)
{
    try {
        //redimensiona el array deslizante
        if(Count < getCount())
            Items.DelLast(getCount() - Count);
        else if(Count > getCount())
            Items.NewLast(Count - getCount());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting Count: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//PROPIEDADES DE ACCESO A ELEMENTOS:

template <class T> inline const T &TItemsList<T>::getFirst(void) const
{
    //el número de elementos de la lista getCount() debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("list items number getCount() should be one almost");

    return Items.getFirst();
}
template <class T> inline void TItemsList<T>::setFirst(T& item)
{
    //el número de elementos de la lista getCount() debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("list items number getCount() should be one almost");

    Items.setFirst(item);
}

template <class T> inline const T &TItemsList<T>::getLast(void) const
{
    //el número de elementos de la lista getCount() debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("list items number getCount() should be one almost");

    return Items.getLast();
}
template <class T> inline void TItemsList<T>::setLast(T& item)
{
    //el número de elementos de la lista getCount() debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("list items number getCount() should be one almost");

    Items.setLast(item);
}

//---------------------------------------------------------------------------
//PROPIEDADES DE COMPARACIÓN:

template <class T> int TItemsList<T>::getFirstMin(void) const
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    //la lista debería contener algún elemento
    if(getCount() < 1)
        throw EImproperCall("list should contain some item");

    //busca el elemento mínimo
    int imin = 0;
    for(int i=1; i<getCount(); i++)
        if(Compare(Items[i], Items[imin]) < 0)
            imin = i;

    return imin;
}

template <class T> int TItemsList<T>::getLastMax(void) const
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    //la lista debería contener algún elemento
    if(getCount() < 1)
        throw EImproperCall("list should contain some item");

    //busca el elemento máximo
    int imax = getCount() - 1;
    for(int i=getCount()-2; i>=0; i--)
        if(Compare(Items[i], Items[imax]) > 0)
            imax = i;

    return imax;
}

//comprueba si está ordenado de menor a mayor
template <class T> bool TItemsList<T>::getIncreasing(void) const
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    //si encuentra un elemento menor que el anterior devuelve falso
    for(int i=1; i<getCount(); i++)
        if(Compare(Items[i-1], Items[i]) > 0)
            return false;

    return true;
}

template <class T> bool TItemsList<T>::getDecreasing(void) const
//comprueba si está ordenado de mayor a menor
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    //si encueltra un elemento mayor que el anterior devuelve falso
    for(int i=1; i<getCount(); i++)
        if(Compare(Items[i-1], Items[i]) < 0)
            return false;

    return true;
}

//---------------------------------------------------------------------------
//PROPIEDADES DE EVALUACIÓN:

template <class T> double TItemsList<T>::getMin(void) const
{
    //debe haber una función de evaluación apuntada
    if(Evaluate == NULL)
        throw EImproperCall("function Evaluate not pointed");

    //la lista no debe estar vacía
    if(getCount() < 1)
        throw EImproperCall("Empty list");

    //busca el elemento mínimo
    double xmin = Evaluate(Items[0]);
    double x;
    for(int i=1; i<getCount(); i++) {
        x = Evaluate(Items[i]);
        if(x < xmin)
            xmin = x;
    }

    return xmin;
}

template <class T> double TItemsList<T>::getMax(void) const
{
    //debe haber una función de evaluación apuntada
    if(Evaluate == NULL)
        throw EImproperCall("function Evaluate not pointed");

    //la lista no debe estar vacía
    if(getCount() < 1)
        throw EImproperCall("Empty list");

    //busca el elemento máximo
    double xmax = Evaluate(Items[0]);
    double x;
    for(int i=1; i<getCount(); i++) {
        x = Evaluate(Items[i]);
        if(x > xmax)
            xmax = x;
    }

    return xmax;
}

template <class T> double TItemsList<T>::getSum(void) const
{
    //debe haber una función de evaluación apuntada
    if(Evaluate == NULL)
        throw EImproperCall("function Evaluate not pointed");

    //suma las evaluaciones
    double sum = 0;
    for(int i=0; i<getCount(); i++)
        sum += Evaluate(Items[i]);

    return sum;
}

template <class T> double TItemsList<T>::getAve(void) const
{
    //la lista no debe estar vacía
    if(getCount() < 1)
        throw EImproperCall("Empty list");

    return getSum()/getCount();
}

template <class T> double TItemsList<T>::getVar(void) const
{
    //debe haber una función de evaluación apuntada
    if(Evaluate == NULL)
        throw EImproperCall("function Evaluate not pointed");

    //la lista no debe estar vacía
    if(getCount() < 1)
        throw EImproperCall("Empty list");

    //caclula la varianza
    double ave = getAve();
    double var = pow(Evaluate(Items[0])-ave, 2.);
    for(int i=1; i<getCount(); i++)
        var += pow(Evaluate(Items[i])-ave, 2.);
    var /= getCount();

    return var;
}

//---------------------------------------------------------------------------
//PROPIEDADES EN FORMATO TEXTO:

template <class T> AnsiString TItemsList<T>::getCapacityText(void) const
{
    return IntToStr(getCapacity());
}
template <class T> void TItemsList<T>::setCapacityText(const AnsiString& S)
{
    try {
        int aux = StrToInt(S);
        setCapacity(aux);

    } catch(...) {
        throw;
    }
}
template <class T> AnsiString TItemsList<T>::getCountText(void) const
{
    return IntToStr(getCount());
}
template <class T> void TItemsList<T>::setCountText(const AnsiString &S)
{
    try {
        getCount() = StrToInt(S);

    } catch(...) {
        throw;
    }
}
template <class T> AnsiString TItemsList<T>::getItemsText(int i) const
{
    //debería haber una función de impresión apuntada
    if(Print == NULL)
        throw EImproperArgument("should have pointed Print funcion");

    AnsiString S;

    try {
        Print(S, Items[i]);

    } catch(...) {
        throw;
    }

    return S;
}
template <class T> void TItemsList<T>::setItemsText(int i, const AnsiString &S)
{
    try {
        int j = 1;
        Read(Items[i], S, j);

    } catch(...) {
        throw;
    }
}
template <class T> AnsiString TItemsList<T>::getFirstText(void) const
{
    //debería haber una función de impresión apuntada
    if(Print == NULL)
        throw EImproperArgument("should have pointed Print funcion");

    AnsiString S;

    try {
        Print(S, getFirst());

    } catch(...) {
        throw;
    }

    return S;
}
template <class T> void TItemsList<T>::setFirstText(const AnsiString &S)
{
    //debería haber una función de lectura apuntada
    if(Read == NULL)
        throw EImproperArgument("should have pointed Read funcion");

    int i = 1;

    try {
        Read(getFirst(), S, i);

    } catch(...) {
        throw;
    }
}
template <class T> AnsiString TItemsList<T>::getLastText(void) const
{
    //debería haber una función de impresión apuntada
    if(Print == NULL)
        throw EImproperArgument("should have pointed Print funcion");

    AnsiString S;

    try {
        Print(S, getLast());

    } catch(...) {
        throw;
    }

    return S;
}
template <class T> void TItemsList<T>::setLastText(const AnsiString &S)
{
    //debería haber una función de lectura apuntada
    if(Read == NULL)
        throw EImproperArgument("should have pointed Read funcion");

    int i = 1;

    try {
        Read(getLast(), S, i);

    } catch(...) {
        throw;
    }
}

//traduce de lista a formato texto
template <class T> AnsiString TItemsList<T>::getText(void) const
{
    AnsiString S;

    try {
        PrintList(S, this);

    } catch(...) {
        throw;
    }

    return S;
}

//ADVERTENCIA: en listas de punteros los elementos pueden ser punteros vacíos
//por lo que la función SetText pasaría un "puntero basura" a la función Read
//y por consiguiente se produciría una violación de acceso a memoria

//traduce de formato texto a lista
template <class T> void TItemsList<T>::setText(const AnsiString &S)
{
    try {
        int i = 1;
        ReadList(this, S, i);
        //                StrTravelToEnd(S, i);
    } catch(...) {
        throw;
    }
}

//traduce de lista a formato columna de texto
template <class T> AnsiString TItemsList<T>::getRowText(void) const
{
    //el puntero Print debería apuntar a una función de impresión
    if(Print == NULL)
        throw EImproperCall("pointer Print should point to print function");

    AnsiString S;

    if(getCount() > 0) {
        Print(S, Items.First);

        for(int i=1; i<getCount(); i++) {
            S += "\t";
            Print(S, Items[i]);
        }
    }

    return S;
}
//traduce de formato texto a lista
template <class T> void TItemsList<T>::setRowText(const AnsiString &S)
{
    try {
        int i = 1;
        ReadSeparated(this, S, i);
    } catch(...) {
        throw;
    }
}
//traduce de lista a formato columna de texto
template <class T> AnsiString TItemsList<T>::getColumnText(void) const
{
    //el puntero Print debería apuntar a una función de impresión
    if(Print == NULL)
        throw EImproperCall("pointer Print should point to print function");

    AnsiString S;

    if(getCount() > 0) {
        Print(S, Items.getFirst());

        for(int i=1; i<getCount(); i++) {
            S += "\r\n";
            Print(S, Items[i]);
        }
    }

    return S;
}
//traduce de formato texto a lista
template <class T> void TItemsList<T>::setColumnText(const AnsiString &S)
{
    int i = 1; //initialize the index to the first char of string S

    try {
        //lee los elementos de la lista separados por separadores
        ReadSeparated(this, S, i);

        //avanza el índice i hasta la próxima posición que no contenga un separador
        StrTravelSeparators(S, i);
        //si el índice i indica a algún caracter de la cadena S
        if(i <= S.Length())
            //indica que la cadena S solo debería contener la lista de elmentos
            throw EImproperArgument("string S should contain the item list only");

    } catch(Exception& E) {
        unsigned int row, col;
        strPositionToCoordinates(row, col, S.str, i-1);
        E.Message.Insert(1, "setting column text in row "+inttostr(row)+" and column "+inttostr(col)+": ");
        throw;
    }
}

//---------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

template <class T> TItemsList<T>::TItemsList(int Capacity,
                                             int ( *t_Compare)(T, T),
                                             double ( *t_Evaluate)(T),
                                             void ( *t_Assign)(T&, double),
                                             void ( *t_Print)(AnsiString&, T),
                                             void ( *t_Read)(T&, const AnsiString&, int&)) :
    Items(Capacity, 0)
{
    Compare = t_Compare;
    Evaluate = t_Evaluate;
    Assign = t_Assign;
    Print = t_Print;
    Read = t_Read;
}

//construye un clon de una lista de elementos
template <class T> TItemsList<T>::TItemsList(const TItemsList *ItemList)
//se construye aquí el array deslizante por defecto
{
    //debe apuntar a una lista construida
    if(ItemList == NULL)
        throw EImproperArgument("ItemList should not be null");

    //clona el array deslizante
    Items.Clone(ItemList->Items);

    //apunta a las mismas funciones externas
    Compare = ItemList->Compare;
    Evaluate = ItemList->Evaluate;
    Assign = ItemList->Assign;
    Print = ItemList->Print;
    Read = ItemList->Read;
}
template <class T> TItemsList<T>::TItemsList(const TItemsList& ItemList) /*#:
    QObject()*/
//se construye aquí el array deslizante por defecto
{
    //clona el array deslizante
    Items.Clone(ItemList.Items);

    //apunta a las mismas funciones externas
    Compare = ItemList.Compare;
    Evaluate = ItemList.Evaluate;
    Assign = ItemList.Assign;
    Print = ItemList.Print;
    Read = ItemList.Read;
}

//clona una lista de elementos
template <class T> void TItemsList<T>::Clone(const TItemsList<T>& ItemsList)
{
    //clona el array deslizante
    Items.Clone(ItemsList.Items);

    //apunta a las mismas funciones externas
    Compare = ItemsList.Compare;
    Evaluate = ItemsList.Evaluate;
    Assign = ItemsList.Assign;
    Print = ItemsList.Print;
    Read = ItemsList.Read;
}

//libera la memoria ocupada por una lista de elementos
template <class T> TItemsList<T>::~TItemsList()
{
}

//---------------------------------------------------------------------------
//MÉTODOS DE DIMENSIONAMIENTO:

//añade elementos al principio de la lista
template <class T> inline void TItemsList<T>::NewFirst(int N)
{
    //el número de elementos a añadir N debería ser al menos uno
    if(N < 1)
        throw EImproperArgument("number items to build should be une almost");

    try {
        Items.NewFirst(N);
    } catch(...) {
        throw;
    }
}

//añade elementos al final de la lista
template <class T> inline void TItemsList<T>::NewLast(int N)
{
    //el número de elementos a añadir N debería ser al menos uno
    if(N < 1)
        throw EImproperArgument("number items to build should be une almost");

    try {
        Items.NewLast(N);
    } catch(...) {
        throw;
    }
}

//borra elementos al principio de la lista
template <class T> inline void TItemsList<T>::DelFirst(int N)
{
    //el número de elementos a borrar N debería estar en [1, getCount]
    if(N<1 || getCount()<N)
        throw EImproperArgument("number items to delete should be int [1, getCount]");

    Items.DelFirst(N);
}

//borra elementos al final de la lista
template <class T> inline void TItemsList<T>::DelLast(int N)
{
    //el número de elementos a borrar N debería estar en [1, getCount]
    if(N<1 || getCount()<N)
        throw EImproperArgument("number items to delete should be int [1, getCount]");

    Items.DelLast(N);
}

//---------------------------------------------------------------------------
//MÉTODOS DE ACCESO:

//lectura del elemento indicado en listas no constantes
template <class T> inline T &TItemsList<T>::Get(int i)
{
    //el elemento indicado debe existir
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i out bounds");

    return Items[i];
}
//lectura del elemento indicado en listas constantes
template <class T> inline const T &TItemsList<T>::Get(int i) const
{
    //el elemento indicado debe existir
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i out bounds");

    return Items[i];
}
//escritura del elemento indicado
template <class T> inline void TItemsList<T>::Set(int i, const T &item)
{
    //i debe indicar un elemento existente
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i out bounds");

    Items[i] = item;
}
//lectura del elemento indicado en listas no constantes
template <class T> inline T& TItemsList<T>::operator[](int i)
{
    //i debe indicar un elemento existente
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i out bounds");

    return Items[i];
}
//lectura del elemento indicado en listas constantes
template <class T> inline const T& TItemsList<T>::operator[](int i) const
{
    //i debe indicar un elemento existente
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i out bounds");

    return Items[i];
}

//---------------------------------------------------------------------------
//MÉTODOS DE ADICIÓN:

//Añade un elemento
template <class T> inline void TItemsList<T>::Add(const T &item)
{
    try {
        //añade el elemento
        Items.AddLast(item);
    } catch(...) {
        throw;
    }
}

//añade una lista
template <class T> void TItemsList<T>::Add(TItemsList<T>& L)
{
    //si la lista está vacía no hace nada
    if(L.getCount() < 1)
        return;

    //apunta a la primera posición a ocupar
    int j = getCount();

    //reserva los elementos
    Items.NewLast(L.getCount());

    //copia los elementos
    for(int i=0; i<L.getCount(); i++, j++)
        Items[j] = L[i];
}

//inserta un elemento al principio de la lista
template <class T> void TItemsList<T>::InsertFirst(T &item)
{
    try {
        Items.AddFirst(item);
    } catch(...) {
        throw;
    }
}

//añade los elementos del intervalo indicado de una lista
template <class T> void TItemsList<T>::Add(TItemsList<T>& L,
                                           int i1, int i2)
{
    //los indices deben apuntar a elementos de L
    if(i1<0  || L.getCount()<=i1 || i2<0  || L.getCount()<=i2)
        throw EImproperArgument("index out bounds");

    //el intervalo [i1, i2] debe ser positivo
    if(i1 > i2)
        throw EImproperArgument("i2 should be upper or equal i1");

    //reserva los elementos
    Items.NewLast(12-i1+1);
    //copia los elementos
    for(int i=i1; i<=i2; i++)
        Items.Last = L[i];
}

//inserta un elemento en la posición indicada
template <class T> void TItemsList<T>::Insert(int i, const T &item)
{
    try {
        Items.Insert(i, item);
    } catch(...) {
        throw;
    }
}

//apila por el principio según la disciplina FIFO
template <class T> void TItemsList<T>::PushFirst(T &item)
{
    //la lista no debe estar vacía
    if(getCount() < 1)
        throw EImproperCall("Empty list");

    //desliza el array una posición a la derecha
    Items.SlideLeft();
    //escribe el elemento en el array
    Items.First = item;
}

//apila por el final según la disciplina FIFO
template <class T> void TItemsList<T>::PushLast(T &item)
{
    //la lista no debe estar vacía
    if(getCount() < 1)
        throw EImproperCall("Empty list");

    //desliza el array una posición a la izquierda
    Items.SlideRight();
    //escribe el elemento en el array
    Items.setLast(item);
}

//---------------------------------------------------------------------------
//MÉTODOS DE BORRADO:

//borra el elemento i-ésimo
template <class T> void TItemsList<T>::Delete(int i)
{
    //el elemento indicado debe existir
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i out bounds");

    if(i < getCount()/2) { //el elemento está en la primera mitad
        //Desplaza los elementos
        for(int j=i; j>0; j--)
            Items[j] = Items[j-1];
        //libera el elemento
        Items.DelFirst();
    } else { //el elemento está en la segunda mitad
        //Desplaza los elementos
        for(int j=i+1; j<getCount(); j++)
            Items[j-1] = Items[j];
        //libera el elemento
        Items.DelLast();
    }
}

//---------------------------------------------------------------------------
//MÉTODOS DE COPIA:

//copia los elemenetos de una lista
template <class T> void TItemsList<T>::Copy(const TItemsList<T> &L)
{
    //redimensiona la lista
    if(L.getCount() < getCount())
        Items.DelLast(getCount() - L.getCount());
    else if(L.getCount() > getCount())
        Items.NewLast(L.getCount() - getCount());

    //copia los elementos
    for(int i=0; i<getCount(); i++)
        Items[i] = L[i];
}

//Copia los elementos [i1, i2] de una lista
template <class T> void TItemsList<T>::Copy(const TItemsList<T>& L,
                                            int i1, int i2)
{
    //los indices i1 e i2 deben apuntar a elementos de L
    if(i1<0  || L.getCount()<=i1 || i2<0  || L.getCount()<=i2)
        throw EImproperArgument("index out bounds");

    //el intervalo [i1, i2] debe ser positivo
    if(i1 > i2)
        throw EImproperArgument("i2 should be upper or equal i1");

    //redimensiona la lista
    int NewCount = i2-i1+1;
    if(NewCount < getCount())
        Items.DelLast(getCount() - NewCount);
    else if(NewCount > getCount())
        Items.NewLast(NewCount - getCount());


    //copia los elementos
    for(int i=0; i<getCount(); i++)
        Items[i] = L[i1++];
}

//copia los elemeentos de una lista
template <class T> TItemsList<T>& TItemsList<T>::operator=(
        const TItemsList<T> &L)
{
    //redimensiona la lista
    if(L.getCount() < getCount())
        Items.DelLast(getCount() - L.getCount());
    else if(L.getCount() > getCount())
        Items.NewLast(L.getCount() - getCount());

    //copia los elementos
    for(int i=0; i<getCount(); i++)
        Items[i] = L[i];

    return *this;
}

//---------------------------------------------------------------------------
//MÉTODOS DE TRANSPOSICIÓN:

//invierte el orden de los elementos de la lista
template <class T> void TItemsList<T>::Invert(void)
{
    //Solo se invierten listas de dos o más elementos
    if(getCount() > 1) {
        int i1 = 0; //índice desde el comienzo de la lista
        int i2 = getCount() - 1; //índice desde el final de la lista
        T aux; //para el intercambio

        do {
            aux = Items[i1];
            Items[i1] = Items[i2];
            Items[i2] = aux;
            i1++;
            i2--;
        } while(i1<i2);
    }
}

//desordena los elementos de la lista
template <class T> void TItemsList<T>::Randomize(void)
{
    //Solo se desordenan Listes de dos o más elementos
    if(getCount() > 1) {
        int j; //índice a la posición de intercambio
        T aux; //para el intercambio

        for(int i=getCount()-1; i>0; i--) {
            j = random(i+1); //genera la posición de intercambio
            if(j!=i) { //si sale el mismo no se intercambia
                aux = Items[i];
                Items[i] = Items[j];
                Items[j] = aux;
            }
        }
    }
}

//---------------------------------------------------------------------------
//MÉTODOS DEPENDIENTES DE LAS FUNCIONES EXTERNAS:
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//MÉTODOS DE COMPARACIÓN:

//compara dos listas
template <class T> bool TItemsList<T>::operator==(const TItemsList<T> &L) const
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    //para ser iguales deben tener el mismo nº de elementos
    if(L.getCount() != getCount())
        return false;

    //compara los elementos homónimos de las listas
    for(int i=0; i<getCount(); i++)
        if(Compare(L[i], Items[i]) != 0)
            return false;

    return true;
}

template <class T> bool TItemsList<T>::operator!=(const TItemsList<T> &L) const
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    //para ser iguales deben tener el mismo nº de elementos
    if(L.getCount() != getCount())
        return true;

    //compara los elementos homónimos de las listas
    for(int i=0; i<getCount(); i++)
        if(Compare(L[i], Items[i]) != 0)
            return true;

    return false;
}

//busca un elemento y si no lo encuentra devuelve getCount
template <class T> int TItemsList<T>::Search(const T &item) const
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    int i = 0;
    while(i<getCount() && Compare(Items[i], item))
        i++;

    return i;
}

//devuelve un índice al primer elemento mayor que
template <class T> int TItemsList<T>::FirstUpperTo(const T &item) const
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    //la lista debería contener algún elemento
    if(getCount() < 1)
        throw EImproperCall("list should contain some item");

    //busca el elemento
    int i;
    for(i=0; i<getCount(); i++)
        if(Compare(Items[i], item) > 0)
            return i;

    //si no lo encuentra devuelve getCount
    return i;
}

//devuelve un índice al primer elemento mayor o igual que
template <class T> int TItemsList<T>::FirstUpperOrEqualTo(const T &item) const
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    //la lista debería contener algún elemento
    if(getCount() < 1)
        throw EImproperCall("list should contain some item");

    //busca el elemento
    int i;
    for(i=0; i<getCount(); i++)
        if(Compare(Items[i], item) >= 0)
            return i;

    //si no lo encuentra devuelve getCount
    return i;
}

//devuelve un índice al primer elemento menor que
template <class T> int TItemsList<T>::LastLowerTo(const T &item) const
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    //la lista debería contener algún elemento
    if(getCount() < 1)
        throw EImproperCall("list should contain some item");

    //busca el elemento
    int i;
    for(i=getCount()-1; i>=0; i--)
        if(Compare(Items[i], item) < 0)
            return i;

    //si no lo encuentra devuelve -1
    return i;
}

//retorna un índice al primer elemento menor o igual que
template <class T> int TItemsList<T>::LastLowerOrEqualTo(const T &item) const
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    //la lista debería contener algún elemento
    if(getCount() < 1)
        throw EImproperCall("list should contain some item");

    //busca el elemento
    int i;
    for(i=getCount()-1; i>=0; i--)
        if(Compare(Items[i], item) <= 0)
            return i;

    //si no lo encuentra devuelve -1
    return i;
}

//Ordena aplicando el algoritmo quick sort al intervalo [first, last]
template <class T> void TItemsList<T>::SortInc(int first, int last)
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    //debe indicar a elementos de la lista
    if(first<0 || getCount()<=last)
        throw EImproperArgument("index out bounds");

    //debe indicar un intervalo positivo
    if(first > last)
        throw EImproperArgument("last should be upper or equeal first");

    int i = first;
    int j = last;
    T central = Items[(i + j)/2];
    T aux; //para el intercambio

    do {
        while(Compare(Items[i], central) < 0)
            i++;
        while(Compare(central, Items[j]) < 0)
            j--;
        if(i<j) {
            if(Compare(Items[i], Items[j]) > 0) {
                aux = Items[i];
                Items[i] = Items[j];
                Items[j] = aux;
            }
            //NOTA: debido a que los elementos pueden ser de
            //culaquier tipo, los casos en los que dos elementos
            //tengan igual evaluación no interesa trasponerlos.
            //P.e: una lista vacía y una lista conteniendo un cero,
            //puede que sus evaluaciones seean iguales a cero.
            //Es por eso que conviene coprobar si realmente
            //hace falta o no trasponerlos.
            i++;
            j--;
        } else if(i==j) {
            i++;
            j--;
        }
    } while(i<j);

    if(first<j)
        SortInc(first, j);
    if(i<last)
        SortInc(i, last);
}

//Ordena aplicando el algoritmo quick sort al intervalo [first, last]
template <class T> void TItemsList<T>::SortDec(int first, int last)
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    //debe indicar a elementos de la lista
    if(first<0 || getCount()<=last)
        throw EImproperArgument("index out bounds");

    //debe indicar un intervalo positivo
    if(first > last)
        throw EImproperArgument("last should be upper or equeal first");

    int i = first;
    int j = last;
    T central = Items[(i + j)/2];
    T aux; //para el intercambio

    do {
        while(Compare(Items[i], central) > 0)
            i++;
        while(Compare(central, Items[j]) > 0)
            j--;
        if(i<j) {
            if(Compare(Items[i], Items[j]) < 0) {
                aux = Items[i];
                Items[i] = Items[j];
                Items[j] = aux;
            }
            //NOTA: debido a que los elementos pueden ser de
            //culaquier tipo, los casos en los que dos elementos
            //tengan igual evaluación no interesa trasponerlos.
            //P.e: una lista vacía y una lista conteniendo un cero,
            //puede que sus evaluaciones seean iguales a cero.
            //Es por eso que conviene coprobar si realmente
            //hace falta o no trasponerlos.
            i++;
            j--;
        } else if(i==j) {
            i++;
            j--;
        }
    } while(i<j);

    if(first<j)
        SortDec(first, j);
    if(i<last)
        SortDec(i, last);
}
//Ordena aplicando el algoritmo quick sort al intervalo [first, last]
template <class T> void TItemsList<T>::SortInc(void)
{
    if(getCount() < 2)
        return;

    SortInc(0, getCount()-1);
}
//Ordena aplicando el algoritmo quick sort al intervalo [first, last]
template <class T> void TItemsList<T>::SortDec(void)
{
    if(getCount() < 2)
        return;

    SortDec(0, getCount()-1);
}

//inserta en orden creciente y devuelve la posición de inserción.
template <class T> int TItemsList<T>::InsertInc(const T& item)
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    if(getCount() == 0) { //si la lista está vacía
        Add(item); //inserta en la posición 0
        return 0; //retorna la posición de inserción
    } else {
        //la lista debe estar ordenada
        if(!getIncreasing())
            throw EImproperArgument("list isn't increasing");

        //Busca la posición donde debe insertarlo
        //en el intervalo  [0, getCount]
        int i=0;
        while(i<getCount() && Compare(Items[i], item)==-1)
            i++;

        //inserta el elemento en la posición i
        Insert(i, item);

        return i; //devuelve la posición de inserción
    }
}

//inserta en orden decreciente y devuelve la posición de inserción.
template <class T> int TItemsList<T>::InsertDec(const T& item)
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    if(getCount() == 0) { //si la lista está vacía
        Add(item); //inserta en la posición 0
        return 0; //retorna la posición de inserción
    } else {
        //la lista debe estar ordenada
        if(!getDecreasing())
            throw EImproperArgument("list isn't decreasing");

        //Busca la posición donde debe insertarlo
        //en el intervalo  [0, getCount]
        int i=0;
        while(i<getCount() && Compare(Items[i], item)==1)
            i++;

        //inserta el elemento en la posición i
        Insert(i, item);

        return i; //devuelve la posición de inserción
    }
}

//---------------------------------------------------------------------------
//MÉTODOS DE EVALUACIÓN:

//suma de potencias
template <class T> double TItemsList<T>::Pow(int n) const
{
    //debe haber una función de evaluación apuntada
    if(Evaluate == NULL)
        throw EImproperCall("function Evaluate not pointed");

    //suma las potencias de las evaluaciones
    double acum = 0;
    for(int i=0; i<getCount(); i++)
        acum += pow(Evaluate(Items[i]), double(n));

    return acum;
}
//producto interno con un vector
template <class T> double TItemsList<T>::Dot(const TItemsList<T> &L) const
{
    //debe haber una función de evaluación apuntada
    if(Evaluate == NULL)
        throw EImproperCall("function Evaluate not pointed");

    //la lista L debería tener una función de evaluación apuntada
    if(L.Evaluate == NULL)
        throw EImproperArgument("list L should have a evaluation function pointed");

    //los vectores deben tener la mima dimensión
    if(getCount() != L.getCount())
        throw EImproperArgument("item list L should have the same number items");

    //suma los productos de las evaluaciones
    double acum = 0;
    for(int i=0; i<getCount(); i++)
        acum += Evaluate(Items[i])*L.Evaluate(L[i]);

    return acum;
}

//---------------------------------------------------------------------------
//MÉTODOS DE ASIGNACIÓN:

//asigna el mismo valor a todas los elementos
template <class T> void TItemsList<T>::SetAll(double x)
{
    //debe haber una función de asignación apuntada
    if(Assign == NULL)
        throw EImproperCall("Function Assign not pointed");

    for(int i=0; i<getCount(); i++)
        Assign(Items[i], x);
}
//asigna valores equiespaciados en [xmin, (getCount()-1) deltax]
template <class T> void TItemsList<T>::SetEquidist(double xmin, double deltax)
{
    //debe haber una función de asignación apuntada
    if(Assign == NULL)
        throw EImproperCall("Function Assign not pointed");

    double x = xmin;
    for(int i=0; i<getCount(); i++) {
        Assign(Items[i], x);
        x += deltax;
    }
}
//asigna las particiones del segmento [xmin, xmax] en (getCount() - 1) partes
template <class T> void TItemsList<T>::SetPartitions(double xmin, double xmax)
{
    //debe haber una función de asignación apuntada
    if(Assign == NULL)
        throw EImproperCall("Function Assign not pointed");

    double delta;
    T item;

    if(getCount() > 1) {
        for(int i=0; i<getCount(); i++) {
            delta = double(i)/double(getCount()-1);
            Assign(Items[i], xmin*(1 - delta) + xmax*delta);
        }
    } else if(getCount() > 0) { //Count==1
        Assign(Items[0], (xmin + xmax)/2);
    }
}
//asigna valores con distribución uniforme en [xmin, xmax]
template <class T> void TItemsList<T>::Random(double xmin, double xmax)
{
    //debe haber una función de asignación apuntada
    if(Assign == NULL)
        throw EImproperCall("Function Assign not pointed");

    //el intervalo debe ser positivo
    if(xmin > xmax)
        throw EImproperArgument("xmin should be less or equal xmax");

    for(int i=0; i<getCount(); i++)
        Assign(Items[i], RandomUniform(xmin, xmax));
}

//suma los valores de los elementos homónimos de una lista
template <class T> TItemsList<T>& TItemsList<T>::operator+=(const TItemsList<T> &L)
{
    //debe haber una función de evaluación apuntada
    if(Evaluate == NULL)
        throw EImproperCall("function Evaluate not pointed");

    //debe haber una función de asignación apuntada
    if(Assign == NULL)
        throw EImproperCall("Function Assign not pointed");

    //la lista L debería tener una función de evaluación apuntada
    if(L.Evaluate == NULL)
        throw EImproperArgument("list L should have a evaluation function pointed");

    //debe tener el mismo nº de elementos
    if(L.getCount() != getCount())
        throw EImproperArgument("Unequal dimension");

    for(int i=0; i<getCount(); i++)
        Assign(Items[i], Evaluate(Items[i]) + L.Evaluate(L[i]));

    return *this;
}
//resta los valores de los elementos homónimos de una lista
template <class T> TItemsList<T>& TItemsList<T>::operator-=(const TItemsList<T> &L)
{
    //debe haber una función de evaluación apuntada
    if(Evaluate == NULL)
        throw EImproperCall("function Evaluate not pointed");

    //debe haber una función de asignación apuntada
    if(Assign == NULL)
        throw EImproperCall("Function Assign not pointed");

    //la lista L debería tener una función de evaluación apuntada
    if(L.Evaluate == NULL)
        throw EImproperArgument("list L should have a evaluation function pointed");

    //debe tener el mismo nº de elementos
    if(L.getCount() != getCount())
        throw EImproperArgument("Unequal dimension");

    for(int i=0; i<getCount(); i++)
        Assign(Items[i], Evaluate(Items[i]) - L.Evaluate(L[i]));

    return *this;
}
//multiplica los valores por un escalar
template <class T> TItemsList<T>& TItemsList<T>::operator*=(double t)
{
    //debe haber una función de evaluación apuntada
    if(Evaluate == NULL)
        throw EImproperCall("function Evaluate not pointed");

    //debe haber una función de asignación apuntada
    if(Assign == NULL)
        throw EImproperCall("Function Assign not pointed");

    for(int i=0; i<getCount(); i++)
        Assign(Items[i], Evaluate(Items[i])*t);

    return *this;
}
//divide los valores por un escalar
template <class T> TItemsList<T>& TItemsList<T>::operator/=(double t)
{
    //debe haber una función de evaluación apuntada
    if(Evaluate == NULL)
        throw EImproperCall("function Evaluate not pointed");

    //debe haber una función de asignación apuntada
    if(Assign == NULL)
        throw EImproperCall("Function Assign not pointed");

    //RECUERDE: excepciones que lanza la VCL al dividir por cero
    //división entera: EDivByZero
    //división real: EZeroDivide

    for(int i=0; i<getCount(); i++)
        Assign(Items[i], Evaluate(Items[i])/t);

    return *this;
}

//---------------------------------------------------------------------------

} //namespace Lists

//---------------------------------------------------------------------------
#endif // ITEMSLIST_H
