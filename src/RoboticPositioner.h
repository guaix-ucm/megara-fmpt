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
//File: RoboticPositioner.h
//Content: robotic positioner (RP) model
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ROBOTICPOSITIONER_H
#define ROBOTICPOSITIONER_H

#include "Actuator.h"
#include "Instruction.h"
#include "ComposedMotionFunction.h"
#include "ItemsList.h"
#include "MotionProgram.h"

//---------------------------------------------------------------------------

using namespace MotionFunctions;

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//TControlMode

//Functioning mode of the controller:
//      cmSinc: programmed gestures will be inmediately executed;
//      cmAsinc: programmed gestures will be excuted when the accordant
//      instruction is received.
enum TControlMode {cmSinc, cmAsinc};

void  StrPrintControlMode(AnsiString& S,
        TControlMode cm);
void  StrReadControlMode(TControlMode& cm,
        const AnsiString &S, int &i);
AnsiString ControlModeToStr(TControlMode cm);
TControlMode StrToControlMode(const AnsiString& S);

//---------------------------------------------------------------------------
//TFaultType

//Fault type of the RP
//      ftUnk: unknown;
//      ftSta: static;
//      ftDyn: dynamic;
enum TFaultType {ftUnk, ftSta, ftDyn};

void  StrPrintFaultType(AnsiString& S,
        TFaultType ft);
void  StrReadFaultType(TFaultType& ft,
        const AnsiString &S, int &i);
AnsiString FaultTypeToStr(TFaultType ft);
TFaultType StrToFaultType(const AnsiString& S);

//---------------------------------------------------------------------------
//TRoboticPositioner

//Class TRoboticPositioner; description:
//
//Un objeto de la clase TRoboticPositioner contiene un actuador y
//una función de movimiento compuesta.
//Entonces un gesto puede ser programado para el actuador, obteniendo
//la función de movimeinto compuesto para ambos rotores, mediante:
//  - los métodos que programan gestos;
//  - asignando el comando en formato de cadena de bytes;
//Una vez programado el gesto puede ser ejecutado mediante los métodos
//de movimeinto tantas veces como se desee.
//
//Nótese que no es necesario cambiar las propiedades del gesto de manera
//independiente.
//
//Durante la ejecución del porgrama las instrucciones deben ser traducidas
//a cadenas de bytes que deberán ser entramadas para su envío, de modo que
//conviene poder programar los gestos a partir de dichas cadenas.
//
//Durante la programación habrá que programar gestos cuyos comandos no
//serán entramados, de modo que conviene poder programarlos directamente.
//
//Por último la configuración del gesto puede ser editada mediante
//formularios, por lo que conviene poder acceder a la escritura de
//cada parámetro por separado para simplificar el tratamiento.

//Lista de gestos fundamentales:
//    None, //no gira ningún rotor
//    M1, //gira el cilindro hasta p_1
//    M2, //gira el brazo hasta p___3
//    MM, //gira el brazo y el cilindro hasta (p_1, p___3),

//Lista de gestos que requieren un cálculo previo:
//    turnCilinderTotheta_1, //gira el cilindro hasta theta_1
//    turnCilinderTotheta_2, //gira el cilindro hasta theta_2
//    turnCilinderTotheta_3, //gira el cilindro hasta theta_3
//
//    turnArmTotheta_2, //gira el brazo hasta theta_2
//    turnArmTotheta_3, //gira el brazo hasta theta_3
//    turnArmTor_2, //gira el brazo hasta r_2
//    turnArmTor_3, //gira el brazo hasta r_3
//    turnArmToSafeArea, //gira el brazo hasta el área de seguridad
//    turnArmToOrigin; //gira el brazo hasta el origen de coordenadas

//    goRadialTor_2, //desplaza P2 radialmente hasta r_2
//    goRadialToSafeArea, //desplaza P2 radialmente hasta que el brazo
//            //quede dentro del área de seguridad
//            //o hasta donde se pueda
//    goRadialToOrigin, //desplaza P2 radialmente hasta P0
//            //o hasta donde se pueda

