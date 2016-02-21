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
//File: Barrier.cpp
//Content: class barrier of an EA
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Barrier.h"
#include "FiberMOSModelConstants.h"

//---------------------------------------------------------------------------

/*using namespace Lists;
using namespace Mathematics;*/

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TBarrier
//---------------------------------------------------------------------------
//PROPIEDADES:

void TBarrier::setContour_(const TContourFigureList& Contour_)
{
    //libera las figuras de la plantilla previa
    Destroy((TContourFigureList&)getContour_());
    Destroy((TContourFigureList&)getContour());

    //asigna la plantilla
    p_Contour_.Copy(Contour_);

    //determina la distancia máxima del contorno al origen de S1
    if(getContour_().getCount() < 1)
        p_r_max = 0;
    else {
        p_r_max = getContour_().distanceMax(TDoublePoint(0, 0));
    }
    //copia la plantilla
    p_Contour.Copy(getContour_());

    //calcula la imagen de la plantilla
    calculateImage();
}

void TBarrier::setthetaO1(double thetaO1)
{
    p_thetaO1 = thetaO1;

    calculateImage();
}
void TBarrier::setP0(TDoublePoint P0)
{
    p_P0 = P0;

    calculateImage();
}

void TBarrier::setSPM(double SPM)
{
    //el margen perimetral de seguridad SPM debe ser mayor que cero
    if(SPM <= 0)
        throw EImproperArgument("security perimetral margin SPM should be upper zero");

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

        //La asignación a Contour_ provocará la asimilación de Contour_ mediante:
        //      CalculateImage();

        //No olvidar que la cadena S no no debe asignarse directamente
        //a Contour_.Text, por que el método SetContour_ comprueba si Contour_
        //cumple las restricciones.

    } catch(...) {
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

        //La asignación a Contour_ provocará la asimilación de Contour_ mediante:
        //      CalculateImage();

        //No olvidar que la cadena S no no debe asignarse directamente
        //a Contour_.Text, por que el método SetContour_ compruba si Contour_
        //cumple las restricciones.

    } catch(...) {
        throw;
    }
}

AnsiString TBarrier::getr_maxText(void) const
{
    return FloatToStr(getr_max());
}

