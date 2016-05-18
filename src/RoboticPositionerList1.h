// Copyright (c) 2012-2016 Isaac Morales Durán. All rights reserved.
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
//File: RoboticPositionerList1.h
//Content: RPlist with sizing properties
//Last update: 06/05/2014
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ROBOTICPOSITIONERLIST1_H
#define ROBOTICPOSITIONERLIST1_H

#include "ExclusionArea.h"
#include "PointersList.h"
#include "PairPositionAngles.h"
#include "MotionProgram.h"

//---------------------------------------------------------------------------

using namespace Lists;
using namespace Positioning;

//namespace for models
namespace Models {

//---------------------------------------------------------------------------

//class RP list
class TRoboticPositionerList1 : public TItemsList<TRoboticPositioner*> {
protected:
    //TOLERANCES:

//    double p_SPMmin;
    double p_PAem;
    double p_Pem;

    //SIZING PROPERTIES:

    TDoublePoint p_O;

    double p_LO3max;

    double p_x3min;
    double p_x3max;
    double p_y3min;
    double p_y3max;

    //AREA PROPERTIES:

    double p_Spt;
    double p_Set;
    double p_Ret;
    double p_FiberDensity;

public:
    //TOLERANCE PROPERTIES:

/*    //SPM for minimun jump
    //must be nonnegative
    //default value: MEGARA_SPMmin mm
    double getSPMmin(void) const {return p_SPMmin;}
    void setSPMmin(double);*/
    //margen de error de orientación de S0.
    //debe ser no negativo
    //valor por defecto: MEGARA_PAem rad
    double getPAem(void) const {return p_PAem;}
    void setPAem(double);
    //margen de error de apuntado de S0.
    //debe ser no negativo
    //valor por defecto: MEGARA_Pem mm
    double getPem(void) const {return p_Pem;}
    void setPem(double);

    //Los márgenes de tolerancia conjunta del Fiber MOS
    //deben poder ser iguales a cero, para permitir desplazar
    //todo el error a los márgenes de los RPs individuales.
    //Por otra parte, podría desearse que el PAem absorbiera
    //el error de orientación del Fiber MOS, debido al elevado
    //peso de la rueda que lo sostiene.

    //Los márgenes de seguridad serán tenidas en cuenta en el cálculo
    //de las componentes del SPM de cada posicionador. Para facilitar
    //este proceso se definen dos métodos en la lista de posicionadores:
    //      void calculateSPMcomponents(TRoboticPositioner&);
    //      void calculateSPMcomponents();
    //Ý los métodos de cálculo de las componentes de SPM en elposicionador:
    //      double SPMrec(void) const;
    //      double SPMsta(void) const;
    //      double SPMdyn(void) const;
    //      double SPMoff(double PAem, double Pem) const;

    //SIZING PROPERTIES:

    //posición del origen del sistema S0
    //valor constante: (0, 0)
    TDoublePoint getO(void) const {return p_O;}

    //radio del círculo mínimo que contiene
    //el dominio de todos los posicionadores
    double getLO3max(void) const {return p_LO3max;}

    //límite abcisa inferior del cuadrado que contiene
    //el dominio conjunto de P3
    double getx3min(void) const {return p_x3min;}
    //límite abcisa superior del cuadrado que contiene
    //el dominio conjunto de P3
    double getx3max(void) const {return p_x3max;}

    //límite ordenada inferior del cuadrado que contiene
    //el dominio conjunto de P3
    double gety3min(void) const {return p_y3min;}
    //límite ordenada superior del cuadrado que contiene
    //el dominio conjunto de P3
    double gety3max(void) const {return p_y3max;}

    //AREA PROPERTIES:

    //NOTA: los parámetros Sp, Se y Re son calculados analíticamente
    //para el caso en que los posicionadores adyacentes cumplen:
    //       radio del dominio igual r_3max;
    //       equidistantes 4*L*cos(M_PI/6).

    //área de participación del conjunto de posicionadores
    //      Spt = Sum{Spt[i]}
    double getSpt(void) const {return p_Spt;}

    //área exclusiva del conjunto de posicionadores
    //      Set = Sum{Set[i]}
    double getSet(void) const {return p_Set;}

    //proporción de área exclusiva del conjunto de posicionadores
    //      Ret = Sum{Ret[i]*Spt[i]}/Spt
    double getRet(void) const {return p_Ret;}

    //densidad de posicionadores de la lista
    //número de posicionadores por unidad de superficie
    //      FiberDensity = double(RoboticPositioners.Count)/Spt
    double getFiberDensity(void) const {return p_FiberDensity;}

