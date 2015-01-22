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
//Archivo: MotionProgram.cpp
//Contenido: programa de posicionamiento
//Última actualización: 28/04/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "MotionProgram.h"
#include "Strings.h"

//--------------------------------------------------------------------------

//espacio de nombres de posicionamiento
namespace Positioning {

using namespace Strings; //StrIndent

//--------------------------------------------------------------------------
//TMessageList
//--------------------------------------------------------------------------

//MÉTODOS ESTÁTICOS:

//imprime una lista en una cadena de texto
void  TMessageList::PrintMessageList(AnsiString &S,
                                     const TMessageList *L)
{
    try {
        PrintList(S, L);
    } catch(...) {
        throw;
    }
}
//lee una lista de emnsajes en una cadena de texto
void  TMessageList::ReadMessageList(TMessageList *L,
                                    const AnsiString &S, int &i)
{
    try {
        ReadList(L, S, i);
    } catch(...) {
        throw;
    }
}
/*
//PROPIEDADES EN FORMATO TEXTO:

void TMessageList::SetLabel(const AnsiString &S)
{
        //la etiqueta Label debería ser un acadena de texto imprimible
        if(StrIsntPrintable(S))
                throw EImproperArgument("label Label should be a printable string");

        //asigna el nuevo valor
        __Label = S;

        //la cadena S debería contener al menos los caracteres delimitadores []
        if(S.Length() < 2)
                throw EImproperArgument("string '"+S+"' should contain the delimiters characters [] almost");

        //segrega el primer caracter de la cadena
        char ch = S[1];

        //el primer caracter de la cadena de texto S debería ser '['
        if(ch != '[')
                throw EImproperArgument("first char in string '"+S+"' should be '['");

        //segrega el último caractr de la cadena
        ch = S[S.Length()];

        //el último caracter de la cadena de texto S debería ser '['
        if(ch != ']')
                throw EImproperArgument("last char in string '"+S+"' should be ']'");

        //la cadena S debería ser imprimible
        if(StrIsntPrintable(S))
                throw EImproperArgument("string '"+S+"' should be a printable string");

        //la cadena S debería contener una sola linea de texto
        if(StrCountLines(S) != 1)
                throw EImproperArgument("string '"+S+"' should contain only one line text");

        //asigna el nuevo valor
        __LabelText = S;
}    */

//MÉTODOS PÚBLICOS:
/*#
//imprime los mensajes de una lista y adscribe la lista
void TMessageList::Print(QListWidget *LB)
{
        //el puntero LB debería apuntar a una caja de lista contruida
        if(LB == NULL)
                throw EImproperArgument("pointer LB should point to built list box");

        //limpia la caja de lista en congruencia con el estado inicial
        LB->clear();

        //por cada mensaje de la lista
        for(int i=0; i<getCount(); i++) {
                //imprime el mensaje indicado
            QString QS(Items[i]->getText().c_str());
                LB->addItem(QS);
        }
}
*/
//--------------------------------------------------------------------------
//TMotionProgram
//--------------------------------------------------------------------------
/*#
//imprime las etiquetas de las listas de mensajes de un programa
//en una caja de lista
void TMotionProgram::Print(QListWidget *LB)
{
        //el puntero LB debería apuntar a una caja de lista contruida
        if(LB == NULL)
                throw EImproperArgument("pointer LB should point to built list box");

        //limpia la caja de lista en congruencia con el estado inicial
        LB->clear();

        //por cada lista de mensajes del programa
        for(int i=0; i<getCount(); i++) {
                //imprime la etiqueta indicada
            QString QS("message list "); //quizás debería ser Items[i]->Label
            QS += IntToStr(i).c_str();
                LB->addItem(QS);
        }
}

//imprime el programa de movimiento en un memorando
void TMotionProgram::Print(QTextEdit *Memo)
{
//        //todas las listas de mensajes deben tener apuntada la función de impresión TMessageInstruction::Print
  //      for(int i=0; i<Count; i++)
    //            if(Items[i]->Print != TMessageInstruction::Print)
      //                  throw EImproperCall("all message list should be pointed the print function TMessageInstruction::Print");

        //el puntero Memo debería apuntar a un memorando contruido
        if(Memo == NULL)
                throw EImproperArgument("pointer Memo should point to built memo");

        //limpia el memorando en congruencia con el estado inicial
        Memo->clear();

        //por cada lista de mensajes del programa
        for(int i=0; i<getCount(); i++) {
                //apunta la lista de mensajes indicada para facilitar su acceso
                TMessageList *ML = Items[i];
                //imprime la etiqueta de la lista de mensajes
                QString QS("MessajeList");
                QS += IntToStr(i+1).c_str();
                Memo->append(QS);
                //imprime la lista de mensajes indentada
                Memo->append(StrIndent(ML->getColumnText()).c_str());
        }
}
*/
//--------------------------------------------------------------------------

} //namespace Positiong

//---------------------------------------------------------------------------
