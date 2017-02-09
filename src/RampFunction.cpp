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
//File: RampFunction.cpp
//Content: ramp motion function with acceleration and deceleration ramps
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include <math.h>

#include "RampFunction.h"
#include "Exceptions.h"
#include "Scalars.h"
#include "StrPR.h"

//---------------------------------------------------------------------------

using namespace Mathematics;
using namespace Strings;

//espacio de nombres de funciones de movimiento
namespace MotionFunctions {

//---------------------------------------------------------------------------
//TRampFunction
//---------------------------------------------------------------------------

void TRampFunction::setvmaxabs(double vmaxabs)
{
        //debe ser mayor que cero
        if(vmaxabs <= 0)
                throw EImproperArgument("vmaxabs should be upper zero");

        p_vmaxabs = vmaxabs; //asigna el nuevo valor

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tmin, Tv, av)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(p_T < p_Tmin)
                p_T = p_Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //arrastra 'Tc' con los límties del dominio
        if(p_Tc < p_Tcmin)
                p_Tc = p_Tcmin;
        else if(p_Tc > p_Tcmax)
                p_Tc = p_Tcmax;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}
void TRampFunction::setamaxabs(double amaxabs)
{
        //debe ser mayor que cero
        if(amaxabs <= 0)
                throw EImproperArgument("amaxabs should be upper zero");

        p_amaxabs = amaxabs; //asigna el nuevo valor

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tmin, Tv, av)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(p_T < p_Tmin)
                p_T = p_Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //arrastra 'Tc' con los límties del dominio
        if(p_Tc < p_Tcmin)
                p_Tc = p_Tcmin;
        else if(p_Tc > p_Tcmax)
                p_Tc = p_Tcmax;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}

void TRampFunction::setpsta(double psta)
{
        p_psta = psta; //asigna el nuevo valor

        p_D = p_pfin - p_psta; //calcula la distancia

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tv, Dv, av)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(p_T < p_Tmin)
                p_T = p_Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //arrastra 'Tc' con los límties del dominio
        if(p_Tc < p_Tcmin)
                p_Tc = p_Tcmin;
        else if(p_Tc > p_Tcmax)
                p_Tc = p_Tcmax;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}
void TRampFunction::setpfin(double pfin)
{
        p_pfin = pfin; //asigna el nuevo valor

        p_D = p_pfin - p_psta; //calcula la distancia

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tv, Dv, av)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(p_T < p_Tmin)
                p_T = p_Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //arrastra 'Tc' con los límties del dominio
        if(p_Tc < p_Tcmin)
                p_Tc = p_Tcmin;
        else if(p_Tc > p_Tcmax)
                p_Tc = p_Tcmax;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}

//------------------------------------------------------------------

void TRampFunction::setD(double D)
{
        p_D = D; //asigna el nuevo valor

        p_pfin = p_psta + p_D; //calcula la posición final

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tv, Dv, av)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(p_T < p_Tmin)
                p_T = p_Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //arrastra 'Tc' con los límties del dominio
        if(p_Tc < p_Tcmin)
                p_Tc = p_Tcmin;
        else if(p_Tc > p_Tcmax)
                p_Tc = p_Tcmax;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}

void TRampFunction::setT(double T)
{
        //debe dar tiempo a ir de psta a pfin con amax y vmax dadas
        if(T < p_Tmin)
                throw EImproperArgument("T should not be less than Tmin");

        p_T = T; //asigna el nuevo valor

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //arrastra 'Tc' con los límties del dominio
        if(p_Tc < p_Tcmin)
                p_Tc = p_Tcmin;
        else if(p_Tc > p_Tcmax)
                p_Tc = p_Tcmax;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}

//------------------------------------------------------------------