    //ADVERTENCIA: cuando cambia el radio del dominio del posicionador
    //o de alguno de los adyacentes, o la distancia entre posicionadores
    //no es igual a 4*L*cos(M_PI/6), los valores Spt, Set y Ret no
    //se corresponderán con la realidad.

    //ADVERTENCIA: cuando r_3min!=0 se tiene que parte del área excusiva
    //no forma parte del dominio del posicioandor, con lo que habría que
    //distinguir dos tipos de áreas exclusivas:
    //      área exclusiva total;
    //      área exclusiva accesible.

    //-------------------------------------------------------------------
    //COMPOSED PROPERTIES IN TEXT FORMAT:

    //dirección en memoria de la lista de elementos
    AnsiString getItemsAddressText(void) const {
        return IntToHex(reinterpret_cast<intptr_t>(&Items), 8);}

    //TOLERANCE PROPERTIES IN TEXT FORMAT:

//    AnsiString getSPMminText(void) const;
  //  void setSPMminText(const AnsiString&);
    AnsiString getPAemText(void) const;
    void setPAemText(const AnsiString&);
    AnsiString getPemText(void) const;
    void setPemText(const AnsiString&);

    //SIZING PROPERTIES IN TEXT FORMAT:

    AnsiString getOText(void) const;

    AnsiString getLO3maxText(void) const;

    AnsiString getx3minText(void) const;
    AnsiString getx3maxText(void) const;
    AnsiString gety3minText(void) const;
    AnsiString gety3maxText(void) const;

    //AREA PROPERTIES IN TEXT FORMAT:

    AnsiString getSptText(void) const;
    AnsiString getSetText(void) const;
    AnsiString getRetText(void) const;
    AnsiString getFiberDensityText(void) const;

    //SETS OF PROPERTIES IN TEXT FORMAT:

    //get the origins table in text format:
    //      Id      x0      y0      thetaO1
    AnsiString getOriginsTableText(void) const;
    //set the origins table in text format:
    //      Id      x0      y0      thetaO1
    void setOriginsTableText(const AnsiString&);
    //get the P3 table in text format:
    //      Id      x3      y3
    AnsiString getPositionsP3TableText(void) const;
    //set the P3 table in text format:
    //      Id      x3      y3
    void setPositionsP3TableText(const AnsiString&);
    //get the P_3 table in text format:
    //      Id      x_3      y_3
    AnsiString getPositionsP_3TableText(void) const;
    //tabla de PPA table in text format:
    //      Id      p_1     p___3
    AnsiString getPositionsPPATableText(void) const;

    //conjunto de todas las propiedades de seguridad
    //en formato asignaciones
    AnsiString getToleranceText(void) const;
    //conjunto de propiedades de dimensionamiento
    //en formato asignaciones
    AnsiString getSizingText(void) const;
    //conjunto de propiedades de área
    //en formato asignaciones
    AnsiString getAreaText(void) const;
    //conjunto de propiedades de gráficas
    //en formato asignaciones
    AnsiString getGraphicsText(void) const;

    //conjunto de todas las propiedades
    //en formato de asignaciones
    AnsiString getAllText(void) const;
    //instancia de la lista de posicionadores
    //en formato de asignaciones
    AnsiString getInstanceText(void) const;
    void setInstanceText(const AnsiString&);

    //-------------------------------------------------------------------
    //STATIC METHODS:

    //lee una instancia en una cadena
    static void  readInstance(TRoboticPositionerList1* &RPL,
                              const AnsiString& S, int &i);

    //-------------------------------------------------------------------
    //BUILDING AND DESTROYING METHODS:

    //construye una lista de posicionadores
    TRoboticPositionerList1(void);

    //copia las propiedades de tolerancia
    //      (PAem, Pem)
    void copyTolerance(const TRoboticPositionerList1*);
    //copia las propiedades de dimensionamiento
    //      (LO3, x3min, y3min, x3max, y3max)
    void copySizing(const TRoboticPositionerList1*);
    //copia las propiedades de área
    //      (Spt, Set, Ret, FiberDensity)
    void copyArea(const TRoboticPositionerList1*);
    //copia las propiedades gráficas
    //      (LimitDomainColor, PaintActuators_, PaintLimitDomain_, PaintMap)
    void copyGraphics(const TRoboticPositionerList1*);

    //clone a RP list
    //      (Tolerance, Sizing, Area, Graphics)
    void Clone(const TRoboticPositionerList1*);

    //WARNING: inherited methods must be redefined withidentical name
    //for avoid errors.

