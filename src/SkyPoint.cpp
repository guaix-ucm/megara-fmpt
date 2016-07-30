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
//File: SkyPoint.cpp
//Content: sky point list
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "SkyPoint.h"
#include "TextFile.h"
#include "Strings.h"
#include "Scalars.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TPointType:
//---------------------------------------------------------------------------

void  strPrinPointType(string& str, TPointType value)
{
    switch(value) {
    case ptUNKNOWN: str += "UNKNOWN"; break;
    case ptSOURCE: str += "SOURCE"; break;
    case ptREFERENCE: str += "REFERENCE"; break;
    case ptBLANK: str += "BLANK"; break;
    }
    throw EImpossibleError("unknowledge value in type TPointType");
}
void  strReadPointType(TPointType& value, const string& str, unsigned int& i)
{
    //Here it is not required that text string str is printable.

    //check the precondition
    if(str.length() < i)
        throw EImproperArgument("index i should indicate a position in the string str");

    //Length of the known possible values of the string:
    //      strlen("UNKNOWN"): 7
    //      strlen("SOURCE"): 6
    //      strlen("REFERENCE"): 9
    //      strlen("BLANK"): 5

    //copy the index in an auxiliary variable
    unsigned int i_ = i;

    //advance the auxiliary index to the nextnon separator character
    strTravelSeparatorsIfAny(str, i_);

    //determine if the label is "REFERENCE" and return the corresponding value if any
    bool label_found = strTravelLabel_("REFERENCE", str, i_);
    if(label_found) {
        i = i_;
        value = ptREFERENCE;
        return;
    }

    //determine if the label is "UNKNOWN" and return the corresponding value if any
    label_found = strTravelLabel_("UNKNOWN", str, i_);
    if(label_found) {
        i = i_;
        value = ptUNKNOWN;
        return;
    }

    //determine if the label is "SOURCE" and return the corresponding value if any
    label_found = strTravelLabel_("SOURCE", str, i_);
    if(label_found) {
        i = i_;
        value = ptSOURCE;
        return;
    }

    //determine if the label is "BLANK" and return the corresponding value if any
    label_found = strTravelLabel_("BLANK", str, i_);
    if(label_found) {
        i = i_;
        value = ptBLANK;
        return;
    }

    throw EImproperArgument("there is a value of type TPointType from position i in string str");
}
string pointTypeToStr(TPointType value)
{
    switch(value) {
    case ptUNKNOWN: return "UNKNOWN";
    case ptSOURCE: return "SOURCE";
    case ptREFERENCE: return "REFERENCE";
    case ptBLANK: return "BLANK";
    }
    throw EImpossibleError("unknowledge value in variable of type TPointType");
}
TPointType strToPointType(const string& str)
{
    //Here it is not required that text string str is printable.

    //Length of the known possible values of the string:
    //      strlen("UNKNOWN"): 7
    //      strlen("SOURCE"): 6
    //      strlen("REFERENCE"): 9
    //      strlen("BLANK"): 5

    //copy the index in an auxiliary variable
    unsigned int i = 0;

    //advance the auxiliary index to the nextnon separator character
    strTravelSeparatorsIfAny(str, i);

    //determine if the label is "REFERENCE" and return the corresponding value if any
    bool label_found = strTravelLabel_("REFERENCE", str, i);
    if(label_found)
        return ptREFERENCE;

    //determine if the label is "UNKNOWN" and return the corresponding value if any
    label_found = strTravelLabel_("UNKNOWN", str, i);
    if(label_found)
        return ptUNKNOWN;

    //determine if the label is "SOURCE" and return the corresponding value if any
    label_found = strTravelLabel_("SOURCE", str, i);
    if(label_found)
        return ptSOURCE;

    //determine if the label is "BLANK" and return the corresponding value if any
    label_found = strTravelLabel_("BLANK", str, i);
    if(label_found)
        return ptBLANK;

    throw EImproperArgument("Point Type should be [\"UNKNOWN\" | \"SOURCE\" | \"REFERENCE\" | \"BLANCK\"] but it is: \""+StrFirstChars(strTrim(str.substr(i, str.length() - i))).str+"\"");
}

