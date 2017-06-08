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
///@file FiberMOSModel.h
///@brief Fiber MOS model
///@author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FiberMOSModel_H
#define FiberMOSModel_H

#include "FiberConnectionModel.h"
//#include "ExclusionAreaList.h"
//#include "ExclusionArea.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TFiberMOSModel
//---------------------------------------------------------------------------

//build a structure for anotate the colliding sets
class TCollidingSet {
public:
    TVector<int> EAids;
    TVector<int> RPids;
};
class TCSL : public TPointersList<TCollidingSet> {
public:
    bool findRP(int& i, int& j, const int id) const {
        for(i=0; i<getCount(); i++)
            for(j=0; j<Items[i]->RPids.getCount(); j++)
                if(Items[i]->RPids[j] == id)
                    return true;
        return false;
    }
    bool findEA(int& i, int& j, const int id) const {
        for(i=0; i<getCount(); i++)
            for(j=0; j<Items[i]->EAids.getCount(); j++)
                if(Items[i]->EAids[j] == id)
                    return true;
        return false;
    }
};

//clase Fiber MOS Model
class TFiberMOSModel {
public:
        //------------------------------------------------------------------
        //LISTAS DE OBJETOS:

        //lista de áreas de exclusión
        TExclusionAreaList EAL;
        //lista de robots posicionadores
        TRoboticPositionerList RPL;

        //------------------------------------------------------------------
        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjunto de propiedades de instancia
        //en formato asignaciones
        AnsiString getInstanceText(void) const;
        void setInstanceText(const AnsiString&);

        //------------------------------------------------------------------
        //MÉTODOS ESTÁTICOS:

        //lee una instancia de Fiber MOS Model
        //en una cadena de texto en formato de asignaciones
        static void  readInstance(TFiberMOSModel *MP,
                const AnsiString &S, int &i);

        //------------------------------------------------------------------
        //MÉTODOS DE CONSTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

        //construye un Fiber MOS Model
        TFiberMOSModel(void);

        //clona un Fiber MOS Model
        void Clone(const TFiberMOSModel*);

        //construye un clon de un Fiber MOS Model
        TFiberMOSModel(const TFiberMOSModel*);

        //destruye un Fiber MOS Model
        ~TFiberMOSModel();

        //MÉTODOS DE DESTRUCCION DE OBJETOS:

        //destruye todos los objetos de la lista
        void Destroy(void);

        //WARNING: inherited methods must be redefined withidentical name
        //for avoid errors.

        //MÉTODOS PARA CONTRUIR Y DESTRUIR POSICIONADORES:

        //contruye una colmena cuadrada y asimilala situación
        void buildSquareHive(void);
/*        //borra los posicionadores seleccionados y asimila la situación
        void DeleteSelected(void);*/

/*        //MÉTODOS DE TRANSFORMACIÓN DE COORDENADAS:

        //transforma una lista de puntos objetivo dados en coordenadas cartesianas en S0,
        //en una lista de puntos objetivo dados en posiciones angulares de los ejes en pasos de sus posicionadores adscritos
        //los puntos objetivo deben estar en el dominio de sus posic. adscritos (en sentido laxo, teniendo en cuenta el error num)
        void S0recToAmgularPositions(TTargetPointAngPosList& TPAPL,
                TTargetPoinRecList& TPRL);
  */
        //MÉTODOS DE ASIMILACIÓN:

        //asimila la configuración de las listas de objetos
        void assimilate(void);

        //MÉTODOS PARA DETERMINAR SI HAY COLISIONES:

        //determina si algún brazo de algún posicionador
        //colisiona con el brazo de algún posicionador adyacente
        bool thereIsCollision(void);

        //imprime los elementos colisionados (EAs y RPs) en una lista de listas
        void getCollidedText(string& str);

/*        //MÉTODOS RANDOMIZADO DE LOS EJES:

        //lleva los ejes de los posicionadores a posiciones aleatorias
        //con distribución uniforme en sus dominios
        //en las que no colisionan entre si
        void randomizeWithoutCollision(void);

        //MÉTODOS DE RANDOMIZADO DE P3:

        //lleva el punto P3 de los posicionadores a posiciones aleatorias
        //con distribución uniforme en su dominio
        //en las que no colisionan entre si
        void randomizeP3WithoutCollision(void);*/
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // FiberMOSModel_H
