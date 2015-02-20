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
#include "SPPP.h"
#include "PositionerCenter.h"
#include "Strings.h"
#include "TextFile.h"

#include <locale.h> //setlocale, LC_NUMERIC
#include <iostream> //std::cout

using namespace Strings;
using namespace Models;
using namespace Positioning;

//print a text line in the standard output and in the log file
void append(const string& str, const char *log_filename)
{
    std::cout << str.c_str() << endl;

    char mode[] = "a";
    TTextFile TF(log_filename, mode);
    string str_aux = str;
    str_aux += "\r\n";
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

//print the help in the standard output
void print_help(void)
{
    std::cout << "\r\nfmpt_saa -applyPositionerCenterTable <path>";
    std::cout << "\r\n\t<path>: absolute or relative path to file containing a positioner center table.";
    std::cout << endl;
    std::cout << "\r\nfmpt_saa -applyRP <path>";
    std::cout << "\r\n\t<path>: absolute or relative path to dir containing a RP instance.";
    std::cout << endl;
    std::cout << "\r\nfmpt_saa -generateDP <path>";
    std::cout << "\r\n\t<path>: absolute or relative path to file containing a PP table." << endl;
    std::cout << endl;
    std::cout << "\r\nfmpt_saa -generatePPDP <path>";
    std::cout << "\r\n\t<path>: absolute or relative path to file containing a PP table." << endl;
}

//apply a positioner center table to the Fiber MOS Model Instance
void applyPositionerCenterTable(string& dir1, string& dir2, string& path, string& log_filename)
{
    try {

        //BUILDS THE OBJECTS:

        //build the Fiber MOS Model
        TFiberMOSModel FMM;
        //build the positioner center list
        TPositionerCenterList PCL;

        //LOAD SETTINGS FROM FILES:

        //load the instance of the Fiber MOS Model from a dir
        char selected_dir = 0;
        string dir;
        try {
            dir = dir1;
            readInstanceFromDir(FMM, dir);
            selected_dir = 1;
        } catch(...) {
            try {
                dir = dir2;
                readInstanceFromDir(FMM, dir);
                selected_dir = 2;
            } catch(...) {
                throw;
            }
        }
        append("Fiber MOS Model instance loaded from '"+dir+"'.", log_filename.c_str());

        //load the positioner center table from a file
        try {
            string str;
            strReadFromFile(str, path);
            PCL.setTableText(str);
        } catch(...) {
            throw;
        }
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

        //save the instance of the Fiber MOS Model to the default directory
        switch(selected_dir) {
        case 1:
            dir = dir1;
            break;
        case 2:
            dir = dir2;
            break;
        default:
            throw EImpossibleError("lateral effect");
        }
        try {
            writeInstanceToDir(dir, FMM);
        } catch(...) {
            throw;
        }
        append("Fiber MOS Model instance saved in '"+dir+"'.", log_filename.c_str());

        //DESTROY THE OBJECTS:

    } catch(...) {
        throw;
    }
}

//apply a RP instance for all RPs of the Fiber MOS Model Instance
void applyRP(string& dir1, string& dir2, string& path, string& log_filename)
{
    try {

        //BUILDS THE OBJECTS:

        //build the Fiber MOS Model
        TFiberMOSModel FMM;
        //build the RP Model
        TRoboticPositioner RP;

        //LOAD SETTINGS FROM FILES:

        //load the instance of the Fiber MOS Model from a dir
        string dir;
        char selected_dir = 0;
        try {
            dir = dir1;
            readInstanceFromDir(FMM, dir);
            selected_dir = 1;
        } catch(...) {
            try {
                dir = dir2;
                readInstanceFromDir(FMM, dir);
                selected_dir = 2;
            } catch(...) {
                throw;
            }
        }
        append("Fiber MOS Model instance loaded from '"+dir+"'.", log_filename.c_str());

        //load the RP instance from a dir
        try {
            readInstanceFromDir(RP, path);
        } catch(...) {
            throw;
        }
        append("RP instance loaded from '"+path+"'.", log_filename.c_str());

        //MAKE THE OPERATIONS:

        //assimilates the instance of the RP in the Fiber MOS Model
        FMM.RPL.Apply(RP);
        append("RP instance applied to the Fiber MOS Model.", log_filename.c_str());

        //save the instance of the Fiber MOS Model to the default directory
        switch(selected_dir) {
        case 1:
            dir = dir1;
            break;
        case 2:
            dir = dir2;
            break;
        default:
            throw EImpossibleError("lateral effect");
        }
        try {
            writeInstanceToDir(dir, FMM);
        } catch(...) {
            throw;
        }
        append("Fiber MOS Model instance saved in '"+dir+"'.", log_filename.c_str());

        //DESTROY THE OBJECTS:

    } catch(...) {
        throw;
    }
}

//attempt generate a DP from a path file
void generateDP(string& dir1, string& dir2, string& path, string& log_filename)
{
    try {

        //BUILDS THE OBJECTS:

        //build the Fiber MOS Model
        TFiberMOSModel FMM;
        //build the Motion Program Generator attachet to the Fiber MOS Model
        TMotionProgramGenerator MPG(&FMM);

        //LOAD SETTINGS FROM FILES:

        //load the instance of the Fiber MOS Model from a dir
        string dir;
        try {
            dir = dir1;
            readInstanceFromDir(FMM, dir);
        } catch(...) {
            try {
                dir = dir2;
                readInstanceFromDir(FMM, dir);
            } catch(...) {
                throw;
            }
        }
        append("Fiber MOS Model instance loaded from '"+dir+"'.", log_filename.c_str());

        //load the PP table from a file
        TSPPPList SPPPL;
        try {
            string str;
            strReadFromFile(str, path);
            SPPPL.setTableText(str);
        } catch(...) {
            throw;
        }
        append("PP table loaded from '"+path+"'.", log_filename.c_str());

        //assign the PP table to the MPG
        SPPPL.getTPL(MPG);
        append("PP table allocated to the MPG.", log_filename.c_str());

        //MAKE THE OPERATIONS:

        //split the path of the file containing the PP table
        string parent_path, filename;
        splitpath(parent_path, filename, path);

        //The filename will be used to attach the outputs filenames witht the input filename.

        //move the RPs to the more closer stable point to the TPs
        MPG.MoveToTargetP3();
        append("RPs moved to target points.", log_filename.c_str());

        //Other way to obtain the more closer stablepoints to the PPs,
        //consist in get from the PP table the following lists:
        //  the allocation list;
        //  the PP list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

        //A PPA table shall be stored how a table (Id, p_1, p___3).

        //captures the initial positions of the RPs in a PPA list
        TPairPositionAnglesList IPL;
        FMM.RPL.GetPositions(IPL);
        string str = TActuator::GetPositionPPALabelsRow().str;
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
            FMM.RPL.translateMotionProgram(str, 1, "obs depos", IPL, DP);
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
        str = "generateDepositioningProgram: "+BoolToStr(success).str+"\r\n";
        str += "Collided: "+Collided.getText().str+"\r\n";
        str += "Obstructed: "+Obstructed.getText().str+"\r\n";
        output_filename = "outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Others outputs saved in '"+output_filename+"'.", log_filename.c_str());

        //DESTROY THE OBJECTS:

    } catch(...) {
        throw;
    }
}

