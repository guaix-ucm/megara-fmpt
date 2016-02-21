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
//Archivo: FiberMOSModel.h
//Contenido: Fiber MOS Model
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FIBERMOSMODEL2_H
#define FIBERMOSMODEL2_H

#include "FiberMOSModel1.h"
#include "ExclusionArea.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------

//La clase TFiberMOSModel2:
//      Permite la detección de colisiones conlas áreas de exclusión fijas.
//      Permitela ejecución simulada por temporizador.

//clase lista de posicionadores
class TFiberMOSModel : public TFiberMOSModel1 {
protected:
public:
        //##################################################################
        //MÉTODOS PÚBLICOS:
        //##################################################################

        //------------------------------------------------------------------
        //MÉTODOS DE CONSTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

        //construye un Fiber MOS Model
        TFiberMOSModel(void);

        //clona un Fiber MOS Model
        void Clone(const TFiberMOSModel*);

        //construye un clon de un Fiber MOS Model
        TFiberMOSModel(const TFiberMOSModel*);

/*        //destruye un Fiber MOS Model
        ~TFiberMOSModel();
  */
};
/*
//Fiber MOS Model en formato Texto
class TFiberMOSModelInstanceText {
public:
        //lista de posicionadores de fibra en formato texto
        TFiberPositionerListText FPLT;
        //lista deñáreas de exclusión en formato texto
        TExclusionAreaListText EALT;

        //construye un Fiber MOS Model en formato texto
        TFiberMOSModelText(void);

        //vacía el Fiber MOS Model en formato texto
        void Clear(void);

        //indica si el objeto no contiene una instancia
        bool IsEmpty(void);

        //aplica la instancia a una lista de posicionadores de fibra
        void GetInstance(TFiberPositionerList& FPL) const;
        //obtiene la instancia de una lista de posicionadores de fibra
        void SetInstance(TFiberPositionerList& FPL);
};*/

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // FIBERMOSMODEL2_H
