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
//File: testGeometry.cpp
//Content: test for the module Geometry
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testGeometry.h"
#include "../src/Geometry.h"
//#include "cppunit_assert_emulator.h" //Qt only.

#include <iostream>
#include <limits.h> //INT_MAX
#include <float.h> //DBL_MAX

using namespace Mathematics;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestGeometry);

//overide setUp(), init data, etc
void TestGeometry::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestGeometry::tearDown() {
}

//---------------------------------------------------------------------------

void TestGeometry::test_()
{
    //---------------------------------------------------------------------------
    //Funciones geométricas bidimensionales:
    //---------------------------------------------------------------------------

    //Obtiene los factores A, B y C de la ecuación canónica
    //a partir de dos puntos de la recta.
    //Los factores se obtienen noramlizados, de forma que C = -d donde d es
    //la distancia del origen de coordenadas a la recta.
    TDoublePoint p1(0, 1);
    TDoublePoint p2(1, 1);
    double A, B, C;
    canonicalEquation(p1, p2, A, B, C);
    if(A != 0 || B != 1 || C != -1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //Normaliza los parámetros (A, B, C) t.q. (A, B) sea un versor
    for(int i=0; i<1000; i++) {
        //genera un vector aleatorio donde (A, B) es no nulo
        do {
            A = RandomUniform(-1000, 1000);
            B = RandomUniform(-1000, 1000);
            C = RandomUniform(-1000, 1000);
        } while(A==0 && B==0);

        //normaliza los parámetros
        normalize(A, B, C);

        //comprueba que (A, B) es un versor
        double error = sqrt(A*A + B*B) - 1;
        if(error > ERR_NUM) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

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
    double A1 = -1;
    double B1 = 1;
    double C1 = -1;
    double A2 = 1;
    double B2 = 1;
    double C2 = -1;
    TDoublePoint r;
    bool b = intersection(r, A1, B1, C1, A2, B2, C2);
    if(b != true || r != TDoublePoint(0, 1)) {
        CPPUNIT_ASSERT(false);
        return;
    }

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
    //	s: posición relativa de los vértices p1 y p2 a la recta contiente
    //		del segmento clipeante:
    //			-2: p1 y p2 ambos a la dcha
    //			-1: p1 a la izda y p2 a la dcha
    //			0: p1 y p2 sobre la recta continente
    //			1: p2 a la izda y p1 a la dcha
    //			2: p1 y p2 ambos a la izda
    p1 = TDoublePoint(-1, -1);
    p2 = TDoublePoint(1, 1);
    TDoublePoint q1(-1, 1);
    TDoublePoint q2(1, -1);
    r = TDoublePoint(0, 0);
    char s;
    b = intersection(p1, p2, q1, q2, r, s);
    if(b != true || r != TDoublePoint(0, 0) || s != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //calcula los puntos de intersección (P1, P2)
    //de la recta (A, B, C) con la circunferencia (Pc, R)
    //P1 y P2 son el primer y segundo punto en el sentido de orientación de la recta
    //si no hay intersección devuelve falso
    TDoublePoint P1, P2;
    A = 1;
    B = 1;
    C = -1;
    TDoublePoint Pc(0, 0);
    double R = 1;
    b = intersectionLineCircum(P1, P2, A, B, C, Pc, R);
    if(b != true || fabs(P1.x - 0) > ERR_NUM || fabs(P1.y - 1) > ERR_NUM || fabs(P2.x - 1) > ERR_NUM || fabs(P2.y - 0) > ERR_NUM) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //calcula los puntos de intersección (P1, P2)
    //de la recta que pasa por los puntos (Pa, Pb) con la circunferencia (Pc, R)
    //P1 y P2 son el primer y segundo punto en el sentido de orientación de la recta
    //si no hay intersección devuelve falso
    P1 = TDoublePoint(0, 0);
    P1 = TDoublePoint(0, 0);
    TDoublePoint Pa(-1, 0);
    TDoublePoint Pb(1, 0);
    Pc = TDoublePoint(0, 0);
    R = 1;
    b = intersectionLineCircum(P1, P2, Pa, Pb, Pc, R);
    if(b != true || fabs(P1.x + 1) > ERR_NUM || fabs(P1.y - 0) > ERR_NUM || fabs(P2.x - 1) > ERR_NUM || fabs(P2.y - 0) > ERR_NUM) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P1 = TDoublePoint(0, 0);
    P1 = TDoublePoint(0, 0);
    b = intersectionCircumLine(P1, P2, Pc, R, Pa, Pb);
    if(b != true || fabs(P1.x + 1) > ERR_NUM || fabs(P1.y - 0) > ERR_NUM || fabs(P2.x - 1) > ERR_NUM || fabs(P2.y - 0) > ERR_NUM) {
        CPPUNIT_ASSERT(false);
        return;
    }
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
    P1 = TDoublePoint(0, 0);
    P2 = TDoublePoint(0, 0);
    TDoublePoint Pc1(-1, 0);
    double R1 = 0.99;
    TDoublePoint Pc2(1, 0);
    double R2 = 0.99;
    int n = intersectionCircumCircum(P1, P2, Pc1, R1, Pc2, R2);
    if(n != 0 || P1 != TDoublePoint(0, 0) || P2 != TDoublePoint(0, 0)) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P1 = TDoublePoint(0, 0);
    P2 = TDoublePoint(0, 0);
    Pc1 = TDoublePoint(0, 0);
    R1 = 0.99;
    Pc2 = TDoublePoint(0, 0);
    R2 = 0.99;
    n = intersectionCircumCircum(P1, P2, Pc1, R1, Pc2, R2);
    if(n != 1 || P1 != TDoublePoint(0, 0) || P2 != TDoublePoint(0, 0)) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P1 = TDoublePoint(0, 0);
    P2 = TDoublePoint(0, 0);
    Pc1 = TDoublePoint(0, 0);
    R1 = 1;
    Pc2 = TDoublePoint(0, 0);
    R2 = 0.1;
    n = intersectionCircumCircum(P1, P2, Pc1, R1, Pc2, R2);
    if(n != 2 || P1 != TDoublePoint(0, 0) || P2 != TDoublePoint(0, 0)) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P1 = TDoublePoint(0, 0);
    P2 = TDoublePoint(0, 0);
    Pc1 = TDoublePoint(0, 0);
    R1 = 1;
    Pc2 = TDoublePoint(0.5, 0);
    R2 = 0.1;
    n = intersectionCircumCircum(P1, P2, Pc1, R1, Pc2, R2);
    if(n != 3 || P1 != TDoublePoint(0, 0) || P2 != TDoublePoint(0, 0)) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P1 = TDoublePoint(0, 0);
    P2 = TDoublePoint(0, 0);
    Pc1 = TDoublePoint(0, 0);
    R1 = 1;
    Pc2 = TDoublePoint(0.9, 0);
    R2 = 0.1;
    n = intersectionCircumCircum(P1, P2, Pc1, R1, Pc2, R2);
    if(n != 4 || fabs(P1.x - 1) > ERR_NUM || fabs(P1.y - 0) > ERR_NUM || fabs(P2.x - 1) > ERR_NUM || fabs(P2.y - 0) > ERR_NUM) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P1 = TDoublePoint(0, 0);
    P2 = TDoublePoint(0, 0);
    Pc1 = TDoublePoint(-1, 0);
    R1 = 1;
    Pc2 = TDoublePoint(1, 0);
    R2 = 1;
    n = intersectionCircumCircum(P1, P2, Pc1, R1, Pc2, R2);
    if(n != 5 || fabs(P1.x - 0) > ERR_NUM || fabs(P1.y - 0) > ERR_NUM || fabs(P2.x - 0) > ERR_NUM || fabs(P2.y - 0) > ERR_NUM) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P1 = TDoublePoint(0, 0);
    P2 = TDoublePoint(0, 0);
    Pc1 = TDoublePoint(-1, 0);
    R1 = sqrt(2.);
    Pc2 = TDoublePoint(1, 0);
    R2 = sqrt(2.);
    n = intersectionCircumCircum(P1, P2, Pc1, R1, Pc2, R2);
    if(n != 6 || fabs(P1.x - 0) > ERR_NUM || fabs(P1.y - 1) > ERR_NUM || fabs(P2.x - 0) > ERR_NUM || fabs(P2.y + 1) > ERR_NUM) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //---------------------------------------------------------------------------
    //INTERSECCIÓN ENTRE PARTES DE FIGURAS:

    //determina si hay intersección entre los segmentos (Pa, Pb), (Qa, Qb)
    //si los segmentos se intersecan calcula el punto de intersección P
    //si los segmentos no se intersecan devuelve falso
    TDoublePoint P;
    Pa = TDoublePoint(-1, 0);
    Pb = TDoublePoint(0, 0.999);
    TDoublePoint Qa(1, 0);
    TDoublePoint Qb(0, 1);
    b = intersectionSegmentSegment(P, Pa, Pb, Qa, Qb);
    if(b != false || P != TDoublePoint(0, 0)) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P = TDoublePoint(0, 0);
    Pa = TDoublePoint(-1, 0);
    Pb = TDoublePoint(0, 1);
    Qa = TDoublePoint(1, 0);
    Qb = TDoublePoint(0, 1);
    b = intersectionSegmentSegment(P, Pa, Pb, Qa, Qb);
    if(b != false || P != TDoublePoint(0, 0)) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P = TDoublePoint(0, 0);
    Pa = TDoublePoint(-1, 0);
    Pb = TDoublePoint(0, 1);
    Qa = TDoublePoint(0, 1);
    Qb = TDoublePoint(1, 0);
    b = intersectionSegmentSegment(P, Pa, Pb, Qa, Qb);
    if(b != false || P != TDoublePoint(0, 0)) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P = TDoublePoint(0, 0);
    Pa = TDoublePoint(-1, 1);
    Pb = TDoublePoint(1, -1);
    Qa = TDoublePoint(-1, -1);
    Qb = TDoublePoint(1, 1);
    b = intersectionSegmentSegment(P, Pa, Pb, Qa, Qb);
    if(b != true || P != TDoublePoint(0, 0)) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P = TDoublePoint(0, 0);
    Pa = TDoublePoint(0, 1);
    Pb = TDoublePoint(1, 1);
    Qa = TDoublePoint(1, 0);
    Qb = TDoublePoint(1, 1);
    b = intersectionSegmentSegment(P, Pa, Pb, Qa, Qb);
    if(b != false || P != TDoublePoint(0, 0)) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P = TDoublePoint(0, 0);
    Pa = TDoublePoint(0, 0.5);
    Pb = TDoublePoint(1, 0.5);
    Qa = TDoublePoint(0.5, 0);
    Qb = TDoublePoint(0.5, 1);
    b = intersectionSegmentSegment(P, Pa, Pb, Qa, Qb);
    if(b != true || P != TDoublePoint(0.5, 0.5)) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //determina si hay intersección entre el segmento (Pa, Pb) y
    //el círculo (Pc, R)
    //si no hay intersección devuelve falso
    Pa = TDoublePoint(0.01, 1);
    Pb = TDoublePoint(1, 1);
    Pc = TDoublePoint(-1, 1);
    R = 1;
    b = intersectionSegmentCircle(Pa, Pb, Pc, R);
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }
    Pa = TDoublePoint(0, 1);
    Pb = TDoublePoint(1, 1);
    Pc = TDoublePoint(-1, 1);
    R = 1;
    b = intersectionSegmentCircle(Pa, Pb, Pc, R);
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }
    Pa = TDoublePoint(-0.01, 1);
    Pb = TDoublePoint(1, 1);
    Pc = TDoublePoint(-1, 1);
    R = 1;
    b = intersectionSegmentCircle(Pa, Pb, Pc, R);
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //determina si hay intersección entre el segmento (Pa, Pb) y
    //el arco de circunferencia (Pc, R, Pfin, Pini) en sentido levógiro
    //si no hay intersección devuelve falso
    Pa = TDoublePoint(0, 1);
    Pb = TDoublePoint(1, 1);
    TDoublePoint Pfin(-1, 2);
    TDoublePoint Pini(-1, 0);
    Pc = TDoublePoint(-1, 1);
    R = 1;
    b = intersectionSegmentArc(Pa, Pb, Pfin, Pini, Pc, R);
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }
    Pa = TDoublePoint(-0.01, 1);
    Pb = TDoublePoint(1, 1);
    Pfin = TDoublePoint(-1, 2);
    Pini = TDoublePoint(-1, 0);
    Pc = TDoublePoint(-1, 1);
    R = 1;
    b = intersectionSegmentArc(Pa, Pb, Pfin, Pini, Pc, R);
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //determina si hay intersección entre el arco (Pfin, Pini, Pc, R) y
    //el segmento (Pa, Pb) en sentido levógiro
    //si no hay intersección devuelve falso
    Pfin = TDoublePoint(-1, 2);
    Pini = TDoublePoint(-1, 0);
    Pc = TDoublePoint(-1, 1);
    R = 1;
    Pa = TDoublePoint(0, 1);
    Pb = TDoublePoint(1, 1);
    b = intersectionArcSegment(Pfin, Pini, Pc, R, Pa, Pb);
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }
    Pfin = TDoublePoint(-1, 2);
    Pini = TDoublePoint(-1, 0);
    Pc = TDoublePoint(-1, 1);
    R = 1;
    Pa = TDoublePoint(-0.01, 1);
    Pb = TDoublePoint(1, 1);
    b = intersectionArcSegment(Pfin, Pini, Pc, R, Pa, Pb);
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //determina si hay intersección entre
    //el círculo (Pc1, R1) y el círculo (Pc2, R2)
    Pc1 = TDoublePoint(0, -1.001);
    R1 = 1;
    Pc2 = TDoublePoint(0, 1);
    R2 = 1;
    b = intersectionCircleCircle(Pc1, R1, Pc2, R2);
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }
    Pc1 = TDoublePoint(0, -1);
    R1 = 1;
    Pc2 = TDoublePoint(0, 1);
    R2 = 1;
    b = intersectionCircleCircle(Pc1, R1, Pc2, R2);
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }
    Pc1 = TDoublePoint(1, 0);
    R1 = 1;
    Pc2 = TDoublePoint(0, 1);
    R2 = 1;
    b = intersectionCircleCircle(Pc1, R1, Pc2, R2);
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //determina si hay intersección entre
    //el arco de circunferencia (Pa, Pb, Pc, R1)
    //la circunferencia (Qa, R2)
    Pa = TDoublePoint(-1, 2);
    Pb = TDoublePoint(-1, 0);
    Pc = TDoublePoint(-1, 1);
    R1 = 1;
    TDoublePoint Qc(1.01, 1);
    R2 = 1;
    b = intersectionArcCircunference(Pa, Pb, Pc, R1, Qc, R2);
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }
    Pa = TDoublePoint(-1, 2);
    Pb = TDoublePoint(-1, 0);
    Pc = TDoublePoint(-1, 1);
    R1 = 1;
    Qc = TDoublePoint(1, 1);
    R2 = 1;
    b = intersectionArcCircunference(Pa, Pb, Pc, R1, Qc, R2);
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //determina si hay intersección entre
    //el arco de circunferencia (Pc1, R1, Pfin1, Pini1) en sentido levógiro y
    //el arco de circunferencia (Pc2, R2, Pfin2, Pini2) en sentido levógiro
    //si no hay intersección devuelve falso
    TDoublePoint Pa1(-1, 2);
    TDoublePoint Pb1(-1, 0);
    Pc1 = TDoublePoint(-1, 1);
    R1 = 1;
    TDoublePoint Pa2(1.01, 0);
    TDoublePoint Pb2(1.01, 2);
    Pc2 = TDoublePoint(1.01, 1);
    R2 = 1;
    b = intersectionArcArc(Pa1, Pb1, Pc1, R1, Pa2, Pb2, Pc2, R2);
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }
    Pa1 = TDoublePoint(-1, 2);
    Pb1 = TDoublePoint(-1, 0);
    Pc1 = TDoublePoint(-1, 1);
    R1 = 1;
    Pa2 = TDoublePoint(1, 0);
    Pb2 = TDoublePoint(1, 2);
    Pc2 = TDoublePoint(1, 1);
    R2 = 1;
    b = intersectionArcArc(Pa1, Pb1, Pc1, R1, Pa2, Pb2, Pc2, R2);
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //---------------------------------------------------------------------------
    //POSICIONES RELATIVAS DE OBJETOS:

    //determina si el punto P está en el subespacio comprendido
    //entre las dos rectas (rectas incluidas) que pasan por Pa y Pb
    //y son normales a la recta que pasa por Pa y Pb
    P = TDoublePoint(-0.01, 2);
    Pa = TDoublePoint(-1, 1);
    Pb = TDoublePoint(1, -1);
    b = pointIsInClosedStrip(P, Pa, Pb);
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P = TDoublePoint(0, 2);
    Pa = TDoublePoint(-1, 1);
    Pb = TDoublePoint(1, -1);
    b = pointIsInClosedStrip(P, Pa, Pb);
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P = TDoublePoint(0.01, 2);
    Pa = TDoublePoint(-1, 1);
    Pb = TDoublePoint(1, -1);
    b = pointIsInClosedStrip(P, Pa, Pb);
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //determina si el punto P está en el ángulo (lados incluidos)
    //de vértice Pv y cuyos lados pasan por Pini y Pfin en sentido levógiro
    P = TDoublePoint(-0.01, 1);
    Pa = TDoublePoint(0, 1);
    Pb = TDoublePoint(1, 0);
    Pc = TDoublePoint(0, 0);
    b = pointIsInClosedAngle(P, Pa, Pb, Pc);
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P = TDoublePoint(0, 1);
    Pa = TDoublePoint(0, 1);
    Pb = TDoublePoint(1, 0);
    Pc = TDoublePoint(0, 0);
    b = pointIsInClosedAngle(P, Pa, Pb, Pc);
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P = TDoublePoint(0.01, 1);
    Pa = TDoublePoint(0, 1);
    Pb = TDoublePoint(1, 0);
    Pc = TDoublePoint(0, 0);
    b = pointIsInClosedAngle(P, Pa, Pb, Pc);
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //determina si el punto P está en el subespacio comprendido
    //entre las dos rectas (rectas excluidas) que pasan por Pa y Pb
    //y son normales a la recta que pasa por Pa y Pb
    P = TDoublePoint(-0.01, 2);
    Pa = TDoublePoint(-1, 1);
    Pb = TDoublePoint(1, -1);
    b = pointIsInOpenStrip(P, Pa, Pb);
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P = TDoublePoint(0, 2);
    Pa = TDoublePoint(-1, 1);
    Pb = TDoublePoint(1, -1);
    b = pointIsInOpenStrip(P, Pa, Pb);
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P = TDoublePoint(0.01, 2);
    Pa = TDoublePoint(-1, 1);
    Pb = TDoublePoint(1, -1);
    b = pointIsInOpenStrip(P, Pa, Pb);
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //determina si el punto P está en el ángulo (lados excluidos)
    //de vértice Pv y cuyos lados pasan por Pini y Pfin en sentido levógiro
    P = TDoublePoint(-0.01, 1);
    Pa = TDoublePoint(0, 1);
    Pb = TDoublePoint(1, 0);
    Pc = TDoublePoint(0, 0);
    b = pointIsInOpenAngle(P, Pa, Pb, Pc);
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P = TDoublePoint(0, 1);
    Pa = TDoublePoint(0, 1);
    Pb = TDoublePoint(1, 0);
    Pc = TDoublePoint(0, 0);
    b = pointIsInOpenAngle(P, Pa, Pb, Pc);
    if(b != false) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P = TDoublePoint(0.01, 1);
    Pa = TDoublePoint(0, 1);
    Pb = TDoublePoint(1, 0);
    Pc = TDoublePoint(0, 0);
    b = pointIsInOpenAngle(P, Pa, Pb, Pc);
    if(b != true) {
        CPPUNIT_ASSERT(false);
        return;
    }
