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
//File: adjacentitem.cpp
//Content: class of adjacent items EAs and RPs
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "adjacentitem.h"
#include "ExclusionArea.h"
#include "RoboticPositioner.h"

//---------------------------------------------------------------------------

using namespace Lists;

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TAdjacentEA
//---------------------------------------------------------------------------

//build an item with the indicated values
TAdjacentEA::TAdjacentEA(TExclusionArea *t_EA, double t_Dmin, double t_Dend)
{
    EA = t_EA;
    Dmin = t_Dmin;
    Dend = t_Dend;
}
//clone an item
void TAdjacentEA::Clone(TAdjacentEA& AEA)
{
    EA = AEA.EA;
    Dmin = AEA.Dmin;
    Dend = AEA.Dend;
}
//build a clon of an item
TAdjacentEA::TAdjacentEA(TAdjacentEA *&AEA)
{
    //check the precondition
    if(AEA == NULL)
        throw EImproperArgument("pointer AEA should point to built adjacent EA");

    EA = AEA->EA;
    Dmin = AEA->Dmin;
    Dend = AEA->Dend;
}

//compare the Id of the EA of two adjacent EAs
int TAdjacentEA::compareIds(const TAdjacentEA *AEA1, const TAdjacentEA *AEA2)
{
    //check the preconditions
    if(AEA1 == NULL)
        throw EImproperArgument("adjacent exclusion area AEA1 should point to built adjacent EA");
    if(AEA2 == NULL)
        throw EImproperArgument("adjacent exclusion area AEA2 should point to built adjacent EA");
    if(AEA1->EA == NULL)
        throw EImproperArgument("adjacent exclusion area AEA1 should has an attached EA");
    if(AEA2->EA == NULL)
        throw EImproperArgument("adjacent exclusion area AEA2 should has an attached EA");

    //compare Id
    if(AEA1->EA->getId() < AEA2->EA->getId())
        return -1;
    if(AEA1->EA->getId() > AEA2->EA->getId())
        return 1;
    return 0;
}
//print the Id of the EA of an adjacent EA
void TAdjacentEA::printId(AnsiString &S, const TAdjacentEA *AEA)
{
    //check the precondition
    if(AEA == NULL)
        throw EImproperArgument("adjacent exclusion area AEA should pint to built adjacent EA");
    if(AEA->EA == NULL)
        throw EImproperArgument("adjacent exclusion area AEA should has an attached EA");

    //concatenate the string
    S += AEA->EA->getIdText();
}

//---------------------------------------------------------------------------
//TAdjacentEAList
//---------------------------------------------------------------------------

//build a list by default
TAdjacentEAList::TAdjacentEAList(void) :
    TPointersList<TAdjacentEA>(1, TAdjacentEA::compareIds, NULL, NULL, TAdjacentEA::printId, NULL)
{
}
//build a clon of a list
TAdjacentEAList::TAdjacentEAList(const TAdjacentEAList& AEA) : TPointersList<TAdjacentEA>(AEA)
{
}

//get partial lists in text format
string TAdjacentEAList::getIdText(void) const
{
    string str = "{";
    if(getCount() > 0) {
        str += Items.getFirst()->EA->getIdText().str;
        for(int i=1; i<getCount(); i++) {
            str += ", ";
            str += Items[i]->EA->getIdText().str;
        }
    }
    str += "}";

    return str;
}
string TAdjacentEAList::getDminText(void) const
{
    string str = "{";
    if(getCount() > 0) {
        str += "(EA" + Items.getFirst()->EA->getIdText().str + ", " + floattostr(Items.getFirst()->Dmin) + ")";
        for(int i=1; i<getCount(); i++) {
            str += ", ";
            str += "(EA" + Items[i]->EA->getIdText().str + ", " + floattostr(Items[i]->Dmin) + ")";
        }
    }
    str += "}";

    return str;
}
string TAdjacentEAList::getDendText(void) const
{
    string str = "{";
    if(getCount() > 0) {
        str += "(EA" + Items.getFirst()->EA->getIdText().str + ", " + floattostr(Items.getFirst()->Dend) + ")";
        for(int i=1; i<getCount(); i++) {
            str += ", ";
            str += "(EA" + Items[i]->EA->getIdText().str + ", " + floattostr(Items[i]->Dend) + ")";
        }
    }
    str += "}";

    return str;
}

//set a same value to all Dmins
void TAdjacentEAList::setAllDmins(double Dmin)
{
    for(int i=0; i<getCount(); i++)
        Items[i]->Dmin = Dmin;
}
//set a same value to all Dends
void TAdjacentEAList::setAllDends(double Dend)
{
    for(int i=0; i<getCount(); i++)
        Items[i]->Dend = Dend;
}

