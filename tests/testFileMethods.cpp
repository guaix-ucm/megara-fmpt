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
#include "../src/vclemu.h"
#include "../src/Strings.h"
#include "../src/TextFile.h"
#include "../src/FileMethods.h"

#include <unistd.h> //rmdir

using namespace Strings;
using namespace Models;

CPPUNIT_TEST_SUITE_REGISTRATION(TestFileMethods);

//overide setUp(), init data, etc
void TestFileMethods::setUp() {
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

//    RP = new TRoboticPositioner();
}

//overide tearDown(), free allocated memory, etc
void TestFileMethods::tearDown() {
//    delete RP;
}

//---------------------------------------------------------------------------
//test methods for functions of the FileMethods:

void TestFileMethods::test_readInstanceFromDir_RP()
{
    //declare and initialize the properties for call the function
    TRoboticPositioner RP;
    string dir = "../../../data/Models/MEGARA_RP_Instance";

    //read an instance of a RP from a directory
    //using the function to test
    readInstanceFromDir(RP, dir);

    //--------------------------------------------------------------

    //load the set in a string
    string str_loaded;
    string filename = dir+"/Contour____.txt";
    strReadFromFile(str_loaded, filename);

    //regenerate the set in other string
    string str_regenerated = RP.getActuator()->getArm()->getContour____().getColumnText().str;

    //determine if the string is wrong assigned
    if(strTrim(str_loaded) != strTrim(str_regenerated))
        CPPUNIT_ASSERT(false);

    //--------------------------------------------------------------

    //load the set in a string
    str_loaded = "";
    filename = dir+"/Contour_.txt";
    strReadFromFile(str_loaded, filename);

    //regenerate the set in other string
    str_regenerated = RP.getActuator()->getBarrier()->getContour_().getColumnText().str;

    //determine if the string is wrong assigned
    if(strTrim(str_loaded) != strTrim(str_regenerated))
        CPPUNIT_ASSERT(false);

    //--------------------------------------------------------------

    //load the set in a string
    str_loaded = "";
    filename = dir+"/F1.txt";
    strReadFromFile(str_loaded, filename);

    //regenerate the set in other string
    str_regenerated = RP.getActuator()->getF().getTableText().str;

    //determine if the string is wrong assigned
    if(strTrim(str_loaded) != strTrim(str_regenerated))
        CPPUNIT_ASSERT(false);

    //--------------------------------------------------------------

    //load the set in a string
    str_loaded = "";
    filename = dir+"/F2.txt";
    strReadFromFile(str_loaded, filename);

    //regenerate the set in other string
    str_regenerated = RP.getActuator()->getArm()->getF().getTableText().str;

    //determine if the string is wrong assigned
    if(strTrim(str_loaded) != strTrim(str_regenerated))
        CPPUNIT_ASSERT(false);

    //--------------------------------------------------------------

    //load the set in a string
    str_loaded = "";
    filename = dir+"/Instance.txt";
    strReadFromFile(str_loaded, filename);

    //regenerate the set in other string
    str_regenerated = RP.getInstanceText().str;

    //determine if the string is wrong assigned
    if(strTrim(str_loaded) != strTrim(str_regenerated))
        CPPUNIT_ASSERT(false);

    //--------------------------------------------------------------

    CPPUNIT_ASSERT(true);
}