//---------------------------------------------------------------------------
//TSkyPoint:
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//INICIALIZA LAS PROPIEDADES ESTÁTICAS:

AnsiString TSkyPoint::IdLabel = "Id";
AnsiString TSkyPoint::RALabel = "RA";
AnsiString TSkyPoint::DECLabel = "DEC";
AnsiString TSkyPoint::TintLabel = "Tint";
AnsiString TSkyPoint::TypeLabel = "Type";
AnsiString TSkyPoint::PriorityLabel = "Priority";

//---------------------------------------------------------------------------
//PROPIEDADES DEFINITORIAS:

void TSkyPoint::setId(int Id)
{
    //comprueba las preocndiciones
    if(Id < 1)
        throw EImproperArgument("identifier number Id should be upper zero");

    //asigna el nuevo valor
    p_Id = Id;
}
void TSkyPoint::setDEC(double DEC)
{
    //la declinación DEC debería estar en [-pi/2, pi/2]
    if(DEC<-M_PI/2 || M_PI/2<DEC)
        throw EImproperArgument(AnsiString("decline ")+FloatToStr(DEC)+AnsiString(" should be in [-pi/2, pi/2]"));

    //asigna el nuevo valor
    p_DEC = DEC;
}
void TSkyPoint::setRA(double RA)
{
    //asigna el nuevo valor
    p_RA = RA;
}

void TSkyPoint::setTint(double Tint)
{
    //comprueba las preocndiciones
    if(Tint < 0)
        throw EImproperArgument("integration time Tint should be nonnegative");

    //asigna el nuevo valor
    p_Tint = Tint;
}
void TSkyPoint::setName(const AnsiString &Name)
{
    //la cadena de texto Name debería estar entre comillas simples
    if(StrIsntString(Name))
        throw EImproperArgument(AnsiString("string '")+Name+AnsiString("' should to be between single quotes"));

    //el nombre Name debería ser una cadena de texto imprimible
    if(StrIsntPrintable(Name))
        throw EImproperArgument(AnsiString("name '")+Name+AnsiString("' should be a printable string"));

    //el nombre Name debería tener 23 caracteres como máximo
    if(Name.Length() > 23)
        throw EImproperArgument("name "" should have 23 characters maximun");

    //asigna el nuevo valor
    p_Name = Name;
}

//PROPIEDADES DE INTERFAZ:

AnsiString TSkyPoint::getIdText(void) const
{
    return IntToStr(getId());
}
void TSkyPoint::setIdText(const AnsiString &S)
{
    try {
        p_Id = StrToInt(S);
    } catch(...) {
        throw;
    }
}
AnsiString TSkyPoint::getRAText(void) const
{
    return FloatToStr(getRA());
}
void TSkyPoint::setRAText(const AnsiString &S)
{
    try {
        setRA(StrToFloat(S));
    } catch(...) {
        throw;
    }
}
AnsiString TSkyPoint::getDECText(void) const
{
    return FloatToStr(getDEC());
}
void TSkyPoint::setDECText(const AnsiString &S)
{
    try {
        setDEC(StrToFloat(S));
    } catch(...) {
        throw;
    }
}
AnsiString TSkyPoint::getTintText(void) const
{
    return FloatToStr(getTint());
}
void TSkyPoint::setTintText(const AnsiString &S)
{
    try {
        p_Tint = StrToFloat(S);
    } catch(...) {
        throw;
    }
}
AnsiString TSkyPoint::getTypeText(void) const
{
    AnsiString S;
    switch(Type) {
    case ptUNKNOWN: S = "UNKNOWN"; break;
    case ptSOURCE: S = "SOURCE"; break;
    case ptREFERENCE: S = "REFERENCE"; break;
    case ptBLANK: S = "BLANK"; break;
    }
    return S;
}
void TSkyPoint::setTypeText(const AnsiString &S)
{
    if(S == "UNKNOWN")
        Type = ptUNKNOWN;
    else if(S == "SOURCE")
        Type = ptSOURCE;
    else if(S == "REFERENCE")
        Type = ptREFERENCE;
    else if(S == "BLANK")
        Type = ptBLANK;
    else
        throw EImproperArgument("sky point type should be known");
}

