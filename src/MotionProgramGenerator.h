// Copyright (c) 2014-2015 Isaac Morales Durán. All rights reserved.
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
//File: MotionProgramGenerator.h
//Content: generator of motionprograms
//Last update: 26/12/2014
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef MOTIONPROGRAMGENERATOR_H
#define MOTIONPROGRAMGENERATOR_H

#include "MotionProgramValidator.h"
#include "TargetPointList.h"
#include "FiberMOSModel2.h"
#include "MotionProgram.h"

//---------------------------------------------------------------------------

using namespace Models;

//namespace for positioning
namespace Positioning {

//###########################################################################
//TMotionProgramGenerator:
//###########################################################################

//A MPG (Motion Program Generator) provide functions to generates MPs (motion
//programs). For this, the MPG shall be use the Fiber MOS Model to perform
//simulations. All MPs  generated by the MPG will be valid, is to say they
//will avoid dynamic collisions. For accomplish of this purpose the MPG
//will make use of the validator function, to assure that the generation
//process is valid. Each MP shall go with an IPL (Initial Position List)
//of type TPairsPositionAngleList.

//class motion program generator
class TMotionProgramGenerator : public TMotionProgramValidator,
        public TTargetPointList {
protected:
    //SETTING PARAMETERS OF THE ALGORITHMS:

    double __dt_jmp;

public:
    //SETTING PROPERTIES:

    //jump (in negative direction) in radians, it is performed each time
    //that a collission is detected during the radial retraction
    //shall be upper zero
    //default value: -M_PI/36 rad
    double getdt_jmp(void) const {return __dt_jmp;}

    //It is desirable that the jump is expressed in radians and not in steps,
    //for several reasons:
    // - If the number of steps change, the jump not should vary.
    // - The rotor position and their domain limits, are stored in radians
    //   so that the checking belonging to domain, could introduce numerical
    //   errors.

    //Note that when the jump correspond to a integer number of steps, when
    //quantification is enabled, the rotors do not change their positions.

    //-----------------------------------------------------------------------
    //BUILDING AND DESTROYING METHODS:

    //build a motion program generator
    //attached to a robotic positionerlist
    TMotionProgramGenerator(TFiberMOSModel *FiberMOSModel);
    //destroy the targetpoints and destroy the MPG
    ~TMotionProgramGenerator(void);

    //-----------------------------------------------------------------------
    //METHODS TO GENERATE MPs:

    //Programs the retraction of all RP of the list Outsiders_.
    //Preconditions:
    //  All RPs of the list Outsiders_ shall be in unsecure position.
    //  All RPs of the list Outsiders_ shall has enabled the quantifiers.
    void programRetraction(TRoboticPositionerList &Outsiders_);

    //Segregate the RPs of the list Outsiders, in disjoint subsets.
    //Preconditions:
    //  All RPs in the list Outsiders shall be operatives
    //  All RPs in the list Outsiders shall be in unsecure positions
    void segregateRPsInDisjointSets(
            TPointersList<TRoboticPositionerList>& DisjointSets,
            const TRoboticPositionerList& Outsiders);

    //Determines if not all RPs of a list follow the MEGARA distribution.
    //Preconditions:
    //  All RPs of the disjoint set shall be separated
    //  a distance D - 2*L or upper.
    bool notAllRPsFollowMEGARADistribution(
            const TRoboticPositionerList& Set);

    //Segregates the RPs of a set, in disperse subsets.
    //Preconditions:
    //  Pointer Subsets should point to built list of disperse subsets.
    //  All RPs of the list Set shall follow the MEGARA distribution.
    //  All RPs in the list Set shall be in the Fiber MOS Model.
    void segregateRPsInDisperseSubsets(
            TPointersList<TRoboticPositionerList > *Subsets,
            TRoboticPositionerList& Set);

    //Determines the limits of the open interval (l1, l2)
    //where RP collide with RPA.
    //Inputs:
    //  RP: robotic positioner which rotor 1 shall be displaced.
    //  RPA: roboticpositioner adjacentto RP which shall remains stoped.
    //Outputs:
    //  l1: lower limit of interval (l1, l2)
    //  l2: upper limit of interval (l1, l2)
    //Preconditions:
    //  RP shall beenabled their rotor 1 quantifier.
    //Posconditions:
    //  Robotic positioner RP and RPA remains without change,
    //      and the limits l1and l2 has been determined.
    //Meaning:
    //  When l1 == l2 there isn't collision. (In this casemoreover l1 == p1 ==l2).
    //  When l1 < l2, the soltion can be searched in both senses.
    //  When l2 < l1, the solution shall be searched:
    //      in positive sense, when p_1 is in [p_1min, l2)
    //      in negative sense, when p_1 is in (l1, p_1max]
    void determinesCollisionInterval(double& l1, double& l2,
            TRoboticPositioner *RP, const TRoboticPositioner *RPA);

    //Determines if a RP in collision status can solve
    //their collision turning the rotor 1,
    //and calculates the new position of the rotor 1.
    //Inputs:
    //  RP: the robotic positioner in collission status.
    //Onputs:
    //- collisionCanbesolved: flag indicating if collision can be solved
    //  turning the rotor 1.
    //- p_1new: the position which the rotor 1 of the RP shall jump
    //  to solve the collision.
    //Preconditions:
    //  Pointer RP shall  point to built robotic positioner.
    //  The RP shall be in collision sattus with one or more adjacent RPs.
    //  The RP shall has enabled their rotor 1 quantifier.
    //  The rotor 1 of the RP shall has stacked one position almost.
    //Posconditions:
    //  Te RP will be in their initial status, and the proposed quastion
    //  will be solved.
    bool collisionCanBesolved(double& p_1new, TRoboticPositioner *RP);

    //Determines the RPs which can be retracted in each subset of each set.
    //Inputs:
    //  DDS: structure to contains disjoint disperse subsets
    //Outputs:
    //  RetractilesDDS: structure to contains the RPs which can be retracted
    //  in one or two gestures avoiding dynamic collisions.
    //  InvadersDDS: structure to contains the RPs which can not be retracted
    //  in one or two gestures avoiding dynamic collisions.
    //Preconditions:
    //  All RPs of the Fiber MOS Model shall be configurated to
    //  motion program generation.
    //  All RPs of DDS shall be in the Fiber MOS  Model
    //  All RPs of DDS shall be in insecure position
    //  All RPs of DDS shall have enabled the quantifiers
    //  All RPs of DDS shall be free of dynamic collisions
    //  All RPs of DDS shall to have programmed a gesture of retraction
    //Posconditions:
    //  RetractilesDDS will contains the RPs which can be retracted
    //  in one or two gestures.
    //  InvadersDDS will contains the RPs which can not be retracted
    //  in one or two gestures.
    void segregateRetractilesInvaders(
      TPointersList<TPointersList<TRoboticPositionerList> >& RetractilesDDS,
      TPointersList<TPointersList<TRoboticPositionerList> >& InvadersDDS,
      TPointersList<TPointersList<TRoboticPositionerList> >& DDS);

    //Add to the DP, the corresponding list or lists
    //of message of instructions
    //Inputs:
    //  RPsToBeRetracted: list of the RPs which has been retracted.
    //  DP: depositioning program to bemodified.
    //Outputs:
    //  DP: depositioning program modified.
    //Preconditions:
    //  All RPs of the list RPsToBeRetracted shall have stacked the initial
    //      positions of their rotors.
    //  All RPs of the list RPsToBeRetracted shall be in their stacked positions.
    //  All RPs of the list RPsToBeRetracted shall have programmed a gesture.
    void addMessageLists(TMotionProgram& DP,
            const TRoboticPositionerList& RPsToBeRetracted);

    //Add to the DP the message-instruction list to move the RPs
    //of the list Inners to the origins
    //Inputs:
    //  Inners: list of operative RPs in seciry position out the origin.
    //Output:
    //  DP: depositioning program which the message list will be added.
    //Preconditions:
    //  All RPs of the list Inners shall be operatives in secure position
    //  but out the origin.
    void addMessageListToGoToTheOrigins(TMotionProgram& DP,
        const TRoboticPositionerList& Inners);

    //NOTE: nomenclature:
    //- Positioning Program: to go the observing positiong, need has a first
    //  gesture to go to the starting positions in the secure area.
    //- Depositioning Program: antagonicus of PP, include a final gesture
    //  tomove therotors to their origins.
    //- Recovery Program: is a DP without the final gesture to go to the
    //  origins.
    //- Parking Gesture: gesture to mo the rotors to their origins.

    //Generates a recovery program for a given set of operative RPs
    //in unsecure positions and determines the RPs of the given set,
    //which can not be recovered because are in collision status
    //or because are obstructed in unsecure positions.
    //Preconditions:
    //  All RPs of the Fiber MOS Model, shall be in their initial positions.
    //  All RPs of the list Outsiders, shall be in the Fiber MOS Model.
    //  All RPs of the list Outsiders, shall be operatives.
    //  All RPs of the list Outsiders, shall be in unsecure positions.
    //  All RPs of the list Outsiders, shall have enabled the quantifiers.
    //Inputs:
    //  FiberMOSModel: Fiber MOS Model with RPs in their initial positions.
    //  Outsiders: list of operative RPs in insecure positions which
    //      we want recover the security positions.
    //Outputs:
    //  generateDepositioningProgram: flag indicating if the DP can be
    //      generated or not with this function.
    //  Collided: list of RPs collided in unsecure position.
    //  Obstructed: list of RPs obstructed in unsecure position.
    //  MP: recovery program.
    bool generateRecoveryProgram(TRoboticPositionerList& Collided,
        TRoboticPositionerList& Obstructed, TMotionProgram& MP,
        const TRoboticPositionerList& Outsiders);

    //Generates a depositioning program for a given set of operative RPs
    //in unsecure positions and determines the RPs of the given set,
    //which can not be recovered because are in collision status
    //or because are obstructed in unsecure positions.
    //Preconditions:
    //  All RPs of the Fiber MOS Model, shall be in their initial positions.
    //  All RPs of the list Outsiders, shall be in the Fiber MOS Model.
    //  All RPs of the list Outsiders, shall be operatives.
    //  All RPs of the list Outsiders, shall be in unsecure positions.
    //  All RPs of the list Outsiders, shall have enabled the quantifiers.
    //Inputs:
    //  FiberMOSModel: Fiber MOS Model with RPs in their initial positions.
    //  Outsiders: list of operative RPs in insecure positions which
    //      we want recover the security positions.
    //Outputs:
    //  generateParkingProgram: flag indicating if the parking program
    //      can be generated or not with this function.
    //  Collided: list of RPs collided in unsecure position.
    //  Obstructed: list of RPs obstructed in unsecure position.
    //  DP: depositioning program.
    bool generateDepositioningProgram(TRoboticPositionerList& Collided,
        TRoboticPositionerList& Obstructed, TMotionProgram& DP,
        const TRoboticPositionerList& Outsiders);

    //Generates a positioning program from a given depositioning program.
    void generatePositioningProgram(TMotionProgram& PP,
        const TMotionProgram& DP, const TPairPositionAnglesList& IPL);

    //Generates a pair (PP, DP) for a given set of operative RPs
    //in unsecure positions and determines the RPs of the given set,
    //which can not be recovered because are in collision status
    //or because are obstructed in unsecure positions.
    //Preconditions:
    //  All RPs of the Fiber MOS Model, shall be in their observing positions.
    //  All RPs of the list Outsiders, shall be in the Fiber MOS Model.
    //  All RPs of the list Outsiders, shall be operatives.
    //  All RPs of the list Outsiders, shall be in unsecure positions.
    //  All RPs of the list Outsiders, shall have enabled the quantifiers.
    //Inputs:
    //  FiberMOSModel: Fiber MOS Model with RPs in their observing positions.
    //  Outsiders: list of operative RPs in insecure positions which
    //      we want recover the security positions.
    //Outputs:
    //  generateParkingProgram: flag indicating if the pair (PP, DP)
    //      can be generated or not with this function.
    //  Collided: list of RPs collided in unsecure position.
    //  Obstructed: list of RPs obstructed in unsecure position.
    //  PP: positioning program.
    //  DP: depositioning program.
    bool generatePairPPDP(TRoboticPositionerList& Collided,
        TRoboticPositionerList& Obstructed, TMotionProgram& PP,
        TMotionProgram& DP, const TRoboticPositionerList& Outsiders);
};

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
#endif // MOTIONPROGRAMGENERATOR_H