//---------------------------------------------------------------------------
//TAdjacentRP
//---------------------------------------------------------------------------

//build an item with the indicated values
TAdjacentRP::TAdjacentRP(TRoboticPositioner *t_RP, double t_Dmin, double t_Dend)
{
    RP = t_RP;
    Dmin = t_Dmin;
    Dend = t_Dend;
}
//clone an item
void TAdjacentRP::Clone(TAdjacentRP& ARP)
{
    RP = ARP.RP;
    Dmin = ARP.Dmin;
    Dend = ARP.Dend;
}
//build a clon of an item
TAdjacentRP::TAdjacentRP(TAdjacentRP *&ARP)
{
    //check the precondition
    if(ARP == NULL)
        throw EImproperArgument("pointer ARP should point to built adjacent RP");

    RP = ARP->RP;
    Dmin = ARP->Dmin;
    Dend = ARP->Dend;
}

//compare the Id of the RP of two adjacent RPs
int TAdjacentRP::compareIds(const TAdjacentRP *ARP1, const TAdjacentRP *ARP2)
{
    //check the preconditions
    if(ARP1 == NULL)
        throw EImproperArgument("adjacent exclusion arRP ARP1 should point to built adjacent RP");
    if(ARP2 == NULL)
        throw EImproperArgument("adjacent exclusion arRP ARP2 should point to built adjacent RP");
    if(ARP1->RP == NULL)
        throw EImproperArgument("adjacent exclusion arRP ARP1 should has an attached RP");
    if(ARP2->RP == NULL)
        throw EImproperArgument("adjacent exclusion arRP ARP2 should has an attached RP");

    //compare Id
    if(ARP1->RP->getActuator()->getId() < ARP2->RP->getActuator()->getId())
        return -1;
    if(ARP1->RP->getActuator()->getId() > ARP2->RP->getActuator()->getId())
        return 1;
    return 0;
}
//print the Id of the RP of an adjacent RP
void TAdjacentRP::printId(AnsiString &S, const TAdjacentRP *ARP)
{
    //check the precondition
    if(ARP == NULL)
        throw EImproperArgument("adjacent exclusion arRP ARP should pint to built adjacent RP");
    if(ARP->RP == NULL)
        throw EImproperArgument("adjacent exclusion arRP ARP should has an attached RP");

    //concatenate the string
    S += ARP->RP->getActuator()->getIdText();
}

//---------------------------------------------------------------------------
//TAdjacentRPList
//---------------------------------------------------------------------------

//build a list by default
TAdjacentRPList::TAdjacentRPList(void) :
    TPointersList<TAdjacentRP>(6, TAdjacentRP::compareIds, NULL, NULL, TAdjacentRP::printId, NULL)
{
}
//build a clon of a list
TAdjacentRPList::TAdjacentRPList(const TAdjacentRPList& ARP) : TPointersList<TAdjacentRP>(ARP)
{
}

//get partial lists in text format
string TAdjacentRPList::getIdText(void) const
{
    string str = "{";
    if(getCount() > 0) {
        str += Items.getFirst()->RP->getActuator()->getIdText().str;
        for(int i=1; i<getCount(); i++) {
            str += ", ";
            str += Items[i]->RP->getActuator()->getIdText().str;
        }
    }
    str += "}";

    return str;
}
string TAdjacentRPList::getDminText(void) const
{
    string str = "{";
    if(getCount() > 0) {
        str += "(RP" + Items.getFirst()->RP->getActuator()->getIdText().str + ", " + floattostr(Items.getFirst()->Dmin) + ")";
        for(int i=1; i<getCount(); i++) {
            str += ", ";
            str += "(RP" + Items[i]->RP->getActuator()->getIdText().str + ", " + floattostr(Items[i]->Dmin) + ")";
        }
    }
    str += "}";

    return str;
}
string TAdjacentRPList::getDendText(void) const
{
    string str = "{";
    if(getCount() > 0) {
        str += "(RP" + Items.getFirst()->RP->getActuator()->getIdText().str + ", " + floattostr(Items.getFirst()->Dend) + ")";
        for(int i=1; i<getCount(); i++) {
            str += ", ";
            str += "(RP" + Items[i]->RP->getActuator()->getIdText().str + ", " + floattostr(Items[i]->Dend) + ")";
        }
    }
    str += "}";

    return str;
}

//set a same value to all Dmins
void TAdjacentRPList::setAllDmins(double Dmin)
{
    for(int i=0; i<getCount(); i++)
        Items[i]->Dmin = Dmin;
}
//set a same value to all Dends
void TAdjacentRPList::setAllDends(double Dend)
{
    for(int i=0; i<getCount(); i++)
        Items[i]->Dend = Dend;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
