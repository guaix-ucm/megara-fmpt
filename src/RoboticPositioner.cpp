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
//Archivo: RoboticPositioner.cpp
//Contenido: posicionador básico (dimensión y posición)
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "RoboticPositioner.h"
#include "Strings.h" //StrIndent

//#include <values.h>

//---------------------------------------------------------------------------

using namespace Strings;

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

//PROPIEDADES DE TOLERANCIA:

void TRoboticPositioner::setEo(double _Eo)
{
    //el valor de Eo debe ser no negativo
    if(_Eo < 0)
        throw EImproperArgument("Eo value should be nonnegative");

    __Eo = _Eo; //asigna el nuevo valor

    //ERROR: se ha dispuesto asignar E0 = 0 para
    //reducir el SPM y evitar una colisión dinámica
    //en el ejemplo original.

    //asimila Eo
    CalculateSPMComponents();
}
void TRoboticPositioner::setEp(double _Ep)
{
    //el valor de Ep debe ser no negativo
    if(_Ep < 0)
        throw EImproperArgument("Ep value should be nonnegative");

    __Ep = _Ep; //asigna el nuevo valor

    //asimila Ep
    CalculateSPMComponents();
}

void TRoboticPositioner::setTstop(double _Tstop)
{
    //el valor de Tstop debe ser no negativo
    if(_Tstop < 0)
        throw EImproperArgument("Tstop value should be nonnegative");

    __Tstop = _Tstop; //asigna el nuevo valor

    //asimila Tstop
    CalculateSPMComponents();
}
void TRoboticPositioner::setTshiff(double _Tshiff)
{
    //el valor de Tshiff debe ser no negativo
    if(_Tshiff < 0)
        throw EImproperArgument("Tshiff value should be nonnegative");

    __Tshiff = _Tshiff; //asigna el nuevo valor

    //asimila Tshiff
    CalculateSPMComponents();
}

//------------------------------------------------------------------
//PROPIEDADES DE SEGURIDAD EN FORMATO TEXTO:

//PROPIEDADES DE TOLERANCIA EN FORMATO TEXTO:

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
void TRoboticPositioner::setFaultProbability(double _FaultProbability)
{
    //la probabilidad de fallo FaultProbability debe estar en [0, 1]
    if(_FaultProbability<0 || 1<_FaultProbability)
        throw EImproperArgument("fault probability FaultProbability should be in [0, 1]");

    //asigna el valor
    __FaultProbability = _FaultProbability;
}

//---------------------------------------------------------------------------
//PROPIEDADES DE ESTADO EN FORMATO TEXTO:

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
    S += AnsiString("    Tshiff = ")+getTshiffText();

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
        TRoboticPositioner *FP = &aux;
        //lee la instancia y la asigna a la variable tampón
        int i = 1;
        ReadInstance((TRoboticPositioner*&)FP, S, i);

        //avanza el índice i hasta la próxima posición que no contenga un separador
        StrTravelSeparatorsIfAny(S, i);
        //si el índice i indica a algún caracter de la cadena S
        if(i <= S.Length())
            //indica que la cadena S solo debería contener el valor para una instancia
            throw EImproperArgument("string S should contain the instance value only");

        //asigna la variable tampón
        Clone(FP);

    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//compara los identificadores de dos posicionadores
int  TRoboticPositioner::CompareIds(TRoboticPositioner *FP1,
                                              TRoboticPositioner *FP2)
{
    return TActuator::CompareIds(
                FP1->getActuator(), FP2->getActuator());
}

//imprime el identificador de un posicionador
void  TRoboticPositioner::PrintId(AnsiString &S, TRoboticPositioner *FP)
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
void  TRoboticPositioner::PrintOriginsRow(AnsiString& S,
                                          TRoboticPositioner *FP)
{
    TActuator::PrintOriginsRow(S, FP->getActuator());
}
//lee los valores de las propiedades de origen de un posicionador
//(Id, x0, y0, thetaO1) desde la posición indicada de una cadena
//de texto, en formato fila de texto
void  TRoboticPositioner::ReadOriginsRow(TRoboticPositioner *FP,
                                         const AnsiString& S, int &i)
{
    TActuator *A = FP->getActuator();
    TActuator::ReadOriginsRow(A, S,  i);
}

