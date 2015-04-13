// Copyright (c) 2014-2015 Isaac Morales Durán. All rights reserved.
// Institute of Astrophysics of Andalusia, IAA-CSIC
//
// This file is part of FMPT (Fiber MOS Positioning Tools)
//
// FMPT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//---------------------------------------------------------------------------
//Archivo: Figure.h
//Contenido: clases de figuras geométricas
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FIGURE_H
#define FIGURE_H

//##include "..\5_Graphics\PloterShapes.h"
#include "Vectors.h"
#include "vclemu.h"

//##include <QColor>

//---------------------------------------------------------------------------

//#using namespace Graphics;
using namespace Mathematics;

//espacio de nombres de modelos
namespace Models {

//RECUERDE: la eterogeneidad en las formas de representación de las diversas
//figuras geométricas no recomienda la creación de una jerarquía,
//salvo en circunstancias como esta, donde se pretende alistar
//las figuras y unificar la interfaz de su tratamiento.

//--------------------------------------------------------------------------
//TFigure
//--------------------------------------------------------------------------

//predefine las clases derivadas de TFigure
//class TPoint;
class TCircle;
class TCircunference;

//predefine las clases derivadas de TContourFigure
class TSegment;
class TArc;

//Un objeto de la clase TContourFigure:
//- solo puede ser un segmento o un arco;
//- permite acceder a los vértices Pa y Pb de segmentos y arcos
//  mediante un puntero a TContourFigure.

//clase abstracta figura geométrica
class TFigure {
protected:
public:
        //PROPIEDADES ESTÁTICAS:

        //etiqueta de la propiedad Color
        //valor por defecto: "Color"
        static AnsiString ColorLabel;

        //PROPIEDADES GRÁFICAS:

        //color de la figura
        //valor por defecto: clWhite
        //#QColor Color;

        //PROPIEDADES GRÁFICAS EN MODO TEXTO:

        //color del círculo
        AnsiString getColorText(void) const;
        void setColorText(const AnsiString&);

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjunto de propiedades de definición
        //en formato texto entre paréntesis
        virtual AnsiString getText(void) const = 0;
        virtual void setText(const AnsiString&) = 0;
        //conjunto de propiedades de definición
        //en formato línea de texto
        virtual AnsiString getRowText(void) const = 0;
        virtual void setRowText(const AnsiString&) = 0;

        //MÉTODOS ESTÁTICOS DE COMPARACIÓN:

        //compara el color de dos figuras
        //      si F1->Color < F2->Color entonces CompareColor = -1
        //      si F1->Color > F2->Color entonces CompareColor = 1
        //      si F1->Color == F2->Color entonces CompareColor = 0
        int  CompareColor(const TFigure *F1, const TFigure *F2);

        //MÉTODOS ESTÁTICOS DE IMPRESIÓN:

        //imprime un objeto en una cadena de texto
        //en formato texto entre paréntesis
        static void  FigurePrint(AnsiString& S, TFigure *F);
        //imprime un objeto en una cadena de texto
        //en formato fila de texto
        static void  FigurePrintRow(AnsiString& S, TFigure *F);

        //MÉTODOS ESTÁTICOS DE LECTURA:

        //lee un objeto en una cadena
        //en formato texto entre paréntesis
        static void  FigureRead(TFigure* &F,
                const AnsiString& S, int &i);
        //lee un objeto en una cadena
        //en formato fila de texto
        static void  FigureReadRow(TFigure* &F,
                const AnsiString& S, int &i);

        //ADVERTENCIA: si hay una figura descrita en la posición i
        //de lacadena S, el método FigureRead o FigureReadRow
        //construirá la figura del tipo correspondiente.
        //En caso contrario F e i permanecerán inalterados.

        //AVERTENCIA: TFigureRead solo distinguirá entre tres tipos de figuras:
        //      Circunferencias
        //      Segmentos
        //      Arcos
        //Ya que los círculos se describen igual que las circunferencias y no
        //es posible distinguirlos.

        //MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACIÓN:

