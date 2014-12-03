//---------------------------------------------------------------------------
//Archivo: Geometry.h
//Contenido: clases y funciones geométricas
//Última actualización: 25/08/2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Constants.h"
#include "Vectors.h"
#include "ItemsList.h"

using namespace Lists;

//---------------------------------------------------------------------------

//espacio de nombres de clases y funciones matemáticas
namespace Mathematics {

//---------------------------------------------------------------------------
//Funciones geométricas bidimensionales:
//---------------------------------------------------------------------------

//Obtiene los factores A, B y C de la ecuación canónica
//a partir de dos puntos de la recta.
//Los factores se obtienen noramlizados, de forma que C = -d donde d es
//la distancia del origen de coordenadas a la recta.
void CanonicalEquation(const TDoublePoint& p1, const TDoublePoint& p2,
    double& A, double& B, double& C);

//Normaliza los parámetros (A, B, C) t.q. (A, B) sea un versor
void Normalize(double& A, double& B, double& C);

//---------------------------------------------------------------------------
//INTERSECCIÓN ENTRE FIGURAS ENTERAS:

//Determina si las rectas (A1, B1, C1) y (A2, B2, C2) se intersecan, en cuyo
//caso devuelve el punto de intersección r.
//Parametros de entrada:
//	A1, B1, C1: parámetros de la recta	1
//	A2, B2, C2: parámetros de la recta	2
//Valores de retorno:
//	Intersection: true: se intersecan; false: no se intersecan
//	r: punto de intersección en caso de haberlo
bool Intersection(TDoublePoint& r,
        double A1, double B1, double C1,
    double A2, double B2, double C2);

//determina si el segmento clipeante (q1, q2) interseca al segmento a
//clipear (p1, p2), en cuyo caso devuelve el punto de itnersección r.
//Esta función también devuelve la posición relativa a la recta que
//contiene el segmento clipeante de los puntos del segmento a clipear.
//Parámetros de paso:
//	p1, p2: vértices del segmento a clipear
//	q1, q2: vértices del segmento a clipeante
//Parámetros devueltos:
//	Intersectión: true si los segmentos se intersecan
//	r: punto de intersección en caso de haberlo
//	s: posición relativa de los vñertices p1 y p2 a la recta contiente
//		del segmento clipeante:
//			-2: p1 y p2 ambos a la dcha
//			-1: p1 a la izda y p2 a la dcha
//			0: p1 y p2 sobre la recta continente
//			1: p2 a la izda y p1 a la dcha
//			2: p1 y p2 ambos a la izda
bool Intersection(const TDoublePoint& p1, const TDoublePoint& p2,
    const TDoublePoint& q1, const TDoublePoint& q2,
    TDoublePoint& r, char& s);

//calcula los puntos de intersección (P1, P2)
//de la recta (A, B, C) con la circunferencia (Pc, R)
//P1 y P2 son el primer y segundo punto en el sentido de orientación de la recta
//si no hay intersección devuelve falso
bool IntersectionLineCircum(TDoublePoint &P1, TDoublePoint &P2,
        double A, double B, double C, TDoublePoint Pc, double R);
//calcula los puntos de intersección (P1, P2)
//de la recta que pasa por los puntos (Pa, Pb) con la circunferencia (Pc, R)
//P1 y P2 son el primer y segundo punto en el sentido de orientación de la recta
//si no hay intersección devuelve falso
bool IntersectionLineCircum(TDoublePoint &P1, TDoublePoint &P2,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R);
bool IntersectionCircumLine(TDoublePoint &P1, TDoublePoint &P2,
        TDoublePoint Pc, double R, TDoublePoint Pa, TDoublePoint Pb);
//determina la posición relativa de
//las circunferencias (Pc1, R1) y (Pc2, R2), y
//calcula los puntos de intersección (P1, P2) en caso de haberlos
//P1 y P2 son los puntos superior e inferior
//cuando la circunferencia mayor está a la izquierda
//devuelve:
//      0: exteriores
//      1: coincidentes
//      2: interiores concéntricas
//      3: interiores excéntricas
//      4: tangentes interiormente
//      5: tangentes exteriormente
//      6: secantes
int IntersectionCircumCircum(TDoublePoint &P1, TDoublePoint &P2,
        TDoublePoint Pc1, double R1, TDoublePoint Pc2, double R2);

//---------------------------------------------------------------------------
//INTERSECCIÓN ENTRE PARTES DE FIGURAS:

//determina si hay intersección entre los segmentos (Pa, Pb), (Qa, Qb)
//si los segmentos se intersecan calcula el punto de intersección P
//si los segmentos no se intersecan devuelve falso
bool IntersectionSegmentSegment(TDoublePoint &P,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Qa, TDoublePoint Qb);
//determina si hay intersección entre el segmento (Pa, Pb) y
//el círculo (Pc, R)
//si no hay intersección devuelve falso
bool IntersectionSegmentCircle(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Pc, double R);
//determina si hay intersección entre el segmento (Pa, Pb) y
//el arco de circunferencia (Pc, R, Pfin, Pini) en sentido levógiro
//si no hay intersección devuelve falso
bool IntersectionSegmentArc(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Pfin, TDoublePoint Pini, TDoublePoint Pc, double R);
//determina si hay intersección entre el arco (Pfin, Pini, Pc, R) y
//el segmento (Pa, Pb) en sentido levógiro
//si no hay intersección devuelve falso
bool IntersectionArcSegment(TDoublePoint Pfin, TDoublePoint Pini,
        TDoublePoint Pv, double R, TDoublePoint Pa, TDoublePoint Pb);

//determina si hay intersección entre
//el círculo (Pc1, R1) y el círculo (Pc2, R2)
bool IntersectionCircleCircle(TDoublePoint Pc1, double R1,
        TDoublePoint Pc2, double R2);

//determina si hay intersección entre
//el arco de circunferencia (Pa, Pb, Pc, R1)
//la circunferencia (Qa, R2)
bool IntersectionArcCircunference(
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R1,
        TDoublePoint Qc, double R2);
//determina si hay intersección entre
//el arco de circunferencia (Pc1, R1, Pfin1, Pini1) en sentido levógiro y
//el arco de circunferencia (Pc2, R2, Pfin2, Pini2) en sentido levógiro
//si no hay intersección devuelve falso
bool IntersectionArcArc(
        TDoublePoint Pfin1, TDoublePoint Pini1, TDoublePoint Pc1, double R1,
        TDoublePoint Pfin2, TDoublePoint Pini2, TDoublePoint Pc2, double R2);

/*//---------------------------------------------------------------------------
//INTERSECCIÓN ENTRE CIRCUNFERENCIA Y LA FRONTERA DE FIGURAS:

//calcula los puntos de intersección de la circunferencia (P, R)
//con la forntera de seguridad (cuyo margen es sm) del segmento (Pa, Pb)
//puede haber 0, 1, 2, 3 ó 4 puntos de intersección
//los puntos son añadidos a la lista Points
void IntersectionCircumBorderlineSegment(TItemList<TDoublePoint> &Points,
        TDoublePoint P, double R,
        TDoublePoint Pa, TDoublePoint Pb,
        double sm);

//calcula los puntos de intersección de la circunferencia (P, R)
//con la forntera de seguridad (cuyo margen es sm) del arco (Pa, Pb, Pc, Rp)
//puede haber 0, 1, 2, 3 ó 4 puntos de intersección
//los puntos son añadidos a la lista Points
void IntersectionCircumBorderlineArc(TItemList<TDoublePoint> &Points,
        TDoublePoint P, double R,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double Rp,
        double sm);
*/
//---------------------------------------------------------------------------
//POSICIONES RELATIVAS DE OBJETOS:

//determina si el punto P está en el subespacio comprendido
//entre las dos rectas (rectas incluidas) que pasan por Pa y Pb
//y son normales a la recta que pasa por Pa y Pb
bool PointIsInClosedStrip(TDoublePoint P, TDoublePoint Pa, TDoublePoint Pb);
//determina si el punto P está en el ángulo (lados incluidos)
//de vértice Pv y cuyos lados pasan por Pini y Pfin en sentido levógiro
bool PointIsInClosedAngle(TDoublePoint P,
        TDoublePoint Pfin, TDoublePoint Pini, TDoublePoint Pv);

//determina si el punto P está en el subespacio comprendido
//entre las dos rectas (rectas excluidas) que pasan por Pa y Pb
//y son normales a la recta que pasa por Pa y Pb
bool PointIsInOpenStrip(TDoublePoint P, TDoublePoint Pa, TDoublePoint Pb);
//determina si el punto P está en el ángulo (lados excluidos)
//de vértice Pv y cuyos lados pasan por Pini y Pfin en sentido levógiro
bool PointIsInOpenAngle(TDoublePoint P,
        TDoublePoint Pfin, TDoublePoint Pini, TDoublePoint Pv);

//determina la posición relativa del punto P
//respecto la semibanda delimitada por (Pa, Pb)
//devuelve:
//      -1: el punto P está antes de la línea que pasa por Pa
//      0: el punto P está en la banda delimitada por (Pa, Pb) y su distancia
//         a la recta que pasa por (Pa, Pb) es mayor o igual que cero
//      1: el punto P está despues d ela línea que pasa por Pb
//      2: el punto P está en la banda delimitada por (Pa, Pb) y su distancia
//         a la recta que pasa por (Pa, Pb) es menor que cero
int PointInHalfStrip(TDoublePoint P, TDoublePoint Pa, TDoublePoint Pb);

//determina si los puntos (Pa, Pb, Pc)
//son los vértices de un trilátero convexo
bool IsTrilateral(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc);
//determina si los puntos (Pa, Pb, Pc, Pd)
//son los vértices de un cuadrilátero convexo
bool IsCuadrilateral(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Pc, TDoublePoint Pd);
//determina si en (Pa, Pb, Pc)
//Pc es el vértice y Pa y Pb son puntos en los lados de un ángulo
//si Pa==Pc o Pb==Pc devuelve falso
bool IsAngle(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc);
//determina si en (Pa, Pb, Pc, R)
//Pc es el vértice y Pa y Pb son los vértices de un arco
//si Pa==Pc o Pb==Pc o R<=0, devuelve falso
//si Abs(Mod(Pa - Pc)) > ERR_NUM, devuelve falso
//si Abs(Mod(Pb - Pc)) > ERR_NUM, devuelve falso
bool IsArc(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R);

//Determina si el punto p es interior al rectángulo R
bool Inner(const TDoublePoint& p, const TDoubleRect &R);

//---------------------------------------------------------------------------
//DETERMINACIÓN DEL ÁNGULO QUE CONTIENE A UNA FIGURA:

//Determina los puntos por los que pasa el primer y segundo eje P1 y P2
//del ángulo que contiene el segmento (Pa, Pb), con vértice en Q.
//Si el punto P está sobre alguno de los ve´rtices,
//lanza una excepción EImproperArgument.
void AxisAngleSegment(TDoublePoint &P1, TDoublePoint &P2,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Q);
//Determina los puntos por los que pasa el primer y segundo eje P1 y P2
//del ángulo que contiene el arco (Pa, Pb, Pc, R), con vértice en Q.
//Si el punto P está sobre alguno de los ve´rtices,
//lanza una excepción EImproperArgument.
//Si el punto está sobre el arco, la solución son los vértices del arco
//(ordenados).
void AxisAngleArc(TDoublePoint &P1, TDoublePoint &P2,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R,
        TDoublePoint Q);

//---------------------------------------------------------------------------
//DISTANCIAS ARITMÉTICAS MÍNIMAS ENTRE PARES DE FIGURAS:

//ADVERTENCIA: la distancia aritmética puede ser negativa.

//Distancia entre el punto p y la recta (A, B, C).
//Si el punto P está a la derecha de la recta la distancia es negativa.
double Distance(const TDoublePoint P, double A, double B, double C);

//Distancia entre el punto p y la recta que pasa por (Pa, Pb).
//Si el punto P está a la derecha de la recta la distancia es negativa.
double DistanceLineToPoint(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint P);

//---------------------------------------------------------------------------
//DISTANCIAS MÍNIMAS ENTRE PARES DE FIGURAS:

//ADVERTENCIA: todas las distancias devueltas
//por estas funciones son no negativas.

//calcula la distancia mínima entre el segmento (Pa, Pb) y el punto P
double DistanceSegmentPoint(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint P);
//calcula la distancia entre el segmento (Pa, Pb) y el segmento (Qa, Qb)
double DistanceSegmentSegment(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Qa, TDoublePoint Qb);
//calcula la distancia entre el segmento (Pa, Pb) y la circunferencia (Pc, R)
double DistanceSegmentCircunference(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Pc, double R);
//calcula la distancia entre el segmento (Qa, Qb) y el arco (Pa, Pb, Pc, R)
double DistanceSegmentArc(TDoublePoint Qa, TDoublePoint Qb,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R);
//calcula la distancia entre el segmento (Pa, Pb) y el círculo (Pc, R)
double DistanceSegmentCircle(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Pc, double R);

//calcula la distancia entre el arco (Pa, Pb, Pc, R) y el punto P
double DistanceArcPoint(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc,
        double R, TDoublePoint P);
//calcula la distancia entre el arco (Pa, Pb, Pc, R) y el segmento (Qa, Qb)
double DistanceArcSegment(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc,
        double R, TDoublePoint Qa, TDoublePoint Qb);
//calcula la distancia entre el círculo (Pc, R1) y el arco (Qa, Qb, Qc, R2)
double DistanceArcCircle(
        TDoublePoint Qa, TDoublePoint Qb, TDoublePoint Qc, double R2,
        TDoublePoint Pc, double R1);
//calcula la distancia entre el arco (Pa, Pb, Pc, R1)
//y la circunferencia (Qc, R2)
double DistanceArcCircunference(
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R1,
        TDoublePoint Qc, double R2);
//calcula la distancia entre el arco (Pa1, Pb1, Pc1, R1)
//y el arco (Pa2, Pb2, Pc2, R2)
double DistanceArcArc(
        TDoublePoint Pa1, TDoublePoint Pb1, TDoublePoint Pc1, double R1,
        TDoublePoint Pa2, TDoublePoint Pb2, TDoublePoint Pc2, double R2);

//determina la distancia entre una circunferencia y un punto
double DistanceCircunferencePoint(TDoublePoint Pc, double R, TDoublePoint Q);
//calcula la distancia entre la circunferencia (P, R) y el segmento (Pa, Pb)
double DistanceCircunferenceSegment(TDoublePoint Pc, double R,
        TDoublePoint Pa, TDoublePoint Pb);
//calcula la distancia entre la circunferencia (P1, R1) y el círculo (P2, R2)
double DistanceCircunferenceCircle(TDoublePoint P1, double R1,
        TDoublePoint P2, double R2);
//calcula la distancia entre la circunferencia (P1, R1)
//y la circunferencia (P2, R2)
double DistanceCircunferenceCircunference(TDoublePoint P1, double R1,
        TDoublePoint P2, double R2);
//calcula la distancia entre la circunferencia (Qc, R2)
//y el arco (Pa, Pb, Pc, R1)
double DistanceCircunferenceArc(
        TDoublePoint Qc, double R2,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R1);

//alcula la distancia entre los círculos (Pc1, R1) y (Pc2, R2)
double DistanceCircleCircle(TDoublePoint Pc1, double R1,
        TDoublePoint Pc2, double R2);
//calcula la distancia entre el círculo (Pc, R) y el segmento (Pa, Pb)
double DistanceCircleSegment(TDoublePoint Pc, double R,
        TDoublePoint Pa, TDoublePoint Pb);
//calcula la distancia entre el círculo (P1, R1) y la circunferencia (P2, R2)
double DistanceCircleCircunference(TDoublePoint P1, double R1,
        TDoublePoint P2, double R2);
//calcula la distancia entre el círculo (Pc, R)
//y el cuadrilátero (Qa, Qb, Qc, Qd)
double DistanceCircleCuadrilateral(TDoublePoint Pc, double R,
        TDoublePoint Qa, TDoublePoint Qb, TDoublePoint Qc, TDoublePoint Qd);
//calcula la distancia entre el círculo (Pc, R1) y el arco (Qa, Qb, Qc, R2)
double DistanceCircleArc(TDoublePoint Pc, double R1,
        TDoublePoint Qa, TDoublePoint Qb, TDoublePoint Qc, double R2);

//---------------------------------------------------------------------------
//DISTANCIAS MÁXIMAS ENTRE PARES DE FIGURAS:

//ADVERTENCIA: todas las distancias devueltas
//por estas funciones son no negativas.

double DistanceCirclePointMax(TDoublePoint Pc, double R,
        TDoublePoint P);
double DistanceSegmentPointMax(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint P);
double DistanceCircunferencePointMax(TDoublePoint Pc, double R,
        TDoublePoint P);
double DistanceArcPointMax(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc,
        double R, TDoublePoint P);

//---------------------------------------------------------------------------
//INTERSECCIÓN ENTRE PUNTO Y FIGURAS:

//para triláteros convexos dados por los vértices en sentido dextrógiro
//determina si hay intersección entre el punto P
//y el trilátero (Pa, Pb, Pc)
//si el punto no toca al trilátero devuelve falso
//si los puntos (Pa, Pb, Pc) no son los vértices del trilátero
//recorrido en sentido dextrógiro, esta función no indica intersección
bool IntersectionPointTrilateral(TDoublePoint P,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc);

//para cuadriláteros convexos dados por los vértices en sentido dextrógiro
//determina si hay intersección entre el punto P
//y el cuadrilátero (Pa, Pb, Pc, Pb)
//si el punto no toca al cuadrilátero devuelve falso
//si los puntos (Pa, Pb, Pc, Pd) no son los vértices del cuadrilátero
//recorrido en sentido dextrógiro, esta función no indica intersección
bool IntersectionPointCuadrilateral(TDoublePoint P,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, TDoublePoint Pd);

//determina si hay intersección entre el punto O y el círculo (Pc, R)
//si el punto no toca al círculo devuelve falso
bool IntersectionPointCircle(TDoublePoint P, TDoublePoint Pc, double R);

//determina si hay intersección entre el punto P
//y la porción de círculo (Pfin, Pini, Pc, R) en sentido levógiro
//si el punto no toca a la porción devuelve falso
bool IntersectionPointPie(TDoublePoint P,
        TDoublePoint Pfin, TDoublePoint Pini, TDoublePoint Pc, double R);

//---------------------------------------------------------------------------
//Funciones geométricas tridimensionales:
//---------------------------------------------------------------------------

/*
//Sea el sistema de versores ortogonales (u, v, w),
//dado el vector A en la dirección de u, y el vector C en el plano (u, w),
//calcular el versor u',
//resultado de girar el vector u un ángulo theta en torno de w.
//Es decir, despeja u' en la ecuación:
//      u x u' = sin(theta)*w
//donde:
//      u = A/Mod(A)
//      w = C/Mod(C)
Solve(double &u_x, double &u_y, double &u_z,
        double Ax, double Ay, double Az,
        double Cx, double Cy, double Cz,
        double theta);            */

//determina la distancia de un punto a un plano en el espacio 3D
double DistancePointPlane(double x, double y, double z,
        double A, double B, double C, double D);

//---------------------------------------------------------------------------

} //namespace Mathematics

//---------------------------------------------------------------------------
#endif // GEOMETRY_H
