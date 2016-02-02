// Copyright (c) 2014-201 Isaac Morales Durán. All rights reserved.
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
#include "outputs.h" //Outputs

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

    str += "\r\n$ fmpt_saa valuesSPM";
    str += "\r\n\tView the SPM values pending varibales PAkd and Purpose.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa testRadialMotion";
    str += "\r\n\tTake the measure of maximun deviation around the radial trayectory of the fiber of each RP.";
    str += "\r\n";

    str += "\r\n$ fmpt_saa applyPC <path_PC>";
    str += "\r\n\t<path_PC>: absolute or relative path to file containing a positioner center table.";
    str += "\r\n\tApply a positioner center table to the Fiber MOS Model instance.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa applyRP <dir_RP>";
    str += "\r\n\t<dir_RP>: absolute or relative path to dir containing a RP instance.";
    str += "\r\n\tApply a RP instance to the Fiber MOS Model instance.";
    str += "\r\n";

    str += "\r\n$ fmpt_saa generatePairPPDP_offline <path_FMOSA>";
    str += "\r\n\t<path_FMOSA>: absolute or relative path to file containing a FMOSA table.";
    str += "\r\n\tGenerate a pair (PP, DP) offline. So the inputs and outputs are files.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa testGeneratePairPPDP_offline";
    str += "\r\n\tTest the function generatePairPPDP_offline.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa testGeneratePairPPDP_online";
    str += "\r\n\tTest the function generatePairPPDP_online.";
    str += "\r\n";

    str += "\r\n$ fmpt_saa checkPairPPDP <path_PP> <path_DP> [Pid list]";
    str += "\r\n\t<path_PP>: absolute or relative path to file containing the PP.";
    str += "\r\n\t<path_DP>: absolute or relative path to file containing the DP.";
    str += "\r\n\t[Pid list]: optional identifier list of RPs to be disabled.";
    str += "\r\n\tCheck the enabling status of the RPs included in a pair (PP, DP).";
    str += "\r\n\tThe instance of the Fiber MOS Model will not be written.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa regeneratePairPPDP <path_PP> <path_DP> <path_FMOSA> [Pid list]";
    str += "\r\n\t<path_PP>: absolute or relative path to file containing the PP.";
    str += "\r\n\t<path_DP>: absolute or relative path to file containing the DP.";
    str += "\r\n\t<path_FMOSA>: absolute or relative path to file containing a FMOSA table.";
    str += "\r\n\t[Pid list]: optional identifier list of RPs to be disabled.";
    str += "\r\n\tAttempt regenerate a pair (PP, DP).";
    str += "\r\n\tThe instance of the Fiber MOS Model will not be written.";
    str += "\r\n";

    str += "\r\n$ fmpt_saa generateParkingProgram_offline <path_FMOSA>";
    str += "\r\n\t<path_FMOSA>: absolute or relative path to file containing a FMOSA table.";
    str += "\r\n\tGenerate a parking program offline. So the inputs and outputs are files.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa testGenerateParkingProgram_offline";
    str += "\r\n\tTest the function generateParkingProgram_offline.";
    str += "\r\n";
    str += "\r\n$ fmpt_saa testGenerateParkingProgram_online";
    str += "\r\n\tTest the function generateParkingProgram_online.";
    str += "\r\n";

    str += "\r\n$ fmpt_saa aboutOf";
    str += "\r\n\tPrint the about of legend.";
    str += "\r\n";

    //Note that here not has sense define functions for generate MPs online.

    //Others diserable functionalities:
    //  - Determine the RPs included in a MP.
    //  - Determine the RPs included in a pair of MPs.
    //  - Enable all not faulty RPs of the FMM instance,
    //  - Disable the indicated PRs of the FMM instance,
    //  - Print the samples includes in the directory data/Samples

    return str;
}

//get a stringcontaining the about of legend
string aboutOf(void)
{
    string str;

    str += "Copyright (c) 2014-2016 Isaac Morales Durán. All rights reserved.\r\n";
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
        FMM.RPL.apply(&RP);
        append("RP instance applied to the Fiber MOS Model.", log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "aplying RP instance: ");
        throw E;
    }
}

