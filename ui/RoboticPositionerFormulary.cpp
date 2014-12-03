//---------------------------------------------------------------------------
//Archivo: RoboticPositionerFormulary.cpp
//Contenido: formulario de edición de posicionadores
//Última actualización: 12/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "RoboticPositionerFormulary.h"
#include "ui_RoboticPositionerFormulary.h"
#include "..\2_Strings\Strings.h"

//---------------------------------------------------------------------------
//TRoboticPositioner

//construye un formulario de edición de RPs
TRoboticPositionerFormulary::TRoboticPositionerFormulary(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TRoboticPositionerFormulary)
{
    ui->setupUi(this);
}

//destruye un formulatrio de edición de RPs
TRoboticPositionerFormulary::~TRoboticPositionerFormulary()
{
    delete ui;
}

//edita un posicinador
//valores devueltos:
//      true: indica que se ha modificado
//      falso: indica que no se ha modificado
bool TRoboticPositionerFormulary::Edit(TRoboticPositioner *_RoboticPositioner)
{
        //el puntero RoboticPositioner no debe ser nulo
        if(_RoboticPositioner == NULL)
                throw EImproperArgument("pointer RoboticPositioner should not be null");

        //apunta el posicionador para permitir su acceso
        __RoboticPositioner = _RoboticPositioner;

        //-------------------------------------------------------------------

        ui->textEdit->clear();
        ui->textEdit->append(getRoboticPositioner()->getAllText().c_str());

        ui->textEdit_2->clear();
        ui->textEdit_2->append(getRoboticPositioner()->getActuator()->getCilinderText().c_str());
/*        Memo3->Clear();
        Memo3->Lines->Add(RoboticPositioner->Actuator->F.TableText);
        Memo4->Clear();
        Memo4->Lines->Add(RoboticPositioner->Actuator->G.TableText);

        Memo5->Clear();
        Memo5->Lines->Add(RoboticPositioner->Actuator->Arm->AllText);
        Memo6->Clear();
        Memo6->Lines->Add(RoboticPositioner->Actuator->Arm->Contour____.ColumnText);
        Memo7->Clear();
        Memo7->Lines->Add(RoboticPositioner->Actuator->Arm->F.TableText);
        Memo8->Clear();
        Memo8->Lines->Add(RoboticPositioner->Actuator->Arm->G.TableText);
        Memo9->Clear();
        Memo9->Lines->Add(RoboticPositioner->Actuator->Arm->Contour.ColumnText);

        Memo10->Clear();
        Memo10->Lines->Add(RoboticPositioner->Actuator->Barrier->AllText);
        Memo11->Clear();
        Memo11->Lines->Add(RoboticPositioner->Actuator->Barrier->Contour_.ColumnText);
        Memo12->Clear();
        Memo12->Lines->Add(RoboticPositioner->Actuator->Barrier->Contour.ColumnText);

        Memo13->Clear();
        Memo13->Lines->Add(RoboticPositioner->Actuator->OthersText);

        Memo14->Clear();
        Memo14->Lines->Add(RoboticPositioner->CMF.AllText);
        Memo15->Clear();
        Memo15->Lines->Add(RoboticPositioner->CMF.SF1->Text);
        Memo16->Clear();
        Memo16->Lines->Add(RoboticPositioner->CMF.RF1->Text);
        Memo17->Clear();
        Memo17->Lines->Add(RoboticPositioner->CMF.SF2->Text);
        Memo18->Clear();
        Memo18->Lines->Add(RoboticPositioner->CMF.RF2->Text);

        //-------------------------------------------------------------------
*/
        //abre el formulario en modo modal
        exec();

        //indica que no se ha modificado
        return false;
}

/*void __fastcall TRoboticPositionerFormulary::FormShow(TObject *Sender)
{
}
*/
