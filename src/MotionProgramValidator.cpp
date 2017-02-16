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
//File: MotionProgramValidator.cpp
//Content: motion program validator
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "MotionProgramValidator.h"
#include "TextFile.h" //StrWriteToFile
#include "Geometry.h" //distanceLineToPoint

//---------------------------------------------------------------------------

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------

//Get the list of RPs included in a MP.
//Precondition:
//  All message of instruction in the MP shall be addressed
//  to an existent RP of the Fiber MOS Model.
void getRPsIncludedInMP(TRoboticPositionerList& RPL,
                        const TMotionProgram& MP,
                        const TFiberMOSModel *FMM)
{
    //CHECK THE PRECONDITION:

    if(FMM == NULL)
        throw EImproperArgument("pointer FMM should point to built Fiber MOS Model");

    //check if all message of instruction in the MP are addressed to an existent RP of the Fiber MOS Model
    for(int i=0; i<MP.getCount(); i++) {
        const TMessageList *ML = MP.GetPointer(i);
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);
            int k = FMM->RPL.searchId(MI->getId());
            if(k >= FMM->RPL.getCount())
                throw EImproperArgument("all message of instruction in the MP shall be addressed to an existent RP of the Fiber MOS Model");
        }
    }

    //ADD THE RPs TO THE RPL:

    //initialize the output
    RPL.Clear();

    //add the RPs avoiding repetitions
    for(int i=0; i<MP.getCount(); i++) {
        const TMessageList *ML = MP.GetPointer(i);
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);

            //search the identifier RP in the Fiber MOS Model
            int k = FMM->RPL.searchId(MI->getId());
            //if not has found the identifier Id
            if(k >= FMM->RPL.getCount())
                //indicates lateral effect
                throw EImpossibleError("lateral effect");
            //if has found the identifier Id
            else {
                //actualice the RPL avoiding repetitions
                TRoboticPositioner *RP = FMM->RPL[k];
                int l = RPL.searchId(RP->getActuator()->getId());
                if(l >= RPL.getCount())
                    RPL.Add(FMM->RPL[k]);
            }
        }
    }
}

//Get the list of RPs included in a pair of MPs.
//Precondition:
//  All message of instruction in the MPs shall be addressed
//  to an existent RP of the Fiber MOS Model.
void getRPsIncludedInMPs(TRoboticPositionerList& RPL,
                         const TMotionProgram& MP1, const TMotionProgram& MP2,
                         const TFiberMOSModel* FMM)
{
    //CHECK THE PRECONDITION:

    if(FMM == NULL)
        throw EImproperArgument("pointer FMM should point to built Fiber MOS Model");

    for(int i=0; i<MP1.getCount(); i++) {
        const TMessageList *ML = MP1.GetPointer(i);
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);
            int k = FMM->RPL.searchId(MI->getId());
            if(k >= FMM->RPL.getCount())
                throw EImproperArgument("all message of instruction in the MP1 shall be addressed to an existent RP of the Fiber MOS Model");
        }
    }
    for(int i=0; i<MP2.getCount(); i++) {
        const TMessageList *ML = MP2.GetPointer(i);
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);
            int k = FMM->RPL.searchId(MI->getId());
            if(k >= FMM->RPL.getCount())
                throw EImproperArgument("all message of instruction in the MP2 shall be addressed to an existent RP of the Fiber MOS Model");
        }
    }

    //ADD THE RPs TO THE RPL:

    //initialize the RPL
    RPL.Clear();

    //add the RPs avoiding repetitions
    for(int i=0; i<MP1.getCount(); i++) {
        const TMessageList *ML = MP1.GetPointer(i);
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);

            //search the identifier RP in the Fiber MOS Model
            int k = FMM->RPL.searchId(MI->getId());
            //if not has found the identifier Id
            if(k >= FMM->RPL.getCount())
                //indicates lateral effect
                throw EImpossibleError("lateral effect");
            //if has found the identifier Id
            else {
                //actualice the RPL avoiding repetitions
                TRoboticPositioner *RP = FMM->RPL[k];
                int l = RPL.searchId(RP->getActuator()->getId());
                if(l >= RPL.getCount())
                    RPL.Add(FMM->RPL[k]);
            }
        }
    }
    for(int i=0; i<MP2.getCount(); i++) {
        const TMessageList *ML = MP2.GetPointer(i);
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);

            //search the identifier RP in the Fiber MOS Model
            int k = FMM->RPL.searchId(MI->getId());
            //if not has found the identifier Id
            if(k >= FMM->RPL.getCount())
                //indicates lateral effect
                throw EImpossibleError("lateral effect");
            //if has found the identifier Id
            else {
                //actualice the RPL avoiding repetitions
                TRoboticPositioner *RP = FMM->RPL[k];
                int l = RPL.searchId(RP->getActuator()->getId());
                if(l >= RPL.getCount())
                    RPL.Add(FMM->RPL[k]);
            }
        }
    }
}

