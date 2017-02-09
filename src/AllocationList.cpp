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
//File: AllocationList.cpp
//Content: allocation list
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "AllocationList.h"
#include "Strings.h"

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres del exposicionamiento
namespace Positioning {

//---------------------------------------------------------------------------

AnsiString TAllocationList::getAllocationsText(void)
{
    //print the labels of the table
    string str = TAllocation::GetIdPPLabelsRow().str;

    //para cada asignación de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el objeto indicado para facilitar su acceso
        TAllocation *A = Items[i];

        //imprime el identificador
        string row = "\r\n";
        row += A->getRP()->getActuator()->getIdText().str;

        //imprime la coordenada x
        row += "\t";
        row += A->PP.getxText().str;

        //imprime la coordenada y
        row += "\t";
        row += A->PP.getyText().str;

        //aade la fila
        str += row;
    }

    return AnsiString(str);
}
void TAllocationList::setAllocationsText(AnsiString &S)
{
    TSlideArray<TAllocation*> Items; //variable tampón

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
        if(i<1 || S.Length()+1<i)
            throw EImproperArgument("index i should indicate a posicion in the string S");

        //estado de la máquina de estados
        //      0: esperando próximo elemento o fin de cadena
        //      1: fin de cadena encontrada y lista de elementos leída con éxito
        int status = 0;

        //avanza el índice hasta el próximo caracter no separador
        StrTravelSeparatorsIfAny(S, i);

        do { //repite
            //si no ha llegado al final de la cadena
            if(i <= S.Length()) {
                try {
                    //lee los valores de las propiedades próximo elemento
                    int Id;
                    double x, y;
                    TAllocation::ReadSeparated(Id, x, y, S, i);
                    //busca el RP en la RPL de la TPL
                    int j = getRoboticPositionerList()->searchId(Id);
                    //comprueba que ha encontrado el RP
                    if(j >= getRoboticPositionerList()->getCount())
                        throw EImproperFileLoadedValue("RP "+inttostr(Id)+"not found");
                    //apunta el RP encontrado
                    TRoboticPositioner *RP = getRoboticPositionerList()->Get(j);
                    //contruye el elemento adscrito al RP encontrado
                    TAllocation *Item = new TAllocation(RP, x, y);

                    //ERROR: si el RP tiene otro RP asignado, TAllocation lanzará una excepción EImproperArgument.
                    //Eso impedirá que puedan signarse nuevas PP tables, amenos que la TPL esté inicializada.

                    //añade el elemento
                    Items.AddLast(Item);
                    //avanza el índice hasta el próximo caracter no separador
                    StrTravelSeparatorsIfAny(S, i);
                    //si el índice indica a la posúltima posición de la cadena
                    if(i > S.Length())
                        status++; //indica que se ha leido la lista con éxito

                    //ADVERTENCIA: el elemento tiene que ser añadido a la lista tampón
                    //tan pronto como es contruido, para que pueda ser destruido
                    //en caso de error

                } catch(...) {
                    throw;
                }
            }
            else //si ha llegado al final de la cadena
                //indica que ha llegado al final de la cadena
                status++;
        }
        //mientras no se haya acabado la cadena y no se haya leido la lista con éxito
        while(status < 1);

        //asigna la variable tampón:

        //reinicia la lista de puntos objetivo
        //copia las variables tampón
        Delete_();
        for(int j=0; j<Items.getCount(); j++)
            Add(Items[j]);
        Items.DelLast(Items.getCount());
    }
    catch(...) {
        //destruye los elementos tampón
        for(int i=0; i<Items.getCount(); i++)
            delete Items[i];
        throw;
    }
}

//---------------------------------------------------------------------------
//BUILDING AND DESTROYING METHODS:

//construye una lista de puntos objetivo
//adscrita a una lista de RPs
TAllocationList::TAllocationList(TRoboticPositionerList *RPL) :
    //        TAllocations(1000, TAllocation::CompareIds)
    TItemsList<TAllocation*>(1000, TAllocation::CompareIds)
{
    //el puntero RoboticPositionerList debería apuntar a una lista de RPs contruida
    if(RPL == NULL)
        throw EImproperArgument("pointer RoboticPositionerList should point to built RP list");

    //apunta el objeto externo
    p_RoboticPositionerList = RPL;

    Print = TAllocation::PrintRow;
}

