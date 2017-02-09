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
//File: ProjectionPoint.h
//Content:  projection point list
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef PROJECTIONPOINT_H
#define PROJECTIONPOINT_H

#include "SkyPoint.h"
#include "Figure.h"
#include "PointersList.h"

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//TProjectionPoint
//---------------------------------------------------------------------------

//class projection point
class TProjectionPoint : public TDoublePoint {
protected:
    //ATACHED EXTERN OBJECTS:

    TSkyPoint *p_SkyPoint;

public:
    //STATIC PROPERTIES:

    //label of property Id
    //default value: "Id"
    static AnsiString IdLabel;

    //ATACHED EXTERN OBJECTS:

    //punto de cielo externo adscrito
    //valor por defecto: NULL
    TSkyPoint *getSkyPoint(void) const {return p_SkyPoint;}

    //PROPERTIES:

    //número de identificación del punto de proyección
    //      SkyPoint->Id
    //debe ser no negativo
    int getId(void) const; void setId(int);

    //type of point:
    //  ptUNKNOWN: unknown type
    //  ptSOURCE: scientific source
    //  ptREFERENCE_SOURCE: reference source
    //  ptBLANK: background of sky
    //default value: ptUNKNOWN
    TPointType Type;

    //allocation Priority
    //devault value: 0
    unsigned int Priority;

    //PROPERTIES IN TEXT FORMAT:

    //número de identificación del punto de proyección
    AnsiString getIdText(void) const; void setIdText(const AnsiString&);

    //SETS OF PROPERTIES IN TEXT FORMAT:

    //get the labels of the properties
    //in row text format
    //  IdLabel+"\t"+xLabel+"\t"+yLabel
    static AnsiString getRowLabels(void);

    //todas las propiedades en fila
    AnsiString getRowText(void) const; void setRowText(const AnsiString&);

    //STATIC METHODS:

    //imprime las propiedades de un punto de proyección en una cadena
    static void  PrintRow(AnsiString &S, TProjectionPoint *PP);

    //lee las propiedades de un punto de proyección en una cadena
    static void  ReadRow(TProjectionPoint *PP, const AnsiString &S, int &i);

    //PUBLIC METHODS:

    //build a PP attached to a SP
    TProjectionPoint(TSkyPoint *_SkyPoint=NULL);
    //build a clon of a PP
    TProjectionPoint(TProjectionPoint*);

    //WARNING: PPs without attached SP can not be deprojected.

    //COPY METHODS:

    //copy the properties of a PP
    void Copy(TProjectionPoint*);
    TProjectionPoint& operator=(const TProjectionPoint&);
    //copy the properties of a DP
    TProjectionPoint& operator=(const TDoublePoint&);

    //WARNING: the PP will be attached to the same SP.
};

//---------------------------------------------------------------------------
//TProjectionPointList
//---------------------------------------------------------------------------

//clase lista de puntos de cielo
class TProjectionPointList : public TPointersList<TProjectionPoint> {
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
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // PROJECTIONPOINT_H
