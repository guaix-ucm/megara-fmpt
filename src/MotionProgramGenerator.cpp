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
/// @file MotionProgramGenerator.cpp
/// @brief motion program generator
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#include "globalconsts.h"
#include "MotionProgramGenerator.h"
#include "MessageInstruction.h"
#include "SkyPoint.h"
#include "FileMethods.h"
#include "OutputsParkProg.h" //for function generateParkProg_online
#include "OutputsPairPPDP.h" //for function generatePairPPDP_online
#include "adjacentitem.h"

#include <algorithm> //std::min, std::max
#include <config.h> //PACKAGE_VERSION

//---------------------------------------------------------------------------

using namespace Models;

//spacename for positioning
namespace Positioning {

//###########################################################################
//TMotionProgramGenerator:
//###########################################################################

//---------------------------------------------------------------------------
//METHODS FOR SEGREGATE SUBSETS OF RPs:

//Segregate the RPs of the list Outsiders, in disjoint subsets.
//Inputs:
//  Outsiders: list of operative outsiders RPs tobe segregated.
//Outputs:
//  DisjointSets: list of disjoint sets.
//Preconditions:
//  All RPs in the list Outsiders:
//      shall be operatives.
//      shall be in insecurity positions.
void TMotionProgramGenerator::segregateRPsInDisjointSets(
        TPointersList<TRoboticPositionerList>& DisjointSets,
        const TRoboticPositionerList& Outsiders)
{
    //CHECK THE PRECONDITIONS:

    for(int i=0; i<Outsiders.getCount(); i++) {
        TRoboticPositioner *RP = Outsiders[i];
        if(!RP->getOperative())
            throw EImproperArgument("all RPs in the list Outsiders shall be operative");
        if(RP->getActuator()->ArmIsInSafeArea())
            throw EImproperArgument("all RPs in the list Outsiders shall be in insecurity positions");
    }

    //MAKE ACTIONS:

    //initialize the list of disjoint sets
    DisjointSets.Clear();

    //The RPs in insecurity position, which not are in the list Outsiders,
    //shall be taken how obstacles, so that the sets separated only for
    //these RPs, are disjoint sets.

    //for each RP of the list Outsiders
    for(int i=0; i<Outsiders.getCount(); i++) {
        //points the indicated RP to facilitate its access
        TRoboticPositioner *RP = Outsiders[i];

        //determines the list of adjacent RPs which are in insecurity position
        //and are in the list Outsiders
        TItemsList<TRoboticPositioner*> Adjacents;
        for(int j=0; j<RP->getActuator()->AdjacentRPs.getCount(); j++) {
            TRoboticPositioner *RPA = RP->getActuator()->AdjacentRPs[j].RP;
            if(RPA->getActuator()->ArmIsOutSafeArea()) {
                int k = Outsiders.Search(RPA);
                if(k < Outsiders.getCount())
                    Adjacents.Add(RPA);
            }
        }

        //NOTE: list Adjacents is of type TItemsList<TRoboticPositioner*>,
        //not TRoboticPositionerList.

        //Here has been found the adjacents of a RP in the list RP->getActuator()->AdjacentRPs.
        //The content of that list is determined when the instance of the Fiber MOS Model is assimilated.
        //That list will contains the RPs whose points P0 are to a distance less than:
        //  RPi->getActuator()->getr_max() + RPi->getActuator()->getSPMall_a() +
        //  RPj->getActuator()->getr_max() + RPj->getActuator()->getSPMall_a()

        //search the adjacents in the disjoint sets
        TVector<int> index;
        for(int j=0; j<Adjacents.getCount(); j++) {
            TRoboticPositioner *RPA = Adjacents[j];
            for(int k=0; k<DisjointSets.getCount(); k++) {
                TRoboticPositionerList *Set = DisjointSets.GetPointer(k);
                int l = Set->Search(RPA);
                int m = index.Search(k);
                if(l<Set->getCount() && m>=index.getCount())
                    index.Add(k);
            }
        }

        //if not found the adjacents in the disjoint sets
        if(index.getCount() <= 0) {
            //add the RP in a new set
            TRoboticPositionerList *Set = new TRoboticPositionerList();
            Set->Add(RP);
            DisjointSets.Add(Set);
        }
        //if has found some adjacent in the disjoint sets
        else {
            //join the disjoint sets where the RPAs was found
            index.SortInc();
            TRoboticPositionerList *Set = DisjointSets.GetPointer(index[0]);
            for(int j=1; j<index.getCount(); j++) {
                TRoboticPositionerList *SetToAdd = DisjointSets.GetPointer(index[j]);
                for(int k=0; k<SetToAdd->getCount(); k++) {
                    TRoboticPositioner *RPToAdd = SetToAdd->Get(k);
                    Set->Add(RPToAdd);
                }
            }
            for(int j=index.getCount()-1; j>=1; j--)
                DisjointSets.Delete(index[j]);
            //add the RP to the Set
            Set->Add(RP);
        }
    }
}

//Determines if not all RPs of a list follow the MEGARA distribution.
//Inputs:
//  Set: disjoint set to check.
//Outputs:
//  notAllRPsFollowMEGARADistribution: indicates if the RPs of
//      the disjoint set follow a MEGARA distribution.
//Preconditions:
//  All RPs of the disjoint set:
//      shall be separated a distance D - 2*L or upper.
bool TMotionProgramGenerator::notAllRPsFollowMEGARADistribution(
        const TRoboticPositionerList& Set)
{
    //CHECK THE PRECONDITIONS:

    for(int i=1; i<Set.getCount(); i++) {
        TRoboticPositioner *RP = Set[i];
        for(int j=0; j<i; j++)
            if(Mod(Set[j]->getActuator()->getP0() - RP->getActuator()->getP0()) <= 4*MEGARA_L*cos(M_PI/6) - 2*MEGARA_L)
                throw EImproperArgument("all RPs of the disjoint set shall be separated a distance D - 2*L or upper");
    }

    //MAKE ACTIONS:

    //calculate the constants
    double D = 4*MEGARA_L*cos(M_PI/6); //distance between points P0
    double Sx = 3*D;        //base of the rectangle
    double Sy = 6*MEGARA_L; //height of the rectangle

    //for each RP of the set
    for(int i=0; i<Set.getCount(); i++) {
        //point the indicated RP to facilitate its access
        TRoboticPositioner *RP = Set[i];

        //APPLY A DISPLACEMENT TO THE POINT P0 OF THE RP,
        //FOR THAT IS ON THE TEMPLATE:

        //assign the P0 of the RP to facilitate its access
        TDoublePoint P0 = RP->getActuator()->getP0();

        //displace the point P0 to the rectangle ({0, 0}, {Sx, Sy})
        P0.x -= floor(P0.x/Sx)*Sx;
        P0.y -= floor(P0.y/Sy)*Sy;

        //ASSIGN THE RP TO THE CORRESPONDING SUBSET:

        bool aux1 = Mod(P0 - TDoublePoint(0, Sy/2)) > MEGARA_L - ERR_NUM;
        bool aux2 = Mod(P0 - TDoublePoint(Sx, Sy/2)) > MEGARA_L - ERR_NUM;
        bool aux3 = Mod(P0 - TDoublePoint(Sx/2, 0)) > MEGARA_L - ERR_NUM;
        bool aux4 = Mod(P0 - TDoublePoint(Sx/2, Sy)) > MEGARA_L - ERR_NUM;
        bool aux5 = Mod(P0 - TDoublePoint(D, Sy/2)) > MEGARA_L - ERR_NUM;
        bool aux6 = Mod(P0 - TDoublePoint(D*5/2, 0)) > MEGARA_L - ERR_NUM;
        bool aux7 = Mod(P0 - TDoublePoint(D*5/2, Sy)) > MEGARA_L - ERR_NUM;
        bool aux8 = Mod(P0 - TDoublePoint(D/2, 0)) > MEGARA_L - ERR_NUM;
        bool aux9 = Mod(P0 - TDoublePoint(D/2, Sy)) > MEGARA_L - ERR_NUM;
        bool aux10 = Mod(P0 - TDoublePoint(2*D, Sy/2)) > MEGARA_L - ERR_NUM;

        //if the point P0 isn't on:
        //Subset 1:
        //  Circle ({0, Sy/2}, L)
        //  Circle ({Sx, Sy/2}, L)
        //  Circle ({Sx/2, 0}, L)
        //  Circle ({Sx/2, Sy}, L)
        //Subset 2:
        //  Circle ({D, Sy/2}, L)
        //  Circle ({D*5/2, 0}, L)
        //  Circle ({D*5/2, Sy}, L)
        //Subset 3:
        //  Circle ({D/2, 0}, L)
        //  Circle ({D/2, Sy}, L)
        //  Circle ({2*D, Sy/2}, L)
        //indicates that not all RPs follow the MEGARA distribution
        if(aux1 && aux2 && aux3 && aux4 && aux5 && aux6 && aux7 && aux8 && aux9 && aux10)
            return true;
    }
    //indicates that all RPs follow the MEGARA distribution
    return false;
}

//Segregates the RPs of a set, in disperse subsets.
//Inputs:
//  Set: disjoint set to be splited.
//Outputs:
//  Subsets: list of disperse subsets.
//Preconditions:
//  Pointer Subsets should point to built list of disperse subsets.
//  All RPs of the list Set:
//      shall follow the MEGARA distribution;
//      shall be in the Fiber MOS Model.
void TMotionProgramGenerator::segregateRPsInDisperseSubsets(
        TPointersList<TRoboticPositionerList> *DisperseSubsets,
        TRoboticPositionerList& Set)
{
    //CHECK THE PRECONDITIONS:

    if(DisperseSubsets == NULL)
        throw EImproperArgument("pointer DisperseSubsets should point to built list of disperse subsets");

    if(notAllRPsFollowMEGARADistribution(Set))
        throw EImproperArgument("all RPs should follow MEGARA distribution");

    for(int i=0; i<Set.getCount(); i++) {
        TRoboticPositioner *RP = Set[i];
        int j = getFiberMOSModel()->RPL.search(RP->getActuator());
        if(j >= getFiberMOSModel()->RPL.getCount())
            throw EImproperArgument("all RPs in the disjoint set shall be in the Fiber MOS Model");
    }

    //MAKE ACTIONS:

    //For each RP, displace its point P0:
    //  x - m*(3*D)
    //  y - n*(6*L);
    //where m and n are integers, until the point P0 is on a circle
    //of the following template:
    //Subset 1:
    //  Circle ({0, Sy/2}, L)
    //  Circle ({Sx, Sy/2}, L)
    //  Circle ({Sx/2, 0}, L)
    //  Circle ({Sx/2, Sy}, L)
    //Subset 2:
    //  Circle ({D, Sy/2}, L)
    //  Circle ({D*5/2, 0}, L)
    //  Circle ({D*5/2, Sy}, L)
    //Subset 3:
    //  Circle ({D/2, 0}, L)
    //  Circle ({D/2, Sy}, L)
    //  Circle ({2*D, Sy/2}, L)

    //calculate the constants
    double D = 4*MEGARA_L*cos(M_PI/6); //distance between points P0
    double Sx = 3*D;        //base of the rectangle
    double Sy = 6*MEGARA_L; //height of the rectangle

    TRoboticPositionerList *Subset1 = NULL;
    TRoboticPositionerList *Subset2 = NULL;
    TRoboticPositionerList *Subset3 = NULL;

    //for each RP of the set
    for(int i=0; i<Set.getCount(); i++) {
        //point the indicated RP to facilitate its access
        TRoboticPositioner *RP = Set[i];

        //APPLY A DISPLACEMENT TO THE POINT P0 OF THE RP,
        //FOR THAT IS ON THE TEMPLATE:

        //assign the P0 of the RP to facilitate its access
        TDoublePoint P0 = RP->getActuator()->getP0();

        //displace the point P0 to the rectangle ({0, 0}, {Sx, Sy})
        P0.x -= floor(P0.x/Sx)*Sx;
        P0.y -= floor(P0.y/Sy)*Sy;

        //ASSIGN THE RP TO THE CORRESPONDING SUBSET:

        //if the point P0 is on:
        //Subset 1:
        //  Circle ({0, Sy/2}, L)
        //  Circle ({Sx, Sy/2}, L)
        //  Circle ({Sx/2, 0}, L)
        //  Circle ({Sx/2, Sy}, L)
        //the RP shall be added to subset 1
        if((Mod(P0 - TDoublePoint(0, Sy/2)) <= MEGARA_L) ||
                (Mod(P0 - TDoublePoint(Sx, Sy/2)) <= MEGARA_L) ||
                (Mod(P0 - TDoublePoint(Sx/2, 0)) <= MEGARA_L) ||
                (Mod(P0 - TDoublePoint(Sx/2, Sy)) <= MEGARA_L)) {
            //add the RP to subset 1
            if(Subset1 == NULL)
                Subset1 = new TRoboticPositionerList();
            Subset1->Add(RP);
        }

        //else if the point P0 is on:
        //Subset 2:
        //  Circle ({D, Sy/2}, L)
        //  Circle ({D*5/2, 0}, L)
        //  Circle ({D*5/2, Sy}, L)
        //the RP shall be added to subset 2
        else if((Mod(P0 - TDoublePoint(D, Sy/2)) <= MEGARA_L) ||
                (Mod(P0 - TDoublePoint(D*5/2, 0)) <= MEGARA_L) ||
                (Mod(P0 - TDoublePoint(D*5/2, Sy)) <= MEGARA_L)) {
            //add the RP to subset 2
            if(Subset2 == NULL)
                Subset2 = new TRoboticPositionerList();
            Subset2->Add(RP);
        }

        //else if the point P0 is on:
        //Subset 3:
        //  Circle ({D/2, 0}, L)
        //  Circle ({D/2, Sy}, L)
        //  Circle ({2*D, Sy/2}, L)
        //the RP shall be added to subset 3
        else if((Mod(P0 - TDoublePoint(D/2, 0)) <= MEGARA_L) ||
                (Mod(P0 - TDoublePoint(D/2, Sy)) <= MEGARA_L) ||
                (Mod(P0 - TDoublePoint(2*D, Sy/2)) <= MEGARA_L)) {
            //add the RP to subset 3
            if(Subset3 == NULL)
                Subset3 = new TRoboticPositionerList();
            Subset3->Add(RP);
        }

        //else indicate that:
        //  the RP is more far that MEGARA_L from its theoretical position
        else
            throw EImpossibleError("the RP is more far that MEGARA_L from its theoretical position");
    }

    //ADD THE SUBSETS TO THE LIST OF DISPERSE SUBSETS:

    if(Subset1 != NULL)
        DisperseSubsets->Add(Subset1);
    if(Subset2 != NULL)
        DisperseSubsets->Add(Subset2);
    if(Subset3 != NULL)
        DisperseSubsets->Add(Subset3);
}

//---------------------------------------------------------------------------
//METHODS FOR PROGRAM THE RPs AND SEGREGATE THE RECOVERABLES:

//Determines if a pair of motion programs (MPturn, MPretraction)
//is incongruent. For this make the following action:
//
//For the MPturn:
//  check if contains none or one message list
//  if the MPturn contains a message list:
//      check if the message list contains one message instruction
//      check if the message instruction is directed to a RP of the FMM
//      check if the instruction is for turn the rotor 1
//
//For the MPretraction:
//  check if contains one or two message list
//  check if in each message list:
//      the message list contains one message instruction
//      the message instruction is directed to a RP of the FMM
//  check if the first instruction is for retraction
//  check if the second instruction (if any) is for abatement
//
//For the pair (MPturn, MPretraction):
//  check if all message instructions are directed to the same RP
//
//Inputs:
//  (MPturn, MPretraction): the pair of MPs to check
//Outputs:
//  RP: the RP included in the pair (MPturn, MPretraction)
//      when the pair is congruent
//  motionProgramsAreIncongruent: flag indicating if
//      the pair is incongruent
bool TMotionProgramGenerator::motionProgramsAreIncongruent(TRoboticPositioner* &RP,
                                                           const TMotionProgram& MPturn,
                                                           const TMotionProgram& MPretraction) const
{
    //initialize the output
    RP = NULL;
    //vector for add the Ids of the RPs which go directed the isntructions
    vector<int> Ids;
    //index to the RP attached to the pair (MPturn, MPretraction)
    int j;

    //--------------------------------------------------------
    //For the MPturn:

    //check if contains none or one message list
    if(MPturn.getCount() > 1)
        return true;

    //if the MPturn contains a message list:
    if(MPturn.getCount() == 1) {
        //point the message list for easier their access
        TMessageList *ML = MPturn.getFirstPointer();

        //check if the message list contains one message instruction
        if(ML->getCount() != 1)
            return true;

        //point the message instruction for easier their access
        TMessageInstruction *MI = ML->getFirstPointer();

        //search the RP of the message list in the FMM
        j = getFiberMOSModel()->RPL.searchId(MI->getId());

        //check if the message instruction is directed to a RP of the FMM
        if(j >= getFiberMOSModel()->RPL.getCount())
            return true;

        //add the Id of the message list to the vector
        Ids.push_back(MI->getId());

        //check if the instruction is for turn the rotor 1
        if(MI->Instruction.getName() != "M1")
            return true;
    }

    //--------------------------------------------------------
    //For the MPretraction:

    //check if contains one or two message list
    if(MPretraction.getCount()<1 || 2<MPretraction.getCount())
        return true;

    //check if in each message list...
    for(int i=0; i<MPretraction.getCount(); i++) {
        //point the message list for easier their access
        const TMessageList *ML = MPretraction.GetPointer(i);

        //...the message list contains one message instruction
        if(ML->getCount() != 1)
            return true;

        //point the message instruction for easier their access
        TMessageInstruction *MI = ML->getFirstPointer();

        //search the RP of the message list in the FMM
        j = getFiberMOSModel()->RPL.searchId(MI->getId());

        //...the message instruction is directed to a RP of the FMM
        if(j >= getFiberMOSModel()->RPL.getCount())
            return true;

        //add the Id of the message list to the vector
        Ids.push_back(MI->getId());
    }

    //check when there is a single instruction
    if(MPretraction.getCount() == 1)
        //the single instruction may be either for retraccion or abatement
        if(MPretraction[0][0].Instruction.getName() != "MM" && MPretraction[0][0].Instruction.getName() != "M2")
            return true;
    //check when there are two instructions
    if(MPretraction.getCount() == 2) {
        //the first intruccions must be for retraccion
        if(MPretraction[0][0].Instruction.getName() != "MM")
            return true;
        //the second intruccions must be for abatement
        if(MPretraction[1][0].Instruction.getName() != "M2")
            return true;
    }

    //--------------------------------------------------------
    //For the pair (MPturn, MPretraction):

    //check if all message instructions are directed to the same RP
    for(unsigned int i=1; i<Ids.size(); i++)
        if(Ids[i] != Ids[0])
            return true;

    //point the RP attached to the pair (MPturn, MPretraction)
    RP = getFiberMOSModel()->RPL[j];

    return false; //indicates that the pair is congruent
}

//Determines if a pair of motion programs (MPturn, MPretraction)
//produces a colission,
//Preconditions:
//  All RPs of the Fiber MOS Model:
//      shall be configured for MP generation;
//  The pair (MPturn, MPretraction):
//      shall be congruent.
//  The RP included in the pair (MPturn, MPretraction):
//      shall has enabled the quantifiers of their rotors;
//Postconditions:
//  If the MPretraction contains a single message list:
//      the single message list shall contains a single message
//      instruction for retract the arm.
//  If the MPretraction contains two messages of instruction:
//      the first message list shall contains a single message
//      instruction for retract the arm,
//      and the second message list shall contains a single message
//      instruction for abate the arm.
//Inputs:
//  (MPturn, MPretraction): the pair of MPs to validate
//  motionProgramsAreIncongruent: flag indicating if
//      the pair avoid collisions
bool TMotionProgramGenerator::motionProgramsAreValid(const TMotionProgram& MPturn,
                                                     const TMotionProgram& MPretraction) const
{
    //CHECK THE PRECONDITIONS:

    for(int i=0; i<getFiberMOSModel()->RPL.getCount(); i++) {
        TRoboticPositioner *RP = getFiberMOSModel()->RPL[i];
        if(RP->getActuator()->getPurpose()!=pGenPairPPDP && RP->getActuator()->getPurpose()!=pGenParPro)
            throw EImproperCall("all RPs of the Fiber MOS Model should be configured for MP generation");
    }

    TRoboticPositioner *RP;
    if(motionProgramsAreIncongruent(RP, MPturn, MPretraction))
        throw EImproperArgument("the pair (MPturn, MPretraction) should be congruent");

    if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
        throw EImproperCall("the RP included in the pair (MPturn, MPretraction) should has enabled the quantifiers of their rotors");

    //MAKE ACTIONS:

    //store the actual positions
    getFiberMOSModel()->RPL.pushPositions();

    //validate MPturn
    bool valid = validateMotionProgram(RP->MPturn);

    //Here the RP included in the MPturn, are in the position from which start the retraction.

    if(valid) {
        //change the SPM
        double SPMbak = RP->getActuator()->getArm()->getSPM();
        RP->getActuator()->getArm()->setSPM(SPMbak + RP->getDsec());

        //validate MPretraction
        valid = validateMotionProgram(RP->MPretraction);

        //Here the RP is in the final-security position.

        //restore the initial value of the SPM
        RP->getActuator()->getArm()->setSPM(SPMbak);
    }

    //restore the initial status of the Fiber MOS Model
    getFiberMOSModel()->RPL.restoreAndPopPositions();
    if(!valid)
        getFiberMOSModel()->RPL.restoreAndPopQuantifys();

    //return the result of the validation process
    return valid;
}


//Search in negative sense, the individual MP (composed by MPturn and
//MPretraction), for recover the security position of a RP
//avoiding dynamic collisions.
//Inputs:
//  RP: the robotic positioner to be recovered.
//  dt1max: maximun displacement of the rotor 1 for search the solution
//      in rad. Recomended value (M_PI/2).
//Outputs:
//  p_1new: last position where solution has been searched.
//  RP->MPturn: the generated MP for turn the rotor 1 of the RP,
//      to be executed in first place.
//  RP->MPretraction: the generated MP for radial retraction,
//      to be executed in second place.
//  searchSolutionInNegativeSense: indicates if the solution is valid.
//Preconditions:
//  All RPs of the Fiber MOS Model:
//      shall be configurated for MP generation.
//  The pointer RP:
//      shall point to built robotic positioner.
//  The RP:
//      shall be in the Fiber MOS  Model;
//      shall be in unsecurity position;
//      shall has enabled the quantifiers of their rotors;
//      shall not has dynamic collisions;
//      shall has stacked the actual positions of their rotors.
//  The length of the searching interval dt1max shall be upper zero.
//Postconditions:
//  The RP will be in their initial status.
//  If searchSolutionInNegativeSense == false
//      solution p_1new will be nearest to
//      max(RP->Actuator->theta_1first, theta_1 - dt1max).
bool TMotionProgramGenerator::searchSolutionInNegativeSense(double& p_1new, TRoboticPositioner *RP,
                                                            double dt1max)
{
    //CHECK THE PRECONDITIONS:

    if(dt1max <= 0)
        throw EImproperArgument("the length of the searching interval dt1max should be upper zero");

    if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
        throw EImproperCall("the RP shall has enabled the quantifier of their rotors");

    if(RP->getActuator()->thetasNotCoincideWithStacked())
        throw EImproperCall("the RP should has stacked the actual positions of their rotors");

    //DETERMINE THE SEARCHING INTERVAL [p_1lower, p_1upper]:

    //get the upper position where search the solution
    double theta_1 = RP->getActuator()->gettheta_1();
    double p_1upper = RP->getActuator()->getF().Image(theta_1);
    p_1upper = ceil(p_1upper);

    //get the first position where the rotor 1 can be positioned
    double theta_1first = RP->getActuator()->gettheta_1first();

    //get the lower position where search the solution
    theta_1 -=  dt1max;
    if(theta_1 < theta_1first)
        theta_1 = theta_1first;
    double p_1lower = RP->getActuator()->getF().Image(theta_1);
    p_1lower = ceil(p_1lower);

    //correct the limit case
    if(p_1lower > p_1upper)
        p_1lower = p_1upper;

    //SEARCH THE SOLUTION:

    //WARNING: the binary search is only valid when there is a single adyacent.
    //Therefore will be checked the proposed solution with a single adjacent every time.
    //The content of the list of adjacents shall be restablished at the end of the process.

    //save the content of the list of adjacents
    TAdjacentRPList AdjacentRPs = RP->getActuator()->AdjacentRPs;

    //suppose that there is solution until the contrary is proved
    bool there_is_solution = true;

    //initialize the index for adjacents
    int i = 0;

    //initialize the motion program
    RP->proposeRecoveryProgram();

    //search a solution with an adjacent every time
    while(there_is_solution && i<AdjacentRPs.getCount()) {
        //point the next adjacent RP single in the list of adjacents
        TRoboticPositioner *RPA = AdjacentRPs[i].RP;
        RP->getActuator()->AdjacentRPs.Clear();
        RP->getActuator()->AdjacentRPs.Add(new TAdjacentRP(RPA));

        //determine if the MP is valid (with the single adjacent)
        there_is_solution = motionProgramsAreValid(RP->MPturn, RP->MPretraction);

        //Function motionProgramsAreValid not changes the position of the RP.
        //Then here it is not necesary restablish the initial status of the RP:
        //  RP->getActuator()->restorethetas();

        //if the MP is valid pass to the next adjacent
        if(there_is_solution)
            i++;

        //if the MP is invalid,  perform the binary search of the p_1new (with the single adjacent)
        else {
            //restablish the initial status of the FMM
            //getFiberMOSModel()->RPL.restoreAndPopQuantifys();
            //it is not necesary

            //perform the binary search of the p_1new (with the single adjacent)
            do {
                //calculates the new jumping position
                p_1new = round((p_1lower + p_1upper)/2);

                //propose the program for the new jumping position
                RP->proposeRecoveryProgram(p_1new);

                //determine if the MP is valid (with the single adjacent)
                there_is_solution = motionProgramsAreValid(RP->MPturn, RP->MPretraction);

                //Function motionProgramsAreValid not changes the position of the RP.
                //Then here it is not necesary restablish the initial status of the RP:
                //  RP->getActuator()->restorethetas();

                //adjust the new searching interval
                if(there_is_solution)
                    //displace the lower limit
                    p_1lower = p_1new;

                else {
                    //restablish the initial status of the FMM
                    //getFiberMOSModel()->RPL.restoreAndPopQuantifys();
                    //it is not necesary

                    //displace the upper limit
                    p_1upper = p_1new;
                }

                //while p_1new can be changed
            } while(p_1new!=p_1lower && p_1new!=p_1upper);

            //How the MP has been changed, the searching process must be restarted.

            i = 0; //restart the searching process
        }
    }

    //restablish the content of the list of adjacents
    RP->getActuator()->AdjacentRPs = AdjacentRPs;

    //indicates if there is a solution or not
    return there_is_solution;
}

//Search in positive sense, the individual MP (composed by MPturn and
//MPretraction), for recover the security position of a RP
//avoiding dynamic collisions.
//Inputs:
//  RP: the robotic positioner to be recovered.
//  dt1max: maximun displacement of the rotor 1 for search the solution
//      in rad. Recomended value (M_PI/2).
//Outputs:
//  p_1new: last position where solution has been searched.
//  RP->MPturn: the generated MP for turn the rotor 1 of the RP,
//      to be executed in first place.
//  RP->MPretraction: the generated MP for radial retraction,
//      to be executed in second place.
//  searchSolutionInPositiveSense: indicates if the solution is valid.
//Preconditions:
//  All RPs of the Fiber MOS Model:
//      shall be configurated for MP generation.
//  The pointer RP:
//      shall point to built robotic positioner.
//  The RP:
//      shall be in the Fiber MOS  Model;
//      shall be in unsecurity position;
//      shall has enabled the quantifiers of their rotors;
//      shall not has dynamic collisions;
//      shall has stacked the actual positions of their rotors.
//  The length of the searching interval dt1max shall be upper zero.
//Postconditions:
//  The RP will be in their initial status.
//  If searchSolutionInPositiveSense == false
//      solution p_1new will be nearest to
//      min(RP->Actuator->theta_1last, theta_1 + dt1max).
bool TMotionProgramGenerator::searchSolutionInPositiveSense(double& p_1new, TRoboticPositioner *RP,
                                                            double dt1max)
{
    //CHECK THE PRECONDITIONS:

    if(dt1max <= 0)
        throw EImproperArgument("the length of the searching interval dt1max shall be upper zero");

    if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
        throw EImproperCall("the RP shall has enabled the quantifiers of their rotors");

    if(RP->getActuator()->thetasNotCoincideWithStacked())
        throw EImproperCall("the RP shall has stacked the actual positions of their rotors");

    //DETERMINE THE SEARCHING INTERVAL [p_1lower, p_1upper]:

    //get the lower position where search the solution
    double theta_1 = RP->getActuator()->gettheta_1();
    double p_1 = RP->getActuator()->getF().Image(theta_1);
    double p_1lower = ceil(p_1);

    //get the last position where the rotor 1 can be positioned
    double theta_1last = RP->getActuator()->gettheta_1last();

    //get the upper position where search the solution
    theta_1 = RP->getActuator()->gettheta_1() + dt1max;
    if(theta_1 > theta_1last)
        theta_1 = theta_1last;
    double p_1upper = RP->getActuator()->getF().Image(theta_1);
    p_1upper = floor(p_1upper);

    //correct the limit case
    if(p_1upper < p_1lower)
        p_1upper = p_1lower;

    //SEARCH THE SOLUTION IN THE INTERVAL [p_1lower, p_1upper]

    //WARNING: the binary search is only valid when there is a single adyacent.
    //Therefore will be checked the proposed solution with a single adjacent every time.
    //The content of the list of adjacents shall be restablished at the end of the process.

    //save the content of the list of adjacents
    TAdjacentRPList AdjacentRPs = RP->getActuator()->AdjacentRPs;

    //suppose that there is solution until the contrary is proved
    bool there_is_solution = true;

    //initialize the index for adjacents
    int i = 0;

    //initialize the motion program
    RP->proposeRecoveryProgram();

    //search a solution with an adjacent every time
    while(there_is_solution && i<AdjacentRPs.getCount()) {
        //point the next RP single in the list of adjacents
        TRoboticPositioner *RP_ = AdjacentRPs[i].RP;
        RP->getActuator()->AdjacentRPs.Clear();
        RP->getActuator()->AdjacentRPs.Add(new TAdjacentRP(RP_));

        //determine if the MP is valid (with the single adjacent)
        there_is_solution = motionProgramsAreValid(RP->MPturn, RP->MPretraction);

        //Function motionProgramsAreValid not changes the position of the RP.
        //Then here it is not necesary restablish the initial status of the RP:
        //  RP->getActuator()->restorethetas();

        //if the MP is valid pass to the next adjacent
        if(there_is_solution)
            i++;

        //if the MP is invalid,  perform the binary search of the p_1new (with the single adjacent)
        else {
            //restablish the initial status of the FMM
            //getFiberMOSModel()->RPL.restoreAndPopQuantifys();
            //it is not necesary

            //perform the binary search of the p_1new (with the single adjacent)
            do {
                //calculates the new jumping position
                p_1new = round((p_1lower + p_1upper)/2);

                //propose the program for the new jumping position
                RP->proposeRecoveryProgram(p_1new);

                //determine if the MP is valid (with the single adjacent)
                there_is_solution = motionProgramsAreValid(RP->MPturn, RP->MPretraction);

                //Function motionProgramsAreValid not changes the position of the RP.
                //Then here it is not necesary restablish the initial status of the RP:
                //  RP->getActuator()->restorethetas();

                //adjust the new searching interval
                if(there_is_solution)
                    //displace the upper limit
                    p_1upper = p_1new;

                else {
                    //restablish the initial status of the FMM
                    //getFiberMOSModel()->RPL.restoreAndPopQuantifys();
                    //it is not necesary

                    //displace the lower limit
                    p_1lower = p_1new;
                }

                //while p_1new can be changed
            } while(p_1new!=p_1lower && p_1new!=p_1upper);

            //How the MP has been changed, the searching process must be restarted.

            i = 0; //restart the searching process
        }
    }

    //restablish the content of the list of adjacents
    RP->getActuator()->AdjacentRPs = AdjacentRPs;

    //indicates if there is a solution or not
    return there_is_solution;
}
//Search the best recoveery program for a individual RP.
//Inputs:
//  RP: the RP to be recovered.
//Outputs:
//  searchSolution: indicates if the solution is valid.
//  RP->MPturn: the generated MP for turn the rotor 1 of the RP,
//      to be executed in first place.
//  RP->MPretraction: the generated MP for radial retraction,
//      to be executed in second place.
//  RP->Dsec: security distance additional to SPM during retraction
//Preconditions:
//  The pointer RP shall point to built robotic positioner.
//  All RPs of the Fiber MOS Model:
//      shall be configurated for MP generation.
//  The RP:
//      shall be in the Fiber MOS  Model;
//      shall be in unsecurity position;
//      shall be enabled the quantifiers of their rotors;
//      shall not have dynamic collisions;
//      shall have stacked the actual positions of their rotors.
//Postconditions:
//  The RP will be in their initial status, except their properties
//  (MPturn, MPretraction, Dmin) which will contains the last proposal.
bool TMotionProgramGenerator::searchSolution(TRoboticPositioner *RP)
{
    //CHECK THE PRECONDITIONS:

    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to  built RP");

    for(int i=0; i<getFiberMOSModel()->RPL.getCount(); i++) {
        TRoboticPositioner *RP = getFiberMOSModel()->RPL[i];
        if(RP->getActuator()->getPurpose()!=pGenPairPPDP && RP->getActuator()->getPurpose()!=pGenParPro)
            throw EImproperCall("all RPs of the Fiber MOS Model should be configurated for MP generation");
    }

    int i = getFiberMOSModel()->RPL.Search(RP);
    if(i >= getFiberMOSModel()->RPL.getCount())
        throw EImproperArgument("the RP should be in the Fiber MOS Model");

    if(RP->getActuator()->ArmIsInSafeArea())
        throw EImproperArgument("the RP should be in unsecurity position");

    if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
        throw EImproperArgument("the RP should have enabled the quantifiers of their rotors");

    if(RP->getActuator()->thereIsCollisionWithAdjacent())
        throw EImproperArgument("the RP should not have dynamic collisions");

    if(RP->getActuator()->thetasNotCoincideWithStacked())
        throw EImproperArgument("the RP should have stacked the actual positions of their rotors");

    //MAKE ACTIONS:

    //search a solution in negative sense
    double p_1new;
    bool valid = searchSolutionInNegativeSense(p_1new, RP, dt1Max);

    //The funtion searchSolutionInNegativeSense not change the status of the RP.

    bool RP_is_recoverable;

    //if has found a solution
    if(valid)
        //indicates that the RP is recoverable
        RP_is_recoverable = true;

    //if not has found a solution
    else {
        //search a solution in positive sense
        valid = searchSolutionInPositiveSense(p_1new, RP, dt1Max);

        //The funtion searchSolutionInPositiveSense not change the status of the RP.

        //if has found a solution
        if(valid)
            //indicates that the RP is recoverable
            RP_is_recoverable = true;
        else //if not has found asolution
            //indicates that the RP is unrecoverable
            RP_is_recoverable = false;
    }

    //return the result of the search
    return RP_is_recoverable;
}

//Determines the RPs which can be recovered in each subset of each set.
//Inputs:
//  DDS: structure to contain disjoint disperse subsets.
//Outputs:
//  RecoverablesDDS: structure to contain the RPs which can be recovered.
//  UnrecoverablesDDS: structure to contain the RPs which can not be recovered.
//Preconditions:
//  All RPs of the Fiber MOS Model:
//      shall be configurated for MP generation.
//  All RPs of DDS:
//      shall be in the Fiber MOS  Model;
//      shall be in unsecurity position;
//      shall be enabled the quantifiers of their rotors;
//      shall not have dynamic collisions;
//      shall have stacked the actual positions of their rotors.
//Postconditions:
//  RecoverablesDDS will contains the RPs which can be recovered,
//  programmedwith the necessary instruction for their recovery.
//  All RPs of the RecoverablesDDS, will be in their initial  positions.
//  UnrecoverablesDDS will contains the RPs which can not be recovered.
//Notes:
//  It is not necessary that the RPs of DDS have stored their initial
//  positions.
//  It is not necessary that the RPs of DDS have disabled the quantifiers
//  of their rotors.
//  It is not necessary that the RPs of DDS have programmed any movement.
void TMotionProgramGenerator::segregateRecoverables(
        TPointersList<TPointersList<TRoboticPositionerList> >& RecoverablesDDS,
        TPointersList<TPointersList<TRoboticPositionerList> >& UnrecoverablesDDS,
        TPointersList<TPointersList<TRoboticPositionerList> >& DDS)
{
    //CHECK THE PRECONDITIONS:

    for(int i=0; i<getFiberMOSModel()->RPL.getCount(); i++) {
        TRoboticPositioner *RP = getFiberMOSModel()->RPL[i];
        if(RP->getActuator()->getPurpose()!=pGenPairPPDP && RP->getActuator()->getPurpose()!=pGenParPro)
            throw EImproperCall("all RPs of the Fiber MOS Model should be configurated for MP generation");
    }

    //All RPs of DDS:
    //  shall be in the Fiber MOS  Model;
    //  shall be in unsecurity position;
    //  shall be enabled the quantifiers of their rotors.
    //  shall not have dynamic collisions.
    //  shall have stacked the actual positions of their rotors.
    for(int i=0; i<DDS.getCount(); i++) {
        TPointersList<TRoboticPositionerList> *DisjointSet = DDS.GetPointer(i);
        for(int j=0; j<DisjointSet->getCount(); j++) {
            TRoboticPositionerList *DisperseSubset = DisjointSet->GetPointer(j);
            for(int k=0; k<DisperseSubset->getCount(); k++) {
                TRoboticPositioner *RP = DisperseSubset->Get(k);
                int l = getFiberMOSModel()->RPL.Search(RP);
                if(l >= getFiberMOSModel()->RPL.getCount())
                    throw EImproperArgument("all RPs of DDS should be in the Fiber MOS Model");
                if(RP->getActuator()->ArmIsInSafeArea())
                    throw EImproperArgument("all RPs of DDS should be in unsecurity position");
                if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
                    throw EImproperArgument("all RPs of DDS should be enabled the quantifiers of their rotors");
                if(RP->getActuator()->thereIsCollisionWithAdjacent())
                    throw EImproperArgument("all RPs of DDS should not have dynamic collisions");
                if(RP->getActuator()->thetasNotCoincideWithStacked())
                    throw EImproperArgument("all RPs of DDS should have stacked the actual positions of their rotors");
            }
        }
    }

    //INITIALIZE THE OUTPUTS VARIABLES:

    //build RecoverablesDDS similar to DDS but empty
    RecoverablesDDS.Clear();
    //for each disjoint set
    for(int i=0; i<DDS.getCount(); i++) {
        //point the indicated disjoint set to facilitate its access
        TPointersList<TRoboticPositionerList> *DisjointSet = DDS.GetPointer(i);

        //built the list DisjointSet_ with so many disperse subsets how has the DisjointSet
        TPointersList<TRoboticPositionerList> *DisjointSet_ = new TPointersList<TRoboticPositionerList>(max(1, DisjointSet->getCount()));
        for(int j=0; j<DisjointSet->getCount(); j++) {
            TRoboticPositionerList *DisperseSubset_ = new TRoboticPositionerList();
            DisjointSet_->Add(DisperseSubset_);
        }

        //add the DisjointSet_ to RecoverablesDDS
        RecoverablesDDS.Add(DisjointSet_);
    }

    //clone RecoverablesDDS with UnrecoverablesDDS
    UnrecoverablesDDS.Clone(RecoverablesDDS);

    //WARNING: do not clone the DDS, because TRoboticPositionerList::Clone
    //will clone the RPs, while TRoboticPositionerList::operator= will copy
    //the pointers to the RPs. Then for DDS use the method Copy.

    //Here all RPs of RecoverablesDDS accomplish the preconditions:
    //  are in the Fiber MOS  Model.
    //  are in unsecurity position.
    //  have enabled the quantifiers of their rotors.
    //  are free of dynamic collisions.

    //FOR EACH ISOLATED RP, VALIDATE THEIR INDIVIUDAL MP FOR RECOVERY IT,
    //AND IF A COLLISION IS DETECTED, MODIFY THE INDIVIDUAL MP AND
    //REITERATES THE PROCESS:

    for(int i=0; i<DDS.getCount(); i++) {
        TPointersList<TRoboticPositionerList> *DisjointSet = DDS.GetPointer(i);
        for(int j=0; j<DisjointSet->getCount(); j++) {
            TRoboticPositionerList *DisperseSubset = DisjointSet->GetPointer(j);
            for(int k=0; k<DisperseSubset->getCount(); k++) {
                TRoboticPositioner *RP = DisperseSubset->Get(k);

                //determines if there is solution with the minimun Dsec
                RP->setDsec(0);
                bool RP_is_recoverable = searchSolution(RP);

                //if there is solution, search the best solution
                if(RP_is_recoverable) {
                    //determines if there is solution with the maximun Dsec
                    RP->setDsec(RP->getDsecMax());
                    RP_is_recoverable = searchSolution(RP);

                    //if with the maximun Dsec there is'nt solution, perform a binary search
                    if(!RP_is_recoverable) {
                        //initialize the searching interval
                        double DsecMin = 0;
                        double DsecMax = RP->getDsecMax();

                        //variables for jumping point
                        double Dsec = DsecMax;
                        double PrevDsec;

                        do {
                            //save the last value of the jumping point
                            PrevDsec = Dsec;

                            //calculates the new jumping point
                            Dsec = (DsecMax + DsecMin)/2;

                            //set the jumping point in the RP
                            RP->setDsec(Dsec);

                            //search a solution
                            RP_is_recoverable = searchSolution(RP);

                            //actualize the searching interval
                            if(RP_is_recoverable)
                                DsecMin = Dsec;
                            else
                                DsecMax = Dsec;

                        } while(Dsec != PrevDsec);

                        //set the solution and regenerate the motion program
                        RP->setDsec(DsecMin);
                        RP_is_recoverable = searchSolution(RP);
                    }
                }

                //-----------------------------------------------------------
                //REACT ACCORDING THE RESULT OF THE SEARCH:

                if(RP_is_recoverable) {
                    //add the RP to the indicated disperse subset of RecoverablesDDS
                    RecoverablesDDS.GetPointer(i)->GetPointer(j)->Add(RP);
                } else { //if there isn't solution
                    //add the RP to the indicated disperse subset of UnrecoverablesDDS
                    UnrecoverablesDDS.GetPointer(i)->GetPointer(j)->Add(RP);
                }
            }
        }
    }

    //erase the empty disperse subsets in RecoverablesDDS
    int i = 0;
    while(i < RecoverablesDDS.getCount()) {
        //point the indicated disjoint set to facilitate its access
        TPointersList<TRoboticPositionerList> *DisjointSet = RecoverablesDDS.GetPointer(i);

        //for each disperse subset
        int j = 0;
        while(j < DisjointSet->getCount()) {
            //point the indicated disperse subset to facilitate its access
            TRoboticPositionerList *DisperseSubset = DisjointSet->GetPointer(j);

            //delete the disperse subset if empty, and indicates the next
            if(DisperseSubset->getCount() > 0)
                j++;
            else
                DisjointSet->Delete(j);
        }

        //delete the disjoint set if empty, and indicates the next
        if(DisjointSet->getCount() > 0)
            i++;
        else
            RecoverablesDDS.Delete(i);
    }

    //erase the empty disperse subsets from UnrecoverablesDDS
    i = 0;
    while(i < UnrecoverablesDDS.getCount()) {
        //point the indicated disjoint set to facilitate its access
        TPointersList<TRoboticPositionerList> *DisjointSet = UnrecoverablesDDS.GetPointer(i);

        //for each disperse subset
        int j = 0;
        while(j < DisjointSet->getCount()) {
            //point the indicated disperse subset to facilitate its access
            TRoboticPositionerList *DisperseSubset = DisjointSet->GetPointer(j);

            //delete the disperse subset if empty, and indicates the next
            if(DisperseSubset->getCount() > 0)
                j++;
            else
                DisjointSet->Delete(j);
        }

        //delete the disjoint set if empty, and indicates the next
        if(DisjointSet->getCount() > 0)
            i++;
        else
            UnrecoverablesDDS.Delete(i);
    }
}

//---------------------------------------------------------------------------
//METHODS FOR ADD MESSAGE LISTS TO A MP:

//Add to a MP, the corresponding list or lists of message instruction
//correspondint to the individual MPs of the RPs of a RP list.
//Inputs:
//  RPsToBeRetracted: list of the RPs which has been programmed.
//  MP: motion program to add themessage list.
//Outputs:
//  MP: motion program modified.
//Preconditions:
//  All RPs of the list RPsToBeRetracted shall have stacked the starting
//      positions of their rotors.
//  All RPs of the list RPsToBeRetracted shall be in their stacked positions.
//  All RPs of the list RPsToBeRetracted shall have a recovery program.
void TMotionProgramGenerator::addMessageLists(TMotionProgram& MP,
                                              const TRoboticPositionerList& RPsToBeRecovered)
{
    //CHECK THE PRECONDITIONS:

    for(int i=0; i<RPsToBeRecovered.getCount(); i++) {
        TRoboticPositioner *RP = RPsToBeRecovered[i];
        if(RP->getActuator()->theta_1s.getCount()<=0 || RP->getActuator()->getArm()->theta___3s.getCount()<=0)
            throw EImproperArgument("all RPs of the list RPsToBeRetracted shall have stacked the starting positions of their rotors");
        //        if(RP->getActuator()->thetasNotCoincideWithStacked())
        //          throw EImproperArgument("all RPs of the list RPsToBeRetracted shall be in their stacked positions");
        if(RP->MPturn.getCount()<=0 && RP->MPretraction.getCount()<=0)
            throw EImproperArgument("all RPs of the list RPsToBeRetracted shall have a recovery program");
    }

    TMessageList *MLjoin= NULL;

    do {
        //build a new MLjoin for contains the next MIs
        MLjoin = new TMessageList();

        //for each RP to be recovered
        for(int i=0; i<RPsToBeRecovered.getCount(); i++) {
            TRoboticPositioner *RP = RPsToBeRecovered[i];

            //appoint ML to the next ML of the individual MP
            TMotionProgram *MPind;
            TMessageList *ML;
            if(RP->MPturn.getCount() > 0) {
                MPind = &(RP->MPturn);
                ML = RP->MPturn.getFirstPointer();
            } else if(RP->MPretraction.getCount() > 0) {
                MPind = &(RP->MPretraction);
                ML = RP->MPretraction.getFirstPointer();
            } else {
                MPind = NULL;
                ML = NULL;
            }

            //if the individual MP contains some ML
            if(MPind!=NULL && ML!=NULL) {
                //translate all MIs from the ML to MLjoin
                while(ML->getCount() > 0) {
                    TMessageInstruction *MI = ML->getFirstPointer();
                    MLjoin->Add(MI);
                    ML->DeleteWithoutDestroy(0);
                }

                //Note that the usual is that MLjoin have a ML for each MI.

                //delete the empty ML of the individual MP
                if(MPind->getCount()>0 && ML->getCount()<=0)
                    MPind->Delete(0);
            }
        }

        //add the MLjoin to the MP or destroy it
        if(MLjoin->getCount() > 0)
            MP.Add(MLjoin);
        else {
            delete MLjoin;
            MLjoin = NULL;
        }

        //while has found instructions in the RPs to be recovered
    } while(MLjoin != NULL);
}

//Add to the DP the message-instruction list to move the RPs
//of the list Inners to the origins.
//Inputs:
//  Inners: list of operative RPs in seciry position out the origin.
//Output:
//  DP: depositioning program which the message list will be added.
//Preconditions:
//  All RPs of the list Inners shall be operatives in secure position
//  but out the origin.
void TMotionProgramGenerator::addMessageListToGoToTheOrigins(TMotionProgram& DP,
                                                             const TRoboticPositionerList& Inners)
{
    //check the preconditions
    for(int i=0; i<Inners.getCount(); i++) {
        TRoboticPositioner *RP = Inners[i];

        if(RP == NULL)
            throw EImproperArgument("list Inners should contains pointers to built RPs only");
        if(!RP->getOperative())
            throw EImproperArgument("all RPs in the list Inners shall be operative");
        if(RP->getActuator()->ArmIsOutSafeArea())
            throw EImproperArgument("all RPs in the list Inners shall be in security positions");
        if(!RP->getActuator()->isOutTheOrigin())
            throw EImproperArgument("all RPs in the list Inners shall be out the origins");
    }

    //WARNING: shall be taken into account that could hassome RP in the prigin.

    //get a message list with message instruction to go to the origin
    //for each RP of the list Inners
    TMessageList *ML = new TMessageList();
    for(int i=0; i<Inners.getCount(); i++) {
        TRoboticPositioner *RP = Inners[i];

        TMessageInstruction *MI = new TMessageInstruction();
        MI->setId(RP->getActuator()->getId());
        RP->getInstructionToGoToTheOrigin(MI->Instruction);
        ML->Add(MI);
    }

    //add the message list to the DP, if it isn't empty
    if(ML->getCount() > 0)
        DP.Add(ML);
    else
        delete ML;
}

//---------------------------------------------------------------------------
//SETTING PARAMETERS OF THE ALGORITHMS:

//maximun displacement of the rotor 1 for search the solution
//must be nonnegative
//default value: M_PI/2 rad
void TMotionProgramGenerator::setdt1Max(double t_dt1Max)
{
    if(t_dt1Max < 0)
        throw EImproperArgument("maximun displacement of the rotor 1 for search thesolution (dt1Max) should be nonnegative");

    dt1Max = t_dt1Max;
}

//---------------------------------------------------------------------------
//BUILDING AND DESTROYING METHODS:

//build a motion program generator
//attached to a Fiber MOS Model
TMotionProgramGenerator::TMotionProgramGenerator(TFiberMOSModel *t_FiberMOSModel) :
    TMotionProgramValidator(t_FiberMOSModel),
    TAllocationList(&(t_FiberMOSModel->RPL)),
    dt1Max(M_PI/2),
    NRmin(3), NBmin(1), PrMax(0)
{
}
//destroy the targetpoints and destroy the MPG
TMotionProgramGenerator::~TMotionProgramGenerator(void)
{
}

//---------------------------------------------------------------------------
//METHODS FOR GENERATE MPs:

//Generates a recovery program for a given set of operative RPs
//in insecurity positions and determines the RPs of the given set,
//which can not be recovered because are in colliding status
//or because are obstructed in insecurity positions.
//Inputs:
//  Outsiders: list of operative RPs in unsecurity positions which
//      we want recover the security positions.
//Outputs:
//  Collided: list of RPs collided in insecurity position.
//  Obstructed: list of RPs obstructed in insecurity position.
//  RecoveryProgram: the generated recovery program.
//Preconditions:
//  All RPs of the Fiber MOS Model:
//    - shall be in their starting positions.
//    - shall be configurated for MP generation.
//      (Purpose == pGenPairPPDP || Purpose == pGenParPro).
//  All RPs of the list Outsiders:
//    - shall be in the Fiber MOS Model.
//    - shall be operatives.
//    - shall be in insecurity positions.
//    - shall have enabled the quantifiers of their rotors.
//Postconditions:
//  All RPs of the Fiber MOS Model:
//    - will be in the starting positions.
//    - will have enabled the quantifiers.
void TMotionProgramGenerator::generateRecoveryProgram(
        TRoboticPositionerList& Collided, TRoboticPositionerList& Obstructed,
        TMotionProgram& RecoveryProgram, const TRoboticPositionerList& Outsiders)
{
    //CHECK THE PRECONDITIONS:

    for(int i=0; i<getFiberMOSModel()->RPL.getCount(); i++) {
        TRoboticPositioner *RP = getFiberMOSModel()->RPL[i];
        if(RP->getActuator()->getPurpose()!=pGenPairPPDP && RP->getActuator()->getPurpose()!=pGenParPro)
            throw EImproperCall("all RPs of the Fiber MOS Model shall be configurated for MP generation");
    }

    for(int i=0; i<Outsiders.getCount(); i++) {
        TRoboticPositioner *RP = Outsiders[i];
        int j = getFiberMOSModel()->RPL.Search(RP);
        if(j >= getFiberMOSModel()->RPL.getCount())
            throw EImproperArgument("all RPs of the list Outsiders shall be in the Fiber MOS Model");
        if(!RP->getOperative())
            throw EImproperArgument("all RPs in the list Outsiders shall be operative");
        if(RP->getActuator()->ArmIsInSafeArea())
            throw EImproperArgument("all RPs in the list Outsiders shall be in insecurity positions");
        if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
            throw EImproperArgument("all RPs in the list Outsiders shall have enabled the quantifiers of their rotors");
    }

    //MAKE ACTIONS:

    //initialize the outputs
    Collided.Clear();
    Obstructed.Clear();
    RecoveryProgram.Clear();

    //build the list Outsiders_ to contains the pointers to
    //the RPs wich remain in insecurity positions
    TRoboticPositionerList Outsiders_;

    //Here the Fiber MOS Model is configured for generate a pair (PP, DP) or generate a parking program.

    //segregates the RPs of the list Outsiders which are in collision status, in the list Collided
    Outsiders.segregateCollided(Collided, Outsiders_);

    //The following check is unnecesary, because in the preconditions is checked that
    //all RPs of the list Outsiders are in insecurity positions:
    //  //solve the trivial case when there aren't RPs of the list Outsiders in insecurity positions
    //  if(Outsiders.allRPsAreInSecurePosition())
    //      //return the empty solution
    //      return;

    //Here all RPs are in their starting positions.

    //stacks the starting positions of all RPs
    getFiberMOSModel()->RPL.pushPositions();

    //Since the starting position must be recovered repeteadly,
    //the starting positions must be stored.

    //Note that here it is not necessary disable the quantifier, because
    //the quantifiers will be disabled only in two places:
    //  1. During segregation of recoverables, for simulate the movement
    //     of each RP from their starting position to their final position.
    //  2. During validation of the generated recovery program, for simmulate
    //     the jointly movement of the RPs which must be moved together.
    //If the recovery program not pass the validation process, the quantifiers
    //remains disabled, and their original status stacked.

    //=======================================================================
    //ITERATES THE GENERATION OF RECOVERY STEPS, USING A COPY OF Outsiders,
    //(Outsiders_), WHILE ARE ACCOMPLISH THE CONDITIONS:
    //  1. In the list Outsiders_ remains RPs to be recovered.
    //  2. Has been recovered some RP in the last iteration.

    //The list Outsiders_ will contains all RPs to be retracted each time.
    //Each time can be only retracted some RPs of the list Outsiders_.
    //The process shall be reiterated until deplete the list.
    //The list Outsiders_ will start being a copy of the list Outsiders,
    //which will contains the set of all RPs in insecurity positions,
    //which we want recover the security position of most of them.
    //Only the obstructed RPs will remain in the list Outsiders_.

    //The proces of recovery consist in the iteration of recovery steps.
    //Each recovery step can be composed by one, two or three steps:
    //1. Turn of rotor 1 of the RPs which can't be directly retracted.
    //2. Retraction of the RPs.
    //3. Abatement of the arms (or turn of the rotor 2) of the RPs whose
    //   rotor 1 is in the origin.

    //define the conditions to finish the iterative process
    bool condition1; //indicates if there is some RPs to berecovered in the list Outsiders_
    bool condition2; //indicates if has recovered some RP in the last iteration

    do {
        //segregates the RPs to recover, in disjoint sets
        TPointersList<TRoboticPositionerList> DisjointSets;
        segregateRPsInDisjointSets(DisjointSets, Outsiders_);

        //WARNING: if recicle the DDS, would need to make here:
        //  DDS.Clear();

        //segregates the RPs of each disjoint set, in disperse subsets
        TPointersList<TPointersList<TRoboticPositionerList> > DDS;
        for(int i=0;i<DisjointSets.getCount(); i++) {
            TPointersList<TRoboticPositionerList> *DisperseSubsets = new TPointersList<TRoboticPositionerList>();
            segregateRPsInDisperseSubsets(DisperseSubsets, DisjointSets[i]);
            DDS.Add(DisperseSubsets);
        }

        //segregates the RPs which can be recovered in each subset of each set
        TPointersList<TPointersList<TRoboticPositionerList> > RecoverablesDDS;
        TPointersList<TPointersList<TRoboticPositionerList> > UnrecoverablesDDS;
        segregateRecoverables(RecoverablesDDS, UnrecoverablesDDS, DDS);

        //Here all RPs of RecoverablesDDS will be in their starting positions,
        //and we want retract the RPs in the larger disperse subsets.

        //-------------------------------------------------------------------
        //SEGREGATES THE RPs TO BE RECOVERED:

        //WARNING: if recicle the RPsToBeRecovered, would need to make here:
        //  RPsToBeRecovered.Clear();

        //build the list of RPs to be recovered in each step
        TRoboticPositionerList RPsToBeRecovered;

        //for each disjoint set of retractiles RPs
        for(int i=0; i<RecoverablesDDS.getCount(); i++) {
            //point the disjoint set indicated to facilitate its access
            TPointersList<TRoboticPositionerList> *DisjointSet = RecoverablesDDS.GetPointer(i);

            //search the disperse subset which has:
            //  1. the maximun Dsec;
            //  2. the maximun number of RPs.
            TRoboticPositionerList *DisperseSubsetMax = NULL;
            for(int j=0; j<DisjointSet->getCount(); j++) {
                TRoboticPositionerList *DisperseSubset = DisjointSet->GetPointer(j);

                //actualize the maximun disperse subset
                if(DisperseSubset->getCount()>0 &&
                        (DisperseSubsetMax==NULL ||
                         (DisperseSubset->getDsecMin() > DisperseSubsetMax->getDsecMin()) ||
                         ((DisperseSubset->getDsecMin() == DisperseSubsetMax->getDsecMin()) && (DisperseSubset->getCount() > DisperseSubsetMax->getCount())) ))
                    DisperseSubsetMax = DisperseSubset;
            }

            //add the RPs of the subset to the list RPsToBeRecovered
            if(DisperseSubsetMax != NULL)
                for(int j=0; j<DisperseSubsetMax->getCount(); j++) {
                    TRoboticPositioner *RP = DisperseSubsetMax->Get(j);
                    RPsToBeRecovered.Add(RP);
                }
        }
        //sort the list RPsToBeRecovered
        RPsToBeRecovered.Compare = TRoboticPositioner::compareIds;
        RPsToBeRecovered.SortInc();

        //Sort the RPs isn't really necessary, but is recomendable because produce a more legible output.

        //-------------------------------------------------------------------
        //GET THE MESSAGE-INSTRUCTION LIST CORRESPONDING TO THE RPsToBeRecovered.
        //MOVE THE RPs TO BE RECOVEREDTOTHEIRFINAL  POSITIONS.
        //DELETEDTHE RECOVERED RPs FROM THE LIST Outsiders_:

        //if there is some RP to be retracted
        if(RPsToBeRecovered.getCount() > 0) {
            //move the RPs to be retracted to their programmed final positions
            RPsToBeRecovered.moveFinMP();

            //WARNING: RPs to be retracted shall be in their final positions
            //for avoid inter ferences with the next iteration and to get the
            //message-instruction list to turn the rotors 1.

            //trasfer the MIs from the RPs to be recovered to the recovery program
            addMessageLists(RecoveryProgram, RPsToBeRecovered);

            //delete the retracted RPs from the list Outsiders_
            for(int i=0; i<RPsToBeRecovered.getCount(); i++) {
                TRoboticPositioner *RP = RPsToBeRecovered[i];
                int j = Outsiders_.Search(RP);
                //perform a rutinary checking
                if(j >= Outsiders_.getCount())
                    throw EImpossibleError("previously extracted RP  fron the list Outsiders_ not found in this list");
                Outsiders_.Delete(j);
            }
        }

        //-------------------------------------------------------------------
        //DETERMINES IF IS ACCOMPLISHED THE REITERATION CONDITIONS:

        //determines if there is some RPs to be recovered in the list Outsiders_
        condition1 = (Outsiders_.getCount() > 0);
        //determines if has recovered some RP in the last iteration
        condition2 = (RPsToBeRecovered.getCount() > 0);

        //while both conditions are met
    } while(condition1 && condition2);

    //=======================================================================

    //indicates in the obstructed list, the operative RPs which remain in
    //unsequrity positions and are in the Outsiders list
    Obstructed = Outsiders_;

    //restore and discard the starting positions
    getFiberMOSModel()->RPL.restoreAndPopPositions();
}

//Generates a depositioning program for a given set of operative RPs
//in insecurity positions and determines the RPs of the given set,
//which can not be recovered because are in colliding status
//or because are obstructed in insecurity positions.
//Inputs:
//  Outsiders: list of operative RPs in unsecurity positions which
//      we want recover the origin positions.
//Outputs:
//  generateDepositioningProgram: indicates if the generated DP is valid.
//  Collided: list of RPs collided in insecurity position.
//  Obstructed: list of RPs obstructed in insecurity position.
//  DP: the generated depositioning program.
//Preconditions:
//  All RPs of the Fiber MOS Model:
//    - shall be in their starting positions.
//  All RPs of the list Outsiders:
//    - shall be in the Fiber MOS Model.
//    - shall be operatives.
//    - shall be in insecurity positions.
//    - shall have enabled the quantifiers.
//    - shall have velocity of rotor 2 approximately double than rotor 1.
//Postconditions:
//  All RPs of the Fiber MOS Model:
//    - will be configured for validate a DP. (Purpose == pValDP).
//  When the generated depositioning program isn't valid:
//      All RPs of the FMM:
//        - will have disabled the quantifiers of their rotors.
//        - will be in the first position where the collision was detected
//              during the validation process.
//  When the generated depositioning program is valid (even the trivial case):
//      All RPs of the FMM:
//        - will have enabled the quantifiers of their rotors.
//        - will be in their final positions.
bool TMotionProgramGenerator::generateDepositioningProgram(
        TRoboticPositionerList& Collided, TRoboticPositionerList& Obstructed,
        TMotionProgram& DP, const TRoboticPositionerList& Outsiders)
{
    //check the precondition
    for(int i=0; i<Outsiders.getCount(); i++) {
        TRoboticPositioner *RP = Outsiders[i];
        if(Abs(RP->wmaxabs2()/RP->wmaxabs1() - 2) > 0.1)
            throw EImproperCall("all RPs in the list Outsiders shall have velocity of rotor 2 approximately double than rotor 1");
    }

    //configure the Fiber MOS Model for generate a DP
    getFiberMOSModel()->RPL.setPurpose(pGenPairPPDP);

    //generate the recovery program
    generateRecoveryProgram(Collided, Obstructed, DP, Outsiders);

    //The function generateRecoveryProgram first store and after restore and pop
    //the positions of all RPs of the FMM.

    //configure the Fiber MOS Model for validate the DP
    getFiberMOSModel()->RPL.setPurpose(pValDP);

    //determine if the generated DP is valid
    bool DPvalid = validateMotionProgram(DP);

    //WARNING: here all RPs retracted must be in security positions,
    //to allow add a message list to go to the origins.

    //Here all RPs included in the DP, are in security position, out the origin.

    //if the DP is valid
    if(DPvalid) {
        //Here all operative outsiders RPs which aren't obstructed are in secure position,
        //in their final position after execute the recovery program.

        //segregate the operative inners RPs out of the origin and sort it
        TRoboticPositionerList Inners;
        getFiberMOSModel()->RPL.segregateOperativeInnersOutTheOrigins(Inners);
        Inners.SortInc();

        //Sort the RPs isn't really necessary, but is recomendable because produce a more legible output.

        //generate the parking gesture for the operative RPs in security position out the origin
        addMessageListToGoToTheOrigins(DP, Inners);

        //move the segregated RPs to the origin
        Inners.moveToOrigins();

        //Here all RPs included in the DP, are in the origin.
    }

    return DPvalid;
}

//Generates a positioning program from a given depositioning program.
//Inputs:
//  DP: depositioning program.
//  OPL: observing position list.
//Outputs:
//  PP: the generated positioning program.
//Preconditions:
//  All RPs included in the OPL shall be in the FMM.
//  All RPs included in the DP shall be in the FMM.
void TMotionProgramGenerator::generatePositioningProgram(TMotionProgram& PP,
                                                         const TMotionProgram& DP, const TPairPositionAnglesList& OPL)
{
    //CHECK THE PRECONDITIONS:

    for(int i=0; i<OPL.getCount(); i++) {
        int id = OPL[i].getId();
        int j = getFiberMOSModel()->RPL.searchId(id);
        if(j >= getFiberMOSModel()->RPL.getCount())
            throw EImproperArgument("all RPs included in the OPL shall be in the FMM");
    }

    for(int i=0; i<DP.getCount(); i++) {
        const TMessageList *ML = DP.GetPointer(i);
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);
            int k= getFiberMOSModel()->RPL.searchId(MI->getId());
            if(k >= getFiberMOSModel()->RPL.getCount())
                throw EImproperArgument("all RPs included in the DP shall be in the FMM");
        }
    }

    //MAKE ACTIONS:

    //initialize the output
    PP.Clear();

    //for each message list,move the RPs to the final position (of the gessture),
    //and then get and insert the corresponding message list for the PP:

    //initialize initial position list and final position list
    TPairPositionAnglesList IPL;
    TPairPositionAnglesList FPL(OPL);

    //for each message list of the DP
    for(int i=0; i<DP.getCount(); i++) {
        const TMessageList *ML = DP.GetPointer(i);

        //actualice initial positions
        IPL = FPL;

        //build the inverse message list for contains the inverse gesture
        TMessageList *IML = new TMessageList();

        //for each message instruction build and add the corresponding inverse message instruction
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);
            int Id = MI->getId();

            //search the initial and final positions corresponding to the message
            int k = FPL.searchId(Id);
            if(k >= FPL.getCount())
                throw EImpossibleError("lateral effect");
            TPairPositionAngles *PPAini = IPL.GetPointer(k);
            TPairPositionAngles *PPAfin = FPL.GetPointer(k);

            //actualice the final position
            if(MI->Instruction.getName() == "MM") {
                PPAfin->p_1 = MI->Instruction.Args[0];
                PPAfin->p___3 = MI->Instruction.Args[1];
            } else if(MI->Instruction.getName() == "M1")
                PPAfin->p_1 = MI->Instruction.Args[0];
            else if(MI->Instruction.getName() == "M2")
                PPAfin->p___3 = MI->Instruction.Args[0];

            //set the corresponding initial position as final position in  the inverse message instruction
            TMessageInstruction *IMI = new TMessageInstruction();
            IMI->setId(Id);
            IMI->Instruction.setName(MI->Instruction.getName());
            if(IMI->Instruction.getName() == "MM") {
                IMI->Instruction.Args.setCount(2);
                IMI->Instruction.Args[0] = PPAini->p_1;
                IMI->Instruction.Args[1] = PPAini->p___3;
            } else if(MI->Instruction.getName() == "M1") {
                IMI->Instruction.Args.setCount(1);
                IMI->Instruction.Args[0] = PPAini->p_1;
            } else if(MI->Instruction.getName() == "M2") {
                IMI->Instruction.Args.setCount(1);
                IMI->Instruction.Args[0] = PPAini->p___3;
            }
            IMI->setCommentDsec(MI->getCommentDsec());

            //add the inverse message instruction to the inverse message list
            IML->Add(IMI);
        }

        //insert the invert message list in the PP
        PP.Insert(0, IML);
    }
}

