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
//Archivo: TAllocationList.cpp
//Contenido: class allocation list
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "AllocationList.h"
#include "Strings.h"

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres del exposicionamiento
namespace Positioning {

//---------------------------------------------------------------------------

AnsiString TAllocationList::geTAllocationsText(void)
{
        AnsiString S;

        AnsiString aux;
        TAllocation *TP;

        //para cada asignación de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el objeto indicado para facilitar su acceso
                TP = Items[i];

                //imprime el identificador
                aux = IntToStr(TP->getRP()->getActuator()->getId());
                while(aux.Length() < 8)
                        aux += char(32);

                //imprime x
                aux += FloatToStr(TP->PP.x);
                while(aux.Length() < 32)
                        aux += char(32);

                //imprime y
                aux += FloatToStr(TP->PP.y);
                aux += "\r\n";

                S += aux;
        }

        return S;
}
void TAllocationList::seTAllocationsText(AnsiString &S)
{
    try {
        //indica a la primera posición de la cadena
        int i = 1;
        //atraviesa las etiquetas
        TAllocation::TravelLabels(S, i);

        //lee la tabla en una variable tampón
        //TPointersList<TAllocation> TPL;
        //TPL.Read = TAllocation::ReadRow;
        //TPointersList<TAllocation>::ReadSeparated(&TPL, S, i);

        //No puede invocar a TPointersList<TAllocation>::ReadSeparated,
        //porque TAllocation no tiene un constructor por defecto, de modo que
        //se hará a pelo.

        //-----------------------------------------------------------------------------

        //el índice i debería indicar a alguna posición de la cadena de texto S
        if(i<0 || S.Length()+1<i)
            throw EImproperArgument("index i should indicate a posicion in the string text S");

        //estado de la máquina de estados
        //      0: esperando próximo elemento o fin de cadena
        //      1: fin de cadena encontrada y lista de elementos leida con éxito
        int status = 0;

        int Id;
        double x, y;
        TAllocation *Item;
        TSlideArray<TAllocation*> Items;

        //avanza el índice hasta el próximo caracter no separador
        StrTravelSeparatorsIfAny(S, i);

        do { //repite
            //si no ha llegado al final de la cadena
            if(i <= S.Length())
                try {
                    //lee los valores de las propiedades próximo elemento
                    TAllocation::ReadSeparated(Id, x, y, S, i);
                    //busca el RP en la RPL de la TPL
                    int j = getRoboticPositionerList()->SearchId(Id);
                    //si no ha encontrado el RP
                    if(j >= getRoboticPositionerList()->getCount())
                        //indica que no ha encontrado el RP indicado
                        throw EImproperFileLoadedValue(AnsiString("RP ")+IntToStr(Id)+AnsiString("not found"));
                    //apunta el RP encontrado
                    TRoboticPositioner *RP = getRoboticPositionerList()->Get(j);
                    //contruye el elemento adscrito al RP encontrado
                    Item = new TAllocation(RP, x, y);

                    //ERROR: si el RP tiene otro RP asignado, TAllocation lanzará una excepción EImproperArgument.
                    //Eso impedirá que puedan signarse nuevas PP tables, amenos que la TPL esté inicializada.

                    //añade el elemento
                    Items.AddLast(Item);
                    //avanza el índice hasta el próximo caracter no separador
                    StrTravelSeparators(S, i);
                    //si el índice indica a la posúltima posición de la cadena
                    if(i > S.Length())
                        status++; //indica que se ha leido la lista con éxito

                    //ADVERTENCIA: el elemento tiene que ser añadido a la lista tampón
                    //tan pronto como es contruido, para que pueda ser destruido
                    //en caso de error

                } catch(...) {
                    throw;
                }

            else //si ha llegado al final de la cadena
                //indica que ha llegado al final de la cadena
                status++;

        //mientras no se haya acabado la cadena y no se haya leido la lista con éxito
        } while(status < 1);

        //asigna la variable tampón:

        //reinicia la lista de puntos objetivo
        //copia las variables tampón
        Delete_();
        for(int j=0; j<Items.getCount(); j++)
            Add(Items[j]);
        Items.DelLast(Items.getCount());

    } catch(...) {
        //destruye los elementos tampón
        for(int i=0; i<Items.getCount(); i++)
            delete Items[i];
        throw;
    }
}

