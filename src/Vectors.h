// Copyright (c) 2012-2017 Isaac Morales Durán. All rights reserved.
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
///@file Vectors.h
///@brief classes and functions for vectors
///@author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef VECTORS_H
#define VECTORS_H

#include "VCLemu.h" //AnsiString

#include <complex> //complex

//---------------------------------------------------------------------------

using namespace std; //complex

//espacio de nombres de clases y funciones matemáticas
namespace Mathematics {

//---------------------------------------------------------------------------
//Estructuras TDoublePoint y TDoubleRect:

//se definen TDoublePoint y TDoubleRect homólogos a
//TPoint y TRect pero con doubles en vez de enteros

/*typedef struct {
        double x;
        double y;

//        AnsiString xText;
} TDoublePoint;

typedef struct {
        union {
        struct {
            TDoublePoint BottomLeft;
            TDoublePoint TopRight;
        };
        struct {
            double Left;
                        double Bottom;
                        double Right;
                        double Top;
        };
        };
} TDoubleRect;*/

//RECUERDE: la definición a base de solo tipos primitivos permite inicializar
//los objetos mediante asignaciones:
//      TDoublePoint P = {1, 2};
//En cambio la definición con tipos no primitivos o métodos, obliga a
//la inicialización mediante el constructor:
//      TDoublePoint P(1, 2);

//Para evitar las limitaciones de la declaración anterior se prefiere
//la definición de la clase con propiedades no primitivas y métodos:

//clase punto bidimensional
class TDoublePoint {
public:
    //labelof property x
    //dafault value: "x"
    static AnsiString xLabel;
    //labelof property y
    //dafault value: "y"
    static AnsiString yLabel;

    //abcisa of the point
    //default value: 0
    double x;
    //ordinate of the point
    //default value: 0
    double y;

    //PROPIEDADES DE DEFINICIÓN EN FORMATO TEXTO:

    AnsiString getxText(void) const; void setxText(const AnsiString&);
    AnsiString getyText(void) const; void setyText(const AnsiString&);

    //CONJUNTOS DEPROPIEDADES EN FORMATO TEXTO:

    AnsiString getText(void) const; void setText(const AnsiString&);
    AnsiString getRowText(void) const;

    //FUNCIONES ESTÁTICAS PARA OBTENER LAS ETIQUETAS
    //DE LAS PROPIEDADES EN FORMATO TEXTO:

    //print the properties of an double point in a string
    //in row format
    static void  PrintRow(AnsiString&, const TDoublePoint);

    //obtiene las etiquetas de las propiedades de definición
    //en formato texto entre paréntesis
    static AnsiString GetLabels(void);
    //obtiene las etiquetas de las propiedades de definición
    //en formato línea de texto
    static AnsiString GetRowLabels(void);

    //MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACION:

    //contruye un punto
    TDoublePoint(void) : x(0), y(0) {;}
    TDoublePoint(double t_x, double t_y) : x(t_x), y(t_y) {;}

    //copia un punto
    void Copy(TDoublePoint);
    TDoublePoint& operator=(const TDoublePoint&);

    //MÉTODOS DE CONFIGURACIÓN:

    //randomiza las coordenadas
    //distribución uniforme en un intervalo
    void Randomize(double xmin, double xmax, double ymin, double ymax);

    //MÉTODOS SIN ARGUMENTOS:

    //calcula el módulo del punto:
    //      Mathematics::Mod(x, y);
    double Mod(void) const;
    //calcula el argumento del punto:
    //      Mathematics::Arg(x, y);
    double Arg(void) const;
};

//clase rectángulo
class TDoubleRect {
    TDoublePoint GetLeftBottom(void) const;
    void SetLeftBottom(const TDoublePoint&);
    TDoublePoint GetRightTop(void) const;
    void SetRightTop(const TDoublePoint&);

    AnsiString GetLeftText(void) const; void SetLeftText(const AnsiString&);
    AnsiString GetBottomText(void) const; void SetBottomText(const AnsiString&);
    AnsiString GetRightText(void) const; void SetRightText(const AnsiString&);
    AnsiString GetTopText(void) const; void SetTopText(const AnsiString&);

    AnsiString GetText(void) const; void SetText(const AnsiString&);
    AnsiString GetRowText(void) const;

public:
    //ETIQUETAS:

    static AnsiString LeftLabel;
    static AnsiString BottomLabel;
    static AnsiString RightLabel;
    static AnsiString TopLabel;

    //PROPIEDADES INDIVIDUALES:

    double Left;
    double Bottom;
    double Right;
    double Top;

    //CONJUNTOS DE PROPIEDADES:

    TDoublePoint getLeftBottom(void) const;
    void setLeftBottom(const TDoublePoint&);
    TDoublePoint getRightTop(void) const;
    void setRightTop(const TDoublePoint&);

