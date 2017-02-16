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
//File: Outputs.h
//Content: structure of data for contains all outputs data
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef OUTPUTS_H
#define OUTPUTS_H

#include "MotionProgram.h"
#include "RoboticPositionerList3.h"
#include "FMOSATable.h"

//---------------------------------------------------------------------------

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//Outputs:
//---------------------------------------------------------------------------

class Outputs
{
public:
    TMotionProgram PP;
    TMotionProgram DP;
    TRoboticPositionerList Collided;
    TRoboticPositionerList Obstructed;
    TFMOSATable FMOSAT;

    //get the pair (PP, DP) in text format
    //with comments and with FMOSAT
    void getOutputsText(string& str, const int Bid,
                 const TPairPositionAnglesList& OPL,
                 const TPairPositionAnglesList& IPL, bool r2_negative) const;

    //get the pair (PP, DP) in text format
    //without comments and without the FMOSAT
    void getFiberMOSText(string& str, const int Bid,
                 const TPairPositionAnglesList& OPL,
                 const TPairPositionAnglesList& IPL, bool r2_negative) const;

    //set the outputs in text format
    void setText(unsigned int& Bid, const string& str);

    //build an estructure of class Outputs
    Outputs();
};

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
#endif // OUTPUTS_H