//Generates a pair (PP, DP) for a given set of operative RPs
//in insecurity positions and determines the RPs of the given set,
//which can not be recovered because are in colliding status
//or because are obstructed in insecurity positions.
//Inputs:
//  Outsiders: list of operative RPs in unsecurity positions which
//      we want move.
//Outputs:
//  PPvalid: flag indicating if the generated PP is valid.
//  DPvalid: flag indicating if the generated DP is valid.
//  Collided: list of RPs collided in insecurity position.
//  Obstructed: list of RPs obstructed in insecurity position.
//  PP: the generated positioning program.
//  DP: the generated depositioning program.
//Preconditions:
//  All RPs of the Fiber MOS Model:
//    - shall be in their observing positions.
//  All RPs of the list Outsiders:
//    - shall be in the Fiber MOS Model.
//    - shall be operatives.
//    - shall be in insecurity positions.
//    - shall have enabled the quantifiers.
//    - shall have velocity of rotor 2 approximately double than rotor 1.
//Postconditions:
//  All RPs of the Fiber MOS Model:
//    - will be configured for validate a PP. (Purpose = pValPP).
//    - will have disabled the quantifiers.
//  When the generated pair (PP, DP) isn't valid:
//      All RPs of the FMM:
//        - will have disabled the quantifiers of their rotors;
//        - will be in the first position where the collision was detected
//              during the validation process.
//  When the generated pair (PP, DP) is valid (even the trivial case):
//      All RPs of the FMM:
//        - will have enabled the quantifiers of their rotors;
//        - will be in their initial positions.
void TMotionProgramGenerator::generatePairPPDP(bool& PPvalid, bool& DPvalid,
                                               TRoboticPositionerList& Collided, TRoboticPositionerList& Obstructed,
                                               TMotionProgram& PP, TMotionProgram& DP,
                                               const TRoboticPositionerList& Outsiders)
{
    //captures the observing positions of the RPs in a PPA list
    TPairPositionAnglesList OPL;
    getFiberMOSModel()->RPL.getPositions(OPL);

    //attempt to generate the depositioning program
    DPvalid = generateDepositioningProgram(Collided, Obstructed, DP, Outsiders);

    //Here the RPs of the list Outsiders are in the origin, less the RPs
    //in collision or obstructed, which will be in their observing positions.

    //if has obtained a valid DP, generates the corresponding PP
    if(DPvalid) {
        //captures the initial positions of the RPs in a PPA list
        TPairPositionAnglesList IPL;
        getFiberMOSModel()->RPL.getPositions(IPL);

        //generate the PP from the DP
        generatePositioningProgram(PP, DP, OPL);

        //configure the FiberMOs Model for PP validation
        getFiberMOSModel()->RPL.setPurpose(pValPP);

        //validate the PP
        getFiberMOSModel()->RPL.setPositions(IPL);
        PPvalid = validateMotionProgram(PP);

        //if the generated PP is valid
        if(PPvalid)
            //move the RPs to the initial positions
            getFiberMOSModel()->RPL.setPositions(IPL);

        //else stay in the position where collission was detected
    }
    else {
        PP.Clear();
        PPvalid = false;
    }
}

