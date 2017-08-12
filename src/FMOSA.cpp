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
/// @file FMOSA.cpp
/// @brief structure FMOSA
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#include "FMOSA.h"
#include "Strings.h"
#include "StrPR.h"

#include <algorithm> //transform

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
    while(str.length() < 31)
        str += ' ';

    str += "|"+floattostr_fixed(Dec, 6);
    while(str.length() < 42)
        str += ' ';


    str += "|";
    if(there_is_Mag)
        str += floattostr_fixed(Mag, 2);
    else
        str += "     ";
    while(str.length() < 48)
        str += ' ';

    str += "|"+pointTypeToStr(Type);
    while(str.length() < 65)
        str += ' ';

    str += "|";
    if(there_is_Pr)
        str += uinttostr(Pr);
    else
        str += "  ";
    while(str.length() < 68)
        str += ' ';

    str += "|";
    if(there_is_Bid)
        str += uinttostr(Bid);
    else
        str += "   ";
    while(str.length() < 72)
        str += ' ';


    str += "|"+uinttostr(Pid);
    while(str.length() < 76)
        str += ' ';

    str += "|"+floattostr_fixed(X, 6);
    while(str.length() < 87)
        str += ' ';

    str += "|"+floattostr_fixed(Y, 6);
    while(str.length() < 98)
        str += ' ';

    str += "|"+floattostr_fixed(Angle, 4);
    while(str.length() < 109)
        str += ' ';

    str += "|"+inttostr(int(Enabled));
    while(str.length() < 111)
        str += ' ';

    str += "|"+Comment;
    while(str.length() < 132)
        str += ' ';

    AnsiString S(str);
    return S;
}

//get the structure in JSON format
Json::Value TObservingSource::getJSON(void) const
{
    Json::Value OS_JSON;
    OS_JSON["coordinate_x"] = X;
    OS_JSON["coordinate_y"] = Y;
    OS_JSON["position_angle"] = Angle;
    OS_JSON["RA"] = RA;
    OS_JSON["Dec"] = Dec;
    OS_JSON["enabled"] = Enabled;
    OS_JSON["name"] = Name;

    if(there_is_Pr)
        OS_JSON["priority"] = Pr;
    else
        OS_JSON["priority"] = 0;

    OS_JSON["rpid"] = Pid;
    OS_JSON["type"] = pointTypeToStr(Type);

    if(there_is_Mag)
        OS_JSON["magnitude"] = Mag;
    else
        OS_JSON["magnitude"] = 0;

    OS_JSON["comment"] = Comment;

    //Note that Bid has been missed:
    //  if(there_is_Bid)
    //      OS_JSON["Bid"] = Bid;
    //  else
    //      OS_JSON["Bid"] = 0;

    return OS_JSON;
}