void TRampFunction::setvc(double vc)
{
        //debe estar en el intervalo válido
        if(p_vcmax>=0 && (vc<p_vcmin || p_vcmax<vc))
                throw EImproperArgument("vc should be in [vcmin, vcmax]");
        else if(p_vcmax<0 && (vc>p_vcmin || p_vcmax>vc))
                throw EImproperArgument("vc should be in [vcmin, vcmax]");

        p_vc = vc; //asigna el nuevo valor

        //calcula las variables correspondientes a 'vc': (a, Tr, Tc)
        CalculateVariables_vc();
}
void TRampFunction::setac(double ac)
{
        //debe estar en el intervalo válido
        if(p_acmax>=0 && (ac<p_acmin || p_acmax<ac))
                        throw EImproperArgument("ac should be in [acmin, acmax]");
        else if(p_acmax<0 && (ac>p_acmin || p_acmax>ac))
                        throw EImproperArgument("ac should be in [acmin, acmax]");

        p_ac = ac; //asigna el nuevo valor

        //calcula las variables correspondientes a 'ac': (vc, Tr, Tc)
        CalculateVariables_ac();
}
void TRampFunction::setTr(double Tr)
{
        //debe estar en el intervalo válido
        if(Tr<p_Trmin || p_Trmax<Tr)
                throw EImproperArgument("Tr should be in [Trmin, Trmax]");

        p_Tr = Tr; //asigna el nuevo valor

        //calcula las variables correspondientes a 'Tr': (vc, a, Tc)
        CalculateVariables_Tr();
}
void TRampFunction::setTc(double Tc)
{
        //debe estar en el intervalo válido
        if(Tc<p_Tcmin || p_Tcmax<Tc)
                throw EImproperArgument("Tc should be in [Tcmin, Tcmax]");

        p_Tc = Tc; //asigna el nuevo valor

        //calcula las variables correspondientes a 'Tc': (vc, a, Tr)
        CalculateVariables_Tc();
}

//------------------------------------------------------------------
//PROPIEDADES EN FORMATO TEXTO:

AnsiString TRampFunction::getamaxabsText(void) const
{
        return FloatToStr(p_amaxabs);
}
void TRampFunction::setamaxabsText(const AnsiString& S)
{
        try {
                setamaxabs(StrToFloat(S));
        } catch(...) {
                throw;
        }
}

AnsiString TRampFunction::getText(void)
{
        AnsiString S;

        S += AnsiString("vmaxabs=")+FloatToStr(p_vmaxabs)+AnsiString(";\r\n");
        S += AnsiString("amaxabs=")+FloatToStr(p_amaxabs)+AnsiString("\r\n");

        S += AnsiString("psta=")+FloatToStr(p_psta)+AnsiString(";\r\n");
        S += AnsiString("pfin=")+FloatToStr(p_pfin)+AnsiString(";\r\n");

        S += AnsiString("vmax=")+FloatToStr(p_vmax)+AnsiString(";\r\n");
        S += AnsiString("amax=")+FloatToStr(p_amax)+AnsiString(";\r\n");

        S += AnsiString("Tmin=")+FloatToStr(p_Tmin)+AnsiString(";\r\n");
        S += AnsiString("Tv=")+FloatToStr(p_Tv)+AnsiString(";\r\n");
        S += AnsiString("Tspr=")+FloatToStr(p_Tspr)+AnsiString(";\r\n");

        S += AnsiString("Dmax=")+FloatToStr(p_Dmax)+AnsiString(";\r\n");
        S += AnsiString("Dv=")+FloatToStr(p_Dv)+AnsiString(";\r\n");
        S += AnsiString("Dspr=")+FloatToStr(p_Dspr)+AnsiString(";\r\n");

        S += AnsiString("av=")+FloatToStr(p_av)+AnsiString(";\r\n");

        S += AnsiString("D=")+FloatToStr(p_D)+AnsiString(";\r\n");
        S += AnsiString("T=")+FloatToStr(p_T)+AnsiString(";\r\n");

        S += AnsiString("vcmax=")+FloatToStr(p_vcmax)+AnsiString(";\r\n");
        S += AnsiString("acmin=")+FloatToStr(p_acmin)+AnsiString(";\r\n");
        S += AnsiString("Trmax=")+FloatToStr(p_Trmax)+AnsiString(";\r\n");
        S += AnsiString("Tcmin=")+FloatToStr(p_Tcmin)+AnsiString(";\r\n");

        S += AnsiString("vcmin=")+FloatToStr(p_vcmin)+AnsiString(";\r\n");
        S += AnsiString("acmax=")+FloatToStr(p_acmax)+AnsiString(";\r\n");
        S += AnsiString("Trmin=")+FloatToStr(p_Trmin)+AnsiString(";\r\n");
        S += AnsiString("Tcmax=")+FloatToStr(p_Tcmax)+AnsiString(";\r\n");

        S += AnsiString("vc=")+FloatToStr(p_vc)+AnsiString(";\r\n");
        S += AnsiString("ac=")+FloatToStr(p_ac)+AnsiString(";\r\n");
        S += AnsiString("Tr=")+FloatToStr(p_Tr)+AnsiString(";\r\n");
        S += AnsiString("Tc=")+FloatToStr(p_Tc)+AnsiString(";\r\n");

        return S;
}