//Generates a parking program for a given set of operative RPs
//in insecurity positions and determines the RPs of the given set,
//which can not be recovered because are in colliding status
//or because are obstructed in insecurity positions.
//Inputs:
//  Outsiders: list of operative RPs in unsecurity positions which
//      we want recover the security positions.
//Outputs:
//  generateDepositioningProgram: indicates if the generated ParkProg is valid.
//  Collided: list of RPs collided in insecurity position.
//  Obstructed: list of RPs obstructed in insecurity position.
//  ParkProg: the generated parking program.
//Preconditions:
//  All RPs of the Fiber MOS Model:
//    - shall be in their starting positions.
//  All RPs of the list Outsiders:
//    - shall be in the Fiber MOS Model.
//    - shall be operatives.
//    - shall be in insecurity positions.
//    - shall have enabled the quantifiers.
//Postconditions:
//  All RPs of the FMM:
//    - will be configured for validate a parking program.
//  When the generated recovery program isn't valid:
//      All RPs of the FMM:
//        - will have disabled the quantifiers of their rotors;
//        - will be in the first position where the collision was detected
//              during the validation process.
//  When the generated recovery program is valid (even the trivial case):
//      All RPs of the FMM:
//        - will have enabled the quantifiers of their rotors;
//        - will be in their final positions.
bool TMotionProgramGenerator::generateParkProg(
        TRoboticPositionerList& Collided, TRoboticPositionerList& Obstructed,
        TMotionProgram& ParkProg, const TRoboticPositionerList& Outsiders)
{
    //configure the Fiber MOS Model for generate a parking program
    getFiberMOSModel()->RPL.setPurpose(pGenParPro);

    //generates the recovery program
    generateRecoveryProgram(Collided, Obstructed, ParkProg, Outsiders);

    //configure the Fiber MOS Model for validate the parking program
    getFiberMOSModel()->RPL.setPurpose(pValParPro);

    //determines if the generated parking program is valid
    bool valid = validateMotionProgram(ParkProg);

    //WARNING: here all RPs retracted must be in security positions,
    //to allow add a message list to go to the origins.

    //Here all RPs included in the DP, are in security position, out the origin.

    //if the parking program is valid
    if(valid) {
        //Here all operative outsiders RPs which aren't obstructed are in secure position,
        //in their final position after execute the recovery program.

        //segregate the operative inners RPs out of the origin and sorts it
        TRoboticPositionerList Inners;
        Outsiders.segregateOperativeInnersOutTheOrigins(Inners);
        Inners.SortInc();

        //Sort the RPs isn't really necessary, but is recomendable because produce a more legible output.

        //generates the parking gesture for the operative RPs in security position out the origin
        addMessageListToGoToTheOrigins(ParkProg, Inners);

        //move the segregated RPs to the origin
        Inners.moveToOrigins();

        //Here all RPs included in the DP, are in the origin.
    }

    return valid;
}

