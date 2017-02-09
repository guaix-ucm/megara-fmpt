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
//File: FigureList.h
//Content: contour figure list
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef CONTOURFIGURELIST_H
#define CONTOURFIGURELIST_H

#include "Figure.h"
#include "PointersList.h"
#include "Vectors.h"

//---------------------------------------------------------------------------

using namespace Mathematics;
using namespace Lists;

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TContourFigureList
//---------------------------------------------------------------------------

//ADVERTENCIA: TContourFigureList no puede derivar de la clase
//TPointersList<TContourFigure>, por que la clase TContourFigure
//es abstracta, y los objetos de la clase TContourFigure no pueden
//ser construidos por defecto.

//clase lista de figuras de contorno
class TContourFigureList : public TItemsList<TContourFigure*> {
public:
        //-------------------------------------------------------------------
        //MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACIÓN:

        //construye una lista de figuras
        TContourFigureList(int Capacity=8);

        //ADVERTENCIA:
        //al derivar TContourFigureList de TItemsList<TContourFigure*>
        //los objetos a los que apuntan sus punteros deberán ser destruidos
        //manualmente antes de destruir el último puntero que los apunte.

        //copia una lista de figuras
        void Copy(const TContourFigureList&);
        TContourFigureList& operator=(const TContourFigureList&);

        //WARNING: Copy must be written wit upcase
        //for mask the inherited method Copy.

        //construye un clon de una lista de figuras
        TContourFigureList(const TContourFigureList &Contour);

        //determina si una CFL es distinta
        bool operator!=(const TContourFigureList&) const;

        //MÉTODOS DE CARACTERIZACIÓN:

        //determina si todos los punteros de la lista son nulos
        bool areAllNULL(void) const;

        //determina si la lista de figuras constituye un contorno ordenado
        //comprobando que:
        //- La lista contiene al menos dos figuras.
        //- Cada figura se encuentra engarzada con la siguiente.
        //- Ningún vértice siguiente coincide con el vértice inicial
        //  de otra figura previa, excepto el de la última figura,
        //  que coincide con el inicial de la primera figura.
        //- Ninguna figura se interseca con las demás.
        bool isAContourSorted(void) const;

        //ADVERTENCIA: por ahora IsAContourSorted está comprobando solamente
        //que:
        //- La lista contiene al menos dos figuras.
        //- Cada figura se encuentra engarzada con la siguiente.

        //MÉTODOS DE CÁLCULO DE DISTANCIAS CON EL CONTORNO:

        //determina la distancia mínima de un punto a este contorno
        double distanceMin(TDoublePoint P) const;
        //determina la distancia máxima de un punto a este contorno
        double distanceMax(TDoublePoint P) const;
        //determina la distancia mínima de un contorno a este contorno
        double distanceMin(const TContourFigureList&) const;

        //MÉTODOS DE DETERMINACIÓN DE INTERSECCIÓN:

        //determina si la distancia de una lista de figuras
        //a esta lista de figuras es inferior
        //al margen perimetral de seguridad SPM.
        bool collides(const TContourFigureList &C, double SPM) const;

        //Se advierte que la función que determina el estado de colisión
        //puede tener un tiempo de procesado medio ligeramente inferior
        //a la de determinación de la distancia, por que la colisión se
        //detecta a mayor distancia que la distancia igual a cero.

        //determina si un punto está en el interior del contorno
        bool isInner(TDoublePoint P) const;

        //MÉTODOS DE TRANSFORMACIONES GEOMÉTRICAS:

        //obtiene la lista de figuras rotada y trasladada.
        //si el número de figuras de la lista no coincide:
        //      lanza EImproperArgument
        //si alguna figuradelalista no es del mimo tipo
        //      lanza EImproperArgument
        void getRotatedAndTranslated(TContourFigureList &Contour,
                double theta, TDoublePoint V) const;
};

//--------------------------------------------------------------------------
//FUNCIONES RELACIONADAS:

//segrega las figuras de una lista en listas de figuras concatenadas
//devuelve falso si encuentra más de dos vértices engarzados entre si
//bool segregate(TPointersList<TContourFigureList> &LC, TContourFigureList &C);
//reduce un conjunto de listas de figuras concatenadas
//concatenando las listas que se puedan entre si
//void concatenate(TPointersList<TContourFigureList> &LC);

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // CONTOURFIGURELIST_H
