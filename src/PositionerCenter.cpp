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
//File: PositionersCenters.cpp
//Content: structure containing triplets (Id, x0, y0)
//Last update: 06/02/2015
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "PositionerCenter.h"
#include "Strings.h"
#include "StrPR.h"

using namespace Strings;

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//class TPositionerCenter:

//set the structure in text format
void TPositionerCenter::setText(const string& str)
{
    TStrings Strings;
    StrSplit(Strings, str, '|');

    if(Strings.getCount() != 3)
        throw EImproperArgument("string S should contains 3 fields separated by '|'");

    try {
        //translate the values and assign to a tampon variable
        TPositionerCenter PC;
        PC.Id = StrToInt_(Strings[0]);
        PC.x0 = StrToFloat_(Strings[1]);
        PC.y0 = StrToFloat_(Strings[2]);

        //assign the tampon variable
        *this = PC;

    } catch(...) {
        throw;
    }
}

//print the properties in a row text
void TPositionerCenter::PrintRow(AnsiString& S, const TPositionerCenter *PC)
{
    if(PC == NULL)
        throw EImproperArgument("pinter PC should point to built positioner center");

    S += IntToStr(PC->Id)+AnsiString("\t")+FloatToStr(PC->x0)+AnsiString("\t")+FloatToStr(PC->y0);
}

//build a structure by default
TPositionerCenter::TPositionerCenter() : Id(0), x0(0), y0(0)
{
}

//copy all properties of am object of the same type
TPositionerCenter& TPositionerCenter::operator=(const TPositionerCenter& PC)
{
    Id = PC.Id;
    x0 = PC.x0;
    y0 = PC.y0;

    return *this;
}

//---------------------------------------------------------------------------
//class TPositionerCenterList:

//set the PositionerCenterL in text format
void TPositionerCenterList::setTableText(const string& str)
{
    //divide the string S in lines
    TStrings Strings;
    StrDivideInLines(Strings, str);

    //read all lines (using a tampon variable) until the close label @@EOS@
    try {
        TPositionerCenterList PCL;

        for(int i=0; i<Strings.getCount(); i++) {
            TPositionerCenter *PC = new TPositionerCenter();
            PCL.Add(PC);
            PC->setText(Strings[i].str);
        }

        *this = PCL;

    } catch(...) {
        throw;
    }
}
/*
//get the TPL
void TSPPPList::getTPL(TTargetPointList& TPL)
{
    //initialize the TPL
    TPL.Cleat();

    //add the TPLs corresponding to the SPPP which accomplish the two following conditions:
    //  SPPP->Bid.length() > 0
    //  SPPP->Enabled
    for(int i=0; i<getCount(); i++) {
        TSPPP *SPPP = Items[i];
        if(SPPP->Bid.length() > 0) { //if the SPPP correspond to an PP
            if(SPPP->Enabled) { //if the point is allocated
                //extract the Id from the SPPP
                int Id = StrToInt_(SPPP->Pid);
                //search the RP in the RPL attached to the TPL
                int j = TPL.getRoboticPositionerList()->SearchId(Id);
                //if has found the RP, build and attach a TP
                if(j < TPL.getRoboticPositionerList()->getCount()) {
                    //copy the properties in a TP nd add the TP to the MPG
                    TTargetPoint *TP = new TTargetPoint(TPL.getRoboticPositionerList()->Get(j), SPPP->X, SPPP->Y);
                    //add the TP to the TPL
                    TPL.Add(TP);
                }
            }
        }
    }
}*/

//build a PositionerCenterList by default
TPositionerCenterList::TPositionerCenterList(void) : TPointersList<TPositionerCenter>()
{
    Print = TPositionerCenter::PrintRow;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