//imprime los valores de las propiedades de posición de un posicionador
//(Id, x3, y3) al final de una cadena de texto
//en formato fila de texto
void  TRoboticPositioner::PrintPositionP3Row(AnsiString& S,
                                             TRoboticPositioner *FP)
{
    TActuator::PrintPositionP3Row(S, FP->getActuator());
}
//lee los valores de las propiedades de posición de un posicionador
//(Id, x3, y3) desde la posición indicada de una cadena
//de texto, en formato fila de texto
void  TRoboticPositioner::ReadPositionP3Row(TRoboticPositioner* &FP,
                                            const AnsiString& S, int &i)
{
    TActuator *A = FP->getActuator();
    TActuator::ReadPositionP3Row(A, S,  i);
}
//imprime los valores de las propiedades de posición de un posicionador
//(Id, p_1, p___3) al final de una cadena de texto
//en formato fila de texto
void  TRoboticPositioner::PrintPositionPPARow(AnsiString& S,
                                              TRoboticPositioner *FP)
{
    TActuator::PrintPositionPPARow(S, FP->getActuator());
}

//lee una instancia en una cadena
void  TRoboticPositioner::ReadInstance(TRoboticPositioner* &FP,
                                                 const AnsiString& S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el puntero FP debería apauntar a un posicionador construído
    if(FP == NULL)
        throw EImproperArgument("pointer FP should point to built fiberpositioner");

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

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
    //      9: esperando etiqueta "Status:"
    //      10: esperando asignación a Disabled
    //      11: esperando asignación a FaultProbability
    //      12: esperando asignación a FaultType
    //      13: instancia de posicionador leida con éxito
    int status = 0;

    //variables tampón
    TRoboticPositioner _FP(FP);

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
            TActuator *aux = _FP.getActuator();
            TActuator::ReadInstance(aux, S, i);
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
            TComposedMotionFunction *aux = &_FP.CMF;
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
            _FP.setEo(aux);
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
            _FP.setEp(aux);
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
            _FP.setTstop(aux);
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
            _FP.setTshiff(aux);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 9: //esperando etiqueta "Status:"
            try {
            StrTravelLabel("Status:", S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 10: //esperando asignación a Disabled
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("Disabled", S, i);
            StrTravelLabel("=", S, i);
            bool aux;
            StrReadBool(aux, S, i);
            _FP.Disabled = aux;
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 11: //esperando asignación a FaultProbability
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("FaultProbability", S, i);
            StrTravelLabel("=", S, i);
            double aux;
            StrReadFloat(aux, S, i);
            _FP.setFaultProbability(aux);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 12: //esperando asignación a FaultType
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("FaultType", S, i);
            StrTravelLabel("=", S, i);
            TFaultType aux;
            StrReadFaultType(aux, S, i);
            _FP.FaultType = aux;
        }catch(...) {
            throw;
        }
            status++;
            break;

        }
        //mientras no se haya leido la instancia de posicionador con éxito
    } while(status < 13);

    //asigna la variable tampón
    FP->Clone(&_FP);
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONSTRUCCIÓN, DESTRUCCIÓN Y COPIA:

//build a RP by default
TRoboticPositioner::TRoboticPositioner(void) :
    //inicializa las propiedades de tolerancia
    __Eo(0.001), __Ep(0.01),
    __Tstop(1), __Tshiff(1),
    //contruye e inicializa las propiedades de control
    CMF(),
    //inicializa las propiedades de estado
    Disabled(false), __FaultProbability(0), FaultType(ftUnk),
    ControlMode(cmSinc), MP()
{
    //contruye el actuador del posicionador
    //con el identificador 0
    //en la posición {0, 0}
    __Actuator = new TActuator(0, TDoublePoint(0, 0), 0);

    //inicializa las propiedades de control
    CMF.setLabel("CMF");

    //añade el posicionador a la lista de contruidos
    Builts.Add(this);
}
//build a RP with the indicated values
//if Id is less 1 throw an exception EimproperArgument
TRoboticPositioner::TRoboticPositioner(int _Id, TDoublePoint _P0,
                                       double _thetaO1) :
    //inicializa las propiedades de tolerancia
    __Eo(0.000001), __Ep(0.00001),
    __Tstop(0.001), __Tshiff(0.001),
    //contruye e inicializa las propiedades de control
    CMF(),
    //construye e inicializa las propiedades de estado
    Disabled(false), __FaultProbability(0), FaultType(ftUnk),
    ControlMode(cmSinc)
{
    //el número de identificación Id debe ser mayor que cero
    if(_Id < 1)
        throw EImproperArgument("identification number Id should be upper zero");

    //WARNING: duplicity of identifiers is allowed.

    //contruye el actuador del posicionador
    __Actuator = new TActuator(_Id, _P0, _thetaO1);

    //inicializa las propiedades de control
    CMF.setLabel("CMF");

    //añade el posicionador a la lista de contruidos
    Builts.Add(this);
}

