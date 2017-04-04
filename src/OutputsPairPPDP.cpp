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
//File: OutputsPairPPDP.cpp
//Content: structure for contains all outputs data about a generated pair (PP, DP)
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "OutputsPairPPDP.h"
#include "Strings.h"

//---------------------------------------------------------------------------

using namespace Strings;

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//class OutputsPairPPDP:
//---------------------------------------------------------------------------

//get the comments about outputs
string OutputsPairPPDP::getComments(void) const
{
    locale l;
    if(l.name() != "C")
        throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

    string str = "# Pair (PP, DP) outputs generated ";
    if(FMOSA_filename.length() > 0)
        str += "from FMOSA " + FMOSA_filename;
    else
        str += "online (not from FMOSA)";
    str += "\r\n# Generated with FMPT version "+FMPT_version;
    str += "\r\n# Date of generation: "+datetime;
    str += "\r\n# When FMOSA contains either collided or obstructed RPs, the generated pair (PP, DP) is not valid.";
    if(Collided.getCount() > 0 || Obstructed.getCount() > 0 || EnabledNotOperative.getCount() > 0) {
        if(Obstructed.getCount() > 0) {
            str += "\r\n# WARNING! This pair (PP, DP) is not valid, because the FMOSA contains enabled-not-operative RPs:";
            str += "\r\n# Enabled-not-operative RP ids: " + EnabledNotOperative.getText().str;
        }
        if(Collided.getCount() > 0) {
            str += "\r\n# WARNING! This pair (PP, DP) is not valid, because the FMOSA contains collided items (either EAs or RPs):";
            str += "\r\n# Collided items: " + collided_str;
            str += "\r\n# Collided RP ids: " + Collided.getText().str;
        }
        if(Obstructed.getCount() > 0) {
            str += "\r\n# WARNING! This pair (PP, DP) is not valid, because the FMOSA contains obstructed RPs:";
            str += "\r\n# Obstructed RP ids: " + Obstructed.getText().str;
        }
        str += "\r\n# The initial positions of the collided and obstructed RPs match with their observing position.";
    }
    else
        str += "\r\n# This pair (PP, DP) has been generated without enabled-not-operative RPs and without neither collided nor obstructed RPs.";

    return str;
}

//get outputs in text format with:
//  comments
//  the pair (PP, DP)
//  the FMOSA
void OutputsPairPPDP::getText(string& str, bool includeFMOSA) const

{
    locale l;
    if(l.name() != "C")
        throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

    //print the coments about outputs
    str = getComments();

    str += "\r\n";
    str += "\r\n# Positioning program";
    if(PPvalid) {
        str += "\r\n# This PP avoids collisions when it is executed starting from the initial positions (check out IPL).";
    } else {
        str += "\r\n# ERROR! This PP produces a collision when it is executed starting from the initial positions (check out IPL).";
    }
    if(PP.thereIsSomeCommentDsec()) {
        str += "\r\n# WARNING! This PP contains instructions that produce radial movements which produce approximations closer than "+floattostr(DsecMax)+" mm:";
        str += "\r\n"+PP.getCommentsDsecInterfaceText();
    } else {
        str += "\r\n# All radial movements of this PP keep a security distance upper or equal to "+floattostr(DsecMax)+" mm.";
    }
    str += "\r\n@@SPP@@";
    string aux;
    PP.getInterfaceText(aux, "pos", Bid, IPL);
    str += "\r\n"+aux;
    str += "\r\n@@EPP@@";

    str += "\r\n";
    str += "\r\n# Depositioning program";
    if(DPvalid) {
        str += "\r\n# This DP avoids collisions when it is executed starting from the observing positions (check out OPL).";
    } else {
        str += "\r\n# ERROR! This DP produces a collision when it is executed starting from the observing positions (check out OPL).";
    }
    if(DP.thereIsSomeCommentDsec()) {
        str += "\r\n# WARNING! This DP contains instructions that produce radial movements which produce approximations closer than "+floattostr(DsecMax)+" mm:";
        str += "\r\n"+DP.getCommentsDsecInterfaceText();
    } else {
        str += "\r\n# All radial movements of this DP keep a security distance upper or equal to "+floattostr(DsecMax)+" mm.";
    }
    str += "\r\n@@SDP@@";
    DP.getInterfaceText(aux, "depos", Bid, OPL);
    str += "\r\n"+aux;
    str += "\r\n@@EDP@@";

    if(includeFMOSA) {
        str += "\r\n";
        FMOSA.getTableText(aux);
        str += "\r\n"+aux;
    }
}

