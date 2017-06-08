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
///@file PositionersCenters.h
///@brief positioner center list
///@author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef POSITIONERCENTER_H
#define POSITIONERCENTER_H

//#include "MotionProgramValidator.h"
//#include "TargetPointList.h"
//#include "FiberMOSModel.h"
//#include "MotionProgram.h"
#include "PointersList.h"

//---------------------------------------------------------------------------

using namespace Lists;

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//TPositionerCenter:
//---------------------------------------------------------------------------

//class centro de posicionador
class TPositionerCenter {
public:
    //SP properties:
    int Id;
    double x0;
    double y0;

    //set the structure in text format
    void setText(const string& S);

    //print the properties in a row text
    static void PrintRow(AnsiString& S, const TPositionerCenter *PC);

    //build a structure by default
    TPositionerCenter(void);

    //copy all properties of am object of the same type
    TPositionerCenter& operator=(const TPositionerCenter&);
};

//---------------------------------------------------------------------------
//class TPositionerCenterList:

class TPositionerCenterList : public TPointersList<TPositionerCenter> {
public:
    //set the PCL in text format
    void setTableText(const string& S);

    //build a PositionerCenterList by default
    TPositionerCenterList(void);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

#endif // POSITIONERCENTER_H