//---------------------------------------------------------------------------
//CONSTRUCCIÓN Y DESTRUCCIÓN:
//---------------------------------------------------------------------------

//construye una lista de puntos objetivo
//adscrita a una lista de RPs
TAllocationList::TAllocationList(TRoboticPositionerList *_RoboticPositionerList) :
//        TAllocations(1000, TAllocation::CompareIds)
        TItemsList<TAllocation*>(1000, TAllocation::CompareIds)
{
        //el puntero RoboticPositionerList debería apuntar a una lista de RPs contruida
        if(_RoboticPositionerList == NULL)
                throw EImproperArgument("pointer RoboticPositionerList should point to built RP list");

        //apunta el objeto externo
        __RoboticPositionerList = _RoboticPositionerList;
}

//borra los puntos objetivo y destruye la lista
TAllocationList::~TAllocationList()
{
        Delete_();
}

//---------------------------------------------------------------------------
//BÚSQUEDA:
//---------------------------------------------------------------------------

//busca el punto objetivo adscrito a un posicionador
int TAllocationList::searchAllocation(const TRoboticPositioner *RP) const
{
        //el puntero RP debe apuntar a un RP construido
        if(RP == NULL)
                throw EImproperArgument("pointer RP should point to built RP");

        int i = 0;
        while(i<getCount() && Items[i]->getRP()!=RP)
                i++;

        return i;
}
//busca el punto objetivo adscrito a un posicionador identificado
int TAllocationList::searchAllocation(int Id) const
{
        int i = 0;
        while(i<getCount() && Items[i]->getRP()->getActuator()->getId()!=Id)
                i++;

        return i;
}

//---------------------------------------------------------------------------
//OBJETOS INDICADOS:
//---------------------------------------------------------------------------

//añade un punto objetivo para el posicionador indicado de la lista
void TAllocationList::AddAllocation(int i)
{
        //el índice i debería indicar un posicionador de la lista RoboticPositionerList
        if(i<0 || getRoboticPositionerList()->getCount()<=i)
                throw EImproperArgument("index i should indicate a position in the list ListFibeRPositioners");

        //apunta el posicionador indicado para facilitar su acceso
        TRoboticPositioner *RP = getRoboticPositionerList()->Get(i);

        //busca el punto objetivo adscrito al posicionador
        i = searchAllocation(RP);

        //si el posicionador indicado i ya tiene un ponto objetivo adscrito
        if(i > getRoboticPositionerList()->getCount())
                //indica que el posicionador indicado i ya tiene adscrito un punto objetivo
                throw EImproperArgument("indexed positioner i already have assigned a allocation");

        //añade el punto objetivo para el posiciondor indicado
        //con ls coordenadas de su punto P3
        Add(new TAllocation(RP, RP->getActuator()->getArm()->getP3().x, RP->getActuator()->getArm()->getP3().y));
}
//borra el punto objetivo indicada de la lista
void TAllocationList::DeleteAllocation(int i)
{
        //el índice i debería indicar un punto objetivo de la lista
        if(i<0 || getCount()<=i)
                throw EImproperArgument("index i should indicate a allocation in the list");

        //destruye el punto objetivo
        delete Items[i];
        //borra la posición de la lista
        Delete(i);
}

//---------------------------------------------------------------------------
//OBJETOS SELECCIONADOS DE LA LISTA:
//---------------------------------------------------------------------------

