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
//File: Actuator.cpp
//Content: actuator model of a RP
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Actuator.h"
#include "RoboticPositioner.h"
#include "Strings.h"
#include "Geometry.h"
#include "RoboticPositioner.h"
#include "TextFile.h"

#include <algorithm> //std::min, std::max

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TKnowledgeDegree
//---------------------------------------------------------------------------

void  StrPrintKnowledgeDegree(AnsiString& S, TKnowledgeDegree kd)
{
    switch(kd) {
    case kdPre: S += AnsiString("Pre"); break;
    case kdApp: S += AnsiString("App"); break;
    case kdUnk: S += AnsiString("Unk"); break;
    }
    throw EImpossibleError("unknowledge value in type TKnowledgeValue");
}
void  StrReadKnowledgeDegree(TKnowledgeDegree& kd,
                             const AnsiString &S, int &i)
{
    //ADVERTENCIA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //La longitud de los valores legibles de la cadena son:
    //      strlen("Pre"): 3
    //      strlen("Apr"): 3
    //      strlen("Unk"): 3

    //variable tampón
    AnsiString SubS;

    //copia el índice en una variable auxiliar
    int j = i;
    //avanza el índice auxiliar hasta el próximo caracter no separador
    StrTravelSeparatorsIfAny(S, j);

    //si desde el primera caracter hay al menos 3 caracteres (con él incluido)
    if(j+2 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 3 caracteres
        SubS = S.SubString(j, 3);

        //si la subcadena coincide con kdPre
        if(SubS == "Pre") {
            //traduce de texto a estructura
            kd = kdPre;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 3;
            return; //no hace nada más
        }

        //si la subcadena coincide con kdApp
        if(SubS == "App") {
            //traduce de texto a estructura
            kd = kdApp;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 3;
            return; //no hace nada más
        }

        //si la subcadena coincide con kdUnk
        if(SubS == "Unk") {
            //traduce de texto a estructura
            kd = kdUnk;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 3;
            return; //no hace nada más
        }
    }

    throw EImproperArgument("there is a value of type TKnowledgeValue as from position i in string S");
}
AnsiString KnowledgeDegreeToStr(TKnowledgeDegree kd)
{
    switch(kd) {
    case kdPre: return AnsiString("Pre");
    case kdApp: return AnsiString("App");
    case kdUnk: return AnsiString("Unk");
    }
    throw EImpossibleError("unknowledge value in type TKnowledgeValue");
}
TKnowledgeDegree StrToKnowledgeDegree(const AnsiString& S)
{
    //ADVERTENCIA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //La longitud de los valores legibles de la cadena son:
    //      strlen("Pre"): 3
    //      strlen("App"): 3
    //      strlen("Unk"): 3

    //variable tampón
    AnsiString SubS;

    //inicializa el índice
    int j = 1;
    //avanza el índice auxiliar hasta el próximo caracter no separador
    StrTravelSeparatorsIfAny(S, j);

    //si desde el primera caracter hay al menos 3 caracteres (con él incluido)
    if(j+2 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 3 caracteres
        SubS = S.SubString(j, 3);

        //si la subcadena coincide con kdPre
        if(SubS == "Pre")
            //devuelve el valor correspondiente
            return kdPre;

        //si la subcadena coincide con kdApp
        if(SubS == "App")
            //devuelve el valor correspondiente
            return kdApp;

        //si la subcadena coincide con kdUnk
        if(SubS == "Unk")
            //devuelve el valor correspondiente
            return kdUnk;
    }

    throw EImproperArgument("there is a value of type TKnowledgeValue as from position i in string S");
}

//---------------------------------------------------------------------------
//TPurpose

void  strPrintPurpose(string& str, TPurpose p)
{
    switch(p) {
    case pAll: str += "All"; break;
    case pGenPairPPDP: str += "GenPairPPDP"; break;
    case pValDP: str += "ValPP"; break;
    case pValPP: str += "ValPP"; break;
    case pGenParPro: str += "GenParPro"; break;
    case pValParPro: str += "ValParPro"; break;
    case pExe: str += "Exe"; break;
    }
    throw EImpossibleError("unknowledge value in type TPurpose");
}
void  strReadPurpose(TPurpose& value, const string& str, unsigned int &i)
{
    //Here it is not required that text string str is printable.

    //check the precondition
    if(str.length() < i)
        throw EImproperArgument("index i should indicate a char of the string str");

    //Length of the known possible values of the string:
    //  strlen("All"): 3
    //  strlen("GenPairPPDP"): 11
    //  strlen("ValDP"): 5
    //  strlen("ValPP"): 5
    //  strlen("GenParPro"): 9
    //  strlen("ValParPro"): 9
    //  strlen("Exe"): 3

    //copy the index in an auxiliary variable
    unsigned int i_ = i;

    //advance the auxiliary index to the nextnon separator character
    strTravelSeparatorsIfAny(str, i_);

    //determine if the label is "All" and return the corresponding value if any
    bool label_found = strTravelLabel_("All", str, i_);
    if(label_found) {
        i = i_;
        value = pAll;
        return;
    }

    //determine if the label is "GenPairPPDP" and return the corresponding value if any
    label_found = strTravelLabel_("GenPairPPDP", str, i_);
    if(label_found) {
        i = i_;
        value = pGenPairPPDP;
        return;
    }

    //determine if the label is "ValDP" and return the corresponding value if any
    label_found = strTravelLabel_("ValDP", str, i_);
    if(label_found) {
        i = i_;
        value = pValDP;
        return;
    }

    //determine if the label is "ValPP" and return the corresponding value if any
    label_found = strTravelLabel_("ValPP", str, i_);
    if(label_found) {
        i = i_;
        value = pValPP;
        return;
    }

    //determine if the label is "GenParPro" and return the corresponding value if any
    label_found = strTravelLabel_("GenParPro", str, i_);
    if(label_found) {
        i = i_;
        value = pGenParPro;
        return;
    }

    //determine if the label is "ValParPro" and return the corresponding value if any
    label_found = strTravelLabel_("ValParPro", str, i_);
    if(label_found) {
        i = i_;
        value = pValParPro;
        return;
    }

    //determine if the label is "Exe" and return the corresponding value if any
    label_found = strTravelLabel_("Exe", str, i_);
    if(label_found) {
        i = i_;
        value = pExe;
        return;
    }

    throw EImproperArgument("there isn't' a value of type TPurpose from position i in string str");
}
string purposeToStr(TPurpose value)
{
    switch(value) {
    case pAll: return "All";
    case pGenPairPPDP: return "GenPairPPDP";
    case pValDP: return "ValDP";
    case pValPP: return "ValPP";
    case pGenParPro: return "GenParPro";
    case pValParPro: return "ValParPro";
    case pExe: return "Exe";
    }
    throw EImpossibleError("unknowledge value in type TPurpose");
}
TPurpose strToPurpose(const string& str)
{
    //Here it is not required that text string str is printable.

    //Length of the known possible values of the string:
    //  strlen("All"): 3
    //  strlen("GenPairPPDP"): 11
    //  strlen("ValDP"): 5
    //  strlen("ValPP"): 5
    //  strlen("GenParPro"): 9
    //  strlen("ValParPro"): 9
    //  strlen("Exe"): 3

    //copy the index in an auxiliary variable
    unsigned int i_ = 0;

    //advance the auxiliary index to the nextnon separator character
    strTravelSeparatorsIfAny(str, i_);

    //determine if the label is "All" and return the corresponding value if any
    bool label_found = strTravelLabel_("All", str, i_);
    if(label_found)
        return pAll;

    //determine if the label is "GenPairPPDP" and return the corresponding value if any
    label_found = strTravelLabel_("GenPairPPDP", str, i_);
    if(label_found)
        return pGenPairPPDP;

    //determine if the label is "ValDP" and return the corresponding value if any
    label_found = strTravelLabel_("ValDP", str, i_);
    if(label_found)
        return pValDP;

    //determine if the label is "ValPP" and return the corresponding value if any
    label_found = strTravelLabel_("ValPP", str, i_);
    if(label_found)
        return pValPP;

    //determine if the label is "GenParPro" and return the corresponding value if any
    label_found = strTravelLabel_("GenParPro", str, i_);
    if(label_found)
        return pGenParPro;

    //determine if the label is "ValParPro" and return the corresponding value if any
    label_found = strTravelLabel_("ValParPro", str, i_);
    if(label_found)
        return pValParPro;

    //determine if the label is "Exe" and return the corresponding value if any
    label_found = strTravelLabel_("Exe", str, i_);
    if(label_found)
        return pExe;

    throw EImproperArgument("there isn't' a value of type TPurpose from position i in string str");
}

//---------------------------------------------------------------------------
//TAdjacentEA
//---------------------------------------------------------------------------

