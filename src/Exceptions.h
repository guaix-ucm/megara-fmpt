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
//Archivo: Exceptions.h
//Contenido: clases de excepciones
//Última actualización: 03/10/2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "vclemu.h" //AnsiString

//#include <QCoreApplication> //std::exception
#include <exception>
#include <time.h> //time

//---------------------------------------------------------------------------

using namespace std;

//---------------------------------------------------------------------------

//ADVERTENCIA: todas las excepciones deben derivarse de Exception para
//permitir su captura unificada.

//invocación indevida de método o función
class EImproperCall : public Exception {
public:
    EImproperCall(AnsiString _Message) : Exception(_Message) {;}
};

//argumento de método o función incorrecto
class EImproperArgument : public Exception {
public:
    EImproperArgument(AnsiString _Message) : Exception(_Message) {;}
};

//estado o configuración local incorrecta
class EImproperSetting : public Exception {
public:
    EImproperSetting(AnsiString _Message) : Exception(_Message) {;}
};

//no puede completar una operación, un argumento o una función
class ECantComplete : public Exception {
public:
    ECantComplete(AnsiString _Message) : Exception(_Message) {;}
};

//operación completada
class EComplete : public Exception {
public:
    EComplete(AnsiString _Message) : Exception(_Message) {;}
};

//no puede encontrar solución
class ECantFindSolution : public Exception {
public:
    ECantFindSolution(AnsiString _Message) : Exception(_Message) {;}
};

//valor cargado de archivo inapropiado
class EImproperFileLoadedValue : public Exception {
public:
        EImproperFileLoadedValue(AnsiString _Message) : Exception(_Message) {;}
};

/*//objeto no construido
class ENoBuiltObject : public Exception {
public:
        ENoBuiltObject(AnsiString _Message) : Exception(_Message) {;}
};

//ENoBuiltObject sirve para indicar que un objeto que se está intentando
//utilizar no está construido, por eemplo para destruirlo.
//EImproperCall no se considera adecuado para indicar esto,
//ya que sirve para llevar un control de configuración en vez de existencia.
*/
//error imposible
class EImpossibleError : public Exception {
public:
        EImpossibleError(AnsiString _Message) : Exception(_Message) {;}
};

//EImpossibleError sirve para indicar los errores que en teoría nunca
//pueden llegar a producirse pero que por alguna razón llegan a producirse
//y detectarse en alguna comprobación rutinaria.

//---------------------------------------------------------------------------
#endif // EXCEPTIONS_H
