//---------------------------------------------------------------------------
//Archivo: tcanvas.h
//Contenido: clase lienzo
//Última actualización: 03/10/2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef TCANVAS_H
#define TCANVAS_H

#include "../src/vclemu.h" //AnsiString

#include <QPicture>
#include <QPainter>
#include <QRect>

//---------------------------------------------------------------------------

class TBitmap;

//clase fotografía
class TCanvas
{
    QColor __BrushColor;
    QColor __PenColor;

    //picture donde guardar lo ya dibujado
    QPicture picturebak;

public:
    //picture conteniendo la imagen
    QPicture picture;
    //painter adscrito a la picture
    QPainter painter;

    //ADVERTENCIA: para que los cambios surtan efecto en la picture,
    //el painter solo debe estar activo durante el intervalo durante
    //el cual se realizan los cambios. Al activar el painter la picture
    //es borrada y la configuración del painter reseteada.

    const QBrush getBrush(void) const;
    const QPen getPen(void) const;
    const QFont getFont(void) const;
    const QRect getClipRect(void) const;

    //Estos métodos no conviene incluirlos,
    //porque para leer los atributos el painter
    //debe estar activo.

    //ADVERTENCIA: incluso para la lectura de atributos,
    //el painter debe estar activo.

    //ADVERTENCIA: QFont no tiene una propiedad para el color.
    //El painter dibujará el texto con el color del pen.

    //Para cambiar los colores del painter deben usarse los métodos:
    //  painter->setPen(Qt::blue);
    //  painter->setBrush(Qt::red);
    //pero para ello el painter deberá estar activo.

    //lee el color de la brocha
    QColor getBrushColor(void);
    //establece el color de la brocha
    void setBrushColor(const QColor&);
    //lee el color de la pluma
    QColor getPenColor(void);
    //establece el color de la pluma
    void setPenColor(const QColor&);

    //El color de la fuente coincide con el de la pluma.

    //ADVERTENCIA: para establecer los colores el painter debe estar actrivo,
    //razón por la cual debería realizarse mediante los métodos anteriores.

    //MÉTODOS PÚBLICOS:

    //construye un lienzo por defecto
    TCanvas();
    //destruye este lienzo
    ~TCanvas();

    //inicializa el contenido del canvas
    void Clear(void);

    //determina la longitud de una cadena en pixels
    int TextWidth(const AnsiString&);
    //determina la altura de una cadena en pixels
    int TextHeight(void);

    //Parea poder activar el canvas, estas funciones no
    //deben ser const.

    //ADVERTENCIA: incluir QtQuick/QQuickWindow provoca que
    //TextOut sea una palabra reservada, de modo que el método
    //TCanvas::TextOut producirá un error.

    //imprime un texto en el lienzo en la posición (X, Y)
    void TextOut(int X, int Y, const AnsiString& Text);
    //imprime un texto en el liezo según las propiedades
    //este método no está en C++Builder
    void drawText(const QRect & rectangle, int flags,
                  const QString & text, QRect * boundingRect = 0);
    //rellena un rectángulo con el color de la brocha
    void FillRect(const QRect&);
    //dibuja un punto del color de la pluma
    void Point(int x, int y);
    //dibuja un segmento del punto (x1, y1) a punto (x2, y2)
    void Segment(int x1, int y1, int x2, int y2);
    //dibuja una elipse inscrita en el rectángulo (u, l, r, d)
    void Ellipse(int u, int l, int r, int d);
    //dibuja una polilínea
    void Polyline(QPoint *ps, int count);
    //Dibuja un arco inscrito en en rectángulo (x1, y1, x2, y2),
    //con origen en en la semilinea 1 y fin en la semilinea 2.
    //La semilinea 1 tiene origen en el centro del arco y pasa por (x3, y3).
    //La semilinea 2 tiene origen en el centro del arco y pasa por (x4, y4).
    void Arc(int x1, int y1, int x2, int y2,
             int x3, int y3, int x4, int y4);

    //dibuja un canvas en la posición indicada de este canvas
    void Draw(int X, int Y, const TBitmap*);
};

//clase mapa de bits
class TBitmap {
    TCanvas *__Canvas;
public:
    TCanvas *getCanvas(void) const {return __Canvas;}

    QColor getTransparentColor(void) const {return Qt::black;}
    int getWidth(void) const; void setWidth(int);
    int getHeight(void) const; void setHeight(int);

    //Construye un mapa de bits por defecto
    TBitmap(void);
    //destruye este mapa de bits
    ~TBitmap();
};

//clase fotografía
class TPicture {
    TBitmap *__Bitmap;
public:
    TBitmap *getBitmap(void) const {return __Bitmap;}

    //Construye una fotografía por defecto
    TPicture(void);
    //destruye esta fotografía
    ~TPicture();

    //guarda la imagen en un archivo
    void SaveToFile(const AnsiString&) {/*completar*/;}
};

//---------------------------------------------------------------------------
#endif // TCANVAS_H
