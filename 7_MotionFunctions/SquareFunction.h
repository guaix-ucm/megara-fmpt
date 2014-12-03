//---------------------------------------------------------------------------
//Archivo: SquareFunction.h
//Contenido: función de movimeinto cuadrada
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef SQUAREFUNCTION_H
#define SQUAREFUNCTION_H

#include "MotionFunction.h"

//---------------------------------------------------------------------------

//espacio de nombres de funciones de movimiento
namespace MotionFunctions {

//---------------------------------------------------------------------------
//TSquareFunction
//---------------------------------------------------------------------------

//clase función de movimineto cuadrada
class TSquareFunction : public TMotionFunction {
        //MÉTODOS DE ASIMILACIÓN DE PARÁMETROS:

        //calcula los hitos previos a la elección de 'T'
        //(vmax, Tmin)
        void CalculateMilestones(void);
        //calcula las distancias máximas que se pueden recorrer en el tiempo 'T'
        //(Dmax)
        void CalculateDistances(void);
        //calcula los límites del dominio de las variables:
        //[vcmin, vcmax] (en realidad solo vcmax, ya que vcmin es cte = 0)
        void CalculateBoundaries(void);

        //calcula las variables derivadas de 'vc'
        void CalculateVariables_vc(void);

public:
        //en la función cuadrada:
        //vcmin = 0;
        //cuando fabs(D)>0 => vcmax=vmax;
        //cuando D=0 => vcmax = 0;

        //PARÁMETROS IMPOSITIVOS:

        void setvmaxabs(double);
        void setpsta(double);
        void setpfin(double);

        //VARIABLES DE PRIMER ORDEN:

        void setD(double);
        void setT(double);

        //VARIABLES DE SEGUNDO ORDEN:

        void setvc(double);

        //PROPIEDADES EN FORMATO TEXTO:

        //MÉTODOS DE CONTRUCCIÓN Y COPIA:

        //inicializa {vmax=vmaxabs=vmaxabsdef, tini=0, tfin=0}
        //y asimila los parámetros
        TSquareFunction(double _vmaxabs=VMAXABSDEF);
        //clona una función cuadrada
        TSquareFunction(TSquareFunction*);
        //copia todas las propiedades una función cuadrada
        void Copy(TMotionFunction *_SquareFunction);

        //asigna (psta, pfin) conjuntamente
        void SetInterval(double _psta, double _pfin);
        //invierte (psta, pfin) manteniendo el tiempo de desplazameinto T
        //y la forma de la función
        void InvertTime(void);

        //velocidad correspondiente al instante t en ul/ut
        double v(double t);
        //posición correspondiente al instante t en ul
        double p(double t);
};

//---------------------------------------------------------------------------

} //namespace MotionFunctions

//---------------------------------------------------------------------------
#endif // SQUAREFUNCTION_H
