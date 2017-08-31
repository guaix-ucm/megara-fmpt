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
/// @file main_example_generatePairPPDP_online.cpp
/// @brief program for generate online a pair (PP, DP)
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

void readInput(vector<int>& RPids, vector<double>& p_1s, vector<double>& p___3s,
               const string& input_path)
{
    //load the content of the input file in a string
    string str;
    strReadFromFileWithComments(str, input_path);

    int RPid;
    double p_1;
    double p___3;

    unsigned int i = 0;

    //get the values (RPid, p_1, p___3) from the string
    do {
        i = (unsigned int)str.find("inserted in fmpt", i);
        if(i != (unsigned int)string::npos) {
            strTravelLabel("inserted in fmpt", str, i);
            strTravelSeparatorsIfAny(str, i);
            strTravelLabel("rp=", str, i);
            strReadInt(RPid, str, i);
            strTravelLabel(",", str, i);
            strTravelSeparatorsIfAny(str, i);
            strTravelLabel("r1=", str, i);
            strReadFloat(p_1, str, i);
            strTravelLabel(",", str, i);
            strTravelSeparatorsIfAny(str, i);
            strTravelLabel("r2=", str, i);
            strReadFloat(p___3, str, i);

            RPids.push_back(RPid);
            p_1s.push_back(p_1);
            p___3s.push_back(p___3);
        }
    } while(i != (unsigned int)string::npos);

    //complete with the RPs that aren't for work
    for(int i=93; i<=100; i++) {
        RPids.push_back(i);
        p_1s.push_back(0);
        p___3s.push_back(0);
    }
}

