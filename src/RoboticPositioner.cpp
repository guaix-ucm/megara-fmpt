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
//Archivo: RoboticPositioner.cpp
//Contenido: posicionador básico (dimensión y posición)
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "RoboticPositioner.h"
#include "Strings.h" //StrIndent

#include <limits> //std::numeric_limits

//---------------------------------------------------------------------------

using namespace Strings;
using namespace Positioning;

//espacio de nombres de modelos
namespace Models {

//##########################################################################
//TControlMode
//##########################################################################

void  StrPrintControlMode(AnsiString& S,
                          TControlMode cm)
{
    switch(cm) {
    case cmSinc: S +=  AnsiString("Sinc"); break;
    case cmAsinc: S +=  AnsiString("Asinc"); break;
    }
    throw EImpossibleError("unknown value in type TControlMode");
}
void  StrReadControlMode(TControlMode& cm,
                         const AnsiString &S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //La longitud de los valores legibles de la cadena son:
    //      strlen("Sinc"): 4
    //      strlen("Asinc"): 5

    //variable tampón
    AnsiString SubS;

    //copia el índice en una variable auxiliar
    int j = i;
    //avanza el índice auxiliar hasta el próximo caracter no separador
    StrTravelSeparatorsIfAny(S, j);

    //si desde el primera caracter hay al menos 5 caracteres (con él incluido)
    if(j+4 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 5 caracteres
        SubS = S.SubString(j, 5);

        //si la subcadena coincide con "Asinc"
        if(SubS == "Asinc") {
            //traduce de texto a estructura
            cm = cmAsinc;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 5;
            return; //no hace nada más
        }
    }

    //si desde el primera caracter hay al menos 4 caracteres (con él incluido)
    if(j+3 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 4 caracteres
        SubS = S.SubString(j, 4);

        //si la subcadena coincide con "Sinc"
        if(SubS == "Sinc") {
            //traduce de texto a estructura
            cm = cmSinc;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 4;
            return; //no hace nada más
        }
    }

    throw EImproperArgument("there isn't a value of type TControlMode in position i of string text S");
}
AnsiString ControlModeToStr(TControlMode cm)
{
    switch(cm) {
    case cmSinc: return AnsiString("Sinc");
    case cmAsinc: return AnsiString("Asinc");
    }
    throw EImpossibleError("unknown value in type TControlMode");
}
TControlMode StrToControlMode(const AnsiString& S)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //La longitud de los valores legibles de la cadena son:
    //      strlen("Sinc"): 4
    //      strlen("Asinc"): 5

    //variable tampón
    AnsiString SubS;

    //inicializa el índice
    int j = 1;
    //avanza el índice auxiliar hasta el próximo caracter no separador
    StrTravelSeparatorsIfAny(S, j);

    //si desde el primera caracter hay al menos 5 caracteres (con él incluido)
    if(j+4 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 5 caracteres
        SubS = S.SubString(j, 5);

        //si la subcadena coincide con "Asinc"
        if(SubS == "Asinc")
            //devuelve el valor correspondiente
            return cmAsinc;
    }

    //si desde el primera caracter hay al menos 4 caracteres (con él incluido)
    if(j+3 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 4 caracteres
        SubS = S.SubString(j, 4);

        //si la subcadena coincide con "Sinc"
        if(SubS == "Sinc")
            //devuelve el valor correspondiente
            return cmSinc;
    }

    throw EImproperArgument("there isn't a value of type TControlMode in position i of string text S");
}

//##########################################################################
//TFaultType
//##########################################################################

void  StrPrintFaultType(AnsiString& S,
                        TFaultType ft)
{
    switch(ft) {
    case ftUnk: S +=  AnsiString("Unk"); break;
    case ftSta: S +=  AnsiString("Sta"); break;
    case ftDyn: S +=  AnsiString("Dyn"); break;
    }
    throw EImpossibleError("unknown value in type TFaultType");
}
void  StrReadFaultType(TFaultType& ft,
                       const AnsiString &S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //La longitud de los valores legibles de la cadena son:
    //      strlen("Unk"): 3
    //      strlen("Sta"): 3
    //      strlen("Dyn"): 3

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

        //si la subcadena coincide con "Unk"
        if(SubS == "Unk") {
            //traduce de texto a estructura
            ft = ftUnk;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 3;
            return; //no hace nada más
        }

        //si la subcadena coincide con "Sta"
        if(SubS == "Sta") {
            //traduce de texto a estructura
            ft = ftSta;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 3;
            return; //no hace nada más
        }

        //si la subcadena coincide con "Dyn"
        if(SubS == "Dyn") {
            //traduce de texto a estructura
            ft = ftDyn;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 3;
            return; //no hace nada más
        }
    }

    throw EImproperArgument("there isn't a value of type TFaultType in position i of string text S");
}
AnsiString FaultTypeToStr(TFaultType ft)
{
    switch(ft) {
    case ftUnk: return AnsiString("Unk");
    case ftSta: return AnsiString("Sta");
    case ftDyn: return AnsiString("Dyn");
    }
    throw EImpossibleError("unknown value in type TFaultType");
}
TFaultType StrToFaultType(const AnsiString& S)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //La longitud de los valores legibles de la cadena son:
    //      strlen("Unk"): 3
    //      strlen("Sta"): 3
    //      strlen("Dyn"): 3

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

        //si la subcadena coincide con "Unk"
        if(SubS == "Unk")
            //devuelve el valor correspondiente
            return ftUnk;

        //si la subcadena coincide con "Sta"
        if(SubS == "Sta")
            //devuelve el valor correspondiente
            return ftSta;

        //si la subcadena coincide con "Dyn"
        if(SubS == "Dyn")
            //devuelve el valor correspondiente
            return ftDyn;
    }

    throw EImproperArgument("there isn't a value of type TFaultType in position i of string text S");
}

//##########################################################################
//TRoboticPositioner
//##########################################################################

//---------------------------------------------------------------------------
//PROPIEDADES ESTÁTICAS:

//lista de objetos construidos
TItemsList<TRoboticPositioner*> TRoboticPositioner::Builts;

//---------------------------------------------------------------------------
//PROPIEDADES DE SEGURIDAD:

//TOLERANCES:

void TRoboticPositioner::setEo(double Eo)
{
    //check the precondition
    if(Eo < 0)
        throw EImproperArgument("orientation error margin in S0 (Eo) should be nonnegative");

    //set the new value
    p_Eo = Eo;

    //assimilates the new value
    calculateSPMcomponents();
}
void TRoboticPositioner::setEp(double Ep)
{
    //check the precondition
    if(Ep < 0)
        throw EImproperArgument("position error margin in S0 (Ep) should be nonnegative");

    //set the new value
    p_Ep = Ep;

    //assimilates the new value
    calculateSPMcomponents();
}

void TRoboticPositioner::setTstop(double Tstop)
{
    //check the precondition
    if(Tstop < 0)
        throw EImproperArgument("maximun time betweem last storage of the position angles (Tstop) should be nonnegative");

    //set the new value
    p_Tstop = Tstop;

    //assimilates the new value
    calculateSPMcomponents();
}
void TRoboticPositioner::setTshiff(double Tshiff)
{
    //check the precondition
    if(Tshiff < 0)
        throw EImproperArgument("maximun time shift between RPs in motion (Tshiff) should be nonnegative");

    //set the new value
    p_Tshiff = Tshiff;

    //assimilates the new value
    calculateSPMcomponents();
}

void TRoboticPositioner::setSPMadd(double SPMadd)
{
    //check the precondition
    if(SPMadd < 0)
        throw EImproperArgument("SPM additional to add to SPMdyn (SPMadd) should be nonnegative");

    //set the new value
    p_SPMadd = SPMadd;

    //assimilates the new value
    calculateSPMcomponents();
}

//STATUS PROPERTIES:

void TRoboticPositioner::setFaultProbability(double FaultProbability)
{
    //la probabilidad de fallo FaultProbability debe estar en [0, 1]
    if(FaultProbability<0 || 1<FaultProbability)
        throw EImproperArgument("fault probability FaultProbability should be in [0, 1]");

    //asigna el valor
    p_FaultProbability = FaultProbability;
}

void TRoboticPositioner::setDsecMax(double DsecMax)
{
    if(DsecMax < 0)
        throw EImproperArgument("maximun additional security distance during retraction (DsecMax) should be nonnegative");

    p_DsecMax = DsecMax;
}
void TRoboticPositioner::setDsec(double Dsec)
{
    if(Dsec<0 || getDsecMax()<Dsec)
        throw EImproperArgument("additional security distance during retraction (Dsec) should be in [0, DsecMax]");

    p_Dsec = Dsec;
}