//attempt generate a pair (PP, DP) from a path file
void generatePPDP(string& dir1, string& dir2, string& path, string& log_filename)
{
    try {

        //BUILDS THE OBJECTS:

        //build the Fiber MOS Model
        TFiberMOSModel FMM;
        //build the Motion Program Generator attachet to the Fiber MOS Model
        TMotionProgramGenerator MPG(&FMM);

        //LOAD SETTINGS FROM FILES:

        //load the instance of the Fiber MOS Model from a dir
        string dir;
        try {
            dir = dir1;
            readInstanceFromDir(FMM, dir);
        } catch(...) {
            try {
                dir = dir2;
                readInstanceFromDir(FMM, dir);
            } catch(...) {
                throw;
            }
        }
        append("Fiber MOS Model instance loaded from '"+dir+"'.", log_filename.c_str());

        //load the PP table from a file
        TSPPPList SPPPL;
        try {
            string str;
            strReadFromFile(str, path);
            SPPPL.setTableText(str);
        } catch(...) {
            throw;
        }
        append("PP table loaded from '"+path+"'.", log_filename.c_str());

        //assign the PP table to the MPG
        SPPPL.getTPL(MPG);
        append("PP table allocated to the MPG.", log_filename.c_str());

        //MAKE THE OPERATIONS:

        //split the path of the file containing the PP table
        string parent_path, filename;
        splitpath(parent_path, filename, path);

        //The filename will be used to attach the outputs filenames witht the input filename.

        //move the RPs to the more closer stable point to the TPs
        MPG.MoveToTargetP3();
        append("RPs moved to target points.", log_filename.c_str());

        //Other way to obtain the more closer stablepoints to the PPs,
        //consist in get from the PP table the following lists:
        //  the allocation list;
        //  the PP list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

        //A PPA table shall be stored how a table (Id, p_1, p___3).

        //captures the observing positions of the RPs in a PPA list
        TPairPositionAnglesList OPL;
        FMM.RPL.GetPositions(OPL);
        string str = TActuator::GetPositionPPALabelsRow().str;
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
        append("Generating depositioning program...", log_filename.c_str());
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
            FMM.RPL.translateMotionProgram(str, 1, "obs pos", IPL, PP);
            append("Positiong program translated to the format of MCS interface.", log_filename.c_str());
            output_filename = "PP-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Positioning program saved in '"+output_filename+"'.", log_filename.c_str());

            //translates the depositioning program to the format of the interface MCS-FMPT
            //and save it in a file
            FMM.RPL.translateMotionProgram(str, 1, "obs depos", OPL, DP);
            append("Depositiong program translated to the format of MCS interface.", log_filename.c_str());
            output_filename = "DP-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Depositioning program saved in '"+output_filename+"'.", log_filename.c_str());
        }

        //print the other outputs in the corresponding file
        str = "generatePairPPDP: "+BoolToStr(success).str+"\r\n";
        str += "Collided: "+Collided.getText().str+"\r\n";
        str += "Obstructed: "+Obstructed.getText().str+"\r\n";
        output_filename = "outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Others outputs saved in '"+output_filename+"'.", log_filename.c_str());

        //DESTROY THE OBJECTS:

    } catch(...) {
        throw;
    }
}