//CONJUNTOS DE PROPIEDADES DEFINITORIAS EN FORMATO TEXTO:

AnsiString TSkyPoint::getRowText(void) const
{
    AnsiString S;

    S += getIdText();
    StrFill(S, 12, ' ');
    S += getRAText();
    StrFill(S, 12+24, ' ');
    S += getDECText();

    return S;
}
void TSkyPoint::setRowText(const AnsiString &S)
{
    try {
        int i = 1;
        TSkyPoint *aux = this;
        ReadRow(aux, S, i);
        StrTravelToEnd(S, i);
    } catch(...) {
        throw;
    }
}
AnsiString TSkyPoint::getColText(void) const
{
    AnsiString S;

    S += getIdText()+AnsiString("\r\n");
    S += getRAText()+AnsiString("\r\n");
    S += getDECText();

    return S;
}
void TSkyPoint::setColText(const AnsiString &S)
{
    try {
        int i = 1;
        TSkyPoint *aux = this;
        ReadCol(aux, S, i);
        StrTravelToEnd(S, i);
    } catch(...) {
        throw;
    }
}
AnsiString TSkyPoint::getAssignsText(void) const
{
    AnsiString S;

    S += AnsiString("Id = ")+getIdText()+AnsiString("\r\n");
    S += AnsiString("phi' = ")+getRAText()+AnsiString("\r\n");
    S += AnsiString("theta' = ")+getDECText();

    return S;
}
void TSkyPoint::setAssignsText(const AnsiString &S)
{
    try {
        int i = 1;
        TSkyPoint *aux = this;
        ReadAssigns(aux, S, i);
        StrTravelToEnd(S, i);
    } catch(...) {
        throw;
    }
}

//MÉTODOS ESTÁTICOS DE LISTA:

//compara una de las propiedades de dos puntos de cielo
int  TSkyPoint::CompareIds(TSkyPoint *SP1,
                                     TSkyPoint *SP2)
{
    //el puntero SP1 debería apuntar a un punto de cielo contruido
    if(SP1 == NULL)
        throw EImproperArgument("pointer SP1 should point to built sky point");

    //el puntero SP2 debería apuntar a un punto de cielo contruido
    if(SP2 == NULL)
        throw EImproperArgument("pointer SP2 should point to built sky point");

    if(SP1->getId() < SP2->getId())
        return -1;
    if(SP1->getId() > SP2->getId())
        return 1;
    return 0;
}
int  TSkyPoint::CompareRAs(TSkyPoint *SP1,
                                     TSkyPoint *SP2)
{
    //el puntero SP1 debería apuntar a un punto de cielo contruido
    if(SP1 == NULL)
        throw EImproperArgument("pointer SP1 should point to built sky point");

    //el puntero SP2 debería apuntar a un punto de cielo contruido
    if(SP2 == NULL)
        throw EImproperArgument("pointer SP2 should point to built sky point");

    if(SP1->getRA() < SP2->getRA())
        return -1;
    if(SP1->getRA() > SP2->getRA())
        return 1;
    return 0;
}
int  TSkyPoint::CompareDECs(TSkyPoint *SP1,
                                      TSkyPoint *SP2)
{
    //el puntero SP1 debería apuntar a un punto de cielo contruido
    if(SP1 == NULL)
        throw EImproperArgument("pointer SP1 should point to built sky point");

    //el puntero SP2 debería apuntar a un punto de cielo contruido
    if(SP2 == NULL)
        throw EImproperArgument("pointer SP2 should point to built sky point");

    if(SP1->getDEC() < SP2->getDEC())
        return -1;
    if(SP1->getDEC() > SP2->getDEC())
        return 1;
    return 0;
}