//------------------------------------------------------------------
//TOLERANCE PROPERTIES IN TEXT FORMAT:

AnsiString TRoboticPositioner::getEoText(void) const
{
    return FloatToStr(getEo());
}
void TRoboticPositioner::setEoText(const AnsiString &S)
{
    try {
        setEo(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TRoboticPositioner::getEpText(void) const
{
    return FloatToStr(getEp());
}
void TRoboticPositioner::setEpText(const AnsiString &S)
{
    try {
        setEp(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TRoboticPositioner::getTstopText(void) const
{
    return FloatToStr(getTstop());
}
void TRoboticPositioner::setTstopText(const AnsiString &S)
{
    try {
        setTstop(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TRoboticPositioner::getTshiffText(void) const
{
    return FloatToStr(getTshiff());
}
void TRoboticPositioner::setTshiffText(const AnsiString &S)
{
    try {
        setTshiff(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TRoboticPositioner::getSPMaddText(void) const
{
    return FloatToStr(getSPMadd());
}
void TRoboticPositioner::setSPMaddText(const AnsiString &S)
{
    try {
        setSPMadd(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//STATUS PROPERTIES IN TEXTFORMAT:

AnsiString TRoboticPositioner::getDisabledText(void) const
{
    return BoolToStr(Disabled, true);
}
void TRoboticPositioner::setDisabledText(const AnsiString &S)
{
    try {
        Disabled = StrToBool(S);
    } catch(...) {
        throw;
    }
}
AnsiString TRoboticPositioner::getFaultProbabilityText(void) const
{
    return FloatToStr(getFaultProbability());
}
void TRoboticPositioner::setFaultProbabilityText(const AnsiString &S)
{
    try {
        setFaultProbability(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TRoboticPositioner::getFaultTypeText(void) const
{
    return FaultTypeToStr(FaultType);
}
void TRoboticPositioner::setFaultTypeText(const AnsiString &S)
{
    try {
        FaultType = StrToFaultType(S);
    } catch(...) {
        throw;
    }
}

AnsiString TRoboticPositioner::getControlModeText(void) const
{
    return ControlModeToStr(ControlMode);
}
void TRoboticPositioner::setControlModeText(AnsiString &S)
{
    try {
        ControlMode = StrToControlMode(S);
    } catch(...) {
        throw;
    }
}

AnsiString TRoboticPositioner::getOperativeText(void) const
{
    return BoolToStr(getOperative(), true);
}

//---------------------------------------------------------------------------
//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TRoboticPositioner::getToleranceText(void) const
{
    AnsiString S;

    //PROPIEDADES DE SEGURIDAD:

    S = "R/W:\r\n";

    S += AnsiString("    Eo = ")+getEoText()+AnsiString("\r\n");
    S += AnsiString("    Ep = ")+getEpText()+AnsiString("\r\n");
    S += AnsiString("    Tstop = ")+getTstopText()+AnsiString("\r\n");
    S += AnsiString("    Tshiff = ")+getTshiffText()+AnsiString("\r\n");
    S += AnsiString("    SPMadd = ")+getSPMaddText();

    return S;
}
AnsiString TRoboticPositioner::getStatusText(void) const
{
    AnsiString S;

    //PROPIEDADES DE ESTADO:

    S = "R/W:\r\n";

    S += AnsiString("    Disabled = ")+getDisabledText()+AnsiString("\r\n");
    S += AnsiString("    FaultProbability = ")+getFaultProbabilityText()+AnsiString("\r\n");
    S += AnsiString("    FaultType = ")+getFaultTypeText()+AnsiString("\r\n");
    S += AnsiString("    ControlMode = ")+getControlModeText()+AnsiString("\r\n");

    S += "R:\r\n";

    S += AnsiString("    Operative = ")+getOperativeText();

    return S;
}

AnsiString TRoboticPositioner::getAllText(void) const
{
    AnsiString S;

    S = AnsiString("ActuatorAddress: ")+getActuatorAddressText()+AnsiString("\r\n");
    S += AnsiString("CMFAddress: ")+getCMFAddressText()+AnsiString("\r\n");
    S += AnsiString("Tolerance:\r\n")+StrIndent(getToleranceText())+AnsiString("\r\n");
    S += AnsiString("Status:\r\n")+StrIndent(getStatusText());

    return S;
}
AnsiString TRoboticPositioner::getInstanceText(void) const
{
    AnsiString S;

    S += AnsiString("ActuatorInstance:\r\n")+StrIndent(getActuator()->getInstanceText())+AnsiString("\r\n");
    S += AnsiString("CMFInstance:\r\n")+StrIndent(CMF.getInstanceText())+AnsiString("\r\n");

    S += "Tolerances:\r\n";
    S += AnsiString("    Eo = ")+getEoText()+AnsiString("\r\n");
    S += AnsiString("    Ep = ")+getEpText()+AnsiString("\r\n");
    S += AnsiString("    Tstop = ")+getTstopText()+AnsiString("\r\n");
    S += AnsiString("    Tshiff = ")+getTshiffText()+AnsiString("\r\n");
    S += AnsiString("    SPMadd = ")+getSPMaddText()+AnsiString("\r\n");

    S += "Status:\r\n";
    S += AnsiString("    Disabled = ")+getDisabledText()+AnsiString("\r\n");
    S += AnsiString("    FaultProbability = ")+getFaultProbabilityText()+AnsiString("\r\n");
    S += AnsiString("    FaultType = ")+getFaultTypeText();

    return S;
}
void TRoboticPositioner::setInstanceText(const AnsiString &S)
{
    try {
        //contruye una variable tampón
        TRoboticPositioner aux(this);
        TRoboticPositioner *RP = &aux;
        //lee la instancia y la asigna a la variable tampón
        int i = 1;
        readInstance((TRoboticPositioner*&)RP, S, i);

        //avanza el índice i hasta la próxima posición que no contenga un separador
        StrTravelSeparatorsIfAny(S, i);
        //si el índice i indica a algún caracter de la cadena S
        if(i <= S.Length())
            //indica que la cadena S solo debería contener el valor para una instancia
            throw EImproperArgument("string S should contain the instance value only");

        //asigna la variable tampón
        clone(RP);

    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//compara los identificadores de dos posicionadores
int  TRoboticPositioner::compareIds(TRoboticPositioner *FP1,
                                    TRoboticPositioner *FP2)
{
    return TActuator::CompareIds(
                FP1->getActuator(), FP2->getActuator());
}

//imprime el identificador de un posicionador
void  TRoboticPositioner::printId(AnsiString &S, TRoboticPositioner *FP)
{
    return TActuator::PrintId(S, FP->getActuator());
}

//Los métodos estáticos:
//      CompareIds
//      PrintId
//serán apuntados en la lista de posicionadores adyacentes
//para permitir su ordenacíon en función de los identificadores
//y la impresión de los mismos.

//imprime los valores de las propiedades de origen de un posicionador
//(Id, x0, y0, thetaO1) al final de una cadena de texto
//en formato fila de texto
void  TRoboticPositioner::printOriginsRow(AnsiString& S,
                                          TRoboticPositioner *FP)
{
    TActuator::printOriginsRow(S, FP->getActuator());
}
//lee los valores de las propiedades de origen de un posicionador
//(Id, x0, y0, thetaO1) desde la posición indicada de una cadena
//de texto, en formato fila de texto
void  TRoboticPositioner::readOriginsRow(TRoboticPositioner *FP,
                                         const AnsiString& S, int &i)
{
    TActuator *A = FP->getActuator();
    TActuator::readOriginsRow(A, S,  i);
}

//imprime los valores de las propiedades de posición de un posicionador
//(Id, x3, y3) al final de una cadena de texto
//en formato fila de texto
void  TRoboticPositioner::printPositionP3Row(AnsiString& S,
                                             TRoboticPositioner *FP)
{
    TActuator::printPositionP3Row(S, FP->getActuator());
}
//lee los valores de las propiedades de posición de un posicionador
//(Id, x3, y3) desde la posición indicada de una cadena
//de texto, en formato fila de texto
void  TRoboticPositioner::readPositionP3Row(TRoboticPositioner* &FP,
                                            const AnsiString& S, int &i)
{
    TActuator *A = FP->getActuator();
    TActuator::readPositionP3Row(A, S,  i);
}
//imprime los valores de las propiedades de posición de un posicionador
//(Id, x_3, y_3) al final de una cadena de texto
//en formato fila de texto
void  TRoboticPositioner::printPositionP_3Row(AnsiString& S,
                                             TRoboticPositioner *FP)
{
    TActuator::printPositionP_3Row(S, FP->getActuator());
}
//imprime los valores de las propiedades de posición de un posicionador
//(Id, p_1, p___3) al final de una cadena de texto
//en formato fila de texto
void  TRoboticPositioner::printPositionPPARow(AnsiString& S,
                                              TRoboticPositioner *FP)
{
    TActuator::printPositionPPARow(S, FP->getActuator());
}

//lee una instancia en una cadena
void  TRoboticPositioner::readInstance(TRoboticPositioner* &RP,
                                       const AnsiString& S, int &i)
{
    //CHECK THE PRECONDITIONS:

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //MAKE ACTIONS:

    //estado de la máquina de estados de lectura
    //      0: esperando etiqueta "ActuatorInstance:"
    //      1: esperando instancia de Actuator
    //      2: esperando etiqueta "CMF:"
    //      3: esperando instancia de CMF
    //      4: esperando etiqueta "Tolerances:"
    //      5: esperando separador y asignación a Eo
    //      6: esperando separador y asignación a Ep
    //      7: esperando separador y asignación a Tstop
    //      8: esperando separador y asignación a Tshiff
    //      9: esperando separador y asignación a SPMadd
    //      10: esperando etiqueta "Status:"
    //      11: esperando asignación a Disabled
    //      12: esperando asignación a FaultProbability
    //      13: esperando asignación a FaultType
    //      14: instancia de posicionador leida con éxito
    int status = 0;

    //variables tampón
    TRoboticPositioner t_RP(RP);

    do {
        switch(status) {
        case 0: //esperando etiqueta "ActuatorInstance:"
            try {
            StrTravelLabel("ActuatorInstance:", S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 1: //esperando instancia de Actuator
            try {
            StrTravelSeparators(S, i);
            TActuator *aux = t_RP.getActuator();
            TActuator::readInstance(aux, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;

        case 2: //esperando etiqueta "CMFInstance:"
            try {
            StrTravelLabel("CMFInstance:", S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 3: //esperando instancia de CMF
            try {
            StrTravelSeparators(S, i);
            TComposedMotionFunction *aux = &t_RP.CMF;
            TComposedMotionFunction::ReadInstance(aux, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 4: //esperando etiqueta "Tolerances:"
            try {
            StrTravelLabel("Tolerances:", S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 5: //esperando separador y asignación a Eo
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("Eo", S, i);
            StrTravelLabel("=", S, i);
            double aux;
            StrReadFloat(aux, S,i);
            t_RP.setEo(aux);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 6: //esperando separador y asignación a Ep
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("Ep", S, i);
            StrTravelLabel("=", S, i);
            double aux;
            StrReadFloat(aux, S,i);
            t_RP.setEp(aux);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 7: //esperando separador y asignación a Tstop
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("Tstop", S, i);
            StrTravelLabel("=", S, i);
            double aux;
            StrReadFloat(aux, S,i);
            t_RP.setTstop(aux);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 8: //esperando separador y asignación a Tshiff
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("Tshiff", S, i);
            StrTravelLabel("=", S, i);
            double aux;
            StrReadFloat(aux, S,i);
            t_RP.setTshiff(aux);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 9: //esperando separador y asignación a SPMadd
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("SPMadd", S, i);
            StrTravelLabel("=", S, i);
            double aux;
            StrReadFloat(aux, S,i);
            t_RP.setSPMadd(aux);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 10: //esperando etiqueta "Status:"
            try {
            StrTravelLabel("Status:", S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 11: //esperando asignación a Disabled
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("Disabled", S, i);
            StrTravelLabel("=", S, i);
            bool aux;
            StrReadBool(aux, S, i);
            t_RP.Disabled = aux;
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 12: //esperando asignación a FaultProbability
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("FaultProbability", S, i);
            StrTravelLabel("=", S, i);
            double aux;
            StrReadFloat(aux, S, i);
            t_RP.setFaultProbability(aux);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 13: //esperando asignación a FaultType
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("FaultType", S, i);
            StrTravelLabel("=", S, i);
            TFaultType aux;
            StrReadFaultType(aux, S, i);
            t_RP.FaultType = aux;
        }catch(...) {
            throw;
        }
            status++;
            break;

        }
        //mientras no se haya leido la instancia de posicionador con éxito
    } while(status < 14);

    //asigna la variable tampón
    RP->clone(&t_RP);
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONSTRUCCIÓN, DESTRUCCIÓN Y COPIA:

//build a RP by default
TRoboticPositioner::TRoboticPositioner(void) :
    //inicializa las propiedades de tolerancia
    p_Eo(MEGARA_Eo), p_Ep(MEGARA_Ep),
    p_Tstop(MEGARA_Tstop), p_Tshiff(MEGARA_Tshiff),
    p_SPMadd(MEGARA_SPMadd),
    //inicializa las propiedades de estado
    p_FaultProbability(0), p_DsecMax(1), p_Dsec(p_DsecMax),
    //contruye e inicializa las propiedades de control
    CMF(),
    //inicializa las propiedades de estado
    Disabled(false), FaultType(ftUnk),
    ControlMode(cmSinc), MPturn(), MPretraction(),
    Dfmin(std::numeric_limits<double>::max())
{
    //contruye el actuador del posicionador
    //con el identificador 0
    //en la posición {0, 0}
    p_Actuator = new TActuator(0, TDoublePoint(0, 0), 0);

    //inicializa las propiedades de control
    CMF.setLabel("CMF");

    //añade el posicionador a la lista de contruidos
    Builts.Add(this);
}
//build a RP with the indicated values
//if Id is less 1 throw an exception EImproperArgument
TRoboticPositioner::TRoboticPositioner(int Id, TDoublePoint P0,
                                       double thetaO1) :
    //inicializa las propiedades de tolerancia
    p_Eo(MEGARA_Eo), p_Ep(MEGARA_Ep),
    p_Tstop(MEGARA_Tstop), p_Tshiff(MEGARA_Tshiff),
    p_SPMadd(MEGARA_SPMadd),
    //inicializa las propiedades de estado
    p_FaultProbability(0), p_Dsec(1),
    //contruye e inicializa las propiedades de control
    CMF(),
    //construye e inicializa las propiedades de estado
    Disabled(false), FaultType(ftUnk),
    ControlMode(cmSinc), MPturn(), MPretraction(),
    Dfmin(std::numeric_limits<double>::max())
{
    //el número de identificación Id debe ser mayor que cero
    if(Id < 1)
        throw EImproperArgument("identification number Id should be upper zero");

    //WARNING: duplicity of identifiers is allowed.

    //contruye el actuador del posicionador
    p_Actuator = new TActuator(Id, P0, thetaO1);

    //inicializa las propiedades de control
    CMF.setLabel("CMF");

    //añade el posicionador a la lista de contruidos
    Builts.Add(this);
}

//copy all tolerance properties of a RP
void TRoboticPositioner::copyTolerance(const TRoboticPositioner *RP)
{
    //check the precondition
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    //copia las tolerancias
    p_Eo = RP->getEo();
    p_Ep = RP->getEp();
    p_Tstop = RP->getTstop();
    p_Tshiff = RP->getTshiff();
    p_SPMadd = RP->getSPMadd();
}
//copy all status properties of a RP
void TRoboticPositioner::copyStatus(const TRoboticPositioner *RP)
{
    //check the precondition
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    Disabled = RP->Disabled;
    setFaultProbability(RP->getFaultProbability());
    FaultType = RP->FaultType;
    ControlMode = RP->ControlMode;
    MPturn.Clone(RP->MPturn);
    MPretraction.Clone(RP->MPretraction);
    p_DsecMax = RP->p_DsecMax;
    p_Dsec = RP->p_Dsec;
    Dfmin = RP->Dfmin;
}

//copy all properties of a RP
void TRoboticPositioner::clone(const TRoboticPositioner *RP)
{
    //check the precondition
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    //copia las propiedades
    getActuator()->clone(RP->getActuator());
    CMF.Copy(RP->CMF);
    copyTolerance(RP);
    copyStatus(RP);
}
TRoboticPositioner& TRoboticPositioner::operator=(const TRoboticPositioner& RP)
{
    //copia las propiedades
    getActuator()->clone(RP.getActuator());
    CMF.Copy(RP.CMF);
    copyTolerance(&RP);
    copyStatus(&RP);

    //devuelve la referenciaa este posicionador
    return *this;
}

//build a clon of a RP
TRoboticPositioner::TRoboticPositioner(const TRoboticPositioner *RP)
{
    //check the precondition
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    //build a clone of the actuator of the RP
    p_Actuator = new TActuator(RP->getActuator());
    //copy all other properties of the RP
    clone(RP);

    //añade el posicionador a la lista de contruidos
    Builts.Add(this);
}

//copy all properties of a RP
//except (P0, Id, Id1, Id2)
void TRoboticPositioner::apply(const TRoboticPositioner *RP)
{
    //check the precondition
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    TDoublePoint P0 = getActuator()->getP0();
    int Id = getActuator()->getId();
    int Id1 = CMF.getId1();
    int Id2 = CMF.getId2();

    clone(RP);

    getActuator()->setP0(P0);
    getActuator()->setId(Id);
    CMF.setId1(Id1);
    CMF.setId2(Id2);
}

//free the dynamic memory and delete the RP in Builts
//if the RP isn't in Built throw EImproperCall
TRoboticPositioner::~TRoboticPositioner()
{
    //debe haber algún objeto para destruir
    if(Builts.getCount() < 1)
        throw EImproperCall("there isn't an object to destroy");

    //busca el objeto a destruir en la lista
    int i = 0;
    while(i<Builts.getCount() && Builts[i]!=this)
        i++;

    //si no ha encontrado el objeto
    if(i >= Builts.getCount())
        //indica que está intentando destruir un objeto no contruido
        throw EImproperCall(AnsiString("destruction attempt of non built object: ")+
                            IntToHex(reinterpret_cast<intptr_t>(this), 8));

    //destruye el actuador
    delete p_Actuator;

    //borra el puntero de la lista de construídos
    Builts.Delete(i);
}

//---------------------------------------------------------------------------
//MÉTODOS PARA CALCULAR LOS VALORES RECOMENDADOS DE
//LAS COMPONENTES DE SPM:

//SPM recovery in mm:
//  SPMrec = (CMF.vmaxabs1*Actuator->rbs*Actuator->r_max +
//      CMF.vmaxabs2*Actuator->Arm->rbs*Actuator->Arm->L1V)*Tstop;
double TRoboticPositioner::SPMrec(void) const
{
    return (CMF.getvmaxabs1()*getActuator()->getrbs()*getActuator()->getr_max() +
            CMF.getvmaxabs2()*getActuator()->getArm()->getrbs()*getActuator()->getArm()->getL1V())*getTstop();
}

//static SPM in mm:
//  SPMsta = Eo*Actuator->r_max + Ep;
double TRoboticPositioner::SPMsta(void) const
{
    return getEo()*getActuator()->getr_max() + getEp();
}

//dynamic SPM:
//  SPMdyn = (CMF.vmaxabs1*Actuator->rbs*Actuator->r_max +
//      CMF.vmaxabs2*Actuator->Arm->rbs*Actuator->Arm->L1V)*Tshiff + SPMadd;
double TRoboticPositioner::SPMdyn(void) const
{
    return (CMF.getvmaxabs1()*getActuator()->getrbs()*getActuator()->getr_max() +
            CMF.getvmaxabs2()*getActuator()->getArm()->getrbs()*getActuator()->getArm()->getL1V())*getTshiff() +
            getSPMadd();
}

//--------------------------------------------------------------------------
//MÉTODOS DE ASIGNACIÓN CONJUNTA:

//asigna conjuntamente los márgenes de segudidad
//      (Eo, Ep, Tstop, Tshiff, SPMadd)
void TRoboticPositioner::setTolerances(double Eo, double Ep,
                                       double Tstop, double Tshiff,
                                       double SPMadd)
{
    //el valor de Eo debe ser no negativo
    if(Eo < 0)
        throw EImproperArgument("Eo value should be nonnegative");

    //el valor de Ep debe ser no negativo
    if(Ep < 0)
        throw EImproperArgument("Ep value should be nonnegative");

    //el valor de Tstop debe ser no negativo
    if(Tstop < 0)
        throw EImproperArgument("Tstop value should be nonnegative");

    //el valor de Tshiff debe ser no negativo
    if(Tshiff < 0)
        throw EImproperArgument("Tshiff value should be nonnegative");

    //asigna los nuevos valores
    p_Eo = Eo;
    p_Ep = Ep;
    p_Tstop = Tstop;
    p_Tshiff = Tshiff;
    p_SPMadd = SPMadd;

    //asimila (Eo, Ep, Tstop, Tshiff)
    calculateSPMcomponents();
}

//--------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//A partir de:
//      (Eo, Ep, Tstop, Tshiff, SPMadd)
//      FP->rmax
//determina:
//      (SPMrec, SPMsta, SPMdyn)
void TRoboticPositioner::calculateSPMcomponents(void)
{
    //CALULA LAS COMPONENTES DE SPM:

    double aux = SPMsta();
    getActuator()->setSPMcomponents(SPMrec(), aux, SPMdyn());
    getActuator()->getBarrier()->setSPM(aux);
}

//asigna la componente de SPM para absorber el desplazamiento
//por corrección del offset
//      Actuator->SPMoff = PAem*Actuator->rmax + Pem mm
void TRoboticPositioner::setSPMoff(double PAem, double Pem)
{
    //el valor de PAem debe ser no negativo
    if(PAem < 0)
        throw EImproperArgument("PAem value should be nonnegative");

    //el valor de Pem debe ser no negativo
    if(Pem < 0)
        throw EImproperArgument("Pem value should be nonnegative");

    //calculates and assign the value
    getActuator()->setSPMoff(PAem*getActuator()->getrmax() + Pem);
}

//--------------------------------------------------------------------------
//METHODS TO CHECK THE CONFIGURATION:

//calculate the maximun absolute velocity of rotor 1 in rad/s
double TRoboticPositioner::wmaxabs1(void)
{
    double wmaxabs1;
    switch(CMF.getMFM()) {
    case mfmSquare:
        wmaxabs1 = CMF.getSF1()->getvmaxabs()/getActuator()->getSB1()*M_2PI;
        break;
    case mfmRamp:
        wmaxabs1 = CMF.getRF1()->getvmaxabs()/getActuator()->getSB1()*M_2PI;
        break;
    }
    return wmaxabs1;
}

//calculate the maximun absolute velocity of rotor 2 in rad/s
double TRoboticPositioner::wmaxabs2(void)
{
    double wmaxabs2;
    switch(CMF.getMFM()) {
    case mfmSquare:
        wmaxabs2 = CMF.getSF2()->getvmaxabs()/getActuator()->getArm()->getSB2()*M_2PI;
        break;
    case mfmRamp:
        wmaxabs2 = CMF.getRF2()->getvmaxabs()/getActuator()->getArm()->getSB2()*M_2PI;
        break;
    }
    return wmaxabs2;
}

//--------------------------------------------------------------------------
//INSTRUCTION METHODS:

//program turn of rotor 1 from actual position to p_1fin
//if rotor 1 quantifier is disabled:
//  throw and exception EImproperCall
//if the p_1fin is out rotor 1 domain:
//  throw an exception EImproperArgument
void TRoboticPositioner::M1(double p_1fin)
{
    //check the preconditions
    if(getActuator()->getQuantify_() != true)
        throw EImproperCall("rotor 1 quantifier should be enabled");
    if(getActuator()->isntInDomainp_1(p_1fin))
        throw EImproperArgument("position p_1fin should be in rot 1 domain");

    //Rotor 1 quantifier shall be enabled to avoid
    //the numerical error in getActuator()->getp_1().

    //get the actual position of rotor 1
    double p_1sta = getActuator()->getp_1();

    //program turn of rotor 1 from actual position to final position
    CMF.ProgramMF1(p_1sta, p_1fin);
}
//program turn of rotor 2 from actual position to p___3fin
//if rotor 2 quantifier is disabled:
//  throw and exception EImproperCall
//if the p___3fin is out rotor 2 domain:
//  throw an exception EImproperArgument
void TRoboticPositioner::M2(double p___3fin)
{
    //check the preconditions
    if(getActuator()->getArm()->getQuantify___() != true)
        throw EImproperCall("rotor 2 quantifier should be enabled");
    if(getActuator()->getArm()->isntInDomainp___3(p___3fin))
        throw EImproperArgument("position p___3fin should be in rot 2 domain");

    //Rotor 2 quantifier shall be enabled to avoid
    //the numerical error in getActuator()->getp_1().

    //get the actual position of rotor 2
    double p___3sta = getActuator()->getArm()->getp___3();

    //program turn of rotor 2 from actual position to final position
    CMF.ProgramMF2(p___3sta, p___3fin);
}
//program turns of rotors 1 and 2 from actual position to (p_1fin, p___3fin)
//if rotor 1 quantifier or rotor 2 quantifier is disabled:
//  throw and exception EImproperCall
//if the p_1fin is out rotor 1 domain or p___3fin is out rotor 2 domain:
//  throw an exception EImproperArgument
void TRoboticPositioner::MM(double p_1fin, double p___3fin)
{
    //check the preconditions
    if(getActuator()->getQuantify_() != true || getActuator()->getArm()->getQuantify___() != true)
        throw EImproperCall("rotor 1 quantifier and rotor 2 quantifier should be enabled");
    if(getActuator()->isntInDomainp_1(p_1fin) || getActuator()->getArm()->isntInDomainp___3(p___3fin))
        throw EImproperArgument("PPA (p_1fin, p___3fin) should be in rotor domain");

    //Quantifier of the rotors shall be enabled to avoid
    //the numerical error in actual position.

    //get the actual position of rotor 1
    double p_1sta = getActuator()->getp_1();
    //get the actual position of rotor 2
    double p___3sta = getActuator()->getArm()->getp___3();

    //program the turn of the rotors
    CMF.ProgramBoth(p_1sta, p___3sta, p_1fin, p___3fin);
}

//program turn of rotor 1 from p_1sta to p_1fin
//if the p_1sta or p_1fin is out rotor 1 domain:
//  throw an exception EImproperArgument
void TRoboticPositioner::M1(double p_1sta, double p_1fin)
{
    //check theprecondition
    if(getActuator()->isntInDomainp_1(p_1sta) || getActuator()->isntInDomainp_1(p_1fin))
        throw EImproperArgument("position angles p_1sta and p_1fin should be in rot 1 domain");

    //program the turn of rotor 1
    CMF.ProgramMF1(p_1sta, p_1fin);
}
//program turn of rotor 2 from p___3sta to p___3fin
//if the p___3sta or p___3fin is out rotor 2 domain:
//  throw an exception EImproperArgument
void TRoboticPositioner::M2(double p___3sta, double p___3fin)
{
    //check theprecondition
    if(getActuator()->getArm()->isntInDomainp___3(p___3sta) || getActuator()->getArm()->isntInDomainp___3(p___3fin))
        throw EImproperArgument("position angles p___3sta and p___3fin should be in rot 2 domain");

    //program the turn of rotor 1
    CMF.ProgramMF2(p___3sta, p___3fin);
}
//program turn of rotor 1 from p_1sta to p_1fin, and
//program turn of rotor 2 from p___3sta to p___3fin
//if the p_1sta or p_1fin is out rotor 1 domain, or:
//if the p___3sta or p___3fin is out rotor 2 domain:
//  throw an exception EImproperArgument
void TRoboticPositioner::MM(double p_1sta, double p___3sta,
                            double p_1fin, double p___3fin)
{
    //CHECK THE PRECONDITIONS:

    if(getActuator()->isntInDomainp_1(p_1sta))
        throw EImproperArgument("position angle p_1sta should be in the rot 1 domain");

    if(getActuator()->isntInDomainp_1(p_1fin))
        throw EImproperArgument("position angle p_1fin should be in the rot 1 domain");

    if(getActuator()->getArm()->isntInDomainp___3(p___3sta))
        throw EImproperArgument("position angle p___3sta should be in the rot 2 domain");

    if(getActuator()->getArm()->isntInDomainp___3(p___3fin))
        throw EImproperArgument("position angle p___3fin should be in the rot 2 domain");

    //MAKE ACTIONS:

    //program the turn of the rotors
    CMF.ProgramBoth(p_1sta, p___3sta, p_1fin, p___3fin);
}

//stop displacement and clear the programmed gesture
//if there isn't programmed a gesture:
//  throw an exception EImproperCall
void TRoboticPositioner::SP(void)
{
    //check the precondition
    if(CMF.getMF1()==NULL && CMF.getMF2()==NULL)
        throw EImproperCall("should have a programmed gesture");

    //clear the program
    CMF.ClearProgram();
}

//set an instruction
//if instruction is empty:
//  throw an exception EImproperArgument
void TRoboticPositioner::setInstruction(const TInstruction &Instruction)
{
    //Nombre del comando y número de argumentos:
    //      ""      0
    //      "MS"    0
    //      "MA"    0
    //      "M1"    1
    //      "M2"    1
    //      "MM"    2
    //      "ST"    0
    //      "SP"    0

    //la instruccion 'Instruction' no debería estar vacía
    if(Instruction.getName() == "")
        throw EImproperArgument("instruction 'Instruction' should not be empty");

    try {
        //execute the instruction
        if(Instruction.getName() == "MS") {
            ControlMode = cmSinc;

        } else if(Instruction.getName() == "MA") {
            ControlMode = cmAsinc;

        } else if(Instruction.getName() == "M1") {
            //enable temporarity the rotor quantifier to accomplish precondition
            getActuator()->pushQuantify_();
            getActuator()->setQuantify_(true);
            //program the gesture
            M1(Instruction.Args.getFirst());
            //restore the status of rotor quantifier
            getActuator()->restoreAndPopQuantify_();

        } else if(Instruction.getName() == "M2") {
            //enable temporarity the rotor quantifier to accomplish precondition
            getActuator()->getArm()->pushQuantify___();
            getActuator()->getArm()->setQuantify___(true);
            //program the gesture
            M2(Instruction.Args.getFirst());
            //restore the status of rotor quantifier
            getActuator()->getArm()->restoreAndPopQuantify___();

        } else if(Instruction.getName() == "MM") {
            //enable temporarity the rotor quantifier to accomplish precondition
            getActuator()->pushQuantifys();
            getActuator()->setQuantify_(true);
            getActuator()->getArm()->setQuantify___(true);
            //program the gesture
            MM(Instruction.Args.getFirst(), Instruction.Args[1]);
            //restore the status of rotor quantifier
            getActuator()->restoreAndPopQuantifys();

        } else if(Instruction.getName() == "SP") {
            SP();

        } else
            //indicates that the instruction should be known
            throw EImpossibleError("instruction name should be known");

    } catch(...) {
        throw; //relanza la excepción
    }
}

//get the programmed instruction
//if there isn't progrmmed a gesture:
//  throw an exception EImproperCall
void TRoboticPositioner::getInstruction(TInstruction &Instruction) const
{
    if(CMF.getMF1()!=NULL && CMF.getMF2()!=NULL) {
        Instruction.setName("MM");
        Instruction.Args.setCount(2);
        Instruction.Args[0] = CMF.getMF1()->getpfin();
        Instruction.Args[1] = CMF.getMF2()->getpfin();
    } else if(CMF.getMF1() != NULL) {
        Instruction.setName("M1");
        Instruction.Args.setCount(1);
        Instruction.Args[0] = CMF.getMF1()->getpfin();
    } else if(CMF.getMF2() != NULL) {
        Instruction.setName("M2");
        Instruction.Args.setCount(1);
        Instruction.Args[0] = CMF.getMF2()->getpfin();
    } else //CMF.MF1==NULL && CMF.MF2==NULL
        //indica que debería haber alguna funcion de movimiento programada
        throw EImproperCall("should be programmed some motion function");
}

//get the instruction to move:
//  rotor 1 from actualposition to p_1first
//  rotor 2 from actualposition to p___3first
void TRoboticPositioner::getInstructionToGoToTheOrigin(TInstruction& Instruction) const
{
    //determine the position of the origin of each rotor
    double p_1origin = max(0., ceil(getActuator()->getp_1min()));
    double p___3origin = max(0., ceil(getActuator()->getArm()->getp___3min()));

    //determine if rotor 1 is out of the origin
    bool rot1out;
    if(getActuator()->getp_1() != p_1origin)
        rot1out = true;
    else
        rot1out = false;

    //determine if rotor 2 is out of the origin
    bool rot2out;
    if(getActuator()->getArm()->getp___3() != p___3origin)
        rot2out = true;
    else
        rot2out = false;

    //set the instruction according the status of the rotors
    if(rot1out && rot2out) {
        Instruction.setName("MM");
        Instruction.Args.setCount(2);
        Instruction.Args[0] = p_1origin;
        Instruction.Args[1] = p___3origin;
    } else if(rot1out) {
        Instruction.setName("M1");
        Instruction.Args.setCount(1);
        Instruction.Args[0] = p_1origin;
    } else if(rot2out) {
        Instruction.setName("M2");
        Instruction.Args.setCount(1);
        Instruction.Args[0] = p___3origin;
    } else //!rot1out && !rot2out
        throw EImproperCall("some rotor should be out their origin");
}
//set the position indicated in an instruction
void TRoboticPositioner::setPosition(const TInstruction& Instruction)
{
    if(Instruction.getName()!="M1" && Instruction.getName()!="M2" && Instruction.getName()!="MM")
        throw EImproperArgument("instruction should be M1, M2 or MM");

    if(Instruction.getName() == "M1") {
        double p_1 = Instruction.Args.getFirst();
        getActuator()->setp_1(p_1);

    } else if(Instruction.getName() == "M2") {
        double p___3 = Instruction.Args.getFirst();
        getActuator()->getArm()->setp___3(p___3);

    } else if(Instruction.getName() == "MM") {
        double p_1 = Instruction.Args.getFirst();
        double p___3 = Instruction.Args[1];
        getActuator()->setPositionPPASteps(p_1, p___3);

    } else
        //indicates that the instruction should be known
        throw EImpossibleError("lateral effect");
}

//METHODS TO PROGRAM GESTURES:

//program the abatement of the arm to
//the more closer-stable security position
void TRoboticPositioner::programTurnArmToSecurityPosition(void)
{
    //translate to steps the security position limit of the rotor 2
    double p___3saf = getActuator()->getArm()->getF().Image(getActuator()->gettheta___3saf());

    //get the first stable position less to the limit
    p___3saf = floor(p___3saf);

    //check if there is a security position
    if(p___3saf < 0)
        throw EImproperCall("there isn't a security position");

    //set the firs stable position less than the security position limit
    M2(p___3saf);
}

//program the retraction of the arm to
//the more closer-stable security position
//if rotor 1 quantifier or rotor 2 quantifier is disabled:
//  throw and exception EImproperCall
void TRoboticPositioner::programRetractionToSecurityPosition(void)
{
    //check the preconditions
    if(getActuator()->getQuantify_() != true || getActuator()->getArm()->getQuantify___() != true)
        throw EImproperCall("rotor 1 quantifier and rotor 2 quantifier should be enabled");

    //DETERMINA LA POSICIÓN A LA QUE DEBE  MOVERSE ROT 2:

    //traduce la posisión de seguridad de rot 2 a pasos
    double p___3saf = getActuator()->getArm()->getF().Image(getActuator()->gettheta___3saf());
    //determina la primra posición de seguridad estable de rot 2
    p___3saf = floor(p___3saf);

    //check if there is a security position
    if(p___3saf < 0)
        throw EImproperCall("there isn't a security position");

    //DETERMINA LA POSICIÓN A LA QUE DEBE MOVERSE ROT 1:

    double theta_1; //posición en radianes a la que debe moverse rot 1

    //determina la distancia en pasos a recorrer por rot 2 en pasos
    double dp2 = getActuator()->getArm()->getp___3() -  p___3saf;
    //traduce la distancia a radianes
    double dt2 = getActuator()->getArm()->getG().Image(dp2);
    //divide la distancia entre dos
    double dt1 = dt2/2;

    //si rot 1 está lo bastante alejado del límite
    if(getActuator()->gettheta_1() >= dt1) {
        //calcula la nueva posición de rot 1
        theta_1 = getActuator()->gettheta_1() - dt1;
    } else { //si rot 1 está demasiado cerca del límite
        //rot 1 debe moverse al origen
        theta_1 = 0;
    }
    //traduce la nueva posición a pasos
    double p_1 = getActuator()->getF().Image(theta_1);
    //determines the more closer estable position
    p_1 = round(p_1);

    //check if there is a security position
    if(p_1 < 0)
        throw EImproperCall("there isn't a security position");

    //PROGRAM THE MOVEMENT OF THE ROTORS:

    //program the movement to go to the stable position
    MM(p_1, p___3saf);
}
//program the extension of the fiber from the actual position
//to the more spread position
//if rotor 1 quantifier or rotor 2 quantifier is disabled:
//  throw and exception EImproperCall
void TRoboticPositioner::programExtensionToMoreSpreadPosition(void)
{
    //check the preconditions
    if(getActuator()->getQuantify_() != true || getActuator()->getArm()->getQuantify___() != true)
        throw EImproperCall("rotor 1 quantifier and rotor 2 quantifier should be enabled");

    double theta___3ini = getActuator()->getArm()->gettheta___3();
    double theta___3fin = M_PI;
    double dt2 = theta___3fin - theta___3ini;
    double dt1 = dt2/2;
    double theta_1ini = getActuator()->gettheta_1();
    double theta_1fin = theta_1ini + dt1;

    //get the final more closer stable position
    double p_1nsp, p___3nsp;
    getActuator()->getNearestStablePosition(p_1nsp, p___3nsp, theta_1fin, theta___3fin);

    //program the movement to go to the stable position
    MM(p_1nsp, p___3nsp);
}

//program turn of rotor 1 to theta_1
//if theta_1 is out rotor 1 domain:
//  throw an exception EImproperArgument
void TRoboticPositioner::programTurnCilinderTotheta_1(double theta_1)
{
    try{
        //translate from radians to steps
        int p_1 = getActuator()->getQ()[getActuator()->getF().Image(theta_1)];
        //program the gesture
        M1(p_1);

    } catch(...) {
        throw;
    }
}

//program go directly from actual position to (x_3, y_3)
//if (x_3, y_3) is out the scope of the RP:
//  throw an exception EImproperArgument
double TRoboticPositioner::programGoDirectlyToCartesianP_3(double x_3, double y_3)
{
    try {
        //traduce a polares en S1
        double r_3, theta_3;
        if(x_3!=0 && y_3!=0) {
            r_3 = Mod(x_3, y_3);
            theta_3 = Arg(x_3, y_3);
        } else {
            r_3 = 0;
            theta_3 = M_PI/2;
        }

        //determina las posiciones de los rotores para llevar P3 a (r_3, theta_3)
        //y determina si la posición está dentro del dominio
        double theta_1, theta___3;
        bool isindomine = getActuator()->anglesToGoP_3(theta_1, theta___3,
                                                       r_3, theta_3);

        //la posición (x_3, y_3) debería estar dentro del dominio de P_3
        if(!isindomine)
            throw EImproperArgument("position (x_3, y_3) should be in P_3 domain");

        //get themore closer stableposition
        //and calculate the distance fromthe observing position to the indictaed point
        double p_1nsp, p___3nsp;
        double distance = getActuator()->getNearestStablePosition(p_1nsp, p___3nsp, theta_1, theta___3);

        //programa el gesto
        MM(int(p_1nsp), int(p___3nsp));

        //return the distance
        return distance;

    } catch(...) {
        throw;
    }
}

//program go directly from actual position to (x3, y3)
//if (x3, y3) is out the scope of the RP:
//  throw an exception EImproperArgument
double TRoboticPositioner::programGoDirectlyToCartesianP3(double x3, double y3)
{
    //traduce las coordenadas cartesianas de S0 a S1
    TDoublePoint P_3 = getActuator()->S0recToS1rec(x3, y3);

    //program go directly using coordinates in S1
    //and calculate the distance fromthe observing position to the indictaed point
    double distance = programGoDirectlyToCartesianP_3(P_3.x, P_3.y);

    //return the distance
    return distance;
}

//MÉTODOS PARA MODIFICAR EL GESTO PROGRAMADO:

//invert thep rogrammed gesture in the time domain
void TRoboticPositioner::invertTime(void)
{
    //invert the composed motion function in the time domain
    CMF.InvertTime();
}

//METHODS TO ADD MESSAGE INSTRUCTIONS TO THE MP:

//Propose in (MPturn, MPretraction) a recovery program composed by
//one or two gestures:
//  1. Radial retraction to where it is possible.
//  2. Abatement of the arm to security position (if necessary).
//Preconditions:
//- The quantifiers of the rotors shall be enabled.
//- The rotor 2 shall be in unsecurity position.
//Postconditions:
//- The MPturn will be empty.
//- The MPretraction will contains the MP for retract the arm
//  to the first stable security position.
void TRoboticPositioner::proposeRecoveryProgram(void)
{
    //CHECK THE PRECONDITIONS:

    if(getActuator()->getQuantify_()!=true || getActuator()->getArm()->getQuantify___()!=true)
        throw EImproperCall("the quantifiers of the rotors shall be enabled");

    if(getActuator()->ArmIsInSafeArea())
        throw EImproperArgument("the rotor 2 shall be in unsecurity position");

    //MAKE ACTIONS:

    //initialize the output variables
    MPturn.Clear();
    MPretraction.Clear();

    //CALCULATES DE FINAL POSITIONS OF THE ROTORS (p_1fin, p___3fin, p___3saf):

    //determines the first stable security position of rot 2 in steps
    double theta___3saf = getActuator()->gettheta___3saf();
    double p___3saf = getActuator()->getArm()->getF().Image(theta___3saf);
    p___3saf = floor(p___3saf);

    //perform a rutinary check
    if(p___3saf < 0)
        throw EImpossibleError("rotor 2 position p___3saf should be nonnegative");

    //Note that if(p___3saf < RP->getActuator()->getArm()->getp___3first())
    //the MP will be proposed but it will not pass the validation process.

    //determines the necessary displacement of rot 2 in radians
    theta___3saf = getActuator()->getArm()->getG().Image(p___3saf);
    double theta___3ini = getActuator()->getArm()->gettheta___3();
    double dt2 = theta___3ini - theta___3saf;

    //calculates the necessary displacement of rotor 1 in radians
    double dt1 = dt2/2;

    //get the initial position of the rotors 1 in radians
    double theta_1ini = getActuator()->gettheta_1();

    //determines the final position of the rotors 1 in radians
    double theta_1fin;
    theta_1fin = theta_1ini - dt1;

    //determines the final position of rot 1 in steps
    double p_1fin = getActuator()->getF().Image(theta_1fin);
    p_1fin = round(p_1fin);

    //determines if the rotor 1 have enough space for retraction
    double p_1first = getActuator()->getp_1first();
    p_1first = ceil(p_1first); //purge lateral effect
    bool rot1_has_enough = (p_1fin >= p_1first);

    //calculates the final positions according the space available for the rotor 1
    double p___3fin;
    //if the rotor 1 has enpough space for execue the retraction
    if(rot1_has_enough)
        //determines the final position of rot 2 in steps
        p___3fin = p___3saf;

    //if the rotor 1 not has enpough space for execue the retraction
    else {
        //determines the final position of rot 1 in steps
        p_1fin = getActuator()->getp_1first();

        //calulates the final position of the rotors in radians
        theta_1fin = getActuator()->getG().Image(p_1fin);
        dt1 = theta_1ini - theta_1fin;
        dt2 = dt1*2;
        double theta___3fin = theta___3ini - dt2;

        //determines the final position of rot 2 in steps
        p___3fin = getActuator()->getArm()->getF().Image(theta___3fin);
        p___3fin = round(p___3fin);
    }

    //BUILD AND ADD THE INSTRUCTION FOR RADIAL RETRACTION:

    //build a message instruction and set it
    TMessageInstruction *MI = new TMessageInstruction();
    MI->setId(getActuator()->getId());
    MI->Instruction.setName("MM");
    MI->Instruction.Args.setCount(2);
    MI->Instruction.Args[0] = p_1fin;
    MI->Instruction.Args[1] = p___3fin;
    if(getDsec() < getDsecMax())
        MI->setComment1("Dsec = "+floattostr(getDsec())+" mm");

    //add the message instruction to the MP
    Positioning::TMessageList *ML = new Positioning::TMessageList();
    ML->Add(MI);
    MPretraction.Add(ML);

    //ADD INSTRUCTION FOR ABATEMENT OF THE ARM IF NECESARY:

    //if it is necessary abate the arm for recovery the security position
    if (p___3fin > p___3saf) {
        //build a message instruction and set up
        TMessageInstruction *MI = new TMessageInstruction();
        MI->setId(getActuator()->getId());
        MI->Instruction.setName("M2");
        MI->Instruction.Args.setCount(1);
        MI->Instruction.Args[0] = p___3saf;
        if(getDsec() < getDsecMax())
            MI->setComment1("Dsec = "+floattostr(getDsec())+" mm");

        //add the message instruction to the MP
        Positioning::TMessageList *ML = new Positioning::TMessageList();
        ML->Add(MI);
        MPretraction.Add(ML);
    }
}

//Propose (in MPturn, MPretraction) a recovery program composed by
//two or three gestures:
//  1. turn of the rotor 1 to p_1new.
//  2. Radial retraction to where it is possible.
//  3. Abatement of the arm to security position (if necessary).
//Inputs:
//  p_1new: new position where the rotor 1 shall be turned
//      before the retraction.
//Preconditions:
//- The quantifiers of the rotors shall be enabled.
//- The rotor 2 shall be in unsecurity position.
//- The new rotor 1 position p_1new shall be in the rotor 1 domain.
//Postconditions:
//- The MPturn will contains the MP for turn the rotor 1
//  to the position p_1new.
//- The MPretraction will contains the MP for retract the arm
//  to the first stable security position.
void TRoboticPositioner::proposeRecoveryProgram(double p_1new)
{
    //CHECK THE PRECONDITIONS:

    if(getActuator()->getQuantify_()!=true || getActuator()->getArm()->getQuantify___()!=true)
        throw EImproperCall("the quantifiers of the rotors shall be enabled");

    if(getActuator()->ArmIsInSafeArea())
        throw EImproperArgument("the rotor 2 shall be in unsecurity position");

    if(getActuator()->isntInDomainp_1(p_1new))
        throw EImproperArgument("the new rotor 1 position p_1new should be in the rotor 1 domain");

    //MAKE ACTIONS:

    //initialize the output variables
    MPturn.Clear();
    MPretraction.Clear();

    //BUILD AND ADD THE INSTRUCTION FOR ROTOR 1 TURN:

    //add a instruction for turn of the rotor 1
    TMessageInstruction *MI = new TMessageInstruction();
    MI->setId(getActuator()->getId());
    MI->Instruction.setName("M1");
    MI->Instruction.Args.setCount(1);
    MI->Instruction.Args[0] = p_1new;
    Positioning::TMessageList *ML = new Positioning::TMessageList();
    ML->Add(MI);
    MPturn.Add(ML);

    //CALCULATES DE FINAL POSITIONS OF THE ROTORS (p_1fin, p___3fin, p___3saf):

    //determines the first stable security position of rot 2 in steps
    double theta___3saf = getActuator()->gettheta___3saf();
    double p___3saf = getActuator()->getArm()->getF().Image(theta___3saf);
    p___3saf = floor(p___3saf);

    //perform a rutinary check
    if(p___3saf < 0)
        throw EImpossibleError("rotor 2 position p___3saf should be nonnegative");

    //Note that if(p___3saf < RP->getActuator()->getArm()->getp___3first())
    //the MP will be proposed but it will not pass the validation process.

    //determines the necessary displacement of rot 2 in radians
    theta___3saf = getActuator()->getArm()->getG().Image(p___3saf);
    double theta___3ini = getActuator()->getArm()->gettheta___3();
    double dt2 = theta___3ini - theta___3saf;

    //calculates the necessary displacement of rotor 1 in radians
    double dt1 = dt2/2;

    //get the initial position of the rotors 1 in radians
    double theta_1ini = getActuator()->getG().Image(p_1new); //<-------------------DIFERENT!

    //determines the final position of the rotors 1 in radians
    double theta_1fin;
    theta_1fin = theta_1ini - dt1;

    //determines the final position of rot 1 in steps
    double p_1fin = getActuator()->getF().Image(theta_1fin);
    p_1fin = round(p_1fin);

    //determines if the rotor 1 have enough space for retraction
    double p_1first = getActuator()->getp_1first();
    p_1first = ceil(p_1first); //purge lateral effect
    bool rot1_has_enough = (p_1fin >= p_1first);

    //calculates the final positions according the space available for the rotor 1
    double p___3fin;
    //if the rotor 1 has enpough space for execue the retraction
    if(rot1_has_enough)
        //determines the final position of rot 2 in steps
        p___3fin = p___3saf;

    //if the rotor 1 not has enpough space for execue the retraction
    else {
        //determines the final position of rot 1 in steps
        p_1fin = getActuator()->getp_1first();

        //calulates the final position of the rotors in radians
        theta_1fin = getActuator()->getG().Image(p_1fin);
        dt1 = theta_1ini - theta_1fin;
        dt2 = dt1*2;
        double theta___3fin = theta___3ini - dt2;

        //determines the final position of rot 2 in steps
        p___3fin = getActuator()->getArm()->getF().Image(theta___3fin);
        p___3fin = round(p___3fin);
    }

    //BUILD AND ADD THE INSTRUCTION FOR RADIAL RETRACTION:

    //build a message instruction and set it
    MI = new TMessageInstruction();
    MI->setId(getActuator()->getId());
    MI->Instruction.setName("MM");
    MI->Instruction.Args.setCount(2);
    MI->Instruction.Args[0] = p_1fin;
    MI->Instruction.Args[1] = p___3fin;
    if(getDsec() < getDsecMax())
        MI->setComment1("Dsec = "+floattostr(getDsec())+" mm");

    //add the message instruction to the MP
    ML = new Positioning::TMessageList();
    ML->Add(MI);
    MPretraction.Add(ML);

    //ADD INSTRUCTION FOR ABATEMENT OF THE ARM IF NECESARY:

    //if it is necessary abate the arm for recovery the security position
    if (p___3fin > p___3saf) {
        //build a message instruction and set up
        TMessageInstruction *MI = new TMessageInstruction();
        MI->setId(getActuator()->getId());
        MI->Instruction.setName("M2");
        MI->Instruction.Args.setCount(1);
        MI->Instruction.Args[0] = p___3saf;
        if(getDsec() < getDsecMax())
            MI->setComment1("Dsec = "+floattostr(getDsec())+" mm");

        //add the message instruction to the MP
        Positioning::TMessageList *ML = new Positioning::TMessageList();
        ML->Add(MI);
        MPretraction.Add(ML);
    }
}

//MÉTODOS PARA DETERMINAR LA DISTANCIA MÁXIMA RECORRIDA
//POR UN PUNTO DEL BRAZO:

//determina el valor absoluto de
//el ángulo máximo recorrido al moverse el rotor 1 durante un tiempo T
//      theta_MaxAbs(T) = Actuator->G.Image(CMF.vmaxabs1)*T;
double TRoboticPositioner::theta_MaxAbs(double T)
{
    //el tiempo de desplazamiento T debería ser no negativo
    if(T < 0)
        throw EImproperArgument("displacement time T should be nonnegative");

    return getActuator()->getG().Image(CMF.getvmaxabs1())*T;
}
//determina el valor absoluto de
//la distancia máxima recorrida por un punto del brazo
//al moverse el rotor 1 durante un tiempo T
//      d_MaxAbs(T) = theta_MaxAbs(T)*Actuator->r_max;
double TRoboticPositioner::d_MaxAbs(double T)
{
    //el tiempo de desplazamiento T debería ser no negativo
    if(T < 0)
        throw EImproperArgument("displacement time T should be nonnegative");

    return theta_MaxAbs(T)*getActuator()->getr_max();
}

//determina el valor absoluto de
//el ángulo máximo recorrido al moverse el rotor 2 durante un tiempo T
//      theta___MaxAbs(T) = Actuator->Arm->G.Image(CMF.vmaxabs2)*T;
double TRoboticPositioner::theta___MaxAbs(double T)
{
    //el tiempo de desplazamiento T debería ser no negativo
    if(T < 0)
        throw EImproperArgument("displacement time T should be nonnegative");

    return getActuator()->getArm()->getG().Image(CMF.getvmaxabs2())*T;
}
//determina el valor absoluto de
//la distancia máxima recorrida por un punto del brazo
//al moverse el rotor 2 durante un tiempo T
//      d___MaxAbs(T) = theta___MaxAbs(T)*Actuator->Arm->L1V;
double TRoboticPositioner::d___MaxAbs(double T)
{
    //el tiempo de desplazamiento T debería ser no negativo
    if(T < 0)
        throw EImproperArgument("displacement time T should be nonnegative");

    return theta___MaxAbs(T)*getActuator()->getArm()->getL1V();
}

//determina una cota superior para la distancia máxima recorrida
//por un punto del brazo del posicionador, al moverse ambos rotores
//durante un intervalo de tiempo T
//      dMaxAbs = Max(d_MaxAbs(T), d___MaxAbs(T))
double TRoboticPositioner::dMaxAbs(double T)
{
    return Max(d_MaxAbs(T), d___MaxAbs(T));
}

//determine a upper top for longitudinal velocity
//of all points of the arm
double TRoboticPositioner::calculatevmaxabs(void) const
{
    double vmaxabs;

    //if it is going to be moved any rotor
    if(CMF.getMF1()==NULL && CMF.getMF2()==NULL) {
        //indicates that the longitudinal velocity is cero
        return 0;
    }

    //if it is going to be moved the rotor 1 only
    else if(CMF.getMF1()!=NULL && CMF.getMF2()==NULL) {
        //calculate the angular velocity of the rotor 1 in rad/ms
        double w1 = CMF.getMF1()->getvmaxabs()/getActuator()->getSB1()*M_2PI;
        //calculate the upper top for longitudinal velocity in mm/ms
        vmaxabs = w1*getActuator()->getr_max();
    }

    //else, if it is going to be moved the rotor 2 only
    else if(CMF.getMF1()==NULL && CMF.getMF2()!=NULL) {
        //calculate the angular velocity of the rotor 2 in rad/ms
        double w2 = CMF.getMF2()->getvmaxabs()/getActuator()->getArm()->getSB2()*M_2PI;
        //calculate the upper top for longitudinal velocity in mm/ms
        vmaxabs = w2*getActuator()->getArm()->getL1V();
    }

    //else, if it is going to be moved both rotors
    else {
        //calculate the angular velocity of the rotor 1 in rad/ms
        double w1 = CMF.getMF1()->getvmaxabs()/getActuator()->getSB1()*M_2PI;
        //calculate the angular velocity of the rotor 2 in rad/ms
        double w2 = CMF.getMF2()->getvmaxabs()/getActuator()->getArm()->getSB2()*M_2PI;
        //calculate the upper top for longitudinal velocity in mm/ms
        vmaxabs = w1*getActuator()->getr_max() + w2*getActuator()->getArm()->getL1V();
    }

    return vmaxabs;
}

//calculates the displacement of rotor 1
//  double theta_1 = getActuator()->theta_1s.getLast();
//  double p_1 = getActuator()->getF().Image(theta_1);
//  p_1 = Round(p_1);
//  dp1 =  CMF.getMF1()->getpsta() - p_1;
//preconditions:
//  RP shall be stascked one position almost
//  rotor 1 quantifier shall be enabled
double TRoboticPositioner::dp1(void) const
{
    //CHECK THE PRECONDITIONS;

    if(getActuator()->theta_1s.getCount() < 1)
        throw EImproperCall("RP shall be stascked one position almost");

    if(!getActuator()->getQuantify_())
        throw EImproperCall("rotor 1 quantifier shall be enabled");

    //CALCULATES AND RETURN THE DISPLACEMENT IN STEPS:

    //get the stored initial position in radians
    double theta_1 = getActuator()->theta_1s.getLast();
    //translate the position to steps
    double p_1 = getActuator()->getF().Image(theta_1);
    //eliminates the numerical error
    p_1 = Round(p_1);
    //calculates the displacement from initial positions to start positions
    double dp1 =  CMF.getMF1()->getpsta() - p_1;

    return dp1;
}

//MOTION METHODS:

//lleva los rotores del cilindro adscrito a
//las posiciones correspondientes al instante t
//suponiendo que no hay pérdida de pasos
void TRoboticPositioner::move(double t)
{
    if(CMF.getMF1()!=NULL && CMF.getMF2()!=NULL)
        getActuator()->setAnglesSteps(CMF.MF1p(t), CMF.MF2p(t));
    else if(CMF.getMF1()!=NULL && CMF.getMF2()==NULL)
        getActuator()->setp_1(CMF.MF1p(t));
    else if(CMF.getMF1()==NULL && CMF.getMF2()!=NULL)
        getActuator()->getArm()->setp___3(CMF.MF2p(t));
}
//lleva los rotores del cilindro adscrito a
//las posiciones correspondientes al instante 0
//suponiendo que no hay pérdida de pasos
void TRoboticPositioner::moveSta(void)
{
    if(CMF.getMF1()!=NULL && CMF.getMF2()!=NULL)
        getActuator()->setAnglesSteps(CMF.getMF1()->getpsta(), CMF.getMF2()->getpsta());
    else if(CMF.getMF1()!=NULL && CMF.getMF2()==NULL)
        getActuator()->setp_1(CMF.getMF1()->getpsta());
    else if(CMF.getMF1()==NULL && CMF.getMF2()!=NULL)
        getActuator()->getArm()->setp___3(CMF.getMF2()->getpsta());
}
//lleva los rotores del cilindro adscrito a
//las posiciones correspondientes al instante Tmax
//suponiendo que no hay pérdida de pasos
void TRoboticPositioner::moveFin(void)
{
    if(CMF.getMF1()!=NULL && CMF.getMF2()!=NULL)
        getActuator()->setAnglesSteps(CMF.getMF1()->getpfin(), CMF.getMF2()->getpfin());
    else if(CMF.getMF1()!=NULL && CMF.getMF2()==NULL)
        getActuator()->setp_1(CMF.getMF1()->getpfin());
    else if(CMF.getMF1()==NULL && CMF.getMF2()!=NULL)
        getActuator()->getArm()->setp___3(CMF.getMF2()->getpfin());
}

//lleva los rotores del posicionador adscrito a las posiciones
//correspondientes a después de ejecutar (MPturn, MPretraction)
void TRoboticPositioner::moveFinMP(void)
{
    for(int i=0; i<MPturn.getCount(); i++) {
        TMessageList *ML = MPturn.GetPointer(i);
        for(int j=0; j<ML->getCount(); j++) {
            TMessageInstruction *MI = ML->GetPointer(j);
            setPosition(MI->Instruction);
        }
    }
    for(int i=0; i<MPretraction.getCount(); i++) {
        TMessageList *ML = MPretraction.GetPointer(i);
        for(int j=0; j<ML->getCount(); j++) {
            TMessageInstruction *MI = ML->GetPointer(j);
            setPosition(MI->Instruction);
        }
    }
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

