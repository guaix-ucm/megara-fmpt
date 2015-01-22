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
//Archivo: SquareFunction.cpp
//Contenido: función de movimiento cuadrada
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
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

void TSquareFunction::setvmaxabs(double _vmaxabs)
{
        //debe ser mayor que cero
        if(_vmaxabs <= 0)
                throw EImproperArgument("vmaxabs should not be less than zero");

        __vmaxabs = _vmaxabs; //asigna el nuevo valor

        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(__T < __Tmin)
                __T = __Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Smax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //arrastra 'vc' con los límties del dominio
        if(__vcmax>0 && __vc>__vcmax)
                __vc = __vcmax;
        else if(__vcmax<0 && __vc<__vcmax)
                __vc = __vcmax;

        //calcula las variables correspondientes a 'vc'
        CalculateVariables_vc();
}

void TSquareFunction::setpsta(double _psta)
{
        __psta = _psta; //asigna el nuevo valor

        __D = __pfin - __psta; //calcula la distancia a recorrer

        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(__T < __Tmin)
                __T = __Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Smax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //arrastra 'vc' con los límties del dominio
        if(__vcmax>0 && __vc>__vcmax)
                __vc = __vcmax;
        else if(__vcmax<0 && __vc<__vcmax)
                __vc = __vcmax;

        //calcula las variables correspondientes a 'vc'
        CalculateVariables_vc();
}
void TSquareFunction::setpfin(double _pfin)
{
        __pfin = _pfin; //asigna el nuevo valor

        __D = __pfin - __psta; //calcula la distancia a recorrer

        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(__T < __Tmin)
                __T = __Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Smax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //arrastra 'vc' con los límties del dominio
        if(__vcmax>0 && __vc>__vcmax)
                __vc = __vcmax;
        else if(__vcmax<0 && __vc<__vcmax)
                __vc = __vcmax;

        //calcula las variables correspondientes a 'vc'
        CalculateVariables_vc();
}

//--------------------------------------------------------------------------
//TIEMPO DE DESPLAZAMIENTO ELEGIDO:

void TSquareFunction::setD(double _D)
{
        __D = _D; //asigna el nuevo valor

        __pfin = __psta + __D; //calcula la posición final

        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(__T < __Tmin)
                __T = __Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Smax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //arrastra 'vc' con los límties del dominio
        if(__vcmax>0 && __vc>__vcmax)
                __vc = __vcmax;
        else if(__vcmax<0 && __vc<__vcmax)
                __vc = __vcmax;

        //calcula las variables correspondientes a 'vc'
        CalculateVariables_vc();
}

void TSquareFunction::setT(double _T)
{
        //debe dar tiempo a ir de psta a pfin con amax y vmax dadas
        if(_T < __Tmin)
                throw EImproperArgument("T should not be less than Tmin");

        __T = _T; //asigna el nuevo valor

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Smax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //arrastra 'vc' con los límties del dominio
        if(__vcmax>0 && __vc>__vcmax)
                __vc = __vcmax;
        else if(__vcmax<0 && __vc<__vcmax)
                __vc = __vcmax;

        //calcula las variables correspondientes a 'vc'
        CalculateVariables_vc();
}

//--------------------------------------------------------------------------
//VARIABLE:

void TSquareFunction::setvc(double _vc)
{
        if(__vmax >= 0) {
                //debe estar en el intervalo válido
                if(__pfin != __psta) {
                        if(_vc<=0 || __vmax<_vc)
                                throw EImproperArgument(
                                        "vc should be in (0, vmax]");
                }
                else //pfin == psta
                        if(_vc<0 || __vmax<_vc)
                                throw EImproperArgument(
                                        "vc should be in [0, vmax]");
        }
        else { //vmax < 0
                //debe estar en el intervalo válido
                if(__pfin != __psta) {
                        if(_vc<__vmax || 0<=_vc)
                                throw EImproperArgument(
                                        "vc should be in [vmax, 0)");
                }
                else //pfin == psta
                        if(_vc<__vmax || 0<_vc)
                                throw EImproperArgument(
                                        "vc should be in [vmax, 0]");
        }

        __vc = _vc; //asigna el nuevo valor

        //calcula T
        if(__vc != 0)
                __T = __D/__vc;
        else //vc == 0
                __T = 0;
}

//--------------------------------------------------------------------------
//MÉTODOS:

//calcula los hitos previos a la elección de 'T'
//(vmax, Tmin)
void TSquareFunction::CalculateMilestones()
{
        __vmax = Sign(__D)*__vmaxabs; //calcula el valor algebraico de la velocidad
        __Tmin = __D/__vmax; //calcula el timepo mínimo para recorrer la distancia
}

