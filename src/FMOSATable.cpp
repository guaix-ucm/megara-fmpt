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
//File: FMOSATable.cpp
//Content: FMOSA table
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
//class TObservingSource:

//SETS OF PROPERTIES IN TEXT FORMAT:

//get the structure in text format
AnsiString TObservingSource::getRowText(void) const
{
    string str = Name;
    while(str.length() < 20)
        str += ' ';

    str += "|"+floattostr_fixed(RA, 6);
    while(str.length() < 30)
        str += ' ';

    str += "|"+floattostr_fixed(Dec, 6);
    while(str.length() < 40)
        str += ' ';


    str += "|";
    if(there_is_Mag)
        str += floattostr_fixed(Mag, 2);
    else
        str += "     ";
    while(str.length() < 46)
        str += ' ';

    str += "|"+pointTypeToStr(Type);
    while(str.length() < 63)
        str += ' ';

    str += "|";
    if(there_is_Pr)
        str += uinttostr(Pr);
    else
        str += "  ";
    while(str.length() < 66)
        str += ' ';

    str += "|";
    if(there_is_Bid)
        str += uinttostr(Bid);
    else
        str += "   ";
    while(str.length() < 70)
        str += ' ';


    str += "|"+uinttostr(Pid);
    while(str.length() < 74)
        str += ' ';

    str += "|"+floattostr_fixed(X, 6);
    while(str.length() < 85)
        str += ' ';

    str += "|"+floattostr_fixed(Y, 6);
    while(str.length() < 96)
        str += ' ';

    str += "|"+inttostr(int(Enabled));
    while(str.length() < 98)
        str += ' ';

    str += "|"+Comment;
    while(str.length() < 119)
        str += ' ';

    AnsiString S(str);
    return S;
}

//set the structure in text format
void TObservingSource::setText(const string& str)
{
    try {
        //decompose the row in fields
        TStrings Strings;
        StrSplit(Strings, str, '|');

        //check the number of fields
        if(Strings.getCount() != 12)
            throw EImproperArgument("should have 12 fields separated by '|': \"<Name> | <RA> | <Dec> | <Mag> | <Type> | <Pr> | <Bid> | <Pid> | <X(mm)> | <Y(mm)> | <Enabled> | <Comment>\"");

        //translate the values and assign to a tampon variable
        TObservingSource OS;

        //trims Name property
        try {
            OS.Name = StrTrim(Strings[0]).str;
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Name: ");
            throw;
        }

        //translate the (RA, Dec) properties
        try {
            OS.RA = strToFloat(Strings[1].str);
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Ra: ");
            throw;
        }
        try {
            OS.Dec = strToFloat(Strings[2].str);
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Dec: ");
            throw;
        }

        //translate Mag property (if it is not empty)
        try {
            string aux = StrTrim(Strings[3]).str;
            if(aux.length() > 0) {
                OS.Mag = strToFloat(aux);
                OS.there_is_Mag = true;
            } else
                OS.there_is_Mag = false;
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Mag: ");
            throw;
        }

        //translate Type property
        try {
            OS.Type = strToPointType(Strings[4].str);
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Type: ");
            throw;
        }

        //translate Pr property (if it is not empty)
        try {
            string aux = StrTrim(Strings[5]).str;
            if(aux.length() > 0) {
                int Pr = strToInt(aux);
                if(Pr < 0)
                    throw EImproperArgument("priority Pr should be nonnegative");
                OS.Pr = (unsigned int)Pr;
                OS.there_is_Pr = true;
            } else
                OS.there_is_Pr = false;
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Pr: ");
            throw;
        }

        //translate Bid property (if it is not empty)
        try {
            string aux = StrTrim(Strings[6]).str;
            if(aux.length() > 0) {
                int Bid = strToInt(aux);
                if(Bid < 0)
                    throw EImproperArgument("block identifier Bid should be nonnegative");
                OS.Bid = (unsigned int)Bid;
                OS.there_is_Bid = true;
            } else
                OS.there_is_Bid = false;
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Bid: ");
            throw;
        }

        //translate projection point properties
        try {
            int Pid = strToInt(Strings[7].str);
            if(Pid < 0)
                throw EImproperArgument("positioner identifier Pid should be nonnegative");
            OS.Pid = (unsigned int)Pid;
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Pid: ");
            throw;
        }
        try {
            OS.X = strToFloat(Strings[8].str);
        } catch(Exception& E) {
            E.Message.Insert(1, "reading X: ");
            throw;
        }
        try {
            OS.Y = strToFloat(Strings[9].str);
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Y: ");
            throw;
        }
        try {
            OS.Enabled = strToBool(Strings[10].str);
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Enabled: ");
            throw;
        }

        ////translate notAllocated property (if it is not empty)
        //aux = StrTrim(Strings[11]).str;
        //if(aux.length() > 0) {
        //    OS.notAllocated = strtobool(aux);
        //    OS.there_is_notAllocated = true;
        //} else
        //    OS.there_is_notAllocated = false;

        ////translate allocateInAll property (if it is not empty)
        //aux = StrTrim(Strings[12]).str;
        //if(aux.length() > 0) {
        //    OS.allocateInAll = strtobool(aux);
        //    OS.there_is_allocateInAll = true;
        //} else
        //    OS.there_is_allocateInAll = false;

        //OS.there_is_notAllocated = true;
        //OS.there_is_allocateInAll = true;

        //trims Comment property
        try {
            OS.Comment = StrTrim(Strings[11]).str;
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Comment: ");
            throw;
        }

        //check the precondition
        if(there_is_Bid && (!there_is_Mag || !there_is_Pr/* || !there_is_notAllocated || !there_is_allocateInAll*/))
            throw EImproperArgument("if field Bid is not empty, all fields (except Name) shall be filled");

        //assign the tampon variable
        *this = OS;

    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting an OS: ");
        throw;
    }
}

