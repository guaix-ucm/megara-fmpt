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
/// @file ComposedMotionFunction.h
/// @brief composed motion funtion (CMF) model of a RP
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef COMPOSEDMOTIONFUNCTION_H
#define COMPOSEDMOTIONFUNCTION_H

#include "SquareFunction.h"
#include "RampFunction.h"

//---------------------------------------------------------------------------

//namespace for motion functions
namespace MotionFunctions {

//---------------------------------------------------------------------------
//TComposedMotionFunction
//---------------------------------------------------------------------------

//La clase TComposedMotionFunction permite programar una función de movimeinto
//para cada uno de los ejes que deben ser girados para que un posicionador
//realice un determinado gesto.
//Las funciones de movimeinto serán programadas partiendo de la posición inicial
//para que P1, P2 ó P3, alcancen la posición final indicada
//en coordenadas polares relativas a S1:
//      theta_1fin;
//      r_2fin, theta_2fin;
//      r_3fin, theta_3fin;
//Cuando en el gesto interviene un único rotor, es posible ajustar
//el intervalo temporal de desplazamiento de la función, así como
//el resto de parámetros libres de la función,
//y cuando en el gesto intervienen los dos rotores, es posible sincronizar
//los periodos de las funciones de movimeinto de los rotores.
//Las funciones de movimiento se programarán en pasos.

//Son importantes dos cuestiones:
//      1. Debe programar el movimiento de cada eje de manera independiente,
//      de modo que al mover un eje, el otro quede libre para ser desplazado;
//      de este modo no hace falta implementar una función Displace(t, qt).
//      2. Cada gesto se inicia en el instante t=0, de modo que se facilita
//      la realización aislada, lo cual simplifica el algoritmo de busqueda;
//      cuando haya que concatenar varios gestos, se sincronizarán con una
//      variable temporal externa que indique el instante en que debe iniciarse.

//---------------------------------------------------------------------------
//TMotionFunctionMode
//---------------------------------------------------------------------------

//modo de función de movimiento
enum TMotionFunctionMode {
    mfmSquare, //función de movimeinto cuadrada
    mfmRamp //función de movimiento rampa
};

void  StrPrintMotionFunctionMode(AnsiString& S,
                                 TMotionFunctionMode mfm);
void  StrReadMotionFunctionMode(TMotionFunctionMode& mfm,
                                const AnsiString &S, int &i);
AnsiString MotionFunctionModeToStr(TMotionFunctionMode mfm);
TMotionFunctionMode StrToMotionFunctionMode(const AnsiString& S);

//---------------------------------------------------------------------------
//TSquareSynchronismMode
//---------------------------------------------------------------------------

//modo de sincronismo de las funciones cuadradas
enum TSquareSynchronismMode {
    ssmFree, //no cambia los tiempos de desplazamiento

    ssmTmin, //MF1->T = MF1->Tmin; MF2->T = MF2->Tmin;
    ssmMaxTmin, //MF2->T = MF1->T = max(MF1->Tmin, MF2->Tmin);
    ssmDouTmin //MF1->T = MF1->Tmin; MF2->T = 2*MF2->Tmin;
};

void  StrPrintSquareSynchronismMode(AnsiString& S,
                                    TSquareSynchronismMode ssm);
void  StrReadSquareSynchronismMode(TSquareSynchronismMode& ssm,
                                   const AnsiString &S, int &i);
AnsiString SquareSynchronismModeToStr(TSquareSynchronismMode ssm);
TSquareSynchronismMode StrToSquareSynchronismMode(const AnsiString& S);

//---------------------------------------------------------------------------
//TRampSynchronismMode
//---------------------------------------------------------------------------

//modo de sincronismo de las funciones rampa
enum TRampSynchronismMode {
    rsmFree, //no cambia los tiempos de desplazamiento

    rsmTmin, //MF1->T = MF1->Tmin; MF2->T = MF2->Tmin;
    rsmMaxTmin, //MF2->T = MF1->T = max(MF1->Tmin, MF2->Tmin);
    rsmDouTmin, //MF1->T = MF1->Tmin; MF2->T = 2*MF2->Tmin;

