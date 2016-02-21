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
//File: Arm.cpp
//Content: class arm of am actuator of a RP
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Arm.h"
#include "Constants.h"
#include "Strings.h"
#include "TextFile.h"

#include <math.h>
//#include <sysutils.hpp>

//---------------------------------------------------------------------------

using namespace Strings;

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//TArm
//---------------------------------------------------------------------------
//INICIALIZA LAS PROPIEDADES CONSTANTES:

//const TDoublePoint TArm::P____1 = {0, 0};

//ADVERTENCIA: el contructor se ejecutará antes
//de que se inicialicen las variables estáticas.

//---------------------------------------------------------------------------
//PROPIEDADES DE PLANTILLA
//DE LECTURA/ESCRITURA:

void TArm::setL12(double L12)
{
    //la longitud L12 debe ser mayor que cero
    if(L12 <= 0)
        throw EImproperArgument("length L12 should be upper zero");

    p_L12 = L12; //asigna el nuevo valor

    //asimila L12
    processateTemplate();
    calculateImage();
}
void TArm::setP____2(const TDoublePoint& P____2)
{
    //P____2.x no debe ser cero
    if(getP____2().x == 0)
        throw EImproperArgument("P____2.x not should be zero");

    //P____2.y debe ser cero
    if(getP____2().y != 0)
        throw EImproperArgument("P____2.y should be zero");

    p_P____2 = P____2; //asigna el nuevo valor

    //asimila P____2
    p_L12 = getP____2().Mod();
    double aux = gettheta2();
    const double COS = cos(aux);
    const double SIN = sin(aux);
    p_P2.x = getP____2().x*COS - getP____2().y*SIN + getP1().x;
    p_P2.y = getP____2().x*SIN + getP____2().y*COS + getP1().y;
    calculateImage();
}

void TArm::setL13(double L13)
{
    //el radio L13 debe ser no negativo
    if(L13 < 0)
        throw EImproperArgument("radio L13 should be nonnegative");

    p_L13 = L13; //asigna el nuevo valor

    //asimila L13
    processateTemplate();
    calculateImage();
}

void TArm::settheta____3(double theta____3)
{
    //Nótese que theta____3 debe admitir cualquier valor.

    p_theta____3 = theta____3; //asigna el nuevo valor

    //asimila theta____3
    processateTemplate();
    calculateImage();
}
void TArm::settheta__O3(double theta__O3)
{
    try {
        settheta____3(theta__O3);

    } catch(...) {
        throw;
    }
}
void TArm::setP____3(const TDoublePoint& P____3)
{
    //Nótese que P____3 debe admitir cualquier valor.

    p_P____3 = P____3; //asigna el nuevo valor

    //asimila las propiedades de plantilla
    p_L13 = getP____3().Mod();
    p_theta____3 = getP____3().Arg();
    double aux = gettheta2();
    const double COS = cos(aux);
    const double SIN = sin(aux);
    p_P3.x = getP____3().x*COS - getP____3().y*SIN + getP1().x;
    p_P3.y = getP____3().x*SIN + getP____3().y*COS + getP1().y;
    calculateImage();
}

void TArm::setR3(double R3)
{
    //debe ser mayor que cero
    if(R3 <= 0)
        throw EImproperArgument("radio R3 should be upper zero");

    p_R3 = R3; //asigna el nuevo valor

    //R3 surte efecto cuando el brazo es dibujado
    //o se determina si un punto está sobre la lente
}

//asigna una plantilla y la asimila por primera vez
void TArm::setContour____(const TContourFigureList &_Contour____)
{
    //el contorno debe estar ordenado
    if(!_Contour____.isAContourSorted())
        throw EImproperArgument("the contour should be sorted");

    //libera las figuras de la plantilla previa
    Destroy((TContourFigureList&)getContour____());

    //asigna el nuevo valor
    p_Contour____.Copy(_Contour____);

    //asimila el contorno
    processateTemplate();
    calculateImage();
}

//--------------------------------------------------------------------------
//PROPIEDADES DE ORIENTACIÓN EN RADIANES
//DE LECTURA/ESCRITURA:

void TArm::setthetaO3(double thetaO3)
{
    //asigna el nuevo valor
    p_thetaO3 = thetaO3;

    //asimila la orientación
    calculateImage();
}

void TArm::settheta___3min(double theta___3min)
{
    //el ángulo theta___3min debe estar en el dominio de F(theta___3)
    if(!getF().BelongToDomain(theta___3min))
        throw EImproperArgument("angle theta___3min should be in domain F(theta___3)");

    //el ángulo theta___3min no debe ser mayor que el ángulo theta___3max
    if(theta___3min > gettheta___3max())
        throw EImproperArgument("angle theta___3min should not be upper than angle theta___3max");

    p_theta___3min = theta___3min; //asigna el nuevo valor

    //asimila theta___3min
    p_Q.setxmin(getp___3min()); //ajusta el dominio de cuantificación
    if(gettheta___3() < gettheta___3min()) //constriñe theta___3 a su dominio
        settheta___3(gettheta___3min());

    //la asignación a theta___3 implicaría su asimilación
}
void TArm::settheta___3max(double theta___3max)
{
    //el ángulo theta___3max debe estar en el dominio de F(theta___3)
    if(!getF().BelongToDomain(theta___3max))
        throw EImproperArgument("angle theta___3max should be in domain F(theta___3)");

    //el ángulo theta___3max no debe ser menor que el ángulo theta___3min
    if(theta___3max < gettheta___3min())
        throw EImproperArgument("angle theta___3max should be less than angle theta___3min");

    p_theta___3max = theta___3max; //asigna el nuevo valor

    //asimila theta___3max:
    p_Q.setxmax(getp___3max()); //ajusta el dominio de cuantificación
    if(gettheta___3() > gettheta___3max()) //constriñe theta___3 a su dominio
        settheta___3(gettheta___3max());

    //la asignación a theta___3 implicaría su asimilación
}
void TArm::settheta___3(double theta___3)
{
    //el ángulo theta___3 debe estar en su dominio [theta___3min, theta___3max]
    if(isntInDomaintheta___3(theta___3))
        throw EImproperArgument("angle theta___3 should be in his domain [theta___3min, theta___3max]");

    //si la cuantificación está activada
    if(getQuantify___())
        //cuantifica el nuevo valor
        theta___3 = Qtheta___3(theta___3);

    //ADVERTENCIA: la cuantificación podría dar lugar a que theta___3
    //quedara ligeramente fuera de su dominio.

    //constriñe theta___3 a su dominio
    if(theta___3 < gettheta___3min())
        theta___3 = gettheta___3min();
    else if(theta___3 > gettheta___3max())
        theta___3 = gettheta___3max();

    //si el nuevo valor difiere del actual
    if(theta___3 != gettheta___3()) {
        //asigna el nuevo valor
        p_theta___3 = theta___3;
        //asimila theta___3
        calculateImage();
    }
}
double TArm::gettheta3(void) const
{
    return getthetaO3() + gettheta___3();
}
void TArm::settheta3(double theta3)
{
    try {
        settheta___3(theta3 - getthetaO2() - gettheta__O3());
        //las propiedades de orientación serán asimiladas
        //en la asignación a theta___3

    } catch(EImproperArgument &E) {
        //cambia el texto de la excepción
        E.Message = "angle theta3 should be in his domain";
        throw E; //relanza la excepción
    }
}

double TArm::getthetaO2(void) const
{
    //traduce y devuelve el valor
    return getthetaO3() - gettheta__O3();
}
void TArm::setthetaO2(double thetaO2)
{
    //traduce y asigna el nuevo valor
    setthetaO3(thetaO2 + gettheta__O3());
}

