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
//File: TargetPoint.cpp
//Content: target point attached to a RP
//Last update: 27/09/2013
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "TargetPoint.h"
#include "Geometry.h"
#include "Strings.h" //StrTrim
#include "StrPR.h" //StrReadInt

using namespace Strings;

//---------------------------------------------------------------------------

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//TTargetPoint:
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//STATIC PROPERTIES:

//list of built objects
TItemsList<TTargetPoint*> TTargetPoint::Builts;

//---------------------------------------------------------------------------
//PROPERTIES IN TEXT FORMAT:

AnsiString TTargetPoint::getTargetP3Text(void)
{
        return DPointToStr(TargetP3);
}
void TTargetPoint::setTargetP3Text(const AnsiString &S)
{
        try {
                TargetP3 = StrToDPoint(S);

        } catch(...) {
                throw;
        }
}

//SETS OF PROPERTIES IN TEXT FORMAT:

AnsiString TTargetPoint::getText(void)
{
        AnsiString S;

        S = IntToStr(getRoboticPositioner()->getActuator()->getId())+AnsiString(": ")+getTargetP3Text();

        return S;
}
void TTargetPoint::setText(const AnsiString&)
{
}

//---------------------------------------------------------------------------
//STATIC METHODS:

//compare the identifiers of the RPs attached to two target points
int  TTargetPoint::CompareIds(TTargetPoint *TPA1, TTargetPoint *TPA2)
{
        //el puntero TPA1 no debe ser nulo
        if(TPA1 == NULL)
                throw EImproperArgument("pointer TPA1 should not be null");

        //el puntero TPA2 no debe ser nulo
        if(TPA2 == NULL)
                throw EImproperArgument("pointer TPA2 should not be null");

        if(TPA1->getRoboticPositioner()->getActuator()->getId() < TPA2->getRoboticPositioner()->getActuator()->getId())
                return -1;

        if(TPA1->getRoboticPositioner()->getActuator()->getId() > TPA2->getRoboticPositioner()->getActuator()->getId())
                return 1;

        return 0;
}

//get the labels of the properties
//in row text format
AnsiString TTargetPoint::GetIdTargetP3LabelsRow(void)
{
        return AnsiString("Id\t")+TDoublePoint::xLabel+AnsiString("\t")+TDoublePoint::yLabel;
}

//travel the labels of the properties
//in a text string from the position i
void  TTargetPoint::TravelLabels(const AnsiString& S, int& i)
{
    try {
        //atraviesa los separadores hasta el próximo caracter no separador
        StrTravelSeparatorsIfAny(S, i);
        //lee la etiqueta de la propiedad Id
        StrTravelLabel(StrTrim(AnsiString("Id")), S, i);
        //atraviesa el separador obligatorio
        StrTravelSeparators(S, i);
        //lee la etiqueta de la propiedad x
        StrTravelLabel(StrTrim(TDoublePoint::xLabel), S, i);
        //atraviesa el separador obligatorio
        StrTravelSeparators(S, i);
        //lee la etiqueta de la propiedad y
        StrTravelLabel(StrTrim(TDoublePoint::yLabel), S, i);

    } catch(...) {
        throw;
    }
}

//read the values of the properties
//in a text string from the position i
void  TTargetPoint::ReadSeparated(int& Id, double& x, double& y,
                                      const AnsiString& S, int& i)
{
    //estado de lectura:
    //      0: esperando valor para Id
    //      1: esperando separador y valor para x
    //      2: esperando separador y valor para y
    //      3: punto objetivo leido con éxito
    int status = 0;

    //caracter indicado S[i]
    char c;

    //variables tampón en formato conveniente
    int _Id;
    double _x, _y;

    do {
        c = S[i]; //asigna el próximo caracter

        //reacciona según el estado y al caracter
        switch(status) {
        case 0: //esperando valor para Id
            try {
                //avanza el índice hasta el próximo caracter no separador
                StrTravelSeparatorsIfAny(S, i);
                //lee el valor para Id
                StrReadInt(_Id, S, i);
                //pasa a leer el valor para x
                status++;
            } catch(...) {
                throw;
            }
            break;
        case 1: //esperando separador y valor para x
            try {
                //atraviesa el separador obligatorio
                StrTravelSeparators(S, i);
                //lee el valor para x
                StrReadFloat(_x, S, i);
                //pasa a leer el valor para y
                status++;
            } catch(...) {
                throw;
            }
            break;
        case 2: //esperando separador y valor para y
            try {
                //atraviesa el separador obligatorio
                StrTravelSeparators(S, i);
                //lee el valor para y
                StrReadFloat(_y, S, i);
                //pasa a asignar la variable tampón
                status++;
            } catch(...) {
                throw;
            }
            break;
        }
    //mientras nos e haya leido el punto objetivo con éxito
    } while(status < 3);

    //asigna las propiedades
    Id = _Id;
    x = _x;
    y = _y;
}

//---------------------------------------------------------------------------
//BUILDING AND DESTROYING METHODS:

//build a target point attached a RP
//if the RP already has an attached target point
//  throw an exception EImproperArgument
TTargetPoint::TTargetPoint(TRoboticPositioner *_RoboticPositioner,
        double x, double y)
{
        //el puntero RoboticPositioner debería apuntar a un RP construido
        if(_RoboticPositioner == NULL)
                throw EImproperArgument("pointer RoboticPositioner should point to built fiber positioner");

        //el RP RoboticPositioner no debería estar adscrito a
        //un punto objetivo previamente construido
        for(int i=0; i<Builts.getCount(); i++)
                if(_RoboticPositioner == Builts[i]->getRoboticPositioner())
                        throw EImproperArgument("Fiber positioner FibeRPositioner should not be assigned to an previously built target point");

        //asigna los valores
        __RoboticPositioner = _RoboticPositioner;
        TargetP3.x = x;
        TargetP3.y = y;

        //apunta el nuevo punto objetivo a la lista de construidos
        Builts.Add(this);
}
TTargetPoint::TTargetPoint(TRoboticPositioner *_RoboticPositioner,
        TDoublePoint _TargetP3)
{
        //el puntero RoboticPositioner debería apuntar a un RP construido
        if(_RoboticPositioner == NULL)
                throw EImproperArgument("pointer RoboticPositioner should point to built fiber positioner");

        //el RP RoboticPositioner no debería estar adscrito a
        //un punto objetivo previamente construido
        for(int i=0; i<Builts.getCount(); i++)
                if(_RoboticPositioner == Builts[i]->getRoboticPositioner())
                        throw EImproperArgument("Fiber positioner FibeRPositioner should not be assigned to an previously built target point");

        //asigna los valores
        __RoboticPositioner = _RoboticPositioner;
        TargetP3 = _TargetP3;

        //apunta el nuevo punto objetivo a la lista de construidos
        Builts.Add(this);
}
//destroy a targetpoint
//if thereisn't a built target point
//  throw an exception EImproperCall
TTargetPoint::~TTargetPoint()
{
        //debe haber algún punto objetivo construido
        if(Builts.getCount() < 1)
                throw EImproperCall("should be some constructed target point");

        //busca el posicionador que se va a destruir en la lista de construidos
        int i = 0;
        while(i<Builts.getCount() && this!=Builts[i])
                i++;

        //si no lo encuentra
        if(i >= Builts.getCount())
                //indica que no se encuentra un punto objetivo previamente contruido
                throw EImpossibleError("dont find a previously built target point");

        //borra el puntero de la lista
        Builts.Delete(i);
}

//---------------------------------------------------------------------------
//CHECKING METHODS:

//determines if the target point is out of the domain
//of thepoint P3 of the attached RP
bool TTargetPoint::IsOutDomainP3(void)
{
        if(getRoboticPositioner()->getActuator()->PointIsOutDomainP3(TargetP3))
                return true;

        return false;
}
//determines if the target point is in the secure area
//of thepoint P3 of the attached RP
bool TTargetPoint::IsInSafeAreaP3(void)
{
        //determina si el punto objetivo está en el dominio del posicionador
        //adscrito y calcula las posiciones angulares de los ejes
        double theta_1, theta___3;
        bool isindomain = getRoboticPositioner()->getActuator()->AnglesToGoP3(theta_1, theta___3, TargetP3.x, TargetP3.y);

        //si no está en el dominio o no está dentro del área de seguridad
        if(!isindomain || getRoboticPositioner()->getActuator()->theta___3IsOutSafeArea(theta___3))
                return false; //indica que no está en el área de seguridad

        //indica que está en el área de seguridad
        return true;
}

//---------------------------------------------------------------------------
//MOVING METHODS:

//randomize the point TargetP3 with uniform distribution
//in the domain of the point P3 of its attached RP
void TTargetPoint::Randomize(void)
{
        TargetP3 = getRoboticPositioner()->getActuator()->RandomP3();
}

//assign the point TargetP3 to the point P3 of its attached RP
//and return the distance from the stable position to the target point
//if the the point TargetP3 isn't on the domain of its attached RP:
//  throw an exception EImpropercall
double TTargetPoint::MoveToTargetP3(void)
{
        //determines if the target point is in the domain of the attached RP
        //and calculates the position angles of the rotors
        double theta_1, theta___3;
        bool isindomain = getRoboticPositioner()->getActuator()->AnglesToGoP3(theta_1, theta___3, TargetP3.x, TargetP3.y);

        //target point TargetP3 should be in the point P3 domain of his allocated RP
        if(!isindomain)
                throw EImproperCall("target point TargetP3 should be in the point P3 domain of his allocated RP");

        //determines the stable position more closer to the target point
        //and determines the distance fromthe stable position to the target point
        double p_1nsp, p___3nsp;
        double d = getRoboticPositioner()->getActuator()->GetNearestStablePosition(p_1nsp, p___3nsp, theta_1, theta___3);

        //assign the positions to the rotors
        getRoboticPositioner()->getActuator()->SetAnglesSteps(p_1nsp, p___3nsp);

        //returns the distance fromthe stable position to the target point
        return d;
}

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