//###########################################################################
//TMotionProgramValidator:
//###########################################################################

//calculates the time free of collission of a RP and an EA
double TMotionProgramValidator::calculateTf(TRoboticPositioner *RP,
                   const TExclusionArea *EAA) const
{
    //CHECK THE PRECONDITIONS:

    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");
    if(EAA == NULL)
        throw EImproperArgument("pointer EAA should point to built exclusion area");

    //MAKE ACTIONS:

    //calcula la distancia entre los contornos
    double D = RP->getActuator()->distanceMin(EAA);
    //calcula la distancia libre
    double Df = D - RP->getActuator()->getArm()->getSPM() - EAA->Barrier.getSPM();

    //El calculo de la distancia entre contornos,
    //actualizará el vector de distancias mínimas.

/*    //actualize the minimun free distance of the RP
    if(D < RP->Dmin)
        RP->Dmin = D;
    //actualize the minimun free end distance of the RP
    if(D < RP->Dend)
        RP->Dend = D;
*/
    //Para que el método de vlaidación de MPs funciones, el tiempo libre debe poder ser negativo.
    //Por eso aquí no debe hacerse:
    //  //si los brazos colisionan entre si
    //  if(Df < 0)
    //      return 0; //indica que el tiempo libre es cero

    //calculates an upper top for longitudinal velocity
    double vmaxabs = RP->calculatevmaxabs();

    //calculates Tf
    double Tf;
    if(vmaxabs != 0)
        Tf = Df/vmaxabs;
    else
        Tf = DBL_MAX;

    //Note that when vmaxabs == 0, the Tf is infinity, even when Df == 0.

    return Tf; //return Tf
}
//calculates the time free of collission of two RPs
double TMotionProgramValidator::calculateTf(TRoboticPositioner *RP,
                                            const TRoboticPositioner *RPA) const
{
    //CHECK THE PRECONDITIONS:

    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");
    if(RPA == NULL)
        throw EImproperArgument("pointer RPA should point to built robotic positioner");

    //MAKE ACTIONS:

    //calcula la distancia entre los contornos
    double D = RP->getActuator()->distanceMin(RPA->getActuator());
    //calcula la distancia libre de los brazos
    double Df = D - RP->getActuator()->getArm()->getSPM() - RPA->getActuator()->getArm()->getSPM();

    //El calculo de la distancia entre contornos,
    //actualizará el vector de distancias mínimas.

/*    //actualize the minimun free distance of the RP
    if(D < RP->Dmin)
        RP->Dmin = D;
    //actualize the minimun free end distance of the RP
    if(D < RP->Dend)
        RP->Dend = D;
*/
    //Para que el método de vlaidación de MPs funciones, el tiempo libre debe poder ser negativo.
    //Por eso aquí no debe hacerse:
    //  //si los brazos colisionan entre si
    //  if(Df < 0)
    //      return 0; //indica que el tiempo libre es cero

    //calculates an upper top for longitudinal velocity
    double vmaxabs1 = RP->calculatevmaxabs();
    double vmaxabs2 = RPA->calculatevmaxabs();
    double vmaxabs = vmaxabs1 + vmaxabs2;

    //calculates Tf
    double Tf;
    if(vmaxabs != 0)
        Tf = Df/vmaxabs;
    else
        Tf = DBL_MAX;

    //Note that when vmaxabs == 0, the Tf is infinity, even when Df == 0.

    return Tf; //return Tf
}
//calculates the minimun step time of a pair (RP, EA)
double TMotionProgramValidator::calculateTmin(const TRoboticPositioner *RP,
                     const TExclusionArea *EAA) const
{
    //check the preconditions
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");
    if(EAA == NULL)
        throw EImproperArgument("pointer EAA should point to built exclusion area");


    //calculates the joint SPMmin
    double SPMmin = RP->getActuator()->getSPMmin();

    //calculates an upper level for longitudinal velocity
    double vmaxabs = RP->calculatevmaxabs();

    //calculates and return Tmin
    double Tmin;
    if(vmaxabs != 0)
        Tmin = SPMmin/vmaxabs;
    else
        Tmin = DBL_MAX;
    return Tmin;
}
//calculates the minimun step time of two RPs
double TMotionProgramValidator::calculateTmin(const TRoboticPositioner *RP,
                                              const TRoboticPositioner *RPA) const
{
    //check the preconditions
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");
    if(RPA == NULL)
        throw EImproperArgument("pointer RPA should point to built robotic positioner");


    //calculates the joint SPMmin
    double SPMmin = RP->getActuator()->getSPMmin() + RPA->getActuator()->getSPMmin();

    //calculates an upper level for longitudinal velocity
    double vmaxabs1 = RP->calculatevmaxabs();
    double vmaxabs2 = RPA->calculatevmaxabs();
    double vmaxabs = vmaxabs1 + vmaxabs2;

    //calculates and return Tmin
    double Tmin;
    if(vmaxabs != 0)
        Tmin = SPMmin/vmaxabs;
    else
        Tmin = DBL_MAX;
    return Tmin;
}

