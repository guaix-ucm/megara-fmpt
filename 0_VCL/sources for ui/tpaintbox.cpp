//---------------------------------------------------------------------------
//Archivo: tpaintbox.cpp
//Contenido: clase caja de pintura
//Última actualización: 03/10/2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "tpaintbox.h"

#include <QPainter>

//---------------------------------------------------------------------------

TPaintBox::TPaintBox(QWidget *parent) :
    QWidget(parent)
{
    //redimensionar el Canvas según esta PaintBox no surte efecto aquí,
    //porque todavia no ha sido construido

    __Canvas = new TCanvas();
}

TPaintBox::~TPaintBox()
{
    delete __Canvas;
}

//redimensiona el lienzo según esta caja de pintura
void TPaintBox::assimilateRect(void)
{
    //redimensiona el lienzo según esta caja de pintura
    __Canvas->picture.setBoundingRect(rect());
}

//dibuja el contenido del lienzo sobre la caja de pintura
void TPaintBox::paintEvent(QPaintEvent *)
{
    //construye un painter adscrito a esta caja de pintura
    QPainter painter(this);
    //dibuja el lienzo en la caja de pintura
    painter.drawPicture(0, 0, __Canvas->picture);
    //desactiva el painter
    painter.end();
}
//redimensiona el lienzo para que coincida con la caja de pintura
void TPaintBox::resizeEvent(QResizeEvent *)
{
    //redimensiona el lienzo según esta caja de pintura
    assimilateRect();

    //Aquí no debe invocarse al método paintEvent.
}