        //RECUERDE: en el constructor de una clase abstracta
        //no se puden invocar a los métodos virtuales de la clase,
        //de modo que los métodos de asimilación de una clase abstracta
        //no pueden ser virtuales.

        //inicializa la propiedad Color
        //a su valor por defecto
        TFigure(void) /*#: Color(Qt::white) */{;}
        //destruye una figura
        virtual ~TFigure() = 0;

        //El destructor virtual es necesario para evitar la ambiguedad
        //al destruir objetos de clases derivadas.

        //MÉTODOS DE CÁLCULO DE DISTANCIAS:

        //calcula la distancia mínima entre la figura y un punto
        virtual double DistancePoint(TDoublePoint P) = 0;
        //calcula la distancia máxima entre la figura y un punto
        virtual double DistancePointMax(TDoublePoint P) = 0;

        //ADVERTENCIA: TDoublePoint no es una clase derivada de TFigure.

        //calcula la distancia mínima entre la figura y un circulo
        virtual double DistanceCircle(TCircle *C) = 0;
        //calcula la distancia mínima entre la figura y una circunferencia
        virtual double DistanceCircunference(TCircunference *C) = 0;

        //calcula la distancia mínima entre la figura y un segmento
        virtual double DistanceSegment(TSegment *S) = 0;
        //calcula la distancia mínima entre la figura y un arco
        virtual double DistanceArc(TArc *A) = 0;

        //calcula la distancia mínima entre la figura y otra figura
        virtual double Distance(TFigure *F) = 0;

        //MÉTODOS DE ROTACIÓN Y TRASLACIÓN:

        //obtiene la figutra geométrica rotada theta radianes
        //Figure debe contener una figura geométrica del mismo tipo
        virtual void GetRotated(TFigure *F, double theta) = 0;
        //traslada la figura geométrica según el vector V
        virtual void Translate(TDoublePoint V) = 0;

        //MÉTODOS GRÁFICOS:

        //dibuja una figura
        //en un trazador de formas
        //#virtual void Paint(TPloterShapes *) = 0;
};

//clase círculo
class TCircle : public TFigure {
protected:
        //PROPIEDADES DE DEFINICIÓN:

        double __R;

public:
        //PROPIEDADES ESTÁTICAS:

        //etiqueta de la propiedad R
        //valor por defecto: "R"
        static AnsiString RLabel;

        //PROPIEDADES DE DEFINICIÓN:

        //centro del círculo
        //valor por defecto: (0, 0)
        TDoublePoint P;
        //radio del círculo
        //debe ser mayor que cero
        //valor por defecto: 1
        double getR(void) const {return __R;} void setR(double);

        //PROPIEDADES DEDEFINICIÓN EN FORMATO TEXTO:

        //radio del círculo
        AnsiString getRText(void) const; void setRText(const AnsiString&);

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjunto de propiedades de definición
        //en formato texto entre paréntesis
        AnsiString getText(void) const; void setText(const AnsiString&);
        //conjunto de propiedades de definición
        //en formato fila de texto
        AnsiString getRowText(void) const; void setRowText(const AnsiString&);
        //conjunto de propiedades de definición
        //en formato asignaciones
        AnsiString getAssignsText(void) const;

        //FUNCIONES ESTÁTICAS PARA OBTENER LAS ETIQUETAS
        //DE LAS PROPIEDADES EN FORMATO TEXTO:

        //obtiene las etiquetas de las propiedades de definición
        //en formato texto entre paréntesis
        static AnsiString GetLabels(void);
        //obtiene las etiquetas de las propiedades de definición
        //en formato línea de texto
        static AnsiString GetRowLabels(void);

        //MÉTODOS ESTÁTICOS DE COMPARACIÓN:

        //compara la propiedad P.x de dos figuras
        static int  Comparex(const TCircle*, const TCircle*);
        //compara la propiedad P.Y de dos figuras
        static int  Comparey(const TCircle*, const TCircle*);
        //compara la propiedad R de dos figuras
        static int  CompareR(const TCircle*, const TCircle*);