//attempt generate a pair (PP, DP) from a path file
//Parameters for can use the function for test:
//  (PPvalid, DPvalid, Collided, Obstructed, PP, DP)
//Parameters for generate a pair PPDP:
//  (FMM, path, log_filename)
void generatePairPPDP_offline(bool& PPvalid, bool& DPvalid,
                      TRoboticPositionerList& Collided, TRoboticPositionerList& Obstructed,
                      TMotionProgram& PP, TMotionProgram& DP,
                      //------------------------------------------------------
                      TFiberMOSModel& FMM, const string& input_path, const string& output_dir, const string& log_filename)
{
    try {

        //LOAD SETTINGS FROM FILES:

        //load the Outputs structure from a file
        string str;
        strReadFromFile(str, input_path);
        Outputs outputs;
        unsigned int Bid;
        try {
            outputs.setText(Bid, str);
        } catch(EImproperArgument& E) {
            try {
                outputs.FMOSAT.setTableText(Bid, str);
            } catch(Exception& E) {
                E.Message.Insert(1, "reading FMOSA file: ");
                throw;
            } catch(...) {
                throw;
            }
        } catch(Exception& E) {
            E.Message.Insert(1, "reading PPDPandFMOSA file: ");
            throw;
        } catch(...) {
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
        append("RPs moved to observing positions.", log_filename.c_str());

        //Other way to obtain the more closer stable positions to the allocated projection points,
        //consist in get from the FMOSA table the following lists:
        //  the allocation list;
        //  the projection point list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

        //A PPA table shall be stored how a table (Id, p_1, p___3).

        //captures the observing positions of the RPs in a PPA list
        TPairPositionAnglesList OPL;
        FMM.RPL.getPositions(OPL);
        str = TActuator::getPositionPPALabelsRow().str;
        str += "\r\n"+OPL.getColumnText().str;
        ForceDirectories(AnsiString(output_dir));
        string output_filename = output_dir+"/OPL-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Observing position list saved in '"+output_filename+"'.", log_filename.c_str());

        //Other whay to obtain the observing position table directly in text format:
        //  FMM.RPL.getPositionsPPATableText()

        //segregates the operative outsider RPs
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
        PPvalid = false;
        DPvalid = false;
        Collided.Clear();
        Obstructed.Clear();
        outputs.PP.Clear();
        outputs.DP.Clear();
        MPG.generatePairPPDP(PPvalid, DPvalid, Collided, Obstructed, outputs.PP, outputs.DP, Outsiders);

        //Now are fulfilled the postconditions:
        //  All RPs of the Fiber MOS Model will be configured for MP validation
        //  All RPs of the fiber MOS Model will be in their final positions,
        //  or the first position where the collision was detected.
        //  All RPs of the Fiber MOS Model will have disabled the quantifiers.

        //########################################################################
        //#WARNING: before re-use a function to generation, shall be restablished#
        //#the preconditions.                                                    #
        //########################################################################

        //SAVE THE OUTPUTS AND PRINT THE CORRESPONDING MESSAGES:

        //if generation function was successfully generated
        if(PPvalid && DPvalid) {
            //indicates the result of the generation
            append("Generated pair (PP, DP) is valid.", log_filename.c_str());

            //save the PP in the format of the FMPT
            str = PP.getText().str;
            output_filename = output_dir+"/PP-FMPT-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Positioning program in propietary format saved in '"+output_filename+"'.", log_filename.c_str());

            //save the DP in the format of the FMPT
            str = DP.getText().str;
            output_filename = output_dir+"/DP-FMPT-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Depositioning program in propietary format saved in '"+output_filename+"'.", log_filename.c_str());

            //Given that here the generated pair (PP, DP) is valid,
            //all operative outsider RPs which aren't obstructed,can be:
            //- in the origin positions, in their final position after execute the DP.

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

            //translates the positioning program to the format of the interface MCS-FMPT
            //and save it in a file
//            PP.getInterfaceText(str, "pos", Bid, IPL);
//            append("Positiong program translated to the MCS format.", log_filename.c_str());
//            output_filename = output_dir+"/PP-from-"+filename;
//            strWriteToFile(output_filename, str);
//            append("Positioning program in MCS format saved in '"+output_filename+"'.", log_filename.c_str());

            //translates the depositioning program to the format of the interface MCS-FMPT
            //and save it in a file
//            DP.getInterfaceText(str, "depos", Bid, OPL);
//            append("Depositiong program translated to the MCS format.", log_filename.c_str());
//            output_filename = output_dir+"/DP-from-"+filename;
//            strWriteToFile(output_filename, str);
//            append("Depositioning program in MCS format saved in '"+output_filename+"'.", log_filename.c_str());

            outputs.getText(str, Bid, OPL, IPL);
            output_filename = output_dir+"/PPDPandFMOSA-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Pair (PP, DP) saved in '"+output_filename+"'.", log_filename.c_str());

            //save PP-Dfmin in a file
            PP.getDfminInterfaceText(str, "pos", Bid);
            output_filename = output_dir+"/PP-Dfmin-from-"+filename;
            strWriteToFile(output_filename, str);
            append("PP-Dfmin saved in '"+output_filename+"'.", log_filename.c_str());

            //save DP-Dfmin in a file
            DP.getDfminInterfaceText(str, "depos", Bid);
            output_filename = output_dir+"/DP-Dfmin-from-"+filename;
            strWriteToFile(output_filename, str);
            append("DP-Dfmin saved in '"+output_filename+"'.", log_filename.c_str());
        }
        else {
            //Given that here the generated pair (PP, DP) is invalid,
            //all operative outsider RPs which aren't obstructed,can be:
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

        //print the other outputs in the corresponding file
        str = "DPvalid: "+BoolToStr(DPvalid,true).str;
        str += "\r\nPPvalid: "+BoolToStr(PPvalid,true).str;
        str += "\r\nCollided: "+Collided.getText().str;
        str += "\r\nObstructed: "+Obstructed.getText().str;
        if(DP.thereIsSomeComment1())
            str += "\r\nDP comments:\r\n"+DP.getComment1sColumnText();
        if(PP.thereIsSomeComment1())
            str += "\r\nPP comments:\r\n"+PP.getComment1sColumnText();
        output_filename = output_dir+"/outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "generating pair (PP, DP) offline: ");
        throw E;
    } catch(...) {
        throw;
    }
}

