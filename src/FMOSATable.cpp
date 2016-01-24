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
//File: FMOSATable.cpp
//Content: class FMOSA table
//Last update: 23/01/2015
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "FMOSATable.h"
#include "Strings.h"
#include "StrPR.h"

using namespace Strings;

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//class TSPPP:

//SETS OF PROPERTIES IN TEXT FORMAT:

//get the structure in text format
AnsiString TSPPP::getRowText(void) const
{
    string str = Name+"\t|"+floattostr(RA)+"|"+floattostr(Dec)+"|"+floattostr(Mag)+"|"+pointTypeToStr(Type)+
            "\t|"+inttostr(Pr)+"|"+inttostr(Bid)+"|"+inttostr(Pid)+
            "|"+floattostr(X)+"|"+floattostr(Y)+"|"+inttostr(int(Enabled))+
            "|"+Comment;

    AnsiString S(str);
    return S;
}

//set the structure in text format
void TSPPP::setText(const string& str)
{
    TStrings Strings;
    StrSplit(Strings, str, '|');

    if(Strings.getCount() != 12)
        throw EImproperArgument("should have 12 fields separated by '|'");

    try {
        //translate the values and assign to a tampon variable
        TSPPP SPPP;

        //trims Name property
        SPPP.Name = StrTrim(Strings[0]).str;

        //translate the (RA, Dec) properties
        SPPP.RA = strtofloat(Strings[1].str);
        SPPP.Dec = strtofloat(Strings[2].str);

        //translate Mag property (if it is not empty)
        string aux = StrTrim(Strings[3]).str;
        if(aux.length() > 0) {
            SPPP.Mag = strtofloat(aux);
            SPPP.there_is_Mag = true;
        } else
            SPPP.there_is_Mag = false;

        //translate Type property
        SPPP.Type = strToPointType(Strings[4].str);

        //translate Pr property (if it is not empty)
        aux = StrTrim(Strings[5]).str;
        if(aux.length() > 0) {
            SPPP.Pr = strtoint(aux);
            SPPP.there_is_Pr = true;
        } else
            SPPP.there_is_Pr = false;

        //translate Bid property (if it is not empty)
        aux = StrTrim(Strings[6]).str;
        if(aux.length() > 0) {
            SPPP.Bid = strtoint(aux);
            SPPP.there_is_Bid = true;
        } else
            SPPP.there_is_Bid = false;

        //translate projection point properties
        SPPP.Pid = strtoint(Strings[7].str);
        SPPP.X = strtofloat(Strings[8].str);
        SPPP.Y = strtofloat(Strings[9].str);
        SPPP.Enabled = strtobool(Strings[10].str);

/*        //translate notAllocated property (if it is not empty)
        aux = StrTrim(Strings[11]).str;
        if(aux.length() > 0) {
            SPPP.notAllocated = strtobool(aux);
            SPPP.there_is_notAllocated = true;
        } else
            SPPP.there_is_notAllocated = false;

        //translate allocateInAll property (if it is not empty)
        aux = StrTrim(Strings[12]).str;
        if(aux.length() > 0) {
            SPPP.allocateInAll = strtobool(aux);
            SPPP.there_is_allocateInAll = true;
        } else
            SPPP.there_is_allocateInAll = false;

        SPPP.there_is_notAllocated = true;
        SPPP.there_is_allocateInAll = true;
*/
        //trims Comment property
        SPPP.Comment = StrTrim(Strings[11]).str;

        //check the precondition
        if(there_is_Bid && (!there_is_Mag || !there_is_Pr/* || !there_is_notAllocated || !there_is_allocateInAll*/))
            throw EImproperArgument("if field Bid is not empty, all fields (except Name) shall be filled");

        //assign the tampon variable
        *this = SPPP;

    } catch(...) {
        throw;
    }
}

