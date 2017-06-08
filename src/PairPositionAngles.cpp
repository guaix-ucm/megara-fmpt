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
///@file PairPositionAngles.cpp
///@brief pair of projection angles
///@author Isaac Morales Durán
//---------------------------------------------------------------------------

#include "PairPositionAngles.h"
#include "TextFile.h"
#include "Strings.h"
#include "Geometry.h"
#include "Scalars.h"
#include "RoboticPositioner.h"

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//--------------------------------------------------------------------------
//TPairPositionAngles
//--------------------------------------------------------------------------

//SATIC PROPERTIES:

//label of property Id
//default value: "Id"
AnsiString TPairPositionAngles::IdLabel = "Id";
//label of property p_1
//default value: "p_1"
AnsiString TPairPositionAngles::p_1Label = "p_1";
//label of property p___3
//default value: "p___3"
AnsiString TPairPositionAngles::p___3Label = "p___3";

//PUBLIC PROPERTIES:

//single identification number of the attached RP
void TPairPositionAngles::setId(const int Id_)
{
    if(Id_ < 0)
        throw EImproperArgument("RP identifier (Id) should be nonnegative");

    Id = Id_;
}

//PROPERTIES IN TEXT FORMAT:

AnsiString TPairPositionAngles::getIdText(void) const
{
    return IntToStr(getId());
}
AnsiString TPairPositionAngles::getp_1Text(void) const
{
    return FloatToStr(p_1);
}
void TPairPositionAngles::setp_1Text(const AnsiString &S)
{
    try {
        p_1 = StrToFloat(S);
    } catch(Exception& E) {
        E.Message.Insert(1, "setting p_1 in text format: ");
        throw;
    }
}
AnsiString TPairPositionAngles::getp___3Text(void) const
{
    return FloatToStr(p___3);
}
void TPairPositionAngles::setp___3Text(const AnsiString &S)
{
    try {
        p___3 = StrToFloat(S);
    } catch(Exception& E) {
        E.Message.Insert(1, "setting p___3 in text format: ");
        throw;
    }
}

//SETS OF PROPERTIES IN TEXT FORMAT:

AnsiString TPairPositionAngles::getRowText(void) const
{
    AnsiString S;

    S = getIdText()+AnsiString("\t")+getp_1Text()+AnsiString("\t")+getp___3Text();

    return S;
}
void TPairPositionAngles::setRowText(const AnsiString &S)
{
    //initialize the index to the first char of the string
    int i = 1;

    try {
        //read the PPA in row text format to a tanpon variable
        TPairPositionAngles PPA;
        readRow(&PPA, S, i);

        //search unexpected text
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text setting pair position angles in text format");

        clone(&PPA); //clone the tampon object

    } catch(Exception& E) {
        unsigned int row, col;
        strPositionToCoordinates(row, col, S.str, i-1);
        E.Message.Insert(1, "setting PPA in row text format in row "+inttostr(row)+" and column "+inttostr(col)+": ");
        throw;
    }
}

//STATIC METHODS:

//labels of all properties in a row:
//  IdLabel+"\t"+p_1Label+"\t"+p___3Label
AnsiString TPairPositionAngles::getLabelsRow(void)
{
    return IdLabel+AnsiString("\t")+p_1Label+AnsiString("\t")+p___3Label;
}
//travel the labels of the properties
//in row text format
void TPairPositionAngles::travelLabelsRow(const AnsiString& S, int& i)
{
    //check the precondition
    if(i<0 || S.Length()<i)
        throw EImproperArgument("index i shall indicates a character of the string S");

    try {
        StrTravelSeparatorsIfAny(S,i);
        StrTravelLabel(IdLabel, S, i);
        StrTravelSeparators(S,i);
        StrTravelLabel(p_1Label, S, i);
        StrTravelSeparators(S,i);
        StrTravelLabel(p___3Label, S, i);

    } catch(Exception& E) {
        E.Message.Insert(1, "traveling labels in row format: ");
        throw;
    }
}

