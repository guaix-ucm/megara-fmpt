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
///@file ComposedMotionFunction.cpp
///@brief composed motion funtion (CMF) model of a RP
///@author Isaac Morales Durán
//---------------------------------------------------------------------------

#include <stdint.h>

#include "ComposedMotionFunction.h"
#include "Scalars.h"
#include "Strings.h"
#include "StrPR.h"
#include "TextFile.h"

#include <algorithm> //std::min, std::max

//---------------------------------------------------------------------------

using namespace Mathematics;
using namespace Strings;

//espacio de nombres de funciones de movimiento
namespace MotionFunctions {

//---------------------------------------------------------------------------
//TMotionFunctionMode
//---------------------------------------------------------------------------

void  StrPrintMotionFunctionMode(AnsiString& S,
                                 TMotionFunctionMode mfm)
{
    switch(mfm) {
    case mfmSquare: S += AnsiString("Square"); break;
    case mfmRamp: S += AnsiString("Ramp"); break;
    }

    throw EImpossibleError("type TMotionFunctionMode should be in [mfmSquare, mfmRamp]");
}
void  StrReadMotionFunctionMode(TMotionFunctionMode& mfm,
                                const AnsiString &S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //La longitud de los valores legibles de la cadena son:
    //      strlen("Square"): 6
    //      strlen("Ramp"): 4

    //variable tampón
    AnsiString SubS;

    //copia el índice en una variable auxiliar
    int j = i;
    //avanza el índice auxiliar hasta el próximo caracter no separador
    StrTravelSeparatorsIfAny(S, j);

    //si desde el primera caracter hay al menos 6 caracteres (con él incluido)
    if(j+5 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 6);

        //si la subcadena coincide con "Square"
        if(SubS == "Square") {
            //traduce de texto a estructura
            mfm = mfmSquare;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 6;
            return; //no hace nada más
        }
    }

    //si desde el primera caracter hay al menos 4 caracteres (con él incluido)
    if(j+3 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 4);

        //si la subcadena coincide con "Ramp"
        if(SubS == "Ramp") {
            //traduce de texto a estructura
            mfm = mfmRamp;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 4;
            return; //no hace nada más
        }
    }

    throw EImproperArgument("there isn't a value of type TMotionFunctionMode in position i of string S");
}
AnsiString MotionFunctionModeToStr(TMotionFunctionMode mfm)
{
    switch(mfm) {
    case mfmSquare: return AnsiString("Square");
    case mfmRamp: return AnsiString("Ramp");
    }

    throw EImpossibleError("type TMotionFunctionMode should be in [mfmSquare, mfmRamp]");
}
TMotionFunctionMode StrToMotionFunctionMode(const AnsiString& S)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //La longitud de los valores legibles de la cadena son:
    //      strlen("Square"): 6
    //      strlen("Ramp"): 4

    //variable tampón
    AnsiString SubS;

    //inicializa el índice
    int j = 1;
    //avanza el índice auxiliar hasta el próximo caracter no separador
    StrTravelSeparatorsIfAny(S, j);

    //si desde el primera caracter hay al menos 6 caracteres (con él incluido)
    if(j+5 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 6);

        //si la subcadena coincide con "Square"
        if(SubS == "Square")
            //traduce de texto a estructura y devuelve el valor
            return mfmSquare;
    }

    //si desde el primera caracter hay al menos 4 caracteres (con él incluido)
    if(j+3 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 4);

        //si la subcadena coincide con "Ramp"
        if(SubS == "Ramp")
            //traduce de texto a estructura y devuelve el valor
            return mfmRamp;
    }

    throw EImproperArgument("there isn't a value of type TMotionFunctionMode in string S");
}

//##########################################################################
//TSquareSynchronismMode
//##########################################################################

void  StrPrintSquareSynchronismMode(AnsiString& S,
                                    TSquareSynchronismMode ssm)
{
    switch(ssm) {
    case ssmFree: S +=  AnsiString("Free")+AnsiString("\r\n");
    case ssmTmin: S +=  AnsiString("Tmin")+AnsiString("\r\n");
    case ssmMaxTmin: S +=  AnsiString("MaxTmin")+AnsiString("\r\n");
    case ssmDouTmin: S +=  AnsiString("DouTmin")+AnsiString("\r\n");
    }
    throw EImpossibleError("unknown value in type TSquareSynchronousMode");
}
void  StrReadSquareSynchronismMode(TSquareSynchronismMode& ssm,
                                   const AnsiString &S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //La longitud de los valores legibles de la cadena son:
    //      strlen("Free"): 4
    //      strlen("Tmin"): 4
    //      strlen("MaxTmin"): 7
    //      strlen("DoyTmin"): 7

    //variable tampón
    AnsiString SubS;

    //copia el índice en una variable auxiliar
    int j = i;
    //avanza el índice auxiliar hasta el próximo caracter no separador
    StrTravelSeparatorsIfAny(S, j);

    //si desde el primera caracter hay al menos 7 caracteres (con él incluido)
    if(j+6 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 7);

        //si la subcadena coincide con "MaxTmin"
        if(SubS == "MaxTmin") {
            //traduce de texto a estructura
            ssm = ssmMaxTmin;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 7;
            return; //no hace nada más
        }

        //si la subcadena coincide con "DouTmin"
        if(SubS == "DouTmin") {
            //traduce de texto a estructura
            ssm = ssmDouTmin;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 7;
            return; //no hace nada más
        }
    }


    //si desde el primera caracter hay al menos 4 caracteres (con él incluido)
    if(j+3 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 4);

        //si la subcadena coincide con "Free"
        if(SubS == "Free") {
            //traduce de texto a estructura
            ssm = ssmFree;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 4;
            return; //no hace nada más
        }

        //si la subcadena coincide con "Tmin"
        if(SubS == "Tmin") {
            //traduce de texto a estructura
            ssm = ssmTmin;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 4;
            return; //no hace nada más
        }
    }

    throw EImproperArgument("there isn't a value of type TMotionFunctionMode in position i of string S");
}
AnsiString SquareSynchronismModeToStr(TSquareSynchronismMode ssm)
{
    switch(ssm) {
    case ssmFree: return  AnsiString("Free");
    case ssmTmin: return  AnsiString("Tmin");
    case ssmMaxTmin: return  AnsiString("MaxTmin");
    case ssmDouTmin: return  AnsiString("DouTmin");
    }
    throw EImpossibleError("unknown value in type TSquareSynchronousMode");
}
TSquareSynchronismMode StrToSquareSynchronismMode(const AnsiString& S)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //La longitud de los valores legibles de la cadena son:
    //      strlen("Free"): 4
    //      strlen("Tmin"): 4
    //      strlen("MaxTmin"): 7
    //      strlen("DoyTmin"): 7

    //variable tampón
    AnsiString SubS;

    //inicializa el índice
    int j = 1;
    //avanza el índice auxiliar hasta el próximo caracter no separador
    StrTravelSeparatorsIfAny(S, j);

    //si desde el primera caracter hay al menos 7 caracteres (con él incluido)
    if(j+6 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 7);

        //si la subcadena coincide con "MaxTmin"
        if(SubS == "MaxTmin")
            //devuelve el valor correspondiente
            return ssmMaxTmin;

        //si la subcadena coincide con "DouTmin"
        if(SubS == "DouTmin")
            //devuelve el valor correspondiente
            return ssmDouTmin;
    }


    //si desde el primera caracter hay al menos 4 caracteres (con él incluido)
    if(j+3 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 4);

        //si la subcadena coincide con "Free"
        if(SubS == "Free")
            //devuelve el valor correspondiente
            return ssmFree;

        //si la subcadena coincide con "Tmin"
        if(SubS == "Tmin")
            //devuelve el valor correspondiente
            return ssmTmin;
    }

    throw EImproperArgument("there isn't a value of type TMotionFunctionMode in string S");
}

