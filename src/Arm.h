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
//File: Arm.h
//Content: arm model of an actuator of a RP
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ARM_H
#define ARM_H

#include "ExclusionAreaList.h"
#include "Quantificator.h"
#include "Function.h"
#include "FiberMOSModelConstants.h"
#include "Constants.h"

//---------------------------------------------------------------------------

using namespace Mathematics;
using namespace Operators;
using namespace Lists;

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//TArm
//---------------------------------------------------------------------------

//SUBSISTEMAS Y ENCAPSULACIÓN DEL SISTEMA RP:

//Un RP se concibe compuesto de dos subsistemas:
//      - las propiedades de dimensionamiento;
//      - las propiedades de seguridad.
//Las propiedades de seguridad solo son usadas para determinar las
//posiciones finales de las funciones de movimiento.
//Además las propiedades de seguridad dependen de las propiedades
//de dimensionamiento de todos los RPs del sistema, por lo que su cálculo
//debe posponerse hasta la definción del FiberMOS.

//Las propiedades de seguridad dependen de las propiedades del brazo
//así como de dos parámetros:
//      L1: radio del cilindro;
//      Rbsaf: radio de seguridad del cilindro el cual no es alcanzado por los
//      demás cilindros.
//de modo que el brazo debe disponer de la siguiente función:
//      CalculateSafeParameters(L1, Rbsaf)
//la cual debe ser invocada cada vez que cambie algún parámetro de
//dimensionamiento del sistema completo, para que los parámetros de seguridad
//del posicionador estén actualizados.

//De modo que se dispone de dos opciones para definir las clases que componen
//el sistema de RPs:
//  1. respetar estrictamente el criterio de encapculación restringiendo el
//  acceso a paráemtros de dimensionamiento de los posicionadores,
//  programando una invocación a la función que calcula los parámetros
//  de seguridad despues de cambiar cada parametro.
//  2. delegar en el programador la invocación a la función que calcula
//  los parámetros de seguridad cada vez que se cambien los parámetros.

//La opción preferida es la 2, ya que cada posicionador tiene muchos parámetos,
//y de todos modos los posicionadores deben ser accesibles individualmente
//de modo que los parámetros de dimensionamiento podrían ser cambiados
//al margen del sistema. Hacerlo de este modo simplifica la definición
//de las clases y aumenta la legilibilidad, lo cual es preferible en este caso.

//Por ello, el brazo se concibe como si estuviera montado en el posicionador
//para que las propiedades del mismo puedan ser accesibles directamente
//incluidas las que solo tienen sentido cuando el brazo está montado
//en el cilindro, como los límites del dominio del ángulo de giro.

//ADVERTENCIA: aunque el brazo es un tipo de obstáculo, no conviene que derive
//de TBarrier, ya que la nomenclatura difiere:
//En TBarrier:
//  Contour_ <---------
//  r_max
//  P0 <---------------
//  thetaO1 <----------
//  Contour
//  SPM
//En TArm:
//  Contour____ <------
//  L1V
//  P1 <---------------
//  thetaO3 <----------
//  Contour
//  SPM

//clase brazo de posicionador
class TArm {
protected:
    //-------------------------------------------------------------------
    //PROPIEDADES DE PLANTILLA
    //DE LECTURA/ESCRITURA:

    double L12;
    TDoublePoint P__2;

    double L13;
    double theta__3;
    TDoublePoint P__3;

    double R3;

    TContourFigureList Contour____;

    //PROPIEDADES DE PLANTILLA
    //DE SOLO LECTURA:

//    TDoublePoint P____2;
//    TDoublePoint P____1;
//    TContourFigureList Contour__;

    double L1V;
    TDoublePoint V__;

    //-------------------------------------------------------------------
    //PROPIEDADES DE ORIENTACIÓN EN RADIANES
    //DE LECTURA/ESCRITURA:

    double thetaO3;

    double theta___3min;
    double theta___3max;
    double theta___3;

    //-------------------------------------------------------------------
    //PROPIEDADES DE CUANTIFICACIÓN
    //DE LECTURA/ESCRITURA:

    TFunction F;

    bool Quantify___;

    //PROPIEDADES DE CUANTIFICACIÓN
    //DE SOLO LECTURA:

    TFunction G;

    double SB2;
    double rbs;

    TQuantificator Q;

    //-------------------------------------------------------------------
    //PROPIEDADES DE ORIENTACIÓN EN PASOS
    //DE LECTURA/ESCRITURA:

    //-------------------------------------------------------------------
    //PROPIEDADES DE LOCALIZACIÓN
    //DE LECTURA/ESCRITURA:

    TDoublePoint P1;

    double SPM;

    //PROPIEDADES DE LOCALIZACIÓN
    //DE SOLO LECTURA:

    TDoublePoint P2;
    TDoublePoint P3;
    TDoublePoint V;

    TContourFigureList Contour;

    //------------------------------------------------------------------
    //MÉTODOS DE ASIMILACIÓN:

    //Apartir de:
    //  {L12, L13, theta__3}
    //  {Contour____}
    //Determina:
    //  {P____2, P____1}
    //  {P__2, P__3}
    //  {Contour__}
    //  {V__, L1V}
    void processateTemplate(void);

    //A partir de:
    //  {F(theta___3)}
    //Determina:
    //  {G(p___3), SB2, rbs}
    //  {theta___3min, theta___3max}
    //  {Q(p___3)}
    //Restricciones:
    //  La función F(theta___3) debe ser monótona creciente.
    void processateF(void);

    //Nótese que no se imponen las siguientes restricciones:
    //  La función F(theta___3) debe estar definida en 0.
    //  F(0) debería ser igual a cero.
    //  La función F(theta___3) debe estar definida en M_2PI.

    //a partir de:
    //  {P__2, P__3}
    //  {Contour__, V__}
    //  {P1, theta2}
    //Determina:
    //  {P2, P3}
    //  {Contour, V}
    void calculateImage(void);

    //ADVERTENCIA: el único método de asimilación que necesita tener
    //alto rendimiento es CalculateImage, ya que (P1, theta2)
    //son variables que pueden cambiar rápidamente.

public:
    //------------------------------------------------------------------
    //PROPIEDADES DE PLANTILLA
    //DE LECTURA/ESCRITURA:

    //Las propiedades seleccionadas para la instancia son:
    //  L12, L13, theta__O3, R3, thetaO3,
    //  theta___3min, theta___3max, theta_3,
    //  SB2,
    //  Contour____

    //Definiciones:
    //  P1: posición del rotor del brazo
    //  P2: punto de referencia del cabezal del brazo
    //  P3: punto central de la microlentilla

    //distancia de P1 a P2
    //debe ser mayor que cero
    //valor por defecto: MEGARA_L mm
    double getL12(void) const {return L12;}
    void setL12(double);
    //P2 en S2
    //P__2.x debe ser distinto de cero
    //P__2.y debe ser igual a cero
    //valor por defecto: {L12, 0}
    TDoublePoint getP__2(void) const {return P__2;}
    void setP__2(const TDoublePoint&);

    //El valor por defecto de L12 se va a usar como
    //valor por defecto para L01.

    //distancia de P1 a P3
    //coordenada radial de P3 en S2
    //debe ser no negativa
    //valor por defecto: MEGARA_L mm
    double getL13(void) const {return L13;}
    void setL13(double);
    //coordenada angular de P3 en S2
    //puede ser cualquier valor
    //pero es recomendable que esté en (-M_PI, M_PI]
    //valor por defecto: 0 rad
    double gettheta__3(void) const {return theta__3;}
    void settheta__3(double);
    //orientación del origen de coordenadas de S3 en S2
    //es igual que theta__3
    //valor por defecto: 0 rad
    double gettheta__O3(void) const {return theta__3;}
    void settheta__O3(double);
    //P3 en S2
    TDoublePoint getP__3(void) const {return P__3;}
    void setP__3(const TDoublePoint&);

    //radio de la lente del brazo
    //debe ser mayor que cero
    //valor por defecto: 0.75 mm
    double getR3(void) const {return R3;}
    void setR3(double);

    //plantilla del contorno del brazo en S4
    //valor por defecto: MEGARA_Contour____Text
    const TContourFigureList &getContour____(void) const {
        return (const TContourFigureList&)Contour____;}
    void setContour____(const TContourFigureList&);

    //ADVERTENCIA: la escritura de Contour____.Text conviene hacerla
    //a tavés de la propiedad Contour____.Text, con un valor previamente
    //leido, de modo que el valor de lectura en modo texto coincida
    //con el valor asignado.

    //El modelo del brazo debe cumplir las siguientes restricciones:
    //- Debe tener un solo intervalo apical.
    //- Debe haber un ángulo de de repleción a partir del cual
    //  el codo del brazo no debe poder chocar con las trayectorias de
    //  los intervalos apicales de los brazos de los posicionadores
    //  circundantes.
    //Además es recomendable que:
    //- El ángulo a partir del cual se cumple la segunda restricción
    //  sea lo más amplio posible, de modo que la posición de seguridad
    //  se de con el brazo lo más desplegado posible (el límite teórico
    //  cuando el radio del codo no sobrepasa el radio del cabezal es PI/3).
    //- El área ocupada por la parte del brazo que puede invadir el área
    //  común sea lo más reducida posible.