//get other outputs in text format with:
//  comments
//  other properties in format assigns
void OutputsPairPPDP::getOtherText(string& str) const
{
    locale l;
    if(l.name() != "C")
        throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

    //print the coments about outputs
    str = getComments();

    str += "\r\n";
    str += "\r\n# Positioning program";
    if(PPvalid) {
        str += "\r\n# This PP avoids collisions when it is executed starting from the initial positions.";
    } else {
        str += "\r\n# ERROR! This PP produces a collision when it is executed starting from the initial positions.";
    }
    if(PP.thereIsSomeCommentDsec()) {
        str += "\r\n# WARNING! This PP contains instructions that produce radial movements which produce approximations closer than "+floattostr(DsecMax)+" mm:";
        str += "\r\n"+PP.getCommentsDsecInterfaceText();
    } else {
        str += "\r\n# All radial movements of this PP keep a security distance upper or equal to "+floattostr(DsecMax)+" mm.";
    }

    str += "\r\n";
    str += "\r\n# Depositioning program";
    if(DPvalid) {
        str += "\r\n# This DP avoids collisions when it is executed starting from the observing positions.";
    } else {
        str += "\r\n# ERROR! This DP produces a collision when it is executed starting from the observing positions.";
    }
    if(DP.thereIsSomeCommentDsec()) {
        str += "\r\n# WARNING! This DP contains instructions that produce radial movements which produce approximations closer than "+floattostr(DsecMax)+" mm:";
        str += "\r\n"+DP.getCommentsDsecInterfaceText();
    } else {
        str += "\r\n# All radial movements of this DP keep a security distance upper or equal to "+floattostr(DsecMax)+" mm.";
    }

    //print other properties in format assigns
    str += "\r\n";
    str += "\r\nPPvalid: "+BoolToStr(PPvalid,true).str;
    str += "\r\nDPvalid: "+BoolToStr(DPvalid,true).str;
    str += "\r\nCollided: "+Collided.getText().str;
    str += "\r\nObstructed: "+Obstructed.getText().str;
    str += "\r\nCollided (including EAs): "+collided_str;
}