//---------------------------------------------------------------------------
//METHODS FOR REGENERATE MPs:

//search the replacement RPs of a RP
//Inputs:
//  RP: the RP to be replaced.
//Outputs:(
//  RRPs: list of replacement RPs.
//Preconditions of the FMM:
//  All pointer of the Fiber MOS Model must point to built RPs.
//  All pointer of the fiber MOS Model must point to different RPs.
//  None operative RP of the Fiber MOS Model must has a dynamic fault.
//Preconditions of the RP:
//  Pointer RP must point to built robotic positioner.
//  The RP must be in the Fiber MOS Model.
//Preconditions of the TPL:
//  All RPs included in the TPL must be in the Fiber MOS Model.
//  All PPs included in the TPL must be in the scope of their allocated RP.
//  The RP must be included in the TPL.
//Postconditions:
//  If the list RRPs is not empty, the RP is a simple case,
//  and can be replaced by any of the replacement RPs.
//  In other case the RP is not a simple case.
void TMotionProgramGenerator::searchReplacementRPs(TRoboticPositionerList& RRPs,
                                                   const TRoboticPositioner *RP) const
{
    //CHECK THE PRECONDITIONS OF THE FMM:

    if(getFiberMOSModel()->RPL.thereIsSomeNullPointer())
        throw EImproperCall("all pointers of the Fiber MOS Model should point to build RPs");

    if(getFiberMOSModel()->RPL.thereIsSomeRepeatedPointer())
        throw EImproperCall("all pointer of the fiber MOS Model should point to different RPs");

    if(getFiberMOSModel()->RPL.thereIsSomeOperativeRPwithDynamicFaul())
        throw EImproperCall("none operative RP of the Fiber MOS Model should has a dynamic fault");

    //CHECK THE PRECONDITIONS OF THE RP:

    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    int i = getFiberMOSModel()->RPL.search(RP);
    if(i >= getFiberMOSModel()->RPL.getCount())
        throw EImproperArgument("the RP should be in the Fiber MOS Model");

    //CHECK THE PRECONDITIONS OF THE TPL:

    for(int i=0; i<getCount(); i++) {
        TAllocation *TP = Items[i];

        TRoboticPositioner *RP = TP->getRP();
        int j = getFiberMOSModel()->RPL.Search(RP);
        if(j >= getFiberMOSModel()->RPL.getCount())
            throw EImproperArgument("all RPs included in the TPL must be in the Fiber MOS Model");

        TDoublePoint PP = TP->PP;
        if(RP->getActuator()->pointIsOutDomainP3(PP.x, PP.y))
            throw EImproperArgument("all PPs included in the TPL must be in the scope of their allocated RP");
    }

    i = searchAllocation(RP);
    if(i >= getCount())
        throw EImproperArgument("the RP must be included in the TPL");

    //MAKE ACTIONS:

    //initialize the output
    RRPs.Clear(); //replacement RPs

    //search the replacement RPs
    for(i=0; i<RP->getActuator()->AdjacentRPs.getCount(); i++) {
        TRoboticPositioner *RRP = RP->getActuator()->AdjacentRPs[i].RP;

        //Here the RRP is adjacent to the RP to be replaced.

        //determines if the PP allocated to the RP is in the scope of the RRP
        i = searchAllocation(RP);
        TAllocation *TP = Items[i];
        bool is_in_domain = RRP->getActuator()->pointIsInDomainP3(TP->PP);

        if(is_in_domain) {

            //Here the PP allocated to the RP, is in the scope of the RRP.

            if(RRP->getOperative()) {
                int j = searchAllocation(RRP);
                if(j >= getCount()) {

                    //Here the RRP is adjacent, operative and it is not allocated.

                    //check if the unsecurity area of the replacement RP is clear and will stay clear
                    bool clear = true;
                    int k = 0;
                    while(clear && k<RRP->getActuator()->AdjacentRPs.getCount()) {
                        TRoboticPositioner *RPA = RRP->getActuator()->AdjacentRPs[k].RP;

                        //if the RPA is operative
                        //must be in security area, and if it has allocated PP,
                        //this must be in the security area
                        if(RPA->getOperative()) {
                            if(RPA->getActuator()->ArmIsInSafeArea()) {
                                int l = searchAllocation(RPA);
                                if(l < getCount()) {
                                    TAllocation *TP = Items[l];
                                    TDoublePoint PP = TP->PP;
                                    if(RPA->getActuator()->pointIsInSecurityArea(PP))
                                        k++;
                                    else
                                        clear = false;
                                } else
                                    k++;

                            } else
                                clear = false;
                        }

                        //if the RPA is not operative
                        //must be non invading the maneuvering domain of the RRP
                        else {
                            if(RPA->getActuator()->notInvadeManeuveringDomain(RRP->getActuator()))
                                k++;
                            else
                                clear = false;
                        }

                    }

                    //if the unsecurity area is clear and stay clear,add the RRP to the list
                    if(clear)
                        RRPs.Add(RRP);
                }
            }
        }
    }
}