//set the structure in text format
void TObservingSource::setText(const string& str)
{
    try {
        //decompose the row in fields
        TStrings Strings;
        StrSplit(Strings, str, '|');

        //check the number of fields
        if(Strings.getCount() != 13)
            throw EImproperArgument("should have 13 fields separated by '|': \"<Name> | <RA> | <Dec> | <Mag> | <Type> | <Pr> | <Bid> | <Pid> | <X(mm)> | <Y(mm)> | <Angle(deg)> | <Enabled> | <Comment>\"");

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
            OS.Angle = strToFloat(Strings[10].str);
        } catch(Exception& E) {
            E.Message.Insert(1, "reading Angle: ");
            throw;
        }
        try {
            OS.Enabled = strToBool(Strings[11].str);
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
            OS.Comment = StrTrim(Strings[12]).str;
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
    X(0), Y(0), Angle(0), Enabled(false), //notAllocated(true), allocateInAll(false),
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
    Angle = OS.Angle;
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
            Angle != OS.Angle ||
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
//class TFMOSA:

// get from the comments the value after "Total number of blocks"
// if not found return empty string
int TFMOSA::getTotal_number_of_blocks(void) const
{
    //trye read the string after label "Total number of blocks"
    for(int i=0; i<comments.getCount(); i++) {
        string line = comments[i].str;
        try {
            unsigned int j = 0;
            strTravelLabel("#", line, j);
            strTravelSeparatorsIfAny(line, j);
            strTravelLabel("Total number of blocks", line, j);
            strTravelLabel(":", line, j);
            strTravelSeparatorsIfAny(line, j);

            int n;
            strReadInt(n, line, j);

            return n;
        } catch(...) {
            //do nothing
        }
    }
    //return the default value
    return 0;
}

// get from the comments the value after "Title"
// if not found return empty string
string TFMOSA::getTitle(void) const
{
    //trye read the string after label "Tittle"
    for(int i=0; i<comments.getCount(); i++) {
        string line = comments[i].str;
        try {
            unsigned int j = 0;
            strTravelLabel("#", line, j);
            strTravelSeparatorsIfAny(line, j);
            strTravelLabel("Title", line, j);
            strTravelLabel(":", line, j);
            strTravelSeparatorsIfAny(line, j);
            string aux = line.substr(j, line.length()-1);
            return aux;
        } catch(...) {
            //do nothing
        }
    }
    //return the empty string
    return "";
}

// get from the comments the value after "Description"
// if not found return empty string
string TFMOSA::getDescription(void) const
{
    //trye read the string after label "Description"
    for(int i=0; i<comments.getCount(); i++) {
        string line = comments[i].str;
        try {
            unsigned int j = 0;
            strTravelLabel("#", line, j);
            strTravelSeparatorsIfAny(line, j);
            strTravelLabel("Description", line, j);
            strTravelLabel(":", line, j);
            strTravelSeparatorsIfAny(line, j);
            string aux = line.substr(j, line.length()-1);
            return aux;
        } catch(...) {
            //do nothing
        }
    }
    //return the empty string
    return "";
}

// get from the comments the value after "Generated with FMAT version"
// if not found return empty string
string TFMOSA::getGenerated_with_FMAT_version(void) const
{
    //trye read the string after label "Generated with FMAT version"
    for(int i=0; i<comments.getCount(); i++) {
        string line = comments[i].str;
        try {
            unsigned int j = 0;
            strTravelLabel("#", line, j);
            strTravelSeparatorsIfAny(line, j);
            strTravelLabel("Generated with FMAT version", line, j);
            strTravelLabel(":", line, j);
            strTravelSeparatorsIfAny(line, j);
            string aux = line.substr(j, line.length()-1);
            return aux;
        } catch(...) {
            //do nothing
        }
    }
    //return the empty string
    return "";
}

// get from the comments the value after "Properties file version"
// if not found return empty string
int TFMOSA::getProperties_file_version(void) const
{
    //trye read the string after label "Properties file version"
    for(int i=0; i<comments.getCount(); i++) {
        string line = comments[i].str;
        try {
            unsigned int j = 0;
            strTravelLabel("#", line, j);
            strTravelSeparatorsIfAny(line, j);
            strTravelLabel("Properties file version", line, j);
            strTravelLabel(":", line, j);
            strTravelSeparatorsIfAny(line, j);

            int n;
            strReadInt(n, line, j);

            return n;
        } catch(...) {
            //do nothing
        }
    }
    //return the default value
    return 0;
}

// get from the comments the value after "Date of generation"
// if not found return empty string
string TFMOSA::getDate_of_generation(void) const
{
    //trye read the string after label "Date of generation"
    for(int i=0; i<comments.getCount(); i++) {
        string line = comments[i].str;
        try {
            unsigned int j = 0;
            strTravelLabel("#", line, j);
            strTravelSeparatorsIfAny(line, j);
            strTravelLabel("Date of generation", line, j);
            strTravelSeparatorsIfAny(line, j);
            string aux = line.substr(j, line.length()-1);
            return aux;
        } catch(...) {
            //do nothing
        }
    }
    //return the empty string
    return "";
}

//read the OB section in tampon variables
void TFMOSA::readOBText(int& _Id, double& _Ra, double& _Dec, double& _Pos,
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

//set a FMOSA in text format
void TFMOSA::setTableText(unsigned int& Bid, const string& str)
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

        //discard the empty lines and read the comments
        TFMOSA t_FMOSA;
        while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i].str).Length()<=0)) {
            if(strFirstNonseparatorChar(Strings[i].str)=='#')
                t_FMOSA.comments.Add(StrTrim(Strings[i]));
            i++;
        }

        //delete the header of the OB
        try {
            string str = t_FMOSA.comments[t_FMOSA.comments.getCount() - 1].str;
            transform(str.begin(), str.end(), str.begin(), (int (*)(int))tolower);
            unsigned int i = 0;
            strTravelSeparatorsIfAny(str, i);
            strTravelLabel("#", str, i);
            strTravelSeparatorsIfAny(str, i);
            strTravelLabel("id", str, i);
            strTravelSeparatorsIfAny(str, i);
            strTravelLabel("|", str, i);
            strTravelSeparatorsIfAny(str, i);
            strTravelLabel("ra", str, i);
            strTravelSeparatorsIfAny(str, i);
            strTravelLabel("|", str, i);
            strTravelSeparatorsIfAny(str, i);
            strTravelLabel("dec", str, i);
            strTravelSeparatorsIfAny(str, i);
            strTravelLabel("|", str, i);
            strTravelSeparatorsIfAny(str, i);
            strTravelLabel("pos", str, i);
            t_FMOSA.comments.setCount(t_FMOSA.comments.getCount() - 1);
        }
        catch(...) {
            //do nothing
        }

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

        //read the OB section in tampon variables
        int t_Id;
        readOBText(t_Id, t_FMOSA.Ra, t_FMOSA.Dec, t_FMOSA.Pos, Strings[i].str);

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
            throw EImproperArgument("OS parameters not found: \"<Name> | <RA> | <Dec> | <Mag> | <Type> | <Pr> | <Bid> | <Pid> | <X(mm)> | <Y(mm)> | | <Angle(deg)> | <Enabled> | <Comment>\"");

        //read all lines (using a tampon variable) until the close label @@EOS@
        try {
            while(i<Strings.getCount() && StrTrim(Strings[i])!=AnsiString("@@EOS@@")) {
                TObservingSource *OS = new TObservingSource();
                t_FMOSA.Add(OS);
                OS->setText(Strings[i].str);

                //check the precondition
                if(OS->Type != ptUNKNOWN)
                    if(OS->Bid != (unsigned int)t_Id)
                        throw EImproperArgument("all Bid should be equal to Id");

                i++;
            }
        }
        catch(Exception& E) {
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

        //set the tampons variables
        t_FMOSA.Id = (unsigned int)t_Id;
        *this = t_FMOSA;

        //REMEMBER: not define the operator= may produce erased of attributes. E.i:
        //  *this = t_FMOSA;
        //When TFMOSA::operator= is not defined, is called the function TPointersList<>::operator=,
        //but properties of the FMOSA also will be assigned.

        //set the last valid setted string
        str_original = str;

        //return the Bid value
        Bid = Id;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting FMOSA in text format in row "+inttostr(i+1)+": ");
        throw;
    }
}

