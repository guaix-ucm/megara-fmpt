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
/// @file main.cpp
/// @brief main program of the FMPT SAA
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

//includes for generic
#include "globalconsts.h"
#include "Strings.h"
#include "TextFile.h"
#include "Geometry.h" //distanceSegmentPoint
#include "TextFile.h"
//includes for calculus
#include "MotionProgramGenerator.h"
#include "FMOSA.h"
#include "PositionerCenter.h"
#include "OutputsPairPPDP.h" //Outputs
#include "OutputsParkProg.h" //ParkProg
#include "FileMethods.h"
#include "roboticpositionertuner.h"

#include <clocale> //setlocale, LC_ALL
#include <iostream> //std::cout, ios::fixed
#include <stdio.h> //getchar
#include <config.h> //PACKAGE_VERSION

//#include "tests/testFileMethods_copia.h"

using namespace Strings;
using namespace Models;
using namespace Positioning;

//---------------------------------------------------------------------------
//GENERAL FUNCTIONS:

//print a text line in the standard output and in the log file
void append(const string& str, const char *log_path)
{
    string str_aux = str;
    str_aux += "\r\n";

    std::cout << str_aux;

    char mode[] = "a";
    TTextFile TF(log_path, mode);
    TF.Print(str_aux.c_str());
    TF.Close();
}

//print a text line in the standard output and in the log file
void append_without_endl(const string& str, const char *log_path)
{
    std::cout << str;
    std::cout.flush();

    char mode[] = "a";
    TTextFile TF(log_path, mode);
    TF.Print(str.c_str());
    TF.Close();
}

//gets a string containing the help
string help(void)
{
    string str;

    str = "$ fmpt_saa help";
    str += "\r\n    Print this help.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa help_generatePairPPDP_offline";
    str += "\r\n    Print the help for function generatePairPPDP_offline.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa help_generateParkProg_offline";
    str += "\r\n    Print the help for function generateParkProg_offline.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa help_others";
    str += "\r\n    Print the help about other commands of the FMPT.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa aboutOf";
    str += "\r\n    Print the legend about of...";
    str += "\r\n";
    str += "\r\n$ fmpt_saa generateParkProg_offline <FMOSA_path>";
    str += "\r\n    Generate a valid parking program offline.";
    str += "\r\n    The generated parking program will be saved, even if there are either collided or obstructed RPs.";
    str += "\r\n    Note that motion programs generated with this function (without the argument outputs),";
    str += "\r\n    is garanteed to be valid, beause they pass a validation process considered unerring.";
    str += "\r\n    <FMOSA_path>: absolute or relative path to the input file type FMOSA.";
    str += "\r\n    This command generates the files:";
    str += "\r\n        ParkProg_outputs_from_<filename>: the parking program in format MCS,";
    str += "\r\n            comments about the collided RPs, if any,";
    str += "\r\n            comments about the obstructed RPs, if any,";
    str += "\r\n            comments about the validity of the generated MP,";
    str += "\r\n            and comments about the RPs whose radial motion is more close to 1 mm.";
    str += "\r\n        Where <filename> is the name of the input file.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa generatePairPPDP_offline <FMOSA_path>";
    str += "\r\n    Generate a valid pair (PP, DP) offline.";
    str += "\r\n    If there are either collided or obstructed RPs, the generated pair (PP, DP) will not be saved.";
    str += "\r\n    Note that motion programs generated with this function (without the argument outputs),";
    str += "\r\n    is garanteed to be valid, beause they pass a validation process considered unerring.";
    str += "\r\n    <FMOSA_path>: absolute or relative path to the input file type FMOSA.";
    str += "\r\n    This command generates the file:";
    str += "\r\n        PairPPDP_outputs_from_<filename>: the pair (PP, DP) in format MCS,";
    str += "\r\n            comments about the collided RPs, if any,";
    str += "\r\n            comments about the obstructed RPs, if any,";
    str += "\r\n            comments about the validity of the generated MPs,";
    str += "\r\n            and comments about the RPs whose radial motion is more close to 1 mm.";
    str += "\r\n            the content of the input file type FMOSA,";
    str += "\r\n        Where <filename> is the name of the input file.";
    str += "\r\n";
    str += "\r\nOnly offline generation function use files type FMOSA.";
    str += "\r\nIn the file type FMOSA:";
    str += "\r\n    The following parameters could be empty: Name, Mag, Pr, Bid and Comment.";
    str += "\r\n    Parameter Bid indicates if the source is allocated or not.";
    str += "\r\n    Parameter Enabled indicates if the RP is enabled or not.";
    str += "\r\n    When Bid is empty:";
    str += "\r\n        parameters Name, Mag, Pr and Comment are usually empty;";
    str += "\r\n        parameter Type is usually UNKNOWN.";
    str += "\r\n    Parameter Enabled shall be in accordance with parameter Disabled of the FMM Instance.";
    str += "\r\n    Otherwise motion program generation will be interupted and message error will be shown.";
    str += "\r\n";
    str += "\r\nIn case of RPs stopped in unsecurity positions which aren't neither the initial position nor the";
    str += "observing positions, the parking program shall be generated using the function generateParkProg_online.";
    str += " The function generateParkProg_offline has been included in the FMPT SAA with the exclusive purpose of";
    str += " familiarize to the user with parking program generation, but it is not part of the protocol of action";
    str += " during megara operations. For further reference of online generation functions, check out FMPT GUI";
    str += " and file: '/usr/local/include/megara-fmpt/MotionProgramGenerator.h' (or similar path).";

    //Note that here not has sense define functions for generate MPs online.

    return str;
}

//gets a string containing the help about function generateParkProg_offline
string help_generateParkProg_offline(void)
{
    string str;

    str = "$ fmpt_saa generateParkProg_offline <FMOSA_path> [all]";
    str += "\r\n    [SPL] [FPL]";
    str += "\r\n    [DispCorners1] [DispCorners2]";
    str += "\r\n    [Dmin] [Dend]";
    str += "\r\n    [FMPT] [MCS]";
    str += "\r\n    [outputs] [other-outputs]";
    str += "\r\n";
    str += "\r\n    Generate a valid parking program offline.";
    str += "\r\n    The generated parking program will be saved, even if there are either collided or obstructed RPs.";
    str += "\r\n    Note that motion programs generated with this function (without the argument outputs),";
    str += "\r\n    is garanteed to be valid, beause they pass a validation process considered unerring.";
    str += "\r\n    <FMOSA_path>: absolute or relative path to the input file type FMOSA.";
    str += "\r\n    This command generates the files:";
    str += "\r\n        ParkProg_outputs_from_<filename>: the parking program in format MCS,";
    str += "\r\n            comments about the collided RPs, if any,";
    str += "\r\n            comments about the obstructed RPs, if any,";
    str += "\r\n            comments about the validity of the generated MP,";
    str += "\r\n            and comments about the RPs whose radial motion is more close to 1 mm.";
    str += "\r\n        Where <filename> is the name of the input file.";
    str += "\r\n";
    str += "\r\n    This function has several optional arguments, each of witch allow save a data in a file,";
    str += "\r\n    whose name is composed as follow:";
    str += "\r\n        \"ParkProg_\" + <argument> + \"_from_\" + <filename>";
    str += "\r\n    Where <filename> is the name of the input file, and <argument> is one of the following strings:";
    str += "\r\n";
    str += "\r\n    all: save all files.";
    str += "\r\n";
    str += "\r\n    SPL: Starting Position List in format PPA (Pair Position Angles in steps).";
    str += "\r\n    FPL: Final Position List in format PPA (Pair Position Angles in steps).";
    str += "\r\n";
    str += "\r\n    DispCorners1: displacement of corners 1 from starting position to final position.";
    str += "\r\n    DispCorners2: displacement of corners 2 from starting position to final position.";
    str += "\r\n    (1 and 2 are upper and lower corners of the arm elbow, when rotors are in the origin).";
    str += "\r\n";
    str += "\r\n    Dmin: minimun distance during executing the Parking Program.";
    str += "\r\n    Dend: distance at the end of the trajectory during executing the Parking Program.";
    str += "\r\n    (The distances are given in mm for each gesture for each RP).";
    str += "\r\n";
    str += "\r\n    FMPT: Parking Program in format FMPT.";
    str += "\r\n    (The format of the FMPT use absolute coordinates and own instruction).";
    str += "\r\n";
    str += "\r\n    MCS: Parking Program in format MCS (without neither comments nor additional information.).";
    str += "\r\n    (ParkProg is saved also in the file ouputs).";
    str += "\r\n";
    str += "\r\n    outputs: force save file outputs in formats MCS and JSON, even if there are dangerous RPs.";
    str += "\r\n    other-outputs: the same comments of the file outputs in the old format other-outputs.";
    str += "\r\n";
    str += "\r\n    WARNING! rotor 2 position angle (r2) has sign minus. r2 is exclusive of formats MCS and JSON:";
    str += "\r\n        Files MCS and outputs, contain (r1, r2).";
    str += "\r\n        Files SPL, FPL and FMPT, contain (p_1, p___3), not (r1, r2).";
    str += "\r\n";
    str += "\r\n    WARNING! Although format JSON is used in files type outputs generated by the functions:";
    str += "\r\n        generateParkProg_offline and generatePairPPDP_offline,";
    str += "\r\n    ad-hoc old format (called format MCS), is requested for other functions which";
    str += "\r\n    require a file tye outputs how input.";
    str += "\r\n    Besides FMAT generates file type FMOSA using their own ad-hoc format (not JSON format).";

    return str;
}

//gets a string containing the help about function generatePairPPDP_offline
string help_generatePairPPDP_offline(void)
{
    string str;

    str = "$ fmpt_saa generatePairPPDP_offline <FMOSA_path> [all]";
    str += "\r\n    [OPL] [OPL-S0] [OPL-S1] [IPL]";
    str += "\r\n    [DispCorners1] [DispCorners2]";
    str += "\r\n    [PP-Dmin] [PP-Dend] [DP-Dmin] [DP-Dend]";
    str += "\r\n    [PP-FMPT] [DP-FMPT] [PP-MCS] [DP-MCS]";
    str += "\r\n    [MCS] [outputs] [other-outputs]";
    str += "\r\n";
    str += "\r\n    Generate a valid pair (PP, DP) offline.";
    str += "\r\n    If there are either collided or obstructed RPs, the generated pair (PP, DP) will not be saved.";
    str += "\r\n    Note that motion programs generated with this function (without the argument outputs),";
    str += "\r\n    is garanteed to be valid, beause they pass a validation process considered unerring.";
    str += "\r\n    <FMOSA_path>: absolute or relative path to the input file type FMOSA.";
    str += "\r\n    This command generates the file:";
    str += "\r\n        PairPPDP_outputs_from_<filename>: the pair (PP, DP) in format MCS,";
    str += "\r\n            comments about the collided RPs, if any,";
    str += "\r\n            comments about the obstructed RPs, if any,";
    str += "\r\n            comments about the validity of the generated MPs,";
    str += "\r\n            and comments about the RPs whose radial motion is more close to 1 mm.";
    str += "\r\n            the content of the input file type FMOSA,";
    str += "\r\n        Where <filename> is the name of the input file.";
    str += "\r\n";
    str += "\r\n    This function has several optional arguments, each of witch allow save a data in a file,";
    str += "\r\n    whose name is composed as follow:";
    str += "\r\n        \"PairPPDP_\" + <argument> + \"_from_\" + <filename>";
    str += "\r\n    Where <filename> is the name of the input file, and <argument> is one of the following strings:";
    str += "\r\n";
    str += "\r\n    all: save all files.";
    str += "\r\n";
    str += "\r\n    OPL:    Observing Position List in format PPA (Pair Position Angles in steps).";
    str += "\r\n    OPL-S0: Observing Position List in format CC (Cartesian Coordinates) from S0.";
    str += "\r\n    OPL-S1: Observing Position List in format CC (Cartesian Coordinates) from S1.";
    str += "\r\n";
    str += "\r\n    IPL: Initial Position List in format PPA (Pair Position Angles in steps).";
    str += "\r\n";
    str += "\r\n    DispCorners1: displacement of corners 1 from initial position to observing position.";
    str += "\r\n    DispCorners2: displacement of corners 2 from initial position to observing position.";
    str += "\r\n    (1 and 2 are upper and lower corners of the arm elbow, when rotors are in the origin).";
    str += "\r\n";
    str += "\r\n    PP-Dmin: minimun distance during executing the Positioning Program.";
    str += "\r\n    PP-Dend: distance at the end of the trajectory during executing the Positioning Program.";
    str += "\r\n    DP-Dmin: minimun distance during executing the Depositioning Program.";
    str += "\r\n    DP-Dend: distance at the end of the trajectory during executing the Depositioning Program.";
    str += "\r\n    (The distances are given in mm for each gesture for each RP).";
    str += "\r\n";
    str += "\r\n    PP-FMPT: Positioning Program in format FMPT.";
    str += "\r\n    DP-FMPT: Depositioning Program in format FMPT.";
    str += "\r\n    (The format of the FMPT use absolute coordinates and own instruction).";
    str += "\r\n";
    str += "\r\n    PP-MCS: Positioning Program in format MCS.";
    str += "\r\n    DP-MCS: Depositioning Program in format MCS.";
    str += "\r\n    MCS: Pair (PP, DP) in format MCS (without neither comments nor additional information.).";
    str += "\r\n    (PP and DP are also saved in the file ouputs, followed by the content of the input file).";
    str += "\r\n";
    str += "\r\n    outputs: force save file outputs in formats MCS and JSON, even if there are dangerous RPs,";
    str += "\r\n    and even if there are either enabled-not-operative or collided or obstructed RPs.";
    str += "\r\n    other-outputs: the same comments of the file outputs in the old format other-outputs.";
    str += "\r\n";
    str += "\r\n    WARNING! rotor 2 position angle (r2) has sign minus. r2 is exclusive of formats MCS and JSON:";
    str += "\r\n        Files PP-MCS, DP-MCS, MCS and outputs, contain (r1, r2).";
    str += "\r\n        Files OPL, OPL-S0, OPL-S1, IPL, PP-FMPT and DP-FMPT, contain (p_1, p___3), not (r1, r2).";
    str += "\r\n";
    str += "\r\n    WARNING! Although format JSON is used in files type outputs generated by the functions:";
    str += "\r\n        generateParkProg_offline and generatePairPPDP_offline,";
    str += "\r\n    ad-hoc old format (called format MCS), is requested for other functions which";
    str += "\r\n    require a file tye outputs how input.";
    str += "\r\n    Besides FMAT generates file type FMOSA using their own ad-hoc format (not JSON format).";

    return str;
}

//gets a string containing the help about other functions
string help_others(void)
{
    string str;

    str = "$ fmpt_saa valuesSPM_EA <Id>";
    str += "\r\n    View the SPM values.";
    str += "\r\n    <Id>: identifier list of the EA to view.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa valuesSPM_RP <Id>";
    str += "\r\n    View the SPM values pending varibales PAkd and Purpose.";
    str += "\r\n    <Id>: identifier list of the RP to view.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa testRadialMotion";
    str += "\r\n    Take the measure of theoretical maximun deviation around the radial trajectory.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa testDistanceMin";
    str += "\r\n    Determine the minimun distance for each RP, with each of their adjacents,";
    str += "\r\n    when one of each pair is in the origin position.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa applyPCL <PCL_path>";
    str += "\r\n    Apply a positioner center list to the Fiber MOS Model instance.";
    str += "\r\n    The new added RPs are built by default.";
    str += "\r\n    <PCL_path>: absolute or relative path to file containing";
    str += "\r\n    a positioner center list.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa applyRPI <RPI_dir>";
    str += "\r\n    Apply a Robotic Positioner Instance to the Fiber MOS Model Instance.";
    str += "\r\n    <RPI_dir>: absolute or relative path to dir containing a Robotic Positioner Instance.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa evaluateErrors <measures_path>";
    str += "\r\n$   For a RP (minibundle) generate:";
    str += "\r\n        The file type errors.";
    str += "\r\n        The file type vectors.";
    str += "\r\n    <measures_path>: absolute or relative path to file type measures.";
    str += "\r\n    A file type measures contains:";
    str += "\r\n        The tuning parameters: (minibundle, x0, y0, L01, L13, offset1, offset2).";
    str += "\r\n        A position table with the fields (rot1, rot2, x, y).";
    str += "\r\n    A file type errors contains:";
    str += "\r\n        A table with the fields (ave, max, rot1, rot2, x, y, positions).";
    str += "\r\n    A file type vectors contans:";
    str += "\r\n        The vector (positions, aves, maxs) in format Matlab.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa tuneRP <measures_path> <errors_path> <positions>";
    str += "\r\n    Generate the tuning file for a RP.";
    str += "\r\n    <measures_path>: absolute or relative path to file type measures.";
    str += "\r\n    <errors_path>: absolute or relative path to file type errors.";
    str += "\r\n    <positions>: number of positions taken into accounts.";
    str += "\r\n    A file type measures contains:";
    str += "\r\n        The tuning parameters: (minibundle, x0, y0, L01, L13, offset1, offset2).";
    str += "\r\n        A position table with the fields (rot1, rot2, x, y).";
    str += "\r\n    A file type errors contains:";
    str += "\r\n        A table with the fields (ave, max, rot1, rot2, x, y, positions).";
    str += "\r\n    A tuning file is similar to a meaasures file, but containing:";
    str += "\r\n        The tuning parameters with the tuned values.";
    str += "\r\n        The tuning table with only the positions taken into account for tuning.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa applyTuning <tuning_path>";
    str += "\r\n    Apply a file type tuning to the FMM Instance.";
    str += "\r\n    <tuning_path>: absolute or relative path to file type tuning.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa PPA_to_CC-S0 <PPAL_path>";
    str += "\r\n    Translate from pair position angles (in steps) to cartesian coordinates in S0 (in mm).";
    str += "\r\n    <PPAL_path>: absolute or relative path to file containing a PPA list.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa test_generateParkProg_offline";
    str += "\r\n    Test the function generateParkProg_offline:";
    str += "\r\n        generateParkProg_offline(FMM, input_path, output_dir, log_path);";
    str += "\r\n";
    str += "\r\n$ fmpt_saa test_generateParkProg_online";
    str += "\r\n    Test the function generateParkProg_online:";
    str += "\r\n        ParkProgValid = generateParkProgValid_online(outputs_str, FMM_dir,";
    str += "\r\n                                                     p_1s, p___3s, RPids, Bid)";
    str += "\r\n";
    str += "\r\n$ fmpt_saa test_generatePairPPDP_offline";
    str += "\r\n    Test the function generatePairPPDP_offline:";
    str += "\r\n        generatePairPPDP_offline(FMM, input_path, output_dir, log_path);";
    str += "\r\n";
    str += "\r\n$ fmpt_saa test_generatePairPPDP_online";
    str += "\r\n    Test the function generatePairPPDP_online:";
    str += "\r\n        PairPPDPvalid = generatePairPPDP_online(outputs_str, FMM_dir,";
    str += "\r\n                                                p_1s, p___3s, RPids, Bid)";
    str += "\r\n";
    str += "\r\n$ fmpt_saa checkPairPPDP <PP_path> <DP_path> [RPids]";
    str += "\r\n    <PP_path>: absolute or relative path to file containing the PP in teh format of the MCS.";
    str += "\r\n    <DP_path>: absolute or relative path to file containing the DP in format MCS.";
    str += "\r\n    [RPids]: optional identifier list of RPs to be disabled.";
    str += "\r\n    Determine if all RPs included in the pair (PP, DP), are enabled.";
    str += "\r\n    The instance of the Fiber MOS Model will not be written.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa validatePairPPDP <outputs_path>";
    str += "\r\n    Check if a pair (PP, DP) avoid collisions, when it is executed starting from the origin.";
    str += "\r\n    <outputs_path>: absolute or relative path to file type outputs,";
    str += "\r\n    containing the pair (PP, DP).";
    str += "\r\n";
    str += "\r\n$ fmpt_saa regeneratePairPPDP <PP_path> <DP_path> <FMOSA_path> [RPids]";
    str += "\r\n    Attempt regenerate a pair (PP, DP) in limited sense.";
    str += "\r\n    Functions for attempt regenerate a pair (PP, DP) in limited sense";
    str += "\r\n    was developed for restrictive circunstances where the motion programs";
    str += "\r\n    can not be changed. For regenerate a pair (PP, DP) in not limited sense";
    str += "\r\n    use the same function for generate the pair (PP, DP) generatePairPPDP.";
    str += "\r\n    <PP_path>: absolute or relative path to file containing the PP.";
    str += "\r\n    <DP_path>: absolute or relative path to file containing the DP.";
    str += "\r\n    <FMOSA_path>: absolute or relative path to file type FMOSA.";
    str += "\r\n    [RPids]: optional identifier list of RPs to be disabled.";
    str += "\r\n    The instance of the Fiber MOS Model will not be written in permant memory.";

    //Other diserable functionalities:
    //  - Determine the RPs included in a MP.
    //  - Determine the RPs included in a pair of MPs.
    //  - Enable all not faulty RPs of the FMM Instance,
    //  - Disable the indicated RPs of the FMM Instance,
    //  - Print the samples includes in the directory data/Samples

    return str;
}

