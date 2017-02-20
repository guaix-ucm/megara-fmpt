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
//File: main.cpp
//Content: principal program fmpt_saa
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "FileMethods.h"
#include "MotionProgramGenerator.h"
#include "FMOSATable.h"
#include "PositionerCenter.h"
#include "Strings.h"
#include "TextFile.h"
#include "Geometry.h" //distanceSegmentPoint
#include "Outputs.h" //Outputs
#include "TextFile.h"

//#include "../megarafmpt/ui/GenerateFrames.h"
//#include "../megarafmpt/ui/mainwindow.h"

//#include <QApplication> //Qt only
#include <locale.h> //setlocale, LC_NUMERIC
#include <iostream> //std::cout, ios::fixed

using namespace Strings;
using namespace Models;
using namespace Positioning;

//---------------------------------------------------------------------------
//GENERAL FUNCTIONS:

//print a text line in the standard output and in the log file
void append(const string& str, const char *log_filename)
{
    string str_aux = str;
    str_aux += "\r\n";

    std::cout << str_aux;

    char mode[] = "a";
    TTextFile TF(log_filename, mode);
    TF.Print(str_aux.c_str());
    TF.Close();
}

//split a path
void splitpath(string& parent_path, string& filename, const string& path)
{
    int i = path.length() - 1;
    while(i>=0 && path[i] != '/')
        i--;

    parent_path = "";
    if(i >= 0)
        parent_path = path.substr(0, i);

    filename = "";
    int aux = path.length() - 1;
    if(i < aux) {
        int count = aux - i;
        filename = path.substr(i+1, count);
    }

    //WARNING: in GCC 2.95 it is not recognized:
    //    string::clear()
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
    str += "\r\n$ fmpt_saa help_generateParkingProgram_offline";
    str += "\r\n    Print the help for function generatePairPPDP_offline.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa help_others";
    str += "\r\n    Print the help about other functions.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa aboutOf";
    str += "\r\n    Print the legend about of...";
    str += "\r\n";
    str += "\r\n$ fmpt_saa generatePairPPDP_offline <path_FMOSA>";
    str += "\r\n    Generate a pair (PP, DP) offline.";
    str += "\r\n    <path_FMOSA>: absolute or relative path to the input file type FMOSA.";
    str += "\r\n    This command generates the files:";
    str += "\r\n        outputs-from-<filename>: the pair (PP, DP) in the format of the MCS,";
    str += "\r\n            and the observing source list (OS) contained in the input file;";
    str += "\r\n        other_outputs-from-<filename>: PPvalid, DPvalid, Collided, Obstructed";
    str += "\r\n            and comments about the RPs whose radial motion is more close to 1 mm.";
    str += "\r\n    Where <filename> is the name of the input file.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa generateParkingProgram_offline <path_FMOSA>";
    str += "\r\n    Generate a parking program offline.";
    str += "\r\n    <path_FMOSA>: absolute or relative path to the input file type FMOSA.";
    str += "\r\n    This command generates the files:";
    str += "\r\n        ParkingProgram-from-<filename>: the parking program in the format of the MCS;";
    str += "\r\n        other_outputs-from-<filename>: ParkingProgramValid, Collided, Obstructed";
    str += "\r\n            and comments about the RPs whose radial motion is more close to 1 mm.";
    str += "\r\n    Where <filename> is the name of the input file.";

    //Note that here not has sense define functions for generate MPs online.

    return str;
}

//gets a string containing the help about function generatePairPPDP_offline
string help_generatePairPPDP_offline(void)
{
    string str;

    str = "$ fmpt_saa generatePairPPDP_offline <path_FMOSA> [OPL] [OPL_S0] [OPL_S1] [PP-FMPT] [DP-FMPT] [IPL] [Disp_Corners1] [Disp_Corners2] [PP] [DP] [PP-Dmin] [PP-Dend] [DP-Dmin] [DP-Dend] [fiberMOS] [r2_negative]";
    str += "\r\n    Generate a pair (PP, DP) offline.";
    str += "\r\n    <path_FMOSA>: absolute or relative path to the input file type FMOSA.";
    str += "\r\n    This command generates the files:";
    str += "\r\n        outputs-from-<filename>: the pair (PP, DP) in the format of the MCS,";
    str += "\r\n            and the observing source list (OS) contained in the input file;";
    str += "\r\n        other_outputs-from-<filename>: PPvalid, DPvalid, Collided, Obstructed";
    str += "\r\n            and comments about the RPs whose radial motion is more close to 1 mm.";
    str += "\r\n    Where <filename> is the name of the input file.";
    str += "\r\n";
    str += "\r\n    In the file type FMOSA:";
    str += "\r\n        The following parameters could be empty: Name, Mag, Pr, Bid and Comment.";
    str += "\r\n        Parameter Bid indicates if the source is allocated or not.";
    str += "\r\n        Parameter Enabled indicates if the RP is enabled or not.";
    str += "\r\n        When Bid is empty:";
    str += "\r\n            parameters Name, Mag, Pr and Comment must be empty;";
    str += "\r\n            parameter Type must be UNKNOWN.";
    str += "\r\n";
    str += "\r\n    This function has several optional arguments, each of witch allow save a data in a file,";
    str += "\r\n    whose name is composed as follow:";
    str += "\r\n        <argument> + \"-from-\" + <filename>";
    str += "\r\n    Where <filename> is the name of the input file, and <argument> is one of the following strings:";
    str += "\r\n";
    str += "\r\n    OPL:    Observing Position List in the format PPA (Pair Position Angles in steps).";
    str += "\r\n    OPL_S0: Observing Position List in the format cartesian coordinates from S0.";
    str += "\r\n    OPL_S1: Observing Position List in the format cartesian coordinates from S1.";
    str += "\r\n";
    str += "\r\n    PP-FMPT: Positioning Program in the format of the FMPT.";
    str += "\r\n    DP-FMPT: Depositioning Program in the format of the FMPT.";
    str += "\r\n    (The FMPT format use absolute coordinates and own instruction).";
    str += "\r\n";
    str += "\r\n    IPL: Initial Position List in the format PPA (Pair Position Angles in steps).";
    str += "\r\n";
    str += "\r\n    Disp_Corners1: displacement of corners 1 from initial position to observing position.";
    str += "\r\n    Disp_Corners2: displacement of corners 2 from initial position to observing position.";
    str += "\r\n    (1 and 2 are upper and lower corners of the arm elbow, when rotors are in the origin).";
    str += "\r\n";
    str += "\r\n    PP: Positioning Program in the format of the MCS.";
    str += "\r\n    DP: Depositioning Program in the format of the MCS.";
    str += "\r\n    (PP and DP are saved also in the file ouputs, followed by the content of the input file).";
    str += "\r\n";
    str += "\r\n    PP-Dmin: minimun distance during executing the Positioning Program.";
    str += "\r\n    PP-Dend: distance at the end of the trajectory during executing the Positioning Program.";
    str += "\r\n    DP-Dmin: minimun distance during executing the Depositioning Program.";
    str += "\r\n    DP-Dend: distance at the end of the trajectory during executing the Depositioning Program.";
    str += "\r\n    (The distances are given in mm for each gesture for each RP).";
    str += "\r\n";
    str += "\r\n    fiberMOS: a file containing only the PP followed by the DP, in the format of the MCS.";
    str += "\r\n    (For this option, the file name will be 'fiberMOS.txt' only).";
    str += "\r\n";
    str += "\r\n    r2_negative: switch the sign of rotor 2 coordinates in PP and DP in the format of the MCS.";
    str += "\r\n    (r2 negative affects exclusively to files: PP, DP, outputs and fiberMOS).";
    str += "\r\n    (r2_negative not afects to files: OPL, PP-FMPT, DP-FMPT and IPL).";

    return str;
}

//gets a string containing the help about function generateParkingProgram_offline
string help_generateParkingProgram_offline(void)
{
    string str;

    str = "$ fmpt_saa generateParkingProgram_offline <path_FMOSA> [SPL] [ParkingProgram-FMPT] [ParkingProgram-Dmin] [ParkingProgram-Dend] [FPL] [r2_negative]";
    str += "\r\n    Generate a parking program offline.";
    str += "\r\n    <path_FMOSA>: absolute or relative path to the input file type FMOSA.";
    str += "\r\n    This command generates the files:";
    str += "\r\n        ParkingProgram-from-<filename>: the parking program in the format of the MCS;";
    str += "\r\n        other_outputs-from-<filename>: ParkingProgramValid, Collided, Obstructed";
    str += "\r\n            and comments about the RPs whose radial motion is more close to 1 mm.";
    str += "\r\n    Where <filename> is the name of the input file.";
    str += "\r\n";
    str += "\r\n    In the file type FMOSA:";
    str += "\r\n        The following parameters could be empty: Name, Mag, Pr, Bid and Comment.";
    str += "\r\n        Parameter Bid indicates if the source is allocated or not.";
    str += "\r\n        Parameter Enabled indicates if the RP is enabled or not.";
    str += "\r\n        When Bid is empty:";
    str += "\r\n            parameters Name, Mag, Pr and Comment must be empty;";
    str += "\r\n            parameter Type must be UNKNOWN.";
    str += "\r\n";
    str += "\r\n    This function has several optional arguments, each of witch allow save a data in a file,";
    str += "\r\n    whose name is composed as follow:";
    str += "\r\n        <argument> + \"-from-\" + <filename>";
    str += "\r\n    Where <filename> is the name of the input file, and <argument> is one of the following strings:";
    str += "\r\n";
    str += "\r\n    SPL: Starting Position List in the format PPA (Pair Position Angles in steps).";
    str += "\r\n";
    str += "\r\n    ParkingProgram-FMPT: Parking Program in the format of the FMPT.";
    str += "\r\n    (The FMPT format use absolute coordinates and own instruction).";
    str += "\r\n";
    str += "\r\n    ParkingProgram-Dmin: minimun distance during executing the Parking Program.";
    str += "\r\n    ParkingProgram-Dend: distance at the end of the trajectory during executing the Parking Program.";
    str += "\r\n";
    str += "\r\n    FPL: Final Position List in the format PPA (Pair Position Angles in steps).";
    str += "\r\n";
    str += "\r\n    r2_negative: switch the sign of rotor 2 coordinates in ParkingProgram in the format of the MCS.";
    str += "\r\n    (r2 negative affects exclusively to file: ParkingProgram).";
    str += "\r\n    (r2_negative not afects to files: SPL, ParkingProgram-FMPT and FPL).";

    return str;
}

//gets a string containing the help about other functions
string help_others(void)
{
    string str;

    str = "\r\n$ fmpt_saa valuesSPM_EA <Aid>";
    str += "\r\n    View the SPM values.";
    str += "\r\n    <Aid>: identifier list of the EA to view.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa valuesSPM_RP <Pid>";
    str += "\r\n    View the SPM values pending varibales PAkd and Purpose.";
    str += "\r\n    <Pid>: identifier list of the RP to view.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa testRadialMotion";
    str += "\r\n    Take the measure of maximun deviation around the radial trajectory of the fiber of each RP.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa testDistanceMin";
    str += "\r\n    Determine the minimun distance for each RP, with each of their adjacents,";
    str += "\r\n    when one of each pair is in the origin position.";

    str += "\r\n";
    str += "\r\n$ fmpt_saa applyPC <path_PC>";
    str += "\r\n    <path_PC>: absolute or relative path to file containing";
    str += "\r\n    a positioner center table.";
    str += "\r\n    Apply a positioner center table to the Fiber MOS Model instance.";
    str += "\r\n    The new added RPs are built by default.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa applyRP <dir_RP>";
    str += "\r\n    <dir_RP>: absolute or relative path to dir containing a RP instance.";
    str += "\r\n    Apply a RP instance to the Fiber MOS Model instance.";

    str += "\r\n";
    str += "\r\n$ fmpt_saa test_generatePairPPDP_offline";
    str += "\r\n    Test the function generatePairPPDP_offline:";
    str += "\r\n        generatePairPPDP_offline(PPvalid, DPvalid, Collided, Obstructed, PP, DP,";
    str += "\r\n                                 FMM, input_path, output_dir, log_filename);";
    str += "\r\n";
    str += "\r\n$ fmpt_saa test_generatePairPPDP_online";
    str += "\r\n    Test the function generatePairPPDP_online:";
    str += "\r\n        PairPPDPvalid = generatePairPPDP_online(PP, DP,";
    str += "\r\n                                                FMM, p_1s, p___3s, Ids);";

    str += "\r\n";
    str += "\r\n$ fmpt_saa checkPairPPDP <path_PP> <path_DP> [Pid list]";
    str += "\r\n    <path_PP>: absolute or relative path to file containing the PP.";
    str += "\r\n    <path_DP>: absolute or relative path to file containing the DP.";
    str += "\r\n    [Pid list]: optional identifier list of RPs to be disabled.";
    str += "\r\n    Determine if all RPs included in the pair (PP, DP), are enabled.";
    str += "\r\n    The instance of the Fiber MOS Model will not be written.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa validatePairPPDP <path_outputs>";
    str += "\r\n    <path_outputs>: absolute or relative path to file type outputs,";
    str += "\r\n    containing the pair (PP, DP).";
    str += "\r\n    Check if a pair (PP, DP) avoid collisions,";
    str += "\r\n    wehn it is executed starting from the origin.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa regeneratePairPPDP <path_PP> <path_DP> <path_FMOSA> [Pid list]";
    str += "\r\n    <path_PP>: absolute or relative path to file containing the PP.";
    str += "\r\n    <path_DP>: absolute or relative path to file containing the DP.";
    str += "\r\n    <path_FMOSA>: absolute or relative path to file type FMOSA.";
    str += "\r\n    [Pid list]: optional identifier list of RPs to be disabled.";
    str += "\r\n    Attempt regenerate a pair (PP, DP).";
    str += "\r\n    The instance of the Fiber MOS Model will not be written.";

    str += "\r\n";
    str += "\r\n$ fmpt_saa test_generateParkingProgram_offline";
    str += "\r\n    Test the function generateParkingProgram_offline:";
    str += "\r\n        generateParkingProgram_offline(ParkingProgramValid, ";
    str += "\r\n                                       Collided, Obstructed, ParkingProgram,";
    str += "\r\n                                       FMM, input_path, output_dir, log_filename);";
    str += "\r\n";
    str += "\r\n$ fmpt_saa test_generateParkingProgram_online";
    str += "\r\n    Test the function generateParkingProgram_online:";
    str += "\r\n        ParkingProgramValid = generateParkingProgram_online(ParkingProgram,";
    str += "\r\n                                                            FMM, p_1s, p___3s,";
    str += "\r\n                                                            Ids);";
    str += "\r\n$ fmpt_saa visualizePairPPDP <path_PP> <path_DP>";
    str += "\r\n    <path_PP>: absolute or relative path to file containing the PP.";
    str += "\r\n    <path_DP>: absolute or relative path to file containing the DP.";
    str += "\r\n    Generate one animation with the pair (PP, DP).";

    //Others diserable functionalities:
    //  - Determine the RPs included in a MP.
    //  - Determine the RPs included in a pair of MPs.
    //  - Enable all not faulty RPs of the FMM instance,
    //  - Disable the indicated PRs of the FMM instance,
    //  - Print the samples includes in the directory data/Samples

    return str;
}

