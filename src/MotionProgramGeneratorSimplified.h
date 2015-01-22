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
//File: MotionProgramGeneratorSimplified.h
//Content: generator of motionprograms
//Last update: 26/12/2014
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef MOTIONPROGRAMGENERATORSIMPLIFIED_H
#define MOTIONPROGRAMGENERATORSIMPLIFIED_H

#include "FiberMOSModel2.h"
#include "TargetPointList.h"
#include "MotionProgram.h"
#include "MotionProgramValidator.h"

//---------------------------------------------------------------------------

using namespace Models;

//namespace for positioning
namespace Positioning {

//###########################################################################
//TMotionProgramGeneratorSimplifiedSimplified:
//###########################################################################

//class motion program generator
class TMotionProgramGeneratorSimplified : public TTargetPointList, public TMotionProgramValidator {
protected:
    //SETTING PARAMETERS OF THE ALGORITHMS:

    double __dt_jmp;

    //GRAPHICAL PROPERTIES:

    int __SleepingTime;

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
    //   so that the checking belonging to domain, could introduce numerical errors.

    //Note that when the jump correspond to a integer number of steps,
    //when quantification is enabled, the rotors do not change their positions.

    //GRAPHICAL PROPERTIES:

    //attached drawing external function
    //for update the graphical interfaz
    //default value: NULL
    void ( *PaintAll)(void);

    //delay between iterations in ms
    //shall be nonnegative
    //default value: 0 ms
    int getSleepingTime(void) const {return __SleepingTime;}
    void setSleepingTime(int);

    //------------------------------------------------------------------
    //BUILDING AND DESTROYING METHODS:

    //build a motion program generator
    //attached to a robotic positionerlist
    TMotionProgramGeneratorSimplified(TRoboticPositionerList *RoboticPositionerList);
    //destory the targetpoints and destroy the MPG
    ~TMotionProgramGeneratorSimplified(void);

    //------------------------------------------------------------------
    //BASIC METHODS:

    //DEFINICIÓN: SPM de salto mínimo conjunto de dos posicioandores:
    //      SPMmin = RP1->Actuator->SPMmin + RP2->Actuator->SPMmin;

    //calcula el tiempo libre de dos posicionadores acotado inferiormente
    //de modo que cuando Df > SPMmin
    //      el tiempo libre se calcula de modo que se consuma la Df
    //      sin llegar a rebasarla
    //y cuando Df <= SPMmin
    //      el tiempo libre se calcula de modo que se consuma el SPMmin
    //      sin llegar a rebasarlo
    double CalculateTf(TRoboticPositioner *RP1, TRoboticPositioner *RP2);

    //Por el método de salto,
    //determina el desplazamiento angular en sentido positivo o negativo
    //del rotor 1 de un posicionador, en pasos, para que no colisione
    //con sus adyacentes, y determina el tiempo libre respecto de sus
    //adyacentes, en la nueva posición estable.
    //Si no es posible encontrar una solución lanza ECantFindSolution.
    double DisplacementToAvoidInvasion_JumpingMethod(double &Tfmin,
                                                     TRoboticPositioner *RP);
    /*        //Por el método de búsqueda binaria,
        //determina el desplazamiento angular en sentido negativo
        //del eje 1 de un posicionador, en radianes, para que
        //no colisione con sus adyacentes,
        //y determina el tiempo libre respecto de sus adyacentes.
        //Si no es posible encontrar una solución lanza ECantFindSolution.
        double DisplacementToAvoidInvasion_BinarySearch(double &Tfmin,
                TRoboticPositioner *RP);
  */

    //Conviene que el método DisplacementToAvoidInvasion devuelva
    //el desplazamiento angular en pasos, porque dicho desplazamiento
    //debe ser un número entero de pasos, para que la nueva posición
    //sea estable.

    //Resuelve la colisiones girando el rotor 1 de los posicionadores
    //operativos de la lista, en orden de menor a mayor desplazamiento
    //angular absoluto, e indica si se ha encontrado alguna colisión.
    //Todos los posicionadores de la lista Invaders:
    //      - deben tener el cuantificador de rot 1 deshabilitado;
    //      - deben tener un gesto de retracción programado;
    //      - deben estar operativos;
    //      - pueden quedar con rot 1 desplazado;
    //      - quedarán con la función de movimiento de rot 1 desplazada
    //        la misma distancia que rot 1.
    //Si para encontrar la solución ha tenido que girar algún posicionador:
    //	devuelve verdadero.
    //Si no es posible encontrar una solución lanza ECantFindSolution.
    bool TurnToSolveCollisions(double& Tfmin,
                               TRoboticPositionerList& Invaders);

