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
//Archivo: Arm.h
//Contenido: clase brazo de posicionador
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ARM_H
#define ARM_H

#include "Barrier.h"
#include "FiberMOSModelConstants.h"
#include "Constants.h"

//---------------------------------------------------------------------------

using namespace Mathematics;
using namespace Operators;
using namespace Lists;
//#using namespace Graphics;

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TArm
//---------------------------------------------------------------------------

//SUBSISTEMAS Y ENCAPSULACIÓN DEL SISTEMA POSICIONADOR DE FIBRA:

//Un posicionador de fibra se concibe compuesto de dos subsistemas:
//      - las propiedades de dimensionamiento;
//      - las propiedades de seguridad.
//Las propiedades de seguridad solo son usadas para determinar las
//posiciones finales de las funciones de movimiento.
//Además las propiedades de seguridad dependen de las propiedades
//de dimensionamiento de todos los posicionadores del sistema, por lo que
//su cálculo debe posponerse hasta la definción del posicionador multifibra.

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

//De modo qu<e se dispone de dos opciones para definir las clases que componen
//el sistema de posicionadores:
//      1. respetar estrictamente el criterio de encapculación restringiendo el
//      acceso a paráemtros de dimensionamiento de los posicionadores,
//      programando una invocación a la función que calcula los parámetros
//      de seguridad despues de cambiar cada parametro.
//      2. delegar en el programador la invocación a la función que calcula
//      los parámetros de seguridad cada vez que se cambien los parámetros.

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
//de TWall, ya que la no menclatura difiere:
//En TWall:
//      Contour_ <---------
//      L1V
//      thetaO1 <----------
//      P0 <---------------
//      Contour
//      SPM
//En TArm:
//      Contour____ <------
//      L1V
//      thetaO3 <----------
//      P1 <---------------
//      Contour
//      SPM

//clase brazo de posicionador
class TArm {
protected:
        //-------------------------------------------------------------------
        //PROPIEDADES DE PLANTILLA
        //DE LECTURA/ESCRITURA:

        double __L12;
        TDoublePoint __P____2;

        double __L13;
        double __theta____3;
        TDoublePoint __P____3;

        double __R3;

        TContourFigureList __Contour____;

        //PROPIEDADES DE PLANTILLA
        //DE SOLO LECTURA:

        double __L1V;
        TDoublePoint __V____;

        //-------------------------------------------------------------------
        //PROPIEDADES DE ORIENTACIÓN EN RADIANES
        //DE LECTURA/ESCRITURA:

        double __thetaO3;

        double __theta___3min;
        double __theta___3max;
        double __theta___3;

        //-------------------------------------------------------------------
        //PROPIEDADES DE CUANTIFICACIÓN
        //DE LECTURA/ESCRITURA:

        TFunction __F;

        bool __Quantify___;

        //PROPIEDADES DE CUANTIFICACIÓN
        //DE SOLO LECTURA:

        TFunction __G;

        //ADVERTENCIA: la propiedad G debe ser definida mediante:
        //      __property const TFunction G = {read=GetG};
        //Si se definiera mediante:
        //      __property const TFunction G = {read=__G};
        //no sería considerada constante.

        double __SB2;
        double __rbs;

        TQuantificator __Q;

        //-------------------------------------------------------------------
        //PROPIEDADES DE ORIENTACIÓN EN PASOS
        //DE LECTURA/ESCRITURA:

        //-------------------------------------------------------------------
        //PROPIEDADES DE LOCALIZACIÓN
        //DE LECTURA/ESCRITURA:

        TDoublePoint __P1;

        double __SPM;

        //PROPIEDADES DE LOCALIZACIÓN
        //DE SOLO LECTURA:

        TDoublePoint __P2;
        TDoublePoint __P3;
        TDoublePoint __V;

        TContourFigureList __Contour;

        //##################################################################
        //MÉTODOS PRIVADOS:
        //##################################################################

        //------------------------------------------------------------------
        //MÉTODOS DE ASIMILACIÓN:

        //Apartir de:
        //      {L12, L13, theta____3},
        //      {Contour____}
        //Determina:
        //      {P____2, P____3},
        //      {V____, L1V}
        void ProcessateTemplate(void);

