//---------------------------------------------------------------------------
//Archivo: RampFunction.cpp
//Contenido: funcion de movimiento con rampas de aceleración y deceleración
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include <math.h>

#include "RampFunction.h"
#include "..\0_VCL\Exceptions.h"
#include "..\1_Mathematics\Scalars.h"
//---------------------------------------------------------------------------

using namespace Mathematics;

//espacio de nombres de funciones de movimiento
namespace MotionFunctions {

//---------------------------------------------------------------------------
//TRampFunction
//---------------------------------------------------------------------------

void TRampFunction::setvmaxabs(double _vmaxabs)
{
        //debe ser mayor que cero
        if(_vmaxabs <= 0)
                throw EImproperArgument("vmaxabs should be upper zero");

        __vmaxabs = _vmaxabs; //asigna el nuevo valor

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tmin, Tv, av)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(__T < __Tmin)
                __T = __Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //arrastra 'Tc' con los límties del dominio
        if(__Tc < __Tcmin)
                __Tc = __Tcmin;
        else if(__Tc > __Tcmax)
                __Tc = __Tcmax;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}
void TRampFunction::setamaxabs(double _amaxabs)
{
        //debe ser mayor que cero
        if(_amaxabs <= 0)
                throw EImproperArgument("amaxabs should be upper zero");

        __amaxabs = _amaxabs; //asigna el nuevo valor

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tmin, Tv, av)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(__T < __Tmin)
                __T = __Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //arrastra 'Tc' con los límties del dominio
        if(__Tc < __Tcmin)
                __Tc = __Tcmin;
        else if(__Tc > __Tcmax)
                __Tc = __Tcmax;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}

void TRampFunction::setpsta(double _psta)
{
        __psta = _psta; //asigna el nuevo valor

        __D = __pfin - __psta; //calcula la distancia

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tv, Dv, av)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(__T < __Tmin)
                __T = __Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //arrastra 'Tc' con los límties del dominio
        if(__Tc < __Tcmin)
                __Tc = __Tcmin;
        else if(__Tc > __Tcmax)
                __Tc = __Tcmax;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}
void TRampFunction::setpfin(double _pfin)
{
        __pfin = _pfin; //asigna el nuevo valor

        __D = __pfin - __psta; //calcula la distancia

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tv, Dv, av)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(__T < __Tmin)
                __T = __Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //arrastra 'Tc' con los límties del dominio
        if(__Tc < __Tcmin)
                __Tc = __Tcmin;
        else if(__Tc > __Tcmax)
                __Tc = __Tcmax;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}

//------------------------------------------------------------------

void TRampFunction::setD(double _D)
{
        __D = _D; //asigna el nuevo valor

        __pfin = __psta + __D; //calcula la posición final

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tv, Dv, av)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(__T < __Tmin)
                __T = __Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //arrastra 'Tc' con los límties del dominio
        if(__Tc < __Tcmin)
                __Tc = __Tcmin;
        else if(__Tc > __Tcmax)
                __Tc = __Tcmax;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}

void TRampFunction::setT(double _T)
{
        //debe dar tiempo a ir de psta a pfin con amax y vmax dadas
        if(_T < __Tmin)
                throw EImproperArgument("T should not be less than Tmin");

        __T = _T; //asigna el nuevo valor

        //calcula las distancias máximas que se pueden recorrer en el tiempo T
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //arrastra 'Tc' con los límties del dominio
        if(__Tc < __Tcmin)
                __Tc = __Tcmin;
        else if(__Tc > __Tcmax)
                __Tc = __Tcmax;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}

//------------------------------------------------------------------

