//---------------------------------------------------------------------------
//Archivo: Actuator.cpp
//Contenido: actuador de un posicionador
//Última actualización: 07/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Actuator.h"
#include "RoboticPositioner.h"
#include "Strings.h"
#include "Geometry.h"

//#include <values.h>

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//###########################################################################
//TKnowledgeDegree
//###########################################################################

void  StrPrintKnowledgeDegree(AnsiString& S, TKnowledgeDegree kd)
{
    switch(kd) {
    case kdPre: S += AnsiString("Pre"); break;
    case kdApp: S += AnsiString("App"); break;
    case kdUnk: S += AnsiString("Unk"); break;
    }
    throw EImposibleError("unknowledge value in type TKnowledgeValue");
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

    throw EImproperArgument("there is a value of type TKnowledgeValue as from position i in string text S");
}
AnsiString KnowledgeDegreeToStr(TKnowledgeDegree kd)
{
    switch(kd) {
    case kdPre: return AnsiString("Pre");
    case kdApp: return AnsiString("App");
    case kdUnk: return AnsiString("Unk");
    }
    throw EImposibleError("unknowledge value in type TKnowledgeValue");
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

    throw EImproperArgument("there is a value of type TKnowledgeValue as from position i in string text S");
}

//###########################################################################
//TPorpose
//###########################################################################

void  StrPrintPorpose(AnsiString& S, TPorpose p)
{
    switch(p) {
    case pAll: S += AnsiString("All"); break;
    case pGen: S += AnsiString("Gen"); break;
    case pVal: S += AnsiString("Val"); break;
    case pExe: S += AnsiString("Exe"); break;
    }
    throw EImposibleError("unknowledge value in type TPurpose");
}
void  StrReadPorpose(TPorpose& p, const AnsiString &S, int &i)
{
    //ADVERTENCIA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //La longitud de los valores legibles de la cadena son:
    //      strlen("All"): 3
    //      strlen("Pro"): 3
    //      strlen("Val"): 3
    //      strlen("Exe"): 3

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

        //si la subcadena coincide con kdAll
        if(SubS == "All") {
            //traduce de texto a estructura
            p = pAll;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 3;
            return; //no hace nada más
        }

        //si la subcadena coincide con kdGen
        if(SubS == "Gen") {
            //traduce de texto a estructura
            p = pGen;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 3;
            return; //no hace nada más
        }

        //si la subcadena coincide con kdVal
        if(SubS == "Val") {
            //traduce de texto a estructura
            p = pVal;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 3;
            return; //no hace nada más
        }

        //si la subcadena coincide con kdExe
        if(SubS == "Exe") {
            //traduce de texto a estructura
            p = pExe;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 3;
            return; //no hace nada más
        }
    }

    throw EImproperArgument("there is a value of type TPorpose as from position i in string text S");
}
AnsiString PorposeToStr(TPorpose p)
{
    switch(p) {
    case pAll: return AnsiString("All");
    case pGen: return AnsiString("Gen");
    case pVal: return AnsiString("Val");
    case pExe: return AnsiString("Exe");
    }
    throw EImposibleError("unknowledge value in type TPurpose");
}
TPorpose StrToPorpose(const AnsiString& S)
{
    //ADVERTENCIA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //La longitud de los valores legibles de la cadena son:
    //      strlen("All"): 3
    //      strlen("Pro"): 3
    //      strlen("Val"): 3
    //      strlen("Exe"): 3

    //variable tampón
    AnsiString SubS;

    //inicializa elíndice
    int j = 1;
    //avanza el índice auxiliar hasta el próximo caracter no separador
    StrTravelSeparatorsIfAny(S, j);

    //si desde el primera caracter hay al menos 3 caracteres (con él incluido)
    if(j+2 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 3 caracteres
        SubS = S.SubString(j, 3);

        //si la subcadena coincide con kdAll
        if(SubS == "All")
            //devuelve el valor correspondiente
            return pAll;

        //si la subcadena coincide con kdGen
        if(SubS == "Gen")
            //devuelve el valor correspondiente
            return pGen;

        //si la subcadena coincide con kdVal
        if(SubS == "Val")
            //devuelve el valor correspondiente
            return pVal;

        //si la subcadena coincide con kdExe
        if(SubS == "Exe")
            //devuelve el valor correspondiente
            return pExe;
    }

    throw EImproperArgument("there is a value of type TPorpose as from position i in string text S");
}

//###########################################################################
//TActuator
//###########################################################################

//---------------------------------------------------------------------------
//PROPIEDADES ESTÁTICAS:

//inicializa el color con que son contruidos los posicionadores
//#QColor TActuator::DefaultColor = Qt::gray;

//--------------------------------------------------------------------------
//PROPIEDADES DE SEGURIDAD:

//COMPONENTES DE SPM:

void TActuator::setSPMrec(double _SPMrec)
{
    //el SPM de recuperación (SPMrec) debe ser no negativo
    if(_SPMrec < 0)
        throw EImproperArgument("the recovery SPM (SPMrec) should be nonnegative");

    __SPMrec = _SPMrec; //asigna el nuevo valor

    //asimila SPMrec
    AssignSPM();
}
void TActuator::setSPMsta(double _SPMsta)
{
    //el SPM staánico (SPMsta) debe ser no negativo
    if(_SPMsta < 0)
        throw EImproperArgument("the staerical SPM (SPMsta) should be nonnegative");

    __SPMsta = _SPMsta; //asigna el nuevo valor

    //asimila SPMsta
    AssignSPM();
}
void TActuator::setSPMdyn(double _SPMdyn)
{
    //el SPM dinánico (SPMdyn) debe ser no negativo
    if(_SPMdyn < 0)
        throw EImproperArgument("the dynamic SPM (SPMdyn) should be nonnegative");

    __SPMdyn = _SPMdyn; //asigna el nuevo valor

    //asimila SPMdyn
    AssignSPM();
}
void TActuator::setSPMmin(double _SPMmin)
{
    //el SPM de salto mínimo (SPMmin) debe ser no negativo
    if(_SPMmin < 0)
        throw EImproperArgument("the minimum jump SPM (SPMmin) should be nonnegative");

    __SPMmin = _SPMmin; //asigna el nuevo valor

    //asimila SPMmin
    AssignSPM();
}
void TActuator::setSPMoff(double _SPMoff)
{
    //el SPM de desplazamiento (SPMoff) debe ser no negativo
    if(_SPMoff < 0)
        throw EImproperArgument("the offset SPM (SPMoff) should be nonnegative");

    __SPMoff = _SPMoff; //asigna el nuevo valor

    //asimila SPMoff
    AssignSPM();
}

//VALOR DE SPM PARA CADA OCASIÓN:

double TActuator::getSPMexe_p(void) const
{
    return getSPMsta() + getSPMdyn();
}
double TActuator::getSPMval_p(void) const
{
    return getSPMexe_p() + getSPMmin();
}
double TActuator::getSPMgen_p(void) const
{
    return getSPMval_p() + getSPMmin();
}
double TActuator::getSPMall_p(void) const
{
    return getSPMgen_p() + getSPMoff();
}

double TActuator::getSPMexe_a(void) const
{
    return getSPMrec() + getSPMexe_p();
}
double TActuator::getSPMval_a(void) const
{
    return getSPMexe_a() + getSPMmin();
}
double TActuator::getSPMgen_a(void) const
{
    return getSPMval_a() + getSPMmin();
}
double TActuator::getSPMall_a(void) const
{
    return getSPMgen_a() + getSPMoff();
}

//---------------------------------------------------------------------------
//PROPIEDADES DE ESTADO:

void TActuator::setPAkd(TKnowledgeDegree _PAkd)
{
    //si el nuevo valor difiere del actual
    if(_PAkd != getPAkd()) {
        //asigna el nuevo valor
        __PAkd = _PAkd;

        //asimila PAkd
        AssignSPM();
    }
}
void TActuator::setPorpose(TPorpose _Porpose)
{
    //siel nuevo valor difiere del actual
    if(_Porpose != getPorpose()) {
        //asigna el nuevo valor
        __Porpose = _Porpose;

        //asimila Porpose
        AssignSPM();
    }
}

void TActuator::setId(int _Id)
{
    //el número de identificación Id debe ser no negativo
    if(_Id < 0)
        throw EImproperArgument("number identifier Id should be nonnegative");

    //ADVERTENCIA: está permitida la duplicidad de números de identificación Id.

    __Id = _Id; //asigna el nuevo valor
}

//--------------------------------------------------------------------------
//PROPIEDADES DE ÁREA:

void TActuator::setr_3maxnom(double _r_3maxnom)
{
    //el radio nominal del dominio de P3 r_3maxnom debe ser mayor que cero
    if(_r_3maxnom <= 0)
        throw EImproperArgument("nominal domain radio of P3 r_3maxnom should be upper zero");

    //el radio nominal del dominio de P3 r_3maxnom debe ser menor que infinito
    if(MAXDOUBLE <= _r_3maxnom)
        throw EImproperArgument("nominal domain radio of P3 r_3maxnom should be less infinite");

    //el número de posicionadores adyacentes Adjacents.Count debería estar en [0, 6]
    if(Adjacents.getCount()<0 || 6<Adjacents.getCount())
        throw EImproperArgument("adjacent ositioner number Adjacents.Count should be in [0, 6]");

    __r_3maxnom = _r_3maxnom; //asigna el nuevo valor

    //------------------------------------------------------------------

    //renombra el radio para simplificar la notación
    double &r = __r_3maxnom;

    //calcula el área del círculo de radio r
    __Sc = M_PI*r*r;

    //calcula el área de una porción exagonal del círculo de radio r
    __Sw = getSc()/6;

    //calcula el coseno de PI/6
    double aux = sqrt(0.75);

    //calcula el área participativa de una porción triangular
    //del exágono cinrcunscrito en la circunferencia de radio r
    __Sp = r*r*aux/2;

    //calcula el área de uno de los segmentos de circunferencia
    //en las inmediaciones del exágono circunscrito
    //en la circunferencia de radio r
    __Ss = getSw() - getSp(); //= r*r*(M_PI/6 - aux/2)

    //calcula el área exclusiva de una porción exagonal
    //del círculode radio r
    __Se = getSp() - getSs(); //= r*r*(aux - M_PI/6)

    //calcula el ratio entre área exclusiva y el área participativa
    __Re = getSe()/getSp();

    //calcula el número de posicionadores no adyacentes
    aux = double(6 - Adjacents.getCount());

    //calcula el área participativa total
    __Spt = aux*getSw() + Adjacents.getCount()*getSp(); //r*r*(M_PI + N*(sqrt(0.75) - M_PI/3)/2);
    //calcula el área exclusiva total
    __Set = aux*getSw() + Adjacents.getCount()*getSe(); //r*r*(sqrt(0.75) - M_PI/6);
    //calcula el ratio entre Set y Spt
    __Ret = getSet()/getSpt(); //2 - M_PI/(3*sqrt(0.75))
}

