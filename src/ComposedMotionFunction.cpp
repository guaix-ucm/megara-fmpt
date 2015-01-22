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
//Archivo: ComposedMotionFunction.cpp
//Contenido: funcion de movimiento compuesta
//Última actualización: 07/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------
//

#include <stdint.h>

#include "ComposedMotionFunction.h"
#include "Scalars.h"
#include "Strings.h"
#include "StrPR.h"

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

         throw EImproperArgument("there isn't a value of type TMotionFunctionMode in position i of string text S");
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

         throw EImproperArgument("there isn't a value of type TMotionFunctionMode in string text S");
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
        throw EImpossibleError("ununknown value in type TSquareSynchronousMode");
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

         throw EImproperArgument("there isn't a value of type TMotionFunctionMode in position i of string text S");
}
AnsiString SquareSynchronismModeToStr(TSquareSynchronismMode ssm)
{
        switch(ssm) {
                case ssmFree: return  AnsiString("Free");
                case ssmTmin: return  AnsiString("Tmin");
                case ssmMaxTmin: return  AnsiString("MaxTmin");
                case ssmDouTmin: return  AnsiString("DouTmin");
        }
        throw EImpossibleError("ununknown value in type TSquareSynchronousMode");
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

         throw EImproperArgument("there isn't a value of type TMotionFunctionMode in string text S");
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
        throw EImpossibleError("ununknown value in type TRampSynchronousMode");
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

         throw EImproperArgument("there isn't a value of type TMotionFunctionMode in position i of string text S");
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
        throw EImpossibleError("ununknown value in type TRampSynchronousMode");
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

         throw EImproperArgument("there isn't a value of type TMotionFunctionMode in string text S");
}

//##########################################################################
//TComposedMotionFunction
//##########################################################################

///---------------------------------------------------------------------------
//PROPIEDADES DE CONFIGURACIÓN:

void TComposedMotionFunction::setMFM(TMotionFunctionMode _MFM)
{
        __MFM = _MFM; //asigna el nuevo valor

        //apunta la funcion correspondiente al eje 1
        if(__MF1 != NULL)
                switch(__MFM) {
                        case mfmSquare:
                                __MF1 = __SF1;
                                break;
                        case mfmRamp:
                                __MF1 = __RF1;
                                break;
                }

        //apunta la funcion correspondiente al eje 2
        if(__MF2 != NULL)
                switch(__MFM) {
                        case mfmSquare:
                                __MF2 = __SF2;
                                break;
                        case mfmRamp:
                                __MF2 = __RF2;
                                break;
                }
}
void TComposedMotionFunction::setSSM(TSquareSynchronismMode _SSM)
{
        __SSM = _SSM; //asigna el nuevo valor

        //sincroniza las funciones según el modo de sincronismo seleccionado
        switch(__SSM) {
                case ssmFree: //no cambia los tiempos de desplazamiento
                        break;
                case ssmTmin: //SF1->T = SF1->Tmin; SF2->T = SF2->Tmin;
                        __SF1->setT(__SF1->getTmin());
                        __SF2->setT(__SF2->getTmin());
                        break;
                case ssmMaxTmin: { //SF2->T = SF1->T = Max(SF1->Tmin, SF2->Tmin);
                        double SFMaxTmin = Max(__SF1->getTmin(), __SF2->getTmin());
                        __SF1->setT(SFMaxTmin);
                        __SF2->setT(SFMaxTmin);
                        }
                        break;
                case ssmDouTmin: { //2*SF1->T = SF1->Tmin*2; SF2->T = SF2->Tmin;
                        double SFMaxTmin = Max(__SF1->getTmin(), __SF2->getTmin());
                        __SF1->setT(SFMaxTmin*2);
                        __SF2->setT(SFMaxTmin);
                        }
                        break;
        }
}

