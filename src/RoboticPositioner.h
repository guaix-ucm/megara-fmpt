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
//File: RoboticPositioner.h
//Content: robotic positioner model
//Last update: 06/05/2014
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ROBOTICPOSITIONER_H
#define ROBOTICPOSITIONER_H

#include "Instruction.h"
#include "Actuator.h"
#include "ComposedMotionFunction.h"
#include "ItemsList.h"
#include "MotionProgram.h"

//---------------------------------------------------------------------------

using namespace MotionFunctions;

//namespace for models
namespace Models {

//##########################################################################
//TControlMode
//##########################################################################

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

//##########################################################################
//TFaultType
//##########################################################################

//Fault type of the RP
//      ftUnk: unknowledge;
//      ftSta: static;
//      ftDyn: dynamic;
enum TFaultType {ftUnk, ftSta, ftDyn};

void  StrPrintFaultType(AnsiString& S,
        TFaultType ft);
void  StrReadFaultType(TFaultType& ft,
        const AnsiString &S, int &i);
AnsiString FaultTypeToStr(TFaultType ft);
TFaultType StrToFaultType(const AnsiString& S);

//##########################################################################
//TRoboticPositioner
//##########################################################################

//Class TRoboticPositioner; description:
//
//Un objeto de la clase TRoboticPositioner contiene un actuador y
//una funciónde movimiento.
//Entonces un gesto puede ser programado para el actuador, obteniendo
//la función de movimeinto compuesto para ambos ejes, mediante:
//      - los métodos que programan gestos;
//      - asignando el comando en formato de cadena de bytes;
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
//Durante la programación habrá que programar gestos suyos comandos no
//serán entramados, de modo que conviene poder programarlos directamente.
//
//Por último la configuración del gesto puede ser editada mediante
//formularios, por lo que conviene poder acceder a la escritura de
//cada parámetro por separado para simplificar el tratamiento.

//Lista de gestos fundamentales:
//        None, //no gira ningún eje
//        M1, //gira el cilindro hasta p_1
//        M2, //gira el brazo hasta p___3
//        M, //gira el brazo y el cilindro hasta (p_1, p___3),

//Lista de gestos que requieren un cálculo previo:
//        TurnCilinderTotheta_1, //gira el cilindro hasta theta_1
//        TurnCilinderTotheta_2, //gira el cilindro hasta theta_2
//        TurnCilinderTotheta_3, //gira el cilindro hasta theta_3
//
//        TurnArmTotheta_2, //gira el brazo hasta theta_2
//        TurnArmTotheta_3, //gira el brazo hasta theta_3
//        TurnArmTor_2, //gira el brazo hasta r_2
//        TurnArmTor_3, //gira el brazo hasta r_3
//        TurnArmToSafeArea, //gira el brazo hasta el área de seguridad
//        TurnArmToOrigin; //gira el brazo hasta el origen de coordenadas

//        GoRadialTor_2, //desplaza P2 radialmente hasta r_2
//        GoRadialToSafeArea, //desplaza P2 radialmente hasta que el brazo
//                //quede dentro del área de seguridad
//                //o hasta donde se pueda
//        GoRadialToOrigin, //desplaza P2 radialmente hasta P0
//                //o hasta donde se pueda

//        GoDirectlyToPolarP_3, //va directamente a (r_3, theta_3)
//        GoDirectlyToCartesianP_3, //va directamente a (x_3, y_3)
//        GoDirectlyToCartesianP3, //va directamente a (x3, y3)

//ADVERTENCIA: para mantener el formalismo estricto se define TRoboticPositioner
//con una propiedad de la clase TActuator, evitándose intergar las propiedades
//de TActuator en la clase TRoboticPositioner. De este odo se obtienen algunas
//ventajas:
//- Las propiedades de inicialización de TActuator pueden ser filtradas antes de
//  cotnstruir el actuador.
//- El método Copy de TActuator no tiene que ser redefinido, manteniendose
//  la encapsulación de las propiedades del actuador.

//class robotic positioner
class TRoboticPositioner {
protected:
        //MECHANICAL PROPERTIES:

        TActuator *__Actuator;

        //TOLERANCES:

        double __Eo;
        double __Ep;

        double __Tstop;
        double __Tshiff;

        double __SPMadd;

        //ESTATUS PROPERTIES:

        double __FaultProbability;

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
        TActuator *getActuator(void) const {return __Actuator;}

        //CONTROL PROPERTIES:

        //composed motion function
        TComposedMotionFunction CMF;

        //TOLERANCES:

        //orientation error margin in S0
        //default value: MEGARA_Eo rad
        double getEo(void) const {return __Eo;}
        void setEo(double);
        //position error margin in S0
        //default value: MEGARA_Ep mm
        double getEp(void) const {return __Ep;}
        void setEp(double);

        //maximun time betweem last storage of the position angles,
        //and rotors detention.
        //default value: MEGARA_Tstop ms
        double getTstop(void) const {return __Tstop;}
        void setTstop(double);
        //maximun time shift between RPs in motion.
        //  default value: MEGARA_Tshiff ms
        double getTshiff(void) const {return __Tshiff;}
        void setTshiff(double);

        //SPM additional to add to SPMdyn
        //must be nonnegative
        //default value: 0.1 mm
        double getSPMadd(void) const {return __SPMadd;}
        void setSPMadd(double);

        //STATUS PROPERTIES:

        //disabling status of the RP
        //default value: false
        bool Disabled;
        //probability of fault
        //shall be in [0, 1]
        //default value: 0
        double getFaultProbability(void) const {return __FaultProbability;}
        void setFaultProbability(double);
        //type of fault:
        //  ftUnk: unknoledge
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
        //additional security distance during retraction
        //must be nonnegative
        //default value: 1 mm
        double getDsec(void) const {return p_Dsec;}
        void setDsec(double);

        //------------------------------------------------------------------
        //PROPERTIES IN TEXT FORMAT:

        //MECHANICAL PROPERTIES IN TEXT FORMAT:

        //address in memory of property Actuator
        AnsiString getActuatorAddressText(void) const {
                return IntToHex(reinterpret_cast<intptr_t>(getActuator()), 8);}

        //CONTROL PROPERTIES IN TEXT FORMAT:

        //address in memory of property CMF
        AnsiString getCMFAddressText(void) const {
                return IntToHex(reinterpret_cast<intptr_t>(&CMF), 8);}

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

        //set of all security properties
        //in assign text format
        AnsiString getToleranceText(void) const;
        //set ofall status properties
        //in assign text format
        AnsiString getStatusText(void) const;

        //set of all properties
        //in assign text format
        AnsiString getAllText(void) const;

        //set of properties of the instance of the RP
        //in assign text format
        AnsiString getInstanceText(void) const;
        void setInstanceText(const AnsiString&);

        //------------------------------------------------------------------
        //STATIC METHODS:

        //compare the identifiers of two RPs
        static int  CompareIds(TRoboticPositioner *FP1,
                TRoboticPositioner *FP2);

        //print the identifier of a RP
        static void  PrintId(AnsiString &S, TRoboticPositioner *FP);

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
        static void  PrintOriginsRow(AnsiString& S,
                TRoboticPositioner *FP);
        //read the value of the origin properties of a RP
        //  (Id, x0, y0, thetaO1)
        //from the indicated position of a text string
        //in row text format
        static void  ReadOriginsRow(TRoboticPositioner *FP,
                const AnsiString& S, int &i);

        //print the value of the position properties of a RP
        //  (Id, x3, y3)
        //to the end of a text string
        //in row text format
        static void  PrintPositionP3Row(AnsiString& S,
                                        TRoboticPositioner *FP);
        //read the value of the position properties of a RP
        //  (Id, x3, y3)
        //from the indicated position of a text string
        //in row text format
        static void  ReadPositionP3Row(TRoboticPositioner* &FP,
                                       const AnsiString& S, int &i);

        //print the value of the position properties of a RP
        //  (Id, p_1, p___3)
        //to the end of a text string
        //in row text format
        static void  PrintPositionPPARow(AnsiString& S,
                                        TRoboticPositioner *FP);

        //lee una instancia en una cadena
        static void  ReadInstance(TRoboticPositioner* &FP,
                const AnsiString& S, int &i);

        //------------------------------------------------------------------
        //BUILDING, CLONATION, COPYING AND DESTROYING METHODS:

        //build a RP by default
        TRoboticPositioner(void);
        //build a RP with the indicated values
        //if Id is less 1 throw an exception EimproperArgument
        TRoboticPositioner(int _Id, TDoublePoint _P0,
                double _thetaO1=MEGARA_thetaO1);

        //WARNING: duplicity of identifiers is allowed.

        //copy all control properties of a RP
        void CopyControl(const TRoboticPositioner*);
        void CopyControl(const TRoboticPositioner&);
        //copy all tolerance properties of a RP
        void CopyTolerance(const TRoboticPositioner*);
        void CopyTolerance(const TRoboticPositioner&);
        //copy all status properties of a RP
        void CopyStatus(const TRoboticPositioner*);
        void CopyStatus(const TRoboticPositioner&);