//main function
int main(int argc, char *argv[])
{
    try {
        //-------------------------------------------------------
        //Get inputs (RPids_actual, p_1s_actual, p___3s_actual) and RPids_disabled_actual.

        string input_path = "/usr/local/share/megara-fmpt/DataForTests/RP_actual_positions.txt";
        //string input_path = "/home/user/MEGARA/megara-fmpt/data/DataForTests/RP_actual_positions.txt";

        //get (RPids_actual, p_1s_actual, p___3s_actual)
        vector<int> RPids_actual;               //the correspondig identifiers of the RPs.
        vector<double> p_1s_actual;             //the rotor 1 starting positions of all RPs of the FMM.
        vector<double> p___3s_actual;           //the rotor 2 starting positions of all RPs of the FMM.
        readInput(RPids_actual, p_1s_actual, p___3s_actual, input_path);

        //get RPids_disabled_actual
        vector<int> RPids_disabled_actual;      //the identifiers of the RPs of the FMM to be disabled.
        RPids_disabled_actual.push_back(64);    //for example here is disabled RP64

        //In MCS you get this inputs directly from the real Fiber MOS.
        //When you get from the MCS the input data for RPs that aren't for work,
        //maybe you can get the their (RPid_actual, p_1_actual, p___3_actual) directly from the real Fiber MOS.

        std::cout << "Got inputs (RPids_actual, p_1s_actual, p___3s_actual) and RPids_disabled_actual." << std::endl;

        //-------------------------------------------------------
        //Load the FMM Instance.

        input_path = "/usr/local/share/megara-fmpt/Models/MEGARA_FiberMOSModel_Instance";

        //In MCS you load the FMM Instance from:
        //  input_path = "/work/megara/share/megara-fmpt/Models/MEGARA_FiberMOSModel_Instance"

        TFiberMOSModel FMM;     //the Fiber MOS Model
        try {
            readInstanceFromDir(FMM, input_path);
        } catch(Exception& E) {
            E.Message.Insert(1, "reading FMM Instance: ");
            throw;
        }

        //The FMM Instance shall correspont to the status of the real Fiber MOS,
        //although could have some diferences:
        //  - have enabled the RPs recentlly disabled in the real Fiber MOS
        //  - have the (p_1, p___3) in diferent positions that the real Fiber MOS.

        std::cout << "FMM Instance loaded from: '" << input_path << "'" << std::endl;

        //-------------------------------------------------------
        //Get SPL (Starting Position List) from (RPids_actual, p_1s_actual, p___3s_actual).

        //In parking program:
        //  SPL: satrting position list
        //  FPL: final position list
        //  CPL: collided position list
        //In pair (PP, DP):
        //  IPL: initial position list
        //  OPL: observing position list
        //  CPL: collided position list

        TPairPositionAnglesList SPL;
        for(int i=0; i<FMM.RPL.getCount(); i++)
        {
            TPairPositionAngles *PPA = new TPairPositionAngles(RPids_actual[i]);
            PPA->p_1 = p_1s_actual[i];
            PPA->p___3 = p___3s_actual[i];
            SPL.Add(PPA);
        }

        std::cout << "SPL (Starting Position List) got from (RPids_actual, p_1s_actual, p___3s_actual)." << std::endl;

        //-------------------------------------------------------
        //Get the OPL from file type FMOSA.

        input_path = "/usr/local/share/megara-fmpt/Samples/megara-cb0.meg";
        //input_path = "/home/user/MEGARA/megara-fmpt/data/Samples/megara-cb0.meg";

        //load the FMOSA from the file input_path
        string str;
        OutputsPairPPDP outputs;
        unsigned int Bid; //the block identifier
        try {
            strReadFromFileWithComments(str, input_path);
            outputs.FMOSA.setTableText(Bid, str);
        } catch(Exception& E) {
            E.Message.Insert(1, "reading FMOSA file: ");
            throw;
        }
        std::cout << "FMOSA loaded from file '"+input_path+"'." << std::endl;

        //In the MCS you get the OPL also from the file type FMOSA, but it is possible it was already in memory.

        //get the allocations from the FMOSA
        TAllocationList AL(&FMM.RPL);
        outputs.FMOSA.getAllocations(AL);
        std::cout << "Allocations got from the FMOSA to MPG." << std::endl;

        //get the Observing Position List
        TPairPositionAnglesList OPL;
        AL.getFinalPositionList(OPL, SPL);

        //For the point P3 of the RPs go to the more closer stable point to the allocated point (X, Y),
        //this method perform a simmulation of the FMM, determining the angular positions of the rotors (in steps).
        //This method restablish the status of the FMM recovering their initial positions.

        std::cout << "Got Bid from the FMOSA." << std::endl;

        //-------------------------------------------------------
        //Get (p_1s_observing, p___3s_observing) from the OPL

        vector<double> p_1s_observing;
        vector<double> p___3s_observing;

        for(int i=0; i<OPL.getCount(); i++) {
            TPairPositionAngles *PPA = OPL.GetPointer(i);
            p_1s_observing.push_back(PPA->p_1);
            p___3s_observing.push_back(PPA->p___3);
        }

        //-------------------------------------------------------
        //Set the SPL in the FMM.

        FMM.RPL.setPositions(SPL);

        std::cout << "SPL setted in the FMM." << std::endl;

        //-------------------------------------------------------
        //Call the function generatePairPPDP_online:

        bool pairPPDPSuitable;      //true: if the generated pair (PP, DP) is suitable to be executed.

        //A pair (PP, DP) is suitable to be executed when avoid collissions,
        //and there aren't collided or obstructed RPs (in the initial status).

        std::cout << "Calling function generatePairPPDP_online(outputs, FMM, p_1s_observing, p___3s_observing, RPids_disabled_actual, Bid)..." << std::endl;

        //Generate the pair (PP, DP) online.
        pairPPDPSuitable = generatePairPPDP_online(outputs, FMM, p_1s_observing, p___3s_observing, RPids_disabled_actual, Bid);

        std::cout << "Returned from function generatePairPPDP_online." << std::endl;
        std::cout << "PairPPDPSuitable: " << BoolToStr(pairPPDPSuitable, true).str << std::endl;

        //-------------------------------------------------------
        //Use the output data for program the real Fiber MOS:

        //Definitions:
        //  outputs.PP:                   the motion program containing the instructions
        //  outputs.PP.getcount():        number of gestures to be exxecuted
        //  outputs.PP[i].getCount():     number of message-of-instructions in the gesture i
        //  outputs.PP[i][j].Instruction: the instructions contained in the message (i, j)
        //  Instruction.getName():              name of the instruction
        //  Instruction.Args.getCount():        argument's number of the instruction
        //  Instruction.Args[k]:                the argument k of the instruction

        //Note that Instruction.Args constains coordinates p_1 and p___3 in doubles.
        //For get r1 and r2, you need invert the sign of p___3.

        TMotionProgram *PP = &outputs.PP;

        std::cout << std::endl;
        std::cout << "Accessing to the PP (Positioning Program):" << std::endl;

        for(int i=0; i<PP->getCount(); i++) {
            TMessageList *ML = PP->GetPointer(i);

            std::cout << StrIndent("Message list ").str << inttostr(i) << ":" << std::endl;

            for(int j=0; j<ML->getCount(); j++) {
                TMessageInstruction *MI = ML->GetPointer(j);
                int RPid = MI->getId();
                TInstruction *Instruction = &MI->Instruction;

                std::cout << StrIndent(StrIndent("Message instruction ")).str << inttostr(j) << ":";
                std::cout << " rpid: " << RPid << ";";

                if(Instruction->getName().str == "M1") {
                    std::cout << " r1: " << inttostr((int)Instruction->Args[0]) << ";";
                    std::cout << " r2: " << "the actual" << ";";
                } else if(Instruction->getName().str == "M2") {
                    std::cout << " r1: " << "the actual" << ";";
                    std::cout << " r2: " << inttostr((int)-Instruction->Args[0]) << ";"; //here you invert the sign of p___3 for get r2
                } else if(Instruction->getName() == "MM") {
                    std::cout << " r1: " << inttostr((int)Instruction->Args[0]) << ";";
                    std::cout << " r2: " << inttostr((int)-Instruction->Args[1]) << ";"; //here you invert the sign of p___3 for get r2
                }

                std::cout << std::endl;

            }
        }

        //-------------------------------------------------------
        //Use the output data for get the outputs file:

        string outputs_str_MEG;
        outputs.getTextMEG(outputs_str_MEG);
        string outputs_str_JSON;
        outputs.getTextJSON(outputs_str_JSON);

        std::cout << std::endl;
        std::cout << "Printing the pair (PP, DP):";
        std::cout << " (outputs_str_meg, outputs_str_json) got from structure outputs." << std::endl;

        //Now you can use outputs_str_meg or outputs_str_json.

    }
    catch(Exception &E) {
        //indicates that has happened an exception
        //and show the message of the exception
        std::cout << "ERROR: " << E.Message.str << std::endl;
        return 1;
    }
    catch(...) {
        //indicates that has happened an unknown exception
        std::cout << "ERROR: unknown exception" << std::endl;
        return 2;
    }

    //indicates that the program has been executed without error
    return 0;
}