void TComposedMotionFunction::setRSM(TRampSynchronismMode _RSM)
{
        __RSM = _RSM; //asigna el nuevo valor

        //sincroniza las funciones según el modo de sincronismo seleccionado
        switch(__RSM) {
                case rsmFree: //no cambia los tiempos de desplazamiento
                        break;

                case rsmTmin: //RF1->T = RF1->Tmin; RF2->T = RF2->Tmin;
                        __RF1->setT(__RF1->getTmin());
                        __RF2->setT(__RF2->getTmin());
                        break;
                case rsmMaxTmin: { //RF2->T = RF1->T = Max(RF1->Tmin, RF2->Tmin);
                        double RFMaxTmin = Max(__RF1->getTmin(), __RF2->getTmin());
                        __RF1->setT(RFMaxTmin);
                        __RF2->setT(RFMaxTmin);
                        }
                        break;
                case rsmDouTmin: { //RF1->T = RF1->Tmin*2; RF2->T = RF2->Tmin;
                        double RFMaxTmin = Max(__RF1->getTmin(), __RF2->getTmin());
                        __RF1->setT(RFMaxTmin*2);
                        __RF2->setT(RFMaxTmin);
                        }
                        break;

                case rsmTv: //RF1->T = RF1->Tv; RF2->T = RF2->Tv;
                        __RF1->setT(__RF1->getTv());
                        __RF2->setT(__RF2->getTv());
                        break;
                case rsmMaxTv: {//RF2->T = RF1->T = Max(RF1->Tv, RF2->Tv);
                        double RFMaxTv = Max(__RF1->getTv(), __RF2->getTv());
                        __RF1->setT(RFMaxTv);
                        __RF2->setT(RFMaxTv);
                        }
                        break;
                case rsmDouTv: { //RF1->T = RF1->Tv*2; RF2->T = RF2->Tv;
                        double RFMaxTv = Max(__RF1->getTv(), __RF2->getTv());
                        __RF1->setT(RFMaxTv*2);
                        __RF2->setT(RFMaxTv);
                        }
                        break;
        }
}

/*
double TComposedMotionFunction::GetT(void)
{
        //si hay funciones para ambos ejes devuelve el periodo mayor
        if(MF1!=NULL && MF2!=NULL)
                return Max(MF1->T, MF2->T);

        //si solo hay una función para el eje 1 devuelve su periodo
        if(MF1!=NULL && MF2==NULL)
                return MF1->T;

        //si solo hay una función para el eje 2 devuelve su periodo
        if(MF1==NULL && MF2!=NULL)
                return MF2->T;

        //si todavía no hay funciones para ninguno de los ejes devuelve cero
        return 0;
}
void TComposedMotionFunction::setT(double _T)
{
        //debe ser mayor que cero
        if(_T <= 0)
                throw EImproperArgument("T should be upper 0");

        if(T == 0)
                throw EImproperCall("should be an programmed gesture");

        double r = _T/T; //calcula el factor de escala

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
}        */

void TComposedMotionFunction::settsta1(double _tsta1)
{
        //debe ser un valor no negativo
        if(_tsta1 < 0)
                throw EImproperArgument("tsta1 should be not negative");

        __tsta1 = _tsta1; //asigna el nuevo valor
}
void TComposedMotionFunction::settsta2(double _tsta2)
{
        //debe ser un valor no negativo
        if(_tsta2 < 0)
                throw EImproperArgument("tsta2 should not be negative");

        __tsta2 = _tsta2; //asigna el nuevo valor
}

void TComposedMotionFunction::setId1(int _Id1)
{
        //el identificador de rot 1 debe ser no negativo
        if(_Id1 < 0)
                throw EImproperArgument("rot 1 identifier Id1, should be nonnegative");

        __Id1 = _Id1; //asigna el nuevo valor
}
void TComposedMotionFunction::setId2(int _Id2)
{
        //el identificador de rot 2 debe ser no negativo
        if(_Id2 < 0)
                throw EImproperArgument("rot 2 identifier Id2, should be nonnegative");

        __Id2 = _Id2; //asigna el nuevo valor
}

//---------------------------------------------------------------------------
//PROPIEDADES DE CONFIGURACIÓN DERIVADAS:

double TComposedMotionFunction::gettend1(void) const
{
        if(__MF1 != NULL)
                return __tsta1 + __MF1->getT();
        else
                return __tsta1;
}
double TComposedMotionFunction::gettend2(void) const
{
        if(__MF2 != NULL)
                return __tsta2 + __MF2->getT();
        else
                return __tsta2;
}

