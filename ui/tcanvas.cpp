//---------------------------------------------------------------------------
//Archivo: tcanvas.cpp
//Contenido: clase lienzo
//Última actualización: 03/10/2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "tcanvas.h"
#include "../src/Exceptions.h"
#include "../src/Constants.h"
#include "../src/Vectors.h"
#include "../src/Scalars.h"

//---------------------------------------------------------------------------

using namespace Mathematics;

const QBrush TCanvas::getBrush(void) const
{
    return painter.brush();
}
const QPen TCanvas::getPen(void) const
{
    return painter.pen();
}
const QFont TCanvas::getFont(void) const
{
    return painter.font();
}
const QRect TCanvas::getClipRect(void) const
{
    return picture.boundingRect();
}

//lee el color de la brocha
QColor TCanvas::getBrushColor(void)
{
/*    //activa el painter adscribiéndolo a la picture
    painter.begin(&picture);
    //utiliza el painter
    QColor color = painter.brush().color();
    //desactiva el painter
    painter.end();

    return color; //devuelve el valor de lectura*/
    return __BrushColor;
}
//establece el color de la brocha
void TCanvas::setBrushColor(const QColor& color)
{
    //ADVERTENCIA: painter.brush() devuelve una brocha constante
    //de modo que no puede ser modificada.

    //contruye una brocha clon y le asigna el color
//    QBrush aux(painter.brush());
  //  aux.setColor(color);

/*    //activa el painter adscribiéndolo a la picture
    painter.begin(&picture);
    //utiliza el painter
    painter.setBrush(color);
    //desactiva el painter
    painter.end();*/
    __BrushColor = color;
}
//lee el color de la pluma
QColor TCanvas::getPenColor(void)
{
/*    //activa el painter adscribiéndolo a la picture
    painter.begin(&picture);
    //utiliza el painter
    QColor color = painter.pen().color();
    //desactiva el painter
    painter.end();

    return color; //devuelve el valor de lectura*/
    return __PenColor;
}
//establece el color de la pluma
void TCanvas::setPenColor(const QColor& color)
{
    //ADVERTENCIA: painter.pen() devuelve una pluma constante
    //de modo que no puede ser modificada.

    //contruye una brocha clon y le asigna el color
//    QPen aux(painter.pen());
  //  aux.setColor(color);

/*    //activa el painter adscribiéndolo a la picture
    painter.begin(&picture);
    //utiliza el painter
    painter.setPen(color);
    //desactiva el painter
    painter.end();*/
    __PenColor = color;
}

//---------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//construye un lienzo por defecto
TCanvas::TCanvas() :
    //contruye las propiedades
    picturebak(),
    picture(),
    painter()
{
    //inicializa las propiedades de configuración del painter
    painter.begin(&picture);
    __BrushColor = painter.brush().color();
    __PenColor = painter.pen().color();
    painter.end();
}
//destruye este lienzo
TCanvas::~TCanvas()
{
}

//inicializa el contenido del canvas
void TCanvas::Clear(void)
{
    painter.begin(&picture);
    painter.end();
    painter.begin(&picturebak);
    painter.end();
}

//determina la longitud de una cadena en pixels
int TCanvas::TextWidth(const AnsiString& S)
{
    //activa el painter adscribiéndolo a la picture
//    painter.begin(&picture);

    try {
        //intercambia el contenido de las pictures
        picture.swap(picturebak);
        //activa el painter adscribiéndolo a la picture
        painter.begin(&picture);
        //restaura el contenido de la picture
        painter.drawPicture(0, 0, picturebak);

        //Nótese que aunque solo va a medir las simensiones de un texto
        //el painter debe estar activo, y para que no se borre todo el contenido
        //deberá intercambiarse su contenido.

        //utiliza el painter
        QString QS(S.c_str()); //contruye una QString a partir de la AnsiString
        int aux = painter.fontMetrics().width(QS);

        //desactiva el painter realizando los dibujos
        painter.end();

        return aux; //devuelve el valor de lectura

    } catch(...) {
        throw;
    }
}
//determina la altura de una cadena en pixels
int TCanvas::TextHeight(void)
{
    //activa el painter adscribiéndolo a la picture
//    painter.begin(&picture);

    try {
        //intercambia el contenido de las pictures
        picture.swap(picturebak);
        //activa el painter adscribiéndolo a la picture
        painter.begin(&picture);
        //restaura el contenido de la picture
        painter.drawPicture(0, 0, picturebak);

        //Nótese que aunque solo va a medir las simensiones de un texto
        //el painter debe estar activo, y para que no se borre todo el contenido
        //deberá intercambiarse su contenido.

        //utiliza el painter
        int aux = painter.fontMetrics().height();

        //desactiva el painter realizando los dibujos
        painter.end();

        return aux; //devuelve el valor de lectura

    } catch(...) {
        throw;
    }
}