//obtiene las etiquetas de las propiedades en una cadena
AnsiString TSkyPoint::GetLabels(void)
{
    return AnsiString("(")+IdLabel+AnsiString(", ")+RALabel+AnsiString(", ")+DECLabel+AnsiString(")");
}
AnsiString TSkyPoint::GetLabelsRow(void)
{
    return IdLabel+AnsiString("\t")+RALabel+AnsiString("\t")+DECLabel;
}

//imprime las propiedades de un punto de cielo en una cadena
void  TSkyPoint::PrintRow(AnsiString &S,
                                    TSkyPoint *SP)
{
    //el puntero SP debería apuntar a un punto de cielo contruido
    if(SP == NULL)
        throw EImproperArgument("pointer SP should point to built sky point");

    S += SP->getRowText();
}
void  TSkyPoint::PrintCol(AnsiString &S,
                                    TSkyPoint *SP)
{
    //el puntero SP debería apuntar a un punto de cielo contruido
    if(SP == NULL)
        throw EImproperArgument("pointer SP should point to built sky point");

    S += SP->getColText();
}
void  TSkyPoint::PrintAssigns(AnsiString &S, TSkyPoint *SP)
{
    //el puntero SP debería apuntar a un punto de cielo contruido
    if(SP == NULL)
        throw EImproperArgument("pointer SP should point to built sky point");

    S += SP->getAssignsText();
}

