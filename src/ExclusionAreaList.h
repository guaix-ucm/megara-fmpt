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
//File: ExclusionAreaList.h
//Content: EA list
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef EXCLUSIONAREALIST_H
#define EXCLUSIONAREALIST_H

//#include "RoboticPositionerList3.h"
#include "PointersList.h"
#include "ExclusionArea.h"

using namespace Lists;

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TExclusionAreaList
//---------------------------------------------------------------------------

class TRoboticPositionerList;

//clase lista de EAs
class TExclusionAreaList : public TItemsList<TExclusionArea*> {
protected:
    //PROPIEDADES DE LOCALIZACIÓN:

    TDoublePoint p_O;

    double p_rmax;

/*    double p_xmin;
    double p_xmax;
    double p_ymin;
    double p_ymax;
*/
public:
    //PROPIEDADES DE LOCALIZACIÓN:

    //posición del origen del sistema S0
    //valor por defecto: {0, 0}
    TDoublePoint getO(void) const {return p_O;}

    //radio del círculo mínimo que contiene
    //el dominio de todas las EAs
    double getrmax(void) const {return p_rmax;}

/*    //intervalo cartesiano que contiene todas las EAs
    double getxmin(void) const {return p_xmin;}
    double getxmax(void) const {return p_xmax;}
    double getymin(void) const {return p_ymin;}
    double getymax(void) const {return p_ymax;}
*/
    //-------------------------------------------------------------------
    //PROPIEDADES EN FORMATO TEXTO

    //dirección en memoria de la lista de EAs
    AnsiString getItemsAddressText(void) const {
        return IntToHex(intptr_t(&Items));}

    //PROPIEDADES DE LOCALIZACIÓN EN FORMATO TEXTO:

    AnsiString getOText(void) const;

    AnsiString getrmaxText(void) const;

/*    AnsiString getxminText(void) const;
    AnsiString getxmaxText(void) const;
    AnsiString getyminText(void) const;
    AnsiString getymaxText(void) const;
*/
    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    //tabla de orígenes de coordenadas:
    //    Id      x0      y0      thetaO1
    AnsiString getOriginsTableText(void) const;
    void setOriginsTableText(const AnsiString&);

    //conjunto de propiedades de localización
    //en formato asignaciones
    AnsiString getLocationText(void) const;

    //conjunto de todas las propiedades
    //en formato de asignaciones
    AnsiString getAllText(void) const;

    //-------------------------------------------------------------------
    //MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN

    //construye una lista de EAs
    TExclusionAreaList(void);

    //copia las propiedades de localización
    //    (O)//, rmax, xmin, xmax, ymin, ymax)
    void copyLocation(const TExclusionAreaList*);

    //clona una lista de EAs
    void Clone(const TExclusionAreaList*);

    //construye un clon de una lista de EAs
    TExclusionAreaList(const TExclusionAreaList*);

    //ADVERTENCIA: si al método de clonación, en vez de pasarle un puntero
    //se le pasa una lista de áreas de exclusión, el compilador puede no indicar
    //ni advertencia ni error, pero no se invocará al método correctamente.

    //ADVERTENCIA: la destrucción de la lista de EAs no causará
    //la destrucción de las EAs de la lista. Si lo que desea
    //es destruir las EAs, debería hacerlo mediante el método:
    //    EAL.Destroy();
    //Este método será invocado en el método destroy de TFiberMOSModel.

    //AÑADIR Y BORAR EAs:

    //Añadir un EA a la lista:
    //    Add(RP);
    //Borrar un EA de la lista
    //    Delete(i);
    //    DeleteWithoutDestroy(i);

    //------------------------------------------------------------------
    //MÉTODOS DE DESTRUCCION DE EAs:

    //detruye el EA indicada de la lista
    void Destroy(int i);
    //destruye todas las EAs de la lista
    void Destroy(void);

    //------------------------------------------------------------------
    //MÉTODOS DE BÚSQUEDA DE EAs:

    //busca el primer puntero a EA
    //con la dirección en memoria indicada
    int search(TExclusionArea *EA) const;
    //busca el primer índice a EA
    //con el identificador indicado
    int searchId(int Id) const;
    //busca el primer puntero a EA
    //con el identificador indicado
    const TExclusionArea *searchIdPointer(int Id) const;

    //------------------------------------------------------------------
    //MÉTODOS DE ASIMILACIÓN:

    //calcula el SPM de todas las EAs
    void calculateSPM(void);

    //El método CalculateSPM() y los sucesivos
    //deberán ser invocados toda vez que los márgenes de seguridad
    //sean modificados.

    //Los métodos de asimilación:
    //  - Deben ser aplicados despues de contruir, destruir EAs
    //    o modificar la geometría de alguna de ellas.
    //  - Son computacionalmente costosos, de modo que no serán invocados
    //    automáticamente al ajustar cada parámetro independiente,
    //    si no que deben ser invocados manualmente una sola vez
    //    despues de ajustar todos los parámetros.

    //calcula las propiedades de localización:
    //    (rmax)//, xmin, xmax, ymin, ymax)
    void calculateLocationParameters(void);

    //determina los RPs que están lo bastante cerca
    //de cada posicionador como para invadir su perímetro de seguridad
    void determineAdjacents(const TRoboticPositionerList& RPL);
    //ordena las listas de RPs adyacentes en
    //sentido levógiro empezando por el más próximo a 0
    void sortAdjacents(void);

    //asimila la configurración de posicionadores dada ejecutando:
    //  calculateSPM();
    //  calculateLocationParameters();
    void assimilate(void);

    //------------------------------------------------------------------
    //MÉTODOS DE DOMINIO CONJUNTO:

/*    //determina el intervalo cartesiano que contiene
    //el conjunto de todas las EAs
    //si el número de EAs de la lista es menor que uno:
    //    lanza una excepcion EImproperCall
    void getDomain(double &xmin, double &xmax,
                   double &ymin, double &ymax);
*/
    //determina si un punto se encuentra dentro del círculo
    //que contiene el dominio conjunto de las EAs
    bool isInCircle(const TDoublePoint&);
/*    //determina si un punto se encuentra dentro del intervalo carteasiano
    //que contiene el dominio conjunto de las EAs
    bool isInRectangle(const TDoublePoint&);*/

    //------------------------------------------------------------------
    //MÉTODOS PARA DETERMINAR SI HAY COLISIONES:

    //levanta las banderas indicadoras de determinación de colisión
    //pendiente de todos los posicionadores de la lista
    void enableAllPending(void);
    //configura el estado de colisión
    //de todos los posicionadores de la lista
//    void setAllCollision(bool);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // EXCLUSIONAREALIST_H
