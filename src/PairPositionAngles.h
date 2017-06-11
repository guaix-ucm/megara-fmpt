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
/// @file PairPositionAngles.h
/// @brief pair of projection angles
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef PAIRPOSITIONANGLES_H
#define PAIRPOSITIONANGLES_H

#include "ProjectionPoint.h"
#include "Figure.h"
#include "PointersList.h"
#include "Vector.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TPairPositionAngles
//---------------------------------------------------------------------------

//predeclares classes to avoid loops
class TRoboticPositioner;

//class PPA (Pair Position Angles)
class TPairPositionAngles {
protected:
    //ATTACHED EXTERN OBJECTS:

    int Id;

public:
    //SATIC PROPERTIES:

    //label of property Id
    //default value: "Id"
    static AnsiString IdLabel;
    //label of property p_1
    //default value: "p_1"
    static AnsiString p_1Label;
    //label of property p___3
    //default value: "p___3"
    static AnsiString p___3Label;

    //PROPERTIES:

    //single identification number of the attached RP
    //shall be nonnegative
    //default value: 0
    int getId(void) const {return Id;} void setId(const int);

    //position angle for rotor 1
    double p_1;
    //position angle for rotor 2
    double p___3;

    //PROPERTIES IN TEXT FORMAT:

    AnsiString getIdText(void) const;
    AnsiString getp_1Text(void) const; void setp_1Text(const AnsiString&);
    AnsiString getp___3Text(void) const; void setp___3Text(const AnsiString&);

    //SETS OF PROPERTIES IN TEXT FORMAT:

    //values of all properties in row text format
    AnsiString getRowText(void) const; void setRowText(const AnsiString&);

    //STATIC METHODS:

    //labels of all properties in a row:
    //  PRidLabel+"\t"+p_1Label+"\t"+p___3Label
    static AnsiString getLabelsRow(void);
    //travel the labels of the properties
    //in row text format
    static void travelLabelsRow(const AnsiString& S, int& i);

    //print the properties of a PPA in a string
    //in row format
    static void  printRow(AnsiString &S,
                          const TPairPositionAngles *PPA);
    //read the properties of a PPA in a string
    //in row format
    static void  readRow(TPairPositionAngles *PPA,
                         const AnsiString &S, int &i);

    //PUBLIC METHODS:

    //build a PPA
    TPairPositionAngles(const int Id = 0);

    //copy all properties of a PPA
    void clone(const TPairPositionAngles*);
    TPairPositionAngles &operator=(const TPairPositionAngles&);

    //build a clon a PPA
    TPairPositionAngles(const TPairPositionAngles*);

    //set all properties
    void set(const int Id, const double p_1, const double p___3);
    //set (p_1, p___3)
    void set(const double p_1, const double p___3);

    //randomize (p_1, p___3)
    void randomize(const double p_1min, const double p_1max,
                   const double p___3min, const double p___3max);
};

//---------------------------------------------------------------------------
//TPairPositionAnglesList
//---------------------------------------------------------------------------

//class list of PPAs
class TPairPositionAnglesList : public TPointersList<TPairPositionAngles> {
public:
    //PROPERTIES IN TEXT FORMAT:

    //PPA list in text format
    AnsiString getText(void) const; void setText(const AnsiString &);

    //WARNING: setText will build new PPAs without attached PP.

    //PUBLIC METHODS:

    //builds a PPA list
    TPairPositionAnglesList(void);

    //copy a PPA list
    TPairPositionAnglesList &operator=(const TPairPositionAnglesList&);

    //search the first PPA with a given identifier
    int searchId(const int Id) const;

    //randomize the PPAs of the list
    void randomize(const double p_1min, const double p_1max,
                   const double p___3min, const double p___3max);

    //check if all PPAs are referred to different RPs
    bool notAllAreReferredToDifferentRPs(void) const;

    //check if all Ids of a list are in the PPA list
    bool notAllIdsAreFound(const TVector<int>& Ids) const;
};

//compare two PPA list
bool operator!=(const TPairPositionAnglesList&,
                const TPairPositionAnglesList&);

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // PAIRPOSITIONANGLES_H
