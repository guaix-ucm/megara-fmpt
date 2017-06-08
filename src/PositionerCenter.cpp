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
///@file PositionersCenters.cpp
///@brief positioner center list
///@author Isaac Morales Durán
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
        PC.Id = StrToInt(Strings[0]);
        PC.x0 = StrToFloat(Strings[1]);
        PC.y0 = StrToFloat(Strings[2]);

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
        throw EImproperArgument("pointer PC should point to built positioner center");

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

    //read all rows (using a tampon variable) until exhaustin the list
    try {
        TPositionerCenterList PCL;

        int i = 0;
        while(i < Strings.getCount()) {
            //discard the coments and empty lines
            while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i].str).Length()<=0))
                i++;

            //if there is a row
            if(i < Strings.getCount()) {
                //add a structure in the tampon variable
                TPositionerCenter *PC = new TPositionerCenter();
                PCL.Add(PC);
                //try set the row in the structure
                PC->setText(Strings[i++].str);
            }
        }

        *this = PCL;

    } catch(...) {
        throw;
    }
}

//build a PositionerCenterList by default
TPositionerCenterList::TPositionerCenterList(void) : TPointersList<TPositionerCenter>()
{
    Print = TPositionerCenter::PrintRow;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

