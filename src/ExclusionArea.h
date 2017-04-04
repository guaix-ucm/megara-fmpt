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
//File: ExclusionArea.h
//Content: exclusion area (EA) model
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef EXCLUSIONAREA_H
#define EXCLUSIONAREA_H

#include "Barrier.h"

//Para determinar el estado de colisión de un EA con un RP adyacente,
//se utilizará el método:
//      Barrier.Contour.Collides(A->Arm->Contour, Barrier.SPM + A->Arm->SPM)
//de modo que hay que acceder a algunas propiedades del RP adyacente;
//por eso aquí no basta predeclarar la clase TRoboticPositioner.

//---------------------------------------------------------------------------

using namespace Lists;

//espacio de nombres de modelos
namespace Models {

class TRoboticPositioner;

//---------------------------------------------------------------------------
//TExclusionArea
//---------------------------------------------------------------------------

//clase área de exclusión
class TExclusionArea {
    //PROPIEDADES DEL ÁREA DE EXCLUSIÓN:

    double p_Eo;
    double p_Ep;

    int p_Id;

public:
    //------------------------------------------------------------------
    //PROPIEDADES ESTÁTICAS:

    //lista de objetos construidos
    static TItemsList<TExclusionArea*> Builts;

    //La lista de objetos contruidos es útil para:
    //- determinar los identificadores que se encuentran repetidos;
    //- obtener identificadores unívocos;
    //- controlar la integridad de memoria.

    //El objeto estático Builts debe ser contruido
    //antes de invocar al contructor mediante:
    //      TItemList<TExclusionArea*> TExclusionArea::Builts;
    //para ello esta sentencia deberá ser insertada en el mismo
    //archivo donde se invoca al constructor, antes de la invocación.
    //Si se insertase en este archivo o en su correspondiente .cpp,
    //sería invocado después de llamar al constructor.

    //------------------------------------------------------------------
    //PROPIEDADES DEL ÁREA DE EXCLUSIÓN:

    //barrera del área de exclusión
    TBarrier Barrier;

    //margen de error de orientación en S0
    //      Valor por defecto: MEGARA_Eo rad
    double getEo(void) const {return p_Eo;} void setEo(double);
    //margen de error de posición en S0
    //      Valor por defecto: MEGARA_Ep mm
    double getEp(void) const {return p_Ep;} void setEp(double);

    //número de identificación
    //debe ser mayor que cero
    //valor por defecto: 0
    int getId(void) const {return p_Id;} void setId(int);

    //lista de clusters (RP, Dmin, Dend) cuyo RP está lo bastante cerca
    //para que pueda colisionar con la barrera
    //valor por defecto: {}
    TItemsList<TRoboticPositioner*> AdjacentRPs;

    //Las EAs adyacentes y los RPs adyacentes serán determinados mediante
    //el método TRoboticPositionerList::determineAdyacents().

    //indica si la barrera del EA será tenida en cuenta
    //en la determinación de distancias y colisiones
    //valor por defecto: true
    bool Pending;

    //La propiedad Pending será usada en los métodos:
    //      thereIsCollisionWithPendingAdjacent
    //      searchCollindingPendingAdjacent
    //con objeto de evitar aplicar el método de determinación de colisión
    //más de una vez entre cada par de brazos.

    //PROPIEDADES EN FORMATO TEXTO:

    //dirección en memoria de la barrera
    //en formato texto
    AnsiString getBarrierAddressText(void) const {
        return IntToHex(intptr_t(&Barrier));}

    AnsiString getEoText(void) const; void setEoText(const AnsiString&);
    AnsiString getEpText(void) const; void setEpText(const AnsiString&);

    AnsiString getIdText(void) const;
    void setIdText(const AnsiString&);

    //Adjacents.Text

    AnsiString getPendingText(void) const;
    void setPendingText(const AnsiString&);

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    //contorno de la barrera
    //en formato de asignaciones
    AnsiString getContour_Text(void) const;

    //conjuntos de propiedades de seguridad
    //en formato asignaciones de texto
    AnsiString getSecurityText(void) const;

    //conjunto de propiedades de tolerancia
    //en formato de asignaciones
    AnsiString getToleranceText(void) const;

    //instancia del posicionador
    //en formato de asignaciones
    AnsiString getInstanceText(void) const;
    void setInstanceText(const AnsiString&);