//The data (NRmin, NBmin, PrMax) are the same for all points in the CB.
//The data (noAllocated, allocateInAll) must be defined for each point.

//Determines if an allocation if of must type.
//Inputs:
//  i: index to the allocation for determine if it is must type.
//Outputs:
//  allocationIsMustType: indicates if the allocation is of must type.
//Preconditions:
//- Index i should indicate to an allocation of this MPG.
bool TMotionProgramGenerator::allocationIsMustType(int i) const
{
    //CHECK THE PRECONDITIONS:

    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i should indicate to an allocation of this MPG");

    //MAKE ACTIONS:

    //point the indicated allocation for facilitate its access
    TAllocation *A = Items[i];

    //check if the allocation corresponds to a reference source,
    //and the CB contains the minimun number of reference sources NRmin.
    unsigned int NR = countNR();
    if(A->PP.Type==ptREFERENCE && NR<=NRmin)
        return true;

    //check if the allocation corresponds to a blank,
    //and the CB contains the minimun number of blanks NBmin.
    unsigned int NB = countNB();
    if(A->PP.Type==ptBLANK && NB<=NBmin)
        return true;

    //check if if the allocation has a priority in [0, PrMax],
    //and it is not allocated in other CB.
    if(A->PP.Priority <= PrMax)// && TP->notAllocated)
        return true;

    //check if has been indicated that the point must be allocated in all CBs
    //if(TP->allocateInAll)
    //    return true;

    //indicates that the allocation is not must type
    return false;
}

