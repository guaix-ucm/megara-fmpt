//---------------------------------------------------------------------------
//Archivo: FileMethods.cpp
//Contenido: métodos de archivo para lectura y escritura de modelos
//Última actualización: 15/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "FileMethods.h"
#include "TextFile.h" //StrReadFromFile, StrWriteToFile

using namespace Strings;

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//FUNCIONES DE ARCHIVO PARA LISTAS DE POSICIONADORES DE FIBRA:

//escribe la instancia de una lista de RPs
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TRoboticPositionerList& RPL)
{
        try {
                //archiva el mapa de posicionadores
                StrWriteToFile(Dir+AnsiString("/InstanceMap.txt"), RPL.getInstanceMapText());

                //escribe la instancia en el archivo 'Instance.txt'
                StrWriteToFile(Dir+AnsiString("/Instance.txt"), RPL.getInstanceText());

                //por cada posicionador de la lista
                for(int i=0; i<RPL.getCount(); i++) {
                        //apunta el posicionador indicado para facilitar su acceso
                        TRoboticPositioner *RP = RPL[i];

                        //construye el nombre del subdirectorio que debe contener la instancia del posicionador
                        AnsiString SubDir = Dir+AnsiString("/RoboticPositioner")+RP->getActuator()->getId();
                        //fuerza la construcción del directorio
                        mkpath(SubDir.str);

                        //archiva el contorno del brazo
                        StrWriteToFile(SubDir+AnsiString("/Contour____.txt"), RP->getActuator()->getArm()->getContour____().getColumnText());

                        //archiva el contorno de la barrera
                        StrWriteToFile(SubDir+AnsiString("/Contour_.txt"), RP->getActuator()->getBarrier()->getContour_().getColumnText());

                        //archiva la función de compresión del rotor 1
                        StrWriteToFile(SubDir+AnsiString("/F1.txt"), RP->getActuator()->getF().getTableText());

                        //archiva la función de compresión del rotor 2
                        StrWriteToFile(SubDir+AnsiString("/F2.txt"), RP->getActuator()->getArm()->getF().getTableText());

                        //archiva la instancia del posicionador
                        StrWriteToFile(SubDir+AnsiString("/Instance.txt"), RP->getInstanceText());
                }

                //archiva la lista de orígenes de coordenadas de la lista de posicionadores
                StrWriteToFile(Dir+AnsiString("/RoboticPositionerOriginsTable.txt"), TActuator::GetOriginsLabelsRow()+AnsiString("\r\n")+RPL.getOriginsTableText());

        } catch(...) {
                throw;
        }
}
//lee la instancia de una lista de RPs
//de un directorio
void ReadInstanceFromDir(TRoboticPositionerList& RPL, const AnsiString& Dir)
{
        try {
                //contruye una lista tampón
                TRoboticPositionerList _RPL;

                AnsiString S;

                //lee y asigna la lista de orígenes de coordenadas de la lista de posicionadores
                StrReadFromFile(S, Dir+AnsiString("/RoboticPositionerOriginsTable.txt"));
                _RPL.setOriginsTableText(S);

                //por cada posicionador de la lista
                for(int i=0; i<_RPL.getCount(); i++) {
                        //apunta el posicionador indicado para facilitar su acceso
                        TRoboticPositioner *RP = _RPL[i];

                        //construye el nombre del subdirectorio que contiene la instancia del posicionador
                        AnsiString SubDir = Dir+AnsiString("/RoboticPositioner")+RP->getActuator()->getIdText();

                        //lee y asigna el contorno del brazo
                        StrReadFromFile(S, SubDir+AnsiString("/Contour____.txt"));
                        RP->getActuator()->getArm()->setContour____ColumnText(S);

                        //lee y asigna el contorno de la barrera
                        StrReadFromFile(S, SubDir+AnsiString("/Contour_.txt"));
                        RP->getActuator()->getBarrier()->setContour_ColumnText(S);

                        //lee y asigna la función de compresión del rotor 1
                        StrReadFromFile(S, SubDir+AnsiString("/F1.txt"));
                        RP->getActuator()->setFTableText(S);

                        //lee y asigna la función de compresión del rotor 2
                        StrReadFromFile(S, SubDir+AnsiString("/F2.txt"));
                        RP->getActuator()->getArm()->setFTableText(S);

                        //lee y asigna la instancia del posicionador
                        StrReadFromFile(S, SubDir+AnsiString("/Instance.txt"));
                        RP->setInstanceText(S);
                }

                //lee y asigna la instancia de la lista de posicionadores
                StrReadFromFile(S, Dir+AnsiString("/Instance.txt"));
                _RPL.setInstanceText(S);

                //carga el mapa de posicionadores
                StrReadFromFile(S, Dir+AnsiString("/InstanceMap.txt"));
                _RPL.setInstanceMapText(S);

                //El mapa de RPs puede ser cargado de un archivo, o puede ser
                //determinado tras la asimilación mediante el método:
                //      void TRoboticPositioneer::GenerateMap(void);
                //Aunque es preferible cargarlo de un archivo porque así
                //se pueden elegir arbitrariamente las dimensiones del mapa.

                //clona la lista tampón
                RPL.Clone(&_RPL);
                //asimila los parámetros de dimensionamiento
                RPL.Assimilate();
                //genera el mapa de RPs
//                RPL.GenerateMap();

                //desturye los RPs de la lista tampón
//                _RPL.Destroy();

                //Los robots posicionadores de la lista tampón no deben
                //ser destruidas aquí, porque se trata de una lista de
                //punteros y no de una lista basada en punteros.

        } catch(...) {
                throw;
        }
}