void TRampFunction::setvc(double _vc)
{
        //debe estar en el intervalo válido
        if(__vcmax>=0 && (_vc<__vcmin || __vcmax<_vc))
                throw EImproperArgument("vc should be in [vcmin, vcmax]");
        else if(__vcmax<0 && (_vc>__vcmin || __vcmax>_vc))
                throw EImproperArgument("vc should be in [vcmin, vcmax]");

        __vc = _vc; //asigna el nuevo valor

        //calcula las variables correspondientes a 'vc': (a, Tr, Tc)
        CalculateVariables_vc();
}
void TRampFunction::setac(double _ac)
{
        //debe estar en el intervalo válido
        if(__acmax>=0 && (_ac<__acmin || __acmax<_ac))
                        throw EImproperArgument("ac should be in [acmin, acmax]");
        else if(__acmax<0 && (_ac>__acmin || __acmax>_ac))
                        throw EImproperArgument("ac should be in [acmin, acmax]");

        __ac = _ac; //asigna el nuevo valor

        //calcula las variables correspondientes a 'ac': (vc, Tr, Tc)
        CalculateVariables_ac();
}
void TRampFunction::setTr(double _Tr)
{
        //debe estar en el intervalo válido
        if(_Tr<__Trmin || __Trmax<_Tr)
                throw EImproperArgument("Tr should be in [Trmin, Trmax]");

        __Tr = _Tr; //asigna el nuevo valor

        //calcula las variables correspondientes a 'Tr': (vc, a, Tc)
        CalculateVariables_Tr();
}
void TRampFunction::setTc(double _Tc)
{
        //debe estar en el intervalo válido
        if(_Tc<__Tcmin || __Tcmax<_Tc)
                throw EImproperArgument("Tc should be in [Tcmin, Tcmax]");

        __Tc = _Tc; //asigna el nuevo valor

        //calcula las variables correspondientes a 'Tc': (vc, a, Tr)
        CalculateVariables_Tc();
}

//------------------------------------------------------------------
//PROPIEDADES EN FORMATO TEXTO:

AnsiString TRampFunction::getText(void)
{
        AnsiString S;

        S += AnsiString("vmaxabs=")+FloatToStr(__vmaxabs)+AnsiString(";\r\n");
        S += AnsiString("amaxabs=")+FloatToStr(__amaxabs)+AnsiString("\r\n");

        S += AnsiString("psta=")+FloatToStr(__psta)+AnsiString(";\r\n");
        S += AnsiString("pfin=")+FloatToStr(__pfin)+AnsiString(";\r\n");

        S += AnsiString("vmax=")+FloatToStr(__vmax)+AnsiString(";\r\n");
        S += AnsiString("amax=")+FloatToStr(__amax)+AnsiString(";\r\n");

        S += AnsiString("Tmin=")+FloatToStr(__Tmin)+AnsiString(";\r\n");
        S += AnsiString("Tv=")+FloatToStr(__Tv)+AnsiString(";\r\n");
        S += AnsiString("Tspr=")+FloatToStr(__Tspr)+AnsiString(";\r\n");

        S += AnsiString("Dmax=")+FloatToStr(__Dmax)+AnsiString(";\r\n");
        S += AnsiString("Dv=")+FloatToStr(__Dv)+AnsiString(";\r\n");
        S += AnsiString("Dspr=")+FloatToStr(__Dspr)+AnsiString(";\r\n");

        S += AnsiString("av=")+FloatToStr(__av)+AnsiString(";\r\n");

        S += AnsiString("D=")+FloatToStr(__D)+AnsiString(";\r\n");
        S += AnsiString("T=")+FloatToStr(__T)+AnsiString(";\r\n");

        S += AnsiString("vcmax=")+FloatToStr(__vcmax)+AnsiString(";\r\n");
        S += AnsiString("acmin=")+FloatToStr(__acmin)+AnsiString(";\r\n");
        S += AnsiString("Trmax=")+FloatToStr(__Trmax)+AnsiString(";\r\n");
        S += AnsiString("Tcmin=")+FloatToStr(__Tcmin)+AnsiString(";\r\n");

        S += AnsiString("vcmin=")+FloatToStr(__vcmin)+AnsiString(";\r\n");
        S += AnsiString("acmax=")+FloatToStr(__acmax)+AnsiString(";\r\n");
        S += AnsiString("Trmin=")+FloatToStr(__Trmin)+AnsiString(";\r\n");
        S += AnsiString("Tcmax=")+FloatToStr(__Tcmax)+AnsiString(";\r\n");

        S += AnsiString("vc=")+FloatToStr(__vc)+AnsiString(";\r\n");
        S += AnsiString("ac=")+FloatToStr(__ac)+AnsiString(";\r\n");
        S += AnsiString("Tr=")+FloatToStr(__Tr)+AnsiString(";\r\n");
        S += AnsiString("Tc=")+FloatToStr(__Tc)+AnsiString(";\r\n");

        return S;
}