double TComposedMotionFunction::gettstamin(void) const
{
        if(__MF1!=NULL && __MF2!=NULL)
                return Min(__tsta1, __tsta2);
        if(__MF1!=NULL && __MF2==NULL)
                return __tsta1;
        if(__MF1==NULL && __MF2!=NULL)
                return __tsta2;

        return 0;
}
double TComposedMotionFunction::gettendmax(void) const
{
        if(__MF1!=NULL && __MF2!=NULL)
                return Max(gettend1(), gettend2());
        if(__MF1!=NULL && __MF2==NULL)
                return gettend1();
        if(__MF1==NULL && __MF2!=NULL)
                return gettend2();

        return 0;
}

double TComposedMotionFunction::getvmaxabs1(void) const
{
        switch(__MFM) {
                case mfmSquare: return __SF1->getvmaxabs();
                case mfmRamp: return __RF1->getvmaxabs();
        }
        throw EImpossibleError("motion funcion mode MFM should be mfmSquare or mfmRamp");
}
double TComposedMotionFunction::getvmaxabs2(void) const
{
        switch(__MFM) {
                case mfmSquare: return __SF2->getvmaxabs();
                case mfmRamp: return __RF2->getvmaxabs();
        }
        throw EImpossibleError("motion funcion mode MFM should be mfmSquare or mfmRamp");
}

//-------------------------------------------------------------------
//PROPIEDADES DE LECTURA/ESCRITURA EN FORMATO TEXTO:

void TComposedMotionFunction::setLabel(AnsiString _Label)
{
        //la cadena Label debe ser una cadena imprimible
        if(!StrIsPrintable(_Label))
                throw EImproperArgument("string Label should be a printable string");

        //asigna el nuevo valor
        __Label = _Label;
}

AnsiString TComposedMotionFunction::getSF1AddressText(void) const
{
        return IntToHex(reinterpret_cast<intptr_t>(__SF1), 8);
}
AnsiString TComposedMotionFunction::getRF1AddressText(void) const
{
        return IntToHex(reinterpret_cast<intptr_t>(__RF1), 8);
}
AnsiString TComposedMotionFunction::getSF2AddressText(void) const
{
        return IntToHex(reinterpret_cast<intptr_t>(__SF2), 8);
}
AnsiString TComposedMotionFunction::getRF2AddressText(void) const
{
        return IntToHex(reinterpret_cast<intptr_t>(__RF2), 8);
}