//build an item with the indicated values
TAdjacentEA::TAdjacentEA(TExclusionArea *t_EA, double t_Dmin, double t_Dend)
{
    EA = t_EA;
    Dmin = t_Dmin;
    Dmin = t_Dend;
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
        str += Items.getFirst()->EA->getId();
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
    Dmin = t_Dend;
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
    TPointersList<TAdjacentRP>(1, TAdjacentRP::compareIds, NULL, NULL, TAdjacentRP::printId, NULL)
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
        str += Items.getFirst()->RP->getActuator()->getId();
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
//TPairEADmin
//---------------------------------------------------------------------------

//pointer to EA attached to Dmin
void TPairEADmin::setEA(TExclusionArea *EA)
{
    //comprueba las precondiciones
    if(EA == NULL)
        throw EImproperArgument("pointer EA should point to built EA");

    p_EA = EA; //assign the new value
}

//minimun distance with the actuator of the attached EA
void TPairEADmin::setDmin(double Dmin)
{
    //comprueba las precondiciones
    if(Dmin < 0)
        throw EImproperArgument("minimun distance Dmin should be nonnegative");

    p_Dmin = Dmin; //assign the new value
}

//get the item in text format
AnsiString TPairEADmin::getText(void) const
{
    string str = "(EA"+getEA()->getIdText().str+", "+floattostr(getDmin())+")";
    return AnsiString(str);
}

//STATIC METHODS FOR LISTS:

//asigna un valor a un elemento
void  TPairEADmin::assign(TPairEADmin& item, double Dmin)
{
    try {
        item.setDmin(Dmin);
    } catch(Exception& E) {
        E.Message.Insert(1, "assign value to pair (EA, Dmin): ");
        throw;
    }
}
//print the item in a string
void  TPairEADmin::print(AnsiString &S, TPairEADmin item)
{
    S += item.getText();
}

//CONTRUCTOR:

//build an item of the class
TPairEADmin::TPairEADmin(TExclusionArea *EA, double Dmin)
{
    //comprueba las precondiciones
    if(EA == NULL)
        throw EImproperArgument("pointer EA should point to built EA");

    //comprueba las precondiciones
    if(Dmin < 0)
        throw EImproperArgument("minimun distance Dmin should be nonnegative");

    p_EA = EA; //assign the new value
    p_Dmin = Dmin; //assign the new value
}

//---------------------------------------------------------------------------
//TPairRPDmin
//---------------------------------------------------------------------------

//pointer to RP attached to Dmin
void TPairRPDmin::setRP(TRoboticPositioner *RP)
{
    //comprueba las precondiciones
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built RP");

    p_RP = RP; //assign the new value
}

//minimun distance with the actuator of the attached RP
void TPairRPDmin::setDmin(double Dmin)
{
    //comprueba las precondiciones
    if(Dmin < 0)
        throw EImproperArgument("minimun distance Dmin should be nonnegative");

    p_Dmin = Dmin; //assign the new value
}

//get the item in text format
AnsiString TPairRPDmin::getText(void) const
{
    string str = "(RP"+getRP()->getActuator()->getIdText().str+", "+floattostr(getDmin())+")";
    return AnsiString(str);
}

//STATIC METHODS FOR LISTS:

//asigna un valor a un elemento
void  TPairRPDmin::assign(TPairRPDmin& item, double Dmin)
{
    try {
        item.setDmin(Dmin);
    } catch(Exception& E) {
        E.Message.Insert(1, "assign value to pair (RP, Dmin): ");
        throw;
    }
}
//print the item in a string
void  TPairRPDmin::print(AnsiString &S, TPairRPDmin item)
{
    S += item.getText();
}

//CONTRUCTOR:

//build an item of the class
TPairRPDmin::TPairRPDmin(TRoboticPositioner *RP, double Dmin)
{
    //comprueba las precondiciones
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built RP");

    //comprueba las precondiciones
    if(Dmin < 0)
        throw EImproperArgument("minimun distance Dmin should be nonnegative");

    p_RP = RP; //assign the new value
    p_Dmin = Dmin; //assign the new value
}

//---------------------------------------------------------------------------
//TActuator
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//PROPIEDADES ESTÁTICAS:

//--------------------------------------------------------------------------
//PROPIEDADES DE SEGURIDAD:

//COMPONENTES DE SPM:

void TActuator::setSPMrec(double SPMrec)
{
    //check the precondition
    if(SPMrec < 0)
        throw EImproperArgument("the recovery SPM (SPMrec) should be nonnegative");

    p_SPMrec = SPMrec; //assigns the new valor
    AssignSPM(); //assimilates the new valor
}
void TActuator::setSPMsta(double SPMsta)
{
    //check the precondition
    if(SPMsta < 0)
        throw EImproperArgument("the staerical SPM (SPMsta) should be nonnegative");

    p_SPMsta = SPMsta; //assigns the new valor
    AssignSPM(); //assimilates the new valor
}
void TActuator::setSPMdyn(double SPMdyn)
{
    //check the precondition
    if(SPMdyn < 0)
        throw EImproperArgument("the dynamic SPM (SPMdyn) should be nonnegative");

    p_SPMdyn = SPMdyn; //assigns the new valor
    AssignSPM(); //assimilates the new valor
}
void TActuator::setSPMmin(double SPMmin)
{
    //check the precondition
    if(SPMmin < 0)
        throw EImproperArgument("the minimum jump SPM (SPMmin) should be nonnegative");

    p_SPMmin = SPMmin; //assigns the new valor
    AssignSPM(); //assimilates the new valor
}
void TActuator::setSPMsim(double SPMsim)
{
    //check the precondition
    if(SPMsim < 0)
        throw EImproperArgument("the simulation SPM (SPMsim) should be nonnegative");

    p_SPMsim = SPMsim; //assigns the new valor
    AssignSPM(); //assimilates the new valor
}
void TActuator::setSPMoff(double SPMoff)
{
    //el SPM de desplazamiento (SPMoff) debe ser no negativo
    if(SPMoff < 0)
        throw EImproperArgument("the offset SPM (SPMoff) should be nonnegative");

    p_SPMoff = SPMoff; //assigns the new valor
    AssignSPM(); //assimilates the new valor
}

//VALOR DE SPM PARA CADA OCASIÓN:

//SPM for execution:
//  SPMsta + SPMdyn
double TActuator::getSPMexe_p(void) const
{
    return getSPMsta() + getSPMdyn();
}
//SPM for validate a parking program:
//  SPMexe_p + SPMmin + SPMsim
double TActuator::getSPMvalParPro_p(void) const
{
    return getSPMexe_p() + getSPMmin() + getSPMsim();
}
//SPM for generate a parking program:
//  SPMvalParPro_p + SPMmin + SPMsim
double TActuator::getSPMgenParPro_p(void) const
{
    return getSPMvalParPro_p() + getSPMmin() + getSPMsim();
}
//SPM for validate a PP:
//  SPMgenParPro_p + SPMmin + SPMsim
double TActuator::getSPMvalPP_p(void) const
{
    return getSPMgenParPro_p() + getSPMmin() + getSPMsim();
}
//SPM for validate a DP:
//  SPMvalPP_p + SPMmin + SPMsim
double TActuator::getSPMvalDP_p(void) const
{
    return getSPMvalPP_p() + getSPMmin() + getSPMsim();
}
//SPM for generate a pair (PP, DP):
//  SPMvalDP_p + SPMmin + SPMsim
double TActuator::getSPMgenPairPPDP_p(void) const
{
    return getSPMvalDP_p() + getSPMmin() + getSPMsim();
}
//SPM for allocation:
//  SPMgenPairPPDP_p + SPMoff
double TActuator::getSPMall_p(void) const
{
    return getSPMgenPairPPDP_p() + getSPMoff();
}

//SPM for execution:
//  SPMrec + SPMexe_p
double TActuator::getSPMexe_a(void) const
{
    return getSPMrec() + getSPMexe_p();
}
//SPM for validate a parking program:
//  SPMexe_a + SPMmin + SPMsim
double TActuator::getSPMvalParPro_a(void) const
{
    return getSPMexe_a() + getSPMmin() + getSPMsim();
}
//SPM for generate a parking program:
//  SPMvalParPro_a + SPMmin + SPMsim
double TActuator::getSPMgenParPro_a(void) const
{
    return getSPMvalParPro_a() + getSPMmin() + getSPMsim();
}
//SPM for validate a PP:
//  SPMgenParPro_a + SPMmin + SPMsim
double TActuator::getSPMvalPP_a(void) const
{
    return getSPMgenParPro_a() + getSPMmin() + getSPMsim();
}
//SPM for validate a DP:
//  SPMvalPP_a + SPMmin + SPMsim
double TActuator::getSPMvalDP_a(void) const
{
    return getSPMvalPP_a() + getSPMmin() + getSPMsim();
}
//SPM for generate a pair (PP, DP):
//  SPMvalDP_a + SPMmin + SPMsim
double TActuator::getSPMgenPairPPDP_a(void) const
{
    return getSPMvalDP_a() + getSPMmin() + getSPMsim();
}
//SPM for allocation:
//  SPMgenPairPPDP_a + SPMoff
double TActuator::getSPMall_a(void) const
{
    return getSPMgenPairPPDP_a() + getSPMoff();
}

//---------------------------------------------------------------------------
//PROPIEDADES DE ESTADO:

void TActuator::setPAkd(TKnowledgeDegree PAkd)
{
    //si el nuevo valor difiere del actual
    if(PAkd != getPAkd()) {
        //asigna el nuevo valor
        p_PAkd = PAkd;

        //asimila PAkd
        AssignSPM();
    }
}
void TActuator::setPurpose(TPurpose Purpose)
{
    //si el nuevo valor difiere del actual
    if(Purpose != getPurpose()) {
        //asigna el nuevo valor
        p_Purpose = Purpose;

        //asimila Purpose
        AssignSPM();
    }
}

void TActuator::setId(int Id)
{
    //el número de identificación Id debe ser no negativo
    if(Id < 0)
        throw EImproperArgument("number identifier Id should be nonnegative");

    //ADVERTENCIA: está permitida la duplicidad de números de identificación Id.

    p_Id = Id; //asigna el nuevo valor
}

//--------------------------------------------------------------------------
//PROPIEDADES DE ÁREA:

void TActuator::setr_3maxnom(double r_3maxnom)
{
    //comprueba las precondiciones
    if(r_3maxnom <= 0)
        throw EImproperArgument("nominal domain radio of P3 r_3maxnom should be upper zero");
    if(DBL_MAX <= r_3maxnom)
        throw EImproperArgument("nominal domain radio of P3 r_3maxnom should be less infinite");
    if(AdjacentRPs.getCount()<0 || 6<AdjacentRPs.getCount())
        throw EImproperArgument("adjacent ositioner number AdjacentRPs.Count should be in [0, 6]");

    p_r_3maxnom = r_3maxnom; //asigna el nuevo valor

    //------------------------------------------------------------------

    //renombra el radio para simplificar la notación
    double &r = p_r_3maxnom;

    //calcula el área del círculo de radio r
    p_Sc = M_PI*r*r;

    //calcula el área de una porción exagonal del círculo de radio r
    p_Sw = getSc()/6;

    //calcula el coseno de PI/6
    double aux = sqrt(0.75);

    //calcula el área participativa de una porción triangular
    //del exágono cinrcunscrito en la circunferencia de radio r
    p_Sp = r*r*aux/2;

    //calcula el área de uno de los segmentos de circunferencia
    //en las inmediaciones del exágono circunscrito
    //en la circunferencia de radio r
    p_Ss = getSw() - getSp(); //= r*r*(M_PI/6 - aux/2)

    //calcula el área exclusiva de una porción exagonal
    //del círculode radio r
    p_Se = getSp() - getSs(); //= r*r*(aux - M_PI/6)

    //calcula el ratio entre área exclusiva y el área participativa
    p_Re = getSe()/getSp();

    //calcula el número de RPs no adyacentes
    aux = double(6 - AdjacentRPs.getCount());

    //calcula el área participativa total
    p_Spt = aux*getSw() + AdjacentRPs.getCount()*getSp(); //r*r*(M_PI + N*(sqrt(0.75) - M_PI/3)/2);
    //calcula el área exclusiva total
    p_Set = aux*getSw() + AdjacentRPs.getCount()*getSe(); //r*r*(sqrt(0.75) - M_PI/6);
    //calcula el ratio entre Set y Spt
    p_Ret = getSet()/getSpt(); //2 - M_PI/(3*sqrt(0.75))
}

//--------------------------------------------------------------------------
//PROPIEDADES DE SEGURIDAD EN FORMATO TEXTO:

//COMPONENTES DE SPM:

AnsiString TActuator::getSPMrecText(void) const
{
    return FloatToStr(getSPMrec());
}
void TActuator::setSPMrecText(const AnsiString &S)
{
    try {
        setSPMrec(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting SPMrec in text format: ");
        throw;
    }
}
AnsiString TActuator::getSPMstaText(void) const
{
    return FloatToStr(getSPMsta());
}
void TActuator::setSPMstaText(const AnsiString &S)
{
    try {
        setSPMsta(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting SPMsta in text format: ");
        throw;
    }
}
AnsiString TActuator::getSPMdynText(void) const
{
    return FloatToStr(getSPMdyn());
}
void TActuator::setSPMdynText(const AnsiString &S)
{
    try {
        setSPMdyn(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting SPMdyn in text format: ");
        throw;
    }
}
AnsiString TActuator::getSPMminText(void) const
{
    return FloatToStr(getSPMmin());
}
void TActuator::setSPMminText(const AnsiString &S)
{
    try {
        setSPMmin(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting SPMmin in text format: ");
        throw;
    }
}
AnsiString TActuator::getSPMsimText(void) const
{
    return FloatToStr(getSPMsim());
}
void TActuator::setSPMsimText(const AnsiString &S)
{
    try {
        setSPMsim(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting SPMsim in text format: ");
        throw;
    }
}
AnsiString TActuator::getSPMoffText(void) const
{
    return FloatToStr(getSPMoff());
}
void TActuator::setSPMoffText(const AnsiString &S)
{
    try {
        setSPMoff(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting SPMoff in text format: ");
        throw;
    }
}

//VALOR DE SPM PARA CADA OCASIÓN:

AnsiString TActuator::getSPMexe_pText(void) const
{
    return FloatToStr(getSPMexe_p());
}
AnsiString TActuator::getSPMvalParPro_pText(void) const
{
    return FloatToStr(getSPMvalParPro_p());
}
AnsiString TActuator::getSPMgenParPro_pText(void) const
{
    return FloatToStr(getSPMgenParPro_p());
}
AnsiString TActuator::getSPMvalPP_pText(void) const
{
    return FloatToStr(getSPMvalPP_p());
}
AnsiString TActuator::getSPMvalDP_pText(void) const
{
    return FloatToStr(getSPMvalDP_p());
}
AnsiString TActuator::getSPMgenPairPPDP_pText(void) const
{
    return FloatToStr(getSPMgenPairPPDP_p());
}
AnsiString TActuator::getSPMall_pText(void) const
{
    return FloatToStr(getSPMall_p());
}

AnsiString TActuator::getSPMexe_aText(void) const
{
    return FloatToStr(getSPMexe_a());
}
AnsiString TActuator::getSPMvalParPro_aText(void) const
{
    return FloatToStr(getSPMvalParPro_a());
}
AnsiString TActuator::getSPMgenParPro_aText(void) const
{
    return FloatToStr(getSPMgenParPro_a());
}
AnsiString TActuator::getSPMvalPP_aText(void) const
{
    return FloatToStr(getSPMvalPP_a());
}
AnsiString TActuator::getSPMvalDP_aText(void) const
{
    return FloatToStr(getSPMvalDP_a());
}
AnsiString TActuator::getSPMgenPairPPDP_aText(void) const
{
    return FloatToStr(getSPMgenPairPPDP_a());
}
AnsiString TActuator::getSPMall_aText(void) const
{
    return FloatToStr(getSPMall_a());
}

//--------------------------------------------------------------------------
//PROPIEDADES DE ESTADO EN FORMATO TEXTO:

AnsiString TActuator::getPAkdText(void) const
{
    return KnowledgeDegreeToStr(getPAkd());
}
void TActuator::setPAkdText(const AnsiString &S)
{
    try {
        setPAkd(StrToKnowledgeDegree(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting PAkd in text format: ");
        throw;
    }
}
AnsiString TActuator::getPurposeText(void) const
{
    return AnsiString(purposeToStr(getPurpose()));
}
void TActuator::setPurposeText(const AnsiString &S)
{
    try {
        setPurpose(strToPurpose(S.str));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting Purpose in text format: ");
        throw;
    }
}

AnsiString TActuator::getIdText(void) const
{
    return IntToStr(getId());
}
void TActuator::setIdText(const AnsiString &S)
{
    try {
        setId(StrToInt(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting Id in text format: ");
        throw;
    }
}

AnsiString TActuator::getPendingText(void) const
{
    return BoolToStr(Pending, true);
}
void TActuator::setPendingText(const AnsiString &S)
{
    try {
        Pending = StrToBool(S);
    } catch(Exception& E) {
        E.Message.Insert(1, "setting Pending in text format: ");
        throw;
    }
}

//--------------------------------------------------------------------------
//COTAS ÚTILES EN FORMATO TEXTO:

AnsiString TActuator::getr_minText(void) const
{
    return FloatToStr(getr_min());
}
AnsiString TActuator::getr_safText(void) const
{
    return FloatToStr(getr_saf());
}

AnsiString TActuator::getr_2safText(void) const
{
    return FloatToStr(getr_2saf());
}
AnsiString TActuator::gettheta___2safText(void) const
{
    return FloatToStr(gettheta___2saf());
}
AnsiString TActuator::gettheta___3safText(void) const
{
    return FloatToStr(gettheta___3saf());
}

AnsiString TActuator::gettheta_2radText(void) const
{
    return FloatToStr(gettheta_2rad());
}

//--------------------------------------------------------------------------
//PROPIEDADES DE ÁREA EN FORMATO TEXTO:

AnsiString TActuator::getr_3maxnomText(void) const
{
    return FloatToStr(getr_3maxnom());
}
void TActuator::setr_3maxnomText(AnsiString &S)
{
    try {
        setr_3maxnom(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting r_3maxnom in text format: ");
        throw;
    }
}

AnsiString TActuator::getScText(void) const
{
    return FloatToStr(getSc());
}
AnsiString TActuator::getSwText(void) const
{
    return FloatToStr(getSw());
}
AnsiString TActuator::getSpText(void) const
{
    return FloatToStr(getSp());
}
AnsiString TActuator::getSsText(void) const
{
    return FloatToStr(getSs());
}
AnsiString TActuator::getSeText(void) const
{
    return FloatToStr(getSe());
}
AnsiString TActuator::getReText(void) const
{
    return FloatToStr(getRe());
}

AnsiString TActuator::getSptText(void) const
{
    return FloatToStr(getSpt());
}
AnsiString TActuator::getSetText(void) const
{
    return FloatToStr(getSet());
}
AnsiString TActuator::getRetText(void) const
{
    return FloatToStr(getRet());
}

//---------------------------------------------------------------------------
//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TActuator::getSecurityText(void) const
{
    AnsiString S;

    //PROPIEDADES DE SEGURIDAD:

    S = "SPM components:\r\n";

    S += AnsiString("    SPMrec = ")+getSPMrecText()+AnsiString("\r\n");

    S += AnsiString("    SPMsta = ")+getSPMstaText()+AnsiString("\r\n");
    S += AnsiString("    SPMdyn = ")+getSPMdynText()+AnsiString("\r\n");
    S += AnsiString("    SPMmin = ")+getSPMminText()+AnsiString("\r\n");
    S += AnsiString("    SPMsim = ")+getSPMsimText()+AnsiString("\r\n");
    S += AnsiString("    SPMoff = ")+getSPMoffText()+AnsiString("\r\n");

    S += "SPMS values when PAkd = Pre:\r\n";

    S += AnsiString("    SPMexe_p: ")+getSPMexe_pText()+AnsiString("\r\n");
    S += AnsiString("    SPMvalParPro_p: ")+getSPMvalParPro_pText()+AnsiString("\r\n");
    S += AnsiString("    SPMgenParPro_p: ")+getSPMgenParPro_pText()+AnsiString("\r\n");
    S += AnsiString("    SPMvalPP_p: ")+getSPMvalPP_pText()+AnsiString("\r\n");
    S += AnsiString("    SPMvalDP_p: ")+getSPMvalDP_pText()+AnsiString("\r\n");
    S += AnsiString("    SPMgenPairPPDP_p: ")+getSPMgenPairPPDP_pText()+AnsiString("\r\n");
    S += AnsiString("    SPMall_p: ")+getSPMall_pText()+AnsiString("\r\n");

    S += "SPMS values when PAkd = Apr:\r\n";

    S += AnsiString("    SPMexe_a: ")+getSPMexe_aText()+AnsiString("\r\n");
    S += AnsiString("    SPMvalParPro_a: ")+getSPMvalParPro_aText()+AnsiString("\r\n");
    S += AnsiString("    SPMgenParPro_a: ")+getSPMgenParPro_aText()+AnsiString("\r\n");
    S += AnsiString("    SPMvalPP_a: ")+getSPMvalPP_aText()+AnsiString("\r\n");
    S += AnsiString("    SPMvalDP_a: ")+getSPMvalDP_aText()+AnsiString("\r\n");
    S += AnsiString("    SPMgenPairPPDP_a: ")+getSPMgenPairPPDP_aText()+AnsiString("\r\n");
    S += AnsiString("    SPMall_a: ")+getSPMall_aText();

    return S;
}
AnsiString TActuator::getStatusText(void) const
{
    AnsiString S;

    //PROPIEDADES DE ESTADO:

    S = "R/W:\r\n";

    S += AnsiString("    PAkd = ")+getPAkdText()+AnsiString("\r\n");
    S += AnsiString("    Purpose = ")+getPurposeText()+AnsiString("\r\n");

    S += AnsiString("    Id = ")+IntToStr(getId())+AnsiString("\r\n");
    S += AnsiString("    AdjacentEAs = ")+AdjacentEAs.getIdText()+AnsiString("\r\n");
    S += AnsiString("    AdjacentDmins = ")+AdjacentEAs.getDminText()+AnsiString("\r\n");
    S += AnsiString("    AdjacentDends = ")+AdjacentEAs.getDendText()+AnsiString("\r\n");
    S += AnsiString("    AdjacentRPs = ")+AdjacentRPs.getText()+AnsiString("\r\n");
    S += AnsiString("    AdjacentDmins = ")+AdjacentRPs.getDminText()+AnsiString("\r\n");
    S += AnsiString("    AdjacentDends = ")+AdjacentRPs.getDendText()+AnsiString("\r\n");
    S += AnsiString("    Pending = ")+getPendingText();

    return S;
}
AnsiString TActuator::getLimitsText(void) const
{
    AnsiString S;

    //COTAS ÚTILES:

    S = "R:\r\n";

    S += AnsiString("    r_min: ")+getr_minText()+AnsiString("\r\n");
    S += AnsiString("    r_saf: ")+getr_safText()+AnsiString("\r\n");

    S += AnsiString("    r_2saf: ")+getr_2safText()+AnsiString("\r\n");
    S += AnsiString("    theta___2saf: ")+gettheta___2safText()+AnsiString("\r\n");
    S += AnsiString("    theta___3saf: ")+gettheta___3safText()+AnsiString("\r\n");

    S += AnsiString("    theta_2rad: ")+gettheta_2radText();

    return S;
}
AnsiString TActuator::getAreaText(void) const
{
    AnsiString S;

    //PROPIEDADES DE ÁREA:

    S = "R/W:\r\n";

    S += AnsiString("    r_3maxnom = ")+getr_3maxnomText()+AnsiString(";\r\n");

    S += "R:\r\n";

    S += AnsiString("    Sc: ")+getScText()+AnsiString("\r\n");
    S += AnsiString("    Sw: ")+getSwText()+AnsiString("\r\n");
    S += AnsiString("    Sp: ")+getSpText()+AnsiString("\r\n");
    S += AnsiString("    Ss: ")+getSsText()+AnsiString("\r\n");
    S += AnsiString("    Se: ")+getSeText()+AnsiString("\r\n");
    S += AnsiString("    Re: ")+getReText()+AnsiString("\r\n");

    S += AnsiString("    Spt: ")+getSptText()+AnsiString("\r\n");
    S += AnsiString("    Set: ")+getSetText()+AnsiString("\r\n");
    S += AnsiString("    Ret: ")+getRetText();

    return S;
}

AnsiString TActuator::getAllText(void) const
{
    //TODAS LASPROPIEDADES:

    AnsiString S;

    S += getCilinderText()+AnsiString("\r\n");
    S += AnsiString("Security:\r\n")+StrIndent(getSecurityText())+AnsiString("\r\n");
    S += AnsiString("Status:\r\n")+StrIndent(getStatusText())+AnsiString("\r\n");
    S += AnsiString("Limits:\r\n")+StrIndent(getLimitsText())+AnsiString("\r\n");
    S += AnsiString("Area:\r\n")+StrIndent(getAreaText());

    return S;
}

AnsiString TActuator::getInstanceText(void) const
{
    string str;

    //Sizing:
    str = commentedLine("L01 = "+getL01Text().str, "distance between P0 and P1 (in mm)");

    //OrientationRadians:
    str += "\r\n"+commentedLine("theta_1min = "+gettheta_1minText().str, "position angle’s lower limit of the axis 0-1 respect S1 (in rad)");
    str += "\r\n"+commentedLine("theta_1max = "+gettheta_1maxText().str, "position angle’s upper limit of the axis 0-1 respect S1 (in rad)");
    str += "\r\n"+commentedLine("theta_1 = "+gettheta_1Text().str, "position angle of axis 0-1 respect S1 (in rad)");
    str += "\r\n"+commentedLine("theta_O3o = "+gettheta_O3oText().str, "orientation of S3 respect S1 when theta_1 = 0 (in rad)");

    //Arm:
    str += "\r\n";
    str += "\r\n"+commentedLine("ArmInstance:", "Instance properties of the RP.Actuator.Arm (sizing, orientation and quantification):");
    str += "\r\n"+StrIndent(getArm()->getInstanceText()).str;

    //Others:
    str += "\r\n";
    str += "\r\n"+commentedLine("SPMmin = "+getSPMminText().str, "SPM minimum: is the SPM due to the minimum jump during generation (in mm)");
    str += "\r\n"+commentedLine("SPMsim = "+getSPMsimText().str, "SPM of simulation: is the maximum deviation in the radial trajectory (in mm)");
    str += "\r\n"+commentedLine("PAkd = "+getPAkdText().str, "position angles knowledge degree [Pre | Apr | Unk]");

    return AnsiString(str);
}
void TActuator::setInstanceText(const AnsiString& S)
{
    //inicializa el indice al primer caracter de la cadena
    int i = 1;

    try {
        //lee la instancia en una variable tampón
        TActuator aux(this);
        TActuator *A = &aux;
        readInstance((TActuator*&)A, S, i);

        //busca un texto inesperado
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("string S should contain the instance value only");

        //asigna la variable tampón
        clone(A);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting  in text format: ");
        throw;
    }
}

AnsiString TActuator::getOriginsRowText(void) const
{
    return getIdText()+AnsiString("\t")+getP0().getRowText()+AnsiString("\t")+getthetaO1Text();
}
AnsiString TActuator::getPositionP3RowText(void) const
{
    return getIdText()+AnsiString("\t")+getArm()->getP3().getRowText();
}
AnsiString TActuator::getPositionP_3RowText(void) const
{
    return getIdText()+AnsiString("\t")+getP_3().getRowText();
}
AnsiString TActuator::getPositionPAPRowText(void) const
{
    return getIdText()+AnsiString("\t")+getp_1Text()+AnsiString("\t")+getArm()->getp___3Text();
}

//------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//asigna el SPM adecuado para (PAkd, Purpose)
void TActuator::AssignSPM(void)
{
    //asigna a Arm->SPM o a Barrier->SPM el SPM correspondiente según (PAkd, Purpose)
    switch(getPAkd()) {
    case kdPre: //precise
        switch(getPurpose()) {
        case pAll: getArm()->setSPM(getSPMall_p()); break;
        case pGenPairPPDP: getArm()->setSPM(getSPMgenPairPPDP_p()); break;
        case pValDP: getArm()->setSPM(getSPMvalDP_p()); break;
        case pValPP: getArm()->setSPM(getSPMvalPP_p()); break;
        case pGenParPro: getArm()->setSPM(getSPMgenParPro_p()); break;
        case pValParPro: getArm()->setSPM(getSPMvalParPro_p()); break;
        case pExe: getArm()->setSPM(getSPMexe_p()); break;
        }
        break;;
    case kdApp: //approximated
        switch(getPurpose()) {
        case pAll: getArm()->setSPM(getSPMall_a()); break;
        case pGenPairPPDP: getArm()->setSPM(getSPMgenPairPPDP_a()); break;
        case pValDP: getArm()->setSPM(getSPMvalDP_a()); break;
        case pValPP: getArm()->setSPM(getSPMvalPP_a()); break;
        case pGenParPro: getArm()->setSPM(getSPMgenParPro_a()); break;
        case pValParPro: getArm()->setSPM(getSPMvalParPro_a()); break;
        case pExe: getArm()->setSPM(getSPMexe_a()); break;
        }
        break;
    case kdUnk: //unknowledge
        getBarrier()->setSPM(getSPMsta());
        break;
    }
}

//------------------------------------------------------------------
//MÉTODOS PRIVADOS PARA EL CÁLCULO DE DISTANCIAS CON BARRERAS:

//determina la distancia mínima con una barrera
double TActuator::distanceMin(const TBarrier *Barrier) const
{
    //comprueba las precondiciones
    if(Barrier == NULL)
        throw EImproperArgument("pointer Barrier should point to built barrier");

    double distanceMin;

    //si la posición angular de ambos rotores de este actuador es conocida
    if(getPAkd() != kdUnk)
        //calcula la distancia brazo-barrera
        distanceMin = getArm()->distanceMin(Barrier);
    //si la posición angular de algún rotor de este actuador es desconocida
    else
        //calcula la distancia barrera-barrera
        distanceMin = getBarrier()->distanceMin(Barrier);

    return distanceMin;
}

//determina si hay colisión con una barrera
bool TActuator::thereIsCollision(const TBarrier *Barrier) const
{
    //comprueba las precondiciones
    if(Barrier == NULL)
        throw EImproperArgument("pointer Barrier should point to built barrier");

    //si la posición angular de ambos rotores de este actuador es conocida
    if(getPAkd() != kdUnk)
        //determina el estado de colisión brazo-barrera
        return getArm()->collides(Barrier);
    //si la posición angular de algún rotor de este actuador es desconocida (barrera-barrera)
    else
        //determina el estado de colisión barrera-barrera
        return getBarrier()->collides(Barrier);

    //indica que no ha encontrado colisión
    return false;
}

//------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//compara los identificadores de dos actuadores
int  TActuator::CompareIds(TActuator *A1, TActuator *A2)
{
    //el puntero A1 debería apuntar a un actuador construido
    if(A1 == NULL)
        throw EImproperArgument("pointer A1 should point a Builts actuator");

    //el puntero A2 debería apuntar a un actuador construido
    if(A2 == NULL)
        throw EImproperArgument("pointer A2 should point a Builts actuator");

    if(A1->getId() < A2->getId())
        return -1;
    if(A1->getId() > A2->getId())
        return 1;
    return 0;
}

//imprime el identificador de un actuador
void  TActuator::PrintId(AnsiString &S, TActuator *A)
{
    //el puntero A debería apuntar a un actuador construido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to builts actuator");

    S += A->getIdText();
}

//lee una instancia de actuador en una cadena
void  TActuator::readInstance(TActuator *A,
                              const AnsiString& S, int &i)
{
    //comprueba las precondiciones
    if(A == NULL)
        throw EImproperArgument("pointer A shouldpoint to built actuator");
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    try {
        //variables tampón
        TActuator t_A(A);

        StrTravelSeparatorsIfAny(S, i);
        StrTravelLabel("L01", S, i);
        StrTravelLabel("=", S, i);
        double aux;
        StrReadFloat(aux, S, i);
        t_A.setL01(aux);

        //-------------------------------------

        StrTravelSeparators(S, i);
        StrTravelLabel("theta_1min", S, i);
        StrTravelLabel("=", S, i);
        double theta_1min;
        StrReadFloat(theta_1min, S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("theta_1max", S, i);
        StrTravelLabel("=", S, i);
        double theta_1max;
        StrReadFloat(theta_1max, S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("theta_1", S, i);
        StrTravelLabel("=", S, i);
        double theta_1;
        StrReadFloat(theta_1, S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("theta_O3o", S, i);
        StrTravelLabel("=", S, i);
        double theta_O3o;
        StrReadFloat(theta_O3o, S, i);

        //-------------------------------------

        StrTravelLabel("ArmInstance:", S, i);
        TArm *aux_A = t_A.getArm();
        TArm::readInstance(aux_A, S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("SPMmin", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(aux, S, i);
        t_A.setSPMmin(aux);

        StrTravelSeparators(S, i);
        StrTravelLabel("SPMsim", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(aux, S, i);
        t_A.setSPMsim(aux);

        StrTravelSeparators(S, i);
        StrTravelLabel("PAkd", S, i);
        StrTravelLabel("=", S, i);
        TKnowledgeDegree aux_kd;
        StrReadKnowledgeDegree(aux_kd, S, i);
        t_A.setPAkd(aux_kd);

        //asigna las variables auxiliares a la variable tampón
        t_A.setOrientationRadians(theta_1min, theta_1max, theta_1, theta_O3o);
        //asigna la variable tampón
        A->clone(&t_A);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading actuator instance: ");
        throw;
    }
}

//obtiene las etiquetas de las propiedades de origen de un actuador
//("Id", "x0", "y0", "thetaO1") al final de una cadena de texto
//en formato fila de texto
AnsiString TActuator::getOriginsLabelsRow(void)
{
    return "Id\tx0\ty0\tthetaO1";
}
//atraviesa las etiquetas de las propiedades de origen
//("Id", "x0", "y0", "thetaO1")
//en formato fila de texto
void  TActuator::travelOriginsLabelsRow(const AnsiString& S, int& i)
{
    //check the precondtion
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    try {
        StrTravelLabel("Id", S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("x0", S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("y0", S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("thetaO1", S, i);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "traveling RP origin labels row: ");
        throw;
    }
}

//imprime los valores de las propiedades de orien de un actuador
//(Id, x0, y0, thetaO1) al final de una cadena de texto
//en formato fila de texto
void  TActuator::printOriginsRow(AnsiString& S, TActuator *A)
{
    //check the precondtion
    if(A == NULL)
        throw EImproperArgument("pointer A should bepoint to built actuator");

    S += A->getOriginsRowText();
}
//lee los valores de las propiedades de orien de un actuador
//(Id, x0, y0, thetaO1) desde la posición indicada de una cadena
//de texto, en formato fila de texto
void  TActuator::readOriginsRow(TActuator* &A,
                                const AnsiString& S, int &i)
{
    //comprueba las precondiciones
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    try {
        //tampon variables
        int Id;
        double x0, y0;
        double thetaO1;

        //read the values and the separators
        StrReadInt(Id, S, i);
        StrTravelSeparators(S, i);
        StrReadFloat(x0, S, i);
        StrTravelSeparators(S, i);
        StrReadFloat(y0, S, i);
        StrTravelSeparators(S, i);
        StrReadFloat(thetaO1, S, i);

        //set the tampon variable
        A->setOrigins(Id, x0, y0, thetaO1);

    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading origins values in row text format: ");
        throw;
    }
}

//obtiene las etiquetas de las propiedades de posición de un actuador
//("Id", "x3", "y3") al final de una cadena de texto
//en formato fila de texto
AnsiString TActuator::getPositionP3LabelsRow(void)
{
    return "Id\tx3\ty3";
}
//obtiene las etiquetas de las propiedades de posición de un actuador
//("Id", "x_3", "y_3") al final de una cadena de texto
//en formato fila de texto
AnsiString TActuator::getPositionP_3LabelsRow(void)
{
    return "Id\tx_3\ty_3";
}
//obtiene las etiquetas de las propiedades de posición de un actuador
//("Id", "p_1", "p___3") al final de una cadena de texto
//en formato fila de texto
AnsiString TActuator::getPositionPPALabelsRow(void)
{
    return "Id\tp_1\tp___3";
}
//atraviesa las etiquetas de las propiedades de posición
//("Id", "x3", "y3")
//en formato fila de texto
void  TActuator::travelPositionP3LabelsRow(const AnsiString& S, int& i)
{
    //comprueba las precondiciones
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    try {
        StrTravelLabel("Id", S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("x3", S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("y3", S, i);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "traveling P3 position labels in row text format: ");
        throw;
    }
}

//imprime los valores de las propiedades de posición de un actuador
//(Id, x3, y3) al final de una cadena de texto
//en formato fila de texto
void  TActuator::printPositionP3Row(AnsiString& S, TActuator *A)
{
    //el puntero A debe apuntar a un actuador construido
    if(A == NULL)
        throw EImproperArgument("pointer A should bepoint to built actuator");

    S += A->getPositionP3RowText();
}
//lee los valores de las propiedades de posición de un actuador
//(Id, x3, y3) desde la posición indicada de una cadena
//de texto, en formato fila de texto
void  TActuator::readPositionP3Row(TActuator* &A,
                                   const AnsiString& S, int &i)
{
    //comprueba las precondiciones
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    try {
        //tampon variables
        int Id;
        double x3, y3;

        //read the values and the separators
        StrReadInt(Id, S, i);
        StrTravelSeparators(S, i);
        StrReadFloat(x3, S, i);
        StrTravelSeparators(S, i);
        StrReadFloat(y3, S, i);

        //set thetampon variables
        A->setPositionP3(Id, x3, y3);

    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading position P3 in row text format: ");
        throw;
    }

    //El método de lectura de las posiciones debe asignar el identificador
    //y no comprobar que coincide, ya que la lectura de una lista puede
    //tener desordenados los elementos.
}
//imprime los valores de las propiedades de posición de un actuador
//(Id, x_3, y_3) al final de una cadena de texto
//en formato fila de texto
void  TActuator::printPositionP_3Row(AnsiString& S, TActuator *A)
{
    //comprueba las precondiciones
    if(A == NULL)
        throw EImproperArgument("pointer A should bepoint to built actuator");

    S += A->getPositionP_3RowText();
}
//imprime los valores de las propiedades de posición de un actuador
//(Id, p_1, p___3) al final de una cadena de texto
//en formato fila de texto
void  TActuator::printPositionPPARow(AnsiString& S, TActuator *A)
{
    //el puntero A debe apuntar a un actuador construido
    if(A == NULL)
        throw EImproperArgument("pointer A should bepoint to built actuator");

    S += A->getPositionPAPRowText();
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONSTRUCCIÓN, DESTRUCCIÓN Y COPIA:

//construye un actuador
//con los valores indicados
TActuator::TActuator(int Id, TDoublePoint P0, double thetao_) :
    //construye e inciializa el cilindro
    TCilinder(P0, thetao_),
    //inicializa las propiedades de estado
    p_PAkd(kdPre), p_Purpose(pGenPairPPDP),
    AdjacentEAs(),
    AdjacentRPs(),
    Pending(true), Collision(false)
{
    //el número de identificación Id debe ser no negativo
    if(Id < 0)
        throw EImproperArgument("identification number Id should be nonnegative");

    //ADVERTENCIA: está permitida la duplicidad de números de identificación Id.

    //----------------------------------------
    //inicializa las propiedades de seguridad:

    //initialize the SPM components
    p_SPMrec = (MEGARA_VMAXABS1*getrbs()*getr_max() + MEGARA_VMAXABS2*getArm()->getrbs()*getArm()->getL1V())*MEGARA_Tstop;
    p_SPMsta = MEGARA_Eo*getr_max() + MEGARA_Ep;
    p_SPMdyn = (MEGARA_VMAXABS1*getrbs()*getr_max() + MEGARA_VMAXABS2*getArm()->getrbs()*getArm()->getL1V())*MEGARA_Tshiff +
            MEGARA_SPMadd;
    p_SPMmin = MEGARA_SPMmin;
    p_SPMsim = MEGARA_SPMsim;
    p_SPMoff = MEGARA_PAem*getrmax() + MEGARA_Pem;

    //----------------------------------------
    //inicializa las propiedades de estado

    //inicializa el identificador
    p_Id = Id;

    //----------------------------------------
    //inicializa las cotas útiles

    p_r_min = getr_max();
    p_r_saf = getr_max();
    p_r_2saf = getL02max();
    p_theta___2saf = M_PI;
    p_theta___3saf = M_PI;
    p_theta_2rad = max(0., gettheta_1min()) + M_PI/2;

    //----------------------------------------
    //inicializa las propiedades de área

    setr_3maxnom(getr_3max());
}

//copia las propiedades de seguridad de un actuador
void TActuator::copySecurity(const TActuator *A)
{
    //el puntero A debe apuntar a un posicinador contruido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built fiberpositioner");

    //copia las componentes de SPM
    p_SPMrec = A->p_SPMrec;
    p_SPMsta = A->p_SPMsta;
    p_SPMdyn = A->p_SPMdyn;
    p_SPMmin = A->p_SPMmin;
    p_SPMsim = A->p_SPMsim;
    p_SPMoff = A->p_SPMoff;
}
//copia las propiedades de estado de un actuador
void TActuator::copyStatus(const TActuator *A)
{
    //el puntero A debe apuntar a un actuador contruido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");

    p_PAkd = A->p_PAkd;
    p_Purpose = A->p_Purpose;

    p_Id = A->p_Id;
    AdjacentEAs.Clone(A->AdjacentEAs);
    AdjacentRPs.Clone(A->AdjacentRPs);
    Pending = A->Pending;
    Collision = A->Collision;
}
//copia las propiedades límite de un actuador
void TActuator::copyLimits(const TActuator *A)
{
    //el puntero A debe apuntar a un actuador contruido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");

    //copia las cotas útiles
    p_r_min = A->p_r_min;
    p_r_saf = A->p_r_saf;
    p_r_2saf = A->p_r_2saf;
    p_theta___2saf = A->p_theta___2saf;
    p_theta___3saf = A->p_theta___3saf;
    p_theta_2rad = A->p_theta_2rad;
}
//copia las propiedades de área de un actuador
void TActuator::copyArea(const TActuator *A)
{
    //el puntero A debe apuntar a un actuador contruido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built fiber posicionator");

    //copia las propiedades
    p_r_3maxnom = A->p_r_3maxnom;
    p_Sc = A->p_Sc;
    p_Sw = A->p_Sw;
    p_Sp = A->p_Sp;
    p_Ss = A->p_Ss;
    p_Se = A->p_Se;
    p_Re = A->p_Re;
    p_Spt = A->p_Spt;
    p_Set = A->p_Set;
    p_Ret = A->p_Ret;
}

//copia todas las propiedades de un actuador
void TActuator::clone(const TActuator *A)
{
    //el puntero A debería apuntar a un actuador construido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");

    //copia las propiedades del cilindro
    copyCilinder(A);

    //copia las propiedades de seguridad
    copySecurity(A);
    //copia las propiedades de estado
    copyStatus(A);
    //copia las propiedades límite
    copyLimits(A);
}

//vuilda clone of an actuator
TActuator::TActuator(const TActuator *A) :
    TCilinder(A) //clone the cilinder
{
    //check the precondition
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");

    //copyall other properties
    copySecurity(A);
    copyStatus(A);
    copyLimits(A);
    copyArea(A);
}

//destruye un actuador
TActuator::~TActuator()
{
}

//---------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//calcula las propiedades de seguridad:
//      {r_min, r_saf, r_2saf, theta___2saf, theta___3saf, theta_2rad}
void TActuator::calculateSafeParameters(void)
{
    //si hay actuadores adyacentes
    if(AdjacentRPs.getCount() > 0) {
        //contruye un clon del actuador para realizar las simulaciones
        TActuator A(this);

        //asigna el SPM de generación con grado de conocimiento aproximado
        A.getArm()->setSPM(getSPMgenPairPPDP_a());

        //Nótese que los parámetros de seguridad serán válidos
        //para la programación del movimiento, incluso cuando
        //las posiciones de los rotores sean conocidas de manera
        //aproximada; pero no serán válidos durante las asignaciones,
        //cuando el SPMall que corresponda supere al SPMgen_a.

        //activa la cuantificación de los rotores
        A.enableQuantification();
        //mueve los rotores a sus posiciones de origen
        A.setp_1(0);
        A.getArm()->setp___3(0);
        //deteremina r_min
        p_r_min = A.getArm()->getContour().distanceMax(A.getP0());

        TActuator *AA;
        double r_saf;
        //por cada actuador adyacente
        for(int i=0; i<AdjacentRPs.getCount(); i++) {
            //apunta el actuador indicado para facilitar su acceso
            AA = AdjacentRPs[i].RP->getActuator();

            //calcula la distancia de seguridad
            double aux = Mod(AA->getP0() - A.getP0());
            r_saf = aux - AA->getr_max() - AA->getSPMgenPairPPDP_a() - A.getArm()->getSPM();

            //ADVERTENCIA: el SPM del brazo de los actuadores
            //adyacentes no ha sido modificado, de ahí que se
            //reste AA->SPMgen_a en vez de AA->Arm->SPM.

            //actualiza r_saf
            if(r_saf < getr_saf())
                p_r_saf = r_saf;

            //Nótese que r_saf podría salir negativo.
        }

        //Busca la posición del rotor 2 a partir de la cual
        //el contorno no invade el espacio más allá de r_saf:

        //como la evaluación es computacionalmente costosa
        //conviene hacer una búsqueda binaria en (0, M_PI)

        //desactiva la cuantificación
        A.disableQuantification();

        //determina los límites del intervalo de búsqueda
        double theta___2min = M_PI - A.gettheta_O3o() + max(0., A.getArm()->gettheta___2min());
        double theta___2max = M_PI - A.gettheta_O3o() + min(M_PI, A.getArm()->gettheta___2max());

        //Nótese que aunque las coordenadas angulares se refieren a P2,
        //están dadas en S3, razón por la cual, debe sumarse M_PI - theta_O3o,
        //para que el brazo quede alineado con el eje 1-0.

        //calcula el punto medio del intervalo de búsqueda
        double theta___2 = (theta___2max + theta___2min)/2;
        //mueve el rotor 2 al punto medio del intervalo de búsqueda
        A.getArm()->settheta___2(theta___2);
        //calcula el radio de la envolvente descrita por el contorno del brazo
        double r_max = A.getArm()->getContour().distanceMax(A.getP0());
        //realiza iteraciones mientras sean necesarias
        //o hasta que elcontador marque el numero de bits
        //de la mantisa de un tipo double (54 bits) más 4.
        int j=0;
        while(fabs(r_max-getr_saf())>ERR_NUM && j<58) {

            if(r_max > getr_saf()) //si se sale por exceso
                theta___2max = theta___2; //retrae la cota superior
            else if(r_max < getr_saf()) //si se sale pordefecto
                theta___2min = theta___2; //retrae la cota inferior

            //si el error cometido supera el error numérico
            if(fabs(r_max-getr_saf()) > ERR_NUM) {
                //calcula el punto medio del intervalo de búsqueda
                theta___2 = (theta___2max + theta___2min)/2;
                //mueve el rotor 2 al punto medio del intervalo de búsqueda
                A.getArm()->settheta___2(theta___2);
                //calcula la distancia máxima
                r_max = A.getArm()->getContour().distanceMax(A.getP0());
            }

            j++; //contabiliza la iteración
        }

        //Nótese que se realizan 4 iteraciones adicionales
        //paracompensar el hecho de que el intervalo
        //contiene M_PI unidades de double.

        //registra las corodenadas angulares
        p_r_2saf = A.getr_2();
        p_theta___3saf = p_theta___2saf = A.getArm()->gettheta___2();
    }

    //si no hay actuadores adyacentes
    else {
        //restaura el valor por defecto de los parámetros de seguridad
        p_r_min = getr_max();
        p_r_saf = getr_max();
        p_r_2saf = getr_2();
        p_theta___2saf = M_PI;
        p_theta___3saf = M_PI;
    }
    p_theta_2rad = max(0., gettheta_1min()) + M_PI/2;
}

//-------------------------------------------------------------------
//MÉTODOS DE ASIGNACIÓN CONJUNTA:

//asigna las propiedades angulares (theta_1min, theta_1max,theta_1)
void TActuator::setOrientationRadians(double theta_1min, double theta_1max,
                                      double theta_1, double theta_O3o)
{
    //el ángulo theta_1min debe estar en el dominio de F(theta_1)
    if(!getF().BelongToDomain(theta_1min))
        throw EImproperArgument("angle theta_1min should be in domain F(theta_1)");

    //el ángulo theta_1max debe estar en el domaxio de F(theta_1)
    if(!getF().BelongToDomain(theta_1max))
        throw EImproperArgument("angle theta_1max should be in domain F(theta_1)");

    //el ángulo theta_1min no debe ser mayor que el ángulo theta_1max
    if(theta_1min > theta_1max)
        throw EImproperArgument("angle theta_1min should not be upper than angle theta_1max");

    //el ángulo theta_1 debe estar en su dominio [theta_1min, theta_1max]
    if(isntInDomaintheta_1(theta_1))
        throw EImproperArgument("angle theta_1 should be in his domain [theta_1min, theta_1max]");

    //PROCESA [theta_1min, theta_1max]:

    //asignalos nuevos límites del intervalo
    p_theta_1min = theta_1min;
    p_theta_1max = theta_1max;

    //configura la función de cuantificación
    p_Q.Set(1, getp_1min(), getp_1max());

    //PROCESA (theta_1, theta_O3):

    //si la cuantificación está activada
    if(getQuantify_())
        //cuantifica el nuevo valor
        theta_1 = Qtheta_1(theta_1);

    //ADVERTENCIA: la cuantificación podría dar lugar a que theta_1
    //quedara ligeramente fuera de su dominio.

    //constriñe theta_1 a su dominio
    if(theta_1 < gettheta_1min())
        theta_1 = gettheta_1min();
    else if(theta_1 > gettheta_1max())
        theta_1 = gettheta_1max();

    //asigna los nuevos valores
    p_theta_1 = theta_1;
    p_theta_O3o = theta_O3o;

    //cambiar la orientación del posicinador
    //puede modificar la posición y orientación
    //del origen de coordenadas del brazo

    //asimila (theta_1, theta_O3o)
    getArm()->set(newP1(), getthetaO3());
    getBarrier()->set(getP0(), getthetaO1());
}
//asigna las propiedades de cuantificación
void TActuator::setQuantification(double SB1)
{
    //DEFINE LA FUNCIÓN F A PARTIR DE SB1:

    //inicializa la función
    p_F.Clear();
    //añade los puntos de la función de compresión
    p_F.Add(-M_2PI, -double(SB1));
    p_F.Add(0., 0.);
    p_F.Add(M_2PI, double(SB1));
    p_F.Add(double(2*M_2PI), double(2*SB1));
    //elimina las incongruencias del formato texto
    //de la función de compansión
    p_F.setPointsText(getF().getPointsText());

    //asimila F
    processateF();
    settheta_1(gettheta_1());
}

//asigna las propiedades de origen
void TActuator::setOrigins(int Id, double x0, double y0, double thetaO1)
{
    //el número de identificación Id debe ser mayor que cero
    if(Id <= 0)
        throw EImproperArgument("identificator number Idshould beupper zero");

    //asigna los nuevos valores
    p_Id = Id;
    p_P0.x = x0;
    p_P0.y = y0;
    p_thetaO1 = thetaO1;

    //desplazar el actuador cambiará la ubicación del brazo
    //pero no su orientación

    //cambiar la orientación del posicinador
    //modifica la posición y orientación del origen de coordenadas del brazo

    //asimila (P0, thetaO1)
    getArm()->set(newP1(), getthetaO3());
    getBarrier()->set(getP0(), getthetaO1());
}
//asigna las propiedades de posición
//si el punto (x3, y3) no está en el dominio del actuador
//      lanza EImproperArgument
void TActuator::setPositionP3(double x3, double y3)
{
    TDoublePoint P_3;
    double r_3, theta_3;
    double theta_1, theta___3;

    //traduce a coordenadas relativas a S1
    P_3 = S0recToS1rec(x3, y3);
    //traduce a coordenadas polares en S1
    r_3 = Mod(P_3.x, P_3.y);
    if(P_3.x!=0 || P_3.y!=0)
        theta_3 = ArgPos(P_3.x, P_3.y);
    else
        theta_3 = this->gettheta_3();

    //el punto a asignar debería estar en el dominio del actuador
    if(!anglesToGoP_3(theta_1, theta___3, r_3, theta_3))
        throw EImproperFileLoadedValue("point to assign should be in the domine of the fiber positioner");

    //asigna el punto
    setAnglesRadians(theta_1, theta___3);
}
//assign a pair of position angles in radians
//if the position angle theta_1 isn't in domain of theta_1,
//or the position angle theta___3 isn't in domain of theta___3
//  throw an exception EImproperArgument
void TActuator::setPositionPPARadians(double theta_1, double theta___3)
{
    //the position angle theta_1 should be in the domain of theta_1
    //and position angle theta___3 should be in the domain of theta___3
    if(isntInDomaintheta_1(theta_1) || getArm()->isntInDomaintheta___3(theta___3))
        throw EImproperArgument("the position angle theta_1 should be in the domain of theta_1, and position angle theta___3 should be in the domain of theta___3");

    //assign the position angles
    setAnglesRadians(theta_1, theta___3);
}
//assign a pair of position angles in steps
//if the position angle p_1 isn't in domain of p_1,
//or the position angle p___3 isn't in domain of p___3
//  throw an exception EImproperArgument
void TActuator::setPositionPPASteps(double p_1, double p___3)
{
    //the position angle p_1 should be in the rotor 1 domain
    //and the position angle p___3 should be in the rotor 2 domain
    if(isntInDomainp_1(p_1) || getArm()->isntInDomainp___3(p___3))
        throw EImproperArgument("the position angle p_1 should be in the domain of p_1, and position angle p___3 should be in the domain of p___3");

    //assign the position angles
    setAnglesSteps(p_1, p___3);
}
//asigna las propiedades de identificación y posición
//si el punto (x3, y3) no está en el dominio del actuador
//      lanza EImproperArgument
void TActuator::setPositionP3(int Id, double x3, double y3)
{
    //el número de identificación Id debe ser no negativo
    if(Id < 0)
        throw EImproperArgument("number identifier Id should be nonnegative");

    //ADVERTENCIA: está permitida la duplicidad de números de identificación Id.

    TDoublePoint P_3;
    double r_3, theta_3;
    double theta_1, theta___3;

    //traduce a coordenadas relativas a S1
    P_3 = S0recToS1rec(x3, y3);
    //traduce a coordenadas polares en S1
    r_3 = Mod(P_3.x, P_3.y);
    if(P_3.x!=0 || P_3.y!=0)
        theta_3 = ArgPos(P_3.x, P_3.y);
    else
        theta_3 = this->gettheta_3();

    //comprueba la precondicion
    if(!anglesToGoP_3(theta_1, theta___3, r_3, theta_3))
        throw EImproperFileLoadedValue(AnsiString("point to assign should be in the domine of the RP ")+IntToStr(Id));

    //asigna el identificador
    p_Id = Id;
    //asigna el punto
    setAnglesRadians(theta_1, theta___3);
}

//asigna las componentes del SPM
void TActuator::setSPMcomponents(double SPMrec, double SPMsta, double SPMdyn)
{
    //el SPM de recuperación (SPMrec) debe ser no negativo
    if(SPMrec < 0)
        throw EImproperArgument("the recovery SPM (SPMrec) should be nonnegative");

    //el SPM estático (SPMsta) debe ser no negativo
    if(SPMsta < 0)
        throw EImproperArgument("the static SPM (SPMsta) should be nonnegative");

    //el SPM dynánico (SPMdyn) debe ser no negativo
    if(SPMdyn < 0)
        throw EImproperArgument("the dynamic SPM (SPMdyn) should be nonnegative");

    p_SPMrec = SPMrec; //asigna el nuevo valor
    p_SPMsta = SPMsta; //asigna el nuevo valor
    p_SPMdyn = SPMdyn; //asigna el nuevo valor

    //SPMmin, SPMsim and SPMall not shall be assigned here because are independents values.

    //asimila las componentes de SPM
    AssignSPM();
}

//asigna las propiedades de estado (PAkd, Purpose)
void TActuator::setStatus(TKnowledgeDegree PAkd, TPurpose Purpose)
{
    //assign the values
    p_PAkd = PAkd;
    p_Purpose = Purpose;

    //assimilates the values
    AssignSPM();
}

//-------------------------------------------------------------------
//MÉTODOS PARA ALMACENAR Y RECUPERAR POSICIONES DE LOS ROTORES:

//apila las posiciones de los rotores
void TActuator::pushthetas(void)
{
    theta_1s.Add(gettheta_1());
    getArm()->pushtheta___3();
}
//restaura las últimas posiciones apuiladas de los rotores
//si no hay una posisición almacenada para algún rotor:
//      lanza EImproperCall
void TActuator::restorethetas(void)
{
    //la pila theta_1s debería contener algún valor apilado
    if(theta_1s.getCount() < 1)
        throw EImproperCall("stack theta_1s should contain some stacking value");
    //la pila Arm->theta___3s debería contener algún valor apilado
    if(getArm()->theta___3s.getCount() < 1)
        throw EImproperCall("stack Arm->theta___3s should contain some stacking value");

    //asigna el último valor apilado
    settheta_1(theta_1s.getLast());
    getArm()->restoretheta___3();
}
//determines if:
//the rotors of the RP not coincide with the last stacked positions
bool TActuator::thetasNotCoincideWithStacked(void)
{
    if(theta_1s.getCount() < 1)
        throw EImproperCall("should be stacked a theta_1 value");

    if(getArm()->theta___3s.getCount() < 1)
        throw EImproperCall("should be stacked a theta___3 value");

    if(gettheta_1() != theta_1s.getLast())
        return true;

    if(getArm()->gettheta___3() != getArm()->theta___3s.getLast())
        return true;

    return false;
}

//desempila las últimas posiciones apiladas de los rotores
//si no hay una posisición almacenada para algún rotor:
//      lanza EImproperCall
void TActuator::popthetas(void)
{
    //la pila theta_1s debería contener algún valor apilado
    if(theta_1s.getCount() < 1)
        throw EImproperCall("stack theta_1s should contain some stacking value");
    //la pila Arm->theta___3s debería contener algún valor apilado
    if(getArm()->theta___3s.getCount() < 1)
        throw EImproperCall("stack Arm->theta___3s should contain some stacking value");

    //borra el último valor apilado
    theta_1s.Delete(theta_1s.getCount() - 1);
    getArm()->poptheta___3();
}
//restaura y desempila las últimas posiciones apiladas de los rotores
//si no hay una posisición almacenada para algún rotor:
//      lanza EImproperCall
void TActuator::restoreAndPopthetas(void)
{
    //la pila theta_1s debería contener algún valor apilado
    if(theta_1s.getCount() < 1)
        throw EImproperCall("stack theta_1s should contain some stacking value");
    //la pila Arm->theta___3s debería contener algún valor apilado
    if(getArm()->theta___3s.getCount() < 1)
        throw EImproperCall("stack Arm->theta___3s should contain some stacking value");

    //asigna el último valor apilado
    settheta_1(theta_1s.getLast());
    getArm()->restoretheta___3();

    //borra el último valor apilado
    theta_1s.Delete(theta_1s.getCount() - 1);
    getArm()->poptheta___3();
}

//ADVERTENCIA: la recuperación de posiciones no estables,
//cuando está la cuantificación activada, dará lugar
//al desplazamiento a la posición estable más próxima de cada rotor.

//-------------------------------------------------------------------
//MÉTODOS PARA ALMACENAR Y RECUPERAR ESTADOS DE CUNATIFICACIÓN:

//apila el estado de los cuantificadores
void TActuator::pushQuantifys(void)
{
    Quantify_s.Add(getQuantify_());
    getArm()->pushQuantify___();
}
//restaura el último estado apilado de los cuantificadores
//si no hay un estado almacenado para algún quantificador:
//      lanza EImproperCall
void TActuator::restoreQuantifys(void)
{
    //la pila Quantify_s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Quantify_s should contain some stacking value");
    //la pila Arm->Quantify___s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Arm->Quantify___s should contain some stacking value");

    //asigna el último valor apilado
    setQuantify_(Quantify_s.getLast());
    getArm()->restoreQuantify___();
}
//desempila el último estado apilado de los cuantificadores
//si no hay un estado almacenado para algún quantificador:
//      lanza EImproperCall
void TActuator::popQuantifys(void)
{
    //la pila Quantify_s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Quantify_s should contain some stacking value");
    //la pila Arm->Quantify___s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Arm->Quantify___s should contain some stacking value");

    //borra el último valor apilado
    Quantify_s.Delete(Quantify_s.getCount() - 1);
    getArm()->popQuantify___();
}
//restaura y desempila el último estado apilado de los cuantificadores
//si no hay un estado almacenado para algún quantificador:
//      lanza EImproperCall
void TActuator::restoreAndPopQuantifys(void)
{
    //la pila Quantify_s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Quantify_s should contain some stacking value");
    //la pila Arm->Quantify___s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Arm->Quantify___s should contain some stacking value");

    //asigna el último valor apilado
    setQuantify_(Quantify_s.getLast());
    getArm()->restoreQuantify___();

    //borra el último valor apilado
    Quantify_s.Delete(Quantify_s.getCount() - 1);
    getArm()->popQuantify___();
}

//--------------------------------------------------------------------------
//METHODS TO DETERMINE THE RELATIVE POSITION OF THE ACTUATOR:

//determina si un ángulo del rotor 2 en radianes
//está fuera del área de seguridad
bool TActuator::theta___3IsOutSafeArea(double theta___3) const
{
    if(theta___3 > gettheta___3saf())
        return true;

    return false;
}
//determina si un ángulo del rotor 2 en radianes
//está dentro del área de seguridad
bool TActuator::theta___3IsInSafeArea(double theta___3) const
{
    if(theta___3 <= gettheta___3saf())
        return true;

    return false;
}

//determina si un ángulo del rotor 2 en pasos
//está fuera del área de seguridad
bool TActuator::p___3IsOutSafeArea(double p___3) const
{
    if(p___3 > getArm()->getF().Image(gettheta___3saf()))
        return true;

    return false;
}
//determina si un ángulo del rotor 2 en pasos
//está dentro del área de seguridad
bool TActuator::p___3IsInSafeArea(double p___3) const
{
    if(p___3 <= getArm()->getF().Image(gettheta___3saf()))
        return true;

    return false;
}

//determina si un punto está fuera del área de seguridad
bool TActuator::P3IsOutSafeArea(TDoublePoint P) const
{
    //calcula las posiciones angulares para ir al punto
    double theta_1, theta___3;
    if(!anglesToGoP3(theta_1, theta___3, P.x, P.y))
        return true;

    //determina si theta___3 está fuera del área de seguridad
    return theta___3IsOutSafeArea(theta___3);
}
//determina si un punto está dentro del área de seguridad
bool TActuator::P3IsInSafeArea(TDoublePoint P) const
{
    //calcula las posiciones angulares para ir al punto
    double theta_1, theta___3;
    if(!anglesToGoP3(theta_1, theta___3, P.x, P.y))
        return false;

    //determina si theta___3 está fuera del área de seguridad
    return theta___3IsInSafeArea(theta___3);
}

//determina si el brazo está fuera del área deseguridad
bool TActuator::ArmIsOutSafeArea(void) const
{
    if(getArm()->gettheta___2() > gettheta___2saf())
        return true;
    else
        return false;
}
//determina si el brazo está dentro del área de seguridad
bool TActuator::ArmIsInSafeArea(void) const
{
    if(getArm()->gettheta___2() <= gettheta___2saf())
        return true;
    else
        return false;
}

//determines if any rotor of the actuator is out the origin
bool TActuator::isOutTheOrigin(void) const
{
    //determines the origin of each rotor
    double p_1origin = max(0., ceil(getp_1min()));
    double p___3origin = max(0., ceil(getArm()->getp___3min()));

    //if the rotor 1 is out the origin, indicates that the actuator is oit the origin
    if(round(getp_1()) != p_1origin)
        return true;

    //if the rotor 2 is out the origin, indicates that the actuator is oit the origin
    if(round(getArm()->getp___3()) != p___3origin)
        return true;

    return false;
}

//PUNTO DE INFLEXIÓN PARA UN ÁNGULO DEL ROTOR 1:

//Coordenada radial del punto de inflexión para la pose dada en S1.
//Pi es el punto de la trayectoria radial de P2 en que:
//      theta_1 == max(theta_1min, 0).
double TActuator::getr_i(double &theta_1, double theta_2)
{
    //la distancia L01 debe ser igual a la distancia L12
    if(getL01() != getArm()->getL12())
        throw EImproperCall("distance L01 should be equal distance Arm->L12");

    //el ángulo theta_2 debe estar en [max(0, theta_1min), min(M_2PI, theta_1max)+M_PI/2]
    if(theta_2<max(0., gettheta_1min()) || min(M_2PI, gettheta_1max())+M_PI/2<theta_2)
        throw EImproperArgument("angle theta_2 should be in [max(0, theta_1min), min(M_2PI, theta_1max)+M_PI/2]");

    //si el punto de inflexión está fuera de la zona segura
    if(theta_2 < gettheta_2rad()) {
        //el rotor 1 se retraerá hasta cero
        theta_1 = 0;

        //calcula el radicando de la solución simplificada
        double aux = getL01()*cos(theta_2);
        double rad = aux*aux - getL01()*getL01() + getArm()->getL12()*getArm()->getL12();

        //corrige el posible error numérico en rad
        if(rad < 0)
            rad = 0;

        //calcula r_i
        double r_i = -aux + sqrt(rad);

        //corrige el posible error numérico en r_i
        if(r_i < 0)
            r_i = 0;

        //devuelve r_i
        return r_i;
    } else { //si el punto de inflexión está en la zona segura
        //el rotor 1 se retraerá hasta la dirección noramal a la radial
        theta_1 = theta_2 - M_PI/2;

        //devuelve r_i
        return fabs(getL01() - getArm()->getL12());
    }
}
//ángulo del rotor 2 respecto de S2 en radianes
//para el punto de inflexión de la pose dada
double TActuator::gettheta___2i(double theta_2, double r_i)
{
    //la distancia r_i debe ser no negativa
    if(r_i < 0)
        throw EImproperArgument("distance r_i should be nonnegative");

    //el ángulo theta_2 debe estar en [max(0, theta_1min), min(M_2PI, theta_1max)+M_PI/2]
    if(theta_2<max(0., gettheta_1min()) || min(M_2PI, gettheta_1max())+M_PI/2<theta_2)
        throw EImproperArgument("angle theta_2 should be in [max(0, theta_1min), min(M_2PI, theta_1max)+M_PI/2]");

    //calcula el argumento de la función coseno
    double arg = (getL01()*getL01() + getArm()->getL12()*getArm()->getL12() - r_i*r_i)/(2*getL01()*getArm()->getL12());

    //corrige el posible error numérico
    if(arg < -1)
        arg = -1;
    else if(arg > 1)
        arg = 1;

    //calcula y devuelve el ángulo en radianes
    return acos(arg);
}

//MÉTODOS DE MOVIMIENTO:

//mueve el brazo hasta a la posición de seguridad estable más próxima
void TActuator::moveArmToSafePosition(void)
{
    if(getArm()->getQuantify___())
        getArm()->settheta___2FirstStableLessOrEqual(gettheta___2saf());
    else
        getArm()->settheta___2(gettheta___2saf());
}

//--------------------------------------------------------------------------
//MÉTODOS PARA EL CÁLCULO DE DISTANCIAS:

//determina la distancia con otro actuador
double TActuator::distance(TActuator *AA)
{
    //el puntero AA debería apuntar a un actuador construido
    if(AA == NULL)
        throw EImproperArgument("pointer AA should point to built actuator");

    //calcula la distancia entre el contorno del obstáculo
    //de este actuador y el del actuador adyacente:

    double D1; //distancia de este al adyacente
    double D2; //distancia del adyacente a este

    //si la posición angular de ambos rotores de este actuador es conocida
    if(getPAkd() != kdUnk) {
        //si la posición angular de ambos rotores del actuador adyacente es conocida
        if(AA->getPAkd() != kdUnk) {
            //calcula la distancia entre contornos
            //this->Arm -> AA->Arm
            D1 = getArm()->getContour().distanceMin(AA->getArm()->getContour());
            D2 = AA->getArm()->getContour().distanceMin(getArm()->getContour());
        }
        //si la posición angular de algún eje del actuador adyacente es desconocida
        else {
            //calcula la distancia entre contornos
            //this->Arm -> AA->Barrier
            D1 = getArm()->getContour().distanceMin(AA->getBarrier()->getContour());
            D2 = AA->getBarrier()->getContour().distanceMin(getArm()->getContour());
        }
    }
    //si la posición angular de algún eje de este actuador es desconocida
    else {
        //si la posición angular de ambos rotores del actuador adyacente es conocida
        if(AA->getPAkd() != kdUnk) {
            //calcula la distancia entre contornos
            //this->Barrier -> AA->Arm
            D1 = getBarrier()->getContour().distanceMin(AA->getArm()->getContour());
            D2 = AA->getArm()->getContour().distanceMin(getBarrier()->getContour());
        }
        //si la posición angular de algún eje del actuador adyacente es desconocida
        else {
            //calcula ladistancia entre contornos
            //this->Barrier -> AA->Barrier
            D1 = getBarrier()->getContour().distanceMin(AA->getBarrier()->getContour());
            D2 = AA->getBarrier()->getContour().distanceMin(getBarrier()->getContour());
        }
    }

    return min(D1, D2); //devuelve la distancia mínima

    //tomar la distancia mínima constituye una corrección del error numérico
}
//determine the distance with a point
double TActuator::distance(TDoublePoint P)
{
    //calcula la distancia entre el contorno del obstáculo
    //de este actuador y el del actuador adyacente:

    double D; //distancia de este al punto

    //si la posición angular de ambos rotores de este actuador es conocida
    if(getPAkd() != kdUnk) {
        //calcula la distancia entre contornos
        //this->Arm -> AA->Arm
        D = getArm()->getContour().distanceMin(P);
    }
    //si la posición angular de algún eje de este actuador es desconocida
    else {
        //calcula la distancia entre contornos
        //this->Barrier -> AA->Arm
        D = getBarrier()->getContour().distanceMin(P);
    }

    return D; //devuelve la distancia

    //tomar la distancia mínima constituye una corrección del error numérico
}
//determina la distancia libre con otro actuador
//      Df = D - SPM1 - SPM2
//donde:
//      Df: distancia libre
//      SPM1: SPM de este actuador (según PAkd)
//      SPM2: SPM del otro actuador (según PAkd)
double TActuator::distanceFree(TActuator *A)
{
    //el puntero A debería apuntar a un actuador construido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");

    //caulcula la distancia libre
    double Df = distance(A) - getArm()->getSPM() - A->getArm()->getSPM();

    return Df; //devuelve la distancia libre
}

//determina la distancia mínima entre el brazo del actuador
//y los brazos de los actuadores adyacentes
double TActuator::distanceWithAdjacent(void)
{
    double dmin = DBL_MAX;

    //por cada EA adyacente
    for(int i=0; i<AdjacentEAs.getCount(); i++) {
        //apunta el EA indicado para facilitar su acceso
        TBarrier *AB = &(AdjacentEAs[i].EA->Barrier);
        //determina la distancia entre el brazo y la barrera
        double d = getArm()->getContour().distanceMin(AB->getContour());
        //si la distancia es menor que la mínima
        if(d < dmin)
            dmin = d; //actualiza la distancia mínima
    }

    //por cada RP adyacente
    for(int i=0; i<AdjacentRPs.getCount(); i++) {
        //apunta el RP indicado para facilitar su acceso
        TActuator *AA = AdjacentRPs[i].RP->getActuator();
        //determina la distancia entre brazos
        double d = getArm()->getContour().distanceMin(AA->getArm()->getContour());
        //si la distancia es menor que la mínima
        if(d < dmin)
            dmin = d; //actualiza la distancia mínima
    }

    return dmin;
}
//determina la distancia mínima entre el punto P3 del actuador
//y los puntos P3 de los actuadores adyacentes
double TActuator::distanceP3WithAdjacent(void)
{
    TActuator *AA;
    double dmin = DBL_MAX;
    double d;

    //por cada RP adyacente
    for(int i=0; i<AdjacentRPs.getCount(); i++) {
        //apunta el actuador indicado para facilitar su acceso
        AA = AdjacentRPs[i].RP->getActuator();
        //determina la distancia entre puntos P3
        d = Mod(AA->getArm()->getP3() - getArm()->getP3());
        //si la distancia es menor que la mínima
        if(d < dmin)
            dmin = d; //actualiza la distancia mínima
    }

    return dmin;
}

//DETERMINACIÓN DE POSICIÓN RELATIVA:

//determines if a point is in the security area
bool TActuator::pointIsInSecurityArea(TDoublePoint P)
{

    //determines the position angles for position the point P3
    //in the point
    double theta_1, theta___3;
    bool is_in_domain = anglesToGoP3(theta_1, theta___3, P.x, P.y);

    //if the point is in the domain
    if(is_in_domain) {
        //determines the nearest stable position for position the point P3
        //ascloserasposssible the point P
        double p_1nsp, p___3nsp;
        getNearestStablePosition(p_1nsp, p___3nsp, theta_1, theta___3);

        //determine is the position angle of rotor 2 is in the security area
        //andindicates if the point is in se security area
        if(p___3IsInSafeArea(p___3nsp))
            return true;
    }
    //indicates that the point is out the security area
    return false;
}

//determina si un punto está fuera del área invasiva
bool TActuator::P3IsOutNoninvasiveArea(TDoublePoint P)
{
    //traduce el punto a coordenadas relativas a S1
    TDoublePoint P_ = S0recToS1rec(P.x, P.y);

    //si el punto está fuera del dominio
    if(pointIsOutDomainP_3(P_))
        //indica que el punto está fuera del área no invasiva
        return true;

    //guarda la configuración original
    double p_1bak = getp_1();
    double p___3bak = getArm()->getp___3();
    bool Quantify_bak = getQuantify_();
    bool Quantify___bak = getArm()->getQuantify___();

    //desactiva la cuantificación de los rotores
    setQuantify_(false);
    getArm()->setQuantify___(false);

    //mueve el brazo al punto
    setCartesianP_3(P_.x, P_.y);

    //por cada EA adyacente
    for(int i=0; i<AdjacentEAs.getCount(); i++) {
        //apunta la barrera indicada para facilitar su acceso
        TBarrier *AB = &(AdjacentEAs[i].EA->Barrier);

        //determina la distancia mínima entre el brazo y
        //la barrera adyacente
        double d = getArm()->getContour().distanceMin(AB->getContour());

        //si la distancia es menor que el SPM + AB->SPM
        if(d < getArm()->getSPM() + AB->getSPM()) {
            //restaura la configuración original
            setQuantify_(Quantify_bak);
            getArm()->setQuantify___(Quantify___bak);
            setp_1(p_1bak);
            getArm()->setp___3(p___3bak);

            //indica que el punto está fuera del área no invasiva
            return true;
        }
    }

    //por cada RP adyacente
    for(int i=0; i<AdjacentRPs.getCount(); i++) {
        //apunta el actuador indicado para facilitar su acceso
        TActuator *AA = AdjacentRPs[i].RP->getActuator();

        //determina la distancia mínima entre el brazo y
        //el centro del actuador adyacente
        double d = getArm()->getContour().distanceMin(AA->getP0());

        //si la distancia es menor que el SPM + AA->SPM + AA->r_max
        if(d < getArm()->getSPM() + AA->getArm()->getSPM() + AA->getr_max()) {
            //restaura la configuración original
            setQuantify_(Quantify_bak);
            getArm()->setQuantify___(Quantify___bak);
            setp_1(p_1bak);
            getArm()->setp___3(p___3bak);

            //indica que el punto está fuera del área no invasiva
            return true;
        }
    }

    //restaura la configuración original
    setQuantify_(Quantify_bak);
    getArm()->setQuantify___(Quantify___bak);
    setp_1(p_1bak);
    getArm()->setp___3(p___3bak);

    //indica que el punto no está fuera del área no invasiva
    return false;
}
//determina si un punto está dentro del área invasiva
bool TActuator::P3IsInNoninvasiveArea(TDoublePoint P)
{
    //traduce el punto a coordenadas relativas a S1
    TDoublePoint P_ = S0recToS1rec(P.x, P.y);

    //si el punto está fuera del dominio
    if(pointIsOutDomainP_3(P_))
        //indica que el punto está fuera del área no invasiva
        return false;

    //guarda la configuración original
    double p_1bak = getp_1();
    double p___3bak = getArm()->getp___3();
    bool Quantify_bak = getQuantify_();
    bool Quantify___bak = getArm()->getQuantify___();

    //desactiva la cuantificación de los rotores
    setQuantify_(false);
    getArm()->setQuantify___(false);

    //mueve el brazo al punto
    setCartesianP_3(P_.x, P_.y);

    //por cada EA adyacente
    for(int i=0; i<AdjacentEAs.getCount(); i++) {
        //apunta la barrera indicada para facilitar su acceso
        TBarrier *AB = &(AdjacentEAs[i].EA->Barrier);

        //determina la distancia mínima entre el brazo y
        //la barrera adyacente
        double d = getArm()->getContour().distanceMin(AB->getContour());

        //si la distancia es menor que el SPM + AB->SPM
        if(d < getArm()->getSPM() + AB->getSPM()) {
            //restaura la configuración original
            setQuantify_(Quantify_bak);
            getArm()->setQuantify___(Quantify___bak);
            setp_1(p_1bak);
            getArm()->setp___3(p___3bak);

            //indica que el punto está fuera del área no invasiva
            return false;
        }
    }

    //por cada RP adyacente
    for(int i=0; i<AdjacentRPs.getCount(); i++) {
        //apunta el actuador indicado para facilitar su acceso
        TActuator *AA = AdjacentRPs[i].RP->getActuator();

        //determina la distancia mínima entre el brazo y
        //el centro del actuador adyacente
        double d = getArm()->getContour().distanceMin(AA->getP0());

        //si la distancia es menor que el SPM + AA->SPM + AA->r_max
        if(d < getArm()->getSPM() + AA->getArm()->getSPM() + AA->getr_max()) {
            //restaura la configuración original
            setQuantify_(Quantify_bak);
            getArm()->setQuantify___(Quantify___bak);
            setp_1(p_1bak);
            getArm()->setp___3(p___3bak);

            //indica que el punto está fuera del área no invasiva
            return false;
        }
    }

    //restaura la configuración original
    setQuantify_(Quantify_bak);
    getArm()->setQuantify___(Quantify___bak);
    setp_1(p_1bak);
    getArm()->setp___3(p___3bak);

    //indica que el punto no está fuera del área no invasiva
    return true;
}

//deterines if this actuator is invading the maneuvering domain
//of other actuator A
bool TActuator::notInvadeManeuveringDomain(const TActuator *A) const
{
    if(A == NULL)
        throw EImproperArgument("pointer A shouldpointto built actuator");

    double D; //distance netween conturs
    double Dfree; //freedistance between contours

    switch(getPAkd()) {
    case kdPre:
        //caulculate the distance between the contour of the arm
        //and the limit of maneuvering domain of the other actuator
        D = getArm()->getContour().distanceMin(A->getP0()) - A->getr_max();

        //calculate the free distance
        Dfree = D - getSPMgenPairPPDP_p() - A->getSPMsta();

        if(Dfree >= 0)
            return true;
        else
            return false;

        break;

    case kdApp:
        //caulculate the distance between the contour of the arm
        //and the limit of maneuvering domain of the other actuator
        D = getArm()->getContour().distanceMin(A->getP0()) - A->getr_max();

        //calculate the free distance
        Dfree = D - getSPMgenPairPPDP_a() - A->getSPMsta();

        if(Dfree >= 0)
            return true;
        else
            return false;

        break;

    case kdUnk:
        //caulculate the distance between the contour of the barrier
        //and the limit of maneuvering domain of the other actuator
        D = getBarrier()->getContour().distanceMin(A->getP0()) - A->getr_max();

        //calculate the free distance
        Dfree = D - getSPMsta() - A->getSPMsta();

        if(Dfree >= 0)
            return true;
        else
            return false;

        break;

    default:
        throw EImpossibleError("unknowledge value of PAkd");
    }
}

//--------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR LAS COLISIONES
//CON ACTUADORES ADYACENTES:

//determina la distancia mínima con una EA
double TActuator::distanceMin(const TExclusionArea *EA)
{
    //comprueba las precondiciones
    if(EA == NULL)
        throw EImproperArgument("pointer EA should point to built exclusion area");

    //busca el EA en la lista de pares (Ea, Dmin)
    int i = 0;
    while(i<AdjacentEAs.getCount() && EA!=AdjacentEAs[i].EA)
        i++;

    //comprueba las precondiciones
    if(i >= AdjacentEAs.getCount())
        throw EImproperArgument("pointer EA should be an adjacent exclusion area");

    //calcula la distancia con la barrera del EA
    double dm = distanceMin(&(EA->Barrier));

    //actualiza la distancia mínima
    if(i >= AdjacentEAs.getCount())
        throw EImpossibleError("lateral effect");
    if(dm < AdjacentEAs[i].Dmin)
        AdjacentEAs[i].Dmin = dm;
    //actualiza la distancia final
    AdjacentEAs[i].Dend = dm;

    return dm;
}
//determina la distancia mínima con un actuador
double TActuator::distanceMin(const TActuator *Actuator)
{
    //comprueba las precondiciones
    if(Actuator == NULL)
        throw EImproperArgument("pointer Actuator should point to built barrier");

    //busca el RP en la lista de pares (RP, Dmin)
    int i = 0;
    while(i<AdjacentRPs.getCount() && Actuator!=AdjacentRPs[i].RP->getActuator())
        i++;

    //comprueba las precondiciones
    if(i >= AdjacentRPs.getCount())
        throw EImproperArgument("pointer RP should be an adjacent robotic positioner");

    //calcula la distancia mínima
    double dm;

    //si la posición angular de ambos rotores del actuador adyacente es conocida
    if(Actuator->getPAkd() != kdUnk) {
        //si la posición angular de ambos rotores de este actuador es conocida
        if(getPAkd() != kdUnk)
            //calcula la distancia brazo-brazo
            dm = getArm()->distanceMin(Actuator->getArm());
        //si la posición angular de algún eje de este actuador es desconocida
        else
            //calcula la distancia barrera-brazo
            dm = getBarrier()->distanceMin(Actuator->getArm());
    }
    //si la posición angular de algún rotor del actuador adyacente es desconocida
    else
        //determina si hay colisión con la barrera del posicionador adyacente
        dm = distanceMin(Actuator->getBarrier());

    //actualiza la distancia mínima
    if(i >= AdjacentRPs.getCount())
        throw EImpossibleError("lateral effect");
    if(dm < AdjacentRPs[i].Dmin)
        AdjacentRPs[i].Dmin = dm;
    //actualiza la distancia final
    AdjacentRPs[i].Dend = dm;

    return dm;
}

//determina si hay colisión con una EA
bool TActuator::thereIsCollision(const TExclusionArea *EA)
{
    //comprueba las precondiciones
    if(EA == NULL)
        throw EImproperArgument("pointer EA should point to built exclusion area");

    //determina si hay colisión con la barrera del EA
    bool tic = thereIsCollision(&(EA->Barrier));

    return tic;
}
//determina si hay colisión con un actuador
bool TActuator::thereIsCollision(const TActuator *Actuator)
{
    //comprueba las precondiciones
    if(Actuator == NULL)
        throw EImproperArgument("pointer Actuator should point to built barrier");

    //si la posición angular de ambos rotores del actuador adyacente es conocida
    if(Actuator->getPAkd() != kdUnk) {
        //si la posición angular de ambos rotores de este actuador es conocida
        if(getPAkd() != kdUnk)
            //determina el estado de colisión brazo-brazo
            return getArm()->collides(Actuator->getArm());
        //si la posición angular de algún eje de este actuador es desconocida
        else
            //determina el estado de colisión barrera-brazo
            return getBarrier()->collides(Actuator->getArm());
    }
    //si la posición angular de algún rotor del actuador adyacente es desconocida
    else
        //determina si hay colisión con la barrera del posicionador adyacente
        return thereIsCollision(Actuator->getBarrier());

    //indica que no ha encontrado colisión
    return false;
}

//determina si hay colisión con una barrera o un actuador adyacente
bool TActuator::thereIsCollisionWithAdjacent(void)
{
    //por cada EA adyacente
    for(int i=0; i<AdjacentEAs.getCount(); i++) {
        //apunta la barrera indicada para facilitar su acceso
        TBarrier *AB = &(AdjacentEAs[i].EA->Barrier);
        //si hay colisión
        if(thereIsCollision(AB))
            //indica que si hay colisión
            return true;
    }

    //por cada RP adyacente
    for(int i=0; i<AdjacentRPs.getCount(); i++) {
        //apunta el actuador indicado para facilitar su acceso
        TActuator *AA = AdjacentRPs[i].RP->getActuator();
        //si hay colisión
        if(thereIsCollision(AA))
            //indica que si hay colisión
            return true;
    }

    //indica que no no hay colisión
    return false;
}
//determina si no hay colisión con una barrera o un actuador adyacente
bool TActuator::thereIsntCollisionWithAdjacent(void)
{
    //por cada EA adyacente
    for(int i=0; i<AdjacentEAs.getCount(); i++) {
        //apunta la barrera indicada para facilitar su acceso
        TBarrier *AB = &(AdjacentEAs[i].EA->Barrier);
        //si hay colisión
        if(thereIsCollision(AB))
            //indica que si hay colisión
            return false;
    }

    //por cada RP adyacente
    for(int i=0; i<AdjacentRPs.getCount(); i++) {
        //apunta el actuador indicado para facilitar su acceso
        TActuator *AA = AdjacentRPs[i].RP->getActuator();
        //si hay colisión
        if(thereIsCollision(AA))
            //indica que si hay colisión
            return false;
    }

    //indica que no no hay colisión
    return true;
}

//Busca los EAs adyacentes cuya
//barrera colisiona con este actuador
void TActuator::searchCollindingAdjacent(TItemsList<TExclusionArea*>& Collindings)
{
    //inicializa la lista de los que colisionan
    Collindings.Clear();

    //determina las barreras adyacentes con las que hay colisión
    for(int i=0; i<AdjacentEAs.getCount(); i++) {
        //apunta el actuador adyacente indicado para facilitar su acceso
        TExclusionArea *EAA = AdjacentEAs[i].EA;
        //si el actuador adyacente colisiona con este actuador
        if(thereIsCollision(&(EAA->Barrier)))
            //añade el EA a la lista de los que colisionan
            Collindings.Add(EAA);
    }
}
//Busca los RPs adyacentes cuyo
//actuador colisiona con este actuador
void TActuator::searchCollindingAdjacent(TItemsList<TRoboticPositioner*>& Collindings)
{
    //inicializa la lista de los que colisionan
    Collindings.Clear();

    //determina los actuadores adyacentes con los que hay colisión
    for(int i=0; i<AdjacentRPs.getCount(); i++) {
        //apunta el actuador adyacente indicado para facilitar su acceso
        TRoboticPositioner *RPA = AdjacentRPs[i].RP;
        //si el actuador adyacente colisiona con este actuador
        if(thereIsCollision(RPA->getActuator()))
            //añade el RP a la lista de los que colisionan
            Collindings.Add(RPA);
    }
}

//Determina si hay colisión con un actuador adyacente
//con evaluación de colisión pendiente.
bool TActuator::thereIsCollisionWithPendingAdjacent(void)
{
    //por cada EA adyacente
    for(int i=0; i<AdjacentEAs.getCount(); i++) {
        //apuntala barrera del EA indicada para facilitar su acceso
        TExclusionArea *EAA = AdjacentEAs[i].EA;
        //si alguno tiene pendiente la determinación de colisión,
        //y hay colisión
        if((Pending || EAA->Pending) && thereIsCollision(&(EAA->Barrier)))
            //indica que se ha encontrado colisión
            return true;
    }

    //por cada RP adyacente
    for(int i=0; i<AdjacentRPs.getCount(); i++) {
        //apunta el actuador del RP indicado para facilitar su acceso
        TActuator *AA = AdjacentRPs[i].RP->getActuator();
        //si alguno de los actuadores tiene pendiente la determinación de colisión,
        //y hay colisión
        if((Pending || AA->Pending) && thereIsCollision(AA))
            //indica que se ha encontrado colisión
            return true;
    }

    //ADVERTENCIA: se excluirá la condición PendingCollisionDetermine
    //mintras no se demuestre que funciona bien.

    //indica que no se invade el espacio de seguridad de ningún adyacente
    return false;
}

//Busca los EAs adyacentes
//con evaluación de colisión pendiente
//cuya barrera colisiona con este actuador.
void TActuator::searchCollindingPendingAdjacent(TItemsList<TExclusionArea*>& Collindings)
{
    //inicializa la lista de los que colisionan
    Collindings.Clear();

    //determina las barreras adyacentes con los que hay colisión
    for(int i=0; i<AdjacentEAs.getCount(); i++) {
        //apunta el RP adyacente indicado para facilitar su acceso
        TExclusionArea *EAA = AdjacentEAs[i].EA;
        //si alguna de las barreras tiene pendiente la determinación de colisión,
        //y colisiona con este actuador
        if((Pending || EAA->Pending) && thereIsCollision(&(EAA->Barrier)))
            //añade el RP a la lista de los que colisionan
            Collindings.Add(EAA);
    }
}
//Busca los RPs adyacentes
//con evaluación de colisión pendiente
//cuyo actuador colisiona con este actuador.
void TActuator::searchCollindingPendingAdjacent(TItemsList<TRoboticPositioner*>& Collindings)
{
    //inicializa la lista de los que colisionan
    Collindings.Clear();

    //determina los actuadores adyacentes con los que hay colisión
    for(int i=0; i<AdjacentRPs.getCount(); i++) {
        //apunta el RP adyacente indicado para facilitar su acceso
        TRoboticPositioner *RPA = AdjacentRPs[i].RP;
        //si alguno de los actuadores tiene pendiente la determinación de colisión,
        //y colisiona con este actuador
        if((Pending || RPA->getActuator()->Pending) && thereIsCollision(RPA->getActuator()))
            //añade el RP a la lista de los que colisionan
            Collindings.Add(RPA);
    }
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