AnsiString TBarrier::getthetaO1Text(void) const
{
    return FloatToStr(getthetaO1());
}
void TBarrier::setthetaO1Text(const AnsiString &S)
{
    try {
        setthetaO1(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TBarrier::getP0Text(void) const
{
    return DPointToStr(getP0());
}
void TBarrier::setP0Text(const AnsiString &S)
{
    try {
        setP0(StrToDPoint(S));
    } catch(...) {
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
        setSPM(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TBarrier::getAllText(void) const
{
    AnsiString S;

    S += AnsiString("Contour_Address = ")+getContour_AddressText()+AnsiString("\r\n");
    S += AnsiString("r_max = ")+getr_maxText()+AnsiString("\r\n");
    S += AnsiString("thetaO1 = ")+getthetaO1Text()+AnsiString("\r\n");
    S += AnsiString("P0 = ")+getP0Text()+AnsiString("\r\n");
    S += AnsiString("ContourAddress = ")+getContourAddressText()+AnsiString("\r\n");
    S += AnsiString("SPM = ")+getSPMText();

    return S;
}
AnsiString TBarrier::getInstanceText(void) const
{
    AnsiString S;

    S += AnsiString("thetaO1 = ")+getthetaO1Text()+AnsiString("\r\n");
    S += AnsiString("P0 = ")+getP0Text()+AnsiString("\r\n");
    S += AnsiString("SPM = ")+getSPMText();

    return S;
}
void TBarrier::setInstanceText(const AnsiString& S)
{
    try {
        //contruye una variable tampón
        TBarrier aux(this);
        TBarrier *B = &aux;
        //lee la instancia y la asigna a la variable tampón
        int i = 1;
        readInstance((TBarrier*&)B, S, i);

        //avanza el índice i hasta la próxima posición que no contenga un separador
        StrTravelSeparatorsIfAny(S, i);
        //si el índice i indica a algún caracter de la cadena S
        if(i <= S.Length())
            //indica que la cadena S solo debería contener un valor de instancia
            throw EImproperArgument("string S should contain a instance value only");

        //asigna la variable tampón
        copy(B);

    } catch(...) {
        throw;
    }
}

//###########################################################################
//MÉTODOS PRIVADOS:
//###########################################################################

//---------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

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

//###########################################################################
//MÉTODOS PÚBLICOS:
//###########################################################################

//---------------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//lee una instancia de barrera en una cadena
void  TBarrier::readInstance(TBarrier* &B,
                             const AnsiString& S, int &i)
{
    //el puntero B debe apuntar a una barrera construida
    if(B == NULL)
        throw EImproperArgument("pointer B shouldpoint to built arm");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //estado de la máquina de estados de lectura
    //      0: esperando asignación a thetaO1
    //      1: esperando asignación a P0
    //      2: esperando asignación a SPM
    //      2: instancia leida con éxito
    int status = 0;

    //variables tampón
    TBarrier t_B(B);

    do {
        switch(status) {
        case 0: //esperando asignación a thetaO1
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("thetaO1", S, i);
            StrTravelLabel("=", S, i);
            double thetaO1;
            StrReadFloat(thetaO1, S, i);
            t_B.setthetaO1(thetaO1);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 1: //esperando asignación a P0
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("P0", S, i);
            StrTravelLabel("=", S, i);
            TDoublePoint P0;
            StrReadDPoint(&P0, S, i);
            t_B.setP0(P0);
        }catch(...) {
            throw;
        }
            status++;
            break;
        case 2: //esperando asignación a SPM
            try {
            StrTravelSeparators(S, i);
            StrTravelLabel("SPM", S, i);
            StrTravelLabel("=", S, i);
            double SPM;
            StrReadFloat(SPM, S, i);
            t_B.setSPM(SPM);
        }catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientras no se haya leido la instancia con éxito
    } while(status < 3);

    //asigna la viariable tampón
    B->copy(&t_B);;
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

//contruye una barrera
TBarrier::TBarrier(TDoublePoint P0, double thetaO1) :
    p_Contour_(),
    p_r_max(0),
    p_thetaO1(thetaO1), p_P0(P0),
    p_Contour()
{
    //initialize the SPM component
    p_SPM = MEGARA_Eo*getr_max() + MEGARA_Ep;

    //set the default value to the contour
    setContour_Text(MEGARA_Contour_);
}
TBarrier::TBarrier(double x, double y, double thetaO1) :
    p_Contour_(),
    p_r_max(0),
    p_thetaO1(thetaO1), p_P0(x, y),
    p_Contour()
{
    //initialize the SPM component
    p_SPM = MEGARA_Eo*getr_max() + MEGARA_Ep;

    //set the default value to the contour
    setContour_Text(MEGARA_Contour_);
}

//copia una barrera
void TBarrier::copy(const TBarrier *B)
{
    //el puntero B debería apuntar a una barrera contruida
    if(B == NULL)
        throw EImproperArgument("pointer B ahould point to built barrier");

    p_Contour_.Copy(B->getContour_());
    p_r_max = B->getr_max();
    p_thetaO1 = B->getthetaO1();
    p_P0 = B->getP0();
    p_Contour.Copy(B->getContour());
    p_SPM = B->getSPM();
}

//contruye un clon de una barrera
TBarrier::TBarrier(const TBarrier *B)
{
    //el puntero B debería apuntar a una barrera contruida
    if(B == NULL)
        throw EImproperArgument("pointer B ahould point to built barrier");

    //copy all properties
    copy(B);
}
//libera la memoria dinámica
TBarrier::~TBarrier()
{
    Destroy(p_Contour);
    Destroy(p_Contour_);
}

//--------------------------------------------------------------------------
//MÉTODOS DE MOVIMIENTO:

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

/*//-------------------------------------------------------------------
//MÉTODOS DE COLISIÓN:

//determina si la barrera está en colisión con otra barrera
bool TBarrier::collides(const TBarrier *Barrier)
{
        //el puntero Barrier debería apuntar a a barrera construido
        if(Barrier== NULL)
                throw EImproperArgument("pointer Barrier should point to built barrier");

        //determina si la distancia entre las dos barreras
        //es menor que la suma de sus SPMs
        return Contour.Collides(Barrier->Contour, SPM + Barrier->SPM);
}
//determina si la barrera está encolisión con un brazo
bool TBarrier::collides(const TArm *Arm)
{
        //el puntero Arm debería apuntar a un brazo construido
        if(Arm== NULL)
                throw EImproperArgument("pointer Arm should point to built arm");

        //determina si la distancia entre lasdos barreras
        //es menor que la suma de sus SPMs
        return Contour.Collides(Arm->Contour, SPM + Arm->SPM);
}
  */
//determina si el punto indicado está dentro de la barrera
bool TBarrier::covers(TDoublePoint P)
{
    //si la distancia entre el punto y el eje del brazo es mayor que L1V
    if(Mod(P - getP0()) > getr_max())
        return false; //indica que no hay colisión

    //determina si el punto indicado está en el interior del contorno
    return getContour().isInner(P);
}

//MÉTODOS GRÁFICOS:
/*#
//dibuja la barrera con el color indicado
//en un trazador
void TBarrier::paint(TPloterShapes *PS, QColor Color)
{
        //el puntero PS no debe ser nulo
        if(PS == NULL)
                throw EImproperArgument("pointer PS should not be null");

        //dibuja el contorno
        p_Contour.SetAllColor(Color);
        getContour().Paint(PS);
}
*/
//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

