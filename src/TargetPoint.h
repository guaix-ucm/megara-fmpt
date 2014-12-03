//---------------------------------------------------------------------------
//Archivo: TargetPoint.h
//Contenido: punto objetivo adscrito a un posicionador de fibra
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef TARGETPOINT_H
#define TARGETPOINT_H

#include "FiberMOSModel2.h"
//#include "..\3_Lists\ItemsList.h"

//---------------------------------------------------------------------------

using namespace Lists;
using namespace Models;

//espacio de nombres de posicionamiento
namespace Positioning {

//---------------------------------------------------------------------------
//TTargetPoint:
//---------------------------------------------------------------------------

//clase punto objetivo
class TTargetPoint {
        //PROPIEDADES ESTÁTICAS:

        //lista de punteros a puntos objetivo construidos
        static TItemsList<TTargetPoint*> Builts;

        //Permite controlar la construcción de un solo punto objetivo
        //por posicionador, así como la destrucción de puntos existentes.

        //PROPIEDADES DINÁMICAS:

        TRoboticPositioner *__RoboticPositioner;

        //PROPIEDADES DE INTERFAZ:

public:
        //PROPIEDADES DINÁMICAS:

        //puntero al RP adscrito
        TRoboticPositioner *getRoboticPositioner(void) const {return __RoboticPositioner;}

        //punto objetivo al que debe ir el punto P3 del posicionador adscrito
        TDoublePoint TargetP3;

        //PROPIEDADES GRÁFICAS:

        //indica si el punto objetivo está seleccionado
        //valor por defecto: false
        bool Selected;

        //color del punto objetivo
        //color por defecto: clYellow
        //#QColor Color;

        //PROPIEDADES EN FORMATO TEXTO:

        //TargetP3 en formato texto
        AnsiString getTargetP3Text(void);
        void setTargetP3Text(const AnsiString&);

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //punto objetivo en formato texto
        AnsiString getText(void); void setText(const AnsiString&);

        //------------------------------------------------------------------
        //MÉTODOS ESTÁTICOS:

        //compara los identificadores de los posicionadores
        //adscritos de dos puntos objetivo
        static int  CompareIds(TTargetPoint *TPA1,
                TTargetPoint *TPA2);
        //este método debe ser apuntado en una lista de punteros
        //para permitir el funcionamiento de
        //los métodos de ordenación y comparación

        //obtiene las etiquetas de las propiedades
        //en formato fila de texto
        static AnsiString GetIdTargetP3LabelsRow(void);

        //atraviesa las etiquetas de las propiedades
        //en una cadena de texto a partir de la posición i
        static void  TravelLabels(const AnsiString& S, int& i);

        //lee los valores de las propiedades
        //en una cadena de texto a partir de la posición i
        static void  ReadSeparated(int& Id, double& x, double& y, const AnsiString& S, int& i);

        //MÉTODOS DE CONSTRUCCIÓN Y DESTRUCCIÓN:

        //construye un punto objetivo adscrito a un RP
        //si el RP ya tiene un punto objetivo contruido
        //lanza una excepción EImproperArgument
        TTargetPoint(TRoboticPositioner *_RoboticPositioner,
                double x, double y);
        TTargetPoint(TRoboticPositioner *_RoboticPositioner,
                TDoublePoint _TargetP3);

        //destruye un punto objetivo
        //si no hay ningún punto objetivo construido
        //lanza una excepción EImproperCall
        ~TTargetPoint();

        //MÉTODOS DE COMPROBACIÓN:

        //determina si el punto objetivo está fuera
        //del dominio del punto P3 de su posicioandor adscrito
        bool IsOutDomainP3(void);
        //determina si el punto objetivo queda dentro
        //del área de seguridad del punto P3  de su posicionador adscrito
        bool IsInSafeAreaP3(void);

        //MÉTODOS DE DESPLAZAMIENTO:

        //Asigna el punto P3o del posicionador adscrito al punto objetivo
        void SetP3o(void) {TargetP3 = getRoboticPositioner()->getActuator()->getP3o();}
        //asigna el punto P3 del posiciondor adscrito al punto objetivo
        void SetP3(void) {TargetP3 = getRoboticPositioner()->getActuator()->getArm()->getP3();}
        //randomiza el punto objetivo con distribución uniforme
        //en el dominio del punto P3 de su posicioandor adscrito
        void Randomize(void);

        //Hay tres modos de randomizar un punto en el dominio de un posicionador
        //    - randomizando las posiciones de sus ejes con distribución
        //      uniforme en sus dominios respectivos;
        //    - randomizando un punto en coordenadas polares respecto S1
        //      con distribución uniforme en el intervalo radial
        //      hasta que el punto esté dentro del dominio del posicionador;
        //    - randomizando un pnto en coordenadas cartesianas respecto S0
        //      en el intervalo cartesiano del dominio del posicionador
        //      hasta que dicho punto esté en el dominio del mimso.
        //Aquí se ha implementado esta última por ser la única que
        //realmente randomiza con distribución uniforme en el dominio.

        //asigna el punto objetivo al punto P3 de su posicionador adscrito
        //si el punto objetivo no está en el dominio de
        //su posicionador adscrito lanza EImpropercall
        void MoveToTargetP3(void);

        //NOTA: cuando la cuantificación de los oejes del posicionador
        //está desactivada, MoveToTargeTP3 moverá los ejes del posicionador
        //a las posicioes exactas para que P3 quede en la posición
        //del punto objetivo, pero cuando la cuantificación de alguno de
        //los ejes está activada, se aplicará la cuantificación primero
        //al eje 1 y despues al eje 2 en todo caso, sin comprobar cual es
        //el punto estable más próximo, ya que se considera que la diferencia
        //es despreciable.

        //MÉTODOS GRÁFICOS:

        //indica que parte del punto objetivo
        //puede ser agarrado en el punto indicado
        //      1: punto (TargetP3, 0.75)
        //      0: niguna;
        int Grab(TDoublePoint P);

        //imprime el puntos objetivo en el lienzo de la fotografía
        //de un trazador de formas
        //#void Paint(TPloterShapes *PS);
};

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
#endif // TARGETPOINT_H
