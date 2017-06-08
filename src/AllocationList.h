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
///@file AllocationList.h
///@brief allocation list
///@author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef TALLOCATIONLIST_H
#define TALLOCATIONLIST_H

#include "Allocation.h"
#include "RoboticPositionerList3.h"

//---------------------------------------------------------------------------

using namespace Models;

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//TAllocationList
//---------------------------------------------------------------------------

//class allocation list
class TAllocationList : public TItemsList<TAllocation*> {
protected:
        TRoboticPositionerList *p_RoboticPositionerList;

public:
        //extern-attached RP list
        TRoboticPositionerList *getRoboticPositionerList(void) const {
            return p_RoboticPositionerList;}

        //PROPERTIES IN TEXT FORMAT:

        //allocation list in text format
        AnsiString getAllocationsText(void);
        void setAllocationsText(AnsiString&);

        //BUILDING AND DESTROYING METHODS:

        //build a allocation list, attached to a RP list
        TAllocationList(TRoboticPositionerList *_RoboticPositionerList);

        //destroy the TAllocation and destroy the list
        ~TAllocationList();

        //SEARCHING METHODS:

        //search the allocation attached to a RP
        int searchAllocation(const TRoboticPositioner *RP) const;
        //search the allocation attached to an identified RP
        int searchAllocation(int Id) const;

        //LISTED ITEMS:

        //añade una asignación para el posicionador indicado de la lista
        void AddAllocation(int i);
        //borra la asignación indicada de la lista
        void DeleteAllocation(int i);

        //ALL ITEMS:

        //añade asignaciones con la posición del punto P3
        //de todos los posicionadores de la lista
        void AddP3(void);
        //destruye las asignaciones
        //de todos los posicionadores de la lista
        void Delete_(void);

        //El método:
        //  void Delete(int i);
        //Parece enmascarado por el método:
        //  void Delete(void);

        //asigna el punto P3o a los puntos objetivo
        //de todos los posicionadores de la lista
        void SetP3o(void);
        //asigna el punto P3 a los puntos objetivo
        //de todos los posicionadores de la lista
        void SetP3(void);

        //randomiza los puntos objetivo con distribución uniforme
        //en el dominio del punto P3 de sus posicionadores adscrito
        //de todos los posicionadores de la lista
        void Randomize(void);
        //randomiza los puntos objetivo con distribución uniforme
        //en el dominio del punto P3 de sus posicionadores adscritos
        //de todos los posicionadores de la lista
        void RandomizeWithoutCollision(void);

        //asigna los puntos objetivo
        //al punto P3 de sus posicionadores adscritos
        //si algún punto objetivo no está en el dominio de
        //su posicionador adscrito lanza EImproperCall
        //de todos los posicionadores de la lista
        void MoveToTargetP3(void);

        //NOTA: cunado un punto es asignado al punto P3 de un posicionador
        //cuya cuantificación está activada, el posicionador se moverá
        //a la posición estable máspróxima.

        //VALIDATION:

        //Los siguientes métodos deben ser invocados antes de
        //desplazar los posicionadores a sus puntos objetivo:

        //busca los puntos objetivo adscritos a posicionadores repetidos
        void SearchRepeatedRPs(TVector<int> &indices);

        //busca los puntos objetivo adscritos a posicionadores ausentes
        //en la lista de posicionadores RoboticPositionerList
        void SearchMissingRPs(TVector<int> &indices);

        //El método SearchMissigRPs es necesario
        //porque la lista de posicionadores adscrita (RoboticPositionerList)
        //puede ser manipulada.

        //busca los puntos objetivo que están fuera del dominio
        //de sus posicionadores adscritos
        void SearchOutDomineTAllocations(TVector<int> &indices);

        //Determina la invalidez de una lista de asignaciones
        //para ser programada.
        //Valores de retorno:
        //  0: lista de asignaciones válida;
        //  1: puntos objetivo adscritos a posicionadores ausentes;
        //  2: puntos objetivo fuera del dominio de sus posicionadores adscritos.
        int Invalid(TVector<int> &indices);

        //SEGREGATION:

        //segrega los posicionadores de las asignaciones en dos listas:
        //  internos: con el brazo dentro del área de seguridad;
        //  externos: con el brazo fuera del área de seguridad;
        //si algún punto objetivo está fuera del dominio de
        //su posicionador adscrito:
        //      lanza EImproperCall
        void SegregateInOut(TRoboticPositionerList &Inners,
                TRoboticPositionerList &Outsiders);

        //STACKING AND RETRIEVAL OF POSITIONS:

        //apila las posiciones de los posicionadores adscritos
        void PushPositions(void);
        //restaura las posiciones de los posicionadores adscritos
        void RestorePositions(void);
        //desempila las posiciones de los posicionadores adscritos
        void PopPositions(void);
        //restaura y desempila las posiciones de los posicionadores adscritos
        void RestoreAndPopPositions(void);

        //COLlISION:

        //levanta las banderas indicadoras de determinación de colisión
        //pendiente de todos los posicionadores adscritos a las asignaciones
        void EnablePendingCollisionDetermineTAllocations(void);

        //El siguiente método debe invocarse una vez desplazados
        //los posicionadores a sus puntos objetivo:

        //busca las asignaciones que colisionan con otras asignaciones
        void SearchCollindingTAllocations(TVector<int> &indices);

        //ADVERTENCIA: cuando la cuantificación de alguno de los ejes
        //de los posicioandores adscritos esta deactivada, SearchCollinding
        //comprobará si hay colisión en las posiciones cuantificadas de
        //los ejes que tengan la cuantificación activada, con lo cual
        //el punto P3 de los posicionadores podría no coincidir exactamente
        //con el punto obejtivo.

        //COUNT OF ALLOCATION TYPES:

        //count the number of reference sources in the allocation list
        unsigned int countNR(void) const;
        //count the number of blanks in the allocation list
        unsigned int countNB(void) const;

        //------------------------------------------------------------------
        //Utilización de los métodos:
        //
        //Para que la solución del programador tenga sentido,
        //se debe partir de una posición inicial sin colisiones.
        //      RoboticPositionerList->SearchCollinding(indices)
        //
        //Comprueba la validez de la lista de asignaciones
        //      Invalid(indices)
        //
        //Segrega los posicionadores internos de los externos:
        //      SegregateInOut(Inssers, Outsiders)
        //
        //Guarda las posiciones originales de los posicionadores adscritos:
        //      PushPositions();
        //
        //Mueve los posicionadores adscritos a sus puntos objetivo:
        //      MoveToTargetP3All()
        //
        //Comprueba que las posiciones finales son compatibles:
        //      Outsiders.SearchCollisions(indices)
        //------------------------------------------------------------------
};

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
#endif // TALLOCATIONLIST_H
