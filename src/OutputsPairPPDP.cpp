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
/// @file OutputsPairPPDP.cpp
/// @brief structure for contains all outputs data about a generated pair (PP, DP)
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#include "OutputsPairPPDP.h"
#include "Strings.h"

#include <uuid/uuid.h>

//For install:
//  sudo apt-get install uuid-dev

//---------------------------------------------------------------------------

using namespace Strings;

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//class OutputsPairPPDP:
//---------------------------------------------------------------------------

//PRIVATE:

//functions for add data to a JSON object
void OutputsPairPPDP::addUUID(Json::Value &object) const
{
    //get the uuid
    uuid_t uuid;
    uuid_generate(uuid);
    //unparse the uuid to string
    char uuid_c_str[37];
    uuid_unparse(uuid, uuid_c_str);
    object["uuid"] = string(uuid_c_str);
}
Json::Value OutputsPairPPDP::getComments(void) const
{
    //build a json array
    Json::Value object;

    string str = "# Pair (PP, DP) outputs generated ";
    if(FMOSA_filename.length() > 0)
        str += "from FMOSA " + FMOSA_filename;
    else
        str += "online (not from FMOSA)";
    object.append(str);

    str = "# Generated with FMPT version "+FMPT_version+" and FMM Instance version "+Instance_version;
    object.append(str);
    str = "# Date of generation: "+datetime;
    object.append(str);

    if(EnabledNotOperative.getCount() > 0) {
        str = "# WARNING! This pair (PP, DP) has been generated with enabled-not-operative RPs:";
        object.append(str);
        str = "# Enabled-not-operative RP ids: " + EnabledNotOperative.getText().str;
        object.append(str);

        //get the list of dangerous RPs
        TRoboticPositionerList Dangerous;
        EnabledNotOperative.getDangerous(Dangerous);

        str = "# Dangerous RPs are enabled-not-operative RPs with fault type dyynamic or unknowledge";
        object.append(str);
        if(Dangerous.getCount() > 0) {
            str = "# ERROR! This pair (PP, DP) has been generated with dangerous RPs:";
            object.append(str);
            str = "# Dangerous RP ids: " + Dangerous.getText().str;
            object.append(str);
        }
        else {
            str = "# This pair (PP, DP) has been generated without dangerous RPs.";
            object.append(str);
        }
    }
    else {
        str = "# This pair (PP, DP) has been generated without enabled-not-operative RPs.";
        object.append(str);
    }

    if(Collided.getCount() > 0 || Obstructed.getCount() > 0) {
        if(Collided.getCount() > 0) {
            str = "# WARNING! the FMOSA contains collided items (either EAs or RPs):";
            object.append(str);
            str = "# Collided items: " + collided_str;
            object.append(str);
            str = "# Collided RP ids: " + Collided.getText().str;
            object.append(str);
        }
        if(Obstructed.getCount() > 0) {
            str = "# WARNING! the FMOSA contains obstructed RPs:";
            object.append(str);
            str = "# Obstructed RP ids: " + Obstructed.getText().str;
            object.append(str);
        }
        str = "# The initial positions of the collided or obstructed RPs match with their observing positions.";
        object.append(str);
    }
    else {
        str = "# The pair (PP, DP) has been generated without neither collided nor obstructed RPs.";
        object.append(str);
    }

    str = "# A pair (PP, DP) is suitable to be executed when it is valid (avoid collisions) and there aren't";
    object.append(str);
    str = "# dangerous RPs in the Fiber MOS (enabled-not-operative RPs with fault type dynamic or unknowledge),";
    object.append(str);
    str = "# and there aren't neither collided nor obstructed RPs.";
    object.append(str);
    if(suitable()) {
        str = "# According the actual status of the FMPT, this pair (PP, DP) is suitable to be executed.";
        object.append(str);
    }
    else {
        str = "# According the actual status of the FMPT, this pair (PP, DP) is not suitable to be executed.";
        object.append(str);
    }

    return object;
}
Json::Value OutputsPairPPDP::getPos(void) const
{
    //build a json object
    Json::Value object;

    //add comment about name
    Json::Value comments;
    comments.append("# PP (Positioning Program)");

    //add comment about validity
    string str;
    if(PPvalid) {
        str = "# This PP avoids collisions when it is executed starting from the initial positions (check out IPL).";
    } else {
        str = "# ERROR! This PP produces a collision when it is executed starting from the initial positions (check out IPL).";
    }
    comments.append(str);

    //add comment about approximations
    if(PP.thereIsSomeCommentDsec()) {
        str = "# WARNING! This PP contains instructions that produce radial movements which produce approximations closer than "+floattostr(DsecMax)+" mm:";
        comments.append(str);
        str = PP.getCommentsDsecMCStext();
        comments.append(str);
    } else {
        str = "# All radial movements of this PP keep a security distance upper or equal to "+floattostr(DsecMax)+" mm.";
        comments.append(str);
    }

    //  //add the coments
    //  object["comments"] = comments;
    //  //add the Bid
    //  object["Bid"] = Bid;

    //add the PPvalid
    object["valid"] = PPvalid;

    //add the PP
    object["program"] = PP.getJSON(IPL);

    return object;
}
Json::Value OutputsPairPPDP::getDepos(void) const
{
    //build a json object
    Json::Value object;

    //add comment about name
    Json::Value comments;
    comments.append("# DP (Depositioning Program)");

    //add comment about validity
    string str;
    if(DPvalid) {
        str = "# This DP avoids collisions when it is executed starting from the observing positions (check out OPL).";
    } else {
        str = "# ERROR! This DP produces a collision when it is executed starting from the observing positions (check out OPL).";
    }
    comments.append(str);

    //add comment about approximations
    if(DP.thereIsSomeCommentDsec()) {
        str = "# WARNING! This DP contains instructions that produce radial movements which produce approximations closer than "+floattostr(DsecMax)+" mm:";
        comments.append(str);
        str = DP.getCommentsDsecMCStext();
        comments.append(str);
    } else {
        str = "# All radial movements of this DP keep a security distance upper or equal to "+floattostr(DsecMax)+" mm.";
        comments.append(str);
    }

    //  //add the coments
    //  object["comments"] = comments;
    //  //add the Bid
    //  object["Bid"] = Bid;

    //add the DPvalid
    object["valid"] = DPvalid;

    //add the DP
    object["program"] = DP.getJSON(OPL);

    return object;
}

