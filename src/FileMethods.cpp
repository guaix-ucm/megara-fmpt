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
//Archivo: FileMethods.cpp
//Contenido: métodos de archivo para lectura y escritura de modelos
//Última actualización: 15/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "FileMethods.h"
#include "TextFile.h" //StrReadFromFile, StrWriteToFile

#include <iostream> //std::cout

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

    } catch(...) {
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
        //write the RP map
        strWriteToFile(dir+"/InstanceMap.txt", RPL.getInstanceMapText().str);

        //write the additional properties of the RPL instance
        strWriteToFile(dir+"/Instance.txt", RPL.getInstanceText().str);

        //por cada posicionador de la lista
        for(int i=0; i<RPL.getCount(); i++) {
            //apunta el posicionador indicado para facilitar su acceso
            TRoboticPositioner *RP = RPL[i];

            //construye el nombre del subdirectorio que debe contener la instancia del posicionador
            string subdir = dir+"/RoboticPositioner"+RP->getActuator()->getIdText().str;
            //fuerza la construcción del directorio
            mkpath(subdir);

            //archiva el contorno del brazo
            strWriteToFile(subdir+"/Contour____.txt", RP->getActuator()->getArm()->getContour____().getColumnText().str);

            //archiva el contorno de la barrera
            strWriteToFile(subdir+"/Contour_.txt", RP->getActuator()->getBarrier()->getContour_().getColumnText().str);

            //archiva la función de compresión del rotor 1
            strWriteToFile(subdir+"/F1.txt", RP->getActuator()->getF().getTableText().str);

            //archiva la función de compresión del rotor 2
            strWriteToFile(subdir+"/F2.txt", RP->getActuator()->getArm()->getF().getTableText().str);

            //archiva la instancia del posicionador
            strWriteToFile(subdir+"/Instance.txt", RP->getInstanceText().str);
        }

        //archiva la lista de orígenes de coordenadas de la lista de posicionadores
        strWriteToFile(dir+"/RoboticPositionerOriginsTable.txt", TActuator::getOriginsLabelsRow().str+"\r\n"+RPL.getOriginsTableText().str);

    } catch(...) {
        throw;
    }
}
//read an instance of a RP list
//from a directory
void readInstanceFromDir(TRoboticPositionerList& RPL, const string& dir)
{
    try {
        //contruye una lista tampón
        TRoboticPositionerList t_RPL;

        string str;

        //lee y asigna la lista de orígenes de coordenadas de la lista de posicionadores
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
        t_RPL.assimilate();

        //clona la lista tampón
        RPL.Clone(&t_RPL);

        //desturye los RPs de la lista tampón
        t_RPL.Destroy();

        //RPs of thetampon list must be destroyed here, because
        //the RPL is a list of pointers.

    } catch(...) {
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
        //escribe la instancia en el archivo 'Instance.txt'
        //                strWriteToFile(dir+"/Instance.txt", EAL.InstanceText);

        //por cada área de exclusión de la lista
        for(int i=0; i<EAL.getCount(); i++) {
            //apunta el área de exclusión indicado para facilitar su acceso
            TExclusionArea *EA = EAL[i];

            //construye el nombre del subdirectorio que debe contener la instancia del área de exclusión
            string subdir = dir+"/ExclusionArea"+EA->getIdText().str;
            //fuerza la construcción del directorio
            mkpath(subdir);

            //archiva el contorno de la barrera
            strWriteToFile(subdir+"/Contour_.txt", EA->Barrier.getContour_().getColumnText().str);

            //archiva la instancia del área de exclusión
            strWriteToFile(subdir+"/Instance.txt", EA->getInstanceText().str);
        }

        //archiva la lista de orígenes de coordenadas de la lista de área de exclusiónes
        strWriteToFile(dir+"/ExclusionAreaOriginsTable.txt", TExclusionArea::getOriginsLabelsRow().str+"\r\n"+EAL.getOriginsTableText().str);

    } catch(...) {
        throw;
    }
}
//read an instance of a EA list
//from a directory
void readInstanceFromDir(TExclusionAreaList& EAL, const string& dir,
                         const TRoboticPositionerList& RPL)
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
        EAL.assimilate(RPL);

        //desturye las EAs de la lista tampón
        //                t_EAL.Destroy();

        //Las EAs de la lista tampón no deben
        //ser destruidas porque se trata de una lista de punteros
        //y no de una lista basada en punteros.

    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR Fiber MOS Models:

/*//write the instance of a Fiber MOS Model
//in a directory
void writeInstanceToDir(const string& dir, const TFiberMOSModel *FMM)
{
    //el puntero FMM debe apuntar a un Fiber MOS Model construido
    if(FMM == NULL)
        throw EImproperArgument("pointer FMM should pointto built Fiber MOS Model");

    try {

        //escribe las demás propiedades de la instancia
        //en el archivo 'FiberMOSModelInstance.txt'
        //            strWriteToFile(dir+AnsiString("/FiberMOSModelInstance.txt"), FMM->getInstanceText().str);

        //escribe las instancias de las listas de objetos en el directorio
        writeInstanceToDir(dir, FMM->EAL);
        writeInstanceToDir(dir, FMM->RPL);

    } catch(...) {
        throw;
    }
}
//read an instance of a Fiber MOS Model
//from a directory
void readInstanceFromDir(TFiberMOSModel *FMM, const string& dir)
{
    //el puntero FMM debe apuntar a un Fiber MOS Model construido
    if(FMM == NULL)
        throw EImproperArgument("pointer FMM should pointto built Fiber MOS Model");

    try {
        //contruye una variable tampón
        TFiberMOSModel t_FMM;

        //lee las instancias de las listas de objetos del directorio
        readInstanceFromDir(t_FMM.RPL, dir);
        readInstanceFromDir(t_FMM.EAL, dir, t_FMM.RPL);

        //lee las demás propiedades de la instancia
        //del archivo 'Instance.txt'
        string str;
        strReadFromFile(str, dir+"/Instance.txt");
        t_FMM.setInstanceText(str);

        //Note that here is not necessary assimilate the instance
        //because the reading of each list producess the assimilation.

        //clona la variable tampón
        FMM->Clone(&_FMM);
        //asimila las propiedades
        FMM->Assimilate();

        //ADVERTENCIA: la configuración del FMM debe ser asimilada,
        //ya que algunas de sus propiedades dependen de su posición
        //en la memoria. Sean los punteros de Adjacents.

    } catch(...) {
        throw;
    }
}
*/
//write the instance of a Fiber MOS Model
//in a directory
void writeInstanceToDir(const string& dir, const TFiberMOSModel &FMM)
{
    try {
        //escribe las demás propiedades de la instancia
        //en el archivo 'FiberMOSModelInstance.txt'
//        strWriteToFile(dir+"/FiberMOSModelInstance.txt", FMM.InstanceText);

        //escribe las instancias de las listas de objetos en el directorio
        writeInstanceToDir(dir, FMM.EAL);
        writeInstanceToDir(dir, FMM.RPL);

    } catch(...) {
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
        readInstanceFromDir(t_FMM.RPL, dir);
        readInstanceFromDir(t_FMM.EAL, dir, t_FMM.RPL);

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

    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR Fiber Connection Models:

/*//write the instance of a Fiber Connection Model
//in a directory
void writeInstanceToDir(const string& dir, const TFiberConnectionModel *FCM)
{
    //el puntero FCM debe apuntar a un Fiber Connection Mdoel construido
    if(FCM == NULL)
        throw EImproperArgument("pointer FCM should point to built Fiber Connection Model");

    try {
        //fuerza la construcción del directorio
        mkpath(dir);
        //escribe la tabla de conexiones de la pseudoslit en el archivo correspondiente
        strWriteToFile(dir+"/Connections.txt", FCM->getConnectionsText().str);

    } catch(...) {
        throw;
    }
}
//read an instance of a Fiber Connection Model
//from a directory
void readInstanceFromDir(TFiberConnectionModel *FCM, const string& dir)
{
    //el puntero FCM debe apuntar a un Fiber Connection Mdoel construido
    if(FCM == NULL)
        throw EImproperArgument("pointer FCM should point to built Fiber Connection Model");
    try {
        //construye una variable tampón
        TFiberConnectionModel t_FCM;

        //lee la tabla de conexiones de la pseudoslit en una cadena de texto
        string str;
        strReadFromFile(str, dir+"/Connections.txt");
        //asigna la cadena de texto a la tabla de conexiones de la pseudoslit
        t_FCM.setConnectionsText(str);

        //clona la variable tampón
        //        FCM->Clone(t_FCM);

    } catch(...) {
        throw;
    }
}
*/
//write the instance of a Fiber Connection Model
//in a directory
void writeInstanceToDir(const string& dir, const TFiberConnectionModel& FCM)
{
    try {
        //fuerza la construcción del directorio
        mkpath(dir);
        //escribe la tabla de conexiones de la pseudoslit en el archivo correspondiente
        strWriteToFile(dir+"/Connections.txt", FCM.getConnectionsText().str);

    } catch(...) {
        throw;
    }
}
//read an instance of a Fiber Connection Model
//from a directory
void readInstanceFromDir(TFiberConnectionModel& FCM, const string& dir)
{
    try {
        //construye una variable tampón
        TFiberConnectionModel t_FCM;

        //lee la tabla de conexiones de la pseudoslit en una cadena de texto
        string str;
        strReadFromFile(str, dir+"/Connections.txt");
        //asigna la cadena de texto a la tabla de conexiones de la pseudoslit
        t_FCM.setConnectionsText(str);

        //clona la variable tampón
        FCM.Clone(t_FCM);

    } catch(...) {
        throw;
    }
}

} //namespace Models

//---------------------------------------------------------------------------

