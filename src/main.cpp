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
//Content: lista de elementos genéricos
//Last update: 23/01/2015
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "FileMethods.h"
#include "MotionProgramGenerator.h"
#include "SPPP.h"
#include "Strings.h"
#include "TextFile.h"

#include <locale.h> //setlocale, LC_NUMERIC
#include <cstring> //strlen
#include <iostream> //std

//using namespace Mathematics;
using namespace Strings;
using namespace Models;
using namespace Positioning;

//print a text line in the standard output and in the log file
void append_(const string& str, const char *log_filename)
{
    std::cout << str.c_str() << endl;

    char mode[] = "a";
    TTextFile TF(log_filename, mode);
    TF.Print((str+"\r\n").c_str());
    TF.Close();
}

//To avoid need to link the Boost.System library, little functions
//will be defined to replace some boost functionalities:
//  #include <boost/filesystem.hpp>
//  boost::filesystem::path path("...");
//  boost::filesystem::path filename = path.filename;

//split a path
void splitpath_(string& parent_path, string& filename, const string& path)
{
    int i = path.length() - 1;
    while(i>=0 && path[i] != '/')
            i--;

    parent_path.clear();
    if(i > 0)
        parent_path = path.substr(0, i);

    filename.clear();
    if(i < int(path.length() - 1)) {
        int count = path.length() - 1 - i;
        filename = path.substr(i+1, count);
    }
}

