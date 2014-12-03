//---------------------------------------------------------------------------
//Archivo: FocalPlanePaintBox1.cpp
//Contenido: caja de pintura del plano focal
//Última actualización: 24/04/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "FocalPlanePaintBox1.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//-------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//construye un controlador de caja de pintura del plano focal
//adscrita a los objetos externos
TFocalPlanePaintBox1::TFocalPlanePaintBox1(TPloterShapes *_PS, TFiberMOSModel *_FMM,
        TRoboticPositionerFormulary *_RPF,
        TExclusionAreaFormulary *_EAF)
{
        //el puntero PloterSahpes debería apuntar a un trazador de formas contruido
        if(_PS == NULL)
                throw EImproperArgument("pointer PS should point to built ploter shapes");

        //el puntero FMM debería apuntar a un Fiber MOS Model construido
        if(_FMM == NULL)
                throw EImproperArgument("pointer FMM should point to built Fiber MOS Model");

        //el puntero RPF debería apuntar a un formulario de edición de RPs construido
        if(_RPF == NULL)
                throw EImproperArgument("pointer RPF should point to built RPs edition formulary");

        //el puntero EAF debería apuntar a un formulario de edición de EAs construido
        if(_EAF == NULL)
                throw EImproperArgument("pointer EAF should point to built EAs edition formulary");

        //APUNTA LOS OBJETOS EXTERNOS:

        __PloterShapes = _PS;
        __FiberMOSModel = _FMM;
        __RoboticPositionerFormulary = _RPF;
        __ExclusionAreaFormulary = _EAF;

        //INICIALIZA LAS PROPIEDADES DE ESTADO:

        //baja las banderas de presión del teclado
        __KeyCtrlDown = false;
        __KeyShiftDown = false;

        //baja las banderas de presión del ratón
        __ButtonLeftDown = false;
        __ButtonMiddleDown = false;
        __ButtonRightDown = false;

        //baja la bandera de arrastre
        __DraggingExclusionArea = false;
        __DraggingRoboticPositioner = false;
        __ItemIndex = 0;
        __PartIndex = 0;
}

//-------------------------------------------------------------------
//MÉTODOS DE IMPRESIÓN:

//imprime el instrumento en el lienzo
//de la fotografía del trazador de formas
void TFocalPlanePaintBox1::PaintInstrument(void)
{
        //determina los posicionadores en colisión
        TVector<int> indices;
        FiberMOSModel->RPL.SearchCollinding(indices);
        FiberMOSModel->RPL.SetCollisions(false);
        for(int i=0; i<indices.Count; i++)
                FiberMOSModel->RPL[indices[i]]->Actuator->Collision = true;

        //imprime el instrumento
        FiberMOSModel->Paint(PloterShapes);
}

//imprime las listas de objetos en el lienzo de la fotografía e
//imprime el lienzo de la fotografía en el lienzo de la caja de pintura
void TFocalPlanePaintBox1::Paint(void)
{
        //rellena el fondo del lienzo
        //de la fotografía del trazador de formas
        PloterShapes->FillBack();

        //dibuja los objetos del sistema en el lienzo
        //de la fotografía del trazador de formas:
        PaintInstrument();

        //imprime el lienzo de la fotografía en el lienzo de la caja de pintura
        PloterShapes->Draw();
}

//-------------------------------------------------------------------
//MÉTODOS DE REACCIÓN A EVENTOS DE UN POSICIONADOR DE FIBRA AGARRADO:

//ADVERTENCIA: como el método Paint se va a redefinir en clases sucesivas
//debe evitarse la invoación de dicho método en los siquientes métodos:

void TFocalPlanePaintBox1::MouseDownExclusionAreaGrab()
{
        //indica que está agarrando un área de exclusión
        __DraggingExclusionArea = true;

        //si la tecla Ctrl está presionada
        if(KeyCtrlDown) {
                //apunta el área de exclusión indicada para facilitar su acceso
                TExclusionArea *EA = FiberMOSModel->EAL[ItemIndex];
                //cambia el estado de selección del área de exclusión
                EA->Selected = !EA->Selected;
        }
}
void TFocalPlanePaintBox1::MouseDownRoboticPositionerGrab()
{
        //indica que está agarrando un robot posicionador
        __DraggingRoboticPositioner = true;

        //si la tecla Ctrl está presionada
        if(KeyCtrlDown) {
                //apunta el posicionador indicado para facilitar su acceso
                TRoboticPositioner *RP = FiberMOSModel->RPL[ItemIndex];
                //cambia el estado de selección del posicionador
                RP->Actuator->Selected = !RP->Actuator->Selected;
        }
}
void TFocalPlanePaintBox1::MouseMoveRoboticPositionerGrab()
{
        //apunta el posicionador agarrado para facilitar su acceso
        TRoboticPositioner *RP = FiberMOSModel->RPL[ItemIndex];
        //apunta elactuador del posicionador para facilitar su acceso
        TActuator *Actuator = RP->Actuator;
        //apunta el brazo del actuador para facilitar su acceso
        TArm *Arm = Actuator->Arm;

        //mueve la parte indicada del posicionador indicado
        switch(PartIndex) {
                case 3: //mueve ambos ejes
                        //obtiene los ángulos correspondientes
                        //a la nueva posición del cursor
                        double _theta_1, _theta___3;
                        if(!Actuator->AnglesToGoP3(_theta_1, _theta___3, Cact.x, Cact.y)) {
                                TDoublePoint C_act = Actuator->S0recToS1rec(Cact.x, Cact.y);
                                _theta_1 = ArgPos(C_act);
                                _theta___3 = M_PI;
                        }
                        //gira los ejes a las nuevas posiciones
                        Actuator->SetAnglesRadians(_theta_1, _theta___3);

                        break;
                case 2: //mueve el brazo en la dirección (P1, Cact)
                        if(Cact != Arm->P1) {
                        //determina el desplazamiento angular en pasos
                                double _deltap___3 = Arm->F.Image(Angle(Cpre - Arm->P1, Cact - Arm->P1));

                                //si está cuantificando el eje, el valor
                                //a comparar debe estar cuantificado
                                if(Arm->Quantify___)
                                        _deltap___3 = Near(_deltap___3);

                                if(_deltap___3 != 0) { //si la posición ha cambiado
                                        try {
                                                //mueve el brazo a la nueva posición angular
                                                Arm->p___3 = Arm->p___3 + _deltap___3;
                                                //actualiza la posición previa del cursor
                                                __Xpre = Xact;
                                                __Ypre = Yact;
                                                Cpre = Cact;
                                        } catch(Exception &E) {
                                                //nop hace nada
                                        }
                                }
                        }
                        break;
                case 1: //mueve el cilindro en la dirección (P0, Cact)
                        if(Cact != Actuator->P0) {
                                //determina el desplazamiento angular en pasos
                                double _deltap_1 = Actuator->F.Image(Angle(Cpre - Actuator->P0, Cact - Actuator->P0));

                        //si está cuanficando el eje, el valor
                        //a comparar debe estar cuantificado
                        if(Actuator->Quantify_)
                                _deltap_1 = Near(_deltap_1);

                                if(_deltap_1 != 0) { //si la posición ha cambiado
                                        try {
                                                //mueve el cilindro a la nueva posición angular
                                                Actuator->p_1 = Actuator->p_1 - _deltap_1;
                                                //actualiza la posición previa del cursor
                                                __Xpre = Xact;
                                                __Ypre = Yact;
                                                Cpre = Cact;
                                      } catch(Exception &E) {
                                                //no hace nada
                                      }
                                }
                        }
                        break;
        }
        //imprime las nuevas coordenadas angulares del posicionador
//        LabelX->Text = FloatToStr(RoboticPositioner->p_1);
  //      LabelY->Text = FloatToStr(Arm->p___3);
}

//-------------------------------------------------------------------
//MÉTODOS DE REACCIÓN A EVENTOS SOBRE LA CAJA DE PINTURA:

void TFocalPlanePaintBox1::MouseDown(TMouseButton Button, int _X, int _Y)
{
        //actualiza la posición del cursor
        __Xpre = __Xact = _X;
        __Ypre = __Yact = _Y;
        Cpre.x = Cact.x = PloterShapes->PixelsToUnitsX(Xact);
        Cpre.y = Cact.y = PloterShapes->PixelsToUnitsY(Yact);

        //captura las coordenadas iniciales del centro de la pantalla
        __xwini = PloterShapes->xw;
        __ywini = PloterShapes->yw;

        switch(Button) {
            case mbLeft: {
                //si hay alguna parte de algún posicionador bajo Cact
                if(FiberMOSModel->RPL.Grab(__ItemIndex, __PartIndex, Cact))
                        //indica que está agarrando un robot posicionador
                        //y si la tecla Ctrl está presionada
                        //cambia el estado de selección del posicionador
                        MouseDownRoboticPositionerGrab();
                //si no hay ninguna parte de ningún objeto bajo Cact
                else {
                        //indica que no está agarrando ningún objeto
                        __DraggingRoboticPositioner = false;

                        //si la tecla Ctrl está presionada
                        if(KeyCtrlDown)
                                //deselecciona todos los posicionadores
                                FiberMOSModel->RPL.DeselectAll();
                }

                //indica el nuevo estado del botón
                __ButtonLeftDown = true;

                } break;
            case mbMiddle:
                //indica el nuevo estado del botón
                __ButtonMiddleDown = true;
                break;
            case mbRight:
                //indica el nuevo estado del botón
                __ButtonRightDown = true;
                break;
        }

        //actualiza el lienzo
        Paint();
}