//calculates the minimun time free of collission of
//a RP with their adjacents
double TMotionProgramValidator::calculateTfmin(TRoboticPositioner *RP) const
{
    //check the precondition
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    //determines the Tf between each adjacent RP and select the minimun
    double Tfmin = DBL_MAX;
    for(int i=0; i<RP->getActuator()->AdjacentEAs.getCount(); i++) {
        TExclusionArea *EAA = RP->getActuator()->AdjacentEAs[i].EA;
        double Tf = calculateTf(RP, EAA);
        if(Tf < Tfmin)
            Tfmin = Tf;
    }
    for(int i=0; i<RP->getActuator()->AdjacentRPs.getCount(); i++) {
        TRoboticPositioner *RPA = RP->getActuator()->AdjacentRPs[i].RP;
        double Tf = calculateTf(RP, RPA);
        if(Tf < Tfmin)
            Tfmin = Tf;
    }

    return Tfmin;
}
//calculates the minimun step time of
//a RP with their adjacents
double TMotionProgramValidator::calculateTminmin(const TRoboticPositioner *RP) const
{
    //check the precondition
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    //determines the Tmin between each adjacent RP and select the minimun
    double Tminmin = DBL_MAX;
    for(int i=0; i<RP->getActuator()->AdjacentEAs.getCount(); i++) {
        TExclusionArea *EAA = RP->getActuator()->AdjacentEAs[i].EA;
        double Tmin = calculateTmin(RP, EAA);
        if(Tmin < Tminmin)
            Tminmin = Tmin;
    }
    for(int i=0; i<RP->getActuator()->AdjacentRPs.getCount(); i++) {
        TRoboticPositioner *RPA = RP->getActuator()->AdjacentRPs[i].RP;
        double Tmin = calculateTmin(RP, RPA);
        if(Tmin < Tminmin)
            Tminmin = Tmin;
    }

    return Tminmin;
}

