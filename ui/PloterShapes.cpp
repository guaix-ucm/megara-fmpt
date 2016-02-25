//---------------------------------------------------------------------------
//Archivo: PloterShapes.cpp
//Contenido: trazador gráfico de formas
//Última actualización: 26/07/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "PloterShapes.h"
//#include "../src/vclemu.h"
#include "../src/Scalars.h"
//#include "../src/StrSimbolic.h"
#include "../src/Exceptions.h"
#include <QPainter>

//---------------------------------------------------------------------------

//using namespace Strings;
using namespace Mathematics;

//espacio de nombres de clases de objetos gráficos
namespace Graphics {

//---------------------------------------------------------------------------
//TPloterShapes
//---------------------------------------------------------------------------

//DIMENSIONES DE LA PANTALLA:

void TPloterShapes::setscrr(double _scrr)
{
    //debe ser mayor que cero
    if(_scrr <= 0)
        throw EImproperArgument("scrr should be upper zero");

    __scrr = _scrr;
    Procesate(); //calcula las propiedades dependientes
}

void TPloterShapes::setscrd_inch(double _scrd_inch)
{
    //debe ser mayor que cero
    if(_scrd_inch <= 0)
        throw EImproperArgument("scrd_inch should be upper zero");

    __scrd_inch = _scrd_inch;
    Procesate(); //calcula las propiedades dependientes
}

/*//FOTOGRAFÍA DEL TRAZADOR:

void TPloterShapes::setBrushColor(QColor _BrushColor)
{
    __PictureCanvas->setBrushColor(_BrushColor);
}
void TPloterShapes::setPenColor(QColor _PenColor)
{
    __PictureCanvas->setPenColor(_PenColor);
}
void TPloterShapes::setFontColor(QColor _FontColor)
{
    __PictureCanvas->setPenColor(_FontColor);

    //El color de la fuente coincide con el de la pluma.
}
*/
//PROPIEDADES IMPOSITIVAS:

void TPloterShapes::setWidth(int _Width)
{
    //la anchura del lienzo debe ser almenos de un pixel
    if(_Width < 1)
        throw EImproperArgument("Width shuld be almos one pixel");

    //cambia y procesa las propiedades
//    __Picture->getBitmap()->setWidth(_Width); //cambia el ancho de la fotografía
    QRect rect = getPicture()->boundingRect();
    rect.setWidth(_Width);
    getPicture()->setBoundingRect(rect);
    Procesate(); //calcula las propiedades dependientes
}
void TPloterShapes::setHeight(int _Height)
{
    //la anchura del lienzo debe ser almenos de un pixel
    if(_Height < 1)
        throw EImproperArgument("Height should be almos one pixel");

    //cambia y procesa las propiedades
//    __Picture->getBitmap()->setHeight(_Height);
    QRect rect = getPicture()->boundingRect();
    rect.setHeight(_Height);
    getPicture()->setBoundingRect(rect);
    Procesate(); //calcula las propiedades dependientes
}
void TPloterShapes::setxw(double _xw)
{
    __xw = _xw; //asigna el nuevo valor
    Procesate(); //calcula las propiedades dependientes
}
void TPloterShapes::setyw(double _yw)
{
    __yw = _yw; //asigna el nuevo valor
    Procesate(); //calcula las propiedades dependientes
}

void TPloterShapes::setscale(double _scale)
{
    //debe ser un valor distinto de cero
    if(_scale == 0)
        throw EImproperArgument("scale should be unequal zero");

    __scale = _scale; //asigna el nuevo valor
    Procesate(); //calcula las propiedades dependientes
}

//MÉTODOS DE ASIMILACIÓN:

//calcula los parámetros de las funciones de conversión
//a partir de (Width, Height) y (xw, yw)
void TPloterShapes::Procesate(void)
{
    //calcula las dimensiones de la pantalla en pulgadas
    //        __scrw_inch = scrh_inch*scrr;
    //        __scrd_inch = sqrt(scrh_inch*scrh_inch + scrw_inch*scrw_inch);

    //calcula las dimensiones de la pantalla en pulgadas
    __scrh_inch = __scrd_inch/sqrt(1 + (__scrr*__scrr));
    //        __scrw_inch = scrd_inch/sqrt(1 + 1/(scrr*scrr));
    __scrw_inch = __scrh_inch*__scrr;

    //calcula las dimensiones e la pantalla en unidades
    __scrw = __scrw_inch*25.4;
    __scrh = __scrh_inch*25.4;

    //al dividir la diagonal en pulgadas entre el número de mm
    //de una pulgada se está eligiendo el mm como unidad

    //calcula la lungitud de los ejes en pixels
    __LAPx = getWidth() - 1;
    __LAPy = getHeight() - 1;

    //        import QtQuick.Window 2.1;
    //calcula las dimensiones de la ventana en unidades

    __winw = __LAPx/double(/*Screen.Width*/640)*__scrw;
    __winh = __LAPy/double(/*Screen.Height*/480)*__scrh;

    //calcula los límites del intervalo de trazado en unidades
    double deltax = __winw/2./__scale;
    __X1 = __xw - deltax;
    __X2 = __xw + deltax;
    double deltay = __winh/2./__scale;
    __Y1 = __yw - deltay;
    __Y2 = __yw + deltay;

    //calcula la lungitud de los ejes en undiades
    __LAUx = __X2 - __X1; //SumDouble(__X2, -__X1);
    __LAUy = __Y2 - __Y1; //SumDouble(__Y2, -__Y1);

    //calcula los factores de conversión
    __PBUx = double(__LAPx)/__LAUx;
    __PBUy = double(__LAPy)/__LAUy;
}

//---------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//construye un trazador adscrito a la caja de pintura indicada
//centrado en el origen de coordenadas
TPloterShapes::TPloterShapes(void)
{
    //inicializa la relación de aspecto y el tamaño de la pantalla
    __scrr = double(/*Screen.Width*/640)/double(/*Screen.Height*/480);
    __scrw_inch = double(/*Screen.Width*/640)/double(/*Screen.PixelsPerInch*/120);
    __scrh_inch = double(/*Screen.Height*/480)/double(/*Screen.PixelsPerInch*/120);
    __scrd_inch = sqrt(__scrw_inch*__scrw_inch + __scrh_inch*__scrh_inch);

    //construye la fotografía
    __Picture = new QPicture;
    //inicia el color de fondo a transparente
//    __BackColor = getTransparentColor();

    //prepara el color de fondo para el autollenado al redimensionar
//    __Picture->getBitmap()->getCanvas()->setBrushColor(__BackColor);

    //dimensiona el lienzo de la fotografía
    //conforme al lienzo de la caja de pintura
//    __Picture->getBitmap()->setWidth(__PaintBox->width());
  //  __Picture->getBitmap()->setHeight(__PaintBox->height());

    //inicializa las coordenadas del centro
    //de la ventana a su valor por defecto
    __xw = 0;
    __yw = 0;

    //inicializa el factor de escala a su valor por defecto
    __scale = 1;

    //calcula las propiedades dependientes
    Procesate();
}

//libera la memoria dinámica
TPloterShapes::~TPloterShapes()
{
    delete __Picture;
}

//redimensiona el lienzo de la fotografía del trazador
void TPloterShapes::SetSize(int _Width, int _Height)
{
    //el lienzo debe contener almenos un punto
    if(_Width <= 0)
        throw EImproperArgument("Width should be uppe zero");
    if(_Height <= 0)
        throw EImproperArgument("Height should be uppe zero");

    //cambia y procesa las propiedades
    QRect rect = getPicture()->boundingRect();
    rect.setWidth(_Width);
    rect.setHeight(_Height);
    getPicture()->setBoundingRect(rect);
    Procesate(); //calcula las propiedades dependientes
}

/*//rellena el fondo con BackColor
void TPloterShapes::FillBack(void)
{
    //pinta el fondo
    __PictureCanvas->setBrushColor(__BackColor);
    __PictureCanvas->FillRect(__PictureCanvas->getClipRect());
}*/
//dibuja un punto
void TPloterShapes::Point(TDoublePoint P, QPainter& painter)
{
    int X = Round(UnitsToPixelsX(P.x));
    int Y = Round(UnitsToPixelsY(P.y));
    //__PictureCanvas->Point(X, Y); //pinta el último punto
    QPoint QP;
    QP.setX(X);
    QP.setY(Y);
    painter.drawPoint(QP);
}
/*//dibuja un círculo centrado en C d eradio R
void TPloterShapes::Circle(TDoublePoint C, double R)
{
    int u = Round(UnitsToPixelsY(C.y + R));
    int l = Round(UnitsToPixelsX(C.x - R));
    int r = Round(UnitsToPixelsX(C.x + R)) + 1;
    int d = Round(UnitsToPixelsY(C.y - R)) + 1;

    setBrushColor(getPenColor());
    __PictureCanvas->Ellipse(u, l, r, d);
    //        __PictureCanvas->FloodFill(Round(UnitsToPixelsX(C.x)), Round(UnitsToPixelsY(C.y)), BrushColor, fsSurface);

    //ADVERTENCIA:
    //la función Ellipse requiere las coordenadas
    //(right, down) incrementadas en un pixel
    //y solo empezará a dibujar algo a partir de una de las
    //coordenadas esté incrementada en un pixel
    //y la otra esté incrementada en dos pixels
    //no dibujará nada cuando l==r+1 y u==d+1
}
//dibuja un polilígono
void TPloterShapes::Polygon(TPolypoint *P)
{
    //no debe apuntar a nulo
    if(P == NULL)
        throw EImproperArgument("null pointer P");

    //construye un vector con suficientes puntos para el polígono
    QPoint *ps = new QPoint[P->getCount() + 1];

    //transcribe las coordenadas de los vértices del polipunto
    for(int i=0; i<P->getCount(); i++) {
        ps[i].setX(Round(UnitsToPixelsX(P->Get(i).x)));
        ps[i].setY(Round(UnitsToPixelsY(P->Get(i).y)));
    }
    //cierra el polígono
    ps[4].setX(ps[0].x());
    ps[4].setY(ps[0].y());

    //dibuja el polígono
    __PictureCanvas->Polyline(ps, P->getCount());

    //libera los objetos dinámicos
    delete ps;
}*/
//dibuja un segmento
void TPloterShapes::Segment(TDoublePoint P1, TDoublePoint P2,
                            QPainter& painter)
{
    int X1 = Round(UnitsToPixelsX(P1.x));
    int Y1 = Round(UnitsToPixelsY(P1.y));
    int X2 = Round(UnitsToPixelsX(P2.x));
    int Y2 = Round(UnitsToPixelsY(P2.y));

    QPoint p1, p2;
    p1.setX(X1);
    p1.setY(Y1);
    p2.setX(X2);
    p2.setY(Y2);
    QLine QL;
    QL.setP1(p1);
    QL.setP2(p2);
    painter.drawLine(QL);
}

//dibuja el arco de la circunferencia (Pc, R)
//limitado en sentido levógiro por los lados
//(Pc, Pini) y (Pc, Pfin)
void TPloterShapes::Arc(TDoublePoint Pfin, TDoublePoint Pini,
                        TDoublePoint Pc, double R,
                        QPainter& painter)
{
    //Los puntos (X1, Y1) y (X2, Y2) delimitan
    //el rectángulo que contien una elipse.

    int X1 = Round(UnitsToPixelsX(Pc.x - R));
    int Y1 = Round(UnitsToPixelsY(Pc.y + R));
    int X2 = Round(UnitsToPixelsX(Pc.x + R)) + 1;
    int Y2 = Round(UnitsToPixelsY(Pc.y - R)) + 1;

    //normaliza Pini y Pfin
    TDoublePoint Pini_ = (Pini - Pc);
    Pini_ = Pc + Pini_/Mod(Pini_)*R;
    TDoublePoint Pfin_ = (Pfin - Pc);
    Pfin_ = Pc + Pfin_/Mod(Pfin_)*R;

    //El arco es dibujado en sentido levógiro desde un punto inicial
    //hasta un punto final siguiendo el contorno de la elipse.
    //El punto inicial se calcula como la intersección con
    //el contorno de la elipse del eje que va desde el centro de la elipse
    //hasta el punto (X3, Y3).
    //El punto final se calcula como la intersección con
    //el contorno de la elipse del eje que va desde el centro de la elipse
    //hasta el punto (X4, Y4).

    //calcula los puntos (X3, Y3) y (X4, Y4)
    int X3 = Round(UnitsToPixelsX(Pini_.x));
    int Y3 = Round(UnitsToPixelsY(Pini_.y));
    int X4 = Round(UnitsToPixelsX(Pfin_.x));
    int Y4 = Round(UnitsToPixelsY(Pfin_.y));

    //La función Arc es algo defectuosa,
    //pudiéndose pintar pixeles previos al punto origen
    //y dejarse de pintar el último pixel del punto final.

    //Por otra parte ha de haber al menos una separación
    //de un pixel entre (X3, Y3) y (X4, Y4), de lo contrario
    //se dibujará el círculo complementario, adoptándose
    //(X3, Y3) como el punto inicial.

    //si el marco entre p1 y p2, tiene un pixel central
    if((X1 + X2)%2!=1 && (Y1 + Y2)%2!=1) {
        //calcula el punto central
        int XC = (X1 + X2)/2;
        int YC = (Y1 + Y2)/2;
        //si elpunto (X3, Y3) coincide con la coordenada del pixel central
        if(X3==XC && Y3==YC)
            //no dibuja la elipse
            return;
        //si elpunto (X4, Y4) coincide con la coordenada del pixel central
        if(X4==XC && Y4==YC)
            //no dibuja la elipse
            return;
    }

    //cuando (X3, Y3)==(X4, Y4), solo debe intentar pintar el arco
    //cuando el ángulo es mayor que llano
    if(X3!=X4 || Y3!=Y4 || AnglePos(Pini-Pc, Pfin-Pc)>M_PI) {
        //traduce los parámetros
        double xc = double(X1 + X2)/2.;
        double yc = double(Y1 + Y2)/2.;
        int a = Round(ArgPos(X3-xc, yc-Y3)/M_PI*180.*16.);
        int alen;
        if(X3!=X4 || Y3!=Y4)
            alen = Round(AnglePos(TDoublePoint(X3-xc, yc-Y3),
                                  TDoublePoint(X4-xc, yc-Y4))/M_PI*180.*16.);
        else
            alen = 360*16;

        //utiliza el painter
        painter.drawArc(X1, Y1, X2-X1, Y2-Y1, a, alen);
    }

    //para poder pintar el último punto del arco debe haber normalizado Pfin
}

//dibuja una circunferencia centrada en C de radio R
void TPloterShapes::Circunference(TDoublePoint C, double R, QPainter& painter)
{
    int X1 = Round(UnitsToPixelsX(C.x - R));
    int Y1 = Round(UnitsToPixelsY(C.y + R));
    int X2 = Round(UnitsToPixelsX(C.x + R)) + 1;
    int Y2 = Round(UnitsToPixelsY(C.y - R)) + 1;

    int X3 = Round(UnitsToPixelsX(C.x));
    int Y3 = Y2;
    int X4 = X3;
    int Y4 = Y3;

    //traza el arco
    //__PictureCanvas->Arc(X1, Y1, X2, Y2, X3, Y3, X4, Y4);
    try {
        //traduce los parámetros
        //QRect rectangle(x1, y1, x2, y2);
        double xc = double(X1 + X2)/2.;
        double yc = double(Y1 + Y2)/2.;
        int a = Round(ArgPos(X3-xc, yc-Y3)/M_PI*180.*16.);
        int alen;
        if(X3!=X4 || X3!=X4)
            alen = Round(AnglePos(TDoublePoint(X3-xc, yc-Y3),
                                  TDoublePoint(X4-xc, yc-Y4))/M_PI*180.*16.);
        else
            alen = 360*16;

        //utiliza el painter
        //painter.setBrush(__BrushColor);
        //painter.setPen(__PenColor);
        painter.drawArc(X1, Y1, X2-X1, Y2-Y1, a, alen);
        //pinta el último punto
//        painter.drawPoint(x4, y4);

    } catch(...) {
        throw;
    }
}
/*
//imprime una cadena de texto centrada en P
void TPloterShapes::Print(TDoublePoint P, const AnsiString& Text)
{
    //determina las coordenadas del lienzo en píxeles donde debe ser impresa
    double x = UnitsToPixelsX(P.x);
    double y = UnitsToPixelsY(P.y);

    //determina las dimesiones del texto
    int Width = __PictureCanvas->TextWidth(Text);
    int Height = __PictureCanvas->TextHeight(**//*Text*//**);

    //determina las coordenadas para que el texto quede centrado
    int X = Round(x - double(Width)/2);
    int Y = Round(y - double(Height)/2);

    //imprime el texto
   __PictureCanvas->TextOut(X, Y, Text);
}
*/
//mueve el área de trazado a la posición (xw, yw)
void TPloterShapes::MovePloterArea(double _xw, double _yw)
{
    __xw = _xw; //asigna el nuevo valor
    __yw = _yw; //asigna el nuevo valor
    Procesate(); //calcula las propiedades dependientes
}
//desplaza el área de trazado (SX, SY) pixels
void TPloterShapes::DisplacePloterArea(double SX, double SY)
{
    //traduce el desplazamiento a unidades y lo añade al centro
    __xw -= SX/__PBUx;
    __yw += SY/__PBUy;
    Procesate(); //calcula las propiedades dependientes
}
/*//redimensiona el lienzo de la fotografía
//conforme a las dimensiones del lienzo de la caja de pintura
void TPloterShapes::Resize(void)
{
    //redimensiona el lienzo de la fotografía
//    SetSize(__PaintBox->width(), __PaintBox->height());
}*/
/*//imprime el lienzo de la fotografía en el lienzo externo adscrito
void TPloterShapes::Draw(void)
{
    //para que Draw copie pixel a pixel debe estar en modo ScrCopy
    //        PaintBoxCanvas->CopyMode = cmSrcCopy;
    //realiza la impresión a partir del primer pixel del lienzo
    __PaintBoxCanvas->Draw(0, 0, __Picture->getBitmap());
}*/

//---------------------------------------------------------------------------

} //Graphics

//---------------------------------------------------------------------------