/*//copy all control properties of a RP
void CopyControl(const TRoboticPositioner*);
void CopyControl(const TRoboticPositioner&);*/
//copy all tolerance properties of a RP
void TRoboticPositioner::CopyTolerance(const TRoboticPositioner *RP)
{
    //check the precondition
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    //copia las tolerancias
    __Eo = RP->getEo();
    __Ep = RP->getEp();
    __Tstop = RP->getTstop();
    __Tshiff = RP->getTshiff();
}
void TRoboticPositioner::CopyTolerance(const TRoboticPositioner& RP)
{
    //copia las tolerancias
    __Eo = RP.getEo();
    __Ep = RP.getEp();
    __Tstop = RP.getTstop();
    __Tshiff = RP.getTshiff();
}
//copy all status properties of a RP
void TRoboticPositioner::CopyStatus(const TRoboticPositioner *RP)
{
    //check the precondition
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    Disabled = RP->Disabled;
    setFaultProbability(RP->getFaultProbability());
    FaultType = RP->FaultType;
    ControlMode = RP->ControlMode;
}
void TRoboticPositioner::CopyStatus(const TRoboticPositioner& RP)
{
    Disabled = RP.Disabled;
    setFaultProbability(RP.getFaultProbability());
    FaultType = RP.FaultType;
    ControlMode = RP.ControlMode;
}

//copy all properties of a RP
void TRoboticPositioner::Clone(const TRoboticPositioner *RP)
{
    //check the precondition
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    //copia las propiedades
    getActuator()->Clone(RP->getActuator());
    CMF.Copy(RP->CMF);
    CopyTolerance(RP);
    CopyStatus(RP);
}
void TRoboticPositioner::Clone(const TRoboticPositioner& RP)
{
    //copia las propiedades
    getActuator()->Clone(RP.getActuator());
    CMF.Copy(RP.CMF);
    CopyTolerance(RP);
    CopyStatus(RP);
}
TRoboticPositioner& TRoboticPositioner::operator=(const TRoboticPositioner& RP)
{
    //copia las propiedades
    getActuator()->Clone(RP.getActuator());
    CMF.Copy(RP.CMF);
    CopyTolerance(RP);
    CopyStatus(RP);

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
    __Actuator = new TActuator(RP->getActuator());
    //copy all other properties of the RP
    Clone(RP);

    //añade el posicionador a la lista de contruidos
    Builts.Add(this);
}
TRoboticPositioner::TRoboticPositioner(const TRoboticPositioner& RP)
{
    //contruye un clon del actuador del posicionador
    __Actuator = new TActuator(RP.getActuator());
    //copia todas las propiedades del posicionador
    Clone(RP);

    //añade el posicionador a la lista de contruidos
    Builts.Add(this);
}

//copy all properties of a RP
//except (P0, Id, Id1, Id2)
void TRoboticPositioner::Apply(const TRoboticPositioner& RP)
{
    TDoublePoint P0 = getActuator()->getP0();
    int Id = getActuator()->getId();
    int Id1 = CMF.getId1();
    int Id2 = CMF.getId2();

    Clone(RP);

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
    delete __Actuator;

    //borra el puntero de la lista de construídos
    Builts.Delete(i);
}

//---------------------------------------------------------------------------
//MÉTODOS PARA CALCULAR LOS VALORES RECOMENDADOS DE
//LAS COMPONENTES DE SPM:

//calcula la componente de SPM para absorber el error de recuperación:
//      SPMrec = (CMF.vmaxabs1*Actuator->rbs*Actuator->r_max +
//      CMF.vmaxabs2*Actuator->Arm->rbs*Actuator->Arm->Ra)*
//      Tstop mm
double TRoboticPositioner::SPMrec(void) const
{
    return (CMF.getvmaxabs1()*getActuator()->getrbs()*getActuator()->getr_max() +
            CMF.getvmaxabs2()*getActuator()->getArm()->getrbs()*getActuator()->getArm()->getL1V())*getTstop();
}

//Calcula el márgen perimetral de seguridad para evitar:
//- el error mecánico debido al mecanizado del actuador;
//- el error mecánico de orientación en el anclaje del posicionador;
//- el error mecánico de posicionamiento en el anclaje del posicionador;
//- el error numérico de las operaciones matemáticas del modelo;
//- el error de posición debido a variaciones de velocidad;
//  al desplazarse de un paso a otro.
//- el error de posición debido a la falta de correspondencia de
//  F(theta_1) y Arm->F(theta___3) con al realidad.
//Todos los errores serán absorbidos por Eo y Ep
//en la siguiente fórmula:
//      SPMsta = Eo*r_max + Ep
//Donde:
//      Eo: margen de error de orientación en rad;
//      Ep: margen de error de posición en mm.
double TRoboticPositioner::SPMsta(void) const
{
    return getEo()*getActuator()->getr_max() + getEp(); //calcula y devuelve el valor
}