    //Se advierte que la primera restricción puede cumplirse solamente en
    //el modelo, de modo que el diseño no es determinate. De este modo
    //el punto V puede quedar en el aire, donde no hay material del brazo
    //real.

    //PROPIEDADES DE PLANTILLA
    //DE SOLO LECTURA:

    TDoublePoint P____2;
    TDoublePoint P____1;
    TContourFigureList Contour__;

    //P2 en S4
    TDoublePoint getP____2(void) const {return P____2;}
    //P1 en S4
    TDoublePoint getP____1(void) const {return P____1;}

    //plantilla del contorno del brazo en S2
    //valor por defecto: MEGARA_Contour__Text
    const TContourFigureList &getContour__(void) const {
        return (const TContourFigureList&)Contour__;}

    //radio máximo de la circunferencia descrita por
    //el ápice del brazo al girar (en mm)
    double getL1V(void) const {return L1V;}
    //vértice del cabezal de la plantilla en S4
    TDoublePoint getV__(void) const {return V__;}

    //V__ se refiere al punto de intersección
    //entre la envolvente del brazo al girar
    //y el eje de simetría longitudinal del brazo.

    //-------------------------------------------------------------------
    //PROPIEDADES DE ORIENTACIÓN EN RADIANES
    //DE LECTURA/ESCRITURA:

    //Los pasos se representarán mediante
    //un tipo primitivo en punto flotante,
    //para poder representar fracciones de paso.
    //El paso p = 0 se corresponde con theta = 0,
    //y el paso p = SB2 se corresponde con theta = 2*PI.

    //Se prefiere almacenar:
    //  ([theta___3min, theta___3max], theta___3),
    //en vez de:
    //  ([theta___2min, theta___2max], theta___2) o
    //  ([p___3min, p___3max] y p___3),
    //ya que se va a trabajar con P3 y las ecuaciones matemáticas
    //utilizan las unidades en radianes. Sin embargo deben definirse
    //valores por defecto para [theta__2min, theta__2max],
    //ya que los topes de los ejes chocarán contra el cuerpo del brazo,
    //cuya orientación corresponde a la del eje de P1 a P2.

    //Por consiguiente solo los límites del dominio de de theta___3
    //son exactos. Los límites de los dominios de theta___2 y p___3,
    //son solo aproximaciones que sirven para discriminar asignaciones
    //a estas variables. Es decir que si hago una asignación a theta___2
    //en el intervalo [theta___2min, theta___2max] no dará error,
    //aunque el valor de lectura puede diferir ligeramente del de escritura.

    //Para ello el valor de la variable es traducido a theta___3
    //y contreñido al intervalo [theta___3min, theta___3max].
    //Este modo de proceder tiene dos consecuencias:
    //- No es posible asignar todos los valores a theta___3 a través de
    //  las otras variables.
    //- theta___3 puede contener valores que hagan que las demás variables
    //  se salgan ligeramente de los límites indicados de sus dominios,
    //  aunque esta es una situación poco probable.

    //Para evitar efectos indeseados por estas inconsistencias,
    //deberán emplearse los siguientes métodos de comprobación
    //de pertenencia al dominio:
    //  double IsntInDomaintheta___3(double theta___3);
    //  double IsntInDomaintheta___2(double theta___2);
    //  double IsntInDomainp___3(double p___3);
    //De los cuales, los dos últimos traducen el valor a theta___3
    //y utilizan el primer método para comprobar la pertenencia al dominio.

    //El proceso de cuantificación de ángulos consistirá en dos pasos:
    //  1. transformación de coordenadas mediante el método:
    //       p___3_ = F(theta___3)
    //  2. cuantificación de la coordenada en pasos mediante el método:
    //       p___3 = Q(p___3_)
    //Donde la función F(theta___3) puede ser no lineal con el objeto de
    //caracterizar la reductora del motor del eje del brazo.

    //El proceso de decuantificación consistirá en dos pasos:
    //  1. decuantificación de la coordenada en pasos mediante el método:
    //       p___3_ = R(p___3)
    //  2. transformación inversa de coordenadas mediante el método:
    //       theta___3 = G(p___3_)
    //Donde la función de cuantificación inversa R(p___3) es la misma
    //que la de cuantificación Q(p___3_) con el objeto de poder realizar
    //asignaciones no exactas a theta___3 y theta___2, ya que sus
    //dominios no han sido definidos en Z como el de p___3.
    //Y la función G(p___3) es la inversa de F(theta___3).