/*//attempt generate a pair (PP, DP) from a path file
//Parameters for can use the function for test:
//  (PPvalid, DPvalid, Collided, Obstructed, PP, DP)
//Parameters for generate a pair PPDP:
//  (FMM, path, log_filename)
void generatePairPPDP_online(bool& PPvalid, bool& DPvalid,
                             TMotionProgram& PP, TMotionProgram& DP,
                             //------------------------------------------------------
                             TFiberMOSModel& FMM, const string& input_path, const string& output_dir, const string& log_filename,
                             vector<int>& Ids)
{
    try {

        //LOAD SETTINGS FROM FILES:

        //load the FMOSA table from a file
        TFMOSATable FMOSA;
        unsigned int Bid;
        string str;
        strReadFromFile(str, input_path);
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
        append("RPs moved to observing positions.", log_filename.c_str());

        //Other way to obtain the more closer stablepoints to the projection points,
        //consist in get from the FMOSA table the following lists:
        //  the allocation list;
        //  the projection point list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

        //A PPA table shall be stored how a table (Id, p_1, p___3).

        //captures the observing positions of the RPs in a PPA list
        TPairPositionAnglesList OPL;
        FMM.RPL.getPositions(OPL);
        str = TActuator::getPositionPPALabelsRow().str;
        str += "\r\n"+OPL.getColumnText().str;
        ForceDirectories(AnsiString(output_dir));
        string output_filename = output_dir+"/OPL-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Observing position list saved in '"+output_filename+"'.", log_filename.c_str());

        //Other whay to obtain the observing position table directly in text format:
        //  FMM.RPL.getPositionsPPATableText()

        //-------------------------------------------------------------------
        //sustituir lo siguiente:

        //segregates the operative outsider RPs
//        TRoboticPositionerList Outsiders;
  //      FMM.RPL.segregateOperativeOutsiders(Outsiders);

        //check the limiting case when all operative RPs are in the origin
//        if(FMM.RPL.allOperativeRPsAreInTheOrigin())
  //          append("WARNING: all operative RPs are in the origin. The generated pair (PP, DP) will be empty.", log_filename.c_str());
        //else, check the limiting case when all operative RPs are in security positions
//        else if(Outsiders.getCount() <= 0)
  //          append("WARNING: all operative RPs are in security positions. The generated (PP, DP) will contains only a message-instruction list to go to the observing positions and back to the origin.", log_filename.c_str());

        //Now are fulfilled the preconditions:
        //  All RPs of the Fiber MOS Model, shall be in their initial positions.
        //  All RPs of the list Outsiders, shall be in the Fiber MOS Model.
        //  All RPs of the list Outsiders, shall be operatives.
        //  All RPs of the list Outsiders, shall be in unsecure positions.
        //  All RPs of the list Outsiders, shall have enabled the quantifiers.

        //generates a de positioning program for the operative RPs in insecure positions
        //and determines the RPs in collision status or obstructed in insecure positions
//        append("Generating pair (PP, DP)...", log_filename.c_str());
  //      PPvalid = false;
    //    DPvalid = false;
      //  Collided.Clear();
//        Obstructed.Clear();
  //      PP.Clear();
    //    DP.Clear();
      //  MPG.generatePairPPDP(PPvalid, DPvalid, Collided, Obstructed, PP, DP, Outsiders);

        //Now are fulfilled the postconditions:
        //  All RPs of the Fiber MOS Model will be configured for MP validation
        //  All RPs of the fiber MOS Model will be in their final positions,
        //  or the first position where the collision was detected.
        //  All RPs of the Fiber MOS Model will have disabled the quantifiers.

        //########################################################################
        //#WARNING: before re-use a function to generation, shall be restablished#
        //#the preconditions.                                                    #
        //########################################################################

        //-------------------------------------------------------------------
        //SAVE THE OUTPUTS AND PRINT THE CORRESPONDING SMESSAGES:

        //if generation function was successfully generated
        if(PPvalid && DPvalid) {
            //indicates the result of the generation
            append("Generated pair (PP, DP) is valid.", log_filename.c_str());

            //save the DP in the format of the FMPT
            str = DP.getText().str;
            output_filename = output_dir+"/DP-FMPT-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Depositioning program in propietary format saved in '"+output_filename+"'.", log_filename.c_str());

            //save the PP in the format of the FMPT
            str = PP.getText().str;
            output_filename = output_dir+"/PP-FMPT-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Positioning program in propietary format saved in '"+output_filename+"'.", log_filename.c_str());

            //Given that here the generated pair ((PP, DP) is valid,
            //all operative outsider RPs which aren't obstructed,can be:
            //- in the origin positions, in their final position after execute the DP.
            //  if success == true.
            //- in the first position where the collision was detected.
            //  if success == false.

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

            //translates the depositioning program to the format of the interface MCS-FMPT
            //and save it in a file
            DP.getInterfaceText(str, "depos", Bid, OPL);
            append("Depositiong program translated to the MCS format.", log_filename.c_str());
            output_filename = output_dir+"/DP-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Depositioning program in MCS format saved in '"+output_filename+"'.", log_filename.c_str());

            //translates the positioning program to the format of the interface MCS-FMPT
            //and save it in a file
            PP.getInterfaceText(str, "pos", Bid, IPL);
            append("Positiong program translated to the MCS format.", log_filename.c_str());
            output_filename = output_dir+"/PP-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Positioning program in MCS format saved in '"+output_filename+"'.", log_filename.c_str());
        }
        else {
            //Given that here the generated pair (PP, DP) can be valid or invalid,
            //all operative outsider RPs which aren't obstructed,can be:
            //- in the starting positions, in their final position after execute the PP.
            //  if success == true.
            //- in the first position where the collision was detected.
            //  if success == false.

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

//        if(Obstructed.getCount() > 0)
  //          append("There are obstructed RPs: "+Obstructed.getText().str, log_filename.c_str());

//        if(Collided.getCount() > 0)
  //          append("There are collided RPs: "+Collided.getText().str, log_filename.c_str());

        //print the other outputs in the corresponding file
        str = "DPvalid: "+BoolToStr(DPvalid,true).str;
        str += "\r\nPPvalid: "+BoolToStr(PPvalid,true).str;
//        str += "\r\nCollided: "+Collided.getText().str;
 //       str += "\r\nObstructed: "+Obstructed.getText().str;
        if(DP.thereIsSomeComment())
            str += "\r\nDP comments:\r\n"+DP.getCommentsColumnText();
        if(PP.thereIsSomeComment())
            str += "\r\nPP comments:\r\n"+PP.getCommentsColumnText();
        output_filename = output_dir+"/outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "generating pair (PP, DP) online: ");
        throw E;
    }
}
*/
//check a pair (PP, DP) from a path file
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
        append("Positioning program loaded from '"+path_PP+"'.", log_filename.c_str());

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
        append("Depositioning program loaded from '"+path_DP+"'.", log_filename.c_str());

        //check the preconditions
        if(DP_label != "depos")
            throw EImproperArgument("DP label should be \"depos\"");
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
        str = "checkPairPPDP: "+BoolToStr(valid, true).str;
        string output_filename = "outputs-from-"+filename_PP+"-"+filename_DP;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "checking pair (PP, DP): ");
        throw E;
    }
}