//get a string containing the about of legend
string aboutOf(void)
{
    string str;

    str = "Copyright (c) 2012-2017 Isaac Morales Durán. All rights reserved.\r\n";
    str += "Institute of Astrophysics of Andalusia, IAA-CSIC. isaac@iaa.es.\r\n";
    str += "\r\n";
    str += "This application is the FMPT SAA "+string(PACKAGE_VERSION)+".\r\n";
    str += "(Fiber MOS Positioning Tools Stand Alone Application).\r\n";
    str += "\r\n";
    str += "FMPT is free software: you can redistribute it and/or modify\r\n";
    str += "it under the terms of the GNU General Public License as published by\r\n";
    str += "the Free Software Foundation, either version 3 of the License, or\r\n";
    str += "(at your option) any later version.\r\n";
    str += "\r\n";
    str += "This program is distributed in the hope that it will be useful,\r\n";
    str += "but WITHOUT ANY WARRANTY; without even the implied warranty of\r\n";
    str += "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\r\n";
    str += "GNU General Public License for more details.\r\n";
    str += "\r\n";
    str += "You should have received a copy of the GNU General Public License\r\n";
    str += "along with this program.  If not, see <http://www.gnu.org/licenses/>.";

    return str;
}

//---------------------------------------------------------------------------
//FUNCTIONS FOR COMMANDS:

//Take the measure of maximun deviation around the radial trajectory of the fiber of each RP.
void testRadialMotion(TFiberMOSModel& FMM, string& log_path)
{
    try {
        //CHECK THE PRECONDITIONS:

        if(FMM.RPL.thereIsSomeNullPointer())
            throw EImproperCall("RP null pointer in the FMM");

        if(FMM.RPL.thereIsSomeRepeatedPointer())
            throw EImproperCall("RP repeated pointer in the FMM");

        if(FMM.RPL.thereIsSomeNotOperative())
            throw EImproperCall("not operative RP in the FMM");

        if(FMM.RPL.thereIsSomeOutOrigin())
            throw EImproperCall("RP out of origin in the FMM");

        //MAKE ACTIONS:

        //indicates that the simulation is extension
        append("SIMULATION THE RADIAL EXTENSION AND RADIAL RETRACTION OF THE FIBERS:", log_path.c_str());
        //print the labels of the table
        append("\r\nId\tdistanceMin\t\tdistanceMax", log_path.c_str());

        for(int i=0; i<FMM.RPL.getCount(); i++) {
            TRoboticPositioner *RP = FMM.RPL[i];

            RP->CMF.ClearProgram();

            //-------------------------------------------------------------
            //RADIAL EXTENSION:

            //get the initial position of the fiber
            TDoublePoint P3ini = RP->getActuator()->getArm()->getP3();

            //programs the RP to make a radial motion
            //from the origin to the maximun extension of the arm
            RP->programExtensionToMoreSpreadPosition();

            //get thefinal position of the fiber
            RP->moveFin();
            TDoublePoint P3fin = RP->getActuator()->getArm()->getP3();

            //The radial trajectory correspond to segment (P3ini, P3fin).

            //SIMULATES THE MOTION:

            //disables the quantifiers of the rotors
            RP->getActuator()->disableQuantification();

            double T = RP->CMF.gettendmax();
            int N = 10000;
            double deltat = T/N;
            double distanceMin = 0;
            double distanceMax = 0;

            //simulates the motion and actualice the limits
            for(int i=0; i<N; i++) {
                //calculate t andmove the RP to t
                double t = i*deltat;
                RP->move(t);

                //calculates the distance between P3 and the segment(P3ini, P3fin)
                TDoublePoint P3 = RP->getActuator()->getArm()->getP3();
                double distance = distanceLineToPoint(P3ini, P3fin, P3);

                //actualize the minimun distance
                if(distance < distanceMin)
                    distanceMin = distance;

                //actualize the maximun distance
                if(distance > distanceMax)
                    distanceMax = distance;
            }

            //enables the quantifiers of the rotors
            RP->getActuator()->enableQuantification();

            //-------------------------------------------------------------
            //RADIAL RETRACTION:

            //programs the RP to make a radial motion
            //from the final position to the origin
            RP->MM(0, 0);

            //disables the quantifiers of the rotors
            RP->getActuator()->disableQuantification();

            //simulates the motion and actualice the limits
            for(int i=0; i<N; i++) {
                //calculate t andmove the RP to t
                double t = i*deltat;
                RP->move(t);

                //calculates the distance between P3 and the segment(P3ini, P3fin)
                TDoublePoint P3 = RP->getActuator()->getArm()->getP3();
                double distance = distanceLineToPoint(P3ini, P3fin, P3);

                //actualize the minimun distance
                if(distance < distanceMin)
                    distanceMin = distance;

                //actualize the maximun distance
                if(distance > distanceMax)
                    distanceMax = distance;
            }

            //enables the quantifiers of the rotors
            RP->getActuator()->enableQuantification();

            //-------------------------------------------------------------
            //PRINT THE RESULT:

            //add a note about the result
            string str = RP->getActuator()->getIdText().str;
            str += "\t"+floattostr(distanceMin)+"\t"+floattostr(distanceMax);

            //print the result
            append(str, log_path.c_str());
        }
    }
    catch(Exception& E) {
        E.Message.Insert(1, "testing radial motion: ");
        throw;
    }
}

//Determine the minimun distance for each RP, with each of their adjacents,
//when one of each pair is in the origin position.
void testDistanceMin(TFiberMOSModel& FMM, string& log_path)
{
    try {
        //CHECK THE PRECONDITIONS:

        if(FMM.RPL.thereIsSomeNullPointer())
            throw EImproperCall("RP null pointer in the FMM");

        if(FMM.RPL.thereIsSomeRepeatedPointer())
            throw EImproperCall("RP repeated pointer in the FMM");

        if(FMM.RPL.thereIsSomeEnabledRPsWithDisabledQuantifiers())
            throw EImproperCall("all enabled RPs in the FMM sjould have enabled the quantifiers of their rotors");

        //MAKE ACTIONS:

        //indicates that the simulation is extension
        append("THE SPMmax OF EACH RP:", log_path.c_str());
        //print the labels of the table
        append("\r\nId\tSPMmax", log_path.c_str());

        //for each RP and for each adjacent, calculates the SPMmin
        for(int i=0; i<FMM.RPL.getCount(); i++) {
            //point the indictaed RP for facilitate their access
            TRoboticPositioner *RP = FMM.RPL[i];

            //-------------------------------------------------------------

            double SPMmax = DBL_MAX; //valor que hay que calcular para el RP actual

            //determina el SPMmax con cada adyacente
            for(int j=0; j<RP->getActuator()->AdjacentRPs.getCount(); j++) {
                //point the indicated adjacent RP for facilitate their access
                TRoboticPositioner *RPA = RP->getActuator()->AdjacentRPs[j].RP;

                double d; //distance between the RP and the actual RPA

                //si el RP está deshabilitado
                if(RP->Disabled) {
                    //if RP disabled the RPA disabled
                    if(RPA->Disabled) {
                        d = RP->getActuator()->distance(RPA->getActuator());
                    }
                    //if RP disabled the RPA enabled
                    else {
                        double ra = RPA->getActuator()->getL01() + RPA->getActuator()->getArm()->getL1V();
                        d = RP->getActuator()->distance(RPA->getActuator()->getP0()) - ra;
                    }
                }
                //si el RP no está deshabilitado
                else {
                    //if RP enabled the RPA disabled
                    if(RPA->Disabled) {
                        double r = RP->getActuator()->getL01() + RP->getActuator()->getArm()->getL1V();
                        d = RPA->getActuator()->distance(RP->getActuator()->getP0()) - r;
                    }
                    //if RP enabled the RPA enabled
                    else {
                        //calculate the vertex radius with the arms deployed
                        double r_deployed = RP->getActuator()->getL01() + RP->getActuator()->getArm()->getL1V();
                        double ra_deployed = RPA->getActuator()->getL01() + RPA->getActuator()->getArm()->getL1V();
                        //calculate the vertex radius with the arms in the origin
                        double r_origin = RP->getActuator()->getArm()->getContour().distanceMax(RP->getActuator()->getP0());
                        double ra_origin = RPA->getActuator()->getArm()->getContour().distanceMax(RPA->getActuator()->getP0());

                        double aux = Mod(RPA->getActuator()->getP0() - RP->getActuator()->getP0());
                        double d1 = aux - r_origin - ra_deployed;
                        double d2 = aux - r_deployed - ra_origin;

                        d = min(d1, d2);
                    }
                }

                //update the SPMmax value
                if(d < SPMmax)
                    SPMmax = d;

            } //for(int i=0; i<FMM.RPL.getCount(); i++) {

            //-------------------------------------------------------------
            //PRINT THE RESULT:

            //add a note about the result
            string str = RP->getActuator()->getIdText().str;
            str += "\t"+floattostr(SPMmax);

            //print the result
            append(str, log_path.c_str());
        }
    }
    catch(Exception& E) {
        E.Message.Insert(1, "testing SPMmax: ");
        throw;
    }
}

//Apply a positioner center list to the Fiber MOS Model instance.
//The new added RPs are built by default.
void applyPCL(TFiberMOSModel& FMM, string& path, string& log_path)
{
    try {
        //LOAD SETTINGS FROM FILES:

        //load the positioner center list from a file
        string str;
        strReadFromFile(str, path);
        TPositionerCenterList PCL;
        PCL.setTableText(str);
        append("Positioner Center List loaded from file '"+path+"'.", log_path.c_str());

        //MAKE THE OPERATIONS:

        //assimilates the list in the Fiber MOS Model
        for(int i=0; i<PCL.getCount(); i++) {
            TPositionerCenter PC = PCL[i];
            int j = FMM.RPL.searchId(PC.Id);
            if(j < FMM.RPL.getCount()) {
                TRoboticPositioner *RP = FMM.RPL[j];
                RP->getActuator()->setP0(TDoublePoint(PC.x0, PC.y0));
            } else {
                int Id = FMM.RPL.searchFirstFreeId(1);
                TDoublePoint P0;
                P0.x = PC.x0;
                P0.y = PC.y0;
                TRoboticPositioner *RP = new TRoboticPositioner(Id, P0);
                FMM.RPL.Add(RP);
            }
        }
        FMM.assimilate();
        append("Positioner Center List applied to the Fiber MOS Model.", log_path.c_str());

    } catch(Exception& E) {
        E.Message.Insert(1, "aplying positioner center list: ");
        throw;
    }
}

//Apply a RP Instance to the Fiber MOS Model instance.
void applyRPI(TFiberMOSModel& FMM, string& RPI_path, string& log_path)
{
    try {
        //LOAD SETTINGS FROM FILES:

        //load the RP Instance from a dir
        TRoboticPositioner RP;
        readInstanceFromDir(RP, RPI_path);
        append("RP Instance loaded from file '"+RPI_path+"'.", log_path.c_str());

        //MAKE THE OPERATIONS:

        //assimilates the instance of the RP in the Fiber MOS Model
        FMM.RPL.apply(&RP, FMM.EAL);
        append("RP Instance applied to the Fiber MOS Model.", log_path.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "aplying RP Instance: ");
        throw;
    }
}

//Generate a table with the fields (errorAve, errorMax, positions).
void evaluateErrors(TFiberMOSModel& FMM, const string& measures_path, const string& output_dir, string& log_path)
{
    try {
        //load the file type measures
        string str;
        strReadFromFile(str, measures_path);
        Measures measures;
        measures.setText(str);
        append("Mesures loaded from file '"+measures_path+"'.", log_path.c_str());

        //check the preconditions
        if(measures.PL.getCount() <= 0)
            throw EImproperFileLoadedValue("measures should contain almost a position");

        //search the RP (of the minibundle) in the FMM
        int i = FMM.RPL.searchId(measures.minibundle);

        //check that has found the RP
        if(i >= FMM.RPL.getCount())
            throw EImproperFileLoadedValue("minibundle not found in the FMM: " + inttostr(measures.minibundle));

        //point the RP
        TRoboticPositioner *RP = FMM.RPL[i];

        //get the tuning parameters from the measures
        measures.getTuningParameters(RP);
        append("", log_path.c_str());
        append("Parameters got from measures:", log_path.c_str());
        append("    P0: "+RP->getActuator()->getP0Text().str+" mmxmm", log_path.c_str());
        append("    L01: "+RP->getActuator()->getL01Text().str+" mm", log_path.c_str());
        append("    L13: "+RP->getActuator()->getArm()->getL13Text().str+" mm", log_path.c_str());
        append("    thetaO1: "+RP->getActuator()->getthetaO1Text().str+" rad", log_path.c_str());
        append("    theta_O3o: "+RP->getActuator()->gettheta_O3oText().str+" rad", log_path.c_str());

        //determine the error
        Error error;
        measures.determineError(error, RP);
        append("", log_path.c_str());
        append("Initial error before fast tuning:", log_path.c_str());
        append(StrIndent(error.getAssignText()).str, log_path.c_str());

        //split the path of the input file
        string dir, filename;
        splitpath(dir, filename, measures_path);

        //initialize the file type errors
        //printing the header of the table
        str = error.getLabelsRowText();
        //string path = output_dir + "/evaluateErrors";
        //ForceDirectories(path);
        //path += "/errors_from_" + filename;
        string path = output_dir + "/errors_from_" + filename;
        strWriteToFile(path, str);
        append("", log_path.c_str());
        append("Error List initialized in file '"+path+"'.", log_path.c_str());

        //print the header of the table
        append(error.getErrorFunctionVariablesLabelsRowText(), log_path.c_str());

        Keyboard keyboard;
        keyboard.initNonCannonical();

        RoboticPositionerTuner RPT;
        ErrorList EL;
        char c = 0;
        do {
            //tune the parameters of the RP
            RPT.tune(RP, &measures);
            //determine the error and the location of the worst position
            int i = measures.determineError(error, RP);
            //delete the worst position
            measures.PL.Delete(i);

            //add the error toe the file type errors
            str = "\r\n" + error.getRowText();
            char mode[] = "a";
            TTextFile TF(path.c_str(), mode);
            TF.Print(str.c_str());
            TF.Close();

            //print the error
            append(error.getErrorFunctionVariablesRowText(), log_path.c_str());

            //add the error to the error list
            EL.Add(error);

            //capture key pulsation
            if(keyboard.kbhit()) {
                c = keyboard.getch();
                keyboard.restoreSetting();
            }
        }
        while(measures.PL.getCount() > 0 && c!=27);

        //indicates that evaluation has finished
        append("Error List finished in file '"+path+"'.", log_path.c_str());

        //save the vectors in a file
        str = "positions = [" + EL.getPositionsRowText() + "]";
        str += "\r\n";
        str += "\r\naves = [" + EL.getAvesRowText() + "]";
        str += "\r\n";
        str += "\r\nmaxs = [" + EL.getMaxsRowText() + "]";
        //path = output_dir + "/evaluateErrors/vectors_from_" + filename;
        path = output_dir + "/vectors_from_" + filename;
        strWriteToFile(path, str);
        append("Vectors saved in file '"+path+"'.", log_path.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "translating from PPA (in steps) to CC-S0 (in mm): ");
        throw;
    }
}

//Generate a tuning file.
void tuneRP(TFiberMOSModel& FMM, const string& measures_path, const string& errors_path, int pos,
            const string& output_dir, string& log_path)
{
    try {
        //load the file type measures
        string str;
        strReadFromFile(str, measures_path);
        Measures measures;
        measures.setText(str);
        append("Mesures loaded from file '"+measures_path+"'.", log_path.c_str());

        //check the preconditions
        if(measures.PL.getCount() <= 0)
            throw EImproperFileLoadedValue("measures should contain almost a position");

        //search the RP (of the minibundle) in the FMM
        int i = FMM.RPL.searchId(measures.minibundle);

        //check that has found the RP
        if(i >= FMM.RPL.getCount())
            throw EImproperFileLoadedValue("minibundle not found in the FMM: " + inttostr(measures.minibundle));

        //point the RP
        TRoboticPositioner *RP = FMM.RPL[i];

        //load the file type errors
        strReadFromFile(str, errors_path);
        ErrorList EL;
        EL.setText(str);
        append("Errors loaded from file '"+errors_path+"'.", log_path.c_str());

        //sort the error table to decreasing pos
        EL.Compare = Error::comparePos;
        EL.SortDec();

        //check the precondition
        int p = 0;
        while(p < EL.getCount() && EL[p].getPos() != pos)
            p++;
        if(p > EL.getCount())
            throw EImproperArgument("number of positions to be into account (pos) should be in the file type error");

        //delete the positions whose error.pos > pos
        for(i=0; i<p; i++) {
            Error error = EL[i];

            //make a rutinary check
            if(error.getPos() <= pos)
                throw EImpossibleError("lateral effect");

            //get the position from the error
            Position position;
            error.worstPosition.get(position);

            //search the position starting by the end of the position list
            int j = measures.PL.getCount() - 1;
            while(j >= 0 && measures.PL[j] != position)
                j--;

            //delete the position if has found it
            if(j >= 0)
                measures.PL.Delete(j);
            else
                throw EImproperFileLoadedValue("file type error not correspond to the file tyoe measures");
        }

        //get the tuning parameters from measures to the RP
        measures.getTuningParameters(RP);
        append("\r\nNot tuned parameters got from measures to the RP.", log_path.c_str());

        //determine the error before fast tuning
        Error error;
        measures.determineError(error, RP);
        append("Error determined before fast tuning:", log_path.c_str());
        append(StrIndent(error.getAssignText()).str, log_path.c_str());

        //get the tuning parameters from error to the RP
        error = EL[p];
        error.tuningParameters.get(RP);
        append("\r\nFast tuned parameters got from error to the RP.", log_path.c_str());

        //determine the error before fine tuning
        measures.determineError(error, RP);
        append("Error determined before fine tuning:", log_path.c_str());
        append(StrIndent(error.getAssignText()).str, log_path.c_str());

        //make the fine tuning
        RoboticPositionerTuner RPT;
        RPT.setFineTuningMode();
        RPT.verbose = true;
        RPT.tune(RP, &measures);

        //determine the error after fine tuning
        measures.determineError(error, RP);
        append("\r\n\r\nError determined after fine tuning:", log_path.c_str());
        append(StrIndent(error.getAssignText()).str, log_path.c_str());

        //get the filename from measures_path
        string dir, filename;
        splitpath(dir, filename, measures_path);

        //save the measures (with the fine tuned parameters)
        measures.setTuningParameters(RP);
        measures.getText(str);
        string output_path = output_dir + "/fine-tuned-parameters-" + inttostr(pos) + "_from_" + filename;
        strWriteToFile(output_path, str);
        append("\r\nTuned parameters saved in file '"+output_path+"'.", log_path.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "translating from PPA (in steps) to CC-S0 (in mm): ");
        throw;
    }
}