AnsiString TComposedMotionFunction::getMFMText(void) const
{
        switch(__MFM) {
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
        return SquareSynchronismModeToStr(__SSM);
}
void TComposedMotionFunction::setSSMText(AnsiString &S)
{
        try {
                setSSM(StrToSquareSynchronismMode(S));
        } catch(...) {
                throw;
        }
}
AnsiString TComposedMotionFunction::getRSMText(void) const
{
        return RampSynchronismModeToStr(__RSM);
}
void TComposedMotionFunction::setRSMText(AnsiString &S)
{
        try {
                setRSM(StrToRampSynchronismMode(S));
        } catch(...) {
                throw;
        }
}

AnsiString TComposedMotionFunction::getMF1Text(void) const
{
        if(__MF1 == NULL)
                return AnsiString("NULL");
        else if(__MF1 == __SF1)
                return getSF1AddressText();
        else if(__MF1 == __RF1)
                return getRF1AddressText();
        else
                throw EImpossibleError("pointer MF1 should point to NULL, SF1 or RF1");
}
AnsiString TComposedMotionFunction::getMF2Text(void) const
{
        if(__MF2 == NULL)
                return AnsiString("NULL");
        else if(__MF2 == __SF2)
                return getSF2AddressText();
        else if(__MF2 == __RF2)
                return getRF2AddressText();
        else
                throw EImpossibleError("pointer MF2 should point to NULL, SF2 or RF2");
}

AnsiString TComposedMotionFunction::gettsta1Text(void) const
{
        return FloatToStr(__tsta1);
}
void TComposedMotionFunction::settsta1Text(AnsiString &S)
{
        try {
                settsta1(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}
AnsiString TComposedMotionFunction::gettsta2Text(void) const
{
        return FloatToStr(__tsta2);
}
void TComposedMotionFunction::settsta2Text(AnsiString &S)
{
        try {
                settsta2(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}

AnsiString TComposedMotionFunction::getId1Text(void) const
{
        return IntToStr(__Id1);
}
void TComposedMotionFunction::setId1Text(AnsiString& S)
{
        try {
                setId1(StrToInt_(S));
        } catch(...) {
                throw;
        }
}
AnsiString TComposedMotionFunction::getId2Text(void) const
{
        return IntToStr(__Id2);
}
void TComposedMotionFunction::setId2Text(AnsiString& S)
{
        try {
                setId2(StrToInt_(S));
        } catch(...) {
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
        AnsiString S;

        //velocidad máxima absoluta
        S += AnsiString("SF1->vmaxabs = ")+__SF1->getvmaxabsText()+AnsiString("\r\n");
        S += AnsiString("SF2->vmaxabs = ")+__SF2->getvmaxabsText()+AnsiString("\r\n");
        S += AnsiString("RF1->vmaxabs = ")+__RF1->getvmaxabsText()+AnsiString("\r\n");
        S += AnsiString("RF2->vmaxabs = ")+__RF2->getvmaxabsText()+AnsiString("\r\n");

        //tipo de función, tipo de sincronismo y tipo de gesto
        S += AnsiString("MFM = ")+getMFMText()+AnsiString("\r\n");
        S += AnsiString("SSM = ")+getSSMText()+AnsiString("\r\n");
        S += AnsiString("RSM = ")+getRSMText()+AnsiString("\r\n");

        //identificadores de los rotores
        S += AnsiString("Id1 = ")+getId1Text()+AnsiString("\r\n");
        S += AnsiString("Id2 = ")+getId2Text();

        return S;
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
                Copy(CMF);

        } catch(...) {
                throw;
        }
}

//##################################################################
//MÉTODOS PÚBLICOS:
//##################################################################

//------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//lee una instancia de función de movimiento compuesta en una cadena
void  TComposedMotionFunction::ReadInstance(TComposedMotionFunction* &CMF,
        const AnsiString& S, int &i)
{
        //el puntero CMF debe apuntar a una función de movimeinto compuesta construido
        if(CMF == NULL)
                throw EImproperArgument("pointer CMF shouldpoint to built composed motion function");

        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de la máquina de estados de lectura
        //      0: esperando asignación a SF1->vmaxabs
        //      1: esperando asignación a SF2->vmaxabs
        //      2: esperando asignación a RF1->vmaxabs
        //      3: esperando asignación a RF2->vmaxabs
        //      4: esperando asignación a MFM
        //      5: esperando asignación a SSM
        //      6: esperando asignación a RSM
        //      7: esperando asignación a Id1
        //      8: esperando asignación a Id2
        //      9: instancia de función de movimiento compuesta leida con éxito
        int status = 0;

        //constuye la variable tampón
        TComposedMotionFunction _CMF(CMF);

        do {
                switch(status) {
                        case 0: //esperando asignación a SF1->vmaxabs
                                try {
                                        StrTravelLabel("SF1->vmaxabs", S, i);
                                        StrTravelLabel("=", S, i);
                                        double aux;
                                        StrReadFloat(aux, S, i);
                                        _CMF.__SF1->setvmaxabs(aux);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 1: //esperando asignación a SF2->vmaxabs
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("SF2->vmaxabs", S, i);
                                        StrTravelLabel("=", S, i);
                                        double aux;
                                        StrReadFloat(aux, S, i);
                                        _CMF.__SF2->setvmaxabs(aux);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 2: //esperando asignación a RF1->vmaxabs
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("RF1->vmaxabs", S, i);
                                        StrTravelLabel("=", S, i);
                                        double aux;
                                        StrReadFloat(aux, S, i);
                                        _CMF.__RF1->setvmaxabs(aux);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 3: //esperando asignación a RF2->vmaxabs
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("RF2->vmaxabs", S, i);
                                        StrTravelLabel("=", S, i);
                                        double aux;
                                        StrReadFloat(aux, S, i);
                                        _CMF.__RF2->setvmaxabs(aux);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 4: //esperando asignación a MFM
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("MFM", S, i);
                                        StrTravelLabel("=", S, i);
                                        TMotionFunctionMode aux;
                                        StrReadMotionFunctionMode(aux, S, i);
                                        _CMF.setMFM(aux);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 5: //esperando asignación a SSM
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("SSM", S, i);
                                        StrTravelLabel("=", S, i);
                                        TSquareSynchronismMode aux;
                                        StrReadSquareSynchronismMode(aux, S, i);
                                        _CMF.setSSM(aux);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 6: //esperando asignación a RSM
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("RSM", S, i);
                                        StrTravelLabel("=", S, i);
                                        TRampSynchronismMode aux;
                                        StrReadRampSynchronismMode(aux, S, i);
                                        _CMF.setRSM(aux);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 7: //esperando asignación a Id1
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("Id1", S, i);
                                        StrTravelLabel("=", S, i);
                                        int aux;
                                        StrReadInt(aux, S, i);
                                        _CMF.setId1(aux);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 8: //esperando asignación a Id2
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("Id2", S, i);
                                        StrTravelLabel("=", S, i);
                                        int aux;
                                        StrReadInt(aux, S, i);
                                        _CMF.setId2(aux);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                }
        //mientras no se haya leido la instancia con éxito
        } while(status < 9);

        //clona la variable tampón
        CMF->Copy(_CMF);
}
//---------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//construye una función de movimiento compuesta
TComposedMotionFunction::TComposedMotionFunction(void) :
        //inicializa los selectores a sus valores por defecto
        __MFM(mfmRamp), __SSM(ssmFree), __RSM(rsmFree),
        //inicializa los instantes de inicio de desplazamiento
        __tsta1(0), __tsta2(0),
        //inicializa los identificadores de las microcontroladoras
        __Id1(0), __Id2(0)
{
        //construye funciones cuadradas
        __SF1 = new TSquareFunction();
        __SF2 = new TSquareFunction();

        //construye funciones rampa
        __RF1 = new TRampFunction();
        __RF2 = new TRampFunction();

        //nombra las funciones
        __SF1->setLabel("SF1");
        __SF2->setLabel("SF2");
        __RF1->setLabel("RF1");
        __RF2->setLabel("RF2");

        //inicializa con ningún gesto
        __MF1 = NULL;
        __MF2 = NULL;
}

//copia todas las propiedades de una función de movimiento compuesta
void TComposedMotionFunction::Copy(const TComposedMotionFunction *CMF)
{
        //debe apuntar un objeto construido
        if(CMF == NULL)
                throw EImproperArgument("pointer CMF should not be null");

        //COPIA TODAS LAS PROPIEDADES:

        //copia las funciones
        __SF1->Copy(CMF->__SF1);
        __SF2->Copy(CMF->__SF2);
        __RF1->Copy(CMF->__RF1);
        __RF2->Copy(CMF->__RF2);

        //copia los selectores
        __MFM = CMF->__MFM;
        __SSM = CMF->__SSM;
        __RSM = CMF->__RSM;

        //copia los instantes de inicio de desplazamiento
        __tsta1 = CMF->__tsta1;
        __tsta2 = CMF->__tsta2;

        //copia los identificadores de las microcontroladoras
        __Id1 = CMF->__Id1;
        __Id2 = CMF->__Id2;

        //apunta a las mismas funciones homónimas que el objeto a clonar
        if(CMF->__MF1 == CMF->__SF1)
                __MF1 = __SF1;
        else if(CMF->__MF1 == CMF->__RF1)
                __MF1 = __RF1;
        else //solo podrá tener NULL, pero para no arastrar posibles errores
                __MF1 = CMF->__MF1; //se copia el valor literalmente

        //apunta a las mismas funciones homónimas que el objeto a clonar
        if(CMF->__MF2 == CMF->__SF2)
                __MF2 = __SF2;
        else if(CMF->__MF2 == CMF->__RF2)
                __MF2 = __RF2;
        else //solo podrá tener NULL, pero para no arastrar posibles errores
                __MF2 = CMF->__MF2; //se copia el valor literalmente

        //copia la etiqueta de la función
        __Label = CMF->__Label;
}
void TComposedMotionFunction::Copy(const TComposedMotionFunction& CMF)
{
        //copia las funciones
        __SF1->Copy(CMF.__SF1);
        __SF2->Copy(CMF.__SF2);
        __RF1->Copy(CMF.__RF1);
        __RF2->Copy(CMF.__RF2);

        //copia los selectores
        __MFM = CMF.__MFM;
        __SSM = CMF.__SSM;
        __RSM = CMF.__RSM;

        //copia los instantes de inicio de desplazamiento
        __tsta1 = CMF.__tsta1;
        __tsta2 = CMF.__tsta2;

        //copia los identificadores de las microcontroladoras
        __Id1 = CMF.__Id1;
        __Id2 = CMF.__Id2;

        //apunta a las mismas funciones homónimas que el objeto a clonar
        if(CMF.__MF1 == CMF.__SF1)
                __MF1 = __SF1;
        else if(CMF.__MF1 == CMF.__RF1)
                __MF1 = __RF1;
        else //solo podrá tener NULL, pero para no arastrar posibles errores
                __MF1 = CMF.__MF1; //se copia el valor literalmente

        //apunta a las mismas funciones homónimas que el objeto a clonar
        if(CMF.__MF2 == CMF.__SF2)
                __MF2 = __SF2;
        else if(CMF.__MF2 == CMF.__RF2)
                __MF2 = __RF2;
        else //solo podrá tener NULL, pero para no arastrar posibles errores
                __MF2 = CMF.__MF2; //se copia el valor literalmente

        //copia la etiqueta de la función
        __Label = CMF.__Label;
}

//construye un clon de una función de movimiento compuesta
TComposedMotionFunction::TComposedMotionFunction(const TComposedMotionFunction *CMF)
{
        //debe apuntar un objeto construido
        if(CMF == NULL)
                throw EImproperArgument("pointer CMF should not be null");

        //COPIA TODAS LAS PROPIEDADES:

        //clona las funciones
        __SF1 = new TSquareFunction(CMF->__SF1);
        __SF2 = new TSquareFunction(CMF->__SF2);
        __RF1 = new TRampFunction(CMF->__RF1);
        __RF2 = new TRampFunction(CMF->__RF2);

        //copia los selectores
        __MFM = CMF->__MFM;
        __SSM = CMF->__SSM;
        __RSM = CMF->__RSM;

        //copia los instantes de inicio de desplazamiento
        __tsta1 = CMF->__tsta1;
        __tsta2 = CMF->__tsta2;

        //copia los identificadores de las microcontroladoras
        __Id1 = CMF->__Id1;
        __Id2 = CMF->__Id2;

        //apunta a las mismas funciones homónimas que el objeto a clonar
        if(CMF->__MF1 == CMF->__SF1)
                __MF1 = __SF1;
        else if(CMF->__MF1 == CMF->__RF1)
                __MF1 = __RF1;
        else //solo podrá tener NULL, pero para no arastrar posibles errores
                __MF1 = CMF->__MF1; //se copia el valor literalmente

        //apunta a las mismas funciones homónimas que el objeto a clonar
        if(CMF->__MF2 == CMF->__SF2)
                __MF2 = __SF2;
        else if(CMF->__MF2 == CMF->__RF2)
                __MF2 = __RF2;
        else //solo podrá tener NULL, pero para no arastrar posibles errores
                __MF2 = CMF->__MF2; //se copia el valor literalmente

        //copia la etiqueta de la función
        __Label = CMF->__Label;
}

//destruye una función de movimiento compuesta
TComposedMotionFunction::~TComposedMotionFunction()
{
        //destruye las funciones rampa
        if(__RF2 != NULL)
                delete __RF2;
        if(__RF1 != NULL)
                delete __RF1;

        //destruye las funciones cuadradas
        if(__SF2 != NULL)
                delete __SF2;
        if(__SF1 != NULL)
                delete __SF1;
}

//-------------------------------------------------------------------
//MÉTODOS QUE PROGRAMAN GESTOS:

//apunta (MF1=(SF1 o RF1), MF2=NULL)
//programa la función de movimeinto 1
//desde la posición p_ini hasta la posición p_fin
void TComposedMotionFunction::ProgramMF1(double p_ini, double p_fin)
{
        //asigna la posición final al eje 1
        __SF1->SetInterval(p_ini, p_fin);
        __RF1->SetInterval(p_ini, p_fin);

        //ADVERTENCIA: la asignación de las posiciones inicial y final
        //a las funciones de movimiento debe ser conjunta
        //para no alterar el valor del periodo de desplazamiento
        //ya que este no puede reajustarse cuando SM == smFree

        //indica a la función correspondiente al tipo de función seleccionado
        switch(__MFM) {
                case mfmSquare: __MF1 = __SF1; break;
                case mfmRamp: __MF1 = __RF1; break;
        }
        __MF2 = NULL;

        //sincroniza las funciones según el modo
        setSSM(__SSM);
        setRSM(__RSM);
}
//apunta (MF1=NULL, MF2=(SF2 o RF2))
//programa la función de movimeinto 2
//desde la posición p__ini hasta la posición p__fin
void TComposedMotionFunction::ProgramMF2(double p__ini, double p__fin)
{
        //asigna la posición final al eje 2
        __SF2->SetInterval(p__ini, p__fin);
        __RF2->SetInterval(p__ini, p__fin);

        //ADVERTENCIA: la asignación de las posiciones inicial y final
        //a las funciones de movimiento debe ser conjunta
        //para no alterar el valor del periodo de desplazamiento
        //ya que este no puede reajustarse cuando SM == smFree

        //indica a la función correspondiente al tipo de función seleccionado
        __MF1 = NULL;
        switch(__MFM) {
                case mfmSquare: __MF2 = __SF2; break;
                case mfmRamp: __MF2 = __RF2; break;
        }

        //sincroniza las funciones según el modo
        setSSM(__SSM);
        setRSM(__RSM);
}
//apunta (MF1=SF1, MF2=SF2) o (MF1=RF1, MF2=RF2)
//programa ambas funciones de movimeinto
//desde la posición p_ini hasta la posición p_fin y
//desde la posición p__ini hasta la posición p__fin
void TComposedMotionFunction::ProgramBoth(double p_ini, double p__ini,
        double p_fin, double p__fin)
{
        //asigna la posición final al eje 1
        __SF1->SetInterval(p_ini, p_fin);
        __RF1->SetInterval(p_ini, p_fin);

        //asigna la posición final al eje 2
        __SF2->SetInterval(p__ini, p__fin);
        __RF2->SetInterval(p__ini, p__fin);

        //ADVERTENCIA: la asignación de las posiciones inicial y final
        //a las funciones de movimiento debe ser conjunta
        //para no alterar el valor del periodo de desplazamiento
        //ya que este no puede reajustarse cuando SM == smFree

        //indica a la función correspondiente al tipo de función seleccionado
        switch(__MFM) {
                case mfmSquare:
                        __MF1 = __SF1;
                        __MF2 = __SF2;
                        break;
                case mfmRamp:
                        __MF1 = __RF1;
                        __MF2 = __RF2;
                        break;
        }

        //sincroniza las funciones según el modo
        setSSM(__SSM);
        setRSM(__RSM);
}

//apunta (MF1=NULL, MF2=NULL)
void TComposedMotionFunction::ClearProgram(void)
{
        __MF1 = NULL;
        __MF2 = NULL;
}

/*//store the setting of the CMF
void TComposedMotionFunction::PushSetting(void)
{
    TO DEFINE
}
//recovery the stored setting of the CMF
void TComposedMotionFunction::PullSetting(void)
{
    TO DEFINE
}
*/
//sincroniza la llegada del gesto con el instante de tiempo indicado
void TComposedMotionFunction::SinchronizeArrival(double t)
{
        if(__MF1 != NULL)
                __tsta1 = t - __MF1->getT();
        if(__MF2 != NULL)
                __tsta2 = t - __MF2->getT();
}
//invierte la función en el dominio del tiempo
//y desplaza su inicio al mismo instante de inicio
void TComposedMotionFunction::InvertTime(void)
{
        double aux = gettendmax();

        //se recuerda que T = Max(tend1, tend2)

        if(__MF1 != NULL) {
                __MF1->InvertTime();
                __tsta1 = aux - gettend1();
        }
        if(__MF2 != NULL) {
                __MF2->InvertTime();
                __tsta2 = aux - gettend2();
        }
}

//evalúa la MF1 desplazada al instante de tiempo tsta1
//      MF1p(t) = MF1->p(t - tsta1)
double TComposedMotionFunction::MF1p(double t)
{
        //el puntero MF1 no debe apuntar a nulo
        if(__MF1 == NULL)
                throw EImproperArgument("pointer MF1 should not be null");

        //evalúa la función desplazada en el tiempo
        return __MF1->p(t - __tsta1);
}
//evalúa la MF2 desplazada al instante de tiempo tsta2
//      MF2p(t) = MF2->p(t - tsta2)
double TComposedMotionFunction::MF2p(double t)
{
        //el puntero MF2 no debe apuntar a nulo
        if(__MF2 == NULL)
                throw EImproperArgument("pointer MF2 should not be null");

        //evalúa la función desplazada en el tiempo
        return __MF2->p(t - __tsta2);
}

//---------------------------------------------------------------------------

} //namespace MotionFunctions

//---------------------------------------------------------------------------