double TArm::gettheta___2min(void) const
{
    return gettheta___3min() - gettheta____3();
}
void TArm::settheta___2min(double theta___2min)
{
    //calcula theta___3min
    double theta___3min = theta___2min + gettheta____3();

    //el ángulo theta___2min debe estar en su dominio de definición
    if(!getF().BelongToDomain(theta___3min))
        throw EImproperArgument("angle theta___2min should be in his definition domain");

    //el ángulo theta___2min no debe ser mayor que el ángulo theta___2max
    if(theta___3min > gettheta___3max())
        throw EImproperArgument("angle theta___2min should not be upper than angle theta___2max");

    p_theta___3min = theta___3min; //asigna el nuevo valor

    //asimila theta___3min
    p_Q.setxmin(getp___3min());
    if(gettheta___3() < gettheta___3min())
        settheta___3(gettheta___3min());

    //la asignación a theta___3 implicaría su asimilación
}
double TArm::gettheta___2max(void) const
{
    return gettheta___3max() - gettheta____3();
}
void TArm::settheta___2max(double theta___2max)
{
    //calcula theta___3max
    double theta___3max = theta___2max + gettheta____3();

    //el ángulo theta___2max debe estar en su dominio de definición
    if(!getF().BelongToDomain(theta___3max))
        throw EImproperArgument("angle theta___2max should be in his definition domain");

    //el ángulo theta___2max mo debe ser menor que el ángulo theta___2min
    if(theta___3max < gettheta___3min())
        throw EImproperArgument("angle theta___2max should not be less than angle theta___2min");

    p_theta___3max = theta___3max; //asigna el nuevo valor

    //asimila theta___3max
    p_Q.setxmax(getp___3max());
    if(gettheta___3() > gettheta___3max())
        settheta___3(gettheta___3max());

    //la asignación a theta___3 implicaría su asimilación
}
double TArm::gettheta___2(void) const
{
    return gettheta___3() - gettheta____3();
}
void TArm::settheta___2(double theta___2)
{
    try {
        settheta___3(theta___2 + gettheta____3());
        //las propiedades de orientación serán asimiladas
        //en la asignación a theta___3

    } catch(EImproperArgument &E) {
        //cambia el texto de la excepción
        E.Message = "angle theta___2 should be in his domain";
        throw E; //relanza la excepción
    }
}
double TArm::gettheta2(void) const
{
    return gettheta3() - gettheta____3();
}
void TArm::settheta2(double theta2)
{
    try {
        settheta3(theta2 + gettheta____3());
        //las propiedades de orientación serán asimiladas
        //en la asignación a theta___3
    } catch(EImproperArgument &E) {
        //cambia el texto de la excepción
        E.Message = "angle theta2 should be in his domain";
        throw E; //relanza la excepción
    }
}

//--------------------------------------------------------------------------
//PROPIEDADES DE CUANTIFICACIÓN
//DE LECTURA/ESCRITURA:

void TArm::setF(const TFunction &F)
{
    //la función F(theta___3) debe estar definida en algún punto
    if(F.getCount() <= 0)
        throw EImproperArgument("the function F should be defined in some point");

    //la función F(theta___3) debe ser monótona creciente
    if(F.Monotony() != 1)
        throw EImproperArgument("function F(theta___3) should be monotically increasing");

    //F(theta___3) debe ser monótona creciente
    //para poder obtener la función inversa G(p___3).

    //Nótese que no se imponen las siguientes restricciones:
    //      La función F(theta___3) debe estar definida en 0.
    //      F(0) debería ser igual a cero.
    //      La función F(theta___3) debe estar definida en M_2PI.

    //asigna la función F
    p_F = F;

    //asimila la función F
    processateF();
    calculateImage();

    //En el método CalculateImage,
    //en caso de que la cuantificación esté activada,
    //la asignación de theta___3 a sí mismo,
    //hará que se desplace a la posición estable más próxima
    //la cual será asimilada.
}

void TArm::setQuantify___(bool Quantify___)
{
    //si activa la cuantificación
    if(!getQuantify___() && Quantify___)
        //establece theta___3 en la posición estable más cercana
        settheta___3(gettheta___3());

    p_Quantify___ = Quantify___; //asigna el nuevo valor
}

//--------------------------------------------------------------------------
//PROPIEDADES DE ORIENTACIÓN EN PASOS
//DE LECTURA/ESCRITURA:

double TArm::getp___3min(void) const
{
    //devuelve theta___3min traducido a pasos
    return getF().Image(gettheta___3min());
}
void TArm::setp___3min(double p___3min)
{
    //traduce a radianes
    double theta___3min = getG().Image(p___3min);

    //el ángulo p___3min debe estar en el dominio imagen de F(theta___3)
    if(!getF().BelongToDomain(theta___3min))
        throw EImproperArgument("angle p___3min should be in image domain F(theta___3)");

    //el ángulo p___3min no debe ser mayor que el ángulo p___3max
    if(theta___3min > gettheta___3max())
        throw EImproperArgument("angle p___3min should not be upper than angle p___3max");

    p_theta___3min = theta___3min; //asigna el nuevo valor

    //asimila theta___3min
    p_Q.setxmin(getp___3min());
    if(gettheta___3() < gettheta___3min())
        settheta___3(gettheta___3min());

    //la asignación a theta___3 implicaría su asimilación
}
double TArm::getp___3max(void) const
{
    return getF().Image(gettheta___3max());
}
void TArm::setp___3max(double p___3max)
{
    //traduce a radianes
    double theta___3max = getG().Image(p___3max);

    //el ángulo p___3max debe estar en el domaxio imagen de F(theta___3)
    if(!getF().BelongToDomain(theta___3max))
        throw EImproperArgument("angle p___3max should be in image domain F(theta___3)");

    //el ángulo p___3max debe ser mayor que el ángulo p___3max
    if(theta___3max <= gettheta___3max())
        throw EImproperArgument("angle p___3max should be upper than angle p___3max");

    p_theta___3max = theta___3max; //asigna el nuevo valor

    //asimila theta___3max
    p_Q.setxmax(getp___3max());
    if(gettheta___3() > gettheta___3max())
        settheta___3(gettheta___3max());

    //la asignación a theta___3 implicaría su asimilación
}
double TArm::getp___3(void) const
{
    //si la cuantificación está activada
    if(getQuantify___())
        //devuelve el valor en pasos cuantificado
        return getQ().Quantifice(getF().Image(gettheta___3()));
    else
        //devuelve el valor en pasos sin cuantificar
        return getF().Image(gettheta___3());

    //En lectura p___3 se cuantifica para evitar el error numérico
    //introducido por la función F(theta___3).
}
void TArm::setp___3(double p___3)
{
    //si la cuantificación está activada
    if(getQuantify___())
        //cuantifica el valor, lo traduce a radianes y lo asigna
        p_theta___3 = getG().Image(getQ().Quantifice(p___3));
    else
        //traduce el valor a radianes y lo asigna
        p_theta___3 = getG().Image(p___3);

    //asimila theta___3
    calculateImage();
}

//--------------------------------------------------------------------------
//PROPIEDADES DE LOCALIZACIÓN
//DE LECTURA/ESCRITURA:

void TArm::setP1(TDoublePoint P1)
{
    p_P1 = P1; //asigna el nuevo valor

    //asimila las propiedades de localización
    calculateImage();
}

void TArm::setSPM(double SPM)
{
    //el margen perimetral de seguridad SPM debería ser mayor que cero
    if(SPM <= 0)
        throw EImproperArgument("security perimetral margin SPM should be upper zero");

    p_SPM = SPM; //asigna el nuevo valor

    //El nuevo valor será utilizado en cuenta en el método:
    //      bool Collides(TArm *Arm);
}