//main function
int main(int argc, char *argv[])
{
    //------------------------------------------------------------------
    //CONFIGURATES THE SYSTEM:

    //REMEMBER: exceptions in runtime can be due to that
    //the system is not configurated.

    //configurates the decimal separator
    setlocale(LC_NUMERIC, "C");

    //initialize the global variables
    if(TrueBoolStrs.getCount() < 1) {
        TrueBoolStrs.setCount(1);
        TrueBoolStrs[0] = "True";
    }
    if(FalseBoolStrs.getCount() < 1) {
        FalseBoolStrs.setCount(1);
        FalseBoolStrs[0] = "False";
    }

    //------------------------------------------------------------------

    //if the program is run without the necessary arguments
    if(argc != 3) {
        //indicates that the arguments has been missed
        std::cout << "Missing arguments.";

        //show the help
        std::cout << endl;
        print_help();

        //indicates that the program has been executed without error
        std::cout << endl;
        return 0;
    }

    //if the program is run with 2 arguments
    else { //argc == 3

        //####################################################################################################
        //Using autotools over Linux:
        //---------------------------------------------------------
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
        //      option 2, when program is compiled:     GetCurrentDir().str+"/../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"
        //  The input data:
        //      path(argv[2])
        //
        //DATADIR is a macro which can be used when the program is compiled using autotools.
        //DATADIR will be probably: "/urs/local/share/megara-fmpt"
        //
        //When the program is compiled using Qt Creator over Linux:
        //---------------------------------------------------------
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
        //      when program is released:  GetCurrentDir().str+"/../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"
        //      when program is debugging: GetCurrentDir().str+"/../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"
        //  The input data:
        //      path = GetCurrentDir().str+"/../megarafmpt/data/Models/positionersCenters.txt";
        //      path = GetCurrentDir().str+"/../megarafmpt/data/Models/MEGARA_RP_Instance";
        //      path = GetCurrentDir().str+"/../megarafmpt/data/Samples/megara-cb6.txt";
        //
        //Using Qt over Windows:
        //---------------------------------------------------------
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
        //      when program is released:  GetCurrentDir().str+"\..\megarafmpt\data\Models\MEGARA_FiberMOSModel_Instance"
        //      when program is debugging: GetCurrentDir().str+"\..\megarafmpt\data\Models\MEGARA_FiberMOSModel_Instance"
        //  The input data:
        //      for applyPositionerCenterTable: path = GetCurrentDir().str+"/../megarafmpt/data/Models/positionersCenters.txt";
        //      for applyRP:                    path = GetCurrentDir().str+"/../megarafmpt/data/Models/MEGARA_RP_Instance";
        //      for generateDP:                 path = GetCurrentDir().str+"/../megarafmpt/data/Samples/megara-cb6.txt";
        //
        //####################################################################################################

        //BUILD THE PATH FOR THE INSTANCE OF THE Fiber MOS Model:

        string dir1 = DATADIR;
        dir1 += "/Models/MEGARA_FiberMOSModel_Instance";
//        string dir2 = GetCurrentDir().str+"/../data/Models/MEGARA_FiberMOSModel_Instance";
        string dir2 = dir1;

        //BUILD THE PATH FOR INPUT DATA:

        //built a path from arg 2
        string path(argv[2]);

        //split the path of the file containing the input data
        string parent_path, filename;
        splitpath(parent_path, filename, path);

        //build the log filename
        string log_filename = filename+".log";

        //initalize the log file
        char mode[] = "w";
        TTextFile TF(log_filename.c_str(), mode);
        TF.Close();

        //indicates that the program is running
        append("Program fmpt_saa is running...", log_filename.c_str());

        //if arg 1 is "-applyPositionerCenterTable"
        if(string(argv[1]) == "-applyPositionerCenterTable") {
            try {
                //apply a positionersCenters table to the Fiber MOS Model Instance
                applyPositionerCenterTable(dir1, dir2, path, log_filename);

            } catch(Exception &E) {
                //indicates that has happened an exception
                //and show the message of the exception
                append("ERROR: "+E.Message.str, log_filename.c_str());
                return 1;

            } catch(...) {
                //indicates that has happened an unknoledge exception
                append("ERROR: unknowledge exception", log_filename.c_str());
                return 2;
            }
        }

        //else if arg 1 is "-applyRP"
        else if(string(argv[1]) == "-applyRP") {
            try {
                //apply a RP model for all RPs of the Fiber MOS Model Instance
                applyRP(dir1, dir2, path, log_filename);

            } catch(Exception &E) {
                //indicates that has happened an exception
                //and show the message of the exception
                append("ERROR: "+E.Message.str, log_filename.c_str());
                return 1;

            } catch(...) {
                //indicates that has happened an unknoledge exception
                append("ERROR: unknowledge exception", log_filename.c_str());
                return 2;
            }
        }

        //else if arg 1 is "-generateDP"
        else if(string(argv[1]) == "-generateDP") {
            try {
                //generate a DP from a path and write the events in the log file
                generateDP(dir1, dir2, path, log_filename);

            } catch(Exception &E) {
                //indicates that has happened an exception
                //and show the message of the exception
                append("ERROR: "+E.Message.str, log_filename.c_str());
                return 1;

            } catch(...) {
                //indicates that has happened an unknoledge exception
                append("ERROR: unknowledge exception", log_filename.c_str());
                return 2;
            }
        }

        //else if arg 1 is "-generatePPDP"
        else if(string(argv[1]) == "-generatePPDP") {
            try {
                //generate a pair (PP, DP) from a path and write the events in the log file
                generatePPDP(dir1, dir2, path, log_filename);

            } catch(Exception &E) {
                //indicates that has happened an exception
                //and show the message of the exception
                append("ERROR: "+E.Message.str, log_filename.c_str());
                return 1;

            } catch(...) {
                //indicates that has happened an unknoledge exception
                append("ERROR: unknowledge exception", log_filename.c_str());
                return 2;
            }
        }

        //if the arg 1 is unknowledge
        else {
            //indicates that the modifier is unknoledge
            std::cout << "Unknowledge modifier: ";
            std::cout << argv[1];

            //print the help
            std::cout << endl;
            print_help();

            //indicates that the program has been executed without error
            std::cout << endl;
            return 0;
        }

    } //else if(argc == 3)

    //------------------------------------------------------------------

    //indicates that the program has been executed without error
    return 0;
}