    //construye un clon de una lista de posicionadores
    TRoboticPositionerList1(const TRoboticPositionerList1*);

    //ADVERTENCIA: la clonación de una lista de posicionadores causará
    //la construcción de posicionadores homólogos en la lista clon.

    //ADVERTENCIA: la destrucción de la lista de posicionadores causará
    //la destrucción de los posicionadores de la lista. Si lo que desea
    //es destruir la lista de posicionadores sin destruir los posicionadores
    //deberá vaciarla antes con el método: ClearWithoutDestroy.

    //METHODS TO ADD OR DELETE RPs:

    //Inherited methods:
    //  Add(TRoboticPositioner *RP): dd a RP to the list.
    //  Delete(int i): delete a RP of the list without destroy it.

    //search and delete a RP of the list
    //return the position where the RP was found
    int deleteIfFind(const TRoboticPositioner* RP);

    //METHODS TOBUILD RPs:

    //construye la capa n de posicionadores equidistantes una distancia D
    //donde la capa 0 representa el posicionador central
    void buildLayer(int &CountId, int n, double D);

    //construye una colmena circular de posicionadores
    //con N capas (más el central) y destruye
    //los posicionadores fuera del radio R
    void buildCircularHive(int &CountId,
                           double D=MEGARA_D, int N=19, double R=500);
    //construye una colmena cuadrada de posicionadores
    //con N capas
    void buildSquareHive(int &CountId,
                         double D=MEGARA_D, int M=5, int N=4);

    //Valores recomendados para BuildCircularHive:
    //      SIDE 85: N=5 pos; R=151 mm;
    //      SIDE 1003: N=19 pos; R=500 mm;
    //Valores recomendados para BuildquareHive:
    //      MEGARA 104: M=5 pos; N=4 pos;

    //METHODS TO DESTROY RPs:

    //detruye el posicionador indicado de la lista
    void Destroy(int i);
    //destruye todos los posicionadores de la lista
    void Destroy(void);
    /*        //destruye los posicionadores seleccionados de la lista
        int DestroySelected(void);*/
    //detruye los posicionadores que se encuentren a
    //una distancia del centro mayor que la indicada
    void Destroy(double rmax);

    //WARNING: inherited methods must be redefined withidentical name
    //for avoid errors.

    //METHODS TO SEARCH RPs:

    //busca un posicionador en la lista
    int search(const TRoboticPositioner *RP) const;
    int search(const TActuator *A) const;
    //busca el primer posicionador con el identificador indicado
    int searchId(int Id) const;
    //devuelve el puntero al primer posicionador
    //con el identificador indicado
    const TRoboticPositioner *searchIdPointer(int Id) const;
    //search the first free identifier starting fron Id
    int searchFirstFreeId(int Id) const;

    //busca el primer posicionador de la lista
    //en cuyo dominio de P3 se encuentra un punto
    int searchDomainP3(TDoublePoint P);
    int searchDomainP3(double x, double y);

    //busca los posicionadores de la lista que tienen
    //algún cuantificador desactivado
    void searchDisabledQuantificators(TVector<int> &indices);

    //------------------------------------------------------------------
    //ASSIMILATION METHODS:

    //A partir de:
    //      (Items[i]->Eo, Items[i]->Ep, Items[i]->Tstop_em, Items[i]->Tshif_em, SPMmin, PAem, Pem)
    //      {Items[i]->rmax}
    //Obtiene y asigna a cada posicionador de la lista:
    //      (Items[i]->SPMrec, Items[i]->SPMsta, Items[i]->SPMdyn, Items[i]->SPMmin, Items[i]->SPMoff)
    void calculateSPMcomponents(void);

    //El método calculateSPMcomponents() y los sucesivos
    //deberán ser invocados toda vez que los márgenes de seguridad
    //sean modificados.

    //determina los posicionadores que están lo bastante cerca
    //de cada posicionador como para invadir su dominio de maniobra
    void determineAdjacents(void);
    //ordena las listas de posicionadores adyacentes en
    //sentido levógiro empezando por el más próximo a 0
    void sortAdjacents(void);

    //El método DetermineAdjacents() y los sucesivos
    //deberán ser invocados toda vez que se añadan o borren posicionadores,
    //o se modifique algún parámetro de posicionamiento o dimensionamiento,
    //que pudiera afectar a la adyacencia de algún posicionador.

    //calcula las propiedades de seguridad
    //paracada posicionador de la lista:
    //      {r_min, r_saf, r_2saf, theta___2saf, theta_2rad}
    void calculateSafeParameters(void);