//get the FMOSA in text format
void TFMOSA::getTableText(string& str) const
{
    if(str_original.length() > 0)
        str = str_original;
    else {
        str = "# Id| Ra\t | Dec\t      | Pos";
        str += "\r\n@@SOB@@";
        string aux = inttostr(Id);
        while(aux.length() < 7)
            aux += ' ';
        str += "\r\n"+aux+" |"+floattostr_fixed(Ra, 6)+" |"+floattostr_fixed(Dec, 6)+" |"+floattostr_fixed(Pos, 6);
        str += "\r\n@@EOB@@";

        str += "\r\n#      Name         |    RA   |    Dec  | Mag |     Type       |Pr|Bid|Pid| X(mm)  | Y(mm)  | Angle(deg) | Enabled| Comment";
        str += "\r\n@@SOS@@";
        str += "\r\n"+getColumnText().str;
        str += "\r\n@@EOS@@";
    }
}

//get the FMOSA in format JSON
Json::Value TFMOSA::getJSON(void) const
{
    //build the FMOSA in format JSON
    Json::Value FMOSA_JSON;

    //  //bild the comments in format JSON
    //  Json::Value comments_JSON;
    //  for(int i=0; i<comments.getCount(); i++)
    //      comments_JSON.append(comments[i].str);
    //  FMOSA_JSON["comments"] = comments_JSON;

    FMOSA_JSON["nblocks"] = getTotal_number_of_blocks();
    FMOSA_JSON["title"] = getTitle();
    FMOSA_JSON["description"] = getDescription();
    FMOSA_JSON["fmat_version"] = getGenerated_with_FMAT_version();
    FMOSA_JSON["fmat_properties"] = getProperties_file_version();
    FMOSA_JSON["creation_date"] = getDate_of_generation();
    FMOSA_JSON["bid"] = Id;

    //build the OB in format JSON
    Json::Value OB_JSON;
    //OB_JSON["Id"] = Id;
    OB_JSON["RA"] = Ra;
    OB_JSON["Dec"] = Dec;
    OB_JSON["position_angle"] = Pos;
    FMOSA_JSON["coordinates"] = OB_JSON;

    //build the OS in ofmrat JSON
    Json::Value OS_JSON;
    for(int i=0; i<getCount(); i++) {
        const TObservingSource *OS = Items[i];
        OS_JSON.append(OS->getJSON());
    }
    FMOSA_JSON["targets"] = OS_JSON;

    return FMOSA_JSON;
}