//Attempt regenerate a pair (PP, DP).
//Inputs:
//  (PP, DP): the pair to regenerate.
//Outputs:
//  attemptRegenerate: flag indicating if the pair (PP, DP)
//      has been regenerated.
//  (PP, DP): the regenerated pair, if any.
//  Excluded: list of identifiers of the excluded RPs, if Any.
//Preconditions:
//  All RPs included in the pair (PP, DP):
//      must be in the FMM;
//      must have one allocation in the MPG.
//  The status of the Fiber MOS Model:
//      must correspond to the status of the real Fiber MOS.
//  The allocations shall contains the properties enough to
//      make the regeneration (Type, Pr, Pid, X, Y).
bool TMotionProgramGenerator::attemptRegenerate(TVector<int>& Excluded,
                                                TMotionProgram& PP, TMotionProgram& DP) const
{
    //determine the list of not operative RPs included in the pair (PP, DP)
    TRoboticPositionerList RPL;
    getRPsIncludedInMPs(RPL, PP, DP, getFiberMOSModel());
    int i = 0;
    while(i < RPL.getCount()) {
        TRoboticPositioner *RP = RPL[i];
        if(RP->getOperative())
            RPL.Delete(i);
        else
            i++;
    }

    //check the preconditions
    for(int i=0; i<RPL.getCount(); i++) {
        TRoboticPositioner *RP = RPL[i];

        int j = getFiberMOSModel()->RPL.Search(RP);
        if(j >= getFiberMOSModel()->RPL.getCount())
            throw EImproperArgument("all RPs included in the pair (PP,DP) should be be in the Fiber MOS Model");

        j = searchAllocation(RP);
        if(j >= getCount())
            throw EImproperArgument("all RPs included in the pair (PP,DP) should have an allocation in the MPG");
    }

    //initialize the output
    Excluded.Clear();

    //determines if there is some point of type must allocated to a not operative RP
    for(int i=0; i<RPL.getCount(); i++) {
        TRoboticPositioner *RP = RPL[i];

        //if the allocation if type must
        //indicates that the pair (PP, DP) can not be regenerated
        int j = searchAllocation(RP);
        if(j >= getCount())
            throw EImpossibleError("lateral effect");
        bool allocation_is_must = allocationIsMustType(j);
        if(allocation_is_must)
            return false; //indicates that the pair (PP, DP) can not be regenerated

        //exclude the RP from the pair (PP,DP)
        PP.excludeRP(RP->getActuator()->getId());
        DP.excludeRP(RP->getActuator()->getId());

        //add the identifier of the RP excluded to the list
        Excluded.Add(RP->getActuator()->getId());
    }

    return true;

    //Other more efficient way of regenerate a pair (PP, PD):
    //  //search the list of replacement RPs for each RP
    //  TPointersList<TRoboticPositionerList> RRPss;
    //  for(int i=0; i<RPL.getCount(); i++) {
    //      TRoboticPositioner *RP = RPL[i];
    //
    //      TRoboticPositionerList *RRPs = new TRoboticPositionerList();
    //      searchReplacementRPs(RRPs, RP);
    //      if(RRPs.getCount() <= 0)
    //          return false;
    //      RRPss.Add(RRPs);
    //  }
    //  //attemp solve all not operative RPs included in the pair (PP, DP)
    //  for(int i=0; i<RPL.getCount(); i++) {
    //      TRoboticPositioner *RP = RPL[i];
    //
    //      //If it is a simple case:
    //          //allocate the point to the adjacent RP (more convenient) satisfing the requirements.
    //      //If it is not a simple case:
    //          //if the allocated point is type must:
    //              //indicates that the pair (PP, DP) can not be regenerated
    //
    //      //If the actual position is secure:
    //          //delete the allocation of the RP.
    //      //Ifthe actual position is unsecure:
    //          //delete the allocation of the RP and all adjacents RPs with point allocated in unsecurity position.
    //  }
    //  return false;
}