    //Para simplificar la notación se admitirá el abuso de notación:
    //  p___3 = F(theta___3);
    //  p___3 = Q(p___3);
    //  theta___3 = G(p___3);
    //y la función R no será representada.

    //Las propiedades seleccionadas para la instancia son:
    //  (thetaO3, theta___3, theta___2min, theta___2max)

    //orientación del origen de coordenadas de S3 en S0
    //cuando el eje 2 está en el origen
    //valor por defecto: thetaO2 + gettheta__O3() rad
    //donde:
    //  thetaO2: lo indica el usuario en el constructor
    //      y su valor por defecto es M_PI/2
    //  theta__O3: es igual a theta__3
    //      y su valor por defecto es 0
    double getthetaO3(void) const {return thetaO3;}
    void setthetaO3(double);

    //límite inferior del intervalo de giro de P3 en S3
    //debe estar en el dominio de definición de F(theta___3)
    //no debe ser mayor que theta___3max
    //valor por defecto: MEGARA_theta___3min rad
    double gettheta___3min(void) const {return theta___3min;}
    void settheta___3min(double);
    //límite superior del intervalo de giro de P3 en S3
    //debe estar en el dominio de definición de F(theta___3)
    //no debe ser menor que theta___3min
    //valor por defecto: MEGARA_theta___3max rad
    double gettheta___3max(void) const {return theta___3max;}
    void settheta___3max(double);
    //ángulo de orientación de P3 en S3
    //debe estar en el dominio de definición de F(theta___3)
    //Escrutura:
    //  if(Quantify___)
    //      p_theta___3 = Qtheta___3(_theta___3);
    //  else
    //      p_theta___3 = theta___3;
    //valor por defecto: 0 rad
    double gettheta___3(void) const {return theta___3;}
    void settheta___3(double);
    //ángulo de orientación de P3 en S0
    //theta3 debe estar en su dominio
    //Lectura:
    //  theta3 = thetaO2 + theta__O3 + theta___3
    //Escritura:
    //  theta___3 = theta3 - thetaO2 - theta__O3
    //valor por defecto: 0 + PI/2 + 0 rad
    double gettheta3(void) const; void settheta3(double);

    //NOTA: cualquier asignación a theta___3 con la cuantificacion activada
    //será cuantificada mediante Qtheta___3(_theta___3) que a su vez
    //utilizará la función Q para cuantificar el valor en pasos,
    //la cual constreñirá el valor asignado a p___3 en el intervalo
    //[Q.Qmin, Q.Qmax].

    //pila LIFO de posiciones angulares del brazo en radianes
    //valor por defecto: {}
    TVector<double> theta___3s;
    //Las propiedades para el almacenamiento del estado inicial
    //deben encapsularse en el posicionador, con objeto de facilitar
    //la manipulación de listas de posicionadores.
    //Por legibilidad las propiedades para el almacenamiento de
    //la configurción inicial del brazo, debe encapsularse en
    //el propio brazo.

    //orientación del origen de coordenadas de S2 en S0
    //Lectura:
    //  thetaO2 = thetaO3 - theta__O3
    //Escritura:
    //  p_thetaO3 = thetaO2 + theta__O3
    double getthetaO2(void) const; void setthetaO2(double);

    //valor aproximado de referencia del
    //límite inferior del intervalo de giro de P2 en S3
    //debe estar en su dominio
    //debe ser menor que theta___2max
    //Lectura:
    //  theta___2min = theta___3min - theta___3;
    //Escritura:
    //  p_theta___3min = theta___2min + theta___3;
    //valor por defecto: MEGARAtheta__2min rad
    double gettheta___2min(void) const; void settheta___2min(double);
    //valor aproximado de referencia del
    //límite superior del intervalo de giro de P2 en S3
    //debe estar en su dominio
    //debe ser mayor que theta___2min
    //Lectura:
    //  p_theta___2max = theta___3max - theta___3;
    //Escritura:
    //  theta___3max = theta___2max + theta___3;
    //valor por defecto: MEGARAtheta__2max rad
    double gettheta___2max(void) const; void settheta___2max(double);
    //valor aproximado de referencia del
    //ángulo de orientación de P2 en S3
    //debe estar en su dominio
    //Lectura:
    //  theta___2 = theta___3 - theta___23
    //Escritura:
    //  double theta___3 = theta___2 + theta___23;
    //  if(IsntInDomaintheta___3(theta___3))
    //      if(IsntInDomaintheta___2(theta___2)
    //          throw EImproperArgument("value for theta___2 isn't in domain");
    //      else
    //          theta___3 = ConstrainInDomaintheta___3(_theta___3);
    //  p_theta___3 = theta___3;
    //la escritura en theta___3 tendrá en cuenta la cuantificación
    //valor por defecto: 0 - 0 rad
    double gettheta___2(void) const; void settheta___2(double);
    //ángulo de orientación de P2 en S0
    //theta2 debe estar en su dominio
    //Lectura:
    //  theta2 = theta3 - theta___23
    //Escritura:
    //  theta3 = theta2 + theta___23
    //valor por defecto: PI/2 - 0 rad
    double gettheta2(void) const; void settheta2(double);

