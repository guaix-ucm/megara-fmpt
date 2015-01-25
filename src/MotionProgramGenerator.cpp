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

//#include <values.h>
#include <unistd.h>
//#include "Windows.h" //Sleep

//---------------------------------------------------------------------------

//spacename for positioning
namespace Positioning {

//###########################################################################
//TMotionProgramGenerator:
//###########################################################################

//---------------------------------------------------------------------------
//BUILDING AND DESTROYING METHODS:

//build a motion program generator
//attached to a robotic positionerlist
TMotionProgramGenerator::TMotionProgramGenerator(TFiberMOSModel *_FiberMOSModel) :
    TMotionProgramValidator(_FiberMOSModel),
    TTargetPointList(&(_FiberMOSModel->RPL)),
    __dt_jmp(-M_PI/36)
{
}
//destroy the targetpoints and destroy the MPG
TMotionProgramGenerator::~TMotionProgramGenerator(void)
{
}

//-----------------------------------------------------------------------
//METHODS TO GENERATE MPs:

//program the retraction of all RP of the list Outsiders
//Preconditions:
//  All RPs of the list Outsiders shall has enabled the quantifiers.
//  All RPs of the list Outsiders shall be in unsecure position.
void TMotionProgramGenerator::programRetraction(TRoboticPositionerList &Outsiders)
{
    //CHECK THE PRECONDITIONS:

    //all RPs of thelist Outsiders shall has enabled the quantifiers and shall be in unsecure position
    for(int i=0; i<Outsiders.getCount(); i++) {
        TRoboticPositioner *RP = Outsiders.Get(i);
        if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
            throw EImproperArgument("all positioners in the list Outsiders should has enabled the quantifiers");
        if(RP->getActuator()->ArmIsInSafeArea())
            throw EImproperArgument("all positioners in the list Outsiders should be in unsecure position");
    }


    //for each RP ofthe list Outsiders:
    for(int i=0; i<Outsiders.getCount(); i++) {
        //point the indicated RP tofacilitate its access
        TRoboticPositioner *RP = Outsiders.Get(i);
        //program the readial retraction
        RP->programRetractArmToSafeArea();
    }

    //WARNING: when rotor 1 is near of its lower limit,
    //not all trajectoryof the fiber may be radial.
}

//segregate the RPs of the list Outsiders, in disjoint subsets
void TMotionProgramGenerator::segregateRPsInDisjointSets(
        TPointersList<TRoboticPositionerList>& DisjointSets,
        const TRoboticPositionerList& Outsiders)
{
    //CHECK THE PRECONDITIONS:

    //all RPs in the list Outsiders, shall be operatives in unsecure positions
    for(int i=0; i<Outsiders.getCount(); i++) {
        //point the indicated RP to facilitate its access
        TRoboticPositioner *RP = Outsiders[i];
        //if the RP is not operative
        if(!RP->getOperative())
            //indicates thar all RPs in the list Outsiders shall be operatives
            throw EImproperArgument("all RPs in thelist Outsiders shall be operatives");
        //if the RP is in safe area
        if(RP->getActuator()->ArmIsInSafeArea())
            //indicates thar all RPs in the list Outsiders shall be in unsecure positions
            throw EImproperArgument("all RPs in thelist Outsiders shall be in unsecure positions");
    }

    //START:

    //initialize the list of disjoint sets
    DisjointSets.Clear();

    //The RPs in unsecure position, which not are in the list Outsiders,
    //shall be taken how obstacles, so that the sets separated only for
    //these RPs, are disjoint sets.

    //for each RP of the list Outsiders
    for(int i=0; i<Outsiders.getCount(); i++) {
        //points the indicated RP to facilitate its access
        TRoboticPositioner *RP = Outsiders[i];

        //determines the list of adjacent RPs which are in unsecure position
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
    //calculate the constants
    double D = 4*MEGARA_L*cos(M_PI/6); //distance between points P0
    double Sx = 3*D;        //base of the rectangle
    double Sy = 6*MEGARA_L; //height of the rectangle

    //all RPs of the disjoint subsets:
    //  shall be separated a distance D - 2*L or upper
    for(int i=1; i<Set.getCount(); i++) {
        TRoboticPositioner *RP = Set[i];

        for(int j=0; j<i; j++)
            if(Mod(Set[j]->getActuator()->getP0() - RP->getActuator()->getP0()) <= D - 2*MEGARA_L)
                throw EImproperArgument("all RPs of the disjoint set shall be separated a distance D - 2*L or upper");
    }

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

    //pointer DisperseSubsets should point to built list of disperse subsets
    if(DisperseSubsets == NULL)
        throw EImproperArgument("pointer DisperseSubsets should point to built list of disperse subsets");

    //all RPs shall follow the MEGARA distribution
    if(notAllRPsFollowMEGARADistribution(Set))
        throw EImproperArgument("all RPs should follow MEGARA distribution");

    //all RPs in the disjoint set shall be in the Fiber MOS Model
    for(int i=0; i<Set.getCount(); i++) {
        TRoboticPositioner *RP = Set[i];
        int j = getFiberMOSModel()->RPL.Search(RP->getActuator());
        if(j >= getFiberMOSModel()->RPL.getCount())
            throw EImproperArgument("all RPs in the disjoint set shall be in the Fiber MOS Model");
    }

    //START:

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
void TMotionProgramGenerator::determinesCollisionInterval(double& l1, double& l2,
        TRoboticPositioner *RP, const TRoboticPositioner *RPA)
{
    //CHECK THEPRECONDITIONS:

    //pointer RP shall points to built robotic positioner
    if(RP == NULL)
            throw EImproperArgument("pointer RP should point to built robotic positioner");

    //pointer RPA shall points to built robotic positioner
    if(RPA == NULL)
            throw EImproperArgument("pointer RPA should point to built robotic positioner");

    //MAKE THE ACTIONS:

    //save the initial position of rotor 1 of the RP
    double p_1bak = RP->getActuator()->getp_1();

    //get the interval of search
    double p_1min = ceil(RP->getActuator()->getp_1min());
    double p_1max = floor(RP->getActuator()->getp_1max());

    //initialice p_1
    double p_1 = p_1min;
    //move the rotor 1 to the first position
    RP->getActuator()->setp_1(p_1);
    //calculates the distance between arms
    double D = RP->getActuator()->getArm()->getContour().DistanceMin(RPA->getActuator()->getArm()->getContour());
    //calculates the free distance
    double Df = D - RP->getActuator()->getArm()->getSPM() - RPA->getActuator()->getArm()->getSPM();
    //initiaize Dfmin
    double Dfmin = Df;
    //initialize p_1_
    double p_1_ = p_1;
    //initialize the collision flag
    bool collision = (Df < 0);

    //search in the interval
    for(p_1=round(p_1min+50); p_1<=p_1max; p_1+=50, p_1=round(p_1)) {
        //capture the status of the cllision flag
        bool collisionbak = collision;

        //set the p_1
        RP->getActuator()->setp_1(p_1);

        //calculates the distance between arms
        D = RP->getActuator()->getArm()->getContour().DistanceMin(RPA->getActuator()->getArm()->getContour());
        //calculates the free distance
        Df = D - RP->getActuator()->getArm()->getSPM() - RPA->getActuator()->getArm()->getSPM();
        //actualice the collision flag
        collision = (Df < 0);

        //actualice the minimun free distance
        if(Df < Dfmin) {
                Dfmin = Df;
                p_1_ = p_1;
        }

        if(collision != collisionbak) {
            if(collision)
                l1 = p_1 - 50;
            else
                l2 = p_1;
        }
    }

    //if there isn't collision
    if(Dfmin >= 0) {
        l1 = p_1_;
        l2 = p_1_;
    }

    //retore the initial position of rotor 1
    RP->getActuator()->setp_1(p_1bak);

    //ERROR: this function is a temporalsolution while a binary searhc is implemented.
}

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
bool TMotionProgramGenerator::collisionCanBesolved(double& p_1new, TRoboticPositioner *RP)
{
    //CHECK THE PRECONDITIONS:

    //pointer RP shall point to built robotic positioner
    if(RP == NULL)
        throw EImproperArgument("pointer should point to built robotic positioner");

    //the RP shall be in collision status
    if(RP->getActuator()->ThereIsntCollisionWithAdjacent())
        throw EImproperArgument("the RP should be in collision status");

    //the RP shall has enabled their rotor 1 quantifier
    if(!RP->getActuator()->getQuantify_())
        throw EImproperArgument("the RP should has enabled their rotor 1 quantifier");

    //The quantifier of the rotor 1 shall be enabled because the solution shall be stable.

    //The rotor 1 of the RP shall has stascked one position almost
    if(RP->getActuator()->theta_1s.getCount() < 1)
        throw EImproperCall("the rotor 1 of the RP shall has stacked one position almost");

    //Therotor 1 of the RP shall be stacked a positions to determines
    //if there is a previous displacement of their rotor 1.

    //MAKE THE ACTION:

    //capture actual p_1 before search the collision intervals
    double p_1 = RP->getActuator()->getp_1();

    //determines the limits of the collision interval of the RP for each adjacent
    TVector<double> l1s, l2s;
    for(int i=0; i<RP->getActuator()->Adjacents.getCount(); i++) {
        TRoboticPositioner *RPA = RP->getActuator()->Adjacents[i];
        double l1,l2;
        determinesCollisionInterval(l1, l2, RP, RPA);
        l1s.Add(l1);
        l2s.Add(l2);
    }

    //When l1 == l2 there isn't collision. (In this casemoreover l1 == p1 ==l2).
    //When l1 < l2, the soltion can be searched in both senses.
    //When l2 < l1, the solution shall be searched:
    //  in positive sense, when p_1 is in [p_1min, l2)
    //  in negative sense, when p_1 is in (l1, p_1max]

    //search the largest interval where p_1 is inner
    int imax = 0;
    double lengthmax = 0;
    for(int i=0; i<l1s.getCount(); i++) {
        //get the limits of the intervals
        double l1 = l1s[i];
        double l2 = l2s[i];
        double p_1min = ceil(RP->getActuator()->getp_1min());
        double p_1max = floor(RP->getActuator()->getp_1max());

        //calculates the length of the interval
        //and determines the collision status
        double length;
        bool collision;
        if(l1 < l2) {
            length = l2 - l1;
            if(p_1<=l1 || l2<=p_1)
                collision = false;
            else
                collision = true;
        } else if(l2 < l1) {
            length = (l2 - p_1min) + (p_1max - l1);
            if(p_1<l2 || l1<p_1)
                collision = true;
            else
                collision = false;
        } else {
            length = 0;
            collision = false;
        }

        //if there is collision actualice (imax, lengthmax)
        if(collision)
            if(length > lengthmax) {
                imax = i;
                lengthmax = length;
            }
    }

    //determines the new position for p_1, to solve the collision with the largest interval
    double l1 = l1s[imax];
    double l2 = l2s[imax];
    double s1, s2;
    if(l1 < l2) {
        s1 = p_1 - l1;
        s2 = l2 - p_1;
        if(s1 <= s2)    //note that negative sense if preferred
            p_1new = l1;
        else
            p_1new = l2;
    } else if(l2 < l1) {
        if(p_1 < l2) {
            s1 = MAXDOUBLE;
            s2 = l2 - p_1;
            p_1new = l2;
        } else if(l1 < p_1) {
            s1 = p_1 - l1;
            s2 = MAXDOUBLE;
            p_1new = l1;
        }
    } else { //l1 == l2
        throw EImpossibleError("l1 shall not be equal to l2, becuase here is supossed that there is a collision to be solved");
    }

    //determines the displacementto escape ofthe collision
    double dp1_esc = p_1new - p_1;

    //determines the previous displacement
    double dp1_prev = RP->dp1();

    //if rotor 1 has been previously displaced in the opposed sense to escape
    if(dp1_prev != 0)
        if(Sign(dp1_esc) != Sign(dp1_prev))
            //indicates that collision can't be solved
            return false;

    //search the first position to avoid collision in the sense to escape
    //if displacement from p_1new envelopes more of middle interval (l1, l2)
    //  indicates that collision can't be solved
    for(int i=0; i<l1s.getCount(); i++) {
        //get the limits of the interval
        double l1 = l1s[i];
        double l2 = l2s[i];

        //and determines the collision status of p_1new
        bool collision;
        if(l1 < l2) {
            if(p_1new<=l1 || l2<=p_1new)
                collision = false;
            else
                collision = true;
        } else if(l2 < l1) {
            if(p_1new<l2 || l1<p_1new)
                collision = true;
            else
                collision = false;
        } else {
            collision = false;
        }

        //Here, if there is collision with the interval (l1, l2),
        //shall attemt solve the collision displacing p_1new.

        //if there is collision actualize p_1new in the sense stablished to the displacement
        //the previous value of p_1new shall be saved to compare the displacement impossed by the interval (l1, l2)
        if(collision) {
            //capture the previous value of p_1new
            double p_1new_prev = p_1new;

            //actualice p_1new and calculates the length of the interval (l1, l2)
            double length;
            if(l1 < l2) {
                length = l2 - l1;
                if(dp1_esc < 0) {
                    p_1new = l1;
                } else if(dp1_esc > 0) {
                    p_1new = l2;
                } else { //dp1_esc == 0
                    throw EImpossibleError("pd1_esc can't be equal to 0, because here is supossed that a collision shall be solved");
                }

            } else if(l2 < l1) {
                double p_1min = ceil(RP->getActuator()->getp_1min());
                double p_1max = floor(RP->getActuator()->getp_1max());
                length = (l2 - p_1min) + (p_1max - l1);
                if(p_1new < l2) {
                    if(dp1_esc < 0) {
                        p_1new = -MAXDOUBLE;
                    } else if(dp1_esc > 0) {
                        p_1new = l2;
                    } else { //dp1_esc == 0
                        throw EImpossibleError("pd1_esc can't be equal to 0, because here is supossed that a collision shall be solved");
                    }
                } else if(l1 < p_1new) {
                    if(dp1_esc < 0) {
                        p_1new = l1;
                    } else if(dp1_esc > 0) {
                        p_1new = MAXDOUBLE;
                    } else { //dp1_esc == 0
                        throw EImpossibleError("pd1_esc can't be equal to 0, because here is supossed that a collision shall be solved");
                    }
                }

            } else { //else, if(l1 == l2)
                throw EImpossibleError("l1 not should be equal to l2, because here is supossed that there is collision");
            }

            //if the displacement impossed by the interval (l1, l2) envelopes more of middle length
            //  indicates that collision can't be solved
            if(abs(p_1new - p_1new_prev) > length/2)
                //indicates that collision can't be solved
                return false;
        }
    } //for(int i=0; i<l1s.getCount(); i++)

    //indicates that collision can be solved
    return true;
}

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
//  motion program generation (Purpose =pGen).
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
void TMotionProgramGenerator::segregateRetractilesInvaders(
        TPointersList<TPointersList<TRoboticPositionerList> >& RetractilesDDS,
        TPointersList<TPointersList<TRoboticPositionerList> >& InvadersDDS,
        TPointersList<TPointersList<TRoboticPositionerList> >& DDS)
{
    //CHECK THE PRECONDITIONS:

    //All RPs of the Fiber MOS Model shall be configurated to motion program generation.
    for(int i=0; i<getFiberMOSModel()->RPL.getCount(); i++) {
        TRoboticPositioner *RP = getFiberMOSModel()->RPL[i];
        if(RP->getActuator()->getPurpose() != pGen)
            throw EImproperCall("All RPs of the Fiber MOS Model shall be configurated to motion program generation.");
    }

    //All RPs of DDS:
    //  shall be in the Fiber MOS  Model.
    //  shall be in insecure position.
    //  shall have enabled the quantifiers.
    //  shall be free of dynamic collisions.
    //  shall to have programmed a gesture of retraction.
    for(int i=0; i<DDS.getCount(); i++) {
        TPointersList<TRoboticPositionerList> *DisjointSet = DDS.GetPointer(i);
        for(int j=0; j<DisjointSet->getCount(); j++) {
            TRoboticPositionerList *DisperseSubset = DisjointSet->GetPointer(j);
            for(int k=0; k<DisperseSubset->getCount(); k++) {
                TRoboticPositioner *RP = DisperseSubset->Get(k);
                int l = getFiberMOSModel()->RPL.Search(RP);
                if(l >= getFiberMOSModel()->RPL.getCount())
                    throw EImproperArgument("All RPs of DDS shall be in the Fiber MOS Model.");
                if(RP->getActuator()->ThereIsCollisionWithAdjacent())
                    throw EImproperArgument("All RPs of DDS shall be free of dynamic collisions.");
                if(RP->getActuator()->ArmIsInSafeArea())
                    throw EImproperArgument("All RPs of DDS shall be in insecure position.");
                if(!RP->getActuator()->getQuantify_() || !RP->getActuator()->getArm()->getQuantify___())
                    throw EImproperArgument("All RPs of DDS shall have enabled the quantifiers.");
                if(RP->CMF.getMF1()==NULL || RP->CMF.getMF2()==NULL)
                    throw EImproperArgument("All RPs of DDS shall to have programmed a gesture of retraction.");
            }
        }
    }

    //INITIALIZE THE ESTRUCTURES:

    //build RetractilesDDS similar to DDS but empty
    RetractilesDDS.Clear();
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

        //add the DisjointSet_ to RetractilesDDS
        RetractilesDDS.Add(DisjointSet_);
    }

    //clone RetractilesDDS with InvadersDDS
    InvadersDDS.Clone(RetractilesDDS);

    //WARNING: do not clone the DDS, because TRoboticPositionerList::Clone
    //will clone the RPs, while TRoboticPositionerList::operator= will copy
    //the pointers to the RPs. for DDS use the method Copy.

    //Here all RPs of RetractilesDDS accomplish the preconditions:
    //  are configurated to motion program generation.
    //  are in the Fiber MOS  Model.
    //  are in insecure position.
    //  have the quantifiers enabled.
    //  are free of dynamic collisions.
    //  have programmed a gesture of retraction.

    //SIMULATES THE RADIAL RETRACTION SOLVING DYNAMIC COLLISIONS BY ROTOR 1 TURNS:

    //for each disjoin set
    for(int i=0; i<DDS.getCount(); i++) {
        //point the indicated disjoint set to facilitate its access
        TPointersList<TRoboticPositionerList> *DisjointSet = DDS.GetPointer(i);

        //for each disperse subset
        for(int j=0; j<DisjointSet->getCount(); j++) {
            //point the indicated disperse subset to facilitate its access
            TRoboticPositionerList *DisperseSubset = DisjointSet->GetPointer(j);

            //for each RP
            for(int k=0; k<DisperseSubset->getCount(); k++) {
                //point the indicated RP to facilitate its access
                TRoboticPositioner *RP = DisperseSubset->Get(k);

                //get the simulation period
                double T = RP->CMF.gettendmax();
                //calculates the minimun jump of the RP
                double Tminmin = calculateTminmin(RP);
                //initialize the simulation time
                double t = 0;

                do {
                    //calculates the minimun free time of the RP
                    double Tfmin = calculateTfmin(RP);
                    //limits underly the step time
                    double st = Max(Tfmin, Tminmin);
                    //calculates the new time
                    t += st;
                    //move the RP to the time of simulation
                    RP->Move(t);
                    //determines the collision status of the RP
                    bool collision = RP->getActuator()->ThereIsCollisionWithAdjacent();
                    //if has been detected collision
                    if(collision) {
                        //determines if the collision can be solved turning rotor 1
                        //in the sense stablished by the tendence
                        double p_1new;
                        bool itispossible = collisionCanBesolved(p_1new, RP);

                        //here p_1new shall be in the rotor 1 domain
                        if(RP->getActuator()->IsntInDomainp_1(p_1new))
                            throw EImpossibleError("p_1new shall be in the rotor 1 domain");

                        //if it is possible solve the collision
                        if(itispossible) {
                            //get the actual position angle of rotor 1
                            int p_1act = RP->getActuator()->getp_1();
                            //displace the rotor 1 to the new position angle
                            RP->getActuator()->setp_1(p_1new);

                            //calculates the limits of the new interval of displacement of rotor 1
                            double dp = double(p_1new - p_1act);
                            double psta = RP->CMF.getMF1()->getpsta() + dp;
                            double pfin = RP->CMF.getMF1()->getpfin() + dp;
                            //constrict the limits to the domain of the rotor
                            double p_1min = RP->getActuator()->getp_1min();
                            double p_1max = RP->getActuator()->getp_1max();
                            if(psta < p_1min)
                                psta = p_1min;
                            else if(psta > p_1max)
                                psta = p_1max;
                            if(pfin < p_1min)
                                pfin = p_1min;
                            else if(pfin > p_1max)
                                pfin = p_1max;
                            //set the limis of the new interval to themotion function of the rotor
                            RP->CMF.getMF1()->SetInterval(psta, pfin);

                            //The displacement of a motion function can produce
                            //that their limits go beyong the domain of its rotor.

                            //initialize the simulation
                            RP->MoveSta();
                            t = 0;

                            //if there is collision in the new starting position
                            if(RP->getActuator()->ThereIsCollisionWithAdjacent())
                                //indicates that isn't possible solve the collision
                                T = 0;
                        }
                        //if it isn't possible solve the collision
                        else {
                            //indicates that isn't possible solve the collision
                            T = 0;
                        }
                    }
                //while has not reached the end
                } while(t < T);

                //ERROR: determine the Tfmin is equivalent to determine the collision status,
                //reason why the block of code above, can be wrote more eficient.

                //if has finished the simulation period avoiding collision
                if(T > 0)
                    //add the RP to the indicated disperse subset of RetractilesDDS
                    RetractilesDDS.GetPointer(i)->GetPointer(j)->Add(RP);
                else //(T <= 0) has finished the simulation period because cannot avoid the collision
                    //add the RP to the indicated disperse subset of InvadersDDS
                    InvadersDDS.GetPointer(i)->GetPointer(j)->Add(RP);
            }
        }
    }

    //erase the empty disperse subset from RetractilesDDS
    int i = 0;
    while(i < RetractilesDDS.getCount()) {
        //point the indicated disjoint set to facilitate its access
        TPointersList<TRoboticPositionerList> *DisjointSet = RetractilesDDS.GetPointer(i);

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
            RetractilesDDS.Delete(i);
    }

    //erase the empty disperse subset from InvadersDDS
    i = 0;
    while(i < InvadersDDS.getCount()) {
        //point the indicated disjoint set to facilitate its access
        TPointersList<TRoboticPositionerList> *DisjointSet = InvadersDDS.GetPointer(i);

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
            InvadersDDS.Delete(i);
    }
}

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
void TMotionProgramGenerator::addMessageLists(TMotionProgram& DP,
        const TRoboticPositionerList& RPsToBeRetracted)
{
    //CHECK THE PRECONDITIONS:
    for(int i=0; i<RPsToBeRetracted.getCount(); i++) {
        TRoboticPositioner *RP = RPsToBeRetracted[i];
        if(RP->getActuator()->theta_1s.getCount()<=0 || RP->getActuator()->getArm()->theta___3s.getCount()<=0)
            throw EImproperArgument("all RPs of the list RPsToBeRetracted shall have stacked the initial positions of their rotors");
        if(RP->getActuator()->thetasNotCoincideWithStacked())
            throw EImproperArgument("all RPs of the list RPsToBeRetracted shall be in their stacked positions");
        if(RP->CMF.getMF1()==NULL && RP->CMF.getMF2()==NULL)
            throw EImproperArgument("all RPs of the list RPsToBeRetracted shall have programmed a gesture");
    }

    //segregates the RPs whose rotor 1 has been displaced
    TRoboticPositionerList DisplacedRPs;
    for(int i=0; i<RPsToBeRetracted.getCount(); i++) {
        TRoboticPositioner *RP = RPsToBeRetracted[i];
        if(RP->dp1() != 0)
            DisplacedRPs.Add(RP);
    }

    //if there is some RPs whose rotor 1 has been displaced
    if(DisplacedRPs.getCount() > 0) {
        //get the message list to produce the turns of the rotors 1
        TMessageList *ML = new TMessageList(Max(1, getFiberMOSModel()->RPL.getCount()));
        //for each RP of the list DisplacedRPs
        for(int i=0; i<DisplacedRPs.getCount(); i++) {
            //point the indicated RP to facilitate irs access
            TRoboticPositioner *RP = DisplacedRPs[i];

            //built a message
            TMessageInstruction *M = new TMessageInstruction();
            //configure the message
            M->setId(RP->getActuator()->getId());
            M->Instruction.setName(AnsiString("M1"));
            M->Instruction.Args.setCount(1);
            M->Instruction.Args[0] = RP->CMF.getMF1()->getpsta();

            //add the message to the message list
            ML->Add(M);
        }
        //add the message list to the DP
        DP.Add(ML);
    }

    //get the message instruction list to produce the retraction
    TMessageList *ML = new TMessageList(Max(1, getFiberMOSModel()->RPL.getCount()));
    //for each RP of the list RPsToBeRetracted
    for(int i=0; i<RPsToBeRetracted.getCount(); i++) {
        //point the indicated RP to facilitate its access
        TRoboticPositioner *RP = RPsToBeRetracted[i];

        //built a message of instruction
        TMessageInstruction *M = new TMessageInstruction();
        //configure the message
        M->setId(RP->getActuator()->getId());
        RP->getInstruction(M->Instruction);

        //add the message to the message list
        ML->Add(M);
    }
    //add the message list to the DP
    DP.Add(ML);
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
//  RP: recovery program.
bool TMotionProgramGenerator::generateRecoveryProgram(TRoboticPositionerList& Collided,
    TRoboticPositionerList& Obstructed, TMotionProgram& MP,
    const TRoboticPositionerList& Outsiders)
{
    //CHECK THE PRECONDITIONS:

    //All RPs of the list Outsiders, shall be in the Fiber MOS Model.
    //All RPs in the list Outsiders, shall be operatives.
    //All RPs in the list Outsiders, shall be in unsecure positions.
    //All RPs in the list Outsiders, shall have enabled the quantifiers.
    for(int i=0; i<Outsiders.getCount(); i++) {
        TRoboticPositioner *RP = Outsiders[i];
        int j = getFiberMOSModel()->RPL.Search(RP);
        if(j >= getFiberMOSModel()->RPL.getCount())
            throw EImproperArgument("All RPs of the list Outsiders, shall be in the Fiber MOS Model.");
        if(!RP->getOperative())
            throw EImproperArgument("All RPs in the list Outsiders, shall be operatives.");
        if(RP->getActuator()->ArmIsInSafeArea())
            throw EImproperArgument("All RPs in the list Outsiders, shall be in unsecure positions.");
        if(!RP->getActuator()->getQuantify_() || !RP->getActuator()->getArm()->getQuantify___())
            //indicates that all RPs in the list Outsiders shall have enabled the quantifiers
            throw EImproperArgument("All RPs in the list Outsiders, shall have enabled the quantifiers.");
    }

    //START:

    //configure the Fiber MOS Model for generation of motion programs
    getFiberMOSModel()->RPL.SetPurpose(pGen);

    //segregates the RPs of the list Outsiders which are in collision status, in the list Collided
    getFiberMOSModel()->RPL.segregateCollided(Collided);

    //initialize the outputs
    Collided.Clear();
    Obstructed.Clear();
    MP.Clear();

    //if there aren't RPs of the list Outsiders in unsecure positions
    if(Outsiders.allRPsAreInSecurePosition())
        //indicates that has found a solution, and return the empty solution
        return true;

    //Here all RPs are in their initial positions.

    //stack the initial positions of all RPs
    getFiberMOSModel()->RPL.PushPositions();

    //build the list Outsiders_ to contains the pointers to
    //the RPs wich remain in unsecure positions
    TRoboticPositionerList Outsiders_(Outsiders);
    //program the retraction for all RPs of the list Outsiders_
    programRetraction(Outsiders_);

    //The list Outsiders_ will contains all RPs to be retracted each time.
    //Each time can be only retracted some RPs of the list Outsiders_.
    //The process shall be reiterated until deplete the list.
    //The list Outsiders_ will start being a copy of the list Outsiders,
    //which will contains the set of all RPs in unsecure positions,
    //which is to recover the security position of most of them.
    //Only the obstructed RPs will remain in the list Outsiders_.

    //Each step can has be composed by one or two gestures:
    //  a gesture of retraction
    //  or a gesture of rot 1 turn, followed by retractionof all

    //ITERATES THE GENERATION OF STEPS UNTIL EXHAUST THE POSSIBILITIES
    //OF RETRACTION OF THE RPs OF THE LIST Outsiders_:

    bool aux1, aux2;

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

        //segregates the RPs which can be retracted in each subset of each set
        TPointersList<TPointersList<TRoboticPositionerList> > RetractilesDDS;
        TPointersList<TPointersList<TRoboticPositionerList> > InvadersDDS;
        segregateRetractilesInvaders(RetractilesDDS, InvadersDDS, DDS);

        //Here all RPs of RetractilesDDS will be in security positions,
        //but we want retract only the RPs in the dispersesubsets more large.

        //SEGREGATES THE RPs TO BE RETRACTED AND RESTORE THE STARTING POSITION
        //OF THE RPs OF RetractilesDDS:

        //WARNING: if recicle the RPsToBeRetracted, would need to make here:
        //  RPsToBeRetracted.Clear();

        //build the list of RPs to be retracted in each step
        TRoboticPositionerList RPsToBeRetracted;

        //for each disjoint set of retractiles RPs
        for(int i=0; i<RetractilesDDS.getCount(); i++) {
            //point the disjoint set indicated to facilitate its access
            TPointersList<TRoboticPositionerList> *DisjointSet = RetractilesDDS.GetPointer(i);

            //search the disperse subset which has the maximun number of RPs
            TRoboticPositionerList *DisperseSubsetMax = NULL;
            for(int j=0; j<DisjointSet->getCount(); j++) {
                TRoboticPositionerList *DisperseSubset = DisjointSet->GetPointer(j);

                //actualize the maximun disperse subset
                if(DisperseSubset->getCount()>0 &&
                        (DisperseSubsetMax==NULL || (DisperseSubset->getCount() > DisperseSubsetMax->getCount())))
                    DisperseSubsetMax = DisperseSubset;

                //restore the initial position of the RPs of the disperse subset
                DisperseSubset->RestorePositions();
            }

            //add the RPs of the subset to the list RPsToBeRetracted
            if(DisperseSubsetMax != NULL)
                for(int j=0; j<DisperseSubsetMax->getCount(); j++) {
                    TRoboticPositioner *RP = DisperseSubsetMax->Get(j);
                    RPsToBeRetracted.Add(RP);
                }
        }
        //sort the list RPsToBeRetracted
        RPsToBeRetracted.Compare = TRoboticPositioner::CompareIds;
        RPsToBeRetracted.SortInc();

        //Sort the RPs isn't really necessary, but is recomendable because produce a more legible output.

        //GET THE MESSAGE-INSTRUCTION LIST CORRESPONDING TO THE RPsToBeRetracted:

        //if there is some RP to be retracted
        if(RPsToBeRetracted.getCount() > 0) {
            //add to the MP, the corresponding list or lists of message of instructions
            addMessageLists(MP, RPsToBeRetracted);

            //move the RPs to be retracted to their final positions
            RPsToBeRetracted.MoveFin();

            //WARNING: RPs to be retracted shall be in their final positions
            //for avoid inter ferences with the next iterationand to get the
            //message-instruction list to turn the rotors 1.

            //delete the retracted RPs from the list Outsiders_
            for(int i=0; i<RPsToBeRetracted.getCount(); i++) {
                TRoboticPositioner *RP = RPsToBeRetracted[i];
                int j = Outsiders_.Search(RP);
                //perform a rutinary checking
                if(j >= Outsiders_.getCount())
                    throw EImpossibleError("previously extracted RP  fron the list Outsiders_ not found in this list");
                Outsiders_.Delete(j);
            }
        }

        //determines if has retracted some RP in this iteration
        aux1 = (RPsToBeRetracted.getCount() > 0);
        //determines if there is more RPs which we would like attempt retract
        aux2 = (Outsiders_.getCount() > 0);

    //while has been retracted some RP and there is more RPs which we would like attempt retract
    } while(aux1 && aux2);

    //determines if the generated MP produces dynamic collisions
    getFiberMOSModel()->RPL.RestorePositions();
    bool collision = motionProgramIsntValid(MP);
    //if the MP poduces dynamic collisions
    if(collision)
        //indicates that not can find a solution
        return false;

    //discard the stacked initial positions
    getFiberMOSModel()->RPL.PopPositions();

    //WRNING: all RPs retracted shall stay in security positions,
    //to allow add a message list to go to the origins.

    //indicates in the obstructed list, the operative RPs which remain in unsequrity positions
    //and are in the outsiders list
    Obstructed = Outsiders_;

    //indicates that has found a solutions, and return the DP and the obstructed list
    return true;
}

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
bool TMotionProgramGenerator::generateDepositioningProgram(TRoboticPositionerList& Collided,
    TRoboticPositionerList& Obstructed, TMotionProgram& DP,
    const TRoboticPositionerList& Outsiders)
{
    //attempt to generates the recovery program
    bool success = generateRecoveryProgram(Collided, Obstructed, DP, Outsiders);

    //if generation function was successfully generated
    if(success) {
        //Here all operative outsiders RPs which aren't obstructed are in secure position,
        //in their final position after execute the DP.

        //segregate the operative inners RPs out of the origin and sorts it
        TRoboticPositionerList Inners;
        getFiberMOSModel()->RPL.segregateOperativeInnersOutTheOrigins(Inners);
        Inners.SortInc();

        //Sort the RPs isn't really necessary, but is recomendable because produce a more legible output.

        //generates the parking gesture for the operative RPs in secure position out the origin
        addMessageListToGoToTheOrigins(DP, Inners);
    }

    return success;
}

//Generates a positioning program from a given depositioning program.
void TMotionProgramGenerator::generatePositioningProgram(TMotionProgram& PP,
    const TMotionProgram& DP, const TPairPositionAnglesList& IPL)
{
    //ERROR: pendient of revisión 25-01-2015

    //CHECK THE PRECONDITIONS:

    //shall be a PPA for each RP of the Fiber MOS Model
    for(int i=0; i<IPL.getCount(); i++) {
        int Id = IPL[i].getId();
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
    TPairPositionAnglesList finalPositions(IPL);

    //for each message list of the DP
    for(int i=0; i<DP.getCount(); i++) {
        const TMessageList *ML = DP.GetPointer(i);

        //actualice initial positions
        initialPositions = finalPositions;

        //build the message list for contains the inverse gesture
        TMessageList *IML = new TMessageList();

        //for each message of instruction build and add the corresponding inverse message instruction
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

            //set thecorresponding initial position as final position in  the inverse message of instruction
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

            //and the inverse message of instruction to the inversemessage list
            IML->Add(IMI);
        }

        //insert the invert message list in the PP
        PP.Insert(0, IML);
    }
}

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
bool TMotionProgramGenerator::generatePairPPDP(TRoboticPositionerList& Collided,
    TRoboticPositionerList& Obstructed, TMotionProgram& PP,
    TMotionProgram& DP, const TRoboticPositionerList& Outsiders)
{
    //captures the initial positions of the RPs in a PPA list
    TPairPositionAnglesList IPL;
    getFiberMOSModel()->RPL.GetPositions(IPL);

    //attempt to generates the depositioning program
    bool success = generateDepositioningProgram(Collided, Obstructed, DP, Outsiders);

    //if has obtained a valid DP, generates the corresponding PP
    if(success)
        generatePositioningProgram(PP, DP, IPL);

    //indicates the result of the generation process
    return success;
}

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------