//------------------------------------------------------------------

//calcula los hitos previos a la elección de 'T'
void TRampFunction::CalculateMilestones(void)
//dados (psta, pfin, D)
//calcula (vmax, amax, Tspr, Dspr, Tmin, Tv, av)
{

        //calcula el valor algebraico
        __vmax = Sign(__D)*__vmaxabs;
        __amax = Sign(__D)*__amaxabs;

        //tiempo y distancia de sprint
        __Tspr = 2*__vmax/__amax;
        __Dspr = pow(__vmax, 2.)/__amax;

        if(__psta == __pfin) { //si v(t) es constante igual a cero
                __Tmin = 0;
                __Tv = __Tmin;
                __av = 0;
        }
        else if(Abs(__D) <= Abs(__Dspr)) { //si v(t) es triangular
                __Tmin = 2*sqrt((__D)/__amax);
                __Tv = __Tmin;
                __av = __amax;
        }
        else { //si v(t) es trapezoidal
                __Tmin = (__D)/__vmax + __vmax/__amax;
                __Tv = 2*(__D)/__vmax;
                __av = 2*__vmax/__Tv;
        }
}

//calcula las distancias máximas que se pueden recorrer en el tiempo 'T'
void TRampFunction::CalculateDistances()
//dados {(vmax, amax, psta, pfin, D), (Tspr, Dspr, Tmin, Tv, av), T}
//calcula (Dmax, Dv)
{
        if(__T <= __Tspr) { //si no da tiempo a superar vmax, v(t) será triangular
                __Dmax = __amax*pow(__T, 2.)/4;
                __Dv = __Dmax;
        }
        else { //si da tiempo a superar vmax, de modo que v(t) será trapezoidal
                __Dmax = __vmax*(__T - __vmax/__amax);
                __Dv = __vmax*__T/2;
        }
}

