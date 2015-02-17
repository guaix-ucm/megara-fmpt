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
    string Name;    //name of the SP
    double RA;
    double Dec;
    string Mag;
    string Type;

    //allocation properties
    string Pr;      //if Name is empty this is empty
    string Bid;     //if Name is empty this is empty
    string Pid;     //Id of the RP

    //PP properties:
    double X;
    double Y;
    bool Enabled;       //indicates if the point is allocated

    //coment about the point
    string Comment; //if Name is empty this is empty

    //set the structure in text format
    void setText(const string& S);

    //build a structure by default
    TSPPP(void);

    //copy all properties of am object of the same type
    TSPPP& operator=(const TSPPP&);
};

//---------------------------------------------------------------------------
//class TSPPPList:

class TSPPPList : public TPointersList<TSPPP> {
public:
    //set the SPPPL in text format
    void setTableText(const string& S);

    //get the TPL
    void getTPL(TTargetPointList& TPL);

    //build a SPPPList by default
    TSPPPList(void);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

#endif // SPPP_H