    //ADVERTENCIA: para inicializar theta___2min y theta___2max,
    //es necesario inicializar primero theta___3, ya que si theta___3
    //se encuentra fuera del dominio de inicialización, será arrastrado
    //dentro del mismo, y para ello, si Quantify___==true, será necesario
    //cuantificar el valor, para lo cual Qmin y Qmax deben estar definidas.
    //pero Qmax y Qmin se definen a partir de
    //F y [p___3min, p___3max].

    //ADVERTENCIA: Si theta___2 está en su dominio aproximado,
    //cuando se sale de theta___3, es constreñido al dominio de este.
    //De este modo es factible utilizar el dominio de theta___2
    //como referencia en escritura. Esto no pasa en lectura,
    //ya que theta___2 puede salirse de su dominio, (por que es aproximado).

    //PROPIEDADES DE ORIENTACIÓN EN RADIANES
    //DE SOLO LECTURA:

    //first position angle of rotor 2 in rad
    //  theta___3first = max(0, theta___3min)
    double gettheta___3first(void) const;
    //last position angle of rotor 2 in rad
    //  theta___3last = min(M_2PI, theta___3max)
    double gettheta___3last(void) const;

    //-------------------------------------------------------------------
    //PROPIEDADES DE CUANTIFICACIÓN
    //DE LECTURA/ESCRITURA:

    //función de transformación de de radianes a pasos de P3
    //debe estar definida en algún punto
    //debe ser monótona creciente
    //valor por defecto:
    //  {(M_2PI, MEGARA_SB2),
    //  (0, 0),
    //  (M_2PI, MEGARA_SB2)}
    const TFunction& getF(void) const {return (const TFunction&)F;}
    void setF(const TFunction&);

    //Nótese que no se imponen las siguientes restricciones:
    //  La función F(theta___3) debe estar definida en 0.
    //  F(0) debería ser igual a cero.
    //  La función F(theta___3) debe estar definida en M_2PI.

    //ADVERTENCIA: F.PointsText debe ser escrito mediante la propiedad
    //FPointsText de modo que se invoque al método SetFPointsText,
    //el cual invocará a SetF, el cual comprobará que F cumple
    //las restricciones y asimilará F mediante:
    //  CalculateG();
    //  CalculateQ();
    //  theta___3 = theta___3;

    //interruptor de cuantificación de p___3
    //indica si deben cuantificarse los valores asignados a p___3
    //valor por defecto: true
    bool getQuantify___(void) const {return Quantify___;}
    void setQuantify___(bool);

    //Cuando Quantify___ es activado theta___3 es situado en la posición
    //estable más próxima mediante la reasignación así mismo:
    //  theta___3 = theta___3;

    //pila LIFO de configuraciones del cuantificador del brazo
    //valor por defecto: {}
    TItemsList<bool> Quantify___s;

    //Las propiedades para el almacenamiento del estado inicial
    //deben encapsularse en el posicionador, con objeto de facilitar
    //la manipulación de listas de posicionadores.
    //Por legibilidad las propiedades para el almacenamiento de
    //la configurción inicial del brazo, debe encapsularse en
    //el propio brazo.

    //ADVERTENCIA: Quantify___ debe estar desactivado cuando se
    //esté simulandom el movimiento simultaneo de varios ejes,
    //y deberá estar activo cuando no haya movimiento.

    //PROPIEDADES DE CUANTIFICACIÓN
    //DE SOLO LECTURA:

    //función de transformación de pasos a radianes
    //es la inversa de F
    const TFunction& getG(void) const {return (const TFunction&)G;}

    //número de pasos de una vuelta completa del eje 2 (SB: Steps Back)
    //es un número real ya que a la salida de la reductora
    //el número de pasos puede ser fraccionario
    //aunque convendría que fuera par ya que solo va a girar media vuelta
    //valor por defecto: F(M_2PI) = SB2SIDE steps
    double getSB2(void) const {return SB2;}
    //radianes por paso medio en el intervalo [0, M_2PI]
    //  rbs = M_2PI/SB2;
    //valor por defecto: M_2PI/SB2SIDE rad
    double getrbs(void) const {return rbs;}