//print the properties of a PPA in a string
//in row format
void  TPairPositionAngles::printRow(AnsiString &S,
                                    const TPairPositionAngles *PPA)
{
    //check the precondition
    if(PPA == NULL)
        throw EImproperArgument("pointer PPA should point to built pair of position angles");

    //add the text to the string in propeerly format
    S += PPA->getRowText();
}

//read the properties of a PPA in a string
//in row format
void  TPairPositionAngles::readRow(TPairPositionAngles *PPA,
                                   const AnsiString &S, int &i)
{
    //check the preconditions
    if(PPA == NULL)
        throw EImproperArgument("pointer PPA shoult point to built pair of position angles");
    if(i<1 || S.Length()<i)
        throw EImproperArgument("index i should indicate to char of string S");

    try {
        //tanpon variables
        int Id;
        double p_1, p___3;

        //ADVERTENCIA: tampon variables with interdependent properties
        //shall be clones of the variables to be modified.

        StrTravelSeparatorsIfAny(S, i);
        StrReadInt(Id, S, i);
        StrTravelSeparators(S, i);
        StrReadFloat(p_1, S, i);
        StrTravelSeparators(S, i);
        StrReadFloat(p___3, S, i);

        //set the tanpon variables
        PPA->set(Id, p_1, p___3);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading a PPA in text format: ");
        throw;
    }
}

//PUBLIC METHODS:

//build a PPA
TPairPositionAngles::TPairPositionAngles(const int Id) :
    p_1(0), p___3(0)
{
    try {
        setId(Id);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "building PPA: ");
        throw;
    }
}

//copy all properties of a PPA
void TPairPositionAngles::clone(const TPairPositionAngles *PPA)
{
    //check the precondition
    if(PPA == NULL)
        throw EImproperArgument("pointer PPA should point to built pair of position angles");

    //assign the properties
    Id = PPA->Id;
    p_1 = PPA->p_1;
    p___3 = PPA->p___3;
}
TPairPositionAngles &TPairPositionAngles::operator=(const TPairPositionAngles &PPA)
{
    //assign the properties
    Id = PPA.Id;
    p_1 = PPA.p_1;
    p___3 = PPA.p___3;

    return *this;
}

//build a clon of a PPA
TPairPositionAngles::TPairPositionAngles(const TPairPositionAngles *PPA)
{
    if(PPA == NULL)
        throw EImproperArgument("pointer PPA should point to built pair position angles");

    try {
        clone(PPA);
    } catch(Exception& E) {
        E.Message.Insert(1, "cloning a PPA: ");
        throw;
    }
}

//set all properties
void TPairPositionAngles::set(const int Id, const double p_1_, const double p___3_)
{
    try {
        setId(Id);
        p_1 = p_1_;
        p___3 = p___3_;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting all properties to pair position angles: ");
    }
}

//set (p_1, p___3)
void TPairPositionAngles::set(const double p_1_, const double p___3_)
{
    p_1 = p_1_;
    p___3 = p___3_;
}

//rabdomize (p_1, p_3)
void TPairPositionAngles::randomize(const double p_1min, const double p_1max,
                                    const double p___3min, const double p___3max)
{
    p_1 = RandomUniform(p_1min, p_1max);
    p___3 = RandomUniform(p___3min, p___3max);
}


//--------------------------------------------------------------------------
//TPairPositionAnglesList
//--------------------------------------------------------------------------

//SET OF PROPERTIES IN TEXT FORMAT:

AnsiString TPairPositionAnglesList::getText(void) const
{
    //add the header
    AnsiString S = TPairPositionAngles::getLabelsRow();
    //add the table
    S += AnsiString("\r\n");
    S += getColumnText();

    return S;
}