//lee las propiedades de un punto de cielo en una cadena de texto
void  TSkyPoint::ReadRow(TSkyPoint* &SP,
                                   const AnsiString &S, int &i)
{
    //el puntero SP debería apuntar a un punto de cielo construído
    if(SP == NULL)
        throw EImproperArgument("pointer SP shoult point to built sky point");

    //si el índice i no indica a una posición de la cadena
    if(i<1 || S.Length()<i)
        //indica que no se han encontrado los valores del punto de cielo
        throw EImproperArgument("sky point values not founds");

    //estado de lectura
    //      0: esperando cadena de texto para Id
    //      1: esperando valor en punto flotante para phi'
    //      2: esperando valor en punto flotante para theta'
    //      3: punto de cielo leido con éxito
    int status = 0;

    //variables auxiliares
    TSkyPoint t_SP(SP); //variable tampón
    AnsiString Ident; //identificador de propiedad
    AnsiString Value; //valor de propiedad

    //ADVERTENCIA: las variables tampón con propiedades interdependientes
    //deben ser clones de las variables que se pretenden modificar.

    //NOTA: adviertase que las propiedades enteras son leídas como
    //valores en punto flotante para detectar errores en los cuales
    //sea especificado un valor en punto flotante en vez de un valor entero.

    do {
        switch(status) {
        case 0: //esperando cadena de texto para Id
            try {
            StrReadWord(Value, S, i);
            t_SP.setIdText(Value);
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property Id"));
            } catch(...) {
            throw;
        }
            if(i > S.Length())
                throw EImproperArgument("phi' value not found");
            status++;
            break;
        case 1: //esperando valor en punto flotante para phi'
            try {
            StrReadWord(Value, S, i);
            t_SP.setRAText(Value);
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property phi'"));
            } catch(...) {
            throw;
        }
            if(i > S.Length())
                throw EImproperArgument("theta' value not found");
            status++;
            break;
        case 2: //esperando valor en punto flotante para theta'
            try {
            StrReadWord(Value, S, i);
            t_SP.setDECText(Value);
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property theta'"));
            } catch(...) {
            throw;
        }
            status++;
            break;
        }
    } while(status < 3);

    //asigna la variable tampón
    *SP = t_SP;
}
void  TSkyPoint::ReadCol(TSkyPoint* &SP,
                                   const AnsiString &S, int &i)
{
    //el puntero SP debería apuntar a un punto de cielo construído
    if(SP == NULL)
        throw EImproperArgument("pointer SP shoult point to built sky point");

    //si el índice i no indica a una posición de la cadena
    if(i<1 || S.Length()<i)
        //indica que no se han encontrado los valores del punto de cielo
        throw EImproperArgument("sky point values not founds");

    //estado de lectura
    //      0: esperando cadena de texto para Name
    //      1: esperando valor en punto flotante para phi'
    //      2: esperando valor en punto flotante para theta'
    //      3: punto de cielo leido con éxito
    int status = 0;

    //variables auxiliares
    TSkyPoint t_SP(SP); //variabletampón
    AnsiString Ident; //identificador de propiedad
    AnsiString Value; //valor de propiedad

    //ADVERTENCIA: las variables tampón con propiedades interdependientes
    //deben ser clones de las variables que se pretenden modificar.

    //NOTA: adviertase que las propiedades enteras son leídas como
    //valores en punto flotante para detectar errores en los cuales
    //sea especificado un valor en punto flotante en vez de un valor entero.

    do {
        switch(status) {
        case 0: //esperando cadena de texto para Id
            try {
            StrReadWord(Value, S, i);
            t_SP.setIdText(Value);
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property Id"));
            } catch(...) {
            throw;
        }
            if(i > S.Length())
                throw EImproperArgument("phi' value not found");
            status++;
            break;
        case 1: //esperando valor en punto flotante para phi'
            try {
            StrReadWord(Value, S, i);
            t_SP.setRAText(Value);
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property phi'"));
            } catch(...) {
            throw;
        }
            if(i > S.Length())
                throw EImproperArgument("theta' value not found");
            status++;
            break;
        case 2: //esperando valor en punto flotante para theta'
            try {
            StrReadWord(Value, S, i);
            t_SP.setDECText(Value);
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property theta'"));
            } catch(...) {
            throw;
        }
            status++;
            break;
        }
    } while(status < 3);

    //asigna la variable tampón
    *SP = t_SP;
}
void  TSkyPoint::ReadAssigns(TSkyPoint* &SP,
                                       const AnsiString &S, int &i)
{
    //el puntero SP debería apuntar a un punto de cielo construído
    if(SP == NULL)
        throw EImproperArgument("pointer SP shoult point to built sky point");

    //si el índice i no indica a una posición de la cadena
    if(i<1 || S.Length()<i)
        //indica que no se han encontrado los valores del punto de cielo
        throw EImproperArgument("sky point values not founds");

    //estado de lectura
    //      0: esperando cadena de texto para Name
    //      1: esperando valor en punto flotante para phi'
    //      2: esperando valor en punto flotante para theta'
    //      3: punto de cielo leido con éxito
    int status = 0;

    //variables auxiliares
    TSkyPoint t_SP(SP); //variable tampón
    AnsiString Ident; //identificador de propiedad
    AnsiString Value; //valor de propiedad

    //ADVERTENCIA: las variables tampón con propiedades interdependientes
    //deben ser clones de las variables que se pretenden modificar.

    //NOTA: adviertase que las propiedades enteras son leídas como
    //valores en punto flotante para detectar errores en los cuales
    //sea especificado un valor en punto flotante en vez de un valor entero.

    do {
        switch(status) {
        case 0: //esperando cadena de texto para Id
            try {
            StrReadLabel(Ident, "Id", S, i);
            StrTravelLabel("=", S, i);
            StrReadWord(Value, S, i);
            t_SP.setIdText(Value);
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property Id"));
            } catch(...) {
            throw;
        }
            if(i > S.Length())
                throw EImproperArgument("phi' value not found");
            status++;
            break;
        case 1: //esperando valor en punto flotante para phi'
            try {
            StrReadLabel(Ident, "phi'", S, i);
            StrTravelLabel("=", S, i);
            StrReadWord(Value, S, i);
            t_SP.setRAText(Value);
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property phi'"));
            } catch(...) {
            throw;
        }
            if(i > S.Length())
                throw EImproperArgument("theta' value not found");
            status++;
            break;
        case 2: //esperando valor en punto flotante para theta'
            try {
            StrReadLabel(Ident, "theta'", S, i);
            StrTravelLabel("=", S, i);
            StrReadWord(Value, S, i);
            t_SP.setDECText(Value);
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property theta'"));
            } catch(...) {
            throw;
        }
            status++;
            break;
        }
    } while(status < 3);

    //asigna la variable tampón
    *SP = t_SP;
}

