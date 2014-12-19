//---------------------------------------------------------------------------
//Archivo: Cilinder.h
//Contenido: cilindro de un actuador
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef CILINDER_H
#define CILINDER_H

#include <stdint.h>

#include "Constants.h"
#include "Quantificator.h"
#include "Arm.h"
#include "Barrier.h"

//---------------------------------------------------------------------------

using namespace Operators;

//espacio de nombres de modelos
namespace Models {

//--------------------------------------------------------------------------
//TCilinder
//--------------------------------------------------------------------------

//clase posicionador de fibra
class TCilinder {
protected:
        //PROPIEDADES DE DIMENSIONAMIENTO
        //DE LECTURA/ESCRITURA:

        double __L01;

        //PROPIEDADES DE DIMENSIONAMIENTO
        //DE SOLO LECTURA:

        //PROPIEDADES DE ORIENTACIÓN EN RADIANES
        //DE LECTURA/ESCRITURA:

        double __thetaO1;

        double __theta_1min;
        double __theta_1max;

        double __theta_1;

        double __theta_O3o;

        //PROPIEDADES DE ORIENTACIÓN EN RADIANES
        //DE SOLO LECTURA:

        //PROPIEDADES DE CUANTIFICACIÓN
        //DE LECTURA/ESCRITURA:

        TFunction __F;

        bool __Quantify_;

        //PROPIEDADES DE CUANTIFICACIÓN
        //DE SOLO LECTURA:

        TFunction __G;

        double __SB1;
        double __rbs;

        TQuantificator __Q;

        //PROPIEDADES DE ORIENTACIÓN EN PASOS
        //DE SOLO LECTURA:

        //PROPIEDADES DE LOCALIZACIÓN
        //DE LECTURA/ESCRITURA:

        TDoublePoint __P0;

        //PROPIEDADES DE LOCALIZACIÓN
        //DE SOLO LECTURA:

        //PROPIEDADES COMPUESTAS:

        TArm *__Arm;
        TBarrier *__Barrier;

        //##################################################################
        //PROPIEDADES EN FORMATO TEXTO:
        //##################################################################

        //------------------------------------------------------------------
        //PROPIEDADES DE DIMENSIONAMIENTO
        //DE LECTURA/ESCRITURA EN FORMATO TEXTO:

        //PROPIEDADES DE DIMENSIONAMIENTO
        //DE SOLO LECTURA EN FORMATO TEXTO:

        //------------------------------------------------------------------
        //PROPIEDADES DE ORIENTACIÓN EN RADIANES
        //DE LECTURA/ESCRITURA EN FORMATO TEXTO:

        //PROPIEDADES DE ORIENTACIÓN EN RADIANES
        //DE SOLO LECTURA EN FORMATO TEXTO:

        //------------------------------------------------------------------
        //PROPIEDADES DE CUANTIFICACIÓN
        //DE LECTURA/ESCRITURA EN FORMATO TEXTO:

        //PROPIEDADES DE CUANTIFICACIÓN
        //DE SOLO LECTURA EN FORMATO TEXTO:

        //------------------------------------------------------------------
        //PROPIEDADES DE ORIENTACIÓN EN PASOS
        //DE SOLO LECTURA EN FORMATO TEXTO:

        //------------------------------------------------------------------
        //PROPIEDADES DE LOCALIZACIÓN
        //DE LECTURA/ESCRITURA EN FORMATO TEXTO:

        //PROPIEDADES DE LOCALIZACIÓN
        //DE SOLO LECTURA EN FORMATO TEXTO:

        //------------------------------------------------------------------
        //PROPIEDADES COMPUESTAS
        //EN FORMATO TEXTO:

        //------------------------------------------------------------------
        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //##################################################################
        //MÉTODOS PRIVADOS:
        //##################################################################

        //------------------------------------------------------------------
        //MÉTODOS DE ASIMILACIÓN:

        //A partir de:
        //      {F(theta_1)}.
        //Determina:
        //      {G(p_1), SB2, rbs},
        //      {theta_1min, theta_1max}.
        //      {Q(p_1)}
        //Restricciones:
        //      La función F(theta_1) debe ser monótona creciente.
        void ProcessateF(void);

        //Nótese que no se imponen las siguientes restricciones:
        //      La función F(theta_1) debe estar definida en 0.
        //      F(0) debería ser igual a cero.
        //      La función F(theta_1) debe estar definida en M_2PI.

        //calcula P1 según (P0, theta_1)
        TDoublePoint NewP1(void);

public:
        //------------------------------------------------------------------
        //PROPIEDADE DE DIMENSIONAMIENTO
        //DE LECTURA/ESCRITURA:

        //radio del cilindro
        //distancia de P0 a P1
        //debe ser mayor que cero
        //valor por defecto: LMEGARA mm
        double getL01(void) const {return __L01;} void setL01(double);

        //NOTA: se ha preferido llamar L01 a la distancia de P0 a P1
        //en vez de R0, porque el sistema se compone de antebrazo y brazo.
        //El hecho de que el brazo se implemente mediante un cilindro es
        //una peculiaridad que debe ser obviada.

        //Hay que advertir que es conveniente que las distancias
        //L01 y L12 sean independientes para poder simular situaciones reales.

        //PROPIEDADE DE DIMENSIONAMIENTO
        //DE SOLO LECTURA:

        //distancia de P0 a P2 cuando los puntos P0, P1 y P2 están alineados
        //      L02max = L01 + Arm->L12;
        //valor por defecto: LMEGARA + LMEGARA mm
        double getL02max(void) const {return getL01() + getArm()->getL12();}

