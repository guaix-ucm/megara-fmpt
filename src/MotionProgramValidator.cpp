//---------------------------------------------------------------------------
//Archivo: MotionProgramValidator.cpp
//Contenido: validador de programas de movimiento
//Última actualización: 14/08/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "MotionProgramValidator.h"

//#include <values.h>

//--------------------------------------------------------------------------

//espacio de nombres de posicionamiento
namespace Positioning {

//###########################################################################
//TMotionProgramValidator:
//###########################################################################

//--------------------------------------------------------------------------
//BUILDING AND DESTROYING METHODS:

//built a validator of motion programs
//attached to an extern Fiber MOS Model
TMotionProgramValidator::TMotionProgramValidator(TFiberMOSModel *_FiberMOSModel)
{
    //el puntero FiberMOSModel debería apuntar a una lista de posicioandores de fibra construida
    if(_FiberMOSModel == NULL)
        throw EImproperArgument("pointer FiberMOSModel should point to built RPs list");

    //apunta los objetos externos
    __FiberMOSModel = _FiberMOSModel;
}

//---------------------------------------------------------------------------
//METHODS TO VALIDATE MOTION PROGRAMS:

//calculates the time free of collission of two RPs
double TMotionProgramValidator::calculateTf(const TRoboticPositioner *RP,
                                            const TRoboticPositioner *RPA) const
{
    //pointer RP should point to built RP
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built RP");
    //pointer RPA should point to built RP
    if(RPA == NULL)
        throw EImproperArgument("pointer RPA should point to built RP");

    //calcula la distancia entre los contornos de los brazos de los RPs
    double D = RP->getActuator()->getArm()->getContour().DistanceMin(RPA->getActuator()->getArm()->getContour());
    //calcula la distancia libre de los brazos
    double Df = D - RP->getActuator()->getArm()->getSPM() - RPA->getActuator()->getArm()->getSPM();

    //si los brazos colisionan entre si
    if(Df < 0)
        return 0; //indica que el tiempo libre es cero

    /*        //CALCULA UNA COTA SUPERIOR PARA EL DESPLAZAMIENTO LONGITUDINAL:

        //cota superior de la aproximación (limitada inferiormente)
        double atop = Max(Df, RP->getActuator()->getSPMmin() + RPA->getActuator()->getSPMmin());
        //cota superio del desplazamiento longitudinal de cada posicionador
        double dtop = atop/2;

        //-------------------------------------------------------------------
        //CALCULA EL TIEMPO LIBRE DEL POSICIONADOR RP:

        double Tf1; //tiempo libre del posicionador RP
        if(RP->CMF.getMF1()==NULL && RP->CMF.getMF2()==NULL) //si no se mueve ningún eje
                Tf1 = MAXDOUBLE;//el tiempo libre es infinito
        else { //si se mueve algún eje
                //calcula la velocidad lineal máxima en mm/ms del ápice del brazo
                double vmax;
                if(RP->CMF.getMF1()!=NULL && RP->CMF.getMF2()==NULL) { //si solo se mueve el eje 1
                        //calcula la velocidad angular del eje 1 en rad/ms
                        double w1 = RP->CMF.getMF1()->getvmaxabs()/RP->getActuator()->getSB1()*M_2PI;
                        //calcula la velocidad lineal máxima en mm/ms
                        vmax = w1*RP->getActuator()->getr_max();
                }
                else if(RP->CMF.getMF1()==NULL && RP->CMF.getMF2()!=NULL) { //si solo se mueve el eje 2
                        //calcula la velocidad angular del eje 2 en rad/ms
                        double w2 = RP->CMF.getMF2()->getvmaxabs()/RP->getActuator()->getArm()->getSB2()*M_2PI;
                        //calcula la velocidad lineal máxima en mm/ms
                        vmax = w2*RP->getActuator()->getArm()->getL1V();
                }
                else { //si se mueven ambos ejes
                        //calcula la velocidad angular del eje 1 en rad/ms
                        double w1 = RP->CMF.getMF1()->getvmaxabs()/RP->getActuator()->getSB1()*M_2PI;
                        //calcula la velocidad angular del eje 2 en rad/ms
                        double w2 = RP->CMF.getMF2()->getvmaxabs()/RP->getActuator()->getArm()->getSB2()*M_2PI;
                        //calcula la velocidad lineal máxima en mm/ms
                        vmax = w1*RP->getActuator()->getr_max() + w2*RP->getActuator()->getArm()->getL1V();
                }

                //calcula el tiempo libre en ms
                if(vmax != 0)
                        Tf1 = dtop/vmax;
                else
                        Tf1 = MAXDOUBLE;
        }

        //-------------------------------------------------------------------
        //CALCULA EL TIEMPO LIBRE DEL POSICIONADOR RPA:

        double Tf2; //tiempo libre del posicionador RPA
        if(RPA->CMF.getMF1()==NULL && RPA->CMF.getMF2()==NULL) //si no se mueve ningún eje
                Tf2 = MAXDOUBLE;//el tiempo libre es infinito
        else { //si se mueve algún eje
                //calcula la velocidad lineal máxima en mm/ms delápice del brazo
                double vmax;
                if(RPA->CMF.getMF1()!=NULL && RPA->CMF.getMF2()==NULL) { //si solo se mueve el eje 1
                        //calcula la velocidad angular del eje 1 en rad/ms
                        double w1 = RPA->CMF.getMF1()->getvmaxabs()/RPA->getActuator()->getSB1()*M_2PI;
                        //calcula la velocidad lineal máxima en mm/ms
                        vmax = w1*RPA->getActuator()->getr_max();
                }
                else if(RPA->CMF.getMF1()==NULL && RPA->CMF.getMF2()!=NULL) { //si solo se mueve el eje 2
                        //calcula la velocidad angular del eje 2 en rad/ms
                        double w2 = RPA->CMF.getMF2()->getvmaxabs()/RPA->getActuator()->getArm()->getSB2()*M_2PI;
                        //calcula la velocidad lineal máxima en mm/ms
                        vmax = w2*RPA->getActuator()->getArm()->getL1V();
                }
                else { //si seueven ambos ejes
                        //calcula la velocidad angular del eje 1 en rad/ms
                        double w1 = RPA->CMF.getMF1()->getvmaxabs()/RPA->getActuator()->getSB1()*M_2PI;
                        //calcula la velocidad angular del eje 2 en rad/ms
                        double w2 = RPA->CMF.getMF2()->getvmaxabs()/RPA->getActuator()->getArm()->getSB2()*M_2PI;
                        //calcula la velocidad lineal máxima en mm/ms
                        vmax = w1*RPA->getActuator()->getr_max() + w2*RPA->getActuator()->getArm()->getL1V();
                }

                //calcula el tiempo libre en ms
                if(vmax != 0)
                        Tf2 = dtop/vmax;
                else
                        Tf2 = MAXDOUBLE;
        }

        //-------------------------------------------------------------------

        //NOTA: no es necesario calcular el límite superior
        //del desplazamiento longitudinal, porque si el tiempo libre sale
        //muy grande, las funciones de movimiento llevarán
        //los brazos a sus posiciones finales.

        //CALCULA Y DEVUELVE EL TIEMPO DE DESPLAZAMIENTO LIBRE DE COLISIÓN:

        //selecciona el tiempo libre menor
        double Tf = Min(Tf1, Tf2);
        //devuelve el tiempo libre
        return Tf;*/

    //calculates an upper top for longitudinal velocity
    double vmaxabs1 = RP->calculatevmaxabs();
    double vmaxabs2 = RPA->calculatevmaxabs();
    double vmaxabs = vmaxabs1 + vmaxabs2;

    //calculates and return Tf
    double Tf;
    if(vmaxabs != 0)
        Tf = Df/vmaxabs;
    else
        Tf= MAXDOUBLE;
    return Tf;
}
//calculates the minimun step time of two RPs
double TMotionProgramValidator::calculateTmin(const TRoboticPositioner *RP,
                                              const TRoboticPositioner *RPA) const
{
    //pointer RP should point to built RP
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built RP");
    //pointer RPA should point to built RP
    if(RPA == NULL)
        throw EImproperArgument("pointer RPA should point to built RP");


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
        Tmin = MAXDOUBLE;
    return Tmin;
}

//calculates the minimun time free of collission of
//a RP with their adjacents
double TMotionProgramValidator::calculateTfmin(const TRoboticPositioner *RP) const
{
    //pointer RP should point to built robotic positioner
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built RP");

    //determines the Tf between each adjacent RP and select the minimun
    double Tfmin = MAXDOUBLE;
    for(int i=0; i<RP->getActuator()->Adjacents.getCount(); i++) {
        TRoboticPositioner *RPA = RP->getActuator()->Adjacents[i];
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
    //pointer RP should point to built robotic positioner
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    //determines the Tmin between each adjacent RP and select the minimun
    double Tminmin = MAXDOUBLE;
    for(int i=0; i<RP->getActuator()->Adjacents.getCount(); i++) {
        TRoboticPositioner *RPA = RP->getActuator()->Adjacents[i];
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
    double Tfmin = MAXDOUBLE; //initialize Tfmin with its maximun possible value
    bool notcollision = true; //initialize the collision indicator flag

    //calculates the Tf for the RPs of the RPL and select the minimun
    int j = 0;
    while(j<RPL.getCount() && notcollision) {
        //point the indicated RP to facilitate its access
        TRoboticPositioner *RP = RPL.Get(j);

        //calculates the Tf for the RP with their adjacents and select the minimun
        int k = 0;
        while(k<RP->getActuator()->Adjacents.getCount() && notcollision) {
            //point the indicated adjacent RP to facilitateits access
            TRoboticPositioner *RPA = RP->getActuator()->Adjacents[k];

            //calculates the free time between the RP and its adjacent
            double Tf = calculateTf(RP, RPA);

            //if there is collision
            if(Tf <= 0) {
                //indicates that the minimun free time is zero
                return Tf;

            } else { //else, if there isn't collision
                //actualize the minimun free time
                if(Tf < Tfmin)
                   Tfmin = Tf;

                k++; //indicates to the next adjacent
            }
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
    double Tminmin = MAXDOUBLE; //initialize Tminmin with its maximun possible value
    bool notcollision = true; //initialize the collision indicator flag

    //calculates the Tmin for the RPs of the RPL and select the minimun
    int j = 0;
    while(j<RPL.getCount() && notcollision) {
        //point the indicated RP to facilitate its access
        TRoboticPositioner *RP = RPL.Get(j);

        //calculates the Tmin for the RP with their adjacents and select the minimun
        int k = 0;
        while(k<RP->getActuator()->Adjacents.getCount() && notcollision) {
            //point the indicated adjacent RP to facilitateits access
            TRoboticPositioner *RPA = RP->getActuator()->Adjacents[k];

            //calculates the free time between the RP and its indicated adjacent
            double Tmin = calculateTmin(RP, RPA);

            //if there is collision
            if(Tmin <= 0) {
                //indicates that the minimun step time is zero
                return Tmin;

            } else { //else, if there isn't collision
                //actualize the minimun step time
                if(Tmin < Tminmin)
                   Tminmin = Tmin;

                k++; //indicates to the next adjacent
            }
        }
        //indicates to the next RP of the RPL
        j++;
    }

    //return the minimun free time
    return Tminmin;
}

//Get the list of RPs includes in a MP.
//Precondition:
//  All message of instruction in the MP shall be addressed
//  to an existent RP of the Fiber MOS Model.
void TMotionProgramValidator::getRPsIncludedInMP(TRoboticPositionerList& RPL,
                                                 const TMotionProgram& MP) const
{
    //CHECK THE PRECONDITION:

    //for each message instruction list of the motion program
    for(int i=0; i<MP.getCount(); i++) {
        //point the message instruction list to facilitate its access
        const TMessageList *ML = MP.GetPointer(i);

        //for each message of instruction
        for(int j=0; j<ML->getCount(); j++) {
            //point themessage of instruction to facilitate its access
            const TMessageInstruction *MI = ML->GetPointer(j);
            //search the identified RP in the Fiber MOS Model
            int k = getFiberMOSModel()->RPL.SearchId(MI->getId());
            //if not has found the identified RP
            if(k >= getFiberMOSModel()->RPL.getCount())
                //indicates that precontition is not accomplished
                throw EImproperArgument("all message of instruction in the MP shall be addressed to an existent RP of the Fiber MOS Model");
        }
    }

    //ADD THE RPs TO THE RPL:

    //initialize the RPL
    RPL.Clear();

    //for each message instruction list of the motion program
    for(int i=0; i<MP.getCount(); i++) {
        //point the message instruction list to facilitate its access
        const TMessageList *ML = MP.GetPointer(i);

        //for each message of instruction
        for(int j=0; j<ML->getCount(); j++) {
            //point themessage of instruction to facilitate its access
            const TMessageInstruction *MI = ML->GetPointer(j);
            //search the identifier RP in the Fiber MOS Model
            int k = getFiberMOSModel()->RPL.SearchId(MI->getId());
            //if not has found the identifier Id
            if(k >= getFiberMOSModel()->RPL.getCount())
                //indicates lateral effect
                throw EImpossibleError("lateral effect");
            else
                RPL.Add(getFiberMOSModel()->RPL[k]);
        }
    }
}

//Determines if the execution of a motion program, starting from
//given initial positions, produce some dynamic collision.
//Preconditions:
//  All RPs ofthe Fiber MOS Model shall be in their initial positions
//Posconditions:
//  All RPs of the Fiber MOS Model will be configured for MP validation
//  All RPs of the fiber MOS Model will be in their final positions
//Inputs:
//  MP: motion program
//Outputs:
//  motionProgramIsntValid: flag indicating if the motion program
//      produces dynamic collision:
//          false: motion program avoid dynamic collision
//          true: motion program not avoid dynamic collision
bool TMotionProgramValidator::motionProgramIsntValid(const TMotionProgram &MP) const
{
    //CONFIGURES ALL RPs OF THE Fiber MOS Model FOR VALIDATION:

    //configure the Fiber MOS Model for validation
    getFiberMOSModel()->RPL.SetPurpose(pVal);

    //CHECK THE INITIAL POSITIONS:

    //get the list of RPs included in the MP
    TRoboticPositionerList RPL;
    getRPsIncludedInMP(RPL, MP);

    //determines if there is a collision
    //in the RPs included in the MP
    bool collision = RPL.ThereIsCollision();
    //if there is a collision
    if(collision)
        //indicates that the motion program notavoid dynamic collision
        return true;

    //CEHCK THE FOLLOWING STEPPING POSITIONS TO END:

    //for each message of instruction list of the motion program
    for(int i=0; i<MP.getCount(); i++) {
        //point the indicated message list to facilitate its access
        const TMessageList *ML = MP.GetPointer(i);

        //PROGRAM THE GESTURE:

        //clear theprograms of all RPs
        getFiberMOSModel()->RPL.ClearInstructions();
        //set the instructions to their respectives RPs
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *M = ML->GetPointer(j);
            getFiberMOSModel()->RPL.SetInstruction(M->getId(), M->Instruction);
        }

        //EXECUTE THE GESTURE:

        double Tfmin; //minimun free time

        //initialize the simulation time
        double t = 0;
        //get the time of displacement
        double Tdis = getFiberMOSModel()->RPL.getTdis();
        //while there is time of displacement
        while(t < Tdis) {
            //calculates the minimun free time
            Tfmin = calculateTfmin(RPL);
            //if there is collision
            if(Tfmin <= 0)
                //indicates that the motion program produces a collision
                return true;
            //apply the lower bound for displacement
            double Tmin = calculateTminmin(RPL);
            if(Tfmin < Tmin)
                Tfmin = Tmin;

            //advance simulation
            t += Tfmin;

            //NOTA: numerical error accumulated in t is aclipsed by
            //the margins of Tfmin.

            //move the rotors of the RPs to time t
            getFiberMOSModel()->RPL.Move(t);

        } while(t < Tdis);

        //calculates the minimun free time
        Tfmin = calculateTfmin(RPL);
        //if there is collision
        if(Tfmin <= 0)
            //indicates that the motion program not avoid collision
            return true;
    }

    //indicates that motion program avoid dynamic collision
    return false;
}

//--------------------------------------------------------------------------

} //namespace Positiong

//---------------------------------------------------------------------------

