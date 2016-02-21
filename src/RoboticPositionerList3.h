// Copyright (c) 2012-2016 Isaac Morales Durán. All rights reserved.
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
//File: RoboticPositionerList3.h
//Content: RP list with programming and execution functins
//Last update: 13/02/2015
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ROBOTICPOSITIONERLIST3_H
#define ROBOTICPOSITIONERLIST3_H

#include "RoboticPositionerList2.h"

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//---------------------------------------------------------------------------

//Class TRoboticPositionerList3:
//      Permite la programación conjunta de controladores
//      mediante comandos con el identificador 0.
//      Permite la determinación del periodo de desplazaemiento máximo.

//class RP list
class TRoboticPositionerList : public TRoboticPositionerList2 {
protected:
        //EXECUTION PROPERTIES:

        double p_Tit;

public:
        //EXECUTION PROPERTIES:

        //tiempo necesario para realizar todos los desplazamientos
        //      Tdis = Max{RoboticPositioners[i]->tendmax} s
        double getTdis(void) const;

        //periodo de iteración escalado
        //valor por defecto: 40 ms
        double getTit(void) const {return p_Tit;}
        void setTit(const double);

        //instante de tiempo actual
        //valor por defecto: 0 ms
        double t;

        //------------------------------------------------------------------
        //EXECUTION PROPERTIES IN TEXT FORMAT:

        AnsiString getTdisText(void) const;

        AnsiString getTitText(void) const; void setTitText(const AnsiString&);
        AnsiString getTimerEnabledText(void) const;
        void setTimerEnabledText(const AnsiString&);
        AnsiString getTimerIntervalText(void) const;
        void setTimerIntervalText(const AnsiString&);
        AnsiString gettText(void) const; void settText(const AnsiString&);

        //SETS OF PROPERTIES IN TEXT FORMAT:

        //conjunto de propiedades de ejecición
        //en formato asignaciones de texto
        AnsiString getExecutionText(void) const;

        //------------------------------------------------------------------
        //BUILDING, CLONATION AND DESTROYING METHODS:

        //construye una lista de RPs
        TRoboticPositionerList(void);

        //ADVERTENCIA: para cosntruir un posicionador multifibra debe haber
        //un posicinador de fibra por defecto que no incluya un puntero
        //al método Paint.

        //copia las propiedades de ejecución de una lista de RPs
        void copyExecution(const TRoboticPositionerList*);

        //clone a RP list
        //      (Tolerance, Sizing, Area, Graphics, Map, Execution)
        void Clone(const TRoboticPositionerList*);

        //WARNING: inherited methods must be redefined withidentical name
        //for avoid errors.

        //construye un clon de un posicionador multifibra
        TRoboticPositionerList(const TRoboticPositionerList*);

        //destruye una lista de RPs
        ~TRoboticPositionerList();

        //WARNING: beforedestroy the las RP list you mustdestroy these
        //using Destroy methods.

        //SETTING METHODS:

        //obtiene la tabla de identificadores (Id, Id1, Id2)
        void getIdTable(TPointersList<TTernIntegers>& IdTable) const;
        //asigna la tabla de identificadores (Id, Id1, Id2)
        //si IdTable no contiene una terna por cada posicionador:
        //      lanza una excepción EImproperArgument
        //si algún identificador en IdTable no es unívoco:
        //      lanza una excepción EImproperArgument
        //si algún Id en IdTable no se refiere a un posicionador de la lista:
        //      lanza una excepción EImproperArgument
        void setIdTable(TPointersList<TTernIntegers>& IdTable);

        //configure el SPM de los posicionadores de una lista
        //según el propósito:
        //	pAll: asignación
        //	pGen: generación
        //	pVal: validación
        //	pExe: ejecución
        void setPurpose(TPurpose Purpose);

        //configura el estado de habilitación del cuantificador de rot 1
        //de todos los posicionadores de la lista
        void setQuantify_s(bool);
        //configura el estado de habilitación del cuantificador de rot 2
        //de todos los posicionadores de la lista
        void setArmQuantify___s(bool);
        //configura el estado de habilitación de los cuantificadores
        //de todos los posicionadores de la lista
        void setQuantifys(bool Quantify_, bool Quantify___);

        //configura el estado de colisión
        //de todos los posicionadores de la lista
        void setCollisions(bool);

        //------------------------------------------------------------------
        //METHODS FOR CHECK PRECONDITIONS:

        //determines if there is some pointer to NULL RP
        bool thereIsSomeNullPointer(void) const;

        //determines if there is some repeated pointer
        bool thereIsSomeRepeatedPointer(void) const;

        //determines if there is some not operative RP
        bool thereIsSomeNotOperative(void) const;

        //determines if there is some RP out of origin
        bool thereIsSomeOutOrigin(void) const;

        //determina si todos los posicionadores operativos en
        //posiciones de inseguridad están en la lista Abatibles
        bool notAllOperativeInUnsafetyPositionsAreInTheList(
                TRoboticPositionerList *Abatibles) const;

        //determina si todos los posicionadores de la lista
        //están operativos en posiciones de inseguridad
        bool notAllPositionersAreOperativesInUnsafetyPositions(void) const;

        //determines if some RP of the Fiber MOS Model
        //could start sudenly motion
        bool thereIsSomeOperativeRPwithDynamicFaul(void) const;

        //------------------------------------------------------------------
        //METHODS FOR PROGRAMMING:

        //borra las instrucciones de movimeinto de los posicionadores
        void clearInstructions(void);

        //asigna una instrucción
        //si el identificador Id es negativo
        //      lanza una excepción EImproperArgument
        //si el posicionador Id no está en la lista
        //      lanza una excepción EImproperArgument
        //si Id==0 asigna la instrucción a todos los controldaores
        void setInstruction(const int Id, const TInstruction &Instruction);

        //programa gestos de abatimiento en todos los controladores de la lista
        void programTurnArmsToSafeAreas(void);
        //sincroniza las llegadas de todos los gestos de la lista
        void sinchronizeArrivals(void);
        //invierte todos los gestos de la lista en el dominio del tiempo
        void invertTime(void);

        //METHODS FOR MOTION:

        //mueve los RPs a la posición indicada
        //por las funciones de movimiento en el instante t
        void move(double t);
        //mueve los RPs a sus posiciones inciales
        void moveSta(void);
        //mueve los RPs a sus posiciones finales
        void moveFin(void);
        //mueve los RPs a sus posiciones finales
        //programadas en (MPturn, MPretraction)
        void moveFinMP(void);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // ROBOTICPOSITIONERLIST3_H
