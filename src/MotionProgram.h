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
//Archivo: PositioningProgram.h
//Contenido: programa de posicionamiento
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef MOTIONPROGRAM_H
#define MOTIONPROGRAM_H

#include "MessageInstruction.h"
#include "PointersList.h"

//##include <QListWidget>
//##include <QTextEdit>

//--------------------------------------------------------------------------

using namespace Lists;
using namespace Models;

//espacio de nombres de posicionamiento
namespace Positioning {

//--------------------------------------------------------------------------
//TMessageList:
//--------------------------------------------------------------------------

//lista de mensajes de instrucción
class TMessageList : public TPointersList<TMessageInstruction> {
    //        AnsiString __Label; void SetLabel(const AnsiString&);

public:
    /*        //PROPIEDADES EN FORMATO TEXTO:

        //etiqueta de la lista de mensajes
        //debe ser una cadena de texto imprimible
        //valor por defecto: ""
        __property AnsiString Label = {read=__Label, write=SetLabel};

        //NOTA: cada lista de mensajes debe tener una etiqueta
*/
    //MÉTODOS ESTÁTICOS:

    //imprime una lista en una cadena de texto
    static void  PrintMessageList(AnsiString &S,
                                  const TMessageList *L);
    //lee una lista de emnsajes en una cadena de texto
    static void  ReadMessageList(TMessageList *L,
                                 const AnsiString &S, int &i);

    //construye una lista de mensajes
    TMessageList(int _Capacity=1003) :
        TPointersList<TMessageInstruction>(_Capacity,
                                           TMessageInstruction::CompareIds,
                                           NULL, NULL,
                                           TMessageInstruction::Print,
                                           TMessageInstruction::Read) {;}

    //imprime los mensajes de una lista en una caja de lista
    //#void Print(QListWidget*);
};

//--------------------------------------------------------------------------
//TMotionProgram:
//--------------------------------------------------------------------------

//programa de movimiento
class TMotionProgram : public TPointersList<TMessageList> {
public:
    //contruye un programa de posicionamientos
    TMotionProgram(int _Capacity=7) :
        TPointersList<TMessageList>(_Capacity,
                                    NULL, NULL, NULL,
                                    TMessageList::PrintMessageList,
                                    TMessageList::ReadMessageList) {;}
    /*#
        //imprime las etiquetas de las listas de mensajes de un programa
        //en una caja de lista
        void Print(QListWidget *LB);

        //imprime el programa de movimiento en un memorando
        void Print(QTextEdit*);*/
};

//--------------------------------------------------------------------------

} //namespace Positiong

//---------------------------------------------------------------------------
#endif // MOTIONPROGRAM_H
