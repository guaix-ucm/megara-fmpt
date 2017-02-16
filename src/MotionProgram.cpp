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
//File: PositioningProgram.cpp
//Content: positioning program
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "MotionProgram.h"
#include "Strings.h"

//---------------------------------------------------------------------------

using namespace Strings; //StrIndent
using namespace Models;

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//TMessageList
//---------------------------------------------------------------------------

//MÉTODOS ESTÁTICOS:

//imprime una lista en una cadena de texto
void  TMessageList::PrintMessageList(AnsiString &S,
                                     const TMessageList *L)
{
    try {
        PrintList(S, L);
    } catch(Exception& E) {
        E.Message.Insert(1, "printing message list: ");
        throw;
    }
}
//lee una lista de emnsajes en una cadena de texto
void  TMessageList::ReadMessageList(TMessageList *L,
                                    const AnsiString &S, int &i)
{
    try {
        ReadList(L, S, i);
    } catch(Exception& E) {
        E.Message.Insert(1, "reading message list: ");
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

//---------------------------------------------------------------------------
//TMotionProgram
//---------------------------------------------------------------------------

//determines if there is some coment 1 in any instruction
//of the the motion program
bool TMotionProgram::thereIsSomeComment1(void) const
{
    for(int i=0; i<getCount(); i++) {
        const TMessageList *ML = Items[i];
        for(int j=0;  j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);
            if(MI->getComment1().length() > 0)
                return true;
        }
    }
    return false;
}
//get the non empty coments of the motion program
//in column text format
string TMotionProgram::getComment1sColumnText(void) const
{
    string str;

    for(int i=0; i<getCount(); i++) {
        const TMessageList *ML = Items[i];
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);

            if(MI->getComment1().length() > 0) {
                if(str.length() > 0)
                    str += "\r\n";
                str += "group"+inttostr(i+1);
                str += ": "+MI->getText().str;
                str += ": "+MI->getComment1();
            }
        }
    }

    return str;
}


//get the list of RP identifiers includes in a MP
void TMotionProgram::getAllIncludedIds(TVector<int>& Ids) const
{
    //initialize the output
    Ids.Clear();

    //add the Ids of the MP avoiding repetitions
    for(int i=0; i<getCount(); i++) {
        const TMessageList *ML = Items[i];
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);
            int Id = MI->getId();

            int k = Ids.Search(Id);
            if(k >= Ids.getCount())
                Ids.Add(Id);
        }
    }
}

//exclude the isntructions addressed to a determined RP
void TMotionProgram::excludeRP(int Id)
{
    for(int i=0; i<getCount(); i++) {
        TMessageList *ML = Items[i];

        int j = 0;
        while(j < ML->getCount()) {
            TMessageInstruction *MI = ML->GetPointer(j);
            if(MI->getId() == Id)
                ML->Delete(j);
            else
                j++;
        }
    }
}