//Apply a set of tuning files to the FMM Instance.
void applyTuning(TFiberMOSModel& FMM, const string& tuning_path, string& log_path)
{
    try {
        //load the file type tuning
        string str;
        strReadFromFile(str, tuning_path);
        Measures measures;
        measures.setText(str);
        append("Mesures loaded from file '"+tuning_path+"'.", log_path.c_str());

        //check the preconditions
        if(measures.PL.getCount() <= 0)
            throw EImproperFileLoadedValue("measures should contain almost a position");

        //search the RP (of the minibundle) in the FMM
        int i = FMM.RPL.searchId(measures.minibundle);

        //check that has found the RP
        if(i >= FMM.RPL.getCount())
            throw EImproperFileLoadedValue("minibundle not found in the FMM: " + inttostr(measures.minibundle));

        //point the RP
        TRoboticPositioner *RP = FMM.RPL[i];

        //get the tuning parameters from the measures
        measures.getTuningParameters(RP);
        append("\r\nParameters got from measures:", log_path.c_str());
        append("    P0: "+RP->getActuator()->getP0Text().str+" mmxmm", log_path.c_str());
        append("    L01: "+RP->getActuator()->getL01Text().str+" mm", log_path.c_str());
        append("    L13: "+RP->getActuator()->getArm()->getL13Text().str+" mm", log_path.c_str());
        append("    thetaO1: "+RP->getActuator()->getthetaO1Text().str+" rad", log_path.c_str());
        append("    theta_O3o: "+RP->getActuator()->gettheta_O3oText().str+" rad", log_path.c_str());

        //assimilate the new parameters
        FMM.assimilate();
        append("\r\nNew tuning parameters assimilated.", log_path.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "aplying RP Instance: ");
        throw;
    }
}

//translate from SSA (in steps) to CC in S0 (in mm)
void PPA_to_CC_S0(TFiberMOSModel& FMM, const string& PPAL_path, const string& output_dir, string& log_path)
{
    try {
        //load the PPA list
        string str;
        strReadFromFile(str, PPAL_path);
        TPairPositionAnglesList PPAL;
        PPAL.setText(str);
        append("PPA list loaded from file '"+PPAL_path+"'.", log_path.c_str());

        //check the preconditions
        if(PPAL.getCount() != FMM.RPL.getCount())
            throw EImproperFileLoadedValue("PPA list should contain a PPA for each RP (including disabled RPs)");

        //set the PPA list in the FMM
        FMM.RPL.setPositions(PPAL);

        //split the path of the file
        string dir, filename;
        splitpath(dir, filename, PPAL_path);

        //The filename will be used to attach the outputs filenames witht the input filename.

        //get the positions in CC-S0
        str = TActuator::getPositionP3LabelsRow().str;
        str += "\r\n";
        str += FMM.RPL.getPositionsP3TableText().str;
        string output_path = output_dir + "/CC-S0-L_from_" + filename;
        strWriteToFile(output_path, str);
        append("CC-S0 list saved in file '"+output_path+"'.", log_path.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "translating from PPA (in steps) to CC-S0 (in mm): ");
        throw;
    }
}

//---------------------------------------------------------------------------------
//FUNCTIONS FOR PARKING PROGRAM:
//---------------------------------------------------------------------------------