//attempt regenerate a pair (PP, DP) from a path file
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
        append("Positioning program loaded from '"+path_PP+"'.", log_filename.c_str());

        //check the precondition
        if(PP_label != "pos")
            throw EImproperArgument("PP label should be \"pos\"");

        //load the DP from a file
        TMotionProgram DP;
        string DP_label;
        unsigned int DP_Bid;
        strReadFromFile(str, path_DP);
        DP.setInterfaceText(DP_label, DP_Bid, str);
        append("Depositioning program loaded from '"+path_DP+"'.", log_filename.c_str());

        //check the precondition
        if(DP_label != "depos")
            throw EImproperArgument("DP label should be \"depos\"");
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
        FMM.RPL.getPositions(IPL);
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
            PP.getInterfaceText(str, "pos", PP_Bid, IPL);
            append("Positiong program translated to the MCS format.", log_filename.c_str());
            output_filename = "regeneratedPP-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Regenerated PP saved in '"+output_filename+"'.", log_filename.c_str());

            //save the regenerated DP
            DP.getInterfaceText(str, "depos", DP_Bid, OPL);
            append("Depositiong program translated to the MCS format.", log_filename.c_str());
            output_filename = "regeneratedDP-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Regenerated DP saved in '"+output_filename+"'.", log_filename.c_str());

        } else
            append("The pair (PP, DP) can not be regenerated.", log_filename.c_str());

        //print the other outputs in the corresponding file
        str = "regeneratePairPPDP: "+BoolToStr(regenerated, true).str;
        str += "\r\nExcluded: "+Excluded.getText().str;
        output_filename = "outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "regenerating pair (PP, DP): ");
        throw E;
    }
}

