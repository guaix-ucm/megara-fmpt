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
/// @file PositioningProgram.h
/// @brief positioning program
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef MOTIONPROGRAM_H
#define MOTIONPROGRAM_H

#include "MessageInstruction.h"
#include "PointersList.h"
#include "PairPositionAngles.h"

#include <jsoncpp/json/json.h>

//---------------------------------------------------------------------------

using namespace Lists;
using namespace Models;

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//TMessageList:
//---------------------------------------------------------------------------

/// @brief A list of instruction messages contains the set of instructions
/// for execute a gesture of a motion program.
class TMessageList : public TPointersList<TMessageInstruction> {
    //AnsiString p_Label; void SetLabel(const AnsiString&);

public:
    //MÉTODOS ESTÁTICOS:

    /// Print a message list in a text string.
    static void  PrintMessageList(AnsiString &S,
                                  const TMessageList *L);
    /// Read a messaje list from a text string.
    static void  ReadMessageList(TMessageList *L,
                                 const AnsiString &S, int &i);

    /// Build a message list.
    TMessageList(int Capacity=100) :
        TPointersList<TMessageInstruction>(Capacity,
                                           TMessageInstruction::CompareIds,
                                           NULL, NULL,
                                           TMessageInstruction::Print,
                                           TMessageInstruction::Read) {;}

    //PUBLIC METHODS:

    /// Build a clone of a message list.
    TMessageList(const TMessageList*);

    /// Determines if a ML is different to this ML.
    bool operator!=(const TMessageList&) const;
};

//---------------------------------------------------------------------------
//TMotionProgram:
//---------------------------------------------------------------------------

/// @brief A MP (Motion Program) is a list of message lists.
/// @brief A MP can be printed in two formats:
/// - format MCS.
/// - format JSON.
/// @brief For execute the MP the MCS need translate from the FMPT format
/// to the propietary format of the controllers of the RPs.
/// @brief The Message instructions contained in a MP have labels Dmin and
/// Dends, for anotate the parameters calculated during validation of the MP.
class TMotionProgram : public TPointersList<TMessageList> {
public:
     /// @brief Determines if there is some coment Dsec
     /// in any instruction of the the motion program.
    bool thereIsSomeCommentDsec(void) const;
    /// Get the non empty coments of the motion program.
    string getCommentsDsecMCStext(void) const;

    /// Builds a motion program.
    TMotionProgram(int Capacity=7) :
        TPointersList<TMessageList>(Capacity,
                                    NULL, NULL, NULL,
                                    TMessageList::PrintMessageList,
                                    TMessageList::ReadMessageList) {;}

    /// Determines if a MP is different to this MP.
    bool operator!=(const TMotionProgram&) const;

    /// Get the list of identifiers of RPs included in a MP.
    void getAllIncludedIds(TVector<int>& Ids) const;

    /// Exclude the MIs addressed to a determined RP.
    void excludeRP(int Id);

    /// @brief Get a motion program in the format MCS.
    /// @param[in] label type of MP ["pos", "depos"].
    /// @param[in] Bid identifier of the CB.
    /// @param[in] SPL starting position list for all RPs of the Fiber MOS.
    /// @pre All PPAs of the SPL must be referred to different RPs.
    /// @pre All RPs included in the MP, must be in included in the SPL.
    void getMCStext(string& str, const string& label, unsigned int Bid,
                          const TPairPositionAnglesList& SPL) const;

    /// get the motion program in format JSON.
    Json::Value getJSON(const TPairPositionAnglesList& SPL) const;

    //Values for label:
    //- "depos" for depositioning programs
    //- "pos" for positioning programs
    //SPL shall be:
    //- IPL if the MP is a PP
    //- OPL if the MP is a DP

    //A MP, must...
    //- can exist without be executable by the FMM;
    //- can be manipulated without the FMM;
    //- not necessary coincide with the propietary format of the RPs.

    /// Set a motion progam in format MCS.
    void setMCStext(string& label, unsigned int& Bid,
                          const string& str);

    /// @brief Get MP-Dmins in the format MCS.
    /// @param[in] label type of MP ["pos", "depos"].
    /// @param[in] Bid identifier of the CB.
    void getDminMCStext(string& str, const string& label,
                               unsigned int Bid) const;

    /// @brief Get MP-Dends in the format MCS.
    /// @param[in] label type of MP ["pos", "depos"].
    /// @param[in] Bid identifier of the CB.
    void getDendMCStext(string& str, const string& label,
                               unsigned int Bid) const;
};

//---------------------------------------------------------------------------

} //namespace Positiong

//---------------------------------------------------------------------------
#endif // MOTIONPROGRAM_H