//calculates the minimun time free of collission of
//the RPs of a list
double TMotionProgramValidator::calculateTfmin(const TRoboticPositionerList& RPL) const
{
    //minimun time free of the RPs of the RPL
    double Tfmin = DBL_MAX; //initialize Tfmin with its maximun possible value
    bool notcollision = true; //initialize the collision indicator flag

    //calculates the Tf for the RPs of the RPL and select the minimun
    int j = 0;
    while(j<RPL.getCount() && notcollision) {
        //point the indicated RP to facilitate its access
        TRoboticPositioner *RP = RPL.Get(j);

        //calculates the Tf for the RP with their adjacents and select the minimun
        int k = 0;
        while(k<RP->getActuator()->AdjacentEAs.getCount() && notcollision) {
            //point the indicated adjacent RP to facilitateits access
            TExclusionArea *EAA = RP->getActuator()->AdjacentEAs[k].EA;

            //calculates the free time between the RP and its adjacent
            double Tf = calculateTf(RP, EAA);

            //if there is collision
            if(Tf < 0) {
                //indicates that the minimun free time is zero
                return Tf;

            } else { //else, if there isn't collision
                //actualize the minimun free time
                if(Tf < Tfmin)
                    Tfmin = Tf;

                k++; //indicates to the next adjacent
            }

            //Note that collision is detected when Tf < 0, and not <=.
        }

        //calculates the Tf for the RP with their adjacents and select the minimun
        k = 0;
        while(k<RP->getActuator()->AdjacentRPs.getCount() && notcollision) {
            //point the indicated adjacent RP to facilitateits access
            TRoboticPositioner *RPA = RP->getActuator()->AdjacentRPs[k].RP;

            //calculates the free time between the RP and its adjacent
            double Tf = calculateTf(RP, RPA);

            //if there is collision
            if(Tf < 0) {
                //indicates that the minimun free time is zero
                return Tf;

            } else { //else, if there isn't collision
                //actualize the minimun free time
                if(Tf < Tfmin)
                    Tfmin = Tf;

                k++; //indicates to the next adjacent
            }

            //Note that collision is detected when Tf < 0, and not <=.
        }

        //indicates to the next RP of the RPL
        j++;
    }

    //return the minimun free time
    return Tfmin;
}

//calculates the minimun step time of
//the RPs of a list
double TMotionProgramValidator::calculateTminmin(const TRoboticPositionerList& RPL) const
{
    //minimun time free of the RPs of the RPL
    double Tminmin = DBL_MAX; //initialize Tminmin with its maximun possible value
    bool notcollision = true; //initialize the collision indicator flag

    //calculates the Tmin for the RPs of the RPL and select the minimun
    int j = 0;
    while(j<RPL.getCount() && notcollision) {
        //point the indicated RP to facilitate its access
        TRoboticPositioner *RP = RPL.Get(j);

        //calculates the Tmin for the RP with their adjacents and select the minimun
        int k = 0;
        while(k<RP->getActuator()->AdjacentEAs.getCount() && notcollision) {
            //point the indicated adjacent RP to facilitateits access
            TExclusionArea *EAA = RP->getActuator()->AdjacentEAs[k].EA;

            //calculates the free time between the RP and its indicated adjacent
            double Tmin = calculateTmin(RP, EAA);

            //if there is collision
            if(Tmin < 0) {
                //indicates that the minimun step time is zero
                return Tmin;

            } else { //else, if there isn't collision
                //actualize the minimun step time
                if(Tmin < Tminmin)
                    Tminmin = Tmin;

                k++; //indicates to the next adjacent
            }

            //Note that collision is detected when Tmin < 0, and not <=.
        }

        //calculates the Tmin for the RP with their adjacents and select the minimun
        k = 0;
        while(k<RP->getActuator()->AdjacentRPs.getCount() && notcollision) {
            //point the indicated adjacent RP to facilitateits access
            TRoboticPositioner *RPA = RP->getActuator()->AdjacentRPs[k].RP;

            //calculates the free time between the RP and its indicated adjacent
            double Tmin = calculateTmin(RP, RPA);

            //if there is collision
            if(Tmin < 0) {
                //indicates that the minimun step time is zero
                return Tmin;

            } else { //else, if there isn't collision
                //actualize the minimun step time
                if(Tmin < Tminmin)
                    Tminmin = Tmin;

                k++; //indicates to the next adjacent
            }

            //Note that collision is detected when Tmin < 0, and not <=.
        }

        //indicates to the next RP of the RPL
        j++;
    }

    //return the minimun free time
    return Tminmin;
}

//---------------------------------------------------------------------------
//BUILDING AND DESTROYING METHODS:

//built a validator of motion programs
//attached to an extern Fiber MOS Model
TMotionProgramValidator::TMotionProgramValidator(TFiberMOSModel *FiberMOSModel)
{
    //el puntero FiberMOSModel debería apuntar a una lista de posicioandores de fibra construida
    if(FiberMOSModel == NULL)
        throw EImproperArgument("pointer FiberMOSModel should point to built Fiber MOS Model");

    //apunta los objetos externos
    p_FiberMOSModel = FiberMOSModel;
}

