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
//File: MotionFunction.cpp
//Content: motion function
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "MotionFunction.h"
#include "Exceptions.h"
#include "Strings.h"
#include "StrPR.h"

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres de funciones de movimiento
namespace MotionFunctions {

//--------------------------------------------------------------------------
//TMotionFunction
//--------------------------------------------------------------------------
//PROPIEDADES EN FORMATO TEXTO:

void TMotionFunction::setLabel(const AnsiString& Label)
{
        //la cadena Label debe ser una cadena imprimible
        if(!StrIsPrintable(Label))
                throw EImproperArgument("string Label should be a printable string");

        p_Label = Label;
}

AnsiString TMotionFunction::getvmaxabsText(void) const
{
        return FloatToStr(p_vmaxabs);
}
void TMotionFunction::setvmaxabsText(const AnsiString& S)
{
        try {
                setvmaxabs(StrToFloat(S));
        } catch(...) {
                throw;
        }
}
AnsiString TMotionFunction::getpstaText(void) const
{
        return FloatToStr(p_psta);
}
void TMotionFunction::setpstaText(const AnsiString& S)
{
        try {
                setpsta(StrToFloat(S));
        } catch(...) {
                throw;
        }
}
AnsiString TMotionFunction::getpfinText(void) const
{
        return FloatToStr(p_pfin);
}
void TMotionFunction::setpfinText(const AnsiString& S)
{
        try {
                setpfin(StrToFloat(S));
        } catch(...) {
                throw;
        }
}

AnsiString TMotionFunction::getvmaxText(void) const
{
        return FloatToStr(p_vmax);
}
AnsiString TMotionFunction::getTminText(void) const
{
        return FloatToStr(p_Tmin);
}

AnsiString TMotionFunction::getDText(void) const
{
        return FloatToStr(p_D);
}
void TMotionFunction::setDText(const AnsiString& S)
{
        try {
                setDText(StrToFloat(S));
        } catch(...) {
                throw;
        }
}
AnsiString TMotionFunction::getTText(void) const
{
        return FloatToStr(p_T);
}
void TMotionFunction::setTText(const AnsiString& S)
{
        try {
                setTText(StrToFloat(S));
        } catch(...) {
                throw;
        }
}

AnsiString TMotionFunction::getDmaxText(void) const
{
        return FloatToStr(p_Dmax);
}
AnsiString TMotionFunction::getvcminText(void) const
{
        return FloatToStr(p_vcmin);
}
AnsiString TMotionFunction::getvcmaxText(void) const
{
        return FloatToStr(p_vcmax);
}

AnsiString TMotionFunction::getvcText(void) const
{
        return FloatToStr(p_vc);
}
void TMotionFunction::setvcText(const AnsiString& S)
{
        try {
                setvc(StrToFloat(S));
        } catch(...) {
                throw;
        }
}

//--------------------------------------------------------------------------
//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TMotionFunction::getText(void)
{
        AnsiString S;

        //imprime las propiedades heredadas
        S += AnsiString("vmaxabs = ")+getvmaxabsText()+AnsiString("\r\n");
        S += AnsiString("psta = ")+getpstaText()+AnsiString("\r\n");
        S += AnsiString("pfin = ")+getpfinText()+AnsiString("\r\n");
        S += AnsiString("vmax: ")+getvmaxText()+AnsiString("\r\n");
        S += AnsiString("Tmin: ")+getTminText()+AnsiString("\r\n");
        S += AnsiString("D: ")+getDText()+AnsiString("\r\n");
        S += AnsiString("T: ")+getTText()+AnsiString("\r\n");
        S += AnsiString("Dmax: ")+getDmaxText()+AnsiString("\r\n");
        S += AnsiString("vcmin: ")+getvcminText()+AnsiString("\r\n");
        S += AnsiString("vcmax: ")+getvcmaxText()+AnsiString("\r\n");
        S += AnsiString("vc = ")+getvcText()+AnsiString("\r\n");

        return S;
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

//inicializa las propiedades a sus valores por defecto
TMotionFunction::TMotionFunction(void) :
        p_vmaxabs(0), p_psta(0), p_pfin(0), p_vmax(0),
        p_Tmin(0), p_D(0),
        p_Dmax(0), p_vcmin(0), p_vcmax(0), p_vc(0)
{
        p_T = 0;
}

//contruye un clon de una función de movimiento
TMotionFunction::TMotionFunction(const TMotionFunction *MF)
{
        //el puntero MF debe apuntar a una función de movimeinto contruida
        if(MF == NULL)
                throw EImproperArgument("pointer MF should point to built motion function");

        //clona todas las propiedades
        p_vmaxabs = MF->p_vmaxabs;
        p_psta = MF->p_psta;
        p_pfin = MF->p_pfin;
        p_vmax = MF->p_vmax;
        p_Tmin = MF->p_Tmin;
        p_D = MF->p_D;
        p_T = MF->p_T;
        p_Dmax = MF->p_Dmax;
        p_vcmin = MF->p_vcmin;
        p_vcmax = MF->p_vcmax;
        p_vc = MF->p_vc;
        p_Label = MF->p_Label;
}

//destruye una función de movimiento
TMotionFunction::~TMotionFunction()
{
}

//---------------------------------------------------------------------------

} //namespace MotionFunctions

//---------------------------------------------------------------------------