//---------------------------------------------------------------------------
//FUNCIONES DE ARCHIVO PARA LISTAS DE ÁREAS DE EXCLUSIÓN:

//escribe la instancia de una lista de EAs
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TExclusionAreaList& EAL)
{
        try {
                //escribe la instancia en el archivo 'Instance.txt'
//                StrWriteToFile(Dir+"/Instance.txt", EAL.InstanceText);

                //por cada área de exclusión de la lista
                for(int i=0; i<EAL.getCount(); i++) {
                        //apunta el área de exclusión indicado para facilitar su acceso
                        TExclusionArea *EA = EAL[i];

                        //construye el nombre del subdirectorio que debe contener la instancia del área de exclusión
                        AnsiString SubDir = Dir+AnsiString("/ExclusionArea")+EA->getId();
                        //fuerza la construcción del directorio
                        mkpath(SubDir.str);

                        //archiva el contorno de la barrera
                        StrWriteToFile(SubDir+AnsiString("/Contour_.txt"), EA->Barrier.getContour_().getColumnText());

                        //archiva la instancia del área de exclusión
                        StrWriteToFile(SubDir+AnsiString("/Instance.txt"), EA->getInstanceText());
                }

                //archiva la lista de orígenes de coordenadas de la lista de área de exclusiónes
                StrWriteToFile(Dir+AnsiString("/ExclusionAreaOriginsTable.txt"), TExclusionArea::GetOriginsLabelsRow()+AnsiString("\r\n")+EAL.getOriginsTableText());

        } catch(...) {
                throw;
        }
}
//lee la instancia de una lista de EAs
//de un directorio
void ReadInstanceFromDir(TExclusionAreaList& EAL, const AnsiString& Dir,
        const TRoboticPositionerList& RPL)
{
        try {
                //contruye una lista tampón
                TExclusionAreaList _EAL;

                AnsiString S;

                //lee y asigna la tabla de orígenes de coordenadas
                StrReadFromFile(S, Dir+AnsiString("/ExclusionAreaOriginsTable.txt"));
                _EAL.setOriginsTableText(S);

                //por cada área de exclusión de la lista
                for(int i=0; i<_EAL.getCount(); i++) {
                        //apunta el área de exclusión indicada para facilitar su acceso
                        TExclusionArea *EA = _EAL[i];

                        //construye el nombre del subdirectorio que contiene la instancia del área de exclusión
                        AnsiString SubDir = Dir+AnsiString("/ExclusionArea")+EA->getId();

                        //lee y asigna el contorno de la barrera
                        StrReadFromFile(S, SubDir+AnsiString("/Contour_.txt"));
                        EA->Barrier.setContour_ColumnText(S);

                        //lee y asigna la instancia del área de exclusión
                        StrReadFromFile(S, SubDir+AnsiString("/Instance.txt"));
                        EA->setInstanceText(S);
                }

                //lee y asigna la instancia de la lista de EAs
//                StrReadFromFile(S, Dir+"/Instance.txt");
  //              _EAL.InstanceText = S;

                //clona la lista tampón
                EAL.Clone(&_EAL);
                //asimila los parámetros de dimensionamiento
                EAL.Assimilate(RPL);

                //desturye las EAs de la lista tampón
//                _EAL.Destroy();

                //Las EAs de la lista tampón no deben
                //ser destruidas porque se trata de una lista de punteros
                //y no de una lista basada en punteros.

        } catch(...) {
                throw;
        }
}

