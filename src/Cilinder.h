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
/// @file Cilinder.h
/// @brief cilinder model of an actuator of a RP
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef CILINDER_H
#define CILINDER_H

#include <stdint.h>
#include <math.h>

#include "Arm.h"
#include "Constants.h"
#include "Quantificator.h"
#include "Barrier.h"

//---------------------------------------------------------------------------

using namespace Operators;

//namespace for models
namespace Models {

//--------------------------------------------------------------------------
//TCilinder
//--------------------------------------------------------------------------

//class cilinder of an actuatorof a RP
class TCilinder {
protected:
    //SIZING PROPERTIES R/W:

    double p_L01;

    //ORIENTATION PROPERTIES IN RAD R/W:

    double p_thetaO1;

    double p_theta_1min;
    double p_theta_1max;

    double p_theta_1;

    double p_theta_O3o;

    //QUANTIFICATION PROPERTIES R/W:

    TFunction p_F;

    bool p_Quantify_;

    //QUANTIFICATION PROPERTIES R:

    TFunction p_G;

    double p_SB1;
    double p_rbs;

    TQuantificator p_Q;

    //LOCATION PROPERTIES R/W:

    TDoublePoint p_P0;

    //STRUCTURED PROPERTIES:

    TArm *p_Arm;
    TBarrier *p_Barrier;

    //------------------------------------------------------------------
    //ASSIMILATION METHODS:

    //From:
    //  {F(theta_1)}.
    //Determine:
    //  {G(p_1), SB2, rbs},
    //  {theta_1min, theta_1max}.
    //  {Q(p_1)}
    //Restrictions:
    //  the function F(theta_1) must be monotonous growing.
    void processateF(void);

    //Note that the following restriction are not impossed:
    //  The funtion F(theta_1) must be defined 0.
    //  F(0) must be equal to zero.
    //  The function F(theta_1) must bedefined in M_2PI.

    //calculate P1 according to (P0, theta_1)
    TDoublePoint newP1(void);

public:
    //------------------------------------------------------------------
    //SIZING RPOPERTIES R/w:

    //radio of the cilinder
    //distance from P0 to P1
    //must be upper zero
    //default value: MEGARA_L mm
    double getL01(void) const {return p_L01;} void setL01(double);

    //NOTA: se ha preferido llamar L01 a la distancia de P0 a P1
    //en vez de R0, porque el sistema se compone de antebrazo y brazo.
    //El hecho de que el brazo se implemente mediante un cilindro es
    //una peculiaridad que debe ser obviada.

    //Hay que advertir que es conveniente que las distancias
    //L01 y L12 sean independientes para poder simular situaciones reales.

    //SIZING RPOPERTIES R:

    //maximun distance from P0 to P2
    //  L02max = L01 + Arm->L12;
    //default value: LMEGARA + LMEGARA mm
    double getL02max(void) const {return getL01() + getArm()->getL12();}

    //maximun distance from P0 to P2
    //  L03max = L01 + Arm->L13;
    //default value: LMEGARA + LMEGARA mm
    double getL03max(void) const {return getL01() + getArm()->getL13();}

    //radio of the frontier of the RP
    //radio of the circunference described by the apex of the arm
    //when a point of the apex is aligned with P0 and P1
    //it is function of contour of the arm
    //  r_max = L01 + Arm->Ra
    double getr_max(void) const {return getL01() + getArm()->getL1V();}

    //minimun distance from P0 to P3
    //  r_3min = Abs(L01 - Arm->L13)
    double getr_3min(void) const {return Abs(getL01() - getArm()->getL13());}
    //maximun distance from P0 to P3
    //  r_3max = L01 + Arm->L13
    double getr_3max(void) const {return getL01() + getArm()->getL13();}

    //minimun value of P3.x
    //  x3min = P0.x - L03max
    double getx3min(void) const {return getP0().x - getL03max();}
    //maximun value of P3.x
    //  x3max = P0.x + L03max
    double getx3max(void) const {return getP0().x + getL03max();}

    //minimun value of P3.y
    //  y3min = P0.x - L03max
    double gety3min(void) const {return getP0().y - getL03max();}
    //maximun value of P3.y
    //  y3max = P0.x + L03max
    double gety3max(void) const {return getP0().y + getL03max();}

    //------------------------------------------------------------------
    //ORIENTATION PROPERTIES IN RAD R/W:

    //orientation of the coordinate system S1 respect to S0
    //default value: 3./4.*M_2PI rad
    double getthetaO1(void) const {return p_thetaO1;}
    void setthetaO1(double);