    //Para cada posicionador de de la lista Outsiders:
    // - Programa un gesto de retracción.
    // - Si tiene algún cuantificador deshabilitado:
    //	lanza una excepción EImproperArgument.
    // - Si está en posición de seguridad:
    //	lanza una excepción EImproperArgument.
    void ProgramRetraction(TRoboticPositionerList &Outsiders);

    //-------------------------------------------------------------------
    //METHODS TO RETRACT THE ARMS:

    //Retrae los posicionadores retráctiles y segrega los invasores
    //y los retráctiles.
    //Todos los posicionadores de la lista Outsiders:
    // - deben estar en posición de inseguridad;
    // - deben estar operativos;
    // - no deben estar en estado de colisión dinámica;
    // - deben tener los cuantificadores de los rotores habilitados;
    //Para todos los posicionadores de la lista Outsiders:
    // - la posición inicial de los rotores quedará apilada;
    // - quedará programado un gesto de retracción.
    // - los cuantificadores de los rotores quedarán deshabilitados.
    //Los posicionadores de la lista Retractiles quedarán retraidos.
    //Los posicionadores de la lista Invaders quedarán en sus
    //posiciones iniciales.
    void RetractRetractiles(TRoboticPositionerList &Invaders,
                            TRoboticPositionerList &Retractiles,
                            TRoboticPositionerList &Outsiders);

    //Simula la retracción de los posicionadores de la lista Invaders.
    //Cuando detecta una colisión, la resuelve mediante giros de rot 1 y
    //reinicia el proceso de simulación con las nuevas posiciones de rot 1.
    //Para cada posicionador de la lista Invaders:
    //      deben tener apilada la posición inicial de los rotores;
    //      deben tener deshabilitados los cuantificadores de los rotores;
    //      deben tener un gesto de retracción programado;
    //	quedarán retraidos en posición de seguridad;
    //	pueden acabar con rot 1 desplazado;
    //      quedarán con la CMF.MF1 desplazada la misma distancia que rot 1.
    void RetractInvaders(TRoboticPositionerList &Invaders);

    //<----24/04/2014
    /*        //Para cada RP de la lista Abatibles:
        //- restaura el estado del cuantificador de su brazo;
        //- restaura la posición inicial de su brazo;
        //- desempila el estado de cuantificación de su brazo;
        //- desempila la posición inicial de su brazo.
        void Pop(TRoboticPositionerList *Abatibles, TRoboticPositionerList *Invaders);
  */
    //Segrega los posicionadores invasores cuyo rot 1 está desplazado,
    //y programa un gesto de giro desde su posición inicial a la actual.
    //Todos los posicionadores de la lista Invaders:
    //      deben tener apilada una posición de rot 1.
    void ProgramTurns(TRoboticPositionerList &TurnsInvaders,
                      TRoboticPositionerList &Invaders);