    //PROPIEDADES DE DEFINICIÓN EN FORMATO TEXTO:

    AnsiString getLeftText(void) const; void setLeftText(const AnsiString&);
    AnsiString getBottomText(void) const; void setBottomText(const AnsiString&);
    AnsiString getRightText(void) const; void setRightText(const AnsiString&);
    AnsiString getTopText(void) const; void setTopText(const AnsiString&);

    //CONJUNTOS DEPROPIEDADES EN FORMATOTEXTO:

    AnsiString getText(void) const; void setText(const AnsiString&);
    AnsiString getRowText(void) const;

    //FUNCIONES ESTÁTICAS PARA OBTENER LAS ETIQUETAS
    //DE LAS PROPIEDADES EN FORMATO TEXTO:

    //obtiene las etiquetas de las propiedades de definición
    //en formato texto entre paréntesis
    static AnsiString GetLabels(void);
    //obtiene las etiquetas de las propiedades de definición
    //en formato línea de texto
    static AnsiString GetRowLabels(void);

    //MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACION:

    //contruye un rectángulo
    TDoubleRect(void) : Left(0), Bottom(0), Right(0), Top(0) {;}
    TDoubleRect(double L, double B, double R, double T) :
        Left(L), Bottom(B), Right(R), Top(T) {;}

    //copia un rectángulo
    void Copy(TDoubleRect);
    TDoubleRect& operator=(const TDoubleRect&);

    //MÉTODOS DE CONFIGURACIÓN:

    //randomiza las coordenadas
    //distribución uniforme en un intervalo
    void Randomize(double Lmin, double Lmax, double Bmin, double Bmax,
                   double Rmin, double Rmax, double Tmin, double Tmax);

};

//---------------------------------------------------------------------------
//Clase TPhasor:

//se define la clase TPhasor para facilitar la conversión
//de coordenadas polares a coordenadas rectangulares
//y simplificar la notación

class TPhasor {
protected:

    double p_r;

public:
    //amplitud en unidades de longitud
    //debe ser no negativo
    //valor por defect0: 0
    double getr(void) const {return p_r;} void setr(double);
    //fase en radianes
    //puede ser cualquier valor real
    //valor por defecto: 0
    double theta;

    //fasor en formato complex
    complex<double> getComplex(void);
    //fasor en formato texto
    AnsiString getText(void);

    //MÉTODOS:

    //construye un un fasor inicializándolo a (0, 0)
    TPhasor(void) : p_r(0), theta(0) {;}
    //construye un un fasor inicializándolo a (r, theta)
    TPhasor(double r, double theta);
    //construye un fasor a partir de un complejo
    TPhasor(const complex<double> &c);
    //construye un clon de un fasor
    TPhasor(const TPhasor &P) : p_r(P.p_r), theta(P.theta) {;}

    //asigna las propiedades de un fasor a este fasor
    TPhasor& operator=(const TPhasor& P);
    //traduce y asigna un complejo a este fasor
    TPhasor& operator=(const complex<double>& c);
};

//---------------------------------------------------------------------------
//Clase TVector3D:

class TVector3D {
public:
    //coordenadas cartesianas
    double x, y, z;

    //módulo del vector
    double getModule(void) const {return sqrt(x*x + y*y + z*z);}

    //contruye un vector tridimensional
    TVector3D(double t_x=0, double t_y=0, double t_z=0) :
        x(t_x), y(t_y), z(t_z) {;}

    //asignación de un vector
    TVector3D &operator=(const TVector3D&);

    //suma de componentes
    TVector3D operator+(const TVector3D&) const;
    //resta de componentes
    TVector3D operator-(const TVector3D&) const;
    //producto por un escalar
    TVector3D operator*(double t) const;
    //división por un escalar
    TVector3D operator/(double t) const;