    //lower limit for theta_1
    //must be in the domain of F
    //shall be lower or equal to theta_1max
    //default value: MEGARA_theta_1min rad
    double gettheta_1min(void) const {return p_theta_1min;}
    void settheta_1min(double);
    //upper limit for theta_1
    //must be in the domain of F
    //shall be upper or equal to theta_1min
    //default value: MEGARA_theta_1max rad
    double gettheta_1max(void) const {return p_theta_1max;}
    void settheta_1max(double);

    //angular coordinate of P1 in S1 in rad
    //must be in the domain of F
    //getting:
    //  return p_theta_1;
    //setting:
    //  if(Quantify_)
    //      p_theta_1 = Qtheta_1(theta_1);
    //  else
    //      p_theta_1 = theta_1;
    //default value: 0 rad
    double gettheta_1(void) const {return p_theta_1;}
    void settheta_1(double);

    //stack for values of theta_1
    //default value: {}
    TVector<double> theta_1s;

    //angular position of P1 in S0
    //  theta1 = thetaO1 - theta_1;
    double gettheta1(void) const {return getthetaO1() - gettheta_1();}
    void settheta1(double);

    //orientation of the coordinate system S3 respect S1
    //when theta_1 == 0
    //default value: M_PI
    double gettheta_O3o(void) const {return p_theta_O3o;}
    void settheta_O3o(double);

    //theta_O3o es el nexo de unión entre los sistemas de coordenadas
    //del brazo (S2 y S3), y el sistema de coordenadas del cilindro (S1).

    //Nótese que podría haberse definido un sistema de coordenadas
    //intermedio entre S1 y S2, con la abcisa orientada de P1 a P0,
    //pero resulta excesivo para definir una sola coordenada (theta_O3o).

    //ORIENTATION PROPERTIES IN RAD R:

    //first position angle of rotor 1 in rad
    //  theta_1first = max(0, theta_1min)
    double gettheta_1first(void) const;
    //last position angle of rotor 1 in rad
    //  theta_1last = min(M_2PI, theta_1max)
    double gettheta_1last(void) const;

    //orientation of the coordinate system S3 respect S0:
    //  thetaO3 = thetaO1 - theta_1 - theta_O3
    double getthetaO3(void) const {return getthetaO1() - gettheta_1() +
                gettheta_O3o();}

    //------------------------------------------------------------------
    //QUANTIFICATION PROPERTIES R/W:

    //transform function for the rotor 1 position angle
    //from radians to steps
    //  p_1 = getF().Image(theta_1);
    //must be monotonous growing
    //default value:
    //  {(M_2PI, MEGARA_SB1), (0, 0), (M_2PI, MEGARA_SB1),
    //  2*(M_2PI, MEGARA_SB1)}
    const TFunction& getF(void) const {return (const TFunction&)p_F;}
    void setF(const TFunction &_F);

    //Note that the following restriction are not impossed:
    //  The funtion F(theta_1) must be defined 0.
    //  F(0) must be equal to zero.
    //  The function F(theta_1) must bedefined in M_2PI.

    //ADVERTENCIA: F.PointsText debe ser escrito mediante la propiedad
    //FPointsText de modo que se invoque al método SetFPointsText,
    //el cual invocará a SetF, el cual comprobará que F cumple
    //las restricciones y asimilará F mediante:
    //      CalculateG();
    //      CalculateQ();
    //      theta_1 = theta_1;

    //interrupt for enabling and disabling the quantificator of the rotor 1
    //default value: true
    bool getQuantify_(void) const {return p_Quantify_;}
    void setQuantify_(bool);

    //When Quantify_ change to enabled theta_1 is assigned to own:
    //      theta_1 = theta_1;

    //Functioning according to the status of Quantify_:
    //
    //angular coordinate of P1 in S1 in rad
    //must be in the domain of F(theta_1)
    //getting:
    //  return p_theta_1;
    //setting:
    //  if(Quantify_)
    //      p_theta_1 = Qtheta_1(theta_1);
    //  else
    //      p_theta_1 = theta_1;
    //default value: 0 rad
    //
    //angular position of P1 in S1 in steps
    //getting:
    //  if(Quantify_)
    //      p_1 = Q(F(theta_1)); //quantified to avoid numerical error
    //  else
    //      p_1 = F(theta_1);
    //  return p_1;
    //setting:
    //  if(Quantify_)
    //      p_theta_1 = G(Q(p_1)); //this is ever successfully executed
    //  else
    //      p_theta_1 = G(p_1);
    //default value: Q(F(0)) steps

    //stack for values of Quantify_
    //default value: {}
    TItemsList<bool> Quantify_s;

    //QUANTIFICATION PROPERTIES R:

    //transform function for the rotor 1 position angle
    //from steps toradians
    //  theta_1 = getG().Image(p_1);
    //is the inferse of F
    //default value:
    //  G.Inverse(getF());;
    const TFunction& getG(void) const {return (const TFunction&)p_G;}