//PUBLIC:

//get the warning for not suitable outputs
string OutputsPairPPDP::getWarningNotSuitable(void) const
{
    string str = "WARNING! This file outputs has been saved by force, because:";
    str += " has been generated with dangerous RPs in the FMM Instance,";
    str += " or has been generated with collided RPs in the FMOSA,";
    str += " or has been generated with obstructed RPs in the FMOSA,";
    str += " or the generated pair (PP, DP) is not valid.";
    str += " Dangerous RPs are enabled-not-operative RPs with fault type dynamic or unknowledge.";
    str += " A pair (PP, DP) is not valid when has been detected a collision during the validation process.";

    //get the list of Dangerous RPs
    TRoboticPositionerList Dangerous;
    EnabledNotOperative.getDangerous(Dangerous);

    if(Dangerous.getCount() > 0)
        str += " This pair (PP, DP) has been generated with dangerous RPs.";
    else
        str += " This pair (PP, DP) has been generated without dangerous RPs.";

    if(PPvalid && DPvalid)
        str += " According the FMPT, the pair (PP, DP) included in this file is valid, but you must disable all dangerous RPs before generate the pair (PP, DP), and delete allocations to all collided and obstructed RPs in the file type FMOSA.";
    else
        str += " According the FMPT, pair (P, DP) included in this file is not valid.";

    str += " This text has been added uncommented for difficult the execution of the parking program contained in this file.";

    return str;
}