        //MÉTODOS ESTÁTICOS DE IMPRESIÓN:

        //imprime las propiedades de un objeto en una cadena de texto
        //en formato texto entre paréntesis
        static void  Print(AnsiString&, const TCircle*);
        //imprime las propiedades de un objeto en una cadena de texto
        //en formato línea de texto
        static void  PrintRow(AnsiString&, const TCircle*);
        //imprime las propiedades de un objeto en una cadena de texto
        //en formato de asignaciones
        static void  PrintAssigns(AnsiString&, const TCircle*);

        //MÉTODOS ESTÁTICOS DE LECTURA:

        //lee las propiedades de un objeto en una cadena de texto
        //en formato texto entre paréntesis
        static void  Read(TCircle*, const AnsiString&, int&);
        //lee las propiedades de un objeto en una cadena de texto
        //en formato línea de texto
        static void  ReadRow(TCircle*, const AnsiString&, int&);

        //MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACIÓN:

        //contruye un objeto
        TCircle(void);
        TCircle(double x, double y, double _R);
        TCircle(TDoublePoint _P, double _R);

        //copia las propiedades de un objeto
        void Copy(TCircle*);
        TCircle& operator=(const TCircle&);

        //contruye un clon de un objeto
        TCircle(TCircle*);

        //MÉTODOS DE CONFIGURACIÓN:

        //asigna conjuntamente los valores de las propiedades de definición
        void SetValues(TDoublePoint _P, double _R);

        //MÉTODOS DE CÁLCULO DE DISTANCIAS:

        //calcula la distancia mínima entre la figura y un punto
        double DistancePoint(TDoublePoint);
        //calcula la distancia máxima entre la figura y un punto
        double DistancePointMax(TDoublePoint);

        //ADVERTENCIA: TDoublePoint no es una clase derivada de TFigure.

        //calcula la distancia mínima entre la figura y una circulo
        double DistanceCircle(TCircle*);
        //calcula la distancia mínima entre la figura y una circunferencia
        double DistanceCircunference(TCircunference*);

        //calcula la distancia mínima entre la figura y un segmento
        double DistanceSegment(TSegment*);
        //calcula la distancia mínima entre la figura y un arco
        double DistanceArc(TArc*);

        //calcula la distancia mínima entre la figura y otra figura
        double Distance(TFigure*);

        //MÉTODOS DE ROTACIÓN Y TRASLACIÓN:

        //obtiene la figutra geométrica rotada theta radianes
        //Figure debe contener una figura geométrica del mismo tipo
        void GetRotated(TFigure *F, double theta);
        //traslada la figura geométrica según el vector V
        void Translate(TDoublePoint V);

        //MÉTODOS GRÁFICOS:

        //dibuja el círculo
        //en un trazador de formas
        //#void Paint(TPloterShapes*);
};

//clase círcunferencia
class TCircunference : public TFigure {
protected:
        //PROPIEDADES DE DEFINICIÓN:

        double __R;

public:
        //PROPIEDADES ESTÁTICAS:

        //etiqueta de la propiedad R
        //valor por defecto: "R"
        static AnsiString RLabel;

        //PROPIEDADES DE DEFINICIÓN:

        //centro de la circunferencia
        //valor por defecto: (0, 0)
        TDoublePoint P;
        //radio de la circunferencia
        //debe ser mayor que cero
        //valor por defecto: 1
        double getR(void) const {return __R;} void setR(double);

        //PROPIEDADES DEDEFINICIÓN EN FORMATO TEXTO:

        //radio de la circunferencia
        AnsiString getRText(void) const; void setRText(const AnsiString&);

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjunto de propiedades de definición
        //en formato texto entre paréntesis
        AnsiString getText(void) const; void setText(const AnsiString&);
        //conjunto de propiedades de definición
        //en formato fila de texto
        AnsiString getRowText(void) const; void setRowText(const AnsiString&);
        //conjunto de propiedades de definición
        //en formato asignaciones
        AnsiString getAssignsText(void) const;