/*****************************************************************************
//FUNCTIONS TO BE USED ONLINE BY MCS (2025):

//Generates a parking program.
//Inputs:
//  FMM: the Fiber MOS Model.
//  p_1s: the rotor 1 starting positions of all RPs of the FMM.
//  p___3s: the rotor 2 starting positions of all RPs of the FMM.
//  Ids: the identifiers of the RPs of the FMM to be disabled.
//Outputs:
//  generateParkProg_online: indicates if the generated parking program is valid.
//  ParkProg: the parking program to be generated.
//Preconditions:
//  All operative outsider RPs of the FMM:
//    - shall be in the Fiber MOS Model;
//    - shall be operatives;
//    - shall be in insecurity positions;
//    - shall have enabled the quantifiers;
//  All position angles in the vector p_1s:
//    - must be in the rotor 1 domain of their corresponding RP.
//  All position angles in the vector p___3s:
//    - must be in the rotor 2 domain of their corresponding RP.
//  All identifiers in the vector Ids:
//    - must be referred to RPs in the FMM.
//Postconditions:
//  The RPs indicated in the list Ids will be disabled.
//  All RPs of the Fiber MOS Model will be configured for
//    - validate a parking program. (Purpose = valParPro).
//  When the generated recovery program isn't valid:
//      All RPs of the FMM:
//        - will have disabled the quantifiers of their rotors;
//        - will be in the first position where the collision was detected
//              during the validation process.
//  When the generated recovery program is valid (even in the trivial case):
//      All RPs of the FMM:
//        - will have enabled the quantifiers of their rotors;
//        - will be in their final positions.
bool generateParkProg_online(TMotionProgram& ParkProg,
                             TFiberMOSModel& FMM,
                             const vector<double>& p_1s, const vector<double>& p___3s,
                             const vector<int>& Ids)
{
    //CHECK THE PRECONDITIONS:

    if(p_1s.size() != (unsigned int)FMM.RPL.getCount())
        throw EImproperArgument("vector p_1s should has one position for each RP of the FMM");
    if(p___3s.size() != (unsigned int)FMM.RPL.getCount())
        throw EImproperArgument("vector p___3s should has one position for each RP of the FMM");

    for(unsigned int i=0; i<p_1s.size(); i++) {
        double p_1 = p_1s[i];
        double p___3 = p___3s[i];
        TRoboticPositioner *RP = FMM.RPL[i];
        if(RP->getActuator()->isntInDomainp_1(p_1))
            throw EImproperArgument("position angle "+floattostr(p_1)+" isnt in the rotor 1 domain of the RP"+inttostr(RP->getActuator()->getId()));
        if(RP->getActuator()->getArm()->isntInDomainp___3(p___3))
            throw EImproperArgument("position angle "+floattostr(p___3)+" isnt in the rotor 2 domain of the RP"+inttostr(RP->getActuator()->getId()));
    }

    for(unsigned int i=0; i<Ids.size(); i++) {
        int Id = Ids[i];
        int j = FMM.RPL.searchId(Id);
        if(j >= FMM.RPL.getCount())
            throw EImproperArgument("vector Ids should contains identifiers of RPs of the FMM");
    }

    //MAKE ACTIONS:

    //move the RPs to their observing positions
    for(unsigned int i=0; i<p_1s.size(); i++) {
        double p_1 = p_1s[i];
        double p___3 = p___3s[i];
        FMM.RPL[i]->getActuator()->setAnglesSteps(p_1, p___3);
    }

    //disable the identified RPs of the FMM
    for(unsigned int i=0; i<Ids.size(); i++) {
        int Id = Ids[i];
        int j = FMM.RPL.searchId(Id);
        if(j >= FMM.RPL.getCount())
            throw EImpossibleError("lateral effect");
        TRoboticPositioner *RP = FMM.RPL[j];
        RP->Disabled = true;
    }

    //builds a MPG attached to the FMM
    TMotionProgramGenerator MPG(&FMM);

    //segregates the operative outsiders RPs
    TRoboticPositionerList Outsiders;
    FMM.RPL.segregateOperativeOutsiders(Outsiders);

    //Now must be fulfilled the preconditions:
    //  All RPs of the Fiber MOS Model, shall be in their starting positions.
    //  All RPs of the list Outsiders, shall be in the Fiber MOS Model.
    //  All RPs of the list Outsiders, shall be operatives.
    //  All RPs of the list Outsiders, shall be in unsecure positions.
    //  All RPs of the list Outsiders, shall have enabled the quantifiers.

    //generates a parking program for the operative RPs in insecure positions
    //and determines the RPs in collision status or obstructed in insecure positions
    TRoboticPositionerList Collided;
    TRoboticPositionerList Obstructed;
    ParkProg.Clear();
    bool ParkProgValid = MPG.generateParkProg(Collided, Obstructed, ParkProg, Outsiders);

    //Now are fulfilled the postconditions:
    //  All RPs of the Fiber MOS Model will be configured for validate a parking program
    //  All RPs of the fiber MOS Model will be in their final positions,
    //  or the first position where the collision was detected.
    //  All RPs of the Fiber MOS Model will have disabled the quantifiers.

    //########################################################################
    //#WARNING: before re-use a function to generation, shall be restablished#
    //#the preconditions.                                                    #
    //########################################################################

    //return the result of the generation process
    return ParkProgValid;
}

//Generates a pair (PP, DP).
//Inputs:
//  FMM: the Fiber MOS Model.
//  p_1s: the rotor 1 observing positions of all RPs of the FMM.
//  p___3s: the rotor 2 observing positions of all RPs of the FMM.
//  Ids: the identifiers of the RPs of the FMM to be disabled.
//Outputs:
//  generatePairPPDP_online: indicates if the generated pair (PP, DP) is valid.
//  PP: the positioning program to be generated.
//  DP: the depositioning program to be generated.
//Preconditions:
//  All operative outsider RPs of the FMM:
//    - shall be in the Fiber MOS Model;
//    - shall be operatives;
//    - shall be in insecurity positions;
//    - shall have enabled the quantifiers;
//  All position angles in the vector p_1s:
//    - must be in the rotor 1 domain of their corresponding RP.
//  All position angles in the vector p___3s:
//    - must be in the rotor 2 domain of their corresponding RP.
//  All identifiers in the vector Ids:
//    - must be referred to RPs in the FMM.
//Postconditions:
//  The RPs indicated in the list Ids will be disabled.
//  All RPs of the Fiber MOS Model will be configured for
//    - validate a PP. (Purpose = valPP).
//  When the generated pair (PP, DP) isn't valid:
//      All RPs of the FMM:
//        - will have disabled the quantifiers of their rotors;
//        - will be in the first position where the collision was detected
//              during the validation process.
//  When the generated pair (PP, DP) is valid (even in the trivial case):
//      All RPs of the FMM:
//        - will have enabled the quantifiers of their rotors;
//        - will be in their initial positions.
bool generatePairPPDP_online(TMotionProgram& PP, TMotionProgram& DP,
                             TFiberMOSModel& FMM,
                             const vector<double>& p_1s, const vector<double>& p___3s,
                             const vector<int>& Ids)
{
    //CHECK THE PRECONDITIONS:

    if(p_1s.size() != (unsigned int)FMM.RPL.getCount())
        throw EImproperArgument("vector p_1s should has one position for each RP of the FMM");
    if(p___3s.size() != (unsigned int)FMM.RPL.getCount())
        throw EImproperArgument("vector p___3s should has one position for each RP of the FMM");

    for(unsigned int i=0; i<p_1s.size(); i++) {
        double p_1 = p_1s[i];
        double p___3 = p___3s[i];
        TRoboticPositioner *RP = FMM.RPL[i];
        if(RP->getActuator()->isntInDomainp_1(p_1))
            throw EImproperArgument("position angle "+floattostr(p_1)+" isnt in the rotor 1 domain of the RP"+inttostr(RP->getActuator()->getId()));
        if(RP->getActuator()->getArm()->isntInDomainp___3(p___3))
            throw EImproperArgument("position angle "+floattostr(p___3)+" isnt in the rotor 2 domain of the RP"+inttostr(RP->getActuator()->getId()));
    }

    for(unsigned int i=0; i<Ids.size(); i++) {
        int Id = Ids[i];
        int j = FMM.RPL.searchId(Id);
        if(j >= FMM.RPL.getCount())
            throw EImproperArgument("vector Ids should contains identifiers of RPs of the FMM");
    }

    //MAKE ACTIONS:

    //move the RPs to their starting positions
    for(unsigned int i=0; i<p_1s.size(); i++) {
        double p_1 = p_1s[i];
        double p___3 = p___3s[i];
        FMM.RPL[i]->getActuator()->setAnglesSteps(p_1, p___3);
    }

    //disable the identified RPs of the FMM
    for(unsigned int i=0; i<Ids.size(); i++) {
        int Id = Ids[i];
        int j = FMM.RPL.searchId(Id);
        if(j >= FMM.RPL.getCount())
            throw EImpossibleError("lateral effect");
        TRoboticPositioner *RP = FMM.RPL[j];
        RP->Disabled = true;
    }

    //builds a MPG attached to the FMM
    TMotionProgramGenerator MPG(&FMM);

    //segregates the operative outsiders RPs
    TRoboticPositionerList Outsiders;
    FMM.RPL.segregateOperativeOutsiders(Outsiders);

    //Now are fulfilled the preconditions:
    //  All RPs of the Fiber MOS Model, shall be in their initial positions.
    //  All RPs of the list Outsiders, shall be in the Fiber MOS Model.
    //  All RPs of the list Outsiders, shall be operatives.
    //  All RPs of the list Outsiders, shall be in unsecure positions.
    //  All RPs of the list Outsiders, shall have enabled the quantifiers.

    //generates a de positioning program for the operative RPs in insecure positions
    //and determines the RPs in collision status or obstructed in insecure positions
    bool PPvalid = false;
    bool DPvalid = false;
    TRoboticPositionerList Collided;
    TRoboticPositionerList Obstructed;
    PP.Clear();
    DP.Clear();
    MPG.generatePairPPDP(PPvalid, DPvalid, Collided, Obstructed, PP, DP, Outsiders);

    //Now are fulfilled the postconditions:
    //  All RPs of the Fiber MOS Model will be configured for validate a PP
    //  All RPs of the fiber MOS Model will be in their final positions,
    //  or the first position where the collision was detected.
    //  All RPs of the Fiber MOS Model will have disabled the quantifiers.

    //########################################################################
    //#WARNING: before re-use a function to generation, shall be restablished#
    //#the preconditions.                                                    #
    //########################################################################

    //return the result of the generation process
    return PPvalid && DPvalid;
}
*****************************************************************************/

//----------------------------------------------------------------------------
//FMM AND OUTPUTS IN STRUCTURE FORMAT:

//Generate a parking program online
//Inputs:
//  FMM: the Fiber MOS Model
//  p_1s: the rotor 1 starting positions of all RPs of the FMM.
//  p___3s: the rotor 2 starting positions of all RPs of the FMM.
//  RPids: the identifiers of the RPs of the FMM to be disabled.
//Outputs:
//  outputs: structure OutputsParkProg.
//  generateParkProg_online:
//    - true: the generated parking program is suitable to be executed.
//    - false: the generated parking program is not suitable to be executed.
bool generateParkProg_online(OutputsParkProg& outputs,
        TFiberMOSModel& FMM,
        const vector<double>& p_1s, const vector<double>& p___3s,
        const vector<int>& RPids, const unsigned int Bid)
{
    //-------------------------------------------------------------------
    //CHECK PRECONDITIONS:

    if(p_1s.size() != (unsigned int)FMM.RPL.getCount())
        throw EImproperArgument("vector p_1s should has one position for each RP of the FMM");
    if(p___3s.size() != (unsigned int)FMM.RPL.getCount())
        throw EImproperArgument("vector p___3s should has one position for each RP of the FMM");

    for(int i=0; i<FMM.RPL.getCount(); i++) {
        TRoboticPositioner *RP = FMM.RPL[i];
        double p_1 = p_1s[(unsigned int)i];
        double p___3 = p___3s[(unsigned int)i];
        if(RP->getActuator()->isntInDomainp_1(p_1))
            throw EImproperArgument("position angle "+floattostr(p_1)+" isn't in the rotor 1 domain of the RP"+RP->getActuator()->getIdText().str);
        if(RP->getActuator()->getArm()->isntInDomainp___3(p___3))
            throw EImproperArgument("position angle "+floattostr(p___3)+" isn't in the rotor 2 domain of the RP"+RP->getActuator()->getIdText().str);
    }

    for(unsigned int i=0; i<RPids.size(); i++) {
        int RPid = RPids[i];
        int j = FMM.RPL.searchId(RPid);
        if(j >= FMM.RPL.getCount())
            throw EImproperArgument("vector RPids should contains exclusively identifiers of RPs of the FMM");
    }

    //-------------------------------------------------------------------
    //CALL THE FUNCTION FOR GENERATE PARKING PROGRAM ONLINE:

    //build a motion program generator attached to the FMM
    TMotionProgramGenerator MPG(&FMM);

    //set the disabling status
    FMM.RPL.setAllDisabled(false);
    for(unsigned int i=0; i<RPids.size(); i++) {
        int j = FMM.RPL.searchId(RPids[i]);
        TRoboticPositioner *RP = FMM.RPL[j];
        RP->Disabled = true;
    }

    //move the enabled RPs to their observing positions
    //and segregate the enabled-not-operative RPs
    TRoboticPositionerList EnabledNotOperative;
    for(int i=0; i<FMM.RPL.getCount(); i++) {
        TRoboticPositioner *RP = FMM.RPL[i];
        if(!RP->Disabled) {
            double p_1 = p_1s[(unsigned int)i];
            double p___3 = p___3s[(unsigned int)i];
            FMM.RPL[i]->getActuator()->setAnglesSteps(p_1, p___3);
            if(!RP->getOperative())
                EnabledNotOperative.Add(RP);
        }
    }

    //check that there aren't enabled-not-operative with fault type dynamic or unknowledge
    if(EnabledNotOperative.searchFaultDynOrUnk() < EnabledNotOperative.getCount())
        return false;

    //captures the starting positions of the RPs in a PPA list
    TPairPositionAnglesList SPL;
    FMM.RPL.getPositions(SPL);

    //A PPA list is a table with the fields (Id, p_1, p___3).

    //get the list of colliding items (both EAs and RPs) in text format
    FMM.RPL.setPurpose(pGenParPro);
    string collided_str;
    FMM.getCollidedText(collided_str);

    //Note that MPG.generateParkProg will execute automatically:
    //  //configure the Fiber MOS Model for generate a DP
    //  getFiberMOSModel()->RPL.setPurpose(pGenParkProg);

    //segregates the operative-outsider RPs
    TRoboticPositionerList Outsiders;
    FMM.RPL.segregateOperativeOutsiders(Outsiders);

    //Note that operative RPs are enabled ever.

    //Now are fulfilled the preconditions for the function TMotionProgramGenerator::generateParkProg:
    //  All RPs of the Fiber MOS Model:
    //    - shall be in their starting positions.
    //  All RPs of the list Outsiders:
    //    - shall be in the Fiber MOS Model.
    //    - shall be operatives.
    //    - shall be in insecurity positions.
    //    - shall have enabled the quantifiers.

    //generates a parking program and determines the collided and obstructed RPs in insecurity positions
    TRoboticPositionerList Collided;
    TRoboticPositionerList Obstructed;
    TMotionProgram ParkProg;
    bool ParkProgValid = MPG.generateParkProg(Collided, Obstructed, ParkProg, Outsiders);

    //Now are fulfilled the postconditions:
    //  All RPs of the FMM:
    //    - will be configured for validate a parking program.
    //  When the generated recovery program isn't valid:
    //      All RPs of the FMM:
    //        - will have disabled the quantifiers of their rotors;
    //        - will be in the first position where the collision was detected
    //              during the validation process.
    //  When the generated recovery program is valid (even the trivial case):
    //      All RPs of the FMM:
    //        - will have enabled the quantifiers of their rotors;
    //        - will be in their final positions.

    //check that the generated parking program is valid
    if(!ParkProgValid)
        return false;

    //captures the final positions of the RPs in a PPA list
    TPairPositionAnglesList FPL;
    FMM.RPL.getPositions(FPL);

    //-------------------------------------------------------------------
    //RETURN THE OUTPUTS:

    //make a rutinary check
    if(SPL.getCount() != FMM.RPL.getCount())
        throw EImpossibleError("lateral effect");
    if(FPL.getCount() != FMM.RPL.getCount())
        throw EImpossibleError("lateral effect");

    //set values in the structure outputs:

    //properties for built comments about file outputs
    outputs.FMOSA_filename = ""; //here there isn't FMOSA_filename
    outputs.FMPT_version = string(PACKAGE_VERSION);
    outputs.datetime = strfromtime_t(time(NULL));
    outputs.EnabledNotOperative = EnabledNotOperative;
    outputs.collided_str = collided_str;
    outputs.Collided = Collided;
    outputs.Obstructed = Obstructed;
    //properties for built comments about each MP
    outputs.ParkProgValid = ParkProgValid;
    outputs.DsecMax = 1;
    //properties for built the executable data
    outputs.Bid = Bid;
    outputs.SPL = SPL;
    outputs.ParkProg = ParkProg;
    outputs.FPL = FPL;

    //indicates if the outputs is suitable to be executed
    return outputs.suitable();

    //Note that when outputs is not suitable to be executed,
    //the outputs is generated with a initial uncommented note,
    //for difficult their execution.
}