//    goDirectlyToPolarP_3, //va directamente a (r_3, theta_3)
//    goDirectlyToCartesianP_3, //va directamente a (x_3, y_3)
//    goDirectlyToCartesianP3, //va directamente a (x3, y3)

//ADVERTENCIA: noconviene intergar las propiedades de TActuator en la clase
//TRoboticPositioner. De este odo se obtienen algunas ventajas:
//- Las propiedades de inicialización de TActuator pueden ser filtradas
//  antes de cotnstruir el actuador.
//- El método Copy de TActuator no tiene que ser redefinido, manteniendose
//  la encapsulación de las propiedades del actuador.

//class robotic positioner
class TRoboticPositioner {
protected:
        //MECHANICAL PROPERTIES:

        TActuator *p_Actuator;

        //TOLERANCES:

        double p_Eo;
        double p_Ep;

        double p_Tstop;
        double p_Tshiff;

        double p_SPMadd;

        //ESTATUS PROPERTIES:

        double p_FaultProbability;

        double p_DsecMax;
        double p_Dsec;

public:
        //STATIC PROPERTIES:

        //list of built RPs
        static TItemsList<TRoboticPositioner*> Builts;

        //list of built RPs is useful for:
        //- determine the identifiers which are repeated;
        //- get univocal identifiers;
        //- control integrity of the memory.

        //El objeto estático Builts debe ser contruido
        //antes de invocar al contructor mediante:
        //      TItemList<TRoboticPositioner*> TRoboticPositioner::Builts;
        //para ello esta sentencia deberá ser insertada en el mismo
        //archivo donde se invoca al constructor, antes de la invocación.
        //Si se insertase en este archivo o en su correspondiente .cpp,
        //sería invocado después de llamar al constructor.

        //MECHANICAL PROPERTIES:

        //actuator of the RP
        TActuator *getActuator(void) const {return p_Actuator;}

        //CONTROL PROPERTIES:

        //composed motion function
        TComposedMotionFunction CMF;

        //TOLERANCES:

        //orientation error margin in S0
        //must be nonnegative
        //default value: MEGARA_Eo rad
        double getEo(void) const {return p_Eo;}
        void setEo(double);
        //position error margin in S0
        //must be nonnegative
        //default value: MEGARA_Ep mm
        double getEp(void) const {return p_Ep;}
        void setEp(double);

        //maximun time betweem last storage of the position angles,
        //and rotors detention.
        //must be nonnegative
        //default value: MEGARA_Tstop ms
        double getTstop(void) const {return p_Tstop;}
        void setTstop(double);
        //maximun time shift between RPs in motion.
        //must be nonnegative
        //default value: MEGARA_Tshiff ms
        double getTshiff(void) const {return p_Tshiff;}
        void setTshiff(double);

        //SPM additional to add to SPMdyn
        //must be nonnegative
        //default value: MEGARA_SPMadd mm
        double getSPMadd(void) const {return p_SPMadd;}
        void setSPMadd(double);

        //STATUS PROPERTIES:

        //disabling status of the RP
        //default value: false
        bool Disabled;
        //probability of fault
        //shall be in [0, 1]
        //default value: 0
        double getFaultProbability(void) const {return p_FaultProbability;}
        void setFaultProbability(double);
        //type of fault:
        //  ftUnk: unknown
        //  ftSta: static
        //  ftDyn: dynamic
        //default value: ftUnk
        TFaultType FaultType;
        //Functioning mode of the controller:
        //  cmSinc: programmed gestures will be inmediately executed;
        //  cmAsinc: programmed gestures will be excuted when the accordant
        //  instruction is received.
        //defult value: cmSinc.
        TControlMode ControlMode;

        //NOTE: class TRoboticPositioner not has capacity for execution.
        //Execution of gestures, will be performed usually in RP lists.

        //NOTE: ControlMode value can be directly changed
        //using the instructions:
        //      "MS": set sinchronous mode;
        //      "MA": set asinchronous mode.