//borra los puntos objetivo y destruye la lista
TAllocationList::~TAllocationList()
{
    Delete_();
}

//---------------------------------------------------------------------------
//SEARCHING METHODS:

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
//LISTED ITEMS:

//añade una asignación para el posicionador indicado de la lista
void TAllocationList::AddAllocation(int i)
{
    //comprueba las precondiciones
    if(i<0 || getRoboticPositionerList()->getCount()<=i)
        throw EImproperArgument("index i should indicate a position in the list ListFibeRPositioners");

    //apunta el posicionador indicado para facilitar su acceso
    TRoboticPositioner *RP = getRoboticPositionerList()->Get(i);

    //busca la asignación adscrita al posicionador
    i = searchAllocation(RP);

    //comprueba que el posicionador no tenga una asignación adscrita
    if(i > getRoboticPositionerList()->getCount())
        throw EImproperArgument("indexed positioner i already have assigned a allocation");

    //contruye y añade una asignación adscrita al posicionador
    //con las coordenadas de su punto P3
    Add(new TAllocation(RP, RP->getActuator()->getArm()->getP3()));
}
//borra la asignación indicada de la lista
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
//ALL ITEMS:

//añade asignaciones con la posición del punto P3
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

//destruye las asignaciones
//de todos los posicionadores de la lista
void TAllocationList::Delete_(void)
{
    //por cada punto objetivo de la lista
    for(int i=0; i<getCount(); i++)
        //destruye el punto objetivo
        delete Items[i];

    //borra las posiciones de la lista
    Clear();
}