//añade puntos objetivo con la posición del punto P3
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
int TAllocationList::AddP3Selected(void)
{
        TRoboticPositioner *RP;
        int j;
        int acum = 0;

        //para cada posicionador de la lista de posicionadores
        for(int i=0; i<getRoboticPositionerList()->getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = getRoboticPositionerList()->Get(i);

                //si el posicionador está seleccionado
                if(RP->getActuator()->Selected) {
                        //busca el posicionador en la lista de puntos objetivo
                        j = searchAllocation(RP->getActuator()->getId());
                        //si el posicionador ya tiene un punto objetivo asignado
                        if(j < getCount())
                                //asigna la posición del posicionador al punto objetivo
                                Items[j]->PP = RP->getActuator()->getArm()->getP3();
                        else //si el posicionador no tiene un punto objetivo asignado
                                //añade un punto objetivo con sus coordenadas actuales
                                Add(new TAllocation(RP, RP->getActuator()->getArm()->getP3()));
                        //contabiliza el posicionador seleccionado
                        acum++;
                }
        }

        return acum;
}

//destruye los puntos objetivo
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
int TAllocationList::DeleteSelected(void)
{
        int i = 0;
        TAllocation *TP;
        int acum = 0;

        //por cada punto objetivo de la lista
        while(i < getCount()) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];
                //si el posicionador adscrito está seleccionado
                if(TP->getRP()->getActuator()->Selected) {
                        //destruye el punto objetivo
                        DeleteAllocation(i);
                        //contabilizael posicionador adscrito seleccionado
                        acum++;
                } else  //si elposicionador adscrito no está seleccionado
                        i++; //indica al próximo punto objetivo
        }

        return acum;
}

//asigna el punto P3o a los puntos objetivo
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
int TAllocationList::SetP3oSelected(void)
{
        TAllocation *TP;
        int acum = 0;

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];
                //si el posicionador adscrito está seleccionado
                if(TP->getRP()->getActuator()->Selected) {
                        //asigna el P0 del posicionador adscrito al punto objetivo
                        TP->SetP3o();
                        //contabiliza el posicionador seleccionado
                        acum++;
                }
        }

        return acum;
}

//asigna el punto P3 a los puntos objetivo
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
int TAllocationList::SetP3Selected(void)
{
        TAllocation *TP;
        int acum = 0;

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo para facilitar su acceso
                TP = Items[i];
                //si el posicionador adscrito está seleccionado
                if(TP->getRP()->getActuator()->Selected) {
                        //asigna la posición actual del posicioandor
                        TP->SetP3();
                        //contabiliza el posicionador seleccionado
                        acum++;
                }
        }

        return acum;
}

//randomiza los puntos objetivo con distribución uniforme
//en el dominio del punto P3 de sus posicionadores adscrito
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
int TAllocationList::RandomizeSelected(void)
{
        TAllocation *TP;
        int acum = 0;

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];
                //si elposicionador adscrito está seleccionado
                if(TP->getRP()->getActuator()->Selected) {
                        //randomiza el punto en el dominio de su RP adscrito
                        Items[i]->RandomizePP();
                        //contabiliza el posicionador seleccionado
                        acum++;
                }
        }

        return acum;
}