        //operativity status:
        //  true: FaultProbability<=0 && !Disabled
        //  false: FaultProbability>0 || Disabled
        bool getOperative(void) const {
            return getFaultProbability()<=0 && !Disabled;}

        //individual MP attached to the RP
        //for turn the rotor 1
        Positioning::TMotionProgram MPturn;
        //individual MP attached to the RP
        //for retract the arm
        Positioning::TMotionProgram MPretraction;
        //maximun additional security distance during retraction
        //must be nonnegative
        //default value: 1 mm
        double getDsecMax(void) const {return p_DsecMax;}
        void setDsecMax(double);
        //additional security distance during retraction
        //must be nonnegative
        //default value: DsecMax mm
        double getDsec(void) const {return p_Dsec;}
        void setDsec(double);

/*        //minimun real distance of the RP with their adjacents
        //default value: DBL_MAX
        double Dmin;
        //minimun real end distance of the RP with their adjacents
        //default value: DBL_MAX
        double Dend;
*/
        //------------------------------------------------------------------
        //PROPERTIES IN TEXT FORMAT:

        //MECHANICAL PROPERTIES IN TEXT FORMAT:

        //address in memory of property Actuator
        AnsiString getActuatorAddressText(void) const {
                return IntToHex(intptr_t(getActuator()));}

        //CONTROL PROPERTIES IN TEXT FORMAT:

        //address in memory of property CMF
        AnsiString getCMFAddressText(void) const {
                return IntToHex(intptr_t(&CMF));}

        //TOLERANCES IN TEXT FORMAT:

        AnsiString getEoText(void) const;
        void setEoText(const AnsiString&);
        AnsiString getEpText(void) const;
        void setEpText(const AnsiString&);

        AnsiString getTstopText(void) const;
        void setTstopText(const AnsiString&);
        AnsiString getTshiffText(void) const;
        void setTshiffText(const AnsiString&);

        AnsiString getSPMaddText(void) const;
        void setSPMaddText(const AnsiString&);

        //STATUS PROPERTIES IN TEXT FORMAT:

        AnsiString getDisabledText(void) const;
        void setDisabledText(const AnsiString&);
        AnsiString getFaultProbabilityText(void) const;
        void setFaultProbabilityText(const AnsiString&);
        AnsiString getFaultTypeText(void) const;
        void setFaultTypeText(const AnsiString&);
        AnsiString getControlModeText(void) const;
        void setControlModeText(AnsiString&);

        AnsiString getOperativeText(void) const;

        //SETS OF PROPERTIES IN TEXT FORMAT:

        //set of R/W security properties
        //in assign text format
        AnsiString getToleranceText(void) const;
        //set of R/W status properties
        //in assign text format
        AnsiString getStatusText(void) const;

        //set of all properties
        //in assign text format
        //AnsiString getAllText(void) const;

        //set of properties of the instance of the RP
        //in assign text format
        AnsiString getInstanceText(void) const;
        void setInstanceText(const AnsiString&);

        //set of minimun distances in text format
        AnsiString getDminsText(void) const;
        //set of final distances in text format
        AnsiString getDendsText(void) const;

        //------------------------------------------------------------------
        //STATIC METHODS:

        //compare the identifiers of two RPs
        static int  compareIds(TRoboticPositioner *FP1,
                TRoboticPositioner *FP2);

        //print the identifier of a RP
        static void  printId(AnsiString &S, TRoboticPositioner *FP);

        //Statics methods:
        //      CompareIds
        //      PrintId
        //will be pointed in the adjacent-RP list
        //to allow sort it, according the identifiers,
        //and to allow the printing of the identifiers.

        //print the value of the origin properties of a RP
        //  (Id, x0, y0, thetaO1)
        //to the end of a text string
        //in row text format
        static void  printOriginsRow(AnsiString& S,
                TRoboticPositioner *FP);
        //read the value of the origin properties of a RP
        //  (Id, x0, y0, thetaO1)
        //from the indicated position of a text string
        //in row text format
        static void  readOriginsRow(TRoboticPositioner *FP,
                const AnsiString& S, int &i);

