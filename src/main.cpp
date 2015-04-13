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
//File: main.cpp
//Content: principal program fmpt_saa
//Last update: 11/02/2015
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "FileMethods.h"
#include "MotionProgramGenerator.h"
#include "FMOSATable.h"
#include "PositionerCenter.h"
#include "Strings.h"
#include "TextFile.h"

#include <locale.h> //setlocale, LC_NUMERIC
#include <iostream> //std::cout

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

    parent_path.clear();
    if(i >= 0)
        parent_path = path.substr(0, i);

    filename.clear();
    int aux = path.length() - 1;
    if(i < aux) {
        int count = aux - i;
        filename = path.substr(i+1, count);
    }
}

//gets a string containing the help
string help(void)
{
    string str;

    str = "$ fmpt_saa help";
    str += "\r\n\tPrint this help.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa applyPC <path_PC>";
    str += "\r\n\t<path_PC>: absolute or relative path to file containing a positioner center table.";
    str += "\r\n\tApply a positioner center table to the Fiber MOS Model instance.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa applyRP <dir_RP>";
    str += "\r\n\t<dir_RP>: absolute or relative path to dir containing a RP instance.";
    str += "\r\n\tApply a RP instance to the Fiber MOS Model instance.";
    str += "\r\n";

    str += "\r\n$ fmpt_saa generateDP <path_FMOSA>";
    str += "\r\n\t<path_FMOSA>: absolute or relative path to file containing a FMOSA table.";
    str += "\r\n\tGenerate a depositioning program.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa generatePPDP <path_FMOSA>";
    str += "\r\n\t<path_FMOSA>: absolute or relative path to file containing a FMOSA table.";
    str += "\r\n\tGenerate a pair (PP, DP).";
    str += "\r\n";
    str += "\r\n$ fmpt_saa checkPPDP <path_PP> <path_DP> [Pid list]";
    str += "\r\n\t<path_PP>: absolute or relative path to file containing the PP.";
    str += "\r\n\t<path_DP>: absolute or relative path to file containing the DP.";
    str += "\r\n\t[Pid list]: optional identifier list of RPs to be disabled.";
    str += "\r\n\tCheck the enabling status of the RPs included in a pair (PP, DP).";
    str += "\r\n\tThe instance of the Fiber MOS Model will not be written.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa regeneratePPDP <path_PP> <path_DP> <path_FMOSA> [Pid list]";
    str += "\r\n\t<path_PP>: absolute or relative path to file containing the PP.";
    str += "\r\n\t<path_DP>: absolute or relative path to file containing the DP.";
    str += "\r\n\t<path_FMOSA>: absolute or relative path to file containing a FMOSA table.";
    str += "\r\n\t[Pid list]: optional identifier list of RPs to be disabled.";
    str += "\r\n\tAttempt regenerate a pair (PP, DP).";
    str += "\r\n\tThe instance of the Fiber MOS Model will not be written.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa aboutof";
    str += "\r\n\tPrint the about of legend.";
    str += "\r\n";

    //Others diserable functionalities:
    //  - Determine the RPs included in a MP.
    //  - Determine the RPs included in a pair of MPs.
    //  - Enable all not faulty RPs of the FMM instance,
    //  - Disable the indicated PRs of the FMM instance,
    //  - Print the samples includes in the directory data/Samples

    return str;
}

