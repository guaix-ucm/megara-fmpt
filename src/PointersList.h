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
//File: PointersList.h
//Content: template for pointer list
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef POINTERSLIST_H
#define POINTERSLIST_H

#include "PointersSlideArray.h"
#include "ItemsList.h"
#include "StrPR.h"

#include <typeinfo> //std

//---------------------------------------------------------------------------

using namespace Strings; //StrTravelToEnd
using namespace std; //type_info

//namespace for lists
namespace Lists {

//---------------------------------------------------------------------------
//class TPointersList
//---------------------------------------------------------------------------

//La clase TPoinList tiene el objeto de funcionar como TItemList, pero
//con ls ventajas de usar punteros como elementos.
//Asi pues, los objetos apuntados por los punteros de una lista pertenecerán
//a la lista con todas sus consecuencias, siendo responsabildiad de la lista
//su construcción y destrucción.
//
//Para que una lista de punteros tipo T pùeda ser construida
//el tipo T deberá tener un constructor por defecto, y el operador '='
//definido.
//
//Si redimensiona una PoinList<T>, los nuevos punteros apuntarán a nuevos
//objetos construidos mediante el constructor por defecto de la clase T;
//y los objetos sobrantes serán destruidos mediante el operador delete;
//además la destrucción de la lista conlleva la destrucción de sus elementos.
//
//Si no quiere construir y destruir objetos durante el redimensionamiento,
//lo que debe construir es una ItemList<*T> en lugar de una PoinList<T>.

//En la clase TPoinList todos los métodos que puedan tener efecto sobre
//el dimensionameinto serán redefinidos para cincorporar la creción y
//destrucción de bjetos mediante los operadores new y delete.

//NOTACIÓN: al tratarse de una lista de putneros se ha decidido utilizar:
//      & en los métodos de acceso a elementos contruidos;
//      * en los métodos de adición de elementos;
//con objeto de mejorar la legibilidad y facilitar la adición de objetos
//grandes previamente contruidos.

//clase lista de punteros
template <class T> class TPointersList {
protected:
    //------------------------------------------------------------------
    //PROPIEDADES INDEPENDIENTES DE LAS FUNCIONES EXTERNAS
    //------------------------------------------------------------------

    //ELEMENTOS:

    //array deslizante de punteros a elementos
    TPointersSlideArray<T> Items;

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
    inline T& getFirst(void) const; inline void setFirst(T&);
    //accede al último elemento
    inline T& getLast(void) const; inline void setLast(T&);

    //ADVERTENCIA: los métodos SetFirst y SetLast no pueden ser incluidos
    //para la esceritura de First y Last para que puedan crearse objetos
    //TPointersList de clases abstractas.

    //PROPIEDADES DE ACCESO A PUNTEROS:

    //accede al primer puntero
    inline T* getFirstPointer(void) const;
    //accede al último puntero
    inline T* getLastPointer(void) const;

    //------------------------------------------------------------------
    //PROPIEDADES DEPENDIENTES DE LAS FUNCIONES EXTERNAS
    //------------------------------------------------------------------

    //FUNCIONES EXTERNAS:

    //función de comparación de elementos tipo T
    int ( *Compare)(const T*, const T*);
    //función de evaluación de elementos tipo T
    double ( *Evaluate)(const T*);
    //función de asignación a elementos tipo T
    void ( *Assign)(T*, double);
    //función de impresión de elemntos tipo T
    void ( *Print)(AnsiString&, const T*);
    //función de lectura de elementos tipo
    void ( *Read)(T*, const AnsiString&, int&);

    //no se especifica un método de evaluación de elemento apuntado,
    //porque si el tipo T fuera entero se confundiría

    //FUNCIONES ESTÁTICAS:

    //imprime una lista en una cadena de texto
    //en formato de fila de texto
    static void  PrintRow(AnsiString &S,
                                    const TPointersList<T> *L);
    //imprime una lista en una cadena de texto
    //en formato de columna de texto
    static void  PrintColumn(AnsiString &S,
                                       const TPointersList<T> *L);
    //lee una lista de una cadena de texto
    //en formato de fila o columna de texto
    static void  ReadSeparated(TPointersList<T> *L,
                                         const AnsiString &S, int &i);
    //lee una lista de una cadena de texto
    //en formato de fila o columna de texto
    //para los elementos de la llista previamente construidos
    static void  ReadSeparatedForBuiltItems(TPointersList<T> *L,
                                                      const AnsiString &S, int &i);

    //imprime una lista en una cadena de texto
    //en formato de lista de texto
    static void  PrintList(AnsiString &S,
                                     const TPointersList<T> *L);
    //lee una lista de una cadena de texto
    //en formato de lista de texto
    static void  ReadList(TPointersList<T> *L,
                                    const AnsiString &S, int &i);
    //lee una lista de una cadena de texto
    //en formato de lista de texto
    //para los elementos de la llista previamente construidos
    static void  ReadListForBuiltItems(TPointersList<T> *L,
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
    double getMin(void) const;
    double getMax(void) const;
    double getSum(void) const;
    double getAve(void) const;
    double getVar(void) const;

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

    //lista en formato texto con delimitadores '{', ',' y '}'
    AnsiString getText(void) const;
    void setText(const AnsiString&);
    //lista en formato fila de texto
    //elementos de la lista separados por tabuladores '\t'
    AnsiString getRowText(void) const;
    void setRowText(const AnsiString&);
    //lista en formato columna de texto
    //una lina de texto terminada en "\r\n" por cada elemento de l alista
    AnsiString getColumnText(void) const;
    void setColumnText(const AnsiString&);

    //------------------------------------------------------------------
    //MÉTODOS INDEPENDIENTES DE LAS FUNCIONES EXTERNAS:
    //------------------------------------------------------------------

    //MÉTODOS PÚBLICOS:

    //constructor general
    TPointersList(int Capacity=8,
                  int ( *Compare)(const T*, const T*)=NULL,
                  double ( *Evaluate)(const T*)=NULL,
                  void ( *Assign)(T*, double)=NULL,
                  void ( *Print)(AnsiString&, const T*)=NULL,
                  void ( *Read)(T*, const AnsiString&, int &)=NULL);

    //ADVERTENCIA: declarar los argumentos por referencia constantes es
    //necesario para evitar las advertencias cuando son pasadas variables
    //"Temporary", y no se quiere que salten las "Warnings".

    //ADVERTENCIA: no es posible contruir una TPointersList<T>,
    // donde T es una clase abstracta, de modo que las listas de
    //punteros a clase abstracta deberán ser del tipo TItemsList<T*>.

    //construye un clon de una lista de elementos basada en punteros
    TPointersList(const TPointersList*);
    TPointersList(const TPointersList&);

    //clona una lista de elementos basada en punteros
    void Clone(const TPointersList&);

    //libera la memoria ocupada por la lista de elementos
    ~TPointersList();

    //ADVERTENCIA: ~TPointersList no destruye los lementos apuntados
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

    //MÉTODOS DE ACCESO A ELEMENTOS:

    //NOTA: el intervalo de indexación corresponde a: [0, Count-1].

    //ADVERTENCIA: en listas de punteros, la sobreescritura o eliminación
    //de un item puede ocasionar el descolgamiento de un objeto

    //lectura del elemento indicado en listas no constantes
    inline T &Get(int);
    //lectura del elemento indicado en listas constantes
    inline const T &Get(int) const;
    //escritura del elemento indicado
    inline void Set(int i, const T &item);
    //lectura del elemento indicado en listas no constantes
    inline T& operator[](int i);
    //lectura del elemento indicado en listas constantes
    inline const T& operator[](int i) const;

    //NOTA: la razón de definir aquí el operador de indexación es
    //poder hacer uso de su capacidad de concatenación, es por eso
    //que debe devolver una referencia y no un puntero.

    //NOTA: el elemento en todo caso es copiado

    //MÉTODOS DE ACESO A PUNTEROS:

    //acceso de solo lectura al elemento indicado
    inline T *GetPointer(int);
    //acceso de solo lectura al elemento indicado
    inline const T *GetPointer(int) const;

    //busca el primer puntero con el valor indicado
    int SearchPointer(const T*) const;

    //MÉTODOS DE ADICIÓN:

    //añade un elemento al final de la lista
    inline void Add(const T*);
    //añade los elementos de una lista
    void Add(TPointersList<T> &List);
    //añade los elementos [i1, i2] de L en esta lista
    void Add(TPointersList<T> &List, int i1, int i2);
    //inserta un elemento al principio de la lista
    void InsertFirst(T*);
    //inserta un elemento en la posición indicada
    void Insert(int, const T*);

    //ADVERTENCIA: las listas cuyos elementos sean añadidos
    //a la lista basada en punteros deberán ser vaciadas
    //sin destruir los elementos.

    //añade los elementos de una lista
    void Add(TItemsList<T*> &List);
    //añade los elementos [i1, i2] de una lista
    void Add(TItemsList<T*> &List, int i1, int i2);

    //apila por el principio según la disciplina FIFO
    void PushFirst(T*);
    //apila por el final según la disciplina FIFO
    void PushLast(T*);

    //NOTA: dado que se trata de una lista de punteros a objetos que
    //pueden ser voluminosos, no se trata de copiar el contenido del
    //objeto sino de añadir los objetos mismos. Para evitar confusiones
    //se empelarán punteros en aquellos casos en los que se quiera
    //apuntar objetos previamente construidos, y se emplearán referencias
    //en los casos en los que se quiera copiar el contenido del objeto.

    //MÉTODOS DE BORRADO:

    //destruye el elemento y borra la posición indicada
    void Delete(int i);
    //destruye los elementos y vacía la lista
    void Clear(void);
/*    //destruye los objetos apuntados por una lista de punteros a punteros
    friend void Destroy(TPointersList<T*> &L);*/

    //borra la posición indicada sin destruir el elemento
    void DeleteWithoutDestroy(int i);
    //vacia la lista sin destruir los elementos
    void ClearWithoutDestroy(void);

    //MÉTODOS DE COPIA:

    //copia los elementos de una lista
    void Copy(const TPointersList<T>& L);
    //copia los elementos de una lista
    TPointersList<T>& operator=(const TPointersList<T>& L);

    //RECUERDE: el operador de asignación = no se hereda

    //copia los elementos del intervalo indicado de una lista
    void Copy(const TPointersList<T>& List, int i1, int i2);

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
    bool operator==(const TPointersList<T>&) const;
    bool operator!=(const TPointersList<T>&) const;
    //busca un elemento y si no lo encuentra devuelve Count
    int Search(T&) const;

    //devuelve un índice al primer elemento mayor que
    int FirstUpperTo(T&) const;
    //devuelve un índice al primer elemento mayor o igual que
    int FirstUpperOrEqualTo(T&) const;
    //devuelve un índice al primer elemento menor que
    int LastLowerTo(T&) const;
    //retorna un índice al primer elemento menor o igual que
    int LastLowerOrEqualTo(T&) const;

    //ordena de menor a mayor en el intervalo [first, last]
    void SortInc(int first, int last);
    //ordena de mayor a menor en el intervalo [first, last]
    void SortDec(int first, int last);
    //ordena de menor a mayor en el intervalo [0, Count-1]
    void SortInc(void);
    //ordena de mayor a menor en el intervalo [0, Count-1]
    void SortDec(void);
    //inserta en orden creciente
    //devuelve un índice a la posición de inserción
    int InsertInc(const T*);
    //inserta en orden decreciente
    //devuelve un índice a la posición de inserción
    int InsertDec(const T*);

    //MÉTODOS DE EVALUACIÓN:

    //suma de potencias
    double Pow(int) const;
    //producto interno con un vector
    double Dot(const TPointersList<T>&) const;

    //MÉTODOS DE ASIGNACIÓN:

    //asigna el mismo valor a todas las componentes
    void SetAll(double x);
    //asigna valores equiespaciados en [xmin, (Count-1) deltax]
    void SetEquidist(double xmin, double deltax);
    //asigna valores con distribución uniforme en [xmin, xmax]
    void Random(double xmin, double xmax);

    //suma los valores de las componentes homónimas de un vector
    TPointersList<T>& operator+=(const TPointersList<T>&);
    //resta los valores de las componentes omónimas de un vector
    TPointersList<T>& operator-=(const TPointersList<T>&);
    //multiplica los valores por un escalar
    TPointersList<T>& operator*=(double t);
    //divide los valores por un escalar
    TPointersList<T>& operator/=(double t);
};

//ADVERTENCIA: si pasa un objeto tipo TPointersList<T> por valor
//se producirá un error cuando la lista que se le pase
//tenga más de 8 elementos, ya que el ovjeto es construido
//con el número de elementos por defecto y la lista
//que se le pase es copiado byte a byte, no permitiendo
//el redimensionado automático.

//Nomenclatura recomendada:
//      lista basada en punteros de elementos tipo TItem:              TItemBPL
//      lista basada en punteros de punteros a elementos tipo TItem:   TItemBPLP

//---------------------------------------------------------------------------
//FUNCIONES ESTÁTICAS:

//imprime una lista en una cadena de texto
//en formato de fila de texto
template <class T> void  TPointersList<T>::PrintRow(AnsiString &S,
                                                              const TPointersList<T> *L)
{
    S += L->getRowText();
}
//imprime una lista en una cadena de texto
//en formato de columna de texto
template <class T> void  TPointersList<T>::PrintColumn(AnsiString &S,
                                                                 const TPointersList<T> *L)
{
    S += L->getColumnText();
}
//lee una lista de una cadena de texto
//en formato de fila o columna de texto
template <class T> void  TPointersList<T>::ReadSeparated(TPointersList<T> *L,
                                                         const AnsiString &S, int &i)
{
    //comprueba las precondiciones
    if(L->Read == NULL)
        throw EImproperCall("pointer Read should point to reading function");
    if(i<0 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a posicion in the string S");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //estado de la máquina de estados
    //      0: esperando próximo elemento o fin de cadena
    //      1: fin de cadena encontrada y lista de elementos leída con éxito
    int status = 0;

    TPointersSlideArray<T> Items;
    T *Item;

    //avanza el índice hasta el próximo caracter no separador
    StrTravelSeparatorsIfAny(S, i);

    do { //repite
        //si no ha llegado al final de la cadena
        if(i <= S.Length()) {
            try {
                //contruye el elemento
                Item = new T;
                //añade el elemento
                Items.AddLast(Item);
                //lee el elemento
                L->Read(Item, S, i);
                //avanza el índice hasta el próximo caracter no separador
                StrTravelSeparatorsIfAny(S, i);
                //si el índice indica a la posúltima posición de la cadena
                if(i > S.Length())
                    status++; //indica que se ha leido la lista con éxito

                //ADVERTENCIA: el elemento tiene que ser añadido a la lista tampón
                //tan pronto como es contruido, para que pueda ser destruido
                //en caso de error
            }
            catch(Exception& E) {
                E.Message.Insert(1, "reading separated item list in text format: ");
                throw;
            }
        } else //si ha llegado al final de la cadena
            //indica que ha llegado al final de la cadena
            status++;
        //mientras no se haya acabado la cadena y no se haya leido la lista con éxito
    } while(status < 1);

    //copia las variables tampón
    L->Clear();
    for(int j=0; j<Items.getCount(); j++)
        L->Add(Items[j]);
    Items.LibLast(Items.getCount());
}
//lee una lista de una cadena de texto
//en formato de fila o columna de texto
//para los elementos de la lista previamente construidos
template <class T> void  TPointersList<T>::ReadSeparatedForBuiltItems(TPointersList<T> *L,
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

    //estado de la máquina de estados
    //      0: esperando próximo elemento o fin de cadena
    //      1: fin de cadena encontrada y lista de elementos leída con éxito
    int status = 0;

    //lista tampón
    TPointersSlideArray<T> Items(L->Items); //clona los elementos de la lista
    //puntero a elemento
    T *Item;

    try {
        //por cada elemento de la lista
        for(int j=0; j<Items.getCount(); j++) {
            //asigna el puntero indicado para facilitar su acceso
            Item = Items[j];
            //si el puntero apunta a un elemento construido
            if(Item != NULL)
                //aplica la función de lectura al elemento
                L->Read(Item, S, i);
        }

    } catch(...) {
        throw;
    }

    //clona la lista tampón
    L->Items->Clone(Items);
    Items.LibLast(Items.getCount());
}

//imprime una lista en una cadena de texto
//en formato de lista de texto
template <class T> void  TPointersList<T>::PrintList(AnsiString &S,
                                                               const TPointersList<T> *L)
{
    //el puntero L debería apuntar a un vector construido
    if(L == NULL)
        throw EImproperArgument("pointr L should point to built vector");

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
template <class T> void  TPointersList<T>::ReadList(TPointersList<T> *L,
                                                              const AnsiString &S, int &i)
{
    //el puntero L debería apuntar a un vector construido
    if(L == NULL)
        throw EImproperArgument("pointr L should point to built vector");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caraacter por defecto.

    //la lista L debería tener una función de lectura apuntada
    if(L->Read == NULL)
        throw EImproperCall("list L should have a pointed reading function");

    //el índice i debería indicar a una posición de la adena de texto S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("index i should indicate a char of the string S");

    //COMPRUEBA EL FORMATO DE LA LISTA:

    char c; //caracter S[i]
    //estado de la máquina de estados
    //      0: esperando ' ', '\r' o '{'
    //      1: esperando ' ', '\r', elemento o '}'
    //      2: esperando ' ', '\r', ',' o '}'
    //      3: esperando ' ', '\r' o elemento
    //      4: lista convertida con éxito
    int status = 0;
    //lista tampón
    TPointersSlideArray<T> Items;

    if(i > S.Length())
        throw EImproperArgument("'{' not found");

    do {
        c = S[i];

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
                L->Read(Items.getLast(), S, i);
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
                L->Read(Items.getLast(), S, i);
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
        L->Set(i, *(Items[i]));

    //ERROR: esta última asignación debería hacerse más eficiente
    //trasladando los punteros de Items a la lista de punteros L.
}
//lee una lista de una cadena de texto
//en formato de lista de texto
//para los elementos de la lista previamente construidos
template <class T> void  TPointersList<T>::ReadListForBuiltItems(TPointersList<T> *L,
                                                                           const AnsiString &S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caraacter por defecto.

    //el puntero L debería apuntar a una lista basada en puntero construida
    if(L == NULL)
        throw EImproperArgument("pointr L should point to built based pointer list");

    //la lista L debería tener una función de lectura apuntada
    if(L->Read == NULL)
        throw EImproperCall("list L should have a pointed reading function");

    //el índice i debería indicar a una posición de la adena de texto S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("index i should indicate a char of the string S");

    //si la lista no contiene elementos
    if(L->getCount() < 1)
        return; //no lee nada

    //Aunque la cadena contuviera una lista de lementos legibles,
    //esta función solo intenta leer por cada elemento de la lista.

    //------------------------------------------------------------------
    //APLICA LA FUNCIÓN DE LECTURA A CADA ELEMENTO DE LA LISTA:

    //lista tampón
    TPointersSlideArray<T> Items(L->Items); //clona los elementos de la lista
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
        Item = Items[j];
        //atraviesa el caracter delimitador de inicio de lista
        StrTravelLabel("{", S, i);
        //aplica la función de lectura al elemento
        L->Read(Item, S, i);

        //por cada puntero adicional de la lista
        for(; j<Items.getCount(); j++) {
            //apunta el elemento indicado para facilitar su acceso
            Item = Items[j];
            //si el puntero apunta a un elemento construido
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
    Items.LibLast(Items.getCount());
}

/*//---------------------------------------------------------------------------
//FUNCIONES FRIEND:

//ADVERTENCIA: el uso del operador de indexación debe restringirse en
//las funciones friend porque está sujeto a redefinición en las clases derivadas

//destruye los objetos apuntados por los punteros de una lista
template <class T> void Destroy(TPointersList<T*> &L)
{
    //destruye los objetos de la lista
    for(int i=0; i<L.getCount(); i++) {
        //copia el puntero indicado para facilitar su acceso
        T *P = L.GetPointer(i);
        //si el puntero no es nulo
        if(P != NULL)
            delete P; //destruye el objeto
    }

    L.Clear(); //vacia la lista
}
*/
//---------------------------------------------------------------------------
//PROPIEDADES DE DIMENSIONAMIENTO:

//redimensiona el buffer
template <class T> void TPointersList<T>::setCapacity(int Capacity)
{
    try {
        //redimensiona la capacidad del array deslizante
        if(Capacity != getCapacity())
            Items.setCapacity(Capacity);
    } catch(...) {
        throw;
    }
}

//redimensiona la lista reservando o liberando elementos por la dcha
template <class T> void TPointersList<T>::setCount(int Count)
{
    try {
        //redimensiona el array deslizante
        if(Count < getCount())
            Items.DelLast(getCount() - Count);
        else if(Count > getCount())
            Items.NewLast(Count - getCount());
    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//PROPIEDADES DE ACCESO A ELEMENTOS:

template <class T> inline T& TPointersList<T>::getFirst(void) const
{
    //el número de elementos de la lista Count debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("list items number Count chould be one almost");

    return *(Items.getFirst());
}
template <class T> inline void TPointersList<T>::setFirst(T& item)
{
    //el número de elementos de la lista Count debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("list items number Count chould be one almost");

    *(Items.First) = item;
}

template <class T> inline T& TPointersList<T>::getLast(void) const
{
    //el número de elementos de la lista Count debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("list items number Count chould be one almost");

    return *(Items.getLast());
}
template <class T> inline void TPointersList<T>::setLast(T& item)
{
    //el número de elementos de la lista Count debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("list items number Count chould be one almost");

    *(Items.Last) = item;
}

//---------------------------------------------------------------------------
//PROPIEDADES DE ACCESO A PUNTEROS:

template <class T> inline T *TPointersList<T>::getFirstPointer(void) const
{
    //el número de elementos de la lista Count debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("list items number Count chould be one almost");

    return Items.getFirst();
}

template <class T> inline T *TPointersList<T>::getLastPointer(void) const
{
    //el número de elementos de la lista Count debería ser al menos uno
    if(getCount() < 1)
        throw EImproperCall("list items number Count chould be one almost");

    return Items.getLast();
}

//---------------------------------------------------------------------------
//PROPIEDADES DE COMPARACIÓN:

template <class T> int TPointersList<T>::getFirstMin(void) const
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

template <class T> int TPointersList<T>::getLastMax(void) const
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

//devuelve un índice al primer elemento mayor que
template <class T> int TPointersList<T>::FirstUpperTo(T &item) const
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

    //si no lo encuentra devuelve Count
    return i;
}

//devuelve un índice al primer elemento mayor o igual que
template <class T> int TPointersList<T>::FirstUpperOrEqualTo(T &item) const
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

    //si no lo encuentra devuelve Count
    return i;
}

//devuelve un índice al primer elemento menor que
template <class T> int TPointersList<T>::LastLowerTo(T &item) const
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
template <class T> int TPointersList<T>::LastLowerOrEqualTo(T &item) const
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

//comprueba si está ordenado de menor a mayor
template <class T> bool TPointersList<T>::getIncreasing(void) const
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

template <class T> bool TPointersList<T>::getDecreasing(void) const
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

template <class T> double TPointersList<T>::getMin(void) const
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

template <class T> double TPointersList<T>::getMax(void) const
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

template <class T> double TPointersList<T>::getSum(void) const
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

template <class T> double TPointersList<T>::getAve(void) const
{
    //la lista no debe estar vacía
    if(getCount() < 1)
        throw EImproperCall("Empty list");

    return getSum()/getCount();
}

template <class T> double TPointersList<T>::getVar(void) const
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

template <class T> AnsiString TPointersList<T>::getCapacityText(void) const
{
    return IntToStr(getCapacity());
}
template <class T> void TPointersList<T>::setCapacityText(const AnsiString& S)
{
    try {
        setCapacity(StrToInt(S));
    } catch(...) {
        throw;
    }
}
template <class T> AnsiString TPointersList<T>::getCountText(void) const
{
    return IntToStr(getCount());
}
template <class T> void TPointersList<T>::setCountText(const AnsiString& S)
{
    try {
        setCount(StrToInt(S));
    } catch(...) {
        throw;
    }
}
template <class T> AnsiString TPointersList<T>::getItemsText(int i) const
{
    //debería haber una función de imrpesión apuntada
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
template <class T> void TPointersList<T>::setItemsText(int i, const AnsiString &S)
{
    try {
        int j = 1;
        Read(Items[i], S, j);
    } catch(...) {
        throw;
    }
}

//traduce de lista a formato texto
template <class T> AnsiString TPointersList<T>::getText(void) const
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
template <class T> void TPointersList<T>::setText(const AnsiString &S)
{
    try {
        //lee la lista desde la primera posición
        int i = 1;
        ReadList(this, S, i);

        //busca texto inesperado
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text translating stirng to list");

    } catch(Exception& E) {
        E.Message.Insert(1, "translating string to list: ");
        throw;
    }
}

//traduce de lista a formato columna de texto
template <class T> AnsiString TPointersList<T>::getRowText(void) const
{
    //el puntero Print deberí aapuntar a una función de impresión
    if(Print == NULL)
        throw EImproperCall("pointer Print should point to print function");

    AnsiString S;

    for(int i=0; i<getCount(); i++) {
        Print(S, Items[i]);
        S += "\t";
    }

    return S;
}

//traduce de formato texto a lista
template <class T> void TPointersList<T>::setRowText(const AnsiString &S)
{
    try {
        int i = 1;
        ReadRow(this, S, i);
    } catch(...) {
        throw;
    }
}
//traduce de lista a formato columna de texto
template <class T> AnsiString TPointersList<T>::getColumnText(void) const
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
template <class T> void TPointersList<T>::setColumnText(const AnsiString &S)
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
    }
    catch(Exception& E) {
        unsigned int row, col;
        strPositionToCoordinates(row, col, S.str, i-1);
        E.Message.Insert(1, "setting column text in row "+inttostr(row)+" and column "+inttostr(col)+": ");
        throw;
    }
}

//---------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

template <class T> TPointersList<T>::TPointersList(int Capacity,
                                                   int ( *t_Compare)(const T*, const T*),
                                                   double ( *t_Evaluate)(const T*),
                                                   void ( *t_Assign)(T*, double),
                                                   void ( *t_Print)(AnsiString&, const T*),
                                                   void ( *t_Read)(T*, const AnsiString&, int&)) :
    Items(Capacity, 0)
{
    Compare = t_Compare;
    Evaluate = t_Evaluate;
    Assign = t_Assign;
    Print = t_Print;
    Read = t_Read;
}

//construye un clon de una lista de elementos basada en punteros
template <class T> TPointersList<T>::TPointersList(const TPointersList *L)
//se construye aquí el array deslizante por defecto
{
    //check the precondition
    if(L == NULL)
        throw EImproperArgument("pointer L should point to built list");

    //clona el array deslizante
    Items.Clone(L->Items);

    //apunta a las mismas funciones externas
    Compare = L->Compare;
    Evaluate = L->Evaluate;
    Assign = L->Assign;
    Print = L->Print;
    Read = L->Read;
}
template <class T> TPointersList<T>::TPointersList(const TPointersList& L)
//se construye aquí el array deslizante por defecto
{
    //clona el array deslizante
    Items.Clone(L.Items);

    //apunta a las mismas funciones externas
    Compare = L.Compare;
    Evaluate = L.Evaluate;
    Assign = L.Assign;
    Print = L.Print;
    Read = L.Read;
}

//clona una lista deelementos basada en punteros
template <class T> void TPointersList<T>::Clone(const TPointersList<T>& L)
{
    //clona el array deslizante
    Items.Clone(L.Items);

    //apunta a las mismas funciones externas
    Compare = L.Compare;
    Evaluate = L.Evaluate;
    Assign = L.Assign;
    Print = L.Print;
    Read = L.Read;
}

//libera la memoria ocupada por una lista de elementos
template <class T> TPointersList<T>::~TPointersList()
{
}

//---------------------------------------------------------------------------
//MÉTODOS DE DIMENSIONAMIENTO:

//añade elementos al principio de la lista
template <class T> inline void TPointersList<T>::NewFirst(int N)
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
template <class T> inline void TPointersList<T>::NewLast(int N)
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
template <class T> inline void TPointersList<T>::DelFirst(int N)
{
    //el número de elementos a borrar N debería estar en [1, Count]
    if(N<1 || getCount()<N)
        throw EImproperArgument("number items to delete should be int [1, Count]");

    Items.DelFirst(N);
}

//borra elementos al final de la lista
template <class T> inline void TPointersList<T>::DelLast(int N)
{
    //el número de elementos a borrar N debería estar en [1, Count]
    if(N<1 || getCount()<N)
        throw EImproperArgument("number items to delete should be int [1, Count]");

    Items.DelLast(N);
}

//---------------------------------------------------------------------------
//MÉTODOS DE ACCESO A ELEMENTOS:

//lectura del elemento indicado en listas no constantes
template <class T> inline T &TPointersList<T>::Get(int i)
{
    //el elemento indicado debe existir
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i out bounds");

    return *(Items[i]);
}
//lectura del elemento indicado en listas constantes
template <class T> inline const T &TPointersList<T>::Get(int i) const
{
    //el elemento indicado debe existir
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i out bounds");

    return *(Items[i]);
}
//escritura del elemento indicado
template <class T> inline void TPointersList<T>::Set(int i, const T &item)
{
    //i debe indicar un elemento existente
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i out bounds");

    *(Items[i]) = item;
}
//lectura del elemento indicado en listas no constantes
template <class T> inline T& TPointersList<T>::operator[](int i)
{
    //i debe indicar un elemento existente
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i out bounds");

    return *(Items[i]);
}
//lectura del elemento indicado en listas constantes
template <class T> inline const T& TPointersList<T>::operator[](int i) const
{
    //i debe indicar un elemento existente
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i out bounds");

    return *(Items[i]);
}

//---------------------------------------------------------------------------
//MÉTODOS DE ACCESO A PUNTEROS:

//Lectura del elemento indicado
template <class T> inline T *TPointersList<T>::GetPointer(int i)
{
    //el elemento indicado debe existir
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i out bounds");

    return Items.Get(i);
}
//Lectura del elemento indicado
template <class T> inline const T *TPointersList<T>::GetPointer(int i) const
{
    //el elemento indicado debe existir
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i out bounds");

    return Items.Get(i);
}

//busca el primer puntero con el valor indicado
template <class T> int TPointersList<T>::SearchPointer(const T *item) const
{
    int i = 0;
    while(i<getCount() && Items[i]!=item)
        i++;

    return i;
}

//---------------------------------------------------------------------------
//MÉTODOS DE EVALUACIÓN:

//suma de potencias
template <class T> double TPointersList<T>::Pow(int n) const
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
template <class T> double TPointersList<T>::Dot(const TPointersList<T> &L) const
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
        acum += Evaluate(Items[i])*L.Evaluate(L.Items[i]);

    return acum;
}

//---------------------------------------------------------------------------
//MÉTODOS DE ADICIÓN:

//Añade un elemento
template <class T> inline void TPointersList<T>::Add(const T *item)
{
    try {
        //añade el elemento
        Items.AddLast(item);
    } catch(...) {
        throw;
    }
}

//añade una lista
template <class T> void TPointersList<T>::Add(TPointersList<T>& L)
{
    //NOTA: se supone que todos los punteros de la lista
    //apuntan a elementos onstruidos, ya que la lista no
    //deja añadir punteros a nulo.

    //si la lista está vacía no hace nada
    if(L.getCount() < 1)
        return;

    //añade los elementos
    for(int i=0; i<L.getCount(); i++)
        Items.AddLast(L.Items[i]);
}

//añade un elemento al principio de la lista
template <class T> void TPointersList<T>::InsertFirst(T *item)
{
    try {
        //añade el elemento
        Items.AddFirst(item);
    } catch(...) {
        throw;
    }
}

//añade los elementos del intervalo [i1, i2] de una lista
template <class T> void TPointersList<T>::Add(TPointersList<T>& L,
                                              int i1, int i2)
{
    //el índice i1 debe indicar un lemento de la lista L
    if(i1<0  || L.getCount()<=i1)
        throw EImproperArgument("index i1 should indicate an iten in the list L");

    //el índice i2 debe indicar un lemento de la lista L
    if(i2<0  || L.getCount()<=i2)
        throw EImproperArgument("index i2 should indicate an iten in the list L");

    //el intervalo [i1, i2] debe ser no negativo
    if(i1 > i2)
        throw EImproperArgument("interval [i1, i2] should be nonnegative");

    //añade los elementos
    for(int i=i1; i<=i2; i++)
        Items.AddLast(L.Items[i]);
}

//inserta un elemento en la posición indicada
template <class T> void TPointersList<T>::Insert(int i, const T *item)
{
    try {
        Items.Insert(i, item);
    } catch(...) {
        throw;
    }
}

//añade los elementos de una lista
template <class T> void TPointersList<T>::Add(TItemsList<T*> &List)
{
    //por cada puntero de la lista
    for(int i=0; i<List.getCount(); i++)
        //añade el puntero al final de esta lista
        Items.AddLast(List[i]);
}
//añade los elementos del intervalo [i1, i2] de una lista
template <class T> void TPointersList<T>::Add(TItemsList<T*> &L, int i1, int i2)
{
    //el índice i1 debe indicar un lemento de la lista L
    if(i1<0  || L.getCount()<=i1)
        throw EImproperArgument("index i1 should indicate an iten in the list L");

    //el índice i2 debe indicar un lemento de la lista L
    if(i2<0  || L.getCount()<=i2)
        throw EImproperArgument("index i2 should indicate an iten in the list L");

    //el intervalo [i1, i2] debe ser no negativo
    if(i1 > i2)
        throw EImproperArgument("interval [i1, i2] should be nonnegative");

    //añade los elementos
    for(int i=i1; i<=i2; i++)
        Items.AddLast(L.Items[i]);
}

//apila por el principio según la disciplina FIFO
template <class T> void TPointersList<T>::PushFirst(T *item)
{
    //el puntero item debería apuntar a un elemento construido
    if(item == NULL)
        throw EImproperArgument("pointer item should point to built item");

    //la lista no debe estar vacía
    if(getCount() < 1)
        throw EImproperCall("Empty list");

    //destruye el último objeto
    delete Items.Last;
    //desliza el intervalo de reserva del array
    Items.SlideLeft();
    //apunta el elemento en el array
    Items.First = item;
}

//apila por el final según la disciplina FIFO
template <class T> void TPointersList<T>::PushLast(T *item)
{
    //el puntero item debería apuntar a un elemento construido
    if(item == NULL)
        throw EImproperArgument("pointer item should point to built item");

    //la lista no debe estar vacía
    if(getCount() < 1)
        throw EImproperCall("Empty list");

    //destruye el primer elemento
    delete Items.First;
    //desliza el intervalo de reserva del array
    Items.SlideRight();
    //apunta el elemento en el array
    Items.Last = item;
}

//---------------------------------------------------------------------------
//MÉTODOS DE BORRADO:

//destruye el elemento y borra la posición indicada
template <class T> void TPointersList<T>::Delete(int i)
{
    //el elemento indicado debe existir
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i out bounds");

    //guarda el puntero al elemento indicado
    T *Item = Items[i];

    //el elemento está en la primera mitad
    if(i < getCount()/2) {
        //Desplaza los elementos
        for(int j=i; j>0; j--)
            Items[j] = Items[j-1];
        //traslada elelemento indicado a la primera posición
        Items.setFirst(Item);
        //libera el elemento
        Items.DelFirst();
    } else { //el elemento está en la segunda mitad
        //Desplaza los elementos
        for(int j=i+1; j<getCount(); j++)
            Items[j-1] = Items[j];
        //traslada el elemento indicado a la última posición
        Items.setLast(Item);
        //libera el elemento
        Items.DelLast();
    }

    //ADVERTENCIA: la dirección del elemento indicado debe ser trasladada
    //a la posición que se va a liberar, ya que TPointersSlideArray
    //destruye el elemento indicado.
}

//destruye los elementos y vacia la lista
template <class T> void TPointersList<T>::Clear(void)
{
    Items.DelLast(getCount());
}

//borra la posición indicada sin destruir el elemento
template <class T> void TPointersList<T>::DeleteWithoutDestroy(int i)
{
    //el índice i debe indicar a un elemento de la lista
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i should indicate a item in the list");

    if(i < getCount()/2) { //el elemento está en la primera mitad
        //Desplaza los elementos
        for(int j=i; j>0; j--)
            Items[j] = Items[j-1];
        //libera el elemento
        Items.LibFirst();
    } else { //el elemento está en la segunda mitad
        //Desplaza los elementos
        for(int j=i+1; j<getCount(); j++)
            Items[j-1] = Items[j];
        //libera el elemento
        Items.LibLast();
    }
}
//vacia la lista sin destruir los elementos
template <class T> void TPointersList<T>::ClearWithoutDestroy(void)
{
    Items.LibLast(getCount());
}

//---------------------------------------------------------------------------
//MÉTODOS DE COPIA:

//copia los elemenetos de una lista
template <class T> void TPointersList<T>::Copy(const TPointersList<T> &L)
{
    //redimensiona la lista
    if(L.getCount() < getCount())
        Items.DelLast(getCount() - L.getCount());
    else if(L.getCount() > getCount())
        Items.NewLast(L.getCount() - getCount());

    //copia los elementos
    for(int i=0; i<getCount(); i++)
        *(Items[i]) = L[i];
}
//copia los elemeentos de una lista
template <class T> TPointersList<T>& TPointersList<T>::operator=(
        const TPointersList<T> &L)
{
    //redimensiona la lista
    if(L.getCount() < getCount())
        Items.DelLast(getCount() - L.getCount());
    else if(L.getCount() > getCount())
        Items.NewLast(L.getCount() - getCount());

    //copia los elementos
    for(int i=0; i<getCount(); i++)
        *(Items[i]) = L[i];

    return *this;
}

//Copia los elementos [i1, i2] de una lista
template <class T> void TPointersList<T>::Copy(const TPointersList<T>& L,
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
        *(Items[i]) = L[i1++];
}

//---------------------------------------------------------------------------
//MÉTODOS DE TRANSPOSICIÓN:

//invierte el orden de los elementos de la lista
template <class T> void TPointersList<T>::Invert(void)
{
    //Solo se invierten listas de dos o más elementos
    if(getCount() > 1) {
        int i1 = 0; //índice desde el comienzo de la lista
        int i2 = getCount() - 1; //índice desde el final de la lista
        T *aux; //para el intercambio

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
template <class T> void TPointersList<T>::Randomize(void)
{
    //Solo se desordenan Listes de dos o más elementos
    if(getCount() > 1) {
        int j; //índice a la posición de intercambio
        T *aux; //para el intercambio

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
template <class T> bool TPointersList<T>::operator==(const TPointersList<T> &L) const
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    //para ser iguales deben tener el mismo nº de elementos
    if(L.getCount() != getCount())
        return false;

    //compara los elementos homónimos de las listas
    for(int i=0; i<getCount(); i++)
        if(Compare(L.Items[i], Items[i]) != 0)
            return false;

    return true;
}

template <class T> bool TPointersList<T>::operator!=(const TPointersList<T> &L) const
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    //para ser iguales deben tener el mismo nº de elementos
    if(L.getCount() != getCount())
        return true;

    //compara los elementos homónimos de las listas
    for(int i=0; i<getCount(); i++)
        if(Compare(L.Items[i], Items[i]) != 0)
            return true;

    return false;
}

//busca un elemento y si no lo encuentra devuelve Count
template <class T> int TPointersList<T>::Search(T &item) const
{
    //debe haber una función de comparación apuntada
    if(Compare == NULL)
        throw EImproperCall("function Compare not pointed");

    int i = 0;
    while(i<getCount() && Compare(Items[i], &item))
        i++;

    return i;
}

//Ordena aplicando el algoritmo quick sort al intervalo [first, last]
template <class T> void TPointersList<T>::SortInc(int first, int last)
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
    T *central = Items[(i + j)/2];
    T *aux; //para el intercambio

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
template <class T> void TPointersList<T>::SortDec(int first, int last)
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
    T *central = Items[(i + j)/2];
    T *aux; //para el intercambio

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
template <class T> void TPointersList<T>::SortInc(void)
{
    if(getCount() < 2)
        return;

    SortInc(0, getCount()-1);
}
//Ordena aplicando el algoritmo quick sort al intervalo [first, last]
template <class T> void TPointersList<T>::SortDec(void)
{
    if(getCount() < 2)
        return;

    SortDec(0, getCount()-1);
}

//inserta en orden creciente y devuelve la posición de inserción.
template <class T> int TPointersList<T>::InsertInc(const T *item)
{
    //el puntero item debería apuntar a un elemento construido
    if(item == NULL)
        throw EImproperArgument("pointer item should point to built item");

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
        //en el intervalo  [0, Count]
        int i=0;
        while(i<getCount() && Compare(Items[i], item)==-1)
            i++;

        //inserta el elemento en la posición i
        Insert(i, item);

        return i; //devuelve la posición de inserción
    }
}

//inserta en orden decreciente y devuelve la posición de inserción.
template <class T> int TPointersList<T>::InsertDec(const T *item)
{
    //el puntero item debería apuntar a un elemento construido
    if(item == NULL)
        throw EImproperArgument("pointer item should point to built item");

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
        //en el intervalo  [0, Count]
        int i=0;
        while(i<getCount() && Compare(Items[i], item)==1)
            i++;

        //inserta el elemento en la posición i
        Insert(i, item);

        return i; //devuelve la posición de inserción
    }
}

//---------------------------------------------------------------------------
//MÉTODOS DE ASIGNACIÓN:

//asigna el mismo valor a todas los elementos
template <class T> void TPointersList<T>::SetAll(double x)
{
    //debe haber una función de asignación apuntada
    if(Assign == NULL)
        throw EImproperCall("Function Assign not pointed");

    for(int i=0; i<getCount(); i++)
        Assign(Items[i], x);
}
//asigna valores equiespaciados en [xmin, (Count-1) deltax]
template <class T> void TPointersList<T>::SetEquidist(double xmin, double deltax)
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
//asigna valores con distribución uniforme en [xmin, xmax]
template <class T> void TPointersList<T>::Random(double xmin, double xmax)
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
template <class T> TPointersList<T>& TPointersList<T>::operator+=(const TPointersList<T> &L)
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
        Assign(Items[i], Evaluate(Items[i]) + L.Evaluate(L.Items[i]));

    return *this;
}
//resta los valores de los elementos homónimos de una lista
template <class T> TPointersList<T>& TPointersList<T>::operator-=(const TPointersList<T> &L)
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
        Assign(Items[i], Evaluate(Items[i]) - L.Evaluate(L.Items[i]));

    return *this;
}
//multiplica los valores por un escalar
template <class T> TPointersList<T>& TPointersList<T>::operator*=(double t)
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
template <class T> TPointersList<T>& TPointersList<T>::operator/=(double t)
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









































/*
template <class T> class TPoinList : public TItemList<T*> {
protected:
    //MÉTODOS DE REDIMENSIONAMIENTO:

        int GetCount(void) const {return Items.getCount();}
        void SetCount(int);

        //MÉTODOS DE ACCESO A ELEMENTOS (EN VEZ DE A PUNTEROS):

        inline T& GetFirstPointed(void) const;
        inline void SetFirstPointed(T item) {First = &item;}
        inline T& GetLastPointed(void) const;
        inline void SetLastPointed(T item) {Last = &item;}

        //PROPIEDADES EN FORMATO TEXTO:

        AnsiString GetListText(void) const; void SetListText(const AnsiString&);
        AnsiString GetColumnText(void) const;
                void SetColumnText(const AnsiString&);

public:
    //construye una lista de punteros con la capacidad inicial indicada
    TPoinList(int _Capacity=8,
                int ( *_Compare)(const T*, const T*)=NULL,
                double ( *_Evaluate)(const T*)=NULL,
                void ( *_Assign)(T*, double)=NULL,
            void ( *_Print)(AnsiString&, const T*)=NULL,
                void ( *_Read)(T*, const AnsiString&, int &i)=NULL) :
                TItemList<T*>(_Capacity,
                        _Compare, _Evaluate, _Assign, _Print, _Read) {;}
        //destruye los elementos
        ~TPoinList() {Destroy();}

        //-------------------------------------------------------------------
        //MÉTODOS DE REDIMENSIONAMIENTO:

        //nº de elementos de la lista
        //construye o destruye elementos al final de la lista
        __property int Count = {read=GetCount, write=SetCount};
        //construye elementos al principio de la lista
        void NewFirst(int n=1);
        //construye elementos al final de la lista
        void NewLast(int n=1);
        //destruye elementos al principio de l alista
        void DelFirst(int n=1);
        //destruye elementos al final de la lista
        void DelLast(int n=1);

        //destruye todos los elementos y vacia la lista
        void Destroy(void);

        //-------------------------------------------------------------------
        //MÉTODOS QUE CONLLEVAN EL REDIMENSIONAMIENTO:

        //copia los elementos de la lista L
        //los apuntados por los punteros de la lista
        void Copy(const TPoinList<T>& L);
        //copia los elementos de la lista L
        //los apuntados por los punteros de la lista
        TPoinList<T>& operator=(const TPoinList<T>&);
        //copia los elementos del intervalo [i1, 12] de la lista L
        //los apuntados por los punteros de la lista
        void Copy(const TPoinList<T>& L, int i1, int i2);

        //-------------------------------------------------------------------
        //MÉTODOS DE ACCESO A ELEMENTOS (EN VEZ DE A PUNTEROS):

        //RECUERDE: la redefinición de métodos miembro
        //enmascara los métodos sobrecargados

        //añade un elemento al final
        inline void AddPointed(T item) {Add(&item);}
        //inserta un elemento en la posición indicada
        void InsertPointed(int i, T item) {Insert(i, &item);}

        //sobrecarga el operador de indexación para que acceda al elemento
        //en lugar de a su puntero, de tal modo que las listas de punteros
        //a listas resulten más fáciles de indexar

        //acceso al elemento indicado
        inline T& GetPointed(int i) const;
        //acceso al elemento indicado mediante el operador de indexación
        inline T& operator[](int i);
        //si la lista es constante, se invocará a esta función que devuelve
        //una referencia a constante impidiendo su modificación
        inline const T& operator[](int i) const;

        //primer elemento de la lista
        __property T FirstPointed = {read=GetFirstPointed,
                write=SetFirstPointed};
        //último elemento de la lista
        __property T LastPointed = {read=GetLastPointed, write=SetLastPointed};

        //ADVERTENCIA: las funciones de comparación y búsqueda aquí heredadas,
        //operan sobre los valores de los punteros y no sobre los elementos
        //por ellos apuntados

        //PROPIEDADES EN FORMATO TEXTO:

    //lista en formato texto con delimitadores '{', ',' y '}'
        __property AnsiString ListText = {read=GetListText, write=SetListText};
        //lista en formato columna de texto
        //una lina de texto terminada en "\r\n" por cada elemento de l alista
        __property AnsiString ColumnText = {read=GetColumnText,
                write=SetColumnText};
};
//---------------------------------------------------------------------------
//Métodos de TPoinList<T>
//---------------------------------------------------------------------------
//construye o destruye elementos al final de la lista
template <class T> void TPoinList<T>::SetCount(int _Count)
{
        //no debe ser negativo
        if(_Count < 0)
                throw EImproperArgument("Count less zero");

        //redimensiona el array deslizante
        if(_Count < getCount()) { //si disminuye
                //destruye los elementos sobrantes
                for(int i=_Count; i<getCount(); i++)
                        delete Items[i];
                //libera los punteros sobrantes
                Items.DelLast(Count - _Count);
        } else if(_Count > getCount()) { //si aumenta
                int i = Count; //indica a la primera nueva posición
                //reserva los nuevos punteros
                Items.NewLast(_Count - Count);
                //construye los nuevos elementos
                for(; i<getCount(); i++)
                        Items[i] = new T;
        }
}
//construye n elementos al principio de la lista
template <class T> inline void TPoinList<T>::NewFirst(int n)
{
    //debe construir almenos un elemento
        if(n < 1)
            throw EImproperArgument("n should be upper zero");

    Items.NewFirst(n); //reserva los punteros a elementos
        //construye los elementos
        for(int i=0; i<n; i++)
            Items[i] = new T;
}
//construye n elementos al final de la lista
template <class T> inline void TPoinList<T>::NewLast(int n)
{
    //debe construir almenos un elemento
        if(n < 1)
            throw EImproperArgument("n should be upper zero");

        int i = Count; //captura el índice al pirmer nuevo elemento
    Items.NewLast(n); //reserva los punteros a elementos
        //construye los elementos
        for(; i<getCount(); i++)
            Items[Count] = new T;
}
//destruye n elementos al principio de la lista
template <class T> inline void TPoinList<T>::DelFirst(int n)
{
    //debe destruir elementos de la lista
        if(n<1 || Count<n)
                throw EImproperArgument("n should be in [1, Count]");

        //destruye los n primeros elementos
        for(int i=0; i<n; i++)
            if(Items[i] != NULL)
                        delete Items[i];
        Items.DelFirst(n); //libera los n primeros punteros
}
//destruye n elementos al final de la lista
template <class T> inline void TPoinList<T>::DelLast(int n)
{
    //debe borrar elementos reservados
        if(n<1 || Count<n)
                throw EImproperArgument("n should be in [1, Count]");

        //destruye los n últimos elementos
        for(int i=Count-n; i<getCount(); i++)
            if(Items[i] != NULL)
                        delete Items[i];
        Items.DelLast(n); //libera los n últimos punteros
}
//destruye todos los elementos y vacia la lista
template <class T> void TPoinList<T>::Destroy(void)
{
    //destruye los elementos
    for(int i=0; i<getCount(); i++)
            if(Items[i] != NULL)
                delete Items[i];

        //libera las posiciones de la lista
        Items.DelLast(Count);
}
//---------------------------------------------------------------------------
//copia los elementos de la lista L
//los apuntados por los punteros de la lista
template <class T> void TPoinList<T>::Copy(const TPoinList<T>& L)
{
        Destroy(); //destruye todos los elementos de la lista
        Items.NewLast(L.getCount() - Items.getCount()); //redimensiona la lista

        //construye y copia los elementos no nulos
        for(int i=0; i<getCount(); i++)
                if(L.Get(i) != NULL) {
                        Items[i] = new T;
                    *(Items[i]) = L.GetPointed(i);
                }
}
//copia los elementos de la lista L
//los apuntados por los punteros de la lista
template <class T> TPoinList<T>& TPoinList<T>::operator=(const TPoinList<T> &L)
{
        Destroy(); //destruye todos los elementos de la lista
        Items.getCount() = L.getCount(); //redimensiona la lista

        //construye y copia los elementos no nulos
        for(int i=0; i<getCount(); i++)
                if(IL.Get(i) != NULL) {
                        Items[i] = new T;
                    *(Items[i]) = L.GetPointed(i);
                }

        //devuelve una referencia a esta lista para
        //permitir la anidación de asignaciones
        return *this;
}
//copia los elementos del intervalo [i1, 12] de la lista L
//los apuntados por los punteros de la lista
template <class T> void TPoinList<T>::Copy(const TPoinList<T>& L,
        int i1, int i2)
{
    //los indices i1 e i2 deben apuntar a elementos de L
        if(i1<0  || L.getCount()<=i1 || i2<0  || L.getCount()<=i2)
                throw EImproperArgument("index out bounds");

        //el intervalo [i1, i2] debe ser positivo
        if(i1 > i2)
                throw EImproperArgument("i2 should be upper or equal i1");

        Destroy(); //destruye todos los elementos de la lista
        Items.getCount() = i2-i1+1; //redimensiona la lista

        //construye y copia los elementos no nulos
        for(int i=0; i<getCount(); i++) {
            if(Items[i] != NULL) {
                        Items[i] = new T;
                        *(Items[i]) = L[i1];
                }
                i1++;
        }
}
//---------------------------------------------------------------------------
//devuelve el elemnto apuntado por el primer puntero de la lista
template <class T> inline T& TPoinList<T>::GetFirstPointed(void) const
{
        //debe contener almenos un elemento
        if(getCount() < 1)
                throw EImproperCall("there isn't First item");

        //la posición indicada debe apuntar a un elemento
    if(First == NULL)
                throw EImproperCall("Null pointer First");

    return *(First);
}
//devuelve el elemnto apuntado por el último puntero de la lista
template <class T> inline T& TPoinList<T>::GetLastPointed(void) const
{
        //debe contener almenos un elemento
        if(getCount() < 1)
                throw EImproperCall("there isn't Last item");

        //la posición indicada debe apuntar a un elemento
    if(Last == NULL)
                throw EImproperCall("Null pointer Last");

    return *(Last);
}
//---------------------------------------------------------------------------
//acceso al elemento indicado
template <class T> inline T& TPoinList<T>::GetPointed(int i) const
{
        //debe indicar un elemento de la lista
        if(i < 0 || Count<=i)
                throw EImproperArgument("index i out bounds");

        //la posición indicada debe apuntar a un elemento
    if(Items[i] == NULL)
                throw EImproperCall("Null pointer indexed item");

    return *(Items[i]); //devuelve una referecia al elemento indicado
}
//acceso al elemento indicado mediante el operador de indexación
template <class T> T& TPoinList<T>::operator [](int i)
{
        //debe indicar un elemento de la lista
        if(i < 0 || Count<=i)
                throw EImproperArgument("index i out bounds");

        //la posición indicada debe apuntar a un elemento
    if(Items[i] == NULL)
                throw EImproperCall("Null pointer");

    return *(Items[i]); //devuelve una referencia al elemento indicado
}
//si la lista es constante, se invocará a esta función que devuelve
//una referencia a constante impidiendo su modificación
template <class T> const T& TPoinList<T>::operator[](int i) const
{
        //debe indicar un elemento de la lista
        if(i < 0 || Count<=i)
                throw EImproperArgument("index i out bounds");

        //la posición indicada debe apuntar a un elemento
    if(Items[i] == NULL)
                throw EImproperCall("Null pointer");

    return *(Items[i]); //devuelve una referecia al elemento indicado
}

//---------------------------------------------------------------------------
//PROPIEDADES EN FORMATO TEXTO:

//traduce de lista a formato texto
template <class T> AnsiString TPoinList<T>::GetListText(void) const
{
        //el puntero Print deberí aapuntar a una función de impresión
        if(Print == NULL)
                throw EImproperCall("pointer Print should point to print function");

        AnsiString S = "{";

        if(getCount() > 0) {
                Print(S, Items.First);
                for(int i=1; i<getCount(); i++) {
                        S += ", ";
                        Print(S, Items[i]);
                }
        }
        S += "}";

        return S;
}

//ADVERTENCIA: en listas de punteros los elementos pueden ser punteros vacíos
//por lo que la función SetText pasaría un "puntero basura" a la función Read
//y por consiguiente se produciría una violación de acceso a memoria

//traduce de formato texto a lista
template <class T> void TPoinList<T>::SetListText(const AnsiString &S)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caraacter por defecto.

        //el puntero Read debería apuntar a una función de lectura
        if(Read == NULL)
                throw EImproperCall("pointer Read should point to reading function");

        //COMPRUEBA EL FORMATO DE LA LISTA:

        int i = 1; //índice a caracteres de S
        char c; //caracter S[i]
        //estado de la máquina de estados
        //      0: esperando ' ' o '{'
        //      1: esperando ' ', elemento o '}'
        //      2: esperando ' ', ',' o '}'
        //      3: esperando ' ' o elemento
        //      4: lista convertida con éxito
        int status = 0;
        //lista tampón
        TPoinList<T> _Items;

        if(i > S.Length())
                throw EImproperArgument("'{' not found");

        do {
                c = S[i];

                switch(status) {
                        case 0: //esperando ' ' o '{'
                                switch(c) {
                                        case ' ':
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("'{' not found");
                                                break;
                                        case '{':
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("Item or '}' not found");
                                                status = 1;
                                                break;
                                        default:
                                                throw EImproperArgument("'"+AnsiString(c)+"' should be ' ' or '{'");
                                }
                                break;
                        case 1: //esperando ' ', elemento o '}'
                                switch(c) {
                                        case ' ':
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("'}' not found");
                                                break;
                                        case '}':
                                                status = 4;
                                                break;
                                        default:
                                                _Items.NewLast();
                                                Read(_Items.LastPointed, S, i);
                                                if(i > S.Length())
                                                        throw EImproperArgument("',' or '}' not found");
                                                status = 2;
                                }
                                break;
                        case 2: //esperando ' ', ',' o '}'
                                switch(c) {
                                        case ' ':
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("',' or '}' not found");
                                                break;
                                        case ',':
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("Item not found");
                                                status = 3;
                                                break;
                                        case '}':
                                                status = 4;
                                                break;
                                        default:
                                                throw EImproperArgument("'"+AnsiString(c)+"' should be ' ', ',' or '}'");
                                }
                                break;
                        case 3: //esperando ' ' o elemento
                                switch(c) {
                                        case ' ':
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("Item not found");
                                                break;
                                        default:
                                                _Items.NewLast();
                                                Read(_Items.LastPointed, S, i);
                                                if(i > S.Length())
                                                        throw EImproperArgument("',' or '}' not found");
                                                status = 2;
                                }
                                break;
                }
        //mientras la lista no esté convertida con éxito
        } while(status<4);

        //ASIGNA LOS ELEMENTOS:

        Count = _Items.getCount();
        for(i=0; i<_Items.getCount(); i++)
                Items[i] = _Items[i];

}

//traduce de lista a formato columna de texto
template <class T> AnsiString TPoinList<T>::GetColumnText(void) const
{
        //el puntero Print deberí aapuntar a una función de impresión
        if(Print == NULL)
                throw EImproperCall("pointer Print should point to print function");

        AnsiString S;

        for(int i=0; i<getCount(); i++) {
                Print(S, Items[i]);
                S += "\r\n";
        }

        return S;
}

//traduce de formato texto a lista
template <class T> void TPoinList<T>::SetColumnText(const AnsiString &S)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caraacter por defecto.

        //el puntero Read debería apuntar a una función de lectura
        if(Read == NULL)
                throw EImproperCall("pointer Read should point to reading function");

        //descompone la columna de texto en lineas de texto
        TStringList *Lines = new TStringList();
        Lines->Text = S;

        AnsiString Line;
        AnsiString SubS;
        TPoinList<T> _Items;

        //escudriña las lineas de etexto en busca de elementos
        _Items.NewLast(Lines->Count);
        //por cada linea de texto
        for(int i=0; i<Lines->Count; i++) {
                //asigna la linea de texto para facilitar su acceso
                _Items.NewLast();
                Line = Lines->Strings[i];
                int j = 1;
                Read(_Items.LastPointed, Line, j);
                if(j <= Line.Length()) {
                        StrReadToEnd(SubS, Line, j);
                        SubS = StrTrim(SubS);
                        if(SubS.Length() > 0)
                                throw EImproperArgument("unrecognizable string '"+SubS+"'");
                }
        }

        //copia los elementos de la variable tampón
        Count = _Items.getCount();
        for(int i=0; i<getCount(); i++)
                Items[i] = _Items[i];

        //libera la memoria dinámica
        delete Lines;
}
                 */

//---------------------------------------------------------------------------

} //namespace Lists

//---------------------------------------------------------------------------
#endif // POINTERSLIST_H