//##########################################################################
//TRampSynchronismMode
//##########################################################################

void  StrPrintRampSynchronismMode(AnsiString& S,
                                  TRampSynchronismMode rsm)
{
    switch(rsm) {
    case rsmFree: S +=  AnsiString("Free")+AnsiString("\r\n");
    case rsmTmin: S +=  AnsiString("Tmin")+AnsiString("\r\n");
    case rsmMaxTmin: S +=  AnsiString("MaxTmin")+AnsiString("\r\n");
    case rsmDouTmin: S +=  AnsiString("DouTmin")+AnsiString("\r\n");
    case rsmTv: S +=  AnsiString("Tv")+AnsiString("\r\n");
    case rsmMaxTv: S +=  AnsiString("MaxTv")+AnsiString("\r\n");
    case rsmDouTv: S +=  AnsiString("DouTv")+AnsiString("\r\n");
    }
    throw EImpossibleError("unknown value in type TRampSynchronousMode");
}
void  StrReadRampSynchronismMode(TRampSynchronismMode& rsm,
                                 const AnsiString &S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //La longitud de los valores legibles de la cadena son:
    //      strlen("Free"): 4
    //      strlen("Tmin"): 4
    //      strlen("MaxTmin"): 7
    //      strlen("DoyTmin"): 7
    //      strlen("Tv"): 2
    //      strlen("MaxTv"): 5
    //      strlen("DoyTv"): 5

    //variable tampón
    AnsiString SubS;

    //copia el índice en una variable auxiliar
    int j = i;
    //avanza el índice auxiliar hasta el próximo caracter no separador
    StrTravelSeparatorsIfAny(S, j);

    //si desde el primera caracter hay al menos 7 caracteres (con él incluido)
    if(j+6 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 7);

        //si la subcadena coincide con "MaxTmin"
        if(SubS == "MaxTmin") {
            //traduce de texto a estructura
            rsm = rsmMaxTmin;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 7;
            return; //no hace nada más
        }

        //si la subcadena coincide con "DouTmin"
        if(SubS == "DouTmin") {
            //traduce de texto a estructura
            rsm = rsmDouTmin;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 7;
            return; //no hace nada más
        }
    }

    //si desde el primera caracter hay al menos 5 caracteres (con él incluido)
    if(j+4 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 5);

        //si la subcadena coincide con "MaxTv"
        if(SubS == "MaxTv") {
            //traduce de texto a estructura
            rsm = rsmMaxTv;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 5;
            return; //no hace nada más
        }

        //si la subcadena coincide con "DouTv"
        if(SubS == "DouTv") {
            //traduce de texto a estructura
            rsm = rsmDouTv;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 5;
            return; //no hace nada más
        }
    }

    //si desde el primera caracter hay al menos 4 caracteres (con él incluido)
    if(j+3 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 4);

        //si la subcadena coincide con "Free"
        if(SubS == "Free") {
            //traduce de texto a estructura
            rsm = rsmFree;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 4;
            return; //no hace nada más
        }

        //si la subcadena coincide con "Tmin"
        if(SubS == "Tmin") {
            //traduce de texto a estructura
            rsm = rsmTmin;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 4;
            return; //no hace nada más
        }
    }

    //si desde el primera caracter hay al menos 2 caracteres (con él incluido)
    if(j+1 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 2);

        //si la subcadena coincide con "Tv"
        if(SubS == "Tv") {
            //traduce de texto a estructura
            rsm = rsmTv;
            //avanza el índice hasta la posúltima posición de la subcadena
            i = j + 2;
            return; //no hace nada más
        }
    }

    throw EImproperArgument("there isn't a value of type TMotionFunctionMode in position i of string S");
}
AnsiString RampSynchronismModeToStr(TRampSynchronismMode rsm)
{
    switch(rsm) {
    case rsmFree: return AnsiString("Free");
    case rsmTmin: return AnsiString("Tmin");
    case rsmMaxTmin: return AnsiString("MaxTmin");
    case rsmDouTmin: return AnsiString("DouTmin");
    case rsmTv: return AnsiString("Tv");
    case rsmMaxTv: return AnsiString("MaxTv");
    case rsmDouTv: return AnsiString("DouTv");
    }
    throw EImpossibleError("unknown value in type TRampSynchronousMode");
}
TRampSynchronismMode StrToRampSynchronismMode(const AnsiString& S)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //La longitud de los valores legibles de la cadena son:
    //      strlen("Free"): 4
    //      strlen("Tmin"): 4
    //      strlen("MaxTmin"): 7
    //      strlen("DoyTmin"): 7
    //      strlen("Tv"): 2
    //      strlen("MaxTv"): 5
    //      strlen("DoyTv"): 5

    //variable tampón
    AnsiString SubS;

    //inicializa el índice
    int j = 1;
    //avanza el índice auxiliar hasta el próximo caracter no separador
    StrTravelSeparatorsIfAny(S, j);

    //si desde el primera caracter hay al menos 7 caracteres (con él incluido)
    if(j+6 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 7);

        //si la subcadena coincide con "MaxTmin"
        if(SubS == "MaxTmin")
            //devuelve el valor correspondiente
            return rsmMaxTmin;

        //si la subcadena coincide con "DouTmin"
        if(SubS == "DouTmin")
            //devuelve el valor correspondiente
            return rsmDouTmin;
    }

    //si desde el primera caracter hay al menos 5 caracteres (con él incluido)
    if(j+4 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 5);

        //si la subcadena coincide con "MaxTv"
        if(SubS == "MaxTv")
            //devuelve el valor correspondiente
            return rsmMaxTv;

        //si la subcadena coincide con "DouTv"
        if(SubS == "DouTv")
            //devuelve el valor correspondiente
            return rsmDouTv;
    }

    //si desde el primera caracter hay al menos 4 caracteres (con él incluido)
    if(j+3 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 4);

        //si la subcadena coincide con "Free"
        if(SubS == "Free")
            //devuelve el valor correspondiente
            return rsmFree;

        //si la subcadena coincide con "Tmin"
        if(SubS == "Tmin")
            //devuelve el valor correspondiente
            return rsmTmin;
    }

    //si desde el primera caracter hay al menos 2 caracteres (con él incluido)
    if(j+1 <= S.Length()) {
        //extrae la subcadena compuesta por los próximos 19 caracteres
        SubS = S.SubString(j, 2);

        //si la subcadena coincide con "Tv"
        if(SubS == "Tv")
            //devuelve el valor correspondiente
            return rsmTv;
    }

    throw EImproperArgument("there isn't a value of type TMotionFunctionMode in string S");
}