//randomiza los puntos objetivo con distribución uniforme
//en el dominio del punto P3 de sus posicionadores adscritos
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
int TAllocationList::RandomizeWithoutCollisionSelected(void)
{
        TAllocation *TP;
        TRoboticPositioner *RP;
        int acum = 0;

        //vectores para guardar la configuración inicial
        TVector<char> Quantify_;
        TVector<char> Quantify__;
        TVector<double> theta_1;
        TVector<double> theta__3;

        //bandera de colisión detectada
        bool collision;

        //levanta las baderas de colisión de todos los posicionadores
        getRoboticPositionerList()->EnablePending();

        //ADVERTENCIA: no basta con levantar las banderas de colisión de
        //los posicionadores adscritos  los puntos objetivo de la lista
        //porque la colisión puede darse con posicionadores que no tengan
        //punto objetivo adscrito.

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];
                //apunta el posicionador adscrito para facilitar su acceso
                RP = TP->getRP();

                //sielposicionador adscrito está seleccionado
                if(RP->getActuator()->Selected) {
                        //guarda la configuración inicial del posicionador
                        Quantify_.Add(char(RP->getActuator()->getQuantify_()));
                        Quantify__.Add(char(RP->getActuator()->getArm()->getQuantify___()));
                        theta_1.Add(RP->getActuator()->gettheta_1());
                        theta__3.Add(RP->getActuator()->getArm()->gettheta___3());

                        //desactiva la cuantificación de los ejes
                        RP->getActuator()->setQuantify_(false);
                        RP->getActuator()->getArm()->setQuantify___(false);

                        //repite
                        do {
                                //randomiza el punto en el dominio de su RP adscrito
                                TP->RandomizePP();
                                //mueve el posicionador al punto objetivo
                                TP->MoveToPP();
                                //determina is hay colisión
                                collision = RP->getActuator()->ThereIsCollisionWithAdjacent();
                        //mientras el brazo del posicionador invada a sus adyacentes
                        } while(collision);

                        //contabiliza el posicionador adscrito seleccionado
                        acum++;
                }
        }

        //restaura las posiciones de los ejes de los posicionadores:

        //si hay posicionadores adscritosseleccionados
        if(acum > 0) {
                int j = 0; //índice a registros de configuración

                //por cada punto objetivo de la lista
                for(int i=0; i<getCount(); i++) {
                        //apunta el posicionador para faciltiar su acceso
                        RP = Items[i]->getRP();

                        //si el posicionador está seleccionado
                        if(RP->getActuator()->Selected) {
                                //restaura la posición de los ejes
                                RP->getActuator()->getArm()->settheta___3(theta__3[j]);
                                RP->getActuator()->settheta_1(theta_1[j]);
                                //restaura la cuantificación de los ejes
                                RP->getActuator()->getArm()->setQuantify___(bool(Quantify__[j]));
                                RP->getActuator()->setQuantify_(bool(Quantify_[j]));

                                //indica al prósimo registro de configuración
                                j++;
                        }
                }
        }

        return acum;
}

//asigna los puntos objetivo
//al punto P3 de sus posicionadores adscritos
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
//si algún punto objetivo no está en el dominio de
//su posicionador adscrito lanza EImproperCall
int TAllocationList::MoveToTargetP3Selected(void)
{
        TAllocation *TP;
        int acum = 0;

        //para cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];
                //si el posicionador adscrito está seleccionado
                if(TP->getRP()->getActuator()->Selected) {
                        //asigna el punto objetivo a su RP adscrito
                        TP->MoveToPP();
                        //contabiliza el posicionador seleccionado
                        acum++;
                }
        }

        return acum;
}

//---------------------------------------------------------------------------
//TODOS LOS OBJETOS DE LA LISTA:
//---------------------------------------------------------------------------

//añade puntos objetivo con la posición del punto P3
//de todos los posicionadores de la lista
void TAllocationList::AddP3(void)
{
        TRoboticPositioner *RP;
        int j;

        //paracadaposicionadordelalistadeposicionadores
        for(int i=0;i<getRoboticPositionerList()->getCount(); i++){
                //apunta el posicionador indicado para facilitar su acceso
                RP = getRoboticPositionerList()->Get(i);

                //busca el posicionador en la lista de puntos objetivo
                j = searchAllocation(RP->getActuator()->getId());
                //si el posicionador ya tiene un punto objetivo asignado
                if(j < getCount())
                        //asigna la posición del posicionador al punto objetivo
                        Items[j]->PP = RP->getActuator()->getArm()->getP3();
                else//si el posicionador no tiene un punto objetivo asignado
                        //añade un punto objetivo con sus coordenadas actuales
                        Add(new TAllocation(RP, RP->getActuator()->getArm()->getP3()));
        }
}

//destruye los puntos objetivo
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
void TAllocationList::Delete_(void)
{
        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++)
                //destruye el punto objetivo
                delete Items[i];

        //borra las posiciones de la lista
        Clear();
}

//asigna el punto PO3 a los puntos objetivo
//de todos los posicionadores de la lista
void TAllocationList::SetPO3(void)
{
        //todos los posicionadores adscritos deben estar en la lista
        TVector<int> indices;
        SearchMissingRPs(indices);
        if(indices.getCount() > 0)
                throw EImproperCall("there are missing RPs in the allocation assignations list");

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++)
                //asigna el P3o del posicionador adscrito al punto objetivo
                Items[i]->SetP3o();
}