//Get a motion progam in the interface format of the MCS.
//Inputs:
//  label: string labeling all the MP.
//  Bid: univoque identifier of the CB.
//  SPL: starting position list for all RPs of the Fiber MOS.
//  r2_negative: indicates if the rotor 2 coordinates must be negative.
//Preconditions:
//  All PPAs of the SPL must be addresed to different RPs.
//  All RPs included in the MP, must be in included in the SPL.
void TMotionProgram::getInterfaceText(string& str, const string& label, unsigned int Bid,
    const TPairPositionAnglesList& SPL, bool r2_negative) const
{
    //VERIFIES THE PRECONDITIONS:

    if(label!="pos" && label!="depos")
        throw EImproperArgument("MP label should be \"pos\" or \"depos\"");

    if(int(Bid) < 0)
        throw EImproperArgument("block identifier Bid should be less maximun integer value");

    if(SPL.notAllPPAsAreAddresedToDifferentRPs())
        throw EImproperArgument("all PPAs of the SPL must be addresed to different RPs");

    TVector<int> Ids;
    getAllIncludedIds(Ids);
    if(SPL.notAllIdsAreFound(Ids))
        throw EImproperArgument("all RPs included in the MP, must be in included in the SPL");

    //PRINT THE MOTION PROGRAM:

    //actualize the actual position list
    TPairPositionAnglesList APL = SPL;

    //print the label of the motion program and their start delimiter
    str = label+"_"+inttostr(int(Bid))+" {";

    //for each list of message of instructions of the motion program
    //  print a group
    for(int i=0; i<getCount(); i++) {
        //points the indicated message list to facilitate its access
        const TMessageList *ML = Items[i];

        //PRINT THE GROUP CORRESPONDING TO THE INDICATED MESSAGE LIST:

        //print the label of the indicated group and their start delimiter
        str += "\r\n\tgroup_"+IntToStr(i+1).str+" {";

        //print the label of the indicated group and their start delimiter
        //inserting zeros on the left:	
        //str += "\r\n\tgroup_"+strInsertChar(IntToStr(i+1), 2).str+" {";

        //for each MI of the list, prints the corresponding instruction in the str, and actualice the APL
        for(int j=0; j<ML->getCount(); j++) {
            //point the indicated MI to facilitate its access
            const TMessageInstruction *MI = ML->GetPointer(j);

            //search and point the corresponding PPA
            int Id = MI->getId();
            int k = APL.SearchId(Id);
            if(k >= APL.getCount())
                throw EImpossibleError("lateral effect");
            TPairPositionAngles *PPA = APL.GetPointer(k);

            //print the corresponding instruction in the interface format
            if(MI->Instruction.getName() == "M1") {
                //print the final position of the rotors
                double r1_final_position = MI->Instruction.Args[0];
                double r2_final_position = PPA->p___3;

                //print the instruction in the interface format
                str += "\r\n\trp"+MI->getIdText().str+" r1";
                str += " "+floattostr(r1_final_position);
                str += "\r\n\trp"+MI->getIdText().str+" r2";
                if(r2_negative)
                    str += " "+floattostr(-r2_final_position);
                else
                    str += " "+floattostr(r2_final_position);

                //print the instruction in the interface format
                //inserting zeros on the left:
                //str += "\r\n\trp"+strInsertChar(MI->getIdText(), 2).str+" r1";
                //str += " "+floattostr(r1_final_position);
                //str += "\r\n\trp"+strInsertChar(MI->getIdText(), 2).str+" r2";
                //str += " "+floattostr(r2_final_position);

                //actualice the actual position
                PPA->p_1 = r1_final_position;

            } else if(MI->Instruction.getName() == "M2") {
                //print the final position of the rotors
                double r1_final_position = PPA->p_1;
                double r2_final_position = MI->Instruction.Args[0];

                //print the instruction in the interface format
                str += "\r\n\trp"+MI->getIdText().str+" r1";
                str += " "+floattostr(r1_final_position);
                str += "\r\n\trp"+MI->getIdText().str+" r2";
                if(r2_negative)
                    str += " "+floattostr(-r2_final_position);
                else
                    str += " "+floattostr(r2_final_position);

                //print the instruction in the interface format
                //inserting zeros on the left:
                //str += "\r\n\trp"+strInsertChar(MI->getIdText(), 2).str+" r1";
                //str += " "+floattostr(r1_final_position);
                //str += "\r\n\trp"+strInsertChar(MI->getIdText(), 2).str+" r2";
                //str += " "+floattostr(r2_final_position);

                //actualice the actual position
                PPA->p___3 = r2_final_position;

            } else if(MI->Instruction.getName() == "MM") {
                //print the final position of the rotors
                double r1_final_position = MI->Instruction.Args[0];
                double r2_final_position = MI->Instruction.Args[1];

                //print the instruction in the interface format
                str += "\r\n\trp"+MI->getIdText().str+" r1";
                str += " "+floattostr(r1_final_position);
                str += "\r\n\trp"+MI->getIdText().str+" r2";
                if(r2_negative)
                    str += " "+floattostr(-r2_final_position);
                else
                    str += " "+floattostr(r2_final_position);

                //print the instruction in the interface format
                //inserting zeros on the left:
                //str += "\r\n\trp"+strInsertChar(MI->getIdText(), 2).str+" r1";
                //str += " "+floattostr(r1_final_position);
                //str += "\r\n\trp"+strInsertChar(MI->getIdText(), 2).str+" r2";
                //str += " "+floattostr(r2_final_position);

                //actualice the actual position
                PPA->p_1 = r1_final_position;
                PPA->p___3 = r2_final_position;

            } else
                throw EImpossibleError("lateral effect");
        }

        //print the end delimiter of the indicated group
        str += "\r\n\t}";
    }
    //print the end delimiter of motion program
    str += "\r\n}";
}