//calcula los límites del dominio de las variables
void TRampFunction::CalculateBoundaries(void)
//dados {(vmax, amax, psta, pfin, D), (Tspr, Dspr, Tmin, Tv, av), T, (Dmax, Dv)}
//calcula [acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
{
        if(__pfin != __psta) {
                //cuando pfin != psta, vmax*T - D != 0

                //CALCULA (vcmax, acmin, Trmax, Tcmin):

                if(__T < __Tv) { //v(t) es trapezoidal
                        __vcmax = __vmax;
                        __acmin = pow(__vmax, 2.)/(__vmax*__T - __D);
                        __Trmax = __vcmax/__acmin;
                        __Tcmin = __T - 2*__Trmax;
                }
                else { //v(t) es triangular
                        __vcmax = 2*__D/__T;
                        __acmin = 2*__vcmax/__T;
                        __Trmax = __T/2;
                        __Tcmin = 0;
                }

                //CALCULA (vcmin, acmax, Trmin, Tcmax):

                //calcula las soluciones de la ecuación:
                //D = (T - vcmin/amax)*vcmin
                //-1/ac*vc^2 + T*vc - D = 0
                double root = pow(__T, 2.) - 4*__D/__amax;
                //corrige el error numérico y recicla la variable
                if(root < 0)
                        root = 0;
                else
                        root = sqrt(root);
                //calcula las dos raices de la ecuación
                double vcmin1 = (__T + root)/2*__amax;
                double vcmin2 = (__T - root)/2*__amax;
                //selecciona la raiz que cumpla Tr <= T/2
                if(vcmin1==0 && vcmin2==0)
                        __vcmin = vcmin1;
                        //auque este caso está contemplado en los dos siguientes
                        //se pone para evitar los efectos del error numérico
                        //que se da cuando root == 0
                else {
                        //calcula las variables antes de comparar para evitar
                        //errores numéricos
                        double Trmax1 = vcmin1/__amax;
                        double Trmax2 = vcmin2/__amax;
                        double T2 = __T/2;
                        if(Trmax1 <= T2)
                                __vcmin = vcmin1;
                        else if(Trmax2 <= T2)
                                __vcmin = vcmin2;
                        else if(Trmax1 == Trmax2) /*REVISAR pues no parece un buen modo de soslayar los errores numéricos*/
                                __vcmin = vcmin1; /*REVISAR pues no parece un buen modo de soslayar los errores numéricos*/
                        else
                                throw Exception("discarded error");
                }

                //calcula las demás variables
                __acmax = __amax;
                __Trmin = __vcmin/__acmax;
                __Tcmax = __T - 2*__Trmin;

                //cuando v(t) es triangular, el radicando es igual a cero:
                //pow(T, 2.) - 4*D/amax == 0

                //debido a a que T >= Tmin
                //      el radicando nunca es negativo
                //      siempre se cuemple:
                //              vcmin1/amax <= T/2 o vcmin2/amax <= T/2
        }
        else { //cuando pfin == psta, vmax == 0 y (vmax*T - D) == 0
                //CALCULA (vcmax, acmin, Trmax, Tcmin)
                __vcmax = 0;
                __acmin = 0;
                __Trmax = 0;
                __Tcmin = __T;

                //CALCULA (vcmin, acmax, Trmin, Tcmax)
                __vcmin = 0;
                __acmax = 0;
                __Trmin = 0;
                __Tcmax = __T;
        }
}

//calcula las variables correspondientes a 'vc': (ac, Tr, Tc)
void TRampFunction::CalculateVariables_vc(void)
{
        if(__pfin != __psta) { //si hay algún espacio que recorrer
                if(Abs(__vc)<Abs(__vcmax) || __T<__Tv) { //si v(t) no es triangular
                        __ac = pow(__vc, 2.)/(__T*__vc - __D);
                        __Tr = __vc/__ac;
                        __Tc = __T - 2*__Tr;
                }
                else { //si v(t) es triangular
                        __ac = 2*__vc/__T;
                        __Tr = __T/2;
                        __Tc = 0;
                }
        }
        else { //si no hay espacio que recorrer
                __ac = 0;
                __Tr = 0;
                __Tc = __T;
        }
}
//calcula las variables correspondientes a 'ac': (vc, Tr, Tc)
void TRampFunction::CalculateVariables_ac(void)
{
        if(__pfin != __psta) { //si hay algún espacio que recorrer
                if(Abs(__ac)>Abs(__acmin) || __T<__Tv) { //si v(t) no es triangular
                        //calcula las soluciones de la ecuación:
                        //D = (T - vc/ac)*vc
                        //-1/ac*vc^2 + T*vc - D = 0

                        double root = pow(__T, 2.) - 4*__D/__ac;
                        //corrige el error numérico y recicla la variable
                        if(root < 0)
                                root = 0;
                        else
                                root = sqrt(root);
                        //calcula las dos soluciones de la ecuación
                        double vc1 = __ac/2*(__T - root);
                        double vc2 = __ac/2*(__T + root);

                        //asigna a vc la solución que cumpla Tr<=T/2
                        if(vc1 == vc2)
                                __vc = vc1;
                        else if(vc1/__ac <= __T/2)
                                __vc = vc1;
                        else if(vc2/__ac <= __T/2)
                                __vc = vc2;
                        else
                                throw Exception("discarded error");

                        //cuando v(t) es triangular, el radicando es cero:
                        //pow(T, 2.) - 4*D/amax == 0

                        //debido a que T >= Tmin
                        //      el radicando nunca es negativo
                        //      siempre se cuemple:
                        //              vcmin1/amax <= T/2 o vcmin2/amax <= T/2

                        //calcula las demás variables
                        __Tr = __vc/__ac;
                        __Tc = __T - 2*__Tr;
                }
                else { //si v(t) es triangular
                        __vc = 2*__D/__T;
                        __Tr = __T/2;
                        __Tc = 0;
                }
        }
        else { //si no hay espacio que recorrer
                __vc = 0;
                __Tr = 0;
                __Tc = __T;
        }
}
//calcula las variables correspondientes a 'Tr': (vc, ac, Tc)
void TRampFunction::CalculateVariables_Tr(void)
{
        if(__pfin != __psta) { //si hay algún espacio que recorrer
                if(__Tr<__Trmax || __T<__Tv) { //si v(t) no es triangular
                        __vc = __D/(__T - __Tr);
                        __ac = pow(__vc, 2.)/(__T*__vc - __D);
                        __Tc = __T - 2.*__Tr;
                }
                else { //si v(t) es triangular
                        __vc = 2*__D/__T;
                        __ac = 2*__vc/__T;
                        __Tc = 0;
                }
        }
        else { //si no hay espacio que recorrer
                __vc = 0;
                __Tr = 0;
                __Tc = __T;
        }
}
//calcula las variables correspondientes a 'Tc': (vc, ac, Tr)
void TRampFunction::CalculateVariables_Tc(void)
{
        if(__pfin != __psta) { //si hay algún espacio que recorrer
                if(__Tc>__Tcmin || __T<__Tv) { //si v(t) no es triangular
                        __Tr = (__T - __Tc)/2.;
                        __vc = __D/(__T - __Tr);
                        __ac = pow(__vc, 2.)/(__T*__vc - __D);
                }
                else { //si v(t) es triangular
                        __vc = 2*__D/__T;
                        __ac = 2*__vc/__T;
                        __Tr = __T/2;
                }
        }
        else { //si no hay espacio que recorrer
                __vc = 0;
                __ac = 0;
                __Tr = 0;
        }
}

