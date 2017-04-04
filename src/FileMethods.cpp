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
//File: FileMethods.cpp
//Content: file functions for read and write instances of models
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "FileMethods.h"
#include "TextFile.h" //StrReadFromFile, StrWriteToFile

using namespace Strings;

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR ROBOTIC POSITIONERS:

//read and assign the content of the files of a RP Instance
void readAndAssign(TRoboticPositioner& RP, const string& dir)
{
    string str;
    strReadFromFile(str, dir+"/Contour____.txt");
    RP.getActuator()->getArm()->setContour____ColumnText(str);
    strReadFromFile(str, dir+"/Contour_.txt");
    RP.getActuator()->getBarrier()->setContour_ColumnText(str);
    strReadFromFile(str, dir+"/Instance.txt");
    RP.setInstanceText(str);
    strReadFromFile(str, dir+"/F1.txt");
    RP.getActuator()->setFTableText(str);
    strReadFromFile(str, dir+"/F2.txt");
    RP.getActuator()->getArm()->setFTableText(str);

    //WARNING: set the instance of a RP, produces the reset of F1 and F2
    //according the SB1 and SB2 respectively. Reason why F1 and F2
    //must be set after.
}

//read an instance of a RP
//from a directory
void readInstanceFromDir(TRoboticPositioner& RP, const string& dir)
{
    try {
        locale l;
        if(l.name() != "C")
            throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

        //build a tampon variable
        TRoboticPositioner t_RP;
        //read and assign the content of the files
        readAndAssign(t_RP, dir);
        //clone the tampon variable
        RP.clone(&t_RP);
    }
    catch(ECantComplete& E) {
        E.Message.Insert(1, "reading instance of RP"+RP.getActuator()->getIdText().str+": ");
        throw;
    }
    catch(Exception& E) {
        throw EImproperFileLoadedValue("reading instance of RP"+RP.getActuator()->getIdText().str+": " + E.Message.str);
    }
}

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR EXCLUSION AREA LIST:

//write the instance of a EA list
//in a directory
void writeInstanceToDir(const string& dir, const TExclusionAreaList& EAL)
{
    try {
        locale l;
        if(l.name() != "C")
            throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

        string str;
        string path;
        string aux;

        //por cada área de exclusión de la lista
        for(int i=0; i<EAL.getCount(); i++) {
            //apunta el área de exclusión indicado para facilitar su acceso
            TExclusionArea *EA = EAL[i];

            //construye el nombre del subdirectorio que debe contener la instancia del área de exclusión
            string subdir = dir+"/ExclusionArea"+EA->getIdText().str;
            //fuerza la construcción del directorio
            mkpath(subdir);

            //archiva el contorno de la barrera
            str = EA->getContour_Text().str;
            path = subdir+"/Contour_.txt";
            if(isfile(path))
                strReadFromFileWithComments(aux, path);
            if(str != aux)
                strWriteToFile(path, str);

            //archiva la instancia del área de exclusión
            str = EA->getInstanceText().str;
            path = subdir+"/Instance.txt";
            if(isfile(path))
                strReadFromFileWithComments(aux, path);
            if(str != aux)
                strWriteToFile(path, str);
        }

        //archiva la lista de orígenes de coordenadas de la lista de área de exclusiónes
        str = EAL.getOriginsTableText().str;
        path = dir+"/ExclusionAreaOriginsTable.txt";
        if(isfile(path))
            strReadFromFileWithComments(aux, path);
        if(str != aux)
            strWriteToFile(path, str);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "writing instance of EAL to dir '"+dir+"': ");
        throw;
    }
}
//read an instance of a EA list
//from a directory
void readInstanceFromDir(TExclusionAreaList& EAL, const string& dir)
{
    try {
        locale l;
        if(l.name() != "C")
            throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

        //contruye una lista tampón
        TExclusionAreaList t_EAL;

        //lee y asigna la tabla de orígenes de coordenadas
        string str;
        strReadFromFile(str, dir+"/ExclusionAreaOriginsTable.txt");
        t_EAL.setOriginsTableText(str);

        //por cada área de exclusión de la lista
        for(int i=0; i<t_EAL.getCount(); i++) {
            //apunta el área de exclusión indicada para facilitar su acceso
            TExclusionArea *EA = t_EAL[i];

            //construye el nombre del subdirectorio que contiene la instancia del área de exclusión
            string subdir = dir+"/ExclusionArea"+EA->getIdText().str;

            //lee y asigna el contorno de la barrera
            strReadFromFile(str, subdir+"/Contour_.txt");
            EA->Barrier.setContour_ColumnText(str);

            //lee y asigna la instancia del área de exclusión
            strReadFromFile(str, subdir+"/Instance.txt");
            EA->setInstanceText(str);
        }

        //lee y asigna la instancia de la lista de EAs
        //                strReadFromFile(S, dir+"/Instance.txt");
        //              t_EAL.InstanceText = S;

        //clona la lista tampón
        EAL.Clone(&t_EAL);
        //asimila los parámetros de dimensionamiento
        EAL.assimilate();

        //desturye las EAs de la lista tampón
        //                t_EAL.Destroy();

        //Las EAs de la lista tampón no deben
        //ser destruidas porque se trata de una lista de punteros
        //y no de una lista basada en punteros.
    }
    catch(ECantComplete& E) {
        E.Message.Insert(1, "reading instance of EAL: ");
        throw;
    }
    catch(Exception& E) {
        throw EImproperFileLoadedValue("reading instance of EAL: " + E.Message.str);
    }
}

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR ROBOTIC POSITIONER LISTS:

//write the instance of a RP list
//in a directory
void writeInstanceToDir(const string& dir, const TRoboticPositionerList& RPL)
{
    try {
        locale l;
        if(l.name() != "C")
            throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

        string str;
        string path;
        string aux;

        //write the RP map
        str = RPL.getInstanceMapText().str;
        path = dir+"/InstanceMap.txt";
        if(isfile(path))
            strReadFromFile(aux, path);
        if(str != aux)
            strWriteToFile(path, str);

/*        //write the instance properties of the RPL
        str = RPL.getInstanceText().str;
        path = dir+"/Instance.txt";
        if(isfile(path))
            strReadFromFile(aux, path);
        if(str != aux)
            strWriteToFile(path, str);
*/
        //por cada posicionador de la lista
        for(int i=0; i<RPL.getCount(); i++) {
            //apunta el posicionador indicado para facilitar su acceso
            TRoboticPositioner *RP = RPL[i];

            //construye el nombre del subdirectorio que debe contener la instancia del posicionador
            string subdir = dir+"/RoboticPositioner"+RP->getActuator()->getIdText().str;
            //fuerza la construcción del directorio
            mkpath(subdir);

            //archiva el contorno del brazo
            str = RP->getContour____Text().str;
            path = subdir+"/Contour____.txt";
            if(isfile(path))
                strReadFromFile(aux, path);
            if(str != aux)
                strWriteToFile(path, str);

            //archiva el contorno de la barrera
            str = RP->getContour_Text().str;
            path = subdir+"/Contour_.txt";
            if(isfile(path))
                strReadFromFile(aux, path);
            if(str != aux)
                strWriteToFile(path, str);

            //archiva la función de compresión del rotor 1
            str = RP->getF1Text().str;
            path = subdir+"/F1.txt";
            if(isfile(path))
                strReadFromFile(aux, path);
            if(str != aux)
                strWriteToFile(path, str);

            //archiva la función de compresión del rotor 2
            str = RP->getF2Text().str;
            path = subdir+"/F2.txt";
            if(isfile(path))
                strReadFromFile(aux, path);
            if(str != aux)
                strWriteToFile(path, str);

            //archiva la instancia del posicionador
            str = RP->getInstanceText().str;
            path = subdir+"/Instance.txt";
            if(isfile(path))
                strReadFromFileWithComments(aux, path);
            if(str != aux)
                strWriteToFile(path, str);
        }

        //archiva la lista de orígenes de coordenadas de la lista de posicionadores
        str = RPL.getOriginsTableText().str;
        path = dir+"/RoboticPositionerOriginsTable.txt";
        if(isfile(path))
            strReadFromFileWithComments(aux, path);
        if(str != aux)
            strWriteToFile(path, str);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "writing instance of RPL: ");
        throw;
    }
}
//read an instance of a RP list
//from a directory
void readInstanceFromDir(TRoboticPositionerList& RPL, const string& dir,
                         const TExclusionAreaList& EAL)
{
    try {
        locale l;
        if(l.name() != "C")
            throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

        //contruye una lista tampón
        TRoboticPositionerList t_RPL;

        //lee y asigna la lista de orígenes de coordenadas de la lista de posicionadores
        string str;
        strReadFromFile(str, dir+"/RoboticPositionerOriginsTable.txt");
        t_RPL.setOriginsTableText(str);

        //por cada posicionador de la lista
        for(int i=0; i<t_RPL.getCount(); i++) {
            //apunta el posicionador indicado para facilitar su acceso
            TRoboticPositioner *RP = t_RPL[i];

            //construye el nombre del subdirectorio que contiene la instancia del posicionador
            string subdir = dir+"/RoboticPositioner"+RP->getActuator()->getIdText().str;

            //lee y asigna el contorno del brazo
            strReadFromFile(str, subdir+AnsiString("/Contour____.txt").str);
            RP->getActuator()->getArm()->setContour____ColumnText(str);

            //lee y asigna el contorno de la barrera
            strReadFromFile(str, subdir+"/Contour_.txt");
            RP->getActuator()->getBarrier()->setContour_ColumnText(str);

            //lee y asigna la instancia del posicionador
            strReadFromFile(str, subdir+"/Instance.txt");
            RP->setInstanceText(str);

            //WARNING: set the isntance produces the reset of F1 and F2
            //according the SB1 and SB2 respectively. Reason why F1 and F2
            //must be setted below.

            //lee y asigna la función de compresión del rotor 1
            strReadFromFile(str, subdir+"/F1.txt");
            RP->getActuator()->setFTableText(str);

            //lee y asigna la función de compresión del rotor 2
            strReadFromFile(str, subdir+"/F2.txt");
            RP->getActuator()->getArm()->setFTableText(str);
        }

/*        //lee y asigna la instancia de la lista de posicionadores
        strReadFromFile(str, dir+AnsiString("/Instance.txt").str);
        t_RPL.setInstanceText(str);
*/
        //El mapa de RPs puede ser cargado de un archivo, o puede ser
        //determinado tras la asimilación mediante el método:
        //      void TRoboticPositioneer::GenerateMap(void);

        //asimila los parámetros de dimensionamiento
        t_RPL.assimilate(EAL);

        //clona la lista tampón
        RPL.Clone(&t_RPL);

        //desturye los RPs de la lista tampón
        t_RPL.Destroy();

        //RPs of thetampon list must be destroyed here, because
        //the RPL is a list of pointers.
    }
    catch(ECantComplete& E) {
        E.Message.Insert(1, "reading instance of RPL: ");
        throw;
    }
    catch(Exception& E) {
        throw EImproperFileLoadedValue("reading instance of RPL: " + E.Message.str);
    }
}

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR Fiber MOS Models:

//write the instance of a Fiber MOS Model
//in a directory
void writeInstanceToDir(const string& dir, const TFiberMOSModel& FMM)
{
    try {
        locale l;
        if(l.name() != "C")
            throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

        //fuerza la creación del directorio
        ForceDirectories(dir);

        string str;
        string path;
        string aux;

        //escribe las demás propiedades de la instancia
        //en el archivo 'Instance.txt'
        str = FMM.getInstanceText().str;
        path = dir+"/Instance.txt";
        if(isfile(path))
            strReadFromFileWithComments(aux, path);
        if(str != aux)
            strWriteToFile(path, str);

        //escribe las instancias de las listas de objetos en el directorio
        writeInstanceToDir(dir, FMM.EAL);
        writeInstanceToDir(dir, FMM.RPL);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "writing instance of FMM: ");
        throw;
    }
}
//read an instance of a Fiber MOS Model
//from a directory
void readInstanceFromDir(TFiberMOSModel& FMM, const string& dir)
{
    try {
        locale l;
        if(l.name() != "C")
            throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

        //contruye una variable tampón
        TFiberMOSModel t_FMM;

        //lee las instancias de las listas de objetos del directorio
        readInstanceFromDir(t_FMM.EAL, dir);
        readInstanceFromDir(t_FMM.RPL, dir, t_FMM.EAL);

        //lee las demás propiedades de la instancia
        //del archivo 'Instance.txt'
        string str;
        strReadFromFile(str, dir+"/Instance.txt");
        t_FMM.setInstanceText(str);

        //Note that here is not necessary assimilate the instance
        //because the reading of each list producess the assimilation.

        //clona la variable tampón
        FMM.Clone(&t_FMM);
        //asimila las propiedades
        FMM.assimilate();

        //ADVERTENCIA: la configuración del FMM debe ser asimilada,
        //ya que algunas de sus propiedades dependen de su posición
        //en la memoria. Sean los punteros de Adjacents.
    }
    catch(ECantComplete& E) {
        E.Message.Insert(1, "reading instance of FMM: ");
        throw;
    }
    catch(Exception& E) {
        throw EImproperFileLoadedValue("reading instance of FMM: " + E.Message.str);
    }
}

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR Fiber Connection Models:

//write the instance of a Fiber Connection Model
//in a directory
void writeInstanceToDir(const string& dir, const TFiberConnectionModel& FCM)
{
    try {
        locale l;
        if(l.name() != "C")
            throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

        string str;
        string path;
        string aux;

        //fuerza la construcción del directorio
        mkpath(dir);
        //escribe la tabla de conexiones de la pseudoslit en el archivo correspondiente
        str = FCM.getConnectionsText().str;
        path = dir+"/Connections.txt";
        if(isfile(path))
            strReadFromFile(aux, path);
        if(str != aux)
            strWriteToFile(path, str);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "writing instance of FCM: ");
        throw;
    }
}
//read an instance of a Fiber Connection Model
//from a directory
void readInstanceFromDir(TFiberConnectionModel& FCM, const string& dir)
{
    try {
        locale l;
        if(l.name() != "C")
            throw EImproperCall("improper locale information (call to setlocale(LC_ALL, \"C\") for set or retrieve locale)");

        //lee la tabla de conexiones de la pseudoslit en una cadena de texto
        string str;
        strReadFromFile(str, dir+"/Connections.txt");

        //asigna la cadena de texto a una variable tampón
        TFiberConnectionModel t_FCM;
        t_FCM.setConnectionsText(str);

        //clona la variable tampón
        FCM.Clone(t_FCM);
    }
    catch(ECantComplete& E) {
        E.Message.Insert(1, "reading instance of FCM: ");
        throw;
    }
    catch(Exception& E) {
        throw EImproperFileLoadedValue("reading instance of FCM: " + E.Message.str);
    }
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