        //distancia de P0 a P2 cuando los puntos P0, P1 y P3 están alineados
        //      L03max = L01 + Arm->L13;
        //valor por defecto: LMEGARA + LMEGARA mm
        double getL03max(void) const {return getL01() + getArm()->getL13();}

        //radio de la frontera del posicionador de fibra
        //radio de la circunferencia descrita por el ápice del brazo
        //cuando un punto del ápice está alineado con P0 y P1
        //depende del contorno del brazo
        //      r_max = L01 + Arm->Ra
        double getr_max(void) const {return getL01() + getArm()->getL1V();}

        //límite radial inferior del subespacio alcanzable por P3
        //cuando theta___3 puede valer 0
        //      r_3min = Abs(L01 - Arm->L13)
        double getr_3min(void) const {return Abs(getL01() - getArm()->getL13());}
        //límite radial superior del subespacio alcanzable por P3
        //cuando theta___3 puede valer M_PI
        //      r_3max = L01 + Arm->L13
        double getr_3max(void) const {return getL01() + getArm()->getL13();}

        //límite abcisa inferior del cuadrado que contiene el dominio de P3
        //      x3min = P0.x - L03max
        double getx3min(void) const {return getP0().x - getL03max();}
        //límite abcisa superior del cuadrado que contiene el dominio de P3
        //      x3max = P0.x + L03max
        double getx3max(void) const {return getP0().x + getL03max();}

        //límite ordenada inferior del cuadrado que contiene el dominio de P3
        //      y3min = P0.x - L03max
        double gety3min(void) const {return getP0().y - getL03max();}
        //límite ordenada superior del cuadrado que contiene el dominio de P3
        //      y3max = P0.x + L03max
        double gety3max(void) const {return getP0().y + getL03max();}

        //------------------------------------------------------------------
        //PROPIEDADES DE ORIENTACIÓN EN RADIANES
        //DE LECTURA/ESCRITURA:

        //orientación del origen del coordenadas de S1 respecto de S0
        //valor por defecto: 3./4.*M_2PI rad
        double getthetaO1(void) const {return __thetaO1;}
        void setthetaO1(double);

        //límite inferior del intervalo de giro de P1 en S1
        //debe estar en el dominio de definición de F(theta_1)
        //no debe ser mayor que theta_1max
        //valor por defecto: MEGARA_theta_1min rad
        double gettheta_1min(void) const {return __theta_1min;}
        void settheta_1min(double);
        //límite superior del intervalo de giro de P1 en S1
        //debe estar en el dominio de definición de F(theta_1)
        //no debe ser menor que theta_1min
        //valor por defecto: MEGARA_theta_1max rad
        double gettheta_1max(void) const {return __theta_1max;}
        void settheta_1max(double);

        //ángulo de orientación de P1 en S1
        //debe estar en el dominio de definición de F(theta_1)
        //Escritura:
        //      if(Quantify_)
        //              __theta_1 = Qtheta_1(theta_1);
        //      else
        //              __theta_1 = theta_1;
        //valor por defecto: 0 rad
        double gettheta_1(void) const {return __theta_1;}
        void settheta_1(double);

        //pila de ángulos theta_1 en radianes
        //valor por defecto: {}
        TVector<double> theta_1s;

        //ángulo de orientación de P1 en S0
        //      theta1 = thetaO1 - theta_1;
        double gettheta1(void) const {return getthetaO1() - gettheta_1();}
        void settheta1(double);

        //orientación del origen de coordenadas de S3 en S1
        //cuando theta_1 está en el origen
        //valor por defecto: M_PI
        double gettheta_O3o(void) const {return __theta_O3o;}
        void settheta_O3o(double);

        //theta_O3o es el nexo de unión entre los sistemas de coordenadas
        //del brazo (S2 y S3), y el sistema de coordenadas del cilindro (S1).

        //Nótese que podría haberse definido un sistema de coordenadas
        //intermedio entre S1 y S2, con la abcisa orientada de P1 a P0,
        //pero resulta excesivo para definir una sola coordenada (theta_O3o).

        //PROPIEDADES DE ORIENTACIÓN EN RADIANES
        //DE SOLO LECTURA:

        //orientación del origen de coordenadas de S3 respecto de S0:
        //      thetaO3 = thetaO1 - theta_1 - theta_O3
        double getthetaO3(void) const {return getthetaO1() - gettheta_1() - gettheta_O3o();}

        //------------------------------------------------------------------
        //PROPIEDADES DE CUANTIFICACIÓN
        //DE LECTURA/ESCRITURA:

        //función de transformación de de radianes a pasos de P1
        //debe ser monótona creciente
        //valor por defecto:
        //      {(M_2PI, MEGARASB1),
        //      (0, 0),
        //      (M_2PI, MEGARASB1),
        //      2*(M_2PI, MEGARASB1)}
        const TFunction& getF(void) const {return (const TFunction&)__F;}
        void setF(const TFunction &_F);

        //Nótese que no se imponen las siguientes restricciones:
        //      La función F(theta_1) debe estar definida en 0.
        //      F(0) debería ser igual a cero.
        //      La función F(theta_1) debe estar definida en M_2PI.

        //ADVERTENCIA: F.PointsText debe ser escrito mediante la propiedad
        //FPointsText de modo que se invoque al método SetFPointsText,
        //el cual invocará a SetF, el cual comprobará que F cumple
        //las restricciones y asimilará F mediante:
        //      CalculateG();
        //      CalculateQ();
        //      theta_1 = theta_1;