//get a string containing the about of legend
string aboutOf(void)
{
    string str;

    str = "Copyright (c) 2012-2017 Isaac Morales Durán. All rights reserved.\r\n";
    str += "Institute of Astrophysics of Andalusia, IAA-CSIC\r\n";
    str += "\r\n";
    str += "This application is the FMPT SAA 3.8.3.\r\n";
    str += "(Fiber MOS Positioning Tools Stand Alone Application. Version 3.8.3).\r\n";
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
    str += "along with this program.  If not, see <http://www.gnu.org/licenses/>.\r\n";

    return str;
}

//---------------------------------------------------------------------------
//FUNCTIONS FOR COMMANDS:

//Take the measure of maximun deviation around the radial trajectory of the fiber of each RP.
void testRadialMotion(TFiberMOSModel& FMM, string& log_filename)
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
        append("SIMULATION THE RADIAL EXTENSION AND RADIAL RETRACTION OF THE FIBERS:", log_filename.c_str());
        //print the labels of the table
        append("\r\nId\tdistanceMin\t\tdistanceMax", log_filename.c_str());

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
            append(str, log_filename.c_str());
        }
    }
    catch(Exception& E) {
        E.Message.Insert(1, "testing radial motion: ");
        throw;
    }
}

//Determine the minimun distance for each RP, with each of their adjacents,
//when one of each pair is in the origin position.
void testDistanceMin(TFiberMOSModel& FMM, string& log_filename)
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
        append("THE SPMmax OF EACH RP:", log_filename.c_str());
        //print the labels of the table
        append("\r\nId\tSPMmax", log_filename.c_str());

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
            append(str, log_filename.c_str());
        }
    }
    catch(Exception& E) {
        E.Message.Insert(1, "testing SPMmax: ");
        throw;
    }
}

//Apply a positioner center table to the Fiber MOS Model instance.
//The new added RPs are built by default.
void applyPC(TFiberMOSModel& FMM, string& path, string& log_filename)
{
    try {

        //LOAD SETTINGS FROM FILES:

        //load the positioner center table from a file
        string str;
        strReadFromFile(str, path);
        TPositionerCenterList PCL;
        PCL.setTableText(str);
        append("Positioner center table loaded from '"+path+"'.", log_filename.c_str());

        //MAKE THE OPERATIONS:

        //assimilates the table in the Fiber MOS Model
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
        append("Positioner center table applied to the Fiber MOS Model.", log_filename.c_str());

    } catch(Exception& E) {
        E.Message.Insert(1, "aplying positioner center table: ");
        throw;
    }
}

//Apply a RP instance to the Fiber MOS Model instance.
void applyRP(TFiberMOSModel& FMM, string& path, string& log_filename)
{
    try {

        //LOAD SETTINGS FROM FILES:

        //load the RP instance from a dir
        TRoboticPositioner RP;
        readInstanceFromDir(RP, path);
        append("RP instance loaded from '"+path+"'.", log_filename.c_str());

        //MAKE THE OPERATIONS:

        //assimilates the instance of the RP in the Fiber MOS Model
        FMM.RPL.apply(&RP, FMM.EAL);
        append("RP instance applied to the Fiber MOS Model.", log_filename.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "aplying RP instance: ");
        throw;
    }
}

//Generate a pair (PP, DP) offline.
//Parameters for can use the function for test:
//  (PPvalid, DPvalid, Collided, Obstructed, PP, DP)
//Parameters for generate a pair PPDP:
//  (FMM, path, log_filename)
void generatePairPPDP_offline(bool& PPvalid, bool& DPvalid,
                              TRoboticPositionerList& Collided, TRoboticPositionerList& Obstructed,
                              TMotionProgram& PP, TMotionProgram& DP,
                              //-------------------------------------------------------------------------------------------------
                              TFiberMOSModel& FMM, const string& input_path, const string& output_dir, const string& log_filename,
                              //-------------------------------------------------------------------------------------------------
                              bool saveOPL, bool saveOPL_S0, bool saveOPL_S1, bool savePP_FMPT, bool saveDP_FMPT,
                              bool saveIPL, bool saveDisp_Corners1, bool saveDisp_Corners2, bool savePP, bool saveDP,
                              bool savePP_Dmin, bool savePP_Dend, bool saveDP_Dmin, bool saveDP_Dend, bool saveFiberMOS, bool r2_negative)
{
    try {

        //LOAD SETTINGS FROM FILES:

        //load the FMOSA table from the file input_path
        string str;
        Outputs outputs;
        unsigned int Bid;
        try {
            strReadFromFileWithComments(str, input_path);
            outputs.FMOSAT.setTableText(Bid, str);

        } catch(Exception& E) {
            E.Message.Insert(1, "reading FMOSA file: ");
            throw;
        }
        append("FMOSA table loaded from '"+input_path+"'.", log_filename.c_str());

        //get the allocation from the FMOSA table
        TMotionProgramGenerator MPG(&FMM);
        outputs.FMOSAT.getAllocations(MPG);
        append("Allocations got from the FMOSA table in MPG.", log_filename.c_str());

        //MAKE THE OPERATIONS:

        //split the path of the file containing the FMOSA table
        string parent_path, filename;
        splitpath(parent_path, filename, input_path);

        //The filename will be used to attach the outputs filenames witht the input filename.

        //move the RPs to the more closer stable position to the allocated projection points
        MPG.MoveToTargetP3();
        if(MPG.getCount() > 0)
            append("RPs moved to observing positions.", log_filename.c_str());
        else
            append("There aren't RPs to be moved to observing positions.", log_filename.c_str());

        //WARNING: sentence could take several seconds:
        //  MPG.MoveToTargetP3();

        //check the limiting case when there aren't allocation

        //Other way to obtain the more closer stable positions to the allocated projection points,
        //consist in get from the FMOSA table the following lists:
        //  the allocation list;
        //  the projection point list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

        //A PPA table shall be stored how a table (Id, p_1, p___3).

        //captures the observing positions of the RPs in a PPA list
        TPairPositionAnglesList OPL;
        FMM.RPL.getPositions(OPL);
        string output_filename;

        //save OPL, if any
        if(saveOPL) {
            str = TActuator::getPositionPPALabelsRow().str;
            str += "\r\n"+OPL.getColumnText().str;
            ForceDirectories(AnsiString(output_dir));
            output_filename = output_dir+"/OPL-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Observing Position List (in Pair Position Angles) saved in '"+output_filename+"'.", log_filename.c_str());
        }

        //Other whay to obtain the observing position table directly in text format:
        //  FMM.RPL.getPositionsPPATableText()

        //save OPL in cartesian coordinates respect S0, if any
        if(saveOPL_S0) {
            str = TActuator::getPositionP3LabelsRow().str;
            str += "\r\n";
            str += FMM.RPL.getPositionsP3TableText().str;
            output_filename = output_dir+"/OPL_S0-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Observing Position List (in Cart. Coord. respect S0) saved in '"+output_filename+"'.", log_filename.c_str());
        }

        //save OPL in cartesian coordinates respect S1, if any
        if(saveOPL_S1) {
            str = TActuator::getPositionP_3LabelsRow().str;
            str += "\r\n";
            str += FMM.RPL.getPositionsP_3TableText().str;
            output_filename = output_dir+"/OPL_S1-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Observing Position List (in Cart. Coord. respect S1) saved in '"+output_filename+"'.", log_filename.c_str());
        }

        //captures the corner of arms in the observing positions in cartesian coordinates
        TItemsList<TDoublePoint> OPL_Corners1;
        FMM.RPL.getPositionsCorners1(OPL_Corners1);
        TItemsList<TDoublePoint> OPL_Corners2;
        FMM.RPL.getPositionsCorners2(OPL_Corners2);

        //segregates the operative outsider RPs
        TRoboticPositionerList Outsiders;
        FMM.RPL.segregateOperativeOutsiders(Outsiders);

        //check the limiting case when there aren't allocations
        if(MPG.getCount() <= 0)
            append("WARNING: there aren't allocations. The generated parking program will be empty.", log_filename.c_str());
        //check the limiting case when all operative RPs are in the origin
        else if(FMM.RPL.allOperativeRPsAreInTheOrigin())
            append("WARNING: all operative RPs are in the origin. The generated pair (PP, DP) will be empty.", log_filename.c_str());
        //else, check the limiting case when all operative RPs are in security positions
        else if(Outsiders.getCount() <= 0)
            append("WARNING: all operative RPs are in security positions. The generated pair (PP, DP) will contains only a message-instruction list to go to the observing positions and back to the origin.", log_filename.c_str());

        //Now are fulfilled the preconditions:
        //  All RPs of the Fiber MOS Model, shall be in their observing positions.
        //  All RPs of the list Outsiders, shall be in the Fiber MOS Model.
        //  All RPs of the list Outsiders, shall be operatives.
        //  All RPs of the list Outsiders, shall be in unsecurity positions.
        //  All RPs of the list Outsiders, shall have enabled the quantifiers.

        //generates a pair (PP, DP) for the operative RPs in unsecurity positions
        //and determines the RPs in collision status or obstructed in insecure positions
        append("Generating pair (PP, DP)...", log_filename.c_str());
        PPvalid = false;
        DPvalid = false;
        Collided.Clear();
        Obstructed.Clear();
        PP.Clear();
        DP.Clear();
        MPG.generatePairPPDP(PPvalid, DPvalid, Collided, Obstructed, PP, DP, Outsiders);

        //Now are fulfilled the postconditions:
        //  All RPs of the Fiber MOS Model will be configured for MP validation.
        //  All RPs of the fiber MOS Model will be in their origin positions,
        //  or the first position where the collision was detected.
        //  All RPs of the Fiber MOS Model will have disabled the quantifiers.

        //########################################################################
        //#WARNING: before re-use a function to generation, shall be restablished#
        //#the preconditions.                                                    #
        //########################################################################

        //SAVE THE OUTPUTS AND PRINT THE CORRESPONDING MESSAGES:

        //if the pair (PP, DP) is valid
        if(PPvalid && DPvalid) {
            //indicates the result of the generation
            append("Generated pair (PP, DP) is valid.", log_filename.c_str());

            //save the PP in the format of the FMPT, if any
            if(savePP_FMPT) {
                str = PP.getText().str;
                output_filename = output_dir+"/PP-FMPT-from-"+filename;
                strWriteToFile(output_filename, str);
                append("PP (in FMPT format) saved in '"+output_filename+"'.", log_filename.c_str());
            }

            //save the DP in the format of the FMPT, if any
            if(saveDP_FMPT) {
                str = DP.getText().str;
                output_filename = output_dir+"/DP-FMPT-from-"+filename;
                strWriteToFile(output_filename, str);
                append("DP (in FMPT format) saved in '"+output_filename+"'.", log_filename.c_str());
            }

            //Given that here the generated pair (PP, DP) is valid,
            //all operative outsider RPs which aren't obstructed, can be:
            //- in the origin positions, in their final position after execute the DP.

            //captures the initial positions of the RPs in a PPA list
            TPairPositionAnglesList IPL;
            FMM.RPL.getPositions(IPL);

            //save IPL if any
            if(saveIPL) {
                str = TActuator::getPositionPPALabelsRow().str;
                str += "\r\n"+IPL.getColumnText().str;
                output_filename = output_dir+"/IPL-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Initial Position List (in Pair Position Angles in steps) saved in '"+output_filename+"'.", log_filename.c_str());
            }

            //Other whay to obtain the initial position list directly in text format:
            //  FMM.RPL.getPositionsPPATableText()

            //captures the corner of arms in the initial positions in cartesian coordinates
            TItemsList<TDoublePoint> IPL_Corners1;
            FMM.RPL.getPositionsCorners1(IPL_Corners1);
            TItemsList<TDoublePoint> IPL_Corners2;
            FMM.RPL.getPositionsCorners2(IPL_Corners2);

            //calculates the displacement of each corner
            TItemsList<TDoublePoint> Disp_Corners1(OPL_Corners1);
            for(int i=0; i<Disp_Corners1.getCount(); i++) {
                Disp_Corners1[i].x -= IPL_Corners1[i].x;
                Disp_Corners1[i].y -= IPL_Corners1[i].y;
            }
            TItemsList<TDoublePoint> Disp_Corners2(OPL_Corners2);
            for(int i=0; i<Disp_Corners2.getCount(); i++) {
                Disp_Corners2[i].x -= IPL_Corners2[i].x;
                Disp_Corners2[i].y -= IPL_Corners2[i].y;
            }

            //save the displacements of the corners 1 in text format, if any
            if(saveDisp_Corners1) {
                //Disp_Corners1.Print = TDoublePoint::PrintRow;
                //str = Disp_Corners1.getText().str;
                str = "Id\tdx\tdy";
                for(int i=0; i<FMM.RPL.getCount(); i++) {
                    str += "\r\n";
                    TRoboticPositioner *RP = FMM.RPL[i];
                    str += RP->getActuator()->getIdText().str;
                    str += "\t";
                    str += Disp_Corners1[i].getRowText().str;
                }
                output_filename = output_dir+"/Disp_Corners1-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Displacement corners 1 saved in '"+output_filename+"'.", log_filename.c_str());
            }

            //save the displacements of the corners 2 in text format, if any
            if(saveDisp_Corners2) {
                //Disp_Corners2.Print = TDoublePoint::PrintRow;
                //str = Disp_Corners2.getText().str;
                str = "Id\tdx\tdy";
                for(int i=0; i<FMM.RPL.getCount(); i++) {
                    str += "\r\n";
                    TRoboticPositioner *RP = FMM.RPL[i];
                    str += RP->getActuator()->getIdText().str;
                    str += "\t";
                    str += Disp_Corners2[i].getRowText().str;
                }
                output_filename = output_dir+"/Disp_Corners2-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Displacement corners 2 saved in '"+output_filename+"'.", log_filename.c_str());
            }

            //save PP in a file, in MCS format, if any
            if(savePP) {
                PP.getInterfaceText(str, "pos", Bid, IPL, r2_negative);
                output_filename = output_dir+"/PP-from-"+filename;
                strWriteToFile(output_filename, str);
                append("PP (in MCS format) saved in '"+output_filename+"'.", log_filename.c_str());
            }

            //save DP in a file, in MCS format, if any
            if(saveDP) {
                DP.getInterfaceText(str, "depos", Bid, OPL, r2_negative);
                output_filename = output_dir+"/DP-from-"+filename;
                strWriteToFile(output_filename, str);
                append("DP (in MCS format) saved in '"+output_filename+"'.", log_filename.c_str());
            }

            //set the pair (PP, DP) in the output structure
            outputs.PP = PP;
            outputs.DP = DP;

            //save the fiberMOS file, if any
            if(saveFiberMOS) {
                outputs.getFiberMOSText(str, Bid, OPL, IPL, r2_negative);
                output_filename = output_dir+"/fiberMOS.txt";
                strWriteToFile(output_filename, str);
                append("Pair (PP, DP) (in fiberMOS format) saved in '"+output_filename+"'.", log_filename.c_str());
            }

            //get the outputs file
            outputs.getOutputsText(str, Bid, OPL, IPL, r2_negative);
            output_filename = output_dir+"/outputs-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Pair (PP, DP) (in the output format) saved in '"+output_filename+"'.", log_filename.c_str());

            //save PP-Dmin in a file, if any
            if(savePP_Dmin) {
                PP.getDminInterfaceText(str, "pos", Bid);
                output_filename = output_dir+"/PP-Dmin-from-"+filename;
                strWriteToFile(output_filename, str);
                append("PP-Dmin saved in '"+output_filename+"'.", log_filename.c_str());
            }

            //save PP-Dend in a file, if any
            if(savePP_Dend) {
                PP.getDendInterfaceText(str, "pos", Bid);
                output_filename = output_dir+"/PP-Dend-from-"+filename;
                strWriteToFile(output_filename, str);
                append("PP-Dend saved in '"+output_filename+"'.", log_filename.c_str());
            }

            //save DP-Dmin in a file, if any
            if(saveDP_Dmin) {
                DP.getDminInterfaceText(str, "depos", Bid);
                output_filename = output_dir+"/DP-Dmin-from-"+filename;
                strWriteToFile(output_filename, str);
                append("DP-Dmin saved in '"+output_filename+"'.", log_filename.c_str());
            }

            //save DP-Dend in a file, if any
            if(saveDP_Dend) {
                DP.getDendInterfaceText(str, "depos", Bid);
                output_filename = output_dir+"/DP-Dend-from-"+filename;
                strWriteToFile(output_filename, str);
                append("DP-Dend saved in '"+output_filename+"'.", log_filename.c_str());
            }
        }
        else {
            //Given that here the generated pair (PP, DP) is invalid,
            //all operative outsider RPs which aren't obstructed, can be:
            //- in the first position where the collision was detected.

            //print the result of generation of the DP
            if(DPvalid)
                append("Generated DP is valid.", log_filename.c_str());
            else
                append("Generated DP is not valid.", log_filename.c_str());

            //print the result of generation of the PP
            if(PPvalid)
                append("Generated PP is valid.", log_filename.c_str());
            else
                append("Generated PP is not valid.", log_filename.c_str());
        }

        if(Obstructed.getCount() > 0)
            append("There are obstructed RPs: "+Obstructed.getText().str, log_filename.c_str());

        if(Collided.getCount() > 0)
            append("There are collided RPs: "+Collided.getText().str, log_filename.c_str());

        //save the other outputs in the corresponding file
        str = "DPvalid: "+BoolToStr(DPvalid,true).str;
        str += "\r\nPPvalid: "+BoolToStr(PPvalid,true).str;
        str += "\r\nCollided: "+Collided.getText().str;
        str += "\r\nObstructed: "+Obstructed.getText().str;
        if(DP.thereIsSomeComment1())
            str += "\r\nDP comments:\r\n"+DP.getComment1sColumnText();
        if(PP.thereIsSomeComment1())
            str += "\r\nPP comments:\r\n"+PP.getComment1sColumnText();
        output_filename = output_dir+"/other_outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "generating pair (PP, DP) offline: ");
        throw;
    }
    catch(...) {
        throw;
    }
}

