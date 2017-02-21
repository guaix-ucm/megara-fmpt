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
//File: Barrier.cpp
//Content: barrier model of an EA or a RP-Actuator
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Barrier.h"
#include "Arm.h"
#include "FiberMOSModelConstants.h"
#include "Strings.h"
#include "TextFile.h"

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//clase TBarrier:

//PROPIEDADES DE DEFINICION:

void TBarrier::setContour_(const TContourFigureList& Contour_)
{
    //libera las figuras de la plantilla previa
    Destroy((TContourFigureList&)getContour_());
    Destroy((TContourFigureList&)getContour());

    //asigna la plantilla
    p_Contour_.Copy(Contour_);

    //asimila r_max
    calculater_max();

    //copia la plantilla
    p_Contour.Copy(getContour_());

    //calcula la imagen de la plantilla
    calculateImage();
}

void TBarrier::setP0(TDoublePoint P0)
{
    p_P0 = P0;

    calculateImage();
}
void TBarrier::setthetaO1(double thetaO1)
{
    p_thetaO1 = thetaO1;

    calculateImage();
}

void TBarrier::setSPM(double SPM)
{
    //comprueba las precondiciones
    if(SPM < 0)
        throw EImproperArgument("security perimetral margin SPM should be upper zero");

    //Nósete que SPM puede ser igual a cero.

    p_SPM = SPM; //asigna el nuevo valor
}

//PROPIEDADES EN FORMATO TEXTO:

void TBarrier::setContour_Text(const AnsiString& S)
{
    try {
        //construye un contorno tampón
        TContourFigureList Contour_;
        //asigna el nuevo valor al contorno tampón
        Contour_.setText(S);
        //intenta asignar el contorno tampón
        setContour_(Contour_);

        //La asignación de la cadena de texto a la variable tampón
        //comprobará si se cumplen las precondiciones.
        //La asignación de la variable tampón a la propiedad de la clase,
        //producirá la asimilación de todas las propiedades.

    } catch(Exception& E) {
        E.Message.Insert(1, "setting Contour_ in text format: ");
        throw;
    }
}
void TBarrier::setContour_ColumnText(const AnsiString &S)
{
    try {
        //construye un contorno tampón
        TContourFigureList Contour_;
        //asigna el nuevo valor al clon
        Contour_.setColumnText(S);
        //intenta asignar la nueva plantilla
        setContour_(Contour_);

        //La asignación de la cadena de texto a la variable tampón
        //comprobará si se cumplen las precondiciones.
        //La asignación de la variable tampón a la propiedad de la clase,
        //producirá la asimilación de todas las propiedades.

    } catch(Exception& E) {
        E.Message.Insert(1, "setting Contour_ in column text format: ");
        throw;
    }
}

AnsiString TBarrier::getr_maxText(void) const
{
    return FloatToStr(getr_max());
}