//Set a motion progam in the interface format of the MCS.
void TMotionProgram::setInterfaceText(string& label, unsigned int& Bid, const string& str)
{
    //initialize the index to the first position of the string
    unsigned int i = 0;

    //status machine:
    //  0: waiting separator or first char of label                 0 -> 0/1
    //  1: reading label and waiting '_'                            1 -> 2
    //  2: reading CBId and waiting separator or '{'                2 -> 2/3
    //  3: waiting separator, first char of next group label or '}' 3 -> 3/4/6
    //  4: reading next group label, and waiting separator or '{'   4 -> 4/5
    //  5: waiting separator, instruction or '}'                    5 -> 5/5/3
    //  6: MP readed successfully
    char status = 0;

    //tampon variables
    TMotionProgram MP;
    string t_label;
    int t_Bid;

    //auxiliary variables
    string aux;
    TMessageInstruction *MI;
    TMessageList *ML;

    do {
        //if the source string has finished
        if(i >= str.length()) {
            if(status < 6)
                throw EImproperArgument("improper value for motion program");
        }
        //if the index i indicates to a char of the source string
        else {
            char c = str[i++];

            switch(status) {
            case 0: //waiting separator or first char of label                 0 -> 0/1
                if(c==' ' || c=='\t')
                    ; //do nothing
                else if(c == '\r') {
                    if(i < str.length()) {
                        c = str[i++];
                        if(c == '\n')
                            ; //do nothing
                        else
                            throw EImproperArgument("missing '\\n' after '\\r'");
                    } else
                        throw EImproperArgument("missing '\\n' after '\\r'");
                } else {
                    t_label += c;
                    status++;
                }
                break;

            case 1: //reading label and waiting '_'                            1 -> 2
                if(c != '_')
                    t_label += c;
                else
                    status++;
                break;

            case 2: //reading CBId and waiting separator or '{'                2 -> 2/3
                if(c==' ' || c=='\t')
                    ; //do nothing
                else if(c == '\r') {
                    if(i < str.length()) {
                        c = str[i++];
                        if(c == '\n')
                            ; //do nothing
                        else
                            throw EImproperArgument("missing '\\n' after '\\r'");
                    } else
                        throw EImproperArgument("missing '\\n' after '\\r'");
                } else if(c != '{')
                    aux += c;
                else //c == '{'
                    try {
                        t_Bid = strToInt(aux);
                        aux = "";
                        status++;
                    } catch(Exception &E) {
                        E.Message.Insert(1, AnsiString("improper value for Bid: "));
                        throw;
                    }
                break;

            case 3: //waiting separator, first char of next group label or '}' 3 -> 3/4/6
                if(c==' ' || c=='\t')
                    ; //do nothing
                else if(c == '\r') {
                    if(i < str.length()) {
                        c = str[i++];
                        if(c == '\n')
                            ; //do nothing
                        else
                            throw EImproperArgument("missing '\\n' after '\\r'");
                    } else
                        throw EImproperArgument("missing '\\n' after '\\r'");
                } else if(c == '}')
                    status = 6;
                else {
                    if(c != '{') {
                        aux += c;
                        status++;
                    } else {
                        throw EImproperArgument("missing next group label in motion program");                    }
                }
                break;

            case 4: //reading next group label, and waiting separator or '{'   4 -> 4/5
                if(c==' ' || c=='\t')
                    ; //do nothing
                else if(c == '\r') {
                    if(i < str.length()) {
                        c = str[i++];
                        if(c == '\n')
                            ; //do nothing
                        else
                            throw EImproperArgument("missing '\\n' after '\\r'");
                    } else
                        throw EImproperArgument("missing '\\n' after '\\r'");
                } else if(c != '{')
                    aux += c;
                else {
                    aux = "";
                    ML = new TMessageList();
                    MP.Add(ML);
                    status++;
                }
                break;

            case 5: //waiting separator, instruction or '}'                    5 -> 5/5/3
                if(c==' ' || c=='\t')
                    ; //do nothing
                else if(c == '\r') {
                    if(i < str.length()) {
                        c = str[i++];
                        if(c == '\n')
                            ; //do nothing
                        else
                            throw EImproperArgument("missing '\\n' after '\\r'");
                    } else
                        throw EImproperArgument("missing '\\n' after '\\r'");
                } else if(c != '}') {
                    MI = new TMessageInstruction();
                    ML->Add(MI);
                    --i;
                    TMessageInstruction::readInterface(MI, str, i);
                } else
                    status = 3;
            }
        }

    //while the MP has not been readed successfully
    } while(status < 6);

    strTravelSeparatorsIfAny(str, i);
    if(i < str.length())
        throw EImproperArgument("improper value for motion program");

    //check the precondition
    if(t_Bid < 0)
        throw EImproperArgument("DP Bid should be nonnegative");

    //set the tampon variables
    *this = MP;
    label = t_label;
    Bid = (unsigned int)t_Bid;
}