        //interruptor de cuantificación de p_1
        //indica si deben cuantificarse los valores asignados a p_1
        //valor por defecto: true
        bool getQuantify_(void) const {return __Quantify_;}
        void setQuantify_(bool);

        //Cuando Quantify_ es activado theta_1 es situado en la posición
        //estable más próxima mediante la reasignación así mismo:
        //      theta_1 = theta_1;

        //pila de estados de activación del cuantificador
        //valor por defecto: {}
        TItemsList<bool> Quantify_s;

        //PROPIEDADES DE CUANTIFICACIÓN
        //DE SOLO LECTURA:

        //función de transformación de pasos a radianes
        //es la inversa de F
        const TFunction& getG(void) const {return (const TFunction&)__G;}

        //número de pasos de una vuelta completa del eje 1 (SB: Steps Back)
        //es un número real ya que a la salida de la reductora
        //el número de pasos puede ser fraccionario
        //aunque convendría que fuera par ya que solo va a girar media vuelta
        //valor por defecto: F(M_2PI) = SIDESB1 steps
        double getSB1(void) const {return __SB1;}
        //radianes por paso medio en el intervalo [0, M_2PI]
        //      rbs = M_2PI/SB1;
        //valor por defecto: M_2PI/SB1 rad
        double getrbs(void) const {return __rbs;}

        //función de cuantificación lineal de p_1
        //valor por defecto: (1, [p_1min, p_1max])
        const TQuantificator& getQ(void) const {
                return (const TQuantificator&)__Q;}

        //NOTA: la constricción al dominio de p_1 se realiza en la función
        //de cuantificación Q, cuyo dominio imagen es [Q.Qmin, Q.Qmax].

        //ADVERTENCIA: cuando theta_1min está ligeramente por cebajo de
        //un centroide (o posición estable), p_1 puede ser calculado con
        //un error numérico que lo desplace por encima de dicho centroide
        //entonces se debe saber que Q.Qmin es calculado del siguiente modo:
        //      p_1min = F(theta_1min);
        //      Q.Qmin = ceil(p_1min/Q.q); (donde Q.q == 1);
        //de modo que puede haber una posición estable para p_1, que esté
        //por debajo de Qmin.
        //
        //Si embargo es poco probable que eso suponga un inconveniente, ya que:
        //      Qtheta_1(theta_1) utiliza la función Q, de modo que no
        //devolverá una posición estable que haga que p_1 se salga de
        //[Q.Qmin, Q.Qmax], y cuando asigna un valor a theta_1, este se irá
        //a la posición estable más próxim adevuelta por Qtheta_1(theta_1).

        //------------------------------------------------------------------
        //PROPIEDADES ORIENTACIÓN EN PASOS
        //DE SOLO LECTURA:

        //Valor aproximado de referencia del límite inferior
        //del intervalo de orientación de P1 en S1 en pasos.
        //El ángulo p_1min debe estar en el dominio imagen de F(theta_1)
        //Lectura:
        //      p_1min = F(theta_1min)
        //Escritura:
        //      __tehta_1min = G(p_1min)
        //Valor por defecto: F(MEGARAtheta_1min) steps
        double getp_1min(void) const; void setp_1min(double);
        //valor aproximado de referencia del
        //límite superior del intervalo de orientación de P1
        //respecto de S1 en pasos
        //Lectura:
        //      p_1max = F(theta_1max)
        //Escritura:
        //      __tehta_1max = G(p_1max)
        //Valor por defecto: F(MEGARAtheta_1max) steps
        double getp_1max(void) const; void setp_1max(double);

        //valor aproximado de referencia del
        //ángulo de orientación de P1 en S1
        //Lectura:
        //      if(Quantify_)
        //              p_1 = Q(F(theta_1));
        //      else
        //              p_1 = F(theta_1);
        //Escritura:
        //      if(Quantify_)
        //              __theta_1 = G(Q(p_1));
        //      else
        //              __theta_1 = G(p_1);
        //Valor por defecto: Q(F(0)) steps
        //En lectura el valor en pasos (p_1) se cuantifica para
        //evitar el error numérico introducido por la función F(theta_1).
        double getp_1(void) const; void setp_1(double);

        //cuando Quantify_ es verdadero:
        //      ninguna asignación a p_1 puede dar error
        //      es cuantificada tambien la lectura evitando el error numérico

       //ADVERTENCIA: el controlador puede estar concebido para
       //trabajar solamente con valores no negativos de p_1,
       //de modo que Min(p_1) = Maax(0, p_1min).

        //------------------------------------------------------------------
          //PROPIEDADES DE LOCALIZACIÓN
          //DE LECTURA/ESCRITURA:

        //posición del eje 1 o centro del posicionador de fibra respecto S0
        TDoublePoint getP0(void) const {return __P0;}
        void setP0(TDoublePoint);
        //Arm->P1 es la posición del eje 2 respecto S0
        //Arm->P2 es la posición del centro del cabezal del brazo respecto S0
        //Arm->P3 es la posición del centro de la lente del brazo respecto S0

        //coordenadas polar radial de P0 en el momento de
        //construirse el posicionador
        //debe ser no negativo
        double getL0(void) const; void setL0(double _L0);
        //coordenadas polar angular de P0 en el momento de
        //construirse el posicionador
        double gettheta0(void) const; void settheta0(double _theta0);

