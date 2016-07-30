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
//File: Instruction.cpp
//Content: instruction of a RP
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Instruction.h"
#include "Strings.h"

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres de modelos
namespace Models {

//###########################################################################
//TInstruction
//###########################################################################

//clase instrucción de posicionador
void TInstruction::setName(AnsiString Name)
{
    //el nombre de instrucción debería ser conocido
    if(Name == "") {
        Args.Clear();
    } else if(Name == "MA") {
        Args.Clear();
    } else if(Name == "MS") {
        Args.Clear();
    } else if(Name == "M1") {
        Args.setCount(1);
    } else if(Name == "M2") {
        Args.setCount(1);
    } else if(Name == "MM") {
        Args.setCount(2);
    } else if(Name == "ST") {
        Args.Clear();
    } else if(Name == "SP") {
        Args.Clear();
    } else { //si no (es que no conoce el nombre de instrucción)
        //indica que el nombre de instrucción no es conocido
        throw EImproperArgument(AnsiString("instruction name \"")+Name+AnsiString("\" should be known"));
    }

    //asigna el nuevo valor
    p_Name = Name;
}

//PROPIEDADES EN FORMATO TEXTO:

AnsiString TInstruction::getText(void) const
{
    AnsiString S;

    //asigna el nombre de la instrucción
    S = getName();

    //cncatena los argumentos de la intrucción en formato texto
    for(int i=0; i<Args.getCount(); i++)
        S += AnsiString(" ")+Args.getItemsText(i);

    return S;
}

void TInstruction::setText(const AnsiString &S)
{
    int i = 1; //initialize the index to the first position of the string

    try {
        //read the instruction in a tampon variable
        TInstruction I;
        Read(&I, S, i);

        //look for unexpected text
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text: "+StrFirstChars(S.SubString(i, S.Length() - i + 1)).str);

        //clone the tampon variable
        *this = I;
    }
    catch(Exception& E) {
        unsigned int row, col;
        strPositionToCoordinates(row, col, S.str, i-1);
        E.Message.Insert(1, "setting instruction in text format in row "+inttostr(row)+" and column "+inttostr(col)+": ");
        throw;
    }
}

//MÉTODOS ESTÁTICOS:

//imprime una instrucción en una cadena de texto
void TInstruction::Print(AnsiString &S, const TInstruction *I)
{
    //el puntro I debería apuntar a una instrucción construida
    if(I == NULL)
        throw EImproperArgument("pointer I should point to built instruction");

    S += I->getText();
}
//lee una instrucción en una cadena de texto
void  TInstruction::Read(TInstruction *I, const AnsiString &S, int &i)
{
    //comprueba las precondiciones
    if(I == NULL)
        throw EImproperArgument("pointer I should point to built instruction");
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("instruction not found");

    //ADVERTENCIA: si la cadena S solo contiene espacios o está vacía
    //corresponde a la instrucción por defecto.

    //estado de lectura
    //      0: esperando ' ' o primer caracter de la primera palabra
    //      1: leyendo la primera palabra y esperando ' '
    //      2: esperando ' ' o primer caracter de la segunda palabra
    //      3: esperando ' ' o primer caracter de la tercera palabra
    //      4: instrucción leída con éxito
    int status = 0;

    char c; //caracter indicado S[i]
    AnsiString Word; //palabra
    TInstruction t_I; //variable tampón

    //si la cadena a partir de i está vacía
    if(i > S.Length()) {
        *I = t_I; //asigna la instrucción por defecto
        return; //no hace nada más
    }

    do {
        c = S[i]; //asigna el próximo caracter para facilitar su acceso

        switch(status) {
        case 0: //esperando ' ' o primer caracter de la primera palabra
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    status = 4;
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    status = 4;
                break;
            case 'M': case 'G': case 'S':
                Word = c;
                i++;
                if(i > S.Length())
                    try {
                    t_I.setName(Word);
                    if(t_I.Args.getCount() > 0)
                        throw EImproperArgument("first argument not found");
                    status = 4;
                } catch(EImproperArgument &E) {
                    throw EImproperArgument(E.Message+AnsiString(" for property Name"));
                } catch(...) {
                throw;
            }
                else
                status = 1;
                break;
            default:
                status = 4;
            }
            break;
        case 1: //leyendo la primera palabra y esperando ' '
            switch(c) {
            case ' ':
                try {
                t_I.setName(Word);
            } catch(EImproperArgument &E) {
                    throw EImproperArgument(E.Message+AnsiString(" for property Name"));
                } catch(...) {
                throw;
            }
                i++;
                if(i > S.Length())
                    if(t_I.Args.getCount() > 0)
                        throw EImproperArgument("first argument not found");
                    else
                        status = 4;
                else
                    status = 2;
                break;
            case '\r':
                try {
                t_I.setName(Word);
            } catch(...) {
                throw;
            }
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    if(t_I.Args.getCount() > 0)
                        throw EImproperArgument("first argument not found");
                    else
                        status = 4;
                else
                    status = 2;
                break;
            default:
                Word += c;
                i++;
                if(i > S.Length())
                    try {
                    t_I.setName(Word);
                    if(t_I.Args.getCount() > 0)
                        throw EImproperArgument("first argument not found");
                    status = 4;
                } catch(EImproperArgument &E) {
                    throw EImproperArgument(E.Message+AnsiString(" for property Name"));
                } catch(...) {
                throw;
            }
            }
            break;
        case 2: //esperando ' ' o primer caracter de la segunda palabra
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("first argument not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("first argument not found");
                break;
            default:
                try {
                StrReadFloatStr(Word, S, i);
                t_I.Args.setItemsText(0, Word);
            } catch(EImproperArgument &E) {
                    throw EImproperArgument(E.Message+AnsiString(" for property Args[0]"));
                } catch(...) {
                throw;
            }

                //reaciona según el número de argumentos
                if(t_I.Args.getCount() > 1) {
                    if(i > S.Length())
                        throw EImproperArgument("second argument not found");
                    status = 3;
                } else {
                    status = 4;
                }
            }
            break;
        case 3: //esperando ' ' o primer caracter de la tercera palabra
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("second argument not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("second argument not found");
                break;
            default:
                try {
                StrReadFloatStr(Word, S, i);
                t_I.Args.setItemsText(1, Word);
            } catch(EImproperArgument &E) {
                    throw EImproperArgument(E.Message+AnsiString(" for property Args[1]"));
                } catch(...) {
                throw;
            }

                //reaciona según el número de argumentos
                if(t_I.Args.getCount() > 2)
                    throw EImpossibleError("there isn't instruction with more two arguments");
                else
                    status = 4;
            }
            break;
        }
        //mientras la instrucción no hya sido leída con éxito
    } while(status < 4);

    //asigna la variable tampón
    *I = t_I;
}

//MÉTODOS PÚBLICOS:

//contruye una instrucción de posicionador
TInstruction::TInstruction(void) : p_Name(""), Args(2)
{
}

//asigna una instrucción
TInstruction& TInstruction::operator=(const TInstruction& Instruction)
{
    p_Name = Instruction.getName();
    Args = Instruction.Args;

    return *this;
}

//determines if an instruction is different to this instruction
bool TInstruction::operator!=(const TInstruction& I) const
{
    AnsiString NameThis = StrTrim(getName());
    AnsiString NameOther = StrTrim(I.getName());
    if(NameThis != NameOther)
        return true;

    if(Args.getCount() != I.Args.getCount())
        return true;

    for(int i=0; i<Args.getCount(); i++) {
        double argThis = Args[i];
        double argOther = I.Args[i];

        if(argThis != argOther)
            return true;
    }

    return false;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

