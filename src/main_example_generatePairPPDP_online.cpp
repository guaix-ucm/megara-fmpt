// Copyright (c) 2014-2016 Isaac Morales Durán. All rights reserved.
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

        //indicates that the program is running
        append("FMPT SAA 3.0.0 is running...", log_filename.c_str());

        //built the paths where search the Fiber MOS Model
        string dir_FMM1 = DATADIR;
        dir_FMM1 += "/Models/MEGARA_FiberMOSModel_Instance";
        string dir_FMM2 = getCurrentDir()+"/../data/Models/MEGARA_FiberMOSModel_Instance";

        std::cout << dir_FMM1 << endl;
        std::cout << dir_FMM2 << endl;

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
        //EXECUTE AN ONLINE GENERATION EXAMPLE OF A PAIR (PP, DP):

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

        output_dir = "example_generatePairPPDP_online";
        ForceDirectories(AnsiString(output_dir));

        //print the tittle
        append("\r\nOnline generation example of a pair (PP, DP) for CB"+inttostr(Bid)+":", log_filename.c_str());
        append("=====================================================", log_filename.c_str());

        //build the filename of reference
        filename = "example-"+inttostr(Bid)+".txt";

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
        output_filename = output_dir+"/OPL-from-"+filename;
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

        //Note that not collided and not obstructed should be part of the stop condition,
        //but they are discarted in the function of generation online.

        //indicates that the generating example has finished
        append("\r\nFinished online generation example of a pair (PP, DP) for CB"+inttostr(Bid)+"!", log_filename.c_str());
        append("PairPPDPvalid: "+BoolToStr(PairPPDPvalid, true).str, log_filename.c_str());
        //append("Collided: "+Collided.gettText().str, log_filename.c_str());
        //append("Obstructed: "+Obstructed.gettText().str, log_filename.c_str());

    }
    catch(Exception &E) {
        //indicates that has happened an exception
        //and show the message of the exception
        append("ERROR generating example: "+E.Message.str, log_filename.c_str());
        return 1;
    }
    catch(...) {
        //indicates that has happened an unknoledge exception
        append("ERROR generating example: unknowledge exception", log_filename.c_str());
        return 2;
    }

    //-----------------------------------------------------------------------

    //indicates that the program has been executed without error
    return 0;
}
