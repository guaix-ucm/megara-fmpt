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
//File: ParkingProgram.cpp
//Content: structure for contains a generated parking program (PP, DP)
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "OutputsParkProg.h"
#include "Strings.h"

//---------------------------------------------------------------------------

using namespace Strings;

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//class ParkProg:
//---------------------------------------------------------------------------

//get the comments about ParkProg
string OutputsParkProg::getComments(void) const
{
    locale l;
    if(l.name() != "C")
        throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

    string str = "# Parkig program outputs generated ";
    if(FMOSA_filename.length() > 0)
        str += "from FMOSA " + FMOSA_filename;
    else
        str += "online (not from FMOSA)";
    str += "\r\n# Generated with FMPT version "+FMPT_version;
    str += "\r\n# Date of generation: "+datetime;
    str += "\r\n# When FMOSA contains either collided or obstructed RPs, the generated parking program is valid.";
    if(Collided.getCount() > 0 || Obstructed.getCount() > 0 || EnabledNotOperative.getCount() > 0) {
        if(Obstructed.getCount() > 0) {
            str += "\r\n# WARNING! This pair (PP, DP) not valid, because the FMOSA contains enabled-not-operative RPs:";
            str += "\r\n# Enabled-not-operative RP ids: " + EnabledNotOperative.getText().str;
        }
        if(Collided.getCount() > 0) {
            str += "\r\n# WARNING! the FMOSA contains collided items (either EAs or RPs):";
            str += "\r\n# Collided items: " + collided_str;
            str += "\r\n# Collided RP ids: " + Collided.getText().str;
        }
        if(Obstructed.getCount() > 0) {
            str += "\r\n# WARNING! the FMOSA contains obstructed RPs:";
            str += "\r\n# Obstructed RP ids: " + Obstructed.getText().str;
        }
        str += "\r\n# The starting positions of the collided and obstructed RPs match with their final positions.";
    }
    else
        str += "\r\n# This parking program has been generated without enabled-not-operative RPs and without neither collided nor obstructed RPs.";

    return str;
}

//get ParkProg in text format with:
//  comments
//  the parking program
void OutputsParkProg::getText(string& str) const

{
    locale l;
    if(l.name() != "C")
        throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

    //print the coments about ParkProg
    str = getComments();

    str += "\r\n";
    str += "\r\n# Parking program";
    if(ParkProgValid) {
        str += "\r\n# This parking program avoids collisions when it is executed starting from the starting positions (check out SPL).";
    } else {
        str += "\r\n# ERROR! This parking program produces a collision when it is executed starting from the starting positions (check out SPL).";
    }
    if(ParkProg.thereIsSomeCommentDsec()) {
        str += "\r\n# WARNING! This parking program contains instructions that produce radial movements which produce approximations closer than "+floattostr(DsecMax)+" mm:";
        str += "\r\n"+ParkProg.getCommentsDsecInterfaceText();
    } else {
        str += "\r\n# All radial movements of this parking program keep a security distance upper or equal to "+floattostr(DsecMax)+" mm.";
    }
    str += "\r\n@@SParkProg@@";
    string aux;
    ParkProg.getInterfaceText(aux, "depos", Bid, SPL);
    str += "\r\n"+aux;
    str += "\r\n@@EParkProg@@";
}

//get other outputs in text format with:
//  comments
//  other properties in format assigns
void OutputsParkProg::getOtherText(string& str) const
{
    locale l;
    if(l.name() != "C")
        throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

    //print the coments about ParkProg
    str = getComments();

    str += "\r\n";
    str += "\r\n# Parking program";
    if(ParkProgValid) {
        str += "\r\n# This parking program avoids collisions when it is executed starting from the starting positions.";
    } else {
        str += "\r\n# ERROR! This parking program produces a collision when it is executed starting from the starting positions.";
    }
    if(ParkProg.thereIsSomeCommentDsec()) {
        str += "\r\n# WARNING! This parking program contains instructions that produce radial movements which produce approximations closer than DsecMax:";
        str += "\r\n"+ParkProg.getCommentsDsecInterfaceText();
    } else {
        str += "\r\n# All radial movements of this parking program keep a security distance upper or equal to "+floattostr(DsecMax)+" mm.";
    }

    //print other properties in format assigns
    str += "\r\n";
    str += "\r\nParkProgValid: "+BoolToStr(ParkProgValid,true).str;
    str += "\r\nCollided: "+Collided.getText().str;
    str += "\r\nObstructed: "+Obstructed.getText().str;
    str += "\r\nCollided (including EAs): "+collided_str;
}