        //A partir de:
        //      {F(theta___3)}.
        //Determina:
        //      {G(p___3), SB2, rbs},
        //      {theta___3min, theta___3max}.
        //      {Q(p___3)}
        //Restricciones:
        //      La función F(theta___3) debe ser monótona creciente.
        void ProcessateF(void);

        //Nótese que no se imponen las siguientes restricciones:
        //      La función F(theta___3) debe estar definida en 0.
        //      F(0) debería ser igual a cero.
        //      La función F(theta___3) debe estar definida en M_2PI.

        //a partir de:
        //      {P____2, P____3}
        //      {Contour____, V____}
        //      {P1, theta2}
        //Determina:
        //      {P2, P3}
        //      {Contour, V},
        void CalculateImage(void);

        //ADVERTENCIA: el únicométodo de asimilación que necesita tener
        //alto rendimiento es CalculateImage, ya que (P1, theta2)
        //son variables que pueden cambiar rápidamente.

public:
        //##################################################################
        //PROPIEDADES PÚBLICAS:
        //##################################################################

        //------------------------------------------------------------------
        //PROPIEDADES DE PLANTILLA
        //DE LECTURA/ESCRITURA:

        //Las propiedades seleccionadas para la instancia son:
        //      (L12, L13, theta__O3, R3, Contour____)

        //posición del eje 2 en S4
        //valor contante: (0, 0)
        const TDoublePoint P____1;

        //distancia desde eje del brazo hasta
        //el punto de referencia del cabezal de la plantilla
        //distancia de P1 a P2
        //debe ser mayor que cero
        //valor por defecto: MEGARAL mm
        double getL12(void) const {return __L12;}
        void setL12(double);
        //punto de referencia del cabezal de la plantilla en S4
        TDoublePoint getP____2(void) const {return __P____2;}
        void setP____2(const TDoublePoint&);

        //El valor por defecto de L12 se va a usar como
        //valor por defecto para L01.

        //coordenada radial de P3 en S4
        //debe ser no negativa
        //valor por defecto: MEGARAL mm
        double getL13(void) const {return __L13;}
        void setL13(double);
        //coordenada angular de P3 en S4
        //puede sercualquier valor
        //pero esrecomendable que esté en (-M_PI, M_PI]
        //valor por defecto: 0 rad
        double gettheta____3(void) const {return __theta____3;}
        void settheta____3(double);
        //orientación del origen de coordenadas de S3 en S2
        //es igual que theta____3
        //valor por defecto: 0 rad
        double gettheta__O3(void) const {return __theta____3;}
        void settheta__O3(double);
        //centro de la lente de la plantilla en S4
        TDoublePoint getP____3(void) const {return __P____3;}
        void setP____3(const TDoublePoint&);

        //radio de la lente del brazo
        //debe ser mayor que cero
        //valor por defecto: 0.75 mm
        double getR3(void) const {return __R3;}
        void setR3(double);

        //plantilla del contorno del brazo en S4
        //valor por defecto: MEGARA_Contour____
        const TContourFigureList &getContour____(void) const {
                return (const TContourFigureList&)__Contour____;}
        void setContour____(const TContourFigureList &_Contour____);

        //ADVERTENCIA: la escritura de Contour____.Text conviene hacerla
        //a tavés de la propiedad Contour____Text, con un valor previamente
        //leido, de modo que el valor delectura en modo texto coincida
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

        //radio máximo de la circunferencia descrita por
        //el ápice del brazo al girar (en mm)
        double getL1V(void) const {return __L1V;}
        //vértice del cabezal de la plantilla en S4
        TDoublePoint getV____(void) const {return __V____;}

        //V____ se refiere al punto de intersección
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
        //      ([theta___3min, theta___3max], theta___3),
        //en vez de:
        //      ([theta___2min, theta___2max], theta___2) o
        //      ([p___3min, p___3max] y p___3),
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
        //      double IsntInDomaintheta___3(double theta___3);
        //      double IsntInDomaintheta___2(double theta___2);
        //      double IsntInDomainp___3(double p___3);
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
        //      p___3 = F(theta___3);
        //      p___3 = Q(p___3);
        //      theta___3 = G(p___3);
        //y la función R no será representada.

        //Las propiedades seleccionadas para la instancia son:
        //      (thetaO3, theta___3, theta___2min, theta___2max)

        //orientación del origen de coordenadas de S3 en S0
        //cuando el eje 2 está en el origen
        //valor por defecto: PI/2 rad
        double getthetaO3(void) const {return __thetaO3;}
        void setthetaO3(double);