//calcula la componente de SPM para absorber el error dinámico
//      SPMdyn = (CMF.vmaxabs1*Actuator->rbs*Actuator->r_max +
//      CMF.vmaxabs2*Actuator->Arm->rbs*Actuator->Arm->Ra)*
//      Tshiff + (Actuator->rbs*Actuator->r_max +
//      Actuator->Arm->rbs*Actuator->Arm->Ra) mm
double TRoboticPositioner::SPMdyn(void) const
{
    return (CMF.getvmaxabs1()*getActuator()->getrbs()*getActuator()->getr_max() +
            CMF.getvmaxabs2()*getActuator()->getArm()->getrbs()*getActuator()->getArm()->getL1V())*
            getTshiff() + (getActuator()->getrbs()*getActuator()->getr_max() +
                      getActuator()->getArm()->getrbs()*getActuator()->getArm()->getL1V());
}

/*//Calcula el margen perimetral de seguridad para
//validación o programación:
//      SPMtop = r_max/1000*(8/M_PI*w1 + 4/M_PI*w2)
//Donde:
//      r_max: radio apical del posicionador;
//      w1: velocidad angular máxima del eje 1 en rad/s;
//      w2: velocidad angular máxima del eje 2 en rad/s.
double TRoboticPositioner::SPMtop(double w1, double w2) const
{
        //la velocidad angular del eje 1 debe ser no negativa
        if(w1 < 0)
                throw EImproperArgument("angular velociti of rot 1 should be nonnegative");

        //la velocidad angular del eje 2 debe ser no negativa
        if(w2 < 0)
                throw EImproperArgument("angular velociti of rot 2 should be nonnegative");

        //ADVERENCIA: los valores de w1 y w2 deberían ser corregidos
        //en función de las derivadas de G(p_1) y Arm->G(p___3).
        //Pero si las funciones G son aproximadamente lineales,
        //realmente no hace falta por que el valor devuelto por SPMtop
        //solo tiene que ser orientativo, pudiendo elegir el valor
        //que se desee.

        //calcula y devuelve el valor
        return r_max*(8/M_PI*w1 + 4/M_PI*w2);
}

//asigna un valor a SPM según el propósito
//      SPM = SPMerr + SPMtop + SPMtop + SPMpro; cuando Popose=sPro
//      SPM = SPMerr + SPMtop;   cuando Popose=sVal
//      SPM = SPMerr;    cuando Popose=sExe
void TRoboticPositioner::SetSPM(TPurpose Purpose,
        double Eo, double Ep,
        double w1, double w2)
{
        //el cueficiente de error de orientación Eo debe ser mayor que cero
        if(Eo <= 0)
                throw EImproperArgument("orientation error coefficient Eo should be upper zero");

        //el cueficiente de error de posición Ep debe ser mayor que cero
        if(Ep <= 0)
                throw EImproperArgument("position error coefficient Ep should be upper zero");

        //la velocidad angular del eje 1 debe ser no negativa
        if(w1 < 0)
                throw EImproperArgument("angular velociti of rot 1 should be nonnegative");

        //la velocidad angular del eje 2 debe ser no negativa
        if(w2 < 0)
                throw EImproperArgument("angular velociti of rot 2 should be nonnegative");

        switch(Purpose) {
                case pPro:
                        Arm->SPM = GetSPMerr(Eo, Ep) + GetSPMtop(w1, w2) + GetSPMtop(w1, w2);
                        break;
                case pVal:
                        Arm->SPM = GetSPMerr(Eo, Ep) + GetSPMtop(w1, w2);
                        break;
                case pExe:
                        Arm->SPM = GetSPMerr(Eo, Ep);
                        break;
                default:
                        throw EImpossibleError("value unknown for property Purpose");
        }
}
  */
//--------------------------------------------------------------------------
//MÉTODOS DE ASIGNACIÓN CONJUNTA:

//asigna conjuntamente los márgenes de segudidad
//      (Eo, Ep, Tstop, Tshiff)
void TRoboticPositioner::SetTolerances(double _Eo, double _Ep,
                                       double _Tstop, double _Tshiff)
{
    //el valor de Eo debe ser no negativo
    if(_Eo < 0)
        throw EImproperArgument("Eo value should be nonnegative");

    //el valor de Ep debe ser no negativo
    if(_Ep < 0)
        throw EImproperArgument("Ep value should be nonnegative");

    //el valor de Tstop debe ser no negativo
    if(_Tstop < 0)
        throw EImproperArgument("Tstop value should be nonnegative");

    //el valor de Tshiff debe ser no negativo
    if(_Tshiff < 0)
        throw EImproperArgument("Tshiff value should be nonnegative");

    //asigna los nuevos valores
    __Eo = _Eo;
    __Ep = _Ep;
    __Tstop = _Tstop;
    __Tshiff = _Tshiff;

    //asimila (Eo, Ep, Tstop, Tshiff)
    CalculateSPMComponents();
}

