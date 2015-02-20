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

//#include <values.h>
#include <unistd.h>
//#include "Windows.h" //Sleep

#include <limits> //std::numeric_limits

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

//---------------------------------------------------------------------------
//METHODS OF LOWER LEVEL:
/*
//program the retraction of all RP of the list Outsiders
//Preconditions:
//  All RPs of the list Outsiders shall be in insecurity position.
//  All RPs of the list Outsiders, shall have a rotor 2 velocity
//  approximately double that rotor 1 velocity.
//Notes:
//  The quantifiers of the rotors may be enabled or disabled.
void TMotionProgramGenerator::programRetraction(TRoboticPositionerList &Outsiders)
{
    //CHECK THE PRECONDITIONS:

    //all RPs of the list Outsiders shall be in insecurity position
    for(int i=0; i<Outsiders.getCount(); i++) {
        TRoboticPositioner *RP = Outsiders.Get(i);
        if(RP->getActuator()->ArmIsInSafeArea())
            throw EImproperArgument("all positioners in the list Outsiders should be in insecurity position");
        if(Abs(RP->wmaxabs2()/RP->wmaxabs1() - 2) > 0.1)
            throw EImproperCall("all positioners in the list Outsiders should be a rotor 2 velocity approximately double that rotor 1 velocity");
    }

    //for each RP of the list Outsiders:
    for(int i=0; i<Outsiders.getCount(); i++) {
        //point the indicated RP tofacilitate its access
        TRoboticPositioner *RP = Outsiders.Get(i);

        //enable the quantifiers to accomplish the precondition
        RP->getActuator()->PushQuantifys();
        RP->getActuator()->setQuantify_(true);
        RP->getActuator()->getArm()->setQuantify___(true);

        //program the retraction
        RP->programRetractArmToSafeArea();               

        //restore and pop the status of the FMM
        getFiberMOSModel()->RPL.RestoreAndPopQuantifys();
    }

    //WARNING: when rotor 1 is near of its lower limit,
    //not all trajectory of the fiber may be radial.
}
*/
//segregate the RPs of the list Outsiders, in disjoint subsets
void TMotionProgramGenerator::segregateRPsInDisjointSets(
        TPointersList<TRoboticPositionerList>& DisjointSets,
        const TRoboticPositionerList& Outsiders)
{
    //CHECK THE PRECONDITIONS:

    //all RPs in the list Outsiders, shall be operatives in insecurity positions
    for(int i=0; i<Outsiders.getCount(); i++) {
        //point the indicated RP to facilitate its access
        TRoboticPositioner *RP = Outsiders[i];
        //if the RP is not operative
        if(!RP->getOperative())
            //indicates thar all RPs in the list Outsiders shall be operatives
            throw EImproperArgument("all RPs in thelist Outsiders shall be operatives");
        //if the RP is in safe area
        if(RP->getActuator()->ArmIsInSafeArea())
            //indicates thar all RPs in the list Outsiders shall be in insecurity positions
            throw EImproperArgument("all RPs in thelist Outsiders shall be in insecurity positions");
    }

    //START:

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

/*//Determines the limits of the open interval (l1, l2)
//where RP collide with RPA.
//It is to say in l1 and l2 there isn't collision.
//Inputs:
//  RP: robotic positioner whose rotor 1 shall be displaced.
//  RPA: robotic positioner adjacent to RP which shall remains stoped.
//Outputs:
//  l1: stable lower limit of interval (l1, l2)
//  l2: stable upper limit of interval (l1, l2)
//Postconditions:
//  The RP will be in their initial status.
//Notes:
//  The quantifiers of the rotors may be enabled or disabled.
//  When l1 == l2 there isn't collision. (In this case moreover l1 == p1 == l2).
//  When l1 < l2, the soltion can be searched in both senses.
//  When l2 < l1, the solution shall be searched:
//      in positive sense, when p_1 is in [p_1min, l2)
//      in negative sense, when p_1 is in (l1, p_1max]
void TMotionProgramGenerator::determinesCollisionInterval(double& l1, double& l2,
        TRoboticPositioner *RP, const TRoboticPositioner *RPA)
{
    //CHECK THE PRECONDITIONS:

    //pointer RP shall points to built robotic positioner
    if(RP == NULL)
            throw EImproperArgument("pointer RP should point to built robotic positioner");

    //pointer RPA shall points to built robotic positioner
    if(RPA == NULL)
            throw EImproperArgument("pointer RPA should point to built robotic positioner");

    //MAKE THE ACTIONS:

    //save the initial position of rotor 1 of the RP
    double theta_1bak = RP->getActuator()->gettheta_1();

    //get the interval of search [p_1low, p_1upp]
    double p_1low = Max(0., ceil(RP->getActuator()->getp_1min()));
    double p_1upp = Min(floor(RP->getActuator()->getSB1()), floor(RP->getActuator()->getp_1max()));

    //initialice p_1
    double p_1 = p_1low;
    //move the rotor 1 to the first position
    RP->getActuator()->setp_1(p_1);
    //calculates the distance between contours of the arms
    double D = RP->getActuator()->getArm()->getContour().DistanceMin(RPA->getActuator()->getArm()->getContour());
    //calculates the free distance
    double Df = D - RP->getActuator()->getArm()->getSPM() - RPA->getActuator()->getArm()->getSPM();
    //initiaize the minimun free distance
    double Dfmin = Df;
    //initialize theposition where the arm of RP is nearest to the arm of RPA
    double p_1_ = p_1;
    //initialize the collision flag
    bool collision = (Df < 0);

    //search in the interval
    for(p_1=round(p_1low+50); p_1<=p_1upp; p_1+=50, p_1=round(p_1)) {
        //capture the status of the collision flag in collision_prev flag
        bool collision_prev = collision;

        //set the p_1
        RP->getActuator()->setp_1(p_1);

        //calculates the distance between contours of the arms
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

        if(collision != collision_prev) {
            if(collision)
                l1 = round(p_1 - 50);
            else
                l2 = p_1;
        }
    }

    //if there isn't collision
    if(Dfmin >= 0) {
        l1 = p_1_;
        l2 = p_1_;
    }

    //Note that here Dfmin can be zero or less zero. This not
    //occur with the Tfmin whose minimun value is zero.

    //retore the initial position of rotor 1
    RP->getActuator()->settheta_1(theta_1bak);

    //ERROR: this function is a temporal solution while a binary searhc is implemented.
}

//Determines if a RP in collision status can solve their collision
//turning the rotor 1, and calculates the new stable position of it,
//in the indicated direction by the tendence.
//Inputs:
//  RP: the robotic positioner in collission status.
//Onputs:
//- collisionCanbesolved: flag indicating if collision can be solved
//  turning the rotor 1.
//- p_1new: the stable position which the rotor 1 of the RP must bemoved
//  to solve the collision.
//Preconditions:
//  Pointer RP shall  point to built robotic positioner.
//  The RP shall be in collision status with one or more adjacent RPs.
//  The rotor 1 of the RP shall has stacked one position almost.
//Postconditions:
//  The RP will be in their initial status.
//Notes:
//  The quantifiers of the rotors may be enabled or disabled.
bool TMotionProgramGenerator::collisionCanBesolved(double& p_1new, TRoboticPositioner *RP)
{
    //CHECK THE PRECONDITIONS:

    //pointer RP shall point to built robotic positioner
    if(RP == NULL)
        throw EImproperArgument("pointer should point to built robotic positioner");

    //the RP shall be in collision status
    if(RP->getActuator()->ThereIsntCollisionWithAdjacent())
        throw EImproperArgument("the RP should be in collision status");

    //The rotor 1 of the RP shall has stacked one position almost
    if(RP->getActuator()->theta_1s.getCount() < 1)
        throw EImproperCall("the rotor 1 of the RP shall has stacked one position almost");

    //The rotor 1 of the RP shall be stacked a positions to determines
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

        //make a rutinary check of (l1, l2)
        if((l1 != round(l1)) || (l2 != round(l2)))
            throw EImpossibleError("l1 and l2 shall be stable positions");

        l1s.Add(l1);
        l2s.Add(l2);

    }

    //When l1 == l2 there isn't collision. (In this case moreover l1 == p1 ==l2).
    //When l1 < l2, the soltion can be searched in both senses.
    //When l2 < l1, the solution shall be searched:
    //  in positive sense, when p_1 is in [p_1min, l2)
    //  in negative sense, when p_1 is in (l1, p_1max]

    //WARNING: the searching interval where search the solution, not must be confused with the interval [p_1min, p_1max]:
    //  double p_1low = Max(0., ceil(RP->getActuator()->getp_1min()));
    //  double p_1upp = Min(floor(RP->getActuator()->getSB1()), floor(RP->getActuator()->getp_1max()));

    //search the largest interval where p_1 is inner
    int imax = 0;
    double lengthmax = 0;
    for(int i=0; i<l1s.getCount(); i++) {
        //get the limits of the intervals
        double l1 = l1s[i];
        double l2 = l2s[i];
        double p_1low = Max(0., ceil(RP->getActuator()->getp_1min()));
        double p_1upp = Min(floor(RP->getActuator()->getSB1()), floor(RP->getActuator()->getp_1max()));

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
            length = (l2 - p_1low) + (p_1upp - l1);
            if(p_1<l2 || l1<p_1)
                collision = true;
            else
                collision = false;
        } else {
            length = 0;
            collision = false;
        }

        //if there is collision actualice the tip (imax, lengthmax)
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
            s1 = std::numeric_limits<double>::max();
            s2 = l2 - p_1;
            p_1new = l2;
        } else if(l1 < p_1) {
            s1 = p_1 - l1;
            s2 = std::numeric_limits<double>::max();
            p_1new = l1;
        }
    } else { //l1 == l2
        throw EImpossibleError("l1 shall not be equal to l2, becuase here is supossed that there is a collision to be solved");
    }

    //determines the displacement to scape from the collision
    double dp1_esc = p_1new - p_1;

    //determines the previous displacement
    //enabling temporarly the quantifier of the rotor
    RP->getActuator()->PushQuantify_();
    RP->getActuator()->setQuantify_(true);
    double dp1_prev = RP->dp1();
    RP->getActuator()->PopQuantify_();

    //if rotor 1 has been previously displaced in the opposed sense to escape from the collision
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
                        p_1new = -std::numeric_limits<double>::max();
                    } else if(dp1_esc > 0) {
                        p_1new = l2;
                    } else { //dp1_esc == 0
                        throw EImpossibleError("pd1_esc can't be equal to 0, because here is supossed that a collision shall be solved");
                    }
                } else if(l1 < p_1new) {
                    if(dp1_esc < 0) {
                        p_1new = l1;
                    } else if(dp1_esc > 0) {
                        p_1new = std::numeric_limits<double>::max();
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
*/

//Propose a recovery program composed to two gestures:
//  1. radial retraction
//  2. abatement of the arm (if necessary)
void TMotionProgramGenerator::proposeRecoveryProgram(TMotionProgram& MP, TRoboticPositioner *RP)
{
    //check the preconditions
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");
    if(RP->getActuator()->theta_1s.getCount()<=0 || RP->getActuator()->getArm()->theta___3s.getCount()<=0)
        throw EImproperCall("roboitc positioner RP should be stacket the initial position");

    //initialize the output variable
    MP.Clear();

    //get the initial position of the rotors in radians
    double theta_1ini = RP->getActuator()->theta_1s.getLast();
    double theta___3ini = RP->getActuator()->getArm()->theta___3s.getLast();

    //ADD INSTRUCTION FOR RADIAL RETRACTION:

    //determines the initial position of rot 2 in steps
    double p___3ini = RP->getActuator()->getArm()->getF().Image(theta___3ini);
    p___3ini= round(p___3ini);

    //determines the first stable security position of rot 2 in steps
    double theta___3saf = Max(0., RP->getActuator()->gettheta___3saf());
    double p___3saf = RP->getActuator()->getArm()->getF().Image(theta___3saf);
    p___3saf = floor(p___3saf);

    //determines the necessary displaceement of rot 2 in steps
    //to go to the first security stable position
    double dp2 = p___3ini -  p___3saf;

    //determines the necessary displacement of rot 2 in radians
    double dt2 = RP->getActuator()->getArm()->getG().Image(dp2);

    //calculates the necessary displacement of rotor 1 in radians
    double dt1 = dt2/2;

    //determines the final position of the rotors in radians
    double theta_1fin, theta___3fin;
    double p_1fin, p___3fin;
    if(RP->getActuator()->gettheta_1() > dt1) {
        theta_1fin = theta_1ini - dt1;
        theta___3fin = theta___3ini - dt2;

        //determines the final position of rot 1 in steps
        p_1fin = RP->getActuator()->getF().Image(theta_1fin);
        p_1fin = Max(0., round(p_1fin));

        //determines the final position of rot 2 in steps
        p___3fin = p___3saf;

    } else {
        theta_1fin = 0;
        dt1 = theta_1ini;
        dt2 = dt1*2;
        theta___3fin = theta___3ini - dt2;

        //determines the final position of rot 1 in steps
        p_1fin = RP->getActuator()->getF().Image(theta_1fin);
        p_1fin = Max(0., round(p_1fin));

        //determines the final position of rot 2 in steps
        p___3fin = RP->getActuator()->getArm()->getF().Image(theta___3fin);
        p___3fin = Max(0., round(p___3fin));

    }

    //build a message instruction and set up
    TMessageInstruction *MI = new TMessageInstruction();
    MI->setId(RP->getActuator()->getId());
    MI->Instruction.setName("MM");
    MI->Instruction.Args.setCount(2);
    MI->Instruction.Args[0] = p_1fin;
    MI->Instruction.Args[1] = p___3fin;

    //add the message instruction to the MP
    Positioning::TMessageList *ML = new Positioning::TMessageList();
    ML->Add(MI);
    MP.Add(ML);

    //determines if it is necessary abate the arm for recovery the security position
    bool necessary_abate = (p___3fin > p___3saf);

    //ADD INSTRUCTION FOR ABATEMENT OF THE ARM IF NECESSARY:

    if(necessary_abate) {
        //build a message instruction and set up
        TMessageInstruction *MI = new TMessageInstruction();
        MI->setId(RP->getActuator()->getId());
        MI->Instruction.setName("M2");
        MI->Instruction.Args.setCount(1);
        MI->Instruction.Args[0] = p___3saf;

        //add the message instruction to the MP
        Positioning::TMessageList *ML = new Positioning::TMessageList();
        ML->Add(MI);
        MP.Add(ML);
    }
}

//Propose a recovery program composed to three gestures:
//  1. turn of rotor 1 to p_1new
//  2. radial retraction
//  3. abatement of the arm (if necessary)
void TMotionProgramGenerator::proposeRecoveryProgram(TMotionProgram& MP, TRoboticPositioner *RP,
                                                     double p_1new)
{
    //check the preconditions
    if(RP == NULL)
        throw EImproperArgument("pointer RP sopuld point to built robotic positioner");
    if(RP->getActuator()->theta_1s.getCount()<=0 || RP->getActuator()->getArm()->theta___3s.getCount()<=0)
        throw EImproperCall("roboitc positioner RP should be stacket the initial position");
    if(RP->getActuator()->IsntInDomainp_1(p_1new))
        throw EImproperArgument("the new rotor 1 position p_1new should be in the rotor 1 domain");

    //initialize the output variable
    MP.Clear();

    //add a instruction for turn of the rotor 1
    TMessageInstruction *MI = new TMessageInstruction();
    MI->setId(RP->getActuator()->getId());
    MI->Instruction.setName("M1");
    MI->Instruction.Args.setCount(1);
    MI->Instruction.Args[0] = p_1new;
    Positioning::TMessageList *ML = new Positioning::TMessageList();
    ML->Add(MI);
    MP.Add(ML);

    //get the initial position of the rotors in radians
    double theta_1ini = RP->getActuator()->getG().Image(p_1new);
    double theta___3ini = RP->getActuator()->getArm()->theta___3s.getLast();

    //ADD INSTRUCTION FOR RADIAL RETRACTION:

    //determines the initial position of rot 2 in steps
    double p___3ini = RP->getActuator()->getArm()->getF().Image(theta___3ini);
    p___3ini= round(p___3ini);

    //determines the first stable security position of rot 2 in steps
    double theta___3saf = Max(0., RP->getActuator()->gettheta___3saf());
    double p___3saf = RP->getActuator()->getArm()->getF().Image(theta___3saf);
    p___3saf = floor(p___3saf);

    //determines the necessary displaceement of rot 2 in steps
    //to go to the first security stable position
    double dp2 = p___3ini -  p___3saf;

    //determines the necessary displacement of rot 2 in radians
    double dt2 = RP->getActuator()->getArm()->getG().Image(dp2);

    //calculates the necessary displacement of rotor 1 in radians
    double dt1 = dt2/2;

    //determines the final position of the rotors in radians
    double theta_1fin, theta___3fin;
    double p_1fin, p___3fin;
    if(RP->getActuator()->gettheta_1() > dt1) {
        theta_1fin = theta_1ini - dt1;
        theta___3fin = theta___3ini - dt2;

        //determines the final position of rot 1 in steps
        p_1fin = RP->getActuator()->getF().Image(theta_1fin);
        p_1fin = Max(0., round(p_1fin));

        //determines the final position of rot 2 in steps
        p___3fin = p___3saf;

    } else {
        theta_1fin = 0;
        dt1 = theta_1ini;
        dt2 = dt1*2;
        theta___3fin = theta___3ini - dt2;

        //determines the final position of rot 1 in steps
        p_1fin = RP->getActuator()->getF().Image(theta_1fin);
        p_1fin = Max(0., round(p_1fin));

        //determines the final position of rot 2 in steps
        p___3fin = RP->getActuator()->getArm()->getF().Image(theta___3fin);
        p___3fin = Max(0., round(p___3fin));

    }

    //build a message instruction and set up
    MI = new TMessageInstruction();
    MI->setId(RP->getActuator()->getId());
    MI->Instruction.setName("MM");
    MI->Instruction.Args.setCount(2);
    MI->Instruction.Args[0] = p_1fin;
    MI->Instruction.Args[1] = p___3fin;

    //add the message instruction to the MP
    ML = new Positioning::TMessageList();
    ML->Add(MI);
    MP.Add(ML);

    //determines if it is necessary abate the arm for recovery the security position
    bool necessary_abate = (p___3fin > p___3saf);

    //ADD INSTRUCTION FOR ABATEMENT OF THE ARM IF NECESSARY:

    if(necessary_abate) {
        //build a message instruction and set up
        TMessageInstruction *MI = new TMessageInstruction();
        MI->setId(RP->getActuator()->getId());
        MI->Instruction.setName("M2");
        MI->Instruction.Args.setCount(1);
        MI->Instruction.Args[0] = p___3saf;

        //add the message instruction to the MP
        Positioning::TMessageList *ML = new Positioning::TMessageList();
        ML->Add(MI);
        MP.Add(ML);
    }
}

//Search in negative sense, the stable positions of the rotor 1 of the RP,
//where the collision can be solved.
//Inputs:
//  RP: robotic positioner whose rotor 1 shall be displaced.
//  RPA: robotic positioner adjacent to RP which shall remains stoped.
//  dt1max: maximun displacement of the rotor 1 for search the solution
//      in rad.
//Outputs:
//  searchSolutionInNegativeSense: indicates if the solution is valid.
//  p_1new: last position where solution has been searched.
//  MP: the solution.
//Preconditions:
//  The RP shall be disabled the quantifier of their rotors.
//  The RP shall be stacked the actual position of their rotor 1.
//  The searching interval dt1max shall be upper zero.
//  The RP shall contains the default MP.
//Postconditions:
//  The RP will be in their initial status.
//Notes:
//  The initial position can be stable or inestable.
//  When searchSolutionInNegativeSense == false
//      solution p_1new will be nearest to theta_1 - dt1max.
bool TMotionProgramGenerator::searchSolutionInNegativeSense(double& p_1new, TMotionProgram& MP,
    TRoboticPositioner *RP, double dt1max)
{
    //CHECK THE PRECONDITIONS:

    //The RP shall be disabled the quantifier of their rotors.
    if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
        throw EImproperCall("the RP shall be disabled the quantifier of their rotors");

    //The RP shall be stacked the actual position of their rotor 1.
    if(RP->getActuator()->theta_1s.getCount()<=0 || RP->getActuator()->getArm()->theta___3s.getLast()!=RP->getActuator()->getArm()->gettheta___3())
        throw EImproperCall("the RP shall be stacked the actual position of their rotor 1");
    if(RP->getActuator()->thetasNotCoincideWithStacked())
        throw EImproperCall("the RP shall be stacked the actual position of their rotor 1");

    //the searching interval dt1max shall be upper zero
    if(dt1max <= 0)
        throw EImproperArgument("the searching interval dt1max should be upper zero");

    //the RP shall contains the default MP
    if(RP->MP.getCount() <= 0)
        throw EImproperArgument("the RP should contains the default MP");

    //START:

    //get the upper position where search the solution
    double theta_1 = RP->getActuator()->theta_1s.getLast();
    double p_1upper = RP->getActuator()->getF().Image(theta_1);
    p_1upper = ceil(p_1upper);

    //get the first position where the rotor 1 can be positioned
    double theta_1first = 0;
    if(theta_1first < RP->getActuator()->gettheta_1min())
        theta_1first = RP->getActuator()->gettheta_1min();
    if(theta_1first < RP->getActuator()->getF().getXFirst())
        theta_1first = RP->getActuator()->getF().getXFirst();

    //get the lower position where search the solution
    theta_1 -=  dt1max;
    if(theta_1 < theta_1first)
        theta_1 = theta_1first;
    double p_1lower = RP->getActuator()->getF().Image(theta_1);
    p_1lower = ceil(p_1lower);

    //correct the limit case
    if(p_1lower > p_1upper)
        p_1lower = p_1upper;

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
    MP = RP->MP;

    //search a solution with an adjacent every time
    while(there_is_solution && i<Adjacents.getCount()) {
        //point the next RP single in the list of adjacents
        TRoboticPositioner *RP_ = Adjacents[i];
        RP->getActuator()->Adjacents.Clear();
        RP->getActuator()->Adjacents.Add(RP_);

        //determine if the MP is valid (with the single adjacent)
        there_is_solution = motionProgramIsValid(MP);
        //restablish the initial position of the RP
        RP->getActuator()->Restorethetas();

        //if the MP is valid pass to the next adjacent
        if(there_is_solution)
            i++;

        //if the MP is invalid,  perform the binary search of the p_1new (with the single adjacent)
        else {
            //restablish the initial status of the FMM
            getFiberMOSModel()->RPL.RestoreAndPopQuantifys();

            //perform the binary search of the p_1new (with the single adjacent)
            do {
                //calculates the new jumping position
                p_1new = round((p_1lower + p_1upper)/2);

                //propose the program for the new jumping position
                proposeRecoveryProgram(MP, RP, p_1new);

                //determine if the MP is valid (with the single adjacent)
                there_is_solution = motionProgramIsValid(MP);
                //restablish the initial position of the RP
                RP->getActuator()->Restorethetas();

                //adjust the new searching interval
                if(there_is_solution)
                    //displace the lower limit
                    p_1lower = p_1new;

                else {
                    //restablish the initial status of the FMM
                    getFiberMOSModel()->RPL.RestoreAndPopQuantifys();

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

//Search in positive sense, the stable positions of the rotor 1 of the RP,
//where the collision can be solved.
//Inputs:
//  RP: robotic positioner whose rotor 1 shall be displaced.
//  RPA: robotic positioner adjacent to RP which shall remains stoped.
//  dt1max: maximun displacement of the rotor 1 for search the solution
//      in rad.
//Outputs:
//  searchSolutionInNegativeSense: indicates if the solution is valid.
//  p_1new: last position where solution has been searched.
//  MP: the solution.
//Preconditions:
//  The RP shall be disabled the quantifier of their rotors.
//  The RP shall be stacked the actual position of their rotor 1.
//  The searching interval dt1max shall be upper zero.
//  The RP shall contains the default MP.
//Postconditions:
//  The RP will be in their initial status.
//Notes:
//  The initial position can be stable or inestable.
//  When searchSolutionInPositiveSense == false
//      solution p_1new will be nearest to theta_1 + dt1max.
bool TMotionProgramGenerator::searchSolutionInPositiveSense(double& p_1new, TMotionProgram& MP,
    TRoboticPositioner *RP, double dt1max)
{
    //CHECK THE PRECONDITIONS:

    //The RP shall be disabled the quantifier of their rotors.
    if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
        throw EImproperCall("the RP shall be disabled the quantifier of their rotors");

    //The RP shall be stacked the actual position of their rotor 1.
    if(RP->getActuator()->theta_1s.getCount()<=0 || RP->getActuator()->getArm()->theta___3s.getLast()!=RP->getActuator()->getArm()->gettheta___3())
        throw EImproperCall("the RP shall be stacked the actual position of their rotor 1");
    if(RP->getActuator()->thetasNotCoincideWithStacked())
        throw EImproperCall("the RP shall be stacked the actual position of their rotor 1");

    //the searching interval dt1max shall be upper zero
    if(dt1max <= 0)
        throw EImproperArgument("the searching interval dt1max should be upper zero");

    //the RP shall contains the default MP
    if(RP->MP.getCount() <= 0)
        throw EImproperArgument("the RP should contains the default MP");

    //START:

    //get the lower position where search the solution
    double theta_1 = RP->getActuator()->gettheta_1();
    double p_1 = RP->getActuator()->getF().Image(theta_1);
    double p_1lower = ceil(p_1);

    //get the last position where the rotor 1 can be positioned
    double theta_1last = M_2PI;
    if(theta_1last > RP->getActuator()->gettheta_1max())
        theta_1last = RP->getActuator()->gettheta_1max();
    if(theta_1last > RP->getActuator()->getF().getXLast())
        theta_1last = RP->getActuator()->getF().getXLast();

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
    MP = RP->MP;

    //search a solution with an adjacent every time
    while(there_is_solution && i<Adjacents.getCount()) {
        //point the next RP single in the list of adjacents
        TRoboticPositioner *RP_ = Adjacents[i];
        RP->getActuator()->Adjacents.Clear();
        RP->getActuator()->Adjacents.Add(RP_);

        //determine if the MP is valid (with the single adjacent)
        there_is_solution = motionProgramIsValid(MP);
        //restablish the initial position of the RP
        RP->getActuator()->Restorethetas();

        //if the MP is valid pass to the next adjacent
        if(there_is_solution)
            i++;

        //if the MP is invalid,  perform the binary search of the p_1new (with the single adjacent)
        else {
            //restablish the initial status of the FMM
            getFiberMOSModel()->RPL.RestoreAndPopQuantifys();

            //perform the binary search of the p_1new (with the single adjacent)
            do {
                //calculates the new jumping position
                p_1new = round((p_1lower + p_1upper)/2);

                //propose the program for the new jumping position
                proposeRecoveryProgram(MP, RP, p_1new);

                //determine if the MP is valid (with the single adjacent)
                there_is_solution = motionProgramIsValid(MP);
                //restablish the initial position of the RP
                RP->getActuator()->Restorethetas();

                //adjust the new searching interval
                if(there_is_solution)
                    //displace the upper limit
                    p_1upper = p_1new;

                else {
                    //restablish the initial status of the FMM
                    getFiberMOSModel()->RPL.RestoreAndPopQuantifys();

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

/*//Determines if a RP in collision status can solve their collision
//turning the rotor 1, and calculates the new stable position of this,
//in the indicated direction by the tendence.
//Inputs:
//  RP: the robotic positioner in collission status.
//  dt1max: maximun displacement of the rotor 1 for search the solution.
//Onputs:
//- collisionCanbesolved: flag indicating if collision can be solved
//  turning the rotor 1.
//- p_1new: the stable position which the rotor 1 of the RP must be moved
//  to solve the collision.
//Preconditions:
//  Pointer RP shall  point to built robotic positioner.
//  The RP shall be in collision status with one or more adjacent RPs.
//  The RP shall be disabled the quantifiers of their rotors.
//Postconditions:
//  The RP will be in their initial status.
//Notes:
//  The quantifiers of the rotors may be enabled or disabled.
bool TMotionProgramGenerator::collisionCanBesolved(double& p_1new,
                                                   TRoboticPositioner *RP, double dt1max)
{
    //CHECK THE PRECONDITIONS:

    //pointer RP shall point to built robotic positioner
    if(RP == NULL)
        throw EImproperArgument("pointer should point to built robotic positioner");

    //the RP shall be in collision status
    if(RP->getActuator()->ThereIsntCollisionWithAdjacent())
        throw EImproperArgument("the RP should be in collision status");

    //the RP shall be disabled the quantifiers of their rotors
    if(RP->getActuator()->getQuantify_()!=false || RP->getActuator()->getArm()->getQuantify___()!=false)
        throw EImproperCall("the RP should be enabled the quantifiers of their rotors");

    //MAKE THE ACTION:

    //determines the previous displacement
    //re-enablingtemporarly the quantifiers
    RP->getActuator()->PushQuantify_();
    RP->getActuator()->setQuantify_(true);
    double dp1_prev = RP->dp1();
    RP->getActuator()->RestoreAndPopQuantify_();

    //ERROR: al rehabilitar el cuantificador, se producirá un desplazamiento de rot 1.
    //SOLUCIÓN: norequerir que se

    //if the tendence of the movement is in negative sense
    if(dp1_prev < 0) {
        //search the solution in negative sense
        p_1new = numeric_limits<double>::max();
        for(int i=0; i<RP->getActuator()->Adjacents.getCount(); i++) {
            TRoboticPositioner *RPA = RP->getActuator()->Adjacents[i];

            //if there is collision with the adjacent
            if(RP->getActuator()->ThereIsCollision(RPA->getActuator())) {
                double p_1a;
                searchSolutionInNegativeSense(p_1a, RP, RPA, dt1max);
                if(p_1a < p_1new)
                    p_1new = p_1a;
            }
        }

        //if has found asolution
        if(p_1new >= 0)
            //indicates that collision can be solved
            return true;
        else //else
            //indicates that collision can't be solved
            return false;
    }

    //else if the tendence of the movement is in positive sense
    if(dp1_prev > 0) {
        //search the solution in positive sense
        p_1new = -numeric_limits<double>::max();
        for(int i=0; i<RP->getActuator()->Adjacents.getCount(); i++) {
            TRoboticPositioner *RPA = RP->getActuator()->Adjacents[i];

            //if there is collision with the adjacent
            if(RP->getActuator()->ThereIsCollision(RPA->getActuator())) {
                double p_1b;
                searchSolutionInPositiveSense(p_1b, RP, RPA, dt1max);
                if(p_1b > p_1new)
                    p_1new = p_1b;
            }
        }

        //if has found asolution
        if(p_1new <= RP->getActuator()->getSB1())
            //indicates that collision can be solved
            return true;
        else //else
            //indicates that collision can't be solved
            return false;
    }

    //else if the tendence of the movement is in both senses:

    //search the solution in negative sense
    double p_1min = numeric_limits<double>::max();
    for(int i=0; i<RP->getActuator()->Adjacents.getCount(); i++) {
        TRoboticPositioner *RPA = RP->getActuator()->Adjacents[i];

        //if there is collision with the adjacent
        if(RP->getActuator()->ThereIsCollision(RPA->getActuator())) {
            double p_1a;
            searchSolutionInNegativeSense(p_1a, RP, RPA, dt1max);
            if(p_1a < p_1min)
                p_1min = p_1a;
        }
    }

    //search the solution in positive sense
    double p_1max = -numeric_limits<double>::max();
    for(int i=0; i<RP->getActuator()->Adjacents.getCount(); i++) {
        TRoboticPositioner *RPA = RP->getActuator()->Adjacents[i];

        //if there is collision with the adjacent
        if(RP->getActuator()->ThereIsCollision(RPA->getActuator())) {
            double p_1b;
            searchSolutionInPositiveSense(p_1b, RP, RPA, dt1max);
            if(p_1b > p_1max)
                p_1max = p_1b;
        }
    }


    //if has found asolution
    if(0<=p_1min || p_1max<=RP->getActuator()->getSB1()) {
        //calculate the displacement in negative sense
        double da;
        if(0 <= p_1min)
            da = RP->getActuator()->getp_1() - p_1min;
        else
            da = numeric_limits<double>::max();

        //calculate the displacement in positive sense
        double db;
        if(p_1max <= RP->getActuator()->getSB1())
            db = p_1max - RP->getActuator()->getp_1();
        else
            db = numeric_limits<double>::max();

        //selectthe nearest solution
        if(da <= db)
            p_1new = p_1min;
        else
            p_1new = p_1max;

        //indicates that collision can be solved
        return true;

    } else //else
        //indicates that collision can't be solved
        return false;

}
*/
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
//      shall be in insecure position;
//      shall be enabled the quantifiers of their rotors.
//      shall not have dynamic collisions;
//Postconditions:
//  RecoverablesDDS will contains the RPs which can be recovered,
//  programmedwith the necessary instruction for their recovery.
//  All RPs of the RecoverablesDDS, will beintheir initial  positions.
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

    //All RPs of the Fiber MOS Model shall be configurated for MP generation.
    for(int i=0; i<getFiberMOSModel()->RPL.getCount(); i++) {
        TRoboticPositioner *RP = getFiberMOSModel()->RPL[i];
        if(RP->getActuator()->getPurpose() != pGen)
            throw EImproperCall("All RPs of the Fiber MOS Model shall be configurated to MP generation.");
    }

    //  All RPs of DDS:
    //      shall be in the Fiber MOS  Model;
    //      shall be in insecure position;
    //      shall be enabled the quantifiers of their rotors.
    //      shall not have dynamic collisions.
    for(int i=0; i<DDS.getCount(); i++) {
        TPointersList<TRoboticPositionerList> *DisjointSet = DDS.GetPointer(i);
        for(int j=0; j<DisjointSet->getCount(); j++) {
            TRoboticPositionerList *DisperseSubset = DisjointSet->GetPointer(j);
            for(int k=0; k<DisperseSubset->getCount(); k++) {
                TRoboticPositioner *RP = DisperseSubset->Get(k);
                int l = getFiberMOSModel()->RPL.Search(RP);
                if(l >= getFiberMOSModel()->RPL.getCount())
                    throw EImproperArgument("all RPs of DDS shall be in the Fiber MOS Model");
                if(RP->getActuator()->ArmIsInSafeArea())
                    throw EImproperArgument("all RPs of DDS shall be in insecure position");
                if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
                    throw EImproperArgument("all RPs of DDS shall be enabled the quantifiers of their rotors");
                if(RP->getActuator()->ThereIsCollisionWithAdjacent())
                    throw EImproperArgument("all RPs of DDS shall not have dynamic collisions");
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
    //  are in insecure position.
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

                //-------------------------------------------------------------
                //INITIALIZE THE INDIVIDUAL MP OF THE RP:

                //propose the recovery program by default
                proposeRecoveryProgram(RP->MP, RP);

                //-------------------------------------------------------------
                //SEARCH THE BEST ALTERNATIVE SOLUTION:

                bool RP_is_recoverable;

                //determine if the motion program is not valid
                bool MP_is_not_valid = !motionProgramIsValid(RP->MP);
                //restore the initial position of the RP
                RP->getActuator()->Restorethetas();

                //After determine if MP is valids:
                //  if the MP is valid:
                //      the quantifiers of the rotors of the RP will be enabled,
                //      and the RP will be in their final security position.
                //  if the MP is not valid:
                //      the quantifiers of the rotors of the FMM will be disabled,
                //      and the RP will be in the first position where collision was detected,

                if(MP_is_not_valid) {
                    //restore the initial status of the FMM
                    getFiberMOSModel()->RPL.RestoreAndPopQuantifys();

                    //search a solution in negative sense
                    double p_1new;
                    TMotionProgram MP;
                    bool valid = searchSolutionInNegativeSense(p_1new, MP, RP, M_PI/2); //<<<<<<<<<<<<<<<<<<<

                    //The funtion searchSolutionInNegativeSense not change the status of the RP.

                    if(valid) {
                        //indicates that the RP is recoverable
                        RP_is_recoverable = true;
                        //attach the solution to the RP
                        RP->MP = MP;
                    }
                    else {
                        //search a solution in positive sense
                        valid = searchSolutionInPositiveSense(p_1new, MP, RP, M_PI/2); //<<<<<<<<<<<<<<<<<<<

                        //The funtion searchSolutionInNegativeSense not change the status of the RP.

                        if(valid) {
                            //indicates that the RP is recoverable
                            RP_is_recoverable = true;
                            //attach the solution to the RP
                            RP->MP = MP;
                        }
                        else {
                            //indicates that the RP is unrecoverable
                            RP_is_recoverable = false;
                        }
                    }
                }
                else //if the MP is valid, indicates that the MP is valid
                    RP_is_recoverable = true;

                //-------------------------------------------------------------
                //REACTACCORDING THE FOUND SOLUTION:

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
        if(RP->MP.getCount() <= 0)
            throw EImproperArgument("all RPs of the list RPsToBeRetracted shall have a recovery program");
    }

    TMessageList *MLjoin= NULL;

    do {
        //clone the MIs of the RP and add to MLjoin
        MLjoin = new TMessageList();
        for(int i=0; i<RPsToBeRecovered.getCount(); i++) {
            TRoboticPositioner *RP = RPsToBeRecovered[i];
            if(RP->MP.getCount()) {
                TMessageList *ML = RP->MP.getFirstPointer();

                for(int j=0; j<ML->getCount(); j++) {
                    TMessageInstruction *MI = ML->GetPointer(j);
                    MI = new TMessageInstruction(MI);
                    MLjoin->Add(MI);
                }

                RP->MP.DelFirst(1);
            }
        }

        //add the message list to the MP
        if(MLjoin->getCount() > 0)
            MP.Add(MLjoin);
        else {
            delete MLjoin;
            MLjoin = NULL;
        }

    } while(MLjoin != NULL);


/*    //segregates the RPs whose rotor 1 has been displaced
    TRoboticPositionerList DisplacedRPs;
    for(int i=0; i<RPsToBeRecovered.getCount(); i++) {
        TRoboticPositioner *RP = RPsToBeRecovered[i];
        RP->getActuator()->PushQuantify_();
        RP->getActuator()->setQuantify_(true);
        if(RP->dp1() != 0)
            DisplacedRPs.Add(RP);
        RP->getActuator()->RestoreAndPopQuantify_();

        //Note that the rot 1 quantifier is temporarily enabled,
        //to accomplish the preconditions of function dp1.
    }

    //if there is some RPs whose rotor 1 has been displaced
    if(DisplacedRPs.getCount() > 0) {
        //get the message list to produce the turns of the rotors 1
        TMessageList *ML = new TMessageList(Max(1, getFiberMOSModel()->RPL.getCount()));
        //for each RP of the list DisplacedRPs
        for(int i=0; i<DisplacedRPs.getCount(); i++) {
            //point the indicated RP to facilitate their access
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
    //for each RP of the list RPsToBeRecovered
    for(int i=0; i<RPsToBeRecovered.getCount(); i++) {
        //point the indicated RP to facilitate its access
        TRoboticPositioner *RP = RPsToBeRecovered[i];

        //built a message of instruction
        TMessageInstruction *M = new TMessageInstruction();
        //configure the message
        M->setId(RP->getActuator()->getId());
        RP->getInstruction(M->Instruction);

        //add the message to the message list
        ML->Add(M);
    }
    //add the message list to the DP
    DP.Add(ML);*/
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

//-----------------------------------------------------------------------
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
//  Outsiders: list of operative RPs in insecure positions which
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

    //START:

    //initialize the outputs
    Collided.Clear();
    Obstructed.Clear();
    MP.Clear();

    //configure the Fiber MOS Model for MP generation
    getFiberMOSModel()->RPL.SetPurpose(pGen);

    //segregates the RPs of the list Outsiders which are in collision status, in the list Collided
    getFiberMOSModel()->RPL.segregateCollided(Collided);

    //solve the trivial case when there aren't RPs of the list Outsiders in insecurity positions
    if(Outsiders.allRPsAreInSecurePosition()) {
        //configure the Fiber MOS Model for MP validation
        getFiberMOSModel()->RPL.SetPurpose(pVal);
        //indicates that has found a solution, and return the empty solution
        return true;
    }

    //Here all RPs are in their initial positions.

    //stack the initial positions of all RPs
    getFiberMOSModel()->RPL.PushPositions();

    //Since the initial position must be recovered repeteadly,
    //the initial positions must be stored.

/*    //disable the quantifiers of the rotors
    //of all RPsofthe Fiber MOS Model
    getFiberMOSModel()->RPL.SetQuantifys(false, false);

    //Quantifiers of the rotors must be disabled to avoid displacements to
    //stable positions, when intermediate positions are simulatedm but initial
    //and final position of each gesture shall correspond to stable positions,
    //reason why the quantifiers shall be temporarity enabled when positions
    //in steps are read or displacements of therotors got.
*/
    //Note that here it is not necessary disable the quantifier, because
    //the quantifiers will be disabled only in two places:
    //  1. During segregation of recoverables, for simulate the movement
    //     of each RP from their initial position to their final position.
    //  2. During validation of the generated recovery program, for simmulate
    //     the jointly movement of the RPs which must be move together.
    //If the recovery progrannotpass the validation process, the quantifiers
    //remains disabled, and their original status stacked.

    //=========================================================================
    //ITERATES THE GENERATION OF RECOVERY STEPS, USING A COPY OF Outsiders,
    //(Outsiders_), WHILE ARE ACCOMPLISH THE CONDITIONS:
    //  1. In the list Outsiders_ remains RPs to be recovered.
    //  2. Has been recovered some RP in the last iteration.

    //build the list Outsiders_ to contains the pointers to
    //the RPs wich remain in insecurity positions
    TRoboticPositionerList Outsiders_(Outsiders);

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

/*    //program the retraction for all RPs of the list Outsiders_
    programRetraction(Outsiders_);
*/
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
        segregateRecoverables(RecoverablesDDS, UnrecoverablesDDS, DDS);   //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<MODIFICAR!

        //Here all RPs of RecoverablesDDS will be in their initial positions,
        //and we want retract the RPs in the larger disperse subsets.

        //---------------------------------------------------------------------
        //SEGREGATES THE RPs TO BE RECOVERED:

        //WARNING: if recicle the RPsToBeRecovered, would need to make here:
        //  RPsToBeRecovered.Clear();

        //build the list of RPs to be retracted in each step
        TRoboticPositionerList RPsToBeRecovered;

        //for each disjoint set of retractiles RPs
        for(int i=0; i<RecoverablesDDS.getCount(); i++) {
            //point the disjoint set indicated to facilitate its access
            TPointersList<TRoboticPositionerList> *DisjointSet = RecoverablesDDS.GetPointer(i);

            //search the disperse subset which has the maximun number of RPs
            TRoboticPositionerList *DisperseSubsetMax = NULL;
            for(int j=0; j<DisjointSet->getCount(); j++) {
                TRoboticPositionerList *DisperseSubset = DisjointSet->GetPointer(j);

                //actualize the maximun disperse subset
                if(DisperseSubset->getCount()>0 &&
                        (DisperseSubsetMax==NULL || (DisperseSubset->getCount() > DisperseSubsetMax->getCount())))
                    DisperseSubsetMax = DisperseSubset;

/*                //restore the initial position of the RPs of the disperse subset
                DisperseSubset->RestorePositions();*/
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

        //---------------------------------------------------------------------
        //GET THE MESSAGE-INSTRUCTION LIST CORRESPONDING TO THE RPsToBeRecovered.
        //MOVE THE RPs TO BE RECOVEREDTOTHEIRFINAL  POSITIONS.
        //DELETEDTHE RECOVERED RPs FROM THE LIST Outsiders_:

        //if there is some RP to be retracted
        if(RPsToBeRecovered.getCount() > 0) {
            //add to the MP, the corresponding list or lists of message of instructions
            addMessageLists(MP, RPsToBeRecovered);  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<MODIFICAR!

            //move the RPs to be retracted to their final positions
            RPsToBeRecovered.MoveFin(); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<MODIFICAR!

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

        //---------------------------------------------------------------------
        //DETERMINES IF IS ACCOMPLISHED THE REITERATION CONDITIONS:

        //determines if there is some RPs to berecovered in the list Outsiders_
        condition1 = (Outsiders_.getCount() > 0);
        //determines if has recovered some RP in the last iteration
        condition2 = (RPsToBeRecovered.getCount() > 0);

    //while are accomplish the two conditions
    } while(condition1 && condition2);

    //=========================================================================

    //indicates in the obstructed list, the operative RPs which remain in
    //unsequrity positions and are in the Outsiders list
    Obstructed = Outsiders_;

    //restore and discard the initial positions
    getFiberMOSModel()->RPL.RestoreAndPopPositions();

    //configure the Fiber MOS Model for MP validation
    getFiberMOSModel()->RPL.SetPurpose(pVal);

    //determines if the generated MP is valid
    bool valid = motionProgramIsValid(MP);

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
//  Outsiders: list of operative RPs in insecure positions which
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

    //if the recovery program was successfully generated
    if(valid) {
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

    return valid;
}

//Generates a positioning program from a given depositioning program.
void TMotionProgramGenerator::generatePositioningProgram(TMotionProgram& PP,
    const TMotionProgram& DP, const TPairPositionAnglesList& OPL)
{
    //ERROR: pendient of revisión 25-01-2015

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

            //set the corresponding initial position as final position in  the inverse message of instruction
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

            //and the inverse message of instruction to the inverse message list
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
//  Outsiders: list of operative RPs in insecure positions which
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

    //Here the RPs of thelist Outsiders are in the origin, less the RPs
    //in collision or obstructed, which will be in their observing positions.

    //if has obtained a valid DP, generates the corresponding PP
    if(success)
        //generate the depositioning program
        generatePositioningProgram(PP, DP, OPL);

    //indicates the result of the generation process
    return success;
}

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------

