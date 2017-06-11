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
/// @file MotionProgramValidator.h
/// @brief motion program validator
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef MOTIONPROGRAMVALIDATOR_H
#define MOTIONPROGRAMVALIDATOR_H

#include "FiberMOSModel.h"
#include "MotionProgram.h"

#include <vector>

//--------------------------------------------------------------------------

using namespace Models;

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------

/// @brief Get the list of RPs included in a MP.
/// @pre All message of instruction in the MP:
/// - shall be addressed to an existent RP of the Fiber MOS Model.
void getRPsIncludedInMP(TRoboticPositionerList& RPL,
                        const TMotionProgram& MP,
                        const TFiberMOSModel *FMM);

/// @brief Get the list of RPs included in a pair of MPs.
/// @pre All message of instruction in the MPs:
/// - shall be addressed to an existent RP of the Fiber MOS Model.
void getRPsIncludedInMPs(TRoboticPositionerList& RPL,
                     const TMotionProgram& MP1, const TMotionProgram& MP2,
                     const TFiberMOSModel *FMM);

//###########################################################################
//TMotionProgramValidator:
//###########################################################################

/// @brief A Motion Program Validator (MPV) provide functions for validate
/// MPs (Motion Programs). Validation process use the Variable Jump Method.
/// The process consume a bit of the Security Perimetral Margin (SPM) for
/// determine apodictically thet the MP avoid collissions.
class TMotionProgramValidator {
protected:
    //EXTERN-ATTACHED OBJECTS:

    TFiberMOSModel *FiberMOSModel;

    //DEFINITION: minimun free distance between two barriers (Dmin)
    //is the distance between the barriers less the SPM of each:
    //  Dmin = Barrier1->Contour.DistanceMin(Barrier2->Contour) -
    //      Barrier1->SPM - Barrier2->SPM.
    //Each barrier can be an arm or to be part of an exclusion area.

    //DEFINITION: minimun free time between two barriers (Tfmin)
    //is displacement time during which there is garantee
    //that cannot collide.

    /// Calculates the time free of collission of a RP and an EA.
    double calculateTf(TRoboticPositioner *RP,
                       const TExclusionArea *EAA) const;
    /// Calculates the time free of collission of two RPs.
    double calculateTf(TRoboticPositioner *RP,
                       const TRoboticPositioner *RPA) const;
    /// Calculates the minimun step time of a pair (RP, EA).
    double calculateTmin(const TRoboticPositioner *RP,
                         const TExclusionArea *EAA) const;
    /// Calculates the minimun step time of two RPs.
    double calculateTmin(const TRoboticPositioner *RP,
                         const TRoboticPositioner *RPA) const;

    /// @brief Calculates the minimun time free of collission of
    /// a RP with their adjacents.
    double calculateTfmin(TRoboticPositioner *RP) const;
    /// @brief Calculates the minimun step time of
    /// a RP with their adjacents.
    double calculateTminmin(const TRoboticPositioner *RP) const;

    /// @brief Calculates the minimun time free of collission of
    /// the RPs of a list.
    double calculateTfmin(const TRoboticPositionerList& RPL) const;
    /// @brief Calculates the minimun step time of
    /// the RPs of a list.
    double calculateTminmin(const TRoboticPositionerList& RPL) const;

public:
    //EXTERN-ATTACHED OBJECTS:

    /// Get extern attached Fiber MOS Model.
    TFiberMOSModel *getFiberMOSModel(void) const {
        return FiberMOSModel;}

    //BUILDING AND DESTROYING METHODS:

    /// @brief Built a validator of motion programs
    /// attached to an extern Fiber MOS Model.
    TMotionProgramValidator(TFiberMOSModel *FiberMOSModel);

    //METHODS TO VALIDATE MOTION PROGRAMS:

    /// @brief Determines if the execution of a motion program, starting from
    /// given initial positions, avoid collisions.
    /// @param[in] MP: motion program to be validated
    /// @return true: if the motion program avoid collisions.
    /// @pre All RPs included in the MP:
    /// - must be enabled the quantifiers of their rotors.
    /// @pre All RPs of the FMM:
    /// - shall be in their initial positions;
    /// - must have the aadecuate SPM.
    /// @post If the MP produces a collision, all RPs of the FMM:
    /// - will have disabled the quantifiers of their rotors.
    /// - will be in the firstposition where collission was detected.
    /// @post If the MP avoid collisions, all RPs of the FMM:
    /// - will have the quantifiers of their rotors in their initial status,
    /// - will be in their final positions.
    /// @note The validation process of a MP consume a component of the SPM, even
    /// when the process is successfully passed. So if a MP pass the validation
    /// process with a value of SPM, the validation shall be make with
    /// the value of SPM inmediately lower.
    /// @note The validation method of a MP will be used during the generation process
    /// with the individual MP of each RP, and at the end of the process for
    /// validate the generated recovery program.
    bool validateMotionProgram(TMotionProgram &MP) const;

    //Validation of a MP can end of two ways:
    //- If the MP not produce a dynamic collision, being all RPs
    //  in their final positions.
    //- If the MP produce a dynamic collisions, being all RPs
    //  in the firs position where collision has been detected.

    /// @brief Validate a pair (PP, DP) in a limited way.
    /// @param[in] (PP, DP):the pair to validate.
    /// @return true: if all RPs includes in the pair are operatives.
    /// @pre The status of the Fiber MOS Model must correspond to the status of
    /// the real Fiber MOS.
    bool checkPairPPDP(const TMotionProgram &PP,
                         const TMotionProgram &DP) const;

};

//---------------------------------------------------------------------------

} //namespace Positiong

//---------------------------------------------------------------------------
#endif // MOTIONPROGRAMVALIDATOR_H
