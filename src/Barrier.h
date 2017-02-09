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
//File: Barrier.h
//Content: barrier model of an EA or a RP-Actuator
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef BARRIER_H
#define BARRIER_H

//#include <stdint.h>

#include "ContourFigureList.h"
//#include "Function.h"
//#include "Quantificator.h"
//#include "Vector.h"
//#include "Vectors.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TBarrier:
//---------------------------------------------------------------------------

//Predefine la clase TArm para poder definir las funciones
//que calculan la distancia mínima y el estado de colisión.
class TArm;

class TBarrier {
    //PROPIEDADES DE DEFINICION:

    TContourFigureList p_Contour_;

    double p_r_max;

    TDoublePoint p_P0;
    double p_thetaO1;

    TContourFigureList p_Contour;

    double p_SPM;

    //-------------------------------------------------------------------
    //MÉTODOS DE ASIMILACIÓN:

    //a partir de:
    //    {Contour_}
    //determina:
    //    {r_max}
    void calculater_max(void);

    //a partir de:
    //    {Contour_}
    //    {P0, thetaO1}
    //determina:
    //    {Contour}
    void calculateImage(void);

public:
    //-------------------------------------------------------------------
    //PROPIEDADES DE DEFINICION:

    //contorno de la barrera en S1
    //valor por defecto: MEGARA_Contour_Text (asignado mediante setContour_Text)
    const TContourFigureList &getContour_(void) const {
        return (const TContourFigureList&)p_Contour_;}
    void setContour_(const TContourFigureList &_Contour_);

    //distancia entre el origen de coordenadas de S1
    //y el punto más alejado de Contour_
    double getr_max(void) const {return p_r_max;}

    //punto origen de S1 en S0 y de referencia de la barrera
    //valor por defecto: {0, 0} {mm, mm}
    TDoublePoint getP0(void) const {return p_P0;}
    void setP0(TDoublePoint);
    //orientación de S1 en S0
    //valor por defecto: 0 rad
    double getthetaO1(void) const {return p_thetaO1;}
    void setthetaO1(double);

    //contorno de la barrera en S0
    //valor por defecto: MEGARA_Contour_Text (asignado mediante setContour_Text)
    const TContourFigureList &getContour(void) const {
        return (const TContourFigureList&)p_Contour;}
    //margen perimetral de seguridad de la barrera
    //puede ser igual a cero
    //valor por defecto: MEGARA_Eo*r_max + MEGARA_Ep mm
    double getSPM(void) const {return p_SPM;}
    void setSPM(double);

    //ADVERTENCIA: SPM es un valor que debe ser actualizado a paritr de
    //los márgenes de tolerancia (Eo, Ep) del EA o el RP-Actuator,
    //de modo que el valor por defecto es:
    //  0.12154396266401415     para el EA
    //  0.11217804007500001     para el RP-Actuator

    //-------------------------------------------------------------------
    //PROPIEDADES EN FORMATO TEXTO:

    AnsiString getContour_AddressText(void) const {
        return IntToHex(intptr_t(&p_Contour_));}
    void setContour_Text(const AnsiString&);
    void setContour_ColumnText(const AnsiString&);

    AnsiString getr_maxText(void) const;

    AnsiString getP0Text(void) const;
    void setP0Text(const AnsiString&);
    AnsiString getthetaO1Text(void) const;
    void setthetaO1Text(const AnsiString&);

    AnsiString getContourAddressText(void) const {
        return IntToHex(intptr_t(&p_Contour));}
    AnsiString getSPMText(void) const;
    void setSPMText(const AnsiString&);

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    //conjunto de todas las propiedades
    //en formato de asignaciones
    AnsiString getAllText(void) const;
    //conjunto de todas las propiedades de instancia
    //en formato de asignaciones
    AnsiString getInstanceText(void) const;
    void setInstanceText(const AnsiString&);

    //-------------------------------------------------------------------
    //MÉTODOS ESTÁTICOS:

    //lee una instancia de barrera en una cadena
    static void  readInstance(TBarrier *B,
                              const AnsiString& S, int& i);

    //-------------------------------------------------------------------
    //MÉTODOS PUBLICOS:

    //contruye una barrera
    TBarrier(TDoublePoint P0, double thetaO1=0);
    //copia una barrera
    void copy(const TBarrier*);
    //contruye un clon de una barrera
    TBarrier(const TBarrier*);
    //libera la memoria dinámica
    ~TBarrier();

    //determina si tiene todos los valores por defecto
    //de una barrera de un EA
    bool dontHasAllDefaultValuesEA(void) const;
    //determina si tiene todos los valores por defecto
    //de una barrera de un actuador de un RP
    bool dontHasAllDefaultValuesActuator(void) const;

    //determina si una barrera es distinta
    bool operator!=(const TBarrier&) const;

    //cambia la posición y orientación
    //del origen de coordenadas simultaneamente
    void set(TDoublePoint P0, double thetaO1);

    //-------------------------------------------------------------------
    //MÉTODOS PARA DETERMINAR SI HAY COLISIÓN:

    //determina la distancia mínima con una barrera
    double distanceMin(const TBarrier*) const;
    //determina la distancia mínima con un brazo
    double distanceMin(const TArm*) const;

    //Si los contornos están tan alejado que no puede haber colisión,
    //la distancia mínima devuelta será igual a DBL_MAX.

    //determina si hay colisión con una barrera
    bool collides(const TBarrier*) const;
    //determina si hay colisión con un brazo
    bool collides(const TArm*) const;

    //determina si un punto está dentro de la barrera
    bool covers(TDoublePoint) const;
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // BARRIER_H
