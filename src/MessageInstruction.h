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
/// @file MessageInstruction.h
/// @brief message of instruction
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef MESSAGEINSTRUCTION_H
#define MESSAGEINSTRUCTION_H

//---------------------------------------------------------------------------

#include "Vector.h"
#include "Instruction.h"

using namespace Lists;

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//--------------------------------------------------------------------------
//TMessageInstruction:
//--------------------------------------------------------------------------

//class MI (message instruction)
class TMessageInstruction {
        int p_Id;
        string p_CommentDsec;
        string p_CommentDmin;
        string p_CommentDend;

public:
        //PUBLIC PROPERTIES:

        //identification number of the CAN port destinatary of the message
        //default value: 0
        int getId(void) const {return p_Id;} void setId(int);

        //instruction of the message
        //default value: the instruction by default
        TInstruction Instruction;

        //Instructions must be written directly in the property Instruction.
        //First must be assigned the instruction name,
        //and then can be assigned the arguments. E.g:
        //      Instruction.Name = "G1";
        //      Instruction.Args.First = p1;

        //comment 1
        //for anotate Dsec value when Dsec < DsecMax
        //during generation of the MP
        //default value: ""
        string getCommentDsec(void) const{return p_CommentDsec;}
        void setCommentDsec(const string&);

        //comment 2
        //for anotate Dmin value during validation of the MP
        //default value: ""
        string getCommentDmin(void) const{return p_CommentDmin;}
        void setCommentDmin(const string&);

        //comment 3
        //for anotate Dend value during validation of the MP
        //default value: ""
        string getCommentDend(void) const{return p_CommentDend;}
        void setCommentDend(const string&);

        //PUBLIC PROPERTIES IN TEXT FORMAT:

        //identification number of the CAN port destinatary of the message
        //in text format
        //default value: "0"
        AnsiString getIdText(void) const; void setIdText(const AnsiString&);

        //instruction of the message
        //in text format:
        //  Instruction.Text

        //the MI in text format:
        //  IdText+": "+Instruction.Text
        //default value: "0: "
        AnsiString getText(void) const; void setText(const AnsiString&);
        //the MI in text format for comment:
        //  "rp"+IdText+", "+Instruction.TextForComment
        //default value: "0: "
        AnsiString getTextForComment(void) const;

        //STATIC METHODS:

        //compare the identifiers of two MIs
        static int  CompareIds(const TMessageInstruction *MI1,
                const TMessageInstruction *MI2);
        //print the MI in a text string
        static void  Print(AnsiString &S,
                const TMessageInstruction *MI);
        //read the MI from a text string
        static void  Read(TMessageInstruction *MI,
                const AnsiString &S, int &i);

        //read an instruction from a text string
        //in the MCS format
        static void  readMCS(TMessageInstruction *MI,
                                   const string& str, unsigned int &i);

        //PUBLIC METHODS:

        //build a MI by default
        TMessageInstruction(void);
        //build a MI with the indicated values
        TMessageInstruction(int Id, AnsiString InstructionText);
        //clone a MI
        TMessageInstruction(TMessageInstruction *);

        //copy the properties of a MI
        void Copy(const TMessageInstruction *MI);
        //assign the properties of a MI
        TMessageInstruction& operator=(const TMessageInstruction &MI);

        //determine if a MI is different to this MI
        bool operator!=(const TMessageInstruction&) const;
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // MESSAGEINSTRUCTION_H