//get the comments about outputs
string OutputsPairPPDP::getCommentsText(void) const
{
    locale l;
    if(l.name() != "C")
        throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

    string str = "# Pair (PP, DP) outputs generated ";
    if(FMOSA_filename.length() > 0)
        str += "from FMOSA " + FMOSA_filename;
    else
        str += "online (not from FMOSA)";

    str += "\r\n# Generated with FMPT version "+FMPT_version+" and FMM Instance version "+Instance_version;
    str += "\r\n# Date of generation: "+datetime;

    if(EnabledNotOperative.getCount() > 0) {
        str += "\r\n# WARNING! This pair (PP, DP) has been generated with enabled-not-operative RPs:";
        str += "\r\n# Enabled-not-operative RP ids: " + EnabledNotOperative.getText().str;

        //get the list of dangerous RPs
        TRoboticPositionerList Dangerous;
        EnabledNotOperative.getDangerous(Dangerous);

        str += "\r\n# Dangerous RPs are enabled-not-operative RPs with fault type dyynamic or unknowledge";
        if(Dangerous.getCount() > 0) {
            str += "\r\n# ERROR! This pair (PP, DP) has been generated with dangerous RPs:";
            str += "\r\n# Dangerous RP ids: " + Dangerous.getText().str;
        }
        else {
            str += "\r\n# This pair (PP, DP) has been generated without dangerous RPs.";
        }
    }
    else {
        str += "\r\n# This pair (PP, DP) has been generated without enabled-not-operative RPs.";
    }

    if(Collided.getCount() > 0 || Obstructed.getCount() > 0) {
        if(Collided.getCount() > 0) {
            str += "\r\n# WARNING! the FMOSA contains collided items (either EAs or RPs):";
            str += "\r\n# Collided items: " + collided_str;
            str += "\r\n# Collided RP ids: " + Collided.getText().str;
        }
        if(Obstructed.getCount() > 0) {
            str += "\r\n# WARNING! the FMOSA contains obstructed RPs:";
            str += "\r\n# Obstructed RP ids: " + Obstructed.getText().str;
        }
        str += "\r\n# The initial positions of the collided or obstructed RPs match with their observing positions.";
    }
    else {
        str += "\r\n# The pair (PP, DP) has been generated without neither collided nor obstructed RPs.";
    }

    str += "\r\n# A pair (PP, DP) is suitable to be executed when it is valid (avoid collisions) and there aren't";
    str += "\r\n# dangerous RPs in the Fiber MOS (enabled-not-operative RPs with fault type dynamic or unknowledge),";
    str += "\r\n# and there aren't neither collided nor obstructed RPs.";
    if(suitable()) {
        str += "\r\n# According the actual status of the FMPT, this pair (PP, DP) is suitable to be executed.";
    }
    else {
        str += "\r\n# According the actual status of the FMPT, this pair (PP, DP) is not suitable to be executed.";
    }

    return str;
}

//get outputs in format MEG with:
//  comments
//  the pair (PP, DP)
//  the FMOSA
void OutputsPairPPDP::getMEGtext(string& str, bool includeFMOSA) const

{
    locale l;
    if(l.name() != "C")
        throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

    if(suitable())
        str = "";
    else {
        str = getWarningNotSuitable();
        str += "\r\n\r\n";
    }

    //print the coments about outputs
    str += getCommentsText();

    str += "\r\n";
    str += "\r\n# Positioning program";
    if(PPvalid) {
        str += "\r\n# This PP avoids collisions when it is executed starting from the initial positions (check out IPL).";
    } else {
        str += "\r\n# ERROR! This PP produces a collision when it is executed starting from the initial positions (check out IPL).";
    }
    if(PP.thereIsSomeCommentDsec()) {
        str += "\r\n# WARNING! This PP contains instructions that produce radial movements which produce approximations closer than "+floattostr(DsecMax)+" mm:";
        str += "\r\n"+PP.getCommentsDsecMCStext();
    } else {
        str += "\r\n# All radial movements of this PP keep a security distance upper or equal to "+floattostr(DsecMax)+" mm.";
    }
    str += "\r\n@@SPP@@";
    string aux;
    PP.getMCStext(aux, "pos", Bid, IPL);
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
        str += "\r\n"+DP.getCommentsDsecMCStext();
    } else {
        str += "\r\n# All radial movements of this DP keep a security distance upper or equal to "+floattostr(DsecMax)+" mm.";
    }
    str += "\r\n@@SDP@@";
    DP.getMCStext(aux, "depos", Bid, OPL);
    str += "\r\n"+aux;
    str += "\r\n@@EDP@@";

    if(includeFMOSA) {
        str += "\r\n";
        FMOSA.getTableText(aux);
        str += "\r\n"+aux;
    }
}

