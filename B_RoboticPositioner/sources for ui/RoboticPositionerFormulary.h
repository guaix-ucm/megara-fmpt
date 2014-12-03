//---------------------------------------------------------------------------
//Archivo: RoboticPositionerFormulary.h
//Contenido: formulario de edición de posicionadores
//Última actualización: 12/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ROBOTICPOSITIONERFORMULARY_H
#define ROBOTICPOSITIONERFORMULARY_H

#include "RoboticPositioner.h"
//#include "..\7_MotionFunctions\ComposedMotionFunctionFormulary.h"

#include <QDialog>

//---------------------------------------------------------------------------

using namespace Models;

//---------------------------------------------------------------------------

namespace Ui {
class TRoboticPositionerFormulary;
}

class TRoboticPositionerFormulary : public QDialog
{
    Q_OBJECT

private:
    TRoboticPositioner *__RoboticPositioner;

//    TComposedMotionFunctionFormulary *__CMFF;

public:
    //puntero al último RP externo
    //valor por defecto: NULL
    TRoboticPositioner *getRoboticPositioner(void) const {
        return __RoboticPositioner;}

    //formulario de edición de funciones de movimiento compuestas
//    TComposedMotionFunctionFormulary *getCMFF(void) const {
  //      return __CMFF;}

    //construye un formulario de edición de RPs
    explicit TRoboticPositionerFormulary(QWidget *parent = 0);
    //destruye un formulatrio de edición de RPs
    ~TRoboticPositionerFormulary();

    //edita un RP
    //valores devueltos:
    //      true: indica que se ha modificado
    //      falso: indica que no se ha modificado
    bool Edit(TRoboticPositioner *);

private:
    Ui::TRoboticPositionerFormulary *ui;
};

#endif // ROBOTICPOSITIONERFORMULARY_H