//asigna el punto P3 a los puntos objetivo
//de todos los posicionadores de la lista
void TAllocationList::SetP3(void)
{
        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++)
                //asigna la posición actual del posicioandor adscrito
                Items[i]->SetP3();
}

//randomiza los puntos objetivo con distribución uniforme
//en el dominio del punto P3 de sus posicionadores adscrito
//de todos los posicionadores de la lista
void TAllocationList::Randomize(void)
{
        //todos los posicionadores de fibra adscritos a
        //los puntos objetivo deben estar en la lista
        TVector<int> indices;
        SearchMissingRPs(indices);
        if(indices.getCount() > 0)
                throw EImproperCall("there are missing RPs in the assignaments list");

        //por cada asignación de la lista
        for(int i=0; i<getCount(); i++)
                //randomiza el punto en el dominio de su RP adscrito
                Items[i]->RandomizePP();
}

//randomiza los puntos objetivo con distribución uniforme
//en el dominio del punto P3 de sus posicionadores adscritos
//de todos los posicionadores de la lista
void TAllocationList::RandomizeWithoutCollision(void)
{
        //todos los posicionadores de fibra adscritos a
        //los puntos objetivo deben estar en la lista
        TVector<int> indices;
        SearchMissingRPs(indices);
        if(indices.getCount() > 0)
                throw EImproperCall("there are missing RPs in the assignaments list");

        TAllocation *TP;
        TRoboticPositioner *RP;

        //vectores para guardar la configuración inicial
        TVector<char> Quantify_;
        TVector<char> Quantify__;
        TVector<double> theta_1;
        TVector<double> theta__3;

        //bandera de colisión detectada
        bool collision;

        //levanta las baderas de colisión de todos los posicionadores
        getRoboticPositionerList()->EnablePending();

        //ADVERTENCIA: no basta con levantar las banderas de colisión de
        //los posicionadores adscritos  los puntos objetivo de la lista
        //porque la colisión puede darse con posicionadores que no tengan
        //punto objetivo adscrito.

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];
                //apunta el posicionador adscrito para facilitar su acceso
                RP = TP->getRP();

                //guarda la configuración inicial del posicionador
                Quantify_.Add(char(RP->getActuator()->getQuantify_()));
                Quantify__.Add(char(RP->getActuator()->getArm()->getQuantify___()));
                theta_1.Add(RP->getActuator()->gettheta_1());
                theta__3.Add(RP->getActuator()->getArm()->gettheta___3());

                //desactiva la cuantificación de los ejes
                RP->getActuator()->setQuantify_(false);
                RP->getActuator()->getArm()->setQuantify___(false);

                //repite
                do {
                        //randomiza el punto en el dominio de su RP adscrito
                        TP->RandomizePP();
                        //mueve el posicionador al punto objetivo
                        TP->MoveToPP();
                        //determina si hay colisión
                        collision = RP->getActuator()->ThereIsCollisionWithAdjacent();
                //mientras el brazo del posicionador invada a sus adyacentes
                } while(collision);
        }

        //restaura las posiciones de los ejes de los posicionadores:

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para faciltiar su acceso
                RP = Items[i]->getRP();

                //restaura la posición de los ejes
                RP->getActuator()->getArm()->settheta___3(theta__3[i]);
                RP->getActuator()->settheta_1(theta_1[i]);

                //restaura la cuantificación de los ejes
                RP->getActuator()->getArm()->setQuantify___(bool(Quantify__[i]));
                RP->getActuator()->setQuantify_(bool(Quantify_[i]));
        }
}

//asigna los puntos objetivo
//al punto P3 de sus posicionadores adscritos
//si algún punto objetivo no está en el dominio de
//su posicionador adscrito lanza EImproperCall
//de todos los posicionadores de la lista
void TAllocationList::MoveToTargetP3(void)
{
        //todos los puntos objetivo deben estar en
        //el dominio de su posicionador adscrito
        TVector<int> indices;
        SearchOutDomineTAllocations(indices);
        if(indices.getCount() > 0) {
            //build a list with the pointer to the RP whose TAllocationis out of their scope
            TItemsList<TRoboticPositioner*> RPL;
            RPL.Print = TRoboticPositioner::PrintId;
            for(int i=0; i<indices.getCount(); i++) {
                TAllocation *TP = Items[indices[i]];
                RPL.Add(TP->getRP());
            }

            //indicates the error
            throw EImproperCall(AnsiString("there is some allocation out of the scope of their atached RP ")+RPL.getText());
        }

        //para cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++)
                //asigna el punto objetivo a su RP adscrito
                Items[i]->MoveToPP();
}