        //FUNCIONES ESTÁTICAS PARA OBTENER LAS ETIQUETAS
        //DE LAS PROPIEDADES EN FORMATO TEXTO:

        //obtiene las etiquetas de las propiedades de definición
        //en formato texto entre paréntesis
        static AnsiString GetLabels(void);
        //obtiene las etiquetas de las propiedades de definición
        //en formato línea de texto
        static AnsiString GetRowLabels(void);

        //MÉTODOS ESTÁTICOS DE COMPARACIÓN:

        //compara la propiedad P.x de dos figuras
        static int  Comparex(const TCircunference*,
                const TCircunference*);
        //compara la propiedad P.Y de dos figuras
        static int  Comparey(const TCircunference*,
                const TCircunference*);
        //compara la propiedad R de dos figuras
        static int  CompareR(const TCircunference*,
                const TCircunference*);

        //MÉTODOS ESTÁTICOS DE IMPRESIÓN:

        //imprime las propiedades de un objeto en una cadena de texto
        //en formato texto entre paréntesis
        static void  Print(AnsiString&, const TCircunference*);
        //imprime las propiedades de un objeto en una cadena de texto
        //en formato línea de texto
        static void  PrintRow(AnsiString&, const TCircunference*);
        //imprime las propiedades de un objeto en una cadena de texto
        //en formato de asignaciones
        static void  PrintAssigns(AnsiString&, const TCircunference*);

        //MÉTODOS ESTÁTICOS DE LECTURA:

        //lee las propiedades de un objeto en una cadena de texto
        //en formato texto entre paréntesis
        static void  Read(TCircunference*, const AnsiString&, int&);
        //lee las propiedades de un objeto en una cadena de texto
        //en formato línea de texto
        static void  ReadRow(TCircunference*, const AnsiString&, int&);

        //MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACIÓN:

        //contruye un objeto
        TCircunference(void);
        TCircunference(double x, double y, double _R);
        TCircunference(TDoublePoint _P, double _R);

        //copia las propiedades de un objeto
        void Copy(TCircunference*);
        TCircunference& operator=(const TCircunference&);

        //contruye un clon de un objeto
        TCircunference(TCircunference*);

        //MÉTODOS DE CONFIGURACIÓN:

        //asigna conjuntamente los valores de las propiedades de definición
        void SetValues(TDoublePoint _P, double _R);

        //MÉTODOS DE CÁLCULO DE DISTANCIAS:

        //calcula la distancia mínima entre la figura y un punto
        double DistancePoint(TDoublePoint);
        //calcula la distancia máxima entre la figura y un punto
        double DistancePointMax(TDoublePoint);

        //ADVERTENCIA: TDoublePoint no es una clase derivada de TFigure.

        //calcula la distancia mínima entre la figura y una circulo
        double DistanceCircle(TCircle*);
        //calcula la distancia mínima entre la figura y una circunferencia
        double DistanceCircunference(TCircunference*);

        //calcula la distancia mínima entre la figura y un segmento
        double DistanceSegment(TSegment*);
        //calcula la distancia mínima entre la figura y un arco
        double DistanceArc(TArc*);

        //calcula la distancia mínima entre la figura y otra figura
        double Distance(TFigure*);

        //MÉTODOS DE ROTACIÓN Y TRASLACIÓN:

        //obtiene la figutra geométrica rotada theta radianes
        //Figure debe contener una figura geométrica del mismo tipo
        void GetRotated(TFigure *F, double theta);
        //traslada la figura geométrica según el vector V
        void Translate(TDoublePoint V);

        //MÉTODOS GRÁFICOS:

        //dibuja la circunferencia
        //en un trazador de formas
        //#void Paint(TPloterShapes*);
};

//--------------------------------------------------------------------------
//TContourFigure
//--------------------------------------------------------------------------

//clase abstracta figura geométrica de contorno
class TContourFigure : public TFigure {
protected:
        //PROPIEDADES DE DEFINICIÓN:

        TDoublePoint __Pa;
        TDoublePoint __Pb;

public:
        //PROPIEDADES DE DEFINICIÓN:

        //vértice Pa de la figura
        //valor por defecto: {0, 0}
        TDoublePoint getPa(void) const {return __Pa;}
        virtual void setPa(TDoublePoint) = 0;
        //vértice Pb de la figura
        //valor por defecto: {0, 0}
        TDoublePoint getPb(void) const {return __Pb;}
        virtual void setPb(TDoublePoint) = 0;

        //MÉTODOS ESTÁTICOS DE LISTA:

        //imprime las propiedades de un objeto en una cadena de texto
        //en formato texto entre paréntesis
        static void  ContourFigurePrint(AnsiString& S,
                TContourFigure *F);
        //lee las propiedades de un objeto en una cadenade texto
        //en formato fila de texto
        static void  ContourFigureRead(TContourFigure* &F,
                const AnsiString& S, int &i);

        //MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACIÓN:

        //inicializa la propiedad Color a su valor por defecto
        //Pa y Pb quedarán sin inicializar
        TContourFigure(void);

        //destructor virtual
        virtual ~TContourFigure() = 0;

        //RECUEERDE: una clase madre debe tener un destructor virtual
        //para que la destrucción de objetos polimórficos nosea ambigua.
};

//--------------------------------------------------------------------------
//TSegment
//--------------------------------------------------------------------------

//clase segmento
class TSegment : public TContourFigure {
protected:
        //PROPIEDADES DE DEFINICIÓN:


        //CONUNTOS DE PROPIEDADES EN FORMATO TEXTO:


public:
        //PROPIEDADES DE DEFINICIÓN:

        //coordenadas cartesianas del punto inicial
        //valor por defecto: (0, 0)
        void setPa(TDoublePoint _Pa);
        //coordenadas cartesianas del punto final
        //valor por defecto: (0, 0)
        void setPb(TDoublePoint _Pb);

        //ADVERTENCIA: Pa y Pb pueden coincidir.

        //CONUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjunto de propiedades de definición
        //en formato texto entre paréntesis
        AnsiString getText(void) const; void setText(const AnsiString&);
        //conjunto de propiedades de definición
        //en formato línea de texto
        AnsiString getRowText(void) const; void setRowText(const AnsiString&);

        //MÉTODOS ESTÁTICOS DE LISTA:

        //imprime las propiedades de un objeto en una cadena de texto
        //en formato texto entre paréntesis
        static void  Print(AnsiString&, TContourFigure*);
        //imprime las propiedades de un objeto en una cadena de texto
        //en formato línea de texto
        static void  PrintRow(AnsiString&, TContourFigure*);

        //lee las propiedades de un objeto en una cadenade texto
        //en formato texto entre paréntesis
        static void  Read(TContourFigure*,
                const AnsiString&, int&);
        //lee las propiedades de un objeto en una cadenade texto
        //en formato línea de texto
        static void  ReadRow(TContourFigure*,
                const AnsiString&, int&);

        //MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

        //contruye un objeto
        TSegment(void);
        TSegment(double ax, double ay, double bx, double by);
        TSegment(TDoublePoint _Pa, TDoublePoint _Pb);

        //copia las propiedades de un objeto
        void Copy(TSegment*);
        TSegment& operator=(const TSegment&);

        //contruye un clon de un objeto
        TSegment(TSegment*);

        //MÉTODOS DE CONFIGURACIÓN:

        //asigna conjuntamente los valores de las propiedades de definición
        void SetValues(TDoublePoint _Pa, TDoublePoint _Pb);

        //MÉTODOS DE CÁLCULO DE DISTANCIAS:

        //calcula la distancia mínima entre la figura y un punto
        double DistancePoint(TDoublePoint Q);
        //calcula la distancia máxima entre la figura y un punto
        double DistancePointMax(TDoublePoint Q);

        //ADVERTENCIA: TDoublePoint no es una clase derivada de TFigure.

        //calcula la distancia mínima entre la figura y una circulo
        double DistanceCircle(TCircle *C);
        //calcula la distancia mínima entre la figura y una circunferencia
        double DistanceCircunference(TCircunference *C);