//imprime un texto en el lienzo
void TCanvas::TextOut(int X, int Y, const AnsiString& Text)
{
    try {
        //intercambia el contenido de las pictures
        picture.swap(picturebak);
        //activa el painter adscribiéndolo a la picture
        painter.begin(&picture);
        //restaura el contenido de la picture
        painter.drawPicture(0, 0, picturebak);

        //utiliza el painter
        painter.setBrush(__BrushColor);
        painter.setPen(__PenColor);
        QString QS(Text.c_str()); //contruye una QString a partir de la AnsiString
        painter.drawText(X, Y, QS);

        //desactiva el painter realizando los dibujos
        painter.end();

    } catch(...) {
        throw;
    }
}
//imprime un texto en el liezo según las propiedades
//este método no está en C++Builder
void TCanvas::drawText(const QRect & rectangle, int flags,
              const QString & text, QRect * boundingRect)
{
    try {
        //intercambia el contenido de las pictures
        picture.swap(picturebak);
        //activa el painter adscribiéndolo a la picture
        painter.begin(&picture);
        //restaura el contenido de la picture
        painter.drawPicture(0, 0, picturebak);

        //utiliza el painter
        painter.setBrush(__BrushColor);
        painter.setPen(__PenColor);
        painter.drawText(rectangle, flags, text, boundingRect);

        //desactiva el painter realizando los dibujos
        painter.end();

    } catch(...) {
        throw;
    }
}

//rellena un rectángulo con el color de la brocha
void TCanvas::FillRect(const QRect& r)
{
    try {
        //intercambia el contenido de las pictures
        picture.swap(picturebak);
        //activa el painter adscribiéndolo a la picture
        painter.begin(&picture);
        //restaura el contenido de la picture
        painter.drawPicture(0, 0, picturebak);

        //utiliza el painter
        painter.setBrush(__BrushColor);
        painter.setPen(__PenColor);
        painter.fillRect(r, __BrushColor);//painter.brush().color());

        //desactiva el painter realizando los dibujos
        painter.end();

    } catch(...) {
        throw;
    }
}
//dibuja un punto del color de la pluma
void TCanvas::Point(int x, int y)
{
    try {
        //intercambia el contenido de las pictures
        picture.swap(picturebak);
        //activa el painter adscribiéndolo a la picture
        painter.begin(&picture);
        //restaura el contenido de la picture
        painter.drawPicture(0, 0, picturebak);

        //utiliza el painter
        painter.setBrush(__BrushColor);
        painter.setPen(__PenColor);
        painter.drawPoint(x, y);

        //desactiva el painter realizando los dibujos
        painter.end();

    } catch(...) {
        throw;
    }
}
//dibuja un segmento del punto (x1, y1) a punto (x2, y2)
void TCanvas::Segment(int x1, int y1, int x2, int y2)
{
    try {
        //intercambia el contenido de las pictures
        picture.swap(picturebak);
        //activa el painter adscribiéndolo a la picture
        painter.begin(&picture);
        //restaura el contenido de la picture
        painter.drawPicture(0, 0, picturebak);

        //activa el painter adscribiéndolo a la picture
        //painter.begin(&picture);
        //utiliza el painter
        painter.setBrush(__BrushColor);
        painter.setPen(__PenColor);
        painter.drawLine(x1, y1, x2, y2);

        //desactiva el painter realizando los dibujos
        painter.end();

    } catch(...) {
        throw;
    }
}