//---------------------------------------------------------------------------
//VALIDACIÓN:
//---------------------------------------------------------------------------

//busca los puntos objetivo adscritos a posicionadores repetidos
void TAllocationList::SearchRepeatedRPs(TVector<int> &indices)
{
        //inicializa la ista de índices
        indices.Clear();

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++)
                //por cada punto objetivo posterior de la lista
                for(int j=i+1; j<getCount(); j++)
                        //si el posicionador adscrito es el mismo
                        if(Items[i]->getRP() == Items[j]->getRP())
                                indices.Add(j); //añade el índice al punto objetivo
}

//busca los puntos objetivo adscritos a posicionadores ausentes
//en la lista de posicionadores RoboticPositionerList
void TAllocationList::SearchMissingRPs(TVector<int> &indices)
{
        TAllocation *TP;
        int j;

        //reinicial el vector de índices en congruencia
        //con la situación de partida
        indices.Clear();

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];

                //busca el posicionador adscrito en la lista de posicionadores
                j = getRoboticPositionerList()->Search(TP->getRP());

                //si el posicionador no está en la lista de posicionadores
                if(j >= getRoboticPositionerList()->getCount())
                        //añade el índice a la lista de omitidos
                        indices.Add(i);
        }
}

//busca los puntos objetivo que están fuera del dominio
//de sus posicionadores adscritos
void TAllocationList::SearchOutDomineTAllocations(TVector<int> &indices)
{
        TAllocation *TP;
        TRoboticPositioner *RP;

        //reinicial el vector de índices en congruencia
        //con la situación de partida
        indices.Clear();

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];

                //apunta el posicionador adscrito para facilitar su acceso
                RP = TP->getRP();

                //traduce el punto a coordenadas angulares de los ejes
                //y determina si está en el dominio del posicionador
                double theta_1, theta___3;
                bool isindomine = RP->getActuator()->AnglesToGoP3(theta_1, theta___3, TP->PP.x, TP->PP.y);

                //si el punto objetivo está fuera del dominio del posicionador
                if(!isindomine)
                        //añade el índice a la lista
                        indices.Add(i);
        }
}

//Determina la invalidez de una lista de puntos objetivo
//para ser programada.
//Valores de retorno:
//      0: lista de puntos objetivo válida;
//      1: posicionadores adscritos de los puntos objetivo indicados
// ausentes;
//      2: puntos objetivo indicados fuera del dominio de
//         sus posicionadores adscritos.
int TAllocationList::Invalid(TVector<int> &indices)
{
        //se supone que ninguno de los posicionadores adscritos
        //está repetido, y que la congruencia de repetición
        //se logra controlando las adiciones

        //busca posicionadores adscritos ausentes
        SearchMissingRPs(indices);
        if(indices.getCount() > 0) //si ha encontrado algún posicionador ausente
                return 1; //indica que ha encontrado posicionadores ausentes

        //busca punto objetivo fuera del dominio de su posicionadores adscritos
        SearchOutDomineTAllocations(indices);
        if(indices.getCount() > 0) //si ha encontrado algún punto fuera del dominio
                return 2; //indica que ha encontrado puntos fuera del dominio

        return 0; //indica que el punto objetivo de puntos finales es válida
}

//---------------------------------------------------------------------------
//SEGREGACIÓN:
//---------------------------------------------------------------------------