    rsmTv, //MF1->T = MF1->Tv; MF2->T = MF2->Tv;
    rsmMaxTv, //MF2->T = MF1->T = max(MF1->Tv, MF2->Tv);
    rsmDouTv //MF1->T = MF1->Tv; MF2->T = 2*MF2->Tv;
};

void  StrPrintRampSynchronismMode(AnsiString& S,
                                  TRampSynchronismMode rsm);
void  StrReadRampSynchronismMode(TRampSynchronismMode& rsm,
                                 const AnsiString &S, int &i);
AnsiString RampSynchronismModeToStr(TRampSynchronismMode rsm);
TRampSynchronismMode StrToRampSynchronismMode(const AnsiString& S);

//---------------------------------------------------------------------------
//TComposedMotionFunction
//---------------------------------------------------------------------------

//la operación de sincronización solo se aplicará a
//las funciones de movimiento que no apunten a nulo

//La función de movimiento compuesta puede ser utilizada con
//cualquier unidad, tanto pasos como radianes.
//Sin embargo para programar los gestos es preferible hacerlo en pasos
//para poder facilitar la elección de escalones de cuantificación iguales
//a un número entero de pasos.

//La posición del eje 2 puede referirse tanto a la orientación de
//P2 (centro del cabezal) como a la orientación de P3 (centro de la lente)
//en S1, pero es preferible que se refiera a la de P3,
//ya que el mismo será alineado con el el cero lógico en la posición 0 pasos
//y por consiguiente los centroides estarán en el centro de los escalones
//de cuantificación, minimizando el error de cuantificación.

//clase función de movimiento compuesta
class TComposedMotionFunction {
    //PROPIEDADES DE LECTURA/ESCRITURA:

    TSquareFunction *p_SF1, *p_SF2;
    TRampFunction *p_RF1, *p_RF2;

    TMotionFunctionMode p_MFM;
    TSquareSynchronismMode p_SSM;
    TRampSynchronismMode p_RSM;

    TMotionFunction *p_MF1, *p_MF2;

    double p_tsta1;
    double p_tsta2;

    int p_Id1;
    int p_Id2;

    //-------------------------------------------------------------------
    //PROPIEDADES DE LECTURA/ESCRITURA EN FORMATO TEXTO:

    AnsiString p_Label;

public:
    //-------------------------------------------------------------------
    //PROPIDADES DE LECTURA/ESCRITURA:

    //función cuadrada del eje 1
    TSquareFunction *getSF1(void) const {return p_SF1;}
    //función cuadrada del eje 2
    TSquareFunction *getSF2(void) const {return p_SF2;}

    //función rampa del eje 1
    TRampFunction *getRF1(void) const {return p_RF1;}
    //función rampa del eje 2
    TRampFunction *getRF2(void) const {return p_RF2;}

    //modo de función de movimiento
    //valores posible:
    //      smfSquare: los punteros a funciones de movimeinto
    //              apuntan a funciones cuadradas o a nulo
    //      smfRamp: los punteros a funciones de movimeinto
    //              apuntan a funciones rampa o a nulo
    //valor por defecto: smfRamp
    TMotionFunctionMode getMFM(void) const {return p_MFM;}
    void setMFM(TMotionFunctionMode);

    //modo de sincronismo de las funciones cuadradas
    //valores posibles:
    //      ssmFree: no cambia los tiempos de desplazamiento
    //      ssmTmin: SF1->T = SF1->Tmin; SF2->T = SF2->Tmin;
    //      ssmMaxTmin: SF2->T = SF1->T = max(SF1->Tmin, SF2->Tmin);
    //      ssmDouTmin: SF1->T = SF1->Tmin*2; SF2->T = SF2->Tmin;
    //valor por defecto ssfFree
    TSquareSynchronismMode getSSM(void) const {return p_SSM;}
    void setSSM(TSquareSynchronismMode);

    //modo de sincronismo de las funciones rampa
    //valores posibles:
    //      rsmFree: no cambia los tiempos de desplazamiento
    //      rsmTmin: RF1->T = RF1->Tmin; RF2->T = RF2->Tmin;
    //      rsmMaxTmin: RF2->T = RF1->T = max(RF1->Tmin, RF2->Tmin);
    //      rsmDouTmin: RF1->T = RF1->Tmin*2; RF2->T = RF2->Tmin;
    //      rsmTv: RF1->T = RF1->Tv; RF2->T = RF2->Tv;
    //      rsmMaxTv: RF2->T = RF1->T = max(RF1->Tv, RF2->Tv);
    //      rsmDouTv: RF1->T = RF1->Tv*2; RF2->T = RF2->Tv;
    //valor por defecto: rsmFree
    TRampSynchronismMode getRSM(void) const {return p_RSM;}
    void setRSM(TRampSynchronismMode);

