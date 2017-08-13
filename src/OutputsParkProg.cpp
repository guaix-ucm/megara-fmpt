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
/// @file OutputsParkProg.cpp
/// @brief structure for contains all outputs data about a generated parking program ParkProg
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#include "OutputsParkProg.h"
#include "Strings.h"

#include <uuid/uuid.h>
//#include <json/json_object.h>
//#include <json/json_tokener.h>

//For install in Ubuntu/Debian:
//  sudo apt-get install uuid-dev
//  sudo apt-get install libjson0-dev

//---------------------------------------------------------------------------

using namespace Strings;

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//class ParkProg:
//---------------------------------------------------------------------------

//PRIVATE:

//functions for add data to a JSON object
void OutputsParkProg::addUUID(Json::Value &object) const
{
    //get the uuid
    uuid_t uuid;
    uuid_generate(uuid);
    //unparse the uuid to string
    char uuid_c_str[37];
    uuid_unparse(uuid, uuid_c_str);
    object["uuid"] = string(uuid_c_str);
}
Json::Value OutputsParkProg::getComments(void) const
{
    //build a json array
    Json::Value object;

    string str = "# Parkig program outputs generated ";
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
        str = "# WARNING! This parking program has been generated with enabled-not-operative RPs:";
        object.append(str);
        str = "# Enabled-not-operative RP ids: " + EnabledNotOperative.getText().str;
        object.append(str);

        //get the list of dangerous RPs
        TRoboticPositionerList Dangerous;
        EnabledNotOperative.getDangerous(Dangerous);

        str = "# Dangerous RPs are enabled-not-operative RPs with fault type dyynamic or unknowledge";
        object.append(str);
        if(Dangerous.getCount() > 0) {
            str = "# ERROR! This parking program has been generated with dangerous RPs:";
            object.append(str);
            str = "# Dangerous RP ids: " + Dangerous.getText().str;
            object.append(str);
        }
        else {
            str = "# This parking program has been generated without dangerous RPs.";
            object.append(str);
        }
    }
    else {
        str = "# This parking program has been generated without enabled-not-operative RPs.";
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
        str = "# The final positions of the collided or obstructed RPs match with their starting positions.";
        object.append(str);
    }
    else {
        str = "# The parking program has been generated without neither collided nor obstructed RPs.";
        object.append(str);
    }

    str = "# A parking program is suitable to be executed when it is valid (avoid collisions) and there aren't";
    object.append(str);
    str = "# dangerous RPs in the Fiber MOS (enabled-not-operative RPs with fault type dynamic or unknowledge).";
    object.append(str);
    if(suitable()) {
        str = "# According the actual status of the FMPT, this parking program is suitable to be executed.";
        object.append(str);
    }
    else {
        str = "# According the actual status of the FMPT, this parking program is not suitable to be executed.";
        object.append(str);
    }

    return object;
}
Json::Value OutputsParkProg::getParking(void) const
{
    //build a json object
    Json::Value object;

    //add comment about name
    Json::Value comments;
    comments.append("# ParkProg (Parking Program)");

    //add comment about validity
    string str;
    if(ParkProgValid) {
        str = "# This parking program avoids collisions when it is executed starting from the starting positions (check out SPL).";
    } else {
        str = "# ERROR! This parking program produces a collision when it is executed starting from the starting positions (check out SPL).";
    }
    comments.append(str);

    //add comment about approximations
    if(ParkProg.thereIsSomeCommentDsec()) {
        str = "# WARNING! This parking program contains instructions that produce radial movements which produce approximations closer than "+floattostr(DsecMax)+" mm:";
        comments.append(str);
        str = ParkProg.getCommentsDsecMCStext();
        comments.append(str);
    } else {
        str = "# All radial movements of this parking program keep a security distance upper or equal to "+floattostr(DsecMax)+" mm.";
        comments.append(str);
    }

    //  //add the coments
    //  object["comments"] = comments;
    //  //add the Bid
    //  object["Bid"] = Bid;

    //add the parking program
    object["program"] = ParkProg.getJSON(SPL);

    return object;
}

//PUBLIC:

//get the warning for not suitable outputs
string OutputsParkProg::getWarningNotSuitable(void) const
{
    string str = "WARNING! This file outputs has been saved by force, because either:";
    str += " has been generated with dangerous RPs in the FMM Instance,";
    str += " or the generated parking program is not valid.";
    str += " Dangerous RPs are enabled-not-operative RPs with fault type dynamic or unknowledge.";
    str += " A parking program is not valid when has been detected a collision during the validation process.";

    //get the list of Dangerous RPs
    TRoboticPositionerList Dangerous;
    EnabledNotOperative.getDangerous(Dangerous);

    if(Dangerous.getCount() > 0)
        str += " This parking program has been generated with dangerous RPs.";
    else
        str += " This parking program has been generated without dangerous RPs.";

    if(ParkProgValid)
        str += " According the FMPT, the parking program included in this file is valid, but you must disable all dangerous RPs before generate the parking program.";
    else
        str += " According the FMPT, parking program included in this file is not valid.";

    str += " This text has been added uncommented for difficult the execution of the parking program contained in this file.";

    return str;
}

