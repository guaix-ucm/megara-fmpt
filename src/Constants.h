// Copyright (c) 2012-2016 Isaac Morales Durán. All rights reserved.
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
//File: Constants.h
//Content: general constants of the program
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef CONSTANTS_H
#define CONSTANTS_H

//---------------------------------------------------------------------------

//El valor de M_PI leido con el depurador es 3.14159265358979
//define un valor textual para 2PI con objeto de evitar
//la discrepancia de los valores cargados de archivo y 2*M_PI
#define M_2PI 6.28318530717959

//número de elementos de un array
#define NELEMS(arr) (sizeof(arr) / sizeof(arr[0]))

//La macro NELEMS no puede ser sustituida por una función, porque para
//la función sizeof, el puntero a un array no es o mismo que el array,
//de modo que la siguiente función simepre devolverá 1:
//      template <class T> int NumberElements(T *arr) {
//              return sizeof(arr)/sizeof(arr[0]);}

//cota superior para el error numérico final
#define ERR_NUM 0.00000001

//---------------------------------------------------------------------------

#endif // CONSTANTS_H
