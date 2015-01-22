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

#include "FiberMOSModel1.h"
#include "TextFile.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//CONJUNTOS DE PROPIEDADES EN MODO TEXTO:

AnsiString TFiberMOSModel1::getInstanceText(void) const
{
        AnsiString S;

//        S += AnsiString("RoboticPositionerListInstance:\r\n");
  //      S += StrIndent(RPL.getInstanceText())+AnsiString("\r\n");

        return S;
}
void TFiberMOSModel1::setInstanceText(const AnsiString& S)
{
        try {
                //contruye un clon de un Fiber MOS Model
                TFiberMOSModel1 aux(this);
                TFiberMOSModel1 *FMM = &aux;
                //lee la instancia mediante el clon
                int i = 1;
                ReadInstance(FMM, S, i);

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

//###########################################################################
//MÉTODOS PÚBLICOS:
//###########################################################################

//------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//lee una instancia de Fiber MOS Model
//en una cadena de texto en formato de asignaciones
void  TFiberMOSModel1::ReadInstance(TFiberMOSModel1 *FMM,
        const AnsiString &S, int &i)
{
        //el puntero FMM debe apuntar a un Fiber MOS Model construído
        if(FMM == NULL)
                throw EImproperArgument("pointer FMM should point to built Fiber MOS Model");

        //lee la instancia en la lista de RPs
        TRoboticPositionerList1 *RPL = &FMM->RPL;
        TRoboticPositionerList1::ReadInstance(RPL, S, i);
}

//MÉTODOS DE CONSTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

//construye un Fiber MOS Model
TFiberMOSModel1::TFiberMOSModel1(void) :
        //contruye las listas de objetos
        RPL(),
        EAL()
{
}

//clona un Fiber MOS Model
void TFiberMOSModel1::Clone(const TFiberMOSModel1 *FMM)
{
        //el puntero FMM debería apuntar a un Fiber MOS Model construído
        if(FMM == NULL)
                throw EImproperArgument("pointer FMM should point to built Fiber MOS Model");

        //clona las listas de objetos
        RPL.Clone(&FMM->RPL);
        EAL.Clone(&FMM->EAL);
}

//construye un clon de un Fiber MOS Model
TFiberMOSModel1::TFiberMOSModel1(const TFiberMOSModel1 *FMM)
{
        //el puntero FMM debería apuntar a un Fiber MOS Model construído
        if(FMM == NULL)
                throw EImproperArgument("pointer FMM should point to built Fiber MOS Model");

        //copia todas las propiedades
        Clone(FMM);
}

//destruye un Fiber MOS Model
TFiberMOSModel1::~TFiberMOSModel1()
{
        //destruye los robots posicionadores
        EAL.Destroy();
        //destruye las áreas de exclusión
        RPL.Destroy();
}

//MÉTODOS DE DESTRUCCION DE ÁREAS DE EXCLUSIÓN:

//destruye todos los objetos de la lista
void TFiberMOSModel1::Destroy(void)
{
        //destruye los robots posicionadores
        EAL.Destroy();
        //destruye las áreas de exclusión
        RPL.Destroy();
}
//destruye los objetos seleccionadas de la lista
int TFiberMOSModel1::DestroySelected(void)
{
        int Count = EAL.DestroySelected();
        Count += RPL.DestroySelected();

        return Count;
}

//MÉTODOS PARA CONTRUIR Y DESTRUIR POSICIONADORES:

//contruye una colmena cuadrada y asimilala situación
void TFiberMOSModel1::BuildSquareHive(void)
{
        //inicializa el contador de identificadores
        int CountId = 1;
        //contruye una colmena cuadrada por defecto
        RPL.BuildSquareHive(CountId);
        //asimila la nueva configuración
        Assimilate();
}
//borra los posicionadores seleccionados y asimilala situación
void TFiberMOSModel1::DeleteSelected(void)
{
        //destruye los posicionadores seleccionados
        RPL.DestroySelected();
        //asimila la nueva configuración
        Assimilate();
}

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
void TFiberMOSModel1::Assimilate()
{
        RPL.Assimilate();
        EAL.Assimilate(RPL);
}

//MÉTODOS PARA DETERMINAR SI HAY COLISIONES:

//determina si algún brazo de algún posicionador
//colisiona con el brazo de algún posicionador adyacente
bool TFiberMOSModel1::ThereIsCollision(void)
{
        //si los posicionadores colisionan entre si o contra las áreas de exclusión
        if(RPL.ThereIsCollision() || EAL.ThereIsCollision())
                return true; //indica que si hay colisión

        return false; //indica que no hay colisión
}
//busca los posicionadores de la lista cuyo brazo colisiona con
//el brazo de algún otro posicionador adyacente
void TFiberMOSModel1::SearchCollinding(TVector<int> &indices)
{

}
//obtiene los conjuntos de posicionadores en colisión en la exposición indicada
void TFiberMOSModel1::GetCollisionClusterList(TPointersList<TItemsList<TRoboticPositioner*> > &CCL)
{
}

//RANDOMIZADO DE LOS EJES:

//lleva los ejes de los posicionadores a posiciones aleatorias
//con distribución uniforme en sus dominios
//en las que no colisionan entre si
void TFiberMOSModel1::RandomizeWithoutCollision(void)
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
//lleva los ejes de los posicionadores seleccionados a
//posiciones aleatorias con distribución uniforme en
//sus dominios en las que no colisionan entre si
int TFiberMOSModel1::RandomizeWithoutCollisionSelected(void)
{
/*        TRoboticPositioner *RP;
        bool collision;
        int j;
        int count = 0;

        //para cada posicionador de la lista de osicionadores
        for(int i=0; i<Count; i++) {
                collision = false;
                do {
                        //apunta el posicionador indicado para facilitar su acceso
                        RP = Items[i];
                        if(RP->Actuator->Selected) {
                                //lleva randomiza los ejes del posicionador
                                RP->Actuator->Randomizep_1();
                                RP->Actuator->Arm->Randomizep___3();
                                //determina si hay colisión
                                collision = RP->Actuator->ThereIsCollisionWithPendingAdjacent();
                                //si no hay colisión
                                if(!collision)
                                        //contabiliza el posicionador seleccionado
                                        count++;
                        }
                //mientras colisione con algún adyacente
                } while(collision);
        }

        return count;*/
}

//RANDOMIZADO DE P3:

//lleva el punto P3 de los posicionadores a posiciones aleatorias
//con distribución uniforme en su dominio
//en las que no colisionan entre si
void TFiberMOSModel1::RandomizeP3WithoutCollision(void)
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
//lleva el punto P3 de los posicionadores seleccioandos a
//posiciones aleatorias con distribución uniforme en su dominio
//en las que no colisionan entre si
int TFiberMOSModel1::RandomizeP3WithoutCollisionSelected(void)
{
/*        TRoboticPositioner *RP;
        bool collision;
        int j;
        int count = 0;

        //para cada posicionador de la lista de osicionadores
        for(int i=0; i<Count; i++) {
                collision = false;
                do {
                        //apunta el posicionador indicado para facilitar su acceso
                        RP = Items[i];
                        if(RP->Actuator->Selected) {
                                //randomiza el punto P3
                                RP->Actuator->RandomizeP3();
                                //determina si hay colisión
                                collision = RP->Actuator->ThereIsCollisionWithPendingAdjacent();
                                //si no hay colisión
                                if(!collision)
                                        //contabiliza el posicionador seleccionado
                                        count++;
                        }
                //mientras colisione con algún adyacente
                } while(collision);
        }

        return count;*/
}

/*//---------------------------------------------------------------------------
//MÉTODOS DE ARCHIVO:

//escribe la instancia de la lista de robots posicionadores
//en un directorio
void TFiberMOSModel1::WriteRPLToDir(const AnsiString& Dir)
{
        try {
                //escribe la instancia en el archivo 'Instance.txt'
                StrWriteToFile(Dir+"\\Instance.txt", RPL.InstanceText);

                //por cada posicionador de la lista
                for(int i=0; i<RPL.Count; i++) {
                        //apunta el posicionador indicado para facilitar su acceso
                        TRoboticPositioner *RP = RPL[i];

                        //construye el nombre del subdirectorio que debe contener la instancia del posicionador
                        AnsiString SubDir = Dir+"\\RoboticPositioner"+RP->Actuator->Id;

                        //archiva el contorno del brazo
                        StrWriteToFile(SubDir+"\\Contour____.txt", RP->Actuator->Arm->Contour____.ColumnText);

                        //archiva el contorno de la barrera
                        StrWriteToFile(SubDir+"\\Contour_.txt", RP->Actuator->Barrier->Contour_.ColumnText);

                        //archiva la instancia del posicionador
                        StrWriteToFile(SubDir+"\\Instance.txt", RP->InstanceText);
                }

                //archiva la lista de orígenes de coordenadas de la lista de posicionadores
                StrWriteToFile(Dir+"\\RoboticPositionerOriginsTable.txt", TActuator::GetOriginsLabelsRow()+"\r\n"+RPL.OriginsTableText);

        } catch(...) {
                throw;
        }
}
//lee una instancia de la lista de robots posicionadores
//de un directorio
void TFiberMOSModel1::ReadRPLFromDir(const AnsiString& Dir)
{
        try {
                //contruye una lista tampón
                TRoboticPositionerList _RPL;

                AnsiString S;

                //lee y asigna la lista de orígenes de coordenadas de la lista de posicionadores
                StrReadFromFile(S, Dir+"\\RoboticPositionerOriginsTable.txt");
                _RPL.OriginsTableText = S;

                //por cada posicionador de la lista
                for(int i=0; i<_RPL.Count; i++) {
                        //apunta el posicionador indicado para facilitar su acceso
                        TRoboticPositioner *RP = _RPL[i];

                        //construye el nombre del subdirectorio que contiene la instancia del posicionador
                        AnsiString SubDir = Dir+"\\RoboticPositioner"+RP->Actuator->IdText;

                        //lee y asigna el contorno del brazo
                        StrReadFromFile(S, SubDir+"\\Contour____.txt");
                        RP->Actuator->Arm->Contour____ColumnText = S;

                        //lee y asigna el contorno de la barrera
                        StrReadFromFile(S, SubDir+"\\Contour_.txt");
                        RP->Actuator->Barrier->Contour_ColumnText = S;

                        //lee y asigna la instancia del posicionador
                        StrReadFromFile(S, SubDir+"\\Instance.txt");
                        RP->InstanceText = S;
                }

                //lee y asigna la instancia de la lista de posicionadores
                StrReadFromFile(S, Dir+"\\Instance.txt");
                _RPL.InstanceText = S;

                //clona la lista tampón
                RPL.Clone(&_RPL);
                //asimila los parámetros de dimensionamiento
                RPL.Assimilate();

                //Los robots posicionadores de la lista tampón no deben
                //ser destruidas porque se trata de una lista de punteros
                //y no de una lista basada en punteros.

        } catch(...) {
                throw;
        }
}

//escribe la instancia de la lista de áreas de exclusión
//en un directorio
void TFiberMOSModel1::WriteEALToDir(const AnsiString& Dir)
{
        try {
                //por cada área de exclusión de la lista
                for(int i=0; i<EAL.Count; i++) {
                        //apunta el área de exclusión indicado para facilitar su acceso
                        TExclusionArea *EA = EAL[i];

                        //construye el nombre del subdirectorio que debe contener la instancia del área de exclusión
                        AnsiString SubDir = Dir+"\\ExclusionArea"+EA->Id;

                        //archiva el contorno de la barrera
                        StrWriteToFile(SubDir+"\\Contour_.txt", EA->Barrier.Contour_.ColumnText);

                        //archiva la instancia del área de exclusión
                        StrWriteToFile(SubDir+"\\Instance.txt", EA->InstanceText);
                }

                //archiva la lista de orígenes de coordenadas de la lista de área de exclusiónes
                StrWriteToFile(Dir+"\\ExclusionAreaOriginsTable.txt", TExclusionArea::GetOriginsLabelsRow()+"\r\n"+EAL.OriginsTableText);

        } catch(...) {
                throw;
        }
}
//lee una instancia de la lista de áreas de exclusión
//de un directorio
void TFiberMOSModel1::ReadEALFromDir(const AnsiString& Dir)
{
        try {
                //contruye una lista tampón
                TExclusionAreaList _EAL;

                AnsiString S;

                //lee y asigna la tabla de orígenes de coordenadas
                StrReadFromFile(S, Dir+"\\ExclusionAreaOriginsTable.txt");
                _EAL.OriginsTableText = S;

                //por cada área de exclusión de la lista
                for(int i=0; i<_EAL.Count; i++) {
                        //apunta el área de exclusión indicada para facilitar su acceso
                        TExclusionArea *EA = _EAL[i];

                        //construye el nombre del subdirectorio que contiene la instancia del área de exclusión
                        AnsiString SubDir = Dir+"\\ExclusionArea"+EA->Id;

                        //lee y asigna el contorno de la barrera
                        StrReadFromFile(S, SubDir+"\\Contour_.txt");
                        EA->Barrier.Contour_ColumnText = S;

                        //lee y asigna la instancia del área de exclusión
                        StrReadFromFile(S, SubDir+"\\Instance.txt");
                        EA->InstanceText = S;
                }

                //clona la lista tampón
                EAL.Clone(&_EAL);
                //asimila los parámetros de dimensionamiento
                EAL.Assimilate(RPL);

                //Las áreas de exclusión de la lista tampón no deben
                //ser destruidas porque se trata de una lista de punteros
                //y no de una lista basada en punteros.

        } catch(...) {
                throw;
        }
}

//escribe la instancia del Fiber MOS Model en un directorio
void TFiberMOSModel1::WriteInstanceToDir(const AnsiString& Dir)
{
        try {
                //escribe las instancias de las listas de objetos en el directorio
                WriteInstanceToDir(Dir);
                WriteInstanceToDir(Dir);

                //escribe las demás propiedades de la instancia
                //en el archivo 'Instance.txt'
                StrWriteToFile(Dir+"\\Instance.txt", InstanceText);

        } catch(...) {
                throw;
        }
}
//lee una instancia del Fiber MOS Model de un directorio
void TFiberMOSModel1::ReadInstanceFromDir(const AnsiString& Dir)
{
        try {
                //lee las instancias de las listas de objetos del directorio
                ReadRPLFromDir(Dir);
                ReadEALFromDir(Dir);

                //lee las demás propiedades de la instancia
                //del archivo 'Instance.txt'
                AnsiString S;
                StrReadFromFile(S, Dir+"\\Instance.txt");
                InstanceText = S;

                //asimila las propiedades
                Assimilate();

        } catch(...) {
                throw;
        }
}
*/
//MÉTODOS GRÁFICOS:
/*#
//dibuja los posicionadores y las áreas de exclusión
//en un trazador de formas
void TFiberMOSModel1::Paint(TPloterShapes* PS)
{
        //el puntero PS debería apuntar a un trazador de formas construído
        if(PS == NULL)
                throw EImproperArgument("pointer PS should point to built ploter shapes");

        RPL.Paint(PS);
        EAL.Paint(PS);
}
*/
//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