//attempt generate a parking program from a path file
//Parameters for can use the function for test:
//  (ParkingProgramValid, Collided, Obstructed, PP, DP)
//Parameters for generate a pair PPDP:
//  (FMM, path, log_filename)
void generateParkingProgram_offline(bool& ParkingProgramValid,
                                    TRoboticPositionerList& Collided, TRoboticPositionerList& Obstructed,
                                    TMotionProgram& ParkingProgram,
                                    //------------------------------------------------------
                                    TFiberMOSModel& FMM, const string& input_path, const string& output_dir, const string& log_filename)
{
    try {

        //LOAD SETTINGS FROM FILES:

        //load the FMOSA table from a file
        TFMOSATable FMOSA;
        unsigned int Bid;
        string str;
        strReadFromFile(str, input_path);
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
        append("RPs moved to starting positions.", log_filename.c_str());

        //Other way to obtain the more closer stable points to the projection points,
        //consist in get from the FMOSA table the following lists:
        //  the allocation list;
        //  the projection point list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

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

        //segregates the operative outsider RPs
        TRoboticPositionerList Outsiders;
        FMM.RPL.segregateOperativeOutsiders(Outsiders);

        //check the limiting case when all operative RPs are in the origin
        if(FMM.RPL.allOperativeRPsAreInTheOrigin())
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

            //save the parking program in the format of the FMPT
            str = ParkingProgram.getText().str;
            output_filename = output_dir+"/ParkingProgram-FMPT-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Parking program in propietary format saved in '"+output_filename+"'.", log_filename.c_str());

            //Given that here the generated parking program is valid,
            //all operative outsider RPs which aren't obstructed are in the origin positions,
            //in their final position after execute the MP.

            //translates the parking program to the format of the interface MCS-FMPT
            //and save it in a file
            ParkingProgram.getInterfaceText(str, "depos", Bid, SPL);
            append("Parking program translated to the MCS format.", log_filename.c_str());
            output_filename = output_dir+"/ParkingProgram-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Parking program in MCS format saved in '"+output_filename+"'.", log_filename.c_str());

            //save ParkingProgram-Dfmin in a file
            ParkingProgram.getDfminInterfaceText(str, "depos", Bid);
            output_filename = output_dir+"/ParkingProgram-Dfmin-from-"+filename;
            strWriteToFile(output_filename, str);
            append("ParkingProgram-Dfmin saved in '"+output_filename+"'.", log_filename.c_str());
        }

        //Given that here the generated parking program can be valid or invalid,
        //all operative outsider RPs which aren't obstructed,can be:
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
        str = TActuator::getPositionPPALabelsRow().str;
        str += "\r\n"+FPL.getColumnText().str;
        output_filename = output_dir+"/FPL-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Final position list saved in '"+output_filename+"'.", log_filename.c_str());

        //print the other outputs in the corresponding file
        str = "\r\ngParkingProgramValid: "+BoolToStr(ParkingProgramValid, true).str;
        str += "\r\nCollided: "+Collided.getText().str;
        str += "\r\nObstructed: "+Obstructed.getText().str;
        if(ParkingProgram.thereIsSomeComment1())
            str += "\r\nMP comments:\r\n"+ParkingProgram.getComment1sColumnText();
        output_filename = output_dir+"/outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "generating parking program offline: ");
        throw E;
    }
}

/*//attempt generate a parking program from a path file
//Parameters for can use the function for test:
//  (ParkingProgramValid, Collided, Obstructed, PP, DP)
//Parameters for generate a pair PPDP:
//  (FMM, path, log_filename)
void generateParkingProgram_online(bool& ParkingProgramValid,
                                   TMotionProgram& ParkingProgram,
                                   //------------------------------------------------------
                                   TFiberMOSModel& FMM, const string& input_path, const string& output_dir, const string& log_filename,
                                   vector<int>& Ids)
{
    try {

        //LOAD SETTINGS FROM FILES:

        //load the FMOSA table from a file
        TFMOSATable FMOSA;
        unsigned int Bid;
        string str;
        strReadFromFile(str, input_path);
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
        append("RPs moved to starting positions.", log_filename.c_str());

        //Other way to obtain the more closer stable points to the projection points,
        //consist in get from the FMOSA table the following lists:
        //  the allocation list;
        //  the projection point list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

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

        //------------------------------------------------------------

        //get the position angles from the FMM
        vector<double> p_1s;
        vector<double> p___3s;
        for(int i=0; i<FMM.RPL.getCount(); i++) {
            TRoboticPositioner *RP = FMM.RPL[i];
            p_1s.push_back(RP->getActuator()->getp_1());
            p___3s.push_back(RP->getActuator()->getArm()->getp___3());
        }

        //randomize the position angles of the FMM
        FMM.RPL.randomizeWithoutCollision();

        //call to the online function
        append("Generating a parking program...", log_filename.c_str());
        ParkingProgram.Clear();
        Ids.clear();
        ParkingProgramValid = generateParkingProgram(ParkingProgram,
                                                     FMM, p_1s, p___3s,
                                                     Ids);

        //------------------------------------------------------------

        //if generation function was successfully generated
        if(ParkingProgramValid) {
            //indicates that the parking program has been generated successfully
            append("Generated parking program is valid.", log_filename.c_str());

            //save the parking program in the format of the FMPT
            str = ParkingProgram.getText().str;
            output_filename = output_dir+"/ParkingProgram-FMPT-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Parking program in propietary format saved in '"+output_filename+"'.", log_filename.c_str());

            //Given that here the generated parking program is valid,
            //all operative outsider RPs which aren't obstructed are in the origin positions,
            //in their final position after execute the MP.

            //translates the parking program to the format of the interface MCS-FMPT
            //and save it in a file
            ParkingProgram.getInterfaceText(str, "depos", Bid, SPL);
            append("Parking program translated to the MCS format.", log_filename.c_str());
            output_filename = output_dir+"/ParkingProgram-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Parking program in MCS format saved in '"+output_filename+"'.", log_filename.c_str());
        }

        //Given that here the generated parking program can be valid or invalid,
        //all operative outsider RPs which aren't obstructed,can be:
        //- in the origin positions, in their final position after execute the MP.
        //  if success == true.
        //- in the first position where the collision was detected.
        //  if success == false.

        //get and save the positions where the collision was detected
        TPairPositionAnglesList FPL;
        FMM.RPL.getPositions(FPL);
        str = TActuator::getPositionPPALabelsRow().str;
        str += "\r\n"+FPL.getColumnText().str;
        output_filename = output_dir+"/FPL-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Final position list saved in '"+output_filename+"'.", log_filename.c_str());

        //print the other outputs in the corresponding file
        str = "\r\ngParkingProgramValid: "+BoolToStr(ParkingProgramValid, true).str;
        if(ParkingProgram.thereIsSomeComment())
            str += "\r\nMP comments:\r\n"+ParkingProgram.getCommentsColumnText();
        output_filename = output_dir+"/outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "generating parking program online: ");
        throw E;
    }
}*/

