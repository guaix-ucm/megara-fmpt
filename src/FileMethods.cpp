// Copyright (c) 2012-2016 Isaac Morales Durán. All rights reserved.
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

//#include <iostream>

using namespace Strings;

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR ROBOTIC POSITIONERS:

//read and assign the content of the files of a RP instance
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
        //build a tampon variable
        TRoboticPositioner t_RP;
        //read and assign the content of the files
        readAndAssign(t_RP, dir);
        //clone the tampon variable
        RP.clone(&t_RP);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading instance of RP"+RP.getActuator()->getIdText().str+": ");
        throw;
    }
}

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR EXCLUSION AREA LIST:

//write the instance of a EA list
//in a directory
void writeInstanceToDir(const string& dir, const TExclusionAreaList& EAL)
{
    try {
        string str;

        //por cada área de exclusión de la lista
        for(int i=0; i<EAL.getCount(); i++) {
            //apunta el área de exclusión indicado para facilitar su acceso
            TExclusionArea *EA = EAL[i];

            //construye el nombre del subdirectorio que debe contener la instancia del área de exclusión
            string subdir = dir+"/ExclusionArea"+EA->getIdText().str;
            //fuerza la construcción del directorio
            mkpath(subdir);

            //archiva el contorno de la barrera
            str = "#A list of segments (Pa, Pb) and arcs (Pa, Pb, Pc, R) for describe";
            str += "\r\n#the template of EA.Barrier.Contour_, in S1 (in mm):";
            str += "\r\n\r\n"+EA->Barrier.getContour_().getColumnText().str;
            strWriteToFile(subdir+"/Contour_.txt", str);

            //archiva la instancia del área de exclusión
            str = "#Instance properties of the EA (Exclusion Area):";
            str += "\r\n\r\n"+EA->getInstanceText().str;
            strWriteToFile(subdir+"/Instance.txt", str);
        }

        //archiva la lista de orígenes de coordenadas de la lista de área de exclusiónes
        str = "#A table for indicate the position and orientation of each EA (Exclusion Area):";
        str += "\r\n#    Id: identifier of the EA (a nonnegative integer number)";
        str += "\r\n#    x0: abscissa of the point P0 of the EA in S0 (in mm)";
        str += "\r\n#    y0: ordinate of the point P0 of the EA in S0 (in mm)";
        str += "\r\n#    thetaO1: orientation of S1 in S0 (in rad)";
        str += "\r\n\r\n"+TExclusionArea::getOriginsLabelsRow().str+"\r\n"+EAL.getOriginsTableText().str;
        strWriteToFile(dir+"/ExclusionAreaOriginsTable.txt", str);
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
        //contruye una lista tampón
        TExclusionAreaList t_EAL;

        string str;

        //lee y asigna la tabla de orígenes de coordenadas
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
    catch(Exception& E) {
        E.Message.Insert(1, "reading instance of EAL: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR ROBOTIC POSITIONER LISTS:

//write the instance of a RP list
//in a directory
void writeInstanceToDir(const string& dir, const TRoboticPositionerList& RPL)
{
    try {
        string str;

        //write the RP map
        str = "#A matrix to transform each point (x, y) given in Cartesian coordinates in S0,";
        str += "\r\n#in a list of identifiers of RPs {Id} in whose observing domain could be the point (x, y):";
        str += "\r\n\r\n"+RPL.getInstanceMapText().str;
        strWriteToFile(dir+"/InstanceMap.txt", str);

        //write the instance properties of the RPL
        str = "#Instance properties of the FMM (Fiber MOS Model):";
        str += "\r\n\r\n"+RPL.getInstanceText().str;
        strWriteToFile(dir+"/Instance.txt", str);

        //por cada posicionador de la lista
        for(int i=0; i<RPL.getCount(); i++) {
            //apunta el posicionador indicado para facilitar su acceso
            TRoboticPositioner *RP = RPL[i];

            //construye el nombre del subdirectorio que debe contener la instancia del posicionador
            string subdir = dir+"/RoboticPositioner"+RP->getActuator()->getIdText().str;
            //fuerza la construcción del directorio
            mkpath(subdir);

            //archiva el contorno del brazo
            str = "#A list of segments (Pa, Pb) and arcs (Pa, Pb, Pc) for describe";
            str += "\r\n#the template of RP.Actuator.Arm.Contour____ in S4 (in mm):";
            str += "\r\n\r\n"+RP->getActuator()->getArm()->getContour____().getColumnText().str;
            strWriteToFile(subdir+"/Contour____.txt", str);

            //archiva el contorno de la barrera
            str = "#A list of segments (Pa, Pb) and arcs (Pa, Pb, Pc, R) for describe";
            str += "\r\n#the template of the RP.Barrier.Contour_, in S1 (in mm):";
            str += "\r\n\r\n"+RP->getActuator()->getBarrier()->getContour_().getColumnText().str;
            strWriteToFile(subdir+"/Contour_.txt", str);

            //archiva la función de compresión del rotor 1
            str = "#The compression-function of the quantifier of rot 1 of the RP in step/rad.";
            str += "\r\n#Must be defined almost in the rot 1 domain [theta_1min, theta_1max]:";
            str += "\r\n#    theta_1: position of rot 1 (in rad)";
            str += "\r\n#    p_1: position of rot 1 (in step)";
            str += "\r\n\r\n"+RP->getActuator()->getF().getTableText().str;
            strWriteToFile(subdir+"/F1.txt", str);

            //archiva la función de compresión del rotor 2
            str = "#The compression-function of the quantifier of rot 2 of the RP in step/rad.";
            str += "\r\n#Must be defined in the rot 2 domain [theta___3min, theta___3max]:";
            str += "\r\n#    theta___3: position of rot 2 (in rad)";
            str += "\r\n#    p___3: position of rot 2 (in step)";
            str += "\r\n\r\n"+RP->getActuator()->getArm()->getF().getTableText().str;
            strWriteToFile(subdir+"/F2.txt", str);

            //archiva la instancia del posicionador
            str = "#Instance properties of the RP (Robotic Positioner):";
            str += "\r\n\r\n"+RP->getInstanceText().str;
            strWriteToFile(subdir+"/Instance.txt", str);
        }

        //archiva la lista de orígenes de coordenadas de la lista de posicionadores
        str = "#A table for indicate the position and orientation of each RP (Robotic Positioner):";
        str += "\r\n#    Id: identifier of the RP (a nonnegative integer number)";
        str += "\r\n#    x0: abscissa of the point P0 of the RP in S0 (in mm)";
        str += "\r\n#    y0: ordinate of the point P0 of the RP in S0 (in mm)";
        str += "\r\n#    thetaO1: orientation of S1 in S0 (in rad)";
        str += "\r\n\r\n"+TActuator::getOriginsLabelsRow().str+"\r\n"+RPL.getOriginsTableText().str;
        strWriteToFile(dir+"/RoboticPositionerOriginsTable.txt", str);
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

        //lee y asigna la instancia de la lista de posicionadores
        strReadFromFile(str, dir+AnsiString("/Instance.txt").str);
        t_RPL.setInstanceText(str);

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
    catch(Exception& E) {
        E.Message.Insert(1, "reading instance of RPL: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR Fiber MOS Models:

//write the instance of a Fiber MOS Model
//in a directory
void writeInstanceToDir(const string& dir, const TFiberMOSModel& FMM)
{
    try {
        //escribe las demás propiedades de la instancia
        //en el archivo 'Instance.txt'
        strWriteToFile(dir+"/Instance.txt", FMM.getInstanceText().str);

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
    catch(Exception& E) {
        E.Message.Insert(1, "reading instance of FMM: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR Fiber Connection Models:

//write the instance of a Fiber Connection Model
//in a directory
void writeInstanceToDir(const string& dir, const TFiberConnectionModel& FCM)
{
    try {
        //fuerza la construcción del directorio
        mkpath(dir);
        //escribe la tabla de conexiones de la pseudoslit en el archivo correspondiente
        strWriteToFile(dir+"/Connections.txt", FCM.getConnectionsText().str);
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
        //lee la tabla de conexiones de la pseudoslit en una cadena de texto
        string str;
        strReadFromFile(str, dir+"/Connections.txt");

        //asigna la cadena de texto a una variable tampón
        TFiberConnectionModel t_FCM;
        t_FCM.setConnectionsText(str);

        //clona la variable tampón
        FCM.Clone(t_FCM);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading instance of FCM: ");
        throw;
    }
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