//Test the function generatePairPPDP_offline.
void test_generatePairPPDP_offline(TFiberMOSModel& FMM, string& log_filename)
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

        //outputs parameters of the function generatePairPPDP_offline
        TRoboticPositionerList Collided, Obstructed;
        TMotionProgram PP, DP;
        bool PPvalid, DPvalid;

        //inputs parameters of the function generatePairPPDP_offline
        string input_path; //path of input file
        string output_dir; //folder to put the outputss

        //other variables
        int count = 0; //number os simulations
        string filename; //filename of reference including the number of simulation
        string output_filename; //output filename each time
        string str; //string to writo to file each time

        //initialize the output directory
        output_dir = "test_generatePairPPDP_offline";
        ForceDirectories(AnsiString(output_dir));

        do {
            //contabilize the test and print the test tittle
            append("TEST "+inttostr(++count)+":", log_filename.c_str());
            append("========================================================================", log_filename.c_str());

            //build the filename of reference
            filename = "test-"+inttostr(count)+".txt";

            //-------------------------------------------------------------------
            //GENERATES A FILE CONTAINING A RANDOM FMOSA TABLE:

            //add to the MPG an allocation for each RP of the FMM
            Destroy(MPG);
            for(int i=0; i<FMM.RPL.getCount(); i++)
                MPG.AddAllocation(i);

            //randomize the projection points in the domain of their attached RPs
            FMM.RPL.setPurpose(pGenPairPPDP);
            append("FMM configured for generate a pair (PP, DP). (Purpose = GenPairPPDP).", log_filename.c_str());
            MPG.RandomizeWithoutCollision();
            append("Projection points randomized avoinding collisions.", log_filename.c_str());

            //WARNING: sentence could take several seconds:
            //  MPG.RandomizeWithoutCollision();

            //save the allocation table
            str = TAllocation::GetIdPPLabelsRow().str;
            str += "\r\n"+MPG.getColumnText().str;
            output_filename = output_dir+"/AL-for-"+filename;
            strWriteToFile(output_filename, str);
            append("Allocation list saved in '"+output_filename+"'.", log_filename.c_str());

            //build a FMOSA table and set the allocations
            TFMOSATable FMOSAT;
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

                //add the OS to the FMOSA table
                FMOSAT.Add(OS);
            }

            //save the FMOSA table in a file
            FMOSAT.getTableText(str);
            output_filename = output_dir+"/FMOSA-for-"+filename;
            strWriteToFile(output_filename, str);
            append("FMOSA table saved in '"+output_filename+"'.", log_filename.c_str());

            //-------------------------------------------------------------------
            //TEST THE FUNCTION FOR GENERATE PAIRS (PP, DP) OFFLINE:

            //initializes the parameters of the function:
            Destroy(MPG);
            input_path = output_filename;

            //call the function to test
            append("Calling function generatePairPPDP_offline...", log_filename.c_str());
            append("------------------------------------------------------------------------", log_filename.c_str());
            append("generatePairPPDP_offline(PPvalid, DPvalid, Collided, Obstructed, PP, DP,", log_filename.c_str());
            append("                         FMM, input_path, output_dir, log_filename);", log_filename.c_str());
            generatePairPPDP_offline(PPvalid, DPvalid, Collided, Obstructed, PP, DP,
                                     FMM, input_path, output_dir, log_filename,
                                     true, true, true, true, true,
                                     true, true, true, true, true,
                                     true, true, true, true, true, true);
            append("------------------------------------------------------------------------", log_filename.c_str());
            append("Returned from function generatePairPPDP_offline.", log_filename.c_str());

            //WARNING: sentence could take several seconds:
            //  generatePairPPDP_offline(PPvalid, DPvalid, Collided, Obstructed, PP, DP,
            //                           FMM, input_path, output_dir, log_filename);

            //Note that the outputs are saved in the function to test.

            //prepare the standard output for the next test
            append("", log_filename.c_str());
        }
        while(PPvalid && DPvalid && Collided.getCount()<=0 && Obstructed.getCount()<=0);

        //indicates that the test has failed
        append("\r\nFailed test "+inttostr(count)+"!", log_filename.c_str());
        append("PPvalid: "+BoolToStr(PPvalid, true).str, log_filename.c_str());
        append("DPvalid: "+BoolToStr(DPvalid, true).str, log_filename.c_str());
        append("Collided: "+Collided.gettText().str, log_filename.c_str());
        append("Obstructed: "+Obstructed.gettText().str, log_filename.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "testing generatePairPPDP_offline: ");
        throw;
    }
}

