//---------------------------------------------------------------------------
//Archivo: ComposedMotionFunction.h
//Contenido: funcion de movimiento compuesta
//Última actualización: 07/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef COMPOSEDMOTIONFUNCTION_H
#define COMPOSEDMOTIONFUNCTION_H

#include "SquareFunction.h"
#include "RampFunction.h"

//---------------------------------------------------------------------------

//espacio de nombres de funciones de movimiento
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
//Cunado en el gesto interviene un único rotor, es posible ajustar
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

void __fastcall StrPrintMotionFunctionMode(AnsiString& S,
        TMotionFunctionMode mfm);
void __fastcall StrReadMotionFunctionMode(TMotionFunctionMode& mfm,
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
        ssmMaxTmin, //MF2->T = MF1->T = Max(MF1->Tmin, MF2->Tmin);
        ssmDouTmin //MF1->T = MF1->Tmin; MF2->T = 2*MF2->Tmin;
};

void __fastcall StrPrintSquareSynchronismMode(AnsiString& S,
        TSquareSynchronismMode ssm);
void __fastcall StrReadSquareSynchronismMode(TSquareSynchronismMode& ssm,
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
        rsmMaxTmin, //MF2->T = MF1->T = Max(MF1->Tmin, MF2->Tmin);
        rsmDouTmin, //MF1->T = MF1->Tmin; MF2->T = 2*MF2->Tmin;

        rsmTv, //MF1->T = MF1->Tv; MF2->T = MF2->Tv;
        rsmMaxTv, //MF2->T = MF1->T = Max(MF1->Tv, MF2->Tv);
        rsmDouTv //MF1->T = MF1->Tv; MF2->T = 2*MF2->Tv;
};

void __fastcall StrPrintRampSynchronismMode(AnsiString& S,
        TRampSynchronismMode rsm);