    //función de cuantificación lineal de p___3
    //valor por defecto: (1, [p___3min, p___3max])
    const TQuantificator& getQ(void) const {
        return (const TQuantificator&)Q;}

    //NOTA: la constricción al dominio de p___3 se realiza en la función
    //de cuantificación Q, cuyo dominio imagen es [Q.Qmin, Q.Qmax].

    //ADVERTENCIA: cuando theta___3min está ligeramente por debajo de
    //un centroide (o posición estable), p___3 puede ser calculado con
    //un error numérico que lo desplace por encima de dicho centroide
    //entonces se debe saber que Q.Qmin es calculado del siguiente modo:
    //  p___3min = F(theta___3min);
    //  Q.Qmin = (int)ceil(p___3min/Q.q); (donde Q.q == 1);
    //de modo que puede haber una posición estable para p___3, que esté
    //por debajo de Qmin.
    //
    //Si embargo es poco probable que eso suponga un inconveniente, ya que:
    //  Qtheta___3(theta___3) utiliza la función Q, de modo que no
    //devolverá una posición estable que haga que p___3 se salga de
    //[Q.Qmin, Q.Qmax], y cuando asigna un valor a theta___3, este se irá
    //a la posición estable más próxima devuelta por Qtheta___3(theta___3).

    //------------------------------------------------------------------
    //PROPIEDADES DE ORIENTACIÓN EN PASOS
    //DE LECTURA/ESCRITURA:

    //Valor aproximado de referencia del límite inferior
    //del intervalo de orientación de P3 en S3 en pasos.
    //El ángulo p___3min debe estar en el dominio imagen de F(theta___3).
    //Lectura:
    //  p___3min = F(theta___3min);
    //Escritura:
    //  p_theta___3min = G(p___3min);
    //Valor por defecto: F(MEGARAtheta___3min) steps.
    double getp___3min(void) const; void setp___3min(double);
    //Valor aproximado de referencia del límite superior
    //del intervalo de orientación de P3 en S3 en pasos.
    //Lectura:
    //  p___3max = F(theta___3max);
    //Escritura:
    //  p_theta___3max = G(p___3max);
    //Valor por defecto: F(MEGARAtheta___3max) steps.
    double getp___3max(void) const; void setp___3max(double);

    //Valor aproximado de referencia del ángulo de orientación
    //de P3 en S3 en pasos.
    //Lectura:
    //  if(Quantify___)
    //      p___3 = Q(F(theta___3));
    //  else
    //      p___3 = F(theta___3);
    //Escritura:
    //  if(Quantify___)
    //      p_theta___3 = G(Q(p___3));
    //  else
    //      p_theta___3 = G(p___3);
    //Valor por defecto: Q(F(0)) steps.
    //En lectura p___3 se cuantifica para evitar el error numérico
    //introducido por la función F(theta___3).
    double getp___3(void) const; void setp___3(double);

    //cuando Quantify___ es verdadero:
    //  ninguna asignación a p___3 puede dar error
    //  es cuantificada tambien la lectura evitando el error numérico

    //PROPIEDADES DE ORIENTACIÓN EN PASOS
    //DE SOLO LECTURA:

    //first stable position angle of rotor 2 in steps:
    //  p___3first = ceil(F(theta___3first));
    double getp___3first(void) const {
        return ceil(getF().Image(gettheta___3first()));}
    //las stable position angle of rotor 2 in steps:
    //  p___3last = floor(F(theta___3last));
    double getp___3last(void) const {
        return floor(getF().Image(gettheta___3last()));}

    //Note that F(0) shall be 0 for avoid negative positions in steps,
    //and the controller of therotor could notwork with negativevalues.

    //------------------------------------------------------------------
    //PROPIEDADES DE LOCALIZACIÓN
    //DE LECTURA/ESCRITURA:

    //posición del eje de rotación del brazo en S0
    TDoublePoint getP1(void) const {return P1;}
    void setP1(TDoublePoint);

    //margen perimentral de seguridad
    //debe ser mayor que cero
    //valor por defecto: MEGARA_SPMgenPairPPDP_p mm
    double getSPM(void) const {return SPM;}
    void setSPM(double);

