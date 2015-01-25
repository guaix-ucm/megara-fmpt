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
//File: TranslationFuunctions.h
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
    AnsiString Name;    //is empty not correespond to an allocated PP
    double RA;
    double Dec;
    AnsiString Mag;
    AnsiString Type;
    //other properties which value is integer:
    AnsiString Pr;      //if Nae is empty this is empty
    AnsiString Bid;     //if Nae is empty this is empty
    AnsiString Pid;
    //corresponding PP properties:
    double X;
    double Y;
    bool Enabled;   //indicates if the point is allocated
    //coment about the point
    AnsiString Comment; //if Nae is empty this is empty

    //set the structure in text format
    void setText(const AnsiString& S);

    //build an structure by default
    TSPPP(void) : Name(""), RA(0), Dec(0), Mag(0), Type(""),
        Pr(""), Bid(""), Pid(""), X(0), Y(0), Enabled(false), Comment("") {;}

    //copy all properties of a SPPP
    TSPPP& operator=(const TSPPP&);
};

//---------------------------------------------------------------------------
//class TSPPPList:

class TSPPPList : public TPointersList<TSPPP> {
public:
    //set the SPPPL in text format
    void setTableText(const AnsiString& S);

    //get the TPL
    void getTPL(TTargetPointList& TPL);

    //build a SPPPList by default
    TSPPPList(void);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

#endif // SPPP_H
