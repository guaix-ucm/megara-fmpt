#include "FileMethods.h"
#include "MotionProgramGenerator.h"
#include "Strings.h"
#include "TextFile.h"

#include <locale.h> //setlocale, LC_NUMERIC
#include <cstring> //strlen
#include <iostream> //std

//using namespace Mathematics;
using namespace Strings;
using namespace Models;
using namespace Positioning;

//print a text line in the standard output
void append(const AnsiString& S)
{
    std::cout << S.c_str() << endl;
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

    try {
        //la llamada a fmpt_saa debe contener un argumento
        if(argc!=2 || strlen(argv[1])<=0 || (strlen(argv[1])==1 && argv[1][0]=='/'))
            throw EImproperArgument("fmpt_saa <relative_path>\r\n\t<relative_path>: relative path to file containing a PP list.");

        //indicates that the program is running
        append(AnsiString("program fmpt_saa is running..."));

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
        AnsiString S;
        try {
            S = AnsiString(DATADIR)+AnsiString("/Models/MEGARA_FiberMOSModel_Instance");
            ReadInstanceFromDir(FMM, S);
        } catch(...) {
            try {
                S = GetCurrentDir()+AnsiString("/../data/Models/MEGARA_FiberMOSModel_Instance");
                ReadInstanceFromDir(FMM, S);
            } catch(Exception& E) {
                append(AnsiString("ERROR: ")+E.Message);
                throw E;
            }
        }
        append(AnsiString("Fiber MOS Model instance loaded from '")+S+AnsiString("'."));

        //load the PP table from a file
        AnsiString FileName;// = AnsiString(argv[1]);
        try {
            FileName = AnsiString(DATADIR);
            if(argv[1][0] != '/')
                FileName += AnsiString("/");
            FileName += AnsiString(argv[1]); //Example: argv[1]: "PPPList/20141124102242.txt"; or  argv[1]: "/PPPList/20141124102242.txt".
            StrReadFromFile(S, FileName);
        } catch(...) {
            try {
                FileName = GetCurrentDir()+AnsiString("/../data");
                if(argv[1][0] != '/')
                    FileName += AnsiString("/");
                FileName += AnsiString(argv[1]); //Example: argv[1]: "PPPList/20141124102242.txt"; or  argv[1]: "/PPPList/20141124102242.txt".
                StrReadFromFile(S, FileName);
            } catch(Exception& E) {
                append(AnsiString("ERROR: ")+E.Message);
                throw E;
            }
        }
        append(AnsiString("PP table loaded from '")+FileName+AnsiString("'."));

        //assign the PP table to the MPG
        MPG.setTargetPointsText(S);
        append("Target points allocated.");

        //MAKE THE OPERATIONS:

        //move the RPs to the more closer stable point to the TPs
        MPG.MoveToTargetP3();
        append("RPs moved to target points.");

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
        StrWriteToFile("InitialPositionList.txt", S);
        append("Initial position list saved in 'InitialPositionList.txt'.");

        //Other whay to obtain the initial position table directly in text format:
        //  FMM.RPL.getPositionsPAPTableText()

        //segregates the operative outsiders RPs
        TRoboticPositionerList Outsiders;
        FMM.RPL.segregateOperativeOutsiders(Outsiders);

        //generates a de positioning program for the operative RPs at insecurity positions
        //and determines the RPs in collision status or obstructed at insecurity positions
        append("Generating depositioning program...");
        TRoboticPositionerList Collided;
        TRoboticPositionerList Obstructed;
        TMotionProgram DP;
        MPG.generateDepositioningProgram(Collided, Obstructed, DP, Outsiders);
        append("Depositioning program generated.");

        //translates the depositioning program to the format of the interface MCS-FMPT
        append("Teanslating the depositiong program.");
        FMM.RPL.TranslateMotionProgram(S, 1, IPL, DP);
        StrWriteToFile("DepositioningProgram.txt", S);
        append("Depositiong program translated.");
        append("Depositioning program saved in 'DepositioningProgram.txt'.");

        //DESTROY THE OBJECTS:

    } catch(Exception &E) {
        //show the message of the exception
        append(AnsiString("ERROR: ")+E.Message);
        return 1;

    } catch(...) {
        //indicates that has haoppened an unknoledgeexception
        append(AnsiString("ERROR: unknowledge exception"));
        return 2;
    }

    //------------------------------------------------------------------

    return 0;
}