//print the properties of an OS in a string
//in row format
void  TObservingSource::printRow(AnsiString &S, const TObservingSource *OS)
{
    //check the precondition
    if(OS == NULL)
        throw EImproperArgument("pointer OS should point to built allocation");

    //add the text to the string in propeerly format
    S += OS->getRowText();
}

//PUBLIC METHODS:

//build a structure by default
TObservingSource::TObservingSource() : RA(0), Dec(0), Mag(0), Type(ptUNKNOWN), Pr(0), Bid(0), Pid(0),
    X(0), Y(0), Enabled(false), //notAllocated(true), allocateInAll(false),
    there_is_Mag(false), there_is_Pr(false), there_is_Bid(false)//,
    //there_is_notAllocated(false), there_is_allocateInAll(false)
{
    Name = "";
    Comment = "";
}

//copy all properties of am object of the same type
TObservingSource& TObservingSource::operator=(const TObservingSource& OS)
{
    Name = OS.Name;
    RA = OS.RA;
    Dec = OS.Dec;
    Mag = OS.Mag;
    Type = OS.Type;
    Pr = OS.Pr;
    Bid = OS.Bid;
    Pid = OS.Pid;
    X = OS.X;
    Y = OS.Y;
    Enabled = OS.Enabled;
    //notAllocated = OS.notAllocated;
    //allocateInAll = OS.allocateInAll;
    Comment = OS.Comment;

    there_is_Mag = OS.there_is_Mag;
    there_is_Pr = OS.there_is_Pr;
    there_is_Bid = OS.there_is_Bid;
    //there_is_notAllocated = OS.there_is_notAllocated;
    //there_is_allocateInAll = OS.there_is_allocateInAll;

    return *this;
}

//build a clon
TObservingSource::TObservingSource(TObservingSource *OS)
{
    //check the preconditions
    if(OS == NULL)
        throw EImproperArgument("pointer OS should point to built observing source");

    *this = *OS;
}

//compare all properties of an object of the same type
bool TObservingSource::operator!=(const TObservingSource& OS)
{
    if(Name != OS.Name ||
            RA != OS.RA ||
            Dec != OS.Dec ||
            Mag != OS.Mag ||
            Type != OS.Type ||
            Pr != OS.Pr ||
            Bid != OS.Bid ||
            Pid != OS.Pid ||
            X != OS.X ||
            Y != OS.Y ||
            Enabled != OS.Enabled ||
            //notAllocated != OS.notAllocated ||
            //allocateInAll != OS.allocateInAll ||
            Comment != OS.Comment ||

            there_is_Mag != OS.there_is_Mag ||
            there_is_Pr != OS.there_is_Pr ||
            there_is_Bid != OS.there_is_Bid)
        //there_is_notAllocated != OS.there_is_notAllocated ||
        //there_is_allocateInAll != OS.there_is_allocateInAll)
        return true;

    return false;
}

//---------------------------------------------------------------------------
//class TFMOSATable:

//read the OB section in tampon variables
void TFMOSATable::readOBText(int& _Id, double& _Ra, double& _Dec, double& _Pos,
                             const string& str)
{
    try {
        //decompose the row in fields
        TStrings Strings;
        StrSplit(Strings, str, '|');

        //check the number of fields
        if(Strings.getCount() != 4)
            throw EImproperArgument("should have 4 fields separated by '|': \"<Id> | <Ra> | <Dec> | <Pos>\"");

        //trims Id property
        try {
            int aux = strToInt(Strings[0].str);
            if(aux < 0)
                throw EImproperArgument("value for block identifier Id shouldbe nonnegative");
            _Id = (unsigned int)aux;
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Id: ");
            throw;
        }

        //translate the (RA, Dec) properties
        try {
            _Ra = strToFloat(Strings[1].str);
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Ra: ");
            throw;
        }
        try {
            _Dec = strToFloat(Strings[2].str);
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Dec: ");
            throw;
        }

        //translate Pos property (if it is not empty)
        try {
            _Pos = strToFloat(Strings[3].str);
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Pos: ");
            throw;
        }
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading OB section: ");
        throw;
    }
}

//set a FMOSA table in text format
void TFMOSATable::setTableText(unsigned int& Bid, const string& str)
{
    int i; //index to rows

    try {
        //divide the string str in lines
        TStrings Strings;
        StrDivideInLines(Strings, str);

        //--------------------------------------------------------------

        //initialize the index to the first line
        i = 0;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@SOB@@ not found");

        //--------------------------------------------------------------

        //discard the coments and empty lines
        while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i].str).Length()<=0))
            i++;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@SOB@@ not found");

        //check if actual line contains the label @@SOB@@
        if(StrTrim(Strings[i]) != AnsiString("@@SOB@@"))
            throw EImproperArgument("label @@SOB@@ not found");

        //discard the label @@SOB@@
        i++;

        //discard the coments and empty lines
        while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i].str).Length()<=0))
            i++;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("OB parameters not found: <Id> | <Ra> | <Dec> | <Pos>");

        //read the OB section
        int _Id;
        double _Ra;
        double _Dec;
        double _Pos;
        readOBText(_Id, _Ra, _Dec, _Pos, Strings[i].str);

        //contabilize the readed line
        i++;

        //discard the coments and empty lines
        while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i].str).Length()<=0))
            i++;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@EOB@@ not found");

        //check if actual line contains the label @@EOB@@
        if(StrTrim(Strings[i]) != AnsiString("@@EOB@@"))
            throw EImproperArgument("label @@EOB@@ not found");

        //discard the label @@EOB@@
        i++;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@SOS@@ not found");

        //--------------------------------------------------------------

        //discard the coments and empty lines
        while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i]).Length()<=0))
            i++;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@SOS@@ not found");

        //check if actual line contains the label @@SOS@@
        if(StrTrim(Strings[i]) != AnsiString("@@SOS@@"))
            throw EImproperArgument("label @@SOS@@ not found");

        //discard the label @@SOS@@
        i++;

        //discard the coments and empty lines
        while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i].str).Length()<=0))
            i++;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("OS parameters not found: \"<Name> | <RA> | <Dec> | <Mag> | <Type> | <Pr> | <Bid> | <Pid> | <X(mm)> | <Y(mm)> | <Enabled> | <Comment>\"");

        //read all lines (using a tampon variable) until the close label @@EOS@
        try {
            TFMOSATable FMOSAT;

            while(i<Strings.getCount() && StrTrim(Strings[i])!=AnsiString("@@EOS@@")) {
                TObservingSource *OS = new TObservingSource();
                FMOSAT.Add(OS);
                OS->setText(Strings[i].str);

                //check the precondition
                if(OS->Type != ptUNKNOWN)
                    if(OS->Bid != Id)
                        throw EImproperArgument("all Bid should be equal to Id");

                i++;
            }

            //set the tampons variables
            *this = FMOSAT;
            Id = (unsigned int)_Id;
            Ra = _Ra;
            Dec = _Dec;
            Pos = _Pos;

        } catch(Exception& E) {
            E.Message.Insert(1, "setting OS section: ");
            throw;
        }

        //discard the coments and empty lines
        while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i].str).Length()<=0))
            i++;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@EOS@@ not found");

        //check if actual line contains the label @@EOS@@
        if(StrTrim(Strings[i]) != AnsiString("@@EOS@@"))
            throw EImproperArgument("label @@EOS@@ not found");

        //discard the label @@EOS@@
        i++;

        //discard the coments and empty lines
        while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i]).Length()<=0))
            i++;

        //look for unexpected text
        if(i < Strings.getCount())
            throw EImproperArgument("only should to be OB and OS block");

        //--------------------------------------------------------------

        //return the Bid value
        Bid = Id;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting FMOSA table in text format in row "+inttostr(i+1)+": ");
        throw;
    }
}