void TPairPositionAnglesList::setText(const AnsiString &S)
{
    try {
        //index the first position of string to read
        int i = 1;
        //travel the labels of the PPA properties
        TPairPositionAngles::travelLabelsRow(S, i);
        //travel the end line
        StrTravelSeparators(S, i);
        //read the values in a tampon variable
        TPairPositionAnglesList PPAL;
        ReadSeparated(&PPAL, S, i);

        //search unexpected text
        StrTravelSeparatorsIfAny(S, i);
        if(i < S.Length()+1)
            throw EImproperArgument("unexpected text reading pair position angles");

        //clone the tampon list
        Clone(&PPAL);

    } catch(...) {
        throw;
    }
}

//PUBLIC METHODS:


//built a PPA list
TPairPositionAnglesList::TPairPositionAnglesList(void) :
        TPointersList<TPairPositionAngles>(100)
{
    Print = TPairPositionAngles::printRow;
    Read = TPairPositionAngles::readRow;
}

//copy a list of PPAs
TPairPositionAnglesList &TPairPositionAnglesList::operator=(const TPairPositionAnglesList &PPAL)
{
    //resize the list
    if(PPAL.getCount() < getCount())
        Items.DelLast(getCount() - PPAL.getCount());
    else if(PPAL.getCount() > getCount())
        Items.NewLast(PPAL.getCount() - getCount());

    //copy the items
    for(int i=0; i<getCount(); i++)
        *(Items[i]) = PPAL[i];

    return *this;
}

//search the first PPA with a given identifier
int TPairPositionAnglesList::searchId(int Id) const
{
    int i;
    for(i=0; i<getCount(); i++) {
        const TPairPositionAngles *PPA = Items[i];
        if(PPA->getId() == Id)
            return i;
    }

    return i;
}

//randomize the PPAs of the list
void TPairPositionAnglesList::randomize(const double p_1min, const double p_1max,
                                        const double p___3min, const double p___3max)
{
    for(int i=0; i<getCount(); i++)
        Items[i]->randomize(p_1min, p_1max, p___3min, p___3max);
}


//check if all PPAs are referred to different RPs
bool TPairPositionAnglesList::notAllAreReferredToDifferentRPs(void) const
{
    for(int i=0; i<getCount(); i++) {
        const TPairPositionAngles *PPAi = Items[i];

        for(int j=i+1; j<getCount(); j++) {
            const TPairPositionAngles *PPAj = Items[j];

            if(PPAi->getId() == PPAj->getId())
                return true;
        }
    }
    return false;
}

//check if all Ids of a list are in the PPA list
bool TPairPositionAnglesList::notAllIdsAreFound(const TVector<int>& Ids) const
{
    for(int i=0; i<Ids.getCount(); i++) {
        const int Id = Ids[i];

        int j = searchId(Id);
        if(j >= getCount())
            return true;
    }
    return false;
}

//compare two PPA list
bool operator!=(const TPairPositionAnglesList& PPAL1,
                       const TPairPositionAnglesList& PPAL2)

{
    //check that match sizes
    if(PPAL2.getCount() != PPAL1.getCount())
        return true;

    //check preconditions
    if(PPAL1.notAllAreReferredToDifferentRPs())
        throw EImproperCall("can't compare pair position angles list with repeated identifiers");
    if(PPAL2.notAllAreReferredToDifferentRPs())
        throw EImproperCall("can't compare pair position angles list with repeated identifiers");

    //compare items (not their locations)
    for(int i=0; i<PPAL1.getCount(); i++) {
        const TPairPositionAngles *PPA1 = PPAL1.GetPointer(i);
        int j = PPAL2.searchId(PPA1->getId());
        if(j >= PPAL2.getCount())
            return true;;
        const TPairPositionAngles *PPA2 = PPAL2.GetPointer(j);
        if(PPA1->p_1 != PPA2->p_1)
            return true;
        if(PPA1->p___3 != PPA2->p___3)
            return true;
    }

    return false;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

