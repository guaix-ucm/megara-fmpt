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
/// @file AllocationList.h
/// @brief allocation list
/// @author Isaac Morales Durán
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

/// @brief An allocation list allow encapsulate a set of allocations
/// (of RPs to projection points), necesaries for generate a parking program
/// or a pair (PP, DP).
/// @brief For this, the allocation list provide a set of functions
/// for simplify the addition of allocations, and determine if the
/// allocation list meet the preconditions for generate motion programs.

class TAllocationList : public TItemsList<TAllocation*> {
protected:
        TRoboticPositionerList *RoboticPositionerList;

public:
        /// Get the extern-attached RP list.
        TRoboticPositionerList *getRoboticPositionerList(void) const {
            return RoboticPositionerList;}

        //PROPERTIES IN TEXT FORMAT:

        /// Get the allocation list in text format.
        AnsiString getAllocationsText(void);
        /// Set the allocation list in text format.
        void setAllocationsText(AnsiString&);

        //BUILDING AND DESTROYING METHODS:

        /// Build a allocation list, attached to a RP list.
        TAllocationList(TRoboticPositionerList *RoboticPositionerList);

        /// Destroy the allocations and destroy the list.
        ~TAllocationList();

        //SEARCHING METHODS:

        /// Search the allocation attached to a RP.
        int searchAllocation(const TRoboticPositioner *RP) const;
        /// Search the allocation attached to an identified RP.
        int searchAllocation(int Id) const;

        //LISTED ITEMS:

        /// Add an allocation for the indexed RP of the attached list.
        void AddAllocation(int i);
        /// Delete the indexed allocation of the list.
        void DeleteAllocation(int i);

        //ALL ITEMS:

        /// @brief Add an allocation with the point P3
        /// of all RPs of the attached list.
        void AddP3(void);
        /// @brief Destroy all allocation of the list.
        void Delete_(void);

        //El método:
        //  void Delete(int i);
        //Parece enmascarado por el método:
        //  void Delete(void);

        /// Assign the point P3o to all allocation of the list.
        void SetP3o(void);
        /// Assign the point P3 to all allocation of the list.
        void SetP3(void);

        /// @brief Randomize the projection point of all allocation of the list
        /// with uniform distribution in the P3-point domain of the attached RP.
        void Randomize(void);
        /// @brief Randomize the projection point of all allocation of the list
        /// with uniform distribution in the P3-point domain of the attached RP,
        /// avoiding collissions.
        void RandomizeWithoutCollision(void);

        /// @brief For all allocations of the list, set the projection point
        /// to the P3 of the attached RP.
        /// @brief
        /// @exception EImproperCall if there is some projection point
        /// out of the P3-domain of their allocated RP.
        void MoveToTargetP3(void);

        //NOTA: cuando un punto es asignado al punto P3 de un posicionador
        //cuya cuantificación está activada, cada rotor se moverá a
        //la posición estable más próxima.

        /// @brief Get the Final Position List from a Initial Position List.
        /// @exception EImproperCall if the allocation list is invalid.
        /// @exception EImproperArgument if the IPL is invalid.
        void getFinalPositionList(TPairPositionAnglesList& FPL,
                                  TPairPositionAnglesList& IPL);

        //VALIDATION:

        //Los siguientes métodos deben ser invocados antes de
        //desplazar los posicionadores a sus puntos objetivo:

        /// Search the allocations with repeated RPs.
        void searchRepeatedRPs(TVector<int> &indices);

        /// Search the allocations with RPs missing in the attached RP list.
        void searchMissingRPs(TVector<int> &indices);

        //El método SearchMissigRPs es necesario
        //porque la lista de posicionadores adscrita (RoboticPositionerList)
        //puede ser manipulada.

        /// @brief Search the allocations with projection points
        /// out of the P3-domain of their attached RP.
        void searchOutDomineTAllocations(TVector<int> &indices);

        /// @brief Determine if the allocation list has any defect
        /// for generate motion programs:
        /// @return 0: the allocation list is valid.
        /// @return 1: projection points attached to missing RPs.
        /// @return 2: projection points out of the P3-domain of their attached RPs.
        int invalid(TVector<int> &indices);

        //SEGREGATION:

        /// @brief Segregate the RPs of the allocations in two lists:
        /// - Inners: with the arm in the security position.
        /// - Outsiders: with the arm out of the security position.
        /// @brief
        /// @exception EImproperCall if there is some projection point
        /// out of the P3-domain of their allocated RP.
        void SegregateInOut(TRoboticPositionerList &Inners,
                TRoboticPositionerList &Outsiders);

        //STACKING AND RETRIEVAL OF POSITIONS:

        /// Push the position angles of the allocated RPs.
        void PushPositions(void);
        /// Restore the position angles of the allocated RPs.
        void RestorePositions(void);
        /// Pop the position angles of the allocated RPs.
        void PopPositions(void);
        /// Restore and pop the position angles of the allocated RPs.
        void RestoreAndPopPositions(void);

        //COLlISION:

        /// Set to up the Pending flag of the RPs of all allocations.
        void EnablePendingCollisionDetermineTAllocations(void);

        //El siguiente método debe invocarse una vez desplazados
        //los posicionadores a sus puntos objetivo:

        /// Search the allocations whose RPs are in colliding status.
        void SearchCollindingTAllocations(TVector<int> &indices);

        //ADVERTENCIA: cuando la cuantificación de alguno de los ejes
        //de los posicioandores adscritos esta deactivada, SearchCollinding
        //comprobará si hay colisión en las posiciones cuantificadas de
        //los ejes que tengan la cuantificación activada, con lo cual
        //el punto P3 de los posicionadores podría no coincidir exactamente
        //con el punto obejtivo.

        //COUNT OF ALLOCATION TYPES:

        /// Count the number of reference sources in the allocation list.
        unsigned int countNR(void) const;
        /// Count the number of blanks in the allocation list.
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
