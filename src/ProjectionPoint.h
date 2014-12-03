//---------------------------------------------------------------------------
//Archivo: ProjectionPoint.h
//Contenido: punto de proyección
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef PROJECTIONPOINT_H
#define PROJECTIONPOINT_H

#include "SkyPoint.h"
#include "..\A_Arm\Figure.h"
#include "..\3_Lists\PointersList.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TProjectionPoint
//---------------------------------------------------------------------------

//punto de proyección
class TProjectionPoint : public TCircle {
protected:
    //OBJETOS EXTERNOS ADSCRITOS:

    TSkyPoint *__SkyPoint;

    //PROPIEDADES:

    //PROPIEDADES EN FORMATO TEXTO:

    //CONJUNTOS DE PROPIEDADES DEFINITORIAS EN FORMATO TEXTO:

public:
    //OBJETOS EXTERNOS ADSCRITOS:

    //punto de cielo externo adscrito
    //valor por defecto: NULL
    TSkyPoint *getSkyPoint(void) const {return __SkyPoint;}

    //PROPIEDADES:

    //número de identificación del punto de proyección
    //      SkyPoint->Id
    //debe ser no negativo
    int getId(void) const; void setId(int);

    //indica si el punto objetivo está seleccionado
    //valor por defecto: false
    bool Selected;

    //color por defecto con el que son contruidos los puntos de proyección
    //valor por defecto: clWhite
    //#static QColor DefaultColor;

    //PROPIEDADES EN FORMATO TEXTO:

    //número de identificación del punto de proyección
    AnsiString getIdText(void) const; void setIdText(const AnsiString&);

    //CONJUNTOS DE PROPIEDADES DEFINITORIAS EN FORMATO TEXTO:

    //las etiquetas de todas las propiedades en fila
    //valor por defecto:
    //      "Id          "+
    //      "x                       "+
    //      "y"
    static AnsiString RowIdent;

    //todas las propiedades en fila
    AnsiString getRowText(void) const; void setRowText(const AnsiString&);
    //todas las propiedades en columna
    AnsiString getColText(void) const; void setColText(const AnsiString&);
    //todas las propiedades en asignaciones
    AnsiString getAssignsText(void) const;
    void setAssignsText(const AnsiString&);

    //Formatos de texto:
    //      fila: los valores de las propiedades ordenados
    //              y separados por espacios;
    //      columna: los valores de las propiedades ordenados
    //              y separados por "\r\n";
    //      asignaciones: las asignaciones a las propiedades ordenadas
    //              y separadas por "\r\n".
    //
    //Las columnas de valores enteros tendrán un indentado
    //de 12 caracteres ya que el valor entero más largo tiene
    //13 caracteres.
    //
    //Las columnas de valores en punto flotante tendrán un indentado
    //de 24 caracteres ya que el valor en punto flotante más largo tiene
    //23 caracteres.

    //MÉTODOS ESTÁTICOS DE LISTA:

    //imprime las propiedades de un punto de proyección en una cadena
    static void __fastcall PrintRow(AnsiString &S,
                                    TProjectionPoint *PP);
    static void __fastcall PrintCol(AnsiString &S,
                                    TProjectionPoint *PP);
    static void __fastcall PrintAssigns(AnsiString &S,
                                        TProjectionPoint *PP);

    //lee las propiedades de un punto de proyección en una cadena
    static void __fastcall ReadRow(TProjectionPoint *PP,
                                   const AnsiString &S, int &i);
    static void __fastcall ReadCol(TProjectionPoint *PP,
                                   const AnsiString &S, int &i);
    static void __fastcall ReadAssigns(TProjectionPoint *PP,
                                       const AnsiString &S, int &i);

    //MÉTODOS PÚBLICOS:

    //contruye un punto de proyección adscrito a un punto de cielo
    TProjectionPoint(TSkyPoint *_SkyPoint=NULL);
    //clona un punto de proyección
    TProjectionPoint(TProjectionPoint*);

