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
/// @file adjacentitem.h
/// @brief class of adjacent items EAs and RPs
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ADJACENTITEM_H
#define ADJACENTITEM_H

#include "PointersList.h"

//---------------------------------------------------------------------------

using namespace Lists;

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TAdjacentEA
//---------------------------------------------------------------------------

//predeclares classes to avoid loops
class TExclusionArea;

//class cluster (EA, Dmin, Dend)
class TAdjacentEA {
public:
    //attached-extern EA
    //default value: NULL
    TExclusionArea *EA;
    //minimun distance during motion
    //default value: DBL_MAX
    double Dmin;
    //distance in the final posicion
    //default value: DBL_MAX
    double Dend;

    //build an item with the indicated values
    TAdjacentEA(TExclusionArea *t_EA=NULL,
                double t_Dmin=DBL_MAX, double t_Dend=DBL_MAX);
    //clone an item
    void Clone(TAdjacentEA&);
    //build a clon of an item
    TAdjacentEA(TAdjacentEA*&);

    //compare the Id of the EA of two adjacent EAs
    static int compareIds(const TAdjacentEA *AEA1, const TAdjacentEA *AEA2);
    //print the Id of the EA of an adjacent EA
    static void printId(AnsiString &S, const TAdjacentEA *AEA);
};

//---------------------------------------------------------------------------
//TAdjacentEAList
//---------------------------------------------------------------------------

//cluss list of adjacent EAs
class TAdjacentEAList : public TPointersList<TAdjacentEA> {
public:
    //Default values:
    //  Count = 0;
    //  Capacity = 1;
    //  Compare = TAdjacentEA::CompareIds;
    //  Print = TAdjacentEA::PrintId;

    //build a list by default
    TAdjacentEAList(void);
    //build a clon of a list
    TAdjacentEAList(const TAdjacentEAList&);

    //get partial lists in text format
    string getIdText(void) const;
    string getDminText(void) const;
    string getDendText(void) const;

    //set a same value to all Dmins
    void setAllDmins(double Dmin);
    //set a same value to all Dends
    void setAllDends(double Dend);
};

//---------------------------------------------------------------------------
//TAdjacentRP
//---------------------------------------------------------------------------

//predeclares classes to avoid loops
class TRoboticPositioner;

//class cluster (RP, Dmin, Dend)
class TAdjacentRP {
public:
    //attached-extern RP
    //default value: NULL
    TRoboticPositioner *RP;
    //minimun distance during motion
    //default value: DBL_MAX
    double Dmin;
    //distance in the final posicion
    //default value: DBL_MAX
    double Dend;

    //build an item with the indicated values
    TAdjacentRP(TRoboticPositioner *t_RP=NULL,
                double t_Dmin=DBL_MAX, double t_Dend=DBL_MAX);
    //clone an item
    void Clone(TAdjacentRP&);
    //build a clon of an item
    TAdjacentRP(TAdjacentRP*&);

    //compare the Id of the RP of two adjacent RPs
    static int compareIds(const TAdjacentRP *ARP1, const TAdjacentRP *ARP2);
    //print the Id of the RP of an adjacent RP
    static void printId(AnsiString &S, const TAdjacentRP *ARP);
};

//---------------------------------------------------------------------------
//TAdjacentRPList
//---------------------------------------------------------------------------

//cluss list of adjacent RPs
class TAdjacentRPList : public TPointersList<TAdjacentRP> {
public:
    //Default values:
    //  Count = 0;
    //  Capacity = 6;
    //  Compare = TAdjacentRP::CompareIds;
    //  Print = TAdjacentRP::PrintId;

    //build a list by default
    TAdjacentRPList(void);
    //build a clon of a list
    TAdjacentRPList(const TAdjacentRPList&);

    //get partial lists in text format
    string getIdText(void) const;
    string getDminText(void) const;
    string getDendText(void) const;

    //set a same value to all Dmins
    void setAllDmins(double Dmin);
    //set a same value to all Dends
    void setAllDends(double Dend);
};


//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // ADJACENTITEM_H