//Generate a parking program offline.
//Inputs:
//  FMM: the Fiber MOS Model
//  input_path: path to the input file type FMOSA
//  output_dir: path to the output directory hwere save the output files
//  log_path: path of the log file where save the events
//Outputs:
//  generateParkProg_offline: indicates if the generated parking program is suitable to be executed:
//      the generated parking program is valid and
//      there aren't enabled-not-operative RPs with fault type dynamic or unknowledge
bool generateParkProg_offline(TFiberMOSModel& FMM, const string& input_path, const string& output_dir, const string& log_path,
                              //------------------------------------------------------
                              bool saveSPL, bool saveFPL,
                              bool saveDmin, bool saveDend,
                              bool saveDispCorners1, bool saveDispCorners2,
                              bool saveFMPT, bool saveMCS,
                              bool saveOutputs, bool saveOtherOutputs)
{
    try {
        //get the list of EnabledNotOperative RPs
        TRoboticPositionerList EnabledNotOperative;
        FMM.RPL.getEnabledNotOperative(EnabledNotOperative);

        //get the list of Dangerous RPs
        TRoboticPositionerList Dangerous;
        FMM.RPL.getDangerous(Dangerous);

        //solve the case with dangerous RPs in the FMM
        if(!saveOutputs && Dangerous.getCount() > 0) {
            append("The FMM Instance contains dangerous RPs, so the parking program will not be generated.", log_path.c_str());
            append("A dangerous RP is an enabled-not-operative RP with fault type dynamic or unknowledge.", log_path.c_str());
            append("You can force generation of the parking program with dangerous RPs, typing the argument outputs.", log_path.c_str());
            append("Files outputs generated by force will contain an initial uncommented text for dificult their execution.", log_path.c_str());
            return false;
        }

        //load the FMOSA from the file input_path
        TFMOSA FMOSA;
        unsigned int Bid;
        string str;
        strReadFromFileWithComments(str, input_path);
        FMOSA.setTableText(Bid, str);
        append("FMOSA loaded from file '"+input_path+"'.", log_path.c_str());

        //get the allocation from the FMOSA
        TMotionProgramGenerator MPG(&FMM);
        FMOSA.getAllocations(MPG);
        append("Allocations got from the FMOSA to MPG.", log_path.c_str());

        //split the path of the file containing the FMOSA
        string dir, filename;
        splitpath(dir, filename, input_path);

        //The filename will be used to attach the outputs filenames witht the input filename.

        //move the RPs to the more closer stable point to the TPs
        MPG.MoveToTargetP3();
        if(MPG.getCount() > 0)
            append("RPs moved to observing positions.", log_path.c_str());
        else
            append("There aren't RPs to be moved to observing positions.", log_path.c_str());

        //WARNING: sentence could take several seconds:
        //  MPG.MoveToTargetP3();

        //Other way to obtain the more closer stable points to the projection points,
        //consist in get from the FMOSA the following lists:
        //  the allocation list;
        //  the projection point list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

        //A PPA list shall be stored how a table (Id, p_1, p___3).

        //captures the starting positions of the RPs in a PPA list
        TPairPositionAnglesList SPL;
        FMM.RPL.getPositions(SPL);
        append("Starting Position List got from the FMM.", log_path.c_str());

        //captures the corner of arms in the observing positions in cartesian coordinates
        TItemsList<TDoublePoint> SPL_Corners1;
        FMM.RPL.getPositionsCorners1(SPL_Corners1);
        TItemsList<TDoublePoint> SPL_Corners2;
        FMM.RPL.getPositionsCorners2(SPL_Corners2);

        //segregates the enabled outsider RPs
        TRoboticPositionerList Outsiders;
        FMM.RPL.segregateEnabledOutsiders(Outsiders);

        //get the list of colliding items (both EAs and RPs) in text format
        FMM.RPL.setPurpose(pGenParPro);
        string collided_str;
        FMM.getCollidedText(collided_str);

        //Note that MPG.generateParkProg will execute automatically:
        //  //configure the Fiber MOS Model for generate a parking program
        //  getFiberMOSModel()->RPL.setPurpose(pGenParPro);

        //check the limiting case when there aren't allocations
        if(MPG.getCount() <= 0)
            append("WARNING: there aren't allocations. The generated parking program will be empty.", log_path.c_str());
        //check the limiting case when all operative RPs are in the origin
        else if(FMM.RPL.allOperativeRPsAreInTheOrigin())
            append("WARNING: all operative RPs are in the origin. The generated parking program will be empty.", log_path.c_str());
        //else, check the limiting case when all operative RPs are in security positions
        else if(Outsiders.getCount() <= 0)
            append("WARNING: all operative RPs are in security positions. The generated parking program will contain only a message-instruction list to go to the origin.", log_path.c_str());

        //Now are fulfilled the preconditions for the function TMotionProgramGenerator::generateParkProg:
        //  All RPs of the Fiber MOS Model:
        //    - shall be in their starting positions.
        //  All RPs of the list Outsiders:
        //    - shall be in the Fiber MOS Model.
        //    - shall be operatives.
        //    - shall be in insecurity positions.
        //    - shall have enabled the quantifiers.

        //generates a parking program and determines the collided and obstructed RPs in insecurity positions
        append_without_endl("Generating a parking program...", log_path.c_str());
        TRoboticPositionerList Collided;
        TRoboticPositionerList Obstructed;
        TMotionProgram ParkProg;
        bool ParkProgValid = MPG.generateParkProg(Collided, Obstructed, ParkProg, Outsiders);
        append(" Process finished.", log_path.c_str());

        //Now are fulfilled the postconditions:
        //  All RPs of the FMM:
        //    - will be configured for validate a parking program.
        //  When the generated recovery program isn't valid:
        //      All RPs of the FMM:
        //        - will have disabled the quantifiers of their rotors;
        //        - will be in the first position where the collision was detected
        //              during the validation process.
        //  When the generated recovery program is valid (even the trivial case):
        //      All RPs of the FMM:
        //        - will have enabled the quantifiers of their rotors;
        //        - will be in their final positions.

        //########################################################################
        //#WARNING: before re-use a function to generation, shall be restablished#
        //#the preconditions.                                                    #
        //########################################################################

        //solve the case when the generated parking program is not valid
        if(!saveOutputs && !ParkProgValid) {
            append("The generated parking program is not valid, so none file will be saved.", log_path.c_str());
            append("A parking program is not valid when a collision is detected during the validation process.", log_path.c_str());
            append("You can force generation of the not valid parking program, typing the argument outputs.", log_path.c_str());
            append("Files outputs generated by force will contain an initial uncommented text for dificult their execution.", log_path.c_str());
            return false;
        }

        //captures the final positions of the RPs in a PPA list
        TPairPositionAnglesList FPL;
        FMM.RPL.getPositions(FPL);
        append("Final Position List got from the FMM.", log_path.c_str());

        //SET VALUES IN THE STRUCTURE OUTPUTS:

        //properties for built comments about file outputs
        OutputsParkProg outputs;
        outputs.FMOSA_filename = filename;
        outputs.FMPT_version = string(PACKAGE_VERSION);
        outputs.datetime = strfromtime_t(time(NULL), "%Y-%m-%dT%H:%M:%S");
        outputs.EnabledNotOperative = EnabledNotOperative;
        outputs.collided_str = collided_str;
        outputs.Collided = Collided;
        outputs.Obstructed = Obstructed;
        //properties for built comments about each MP
        outputs.ParkProgValid = ParkProgValid;
        outputs.DsecMax = 1;
        //properties for built the executable data
        outputs.Bid = Bid;
        outputs.SPL = SPL;
        outputs.ParkProg = ParkProg;
        outputs.FPL = FPL;

        //PRINT THE COMMENTS ABOUT OUTPUTS:

        //print the comments about the outputs
        append("", log_path.c_str());
        append(outputs.getCommentsText(), log_path.c_str());
        append("", log_path.c_str());

        //SAVE FILES:

        //save SPL, if any
        string output_path;
        if(saveSPL) {
            str = TActuator::getPositionPPALabelsRow().str;
            str += "\r\n"+SPL.getColumnText().str;
            ForceDirectories(AnsiString(output_dir));
            output_path = output_dir+"/ParkProg_SPL_from_"+filename;
            strWriteToFile(output_path, str);
            append("Starting Position List saved in file '"+output_path+"'.", log_path.c_str());
        }

        //Other whay to obtain the starting position list directly in text format:
        //  FMM.RPL.getPositionsPPATableText()

        //save FPL, if any
        if(saveFPL) {
            str = TActuator::getPositionPPALabelsRow().str;
            str += "\r\n"+FPL.getColumnText().str;
            if(ParkProgValid) {
                //warn about invalidity of the generated parking program
                append("Generated parking program have passed the validation process, so the FMM is in the final position.", log_path.c_str());

                output_path = output_dir + "/ParkProg_FPL_from_" + filename;
                strWriteToFile(output_path, str);
                append("Final Position List saved in file '"+output_path+"'.", log_path.c_str());
            } else {
                //warn about invalidity of the generated parking program
                append("Generated parking program have not passed the validation process, so the FMM is in the colliding position, detected during validation process.", log_path.c_str());
                append("Will be saved Colliding Position List instead Final Position List.", log_path.c_str());

                output_path = output_dir + "/ParkProg_CPL_from_" + filename;
                strWriteToFile(output_path, str);
                append("Colliding Position List saved in file '"+output_path+"'.", log_path.c_str());
            }
        }

        //save DispCorners1, if any
        if(saveDispCorners1) {
            //captures the corner of arms in the initial positions in cartesian coordinates
            TItemsList<TDoublePoint> IPL_Corners1;
            FMM.RPL.getPositionsCorners1(IPL_Corners1);

            //calculates the displacement of the corner
            TItemsList<TDoublePoint> DispCorners1(SPL_Corners1);
            for(int i=0; i<DispCorners1.getCount(); i++) {
                DispCorners1[i].x -= IPL_Corners1[i].x;
                DispCorners1[i].y -= IPL_Corners1[i].y;
            }

            //DispCorners1.Print = TDoublePoint::PrintRow;
            //str = DispCorners1.getText().str;
            str = "Id\tdx\tdy";
            for(int i=0; i<FMM.RPL.getCount(); i++) {
                str += "\r\n";
                TRoboticPositioner *RP = FMM.RPL[i];
                str += RP->getActuator()->getIdText().str;
                str += "\t";
                str += DispCorners1[i].getRowText().str;
            }
            output_path = output_dir + "/ParkProg_DispCorners1_from_" + filename;
            strWriteToFile(output_path, str);
            append("Displacement corners 1 saved in in file '"+output_path+"'.", log_path.c_str());
        }

        //save DispCorners2, if any
        if(saveDispCorners2) {
            //captures the corner of arms in the initial positions in cartesian coordinates
            TItemsList<TDoublePoint> IPL_Corners2;
            FMM.RPL.getPositionsCorners2(IPL_Corners2);

            //calculates the displacement of the corner
            TItemsList<TDoublePoint> DispCorners2(SPL_Corners2);
            for(int i=0; i<DispCorners2.getCount(); i++) {
                DispCorners2[i].x -= IPL_Corners2[i].x;
                DispCorners2[i].y -= IPL_Corners2[i].y;
            }

            //DispCorners2.Print = TDoublePoint::PrintRow;
            //str = DispCorners2.getText().str;
            str = "Id\tdx\tdy";
            for(int i=0; i<FMM.RPL.getCount(); i++) {
                str += "\r\n";
                TRoboticPositioner *RP = FMM.RPL[i];
                str += RP->getActuator()->getIdText().str;
                str += "\t";
                str += DispCorners2[i].getRowText().str;
            }
            output_path = output_dir + "/ParkProg_DispCorners2_from_" + filename;
            strWriteToFile(output_path, str);
            append("Displacement corners 2 saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save ParkProg_Dmin, if any
        if(saveDmin) {
            ParkProg.getDminMCStext(str, "depos", Bid);
            output_path = output_dir + "/ParkProg_Dmin_from_" + filename;
            strWriteToFile(output_path, str);
            append("ParkProg-Dmin saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save ParkProg_Dend, if any
        if(saveDend) {
            ParkProg.getDendMCStext(str, "depos", Bid);
            output_path = output_dir + "/ParkProg_Dend_from_" + filename;
            strWriteToFile(output_path, str);
            append("ParkProg-Dend saved in file '"+output_path+"'.", log_path.c_str());
        }

        //SAVE FILES ABOUT MPs:

        //save ParkProg_FMPT, if any
        if(saveFMPT) {
            str = ParkProg.getText().str;
            output_path = output_dir + "/ParkProg_FMPT_from_" + filename;
            strWriteToFile(output_path, str);
            append("Parking program in format FMPT saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save ParkProg_MCS, if any
        if(saveMCS) {
            ParkProg.getMCStext(str, "depos", Bid, SPL);
            output_path = output_dir + "/ParkProg_MCS_from_" + filename;
            strWriteToFile(output_path, str);
            append("Parking program in format MCS saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save file outputs, if any
        bool suitable = outputs.suitable();
        if(suitable) {
            //save outputs
            outputs.getText(str);
            output_path = output_dir+"/ParkProg_outputs_from_"+filename;
            strWriteToFile(output_path, str);
            append("Outputs in format MCS saved in file '"+output_path+"'.", log_path.c_str());

            //save outputs in format JSON
            str = outputs.getJSONtext();
            output_path = output_dir + "/ParkProg_outputs_from_" + stem(filename) + ".json";
            strWriteToFile(output_path, str);
            append("Outputs in format JSON saved in file '"+output_path+"'.", log_path.c_str());
        }
        else { //not suitable
            //if force save outputs:
            if(saveOutputs) {
                //warn about force saving file outputs
                append("WARNING! Forced saving file outputs despite it is not suitable to be executed.", log_path.c_str());
                append("Files outputs generated by force contain an initial uncommented text for dificult their execution.", log_path.c_str());

                //save outputs
                outputs.getText(str);
                output_path = output_dir+"/ParkProg_outputs_from_"+filename;
                strWriteToFile(output_path, str);
                append("Outputs in format MCS saved in file '"+output_path+"'.", log_path.c_str());

                //save outputs in format JSON
                str = outputs.getJSONtext();
                output_path = output_dir + "/ParkProg_outputs_from_" + stem(filename) + ".json";
                strWriteToFile(output_path, str);
                append("Outputs in format JSON saved in file '"+output_path+"'.", log_path.c_str());
            }
            //if not force save outputs:
            else {
                //warn about force saving file outputs
                append("WARNING! File outputs will not be saved, because it is not suitable to be executed. You can force write outputs typing argument outputs.", log_path.c_str());
            }
        }

        //save other-outputs, if any
        if(saveOtherOutputs) {
            outputs.getOtherText(str);
            output_path = output_dir+"/ParkProg_other-outputs_from_"+filename;
            strWriteToFile(output_path, str);
            append("Ohter-outputs saved in file '"+output_path+"'.", log_path.c_str());
        }

        //indicates if the generated parking program is suitable to be executed
        return suitable;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "generating parking program offline: ");
        throw;
    }
}

//Test the function generateParkProg_offline.
void test_generateParkProg_offline(TFiberMOSModel& FMM, string& log_path)
{
    try {

        //CHECK THE PRECONDITIONS:

        if(FMM.RPL.thereIsSomeNullPointer())
            throw EImproperCall("RP null pointer in the FMM");

        if(FMM.RPL.thereIsSomeRepeatedPointer())
            throw EImproperCall("RP repeated pointer in the FMM");

        if(FMM.RPL.thereIsSomeNotOperative())
            throw EImproperCall("not operative RP in the FMM");

        //MAKE ACTIONS:

        //builds the MPG attached to the FMM
        TMotionProgramGenerator MPG(&FMM);

        //other variables
        int count = 0; //number os simulations
        string input_path; //path of input file
        string filename; //filename of reference including the number of simulation
        string output_dir; //folder to put the outputss
        string output_path; //output filename each time
        string str; //string to writo to file each time
        bool valid; //indicates if the generated parking program is valid and if there aren't neither collided nor obstructed RPs

        do {
            //contabilize the test and print the test tittle
            append("TEST "+inttostr(++count)+":", log_path.c_str());
            append("========================================================================", log_path.c_str());

            //initialize the output subdirectory
            output_dir = "test_generateParkProg_offline-"+inttostr(count);
            ForceDirectories(AnsiString(output_dir));

            //build the filename of reference
            filename = output_dir+".txt";

            //-------------------------------------------------------------------
            //GENERATES A FILE CONTAINING A RANDOM FMOSA:

            //add to the MPG an allocation for each RP of the FMM
            Destroy(MPG);
            for(int i=0; i<FMM.RPL.getCount(); i++)
                MPG.AddAllocation(i);

            //randomize the projection points in the domain of their attached RPs
            FMM.RPL.setPurpose(pGenParPro);
            append("FMM configured for generate a pair (PP, DP). (Purpose = GenParPro).", log_path.c_str());
            MPG.RandomizeWithoutCollision();
            append("Projection points randomized avoinding collisions.", log_path.c_str());

            //WARNING: sentence could take several seconds:
            //  MPG.RandomizeWithoutCollision();

            //save the allocation list
            str = TAllocation::GetIdPPLabelsRow().str;
            str += "\r\n"+MPG.getColumnText().str;
            output_path = output_dir+"/AL-for-"+filename;
            strWriteToFile(output_path, str);
            append("Allocation list saved in file '"+output_path+"'.", log_path.c_str());

            //build a FMOSA and set the allocations
            TFMOSA FMOSA;
            for(int i=0; i<MPG.getCount(); i++) {
                TAllocation *A = MPG[i];

                TObservingSource *OS = new TObservingSource();

                //transcribe the allocation properties to the OS
                OS->Type = ptSOURCE;
                OS->Pid = A->getRP()->getActuator()->getId();
                OS->X = A->PP.x;
                OS->Y = A->PP.y;
                OS->Enabled = true;

                //set the optional values
                OS->Name = "s:"+inttostr(i+1);
                OS->Mag = 0;
                OS->Pr = 0;
                OS->Bid = 0;
                //OS->notAllocated = true;
                //OS->allocateInAll = false;
                OS->Comment = "too coment";

                //enable the optional values
                OS->there_is_Mag = true;
                OS->there_is_Pr = true;
                OS->there_is_Bid = true;
                //OS->there_is_notAllocated = true;
                //OS->there_is_allocateInAll = true;

                //add the OS to the FMOSA
                FMOSA.Add(OS);
            }

            //save the FMOSA in a file
            FMOSA.getTableText(str);
            output_path = output_dir+"/FMOSA-for-"+filename;
            strWriteToFile(output_path, str);
            append("FMOSA saved in file '"+output_path+"'.", log_path.c_str());

            //-------------------------------------------------------------------
            //TEST THE FUNCTION TO GENERATE PAIRS (PP, DP):

            //initializes the parameters of the function:
            Destroy(MPG);
            input_path = output_path;

            //call the function to test
            append("Calling function generateParkProg_offline...", log_path.c_str());
            append("-------------------------------------------------------------------", log_path.c_str());
            append("generateParkProg_offline(FMM, input_path, output_dir, log_path);", log_path.c_str());
            valid = generateParkProg_offline(FMM, input_path, output_dir, log_path,
                                             true, true,
                                             true, true,
                                             true, true,
                                             true, true,
                                             true, true);
            append("-------------------------------------------------------------------", log_path.c_str());
            append("Returned from function generateParkProg_offline.", log_path.c_str());

            //WARNING: sentence could take several seconds:
            //  generateParkProg_offline(FMM, input_path, output_dir, log_path,
            //                           true, true,
            //                           true, true,
            //                           true, true
            //                           true, true,
            //                           true, true);

            //Note that the outputs are saved in the function to test.

            //prepare the standard output for the next test
            append("", log_path.c_str());
        }
        while(valid);

        //indicates that test has failed
        append("\r\nFailed test "+inttostr(count)+"!", log_path.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "testing generatePairPPDP_offline: ");
        throw;
    }
}

//Test the function generateParkProg_online.
void test_generateParkProg_online(TFiberMOSModel& FMM, string& log_path)
{
    try {

        //CHECK THE PRECONDITIONS:

        if(FMM.RPL.thereIsSomeNullPointer())
            throw EImproperCall("RP null pointer in the FMM");

        if(FMM.RPL.thereIsSomeRepeatedPointer())
            throw EImproperCall("RP repeated pointer in the FMM");

        if(FMM.RPL.thereIsSomeOperativeRPwithDynamicFaul())
            throw EImproperCall("operative RP with dynamic fault in the FMM");

        //MAKE ACTIONS:

        //builds the MPG attached to the FMM
        TMotionProgramGenerator MPG(&FMM);

        //UPDATE FROM HERE TO THE NEW ONLINE FUNCTIONS:

        throw EImproperCall("this function need to be updated");

        /*        //outputs parameters of the function generateParkProg_online
        bool ParkProgValid;
        TMotionProgram ParkProg;

        //inputs parameters of the function generateParkProg_online
        vector<double> p_1s, p___3s;
        vector<int> Ids;

        //other variables
        int Bid = 0; //block identification
        string filename; //filename of reference including the Bid
        string output_dir; //folder to put the outputss
        string output_path; //output filename each time
        string str; //string to writo to file each time
        OutputsPairPPDP outputs; //structure outputs for print the pair (PP, DP) with commentsa and without FMOSA

        do {
            //contabilize the test and print the test tittle
            append("TEST "+inttostr(++Bid)+":", log_path.c_str());
            append("========================================================================", log_path.c_str());

            //build the outputs directory
            output_dir = "test_generateParkProg_online-"+inttostr(Bid);
            ForceDirectories(AnsiString(output_dir));

            //build the filename of reference
            filename = output_dir+".txt";

            //-------------------------------------------------------------------
            //RANDOMIZE THE POSITION ANGLES WITHOUT COLLISIONS AND SAVE THE ALLOCATIONS:

            //add to the MPG an allocation for each RP of the FMM
            Destroy(MPG);
            for(int i=0; i<FMM.RPL.getCount(); i++)
                MPG.AddAllocation(i);

            //randomize the projection points in the domain of their attached RPs
            FMM.RPL.setPurpose(pGenParPro);
            append("FMM configured for generate a pair (PP, DP). (Purpose = GenParPro).", log_path.c_str());
            MPG.RandomizeWithoutCollision();
            append("Projection points randomized avoinding collisions.", log_path.c_str());

            //WARNING: sentence could take several seconds:
            //  MPG.RandomizeWithoutCollision();

            //save the allocation list
            str = TAllocation::GetIdPPLabelsRow().str;
            str += "\r\n"+MPG.getColumnText().str;
            output_path = output_dir+"/ParkProg_AL_from_"+filename;
            strWriteToFile(output_path, str);
            append("Allocation List saved in file '"+output_path+"'.", log_path.c_str());

            //-------------------------------------------------------------------
            //TEST THE FUNCTION FOR GENERATE PAIRS (PP, DP) ONLINE:

            //move the RPs to the more closer stable point to the projection points
            MPG.MoveToTargetP3();
            if(MPG.getCount() > 0)
                append("RPs moved to observing positions.", log_path.c_str());
            else
                append("There aren't RPs to be moved to observing positions.", log_path.c_str());

            //WARNING: sentence could take several seconds:
            //  MPG.MoveToTargetP3();

            //A PPA list shall be stored how a table (Id, p_1, p___3).

            //captures the starting positions of the RPs in a PPA list
            TPairPositionAnglesList SPL;
            FMM.RPL.getPositions(SPL);
            str = TActuator::getPositionPPALabelsRow().str;
            str += "\r\n"+SPL.getColumnText().str;
            ForceDirectories(AnsiString(output_dir));
            string output_path = output_dir+"/ParkProg_SPL_from_"+filename;
            strWriteToFile(output_path, str);
            append("Starting Position List saved in file '"+output_path+"'.", log_path.c_str());

            //Other whay to obtain the starting position list directly in text format:
            //  FMM.RPL.getPositionsPPATableText()

            //copy the position angles in the input parameters
            p_1s.clear();
            p___3s.clear();
            for(int i=0; i<FMM.RPL.getCount(); i++) {
                TRoboticPositioner *RP = FMM.RPL[i];
                p_1s.push_back(RP->getActuator()->getp_1());
                p___3s.push_back(RP->getActuator()->getArm()->getp___3());
            }
            append("Input parameters copied from FMM to (p_1s, p___3s).", log_path.c_str());

            //move the RPs to their origin positions
            FMM.RPL.moveToOrigins();
            append("RPs moved to origins.", log_path.c_str());

            //Move the FMM to the origins in convenient because this function
            //is for test the generating function.

            //determine a number of RPs to be disabled
            Ids.clear();
            //TBD

            //call the function to test
            append("Calling function generateParkProg_online...", log_path.c_str());
            append("----------------------------------------------------------------", log_path.c_str());
            append("ParkProgValid = generateParkProg_online(outputs_str, FMM,", log_path.c_str());
            append("                                        p_1s, p___3s, RPids, Bid);", log_path.c_str());
            ParkProgValid = generateParkProg_online(outputs_str, FMM,
                                                    p_1s, p___3s, RPids, Bid);
            append("----------------------------------------------------------------", log_path.c_str());
            append("Returned from function generateParkProg_online.", log_path.c_str());

            //WARNING: sentence could take several seconds:
            //  ParkProgValid = generateParkProg_online(outputs_str, FMM,
            //                                          p_1s, p___3s, RPids, bid);

            //restore de enabling status of all RPs
            for(unsigned int i=0; i<Ids.size(); i++) {
                int Id = Ids[i];
                int j = FMM.RPL.searchId(Id);
                if(j >= FMM.RPL.getCount())
                    throw EImpossibleError("lateral effect");
                FMM.RPL[j]->Disabled = false;
            }

            //---------------------------------------------------------------
            //SAVE THE OUTPUTS AND PRINT THE CORRESPONDING MESSAGES:

            //if generation function was successfully generated
            if(ParkProgValid) {
                //indicates the result of the generation
                append("Generated parking program is valid.", log_path.c_str());

                //save the parking program in format FMPT
                str = ParkProg.getText().str;
                output_path = output_dir+"/ParkProg_FMPT_from_"+filename;
                strWriteToFile(output_path, str);
                append("Parking Program in format FMPT saved in file '"+output_path+"'.", log_path.c_str());

                //Given that here the generated pair ((PP, DP) is valid,
                //all operative outsider RPs which aren't obstructed, should be:
                //- in the origin positions, in their final position after execute the DP.

                //get IPL
                TPairPositionAnglesList FPL;
                FMM.RPL.getPositions(FPL);
                append("Final Position List got from the FMM.", log_path.c_str());

                //save IPL
                string str = TActuator::getPositionPPALabelsRow().str;
                str += "\r\n"+FPL.getColumnText().str;
                string output_path = output_dir+"/ParkProg_FPL_from_"+filename;
                strWriteToFile(output_path, str);
                append("Final Position List saved in file '"+output_path+"'.", log_path.c_str());

                //Other whay to obtain the observing position list directly in text format:
                //  FMM.RPL.getPositionsPPATableText()

                //translates the parking program to the format MCS
                //and save it in a file
                ParkProg.getMCStext(str, "depos", Bid, SPL);
                append("Parking program translated to the format of the MCS.", log_path.c_str());
                output_path = output_dir+"/ParkProg_MCS_from_"+filename;
                strWriteToFile(output_path, str);
                append("Parking Program in format MCS saved in file '"+output_path+"'.", log_path.c_str());
            }
            else {
                //Given that here the generated parking program is invalid,
                //all operative outsider RPs which aren't obstructed, should be:
                //- in the first position where the collision was detected.

                //print the result of generation of the parking program
                append("Generated ParkProg is not valid, because PP or DP is invalid.", log_path.c_str());
            }

            //END OF SAVE THE OUTPUTS

            //prepare the standard output for the next test
            append("", log_path.c_str());
        }
        while(ParkProgValid); // && Collided.getCount()<=0 && Obstructed.getCount()<=0);

        //Note that not collided and not obstructed should be part of the stop condition,
        //but they are discarted in the function of generation online.

        //indicates that the test has failed
        append("\r\nFailed test "+inttostr(Bid)+"!", log_path.c_str());
        append("ParkProgValid: "+BoolToStr(ParkProgValid, true).str, log_path.c_str());
        //append("Collided RP Ids: "+Collided.gettText().str, log_path.c_str());
        //append("Obstructed RP Ids: "+Obstructed.gettText().str, log_path.c_str());*/
    }
    catch(Exception& E) {
        E.Message.Insert(1, "testing generateParkProg_online: ");
        throw;
    }
}

//---------------------------------------------------------------------------------
//FUNCTIONS FOR PAIR (PP, DP):
//---------------------------------------------------------------------------------

//Generate a pair (PP, DP) offline.
//Inputs:
//  FMM: the Fiber MOS Model
//  input_path: path of the input file type FMOSA
//  output_dir: path to the output dir where save the output files
//  log_path: file name of the log file where anotate eevents
//Outputs:
//  generatePairPPDP_offline: indicates if the generated pair (PP, DP) is valid
//      and if there aren't neither collided nor obstructed RPs
bool generatePairPPDP_offline(TFiberMOSModel& FMM, const string& input_path, const string& output_dir, const string& log_path,
                              //-------------------------------------------------------------------------------------------------
                              bool saveOPL, bool saveOPL_S0, bool saveOPL_S1,
                              bool saveIPL, bool saveDispCorners1, bool saveDispCorners2,
                              bool savePP_Dmin, bool savePP_Dend, bool saveDP_Dmin, bool saveDP_Dend,
                              bool savePP_FMPT, bool saveDP_FMPT, bool savePP_MCS, bool saveDP_MCS,
                              bool saveMCS, bool saveOutputs, bool saveOtherOutputs)
{
    try {
        //get the list of EnabledNotOperative RPs
        TRoboticPositionerList EnabledNotOperative;
        FMM.RPL.getEnabledNotOperative(EnabledNotOperative);

        //get the list of Dangerous RPs
        TRoboticPositionerList Dangerous;
        FMM.RPL.getDangerous(Dangerous);

        //solve the case with dangerous RPs in the FMM
        if(!saveOutputs && Dangerous.getCount() > 0) {
            append("The FMM Instance contains dangerous RPs, so the pair (PP, DP) will not be generated.", log_path.c_str());
            append("A dangerous RP is an enabled-not-operative RP with fault type dynamic or unknowledge.", log_path.c_str());
            append("You can force generation of the pair (PP, DP) with dangerous RPs, typing the argument outputs.", log_path.c_str());
            append("Files outputs generated by force will contain an initial uncommented text for dificult their execution.", log_path.c_str());
            return false;
        }

        //load the FMOSA from the file input_path
        string str;
        OutputsPairPPDP outputs;
        unsigned int Bid;
        try {
            strReadFromFileWithComments(str, input_path);
            outputs.FMOSA.setTableText(Bid, str);
        } catch(Exception& E) {
            E.Message.Insert(1, "reading FMOSA file: ");
            throw;
        }
        append("FMOSA loaded from file '"+input_path+"'.", log_path.c_str());

        //get the allocation from the FMOSA
        TMotionProgramGenerator MPG(&FMM);
        outputs.FMOSA.getAllocations(MPG);
        append("Allocations got from the FMOSA to MPG.", log_path.c_str());

        //split the path of the file containing the FMOSA
        string dir, filename;
        splitpath(dir, filename, input_path);

        //The filename will be used to attach the outputs filenames witht the input filename.

        //move the RPs to the more closer stable position to the allocated projection points
        //taken into account the limiting case when there aren't allocation
        if(MPG.getCount() > 0) {
            MPG.MoveToTargetP3();
            append("RPs moved to observing positions.", log_path.c_str());
        } else
            append("There aren't RPs to be moved to observing positions.", log_path.c_str());

        //WARNING: sentence could take several seconds:
        //  MPG.MoveToTargetP3();

        //Other way to obtain the more closer stable positions to the allocated projection points,
        //consist in get from the FMOSA the following lists:
        //  the allocation list;
        //  the projection point list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

        //captures the observing positions of the RPs in a PPA list
        TPairPositionAnglesList OPL;
        FMM.RPL.getPositions(OPL);
        append("Observing Position List got from the FMM.", log_path.c_str());

        //A PPA list in format PPA in steps, will be stored how a table (Id, p_1, p___3).

        //captures the corner of arms in the observing positions in cartesian coordinates
        TItemsList<TDoublePoint> OPL_Corners1;
        FMM.RPL.getPositionsCorners1(OPL_Corners1);
        TItemsList<TDoublePoint> OPL_Corners2;
        FMM.RPL.getPositionsCorners2(OPL_Corners2);

        //segregates the enabled outsider RPs
        TRoboticPositionerList Outsiders;
        FMM.RPL.segregateEnabledOutsiders(Outsiders);

        //get the list of colliding items (both EAs and RPs) in text format
        FMM.RPL.setPurpose(pGenPairPPDP);
        string collided_str;
        FMM.getCollidedText(collided_str);

        //Note that MPG.generatePairPPDP will execute automatically:
        //  //configure the Fiber MOS Model for generate a DP
        //  getFiberMOSModel()->RPL.setPurpose(pGenPairPPDP);

        //check the limiting case when there aren't allocations
        if(MPG.getCount() <= 0)
            append("WARNING: there aren't allocations. The generated parking program will be empty.", log_path.c_str());
        //check the limiting case when all operative RPs are in the origin
        else if(FMM.RPL.allOperativeRPsAreInTheOrigin())
            append("WARNING: all operative RPs are in the origin. The generated pair (PP, DP) will be empty.", log_path.c_str());
        //else, check the limiting case when all operative RPs are in security positions
        else if(Outsiders.getCount() <= 0)
            append("WARNING: all operative RPs are in security positions. The generated pair (PP, DP) will contains only a message-instruction list to go to the observing positions and back to the origin.", log_path.c_str());

        //Now are fulfilled the preconditions for the function TMotionProgramGenerator::generatePairPPDP:
        //  All RPs of the Fiber MOS Model:
        //    - shall be in their observing positions.
        //  All RPs of the list Outsiders:
        //    - shall be in the Fiber MOS Model.
        //    - shall be operatives.
        //    - shall be in insecurity positions.
        //    - shall have enabled the quantifiers.
        //    - shall have velocity of rotor 2 approximately double than rotor 1.

        //generates a pair (PP, DP) and determines the collided and obstructed RPs in insecurity positions
        append_without_endl("Generating pair (PP, DP)...", log_path.c_str());
        bool PPvalid = false;
        bool DPvalid = false;
        TRoboticPositionerList Collided;
        TRoboticPositionerList Obstructed;
        TMotionProgram PP;
        TMotionProgram DP;
        MPG.generatePairPPDP(PPvalid, DPvalid, Collided, Obstructed, PP, DP, Outsiders);
        append(" Process finished.", log_path.c_str());

        //Now are fulfilled the postconditions:
        //  All RPs of the Fiber MOS Model:
        //    - will be configured for validate a PP. (Purpose = pValPP).
        //    - will have disabled the quantifiers.
        //  When the generated pair (PP, DP) isn't valid:
        //      All RPs of the FMM:
        //        - will have disabled the quantifiers of their rotors;
        //        - will be in the first position where the collision was detected
        //              during the validation process.
        //  When the generated pair (PP, DP) is valid (even the trivial case):
        //      All RPs of the FMM:
        //        - will have enabled the quantifiers of their rotors;
        //        - will be in their initial positions.

        //########################################################################
        //#WARNING: before re-use a function to generation, shall be restablished#
        //#the preconditions.                                                    #
        //########################################################################

        //solve the case when the generated pair (PP, DP) is not valid
        if(!saveOutputs && (!PPvalid || !DPvalid)) {
            append("The generated pair (PP, DP) is not valid, so none file will be saved.", log_path.c_str());
            append("A pair (PP, DP) is not valid when a collision is detected during the validation process.", log_path.c_str());
            append("You can force generation of the not valid pair (PP, DP), typing the argument outputs.", log_path.c_str());
            append("Files outputs generated by force will contain an initial uncommented text for dificult their execution.", log_path.c_str());
            return false;
        }

        //captures the initial positions of the RPs in a PPA list
        TPairPositionAnglesList IPL;
        FMM.RPL.getPositions(IPL);
        append("Initial Position List got from the FMM.", log_path.c_str());

        //SET VALUES IN THE STRUCTURE OUTPUTS:

        //properties for built comments about file outputs
        outputs.FMOSA_filename = filename;
        outputs.FMPT_version = string(PACKAGE_VERSION);
        outputs.datetime = strfromtime_t(time(NULL), "%Y-%m-%dT%H:%M:%S");
        outputs.EnabledNotOperative = EnabledNotOperative;
        outputs.collided_str = collided_str;
        outputs.Collided = Collided;
        outputs.Obstructed = Obstructed;
        //properties for built comments about each MP
        outputs.PPvalid = PPvalid;
        outputs.DPvalid = DPvalid;
        outputs.DsecMax = 1;
        //properties for built the executable data
        outputs.Bid = Bid;
        outputs.IPL = IPL;
        outputs.PP = PP;
        outputs.OPL = OPL;
        outputs.DP = DP;
        //FMOSA has been set above

        //PRINT THE COMMENTS ABOUT OUTPUTS:

        //print the comments about the outputs
        append("", log_path.c_str());
        append(outputs.getCommentsText(), log_path.c_str());
        append("", log_path.c_str());

        //SAVE FILES:

        //save OPL, if any
        string output_path;
        if(saveOPL) {
            str = TActuator::getPositionPPALabelsRow().str;
            str += "\r\n"+OPL.getColumnText().str;
            ForceDirectories(AnsiString(output_dir));
            output_path = output_dir+"/PairPPDP_OPL_from_"+filename;
            strWriteToFile(output_path, str);
            append("Observing Position List in format PPA in steps saved in file '"+output_path+"'.", log_path.c_str());
        }

        //Other whay to obtain the observing position list directly in text format:
        //  FMM.RPL.getPositionsPPATableText()

        //save OPL in cartesian coordinates respect S0, if any
        if(saveOPL_S0) {
            str = TActuator::getPositionP3LabelsRow().str;
            str += "\r\n";
            str += FMM.RPL.getPositionsP3TableText().str;
            output_path = output_dir+"/PairPPDP_OPL-S0_from_"+filename;
            strWriteToFile(output_path, str);
            append("Observing Position List in format CC respect S0 saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save OPL in cartesian coordinates respect S1, if any
        if(saveOPL_S1) {
            str = TActuator::getPositionP_3LabelsRow().str;
            str += "\r\n";
            str += FMM.RPL.getPositionsP_3TableText().str;
            output_path = output_dir+"/PairPPDP_OPL-S1_from_"+filename;
            strWriteToFile(output_path, str);
            append("Observing Position List in format CC respect S1 saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save IPL, if any
        if(saveIPL) {
            str = TActuator::getPositionPPALabelsRow().str;
            str += "\r\n"+IPL.getColumnText().str;
            if(PPvalid && DPvalid) {
                //warn about invalidity of the generated pair (PP, DP)
                append("Generated pair (PP, DP) have passed the validation process, so the FMM is in the initial position.", log_path.c_str());

                output_path = output_dir + "/PairPPDP_IPL_from_" + filename;
                strWriteToFile(output_path, str);
                append("Initial Position List saved in file '"+output_path+"'.", log_path.c_str());
            } else {
                //warn about invalidity of the generated pair (PP, DP)
                append("Generated pair (PP, DP) have not passed the validation process, so the FMM is in the colliding position, detected during validation process.", log_path.c_str());
                append("Will be saved Colliding Position List instead Initial Position List.", log_path.c_str());

                output_path = output_dir + "/PairPPDP_CPL_from_" + filename;
                strWriteToFile(output_path, str);
                append("Colliding Position List saved in file '"+output_path+"'.", log_path.c_str());
            }
        }

        //save DispCorners1, if any
        if(saveDispCorners1) {
            //captures the corner of arms in the initial positions in cartesian coordinates
            TItemsList<TDoublePoint> IPL_Corners1;
            FMM.RPL.getPositionsCorners1(IPL_Corners1);

            //calculates the displacement of the corner
            TItemsList<TDoublePoint> DispCorners1(OPL_Corners1);
            for(int i=0; i<DispCorners1.getCount(); i++) {
                DispCorners1[i].x -= IPL_Corners1[i].x;
                DispCorners1[i].y -= IPL_Corners1[i].y;
            }

            //DispCorners1.Print = TDoublePoint::PrintRow;
            //str = DispCorners1.getText().str;
            str = "Id\tdx\tdy";
            for(int i=0; i<FMM.RPL.getCount(); i++) {
                str += "\r\n";
                TRoboticPositioner *RP = FMM.RPL[i];
                str += RP->getActuator()->getIdText().str;
                str += "\t";
                str += DispCorners1[i].getRowText().str;
            }
            output_path = output_dir+"/PairPPDP_DispCorners1_from_"+filename;
            strWriteToFile(output_path, str);
            append("Displacement corners 1 saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save DispCorners2, if any
        if(saveDispCorners2) {
            //captures the corner of arms in the initial positions in cartesian coordinates
            TItemsList<TDoublePoint> IPL_Corners2;
            FMM.RPL.getPositionsCorners2(IPL_Corners2);

            //calculates the displacement of the corner
            TItemsList<TDoublePoint> DispCorners2(OPL_Corners2);
            for(int i=0; i<DispCorners2.getCount(); i++) {
                DispCorners2[i].x -= IPL_Corners2[i].x;
                DispCorners2[i].y -= IPL_Corners2[i].y;
            }

            //DispCorners2.Print = TDoublePoint::PrintRow;
            //str = DispCorners2.getText().str;
            str = "Id\tdx\tdy";
            for(int i=0; i<FMM.RPL.getCount(); i++) {
                str += "\r\n";
                TRoboticPositioner *RP = FMM.RPL[i];
                str += RP->getActuator()->getIdText().str;
                str += "\t";
                str += DispCorners2[i].getRowText().str;
            }
            output_path = output_dir+"/PairPPDP_DispCorners2_from_"+filename;
            strWriteToFile(output_path, str);
            append("Displacement corners 2 saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save PP_Dmin, if any
        if(savePP_Dmin) {
            PP.getDminMCStext(str, "pos", Bid);
            output_path = output_dir+"/PairPPDP_PP-Dmin_from_"+filename;
            strWriteToFile(output_path, str);
            append("PP-Dmin saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save PP_Dend, if any
        if(savePP_Dend) {
            PP.getDendMCStext(str, "pos", Bid);
            output_path = output_dir+"/PairPPDP_PP-Dend_from_"+filename;
            strWriteToFile(output_path, str);
            append("PP-Dend saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save DP_Dmin, if any
        if(saveDP_Dmin) {
            DP.getDminMCStext(str, "depos", Bid);
            output_path = output_dir+"/PairPPDP_DP-Dmin_from_"+filename;
            strWriteToFile(output_path, str);
            append("DP-Dmin saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save DP_Dend, if any
        if(saveDP_Dend) {
            DP.getDendMCStext(str, "depos", Bid);
            output_path = output_dir+"/PairPPDP_DP-Dend_from_"+filename;
            strWriteToFile(output_path, str);
            append("DP-Dend saved in file '"+output_path+"'.", log_path.c_str());
        }

        //SAVE FILES ABOUT MPs:

        //save PP_FMPT, if any
        if(savePP_FMPT) {
            str = PP.getText().str;
            output_path = output_dir+"/PairPPDP_PP-FMPT_from_"+filename;
            strWriteToFile(output_path, str);
            append("PP in format FMPT saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save DP_FMPT, if any
        if(saveDP_FMPT) {
            str = DP.getText().str;
            output_path = output_dir+"/PairPPDP_DP-FMPT_from_"+filename;
            strWriteToFile(output_path, str);
            append("DP in format FMPT saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save PP, if any
        if(savePP_MCS) {
            PP.getMCStext(str, "pos", Bid, IPL);
            output_path = output_dir+"/PairPPDP_PP-MCS_from_"+filename;
            strWriteToFile(output_path, str);
            append("PP in format MCS saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save DP, if any
        if(saveDP_MCS) {
            DP.getMCStext(str, "depos", Bid, OPL);
            output_path = output_dir+"/PairPPDP_DP-MCS_from_"+filename;
            strWriteToFile(output_path, str);
            append("DP in format MCS saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save PairPPDP, if any
        if(saveMCS) {
            string PP_str;
            PP.getMCStext(PP_str, "pos", Bid, IPL);
            string DP_str;
            DP.getMCStext(DP_str, "depos", Bid, OPL);
            output_path = output_dir+"/PairPPDP_MCS_from_"+filename;
            strWriteToFile(output_path, PP_str + "\r\n" + DP_str);
            append("Pair (PP, DP) in format MCS saved in file '"+output_path+"'.", log_path.c_str());
        }

        //save file outputs, if any
        bool suitable = outputs.suitable();
        if(suitable) {
            //save outputs in format MCS
            outputs.getText(str);
            output_path = output_dir+"/PairPPDP_outputs_from_"+filename;
            strWriteToFile(output_path, str);
            append("Outputs in format MCS, and FMOSA, saved in file '"+output_path+"'.", log_path.c_str());

            //save outputs in format JSON
            str = outputs.getJSONtext();
            output_path = output_dir + "/PairPPDP_outputs_from_" + stem(filename) + ".json";
            strWriteToFile(output_path, str);
            append("Outputs in format JSON saved in file '"+output_path+"'.", log_path.c_str());
        }
        else { //not suitable
            //if force save outputs:
            if(saveOutputs) {
                //warn about force saving file outputs
                append("WARNING! Forced saving file outputs despite it is not suitable to be executed.", log_path.c_str());
                append("Files outputs generated by force contain an initial uncommented text for dificult their execution.", log_path.c_str());

                //save outputs in format MCS
                outputs.getText(str);
                output_path = output_dir+"/PairPPDP_outputs_from_"+filename;
                strWriteToFile(output_path, str);
                append("Outputs in format MCS saved in file '"+output_path+"'.", log_path.c_str());

                //save outputs in format JSON
                str = outputs.getJSONtext();
                output_path = output_dir + "/PairPPDP_outputs_from_" + stem(filename) + ".json";
                strWriteToFile(output_path, str);
                append("Outputs in format JSON saved in file '"+output_path+"'.", log_path.c_str());
            }
            //if not force save outputs:
            else {
                //warn about force saving file outputs
                append("WARNING! File outputs will not be saved, because it is not suitable to be executed. You can force write outputs typing argument outputs.", log_path.c_str());
            }
        }

        //save other-outputs, if any
        if(saveOtherOutputs) {
            outputs.getOtherText(str);
            output_path = output_dir+"/PairPPDP_other-outputs_from_"+filename;
            strWriteToFile(output_path, str);
            append("Other-outputs saved in file '"+output_path+"'.", log_path.c_str());
        }

        //indicates if the generated pair (PP, DP) is suitable to be executed
        return suitable;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "generating pair (PP, DP) offline: ");
        throw;
    }
}

//Test the function generatePairPPDP_offline.
void test_generatePairPPDP_offline(TFiberMOSModel& FMM, string& log_path)
{
    try {

        //CHECK THE PRECONDITIONS:

        if(FMM.RPL.thereIsSomeNullPointer())
            throw EImproperCall("RP null pointer in the FMM");

        if(FMM.RPL.thereIsSomeRepeatedPointer())
            throw EImproperCall("RP repeated pointer in the FMM");

        if(FMM.RPL.thereIsSomeNotOperative())
            throw EImproperCall("not operative RP in the FMM");

        //MAKE ACTIONS:

        //builds the MPG attached to the FMM
        TMotionProgramGenerator MPG(&FMM);

        //inputs parameters of the function generatePairPPDP_offline
        string input_path; //path of input file
        string output_dir; //folder to put the outputss

        //other variables
        int count = 0; //number os simulations
        string filename; //filename of reference including the number of simulation
        string output_path; //output filename each time
        string str; //string to writo to file each time
        bool valid; //indicates if the pair (PP, DP) is valid and there aren't neither collided nor obstructed RPs

        do {
            //contabilize the test and print the test tittle
            append("TEST "+inttostr(++count)+":", log_path.c_str());
            append("========================================================================", log_path.c_str());

            //initialize the output directory
            output_dir = "test_generatePairPPDP_offline-"+inttostr(count);
            ForceDirectories(AnsiString(output_dir));

            //build the filename of reference
            filename = output_dir+".txt";

            //-------------------------------------------------------------------
            //GENERATES A FILE CONTAINING A RANDOM FMOSA:

            //add to the MPG an allocation for each RP of the FMM
            Destroy(MPG);
            for(int i=0; i<FMM.RPL.getCount(); i++)
                MPG.AddAllocation(i);

            //randomize the projection points in the domain of their attached RPs
            FMM.RPL.setPurpose(pGenPairPPDP);
            append("FMM configured for generate a pair (PP, DP). (Purpose = GenPairPPDP).", log_path.c_str());
            MPG.RandomizeWithoutCollision();
            append("Projection points randomized avoinding collisions.", log_path.c_str());

            //WARNING: sentence could take several seconds:
            //  MPG.RandomizeWithoutCollision();

            //save the allocation list
            str = TAllocation::GetIdPPLabelsRow().str;
            str += "\r\n"+MPG.getColumnText().str;
            output_path = output_dir+"/AL-for-"+filename;
            strWriteToFile(output_path, str);
            append("Allocation List saved in file '"+output_path+"'.", log_path.c_str());

            //build a FMOSA and set the allocations
            TFMOSA FMOSA;
            for(int i=0; i<MPG.getCount(); i++) {
                TAllocation *A = MPG[i];

                TObservingSource *OS = new TObservingSource();

                //transcribe the allocation properties to the OS
                OS->Type = ptSOURCE;
                OS->Pid = A->getRP()->getActuator()->getId();
                OS->X = A->PP.x;
                OS->Y = A->PP.y;
                OS->Enabled = true;

                //set the optional values
                OS->Name = "s:"+inttostr(i+1);
                OS->Mag = 0;
                OS->Pr = 0;
                OS->Bid = 0;
                //OS->notAllocated = true;
                //OS->allocateInAll = false;
                OS->Comment = "too coment";

                //enable the optional values
                OS->there_is_Mag = true;
                OS->there_is_Pr = true;
                OS->there_is_Bid = true;
                //OS->there_is_notAllocated = true;
                //OS->there_is_allocateInAll = true;

                //add the OS to the FMOSA
                FMOSA.Add(OS);
            }

            //save the FMOSA in a file
            FMOSA.getTableText(str);
            output_path = output_dir+"/FMOSA-for-"+filename;
            strWriteToFile(output_path, str);
            append("FMOSA saved in file '"+output_path+"'.", log_path.c_str());

            //-------------------------------------------------------------------
            //TEST THE FUNCTION FOR GENERATE PAIRS (PP, DP) OFFLINE:

            //initializes the parameters of the function:
            Destroy(MPG);
            input_path = output_path;

            //call the function to test
            append("Calling function generatePairPPDP_offline...", log_path.c_str());
            append("--------------------------------------------------------------------", log_path.c_str());
            append("generatePairPPDP_offline(FMM, input_path, output_dir, log_path);", log_path.c_str());
            valid = generatePairPPDP_offline(FMM, input_path, output_dir, log_path,
                                             true, true, true,
                                             true, true, true,
                                             true, true, true, true,
                                             true, true, true, true,
                                             true, true, true);
            append("--------------------------------------------------------------------", log_path.c_str());
            append("Returned from function generatePairPPDP_offline.", log_path.c_str());

            //WARNING: sentence could take several seconds:
            //  generatePairPPDP_offline(FMM, input_path, output_dir, log_path);

            //Note that the outputs are saved in the function to test.

            //prepare the standard output for the next test
            append("", log_path.c_str());
        }
        while(valid);

        //indicates that the test has failed
        append("\r\nFailed test "+inttostr(count)+"!", log_path.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "testing generatePairPPDP_offline: ");
        throw;
    }
}

//Test the function generatePairPPDP_online.
void test_generatePairPPDP_online(TFiberMOSModel& FMM, string& log_path)
{
    try {

        //CHECK THE PRECONDITIONS:

        if(FMM.RPL.thereIsSomeNullPointer())
            throw EImproperCall("RP null pointer in the FMM");

        if(FMM.RPL.thereIsSomeRepeatedPointer())
            throw EImproperCall("RP repeated pointer in the FMM");

        if(FMM.RPL.thereIsSomeOperativeRPwithDynamicFaul())
            throw EImproperCall("operative RP with dynamic fault in the FMM");

        //MAKE ACTIONS:

        //builds the MPG attached to the FMM
        TMotionProgramGenerator MPG(&FMM);

        //UPDATE FROM HERE TO THE NEW ONLINE FUNCTIONS:

        throw EImproperCall("this function need to be updated");

        /*        //outputs parameters of the function generatePairPPDP_online
        bool PairPPDPvalid;
        TMotionProgram PP, DP;

        //inputs parameters of the function generatePairPPDP_online
        vector<double> p_1s, p___3s;
        vector<int> Ids;

        //other variables
        int Bid = 0; //block identification
        string filename; //filename of reference including the Bid
        string output_dir; //folder to put the outputss
        string output_path; //output filename each time
        string str; //string to write to file each time
        OutputsPairPPDP outputs; //structure outputs for print the pair (PP, DP) with comments, without FMOSA

        do {
            //contabilize the test and print the test tittle
            append("TEST "+inttostr(++Bid)+":", log_path.c_str());
            append("========================================================================", log_path.c_str());

            //build the output directory
            output_dir = "test_generatePairPPDP_online-"+inttostr(Bid);
            ForceDirectories(AnsiString(output_dir));

            //build the filename of reference
            filename = output_dir+".txt";

            //-------------------------------------------------------------------
            //RANDOMIZE THE POSITION ANGLES WITHOUT COLLISIONS AND SAVE THE ALLOCATIONS:

            //add to the MPG an allocation for each RP of the FMM
            Destroy(MPG);
            for(int i=0; i<FMM.RPL.getCount(); i++)
                MPG.AddAllocation(i);

            //randomize the projection points in the domain of their attached RPs
            FMM.RPL.setPurpose(pGenPairPPDP);
            append("FMM configured for generate a pair (PP, DP). (Purpose = GenPairPPDP).", log_path.c_str());
            MPG.RandomizeWithoutCollision();
            append("Projection points randomized avoinding collisions.", log_path.c_str());

            //WARNING: sentence could take several seconds:
            //  MPG.RandomizeWithoutCollision();

            //save the allocation list
            str = TAllocation::GetIdPPLabelsRow().str;
            str += "\r\n"+MPG.getColumnText().str;
            output_path = output_dir+"/PairPPDP_AL_from_"+filename;
            strWriteToFile(output_path, str);
            append("Allocation List saved in file '"+output_path+"'.", log_path.c_str());

            //-------------------------------------------------------------------
            //TEST THE FUNCTION FOR GENERATE PAIRS (PP, DP) ONLINE:

            //move the RPs to the more closer stable point to the projection points
            //taken into account the trivial case when there aren't allocations
            MPG.MoveToTargetP3();
            if(MPG.getCount() > 0)
                append("RPs moved to observing positions.", log_path.c_str());
            else
                append("There aren't RPs to be moved to observing positions.", log_path.c_str());

            //WARNING: sentence could take several seconds:
            //  MPG.MoveToTargetP3();

            //A PPA list in format PPA is stored how a table (Id, p_1, p___3).

            //captures the observing positions of the RPs in a PPA list
            TPairPositionAnglesList OPL;
            FMM.RPL.getPositions(OPL);
            str = TActuator::getPositionPPALabelsRow().str;
            str += "\r\n"+OPL.getColumnText().str;
            ForceDirectories(AnsiString(output_dir));
            string output_path = output_dir+"/PairPPDP_OPL_from_"+filename;
            strWriteToFile(output_path, str);
            append("Observing Position List saved in file '"+output_path+"'.", log_path.c_str());

            //Other whay to obtain the observing position list directly in text format:
            //  FMM.RPL.getPositionsPPATableText()

            //copy the position angles in the input parameters
            p_1s.clear();
            p___3s.clear();
            for(int i=0; i<FMM.RPL.getCount(); i++) {
                TRoboticPositioner *RP = FMM.RPL[i];
                p_1s.push_back(RP->getActuator()->getp_1());
                p___3s.push_back(RP->getActuator()->getArm()->getp___3());
            }
            append("Input parameters extracted from the FMM.", log_path.c_str());

            //move the RPs to their origin positions
            FMM.RPL.moveToOrigins();
            append("RPs moved to origins.", log_path.c_str());

            //Move the FMM to the origins in convenient because this function
            //is for test the generating function.

            //determine the RPs to be disabled
            Ids.clear();
            //TBD

            //call the function to test
            append("Calling function generatePairPPDP_online...", log_path.c_str());
            append("----------------------------------------------------------------", log_path.c_str());
            append("PairPPDPvalid = generatePairPPDP_online(outputs_str, FMM,", log_path.c_str());
            append("                                        p_1s, p___3s, RPids, Bid);", log_path.c_str());
            PairPPDPvalid = generatePairPPDP_online(outputs_str, FMM,
                                                    p_1s, p___3s, RPids, Bid);
            append("----------------------------------------------------------------", log_path.c_str());
            append("Returned from function generatePairPPDP_online.", log_path.c_str());

            //WARNING: sentence could take several seconds:
            //  PairPPDPvalid = generatePairPPDP_online(outputs_str, FMM,
            //                                          p_1s, p___3s, RPids, Bid);

            //restore de enabling status of all RPs
            for(unsigned int i=0; i<Ids.size(); i++) {
                int Id = Ids[i];
                int j = FMM.RPL.searchId(Id);
                if(j >= FMM.RPL.getCount())
                    throw EImpossibleError("lateral effect");
                FMM.RPL[j]->Disabled = false;
            }

            //---------------------------------------------------------------
            //SAVE THE OUTPUTS AND PRINT THE CORRESPONDING MESSAGES:

            //Note that since there aren't neither Collided nor Obstructed list, it is not possible do:
            //  //Warn about either collided or obstructed RPs.
            //  if(Collided.getCount() > 0 || Obstructed.getCount() > 0) {
            //      if(Collided.getCount() > 0) {
            //          append("WARNING! There are collided items:", log_path.c_str());
            //          append("Collided items: "+collided_str, log_path.c_str());
            //          append("Collided RP Ids: "+Collided.getText().str, log_path.c_str());
            //      } if(Obstructed.getCount() > 0) {
            //          append("WARNING! There are obstructed RPs:", log_path.c_str());
            //          append("Obstructed RP Ids: "+Obstructed.getText().str, log_path.c_str());
            //      }
            //  } else
            //      append("There aren't neither collided nor obstructed RPs.", log_path.c_str());

            //if the motion program was successfully generated
            if(PairPPDPvalid) {
                //indicates the result of the generation
                append("Generated pair (PP, DP) is valid.", log_path.c_str());

                //Given that here the generated pair (PP, DP) is valid,
                //all operative outsider RPs which aren't obstructed, should be:
                //- in the origin positions.
                //Because function TMotionProgramGenerator::generatePairPPDP,
                //test first the DP and after the PP, but at the end
                //move all RPs to their initial positions.

                //get the IPL
                TPairPositionAnglesList IPL;
                FMM.RPL.getPositions(IPL);
                append("Initial Position List got from the FMM.", log_path.c_str());

                //save the IPL
                string str = TActuator::getPositionPPALabelsRow().str;
                str += "\r\n"+IPL.getColumnText().str;
                string output_path = output_dir+"/PairPPDP_IPL_from_"+filename;
                strWriteToFile(output_path, str);
                append("Initial Position List saved in file '"+output_path+"'.", log_path.c_str());

                //Other whay to obtain the observing position list directly in text format:
                //  FMM.RPL.getPositionsPPATableText()

                //save the PP in format FMPT
                str = PP.getText().str;
                output_path = output_dir+"/PairPPDP_PP-FMPT_from_"+filename;
                strWriteToFile(output_path, str);
                append("PP in format FMPT saved in file '"+output_path+"'.", log_path.c_str());

                //save the DP in format FMPT
                str = DP.getText().str;
                output_path = output_dir+"/PairPPDP_DP-FMPT_from_"+filename;
                strWriteToFile(output_path, str);
                append("DP in format FMPT saved in file '"+output_path+"'.", log_path.c_str());

                //save the PP in format MCS
                PP.getMCStext(str, "pos", Bid, IPL);
                append("Positiong program translated to the format of the MCS.", log_path.c_str());
                output_path = output_dir+"/PairPPDP_PP-MCS_from_"+filename;
                strWriteToFile(output_path, str);
                append("PP in format MCS saved in file '"+output_path+"'.", log_path.c_str());

                //save the DP in format MCS
                DP.getMCStext(str, "depos", Bid, OPL);
                append("Depositiong program translated to the format of the MCS.", log_path.c_str());
                output_path = output_dir+"/PairPPDP_DP-MCS_from_"+filename;
                strWriteToFile(output_path, str);
                append("DP in format MCS saved in file '"+output_path+"'.", log_path.c_str());

                //set values in the structure outputs:

                //properties for built comments about file outputs
                outputs.FMOSA_filename = filename;
                outputs.FMPT_version = string(PACKAGE_VERSION);
                outputs.datetime = strfromtime_t(time(NULL), "%Y-%m-%dT%H:%M:%S");
                //outputs.collided_str = collided_str;
                //outputs.Collided = Collided;
                //outputs.Obstructed = Obstructed;
                //properties for built comments about each MP
                outputs.PPvalid = PairPPDPvalid;
                outputs.DPvalid = PairPPDPvalid;
                //properties for built the executable data
                outputs.Bid = Bid;
                outputs.IPL = IPL;
                outputs.PP = PP;
                outputs.OPL = OPL;
                outputs.DP = DP;
                outputs.FMOSA.Clear(); //there isn't a FMOSA for set

                //save outputs in format MCS
                outputs.getText(str, Bid, OPL, IPL, true);
                output_path = output_dir+"/PairPPDP_outputs_from_"+filename;
                strWriteToFile(output_path, str);
                append("Pair (PP, DP) in format MCS, saved in file '"+output_path+"'.", log_path.c_str());

                //save outputs in format JSON
                str = outputs.getJSONtext();
                output_path = output_dir + "/PairPPDP_outputs_from_" + stem(filename) + ".json";
                strWriteToFile(output_path, str);
                append("Pair (PP, DP) in format JSON saved in file '"+output_path+"'.", log_path.c_str());
            }
            else {
                //Given that here the generated pair (PP, DP) is invalid,
                //all operative outsider RPs which aren't obstructed, should be:
                //- in the first position where the collision was detected.
                //During the test of DP or the test of PP.

                //print the result of generation of the PP
                append("Generated Pair (PP, DP) is not valid, because either PP or DP is invalid.", log_path.c_str());
            }

            //END OF SAVE THE OUTPUTS

            //prepare the standard output for the next test
            append("", log_path.c_str());
        }
        while(PairPPDPvalid); //&& Collided.getCount()<=0 && Obstructed.getCount()<=0);

        //Note that not collided and not obstructed should be part of the stop condition,
        //but they are discarted in the online generation function.

        //indicates that the test has failed
        append("\r\nFailed test "+inttostr(Bid)+"!", log_path.c_str());*/
    }
    catch(Exception& E) {
        E.Message.Insert(1, "testing generatePairPPDP_online: ");
        throw;
    }
}

//Determine if all RPs included in the pair (PP, DP), are enabled.
//The instance of the Fiber MOS Model will not be written.
void checkPairPPDP(TFiberMOSModel& FMM, string& PP_path, string& DP_path, string& log_path)
{
    try {

        //LOAD SETTINGS FROM FILES:

        //load the PP from a file
        TMotionProgram PP;
        string PP_label;
        string str;
        strReadFromFile(str, PP_path);
        unsigned int PP_Bid;
        PP.setMCStext(PP_label, PP_Bid, str);
        append("PP loaded from file '"+PP_path+"'.", log_path.c_str());

        //check the precondition
        if(PP_label != "pos")
            throw EImproperArgument("PP label should be \"pos\"");

        //load the DP from a file
        TMotionProgram DP;
        string DP_label;
        //str.clear();
        strReadFromFile(str, DP_path);
        unsigned int DP_Bid;
        DP.setMCStext(DP_label, DP_Bid, str);
        append("DP loaded from file '"+DP_path+"'.", log_path.c_str());

        //check the preconditions
        if(DP_label != "depos")
            throw EImproperArgument("DP label should be \"depos\"");
        if(DP_Bid != PP_Bid)
            throw EImproperArgument("PP Bid should be equal to DP Bid");

        //build the MPV attached to the FMM
        TMotionProgramGenerator MPV(&FMM);

        //-------------------------------------------------------------------
        //MAKE THE OPERATIONS:

        //check the pair (PP, DP)
        bool valid = MPV.checkPairPPDP(PP, DP);

        //print the result
        if(valid)
            append("All RPs included in the pair (PP, DP) are operatives.", log_path.c_str());
        else
            append("Not all RPs included in the pair (PP, DP) are operatives.", log_path.c_str());

        //split the path of the file containing the PP
        string PP_dir, PP_filename;
        splitpath(PP_dir, PP_filename, PP_path);

        //The filename will be used to attach the outputs filenames witht the input filename.

        //split the path of the file containing the DP
        string DP_dir, DP_filename;
        splitpath(DP_dir, DP_filename, DP_path);

        //The filename will be used to attach the outputs filenames witht the input filename.
    }
    catch(Exception& E) {
        E.Message.Insert(1, "checking pair (PP, DP): ");
        throw;
    }
}

//Check if a pair (PP, DP) avoid collisions,
//wehn it is executed starting from the origin.
void validatePairPPDP(TFiberMOSModel& FMM, string& outputs_path, string output_dir, string& log_path)
{
    try {
        //LOAD SETTINGS FROM FILES:

        //load the Outputs structure from a file
        string str;         //auxiliary string
        OutputsPairPPDP outputs;    //outputs structure
        try {
            //load the outputs structure
            strReadFromFile(str, outputs_path);
            outputs.setText(str);

        } catch(Exception& E) {
            E.Message.Insert(1, "reading FMOSA file: ");
            throw;
        }
        append("Structure outputs loaded from file '"+outputs_path+"'.", log_path.c_str());

        //Now the PP and the DP will be validated.

        //---------------------------------------------
        //MAKE THE OPERATIONS:

        //split the path of the file containing the FMOSA
        string dir, filename;
        splitpath(dir, filename, outputs_path);
        string output_path; //auxiliary string

        //The filename will be used to attach the outputs filenames witht the input filename.

        //built a MPV attached to the FMM
        TMotionProgramValidator MPV(&FMM);

        //configure the SPM of all RPs for validate the PP and validate it
        FMM.RPL.setPurpose(pValPP);
        append("FMM configured for validate a PP. (Purpose = ValPP).", log_path.c_str());
        bool PPvalid = MPV.validateMotionProgram(outputs.PP);

        //initalize the flags for indicate the validity of the DP
        bool DPvalid = false;

        //if the PP avoid collisions
        if(PPvalid) {
            //indicates the result of the validation
            append("The PP is valid.", log_path.c_str());

            //Given that here the PP is valid,
            //all operative outsider RPs which aren't obstructed, can be:
            //- in the observing positions, in their final position after execute the PP.

            //A PPA list shall be stored how a table (Id, p_1, p___3).

            //captures the observing positions of the RPs in a PPA list
            TPairPositionAnglesList OPL;
            FMM.RPL.getPositions(OPL);
            str = TActuator::getPositionPPALabelsRow().str;
            str += "\r\n"+OPL.getColumnText().str;
            ForceDirectories(AnsiString(outputs_path));
            output_path = output_dir+"/PairPPDP_OPL_from_"+filename;
            strWriteToFile(output_path, str);
            append("Observing Position List saved in file '"+output_path+"'.", log_path.c_str());

            //Other whay to obtain the observing position list directly in text format:
            //  FMM.RPL.getPositionsPPATableText()

            //captures the observing positions of the RPs in cartesian coordinates respect S0
            str = TActuator::getPositionP3LabelsRow().str;
            str += "\r\n";
            str += FMM.RPL.getPositionsP3TableText().str;
            output_path = output_dir+"/PairPPDP_OPL-S0_from_"+filename;
            strWriteToFile(output_path, str);
            append("Observing Position List in format CC respect S0 saved in file '"+output_path+"'.", log_path.c_str());

            //captures the observing positions of the RPs in cartesian coordinates respect S1
            str = TActuator::getPositionP_3LabelsRow().str;
            str += "\r\n";
            str += FMM.RPL.getPositionsP_3TableText().str;
            output_path = output_dir+"/PairPPDP_OPL-S1_from_"+filename;
            strWriteToFile(output_path, str);
            append("Observing Position List in format CC respect S1 saved in file '"+output_path+"'.", log_path.c_str());

            //captures the corner of arms in the observing positions in cartesian coordinates
            TItemsList<TDoublePoint> OPL_Corners1;
            FMM.RPL.getPositionsCorners1(OPL_Corners1);
            TItemsList<TDoublePoint> OPL_Corners2;
            FMM.RPL.getPositionsCorners2(OPL_Corners2);

            //save PP-Dmin in a file
            outputs.PP.getDminMCStext(str, "pos", outputs.Bid);
            output_path = output_dir+"/PairPPDP_PP-Dmin_from_"+filename;
            strWriteToFile(output_path, str);
            append("PP-Dmin saved in file '"+output_path+"'.", log_path.c_str());

            //save PP-Dend in a file
            outputs.PP.getDendMCStext(str, "pos", outputs.Bid);
            output_path = output_dir+"/PairPPDP_PP-Dend_from_"+filename;
            strWriteToFile(output_path, str);
            append("PP-Dend saved in file '"+output_path+"'.", log_path.c_str());

            //configure the SPM for validate the PP and validate it
            FMM.RPL.setPurpose(pValDP);
            append("FMM configured for validate a DP. (Purpose = ValDP).", log_path.c_str());
            DPvalid = MPV.validateMotionProgram(outputs.DP);

            //SAVE THE OUTPUTS AND PRINT THE CORRESPONDING MESSAGES:

            //if the pair (PP, DP) is valid
            if(DPvalid) {
                //indicates the result of the validation
                append("The DP is valid.", log_path.c_str());

                //Given that here the pair (PP, DP) is valid,
                //all operative outsider RPs which aren't obstructed, can be:
                //- in the origin positions, in their final position after execute the DP.

                //captures the initial positions of the RPs in a PPA list
                TPairPositionAnglesList IPL;
                FMM.RPL.getPositions(IPL);
                string str = TActuator::getPositionPPALabelsRow().str;
                str += "\r\n"+IPL.getColumnText().str;
                string output_path = output_dir+"/PairPPDP_IPL_from_"+filename;
                strWriteToFile(output_path, str);
                append("Initial Position List saved in file '"+output_path+"'.", log_path.c_str());

                //Other whay to obtain the initial position list directly in text format:
                //  FMM.RPL.getPositionsPPATableText()

                //captures the corner of arms in the initial positions in cartesian coordinates
                TItemsList<TDoublePoint> IPL_Corners1;
                FMM.RPL.getPositionsCorners1(IPL_Corners1);
                TItemsList<TDoublePoint> IPL_Corners2;
                FMM.RPL.getPositionsCorners2(IPL_Corners2);

                //calculates the displacement of each corner
                TItemsList<TDoublePoint> DispCorners1(OPL_Corners1);
                for(int i=0; i<DispCorners1.getCount(); i++) {
                    DispCorners1[i].x -= IPL_Corners1[i].x;
                    DispCorners1[i].y -= IPL_Corners1[i].y;
                }
                TItemsList<TDoublePoint> DispCorners2(OPL_Corners2);
                for(int i=0; i<DispCorners2.getCount(); i++) {
                    DispCorners2[i].x -= IPL_Corners2[i].x;
                    DispCorners2[i].y -= IPL_Corners2[i].y;
                }

                //save the displacements in text format
                //            DispCorners1.Print = TDoublePoint::PrintRow;
                //          str = DispCorners1.getText().str;
                str = "Id\tdx\tdy";
                for(int i=0; i<FMM.RPL.getCount(); i++) {
                    str += "\r\n";
                    TRoboticPositioner *RP = FMM.RPL[i];
                    str += RP->getActuator()->getIdText().str;
                    str += "\t";
                    str += DispCorners1[i].getRowText().str;
                }
                output_path = output_dir+"/PairPPDP_DispCorners1_from_"+filename;
                strWriteToFile(output_path, str);
                append("Displacement corners 1 saved in file '"+output_path+"'.", log_path.c_str());
                //            DispCorners2.Print = TDoublePoint::PrintRow;
                //          str = DispCorners2.getText().str;
                str = "Id\tdx\tdy";
                for(int i=0; i<FMM.RPL.getCount(); i++) {
                    str += "\r\n";
                    TRoboticPositioner *RP = FMM.RPL[i];
                    str += RP->getActuator()->getIdText().str;
                    str += "\t";
                    str += DispCorners2[i].getRowText().str;
                }
                output_path = output_dir+"/PairPPDP_DispCorners2_from_"+filename;
                strWriteToFile(output_path, str);
                append("Displacement corners 2 saved in file '"+output_path+"'.", log_path.c_str());

                //save DP-Dmin in a file
                outputs.DP.getDminMCStext(str, "depos", outputs.Bid);
                output_path = output_dir+"/PairPPDP_DP-Dmin_from_"+filename;
                strWriteToFile(output_path, str);
                append("DP-Dmin saved in file '"+output_path+"'.", log_path.c_str());

                //save DP-Dend in a file
                outputs.DP.getDendMCStext(str, "depos", outputs.Bid);
                output_path = output_dir+"/PairPPDP_DP-Dend_from_"+filename;
                strWriteToFile(output_path, str);
                append("DP-Dend saved in file '"+output_path+"'.", log_path.c_str());

                //set values in the structure outputs:

                //properties for built comments about file outputs
                outputs.FMOSA_filename = filename;
                outputs.FMPT_version = string(PACKAGE_VERSION);
                outputs.datetime = strfromtime_t(time(NULL), "%Y-%m-%dT%H:%M:%S");
                outputs.collided_str = ""; //outputs.collided_str not set above
                outputs.Collided.Clear(); //outputs.Collided not set above
                outputs.Obstructed.Clear(); //outputs.Obstructed not set above
                //properties for built comments about each MP
                outputs.PPvalid = PPvalid;
                outputs.DPvalid = DPvalid;
                //properties for built the executable data
                //outputs.Bid set above
                outputs.IPL = IPL;
                //outputs.PP set above
                outputs.OPL = OPL;
                //outputs.DP set avove
                //outputs.FMOSA set above

                //save outputs in format MCS
                outputs.getText(str, true);
                output_path = output_dir+"/PairPPDP_outputs_from_"+filename;
                strWriteToFile(output_path, str);
                append("Pair (PP, DP) in format MCS saved in file '"+output_path+"'.", log_path.c_str());

                //save outputs in format JSON
                str = outputs.getJSONtext();
                output_path = output_dir + "/PairPPDP_outputs_from_" + stem(filename) + ".json";
                strWriteToFile(output_path, str);
                append("PairPPDP in format JSON saved in file '"+output_path+"'.", log_path.c_str());
            }
        }

        //if the pair (PP, DP) is not valid
        if(!PPvalid || !DPvalid) {
            //Given that here the generated pair (PP, DP) is invalid,
            //all operative outsider RPs which aren't obstructed, can be:
            //- in the first position where the collision was detected.

            //warn about invalidity of generated RPs
            if(!PPvalid && DPvalid)
                append("The PP is invalid and the DP is valid.", log_path.c_str());
            else if(PPvalid && !DPvalid)
                append("The PP is valid and the DP is invalid.", log_path.c_str());
            else if(!PPvalid && !DPvalid)
                append("Both the PP and the DP are invalid.", log_path.c_str());
            else
                throw EImpossibleError("lateral effect");
            append("Due to the pair (PP, DP) is invalid, the FMM is in their colliding position, detected during validation process.", log_path.c_str());
        }
    }
    catch(Exception& E) {
        E.Message.Insert(1, "validating pair (PP, DP) offline: ");
        throw;
    }
    catch(...) {
        throw;
    }
}

//Attempt regenerate a pair (PP, DP).
//The instance of the Fiber MOS Model will not be written.
void regeneratePairPPDP_offline(TFiberMOSModel& FMM, string& PP_path, string& DP_path, string& FMOSA_path, string& log_path)
{
    try {

        //LOADS SETTINGS FROM FILES:

        //load the PP from a file
        TMotionProgram PP;
        string PP_label;
        unsigned int PP_Bid;
        string str;
        strReadFromFile(str, PP_path);
        PP.setMCStext(PP_label, PP_Bid, str);
        append("PP loaded from file '"+PP_path+"'.", log_path.c_str());

        //check the precondition
        if(PP_label != "pos")
            throw EImproperArgument("PP label should be \"pos\"");

        //load the DP from a file
        TMotionProgram DP;
        string DP_label;
        unsigned int DP_Bid;
        strReadFromFile(str, DP_path);
        DP.setMCStext(DP_label, DP_Bid, str);
        append("DP loaded from file '"+DP_path+"'.", log_path.c_str());

        //check the precondition
        if(DP_label != "depos")
            throw EImproperArgument("DP label should be \"depos\"");
        if(DP_Bid != PP_Bid)
            throw EImproperArgument("DP Bid should be equal to PP Bid");

        //load the FMOSA from the file FMOSA_path
        TFMOSA FMOSA;
        unsigned int FMOSA_Bid;
        strReadFromFileWithComments(str, FMOSA_path);
        FMOSA.setTableText(FMOSA_Bid, str);
        append("FMOSA loaded from file '"+FMOSA_path+"'.", log_path.c_str());

        //check the precondition
        if(FMOSA_Bid!=PP_Bid || FMOSA_Bid!=DP_Bid)
            throw EImproperArgument("FMOSA Bid should be equal to PP Bid and DP Bid");

        //get the data from the FMOSA
        TMotionProgramGenerator MPG(&FMM);
        FMOSA.getAllocations(MPG);
        append("Allocations got from FMOSA to FMM.", log_path.c_str());

        //-------------------------------------------------------------------
        //MAKE THE OPERATIONS:

        //split the path of the file containing the PP table
        string dir, filename;
        splitpath(dir, filename, FMOSA_path);

        //The filename will be used to attach the outputs filenames witht the input filename.

        //get the initial position list
        TPairPositionAnglesList IPL;
        FMM.RPL.getPositions(IPL);
        append("Initial Position List got from the Fiber MOS Model.", log_path.c_str());

        //The IPL is useful to get the PP in the interface format.

        //move the RPs to the more closer stable point to the TPs
        //taken into account the trivial case when there aren't allocations
        if(MPG.getCount() > 0) {
            MPG.MoveToTargetP3();
            append("RPs moved to observing positions.", log_path.c_str());
        } else
            append("There aren't RPs to be moved to observing positions.", log_path.c_str());

        //WARNING: sentence could take several seconds:
        //  MPG.MoveToTargetP3();

        //Other way to obtain the more closer stablepoints to the projection points,
        //consist in get from the FMOSA the following lists:
        //  the allocation list;
        //  the projection point list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

        //A PPA list in format PPA is a table (Id, p_1, p___3).

        //get the observing position list
        TPairPositionAnglesList OPL;
        FMM.RPL.getPositions(OPL);
        append("Observing Position List got from the Fiber MOS Model.", log_path.c_str());

        //The OPL is useful to get the DP in the interface format.

        //attempt regenerate the pair (PP, DP)
        TVector<int> Excluded;
        bool regenerated = MPG.attemptRegenerate(Excluded, PP, DP);

        string output_path;

        //print the result
        if(regenerated) {
            string aux = "# The pair (PP, DP) has been regenerated excluding the following RPs:";
            aux += "\r\n# Excluded: " + Excluded.getText().str;
            append(aux, log_path.c_str());

            //save the regenerated PP in format MCS
            PP.getMCStext(str, "pos", PP_Bid, IPL);
            output_path = "PairPPDP_regenerated-PP-MCS_from_"+filename;
            strWriteToFile(output_path, aux + "\r\n" + str);
            append("Regenerated PP in format MCS saved in file '"+output_path+"'.", log_path.c_str());

            //save the regenerated DP in format MCS
            DP.getMCStext(str, "depos", DP_Bid, OPL);
            output_path = "PairPPDP_regenerated-DP-MCS_from_"+filename;
            strWriteToFile(output_path, aux + "\r\n" + str);
            append("Regenerated DP in format MCS saved in file '"+output_path+"'.", log_path.c_str());
        }
        else
            append("The pair (PP, DP) can not be regenerated.", log_path.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "regenerating pair (PP, DP): ");
        throw;
    }
}

//---------------------------------------------------------------------------------
//MAIN FUNCTION:
//---------------------------------------------------------------------------------

//get arguments for a command
void getArguments(int& argc, char* argv[], const string& command)
{
    string *arg1 = new string(command);
    argv[1] = (char*)arg1->c_str();

    if(command == "help") {
        argc = 2;
    }
    else if(command == "help_generatePairPPDP_offline") {
        argc = 2;
    }
    else if(command == "help_generateParkProg_offline") {
        argc = 2;
    }
    else if(command == "help_others") {
        argc = 2;
    }
    else if(command == "aboutOf") {
        argc = 2;
    }
    else if(command == "valuesSPM_EA") {
        argc = 3;
        string *arg2 = new string("1");
        argv[2] = (char*)arg2->c_str();
    }
    else if(command == "valuesSPM_RP") {
        argc = 3;
        string *arg2 = new string("1");
        argv[2] = (char*)arg2->c_str();
    }
    else if(command == "testRadialMotion") {
        argc = 2;
    }
    else if(command == "testDistanceMin") {
        argc = 2;
    }
    else if(command == "applyPCL") {
        argc = 3;
        string *arg2 = new string(getCurrentDir()+"/../megarafmpt/data/Models/positionerCenters_theoretical_100RPs.txt");
        argv[2] = (char*)arg2->c_str();
    }
    else if(command == "applyRPI") {
        argc = 3;
        string *arg2 = new string(getCurrentDir()+"/../megara-fmpt/data/Models/MEGARA_RP_Instance");
        argv[2] = (char*)arg2->c_str();
    }
    else if(command == "PPA_to_CC-S0") {
        argc = 3;
        string *arg2 = new string("/home/user/pruebas PPA/PPAL.txt");
        argv[2] = (char*)arg2->c_str();
    }
    else if(command == "evaluateErrors") {
        argc = 3;
        string *arg2 = new string("/home/user/pruebas Measures/Medidas1.txt");
        argv[2] = (char*)arg2->c_str();
    }
    else if(command == "tuneRP") {
        argc = 5;
        string *arg2 = new string("/home/user/pruebas Measures/Medidas1.txt");
        argv[2] = (char*)arg2->c_str();
        string *arg3 = new string("/home/user/pruebas Measures/errors_from_Medidas1.txt");
        argv[3] = (char*)arg3->c_str();
        string *arg4 = new string("154");
        argv[4] = (char*)arg4->c_str();
    }
    else if(command == "applyTuning") {
        argc = 3;
        string *arg2 = new string("/home/user/pruebas Measures/fine-tuned-parameters-154_from_Medidas1.txt");
        argv[2] = (char*)arg2->c_str();
    }
    else if(command == "generateParkProg_offline") {
        argc = 4;
        string *arg2 = new string("/home/user/MEGARA/ejemplos/ejemplo_22-02-2017/megara_2p0e5_000917.txt");
        argv[2] = (char*)arg2->c_str();
        //-----------------------------
        string *arg3 = new string("all");
        argv[3] = (char*)arg3->c_str();
    }
    else if(command == "test_generateParkProg_offline") {
        argc = 2;
    }
    else if(command == "test_generateParkProg_online") {
        argc = 2;
    }
    else if(command == "generatePairPPDP_offline") {
        argc = 4;
//        string *arg2 = new string("/home/user/MEGARA/ejemplos/ejemplo_22-02-2017/megara_2p0e5_000917.txt");
        string *arg2 = new string("/home/user/posible BUG 10-05-2017/megara_2p0e5_000007_without_26_68_86.txt");
        argv[2] = (char*)arg2->c_str();
        //-----------------------------
        string *arg3 = new string("all");
        argv[3] = (char*)arg3->c_str();
    }
    else if(command == "test_generatePairPPDP_offline") {
        argc = 2;
    }
    else if(command == "test_generatePairPPDP_online") {
        argc = 2;
    }
    else if(command == "checkPairPPDP") {
        argc = 5;
        string *arg2 = new string(getCurrentDir()+"/PairPPDP_PP-MCS_from_megara-cb_90sources.txt");
        argv[2] = (char*)arg2->c_str();
        string *arg3 = new string(getCurrentDir()+"/PairPPDP_DP-MCS_from_megara-cb_90sources.txt");
        argv[3] = (char*)arg3->c_str();
        string *arg4 = new string("{1, 3, 4}");
        argv[4] = (char*)arg4->c_str();
    }
    else if(command == "validatePairPPDP") {
        argc = 3;
        //string *arg2 = new string(getCurrentDir()+"/PairPPDP_outputs_from_megara-cb0_RP1.txt");
        string *arg2 = new string(getCurrentDir()+"../../pruebas_choque/margin_3p50/PairPPDP_outputs_from_megara_5p0e5_x1.txt");
        argv[2] = (char*)arg2->c_str();
    }
    else if(command == "regeneratePairPPDP") {
        argc = 6;
        string *arg2 = new string(getCurrentDir()+"/PairPPDP_PP-MCS_from_megara-cb_90sources.txt");
        argv[2] = (char*)arg2->c_str();
        string *arg3 = new string(getCurrentDir()+"/PairPPDP_DP-MCS_from_megara-cb_90sources.txt");
        argv[3] = (char*)arg3->c_str();
        string *arg4 = new string(getCurrentDir()+"/../megarafmpt/data/Samples/megara-cb_90sources.txt");
        argv[4] = (char*)arg4->c_str();
        string *arg5 = new string("{1, 3, 4}");
        argv[5] = (char*)arg5->c_str();
    }
    else
        throw EImproperArgument("unknown command: " + command);
}

//main function
int main(int argc, char *argv[])
{
    //-----------------------------------------------------------------------
    //BUILD THE PATH FOR THE INSTANCE OF THE Fiber MOS Model:

    //###################################################################
    //Using autotools over Linux:
    //-------------------------------------------------------------------
    //
    //Selecting '/home/Isaac/MEGARA' how working directory, before compilation:
    //  sources shall be in:    '/home/Isaac/MEGARA/megarafmpt/src'
    //  data shall be in:       '/home/Isaac/MEGARA/megarafmpt/data'
    //
    //When compilation has been make in the same directory named megarafmpt:
    //  sources will be in:     '/home/Isaac/MEGARA/megarafmpt/src'
    //  data will be in:        '/home/Isaac/MEGARA/megarafmpt/data'
    //
    //When compilation has been make in an appart directory named build:
    //  sources will be in:     '/home/Isaac/MEGARA/build/src'
    //  data will be in:        '/home/Isaac/MEGARA/build/data'
    //
    //When the installation has been make:
    //  executable will be in:  'prefix/bin'
    //  data will be in:        'prefix/share/megara-fmpt'
    //Where prefix is probably '/usr/local'.
    //
    //Then the program fmpt_saa compiled with autotools, shall be search the data in the following paths:
    //  The Fiber MOS Model Instance:
    //      option 1, when program is installed:    DATADIR+"/Models/MEGARA_FiberMOSModel_Instance"
    //      option 2, when program is compiled:     getCurrentDir()+"/../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"
    //  The input data:
    //      path(argv[2])
    //
    //DATADIR is a macro which can be used when the program is compiled using autotools.
    //DATADIR will be probably: "/urs/local/share/megara-fmpt"
    //
    //When the program is compiled using Qt Creator over Linux:
    //-------------------------------------------------------------------
    //
    //The Qt project and the main.cpp file will be in:
    //  '/home/Isaac/MEGARA/FMPT_SAA-CLI'
    //The object code and the executable will be in:
    //  '/home/Isaac/MEGARA/build-FMPT_SAA-CLI-Desktop-Debug'
    //The other source files and data files will be in the same repository for autotools,
    //whose relative path from the executable is:
    //  for source files:   '../megarafmpt/src'
    //  for data files:     '../megarafmpt/data'
    //
    //Then the program fmpt_saa compiled with Qt, shall be search the data in the following paths:
    //  The Fiber MOS Model Instance:
    //      when program is released:  getCurrentDir()+"/../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"
    //      when program is debugging: getCurrentDir()+"/../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"
    //  The input data:
    //      path = getCurrentDir()+"/../megarafmpt/data/Models/positionersCenters.txt";
    //      path = getCurrentDir()+"/../megarafmpt/data/Models/MEGARA_RP_Instance";
    //      path = getCurrentDir()+"/../megarafmpt/data/Samples/megara-cb6.txt";
    //
    //Using Qt over Windows:
    //-------------------------------------------------------------------
    //
    //Selecting 'D:\Isaac\MEGARA' how working directory:
    //The Qt project and the main.cpp file will be in:
    //  'D:\Isaac\MEGARA\FMPT_SAA-CLI'
    //The object code and the executable will be in:
    //  'D:\Isaac\MEGARA\build-FMPT_SAA-CLI-Desktop_Qt_5_4_0_GCC_64bit-Debug'
    //  'D:\Isaac\MEGARA\build-FMPT_SAA-CLI-Desktop_Qt_5_4_0_GCC_64bit-Release'
    //
    //Then the program fmpt_saa compiled with Qt, shall be search the data in the following paths:
    //  The Fiber MOS Model Instance:
    //      when program is released:  getCurrentDir()+"\..\megarafmpt\data\Models\MEGARA_FiberMOSModel_Instance"
    //      when program is debugging: getCurrentDir()+"\..\megarafmpt\data\Models\MEGARA_FiberMOSModel_Instance"
    //  The input data:
    //      for applyPositionerCenterTable: path = getCurrentDir()+"/../megarafmpt/data/Models/positionersCenters.txt";
    //      for applyRP:                    path = getCurrentDir()+"/../megarafmpt/data/Models/MEGARA_RP_Instance";
    //      for generateDP:                 path = getCurrentDir()+"/../megarafmpt/data/Samples/megara-cb6.txt";
    //
    //WARNING:
    //-------------------------------------------------------------------
    //
    //Changes could happen using more actually versions of qmake.
    //The following change has been found:
    //  getCurrentDir(): could return personal folder '/home/user'
    //  instead the current path.
    //This error can't be solved changing the following value:
    //  Tools -> Options... -> Build & Run -> Projects Directory:
    //      - Current Directory
    //      - Directory
    //
    //###################################################################

#ifdef QMAKE //if debugging using qmake
    //Get the arguments for a command
    getArguments(argc, argv, "valuesSPM_RP");

    //Comands:
    //  "valuesSPM_EA"
    //  "valuesSPM_RP"
    //  "testRadialMotion"
    //  "testDistanceMin"
    //  "applyPCL"
    //  "applyRPI"
    //  "evaluateErrors"
    //  "tuneRP"
    //  "applyTuning"
    //  "PPA_to_CC-S0"
    //  "generatePairPPDP_offline"
    //  "test_generatePairPPDP_offline"
    //  "test_generatePairPPDP_online"
    //  "checkPairPPDP"
    //  "validatePairPPDP"
    //  "regeneratePairPPDP"
    //  "generateParkProg_offline"
    //  "test_generateParkProg_offline"
    //  "test_generateParkProg_online"
#endif //#ifndef QMAKE

    //-------------------------------------------------------------------
    //CONFIGURATE THE SYSTEM:

    //REMEMBER: exceptions in runtime can be due to that
    //the system is not configurated.

    //configurates the locale information
    setlocale(LC_ALL, "C");

    //-----------------------------------------------------------------------
    //MAKE ACTIONS:

    string command;
    string log_path;

    try {
        //if the program is run without the necessary command
        //warn that happened, print the help and finish the program
        if(argc <= 1) {
            //indicates that happened
            cout << "Missing command." << endl;
            //print the help
            cout << endl;
            cout << help() << endl;
            cout << endl;
            //indicates that the program has been executed without error
            return 0;
        }

        //build a command string
        command = string(argv[1]);

        //reacts according the comands without log file
        if(command == "help") {
            //print the help
            cout << endl;
            cout << help() << endl;
            cout << endl;
            //indicates that the program has been executed without error
            return 0;
        }
        else if(command == "help_generatePairPPDP_offline") {
            //print the help
            cout << endl;
            cout << help_generatePairPPDP_offline() << endl;
            cout << endl;
            //indicates that the program has been executed without error
            return 0;
        }
        else if(command == "help_generateParkProg_offline") {
            //print the help
            cout << endl;
            cout << help_generateParkProg_offline() << endl;
            cout << endl;
            //indicates that the program has been executed without error
            return 0;
        }
        else if(command == "help_others") {
            //print the help
            cout << endl;
            cout << help_others() << endl;
            cout << endl;
            //indicates that the program has been executed without error
            return 0;
        }
        else if(command == "aboutOf") {
            //print the about of legend
            cout << endl;
            cout << aboutOf() << endl;
            cout << endl;
            //indicates that the program has been executed without error
            return 0;
        }
        else if(command != "valuesSPM_EA" &&
                command != "valuesSPM_RP" &&
                command != "testRadialMotion" &&
                command != "testDistanceMin" &&
                command != "applyPCL" &&
                command != "applyRPI" &&
                command != "evaluateErrors" &&
                command != "tuneRP" &&
                command != "applyTuning" &&
                command != "PPA_to_CC-S0" &&
                //-------------------------------------------
                command != "generateParkProg_offline" &&
                command != "test_generateParkProg_offline" &&
                command != "test_generateParkProg_online" &&
                //-------------------------------------------
                command != "generatePairPPDP_offline" &&
                command != "test_generatePairPPDP_offline" &&
                command != "test_generatePairPPDP_online" &&
                command != "checkPairPPDP" &&
                command != "validatePairPPDP" &&
                command != "regeneratePairPPDP") {
            //-------------------------------------------
            //indicates that happened
            cout << "Unknowledge command: "+command << endl;
            //print the help
            cout << endl;
            cout << help() << endl;
            cout << endl;
            //indicates that the program has been executed without error
            return 0;
        }

        //initalize the log file
        log_path = "fmpt_saa.log";
        char mode[] = "w";
        TTextFile TF(log_path.c_str(), mode);
        TF.Close();
    }
    catch(Exception &E) {
        //indicates that has happened an exception
        //and show the message of the exception
        cout << "ERROR: "+E.Message.str << endl;
        return 1;
    }
    catch(...) {
        //indicates that has happened an unknown exception
        cout << "ERROR: unknown exception" << endl;
        return 2;
    }

    try {
        //indicates that the program is running
        append("FMPT SAA "+string(PACKAGE_VERSION)+" is running...", log_path.c_str());

        //print the arguments with you have called the program
        append("\r\nArguments with you have called the program:", log_path.c_str());
        for(int i=0; i<argc; i++)
            append("    argv["+inttostr(i)+"]: "+string(argv[i]), log_path.c_str());

        //load the FMM Instance
        TFiberMOSModel FMM;
        string FMM_dir = "";
#ifdef QMAKE
        string FMM_dir1 = getCurrentDir()+"/../megara-fmpt/data/Models/MEGARA_FiberMOSModel_Instance";
        //        string FMM_dir1 = "/usr/local/share/megara-fmpt/Models/MEGARA_FiberMOSModel_Instance";

        //try load the FMM Instance from dir 3
        if(FMM_dir.length() <= 0) {
            try {
                append("\r\nLoading FMM Instance from directory '"+FMM_dir1+"'.", log_path.c_str());
                readInstanceFromDir(FMM, FMM_dir1);
                FMM_dir = FMM_dir1;
            }
            catch(ECantComplete& E) {
                append("Cant't complete action: "+E.Message.str, log_path.c_str());
            }
            catch(EImproperFileLoadedValue& E) {
                append("Improper file loaded value: "+E.Message.str, log_path.c_str());
            }
            catch(Exception& E) {
                E.Message.Insert(1, "loading instance: "+E.Message.str);
                throw;
            }
        }
#else
        //built the paths where search the Fiber MOS Model
        string FMM_dir1 = DATADIR;
        FMM_dir1 += "/Models/MEGARA_FiberMOSModel_Instance";
        string FMM_dir2 = getCurrentDir()+"/../data/Models/MEGARA_FiberMOSModel_Instance";

        //Assignation to FMM_dir1 split for avoid the following error:
        //main.cpp:3737:35: error: invalid operands of types ‘const char [29]’ and ‘const char [38]’ to binary ‘operator+’
        //         string FMM_dir1 = DATADIR+"/Models/MEGARA_FiberMOSModel_Instance";

        //try load the FMM Instance from dir 1
        try {
            append("\r\nLoading FMM Instance from directory '"+FMM_dir1+"'.", log_path.c_str());
            readInstanceFromDir(FMM, FMM_dir1);
            FMM_dir = FMM_dir1;
        }
        catch(ECantComplete& E) {
            append("Cant't complete action: "+E.Message.str, log_path.c_str());
        }
        catch(EImproperFileLoadedValue& E) {
            append("Improper file loaded value: "+E.Message.str, log_path.c_str());
        }
        catch(Exception& E) {
            E.Message.Insert(1, "loading instance: "+E.Message.str);
            throw;
        }

        //try load the FMM Instance from dir 2
        if(FMM_dir.length() <= 0) {
            try {
                append("\r\nLoading FMM Instance from directory '"+FMM_dir2+"'.", log_path.c_str());
                readInstanceFromDir(FMM, FMM_dir2);
                FMM_dir = FMM_dir2;
            }
            catch(ECantComplete& E) {
                append("Cant't complete action: "+E.Message.str, log_path.c_str());
            }
            catch(EImproperFileLoadedValue& E) {
                append("Improper file loaded value: "+E.Message.str, log_path.c_str());
            }
            catch(Exception& E) {
                E.Message.Insert(1, "loading instance: "+E.Message.str);
                throw;
            }
        }
#endif
        //check if has loaded a FMM instance
        if(FMM_dir.length() > 0)
            append("FMM Instance loaded.", log_path.c_str());
        else
            throw EImproperCall("the Fiber MOS Model Instance can't be loaded");

        //-------------------------------------------------------------------
        //REACTS ACCORDING THE COMMAND AND THE ARGUMENTS:

        //print a blank line for improve legibility
        append("", log_path.c_str());

        //-------------------------------------------------------------------
        if(command == "valuesSPM_EA") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command valuesSPM_EA sould have 1 arguments: <Id>");

            //built a string from arg 1 (<Id>)
            string str(argv[2]);

            //convert the argument to int
            int Id = strToInt(str);

            //search the EA
            int i = FMM.EAL.searchId(Id);
            if(i >= FMM.EAL.getCount())
                throw EImpossibleError("there isn't a EA with Id = "+inttostr(Id)+": ");
            TExclusionArea *EA = FMM.EAL[i];

            //print the SPM values
            append("The EA"+EA->getIdText().str+" has been found in the FMM, and their SPM values are:", log_path.c_str());
            append(StrIndent(EA->getSecurityText()).str, log_path.c_str());
            append("\r\nFor learn about the calculus of SPM values, see the file: '/usr/local/share/megara-fmpt/Models/Calculus_of_SPM.txt'", log_path.c_str());
        }
        else if(command == "valuesSPM_RP") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command valuesSPM_RP sould have 1 arguments: <Id>");

            //built a string from arg 1 (<Id>)
            string str(argv[2]);

            //convert the argument to int
            int Id = strToInt(str);

            //search the RP
            int i = FMM.RPL.searchId(Id);
            if(i >= FMM.RPL.getCount())
                throw EImpossibleError("there isn't a RP with Id = "+inttostr(Id)+": ");
            TRoboticPositioner *RP = FMM.RPL[i];

            //print the SPM values
            append("The RP"+RP->getActuator()->getIdText().str+" has been found in the FMM, and their SPM values are:", log_path.c_str());
            append(StrIndent(RP->getActuator()->getSecurityText()).str, log_path.c_str());
            append("\r\nFor learn about the calculus of SPM values, see the file: '/usr/local/share/megara-fmpt/Models/Calculus_of_SPM.txt'", log_path.c_str());
        }
        else if(command == "testRadialMotion") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command testRadialMotion should have 0 arguments");

            //execute the test
            testRadialMotion(FMM, log_path);
        }
        else if(command == "testDistanceMin") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command testDistanceMin should have 0 arguments");

            //execute the test
            testDistanceMin(FMM, log_path);
        }
        //-------------------------------------------------------------------
        else if(command == "applyPCL") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command applyPCL sould have 1 arguments: <PCL_path>");

            //built a path from arg 1
            string PCL_path(argv[2]);

            //make a rutinary check
            if(PCL_path.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(PCL_path[0] != '/')
                PCL_path.insert(0, getCurrentDir()+"/");

            //apply the positioner center list to the Fiber MOS Model Instance
            applyPCL(FMM, PCL_path, log_path);

            //save the instance of the Fiber MOS Model from where was loaded
            writeInstanceToDir(FMM_dir, FMM);
            append("Fiber MOS Model instance saved in directory '"+FMM_dir+"'.", log_path.c_str());
        }
        else if(command == "applyRPI") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command applyRPI sould have 1 arguments: <RPI_dir>");

            //built a path from arg 1
            string RPI_dir(argv[2]);

            //make a rutinary check
            if(RPI_dir.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(RPI_dir[0] != '/')
                RPI_dir.insert(0, getCurrentDir()+"/");

            //apply the RP Instance for all RPs of the Fiber MOS Model Instance
            applyRPI(FMM, RPI_dir, log_path);

            //save the instance of the Fiber MOS Model from where was loaded
            writeInstanceToDir(FMM_dir, FMM);
            append("Fiber MOS Model instance saved in directory '"+FMM_dir+"'.", log_path.c_str());
        }
        else if(command == "evaluateErrors") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command evaluateErrors sould have 1 arguments: <measures_path>");

            //built a path from arg 1
            string measures_path(argv[2]);

            //make a rutinary check
            if(measures_path.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(measures_path[0] != '/')
                measures_path.insert(0, getCurrentDir()+"/");

            //call the function
            evaluateErrors(FMM, measures_path, ".", log_path);
        }
        else if(command == "tuneRP") {
            //check the precondition
            if(argc != 5)
                throw EImproperArgument("command tuneRP sould have 3 arguments: <measures_path> <errors_path> <positions>");

            //built a path from args 1 and 2
            string measures_path(argv[2]);
            string errors_path(argv[3]);

            //make a rutinary check
            if(measures_path.length() <= 0)
                throw EImpossibleError("lateral effect");
            if(errors_path.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative paths, if any
            if(measures_path[0] != '/')
                measures_path.insert(0, getCurrentDir()+"/");
            if(errors_path[0] != '/')
                errors_path.insert(0, getCurrentDir()+"/");

            int positions;
            try {
                //convert to int the arg 3
                string positions_str(argv[4]);
                positions = strToInt(positions_str);
            }
            catch(Exception& E) {
                E.Message.Insert(1, "number of positions (positions) should be an integer upper zero");
                throw;
            }

            //call the function
            tuneRP(FMM, measures_path, errors_path, positions, ".", log_path);
        }
        else if(command == "applyTuning") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command applyTuning sould have 1 arguments: <tuning_path>");

            //built a path from arg 1
            string tuning_path(argv[2]);

            //make a rutinary check
            if(tuning_path.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(tuning_path[0] != '/')
                tuning_path.insert(0, getCurrentDir()+"/");

            //call the function
            applyTuning(FMM, tuning_path, log_path);

            //save the instance of the Fiber MOS Model from where was loaded
            writeInstanceToDir(FMM_dir, FMM);
            append("Fiber MOS Model instance saved in directory '"+FMM_dir+"'.", log_path.c_str());
        }
        else if(command == "PPA_to_CC-S0") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command PPA_to_CC-S0 sould have 1 arguments: <PPAL_path>");

            //built a path from arg 1
            string PPAL_path(argv[2]);

            //make a rutinary check
            if(PPAL_path.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(PPAL_path[0] != '/')
                PPAL_path.insert(0, getCurrentDir()+"/");

            //call the function
            PPA_to_CC_S0(FMM, PPAL_path, ".", log_path);
        }
        //-------------------------------------------------------------------
        else if(command == "generateParkProg_offline") {
            //check the precondition
            if(argc < 3)
                throw EImproperArgument("command generateParkProg_offline sould have almost 1 argument: <FMOSA_path>");

            //build a string for the argument 1
            string arg1(argv[2]);

            //argument 1 can be:
            //  command help ("help")
            //  <FMOSA_path>

            //make a rutinary check
            if(arg1.length() <= 0)
                throw EImpossibleError("lateral effect");

            //reacts according the comands without log file
            if(arg1 == "help") {
                //print the help
                cout << help_generateParkProg_offline() << endl;
                cout << endl;
                //indicates that the program has been executed without error
                return 0;
            }

            //complete the relative path, if any
            if(arg1[0] != '/')
                arg1.insert(0, getCurrentDir()+"/");

            bool saveAll=false;
            bool saveSPL=false, saveFPL=false;
            bool saveDmin=false, saveDend=false;
            bool saveDispCorners1=false, saveDispCorners2=false;
            bool saveFMPT=false, saveMCS=false;
            bool saveOutputs=false, saveOtherOutputs=false;
            for(int i=3; i<argc; i++) {
                string argument(argv[i]);
                //------------------------------------
                if(argument == "all")
                    saveAll = true;
                //------------------------------------
                else if(argument == "SPL")
                    saveSPL = true;
                else if(argument == "FPL")
                    saveFPL = true;
                //------------------------------------
                else if(argument == "Dmin")
                    saveDmin = true;
                else if(argument == "Dend")
                    saveDend = true;
                else if(argument == "DispCorners1")
                    saveDispCorners1 = true;
                else if(argument == "DispCorners2")
                    saveDispCorners2 = true;
                //------------------------------------
                else if(argument == "FMPT")
                    saveFMPT = true;
                else if(argument == "MCS")
                    saveMCS = true;
                else if(argument == "outputs")
                    saveOutputs = true;
                else if(argument == "other-outputs")
                    saveOtherOutputs = true;
                //------------------------------------
                else
                    throw EImproperArgument("unknown argument: "+argument);
            }

            if(saveAll) {
                saveSPL=true, saveFPL=true;
                saveDmin=true, saveDend=true;
                saveDispCorners1=true, saveDispCorners2=true;
                saveFMPT=true, saveMCS=true;
                saveOutputs=true, saveOtherOutputs=true;
            }

            //generate a DP from a path and write the events in the log file
            generateParkProg_offline(FMM, arg1, ".", log_path,
                                     //--------------------------------------------
                                     saveSPL, saveFPL,
                                     saveDmin, saveDend,
                                     saveDispCorners1, saveDispCorners2,
                                     saveFMPT, saveMCS,
                                     saveOutputs, saveOtherOutputs);
        }
        else if(command == "test_generateParkProg_offline") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command test_generateParkProg_offline should have 0 arguments");

            //execute the test
            test_generateParkProg_offline(FMM, log_path);
        }
        else if(command == "test_generateParkProg_online") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command test_generateParkProg_online should have 0 arguments");

            //execute the test
            test_generateParkProg_online(FMM, log_path);
        }
        else if(command == "generatePairPPDP_offline") {
            //check the precondition
            if(argc < 3)
                throw EImproperArgument("command generatePairPPDP_offline sould have almost 1 argument: <FMOSA_path>");

            //build a string for the argument 1
            string arg1(argv[2]);

            //argument 1 can be:
            //  command help ("help")
            //  <FMOSA_path>

            //make a rutinary check
            if(arg1.length() <= 0)
                throw EImpossibleError("lateral effect");

            //reacts according the comands without log file
            if(arg1 == "help") {
                //print the help
                cout << help_generatePairPPDP_offline() << endl;
                cout << endl;
                //indicates that the program has been executed without error
                return 0;
            }

            //complete the relative path, if any
            if(arg1[0] != '/')
                arg1.insert(0, getCurrentDir()+"/");

            bool saveAll=false;
            bool saveOPL=false, saveOPL_S0=false, saveOPL_S1=false;
            bool saveIPL=false, saveDispCorners1=false, saveDispCorners2=false;
            bool savePP_Dmin=false, savePP_Dend=false, saveDP_Dmin=false, saveDP_Dend=false;
            bool savePP_FMPT=false, saveDP_FMPT=false, savePP_MCS=false, saveDP_MCS=false;
            bool saveMCS=false, save_outputs=false, save_other_outputs=false;
            for(int i=3; i<argc; i++) {
                string argument(argv[i]);
                //------------------------------------
                if(argument == "all")
                    saveAll = true;
                //------------------------------------
                else if(argument == "OPL")
                    saveOPL = true;
                else if(argument == "OPL-S0")
                    saveOPL_S0 = true;
                else if(argument == "OPL-S1")
                    saveOPL_S1 = true;
                //------------------------------------
                else if(argument == "IPL")
                    saveIPL = true;
                else if(argument == "DispCorners1")
                    saveDispCorners1 = true;
                else if(argument == "DispCorners2")
                    saveDispCorners2 = true;
                //------------------------------------
                else if(argument == "PP-Dmin")
                    savePP_Dmin = true;
                else if(argument == "PP-Dend")
                    savePP_Dend = true;
                else if(argument == "DP-Dmin")
                    saveDP_Dmin = true;
                else if(argument == "DP-Dend")
                    saveDP_Dend = true;
                //------------------------------------
                else if(argument == "PP-FMPT")
                    savePP_FMPT = true;
                else if(argument == "DP-FMPT")
                    saveDP_FMPT = true;
                else if(argument == "PP-MCS")
                    savePP_MCS = true;
                else if(argument == "DP-MCS")
                    saveDP_MCS = true;
                //------------------------------------
                else if(argument == "MCS")
                    saveMCS = true;
                else if(argument == "outputs")
                    save_outputs = true;
                else if(argument == "other-outputs")
                    save_other_outputs = true;
                //------------------------------------
                else
                    throw EImproperArgument("unknown argument: "+argument);
            }

            if(saveAll) {
                saveOPL=true, saveOPL_S0=true, saveOPL_S1=true;
                saveIPL=true, saveDispCorners1=true, saveDispCorners2=true;
                savePP_Dmin=true, savePP_Dend=true, saveDP_Dmin=true, saveDP_Dend=true;
                savePP_FMPT=true, saveDP_FMPT=true, savePP_MCS=true, saveDP_MCS=true;
                saveMCS=true, save_outputs=true, save_other_outputs=true;
            }

            //generate a pair (PP, DP) from a path and write the events in the log file
            generatePairPPDP_offline(FMM, arg1, ".", log_path,
                                     //------------------------------------------------------------
                                     saveOPL, saveOPL_S0, saveOPL_S1,
                                     saveIPL, saveDispCorners1, saveDispCorners2,
                                     savePP_Dmin, savePP_Dend, saveDP_Dmin, saveDP_Dend,
                                     savePP_FMPT, saveDP_FMPT, savePP_MCS, saveDP_MCS,
                                     saveMCS, save_outputs, save_other_outputs);
        }
        else if(command == "test_generatePairPPDP_offline") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command test_generatePairPPDP_offline should have 0 arguments");

            //execute the test
            test_generatePairPPDP_offline(FMM, log_path);
        }
        else if(command == "test_generatePairPPDP_online") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command test_generatePairPPDP_online should have 0 arguments");

            //execute the test
            test_generatePairPPDP_online(FMM, log_path);
        }
        //-------------------------------------------------------------------
        else if(command == "checkPairPPDP") {
            //check the precondition
            if(argc!=4 && argc!=5)
                throw EImproperArgument("command checkPairPPDP sould have 2 or 3 arguments: <PP_path> <DP_path> [RPids]");

            //built a path from arg 1
            string PP_path(argv[2]);
            //built a path from arg 2
            string DP_path(argv[3]);

            //make a rutinary check
            if(PP_path.length()<=0 || DP_path.length()<=0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(PP_path[0] != '/')
                PP_path.insert(0, getCurrentDir()+"/");
            if(DP_path[0] != '/')
                DP_path.insert(0, getCurrentDir()+"/");

            //if has been specified a RP list, disable it
            if(argc == 5) {
                //translate from text to identifier list
                TVector<int> Ids(92);
                string str(argv[4]);
                Ids.setText(str);

                //check the precondition
                for(int i=0; i<Ids.getCount(); i++) {
                    int Id = Ids[i];
                    int j = FMM.RPL.searchId(Id);
                    if(j >= FMM.RPL.getCount())
                        throw EImproperArgument("RP not fount in the Fiber MOS Model: "+inttostr(Id));
                }

                //disble the RPs
                for(int i=0; i<Ids.getCount(); i++) {
                    int Id = Ids[i];
                    int j = FMM.RPL.searchId(Id);
                    if(j >= FMM.RPL.getCount())
                        throw EImpossibleError("lateral effect");
                    TRoboticPositioner *RP = FMM.RPL[j];
                    RP->Disabled = true;
                }
                append("Identifiers of the disabled RPs: "+Ids.getText().str, log_path.c_str());
            }

            //check a pair (PP, DP) from a path and write the events in the log file
            checkPairPPDP(FMM, PP_path, DP_path, log_path);
        }
        else if(command == "validatePairPPDP") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command validatePairPPDP sould have 1 arguments: <outputs_path>");

            //built a path from arg 1
            string outputs_path(argv[2]);

            //make a rutinary check
            if(outputs_path.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(outputs_path[0] != '/')
                outputs_path.insert(0, getCurrentDir()+"/");

            //validates a pair (PP, DP) from a path and write the events in the log file
            validatePairPPDP(FMM, outputs_path, ".", log_path);
        }
        else if(command == "regeneratePairPPDP") {
            //check the precondition
            if(argc!=5 && argc!=6)
                throw EImproperArgument("command regeneratePairPPDP sould have 3 or 4 arguments: <PP_path> <DP_path> <FMOSA_path> [RPids]");

            //built a path from arg 1
            string PP_path(argv[2]);
            //built a path from arg 2
            string DP_path(argv[3]);
            //built a path from arg 3
            string FMOSA_path(argv[4]);

            //make a rutinary check
            if(PP_path.length()<=0 || DP_path.length()<=0 || FMOSA_path.length()<=0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(PP_path[0] != '/')
                PP_path.insert(0, getCurrentDir()+"/");
            if(DP_path[0] != '/')
                DP_path.insert(0, getCurrentDir()+"/");
            if(FMOSA_path[0] != '/')
                FMOSA_path.insert(0, getCurrentDir()+"/");

            //if has been specified a RP list, disable it
            if(argc == 6) {
                //translate from text to identifier list
                TVector<int> Ids(92);
                string str(argv[5]);
                Ids.setText(str);

                //check the precondition
                for(int i=0; i<Ids.getCount(); i++) {
                    int Id = Ids[i];
                    int j = FMM.RPL.searchId(Id);
                    if(j >= FMM.RPL.getCount())
                        throw EImproperArgument("RP not fount in the Fiber MOS Model: "+inttostr(Id));
                }

                //disble the RPs
                for(int i=0; i<Ids.getCount(); i++) {
                    int Id = Ids[i];
                    int j = FMM.RPL.searchId(Id);
                    if(j >= FMM.RPL.getCount())
                        throw EImpossibleError("lateral effect");
                    TRoboticPositioner *RP = FMM.RPL[j];
                    RP->Disabled = true;
                }
                append("Identifiers of the disabled RPs: "+Ids.getText().str, log_path.c_str());
            }

            //validates a pair (PP, DP) from a path and write the events in the log file
            regeneratePairPPDP_offline(FMM, PP_path, DP_path, FMOSA_path, log_path);
        }
        //-------------------------------------------------------------------
        else {
            throw EImpossibleError("lateral effect");
        }
    }
    catch(Exception &E) {
        //indicates that has happened an exception
        //and show the message of the exception
        append("ERROR: "+E.Message.str, log_path.c_str());
        return 1;
    }
    catch(...) {
        //indicates that has happened an unknown exception
        append("ERROR: unknown exception", log_path.c_str());
        return 2;
    }

    //-----------------------------------------------------------------------

/*    TestFileMethods TFM;
    bool b1 = TFM.test_readInstanceFromDir_RP();
    bool b2 = TFM.test_writeInstanceToDir_EAL();
    bool b3 = TFM.test_readInstanceFromDir_EAL();
    bool b4 = TFM.test_writeInstanceToDir_RPL();
    bool b5 = TFM.test_readInstanceFromDir_RPL();
    bool b6 = TFM.test_writeInstanceToDir_FMM();
    bool b7 = TFM.test_readInstanceFromDir_FMM();
*/
    //indicates that the program has been executed without error
    return 0;
}