    //el método CalculateSafeParameters() debe ser invicado
    //toda vez que se invoque el método DetermineAdjacets()
    //o se cambie algún parámetro de dimensionameinto o posicionamiento
    //de algún cilindro adyacente

    //la invocación a CalculateSafeParámeters() no es automática
    //quedando bajo responsabilidad del programador su invocación
    //ya que los posicionadores son accesibles individualmente
    //y por consiguientes sus parámetros, de todos modos,
    //podrían ser cambiados de manera inadvertida
    //y definir una función de acceso para cada parámetro
    //de cada posicionador sería demasiado engorroso

    //Los métodos de asimilación:
    //      Deben ser aplicados despues de contruir, destruir posicionadores
    //      o modificar la geometría de alguno de ellos.
    //
    //      Son computacionalmente costosos, de modo que no serán invocados
    //      automáticamente al ajustar cada parámetro independiente,
    //      si no que deben ser invocados manualmente una sola vez
    //      despues de ajustar todos los parámetros.

    //calcula las propiedades de dimensionamiento:
    //      (LO3max, x3min, x3max, y3min, y3max)
    void calculateSizingParameters(void);

    //calcula todos los parámetros de dimensionamiento derivados
    //      (Spt, Set, Ret, FiberDensity)
    void calculateAreaParameters(void);

    //invoca a todos los métodos de asimilación de
    //los parámetros de dimensionamiento:
    //      determineAdjacents
    //      sortAdjacents
    //      calculateSafeParameters();
    //      calculateSizingParameters();
    //      calculateAreaParameters
    void assimilateSizing(void);

    /*        //asimila la configurración de posicionadores dada ejecutando:
        //      calculateSPMcomponents();
        //      assimilateSizing();
        void assimilate(void);
*/
    //METHODS FOR JOINTLY READING:

    //get the PPA list in steps
    void getPositions(TPairPositionAnglesList& PPAL) const;

    //METHODS FOR JOINTLY ALLOCATION:

    //asigna las posiciones angulares de los ejes
    //este método es atómico
    void setPositions(const TPairPositionAnglesList& PositionList);
    //asigna conjuntamente las tolerancias
    //      (PAem, Pem)
    void setTolerance(double PAem,double Pem);

    //------------------------------------------------------------------
    //METHODS FOR JOINTLY DOMAIN:

    //determina el intervalo cartesiano que contiene
    //el dominio de todos los posicionadores
    //si el número de posicionadores de la lista es menor que uno
    //lanza una excepcion EImproperCall
    void getDomainP3s(double &x3min, double &x3max,
                      double &y3min, double &y3max);

    //determina si un punto se encuentra dentro del círculo
    //que contiene el dominio conjunto de los posicionadores
    bool isInCircle(const TDoublePoint&);
    //determina si un punto se encuentra dentro del cuadrado
    //que contiene el dominio conjunto de los posicionadores
    bool isInSquare(const TDoublePoint&);

    //determine if all RPs of the list are in securepositions
    bool allRPsAreInSecurePosition(void) const;

    //determine if all operative RPs of the list are in the origin
    bool allOperativeRPsAreInTheOrigin(void) const;

    //METHODS FOR JOINTLY DISOPLACEMENT:

    //mueve los ejes de los posicionadores
    //a sus orígenes de coordenadas
    void moveToOrigins(void);
    /*        //mueve los ejes de los posicionadores seleccionados
        //a sus orígenes de coordenadas
        int moveToOriginsSelected(void);*/

    //mueve los brazos que están fuera del área de seguridad a
    //las posiciones de seguridad estables más próximas
    void moveOutsideArmsToSafePositions(void);
    /*        //mueve los brazos de los posicionadores seleccionados que
        //están fuera del área de seguridad a las posiciones de
        //seguridad estables más próximas
        int moveOutsideArmsToSafePositionsSelected(void);*/

    //METHODS TO SEGREGATE RPs:

    //segrega los posicionadores en dos listas:
    //      lista con el brazo dentro del área de seguridad;
    //      lista con el brazo fuera del área de seguridad;
    void segregateInOut(TRoboticPositionerList1 &Inners,
                        TRoboticPositionerList1 &Outsiders) const;

    //segregates the operative RPs in unsecure positions
    void segregateOperativeOutsiders(
            TRoboticPositionerList1& Outsiders) const;

    //segregates the operative RPs in security positions
    //out the origins
    void segregateOperativeInnersOutTheOrigins(
            TRoboticPositionerList1& Inners) const;

    //segregates the collided RPs
    void segregateCollided(TRoboticPositionerList1& Collided,
                           TRoboticPositionerList1& NotCollided) const;