        //print the value of the position properties of a RP
        //  (Id, x3, y3)
        //to the end of a text string
        //in row text format
        static void  printPositionP3Row(AnsiString& S,
                                        TRoboticPositioner *FP);
        //read the value of the position properties of a RP
        //  (Id, x3, y3)
        //from the indicated position of a text string
        //in row text format
        static void  readPositionP3Row(TRoboticPositioner* &FP,
                                       const AnsiString& S, int &i);

        //print the value of the position properties of a RP
        //  (Id, x_3, y_3)
        //to the end of a text string
        //in row text format
        static void  printPositionP_3Row(AnsiString& S,
                                        TRoboticPositioner *FP);
        //print the value of the position properties of a RP
        //  (Id, p_1, p___3)
        //to the end of a text string
        //in row text format
        static void  printPositionPPARow(AnsiString& S,
                                        TRoboticPositioner *FP);

        //lee una instancia en una cadena
        static void  readInstance(TRoboticPositioner* &FP,
                const AnsiString& S, int &i);

        //------------------------------------------------------------------
        //BUILDING, CLONATION, COPYING AND DESTROYING METHODS:

        //build a RP by default
        TRoboticPositioner(void);
        //build a RP with the indicated values
        //if Id is less 1 throw an exception EImproperArgument
        TRoboticPositioner(int Id, TDoublePoint P0,
                double thetaO1=MEGARA_thetaO1);

        //WARNING: duplicity of identifiers is allowed.

        //copy all tolerance properties of a RP
        void copyTolerance(const TRoboticPositioner*);
        //copy all status properties of a RP
        void copyStatus(const TRoboticPositioner*);

        //copy all properties of a RP
        void clone(const TRoboticPositioner*);
        TRoboticPositioner& operator=(const TRoboticPositioner&);

        //build a clon of a RP
        TRoboticPositioner(const TRoboticPositioner*);

        //copy all properties of a RP
        //except (P0, Id, Id1, Id2)
        void apply(const TRoboticPositioner*);

        //free the dynamic memory and delete the RP in Builts
        //if the RP isn't in Built throw EImproperCall
        ~TRoboticPositioner();

        //-------------------------------------------------------------------
        //METHODS TO CALCULATE THE RECOMMENDED VALUES OF THE SPM COMPONENTS:

        //SPM recovery in mm:
        //  SPMrec = (CMF.vmaxabs1*Actuator->rbs*Actuator->r_max +
        //      CMF.vmaxabs2*Actuator->Arm->rbs*Actuator->Arm->L1V)*Tstop;
        double SPMrec(void) const;

        //static SPM in mm:
        //  SPMsta = Eo*Actuator->r_max + Ep;
        double SPMsta(void) const;

        //SPMsta must be enough large to include:
        //- the mechanical error due to mechanization of the actuator;
        //- the position error of the RPs;
        //- the orientation error of the RPs;
        //- the positioning error due the to imprecisión of F(theta_1)
        //  and Arm->F(theta___3).

        //ADVERENCIA: no confundir (Eo, Ep) con (PAem, Pem); (Eo, Ep)
        //se refiere al error de posición y orientación al colocar
        //el posiciondor en el posicionador multifibra, tras la calibración.

        //NOTA: la alinealidad de las funciones G(p_1) y Arm->G(p___3)
        //no tiene por que implicar un eror de posición a lo largo de
        //la trayectoria de P3, ya que las posiciones p_1 y p___3 son
        //calculadas teniendo en cuenta dicha alinealidad.
        //De este modo solo habrá que tener en cuenta el salto más grande
        //de cada rotor para determinar la contribución de F en Ep.

        //dynamic SPM:
        //  SPMdyn = (CMF.vmaxabs1*Actuator->rbs*Actuator->r_max +
        //      CMF.vmaxabs2*Actuator->Arm->rbs*Actuator->Arm->L1V)*Tshiff +
        //      SPMadd;
        double SPMdyn(void) const;