//##################################################################
//PROPIEDADES EN FORMATO TEXTO:
//##################################################################

//PROPIEDADES DE PLANTILLA
//INDEPENDIENTES EN FORMATO TEXTO

AnsiString TArm::getP____1Text(void) const
{
    return DPointToStr(P____1);
}
AnsiString TArm::getL12Text(void) const
{
    return FloatToStr(getL12());
}
void TArm::setL12Text(const AnsiString &S)
{
    try {
        setL12(StrToFloat_(S));
    } catch(...) {
        throw ;
    }
}
AnsiString TArm::getL13Text(void) const
{
    return FloatToStr(getL13());
}
void TArm::setL13Text(const AnsiString &S)
{
    try {
        setL13(StrToFloat_(S));
    } catch(...) {
        throw ;
    }
}
AnsiString TArm::gettheta____3Text(void) const
{
    return FloatToStr(gettheta____3());
}
void TArm::settheta____3Text(const AnsiString &S)
{
    try {
        settheta____3(StrToFloat_(S));
    } catch(...) {
        throw ;
    }
}
AnsiString TArm::gettheta__O3Text(void) const
{
    return FloatToStr(gettheta__O3());
}
void TArm::settheta__O3Text(const AnsiString &S)
{
    try {
        settheta__O3(StrToFloat_(S));
    } catch(...) {
        throw ;
    }
}
AnsiString TArm::getR3Text(void) const
{
    return FloatToStr(getR3());
}
void TArm::setR3Text(const AnsiString &S)
{
    try {
        setR3(StrToFloat_(S));
    } catch(...) {
        throw ;
    }
}