//------------------------------------------------------------------

//calcula los hitos previos a la elección de 'T'
void TRampFunction::CalculateMilestones(void)
//dados (psta, pfin, D)
//calcula (vmax, amax, Tspr, Dspr, Tmin, Tv, av)
{

        //calcula el valor algebraico
        p_vmax = Sign(p_D)*p_vmaxabs;
        p_amax = Sign(p_D)*p_amaxabs;

        //tiempo y distancia de sprint
        p_Tspr = 2*p_vmax/p_amax;
        p_Dspr = pow(p_vmax, 2.)/p_amax;

        if(p_psta == p_pfin) { //si v(t) es constante igual a cero
                p_Tmin = 0;
                p_Tv = p_Tmin;
                p_av = 0;
        }
        else if(Abs(p_D) <= Abs(p_Dspr)) { //si v(t) es triangular
                p_Tmin = 2*sqrt((p_D)/p_amax);
                p_Tv = p_Tmin;
                p_av = p_amax;
        }
        else { //si v(t) es trapezoidal
                p_Tmin = (p_D)/p_vmax + p_vmax/p_amax;
                p_Tv = 2*(p_D)/p_vmax;
                p_av = 2*p_vmax/p_Tv;
        }
}

//calcula las distancias máximas que se pueden recorrer en el tiempo 'T'
void TRampFunction::CalculateDistances()
//dados {(vmax, amax, psta, pfin, D), (Tspr, Dspr, Tmin, Tv, av), T}
//calcula (Dmax, Dv)
{
        if(p_T <= p_Tspr) { //si no da tiempo a superar vmax, v(t) será triangular
                p_Dmax = p_amax*pow(p_T, 2.)/4;
                p_Dv = p_Dmax;
        }
        else { //si da tiempo a superar vmax, de modo que v(t) será trapezoidal
                p_Dmax = p_vmax*(p_T - p_vmax/p_amax);
                p_Dv = p_vmax*p_T/2;
        }
}

