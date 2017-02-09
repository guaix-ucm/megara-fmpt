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
//File: SquareFunction.cpp
//Content: square motion function without acceleration and deceleration ramps
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "SquareFunction.h"
#include "Exceptions.h"
#include "Scalars.h"

//---------------------------------------------------------------------------

using namespace Mathematics;

//espacio de nombres de funciones de movimiento
namespace MotionFunctions {

//---------------------------------------------------------------------------
//TSquareFunction
//---------------------------------------------------------------------------
//PARÁMETROS IMPOSITIVOS:

void TSquareFunction::setvmaxabs(double vmaxabs)
{
        //debe ser mayor que cero
        if(vmaxabs <= 0)
                throw EImproperArgument("vmaxabs should not be less than zero");

        p_vmaxabs = vmaxabs; //asigna el nuevo valor

        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(p_T < p_Tmin)
                p_T = p_Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //arrastra 'vc' con los límties del dominio
        if(p_vcmax>0 && p_vc>p_vcmax)
                p_vc = p_vcmax;
        else if(p_vcmax<0 && p_vc<p_vcmax)
                p_vc = p_vcmax;

        //calcula las variables correspondientes a 'vc'
        CalculateVariables_vc();
}

void TSquareFunction::setpsta(double psta)
{
        p_psta = psta; //asigna el nuevo valor

        p_D = p_pfin - p_psta; //calcula la distancia a recorrer

        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(p_T < p_Tmin)
                p_T = p_Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //arrastra 'vc' con los límties del dominio
        if(p_vcmax>0 && p_vc>p_vcmax)
                p_vc = p_vcmax;
        else if(p_vcmax<0 && p_vc<p_vcmax)
                p_vc = p_vcmax;

        //calcula las variables correspondientes a 'vc'
        CalculateVariables_vc();
}
void TSquareFunction::setpfin(double pfin)
{
        p_pfin = pfin; //asigna el nuevo valor

        p_D = p_pfin - p_psta; //calcula la distancia a recorrer

        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(p_T < p_Tmin)
                p_T = p_Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //arrastra 'vc' con los límties del dominio
        if(p_vcmax>0 && p_vc>p_vcmax)
                p_vc = p_vcmax;
        else if(p_vcmax<0 && p_vc<p_vcmax)
                p_vc = p_vcmax;

        //calcula las variables correspondientes a 'vc'
        CalculateVariables_vc();
}

//--------------------------------------------------------------------------
//TIEMPO DE DESPLAZAMIENTO ELEGIDO:

void TSquareFunction::setD(double D)
{
        p_D = D; //asigna el nuevo valor

        p_pfin = p_psta + p_D; //calcula la posición final

        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(p_T < p_Tmin)
                p_T = p_Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //arrastra 'vc' con los límties del dominio
        if(p_vcmax>0 && p_vc>p_vcmax)
                p_vc = p_vcmax;
        else if(p_vcmax<0 && p_vc<p_vcmax)
                p_vc = p_vcmax;

        //calcula las variables correspondientes a 'vc'
        CalculateVariables_vc();
}

void TSquareFunction::setT(double T)
{
        //debe dar tiempo a ir de psta a pfin con amax y vmax dadas
        if(T < p_Tmin)
                throw EImproperArgument("T should not be less than Tmin");

        p_T = T; //asigna el nuevo valor

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //arrastra 'vc' con los límties del dominio
        if(p_vcmax>0 && p_vc>p_vcmax)
                p_vc = p_vcmax;
        else if(p_vcmax<0 && p_vc<p_vcmax)
                p_vc = p_vcmax;

        //calcula las variables correspondientes a 'vc'
        CalculateVariables_vc();
}

//--------------------------------------------------------------------------
//VARIABLE:

void TSquareFunction::setvc(double vc)
{
        if(p_vmax >= 0) {
                //debe estar en el intervalo válido
                if(p_pfin != p_psta) {
                        if(vc<=0 || p_vmax<vc)
                                throw EImproperArgument(
                                        "vc should be in (0, vmax]");
                }
                else //pfin == psta
                        if(vc<0 || p_vmax<vc)
                                throw EImproperArgument(
                                        "vc should be in [0, vmax]");
        }
        else { //vmax < 0
                //debe estar en el intervalo válido
                if(p_pfin != p_psta) {
                        if(vc<p_vmax || 0<=vc)
                                throw EImproperArgument(
                                        "vc should be in [vmax, 0)");
                }
                else //pfin == psta
                        if(vc<p_vmax || 0<vc)
                                throw EImproperArgument(
                                        "vc should be in [vmax, 0]");
        }

        p_vc = vc; //asigna el nuevo valor

        //calcula T
        if(p_vc != 0)
                p_T = p_D/p_vc;
        else //vc == 0
                p_T = 0;
}

//--------------------------------------------------------------------------
//MÉTODOS:

//calcula los hitos previos a la elección de 'T'
//(vmax, Tmin)
void TSquareFunction::CalculateMilestones()
{
        p_vmax = Sign(p_D)*p_vmaxabs; //calcula el valor algebraico de la velocidad
        p_Tmin = p_D/p_vmax; //calcula el timepo mínimo para recorrer la distancia
}

//calcula la distancia máxima que se pueden recorrer en el tiempo 'T'
//(Dmax)
void TSquareFunction::CalculateDistances()
{
        p_Dmax = p_vmax*p_T;
}

//calcula los límites del dominio de las variables:
//[vcmin, vcmax] (en realidad solo vcmax, ya que vcmin es cte = 0)
void TSquareFunction::CalculateBoundaries(void)
{
        if(p_D != 0)
                p_vcmax = p_vmax;
        else
                p_vcmax = 0;
}

//calcula las variables derivadas de 'vc'
void TSquareFunction::CalculateVariables_vc(void)
{
        //cuando T=0 => D=0 => vc=0

        if(p_T != 0)
                p_vc = p_D/p_T;
        else //T == 0
                p_vc = 0;
}

//---------------------------------------------------------------------------
//MÉTODOS:

//inicializa {vmax=vmaxabs, tini=0, tfin=0} y asimila los parámetros
TSquareFunction::TSquareFunction(double vmaxabs) :
        TMotionFunction()
{
        //vmaxabs debe ser mayor que cero
        if(vmaxabs <= 0)
                throw EImproperArgument("vmaxabs should be upper zero");

        //asigna valores a los parámetros impositivos no inicializados

        p_vmax = p_vmaxabs = vmaxabs;

        //al inicializar todas las variables a cero
        //los siguientes pasos no son necesarios
        //
        //p_D = pfin - psta; //calcula la distancia a recorrer
        //
        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        //CalculateMilestones();
        //
        //elige las variables de primer orden
        //p_T = Tmin;
        //
        //calcula las distancias máximas que se pueden recorrer en el tiempo 'T'
        //(Dmax)
        //CalculateDistances();
        //
        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        //CalculateBoundaries();
        //
        //arrastra 'vc' con los límties del dominio
        //if(vcmax>0 && vc>vcmax)
        //        p_vc = vcmax;
        //else if(vcmax<0 && vc<vcmax)
        //        p_vc = vcmax;

        //calcula la variable (vc)
        //CalculateVariables_vc();
}
//clona una función cuadrada
TSquareFunction::TSquareFunction(TSquareFunction *SquareFunction) :
        TMotionFunction(SquareFunction) //clona las propiedades heredadas
{
        //debe apuntar a un objeto construido
        if(SquareFunction == NULL)
                throw EImproperArgument("pointer SquareFunction should not be null");

        //copia todas las propiedades no heredadas
        //no hay propiedades no heredadas
}
//copia todas las propiedades de una función cuadrada
void TSquareFunction::Copy(TMotionFunction *SquareFunction)
{
        //debe apuntar a un objeto construido
        if(SquareFunction == NULL)
                throw EImproperArgument("pointer SquareFunction should not be null");

        //apunta el objeto con un puntero de la clase
        TSquareFunction *aux = (TSquareFunction*)SquareFunction;

        //copia las propiedades heredadas
        p_vmaxabs = aux->p_vmaxabs;
        p_psta = aux->p_psta;
        p_pfin = aux->p_pfin;
        p_vmax = aux->p_vmax;
        p_Tmin = aux->p_Tmin;
        p_D = aux->p_D;
        p_T = aux->p_T;
        p_Dmax = aux->p_Dmax;
        p_vcmin = aux->p_vcmin;
        p_vcmax = aux->p_vcmax;
        p_vc = aux->p_vc;
}

//initalize all properties except (vmaxabs)
void TSquareFunction::reset(void)
{
    //INITIALIZE THE INHERITED PREOPERTIES:

    p_psta = 0;
    p_pfin = 0;
    p_vmax = 0;

    p_Tmin = 0;
    p_D = 0;

    p_Dmax = 0;
    p_vcmin = 0;
    p_vcmax = 0;
    p_vc = 0;

    p_T = 0;

    //INITIALIZE THE OWN PROPERTIES:

    p_vmax = getvmaxabs();
}

//asigna (psta, pfin) conjuntamente
void TSquareFunction::SetInterval(double psta, double pfin)
{
        //asigna los nuevos valores
        p_psta = psta;
        p_pfin = pfin;

        p_D = p_pfin - p_psta; //calcula la distancia a recorrer

        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(p_T < p_Tmin)
                p_T = p_Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //arrastra 'vc' con los límties del dominio
        if(p_vcmax>0 && p_vc>p_vcmax)
                p_vc = p_vcmax;
        else if(p_vcmax<0 && p_vc<p_vcmax)
                p_vc = p_vcmax;

        //calcula las variables correspondientes a 'vc'
        CalculateVariables_vc();
}

//invierte (psta, pfin) manteniendo el tiempo de desplazameinto T
//y la forma de la función
void TSquareFunction::InvertTime(void)
{
        //asigna los nuevos valores
        double aux = p_psta;
        p_psta = p_pfin;
        p_pfin = aux;

        p_D = p_pfin - p_psta; //calcula la distancia a recorrer

        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        CalculateMilestones();

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //invierte 'vc'
        p_vc *= -1;

        //calcula las variables correspondientes a 'vc'
        CalculateVariables_vc();
}

//velocidad correspondiente al instante t en ul/ut
double TSquareFunction::v(double t)
{
        if(t <= 0)
                return 0;
        else if(t <= p_T)
                return p_vc;
        else
                return 0;
}

//posición correspondiente al instante t en ul
double TSquareFunction::p(double t)
{
        if(t <= 0)
                return p_psta;
        else if(t <= p_T)
                return p_psta + p_vc*t;
        else
                return p_pfin;
}

//---------------------------------------------------------------------------

} //namespace MotionFunctions

//---------------------------------------------------------------------------

