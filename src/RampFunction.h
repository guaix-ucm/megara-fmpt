//---------------------------------------------------------------------------
//Archivo: RampFunction.h
//Contenido: funcion de movimiento con rampas de aceleración y deceleración
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef RAMPFUNCTION_H
#define RAMPFUNCTION_H

#include "MotionFunction.h"

//---------------------------------------------------------------------------

//espacio de nombres de funciones de movimiento
namespace MotionFunctions {

//---------------------------------------------------------------------------
//TRampFunction
//---------------------------------------------------------------------------

//clase función de movimiento con rampas de aceleración y deceleración
class TRampFunction : public TMotionFunction {
    //PARÁMETROS IMPOSITIVOS:

    double __amaxabs;
    double __amax;

    //HITOS PREVIOS A LA ELECCIÓN DE T:

    double __Tspr;
    double __Dspr;
    double __Tv;
    double __av;

    //TIEMPO DE DESPLAZAMIENTO ELEGIDO:

    //DISTANCIAS MÁXIMAS RECORRIBLES EN EL TIEMPO 'T'

    double __Dv;

    //LÍMITES DE LOS INTERVALOS DE DEFINICIÓN:

    double __vcmax;
    double __acmin;
    double __Trmax;
    double __Tcmin;

    double __vcmin;
    double __acmax;
    double __Trmin;
    double __Tcmax;

    //VARIABLES:

    double __ac;
    double __Tr;
    double __Tc;

    //puesto que acmax coincide siempre con amax
    //puede verse tentado de llamar 'a' (en vez de 'ac') a la aceleración
    //lo cual constituiría un abuso de notación

    //PROPIEDADES EN FORMATO TEXTO:


    //MÉTODOS DE ASIMILACIÓN DE PARÁMETROS:

    //calcula los hitos previos a la elección de 'T'
    //(Tspr, Dspr, Tmin, Tv, av)
    void CalculateMilestones(void);
    //calcula las distancias máximas que se pueden recorrer en el tiempo 'T'
    //(Dmax, Dv)
    void CalculateDistances(void);
    //calcula los límites del dominio de las variables:
    //[acmin, acmax], [vcmin, vcmax], [Trmin, Trmax], [Tcmin, Tcmax]
    void CalculateBoundaries(void);

    //calcula las variables correspondientes a 'vc': (ac, Tr, Tc)
    void CalculateVariables_vc(void);
    //calcula las variables correspondientes a 'ac': (vc, Tr, Tc)
    void CalculateVariables_ac(void);
    //calcula las variables correspondientes a 'Tr': (vc, ac, Tc)
    void CalculateVariables_Tr(void);
    //calcula las variables correspondientes a 'Tc': (vc, ac, Tr)
    void CalculateVariables_Tc(void);

    //Cuando se cambia una de las variables, dicha variable es arrastrada
    //dentro de sus límites y las demás avriables derivadas a partir
    //de su valor.
    //Cuando cambia 'T' o 'D', los límites de las variables pueden cambiar
    //y las variables deben ser arrastradas dentro de los límites.
    //La variable elegida para ser arrstrada és 'Tc', ya que cuando
    //v(t) es triangular, su valor debe ser cero y un error numérico
    //puede resultar tangible.

public:
    //PARÁMETROS IMPOSITIVOS:

    //velocidad máxima absoluta en ul/ut
    //debe ser mayor que cero
    void setvmaxabs(double);

    //aceleración máxima absoluta del eje (en ul/ut^2)
    //no debe ser menor que anomabs
    double getamaxabs(void) const {return __amaxabs;}
    void setamaxabs(double);

    //posición de salida (starting) en ul
    void setpsta(double);
    //posición de llegada (finishing) en ul
    void setpfin(double);

    //aceleración máxima algebraica del eje (en ul/ut^2)
    //amax = amaxabs*sign(pfin-psta)
    double getamax(void) const {return __amax;}

    //HITOS PREVIOS A LA ELECCIÓN DE 'T':

    //tiempo que lleva hacer un sprint completo
    //acelerando hasta alcanzar vmax con amax y
    //decelerando hasta alcanzar cero con -amax
    double getTspr(void) const {return __Tspr;}
    //distancia recorrida al hacer un sprint compelto
    //acelerando hasta alcanzar vmax con amax y
    //decelerando hasta alcanzar cero con -amax
    double getDspr(void) const {return __Dspr;}