        //copy all properties of a RP
        void Clone(const TRoboticPositioner*);
        void Clone(const TRoboticPositioner&);
        TRoboticPositioner& operator=(const TRoboticPositioner&);

        //build a clon of a RP
        TRoboticPositioner(const TRoboticPositioner*);
        TRoboticPositioner(const TRoboticPositioner&);

        //copy all properties of a RP
        //except (P0, Id, Id1, Id2)
        void Apply(const TRoboticPositioner&);

        //free the dynamic memory and delete the RP in Builts
        //if the RP isn't in Built throw EImproperCall
        ~TRoboticPositioner();

        //-------------------------------------------------------------------
        //METHODS TO CALCULATE THE RECOMMENDED VALUES OF THE SPMCOMPONENTS:

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
        //- the positioning error due the to imprecisión of F(theta_1) and Arm->F(theta___3).

        //ADVERENCIA: no confundir (Eo, Ep) con (PAem, Pem); (Eo, Ep)
        //se refiere al error de posición y orientación al colocar
        //el posiciondor en el posicionador multifibra, tras la calibración.

        //NOTA: la alinealidad de las funciones G(p_1) y Arm->G(p___3)
        //no tiene por que implicar un eror de posición a lo largo de
        //la trayectoria de P3, ya que las posiciones p_1 y p___3 son
        //calculadas teniendo en cuenta dicha alinealidad.
        //De este modo solo habrá que tener en cuenta el salto más grande
        //de cada eje para determinar la contribución de F en Ep.

        //dynamic SPM:
        //  SPMdyn = (CMF.vmaxabs1*Actuator->rbs*Actuator->r_max +
        //      CMF.vmaxabs2*Actuator->Arm->rbs*Actuator->Arm->L1V)*Tshiff + SPMadd;
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
        void SetTolerances(double _Eo, double _Ep,
                           double _Tstop, double _Tshiff,
                           double _SPMadd);

        //------------------------------------------------------------------
        //ASSIMILATION METHODS:

        //A partir de:
        //      (Eo, Ep, Tstop, Tshiff, SPMadd)
        //      rmax
        //Determina:
        //      (SPMrec, SPMsta, SPMdyn, SPMmin)
        void CalculateSPMComponents(void);

        //asigna la componente de SPM para absorber el desplazamiento
        //por corrección del offset
        //      Actuator->SPMoff = PAem*Actuator->rmax + Pem mm
        void SetSPMoff(double PAem, double Pem);

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
        //  rotor 1 from actualposition to Max(0, p_1min)
        //  rotor 2 from actualposition to Max(0, p___3min)
        void getInstructionToGoToTheOrigin(TInstruction&) const;

        //METHODS TOPROGRAM GESTURES:

        //program the abatement of the arm to
        //the more closer-stable security position
        void programTurnArmToSafeArea(void);
        //program the retraction of the arm to
        //the more closer-stable security position
        //if rotor 1 quantifier or rotor 2 quantifier is disabled:
        //  throw and exception EImproperCall
        void programRetractArmToSafeArea(void);

        //When the vmax of rotor 2 is the double that vmax of rotor 1,
        //the extension and retraction of the arm will be radial. And
        //when the rotor 1is near of origin, part of the trajectory can
        //be circular around the rotor 2.

        //program turn of rotor 1 to theta_1
        //if theta_1 is out rotor 1 domain:
        //  throw an exception EImproperArgument
        void programTurnCilinderTotheta_1(double _theta_1);
        //gira el cilindro hasta theta_2
//        void TurnCilinderTotheta_2(double theta_2);
        //gira el cilindro hasta theta_3
//        void TurnCilinderTotheta_3(double theta_3);

        //gira el brazo hasta theta_2
//        void TurnArmTotheta_2(double theta_2);
        //gira el brazo hasta theta_3
//        void TurnArmTotheta_3(double theta_3);
        //gira el brazo hasta r_2
//        void TurnArmTor_2(double r_2);
        //gira el brazo hasta r_3
//        void TurnArmTor_3(double r_3);
        //gira el brazo hasta el área de seguridad
        //si el brazo ya está en el área de seguridad devuelve falso
//        void TurnArmToSafeArea(void);
        //gira el brazo hasta el origen de coordenadas
//        void TurnArmToOrigin(void);