//set the outputs in text format
void OutputsPairPPDP::setText(const string& str, bool includeFMOSA)
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
            throw EImproperArgument("label @@SPP@@ not found");

        //-----------------------------------------------------------------------
        //SEGREGATE THE PP AND CONVERT IT TO STRUCTURE:

        //discard the coments and empty lines
        while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i].str).Length()<=0))
            i++;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@SPP@@ not found");

        //check if actual line contains the label @@SPP@@
        if(StrTrim(Strings[i]) != AnsiString("@@SPP@@"))
            throw EImproperArgument("label @@SPP@@ not found");

        //discard the label
        i++;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("should to have a positioning program");

        //segregate all lines until the close label @@EPP@
        string str = Strings[i++].str;
        while(i<Strings.getCount() && StrTrim(Strings[i])!=AnsiString("@@EPP@@")) {
            //check if there are more lines
            if(i >= Strings.getCount())
                throw EImproperArgument("label @@EPP@@ not found");

            //add the next line and increase the index
            str += "\r\n";
            str += Strings[i++].str;
        }

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@EPP@@ not found");

        //check if actual line contains the label @@EPP@@
        if(StrTrim(Strings[i]) != AnsiString("@@EPP@@"))
            throw EImproperArgument("label @@EPP@@ not found");

        //discard the label
        i++;

        //convert to structure
        TMotionProgram t_PP;
        string labelPP;
        unsigned int BidPP;
        t_PP.setInterfaceText(labelPP, BidPP, str);

        //-----------------------------------------------------------------------
        //SEGREGATE THE DP AND CONVERT IT TO STRUCTURE:

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("should to have a depositioning program");

        //discard the coments and empty lines
        while(i<Strings.getCount() && (strFirstNonseparatorChar(Strings[i].str)=='#' || StrTrim(Strings[i].str).Length()<=0))
            i++;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@SDP@@ not found");

        //check if actual line contains the label @@SDP@@
        if(StrTrim(Strings[i]) != AnsiString("@@SDP@@"))
            throw EImproperArgument("label @@SDP@@ not found");

        //discard the label
        i++;

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("should to have a depositioning program");

        //segregate all lines until the close label @@EDP@
        str = Strings[i++].str;
        while(i<Strings.getCount() && StrTrim(Strings[i])!=AnsiString("@@EDP@@")) {
            //check if there are more lines
            if(i >= Strings.getCount())
                throw EImproperArgument("label @@EDP@@ not found");

            //add the next line and increase the index
            str += "\r\n";
            str += Strings[i++].str;
        }

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@EDP@@ not found");

        //check if actual line contains the label @@EDP@@
        if(StrTrim(Strings[i]) != AnsiString("@@EDP@@"))
            throw EImproperArgument("label @@EDP@@ not found");

        //discard the label
        i++;

        //convert to structure
        TMotionProgram t_DP;
        string labelDP;
        unsigned int BidDP;
        t_DP.setInterfaceText(labelDP, BidDP, str);

        if(includeFMOSA) {
            //-----------------------------------------------------------------------
            //SEGREGATE THE FMOSA AND CONVERT IT TO STRUCTURE:

            //check if there are more lines
            if(i >= Strings.getCount())
                throw EImproperArgument("should to have a FMOSA");

            //segregate all lines until the end
            str = Strings[i++].str;
            while(i < Strings.getCount()) {
                //add the next line and increase the index
                str += "\r\n";
                str += Strings[i++].str;
            }

            TFMOSA t_FMOSA;
            unsigned int BidOB;
            t_FMOSA.setTableText(BidOB, str);

            //-----------------------------------------------------------------------
            //CHECK THE PRECONDITIONS:

            if(BidPP != BidOB)
                throw EImproperFileLoadedValue("the CB Id of the PP should be equal to the CB Id of the OB");

            if(strTrim(labelPP) != "pos")
                throw EImproperFileLoadedValue("the PP label should be \"pos\"");

            if(BidDP != BidOB)
                throw EImproperFileLoadedValue("the CB Id of the DP should be equal to the CB Id o fthe OB");

            if(strTrim(labelDP) != "depos")
                throw EImproperFileLoadedValue("the DP label should be \"depos\"");

            //-----------------------------------------------------------------------
            //SET THE TAMPONS VARIABLES:

            PP = t_PP;
            DP = t_DP;
            FMOSA = t_FMOSA;

            Bid = BidOB;
        }
        else {
            //-----------------------------------------------------------------------
            //CHECK THE PRECONDITIONS:

            if(strTrim(labelPP) != "pos")
                throw EImproperFileLoadedValue("the PP label should be \"pos\"");

            if(BidDP != BidPP)
                throw EImproperFileLoadedValue("the CB Id of the DP should be equal to the CB Id of the PP");

            if(strTrim(labelDP) != "depos")
                throw EImproperFileLoadedValue("the DP label should be \"depos\"");

            //-----------------------------------------------------------------------
            //SET THE TAMPONS VARIABLES:

            PP = t_PP;
            DP = t_DP;
            FMOSA.Clear();

            Bid = BidPP;

        }
    } catch(Exception& E) {
        E.Message.Insert(1, "setting text in Outputs structure: ");
        throw;
    } catch(...) {
        throw;
    }
}

//build an estructure of class OutputsPairPPDP
OutputsPairPPDP::OutputsPairPPDP() :
    //properties for built comments about file outputs
    FMOSA_filename(""),
    FMPT_version(""),
    datetime(""),
    EnabledNotOperative(),
    collided_str(""),
    Collided(),
    Obstructed(),
    //properties for built comments about each MP
    PPvalid(false), DPvalid(false), DsecMax(0),
    //properties for built the executable data
    Bid(0), IPL(), PP(), OPL(), DP(), FMOSA()
{
}

//reset all properties to values by default
void OutputsPairPPDP::Clear(void)
{
    FMOSA_filename = "";
    FMPT_version = "";
    datetime = "";
    EnabledNotOperative.Clear();
    collided_str = "";
    Collided.Clear();
    Obstructed.Clear();
    PPvalid = false;
    DPvalid = false;
    DsecMax = 0;
    Bid = 0;
    IPL.Clear();
    PP.Clear();
    OPL.Clear();
    DP.Clear();
    FMOSA.Clear();
}

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
