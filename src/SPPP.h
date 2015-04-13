// Copyright (c) 2014-2015 Isaac Morales Durán. All rights reserved.
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
//File: SPPP.h
//Content: structure containing propeties of SP and PP
//Last update: 23/01/2015
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef SPPP_H
#define SPPP_H

#include "MotionProgramValidator.h"
#include "TargetPointList.h"
#include "FiberMOSModel2.h"
#include "MotionProgram.h"
#include "PointersList.h"

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//class TSPPP:

class TSPPP {
public:
    //SP properties:
    string Name;        //name ("")                 (can be empty)
    double RA;          //rect ascension (0)
    double Dec;         //declination (0)
    double Mag;         //magnitude (0)             (can be empty)
    TPointType Type;    //type (ptUNKNOWN)

    //Allocation properties
    unsigned int Pr;    //allocation priority (0)   (can be empty)
    unsigned int Bid;   //Id of the CB (0)          (can be empty)
    unsigned int Pid;   //Id of the RP (0)

    //PP properties:
    double X;           //abcise (0)
    double Y;           //ordinate (0)
    bool Enabled;       //indicates if the point is allocated to the RP (false)

    //Allocation properties:
    bool notAllocated;  //indicates if the point is not allocated in other CB
                        //(true)    (could be empty)
    bool allocateInAll; //indicates if the point must be allocated in all CBs
                        //(false)   (could be empty)
    string Comment;     //coment ("")               (can be empty)

    //Used to generate a pair (PP,DP):
    //  Type, [Bid], Pid, X, Y, Enabled

    //If field Bid is not empty, all fields (except Name and Comment)
    //shall be filled.
    //If field Bid is not empty, the SPPP correspond to a PP.
    //If field Enabled is true, the PP is allocated to the RP.

    //Used to determine if an allocation is of must type
    //when attempt regenerate a pair (PP,DP):
    //  Type, [Pr], [Bid], Pid, X, Y, Enabled, [notAllocated], [allocateInAll]

    //flags indicating fields which can be empty (except Name and Comment):
    //default value: false
    bool there_is_Mag;
    bool there_is_Pr;
    bool there_is_Bid;
    bool there_is_notAllocated;
    bool there_is_allocateInAll;

    //set the structure in text format
    void setText(const string& str);

    //build a structure by default
    TSPPP(void);

    //copy all properties of an object of the same type
    TSPPP& operator=(const TSPPP&);
};

//---------------------------------------------------------------------------
//class TSPPPList:

class TSPPPList : public TPointersList<TSPPP> {
public:
    //set the SPPPL in text format
    void setTableText(unsigned int& Bid, const string& str);

    //get the TPL
    void getTPL(TTargetPointList& TPL);

    //build a SPPP list by default
    TSPPPList(void);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

#endif // SPPP_H