//Get MP-Dmins in the interface format of the MCS.
//Inputs:
//  label: string labeling all the MP-Dmin.
//  Bid: univoque identifier of the CB.
void TMotionProgram::getDminInterfaceText(string& str, const string& label,
                                           unsigned int Bid) const
{
    //CHECK THE PRECONDITIONS:

    for(int i=0; i<getCount(); i++) {
        const TMessageList *ML = Items[i];
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);
            if(MI->Instruction.getName()!="M1" &&
                    MI->Instruction.getName()!="M2" &&
                    MI->Instruction.getName()!="MM")
                throw EImproperCall("all instruction should be motion instructions");
        }
    }

    if(label!="pos" && label!="depos")
        throw EImproperArgument("MP label should be \"pos\" or \"depos\"");

    if(int(Bid) < 0)
        throw EImproperArgument("block identifier Bid should be less maximun integer value");

    //PRINT THE MP-Dmin:

    //print the label of the motion program and their start delimiter
    str = label+"_"+inttostr(int(Bid))+" {";

    //for each list of message of instructions of the motion program
    //  print a group
    for(int i=0; i<getCount(); i++) {
        //points the indicated message list to facilitate its access
        const TMessageList *ML = Items[i];

        //PRINT THE GROUP CORRESPONDING TO THE INDICATED MESSAGE LIST:

        //print the label of the indicated group and their start delimiter
        str += "\r\n\tgroup_"+IntToStr(i+1).str+" {";

        //print the label of the indicated group and their start delimiter
        //inserting zeros on the left:
        //str += "\r\n\tgroup_"+strInsertChar(IntToStr(i+1), 2).str+" {";

        //for each MI of the list, prints the corresponding instruction in the str, and actualice the APL
        for(int j=0; j<ML->getCount(); j++) {
            //point the indicated MI to facilitate its access
            const TMessageInstruction *MI = ML->GetPointer(j);

            //print the corresponding instruction in the interface format
            if(MI->Instruction.getName() == "M1") {
                //print the comment with the Dmin
                str += "\r\n\trp"+MI->getIdText().str+": "+MI->getComment2();

            } else if(MI->Instruction.getName() == "M2") {
                //print the comment with the Dmin
                str += "\r\n\trp"+MI->getIdText().str+": "+MI->getComment2();

            } else if(MI->Instruction.getName() == "MM") {
                //print the comment with the Dmin
                str += "\r\n\trp"+MI->getIdText().str+": "+MI->getComment2();

            } else
                throw EImpossibleError("lateral effect");
        }

        //print the end delimiter of the indicated group
        str += "\r\n\t}";
    }
    //print the end delimiter of motion program
    str += "\r\n}";
}
//Get MP-Dends in the interface format of the MCS.
//Inputs:
//  label: string labeling all the MP-Dend.
//  Bid: univoque identifier of the CB.
void TMotionProgram::getDendInterfaceText(string& str, const string& label,
                                           unsigned int Bid) const
{
    //CHECK THE PRECONDITIONS:

    for(int i=0; i<getCount(); i++) {
        const TMessageList *ML = Items[i];
        for(int j=0; j<ML->getCount(); j++) {
            const TMessageInstruction *MI = ML->GetPointer(j);
            if(MI->Instruction.getName()!="M1" &&
                    MI->Instruction.getName()!="M2" &&
                    MI->Instruction.getName()!="MM")
                throw EImproperCall("all instruction should be motion instructions");
        }
    }

    if(label!="pos" && label!="depos")
        throw EImproperArgument("MP label should be \"pos\" or \"depos\"");

    if(int(Bid) < 0)
        throw EImproperArgument("block identifier Bid should be less maximun integer value");

    //PRINT THE MP-Dend:

    //print the label of the motion program and their start delimiter
    str = label+"_"+inttostr(int(Bid))+" {";

    //for each list of message of instructions of the motion program
    //  print a group
    for(int i=0; i<getCount(); i++) {
        //points the indicated message list to facilitate its access
        const TMessageList *ML = Items[i];

        //PRINT THE GROUP CORRESPONDING TO THE INDICATED MESSAGE LIST:

        //print the label of the indicated group and their start delimiter
        str += "\r\n\tgroup_"+IntToStr(i+1).str+" {";

        //print the label of the indicated group and their start delimiter
        //inserting zeros on the left:
        //str += "\r\n\tgroup_"+strInsertChar(IntToStr(i+1), 2).str+" {";

        //for each MI of the list, prints the corresponding instruction in the str, and actualice the APL
        for(int j=0; j<ML->getCount(); j++) {
            //point the indicated MI to facilitate its access
            const TMessageInstruction *MI = ML->GetPointer(j);

            //print the corresponding instruction in the interface format
            if(MI->Instruction.getName() == "M1") {
                //print the comment with the Dend
                str += "\r\n\trp"+MI->getIdText().str+": "+MI->getComment3();

            } else if(MI->Instruction.getName() == "M2") {
                //print the comment with the Dend
                str += "\r\n\trp"+MI->getIdText().str+": "+MI->getComment3();

            } else if(MI->Instruction.getName() == "MM") {
                //print the comment with the Dend
                str += "\r\n\trp"+MI->getIdText().str+": "+MI->getComment3();

            } else
                throw EImpossibleError("lateral effect");
        }

        //print the end delimiter of the indicated group
        str += "\r\n\t}";
    }
    //print the end delimiter of motion program
    str += "\r\n}";
}

//---------------------------------------------------------------------------

} //namespace Positiong

//---------------------------------------------------------------------------
