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
//File: MotionProgramExecutor.h
//Content: executor of motionprograms
//Last update: 26/12/2014
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef MOTIONPROGRAMEXECUTOR_H
#define MOTIONPROGRAMEXECUTOR_H

#include "MotionProgramValidator.h"

#include <QTimer>
//--------------------------------------------------------------------------

//namespace for positioning
namespace Positioning {

//--------------------------------------------------------------------------

//class motion program executor
class TMotionProgramExecutor {
        //ATTACHED EXTERN OBJECTS:

        TRoboticPositionerList *__RoboticPositionerList;
//        TBUSCAN *__BUSCAN;

        //PROPIEDADES GRÁFICAS:

        int __SleepingTime;

        QTimer *__Timer;

public:
        //ATTACHED EXTERNOBJECTS:

        //robotic positioner list
        TRoboticPositionerList *getRoboticPositionerList(void) const {
            return __RoboticPositionerList;}

        //bus CAN
//        __property TBUSCAN *BUSCAN = {read=__BUSCAN};

        //GRAPHICAL PROPERTIES:

        //tiempo de ejecución relativo
        //valor por defecto: 0 ms
        double t;
        //tiempo de ejecución acumulado
        //valor por defecto: 0 ms
        double acumt;
        //iteraciones acumuladas
        //valor por defecto: 0
        int acumn;

        //indica si se ha detectado una colisión durante la ejecución
        //valor por defecto: false
        bool Collision;

        //función de dibujo externa adscrita
        //para actualizar la caja de pintura del plano focal
        //valor por defecto: NULL
        void (*PaintAll)(void);

        //retardo entre iteraciones
        //debe ser no negativo
        //valor por defecto: 0 ms
        int getSleepingTime(void) const {return __SleepingTime;}
        void setSleepingTime(int);

        //función para almacenar la última imagen generada mediante PaintAll
        //valor por defecto: NULL
        void (*SaveImage)(void);

        //PROPIEDADES DE CONTROL EN MODO TEXTO:

        AnsiString getacumtText(void);
        AnsiString getacumnText(void);

        //MÉTODOS PÚBLICOS:

        //contruye un ejecutor de programas de posicionamiento
        //adscrito a una lista de posicionadores de fibra y a un can bus
        TMotionProgramExecutor(TRoboticPositionerList *_RoboticPositionerList);
//                TCANBUS *_CANBUS);

        //libera la memoria dinámica
        ~TMotionProgramExecutor();

        //rutina del temporizador
        void TimerTimer(/*TObject *Sender*/void);

        //ejecuta una lista de mensajes
        void execute(TMessageList*);
        //ejecuta un programa de movimiento
        void execute(TPairPositionAnglesList& IPL, TMotionProgram &PP);

        //La ejecución puede acabar de dos maneras:
        //1. si no se detecta colisión con los poicionadores en
        //      sus posiciones finales.
        //2. si se detecta colisión con los posicionadores en
        //      posición de colisión.
};

//--------------------------------------------------------------------------

} //namespace Positiong

//---------------------------------------------------------------------------
#endif // MOTIONPROGRAMEXECUTOR_H