//get a stringcontaining the about of legend
string about_of(void)
{
    string str;

    str += "Copyright (c) 2014-2015 Isaac Morales Durán. All rights reserved.\r\n";
    str += "Institute of Astrophysics of Andalusia, IAA-CSIC\r\n";
    str += "\r\n";
    str += "This file is part of FMPT (Fiber MOS Positioning Tools)\r\n";
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

//apply a positioner center table to the Fiber MOS Model Instance
void applyPC(TFiberMOSModel& FMM, string& path, string& log_filename)
{
    try {

        //LOAD SETTINGS FROM FILES:

        //load the positioner center table from a file
        TPositionerCenterList PCL;
        string str;
        strReadFromFile(str, path);
        PCL.setTableText(str);
        append("Positioner center table loaded from '"+path+"'.", log_filename.c_str());

        //MAKE THE OPERATIONS:

        //assimilates the positioner center table in the Fiber MOS Model
        for(int i=0; i<PCL.getCount(); i++) {
            TPositionerCenter PC = PCL[i];
            int j = FMM.RPL.SearchId(PC.Id);
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
        FMM.Assimilate();
        append("Positioner center table applied to the Fiber MOS Model.", log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "aplying positioner center table: ");
        throw E;
    }
}

//apply a RP instance for all RPs of the Fiber MOS Model Instance
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
        FMM.RPL.Apply(RP);
        append("RP instance applied to the Fiber MOS Model.", log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "aplying RP instance: ");
        throw E;
    }
}

//attempt generate a DP from a path file
void generateDP(TFiberMOSModel& FMM, string& path, string& log_filename)
{
    try {

        //LOAD SETTINGS FROM FILES:

        //load the FMOSA table from a file
        TFMOSATable FMOSA;
        unsigned int Bid;
        string str;
        strReadFromFile(str, path);
        FMOSA.setTableText(Bid, str);
        append("FMOSA table loaded from '"+path+"'.", log_filename.c_str());

        //get the allocation from the FMOSA table
        TMotionProgramGenerator MPG(&FMM);
        FMOSA.getAllocations(MPG);
        append("Allocations got from the FMOSA table.", log_filename.c_str());

        //MAKE THE OPERATIONS:

        //split the path of the file containing the FMOSA table
        string parent_path, filename;
        splitpath(parent_path, filename, path);

        //The filename will be used to attach the outputs filenames witht the input filename.

        //move the RPs to the more closer stable point to the TPs
        MPG.MoveToTargetP3();
        append("RPs moved to initial positions.", log_filename.c_str());

        //Other way to obtain the more closer stable points to the projection points,
        //consist in get from the FMOSA table the following lists:
        //  the allocation list;
        //  the projection point list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

        //A PPA table shall be stored how a table (Id, p_1, p___3).

        //captures the initial positions of the RPs in a PPA list
        TPairPositionAnglesList IPL;
        FMM.RPL.GetPositions(IPL);
        str = TActuator::GetPositionPPALabelsRow().str;
        str += "\r\n"+IPL.getColumnText().str;
        string output_filename = "IPL-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Initial position list saved in '"+output_filename+"'.", log_filename.c_str());

        //Other whay to obtain the initial position table directly in text format:
        //  FMM.RPL.getPositionsPAPTableText()

        //segregates the operative outsiders RPs
        TRoboticPositionerList Outsiders;
        FMM.RPL.segregateOperativeOutsiders(Outsiders);

        //check the limiting case when all operative RPs are in the origin
        if(FMM.RPL.allOperativeRPsAreInTheOrigin())
            append("WARNING: all operative RPs are in the origin. The generated DP will be empty.", log_filename.c_str());
        //else, check the limiting case when all operative RPs are in security positions
        else if(Outsiders.getCount() <= 0)
            append("WARNING: all operative RPs are in security positions. The generated DP will contains only a message-instruction list to go to the origin.", log_filename.c_str());

        //Now are fulfilled the preconditions:
        //  All RPs of the Fiber MOS Model, shall be in their initial positions.
        //  All RPs of the list Outsiders, shall be in the Fiber MOS Model.
        //  All RPs of the list Outsiders, shall be operatives.
        //  All RPs of the list Outsiders, shall be in unsecure positions.
        //  All RPs of the list Outsiders, shall have enabled the quantifiers.

        //generates a de positioning program for the operative RPs in insecure positions
        //and determines the RPs in collision status or obstructed in insecure positions
        append("Generating depositioning program...", log_filename.c_str());
        TRoboticPositionerList Collided;
        TRoboticPositionerList Obstructed;
        TMotionProgram DP;
        bool success = MPG.generateDepositioningProgram(Collided, Obstructed, DP, Outsiders);

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
        if(success) {
            //indicates that the depositioning program has been generated successfully
            append("Depositioning program generated successfully.", log_filename.c_str());

            //save the DP in the format of the FMPT
            str = DP.getText().str;
            output_filename = "DP-FMPT-from-"+filename;
            strWriteToFile(output_filename, str);

            //Here all operative outsiders RPs which aren't obstructed are in the origin positions,
            //in their final position after execute the DP.

            //translates the depositioning program to the format of the interface MCS-FMPT
            //and save it in a file
            DP.getInterfaceText(str, "obs depos", Bid, IPL);
            append("Depositiong program translated to the format of MCS interface.", log_filename.c_str());
            output_filename = "DP-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Depositioning program saved in '"+output_filename+"'.", log_filename.c_str());
        }

        //Here all operative outsiders RPs which aren't obstructed,can be:
        //- in the origin positions, in their final position after execute the DP.
        //  if success == true.
        //- in the first position where the collision was detected.
        //  if success == false.

        //get and save the positions where the collision was detected
        TPairPositionAnglesList FPL;
        FMM.RPL.GetPositions(FPL);
        str = TActuator::GetPositionPPALabelsRow().str;
        str += "\r\n"+FPL.getColumnText().str;
        output_filename = "FPL-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Final position list saved in '"+output_filename+"'.", log_filename.c_str());

        //print the other outputs in the corresponding file
        str = "\r\ngenerateDepositioningProgram: "+BoolToStr(success).str;
        str += "\r\nCollided: "+Collided.getText().str;
        str += "\r\nObstructed: "+Obstructed.getText().str;
        str += "\r\nDP comments:\r\n"+DP.getCommentsColumnText();
        output_filename = "outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "generating depositioning program: ");
        throw E;
    }
}

