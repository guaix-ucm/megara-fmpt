// Copyright (c) 2012-2016 Isaac Morales Durán. All rights reserved.
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
//File: testCilinder.cpp
//Content: test for the module Cilinder
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testCilinder.h"
#include "../src/Constants.h" //M_2PI
//#include "cppunit_assert_emulator.h" //Qt only.

#include <cmath> //abs
#include <vector>
//#include <stdlib.h>
#include <limits> //std::numeric_limits

#include "../src/Cilinder.h"

//using namespace Lists;
using namespace Models;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestCilinder);

//overide setUp(), init data, etc
void TestCilinder::setUp() {
    C = new TCilinder(TDoublePoint(0, 0));
}

//overide tearDown(), free allocated memory, etc
void TestCilinder::tearDown() {
    delete C;
}

//---------------------------------------------------------------------------

void TestCilinder::test_()
{
    for(int i=0; i<1000; i++) {
        try {
            //build an object
            TDoublePoint P0(0, 0);
            TCilinder *C1 = new TCilinder(P0);

            //construye un clon de un actuador de fibra
            TCilinder *C2 = new TCilinder(C1);

            C2->setL01(1);
            C2->copySizing(C1);
            if(C2->getL01() != C1->getL01()) {
                CPPUNIT_ASSERT(false);
                return;
            }

            C2->setthetaO1(1);
            C2->settheta_1min(-1);
            C2->settheta_1max(1);
            C2->settheta_1(1);
            C2->settheta_O3o(1);
            C2->copyOrientationRadians(C1);
            if(C2->getthetaO1() != C1->getthetaO1() ||
                    C2->gettheta_1min() != C1->gettheta_1min() ||
                    C2->gettheta_1max() != C1->gettheta_1max() ||
                    C2->gettheta_1() != C1->gettheta_1() ||
                    C2->gettheta_O3o() != C1->gettheta_O3o()) {
                CPPUNIT_ASSERT(false);
                return;
            }

            TFunction F;
            F.Add(TDoublePoint(-M_2PI, -1));
            F.Add(TDoublePoint(0, 0));
            F.Add(TDoublePoint(2*M_2PI, 2));
            C2->setF(F);
            C2->setQuantify_(false);
            TItemsList<bool> Quantify_s;
            C2->Quantify_s.Clone(Quantify_s);
            C2->copyQuantification(C1);
            for(int i=0; i<C2->Quantify_s.getCount(); i++)
                if(C2->Quantify_s[i] != C1->Quantify_s[i]) {
                    CPPUNIT_ASSERT(false);
                    return;
                }
            if(C2->getF() != C1->getF() ||
                    C2->getQuantify_() != C1->getQuantify_() ||
                    C2->getG() != C1->getG() ||
                    C2->getSB1() != C1->getSB1() ||
                    C2->getrbs() != C1->getrbs() ||
                    C2->getQ() != C1->getQ()) {
                CPPUNIT_ASSERT(false);
                return;
            }

            C2->setP0(TDoublePoint(1, 1));
            C2->copyLocation(C1);
            if(C2->getP0() != C1->getP0()) {
                CPPUNIT_ASSERT(false);
                return;
            }

            C2->setL01(1);
            C2->setthetaO1(1);
            C2->settheta_1min(-1);
            C2->settheta_1max(1);
            C2->settheta_1(1);
            C2->settheta_O3o(1);
            C2->setF(F);
            C2->setQuantify_(false);
            C2->Quantify_s.Clone(Quantify_s);
            C2->copyCilinder(C1);
            if(C2->getL01() != C1->getL01()) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(C2->getthetaO1() != C1->getthetaO1() ||
                    C2->gettheta_1min() != C1->gettheta_1min() ||
                    C2->gettheta_1max() != C1->gettheta_1max() ||
                    C2->gettheta_1() != C1->gettheta_1() ||
                    C2->gettheta_O3o() != C1->gettheta_O3o()) {
                CPPUNIT_ASSERT(false);
                return;
            }
            for(int i=0; i<C2->Quantify_s.getCount(); i++)
                if(C2->Quantify_s[i] != C1->Quantify_s[i]) {
                    CPPUNIT_ASSERT(false);
                    return;
                }
            if(C2->getF() != C1->getF() ||
                    C2->getQuantify_() != C1->getQuantify_() ||
                    C2->getG() != C1->getG() ||
                    C2->getSB1() != C1->getSB1() ||
                    C2->getrbs() != C1->getrbs() ||
                    C2->getQ() != C1->getQ()) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(C2->getP0() != C1->getP0()) {
                CPPUNIT_ASSERT(false);
                return;
            }

            //free the dinamic memory
            delete C2;
            delete C1;

        } catch(...) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //------------------------------------------------------------------
    //METHODS FOR TRANSFORM COORDINATES:

    //transforma un ángulo en radianes
    //del sistema S0 al sistema S1
    //      theta_ = thetaO1 - theta
    double theta_ = C->S0thetaToS1theta(M_PI);
    if(theta_ != -M_PI) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //transforma un punto en coordenadas cartesianas
    //del sistema S0 al sistema S1
    //      P_.x = (x-P0.x)*cos(thetaO1) + (y-P0.y)*sin(thetaO1);
    //      P_.y = (x-P0.x)*sin(thetaO1) - (y-P0.y)*cos(thetaO1);
    TDoublePoint P_ = C->S0recToS1rec(1, 1);
    if(P_ != TDoublePoint(1, -1)) {
        CPPUNIT_ASSERT(false);
        return;
    }
    P_ = C->S0recToS1rec(TDoublePoint(1, 1));
    if(P_ != TDoublePoint(1, -1)) {
        CPPUNIT_ASSERT(false);
        return;
    }

/*    //transforma un punto en coordenadas cartesianas
    //del sistema S1 al sistema S0
    //      P.x = (x_)*cos(thetaO1) + (y_)*sin(thetaO1) + P0.x;
    //      P.y = (x_)*sin(thetaO1) - (y_)*cos(thetaO1) + P0.y;
    TDoublePoint S1recToS0rec(double x_, double y_) const;

    //Se advierte que la trasnformación entre S0 y S1 es simétrica
    //a causa de la inversión de uno de los rotores.

    //La transformación de coordenadas cartesianas en S1
    //a coordenadas polares en S1, debe hacerse mediante las funciones:
    //      r_3 = Mod(x_3, y_3);
    //      theta_3 = ArgPos(x_3, y_3).

    //transforma las coordenadas cartesiaans en S2
    //en coordenadas cartesianas en S1
    TDoublePoint S2recToS1rec(double x__, double y__) const;

    //------------------------------------------------------------------
    //METHODS FOR DETERMINE BELONGING TO DOMINES:

    //determina si un ángulo en radianes
    //está fuera del intervalo [theta_1min, theta_1max]
    bool isntInDomaintheta_1(double theta_1) const;
    //determina si un ángulo pasos
    //está fuera del intervalo [p_1min, p_1max]
    bool isntInDomainp_1(double p_1) const;

    //determina si un ángulo en radianes
    //está dentro del intervalo [theta_1min, theta_1max]
    bool isInDomaintheta_1(double theta_1) const;
    //determina si un ángulo pasos
    //está dentro del intervalo [p_1min, p_1max]
    bool isInDomainp_1(double p_1) const;

    //IsntInDomainp_1 traduce la coordenada a radianes antes de comprobar
    //si está en dominio, para evitar los errores numéricos de
    //p_1min y p_1max.
    //Los métodos IsInDomain... utilizan los métodos IsntInDomain...
    //negándolos.

    //------------------------------------------------------------------
    //QUANTIFICATION METHODS:

    //devuelve el valor de cuantificación en radianes
    //más próximo al valor indicado
    //en el intervalo [theta_1min, theta_1max]
    double Qtheta_1(double theta_1);

    //SETTING METHODS:

    //cambia conjuntamente las propiedades de plantilla
    //si no especifica argumentos se asignarán valores por defecto
    void setTemplate(double L01=MEGARA_L, double L12=MEGARA_L, double L13=MEGARA_L,
                     double theta___3=0, double R3=0.75);

    //desactiva la cuantificación de los rotores del posicionador
    void disableQuantification(void);
    //activa la cuantificación de los rotores del posicionador
    void enableQuantification(void);

    //------------------------------------------------------------------
    //METHODS FOR STACK AND RESTORE POSITIONS:

    //apila theta_1 en la pila LIFO theta_1s
    //      theta_1s.Add(theta_1)
    void pushtheta_1(void);
    //restaura el último theta_1 de la pila LIFO theta_1s
    //      theta_1 = theta_1s.Last
    //si no hay una posición apilada lanza EImproperCall
    void restoretheta_1(void);
    //desempila el último theta_1 de la pila LIFO theta_1s
    //      theta_1s.Delete(theta_1.Count - 1)
    //si no hay una posición apilada lanza EImproperCall
    void poptheta_1(void);
    //restaura y desempila el último theta_1 de la pila LIFO theta_1s
    //      theta_1s.Delete(theta_1.Count - 1)
    //si no hay una posición apilada lanza EImproperCall
    void restoreAndPoptheta_1(void);

    //ADVERTENCIA: la recuperación de posiciones no estables,
    //cuando está la cuantificación activada, dará lugar
    //al desplazamiento a la posición estable más próxima.

    //------------------------------------------------------------------
    //METHODS FOR STACK AND RESTORE QUANTIFICATION STATUS:

    //apila Quantify_ en la pila LIFO Quantify_s
    //      Quantify_s.Add(Quantify_)
    void pushQuantify_(void);
    //restaura el último Quantify_ de la pila LIFO Quantify_s
    //      Quantify_ = Quantify_s.Last
    //si no hay una posición apilada lanza EImproperCall
    void restoreQuantify_(void);
    //desempila el último Quantify_ de la pila LIFO Quantify_s
    //      Quantify_s.Delete(Quantify_.Count - 1)
    //si no hay una posición apilada lanza EImproperCall
    void popQuantify_(void);
    //restaura y desempila el último Quantify_ de la pila LIFO Quantify_s
    //      Quantify_s.Delete(Quantify_.Count - 1)
    //si no hay una posición apilada lanza EImproperCall
    void restoreAndPopQuantify_(void);

    //------------------------------------------------------------------
    //METHODS FOR SET POSITION ANGLES:

    //cambia la posición y orientación
    //del origen de coordenadas simultaneamente
    void moveOrigin(TDoublePoint P0, double thetaO1);

    //asigna conjuntamente theta_1 y theta___3
    void setAnglesRadians(double theta_1, double theta___3);
    //asigna conjuntamente p_1 y p___3
    void setAnglesSteps(double p_1, double p___3);

    //mueve los rotores hasta el origen de coordenadas
    void setAnglesZeroSteps(void);

    //añade conjuntamente 'at_1' y 'at___3' a 'theta_1' y 'theta___3'
    void addAnglesRadians(double at_1, double at___3);
    //añade conjuntamente 'ap_1' y 'ap___3' a 'p_1' y 'p___3'
    void addAnglesSteps(double ap_1, double ap___3);

    //genera un valor aleatorio con distribución uniforme en:
    //      [max(0, p_1min), min(floor(SB1), p_1max)]
    double randomp_1(void);
    //asigna a p_1 un valor aleatorio con distribución uniforme en:
    //      [max(0, p_1min), min(floor(SB1), p_1max)]
    void randomizep_1(void);

    //------------------------------------------------------------------
    //METHODS FOR DETERMINE POSITION ANGLES:

    //Dado el punto (r_3, theta_3) (en S1)
    //calcula (theta_1, theta___3) para que P3 vaya a él;
    //si el punto no está dentro del dominio devuelve falso.
    //Aunque el punto sea inalcanzable, este método devolverá
    //las posiciones límite a la que los rotores pueden ir.
    bool anglesToGoP_3(double &_theta_1, double &_theta___3,
                       double r_3, double theta_3) const;
    //Dado el punto (x3, y3) (en S0)
    //calcula (theta_1, theta___3) para que P3 vaya a él;
    //si el punto no está dentro del dominio devuelve falso.
    //Aunque el punto sea inalcanzable, este método devolverá
    //las posiciones límite a la que los rotores pueden ir.
    bool anglesToGoP3(double &_theta_1, double &_theta___3,
                      double x3, double y3) const;

    //Dado el ángulo theta_3 (en S1) calcula theta___3 para que P3 vaya a él;
    //si el ángulo no está dentro del dominio devuelve falso.
    bool theta___3ToGotheta_3(double &_theta___3, double theta_3);
    //Dado el radio r_3 (en S1) calcula theta___3 para que P3 vaya a él;
    //si el radio no está dentro del dominio devuelve falso.
    bool theta___3ToGor_3(double &_theta___3, double r_3);

    //determina las posiciones angulares estables que hacen que
    //la fibra de este posicionador se ubique lo más cerca posible
    //del punto correspondiente a unas posiciones angulares de los rotores
    //devuelve la distancia al punto hallado
    double getNearestStablePosition(double &p_1nsp, double &p___3nsp,
                                    double theta_1, double theta___3);

    //Este método tiene los parámetros de entrada en coordenadas angulares
    //de los rotores en radianes, en congruencia con los valores devueltos por
    //el método:
    //  bool AnglesToGoP_3(double& theta_1, double& theta___3,
    //      double r_3, double theta_3);
    //Es preferible introducir coordenadas angulares de los rotores,
    //para reconstruir el punto por el mismo método con el cual
    //se van a generar los puntos estables, de modo que se evite
    //el error numérico.

    //Este método devuelve valores en pasos, porque los métodos de
    //generación de programas de movimiento van a trabajar en pasos.
    //Se ha elegido que dichos métodos trabajen en pasos por varias razones:
    //  - Existen tres posibilidades:
    //    1. Trabajar en radianes y no cuantificar.
    //    2. Trabajar en radianes y cuantificar.
    //    3. Trabajar en pasos y cuantificar.
    //    Trabajar en pasos constituye el método de cuantificación por
    //    antonomasia, de modo que no tiene sentido trabajar en radianes
    //    y cuantificar al mismo tiempo.
    //  - Si se trabaja en radianes hay que inroducir en el SPM una
    //    componente para absorber el error de cuantificación de los
    //    programas de movimiento.
    //  - La cuantificación de los programas de movimiento no es trivial,
    //    ya que las posiciones finales pueden diferir en más de un paso
    //    en la posición final de algún rotor.
    //  - La componente de SPM a introducir dependería además del tamaño
    //    de los escalones de cuantificación más grandes (en posicones
    //    de inseguridad), de modo que si por defecto mecánico, hubiera
    //    algún escalon incrementado, la componente del SPM se vería
    //    también incrementada.
    //  - Se haría obligatorio introducir el conceptos de distancia de
    //    seguridad.
    //  - Habría que definir en la documentación el concepto de
    //    instrucción de movimiento no cuantificada (en radianes).
    //  - La única ventaja de trabajar en radianes es tener programas
    //    válidos para cualquier número de pasos. Sin embargo, si cambiase
    //    el número de pasos cambiaríán otros parámetros del modelo,
    //    de modo que la adaptación de un programa no tiene sentido,
    //    debiendo generarse un nuevo programa.
    //A la vista de estas razones se concluye que el motivo principal de
    //trabajar en pasos es la simplificación de los algoritmos;
    //en particular la cuantificación de los programas y el cálculo de
    //la nueva componente del SPM. Además trabajar en pasos permite
    //generar programas de movimiento optimizados, tanto en tiempo como en
    //espacio, pudiendo volverse crítico cualquiera de los dos factores
    //en un momento dado.

    //------------------------------------------------------------------
    //METHODS FOR DETERMINE BELONGING TO DOMAIN OF P3:

    //determina si el punto P_ está dentro del dominio de P_3
    bool pointIsInDomainP_3(TDoublePoint P_);
    //determina si el punto P_ está fuera del dominio de P_3
    bool pointIsOutDomainP_3(TDoublePoint P_);

    //determina si el punto P está dentro del dominio de P3
    bool pointIsInDomainP3(TDoublePoint P);
    //determina si el punto P está fuera del dominio de P3
    bool pointIsOutDomainP3(TDoublePoint P);
    //determina si el punto (x, y) está fuera del dominio de P3
    bool pointIsOutDomainP3(double x, double y);

    //determina el arco descrito por P3 al girar el rotor 1
    //en el intervalo [max(0., theta_1min), min(2_MPI, theta_imax)]
    //con el brazo totalmente extendido
    //si el arco contiene un error numérico significativo hace Pfin = Pini
    void getArc(TDoublePoint &Pini, TDoublePoint &Pfin, TDoublePoint &Pa,
                double &R);
    //determina el arco descrito por P3 al plegar el brazo
    //con el rotor 1 en el ángulo theta_
    void getArc(TDoublePoint &Pini, TDoublePoint &Pfin, TDoublePoint &Pa,
                double &R, double theta_);

    //Determina si un segmento interseca al arco descrito por P3
    //al girar el rotor 1 con el brazo totalemnte extendido.
    bool intersectionSegmentArc1(TDoublePoint Pa, TDoublePoint Pb);
    //Determina si un segmento interseca al arco descrito por P3
    //al plegar el brazo cuando theta_1 = max(0, theta_1min).
    bool intersectionSegmentArc2(TDoublePoint Pa, TDoublePoint Pb);
    //Determina si un segmento interseca al arco descrito por P3
    //al plegar el brazo cuando theta_1 = min(M_2PI, theta_1max).
    bool intersectionSegmentArc3(TDoublePoint Pa, TDoublePoint Pb);

    //Determina si un segmento invade el dominio de P3
    //teniendo en cuenta los arcos descritos por P3 al girar el rotor 1.
    bool segmentInvadeDomainP3(TDoublePoint Pa, TDoublePoint Pb);
    //Determina si un segmento no puede ser recorrido
    //totalmente mediante un movimeinto continuo del punto P3,
    //teniendo en cuenta los arcos descritos por P3 al girar el rotor 1.
    bool segmentCantBeFollowedByP3(TDoublePoint Pa, TDoublePoint Pb);

    //Un polisegmento puede estar en una de las cuatro situaciones
    //sigueintes respecto del dominio de P3:
    // 1. Totalmente en el interior sin intersecar los límites del dominio.
    // 2. Totalmente en el interior intersecando los límites del dominio.
    // 3. Parcialmente en el interior y el resto fuera.
    // 4. Totalmente en el exterior.

    //Determina si un polisegmento invade el dominio de P3,
    //teniendo en cuenta los arcos descritos por P3 al girar el rotor 1.
    bool polysegmentInvadeDomainP3(TItemsList<TDoublePoint> &Polysegment);
    //Determina si un polisegmento no puede ser recorrido
    //totalmente mediante un movimeinto continuo del punto P3,
    //teniendo en cuenta los arcos descritos por P3 al girar el rotor 1.
    bool polysegmentCantBeFollowedByP3(TItemsList<TDoublePoint> &Polysegment);

    //ERROR: Los métodos:
    //      PolysegmentIsNotOutDomainP3
    //      PolysegmentIsNotInDomainP3
    //requieren se dejarán pendientes de revisión
    //pues son de dudosa utilidad y sin duda contienen defectos.
    //En caso de revisarlos se advierne la necesidad de
    //obtener los valores correctos de los arcos (Pc, Pini, Pfin)
    //en S1 y cuidar que el arco sea en cada caso inferior a M_2PI
    //en caso contrario deberá considerarse una circunferencia.

    //ADVERTENCIA: los métodos:
    //      bool PolysegmentIsNotOutDomainP3(...);
    //      bool PolysegmentIsNotInDomainP3(...);
    //no son complementarios.

    //-------------------------------------------------------------------
    //HOW TO USE THE METHODS FOR DETERMINE POSITIONS:
    //
    //para obtener las posiciones correspondientes a un punto expresado
    //en coordenadas cartesianas de S0 hay que seguir los pasos:
    //      P_ = S0toS1(x, y); //traduce a cartesianas relativas a S1
    //      r_3 = Mod(P_.x, P_.y); //obtiene la coordenada radial
    //      theta_3 = ArgPos(P_.x, P_.y); //obtiene la coordenada angular

    //ahora ya puede determinar las posiciones de los rotores
    //y si el punto está en el dominio:
    //      isindomine = PositionToGoP_3(theta_1, theta___3, r_3, theta_3);

    //para obtener las posiciones en pasos deben traducirse
    //las posiciones de radianes a pasos mediante:
    //      p_1 = F(theta_1); //posición del rotor 1 en pasos
    //      p__3 = Arm->F(theta___3); //posición del rotor 2 en pasos
    //
    //Para determinar si un punto está dentro del dominio
    //de un punto del posicionador, implica calcular
    //las posiciones de los rotores para ver si estas están
    //dentro de sus respectivos dominios. Por eso
    //tal determinación deberá realizarse invocando a los métodos
    //de determinación de posiciones
    //-------------------------------------------------------------------

    //------------------------------------------------------------------
    //METHODS FOR SET POSITIONS:

    //asigna coordenadas polares a P_3
    //si el punto no está en el dominio de P_3 lanza una excepción
    void setPolarP_3(double r_3, double theta_3);

    //asigna coordenadas cartesianas a P_3
    //si el punto no está en el dominio de P_3 lanza una excepción
    void setCartesianP_3(double x_3, double y_3);
    void setCartesianP_3(TDoublePoint P_3);

    //genera un punto con distribución uniforme en
    //el dominio cartesiano de P3 hasta que dicho punto
    //esté dentro del dominio de P3
    TDoublePoint randomP3(void);

    //genera un punto con distribución uniforme en
    //el dominio cartesiano de P3 hasta que dicho punto
    //esté dentro del dominio de P3 y lo asigna a P3
    void randomizeP3(void);
*/

    CPPUNIT_ASSERT(true);
}