//get the comments about outputs in text format
string OutputsParkProg::getCommentsText(void) const
{
    locale l;
    if(l.name() != "C")
        throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

    string str = "# Parkig program outputs generated ";
    if(FMOSA_filename.length() > 0)
        str += "from FMOSA " + FMOSA_filename;
    else
        str += "online (not from FMOSA)";
    str += "\r\n# Generated with FMPT version "+FMPT_version+" and FMM Instance version "+Instance_version;
    str += "\r\n# Date of generation: "+datetime;

    if(EnabledNotOperative.getCount() > 0) {
        str += "\r\n# WARNING! This parking program has been generated with enabled-not-operative RPs:";
        str += "\r\n# Enabled-not-operative RP ids: " + EnabledNotOperative.getText().str;

        //get the list of dangerous RPs
        TRoboticPositionerList Dangerous;
        EnabledNotOperative.getDangerous(Dangerous);

        str += "\r\n# Dangerous RPs are enabled-not-operative RPs with fault type dyynamic or unknowledge";
        if(Dangerous.getCount() > 0) {
            str += "\r\n# ERROR! This parking program has been generated with dangerous RPs:";
            str += "\r\n# Dangerous RP ids: " + Dangerous.getText().str;
        }
        else {
            str += "\r\n# This parking program has been generated without dangerous RPs.";
        }
    }
    else {
        str += "\r\n# This parking program has been generated without enabled-not-operative RPs.";
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
        str += "\r\n# The final positions of the collided or obstructed RPs match with their starting positions.";
    }
    else {
        str += "\r\n# The parking program has been generated without neither collided nor obstructed RPs.";
    }

    str += "\r\n# A parking program is suitable to be executed when it is valid (avoid collisions) and there aren't";
    str += "\r\n# dangerous RPs in the Fiber MOS (enabled-not-operative RPs with fault type dynamic or unknowledge).";
    if(suitable()) {
        str += "\r\n# According the actual status of the FMPT, this parking program is suitable to be executed.";
    }
    else {
        str += "\r\n# According the actual status of the FMPT, this parking program is not suitable to be executed.";
    }

    return str;
}

//get outputs in format MEG with:
//  comments
//  the parking program
void OutputsParkProg::getMEGtext(string& str) const
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

    //print the coments about ParkProg
    str += getCommentsText();

    str += "\r\n";
    str += "\r\n# Parking program";
    if(ParkProgValid) {
        str += "\r\n# This parking program avoids collisions when it is executed starting from the starting positions (check out SPL).";
    } else {
        str += "\r\n# ERROR! This parking program produces a collision when it is executed starting from the starting positions (check out SPL).";
    }
    if(ParkProg.thereIsSomeCommentDsec()) {
        str += "\r\n# WARNING! This parking program contains instructions that produce radial movements which produce approximations closer than "+floattostr(DsecMax)+" mm:";
        str += "\r\n"+ParkProg.getCommentsDsecMCStext();
    } else {
        str += "\r\n# All radial movements of this parking program keep a security distance upper or equal to "+floattostr(DsecMax)+" mm.";
    }
    str += "\r\n@@SParkProg@@";
    string aux;
    ParkProg.getMCStext(aux, "depos", Bid, SPL);
    str += "\r\n"+aux;
    str += "\r\n@@EParkProg@@";
}

//get outputs in format JSON with:
//  comments
//  the parking program
string OutputsParkProg::getJSONtext(void) const
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
    root["title"] = "IC1613_s36s64s81";
    root["description"] = "Parking Program in format JSON.";
    addUUID(root);
    root["@schema"] = "http://guaix.fis.ucm.es/megara/robot-schema.json";

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

    sequences["type"] = "DP";
    sequences["valid"] = ParkProgValid;

    Json::Value rps_collided(Json::arrayValue);
    for(int i=0; i<Collided.getCount(); i++)
        rps_collided.append(Json::Value(Collided[i]->getActuator()->getId()));
    sequences["rps_collided"] = rps_collided;

    Json::Value rps_obstructed(Json::arrayValue);
    for(int i=0; i<Obstructed.getCount(); i++)
        rps_obstructed.append(Json::Value(Obstructed[i]->getActuator()->getId()));
    sequences["rps_obstructed"] = rps_obstructed;

    sequences["depositioning"] = getParking();

    robot["sequences"] = sequences;
    root["robot"] = robot;

    //In sustitution of:
    //  //add the coments
    //  root["comments"] = getComments();
    //  //add the parking program with comments
    //  root["depos"] = getParking();

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
void OutputsParkProg::getOtherText(string& str) const
{
    locale l;
    if(l.name() != "C")
        throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

    //print the coments about ParkProg
    str = getCommentsText();

    str += "\r\n";
    str += "\r\n# Parking program";
    if(ParkProgValid) {
        str += "\r\n# This parking program avoids collisions when it is executed starting from the starting positions.";
    } else {
        str += "\r\n# ERROR! This parking program produces a collision when it is executed starting from the starting positions.";
    }
    if(ParkProg.thereIsSomeCommentDsec()) {
        str += "\r\n# WARNING! This parking program contains instructions that produce radial movements which produce approximations closer than DsecMax:";
        str += "\r\n"+ParkProg.getCommentsDsecMCStext();
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

//set the outputs in text format
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
        t_ParkProg.setMCStext(labelParkProg, BidParkProg, str);

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
    Instance_version(""),
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
    Instance_version = "";
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

//determine if the outputs is suitable to be executed:
//  the motion program is valid
//  and there aren't dangerous RPs
bool OutputsParkProg::suitable(void) const
{
    if(ParkProgValid != true)
        return false;

    if(EnabledNotOperative.searchFaultDynOrUnk() < EnabledNotOperative.getCount())
        return false;

    return true;
}

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
