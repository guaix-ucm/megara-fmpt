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
//Archivo: MessageInstruction.cpp
//Contenido: mensaje de instrucción
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "MessageInstruction.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//--------------------------------------------------------------------------
//TMessageInstruction
//--------------------------------------------------------------------------

//PROPIDADES DEL MENSAJE:

void TMessageInstruction::setId(int _Id)
{
        //el número de identificación Id debería ser no negativo
        if(_Id < 0)
                throw EImproperArgument(AnsiString("identification number '")+IntToStr(_Id)+AnsiString("' should be nonnegative"));

        //asigna el nuevo valor
        __Id = _Id;
}

void TMessageInstruction::setComment(const string& _Comment)
{
    p_Comment = _Comment;
}

//PROPIEDADES EN FORMATO TEXTO:

AnsiString TMessageInstruction::getIdText(void) const
{
        return IntToStr(getId());
}
void TMessageInstruction::setIdText(const AnsiString &S)
{
        try {
                setId(StrToInt_(S));
        } catch(...) {
                throw;
        }
}

AnsiString TMessageInstruction::getText(void) const
{
        return getIdText()+AnsiString(": ")+Instruction.getText();
}

void TMessageInstruction::setText(const AnsiString &S)
{
        try {
                int i = 1;
                Read(this, S, i);
                StrTravelToEnd(S, i);
        } catch(...) {
                throw;
        }
}

//MÉTODOS ESTÁTICOS:

//compara los identificadores de dos mensajes de instrucción
int  TMessageInstruction::CompareIds(const TMessageInstruction *MI1,
        const TMessageInstruction *MI2)
{
        //el puntero MI1 debería apuntar a un mensaje de instrucción construido
        if(MI1 == NULL)
                throw EImproperArgument("pointer MI1 should point to built instruction message");

        //el puntero MI2 debería apuntar a un mensaje de instrucción construido
        if(MI2 == NULL)
                throw EImproperArgument("pinter MI2 should point to built instruction message");

        if(MI1->getId() < MI2->getId())
                return -1;
        if(MI1->getId() > MI2->getId())
                return 1;

        return 0;
}
//imprime un mensaje de instrucción en una cadena
void  TMessageInstruction::Print(AnsiString &S,
        const TMessageInstruction *MI)
{
        //el puntero MI debería apuntar a un mensaje de instrucción construido
        if(MI == NULL)
                throw EImproperArgument("pointer MI should point to built instruction message");

        S += MI->getText();
}
//lee un mensaje de itnrucción en una cadena de texto
void  TMessageInstruction::Read(TMessageInstruction *MI,
        const AnsiString &S, int &i)
{
        //el puntero MI debería apuntar a un mensaje de instrucción construido
        if(MI == NULL)
                throw EImproperArgument("pointer MI should point to built instruction message");

        //el índice i debería indicar al primer caracter de un mensaje de instrucción
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("instruction message not found");

        //estado de la máquina lectora
        //      0: leyendo ' ' y esperando primer caracter de la primera palabra
        //      1: leyendo la primera palabra y esperando ' ' o ':'
        //      2: leyendo espacios y esperando ':'
        //      3: leyendo la instrucción
        //      4: mensaje leido con éxito
        int status = 0;


        char c; //caracter indicado de la cadena
        AnsiString Word; //palabra leida
        TMessageInstruction _MI; //variables tampón

        do {
                c = S[i]; //lee el próximo caracter de la cadena

                //reacciona en función del estado y del caracter
                switch(status) {
                        case 0: //leyendo ' ' y esperando primer caracter de la primera palabra
                                switch(c) {
                                        case ' ':
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("number identifier Id not found");
                                                break;
                                        default:
                                                Word = c; //lee el primer caracter de la primera palabra
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("character ':' not found");
                                                status++;
                                                break;
                                }
                                break;
                        case 1: //leyendo la primera palabra y esperando ' ' o ':'
                                switch(c) {
                                        case ' ':
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("character ':' not found");
                                                break;
                                        case ':':
                                                try {
                                                        _MI.getIdText() = Word;
                                                } catch(EImproperArgument &E) {
                                                        throw EImproperArgument(E.Message+AnsiString(" for property Id"));
                                                } catch(...) {
                                                        throw;
                                                }
                                                i++;
                                                if(i > S.Length())
                                                        status = 4; //indica caracter leido con éxito
                                                else
                                                        status = 3; //pasa a leer la instrucción
                                                break;
                                        default:
                                                Word += c;
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("character ':' not found");
                                }
                                break;
                        case 2: //leyendo espacios y esperando ':'
                                switch(c) {
                                        case ' ':
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("character ':' not found");
                                                break;
                                        case ':':
                                                try {
                                                        _MI.setIdText(Word);
                                                } catch(...) {
                                                        throw;
                                                }
                                                i++;
                                                if(i > S.Length())
                                                        status = 4; //indica mensaje leido con éxito
                                                else
                                                        status++; //pasa a leer la instrucción
                                                break;
                                        default:
                                                throw EImproperArgument(AnsiString("character '")+AnsiString(c)+AnsiString("' should be ':'"));
                                }
                                break;
                        case 3: //leyendo la instrucción
                                try {
                                        TInstruction::Read(&(_MI.Instruction), S, i);
                                } catch(...) {
                                        throw;
                                }
                                status = 4; //indica mensaje leido con éxito
                                break;
                }
        //mientras el mensaje no haya sido leido con éxito
        } while(status < 4);

        //asigna la variable tampón
        *MI = _MI;
}

//MÉTODOS PÚBLICOS:

//construye un mensaje con los valores por defecto
TMessageInstruction::TMessageInstruction(void) : __Id(0), Instruction(), p_Comment("")
{
}
//construye un mensaje con los valores por defecto
TMessageInstruction::TMessageInstruction(int _Id,
        AnsiString InstructionText) :
        Instruction(), p_Comment()
{
        try {
                setId(_Id);
                Instruction.setText(InstructionText);
        } catch(...) {
                throw;
        }
}
//clona un ensaje
TMessageInstruction::TMessageInstruction(TMessageInstruction *MI) :
        __Id(), Instruction(), p_Comment()
{
        try {
                Copy(MI);
        } catch(...) {
                throw;
        }
}

//copia las propiedades de un mensaje
void TMessageInstruction::Copy(const TMessageInstruction *MI)
{
        //el puntero MI debería apuntar a una mensaje de instrucción contruido
        if(MI == NULL)
                throw EImproperArgument("pointer MI should point to built instruction message");

        //copia las propiedades
        __Id = MI->__Id;
        Instruction = MI->Instruction;
        p_Comment = MI->p_Comment;
}
//asigna las propiedades de un mensaje
TMessageInstruction& TMessageInstruction::operator=(const TMessageInstruction &MI)
{
        //copia las propiedades
        __Id = MI.__Id;
        Instruction = MI.Instruction;
        p_Comment = MI.p_Comment;

        //devuelve una referencia a este mensaje para poder concatenar asignaciones
        return *this;
}

//determine if a MI is different to this MI
bool TMessageInstruction::operator!=(const TMessageInstruction& MI) const
{
    if(getId() != MI.getId())
        return true;

    if(Instruction != MI.Instruction)
        return true;

    if(getComment() != MI.getComment())
        return true;

    return false;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