    //tiempo mínimo para ir de psta a pfin
    //con ac<amaxabs y vc<=vmaxabs
    double getTmin(void) const {return __Tmin;}
    //tiempo mínimo para ir de psta a pfin
    //con ac<amaxabs, vc<=vmaxabs y v(t) triangular
    double getTv(void) const {return __Tv;}
    //aceleración para ir de psta a pfin en el tiempo mínimo
    //con ac<amaxabs, vc<=vmaxabs y v(t) triangular
    double getav(void) const {return __av;}

    //TIEMPO DE DESPLAZAMIENTO ELEGIDO:

    //Distancia a recorrer
    //D = pfin - psta
    //cuando asigna D: pfin = psta + D
    void setD(double);
    //intervalo de tiempo de desplazamiento para ir de 'psta' a 'pfin' en ut
    //debe ser mayor que Tmin
    void setT(double);

    //DISTANCIAS RECORRIBLES EN EL TIEMPO 'T':

    //distancia que puede ser recorrida (en pasos)
    //con ac=amaxabs, vc=vmaxabs y v(t) triangular
    double getDv(void) const {return __Dv;}

    //LÍMITES DE LAS VARIABLES CUANDO ac=acmin y abs(vc)<=vmaxabs:

    //velocidad máxima del eje (en ul/ut)
    //es siempre menor o igual que vmax
    double getvcmax(void) const {return __vcmax;}
    //aceleración mínima del eje (en ul/ut^2)
    double getacmin(void) const {return __acmin;}
    //duración máxima de la rampa de aceleración (en ut)
    //es siempre menor o igual que T/2
    double getTrmax(void) const {return __Trmax;}
    //duración mínima del intervalo con velocidad constante (en ut)
    double getTcmin(void) const {return __Tcmin;}

    //LÍMITES DE LAS VARIABLES CUANDO ac=acmax y abs(vc)<=vmaxabs:

    //velocidad mínima del eje (en ul/ut)
    double getvcmin(void) const {return __vcmin;}
    //aceleración máxima del eje (en ul/ut^2)
    //siempre es igual a amax
    double getacmax(void) const {return __acmax;}
    //duración mínima de la rampa de aceleración (en ut)
    double getTrmin(void) const {return __Trmin;}
    //duración máxima del intervalo con velocidad constante (en ut)
    //es siemrpe menor que T
    double getTcmax(void) const {return __Tcmax;}

    //VARIABLES:

    //velocidad constante del eje en ul/ut
    void setvc(double);
    //pfiniente de la rampa de aceleración del eje (en ul/ut^2)
    //debe estar en [acmin, acmax] ó [acmin_, acmax_]
    double getac(void) const {return __ac;}
    void setac(double);
    //intervalo de tiempo de la rampa de aceleración (en ut)
    //debe estar en [Trmin, Trmax] ó [Trmin_, Trmax_]
    double getTr(void) const {return __Tr;}
    void setTr(double);
    //intervalo de tiempo durante el que se mantiene vc (en ut)
    //debe estar en [vcmin, vcmax] ó [vcmin_, vcmax_]
    double getTc(void) const {return __Tc;}
    void setTc(double);

    //PROPIEDADES EN FORMATO TEXTO:

    //imprime las porpiedades del objeto
    //concatenadas con su nombre separadas por puntos
    AnsiString getText(void);

    //MÉTODOS PÚBLICOS:

    //inicializa {amax=amaxabs=amaxabsdef, vmax=vmaxabs=vmaxabsdef,
    //psta=0, pfin=0} y asimila los parámetros
    TRampFunction(double _amaxabs=AMAXABSDEF, double _vmaxabs=VMAXABSDEF);
    //clona una función rampa
    TRampFunction(TRampFunction*);
    //copia todas las propiedades de una función rampa
    void Copy(TMotionFunction *_RampFunction);

    //asigna (psta, pfin) conjuntamente
    void SetInterval(double _psta, double _pfin);
    //invierte (psta, pfin) manteniendo el tiempo de desplazameinto T
    //y la forma de la función
    void InvertTime(void);

    //velocidad correspondiente al instante t (en ul/ut)
    double v(double t);
    //posición correspondiente al instante t (en ul)
    double p(double t);
};

//---------------------------------------------------------------------------

} //namespace MotionFunctions

//---------------------------------------------------------------------------
#endif // RAMPFUNCTION_H