        //SPMdyn must be enough large to include:
        //- temporal shiff between the start of the movements;
        //- motion funtion not correspond exactly the indicated in the model;
        //- ambiguity in position due the quantification of the rotors;
        //- asymmetrical radial motion due to motion velocity of rotor2
        //  is not double of velocity of rotor 1 in all moment.

        //-------------------------------------------------------------------
        //METHODS FOR JOINTLY SET:

        //asigna conjuntamente las tolerancias
        //      (Eo, Ep, Tstop, Tshiff, SPMadd)
        void setTolerances(double Eo, double Ep,
                           double Tstop, double Tshiff,
                           double SPMadd);

        //------------------------------------------------------------------
        //ASSIMILATION METHODS:

        //A partir de:
        //      (Eo, Ep, Tstop, Tshiff, SPMadd)
        //      rmax
        //Determina:
        //      (SPMrec, SPMsta, SPMdyn)
        void calculateSPMcomponents(void);

        //access to SPMmin thorug "getActuator()->".

        //asigna la componente de SPM para absorber el desplazamiento
        //por corrección del offset
        //      Actuator->SPMoff = PAem*Actuator->rmax + Pem mm
        void setSPMoff(double PAem, double Pem);

        //--------------------------------------------------------------------------
        //METHODS TO CHECK THE CONFIGURATION:

        //calculate the maximun absolute velocity of rotor 1 in rad/s
        double wmaxabs1(void);
        //calculate the maximun absolute velocity of rotor 2 in rad/s
        double wmaxabs2(void);

        //------------------------------------------------------------------
        //INSTRUCTION METHODS:

        //Gestures will be programmed according to the motion functions type
        //and the sinchronous mode of the functions.

        //program turn of rotor 1 from actual position to p_1fin
        //if rotor 1 quantifier is disabled:
        //  throw and exception EImproperCall
        //if the p_1fin is out rotor 1 domain:
        //  throw an exception EImproperArgument
        void M1(double p_1fin);
        //program turn of rotor 2 from actual position to p___3fin
        //if rotor 2 quantifier is disabled:
        //  throw and exception EImproperCall
        //if the p___3fin is out rotor 2 domain:
        //  throw an exception EImproperArgument
        void M2(double p___3fin);
        //program turns of rotors 1 and 2 from actual position to (p_1fin, p___3fin)
        //if rotor 1 quantifier or rotor 2 quantifier is disabled:
        //  throw and exception EImproperCall
        //if the p_1fin is out rotor 1 domain or p___3fin is out rotor 2 domain:
        //  throw an exception EImproperArgument
        void MM(double p_1fin, double p___3fin);

        //program turn of rotor 1 from p_1sta to p_1fin
        //if the p_1sta or p_1fin is out rotor 1 domain:
        //  throw an exception EImproperArgument
        void M1(double p_1sta, double p_1fin);
        //program turn of rotor 2 from p___3sta to p___3fin
        //if the p___3sta or p___3fin is out rotor 2 domain:
        //  throw an exception EImproperArgument
        void M2(double p___3sta, double p___3fin);
        //program turn of rotor 1 from p_1sta to p_1fin, and
        //program turn of rotor 2 from p___3sta to p___3fin
        //if the p_1sta or p_1fin is out rotor 1 domain, or:
        //if the p___3sta or p___3fin is out rotor 2 domain:
        //  throw an exception EImproperArgument
        void MM(double p_1sta, double p___3sta,
                double p_1fin, double p___3fin);

        //stop displacement and clear the programmed gesture
        //if there isn't programmed a gesture:
        //  throw an exception EImproperCall
        void SP(void);

        //set an instruction
        //if instruction is empty:
        //  throw an exception EImproperArgument
        void setInstruction( const TInstruction&);
        //get the programmed instruction
        //if there isn't progrmmed a gesture:
        //  throw an exception EImproperCall
        void getInstruction(TInstruction&) const;

        //get the instruction to move:
        //  rotor 1 from actual position to p_1first
        //  rotor 2 from actual position to p___3first
        void getInstructionToGoToTheOrigin(TInstruction&) const;
        //set the position indicated in an instruction
        void setPosition(const TInstruction&);

