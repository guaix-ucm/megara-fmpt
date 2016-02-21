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

#ifndef FIBERMOSMODEL1_H
#define FIBERMOSMODEL1_H

#include "ExclusionAreaList.h"
#include "ExclusionArea.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------

//La clase TFiberMOSModel12:
//      Permite la detección de colisiones con las áreas de exclusión fijas.

//clase Fiber MOS Model
class TFiberMOSModel1 {
public:
        //------------------------------------------------------------------
        //LISTAS DE OBJETOS:

        //lista de robots posicionadores
        TRoboticPositionerList RPL;
        //lista de áreas de exclusión
        TExclusionAreaList EAL;

        //------------------------------------------------------------------
        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjunto de propiedades de instancia
        //en formato asignaciones
        AnsiString getInstanceText(void) const;
        void setInstanceText(const AnsiString&);

        //##################################################################
        //MÉTODOS PÚBLICOS:
        //##################################################################

        //------------------------------------------------------------------
        //MÉTODOS ESTÁTICOS:

        //lee una instancia de Fiber MOS Model
        //en una cadena de texto en formato de asignaciones
        static void  readInstance(TFiberMOSModel1 *MP,
                const AnsiString &S, int &i);

        //------------------------------------------------------------------
        //MÉTODOS DE CONSTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

        //construye un Fiber MOS Model
        TFiberMOSModel1(void);

        //clona un Fiber MOS Model
        void Clone(const TFiberMOSModel1*);

        //WARNING: inherited methods must be redefined withidentical name
        //for avoid errors.

        //construye un clon de un Fiber MOS Model
        TFiberMOSModel1(const TFiberMOSModel1*);

        //destruye un Fiber MOS Model
        ~TFiberMOSModel1();

        //MÉTODOS DE DESTRUCCION DE OBJETOS:

        //destruye todos los objetos de la lista
        void Destroy(void);

        //WARNING: inherited methods must be redefined withidentical name
        //for avoid errors.

        //MÉTODOS PARA CONTRUIR Y DESTRUIR POSICIONADORES:

        //contruye una colmena cuadrada y asimilala situación
        void buildSquareHive(void);
/*        //borra los posicionadores seleccionados y asimilala situación
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

        //MÉTODOS RANDOMIZADO DE LOS EJES:

        //lleva los ejes de los posicionadores a posiciones aleatorias
        //con distribución uniforme en sus dominios
        //en las que no colisionan entre si
        void randomizeWithoutCollision(void);

        //MÉTODOS DE RANDOMIZADO DE P3:

        //lleva el punto P3 de los posicionadores a posiciones aleatorias
        //con distribución uniforme en su dominio
        //en las que no colisionan entre si
        void randomizeP3WithoutCollision(void);

/*        //------------------------------------------------------------------
        //MÉTODOS DE ARCHIVO:

        //escribe la instancia de la lista de robots posicionadores
        //en un directorio
        void writeRPLToDir(const AnsiString& Dir);
        //lee una instancia de la lista de robots posicionadores
        //de un directorio
        void readRPLFromDir(const AnsiString& Dir);

        //escribe la instancia de la lista de áreas de exclusión
        //en un directorio
        void writeEALToDir(const AnsiString& Dir);
        //lee una instancia de la lista de áreas de exclusión
        //de un directorio
        void readEALFromDir(const AnsiString& Dir);

        //escribe la instancia del Fiber MOS Model en un directorio
        void writeInstanceToDir(const AnsiString& Dir);
        //lee una instancia del Fiber MOS Model de un directorio
        void readInstanceFromDir(const AnsiString& Dir);

        //Estos métodos de archivo no deben ser incluidos en la FMPT
        //porque la FMPT no debe tener acceso al sistema de archivos.
        //En su lugar se definirán funciones similares formando parte
        //de la FMPT Stand Alone Application:
        //      //escribe la instancia de una lista de robots posicionadores
        //      //en un directorio
        //      void writeInstanceToDir(const AnsiString& Dir, const TRoboticPositionerList&);
        //      //lee una instancia de la lista de robots posicionadores
        //      //de un directorio
        //      void readInstanceFromDir(TRoboticPositionerList&, const AnsiString& Dir);
        //      //escribe la instancia de una lista de áreas de exclusión
        //      //en un directorio
        //      void writeInstanceToDir(const AnsiString& Dir, const TExclusionAreaList&);
        //      //lee una instancia de la lista de áreas de exclusión
        //      //de un directorio
        //      void readInstanceFromDir(TExclusionAreaList&, const AnsiString& Dir,
        //              const TRoboticPositionerList&);
        //      //escribe la instancia de un Fiber MOS Model en un directorio
        //      void writeInstanceToDir(const AnsiString& Dir, const TMultifiberPositioner1*);
        //      //lee una instancia del Fiber MOS Model de un directorio
        //      void readInstanceFromDir(TMultifiberPositioner1*, const AnsiString& Dir);
*/
        //MÉTODOS GRÁFICOS:

        //dibuja los posicionadores y las áreas de exclusión
        //en un trazador de formas
        //#void paint(TPloterShapes* PS);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // FIBERMOSMODEL1_H