AnsiString TBarrier::getP0Text(void) const
{
    return DPointToStr(getP0());
}
void TBarrier::setP0Text(const AnsiString &S)
{
    try {
        setP0(StrToDPoint(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting P0 in text format: ");
        throw;
    }
}
AnsiString TBarrier::getthetaO1Text(void) const
{
    return FloatToStr(getthetaO1());
}
void TBarrier::setthetaO1Text(const AnsiString &S)
{
    try {
        setthetaO1(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting thetaO1 in text format: ");
        throw;
    }
}

AnsiString TBarrier::getSPMText(void) const
{
    return FloatToStr(getSPM());
}
void TBarrier::setSPMText(const AnsiString &S)
{
    try {
        setSPM(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting SPM in text format: ");
        throw;
    }
}

//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TBarrier::getAllText(void) const
{
    string str;

    str += "Contour_Address = "+getContour_AddressText().str+"\r\n";
    str += "r_max = "+getr_maxText().str+"\r\n";
    str += "P0 = "+getP0Text().str+"\r\n";
    str += "thetaO1 = "+getthetaO1Text().str+"\r\n";
    str += "ContourAddress = "+getContourAddressText().str+"\r\n";
    str += "SPM = "+getSPMText().str;

    return AnsiString(str);
}

//---------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//a partir de:
//    {Contour_}
//determina:
//    {r_max}
void TBarrier::calculater_max(void)
{
    //si no hay contorno
    if(getContour_().getCount() < 1)
        p_r_max = 0; //la distancia es cero
    else //si hay contorno
        //determina la distancia máxima del contorno al origen de S1
        p_r_max = getContour_().distanceMax(TDoublePoint(0, 0));
}

//a partir de:
//      {Contour_}
//      {thetaO1, P0}
//determina:
//      {Contour}
void TBarrier::calculateImage(void)
{
    //determina el contorno del brazo (rotado y trasladado):
    getContour_().getRotatedAndTranslated(p_Contour, getthetaO1(), getP0());
}

//---------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//contruye una barrera
TBarrier::TBarrier(TDoublePoint P0, double thetaO1) :
    p_Contour_(),
    p_P0(P0), p_thetaO1(thetaO1),
    p_Contour()
{
    //set the default value to the contour
    setContour_Text(MEGARA_Contour_Text);

    //a partir de:
    //    {Contour_}
    //determina:
    //    {r_max}
    calculater_max();

    //a partir de:
    //    {Contour_}
    //    {P0, thetaO1}
    //determina:
    //    {Contour}
    calculateImage();

    //initialize the SPM component
    p_SPM = MEGARA_Eo*getr_max() + MEGARA_Ep;
}

//copia una barrera
void TBarrier::copy(const TBarrier *B)
{
    //comprueba las precondiciones
    if(B == NULL)
        throw EImproperArgument("pointer B ahould point to built barrier");

    p_Contour_.Copy(B->getContour_());
    p_r_max = B->getr_max();
    p_P0 = B->getP0();
    p_thetaO1 = B->getthetaO1();
    p_Contour.Copy(B->getContour());
    p_SPM = B->getSPM();
}

//contruye un clon de una barrera
TBarrier::TBarrier(const TBarrier *B)
{
    //comprueba las precondiciones
    if(B == NULL)
        throw EImproperArgument("pointer B ahould point to built barrier");

    copy(B); //copia todas las propiedades
}
//libera la memoria dinámica
TBarrier::~TBarrier()
{
    Destroy(p_Contour);
    Destroy(p_Contour_);
}

//determina si tiene todos los valores por defecto
//de una barrera de un EA
bool TBarrier::dontHasAllDefaultValuesEA(void) const
{
    if(getContour_().getCount() != 16)
        return true;
    if(getContour_().getText().str != MEGARA_LIFU_Contour_Text)
        return true;
    if(getr_max() != 24.687552112224221)
        return true;
    if(getP0() != TDoublePoint(0, 0))
        return true;
    if(getthetaO1() != 0)
        return true;
    if(getContour().getCount() != getContour_().getCount())
        return true;
    if(getContour().getText().str != MEGARA_LIFU_Contour_Text)
        return true;
    if(getSPM() != 0.12154396266401415)
        return true;
    return false;
}
//determina si tiene todos los valores por defecto
//de una barrera de un actuador de un RP
bool TBarrier::dontHasAllDefaultValuesActuator(void) const
{
    if(getContour_().getCount() != 2)
        return true;
    if(getContour_().getText().str != MEGARA_Contour_Text)
        return true;
    if(getr_max() != 13.955)
        return true;
    if(getP0() != TDoublePoint(0, 0))
        return true;
    if(getthetaO1() != 0)
        return true;
    if(getContour().getCount() != getContour_().getCount())
        return true;
    if(getContour().getText().str != MEGARA_Contour_Text)
        return true;
    if(getSPM() != 0.11217804007500001)
        return true;
    return false;
}

//determina si una barrera es distinta
bool TBarrier::operator!=(const TBarrier& B) const
{
    if(getContour_() != B.getContour_())
        return true;

    if(getr_max() != B.getr_max())
        return true;

    if(getP0() != B.getP0())
        return true;

    if(getthetaO1() != B.getthetaO1())
        return true;

    if(getContour() != B.getContour())
        return true;

    if(getSPM() != B.getSPM())
        return true;

    return false;
}

//cambia la posición y orientación
//del origen de coordenadas simultaneamente
void TBarrier::set(TDoublePoint P0, double thetaO1)
{
    //asigna los nuevos valores
    p_thetaO1 = thetaO1;
    p_P0 = P0;

    //asimila las propiedades de posición y orientación
    calculateImage();
}

//-------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR SI HAY COLISIÓN:

//determina la distancia mínima con una barrera
double TBarrier::distanceMin(const TBarrier *Barrier) const
{
    //comprueba las precondiciones
    if(Barrier == NULL)
        throw EImproperArgument("pointer Barrier should point to built barrier");

    //Una manera de aligerar el proceso sería incluir el código siguiente:
    //  //calcula la suma de SPMs
    //  double SPM = Barrier->getSPM() + getSPM();
    //  //si la distancia entre los ejes es al menos la distancia mínima para que no haya colisión
    //  if(Mod(Barrier->getP0() - getP0()) > Barrier->getr_max() + getr_max() + SPM + ERR_NUM)
    //      return DBL_MAX; //indica distancia infinita
    //Pero con este código las distancias mínimas calculadas resultan más confusas.

    //calcula la distancia mínima en cada sentido
    double distanceMin1 = getContour().distanceMin(Barrier->getContour());
    double distanceMin2 = Barrier->getContour().distanceMin(getContour());

    //devuelve la distancia mínima
    return min(distanceMin1, distanceMin2);
}
//determina la distancia mínima con un brazo
double TBarrier::distanceMin(const TArm *Arm) const
{
    //comprueba las precondiciones
    if(Arm == NULL)
        throw EImproperArgument("pointer Arm should point to built Arm");

    //Una manera de aligerar el proceso sería incluir el código siguiente:
    //  //calcula la suma de SPMs
    //  double SPM = Arm->getSPM() + getSPM();
    //  //si la distancia entre los ejes es al menos la distancia mínima para que no haya colisión
    //  if(Mod(Arm->getP1() - getP0()) > Arm->getL1V() + getr_max() + SPM + ERR_NUM)
    //      return DBL_MAX; //indica distancia infinita
    //Pero con este código las distancias mínimas calculadas resultan más confusas.

    //calcula la distancia mínima en cada sentido
    double distanceMin1 = getContour().distanceMin(Arm->getContour());
    double distanceMin2 = Arm->getContour().distanceMin(getContour());

    //devuelve la distancia mínima
    return min(distanceMin1, distanceMin2);
}

//Si los contornos están tan alejado que no puede haber colisión,
//la distancia mínima devuelta será igual a DBL_MAX.

//determina si hay colisión con una barrera
bool TBarrier::collides(const TBarrier *Barrier) const
{
    //comprueba las precondiciones
    if(Barrier == NULL)
        throw EImproperArgument("pointer Barrier should point to built barrier");

    //calcula la suma de SPMs
    double SPM = Barrier->getSPM() + getSPM();

    //si la distancia entre los ejes es al menos la distancia mínima para que no haya colisión
    if(Mod(Barrier->getP0() - getP0()) > Barrier->getr_max() + getr_max() + SPM + ERR_NUM)
        return false; //indica que no hay colisión

    //determina si hay colisión entre los contornos (en ambos sentidos)
    if(getContour().collides(Barrier->getContour(), SPM))
        return true;
    if(Barrier->getContour().collides(getContour(), SPM))
        return true;

    //indica que no ha encontrado colisión en ningún sentido
    return false;
}
//determina si hay colisión con un brazo
bool TBarrier::collides(const TArm *Arm) const
{
    //comprueba las precondiciones
    if(Arm == NULL)
        throw EImproperArgument("pointer Arm should point to built arm");

    //calcula la suma de SPMs
    double SPM = Arm->getSPM() + getSPM();

    //si la distancia entre los ejes es al menos la distancia mínima para que no haya colisión
    if(Mod(Arm->getP2() - getP0()) > Arm->getL1V() + getr_max() + SPM + ERR_NUM)
        return false; //indica que no hay colisión

    //determina si hay colisión entre los contornos (en ambos sentidos)
    if(getContour().collides(Arm->getContour(), SPM))
        return true;
    if(Arm->getContour().collides(getContour(), SPM))
        return true;

    //indica que no ha encontrado colisión en ningún sentido
    return false;
}

//determina si un punto está dentro de la barrera
bool TBarrier::covers(TDoublePoint P) const
{
    //si el punto está fuera del alcance de la barrera
    if(Mod(P - getP0()) > getr_max())
        return false; //indica que el punto está fuera de la barrera

    //determina si el punto indicado está en el interior del contorno
    bool isInner = getContour().isInner(P);

    return isInner; //devuelve el resultado
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