    //construye una lista de mensajes de instrucción a partir de
    //una lista de posicionadores
    TMessageList *GetMessages(TRoboticPositionerList &RPL);
    /**
        //------------------------------------------------------------------
        //MÉTODOS PARA GENERAR PROGRAMAS DE MOVIMIENTO:

        //Genera un programa de despliegue de hasta tres pasos:
        //      1. despliegue de los invasores;
        //      2. giro de los invasores;
        //      3. despliegue de los desplegables.
        //parámetros:
        //      DeployProgram: programa de despliegue;
        //      Outsiders: lista de RPs a desplegar;
        //Los posicionadores quedarán abatidos en las posiciones de partida
        //para ir a las posiciones finales.
        void GenerateDeployProgram(
                TMotionProgram &DeployProgram,
                TRoboticPositionerList &Outsiders);
**/
    //Genera un programa de deposicionamiento de hasta tres pasos:
    //      1. retracción de los retráctiles;
    //      2. giro de los invasores;
    //      3. retracción de los invasores.
    //parámetros:
    //      DP: programa de deposicionamiento;
    //      Outsiders: lista de posicionadores a retraer;
    //Los posicionadores quedarán retraidos en posiciones de seguridad.
    void GenerateDepositioningProgram(TMotionProgram &DP,
                                      TRoboticPositionerList &Outsiders);
    /**
        //En la posición inicial no debe haber invasiones,
        //Y en la posición final no hay invasiones,
        //ya que despues de este método todos quedan abatidos,
        //de modo que no hay que comprobar si son compatibles.

        //genera un programa de ida directa desde las posiciones actuales hasta:
        //      los puntos internos
        //      los puntos de partida para ir a los puntos externos
        void GenerateGoDirectlyProgram(
                TMotionProgram &PositioningProgram,
                TVector<double> &p_1s, TVector<double> &p___3s,
                TRoboticPositionerList &FinalOutsiders,
                TRoboticPositionerList &FinalInners);

        //Genera un programa de cambio de posiciones.
        //Parámetros:
        //      PCP: programa de cambio de posiciones.
        //      indices: índices a los pares defectuosos.
        //Valores de retorno:
        //      0: programación finalizada con éxito;
        //      1: los posicionadores indicados tienen
        //         algúncuantificador desactivado;
        //      2: los posicionadores indicados están en
        //         posiciones iniciales incompatibles;
        //      3: los puntos objetivo indicados están adscritos a
        //         posicionadores repetidos;
        //      4: los puntos objetivo indicados están adscritos a
        //         posicionadores ausentes;
        //      5: los puntos objetivo indicados están fuera del dominio
        //         de sus posicionadores adscritos;
        //      6: los puntos objetivo indicados están en
        //         posiciones incompatibles.
        char GeneratePositioningChangeProgram(TMotionProgram& PCP,
                TVector<int> &indices);

        //NOTA: en el método GenerateProgram cuando:
        //      la bandera de cuantificación de un eje está activada
        //          los comandos de eje indicarán posiciones enteras (en pasos);
        //      la bandera de cuantificación de un eje está desactivada
        //          los comandos de eje indicarán posiciones reales (en pasos);

        //Aunque el programa de posicionamiento se va a dividir en programas
        //de abatimiento, predespliegue y despliegue, interesa conservar
        //la definición del método GenerateProgram, por ser una manera
        //más eficiente de obtener programas de posicionamiento de hasta 7
        //pasos, los cuales resultan útiles en simulaciones de montecarlo.
     **/
    /*        //------------------------------

        //Programa el despliegue hasta los puntos objetivo,
        //quedando los posicionadores abatidos.
        //Valores devueltos:
        //      0: programación realizada con éxito;
        //
        //      1: los posicionadores indicados se encuentran en
        //         posiciones iniciales incompatibles;
        //      2: los puntos objetivo indicados están adscritos a
        //         posicionadores repetidos;
        //      3: los puntos objetivo indicados están adscritos a
        //         posicionadores ausentes (en ListRoboticPositioner);
        //      4: los puntos objetivo indicados están fuera del dominio
        //         de sus posicionadores adscritos;
        //      5: los puntos objetivo indicados están en
        //         posiciones incompatibles;
        //
        //      6: el algoritmo no puede encontrar una solución.
        char ProgramDeployment(TMotionProgram &DP,
                TRoboticPositionerList &FinalOutsiders,
                TRoboticPositionerList &FinalInners);

        //Programa el predespliegue desde las posiciones actuales
        //hasta las posiciones que se indiquen.
        //Valores devueltos:
        //      0: programación realizada con éxtio;
        //
        //      1: los posicionadores indicados están en
        //         posiciones iniciales no seguras;
        //
        //      2: los puntos objetivo indicados están adscritos a
        //         posicionadores repetidos;
        //      3: los puntos objetivo indicados están adscritos a
        //         posicionadores ausentes (en RoboticPositionerList);
        //      4: los puntos objetivo indicados están fuera del dominio
        //         de sus posicionadores adscritos;
        //      5: los puntos objetivo indicados están en
        //         posiciones incompatibles;
        char ProgramPredeployment(TMotionProgram &PP,
                TVector<double> &p_1s, TVector<double> &p__3s,
                TRoboticPositionerList &FinalOutsiders,
                TRoboticPositionerList &FinalInners);

        //Programa el repliegue desde las posiciones actuales
        //quedando todos los posicionadores en posiciones de seguridad.
        //Valores devueltos:
        //      0: programación realizada con éxito;
        //      1: los posicionadores indicados se encuentran en
        //         posiciones iniciales incompatibles;
        //      2: el algoritmo no puede encontrar una solución.
        char ProgramRetraction(TMotionProgram &AP);
//                TRoboticPositionerList &InitialOutsiders,
  //              TRoboticPositionerList &InitialInners);

        //El programa completo de posicionamiento se compone de:
        //      predespliegue (predeployment);
        //      despliegue (deployment);
        //      repliegue (retraction).

        //Utilización de los programadores:
        //      Todos los posicionadores deben encontrarse en
        //      posiciones iniciales de seguridad.
        //      1. Guarda las posiciones iniciales de todos los posicionadores.
        //
        //      2. Segrega los posicionadores con puntos objetivo internos
        //         y externios y programa el despliegue.
        //      3. Guarda las posiciones de partida para el despliegue de
        //         los posicionadores con puntos objetivo externos.
        //      3. Programa el predespliegue.
        //
        //      4. Mueve los posicionadores a sus posicioens finales.
        //      5. Programa el repliegue.
        //
        //      6. Restaura las posiciones iniciales.*/
};

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
#endif // MOTIONPROGRAMGENERATOR_H
