//---------------------------------------------------------------------------
//Archivo: tshowmessage.cpp
//Contenido: clase cuadro de diálogo para mostrar un mensaje
//Última actualización: 03/10/2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "tshowmessage.h"
#include "ui_tshowmessage.h"

//---------------------------------------------------------------------------

TShowMessage::TShowMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TShowMessage)
{
    ui->setupUi(this);
}

TShowMessage::~TShowMessage()
{
    delete ui;
}

//imprime el mensaje en la etiqueta y abre el formulario
void TShowMessage::showMessage(const QString& QS)
{
    ui->label->setText(QS);
    show();
}

void TShowMessage::on_buttonBox_accepted()
{
}