//get the FMOSA table in text format
void TFMOSATable::getTableText(string& str) const
{
    str = "# Id| Ra| Dec| Pos";
    str += "\r\n@@SOB@@";
    str += "\r\n"+inttostr(Id)+"| "+floattostr_fixed(Ra, 6)+"| "+floattostr_fixed(Dec, 6)+"| "+floattostr_fixed(Pos, 6);
    str += "\r\n@@EOB@@";

    str += "\r\n#      Name             RA         Dec    Mag        Type         Pr  Bid Pid   X(mm)     Y(mm)  Enabled      Comment";
    str += "\r\n@@SOS@@";
    str += "\r\n"+getColumnText().str;
    str += "\r\n@@EOS@@";
}

//get the Pids of the allocations which accomplish:
//  there_is_Bid && Enabled
//  and Pid is not found in the FMM
void TFMOSATable::searchMissingPids(TVector<int>& Pids, TAllocationList& AL)
{
    //initialize the output
    Pids.Clear();

    //check each Pid
    for(int i=0; i<getCount(); i++) {
        TObservingSource *OS = Items[i];
        if(OS->there_is_Bid) { //if the PP is allocated to the RP
            if(OS->Enabled) { //if the RP is enabled
                //search the RP in the RPL attached to the AL
                int j = AL.getRoboticPositionerList()->searchId(OS->Pid);
                //check if not has found the RP in the FMM
                if(j >= AL.getRoboticPositionerList()->getCount())
                    Pids.Add(OS->Pid);
            }
        }
    }
}

//get the allocations which accomplish: there_is_Bid && Enabled
void TFMOSATable::getAllocations(TAllocationList& AL)
{
    //check the precondition
    TVector<int> Pids;
    searchMissingPids(Pids, AL);
    if(Pids.getCount() > 0)
        throw EImproperArgument("there are Pids not found in the FMM: "+Pids.getText().str);

    try {
        //build an allocation for each OS
        for(int i=0; i<getCount(); i++) {
            TObservingSource *OS = Items[i];
            if(OS->there_is_Bid) { //if the PP is allocated to the RP
                if(OS->Enabled) { //if the RP is enabled
                    //extract the Id from the OS
                    int Id = OS->Pid;
                    //search the RP in the RPL attached to the AL
                    int j = AL.getRoboticPositionerList()->searchId(Id);
                    //check if not has found the RP in the FMM
                    if(j >= AL.getRoboticPositionerList()->getCount())
                        throw EImpossibleError("lateral effect");
                    //copy the properties in an allocation and add the allocation to the allocation list
                    TRoboticPositioner *RP = AL.getRoboticPositionerList()->Get(j);
                    TAllocation *A = new TAllocation(RP, OS->X, OS->Y);
                    A->PP.Type = OS->Type;
                    A->PP.Priority = OS->Pr;
                    //add the allocation to the AL
                    AL.Add(A);
                }
            }
        }
    }
    catch(Exception& E) {
        E.Message.Insert(1, "getting allocations from the FMOSA table: ");
        throw;
    }
}

//build a FMOSA table by default
TFMOSATable::TFMOSATable(void) : TPointersList<TObservingSource>(),
    Id(0), Ra(0), Dec(0), Pos(0)
{
    Print = TObservingSource::printRow;
}

//clone a FMOSA table
void TFMOSATable::Clone(TFMOSATable& FMOSAT)
{
    Id = FMOSAT.Id;
    Ra = FMOSAT.Ra;
    Dec = FMOSAT.Dec;
    Pos = FMOSAT.Pos;

    //clona el array deslizante
    Items.Clone(FMOSAT.Items);

    //apunta a las mismas funciones externas
    Compare = FMOSAT.Compare;
    Evaluate = FMOSAT.Evaluate;
    Assign = FMOSAT.Assign;
    Print = FMOSAT.Print;
    Read = FMOSAT.Read;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

