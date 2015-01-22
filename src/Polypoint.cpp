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
//Archivo: Polypoint.cpp
//Contenido: clase polipunto y funciones
//Última actualización: 23/05/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------


#include "Polypoint.h"
#include "Geometry.h"
#include "Scalars.h"

//---------------------------------------------------------------------------

//espacio de nombres de clases y funciones matemáticas
namespace Mathematics {

//---------------------------------------------------------------------------
//Clipeado de segmentos:
//---------------------------------------------------------------------------

//Clipea el segmento p=(p1, p2) con con la recta r=(A, B, C) eliminando
//la parte que queda en el lado negativo de la recta, esto es a la derecha.
//Si todo el segmento está en el lado negativo devuelve false.
bool Clip(TDoublePoint& p1, TDoublePoint& p2, double A, double B, double C)
{
    //el segmentos p debe tener longitud superior a cero
    if(p1.x==p2.x && p1.y==p2.y)
        throw EImproperArgument("p1 should be unequal p2");

    //el vector normal de la recta (A, B) debe tener módulo mayor que cero
    if(A==0 && B==0)
        throw EImproperArgument("(A, B) dont should be null vector");

    //Normaliza los parámetros de la recta
    Normalize(A, B, C);

    double d1 = Distance(p1, A, B, C); //distancia de p1 a r
    double d2 = Distance(p2, A, B, C); //distancia de p2 a r

    //si el segmento no asoma ni la nariz, devuelve false
    if(d1<0 && d2<0)
        return false;

    //si el segmento está todo en el lado positivo devuelve true
    if(d1>=0 && d2>=0)
        return true;

    //si hay un punto a cada lado debe calcular el punto de intersección
    double pA, pB, pC;
    CanonicalEquation(p1, p2, pA, pB, pC);
    TDoublePoint r; //punto de intersección
    Intersection(r, A, B, C, pA, pB, pC);
    //recorta el lado del segmento que queda en el lado negativo
    if(d1<0)
        p1 = r;
    else if(d2<0)
        p2 = r;

    return true; //indica que hay parte del segmento en el lado positivo
}

//Clipeado de polilíneas:

//Determina si la polilinea poly corresponde a un polígono cerrado y convexo
//Valores de retorno:
//	ClosedAndConvex: 0 si no es un polígono;
//		1 si es un polígono a izquierdas (interior positivo);
//		y -1 si es un polígono a derechas (itnerior negativo).
char ClosedAndConvex(TPolypoint *poly)
{
    //la polilinea debe tener por lo menos tres puntos
    //para componer un triángulo
    if(poly->getCount() < 3)
        throw EImproperArgument(
                "poly should contain almos three points");

    //el último punto de la polilínea debe coincidir con el primero
    if(poly->getLast().x!=poly->getFirst().x || poly->getLast().y!=poly->getFirst().y)
        return false;

    //verifica que el segmento contiguo tuerce en la misma dirección
    //para todos los segmentos que componen la polilinea
    double A, B, C; //parámetros de la línea que contiene el segmento

    //toma la referencia del primer segmento
    CanonicalEquation(poly->Get(0), poly->Get(1), A, B, C);
    double d = Distance(poly->Get(poly->getCount()-2), A, B, C);
    //si hay dos segmentos contígüos en la misma recta no es un polígono
    if(d==0)
        return 0;
    char s = char(Sign(d)); //posición del pto contiguo relativa a la recta

    //verifica desde el segundo hasta el último segmento
    for(int i=2; i<poly->getCount(); i++) {
        CanonicalEquation(poly->Get(i-1), poly->Get(i), A, B, C);
        d = Distance(poly->Get(i-2), A, B, C);
        //si hay dos segmentos contígüos en la misma recta
        //no es un polígono
        if(d==0)
            return 0;
        //si el próximo segmento gira en sentido contrario a
        //los precedentes no es un polígono
        if((char)Sign(d) != s)
            return 0;
    }

    return s;
}

//Expresa el rectángulo ortogonal de vértices (Left, Bottom) y (Right, Top)
//en forma de polígono a izquierdas, empezando por el vértice inferior izquierdo
void RectToPoly(double Left, double Bottom, double Right, double Top,
    TPolypoint *poly)
{
    //el rectángulo deberá ser positivo mayor que cero
    if(Right<=Left || Top<=Bottom)
        throw EImproperArgument("rectangle dont should be negative");

    //poly debe ser un polýgono ya construido
    if(poly==NULL)
        throw EImproperArgument("poly cant be null pointer");

    poly->Clear(); //vacía poly
    TDoublePoint p; //punto para añadir

    //contruye y añade el vértice inf izdo
    p.x = Left;
    p.y = Bottom;
    poly->Add(p);

    //contruye y añade el vértice inf dcho
    p.x = Right;
    p.y = Bottom;
    poly->Add(p);

    //contruye y añade el vértice sup dcho
    p.x = Right;
    p.y = Top;
    poly->Add(p);

    //contruye y añade el vértice sup izdo
    p.x = Left;
    p.y = Top;
    poly->Add(p);

    //el último punto debe ser igual que el primero
    p = poly->Get(0);
    poly->Add(p);
}

//Expresa el rectángulo ortogonal r en forma de polígono a izquierdas,
//empezando por el vértice inferior izquierdo
void RectToPoly(const TDoubleRect& r, TPolypoint *poly)
{
    RectToPoly(r.Left, r.Bottom, r.Right, r.Top, poly);
}

//Determina si el punto p es interior al polígono poly
bool Inner(const TDoublePoint& p, TPolypoint *poly)
{
    //poly debe ser un polígono
    if(ClosedAndConvex(poly) == 0)
        throw EImproperArgument("poly should be a polygon");

    double A, B, C;
    //toma la referencia del primer segmento
    CanonicalEquation(poly->Get(0), poly->Get(1), A, B, C);
    double d = Distance(p, A, B, C);
    char s = (char)Sign(d); //posición del punto relativa a la recta
    //Comprueba que coincide con los demás segmentos
    for(int i=1; i<poly->getCount()-1; i++) {
        CanonicalEquation(poly->Get(i), poly->Get(i+1), A, B, C);
        if((char)Sign(Distance(p, A, B, C)) != s)
            return false;
    }

    return true;
}

//Clipea el segmento (p1, p2) con las rectas que contienen los segmentos de
//poly, eliminando la parte que queda en el lado negativo de estas rectas;
//esto es, eliminando la parte que queda en el exterior de poly (si es a
//izquierdas), o la parte que queda en el interior de poly (si es a derechas).
//Si ningun pto de (p1, p2) queda en la parte positiva de poly devuelve false.
//La longitud del segmento puede ser cero, es decir p1 puede ser igual a p2.
bool Clip(TDoublePoint& p1, TDoublePoint& p2, TPolypoint *poly)
{
    //poly debe ser un polígono
    if(ClosedAndConvex(poly)==0)
        throw EImproperArgument("poly should be a polygon");

    //Pueden darse tres casos:
    //  - los dos puntos dentro;
    //  - un punto dentro y el otro fuera;
    //  - y los dos puntos fuera.

    bool inner1 = Inner(p1, poly);
    bool inner2 = Inner(p2, poly);


    //Si los dos son internos no hay intersección
    if(inner1 && inner2)
        return true;

    //Si p1 y p2 son externos puede haber dos puntos de corte o ninguno
    if(!inner1 && !inner2) {
        int npoints=0; //nº de puntos de intersección encontrados
        if(p1.x!=p2.x || p1.y!=p2.y) { //externos e iguales no invaden poly
            TDoublePoint r(0, 0); //pto de intersección (caso de hallar alguno)
            char s; //posición relat. de los ptos (p1, p2) a la recta continente
            bool imposible = false; //indicador de que no puede haber intersección
            int i=1; //índice a los segmentos de poly
            do {
                if(Intersection(p1, p2, poly->Get(i-1), poly->Get(i), r, s)) {

                    //si se encuentra un punto de intersección
                    //debe recortarse el vértice de (p1, p2)
                    //que quede en el lado negativo
                    switch(s) {
                    case 1: p1 = r; break;
                    case -1: p2 = r; break;
                    }
                    npoints++;
                } else if(s == -2) //ambos puntos p1 y p2 a la dcha
                    imposible = true;

                i++; //apunta al siguiente segmento
            } while (!imposible && npoints<2 && i<poly->getCount());
            //mientras ambos puntos no queden en el lado negativo
            //de la recta continente,no se hallan encontrado dos
            //puntos de intersección y no se agoten los segmentos de poly
        }

        //si se encontraron puntos de corte devuelve true
        if(npoints)
            return true;
        else
            return false;
    }

    //si un punto es itnerior y el otro exterior habrá un solo punto de corte
    TDoublePoint r;
    char s; //posición relativa de los punto (p1, p2) a la recta continente
    bool intersected; //flag de intersección
    int i=1; //índice a segmentos de poly
    do {
        intersected = Intersection(p1, p2, poly->Get(i-1), poly->Get(i), r, s);
        i++;
    } while (!intersected && i<poly->getCount());

    //el punto de corte pudiera no encontrarse por error numérico (extraño)

    //si se encuentra el punto de corte recorta el vértice del lado negativo
    if(i < poly->getCount()) {
        if(s == 1)
            p1 = r;
        else if(s == -1)
            p2 = r;
    }

    return true;
}

//---------------------------------------------------------------------------
//Intersección entre pares de figuras:

//determina si hay intersección entre los lados de los polígonos Fa y Fb
//si los polígonos no se tocan devuelve falso
bool IntersectionPolygonPolygon(TPolypoint *Fa, TPolypoint *Fb)
{
    //determina si algún par de segmentos se cortan
    TDoublePoint P;
    for(int i=1; i<Fa->getCount(); i++)
        for(int j=1; j<Fb->getCount(); j++)
            if(IntersectionSegmentSegment(P,
                                          Fa->Get(i-1), Fa->Get(i),
                                          Fb->Get(j-1), Fb->Get(j)))
                return true; //indica que hay intersección

    return false; //indica que no se han encontrado intersección
}
//determina si hay intersección entre
//la circunferencia (P, R) y los lados del polígono F
//si las figuras no se tocan devuelve falso
bool IntersectionPolygonCircle(TPolypoint *F, TDoublePoint P, double R)
{
    //busca algún punto de la polilínea que
    //esté al menos a distancia R del centro
    for(int i=1; i<F->getCount(); i++)
        if(DistanceSegmentPoint(F->Get(i-1), F->Get(i), P) <= R)
            return true; //indica que hay intersección

    return false; //indica que no se ha encontrado intersección
}

//---------------------------------------------------------------------------

} //namespace Mathematics

//---------------------------------------------------------------------------

