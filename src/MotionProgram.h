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
//File: PositioningProgram.h
//Content: class of positioning program
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef MOTIONPROGRAM_H
#define MOTIONPROGRAM_H

#include "MessageInstruction.h"
#include "PointersList.h"
#include "PairPositionAngles.h"

//---------------------------------------------------------------------------

using namespace Lists;
using namespace Models;

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//TMessageList:
//---------------------------------------------------------------------------

//lista de mensajes de instrucción
class TMessageList : public TPointersList<TMessageInstruction> {
    //AnsiString p_Label; void SetLabel(const AnsiString&);

public:
    //MÉTODOS ESTÁTICOS:

    //imprime una lista en una cadena de texto
    static void  PrintMessageList(AnsiString &S,
                                  const TMessageList *L);
    //lee una lista de emnsajes en una cadena de texto
    static void  ReadMessageList(TMessageList *L,
                                 const AnsiString &S, int &i);

    //construye una lista de mensajes
    TMessageList(int Capacity=1003) :
        TPointersList<TMessageInstruction>(Capacity,
                                           TMessageInstruction::CompareIds,
                                           NULL, NULL,
                                           TMessageInstruction::Print,
                                           TMessageInstruction::Read) {;}

    //PUBLIC METHODS:

    //construye un clon de una lista de MIs basada en punteros
    TMessageList(const TMessageList*);

    //determines if a ML is different to this ML
    bool operator!=(const TMessageList&) const;
};

//---------------------------------------------------------------------------
//TMotionProgram:
//---------------------------------------------------------------------------

//class motion program
class TMotionProgram : public TPointersList<TMessageList> {
public:
    //determines if there is some coment 1 in any instruction
    //of the the motion program
    bool thereIsSomeComment1(void) const;
    //get the non empty coments of the motion program
    //in column text format
    string getComment1sColumnText(void) const;

    //builds a motion program
    TMotionProgram(int Capacity=7) :
        TPointersList<TMessageList>(Capacity,
                                    NULL, NULL, NULL,
                                    TMessageList::PrintMessageList,
                                    TMessageList::ReadMessageList) {;}

    //determines if a MP is different to this MP
    bool operator!=(const TMotionProgram&) const;

    //get the list of RP identifiers includes in a MP
    void getAllIncludedIds(TVector<int>& Ids) const;

    //exclude the MIs addressed to a determined RP
    void excludeRP(int Id);

    //Get a motion progam in the interface format of the MCS.
    //Inputs:
    //  label: string labeling all the MP.
    //  Bid: univoque identifier of the CB.
    //  SPL: starting position list for all RPs of the Fiber MOS.
    //Preconditions:
    //  All PPAs of the SPL must be addresed to different RPs.
    //  All RPs included in the MP, must be in included in the SPL.
    void getInterfaceText(string& str, const string& label, unsigned int Bid,
                          const TPairPositionAnglesList& SPL) const;

    //Values for label:
    //  "obs depos" for depositioning programs
    //  "obs pos" for positioning programs

    //SPL shall be:
    //  IPL if the MP is a PP
    //  OPL if the MP is a DP

    //A MP, must...
    //- can exist without be executable by the FMM;
    //- can be manipulated without the FMM;
    //- not necessary coincide with the propietary format of the RPs.

    //Set a motion progam in the interface format of the MCS.
    void setInterfaceText(string& label, unsigned int& Bid,
                          const string& str);

    //Get MP-Dfmins in the interface format of the MCS.
    //Inputs:
    //  label: string labeling all the MP-Dfmin.
    //  Bid: univoque identifier of the CB.
    void getDfminInterfaceText(string& str, const string& label,
                               unsigned int Bid) const;
};

//---------------------------------------------------------------------------

} //namespace Positiong

//---------------------------------------------------------------------------
#endif // MOTIONPROGRAM_H
