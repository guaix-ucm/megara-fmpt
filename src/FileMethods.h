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
//Archivo: FileMethods.h
//Contenido: métodos de archivo para lectura y escritura de modelos
//Última actualización: 15/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FILEMETHODS_H
#define FILEMETHODS_H

#include "FiberMOSModel2.h"
#include "FiberConnectionModel.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//FUNCIONES DE ARCHIVO PARA LISTAS DE POSICIONADORES DE FIBRA:

//escribe la instancia de una lista de RPs
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TRoboticPositionerList&);
//lee la instancia de una lista de RPs
//de un directorio
void ReadInstanceFromDir(TRoboticPositionerList&, const AnsiString& Dir);

//---------------------------------------------------------------------------
//FUNCIONES DE ARCHIVO PARA LISTAS DE ÁREAS DE EXCLUSIÓN:

//escribe la instancia de una lista de EAs
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TExclusionAreaList&);
//lee la instancia de una lista de EAs
//de un directorio
void ReadInstanceFromDir(TExclusionAreaList&, const AnsiString& Dir,
        const TRoboticPositionerList&);

//---------------------------------------------------------------------------
//FUNCIONES DE ARCHIVO PARA Fiber MOS Models:

//escribe la instancia de un Fiber MOS Model
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TFiberMOSModel*);
//lee la instancia de un Fiber MOS Model
//de un directorio
void ReadInstanceFromDir(TFiberMOSModel*, const AnsiString& Dir);

//escribe la instancia de un Fiber MOS Model
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TFiberMOSModel&);
//lee la instancia de un Fiber MOS Model
//de un directorio
void ReadInstanceFromDir(TFiberMOSModel&, const AnsiString& Dir);

//---------------------------------------------------------------------------
//FUNCIONES DE ARCHIVO PARA Fiber Connection Models:

//escribe la instancia de un Fiber Connection Model
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TFiberConnectionModel*);
//lee la instancia de un Fiber Connection Model
//de un directorio
void ReadInstanceFromDir(TFiberConnectionModel*, const AnsiString& Dir);

//escribe la instancia de un Fiber Connection Model
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TFiberConnectionModel&);
//lee la instancia de un Fiber Connection Model
//de un directorio
void ReadInstanceFromDir(TFiberConnectionModel&, const AnsiString& Dir);

/*//###########################################################################
//FUNCIONES PARA OBJETOS EN FORMATO TEXTO
//###########################################################################

//---------------------------------------------------------------------------
//FUNCIONES DE ARCHIVO PARA LISTAS DE POSICIONADORES DE FIBRA
//EN FORMATO TEXTO:

//escribe la instancia de una lista de posicionadores de fibra
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TRoboticPositionerListText&);
//lee la instancia de una lista de posicionadores de fibra
//de un directorio
void ReadInstanceFromDir(TRoboticPositionerListText&, const AnsiString& Dir);

//---------------------------------------------------------------------------
//FUNCIONES DE ARCHIVO PARA LISTAS DE ÁREAS DE EXCLUSIÓN
//EN FORMATO TEXTO:

//escribe la instancia de una lista de EAs
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TExclusionAreaListText&);
//lee la instancia de una lista de EAs
//de un directorio
void ReadInstanceFromDir(TExclusionAreaListText&, const AnsiString& Dir);

//---------------------------------------------------------------------------
//FUNCIONES DE ARCHIVO PARA POSICIONADORES MULTIFIBRA
//EN FORMATO TEXTO:

//escribe la instancia de un Fiber MOS Model
//en un directorio
void WriteInstanceToDir(const AnsiString& Dir, const TFiberMOSModelText*);
//lee la instancia de un Fiber MOS Model
//de un directorio
void ReadInstanceFromDir(TFiberMOSModelText*, const AnsiString& Dir);
*/
//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // FILEMETHODS_H