//make a test for the function generatePairPPDP_offline
void testGeneratePairPPDP_offline(TFiberMOSModel& FMM, string& log_filename)
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
        output_dir = "testsGeneratePairPPDP_offline";
        ForceDirectories(AnsiString(output_dir));

        do {
            //contabilize the test and print the test tittle
            append("\r\nTEST "+inttostr(++count)+":", log_filename.c_str());
            append("=======", log_filename.c_str());

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
            MPG.RandomizeWithoutCollision();
            append("Projection points randomized avoinding collisions.", log_filename.c_str());

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

                TSPPP *SPPP = new TSPPP();

                //transcribe the allocation properties to the SPPP
                SPPP->Type = ptSOURCE;
                SPPP->Pid = A->getRP()->getActuator()->getId();
                SPPP->X = A->PP.x;
                SPPP->Y = A->PP.y;
                SPPP->Enabled = true;

                //set the optional values
                SPPP->Name = "s:"+inttostr(i+1);
                SPPP->Mag = 0;
                SPPP->Pr = 0;
                SPPP->Bid = 0;
                SPPP->notAllocated = true;
                SPPP->allocateInAll = false;
                SPPP->Comment = "too coment";

                //enable the optional values
                SPPP->there_is_Mag = true;
                SPPP->there_is_Pr = true;
                SPPP->there_is_Bid = true;
/*                SPPP->there_is_notAllocated = true;
                SPPP->there_is_allocateInAll = true;*/

                //add the SPPP to the FMOSA table
                FMOSAT.Add(SPPP);
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
            append("--------------------------------------------", log_filename.c_str());
            generatePairPPDP_offline(PPvalid, DPvalid, Collided, Obstructed, PP, DP,
                             FMM, input_path, output_dir, log_filename);
            append("------------------------------------------------", log_filename.c_str());
            append("Returned from function generatePairPPDP_offline.", log_filename.c_str());

            //Note that the outputs are saved in the function to test.

        } while(PPvalid && DPvalid && Collided.getCount()<=0 && Obstructed.getCount()<=0);

        //indicates that the test has failed
        append("\r\nFailed test "+inttostr(count)+"!", log_filename.c_str());
        append("PPvalid: "+BoolToStr(PPvalid, true).str, log_filename.c_str());
        append("DPvalid: "+BoolToStr(DPvalid, true).str, log_filename.c_str());
        append("Collided: "+Collided.gettText().str, log_filename.c_str());
        append("Obstructed: "+Obstructed.gettText().str, log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "testing generatePairPPDP_offline: ");
        throw E;
    }
}

//make a test for the function generatePairPPDP_online
void testGeneratePairPPDP_online(TFiberMOSModel& FMM, string& log_filename)
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

        output_dir = "testsGeneratePairPPDP_online";
        ForceDirectories(AnsiString(output_dir));

        do {
            //contabilize the test and print the test tittle
            append("\r\nTEST "+inttostr(++Bid)+":", log_filename.c_str());
            append("=======", log_filename.c_str());

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
            MPG.RandomizeWithoutCollision();
            append("Projection points randomized avoinding collisions.", log_filename.c_str());

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
            append("RPs moved to observing positions.", log_filename.c_str());

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
            /*TBD*/

            //call the function to test
            append("Calling function generatePairPPDP_online...", log_filename.c_str());
            append("-------------------------------------------", log_filename.c_str());
            PairPPDPvalid = generatePairPPDP_online(PP, DP,
                             FMM, p_1s, p___3s, Ids);
            append("-----------------------------------------------", log_filename.c_str());
            append("Returned from function generatePairPPDP_online.", log_filename.c_str());

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

                //save the DP in the format of the FMPT
                str = DP.getText().str;
                output_filename = output_dir+"/DP-FMPT-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Depositioning program in propietary format saved in '"+output_filename+"'.", log_filename.c_str());

                //save the PP in the format of the FMPT
                str = PP.getText().str;
                output_filename = output_dir+"/PP-FMPT-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Positioning program in propietary format saved in '"+output_filename+"'.", log_filename.c_str());

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

                //translates the depositioning program to the format of the interface MCS-FMPT
                //and save it in a file
                DP.getInterfaceText(str, "depos", Bid, OPL);
                append("Depositiong program translated to the MCS format.", log_filename.c_str());
                output_filename = output_dir+"/DP-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Depositioning program in MCS format saved in '"+output_filename+"'.", log_filename.c_str());

                //translates the positioning program to the format of the interface MCS-FMPT
                //and save it in a file
                PP.getInterfaceText(str, "pos", Bid, IPL);
                append("Positiong program translated to the MCS format.", log_filename.c_str());
                output_filename = output_dir+"/PP-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Positioning program in MCS format saved in '"+output_filename+"'.", log_filename.c_str());
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
            output_filename = output_dir+"/outputs-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

            //END OF SAVE THE OUTPUTS

        } while(PairPPDPvalid); // && Collided.getCount()<=0 && Obstructed.getCount()<=0);

        //Note that not collided and not obstructed should be part of the stop condition,
        //but they are discarted in the function of generation online.

        //indicates that the test has failed
        append("\r\nFailed test "+inttostr(Bid)+"!", log_filename.c_str());
        append("PairPPDPvalid: "+BoolToStr(PairPPDPvalid, true).str, log_filename.c_str());
        //append("Collided: "+Collided.gettText().str, log_filename.c_str());
        //append("Obstructed: "+Obstructed.gettText().str, log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "testing generatePairPPDP_online: ");
        throw E;
    }
}