    //ADVERTENCIA: los puntos de proyección sin punto de cielo adscrito
    //no podrán ser deproyectados.

    //MÉTODOS DE COPIA:

    //copia las propiedades de un punto de proyección
    void Copy(TProjectionPoint*);
    TProjectionPoint &operator=(const TProjectionPoint&);

    //ADVERTENCIA: el punto de proyección copia quedará adscrito
    //al mismo punto de cielo.

    //randomiza el punto de proyección
    void Randomize(double xmin, double xmax, double ymin, double ymax);

    //MÉTODOS GRÁFICOS:

    //indica que parte del punto objetivo
    //puede ser agarrado en el punto indicado
    //      1: punto (P, R)
    //      0: niguna;
    int Grab(TDoublePoint Q);
};

//---------------------------------------------------------------------------
//TProjectionPointList
//---------------------------------------------------------------------------

//clase lista de puntos de cielo
class TProjectionPointList : public TPointersList<TProjectionPoint> {
    //PROPIEDADES DE INTERFAZ:

public:
    //PROPIEDADES DE INTERFAZ:

    //lista de puntos de cielo en formato texto
    AnsiString getText(void); void setText(const AnsiString &);

    //ADVERTENCIA: SetText construirá nuevos puntos de cielo con sus
    //propiedades inicialzadas por defecto.

    //MÉTODOS PÚBLICOS:

    //construye un alista de puntos de proyección
    //        TProjectionPointList(void);

    //copia una lista de puntos de proyección
    TProjectionPointList &operator=(const TProjectionPointList&);

    //añade puntos de proyección adscritos a
    //los puntos de cielo de una lista
    void Build(TSkyPointList&);

    //randomiza los puntos de proyección
    void Randomize(double xmin, double xmax, double ymin, double ymax);

    //contruye una capa de nodos de teleraña
    //donde la capa 0 representa el punto central
    void BuildSpiderwebNodes(int n, double D);

    //CONSEJO: si contruye las capas de dentor a fuera, los nodos
    //quedarán ordenados el función de la lejanía al origen
    //lo cual puede resultar interesante si no se conociera
    //a priori el número de nodos en que se va a realizar la búsqueda.

    //adscribe los posicionadores de fibra en cuyos dominios
    //se encuentra cada punto de proyección
    //        void AttachRoboticPositioners(TRoboticPositionerList &RPF);

    /*        //borra los puntos de proyección cuyo identificador
        //coincida con el indicado
        void DeleteId(int Id);
        //borra los puntos de proyección que han sido asignados
        void DeleteAllocatedPoints(TAllocationPairList *APL);
        void DeleteAllocatedPoints(TAllocationListList &EP);
  */

    //MÉTODOS GRÁFICOS:
/*#
    //imprime los puntos de proyección en una caja de lista
    void Print(QListWidget *LB);
    //imprime los puntos de proyección en un memorando
    void Print(QTextEdit *M);

    //dibuja los puntos de proyección en la fotografía de un trazador de formas
    void Paint(TPloterShapes *PS);
*/
    //guarda la lista de puntos de proyección en un archivo de texto
    void SaveToFile(const AnsiString &FileName);
    //carga los puntos de proyección de un archivo
    void LoadFromFile(const AnsiString &FileName);

    //selecciona todos los puntos objetivo de la lista
    void SelectAll(void);
    //deselecciona todos los puntos objetivo de la lista
    void DeselectAll(void);

    //busca el primer punto de proyección que está bajo el punto P
    //y que parte del punto objetivo puede ser agarrado en
    //el punto indicado:
    //      i: índice al punto de proyección bajo el punto P;
    //      dominio i: [0, Count];
    //      n: parte del punto de proyección agarrada;
    //         valores posibles:
    //            0: niguna;
    //            1: punto objetivo.
    //valores de retorno:
    //      false: ninguna parte agarrada
    //      true: alguna parte agarrada
    bool Grab(int &i, int &n, TDoublePoint P);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // PROJECTIONPOINT_H