//--------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//A partir de:
//      (Eo, Ep, Tstop, Tshiff)
//      FP->rmax
//determina:
//      (SPMrec, SPMsta, SPMdyn, SPMmin)
void TRoboticPositioner::CalculateSPMComponents(void)
{
    //CALULA LAS COMPONENTES DE SPM:

    double aux = SPMsta();
    getActuator()->SetSPMComponents(SPMrec(), aux, SPMdyn(), aux);//, Actuator->SPMoff);
    getActuator()->getBarrier()->setSPM(aux);
}

//asigna la componente de SPM para absorber el desplazamiento
//por corrección del offset
//      Actuator->SPMoff = PAem*Actuator->rmax + Pem mm
void TRoboticPositioner::SetSPMoff(double PAem, double Pem)
{
    //el valor de PAem debe ser no negativo
    if(PAem < 0)
        throw EImproperArgument("PAem value should be nonnegative");

    //el valor de Pem debe ser no negativo
    if(Pem < 0)
        throw EImproperArgument("Pem value should be nonnegative");

    //calcula y devuelve el valor
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
    if(getActuator()->IsntInDomainp_1(p_1fin))
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
    if(getActuator()->getArm()->IsntInDomainp___3(p___3fin))
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
    if(getActuator()->IsntInDomainp_1(p_1fin) || getActuator()->getArm()->IsntInDomainp___3(p___3fin))
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
    if(getActuator()->IsntInDomainp_1(p_1sta) || getActuator()->IsntInDomainp_1(p_1fin))
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
    if(getActuator()->getArm()->IsntInDomainp___3(p___3sta) || getActuator()->getArm()->IsntInDomainp___3(p___3fin))
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
    //check theprecondition
    if(getActuator()->IsntInDomainp_1(p_1sta) || getActuator()->IsntInDomainp_1(p_1fin))
        throw EImproperArgument("position angles p_1sta and p_1fin should be in rot 1 domain");
    //check theprecondition
    if(getActuator()->getArm()->IsntInDomainp___3(p___3sta) || getActuator()->getArm()->IsntInDomainp___3(p___3fin))
        throw EImproperArgument("position angles p___3sta and p___3fin should be in rot 2 domain");

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
            getActuator()->PushQuantify_();
            getActuator()->setQuantify_(true);
            //program the gesture
            M1(Instruction.Args.getFirst());
            //restore the status of rotor quantifier
            getActuator()->RestoreAndPopQuantify_();

        } else if(Instruction.getName() == "M2") {
            //enable temporarity the rotor quantifier to accomplish precondition
            getActuator()->getArm()->PushQuantify___();
            getActuator()->getArm()->setQuantify___(true);
            //program the gesture
            M2(Instruction.Args.getFirst());
            //restore the status of rotor quantifier
            getActuator()->getArm()->RestoreAndPopQuantify___();

        } else if(Instruction.getName() == "MM") {
            //enable temporarity the rotor quantifier to accomplish precondition
            getActuator()->PushQuantifys();
            getActuator()->setQuantify_(true);
            getActuator()->getArm()->setQuantify___(true);
            //program the gesture
            MM(Instruction.Args.getFirst(), Instruction.Args[1]);
            //restore the status of rotor quantifier
            getActuator()->RestoreAndPopQuantifys();

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
//  rotor 1 from actualposition to Max(0, p_1min)
//  rotor 2 from actualposition to Max(0, p___3min)
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

//METHODS TO PROGRAM GESTURES:

//program the abatement of the arm to
//the more closer-stable security position
void TRoboticPositioner::programTurnArmToSafeArea(void)
{
    //translate to steps the security position limit of the rotor 2
    double p___3saf = getActuator()->getArm()->getF().Image(getActuator()->gettheta___3saf());
    //set the firs stable position less than the security position limit
    M2(Max(0., floor(p___3saf)));
}

//program the retraction of the arm to
//the more closer-stable security position
//if rotor 1 quantifier or rotor 2 quantifier is disabled:
//  throw and exception EImproperCall
void TRoboticPositioner::programRetractArmToSafeArea(void)
{
    //check the preconditions
    if(getActuator()->getQuantify_() != true || getActuator()->getArm()->getQuantify___() != true)
        throw EImproperCall("rotor 1 quantifier and rotor 2 quantifier should be enabled");

/*    //CONFIGURA LA VELOCIDAD DE ROT 1 IGUAL A 1/2 DE LA VELOCIDAD DE ROT 2:

    switch(CMF.getMFM()) {
    case mfmSquare: {
        //traduce la velocidad de rot 2 a rad/ms
        double vmaxabs2 = CMF.getSF2()->getvmaxabs()/getActuator()->getArm()->getSB2()*M_2PI;
        //calcula la velocidad a la que debe moverse rot 1 en rad/ms
        double vmaxabs1 = vmaxabs2/2;
        //traduce a pasos/ms y la asigna
        CMF.getSF1()->setvmaxabs(vmaxabs1/M_2PI*getActuator()->getSB1());
    } break;
    case mfmRamp: {
        //traduce la velocidad de rot 2 a rad/ms
        double vmaxabs2 = CMF.getRF2()->getvmaxabs()/getActuator()->getArm()->getSB2()*M_2PI;
        //calcula la velocidad a la que debe moverse rot 1 en rad/ms
        double vmaxabs1 = vmaxabs2/2;
        //traduce a pasos/ms, la cuantifica y la asigna
        CMF.getRF1()->setvmaxabs(round(vmaxabs1/M_2PI*getActuator()->getSB1()));
    } break;
    }

    //Nótese que para traducir la velocidad angular de rad/ms a pasos/ms
    //no debe emplearse la función G de los cuantificadores, cuyo dominio
    //está restringido, por lo que el resultado podría ser menor que 1 ms.
*/
    //DETERMINA LA POSICIÓN A LA QUE DEBE  MOVERSE ROT 2:

    //traduce la posisión de seguridad de rot 2 a pasos
    double p___3saf = getActuator()->getArm()->getF().Image(getActuator()->gettheta___3saf());
    //determina la primra posición de seguridad estable de rot 2
    double p___3saf_ = Max(0., floor(p___3saf));

    //DETERMINA LA POSICIÓN A LA QUE DEBE MOVERSE ROT 1:

    double theta_1; //posición en radianes a la que debe moverse rot 1
    double p_1_;    //posición en pasos estable en a la que debe moverse rot 1

    //determina la distancia en pasos a recorrer por rot 2 en pasos
    double dp2 = getActuator()->getArm()->getp___3() -  p___3saf_;
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
    p_1_ = Max(0., round(p_1));

    //PROGRAM THE MOVEMENT OF THE ROTORS:

    //program the movement to go to the stable position
    MM(p_1_, p___3saf_);
}

//program turn of rotor 1 to theta_1
//if theta_1 is out rotor 1 domain:
//  throw an exception EImproperArgument
void TRoboticPositioner::programTurnCilinderTotheta_1(double _theta_1)
{
    //traduce a pasos
    int _p_1 = getActuator()->getQ()[getActuator()->getF().Image(_theta_1)];

    try{
        //programa el gesto
        M1(_p_1);
    } catch(...) {
        throw;
    }
}
/*//gira el brazo hasta el área de seguridad
//si el brazo ya está en el área de seguridad devuelve falso
void TRoboticPositioner::TurnArmToSafeArea(void)
{
        //el brazo no debería estar en el area de seguridad
        if(ArmIsInSafeArea())
                throw EImproperCall("arm should not to be in safe area");

        //calcula la posición de seguridad estable más próxima
        int _p__3saf = floor(p__3saf);

        try {
                //programa el gesto
                M2(_p__3saf);
        } catch(...) {
                throw;
        }
} */
//progrsam go directly from actual position to (x_3, y_3)
//if (x_3, y_3) is out the scope of the RP:
//  throw an exception EImproperArgument
void TRoboticPositioner::programGoDirectlyToCartesianP_3(double _x_3, double _y_3)
{
    //traduce a polares en S1
    double r_3 = Mod(_x_3, _y_3);
    double theta_3 = Arg(_x_3, _y_3);

    //determina las posiciones de los ejes para llevar P3 a (r_3, theta_3)
    //y determina si la posición está dentro del dominio
    double theta_1, theta___3;
    bool isindomine = getActuator()->AnglesToGoP_3(theta_1, theta___3,
                                              r_3, theta_3);

    //la posición (x_3, y_3) debería estar dentro del dominio de P_3
    if(!isindomine)
        throw EImproperArgument("position (x_3, y_3) should be in P_3 domain");

    //traduce a pasos
    double _p_1 = getActuator()->getF().Image(theta_1);
    double _p___3 = getActuator()->getArm()->getF().Image(theta___3);

    //cuantifica los pasos
    _p_1 = getActuator()->getQ()[_p_1];
    _p___3 = getActuator()->getArm()->getQ()[_p___3];

    //ADVERTENCIA: la cuantificación de los pasos no dará lugar al centroide
    //más próximo, sino solo a una buena aproximación. Para obtener las
    //coordenadas (p_1, p_3) del centroide más próximo debría usarse
    //el algoritmo de Marco Azzaro.

    try {
        //programa el gesto
        MM(int(_p_1), int(_p___3));
    } catch(...) {
        throw;
    }
}

//progrsam go directly from actual position to (x3, y3)
//if (x3, y3) is out the scope of the RP:
//  throw an exception EImproperArgument
void TRoboticPositioner::programGoDirectlyToCartesianP3(double _x3, double _y3)
{
    //traduce las coordenadas cartesianas de S0 a S1
    TDoublePoint _P_3 = getActuator()->S0recToS1rec(_x3, _y3);

    //traduce a polares en S1
    double r_3, theta_3;
    if(_P_3.x!=0 && _P_3.y!=0) {
        r_3 = Mod(_P_3);
        theta_3 = Arg(_P_3);
    } else {
        r_3 = 0;
        theta_3 = M_PI/2;
    }

    //determina las posiciones de los ejes para llevar P3 a (r_3, theta_3)
    //y determina si la posición está dentro del dominio
    double theta_1, theta___3;
    bool isindomine = getActuator()->AnglesToGoP_3(theta_1, theta___3, r_3, theta_3);

    //la posición (x3, y3) debería estar dentro del dominio de P3
    if(!isindomine)
        throw EImproperArgument("position (x3, y3) should be in P3 domain");

    //traduce a pasos
    double _p_1 = getActuator()->getF().Image(theta_1);
    double _p___3 = getActuator()->getArm()->getF().Image(theta___3);

    //cuantifica los pasos
    _p_1 = getActuator()->getQ()[_p_1];
    _p___3 = getActuator()->getArm()->getQ()[_p___3];

    //ADVERTENCIA: la cuantificación de los pasos no dará lugar al centroide
    //más próximo, sino solo a una buena aproximación. Para obtener las
    //coordenadas (p_1, p_3) del centroide más próximo debría usarse
    //el algoritmo de Marco Azzaro.

    try {
        //programa el gesto
        MM(int(_p_1), int(_p___3));
    } catch(...) {
        throw;
    }
}

//MÉTODOS PARA MODIFICAR EL GESTO PROGRAMADO:

//invierte el gesto en el dominio del tiempo
void TRoboticPositioner::InvertTime(void)
{
    //invierte la funcion de movimiento compuesto en el dominio del tiempo
    CMF.InvertTime();
}

//METHODS TO ADD MESSAGE INSTRUCTIONS TO THE MP:

//add a gesture to retract fromactual position to security position
//or to the rotor 1 is in the origin
bool TRoboticPositioner::addMIforRetraction(void)
{
    //determines the necessary displaceement of rot 2 in steps
    double theta___3saf = getActuator()->gettheta___3saf();
    double p___3saf = getActuator()->getArm()->getF().Image(theta___3saf);
    double p___3fin = floor(p___3saf);
    double dp2 = getActuator()->getArm()->getp___3() -  p___3fin;

    //make a rutinary check
    if(p___3fin < 0)
        throw EImpossibleError("final position of rotor 2 can not be less zero");

    //determines the necessary displacement of rot 2 in radians
    double dt2 = getActuator()->getArm()->getG().Image(dp2);

    //calculates the necessary displacement of rotor 1 in radians
    double dt1 = dt2/2;

    //determines the final position of rot 1 in radians
    double theta_1fin, theta___3fin;
    if(getActuator()->gettheta_1() > dt1) {
        theta_1fin = getActuator()->gettheta_1() - dt1;
        theta___3fin = getActuator()->getArm()->gettheta___3() - dt2;

    } else {
        theta_1fin = 0;
        dt1 = getActuator()->gettheta_1();
        dt2 = dt1*2;
        theta___3fin = getActuator()->getArm()->gettheta___3() - dt2;
    }

    //determines the final position of rot 1 in steps
    double p_1fin = getActuator()->getF().Image(theta_1fin);
    p_1fin = Max(0., round(p_1fin));

    //determines the final position of rot 2 in steps
    p___3fin = getActuator()->getArm()->getF().Image(theta___3fin);
    p___3fin = Max(0., round(p___3fin));

    //build a message instruction and set up
    TMessageInstruction *MI = new TMessageInstruction();
    MI->setId(getActuator()->getId());
    MI->Instruction.setName("MM");
    MI->Instruction.Args.setCount(2);
    MI->Instruction.Args[0] = p_1fin;
    MI->Instruction.Args[1] = p___3fin;

    //add the message instruction to the MP
    Positioning::TMessageList *ML = new Positioning::TMessageList();
    ML->Add(MI);
    MP.Add(ML);

    //determines if it is necessary abate the arm for recovery the security position
    bool necessary_abate = !getActuator()->p___3IsInSafeArea(p___3fin);

    //indicates if sill be necessary abate the arm for recovery the security position
    return necessary_abate;
}

//add amessage instruction to abate the arm to security position
void TRoboticPositioner::addMIforAbatement(void)
{
    //get the limit security position of rotor 2
    double theta___3saf = getActuator()->gettheta___3saf();
    //translate to steps
    double p___3saf = getActuator()->getArm()->getF().Image(theta___3saf);
    //determine the firs stable position less than the limit security position
    double p___3fin = floor(p___3saf);

    //make a rutinary check
    if(p___3fin < 0)
        throw EImpossibleError("final position of rotor 2 can not be less zero");

    //build a message instruction and set up
    TMessageInstruction *MI = new TMessageInstruction();
    MI->setId(getActuator()->getId());
    MI->Instruction.setName("M2");
    MI->Instruction.Args.setCount(1);
    MI->Instruction.Args[0] = p___3fin;

    //add the message instruction to the MP
    Positioning::TMessageList *ML = new Positioning::TMessageList();
    ML->Add(MI);
    MP.Add(ML);
}

//MÉTODOS PARA DETERMINAR LA DISTANCIA MÁXIMA RECORRIDA
//POR UN PUNTO DEL BRAZO:

//determina el valor absoluto de
//el ángulo máximo recorrido al moverse el eje 1 durante un tiempo T
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
//al moverse el eje 1 durante un tiempo T
//      d_MaxAbs(T) = theta_MaxAbs(T)*Actuator->r_max;
double TRoboticPositioner::d_MaxAbs(double T)
{
    //el tiempo de desplazamiento T debería ser no negativo
    if(T < 0)
        throw EImproperArgument("displacement time T should be nonnegative");

    return theta_MaxAbs(T)*getActuator()->getr_max();
}

//determina el valor absoluto de
//el ángulo máximo recorrido al moverse el eje 2 durante un tiempo T
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
//al moverse el eje 2 durante un tiempo T
//      d___MaxAbs(T) = theta___MaxAbs(T)*Actuator->Arm->L1V;
double TRoboticPositioner::d___MaxAbs(double T)
{
    //el tiempo de desplazamiento T debería ser no negativo
    if(T < 0)
        throw EImproperArgument("displacement time T should be nonnegative");

    return theta___MaxAbs(T)*getActuator()->getArm()->getL1V();
}

//determina una cota superior para la distancia máxima recorrida
//por un punto del brazo del posicionador, al moverse ambos ejes
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

//lleva los ejes del cilindro adscrito a
//las posiciones correspondientes al instante t
//suponiendo que no hay pérdida de pasos
void TRoboticPositioner::Move(double t)
{
    if(CMF.getMF1()!=NULL && CMF.getMF2()!=NULL)
        getActuator()->SetAnglesSteps(CMF.MF1p(t), CMF.MF2p(t));
    else if(CMF.getMF1()!=NULL && CMF.getMF2()==NULL)
        getActuator()->setp_1(CMF.MF1p(t));
    else if(CMF.getMF1()==NULL && CMF.getMF2()!=NULL)
        getActuator()->getArm()->setp___3(CMF.MF2p(t));
}
//lleva los ejes del cilindro adscrito a
//las posiciones correspondientes al instante 0
//suponiendo que no hay pérdida de pasos
void TRoboticPositioner::MoveSta(void)
{
    if(CMF.getMF1()!=NULL && CMF.getMF2()!=NULL)
        getActuator()->SetAnglesSteps(CMF.getMF1()->getpsta(), CMF.getMF2()->getpsta());
    else if(CMF.getMF1()!=NULL && CMF.getMF2()==NULL)
        getActuator()->setp_1(CMF.getMF1()->getpsta());
    else if(CMF.getMF1()==NULL && CMF.getMF2()!=NULL)
        getActuator()->getArm()->setp___3(CMF.getMF2()->getpsta());
}
//lleva los ejes del cilindro adscrito a
//las posiciones correspondientes al instante Tmax
//suponiendo que no hay pérdida de pasos
void TRoboticPositioner::MoveFin(void)
{
    if(CMF.getMF1()!=NULL && CMF.getMF2()!=NULL)
        getActuator()->SetAnglesSteps(CMF.getMF1()->getpfin(), CMF.getMF2()->getpfin());
    else if(CMF.getMF1()!=NULL && CMF.getMF2()==NULL)
        getActuator()->setp_1(CMF.getMF1()->getpfin());
    else if(CMF.getMF1()==NULL && CMF.getMF2()!=NULL)
        getActuator()->getArm()->setp___3(CMF.getMF2()->getpfin());
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