//---------------------------------------------------------------------------
//METHODS TO VALIDATE MOTION PROGRAMS:

//Determines if the execution of a motion program, starting from
//given initial positions, avoid collisions.
//Preconditions:
//  All RPs included in the MP:
//      must be enabled the quantifiers of their rotors.
//  All RPs of the FMM:
//      shall be in their initial positions;
//      must have the adecuate SPM.
//Postconditions:
//  If the MP produces a collision, all RPs of the FMM:
//      will have disabled the quantifiers of their rotors;
//      will have stacked the initial status of the quantifiers of their rotors;
//      will be in the first position where collission was detected.
//  If the MP avoid collisions, all RPs of the FMM:
//      will have the quantifiers of their rotors in their initial status;
//      will be in their final positions.
//Inputs:
//  MP: motion program to be validated
//Outputs:
//  validateMotionProgram: flag indicating if the motion program
//      avoid collisions.
//Notes:
//- The validation process of a MP consume a component of the SPM, even
//  when the process is successfully passed. So if a MP pass the validation
//  process with a value of SPM, the validation shall be make with
//  the value of SPM inmediately lower.
//- The validation method of a MP will be used during the generation process
//  with the individual MP of each RP, and at the end of the process for
//  validate the generated recovery program.
bool TMotionProgramValidator::validateMotionProgram(TMotionProgram &MP) const
{
    //CHECK THE PRECONDITIONS:

    //get the list of RPs included in the MP
    TRoboticPositionerList RPL;
    getRPsIncludedInMP(RPL, MP, getFiberMOSModel());

    for(int i=0; i<RPL.getCount();  i++) {
        TRoboticPositioner *RP = RPL[i];
        if(RP->getActuator()->getQuantify_()!=true || RP->getActuator()->getArm()->getQuantify___()!=true)
            throw EImproperCall("all RPs included in the MP, should be enabled the quantifiers of their rotors");
    }

    //CONFIGURES ALL RPs OF THE Fiber MOS Model:

    //stack the initial status of the quantifiers of the rotors
    getFiberMOSModel()->RPL.pushQuantifys();
    //disable the quantifiers
    getFiberMOSModel()->RPL.setQuantifys(false, false);

    //SOLVE THE TRIVIAL CASE:

    //determines if there is a collision
    //in the RPs included in the MP
    bool collision = RPL.thereIsCollision();
    //solve the trivial case
    if(collision)
        //indicates that the motion program not avoid dynamic collision
        return false;

    //CHECK THE FOLLOWING STEPPING POSITIONS TO END:

    //search a collision in each gesture
    for(int i=0; i<MP.getCount(); i++) {
        TMessageList *ML = MP.GetPointer(i);

        //program the gesture
        getFiberMOSModel()->RPL.clearInstructions();
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);
            getFiberMOSModel()->RPL.setInstruction(MI->getId(), MI->Instruction);
        }

        //reset the parameter Dmin of all RPs of the FMM
        for(int i=0; i<getFiberMOSModel()->RPL.getCount(); i++) {
            TRoboticPositioner *RP = getFiberMOSModel()->RPL[i];
///            RP->Dmin = DBL_MAX  ;
///            RP->Dend = DBL_MAX  ;
            RP->getActuator()->AdjacentEAs.setAllDmins(DBL_MAX);
            RP->getActuator()->AdjacentEAs.setAllDends(DBL_MAX);
            RP->getActuator()->AdjacentRPs.setAllDmins(DBL_MAX);
            RP->getActuator()->AdjacentRPs.setAllDends(DBL_MAX);
        }

        //The parameter Dmin of the RPs included in the MP,
        //will be updated in the method:
        //  Tfmin = calculateTfmin(RPL);
        //The obtained valued shall be transcribed from the RPs
        //of the RPL to the corresponding MIs of the ML.

        //Note that only will be used the parameter Dmin
        //of the RPs included in the MP, but is convenient
        //reset the parameter Dmin of all RPs of the FMM.

        //EXECUTE THE GESTURE:

        double Tfmin; //minimun free time

        //get the time of displacement
        double Tdis = getFiberMOSModel()->RPL.getTdis();
        //initialize the simulation time
        double t = 0;
        //while has not reached the end
        while(t < Tdis) {
            //move the rotors of the RPs to time t
            getFiberMOSModel()->RPL.move(t);

            //calculates the minimun free time of the RPL
            Tfmin = calculateTfmin(RPL);

            //Calculus of Tfmin pruduces update of (Dmin, Dend) of the RPs of the RPL.

            //if there is collision
            if(Tfmin < 0) {
                //transcript the Dmin of the RPs to corresponding MIs
                //and reset them
                for(int i=0; i<ML->getCount(); i++) {
                    TMessageInstruction *MI = ML->GetPointer(i);
                    int j = RPL.searchId(MI->getId());
                    if(j >= RPL.getCount())
                        throw EImpossibleError("lateral effect");
                    TRoboticPositioner *RP = RPL[j];
                    MI->setComment2(RP->getDminsText().str);

                    //For a single value, you can write:
                    //  MI->setComment2("Dmin = "+floattostr(RP->Dmin));
                }
                //indicates that the motion program not avoid dynamic collision
                return false;
            }

            //Note that collision is detected when Tfmin < 0, and not <=,
            //becuase Tfmin can not be less zero.

            //calculates and applies the minimun jump time of the RPL
            double Tmin = calculateTminmin(RPL);
            if(Tfmin < Tmin)
                Tfmin = Tmin;

            //advance simulation
            t += Tfmin;

            //NOTE: numerical error accumulated in t is eclipsed by
            //the additional margins of Tfmin. Tfmin has additional margins
            //due to the trajectory of all points of the armis circular.

        } //while(t < Tdis);

        //move the rotors of the RPs to final positions
        getFiberMOSModel()->RPL.moveFin();

        //calculates the minimun free time
        Tfmin = calculateTfmin(RPL);

        //Calculus of Tfmin pruduces update of (Dmin, Dend) of the RPs of the RPL.

        //transcript the (Dmin, Dend) of the RPs to corresponding MIs
        //and reset them
        for(int i=0; i<ML->getCount(); i++) {
            TMessageInstruction *MI = ML->GetPointer(i);
            int j = RPL.searchId(MI->getId());
            if(j >= RPL.getCount())
                throw EImpossibleError("lateral effect");
            TRoboticPositioner *RP = RPL[j];
            MI->setComment2(RP->getDminsText().str);
            MI->setComment3(RP->getDendsText().str);

            //For a single value, you can write:
            //  MI->setComment2("Dmin = "+floattostr(RP->Dmin));
            //  MI->setComment3("Dend = "+floattostr(RP->Dend));
        }

        //if there is collision
        if(Tfmin < 0)
            //indicates that the motion program not avoid dynamic collision
            return false;

        //Note that collision is detected when Tfmin < 0, and not <=,
        //becuase Tfmin can not be less zero.
    }

    //restore and discard the initial status of the quantifiers of the rotors
    getFiberMOSModel()->RPL.restoreAndPopQuantifys();

    //indicates that motion program avoid dynamic collision
    return true;
}

//Validate a pair (PP, DP) in a limited way.
//Inputs:
//- (PP, DP):the pair to validate.
//Outputs:
//- checkPairPPDP: flag indicating if all RPs includes in the pair
//  are operatives.
//Preconditions:
//- The status of the Fiber MOS Model must correspond to the status of
//  the real Fiber MOS.
bool TMotionProgramValidator::checkPairPPDP(const TMotionProgram &PP,
                                            const TMotionProgram &DP) const
{
    try {
        //gets the list of RPs in the pair (PP, DP)
        TRoboticPositionerList RPL;
        getRPsIncludedInMPs(RPL, PP, DP, getFiberMOSModel());

        //check if all RPs included are enabled
        for(int i=0; i<RPL.getCount(); i++) {
            TRoboticPositioner *RP = RPL[i];
            if(!RP->getOperative())
                return false;
        }

        //indicates that all RPs included are enabled
        return true;

    } catch(Exception& E) {
        E.Message.Insert(1, "checking pair (PP, DP): ");
        throw;
    }
}

//---------------------------------------------------------------------------

} //namespace Positiong

//---------------------------------------------------------------------------

