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
//Archivo: MotionProgramExecutor.cpp
//Contenido: ejecutor de programas de movimiento
//Última actualización: 14/08/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "MotionProgramExecutor.h"

//--------------------------------------------------------------------------

//espacio de nombres de posicionamiento
namespace Positioning {

//--------------------------------------------------------------------------
//GRAPHICAL PROPERTIES:

void TMotionProgramExecutor::setSleepingTime(int _SleepingTime)
{
        //el intervalo de tiempo SleepingTime debe ser no negativo
        if(_SleepingTime < 0)
                throw EImproperArgument("time interval SleepingTime should be nonnegative");

        __SleepingTime = _SleepingTime; //asigna el nuevo valor
}

//PROPIEDADES DE CONTROLEN MODO TEXTO:

AnsiString TMotionProgramExecutor::getacumtText(void)
{
        return FloatToStr(acumt);
}
AnsiString TMotionProgramExecutor::getacumnText(void)
{
        return IntToStr(acumn);
}

//--------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//contruye un ejecutor de programas de posicionamiento
//adscrito a una lista de posicionadores de fibra y a un can bus
TMotionProgramExecutor::TMotionProgramExecutor(TRoboticPositionerList *_RoboticPositionerList) :
//        TCANBUS *_CANBUS)
        t(0), acumt(0), acumn(0),
        PaintAll(NULL), __SleepingTime(0),
        SaveImage(NULL)
{
        //el puntero RoboticPositionerList debe apuntar a una lista de posicionadoresde fibra construida
        if(_RoboticPositionerList == NULL)
                throw EImproperArgument("pointer RoboticPositionerList should point to built fiber positioner list");

/*        //el puntero CANBUS debe apuntar a un bus CAN construido
        if(_CANBUS == NULL)
                throw EImproperArgument("pointer CANBUS should point to built can BUS");
  */
        //apunta los objetos externos
        __RoboticPositionerList = _RoboticPositionerList;
//        __CANBUS = _CANBUS;
}

//libera la memoria dinámica
TMotionProgramExecutor::~TMotionProgramExecutor()
{
}

//ejecuta una lista de mensajes
void TMotionProgramExecutor::execute(TMessageList *ML)
{
        //el puntero ML debería apuntar a una lista de mensajes contruida
        if(ML == NULL)
                throw EImproperArgument("pointer ML should point to built message list");

        //PROGRAMA EL GESTO:

        TMessageInstruction *MI;

        //desprograma los controladores de los posicionadores
        getRoboticPositionerList()->ClearInstructions();
        //por cada mensaje de la lista de mensajes
        for(int j=0; j<ML->getCount(); j++) {
                //apunta el mensaje para facilitar su acceso
                MI = ML->GetPointer(j);
                //asigna el mensaje
                getRoboticPositionerList()->SetInstruction(MI->getId(), MI->Instruction);
        }

        //EJECUTA EL GESTO:

        //inicializa el acumulador de tiempo relativo
        t = 0;

        //activa el temporizador
//        getRoboticPositionerList()->Timer->Enabled = true;

        //mientras el tiempo acumulado sea menor que el tiempo de desplazamiento
        while(t < getRoboticPositionerList()->getTdis()) {
                //actualiza los contadores
                t += getRoboticPositionerList()->getTit();
                acumt += getRoboticPositionerList()->getTit();
                acumn++;

                //mueve los posicionadores al instante t
                getRoboticPositionerList()->Move(t);

                //si debe mostrar el proceso
                if(PaintAll != NULL) {
                        PaintAll(); //actualiza la interfaz
                        Delay(getSleepingTime()); //espera el tiempo de retardo
                }

                //si debe almacenar la imagen
                if(SaveImage != NULL)
                        SaveImage(); //salva la imagen

                //si detecta una colisión
                if(getRoboticPositionerList()->ThereIsCollision())
                        return; //termina la ejecución
        }

        //eltemporizador se detendrá cuando ocurra una de dos:
        // 1. Cunado el acumulador de tempo relativo alcance el tiempo de desplazamiento
        // 2. Cunado se detecte un acolisión.

        //indica que no se ha detectado colisión
    //    return 0;
}

//ejecuta un programa de movimiento
void TMotionProgramExecutor::execute(TPairPositionAnglesList& IPL, TMotionProgram &MP)
{
/*        //la lista de posicioandores debería estar en modo síncrono
        for(int i=0; i<getRoboticPositionerList()->getCount(); i++)
                if(getRoboticPositionerList()->Get(i)->ControlMode != cmSinc)
                        throw EImproperCall("list positioners should be in sinchronous mode");
*/
//        char collision; //bandera de colisión

        //------------------------------------------------------------------
        //mueve los ejes de los posicionadores a las posiciones iniciales
        getRoboticPositionerList()->SetPositions(IPL);

        //si debe mostrar el proceso
        if(PaintAll != NULL) {
                PaintAll(); //actualiza la interfaz
                Delay(getSleepingTime()); //espera el tiempo de retardo
        }

        //si debe almacenar la imagen
        if(SaveImage != NULL)
                SaveImage(); //salva la imagen

        //si detecta una colisión
        if(getRoboticPositionerList()->ThereIsCollision())
                return; //termina la ejecución

        //------------------------------------------------------------------
        //inicializa los acumuladores absolutos
        acumt = 0;
        acumn = 0;

        //ADVERTENCIA: si utiliza Sleep(T) o while(Timer->Enabled) {;}
        //para esperar el momento de ejecutar el próximo gesto,
        //las invocaciones a la rutina del temporizador serán inhibidas.

        //por cada lista de mensajes del programa de movimiento
        for(int i=0; i<MP.getCount(); i++) {
                //ejecuta la lista de mensajes indicada
                execute(MP.GetPointer(i));
/*                //calcula el tiempo de funcionamiento del temporizador
                double Interval = double(Timer->Interval);
                double T = ceil(getRoboticPositionerList()->Tdis/Interval)*Interval;
                //espera el tiempo de desplazamiento más un margen de seguridad
                Sleep(int(ceil(T)+1));*/
        }

    //    return collision;
}

//--------------------------------------------------------------------------

} //namespace Positiong

//--------------------------------------------------------------------------

