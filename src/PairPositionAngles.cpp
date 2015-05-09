// Copyright (c) 2014-2015 Isaac Morales Durán. All rights reserved.
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
//File: PairPositionAngles.cpp
//Content: pair of projection angles
//Last update: 30/12/2014
//Author: Isaac Morales Durán
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
AnsiString TPairPositionAngles::IdLabel;
//label of property p_1
//default value: "p_1"
AnsiString TPairPositionAngles::p_1Label;
//label of property p___3
//default value: "p___3"
AnsiString TPairPositionAngles::p___3Label;

//PROPIEDADES:

//single identification number of the attached robotic positioner
//      RoboticPositioner->Id
//shall be nonnegative
int TPairPositionAngles::getId(void) const
{
    return getRoboticPositioner()->getActuator()->getId();
}
/*void TPairPositionAngles::setId(int Id)
{
    try {
        getRoboticPositioner()->setId(Id);
    } catch(...) {
        throw;
    }
}*/

//PROPERTIES IN TEXT FORMAT:

AnsiString TPairPositionAngles::getIdText(void) const
{
    return IntToStr(getId());
}
/*void TPairPositionAngles::setIdText(const AnsiString &S)
{
    try {
        setId(StrToInt_(S));
    } catch(...) {
        throw;
    }
}*/
AnsiString TPairPositionAngles::getp_1Text(void) const
{
    return IntToStr(p_1);
}
void TPairPositionAngles::setp_1Text(const AnsiString &S)
{
    try {
        p_1 = StrToInt_(S);
    } catch(...) {
        throw;
    }
}
AnsiString TPairPositionAngles::getp___3Text(void) const
{
    return IntToStr(p___3);
}
void TPairPositionAngles::setp___3Text(const AnsiString &S)
{
    try {
        p___3 = StrToInt_(S);
    } catch(...) {
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
    try {
        TPairPositionAngles PPA; //tampon object
        int i = 1;        
        ReadRow(&PPA, S, i);
        StrTravelToEnd(S, i);

        if(i <= S.Length())
            throw EImproperArgument("string S should contains only a PPA");

        Copy(&PPA); //clonethe tampon object

    } catch(...) {
        throw;
    }
}

//STATIC METHODS:

//labels of all properties in a row:
//      IdLabel+"\t"p_1Label+"\t"p___3Label
AnsiString TPairPositionAngles::getLabelsRow(void)
{
    return IdLabel+AnsiString("\t")+p_1Label+AnsiString("\t")+p___3Label;
}
//travel the labels of the properties
//in row text format
void TPairPositionAngles::travelLabelsRow(const AnsiString& S, int& i)
{
    //index i should indicates a position in the string S
    if(i<0 || S.Length()<i)
        throw EImproperArgument("index i shall indicates a position in the string S");

    try {
        StrTravelLabel(IdLabel, S, i);
        StrTravelSeparators(S,i);
        StrTravelLabel(p_1Label, S, i);
        StrTravelSeparators(S,i);
        StrTravelLabel(p___3Label, S, i);

    } catch(...) {
        throw;
    }
}

//print the properties of a pair of position angles in a string
//in row format
void  TPairPositionAngles::PrintRow(AnsiString &S,
                                const TPairPositionAngles *PPA)
{
    //pointer PPA should point to built pair of position angles
    if(PPA == NULL)
        throw EImproperArgument("pointer PPA should point to built pair of position angles");

    //add the text to the string in propeerly format
    S += PPA->getRowText();
}

//read the properties of a pair of position angles in a string
//in row format
void  TPairPositionAngles::ReadRow(TPairPositionAngles *PPA,
                                   const AnsiString &S, int &i)
{
    //pointer PPA shoult point to built pair of position angles
    if(PPA == NULL)
        throw EImproperArgument("pointer PPA shoult point to built pair of position angles");

    //if index i not indicate a position in the string
    if(i<1 || S.Length()<i)
        //indicates that not has been found the values to PPA
        throw EImproperArgument("values to PPAnot found");

    //reading status
    //      0: waiting value for Id
    //      1: waiting value for p_1
    //      2: waiting value for p___3
    //      3: PPA readed successfully
    int status = 0;

    //variables auxiliares
    TPairPositionAngles t_PPA(PPA); //tampon variable
    AnsiString Value; //tampon string

    //ADVERTENCIA: las variables tampón con propiedades interdependientes
    //deben ser clones de las variables que se pretenden modificar.

    //NOTA: adviertase que las propiedades enteras son leidas como
    //valores en punto flotante para detectar errores en los cuales
    //sea especificado un valor en punto flotante en vez de un valor entero.

    do {
        switch(status) {
        case 0: //waiting value for Id
            try {
                StrReadWord(Value, S, i);
//                t_PPA.setIdText(Value);
            } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property Id"));
            } catch(...) {
                throw;
            }
            if(i > S.Length())
                throw EImproperArgument("valueto p_1 not found");
            status++;
            break;

        case 1: //waiting value for p_1
            try {
                StrReadWord(Value, S, i);
                t_PPA.setp_1Text(Value);
            } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property p_1"));
            } catch(...) {
                throw;
            }
            if(i > S.Length())
                throw EImproperArgument("valueto p___3 not found");
            status++;
            break;

        case 2: //waiting valuefor p___3
            try {
                StrReadWord(Value, S, i);
                t_PPA.setp___3Text(Value);
            } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property p___3"));
            } catch(...) {
                throw;
            }
            status++;
            break;
        }
    //while the PPA it isn't readed successfully
    } while(status < 3);

    //assign the tampon variable
    PPA->Copy(&t_PPA);
}

