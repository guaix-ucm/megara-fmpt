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
/// @file OutputsPairPPDP.h
/// @brief structure for contains all outputs data about a generated pair (PP, DP)
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef OUTPUTSPAIRPPDP_H
#define OUTPUTSPAIRPPDP_H

#include "MotionProgram.h"
#include "RoboticPositionerList3.h"
#include "FMOSA.h"

//#include <jsoncpp/json/json.h>
//#include <json/json_object.h>
//#include <json/json_tokener.h>

//For install in Ubuntu/Debian:
//  sudo apt-get install uuid-dev
//  sudo apt-get install libjson0-dev

//---------------------------------------------------------------------------

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//OutputsPairPPDP:
//---------------------------------------------------------------------------

/// @brief Encapsulates the properties related to a pair (PP, DP).
/// @brief This calss contains all properties necessaries for:
/// - execute the pair (PP, DP),
/// - determine if the pair (PP, DP) is suitable to be executed,
/// - and generate the file outputs.
class OutputsPairPPDP
{
private:
    //functions for add data to a JSON object
    void addUUID(Json::Value &object) const;
    Json::Value getComments(void) const;
    Json::Value getPos(void) const;
    Json::Value getDepos(void) const;

public:
    //properties for built comments about file outputs
    string FMOSA_filename; ///<name of file type FMOSA of procedence
    string FMPT_version; ///<version of the FMPT with which was generated
    string FMAT_version; ///<version of the FMAT with which was generated
    string datetime; ///<date-time of generation
    TRoboticPositionerList EnabledNotOperative; ///<list of pointer to enabled not operative RPs
    string collided_str; ///<list of lists of colliding items (both EAs and RPs)
    TRoboticPositionerList Collided; ///<list of pointers to colliding RPs
    TRoboticPositionerList Obstructed; ///<list of pointer to obstructed RPs

    //properties for built comments about each MP
    bool PPvalid; ///<validity of the PP
    bool DPvalid; ///<validity of the DP
    double DsecMax; ///<maximun value of Dsec (security distance during radial movements)

    //properties for built the executable data
    unsigned int Bid; ///<block identifier
    TPairPositionAnglesList IPL; ///<initial position list
    TMotionProgram PP; ///<positioning program
    TPairPositionAnglesList OPL; ///<observing position list
    TMotionProgram DP; ///<depositioning program
    TFMOSA FMOSA; ///<FMOSA of file type FMOSA

    /// Get the warning for not suitable outputs.
    string getWarningNotSuitable(void) const;

    /// Get the comments about outputs.
    string getCommentsText(void) const;

    /// @brief Get outputs in format MCS with:
    /// - comments.
    /// - the pair (PP, DP).
    /// - the FMOSA.
    void getText(string& str, bool includeFMOSA=true) const;

    /// @brief Get outputs in format JSON with:
    /// - comments.
    /// - the pair (PP, DP).
    /// - the FMOSA.
    string getJSONtext(bool includeFMOSA=true) const;

    /// @brief Get other outputs in text format with:
    /// - comments.
    /// - other properties in format assigns.
    void getOtherText(string& str) const;

    /// Set the outputs in text format.
    void setText(const string& str, bool includeFMOSA=true);

    /// Build an estructure of class OutputsPairPPDP.
    OutputsPairPPDP();

    /// Reset all properties to values by default.
    void Clear(void);

    /// @brief Determine if the outputs is suitable to be executed:
    /// - the pair (PP, DP) is valid,
    /// - and there aren't dangerous RPs,
    /// - and there aren't collided RPs,
    /// - and there aren't obstructed RPs.
    /// @brief
    /// Dangerous RP: enabled-not-operative RP with fault type dynamic or unknowledge.
    bool suitable(void) const;
};

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
#endif // OUTPUTSPAIRPPDP_H