//##########################################################################
//TComposedMotionFunction
//##########################################################################

//---------------------------------------------------------------------------
//PROPIEDADES DE CONFIGURACIÓN:

void TComposedMotionFunction::setMFM(TMotionFunctionMode MFM)
{
    p_MFM = MFM; //asigna el nuevo valor

    //apunta la funcion correspondiente al eje 1
    if(p_MF1 != NULL)
        switch(p_MFM) {
        case mfmSquare:
            p_MF1 = p_SF1;
            break;
        case mfmRamp:
            p_MF1 = p_RF1;
            break;
        }

    //apunta la funcion correspondiente al eje 2
    if(p_MF2 != NULL)
        switch(p_MFM) {
        case mfmSquare:
            p_MF2 = p_SF2;
            break;
        case mfmRamp:
            p_MF2 = p_RF2;
            break;
        }
}
void TComposedMotionFunction::setSSM(TSquareSynchronismMode SSM)
{
    p_SSM = SSM; //asigna el nuevo valor

    //sincroniza las funciones según el modo de sincronismo seleccionado
    switch(p_SSM) {
    case ssmFree: //no cambia los tiempos de desplazamiento
        break;
    case ssmTmin: //SF1->T = SF1->Tmin; SF2->T = SF2->Tmin;
        p_SF1->setT(p_SF1->getTmin());
        p_SF2->setT(p_SF2->getTmin());
        break;
    case ssmMaxTmin: { //SF2->T = SF1->T = max(SF1->Tmin, SF2->Tmin);
        double SFMaxTmin = max(p_SF1->getTmin(), p_SF2->getTmin());
        p_SF1->setT(SFMaxTmin);
        p_SF2->setT(SFMaxTmin);
    }
        break;
    case ssmDouTmin: { //2*SF1->T = SF1->Tmin*2; SF2->T = SF2->Tmin;
        double SFMaxTmin = max(p_SF1->getTmin(), p_SF2->getTmin());
        p_SF1->setT(SFMaxTmin*2);
        p_SF2->setT(SFMaxTmin);
    }
        break;
    }
}

void TComposedMotionFunction::setRSM(TRampSynchronismMode RSM)
{
    p_RSM = RSM; //asigna el nuevo valor

    //sincroniza las funciones según el modo de sincronismo seleccionado
    switch(p_RSM) {
    case rsmFree: //no cambia los tiempos de desplazamiento
        break;

    case rsmTmin: //RF1->T = RF1->Tmin; RF2->T = RF2->Tmin;
        p_RF1->setT(p_RF1->getTmin());
        p_RF2->setT(p_RF2->getTmin());
        break;
    case rsmMaxTmin: { //RF2->T = RF1->T = max(RF1->Tmin, RF2->Tmin);
        double RFMaxTmin = max(p_RF1->getTmin(), p_RF2->getTmin());
        p_RF1->setT(RFMaxTmin);
        p_RF2->setT(RFMaxTmin);
    }
        break;
    case rsmDouTmin: { //RF1->T = RF1->Tmin*2; RF2->T = RF2->Tmin;
        double RFMaxTmin = max(p_RF1->getTmin(), p_RF2->getTmin());
        p_RF1->setT(RFMaxTmin*2);
        p_RF2->setT(RFMaxTmin);
    }
        break;

    case rsmTv: //RF1->T = RF1->Tv; RF2->T = RF2->Tv;
        p_RF1->setT(p_RF1->getTv());
        p_RF2->setT(p_RF2->getTv());
        break;
    case rsmMaxTv: {//RF2->T = RF1->T = max(RF1->Tv, RF2->Tv);
        double RFMaxTv = max(p_RF1->getTv(), p_RF2->getTv());
        p_RF1->setT(RFMaxTv);
        p_RF2->setT(RFMaxTv);
    }
        break;
    case rsmDouTv: { //RF1->T = RF1->Tv*2; RF2->T = RF2->Tv;
        double RFMaxTv = max(p_RF1->getTv(), p_RF2->getTv());
        p_RF1->setT(RFMaxTv*2);
        p_RF2->setT(RFMaxTv);
    }
        break;
    }
}