    //The SPM will be selected according (PAkd, Purpose):
    //  PAkd == kdPre:
    //      Asignación:
    //          FP->Arm->SPM = FP->SPMall_p;
    //      Programación:
    //          FP->Arm->SPM = FP->SPMpro_p;
    //      Validación:
    //          FP->Arm->SPM = FP->SPMval_p;
    //      Ejecución simulada:
    //          FP->Arm->SPM = FP->SPMexe_p;
    //  PAkd == kdApp:
    //      Asignación:
    //          FP->Arm->SPM = FP->SPMall_a;
    //      Programación:
    //          FP->Arm->SPM = FP->SPMpro_a;
    //      Validación:
    //          FP->Arm->SPM = FP->SPMval_a;
    //      Ejecución simulada:
    //          FP->Arm->SPM = FP->SPMexe_a;
    //  PAkd == kdUnk:
    //      En todos los casos:
    //          FP->Wall->SPM = FP->SPMwall;

    //PROPIEDADES DE LOCALIZACIÓN
    //DE SOLO LECTURA:

    //centro teórico de la lente de la fibra en S0
    const TDoublePoint getP2(void) const {return P2;}
    //centro de la lente del brazo en S0
    const TDoublePoint getP3(void) const {return P3;}
    //vértice longitudinal del brazo en S0
    const TDoublePoint getV(void) const {return V;}

    //contorno del brazo correspondiente a la plantilla
    //ubicada y orientada en S0
    const TContourFigureList &getContour(void) const {
        return (const TContourFigureList&)Contour;}

    //------------------------------------------------------------------
    //PROPIEDADES DE PLANTILLA
    //DE LECTURA/ESCRITURA EN FORMATO TEXTO

    AnsiString getL12Text(void) const;
    void setL12Text(const AnsiString&);
    AnsiString getP__2Text(void) const;
    void setP__2Text(AnsiString&);

    AnsiString getL13Text(void) const;
    void setL13Text(const AnsiString&);
    AnsiString gettheta__3Text(void) const;
    void settheta__3Text(const AnsiString&);
    AnsiString gettheta__O3Text(void) const;
    void settheta__O3Text(const AnsiString&);
    AnsiString getP__3Text(void) const;
    void setP__3Text(AnsiString&);

    AnsiString getR3Text(void) const;
    void setR3Text(const AnsiString&);

    //dirección en memoria de la propiedad compuesta Contour____
    AnsiString getContour____AddressText(void) const;
    //propiedad Contour____ en formato texto lista
    void setContour____Text(const AnsiString&);
    //propiedad Contour____ en formato texto columna
    void setContour____ColumnText(const AnsiString&);

    //PROPIEDADES DE PLANTILLA
    //DE SOLO LECTURA EN FORMATO TEXTO

    AnsiString getP____2Text(void) const;
    AnsiString getP____1Text(void) const;

    //dirección en memoria de la propiedad compuesta Contour__
    AnsiString getContour__AddressText(void) const;

    AnsiString getL1VText(void) const;
    AnsiString getV__Text(void) const;

    //------------------------------------------------------------------
    //PROPIEDADES DE ORIENTACIÓN EN RADIANES
    //DE LECTURA/ESCRITURA EN FORMATO TEXTO

    AnsiString getthetaO3Text(void) const;
    void setthetaO3Text(const AnsiString&);

    AnsiString gettheta___3minText(void) const;
    void settheta___3minText(const AnsiString&);
    AnsiString gettheta___3maxText(void) const;
    void settheta___3maxText(const AnsiString&);
    AnsiString gettheta___3Text(void) const;
    void settheta___3Text(const AnsiString&);
    AnsiString gettheta3Text(void) const;
    void settheta3Text(const AnsiString&);

    //La siguiente propiedad puede ser accedida directamente
    //tanto en lectura como en escritura:
    //  theta___3s.Text

    AnsiString getthetaO2Text(void) const;
    void setthetaO2Text(const AnsiString&);

    AnsiString gettheta___2minText(void) const;
    void settheta___2minText(const AnsiString&);
    AnsiString gettheta___2maxText(void) const;
    void settheta___2maxText(const AnsiString&);
    AnsiString gettheta___2Text(void) const;
    void settheta___2Text(const AnsiString&);
    AnsiString gettheta2Text(void) const;
    void settheta2Text(const AnsiString&);

    //------------------------------------------------------------------
    //PROPIEDADES DE CUANTIFICACIÓN
    //DE LECTURA/ESCRITURA EN FORMATO TEXTO

    //dirección en memoria de la función F
    AnsiString getFAddressText(void) const;
    //puntos de lafunción F enformato lista
    void setFPointsText(const AnsiString&);
    //puntos de lafunción F enformato tabla
    void setFTableText(const AnsiString&);

    AnsiString getQuantify___Text(void) const;
    void setQuantify___Text(const AnsiString&);

