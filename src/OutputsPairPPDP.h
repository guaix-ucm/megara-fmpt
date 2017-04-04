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
//File: OutputsPairPPDP.h
//Content: structure for contains all outputs data about a generated pair (PP, DP)
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef OUTPUTSPAIRPPDP_H
#define OUTPUTSPAIRPPDP_H

#include "MotionProgram.h"
#include "RoboticPositionerList3.h"
#include "FMOSA.h"

//---------------------------------------------------------------------------

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//OutputsPairPPDP:
//---------------------------------------------------------------------------

class OutputsPairPPDP
{
public:
    //properties for built comments about file outputs
    string FMOSA_filename; //name of file type FMOSA of procedence
    string FMPT_version; //version of the FMPT with which was generated
    string datetime; //date-time of generation
    TRoboticPositionerList EnabledNotOperative; //list of pointer to enabled not operative RPs
    string collided_str; //list of lists of colliding items (both EAs and RPs)
    TRoboticPositionerList Collided; //list of pointers to colliding RPs
    TRoboticPositionerList Obstructed; //list of pointer to obstructed RPs

    //properties for built comments about each MP
    bool PPvalid; //validity of the PP
    bool DPvalid; //validity of the DP
    double DsecMax; //maximun value of Dsec (security distance during radial movements)

    //properties for built the executable data
    unsigned int Bid; //block identifier
    TPairPositionAnglesList IPL; //initial position list
    TMotionProgram PP; //positioning program
    TPairPositionAnglesList OPL; //observing position list
    TMotionProgram DP; //depositioning program
    TFMOSA FMOSA; //FMOSA of file type FMOSA

    //get the comments about outputs
    string getComments(void) const;

    //get outputs in text format with:
    //  comments
    //  the pair (PP, DP)
    //  the FMOSA
    void getText(string& str, bool includeFMOSA=true) const;

    //get other outputs in text format with:
    //  comments
    //  other properties in format assigns
    void getOtherText(string& str) const;

    //set the outputs in text format
    void setText(const string& str, bool includeFMOSA=true);

    //build an estructure of class OutputsPairPPDP
    OutputsPairPPDP();

    //reset all properties to values by default
    void Clear(void);
};

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
#endif // OUTPUTSPAIRPPDP_H