    //steps back for rotor 1
    //default value: F(M_2PI) steps
    //note that F(M_2PI) == MEGARA_SB1
    double getSB1(void) const {return p_SB1;}
    //radian by stepss avergage in [0, M_2PI]
    //      rbs = M_2PI/SB1;
    //default value: M_2PI/SB1 rad
    double getrbs(void) const {return p_rbs;}

    //quantification function of p_1
    //default value: (1, [p_1min, p_1max])
    const TQuantificator& getQ(void) const {
        return (const TQuantificator&)p_Q;}

    //NOTA: la constricción al dominio de p_1 se realiza en la función
    //de cuantificación Q, cuyo dominio imagen es [Q.Qmin, Q.Qmax].

    //ADVERTENCIA: cuando theta_1min está ligeramente por cebajo de
    //un centroide (o posición estable), p_1 puede ser calculado con
    //un error numérico que lo desplace por encima de dicho centroide
    //entonces se debe saber que Q.Qmin es calculado del siguiente modo:
    //  p_1min = F(theta_1min);
    //  Q.Qmin = (int)ceil(p_1min/Q.q); (donde Q.q == 1);
    //de modo que puede haber una posición estable para p_1, que esté
    //por debajo de Qmin.
    //
    //Si embargo es poco probable que eso suponga un inconveniente, ya que:
    //  Qtheta_1(theta_1) utiliza la función Q, de modo que no
    //devolverá una posición estable que haga que p_1 se salga de
    //[Q.Qmin, Q.Qmax], y cuando asigna un valor a theta_1, este se irá
    //a la posición estable más próxim adevuelta por Qtheta_1(theta_1).

    //------------------------------------------------------------------
    //ORIENTATION PROPERTIES IN STEPS R/W:

    //lower limit for p_1
    //nust be in the domain of F
    //getting:
    //  p_1min = F(theta_1min)
    //setting:
    //  p_theta_1min = G(p_1min)
    //default value: F(MEGARA_theta_1min) steps
    double getp_1min(void) const; void setp_1min(double);
    //upper limit for p_1
    //nust be in the domain of F
    //getting:
    //  p_1max = F(theta_1max)
    //setting:
    //  p_theta_1max = G(p_1max)
    double getp_1max(void) const; void setp_1max(double);

    //angular position of P1 in S1 in steps
    //getting:
    //  if(Quantify_)
    //      p_1 = Q(F(theta_1)); //quantified to avoid numerical error
    //  else
    //      p_1 = F(theta_1);
    //  return p_1;
    //setting:
    //  if(Quantify_)
    //      p_theta_1 = G(Q(p_1)); //this is ever successfully executed
    //  else
    //      p_theta_1 = G(p_1);
    //default value: Q(F(0)) steps
    double getp_1(void) const; void setp_1(double);

    //ORIENTATION PROPERTIES IN STEPS R/W:

    //first stable position angle of rotor 1 in steps:
    //  p_1first = ceil(F(theta_1first));
    double getp_1first(void) const {
        return ceil(getF().Image(gettheta_1first()));}
    //las stable position angle of rotor 1 in steps:
    //  p_1last = floor(F(theta_1last));
    double getp_1last(void) const {
        return floor(getF().Image(gettheta_1last()));}

    //Note that F(0) shall be 0 for avoid negative positions in steps,
    //and thecontroller of the rotor could notwork with negativevalues.

    //------------------------------------------------------------------
    //LOCATION PROPERTIES R/W:

    //position of rotor 1 or center of the RP respect S0
    TDoublePoint getP0(void) const {return p_P0;}
    void setP0(TDoublePoint);
    //Arm->P1 is the position of the rotor 2 respect S0
    //Arm->P2 is the position of the center of the armp's head respect S0
    //Arm->P3 is the position of the fiber respect S0

    //LOCATION PROPERTIES R:

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

    //punto origen o posición de P3 en S1
    //cuando los rotores están en el origen
    TDoublePoint getP_3o(void) const;
    //punto origen o posición de P3 en S0
    //cuando los rotores están en el origen
    TDoublePoint getP3o(void) const;

    //------------------------------------------------------------------
    //STRUCTURED PROPERTIES:

    //brazo del posicionador de fibra
    TArm *getArm(void) const {return p_Arm;}
    //barrera que delimita el área de exclusión
    TBarrier *getBarrier(void) const {return p_Barrier;}

    //ADVERTENCIA: el brazo y la barrera encapsulan todas las propiedades
    //debidas a sus disposiciones sobre el cilindro.

    //------------------------------------------------------------------
    //SIZING PROPERTIES R/W
    //IN TEXT FORMAT:

    AnsiString getL01Text(void) const; void setL01Text(AnsiString&);

    //SIZING PROPERTIES R
    //IN TEXT FORMAT:

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
    //ORIENTATION PROPERTIES IN RAD R/W
    //IN TEXT FORMAT:

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

