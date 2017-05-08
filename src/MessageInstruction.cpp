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
//File: MessageInstruction.cpp
//Content: message of instruction
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "MessageInstruction.h"

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//--------------------------------------------------------------------------
//TMessageInstruction
//--------------------------------------------------------------------------

//PUBLIC PROPERTIES:

void TMessageInstruction::setId(int Id)
{
    //comprueba la precondicion
    if(Id < 0)
        throw EImproperArgument("identification number '"+inttostr(Id)+"' should be nonnegative");

    //asigna el nuevo valor
    p_Id = Id;
}

void TMessageInstruction::setCommentDsec(const string& CommentDsec)
{
    p_CommentDsec = CommentDsec;
}

void TMessageInstruction::setCommentDmin(const string& CommentDmin)
{
    p_CommentDmin = CommentDmin;
}

void TMessageInstruction::setCommentDend(const string& CommentDend)
{
    p_CommentDend = CommentDend;
}

//PUBLIC PROPERTIES IN TEXT FORMAT:

AnsiString TMessageInstruction::getIdText(void) const
{
    return IntToStr(getId());
}
void TMessageInstruction::setIdText(const AnsiString &S)
{
    try {
        setId(StrToInt(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting Id in text format: ");
        throw;
    }
}

AnsiString TMessageInstruction::getText(void) const
{
    return getIdText()+AnsiString(": ")+Instruction.getText();
}
AnsiString TMessageInstruction::getTextForComment(void) const
{
    return AnsiString("rp")+getIdText()+AnsiString(", ")+Instruction.getTextForComment();
}

void TMessageInstruction::setText(const AnsiString &S)
{
    try {
        //read the message instruction from the first position
        int i = 1;
        Read(this, S, i);

        //search unexpected text
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text translating string to message instruction");

    } catch(Exception& E) {
        E.Message.Insert(1, "translating string to message instruction: ");
        throw;
    }
}

//STATIC METHODS:

//compare the identifiers of two MIs
int  TMessageInstruction::CompareIds(const TMessageInstruction *MI1,
                                     const TMessageInstruction *MI2)
{
    //el puntero MI1 debería apuntar a un mensaje de instrucción construido
    if(MI1 == NULL)
        throw EImproperArgument("pointer MI1 should point to built instruction message");

    //el puntero MI2 debería apuntar a un mensaje de instrucción construido
    if(MI2 == NULL)
        throw EImproperArgument("pointer MI2 should point to built instruction message");

    if(MI1->getId() < MI2->getId())
        return -1;
    if(MI1->getId() > MI2->getId())
        return 1;

    return 0;
}
//print the MI in a text string
void  TMessageInstruction::Print(AnsiString &S,
                                 const TMessageInstruction *MI)
{
    //el puntero MI debería apuntar a un mensaje de instrucción construido
    if(MI == NULL)
        throw EImproperArgument("pointer MI should point to built instruction message");

    S += MI->getText();
}
//read the MI from a text string
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
    AnsiString Word; //palabra leída
    TMessageInstruction t_MI; //variables tampón

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
                t_MI.getIdText() = Word;
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
                t_MI.setIdText(Word);
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
            TInstruction::Read(&(t_MI.Instruction), S, i);
        } catch(...) {
            throw;
        }
            status = 4; //indica mensaje leido con éxito
            break;
        }
        //mientras el mensaje no haya sido leido con éxito
    } while(status < 4);

    //asigna la variable tampón
    *MI = t_MI;
}

//read an instruction from a text string
//in the MCS format
void  TMessageInstruction::readMCS(TMessageInstruction *MI,
                                         const string& str, unsigned int &i)
{
    //check the preconditions
    if(MI == NULL)
        throw EImproperArgument("pointer MI should point to built message instruction");
    if(str.length() < i)
        throw EImproperArgument("index i should indicate a position in the string str");

    try {
        //tampon variables
        int RPId1, RPId2;
        double r1, r2;

        strTravelSeparatorsIfAny(str, i);
        strTravelLabel("rp", str, i);
        strReadInt(RPId1, str, i);
        strTravelSeparators(str, i);
        strTravelLabel("r1", str, i);
        strTravelSeparators(str, i);
        strReadFloat(r1, str, i);
        strTravelSeparators(str, i);
        strTravelLabel("rp", str, i);
        strReadInt(RPId2, str, i);

        if(RPId2 != RPId1)
            throw EImproperArgument("unequal RP identifiers");

        strTravelSeparators(str, i);
        strTravelLabel("r2",str, i);
        strTravelSeparators(str, i);
        strReadFloat(r2, str, i);

        if(RPId1 < 0)
            throw EImproperArgument("RP identifier should be nonnegative");

        //Note that RP identifier could be zero.

        //set the tampon variables
        MI->Instruction.setName("MM");
        MI->Instruction.Args.setCount(2);
        MI->Instruction.Args[0] = r1;
        MI->Instruction.Args[1] = -r2;
        MI->setId(RPId1);

    } catch(Exception& E) {
        E.Message.Insert(1, "reading message instruction: ");
        throw;
    }
}

//PUBLIC METHODS:

//build a MI by default
TMessageInstruction::TMessageInstruction(void) :
    p_Id(0), p_CommentDsec(""), p_CommentDmin(""), p_CommentDend(""), Instruction()
{
}
//build a MI with the indicated values
TMessageInstruction::TMessageInstruction(int Id, AnsiString InstructionText) :
    p_CommentDsec(""), p_CommentDmin(""), p_CommentDend(""), Instruction()
{
    try {
        setId(Id);
        Instruction.setText(InstructionText);
    } catch(Exception& E) {
        E.Message.Insert(1, "building a meessage instruction: ");
        throw;
    }
}
//clone a MI
TMessageInstruction::TMessageInstruction(TMessageInstruction *MI) :
    p_Id(), p_CommentDsec(), p_CommentDmin(), p_CommentDend(""), Instruction()
{
    try {
        Copy(MI);
    } catch(Exception& E) {
        E.Message.Insert(1, "clonning a meessage instruction: ");
        throw;
    }
}

//copy the properties of a MI
void TMessageInstruction::Copy(const TMessageInstruction *MI)
{
    //el puntero MI debería apuntar a una mensaje de instrucción contruido
    if(MI == NULL)
        throw EImproperArgument("pointer MI should point to built instruction message");

    //copia las propiedades
    p_Id = MI->p_Id;
    Instruction = MI->Instruction;
    p_CommentDsec = MI->p_CommentDsec;
    p_CommentDmin = MI->p_CommentDmin;
    p_CommentDend = MI->p_CommentDend;
}
//assign the properties of a MI
TMessageInstruction& TMessageInstruction::operator=(const TMessageInstruction &MI)
{
    //copia las propiedades
    p_Id = MI.p_Id;
    Instruction = MI.Instruction;
    p_CommentDsec = MI.p_CommentDsec;
    p_CommentDmin = MI.p_CommentDmin;
    p_CommentDend = MI.p_CommentDend;

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

    if(getCommentDsec() != MI.getCommentDsec())
        return true;

    if(getCommentDmin() != MI.getCommentDmin())
        return true;

    if(getCommentDend() != MI.getCommentDend())
        return true;

    return false;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