//attempt generate a pair (PP, DP) from a path file
void generatePPDP(TFiberMOSModel& FMM, string& path, string& log_filename)
{
    try {

        //LOAD SETTINGS FROM FILES:

        //load the FMOSA table from a file
        TFMOSATable FMOSA;
        unsigned int Bid;
        string str;
        strReadFromFile(str, path);
        FMOSA.setTableText(Bid, str);
        append("FMOSA table loaded from '"+path+"'.", log_filename.c_str());

        //get the allocation from the FMOSA table
        TMotionProgramGenerator MPG(&FMM);
        FMOSA.getAllocations(MPG);
        append("Allocations got from the FMOSA table.", log_filename.c_str());

        //MAKE THE OPERATIONS:

        //split the path of the file containing the FMOSA table
        string parent_path, filename;
        splitpath(parent_path, filename, path);

        //The filename will be used to attach the outputs filenames witht the input filename.

        //move the RPs to the more closer stable point to the TPs
        MPG.MoveToTargetP3();
        append("RPs moved to observing positions.", log_filename.c_str());

        //Other way to obtain the more closer stablepoints to the projection points,
        //consist in get from the FMOSA table the following lists:
        //  the allocation list;
        //  the projection point list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

        //A PPA table shall be stored how a table (Id, p_1, p___3).

        //captures the observing positions of the RPs in a PPA list
        TPairPositionAnglesList OPL;
        FMM.RPL.GetPositions(OPL);
        str = TActuator::GetPositionPPALabelsRow().str;
        str += "\r\n"+OPL.getColumnText().str;
        string output_filename = "OPL-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Observing position list saved in '"+output_filename+"'.", log_filename.c_str());

        //Other whay to obtain the observing position table directly in text format:
        //  FMM.RPL.getPositionsPAPTableText()

        //segregates the operative outsiders RPs
        TRoboticPositionerList Outsiders;
        FMM.RPL.segregateOperativeOutsiders(Outsiders);

        //check the limiting case when all operative RPs are in the origin
        if(FMM.RPL.allOperativeRPsAreInTheOrigin())
            append("WARNING: all operative RPs are in the origin. The generated pair (PP, DP) will be empty.", log_filename.c_str());
        //else, check the limiting case when all operative RPs are in security positions
        else if(Outsiders.getCount() <= 0)
            append("WARNING: all operative RPs are in security positions. The generated (PP, DP) will contains only a message-instruction list to go to the observing positions and back to the origin.", log_filename.c_str());

        //Now are fulfilled the preconditions:
        //  All RPs of the Fiber MOS Model, shall be in their initial positions.
        //  All RPs of the list Outsiders, shall be in the Fiber MOS Model.
        //  All RPs of the list Outsiders, shall be operatives.
        //  All RPs of the list Outsiders, shall be in unsecure positions.
        //  All RPs of the list Outsiders, shall have enabled the quantifiers.

        //generates a de positioning program for the operative RPs in insecure positions
        //and determines the RPs in collision status or obstructed in insecure positions
        append("Generating pair (PP, DP)...", log_filename.c_str());
        TRoboticPositionerList Collided, Obstructed;
        TMotionProgram PP, DP;
        bool success = MPG.generatePairPPDP(Collided, Obstructed, PP, DP, Outsiders);

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
        if(success) {
            //indicates that the depositioning program has been generated successfully
            append("Pair (PP, DP) generated successfully.", log_filename.c_str());

            //save the PP in the format of the FMPT
            str = PP.getText().str;
            output_filename = "PP-FMPT-from-"+filename;
            strWriteToFile(output_filename, str);

            //save the DP in the format of the FMPT
            str = DP.getText().str;
            output_filename = "DP-FMPT-from-"+filename;
            strWriteToFile(output_filename, str);

            //Here all operative outsiders RPs which aren't obstructed,can be:
            //- in the origin positions, in their final position after execute the DP.
            //  if success == true.
            //- in the first position where the collision was detected.
            //  if success == false.

            //captures the initial positions of the RPs in a PPA list
            TPairPositionAnglesList IPL;
            FMM.RPL.GetPositions(IPL);
            string str = TActuator::GetPositionPPALabelsRow().str;
            str += "\r\n"+IPL.getColumnText().str;
            string output_filename = "IPL-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Initial position list saved in '"+output_filename+"'.", log_filename.c_str());

            //Other whay to obtain the observing position table directly in text format:
            //  FMM.RPL.getPositionsPAPTableText()

            //translates the positioning program to the format of the interface MCS-FMPT
            //and save it in a file
            PP.getInterfaceText(str, "obs pos", Bid, IPL);
            append("Positiong program translated to the format of MCS interface.", log_filename.c_str());
            output_filename = "PP-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Positioning program saved in '"+output_filename+"'.", log_filename.c_str());

            //translates the depositioning program to the format of the interface MCS-FMPT
            //and save it in a file
            DP.getInterfaceText(str, "obs depos", Bid, OPL);
            append("Depositiong program translated to the format of MCS interface.", log_filename.c_str());
            output_filename = "DP-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Depositioning program saved in '"+output_filename+"'.", log_filename.c_str());
        }

        //print the other outputs in the corresponding file
        str = "generatePairPPDP: "+BoolToStr(success).str;
        str += "\r\nCollided: "+Collided.getText().str;
        str += "\r\nObstructed: "+Obstructed.getText().str;
        str += "\r\nPP comments:\r\n"+PP.getCommentsColumnText();
        str += "\r\nDP comments:\r\n"+DP.getCommentsColumnText();
        output_filename = "outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "generating pair (PP, DP): ");
        throw E;
    }
}