//calcula los límites del dominio de las variables
void TRampFunction::CalculateBoundaries(void)
//dados {(vmax, amax, psta, pfin, D), (Tspr, Dspr, Tmin, Tv, av), T, (Dmax, Dv)}
//calcula [acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
{
        if(p_pfin != p_psta) {
                //cuando pfin != psta, vmax*T - D != 0

                //CALCULA (vcmax, acmin, Trmax, Tcmin):

                if(p_T < p_Tv) { //v(t) es trapezoidal
                        p_vcmax = p_vmax;
                        p_acmin = pow(p_vmax, 2.)/(p_vmax*p_T - p_D);
                        p_Trmax = p_vcmax/p_acmin;
                        p_Tcmin = p_T - 2*p_Trmax;
                }
                else { //v(t) es triangular
                        p_vcmax = 2*p_D/p_T;
                        p_acmin = 2*p_vcmax/p_T;
                        p_Trmax = p_T/2;
                        p_Tcmin = 0;
                }

                //CALCULA (vcmin, acmax, Trmin, Tcmax):

                //calcula las soluciones de la ecuación:
                //D = (T - vcmin/amax)*vcmin
                //-1/ac*vc^2 + T*vc - D = 0
                double root = pow(p_T, 2.) - 4*p_D/p_amax;
                //corrige el error numérico y recicla la variable
                if(root < 0)
                        root = 0;
                else
                        root = sqrt(root);
                //calcula las dos raices de la ecuación
                double vcmin1 = (p_T + root)/2*p_amax;
                double vcmin2 = (p_T - root)/2*p_amax;
                //selecciona la raiz que cumpla Tr <= T/2
                if(vcmin1==0 && vcmin2==0)
                        p_vcmin = vcmin1;
                        //auque este caso está contemplado en los dos siguientes
                        //se pone para evitar los efectos del error numérico
                        //que se da cuando root == 0
                else {
                        //calcula las variables antes de comparar para evitar
                        //errores numéricos
                        double Trmax1 = vcmin1/p_amax;
                        double Trmax2 = vcmin2/p_amax;
                        double T2 = p_T/2;
                        if(Trmax1 <= T2)
                                p_vcmin = vcmin1;
                        else if(Trmax2 <= T2)
                                p_vcmin = vcmin2;
                        else if(Trmax1 == Trmax2) /*REVISAR pues no parece un buen modo de soslayar los errores numéricos*/
                                p_vcmin = vcmin1; /*REVISAR pues no parece un buen modo de soslayar los errores numéricos*/
                        else
                                throw Exception("discarded error");
                }

                //calcula las demás variables
                p_acmax = p_amax;
                p_Trmin = p_vcmin/p_acmax;
                p_Tcmax = p_T - 2*p_Trmin;

                //cuando v(t) es triangular, el radicando es igual a cero:
                //pow(T, 2.) - 4*D/amax == 0

                //debido a a que T >= Tmin
                //      el radicando nunca es negativo
                //      siempre se cuemple:
                //              vcmin1/amax <= T/2 o vcmin2/amax <= T/2
        }
        else { //cuando pfin == psta, vmax == 0 y (vmax*T - D) == 0
                //CALCULA (vcmax, acmin, Trmax, Tcmin)
                p_vcmax = 0;
                p_acmin = 0;
                p_Trmax = 0;
                p_Tcmin = p_T;

                //CALCULA (vcmin, acmax, Trmin, Tcmax)
                p_vcmin = 0;
                p_acmax = 0;
                p_Trmin = 0;
                p_Tcmax = p_T;
        }
}

//calcula las variables correspondientes a 'vc': (ac, Tr, Tc)
void TRampFunction::CalculateVariables_vc(void)
{
        if(p_pfin != p_psta) { //si hay algún espacio que recorrer
                if(Abs(p_vc)<Abs(p_vcmax) || p_T<p_Tv) { //si v(t) no es triangular
                        p_ac = pow(p_vc, 2.)/(p_T*p_vc - p_D);
                        p_Tr = p_vc/p_ac;
                        p_Tc = p_T - 2*p_Tr;
                }
                else { //si v(t) es triangular
                        p_ac = 2*p_vc/p_T;
                        p_Tr = p_T/2;
                        p_Tc = 0;
                }
        }
        else { //si no hay espacio que recorrer
                p_ac = 0;
                p_Tr = 0;
                p_Tc = p_T;
        }
}
//calcula las variables correspondientes a 'ac': (vc, Tr, Tc)
void TRampFunction::CalculateVariables_ac(void)
{
        if(p_pfin != p_psta) { //si hay algún espacio que recorrer
                if(Abs(p_ac)>Abs(p_acmin) || p_T<p_Tv) { //si v(t) no es triangular
                        //calcula las soluciones de la ecuación:
                        //D = (T - vc/ac)*vc
                        //-1/ac*vc^2 + T*vc - D = 0

                        double root = pow(p_T, 2.) - 4*p_D/p_ac;
                        //corrige el error numérico y recicla la variable
                        if(root < 0)
                                root = 0;
                        else
                                root = sqrt(root);
                        //calcula las dos soluciones de la ecuación
                        double vc1 = p_ac/2*(p_T - root);
                        double vc2 = p_ac/2*(p_T + root);

                        //asigna a vc la solución que cumpla Tr<=T/2
                        if(vc1 == vc2)
                                p_vc = vc1;
                        else if(vc1/p_ac <= p_T/2)
                                p_vc = vc1;
                        else if(vc2/p_ac <= p_T/2)
                                p_vc = vc2;
                        else
                                throw Exception("discarded error");

                        //cuando v(t) es triangular, el radicando es cero:
                        //pow(T, 2.) - 4*D/amax == 0

                        //debido a que T >= Tmin
                        //      el radicando nunca es negativo
                        //      siempre se cuemple:
                        //              vcmin1/amax <= T/2 o vcmin2/amax <= T/2

                        //calcula las demás variables
                        p_Tr = p_vc/p_ac;
                        p_Tc = p_T - 2*p_Tr;
                }
                else { //si v(t) es triangular
                        p_vc = 2*p_D/p_T;
                        p_Tr = p_T/2;
                        p_Tc = 0;
                }
        }
        else { //si no hay espacio que recorrer
                p_vc = 0;
                p_Tr = 0;
                p_Tc = p_T;
        }
}
//calcula las variables correspondientes a 'Tr': (vc, ac, Tc)
void TRampFunction::CalculateVariables_Tr(void)
{
        if(p_pfin != p_psta) { //si hay algún espacio que recorrer
                if(p_Tr<p_Trmax || p_T<p_Tv) { //si v(t) no es triangular
                        p_vc = p_D/(p_T - p_Tr);
                        p_ac = pow(p_vc, 2.)/(p_T*p_vc - p_D);
                        p_Tc = p_T - 2.*p_Tr;
                }
                else { //si v(t) es triangular
                        p_vc = 2*p_D/p_T;
                        p_ac = 2*p_vc/p_T;
                        p_Tc = 0;
                }
        }
        else { //si no hay espacio que recorrer
                p_vc = 0;
                p_Tr = 0;
                p_Tc = p_T;
        }
}
//calcula las variables correspondientes a 'Tc': (vc, ac, Tr)
void TRampFunction::CalculateVariables_Tc(void)
{
        if(p_pfin != p_psta) { //si hay algún espacio que recorrer
                if(p_Tc>p_Tcmin || p_T<p_Tv) { //si v(t) no es triangular
                        p_Tr = (p_T - p_Tc)/2.;
                        p_vc = p_D/(p_T - p_Tr);
                        p_ac = pow(p_vc, 2.)/(p_T*p_vc - p_D);
                }
                else { //si v(t) es triangular
                        p_vc = 2*p_D/p_T;
                        p_ac = 2*p_vc/p_T;
                        p_Tr = p_T/2;
                }
        }
        else { //si no hay espacio que recorrer
                p_vc = 0;
                p_ac = 0;
                p_Tr = 0;
        }
}