//##########################################################################
//PROPIEDADES EN FORMATO TEXTO:
//##########################################################################

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
        setSPMrec(StrToFloat_(S));
    } catch(...) {
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
        setSPMsta(StrToFloat_(S));
    } catch(...) {
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
        setSPMdyn(StrToFloat_(S));
    } catch(...) {
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
        setSPMmin(StrToFloat_(S));
    } catch(...) {
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
        setSPMoff(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

//VALOR DE SPM PARA CADA OCASIÓN:

AnsiString TActuator::getSPMexe_pText(void) const
{
    return FloatToStr(getSPMexe_p());
}
AnsiString TActuator::getSPMval_pText(void) const
{
    return FloatToStr(getSPMval_p());
}
AnsiString TActuator::getSPMgen_pText(void) const
{
    return FloatToStr(getSPMgen_p());
}
AnsiString TActuator::getSPMall_pText(void) const
{
    return FloatToStr(getSPMall_p());
}

AnsiString TActuator::getSPMexe_aText(void) const
{
    return FloatToStr(getSPMexe_a());
}
AnsiString TActuator::getSPMval_aText(void) const
{
    return FloatToStr(getSPMval_a());
}
AnsiString TActuator::getSPMgen_aText(void) const
{
    return FloatToStr(getSPMgen_a());
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
    } catch(...) {
        throw;
    }
}
AnsiString TActuator::getPorposeText(void) const
{
    return PorposeToStr(getPorpose());
}
void TActuator::setPorposeText(const AnsiString &S)
{
    try {
        setPorpose(StrToPorpose(S));
    } catch(...) {
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
        setId(StrToInt_(S));
    } catch(...) {
        throw;
    }
}

AnsiString TActuator::getPendingText(void) const
{
    return BoolToStr_(Pending, true);
}
void TActuator::setPendingText(const AnsiString &S)
{
    try {
        Pending = StrToBool_(S);
    } catch(...) {
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
    } catch(...) {
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

//PROPIEDADES GRÁFICAS EN FORMATO TEXTO:

void TActuator::setLabel(const AnsiString &S)
{
    //la etiqueta Label debe ser una cadena imprimible
    if(!StrIsPrintable(S))
        throw EImproperArgument("label Label should be a printable string");

    __Label = S; //asigna el nuevo valor
}

/*#AnsiString TActuator::getDefaultColorText(void) const
{
    return ColorToString(DefaultColor);
}
void TActuator::setDefaultColorText(const AnsiString &S)
{
    try {
        DefaultColor = StringToColor(S);
    } catch(...) {
        throw;
    }
}

AnsiString TActuator::getColorCilinderText(void) const
{
    return ColorToString(ColorCilinder);
}
void TActuator::setColorCilinderText(const AnsiString &S)
{
    try {
        ColorCilinder = StringToColor(S);
    } catch(...) {
        throw;
    }
}
AnsiString TActuator::getColorArmText(void) const
{
    return ColorToString(ColorArm);
}
void TActuator::setColorArmText(const AnsiString &S)
{
    try {
        ColorArm = StringToColor(S);
    } catch(...) {
        throw;
    }
}
AnsiString TActuator::getColorLimitDomainP3Text(void) const
{
    return ColorToString(ColorLimitDomainP3);
}
void TActuator::setColorLimitDomainP3Text(const AnsiString &S)
{
    try {
        ColorLimitDomainP3 = StringToColor(S);
    } catch(...) {
        throw;
    }
}
AnsiString TActuator::getColorLimitDomainManeuveringText(void) const
{
    return ColorToString(ColorLimitDomainManeuvering);
}
void TActuator::setColorLimitDomainManeuveringText(const AnsiString &S)
{
    try {
        ColorLimitDomainManeuvering = StringToColor(S);
    } catch(...) {
        throw;
    }
}
*/
AnsiString TActuator::getPaintBodyText(void) const
{
    return BoolToStr_(PaintBody, true);
}
void TActuator::setPaintBodyText(const AnsiString &S)
{
    try {
        PaintBody = StrToBool_(S);
    } catch(...) {
        throw;
    }
}
AnsiString TActuator::getPaintLimitDomainP3Text(void) const
{
    return BoolToStr_(PaintLimitDomainP3, true);
}
void TActuator::setPaintLimitDomainP3Text(const AnsiString &S)
{
    try {
        PaintLimitDomainP3 = StrToBool_(S);
    } catch(...) {
        throw;
    }
}
AnsiString TActuator::getPaintLimitDomainManeuveringText(void) const
{
    return BoolToStr_(PaintLimitDomainManeuvering, true);
}
void TActuator::setPaintLimitDomainManeuveringText(const AnsiString &S)
{
    try {
        PaintLimitDomainManeuvering = StrToBool_(S);
    } catch(...) {
        throw;
    }
}

AnsiString TActuator::getSelectedText(void) const
{
    return BoolToStr_(Selected, true);
}
void TActuator::setSelectedText(const AnsiString &S)
{
    try {
        Selected = StrToBool_(S);
    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TActuator::getSecurityText(void) const
{
    AnsiString S;

    //PROPIEDADES DE SEGURIDAD:

    S = "SPMComponents R/W:\r\n";

    S += AnsiString("    SPMrec = ")+getSPMrecText()+AnsiString("\r\n");

    S += AnsiString("    SPMsta = ")+getSPMstaText()+AnsiString("\r\n");
    S += AnsiString("    SPMdyn = ")+getSPMdynText()+AnsiString("\r\n");
    S += AnsiString("    SPMmin = ")+getSPMminText()+AnsiString("\r\n");
    S += AnsiString("    SPMoff = ")+getSPMoffText()+AnsiString("\r\n");

    S += "SPMSpecific R:\r\n";

    S += AnsiString("    SPMexe_p: ")+getSPMexe_pText()+AnsiString("\r\n");
    S += AnsiString("    SPMval_p: ")+getSPMval_pText()+AnsiString("\r\n");
    S += AnsiString("    SPMgen_p: ")+getSPMgen_pText()+AnsiString("\r\n");
    S += AnsiString("    SPMall_p: ")+getSPMall_pText()+AnsiString("\r\n");

    S += AnsiString("    SPMexe_a: ")+getSPMexe_aText()+AnsiString("\r\n");
    S += AnsiString("    SPMval_a: ")+getSPMval_aText()+AnsiString("\r\n");
    S += AnsiString("    SPMgen_a: ")+getSPMgen_aText()+AnsiString("\r\n");
    S += AnsiString("    SPMall_a: ")+getSPMall_aText();

    return S;
}
AnsiString TActuator::getStatusText(void) const
{
    AnsiString S;

    //PROPIEDADES DE ESTADO:

    S = "R/W:\r\n";

    S += AnsiString("    PAkd = ")+getPAkdText()+AnsiString("\r\n");
    S += AnsiString("    Porpose = ")+getPorposeText()+AnsiString("\r\n");

    S += AnsiString("    Id = ")+IntToStr(getId())+AnsiString("\r\n");
    S += AnsiString("    Adjacents = ")+Adjacents.getText()+AnsiString("\r\n");
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
AnsiString TActuator::getGraphicsText(void) const
{
    //PROPIEDADES GRAFÍCAS:

    AnsiString S;

    S = "R/W:\r\n";

    S += AnsiString("    Label = ")+getLabel()+AnsiString("\r\n");
    /*#    S += AnsiString("    ColorCilinder = ")+getColorCilinderText()+AnsiString("\r\n");
    S += AnsiString("    ColorArm = ")+getColorArmText()+AnsiString("\r\n");
    S += AnsiString("    ColorLimitDomainP3 = ")+getColorLimitDomainP3Text()+AnsiString("\r\n");
    S += AnsiString("    ColorLimitDomainManeuvering = ")+getColorLimitDomainManeuveringText()+AnsiString("\r\n");
*/
    S += AnsiString("    PaintBody = ")+getPaintBodyText()+AnsiString("\r\n");
    S += AnsiString("    PaintLimitDomainP3 = ")+getPaintLimitDomainP3Text()+AnsiString("\r\n");
    S += AnsiString("    PaintLimitDomainManeuvering = ")+getPaintLimitDomainManeuveringText()+AnsiString("\r\n");

    S += AnsiString("    Selected = ")+getSelectedText();

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
    S += AnsiString("Area:\r\n")+StrIndent(getAreaText())+AnsiString("\r\n");
    S += AnsiString("Graphics:\r\n")+StrIndent(getGraphicsText());

    return S;
}

AnsiString TActuator::getInstanceText(void) const
{
    AnsiString S;

    //Sizing:
    S += AnsiString("L01 = ")+getL01Text()+AnsiString("\r\n");

    //OrientationRadians:
    S += AnsiString("theta_1min = ")+gettheta_1minText()+AnsiString("\r\n");
    S += AnsiString("theta_1max = ")+gettheta_1maxText()+AnsiString("\r\n");
    S += AnsiString("theta_1 = ")+gettheta_1Text()+AnsiString("\r\n");
    S += AnsiString("theta_O3o = ")+gettheta_O3oText()+AnsiString("\r\n");

    //Quantification:
    S += AnsiString("SB1 = ")+getSB1Text()+AnsiString("\r\n");

    //Arm:
    S += AnsiString("ArmInstance:\r\n")+StrIndent(getArm()->getInstanceText())+AnsiString("\r\n");

    S += AnsiString("PAkd = ")+getPAkdText();

    //Nótese que una instancia de TWall se limita a Barrier->Countour_.Text.
    //la cual deberáser escrite a travñes de Barrier->Contour_Text.

    return S;
}
void TActuator::setInstanceText(const AnsiString& S)
{
    try {
        //contruye una variable tampón
        TActuator aux(this);
        TActuator *A = &aux;
        //lee la instancia y la asigna a la variable tampón
        int i = 1;
        ReadInstance((TActuator*&)A, S, i);

        //avanza el índice i hasta la próxima posición que no contenga un separador
        StrTravelSeparatorsIfAny(S, i);
        //si el índice i indica a algún caracter de la cadena S
        if(i <= S.Length())
            //indica que la cadena S solo debería contener el valor para una instancia
            throw EImproperArgument("string S should contain the instance value only");

        //asigna la variable tampón
        Clone(A);

    } catch(...) {
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
AnsiString TActuator::getPositionPAPRowText(void) const
{
    return getIdText()+AnsiString("\t")+getp_1Text()+AnsiString("\t")+getArm()->getp___3Text();
}

//##########################################################################
//MÉTODOS PRIVADOS:
//##########################################################################

//------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//asigna el SPM adecuado para (PAkd, Porpose)
void TActuator::AssignSPM(void)
{
    //asigna a Arm->SPM o a Barrier->SPM el SPM correspondiente según (PAkd, Porpose)
    switch(getPAkd()) {
    case kdPre: //precise
        switch(getPorpose()) {
        case pAll: getArm()->setSPM(getSPMall_p()); break;
        case pGen: getArm()->setSPM(getSPMgen_p()); break;
        case pVal: getArm()->setSPM(getSPMval_p()); break;
        case pExe: getArm()->setSPM(getSPMexe_p()); break;
        }
        break;;
    case kdApp: //approximated
        switch(getPorpose()) {
        case pAll: getArm()->setSPM(getSPMall_a()); break;
        case pGen: getArm()->setSPM(getSPMgen_a()); break;
        case pVal: getArm()->setSPM(getSPMval_a()); break;
        case pExe: getArm()->setSPM(getSPMexe_a()); break;
        }
        break;
    case kdUnk: //unknowledge
        getBarrier()->setSPM(getSPMsta());
        break;
    }
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
void  TActuator::ReadInstance(TActuator* &A,
                              const AnsiString& S, int &i)
{
    //el puntero A debe apuntar a un actuador construido
    if(A == NULL)
        throw EImproperArgument("pointer A shouldpoint to built actuator");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //estado de la máquina de estados de lectura
    //      0: esperando asignación a L01
    //      1: esperando asignación a theta_1min
    //      2: esperando asignación a theta_1max
    //      3: esperando asignación a theta_1
    //      4: esperando asignación a theta_O3
    //      5: esperando asignación a SB1
    //      6: esperando etiqueta "ArmInstance:"
    //      7: esperando instancia de Arm
    //      8: esperando asignación a PAkd
    //      9: instancia de actuador leida con éxito
    int status = 0;

    //variables tampón
    double _theta_1min, _theta_1max, _theta_1,_theta_O3;
    TActuator _A(A);

    do {
        switch(status) {
        case 0: //esperando asignación a L01
            try {
            StrTravelLabel("L01", S, i);
            StrTravelLabel("=", S, i);
            double aux;
            StrReadFloat(aux, S, i);
            _A.setL01(aux);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 1: //esperando asignación a theta_1min
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("theta_1min", S, i);
            StrTravelLabel("=", S, i);
            StrReadFloat(_theta_1min, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 2: //esperando asignación a theta_1max
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("theta_1max", S, i);
            StrTravelLabel("=", S, i);
            StrReadFloat(_theta_1max, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 3: //esperando asignación a theta_1
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("theta_1", S, i);
            StrTravelLabel("=", S, i);
            StrReadFloat(_theta_1, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 4: //esperando asignación a theta_O3
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("theta_O3o", S, i);
            StrTravelLabel("=", S, i);
            StrReadFloat(_theta_O3, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 5: //esperando asignación a SB1
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("SB1", S, i);
            StrTravelLabel("=", S, i);
            double aux;
            StrReadFloat(aux, S, i);
            _A.SetQuantification(aux);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 6: //esperando la etiqueta "ArmInstance:"
            try {
            StrTravelLabel("ArmInstance:", S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 7: //esperando instancia de Arm
            try {
            TArm *aux = _A.getArm();
            TArm::ReadInstance(aux, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 8: //esperando asignación a PAkd
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("PAkd", S, i);
            StrTravelLabel("=", S, i);
            TKnowledgeDegree aux;
            StrReadKnowledgeDegree(aux, S, i);
            _A.setPAkd(aux);
        }catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientras no se haya leido la instancia con éxito
    } while(status < 9);

    //asigna las variables tampón
    try {
        //aisgna los conjuntos atóicos de valores a la variable tampón
        _A.SetOrientationRadians(_theta_1min, _theta_1max, _theta_1, _theta_O3);

    }catch(...) {
        throw;
    }
    //asigna la variable tampón
    A->Clone(&_A);
}

//obtiene las etiquetas de las propiedades de origen de un posicionador
//("Id", "x0", "y0", "thetaO1") al final de una cadena de texto
//en formato fila de texto
AnsiString TActuator::GetOriginsLabelsRow(void)
{
    return "Id      x0      y0      thetaO1";
}
//atraviesa las etiquetas de las propiedades de origen
//("Id", "x0", "y0", "thetaO1")
//en formato fila de texto
void  TActuator::TravelOriginsLabelsRow(const AnsiString& S, int& i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //estado de la máquina de estados de lectura
    //      0: esperando etiqueta "Id"
    //      1: esperando separador y etiqueta "x0"
    //      2: esperando separador y etiqueta "y0"
    //      3: esperando separador y etiqueta "thetaO1"
    //      4: etiquetas de las propiedades de origen atravesadas con éxito
    int status = 0;

    do {
        switch(status) {
        case 0: //esperando etiqueta "Id"
            try {
            StrTravelLabel("Id", S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 1: //esperando separador y etiqueta "x0"
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("x0", S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 2: //esperando separador y etiqueta "y0"
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("y0", S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 3: //esperando separador y etiqueta "thetaO1"
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("thetaO1", S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientras no se hayan atravesado lasetiquetas de las propiedades de origen con éxito
    } while(status < 4);
}

//imprime los valores de las propiedades de orien de un posicionador
//(Id, x0, y0, thetaO1) al final de una cadena de texto
//en formato fila de texto
void  TActuator::PrintOriginsRow(AnsiString& S, TActuator *A)
{
    //el puntero A debe apuntar a un actuador construido
    if(A == NULL)
        throw EImproperArgument("pointer A should bepoint to built actuator");

    S += A->getOriginsRowText();
}
//lee los valores de las propiedades de orien de un posicionador
//(Id, x0, y0, thetaO1) desde la posición indicada de una cadena
//de texto, en formato fila de texto
void  TActuator::ReadOriginsRow(TActuator* &A,
                                const AnsiString& S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el puntero A debería apauntar a un actuador de fibra construido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //estado de la máquina de estados de lectura
    //      0: esperando valor para Id
    //      1: esperando separador y valor para x0
    //      2: esperando separador y valor para y0
    //      3: esperando separador y valor para thetaO1
    //      4: conjunto devalores leido con éxito
    int status = 0;

    //variables tampón
    int _Id;
    double _x0, _y0;
    double _thetaO1;

    do {
        switch(status) {
        case 0: //esperando valor para Id
            try {
            StrReadInt(_Id, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 1: //esperando separador y valor para x0
            try {
            StrTravelSeparators(S, i);
            StrReadFloat(_x0, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 2: //esperando separador y valor para y0
            try {
            StrTravelSeparators(S, i);
            StrReadFloat(_y0, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 3: //esperando separador y valor para thetaO1
            try {
            StrTravelSeparators(S, i);
            StrReadFloat(_thetaO1, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientras no se haya leido el conjunto de valores con éxito
    } while(status < 4);

    //asigna las variables tampón
    try {
        A->SetOrigins(_Id, _x0, _y0, _thetaO1);
    }catch(...) {
        throw;
    }
}

//obtiene las etiquetas de las propiedades de posición de un posicionador
//("Id", "x3", "y3") al final de una cadena de texto
//en formato fila de texto
AnsiString TActuator::GetPositionP3LabelsRow(void)
{
    return "Id      x3      y3";
}
//obtiene las etiquetas de las propiedades de posición de un posicionador
//("Id", "p_1", "p___3") al final de una cadena de texto
//en formato fila de texto
AnsiString TActuator::GetPositionPAPLabelsRow(void)
{
    return "Id      p_1     p___3";
}
//atraviesa las etiquetas de las propiedades de posición
//("Id", "x3", "y3")
//en formato fila de texto
void  TActuator::TravelPositionP3LabelsRow(const AnsiString& S, int& i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //estado de la máquina de estados de lectura
    //      0: esperando etiqueta "Id"
    //      1: esperando separador y etiqueta "x3"
    //      2: esperando separador y etiqueta "y3"
    //      3: etiquetas de las propiedades de origen atravesadas con éxito
    int status = 0;

    do {
        switch(status) {
        case 0: //esperando etiqueta "Id"
            try {
            StrTravelLabel("Id", S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 1: //esperando separador y etiqueta "x3"
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("x3", S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 2: //esperando separador y etiqueta "y3"
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("y3", S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientras no se hayan atravesado las etiquetas de las propiedades de origen con éxito
    } while(status < 3);
}

//imprime los valores de las propiedades de posición de un posicionador
//(Id, x3, y3) al final de una cadena de texto
//en formato fila de texto
void  TActuator::PrintPositionP3Row(AnsiString& S, TActuator *A)
{
    //el puntero A debe apuntar a un actuador construido
    if(A == NULL)
        throw EImproperArgument("pointer A should bepoint to built actuator");

    S += A->getPositionP3RowText();
}
//lee los valores de las propiedades de posición de un posicionador
//(Id, x3, y3) desde la posición indicada de una cadena
//de texto, en formato fila de texto
void  TActuator::ReadPositionP3Row(TActuator* &A,
                                   const AnsiString& S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el puntero A debería apuntar a un actuador construido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //estado de la máquina de estados de lectura
    //      0: esperando valor para Id
    //      1: esperando separador y valor para x3
    //      2: esperando separador y valor para y3
    //      3: conjunto devalores leido con éxito
    int status = 0;

    //variables tampón
    int _Id;
    double _x3, _y3;

    do {
        switch(status) {
        case 0: //esperando valor para Id
            try {
            StrReadInt(_Id, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 1: //esperando separador y valor para x3
            try {
            StrTravelSeparators(S, i);
            StrReadFloat(_x3, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 2: //esperando separador y valor para y3
            try {
            StrTravelSeparators(S, i);
            StrReadFloat(_y3, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientras no se haya leido el conjunto de valores con éxito
    } while(status < 3);

    //El método de lectura de las posiciones debe asignar el identificador
    //y no comrpobar que coincide, ya que la lectura de una lista puede
    //tener desordenados los elementos.

    //asigna las variables tampón
    try {
        A->SetPositionP3(_Id, _x3, _y3);
    }catch(...) {
        throw;
    }
}
//imprime los valores de las propiedades de posición de un posicionador
//(Id, p_1, p___3) al final de una cadena de texto
//en formato fila de texto
void  TActuator::PrintPositionPAPRow(AnsiString& S, TActuator *A)
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
TActuator::TActuator(int _Id, TDoublePoint _P0, double _thetao_) :
    //construye e inciializa el cilindro
    TCilinder(_P0, _thetao_),
    //inicializa las propiedades de estado
    __PAkd(kdPre), __Porpose(pAll),
    Adjacents(6, TRoboticPositioner::CompareIds, NULL, NULL, TRoboticPositioner::PrintId, NULL),
    Pending(true), Collision(false)
{
    //el número de identificación Id debe ser no negativo
    if(_Id < 0)
        throw EImproperArgument("identification number Id should be nonnegative");

    //ADVERTENCIA: está permitida la duplicidad de números de identificación Id.

    //----------------------------------------
    //inicializa las propiedades de seguridad:

    //inicializa las componentes del SPM:

    __SPMrec = (0.3*getrbs()*getr_max() + 0.3*getArm()->getrbs()*getArm()->getL1V())*0.001;
    __SPMsta = MEGARA_SPMsta;
    __SPMdyn = (0.3*getrbs()*getr_max() + 0.3*getArm()->getrbs()*getArm()->getL1V())*0.001 + (getrbs()*getr_max() + getArm()->getrbs()*getArm()->getL1V());
    __SPMmin = MEGARA_SPMsta;
    __SPMoff = 0.000001*MEGARA_r_max + 0.0001;

    //----------------------------------------
    //inicializa las propiedades de estado

    //inicializa el identificador
    __Id = _Id;

    //----------------------------------------
    //inicializa las cotas útiles

    __r_min = getr_max();
    __r_saf = getr_max();
    __r_2saf = getL02max();
    __theta___2saf = M_PI;
    __theta___3saf = M_PI;
    __theta_2rad = Max(0., gettheta_1min()) + M_PI/2;

    //----------------------------------------
    //inicializa las propiedades de área

    setr_3maxnom(getr_3max());

    //----------------------------------------
    //inicializa las propiedades gráficas
    __Label = "''";
    /*#    ColorCilinder = DefaultColor;
    ColorArm = DefaultColor;
    ColorLimitDomainP3 = DefaultColor;
    ColorLimitDomainManeuvering = DefaultColor;
    ColorCollision = Qt::red;*/
    PaintBody = true;
    PaintLimitDomainP3 = false;
    PaintLimitDomainManeuvering = false;
    Selected = false;
}

//copia las propiedades de seguridad de un actuador
void TActuator::CopySecurity(const TActuator *A)
{
    //el puntero A debe apuntar a un posicinador contruido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built fiberpositioner");

    //copia las componentes de SPM
    __SPMrec = A->__SPMrec;
    __SPMsta = A->__SPMsta;
    __SPMdyn = A->__SPMdyn;
    __SPMmin = A->__SPMmin;
    __SPMoff = A->__SPMoff;
}
//copia las propiedades de estado de un actuador
void TActuator::CopyStatus(const TActuator *A)
{
    //el puntero A debe apuntar a un actuador contruido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");

    __PAkd = A->__PAkd;
    __Porpose = A->__Porpose;

    __Id = A->__Id;
    Adjacents.Clone(A->Adjacents);
    Pending = A->Pending;
    Collision = A->Collision;
}
//copia las propiedades límite de un actuador
void TActuator::CopyLimits(const TActuator *A)
{
    //el puntero A debe apuntar a un actuador contruido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");

    //copia las cotas útiles
    __r_min = A->__r_min;
    __r_saf = A->__r_saf;
    __r_2saf = A->__r_2saf;
    __theta___2saf = A->__theta___2saf;
    __theta___3saf = A->__theta___3saf;
    __theta_2rad = A->__theta_2rad;
}
//copia las propiedades de área de un actuador
void TActuator::CopyArea(const TActuator *A)
{
    //el puntero A debe apuntar a un actuador contruido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built fiber posicionator");

    //copia las propiedades
    __r_3maxnom = A->__r_3maxnom;
    __Sc = A->__Sc;
    __Sw = A->__Sw;
    __Sp = A->__Sp;
    __Ss = A->__Ss;
    __Se = A->__Se;
    __Re = A->__Re;
    __Spt = A->__Spt;
    __Set = A->__Set;
    __Ret = A->__Ret;
}
//copia las propiedades gráficas de un actuador
void TActuator::CopyGraphics(const TActuator *A)
{
    //el puntero A debe apuntar a un actuador contruido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built fiber posicionator");

    //copia las propiedades
    __Label = A->__Label;
    /*#    ColorCilinder = A->ColorCilinder;
    ColorArm = A->ColorArm;
    ColorLimitDomainP3 = A->ColorLimitDomainP3;
    ColorLimitDomainManeuvering = A->ColorLimitDomainManeuvering;
    ColorCollision = A->ColorCollision;*/
    PaintBody = A->PaintBody;
    PaintLimitDomainP3 = A->PaintLimitDomainP3;
    PaintLimitDomainManeuvering = A->PaintLimitDomainManeuvering;
    Selected = A->Selected;
}

//copia todas las propiedades de un actuador
void TActuator::Clone(const TActuator *A)
{
    //el puntero A debería apuntar a un actuador construido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");

    //copia las propiedades del cilindro
    CopyCilinder(A);

    //copia las propiedades de seguridad
    CopySecurity(A);
    //copia las propiedades de estado
    CopyStatus(A);
    //copia las propiedades límite
    CopyLimits(A);
    //copia las propiedades gráficas
    CopyGraphics(A);
}

//construye un clon de un actuador
TActuator::TActuator(const TActuator *A) :
    TCilinder(A)
{
    //el puntero A debe apuntar a un actuador contruido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");

    //copia las propiedades de seguridad
    CopySecurity(A);
    //copia las propiedades de estado
    CopyStatus(A);
    //copia las propiedades límite
    CopyLimits(A);
    //copia las propiedades de área
    CopyArea(A);
    //copia las propiedades gráficas
    CopyGraphics(A);
}

//destruye un actuador
TActuator::~TActuator()
{
}

//---------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//calcula las propiedades de seguridad:
//      {r_min, r_saf, r_2saf, theta___2saf, theta___3saf, theta_2rad}
void TActuator::CalculateSafeParameters(void)
{
    //si hay posicionadores adyacentes
    if(Adjacents.getCount() > 0) {
        //contruye un clon del actuador para realizar las simulaciones
        TActuator A(this);

        //asigna el SPM de generación con grado de conocimiento aproximado
        A.getArm()->setSPM(getSPMgen_a());

        //Nótese que los parámetros de seguridad serán válidos
        //para la programación del movimiento, incluso cuando
        //las posiciones de los ejes sean conocidas de manera
        //aproximada; pero no serán válidos durante las asignaciones,
        //cuando el SPMall que corresponda supere al SPMgen_a.

        //activa la cuantificación de los ejes
        A.EnableQuantification();
        //mueve los ejes a sus posiciones de origen
        A.setp_1(0);
        A.getArm()->setp___3(0);
        //deteremina r_min
        __r_min = A.getArm()->getContour().DistanceMax(A.getP0());

        TActuator *AA;
        double _r_saf;
        //por cada posicionador adyacente
        for(int i=0; i<Adjacents.getCount(); i++) {
            //apunta el actuador indicado para facilitar su acceso
            AA = Adjacents[i]->getActuator();

            //calcula la distancia de seguridad
            double aux = Mod(AA->getP0() - A.getP0());
            _r_saf = aux - AA->getr_max() - AA->getSPMgen_a() - A.getArm()->getSPM();

            //ADVERTENCIA: el SPM del brazo de los actuadores
            //adyacentes no ha sido modificado, de ahí que se
            //reste AA->SPMgen_a en vez de AA->Arm->SPM.

            //actualiza r_saf
            if(_r_saf < getr_saf())
                __r_saf = _r_saf;

            //Nótese que r_saf podría salir negativo.
        }

        //Busca la posición del eje 2 a partir de la cual
        //el contorno no invade el espacio más allá de r_saf:

        //como la evaluación es computacionalmente costosa
        //conviene hacer una búsqueda binaria en (0, M_PI)

        //desactiva la cuantificación
        A.DisableQuantification();

        //determina los límites del intervalo de búsqueda
        double theta___2min = M_PI - A.gettheta_O3o() + Max(0., A.getArm()->gettheta___2min());
        double theta___2max = M_PI - A.gettheta_O3o() + Min(M_PI, A.getArm()->gettheta___2max());

        //Nótese que aunque las coordenadas angulares se refieren a P2,
        //están dadas en S3, razón por la cual, debe sumarse M_PI - theta_O3o,
        //para que el brazo quede alineado con el eje 1-0.

        //calcula el punto medio del intervalo de búsqueda
        double theta___2 = (theta___2max + theta___2min)/2;
        //mueve el eje 2 al punto medio del intervalo de búsqueda
        A.getArm()->settheta___2(theta___2);
        //calcula el radio de la envolvente descrita por el contorno del brazo
        double r_max = A.getArm()->getContour().DistanceMax(A.getP0());
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
                //mueve el eje 2 al punto medio del intervalo de búsqueda
                A.getArm()->settheta___2(theta___2);
                //calcula la distancia máxima
                r_max = A.getArm()->getContour().DistanceMax(A.getP0());
            }

            j++; //contabiliza la iteración
        }

        //Nótese que se realizan 4 iteraciones adicionales
        //paracompensar el hecho de que el intervalo
        //contiene M_PI unidades de double.

        //registra las corodenadas angulares
        __r_2saf = A.getr_2();
        __theta___3saf = __theta___2saf = A.getArm()->gettheta___2();
    }

    //si no hay posicionadores adyacentes
    else {
        //restaura el valor por defecto de los parámetros de seguridad
        __r_min = getr_max();
        __r_saf = getr_max();
        __r_2saf = getr_2();
        __theta___2saf = M_PI;
        __theta___3saf = M_PI;
    }
    __theta_2rad = Max(0., gettheta_1min()) + M_PI/2;
}

//-------------------------------------------------------------------
//MÉTODOS DE ASIGNACIÓN CONJUNTA:

//asigna las propiedades angulares (theta_1min, theta_1max,theta_1)
void TActuator::SetOrientationRadians(double _theta_1min, double _theta_1max,
                                      double _theta_1, double _theta_O3o)
{
    //el ángulo theta_1min debe estar en el dominio de F(theta_1)
    if(!getF().BelongToDomain(_theta_1min))
        throw EImproperArgument("angle theta_1min should be in domain F(theta_1)");

    //el ángulo theta_1max debe estar en el domaxio de F(theta_1)
    if(!getF().BelongToDomain(_theta_1max))
        throw EImproperArgument("angle theta_1max should be in domain F(theta_1)");

    //el ángulo theta_1min no debe ser mayor que el ángulo theta_1max
    if(_theta_1min > _theta_1max)
        throw EImproperArgument("angle theta_1min should not be upper than angle theta_1max");

    //el ángulo theta_1 debe estar en su dominio [theta_1min, theta_1max]
    if(IsntInDomaintheta_1(_theta_1))
        throw EImproperArgument("angle theta_1 should be in his domain [theta_1min, theta_1max]");

    //PROCESA [theta_1min, theta_1max]:

    //asignalos nuevos límites del intervalo
    __theta_1min = _theta_1min;
    __theta_1max = _theta_1max;

    //configura la función de cuantificación
    __Q.Set(1, getp_1min(), getp_1max());

    //PROCESA (theta_1, theta_O3):

    //si la cuantificación está activada
    if(getQuantify_())
        //cuantifica el nuevo valor
        _theta_1 = Qtheta_1(_theta_1);

    //ADVERTENCIA: la cuantificación podría dar lugar a que theta_1
    //quedara ligeramente fuera de su dominio.

    //constriñe theta_1 a su dominio
    if(_theta_1 < gettheta_1min())
        _theta_1 = gettheta_1min();
    else if(_theta_1 > gettheta_1max())
        _theta_1 = gettheta_1max();

    //asigna los nuevos valores
    __theta_1 = _theta_1;
    __theta_O3o = _theta_O3o;

    //cambiar la orientación del posicinador
    //puede modificar la posición y orientación
    //del origen de coordenadas del brazo

    //asimila (theta_1, theta_O3o)
    getArm()->Set(NewP1(), getthetaO3());
    getBarrier()->Set(getP0(), getthetaO1());
}
//asigna las propiedades de cuantificación
void TActuator::SetQuantification(double _SB1)
{
    //DEFINE LA FUNCIÓN F A PARTIR DE SB1:

    //inicializa la función
    __F.Clear();
    //añade los puntos de la función de compresión
    __F.Add(-M_2PI, -double(_SB1));
    __F.Add(0., 0.);
    __F.Add(M_2PI, double(_SB1));
    __F.Add(double(2*M_2PI), double(2*_SB1));
    //elimina las incongruencias del formato texto
    //de la función de compansión
    __F.setPointsText(getF().getPointsText());

    //asimila F
    ProcessateF();
    settheta_1(gettheta_1());
}

//asigna las propiedades de origen
void TActuator::SetOrigins(int _Id, double _x0, double _y0, double _thetaO1)
{
    //el número de identificación Id debe ser mayor que cero
    if(_Id <= 0)
        throw EImproperArgument("identificator number Idshould beupper zero");

    //asigna los nuevos valores
    __Id = _Id;
    __P0.x = _x0;
    __P0.y = _y0;
    __thetaO1 = _thetaO1;

    //desplazar el posicionador cambiará la ubicación del brazo
    //pero no su orientación

    //cambiar la orientación del posicinador
    //modifica la posición y orientación del origen de coordenadas del brazo

    /*        //asimila P0:
        __L0 = P0.Mod();
        if(L0 != 0)
                __theta0 = P0.Arg();
        else
                __theta0 = 0;
  */
    //asimila (P0, thetaO1)
    getArm()->Set(NewP1(), getthetaO3());
    getBarrier()->Set(getP0(), getthetaO1());
}
//asigna las propiedades de posición
//si el punto (x3, y3) no está en el dominio del actuador
//      lanza EImproperArgument
void TActuator::SetPositionP3(double x3, double y3)
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

    //el punto a asignar debería estar en el dominio del posicionador
    if(!AnglesToGoP_3(theta_1, theta___3, r_3, theta_3))
        throw EImproperFileLoadedValue("point to assign should be in the domine of the fiber positioner");

    //asigna el punto
    SetAnglesRadians(theta_1, theta___3);
}
//asigna las propiedades de identificación y posición
//si el punto (x3, y3) no está en el dominio del actuador
//      lanza EImproperArgument
void TActuator::SetPositionP3(int _Id, double x3, double y3)
{
    //el número de identificación Id debe ser no negativo
    if(_Id < 0)
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

    //el punto a asignar debería estar en el dominio del posicionador
    if(!AnglesToGoP_3(theta_1, theta___3, r_3, theta_3))
        throw EImproperFileLoadedValue(AnsiString("point to assign should be in the domine of the RP ")+IntToStr(_Id));

    //asigna el identificador
    __Id = _Id;
    //asigna el punto
    SetAnglesRadians(theta_1, theta___3);
}

//asigna las componentes del SPM
void TActuator::SetSPMComponents(double _SPMrec, double _SPMsta,
                                 double _SPMdyn, double _SPMmin)//, double _SPMoff)
{
    //el SPM de recuperación (SPMrec) debe ser no negativo
    if(_SPMrec < 0)
        throw EImproperArgument("the recovery SPM (SPMrec) should be nonnegative");

    //el SPM estático (SPMsta) debe ser no negativo
    if(_SPMsta < 0)
        throw EImproperArgument("the static SPM (SPMsta) should be nonnegative");

    //el SPM dynánico (SPMdyn) debe ser no negativo
    if(_SPMdyn < 0)
        throw EImproperArgument("the dynamic SPM (SPMdyn) should be nonnegative");

    //el SPM de salto mínimo (SPMmin) debe ser no negativo
    if(_SPMmin < 0)
        throw EImproperArgument("the minimum jump SPM (SPMmin) should be nonnegative");

    /*        //el SPM de corrección del desplazamiento (SPMoff) debe ser no negativo
        if(_SPMoff < 0)
                throw EImproperArgument("the offset SPM (SPMoff) should be nonnegative");
  */
    __SPMrec = _SPMrec; //asigna el nuevo valor
    __SPMsta = _SPMsta; //asigna el nuevo valor
    __SPMdyn = _SPMdyn; //asigna el nuevo valor
    __SPMmin = _SPMmin; //asigna el nuevo valor
    //        __SPMoff = _SPMoff; //asigna el nuevo valor

    //asimila las componentes de SPM
    AssignSPM();
}

//asigna las propiedades de estado (PAkd, Porpose, Faul, disabled)
void TActuator::SetStatus(TKnowledgeDegree _PAkd, TPorpose _Porpose)
{
    //asigna los valores
    __PAkd = _PAkd;
    __Porpose = _Porpose;

    //ASIMILA LOS VALORES:

    //asigna a Arm->SPM o a Barrier->SPM el SPM correspondiente según (PAkd, Porpose)
    switch(getPAkd()) {
    case kdPre:
        switch(getPorpose()) {
        case pAll: getArm()->setSPM(getSPMall_p()); break;
        case pGen: getArm()->setSPM(getSPMgen_p()); break;
        case pVal: getArm()->setSPM(getSPMval_p()); break;
        case pExe: getArm()->setSPM(getSPMexe_p()); break;
        }
        break;
    case kdApp:
        switch(getPorpose()) {
        case pAll: getArm()->setSPM(getSPMall_a()); break;
        case pGen: getArm()->setSPM(getSPMgen_a()); break;
        case pVal: getArm()->setSPM(getSPMval_a()); break;
        case pExe: getArm()->setSPM(getSPMexe_a()); break;
        }
        break;
    case kdUnk:
        getBarrier()->setSPM(getSPMsta());
        break;
    }
}

//-------------------------------------------------------------------
//MÉTODOS PARA ALMACENAR Y RECUPERAR POSICIONES DE LOS ROTORES:

//apila las posiciones de los rotores
void TActuator::Pushthetas(void)
{
    theta_1s.Add(gettheta_1());
    getArm()->Pushtheta___3();
}
//restaura las últimas posiciones apuiladas de los rotores
//si no hay una posisición almacenada para algún rotor:
//      lanza EImproperCall
void TActuator::Restorethetas(void)
{
    //la pila theta_1s debería contener algún valor apilado
    if(theta_1s.getCount() < 1)
        throw EImproperCall("stack theta_1s should contain some stacking value");
    //la pila Arm->theta___3s debería contener algún valor apilado
    if(getArm()->theta___3s.getCount() < 1)
        throw EImproperCall("stack Arm->theta___3s should contain some stacking value");

    //asigna el último valor apilado
    settheta_1(theta_1s.getLast());
    getArm()->Restoretheta___3();
}
//desempila las últimas posiciones apiladas de los rotores
//si no hay una posisición almacenada para algún rotor:
//      lanza EImproperCall
void TActuator::Popthetas(void)
{
    //la pila theta_1s debería contener algún valor apilado
    if(theta_1s.getCount() < 1)
        throw EImproperCall("stack theta_1s should contain some stacking value");
    //la pila Arm->theta___3s debería contener algún valor apilado
    if(getArm()->theta___3s.getCount() < 1)
        throw EImproperCall("stack Arm->theta___3s should contain some stacking value");

    //borra el último valor apilado
    theta_1s.Delete(theta_1s.getCount() - 1);
    getArm()->Poptheta___3();
}
//restaura y desempila las últimas posiciones apiladas de los rotores
//si no hay una posisición almacenada para algún rotor:
//      lanza EImproperCall
void TActuator::RestoreAndPopthetas(void)
{
    //la pila theta_1s debería contener algún valor apilado
    if(theta_1s.getCount() < 1)
        throw EImproperCall("stack theta_1s should contain some stacking value");
    //la pila Arm->theta___3s debería contener algún valor apilado
    if(getArm()->theta___3s.getCount() < 1)
        throw EImproperCall("stack Arm->theta___3s should contain some stacking value");

    //asigna el último valor apilado
    settheta_1(theta_1s.getLast());
    getArm()->Restoretheta___3();

    //borra el último valor apilado
    theta_1s.Delete(theta_1s.getCount() - 1);
    getArm()->Poptheta___3();
}

//ADVERTENCIA: la recuperación de posiciones no estables,
//cuando está la cuantificación activada, dará lugar
//al desplazamiento a la posición estable más próxima de cada rotor.

//-------------------------------------------------------------------
//MÉTODOS PARA ALMACENAR Y RECUPERAR ESTADOS DE CUNATIFICACIÓN:

//apila el estado de los cuantificadores
void TActuator::PushQuantifys(void)
{
    Quantify_s.Add(getQuantify_());
    getArm()->PushQuantify___();
}
//restaura el último estado apilado de los cuantificadores
//si no hay un estado almacenado para algún quantificador:
//      lanza EImproperCall
void TActuator::RestoreQuantifys(void)
{
    //la pila Quantify_s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Quantify_s should contain some stacking value");
    //la pila Arm->Quantify___s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Arm->Quantify___s should contain some stacking value");

    //asigna el último valor apilado
    setQuantify_(Quantify_s.getLast());
    getArm()->RestoreQuantify___();
}
//desempila el último estado apilado de los cuantificadores
//si no hay un estado almacenado para algún quantificador:
//      lanza EImproperCall
void TActuator::PopQuantifys(void)
{
    //la pila Quantify_s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Quantify_s should contain some stacking value");
    //la pila Arm->Quantify___s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Arm->Quantify___s should contain some stacking value");

    //borra el último valor apilado
    Quantify_s.Delete(Quantify_s.getCount() - 1);
    getArm()->PopQuantify___();
}
//restaura y desempila el último estado apilado de los cuantificadores
//si no hay un estado almacenado para algún quantificador:
//      lanza EImproperCall
void TActuator::RestoreAndPopQuantifys(void)
{
    //la pila Quantify_s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Quantify_s should contain some stacking value");
    //la pila Arm->Quantify___s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Arm->Quantify___s should contain some stacking value");

    //asigna el último valor apilado
    setQuantify_(Quantify_s.getLast());
    getArm()->RestoreQuantify___();

    //borra el último valor apilado
    Quantify_s.Delete(Quantify_s.getCount() - 1);
    getArm()->PopQuantify___();
}

//--------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR LA POSICIÓN RELATIVA DEL BRAZO:

//determina si un ángulo del eje 2 en radianes
//está fuera del área de seguridad
bool TActuator::theta___3IsOutSafeArea(double theta___3)
{
    if(theta___3 > gettheta___3saf())
        return true;

    return false;
}
//determina si un ángulo del eje 2 en radianes
//está dentro del área de seguridad
bool TActuator::theta___3IsInSafeArea(double theta___3)
{
    if(theta___3 <= gettheta___3saf())
        return true;

    return false;
}

//determina si un ángulo del eje 2 en pasos
//está fuera del área de seguridad
bool TActuator::p___3IsOutSafeArea(double p___3)
{
    if(p___3 > getArm()->getF().Image(gettheta___3saf()))
        return true;

    return false;
}
//determina si un ángulo del eje 2 en pasos
//está dentro del área de seguridad
bool TActuator::p___3IsInSafeArea(double p___3)
{
    if(p___3 <= getArm()->getF().Image(gettheta___3saf()))
        return true;

    return false;
}

//determina si un punto está fuera del área de seguridad
bool TActuator::P3IsOutSafeArea(TDoublePoint P)
{
    //calcula las posiciones angulares para ir al punto
    double theta_1, theta___3;
    if(!AnglesToGoP3(theta_1, theta___3, P.x, P.y))
        return true;

    //determina si theta___3 está fuera del área de seguridad
    return theta___3IsOutSafeArea(theta___3);
}
//determina si un punto está dentro del área de seguridad
bool TActuator::P3IsInSafeArea(TDoublePoint P)
{
    //calcula las posiciones angulares para ir al punto
    double theta_1, theta___3;
    if(!AnglesToGoP3(theta_1, theta___3, P.x, P.y))
        return false;

    //determina si theta___3 está fuera del área de seguridad
    return theta___3IsInSafeArea(theta___3);
}

//determina si el brazo está fuera del área deseguridad
bool TActuator::ArmIsOutSafeArea(void)
{
    if(getArm()->gettheta___2() > gettheta___2saf())
        return true;
    else
        return false;
}
//determina si el brazo está dentro del área de seguridad
bool TActuator::ArmIsInSafeArea(void)
{
    if(getArm()->gettheta___2() <= gettheta___2saf())
        return true;
    else
        return false;
}

//PUNTO DE INFLEXIÓN PARA UN ÁNGULO DEL EJE 1:

//Coordenada radial del punto de inflexión para la pose dada en S1.
//Pi es el punto de la trayectoria radial de P2 en que:
//      theta_1 == Max{theta_1min, 0}.
double TActuator::Getr_i(double &theta_1, double theta_2)
{
    //la distancia L01 debe ser igual a la distancia L12
    if(getL01() != getArm()->getL12())
        throw EImproperCall("distance L01 should be equal distance Arm->L12");

    //el ángulo theta_2 debe estar en [Max(0, theta_1min), Min(M_2PI, theta_1max)+M_PI/2]
    if(theta_2<Max(0., gettheta_1min()) || Min(M_2PI, gettheta_1max())+M_PI/2<theta_2)
        throw EImproperArgument("angle theta_2 should be in [Max(0, theta_1min), Min(M_2PI, theta_1max)+M_PI/2]");

    //si el punto de inflexión está fuera de la zona segura
    if(theta_2 < gettheta_2rad()) {
        //el eje 1 se retraerá hasta cero
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
        //el eje 1 se retraerá hasta la dirección noramal a la radial
        theta_1 = theta_2 - M_PI/2;

        //devuelve r_i
        return fabs(getL01() - getArm()->getL12());
    }
}
//ángulo del eje 2 respecto de S2 en radianes
//para el punto de inflexión de la pose dada
double TActuator::Gettheta___2i(double theta_2, double r_i)
{
    //la distancia r_i debe ser no negativa
    if(r_i < 0)
        throw EImproperArgument("distance r_i should be nonnegative");

    //el ángulo theta_2 debe estar en [Max(0, theta_1min), Min(M_2PI, theta_1max)+M_PI/2]
    if(theta_2<Max(0., gettheta_1min()) || Min(M_2PI, gettheta_1max())+M_PI/2<theta_2)
        throw EImproperArgument("angle theta_2 should be in [Max(0, theta_1min), Min(M_2PI, theta_1max)+M_PI/2]");

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
void TActuator::MoveArmToSafePosition(void)
{
    if(getArm()->getQuantify___())
        getArm()->Settheta___2FirstStableLessOrEqual(gettheta___2saf());
    else
        getArm()->settheta___2(gettheta___2saf());
}

//--------------------------------------------------------------------------
//MÉTODOS PARA EL CÁLCULO DE DISTANCIAS:

//determina la distancia con otro actuador
double TActuator::Distance(TActuator *AA)
{
    //el puntero AA debería apuntar a un actuador construido
    if(AA == NULL)
        throw EImproperArgument("pointer AA should point to built actuator");

    //calcula la distancia entre el contorno del obstáculo
    //de este actuador y el del actuador adyacente:

    double D1; //distancia de este al adyacente
    double D2; //distancia del adyacente a este

    //si la posición angular de ambos ejes de este posicionador es conocida
    if(getPAkd() != kdUnk) {
        //si la posición angular de ambos ejes del posicionador adyacente es conocida
        if(AA->getPAkd() != kdUnk) {
            //calcula la distancia entre contornos
            //this->Arm -> AA->Arm
            D1 = getArm()->getContour().DistanceMin(AA->getArm()->getContour());
            D2 = AA->getArm()->getContour().DistanceMin(getArm()->getContour());
        }
        //si la posición angular de algún eje del posicionador adyacente es desconocida
        else {
            //calcula la distancia entre contornos
            //this->Arm -> AA->Barrier
            D1 = getArm()->getContour().DistanceMin(AA->getBarrier()->getContour());
            D2 = AA->getBarrier()->getContour().DistanceMin(getArm()->getContour());
        }
    }
    //si la posición angular de algún eje de este posicionador es desconocida
    else {
        //si la posición angular de ambos ejes del posicionador adyacente es conocida
        if(AA->getPAkd() != kdUnk) {
            //calcula la distancia entre contornos
            //this->Barrier -> AA->Arm
            D1 = getBarrier()->getContour().DistanceMin(AA->getArm()->getContour());
            D2 = AA->getArm()->getContour().DistanceMin(getBarrier()->getContour());
        }
        //si la posición angular de algún eje del posicionador adyacente es desconocida
        else {
            //calcula ladistancia entre contornos
            //this->Barrier -> AA->Barrier
            D1 = getBarrier()->getContour().DistanceMin(AA->getBarrier()->getContour());
            D2 = AA->getBarrier()->getContour().DistanceMin(getBarrier()->getContour());
        }
    }

    return Min(D1, D2); //devuelve la distancia mínima

    //tomar la distancia mínima constituye una corrección del error numérico
}
//determina la distancia libre con otro actuador
//      Df = D - SPM1 - SPM2
//donde:
//      Df: distancia libre
//      SPM1: SPM de este actuador (según PAkd)
//      SPM2: SPM del otro actuador (según PAkd)
double TActuator::DistanceFree(TActuator *A)
{
    //el puntero A debería apuntar a un actuador construido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");

    //caulcula la distancia libre
    double Df = Distance(A) - getArm()->getSPM() - A->getArm()->getSPM();

    return Df; //devuelve la distancia libre
}

//determina la distancia mínima entre el brazo del posicionador
//y los brazos de los posicionadores adyacentes
double TActuator::DistanceWithAdjacent(void)
{
    TActuator *AA;
    double dmin = MAXDOUBLE;
    double d;

    //si hay algún adyacente
    if(Adjacents.getCount() > 0)
        //por cada posicionador adyacente
        for(int i=0; i<Adjacents.getCount(); i++) {
            //apunta el posicioandor indicado para facilitar su acceso
            AA = Adjacents[i]->getActuator();
            //determina la distancia entre brazos
            d = getArm()->getContour().DistanceMin(AA->getArm()->getContour());
            //si la distancia es menor que la mínima
            if(d < dmin)
                dmin = d; //actualiza la distancia mínima
        }

    return dmin;
}
//determina la distancia mínima entre el punto P3 del posicionador
//y los puntos P3 de los posicionadores adyacentes
double TActuator::DistanceP3WithAdjacent(void)
{
    TActuator *AA;
    double dmin = MAXDOUBLE;
    double d;

    //si hay algún adyacente
    if(Adjacents.getCount() > 0)
        for(int i=0; i<Adjacents.getCount(); i++) {
            //apunta el posicionador indicado para facilitar su acceso
            AA = Adjacents[i]->getActuator();
            //determina la distancia entre puntos P3
            d = Mod(AA->getArm()->getP3() - getArm()->getP3());
            //si la distancia es menor que la mínima
            if(d < dmin)
                dmin = d; //actualiza la distancia mínima
        }

    return dmin;
}

//DETERMINACIÓN DE POSICIÓN RELATIVA:

//determina si un punto está fuera del área invasiva
bool TActuator::P3IsOutNoninvasiveArea(TDoublePoint P)
{
    //traduce el punto a coordenadas relativas a S1
    TDoublePoint P_ = S0recToS1rec(P.x, P.y);

    //si el punto está fuera del dominio
    if(PointIsOutDomainP_3(P_))
        //indica que el punto está fuera del área no invasiva
        return true;

    //guar la configuración original
    double p_1bak = getp_1();
    double p___3bak = getArm()->getp___3();
    bool Quantify_bak = getQuantify_();
    bool Quantify___bak = getArm()->getQuantify___();

    //desactiva la cuantificación de los ejes
    setQuantify_(false);
    getArm()->setQuantify___(false);

    //mueve el brazo al punto
    SetCartesianP_3(P_.x, P_.y);

    TActuator *AA;
    double d;

    //por cada posicionador adyacente
    for(int i=0; i<Adjacents.getCount(); i++) {
        //apunta el posicioandor indicado para facilitar su acceso
        AA = Adjacents[i]->getActuator();

        //determina la distancia mínimaentre el brazo y
        //el centro del posicionador adyacente
        d = getArm()->getContour().DistanceMin(AA->getP0());

        //si la distancia es menor que el SPM+AA->SPM+AA->r_max
        if(d < getArm()->getSPM()+AA->getArm()->getSPM()+AA->getr_max()) {
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
    if(PointIsOutDomainP_3(P_))
        //indica que el punto no está dentro del área no invasiba
        return false;

    //guarda la configuración original
    double p_1bak = getp_1();
    double p___3bak = getArm()->getp___3();
    bool Quantify_bak = getQuantify_();
    bool Quantify___bak = getArm()->getQuantify___();

    //desactiva la cuantificación de los ejes
    setQuantify_(false);
    getArm()->setQuantify___(false);

    //mueve el brazo al punto
    SetCartesianP_3(P_.x, P_.y);

    TActuator *AA;
    double d;

    //por cada posicionador adyacente
    for(int i=0; i<Adjacents.getCount(); i++) {
        //apunta el posicioandor indicado para facilitar su acceso
        AA = Adjacents[i]->getActuator();

        //determina la distancia mínimaentre el brazo y
        //el centro del posicionador adyacente
        d = getArm()->getContour().DistanceMin(AA->getP0());

        //si la distancia es menor que SPM+AA->SPM+AA->r_max
        if(d < getArm()->getSPM()+AA->getArm()->getSPM()+AA->getr_max()) {
            //restaura la configuración original
            setQuantify_(Quantify_bak);
            getArm()->setQuantify___(Quantify___bak);
            setp_1(p_1bak);
            getArm()->setp___3(p___3bak);

            //indica que el punto no está dentro del área no invasiva
            return false;
        }
    }
    //restaura la configuración original
    setQuantify_(Quantify_bak);
    getArm()->setQuantify___(Quantify___bak);
    setp_1(p_1bak);
    getArm()->setp___3(p___3bak);
    //indica que el punto está dentro del área no invasiva
    return true;
}

//--------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR LAS COLISIONES
//CON POSICIONADORES ADYACENTES:

//determina si hay colisión con un actuador
bool TActuator::ThereIsCollision(const TActuator* AA)
{
    //si la posición angular de ambos ejes de este posicionador es conocida
    if(getPAkd() != kdUnk) {
        //si la posición angular de ambos ejes del posicionador adyacente es conocida
        if(AA->getPAkd() != kdUnk) {
            //calcula la distancia a partir de la cual los brazos de los actuadores
            //están demasiado alejados para que sus brazos puedan colisionar
            double Dmax = getArm()->getL1V() + getArm()->getSPM() + AA->getArm()->getL1V() + AA->getArm()->getSPM();
            //si los brazos de los actuadores no pueden colisionar
            if(Mod(AA->getArm()->getP1() - getArm()->getP1()) >= Dmax)
                //indica que no hay colisión
                return false;

            //calcula el SPM conjunto
            double SPM = getArm()->getSPM() + AA->getArm()->getSPM();
            //si los contornos están a una distancia menor que la suma de SPMs
            if(getArm()->getContour().DistanceMin(AA->getArm()->getContour())<SPM || AA->getArm()->getContour().DistanceMin(getArm()->getContour())<SPM)
                //indica que se ha encontrado colisión
                return true;
        }
        //si la posición angular de algún eje del posicionador adyacente es desconocida
        else {
            //calcula la distancia a partir de la cual los brazos de los actuadores
            //están demasiado alejados para que sus brazos puedan colisionar
            double Dmax = getArm()->getL1V() + getArm()->getSPM() + AA->getBarrier()->getr_max() + AA->getBarrier()->getSPM();
            //si los brazos de los actuadores no pueden colisionar
            if(Mod(AA->getBarrier()->getP0() - getArm()->getP1()) >= Dmax)
                //indica que no hay colisión
                return false;

            //calcula el SPM conjunto
            double SPM = getArm()->getSPM() + AA->getBarrier()->getSPM();
            //si los contornos están a una distancia menor que la suma de SPMs
            if(getArm()->getContour().DistanceMin(AA->getBarrier()->getContour())<SPM || AA->getBarrier()->getContour().DistanceMin(getArm()->getContour())<SPM)
                //indica que se ha encontrado colisión
                return true;
        }
    }
    //si la posición angular de algún eje de este posicionador es desconocida
    else {
        //si la posición angular de ambos ejes del posicionador adyacente es conocida
        if(AA->getPAkd() != kdUnk) {
            //calcula la distancia a partir de la cual los brazos de los actuadores
            //están demasiado alejados para que sus brazos puedan colisionar
            double Dmax = getBarrier()->getr_max() + getBarrier()->getSPM() + AA->getArm()->getL1V() + AA->getArm()->getSPM();
            //si los brazos de los actuadores no pueden colisionar
            if(Mod(AA->getArm()->getP1() - getBarrier()->getP0()) >= Dmax)
                //indica que no hay colisión
                return false;

            //calcula el SPM conjunto
            double SPM = getBarrier()->getSPM() + AA->getArm()->getSPM();
            //si los contornos están a una distancia menor que la suma de SPMs
            if(getBarrier()->getContour().DistanceMin(AA->getArm()->getContour())<SPM || AA->getArm()->getContour().DistanceMin(getBarrier()->getContour())<SPM)
                //indica que se ha encontrado colisión
                return true;
        }
        //si la posición angular de algún eje del posicionador adyacente es desconocida
        else {
            //calcula la distancia a partir de la cual los brazos de los actuadores
            //están demasiado alejados para que sus brazos puedan colisionar
            double Dmax = getBarrier()->getr_max() + getBarrier()->getSPM() + AA->getBarrier()->getr_max() + AA->getBarrier()->getSPM();
            //si los brazos de los actuadores no pueden colisionar
            if(Mod(AA->getBarrier()->getP0() - getBarrier()->getP0()) >= Dmax)
                //indica que no hay colisión
                return false;

            //calcula el SPM conjunto
            double SPM = getBarrier()->getSPM() + AA->getBarrier()->getSPM();
            //si los contornos están a una distancia menor que la suma de SPMs
            if(getBarrier()->getContour().DistanceMin(AA->getBarrier()->getContour())<SPM ||
                    AA->getBarrier()->getContour().DistanceMin(getBarrier()->getContour())<SPM)
                //indica que se ha encontrado colisión
                return true;
        }
    }

    //indica que no ha encontrado colisión
    return false;
}
//determina si hay colisión con un actuador adyacente
bool TActuator::ThereIsCollisionWithAdjacent(void)
{
    //por cada posicionador adyacente
    for(int i=0; i<Adjacents.getCount(); i++) {
        //apunta el actuador de fibra indicado para facilitar su acceso
        TActuator *AA = Adjacents[i]->getActuator();
        //si hay colisión
        if(ThereIsCollision(AA))
            //indica que si hay colisión
            return true;
    }

    //indica que no no hay colisión
    return false;
}
//determina si no hay colisión con un actuador adyacente
bool TActuator::ThereIsntCollisionWithAdjacent(void)
{
    //por cada posicionador adyacente
    for(int i=0; i<Adjacents.getCount(); i++) {
        //apunta el actuador de fibra indicado para facilitar su acceso
        TActuator *AA = Adjacents[i]->getActuator();
        //si hay colisión
        if(ThereIsCollision(AA))
            //indica que si hay colisión
            return false;
    }

    //indica que no no hay colisión
    return true;
}
//Busca los posicionadores adyacentes cuyo
//brazo colisiona con el de este posicionador.
/*void TActuator::SearchCollindingAdjacent(TVector<int> &Ids)
{
        TActuator *FP;

        //inicializa la lista de identificadores en congruencia
        //con la situación de partida
        Ids.Clear();

        //busca la invasión con alguno de sus adyacentes
        for(int i=0; i<Adjacents.Count; i++) {
                //apunta el posicionador adyacente indicado para facilitar su acceso
                FP = Adjacents[i];
                //si el posicionador adyacente
                //      colisiona con el posicionador indicado
                if(Arm->Collides(FP->Arm))
                        //añade el índice al posicionador adyacente
                        Ids.Add(FP->Id);
        }
}*/
//Busca los posicionadores adyacentes cuyo
//brazo colisiona con el de este posicionador.
void TActuator::SearchCollindingAdjacent(TItemsList<TRoboticPositioner*> &Collindings)
{
    //vacia la lista de los que colisionan en congruencia con el estado inicial
    Collindings.Clear();

    TRoboticPositioner *FPA;

    //busca la invasión con alguno de sus adyacentes
    for(int i=0; i<Adjacents.getCount(); i++) {
        //apunta el posicionador adyacente indicado para facilitar su acceso
        FPA = Adjacents[i];
        //si el posicionador adyacente
        //      colisiona con el posicionador indicado
        if(ThereIsCollision(FPA->getActuator()))
            //añade el índice al posicionador adyacente
            Collindings.Add(FPA);
    }
}

//Determina si hay colisión con un actuador adyacente
//con evaluación de colisión pendiente.
bool TActuator::ThereIsCollisionWithPendingAdjacent(void)
{
    TActuator *AA;

    //por cada posicionador adyacente
    for(int i=0; i<Adjacents.getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        AA = Adjacents[i]->getActuator();
        //si alguno de los posicionadores tiene pendiente la determinación de colisión,
        //y el posicionador adyacente colisiona con el posicionador indicado
        if((Pending || AA->Pending) && ThereIsCollision(AA))
            //indica que se ha encontrado colisión
            return true;
    }

    //ADVERTENCIA: se excluirá la condición PendingCollisionDetermine
    //mintras no se demuestre que funciona bien.

    //indica que no se invade el espacio de seguridad de ningún adyacente
    return false;
}

//Busca los posicionadores adyacentes
//con evaluación de colisión pendiente
//cuyo actuador colisiona con el de este posicionador.
void TActuator::SearchCollindingPendingAdjacent(TItemsList<TRoboticPositioner*> &Collindings)
{
    //vacia la lista de los que colisionan en congruencia con el estado inicial
    Collindings.Clear();

    TRoboticPositioner *FPA;

    //busca la invasión con alguno de sus adyacentes
    for(int i=0; i<Adjacents.getCount(); i++) {
        //apunta el posicionador adyacente indicado para facilitar su acceso
        FPA = Adjacents[i];
        //si alguno de los posicionadores tiene pendiente la determinación de colisión,
        //y el posicionador adyacente colisiona con el posicionador indicado
        if((Pending || FPA->getActuator()->Pending) && ThereIsCollision(FPA->getActuator()))
            //añade el índice al posicionador adyacente
            Collindings.Add(FPA);
    }
}

/*
//MÉTODOS PARA EL CÁLCULO DE ÁNGULOS DE GIRO:

//determina los ángulos hay que rotar este posicinador, para que su brazo
//quede adyacente al segmento indicado
void TActuator::TurnSegment(TVector<double> &dts,
        TDoublePoint Pa, TDoublePoint Pb)
{
        Arm->TurnSegment(dts, Pa, Pb, P0);
}

//determina los ángulos hay que rotar este posicinador, para que su brazo
//quede adyacente al arco indicado
void TActuator::TurnArc(TVector<double> &dts,
TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R)
{
        Arm->TurnArc(dts, Pa, Pb, Pc, R, P0);
}

//determina los ángulos hay que rotar este posicinador, para que su brazo
//quede adyacente al brazo indicado
void TActuator::TurnArm(TVector<double> &dts, TArmAbstract *Arm_)
{
        Arm->TurnArm(dts, Arm_, P0);
}
  */
//---------------------------------------------------------------------------
//MÉTODOS GRÁFICOS:

//indica que parte del posicinador puede ser agarrado en el punto indicado
//      3: botón (P2, R2)
//      2: brazo (PA...)
//      1: cilindro (P0, L01)
//      0: niguna;
int TActuator::Grab(TDoublePoint P)
{
    //si está en el botón pero no en su centro
    if(IntersectionPointCircle(P, getArm()->getP3(), getArm()->getR3()))
        return 3; //agarra el botón

    //si no, si está en el brazo pero no en su eje
    if(getArm()->Covers(P) && P!=getArm()->getP1())
        return 2; //agarra el brazo

    //si no, si está en el posicinador pero no en su eje
    if(IntersectionPointCircle(P, getP0(), getL01()) && P!=getP0())
        return 1; //agarra el posicinador

    return 0;
}
/*#
//asigna un color al posicionador
void TActuator::SetAllColors(QColor Color)
{
    ColorCilinder = Color;
    ColorArm = Color;
    ColorLimitDomainP3 = Color;
    ColorLimitDomainManeuvering = Color;
}

//resalta un color si el posicionador está seleccionado
QColor TActuator::HighlightIfSelected(QColor Color)
{
    if(Selected)
        return HighlightColor(Color);
    else
        return Color;
}

//traza el posicinador con los colores indicados
//en un trazador de formas
void TActuator::Paint(TPloterShapes *PS)
{
    //el puntero PS debería apuntar a un trazdor de formas contruido
    if(PS == NULL)
        throw EImproperArgument("pointer PS should point to builts ploter shapes");

    //si el posicionador está seleccionado
    if(Selected) {
        //configura el color de la pluma
        if(double(Luminance(PS->getBackColor())) < 255.*1.5) {
            PS->setPenColor(Qt::white);
            PS->setFontColor(Qt::white);
        } else {
            PS->setPenColor(Qt::black);
            PS->setFontColor(Qt::black);
        }

        //si tiene que dibujar el cuerpo del posicionador
        if(PaintBody) {
            //dibuja el cilindro resaltado
            PS->Circunference(getP0(), getL01());
            PS->Print(getP0(), getIdText());
            //dibuja el brazo resaltado
            if(Collision)
                getArm()->Paint(PS, ColorCollision);
            else
                getArm()->Paint(PS, PS->getPenColor());
        }

        //si tiene que pintar el límite del dominio de P3
        if(PaintLimitDomainP3) {
            //dibuja el límite del dominio de P3 resaltado
            PS->Circunference(getP0(), getr_3min());
            PS->Circunference(getP0(), getr_3max());
        }

        //si tiene que pintar el límite del dominio de maniobra
        if(PaintLimitDomainManeuvering)
            //dibuja el límtie del dominio de maniobra resaltado
            PS->Circunference(getP0(), getr_max());
    }
    //si el posiciondor no está seleccionado
    else {
        //si tiene que dibujar el cuerpo del posicionador
        if(PaintBody) {
            //dibuja el cilindro con su color correspondiente
            PS->setPenColor(ColorCilinder);
            PS->setFontColor(ColorCilinder);
            PS->Circunference(getP0(), getL01());
            PS->Print(getP0(), getIdText());
            //dibuja el brazo con su color correspondiente
            if(Collision)
                getArm()->Paint(PS, ColorCollision);
            else
                getArm()->Paint(PS, ColorArm);
        }

        //si tiene que dibujar el límite del dominio de P3
        if(PaintLimitDomainP3) {
            //dibuja el límite del dominio de P3 con su color correspondiente
            PS->setPenColor(ColorLimitDomainP3);
            PS->Circunference(getP0(), getr_3min());
            PS->Circunference(getP0(), getr_3max());
        }

        //si tiene que dibujar el límite del dominio de maniobra
        if(PaintLimitDomainManeuvering) {
            //dibuja el límite del dominio de maniobra con su color correspondiente
            PS->setPenColor(ColorLimitDomainManeuvering);
            PS->Circunference(getP0(), getr_max());
        }
    }
}
//traza el posicinador con los colores indicados
//en un trazador de formas
//en el modelo simplificado (dos segmentos de P0 a P1 y de P1 a P2)
void TActuator::PaintSimplified(TPloterShapes *PS)
{
    //el puntero PS debería apuntar a un trazdor de formas contruido
    if(PS == NULL)
        throw EImproperArgument("pointer PS should point to builts ploter shapes");

    //si el posicionador está seleccionado
    if(Selected) {
        //configura el color de la pluma
        if(double(Luminance(PS->getBackColor())) < 255.*1.5)
            PS->setPenColor(Qt::white);
        else
            PS->setPenColor(Qt::black);

        //si tiene que dibujar el cuerpo del posicionador
        if(PaintBody) {
            //dibuja el cilindro resaltado
            PS->Segment(getP0(), getArm()->getP1());
            //dibuja el brazo resaltado
            if(Collision)
                getArm()
                        ->PaintSimplified(PS, ColorCollision);
            else
                getArm()->PaintSimplified(PS, PS->getPenColor());
        }

        //si tiene que pintar el límite del dominio de P3
        if(PaintLimitDomainP3) {
            //dibuja el límite del dominio de P3 resaltado
            PS->Circunference(getP0(), getr_3min());
            PS->Circunference(getP0(), getr_3max());
        }

        //si tiene que pintar el límite del dominio de maniobra
        if(PaintLimitDomainManeuvering)
            //dibuja el límite del dominio de maniobra resaltado
            PS->Circunference(getP0(), getr_max());
    }
    //si el posiciondor no está seleccionado
    else {
        //si tiene que dibujar el cuerpo del posicionador
        if(PaintBody) {
            //dibuja el cilindro con su color correspondiente
            PS->setPenColor(ColorCilinder);
            PS->Segment(getP0(), getArm()->getP1());
            //dibuja el brazo con su color correspondiente
            if(Collision)
                getArm()->PaintSimplified(PS, ColorCollision);
            else
                getArm()->PaintSimplified(PS, ColorArm);
        }

        //si tiene que dibujar el límite del dominio de P3
        if(PaintLimitDomainP3) {
            //dibuja el límite del dominio de P3 con su color correspondiente
            PS->setPenColor(ColorLimitDomainP3);
            PS->Circunference(getP0(), getr_3min());
            PS->Circunference(getP0(), getr_3max());
        }

        //si tiene que dibujar el límite del dominio de maniobra
        if(PaintLimitDomainManeuvering) {
            //dibuja el límite del dominio de maniobra con su color correspondiente
            PS->setPenColor(ColorLimitDomainManeuvering);
            PS->Circunference(getP0(), getr_max());
        }
    }
}
*/
//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