//MÉTODOS PÚBLISPS:

//contruye un punto de cielo
TSkyPoint::TSkyPoint(void) :
    p_Id(0), p_RA(0), p_DEC(0), p_Tint(0), Type(ptUNKNOWN), Priority(0)
{
    setName("\'\'");
}

//construye un clon de un punto de cielo
TSkyPoint::TSkyPoint(TSkyPoint *SP)
{
    //el puntero SP debería apuntar a un punto de cielo contruido
    if(SP == NULL)
        throw EImproperArgument("pointer SP should point to built sky point");

    //asigna las propiedades
    Copy(SP);
}

//MÉTODOS DE SPPIA:

//clona un punto de cielo
void TSkyPoint::Copy(TSkyPoint *SP)
{
    //el puntero SP debería apuntar a un punto de cielo contruido
    if(SP == NULL)
        throw EImproperArgument("pointer SP should point to built sky point");

    //asigna las propiedades
    p_Id = SP->p_Id;
    p_RA = SP->p_RA;
    p_DEC = SP->p_DEC;
    p_Tint = SP->p_Tint;
    Type = SP->Type;
}
TSkyPoint &TSkyPoint::operator=(const TSkyPoint &SP)
{
    //asigna las propiedades
    p_Id = SP.p_Id;
    p_RA = SP.p_RA;
    p_DEC = SP.p_DEC;
    p_Tint = SP.p_Tint;
    Type = SP.Type;

    return *this;
}

//MÉTODOS DE DESPLAZAMIENTO:

//asigna (DEC, RA)
void TSkyPoint::SetPosition(double RA, double DEC)
{
    //la colatitud DEC debería estar en [-pi/2, pi/2]
    if(DEC<-M_PI/2 || M_PI/2<DEC)
        throw EImproperArgument(AnsiString("colatitude ")+FloatToStr(DEC)+AnsiString(" should be in [-pi/2, pi/2]"));

    //asigna los nuevos valores
    p_RA = RA;
    p_DEC = DEC;
}

//randomiza las coordenadas celestes con distribución uniforme
//en la esfera celeste
void TSkyPoint::Randomize(void)
{
    //genera un versor en coordenadas cartesianas
    double x, y, z;
    RandomVersor(x, y, z);

    //calcula la ascensión recta del versor
    setRA(Azimut(x, y));
    //calcula la declinación del versor
    setDEC(Colatitud(x, y, z));
}
//randomiza las coordenadas celestes con distribución uniforme
//en un intervalo
void TSkyPoint::Randomize(double RAmin, double RAmax,
                          double DECmin, double DECmax)
{
    //RAmax debería ser mayor que RAmin
    if(RAmax <= RAmin)
        throw EImproperArgument("RAmax should be upper RAmin");

    //DECmin debería estar en [-pi/2, pi/2]
    if(DECmin<-M_PI/2 || M_PI/2<DECmin)
        throw EImproperArgument("DECmin should be in [-pi/2, pi/2]");

    //DECmax debería estar en [-pi/2, pi/2]
    if(DECmax<-M_PI/2 || M_PI/2<DECmax)
        throw EImproperArgument("DECmax should be in [-pi/2, pi/2]");

    //DECmax debería ser mayor que DECmin
    if(DECmax <= DECmin)
        throw EImproperArgument("DECmax should be upper DECmin");

    //randomiza las coordenadas
    setRA(RandomUniform(RAmin, RAmax));
    setDEC(RandomUniform(DECmin, DECmax));
}