//check a pair (PP, DP) from a path file
void checkPPDP(TFiberMOSModel& FMM, string& path_PP, string& path_DP, string& log_filename)
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
        append("Positioning program loaded from '"+path_PP+"'.", log_filename.c_str());

        //check the precondition
        if(PP_label != "obs pos")
            throw EImproperArgument("PP label should be \"obs pos\"");

        //load the DP from a file
        TMotionProgram DP;
        string DP_label;
        //str.clear();
        strReadFromFile(str, path_DP);
        unsigned int DP_Bid;
        DP.setInterfaceText(DP_label, DP_Bid, str);
        append("Depositioning program loaded from '"+path_DP+"'.", log_filename.c_str());

        //check the preconditions
        if(DP_label != "obs depos")
            throw EImproperArgument("DP label should be \"obs depos\"");
        if(DP_Bid != PP_Bid)
            throw EImproperArgument("PP Bid should be equal to DP Bid");

        //build the MPG attached to the FMM
        TMotionProgramGenerator MPG(&FMM);

        //-------------------------------------------------------------------
        //MAKE THE OPERATIONS:

        //check the pair (PP, DP)
        bool valid = MPG.checkPairPPDP(PP, DP);

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

        //PROBLEMA: elnombre del archivo 'outputs-from...' se contruye a partir
        //del filename de path_FMOSA, pero en esta función, solo se pasan path_PP y path_DP.

        //print the other outputs in the corresponding file
        str = "checkPairPPDP: "+BoolToStr(valid).str;
        string output_filename = "outputs-from-"+filename_PP+"-"+filename_DP;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "checking pair (PP, DP): ");
        throw E;
    }
}