        //calcula la distancia mínima entre la figura y un segmento
        double DistanceSegment(TSegment *S);
        //calcula la distancia mínima entre la figura y un arco
        double DistanceArc(TArc *A);

        //calcula la distancia mínima entre la figura y otra figura
        double Distance(TFigure *F);

        //MÉTODOS DE ROTACIÓN Y TRASLACIÓN:

        //obtiene la figutra geométrica rotada theta radianes
        //Figure debe contener una figura geométrica del mismo tipo
        void GetRotated(TFigure *F, double theta);
        //traslada la figura geométrica según el vector V
        void Translate(TDoublePoint V);

        //MÉTODOS GRÁFICOS:

        //dibuja el segmento
        //en un trazador de formas
        //#void Paint(TPloterShapes*);
};

//clase arco
class TArc : public TContourFigure {
protected:
        //PROPIEDADES DE DEFINICIÓN:

        TDoublePoint __Pc;
        double __R;

        //PROPIEDADES DE DEFINICIÓN EN FORMATO TEXTO:


        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:


public:
        //PROPIEDADES DE DEFINICIÓN:

        //vértice final del arco (en sentido levógiro)
        //debe ser distinto del centro Pc
        //debe estar en el entorno del arco
        //valor por defecto: {1, 0}
        void setPa(TDoublePoint);
        //vértice inicial del arco (en sentido levógiro)
        //debe ser distinto del centro Pc
        //debe estar en el entorno del arco
        //valor por defecto: {1, 0}
        void setPb(TDoublePoint);
        //centro del arco
        //debe ser distinto de los vértices
        //debe estar en el entorno del centro del arco
        //valor por defecto: {0, 0}
        TDoublePoint getPc(void) const {return __Pc;} void setPc(TDoublePoint);
        //radio del arco
        //debe ser mayor que cero
        //el radio R debe ser tal que Pa y Pb estén en el entorno del arco
        //valor por defecto: 1
        double getR(void) const {return __R;} void setR(double);

        //ADVERTENCIA: el radio R debe ser mayor que cero, ya que
        //Pa y Pb no pueden coincidir con Pc.

        //PROPIEDADES DE DEFINICIÓN EN FORMATO TEXTO:

        AnsiString getPaText(void) const; void setPaText(const AnsiString& S);
        AnsiString getPbText(void) const; void setPbText(const AnsiString& S);
        AnsiString getPcText(void) const; void setPcText(const AnsiString& S);

        AnsiString getRText(void) const; void setRText(const AnsiString&);

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjunto de propiedades de definición
        //en formato texto entre paréntesis
        AnsiString getText(void) const; void setText(const AnsiString&);
        //conjunto de propiedades de definición
        //en formato línea de texto
        AnsiString getRowText(void) const; void setRowText(const AnsiString&);

        //MÉTODOS ESTÁTICOS DE LISTA:

        //imprime las propiedades de un objeto en una cadena de texto
        //en formato texto entre paréntesis
        static void  Print(AnsiString&, TContourFigure*);
        //imprime las propiedades de un objeto en una cadena de texto
        //en formato línea de texto
        static void  PrintRow(AnsiString&, TContourFigure*);

        //lee las propiedades de un objeto en una cadena de texto
        //en formato texto entre paréntesis
        static void  Read(TContourFigure*,
                const AnsiString&, int&);
        //lee las propiedades de un objeto en una cadena de texto
        //en formato línea de texto
        static void  ReadRow(TContourFigure*,
                const AnsiString&, int&);

        //MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACIÓN:

        //contruye un objeto
        TArc(void);
        TArc(double ax, double ay, double bx, double by, double cx, double cy,
                double _R);
        TArc(TDoublePoint _Pa, TDoublePoint _Pb, TDoublePoint _Pc,
                double _R);

        //copia las propiedades de un objeto
        void Copy(TArc*);
        TArc& operator=(const TArc&);