//---------------------------------------------------------------------------
//TSkyPointList
//---------------------------------------------------------------------------

//PROPIEDADES DE INTERFAZ:

AnsiString TSkyPointList::getTableText(void)
{
    AnsiString S;

    //añade la cabecera
    S = TSkyPoint::GetLabelsRow()+AnsiString("\r\n");
    for(int i=0; i<getCount(); i++)
        S += Items[i]->getRowText()+AnsiString("\r\n");

    return S;
}

void TSkyPointList::setTableText(const AnsiString &S)
{
    //descompone la cadena en lineas
    TStringList *SL = new TStringList();
    SL->setText(S);

    //la lista de SPs debería contener la linea de identificadores
    if(SL->getCount() < 1)
        throw EImproperArgument("row identifiers not found");

    //la primera linea debería tener los identificadores de los campos
    //de la lista de puntos de cielo
    if(StrNotHasSameWords(SL->Strings[0], TSkyPoint::GetLabelsRow()))
        throw EImproperArgument("row identifiers not found");

    //variable tampón
    TItemsList<TSkyPoint*> SPL;
    //variables auxiliares
    TSkyPoint *SP;

    //COMPRUEBA LA SINTAXIS DE LAS LÍNEAS DE TEXTO:

    try {
        //por cada linea de texto a partir de la segunda
        for(int i=1; i<SL->getCount(); i++) {
            //contruye un punto de cielo
            SP = new TSkyPoint();
            //asigna la linea de texto al punto de cielo
            SP->setRowText(SL->Strings[i]);
            //añade el punto decielo a la lista
            SPL.Add(SP);
        }
    } catch(...) {
        throw;
    }

    //ASIGNA LOS NUEVOS VALORES:

    //destruye los objetos originales
    Destroy();
    //traslada los puntos de cielo a la lista
    for(int i=0; i<SPL.getCount(); i++)
        Add(SPL[i]);

    //libera la memoria dinámica
    delete SL;
}

/*
//MÉTODOS ESTÁTICOS:

//imprime una lista de puntos de cielo en una cadena de texto
void  TSkyPointList::PrintTSkyPoints(AnsiString &S,
        const TSkyPointList *L)
{
        try {
                PrintList(S, L);
        } catch(...) {
                throw;
        }
}
//lee una lista de puntos de cielo en una cadena de texto
void  TSkyPointList::ReadTSkyPoints(
        TSkyPointList *L,
        const AnsiString &S, int &i)
{
        try {
                ScanList(L, S, i);
        } catch(...) {
                throw;
        }
}
            */
//MÉTODOS PÚBLICOS:

//construye una lista de puntos de cielo
TSkyPointList::TSkyPointList(int Capacity) :
    TItemsList<TSkyPoint*>(Capacity,
                           TSkyPoint::CompareIds,
                           NULL, NULL,
                           TSkyPoint::PrintRow,
                           TSkyPoint::ReadRow)
{
}

//copia una lista de puntos de cielo
TSkyPointList &TSkyPointList::operator=(const TSkyPointList &SPL)
{
    //redimensiona la lista
    if(SPL.getCount() < getCount())
        Items.DelLast(getCount() - SPL.getCount());
    else if(SPL.getCount() > getCount())
        Items.NewLast(SPL.getCount() - getCount());

    //copia los elementos
    for(int i=0; i<getCount(); i++)
        Items[i] = SPL[i];

    return *this;
}