//------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//inicializa {amax=amaxabs=amaxabsdef, vmax=vmaxabs=vmaxabsdef,
//psta=0, pfin=0} y asimila los parámetros
TRampFunction::TRampFunction(double _amaxabs, double _vmaxabs) :
    TMotionFunction() //inicializa las propiedades heredadas
{
        //deben ser mayores que cero
        if(_amaxabs <= 0)
                throw EImproperArgument("amaxabs should be upper zero");
        if(_vmaxabs <= 0)
                throw EImproperArgument("vmaxabs should be upper zero");

        //asigna valores a los parámetros impositivos no inicializados

        __vmaxabs = _vmaxabs;
        __amaxabs = _amaxabs;

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tv, Dv, av)
        CalculateMilestones();

        //elige 'T' adecuado para que la función v(t) pueda ser triangular
        __T = __Tv;

        //calcula las distancias máximas que se pueden recorrer en el tiempo 'T'
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[amin, amax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //elige Tc lo más próximo a v(t) triangular
        __Tc = __Tcmin;

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
        __amaxabs = RampFunction->__amaxabs;
        __amax = RampFunction->__amax;
        __Tspr = RampFunction->__Tspr;
        __Dspr = RampFunction->__Dspr;
        __Tmin = RampFunction->__Tmin;
        __Tv = RampFunction->__Tv;
        __av = RampFunction->__av;
        __Dv = RampFunction->__Dv;
        __acmin = RampFunction->__acmin;
        __Trmax = RampFunction->__Trmax;
        __Tcmin = RampFunction->__Tcmin;
        __acmax = RampFunction->__acmax;
        __Trmin = RampFunction->__Trmin;
        __Tcmax = RampFunction->__Tcmax;
        __ac = RampFunction->__ac;
        __Tr = RampFunction->__Tr;
        __Tc = RampFunction->__Tc;
}
//copia todas las propiedades de una función rampa
void TRampFunction::Copy(TMotionFunction *_RampFunction)
{
        //debe apuntar a un objeto construido
        if(_RampFunction == NULL)
                throw EImproperArgument("pointer RampFunction should not be null");

        //apunta el objeto con un puntero de la clase
        TRampFunction *RampFunction = (TRampFunction*)_RampFunction;

        //copia las propiedades heredadas
        __vmaxabs = RampFunction->__vmaxabs;
        __psta = RampFunction->__psta;
        __pfin = RampFunction->__pfin;
        __vmax = RampFunction->__vmax;
        __Tmin = RampFunction->__Tmin;
        __D = RampFunction->__D;
        __T = RampFunction->__T;
        __Dmax = RampFunction->__Dmax;
        __vcmin = RampFunction->__vcmin;
        __vcmax = RampFunction->__vcmax;
        __vc = RampFunction->__vc;

        //copia las propiedades no heredadas
        __amaxabs = RampFunction->__amaxabs;
        __amax = RampFunction->__amax;
        __Tspr = RampFunction->__Tspr;
        __Dspr = RampFunction->__Dspr;
        __Tmin = RampFunction->__Tmin;
        __Tv = RampFunction->__Tv;
        __av = RampFunction->__av;
        __Dv = RampFunction->__Dv;
        __acmin = RampFunction->__acmin;
        __Trmax = RampFunction->__Trmax;
        __Tcmin = RampFunction->__Tcmin;
        __acmax = RampFunction->__acmax;
        __Trmin = RampFunction->__Trmin;
        __Tcmax = RampFunction->__Tcmax;
        __ac = RampFunction->__ac;
        __Tr = RampFunction->__Tr;
        __Tc = RampFunction->__Tc;
}

