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
//File: MotionProgramGenerator.cpp
//Content: generador de programas de movimiento
//Last update: 06/05/2014
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "MotionProgramGenerator.h"
#include "MessageInstruction.h"
#include "SkyPoint.h"

#include <unistd.h>
#include <limits> //std::numeric_limits

//---------------------------------------------------------------------------

using namespace Models;

//spacename for positioning
namespace Positioning {

//###########################################################################
//TMotionProgramGenerator:
//###########################################################################

//---------------------------------------------------------------------------
//SETTING PARAMETERS OF THE ALGORITHMS:

void TMotionProgramGenerator::setMSD(double MSD)
{
    if(MSD < 0)
        throw EImproperArgument("maximun security distance MSD should be nonnegative");

    p_MSD = MSD;
}

//---------------------------------------------------------------------------
//METHODS OF LOWER LEVEL:

//segregate the RPs of the list Outsiders, in disjoint subsets
void TMotionProgramGenerator::segregateRPsInDisjointSets(
        TPointersList<TRoboticPositionerList>& DisjointSets,
        const TRoboticPositionerList& Outsiders)
{
    //CHECK THE PRECONDITIONS:

    for(int i=0; i<Outsiders.getCount(); i++) {
        TRoboticPositioner *RP = Outsiders[i];
        if(!RP->getOperative())
            throw EImproperArgument("all RPs in thelist Outsiders shall be operatives");
        if(RP->getActuator()->ArmIsInSafeArea())
            throw EImproperArgument("all RPs in thelist Outsiders shall be in insecurity positions");
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
        for(int j=0; j<RP->getActuator()->Adjacents.getCount(); j++) {
            TRoboticPositioner *RPA = RP->getActuator()->Adjacents[j];
            if(RPA->getActuator()->ArmIsOutSafeArea()) {
                int k = Outsiders.Search(RPA);
                if(k < Outsiders.getCount())
                    Adjacents.Add(RPA);
            }
        }

        //NOTE: list Adjacents is of type TItemsList<TRoboticPositioner*>,
        //not TRoboticPositionerList.

        //Here has been found the adjacents of a RP in the list RP->getActuator()->Adjacents.
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

        //if not founds the adjacent in the disjoint sets
        if(index.getCount() <= 0) {
            //add the RP in a new set
            TRoboticPositionerList *Set = new TRoboticPositionerList();
            Set->Add(RP);
            DisjointSets.Add(Set);
        }
        //if has found some adjacent in the disjoint sets
        else {
            //join the disjoint sets where the RPAs was found
            TRoboticPositionerList *Set = DisjointSets.GetPointer(index[0]);
            for(int j=1; j<index.getCount(); j++) {
                TRoboticPositionerList *SetToAdd = DisjointSets.GetPointer(index[j]);
                for(int k=0; k<SetToAdd->getCount(); k++) {
                    TRoboticPositioner *RPToAdd = SetToAdd->Get(k);
                    Set->Add(RPToAdd);
                }
                DisjointSets.Delete(index[j]);
            }
            //add the RP to the Set
            Set->Add(RP);
        }
    }
}

//verify that all RPs of a list follow the MEGARA distribution
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

//segregate the RPs of a set, in disperse subsets
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
        int j = getFiberMOSModel()->RPL.Search(RP->getActuator());
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

//=================================================================================================================================

//Propose a recovery program composed by one or two gestures:
//  1. Radial retraction to where it is possible.
//  2. Abatement of the arm to security position (if necessary).
//Input:
//  RP: The RP containing the properties (MPturn, MPretraction).
//Outputs:
//  RP->MPturn: motion program for turn the rotor 1.
//  RP->MPretraction: motionprogram for retract the arm.
//Preconditions:
//- Pointer RP shall point to built robotic positioner.
//- The RP shall be enabled the quantifiers of their rotors.
//- The RP shall be in unsecurity position.
//Postconditions:
//- The RP->MPturn will be empty.
//- The RP->MPretraction will contains the MP for retract the arm
//  to the first stable security position.
void TMotionProgramGenerator::proposeRecoveryProgram(TRoboticPositioner *RP)
{
    //CHECK THE PRECONDITIONS:

    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
        throw EImproperCall("the RP shuould be enabled the quantifiers of their rotors");

    if(RP->getActuator()->ArmIsInSafeArea())
        throw EImproperArgument("The SPshould has the arm in unsecurity position");

    //MAKE ACTIONS:

    //initialize the output variables
    RP->MPturn.Clear();
    RP->MPretraction.Clear();

    //CALCULATES DE FINAL POSITIONS OF THE ROTORS (p_1fin, p___3fin, p___3saf):

    //determines the first stable security position of rot 2 in steps
    double theta___3saf = RP->getActuator()->gettheta___3saf();
    double p___3saf = RP->getActuator()->getArm()->getF().Image(theta___3saf);
    p___3saf = floor(p___3saf);

    //perform a rutinary check
    if(p___3saf < 0)
        throw EImpossibleError("rotor 2 position (p___3saf) should be nonnegative");

    //Note that if(p___3saf < RP->getActuator()->getArm()->getp___3first())
    //the MP will be proposed but it will not pass the validation process.

    //determines the necessary displacement of rot 2 in radians
    theta___3saf = RP->getActuator()->getArm()->getG().Image(p___3saf);
    double theta___3ini = RP->getActuator()->getArm()->gettheta___3();
    double dt2 = theta___3ini - theta___3saf;

    //calculates the necessary displacement of rotor 1 in radians
    double dt1 = dt2/2;

    //get the initial position of the rotors 1 in radians
    double theta_1ini = RP->getActuator()->gettheta_1();

    //determines the final position of the rotors 1 in radians
    double theta_1fin;
    theta_1fin = theta_1ini - dt1;

    //determines the final position of rot 1 in steps
    double p_1fin = RP->getActuator()->getF().Image(theta_1fin);
    p_1fin = round(p_1fin);

    //determines if the rotor 1 have enough space for retraction
    double p_1first = RP->getActuator()->getp_1first();
    p_1first = ceil(p_1first); //purge lateral effect
    bool rot1_has_enough = (p_1fin >= p_1first);

    //calculates the final positions according the space available for the rotor 1
    double p___3fin;
    //if the rotor 1 has enpough space for execue the retraction
    if(rot1_has_enough)
        //determines the final position of rot 2 in steps
        p___3fin = p___3saf;

    //if the rotor 1 not has enpough space for execue the retraction
    else {
        //determines the final position of rot 1 in steps
        p_1fin = RP->getActuator()->getp_1first();

        //calulates the final position of the rotors in radians
        theta_1fin = RP->getActuator()->getG().Image(p_1fin);
        dt1 = theta_1ini - theta_1fin;
        dt2 = dt1*2;
        double theta___3fin = theta___3ini - dt2;

        //determines the final position of rot 2 in steps
        p___3fin = RP->getActuator()->getF().Image(theta___3fin);
        p___3fin = round(p___3fin);
    }

    //BUILD AND ADD THE INSTRUCTION FOR RADIAL RETRACTION:

    //build a message instruction and set it
    TMessageInstruction *MI = new TMessageInstruction();
    MI->setId(RP->getActuator()->getId());
    MI->Instruction.setName("MM");
    MI->Instruction.Args.setCount(2);
    MI->Instruction.Args[0] = p_1fin;
    MI->Instruction.Args[1] = p___3fin;
    if(RP->getDsec() < getMSD())
        MI->setComment("Dsec = "+floattostr(RP->getDsec())+" mm");

    //add the message instruction to the MP
    Positioning::TMessageList *ML = new Positioning::TMessageList();
    ML->Add(MI);
    RP->MPretraction.Add(ML);

    //ADD INSTRUCTION FOR ABATEMENT OF THE ARM IF NECESARY:

    //determines if it is necessary abate the arm for recovery the security position
    bool necessary_abate = (p___3fin > p___3saf);

    if(necessary_abate) {
        //build a message instruction and set up
        TMessageInstruction *MI = new TMessageInstruction();
        MI->setId(RP->getActuator()->getId());
        MI->Instruction.setName("M2");
        MI->Instruction.Args.setCount(1);
        MI->Instruction.Args[0] = p___3saf;
        if(RP->getDsec() < getMSD())
            MI->setComment("Dsec = "+floattostr(RP->getDsec())+" mm");

        //add the message instruction to the MP
        Positioning::TMessageList *ML = new Positioning::TMessageList();
        ML->Add(MI);
        RP->MPretraction.Add(ML);
    }
}

//Propose a recovery program composed by one or two gestures:
//  1. Radial retraction to where it is possible.
//  2. Abatement of the arm to security position (if necessary).
//Input:
//  RP: the RP containing the properties (MPturn, MPretraction).
//  p_1new: the position to turn the rotor 1 of the RP.
//Outputs:
//  RP->MPturn: motion program for turn the rotor 1.
//  RP->MPretraction: motionprogram for retract the arm.
//Preconditions:
//- Pointer RP shall point to built robotic positioner.
//- The RP shall be enabled the quantifiers of their rotors.
//- The RP shall be in unsecurity position.
//- The new rotor 1 position p_1new shall be in the rotor 1 domain.
//Postconditions:
//- The RP->MPturn will contains the MP for turn the rotor 1
//  to the position p_1new.
//- The RP->MPretraction will contains the MP for retract the arm
//  to the first stable security position.
void TMotionProgramGenerator::proposeRecoveryProgram(TRoboticPositioner *RP, double p_1new)
{
    //CHECK THE PRECONDITIONS:

    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
        throw EImproperCall("the RP shuould be enabled the quantifiers of their rotors");

    if(RP->getActuator()->ArmIsInSafeArea())
        throw EImproperArgument("The SPshould has the arm in unsecurity position");

    if(RP->getActuator()->IsntInDomainp_1(p_1new))
        throw EImproperArgument("the new rotor 1 position p_1new should be in the rotor 1 domain");

    //MAKE ACTIONS:

    //initialize the output variables
    RP->MPturn.Clear();
    RP->MPretraction.Clear();

    //BUILD AND ADD THEINSTRUCTION FORROTOR 1 TURN:

    //add a instruction for turn of the rotor 1
    TMessageInstruction *MI = new TMessageInstruction();
    MI->setId(RP->getActuator()->getId());
    MI->Instruction.setName("M1");
    MI->Instruction.Args.setCount(1);
    MI->Instruction.Args[0] = p_1new;
    Positioning::TMessageList *ML = new Positioning::TMessageList();
    ML->Add(MI);
    RP->MPturn.Add(ML);

    //CALCULATES DE FINAL POSITIONS OF THE ROTORS (p_1fin, p___3fin, p___3saf):

    //determines the first stable security position of rot 2 in steps
    double theta___3saf = RP->getActuator()->gettheta___3saf();
    double p___3saf = RP->getActuator()->getArm()->getF().Image(theta___3saf);
    p___3saf = floor(p___3saf);

    //perform a rutinary check
    if(p___3saf < 0)
        throw EImpossibleError("rotor 2 position (p___3saf) should be nonnegative");

    //Note that if(p___3saf < RP->getActuator()->getArm()->getp___3first())
    //the MP will be proposed but it will not pass the validation process.

    //determines the necessary displacement of rot 2 in radians
    theta___3saf = RP->getActuator()->getArm()->getG().Image(p___3saf);
    double theta___3ini = RP->getActuator()->getArm()->gettheta___3();
    double dt2 = theta___3ini - theta___3saf;

    //calculates the necessary displacement of rotor 1 in radians
    double dt1 = dt2/2;

    //get the initial position of the rotors 1 in radians
    double theta_1ini = RP->getActuator()->gettheta_1();

    //determines the final position of the rotors 1 in radians
    double theta_1fin;
    theta_1fin = theta_1ini - dt1;

    //determines the final position of rot 1 in steps
    double p_1fin = RP->getActuator()->getF().Image(theta_1fin);
    p_1fin = round(p_1fin);

    //determines if the rotor 1 have enough space for retraction
    double p_1first = RP->getActuator()->getp_1first();
    p_1first = ceil(p_1first); //purge lateral effect
    bool rot1_has_enough = (p_1fin >= p_1first);

    //calculates the final positions according the space available for the rotor 1
    double p___3fin;
    //if the rotor 1 has enpough space for execue the retraction
    if(rot1_has_enough)
        //determines the final position of rot 2 in steps
        p___3fin = p___3saf;

    //if the rotor 1 not has enpough space for execue the retraction
    else {
        //determines the final position of rot 1 in steps
        p_1fin = RP->getActuator()->getp_1first();

        //calulates the final position of the rotors in radians
        theta_1fin = RP->getActuator()->getG().Image(p_1fin);
        dt1 = theta_1ini - theta_1fin;
        dt2 = dt1*2;
        double theta___3fin = theta___3ini - dt2;

        //determines the final position of rot 2 in steps
        p___3fin = RP->getActuator()->getF().Image(theta___3fin);
        p___3fin = round(p___3fin);
    }

    //BUILD AND ADD THE INSTRUCTION FOR RADIAL RETRACTION:

    //build a message instruction and set it
    MI = new TMessageInstruction();
    MI->setId(RP->getActuator()->getId());
    MI->Instruction.setName("MM");
    MI->Instruction.Args.setCount(2);
    MI->Instruction.Args[0] = p_1fin;
    MI->Instruction.Args[1] = p___3fin;
    if(RP->getDsec() < getMSD())
        MI->setComment("Dsec = "+floattostr(RP->getDsec())+" mm");

    //add the message instruction to the MP
    ML = new Positioning::TMessageList();
    ML->Add(MI);
    RP->MPretraction.Add(ML);

    //ADD INSTRUCTION FOR ABATEMENT OF THE ARM IF NECESARY:

    //determines if it is necessary abate the arm for recovery the security position
    bool necessary_abate = (p___3fin > p___3saf);

    if(necessary_abate) {
        //build a message instruction and set up
        TMessageInstruction *MI = new TMessageInstruction();
        MI->setId(RP->getActuator()->getId());
        MI->Instruction.setName("M2");
        MI->Instruction.Args.setCount(1);
        MI->Instruction.Args[0] = p___3saf;
        if(RP->getDsec() < getMSD())
            MI->setComment("Dsec = "+floattostr(RP->getDsec())+" mm");

        //add the message instruction to the MP
        Positioning::TMessageList *ML = new Positioning::TMessageList();
        ML->Add(MI);
        RP->MPretraction.Add(ML);
    }
}

//Determines if a motion program produces a colission,
//Preconditions:
//- All RPs of the Fiber MOS Model:
//      shall be configured for MP generation;
//      shall be enabled the quantifiers of their rotors;
//      shall has stacked the actual position of their rotors.
//- The motion programs (MPturn, MPretraction) shall be valids.
//  If the MP contains a single message list:
//      the single message instruction shall be for retract the arm.
//  If the MPcontains twomessages of onstruction,
//      the first message instruction shall be for turn the rotor 1,
//      and the second message instruction shall be for retract the arm.
bool TMotionProgramGenerator::motionProgramsAreValid(const TMotionProgram& MPturn,
                                                   const TMotionProgram& MPretraction) const
{
    //CHECK THE PRECONDITIONS:

    for(int i=0; i<getFiberMOSModel()->RPL.getCount(); i++) {
        TRoboticPositioner *RP = getFiberMOSModel()->RPL[i];
        if(RP->getActuator()->getPurpose() != pGen)
            throw EImproperCall("all RPs of the Fiber MOS Model should be configured for MP generation");
    }

    //determine the list of RPs includes in the MPs
    TRoboticPositionerList RPL;
    getRPsIncludedInMPs(RPL, MPturn, MPretraction, getFiberMOSModel());

    if(RPL.getCount() != 1)
        throw EImproperArgument("the motion programs MPturn and MP retraction should be included only a RP");

    //search the RP included in the Fiber MOS Model
    TRoboticPositioner *RP = RPL.getFirst();
    int i = getFiberMOSModel()->RPL.Search(RP);

    if(i >= getFiberMOSModel()->RPL.getCount())
        throw EImproperArgument("the RP included in the motion programs MPturn and MPretraction, should be in the Fiber MOS Model");

    if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
        throw EImproperCall("the RP shuould be enabled the quantifiers of their rotors");

    if(RP->getActuator()->thetasNotCoincideWithStacked())
        throw EImproperCall("the RP included in the motion programs MPturn and MPretraction, should have stacked theactual positionof their rotors");

    //MAKE ACTIONS:

    //validate MPturn
    bool valid = validateMotionProgram(RP->MPturn);

    //Here the RP included in the MPturn, has in the position from which start the retraction.

    if(valid) {
        //change the SPM
        double SPMbak = RP->getActuator()->getArm()->getSPM();
        RP->getActuator()->getArm()->setSPM(SPMbak + RP->getDsec());

        //validate MPretraction
        valid = validateMotionProgram(RP->MPretraction);

        //Here el RP has in the final-security position.

        //restore the initial value of the SPM
        RP->getActuator()->getArm()->setSPM(SPMbak);
    }

    //restore the initial status of the Fiber MOS Model
    getFiberMOSModel()->RPL.RestorePositions();
    if(!valid)
        getFiberMOSModel()->RPL.RestoreAndPopQuantifys();

    //return the result of the validation process
    return valid;
}


//Search in negative sense, the individual MP for recover the security
//position of a RP avoiding dynamic collisions.
//Inputs:
//  RP: robotic positioner to be recovered.
//  dt1max: maximun displacement of the rotor 1 for search the solution
//      in rad.
//Outputs:
//  searchSolutionInNegativeSense: indicates if the solution is valid.
//  p_1new: last position where solution has been searched.
//  (RP->MPturn, RP->MPretraction): the solution.
//Preconditions:
//  All RPs of the Fiber MOS Model shall be configurated for MP generation.
//  The pointer RP shall shall point to built robotic positioner.
//  The RP:
//      shall be in the Fiber MOS  Model;
//      shall be in unsecurity position;
//      shall be enabled the quantifiers of their rotors;
//      shall not have dynamic collisions;
//      shall have stacked the actual positionsof their rotors.
//  The length of the searching interval dt1max shall be upper zero.
//Postconditions:
//  The RP will be in their initial status.
//  If searchSolutionInNegativeSense == false
//      solution p_1new will be nearest to
//      Max(RP->Actuator->theta_1first, theta_1 - dt1max).
bool TMotionProgramGenerator::searchSolutionInNegativeSense(double& p_1new, TRoboticPositioner *RP,
                                                            double dt1max)
{
    //CHECK THE PRECONDITIONS:

    if(dt1max <= 0)
        throw EImproperArgument("the length of the searching interval dt1max should be upper zero");

    if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
        throw EImproperCall("the RP shall be enabled the quantifier of their rotors");

    if(RP->getActuator()->thetasNotCoincideWithStacked())
        throw EImproperCall("the RP shall be stacked their actual position");

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
    TItemsList<TRoboticPositioner*> Adjacents = RP->getActuator()->Adjacents;

    //suppose that there is solution until the contrary is proved
    bool there_is_solution = true;

    //initialize the index for adjacents
    int i = 0;

    //initialize the motion program
    proposeRecoveryProgram(RP);

    //search a solution with an adjacent every time
    while(there_is_solution && i<Adjacents.getCount()) {
        //point the next adjacent RP single in the list of adjacents
        TRoboticPositioner *RPA = Adjacents[i];
        RP->getActuator()->Adjacents.Clear();
        RP->getActuator()->Adjacents.Add(RPA);

        //determine if the MP is valid (with the single adjacent)
        there_is_solution = motionProgramsAreValid(RP->MPturn, RP->MPretraction);
        //restablish the initial position of the RP
///        RP->getActuator()->Restorethetas();

        //if the MP is valid pass to the next adjacent
        if(there_is_solution)
            i++;

        //if the MP is invalid,  perform the binary search of the p_1new (with the single adjacent)
        else {
            //restablish the initial status of the FMM
///            getFiberMOSModel()->RPL.RestoreAndPopQuantifys();

            //perform the binary search of the p_1new (with the single adjacent)
            do {
                //calculates the new jumping position
                p_1new = round((p_1lower + p_1upper)/2);

                //propose the program for the new jumping position
                proposeRecoveryProgram(RP, p_1new);

                //determine if the MP is valid (with the single adjacent)
                there_is_solution = motionProgramsAreValid(RP->MPturn, RP->MPretraction);
                //restablish the initial position of the RP
///                RP->getActuator()->Restorethetas();

                //adjust the new searching interval
                if(there_is_solution)
                    //displace the lower limit
                    p_1lower = p_1new;

                else {
                    //restablish the initial status of the FMM
///                    getFiberMOSModel()->RPL.RestoreAndPopQuantifys();

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
    RP->getActuator()->Adjacents = Adjacents;

    //indicates if there is a solution or not
    return there_is_solution;
}

//Search in positive sense, the individual MP for recover the security
//position of a RP avoiding dynamic collisions.
//Inputs:
//  RP: robotic positioner to be recovered.
//  dt1max: maximun displacement of the rotor 1 for search the solution
//      in rad.
//Outputs:
//  searchSolutionInPositiveSense: indicates if the solution is valid.
//  p_1new: last position where solution has been searched.
//  (RP->MPturn, RP->MPretraction): the solution.
//Preconditions:
//  All RPs of the Fiber MOS Model shall be configurated for MP generation.
//  The pointer RP shall shall point to built robotic positioner.
//  The RP:
//      shall be in the Fiber MOS  Model;
//      shall be in unsecurity position;
//      shall be enabled the quantifiers of their rotors;
//      shall not have dynamic collisions;
//      shall have stacked the actual positionsof their rotors.
//  The length of the searching interval dt1max shall be upper zero.
//Postconditions:
//  The RP will be in their initial status.
//  If searchSolutionInPositiveSense == false
//      solution p_1new will be nearest to
//      Max(RP->Actuator->theta_1last, theta_1 + dt1max).
bool TMotionProgramGenerator::searchSolutionInPositiveSense(double& p_1new, TRoboticPositioner *RP,
                                                            double dt1max)
{
    //CHECK THE PRECONDITIONS:

    if(dt1max <= 0)
        throw EImproperArgument("the length of the searching interval dt1max should be upper zero");

    if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
        throw EImproperCall("the RP shall be enabled the quantifier of their rotors");

    if(RP->getActuator()->thetasNotCoincideWithStacked())
        throw EImproperCall("the RP shall be stacked their actual position");

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
    TItemsList<TRoboticPositioner*> Adjacents = RP->getActuator()->Adjacents;

    //suppose that there is solution until the contrary is proved
    bool there_is_solution = true;

    //initialize the index for adjacents
    int i = 0;

    //initialize the motion program
    proposeRecoveryProgram(RP);

    //search a solution with an adjacent every time
    while(there_is_solution && i<Adjacents.getCount()) {
        //point the next RP single in the list of adjacents
        TRoboticPositioner *RP_ = Adjacents[i];
        RP->getActuator()->Adjacents.Clear();
        RP->getActuator()->Adjacents.Add(RP_);

        //determine if the MP is valid (with the single adjacent)
        there_is_solution = motionProgramsAreValid(RP->MPturn, RP->MPretraction);
        //restablish the initial position of the RP
///        RP->getActuator()->Restorethetas();

        //if the MP is valid pass to the next adjacent
        if(there_is_solution)
            i++;

        //if the MP is invalid,  perform the binary search of the p_1new (with the single adjacent)
        else {
            //restablish the initial status of the FMM
///            getFiberMOSModel()->RPL.RestoreAndPopQuantifys();

            //perform the binary search of the p_1new (with the single adjacent)
            do {
                //calculates the new jumping position
                p_1new = round((p_1lower + p_1upper)/2);

                //propose the program for the new jumping position
                proposeRecoveryProgram(RP, p_1new);

                //determine if the MP is valid (with the single adjacent)
                there_is_solution = motionProgramsAreValid(RP->MPturn, RP->MPretraction);
                //restablish the initial position of the RP
///                RP->getActuator()->Restorethetas();

                //adjust the new searching interval
                if(there_is_solution)
                    //displace the upper limit
                    p_1upper = p_1new;

                else {
                    //restablish the initial status of the FMM
///                    getFiberMOSModel()->RPL.RestoreAndPopQuantifys();

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
    RP->getActuator()->Adjacents = Adjacents;

    //indicates if there is a solution or not
    return there_is_solution;
}
//Search the best recoveery program for a RP.
//Inputs:
//  RP: the RP to be recovered.
//Outputs:
//  searchSolution: indicates if the solution is valid.
//  RP->MPturn: the MP for turn the rotor 1.
//  RP->MPretraction: the MP for  retract the arm.
//  RP->Dsec: additional security distance during retraction.
//Preconditions:
//  The pointer RP shall shall point to built robotic positioner.
//  All RPs of the Fiber MOS Model shall be configurated for MP generation.
//  The RP:
//      shall be in the Fiber MOS  Model;
//      shall be in unsecurity position;
//      shall be enabled the quantifiers of their rotors;
//      shall not have dynamic collisions;
//      shall have stacked the actual positionsof their rotors.
//Postconditions:
//  The RP will be in their initial status, except the properties
//  (MPturn, MPretraction, Dmin) which will contains the last proposal.
bool TMotionProgramGenerator::searchSolution(TRoboticPositioner *RP)
{
    //CHECK THE PRECONDITIONS:

    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to  built RP");

    for(int i=0; i<getFiberMOSModel()->RPL.getCount(); i++) {
        TRoboticPositioner *RP = getFiberMOSModel()->RPL[i];
        if(RP->getActuator()->getPurpose() != pGen)
            throw EImproperCall("All RPs of the Fiber MOS Model should be configurated to MP generation.");
    }

    int i = getFiberMOSModel()->RPL.Search(RP);
    if(i >= getFiberMOSModel()->RPL.getCount())
        throw EImproperArgument("the RP should be in the Fiber MOS Model");

    if(RP->getActuator()->ArmIsInSafeArea())
        throw EImproperArgument("the RP should be in unsecurity position");

    if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
        throw EImproperArgument("the RP should be enabled the quantifiers of their rotors");

    if(RP->getActuator()->ThereIsCollisionWithAdjacent())
        throw EImproperArgument("the RP should not have dynamic collisions");

    if(RP->getActuator()->thetasNotCoincideWithStacked())
        throw EImproperArgument("the RP should have stacked the actual positionsof their rotors");

    //MAKE ACTIONS:

    //search a solution in negative sense
    double p_1new;
    bool valid = searchSolutionInNegativeSense(p_1new, RP, M_PI/2);

    //The funtion searchSolutionInNegativeSense not change the status of the RP.

    bool RP_is_recoverable;

    //if has found a solution
    if(valid)
        //indicates that the RP is recoverable
        RP_is_recoverable = true;

    //if not has found a solution
    else {
        //search a solution in positive sense
        valid = searchSolutionInPositiveSense(p_1new, RP, M_PI/2);

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
//  All RPs of the Fiber MOS Model shall be configurated for MP generation.
//  All RPs of DDS:
//      shall be in the Fiber MOS  Model;
//      shall be in unsecurity position;
//      shall be enabled the quantifiers of their rotors;
//      shall not have dynamic collisions;
//      shall have stacked the actual positionsof their rotors.
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
        if(RP->getActuator()->getPurpose() != pGen)
            throw EImproperCall("All RPs of the Fiber MOS Model should be configurated to MP generation.");
    }

    //  All RPs of DDS:
    //  shall be in the Fiber MOS  Model;
    //  shall be in unsecurity position;
    //  shall be enabled the quantifiers of their rotors.
    //  shall not have dynamic collisions.
    //  shall have stacked the actual positionsof their rotors.
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
                if(RP->getActuator()->ThereIsCollisionWithAdjacent())
                    throw EImproperArgument("all RPs of DDS should not have dynamic collisions");
                if(RP->getActuator()->thetasNotCoincideWithStacked())
                    throw EImproperArgument("all RPs of DDS should have stacked the actual positionsof their rotors");
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
        TPointersList<TRoboticPositionerList> *DisjointSet_ = new TPointersList<TRoboticPositionerList>(Max(1, DisjointSet->getCount()));
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

                //determines if there is solutionwith the minimun Dsec
                RP->setDsec(0);
                bool RP_is_recoverable = searchSolution(RP);

                //if there is solution, search the best solution
                if(RP_is_recoverable) {
                    //determines if there is solution with the maximun Dsec
                    RP->setDsec(getMSD());
                    RP_is_recoverable = searchSolution(RP);

                    //ifwith themacimun Dsec there is'nt solution, perform a binary search
                    if(!RP_is_recoverable) {
                        //initiañize the searching interval
                        double DsecMin = 0;
                        double DsecMax = getMSD();

                        //previously valid MP
                        TMotionProgram PrevMPturn;
                        TMotionProgram PrevMPretraction;

                        do {
                            //calculates the point of jump
                            double Dsec = (DsecMax + DsecMin)/2;

                            //set the value in the RP
                            RP->setDsec(Dsec);

                            //save the actual MP if it is valid
                            if(RP_is_recoverable) {
                                PrevMPturn = RP->MPturn;
                                PrevMPretraction = RP->MPretraction;
                            }

                            //search a solution
                            RP_is_recoverable = searchSolution(RP);

                            //actualize the searching interval
                            if(RP_is_recoverable)
                                DsecMin = Dsec;
                            else
                                DsecMax = Dsec;

                        } while(!RP_is_recoverable || RP->MPturn!=PrevMPturn || RP->MPretraction!=PrevMPretraction);
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

    //erase the empty disperse subset in RecoverablesDDS
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

    //erase the empty disperse subset from UnrecoverablesDDS
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

//Add to a MP, the corresponding list or lists of message instruction
//correspondint to the individual MPs of the RPs of a RPlist
//Inputs:
//  RPsToBeRetracted: list of the RPs which has been programmed.
//  MP: motion program to add themessage list.
//Outputs:
//  MP: motion program modified.
//Preconditions:
//  All RPs of the list RPsToBeRetracted shall have stacked the initial
//      positions of their rotors.
//  All RPs of the list RPsToBeRetracted shall be in their stacked positions.
//  All RPs of the list RPsToBeRetracted shall have a recovery program.
void TMotionProgramGenerator::addMessageLists(TMotionProgram& MP,
                     const TRoboticPositionerList& RPsToBeRecovered)
{
    //CHECK THE PRECONDITIONS:

    //Preconditions:
    //  All RPs of the list RPsToBeRetracted shall have stacked the initial
    //      positions of their rotors.
    //  All RPs of the list RPsToBeRetracted shall be in their stacked positions.
    //  All RPs of the list RPsToBeRetracted shall have a recovery program.
    for(int i=0; i<RPsToBeRecovered.getCount(); i++) {
        TRoboticPositioner *RP = RPsToBeRecovered[i];
        if(RP->getActuator()->theta_1s.getCount()<=0 || RP->getActuator()->getArm()->theta___3s.getCount()<=0)
            throw EImproperArgument("all RPs of the list RPsToBeRetracted shall have stacked the initial positions of their rotors");
        if(RP->getActuator()->thetasNotCoincideWithStacked())
            throw EImproperArgument("all RPs of the list RPsToBeRetracted shall be in their stacked positions");
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

                //delete the ML of the individual MP
                if(MPind->getCount()>0)
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
//of the list Inners to the origins
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
//BUILDING AND DESTROYING METHODS:

//build a motion program generator
//attached to a robotic positionerlist
TMotionProgramGenerator::TMotionProgramGenerator(TFiberMOSModel *_FiberMOSModel) :
    TMotionProgramValidator(_FiberMOSModel),
    TAllocationList(&(_FiberMOSModel->RPL)),
    p_MSD(1),
    NRmin(3), NBmin(1), PrMax(0)
{
}
//destroy the targetpoints and destroy the MPG
TMotionProgramGenerator::~TMotionProgramGenerator(void)
{
}

//---------------------------------------------------------------------------
//METHODS TO GENERATE MPs:

//Generates a recovery program for a given set of operative RPs
//in insecurity positions and determines the RPs of the given set,
//which can not be recovered because are in collision status
//or because are obstructed in insecurity positions.
//Preconditions:
//  All RPs of the Fiber MOS Model, shall be in their initial positions.
//  All RPs of the list Outsiders:
//      shall be in the Fiber MOS Model;
//      shall be operatives;
//      shall be in insecurity positions;
//      shall have enabled the quantifiers of their rotors;
//      shall be setted so that the rotor 2 velocity is approximately
//          double than rotor 1 velocity.
//Postconditions:
//  All RPs of the FMM will be configured for MP validation.
//  When the generated recovery program isn't valid:
//      All RPs of the FMM:
//          will have disabled the quantifiers of their rotors;
//          will be in the first position where the collision was detected
//              during the validation process.
//  When the generated recovery program is valid (even the trivial case):
//      All RPs of the FMM:
//          will have enabled the quantifiers of their rotors;
//          will be in their final positions.
//Inputs:
//  FMM: Fiber MOS Model with RPs in their initial positions.
//  Outsiders: list of operative RPs in unsecurity positions which
//      we want recover the security positions.
//Outputs:
//  generateRecoveryProgram: flag indicating if the recovery program
//      generated with this function is valid.
//  Collided: list of RPs collided in insecurity position.
//  Obstructed: list of RPs obstructed in insecurity position.
//  MP: recovery program.
bool TMotionProgramGenerator::generateRecoveryProgram(TRoboticPositionerList& Collided,
    TRoboticPositionerList& Obstructed, TMotionProgram& MP,
    const TRoboticPositionerList& Outsiders)
{
    //CHECK THE PRECONDITIONS:

    //Preconditions:
    //  All RPs of the Fiber MOS Model, shall be in their initial positions. (It is not to be checked).
    //  All RPs of the list Outsiders:
    //      shall be in the Fiber MOS Model;
    //      shall be operatives;
    //      shall be in insecurity positions;
    //      shall have enabled the quantifiers of their rotors;
    //      shall be setted in order to the rotor 2 velocity
    //          is approximately double than rotor 1 velocity.
    for(int i=0; i<Outsiders.getCount(); i++) {
        TRoboticPositioner *RP = Outsiders[i];
        int j = getFiberMOSModel()->RPL.Search(RP);
        if(j >= getFiberMOSModel()->RPL.getCount())
            throw EImproperArgument("All RPs of the list Outsiders, shall be in the Fiber MOS Model.");
        if(!RP->getOperative())
            throw EImproperArgument("All RPs in the list Outsiders, shall be operatives.");
        if(RP->getActuator()->ArmIsInSafeArea())
            throw EImproperArgument("All RPs in the list Outsiders, shall be in insecurity positions.");
        if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
            throw EImproperArgument("All RPs in the list Outsiders, shall have enabled the quantifiers of their rotors.");
        if(Abs(RP->wmaxabs2()/RP->wmaxabs1() - 2) > 0.1)
            throw EImproperCall("all RPs in the list Outsiders, shall be setted in order to the rotor 2 velocity is approximately double than rotor 1 velocity");
    }

    //MAKE ACTIONS:

    //initialize the outputs
    Collided.Clear();
    Obstructed.Clear();
    MP.Clear();

    //build the list Outsiders_ to contains the pointers to
    //the RPs wich remain in insecurity positions
    TRoboticPositionerList Outsiders_;

    //configure the Fiber MOS Model for MP generation
    getFiberMOSModel()->RPL.SetPurpose(pGen);

    //segregates the RPs of the list Outsiders which are in collision status, in the list Collided
    Outsiders.segregateCollided(Collided, Outsiders_);

    //solve the trivial case when there aren't RPs of the list Outsiders in insecurity positions
    if(Outsiders.allRPsAreInSecurePosition()) {
        //configure the Fiber MOS Model for MP validation
        getFiberMOSModel()->RPL.SetPurpose(pVal);
        //indicates that has found a solution, and return the empty solution
        return true;
    }

    //Here all RPs are in their initial positions.

    //stacks the initial positions of all RPs
    getFiberMOSModel()->RPL.PushPositions();

    //Since the initial position must be recovered repeteadly,
    //the initial positions must be stored.

    //Note that here it is not necessary disable the quantifier, because
    //the quantifiers will be disabled only in two places:
    //  1. During segregation of recoverables, for simulate the movement
    //     of each RP from their initial position to their final position.
    //  2. During validation of the generated recovery program, for simmulate
    //     the jointly movement of the RPs which must be move together.
    //If the recovery progrannotpass the validation process, the quantifiers
    //remains disabled, and their original status stacked.

    //=======================================================================
    //ITERATES THE GENERATION OF RECOVERY STEPS, USING A COPY OF Outsiders,
    //(Outsiders_), WHILE ARE ACCOMPLISH THE CONDITIONS:
    //  1. In the list Outsiders_ remains RPs to be recovered.
    //  2. Has been recovered some RP in the last iteration.

    //build the list Outsiders_ to contains the pointers to
    //the RPs wich remain in insecurity positions
//    TRoboticPositionerList Outsiders_(Outsiders);

    //The list Outsiders_ will contains all RPs to be retracted each time.
    //Each time can be only retracted some RPs of the list Outsiders_.
    //The process shall be reiterated until deplete the list.
    //The list Outsiders_ will start being a copy of the list Outsiders,
    //which will contains the set of all RPs in insecurity positions,
    //which we want recover the security position of most of them.
    //Only the obstructed RPs will remain in the list Outsiders_.

    //The proces of recovery consist in the iteration of recovery steps.
    //Each recovery step can be composed by one,two or three steps:
    //1. Turn of rotor 1 of the RPs which can't be directly retracted.
    //2. Retraction of the RPs.
    //3. Abatementof the arms (or turn of the rotor 2) of the RPs whose
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

        //Here all RPs of RecoverablesDDS will be in their initial positions,
        //and we want retract the RPs in the larger disperse subsets.

        //-------------------------------------------------------------------
        //SEGREGATES THE RPs TO BE RECOVERED:

        //WARNING: if recicle the RPsToBeRecovered, would need to make here:
        //  RPsToBeRecovered.Clear();

        //build the list of RPs to be retracted in each step
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
                         (DisperseSubset->DsecMin() > DisperseSubsetMax->DsecMin()) ||
                         ((DisperseSubset->DsecMin() == DisperseSubsetMax->DsecMin()) && (DisperseSubset->getCount() > DisperseSubsetMax->getCount())) ))
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
        RPsToBeRecovered.Compare = TRoboticPositioner::CompareIds;
        RPsToBeRecovered.SortInc();

        //Sort the RPs isn't really necessary, but is recomendable because produce a more legible output.

        //-------------------------------------------------------------------
        //GET THE MESSAGE-INSTRUCTION LIST CORRESPONDING TO THE RPsToBeRecovered.
        //MOVE THE RPs TO BE RECOVEREDTOTHEIRFINAL  POSITIONS.
        //DELETEDTHE RECOVERED RPs FROM THE LIST Outsiders_:

        //if there is some RP to be retracted
        if(RPsToBeRecovered.getCount() > 0) {
            //add to the MP, the corresponding list or lists of message of instructions
            addMessageLists(MP, RPsToBeRecovered);

            //move the RPs to be retracted to their final positions
            RPsToBeRecovered.MoveFin();

            //WARNING: RPs to be retracted shall be in their final positions
            //for avoid inter ferences with the next iterationand to get the
            //message-instruction list to turn the rotors 1.

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

        //determines if there is some RPs to berecovered in the list Outsiders_
        condition1 = (Outsiders_.getCount() > 0);
        //determines if has recovered some RP in the last iteration
        condition2 = (RPsToBeRecovered.getCount() > 0);

    //while are accomplish the two conditions
    } while(condition1 && condition2);

    //=======================================================================

    //indicates in the obstructed list, the operative RPs which remain in
    //unsequrity positions and are in the Outsiders list
    Obstructed = Outsiders_;

    //restore and discard the initial positions
    getFiberMOSModel()->RPL.RestoreAndPopPositions();

    //configure the Fiber MOS Model for MP validation
    getFiberMOSModel()->RPL.SetPurpose(pVal);

    //determines if the generated MP is valid
    bool valid = validateMotionProgram(MP);

    //WARNING: here all RPs retracted must be in security positions,
    //to allow add a message list to go to the origins.

    //indicates if the result of the validation process
    return valid;
}

//Generates a depositioning program for a given set of operative RPs
//in insecurity positions and determines the RPs of the given set,
//which can not be recovered because are in collision status
//or because are obstructed in insecurity positions.
//Preconditions:
//  All RPs of the Fiber MOS Model, shall be in their initial positions.
//  All RPs of the list Outsiders, shall be in the Fiber MOS Model.
//  All RPs of the list Outsiders, shall be operatives.
//  All RPs of the list Outsiders, shall be in insecurity positions.
//  All RPs of the list Outsiders, shall have enabled the quantifiers.
//  All RPs of the list Outsiders, shall have a rotor 2 velocity
//  approximately double that rotor 1 velocity.
//Postconditions:
//  All RPs of the Fiber MOS Model will be configured for MP validation
//  All RPs of the fiber MOS Model will be in their final positions,
//  or the first position where the collision was detected.
//  All RPs of the Fiber MOS Model will have disabled the quantifiers.
//Inputs:
//  FiberMOSModel: Fiber MOS Model with RPs in their initial positions.
//  Outsiders: list of operative RPs in unsecurity positions which
//      we want recover the security positions.
//Outputs:
//  generateParkingProgram: flag indicating if the parking program
//      can be generated or not with this function.
//  Collided: list of RPs collided in insecurity position.
//  Obstructed: list of RPs obstructed in insecurity position.
//  DP: depositioning program.
bool TMotionProgramGenerator::generateDepositioningProgram(TRoboticPositionerList& Collided,
    TRoboticPositionerList& Obstructed, TMotionProgram& DP,
    const TRoboticPositionerList& Outsiders)
{
    //attempt to generates the recovery program
    bool valid = generateRecoveryProgram(Collided, Obstructed, DP, Outsiders);

    //Here all RPs included in the DP, are in security position, out the origin.

    //if the recovery program was successfully generated
    if(valid) {
        //Here all operative outsiders RPs which aren't obstructed are in secure position,
        //in their final position after execute the DP.

        //segregate the operative inners RPs out of the origin and sorts it
        TRoboticPositionerList Inners;
        getFiberMOSModel()->RPL.segregateOperativeInnersOutTheOrigins(Inners);
        Inners.SortInc();

        //Sort the RPs isn't really necessary, but is recomendable because produce a more legible output.

        //generates the parking gesture for the operative RPs in security position out the origin
        addMessageListToGoToTheOrigins(DP, Inners);

        //move the segregated RPs to the origin
        Inners.MoveToOrigins();

        //Here all RPs included in the DP, are in the origin.
    }

    return valid;
}

//Generates a positioning program from a given depositioning program.
void TMotionProgramGenerator::generatePositioningProgram(TMotionProgram& PP,
    const TMotionProgram& DP, const TPairPositionAnglesList& OPL)
{
    //CHECK THE PRECONDITIONS:

    //shall have a PPA for each RP of the Fiber MOS Model
    for(int i=0; i<OPL.getCount(); i++) {
        int Id = OPL[i].getId();
        int j = getFiberMOSModel()->RPL.SearchId(Id);
        if(j >= getFiberMOSModel()->RPL.getCount())
            throw EImproperArgument("shall be a PPA for each RP of the Fiber MOS Model");
    }

    //all RP sincluded in the DP shall be in the Fiber MOS Model
    for(int i=0; i<DP.getCount(); i++) {
        const TMessageList *ML = DP.GetPointer(i);
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);
            int k= getFiberMOSModel()->RPL.SearchId(MI->getId());
            if(k >= getFiberMOSModel()->RPL.getCount())
                throw EImproperArgument("all RP sincluded in the DP shall be inthe Fiber MOS Model");
        }
    }

    //MAKE ACTIONS:

    //initialize the output
    PP.Clear();

    //for each message list,move the RPs to the final position (of the gessture),
    //and then get and insert the corresponding message list for the PP:

    //initialize initial position list and final position list
    TPairPositionAnglesList initialPositions;
    TPairPositionAnglesList finalPositions(OPL);

    //for each message list of the DP
    for(int i=0; i<DP.getCount(); i++) {
        const TMessageList *ML = DP.GetPointer(i);

        //actualice initial positions
        initialPositions = finalPositions;

        //build the inverse message list for contains the inverse gesture
        TMessageList *IML = new TMessageList();

        //for each message instruction build and add the corresponding inverse message instruction
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);
            int Id = MI->getId();

            //search the initial and final positions corresponding to the message
            int k = finalPositions.SearchId(Id);
            if(k >= finalPositions.getCount())
                throw EImpossibleError("lateral effect");
            TPairPositionAngles *PPAini = initialPositions.GetPointer(k);
            TPairPositionAngles *PPAfin = finalPositions.GetPointer(k);

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
            IMI->setComment(MI->getComment());

            //add the inverse message instruction to the inverse message list
            IML->Add(IMI);
        }

        //insert the invert message list in the PP
        PP.Insert(0, IML);
    }
}

//Generates a pair (PP, DP) for a given set of operative RPs
//in insecurity positions and determines the RPs of the given set,
//which can not be recovered because are in collision status
//or because are obstructed in insecurity positions.
//Preconditions:
//  All RPs of the Fiber MOS Model, shall be in their observing positions.
//  All RPs of the list Outsiders, shall be in the Fiber MOS Model.
//  All RPs of the list Outsiders, shall be operatives.
//  All RPs of the list Outsiders, shall be in insecurity positions.
//  All RPs of the list Outsiders, shall have enabled the quantifiers.
//Postconditions:
//  All RPs of the Fiber MOS Model will be configured for MP validation
//  All RPs of the fiber MOS Model will be in their final positions,
//  or the first position where the collision was detected.
//  All RPs of the Fiber MOS Model will have disabled the quantifiers.
//Inputs:
//  FiberMOSModel: Fiber MOS Model with RPs in their observing positions.
//  Outsiders: list of operative RPs in unsecurity positions which
//      we want recover the security positions.
//Outputs:
//  generateParkingProgram: flag indicating if the pair (PP, DP)
//      can be generated or not with this function.
//  Collided: list of RPs collided in insecurity position.
//  Obstructed: list of RPs obstructed in insecurity position.
//  PP: positioning program.
//  DP: depositioning program.
bool TMotionProgramGenerator::generatePairPPDP(TRoboticPositionerList& Collided,
    TRoboticPositionerList& Obstructed, TMotionProgram& PP,
    TMotionProgram& DP, const TRoboticPositionerList& Outsiders)
{
    //captures the observing positions of the RPs in a PPA list
    TPairPositionAnglesList OPL;
    getFiberMOSModel()->RPL.GetPositions(OPL);

    //attempt to generates the depositioning program
    bool success = generateDepositioningProgram(Collided, Obstructed, DP, Outsiders);

    //Here the RPs of the list Outsiders are in the origin, less the RPs
    //in collision or obstructed, which will be in their observing positions.

    //if has obtained a valid DP, generates the corresponding PP
    if(success) {
        //captures the initial positions of the RPs in a PPA list
        TPairPositionAnglesList IPL;
        getFiberMOSModel()->RPL.GetPositions(IPL);

        //generate the PP from the DP
        generatePositioningProgram(PP, DP, OPL);

        //validate the PP
        getFiberMOSModel()->RPL.SetPositions(IPL);
        success = validateMotionProgram(PP);

        //move the RPs to the initial positions
        getFiberMOSModel()->RPL.SetPositions(IPL);

        //check the poscondition
        if(!success)
            throw Exception("the PP generated from the DP is not valid");
    }

    //indicates the result of the generation process
    return success;
}

//---------------------------------------------------------------------------

//search the replacement RPs of a RP
//Inputs:
//  RP: the RP to be replaced.
//Outputs:(
//  RPRs: list of replacement RPs.
//Preconditions of the FMM:
//- All pointer of the Fiber MOS Model must point to built RPs.
//- All pointer of the fiber MOS Model must point to different RPs.
//- None operative RP of the Fiber MOS Model must has a dynamic fault.
//Preconditions of the RP:
//- Pointer RP must point to built robotic positioner.
//- The RP must be in the Fiber MOS Model.
//Preconditions of the TPL:
//- All RPs included in the TPL must be in the Fiber MOS Model.
//- All PPs included in the TPL must be in the scope of their allocated RP.
//- The RP must be included in the TPL.
//Postconditions:
//- If the list RPRs is not empty, the RP is a simple case,
//  and can be replaced by any of the replacement RPs.
//  In other case the RP is not a simple case.
void TMotionProgramGenerator::searchReplacementRPs(TRoboticPositionerList& RPRs,
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

    int i = getFiberMOSModel()->RPL.Search(RP);
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
        if(RP->getActuator()->PointIsOutDomainP3(PP.x, PP.y))
            throw EImproperArgument("all PPs included in the TPL must be in the scope of their allocated RP");
    }

    i = searchAllocation(RP);
    if(i >= getCount())
        throw EImproperArgument("the RP must be included in the TPL");

    //MAKE ACTIONS:

    //initialize the output
    RPRs.Clear(); //replacement RPs

    //search the replacement RPs
    for(i=0; i<RP->getActuator()->Adjacents.getCount(); i++) {
        TRoboticPositioner *RPR = RP->getActuator()->Adjacents[i];

        //Here the RPR is adjacent to the RP to be replaced.

        //determines if the PP allocated to the RP is in the scope of the RPR
        i = searchAllocation(RP);
        TAllocation *TP = Items[i];
        bool is_in_domain = RPR->getActuator()->PointIsInDomainP3(TP->PP);

        if(is_in_domain) {

            //Here the PP allocated to the RP, is in the scope of the RPR.

            if(RPR->getOperative()) {
                int j = searchAllocation(RPR);
                if(j >= getCount()) {

                    //Here the RPR is adjacent, operative and it is not allocated.

                    //check if the unsecurity area of the replacement RP is clear and will stay clear
                    bool clear = true;
                    int k = 0;
                    while(clear && k<RPR->getActuator()->Adjacents.getCount()) {
                        TRoboticPositioner *RPA = RPR->getActuator()->Adjacents[k];

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
                        //must be non invading the maneuvering domain of the RPR
                        else {
                            if(RPA->getActuator()->notInvadeManeuveringDomain(RPR->getActuator()))
                                k++;
                            else
                                clear = false;
                        }

                    }

                    //if the unsecurity area is clear and stay clear,add the RPR to the list
                    if(clear)
                        RPRs.Add(RPR);
                }
            }
        }
    }
}

//The data (MRmin, NBmin, PrMax) are the same for all points in the CB.
//The data All must be defined for each point.

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

/*    const TSPPP *SPPP = SPPPL.GetPointer(i);

    if(!SPPP->there_is_Mag || !SPPP->there_is_Pr || !SPPP->there_is_Bid || !SPPP->there_is_notAllocated || !SPPP->there_is_allocateInAll)
        throw EImproperArgument("the SPPP should have filled all fields");

    if(SPPP->Enabled != true)
        throw EImproperArgument("the PP should be allocated to the RP");
*/
    //MAKE ACTIONS:

    //point the indicated allocation for facilitate its access
    TAllocation *TP = Items[i];

    //check if the allocation corresponds to a reference source,
    //and the CB contains the minimun number of reference sources NRmin.
    unsigned int NR = countNR();
    if(TP->PP.Type==ptREFERENCE && NR<=NRmin)
        return true;

    //check if the allocation corresponds to a blank,
    //and the CB contains the minimun number of blanks NBmin.
    unsigned int NB = countNB();
    if(TP->PP.Type==ptBLANK && NB<=NBmin)
        return true;

    //check if if the allocation has a priority in [0, PrMax],
    //and it is not allocated in other CB.
    if(TP->PP.Priority <= PrMax/* && TP->notAllocated*/)
        return true;

/*    //check if has been indicated that the point must be allocated in all CBs
    if(TP->allocateInAll)
        return true;
*/
    //indicates that the allocation is not must type
    return false;
}

//Attempt regenerate a pair (PP, DP).
//Inputs:
//- (PP, DP): the pair to regenerate.
//Outputs:
//- attemptRegenerate: flag indicating if the pair (PP, DP)
//  has been regenerated.
//- (PP, DP): the regenerated pair, if any.
//- Excluded: list of identifiers of the excluded RPs, if Any.
//Preconditions:
//- All RPs included in the pair (PP, DP):
//  must be in the FMM;
//  must have an allocation in the MPG.
//- The status of the Fiber MOS Model must correspond to
//  the status of the real Fiber MOS.
//- The allocations shall contains the properties enough to
//  make the regeneration (Type, Pr, Pid, X, Y).
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
            throw EImproperArgument("all RPs included in the pair (PP,DP) must be in the Fiber MOS Model");

        j = searchAllocation(RP);
        if(j >= getCount())
            throw EImproperArgument("all RPs included in the pair (PP,DP) must have an allocation in the MPG");
    }

    //initialize the output
    Excluded.Clear();

    //determines if there is some point of must type allocated to a not operative RP
    for(int i=0; i<RPL.getCount(); i++) {
        TRoboticPositioner *RP = RPL[i];

        //if the allocation if must type
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
/*    //search the list of replacement RPs for each RP
    TPointersList<TRoboticPositionerList> RPRss;
    for(int i=0; i<RPL.getCount(); i++) {
        TRoboticPositioner *RP = RPL[i];

        TRoboticPositionerList *RPRs = new TRoboticPositionerList();
        searchReplacementRPs(RPRs, RP);
        if(RPRs.getCount() <= 0)
            return false;
        RPRss.Add(RPRs);
    }

    //attemp solve all not operative RPs included in the pair (PP, DP)
    for(int i=0; i<RPL.getCount(); i++) {
        TRoboticPositioner *RP = RPL[i];

        //Si se trata de un caso sencillo:
        //Asigna el punto al RP adyacente (más conveniente) satisfaciendo los requerimientos.
        //Si no se trata de un caso sencillo:
        //Si el punto asignado es de tipo must:
        //Indica que no es posible regenerar el par (PP, DP).

        //Si la posición actual es de seguridad:
        //borra la asignación del RP.
        //Si la posición actual es de inseguridad:
        //borra la asignación del RP y de todos los RPs adyacentes con PP asignado en posición de inseguridad.
    }

    return false;*/
}

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------

