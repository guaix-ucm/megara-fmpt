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
/// @file FiberMOSModel.cpp
/// @brief Fiber MOS model
/// @author Isaac Morales Durán
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
    string str = "# Instance properties of the FMM (Fiber MOS Model):";

    str += "\r\n\r\n";
    str += commentedLine("Instance_version = \""+Instance_version+"\"", "Version of the Fiber MOS Model Instance");
    str += "\r\n\r\n";
    str += RPL.getInstanceText().str;

    return AnsiString(str);
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

        //busca texto inesperado
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text");

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
    //comprueba las precondiciones
    if(FMM == NULL)
        throw EImproperArgument("pointer FMM should point to built Fiber MOS Model");
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    try {
        //lee la propiedad Instance_version
        AnsiString t_Instance_version;
        StrTravelSeparatorsIfAny(S, i);
        StrTravelLabel("Instance_version", S, i);
        StrTravelSeparatorsIfAny(S, i);
        StrTravelLabel("=", S, i);
        StrTravelSeparatorsIfAny(S, i);
        StrReadStringBetweenChars(t_Instance_version, S, i);

        //lee la instancia de la lista de RPs
        TRoboticPositionerList RPL(&FMM->RPL);
        TRoboticPositionerList1 *aux = &RPL;
        TRoboticPositionerList1::readInstance(aux, S, i);

        //asigna las variables tampón
        FMM->Instance_version = t_Instance_version.str;
        FMM->RPL.Clone(&RPL);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading instance of FMM: ");
        throw;
    }
}

//MÉTODOS DE CONSTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

//construye un Fiber MOS Model
TFiberMOSModel::TFiberMOSModel(void) :
    //contruye las listas de objetos
    EAL(),
    RPL(),
    //inicializa las demás rpopiedades
    Instance_version("")
{
}

//clona un Fiber MOS Model
void TFiberMOSModel::Clone(const TFiberMOSModel *FMM)
{
    //comprueba las precondiciones
    if(FMM == NULL)
        throw EImproperArgument("pointer FMM should point to built Fiber MOS Model");

    //clona las listas de objetos
    EAL.Clone(&FMM->EAL);
    RPL.Clone(&FMM->RPL);
    //clona las demás propiedades
    Instance_version = FMM->Instance_version;
}

//construye un clon de un Fiber MOS Model
TFiberMOSModel::TFiberMOSModel(const TFiberMOSModel *FMM)
{
    //el puntero FMM debería apuntar a un Fiber MOS Model construído
    if(FMM == NULL)
        throw EImproperArgument("pointer FMM should point to built Fiber MOS Model");

    //clona todas las propiedades
    Clone(FMM);
}

//destruye un Fiber MOS Model
TFiberMOSModel::~TFiberMOSModel()
{
    //destruye las áreas de exclusión
    RPL.Destroy();
    //destruye los robots posicionadores
    EAL.Destroy();
}

//MÉTODOS DE DESTRUCCION DE ÁREAS DE EXCLUSIÓN:

//destruye todos los objetos de la lista
void TFiberMOSModel::Destroy(void)
{
    //destruye las áreas de exclusión
    RPL.Destroy();
    //destruye los robots posicionadores
    EAL.Destroy();
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
    EAL.determineAdjacents(RPL);
    EAL.sortAdjacents();
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

//imprime los elementos colisionados (EAs y RPs) en una lista de listas
void TFiberMOSModel::getCollidedText(string& str)
{
    //build a structure for anotate the colliding sets
    TCSL CSL;

    //procesate each RP of the FMM
    for(int i=0; i<RPL.getCount(); i++) {
        TRoboticPositioner *RP = RPL[i];

        //get their colliding adjacent EAs
        TItemsList<TExclusionArea*> CA_EAs;
        RP->getActuator()->searchCollindingAdjacent(CA_EAs);
        //get their colliding adjacent RPs
        TItemsList<TRoboticPositioner*> CA_RPs;
        RP->getActuator()->searchCollindingAdjacent(CA_RPs);

        //Each set {RP, CA_EAs, CA_RPs}:
        //  - can be merged with a set of items of the CSL,
        //  - or can be added in a new set.

        //SEARCH THE FIRST SET OF THE CSL WHERE IS AN ITEM (EA OR RP):

        int j, k;
        bool found = CSL.findRP(j, k, RP->getActuator()->getId());

        if(!found) {
            for(int l=0; l<CA_EAs.getCount(); l++) {
                TExclusionArea *CA_EA = CA_EAs[l];
                found = CSL.findEA(j, k, CA_EA->getId());
            }
        }

        if(!found) {
            for(int l=0; l<CA_RPs.getCount(); l++) {
                TRoboticPositioner *CA_RP = CA_RPs[l];
                found = CSL.findRP(j, k, CA_RP->getActuator()->getId());
            }
        }

        //ADD A NEW SET, IF ANY:

        if(!found) {
            j = CSL.getCount();
            CSL.setCount(CSL.getCount() + 1);
        }

        //ADD ALL ITEMS OF THE SET j:

        int m, n;
        found = CSL.findRP(m, n, RP->getActuator()->getId());
        if(!found)
            CSL.Get(j).RPids.Add(RP->getActuator()->getId());

        for(int l=0; l<CA_EAs.getCount(); l++) {
            TExclusionArea *CA_EA = CA_EAs[l];
            found = CSL.findEA(m, n, CA_EA->getId());
            if(!found)
                CSL.Get(j).EAids.Add(CA_EA->getId());
        }

        for(int l=0; l<CA_RPs.getCount(); l++) {
            TRoboticPositioner *CA_RP = CA_RPs[l];
            found = CSL.findRP(m, n, CA_RP->getActuator()->getId());
            if(!found)
                CSL.Get(j).RPids.Add(CA_RP->getActuator()->getId());
        }
    }

    //delete the colliding sets with only an item
    for(int i=CSL.getCount()-1; i>=0; i--) {
        int count = CSL.Get(i).EAids.getCount() + CSL.Get(i).RPids.getCount();
        if(count < 2)
            CSL.Delete(i);
    }

    //print the CSL in the output string
    str = "{";
    for(int i=0; i<CSL.getCount(); i++) {
        str += "{";
        CSL.Get(i).EAids.SortInc();
        for(int j=0; j<CSL.Get(i).EAids.getCount(); j++) {
            str += "EA";
            str += inttostr(CSL.Get(i).EAids[j]);
            str += ", ";
        }
        CSL.Get(i).RPids.SortInc();
        for(int j=0; j<CSL.Get(i).RPids.getCount(); j++) {
            str += "RP";
            str += inttostr(CSL.Get(i).RPids[j]);
            str += ", ";
        }
        str.resize(str.length() - 2);
        str += "}, ";
    }
    if(str.length() > 1)
        str.resize(str.length() - 2);
    str += "}";
}

//RANDOMIZADO DE LOS EJES:

/*//lleva los ejes de los posicionadores a posiciones aleatorias
//con distribución uniforme en sus dominios
//en las que no colisionan entre si
void TFiberMOSModel::randomizeWithoutCollision(void)
{
        TRoboticPositioner *RP;

        //para cada posicionador de la lista de posicionadores
        for(int i=0; i<Count; i++)
                do {
                        //apunta el posicionador indicado para facilitar su acceso
                        RP = Items[i];
                        //lleva randomiza los ejes del posicionador
                        RP->Actuator->Randomizep_1();
                        RP->Actuator->Arm->Randomizep___3();
                        //mientras colisione con algún adyacente
                } while(RP->Actuator->ThereIsCollisionWithPendingAdjacent());
}

//RANDOMIZADO DE P3:

//lleva el punto P3 de los posicionadores a posiciones aleatorias
//con distribución uniforme en su dominio
//en las que no colisionan entre si
void TFiberMOSModel::randomizeP3WithoutCollision(void)
{
        TRoboticPositioner *RP;

        //para cada posicionador de la lista de osicionadores
        for(int i=0; i<Count; i++)
                do {
                        //apunta el posicionador indicado para facilitar su acceso
                        RP = Items[i];
                        //randomiza el punto P3
                        RP->Actuator->RandomizeP3();
                        //mientras colisione con algún adyacente
                } while(RP->Actuator->ThereIsCollisionWithPendingAdjacent());
}
*/
//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

