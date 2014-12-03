//---------------------------------------------------------------------------
//Archivo: RoboticPositioner.cpp
//Contenido: posicionador básico (dimensión y posición)
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "RoboticPositioner.h"
#include "..\2_Strings\Strings.h" //StrIndent

//#include <values.h>

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres de modelos
namespace Models {

//##########################################################################
//TControlMode
//##########################################################################

void __fastcall StrPrintControlMode(AnsiString& S,
                                    TControlMode cm)
{
    switch(cm) {
    case cmSinc: S +=  AnsiString("Sinc"); break;
    case cmAsinc: S +=  AnsiString("Asinc"); break;
    }
    throw EImposibleError("unknown value in type TControlMode");
}
void __fastcall StrReadControlMode(TControlMode& cm,
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
    throw EImposibleError("unknown value in type TControlMode");
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

void __fastcall StrPrintFaultType(AnsiString& S,
                                  TFaultType ft)
{
    switch(ft) {
    case ftUnk: S +=  AnsiString("Unk"); break;
    case ftSta: S +=  AnsiString("Sta"); break;
    case ftDyn: S +=  AnsiString("Dyn"); break;
    }
    throw EImposibleError("unknown value in type TFaultType");
}
void __fastcall StrReadFaultType(TFaultType& ft,
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
    throw EImposibleError("unknown value in type TFaultType");
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

    __Eo = 0;//_Eo; //asigna el nuevo valor

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
    return BoolToStr_(Disabled, true);
}
void TRoboticPositioner::setDisabledText(const AnsiString &S)
{
    try {
        Disabled = StrToBool_(S);
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
    return BoolToStr_(getOperative(), true);
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
int __fastcall TRoboticPositioner::CompareIds(TRoboticPositioner *FP1,
                                              TRoboticPositioner *FP2)
{
    return TActuator::CompareIds(
                FP1->getActuator(), FP2->getActuator());
}

//imprime el identificador de un posicionador
void __fastcall TRoboticPositioner::PrintId(AnsiString &S, TRoboticPositioner *FP)
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
void __fastcall TRoboticPositioner::PrintOriginsRow(AnsiString& S,
                                                    TRoboticPositioner *FP)
{
    TActuator::PrintOriginsRow(S, FP->getActuator());
}
//lee los valores de las propiedades de origen de un posicionador
//(Id, x0, y0, thetaO1) desde la posición indicada de una cadena
//de texto, en formato fila de texto
void __fastcall TRoboticPositioner::ReadOriginsRow(TRoboticPositioner *FP,
                                                   const AnsiString& S, int &i)
{
    TActuator *A = FP->getActuator();
    TActuator::ReadOriginsRow(A, S,  i);
}

//imprime los valores de las propiedades de posición de un posicionador
//(Id, x3, y3) al final de una cadena de texto
//en formato fila de texto
void __fastcall TRoboticPositioner::PrintPositionP3Row(AnsiString& S,
                                                       TRoboticPositioner *FP)
{
    TActuator::PrintPositionP3Row(S, FP->getActuator());
}
//lee los valores de las propiedades de posición de un posicionador
//(Id, x3, y3) desde la posición indicada de una cadena
//de texto, en formato fila de texto
void __fastcall TRoboticPositioner::ReadPositionP3Row(TRoboticPositioner* &FP,
                                                      const AnsiString& S, int &i)
{
    TActuator *A = FP->getActuator();
    TActuator::ReadPositionP3Row(A, S,  i);
}

//lee una instancia en una cadena
void __fastcall TRoboticPositioner::ReadInstance(TRoboticPositioner* &FP,
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

//construye un posicionador
//con los valores por defecto
TRoboticPositioner::TRoboticPositioner(void) :
    //inicializa las propiedades de tolerancia
    __Eo(0.001), __Ep(0.01),
    __Tstop(1), __Tshiff(1),
    //contruye e inicializa las propiedades de control
    CMF(),
    //inicializa las propiedades de estado
    Disabled(false), __FaultProbability(0), FaultType(ftUnk),
    ControlMode(cmSinc)
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
//construye un posicionador
//con los valores indicados
//si el número de identificación es menor que uno
//lanza una execepción EimproperArgument
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

    //ADVERTENCIA: está permitida la duplicidad de números de identificación Id.

    //contruye el actuador del posicionador
    __Actuator = new TActuator(_Id, _P0, _thetaO1);

    //inicializa las propiedades de control
    CMF.setLabel("CMF");

    //añade el posicionador a la lista de contruidos
    Builts.Add(this);
}

//copia las propiedades de seguridad de un posicionador
void TRoboticPositioner::CopyTolerance(const TRoboticPositioner *FP)
{
    //el puntero FP debe apuntar a un posicionador contruido
    if(FP == NULL)
        throw EImproperArgument("pointer FP should point to built fiberpositioner");

    //copia las tolerancias
    __Eo = FP->getEo();
    __Ep = FP->getEp();
    __Tstop = FP->getTstop();
    __Tshiff = FP->getTshiff();
}
//copia las propiedades de estado de un posicionador
void TRoboticPositioner::CopyStatus(const TRoboticPositioner *FP)
{
    //el puntero FP debe apuntar a un posicionador contruido
    if(FP == NULL)
        throw EImproperArgument("pointer FP should point to built fiber posicionator");

    Disabled = FP->Disabled;
    setFaultProbability(FP->getFaultProbability());
    FaultType = FP->FaultType;
    ControlMode = FP->ControlMode;
}

//copia todas las propiedades de un posicionador
void TRoboticPositioner::Clone(const TRoboticPositioner *FP)
{
    //el puntero FP debe apuntar a un posicionador contruido
    if(FP == NULL)
        throw EImproperArgument("pointer FP should point to built fiber posicionator");

    //copia las propiedades
    getActuator()->Clone(FP->getActuator());
    CMF.Copy(&FP->CMF);
    CopyTolerance(FP);
    CopyStatus(FP);
}
TRoboticPositioner& TRoboticPositioner::operator=(const TRoboticPositioner& FP)
{
    //copia las propiedades
    getActuator()->Clone(FP.getActuator());
    CMF.Copy(&FP.CMF);
    CopyTolerance(&FP);
    CopyStatus(&FP);

    //devuelve la referenciaa este posicionador
    return *this;
}

//contruye un clon de un posicionador
TRoboticPositioner::TRoboticPositioner(const TRoboticPositioner *FP)
{
    //el puntero FP debe apuntar a un posicionador contruido
    if(FP == NULL)
        throw EImproperArgument("pointer FP should point to built fiber posicionator");

    //contruye un clon del actuador del posicionador
    __Actuator = new TActuator(FP->getActuator());
    //copia todas las propiedades del posicionador
    Clone(FP);

    //añade el posicionador a la lista de contruidos
    Builts.Add(this);
}

//libera la memoria dinámica y borra el objeto de Builts
//si el objeto no está en Builts lanza EImproperCall
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
                            IntToHex(int(this), 8));

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
void TRoboticPositioner::SetSPM(TPorpose Porpose,
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

        switch(Porpose) {
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
                        throw EImposibleError("value unknown for property Porpose");
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
//MÉTODOS DE INSTRUCCIÓN:

//gira el cilindro hasta p_1
//si hay un comando programado lanza una excepción EImproperCall
//si la posición es inalcanzable devuelve falso
void TRoboticPositioner::M1(double _p_1)
{
    //la posición p_1 debería estar en el dominio del eje 1
    if(getActuator()->IsntInDomainp_1(_p_1))
        throw EImproperArgument("position p_1 should have in rot 1 domain");

    //programa el giro del cilindro
    CMF.ProgramMF1(getActuator()->getp_1(), _p_1);
}
//gira el brazo hasta p___3
//si no hay un comando programado lanza una excepción EImproperCall
//si la posición es inalcanzable devuelve falso
void TRoboticPositioner::M2(double _p___3)
{
    //la posición p___3 debería estar en el dominio del eje 2
    if(getActuator()->getArm()->IsntInDomainp___3(_p___3))
        throw EImproperArgument("position p___3 should have in rot 2 domain");

    //programa el giro del cilindro
    CMF.ProgramMF2(getActuator()->getArm()->getp___3(), _p___3);
}
//gira ambos ejes hasta (p_1, p___3)
//si no hay un comando programado lanza una excepción EImproperCall
//si la posición es inalcanzable devuelve falso
void TRoboticPositioner::MM(double _p_1, double _p___3)
{
    //las posiciones (p_1, p___3) should has in rotors domain
    if(getActuator()->IsntInDomainp_1(_p_1) || getActuator()->getArm()->IsntInDomainp___3(_p___3))
        throw EImproperArgument("positions (p_1, p___3) should has in rotor domain");

    //programa el giro del cilindro
    CMF.ProgramBoth(getActuator()->getp_1(), getActuator()->getArm()->getp___3(), _p_1, _p___3);
}

//gira el cilindro desde p_1sta hasta p_1fin
//si hay un comando programado lanza una excepción EImproperCall
//si la posición es inalcanzable devuelve falso
void TRoboticPositioner::M1(double p_1sta, double p_1fin)
{
    //la posición p_1sta debería estar en el dominio del eje 1
    if(getActuator()->IsntInDomainp_1(p_1sta))
        throw EImproperArgument("position p_1sta should have in rot 1 domain");
    //la posición p_1fin debería estar en el dominio del eje 1
    if(getActuator()->IsntInDomainp_1(p_1fin))
        throw EImproperArgument("position p_1fin should have in rot 1 domain");

    //programa el giro del cilindro
    CMF.ProgramMF1(p_1sta, p_1fin);
}
//gira el brazo desde p___3sta hasta p___3fin
//si no hay un comando programado lanza una excepción EImproperCall
//si la posición es inalcanzable devuelve falso
void TRoboticPositioner::M2(double p___3sta, double p___3fin)
{
    //la posición p___3sta debería estar en el dominio del eje 2
    if(getActuator()->getArm()->IsntInDomainp___3(p___3sta))
        throw EImproperArgument("position p___3sta should have in rot 2 domain");
    //la posición p___3fin debería estar en el dominio del eje 2
    if(getActuator()->getArm()->IsntInDomainp___3(p___3fin))
        throw EImproperArgument("position p___3fin should have in rot 2 domain");

    //programa el giro del cilindro
    CMF.ProgramMF2(p___3sta, p___3fin);
}
//gira ambos ejes desde (p_1sta, p___3sta) hasta (p_1fin, p___3fin)
//si no hay un comando programado lanza una excepción EImproperCall
//si la posición es inalcanzable devuelve falso
void TRoboticPositioner::MM(double p_1sta, double p___3sta,
                            double p_1fin, double p___3fin)
{
    //las posiciones (p_1sta, p___3sta) should has in rotors domain
    if(getActuator()->IsntInDomainp_1(p_1sta) || getActuator()->getArm()->IsntInDomainp___3(p___3sta))
        throw EImproperArgument("positions (p_1sta, p___3sta) should has in rotors domain");
    //las posiciones (p_1fin, p___3fin) should has in rotors domain
    if(getActuator()->IsntInDomainp_1(p_1fin) || getActuator()->getArm()->IsntInDomainp___3(p___3fin))
        throw EImproperArgument("positions (p_1fin, p___3fin) should has in rotors domain");

    //programa el giro del cilindro
    CMF.ProgramBoth(p_1sta, p___3sta, p_1fin, p___3fin);
}

//detiene el desplazamiento borrando el gesto programado
//      si no hay un gesto programado
//      lanza una excepción EImproperCall
void TRoboticPositioner::SP(void)
{
    //debería haber un gesto programado
    if(CMF.getMF1()==NULL && CMF.getMF2()==NULL)
        throw EImproperCall("should have a programmed gesture");

    //despograma el gesto
    CMF.ClearProgram();
}

//MÉTODOS DE ASIGNACIÓN Y LECTURA DE INSTRUCCIONES:

//asigna una instrucción al controlador
//si la instrucción está vacía lanza una excepción EImproperAgument
void TRoboticPositioner::SetInstruction(TInstruction &Instruction)
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
        //el nombre de la instrucción debería ser conocido
        if(Instruction.getName() == "MS") {
            //ejecuta el comando
            ControlMode = cmSinc;
        } else if(Instruction.getName() == "MA") {
            //ejecuta el comando
            ControlMode = cmAsinc;
        } else if(Instruction.getName() == "M1") {
            //ejecuta el comando
            M1(Instruction.Args.getFirst());
        } else if(Instruction.getName() == "M2") {
            //ejecuta el comando
            M2(Instruction.Args.getFirst());
        } else if(Instruction.getName() == "MM") {
            //ejecuta el comando
            MM(Instruction.Args.getFirst(), Instruction.Args[1]);
        } else if(Instruction.getName() == "SP") {
            //ejecuta el comando
            SP();
        } else
            //indica error imposible
            throw EImposibleError("instruction name should be known");
    } catch(...) {
        throw; //relanza la excepción
    }
}

//lee una instrucción del controlador
//si el controlador no tiene ningún movimiento programado
//      lanza una excepción EImproperCall
void TRoboticPositioner::GetInstruction(TInstruction &Instruction)
{
    if(CMF.getMF1()!=NULL && CMF.getMF2()!=NULL) {
        Instruction.getName() = "MM";
        Instruction.Args.setCount(2);
        Instruction.Args[0] = CMF.getMF1()->getpfin();
        Instruction.Args[1] = CMF.getMF2()->getpfin();
    } else if(CMF.getMF1() != NULL) {
        Instruction.getName() = "M1";
        Instruction.Args.setCount(1);
        Instruction.Args[0] = CMF.getMF1()->getpfin();
    } else if(CMF.getMF2() != NULL) {
        Instruction.getName() = "M2";
        Instruction.Args.setCount(1);
        Instruction.Args[0] = CMF.getMF2()->getpfin();
    } else //CMF.MF1==NULL && CMF.MF2==NULL
        //indica que debería haber alguna funcion de movimiento programada
        throw EImproperCall("should be programmed some motion function");
}

//MÉTODOS DE PROGRAMACIÓN DE GESTOS:

//programa el abatimiento del brazo hasta
//la posición de seguridad estable más próxima
void TRoboticPositioner::TurnArmToSafeArea(void)
{
    //traduce a pasos
    double p___3saf = getActuator()->getArm()->getF().Image(getActuator()->gettheta___3saf());
    //asigna el primer ángulo estable menor o igual que p___3saf
    M2(Max(0., floor(p___3saf)));
}

//programa la retracción del brazo hasta
//la posición de seguridad estable más próxima
void TRoboticPositioner::RetractArmToSafeArea(void)
{
    //CONFIGURA LA VELOCIDAD DE ROT 1 IGUAL A 1/2 DE LA VELOCIDAD DE ROT 2:

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
        //traduce a pasos/ms y la asigna
        CMF.getRF1()->setvmaxabs(vmaxabs1/M_2PI*getActuator()->getSB1());
    } break;
    }

    //Nótese que para traducir la velocidad angular de rad/ms a pasos/ms
    //no debe emplearse la función G de los cuantificadores, cuyo dominio
    //está restringido, por lo que podría ser menor que 1 ms.

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
    //determina la primera posición estable menor o igual
    p_1_ = Max(0., floor(p_1));

    //PROGRAMA EL MOVIMIENTO DE LOS EJES:

    //asigna el primer ángulo estable menor o igual que p___3saf
    MM(p_1_, p___3saf_);
}

//gira el cilindro hasta theta_1
//si la posición es inalcanzable devuelve falso
void TRoboticPositioner::TurnCilinderTotheta_1(double _theta_1)
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
//va directamente a (x_3, y_3)
//si la posición es inalcanzable devuelve falso
void TRoboticPositioner::GoDirectlyToCartesianP_3(double _x_3, double _y_3)
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

//va directamente a (x3, y3)
//si la posición es inalcanzable devuelve falso
void TRoboticPositioner::GoDirectlyToCartesianP3(double _x3, double _y3)
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

//MÉTODOS DE MOVIMEINTO A INSTANTES DE TIEMPO DEL GESTO PROGRAMADO:

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