    /*        //segrega los posicionadores seleccionados en una lista
        void SegregateSelected(TRoboticPositionerList1&) const;
*/
    //METHODS FOR STACKING POSITION ANGLES:

    //apila las posiciones de cada posicionador de la lista
    void pushPositions(void);
    //restaura la siguiente posición apilada de cada posicionador
    //sin desempilarla
    void restorePositions(void);
    //desempila la siguiente posición apilada de cada posicionador
    void popPositions(void);
    //restaura y desempila la siguiente posición apilada de cad aposicionador
    void restoreAndPopPositions(void);

    //apila la posición de los brazos de todos los posicionadores
    void pushArmtheta___3s();
    //restaura la últimaposición apilada del brazo de cada posicionador
    void restoreArmtheta___3s(void);
    //restaura la siguiente posición apilada del brazo de cada posicionador
    void popArmtheta___3s(void);

    //METHODS FOR STACKING QUANTOFIERS STATUS:

    //apila el estado de
    //los cuantificadores de cada posicionador de la lista
    void pushQuantifys(void);
    //restaura el último estado apilado de
    //los cuantificadores de cada posicionador de la lista
    void restoreQuantifys(void);
    //desempila el último estado apilado de
    //los cuantificadores de cada posicionador de la lista
    void popQuantifys(void);
    //restaura y desempila el último estado apilado de
    //los cuantificadores de cada posicionador de la lista
    void restoreAndPopQuantifys(void);

    //apila el estado
    //del cuantificador del brazo de cada posicionador de la lista
    void pushArmQuantify___s(void);
    //restaura el último estado apilado
    //del cuantificador del brazo de cada posicionador de la lista
    void restoreArmQuantify___s(void);
    //desempila el último estado apilado
    //del cuantificador del brazo de cada posicionador de la lista
    void popArmQuantify___s(void);
    //restaura y desempila el último estado apilado
    //del cuantificador del brazo de cada posicionador de la lista
    void restoreAndPopArmQuantify___s(void);

    //------------------------------------------------------------------
    //METHODS FOR DETERMINE COLLISION STATUS:

    //levanta las banderas indicadoras de determinación de colisión
    //pendiente de todos los posicionadores de la lista
    void enablePending(void);

    //determina si algún brazo de algún posicionador
    //colisiona con el brazo de algún posicionador adyacente
    bool thereIsCollision(void);
    //busca los posicionadores de la lista cuyo brazo colisiona con
    //el brazo de algún otro posicionador adyacente
    void searchCollinding(TVector<int> &indices);
    //obtiene los conjuntos de posicionadores en colisión en la exposición indicada
    void getCollisionClusterList(TPointersList<TItemsList<TRoboticPositioner*> > &CCL);

    //METHODS FOR RANDOMIZE AXES:

    //lleva los ejes de los posicionadores a posiciones aleatorias
    //con distribución uniforme en sus dominios
    void randomize(void);
    /*        //lleva los ejes de los posicionadores seleccionados a
        //posiciones aleatorias con distribución uniforme en sus dominios
        int RandomizeSelected(void);*/

    //lleva los ejes de los posicionadores a posiciones aleatorias
    //con distribución uniforme en sus dominios
    //en las que no colisionan entre si
    void randomizeWithoutCollision(void);
    /*        //lleva los ejes de los posicionadores seleccionados a
        //posiciones aleatorias con distribución uniforme en
        //sus dominios en las que no colisionan entre si
        int RandomizeWithoutCollisionSelected(void);*/

    //METHODS FOR RANDOMIZE P3:

    //lleva el punto P3 de los posicionadores a posiciones aleatorias
    //con distribución uniforme en su dominio
    void randomizeP3(void);
    /*        //lleva el punto P3 de los posicionadores seleccionados a
        //posiciones aleatorias con distribución uniforme en su dominio
        int RandomizeP3Selected(void);*/

    //lleva el punto P3 de los posicionadores a posiciones aleatorias
    //con distribución uniforme en su dominio
    //en las que no colisionan entre si
    void randomizeP3WithoutCollision(void);
    /*        //lleva el punto P3 de los posicionadores seleccioandos a
        //posiciones aleatorias con distribución uniforme en su dominio
        //en las que no colisionan entre si
        int RandomizeP3WithoutCollisionSelected(void);*/

    //METHODS RELATED WITH THE MP:

    //search the RP with the minimun Dsec
    int searchDsecMin(double DsecMin) const;
    //get the minimun Dsec
    double getDsecMin() const;
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // ROBOTICPOSITIONERLIST1_H
