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
///@file FiberMOSModelConstants.h
///@brief constants for build a Fiber MOS model by default
///@author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FIBERMOSMODELCONSTANTS_H
#define FIBERMOSMODELCONSTANTS_H

//--------------------------------------------------------------------------
//CONSTANTS OF THE Fiber MOS Model
//--------------------------------------------------------------------------

//ADVERTENCIA: si se nombrara una constante como L a secas, habría conflicto
//con el parámetro L definido en los métodos de las clases de listas.
//Dicho conflicto podría resolverse incluyendo la definición de la constante L
//despues de la inclusión de los módulos que contienen las clases de listas
//que se vayan a utilizar. Pero para evitar este inconveniente se prefiere
//utilizar nombres de contantes más específicos y evitar los nombres genéricos
//como L.

//DEFAULT VALUES FOR TOLERANCES:

//default value for
//orientation error margin of a RP in S1 due to mechanized
#define MEGARA_Eo 0.000872665 //rad
//default value for
//position error margin of a RP in S1 due to mechanized
#define MEGARA_Ep 0.1 //mm

//default value for
//past time between last storage of positions
//and detention of the rotors
#define MEGARA_Tstop 0 //ms
//default value for
//maximun temporal shiff between rotors
#define MEGARA_Tshiff 1 //ms

//default value for
//additional component of SPMdyn
#define MEGARA_SPMadd 0.5 //mm
//SPM of minimun jump, consumed when validation process is applied
#define MEGARA_SPMmin 0.005 //mm
//SPM of simulation, consumed when simulation process is applied
#define MEGARA_SPMsim 0.052 //mm

//default value for
//position angle error margin of the FiberMOS in S0
#define MEGARA_PAem 0 //rad
//default value for
//Position error margin of the Fiber MOS in S0
#define MEGARA_Pem 0 //mm

//DEFAULT VALUES FOR PHISICAL PROPERTIES:

//length of the arm
//must be upper zero
#define MEGARA_L 5.8025 //mm
//contour of the temlate of the arm (in mm)
//#define MEGARA_Contour____Text "{({2.77263413, 2.25}, {2.64034656, 2.3}, {2.77263413, 2.45}, 0.2), ({2.77263413, 2.25}, {5.80237021, 2.25}), ({5.80237021, 2.25}, {5.80237021, -2.25}, {5.80237021, 0}, 2.250), ({5.80237021, -2.25}, {2.77263413, -2.25}), ({2.64034656, -2.3}, {2.77263413, -2.25}, {2.77263413, -2.45}, 0.2), ({2.64034656, -2.3}, {1.80711062, -2.3}), ({1.80711062, -2.3}, {1.80711062, 2.3}), ({1.80711062, 2.3}, {2.64034656, 2.3})}"
//#define MEGARA_Contour____Text "{({0.802, 2.35}, {5.802, 2.35}), ({5.802, 2.35}, {5.802, -2.35}, {5.802, 0}, 2.35), ({5.802, -2.35}, {0.802, -2.35}), ({0.802, -2.35}, {0.802, 2.35})}"
#define MEGARA_Contour____Text "{({0.807111, 2.35}, {5.807111, 2.35}), ({5.807111, 2.35}, {5.807111, -2.35}, {5.807111, 0}, 2.35), ({5.807111, -2.35}, {0.807111, -2.35}), ({0.807111, -2.35}, {0.807111, 2.35})}"
//steps back of rotor 1 (mumber of steps by whole lap)
#define MEGARA_SB1 194086.302 //steps

//orientation of S1 respect S0
#define MEGARA_thetaO1 0 //rad
//lower limit of theta_1
#define MEGARA_theta_1min -0.043633231 //rad
//upper limit of theta_1
#define MEGARA_theta_1max 6.326818538 //rad

//steps back of rotor 2 (mumber of steps by whole lap)
#define MEGARA_SB2 30720 //steps

//lower limit of theta___3
#define MEGARA_theta___3min -0.043633231 //rad
//upper limit of theta___3
#define MEGARA_theta___3max 3.185225885 //rad

//SPM value for generation with precise knolege degree
//this is the value to initialize the SPM of each arm
#define MEGARA_SPMgenPairPPDP_p 0.195723055049871 //mm

//contour of the barrier of a RP corresponding to the maneuvering space of the arm (in mm)
//#define MEGARA_Contour_Text "{({13.940281559018, 0}, {-13.940281559018, 0}, {0, 0}, 13.940281559018), ({-13.940281559018, 0}, {13.940281559018, 0}, {0, 0}, 13.940281559018)}"
#define MEGARA_Contour_Text "{({13.955, 0}, {-13.955, 0}, {0, 0}, 13.955), ({-13.955, 0}, {13.955, 0}, {0, 0}, 13.955)}"

//distance between RPs
#define MEGARA_D 20.1 //mm

//in MEGARA, when L=5.80237021, then 4*L*cos(M_PI/6) = 20.1

//CONSTANTS FOR THE EXCLUSION AREA:

//contour of the barrier of a EA corresponding to the LIFU (in mm)
//#define MEGARA_LIFU_Conatour_Text "{({-15, -15}, {-15, 15}), ({-15, 15}, {15, 15}), ({15, 15}, {15, -15}), ({15, -15}, {-15, -15})}"
#define MEGARA_LIFU_Contour_Text "{({-9, -9.95}, {-14.2, -4.75}, {-9, -4.75}, 5.2), ({-14.2, -4.75}, {-14.2, 6.721909}), ({-14.2, 6.721909}, {-13.0999999516436, 8.62716491624432}, {-12, 6.721909}, 2.2), ({-13.0999999516436, 8.62716491624432}, {-10.6000001182939, 10.070541204891}), ({-9.7, 11.629387}, {-10.6000001182939, 10.070541204891}, {-11.5, 11.629387}, 1.8), ({-9.7, 11.629387}, {-9.7, 21.2}), ({-9.7, 21.2}, {-7.5, 23.4}, {-7.5, 21.2}, 2.2), ({-7.5, 23.4}, {7.5, 23.4}), ({7.5, 23.4}, {9.7, 21.2}, {7.5, 21.2}, 2.2), ({9.7, 21.2}, {9.7, 11.629387}), ({10.6000001182939, 10.070541204891}, {9.7, 11.629387}, {11.5, 11.629387}, 1.8), ({10.6000001182939, 10.070541204891}, {13.0999999516436, 8.62716491624432}), ({13.0999999516436, 8.62716491624432}, {14.2, 6.721909}, {12, 6.721909}, 2.2), ({14.2, 6.721909}, {14.2, -4.75}), ({14.2, -4.75}, {9, -9.95}, {9, -4.75}, 5.2), ({9, -9.95}, {-9, -9.95})}"

//CONSTANTS OF THE MOTION FUNCTIONS:

#define MEGARA_VMAXABS1 3 //steps/ms
#define MEGARA_AMAXABS1 4294.967295 //steps/ms^2
//#define MEGARA_VMAXABS2 2*MEGARA_SB2/MEGARA_SB1*MEGARA_VMAXABS1 //steps/ms
#define MEGARA_VMAXABS2 0.945 //steps/ms
//#define MEGARA_AMAXABS2 2*MEGARA_SB2/MEGARA_SB1*MEGARA_AMAXABS1 //steps/ms^2
#define MEGARA_AMAXABS2 4294.967295 //steps/ms^2

//---------------------------------------------------------------------------
//CONSTANTS OF Telescope Projection Model
//---------------------------------------------------------------------------

//radio del plano focal de GTC en mm
#define GTC_R 130.091752531644

//---------------------------------------------------------------------------
//CALCULUS OS SPM COMPONENTS:
//---------------------------------------------------------------------------

//Default values for SPM components in an actuator:
//  SPMrec = (MEGARA_VMAXABS1*rbs*r_max + MEGARA_VMAXABS2*Arm->rbs*Arm->L1V)*
//      MEGARA_Tstop;
//  SPMsta = MEGARA_Eo*r_max + MEGARA_Ep;
//  SPMdyn = (MEGARA_VMAXABS1*rbs*r_max + MEGARA_VMAXABS2*Arm->rbs*Arm->L1V)*
//      MEGARA_Tshiff + SPMadd;
//  SPMmin = MEGARA_Ep;
//  SPMoff = MEGARA_PAem*rmax + MEGARA_Pem;

//Calculating the SPM components in a RP:
//  SPMrec = (CMF.vmaxabs1*Actuator->rbs*Actuator->r_max +
//      CMF.vmaxabs2*Actuator->Arm->rbs*Actuator->Arm->L1V)*Tstop;
//  SPMsta = Eo*Actuator->r_max + Ep;
//  SPMdyn = (CMF.vmaxabs1*Actuator->rbs*Actuator->r_max +
//      CMF.vmaxabs2*Actuator->Arm->rbs*Actuator->Arm->L1V)*Tshiff + SPMadd;
//  SPMmin = MEGARA_Ep;
//  SPMoff = PAem*Actuator->rmax + Pem;

//SPMsta must be enough large to include:
//- the mechanical error due to mechanization of the actuator;
//- the position error of the RPs;
//- the orientation error of the RPs;
//- the positioning error due the to imprecisión of F and Arm->F
//
//It is convenient a small value for SPMsta because is a measure of
//the error margin in positioning of the fiber.

//SPMdyn must be enough large to include:
//- temporal shiff between the start of the movements;
//- motion funtion not correspond exactly the indicated in the model;
//- ambiguity in position due the quantification of the rotors;
//- asymmetrical radial motion due to motion velocity of rotor2
//  is not double of velocity of rotor 1 in all moment.

//No se debe olvidar que aunque las pruebas permitan acotar el margen de error
//de posicionamiento de la fibra dentro de determindos límites, el margen de
//error que deberá tenerse en cuenta para evitar colisiones, es el que afecta
//a la posición final del brazo del posicionador, el cual podría ser mayor
//que el error de posicionamiento de la fibra.

//Para reducir SPMsta así como el error de posicionamiento
//del punto P3, es posible realizar un procedimiento de calibración
//para medir las siguientes propiedades:
//      P0: posición del eje 1 en S0;
//      thetaO1: orientación de S1 en S0;
//      Arm->theta__O3: orientación de S3 en S2;
//      L01: longitud del antebrazo o radio del cilindro;
//      Arm->L13: distancia entre el eje 2 y el centro de la fibra;
//      F(theta_1): f. de compresión del cuant. del eje 1;
//      Arm->F(theta___3): f. de compresión del cuant. del eje 2.
//Conocer las siguientes propiedades también puede reducir
//el margen de error, pero puede resultar más difícil medirlas,
//porque no es posible hacerlo retroiluminando las fibras:
//      Arm->L12: longitud del brazo;
//      Arm->theta____3: offset angular entre Arm->P2 y Arm->P3.

//---------------------------------------------------------------------------
#endif // FIBERMOSMODELCONSTANTS_H
