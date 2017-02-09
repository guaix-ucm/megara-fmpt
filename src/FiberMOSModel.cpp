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
//File: FiberMOSModel.cpp
//Content: Fiber MOS model
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "FiberMOSModel.h"
#include "TextFile.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//CONJUNTOS DE PROPIEDADES EN MODO TEXTO:

AnsiString TFiberMOSModel::getInstanceText(void) const
{
    AnsiString S;

    //        S += AnsiString("RoboticPositionerListInstance:\r\n");
    //      S += StrIndent(RPL.getInstanceText())+AnsiString("\r\n");

    return S;
}
void TFiberMOSModel::setInstanceText(const AnsiString& S)
{
    try {
        //contruye un clon de un Fiber MOS Model
        TFiberMOSModel aux(this);
        TFiberMOSModel *FMM = &aux;
        //lee la instancia mediante el clon
        int i = 1;
        readInstance(FMM, S, i);

        //avanza el índice hasta el próximo caracterno separador
        //o hasta el final de la cadena
        StrTravelSeparatorsIfAny(S, i);
        //siel índice no indica a la posúltima posición de la cadena
        if(i<=S.Length())
            //indica que la cadena S debería contener solamente una instancia de Fiber MOS Model
            throw EImproperArgument("string S should containan instance of Fiber MOS Model only");

        //asigna la variable tampón
        Clone(FMM);

    } catch(...) {
        throw;
    }
}

//------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//lee una instancia de Fiber MOS Model
//en una cadena de texto en formato de asignaciones
void  TFiberMOSModel::readInstance(TFiberMOSModel *FMM,
                                    const AnsiString &S, int &i)
{
    //el puntero FMM debe apuntar a un Fiber MOS Model construído
    if(FMM == NULL)
        throw EImproperArgument("pointer FMM should point to built Fiber MOS Model");

    //lee la instancia en la lista de RPs
    TRoboticPositionerList1 *RPL = &FMM->RPL;
    TRoboticPositionerList1::readInstance(RPL, S, i);
}

//MÉTODOS DE CONSTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

//construye un Fiber MOS Model
TFiberMOSModel::TFiberMOSModel(void) :
    //contruye las listas de objetos
    RPL(),
    EAL()
{
}

//clona un Fiber MOS Model
void TFiberMOSModel::Clone(const TFiberMOSModel *FMM)
{
    //comprueba las precondiciones
    if(FMM == NULL)
        throw EImproperArgument("pointer FMM should point to built Fiber MOS Model");

    //clona las listas de objetos
    RPL.Clone(&FMM->RPL);
    EAL.Clone(&FMM->EAL);
}

//construye un clon de un Fiber MOS Model
TFiberMOSModel::TFiberMOSModel(const TFiberMOSModel *FMM)
{
    //el puntero FMM debería apuntar a un Fiber MOS Model construído
    if(FMM == NULL)
        throw EImproperArgument("pointer FMM should point to built Fiber MOS Model");

    //copia todas las propiedades
    Clone(FMM);
}

//destruye un Fiber MOS Model
TFiberMOSModel::~TFiberMOSModel()
{
    //destruye los robots posicionadores
    EAL.Destroy();
    //destruye las áreas de exclusión
    RPL.Destroy();
}

//MÉTODOS DE DESTRUCCION DE ÁREAS DE EXCLUSIÓN:

//destruye todos los objetos de la lista
void TFiberMOSModel::Destroy(void)
{
    //destruye los robots posicionadores
    EAL.Destroy();
    //destruye las áreas de exclusión
    RPL.Destroy();
}

//MÉTODOS PARA CONTRUIR Y DESTRUIR POSICIONADORES:

//contruye una colmena cuadrada y asimilala situación
void TFiberMOSModel::buildSquareHive(void)
{
    //inicializa el contador de identificadores
    int CountId = 1;
    //contruye una colmena cuadrada por defecto
    RPL.buildSquareHive(CountId);
    //asimila la nueva configuración
    assimilate();
}
/*//borra los posicionadores seleccionados y asimilala situación
void TFiberMOSModel::DeleteSelected(void)
{
        //destruye los posicionadores seleccionados
        RPL.DestroySelected();
        //asimila la nueva configuración
        Assimilate();
}*/

/*//MÉTODOS DE TRANSFORMACIÓN DE COORDENADAS:

//transforma una lista de puntos objetivo dados en coordenadas cartesianas en S0,
//en una lista de puntos objetivo dados en posiciones angulares de los ejes en pasos de sus posicionadores adscritos
//los puntos objetivo deben estar en el dominio de sus posic. adscritos (en sentido laxo, teniendo en cuenta el error num)
void TFiberMOSModelModel1::S0recToAmgularPositions(TTargetPointAngPosList& TPAPL,
        TTargetPoinRecList& TPRL)
{
}
  */
//MÉTODOS DE ASIMILACIÓN:

//asimila la configuración de las listas de objetos
void TFiberMOSModel::assimilate()
{
    EAL.assimilate();
    RPL.assimilate(EAL);
}

//MÉTODOS PARA DETERMINAR SI HAY COLISIONES:

//determina si algún brazo de algún posicionador
//colisiona con el brazo de algún posicionador adyacente
bool TFiberMOSModel::thereIsCollision(void)
{
    //si los posicionadores colisionan entre si o contra las áreas de exclusión
    if(RPL.thereIsCollision()/* || EAL.thereIsCollision()*/)
        return true; //indica que si hay colisión

    return false; //indica que no hay colisión
}

//RANDOMIZADO DE LOS EJES:

//lleva los ejes de los posicionadores a posiciones aleatorias
//con distribución uniforme en sus dominios
//en las que no colisionan entre si
void TFiberMOSModel::randomizeWithoutCollision(void)
{
    /*        TRoboticPositioner *RP;

        //para cada posicionador de la lista de posicionadores
        for(int i=0; i<Count; i++)
                do {
                        //apunta el posicionador indicado para facilitar su acceso
                        RP = Items[i];
                        //lleva randomiza los ejes del posicionador
                        RP->Actuator->Randomizep_1();
                        RP->Actuator->Arm->Randomizep___3();
                        //mientras colisione con algún adyacente
                } while(RP->Actuator->ThereIsCollisionWithPendingAdjacent());*/
}

//RANDOMIZADO DE P3:

//lleva el punto P3 de los posicionadores a posiciones aleatorias
//con distribución uniforme en su dominio
//en las que no colisionan entre si
void TFiberMOSModel::randomizeP3WithoutCollision(void)
{
    /*        TRoboticPositioner *RP;

        //para cada posicionador de la lista de osicionadores
        for(int i=0; i<Count; i++)
                do {
                        //apunta el posicionador indicado para facilitar su acceso
                        RP = Items[i];
                        //randomiza el punto P3
                        RP->Actuator->RandomizeP3();
                        //mientras colisione con algún adyacente
                } while(RP->Actuator->ThereIsCollisionWithPendingAdjacent());*/
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