//---------------------------------------------------------------------------
//FUNCIONES DE ARCHIVO PARA Fiber MOS Models:

//escribe la instancia de un Fiber MOS Model
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TFiberMOSModel *FMM)
{
        //el puntero FMM debe apuntar a un Fiber MOS Model construido
        if(FMM == NULL)
                throw EImproperArgument("pointer FMM should pointto built Fiber MOS Model");

        try {
            /*
                            //escribe las demás propiedades de la instancia
                            //en el archivo 'FiberMOSModelInstance.txt'
                            StrWriteToFile(Dir+AnsiString("/FiberMOSModelInstance.txt"), FMM->getInstanceText());
            */
                //escribe las instancias de las listas de objetos en el directorio
                WriteInstanceToDir(Dir, FMM->EAL);
                WriteInstanceToDir(Dir, FMM->RPL);

        } catch(...) {
                throw;
        }
}
//lee la instancia de un Fiber MOS Model
//de un directorio
void ReadInstanceFromDir(TFiberMOSModel *FMM, const AnsiString& Dir)
{
        //el puntero FMM debe apuntar a un Fiber MOS Model construido
        if(FMM == NULL)
                throw EImproperArgument("pointer FMM should pointto built Fiber MOS Model");

        try {
            //contruye una variable tampón
            TFiberMOSModel _FMM;

                //lee las instancias de las listas de objetos del directorio
                ReadInstanceFromDir(_FMM.RPL, Dir);
                ReadInstanceFromDir(_FMM.EAL, Dir, _FMM.RPL);

                //lee las demás propiedades de la instancia
                //del archivo 'FiberMOSModelInstance.txt'
//                AnsiString S;
//                StrReadFromFile(S, Dir+"/FiberMOSModelInstance.txt");
  //              FMM->InstanceText = S;

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

//escribe la instancia de un Fiber MOS Model
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TFiberMOSModel &FMM)
{
        try {
        /*                //escribe las demás propiedades de la instancia
                        //en el archivo 'FiberMOSModelInstance.txt'
                        StrWriteToFile(Dir+"/FiberMOSModelInstance.txt", FMM.InstanceText);
          */
                //escribe las instancias de las listas de objetos en el directorio
                WriteInstanceToDir(Dir, FMM.EAL);
                WriteInstanceToDir(Dir, FMM.RPL);

        } catch(...) {
                throw;
        }
}
//lee la instancia de un Fiber MOS Model
//de un directorio
void ReadInstanceFromDir(TFiberMOSModel& FMM, const AnsiString& Dir)
{
        try {
                //contruye una variable tampón
                TFiberMOSModel _FMM;

                //lee las instancias de las listas de objetos del directorio
                ReadInstanceFromDir(_FMM.RPL, Dir);
                ReadInstanceFromDir(_FMM.EAL, Dir, _FMM.RPL);

/*                //lee las demás propiedades de la instancia
                //del archivo 'Instance.txt'
                AnsiString S;
                StrReadFromFile(S, Dir+"/Instance.txt");
                _FMM.InstanceText = S;
                _FMM.Assimilate();
  */
                //clona la variable tampón
                FMM.Clone(&_FMM);
                //asimila las propiedades
                FMM.Assimilate();

                //ADVERTENCIA: la configuración del FMM debe ser asimilada,
                //ya que algunas de sus propiedades dependen de su posición
                //en la memoria. Sean los punteros de Adjacents.

        } catch(...) {
                throw;
        }
}

//---------------------------------------------------------------------------
//FUNCIONES DE ARCHIVO PARA Fiber Connection Models:

//escribe la instancia de un Fiber Connection Model
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TFiberConnectionModel *FCM)
{
    //el puntero FCM debe apuntar a un Fiber Connection Mdoel construido
    if(FCM == NULL)
            throw EImproperArgument("pointer FCM should point to built Fiber Connection Model");

    try {
        //fuerza la construcción del directorio
        mkpath(Dir.str);
        //escribe la tabla de conexiones de la pseudoslit en el archivo correspondiente
        StrWriteToFile(Dir+AnsiString("/Connections.txt"), FCM->getConnectionsText());

    } catch(...) {
        throw;
    }
}

