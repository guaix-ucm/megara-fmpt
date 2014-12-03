//---------------------------------------------------------------------------
//Archivo: tpaintbox.h
//Contenido: clase caja de pintura
//Última actualización: 03/10/2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef TPAINTBOX_H
#define TPAINTBOX_H

#include "tcanvas.h"

#include <QWidget>
#include <QPicture>

//---------------------------------------------------------------------------

//clase caja de pintura
class TPaintBox : public QWidget
{
    Q_OBJECT

private:
    TCanvas *__Canvas;

public:
    //lienzo intermediario paara poder dibujar
    //desde el exterior de esta clase
    TCanvas *getCanvas(void) const {return __Canvas;}

    //Como PaintBox deriva de Widget, dispone de los métodos with y height.

    //construye una caja de pintura por defecto
    explicit TPaintBox(QWidget *parent = 0);
    //destruye una caja de pintura
    ~TPaintBox();

    //redimensiona la picture según esta PaintBox
    void assimilateRect(void);

signals:
    //presiona un botón del ratón
    void mousePressEvent(QMouseEvent *event);
    //mueve el ratón
    void mouseMoveEvent(QMouseEvent *event);
    //suelta un botón del ratón
    void mouseReleaseEvent(QMouseEvent *event);
    //hace doble clic con el botón izquierdo del ratón
    void mouseDoubleClickEvent(QMouseEvent *event);
    //mueve la rueda del ratón
    void wheelEvent(QWheelEvent * event);

public slots:
    //dibuja el contenido del lienzo sobre la caja de pintura
    void paintEvent(QPaintEvent *);
    //redimensiona el lienzo para que coincida con la caja de pintura
    void resizeEvent(QResizeEvent *);

    //El método paintEvent será automáticamente incluido
    //en la lista de signals de la PaintBox.
};

//---------------------------------------------------------------------------
#endif // TPAINTBOX_H
