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
//File: OutputsParkProg.h
//Content: structure for contains all outputs data about a generated parking program (PP, DP)
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef OUTPUTSPARKPROG_H
#define OUTPUTSPARKPROG_H

#include "MotionProgram.h"
#include "RoboticPositionerList3.h"
#include "FMOSA.h"

//#include <jsoncpp/json/json.h>

//---------------------------------------------------------------------------

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//OutputsParkProg:
//---------------------------------------------------------------------------

class OutputsParkProg
{
private:
    //functions for add data to a JSON object
    void addUUID(Json::Value &object) const;
    Json::Value getComments(void) const;
    Json::Value getParking(void) const;

public:
    //properties for built comments about file ParkProg
    string FMOSA_filename; //name of file type FMOSA of procedence
    string FMPT_version; //version of the FMPT with which was generated
    string datetime; //date-time of generation
    TRoboticPositionerList EnabledNotOperative; //list of pointer to enabled-not-operative RPs
    string collided_str; //list of lists of items (both EAs and RPs) in colliding status
    TRoboticPositionerList Collided; //list of pointers to RPs in colliding status
    TRoboticPositionerList Obstructed; //list of pointer to RPs in obstructed status

    //properties for built comments about each MP
    unsigned int Bid; ///<block identifier
    TPairPositionAnglesList SPL; ///<starting position list
    bool ParkProgValid; ///<validity of the ParkProg
    TPairPositionAnglesList FPL; ///<final position list
    double DsecMax; ///<maximun value of Dsec (security distance during radial movements)

    //properties for built the executable data
    TMotionProgram ParkProg; ///<the parking program

    //get the warning for not suitable outputs
    string getWarningNotSuitable(void) const;

    //get the comments about ParkProg in text format
    string getCommentsText(void) const;
    //get outputs in format MCS with:
    //  comments
    //  the parking program
    void getText(string& str) const;

    //get outputs in format JSON with:
    //  comments
    //  the parking program
    string getJSONtext(void) const;

    //get other outputs in text format with:
    //  comments
    //  other properties in format assigns
    void getOtherText(string& str) const;

    //set the outputs in text format
    void setText(const string& str);

    //!build an structure of type ParkProg
    OutputsParkProg();

    //! reset all properties to values by default
    void Clear(void);

    //DEFINITION:
    //  Dangerous RP: enabled-not-operative RP with fault type dynamic or unknowledge.

    //determine if the outputs is suitable to be executed:
    //  the motion program is valid
    //  and there aren't dangerous RPs
    bool suitable(void) const;
};

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
#endif // OUTPUTSPARKPROG_H