//print the properties of an SPPP in a string
//in row format
void  TSPPP::PrintRow(AnsiString &S, const TSPPP *SPPP)
{
    //check the precondition
    if(SPPP == NULL)
        throw EImproperArgument("pointer SPPP should point to built allocation");

    //add the text to the string in propeerly format
    S += SPPP->getRowText();
}

//PUBLIC METHODS:

//build a structure by default
TSPPP::TSPPP() : RA(0), Dec(0), Type(ptUNKNOWN), Pr(0), Bid(0), Pid(0),
      X(0), Y(0), Enabled(false), notAllocated(true), allocateInAll(false),
      there_is_Mag(false), there_is_Pr(false), there_is_Bid(false)/*,
      there_is_notAllocated(false), there_is_allocateInAll(false)*/
{
    Name = "";
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
    notAllocated = SPPP.notAllocated;
    allocateInAll = SPPP.allocateInAll;
    Comment = SPPP.Comment;

    there_is_Mag = SPPP.there_is_Mag;
    there_is_Pr = SPPP.there_is_Pr;
    there_is_Bid = SPPP.there_is_Bid;
/*    there_is_notAllocated = SPPP.there_is_notAllocated;
    there_is_allocateInAll = SPPP.there_is_allocateInAll;*/

    return *this;
}

//---------------------------------------------------------------------------
//class TFMOSATable:

//set a FMOSA table in text format
void TFMOSATable::setTableText(unsigned int& Bid, const string& str)
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
        TFMOSATable SPPPL;

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

    //check the preconditions
    for(int i=1; i<getCount(); i++) {
        TSPPP *SPPP = Items[i];

        if(SPPP->Bid != Items[0]->Bid)
            throw EImproperArgument("all Bid should be equal each other");
    }

    //return the Bid value
    Bid = Items[0]->Bid;
}

//get the FMOSA table in text format
void TFMOSATable::getTableText(string& str) const
{
    str = "# Id| Ra| Dec| Pos";
    str += "\r\n@@SOB@@";
    str += "\r\n0| 0| 0| 0";
    str += "\r\n@@EOB@@";

    str += "\r\n#      Name             RA         Dec    Mag        Type         Pr  Bid Pid   X(mm)     Y(mm)  Enabled      Comment";
    str += "\r\n@@SOS@@";
    str += "\r\n"+getColumnText().str;
    str += "\r\n@@EOS@@";
}

//get the allocations which accomplish: there_is_Bid && Enabled
void TFMOSATable::getAllocations(TAllocationList& AL)
{
    for(int i=0; i<getCount(); i++) {
        TSPPP *SPPP = Items[i];
        if(SPPP->there_is_Bid) { //if the SPPP correspond to an PP
            if(SPPP->Enabled) { //if the PP is allocated to the RP
                //extract the Id from the SPPP
                int Id = SPPP->Pid;
                //search the RP in the RPL attached to the AL
                int j = AL.getRoboticPositionerList()->searchId(Id);
                //if has found the RP, build and attach a TP
                if(j < AL.getRoboticPositionerList()->getCount()) {
                    //copy the properties in an allocation and add the allocation to the MPG
                    TRoboticPositioner *RP = AL.getRoboticPositionerList()->Get(j);
                    TAllocation *A = new TAllocation(RP, SPPP->X, SPPP->Y);
                    A->PP.Type = SPPP->Type;
                    A->PP.Priority = SPPP->Pr;
                    //add the allocation to the AL
                    AL.Add(A);
                }
            }
        }
    }
}

//build a FMOSA table by default
TFMOSATable::TFMOSATable(void) : TPointersList<TSPPP>()
{
    Print = TSPPP::PrintRow;
}

//clone a FMOSA table
void TFMOSATable::Clone(TFMOSATable& FMOSAT)
{
    setCount(FMOSAT.getCount());
    setCapacity(FMOSAT.getCapacity());
    for(int i=0; i<getCount(); i++)
        *(Items[i]) = FMOSAT[i];
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