//calcula la distancia máxima que se pueden recorrer en el tiempo 'T'
//(Smax)
void TSquareFunction::CalculateDistances()
{
        __Dmax = __vmax*__T;
}

//calcula los límites del dominio de las variables:
//[vcmin, vcmax] (en realidad solo vcmax, ya que vcmin es cte = 0)
void TSquareFunction::CalculateBoundaries(void)
{
        if(__D != 0)
                __vcmax = __vmax;
        else
                __vcmax = 0;
}

//calcula las variables derivadas de 'vc'
void TSquareFunction::CalculateVariables_vc(void)
{
        //cuando T=0 => D=0 => vc=0

        if(__T != 0)
                __vc = __D/__T;
        else //T == 0
                __vc = 0;
}

//---------------------------------------------------------------------------
//MÉTODOS:

//inicializa {vmax=vmaxabs, tini=0, tfin=0} y asimila los parámetros
TSquareFunction::TSquareFunction(double _vmaxabs) :
        TMotionFunction()
{
        //vmaxabs debe ser mayor que cero
        if(_vmaxabs <= 0)
                throw EImproperArgument("vmaxabs should be upper zero");

        //asigna valores a los parámetros impositivos no inicializados

        __vmax = __vmaxabs = _vmaxabs;

        //al inicializar todas las variables a cero
        //los siguientes pasos no son necesarios
/*
        __D = pfin - psta; //calcula la distancia a recorrer

        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        CalculateMilestones();

        //elige las variables de primer orden
        __T = Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo 'T'
        //(Smax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //arrastra 'vc' con los límties del dominio
        if(vcmax>0 && vc>vcmax)
                __vc = vcmax;
        else if(vcmax<0 && vc<vcmax)
                __vc = vcmax;

        //calcula la variable (vc)
        CalculateVariables_vc();*/
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
void TSquareFunction::Copy(TMotionFunction *_SquareFunction)
{
        //debe apuntar a un objeto construido
        if(_SquareFunction == NULL)
                throw EImproperArgument("pointer SquareFunction should not be null");

        //apunta el objeto con un puntero de la clase
        TSquareFunction *SquareFunction = (TSquareFunction*)_SquareFunction;

        //copia las propiedades heredadas
        __vmaxabs = SquareFunction->__vmaxabs;
        __psta = SquareFunction->__psta;
        __pfin = SquareFunction->__pfin;
        __vmax = SquareFunction->__vmax;
        __Tmin = SquareFunction->__Tmin;
        __D = SquareFunction->__D;
        __T = SquareFunction->__T;
        __Dmax = SquareFunction->__Dmax;
        __vcmin = SquareFunction->__vcmin;
        __vcmax = SquareFunction->__vcmax;
        __vc = SquareFunction->__vc;
}

//asigna (psta, pfin) conjuntamente
void TSquareFunction::SetInterval(double _psta, double _pfin)
{
        //asigna los nuevos valores
        __psta = _psta;
        __pfin = _pfin;

        __D = __pfin - __psta; //calcula la distancia a recorrer

        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(__T < __Tmin)
                __T = __Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Smax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //arrastra 'vc' con los límties del dominio
        if(__vcmax>0 && __vc>__vcmax)
                __vc = __vcmax;
        else if(__vcmax<0 && __vc<__vcmax)
                __vc = __vcmax;

        //calcula las variables correspondientes a 'vc'
        CalculateVariables_vc();
}

//invierte (psta, pfin) manteniendo el tiempo de desplazameinto T
//y la forma de la función
void TSquareFunction::InvertTime(void)
{
        //asigna los nuevos valores
        double aux = __psta;
        __psta = __pfin;
        __pfin = aux;

        __D = __pfin - __psta; //calcula la distancia a recorrer

        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        CalculateMilestones();

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Smax)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax]
        CalculateBoundaries();

        //invierte 'vc'
        __vc *= -1;

        //calcula las variables correspondientes a 'vc'
        CalculateVariables_vc();
}

//velocidad correspondiente al instante t en ul/ut
double TSquareFunction::v(double t)
{
        if(t <= 0)
                return 0;
        else if(t <= __T)
                return __vc;
        else
                return 0;
}

//posición correspondiente al instante t en ul
double TSquareFunction::p(double t)
{
        if(t <= 0)
                return __psta;
        else if(t <= __T)
                return __psta + __vc*t;
        else
                return __pfin;
}

//---------------------------------------------------------------------------

} //namespace MotionFunctions

//---------------------------------------------------------------------------