//make a test for the function generateParkingProgram_offline
void testGenerateParkingProgram_offline(TFiberMOSModel& FMM, string& log_filename)
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

        output_dir = "testsGenerateParkingProgram_offline";
        ForceDirectories(AnsiString(output_dir));

        do {
            //contabilize the test and print the test tittle
            append("\r\nTEST "+inttostr(++count)+":", log_filename.c_str());
            append("=======", log_filename.c_str());

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
            MPG.RandomizeWithoutCollision();
            append("Projection points randomized avoinding collisions.", log_filename.c_str());

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

                TSPPP *SPPP = new TSPPP();

                //transcribe the allocation properties to the SPPP
                SPPP->Type = ptSOURCE;
                SPPP->Pid = A->getRP()->getActuator()->getId();
                SPPP->X = A->PP.x;
                SPPP->Y = A->PP.y;
                SPPP->Enabled = true;

                //set the optional values
                SPPP->Name = "s:"+inttostr(i+1);
                SPPP->Mag = 0;
                SPPP->Pr = 0;
                SPPP->Bid = 0;
                SPPP->notAllocated = true;
                SPPP->allocateInAll = false;
                SPPP->Comment = "too coment";

                //enable the optional values
                SPPP->there_is_Mag = true;
                SPPP->there_is_Pr = true;
                SPPP->there_is_Bid = true;
/*                SPPP->there_is_notAllocated = true;
                SPPP->there_is_allocateInAll = true;*/

                //add the SPPP to the FMOSA table
                FMOSAT.Add(SPPP);
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
            append("--------------------------------------------------", log_filename.c_str());
            generateParkingProgram_offline(ParkingProgramValid, Collided, Obstructed, ParkingProgram,
                                   FMM, input_path, output_dir, log_filename);
            append("------------------------------------------------------", log_filename.c_str());
            append("Returned from function generateParkingProgram_offline.", log_filename.c_str());

            //Note that the outputs are saved in the function to test.

        } while(ParkingProgramValid && Collided.getCount()<=0 && Obstructed.getCount()<=0);

        //indicates that test
        append("\r\nFailed test "+inttostr(count)+"!", log_filename.c_str());
        append("ParkingProgramValid: "+BoolToStr(ParkingProgramValid, true).str, log_filename.c_str());
        append("Collided: "+Collided.gettText().str, log_filename.c_str());
        append("Obstructed: "+Obstructed.gettText().str, log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "testing generatePairPPDP_offline: ");
        throw E;
    }
}

//make a test for the function generateParkingProgram_online
void testGenerateParkingProgram_online(TFiberMOSModel& FMM, string& log_filename)
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

        output_dir = "testsGenerateParkingProgram_online";
        ForceDirectories(AnsiString(output_dir));

        do {
            //contabilize the test and print the test tittle
            append("\r\nTEST "+inttostr(++Bid)+":", log_filename.c_str());
            append("=======", log_filename.c_str());

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
            MPG.RandomizeWithoutCollision();
            append("Projection points randomized avoinding collisions.", log_filename.c_str());

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
            append("RPs moved to observing positions.", log_filename.c_str());

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
            /*TBD*/

            //call the function to test
            append("Calling function generateParkingProgram_online...", log_filename.c_str());
            append("-------------------------------------------------", log_filename.c_str());
            ParkingProgramValid = generateParkingProgram_online(ParkingProgram,
                                                                FMM, p_1s, p___3s, Ids);
            append("-----------------------------------------------------", log_filename.c_str());
            append("Returned from function generateParkingProgram_online.", log_filename.c_str());

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

                //save the ParkingProgram in the format of the FMPT
                str = ParkingProgram.getText().str;
                output_filename = output_dir+"/ParkingProgram-FMPT-from-"+filename;
                strWriteToFile(output_filename, str);
                append("Parking program in propietary format saved in '"+output_filename+"'.", log_filename.c_str());

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
                ParkingProgram.getInterfaceText(str, "depos", Bid, SPL);
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
            output_filename = output_dir+"/outputs-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

            //END OF SAVE THE OUTPUTS

        } while(ParkingProgramValid); // && Collided.getCount()<=0 && Obstructed.getCount()<=0);

        //Note that not collided and not obstructed should be part of the stop condition,
        //but they are discarted in the function of generation online.

        //indicates that the test has failed
        append("\r\nFailed test "+inttostr(Bid)+"!", log_filename.c_str());
        append("ParkingProgramValid: "+BoolToStr(ParkingProgramValid, true).str, log_filename.c_str());
        //append("Collided: "+Collided.gettText().str, log_filename.c_str());
        //append("Obstructed: "+Obstructed.gettText().str, log_filename.c_str());

    } catch(Exception& E) {
        E.Message.str.insert(0, "testing generateParkingProgram_online: ");
        throw E;
    }
}