        //límite inferior del intervalo de giro de P3 en S3
        //debe estar en el dominio de definición de F(theta___3)
        //no debe ser mayor que theta___3max
        //valor por defecto: MEGARA_theta___3min rad
        double gettheta___3min(void) const {return __theta___3min;}
        void settheta___3min(double);
        //límite superior del intervalo de giro de P3 en S3
        //debe estar en el dominio de definición de F(theta___3)
        //no debe ser menor que theta___3min
        //valor por defecto: MEGARA_theta___3max rad
        double gettheta___3max(void) const {return __theta___3max;}
        void settheta___3max(double);
        //ángulo de orientación de P3 en S3
        //debe estar en el dominio de definición de F(theta___3)
        //Escrutura:
        //      if(Quantify___)
        //              __theta___3 = Qtheta___3(_theta___3);
        //      else
        //              __theta___3 = theta___3;
        //valor por defecto: 0 rad
        double gettheta___3(void) const {return __theta___3;}
        void settheta___3(double);
        //ángulo de orientación de P3 en S0
        //theta3 debe estar en su dominio
        //Lectura:
        //      theta3 = thetaO2 + theta__O3 + theta___3
        //Escritura:
        //      theta___3 = theta3 - thetaO2 - theta__O3
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
        //      thetaO2 = thetaO3 - theta__O3
        //Escritura:
        //      __thetaO3 = thetaO2 + theta__O3
        double getthetaO2(void) const; void setthetaO2(double);

        //valor aproximado de referencia del
        //límite inferior del intervalo de giro de P2 en S3
        //debe estar en su dominio
        //debe ser menor que theta___2max
        //Lectura:
        //      theta___2min = theta___3min - theta___3;
        //Escritura:
        //      __theta___3min = theta___2min + theta___3;
        //valor por defecto: MEGARAtheta__2min rad
        double gettheta___2min(void) const; void settheta___2min(double);
        //valor aproximado de referencia del
        //límite superior del intervalo de giro de P2 en S3
        //debe estar en su dominio
        //debe ser mayor que theta___2min
        //Lectura:
        //      __theta___2max = theta___3max - theta___3;
        //Escritura:
        //      theta___3max = theta___2max + theta___3;
        //valor por defecto: MEGARAtheta__2max rad
        double gettheta___2max(void) const; void settheta___2max(double);
        //valor aproximado de referencia del
        //ángulo de orientación de P2 en S3
        //debe estar en su dominio
        //Lectura:
        //      theta___2 = theta___3 - theta___23
        //Escritura:
        //      double _theta___3 = theta___2 + theta___23;
        //      if(IsntInDomaintheta___3(theta___3))
        //              if(IsntInDomaintheta___2(theta___2)
        //                      throw EImproperArgument("value for theta___2 isn't in domain");
        //              else
        //                      _theta___3 = ConstrainInDomaintheta___3(_theta___3);
        //      theta___3 = _theta___3;
        //la escritura en theta___3 tendrá en cuenta la cuantificación
        //valor por defecto: 0 - 0 rad
        double gettheta___2(void) const; void settheta___2(double);
        //ángulo de orientación de P2 en S0
        //theta2 debe estar en su dominio
        //Lectura:
        //      theta2 = theta3 - theta___23
        //Escritura:
        //      theta3 = theta2 + theta___23
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

        //ORIENTATION PROPERTIES IN RAD R:

        //first position angle of rotor 2 in rad
        //  theta___3first = max(0, theta___3min)
        double gettheta___3first(void) const {
            return max(0., gettheta___3min());}
        //last position angle of rotor 2 in rad
        //  theta___3last = min(M_2PI, theta___3max)
        double gettheta___3last(void) const {
            return min(M_2PI, gettheta___3max());}

        //-------------------------------------------------------------------
        //PROPIEDADES DE CUANTIFICACIÓN
        //DE LECTURA/ESCRITURA:

        //función de transformación de de radianes a pasos de P3
        //debe estar definida en algún punto
        //debe ser monótona creciente
        //valor por defecto:
        //      {(M_2PI, MEGARASB2),
        //      (0, 0),
        //      (M_2PI, MEGARASB2)}
        const TFunction& getF(void) const {return (const TFunction&)__F;}
        void setF(const TFunction &_F);