        //contruye un clon de un objeto
        TArc(TArc*);

        //MÉTODOS DE CONFIGURACIÓN:

        //Si se dispone de nuevas coordenadas para el arco,
        //y se quiere evitar que salten las alarmas introducidas por errores
        //numéricos, pueden realizarse las operaciones siguientes:
        //      v = Pa - Pc;
        //      Pa = (v)/Mod(v)*R + Pc;
        //      v = Pb - Pc;
        //      Pb = (v)/Mod(v)*R + Pc;
        //pero hay que advertir que estas operaciones pueden introducir cambios
        //en Pa y Pb, de modo que debería evitarse su aplicación si no
        //resulta necesario. Para ello se dispone del método:
        //      bool TArc::IsInRangeArc(P);
        //y de la función:
        //      bool Mathematics::IsArc(Pa, Pb, Pc, R);

        //determina si un punto está en el entorno del arco
        //      Abs(Mod(P - Pc) - R) <= ERR_NUM
        bool IsInRangeArc(TDoublePoint P);

        //IsInRangeArc realiza las siguientes operaciones:
        //      Para verificar que Pa está en el entorno del arco,
        //              Abs(Mod(Pa - Pc) - R) < ERR_NUM;
        //      Para verificar que Pb está en el entorno del arco,
        //              Abs(Mod(Pb - Pc) - R) < ERR_NUM;
        //      Y para verificar que Pc está en el entorno del centro del arco,
        //              se realizan las dos operaciones anteriores.

        //ADVERTENCIA: IsInRangeArc no tiene el radio del entorno
        //como argumento, para evitar un uso incorrecto del mismo,
        //ya que hay otros método que invocan a este, que lo invocarán
        //con su valor por defecto, de modo que el uso de dicho método
        //con un radio del entorno distinto se considera innecesario
        //y peligroso.
        //La función Mathematics::IsArc(Pa, Pb, Pc, R, epsilon==ERRNUM)
        //en cambio si tiene el radio del entorno como argumento por
        //que no es utilizada en ninguno de los método de esta clase.

        //asigna conjuntamente los valores de las propiedades de definición
        //Lanza una excepción EImproperArgument si:
        //      Pa o Pb es igual a Pc,
        //      R no es mayor que cero,
        //      Pa o Pb no está en el entorno del arco,
        void SetValues(TDoublePoint _Pa, TDoublePoint _Pb, TDoublePoint _Pc,
                double _R);

        //MÉTODOS DE CÁLCULO DE DISTANCIAS:

        //calcula la distancia mínima entre la figura y un punto
        double DistancePoint(TDoublePoint);
        //calcula la distancia máxima entre la figura y un punto
        double DistancePointMax(TDoublePoint);

        //ADVERTENCIA: TDoublePoint no es una clase derivada de TFigure.

        //calcula la distancia mínima entre la figura y una circulo
        double DistanceCircle(TCircle*);
        //calcula la distancia mínima entre la figura y una circunferencia
        double DistanceCircunference(TCircunference*);

        //calcula la distancia mínima entre la figura y un segmento
        double DistanceSegment(TSegment*);
        //calcula la distancia mínima entre la figura y un arco
        double DistanceArc(TArc*);

        //calcula la distancia mínima entre la figura y otra figura
        double Distance(TFigure*);

        //MÉTODOS DE ROTACIÓN Y TRASLACIÓN:

        //obtiene la figutra geométrica rotada theta radianes
        //Figure debe contener una figura geométrica del mismo tipo
        void GetRotated(TFigure *F, double theta);
        //traslada la figura geométrica según el vector V
        void Translate(TDoublePoint V);

        //MÉTODOS GRÁFICOS:

        //dibuja el arco
        //en un trazador de formas
        //#void Paint(TPloterShapes*);
};

//FUNCIONES RELACIONADAS:

//determina el ángulo entre el vértice inicial de una figura de contorno
//y su vértice final en torno de un punto P
double Rotation(TContourFigure *F, bool PbIsNext, TDoublePoint P);

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // FIGURE_H