void TestFileMethods::test_writeInstanceToDir_RPL()
{
    //read the original instance of a RPL from a directory
    TRoboticPositionerList RPL;
    string dir = "../../../data/Models/MEGARA_FiberMOSModel_Instance";
    readInstanceFromDir(RPL, dir);

    //write the instance of the RPL in a temporal directory
    string dir_ = "../../../MEGARA_FiberMOSModel_Instance";
    ForceDirectories(dir_);
    writeInstanceToDir(dir_, RPL);

    //The new written instance will be the regenerated value to compare.

    rmdir(dir_.c_str());

/*
    //--------------------------------------------------------------

    //lee el valor original de la lista de orígenes de coordenadas de la lista de posicionadores
    string str_loaded;
    strReadFromFile(str_loaded, dir+"/RoboticPositionerOriginsTable.txt");

    //lee el valor temporal de la lista de orígenes de coordenadas de la lista de posicionadores
    string str_regenerated;
    strReadFromFile(str_regenerated, dir_+"/RoboticPositionerOriginsTable.txt");

    //determine if the string is wrong regenerated
    if(strTrim(str_loaded) != strTrim(str_regenerated))
        CPPUNIT_ASSERT(false);

    //--------------------------------------------------------------

    for(int i=0; i<RPL.getCount(); i++) {
        //point the indexed RP to facilitate their access
        TRoboticPositioner *RP = RPL[i];

        //construye el nombre del subdirectorio que contiene el valor original de la instancia del posicionador
        string subdir = dir+"/RoboticPositioner"+RP->getActuator()->getIdText().str;

        //construye el nombre del subdirectorio que contiene el valor regenerado de la instancia del posicionador
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

        //determine if the string is wrong regenerated
        if(strTrim(str_loaded) != strTrim(str_regenerated))
            CPPUNIT_ASSERT(false);

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        filename = subdir+"/Contour_.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        filename_ = subdir_+"/Contour_.txt";
        strReadFromFile(str_regenerated, filename_);

        //determine if the string is wrong regenerated
        if(strTrim(str_loaded) != strTrim(str_regenerated))
            CPPUNIT_ASSERT(false);

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        filename = subdir+"/F1.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        filename_ = subdir_+"/F1.txt";
        strReadFromFile(str_regenerated, filename_);

        //determine if the string is wrong regenerated
        if(strTrim(str_loaded) != strTrim(str_regenerated))
            CPPUNIT_ASSERT(false);

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        filename = subdir+"/F2.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        filename_ = subdir_+"/F2.txt";
        strReadFromFile(str_regenerated, filename_);

        //determine if the string is wrong regenerated
        if(strTrim(str_loaded) != strTrim(str_regenerated))
            CPPUNIT_ASSERT(false);

        //--------------------------------------------------------------

        //load the orignal set in a string
        str_loaded = "";
        filename = subdir+"/Instance.txt";
        strReadFromFile(str_loaded, filename);

        //load the regenerated set in a string
        str_regenerated = "";
        filename_ = subdir_+"/Instance.txt";
        strReadFromFile(str_regenerated, filename_);

        //determine if the string is wrong regenerated
        if(strTrim(str_loaded) != strTrim(str_regenerated))
            CPPUNIT_ASSERT(false);
    }

    //--------------------------------------------------------------
*/
    CPPUNIT_ASSERT(true);
}
void TestFileMethods::test_readInstanceFromDir_RPL()
{
    //declare and initialize the properties for call the function
    TRoboticPositionerList RPL;
    string dir = "../../../data/Models/MEGARA_FiberMOSModel_Instance";

    //read an instance of a RP from a directory
    //using the function to test
    readInstanceFromDir(RPL, dir);

    //--------------------------------------------------------------

    //lee y asigna la lista de orígenes de coordenadas de la lista de posicionadores
    string str_loaded;
    strReadFromFile(str_loaded, dir+"/RoboticPositionerOriginsTable.txt");

    string str_regenerated = TActuator::getOriginsLabelsRow().str+"\r\n"+RPL.getOriginsTableText().str;

    //determine if the string is wrong assigned
    if(strTrim(str_loaded) != strTrim(str_regenerated))
        CPPUNIT_ASSERT(false);

    //--------------------------------------------------------------

    for(int i=0; i<RPL.getCount(); i++) {
        //point the indexed RP to facilitate their access
        TRoboticPositioner *RP = RPL[i];

        //construye el nombre del subdirectorio que contiene la instancia del posicionador
        string subdir = dir+"/RoboticPositioner"+RP->getActuator()->getIdText().str;

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        string filename = subdir+"/Contour____.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getActuator()->getArm()->getContour____().getColumnText().str;

        //determine if the string is wrong assigned
        if(strTrim(str_loaded) != strTrim(str_regenerated))
            CPPUNIT_ASSERT(false);

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        filename = subdir+"/Contour_.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getActuator()->getBarrier()->getContour_().getColumnText().str;

        //determine if the string is wrong assigned
        if(strTrim(str_loaded) != strTrim(str_regenerated))
            CPPUNIT_ASSERT(false);

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        filename = subdir+"/F1.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getActuator()->getF().getTableText().str;

        //determine if the string is wrong assigned
        if(strTrim(str_loaded) != strTrim(str_regenerated))
            CPPUNIT_ASSERT(false);

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        filename = subdir+"/F2.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getActuator()->getArm()->getF().getTableText().str;

        //determine if the string is wrong assigned
        if(strTrim(str_loaded) != strTrim(str_regenerated))
            CPPUNIT_ASSERT(false);

        //--------------------------------------------------------------

        //load the set in a string
        str_loaded = "";
        filename = subdir+"/Instance.txt";
        strReadFromFile(str_loaded, filename);

        //regenerate the set in other string
        str_regenerated = RP->getInstanceText().str;

        //determine if the string is wrong assigned
        if(strTrim(str_loaded) != strTrim(str_regenerated))
            CPPUNIT_ASSERT(false);
    }

    //--------------------------------------------------------------

    CPPUNIT_ASSERT(true);
}