    //ADVERTENCIA: Quantify___Text admitiera solo mayúsculas "True" o "False",
    //ya que utiliza la función StrPrintBool en vez de StrToBool,
    //ya que el uso de StrToBool se considera inapropiado, por no comprobar
    //la condición:
    //  FalseBoolStrs[0] = "False" y
    //  TrueBoolStrs[0] = "True".

    //La siguiente propiedad puede ser accedida directamente,
    //tanto en lectura como en escritura:
    //  Quantify___s.Text

    //-------------------------------------------------------------------
    //PROPIEDADES DE CUANTIFICACIÓN
    //DE SOLO LECTURA EN FORMATO TEXTO:

    //dirección en memoria de la función G
    AnsiString getGAddressText(void) const;

    AnsiString getSB2Text(void) const;
    AnsiString getrbsText(void) const;

    //La siguiente propiedad representa a todo Q:
    //  Q.AssignsText
    //y como tiene un número acotado de líneas
    //es preferible mostrarlas en vez de
    //la dirección en memoria de Q.

    //------------------------------------------------------------------
    //PROPIEDADES DE ORIENTACIÓN EN PASOS
    //DEPENDIENTES EN FORMATO TEXTO

    AnsiString getp___3minText(void) const;
    void setp___3minText(const AnsiString&);
    AnsiString getp___3maxText(void) const;
    void setp___3maxText(const AnsiString&);
    AnsiString getp___3Text(void) const;
    void setp___3Text(const AnsiString&);

    //------------------------------------------------------------------
    //PROPIEDADES DE LOCALIZACIÓN
    //DE LECTURA/ESCRITURA EN FORMATO TEXTO

    AnsiString getP1Text(void) const; void setP1Text(const AnsiString&);

    AnsiString getSPMText(void) const; void setSPMText(const AnsiString&);

    //PROPIEDADES DE LOCALIZACIÓN
    //DE SOLO LECTURA EN FORMATO TEXTO:

    AnsiString getP2Text(void) const;
    AnsiString getP3Text(void) const;
    AnsiString getVText(void) const;

    //dirección en memoria de la propiedad compuesta Contour
    AnsiString getContourAddressText(void) const;

    //------------------------------------------------------------------
    //CONJUNTOS DE TODAS LAS PROPIEDADES EN FORMATO TEXTO:

    //conjunto de propiedades de plantilla
    //en formato texto
    AnsiString getTemplateText(void) const;
    //conjunto de propiedades de orientación en radianes
    //en formato texto
    AnsiString getOrientationRadiansText(void) const;
    //conjunto de propiedades de cuantificación
    //en formato texto
    AnsiString getQuantificationText(void) const;
    //conjunto de propiedades de orientación en pasos
    //en formato texto
    AnsiString getOrientationStepsText(void) const;
    //conjunto de propiedades de localización
    //en formato texto
    AnsiString getLocationText(void) const;

    //conjunto de todas las propiedades
    //en formato texto
    AnsiString getAllText(void) const;

    //-------------------------------------------------------------------
    //CONJUNTOS DE LAS PROPIEDADES DE INSTANCIA EN FORMATO TEXTO:

    //conjunto de todas las propiedades de instancia
    //en formato texto
    AnsiString getInstanceText(void) const;
    void setInstanceText(const AnsiString&);

    //-------------------------------------------------------------------
    //MÉTODOS ESTÁTICOS:

    //lee una instancia de brazo en una cadena
    static void  readInstance(TArm *A, const AnsiString& S, int& i);

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

    //-------------------------------------------------------------------
    //MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

    //construye un brazo
    //con la posición y orientación indicadas
    TArm(TDoublePoint P1, double thetaO2=-M_PI);
    //clona todas las propiedades de un brazo
    void clone(TArm*);
    //contruye un clon de un brazo
    TArm(TArm*);
    //libera la memoria dinámica
    ~TArm();

    //-------------------------------------------------------------------
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
                     double theta__3=0, double R3=0.75);
    //asigna conjuntamente las propiedades de orientación en radianes
    void setOrientationRadians(double theta___3min, double theta___3max,
                               double theta___3);
    //asigna conjuntamente las propiedades de cuantificación
    void setQuantification(double SB2);

    //obtiene el contorno del perímetro de seguridad del brazo
    void getSecurityContour(TContourFigureList& securityContour);

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
    //  Vkd == kdPre:
    //      ejecución:
    //          SPMexe_p = SPMmec + SPMnum + SPMvar
    //      validación:
    //          SPMval_p = SPMexe_p + SPMmin
    //      programación:
    //          SPMpro_p = SPMval_p + SPMmin
    //      asignación:
    //          SPMall_p = SPMpro_p + SPMoff
    //  Vkd == kdApp:
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
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // ARM_H