//Test the function generatePairPPDP_online.
void test_generatePairPPDP_online(TFiberMOSModel& FMM, string& log_filename)
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

        //outputs parameters of the function generatePairPPDP_online
        bool PairPPDPvalid;
        TMotionProgram PP, DP;

        //inputs parameters of the function generatePairPPDP_online
        vector<double> p_1s, p___3s;
        vector<int> Ids;

        //other variables
        int Bid = 0; //block identification
        string filename; //filename of reference including the Bid
        string output_dir; //folder to put the outputss
        string output_filename; //output filename each time
        string str; //string to write to file each time

        output_dir = "test_generatePairPPDP_online";
        ForceDirectories(AnsiString(output_dir));

        do {
            //contabilize the test and print the test tittle
            append("TEST "+inttostr(++Bid)+":", log_filename.c_str());
            append("========================================================================", log_filename.c_str());

            //build the filename of reference
            filename = "test-"+inttostr(Bid)+".txt";

            //-------------------------------------------------------------------
            //RANDOMIZE THE POSITION ANGLES WITHOUT COLLISIONS AND SAVE THE ALLOCATIONS:

            //add to the MPG an allocation for each RP of the FMM
            Destroy(MPG);
            for(int i=0; i<FMM.RPL.getCount(); i++)
                MPG.AddAllocation(i);

            //randomize the projection points in the domain of their attached RPs
            FMM.RPL.setPurpose(pGenPairPPDP);
            append("FMM configured for generate a pair (PP, DP). (Purpose = GenPairPPDP).", log_filename.c_str());
            MPG.RandomizeWithoutCollision();
            append("Projection points randomized avoinding collisions.", log_filename.c_str());

            //WARNING: sentence could take several seconds:
            //  MPG.RandomizeWithoutCollision();

            //save the allocation table
            str = TAllocation::GetIdPPLabelsRow().str;
            str += "\r\n"+MPG.getColumnText().str;
            output_filename = output_dir+"/AL-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Allocation list saved in '"+output_filename+"'.", log_filename.c_str());

            //-------------------------------------------------------------------
            //TEST THE FUNCTION FOR GENERATE PAIRS (PP, DP) ONLINE:

            //move the RPs to the more closer stable point to the projection points
            MPG.MoveToTargetP3();
            if(MPG.getCount() > 0)
                append("RPs moved to observing positions.", log_filename.c_str());
            else
                append("There aren't RPs to be moved to observing positions.", log_filename.c_str());

            //WARNING: sentence could take several seconds:
            //  MPG.MoveToTargetP3();

            //A PPA list shall be stored how a table (Id, p_1, p___3).

            //captures the observing positions of the RPs in a PPA list
            TPairPositionAnglesList OPL;
            FMM.RPL.getPositions(OPL);
            str = TActuator::getPositionPPALabelsRow().str;
            str += "\r\n"+OPL.getColumnText().str;
            ForceDirectories(AnsiString(output_dir));
            string output_filename = output_dir+"/OPL-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Observing position list saved in '"+output_filename+"'.", log_filename.c_str());

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
            append("Input parameters extracted from the FMM.", log_filename.c_str());

            //move the RPs to their origin positions
            FMM.RPL.moveToOrigins();
            append("RPs moved to origins.", log_filename.c_str());

            //Move the FMM to the origins in convenient because this function
            //is for test the generating function.

            //determine the RPs to be disabled
            Ids.clear();
            //TBD

            //call the function to test
            append("Calling function generatePairPPDP_online...", log_filename.c_str());
            append("----------------------------------------------------------------", log_filename.c_str());
            append("PairPPDPvalid = generatePairPPDP_online(PP, DP,", log_filename.c_str());
            append("                                        FMM, p_1s, p___3s, Ids);", log_filename.c_str());
            PairPPDPvalid = generatePairPPDP_online(PP, DP,
                                                    FMM, p_1s, p___3s, Ids);
            append("----------------------------------------------------------------", log_filename.c_str());
            append("Returned from function generatePairPPDP_online.", log_filename.c_str());

            //WARNING: sentence could take several seconds:
            //  PairPPDPvalid = generatePairPPDP_online(PP, DP,
            //                                          FMM, p_1s, p___3s, Ids);

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
            if(PairPPDPvalid) {
                //indicates the result of the generation
                append("Generated pair (PP, DP) is valid.", log_filename.c_str());

                //save the PP in the format of the FMPT
                str = PP.getText().str;
                output_filename = output_dir+"/PP-FMPT-from-"+filename;
                strWriteToFile(output_filename, str);
                append("PP in FMPT format saved in '"+output_filename+"'.", log_filename.c_str());

                //save the DP in the format of the FMPT
                str = DP.getText().str;
                output_filename = output_dir+"/DP-FMPT-from-"+filename;
                strWriteToFile(output_filename, str);
                append("DP in FMPT format saved in '"+output_filename+"'.", log_filename.c_str());

                //Given that here the generated pair (PP, DP) is valid,
                //all operative outsider RPs which aren't obstructed, should be:
                //- in the origin positions.
                //Because function TMotionProgramGenerator::generatePairPPDP,
                //test first the DP and after the PP, but at the end
                //move all RPs to their initial positions.

                //captures the initial positions of the RPs in a PPA list
                TPairPositionAnglesList IPL;
                FMM.RPL.getPositions(IPL);
                string str = TActuator::getPositionPPALabelsRow().str;
                str += "\r\n"+IPL.getColumnText().str;
                string output_filename = output_dir+"/IPL-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Initial position list saved in '"+output_filename+"'.", log_filename.c_str());

                //Other whay to obtain the observing position table directly in text format:
                //  FMM.RPL.getPositionsPPATableText()

                //translates the PP to the format of the interface MCS-FMPT
                //and save it in a file
                PP.getInterfaceText(str, "pos", Bid, IPL, false);
                append("Positiong program translated to the MCS format.", log_filename.c_str());
                output_filename = output_dir+"/PP-from-"+filename;
                strWriteToFile(output_filename, str);
                append("PP in MCS format saved in '"+output_filename+"'.", log_filename.c_str());

                //translates the DP to the format of the interface MCS-FMPT
                //and save it in a file
                DP.getInterfaceText(str, "depos", Bid, OPL, false);
                append("Depositiong program translated to the MCS format.", log_filename.c_str());
                output_filename = output_dir+"/DP-from-"+filename;
                strWriteToFile(output_filename, str);
                append("DP in MCS format saved in '"+output_filename+"'.", log_filename.c_str());
            }
            else {
                //Given that here the generated pair (PP, DP) is invalid,
                //all operative outsider RPs which aren't obstructed, should be:
                //- in the first position where the collision was detected.
                //During the test of DP or the test of PP.

                //print the result of generation of the PP
                append("Generated Pair (PP, DP) is not valid, because either PP nor DP is invalid.", log_filename.c_str());
            }

            //Below alll instruction about Obstructed or Collided has been comented:
            //
            //if(Obstructed.getCount() > 0)
            //    append("There are obstructed RPs: "+Obstructed.getText().str, log_filename.c_str());
            //
            //if(Collided.getCount() > 0)
            //    append("There are collided RPs: "+Collided.getText().str, log_filename.c_str());

            //print the other outputs in the corresponding file
            str = "PairPPDPvalid: "+BoolToStr(PairPPDPvalid,true).str;
            //str += "\r\nCollided: "+Collided.getText().str;
            //str += "\r\nObstructed: "+Obstructed.getText().str;
            if(DP.thereIsSomeComment1())
                str += "\r\nDP comments:\r\n"+DP.getComment1sColumnText();
            if(PP.thereIsSomeComment1())
                str += "\r\nPP comments:\r\n"+PP.getComment1sColumnText();
            output_filename = output_dir+"/other_outputs-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

            //END OF SAVE THE OUTPUTS

            //prepare the standard output for the next test
            append("", log_filename.c_str());
        }
        while(PairPPDPvalid); // && Collided.getCount()<=0 && Obstructed.getCount()<=0);

        //Note that not collided and not obstructed should be part of the stop condition,
        //but they are discarted in the function of generation online.

        //indicates that the test has failed
        append("\r\nFailed test "+inttostr(Bid)+"!", log_filename.c_str());
        append("PairPPDPvalid: "+BoolToStr(PairPPDPvalid, true).str, log_filename.c_str());
        //append("Collided: "+Collided.gettText().str, log_filename.c_str());
        //append("Obstructed: "+Obstructed.gettText().str, log_filename.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "testing generatePairPPDP_online: ");
        throw;
    }
}

//Determine if all RPs included in the pair (PP, DP), are enabled.
//The instance of the Fiber MOS Model will not be written.
void checkPairPPDP(TFiberMOSModel& FMM, string& path_PP, string& path_DP, string& log_filename)
{
    try {

        //LOAD SETTINGS FROM FILES:

        //load the PP from a file
        TMotionProgram PP;
        string PP_label;
        string str;
        strReadFromFile(str, path_PP);
        unsigned int PP_Bid;
        PP.setInterfaceText(PP_label, PP_Bid, str);
        append("PP loaded from '"+path_PP+"'.", log_filename.c_str());

        //check the precondition
        if(PP_label != "pos")
            throw EImproperArgument("PP label should be \"pos\"");

        //load the DP from a file
        TMotionProgram DP;
        string DP_label;
        //str.clear();
        strReadFromFile(str, path_DP);
        unsigned int DP_Bid;
        DP.setInterfaceText(DP_label, DP_Bid, str);
        append("DP loaded from '"+path_DP+"'.", log_filename.c_str());

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
            append("All RPs included in the pair (PP, DP) are operatives.", log_filename.c_str());
        else
            append("Not all RPs included in the pair (PP, DP) are operatives.", log_filename.c_str());

        //split the path of the file containing the PP
        string parent_path_PP, filename_PP;
        splitpath(parent_path_PP, filename_PP, path_PP);

        //The filename will be used to attach the outputs filenames witht the input filename.

        //split the path of the file containing the DP
        string parent_path_DP, filename_DP;
        splitpath(parent_path_DP, filename_DP, path_DP);

        //The filename will be used to attach the outputs filenames witht the input filename.

        //PROBLEM: el nombre del archivo 'other_outputs-from...' se contruye a partir
        //del filename de path_FMOSA, pero en esta función, solo se pasan path_PP y path_DP.

        //print the other outputs in the corresponding file
        str = "checkPairPPDP: "+BoolToStr(valid, true).str;
        string output_filename = "other_outputs-from-"+filename_PP+"-"+filename_DP;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "checking pair (PP, DP): ");
        throw;
    }
}

