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

//determines if a ML is different to this ML
bool TMessageList::operator!=(const TMessageList& ML) const
{
    if(ML.getCount() != getCount())
        return true;

    for(int i=0; i<getCount(); i++) {
        const TMessageInstruction *MIthis = Items[i];
        const TMessageInstruction *MIother = ML.GetPointer(i);

        if(*MIthis != *MIother)
            return true;
    }

    return false;
}

//PUBLIC METHODS:

//construye un clon de una lista de MIs basada en punteros
TMessageList::TMessageList(const TMessageList *L)
//se construye aquí el array deslizante por defecto
{
    //check the precondition
    if(L == NULL)
        throw EImproperArgument("pointer L should point to built list");

    //clona el array deslizante
    Items.Clone(L->Items);

    //apunta a las mismas funciones externas
    Compare = L->Compare;
    Evaluate = L->Evaluate;
    Assign = L->Assign;
    Print = L->Print;
    Read = L->Read;
}

//determines if a MP is different to this MP
bool TMotionProgram::operator!=(const TMotionProgram& MP) const
{
    if(MP.getCount() != getCount())
        return true;

    for(int i=0; i<getCount(); i++) {
        const TMessageList *MLthis = Items[i];
        const TMessageList *MLother = MP.GetPointer(i);

        if(*MLthis != *MLother)
            return true;
    }

    return false;
}

//--------------------------------------------------------------------------
//TMotionProgram
//--------------------------------------------------------------------------

//get the non empty coments of the motion program
//in column text format
string TMotionProgram::getCommentsColumnText(void) const
{
    string str;

    for(int i=0; i<getCount(); i++) {
        const TMessageList *ML = Items[i];
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);

            if(MI->getComment().length() > 0) {
                if(str.length() > 0)
                    str += "\r\n";
                str += "group"+inttostr(i+1);
                str += ": "+MI->getText().str;
                str += ": "+MI->getComment();
            }
        }
    }

    return str;
}


//--------------------------------------------------------------------------

} //namespace Positiong

//---------------------------------------------------------------------------