        //NOTA: (L0, heta0) es guardado en el posicionador
        //para ser utilizado en el bastidor.

        //PROPIEDADES DE LOCALIZACIÓN
        //DE SOLO LECTURA:

        //distancia máxima entre un punto del brazo del posicionador
        //y elorigen de S0
        //      rmax = L0 + r_max
        double getrmax(void) const {return getP0().Mod() + getr_max();}

        //coordenada polar radial de P2 relativas a S1 en mm
        //      r_2 = sqrt(L01^2 + Arm->L12^2 -
        //              2*L01*Arm->L12*cos(Arm->theta__2));
        double getr_2(void) const;
        //distancia angular de P1 a P2 en S1 en radianes
        //      theta_12 = acos((L01^2 + r_2^2 - Arm->L12^2)/(2*L01*r_2));
        double gettheta_12(void) const;
        //coordenada polar angular de P2 relativas a S1 en radianes
        //      theta_2 = theta_1 + theta_12;
        double gettheta_2(void) const;

        //NOTA: a^2 + b^2 - 2*a*b*cos(theta) nunca es negativo, en virtud de que
        //manteniendo constante a^2 + b^2, a*b es máximo cuando a=b,
        //en cuyo caso se verifica que:
        //      2ab = a^2 + b^2;
        //en los demás casos se tiene:
        //      2ab < a^2 + b^2.

        //coordenada cartesiana abcisa de P2 relativa a S1 en mm
        //      x_2 = r_2*cos(theta_2)
        double getx_2(void) const;
        //coordenada cartesiana ordenada de P2 relativa a S1 en mm
        //      y_2 = r_2*sin(theta_2)
        double gety_2(void) const;
        //coordenadas cartesianas de P2 relativas a S1
        TDoublePoint getP_2(void) const;

        //la transformación a (x2, y2) debe hacerse mediante el método:
        //      double S1toS0(double x_, double y_)

        //coordenada polar radial de P3 relativas a S1 en mm
        //      r_3 = sqrt(L01*L01 + Arm->L13*Arm->L13 -
        //              2*L01*Arm->L13*cos(Arm->theta__3));
        double getr_3(void) const;
        //distancia angular de P1 a P3 en S1 en radianes
        //      theta_13 = acos((L01^2 + r_3^2 - Arm->L13^2)/(2*L01*r_3));
        double gettheta_13(void) const;
        //coordenada polar angular de P2 relativas a S1 en radianes
        //      theta_3 = theta_1 + theta_13;
        double gettheta_3(void) const;

        //coordenada cartesiana abcisa de P3 relativa a S0 en mm
        //      x_3 = r_3*cos(theta_3)
        double getx_3(void) const;
        //coordenada cartesiana ordenada de P3 relativa a S0 en mm
        //      y_3 = r_3*sin(theta_3)
        double gety_3(void) const;
        //coordenadas cartesianas de P3 relativas a S1
        TDoublePoint getP_3(void) const;

        //La transformación a (x3, y3) debe hacerse mediante el método:
        //      TDoublePoint S1toS0(double x_, double y_)
        //ya que calcular cada coordenada por separado sería redundante.

        //punto origen o posición de P3 en S0 cuando los ejes están retraidos
        TDoublePoint getP3o(void) const;

        //------------------------------------------------------------------
        //PROPIEDADES COMPUESTAS:

        //brazo del posicionador de fibra
        TArm *getArm(void) const {return __Arm;}
        //barrera que delimita el área de exclusión
        TBarrier *getBarrier(void) const {return __Barrier;}

        //ADVERTENCIA: el brazo y la barrera encapsulan todas las propiedades
        //debidas a sus disposiciones sobre el cilindro.

        //##################################################################
        //PROPIEDADES EN FORMATO TEXTO:
        //##################################################################

        //------------------------------------------------------------------
        //PROPIEDADES DE DIMENSIONAMIENTO
        //DE LECTURA/ESCRITURA EN FORMATO TEXTO:

        AnsiString getL01Text(void) const; void setL01Text(AnsiString&);

        //PROPIEDADES DE DIMENSIONAMIENTO
        //DE SOLO LECTURA EN FORMATO TEXTO:

        AnsiString getL02maxText(void) const;
        AnsiString getL03maxText(void) const;
        AnsiString getr_maxText(void) const;

        AnsiString getr_3minText(void) const;
        AnsiString getr_3maxText(void) const;

        AnsiString getx3minText(void) const;
        AnsiString getx3maxText(void) const;

        AnsiString gety3minText(void) const;
        AnsiString gety3maxText(void) const;

        //------------------------------------------------------------------
        //PROPIEDADES DE ORIENTACIÓN EN RADIANES
        //DE LECTURA/ESCRITURA EN FORMATO TEXTO:

        AnsiString getthetaO1Text(void) const;
        void setthetaO1Text(AnsiString&);

        AnsiString gettheta_1minText(void) const;
        void settheta_1minText(AnsiString&);
        AnsiString gettheta_1maxText(void) const;
        void settheta_1maxText(AnsiString&);

        AnsiString gettheta_1Text(void) const;
        void settheta_1Text(AnsiString&);

        AnsiString gettheta1Text(void) const;
        void settheta1Text(AnsiString&);

        AnsiString gettheta_O3oText(void) const;
        void settheta_O3oText(const AnsiString&);

        //PROPIEDADES DE ORIENTACIÓN EN RADIANES
        //DE SOLO LECTURA EN FORMATO TEXTO:

        AnsiString getthetaO3Text(void) const;

        //------------------------------------------------------------------
        //PROPIEDADES DE CUANTIFICACIÓN
        //DE LECTURA/ESCRITURA EN FORMATO TEXTO:

        AnsiString getFAddressText(void) const;
        void setFPointsText(const AnsiString&);
        void setFTableText(const AnsiString&);

        AnsiString getQuantify_Text(void) const;
        void setQuantify_Text(AnsiString&);

        //PROPIEDADES DE CUANTIFICACIÓN
        //DE SOLO LECTURA EN FORMATO TEXTO:

        AnsiString getGAddressText(void) const;

        //La siguiente propiedad representa a todo Q:
        //      Q.AssignsText
        //y como tiene un número acotado de líneas
        //es preferible mostrarlas en vez de
        //la dirección en memoria de Q.

        AnsiString getSB1Text(void) const;
        AnsiString getrbsText(void) const;

        //------------------------------------------------------------------
        //PROPIEDADES DE ORIENTACIÓN EN PASOS
        //DE SOLO LECTURA EN FORMATO TEXTO:

        AnsiString getp_1minText(void) const; void setp_1minText(AnsiString&);
        AnsiString getp_1maxText(void) const; void setp_1maxText(AnsiString&);

        AnsiString getp_1Text(void) const; void setp_1Text(AnsiString&);

        //------------------------------------------------------------------
        //PROPIEDADES DE LOCALIZACIÓN
        //DE LECTURA/ESCRITURA EN FORMATO TEXTO:

        AnsiString getP0Text(void) const; void setP0Text(const AnsiString&);

        AnsiString getL0Text(void) const; void setL0Text(const AnsiString&);
        AnsiString gettheta0Text(void) const;
        void settheta0Text(const AnsiString&);

        //PROPIEDADES DE LOCALIZACIÓN
        //DE SOLO LECTURA EN FORMATO TEXTO:

        AnsiString getrmaxText(void) const;

        AnsiString getr_2Text(void) const;
        AnsiString gettheta_12Text(void) const;
        AnsiString gettheta_2Text(void) const;

        AnsiString getx_2Text(void) const;
        AnsiString gety_2Text(void) const;
        AnsiString getP_2Text(void) const;

        AnsiString getr_3Text(void) const;
        AnsiString gettheta_13Text(void) const;
        AnsiString gettheta_3Text(void) const;

        AnsiString getx_3Text(void) const;
        AnsiString gety_3Text(void) const;
        AnsiString getP_3Text(void) const;

        AnsiString getP3oText(void) const;

        //------------------------------------------------------------------
        //PROPIEDADES COMPUESTAS EN FORMATO TEXTO:

        //dirección en memoria del brazo
        AnsiString getArmAddressText(void) const {
                return IntToHex(reinterpret_cast<intptr_t>(getArm()), 8);}
        //dirección en memoria de la barrera
        AnsiString getBarrierAddressText(void) const {
                return IntToHex(reinterpret_cast<intptr_t>(getBarrier()), 8);}

        //------------------------------------------------------------------
        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjunto de propiedades de dimensionamiento en formato texto
        AnsiString getSizingText(void) const;
        //conjunto de propiedades de orientación en radianes en formato texto
        AnsiString getOrientationRadiansText(void) const;
        //conjunto de propiedades de cuantificación en formato texto
        AnsiString getQuantificationText(void) const;
        //conjunto de propiedades de orientación en pasos en formato texto
        AnsiString getOrientationStepsText(void) const;
        //conjunto de propiedades de localización en formato texto:
        AnsiString getLocationText(void) const;

        //conjunto de todas las propiedades del cilindro del posicionador
        AnsiString getCilinderText(void) const;

        //PROPIEDADES EXTENSAS EN FORMATO TEXTO:
        //   QuantificationText:
        //      F.PointsText
        //
        //      G.PointsText
        //      Q.Text
        //   Arm:
        //      Arm->AllText
        //
        //      Arm->Contour____.Text
        //      Arm->F.PointsText
        //
        //      Arm->G.PointsText;
        //      Arm->Q.AssignsText.
        //      Arm->Contour.Text;
        //   Wall:
        //      Wall->AllText;
        //
        //      Wall->Contour____.Text
        //
        //      Wall->Contour.Text;

        //##################################################################
        //MÉTODOS PÚBLICOS:
        //##################################################################

        //------------------------------------------------------------------
        //MÉTODOS DE CONTRUCCION, COPIA, CLONACIÓN Y DESTRUCCIÓN:

        //construye un actuador de fibra
        //con la posición y orientación indicadas
        TCilinder(TDoublePoint _P0, double _thetaO1=MEGARA_thetaO1);

        //copia un conjunto de propiedades de un actuador de fibra
        void CopySizing(const TCilinder*);
        void CopyOrientationRadians(const TCilinder*);
        void CopyQuantification(const TCilinder*);
        void CopyLocation(const TCilinder*);
        void CopyCilinder(const TCilinder*);

        //construye un clon de un actuador de fibra
        TCilinder(const TCilinder*);

        //libera la memoria dinámica del posicionador de fibra
        ~TCilinder();

        //MÉTODOS DE TRANSFORMACIÓN ENTRE SISTEMAS DE COORDENADAS:

        //transforma un ángulo en radianes
        //del sistema S0 al sistema S1
        //      theta_ = thetaO1 - theta
        double S0thetaToS1theta(double theta) const;
        //transforma un punto en coordenadas cartesianas
        //del sistema S0 al sistema S1
        //      P_.x = (x-P0.x)*cos(thetaO1) + (y-P0.y)*sin(thetaO1);
        //      P_.y = (x-P0.x)*sin(thetaO1) - (y-P0.y)*cos(thetaO1);
        TDoublePoint S0recToS1rec(double x, double y) const;
        TDoublePoint S0recToS1rec(TDoublePoint) const;
        //transforma un punto en coordenadas cartesianas
        //del sistema S1 al sistema S0
        //      P.x = (x_)*cos(thetaO1) + (y_)*sin(thetaO1) + P0.x;
        //      P.y = (x_)*sin(thetaO1) - (y_)*cos(thetaO1) + P0.y;
        TDoublePoint S1recToS0rec(double x_, double y_) const;

        //Se advierte que la trasnformación entre S0 y S1 es simétrica
        //a causa de la inversión de uno de los ejes.

        //La transformación de coordenadas cartesianas en S1
        //a coordenadas polares en S1, debe hacerse mediante las funciones:
        //      r_3 = Mod(x_3, y_3);
        //      theta_3 = ArgPos(x_3, y_3).

        //transforma las coordenadas cartesiaans en S2
        //en coordenadas cartesianas en S1
        TDoublePoint S2recToS1rec(double x__, double y__) const;

        //MÉTODOS PARA DETERMINAR LA PERTENENCIA A DOMIOS DE ORIENTACIÓN:

        //determina si un ángulo en radianes
        //está fuera del intervalo [theta_1min, theta_1max]
        bool IsntInDomaintheta_1(double _theta_1) const;
        //determina si un ángulo pasos
        //está fuera del intervalo [p_1min, p_1max]
        bool IsntInDomainp_1(double _p_1) const;

        //determina si un ángulo en radianes
        //está dentro del intervalo [theta_1min, theta_1max]
        bool IsInDomaintheta_1(double _theta_1) const;
        //determina si un ángulo pasos
        //está dentro del intervalo [p_1min, p_1max]
        bool IsInDomainp_1(double _p_1) const;

        //IsntInDomainp_1 traduce la coordenada a radianes antes de comprobar
        //si está en dominio, para evitar los errores numéricos de
        //p_1min y p_1max.
        //Los métodos IsInDomain... utilizan los métodos IsntInDomain...
        //negándolos.

        //MÉTODOS PARA DETERMINAR VALORES CUANTIFICADOS DE ORIENTACIONES:

        //devuelve el valor de cuantificación en radianes
        //más próximo al valor indicado
        //en el intervalo [theta_1min, theta_1max]
        double Qtheta_1(double theta_1);

        //determina las posiciones angulares estables que hacen que
        //la fibra de este posicionador se ubique lo más cerca posible
        //del punto correspondiente a unas posiciones angulares de los ejes
        //devuelve la distancia al punto hallado
        double GetNearestStablePosition(double &p_1nsp, double &p___3nsp,
            double theta_1, double theta___3);

        //Este método tiene los parámetros de entrada en coordenadas angulares
        //de los ejes en radianes, en congruencia con los valores devueltos por
        //el método:
        //      bool AnglesToGoP_3(double& theta_1, double& theta___3,
    //              double r_3, double theta_3);
        //Es preferible introducir coordenadas angulares de los ejes,
        //para reconstruir el punto por el mismo método con el cual
        //se van a generar los puntos estables, de modo que se evite
        //el error numérico.

        //Este método devuelve valores en pasos, porque los métodos de
        //generación de programas de movimiento van a trabajar en pasos.
        //Se ha elegido que dichos métodos trabajen en pasos por varias razones:
        //    - Existen tres posibilidades:
        //      1. Trabajar en radianes y no cuantificar.
        //      2. Trabajar en radianes y cuantificar.
        //      3. Trabajar en pasos y cuantificar.
        //      Trabajar en pasos constituye el método de cuantificación por
        //      antonomasia, de modo que no tiene sentido trabajar en radianes
        //      y cuantificar al mismo tiempo.
        //    - Si se trabaja en radianes hay que inroducir en el SPM una
        //      componente para absorber el error de cuantificación de los
        //      programas de movimiento.
        //    - La cuantificación de los programas de movimiento no es trivial,
        //      ya que las posiciones finales pueden diferir en más de un paso
        //      en la posición final de algún eje.
        //    - La componente de SPM a introducir dependería además del tamaño
        //      de los escalones de cuantificación más grandes (en posicones
        //      de inseguridad), de modo que si por defecto mecánico, hubiera
        //      algún escalon incrementado, la componente del SPM se vería
        //      también incrementada.
        //    - Se haría obligatorio introducir el conceptos de distancia de
        //      seguridad.
        //    - Habría que definir en la documentación el concepto de
        //      instrucción de movimiento no cuantificada (en radianes).
        //    - La única ventaja de trabajar en radianes es tener programas
        //      válidos para cualquier número de pasos. Sin embargo, si cambiase
        //      el número de pasos cambiaríán otros parámetros del modelo,
        //      de modo que la adaptación de un programa no tiene sentido,
        //      debiendo generarse un nuevo programa.
        //A la vista de estas razones se concluye que el motivo principal de
        //trabajar en pasos es la simplificación de los algoritmos;
        //en particular la cuantificación de los programas y el cálculo de
        //la nueva componente del SPM. Además trabajar en pasos permite
        //generar programas de movimiento optimizados, tanto en tiempo como en
        //espacio, pudiendo volverse crítico cualquiera de los dos factores
        //en un momento dado.