        //Nótese que no se imponen las siguientes restricciones:
        //      La función F(theta___3) debe estar definida en 0.
        //      F(0) debería ser igual a cero.
        //      La función F(theta___3) debe estar definida en M_2PI.

        //ADVERTENCIA: F.PointsText debe ser escrito mediante la propiedad
        //FPointsText de modo que se invoque al método SetFPointsText,
        //el cual invocará a SetF, el cual comprobará que F cumple
        //las restricciones y asimilará F mediante:
        //      CalculateG();
        //      CalculateQ();
        //      theta___3 = theta___3;

        //interruptor de cuantificación de p___3
        //indica si deben cuantificarse los valores asignados a p___3
        //valor por defecto: true
        bool getQuantify___(void) const {return __Quantify___;}
        void setQuantify___(bool);

        //Cuando Quantify___ es activado theta___3 es situado en la posición
        //estable más próxima mediante la reasignación así mismo:
        //      theta___3 = theta___3;

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
        const TFunction& getG(void) const {return (const TFunction&)__G;}

        //número de pasos de una vuelta completa del eje 2 (SB: Steps Back)
        //es un número real ya que a la salida de la reductora
        //el número de pasos puede ser fraccionario
        //aunque convendría que fuera par ya que solo va a girar media vuelta
        //valor por defecto: F(M_2PI) = SB2SIDE steps
        double getSB2(void) const {return __SB2;}
        //radianes por paso medio en el intervalo [0, M_2PI]
        //      rbs = M_2PI/SB2;
        //valor por defecto: M_2PI/SB2SIDE rad
        double getrbs(void) const {return __rbs;}

        //función de cuantificación lineal de p___3
        //valor por defecto: (1, [p___3min, p___3max])
        const TQuantificator& getQ(void) const {
                return (const TQuantificator&)__Q;}

        //NOTA: la constricción al dominio de p___3 se realiza en la función
        //de cuantificación Q, cuyo dominio imagen es [Q.Qmin, Q.Qmax].

        //ADVERTENCIA: cuando theta___3min está ligeramente por debajo de
        //un centroide (o posición estable), p___3 puede ser calculado con
        //un error numérico que lo desplace por encima de dicho centroide
        //entonces se debe saber que Q.Qmin es calculado del siguiente modo:
        //      p___3min = F(theta___3min);
        //      Q.Qmin = ceil(p___3min/Q.q); (donde Q.q == 1);
        //de modo que puede haber una posición estable para p___3, que esté
        //por debajo de Qmin.
        //
        //Si embargo es poco probable que eso suponga un inconveniente, ya que:
        //      Qtheta___3(theta___3) utiliza la función Q, de modo que no
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
        //      p___3min = F(theta___3min);
        //Escritura:
        //      __theta___3min = G(p___3min);
        //Valor por defecto: F(MEGARAtheta___3min) steps.
        double getp___3min(void) const; void setp___3min(double);
        //Valor aproximado de referencia del límite superior
        //del intervalo de orientación de P3 en S3 en pasos.
        //Lectura:
        //      p___3max = F(theta___3max);
        //Escritura:
        //      __tehta___3max = G(p___3max);
        //Valor por defecto: F(MEGARAtheta___3max) steps.
        double getp___3max(void) const; void setp___3max(double);

        //Valor aproximado de referencia del ángulo de orientación
        //de P3 en S3 en pasos.
        //Lectura:
        //      if(Quantify___)
        //              p___3 = Q(F(theta___3));
        //      else
        //              p___3 = F(theta___3);
        //Escritura:
        //      if(Quantify___)
        //              __theta___3 = G(Q(p___3));
        //      else
        //              __theta___3 = G(p___3);
        //Valor por defecto: Q(F(0)) steps.
        //En lectura p___3 se cuantifica para evitar el error numérico
        //introducido por la función F(theta___3).
        double getp___3(void) const; void setp___3(double);

        //cuando Quantify___ es verdadero:
        //      ninguna asignación a p___3 puede dar error
        //      es cuantificada tambien la lectura evitando el error numérico

        //ORIENTATION PROPERTIES IN STEPS R/W:

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
        TDoublePoint getP1(void) const {return __P1;}
        void setP1(TDoublePoint);

        //margen perimentral de seguridad
        //debe ser mayor que cero
        //valor por defecto: MEGARA_SPMall_p mm
        double getSPM(void) const {return __SPM;}
        void setSPM(double);

