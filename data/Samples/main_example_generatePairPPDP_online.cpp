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
//Content: principal program fmpt_example
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

//###########################################################################
//This file contains an example to ilustrate the use of the FMPT library
//for generate a pair (PP, DP) using the online function.
//If you want compile and execute this example perform the following actions:
//
//    1. Decompress the release 'megara-fmpt-3.3.0.tar.gz (or xz)
//
//    2. Move this file to 'megara-fmpt-3.3.0/src'.
//
//    3. Edit the file 'megara-fmpt-3.0.0/src/Makefile.am', and change:
//           bin_PROGRAMS = fmpt_saa
//           fmpt_saa_SOURCES = main.cpp
//           fmpt_saa_LDADD = libfmtp.la
//           fmpt_saa_CPPFLAGS = $(AM_CPPFLAGS)
//       By:
//           bin_PROGRAMS = fmpt_example_generatePairPPDP_online
//           fmpt_example_generatePairPPDP_online_SOURCES = main_example_generatePairPPDP_online.cpp
//           fmpt_example_generatePairPPDP_online_LDADD = libfmtp.la
//           fmpt_example_generatePairPPDP_online_CPPFLAGS = $(AM_CPPFLAGS)
//
//    4. Install and execute the example in a separated directory:
//           $ make build
//           $ cd build
//           $ ../megara-fmpt-3.3.0/configure (or the corresponding path)
//           $ sudo make install
//
//    Then the executable will be written in '/usr/local/bin' (or similar).
//    If you have installed previously the FMPT SAA, the directory will look:
//        fmpt_saa
//        fmpt_example_generatePairPPDP_online
//
//    If you don't have installed the FMPT SAA previously, maybe you need
//    execute $ ldcondig for assimilate the FMPT library in the system.
//
//    Now you may execute the example with the comand:
//        $ fmpt_example_generatePairPPDP_online.
//    The output files will be written in the directory where you execute it.
//###########################################################################