/*
double TComposedMotionFunction::GetT(void)
{
        //si hay funciones para ambos ejes devuelve el periodo mayor
        if(MF1!=NULL && MF2!=NULL)
                return max(MF1->T, MF2->T);

        //si solo hay una función para el eje 1 devuelve su periodo
        if(MF1!=NULL && MF2==NULL)
                return MF1->T;

        //si solo hay una función para el eje 2 devuelve su periodo
        if(MF1==NULL && MF2!=NULL)
                return MF2->T;

        //si todavía no hay funciones para ninguno de los ejes devuelve cero
        return 0;
}
void TComposedMotionFunction::setT(double T)
{
        //debe ser mayor que cero
        if(T <= 0)
                throw EImproperArgument("T should be upper 0");

        if(p_T == 0)
                throw EImproperCall("should be an programmed gesture");

        double r = T/p_T; //calcula el factor de escala

        double T1;
        if(MF1 != NULL) {
                //calcula el nuevo periodo del eje 1
                T1 = MF1->T*r;
                //los nuevos periodos de desplazameinto deben ser al menos Tmin
                if(T1 < MF1->Tmin)
                        throw EImproperArgument("T1 should be almost MF1->Tmin");
        }

        double T2;
        if(MF2 != NULL) {
                //calcula el nuevo periodo del eje 2
                T2 = MF2->T*r;
                //los nuevos periodos de desplazameinto deben ser al menos Tmin
                if(T2 < MF2->Tmin)
                        throw EImproperArgument("T2 should be almost MF2->Tmin");
        }

        //ajusta los periodos de desplazamiento de los ejes
        if(MF1 != NULL)
                MF1->T = T1;
        if(MF2 != NULL)
                MF2->T = T2;
}*/

void TComposedMotionFunction::settsta1(double tsta1)
{
    //debe ser un valor no negativo
    if(tsta1 < 0)
        throw EImproperArgument("tsta1 should be not negative");

    p_tsta1 = tsta1; //asigna el nuevo valor
}
void TComposedMotionFunction::settsta2(double tsta2)
{
    //debe ser un valor no negativo
    if(tsta2 < 0)
        throw EImproperArgument("tsta2 should not be negative");

    p_tsta2 = tsta2; //asigna el nuevo valor
}

void TComposedMotionFunction::setId1(int Id1)
{
    //el identificador de rot 1 debe ser no negativo
    if(Id1 < 0)
        throw EImproperArgument("rot 1 identifier Id1, should be nonnegative");

    p_Id1 = Id1; //asigna el nuevo valor
}
void TComposedMotionFunction::setId2(int Id2)
{
    //el identificador de rot 2 debe ser no negativo
    if(Id2 < 0)
        throw EImproperArgument("rot 2 identifier Id2, should be nonnegative");

    p_Id2 = Id2; //asigna el nuevo valor
}

//---------------------------------------------------------------------------
//PROPIEDADES DE CONFIGURACIÓN DERIVADAS:

double TComposedMotionFunction::gettend1(void) const
{
    if(p_MF1 != NULL)
        return p_tsta1 + p_MF1->getT();
    else
        return p_tsta1;
}
double TComposedMotionFunction::gettend2(void) const
{
    if(p_MF2 != NULL)
        return p_tsta2 + p_MF2->getT();
    else
        return p_tsta2;
}

double TComposedMotionFunction::gettstamin(void) const
{
    if(p_MF1!=NULL && p_MF2!=NULL)
        return min(p_tsta1, p_tsta2);
    if(p_MF1!=NULL && p_MF2==NULL)
        return p_tsta1;
    if(p_MF1==NULL && p_MF2!=NULL)
        return p_tsta2;

    return 0;
}
double TComposedMotionFunction::gettendmax(void) const
{
    if(p_MF1!=NULL && p_MF2!=NULL)
        return max(gettend1(), gettend2());
    if(p_MF1!=NULL && p_MF2==NULL)
        return gettend1();
    if(p_MF1==NULL && p_MF2!=NULL)
        return gettend2();

    return 0;
}

double TComposedMotionFunction::getvmaxabs1(void) const
{
    switch(p_MFM) {
    case mfmSquare: return p_SF1->getvmaxabs();
    case mfmRamp: return p_RF1->getvmaxabs();
    }
    throw EImpossibleError("motion funcion mode MFM should be mfmSquare or mfmRamp");
}
double TComposedMotionFunction::getvmaxabs2(void) const
{
    switch(p_MFM) {
    case mfmSquare: return p_SF2->getvmaxabs();
    case mfmRamp: return p_RF2->getvmaxabs();
    }
    throw EImpossibleError("motion funcion mode MFM should be mfmSquare or mfmRamp");
}

//-------------------------------------------------------------------
//PROPIEDADES DE LECTURA/ESCRITURA EN FORMATO TEXTO:

void TComposedMotionFunction::setLabel(AnsiString Label)
{
    //la cadena Label debe ser una cadena imprimible
    if(!StrIsPrintable(Label))
        throw EImproperArgument("string Label should be a printable string");

    //asigna el nuevo valor
    p_Label = Label;
}

AnsiString TComposedMotionFunction::getSF1AddressText(void) const
{
    return IntToHex(intptr_t(p_SF1));
}
AnsiString TComposedMotionFunction::getRF1AddressText(void) const
{
    return IntToHex(intptr_t(p_RF1));
}
AnsiString TComposedMotionFunction::getSF2AddressText(void) const
{
    return IntToHex(intptr_t(p_SF2));
}
AnsiString TComposedMotionFunction::getRF2AddressText(void) const
{
    return IntToHex(intptr_t(p_RF2));
}