        //Cuando el posicionador sea construido en el posicionador multifibra,
        //el SPM del brazo de cada posicionador, será inicializado al valor
        //del SPM de asignación precisa (SPMall_p), el cual es igual a:
        //      SPMmec + SPMnum + SPMvar + SPMrec + SPMmin + SPMmin + SPMoff.

        //El SPM de cada posicionador será establecido en cada posicionador
        //multifibra atendiendo a la operación que se va a realizar, y
        //el grado de conocimiento de los ejes del posicionados:
        //      PAkd == kdPRECISE:
        //              Asignación:
        //                      FP->Arm->SPM = FP->SPMall_p;
        //              Programación:
        //                      FP->Arm->SPM = FP->SPMpro_p;
        //              Validación:
        //                      FP->Arm->SPM = FP->SPMval_p;
        //              Ejecución simulada:
        //                      FP->Arm->SPM = FP->SPMexe_p;
        //      PAkd == kdAPROXIMATE:
        //              Asignación:
        //                      FP->Arm->SPM = FP->SPMall_a;
        //              Programación:
        //                      FP->Arm->SPM = FP->SPMpro_a;
        //              Validación:
        //                      FP->Arm->SPM = FP->SPMval_a;
        //              Ejecución simulada:
        //                      FP->Arm->SPM = FP->SPMexe_a;
        //      PAkd == kdUNKNOWN:
        //              En todos los casos:
        //                      FP->Wall->SPM = FP->SPMwall;

        //PROPIEDADES DE LOCALIZACIÓN
        //DE SOLO LECTURA:

        //centro teórico de la lente de la fibra en S0
        const TDoublePoint getP2(void) const {return __P2;}
        //centro de la lente del brazo en S0
        const TDoublePoint getP3(void) const {return __P3;}
        //vértice longitudinal del brazo en S0
        const TDoublePoint getV(void) const {return __V;}

        //contorno del brazo correspondiente a la plantilla
        //ubicada y orientada en S0
        const TContourFigureList &getContour(void) const {
                return (const TContourFigureList&)__Contour;}

        //##################################################################
        //PROPIEDADES PÚBLICAS EN FORMATO TEXTO:
        //##################################################################

        //------------------------------------------------------------------
        //PROPIEDADES DE PLANTILLA
        //DE LECTURA/ESCRITURA EN FORMATO TEXTO

        AnsiString getP____1Text(void) const;
        AnsiString getL12Text(void) const;
        void setL12Text(const AnsiString&);
        AnsiString getP____2Text(void) const;
        void setP____2Text(AnsiString&);

        AnsiString getL13Text(void) const;
        void setL13Text(const AnsiString&);
        AnsiString gettheta____3Text(void) const;
        void settheta____3Text(const AnsiString&);
        AnsiString gettheta__O3Text(void) const;
        void settheta__O3Text(const AnsiString&);
        AnsiString getP____3Text(void) const;
        void setP____3Text(AnsiString&);

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

        AnsiString getL1VText(void) const;
        AnsiString getV____Text(void) const;

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
        //      theta___3s.Text

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
        //      FalseBoolStrs[0] = "False" y
        //      TrueBoolStrs[0] = "True".

        //La siguiente propiedad puede ser accedida directamente,
        //tanto en lectura como en escritura:
        //      Quantify___s.Text

        //PROPIEDADES DE CUANTIFICACIÓN
        //DE SOLO LECTURA EN FORMATO TEXTO:

        //dirección en memoria de la función G
        AnsiString getGAddressText(void) const;

        AnsiString getSB2Text(void) const;
        AnsiString getrbsText(void) const;