//take the measure of deviation around the radial trayectory of each RP
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
        append("\r\nSIMULATION THE RADIAL EXTENSION AND RADIAL RETRACTION OF THE FIBERS:", log_filename.c_str());
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

            //The radial trayectory correspond to segment (P3ini, P3fin).

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

            //add a note aboput the result
            string str = RP->getActuator()->getIdText().str;
            str += "\t"+floattostr(distanceMin)+"\t"+floattostr(distanceMax);

            //print the result
            append(str, log_filename.c_str());
        }

    } catch(Exception& E) {
        E.Message.str.insert(0, "testing radial motion: ");
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
        append("FMPT SAA 3.0.5 is running...", log_filename.c_str());

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
        //-------------------------------------------------------------------
        else if(command == "valuesSPM") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command valuesSPM sould have 0 arguments");

            TRoboticPositioner *RP = FMM.RPL[0];
            AnsiString S;
            S = AnsiString("SPM values:\r\n");
            S += RP->getActuator()->getSecurityText();

            //save the instance of the Fiber MOS Model from where was loaded
            append(S.str, log_filename.c_str());
        }
        else if(command == "testRadialMotion") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command testRadialMotion should have 0 arguments");

            //execute the test
            testRadialMotion(FMM, log_filename);
        }
        //-------------------------------------------------------------------
        else if(command == "applyPC") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command applyCP sould have 1 arguments");

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
                throw EImproperArgument("command applyRP sould have 1 arguments");

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
        //-------------------------------------------------------------------
        else if(command == "generatePairPPDP_offline") {
            //check the precondition
            if(argc != 3)
                throw EImproperArgument("command generatePairPPDP_offline sould have 1 arguments");

            //built a path from arg 1
            string path_FMOSA(argv[2]);

            //make a rutinary check
            if(path_FMOSA.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(path_FMOSA[0] != '/')
                path_FMOSA.insert(0, getCurrentDir()+"/");

            //generate a pair (PP, DP) from a path and write the events in the log file
            bool PPvalid;
            bool DPvalid;
            TRoboticPositionerList Collided;
            TRoboticPositionerList Obstructed;
            TMotionProgram PP;
            TMotionProgram DP;
            generatePairPPDP_offline(PPvalid, DPvalid, Collided, Obstructed, PP, DP,
                             FMM, path_FMOSA, ".", log_filename);
        }
        else if(command == "testGeneratePairPPDP_offline") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command testGeneratePairPPDP_offline should have 0 arguments");

            //execute the test
            testGeneratePairPPDP_offline(FMM, log_filename);
        }
        else if(command == "testGeneratePairPPDP_online") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command testGeneratePairPPDP_online should have 0 arguments");

            //execute the test
            testGeneratePairPPDP_online(FMM, log_filename);
        }
        //-------------------------------------------------------------------
        else if(command == "checkPairPPDP") {
            //check the precondition
            if(argc!=4 && argc!=5)
                throw EImproperArgument("command checkPairPPDP sould have 2 or 3 arguments");

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

            //validates a pair (PP, DP) from a path and write the events in the log file
            checkPairPPDP(FMM, path_PP, path_DP, log_filename);
        }
        else if(command == "regeneratePairPPDP") {
            //check the precondition
            if(argc!=5 && argc!=6)
                throw EImproperArgument("command regeneratePairPPDP sould have 3 or 4 arguments");

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
            if(argc != 3)
                throw EImproperArgument("command generateParkingProgram_offline sould have 1 arguments");

            //built a path from arg 1
            string path_FMOSA(argv[2]);

            //make a rutinary check
            if(path_FMOSA.length() <= 0)
                throw EImpossibleError("lateral effect");

            //complete the relative path, if any
            if(path_FMOSA[0] != '/')
                path_FMOSA.insert(0, getCurrentDir()+"/");

            //generate a DP from a path and write the events in the log file
            bool ParkingProgramValid;
            TRoboticPositionerList Collided;
            TRoboticPositionerList Obstructed;
            TMotionProgram ParkingProgram;
            generateParkingProgram_offline(ParkingProgramValid, Collided, Obstructed, ParkingProgram,
                                   FMM, path_FMOSA, ".", log_filename);
        }
        else if(command == "testGenerateParkingProgram_offline") {
            //check the precondition
            if(argc != 2)
                throw EImproperArgument("command testGenerateParkingProgram_offline should have 0 arguments");

            //execute the test
            testGenerateParkingProgram_offline(FMM, log_filename);
        }
        //-------------------------------------------------------------------
        else if(command == "aboutOf") {
            //print the about of legend
            append("\r\n"+aboutOf(), log_filename.c_str());
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