AnsiString TComposedMotionFunction::getMFMText(void) const
{
    switch(p_MFM) {
    case mfmSquare: return AnsiString("Square");
    case mfmRamp: return AnsiString("Ramp");

    };
    throw EImpossibleError("motion function mode MFM should be mfmSquare or mfmRamp");
}
void TComposedMotionFunction::setMFMText(AnsiString &S)
{
    if(S == "Square")
        setMFM(mfmSquare);
    else if(S == "Ramp")
        setMFM(mfmRamp);
    else
        throw EImproperArgument("motion function mode MFM should be \"Square\" or \"Ramp\"");
}
AnsiString TComposedMotionFunction::getSSMText(void) const
{
    return SquareSynchronismModeToStr(p_SSM);
}
void TComposedMotionFunction::setSSMText(AnsiString &S)
{
    try {
        setSSM(StrToSquareSynchronismMode(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting SSM in text format: ");
        throw;
    }
}
AnsiString TComposedMotionFunction::getRSMText(void) const
{
    return RampSynchronismModeToStr(p_RSM);
}
void TComposedMotionFunction::setRSMText(AnsiString &S)
{
    try {
        setRSM(StrToRampSynchronismMode(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting RSM in text format: ");
        throw;
    }
}

AnsiString TComposedMotionFunction::getMF1Text(void) const
{
    if(p_MF1 == NULL)
        return AnsiString("NULL");
    else if(p_MF1 == p_SF1)
        return getSF1AddressText();
    else if(p_MF1 == p_RF1)
        return getRF1AddressText();
    else
        throw EImpossibleError("pointer MF1 should point to NULL, SF1 or RF1");
}
AnsiString TComposedMotionFunction::getMF2Text(void) const
{
    if(p_MF2 == NULL)
        return AnsiString("NULL");
    else if(p_MF2 == p_SF2)
        return getSF2AddressText();
    else if(p_MF2 == p_RF2)
        return getRF2AddressText();
    else
        throw EImpossibleError("pointer MF2 should point to NULL, SF2 or RF2");
}

AnsiString TComposedMotionFunction::gettsta1Text(void) const
{
    return FloatToStr(p_tsta1);
}
void TComposedMotionFunction::settsta1Text(AnsiString &S)
{
    try {
        settsta1(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting tsta1 in text format: ");
        throw;
    }
}
AnsiString TComposedMotionFunction::gettsta2Text(void) const
{
    return FloatToStr(p_tsta2);
}
void TComposedMotionFunction::settsta2Text(AnsiString &S)
{
    try {
        settsta2(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting tsta2 in text format: ");
        throw;
    }
}

AnsiString TComposedMotionFunction::getId1Text(void) const
{
    return IntToStr(getId1());
}
void TComposedMotionFunction::setId1Text(AnsiString& S)
{
    try {
        setId1(StrToInt(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting Id1 in text format: ");
        throw;
    }
}
AnsiString TComposedMotionFunction::getId2Text(void) const
{
    return IntToStr(getId2());
}
void TComposedMotionFunction::setId2Text(AnsiString& S)
{
    try {
        setId2(StrToInt(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting Id2 in text format: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//PROPIEDADES DE SOLO LECTURA EN FORMATO TEXTO:

AnsiString TComposedMotionFunction::gettend1Text(void) const
{
    return FloatToStr(gettend1());
}
AnsiString TComposedMotionFunction::gettend2Text(void) const
{
    return FloatToStr(gettend2());
}

AnsiString TComposedMotionFunction::gettstaminText(void) const
{
    return FloatToStr(gettstamin());
}
AnsiString TComposedMotionFunction::gettendmaxText(void) const
{
    return FloatToStr(gettendmax());
}

AnsiString TComposedMotionFunction::getvmaxabs1Text(void) const
{
    return FloatToStr(getvmaxabs1());
}
AnsiString TComposedMotionFunction::getvmaxabs2Text(void) const
{
    return FloatToStr(getvmaxabs2());
}

//---------------------------------------------------------------------------
//CONJUNTO DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TComposedMotionFunction::getAllText(void) const
{
    AnsiString S;

    S = "R/W:\r\n";

    //propiedades compuestas
    S += AnsiString("    SF1Address = ")+getSF1AddressText()+AnsiString("\r\n");
    S += AnsiString("    RF1Address = ")+getRF1AddressText()+AnsiString("\r\n");
    S += AnsiString("    SF2Address = ")+getSF2AddressText()+AnsiString("\r\n");
    S += AnsiString("    RF2Address = ")+getRF2AddressText()+AnsiString("\r\n");

    //tipo de función, tipo de sincronismo y tipo de gesto
    S += AnsiString("    MFM = ")+getMFMText()+AnsiString("\r\n");
    S += AnsiString("    SSM = ")+getSSMText()+AnsiString("\r\n");
    S += AnsiString("    RSM = ")+getRSMText()+AnsiString("\r\n");

    //instantes de inicio de desplazamiento
    S += AnsiString("    tsta1 = ")+gettsta1Text()+AnsiString("\r\n");
    S += AnsiString("    tsta2 = ")+gettsta2Text()+AnsiString("\r\n");

    //identificadores de los rotores
    S += AnsiString("    Id1 = ")+getId1Text()+AnsiString("\r\n");
    S += AnsiString("    Id2 = ")+getId2Text()+AnsiString("\r\n");

    S += "R:\r\n";

    //funciones de movimiento
    S += AnsiString("    MF1: ")+getMF1Text()+AnsiString("\r\n");
    S += AnsiString("    MF2: ")+getMF2Text()+AnsiString("\r\n");

    //instantes de fin de desplazamiento
    S += AnsiString("    tend1: ")+gettend1Text()+AnsiString("\r\n");
    S += AnsiString("    tend2: ")+gettend2Text()+AnsiString("\r\n");

    //intervalo de desplazamiento
    S += AnsiString("    tstamin: ")+gettstaminText()+AnsiString("\r\n");
    S += AnsiString("    tendmax: ")+gettendmaxText()+AnsiString("\r\n");

    //velocidad máxima absoluta
    S += AnsiString("    vmaxabs1: ")+getvmaxabs1Text()+AnsiString("\r\n");
    S += AnsiString("    vmaxabs2: ")+getvmaxabs2Text();

    return S;
}

AnsiString TComposedMotionFunction::getInstanceText(void) const
{
    string str;

    //velocidad máxima absoluta
    str = commentedLine("SF1.vmaxabs = "+p_SF1->getvmaxabsText().str, "absolute maximum velocity of rot 1 when MFT = mftSquare (in step/ms)");
    str += "\r\n"+commentedLine("SF2.vmaxabs = "+p_SF2->getvmaxabsText().str, "absolute maximum velocity of rot 2 when MFT = mftSquare (in step/ms)");
    str += "\r\n"+commentedLine("RF1.vmaxabs = "+p_RF1->getvmaxabsText().str, "absolute maximum velocity of rot 1 when MFT = mftRamp (in step/ms)");
    str += "\r\n"+commentedLine("RF2.vmaxabs = "+p_RF2->getvmaxabsText().str, "absolute maximum velocity of rot 2 when MFT = mftRamp (in step/ms)");
    str += "\r\n"+commentedLine("RF1.amaxabs = "+p_RF1->getamaxabsText().str, "absolute maximum acceleration of rot 1 when MFT = mftRamp (in step/ms^2)");
    str += "\r\n"+commentedLine("RF2.amaxabs = "+p_RF2->getamaxabsText().str, "absolute maximum acceleration of rot 2 when MFT = mftRamp (in step/ms^2)");

    //tipo de función, tipo de sincronismo y tipo de gesto
    str += "\r\n"+commentedLine("MFM = "+getMFMText().str, "motion function type [Square | Ramp]");
    str += "\r\n"+commentedLine("SSM = "+getSSMText().str, "square synchronous mode [Free, Tmin, MaxTmin]");
    str += "\r\n"+commentedLine("RSM = "+getRSMText().str, "ramp synchronous mode [Free | Tmin | MaxTmin | Tv | MaxTv]");

    //identificadores de los rotores
    str += "\r\n"+commentedLine("Id1 = "+getId1Text().str, "CAN identifier of the rot 1 controller (a nonnegative integer number)");
    str += "\r\n"+commentedLine("Id2 = "+getId2Text().str, "CAN identifier of the rot 2 controller (a nonnegative integer number)");

    return AnsiString(str);
}
void TComposedMotionFunction::setInstanceText(const AnsiString& S)
{
    try {
        //contruye una variable tampón
        TComposedMotionFunction aux(this);
        TComposedMotionFunction *CMF = &aux;
        //lee la instancia y la asigna a la variable tampón
        int i = 1;
        ReadInstance(/*(TComposedMotionFunction*&)*/CMF, S, i);

        //avanza el índice i hasta la próxima posición que no contenga un separador
        StrTravelSeparatorsIfAny(S, i);
        //si el índice i indica a algún caracter de la cadena S
        if(i <= S.Length())
            //indica que la cadena S solo debería contener el valor para una instancia
            throw EImproperArgument("string S should contain the instance value only");

        //asigna la variable tampón
        Clone(CMF);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting instance to CMF in text format: ");
        throw;
    }
}

//------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//lee una instancia de función de movimiento compuesta en una cadena
void  TComposedMotionFunction::ReadInstance(TComposedMotionFunction *CMF,
                                            const AnsiString& S, int &i)
{
    //check the preconditions
    if(CMF == NULL)
        throw EImproperArgument("pointer CMF shouldpoint to built composed motion function");
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    try {
        //constuye la variable tampón
        TComposedMotionFunction t_CMF(CMF);

        StrTravelLabel("SF1.vmaxabs", S, i);
        StrTravelLabel("=", S, i);
        double aux;
        StrReadFloat(aux, S, i);
        t_CMF.p_SF1->setvmaxabs(aux);

        StrTravelSeparators(S, i);
        StrTravelLabel("SF2.vmaxabs", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(aux, S, i);
        t_CMF.p_SF2->setvmaxabs(aux);

        StrTravelSeparators(S, i);
        StrTravelLabel("RF1.vmaxabs", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(aux, S, i);
        t_CMF.p_RF1->setvmaxabs(aux);

        StrTravelSeparators(S, i);
        StrTravelLabel("RF2.vmaxabs", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(aux, S, i);
        t_CMF.p_RF2->setvmaxabs(aux);

        StrTravelSeparators(S, i);
        StrTravelLabel("RF1.amaxabs", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(aux, S, i);
        t_CMF.p_RF1->setamaxabs(aux);

        StrTravelSeparators(S, i);
        StrTravelLabel("RF2.amaxabs", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(aux, S, i);
        t_CMF.p_RF2->setamaxabs(aux);

        StrTravelSeparators(S, i);
        StrTravelLabel("MFM", S, i);
        StrTravelLabel("=", S, i);
        TMotionFunctionMode aux_mfm;
        StrReadMotionFunctionMode(aux_mfm, S, i);
        t_CMF.setMFM(aux_mfm);

        StrTravelSeparators(S, i);
        StrTravelLabel("SSM", S, i);
        StrTravelLabel("=", S, i);
        TSquareSynchronismMode aux_ssm;
        StrReadSquareSynchronismMode(aux_ssm, S, i);
        t_CMF.setSSM(aux_ssm);

        StrTravelSeparators(S, i);
        StrTravelLabel("RSM", S, i);
        StrTravelLabel("=", S, i);
        TRampSynchronismMode aux_rsm;
        StrReadRampSynchronismMode(aux_rsm, S, i);
        t_CMF.setRSM(aux_rsm);

        StrTravelSeparators(S, i);
        StrTravelLabel("Id1", S, i);
        StrTravelLabel("=", S, i);
        int aux_i;
        StrReadInt(aux_i, S, i);
        t_CMF.setId1(aux_i);

        StrTravelSeparators(S, i);
        StrTravelLabel("Id2", S, i);
        StrTravelLabel("=", S, i);
        StrReadInt(aux_i, S, i);
        t_CMF.setId2(aux_i);

        //clona la variable tampón
        CMF->Clone(t_CMF);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading instance of the CMF: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//construye una función de movimiento compuesta
TComposedMotionFunction::TComposedMotionFunction(void) :
    //inicializa los selectores a sus valores por defecto
    p_MFM(mfmSquare), p_SSM(ssmFree), p_RSM(rsmFree),
    //inicializa los instantes de inicio de desplazamiento
    p_tsta1(0), p_tsta2(0),
    //inicializa los identificadores de las microcontroladoras
    p_Id1(0), p_Id2(0)
{
    //construye funciones cuadradas
    p_SF1 = new TSquareFunction(MEGARA_VMAXABS1);
    p_SF2 = new TSquareFunction(MEGARA_VMAXABS2);

    //construye funciones rampa
    p_RF1 = new TRampFunction(MEGARA_VMAXABS1, MEGARA_AMAXABS1);
    p_RF2 = new TRampFunction(MEGARA_VMAXABS2, MEGARA_AMAXABS2);

    //set the relative velocity and acceleration of rotor 2:
    //  double r = 2*MEGARA_SB1/MEGARA_SB2;
    //  p_SF2->setvmaxabs(r*p_SF1->getvmaxabs());
    //  p_RF2->setvmaxabs(r*p_RF1->getvmaxabs());
    //  p_RF2->setamaxabs(r*p_RF1->getamaxabs());

    //nombra las funciones
    p_SF1->setLabel("SF1");
    p_SF2->setLabel("SF2");
    p_RF1->setLabel("RF1");
    p_RF2->setLabel("RF2");

    //inicializa con ningún gesto
    p_MF1 = NULL;
    p_MF2 = NULL;

    //This values will be overwritten with the loaded instance values.
}

//clona todas las propiedades de una función de movimiento compuesta
void TComposedMotionFunction::Clone(const TComposedMotionFunction *CMF)
{
    if(CMF == NULL)
        throw EImproperArgument("pointer CMF should point to built composed motion function");

    //CLONA TODAS LAS PROPIEDADES:

    //clona las funciones
    p_SF1->Clone(CMF->p_SF1);
    p_SF2->Clone(CMF->p_SF2);
    p_RF1->Clone(CMF->p_RF1);
    p_RF2->Clone(CMF->p_RF2);

    //copia los selectores
    p_MFM = CMF->p_MFM;
    p_SSM = CMF->p_SSM;
    p_RSM = CMF->p_RSM;

    //copia los instantes de inicio de desplazamiento
    p_tsta1 = CMF->p_tsta1;
    p_tsta2 = CMF->p_tsta2;

    //copia los identificadores de las microcontroladoras
    p_Id1 = CMF->p_Id1;
    p_Id2 = CMF->p_Id2;

    //apunta a las mismas funciones homónimas que el objeto a clonar
    if(CMF->p_MF1 == CMF->p_SF1)
        p_MF1 = p_SF1;
    else if(CMF->p_MF1 == CMF->p_RF1)
        p_MF1 = p_RF1;
    else //solo podrá tener NULL, pero para no arastrar posibles errores
        p_MF1 = CMF->p_MF1; //se copia el valor literalmente

    //apunta a las mismas funciones homónimas que el objeto a clonar
    if(CMF->p_MF2 == CMF->p_SF2)
        p_MF2 = p_SF2;
    else if(CMF->p_MF2 == CMF->p_RF2)
        p_MF2 = p_RF2;
    else //solo podrá tener NULL, pero para no arastrar posibles errores
        p_MF2 = CMF->p_MF2; //se copia el valor literalmente

    //copia la etiqueta de la función
    p_Label = CMF->p_Label;
}
void TComposedMotionFunction::Clone(const TComposedMotionFunction& CMF)
{
    //copia las funciones
    p_SF1->Clone(CMF.p_SF1);
    p_SF2->Clone(CMF.p_SF2);
    p_RF1->Clone(CMF.p_RF1);
    p_RF2->Clone(CMF.p_RF2);

    //copia los selectores
    p_MFM = CMF.p_MFM;
    p_SSM = CMF.p_SSM;
    p_RSM = CMF.p_RSM;

    //copia los instantes de inicio de desplazamiento
    p_tsta1 = CMF.p_tsta1;
    p_tsta2 = CMF.p_tsta2;

    //copia los identificadores de las microcontroladoras
    p_Id1 = CMF.p_Id1;
    p_Id2 = CMF.p_Id2;

    //apunta a las mismas funciones homónimas que el objeto a clonar
    if(CMF.p_MF1 == CMF.p_SF1)
        p_MF1 = p_SF1;
    else if(CMF.p_MF1 == CMF.p_RF1)
        p_MF1 = p_RF1;
    else //solo podrá tener NULL, pero para no arastrar posibles errores
        p_MF1 = CMF.p_MF1; //se copia el valor literalmente

    //apunta a las mismas funciones homónimas que el objeto a clonar
    if(CMF.p_MF2 == CMF.p_SF2)
        p_MF2 = p_SF2;
    else if(CMF.p_MF2 == CMF.p_RF2)
        p_MF2 = p_RF2;
    else //solo podrá tener NULL, pero para no arastrar posibles errores
        p_MF2 = CMF.p_MF2; //se copia el valor literalmente

    //copia la etiqueta de la función
    p_Label = CMF.p_Label;
}

//construye un clon de una función de movimiento compuesta
TComposedMotionFunction::TComposedMotionFunction(const TComposedMotionFunction *CMF)
{
    //debe apuntar un objeto construido
    if(CMF == NULL)
        throw EImproperArgument("pointer CMF should not be null");

    //construye las funciones
    p_SF1 = new TSquareFunction(1);
    p_SF2 = new TSquareFunction(1);
    p_RF1 = new TRampFunction(1, 1);
    p_RF2 = new TRampFunction(1, 1);

    //clona todas las propiedades
    Clone(CMF);
}

//destruye una función de movimiento compuesta
TComposedMotionFunction::~TComposedMotionFunction()
{
    //destruye las funciones rampa
    if(p_RF2 != NULL)
        delete p_RF2;
    if(p_RF1 != NULL)
        delete p_RF1;

    //destruye las funciones cuadradas
    if(p_SF2 != NULL)
        delete p_SF2;
    if(p_SF1 != NULL)
        delete p_SF1;
}

//-------------------------------------------------------------------
//MÉTODOS QUE PROGRAMAN GESTOS:

//apunta (MF1=(SF1 o RF1), MF2=NULL)
//programa la función de movimeinto 1
//desde la posición p_ini hasta la posición p_fin
void TComposedMotionFunction::ProgramMF1(double p_ini, double p_fin)
{
    //asigna la posición final al eje 1
    p_SF1->SetInterval(p_ini, p_fin);
    p_RF1->SetInterval(p_ini, p_fin);

    //ADVERTENCIA: la asignación de las posiciones inicial y final
    //a las funciones de movimiento debe ser conjunta
    //para no alterar el valor del periodo de desplazamiento
    //ya que este no puede reajustarse cuando SM == smFree

    //indica a la función correspondiente al tipo de función seleccionado
    switch(p_MFM) {
    case mfmSquare: p_MF1 = p_SF1; break;
    case mfmRamp: p_MF1 = p_RF1; break;
    }
    p_MF2 = NULL;

    //sincroniza las funciones según el modo
    setSSM(p_SSM);
    setRSM(p_RSM);
}
//apunta (MF1=NULL, MF2=(SF2 o RF2))
//programa la función de movimeinto 2
//desde la posición p__ini hasta la posición p__fin
void TComposedMotionFunction::ProgramMF2(double p__ini, double p__fin)
{
    //asigna la posición final al eje 2
    p_SF2->SetInterval(p__ini, p__fin);
    p_RF2->SetInterval(p__ini, p__fin);

    //ADVERTENCIA: la asignación de las posiciones inicial y final
    //a las funciones de movimiento debe ser conjunta
    //para no alterar el valor del periodo de desplazamiento
    //ya que este no puede reajustarse cuando SM == smFree

    //indica a la función correspondiente al tipo de función seleccionado
    p_MF1 = NULL;
    switch(p_MFM) {
    case mfmSquare: p_MF2 = p_SF2; break;
    case mfmRamp: p_MF2 = p_RF2; break;
    }

    //sincroniza las funciones según el modo
    setSSM(p_SSM);
    setRSM(p_RSM);
}
//apunta (MF1=SF1, MF2=SF2) o (MF1=RF1, MF2=RF2)
//programa ambas funciones de movimeinto
//desde la posición p_ini hasta la posición p_fin y
//desde la posición p__ini hasta la posición p__fin
void TComposedMotionFunction::ProgramBoth(double p_ini, double p__ini,
                                          double p_fin, double p__fin)
{
    //asigna la posición final al eje 1
    p_SF1->SetInterval(p_ini, p_fin);
    p_RF1->SetInterval(p_ini, p_fin);

    //asigna la posición final al eje 2
    p_SF2->SetInterval(p__ini, p__fin);
    p_RF2->SetInterval(p__ini, p__fin);

    //ADVERTENCIA: la asignación de las posiciones inicial y final
    //a las funciones de movimiento debe ser conjunta
    //para no alterar el valor del periodo de desplazamiento
    //ya que este no puede reajustarse cuando SM == smFree

    //indica a la función correspondiente al tipo de función seleccionado
    switch(p_MFM) {
    case mfmSquare:
        p_MF1 = p_SF1;
        p_MF2 = p_SF2;
        break;
    case mfmRamp:
        p_MF1 = p_RF1;
        p_MF2 = p_RF2;
        break;
    }

    //sincroniza las funciones según el modo
    setSSM(p_SSM);
    setRSM(p_RSM);
}

//initialize the CMF
void TComposedMotionFunction::ClearProgram(void)
{
    //reset the motion funtions
    p_SF1->reset();
    p_SF2->reset();
    p_RF1->reset();
    p_RF2->reset();

    //indicates that there isn't an instruction set
    p_MF1 = NULL;
    p_MF2 = NULL;
}

/*//store the setting of the CMF
void TComposedMotionFunction::PushSetting(void)
{
    TBD
}
//recovery the stored setting of the CMF
void TComposedMotionFunction::PullSetting(void)
{
    TBD
}
*/
//sincroniza la llegada del gesto con el instante de tiempo indicado
void TComposedMotionFunction::SinchronizeArrival(double t)
{
    if(p_MF1 != NULL)
        p_tsta1 = t - p_MF1->getT();
    if(p_MF2 != NULL)
        p_tsta2 = t - p_MF2->getT();
}
//invierte la función en el dominio del tiempo
//y desplaza su inicio al mismo instante de inicio
void TComposedMotionFunction::InvertTime(void)
{
    double aux = gettendmax();

    //se recuerda que T = max(tend1, tend2)

    if(p_MF1 != NULL) {
        p_MF1->InvertTime();
        p_tsta1 = aux - gettend1();
    }
    if(p_MF2 != NULL) {
        p_MF2->InvertTime();
        p_tsta2 = aux - gettend2();
    }
}

//evalúa la MF1 desplazada al instante de tiempo tsta1
//      MF1p(t) = MF1->p(t - tsta1)
double TComposedMotionFunction::MF1p(double t)
{
    //el puntero MF1 no debe apuntar a nulo
    if(p_MF1 == NULL)
        throw EImproperArgument("pointer MF1 should not be null");

    //evalúa la función desplazada en el tiempo
    return p_MF1->p(t - p_tsta1);
}
//evalúa la MF2 desplazada al instante de tiempo tsta2
//      MF2p(t) = MF2->p(t - tsta2)
double TComposedMotionFunction::MF2p(double t)
{
    //el puntero MF2 no debe apuntar a nulo
    if(p_MF2 == NULL)
        throw EImproperArgument("pointer MF2 should not be null");

    //evalúa la función desplazada en el tiempo
    return p_MF2->p(t - p_tsta2);
}

//---------------------------------------------------------------------------

} //namespace MotionFunctions

//---------------------------------------------------------------------------