//Check if a pair (PP, DP) avoid collisions,
//wehn it is executed starting from the origin.
void validatePairPPDP(TFiberMOSModel& FMM, string& path_outputs, string output_dir, string& log_filename)
{
    try {
        //LOAD SETTINGS FROM FILES:

        //load the Outputs structure from a file
        string str;         //auxiliary string
        Outputs outputs;    //outputs structure
        unsigned int Bid;   //the block identificator in the FMOSA FILE
        try {
            //load the outputs structure
            strReadFromFile(str, path_outputs);
            outputs.setText(Bid, str);

        } catch(Exception& E) {
            E.Message.Insert(1, "reading FMOSA file: ");
            throw;
        }
        append("Outputs structure loaded from '"+path_outputs+"'.", log_filename.c_str());

        //Now the PP and the DP will be validated.

        //---------------------------------------------
        //MAKE THE OPERATIONS:

        //split the path of the file containing the FMOSA table
        string parent_path, filename;
        splitpath(parent_path, filename, path_outputs);
        string output_filename; //auxiliary string

        //The filename will be used to attach the outputs filenames witht the input filename.

        //built a MPV attached to the FMM
        TMotionProgramValidator MPV(&FMM);

        //configure the SPM of all RPs for validate the PP and validate it
        FMM.RPL.setPurpose(pValPP);
        append("FMM configured for generate a pair (PP, DP). (Purpose = ValPP).", log_filename.c_str());
        bool PPvalid = MPV.validateMotionProgram(outputs.PP);

        //initalize the flags for indicate the validity of the DP
        bool DPvalid = false;

        //if the PP avoid collisions
        if(PPvalid) {
            //indicates the result of the validation
            append("The PP is valid.", log_filename.c_str());

            //Given that here the PP is valid,
            //all operative outsider RPs which aren't obstructed, can be:
            //- in the observing positions, in their final position after execute the PP.

            //A PPA table shall be stored how a table (Id, p_1, p___3).

            //captures the observing positions of the RPs in a PPA list
            TPairPositionAnglesList OPL;
            FMM.RPL.getPositions(OPL);
            str = TActuator::getPositionPPALabelsRow().str;
            str += "\r\n"+OPL.getColumnText().str;
            ForceDirectories(AnsiString(path_outputs));
            output_filename = output_dir+"/OPL-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Observing position list saved in '"+output_filename+"'.", log_filename.c_str());

            //Other whay to obtain the observing position table directly in text format:
            //  FMM.RPL.getPositionsPPATableText()

            //captures the observing positions of the RPs in cartesian coordinates respect S0
            str = TActuator::getPositionP3LabelsRow().str;
            str += "\r\n";
            str += FMM.RPL.getPositionsP3TableText().str;
            output_filename = output_dir+"/OPL_S0-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Observing position list (in cartesian coordinates respect S0) saved in '"+output_filename+"'.", log_filename.c_str());

            //captures the observing positions of the RPs in cartesian coordinates respect S1
            str = TActuator::getPositionP_3LabelsRow().str;
            str += "\r\n";
            str += FMM.RPL.getPositionsP_3TableText().str;
            output_filename = output_dir+"/OPL_S1-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Observing position list (in cartesian coordinates respect S1) saved in '"+output_filename+"'.", log_filename.c_str());

            //captures the corner of arms in the observing positions in cartesian coordinates
            TItemsList<TDoublePoint> OPL_Corners1;
            FMM.RPL.getPositionsCorners1(OPL_Corners1);
            TItemsList<TDoublePoint> OPL_Corners2;
            FMM.RPL.getPositionsCorners2(OPL_Corners2);

            //save PP-Dmin in a file
            outputs.PP.getDminInterfaceText(str, "pos", Bid);
            output_filename = output_dir+"/PP-Dmin-from-"+filename;
            strWriteToFile(output_filename, str);
            append("PP-Dmin saved in '"+output_filename+"'.", log_filename.c_str());

            //save PP-Dend in a file
            outputs.PP.getDendInterfaceText(str, "pos", Bid);
            output_filename = output_dir+"/PP-Dend-from-"+filename;
            strWriteToFile(output_filename, str);
            append("PP-Dend saved in '"+output_filename+"'.", log_filename.c_str());

            //configure the SPM for validate the PP and validate it
            FMM.RPL.setPurpose(pValDP);
            append("FMM configured for generate a pair (PP, DP). (Purpose = ValDP).", log_filename.c_str());
            DPvalid = MPV.validateMotionProgram(outputs.DP);

            //SAVE THE OUTPUTS AND PRINT THE CORRESPONDING MESSAGES:

            //if the pair (PP, DP) is valid
            if(DPvalid) {
                //indicates the result of the validation
                append("The DP is valid.", log_filename.c_str());

                //Given that here the pair (PP, DP) is valid,
                //all operative outsider RPs which aren't obstructed, can be:
                //- in the origin positions, in their final position after execute the DP.

                //captures the initial positions of the RPs in a PPA list
                TPairPositionAnglesList IPL;
                FMM.RPL.getPositions(IPL);
                string str = TActuator::getPositionPPALabelsRow().str;
                str += "\r\n"+IPL.getColumnText().str;
                string output_filename = output_dir+"/IPL-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Initial position list saved in '"+output_filename+"'.", log_filename.c_str());

                //Other whay to obtain the initial position list directly in text format:
                //  FMM.RPL.getPositionsPPATableText()

                //captures the corner of arms in the initial positions in cartesian coordinates
                TItemsList<TDoublePoint> IPL_Corners1;
                FMM.RPL.getPositionsCorners1(IPL_Corners1);
                TItemsList<TDoublePoint> IPL_Corners2;
                FMM.RPL.getPositionsCorners2(IPL_Corners2);

                //calculates the displacement of each corner
                TItemsList<TDoublePoint> Disp_Corners1(OPL_Corners1);
                for(int i=0; i<Disp_Corners1.getCount(); i++) {
                    Disp_Corners1[i].x -= IPL_Corners1[i].x;
                    Disp_Corners1[i].y -= IPL_Corners1[i].y;
                }
                TItemsList<TDoublePoint> Disp_Corners2(OPL_Corners2);
                for(int i=0; i<Disp_Corners2.getCount(); i++) {
                    Disp_Corners2[i].x -= IPL_Corners2[i].x;
                    Disp_Corners2[i].y -= IPL_Corners2[i].y;
                }

                //save the displacements in text format
                //            Disp_Corners1.Print = TDoublePoint::PrintRow;
                //          str = Disp_Corners1.getText().str;
                str = "Id\tdx\tdy";
                for(int i=0; i<FMM.RPL.getCount(); i++) {
                    str += "\r\n";
                    TRoboticPositioner *RP = FMM.RPL[i];
                    str += RP->getActuator()->getIdText().str;
                    str += "\t";
                    str += Disp_Corners1[i].getRowText().str;
                }
                output_filename = output_dir+"/Disp_Corners1-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Displacement corners 1 saved in '"+output_filename+"'.", log_filename.c_str());
                //            Disp_Corners2.Print = TDoublePoint::PrintRow;
                //          str = Disp_Corners2.getText().str;
                str = "Id\tdx\tdy";
                for(int i=0; i<FMM.RPL.getCount(); i++) {
                    str += "\r\n";
                    TRoboticPositioner *RP = FMM.RPL[i];
                    str += RP->getActuator()->getIdText().str;
                    str += "\t";
                    str += Disp_Corners2[i].getRowText().str;
                }
                output_filename = output_dir+"/Disp_Corners2-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Displacement corners 2 saved in '"+output_filename+"'.", log_filename.c_str());

                //save DP-Dmin in a file
                outputs.DP.getDminInterfaceText(str, "depos", Bid);
                output_filename = output_dir+"/DP-Dmin-from-"+filename;
                strWriteToFile(output_filename, str);
                append("DP-Dmin saved in '"+output_filename+"'.", log_filename.c_str());

                //save DP-Dend in a file
                outputs.DP.getDendInterfaceText(str, "depos", Bid);
                output_filename = output_dir+"/DP-Dend-from-"+filename;
                strWriteToFile(output_filename, str);
                append("DP-Dend saved in '"+output_filename+"'.", log_filename.c_str());
            }
        }

        //if the pair (PP, DP) is not valid
        if(!PPvalid || !DPvalid) {
            //Given that here the generated pair (PP, DP) is invalid,
            //all operative outsider RPs which aren't obstructed, can be:
            //- in the first position where the collision was detected.

            //print the result of generation of the DP
            if(DPvalid)
                append("The DP is valid.", log_filename.c_str());
            else
                append("The DP is not valid.", log_filename.c_str());

            //print the result of generation of the PP
            if(PPvalid)
                append("The PP is valid.", log_filename.c_str());
            else
                append("The PP is not valid.", log_filename.c_str());
        }

        //print the other outputs in the corresponding file
        str = "PPvalid: "+BoolToStr(PPvalid,true).str;
        str += "\r\nDPvalid: "+BoolToStr(DPvalid,true).str;
        if(outputs.PP.thereIsSomeComment1())
            str += "\r\nPP comments:\r\n"+outputs.PP.getComment1sColumnText();
        if(outputs.DP.thereIsSomeComment1())
            str += "\r\nDP comments:\r\n"+outputs.DP.getComment1sColumnText();
        output_filename = output_dir+"/other_outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());
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
void regeneratePairPPDP_offline(TFiberMOSModel& FMM, string& path_PP, string& path_DP, string& path_FMOSA, string& log_filename)
{
    try {

        //LOADS SETTINGS FROM FILES:

        //load the PP from a file
        TMotionProgram PP;
        string PP_label;
        unsigned int PP_Bid;
        string str;
        strReadFromFile(str, path_PP);
        PP.setInterfaceText(PP_label, PP_Bid, str);
        append("PP loaded from '"+path_PP+"'.", log_filename.c_str());

        //check the precondition
        if(PP_label != "pos")
            throw EImproperArgument("PP label should be \"pos\"");

        //load the DP from a file
        TMotionProgram DP;
        string DP_label;
        unsigned int DP_Bid;
        strReadFromFile(str, path_DP);
        DP.setInterfaceText(DP_label, DP_Bid, str);
        append("DP loaded from '"+path_DP+"'.", log_filename.c_str());

        //check the precondition
        if(DP_label != "depos")
            throw EImproperArgument("DP label should be \"depos\"");
        if(DP_Bid != PP_Bid)
            throw EImproperArgument("DP Bid should be equal to PP Bid");

        //load the FMOSA table from the file path_FMOSA
        TFMOSATable FMOSA;
        unsigned int FMOSA_Bid;
        strReadFromFileWithComments(str, path_FMOSA);
        FMOSA.setTableText(FMOSA_Bid, str);
        append("FMOSA table loaded from '"+path_FMOSA+"'.", log_filename.c_str());

        if(FMOSA_Bid!=PP_Bid || FMOSA_Bid!=DP_Bid)
            throw EImproperArgument("FMOSA Bid should be equal to PP Bid and DP Bid");

        //get the data from the FMOSA table
        TMotionProgramGenerator MPG(&FMM);
        FMOSA.getAllocations(MPG);
        append("Got data from  the FMOSA table.", log_filename.c_str());

        //-------------------------------------------------------------------
        //MAKE THE OPERATIONS:

        //split the path of the file containing the PP table
        string parent_path, filename;
        splitpath(parent_path, filename, path_FMOSA);

        //The filename will be used to attach the outputs filenames witht the input filename.

        //get the initial position list
        TPairPositionAnglesList IPL;
        FMM.RPL.getPositions(IPL);
        append("Initial position list got from the Fiber MOS Model.", log_filename.c_str());

        //The IPL is useful to get the PP in the interface format.

        //move the RPs to the more closer stable point to the TPs
        MPG.MoveToTargetP3();
        if(MPG.getCount() > 0)
            append("RPs moved to observing positions.", log_filename.c_str());
        else
            append("There aren't RPs to be moved to observing positions.", log_filename.c_str());

        //WARNING: sentence could take several seconds:
        //  MPG.MoveToTargetP3();

        //Other way to obtain the more closer stablepoints to the projection points,
        //consist in get from the FMOSA table the following lists:
        //  the allocation list;
        //  the projection point list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

        //A PPA table shall be stored how a table (Id, p_1, p___3).

        //get the observing position list
        TPairPositionAnglesList OPL;
        FMM.RPL.getPositions(OPL);
        append("Observing position list got from the Fiber MOS Model.", log_filename.c_str());

        //The OPL is useful to get the DP in the interface format.

        //attempt regenerate the pair (PP, DP)
        TVector<int> Excluded;
        bool regenerated = MPG.attemptRegenerate(Excluded, PP, DP);

        string output_filename;

        //print the result
        if(regenerated) {
            append("The pair (PP, DP) has been regenerated excluding the following RPs: "+Excluded.getText().str, log_filename.c_str());

            //save the regenerated PP
            PP.getInterfaceText(str, "pos", PP_Bid, IPL, false);
            append("Positiong program translated to the MCS format.", log_filename.c_str());
            output_filename = "regeneratedPP-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Regenerated PP saved in '"+output_filename+"'.", log_filename.c_str());

            //save the regenerated DP
            DP.getInterfaceText(str, "depos", DP_Bid, OPL, false);
            append("Depositiong program translated to the MCS format.", log_filename.c_str());
            output_filename = "regeneratedDP-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Regenerated DP saved in '"+output_filename+"'.", log_filename.c_str());
        }
        else
            append("The pair (PP, DP) can not be regenerated.", log_filename.c_str());

        //print the other outputs in the corresponding file
        str = "regeneratePairPPDP: "+BoolToStr(regenerated, true).str;
        str += "\r\nExcluded: "+Excluded.getText().str;
        output_filename = "other_outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "regenerating pair (PP, DP): ");
        throw;
    }
}