        //desplaza P2 radialmente hasta r_2
//        void GoRadialTor_2(double r_2);
        //desplaza P2 radialmente hasta que el brazo quede dentro
        //del área de seguridad o hasta donde se pueda
//        void GoRadialToSafeArea(void);
        //desplaza P2 radialmente hasta P0 o hasta donde se pueda
//        void GoRadialToOrigin(void);

        //va directamente a (r_3, theta_3)
//        void GoDirectlyToPolarP_3(double r_3, double theta_3);
        //progrsam go directly from actual position to (x_3, y_3)
        //if (x_3, y_3) is out the scope of the RP:
        //  throw an exception EImproperArgument
        void programGoDirectlyToCartesianP_3(double x_3, double y_3);
        //progrsam go directly from actual position to (x3, y3)
        //if (x3, y3) is out the scope of the RP:
        //  throw an exception EImproperArgument
        void programGoDirectlyToCartesianP3(double x3, double y3);

        //METHODS TO MODIFY THE PROGRAMMED GESTURE:

        //invertgesture in the time domain
        void InvertTime(void);

        //METHODS TO ADD MESSAGE INSTRUCTIONS TO THE MP:

        //add a amessage instruction to retract to security position
        //or to where rotor 1 is in the origin
        bool addMIforRetraction(void);

        //add amessage instruction to abate the arm to security position
        void addMIforAbatement(void);

        //METHODS TO DETERMINE MOTION PARAMETERS:

        //determina el valor absoluto de
        //el ángulo máximo recorrido al moverse el eje 1 durante un tiempo T
        //      theta_MaxAbs(T) = Actuator->G(CMF.vmaxabs1)*T;
        double theta_MaxAbs(double T);
        //determina el valor absoluto de
        //la distancia máxima recorrida por un punto del brazo
        //al moverse el eje 1 durante un tiempo T
        //      d_MaxAbs(T) = theta_MaxAbs(T)*Actuator->r_max;
        double d_MaxAbs(double T);

        //determina el valor absoluto de
        //el ángulo máximo recorrido al moverse el eje 2 durante un tiempo T
        //      theta___MaxAbs(T) = Actuator->Arm->G.Image(CMF.vmaxabs2)*T;
        double theta___MaxAbs(double T);
        //determina el valor absoluto de
        //la distancia máxima recorrida por un punto del brazo
        //al moverse el eje 2 durante un tiempo T
        //      d___MaxAbs(T) = theta___MaxAbs(T)*Actuator->Arm->Ra;
        double d___MaxAbs(double T);

        //determina una cota superior para la distancia máxima recorrida
        //por un punto del brazo del posicionador, al moverse ambos ejes
        //durante un intervalo de tiempo T
        //      dMaxAbs = Max(d_MaxAbs(T), d___MaxAbs(T))
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

        //ADVERTENCIA: Para simular el movimeinto de los ejes correctamente
        //debe desactivarse la cuantificación de los ejes que se van a mover.
        //Esto queda bajo responsabilidad del programador.
        //Lo normal es que se desactiven ambos ejes, ya que el eje
        //que no se va a desplazar debe quedar libre para su desplazameinto.

        //lleva los ejes del posicionador adscrito a
        //las posiciones correspondientes al instante t
        //sila cuantificación de los ejes que se van amover está activada
        //lanza una excepción EImproperCall
        void Move(double t);
        //lleva los ejes del posicionador adscrito a
        //las posiciones correspondientes al instante 0
        //(MF1->psta, MF2->psta)
        void MoveSta(void);
        //lleva los ejes del posicionador adscrito a
        //las posiciones correspondientes al instante CMF->tendmax
        //(MF1->pfin, MF2->pfin)
        void MoveFin(void);

        //desplaza los ejes hasta sus posiciones correspondientes
        //al instante t+qt, donde:
        //      t: instante de tiempo previo al actual
        //      qt: escalón de cuantificación
        //void Displace(double t, double qt);

        //si se invoca el método Displace(t, qt) reiteradamente
        //se acumulará el error de suscesivos desplazamientos en los ejes
        //para minimizar este error este método debería invocarse
        //con la cuantificación de los ejes del posicionador desactivada

        //Como el gesto mueve ambos ejes de manera independiente,
        //se puede prescindir del método Displace(t, qt)
        //P.e: cuando el brazo es abatido, es posible cambiar
        //la orientación del cilindro, y al invocar al método Move(t),
        //este conservará su dirección; cosa que no ocurriría si
        //el gesto hubiera definido una función constante para el eje
        //que no se mueve, en cuyo caso volvería a su posición inicial.
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // ROBOTICPOSITIONER_H
