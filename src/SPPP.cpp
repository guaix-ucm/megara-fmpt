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
//File: TranslationFuunctions.cpp
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
void TSPPP::setText(const AnsiString& S)
{
    TStrings Strings;
    StrSplit(Strings, S, '|');

    if(Strings.getCount() != 12)
        throw EImproperArgument("string S should contains 12 dields separated by '|'");

    try {
        Name = StrTrim(Strings[0]);
        RA = StrToFloat_(Strings[1]);
        Dec = StrToFloat_(Strings[2]);
        Mag = StrTrim(Strings[3]);
        Type = StrTrim(Strings[4]);

        Pr = StrTrim(Strings[5]);
        Bid = StrTrim(Strings[6]);
        Pid = StrTrim(Strings[7]);

        X = StrToFloat_(Strings[8]);
        Y = StrToFloat_(Strings[9]);
        //Enabled = StrToBool_(Strings[10]);
        if(StrTrim(Strings[10])==AnsiString("0"))
            Enabled = false;
        else
            Enabled = true;

        Comment = StrTrim(Strings[11]);

    } catch(...) {
        throw;
    }
}


//copy all properties of a SPPP
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

char StrFirstNonseparatorChar(const AnsiString& S)
{
    int i = StrSearchFirstNonseparatorChar(S);
    if(i <= S.Length())
        return S[i];
    return 0;
}

//set the SPPPL in text format
void TSPPPList::setTableText(const AnsiString& S)
{
    //divide the string S in lines
    TStrings Strings;
    StrDivideInLines(Strings, S);

    //--------------------------------------------------------------

    //check if there is more lines
    int i = 0;
    if(i >= Strings.getCount())
        throw EImproperArgument("string S should contains a SPPPL");

    //discard the coments and empty lines
    while(i<Strings.getCount() && (StrFirstNonseparatorChar(Strings[i])=='#' || StrTrim(Strings[i]).Length()<=0))
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
    while(i<Strings.getCount() && (StrFirstNonseparatorChar(Strings[i])=='#' || StrTrim(Strings[i]).Length()<=0))
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
            SPPP->setText(Strings[i]);
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
        if(SPPP->Name.Length() > 0) { //if the SPPP correspond to an PP
            if(SPPP->Enabled) { //if the point is allocated
                //extract the Id from the name of the SPPP
                TStrings Strings;
                StrSplit(Strings, SPPP->Name, ':');
                if(Strings.getCount() <= 0)
                    throw EImproperArgument("");
                int Id = StrToInt_(Strings[Strings.getCount() - 1]);
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

