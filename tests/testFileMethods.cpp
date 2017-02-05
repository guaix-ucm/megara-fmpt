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
//Archivo: testFileMethods.cpp
//Contenido: test para FileMethods
//Última actualización: 12/02/2015
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------


#include "testFileMethods.h"
#include "../src/VCLemu.h"
#include "../src/Strings.h"
#include "../src/TextFile.h"
#include "../src/FileMethods.h"
//#include "cppunit_assert_emulator.h" //Qt only.

using namespace Strings;
using namespace Models;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestFileMethods);

//overide setUp(), init data, etc
void TestFileMethods::setUp() {
    //------------------------------------------------------------------
    //CONFIGURATES THE SYSTEM:

    //REMEMBER: exceptions in runtime can be due to that
    //the system is not configurated.

    //configurates the decimal separator
    setlocale(LC_NUMERIC, "C");
}

//overide tearDown(), free allocated memory, etc
void TestFileMethods::tearDown() {
}

//---------------------------------------------------------------------------
//test methods for functions of the FileMethods:

void TestFileMethods::test_readInstanceFromDir_RP()
{
    //read an original instance of a RP from a directory
    //using the function to test
    TRoboticPositioner RP;
    string dir = "../../../data/Models/MEGARA_RP_Instance";
    //string dir = "../megarafmpt/data/Models/MEGARA_RP_Instance"; //Qt only
    readInstanceFromDir(RP, dir);

    //--------------------------------------------------------------

    //load the set in a string
    string str_loaded;
    string filename = dir+"/Contour____.txt";
    strReadFromFile(str_loaded, filename);

    //regenerate the set in other string
    string str_regenerated = RP.getActuator()->getArm()->getContour____().getColumnText().str;
    strClearComments(str_regenerated);

    //determine if the values dont match
    if(strTrim(str_loaded) != strTrim(str_regenerated)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //--------------------------------------------------------------

    //load the set in a string
    str_loaded = "";
    filename = dir+"/Contour_.txt";
    strReadFromFile(str_loaded, filename);

    //regenerate the set in other string
    str_regenerated = RP.getActuator()->getBarrier()->getContour_().getColumnText().str;
    strClearComments(str_regenerated);

    //determine if the values dont match
    if(strTrim(str_loaded) != strTrim(str_regenerated)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //--------------------------------------------------------------

    //load the set in a string
    str_loaded = "";
    filename = dir+"/F1.txt";
    strReadFromFile(str_loaded, filename);

    //regenerate the set in other string
    str_regenerated = RP.getActuator()->getF().getTableText().str;
    strClearComments(str_regenerated);

    //determine if the values dont match
    if(strTrim(str_loaded) != strTrim(str_regenerated)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //--------------------------------------------------------------

    //load the set in a string
    str_loaded = "";
    filename = dir+"/F2.txt";
    strReadFromFile(str_loaded, filename);

    //regenerate the set in other string
    str_regenerated = RP.getActuator()->getArm()->getF().getTableText().str;
    strClearComments(str_regenerated);

    //determine if the values dont match
    if(strTrim(str_loaded) != strTrim(str_regenerated)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //--------------------------------------------------------------

    //load the set in a string
    str_loaded = "";
    filename = dir+"/Instance.txt";
    strReadFromFile(str_loaded, filename);

    //regenerate the set in other string
    str_regenerated = RP.getInstanceText().str;
    strClearComments(str_regenerated);

    //determine if the values dont match
    if(strTrim(str_loaded) != strTrim(str_regenerated)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //--------------------------------------------------------------

    CPPUNIT_ASSERT(true);
}

void TestFileMethods::test_writeInstanceToDir_EAL()
{
    //read an original instance of a EAL from a directory
    TExclusionAreaList EAL;
    string dir = "../../../data/Models/MEGARA_FiberMOSModel_Instance";
    //string dir = "../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"; //Qt only
    readInstanceFromDir(EAL, dir);

    //write the instance of the EAL in a temporal directory
    //with the function to test
    string dir_ = "../data/MEGARA_FiberMOSModel_Instance_temporal";
    //string dir_ = "../megarafmpt/MEGARA_FiberMOSModel_Instance_temporal"; //Qt only
    ForceDirectories(dir_);
    writeInstanceToDir(dir_, EAL);

    //The new written instance will be the regenerated value to compare.

    //--------------------------------------------------------------

    //load the EA origins table from the original file
    string str_loaded;
    strReadFromFile(str_loaded, dir+"/ExclusionAreaOriginsTable.txt");

    //load the EA origins table from the temporal file
    string str_regenerated;
    strReadFromFile(str_regenerated, dir_+"/ExclusionAreaOriginsTable.txt");
    strClearComments(str_regenerated);

    //determine if the values dont match
    if(strTrim(str_loaded) != strTrim(str_regenerated)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //--------------------------------------------------------------

    //for each EA of the EAL
    for(int i=0; i<EAL.getCount(); i++) {
        //point the indexed EA to facilitate their access
        TExclusionArea *EA = EAL[i];

        //build the subdirectory path containing the original instance of the EA
        string subdir = dir+"/ExclusionArea"+EA->getIdText().str;

        //build the subdirectory path containing the regenerated instance of the EA
        string subdir_ = dir_+"/ExclusionArea"+EA->getIdText().str;

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        string filename = subdir+"/Contour_.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        string filename_ = subdir_+"/Contour_.txt";
        strReadFromFile(str_regenerated, filename_);
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        filename = subdir+"/Instance.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        filename_ = subdir_+"/Instance.txt";
        strReadFromFile(str_regenerated, filename_);
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //--------------------------------------------------------------

    //remove the temporal directory and all their content
    rmpath(dir_);

    CPPUNIT_ASSERT(true);
}
void TestFileMethods::test_readInstanceFromDir_EAL()
{
    //read an original instance of a EAL from a directory
    //using the function to test
    TExclusionAreaList EAL;
    string dir = "../../../data/Models/MEGARA_FiberMOSModel_Instance";
    //string dir = "../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"; //Qt only
    readInstanceFromDir(EAL, dir);

    //--------------------------------------------------------------

    //load the EA origins table from the original file
    string str_loaded;
    strReadFromFile(str_loaded, dir+"/ExclusionAreaOriginsTable.txt");

    //regenerate the EA origins table
    string str_regenerated = TExclusionArea::getOriginsLabelsRow().str+"\r\n"+EAL.getOriginsTableText().str;
    strClearComments(str_regenerated);

    //determine if the values dont match
    if(strTrim(str_loaded) != strTrim(str_regenerated)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //--------------------------------------------------------------

    //for each EA of the EAL
    for(int i=0; i<EAL.getCount(); i++) {
        //point the indexed EA to facilitate their access
        TExclusionArea *EA = EAL[i];

        //build the sundirectory path containing the instance of the EA
        string subdir = dir+"/ExclusionArea"+EA->getIdText().str;

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        string filename = subdir+"/Contour_.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = EA->Barrier.getContour_().getColumnText().str;
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        filename = subdir+"/Instance.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = EA->getInstanceText().str;
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //--------------------------------------------------------------

    CPPUNIT_ASSERT(true);
}

void TestFileMethods::test_writeInstanceToDir_RPL()
{
    //read an original instance of a RPL from a directory
    TRoboticPositionerList RPL;
    string dir = "../../../data/Models/MEGARA_FiberMOSModel_Instance";
    //string dir = "../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"; //Qt only
    TExclusionAreaList EAL;
    readInstanceFromDir(RPL, dir, EAL);

    //write the instance of the RPL in a temporal directory
    //with te function to test
    string dir_ = "../data/MEGARA_FiberMOSModel_Instance_temporal";
    //string dir_ = "../megarafmpt/MEGARA_FiberMOSModel_Instance_temporal"; //Qt only
    ForceDirectories(dir_);
    writeInstanceToDir(dir_, RPL);

    //The new written instance will be the regenerated value to compare.

    //--------------------------------------------------------------

    //read the original value of the RP origins table
    string str_loaded;
    strReadFromFile(str_loaded, dir+"/RoboticPositionerOriginsTable.txt");

    //read the temporal value of the RP origins table
    string str_regenerated;
    strReadFromFile(str_regenerated, dir_+"/RoboticPositionerOriginsTable.txt");
    strClearComments(str_regenerated);

    //determine if the values dont match
    if(strTrim(str_loaded) != strTrim(str_regenerated)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //--------------------------------------------------------------

    //for each RP of the RPL
    for(int i=0; i<RPL.getCount(); i++) {
        //point the indexed RP to facilitate their access
        TRoboticPositioner *RP = RPL[i];

        //build the subdirectory path containing the instance of the original RP
        string subdir = dir+"/RoboticPositioner"+RP->getActuator()->getIdText().str;

        //build the subdirectory path containing the instance of the regenerated RP
        string subdir_ = dir_+"/RoboticPositioner"+RP->getActuator()->getIdText().str;

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        string filename = subdir+"/Contour____.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        string filename_ = subdir_+"/Contour____.txt";
        strReadFromFile(str_regenerated, filename_);
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        filename = subdir+"/Contour_.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        filename_ = subdir_+"/Contour_.txt";
        strReadFromFile(str_regenerated, filename_);
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        filename = subdir+"/F1.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        filename_ = subdir_+"/F1.txt";
        strReadFromFile(str_regenerated, filename_);
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        filename = subdir+"/F2.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        filename_ = subdir_+"/F2.txt";
        strReadFromFile(str_regenerated, filename_);
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        filename = subdir+"/Instance.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        filename_ = subdir_+"/Instance.txt";
        strReadFromFile(str_regenerated, filename_);
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //--------------------------------------------------------------

    //remove the temporal directory and all their content
    rmpath(dir_);

    CPPUNIT_ASSERT(true);
}
void TestFileMethods::test_readInstanceFromDir_RPL()
{
    //read an original instance of a RPL from a directory
    //with the function to test
    TRoboticPositionerList RPL;
    string dir = "../../../data/Models/MEGARA_FiberMOSModel_Instance";
    //string dir = "../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"; //Qt only
    TExclusionAreaList EAL;
    readInstanceFromDir(RPL, dir, EAL);

    //--------------------------------------------------------------

    //load the RP origins table
    string str_loaded;
    strReadFromFile(str_loaded, dir+"/RoboticPositionerOriginsTable.txt");

    //regenerate the RP origins table
    string str_regenerated = TActuator::getOriginsLabelsRow().str+"\r\n"+RPL.getOriginsTableText().str;
    strClearComments(str_regenerated);

    //determine if the values dont match
    if(strTrim(str_loaded) != strTrim(str_regenerated)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //--------------------------------------------------------------

    //for each RP of the RPL
    for(int i=0; i<RPL.getCount(); i++) {
        //point the indexed RP to facilitate their access
        TRoboticPositioner *RP = RPL[i];

        //build the subdirectory path containing the instance of RP
        string subdir = dir+"/RoboticPositioner"+RP->getActuator()->getIdText().str;

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        string filename = subdir+"/Contour____.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getActuator()->getArm()->getContour____().getColumnText().str;
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        filename = subdir+"/Contour_.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getActuator()->getBarrier()->getContour_().getColumnText().str;
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        filename = subdir+"/F1.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getActuator()->getF().getTableText().str;
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        filename = subdir+"/F2.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getActuator()->getArm()->getF().getTableText().str;
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        filename = subdir+"/Instance.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getInstanceText().str;
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //--------------------------------------------------------------

    CPPUNIT_ASSERT(true);
}

void TestFileMethods::test_writeInstanceToDir_FMM()
{
    //read an original instance of a FMM from a directory
    TFiberMOSModel FMM;
    string dir = "../../../data/Models/MEGARA_FiberMOSModel_Instance";
    //string dir = "../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"; //Qt only
    readInstanceFromDir(FMM, dir);

    //write the instance of the FMM in a temporal directory
    //using the function to test
    string dir_ = "../data/MEGARA_FiberMOSModel_Instance_temporal";
    //string dir_ = "../megarafmpt/MEGARA_FiberMOSModel_Instance_temporal"; //Qt only
    ForceDirectories(dir_);
    writeInstanceToDir(dir_, FMM);

    //The new written instance will be the regenerated value to compare.

    //--------------------------------------------------------------

    //load the the RP origins table from the original file
    string str_loaded;
    strReadFromFile(str_loaded, dir+"/RoboticPositionerOriginsTable.txt");

    //load the the RP origins table from the regenerated file
    string str_regenerated;
    strReadFromFile(str_regenerated, dir_+"/RoboticPositionerOriginsTable.txt");
    strClearComments(str_regenerated);

    //determine if the values dont match
    if(strTrim(str_loaded) != strTrim(str_regenerated)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //--------------------------------------------------------------

    //for each RP of the FMM
    for(int i=0; i<FMM.RPL.getCount(); i++) {
        //point the indexed RP to facilitate their access
        TRoboticPositioner *RP = FMM.RPL[i];

        //build the subdirectory path containing the original instance of the RP
        string subdir = dir+"/RoboticPositioner"+RP->getActuator()->getIdText().str;

        //build the subdirectory path containing the regenerated instance of the RP
        string subdir_ = dir_+"/RoboticPositioner"+RP->getActuator()->getIdText().str;

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        string filename = subdir+"/Contour____.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        string filename_ = subdir_+"/Contour____.txt";
        strReadFromFile(str_regenerated, filename_);
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        filename = subdir+"/Contour_.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        filename_ = subdir_+"/Contour_.txt";
        strReadFromFile(str_regenerated, filename_);
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        filename = subdir+"/F1.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        filename_ = subdir_+"/F1.txt";
        strReadFromFile(str_regenerated, filename_);
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        filename = subdir+"/F2.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        filename_ = subdir_+"/F2.txt";
        strReadFromFile(str_regenerated, filename_);
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        filename = subdir+"/Instance.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        filename_ = subdir_+"/Instance.txt";
        strReadFromFile(str_regenerated, filename_);
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //--------------------------------------------------------------

    //load the EA origins table from the original file
    strReadFromFile(str_loaded, dir+"/ExclusionAreaOriginsTable.txt");

    //load the EA origins table from the regenerated file
    strReadFromFile(str_regenerated, dir_+"/ExclusionAreaOriginsTable.txt");
    strClearComments(str_regenerated);

    //determine if the values dont match
    if(strTrim(str_loaded) != strTrim(str_regenerated)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //--------------------------------------------------------------

    //for each EA of the FMM
    for(int i=0; i<FMM.EAL.getCount(); i++) {
        //point the indexed EA to facilitate their access
        TExclusionArea *EA = FMM.EAL[i];

        //build the subdirectory path containing the original instance of the EA
        string subdir = dir+"/ExclusionArea"+EA->getIdText().str;

        //build the subdirectory path containing the regenerated instance of the EA
        string subdir_ = dir_+"/ExclusionArea"+EA->getIdText().str;

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        string filename = subdir+"/Contour_.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        string filename_ = subdir_+"/Contour_.txt";
        strReadFromFile(str_regenerated, filename_);
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        filename = subdir+"/Instance.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        filename_ = subdir_+"/Instance.txt";
        strReadFromFile(str_regenerated, filename_);
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //--------------------------------------------------------------

    //remove the temporal directory and all their content
    rmpath(dir_);

    CPPUNIT_ASSERT(true);
}
void TestFileMethods::test_readInstanceFromDir_FMM()
{
    //read an original instance of a FMM from a directory
    //using the function to test
    TFiberMOSModel FMM;
    string dir = "../../../data/Models/MEGARA_FiberMOSModel_Instance";
    //string dir = "../megarafmpt/data/Models/MEGARA_FiberMOSModel_Instance"; //Qt only
    readInstanceFromDir(FMM, dir);

    //--------------------------------------------------------------

    //load the RP origins table
    string str_loaded;
    strReadFromFile(str_loaded, dir+"/RoboticPositionerOriginsTable.txt");

    //regenerate the RP origins table
    string str_regenerated = TActuator::getOriginsLabelsRow().str+"\r\n"+FMM.RPL.getOriginsTableText().str;
    strClearComments(str_regenerated);

    //determine if the values dont match
    if(strTrim(str_loaded) != strTrim(str_regenerated)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //--------------------------------------------------------------

    //for each RP of the FMM
    for(int i=0; i<FMM.RPL.getCount(); i++) {
        //point the indexed RP to facilitate their access
        TRoboticPositioner *RP = FMM.RPL[i];

        //build the subdirectory path containing the instance of RP
        string subdir = dir+"/RoboticPositioner"+RP->getActuator()->getIdText().str;

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        string filename = subdir+"/Contour____.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getActuator()->getArm()->getContour____().getColumnText().str;
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        filename = subdir+"/Contour_.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getActuator()->getBarrier()->getContour_().getColumnText().str;
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        filename = subdir+"/F1.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getActuator()->getF().getTableText().str;
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        filename = subdir+"/F2.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getActuator()->getArm()->getF().getTableText().str;
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        filename = subdir+"/Instance.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getInstanceText().str;
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //--------------------------------------------------------------

    //load the EA origins table from the original file
    strReadFromFile(str_loaded, dir+"/ExclusionAreaOriginsTable.txt");

    //regenerate the EA origins table
    str_regenerated = TExclusionArea::getOriginsLabelsRow().str+"\r\n"+FMM.EAL.getOriginsTableText().str;
    strClearComments(str_regenerated);

    //determine if the values dont match
    if(strTrim(str_loaded) != strTrim(str_regenerated)) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //--------------------------------------------------------------

    //for each EA of the FMM
    for(int i=0; i<FMM.EAL.getCount(); i++) {
        //point the indexed EA to facilitate their access
        TExclusionArea *EA = FMM.EAL[i];

        //build the subdirectory path containing the original instance of the EA
        string subdir = dir+"/ExclusionArea"+EA->getIdText().str;

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        string filename = subdir+"/Contour_.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = EA->Barrier.getContour_().getColumnText().str;
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        filename = subdir+"/Instance.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = EA->getInstanceText().str;
        strClearComments(str_regenerated);

        //determine if the values dont match
        if(strTrim(str_loaded) != strTrim(str_regenerated)) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //--------------------------------------------------------------

    CPPUNIT_ASSERT(true);
}