    //conjunto de propiedades de origen (Id, x0, y0, thetaO1)
    //en formato línea de texto
    AnsiString getOriginsRowText(void) const;

    //-------------------------------------------------------------------
    //MÉTODOS ESTÁTICOS:

    //compara los identificadores de dos EAs
    static int  compareIds(TExclusionArea *EA1,
                           TExclusionArea *EA2);

    //imprime el identificador de un EA
    static void  printId(AnsiString &S, TExclusionArea *EA);

    //Los métodos estáticos:
    //      CompareIds
    //      PrintId
    //serán apuntados en la lista de EAs (del tipo TExclusionAreaList),
    //para permitir su ordenacíon en función de los identificadores
    //y permitir la impresión de los mismos.

    //obtiene las etiquetas de las propiedades de origen
    //en formato fila de texto
    static AnsiString getOriginsLabelsRow(void);
    //atraviesa las etiquetas de las propiedades de origen
    //en formato fila de texto
    static void  travelOriginsLabelsRow(const AnsiString&, int&);

    //imprime los valores de las propiedades de orien de un EA
    //al final de una cadena de texto
    //en formato fila de texto
    static void  printOriginsRow(AnsiString& S,
                                 TExclusionArea *EA);
    //lee los valores de las propiedades de orien para un EA
    //desde la posición indicada de una cadena de texto
    //en formato fila de texto
    static void  readOriginsRow(TExclusionArea *EA,
                                const AnsiString& S, int &i);

    //lee una instancia del EA
    //desde la posición indicada de una cadena de texto
    //en formato de asignaciones
    static void  readInstance(TExclusionArea *EA,
                              const AnsiString& S, int &i);

    //-------------------------------------------------------------------
    //MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACIÓN:

    //contruye un área de exclusión
    //con los valores por defecto
    TExclusionArea(void);
    //construye un área de exclusión
    //con los valores indicados
    //si el número de identificación es menor que uno
    //      lanza una execepción EImproperArgument
    TExclusionArea(int Id, TDoublePoint P0, double thetaO1=0);

    //ADVERTENCIA: para poder contruir clones de áreas de exclusión
    //la duplicidad de números de identificación está permitida.
    //El control de duplicidad de identificadores debe llevarse
    //en todo caso mediante métodos. Al fin y al cabo en el mundo real
    //será posible configurar dos objetos para que tengan el mismo
    //identificador.

    //clona un área de exclusión
    void clone(const TExclusionArea *EA);

    //contruye un clon de un área de exclusión
    TExclusionArea(const TExclusionArea *EA);

    //libera la memoria dinámica y borra el objeto de Builts
    //si el objeto no está en Builts lanza EImproperCall
    ~TExclusionArea();

    //-------------------------------------------------------------------
    //MÉTODOS PARA CALCULAR LOS VALORES RECOMENDADOS DE
    //LAS COMPONENTES DE SPM:

    //Calcula el márgen perimetral de seguridad para evitar:
    //- el error mecánico debido al mecanizado del área de exclusión;
    //- el error mecánico de orientación en el anclaje del área de exclusión;
    //- el error mecánico de posicionamiento en el anclaje del área de exclusión;
    //- el error numérico de las operaciones matemáticas del modelo;
    //Todos los errores serán absorbidos por Eo y Ep
    //en la siguiente fórmula:
    //      SPMsta = Eo*r_max + Ep
    //Donde:
    //      Eo: margen de error de orientación en rad;
    //      Ep: margen de error de posición en mm.
    double SPMsta(void) const;

    //------------------------------------------------------------------
    //MÉTODOS DE ASIMILACIÓN:

    //A partir de:
    //      (Eo, Ep)
    //      Barrier.r_max
    //Determina:
    //      Barrier.SPM
    void calculateSPM(void);

    //-------------------------------------------------------------------
    //MÉTODOS DE ASIGNACIÓN CONJUNTA:

    //asigna las propiedades de origen
    void setOrigins(int Id, double x0, double y0, double thetaO1);

    //asigna conjuntamente las tolerancias y las asimila
    //      (Eo, Ep)
    void setTolerances(double Eo, double Ep);
};

//determina si dos areas de exclusión difieren entre si
bool areUnequals(const TExclusionArea*, const TExclusionArea*);

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // EXCLUSIONAREA_H