    //producto interno (producto punto) por un vector
    double Dot(const TVector3D&) const;
    //producto vectorial por un vector
    TVector3D Cross(const TVector3D&) const;
};

//---------------------------------------------------------------------------
//Funciones con vectores bibimensionales:

//determina si los vectores v1 y v2 son semejantes
bool operator== (TDoublePoint v1, TDoublePoint v2);
//determina si los vectores v1 y v2 son diferentes
bool operator!= (TDoublePoint v1, TDoublePoint v2);

//invierte el sentido del vector
TDoublePoint operator- (TDoublePoint v);
//suma v1 y v2
TDoublePoint operator+ (TDoublePoint v1, TDoublePoint v2);
//resta v2 a v1
TDoublePoint operator- (TDoublePoint v1, TDoublePoint v2);
//producto del escalar x por el vector v
TDoublePoint operator* (double x, TDoublePoint v);
//producto del vector v por el escalar x
TDoublePoint operator* (TDoublePoint v, double x);
//división del vector v por el escalar x
TDoublePoint operator/ (TDoublePoint v, double x);

//producto interno de v1 y v2
double Dot(TDoublePoint v1, TDoublePoint v2);
//gira el vector PI/2 rad en sentido levógiro
TDoublePoint Cross(TDoublePoint v);
//determinante de la matriz {{v1.x, v1.y}, {v2.x, v2.y}}
double Det(TDoublePoint v1, TDoublePoint v2);

//obtiene el modulo de un punto dado en coordenadas cartesianas rectangulares
double Mod(double x, double y);
double Mod(TDoublePoint v);// {return Mod(v.x, v.y);}
//obtiene el argument principal (en el intervalo (-M_PI, M_PI],
//de un punto dado en coordenadas cartesianas rectangulares
double Arg(double x, double y);
double Arg(TDoublePoint v);// {return Arg(v.x, v.y);}
//obtiene el argument positivo principal (en el intervalo [0, M_2PI),
//de un punto dado en coordenadas cartesianas rectangulares
double ArgPos(double x, double y);
double ArgPos(TDoublePoint v);// {return ArgPos(v.x, v.y);}
//traduce de corodenadas polares a cartesianas
void PolarToCartesian(double &x, double &y, double r, double theta);
//traduce de coordenadas cartesianas a polares
void CartesianToPolar(double &r, double &theta, double x, double y);
//calcula el ángulo entre los vectores vini y vfin en sentido levógiro
//en el intervalo (-PI, PI]
double Angle(TDoublePoint vini, TDoublePoint vfin);
//calcula el ángulo entre los vectores vini y vfin en sentido levógiro
//en el intervalo [0, 2*PI)
double AnglePos(TDoublePoint vini, TDoublePoint vfin);


//imprime el punto P en la cadena de texto S
AnsiString DPointToStr(TDoublePoint P);
//imprime el rectángulo R en la cadena de texto S
AnsiString DRectToStr(TDoubleRect R);

//calcula el punto al que se desplaza el punto P
//cuando es rotado a rad en torno de Q
TDoublePoint Rotate(TDoublePoint P, TDoublePoint Q, double a);

//rotate a bidimensional vector 90 degrees
TDoublePoint rotate90(TDoublePoint v);

//determine the versor in the middle of two vectors
//when ArgPos(v1) == ArgPos(v2)
//  return TDoublePoint(cos(ArgPos(v1)), sin(ArgPos(v1)))
TDoublePoint middleVersor(TDoublePoint v1, TDoublePoint v2);

//---------------------------------------------------------------------------
//Funciones con vectores tridimensionales:

//detrmina si un vector es el vector nulo
bool IsNull(const TVector3D&);

//genera un versor tridimensional con distribución uniforme
//en el casquete esférico centrado en el origen que lo contiene
//      x = sen(a)*sen(b)*cos(c) - cos(a)*sen(c)
//      y = sen(a)*sen(b)*sen(c) + cos(a)*cos(c)
//      z = sen(a)*cos(b)
//donde a, b y c son variables aleatorias con distribución uniforme en (-pi, pi]
void RandomVersor(double &x, double &y, double &z);

//NOTA: las funcioones de Randomize correpsonden a girar el versor
//en torno de los ejes {x, y, z} los ángulos a, b y c.

//calcula la colatitud (theta) en el intervalo [-pi/2, pi/2]
//de un vector tridimensional en coordenadas cartesianas
double Colatitud(double x, double y, double z);
//calcula el azimut (phi) en el intervalo [-pi, pi]
//de un vector tridimensional en coordenadas cartesianas
double Azimut(double x, double y);

//traduce de coordenadas esféricas con theta en [-pi/2, pi/2] y phi en R
//a coordenadas cartesianas
void SphericToCartesian(double &x, double &y, double &z,
                        double theta, double phi, double r);
//traduce de coordenadas cartesianas a coordenadas esféricas
//con theta en [-pi/2, pi/2] y phi en [-pi, pi]
void CartesianToSpheric(double &theta, double &phi, double &r,
                        double x, double y, double z);

//ADVERTENCIA: las coordenadas polares que aquí estamos utilizando
//tienen el origen de phi en el ecuador.

//calcula el ángulo entre los vectores tridimensionales
//en el intervalo [0, 2*PI)
double Angle(const TVector3D &vini, const TVector3D &vfin);

//Dados dos vectores no colineales U y V:
//      calcula los versores u y v crrespondientes a normalizar U y V,
//      calcular el versor w normal a u y v,
//      y calcular los versores u' y v' correspondientes a girar u y v
//      gamma radianes en torno de w.
void RotateVectors(TVector3D &u_, TVector3D &v_, TVector3D &w,
                   const TVector3D &U, const TVector3D &V, double gamma);

//---------------------------------------------------------------------------

} //namespace Mathematics

//---------------------------------------------------------------------------
#endif //VECTORS_H