    //ORIENTATION PROPERTIES IN RAD R
    //IN TEXT FORMAT:

    AnsiString getthetaO3Text(void) const;

    //------------------------------------------------------------------
    //QUANTIFICATION PROPERTIES R/W
    //IN TEXT FORMAT:

    AnsiString getFAddressText(void) const;
    void setFPointsText(const AnsiString&);
    void setFTableText(const AnsiString&);

    AnsiString getQuantify_Text(void) const;
    void setQuantify_Text(AnsiString&);

    //QUANTIFICATION PROPERTIES R
    //IN TEXT FORMAT:

    AnsiString getGAddressText(void) const;

    //La siguiente propiedad representa a todo Q:
    //  Q.AssignsText
    //y como tiene un número acotado de líneas
    //es preferible mostrarlas en vez de
    //la dirección en memoria de Q.

    AnsiString getSB1Text(void) const;
    AnsiString getrbsText(void) const;

    //------------------------------------------------------------------
    //ORIENTATION PROPERTIES IN STEPS R
    //IN TEXT FORMAT:

    AnsiString getp_1minText(void) const; void setp_1minText(AnsiString&);
    AnsiString getp_1maxText(void) const; void setp_1maxText(AnsiString&);

    AnsiString getp_1Text(void) const; void setp_1Text(AnsiString&);

    //------------------------------------------------------------------
    //LOCATION PROPERTIES R/W
    //IN TEXT FORMAT:

    AnsiString getP0Text(void) const; void setP0Text(const AnsiString&);

    //LOCATION PROPERTIES R
    //IN TEXT FORMAT:

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
    //COMPOSED PROPERTIES
    //IN TEXT FORMAT:

    //dirección en memoria del brazo
    AnsiString getArmAddressText(void) const {
        return IntToHex(intptr_t(getArm()));}
    //dirección en memoria de la barrera
    AnsiString getBarrierAddressText(void) const {
        return IntToHex(intptr_t(getBarrier()));}

    //------------------------------------------------------------------
    //SET OF PROPERTIES IN TEXT FORMAT:

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

    //Other sets of properties in text format:
    //   QuantificationText:
    //      F.PointsText
    //      G.PointsText
    //      Q.Text
    //   Arm:
    //      Arm->AllText
    //      Arm->Contour____.Text
    //      Arm->F.PointsText
    //      Arm->G.PointsText;
    //      Arm->Q.AssignsText.
    //      Arm->Contour.Text;
    //   Barrier:
    //      Barrier->AllText;
    //      Barrier->Contour_.Text
    //      Barrier->Contour.Text;

    //------------------------------------------------------------------
    //BUILDING, COPYING, CLONATION AND DESTROYING METHODS:

    //construye un actuador de fibra
    //con la posición y orientación indicadas
    TCilinder(TDoublePoint P0, double thetaO1=MEGARA_thetaO1);

    //copia un conjunto de propiedades de un cilindro
    void copySizing(const TCilinder*);
    void copyOrientationRadians(const TCilinder*);
    void copyQuantification(const TCilinder*);
    void copyLocation(const TCilinder*);
    //clona todas las propiedades de un cilindro
    void cloneCilinder(const TCilinder*);

    //construye un clon de un actuador de fibra
    TCilinder(const TCilinder*);

    //libera la memoria dinámica del posicionador de fibra
    ~TCilinder();

    //------------------------------------------------------------------
    //METHODS FOR TRANSFORM COORDINATES:

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

    //dada la posición actual de theta_1,
    //calcula theta___3 para que P3 vaya a theta_3
    //si theta_3 no está al alcance devuelve falso
    bool theta___3ToGotheta_3(double &_theta___3, double theta_3);
    //calcula theta___3 a partir de r_3
    //si r_3 no está al alcance devuelve falso
    bool theta___3ToGor_3(double &_theta___3, double r_3);
    //dada la posición actual de theta___3,
    //calcula theta_1 para que P3 vaya a theta_3
    //si theta_3 no está al alcance devuelve falso
    bool theta_1ToGotheta_3(double& theta_1, double theta_3);

    //busca la posición estable que hace que el punto P3 quede lo más próximo a P
    //en el intervalo [p_1min, p_1max]x[p___3min, p___3max]
    //devuelve la distancia de P a P3
    double searchNearestStablePosition(double& p_1nsp, double& p___3nsp,
                                       TDoublePoint P,
                                       double p_1min, double p_1max,
                                       double p___3min, double p___3max);

    //determina las posiciones angulares estables que hacen que  el punto P3
    //se ubique lo más cerca posible del punto correspondiente a unas posiciones
    //angulares de los rotores devuelve la distancia al punto hallado
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
    //siguientes respecto del dominio de P3:
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

};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // CILINDER_H