void TFocalPlanePaintBox1::MouseMove(int _X, int _Y)
{
        //indica la nueva posición del cursor
        __Xact = _X;
        __Yact = _Y;
        Cact.x = PloterShapes->PixelsToUnitsX(Xact);
        Cact.y = PloterShapes->PixelsToUnitsY(Yact);

        //si tiene el botón izquierdo apretado
        if(ButtonLeftDown) {
                if(DraggingRoboticPositioner) { //si está agarrando un robot posicionador
                        //mueve el posicionador indicado según la parte agarrada
                        MouseMoveRoboticPositionerGrab();
                        //actualiza el lienzo
                        Paint();
                }
        }
        //si tiene el botón derecho apretado
        else if(ButtonRightDown) {
                //desplaza el área de trazado según el desplazamiento del cursor
                PloterShapes->DisplacePloterArea(Xact-Xpre, Yact-Ypre);
                //actualiza la posición previa del cursor
                __Xpre = Xact;
                __Ypre = Yact;
                Cpre = Cact;
                //traza los objetos e imprime la imagen
                Paint();
        }
}

void TFocalPlanePaintBox1::MouseUp(TMouseButton Button, int _X, int _Y)
{
        switch (Button) {
                case mbLeft:
                        //indica el nuevo estado del botón
                        __ButtonLeftDown = false;
                        //suelta la parte agarrada en caso de haberla
                        __DraggingRoboticPositioner = false;
                        break;
                case mbMiddle:
                        //indica el nuevo estado del botón
                        __ButtonMiddleDown = false;
                        break;
                case mbRight:
                        //indica el nuevo estado del botón
                        __ButtonRightDown = false;
                        break;
        }

        //actualiza la posición del cursor
        __Xact = _X;
        __Yact = _Y;
}

//hace doble click sobre el PaintBox
void TFocalPlanePaintBox1::DblClick(void)
{
        //declara dos índices locales para enmascarar los índices del sistema
        int ItemIndex;
        int PartIndex;

        //si hay alguna parte de algún posicionador bajo Cact
        if(FiberMOSModel->RPL.Grab(ItemIndex, PartIndex, Cact))
                //si las teclas Ctrl y Shift no están presionadas
                if(KeyCtrlDown!=true && KeyShiftDown!=true) {
                        //apunta el posicionador indicado para facilitar su acceso
                        TRoboticPositioner *RP = FiberMOSModel->RPL[ItemIndex];
                        //edita el posicionador y si lo modifica
                        if(RoboticPositionerFormulary->Edit(RP))
                                //actualiza el lienzo
                                Paint();
                        return; //no hace nada más
                }

        //si no, si hay alguna parte de algún área de exclusión bajo Cact
        if(FiberMOSModel->EAL.Grab(ItemIndex, PartIndex, Cact))
                //si las teclas Ctrl y Shift no están presionadas
                if(KeyCtrlDown!=true && KeyShiftDown!=true) {
                        //apunta el área de exclusión indicada para facilitar su acceso
                        TExclusionArea *EA = FiberMOSModel->EAL[ItemIndex];
                        //edita el área de exclusión y si la modifica
                        if(ExclusionAreaFormulary->Edit(EA))
                                //actualiza el lienzo
                                Paint();
                        return; //no hace nada más
                }

        //ADVERTENCIA: en todo formulario, después de DblClick se invoca a
        //MouseDown seguido de MouseUp; pero si después de DblClick abre
        //un formulario en modo modal como aquí, solamente se invocará a MouseDown;
        //este defecto deberá ser corregido en el formulario correspondiente.
}

//---------------------------------------------------------------------------
//MÉTODOS DE REACCIÓN A EVENTOS SOBRE EL FORMULARIO:

void TFocalPlanePaintBox1::MouseWheel(int WheelDelta)
{
        //cambia el factor de escala
        if(WheelDelta > 0) //zoom in
                PloterShapes->scale *= 1.05;
        else //zoom out
                PloterShapes->scale /= 1.05;

        //traza las funciones e imprime la imagen
        Paint();
}
void TFocalPlanePaintBox1::Resize(void)
{
        //resimensiona el trazador de formas conforme a la caja de pintura
        PloterShapes->Resize();
        //traza las funciones e imprime la imagen
        Paint();

        //auqnue realmente no haría falta invocar a Paint() ya que
        //es invocado automáticamente cuando se redimensiona un formulario
}
void TFocalPlanePaintBox1::KeyDown(WORD &Key, TShiftState Shift)
{
        //si está presionado la tecla Ctrl
        if(Shift.Contains(ssCtrl))
                __KeyCtrlDown = true; //indica su nuevo estado

        //si está presionando la tecla Shift
        if(Shift.Contains(ssShift))
                __KeyShiftDown = true; //indica su nuevo estado
}
void TFocalPlanePaintBox1::KeyUp(WORD &Key, TShiftState Shift)
{
        //si no está presionando la tecla Ctrl
        if(!Shift.Contains(ssCtrl))
                __KeyCtrlDown = false; //indica su nuevo estado

        //si no está presionando la tecla Shift
        if(!Shift.Contains(ssShift))
                __KeyShiftDown = false; //indica su nuevo estado
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
