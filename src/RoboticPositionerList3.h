//---------------------------------------------------------------------------
//Archivo: RoboticPositionerList3.h
//Contenido: lista de posicionadores con funciones de programación y ejecución
//Última actualización: 07/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ROBOTICPOSITIONERLIST3_H
#define ROBOTICPOSITIONERLIST3_H

#include "RoboticPositionerList2.h"

//##include <QTimer>

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------

//La clase TRoboticPositionerList2:
//      Permite la programación conjunta de controladores
//      mediante comandos con el identificador 0.
//      Permite la determinación del periodo de desplazaemiento máximo.

//clase lista de posicionadores
class TRoboticPositionerList : public TRoboticPositionerList2 {
protected:
        //PROPIEDADES DE EJECUCIÓN:

        double __Tit;
        //#QTimer *__Timer;

        //PROPIEDADES DE EJECUCIÓN
        //EN FORMATO TEXTO:

        //CONJUNTOS DE PROPIEDADES EN MODO TEXTO:

public:
        //PROPIDADES EXTERNAS:

        //función de dibujo externa adscrita
        //para actualizar la caja de pintura del plano focal
        //valor por defecto: NULL
        void (__fastcall *PaintAll)(void);

        //Para que el temporizador pueda estar en el posicionador multifibra,
        //es necesario que disponga de un puntero a una función
        //de actualización de la interfaz.

        //PROPIEDADES DE EJECUCIÓN:

        //tiempo necesario para realizar todos los desplazamientos
        //      Tdis = Max{RoboticPositioners[i]->tendmax} s
        double getTdis(void) const;

        //periodo de iteración escalado
        //valor por defecto: 40 ms
        double getTit(void) const {return __Tit;}
        void setTit(const double);

        //temporizador del posicionador multifibra
        //configuración por defecto:
        //      Timer->Enabled = false
        //      Timer->Interval = 40 ms
        //      Timer->OnTimer = TimerTimer
//#        QTimer *getTimer(void) const {return __Timer;}

        //NOTA: el factor de dilatación del tiempo o escala temporal puede
        //calcularse como:
        //      gamma = double(Timer->Interval)/Tit;
        //donde 1./gamma representa el factor de aceleración del tiempo.

        //instante de tiempo actual
        //valor por defecto: 0 ms
        double t;

/*
        //número de iteraciones para realizar todos los desplazamientos
        //      Kd = ceil(Tdis/Tit)
        //valor por defecto: 0 it
        __property int Kd = {read=GetKd};

        //periodo de iteración del intervalo de desplazamiento en ms
        //      Td = Kd*Tit
        //valor por defecto: 0 ms
        __property double Td = {read=GetTd};

        //iteración de desplazameinto actual
        //dominio: [0, Kd]
        //valor por defecto: 0 it
        __property int kd = {read=__kd};

        //instante de desplazameinto actual en ms
        //      td = kd*Tit
        //dominio: [0, Td]
        //valor por defecto: 0 ms
        __property double td = {read=Gettd};
  */
        //------------------------------------------------------------------
        //PROPIEDADES DE EJECUCIÓN
        //EN FORMATO TEXTO:

        AnsiString getTdisText(void) const;

        AnsiString getTitText(void) const; void setTitText(const AnsiString&);
        AnsiString getTimerEnabledText(void) const;
        void setTimerEnabledText(const AnsiString&);
        AnsiString getTimerIntervalText(void) const;
        void setTimerIntervalText(const AnsiString&);
        AnsiString gettText(void) const; void settText(const AnsiString&);

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjunto de propiedades de ejecición
        //en formato asignaciones de texto
        AnsiString getExecutionText(void) const;

/*        //conjunto de propiedades de instancia
        //en formato asignacionesde texto
        AnsiString getInstanceText(void) const;
        void setInstanceText(const AnsiString&);
  */
        //##################################################################
        //MÉTODOS PÚBLICOS:
        //##################################################################

/*        //------------------------------------------------------------------
        //MÉTODOS ESTÁTICOS:

        //lee una instancia de posicionador multifibra
        //en una cadena de texto en formato de asignaciones
        static void __fastcall ReadInstance(TRoboticPositionerList *FPL,
                const AnsiString &S, int &i);
  */
        //------------------------------------------------------------------
        //MÉTODOS DE CONSTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

        //construye una lista de posicionadores de fibra
        TRoboticPositionerList(void);

        //ADVERTENCIA: para cosntruir un posicionador multifibra debe haber
        //un posicinador de fibra por defecto que no incluya un puntero
        //al método Paint.

        //copia las propiedades de ejecución de una lista de posicionadores de fibra
        void CopyExecution(const TRoboticPositionerList*);

        //clona una lista de posicionadores de fibra
        void Clone(const TRoboticPositionerList*);

        //construye un clon de un posicionador multifibra
        TRoboticPositionerList(const TRoboticPositionerList*);

        //destruye una lista de posicionadores de fibra
        ~TRoboticPositionerList();

/*        //ADVERTENCIA: antes de destruir la última lista de controladores
        //debe desturir los controladores, lo cual puede hacerse mediante
        //'Destroy()'.
  */
/*        //------------------------------------------------------------------
        //MARGEN PERIMETRAL DE SEGURIDAD:

        //asigna un periodo de iteración a todos los posicionadores
        void SetTi(double);
        //asigna un estado de validación a todos los posicionadores
        void SetValidating(bool);
        //asigna un estado de ejecución a todos los posicionadores
        void SetExecuting(bool);           */

