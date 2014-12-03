//---------------------------------------------------------------------------
//Archivo: FocalPlanePaintBox1.h
//Contenido: caja de pintura del plano focal
//Última actualización: 08/07/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FOCALPLANEPAINTBOX1_H
#define FOCALPLANEPAINTBOX1_H

#include "FiberMOSModel2.h"
//#include "..\B_ExclusionArea\ExclusionAreaFormulary.h"
//#include "..\B_RoboticPositioner\RoboticPositionerFormulary.h"
#include "..\5_Graphics\PloterShapes.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------

//TListTargetPointsPaintBox define una clase de objeto de interfaz intermediario
//entre una caja de pintura del plano focal y los siguientes objetos
//del sistema:
//      ListRoboticPositioners

//controlador de la caja de pintura del plano focal
class TFocalPlanePaintBox1 {
protected:
        //OBJETOS EXTERNOS ADSCRITOS:

        TPloterShapes *__PloterShapes;

        TFiberMOSModel *__FiberMOSModel;

        TRoboticPositionerFormulary *__RoboticPositionerFormulary;
        TExclusionAreaFormulary *__ExclusionAreaFormulary;

        //PROPIEDADES DE ESTADO:

        //estado de los botones del teclado
        bool __KeyCtrlDown;
        bool __KeyShiftDown;

        //estado de los botones del ratón
        bool __ButtonLeftDown;
        bool __ButtonMiddleDown;
        bool __ButtonRightDown;

        //arrastrado de una parte de un objeto
        bool __DraggingExclusionArea;
        bool __DraggingRoboticPositioner;
        int __ItemIndex;
        int __PartIndex;

        //coordenadas del cusor
        //tras realizar el último movimiento del posicionador
        int __Xpre;
        int __Ypre;
        TDoublePoint Cpre; //en unidades

        //coordenadas del cusor tras realizar
        //el último movimiento del ratón
        int __Xact;
        int __Yact;
        TDoublePoint Cact; //en unidades

        //posición del centro de la pantalla
        //al presionar uno de los botones del ratón
        double __xwini;
        double __ywini;

        //etiquetas de coordenadas
//        TMobileString *__LabelX, *__LabelY;

public:
        //OBJETOS EXTERNOS ADSCRITOS:

        //trazador de formas externo adscrito a
        //la caja de pintura del plano focal
        TPloterShapes *getPloterShapes(void) const {return __PloterShapes;}

        //Fiber MOS Model externo adscrito
        TFiberMOSModel *getFiberMOSModel(void) const {return __FiberMOSModel;}

        //formulario de edición de RPs externo adscrito
        TRoboticPositionerFormulary *getRoboticPositionerFormulary(void) const {
            return __RoboticPositionerFormulary;}
        //formulario de edición de áreas de exclusión externo adscrito
        TExclusionAreaFormulary *getExclusionAreaFormulary(void) const {
            return __ExclusionAreaFormulary;}

        //PROPIEDADES RELACIONADAS CON EVENTOS:

        //estado de la tecla Ctrl
        bool getKeyCtrlDown(void) const {return __KeyCtrlDown;}
        //estado de la tecla Shift
        bool KeyShiftDown(void) const {return __KeyShiftDown;}

        //estado del botón izquierdo del ratón
        bool ButtonLeftDown(void) const {return __ButtonLeftDown;}
        //estado del botón central del ratón
        bool ButtonMiddleDown(void) const {return __ButtonMiddleDown;}
        //estado del botón derecho del ratón
        bool ButtonRightDown(void) const {return __ButtonRightDown;}

        //indica si está arrastrando una parte de un áreade exclusión
        //valor por defecto: false
        bool DraggingExclusionArea(void) const {
            return __DraggingExclusionArea;}
        //indica si está arrastrando una parte de un posicionador
        //valor por defecto: false
        bool DraggingRoboticPositioner(void) const {
            return __DraggingRoboticPositioner;}
        //indica el objeto cuya parte se está arrastrando
        //dominio: [0, ListTargetPoints->Count-1]
        //valor por defecto: 0
        int ItemIndex(void) const {return __ItemIndex;}
        //indica la parte del objeto que se está arrastrando:
        //      0: niguna;
        //      1: cilindro (P0, L1)
        //      2: brazo (PA..PJ)
        //      3: lente (P2, R2)
        //valor por defecto: 0
        int getPartIndex(void) const {return __PartIndex;}

        //coordenadas del cusor
        //tras realizar el último movimiento del objeto
        //dominio Xpre: [0, PloterSahpes->Width-1] pixels;
        //dominio Ypre: [0, PloterSahpes->Height-1] pixels;
        int getXpre(void) const {return __Xpre;}
        int getYpre(void) const {return __Ypre;}

        //coordenadas del cusor tras realizar el último movimiento del ratón
        //dominio Xpre: [0, PloterSahpes->Width-1] pixels;
        //dominio Ypre: [0, PloterSahpes->Height-1] pixels;
        int getXact(void) const {return __Xact;}
        int getYact(void) const {return __Yact;}

        //posición del centro de la pantalla
        //al presionar uno de los botones del ratón
        double getxwini(void) const {return __xwini;}
        double getywini(void) const {return __ywini;}

        //etiquetas de coordenadas
//        __property TMobileString *LabelX = {read=__LabelX};
  //      __property TMobileString *LabelY = {read=__LabelY};

        //MÉTODOS PÚBLICOS:

        //construye un controlador de caja de pintura del plano focal
        //adscrito a los objetos externos
        TFocalPlanePaintBox1(TPloterShapes *_PS, TFiberMOSModel *_FMM,
                TRoboticPositionerFormulary *_RPF,
                TExclusionAreaFormulary *_EAF);

        //MÉTODOS DE IMPRESIÓN:

        //dibuja el instrumento en el lienzo
        //de la fotografía del trazador de formas
        void PaintInstrument(void);

        //dibuja los puntos objetivo en el lienzo
        //de la fotografía del trazador de formas
        void PaintTargetPoints(void);

        //dinbuja las listas de objetos en el lienzo de la fotografía e
        //imprime el lienzo de la fotografía en el lienzo de la caja de pintura
        void Paint(void);

        //MÉTODOS DE REACCIÓN A EVENTOS DE UN POSICIONADOR DE FIBRA AGARRADO:

        void MouseDownExclusionAreaGrab(void);
        void MouseDownRoboticPositionerGrab(void);
        void MouseMoveRoboticPositionerGrab(void);

        //MÉTODOS DE REACCIÓN A EVENTOS SOBRE LA CAJA DE PINTURA:

        void MouseDown(TMouseButton Button, int _X, int _Y);
        void MouseMove(int _X, int _Y);
        void MouseUp(TMouseButton Button, int _X, int _Y);
        void DblClick(void);

        //MÉTODOS DE REACCIÓN A EVENTOS SOBRE EL FORMULARIO:

        void MouseWheel(int WheelDelta);
        void Resize(void);
        void KeyDown(WORD &Key, TShiftState Shift);
        void KeyUp(WORD &Key, TShiftState Shift);

        //ADVERTENCIA: en todo formulario, después de DblClick se invoca a
        //MouseDown seguido de MouseUp; pero si en el método de respuesta
        //a DblClick, abre un formulario en modo modal, a continuación
        //solo se ejecutará MouseDown; este error debe ser corregido
        //en el formulario correspondiente.
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // FOCALPLANEPAINTBOX1_H