        //La siguiente propiedad representa a todo Q:
        //      Q.AssignsText
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
        //conjunto de propiedades de cuantificación
        //en formato texto
        AnsiString getQuantificationText(void) const;
        //conjunto de propiedades de orientación en radianes
        //en formato texto
        AnsiString getOrientationRadiansText(void) const;
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

/*        //conjunto de propiedades de instancia de la plantilla
        //en formato texto
        __property AnsiString InstanceTemplateText = {
                read=GetInstanceTemplateText};
        //conjunto de propiedades de instancia de la cuantificación
        //en formato texto
        __property AnsiString InstanceQuantificationText = {
                read=GetInstanceQuantificationText};
        //conjunto de propiedades de instancia de la orientación en radianes
        //en formato texto
        __property AnsiString InstanceOrientationRadiansText = {
                read=GetInstanceOrientationRadiansText};
        //conjunto de propiedades de instancia de la orientación en pasos
        //en formato texto
        __property AnsiString InstanceOrientationStepsText = {
                read=GetInstanceOrientationStepsText};
        //conjunto de propiedades de instancia de la localización
        //en formato texto
        __property AnsiString InstanceLocationText = {
                read=GetInstanceLocationText};
  */
        //conjunto de todas las propiedades de instancia
        //en formato texto
        AnsiString getInstanceText(void) const;
        void setInstanceText(const AnsiString&);

        //##################################################################
        //MÉTODOS PÚBLICOS:
        //##################################################################
/*
        //-------------------------------------------------------------------
        //MÉTODOS ESTÁTICOS:

        //lee un cluster de configuración en una cadena de texto
        static void  ReadTemplate(TArm *Arm,
                const AnsiString &S, int &i);
        //lee un cluster de configuración en una cadena de texto
        static void  ReadOrientationRadians(TArm *Arm,
                const AnsiString &S, int &i);
        //lee un cluster de configuración en una cadena de texto
        static void  ReadQuantification(TArm *Arm,
                const AnsiString &S, int &i);
        //lee un cluster de configuración en una cadena de texto
        static void  ReadOrientationSteps(TArm *Arm,
                const AnsiString &S, int &i);
        //lee un cluster de configuración en una cadena de texto
        static void  ReadLocation(TArm *Arm,
                const AnsiString &S, int &i);

        //lee un cluster de instancia en una cadena de texto
        static void  ReadInstance(TArm *Arm,
                const AnsiString &S, int &i);
      */

        //lee una instancia de posicionador de fibra en una cadena
        static void  ReadInstance(TArm* &A,
                const AnsiString& S, int &i);

        //MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

        //construye un brazo
        //con la posición y orientación indicadas
        TArm(TDoublePoint _P1, double _thetaO2=M_PI/2);
        //copia todas las propiedades de un brazo
        void Copy(TArm*);
        //contruye un clon de un brazo
        TArm(TArm*);
        //libera la memoria dinámica
        ~TArm();

        //MÉTODOS DE TRANSFORMACIÓN ENTRE SISTEMAS DE COORDENADAS:

        //transforma la coordenada angular polar de S3 a S2
        //      theta__ = theta___ + theta__O3
        double S3thetaToS2theta(double theta___) const;
        //transforma las coordenadas polares en S2
        //en coordenadas rectangulares en S2
        TDoublePoint S2polToS2rec(double r__, double theta__) const;

        //MÉTODOS PARA DETERMINAR LA PERTENENCIA A DOMINIOS DE ORIENTACIÓN:

        //determina si un ángulo en radianes está fuera
        //del intervalo [theta__3min, theta__3max]
        bool IsntInDomaintheta___3(double _theta___3);
        //determina si un ángulo en radianes está fuera
        //del intervalo [theta___2min, theta___2max]
        //      IsntInDomaintheta___2(theta___2) =
        //              IsntInDomaintheta___3(theta___2 + theta___23)
        bool IsntInDomaintheta___2(double _theta___2);

        //determina si un ángulo en pasos está fuera
        //del intervalo [p___3min, p___3max]
        //      IsntInDomainp___3(p___3) = IsntInDomaintheta___3(G(p___3))
        bool IsntInDomainp___3(double _p___3);

        //MÉTODOS PARA DETERMINAR VALORES CUANTIFICADOS DE ORIENTACIONES:

        //cuantifica theta___3
        double Qtheta___3(double theta___3);
        //cuantifica theta___2
        double Qtheta___2(double theta___2);

        //MÉTODOS DE CONFIGURACIÓN:

        //asigna conjuntamente las propiedades de plantilla
        //si no espeficifa argumentos se asignan valores por defecto
        void SetTemplate(double _L12=MEGARA_L, double _L13=MEGARA_L,
                double _theta____3=0, double _R3=0.75);
        //asigna conjuntamente las propiedades de orientación en radianes
        void SetOrientationRadians(double _thetaO3,
                double _theta___3min, double _theta___3max,
                double _theta___3);
        //asigna conjuntamente las propiedades de cuantificación
        void SetQuantification(double _SB2);