    //la sincronización de ejes puede afectar a los gestos compuestos:
    //      GoDirectly: un eje puede llegar antes que el otro;
    //      GoRadial: la trayectoria puede no ser radial.
    //El hecho de que sea radial está supeditado a que la velocidad
    //del eje 2 seael doble que la velocidad del eje 1.

    //puntero a la función de movimiento seleccionada del eje 1
    //valor por defecto: NULL
    TMotionFunction *getMF1(void) const {return p_MF1;}
    //puntero a la función de movimiento seleccionada del eje 2
    //valor por defecto: NULL
    TMotionFunction *getMF2(void) const {return p_MF2;}

    //cuando:
    //      MF1==NULL && MF2==NULL: gira ningún eje
    //      MF1!=NULL && MF2==NULL: gira el eje 1
    //      MF1==NULL && MF2!=NULL: gira el eje 2
    //      MF1!=NULL && MF2!=NULL: gira ambos ejes

    //periodo de desplazamiento del gesto
    //      T = max(tend1, tend2)
    //cuando T es cambiado,
    //los periodos de los rotores mantienen la proporción
    //valor mínimo: t.q. MF1->T==MF1->Tmin && MF2->T>=MF2->Tmin ||
    //      MF1->T>=MF1->Tmin && MF2->T==MF2->Tmin
    //double GetT(void);  void SetT(double);

    //instante de inicio de la función de movimeinto del eje 1
    //valor por defecto: 0
    double gettsta1(void) const {return p_tsta1;}
    void settsta1(double);
    //instante de inicio de la función de movimeinto del eje 2
    //valor por defecto: 0
    double gettsta2(void) const {return p_tsta2;}
    void settsta2(double);

    //identificador de la microcontroladora de rot 1
    //debe ser un número no negativo
    //valor por defecto: 0
    int getId1(void) const {return p_Id1;}
    void setId1(int);
    //identificador de la microcontroladora de rot 2
    //debe ser un número no negativo
    //valor por defecto: 0
    int getId2(void) const {return p_Id2;}
    void setId2(int);

    //PROPIEDADES DE SOLO LECTURA:

    //instante de fin de la función de movimeinto del eje 1
    //      tend1 = tsta1 + MF1->T
    double gettend1(void) const;
    //instante de fin de la función de movimeinto del eje 2
    //      tend2 = tsta2 + MF2->T
    double gettend2(void) const;

    //instante de inicio del desplazamiento
    //      tstamin = min(tsta1, tsta2)
    double gettstamin(void) const;
    //instante de fin del desplazamiento
    //      tendmax = max(tend1, tend2)
    double gettendmax(void) const;

    //determina el valor absoluto de la velocidad máxima del eje 1
    //      MFM=mfmSquare => SF1->vmaxabs steps/ms
    //      MFM=mfmRamp => RF1->vmaxabs steps/ms
    double getvmaxabs1(void) const;
    //determina el valor absoluto de la velocidad máxima del eje 2
    //      MFM=mfmSquare => SF2->vmaxabs steps/ms
    //      MFM=mfmRamp => RF2->vmaxabs steps/ms
    double getvmaxabs2(void) const;

    //-------------------------------------------------------------------
    //PROPIEDADES DE LECTURA/ESCRITURA EN FORMATO TEXTO:

    //nombre de la función de movimiento compuesto
    //debe ser una cadena imprimible
    //valor por defecto: ""
    AnsiString getLabel(void) const {return p_Label;}
    void setLabel(AnsiString);

    //dirección en memoria de la función SF1 en formato texto
    AnsiString getSF1AddressText(void) const;
    //dirección en memoria de la función RF1 en formato texto
    AnsiString getRF1AddressText(void) const;
    //dirección en memoria de la función SF2 en formato texto
    AnsiString getSF2AddressText(void) const;
    //dirección en memoria de la función RF2 en formato texto
    AnsiString getRF2AddressText(void) const;