//------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//inicialize:
//  amax=amaxabs,
//  vmax=vmaxabs,
//  psta=0, pfin=0;
//and assimilate the parameters
TRampFunction::TRampFunction(double amaxabs, double vmaxabs) :
    TMotionFunction() //inicializa las propiedades heredadas
{
        //deben ser mayores que cero
        if(amaxabs <= 0)
                throw EImproperArgument("amaxabs should be upper zero");
        if(vmaxabs <= 0)
                throw EImproperArgument("vmaxabs should be upper zero");

        //asigna valores a los parámetros impositivos no inicializados

        p_vmaxabs = vmaxabs;
        p_amaxabs = amaxabs;

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tv, Dv, av)
        CalculateMilestones();

        //elige 'T' adecuado para que la función v(t) pueda ser triangular
        p_T = p_Tv;

        //calcula las distancias máximas que se pueden recorrer en el tiempo 'T'
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[amin, amax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //elige Tc lo más próximo a v(t) triangular
        p_Tc = p_Tcmin;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}
//clona una función rampa
TRampFunction::TRampFunction(TRampFunction *RampFunction) :
        TMotionFunction(RampFunction) //clona las propiedades heredadas
{
        //debe apuntar a un objeto construido
        if(RampFunction == NULL)
                throw EImproperArgument("pointer RampFunction should not be null");

        //copia todas las propiedades no heredadas
        p_amaxabs = RampFunction->p_amaxabs;
        p_amax = RampFunction->p_amax;
        p_Tspr = RampFunction->p_Tspr;
        p_Dspr = RampFunction->p_Dspr;
        p_Tmin = RampFunction->p_Tmin;
        p_Tv = RampFunction->p_Tv;
        p_av = RampFunction->p_av;
        p_Dv = RampFunction->p_Dv;
        p_acmin = RampFunction->p_acmin;
        p_Trmax = RampFunction->p_Trmax;
        p_Tcmin = RampFunction->p_Tcmin;
        p_acmax = RampFunction->p_acmax;
        p_Trmin = RampFunction->p_Trmin;
        p_Tcmax = RampFunction->p_Tcmax;
        p_ac = RampFunction->p_ac;
        p_Tr = RampFunction->p_Tr;
        p_Tc = RampFunction->p_Tc;
}
//copia todas las propiedades de una función rampa
void TRampFunction::Copy(TMotionFunction *RampFunction)
{
        //debe apuntar a un objeto construido
        if(RampFunction == NULL)
                throw EImproperArgument("pointer RampFunction should not be null");

        //apunta el objeto con un puntero de la clase
        TRampFunction *aux = (TRampFunction*)RampFunction;

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

        //copia las propiedades no heredadas
        p_amaxabs = aux->p_amaxabs;
        p_amax = aux->p_amax;
        p_Tspr = aux->p_Tspr;
        p_Dspr = aux->p_Dspr;
        p_Tmin = aux->p_Tmin;
        p_Tv = aux->p_Tv;
        p_av = aux->p_av;
        p_Dv = aux->p_Dv;
        p_acmin = aux->p_acmin;
        p_Trmax = aux->p_Trmax;
        p_Tcmin = aux->p_Tcmin;
        p_acmax = aux->p_acmax;
        p_Trmin = aux->p_Trmin;
        p_Tcmax = aux->p_Tcmax;
        p_ac = aux->p_ac;
        p_Tr = aux->p_Tr;
        p_Tc = aux->p_Tc;
}