//main program
int main(int argc, char *argv[])
{
    //------------------------------------------------------------------
    //CONFIGURATES THE SYSTEM:

    //REMEMBER: exceptions in runtime can be due to that
    //the system is not configurated.

    //configurates the decimal separator
    setlocale(LC_NUMERIC, "C");

    //configurates the global variables in order to the function
    //StrToBool can work
    if(TrueBoolStrs.getCount() < 1) {
        TrueBoolStrs.setCount(1);
        TrueBoolStrs[0] = "True";
    }
    if(FalseBoolStrs.getCount() < 1) {
        FalseBoolStrs.setCount(1);
        FalseBoolStrs[0] = "False";
    }

    //WARNING: function BoolToStr not check the precondition:
    //      TrueBoolStrs[0] == "True" && FalseBoolStrs[0] == "False",
    //instead of these shall be used the function BoolToStr_ which
    //check the precondition.

    //------------------------------------------------------------------

    //if the program is run without arguments
    if(argc!=2 || strlen(argv[1])<=0 || (strlen(argv[1])==1 && argv[1][0]=='/')) {
	//show the help and indicates that the program has been executed without error
        std::cout << "missing argument.\r\nfmpt_saa <path>\r\n\t<path>: absolute or relative path to file containing a PP table." << endl;
        return 0;
    }

    //split the path of the file containing the PP table
    string path = string(argv[1]);
    string parent_path, filename;
    splitpath_(parent_path, filename, path);

    //get the log filename
    string log_filename = "log-from-"+filename;

    try {
        //initalize the log file
        char mode[] = "w";
        TTextFile TF(log_filename.c_str(), mode);
        TF.Close();

        //indicates that the program is running
        append_("Program fmpt_saa is running...", log_filename.c_str());

        //BUILDS THE OBJECTS:

        //build the Fiber MOS Model
        TFiberMOSModel FMM;
        //build the Motion Program Generator attachet to the Fiber MOS Model
        TMotionProgramGenerator MPG(&FMM);

        //LOAD SETTTINGS FROM FILES:

        //####################################################################################################
        //When the program is compiled using autotools:
        //--------------------------------------------
        //
        //Calling WORKINGDIR to '/home/Isaac/MEGARA'
        //  sources shall be in:    'WORKINGDIR/megarafmpt/src'
        //  data shall be in:       'WORKINGDIR/megarafmpt/data'
        //When compilation has been make in an appart directory named build:
        //  sources will be in:     'WORKINGDIR/build/src'
        //  data will be in:        'WORKINGDIR/build/data'
        //When the installation has been make:
        //  executable will be in:  'prefix/bin'
        //  data will be in:        'prefix/share/megara-fmpt'
        //Where prefix is probably '/usr/local'.
        //
        //Then the program fmpt_saa compiled with autotools, shall be search the data in the following paths:
        //  when program is installed:  DATADIR+"..." or GetCurrentDir()+"/../share/megara-fmpt..."
        //  when program is compiled:   GetCurrentDir()+"/../data..."
        //DATADIR is a macro which can be used when the program is compiled using autotools.
        //It is say, that DATADIR will be probably: "/urs/local/share/megara-fmpt"
        //
        //When the program is compiled using Qr Creator:
        //----------------------------------------------
        //
        //The main file will be in:
        //  'WORKINGDIR/FMPT_SAA-CLI'
        //The object code and the executable will be in:
        //  'WORKINGDIR/build-FMPT_SAA-CLI-Desktop_Qt_5_4_0_GCC_64bit-Debug'
        //  'WORKINGDIR/build-FMPT_SAA-CLI-Desktop_Qt_5_4_0_GCC_64bit-Release'
        //The other source files and data files will be in the same repository for autotools,
        //whose relative path from the executable is:
        //  sources shall be in:    '../megarafmpt/src'
        //  data shall be in:       '../megarafmpt/data'
        //
        //But the app fmpt_saa, shall be provided with the data in the same directory
        //where is the executable.
        //
        //Then the program fmpt_saa sompiled with Qt, shall be search the data in the following paths:
        //  when program is released:  GetCurrentDir()
        //  when program is debugging: GetCurrentDir()+"/../megarafmpt/data"
        //####################################################################################################

        //load the instance of the Fiber MOS Model from the default directory
        AnsiString DirName;
        try {
            DirName = AnsiString(DATADIR)+AnsiString("/Models/MEGARA_FiberMOSModel_Instance");
            ReadInstanceFromDir(FMM, DirName);
        } catch(...) {
            try {
                DirName = GetCurrentDir()+AnsiString("/../data/Models/MEGARA_FiberMOSModel_Instance");
                ReadInstanceFromDir(FMM, DirName);
            } catch(...) {
                throw;
            }
        }
        append_("Fiber MOS Model instance loaded from '"+DirName.str+"'.", log_filename.c_str());

        //load the PP table from a file
        AnsiString S;
        AnsiString FileName(path);
        try {
            StrReadFromFile(S, FileName);
        } catch(...) {
            throw;
        }
        append_("PP table loaded from '"+FileName.str+"'.", log_filename.c_str());

        //translate the SPPP table to structure
        TSPPPList SPPPL;
        SPPPL.setTableText(S);

        //assign the PP table to the MPG
        SPPPL.getTPL(MPG);
        append_("PP table allocated to the MPG.", log_filename.c_str());

        //MAKE THE OPERATIONS:

        //move the RPs to the more closer stable point to the TPs
        MPG.MoveToTargetP3();
        append_("RPs moved to target points.", log_filename.c_str());

        //Other way to obtain the more closer stablepoints to the PPs,
        //consist in get from the PP table the following lists:
        //  the allocation list;
        //  the PP list.
        //Them using the Fiber MOS Model, get the PPA list corresponding to these lists.

        //A PPA table shall be stored how a table (Id, p_1, p___3).

        //captures the initial positions of the RPs in a PPA list
        TPairPositionAnglesList IPL;
        FMM.RPL.GetPositions(IPL);
        S = TActuator::GetPositionPPALabelsRow()+AnsiString("\r\n");
        S += IPL.getColumnText();
        string output_filename = "IPL-from-"+filename;
        StrWriteToFile(output_filename, S);
        append_("Initial position list saved in '"+output_filename+"'.", log_filename.c_str());

        //Other whay to obtain the initial position table directly in text format:
        //  FMM.RPL.getPositionsPAPTableText()

        //segregates the operative outsiders RPs
        TRoboticPositionerList Outsiders;
        FMM.RPL.segregateOperativeOutsiders(Outsiders);

        //check the limiting case when all operative RPs are in the origin
        if(FMM.RPL.allOperativeRPsAreInTheOrigin())
            append_("WARNING: all operative RPs are in the origin. The generated DP will be empty.", log_filename.c_str());
        //else, check the limiting case when all operative RPs are in security positions
        else if(Outsiders.getCount() <= 0)
            append_("WARNING: all operative RPs are in security positions. The generated DP will contains only a message-instruction list to go to the origin.", log_filename.c_str());

        //generates a de positioning program for the operative RPs in insecure positions
        //and determines the RPs in collision status or obstructed in insecure positions
        append_("Generating depositioning program...", log_filename.c_str());
        TRoboticPositionerList Collided;
        TRoboticPositionerList Obstructed;
        TMotionProgram DP;
        bool success = MPG.generateDepositioningProgram(Collided, Obstructed, DP, Outsiders);

        //if generation function was successfully generated
        if(success) {
            //indicates that themotion program has been generated
            append_("Depositioning program generated.", log_filename.c_str());

            //Here all operative outsiders RPs which aren't obstructed
            //are in secure position, in their final position
            //after has been execute the DP.

            //segregate the operative inners RPs out of the origin and sorts it
            TRoboticPositionerList Inners;
            FMM.RPL.segregateOperativeInnersOutTheOrigins(Inners);
            Inners.SortInc();

            //Sort the RPs isn't really necessary, but is recomendable because produce a more legible output.

            //generates the parking gesture for the operative RPs in secure position out the origin
            MPG.getTheMessageListToGoToTheOrigins(DP, Inners);
            append_("Message list to go the origins added to depositioning program.", log_filename.c_str());

            //translates the depositioning program to the format of the interface MCS-FMPT
            //and save it in a file
            FMM.RPL.TranslateMotionProgram(S, 1, IPL, DP);
            append_("Depositiong program translated to the format of MCS interface.", log_filename.c_str());
            output_filename = "DP-from-"+filename;
            StrWriteToFile(output_filename, S);
            append_("Depositioning program saved in '"+output_filename+"'.", log_filename.c_str());

            //print the other outputs in the corresponding file
            S = AnsiString("generateDepositioningProgram: true\r\n");
            S += AnsiString("Collided: ")+Collided.getText()+AnsiString("\r\n");
            S += AnsiString("Obstructed: ")+Obstructed.getText()+AnsiString("\r\n");
            output_filename = "others-from-"+filename;
            StrWriteToFile(output_filename, S);
            append_("Others outputs saved in '"+output_filename+"'.", log_filename.c_str());
        }
        //if generation function fail
        else {
            //indicates that the motion program can't be generated
            append_("Depositioning program can't' be generated.", log_filename.c_str());
            //get and save the positions where the collision was detected
            TPairPositionAnglesList FPL;
            FMM.RPL.GetPositions(FPL);
            S = TActuator::GetPositionPPALabelsRow()+AnsiString("\r\n");
            S += IPL.getColumnText();
            output_filename = "FPL-from-"+filename;
            StrWriteToFile(output_filename, S);
            append_("Final position list (whear collission has been detected) saved in '"+output_filename+"'.", log_filename.c_str());

            //print the other outputs in the corresponding file
            S = AnsiString("generateDepositioningProgram: false\r\n");
            S += AnsiString("Collided: ")+Collided.getText()+AnsiString("\r\n");
            S += AnsiString("Obstructed: ")+Obstructed.getText()+AnsiString("\r\n");
            output_filename = "outputs-from-"+filename;
            StrWriteToFile(output_filename, S);
            append_("Others outputs saved in '"+output_filename+"'.", log_filename.c_str());
        }

        //DESTROY THE OBJECTS:

    } catch(Exception &E) {
        //indicates that has happened an exception
        //and show the message of the exception
        append_("ERROR: "+E.Message.str, log_filename.c_str());
        return 1;

    } catch(...) {
        //indicates that has happened an unknoledge exception
        append_("ERROR: unknowledge exception", log_filename.c_str());
        return 2;
    }

    //------------------------------------------------------------------

    //indicates that the program has been executed without error
    return 0;
}