void __fastcall StrReadRampSynchronismMode(TRampSynchronismMode& rsm,
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

        TSquareFunction *__SF1, *__SF2;
        TRampFunction *__RF1, *__RF2;

        TMotionFunctionMode __MFM;
        TSquareSynchronismMode __SSM;
        TRampSynchronismMode __RSM;

        TMotionFunction *__MF1, *__MF2;

//        double GetT(void);  void SetT(double);

        double __tsta1;
        double __tsta2;

        int __Id1;
        int __Id2;

        //PROPIEDADES DE SOLO LECTURA:


        //-------------------------------------------------------------------
        //PROPIEDADES DE LECTURA/ESCRITURA EN FORMATO TEXTO:

        AnsiString __Label;

        //PROPIEDADES DE SOLO LECTURA EN FORMATO TEXTO:


        //CONJUNTO DE PROPIEDADES EN FORMATO TEXTO:


public:
        //-------------------------------------------------------------------
        //PROPIDADES DE LECTURA/ESCRITURA:

        //función cuadrada del eje 1
        TSquareFunction *getSF1(void) const {return __SF1;}
        //función cuadrada del eje 2
        TSquareFunction *getSF2(void) const {return __SF2;}

        //función rampa del eje 1
        TRampFunction *getRF1(void) const {return __RF1;}
        //función rampa del eje 2
        TRampFunction *getRF2(void) const {return __RF2;}

        //modo de función de movimiento
        //valores posible:
        //      smfSquare: los punteros a funciones de movimeinto
        //              apuntan a funciones cuadradas o a nulo
        //      smfRamp: los punteros a funciones de movimeinto
        //              apuntan a funciones rampa o a nulo
        //valor por defecto: smfSquare
        TMotionFunctionMode getMFM(void) const {return __MFM;}
        void setMFM(TMotionFunctionMode);

        //modo de sincronismo de las funciones cuadradas
        //valores posibles:
        //      ssmFree: no cambia los tiempos de desplazamiento
        //      ssmTmin: SF1->T = SF1->Tmin; SF2->T = SF2->Tmin;
        //      ssmMaxTmin: SF2->T = SF1->T = Max(SF1->Tmin, SF2->Tmin);
        //      ssmDouTmin: SF1->T = SF1->Tmin*2; SF2->T = SF2->Tmin;
        //valor por defecto ssfFree
        TSquareSynchronismMode getSSM(void) const {return __SSM;}
        void setSSM(TSquareSynchronismMode);

        //modo de sincronismo de las funciones rampa
        //valores posibles:
        //      rsmFree: no cambia los tiempos de desplazamiento
        //      rsmTmin: RF1->T = RF1->Tmin; RF2->T = RF2->Tmin;
        //      rsmMaxTmin: RF2->T = RF1->T = Max(RF1->Tmin, RF2->Tmin);
        //      rsmDouTmin: RF1->T = RF1->Tmin*2; RF2->T = RF2->Tmin;
        //      rsmTv: RF1->T = RF1->Tv; RF2->T = RF2->Tv;
        //      rsmMaxTv: RF2->T = RF1->T = Max(RF1->Tv, RF2->Tv);
        //      rsmDouTv: RF1->T = RF1->Tv*2; RF2->T = RF2->Tv;
        //valor por defecto: rsmFree
        TRampSynchronismMode getRSM(void) const {return __RSM;}
        void setRSM(TRampSynchronismMode);

        //la sincronización de ejes puede afectar a los gestos compuestos:
        //      GoDirectly: un eje puede llegar antes que el otro;
        //      GoRadial: la trayectoria puede no ser radial.
        //El hecho de que sea radial está supeditado a que la velocidad
        //del eje 2 seael doble que la velocidad del eje 1.

        //puntero a la función de movimiento seleccionada del eje 1
        //valor por defecto: NULL
        TMotionFunction *getMF1(void) const {return __MF1;}
        //puntero a la función de movimiento seleccionada del eje 2
        //valor por defecto: NULL
        TMotionFunction *getMF2(void) const {return __MF2;}

        //cuando:
        //      MF1==NULL && MF2==NULL: no gira ningún eje
        //      MF1!=NULL && MF2==NULL: gira el eje 1
        //      MF1==NULL && MF2!=NULL: gira el eje 2
        //      MF1!=NULL && MF2!=NULL: gira ambos ejes

        //periodo de desplazamiento del gesto
        //      T = Max(tend1, tend2)
        //cuando T es cambiado,
        //los periodos de los ejes mantienen la proporción
        //valor mínimo: t.q. MF1->T==MF1->Tmin && MF2->T>=MF2->Tmin ||
        //      MF1->T>=MF1->Tmin && MF2->T==MF2->Tmin
//        __property double T = {read=GetT, write=SetT};

        //instante de inicio de la función de movimeinto del eje 1
        //valor por defecto: 0
        double gettsta1(void) const {return __tsta1;}
        void settsta1(double);
        //instante de inicio de la función de movimeinto del eje 2
        //valor por defecto: 0
        double gettsta2(void) const {return __tsta2;}
        void settsta2(double);

        //identificador de la microcontroladora de rot 1
        //debe ser un número no negativo
        //valor por defecto: 0
        int getId1(void) const {return __Id1;}
        void setId1(int);
        //identificador de la microcontroladora de rot 2
        //debe ser un número no negativo
        //valor por defecto: 0
        int getId2(void) const {return __Id2;}
        void setId2(int);

        //PROPIEDADES DE SOLO LECTURA:

        //instante de fin de la función de movimeinto del eje 1
        //      tend1 = tsta1 + MF1->T
        double gettend1(void) const;
        //instante de fin de la función de movimeinto del eje 2
        //      tend2 = tsta2 + MF2->T
        double gettend2(void) const;

        //instante de inicio del desplazamiento
        //      tstamin = Min(tsta1, tsta2)
        double gettstamin(void) const;
        //instante de fin del desplazamiento
        //      tendmax = Max(tend1, tend2)
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
        AnsiString getLabel(void) const {return __Label;}
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

        //##################################################################
        //MÉTODOS PÚBLICOS:
        //##################################################################

        //-------------------------------------------------------------------
        //MÉTODOS ESTÁTICOS:

        //lee una instancia de función de movimiento compuesta en una cadena
        static void __fastcall ReadInstance(TComposedMotionFunction* &CMF,
                const AnsiString& S, int &i);

        //-------------------------------------------------------------------
        //MÉTODOS PÚBLICOS:

        //construye una función de movimiento compuesta
        TComposedMotionFunction(void);

        //copia todas las propiedades de una función de movimiento compuesta
        void Copy(const TComposedMotionFunction*);
        void Copy(const TComposedMotionFunction&);

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
        //programa el movimiento de ambas funciones de movimeinto
        //desde la posición (p_ini, p__ini)
        //hasta la posición (p_fin, p__fin)
        void ProgramBoth(double p_ini, double p__ini,
                double p_fin, double p__fin);

        //los gestos se programarán según el tipo de función seleccionada
        //y el tipo de sincronismo seleccionado para el tipo de función

        //apunta (MF1=NULL, MF2=NULL)
        void ClearProgram(void);

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