//initalize all properties except (vmaxabs, amaxabs)
void TRampFunction::reset(void)
{
    //INITIALIZE THE INHERITED PROPERTIES:

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

    //calcula los hitos previos a la elección de 'T'
    //(vmax, amax, Tspr, Dspr, Tv, Dv, av)
    CalculateMilestones();

    //elige 'T' adecuado para que la función v(t) pueda ser triangular
    p_T = p_Tv;

    //calcula las distancias máximas que se pueden recorrer en el tiempo 'T'
    //(Dmax, Dv)
    CalculateDistances();

    //calcula los límites del dominio de las variables:
    //[amin, amax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
    CalculateBoundaries();

    //elige Tc lo más próximo a v(t) triangular
    p_Tc = p_Tcmin;

    //calcula las variables correspondientes a 'Tc'
    CalculateVariables_Tc();
}

//asigna (psta, pfin)
void TRampFunction::SetInterval(double psta, double pfin)
{
        p_psta = psta; //asigna el nuevo valor
        p_pfin = pfin; //asigna el nuevo valor

        p_D = p_pfin - p_psta; //calcula la distancia

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tv, Dv, av)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(p_T < p_Tmin)
                p_T = p_Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo 'T'
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //arrastra 'Tc' con los límties del dominio
        if(p_Tc < p_Tcmin)
                p_Tc = p_Tcmin;
        else if(p_Tc > p_Tcmax)
                p_Tc = p_Tcmax;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}
//invierte (psta, pfin) manteniendo el tiempo de desplazameinto T
//y la forma de la función
void TRampFunction::InvertTime(void)
{
        //invierte (psta, pfin)
        double aux = p_psta;
        p_psta = p_pfin;
        p_pfin = aux;

        p_D = p_pfin - p_psta; //calcula la distancia

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tv, Dv, av)
        CalculateMilestones();

        //calcula las distancias máximas que se pueden recorrer en el tiempo 'T'
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}

//devuelve la velocidad correspondiente al instante t
double TRampFunction::v(double t)
{
        if(t <= 0)
                return 0;
        else if(t <= p_Tr)
                return p_ac*t;
        else if(t <= p_Tr+p_Tc)
                return p_vc;
        else if(t < p_T)
                return p_vc - p_ac*(t - p_Tr - p_Tc);
        else
                return 0;
}

//devuelve el paso correspondiente al instante t
//t debe estar en [0, T]
double TRampFunction::p(double t)
{
        if(t <= 0)
                return p_psta;
        else if(t <= p_Tr)
                return p_psta + t*t*p_ac/2;
        else if(t <= p_Tr+p_Tc)
                return p_psta + p_Tr*p_vc/2 + (t - p_Tr)*p_vc;
        else if(t < p_T)
                return p_psta + (p_Tr + p_Tc)*p_vc - pow(p_T-t, 2.)*p_ac/2;
        else
                return p_pfin;
}

//---------------------------------------------------------------------------

} //namespace MotionFunctions

//---------------------------------------------------------------------------