//Generate a pair (PP, DP) online
//Inputs:
//  FMM: the Fiber MOS Model
//  p_1s: the rotor 1 observing positions of all RPs of the FMM.
//  p___3s: the rotor 2 observing positions of all RPs of the FMM.
//  RPids: the identifiers of the RPs of the FMM to be disabled.
//Outputs:
//  outputs: structure OutputsPairPPDP (without FMOSA).
//  generatePairPPDP_online:
//    - true: the generated pair (PP, DP) is suitable to be executed.
//    - false: the generated pair (PP, DP) is not suitable to be executed.
bool generatePairPPDP_online(OutputsPairPPDP& outputs,
        TFiberMOSModel& FMM,
        const vector<double>& p_1s, const vector<double>& p___3s,
        const vector<int>& RPids, const unsigned int Bid)
{
    //-------------------------------------------------------------------
    //CHECK PRECONDITIONS:

    if(p_1s.size() != (unsigned int)FMM.RPL.getCount())
        throw EImproperArgument("vector p_1s should has one position for each RP of the FMM");
    if(p___3s.size() != (unsigned int)FMM.RPL.getCount())
        throw EImproperArgument("vector p___3s should has one position for each RP of the FMM");

    for(unsigned int i=0; i<p_1s.size(); i++) {
        double p_1 = p_1s[i];
        double p___3 = p___3s[i];
        TRoboticPositioner *RP = FMM.RPL[i];
        if(RP->getActuator()->isntInDomainp_1(p_1))
            throw EImproperArgument("position angle "+floattostr(p_1)+" isn't in the rotor 1 domain of the RP"+RP->getActuator()->getIdText().str);
        if(RP->getActuator()->getArm()->isntInDomainp___3(p___3))
            throw EImproperArgument("position angle "+floattostr(p___3)+" isn't in the rotor 2 domain of the RP"+RP->getActuator()->getIdText().str);
    }

    for(unsigned int i=0; i<RPids.size(); i++) {
        int RPid = RPids[i];
        int j = FMM.RPL.searchId(RPid);
        if(j >= FMM.RPL.getCount())
            throw EImproperArgument("vector RPids should contains exclusively identifiers of RPs of the FMM");
    }

    //-------------------------------------------------------------------
    //CALL THE FUNCTION FOR GENERATE PAIRS (PP, DP) ONLINE:

    //build a motion program generator attached to the FMM
    TMotionProgramGenerator MPG(&FMM);

    //set the disabling status
    FMM.RPL.setAllDisabled(false);
    for(unsigned int i=0; i<RPids.size(); i++) {
        int j = FMM.RPL.searchId(RPids[i]);
        TRoboticPositioner *RP = FMM.RPL[j];
        RP->Disabled = true;
    }

    //move the enabled RPs to their observing positions
    //and segregate the enabled-not-operative RPs
    TRoboticPositionerList EnabledNotOperative;
    for(int i=0; i<FMM.RPL.getCount(); i++) {
        TRoboticPositioner *RP = FMM.RPL[i];
        if(!RP->Disabled) {
            double p_1 = p_1s[i];
            double p___3 = p___3s[i];
            FMM.RPL[i]->getActuator()->setAnglesSteps(p_1, p___3);
            if(!RP->getOperative())
                EnabledNotOperative.Add(RP);
        }
    }

    //check that there aren't enabled-not-operative with fault type dynamic or unknowledge
    if(EnabledNotOperative.searchFaultDynOrUnk() < EnabledNotOperative.getCount())
        return false;

    //captures the observing positions of the RPs in a PPA list
    TPairPositionAnglesList OPL;
    FMM.RPL.getPositions(OPL);

    //A PPA list is a table with the fields (Id, p_1, p___3).

    //get the list of colliding items (both EAs and RPs) in text format
    FMM.RPL.setPurpose(pGenPairPPDP);
    string collided_str;
    FMM.getCollidedText(collided_str);

    //Note that MPG.generatePairPPDP will execute automatically:
    //  //configure the Fiber MOS Model for generate a DP
    //  getFiberMOSModel()->RPL.setPurpose(pGenPairPPDP);

    //segregates the operative-outsider RPs
    TRoboticPositionerList Outsiders;
    FMM.RPL.segregateOperativeOutsiders(Outsiders);

    //Note that operative RPs are enabled ever.

    //Now are fulfilled the preconditions for the function TMotionProgramGenerator::generatePairPPDP:
    //  All RPs of the Fiber MOS Model:
    //    - shall be in their observing positions.
    //  All RPs of the list Outsiders:
    //    - shall be in the Fiber MOS Model.
    //    - shall be operatives.
    //    - shall be in insecurity positions.
    //    - shall have enabled the quantifiers.
    //    - shall have velocity of rotor 2 approximately double than rotor 1.

    //generates a pair (PP, DP) and determines the collided and obstructed RPs in insecurity positions
    bool PPvalid = false;
    bool DPvalid = false;
    TRoboticPositionerList Collided;
    TRoboticPositionerList Obstructed;
    TMotionProgram PP;
    TMotionProgram DP;
    MPG.generatePairPPDP(PPvalid, DPvalid, Collided, Obstructed, PP, DP, Outsiders);

    //Now are fulfilled the postconditions:
    //  All RPs of the Fiber MOS Model:
    //    - will be configured for validate a PP. (Purpose = pValPP).
    //    - will have disabled the quantifiers.
    //  When the generated pair (PP, DP) isn't valid:
    //      All RPs of the FMM:
    //        - will have disabled the quantifiers of their rotors;
    //        - will be in the first position where the collision was detected
    //              during the validation process.
    //  When the generated pair (PP, DP) is valid (even the trivial case):
    //      All RPs of the FMM:
    //        - will have enabled the quantifiers of their rotors;
    //        - will be in their initial positions.

    //check that the generated pair (PP, DP) is valid
    if(!PPvalid || !DPvalid)
        return false;

    //capture the initial positions of the RPs in a PPA list
    TPairPositionAnglesList IPL;
    FMM.RPL.getPositions(IPL);

    //-------------------------------------------------------------------
    //RETURN THE OUTPUTS:

    //make a rutinary check
    if(OPL.getCount() != FMM.RPL.getCount())
        throw EImpossibleError("lateral effect");
    if(IPL.getCount() != FMM.RPL.getCount())
        throw EImpossibleError("lateral effect");

    //set values in the structure outputs:

    //properties for built comments about file outputs
    outputs.FMOSA_filename = ""; //here there isn't FMOSA_filename
    outputs.FMPT_version = string(PACKAGE_VERSION);
    outputs.datetime = strfromtime_t(time(NULL));
    outputs.EnabledNotOperative = EnabledNotOperative;
    outputs.collided_str = collided_str;
    outputs.Collided = Collided;
    outputs.Obstructed = Obstructed;
    //properties for built comments about each MP
    outputs.PPvalid = PPvalid;
    outputs.DPvalid = DPvalid;
    outputs.DsecMax = 1;
    //properties for built the executable data
    outputs.Bid = Bid;
    outputs.IPL = IPL;
    outputs.PP = PP;
    outputs.OPL = OPL;
    outputs.DP = DP;
    outputs.FMOSA.Clear(); //here there isn't FMOSA

    //indicates if the outputs is suitable to be executed
    return outputs.suitable();
}

//----------------------------------------------------------------------------
//FMM IN STRING FORMAT AND OUTPUTS IN STRUCTURE FORMAT:

//Generate a parking program online
//Inputs:
//  FMMI_dir: dir of the FMM Instance
//  p_1s: the rotor 1 starting positions of all RPs of the FMM.
//  p___3s: the rotor 2 starting positions of all RPs of the FMM.
//  RPids: the identifiers of the RPs of the FMM to be disabled.
//Outputs:
//  outputs: structure OutputsParkProg.
//  generateParkProg_online:
//    - true: the generated parking program is suitable to be executed.
//    - false: the generated parking program is not suitable to be executed.
bool generateParkProg_online(OutputsParkProg& outputs,
        const string& FMMI_dir,
        const vector<double>& p_1s, const vector<double>& p___3s,
        const vector<int>& RPids, const unsigned int Bid)
{
    //-------------------------------------------------------------------
    //CHECK PRECONDITIONS:

    locale l;
    if(l.name() != "C")
        throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

    //-----------------------------------------------------------------------
    //LOAD THE FIBER MOS MODEL INSTANCE:

    //load the FMM Instance from a dir
    TFiberMOSModel FMM;
    try {
        readInstanceFromDir(FMM, FMMI_dir);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "loading instance: ");
        throw;
    }

    //make a rutinary cheack
    if(FMM.RPL.Compare != TRoboticPositioner::compareIds)
        throw EImpossibleError("improper initialization value of TRoboticPositionerList::Compare");

    //short the RPs according their identifiers
    FMM.RPL.SortInc();

    //-----------------------------------------------------------------------
    //GENERATE THE PARKING PROGRAM:

    bool suitable = generateParkProg_online(outputs, FMM,
                                            p_1s, p___3s, RPids, Bid);

    //-----------------------------------------------------------------------
    //SAVE THE FMM INSTANCE, IF ANY:

    if(suitable) {
        //save the FMM Instance in the same dir from was loaded
        try {
            writeInstanceToDir(FMMI_dir, FMM);
        }
        catch(Exception& E) {
            E.Message.Insert(1, "writing instance: ");
            throw;
        }

        //Only FMM Instance's changed files will be written.
    }

    return suitable;
}

//Generate a pair (PP, DP) online
//Inputs:
//  FMMI_dir: dir of the FMM Instance
//  p_1s: the rotor 1 observing positions of all RPs of the FMM.
//  p___3s: the rotor 2 observing positions of all RPs of the FMM.
//  RPids: the identifiers of the RPs of the FMM to be disabled.
//Outputs:
//  outputs: structure OutputsPairPPDP (without FMOSA).
//  generatePairPPDP_online:
//    - true: the generated pair (PP, DP) is suitable to be executed.
//    - false: the generated pair (PP, DP) is not suitable to be executed.
bool generatePairPPDP_online(OutputsPairPPDP& outputs,
        const string& FMMI_dir,
        const vector<double>& p_1s, const vector<double>& p___3s,
        const vector<int>& RPids, const unsigned int Bid)
{
    //-------------------------------------------------------------------
    //CHECK PRECONDITIONS:

    locale l;
    if(l.name() != "C")
        throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

    //-----------------------------------------------------------------------
    //LOAD THE FIBER MOS MODEL INSTANCE:

    //load the FMM Instance from a dir
    TFiberMOSModel FMM;
    try {
        readInstanceFromDir(FMM, FMMI_dir);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "loading instance: ");
        throw;
    }

    //make a rutinary cheack
    if(FMM.RPL.Compare != TRoboticPositioner::compareIds)
        throw EImpossibleError("improper initialization value of TRoboticPositionerList::Compare");

    //short the RPs according their identifiers
    FMM.RPL.SortInc();

    //-----------------------------------------------------------------------
    //GENERATE THE PARKING PROGRAM:

    bool suitable = generatePairPPDP_online(outputs, FMM,
                                            p_1s, p___3s, RPids, Bid);

    //-----------------------------------------------------------------------
    //SAVE THE FMM INSTANCE, IF ANY:

    if(suitable) {
        //Note that when outputs is not suitable to be executed,
        //the outputs is generated with a initial uncommented note,
        //for difficult their execution.

        //save the FMM Instance in the same dir from was loaded
        try {
            writeInstanceToDir(FMMI_dir, FMM);
        }
        catch(Exception& E) {
            E.Message.Insert(1, "writing instance: ");
            throw;
        }

        //Only FMM Instance's changed files will be written.
    }

    return suitable;
}

//----------------------------------------------------------------------------
//FMM AND OUTPUTS IN STRING FORMAT:

//Generate a parking program online
//Inputs:
//  FMMI_dir: dir of the FMM Instance
//  p_1s: the rotor 1 starting positions of all RPs of the FMM.
//  p___3s: the rotor 2 starting positions of all RPs of the FMM.
//  RPids: the identifiers of the RPs of the FMM to be disabled.
//  Bid: identifier of the block.
//Outputs:
//  outputs_str: structure OutputsParkProg in format string.
//  generateParkProg_online:
//    - true: the generated parking program is suitable to be executed.
//    - false: the generated parking program is not suitable to be executed.
bool generateParkProg_online(string& outputs_str,
        const string& FMMI_dir,
        const vector<double>& p_1s, const vector<double>& p___3s,
        const vector<int>& RPids, const unsigned int Bid)
{
    //call the function online (in format structure)
    OutputsParkProg outputs;
    bool valid = generateParkProg_online(outputs,
                                         FMMI_dir, p_1s, p___3s, RPids, Bid);

    //print the structure outputs
    outputs.getText(outputs_str);

    //indicates that conditions for approbation are met
    return valid;
}

//Generate a pair (PP, DP) online
//Inputs:
//  FMMI_dir: dir of the FMM Instance
//  p_1s: the rotor 1 observing positions of all RPs of the FMM.
//  p___3s: the rotor 2 observing positions of all RPs of the FMM.
//  RPids: the identifiers of the RPs of the FMM to be disabled.
//  Bid: identifier of the block.
//Outputs:
//  outputs_str: structure OutputsPairPPDP in format string (without FMOSA).
//  generatePairPPDP_online:
//    - true: the generated pair (PP, DP) is suitable to be executed.
//    - false: the generated pair (PP, DP) is not suitable to be executed.
bool generatePairPPDP_online(string& outputs_str,
        const string& FMMI_dir,
        const vector<double>& p_1s, const vector<double>& p___3s,
        const vector<int>& RPids, const unsigned int Bid)
{
    //call the function online (in format structure)
    OutputsPairPPDP outputs;
    bool valid = generatePairPPDP_online(outputs,
                                         FMMI_dir, p_1s, p___3s, RPids, Bid);

    //print the structure outputs without FMOSA
    outputs.getText(outputs_str, false);

    //indicates that conditions for approbation are met
    return valid;
}

//---------------------------------------------------------------------------

//get the version of the package
string getPACKAGE_VERSION(void)
{
    return string(PACKAGE_VERSION);
}

} //namespace Positioning

//---------------------------------------------------------------------------