//segrega los posicionadores de los puntos objetivo en dos listas:
//      internos: con el brazo dentro del área de seguridad;
//      externos: con el brazo fuera del área de seguridad;
//si algún punto objetivo está fuera del dominio de
//su posicionador adscrito:
//      lanza EImproperCall
void TAllocationList::SegregateInOut(TRoboticPositionerList &Inners,
        TRoboticPositionerList &Outsiders)
{
        //COMPRUEBA QUE TODOS LOS PUNTOS OBJETIVO ESTÁN EN
        //EL DOMINIO DE SUS POSICIONADORES ADSCRITOS:

    //busca los puntos objetivo que están fuera del dominio
        //de sus posicionadores adscritos
        TVector<int> indices;
        SearchOutDomineTAllocations(indices);
    //Si ha encontrado algún punto objetivo
        if(indices.getCount() > 0) //si ha encontrado algún punto fuera del dominio
                throw EImproperCall(AnsiString("there are allocations out of domain his atached RP ")+indices.getText());

        //CALCULA LOS ÁNGULOS PARA POSICIONAR CADA FIBRA EN SU PUNTO OBJETIVO
        //Y CLASIFICA EL PUNTO OBJETIVO EN FUNCIÓN DE LA EXTENSIÓN DEL BRAZO:

        TAllocation *TP;
        TRoboticPositioner *RP;
        double theta_1, theta___3;
        bool isindomain;

        //reinicializa las listas en congruencia con su estado inicial
        Inners.Clear();
        Outsiders.Clear();

        //para cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];

                //apunta el posicionador adscrito
                //para facilitar su acceso
                RP = TP->getRP();

                //traduce el punto a coordenadas angulares de los ejes
                //y determina si el punto objetivo está en el dominio del posicionador
                isindomain = RP->getActuator()->AnglesToGoP3(theta_1, theta___3, TP->PP.x, TP->PP.y);

                //el punto objetivo no puede estar fuera del dominio de su posicionador adscrito
                if(!isindomain)
                        throw EImpossibleError("lateral effects");

                //si el punto objetivo está en el área de seguridad
                if(RP->getActuator()->theta___3IsInSafeArea(theta___3))
                        //apunta el controlador adscrito en la lista de internos
                        Inners.Add(RP);
                else //si está fuera del área de seguridad
                        //apunta el posicionador adscrito en la lista de externos
                        Outsiders.Add(RP);
        }
}

//---------------------------------------------------------------------------
//ALMACENAMIENTO Y RECUPERACIÓN DE POSICIONES:
//---------------------------------------------------------------------------

//apila las posiciones de los posicionadores adscritos
void TAllocationList::PushPositions(void)
{
        TRoboticPositioner *RP;

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador adscrito inicado para facilitar su acceso
                RP = Items[i]->getRP();

                //apila la posición de sus ejes
                RP->getActuator()->Pushtheta_1();
                RP->getActuator()->getArm()->Pushtheta___3();
        }
}
//restaura las posiciones de los posicionadores adscritos
void TAllocationList::RestorePositions(void)
{
        TRoboticPositioner *RP;

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador adscrito indicado para facilitar su acceso
                RP = Items[i]->getRP();

                //restaura y desempila la posición de sus ejes
                RP->getActuator()->getArm()->Restoretheta___3();
                RP->getActuator()->Restoretheta_1();
        }
}
//desempila las posiciones de los posicionadores adscritos
void TAllocationList::PopPositions(void)
{
        TRoboticPositioner *RP;

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador adscrito indicado para facilitar su acceso
                RP = Items[i]->getRP();

                //desempila la posición de sus ejes
                RP->getActuator()->getArm()->Poptheta___3();
                RP->getActuator()->Poptheta_1();
        }
}
//restaura y desempila las posiciones de los posicionadores adscritos
void TAllocationList::RestoreAndPopPositions(void)
{
        TRoboticPositioner *RP;

        //por cada punto objtivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador adscrito indicado para facilitar su acceso
                RP = Items[i]->getRP();

                //restaura y desempila la posición de sus ejes
                RP->getActuator()->getArm()->Restoretheta___3();
                RP->getActuator()->getArm()->Poptheta___3();
                RP->getActuator()->Restoretheta_1();
                RP->getActuator()->Poptheta_1();
        }
}

//---------------------------------------------------------------------------
//COLISIÓN:
//---------------------------------------------------------------------------