//busca el objeto cleste con el nombre indicado
int TSkyPointList::SearchId(int Id)
{
    int i;

    for(i=0; i<getCount(); i++)
        if(Items[i]->getId() == Id)
            return i;

    return i;
}

//contruye N puntos de cielo inicializados por defecto
void TSkyPointList::Build(int N)
{
    //el número de puntos de cielo N debe ser mayor que cero
    if(N < 1)
        throw EImproperArgument("sky points number N should be upper zero");

    //repite N veces
    for(int i=1; i<=N; i++)
        //añade un nuevo objeto eleste
        Add(new TSkyPoint());
}
//destruye los puntos de cielo de la lista
void TSkyPointList::Destroy(void)
{
    //destruye los objetos de la lista
    for(int i=0; i<getCount(); i++)
        if(Items[i] != NULL)
            delete Items[i];

    Clear(); //vacia la lista
}

//reenumera los puntos de cielo
void TSkyPointList::Reidentify(void)
{
    for(int i=0; i<getCount(); i++)
        Items[i]->setId(i+1);
}

//randomiza las coordenadas celestes
//de los puntos de cielo de la lista
//con distribución uniforme en toda la esfera celeste
void TSkyPointList::Randomize(void)
{
    //por cada punto de cielo de la lista
    for(int i=0; i<getCount(); i++)
        //randomiza el punto de cielo indicado
        Items[i]->Randomize();
}
//randomiza las coordenadas celestes
//de los puntos de cielo de la lista
//con distribución uniforme en un intervalo
void TSkyPointList::Randomize(double RAmin, double RAmax,
                              double DECmin, double DECmax)
{
    try {
        //por cada punto decielo de la lista
        for(int i=0; i<getCount(); i++)
            //randomiza las coordenadas del punto de cielo indicado
            Items[i]->Randomize(RAmin, RAmax, DECmin, DECmax);
    } catch(...) {
        throw;
    }
}

//borra los puntos de cielo cuyo identificador se coincida con el indicado
void TSkyPointList::DeleteId(int Id)
{
    //el número de identificación debería ser no negativo
    if(Id < 0)
        throw EImproperArgument("identifier number Id should be nonnegative");

    //por cada punto de cielo de la lista
    for(int i=0; i<getCount(); i++)
        //si el identificador del punto de cielo indicado coincide con el indicado
        if(Items[i]->getId() == Id)
            Delete(i); //borra la posición indicada
}
/*//borra los puntos de cielo correspondientes a
//los puntos de proyección que han sido asignados
void TSkyPointList::DeleteAllocatedObjects(TAllocationPairList *APL)
{
        //el puntero APL debería apuntar a una lista de pares de asignación contruida
        if(APL == NULL)
                throw EImproperArgument("pointer APL should point to built allocation pair list");

        //por cada par de asignación
        for(int i=0; i<APL->Count; i++)
                //borra los puntos de cielo cuyo identificador coindida con el indicado
                DeleteId(APL->Get(i).ProjectionPoint->Id);
} */

//MÉTODOS DE INTERFAZ:
/*#
//imprime los puntos de cielo de la lista en una caja de lista
void TSkyPointList::Print(QListWidget *LB)
{
    //el puntero LB debería apuntar a una caja de lista construida
    if(LB == NULL)
        throw EImproperArgument("pointer LB should point to built list box");

    LB->clear();
    for(int i=0; i<getCount(); i++) {
        QString QS(Items[i]->getRowText().c_str());
        LB->addItem(QS);
    }
}
//imprime los puntos de cielo en un memorando
void TSkyPointList::Print(QTextEdit *M)
{
    //el puntero M debería apuntar a un memorando construído
    if(M == NULL)
        throw EImproperArgument("pointer M should point to built memo");

    M->clear();
    QString QS = TSkyPoint::GetLabelsRow().c_str();
    M->append(QS);
    for(int i=0; i<getCount(); i++) {
        QS = Items[i]->getRowText().c_str();
        M->append(QS);
    }
}
*/

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

