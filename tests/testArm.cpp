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
//File: testArm.cpp
//Content: test for the module Arm
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testArm.h"
#include "../src/Constants.h" //M_2PI
//#include "cppunit_assert_emulator.h" //Qt only.

#include <cmath> //abs
#include <vector>
//#include <stdlib.h>
#include <limits> //std::numeric_limits

#include "../src/Arm.h"

//using namespace Lists;
using namespace Models;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestArm);

//overide setUp(), init data, etc
void TestArm::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestArm::tearDown() {
}

//---------------------------------------------------------------------------

void TestArm::test_()
{
    for(int i=0; i<10; i++) {
        try {
            //construye un brazo
            TArm *A = new TArm(TDoublePoint(5.8025, 0));

            //comrpueba que le punto P3 está donde debe estar
            if(Mod(A->getP3() - TDoublePoint(0, 0)) > ERR_NUM) {
                CPPUNIT_ASSERT(false);
                return;
            }

            //libera la memoria dinámica
            delete A;

        } catch(...) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //-------------------------------------------------------------------
    //MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

    //construye un brazo
    //con la posición y orientación indicadas
    TArm A1(TDoublePoint(-5.8025, 0), 0);

    //contruye un clon de un brazo
    TArm A2(&A1);

    //comprueba todas las propiedades
    if(A1.Quantify___s.getCount() != 0 || A2.Quantify___s.getCount() != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(A1.getL12() != A2.getL12() ||
            A1.getP____2() != A2.getP____2() ||
            A1.getL13() != A2.getL13() ||
            A1.gettheta____3() != A2.gettheta____3() ||
            A1.getP____3() != A2.getP____3() ||
            A1.getR3() != A2.getR3() ||
            A1.getContour____() != A2.getContour____() ||
            A1.getL1V() != A2.getL1V() ||
            A1.getV____() != A2.getV____() ||
            A1.getF() != A2.getF() ||
            A1.getQuantify___() != A2.getQuantify___() ||
            //A1.Quantify___s != A2.Quantify___s || //no puede ser comparado directamente
            A1.getG() != A2.getG() ||
            A1.getSB2() != A2.getSB2() ||
            A1.getrbs() != A2.getrbs() ||
            A1.getQ() != A2.getQ() ||
            A1.getthetaO3() != A2.getthetaO3() ||
            A1.gettheta___3() != A2.gettheta___3() ||
            A1.gettheta___3min() != A2.gettheta___3min() ||
            A1.gettheta___3max() != A2.gettheta___3max() ||
            A1.theta___3s != A2.theta___3s ||
            A1.getP1() != A2.getP1() ||
            A1.getP2() != A2.getP2() ||
            A1.getP3() != A2.getP3() ||
            A1.getV() != A2.getV() ||
            A1.getContour() != A2.getContour()) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //Propiedades de la instancia:
    //  L12
    //  L13
    //  theta__O3
    //  R3
    //  thetaO3
    //  theta___3min
    //  theta___3max
    //  theta___3
    //  SB2

    /*    //-------------------------------------------------------------------

    //coloca el brazo A2 en frente del brazo A1
    A2.set(TDoublePoint(5.8025, 0), -M_PI);

    //comrpueba que le punto P3 está donde debe estar
    if(Mod(A2.getP3() - TDoublePoint(0, 0)) > ERR_NUM) {
        CPPUNIT_ASSERT(false);
        return;
    }
*/
    //-------------------------------------------------------------------
    //MÉTODOS ESTÁTICOS:

    //lee una instancia de brazo en una cadena
    AnsiString S;
    S += "\r\nL12 = 5.8025";
    S += "\r\nL13 = 5.8025";
    S += "\r\ntheta__O3 = 0";
    S += "\r\nR3 = 0.75";
    S += "\r\ntheta___3min = -0.043633231";
    S += "\r\ntheta___3max = 3.185225885";
    S += "\r\ntheta___3 = 0";
    S += "\r\nSB2 = 30720";
    int i = 1;
    TArm::readInstance(&A2, S, i);

    //Nótese que en la función estática TArm::readInstance,
    //cuando es asignado SB2, es determinada y procesada F,
    //produciendose una recalculación de SB2 y pudiendo
    //incorporar un pequeño error numérico.

    if(A2.getL12() != 5.8025 ||
            A2.getL13() != 5.8025 ||
            A2.gettheta__O3() != 0 ||
            A2.getR3() != 0.75 ||
            A2.gettheta___3min() != -0.043633231 ||
            A2.gettheta___3max() != 3.185225885 ||
            A2.gettheta___3() != 0 ||
            abs(A2.getSB2() - 30720) > ERR_NUM) {
        CPPUNIT_ASSERT(false);
        return;
    }


    /*    //-------------------------------------------------------------------
    //MÉTODOS DE TRANSFORMACIÓN ENTRE SISTEMAS DE COORDENADAS:

    //transforma la coordenada angular polar de S3 a S2
    //      theta__ = theta___ + theta__O3
    double S3thetaToS2theta(double theta___) const;
    //transforma las coordenadas polares en S2
    //en coordenadas rectangulares en S2
    TDoublePoint S2polToS2rec(double r__, double theta__) const;

    //-------------------------------------------------------------------
    //MÉTODOS PARA DETERMINAR LA PERTENENCIA A DOMINIOS DE ORIENTACIÓN:

    //determina si un ángulo en radianes está fuera
    //del intervalo [theta__3min, theta__3max]
    bool isntInDomaintheta___3(double theta___3);
    //determina si un ángulo en radianes está fuera
    //del intervalo [theta___2min, theta___2max]
    //  IsntInDomaintheta___2(theta___2) =
    //      IsntInDomaintheta___3(theta___2 + theta___23)
    bool isntInDomaintheta___2(double theta___2);

    //determina si un ángulo en pasos está fuera
    //del intervalo [p___3min, p___3max]
    //  IsntInDomainp___3(p___3) = IsntInDomaintheta___3(G(p___3))
    bool isntInDomainp___3(double p___3);

    //-------------------------------------------------------------------
    //MÉTODOS PARA DETERMINAR VALORES CUANTIFICADOS DE ORIENTACIONES:

    //cuantifica theta___3
    double Qtheta___3(double theta___3);
    //cuantifica theta___2
    double Qtheta___2(double theta___2);

    //-------------------------------------------------------------------
    //MÉTODOS DE CONFIGURACIÓN:

    //asigna conjuntamente las propiedades de plantilla
    //si no espeficifa argumentos se asignan valores por defecto
    void setTemplate(double L12=MEGARA_L, double L13=MEGARA_L,
                     double theta____3=0, double R3=0.75);
    //asigna conjuntamente las propiedades de orientación en radianes
    void setOrientationRadians(double thetaO3,
                               double theta___3min, double theta___3max,
                               double theta___3);
    //asigna conjuntamente las propiedades de cuantificación
    void setQuantification(double SB2);

    //-------------------------------------------------------------------
    //MÉTODOS PARA DETERMIANAR DISTANCIAS:

    //Para determinar la distancia máxima del contorno de este brazo a
    //un punto, debe hacerse mediante:
    //  double dmax = Contour.DistanceMax(P);
    //donde P puede ser el eje del brazo en cuyo caso se obtendría L1V.

    //Para determinar la distancia mínima del contorno de este brazo a
    //otro contorno, debe hacerse mediante:
    //  double dmin = Contour.DistanceMin(Contour);
    //donde Contour puede ser el contorno de otro brazo
    //o cualquier otro obstáculo.

    //ADVERTENCIA: se recuerda que las distancias aquí calculadas no son
    //con el cuerpo del brazo si no con el contorno del mismo.

    //-------------------------------------------------------------------
    //MÉTODOS PARA DETERMINAR SI HAY COLISIÓN:

    //determina la distancia mínima con una barrera
    double distanceMin(const TBarrier*) const;
    //determina la distancia mínima con un brazo
    double distanceMin(const TArm*) const;

    //Si los contornos están tan alejado que no puede haber colisión,
    //la distancia mínima devuelta será igual a DBL_MAX.

    //determina si hay colisión con una barrera
    bool collides(const TBarrier*) const;
    //determina si hay colisión con un brazo
    bool collides(const TArm*) const;

    //La función que determina el estado de colisión tiene un tiempo
    //de procesado medio inferior a la de determinación de la distancia,
    //porque detiene el proceso cuando la colisión es detectada.

    //Contantes a partir de las cuales se determinan los valores de SPM:
    //  SPMmec: para el error mecánico. (10 um)
    //  SPMrec: para el error de recuperación aproximada. (TBD)
    //  SPMnum: para el error numérico. (10^-13)
    //  SPMvar: para absorber las variaciones de velocidad
    //          debidas a los motores paso a paso. (~rbs*Rc)
    //  SPMmin: para absorber la incertidumbre del salto mínimo
    //          durante la programación. (~10 um)
    //  SPMoff: para absorver las aproximaciones producidas
    //          por corrección del offset. (~offset*(Rc/L13))

    //Valor de SPM para cada ocasión:
    //  Vkd == kdPRECISE:
    //      ejecución:
    //          SPMexe_p = SPMmec + SPMnum + SPMvar
    //      validación:
    //          SPMval_p = SPMexe_p + SPMmin
    //      programación:
    //          SPMpro_p = SPMval_p + SPMmin
    //      asignación:
    //          SPMall_p = SPMpro_p + SPMoff
    //  Vkd == kdAPROXIMATE:
    //      ejecución:
    //          SPMexe_a = SPMexe_p + SPMrec
    //      validación:
    //          SPMval_a = SPMexe_a + SPMmin
    //      programación:
    //          SPMpro_a = SPMval_a + SPMmin
    //      asignación:
    //          SPMall_a = SPMpro_a + SPMoff

    //Ciclo de vida del SPM:
    //  1. Asignación.
    //  2. Programación.
    //  3. Validación.
    //  4. Ejecución.
    //En cada una de las etapas se consume el SPM destinado a la etapa,
    //motivo por el cual son sucesivamente más pequeños.
    //En ejecución el SPM representa la incertidumbre de localización
    //de las barreras físicas.

    //determina si un punto está sobre el brazo
    bool covers(TDoublePoint) const;

    //-------------------------------------------------------------------
    //MÉTODOS PARA APILAR Y RECUPERAR ORIENTACIONES:

    //Las propiedades para el almacenamiento de la configuración inicial
    //deben encapsularse en el posicionador, con objeto de facilitar
    //la manipulación de listas de posicionadores.
    //Por legibilidad las propiedades para el almacenamiento de
    //la configurción inicial del brazo, debe encapsularse en
    //el propio brazo.

    //apila theta___3 en la pila LIFO theta___3s
    //  theta___3s.Add(theta___3)
    void pushtheta___3(void);
    //restaura el último theta___3 de la pila LIFO theta___3s
    //  theta___3 = theta___3s.Last
    //si no hay una posición apilada lanza EImproperCall
    void restoretheta___3(void);
    //desempila el último theta___3 de la pila LIFO theta___3s
    //  theta___3s.Delete(theta___3.Count - 1)
    //si no hay una posición apilada lanza EImproperCall
    void poptheta___3(void);
    //restaura y desempila el último theta___3 de la pila LIFO theta___3s
    //  theta___3s.Delete(theta___3.Count - 1)
    //si no hay una posición apilada lanza EImproperCall
    void restoreAndPoptheta___3(void);

    //ADVERTENCIA: la recuperación de posiciones no coincidentes con
    //el centroide de un escalón de cuantificación, cuando está la
    //cuantificación activada, dará lugar al desplazamiento al centroide
    //más próximo.

    //-------------------------------------------------------------------
    //MÉTODOS PARA APILAR Y RECUPERAR ESTADOS DE CUANTIFICACIÓN:

    //apila Quantify___ en la pila LIFO Quantify___s
    //  Quantify___s.Add(Quantify___)
    void pushQuantify___(void);
    //restaura el último Quantify___ de la pila LIFO Quantify___s
    //  Quantify___ = Quantify___s.Last
    //si no hay una posición apilada lanza EImproperCall
    void restoreQuantify___(void);
    //desempila el último Quantify___ de la pila LIFO Quantify___s
    //  Quantify___s.Delete(Quantify___.Count - 1)
    //si no hay una posición apilada lanza EImproperCall
    void popQuantify___(void);
    //restaura y desempila el último Quantify___ de la pila LIFO Quantify___s
    //  Quantify___s.Delete(Quantify___.Count - 1)
    //si no hay una posición apilada lanza EImproperCall
    void restoreAndPopQuantify___(void);

    //-------------------------------------------------------------------
    //MÉTODOS DE MOVIMIENTO:

    //cambia la posición y orientación
    //del origen de coordenadas simultaneamente
    void set(TDoublePoint P1, double thetaO3);

    //Cambia simultaneamente:
    //  la posición del brazo;
    //  la orientación del del origen de coordenadas de S3;
    //  la orientación del brazo respecto de su origen de coordenadas.
    void set(TDoublePoint P1, double thetaO3, double theta___3);

    //como la orientación del brazo es guardada en radianes
    //resulta apropiado pasarlo ya en radianes

    //genera un valor aleatorio con distribución uniforme en
    //[max(0, Qmin), min(floor(SB2/2), Qmax)]
    double randomp___3(void);
    //asigna a p___3 un valor aleatorio con distribución uniforme en
    //[max(0, Qmin), min(floor(SB2/2), Qmax)]
    void randomizep___3(void);

    //asigna a theta___3 el primer angulo
    //estable menor o igual que el indicado
    void settheta___3FirstStableLessOrEqual(double theta___3);
    //asigna a theta___2 el primer angulo
    //estable menor o igual que el indicado
    void settheta___2FirstStableLessOrEqual(double theta___2);
    */
    CPPUNIT_ASSERT(true);
}