AnsiString TArm::getContour____AddressText(void) const
{
    return IntToHex(reinterpret_cast<intptr_t>(&p_Contour____), 8);
}
void TArm::setContour____Text(const AnsiString &S)
{
    try {
        //construye un contorno tampón
        TContourFigureList Contour____;
        //asigna el nuevo valor al contorno tampón
        Contour____.setText(S);
        //intenta asignar el contorno tampón
        setContour____(Contour____);

        //La asignación a Contour____ provocará la asimilación de Contour____ mediante:
        //      processateTemplate();
        //      calculateImage();

        //No olvidar que la cadena S no no debe asignarse directamente
        //a Contour____.Text, por que el método SetContour____ comprueba si Contour____
        //cumple las restricciones.

    } catch(...) {
        throw;
    }
}
void TArm::setContour____ColumnText(const AnsiString &S)
{
    try {
        //construye un contorno tampón
        TContourFigureList Contour____;
        //asigna el nuevo valor al clon
        Contour____.setColumnText(S);
        //intenta asignar la nueva plantilla
        setContour____(Contour____);

        //La asignación a Contour____ provocará la asimilación de Contour____ mediante:
        //      processateTemplate();
        //      calculateImage();

        //No olvidar que la cadena S no no debe asignarse directamente
        //a Contour____.Text, por que el método SetContour____ compruba si Contour____
        //cumple las restricciones.

    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//PROPIEDADES DE PLANTILLA
//DEPENDIENTES EN FORMATO TEXTO

AnsiString TArm::getP____2Text(void) const
{
    return DPointToStr(getP____2());
}
AnsiString TArm::getP____3Text(void) const
{
    return DPointToStr(getP____3());
}
AnsiString TArm::getV____Text(void) const
{
    return DPointToStr(getV____());
}

AnsiString TArm::getL1VText(void) const
{
    return FloatToStr(getL1V());
}

//---------------------------------------------------------------------------
//PROPIEDADES DE ORIENTACIÓN EN RADIANES
//DE LECTURA/ESCRITURA EN FORMATO TEXTO

AnsiString TArm::getthetaO3Text(void) const
{
    return FloatToStr(getthetaO3());
}
void TArm::setthetaO3Text(const AnsiString &S)
{
    try {
        setthetaO3(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

AnsiString TArm::gettheta___3minText(void) const
{
    return FloatToStr(gettheta___3min());
}
void TArm::settheta___3minText(const AnsiString &S)
{
    try {
        settheta___3min(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TArm::gettheta___3maxText(void) const
{
    return FloatToStr(gettheta___3max());
}
void TArm::settheta___3maxText(const AnsiString &S)
{
    try {
        settheta___3max(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TArm::gettheta___3Text(void) const
{
    return FloatToStr(gettheta___3());
}
void TArm::settheta___3Text(const AnsiString &S)
{
    try {
        settheta___3(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TArm::gettheta3Text(void) const
{
    return FloatToStr(gettheta3());
}
void TArm::settheta3Text(const AnsiString &S)
{
    try {
        settheta3(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

AnsiString TArm::getthetaO2Text(void) const
{
    return FloatToStr(getthetaO2());
}
void TArm::setthetaO2Text(const AnsiString &S)
{
    try {
        setthetaO2(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

AnsiString TArm::gettheta___2minText(void) const
{
    return FloatToStr(gettheta___2min());
}
void TArm::settheta___2minText(const AnsiString &S)
{
    try {
        settheta___2min(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TArm::gettheta___2maxText(void) const
{
    return FloatToStr(gettheta___2max());
}
void TArm::settheta___2maxText(const AnsiString &S)
{
    try {
        settheta___2max(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TArm::gettheta___2Text(void) const
{
    return FloatToStr(gettheta___2());
}
void TArm::settheta___2Text(const AnsiString &S)
{
    try {
        settheta___2(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TArm::gettheta2Text(void) const
{
    return FloatToStr(gettheta2());
}
void TArm::settheta2Text(const AnsiString &S)
{
    try {
        settheta2(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//PROPIEDADES DE CUANTIFICACIÓN
//DE LECTURA/ESCRITURA EN FORMATO TEXTO

AnsiString TArm::getFAddressText(void) const
{
    return IntToHex(reinterpret_cast<intptr_t>(&p_F), 8);
}
void TArm::setFPointsText(const AnsiString &S)
{
    try {
        //construye una función tampón
        TFunction F;
        //asigna el nuevo valor a la función tampón
        F.setPointsText(S);
        //intenta asignar la función tampón a la función
        setF(F);

        //La asignación a F provocará la asimilación de F mediante:
        //      processateF();
        //      calculateImage();

        //No olvidar que la cadena S no no debe asignarse directamente
        //a F.PointsText, por que el método SetF comprubea si F
        //cumple las restricciones.

    } catch(...) {
        throw;
    }
}
void TArm::setFTableText(const AnsiString &S)
{
    try {
        //construye una función tampón
        TFunction F;
        //asigna el nuevo valor a la función tampón
        F.setTableText(S);
        //intenta asignar la función tampón a la función
        setF(F);

        //La asignación a F provocará la asimilación de F mediante:
        //      processateF();
        //      calculateImage();

        //No olvidar que la cadena S no no debe asignarse directamente
        //a F.PointsText, por que el método SetF comprubea si F
        //cumple las restricciones.

    } catch(...) {
        throw;
    }
}

AnsiString TArm::getQuantify___Text(void) const
{
    return BoolToStr(getQuantify___(), true);
}
void TArm::setQuantify___Text(const AnsiString &S)
{
    try {
        p_Quantify___ = StrToBool(S);
    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//PROPIEDADES DE CUANTIFICACIÓN
//DE SOLO LECTURA EN FORMATO TEXTO

AnsiString TArm::getGAddressText(void) const
{
    return IntToHex(reinterpret_cast<intptr_t>(&p_G), 8);
}

AnsiString TArm::getSB2Text(void) const
{
    return FloatToStr(getSB2());
}
AnsiString TArm::getrbsText(void) const
{
    return FloatToStr(getrbs());
}

//---------------------------------------------------------------------------
//PROPIEDADES DE ORIENTACIÓN EN PASOS
//DE LECTURA/ESCRITURA EN FORMATO TEXTO

AnsiString TArm::getp___3minText(void) const
{
    return FloatToStr(getp___3min());
}
void TArm::setp___3minText(const AnsiString &S)
{
    try {
        setp___3min(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TArm::getp___3maxText(void) const
{
    return FloatToStr(getp___3max());
}
void TArm::setp___3maxText(const AnsiString &S)
{
    try {
        setp___3max(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TArm::getp___3Text(void) const
{
    return FloatToStr(getp___3());
}
void TArm::setp___3Text(const AnsiString &S)
{
    try {
        setp___3(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//PROPIEDADES DE LOCALIZACIÓN
//DE LECTURA/ESCRITURA EN FORMATO TEXTO

AnsiString TArm::getP1Text(void) const
{
    return DPointToStr(getP1());
}
void TArm::setP1Text(const AnsiString &S)
{
    try {
        setP1(StrToDPoint(S));
    } catch(...) {
        throw;
    }
}

AnsiString TArm::getSPMText(void) const
{
    return FloatToStr(getSPM());
}
void TArm::setSPMText(const AnsiString& S)
{
    try {
        setSPM(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//PROPIEDADES DE LOCALIZACIÓN
//DE SOLO LECTURA EN FORMATO TEXTO

AnsiString TArm::getP2Text(void) const
{
    return DPointToStr(getP2());
}
AnsiString TArm::getP3Text(void) const
{
    return DPointToStr(getP3());
}
AnsiString TArm::getVText(void) const
{
    return DPointToStr(getV());
}

AnsiString TArm::getContourAddressText(void) const
{
    return IntToHex(reinterpret_cast<intptr_t>(&p_Contour), 8);
}

//---------------------------------------------------------------------------
//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO

AnsiString TArm::getTemplateText(void) const
{
    AnsiString S;

    S += "R/W:\r\n";

    S += AnsiString("    L12 = ")+getL12Text()+AnsiString("\r\n");
    S += AnsiString("    P____2: ")+getP____2Text()+AnsiString("\r\n");
    S += AnsiString("    L13 = ")+getL13Text()+AnsiString("\r\n");
    S += AnsiString("    theta____3 = ")+gettheta____3Text()+AnsiString("\r\n");
    S += AnsiString("    theta__O3 = ")+gettheta__O3Text()+AnsiString("\r\n");
    S += AnsiString("    P____3: ")+getP____3Text()+AnsiString("\r\n");
    S += AnsiString("    R3 = ")+getR3Text()+AnsiString("\r\n");

    S += AnsiString("    Contour____Address = ")+getContour____AddressText()+AnsiString("\r\n");

    S += "R:\r\n";

    S += AnsiString("    L1V: ")+getL1VText()+AnsiString("\r\n");
    S += AnsiString("    V____: ")+getV____Text();

    return S;
}

AnsiString TArm::getOrientationRadiansText(void) const
{
    AnsiString S;

    S += "R/W:\r\n";

    S += AnsiString("    thetaO3 = ")+getthetaO3Text()+AnsiString("\r\n");

    S += AnsiString("    theta___3min = ")+gettheta___3minText()+AnsiString("\r\n");
    S += AnsiString("    theta___3max = ")+gettheta___3maxText()+AnsiString("\r\n");
    S += AnsiString("    theta___3 = ")+gettheta___3Text()+AnsiString("\r\n");
    S += AnsiString("    theta3 = ")+gettheta3Text()+AnsiString("\r\n");
    S += AnsiString("    theta___3s = ")+theta___3s.getText()+AnsiString("\r\n");

    S += AnsiString("    thetaO2 = ")+getthetaO2Text()+AnsiString("\r\n");

    S += AnsiString("    theta___2min = ")+gettheta___2minText()+AnsiString("\r\n");
    S += AnsiString("    theta___2max = ")+gettheta___2maxText()+AnsiString("\r\n");
    S += AnsiString("    theta___2 = ")+gettheta___2Text()+AnsiString("\r\n");
    S += AnsiString("    theta2 = ")+gettheta2Text();

    return S;
}

AnsiString TArm::getQuantificationText(void) const
{
    AnsiString S;

    S += "R/W:\r\n";

    S += AnsiString("    FAddress = ")+getFAddressText()+AnsiString("\r\n");

    S += AnsiString("    Quantify___ = ")+getQuantify___Text()+AnsiString("\r\n");
    S += AnsiString("    Quantify___s = ")+Quantify___s.getText()+AnsiString("\r\n");

    S += AnsiString("R:\r\n");

    S += AnsiString("    GAddress: ")+getGAddressText()+AnsiString("\r\n");

    S += AnsiString("    SB2: ")+getSB2Text()+AnsiString("\r\n");
    S += AnsiString("    rbs: ")+getrbsText()+AnsiString("\r\n");
    S += AnsiString("    Q: ")+StrIndent(getQ().getAssignsText(), AnsiString("        "));

    return S;
}

AnsiString TArm::getOrientationStepsText(void) const
{
    AnsiString S;

    S += "R/W:\r\n";

    S += AnsiString("    p___3 = ")+getp___3Text()+AnsiString("\r\n");
    S += AnsiString("    p___3min = ")+getp___3minText()+AnsiString("\r\n");
    S += AnsiString("    p___3max = ")+getp___3maxText();

    return S;
}

AnsiString TArm::getLocationText(void) const
{
    AnsiString S;

    S += "R/W:\r\n";

    S += AnsiString("    P1 = ")+getP1Text()+AnsiString("\r\n");
    S += AnsiString("    SPM = ")+getSPMText()+AnsiString("\r\n");

    S += "R:\r\n";

    S += AnsiString("    P2: ")+getP2Text()+AnsiString("\r\n");
    S += AnsiString("    P3: ")+getP3Text()+AnsiString("\r\n");
    S += AnsiString("    V: ")+getVText()+AnsiString("\r\n");

    S += AnsiString("    ContourAddress: ")+getContourAddressText();

    return S;
}

AnsiString TArm::getAllText(void) const
{
    AnsiString S;

    S += "Template:\r\n";
    S += StrIndent(getTemplateText())+AnsiString("\r\n");
    S += "OrientationRadians:\r\n";
    S += StrIndent(getOrientationRadiansText())+AnsiString("\r\n");
    S += "Quantification:\r\n";
    S += StrIndent(getQuantificationText())+AnsiString("\r\n");
    S += "OrientationSteps:\r\n";
    S += StrIndent(getOrientationStepsText())+AnsiString("\r\n");
    S += "Location:\r\n";
    S += StrIndent(getLocationText());

    return S;
}

//CONJUNTOS DE LAS PROPIEDADES DE INSTANCIA EN FORMATO TEXTO:

AnsiString TArm::getInstanceText(void) const
{
    AnsiString S;

    S += AnsiString("L12 = ")+getL12Text()+AnsiString("\r\n");
    S += AnsiString("L13 = ")+getL13Text()+AnsiString("\r\n");
    S += AnsiString("theta__O3 = ")+gettheta__O3Text()+AnsiString("\r\n");
    S += AnsiString("R3 = ")+getR3Text()+AnsiString("\r\n");

    S += AnsiString("thetaO3 = ")+getthetaO3Text()+AnsiString("\r\n");
    S += AnsiString("theta___3min = ")+gettheta___3minText()+AnsiString("\r\n");
    S += AnsiString("theta___3max = ")+gettheta___3maxText()+AnsiString(AnsiString("\r\n"));
    S += AnsiString("theta___3 = ")+gettheta___3Text()+AnsiString("\r\n");

    S += AnsiString("SB2 = ")+getSB2Text();

    return S;
}
void TArm::setInstanceText(const AnsiString& S)
{
    try {
        //contruye una variable tampón
        TArm aux(this);
        TArm *A = &aux;
        //lee la instancia y la asigna a la variable tampón
        int i = 1;
        readInstance((TArm*&)A, S, i);

        //avanza el índice i hasta la próxima posición que no contenga un separador
        StrTravelSeparatorsIfAny(S, i);
        //si el índice i indica a algún caracter de la cadena S
        if(i <= S.Length())
            //indica que la cadena S solo debería contener un valor de instancia
            throw EImproperArgument("string S should contain a instance value only");

        //asigna la variable tampón
        copy(A);

    } catch(...) {
        throw;
    }
}

//##################################################################
//MÉTODOS PRIVADOS:
//##################################################################

//---------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//Apartir de:
//      {L12, L13, theta____3},
//      {Contour____}
//Determina:
//      {P____2, P____3},
//      {V____, L1V}
void TArm::processateTemplate(void)
{
    //calcula las coordenadas de los puntos significativos
    p_P____2.x = getL12();
    p_P____2.y = 0;
    p_P____3.x = getL13()*cos(gettheta____3());
    p_P____3.y = getL13()*sin(gettheta____3());

    //libera las figuras de la plantilla previa
    Destroy((TContourFigureList&)getContour());
    //contruye el contorno del brazo a partir de la plantilla
    p_Contour.Copy(getContour____());

    //ADVERTENCIA: aquí no debe usarse el operador =
    //por que es una lista de punteros y se asignarian
    //los punteros a los mismos objetos.
    //El método Copy ha sido especialmente definido para
    //que se contruyan clones de los objetos.

    //determina la distancia máxima delcontorno al origen de S4
    if(getContour____().getCount() < 1)
        p_L1V = 0;
    else {
        p_L1V = getContour____().distanceMax(P____1);
    }

    p_V____.x = getL1V();
    p_V____.y = 0;
}

//A partir de:
//      {F(theta___3)}.
//Determina:
//      {G(p___3), SB2, rbs},
//      {theta___3min, theta___3max}.
//      {Q(p___3)}
//Restricciones:
//      La función F(theta___3) debe ser monótona creciente.
void TArm::processateF(void)
{
    try {
        //determina la función G (inversa de F)
        p_G.Inverse(getF());
    } catch(...) {
        throw;
    }

    //ADVERTENCIA: esposible que F no esté definida en 0 y/o en M_2PI.

    //selecciona el intervalo a partir del cual va a calcular
    //el número de pasos de una vuelta completa
    double xmax = Min(M_2PI, getF().getXLast());
    double xmin = Max(0., getF().getXFirst());
    //calcula el número de pasos de una vuelta completa
    p_SB2 = (getF().Image(xmax) - getF().Image(xmin))/(xmax - xmin)*M_2PI;

    //calcula el escalón de cuantificación promedio en [0, M_2PI]
    p_rbs = M_2PI/getSB2();

    //si el dominio de F no contiene todo [theta___3min, theta___3max]
    //        if(gettheta___3min()<getF().getXFirst() || getF().getXLast()<gettheta___3max()) {
    //constriñe los límites del dominio de theta___3 según el dominio de F
    if(gettheta___3min() < getF().getXFirst())
        p_theta___3min = getF().getXFirst();
    if(gettheta___3max() > getF().getXLast())
        p_theta___3max = getF().getXLast();
    //        }

    //asimila [theta___3min, theta___3max]
    p_Q.Set(1, getp___3min(), getp___3max());

    //constriñe theta___3 a su dominio
    //y/o lo cuantifica en su caso
    if(gettheta___3() < gettheta___3min())
        settheta___3(gettheta___3min());
    else if(gettheta___3() > gettheta___3max())
        settheta___3(gettheta___3max());
    else
        settheta___3(gettheta___3());
}

//a partir de:
//      {P____2, P____3}
//      {Contour____}
//      {theta2, P1}
//Determina:
//      {P2, P3}
//      {Contour, V},
void TArm::calculateImage(void)
{
    //calcula el ángulo en coordenadas absolutas
    double theta = gettheta2();

    //calcula los coeficientes de rotación
    const double COS = cos(theta);
    const double SIN = sin(theta);
    //rota los puntos P2 y P3
    p_P2.x = getP____2().x*COS - getP____2().y*SIN + getP1().x;
    p_P2.y = getP____2().x*SIN + getP____2().y*COS + getP1().y;
    p_P3.x = getP____3().x*COS - getP____3().y*SIN + getP1().x;
    p_P3.y = getP____3().x*SIN + getP____3().y*COS + getP1().y;

    //determina el contorno del brazo (rotado y trasladado):
    getContour____().getRotatedAndTranslated(p_Contour, theta, getP1());

    //rota el punto V
    p_V.x = getV____().x*COS - getV____().y*SIN + getP1().x;
    p_V.y = getV____().x*SIN + getV____().y*COS + getP1().y;
}

/*//a partir de:
//      {p___3min, p___3max}.
//Determina:
//      {Q(p___3)},
void TArm::CalculateQ(void)
{
        //configura la función de cuantificación
        p_Q.Set(1, F.YMin,F.YMax);

**//*        //busca el primer centroide dentro del rango dinámico
        p_Qmin = Q[p___3min];
        if(G.Image(Qmin) < theta___3min)
                p_Qmin++;
        else if(G.Image(Qmin - 1) >= theta___3min)
                p_Qmin--;

        //ADVERTENCIA: si se hiciera Qmin = ceil(p___3min)
        //los errores numéricos en p___3min podrían provocar
        //que el centroide se saliera del dominio de theta___3.

        //busca el último centroide dentro del rango dinámico
        p_Qmax = Q[p___3max];
        if(G.Image(Qmax) > theta___3max)
                p_Qmin--;
        else if(G.Image(Qmin + 1) <= theta___3max)
                p_Qmax++;

        //ADVERTENCIA: si se hiciera Qmax = floor(p___3max)
        //los errores numéricos en p___3max podrían provocar
        //que el centroide se saliera del dominio de theta___3.
*/ /**
}     **/

//##########################################################################
//MÉTODOS PÚBLICOS:
//##########################################################################

//lee una instancia de posicionador de fibra en una cadena
void  TArm::readInstance(TArm* &A, const AnsiString& S, int &i)
{
    //el puntero A debe apuntar a un brazo construido
    if(A == NULL)
        throw EImproperArgument("pointer A shouldpoint to built arm");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //estado de la máquina de estados de lectura
    //      0: L12
    //      1: L13
    //      2: theta__O3
    //      3: R3
    //      4: thetaO3
    //      5: theta___3min
    //      6: theta___3max
    //      7: theta___3
    //      8: SB2
    //      9: instancia de brazo leida con éxito
    int status = 0;

    //variables tampón
    double L12, L13, theta__O3, R3;
    double thetaO3, theta___3min, theta___3max, theta___3;
    double SB2;

    do {
        switch(status) {
        case 0: //esperando asignación a L12
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("L12", S, i);
            StrTravelLabel("=", S, i);
            StrReadFloat(L12, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 1: //esperando asignación a L13
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("L13", S, i);
            StrTravelLabel("=", S, i);
            StrReadFloat(L13, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 2: //esperando asignación a theta__O3
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("theta__O3", S, i);
            StrTravelLabel("=", S, i);
            StrReadFloat(theta__O3, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 3: //esperando asignación a R3
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("R3", S, i);
            StrTravelLabel("=", S, i);
            StrReadFloat(R3, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 4: //esperando asignación a thetaO3
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("thetaO3", S, i);
            StrTravelLabel("=", S, i);
            StrReadFloat(thetaO3, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 5: //esperando asignación a theta___3min
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("theta___3min", S, i);
            StrTravelLabel("=", S, i);
            StrReadFloat(theta___3min, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 6: //esperando asignación a theta___3max
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("theta___3max", S, i);
            StrTravelLabel("=", S, i);
            StrReadFloat(theta___3max, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 7: //esperando asignación a theta___3
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("theta___3", S, i);
            StrTravelLabel("=", S, i);
            StrReadFloat(theta___3, S, i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 8: //esperando asignación a SB2
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("SB2", S, i);
            StrTravelLabel("=", S, i);
            StrReadFloat(SB2, S, i);
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
        A->setTemplate(L12, L13, theta__O3, R3);
        A->setOrientationRadians(thetaO3, theta___3min, theta___3max, theta___3);
        A->setQuantification(SB2);

    }catch(...) {
        throw;
    }
}

//--------------------------------------------------------------------------
//MÉTODOS DE CONTRUCCIÓN Y COPIA:

//build an arm
//with the position and orientation indicated
TArm::TArm(TDoublePoint P1, double thetaO2) :
    //build the private properties
    p_Contour____(8),
    p_F(),
    p_G(), p_Q(),
    p_Contour(8),
    //build the public properties
    P____1(0, 1),
    theta___3s(2), //las funciones de theta___3s ya están apuntadas por que es de tipo TVector
    Quantify___s(2, NULL, NULL, NULL, StrPrintBool)
{
    //INITIALIZE THE TEMPLATE:

    //inicializa los parámetros de dimensión por defecto
    p_L12 = MEGARA_L;
    p_L13 = getL12();
    p_theta____3 = 0; //recuerde que theta__O3 es igual que theta____3
    p_R3 = 0.75;

    //inicializa el contorno
    setContour____Text(MEGARA_Contour____Text);

    //INITIALIZE THE ORIENTATION PROPERTIES:

    //inicializa los parámetros y variables de orientación en radianes
    p_thetaO3 = thetaO2 + gettheta__O3();
    p_theta___3min = MEGARA_theta___3min;
    p_theta___3max = MEGARA_theta___3max;
    p_theta___3 = 0;

    //Para poder cuantificar theta___3 debe determinarse antes:
    //      {[theta___3min, theta___3max], F} --> {Q(p___3)}
    //Pero si theta___3 está fuera de [theta___3min,theta___3max]
    //será arrastrado, pudiendo ser cuantificado por Q.
    //Por eso theta___3 dbe inicializarse dentro de su dominio.

    //INITIALIZE THE QUANTIFICATION PROPERTIES:

    //añade los puntos de la función de compresión
    p_F.Add(-M_2PI, -double(MEGARA_SB2));
    p_F.Add(0., 0.);
    p_F.Add(M_2PI, double(MEGARA_SB2));
    p_F.Add(2*M_2PI, double(2*MEGARA_SB2));
    //elimina las incongruencias del formato texto
    //de la función de compansión
    p_F.setPointsText(getF().getPointsText());

    //F debe estar definida en el intervalo [-M_2PI, 2*M_2PI]
    //para permitir el ajuste olgado de [theta___3min, theta___3max].

    //inicializa las propiedades de cuantificación
    p_Quantify___ = true;

    //INITIALIZE THE LOCATION PROPERTIES:

    //inicializa las propiedades de posición
    p_P1 = P1;

    //initialize the SPM
    p_SPM = MEGARA_SPMgenPairPPDP_p;


    //------------------------------------------------------------------
    //ASSIMILATES THE VALUES OF THE TEMPLATE:

    //Apartir de:
    //      {L12, L13, theta____3},
    //      {Contour____}
    //Determina:
    //      {P____2, P____3},
    //      {V____, L1V}
    processateTemplate();

    //ASSIMILATES THE VALUES OF THE QUANTIFICATION:

    //A partir de:
    //      {F(theta___3)}.
    //Determina:
    //      {G(p___3), SB2, rbs},
    //      {theta___3min, theta___3max}.
    //      {Q(p___3)}
    //Restricciones:
    //      La función F(theta___3) debe ser monótona creciente.
    processateF();

    //ASSIMILATES THE VALUES OF ORIENTATION AND LOCATION:

    //a partir de:
    //      {P____2, P____3}
    //      {Contour____, V____}
    //      {P1, theta2}
    //Determina:
    //      {P2, P3}
    //      {Contour, V},
    calculateImage();

    //RECUERDE: en el constructor de una clase abstracta
    //no se pude invocar a los métodos virtuales de la clase,
    //de modo que los métodos de asimilación no pueden ser abstractos.
}
//copia todas las propiedades de un brazo
void TArm::copy(TArm *Arm)
{
    //el puntero Arm debería apuntar a un brazo contruido
    if(Arm == NULL)
        throw EImproperArgument("pointer Arm should point to built arm");

    //copia las propiedades de plantilla de lectura/escritura
    p_L12 = Arm->getL12();
    p_P____2 = Arm->getP____2();
    p_L13 = Arm->getL13();
    p_theta____3 = Arm->gettheta____3();
    p_P____3 = Arm->getP____3();
    p_R3 = Arm->getR3();
    p_Contour____ = Arm->getContour____();

    //copia las propiedades de plantilla de solo lectura
    p_L1V = Arm->getL1V();
    p_V____ = Arm->getV____();

    //copia las propiedades de cuanticiación delectura/escritura
    p_F = Arm->getF();
    p_Quantify___ = Arm->getQuantify___();
    Quantify___s.Clone(Arm->Quantify___s);

    //copia las propiedades de cuantificación de solo lectura
    p_G = Arm->getG();
    p_SB2 = Arm->getSB2();
    p_rbs = Arm->getrbs();
    p_Q = Arm->getQ();

    //copia las propiedades de orientación en radianes de lectura/escritura
    p_thetaO3 = Arm->getthetaO3();
    p_theta___3 = Arm->gettheta___3();
    p_theta___3min = Arm->gettheta___3min();
    p_theta___3max = Arm->gettheta___3max();

    //copia las propiedades de localización de lectura/escritura
    p_P1 = Arm->getP1();

    //copia las propiedades de localización de solo lectura
    p_P2 = Arm->getP2();
    p_P3 = Arm->getP3();
    p_V = Arm->getV();
    p_Contour = Arm->getContour();
}
//contruye un clon de un brazo
TArm::TArm(TArm *Arm)
{
    //el puntero Arm debería apuntar a un brazo contruido
    if(Arm == NULL)
        throw EImproperArgument("pointer Arm should point to built arm");

    //copia las propiedades del brazo
    copy(Arm);
}
//libera la memoria dinámica
TArm::~TArm()
{
    Destroy(p_Contour);
    Destroy(p_Contour____);
}

//--------------------------------------------------------------------------
//NÉTODOS DE TRANSFORMACIÓN ENTRE SISTEMAS DE COORDENADAS:

//transforma la coordenada angular polar de S3 a S2
//      theta__ = theta___ + theta__O3
double TArm::S3thetaToS2theta(double theta___) const
{
    return theta___ + gettheta__O3();
}
//transforma las coordenadas polares en S2
//en coordenadas rectangulares en S2
TDoublePoint TArm::S2polToS2rec(double r__, double theta__) const
{
    //la distancia radial r debe ser no negativa
    if(r__ < 0)
        throw EImproperArgument("radial coordinate r should be nonnegative");

    TDoublePoint P__;
    P__.x = r__*cos(theta__);
    P__.y = r__*sin(theta__);
    return P__;
}

//--------------------------------------------------------------------------
//MÉTODOS DE DETERMINACIÓN DE PERTENENCIA A DOMINIOS DE ORIENTACIÓN:

//determina si un ángulo en radianes está fuera
//del intervalo [theta__3min, theta__3max]
bool TArm::isntInDomaintheta___3(double theta___3)
{
    //si theta___3 está fuera del intervalo [theta___3min, theta___3max]
    if(theta___3<gettheta___3min() || gettheta___3max()<theta___3)
        return true; //indica que no está en el dominio
    else //si no
        return false; //indica que si está en el dominio
}
//determina si un ángulo en radianes está fuera
//del intervalo [theta___2min, theta___2max]
//      IsntInDomaintheta___2(theta___2) =
//      IsntInDomaintheta___3(theta___2 + theta____23)
bool TArm::isntInDomaintheta___2(double theta___2)
{
    //traduce a theta___3 y aplica el método IsntInDomaintheta___3
    return isntInDomaintheta___3(theta___2 + gettheta____3());
}

//determina si un ángulo en pasos está fuera
//del intervalo [p___3min, p___3max]
//      IsntInDomainp___3(p___3) = IsntInDomaintheta___3(G(p___3))
bool TArm::isntInDomainp___3(double p___3)
{
    //traduce a theta___3 y aplica el método IsntInDomaintheta___3
    return isntInDomaintheta___3(getG().Image(p___3));
}

//--------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR VALORES CUANTIFICADOS DE ORIENTACIONES:

//cuantifica theta___3
double TArm::Qtheta___3(double theta___3)
{
    //el ángulo de orientación theta___3 debería estar en el dominio de theta___3
    if(theta___3<gettheta___3min() || gettheta___3max()<theta___3)
        throw EImproperArgument("orientation angle theta___3 should be in theta___3 domain");

    double p___3 = getF().Image(theta___3);
    p___3 = getQ().Quantifice(p___3);
    return getG().Image(p___3);
}
//cuantifica theta___2
double TArm::Qtheta___2(double theta___2)
{
    //el ángulo de orientación theta___2 debería estar en el dominio de theta___2
    if(theta___2<gettheta___2min() || gettheta___2max()<theta___2)
        throw EImproperArgument("orientation angle theta___2 should be in theta___2 domain");

    return Qtheta___3(theta___2 + gettheta____3()) - gettheta____3();
}

//--------------------------------------------------------------------------
//MÉTODOS DE CONFIGURACIÓN:

//asigna conjuntamente las propiedades de plantilla
//si no espeficifa argumentos se asignan valores por defecto
void TArm::setTemplate(double L12, double L13, double theta____3, double R3)
{
    //la longitud L12 debe ser mayor que cero
    if(L12 <= 0)
        throw EImproperArgument("length L12 should be upper zero");

    //la coordenada radial L13 debe ser no negativa
    if(L13 <= 0)
        throw EImproperArgument("radial coordinate L13 should be nonnegative");

    //el radio R3 debe ser mayor que cero
    if(R3 <= 0)
        throw EImproperArgument("radio R3 should be upper zero");

    //asigna los nuevos valores
    p_L12 = L12;
    p_L13 = L13;
    p_theta____3 = theta____3;
    p_R3 = R3;

    //asimila los parametros de plantilla
    processateTemplate();
    calculateImage();
}
//asigna conjuntamente las propiedades de orientación en radianes
void TArm::setOrientationRadians(double thetaO3,
                                 double theta___3min, double theta___3max,
                                 double theta___3)
{
    //el ángulo theta___3min debe estar en el dominio de F(theta___3)
    if(!getF().BelongToDomain(theta___3min))
        throw EImproperArgument("angle theta___3min should be in domain F(theta___3)");

    //el ángulo theta___3max debe estar en el domaxio de F(theta___3)
    if(!getF().BelongToDomain(theta___3max))
        throw EImproperArgument("angle theta___3max should be in domain F(theta___3)");

    //el ángulo theta___3min no debe ser mayor que el ángulo theta___3max
    if(theta___3min > theta___3max)
        throw EImproperArgument("angle theta___3min should not be upper than angle theta___3max");

    //el ángulo theta___3 debe estar en su dominio [theta___3min, theta___3max]
    if(isntInDomaintheta___3(theta___3))
        throw EImproperArgument("angle theta___3 should be in his domain [theta___3min, theta___3max]");

    //asigna el origen de coordenadas
    p_thetaO3 = thetaO3;

    //asignalos nuevos límites del intervalo
    p_theta___3min = theta___3min;
    p_theta___3max = theta___3max;

    //configura la función de cuantificación
    p_Q.Set(1, getp___3min(), getp___3max());

    //ASIGNA theta___3:

    //si la cuantificación está activada
    if(getQuantify___())
        //cuantifica el nuevo valor
        theta___3 = Qtheta___3(theta___3);

    //ADVERTENCIA: la cuantificación podría dar lugar a que theta___3
    //quedara ligeramente fuera de su dominio.

    //constriñe theta___3 a su dominio
    if(theta___3 < gettheta___3min())
        p_theta___3 = gettheta___3min();
    else if(theta___3 > gettheta___3max())
        p_theta___3 = gettheta___3max();

    //si el nuevo valor difiere del actual
    if(theta___3 != gettheta___3())
        //asigna el nuevo valor
        p_theta___3 = theta___3;

    //ASIMILA thetaO3 y theta___3:

    //calcula la imagen correspondiente a la nueva posición y orientación
    calculateImage();
}
//asigna conjuntamente las propiedades de cuantificación
void TArm::setQuantification(double SB2)
{
    //DEFINE LA FUNCIÓN F A PARTIR DE SB2:

    //inicializa la función
    p_F.Clear();
    //añade los puntos de la función de compresión
    p_F.Add(-M_2PI, -double(SB2));
    p_F.Add(0., 0.);
    p_F.Add(M_2PI, double(SB2));
    p_F.Add(2*M_2PI, double(2*SB2));
    //elimina las incongruencias del formato texto
    //de la función de compansión
    p_F.setPointsText(getF().getPointsText());

    //asimila F
    processateF();
    calculateImage();
}

//---------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR DISTANCIAS:

//---------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR SI HAY COLISIÓN:

//Determina si la distancia entre el contorno de un brazo y
//el contorno de este brazo, es inferior a la suma del SPM
//de cada uno de ellos.
//      Mod(Arm->P2 - P2) >= Arm->L1V+Arm->SPM + L1V+SPM + ERR_NUM
//no invoca a Contour.DistanceMin, y devuelve directamente false.
bool TArm::collides(TArm *Arm)
{
    //el puntero Arm debe apuntar a un brazo construido
    if(Arm != NULL)
        throw EImproperArgument("pointer Arm should point to built arm");

    //si la distancia entre los ejes 2 es al menos la distancia mínima
    //de seguridad para que no haya colisión
    if(Mod(Arm->getP2() - getP2()) >= Arm->getL1V()+getL1V()+getSPM())
        return false; //indica que no hay colisión

    //determina si hay colisión entre los contornos de los brazos
    return getContour().collides(Arm->getContour(), getSPM());
}

//determina si el punto indicado está sobre el brazo
bool TArm::covers(TDoublePoint P)
{
    //si la distancia entre el punto y el eje del brazo es mayor que L1V
    if(Mod(P - getP1()) > getL1V())
        return false; //indica que no hay colisión

    //determina si el punto indicado está en el interior del contorno
    return getContour().isInner(P);
}

/*//---------------------------------------------------------------------------
//MÉTODOS PARA CALCULAR ÁNGULOS DE GIRO:

//determina los ángulos que hay que rotar este brazo en torno al punto Q
//para que quede adyacente al segmento (Pa, Pb)
//las soluciones que no estén en dts serán añadidas a dicho vector
void TArm::turnSegment(TVector<double> &dts,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Q)
{
}

//determina los ángulos que hay que rotar este brazo en torno al punto Q
//para que quede adyacente al arco (Pa, Pb, Pc, R)
//las soluciones que no estén en dts serán añadidas a dicho vector
void TArm::turnArc(TVector<double> &dts,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R,
        TDoublePoint Q)
{
}

//determina los ángulos que hay que rotar este brazo en torno al punto Q
//para que quede adyacente al brazo Arm
//las soluciones que no estén en dts serán añadidas a dicho vector
void TArm::turnArm(TVector<double> &dts, TArm *Arm_, TDoublePoint Q)
{
}
*/
//---------------------------------------------------------------------------
//MÉTODOS PARA APILAR Y RECUPERAR ORIENTACIONES:

//apila theta___3 en la pila LIFO theta___3s
//      theta___3s.Add(theta___3)
void TArm::pushtheta___3(void)
{
    theta___3s.Add(gettheta___3());
}
//restaura el último theta___3 de la pila LIFO theta___3s
//      theta___3 = theta___3s.Last
//si no hay una posición apilada lanza EImproperCall
void TArm::restoretheta___3(void)
{
    //la pila theta___3s debería contener algún valor apilado
    if(theta___3s.getCount() < 1)
        throw EImproperCall("stack theta___3s should contain some stacking value");

    //asigna el último valor apilado
    settheta___3(theta___3s.getLast());
}
//desempila el último theta___3 de la pila LIFO theta___3s
//      theta___3s.Delete(theta___3.Count - 1)
//si no hay una posición apilada lanza EImproperCall
void TArm::poptheta___3(void)
{
    //la pila theta___3s debería contener algún valor apilado
    if(theta___3s.getCount() < 1)
        throw EImproperCall("stack theta___3s should contain some stacking value");

    //borra el último valor apilado
    theta___3s.Delete(theta___3s.getCount() - 1);
}
//restaura y desempila el último theta___3 de la pila LIFO theta___3s
//      theta___3s.Delete(theta___3.Count - 1)
//si no hay una posición apilada lanza EImproperCall
void TArm::restoreAndPoptheta___3(void)
{
    //la pila theta___3s debería contener algún valor apilado
    if(theta___3s.getCount() < 1)
        throw EImproperCall("stack theta___3s should contain some stacking value");

    //asigna el último valor apilado
    settheta___3(theta___3s.getLast());
    //borra el último valor apilado
    theta___3s.Delete(theta___3s.getCount() - 1);
}

//---------------------------------------------------------------------------
//MÉTODOS PARA APILAR Y RECUPERAR ESTADOS DE CUANTIFICACIÓN:

//apila Quantify___ en la pila LIFO Quantify___s
//      Quantify___s.Add(Quantify___)
void TArm::pushQuantify___(void)
{
    Quantify___s.Add(getQuantify___());
}
//restaura el último Quantify___ de la pila LIFO Quantify___s
//      Quantify___ = Quantify___s.Last
//si no hay una posición apilada lanza EImproperCall
void TArm::restoreQuantify___(void)
{
    //la pila Quantify___s debería contener algún valor apilado
    if(Quantify___s.getCount() < 1)
        throw EImproperCall("stack Quantify___s should contain some stacking value");

    //asigna el último valor apilado
    setQuantify___(Quantify___s.getLast());
}
//desempila el último Quantify___ de la pila LIFO Quantify___s
//      Quantify___s.Delete(Quantify___.Count - 1)
//si no hay una posición apilada lanza EImproperCall
void TArm::popQuantify___(void)
{
    //la pila Quantify___s debería contener algún valor apilado
    if(Quantify___s.getCount() < 1)
        throw EImproperCall("stack Quantify___s should contain some stacking value");

    //borra el último valor apilado
    Quantify___s.Delete(Quantify___s.getCount() - 1);
}
//restaura y desempila el último Quantify___ de la pila LIFO Quantify___s
//      Quantify___s.Delete(Quantify___.Count - 1)
//si no hay una posición apilada lanza EImproperCall
void TArm::restoreAndPopQuantify___(void)
{
    //la pila Quantify___s debería contener algún valor apilado
    if(Quantify___s.getCount() < 1)
        throw EImproperCall("stack Quantify___s should contain some stacking value");

    //asigna el último valor apilado
    setQuantify___(Quantify___s.getLast());
    //borra el último valor apilado
    Quantify___s.Delete(Quantify___s.getCount() - 1);
}

//--------------------------------------------------------------------------
//MÉTODOS DE MOVIMIENTO:

//cambia la posición y orientación
//del origen de coordenadas simultaneamente
void TArm::set(TDoublePoint P1, double thetaO3)
{
    //asigna los nuevos valores
    p_P1 = P1;
    p_thetaO3 = thetaO3;

    //asimila las propiedades de posición y orientación
    calculateImage();
}

//Cambia simultaneamente:
//      la posición del brazo;
//      la orientación del del origen de coordenadas del brazo;
//      la orientación del brazo respecto de su origen de coordenadas.
void TArm::set(TDoublePoint P1, double thetaO3, double theta___3)
{
    //el ángulo theta___3 debe estar en su dominio [theta___3min, theta___3max]
    if(isntInDomaintheta___3(theta___3))
        throw EImproperArgument("angle theta___3 should be in his domain [theta___3min, theta___3max]");

    //si la cuantificación está activada
    if(getQuantify___())
        //cuantifica el nuevo valor
        theta___3 = Qtheta___3(theta___3);

    //si alguno de os nuevos valores difiere del actual
    if(P1!=getP1() || thetaO3!=getthetaO3() || theta___3!=gettheta___3()) {
        //asigna los nuevos valores
        p_P1 = P1;
        p_thetaO3 = thetaO3;
        p_theta___3 = theta___3;
        //asimila las propiedades de posición y orientación
        calculateImage();
    }
}

//genera un valor aleatorio con distribución uniforme en
//[Max(0, Qmin), Min(floor(SB2/2), Qmax)]
double TArm::randomp___3(void)
{
    return RandomUniform(Max(0., getp___3min()), Min(floor(getSB2()/2), getp___3max()));
}
//asigna a p__3 un valor aleatorio con distribución uniforme en
//[Max(0, ceil(p___3min)), Min(floor(SB2/2), floor(p___3max))]
void TArm::randomizep___3(void)
{
    setp___3(randomp___3());
}

//asigna a theta___3 el primer angulo
//estable menor o igual que el indicado
void TArm::settheta___3FirstStableLessOrEqual(double theta___3)
{
    //el ángulo theta___3 debe estar en [theta___3min, theta___3max]
    if(isntInDomaintheta___3(theta___3))
        throw EImproperArgument("angle theta___3 should be in [theta___3min, theta___3max]");

    //traduce a pasos
    double p___3 = getF().Image(theta___3);

    //asigna el primer entero menor o igual
    setp___3(floor(p___3));
}
//asigna a theta___2 el primer angulo
//estable menor o igual que el indicado
void TArm::settheta___2FirstStableLessOrEqual(double theta___2)
{
    //el ángulo theta___2 debe estar en [theta___2min, theta___2max]
    if(isntInDomaintheta___2(theta___2))
        throw EImproperArgument("angle theta___2 should be in [theta___2min, theta___2max]");

    //calcula theta___3 y lo asigna
    settheta___3FirstStableLessOrEqual(theta___2 + gettheta____3());
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
