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
//Archivo: FiberMOSModel.cpp
//Contenido: Fiber MOS Model
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "FiberMOSModel2.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//###########################################################################
//MÉTODOS PÚBLICOS:
//###########################################################################

//---------------------------------------------------------------------------
//MÉTODOS DE CONSTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

//construye un Fiber MOS Model
TFiberMOSModel::TFiberMOSModel(void) :
        TFiberMOSModel1()
{
}

//clona un Fiber MOS Model
void TFiberMOSModel::Clone(const TFiberMOSModel *FMM)
{
        //el puntero FMM debería apuntar a un Fiber MOS Model construido
        if(FMM == NULL)
                throw EImproperArgument("pointer FMM should point to built Fiber MOS Model");

        //clona las listas
        RPL.Clone(&FMM->RPL);
        EAL.Clone(&FMM->EAL);
}

//construye un clon de un Fiber MOS Model
TFiberMOSModel::TFiberMOSModel(const TFiberMOSModel *FMM) :
        TFiberMOSModel1(FMM)
{
}

/*//destruye un Fiber MOS Model
TFiberMOSModel::~TFiberMOSModel()
{
}
  */
//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

