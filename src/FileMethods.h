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
//File: FileMethods.h
//Content: file functions for read and write instances of models
//Last update: 12/02/2015
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FILEMETHODS_H
#define FILEMETHODS_H

#include "FiberMOSModel2.h"
#include "FiberConnectionModel.h"

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR ROBOTIC POSITIONERS:

//read an instance of a RP
//from a directory
void readInstanceFromDir(TRoboticPositioner&, const string& dir);

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR ROBOTIC POSITIONER LISTS:

//write the instance of a RP list
//in a directory
void writeInstanceToDir(const string& dir, const TRoboticPositionerList&);
//read an instance of a RP list
//from a directory
void readInstanceFromDir(TRoboticPositionerList&, const string& dir);

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR EXCLUSION AREA LIST:

//write the instance of a EA list
//in a directory
void writeInstanceToDir(const string& dir, const TExclusionAreaList&);
//read an instance of a EA list
//from a directory
void readInstanceFromDir(TExclusionAreaList&, const string& dir,
        const TRoboticPositionerList&);

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR Fiber MOS Models:

/*//write the instance of a Fiber MOS Model
//in a directory
void writeInstanceToDir(const string& Dir, const TFiberMOSModel*);
//read an instance of a Fiber MOS Model
//from a directory
void readInstanceFromDir(TFiberMOSModel*, const string& dir);
*/
//write the instance of a Fiber MOS Model
//in a directory
void writeInstanceToDir(const string& dir, const TFiberMOSModel&);
//read an instance of a Fiber MOS Model
//from a directory
void readInstanceFromDir(TFiberMOSModel&, const string& dir);

//---------------------------------------------------------------------------
//FILE FUNCTIONS FOR Fiber Connection Models:

/*//write the instance of a Fiber Connection Model
//in a directory
void writeInstanceToDir(const string& dir, const TFiberConnectionModel*);
//read an instance of a Fiber Connection Model
//from a directory
void readInstanceFromDir(TFiberConnectionModel*, const string& dir);
*/
//write the instance of a Fiber Connection Model
//in a directory
void writeInstanceToDir(const string& dir, const TFiberConnectionModel&);
//read an instance of a Fiber Connection Model
//from a directory
void readInstanceFromDir(TFiberConnectionModel&, const string& dir);

} //namespace Models

//---------------------------------------------------------------------------
#endif // FILEMETHODS_H