//asigna (psta, pfin)
void TRampFunction::SetInterval(double _psta, double _pfin)
{
        __psta = _psta; //asigna el nuevo valor
        __pfin = _pfin; //asigna el nuevo valor

        __D = __pfin - __psta; //calcula la distancia

        //calcula los hitos previos a la elección de 'T'
        //(vmax, amax, Tspr, Dspr, Tv, Dv, av)
        CalculateMilestones();

        //arrastra 'T' con el límite del dominio
        if(__T < __Tmin)
                __T = __Tmin;

        //calcula las distancias máximas que se pueden recorrer en el tiempo 'T'
        //(Dmax, Dv)
        CalculateDistances();

        //calcula los límites del dominio de las variables:
        //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
        CalculateBoundaries();

        //arrastra 'Tc' con los límties del dominio
        if(__Tc < __Tcmin)
                __Tc = __Tcmin;
        else if(__Tc > __Tcmax)
                __Tc = __Tcmax;

        //calcula las variables correspondientes a 'Tc'
        CalculateVariables_Tc();
}
//invierte (psta, pfin) manteniendo el tiempo de desplazameinto T
//y la forma de la función
void TRampFunction::InvertTime(void)
{
        //invierte (psta, pfin)
        double aux = __psta;
        __psta = __pfin;
        __pfin = aux;

        __D = __pfin - __psta; //calcula la distancia

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
        else if(t <= __Tr)
                return __ac*t;
        else if(t <= __Tr+__Tc)
                return __vc;
        else if(t < __T)
                return __vc - __ac*(t - __Tr - __Tc);
        else
                return 0;
}

//devuelve el paso correspondiente al instante t
//t debe estar en [0, T]
double TRampFunction::p(double t)
{
        if(t <= 0)
                return __psta;
        else if(t <= __Tr)
                return __psta + t*t*__ac/2;
        else if(t <= __Tr+__Tc)
                return __psta + __Tr*__vc/2 + (t - __Tr)*__vc;
        else if(t < __T)
                return __psta + (__Tr + __Tc)*__vc - pow(__T-t, 2.)*__ac/2;
        else
                return __pfin;
}

//---------------------------------------------------------------------------

} //namespace MotionFunctions

//---------------------------------------------------------------------------

