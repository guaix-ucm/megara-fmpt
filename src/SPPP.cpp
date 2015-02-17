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
//File: SPPP.cpp
//Content: structure containing propeties of SP and PP
//Last update: 23/01/2015
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "SPPP.h"
#include "Strings.h"
#include "StrPR.h"

using namespace Strings;

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//class TSPPP:

//set the structure in text format
void TSPPP::setText(const string& str)
{
    TStrings Strings;
    StrSplit(Strings, str, '|');

    if(Strings.getCount() != 12)
        throw EImproperArgument("string S should contains 12 fields separated by '|'");

    try {
        //translate the values and assign to a tampon variable
        TSPPP SPPP;
        SPPP.Name = StrTrim(Strings[0]).str;
        SPPP.RA = StrToFloat_(Strings[1]);
        SPPP.Dec = StrToFloat_(Strings[2]);
        SPPP.Mag = StrTrim(Strings[3]).str;
        SPPP.Type = StrTrim(Strings[4]).str;

        SPPP.Pr = StrTrim(Strings[5]).str;
        SPPP.Bid = StrTrim(Strings[6]).str;
        SPPP.Pid = StrTrim(Strings[7]).str;

        SPPP.X = StrToFloat_(Strings[8]);
        SPPP.Y = StrToFloat_(Strings[9]);

        //SPPP.Enabled = StrToBool_(Strings[10]);
        if(StrTrim(Strings[10])=="0")
            SPPP.Enabled = false;
        else
            SPPP.Enabled = true;

        SPPP.Comment = StrTrim(Strings[11]).str;

        //assign the tampon variable
        *this = SPPP;

    } catch(...) {
        throw;
    }
}

//build a structure by default
TSPPP::TSPPP() : RA(0), Dec(0),
      X(0), Y(0), Enabled(false)
{
    Name = "";
    Mag = "";
    Type = "";
    Pr = "";
    Bid = "";
    Pid = "";
    Comment = "";
}

//copy all properties of am object of the same type
TSPPP& TSPPP::operator=(const TSPPP& SPPP)
{
    Name = SPPP.Name;
    RA = SPPP.RA;
    Dec = SPPP.Dec;
    Mag = SPPP.Mag;
    Type = SPPP.Type;
    Pr = SPPP.Pr;
    Bid = SPPP.Bid;
    Pid = SPPP.Pid;
    X = SPPP.X;
    Y = SPPP.Y;
    Enabled = SPPP.Enabled;
    Comment = SPPP.Comment;

    return *this;
}

//---------------------------------------------------------------------------
//class TSPPPList:

char strFirstNonseparatorChar(const string& str)
{
    int i = StrSearchFirstNonseparatorChar(str) - 1;
    if(i <= (int)str.length())
        return str[i];
    return 0;
}

//set the SPPPL in text format
void TSPPPList::setTableText(const string& str)
{
    //divide the string S in lines
    TStrings Strings;
    StrDivideInLines(Strings, str);

    //--------------------------------------------------------------

    //check if there is more lines
    int i = 0;
    if(i >= Strings.getCount())
        throw EImproperArgument("string S should contains a SPPPL");

    //discard the coments and empty lines
    while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i].str).Length()<=0))
        i++;

    //check if there is more lines
    if(i >= Strings.getCount())
        throw EImproperArgument("string S should contains a SPPPL");

    //check if actual line contains the label @@SOB@@
    if(StrTrim(Strings[i]) != AnsiString("@@SOB@@"))
        throw EImproperArgument("string S should contains a SPPPL");

    //discard the label @@SOB@@
    i++;

    //check if there is more lines
    if(i >= Strings.getCount())
        throw EImproperArgument("string S should contains a SPPPL");

    //discard all lines until the close label @@EOB@
    while(StrTrim(Strings[i]) != AnsiString("@@EOB@@"))
        i++;

    //discard the label @@SOB@@
    i++;

    //--------------------------------------------------------------

    //check if there is more lines
    if(i >= Strings.getCount())
        throw EImproperArgument("string S should contains a SPPPL");

    //discard the coments and empty lines
    while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i]).Length()<=0))
        i++;

    //check if there is more lines
    if(i >= Strings.getCount())
        throw EImproperArgument("string S should contains a SPPPL");

    //check if actual line contains the label @@SOS@@
    if(StrTrim(Strings[i]) != AnsiString("@@SOS@@"))
        throw EImproperArgument("string S should contains a SPPPL");

    //discard the label @@SOS@@
    i++;

    //check if there is more lines
    if(i >= Strings.getCount())
        throw EImproperArgument("string S should contains a SPPPL");

    //read all lines (using a tampon variable) until the close label @@EOS@
    try {
        TSPPPList SPPPL;

        while(i<Strings.getCount() && StrTrim(Strings[i])!=AnsiString("@@EOS@@")) {
            TSPPP *SPPP = new TSPPP();
            SPPPL.Add(SPPP);
            SPPP->setText(Strings[i].str);
            i++;
        }

        *this = SPPPL;

    } catch(...) {
        throw;
    }

    //check if there is more lines
    if(i >= Strings.getCount())
        throw EImproperArgument("string S should contains a SPPPL");

    //check if actual line contains the label @@EOS@@
    if(StrTrim(Strings[i]) != AnsiString("@@EOS@@"))
        throw EImproperArgument("missing closing label @@EOS@@ in string S");

}

//get the TPL
void TSPPPList::getTPL(TTargetPointList& TPL)
{
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
}

//build a SPPPList by default
TSPPPList::TSPPPList(void) : TPointersList<TSPPP>()
{
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