        //MÉTODOS DE CONFIGURACIÓN:

        //cambia conjuntamente las propiedades de plantilla
        //si no especifica argumentos se asignarán valores por defecto
        void SetTemplate(double _L01=MEGARA_L, double _L12=MEGARA_L, double _L13=MEGARA_L,
                double _theta___3=0, double _R3=0.75);

        //desactiva la cuantificación de los ejes del posicionador
        void DisableQuantification(void);
        //activa la cuantificación de los ejes del posicionador
        void EnableQuantification(void);

        //MÉTODOS PARA ALMACENAR Y RECUPERAR ORIENTACIONES:

        //apila theta_1 en la pila LIFO theta_1s
        //      theta_1s.Add(theta_1)
        void Pushtheta_1(void);
        //restaura el último theta_1 de la pila LIFO theta_1s
        //      theta_1 = theta_1s.Last
        //si no hay una posición apilada lanza EImproperCall
        void Restoretheta_1(void);
        //desempila el último theta_1 de la pila LIFO theta_1s
        //      theta_1s.Delete(theta_1.Count - 1)
        //si no hay una posición apilada lanza EImproperCall
        void Poptheta_1(void);
        //restaura y desempila el último theta_1 de la pila LIFO theta_1s
        //      theta_1s.Delete(theta_1.Count - 1)
        //si no hay una posición apilada lanza EImproperCall
        void RestoreAndPoptheta_1(void);

        //ADVERTENCIA: la recuperación de posiciones no estables,
        //cuando está la cuantificación activada, dará lugar
        //al desplazamiento a la posición estable más próxima.

        //MÉTODOS PARA ALMACENAR Y RECUPERAR ESTADOS DE CUNATIFICACIÓN:

        //apila Quantify_ en la pila LIFO Quantify_s
        //      Quantify_s.Add(Quantify_)
        void PushQuantify_(void);
        //restaura el último Quantify_ de la pila LIFO Quantify_s
        //      Quantify_ = Quantify_s.Last
        //si no hay una posición apilada lanza EImproperCall
        void RestoreQuantify_(void);
        //desempila el último Quantify_ de la pila LIFO Quantify_s
        //      Quantify_s.Delete(Quantify_.Count - 1)
        //si no hay una posición apilada lanza EImproperCall
        void PopQuantify_(void);
        //restaura y desempila el último Quantify_ de la pila LIFO Quantify_s
        //      Quantify_s.Delete(Quantify_.Count - 1)
        //si no hay una posición apilada lanza EImproperCall
        void RestoreAndPopQuantify_(void);

        //MÉTODOS DE MOVIMIENTO A ÁNGULOS:

        //cambia la posición y orientación
        //del origen de coordenadas simultaneamente
        void MoveOrigin(TDoublePoint _P0, double _thetaO1);

        //asigna conjuntamente theta_1 y theta___3
        void SetAnglesRadians(double _theta_1, double _theta___3);
        //asigna conjuntamente p_1 y p___3
        void SetAnglesSteps(double _p_1, double _p___3);

        //mueve los ejes hasta el origen de coordenadas
        void SetAnglesZeroSteps(void);

        //añade conjuntamente 'at_1' y 'at___3' a 'theta_1' y 'theta___3'
        void AddAnglesRadians(double at_1, double at___3);
        //añade conjuntamente 'ap_1' y 'ap___3' a 'p_1' y 'p___3'
        void AddAnglesSteps(double ap_1, double ap___3);

        //genera un valor aleatorio con distribución uniforme en:
        //      [Max(0, p_1min), Min(floor(SB1), p_1max)]
        double Randomp_1(void);
        //asigna a p_1 un valor aleatorio con distribución uniforme en:
        //      [Max(0, p_1min), Min(floor(SB1), p_1max)]
        void Randomizep_1(void);

        //------------------------------------------------------------------
        //MÉTODOS DE DETERMINACIÓN DE ÁNGULOS:

        //Dado el punto (r_3, theta_3) (en S1)
        //calcula (theta_1, theta___3) para que P3 vaya a él;
        //si el punto no está dentro del dominio devuelve falso.
        //Aunque el punto sea inalcanzable, este método devolverá
        //las posiciones límite a la que los ejes pueden ir.
        bool AnglesToGoP_3(double &_theta_1, double &_theta___3,
                double r_3, double theta_3);
        //Dado el punto (x3, y3) (en S0)
        //calcula (theta_1, theta___3) para que P3 vaya a él;
        //si el punto no está dentro del dominio devuelve falso.
        //Aunque el punto sea inalcanzable, este método devolverá
        //las posiciones límite a la que los ejes pueden ir.
        bool AnglesToGoP3(double &_theta_1, double &_theta___3,
                double x3, double y3);

        //Dado el ángulo theta_3 (en S1) calcula theta___3 para que P3 vaya a él;
        //si el ángulo no está dentro del dominio devuelve falso.
        bool theta___3ToGotheta_3(double &_theta___3, double _theta_3);
        //Dado el radio r_3 (en S1) calcula theta___3 para que P3 vaya a él;
        //si el radio no está dentro del dominio devuelve falso.
        bool theta___3ToGor_3(double &_theta___3, double _r_3);

        //MÉTODOS DE PERTENENCIA AL DOMINIO DE P3:

        //determina si el punto P_ está dentro del dominio de P_3
        bool PointIsInDomainP_3(TDoublePoint P_);
        //determina si el punto P_ está fuera del dominio de P_3
        bool PointIsOutDomainP_3(TDoublePoint P_);

        //determina si el punto P está dentro del dominio de P3
        bool PointIsInDomainP3(TDoublePoint P);
        //determina si el punto P está fuera del dominio de P3
        bool PointIsOutDomainP3(TDoublePoint P);
        //determina si el punto (x, y) está fuera del dominio de P3
        bool PointIsOutDomainP3(double x, double y);

        //determina el arco descrito por P3 al girar el eje 1
        //en el intervalo [Max(0., theta_1min), Min(2_MPI, theta_imax)]
        //con el brazo totalmente extendido
        //si el arco contiene un error numérico significativo hace Pfin = Pini
        void GetArc(TDoublePoint &Pini, TDoublePoint &Pfin, TDoublePoint &Pa,
                double &R);
        //determina el arco descrito por P3 al plegar el brazo
        //con el eje 1 en el ángulo theta_
        void GetArc(TDoublePoint &Pini, TDoublePoint &Pfin, TDoublePoint &Pa,
                double &R, double theta_);

        //Determina si un segmento interseca al arco descrito por P3
        //al girar el eje 1 con el brazo totalemnte extendido.
        bool IntersectionSegmentArc1(TDoublePoint Pa, TDoublePoint Pb);
        //Determina si un segmento interseca al arco descrito por P3
        //al plegar el brazo cuando theta_1 = Max(0, theta_1min).
        bool IntersectionSegmentArc2(TDoublePoint Pa, TDoublePoint Pb);
        //Determina si un segmento interseca al arco descrito por P3
        //al plegar el brazo cuando theta_1 = Min(M_2PI, theta_1max).
        bool IntersectionSegmentArc3(TDoublePoint Pa, TDoublePoint Pb);

        //Determina si un segmento invade el dominio de P3
        //teniendo en cuenta los arcos descritos por P3 al girar el eje 1.
        bool SegmentInvadeDomainP3(TDoublePoint Pa, TDoublePoint Pb);
        //Determina si un segmento no puede ser recorrido
        //totalmente mediante un movimeinto continuo del punto P3,
        //teniendo en cuenta los arcos descritos por P3 al girar el eje 1.
        bool SegmentCantBeFollowedByP3(TDoublePoint Pa, TDoublePoint Pb);

        //Un polisegmento puede estar en una de las cuatro situaciones
        //sigueintes respecto del dominio de P3:
        // 1. Totalmente en el interior sin intersecar los límites del dominio.
        // 2. Totalmente en el interior intersecando los límites del dominio.
        // 3. Parcialmente en el interior y el resto fuera.
        // 4. Totalmente en el exterior.

        //Determina si un polisegmento invade el dominio de P3,
        //teniendo en cuenta los arcos descritos por P3 al girar el eje 1.
        bool PolysegmentInvadeDomainP3(TItemsList<TDoublePoint> &Polysegment);
        //Determina si un polisegmento no puede ser recorrido
        //totalmente mediante un movimeinto continuo del punto P3,
        //teniendo en cuenta los arcos descritos por P3 al girar el eje 1.
        bool PolysegmentCantBeFollowedByP3(TItemsList<TDoublePoint> &Polysegment);

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
        //MODO DE USO DE LOS MÉTOS DE DETERMINACIÓN DE POSICIONES:
        //
        //para obtener las posiciones correspondientes a un punto expresado
        //en coordenadas cartesianas de S0 hay que seguir los pasos:
        //      P_ = S0toS1(x, y); //traduce a cartesianas relativas a S1
        //      r_3 = Mod(P_.x, P_.y); //obtiene la coordenada radial
        //      theta_3 = ArgPos(P_.x, P_.y); //obtiene la coordenada angular

        //ahora ya puede determinar las posiciones de los ejes
        //y si el punto está en el dominio:
        //      isindomine = PositionToGoP_3(theta_1, theta___3, r_3, theta_3);

        //para obtener las posiciones en pasos deben traducirse
        //las posiciones de radianes a pasos mediante:
        //      p_1 = F(theta_1); //posición del eje 1 en pasos
        //      p__3 = Arm->F(theta___3); //posición del eje 2 en pasos
        //
        //Para determinar si un punto está dentro del dominio
        //de un punto del posicionador, implica calcular
        //las posiciones de los ejes para ver si estas están
        //dentro de sus respectivos dominios. Por eso
        //tal determinación deberá realizarse invocando a los métodos
        //de determinación de posiciones
        //-------------------------------------------------------------------

        //MÉTODOS DE MOVIMIENTO A PUNTOS:

        //asigna coordenadas polares a P_3
        //si el punto no está en el dominio de P_3 lanza una excepción
        void SetPolarP_3(double _r_3, double _theta_3);

        //asigna coordenadas cartesianas a P_3
        //si el punto no está en el dominio de P_3 lanza una excepción
        void SetCartesianP_3(double _x_3, double _y_3);
        void SetCartesianP_3(TDoublePoint _P_3);

        //genera un punto con distribución uniforme en
        //el dominio cartesiano de P3 hasta que dicho punto
        //esté dentro del dominio de P3
        TDoublePoint RandomP3(void);

        //genera un punto con distribución uniforme en
        //el dominio cartesiano de P3 hasta que dicho punto
        //esté dentro del dominio de P3 y lo asigna a P3
        void RandomizeP3(void);

};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // CILINDER_H