        //METHODS TO PROGRAM GESTURES:

        //program the abatement of the arm to
        //the more closer-stable security position
        void programTurnArmToSecurityPosition(void);
        //program the retraction of the arm to
        //the more closer-stable security position
        //if rotor 1 quantifier or rotor 2 quantifier is disabled:
        //  throw and exception EImproperCall
        void programRetractionToSecurityPosition(void);
        //program the extension of the fiber from the actual position
        //to the more spread position
        //if rotor 1 quantifier or rotor 2 quantifier is disabled:
        //  throw and exception EImproperCall
        void programExtensionToMoreSpreadPosition(void);

        //When the vmax of rotor 2 is the double that vmax of rotor 1,
        //the extension and retraction of the arm will be radial. And
        //when the rotor 1 is near of origin, part of the trajectory can
        //be circular around the rotor 2.

        //program turn of rotor 1 to theta_1
        //if theta_1 is out rotor 1 domain:
        //  throw an exception EImproperArgument
        void programTurnCilinderTotheta_1(double theta_1);

        //Other methods which could be useful:
        //
        //  void turnCilinderTotheta_2(double theta_2);
        //  void turnCilinderTotheta_3(double theta_3);
        //
        //  void turnArmTotheta_2(double theta_2);
        //  void turnArmTotheta_3(double theta_3);
        //  void turnArmTor_2(double r_2);
        //  void turnArmTor_3(double r_3);
        //  void turnArmToSafeArea(void);
        //  void turnArmToOrigin(void);
        //
        //  void goRadialTor_2(double r_2);
        //  void goRadialToSafeArea(void);
        //  void goRadialToOrigin(void);
        //
        //  void GoDirectlyToPolarP_3(double r_3, double theta_3);

        //program go directly from actual position to (x_3, y_3)
        //if (x_3, y_3) is out the scope of the RP:
        //  throw an exception EImproperArgument
        double programGoDirectlyToCartesianP_3(double x_3, double y_3);
        //program go directly from actual position to (x3, y3)
        //if (x3, y3) is out the scope of the RP:
        //  throw an exception EImproperArgument
        double programGoDirectlyToCartesianP3(double x3, double y3);

        //METHODS TO MODIFY THE PROGRAMMED GESTURE:

        //invert the programmed gesture in the time domain
        void invertTime(void);

        //METHODS TO ADD MESSAGE INSTRUCTIONS TO THE MP:

        //Propose in (MPturn, MPretraction) a recovery program composed by
        //one or two gestures:
        //  1. Radial retraction to where it is possible.
        //  2. Abatement of the arm to security position (if necessary).
        //Preconditions:
        //- The quantifiers of the rotors shall be enabled.
        //- The rotor 2 shall be in unsecurity position.
        //Postconditions:
        //- The MPturn will be empty.
        //- The MPretraction will contains the MP for retract the arm
        //  to the first stable security position.
        void proposeRecoveryProgram(void);

        //Propose (in MPturn, MPretraction) a recovery program composed by
        //two or three gestures:
        //  1. turn of the rotor 1 to p_1new.
        //  2. Radial retraction to where it is possible.
        //  3. Abatement of the arm to security position (if necessary).
        //Inputs:
        //  p_1new: new position where the rotor 1 shall be turned
        //      before the retraction.
        //Preconditions:
        //- The quantifiers of the rotors shall be enabled.
        //- The rotor 2 shall be in unsecurity position.
        //- The new rotor 1 position p_1new shall be in the rotor 1 domain.
        //Postconditions:
        //- The MPturn will contains the MP for turn the rotor 1
        //  to the position p_1new.
        //- The MPretraction will contains the MP for retract the arm
        //  to the first stable security position.
        void proposeRecoveryProgram(double p_1new);

        //METHODS TO DETERMINE MOTION PARAMETERS:

        //determina el valor absoluto de
        //el ángulo máximo recorrido al moverse el rotor 1 durante un tiempo T
        //      theta_MaxAbs(T) = Actuator->G(CMF.vmaxabs1)*T;
        double theta_MaxAbs(double T);
        //determina el valor absoluto de
        //la distancia máxima recorrida por un punto del brazo
        //al moverse el rotor 1 durante un tiempo T
        //      d_MaxAbs(T) = theta_MaxAbs(T)*Actuator->r_max;
        double d_MaxAbs(double T);

        //determina el valor absoluto de
        //el ángulo máximo recorrido al moverse el rotor 2 durante un tiempo T
        //      theta___MaxAbs(T) = Actuator->Arm->G.Image(CMF.vmaxabs2)*T;
        double theta___MaxAbs(double T);
        //determina el valor absoluto de
        //la distancia máxima recorrida por un punto del brazo
        //al moverse el rotor 2 durante un tiempo T
        //      d___MaxAbs(T) = theta___MaxAbs(T)*Actuator->Arm->Ra;
        double d___MaxAbs(double T);

        //determina una cota superior para la distancia máxima recorrida
        //por un punto del brazo del posicionador, al moverse ambos rotores
        //durante un intervalo de tiempo T
        //      dMaxAbs = max(d_MaxAbs(T), d___MaxAbs(T))
        double dMaxAbs(double T);

        //determine a upper top for longitudinal velocity
        //of all points of the arm
        double calculatevmaxabs(void) const;

        //calculates the displacement of rotor 1
        //  double theta_1 = getActuator()->theta_1s.getLast();
        //  double p_1 = getActuator()->getF().Image(theta_1);
        //  p_1 = Round(p_1);
        //  dp1 =  CMF.getMF1()->getpsta() - p_1;
        //preconditions:
        //  RP shall be stascked one position almost
        //  rotor 1 quantifier shall be enabled
        double dp1(void) const;

        //WARNING: the value of psta not involve numerical error, becuase
        //was calculated and assigned when the quantifier of the rotor
        //was enabled. but recover thes tacked position of rotor 1 in steps
        //could involves a numerical error introduced by the function F.

        //MOTION METHODS:

        //ADVERTENCIA: Para simular el movimeinto de los rotores correctamente
        //debe desactivarse la cuantificación de los rotores que se van a mover.
        //Esto queda bajo responsabilidad del programador.
        //Lo normal es que se desactiven ambos rotores, ya que el rotor
        //que no se va a desplazar debe quedar libre para su desplazameinto.

        //lleva los rotores del posicionador adscrito a
        //las posiciones correspondientes al instante t
        //sila cuantificación de los rotores que se van amover está activada
        //lanza una excepción EImproperCall
        void move(double t);
        //lleva los rotores del posicionador adscrito a
        //las posiciones correspondientes al instante 0
        //(MF1->psta, MF2->psta)
        void moveSta(void);
        //lleva los rotores del posicionador adscrito a
        //las posiciones correspondientes al instante CMF->tendmax
        //(MF1->pfin, MF2->pfin)
        void moveFin(void);

        //lleva los rotores del posicionador adscrito a las posiciones
        //correspondientes a después de ejecutar (MPturn, MPretraction)
        void moveFinMP(void);

        //desplaza los rotores hasta sus posiciones correspondientes
        //al instante t+qt, donde:
        //      t: instante de tiempo previo al actual
        //      qt: escalón de cuantificación
        //void Displace(double t, double qt);

        //si se invoca el método Displace(t, qt) reiteradamente
        //se acumulará el error de suscesivos desplazamientos en los rotores
        //para minimizar este error este método debería invocarse
        //con la cuantificación de los rotores del posicionador desactivada

        //Como el gesto mueve ambos rotores de manera independiente,
        //se puede prescindir del método Displace(t, qt)
        //P.e: cuando el brazo es abatido, es posible cambiar
        //la orientación del cilindro, y al invocar al método Move(t),
        //este conservará su dirección; cosa que no ocurriría si
        //el gesto hubiera definido una función constante para el rotor
        //que no se mueve, en cuyo caso volvería a su posición inicial.
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // ROBOTICPOSITIONER_H