        //MÉTODOS PARA DETERMIANAR DISTANCIAS:

        //Para determinar la distancia máxima del contorno de este brazo a
        //un punto, debe hacerse mediante:
        //      double dmax = Contour.DistanceMax(P);
        //donde P puede ser el eje del brazo en cuyo caso se obtendría L1V.

        //Para determinar la distancia mínima del contorno de este brazo a
        //otro contorno, debe hacerse mediante:
        //      double dmin = Contour.DistanceMin(Contour);
        //donde Contour puede ser el contorno de otro brazo
        //o cualquier otro obstáculo.

        //ADVERTENCIA: se recuerda que las distancias aquí calculadas no son
        //con el cuerpo del brazo si no con el contorno del mismo.

        //MÉTODOS PARA DETERMINAR SI HAY COLISIÓN:

        //Determina si la distancia entre el contorno de un brazo y
        //el contorno de este brazo, es inferior a la suma del SPM
        //de cada uno de ellos.
        //Si la distancia entre los ejes de los brazos es t.q:
        //      Mod(Arm->P2 - P2) >= Arm->L1V+Arm->SPM + L1V+SPM + ERR_NUM
        //no invoca a Contour.DistanceMin, y devuelve directamente false.
        bool Collides(TArm *Arm);

        //NOTA: la función que determina el estado de
        //colisión tiene un tiempo de procesado medio
        //inferior a la de determinación de la distancia,
        //por que detiene el proceso cuando la colisión es detectada.

        //Contantes a partir de las cuales se determinan los valores de SPM:
        //      SPMmec: para el error mecánico. (10 um)
        //      SPMrec: para el error de recuperación aproximada. (TBD)
        //      SPMnum: para el error numérico. (10^-13)
        //      SPMvar: para absorber las variaciones de velocidad
        //              debidas a los motores paso a paso. (~rbs*Rc)
        //      SPMmin: para absorber la incertidumbre del salto mínimo
        //              durante la programación. (~10 um)
        //      SPMoff: para absorver las aproximaciones producidas
        //              por corrección del offset. (~offset*(Rc/L13))

        //Valor de SPM para cada ocasión:
        //      Vkd == kdPRECISE:
        //              ejecución:
        //                      SPMexe_p = SPMmec + SPMnum + SPMvar
        //              validación:
        //                      SPMval_p = SPMexe_p + SPMmin
        //              programación:
        //                      SPMpro_p = SPMval_p + SPMmin
        //              asignación:
        //                      SPMall_p = SPMpro_p + SPMoff
        //      Vkd == kdAPROXIMATE:
        //              ejecución:
        //                      SPMexe_a = SPMexe_p + SPMrec
        //              validación:
        //                      SPMval_a = SPMexe_a + SPMmin
        //              programación:
        //                      SPMpro_a = SPMval_a + SPMmin
        //              asignación:
        //                      SPMall_a = SPMpro_a + SPMoff

        //Ciclo de vida del SPM:
        //      1. Asignación.
        //      2. Programación.
        //      3. Validación.
        //      4. Ejecución.
        //En cada una de las etapas se consume el SPM destinado a la etapa,
        //motivo por el cual son sucesivamente más pequeños.
        //En ejecución el SPM representa la incertidumbre de localización
        //de las barreras físicas.

        //determina si el punto indicado está sobre el brazo
        bool Covers(TDoublePoint);

        //MÉTODOS PARA CALCULAR ÁNGULOS DE GIRO:

        //determina los ángulos que hay que rotar este brazo en torno al punto Q
        //para que quede adyacente al segmento (Pa, Pb)
//        void TurnSegment(TVector<double> &dts,
  //              TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Q);

        //determina los ángulos que hay que rotar este brazo en torno al punto Q
        //para que quede adyacente al arco (Pa, Pb, Pc, R)
    //    void TurnArc(TVector<double> &dts,
      //          TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R,
        //        TDoublePoint Q);

        //determina los ángulos que hay que rotar este brazo en torno al punto Q
        //para que quede adyacente al brazo Arm
//        void TurnArm(TVector<double> &dts, TArm *Arm_,
  //              TDoublePoint Q);

        //MÉTODOS PARA APILAR Y RECUPERAR ORIENTACIONES:

        //Las propiedades para el almacenamiento de la configuración inicial
        //deben encapsularse en el posicionador, con objeto de facilitar
        //la manipulación de listas de posicionadores.
        //Por legibilidad las propiedades para el almacenamiento de
        //la configurción inicial del brazo, debe encapsularse en
        //el propio brazo.

        //apila theta___3 en la pila LIFO theta___3s
        //      theta___3s.Add(theta___3)
        void Pushtheta___3(void);
        //restaura el último theta___3 de la pila LIFO theta___3s
        //      theta___3 = theta___3s.Last
        //si no hay una posición apilada lanza EImproperCall
        void Restoretheta___3(void);
        //desempila el último theta___3 de la pila LIFO theta___3s
        //      theta___3s.Delete(theta___3.Count - 1)
        //si no hay una posición apilada lanza EImproperCall
        void Poptheta___3(void);
        //restaura y desempila el último theta___3 de la pila LIFO theta___3s
        //      theta___3s.Delete(theta___3.Count - 1)
        //si no hay una posición apilada lanza EImproperCall
        void RestoreAndPoptheta___3(void);

        //ADVERTENCIA: la recuperación de posiciones no coincidentes con
        //el centroide de un escalón de cuantificación, cuando está la
        //cuantificación activada, dará lugar al desplazamiento al centroide
        //más próximo.

        //MÉTODOS PARA APILAR Y RECUPERAR ESTADOS DE CUANTIFICACIÓN:

        //apila Quantify___ en la pila LIFO Quantify___s
        //      Quantify___s.Add(Quantify___)
        void PushQuantify___(void);
        //restaura el último Quantify___ de la pila LIFO Quantify___s
        //      Quantify___ = Quantify___s.Last
        //si no hay una posición apilada lanza EImproperCall
        void RestoreQuantify___(void);
        //desempila el último Quantify___ de la pila LIFO Quantify___s
        //      Quantify___s.Delete(Quantify___.Count - 1)
        //si no hay una posición apilada lanza EImproperCall
        void PopQuantify___(void);
        //restaura y desempila el último Quantify___ de la pila LIFO Quantify___s
        //      Quantify___s.Delete(Quantify___.Count - 1)
        //si no hay una posición apilada lanza EImproperCall
        void RestoreAndPopQuantify___(void);

        //MÉTODOS DE MOVIMIENTO:

        //cambia la posición y orientación
        //del origen de coordenadas simultaneamente
        void Set(TDoublePoint _P1, double _thetaO3);

        //Cambia simultaneamente:
        //      la posición del brazo;
        //      la orientación del del origen de coordenadas de S3;
        //      la orientación del brazo respecto de su origen de coordenadas.
        void Set(TDoublePoint _P1, double _thetaO3, double _theta___3);

        //como la orientación del brazo es guardada en radianes
        //resulta apropiado pasarlo ya en radianes

        //genera un valor aleatorio con distribución uniforme en
        //[Max(0, Qmin), Min(floor(SB2/2), Qmax)]
        double Randomp___3(void);
        //asigna a p___3 un valor aleatorio con distribución uniforme en
        //[Max(0, Qmin), Min(floor(SB2/2), Qmax)]
        void Randomizep___3(void);

        //asigna a theta___3 el primer angulo
        //estable menor o igual que el indicado
        void Settheta___3FirstStableLessOrEqual(double _theta___3);
        //asigna a theta___2 el primer angulo
        //estable menor o igual que el indicado
        void Settheta___2FirstStableLessOrEqual(double _theta___2);

        //MÉTODOS DE GRÁFICOS:

        //traza el brazo con el color indicado
        //en el lienzo de la fotografía de un trazador
        //#void Paint(TPloterShapes *PloterShapes, QColor Color);

        //Pasar PloterShapes como un argumento tiene ventajas:
        //      los objetos polimórficos infocarán su método correspondiente.
        //      el trazador no necesita conocer la clase del objeto.

        //dibuja el eje del brazo de P1 a P2 con el color indicado
        //en el lienzo de la fotografía de un trazador
        //#void PaintSimplified(TPloterShapes *PloterShapes, QColor Color);

        //Colores recomendados para Color:
        //      clGray: no resaltado.
        //      clWhite: resaltado.

        //Dibujar solamente el eje del brazo puede ser interesante cuando haya
        //muchos brazos y se quieran observar en movimiento al mismo tiempo.
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // ARM_H