//asigna el punto P3o a los puntos objetivo
//de todos los posicionadores de la lista
void TAllocationList::SetP3o(void)
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
    //comprueba las precondiciones
    TVector<int> indices;
    SearchMissingRPs(indices);
    if(indices.getCount() > 0)
        throw EImproperCall("all RPs should be in the FMM");

    //vectores para guardar la configuración inicial
    TVector<char> Quantify_;
    TVector<char> Quantify__;
    TVector<double> theta_1;
    TVector<double> theta__3;

    //bandera de colisión detectada
    bool collision;

    //levanta las baderas de colisión de todos los posicionadores
    getRoboticPositionerList()->enablePending();

    //ADVERTENCIA: no basta con levantar las banderas de colisión de
    //los posicionadores adscritos  los puntos objetivo de la lista
    //porque la colisión puede darse con posicionadores que no tengan
    //punto objetivo adscrito.

    //por cada punto objetivo de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el punto objetivo indicado para facilitar su acceso
        TAllocation *A = Items[i];
        //apunta el posicionador adscrito para facilitar su acceso
        TRoboticPositioner *RP = A->getRP();

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
            A->RandomizePP();
            //mueve el posicionador al punto objetivo
            A->MoveToPP();
            //determina si hay colisión
            collision = RP->getActuator()->thereIsCollisionWithAdjacent();
            //mientras el brazo del posicionador invada a sus adyacentes
        } while(collision);
    }

    //restore the original status of the RPs
    for(int i=0; i<getCount(); i++) {
        TRoboticPositioner *RP = Items[i]->getRP();
        RP->getActuator()->getArm()->settheta___3(theta__3[i]);
        RP->getActuator()->settheta_1(theta_1[i]);
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
    try {
    //comprueba las precondiciones
    TVector<int> indices;
    SearchOutDomineTAllocations(indices);
    if(indices.getCount() > 0) {
        //obtiene el vector de identificadores
        TVector<int> Ids;
        for(int i=0; i<indices.getCount(); i++)
            Ids.Add(Items[indices[i]]->getRP()->getActuator()->getId());

        //indica el error
        throw EImproperCall(AnsiString("there is some allocation out of the scope of their atached RP: ")+Ids.getText());
    }

    //para cada punto objetivo de la lista
    for(int i=0; i<getCount(); i++)
        //asigna el punto objetivo a su RP adscrito
        Items[i]->MoveToPP();
    }
    catch(Exception& E) {
        E.Message.Insert(1, "moving the RPs to their attached target points: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//VALIDATION:

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
    //initialize the output
    indices.Clear();

    //search the missing RPs
    for(int i=0; i<getCount(); i++) {
        TAllocation *A = Items[i];

        //search the attached RP in the RPL
        int j = getRoboticPositionerList()->search(A->getRP());

        //if the RP isn't in the RPL
        if(j >= getRoboticPositionerList()->getCount())
            indices.Add(i); //add it to the missing list
    }
}

//busca los puntos objetivo que están fuera del dominio
//de sus posicionadores adscritos
void TAllocationList::SearchOutDomineTAllocations(TVector<int> &indices)
{
    //reinicial el vector de índices en congruencia
    //con la situación de partida
    indices.Clear();

    //por cada asignación de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta la asignación indicada para facilitar su acceso
        TAllocation *A = Items[i];

        //apunta el posicionador asignado para facilitar su acceso
        TRoboticPositioner *RP = A->getRP();

        //traduce el punto a coordenadas angulares de los ejes
        //y determina si está en el dominio del posicionador
        double theta_1, theta___3;
        bool isindomine = RP->getActuator()->anglesToGoP3(theta_1, theta___3, A->PP.x, A->PP.y);

        //si el punto objetivo está fuera del dominio del posicionador
        if(!isindomine)
            //añade el índice a la lista
            indices.Add(i);
    }
}

//Determina la invalidez de una lista de asignaciones
//para ser programada.
//Valores de retorno:
//  0: lista de asignaciones válida;
//  1: puntos objetivo adscritos a posicionadores ausentes;
//  2: puntos objetivo fuera del dominio de sus posicionadores adscritos.
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
//SEGREGATION:

//segrega los posicionadores de las asignaciones en dos listas:
//  internos: con el brazo dentro del área de seguridad;
//  externos: con el brazo fuera del área de seguridad;
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
        isindomain = RP->getActuator()->anglesToGoP3(theta_1, theta___3, TP->PP.x, TP->PP.y);

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
//STACKING AND RETRIEVAL OF POSITIONS:

//apila las posiciones de los posicionadores adscritos
void TAllocationList::PushPositions(void)
{
    TRoboticPositioner *RP;

    //por cada punto objetivo de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador adscrito inicado para facilitar su acceso
        RP = Items[i]->getRP();

        //apila la posición de sus ejes
        RP->getActuator()->pushtheta_1();
        RP->getActuator()->getArm()->pushtheta___3();
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
        RP->getActuator()->getArm()->restoretheta___3();
        RP->getActuator()->restoretheta_1();
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
        RP->getActuator()->getArm()->poptheta___3();
        RP->getActuator()->poptheta_1();
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
        RP->getActuator()->getArm()->restoretheta___3();
        RP->getActuator()->getArm()->poptheta___3();
        RP->getActuator()->restoretheta_1();
        RP->getActuator()->poptheta_1();
    }
}

//---------------------------------------------------------------------------
//COLLISION:

//levanta las banderas indicadoras de determinación de colisión
//pendiente de todos los posicionadores adscritos a las asignaciones
void TAllocationList::EnablePendingCollisionDetermineTAllocations(void)
{
    for(int i=0; i<getCount(); i++)
        Items[i]->getRP()->getActuator()->Pending = true;
}

//busca las asignaciones que colisionan con otras asignaciones
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
    getRoboticPositionerList()->enablePending();

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
            RP->getActuator()->searchCollindingPendingAdjacent(RPs);
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

//count the number of reference sources in the allocation list
unsigned int TAllocationList::countNR(void) const
{
    unsigned int count = 0;
    for(int i=0; i<getCount(); i++) {
        const TAllocation *A = Items[i];
        if(A->PP.Type == ptREFERENCE)
            count++;
    }
    return count;
}
//count the number of blanks in the allocation list
unsigned int TAllocationList::countNB(void) const
{
    unsigned int count = 0;
    for(int i=0; i<getCount(); i++) {
        const TAllocation *A = Items[i];
        if(A->PP.Type == ptBLANK)
            count++;
    }
    return count;
}

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------