//get outputs in format JSON with:
//  comments
//  the pair (PP, DP)
//  the FMOSA
string OutputsPairPPDP::getJSONtext(bool includeFMOSA) const
{
    locale l;
    if(l.name() != "C")
        throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

    //build the JSON object and parse the schema
    //  json_object *root;
    //  root = json_tokener_parse(schema.c_str());

    //check the result of the parsing
    //  if(root == NULL)
    //      throw EImproperFileLoadedValue("failed so parse JSON");

    //set value for token "instrument"
    //  json_object *val;
    //  //val = json_object_object_get(root, "instrument");
    //  val = json_object_array_get_idx(root, 0);
    //  if(val == NULL || json_object_get_type(val) != json_type_string)
    //      throw EImpossibleError("lateral effect");
    //  json_object_object_add(val, "instrument", json_object_new_string("MEGARA"));
    //  json_object_array_put_idx(root, 0, val);

    //--------------------

    //build a json object
    Json::Value root;

    //add general section
    root["instrument"] = "MEGARA";
    root["title"] = FMOSA.getTitle();
    root["description"] = FMOSA.getDescription();
    addUUID(root);
    root["@schema"] = "http://guaix.fis.ucm.es/megara/robot-schema.json";
    root["@version"] = 1;

    //------------------------
    //ADD ROBOT SECTION:

    //build a json object for robot
    Json::Value robot;

    //  //add the coments
    //  robot["comments"] = getComments();

    robot["fmpt_version"] = FMPT_version;
    robot["fmpt_instance_version"] = Instance_version;
    robot["creation_date"] = datetime;
    robot["fmat_filename"] = FMOSA_filename;

    //build a json object for secuences
    Json::Value sequences;

    sequences["type"] = "PPDP";
    sequences["valid"] = PPvalid && DPvalid;

    //if the EA1 is collided, add 0 to rps_collided
    Json::Value rps_collided(Json::arrayValue);
    int i = collided_str.find("EA1", 0);
    if(i < string::npos)
        rps_collided.append(Json::Value(0));

    //add the identifiers of the collided RPs to rps_collided
    for(int i=0; i<Collided.getCount(); i++)
        rps_collided.append(Json::Value(Collided[i]->getActuator()->getId()));
    sequences["rps_collided"] = rps_collided;

    //add the identifiers of the obstructed RPs to rps_obstructed
    Json::Value rps_obstructed(Json::arrayValue);
    for(int i=0; i<Obstructed.getCount(); i++)
        rps_obstructed.append(Json::Value(Obstructed[i]->getActuator()->getId()));
    sequences["rps_obstructed"] = rps_obstructed;

    sequences["positioning"] = getPos();
    sequences["depositioning"] = getDepos();

    robot["sequences"] = sequences;
    root["robot"] = robot;

    //In sustitution of:
    //  //add the coments
    //  root["comments"] = getComments();
    //  //add the positioning program with comments
    //  root["depos"] = getPos();
    //  //add the depositioning program with comments
    //  root["depos"] = getDepos();

    //------------------------
    //ADD ASSIGNATION SECTION:

    //add the (assignations) if any
    if(includeFMOSA)
        root["assignments"] = FMOSA.getJSON();

    //------------------------

    string str;
    if(suitable())
        str = "";
    else {
        str = getWarningNotSuitable();
        str += "\r\n\r\n";
    }
    Json::StyledWriter writer;
    str += writer.write(root);
    return str;
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
    str = getCommentsText();

    str += "\r\n";
    str += "\r\n# Positioning program";
    if(PPvalid) {
        str += "\r\n# This PP avoids collisions when it is executed starting from the initial positions.";
    } else {
        str += "\r\n# ERROR! This PP produces a collision when it is executed starting from the initial positions.";
    }
    if(PP.thereIsSomeCommentDsec()) {
        str += "\r\n# WARNING! This PP contains instructions that produce radial movements which produce approximations closer than "+floattostr(DsecMax)+" mm:";
        str += "\r\n"+PP.getCommentsDsecMCStext();
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
        str += "\r\n"+DP.getCommentsDsecMCStext();
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
        t_PP.setMCStext(labelPP, BidPP, str);

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
        t_DP.setMCStext(labelDP, BidDP, str);

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
    Instance_version(""),
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
    Instance_version = "";
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

//determine if the outputs is suitable to be executed:
//  the pair (PP, DP) is valid
//  and there aren't dangerous RPs
//  and there aren't collided RPs
//  and there aren't obstructed RPs
bool OutputsPairPPDP::suitable(void) const
{
    if(PPvalid != true || DPvalid != true)
        return false;

    if(EnabledNotOperative.searchFaultDynOrUnk() < EnabledNotOperative.getCount())
        return false;

    if(Collided.getCount() > 0 || Obstructed.getCount() > 0)
        return false;
    return true;
}

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
