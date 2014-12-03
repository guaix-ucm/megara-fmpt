//---------------------------------------------------------------------------
//Archivo: Polypoint.h
//Contenido: clase polipunto y funciones
//Última actualización: 22/03/2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef POLYPOINT_H
#define POLYPOINT_H

#include "..\3_Lists\ItemsList.h"
#include "Vectors.h"

using namespace Lists;

//---------------------------------------------------------------------------

//espacio de nombres de clases y funciones matemáticas
namespace Mathematics {

//---------------------------------------------------------------------------
//Clipeado de segmentos:
//---------------------------------------------------------------------------

//Clipea el segmento p=(p1, p2) con con la recta r=(A, B, C) eliminando
//la parte que queda en el lado negativo de la recta, esto es a la derecha.
//Si todo el segmento está en el lado negativo devuelve false.
bool Clip(TDoublePoint& p1, TDoublePoint& p2, double A, double B, double C);

//Clipeado de polilíneas:

//Una polilínea cerrada y convexa es un polígono.
//Un polígono a izdas tiene el interior positivo y el exterior negativo,
//y uno derechas tiene el interior negativo y el exterior positivo.

//NOTA: no se puede declarar una TGenericList de referencias,
//porque no se puede definir una referencia a referencia
typedef TItemsList<TDoublePoint> TPolypoint;

//Determina si la polilinea poly corresponde a un polígono cerrado y convexo
//devuelve: 0 si no es un polígono; 1 si es un polígono a izquierdas;
//y -1 si es un polígono a derechas.
char ClosedAndConvex(TPolypoint *poly);

//Expresa el rectángulo ortogonal de vértices (Left, Bottom) y (Right, Top)
//en forma de polígono a izquierdas, empezando por el vértice inferior izquierdo
void RectToPoly(double Left, double Bottom, double Right, double Top,
    TPolypoint *poly);
//NOTA: las coordenadas se refieren a un sistema de coordenadas imaginario
//con el eje y apuntando de abajo arriba y el eje x a de izda a dcha
//Expresa el rectángulo ortogonal r en forma de polígono a izquierdas,
//empezando por el vértice inferior izquierdo
void RectToPoly(const TDoubleRect &r, TPolypoint *poly);

//Determina si el punto p es interior al polígono poly
bool Inner(const TDoublePoint& p, TPolypoint *poly);

//Clipea el segmento (p1, p2) con las rectas que contienen los segmentos de
//poly, eliminando la parte que queda en el lado negativo de estas rectas;
//esto es, eliminando la parte que queda en el exterior de poly (si es a
//izquierdas), o la parte que queda en el interior de poly (si es a derechas).
//Si ningun pto de (p1, p2) queda en la parte positiva de poly devuelve false.
//La longitud del segmento puede ser cero, es decir p1 puede ser igual a p2.
bool Clip(TDoublePoint& p1, TDoublePoint& p2, TPolypoint *poly);

//---------------------------------------------------------------------------
//Intersección entre pares de figuras:

//determina si hay intersección entre los lados de los polígonos Fa y Fb
//si los polígonos no se tocan devuelve falso
bool IntersectionPolygonPolygon(TPolypoint *Fa, TPolypoint *Fb);
//determina si hay intersección entre
//la circunferencia (P, R) y los lados del polígono F
//si las figuras no se tocan devuelve falso
bool IntersectionPolygonCircle(TPolypoint *F, TDoublePoint P, double R);

//---------------------------------------------------------------------------

} //namespace Mathematics

//---------------------------------------------------------------------------
#endif // POLYPOINT_H