    AnsiString getMFMText(void) const;
    void setMFMText(AnsiString&);
    AnsiString getSSMText(void) const;
    void setSSMText(AnsiString&);
    AnsiString getRSMText(void) const;
    void setRSMText(AnsiString&);

    //dirección enmemoria de la función de movimiento 1
    //si MF1=NULL => MF1Text="NULL"
    AnsiString getMF1Text(void) const;
    //dirección en memoria de la función de movimiento 2
    //si MF2=NULL => MF2Text="NULL"
    AnsiString getMF2Text(void) const;

    AnsiString gettsta1Text(void) const; void settsta1Text(AnsiString&);
    AnsiString gettsta2Text(void) const; void settsta2Text(AnsiString&);

    AnsiString getId1Text(void) const; void setId1Text(AnsiString&);
    AnsiString getId2Text(void) const; void setId2Text(AnsiString&);

    //PROPIEDADES DERIVADAS DE CONFIGURACIÓN EN MODO TEXTO:

    AnsiString gettend1Text(void) const;
    AnsiString gettend2Text(void) const;

    AnsiString gettstaminText(void) const;
    AnsiString gettendmaxText(void) const;

    AnsiString getvmaxabs1Text(void) const;
    AnsiString getvmaxabs2Text(void) const;

    //CONJUNTO DE PROPIEDADES EN MODO TEXTO:

    //conjunto de todas las propiedades en formato texto
    AnsiString getAllText(void) const;

    //conjunto de las propiedades de instancia en formato texto
    AnsiString getInstanceText(void) const;
    void setInstanceText(const AnsiString&);

    //-------------------------------------------------------------------
    //MÉTODOS ESTÁTICOS:

    //lee una instancia de función de movimiento compuesta en una cadena
    static void  ReadInstance(TComposedMotionFunction *CMF,
                              const AnsiString& S, int &i);

    //-------------------------------------------------------------------
    //MÉTODOS PÚBLICOS:

    //construye una función de movimiento compuesta
    TComposedMotionFunction(void);

    //clona todas las propiedades de una función de movimiento compuesta
    void Clone(const TComposedMotionFunction*);
    void Clone(const TComposedMotionFunction&);

    //contruye un clon de una función de movimiento compuesta
    TComposedMotionFunction(const TComposedMotionFunction*);

    //destruye una función de movimiento compuesta
    ~TComposedMotionFunction();

    //MÉTODOS QUE PROGRAMAN DESPLAZAMIENTOS:

    //apunta (MF1=(SF1 ó RF1), MF2=NULL)
    //programa la función de movimeinto 1
    //desde la posición p_ini hasta la posición p_fin
    void ProgramMF1(double p_ini, double p_fin);
    //apunta (MF1=NULL, MF2=(SF2 ó RF2))
    //programa la función de movimeinto 2
    //desde la posición p__ini hasta la posición p__fin
    void ProgramMF2(double p__ini, double p__fin);
    //apunta (MF1=SF1, MF2=SF2) ó (MF1=RF1, MF2=RF2)
    //programa el movimiento de ambas funciones de movimiento
    //desde la posición (p_ini, p__ini)
    //hasta la posición (p_fin, p__fin)
    void ProgramBoth(double p_ini, double p__ini,
                     double p_fin, double p__fin);

    //los gestos se programarán según el tipo de función seleccionada
    //y el tipo de sincronismo seleccionado para el tipo de función

    //initialize the CMF
    void ClearProgram(void);
    //store the setting of the CMF
    void PushSetting(void);
    //recovery the stored setting of the CMF
    void PullSetting(void);

    //sincroniza la llegada del gesto con el instante de tiempo indicado
    void SinchronizeArrival(double t);
    //invierte la función en el dominio del tiempo
    //y desplaza su inicio al mismo instante de inicio
    void InvertTime(void);

    //evalúa la MF1 desplazada al instante de tiempo tsta1
    //      MF1p(t) = MF1->p(t - tsta1)
    double MF1p(double t);
    //evalúa la MF2 desplazada al instante de tiempo tsta2
    //      MF2p(t) = MF2->p(t - tsta2)
    double MF2p(double t);
};

//---------------------------------------------------------------------------

} //namespace MotionFunctions

//---------------------------------------------------------------------------
#endif // COMPOSEDMOTIONFUNCTION_H