        //MÉTODOS DE CONFIGURACIÓN:

        //obtiene la tabla de identificadores (Id, Id1, Id2)
        void GetIdTable(TPointersList<TTernIntegers>& IdTable) const;
        //asigna la tabla de identificadores (Id, Id1, Id2)
        //si IdTable no contiene una terna por cada posicionador:
        //      lanza una excepción EImproperArgument
        //si algún identificador en IdTable no es unívoco:
        //      lanza una excepción EImproperArgument
        //si algún Id en IdTable no se refiere a un posicionador de la lista:
        //      lanza una excepción EImproperArgument
        void SetIdTable(TPointersList<TTernIntegers>& IdTable);

        //configure el SPM de los posicionadores de una lista
        //según el propósito:
        //	pAll: asignación
        //	pGen: generación
        //	pVal: validación
        //	pExe: ejecución
        void SetPorpose(TPorpose Porpose);

        //configura el estado de habilitación del cuantificador de rot 1
        //de todos los posicionadores de la lista
        void SetQuantify_s(bool);
        //configura el estado de habilitación del cuantificador de rot 2
        //de todos los posicionadores de la lista
        void SetArmQuantify___s(bool);
        //configura el estado de habilitación de los cuantificadores
        //de todos los posicionadores de la lista
        void SetQuantifys(bool Quantify_, bool Quantify___);

        //configura el estado de colisión
        //de todos los posicionadores de la lista
        void SetCollisions(bool);

        //------------------------------------------------------------------
        //MÉTODOS PARA COMPROBAR LASPRECONDICIONES:

        //determina si todos los posicionadores operativos en
        //posiciones de inseguridad están en la lista Abatibles
        bool NotAllOperativeInUnsafetyPositionsAreInTheList(
                TRoboticPositionerList *Abatibles) const;

        //determina si todos los posicionadores de la lista
        //están operativos en posiciones de inseguridad
        bool NotAllPositionersAreOperativesInUnsafetyPositions(void) const;

        //------------------------------------------------------------------
        //MÉTODOS DE PROGRAMACIÓN:

        //borra las instrucciones de movimeinto de los posicionadores
        void ClearInstructions(void);

        //asigna una instrucción
        //si el identificador Id es negativo
        //      lanza una excepción EImproperArgument
        //si el posicionador Id no está en la lista
        //      lanza una excepción EImproperArgument
        //si Id==0 asigna la instrucción a todos los controldaores
        void SetInstruction(int Id, TInstruction &Instruction);

        //programa gestos de abatimiento en todos los controladores de la lista
        void TurnArmsToSafeAreas(void);
        //sincroniza las llegadas de todos los gestos de la lista
        void SinchronizeArrivals(void);
        //invierte todos los gestos de la lista en el dominio del tiempo
        void InvertTime(void);

        //MÉTODOS DE MOVIMIENTO:

        //mueve los RPs a la posición indicada
        //si el puntero PainAll no apunta a una función
        //      lanza EImproperCall
        //por las funciones de movimiento en el instante t
        //no actualiza k
        void Move(double t);
        //mueve los RPs a sus posiciones inciiales
        //si el puntero PainAll no apunta a una función
        //      lanza EImproperCall
        //no actualiza k
        void MoveSta(void);
        //mueve los RPs a sus posiciones finales
        //si el puntero PainAll no apunta a una función
        //      lanza EImproperCall
        //no actualiza k
        void MoveFin(void);

/*#public slots:
        //realiza una iteración
        //si el puntero PainAll no apunta a una función
        //      lanza EImproperCall
        void TimerTimer(void);
*/
/*public:
        //MÉTODOS DE EJECUCIÓN:

        //avanza la simulación un paso (kd++)
        //si kd>=Kd lanza una excepción EImproperCall
        //cuando kd>=Kd deshabilita el reloj de desplazamiento
        void NextStep(void);
        //retrocede la simulación un paso (kd--)
        //si kd<=0 lanza una excepción EImproperCall
        //cuando kd<=0 deshabilita el reloj de desplazamiento
        void PrevStep(void);

        //asigna NexStep a Timer->OnTimer y
        //desplaza los posicionadores hasta el final (kd==Kd)
        void DisplaceForward(void);
        //asigna PrevStep a Timer->OnTimer y
        //desplaza los posicionadores hasta el principio (kd==0)
        void DisplaceBackward(void);
  */
};
/*
//lista de posicionadores de fibra en formato texto
class TRoboticPositionerListText {
public:
        //tabla de orígenes de coordenadas
        AnsiString OriginsTableText;

        //lista de ternas de cadenas de texto para coneter
        //las siguientes cadenas por cada posicionador:
        //      Actuator->Arm->Contour____ColumnText
        //      Actuator->Barrier->Contour_ColumnText
        //      InstanceText
        TTPointersList<ClusterSSS> ItemsText;

        //consturye una lista de posicionadores de fibra en formato texto
        TRoboticPositionerListText(void);

        //vacia la lista de posicionadores de fibra en formato texto
        void Clear(void);

        //indica si el objeto no contiene una instancia
        bool IsEmpty(void);

        //aplica la instancia a una lista de posicionadores de fibra
        void GetInstance(TRoboticPositionerList& FPL) const;
        //obtiene la instancia de una lista de posicionadores de fibra
        void SetInstance(TRoboticPositionerList& FPL);
};*/

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // ROBOTICPOSITIONERLIST3_H
