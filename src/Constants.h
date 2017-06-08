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
///@file Constants.h
///@brief general constants of the program
///@author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef CONSTANTS_H
#define CONSTANTS_H

//--------------------------------------------------------------------------
//GENERAL CONSTANTS
//--------------------------------------------------------------------------

//The value of M_PI readed by the debugger is 3.14159265358979
//define a textual value for 2PI to avoid discrepancy
//of values loaded from file and 2*M_PI
#define M_2PI 6.28318530717959

//number of items of an array
#define NITEMS(arr) (sizeof(arr) / sizeof(arr[0]))

//The macro NITEMS can't be relaceb by a function, because for the function
//sizeof, the pointer to an array and the array aren't the same,
//so the following frunction alwais return 1:
//      template <class T> int numberItems(T *arr) {
//              return sizeof(arr)/sizeof(arr[0]);}

//upper bound for the final numercal error
#define ERR_NUM 0.0000001

//---------------------------------------------------------------------------

#endif // CONSTANTS_H