//get the Pids of the OSs which accomplish:
//  there_is_Bid
//  Pid is not found in the FMM
void TFMOSA::searchMissingPids(TVector<int>& Pids,
                                    const TRoboticPositionerList2 *RPL)
{
    //check the precondition
    if(RPL == NULL)
        throw EImproperArgument("pinter RPL should point to built robotic positioner list");

    //initialize the output
    Pids.Clear();

    //check each OS
    for(int i=0; i<getCount(); i++) {
        TObservingSource *OS = Items[i];
        if(OS->there_is_Bid) { //if the PP is allocated to the RP
            //search the RP in the RPL attached to the AL
            int j = RPL->searchId(OS->Pid);
            //check if not has found the RP in the FMM
            if(j >= RPL->getCount())
                Pids.Add(OS->Pid);
        }
    }
}

//get the Pids of the OSs which accomplish:
//  there_is_Bid
//  Enabled don't match with Disbaled in the FMM
//If there are missing Pids:
//  throw EImproperCall
void TFMOSA::searchDontMatchEnabled(TVector<int>& Pids,
                                         const TRoboticPositionerList2 *RPL)
{
    //check the precondition
    if(RPL == NULL)
        throw EImproperArgument("pinter RPL should point to built robotic positioner list");

    //check the preconditions
    TVector<int> Pids_;
    searchMissingPids(Pids_, RPL);
    if(Pids_.getCount() > 0)
        throw EImproperCall("there are Pids not found in the FMM: "+Pids_.getText().str);

    //initialize the output
    Pids.Clear();

    //check each OS
    for(int i=0; i<getCount(); i++) {
        TObservingSource *OS = Items[i];
        if(OS->there_is_Bid) { //if the PP is allocated to the RP
            //search the RP in the RPL attached to the AL
            int j = RPL->searchId(OS->Pid);
            //make a rutinary check
            if(j >= RPL->getCount())
                throw EImpossibleError("lateral effect");
            //check that Enabled match with Disabled in the RPL
            if(OS->Enabled == RPL->Get(j)->Disabled)
                Pids.Add(OS->Pid);
        }
    }
}

//get the allocations which accomplish: there_is_Bid && Enabled
//Preconditions:
//  there aren't missing Pids;
//  there aren't don't match Enabled.
//Exceptions:
//  EImproperCall: If not meet the pre-conditions
//Allocations to not operative (although enabled) RPs, can be got,
//but them motion programs can will be generated only by force.
void TFMOSA::getAllocations(TAllocationList& AL)
{
    //check the preconditions
    TVector<int> Pids;
    searchMissingPids(Pids, AL.getRoboticPositionerList());
    if(Pids.getCount() > 0)
        throw EImproperCall("there are Pids not found in the FMM: "+Pids.getText().str);

    searchDontMatchEnabled(Pids, AL.getRoboticPositionerList());
    if(Pids.getCount() > 0)
        throw EImproperCall("there are Enabled (in the FMOSA) that don't match with Disabled (in the FMM Instance): "+Pids.getText().str);

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
        E.Message.Insert(1, "getting allocations from the FMOSA: ");
        throw;
    }
}

//build a FMOSA by default
TFMOSA::TFMOSA(void) : TPointersList<TObservingSource>(),
    str_original(""), comments(),
    Id(0), Ra(0), Dec(0), Pos(0)
{
    Print = TObservingSource::printRow;
}

//clone a FMOSA
void TFMOSA::Clone(TFMOSA& FMOSA)
{
    //clone the original setted string
    str_original = FMOSA.str_original;

    //clone the comments
    comments.Clone(FMOSA.comments);

    //clone OB properties
    Id = FMOSA.Id;
    Ra = FMOSA.Ra;
    Dec = FMOSA.Dec;
    Pos = FMOSA.Pos;

    //clone OSs
    Items.Clone(FMOSA.Items);

    //point the same external functions
    Compare = FMOSA.Compare;
    Evaluate = FMOSA.Evaluate;
    Assign = FMOSA.Assign;
    Print = FMOSA.Print;
    Read = FMOSA.Read;
}

//copy a FMOSA
TFMOSA& TFMOSA::operator=(const TFMOSA& FMOSA)
{
    //copy the original setted string
    str_original = FMOSA.str_original;

    //copy the comments
    comments = FMOSA.comments;

    //copy OB properties
    Id = FMOSA.Id;
    Ra = FMOSA.Ra;
    Dec = FMOSA.Dec;
    Pos = FMOSA.Pos;

    //copy OSs
    Items.Copy(FMOSA.Items);

    return *this;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

