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
//File: Outputs.cpp
//Content: structure of data for contains all outputs data
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Outputs.h"
#include "Strings.h"

//---------------------------------------------------------------------------

using namespace Strings;

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//class Outputs:
//---------------------------------------------------------------------------

//get the outputs in text format
void Outputs::getText(string& str, const int Bid,
                      const TPairPositionAnglesList& OPL,
                      const TPairPositionAnglesList& IPL) const
{
    string aux;

    PP.getInterfaceText(aux, "pos", Bid, IPL);
    str = "#Positioning program";
    str += "\r\n@@SPP@@";
    str += "\r\n"+aux;
    str += "\r\n@@EPP@@";

    DP.getInterfaceText(aux, "depos", Bid, OPL);
    str += "\r\n#Depositioning program";
    str += "\r\n@@SDP@@";
    str += "\r\n"+aux;
    str += "\r\n@@EDP@@";

    //str += "#RP lists";
    //str += "\r\n@@SRPL@@";
    //str += "\r\nCollided: "+Collided.getText().str;
    //str += "\r\nObstructed: "+Obstructed.getText().str;
    //str += "\r\n@@ERPL@@";

    FMOSAT.getTableText(aux);
    str += "\r\n"+aux;
}

//get the outputs in text format
//with rotor 2 negative
void Outputs::getNegativeText(string& str, const int Bid,
                      const TPairPositionAnglesList& OPL,
                      const TPairPositionAnglesList& IPL) const
{
    string aux;

    PP.getInterfaceNegativeText(aux, "pos", Bid, IPL);
    str = "#Positioning program";
    str += "\r\n@@SPP@@";
    str += "\r\n"+aux;
    str += "\r\n@@EPP@@";

    DP.getInterfaceNegativeText(aux, "depos", Bid, OPL);
    str += "\r\n#Depositioning program";
    str += "\r\n@@SDP@@";
    str += "\r\n"+aux;
    str += "\r\n@@EDP@@";

    //str += "#RP lists";
    //str += "\r\n@@SRPL@@";
    //str += "\r\nCollided: "+Collided.getText().str;
    //str += "\r\nObstructed: "+Obstructed.getText().str;
    //str += "\r\n@@ERPL@@";

    FMOSAT.getTableText(aux);
    str += "\r\n"+aux;
}

//get the outputs in text format
void Outputs::setText(unsigned int& Bid, const string& str)
{
    try {
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
        AnsiString S = Strings[i++];
        while(i<Strings.getCount() && StrTrim(Strings[i])!=AnsiString("@@EPP@@")) {
            //check if there are more lines
            if(i >= Strings.getCount())
                throw EImproperArgument("label @@EPP@@ not found");

            //add the next line and increase the index
            S += "\r\n";
            S += Strings[i++];
        }

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@EPP@@ not found");

        //check if actual line contains the label @@EPP@@
        if(StrTrim(Strings[i]) != AnsiString("@@EPP@@"))
            throw EImproperArgument("label @@EPP@@ not found");

        //convert to structure
        TMotionProgram _PP;
        string labelPP;
        unsigned int BidPP;
        _PP.setInterfaceText(labelPP, BidPP, S.str);

        //discard the label
        i++;

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
        S = Strings[i++];
        while(i<Strings.getCount() && StrTrim(Strings[i])!=AnsiString("@@EDP@@")) {
            //check if there are more lines
            if(i >= Strings.getCount())
                throw EImproperArgument("label @@EDP@@ not found");

            //add the next line and increase the index
            S += "\r\n";
            S += Strings[i++];
        }

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("label @@EDP@@ not found");

        //check if actual line contains the label @@EDP@@
        if(StrTrim(Strings[i]) != AnsiString("@@EDP@@"))
            throw EImproperArgument("label @@EDP@@ not found");

        //convert to structure
        TMotionProgram _DP;
        string labelDP;
        unsigned int BidDP;
        _DP.setInterfaceText(labelDP, BidDP, S.str);

        //discard the label
        i++;

        //-----------------------------------------------------------------------
        //SEGREGATE THE FMOSAT AND CONVERT IT TO STRUCTURE:

        //check if there are more lines
        if(i >= Strings.getCount())
            throw EImproperArgument("should to have a FMOSA table");

        //segregate all lines until the end
        S = Strings[i++];
        while(i < Strings.getCount()) {
            //add the next line and increase the index
            S += "\r\n";
            S += Strings[i++];
        }

        TFMOSATable _FMOSAT;
        unsigned int BidOB;
        _FMOSAT.setTableText(BidOB, S.str);

        //-----------------------------------------------------------------------
        //CHECK THE PRECONDITIONS:

        if(BidPP != BidOB)
            throw EImproperFileLoadedValue("the CB Id of the PP should be equal to the CB Id ofthe OB");

        if(strTrim(labelPP) != "pos")
            throw EImproperFileLoadedValue("the PP label should be \"pos\"");

        if(BidDP != BidOB)
            throw EImproperFileLoadedValue("the CB Id of the DP should be equal to the CB Id ofthe OB");

        if(strTrim(labelDP) != "depos")
            throw EImproperFileLoadedValue("the DP label should be \"depos\"");

        //-----------------------------------------------------------------------
        //SET THE TAMPONS VARIABLES:

        PP = _PP;
        DP = _DP;
        FMOSAT = _FMOSAT;

        Bid = BidOB;

    } catch(Exception& E) {
        E.Message.Insert(1, "setting text in Outputs structure: ");
        throw;
    } catch(...) {
        throw;
    }
}

//build an estructure of class Outputs
Outputs::Outputs() : PP(), DP(), Collided(), Obstructed(), FMOSAT()
{
}

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