//PUBLIC METHODS:

//builds a PPA
TPairPositionAngles::TPairPositionAngles(TRoboticPositioner *RP)
{
    //Pointer RP can be null.

    //point the extern objects
    p_RoboticPositioner = RP;

    //initialize the properties
    p_1 = 0;
    p___3 = 0;
}
//clone a PPA
TPairPositionAngles::TPairPositionAngles(TPairPositionAngles *PPA)
{
    try {
        Copy(PPA);
    } catch(...) {
        throw;
    }
}

//copy all properties of a PPA
void TPairPositionAngles::Copy(TPairPositionAngles *PPA)
{
    //pointer PPA should point to built pair of position angles
    if(PPA == NULL)
        throw EImproperArgument("pointer PPA should point to built pair of position angles");

    //assign the properties
    p_RoboticPositioner = PPA->p_RoboticPositioner;
    p_ProjectionPoint = PPA->p_ProjectionPoint;
    p_1 = PPA->p_1;
    p___3 = PPA->p___3;
}
TPairPositionAngles &TPairPositionAngles::operator=(const TPairPositionAngles &PPA)
{
    //assign the properties
    p_RoboticPositioner = PPA.p_RoboticPositioner;
    p_ProjectionPoint = PPA.p_ProjectionPoint;
    p_1 = PPA.p_1;
    p___3 = PPA.p___3;

    return *this;
}

//rabdomize the PPA
void TPairPositionAngles::Randomize(double p_1min, double p_1max,
                                    double p___3min, double p___3max)
{
    p_1 = RandomUniform(p_1min, p_1max);
    p___3 = RandomUniform(p___3min, p___3max);
}


//--------------------------------------------------------------------------
//TPairPositionAnglesList
//--------------------------------------------------------------------------

//SET OF PROPERTIES IN TEXT FORMAT:

AnsiString TPairPositionAnglesList::getText(void)
{
    AnsiString S;

    //añade la cabecera
    S = TPairPositionAngles::getLabelsRow()+AnsiString("\r\n");
//    for(int i=0; i<getCount(); i++)
  //      S += Items[i]->getRowText()+AnsiString("\r\n");
    S += getColumnText();

    return S;
}

void TPairPositionAnglesList::setText(const AnsiString &S)
{
    try {
        //index the first position of string to read
        int i = 1;
        //travel the labels of the PAP properties
        TPairPositionAngles::travelLabelsRow(S, i);
        //travel the end line
        StrTravelSeparators(S, i);
        //read the values in a tampon variable
        TPairPositionAnglesList PPAL;
        ReadSeparated(&PPAL, S, i);
        //travel the rest of separators of the string
        StrTravelToEnd(S, i);

        //if the string contains anymore
        if(i < S.Length()+1)
            //indicates that string S should contains only a PPA list
            throw EImproperArgument("string S shouldcontains only a PPA list");

        //clone the tampon list
        Clone(&PPAL);

    } catch(...) {
        throw;
    }
}

//PUBLIC METHODS:


//built a PPA list
TPairPositionAnglesList::TPairPositionAnglesList(void) :
        TPointersList<TPairPositionAngles>()
{
    Print = TPairPositionAngles::PrintRow;
    Read = TPairPositionAngles::ReadRow;
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

/*//add the PPAs attached to the PPs of a PP list
void TPairPositionAnglesList::Build(TProjectionPointList &PPL)
{
    for(int i=0; i<PPL.getCount(); i++)
        Add(new TPairPositionAngles(PPL.GetPointer(i)));
}
*/

//search the first PPA with a given identifier
int TPairPositionAnglesList::SearchId(int Id) const
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
void TPairPositionAnglesList::Randomize(double p_1min, double p_1max,
               double p___3min, double p___3max)
{
    try {
        for(int i=0; i<getCount(); i++)
            Items[i]->Randomize(p_1min, p_1max, p___3min, p___3max);
    } catch(...) {
        throw;
    }
}


//check if all PPAs are addresed to different RPs
bool TPairPositionAnglesList::notAllPPAsAreAddresedToDifferentRPs(void) const
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
        int Id = Ids[i];

        int j = SearchId(Id);
        if(j >= getCount())
            return true;
    }
    return false;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