//lee la instancia de un Fiber Connection Model
//de un directorio
void ReadInstanceFromDir(TFiberConnectionModel *FCM, const AnsiString& Dir)
{
    //el puntero FCM debe apuntar a un Fiber Connection Mdoel construido
    if(FCM == NULL)
            throw EImproperArgument("pointer FCM should point to built Fiber Connection Model");
    try {
        //construye una variable tampón
        TFiberConnectionModel _FCM;

        //lee la tabla de conexiones de la pseudoslit en una cadena de texto
        AnsiString S;
        StrReadFromFile(S, Dir+AnsiString("/Connections.txt"));
        //asigna la cadena de texto a la tabla de conexiones de la pseudoslit
        _FCM.setConnectionsText(S);

        //clona la variable tampón
//        FCM->Clone(_FCM);

    } catch(...) {
        throw;
    }
}

//escribe la instancia de un Fiber Connection Model
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TFiberConnectionModel& FCM)
{
    try {
        //fuerza la construcción del directorio
        mkpath(Dir.str);
        //escribe la tabla de conexiones de la pseudoslit en el archivo correspondiente
        StrWriteToFile(Dir+AnsiString("/Connections.txt"), FCM.getConnectionsText());

    } catch(...) {
        throw;
    }
}

//lee la instancia de un Fiber Connection Model
//de un directorio
void ReadInstanceFromDir(TFiberConnectionModel& FCM, const AnsiString& Dir)
{
    try {
        //construye una variable tampón
        TFiberConnectionModel _FCM;

        //lee la tabla de conexiones de la pseudoslit en una cadena de texto
        AnsiString S;
        StrReadFromFile(S, Dir+AnsiString("/Connections.txt"));
        //asigna la cadena de texto a la tabla de conexiones de la pseudoslit
        _FCM.setConnectionsText(S);

        //clona la variable tampón
//        FCM.Clone(_FCM);

    } catch(...) {
        throw;
    }
}

/*//###########################################################################
//FUNCIONES PARA OBJETOS EN FORMATO TEXTO
//###########################################################################

//---------------------------------------------------------------------------
//FUNCIONES DE ARCHIVO PARA LISTAS DE POSICIONADORES DE FIBRA
//EN FORMATO TEXTO:

//escribe la instancia de una lista de RPs
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TRoboticPositionerListText& RPLT)
{
        //si la lista de RPs no contiene una instancia
        if(RPLT.IsEmpty())
                return; //no hace nada

        //fuerza la construcción del directorio
        try {
                ForceDirectories(Dir);
        } catch(...) {
                throw;
        }

        //por cada RP de la lista
        for(int i=0; i<RPLT.ItemsText.Count; i++) {
                //componela ruta del subdirectorio correspondiente al posicionador indicado
                SubDir = Dir+"RoboticPositioner"+IntToStr(i);

                //escribe las cadenas ue contiene la instancia del posicionador indicado
                StrWriteToFile(SubDir+"/Contour____.txt", RPLT.ItemsText[i].Contour____ColumnText);
                StrWriteToFile(SubDir+"/Contour_.txt", RPLT.ItemsText[i].Contour_ColumnText);
                StrWriteToFile(SubDir+"/Instance.txt", RPLT.ItemsText[i].InstanceText);
        }
        //archiva la tabla de orígenes de coordenada
        StrWriteToFile(Dir+"/RoboticPositionerOriginsTableText.txt", RPL.OriginsTableText);
}
//lee la instancia de una lista de RPs
//de un directorio
void ReadInstanceFromDir(TRoboticPositionerListText& RPLT, const AnsiString& Dir)
{
        //el directorio Dir debe existir
        if(!DirectoryExists(Dir))
                throw EImproperArgument("diectory Dir should to exists");

        //el archivo Dir+"/RoboticPositionerOriginsTableText.txt" debe existir
        if(!FileExists(Dir+"/RoboticPositionerOriginsTableText.txt"))
                throw EImproperArgument("file Dir+\"/RoboticPositionerOriginsTableText.txt\" should exists");


}

//---------------------------------------------------------------------------
//FUNCIONES DE ARCHIVO PARA LISTAS DE ÁREAS DE EXCLUSIÓN
//EN FORMATO TEXTO:

//escribe la instancia de una lista de EAs
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TExclusionAreaListText& EALT)
{
}
//lee la instancia de una lista de EAs
//de un directorio
void ReadInstanceFromDir(TExclusionAreaListText& EALT, const AnsiString& Dir)
{
}

//---------------------------------------------------------------------------
//FUNCIONES DE ARCHIVO PARA POSICIONADORES MULTIFIBRA
//EN FORMATO TEXTO:

//escribe la instancia de un Fiber MOS Model
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TFiberMOSModelText *MP)
{
}
//lee la instancia de un Fiber MOS Model
//de un directorio
void ReadInstanceFromDir(TFiberMOSModelText *MP, const AnsiString& Dir)
{
}
*/
//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