#include "FileMethods.h"
#include "MotionProgramGenerator.h"
#include "FMOSATable.h"
#include "PositionerCenter.h"
#include "Strings.h"
#include "TextFile.h"
#include "Geometry.h" //distanceSegmentPoint
#include "Outputs.h" //Outputs

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

    string log_filename; //the log filename

    try {
        //initalize the log file
        log_filename = "fmpt_testGeneratePairPPDP_online.log";
        char mode[] = "w";
        TTextFile TF(log_filename.c_str(), mode);
        TF.Close();
    }
    catch(Exception &E) {
        //indicates that has happened an exception
        //and show the message of the exception
        cout << "ERROR generating example: "+E.Message.str << endl;
        return 1;
    }
    catch(...) {
        //indicates that has happened an unknoledge exception
        cout << "ERROR generating example: unknowledge exception" << endl;
        return 2;
    }

    try {
        //-------------------------------------------------------------------
        //CONFIGURATE THE SYSTEM:

        //REMEMBER: exceptions in runtime can be due to that
        //the system is not configurated.

        //configurates the decimal separator
        setlocale(LC_NUMERIC, "C");

        //-----------------------------------------------------------------------
        //LOAD THE FIBER MOS MODEL INSTANCE FROM A DIR:

        //indicates that the program is running
        append("FMPT example generatePairPPDP_online is running...", log_filename.c_str());

        //built the paths where search the Fiber MOS Model
        string dir_FMM1 = DATADIR;
        dir_FMM1 += "/Models/MEGARA_FiberMOSModel_Instance";
        string dir_FMM2 = getCurrentDir()+"/../data/Models/MEGARA_FiberMOSModel_Instance";

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
        //DECLARE AND BUILD ALL VARIABLES:

        //make the folder to put the outputss
        string output_dir = "example_generatePairPPDP_online";
        ForceDirectories(AnsiString(output_dir));

        //print the tittle
        unsigned int Bid = 0; //block identifier
        append("\r\nOnline generation example of a pair (PP, DP) for a CB"+inttostr(Bid)+":", log_filename.c_str());
        append("=======================================================", log_filename.c_str());

        //build the filename of reference (including the Bid)
        string filename = "example-"+inttostr(Bid)+".txt";

        //the filename will be used to attach the outputs with the input.

        //###################################################################
        //There are two ways to get a set of allocations:
        // 1. Load the allocations from a FMOSA file and get them from the MPG.
        // 2. Randomize the allocations without collisions.

        //-------------------------------------------------------------------
        //WAY 1: GETTING A SET OF ALLOCATIONS LOADING THEM FROM A FMOSA FILE:

        //load the Outputs structure from a file
        string input_path = "/usr/local/share/megara-fmpt/Samples/megara-cb0.txt";
        Outputs outputs;
        string str; //string to be written to file each time
        try {
            strReadFromFile(str, input_path);
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

        //save the allocation table
        str = TAllocation::GetIdPPLabelsRow().str;
        str += "\r\n"+MPG.getColumnText().str;
        string output_filename = output_dir+"/AL-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Allocation list saved in '"+output_filename+"'.", log_filename.c_str());

        //FOR UNCOMMENT THE GETTING OF THE OUTPUTS FILE GO TO LABEL:
        //  "//get the output file".

        //###################################################################

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
        vector<double> p_1s, p___3s;
        for(int i=0; i<FMM.RPL.getCount(); i++) {
            TRoboticPositioner *RP = FMM.RPL[i];
            p_1s.push_back(RP->getActuator()->getp_1());
            p___3s.push_back(RP->getActuator()->getArm()->getp___3());
        }
        append("Input parameters extracted from the FMM.", log_filename.c_str());

        //move the RPs to their origin positions
        FMM.RPL.moveToOrigins();
        append("RPs moved to origins.", log_filename.c_str());

        //Move the FMM to the origins is convenient because this function is
        //for test the generating function, but it is not really necessary.

        //determine the RPs to be disabled
        vector<int> Ids;
        //TBD

        //When you reuse the Ids, you need to do:
        //  Ids.clear();

        //The RPs that shall be disabled depend of the status of the RPs
        //of the real Fiber MOS.

        //call the function to test
        TMotionProgram PP, DP;
        append("Calling function generatePairPPDP_online...", log_filename.c_str());
        append("----------------------------------------------------------------", log_filename.c_str());
        append("PairPPDPvalid = generatePairPPDP_online(PP, DP,", log_filename.c_str());
        append("                                        FMM, p_1s, p___3s, Ids);", log_filename.c_str());
        bool PairPPDPvalid = generatePairPPDP_online(PP, DP,
                                                     FMM, p_1s, p___3s, Ids);
        append("----------------------------------------------------------------", log_filename.c_str());
        append("Returned from function generatePairPPDP_online.", log_filename.c_str());

        //When you reuse the MPs, you need to do:
        //  PP.clear();
        //  DP.clear();
        //before call the generation function.

        //WARNING: function generatePairPPDP_online not return the lists Collided and Obstructed.
        //So it is possible that the function return true, but there are RPs collided or obstructed.
        //in these case, these RPs will not be included in the pair (PP, DP).
        //Inputs and outputs parameters of the function generatePairPPDP_online,
        //was stablished by the programmer of the MCS, which was warned about this circunstance.

        //determine the list of RPs included in the pair (PP, DP)
        TRoboticPositionerList Included;
        getRPsIncludedInMPs(Included, PP, DP, &FMM);
        TVector<int> Excluded;
        for(int i=0; i<FMM.RPL.getCount(); i++) {
            TRoboticPositioner *RP = FMM.RPL[i];
            int j = Included.Search(RP);
            if(j >= Included.getCount())
                Excluded.Add(RP->getActuator()->getId());
        }

        //THE DISABLED RPs WILL STAY DISABLED IN THE FMM.

        //restore de enabling status of all RPs
        for(unsigned int i=0; i<Ids.size(); i++) {
            int Id = Ids[i];
            int j = FMM.RPL.searchId(Id);
            if(j >= FMM.RPL.getCount())
                throw EImpossibleError("lateral effect");
            FMM.RPL[j]->Disabled = false;
        }

        //Here are re-enabled the RPs for ilustrate the modo to do this,
        //but in MCS, RPs not should be re-enabled. Instead of that,
        //the new status of the RPs should be indicated in the FMM instance,
        //(writing Disabled = true), because the instance will be re-loaded
        //in case of fault of power supply.

        //---------------------------------------------------------------
        //SAVE THE OUTPUTS AND PRINT THE CORRESPONDING MESSAGES:

        //if generation function was successfully generated
        if(PairPPDPvalid) {
            //access to each instruction of the PP
            for(int i=0; i<PP.getCount(); i++) {
                TMessageList *ML = PP.GetPointer(i);
                for(int j=0; j<ML->getCount(); j++) {
                    //point the indicated message instruction
                    TMessageInstruction *MI = ML->GetPointer(j);

                    //get the information from the message instruction
                    int Id = MI->getId();
                    TInstruction I = MI->Instruction;
                    string name = I.getName().str;
                    if(name == "M1") {
                        double p_1 = I.Args[0];
                    } else if(name == "M2") {
                        double p___3 = I.Args[0];
                    } else if(name == "MM") {
                        double p_1 = I.Args[0];
                        double p___3 = I.Args[1];
                    } else
                        throw EImpossibleError("lateral effect");

                    //here is possible use the properties (Id, p_1, p___3) of the message instruction (i, j)
                }
            }

            //access to each instruction of the DP
            for(int i=0; i<DP.getCount(); i++) {
                TMessageList *ML = DP.GetPointer(i);
                for(int j=0; j<ML->getCount(); j++) {
                    //point the indicated message instruction
                    TMessageInstruction *MI = ML->GetPointer(j);

                    //get the information from the message instruction
                    int Id = MI->getId();
                    TInstruction I = MI->Instruction;
                    string name = I.getName().str;
                    if(name == "M1") {
                        double p_1 = I.Args[0];
                    } else if(name == "M2") {
                        double p___3 = I.Args[0];
                    } else if(name == "MM") {
                        double p_1 = I.Args[0];
                        double p___3 = I.Args[1];
                    } else
                        throw EImpossibleError("lateral effect");

                    //here is possible use the properties (Id, p_1, p___3) of the message instruction (i, j)
                }
            }

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

            //Other whay to obtain the initial position table directly in text format:
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

            //###############################################################
            //WAY 1: GETTING A SET OF ALLOCATIONS LOADING THEM FROM A FMOSA FILE:

            //If you use the way 2, you must comment this instructions,
            //because the outputs file only can be obtained when the allocations
            //has been loaded from a FMOSA file, because it contains
            //the FMOSA table in addition to the pair(PP, DP).

            //get the outputs file
            outputs.PP = PP;
            outputs.DP = DP;
            outputs.getText(str, Bid, OPL, IPL);
            output_filename = output_dir+"/outputs-from-"+filename;
            strWriteToFile(output_filename, str);
            append("Pair (PP, DP) saved in '"+output_filename+"'.", log_filename.c_str());
            //###############################################################
        }
        else {
            //Given that here the generated pair (PP, DP) is invalid,
            //all operative outsider RPs which aren't obstructed, should be:
            //- in the first position where the collision was detected.
            //During the test of DP or the test of PP.

            //print the result of generation of the PP
            append("Generated pair (PP, DP) is not valid, because either PP nor DP is invalid.", log_filename.c_str());
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
        //str += "\r\nCollided: "+Collided.getText().str;                         //not returned by the function online
        //str += "\r\nObstructed: "+Obstructed.getText().str;                     //not returned by the function online
        str += "\r\nExcluded: "+Excluded.getText().str;
        if(DP.thereIsSomeComment1())
            str += "\r\nDP comments:\r\n"+DP.getComment1sColumnText();
        if(PP.thereIsSomeComment1())
            str += "\r\nPP comments:\r\n"+PP.getComment1sColumnText();
        output_filename = output_dir+"/other-outputs-from-"+filename;
        strWriteToFile(output_filename, str);
        append("Other outputs saved in '"+output_filename+"'.", log_filename.c_str());

        //END OF SAVE THE OUTPUTS

        //Note that not collided and not obstructed should be part of the stop condition,
        //but they are discarted in the function of generation online.

        //indicates that the generating example has finished
        append("\r\nFinished online generation example of a pair (PP, DP) for CB"+inttostr(Bid)+"!", log_filename.c_str());
        append("PairPPDPvalid: "+BoolToStr(PairPPDPvalid, true).str, log_filename.c_str());
        //append("Collided: "+Collided.gettText().str, log_filename.c_str());     //not returned by the function online
        //append("Obstructed: "+Obstructed.gettText().str, log_filename.c_str()); //not returned by the function online
        append("Excluded: "+Excluded.getText().str, log_filename.c_str());
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