//set the ParkProg in text format
void OutputsParkProg::setText(const string& str)
{
    try {
        locale l;
        if(l.name() != "C")
            throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

        //divide the string str in lines
        TStrings Strings;
        StrDivideInLines(Strings, str);

        //initialize the index to the first line
        int i = 0;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@SParkProg@@ not found");

        //-----------------------------------------------------------------------
        //SEGREGATE THE ParkProg AND CONVERT IT TO STRUCTURE:

        //discard the coments and empty lines
        while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i].str).Length()<=0))
            i++;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@SParkProg@@ not found");

        //check if actual line contains the label @@SParkProg@@
        if(StrTrim(Strings[i]) != AnsiString("@@SParkProg@@"))
            throw EImproperArgument("label @@SParkProg@@ not found");

        //discard the label
        i++;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("should to have a positioning program");

        //segregate all lines until the close label @@EParkProg@
        string str = Strings[i++].str;
        while(i<Strings.getCount() && StrTrim(Strings[i])!=AnsiString("@@EParkProg@@")) {
            //check if there are more lines
            if(i >= Strings.getCount())
                throw EImproperArgument("label @@EParkProg@@ not found");

            //add the next line and increase the index
            str += "\r\n";
            str += Strings[i++].str;
        }

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@EParkProg@@ not found");

        //check if actual line contains the label @@EParkProg@@
        if(StrTrim(Strings[i]) != AnsiString("@@EParkProg@@"))
            throw EImproperArgument("label @@EParkProg@@ not found");

        //discard the label
        i++;

        //discard the coments and empty lines
        while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i]).Length()<=0))
            i++;

        //look for unexpected text
        if(i < Strings.getCount())
            throw EImproperArgument("only should to be OB and OS block");

        //convert to structure
        TMotionProgram t_ParkProg;
        string labelParkProg;
        unsigned int BidParkProg;
        t_ParkProg.setInterfaceText(labelParkProg, BidParkProg, str);

        //discard the label
        i++;

        //-----------------------------------------------------------------------
        //CHECK THE PRECONDITIONS:

        //It is not necessary:
        //  if(Bid < 0)
        //      throw EImproperFileLoadedValue("the CB Id of the ParkPprog should be non-negative");

        if(strTrim(labelParkProg) != "depos")
            throw EImproperFileLoadedValue("the ParkProg label should be \"depos\"");

        //-----------------------------------------------------------------------
        //SET THE TAMPONS VARIABLES:

        ParkProg = t_ParkProg;
        Bid = BidParkProg;

    } catch(Exception& E) {
        E.Message.Insert(1, "setting text in ParkProg structure: ");
        throw;
    } catch(...) {
        throw;
    }
}

//build an estructure of type OutputsParkProg
OutputsParkProg::OutputsParkProg() :
    //inicializa las properties for built comments about file ParkProg
    FMOSA_filename(""),
    FMPT_version(""),
    datetime(""),
    EnabledNotOperative(),
    collided_str(""),
    Collided(),
    Obstructed(),
    //properties for built comments about each MP
    Bid(0), SPL(), ParkProgValid(false), FPL(), DsecMax(0),
    //properties for built the executable data
    ParkProg()
{
}

//reset all properties to values by default
void OutputsParkProg::Clear(void)
{
    FMOSA_filename = "";
    FMPT_version = "";
    datetime = "";
    EnabledNotOperative.Clear();
    collided_str = "";
    Collided.Clear();
    Obstructed.Clear();
    ParkProgValid = false;
    DsecMax = 0;
    Bid = 0;
    SPL.Clear();
    ParkProg.Clear();
    FPL.Clear();
}

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