//Generate a parking program offline.
//Parameters for can use the function for test:
//  (ParkingProgramValid, Collided, Obstructed, PP, DP)
//Parameters for generate a pair PPDP:
//  (FMM, path, log_filename)
void generateParkingProgram_offline(bool& ParkingProgramValid,
                                    TRoboticPositionerList& Collided, TRoboticPositionerList& Obstructed,
                                    TMotionProgram& ParkingProgram,
                                    //------------------------------------------------------
                                    TFiberMOSModel& FMM, const string& input_path, const string& output_dir, const string& log_filename,
                                    //------------------------------------------------------
                                    bool saveSPL, bool saveParkingProgram_FMPT,
                                    bool saveParkingProgram_Dmin, bool saveParkingProgram_Dend,
                                    bool saveFPL, bool r2_negative)
{
    try {

        //LOAD SETTINGS FROM FILES:

        //load the FMOSA table from the file input_path
        TFMOSATable FMOSA;
        unsigned int Bid;
        string str;
        strReadFromFileWithComments(str, input_path);
        FMOSA.setTableText(Bid, str);
        append("FMOSA table loaded from '"+input_path+"'.", log_filename.c_str());

        //get the allocation from the FMOSA table
        TMotionProgramGenerator MPG(&FMM);
        FMOSA.getAllocations(MPG);
        append("Allocations got from the FMOSA table in MPG.", log_filename.c_str());

        //MAKE THE OPERATIONS:

        //split the path of the file containing the FMOSA table
        string parent_path, filename;
        splitpath(parent_path, filename, input_path);

        //The filename will be used to attach the outputs filenames witht the input filename.

        //move the RPs to the more closer stable point to the TPs
        MPG.MoveToTargetP3();
        if(MPG.getCount() > 0)
            append("RPs moved to observing positions.", log_filename.c_str());
        else
            append("There aren't RPs to be moved to observing positions.", log_filename.c_str());

        //WARNING: sentence could take several seconds:
        //  MPG.MoveToTargetP3();

        //Other way to obtain the more closer stable points to the projection points,
        //consist in get from the FMOSA table the following lists:
        //  the allocation list;
        //  the projection point list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

        //A PPA table shall be stored how a table (Id, p_1, p___3).

        //captures the starting positions of the RPs in a PPA list
        TPairPositionAnglesList SPL;
        FMM.RPL.getPositions(SPL);

        //save SPL in a file, if any
        string output_filename;
        if(saveSPL) {
            str = TActuator::getPositionPPALabelsRow().str;
            str += "\r\n"+SPL.getColumnText().str;
            ForceDirectories(AnsiString(output_dir));
            output_filename = output_dir+"/SPL-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Starting position list saved in '"+output_filename+"'.", log_filename.c_str());
        }

        //Other whay to obtain the starting position table directly in text format:
        //  FMM.RPL.getPositionsPPATableText()

        //segregates the operative outsider RPs
        TRoboticPositionerList Outsiders;
        FMM.RPL.segregateOperativeOutsiders(Outsiders);

        //check the limiting case when there aren't allocations
        if(MPG.getCount() <= 0)
            append("WARNING: there aren't allocations. The generated parking program will be empty.", log_filename.c_str());
        //check the limiting case when all operative RPs are in the origin
        else if(FMM.RPL.allOperativeRPsAreInTheOrigin())
            append("WARNING: all operative RPs are in the origin. The generated parking program will be empty.", log_filename.c_str());
        //else, check the limiting case when all operative RPs are in security positions
        else if(Outsiders.getCount() <= 0)
            append("WARNING: all operative RPs are in security positions. The generated parking program will contains only a message-instruction list to go to the origin.", log_filename.c_str());

        //Now are fulfilled the preconditions:
        //  All RPs of the Fiber MOS Model, shall be in their starting positions.
        //  All RPs of the list Outsiders, shall be in the Fiber MOS Model.
        //  All RPs of the list Outsiders, shall be operatives.
        //  All RPs of the list Outsiders, shall be in unsecure positions.
        //  All RPs of the list Outsiders, shall have enabled the quantifiers.

        //generates a parking program for the operative RPs in insecure positions
        //and determines the RPs in collision status or obstructed in insecure positions
        append("Generating a parking program...", log_filename.c_str());
        Collided.Clear();
        Obstructed.Clear();
        ParkingProgram.Clear();
        ParkingProgramValid = MPG.generateParkingProgram(Collided, Obstructed, ParkingProgram, Outsiders);

        //Now are fulfilled the postconditions:
        //  All RPs of the Fiber MOS Model will be configured for MP validation
        //  All RPs of the fiber MOS Model will be in their final positions,
        //  or the first position where the collision was detected.
        //  All RPs of the Fiber MOS Model will have disabled the quantifiers.

        //########################################################################
        //#WARNING: before re-use a function to generation, shall be restablished#
        //#the preconditions.                                                    #
        //########################################################################

        //if generation function was successfully generated
        if(ParkingProgramValid) {
            //indicates that the parking program has been generated successfully
            append("Generated parking program is valid.", log_filename.c_str());

            //save the parking program in the format of the FMPT, if any
            if(saveParkingProgram_FMPT) {
                str = ParkingProgram.getText().str;
                output_filename = output_dir+"/ParkingProgram-FMPT-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Parking program in FMPT format saved in '"+output_filename+"'.", log_filename.c_str());
            }

            //Given that here the generated parking program is valid,
            //all operative outsider RPs which aren't obstructed are in the origin positions,
            //in their final position after execute the MP.

            //translates the parking program to the format of the interface MCS-FMPT
            //and save it in a file
            ParkingProgram.getInterfaceText(str, "depos", Bid, SPL, r2_negative);
            append("Parking program translated to the MCS format.", log_filename.c_str());
            string output_filename = output_dir+"/ParkingProgram-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Parking program in MCS format saved in '"+output_filename+"'.", log_filename.c_str());

            //save ParkingProgram-Dmin in a file, if any
            if(saveParkingProgram_Dmin) {
                ParkingProgram.getDminInterfaceText(str, "depos", Bid);
                output_filename = output_dir+"/ParkingProgram-Dmin-from-"+filename;
                strWriteToFile(output_filename, str);
                append("ParkingProgram-Dmin saved in '"+output_filename+"'.", log_filename.c_str());
            }

            //save ParkingProgram-Dend in a file, if any
            if(saveParkingProgram_Dend) {
                ParkingProgram.getDendInterfaceText(str, "depos", Bid);
                output_filename = output_dir+"/ParkingProgram-Dend-from-"+filename;
                strWriteToFile(output_filename, str);
                append("ParkingProgram-Dend saved in '"+output_filename+"'.", log_filename.c_str());
            }
        }

        //Given that here the generated parking program can be valid or invalid,
        //all operative outsider RPs which aren't obstructed, can be:
        //- in the origin positions, in their final position after execute the MP.
        //  if success == true.
        //- in the first position where the collision was detected.
        //  if success == false.

        if(Obstructed.getCount() > 0)
            append("There are obstructed RPs: "+Obstructed.getText().str, log_filename.c_str());

        if(Collided.getCount() > 0)
            append("There are collided RPs: "+Collided.getText().str, log_filename.c_str());

        //get and save the positions where the collision was detected
        TPairPositionAnglesList FPL;
        FMM.RPL.getPositions(FPL);

        //save FPL in a file, if any
        if(saveFPL) {
            str = TActuator::getPositionPPALabelsRow().str;
            str += "\r\n"+FPL.getColumnText().str;
            output_filename = output_dir+"/FPL-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Final position list saved in '"+output_filename+"'.", log_filename.c_str());
        }

        //save the other outputs in the corresponding file
        str = "ParkingProgramValid: "+BoolToStr(ParkingProgramValid, true).str;
        str += "\r\nCollided: "+Collided.getText().str;
        str += "\r\nObstructed: "+Obstructed.getText().str;
        if(ParkingProgram.thereIsSomeComment1())
            str += "\r\nMP comments:\r\n"+ParkingProgram.getComment1sColumnText();
        output_filename = output_dir+"/other_outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "generating parking program offline: ");
        throw;
    }
}

//Test the function generateParkingProgram_offline.
void test_generateParkingProgram_offline(TFiberMOSModel& FMM, string& log_filename)
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

        //parameters of the function generateParkingProgram_offline
        TRoboticPositionerList Collided, Obstructed;
        TMotionProgram ParkingProgram;
        bool ParkingProgramValid;

        int count = 0; //number os simulations
        string input_path; //path of input file
        string filename; //filename of reference including the number of simulation
        string output_dir; //folder to put the outputss
        string output_filename; //output filename each time
        string str; //string to writo to file each time

        output_dir = "test_generateParkingProgram_offline";
        ForceDirectories(AnsiString(output_dir));

        do {
            //contabilize the test and print the test tittle
            append("TEST "+inttostr(++count)+":", log_filename.c_str());
            append("========================================================================", log_filename.c_str());

            //build the filename of reference
            filename = "test-"+inttostr(count)+".txt";

            //-------------------------------------------------------------------
            //GENERATES A FILE CONTAINING A RANDOM FMOSA TABLE:

            //add to the MPG an allocation for each RP of the FMM
            Destroy(MPG);
            for(int i=0; i<FMM.RPL.getCount(); i++)
                MPG.AddAllocation(i);

            //randomize the projection points in the domain of their attached RPs
            FMM.RPL.setPurpose(pGenParPro);
            append("FMM configured for generate a pair (PP, DP). (Purpose = GenParPro).", log_filename.c_str());
            MPG.RandomizeWithoutCollision();
            append("Projection points randomized avoinding collisions.", log_filename.c_str());

            //WARNING: sentence could take several seconds:
            //  MPG.RandomizeWithoutCollision();

            //save the allocation table
            str = TAllocation::GetIdPPLabelsRow().str;
            str += "\r\n"+MPG.getColumnText().str;
            output_filename = output_dir+"/AL-for-"+filename;
            strWriteToFile(output_filename, str);
            append("Allocation list saved in '"+output_filename+"'.", log_filename.c_str());

            //build a FMOSA table and set the allocations
            TFMOSATable FMOSAT;
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

                //add the OS to the FMOSA table
                FMOSAT.Add(OS);
            }

            //save the FMOSA table in a file
            FMOSAT.getTableText(str);
            output_filename = output_dir+"/FMOSA-for-"+filename;
            strWriteToFile(output_filename, str);
            append("FMOSA table saved in '"+output_filename+"'.", log_filename.c_str());

            //-------------------------------------------------------------------
            //TEST THE FUNCTION TO GENERATE PAIRS (PP, DP):

            //initializes the parameters of the function:
            Destroy(MPG);
            input_path = output_filename;

            //call the function to test
            append("Calling function generateParkingProgram_offline...", log_filename.c_str());
            append("--------------------------------------------------------------------------", log_filename.c_str());
            append("generateParkingProgram_offline(ParkingProgramValid,", log_filename.c_str());
            append("                               Collided, Obstructed, ParkingProgram,", log_filename.c_str());
            append("                               FMM, input_path, output_dir, log_filename,", log_filename.c_str());
            append("                               true, true, true, true, true, true);", log_filename.c_str());
            generateParkingProgram_offline(ParkingProgramValid, Collided, Obstructed, ParkingProgram,
                                           FMM, input_path, output_dir, log_filename,
                                           true, true, true, true, true, true);
            append("--------------------------------------------------------------------------", log_filename.c_str());
            append("Returned from function generateParkingProgram_offline.", log_filename.c_str());

            //WARNING: sentence could take several seconds:
            //  generateParkingProgram_offline(ParkingProgramValid, Collided, Obstructed, ParkingProgram,
            //                                 FMM, input_path, output_dir, log_filename,
            //                                 true, true, true, true, true, true);

            //Note that the outputs are saved in the function to test.

            //prepare the standard output for the next test
            append("", log_filename.c_str());
        }
        while(ParkingProgramValid && Collided.getCount()<=0 && Obstructed.getCount()<=0);

        //indicates that test
        append("\r\nFailed test "+inttostr(count)+"!", log_filename.c_str());
        append("ParkingProgramValid: "+BoolToStr(ParkingProgramValid, true).str, log_filename.c_str());
        append("Collided: "+Collided.gettText().str, log_filename.c_str());
        append("Obstructed: "+Obstructed.gettText().str, log_filename.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "testing generatePairPPDP_offline: ");
        throw;
    }
}