/*
    //determina la posición relativa del punto P
    //respecto la semibanda delimitada por (Pa, Pb)
    //devuelve:
    //      -1: el punto P está antes de la línea que pasa por Pa
    //      0: el punto P está en la banda delimitada por (Pa, Pb) y su distancia
    //         a la recta que pasa por (Pa, Pb) es mayor o igual que cero
    //      1: el punto P está despues d ela línea que pasa por Pb
    //      2: el punto P está en la banda delimitada por (Pa, Pb) y su distancia
    //         a la recta que pasa por (Pa, Pb) es menor que cero
    int pointInHalfStrip(TDoublePoint P, TDoublePoint Pa, TDoublePoint Pb);

    //determina si los puntos (Pa, Pb, Pc)
    //son los vértices de un trilátero convexo
    bool isTrilateral(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc);
    //determina si los puntos (Pa, Pb, Pc, Pd)
    //son los vértices de un cuadrilátero convexo
    bool isCuadrilateral(TDoublePoint Pa, TDoublePoint Pb,
                         TDoublePoint Pc, TDoublePoint Pd);
    //determina si en (Pa, Pb, Pc)
    //Pc es el vértice y Pa y Pb son puntos en los lados de un ángulo
    //si Pa==Pc o Pb==Pc devuelve falso
    bool isAngle(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc);
    //determina si en (Pa, Pb, Pc, R)
    //Pc es el vértice y Pa y Pb son los vértices de un arco
    //si Pa==Pc o Pb==Pc o R<=0, devuelve falso
    //si Abs(Mod(Pa - Pc)) > ERR_NUM, devuelve falso
    //si Abs(Mod(Pb - Pc)) > ERR_NUM, devuelve falso
    bool isArc(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R);

    //Determina si el punto p es interior al rectángulo R
    bool inner(const TDoublePoint& p, const TDoubleRect &R);

    //---------------------------------------------------------------------------
    //DETERMINACIÓN DEL ÁNGULO QUE CONTIENE A UNA FIGURA:

    //Determina los puntos por los que pasa el primer y segundo eje P1 y P2
    //del ángulo que contiene el segmento (Pa, Pb), con vértice en Q.
    //Si el punto P está sobre alguno de los ve´rtices,
    //lanza una excepción EImproperArgument.
    void axisAngleSegment(TDoublePoint &P1, TDoublePoint &P2,
                          TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Q);
    //Determina los puntos por los que pasa el primer y segundo eje P1 y P2
    //del ángulo que contiene el arco (Pa, Pb, Pc, R), con vértice en Q.
    //Si el punto P está sobre alguno de los ve´rtices,
    //lanza una excepción EImproperArgument.
    //Si el punto está sobre el arco, la solución son los vértices del arco
    //(ordenados).
    void axisAngleArc(TDoublePoint &P1, TDoublePoint &P2,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R,
        TDoublePoint Q);

    //---------------------------------------------------------------------------
    //DISTANCIAS ARITMÉTICAS MÍNIMAS ENTRE PARES DE FIGURAS:

    //ADVERTENCIA: la distancia aritmética puede ser negativa.

    //Distancia entre el punto p y la recta (A, B, C).
    //Si el punto P está a la derecha de la recta la distancia es negativa.
    double distance(const TDoublePoint P, double A, double B, double C);

    //Distancia entre el punto p y la recta que pasa por (Pa, Pb).
    //Si el punto P está a la derecha de la recta la distancia es negativa.
    double distanceLineToPoint(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint P);

    //---------------------------------------------------------------------------
    //DISTANCIAS MÍNIMAS ENTRE PARES DE FIGURAS:

    //ADVERTENCIA: todas las distancias devueltas
    //por estas funciones son no negativas.

    //calcula la distancia mínima entre el segmento (Pa, Pb) y el punto P
    double distanceSegmentPoint(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint P);
    //calcula la distancia entre el segmento (Pa, Pb) y el segmento (Qa, Qb)
    double distanceSegmentSegment(TDoublePoint Pa, TDoublePoint Pb,
                                  TDoublePoint Qa, TDoublePoint Qb);
    //calcula la distancia entre el segmento (Pa, Pb) y la circunferencia (Pc, R)
    double distanceSegmentCircunference(TDoublePoint Pa, TDoublePoint Pb,
                                        TDoublePoint Pc, double R);
    //calcula la distancia entre el segmento (Qa, Qb) y el arco (Pa, Pb, Pc, R)
    double distanceSegmentArc(TDoublePoint Qa, TDoublePoint Qb,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R);
    //calcula la distancia entre el segmento (Pa, Pb) y el círculo (Pc, R)
    double distanceSegmentCircle(TDoublePoint Pa, TDoublePoint Pb,
                                 TDoublePoint Pc, double R);

    //calcula la distancia entre el arco (Pa, Pb, Pc, R) y el punto P
    double distanceArcPoint(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc,
                            double R, TDoublePoint P);
    //calcula la distancia entre el arco (Pa, Pb, Pc, R) y el segmento (Qa, Qb)
    double distanceArcSegment(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc,
                              double R, TDoublePoint Qa, TDoublePoint Qb);
    //calcula la distancia entre el círculo (Pc, R1) y el arco (Qa, Qb, Qc, R2)
    double distanceArcCircle(
            TDoublePoint Qa, TDoublePoint Qb, TDoublePoint Qc, double R2,
            TDoublePoint Pc, double R1);
    //calcula la distancia entre el arco (Pa, Pb, Pc, R1)
    //y la circunferencia (Qc, R2)
    double distanceArcCircunference(
            TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R1,
            TDoublePoint Qc, double R2);
    //calcula la distancia entre el arco (Pa1, Pb1, Pc1, R1)
    //y el arco (Pa2, Pb2, Pc2, R2)
    double distanceArcArc(
            TDoublePoint Pa1, TDoublePoint Pb1, TDoublePoint Pc1, double R1,
            TDoublePoint Pa2, TDoublePoint Pb2, TDoublePoint Pc2, double R2);

    //determina la distancia entre una circunferencia y un punto
    double distanceCircunferencePoint(TDoublePoint Pc, double R, TDoublePoint Q);
    //calcula la distancia entre la circunferencia (P, R) y el segmento (Pa, Pb)
    double distanceCircunferenceSegment(TDoublePoint Pc, double R,
                                        TDoublePoint Pa, TDoublePoint Pb);
    //calcula la distancia entre la circunferencia (P1, R1) y el círculo (P2, R2)
    double distanceCircunferenceCircle(TDoublePoint P1, double R1,
                                       TDoublePoint P2, double R2);
    //calcula la distancia entre la circunferencia (P1, R1)
    //y la circunferencia (P2, R2)
    double distanceCircunferenceCircunference(TDoublePoint P1, double R1,
                                              TDoublePoint P2, double R2);
    //calcula la distancia entre la circunferencia (Qc, R2)
    //y el arco (Pa, Pb, Pc, R1)
    double distanceCircunferenceArc(
            TDoublePoint Qc, double R2,
            TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R1);

    //alcula la distancia entre los círculos (Pc1, R1) y (Pc2, R2)
    double distanceCircleCircle(TDoublePoint Pc1, double R1,
                                TDoublePoint Pc2, double R2);
    //calcula la distancia entre el círculo (Pc, R) y el segmento (Pa, Pb)
    double distanceCircleSegment(TDoublePoint Pc, double R,
                                 TDoublePoint Pa, TDoublePoint Pb);
    //calcula la distancia entre el círculo (P1, R1) y la circunferencia (P2, R2)
    double distanceCircleCircunference(TDoublePoint P1, double R1,
                                       TDoublePoint P2, double R2);
    //calcula la distancia entre el círculo (Pc, R)
    //y el cuadrilátero (Qa, Qb, Qc, Qd)
    double distanceCircleCuadrilateral(TDoublePoint Pc, double R,
        TDoublePoint Qa, TDoublePoint Qb, TDoublePoint Qc, TDoublePoint Qd);
    //calcula la distancia entre el círculo (Pc, R1) y el arco (Qa, Qb, Qc, R2)
    double distanceCircleArc(TDoublePoint Pc, double R1,
        TDoublePoint Qa, TDoublePoint Qb, TDoublePoint Qc, double R2);

    //---------------------------------------------------------------------------
    //DISTANCIAS MÁXIMAS ENTRE PARES DE FIGURAS:

    //ADVERTENCIA: todas las distancias devueltas
    //por estas funciones son no negativas.

    double distanceCirclePointMax(TDoublePoint Pc, double R,
                                  TDoublePoint P);
    double distanceSegmentPointMax(TDoublePoint Pa, TDoublePoint Pb,
                                   TDoublePoint P);
    double distanceCircunferencePointMax(TDoublePoint Pc, double R,
                                         TDoublePoint P);
    double distanceArcPointMax(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc,
                               double R, TDoublePoint P);

    //---------------------------------------------------------------------------
    //INTERSECCIÓN ENTRE PUNTO Y FIGURAS:

    //para triláteros convexos dados por los vértices en sentido dextrógiro
    //determina si hay intersección entre el punto P
    //y el trilátero (Pa, Pb, Pc)
    //si el punto no toca al trilátero devuelve falso
    //si los puntos (Pa, Pb, Pc) no son los vértices del trilátero
    //recorrido en sentido dextrógiro, esta función no indica intersección
    bool intersectionPointTrilateral(TDoublePoint P,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc);

    //para cuadriláteros convexos dados por los vértices en sentido dextrógiro
    //determina si hay intersección entre el punto P
    //y el cuadrilátero (Pa, Pb, Pc, Pb)
    //si el punto no toca al cuadrilátero devuelve falso
    //si los puntos (Pa, Pb, Pc, Pd) no son los vértices del cuadrilátero
    //recorrido en sentido dextrógiro, esta función no indica intersección
    bool intersectionPointCuadrilateral(TDoublePoint P,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, TDoublePoint Pd);

    //determina si hay intersección entre el punto O y el círculo (Pc, R)
    //si el punto no toca al círculo devuelve falso
    bool intersectionPointCircle(TDoublePoint P, TDoublePoint Pc, double R);

    //determina si hay intersección entre el punto P
    //y la porción de círculo (Pfin, Pini, Pc, R) en sentido levógiro
    //si el punto no toca a la porción devuelve falso
    bool intersectionPointPie(TDoublePoint P,
        TDoublePoint Pfin, TDoublePoint Pini, TDoublePoint Pc, double R);

    //---------------------------------------------------------------------------
    //Funciones geométricas tridimensionales:
    //---------------------------------------------------------------------------

    //determina la distancia de un punto a un plano en el espacio 3D
    double distancePointPlane(double x, double y, double z,
                              double A, double B, double C, double D);
*/
    //---------------------------------------------------------------------------

    CPPUNIT_ASSERT(true);
}
