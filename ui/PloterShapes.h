//---------------------------------------------------------------------------
//Archivo: PloterShapes.cpp
//Contenido: trazador gráfico de formas
//Última actualización: 09/07/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef PLOTERSHAPES_H
#define PLOTERSHAPES_H

//#include "../src/Polypoint.h"
#include "../src/Vectors.h"

#include <QPicture>
#include <QColor>

//ADVERTENCIA: incluir QtQuick/QQuickWindow provoca que
//TextOut sea una palabra reservada, de modo que el método
//TCanvas::TextOut producirá un error.

//---------------------------------------------------------------------------

using namespace Mathematics;

//espacio de nombres de clases de objetos gráficos
namespace Graphics {

//---------------------------------------------------------------------------
//TPloterShapes
//---------------------------------------------------------------------------

//clase: trazador de formas
class TPloterShapes {
    //DIMENSIONES DE LA PANTALLA:

    double __scrr;
    double __scrd_inch;

    double __scrw_inch;
    double __scrh_inch;

    double __scrw;
    double __scrh;

    //FOTOGRAFÍA DEL TRAZADOR:

    QPicture *__Picture;

    QColor __BackColor;

    //PROPIEDADES IMPOSITIVAS:

    double __xw;
    double __yw;

    double __scale;

    //PROPIEDADES DERIVADAS:

    double __winw, __winh;
    double __X1, __Y1, __X2, __Y2;
    int __LAPx, __LAPy;
    double __LAUx, __LAUy;
    double __PBUx, __PBUy;

    //MÉTODOS DE ASIMILACIÓN:

    //calcula los parámetros de las funciones de conversión
    //a partir de (Width, Height) y (xw, yw)
    void Procesate(void);

public:
    //DIMENSIONES DE LA PANTALLA:

    //relación de aspecto de la pantalla base:altura
    //valor por defecto: double(Screen->Width)/double(Screen->Height)
    double getscrr(void) const {return __scrr;}
    void setscrr(double);
    //diagonal de la pantalla en pulgadas
    //      scrd_inch = sqrt(scrh_inch^2 + scrw_inch^2)
    double getscrd_inch(void) const {return __scrd_inch;}
    void setscrd_inch(double);

    //La relación de aspecto de pantallas panorámicas es de 1.6
    //y la resolución máxima suele ser 1680*1050 pix.

    //Dimensiones de la pantalla del monitor HP 1750:
    //      scrd: 17 in;
    //      scrr: 5/4;
    //      resolución: 1280*1050 pix;
    //      distancia entre pix: 0.264 (según especif. tec. del fabricante).

    //Interesa que la relación de aspecto corresponda a la altura/anchura
    //y no al revés:
    //      1680/1050 = 1.6
    //      1050/1680 = 0.625
    //      1280/1050 = 1.2190476190476190...
    //      1050/1280 = 0.8203125
    //aunque en realidad no es importante.

    //anchura de la pantalla en pulgadas
    //      scrw_inch = scrh_inch*scrr
    double getscrw_inch(void) const {return __scrw_inch;}
    //altura de la pantalla en pulgadas
    //valor por def: double(Screen->Height)/double(Screen->PixelsPerUnits)
    double getscrh_inch(void) const {return __scrh_inch;}

    //anchura de la pantalla en unidades
    //      scrw = scrw_inch*25.4
    double getscrw(void) const {return __scrw;}
    //altura de la pantalla en unidades
    //      scrh = scrh_inch*25.4
    double getscrh(void) const {return __scrh;}

    //FOTOGGRAFÍA DEL TRAZADOR:

    //fotografía donde se trazarán las figuras
    QPicture *getPicture(void) const {return __Picture;}

    //El lienzo de la fotografía es apuntado mediante un puntero en
    //el momento de contruir la fotografía con objeto de mejorar
    //su tiempo de acceso.
/*
    //color de la brocha del lienzo de la fotografía
    //valor por defecto: el de la fotografía
    QColor getBrushColor(void) {return __PictureCanvas->getBrushColor();}
    void setBrushColor(QColor);
    //color de la pluma del lienzo de la fotografía
    //valor por defecto: el de la fotografía
    QColor getPenColor(void) {return __PictureCanvas->getPenColor();}
    void setPenColor(QColor);
    //color del texto del lienzo de la fotografía
    //valor por defecto: el de la fotografía
    QColor getFontColor(void) {return __PictureCanvas->painter.pen().color();}
    void setFontColor(QColor);
    //color transparente de la fotografía
    //valor por defecto: el de la fotografía
    QColor getTransparentColor(void) {return
                __Picture->getBitmap()->getTransparentColor();}

    //color de fondo que es asignado a BrushColor en FillBack
    //valor por defecto: Picture->Bitmap->TransparentColor
    QColor getBackColor(void) const {return __BackColor;}
    void setBackColor(QColor _BackColor) {__BackColor=_BackColor;}
*/
    //El color de fondo conviene que sea una propiedad de la clase
    //ya que es inicializado al color transparente y el lienzo
    //es rellenado del color de fondo cada vez que es redimensionado
    //incluida la primera vez en el constructor.

    //El color transparente corresponde a (R, G, B) = (255, 255, 255)
    //siendo el más luminoso que hay, pero si se rellena el fondo de
    //un lienzo de ese color se mostrará negro.

    //la luminancia del color de fundo será utilizada para determinar
    //el coor de resalte:
    //      Luminance(BackColor)<255*1.5 => PenColor=clWhite
    //      Luminance(BackColor)>=255*1.5 => PenColor=clBlack
    //Es por ello que el color transparente no debería ser usado
    //normalmente como fondo, a menos que se quiera obtener una imagen
    //con transparencias.

    //PROPIEDADES IMPOSITIVAS:

    //las dimensiones de la pantalla en pixels se encuentran en
    //el objeto global Screen: (Screen->Width, Screen->Height)

    //dimensiones de la ventana en pixels
    int getWidth(void) {
        QRect r = getPicture()->boundingRect();
        return r.width();
    }
    void setWidth(int);
    int getHeight(void) {
        QRect r = getPicture()->boundingRect();
        return r.height();
    }
    void setHeight(int);

    //coordenadas del centro de la ventana en unidades
    double getxw(void) const {return __xw;} void setxw(double);
    double getyw(void) const {return __yw;} void setyw(double);

    //factor de escala correspondiente a:
    //(unidades en la pantalla)/(unidades en el sistema)
    //valor por defecto: 1
    double getscale(void) const {return __scale;} void setscale(double);

    //PROPIEDADES DERIVADAS:

    //dimensiones de la ventana en cm
    double getwinw(void) const {return __winw;}
    double getwinh(void) const {return __winh;}

    //longitud del eje x en pixels
    int getLAPx(void) const {return __LAPx;}
    //longitud del eje y en pixels
    int getLAPy(void) const {return __LAPy;}

    //límites del intervalo de trazado en cm
    double getX1(void) const {return __X1;}
    double getY1(void) const {return __Y1;}
    double getX2(void) const {return __X2;}
    double getY2(void) const {return __Y2;}

    //longitud de los ejes en cm
    double getLAUx(void) const {return __LAUx;}
    double getLAUy(void) const {return __LAUy;}

    //pixels por unidad
    double getPBUx(void) const {return __PBUx;}
    double getPBUy(void) const {return __PBUy;}

    //MÉTODOS PÚBLICOS:

    //construye un trazador adscrito a la caja de pintura indicada
    //centrado en el origen de coordenadas
    TPloterShapes(void);
    //libera la memoria dinámica
    ~TPloterShapes();

    //redimensiona el lienzo de la fotografía del trazador
    void SetSize(int _Width, int _Height);

    //ADVERTENCIA: cuando cambia las dimensiones
    //partes del lienzo pueden ser borradas

    //NOTA: podría redimensionarse el lienzo de la fottografía
    //del trazador de formas haciendo:
    //      PloterShapes.PaintBox->SetBounds(0, 0, Width, Height);
    //      PloterSahpes.Resize();
    //Pero cuando la caja de pintura no se va a usar es preferible
    //redimensionarlo directamente mediante:
    //      PloterShapes.SetSize(Width, Height);

    //MÉTODOS DE TRANSFORMACIÓN DE COORDENADAS:

    //traduce de unidades del sistema a pixels del lienzo
    double UnitsToPixelsX(double x) {return (x - __X1)*__PBUx;}
    double UnitsToPixelsY(double y) {return (__Y2 - y)*__PBUy;}
    //traduce de pixels del lienzo a unidades del sistema
    double PixelsToUnitsX(double X) {return X/__PBUx + __X1;}
    double PixelsToUnitsY(double Y) {return __Y2 - Y/__PBUy;}

    //NOTA: las dimensiones en pixels de cara a la transformación de
    //coordenadas son (Width - 1)x(Height - 1) porque se mide desde
    //el centro del primer pixel hasta el centro del último pixel

    //ADVERTENCIA: la función TCanvas::Ellipse(l, u, r, d)
    //requiere que r y d sean una unidad mayor, ya que dibuja
    //una elipse en el intervalo [l, r)x[u, d)

    //MÉTODOS DE TRAZADO DE FORMAS:

/*    //rellena el fondo con BackColor
    void FillBack(void);*/
    //dibuja un punto
    void Point(TDoublePoint, QPainter& painter);
/*    //dibuja un círculo centrado en C de radio R
    void Circle(TDoublePoint C, double R);
    //dibuja una polilínea
    void Polygon(TPolypoint *P);*/
    //dibuja un segmento
    void Segment(TDoublePoint P1, TDoublePoint P2,
                 QPainter& painter);
    //dibuja el arco de la circunferencia (Pc, R)
    //limitado en sentido levógiro por los lados
    //(Pc, Pini) y (Pc, Pfin)
    void Arc(TDoublePoint Pfin, TDoublePoint Pini,
             TDoublePoint Pc, double R,
             QPainter& painter);

    //dibuja una circunferencia centrada en C de radio R
    void Circunference(TDoublePoint C, double R, QPainter& painter);

    //imprime una cadena de texto centrada en P
//    void Print(TDoublePoint P, const AnsiString& Text);

    //POSIBLE MEJORA: esta clase no tiene método Paint,
    //ya que ello requeriría alistar las instrucciones de trazado
    //de modo que cuando se cambia algún parámetro hay
    //que volver a dibuajarlo todo.
    //Un tipo de instrucción podría ser la orden de trazado de una
    //función apuntada por un puntero, de modo que esta clase sería
    //un trazador genérico para toda clase de objetos incluidas funciones.

    //mueve el área de trazado a la posición (xw, yw)
    void MovePloterArea(double _xw, double _yw);
    //desplaza el área de trazado (SX, SY) pixels
    void DisplacePloterArea(double SX, double SY);
    //redimensiona el lienzo de la fotografía
    //conforme a las dimensiones del lienzo de la caja de pintura
//    void Resize(void);
    //ADVERTENCIA: cambiar las dimensiones del lienzo de la fotografía
    //puede provocar la pérdida de información de la misma.
    //RECORDATORIO: el método Paint es invocado automáticamente
    //cuando se redimensiona el formulario.

    //imprime el lienzo de la fotografía en el lienzo externo adscrito
    void Draw(void);
};

//---------------------------------------------------------------------------

} //Graphics

//---------------------------------------------------------------------------
#endif // PLOTERSHAPES_H