//Test the function generateParkingProgram_online.
void test_generateParkingProgram_online(TFiberMOSModel& FMM, string& log_filename)
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

        //outputs parameters of the function generateParkingProgram_online
        bool ParkingProgramValid;
        TMotionProgram ParkingProgram;

        //inputs parameters of the function generateParkingProgram_online
        vector<double> p_1s, p___3s;
        vector<int> Ids;

        //other variables
        int Bid = 0; //block identification
        string filename; //filename of reference including the Bid
        string output_dir; //folder to put the outputss
        string output_filename; //output filename each time
        string str; //string to writo to file each time

        output_dir = "test_generateParkingProgram_online";
        ForceDirectories(AnsiString(output_dir));

        do {
            //contabilize the test and print the test tittle
            append("TEST "+inttostr(++Bid)+":", log_filename.c_str());
            append("========================================================================", log_filename.c_str());

            //build the filename of reference
            filename = "test-"+inttostr(Bid)+".txt";

            //-------------------------------------------------------------------
            //RANDOMIZE THE POSITION ANGLES WITHOUT COLLISIONS AND SAVE THE ALLOCATIONS:

            //add to the MPG an allocation for each RP of the FMM
            Destroy(MPG);
            for(int i=0; i<FMM.RPL.getCount(); i++)
                MPG.AddAllocation(i);

            //randomize the projection points in the domain of their attached RPs
            FMM.RPL.setPurpose(pGenParPro);
            append("FMM configured for generate a pair (PP, DP). (Purpose = GenParPro).", log_filename.c_str());
            MPG.RandomizeWithoutCollision();
            append("Projection points randomized avoinding collisions.", log_filename.c_str());

            //WARNING: sentence could take several seconds:
            //  MPG.RandomizeWithoutCollision();

            //save the allocation table
            str = TAllocation::GetIdPPLabelsRow().str;
            str += "\r\n"+MPG.getColumnText().str;
            output_filename = output_dir+"/AL-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Allocation list saved in '"+output_filename+"'.", log_filename.c_str());

            //-------------------------------------------------------------------
            //TEST THE FUNCTION FOR GENERATE PAIRS (PP, DP) ONLINE:

            //move the RPs to the more closer stable point to the projection points
            MPG.MoveToTargetP3();
            if(MPG.getCount() > 0)
                append("RPs moved to observing positions.", log_filename.c_str());
            else
                append("There aren't RPs to be moved to observing positions.", log_filename.c_str());

            //WARNING: sentence could take several seconds:
            //  MPG.MoveToTargetP3();

            //A PPA table shall be stored how a table (Id, p_1, p___3).

            //captures the starting positions of the RPs in a PPA list
            TPairPositionAnglesList SPL;
            FMM.RPL.getPositions(SPL);
            str = TActuator::getPositionPPALabelsRow().str;
            str += "\r\n"+SPL.getColumnText().str;
            ForceDirectories(AnsiString(output_dir));
            string output_filename = output_dir+"/SPL-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Starting position list saved in '"+output_filename+"'.", log_filename.c_str());

            //Other whay to obtain the starting position table directly in text format:
            //  FMM.RPL.getPositionsPPATableText()

            //copy the position angles in the input parameters
            p_1s.clear();
            p___3s.clear();
            for(int i=0; i<FMM.RPL.getCount(); i++) {
                TRoboticPositioner *RP = FMM.RPL[i];
                p_1s.push_back(RP->getActuator()->getp_1());
                p___3s.push_back(RP->getActuator()->getArm()->getp___3());
            }
            append("Input parameters extracted from the FMM.", log_filename.c_str());

            //move the RPs to their origin positions
            FMM.RPL.moveToOrigins();
            append("RPs moved to origins.", log_filename.c_str());

            //Move the FMM to the origins in convenient because this function
            //is for test the generating function.

            //determine a number of RPs to be disabled
            Ids.clear();
            //TBD

            //call the function to test
            append("Calling function generateParkingProgram_online...", log_filename.c_str());
            append("----------------------------------------------------------------------------", log_filename.c_str());
            append("ParkingProgramValid = generateParkingProgram_online(ParkingProgram,", log_filename.c_str());
            append("                                                    FMM, p_1s, p___3s, Ids);", log_filename.c_str());
            ParkingProgramValid = generateParkingProgram_online(ParkingProgram,
                                                                FMM, p_1s, p___3s, Ids);
            append("----------------------------------------------------------------------------", log_filename.c_str());
            append("Returned from function generateParkingProgram_online.", log_filename.c_str());

            //WARNING: sentence could take several seconds:
            //  ParkingProgramValid = generateParkingProgram_online(ParkingProgram,
            //                                                      FMM, p_1s, p___3s, Ids);

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
            if(ParkingProgramValid) {
                //indicates the result of the generation
                append("Generated pair (PP, DP) is valid.", log_filename.c_str());

                //save the parking program in the format of the FMPT
                str = ParkingProgram.getText().str;
                output_filename = output_dir+"/ParkingProgram-FMPT-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Parking program in FMPT format saved in '"+output_filename+"'.", log_filename.c_str());

                //Given that here the generated pair ((PP, DP) is valid,
                //all operative outsider RPs which aren't obstructed, should be:
                //- in the origin positions, in their final position after execute the DP.

                //captures the initial positions of the RPs in a PPA list
                TPairPositionAnglesList FPL;
                FMM.RPL.getPositions(FPL);
                string str = TActuator::getPositionPPALabelsRow().str;
                str += "\r\n"+FPL.getColumnText().str;
                string output_filename = output_dir+"/FPL-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Final position list saved in '"+output_filename+"'.", log_filename.c_str());

                //Other whay to obtain the observing position table directly in text format:
                //  FMM.RPL.getPositionsPPATableText()

                //translates the parking program to the format of the interface MCS-FMPT
                //and save it in a file
                ParkingProgram.getInterfaceText(str, "depos", Bid, SPL, false);
                append("Parking program translated to the MCS format.", log_filename.c_str());
                output_filename = output_dir+"/ParkingProgram-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Parking program in MCS format saved in '"+output_filename+"'.", log_filename.c_str());
            }
            else {
                //Given that here the generated parking program is invalid,
                //all operative outsider RPs which aren't obstructed, should be:
                //- in the first position where the collision was detected.

                //print the result of generation of the parking program
                append("Generated ParkingProgram is not valid, because PP or DP is invalid.", log_filename.c_str());
            }

            //Below alll instruction about Obstructed or Collided has been comented:
            //
            //if(Obstructed.getCount() > 0)
            //    append("There are obstructed RPs: "+Obstructed.getText().str, log_filename.c_str());
            //
            //if(Collided.getCount() > 0)
            //    append("There are collided RPs: "+Collided.getText().str, log_filename.c_str());

            //print the other outputs in the corresponding file
            str = "ParkingProgramValid: "+BoolToStr(ParkingProgramValid,true).str;
            //str += "\r\nCollided: "+Collided.getText().str;
            //str += "\r\nObstructed: "+Obstructed.getText().str;
            if(ParkingProgram.thereIsSomeComment1())
                str += "\r\nParkingProgram comments:\r\n"+ParkingProgram.getComment1sColumnText();
            output_filename = output_dir+"/other_outputs-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

            //END OF SAVE THE OUTPUTS

            //prepare the standard output for the next test
            append("", log_filename.c_str());
        }
        while(ParkingProgramValid); // && Collided.getCount()<=0 && Obstructed.getCount()<=0);

        //Note that not collided and not obstructed should be part of the stop condition,
        //but they are discarted in the function of generation online.

        //indicates that the test has failed
        append("\r\nFailed test "+inttostr(Bid)+"!", log_filename.c_str());
        append("ParkingProgramValid: "+BoolToStr(ParkingProgramValid, true).str, log_filename.c_str());
        //append("Collided: "+Collided.gettText().str, log_filename.c_str());
        //append("Obstructed: "+Obstructed.gettText().str, log_filename.c_str());
    }
    catch(Exception& E) {
        E.Message.Insert(1, "testing generateParkingProgram_online: ");
        throw;
    }
}

