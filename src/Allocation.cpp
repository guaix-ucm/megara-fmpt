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
//File: TAllocation.cpp
//Content: class allocation
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Allocation.h"
#include "Geometry.h"
#include "Strings.h" //StrTrim
#include "StrPR.h" //StrReadInt

using namespace Strings;

//---------------------------------------------------------------------------

//namespace for positioning
namespace Positioning {

//---------------------------------------------------------------------------
//TAllocation:
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//STATIC PROPERTIES:

//list of built objects
TItemsList<TAllocation*> TAllocation::Builts;

//---------------------------------------------------------------------------
//PROPERTIES IN TEXT FORMAT:

AnsiString TAllocation::getPPText(void)
{
        return DPointToStr(PP);
}
void TAllocation::setPPText(const AnsiString &S)
{
        try {
                PP = StrToDPoint(S);

        } catch(...) {
                throw;
        }
}

//SETS OF PROPERTIES IN TEXT FORMAT:

AnsiString TAllocation::getText(void)
{
        AnsiString S;

        S = IntToStr(getRP()->getActuator()->getId())+AnsiString(": ")+getPPText();

        return S;
}
void TAllocation::setText(const AnsiString&)
{
}

//---------------------------------------------------------------------------
//STATIC METHODS:

//compare the identifiers of the RPs attached to two target points
int  TAllocation::CompareIds(TAllocation *TPA1, TAllocation *TPA2)
{
        //el puntero TPA1 no debe ser nulo
        if(TPA1 == NULL)
                throw EImproperArgument("pointer TPA1 should not be null");

        //el puntero TPA2 no debe ser nulo
        if(TPA2 == NULL)
                throw EImproperArgument("pointer TPA2 should not be null");

        if(TPA1->getRP()->getActuator()->getId() < TPA2->getRP()->getActuator()->getId())
                return -1;

        if(TPA1->getRP()->getActuator()->getId() > TPA2->getRP()->getActuator()->getId())
                return 1;

        return 0;
}

//get the labels of the properties
//in row text format
AnsiString TAllocation::GetIdPPLabelsRow(void)
{
        return AnsiString("Id\t")+TDoublePoint::xLabel+AnsiString("\t")+TDoublePoint::yLabel;
}

//travel the labels of the properties
//in a text string from the position i
void  TAllocation::TravelLabels(const AnsiString& S, int& i)
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
void  TAllocation::ReadSeparated(int& Id, double& x, double& y,
                                      const AnsiString& S, int& i)
{
    //estado de lectura:
    //      0: esperando valor para Id
    //      1: esperando separador y valor para x
    //      2: esperando separador y valor para y
    //      3: punto objetivo leido con éxito
    int status = 0;

    //variables tampón en formato conveniente
    int _Id;
    double _x, _y;

    do {
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
TAllocation::TAllocation(TRoboticPositioner *_RP,
        double x, double y)
{
        //check the precondition
        if(_RP == NULL)
                throw EImproperArgument("pointer RP should point to built robotic positioner");

        //el RP RP no debería estar adscrito a
        //un punto objetivo previamente construido
        for(int i=0; i<Builts.getCount(); i++)
                if(_RP == Builts[i]->getRP())
                        throw EImproperArgument("robotic positioner RP should not be allocated to an previously built target point");

        //asigna los valores
        __RP = _RP;
        PP.x = x;
        PP.y = y;

        //apunta el nuevo punto objetivo a la lista de construidos
        Builts.Add(this);
}
TAllocation::TAllocation(TRoboticPositioner *_RP,
        TDoublePoint _PP)
{
        //el puntero RP debería apuntar a un RP construido
        if(_RP == NULL)
                throw EImproperArgument("pointer RP should point to built robotic positioner");

        //el RP RP no debería estar adscrito a
        //un punto objetivo previamente construido
        for(int i=0; i<Builts.getCount(); i++)
                if(_RP == Builts[i]->getRP())
                        throw EImproperArgument("robotic positioner RP should not be allocated to an previously built target point");

        //asigna los valores
        __RP = _RP;
        PP = _PP;

        //apunta el nuevo punto objetivo a la lista de construidos
        Builts.Add(this);
}
//destroy a TAllocation
//if thereisn't a built target point
//  throw an exception EImproperCall
TAllocation::~TAllocation()
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
bool TAllocation::IsOutDomainP3(void)
{
        if(getRP()->getActuator()->PointIsOutDomainP3(PP))
                return true;

        return false;
}
//determines if the target point is in the secure area
//of thepoint P3 of the attached RP
bool TAllocation::IsInSafeAreaP3(void)
{
        //determina si el punto objetivo está en el dominio del posicionador
        //adscrito y calcula las posiciones angulares de los ejes
        double theta_1, theta___3;
        bool isindomain = getRP()->getActuator()->AnglesToGoP3(theta_1, theta___3, PP.x, PP.y);

        //si no está en el dominio o no está dentro del área de seguridad
        if(!isindomain || getRP()->getActuator()->theta___3IsOutSafeArea(theta___3))
                return false; //indica que no está en el área de seguridad

        //indica que está en el área de seguridad
        return true;
}

//---------------------------------------------------------------------------
//MOTION METHODS:

//randomize the point PP with uniform distribution
//in the domain of the point P3 of its attached RP
void TAllocation::RandomizePP(void)
{
        PP = getRP()->getActuator()->RandomP3();
}

//assign the point PP to the point P3 of its attached RP
//and return the distance from the stable position to the target point
//if the the point PP isn't on the domain of its attached RP:
//  throw an exception EImpropercall
double TAllocation::MoveToPP(void)
{
        //determines if the target point is in the domain of the attached RP
        //and calculates the position angles of the rotors
        double theta_1, theta___3;
        bool isindomain = getRP()->getActuator()->AnglesToGoP3(theta_1, theta___3, PP.x, PP.y);

        //target point PP should be in the point P3 domain of his allocated RP
        if(!isindomain)
                throw EImproperCall("target point PP should be in the point P3 domain of his allocated RP");

        //determines the stable position more closer to the target point
        //and determines the distance fromthe stable position to the target point
        double p_1nsp, p___3nsp;
        double d = getRP()->getActuator()->GetNearestStablePosition(p_1nsp, p___3nsp, theta_1, theta___3);

        //assign the positions to the rotors
        getRP()->getActuator()->SetAnglesSteps(p_1nsp, p___3nsp);

        //returns the distance fromthe stable position to the target point
        return d;
}

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
