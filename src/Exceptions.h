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
///@file Exceptions.h
///@brief classes of exceptions
///@author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "VCLemu.h" //AnsiString

//#include <exception>
#include <time.h> //time

//---------------------------------------------------------------------------

using namespace std;

//---------------------------------------------------------------------------

//ADVERTENCIA: todas las excepciones deben derivarse de Exception para
//permitir su captura unificada.

//invocación indevida de método o función
class EImproperCall : public Exception {
public:
    EImproperCall(AnsiString Message) : Exception(Message) {;}
};

//argumento de método o función incorrecto
class EImproperArgument : public Exception {
public:
    EImproperArgument(AnsiString Message) : Exception(Message) {;}
};

//estado o configuración local incorrecta
class EImproperSetting : public Exception {
public:
    EImproperSetting(AnsiString Message) : Exception(Message) {;}
};

//no puede completar una operación, un argumento o una función
class ECantComplete : public Exception {
public:
    ECantComplete(AnsiString Message) : Exception(Message) {;}
};

//operación completada
class EComplete : public Exception {
public:
    EComplete(AnsiString Message) : Exception(Message) {;}
};

//no puede encontrar solución
class ECantFindSolution : public Exception {
public:
    ECantFindSolution(AnsiString Message) : Exception(Message) {;}
};

//valor cargado de archivo inapropiado
class EImproperFileLoadedValue : public Exception {
public:
        EImproperFileLoadedValue(AnsiString Message) : Exception(Message) {;}
};

//error imposible
class EImpossibleError : public Exception {
public:
        EImpossibleError(AnsiString Message) : Exception(Message) {;}
};

//EImpossibleError sirve para indicar los errores que en teoría nunca
//pueden llegar a producirse pero que por alguna razón llegan a producirse
//y detectarse en alguna comprobación rutinaria.

//---------------------------------------------------------------------------
#endif // EXCEPTIONS_H