//dibuja una elipse inscrita en el rectángulo (u, l, r, d)
void TCanvas::Ellipse(int u, int l, int r, int d)
{
    try {
        //intercambia el contenido de las pictures
        picture.swap(picturebak);
        //activa el painter adscribiéndolo a la picture
        painter.begin(&picture);
        //restaura el contenido de la picture
        painter.drawPicture(0, 0, picturebak);

        //utiliza el painter
        painter.setBrush(__BrushColor);
        painter.setPen(__PenColor);
        painter.drawEllipse(l, u, r-l, d-u);

        //desactiva el painter realizando los dibujos
        painter.end();

    } catch(...) {
        throw;
    }
}
//dibuja una polilínea
void TCanvas::Polyline(QPoint *ps, int count)
{
    try {
        //intercambia el contenido de las pictures
        picture.swap(picturebak);
        //activa el painter adscribiéndolo a la picture
        painter.begin(&picture);
        //restaura el contenido de la picture
        painter.drawPicture(0, 0, picturebak);

        //activa el painter adscribiéndolo a la picture
        //painter.begin(&picture);
        //utiliza el painter
        painter.setBrush(__BrushColor);
        painter.setPen(__PenColor);
        painter.drawPolyline(ps, count);

        //desactiva el painter realizando los dibujos
        painter.end();

    } catch(...) {
        throw;
    }
}
//Dibuja un arco inscrito en en rectángulo (x1, y1, x2, y2),
//con origen en en la semilinea 1 y fin en la semilinea 2.
//La semilinea 1 tiene origen en el centro del arco y pasa por (x3, y3).
//La semilinea 2 tiene origen en el centro del arco y pasa por (x4, y4).
void TCanvas::Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
    try {
        //intercambia el contenido de las pictures
        picture.swap(picturebak);
        //activa el painter adscribiéndolo a la picture
        painter.begin(&picture);
        //restaura el contenido de la picture
        painter.drawPicture(0, 0, picturebak);

        //traduce los parámetros
        //QRect rectangle(x1, y1, x2, y2);
        double xc = double(x1 + x2)/2.;
        double yc = double(y1 + y2)/2.;
        int a = Round(ArgPos(x3-xc, yc-y3)/M_PI*180.*16.);
        int alen;
        if(x3!=x4 || y3!=y4)
            alen = Round(AnglePos(TDoublePoint(x3-xc, yc-y3),
                                  TDoublePoint(x4-xc, yc-y4))/M_PI*180.*16.);
        else
            alen = 360*16;

        //utiliza el painter
        painter.setBrush(__BrushColor);
        painter.setPen(__PenColor);
        painter.drawArc(x1, y1, x2-x1, y2-y1, a, alen);
        //pinta el último punto
//        painter.drawPoint(x4, y4);

        //desactiva el painter realizando los dibujos
        painter.end();

    } catch(...) {
        throw;
    }
}

//dibuja un canvas en la posición indicada de este canvas
void TCanvas::Draw(int X, int Y, const TBitmap *Bitmap)
{
    //el puntero Bitmap debe indicar a un lienzo contruido
    if(Bitmap == NULL)
        throw EImproperArgument("pinter Bitmap should point to built Bitmap");

    try {
        //intercambia el contenido de las pictures
        picture.swap(picturebak);
        //activa el painter adscribiéndolo a la picture
        painter.begin(&picture);
        //restaura el contenido de la picture
        painter.drawPicture(0, 0, picturebak);

        //utiliza el painter
//        painter.setBrush(__BrushColor);
  //      painter.setPen(__PenColor);
    //    painter.setBackgroundMode(Qt::TransparentMode);
        painter.drawPicture(X, Y, Bitmap->getCanvas()->picture);
        //Bitmap->getCanvas()->picture.play(&painter);

        //desactiva el painter realizando los dibujos
        painter.end();

        //Otra opción para imprimir la picture en la Bitmap->getCanvas()->picture
        //es invocar el método play:
        //  Bitmap->getCanvas()->picture.play(&painter);

    } catch(...) {
        throw;
    }
}

//--------------------------------------------------------------------------
//TBitmap

//TCanvas *getCanvas(void) const {return __Canvas;}

int TBitmap::getWidth(void) const
{
    QRect r = __Canvas->picture.boundingRect();
    return r.width();
}
void TBitmap::setWidth(int _Width)
{
    try {
//        QRect r = picture.boundingRect();
  //      r.setWidth(_Width);
    //    picture.setBoundingRect();
        QRect rect = __Canvas->picture.boundingRect();
        rect.setWidth(_Width);
        __Canvas->picture.setBoundingRect(rect);

    } catch(...) {
        throw;
    }
}
int TBitmap::getHeight(void) const
{
    QRect r = __Canvas->picture.boundingRect();
    return r.height();
}
void TBitmap::setHeight(int _Height)
{
    try {
//        QRect r = picture.boundingRect();
  //      r.setHeight(_Height);
    //    picture.setBoundingRect();
        QRect rect = __Canvas->picture.boundingRect();
        rect.setHeight(_Height);
        __Canvas->picture.setBoundingRect(rect);

    } catch(...) {
        throw;
    }
}

//Construye un mapa de bits por defecto
TBitmap::TBitmap(void)
{
    __Canvas = new TCanvas;
}

//destruye este mapa de bits
TBitmap::~TBitmap()
{
    delete __Canvas;
}

//--------------------------------------------------------------------------
//TPicture

//TBitmap *getBitmap(void) const {return __Bitmap;}

//Construye una fotografía por defecto
TPicture::TPicture(void)
{
    __Bitmap = new TBitmap;
}

//destruye esta fotografía
TPicture::~TPicture()
{
    delete __Bitmap;
}