/*//Generate one animation with the pair (PP, DP).
int visualizePPDP(int argc, char *argv[], TFiberMOSModel& FMM, string& path_PP, string& path_DP, string& log_filename)
{
    try {

        //load the PP from a file
        TMotionProgram PP;
        string PP_label;
        unsigned int PP_Bid;
        string str;
        strReadFromFile(str, path_PP);
        PP.setInterfaceText(PP_label, PP_Bid, str);
        append("PP loaded from '"+path_PP+"'.", log_filename.c_str());

        //check the precondition
        if(PP_label != "pos")
            throw EImproperArgument("PP label should be \"pos\"");

        //load the DP from a file
        TMotionProgram DP;
        string DP_label;
        unsigned int DP_Bid;
        strReadFromFile(str, path_DP);
        DP.setInterfaceText(DP_label, DP_Bid, str);
        append("DP loaded from '"+path_DP+"'.", log_filename.c_str());

        //check the precondition
        if(DP_label != "depos")
            throw EImproperArgument("DP label should be \"depos\"");
        if(DP_Bid != PP_Bid)
            throw EImproperArgument("DP Bid should be equal to PP Bid");

        //TODO
        //set path_frames according to nomenclature based on configuration block and/or number of sources
        //
        //
        //
        //
        string path_frames = "/home/user/MEGARA/megarafmpt/data/Samples/frames/builder10/";
        //        string path_frames = "/home/user/pruebas_choque/magin_3p50/frames/builder10";
        QString pathSequence = QString(path_frames.c_str());

        //check if folder exists
        if (!QDir(pathSequence).exists()){
            QDir().mkdir(pathSequence);
        }

        //check if sequence is already stored in disk
        if(QDir(pathSequence).entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() == 0)
        {

            //generate Frames
            GenerateFrames FrameBuilder;
            FrameBuilder.pathSequence = pathSequence;
            FrameBuilder.FMM = &FMM;
            FrameBuilder.PP = &PP;
            FrameBuilder.DP = &DP;

            append("Generating PP frames in '"+path_frames+"'.", log_filename.c_str());
            FrameBuilder.generatePPFrames();

            append("Generating DP frames in '"+path_frames+"'.", log_filename.c_str());
            FrameBuilder.generateDPFrames();
        }


        //executes Qt GUI app
        QApplication a(argc, argv);
        MainWindow w;

        //set pathSequence dir
        //..
        //QString appPath = QDir().currentPath();
        //QDir dir(appPath+"/../megarafmpt/data/Samples/frames/eclipse/");
        //..
        QDir dir(pathSequence);
        w.setFramesDir(dir);

        w.show();          // visualize PP/DP sequence
        return a.exec();
    }
    catch(Exception& E) {
        E.Message.Insert(1, "visualizing motion progress: ");
        throw;
    }
}
*/
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

    //-------------------------------------------------------------------
    //CONFIGURATE THE SYSTEM:

    //REMEMBER: exceptions in runtime can be due to that
    //the system is not configurated.

    //configurates the decimal separator
    setlocale(LC_NUMERIC, "C");

    //-----------------------------------------------------------------------
    //MAKE ACTIONS:

    string command;
    string log_filename;

    try {
        //if the program is run without the necessary command
        //warn that happened, print the help and finish the program
        if(argc <= 1) {
            //indicates that happened
            cout << "Missing command." << endl;
            //print the help
            cout << "\r\n"+help()+"\r\n" << endl;
            //indicates that the program has been executed without error
            return 0;
        }

        //build a command string
        command = string(argv[1]);

        //reacts according the comands without log file
        if(command == "help") {
            //print the help
            cout << "\r\n"+help()+"\r\n" << endl;
            //indicates that the program has been executed without error
            return 0;
        }
        else if(command == "help_generatePairPPDP_offline") {
            //print the help
            cout << "\r\n"+help_generatePairPPDP_offline()+"\r\n" << endl;
            //indicates that the program has been executed without error
            return 0;
        }
        else if(command == "help_generateParkingProgram_offline") {
            //print the help
            cout << "\r\n"+help_generateParkingProgram_offline()+"\r\n" << endl;
            //indicates that the program has been executed without error
            return 0;
        }
        else if(command == "help_others") {
            //print the help
            cout << "\r\n"+help_others()+"\r\n" << endl;
            //indicates that the program has been executed without error
            return 0;
        }
        else if(command == "aboutOf") {
            //print the about of legend
            cout << "\r\n"+aboutOf() << endl;
            //indicates that the program has been executed without error
            return 0;
        }
        else if(command != "valuesSPM_EA" &&
                command != "valuesSPM_RP" &&
                command != "testRadialMotion" &&
                command != "testDistanceMin" &&
                command != "applyPC" &&
                command != "applyRP" &&
                command != "generatePairPPDP_offline" &&
                command != "test_generatePairPPDP_offline" &&
                command != "test_generatePairPPDP_online" &&
                command != "checkPairPPDP" &&
                command != "validatePairPPDP" &&
                command != "regeneratePairPPDP" &&
                command != "generateParkingProgram_offline" &&
                command != "test_generateParkingProgram_offline" &&
                command != "test_generateParkingProgram_online" &&
                command != "visualizePPDP") {
            //indicates that happened
            cout << "Unknoledge command." << endl;
            //print the help
            cout << "\r\n"+help() << endl;
            //indicates that the program has been executed without error
            return 0;
        }

        //initalize the log file
        log_filename = "fmpt_saa.log";
        char mode[] = "w";
        TTextFile TF(log_filename.c_str(), mode);
        TF.Close();
    }
    catch(Exception &E) {
        //indicates that has happened an exception
        //and show the message of the exception
        cout << "ERROR: "+E.Message.str << endl;
        return 1;
    }
    catch(...) {
        //indicates that has happened an unknoledge exception
        cout << "ERROR: unknowledge exception" << endl;
        return 2;
    }

    try {
        //indicates that the program is running
        append("FMPT SAA 3.8.3 is running...", log_filename.c_str());

        //print the arguments with has called the program
        append("\r\nArguments with has called the program:", log_filename.c_str());
        for(int i=0; i<argc; i++)
            append("    argv["+inttostr(i)+"]: "+string(argv[i]), log_filename.c_str());

        //built the paths where search the Fiber MOS Model
        string dir_FMM1 = DATADIR;
        dir_FMM1 += "/Models/MEGARA_FiberMOSModel_Instance";
        string dir_FMM2 = getCurrentDir()+"/../data/Models/MEGARA_FiberMOSModel_Instance";

        //Qt only:
        //string dir_FMM1 = getCurrentDir()+"/../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance";
        //string dir_FMM2 = dir_FMM1;

        //load the instance of the Fiber MOS Model from a dir
        TFiberMOSModel FMM;
        string dir_FMM;

        try {
            append("\r\nLoading FMM instance from: '"+dir_FMM1+"'.", log_filename.c_str());
            readInstanceFromDir(FMM, dir_FMM1);
            dir_FMM = dir_FMM1;
        }
        catch(Exception& E) {
            append("FMM instance can't be loaded: "+E.Message.str, log_filename.c_str());
            try {
                append("Loading FMM instance from: '"+dir_FMM2+"'.", log_filename.c_str());
                readInstanceFromDir(FMM, dir_FMM2);
                dir_FMM = dir_FMM2;
            }
            catch(Exception& E) {
                E.Message.Insert(1, "loading instance: ");
                throw;
            }
        }
        append("FMM instance loaded.", log_filename.c_str());

        //-------------------------------------------------------------------
        //REACTS ACCORDING THE COMMAND AND THE ARGUMENTS:

        //print a blank line for improve legibility
        append("", log_filename.c_str());

        //-------------------------------------------------------------------
        if(command == "valuesSPM_EA") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command valuesSPM_EA sould have 1 arguments: <Aid>");

            //built a string from arg 1 (<Aid>)
            string str(argv[2]);

            //convert the argument to int
            int Id = strToInt(str);

            //search the EA
            int i = FMM.EAL.searchId(Id);
            if(i >= FMM.EAL.getCount())
                throw EImpossibleError("there isn't a EA with Id = "+inttostr(Id)+": ");
            TExclusionArea *EA = FMM.EAL[i];

            //print the SPM values
            append("The EA"+EA->getIdText().str+" has been found in the FMM, and their SPM values are:", log_filename.c_str());
            append(StrIndent(EA->getSecurityText()).str, log_filename.c_str());
            append("\r\nFor learn about the calculus of SPM values, see the file: '/usr/local/share/megara-fmpt/Models/Calculus_of_SPM.txt'", log_filename.c_str());
        }
        else if(command == "valuesSPM_RP") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command valuesSPM_RP sould have 1 arguments: <Pid>");

            //built a string from arg 1 (<Pid>)
            string str(argv[2]);

            //convert the argument to int
            int Id = strToInt(str);

            //search the RP
            int i = FMM.RPL.searchId(Id);
            if(i >= FMM.RPL.getCount())
                throw EImpossibleError("there isn't a RP with Id = "+inttostr(Id)+": ");
            TRoboticPositioner *RP = FMM.RPL[i];

            //print the SPM values
            append("The RP"+RP->getActuator()->getIdText().str+" has been found in the FMM, and their SPM values are:", log_filename.c_str());
            append(StrIndent(RP->getActuator()->getSecurityText()).str, log_filename.c_str());
            append("\r\nFor learn about the calculus of SPM values, see the file: '/usr/local/share/megara-fmpt/Models/Calculus_of_SPM.txt'", log_filename.c_str());
        }
        else if(command == "testRadialMotion") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command testRadialMotion should have 0 arguments");

            //execute the test
            testRadialMotion(FMM, log_filename);
        }
        else if(command == "testDistanceMin") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command testDistanceMin should have 0 arguments");

            //execute the test
            testDistanceMin(FMM, log_filename);
        }
        //-------------------------------------------------------------------
        else if(command == "applyPC") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command applyPC sould have 1 arguments: <path_PC>");

            //built a path from arg 1
            string path_PC(argv[2]);

            //make a rutinary check
            if(path_PC.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(path_PC[0] != '/')
                path_PC.insert(0, getCurrentDir()+"/");

            //apply the positioner center table to the Fiber MOS Model Instance
            applyPC(FMM, path_PC, log_filename);

            //save the instance of the Fiber MOS Model from where was loaded
            writeInstanceToDir(dir_FMM, FMM);
            append("Fiber MOS Model instance saved in '"+dir_FMM+"'.", log_filename.c_str());
        }
        else if(command == "applyRP") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command applyRP sould have 1 arguments: <dir_RP>");

            //built a path from arg 1
            string dir_RP(argv[2]);

            //make a rutinary check
            if(dir_RP.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(dir_RP[0] != '/')
                dir_RP.insert(0, getCurrentDir()+"/");

            //apply the RP model for all RPs of the Fiber MOS Model Instance
            applyRP(FMM, dir_RP, log_filename);

            //save the instance of the Fiber MOS Model from where was loaded
            writeInstanceToDir(dir_FMM, FMM);
            append("Fiber MOS Model instance saved in '"+dir_FMM+"'.", log_filename.c_str());
        }
        //-------------------------------------------------------------------
        else if(command == "generatePairPPDP_offline") {
            //check the precondition
            if(argc < 3)
                throw EImproperArgument("command generatePairPPDP_offline sould have almost 1 argument: <path_FMOSA>");

            //build a string for the argument 1
            string arg1(argv[2]);

            //argument 1 can be:
            //  option help ("help")
            //  <path_FMOSA>

            //make a rutinary check
            if(arg1.length() <= 0)
                throw EImpossibleError("lateral effect");

            //reacts according the comands without log file
            if(arg1 == "help") {
                //print the help
                cout << help_generatePairPPDP_offline()+"\r\n" << endl;
                //indicates that the program has been executed without error
                return 0;
            }

            //complete the relative path, if any
            if(arg1[0] != '/')
                arg1.insert(0, getCurrentDir()+"/");

            bool saveOPL=false, saveOPL_S0=false, saveOPL_S1=false, savePP_FMPT=false, saveDP_FMPT=false;
            bool saveIPL=false, saveDisp_Corners1=false, saveDisp_Corners2=false, savePP=false, saveDP=false;
            bool savePP_Dmin=false, savePP_Dend=false, saveDP_Dmin=false, saveDP_Dend=false, saveFiberMOS=false, r2_negative=false;
            for(int i=3; i<argc; i++) {
                string argument(argv[i]);
                //------------------------------------
                if(argument == "OPL")
                    saveOPL = true;
                else if(argument == "OPL_S0")
                    saveOPL_S0 = true;
                else if(argument == "OPL_S1")
                    saveOPL_S1 = true;
                else if(argument == "PP-FMPT")
                    savePP_FMPT = true;
                else if(argument == "DP-FMPT")
                    saveDP_FMPT = true;
                //------------------------------------
                else if(argument == "IPL")
                    saveIPL = true;
                else if(argument == "Disp_Corners1")
                    saveDisp_Corners1 = true;
                else if(argument == "Disp_Corners2")
                    saveDisp_Corners2 = true;
                else if(argument == "PP")
                    savePP = true;
                else if(argument == "DP")
                    saveDP = true;
                //------------------------------------
                else if(argument == "PP_Dmin")
                    savePP_Dmin = true;
                else if(argument == "PP_Dend")
                    savePP_Dend = true;
                else if(argument == "DP_Dmin")
                    saveDP_Dmin = true;
                else if(argument == "DP_Dend")
                    saveDP_Dend = true;
                else if(argument == "fiberMOS")
                    saveFiberMOS = true;
                else if(argument == "r2_negative")
                    r2_negative = true;
                //------------------------------------
                else
                    throw EImproperArgument("unknoledge option: "+argument);
            }

            //generate a pair (PP, DP) from a path and write the events in the log file
            bool PPvalid;
            bool DPvalid;
            TRoboticPositionerList Collided;
            TRoboticPositionerList Obstructed;
            TMotionProgram PP;
            TMotionProgram DP;
            generatePairPPDP_offline(PPvalid, DPvalid, Collided, Obstructed, PP, DP,
                                     //------------------------------------------------------------
                                     FMM, arg1, ".", log_filename,
                                     //------------------------------------------------------------
                                     saveOPL, saveOPL_S0, saveOPL_S1, savePP_FMPT, saveDP_FMPT,
                                     saveIPL, saveDisp_Corners1, saveDisp_Corners2, savePP, saveDP,
                                     savePP_Dmin, savePP_Dend, saveDP_Dmin, saveDP_Dend, saveFiberMOS, r2_negative);
        }
        else if(command == "test_generatePairPPDP_offline") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command test_generatePairPPDP_offline should have 0 arguments");

            //execute the test
            test_generatePairPPDP_offline(FMM, log_filename);
        }
        else if(command == "test_generatePairPPDP_online") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command test_generatePairPPDP_online should have 0 arguments");

            //execute the test
            test_generatePairPPDP_online(FMM, log_filename);
        }
        //-------------------------------------------------------------------
        else if(command == "checkPairPPDP") {
            //check the precondition
            if(argc!=4 && argc!=5)
                throw EImproperArgument("command checkPairPPDP sould have 2 or 3 arguments: <path_PP> <path_DP> [Pid list]");

            //built a path from arg 1
            string path_PP(argv[2]);
            //built a path from arg 2
            string path_DP(argv[3]);

            //make a rutinary check
            if(path_PP.length()<=0 || path_DP.length()<=0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(path_PP[0] != '/')
                path_PP.insert(0, getCurrentDir()+"/");
            if(path_DP[0] != '/')
                path_DP.insert(0, getCurrentDir()+"/");

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
                append("Identifiers of the disabled RPs: "+Ids.getText().str, log_filename.c_str());
            }

            //check a pair (PP, DP) from a path and write the events in the log file
            checkPairPPDP(FMM, path_PP, path_DP, log_filename);
        }
        else if(command == "validatePairPPDP") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command validatePairPPDP sould have 1 arguments: <path_outputs>");

            //built a path from arg 1
            string path_outputs(argv[2]);

            //make a rutinary check
            if(path_outputs.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(path_outputs[0] != '/')
                path_outputs.insert(0, getCurrentDir()+"/");

            //validates a pair (PP, DP) from a path and write the events in the log file
            validatePairPPDP(FMM, path_outputs, ".", log_filename);
        }
        else if(command == "regeneratePairPPDP") {
            //check the precondition
            if(argc!=5 && argc!=6)
                throw EImproperArgument("command regeneratePairPPDP sould have 3 or 4 arguments: <path_PP> <path_DP> <path_FMOSA> [Pid list]");

            //built a path from arg 1
            string path_PP(argv[2]);
            //built a path from arg 2
            string path_DP(argv[3]);
            //built a path from arg 3
            string path_FMOSA(argv[4]);

            //make a rutinary check
            if(path_PP.length()<=0 || path_DP.length()<=0 || path_FMOSA.length()<=0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(path_PP[0] != '/')
                path_PP.insert(0, getCurrentDir()+"/");
            if(path_DP[0] != '/')
                path_DP.insert(0, getCurrentDir()+"/");
            if(path_FMOSA[0] != '/')
                path_FMOSA.insert(0, getCurrentDir()+"/");

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
                append("Identifiers of the disabled RPs: "+Ids.getText().str, log_filename.c_str());
            }

            //validates a pair (PP, DP) from a path and write the events in the log file
            regeneratePairPPDP_offline(FMM, path_PP, path_DP, path_FMOSA, log_filename);
        }
        //-------------------------------------------------------------------
        else if(command == "generateParkingProgram_offline") {
            //check the precondition
            if(argc < 3)
                throw EImproperArgument("command generateParkingProgram_offline sould have almost 1 argument: <path_FMOSA>");

            //build a string for the argument 1
            string arg1(argv[2]);

            //argument 1 can be:
            //  option help ("help")
            //  <path_FMOSA>

            //make a rutinary check
            if(arg1.length() <= 0)
                throw EImpossibleError("lateral effect");

            //reacts according the comands without log file
            if(arg1 == "help") {
                //print the help
                cout << help_generateParkingProgram_offline()+"\r\n" << endl;
                //indicates that the program has been executed without error
                return 0;
            }

            //complete the relative path, if any
            if(arg1[0] != '/')
                arg1.insert(0, getCurrentDir()+"/");

            bool saveSPL=false, saveParkingProgram_FMPT=false;
            bool saveParkingProgram_Dmin=false, saveParkingProgram_Dend=false;
            bool saveFPL=false, r2_negative=false;
            for(int i=3; i<argc; i++) {
                string argument(argv[i]);
                //------------------------------------
                if(argument == "SPL")
                    saveSPL = true;
                else if(argument == "ParkingProgram_FMPT")
                    saveParkingProgram_FMPT = true;
                //------------------------------------
                else if(argument == "ParkingProgram_Dmin")
                    saveParkingProgram_Dmin = true;
                else if(argument == "ParkingProgram_Dend")
                    saveParkingProgram_Dend = true;
                //------------------------------------
                else if(argument == "FPL")
                    saveFPL = true;
                else if(argument == "r2_negative")
                    r2_negative = true;
                //------------------------------------
                else
                    throw EImproperArgument("unknoledge option: "+argument);
            }

            //generate a DP from a path and write the events in the log file
            bool ParkingProgramValid;
            TRoboticPositionerList Collided;
            TRoboticPositionerList Obstructed;
            TMotionProgram ParkingProgram;
            generateParkingProgram_offline(ParkingProgramValid, Collided, Obstructed, ParkingProgram,
                                           //--------------------------------------------------------
                                           FMM, arg1, ".", log_filename,
                                           //--------------------------------------------------------
                                           saveSPL, saveParkingProgram_FMPT,
                                           saveParkingProgram_Dmin, saveParkingProgram_Dend,
                                           saveFPL, r2_negative);
        }
        else if(command == "test_generateParkingProgram_offline") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command test_generateParkingProgram_offline should have 0 arguments");

            //execute the test
            test_generateParkingProgram_offline(FMM, log_filename);
        }
        else if(command == "test_generateParkingProgram_online") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command test_generateParkingProgram_online should have 0 arguments");

            //execute the test
            test_generateParkingProgram_online(FMM, log_filename);
        }
/*        //-------------------------------------------------------------------
        else if(command == "visualizePPDP") {
            //check the precondition
            if(argc != 4)
                throw EImproperArgument("command visualizePPDP should have 2 arguments: <path_PP> <path_DP>");

            //built a path from arg 1
            string path_PP(argv[2]);
            //built a path from arg 2
            string path_DP(argv[3]);

            //make a rutinary check
            if(path_PP.length()<=0 || path_DP.length()<=0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(path_PP[0] != '/')
                path_PP.insert(0, getCurrentDir()+"/");
            if(path_DP[0] != '/')
                path_DP.insert(0, getCurrentDir()+"/");

            visualizePPDP(argc, argv, FMM, path_PP, path_DP, log_filename);
        }*/
        //-------------------------------------------------------------------
        else {
            throw EImpossibleError("lateral effect");
        }
    }
    catch(Exception &E) {
        //indicates that has happened an exception
        //and show the message of the exception
        append("ERROR: "+E.Message.str, log_filename.c_str());
        return 1;
    }
    catch(...) {
        //indicates that has happened an unknoledge exception
        append("ERROR: unknowledge exception", log_filename.c_str());
        return 2;
    }

    //-----------------------------------------------------------------------

    //indicates that the program has been executed without error
    //    return a.exec(); //Qt only
    return 0;
}