//levanta las banderas indicadoras de determinación de colisión
//pendiente de todos los posicionadores adscritos a los puntos objetivo
void TAllocationList::EnablePendingCollisionDetermineTAllocations(void)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->getRP()->getActuator()->Pending = true;
}

//busca los puntos objetivo que colisionan con otros puntos objetivo
void TAllocationList::SearchCollindingTAllocations(TVector<int> &indices)
{
        TAllocation *TP;
        TRoboticPositioner *RP;

        TVector<double> theta_1;
        TVector<double> theta___3;

        //NOTA: la cuantificación de los ejes puede estar activada o no.

        //guarda la configuración original de los posicionadores
        //desactiva las cuantificaciones de los ejes y
        //mueve los posicionadores a sus puntos objetivo adscritos:

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];
                //apunta el posicionador para facilitar su acceso
                RP = TP->getRP();
                //guarda las posiciones actuales
                theta_1.Add(RP->getActuator()->gettheta_1());
                theta___3.Add(RP->getActuator()->getArm()->gettheta___3());
                //mueve el posicionador a su punto objetivo
                TP->MoveToPP();
        }

        //reinicia el vector de índices en congruencia
        //con la situación de partida
        indices.Clear();

        //levanta las baderas de colisión de todos los posicionadores
        getRoboticPositionerList()->EnablePending();

        //ADVERTENCIA: no basta con levantar las banderas de colisión de
        //los posicionadores adscritos  los puntos objetivo de la lista
        //porque la colisión puede darse con posicionadores que no tengan
        //punto objetivo adscrito.

        TItemsList<TRoboticPositioner*> RPs;
        bool collision;

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //si el posicionador indicado no está en la lista
                if(indices.Search(i) >= indices.getCount()) {
                        //apunta el posicionador adscrito al punto objetivo indicado
                        //para facilitar su acceso
                        RP = Items[i]->getRP();
                        //busca los posicionadores adyacentes que colisionan
                        RP->getActuator()->SearchCollindingPendingAdjacent(RPs);
                        //indica que ya ha determinado el estado de colisión del brazo del posicionador
                        RP->getActuator()->Pending = false;

                        //deshabilita la bandera de detección de colisión
                        collision = false;
                        //añade los índices a los puntos objetivo adscritos a los posicionadores con los que colisiona
                        for(int j=0; j<RPs.getCount(); j++) {
                                //busca un punto objetivo adscrito al posicioandor indicado
                                int k = searchAllocation((TRoboticPositioner*)RPs[j]);
                                //si hay un punto objetivo adscrito al posicionador
                                if(k < getCount()) {
                                        //si el índice no está en la lista
                                        if(indices.Search(k) >= indices.getCount()) {
                                                //añade el índice al posicionador adyacente
                                                indices.Add(k);
                                                //habilita la bandera de detección de colisión
                                                collision = true;
                                        }
                                }
                        }
                        //si ha encontrado colisión con algún adyacente
                        if(collision)
                                //añade el índice al posicionador actual
                                indices.Add(i);
                }
        }

        //restaura la configuración original de los posicionadores:

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i]->getRP();
                //asigna los valores indicados de las listas
                RP->getActuator()->settheta_1(theta_1[i]);
                RP->getActuator()->getArm()->settheta___3(theta___3[i]);
        }

        //orde la lista de índices
        if(indices.getCount() > 1)
                indices.SortInc(0, indices.getCount()-1);
}

//COUNT OF ALLOCATION TYPES:

//count the number of reference sources in the SPPP list
unsigned int TAllocationList::countNR(void) const
{
    unsigned int count = 0;
    for(int i=0; i<getCount(); i++) {
        const TAllocation *TP = Items[i];
        if(TP->PP.Type == ptREFERENCE)
            count++;
    }
    return count;
}
//count the number of blanks in the SPPP list
unsigned int TAllocationList::countNB(void) const
{
    unsigned int count = 0;
    for(int i=0; i<getCount(); i++) {
        const TAllocation *TP = Items[i];
        if(TP->PP.Type == ptBLANK)
            count++;
    }
    return count;
}

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------

