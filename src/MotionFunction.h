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
//Archivo: MotionFunction.h
//Contenido: función de movimiento
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef MOTIONFUNCTION_H
#define MOTIONFUNCTION_H

#include "vclemu.h"
#include "FiberMOSModelConstants.h" //VMAXABS, AMAXABS

//---------------------------------------------------------------------------

//espacio de nombres de funciones de movimiento
namespace MotionFunctions {

//---------------------------------------------------------------------------
//TMotionFunction
//---------------------------------------------------------------------------

//ABREVIATURAS (en castellano):
//      ul: udidades de longitud;
//      ut: unidades de tiempo.

//clase función de movimiento
class TMotionFunction {
protected:
        //PARÁMETROS IMPOSITIVOS:

        double __vmaxabs;

        double __psta;
        double __pfin;

        double __vmax;

        //HITOS PREVIOS A LA ELECCIÓN DE VARIABLES DE 'T':

        double __Tmin;

        //VARIABLES DE PRIMER ORDEN:

        double __D;
        double __T;

        //HITOS DEPENDIENTES DE LAS VARIABLES DE PRIMER ORDEN:

        double __Dmax;

        //LÍMITES DEL DOMINIO DE LAS VARIABLES DE SEGUNDO ORDEN:

        double __vcmin;
        double __vcmax;

        //VARIABLES DE SEGUNDO ORDEN:

        double __vc;

        //PROPIEDADES EN FORMATO TEXTO:

        AnsiString __Label;

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

public:
        //PARÁMETROS IMPOSITIVOS:

        //velocidad máxima absoluta en ul/ut
        //debe ser mayor que cero
        double getvmaxabs(void) const {return __vmaxabs;}
        virtual void setvmaxabs(double) = 0;

        //posición de salida (starting) en ul
        double getpsta(void) const {return __psta;}
        virtual void setpsta(double) = 0;
        //posición de llegada (finishing) en ul
        double getpfin(void) const {return __pfin;}
        virtual void setpfin(double) = 0;

        //velocidad máxima algebraica en ul/ut
        //vmax = vmaxabs*sign(pfin-psta)
        double getvmax(void) const {return __vmax;}

        //HITOS PREVIOS A LA ELECCIÓN DE 'T':

        //tiempo mínimo para ir de 'psta' a 'pfin' en ut
        //aplicando 'amax' y abs(vc)<=vmaxabs
        double getTmin(void) const {return __Tmin;}

        //VARIABLES DE PRIMER ORDEN:

        //Distancia a recorrer
        //D = pfin - psta
        //cuando asigna D: pfin = psta + D
        double getD(void) const {return __D;}
        virtual void setD(double) = 0;
        //intervalo de tiempo de desplazamiento para ir de 'psta' a 'pfin' en ut
        //debe ser mayor que Tmin
        double getT(void) const {return __T;}
        virtual void setT(double) = 0;

        //HITOS DEPENDIENTES DE LAS VARIABLES DE PRIMER ORDEN:

        //distancia máxima en ul
        //que puede ser recorrida aplicando la máxima aceleración y velocidad
        double getDmax(void) const {return __Dmax;}

        //LÍMITES DE LOS DOMINIOS DE LAS VARIABLES DE SEGUNDO ORDEN:

        //límites algebraicos de 'vc'
        double getvcmin(void) const {return __vcmin;}
        double getvcmax(void) const {return __vcmax;}

        //VARIABLES DE SEGUNDO ORDEN:

        //velocidad constante del eje en ul/ut
        double getvc(void) const {return __vc;}
        virtual void setvc(double) = 0;

        //PROPIEDADES EN FORMATO TEXTO:

        //etiqueta de identificación del objeto
        //debe ser un nombre de variable válido
        //valor por defecto: ""
        AnsiString getLabel(void) const {return __Label;}
        void setLabel(const AnsiString&);

        AnsiString getvmaxabsText(void) const;
        void setvmaxabsText(const AnsiString&);
        AnsiString getpstaText(void) const;
        void setpstaText(const AnsiString&);
        AnsiString getpfinText(void) const;
        void setpfinText(const AnsiString&);

        AnsiString getvmaxText(void) const;
        AnsiString getTminText(void) const;

        AnsiString getDText(void) const;
        void setDText(const AnsiString&);
        AnsiString getTText(void) const;
        void setTText(const AnsiString&);

        AnsiString getDmaxText(void) const;
        AnsiString getvcminText(void) const;
        AnsiString getvcmaxText(void) const;

        AnsiString getvcText(void) const;
        void setvcText(const AnsiString&);

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //imprime las porpiedades del objeto
        //concatenadas con su nombre separadas por puntos
        AnsiString getText(void);

        //-------------------------------------------------------------------
        //MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

        //inicializa las propiedades a sus valores por defecto
        TMotionFunction(void);

        //copia todas las propiedades de una función de movimiento
        virtual void Copy(TMotionFunction*) = 0;

        //construye un clon de una función de movimiento
        TMotionFunction(TMotionFunction*);

        //destruye una función de movimiento
        virtual ~TMotionFunction() = 0;

        //La existencia de un destructor virtual es condición indispensable
        //para evitar la ambiguedad en la destrucción de objetos
        //polimórficos.
        //El cuerpo del destructor virtual debe ser definido en el archivo
        //fuente (.cpp), aunque no haga nada.

        //MÉTODOS DE CONFIGURACIÓN:

        //asigna (psta, pfin) conjuntamente
        virtual void SetInterval(double _psta, double _pfin) = 0;
        //invierte (psta, pfin) manteniendo el tiempo de desplazameinto T
        //y la forma de la función
        virtual void InvertTime(void) = 0;

        //MÉTODOS DE SIMULACIÓN:

        //velocidad correspondiente al instante t en pasos/s
        virtual double v(double t) = 0;
        //posición correspondiente al instante t en pasos
        virtual double p(double t) = 0;
};

//---------------------------------------------------------------------------

} //namespace MotionFunctions

//---------------------------------------------------------------------------
#endif // MOTIONFUNCTION_H
