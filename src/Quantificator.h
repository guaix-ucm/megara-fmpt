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
//File: Quantificator.h
//Content: quantifier operator
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef QUANTIFICATOR_H
#define QUANTIFICATOR_H

#include "Vector.h"

//---------------------------------------------------------------------------

using namespace Lists;

//espacio de nombres de operadores matemáticos
namespace Operators {

//---------------------------------------------------------------------------
//clase TQuantificator
//---------------------------------------------------------------------------

//La clase TQuantificator representa cuantificadores centrados en cero,
//que pueden verse como funciones de transformación de coordenadas,
//donde las coordenadas del dominio imagen vienen dadas en pasos.

//clase cuantificador
class TQuantificator {
protected:
        //PARÁMETROS DE CONFIGURACIÓN:

        double p_q;
        double p_xmin;
        double p_xmax;

        //PARÁMETROS DE CONFIGURACIÓN DERIVADOS:

        int p_Qmin;
        int p_Qmax;

        //PROPIEDADES EN FORMATO TEXTO:

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //MÉTODOS DE ASIMILACIÓN:

        //calcula los límties del dominio imagen de la función:
        //      (Qmin, Qmax),
        //a partir de los parámetros de cuantificación:
        //      (q, xmin, xmax)
        void CalculateImageDomain(void);

public:
        //PARÁMETROS DE CONFIGURACIÓN:

        //escalón de cuantificación
        //q debe ser distinto de cero
        //q puede ser negativo
        //valor por defecto: 1
        double getq(void) const {return p_q;} void setq(double);
        //límite inferior del dominio de definición de la función
        //no debe ser mayor que xmax
        //valor por defecto: -1
        double getxmin(void) const {return p_xmin;} void setxmin(double);
        //límite superior del dominio de definición de la función
        //no debe ser menor que xmin
        //valor por defecto: 1
        double getxmax(void) const {return p_xmax;} void setxmax(double);

        //PARÁMETROS DE CONFIGURACIÓN DERIVADOS:

        //límite inferior del dominio imagen de la función
        int getQmin(void) const {return p_Qmin;}
        //límite superior del dominio imagen de la función
        int getQmax(void) const {return p_Qmax;}

        //PROPIEDADES EN FORMATO TEXTO:

        AnsiString getqText(void) const {return FloatToStr(p_q);}
        void setqText(AnsiString&);
        AnsiString getxminText(void) const {return FloatToStr(p_xmin);}
        void setxminText(AnsiString&);
        AnsiString getxmaxText(void) const {return FloatToStr(p_xmax);}
        void setxmaxText(AnsiString&);

        AnsiString getQminText(void) const {return FloatToStr(p_Qmin);}
        AnsiString getQmaxText(void) const {return FloatToStr(p_Qmax);}

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjunto de propiedades de configuración en formato texto
        //      (q, [xmin, xmax])
        AnsiString getRWText(void); void setRWText(const AnsiString&);
        //conjunto de propiedades de configuración derivadas en formato texto
        //      [Qmin, Qmax]
        AnsiString getRText(void);

        //conjunto de todas las propiedades en formato texto
        //en forma de asignaciones
        AnsiString getAssignsText(void) const;
        void setAssignsText(const AnsiString&);

        //FUNCIONES ESTÁTICAS DE IMPRESIÓN Y LECTURA:

        //imprime el conjunto de propiedades RW de un cuantificador
        //en una cadena de caracteres
        static void  PrintRW(AnsiString &S, const TQuantificator *Q);
        //intenta leer el conjunto de propiedades RW un cuantificador
        //a partir de la posición indicada en una cadena de caracteres
        static void  ReadRW(TQuantificator *Q, const AnsiString &S,
                int &i);

        //-------------------------------------------------------------------
        //MÉTODOS PÚBLICOS:

        //imprime un cluster de configuración al final de una cadena de texto
        static void  PrintAssigns(AnsiString &S,
                const TQuantificator *Q);
        //lee un cluster de configuración en una cadena de texto
        static void  ReadAssigns(TQuantificator *Q,
                const AnsiString &S, int &i);

        //contruye un cuantificador
        TQuantificator(double q=1, double xmin=-1, double xmax=1);
        //copia todas las propiedades de un cuantificador
        void Copy(const TQuantificator*);
        void Copy(const TQuantificator&);
        TQuantificator& operator=(const TQuantificator& Q);
        //construye un clon del cuantificador
        TQuantificator(const TQuantificator*);

        //compara un cuantificador
        bool operator==(const TQuantificator& Q) const;
        bool operator!=(const TQuantificator& Q) const;

        //configura el cuantificador
        void Set(double q,  double xmin, double xmax);

        //cuantifica un valor
        int Quantifice(double x) const;
        int operator[](double x) const;
        //reconstruye un valor
        double Reconstruct(int n) const;
        double operator[](int n) const;
};

//---------------------------------------------------------------------------

} //namespace Operators

//---------------------------------------------------------------------------
#endif // QUANTIFICATOR_H
