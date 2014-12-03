//---------------------------------------------------------------------------
//Archivo: tshowmessage.h
//Contenido: clase cuadro de diálogo para mostrar un mensaje
//Última actualización: 03/10/2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

//---------------------------------------------------------------------------

namespace Ui {
class TShowMessage;
}

class TShowMessage : public QDialog
{
    Q_OBJECT

public:
    explicit TShowMessage(QWidget *parent = 0);
    ~TShowMessage();

    //imprime el mensaje en la etiqueta y abre el formulario
    void showMessage(const QString& QS);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::TShowMessage *ui;
};

//---------------------------------------------------------------------------
#endif // DIALOG_H