//attempt regenerate a pair (PP, DP) from a path file
void regeneratePPDP(TFiberMOSModel& FMM, string& path_PP, string& path_DP, string& path_FMOSA, string& log_filename)
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
        append("Positioning program loaded from '"+path_PP+"'.", log_filename.c_str());

        //check the precondition
        if(PP_label != "obs pos")
            throw EImproperArgument("PP label should be \"obs pos\"");

        //load the DP from a file
        TMotionProgram DP;
        string DP_label;
        unsigned int DP_Bid;
        strReadFromFile(str, path_DP);
        DP.setInterfaceText(DP_label, DP_Bid, str);
        append("Depositioning program loaded from '"+path_DP+"'.", log_filename.c_str());

        //check the precondition
        if(DP_label != "obs depos")
            throw EImproperArgument("DP label should be \"obs depos\"");
        if(DP_Bid != PP_Bid)
            throw EImproperArgument("DP Bid should be equal to PP Bid");

        //load the FMOSA table from a file
        TFMOSATable FMOSA;
        unsigned int FMOSA_Bid;
        strReadFromFile(str, path_FMOSA);
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
        FMM.RPL.GetPositions(IPL);
        append("Initial position list got from the Fiber MOS Model.", log_filename.c_str());

        //The IPL is useful to get the PP in the interface format.

        //move the RPs to the more closer stable point to the TPs
        MPG.MoveToTargetP3();
        append("RPs moved to observing positions.", log_filename.c_str());

        //Other way to obtain the more closer stablepoints to the projection points,
        //consist in get from the FMOSA table the following lists:
        //  the allocation list;
        //  the projection point list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

        //A PPA table shall be stored how a table (Id, p_1, p___3).

        //get the observing position list
        TPairPositionAnglesList OPL;
        FMM.RPL.GetPositions(OPL);
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
            PP.getInterfaceText(str, "obs pos", PP_Bid, IPL);
            append("Positiong program translated to the format of MCS interface.", log_filename.c_str());
            output_filename = "regeneratedPP-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Regenerated PP saved in: "+output_filename, log_filename.c_str());

            //save the regenerated DP
            DP.getInterfaceText(str, "obs depos", DP_Bid, OPL);
            append("Depositiong program translated to the format of MCS interface.", log_filename.c_str());
            output_filename = "regeneratedDP-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Regenerated DP saved in: "+output_filename, log_filename.c_str());

        } else
            append("The pair (PP, DP) can not be regenerated.", log_filename.c_str());

        //print the other outputs in the corresponding file
        str = "regeneratePairPPDP: "+BoolToStr(regenerated).str;
        str += "\r\nExcluded: "+Excluded.getText().str;
        output_filename = "outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "regenerating pair (PP, DP): ");
        throw E;
    }
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
    //###################################################################

    //initalize the log file
    string log_filename = "fmpt_saa.log";
    char mode[] = "w";
    TTextFile TF(log_filename.c_str(), mode);
    TF.Close();

    try {

        //-------------------------------------------------------------------
        //CONFIGURATES THE SYSTEM:

        //REMEMBER: exceptions in runtime can be due to that
        //the system is not configurated.

        //configurates the decimal separator
        setlocale(LC_NUMERIC, "C");

        //initializes the global variables
        if(TrueBoolStrs.getCount() < 1) {
            TrueBoolStrs.setCount(1);
            TrueBoolStrs[0] = "True";
        }
        if(FalseBoolStrs.getCount() < 1) {
            FalseBoolStrs.setCount(1);
            FalseBoolStrs[0] = "False";
        }

        //-----------------------------------------------------------------------
        //MAKE ACTIONS:

        //if the program is run without the necessary comand
        //warn that happened, print the help and finish the program
        if(argc <= 1) {
            //indicates that happened
            append("Missing command.", log_filename.c_str());

            //print the help
            append("\r\n"+help(), log_filename.c_str());

            //indicates that the program has been executed without error
            return 0;
        }

        //indicates that the program is running
        append("FMPT SAA 1.3.1 is running...", log_filename.c_str());

        //print  the arguments
        string str;
        str += "\tExecutable path: "+string(argv[0])+"\r\n";
        if(argc > 1) {
            str += "\tCommand: "+string(argv[1])+"\r\n";
            for(int i=2; i<argc; i++)
                str += "\tArg[" + inttostr(i - 1) + "]: " + string(argv[i]) + "\r\n";
        }
        append(str, log_filename.c_str());

        //built the paths where search the Fiber MOS Model
        string dir_FMM1 = DATADIR;
        dir_FMM1 += "/Models/MEGARA_FiberMOSModel_Instance";
        string dir_FMM2 = getCurrentDir()+"/../data/Models/MEGARA_FiberMOSModel_Instance";

        //load the instance of the Fiber MOS Model from a dir
        TFiberMOSModel FMM;
        string dir_FMM;
        try {
            readInstanceFromDir(FMM, dir_FMM1);
            dir_FMM = dir_FMM1;
        } catch(...) {
            try {
                readInstanceFromDir(FMM, dir_FMM2);
                dir_FMM = dir_FMM2;
            } catch(...) {
                throw;
            }
        }
        append("Fiber MOS Model instance loaded from '"+dir_FMM+"'.", log_filename.c_str());

        //-------------------------------------------------------------------
        //REACTS ACCORDING THE COMMAND AND THE ARGUMENTS:

        //build a command string
        string command(argv[1]);

        if(command == "help") {
            //print the help
            append("\r\n"+help(), log_filename.c_str());
        }
        else if(command == "applyPC") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command applyCP should has 1 arguments");

            //built a path from arg 1
            string path_PC(argv[2]);

            //make a rutinary check
            if(path_PC.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(path_PC[0] != '/')
                path_PC.insert(0, getCurrentDir()+"/");

            //apply a positioner center table to the Fiber MOS Model Instance
            applyPC(FMM, path_PC, log_filename);

            //save the instance of the Fiber MOS Model from where was loaded
            writeInstanceToDir(dir_FMM, FMM);
            append("Fiber MOS Model instance saved in '"+dir_FMM+"'.", log_filename.c_str());
        }
        else if(command == "applyRP") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command applyRP should has 1 arguments");

            //built a path from arg 1
            string dir_RP(argv[2]);

            //make a rutinary check
            if(dir_RP.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(dir_RP[0] != '/')
                dir_RP.insert(0, getCurrentDir()+"/");

            //apply a RP model for all RPs of the Fiber MOS Model Instance
            applyRP(FMM, dir_RP, log_filename);

            //save the instance of the Fiber MOS Model from where was loaded
            writeInstanceToDir(dir_FMM, FMM);
            append("Fiber MOS Model instance saved in '"+dir_FMM+"'.", log_filename.c_str());
        }
        else if(command == "generateDP") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command generateDP should has 1 arguments");

            //built a path from arg 1
            string path_FMOSA(argv[2]);

            //make a rutinary check
            if(path_FMOSA.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(path_FMOSA[0] != '/')
                path_FMOSA.insert(0, getCurrentDir()+"/");

            //generate a DP from a path and write the events in the log file
            generateDP(FMM, path_FMOSA, log_filename);
        }
        else if(command == "generatePPDP") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command generatePPDP should has 1 arguments");

            //built a path from arg 1
            string path_FMOSA(argv[2]);

            //make a rutinary check
            if(path_FMOSA.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(path_FMOSA[0] != '/')
                path_FMOSA.insert(0, getCurrentDir()+"/");

            //generate a pair (PP, DP) from a path and write the events in the log file
            generatePPDP(FMM, path_FMOSA, log_filename);
        }
        else if(command == "checkPPDP") {
            //check the precondition
            if(argc!=4 && argc!=5)
                throw EImproperArgument("command checkPPDP should has 2 or 3 arguments");

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
                    int j = FMM.RPL.SearchId(Id);
                    if(j >= FMM.RPL.getCount())
                        throw EImproperArgument("RP not fount in the Fiber MOS Model: "+inttostr(Id));
                }

                //disble the RPs
                for(int i=0; i<Ids.getCount(); i++) {
                    int Id = Ids[i];
                    int j = FMM.RPL.SearchId(Id);
                    if(j >= FMM.RPL.getCount())
                        throw EImpossibleError("lateral effect");
                    TRoboticPositioner *RP = FMM.RPL[j];
                    RP->Disabled = true;
                }
                append("Identifiers of the disabled RPs: "+Ids.getText().str, log_filename.c_str());
            }

            //validates a pair (PP, DP) from a path and write the events in the log file
            checkPPDP(FMM, path_PP, path_DP, log_filename);
        }
        else if(command == "regeneratePPDP") {
            //check the precondition
            if(argc!=5 && argc!=6)
                throw EImproperArgument("command regeneratePPDP should has 3 or 4 arguments");

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
                    int j = FMM.RPL.SearchId(Id);
                    if(j >= FMM.RPL.getCount())
                        throw EImproperArgument("RP not fount in the Fiber MOS Model: "+inttostr(Id));
                }

                //disble the RPs
                for(int i=0; i<Ids.getCount(); i++) {
                    int Id = Ids[i];
                    int j = FMM.RPL.SearchId(Id);
                    if(j >= FMM.RPL.getCount())
                        throw EImpossibleError("lateral effect");
                    TRoboticPositioner *RP = FMM.RPL[j];
                    RP->Disabled = true;
                }
                append("Identifiers of the disabled RPs: "+Ids.getText().str, log_filename.c_str());
            }

            //validates a pair (PP, DP) from a path and write the events in the log file
            regeneratePPDP(FMM, path_PP, path_DP, path_FMOSA, log_filename);
        }
        else if(command == "aboutof") {
            //print the about of legend
            append("\r\n"+about_of(), log_filename.c_str());
        }
        else {
            //indicates that happened
            append("Unknoledge command.", log_filename.c_str());

            //print the help
            append("\r\n"+help(), log_filename.c_str());
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
    return 0;
}

