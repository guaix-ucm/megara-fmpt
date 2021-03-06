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
/// @file Arm.cpp
/// @brief arm model of an actuator of a RP
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Arm.h"
#include "Constants.h"
#include "Strings.h"
#include "TextFile.h"

#include <math.h>
#include <algorithm> //std::min, std::max

//---------------------------------------------------------------------------

using namespace Strings;

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//TArm
//---------------------------------------------------------------------------

//ADVERTENCIA: el contructor se ejecutará antes
//de que se inicialicen las variables estáticas.

//---------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//Apartir de:
//  {L12, L13, theta__3}
//  {Contour____}
//Determina:
//  {P____2, P____1}
//  {P__2, P__3}
//  {Contour__}
//  {V__, L1V}
void TArm::processateTemplate(void)
{
    //determina la posición de P2 en S4
    P____2.x = 5.807111;//MEGARA_L;
    P____2.y = 0;

    //Nótese que MEGARA_L == 5.8025, mientras que MEGARA_Contour____Text
    //define un contorno desplazado 5.807111 en el eje x.

    //determina el desplazamiento de S4 para que el contorno del brazo se adapte a L13
    P____1.x = P____2.x - L13*cos(theta__3);
    P____1.y = 0;

    //Esto hará que la plantilla del brazo se desplace en S2
    //de modo que P2 coincida en x__ = 5.80711.

    //calcula las coordenadas de los puntos significativos
    P__2.x = L12;
    P__2.y = 0;
    P__3.x = L13*cos(theta__3);
    P__3.y = L13*sin(theta__3);

    //contruye el contorno del brazo a partir de la plantilla
    Destroy(Contour__);
    Contour__.Copy(Contour____);
    Destroy(Contour);
    Contour.Copy(Contour____);

    //El sistema S2 tiene origen en P1 y su eje x pasa por P2.
    //De modo que S2 está trasladado a P____1 y rotado
    //el mismo ángulo que el vector (P____2 - P____1).

    //determina el contorno del brazo en S2:
    double theta = Arg(P____2 - P____1);
    Contour____.getRotatedAndTranslated(Contour__, -theta, -P____1);

    //ADVERTENCIA: aquí no debe usarse el operador =
    //por que es una lista de punteros y se asignarian
    //los punteros a los mismos objetos.
    //El método Copy ha sido especialmente definido para
    //que se contruyan clones de los objetos.

    //determina la distancia máxima del contorno al origen de S4
    if(Contour____.getCount() > 0)
        L1V = Contour____.distanceMax(P____1);
    else
        L1V = 0;

    V__.x = L1V;
    V__.y = 0;
}

//A partir de:
//  {F(theta___3)}
//Determina:
//  {G(p___3), SB2, rbs}
//  {theta___3min, theta___3max}
//  {Q(p___3)}
//Restricciones:
//  La función F(theta___3) debe ser monótona creciente.
void TArm::processateF(void)
{
    try {
        //determina la función G (inversa de F)
        G.Inverse(F);
    } catch(Exception& E) {
        E.Message.Insert(1, "getting inverse of F: ");
        throw;
    }

    //ADVERTENCIA: es posible que F no esté definida en 0 y/o en M_2PI.

    //selecciona el intervalo a partir del cual va a calcular
    //el número de pasos de una vuelta completa
    double xmax = min(M_2PI, F.getXLast());
    double xmin = max(0., F.getXFirst());
    //calcula el número de pasos de una vuelta completa
    SB2 = (F.Image(xmax) - F.Image(xmin))/(xmax - xmin)*M_2PI;

    //calcula el escalón de cuantificación promedio en [0, M_2PI]
    rbs = M_2PI/SB2;

    //constriñe los límites del dominio de theta___3 según el dominio de F
    if(theta___3min < F.getXFirst())
        theta___3min = F.getXFirst();
    if(theta___3max > F.getXLast())
        theta___3max = F.getXLast();

    //asimila [theta___3min, theta___3max]
    Q.Set(1, getp___3min(), getp___3max());

    //constriñe theta___3 a su dominio
    //y/o lo cuantifica en su caso
    if(theta___3 < theta___3min)
        settheta___3(theta___3min);
    else if(theta___3 > theta___3max)
        settheta___3(theta___3max);
    else
        settheta___3(theta___3);
}

//a partir de:
//  {P__2, P__3}
//  {Contour__, V__}
//  {P1, theta2}
//Determina:
//  {P2, P3}
//  {Contour, V}
void TArm::calculateImage(void)
{
    //calcula los coeficientes de rotación de theta2
    double theta = gettheta2();
    const double COS = cos(theta);
    const double SIN = sin(theta);

    //rota y traslada los puntos P2 y P3
    P2.x = P__2.x*COS - P__2.y*SIN + P1.x;
    P2.y = P__2.x*SIN + P__2.y*COS + P1.y;
    P3.x = P__3.x*COS - P__3.y*SIN + P1.x;
    P3.y = P__3.x*SIN + P__3.y*COS + P1.y;

    //determina el contorno del brazo (rotado y trasladado):
    Contour__.getRotatedAndTranslated(Contour, theta, P1);

    //rota y traslada el punto V
    V.x = V__.x*COS - V__.y*SIN + P1.x;
    V.y = V__.x*SIN + V__.y*COS + P1.y;
}

//---------------------------------------------------------------------------
//PROPIEDADES DE PLANTILLA
//DE LECTURA/ESCRITURA:

//distancia desde rotor del brazo hasta
//el punto de referencia del cabezal de la plantilla
//distancia de P1 a P2
//debe ser mayor que cero
//valor por defecto: MEGARA_L mm
void TArm::setL12(double t_L12)
{
    //comprueba las precondiciones
    if(t_L12 <= 0)
        throw EImproperArgument("length L12 should be upper zero");

    L12 = t_L12; //asigna el nuevo valor

    //asimila L12
    processateTemplate();
    calculateImage();
}
//punto de referencia del cabezal de la plantilla en S4
//valor por defecto: {L12, 0}
void TArm::setP__2(const TDoublePoint& t_P__2)
{
    //comprueba las precondiciones
    if(t_P__2.x == 0)
        throw EImproperArgument("P__2.x not should be zero");
    if(t_P__2.y != 0)
        throw EImproperArgument("P__2.y should be zero");

    P__2 = t_P__2; //asigna el nuevo valor

    //asimila P__2
    L12 = P__2.Mod();
    double aux = gettheta2();
    const double COS = cos(aux);
    const double SIN = sin(aux);
    P2.x = P__2.x*COS - P__2.y*SIN + P1.x;
    P2.y = P__2.x*SIN + P__2.y*COS + P1.y;
    calculateImage();
}

//coordenada radial de P3 en S4
//debe ser no negativa
//valor por defecto: MEGARA_L mm
void TArm::setL13(double t_L13)
{
    //comprueba las precondiciones
    if(t_L13 < 0)
        throw EImproperArgument("radio L13 should be nonnegative");

    L13 = t_L13; //asigna el nuevo valor

    //asimila L13
    processateTemplate();
    calculateImage();
}

//coordenada angular de P3 en S4
//puede ser cualquier valor
//pero es recomendable que esté en (-M_PI, M_PI]
//valor por defecto: 0 rad
void TArm::settheta__3(double t_theta__3)
{
    //Nótese que theta__3 debe admitir cualquier valor.

    theta__3 = t_theta__3; //asigna el nuevo valor

    //asimila theta__3
    processateTemplate();
    calculateImage();
}
//orientación del origen de coordenadas de S3 en S2
//es igual que theta__3
//valor por defecto: 0 rad
void TArm::settheta__O3(double theta__O3)
{
    try {
        settheta__3(theta__O3);
        //es igual que theta__3
    } catch(Exception& E) {
        E.Message.Insert(1, "setting theta__O3: ");
        throw;
    }
}
//centro de la lente de la plantilla en S4
void TArm::setP__3(const TDoublePoint& t_P__3)
{
    //Nótese que P__3 debe admitir cualquier valor.

    P__3 = t_P__3; //asigna el nuevo valor

    //asimila las propiedades de plantilla
    L13 = P__3.Mod();
    theta__3 = P__3.Arg();
    double aux = gettheta2();
    const double COS = cos(aux);
    const double SIN = sin(aux);
    P3.x = P__3.x*COS - P__3.y*SIN + P1.x;
    P3.y = P__3.x*SIN + P__3.y*COS + P1.y;
    calculateImage();
}

//radio de la lente del brazo
//debe ser mayor que cero
//valor por defecto: 0.75 mm
void TArm::setR3(double t_R3)
{
    //comprueba las precondiciones
    if(t_R3 <= 0)
        throw EImproperArgument("radio R3 should be upper zero");

    R3 = t_R3; //asigna el nuevo valor

    //R3 surte efecto cuando el brazo es dibujado
    //o se determina si un punto está sobre la lente
}

//plantilla del contorno del brazo en S4
//valor por defecto: MEGARA_Contour____Text
void TArm::setContour____(const TContourFigureList& t_Contour____)
{
    //comprueba las precondiciones
    if(!t_Contour____.isAContourSorted())
        throw EImproperArgument("the contour (Contour____) should be sorted");

    //asigna el nuevo valor
    Destroy(Contour____);
    Contour____.Copy(t_Contour____);

    //asimila el nuevo valor
    processateTemplate();
    calculateImage();
}

//--------------------------------------------------------------------------
//PROPIEDADES DE ORIENTACIÓN EN RADIANES
//DE LECTURA/ESCRITURA:

//orientación del origen de coordenadas de S3 en S0
//cuando el eje 2 está en el origen
//valor por defecto: thetaO2 + gettheta__O3() rad
//donde:
//  thetaO2: lo indica el usuario en el constructor
//      y su valor por defecto es M_PI/2
//  theta__O3: es igual a theta____3
//      y su valor por defecto es 0
void TArm::setthetaO3(double t_thetaO3)
{
    //asigna el nuevo valor
    thetaO3 = t_thetaO3;

    //asimila la orientación
    calculateImage();
}

//límite inferior del intervalo de giro de P3 en S3
//debe estar en el dominio de definición de F(theta___3)
//no debe ser mayor que theta___3max
//valor por defecto: MEGARA_theta___3min rad
void TArm::settheta___3min(double t_theta___3min)
{
    //comprueba las precondiciones
    if(!getF().BelongToDomain(t_theta___3min))
        throw EImproperArgument("angle theta___3min should be in domain F(theta___3)");
    if(t_theta___3min > theta___3max)
        throw EImproperArgument("angle theta___3min should not be upper than angle theta___3max");

    theta___3min = t_theta___3min; //asigna el nuevo valor

    //asimila theta___3min
    Q.setxmin(getp___3min()); //ajusta el dominio de cuantificación
    if(theta___3 < theta___3min) //constriñe theta___3 a su dominio
        settheta___3(theta___3min);

    //la asignación a theta___3 implicaría su asimilación
}
//límite superior del intervalo de giro de P3 en S3
//debe estar en el dominio de definición de F(theta___3)
//no debe ser menor que theta___3min
//valor por defecto: MEGARA_theta___3max rad
void TArm::settheta___3max(double t_theta___3max)
{
    //comprueba las precondiciones
    if(!getF().BelongToDomain(t_theta___3max))
        throw EImproperArgument("angle theta___3max should be in domain F(theta___3)");
    if(t_theta___3max < theta___3min)
        throw EImproperArgument("angle theta___3max should be less than angle theta___3min");

    theta___3max = t_theta___3max; //asigna el nuevo valor

    //asimila theta___3max:
    Q.setxmax(getp___3max()); //ajusta el dominio de cuantificación
    if(theta___3 > theta___3max) //constriñe theta___3 a su dominio
        settheta___3(theta___3max);

    //la asignación a theta___3 implicaría su asimilación
}
//ángulo de orientación de P3 en S3
//debe estar en el dominio de definición de F(theta___3)
//Escrutura:
//  if(Quantify___)
//      p_theta___3 = Qtheta___3(_theta___3);
//  else
//      p_theta___3 = theta___3;
//valor por defecto: 0 rad
void TArm::settheta___3(double t_theta___3)
{
    //comprueba las precondiciones
    if(isntInDomaintheta___3(t_theta___3))
        throw EImproperArgument("angle theta___3 should be in his domain [theta___3min, theta___3max]");

    //si la cuantificación está activada
    if(Quantify___)
        //cuantifica el nuevo valor
        t_theta___3 = Qtheta___3(t_theta___3);

    //ADVERTENCIA: la cuantificación podría dar lugar a que theta___3
    //quedara ligeramente fuera de su dominio.

    //constriñe theta___3 a su dominio
    if(t_theta___3 < theta___3min)
        t_theta___3 = theta___3min;
    else if(t_theta___3 > theta___3max)
        t_theta___3 = theta___3max;

    //si el nuevo valor difiere del actual
    if(t_theta___3 != theta___3) {
        //asigna el nuevo valor
        theta___3 = t_theta___3;
        //asimila theta___3
        calculateImage();
    }
}
//ángulo de orientación de P3 en S0
//theta3 debe estar en su dominio
//Lectura:
//  theta3 = thetaO2 + theta__O3 + theta___3
//Escritura:
//  theta___3 = theta3 - thetaO2 - theta__O3
//valor por defecto: 0 + PI/2 + 0 rad
double TArm::gettheta3(void) const
{
    return thetaO3 + theta___3;
}
void TArm::settheta3(double theta3)
{
    try {
        settheta___3(theta3 - getthetaO2() - gettheta__O3());
        //las propiedades de orientación serán asimiladas
        //en la asignación a theta___3
    } catch(EImproperArgument& E) {
        E.Message.Insert(1, "setting theta3: ");
        throw; //relanza la excepción
    }
}

//orientación del origen de coordenadas de S2 en S0
//Lectura:
//  thetaO2 = thetaO3 - theta__O3
//Escritura:
//  p_thetaO3 = thetaO2 + theta__O3
double TArm::getthetaO2(void) const
{
    //traduce y devuelve el valor
    return thetaO3 - gettheta__O3();
}
void TArm::setthetaO2(double thetaO2)
{
    try {
        //traduce y asigna el nuevo valor
        setthetaO3(thetaO2 + gettheta__O3());
    } catch(EImproperArgument& E) {
        E.Message.Insert(1, "setting thetaO2: ");
        throw; //relanza la excepción
    }
}

//valor aproximado de referencia del
//límite inferior del intervalo de giro de P2 en S3
//debe estar en su dominio
//debe ser menor que theta___2max
//Lectura:
//  theta___2min = theta___3min - theta___3;
//Escritura:
//  p_theta___3min = theta___2min + theta___3;
//valor por defecto: MEGARAtheta__2min rad
double TArm::gettheta___2min(void) const
{
    return theta___3min - theta__3;
}
void TArm::settheta___2min(double theta___2min)
{
    //calcula theta___3min
    double t_theta___3min = theta___2min + theta__3;

    //comprueba las precondiciones
    if(!F.BelongToDomain(t_theta___3min))
        throw EImproperArgument("angle theta___2min should be in his definition domain");
    if(t_theta___3min > theta___3max)
        throw EImproperArgument("angle theta___2min should not be upper than angle theta___2max");

    theta___3min = t_theta___3min; //asigna el nuevo valor

    //asimila theta___3min
    Q.setxmin(getp___3min());
    if(theta___3 < theta___3min)
        settheta___3(theta___3min);

    //la asignación a theta___3 implicaría su asimilación
}
//valor aproximado de referencia del
//límite superior del intervalo de giro de P2 en S3
//debe estar en su dominio
//debe ser mayor que theta___2min
//Lectura:
//  p_theta___2max = theta___3max - theta___3;
//Escritura:
//  theta___3max = theta___2max + theta___3;
//valor por defecto: MEGARAtheta__2max rad
double TArm::gettheta___2max(void) const
{
    return theta___3max - theta__3;
}
void TArm::settheta___2max(double theta___2max)
{
    //calcula theta___3max
    double t_theta___3max = theta___2max + theta__3;

    //comprueba las precondiciones
    if(!F.BelongToDomain(t_theta___3max))
        throw EImproperArgument("angle theta___2max should be in his definition domain");
    if(t_theta___3max < theta___3min)
        throw EImproperArgument("angle theta___2max should not be less than angle theta___2min");

    theta___3max = t_theta___3max; //asigna el nuevo valor

    //asimila theta___3max
    Q.setxmax(getp___3max());
    if(theta___3 > theta___3max)
        settheta___3(theta___3max);

    //la asignación a theta___3 implicaría su asimilación
}
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
double TArm::gettheta___2(void) const
{
    return theta___3 - theta__3;
}
void TArm::settheta___2(double theta___2)
{
    try {
        settheta___3(theta___2 + theta__3);
        //las propiedades de orientación serán asimiladas
        //en la asignación a theta___3
    } catch(EImproperArgument& E) {
        E.Message.Insert(1, "setting theta___2: ");
        throw;
    }
}
//ángulo de orientación de P2 en S0
//theta2 debe estar en su dominio
//Lectura:
//  theta2 = theta3 - theta___23
//Escritura:
//  theta3 = theta2 + theta___23
//valor por defecto: PI/2 - 0 rad
double TArm::gettheta2(void) const
{
    return gettheta3() - theta__3;
}
void TArm::settheta2(double theta2)
{
    try {
        settheta3(theta2 + theta__3);
        //las propiedades de orientación serán asimiladas
        //en la asignación a theta___3
    } catch(EImproperArgument& E) {
        //cambia el texto de la excepción
        E.Message.Insert(1, "setting theta2: ");
        throw;
    }
}

//first position angle of rotor 2 in rad
//  theta___3first = max(0, theta___3min)
double TArm::gettheta___3first(void) const
{
    return max(0., theta___3min);
}
//last position angle of rotor 2 in rad
//  theta___3last = min(M_2PI, theta___3max)
double TArm::gettheta___3last(void) const
{
    return min(M_2PI, gettheta___3max());
}

//--------------------------------------------------------------------------
//PROPIEDADES DE CUANTIFICACIÓN
//DE LECTURA/ESCRITURA:

//función de transformación de de radianes a pasos de P3
//debe estar definida en algún punto
//debe ser monótona creciente
//valor por defecto:
//  {(M_2PI, MEGARASB2),
//  (0, 0),
//  (M_2PI, MEGARASB2)}
void TArm::setF(const TFunction& t_F)
{
    //comprueba las precondiciones
    if(t_F.getCount() <= 0)
        throw EImproperArgument("function F should be defined in some point");
    if(t_F.Monotony() != 1)
        throw EImproperArgument("function F(theta___3) should be monotically increasing");
    if(t_F.haventInverse())
        throw EImproperArgument("function F should have inverse");

    //F(theta___3) debe ser monótona creciente
    //para poder obtener la función inversa G(p___3).

    //Nótese que no se imponen las siguientes restricciones:
    //      La función F(theta___3) debe estar definida en 0.
    //      F(0) debería ser igual a cero.
    //      La función F(theta___3) debe estar definida en M_2PI.

    //asigna la función F
    F = t_F;

    //asimila la función F
    processateF();
    calculateImage();

    //En el método CalculateImage,
    //en caso de que la cuantificación esté activada,
    //la asignación de theta___3 a sí mismo,
    //hará que se desplace a la posición estable más próxima
    //la cual será asimilada.
}

//interruptor de cuantificación de p___3
//indica si deben cuantificarse los valores asignados a p___3
//valor por defecto: true
void TArm::setQuantify___(bool t_Quantify___)
{
    //si activa la cuantificación
    if(!Quantify___ && t_Quantify___)
        //establece theta___3 en la posición estable más cercana
        settheta___3(theta___3);

    Quantify___ = t_Quantify___; //asigna el nuevo valor
}

//--------------------------------------------------------------------------
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
double TArm::getp___3min(void) const
{
    //devuelve theta___3min traducido a pasos
    return F.Image(theta___3min);
}
void TArm::setp___3min(double p___3min)
{
    //traduce a radianes
    double t_theta___3min = G.Image(p___3min);

    //comprueba las precondiciones
    if(!F.BelongToDomain(t_theta___3min))
        throw EImproperArgument("angle p___3min should be in image domain F(theta___3)");
    if(t_theta___3min > theta___3max)
        throw EImproperArgument("angle p___3min should not be upper than angle p___3max");

    theta___3min = t_theta___3min; //asigna el nuevo valor

    //asimila theta___3min
    Q.setxmin(getp___3min());
    if(theta___3 < theta___3min)
        settheta___3(theta___3min);

    //la asignación a theta___3 implicaría su asimilación
}
//Valor aproximado de referencia del límite superior
//del intervalo de orientación de P3 en S3 en pasos.
//Lectura:
//  p___3max = F(theta___3max);
//Escritura:
//  p_theta___3max = G(p___3max);
//Valor por defecto: F(MEGARAtheta___3max) steps.
double TArm::getp___3max(void) const
{
    return F.Image(theta___3max);
}
void TArm::setp___3max(double p___3max)
{
    //traduce a radianes
    double t_theta___3max = getG().Image(p___3max);

    //comprueba las precondiciones
    if(!F.BelongToDomain(t_theta___3max))
        throw EImproperArgument("angle p___3max should be in image domain F(theta___3)");
    if(t_theta___3max <= theta___3max)
        throw EImproperArgument("angle p___3max should be upper than angle p___3max");

    theta___3max = t_theta___3max; //asigna el nuevo valor

    //asimila theta___3max
    Q.setxmax(getp___3max());
    if(theta___3 > theta___3max)
        settheta___3(theta___3max);

    //la asignación a theta___3 implicaría su asimilación
}
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
double TArm::getp___3(void) const
{
    //si la cuantificación está activada
    if(Quantify___)
        //devuelve el valor en pasos cuantificado
        return Q.Quantifice(F.Image(theta___3));
    else
        //devuelve el valor en pasos sin cuantificar
        return F.Image(theta___3);

    //En lectura p___3 se cuantifica para evitar el error numérico
    //introducido por la función F(theta___3).
}
void TArm::setp___3(double p___3)
{
    //si la cuantificación está activada
    if(Quantify___)
        //cuantifica el valor, lo traduce a radianes y lo asigna
        theta___3 = G.Image(Q.Quantifice(p___3));
    else
        //traduce el valor a radianes y lo asigna
        theta___3 = G.Image(p___3);

    //asimila theta___3
    calculateImage();
}

//--------------------------------------------------------------------------
//PROPIEDADES DE LOCALIZACIÓN
//DE LECTURA/ESCRITURA:

//posición del eje de rotación del brazo en S0
void TArm::setP1(TDoublePoint t_P1)
{
    P1 = t_P1; //asigna el nuevo valor

    //asimila las propiedades de localización
    calculateImage();
}

//margen perimentral de seguridad
//debe ser mayor que cero
//valor por defecto: MEGARA_SPMgenPairPPDP_p mm
void TArm::setSPM(double t_SPM)
{
    //el margen perimetral de seguridad SPM debería ser mayor que cero
    if(t_SPM <= 0)
        throw EImproperArgument("security perimetral margin SPM should be upper zero");

    SPM = t_SPM; //asigna el nuevo valor

    //El nuevo valor será utilizado en cuenta en el método:
    //      bool Collides(TArm *Arm);
}

//--------------------------------------------------------------------------
//PROPIEDADES DE PLANTILLA
//INDEPENDIENTES EN FORMATO TEXTO

AnsiString TArm::getL12Text(void) const
{
    return FloatToStr(getL12());
}
void TArm::setL12Text(const AnsiString& S)
{
    try {
        setL12(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting L12 in text format: ");
        throw ;
    }
}
AnsiString TArm::getP__2Text(void) const
{
    return DPointToStr(P__2);
}
void TArm::setP__2Text(AnsiString &S)
{
    try {
        setP__2(StrToDPoint(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting P__2 in text format: ");
        throw ;
    }
}

AnsiString TArm::getL13Text(void) const
{
    return FloatToStr(getL13());
}
void TArm::setL13Text(const AnsiString& S)
{
    try {
        setL13(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting L13 in text format: ");
        throw ;
    }
}
AnsiString TArm::gettheta__3Text(void) const
{
    return FloatToStr(theta__3);
}
void TArm::settheta__3Text(const AnsiString& S)
{
    try {
        settheta__3(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting theta__3 in text format: ");
        throw ;
    }
}
AnsiString TArm::gettheta__O3Text(void) const
{
    return FloatToStr(gettheta__O3());
}
void TArm::settheta__O3Text(const AnsiString& S)
{
    try {
        settheta__O3(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting theta__O3 in text format: ");
        throw ;
    }
}
AnsiString TArm::getP__3Text(void) const
{
    return DPointToStr(P__3);
}
void TArm::setP__3Text(AnsiString &S)
{
    try {
        setP__3(StrToDPoint(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting P__3 in text format: ");
        throw ;
    }
}

AnsiString TArm::getR3Text(void) const
{
    return FloatToStr(getR3());
}
void TArm::setR3Text(const AnsiString& S)
{
    try {
        setR3(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting R3 in text format: ");
        throw ;
    }
}

//dirección en memoria de la propiedad compuesta Contour____
AnsiString TArm::getContour____AddressText(void) const
{
    return IntToHex(intptr_t(&Contour____));
}
//propiedad Contour____ en formato texto lista
void TArm::setContour____Text(const AnsiString& S)
{
    try {
        //construye un contorno tampón
        TContourFigureList t_Contour____;
        //asigna el nuevo valor al contorno tampón
        t_Contour____.setText(S);
        //intenta asignar el contorno tampón
        setContour____(t_Contour____);

        //La asignación a Contour____ provocará la asimilación de Contour____ mediante:
        //      processateTemplate();
        //      calculateImage();

        //No olvidar que la cadena S no no debe asignarse directamente
        //a Contour____.Text, por que el método SetContour____ comprueba si Contour____
        //cumple las restricciones.

        Destroy(t_Contour____);

    } catch(Exception& E) {
        E.Message.Insert(1, "setting Contour____ in text format: ");
        throw;
    }
}
//propiedad Contour____ en formato texto columna
void TArm::setContour____ColumnText(const AnsiString& S)
{
    try {
        //construye un contorno tampón
        TContourFigureList t_Contour____;
        //asigna el nuevo valor al clon
        t_Contour____.setColumnText(S);
        //intenta asignar la nueva plantilla
        setContour____(t_Contour____);

        //La asignación a Contour____ provocará la asimilación de Contour____ mediante:
        //      processateTemplate();
        //      calculateImage();

        //No olvidar que la cadena S no no debe asignarse directamente
        //a Contour____.Text, por que el método SetContour____ compruba si Contour____
        //cumple las restricciones.

        Destroy(t_Contour____);

    } catch(Exception& E) {
        E.Message.Insert(1, "setting Contour____ in column text format: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//PROPIEDADES DE PLANTILLA
//DEPENDIENTES EN FORMATO TEXTO

/*AnsiString TArm::getP____2Text(void) const
{
    return DPointToStr(P____2);
}*/
AnsiString TArm::getP____1Text(void) const
{
    return DPointToStr(P____1);
}

/*//dirección en memoria de la propiedad compuesta Contour__
AnsiString TArm::getContour__AddressText(void) const
{
    return IntToHex(intptr_t(&Contour__));
}
*/
AnsiString TArm::getL1VText(void) const
{
    return FloatToStr(L1V);
}
AnsiString TArm::getV__Text(void) const
{
    return DPointToStr(V__);
}

//---------------------------------------------------------------------------
//PROPIEDADES DE ORIENTACIÓN EN RADIANES
//DE LECTURA/ESCRITURA EN FORMATO TEXTO

AnsiString TArm::getthetaO3Text(void) const
{
    return FloatToStr(getthetaO3());
}
void TArm::setthetaO3Text(const AnsiString& S)
{
    try {
        setthetaO3(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting thetaO3 in text format: ");
        throw;
    }
}

AnsiString TArm::gettheta___3minText(void) const
{
    return FloatToStr(gettheta___3min());
}
void TArm::settheta___3minText(const AnsiString& S)
{
    try {
        settheta___3min(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting theta___3min in text format: ");
        throw;
    }
}
AnsiString TArm::gettheta___3maxText(void) const
{
    return FloatToStr(gettheta___3max());
}
void TArm::settheta___3maxText(const AnsiString& S)
{
    try {
        settheta___3max(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting theta___3max in text format: ");
        throw;
    }
}
AnsiString TArm::gettheta___3Text(void) const
{
    return FloatToStr(gettheta___3());
}
void TArm::settheta___3Text(const AnsiString& S)
{
    try {
        settheta___3(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting theta___3 in text format: ");
        throw;
    }
}
AnsiString TArm::gettheta3Text(void) const
{
    return FloatToStr(gettheta3());
}
void TArm::settheta3Text(const AnsiString& S)
{
    try {
        settheta3(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting theta3 in text format: ");
        throw;
    }
}

AnsiString TArm::getthetaO2Text(void) const
{
    return FloatToStr(getthetaO2());
}
void TArm::setthetaO2Text(const AnsiString& S)
{
    try {
        setthetaO2(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting thetaO2 in text format: ");
        throw;
    }
}

AnsiString TArm::gettheta___2minText(void) const
{
    return FloatToStr(gettheta___2min());
}
void TArm::settheta___2minText(const AnsiString& S)
{
    try {
        settheta___2min(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting theta___2min in text format: ");
        throw;
    }
}
AnsiString TArm::gettheta___2maxText(void) const
{
    return FloatToStr(gettheta___2max());
}
void TArm::settheta___2maxText(const AnsiString& S)
{
    try {
        settheta___2max(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting theta___2max in text format: ");
        throw;
    }
}
AnsiString TArm::gettheta___2Text(void) const
{
    return FloatToStr(gettheta___2());
}
void TArm::settheta___2Text(const AnsiString& S)
{
    try {
        settheta___2(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting theta___2 in text format: ");
        throw;
    }
}
AnsiString TArm::gettheta2Text(void) const
{
    return FloatToStr(gettheta2());
}
void TArm::settheta2Text(const AnsiString& S)
{
    try {
        settheta2(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting theta2 in text format: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//PROPIEDADES DE CUANTIFICACIÓN
//DE LECTURA/ESCRITURA EN FORMATO TEXTO

//dirección en memoria de la función F
AnsiString TArm::getFAddressText(void) const
{
    return IntToHex(intptr_t(&F));
}
//puntos de lafunción F enformato lista
void TArm::setFPointsText(const AnsiString& S)
{
    try {
        //construye una función tampón
        TFunction F;
        //asigna el nuevo valor a la función tampón
        F.setPointsText(S);
        //intenta asignar la función tampón a la función
        setF(F);

        //La asignación a F provocará la asimilación de F mediante:
        //  processateF();
        //  calculateImage();

        //No olvidar que la cadena S no no debe asignarse directamente
        //a F.PointsText, por que el método SetF comprubea si F
        //cumple las restricciones.

    } catch(Exception& E) {
        E.Message.Insert(1, "setting FPoints in text format: ");
        throw;
    }
}
//puntos de lafunción F enformato tabla
void TArm::setFTableText(const AnsiString& S)
{
    try {
        //construye una función tampón
        TFunction F;
        //borra los separadores iniciales y finales
        AnsiString D;
        StrTrim(D, S);
        //asigna el nuevo valor a la función tampón
        F.setTableText(D);
        //intenta asignar la función tampón a la función
        setF(F);

        //La asignación a F provocará la asimilación de F mediante:
        //  processateF();
        //  calculateImage();

        //No olvidar que la cadena S no no debe asignarse directamente
        //a F.PointsText, por que el método SetF comprubea si F
        //cumple las restricciones.

    } catch(Exception& E) {
        E.Message.Insert(1, "setting FTable in text format: ");
        throw;
    }
}

AnsiString TArm::getQuantify___Text(void) const
{
    return BoolToStr(getQuantify___(), true);
}
void TArm::setQuantify___Text(const AnsiString& S)
{
    try {
        Quantify___ = StrToBool(S);
    } catch(Exception& E) {
        E.Message.Insert(1, "setting Quantify___ in text format: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//PROPIEDADES DE CUANTIFICACIÓN
//DE SOLO LECTURA EN FORMATO TEXTO

//dirección en memoria de la función G
AnsiString TArm::getGAddressText(void) const
{
    return IntToHex(intptr_t(&G));
}

AnsiString TArm::getSB2Text(void) const
{
    return FloatToStr(getSB2());
}
AnsiString TArm::getrbsText(void) const
{
    return FloatToStr(getrbs());
}

//---------------------------------------------------------------------------
//PROPIEDADES DE ORIENTACIÓN EN PASOS
//DE LECTURA/ESCRITURA EN FORMATO TEXTO

AnsiString TArm::getp___3minText(void) const
{
    return FloatToStr(getp___3min());
}
void TArm::setp___3minText(const AnsiString& S)
{
    try {
        setp___3min(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting p___3min in text format: ");
        throw;
    }
}
AnsiString TArm::getp___3maxText(void) const
{
    return FloatToStr(getp___3max());
}
void TArm::setp___3maxText(const AnsiString& S)
{
    try {
        setp___3max(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting p___3max in text format: ");
        throw;
    }
}
AnsiString TArm::getp___3Text(void) const
{
    return FloatToStr(getp___3());
}
void TArm::setp___3Text(const AnsiString& S)
{
    try {
        setp___3(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting p___3 in text format: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//PROPIEDADES DE LOCALIZACIÓN
//DE LECTURA/ESCRITURA EN FORMATO TEXTO

AnsiString TArm::getP1Text(void) const
{
    return DPointToStr(getP1());
}
void TArm::setP1Text(const AnsiString& S)
{
    try {
        setP1(StrToDPoint(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting P1 in text format: ");
        throw;
    }
}

AnsiString TArm::getSPMText(void) const
{
    return FloatToStr(getSPM());
}
void TArm::setSPMText(const AnsiString& S)
{
    try {
        setSPM(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting SPM in text format: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//PROPIEDADES DE LOCALIZACIÓN
//DE SOLO LECTURA EN FORMATO TEXTO

AnsiString TArm::getP2Text(void) const
{
    return DPointToStr(getP2());
}
AnsiString TArm::getP3Text(void) const
{
    return DPointToStr(getP3());
}
AnsiString TArm::getVText(void) const
{
    return DPointToStr(getV());
}

//dirección en memoria de la propiedad compuesta Contour
AnsiString TArm::getContourAddressText(void) const
{
    return IntToHex(intptr_t(&Contour));
}

//---------------------------------------------------------------------------
//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO

//conjunto de propiedades de plantilla
//en formato texto
AnsiString TArm::getTemplateText(void) const
{
    AnsiString S;

    S += "R/W:\r\n";

    S += AnsiString("    L12 = ")+getL12Text()+AnsiString("\r\n");
    S += AnsiString("    P__2: ")+getP__2Text()+AnsiString("\r\n");
    S += AnsiString("    L13 = ")+getL13Text()+AnsiString("\r\n");
    S += AnsiString("    theta__3 = ")+gettheta__3Text()+AnsiString("\r\n");
    S += AnsiString("    theta__O3 = ")+gettheta__O3Text()+AnsiString("\r\n");
    S += AnsiString("    P__3: ")+getP__3Text()+AnsiString("\r\n");
    S += AnsiString("    R3 = ")+getR3Text()+AnsiString("\r\n");

    S += AnsiString("    Contour____Address = ")+getContour____AddressText()+AnsiString("\r\n");

    S += "R:\r\n";

//    S += AnsiString("    P____2: ")+getP____2Text()+AnsiString("\r\n");
    S += AnsiString("    P____1: ")+getP____1Text()+AnsiString("\r\n");
//    S += AnsiString("    Contour__Address = ")+getContour__AddressText()+AnsiString("\r\n");
    S += AnsiString("    L1V: ")+getL1VText()+AnsiString("\r\n");
    S += AnsiString("    V__: ")+getV__Text();

    return S;
}
//conjunto de propiedades de orientación en radianes
//en formato texto
AnsiString TArm::getOrientationRadiansText(void) const
{
    AnsiString S;

    S += "R/W:\r\n";

    S += AnsiString("    thetaO3 = ")+getthetaO3Text()+AnsiString("\r\n");

    S += AnsiString("    theta___3min = ")+gettheta___3minText()+AnsiString("\r\n");
    S += AnsiString("    theta___3max = ")+gettheta___3maxText()+AnsiString("\r\n");
    S += AnsiString("    theta___3 = ")+gettheta___3Text()+AnsiString("\r\n");
    S += AnsiString("    theta3 = ")+gettheta3Text()+AnsiString("\r\n");
    S += AnsiString("    theta___3s = ")+theta___3s.getText()+AnsiString("\r\n");

    S += AnsiString("    thetaO2 = ")+getthetaO2Text()+AnsiString("\r\n");

    S += AnsiString("    theta___2min = ")+gettheta___2minText()+AnsiString("\r\n");
    S += AnsiString("    theta___2max = ")+gettheta___2maxText()+AnsiString("\r\n");
    S += AnsiString("    theta___2 = ")+gettheta___2Text()+AnsiString("\r\n");
    S += AnsiString("    theta2 = ")+gettheta2Text();

    return S;
}
//conjunto de propiedades de cuantificación
//en formato texto
AnsiString TArm::getQuantificationText(void) const
{
    AnsiString S;

    S += "R/W:\r\n";

    S += AnsiString("    FAddress = ")+getFAddressText()+AnsiString("\r\n");

    S += AnsiString("    Quantify___ = ")+getQuantify___Text()+AnsiString("\r\n");
    S += AnsiString("    Quantify___s = ")+Quantify___s.getText()+AnsiString("\r\n");

    S += AnsiString("R:\r\n");

    S += AnsiString("    GAddress: ")+getGAddressText()+AnsiString("\r\n");

    S += AnsiString("    SB2: ")+getSB2Text()+AnsiString("\r\n");
    S += AnsiString("    rbs: ")+getrbsText()+AnsiString("\r\n");
    S += AnsiString("    Q: ")+StrIndent(getQ().getAssignsText(), AnsiString("        "));

    return S;
}
//conjunto de propiedades de orientación en pasos
//en formato texto
AnsiString TArm::getOrientationStepsText(void) const
{
    AnsiString S;

    S += "R/W:\r\n";

    S += AnsiString("    p___3 = ")+getp___3Text()+AnsiString("\r\n");
    S += AnsiString("    p___3min = ")+getp___3minText()+AnsiString("\r\n");
    S += AnsiString("    p___3max = ")+getp___3maxText();

    return S;
}
//conjunto de propiedades de localización
//en formato texto
AnsiString TArm::getLocationText(void) const
{
    AnsiString S;

    S += "R/W:\r\n";

    S += AnsiString("    P1 = ")+getP1Text()+AnsiString("\r\n");
    S += AnsiString("    SPM = ")+getSPMText()+AnsiString("\r\n");

    S += "R:\r\n";

    S += AnsiString("    P2: ")+getP2Text()+AnsiString("\r\n");
    S += AnsiString("    P3: ")+getP3Text()+AnsiString("\r\n");
    S += AnsiString("    V: ")+getVText()+AnsiString("\r\n");

    S += AnsiString("    ContourAddress: ")+getContourAddressText();

    return S;
}

//conjunto de todas las propiedades
//en formato texto
AnsiString TArm::getAllText(void) const
{
    AnsiString S;

    S += "Template:\r\n";
    S += StrIndent(getTemplateText())+AnsiString("\r\n");
    S += "OrientationRadians:\r\n";
    S += StrIndent(getOrientationRadiansText())+AnsiString("\r\n");
    S += "Quantification:\r\n";
    S += StrIndent(getQuantificationText())+AnsiString("\r\n");
    S += "OrientationSteps:\r\n";
    S += StrIndent(getOrientationStepsText())+AnsiString("\r\n");
    S += "Location:\r\n";
    S += StrIndent(getLocationText());

    return S;
}

//CONJUNTOS DE LAS PROPIEDADES DE INSTANCIA EN FORMATO TEXTO:

//conjunto de todas las propiedades de instancia
//en formato texto
AnsiString TArm::getInstanceText(void) const
{
    string str;

    str = commentedLine("L12 = "+getL12Text().str, "distance between P1 and P2 (in mm)");
    str += "\r\n"+commentedLine("L13 = "+getL13Text().str, "distance between P1 and P3 (in mm)");
    str += "\r\n"+commentedLine("theta__O3 = "+gettheta__O3Text().str, "orientation of S3 respect S2 (in rad)");
    str += "\r\n"+commentedLine("R3 = "+getR3Text().str, "radio of representative circle of the microlens (in mm)");

    str += "\r\n"+commentedLine("theta___3min = "+gettheta___3minText().str, "position angle’s lower limit of axis 1-3 respect S3 (in rad)");
    str += "\r\n"+commentedLine("theta___3max = "+gettheta___3maxText().str, "position angle’s upper limit of axis 1-3 respect S3 (in rad)");
    str += "\r\n"+commentedLine("theta___3 = "+gettheta___3Text().str, "position angle of axis 1-3 respect S3 (in rad)");

    return AnsiString(str);
}
void TArm::setInstanceText(const AnsiString& S)
{
    //initalize the index to the first char of S
    int i = 1;
    try {
        //lee la instancia en una variable tampón
        TArm A(this);
        readInstance(&A, S, i);

        //busca un texto inesperado
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text: "+StrFirstChars(S.SubString(i, S.Length() - i + 1)).str);

        //asigna la variable tampón
        clone(&A);
    }
    catch(Exception& E) {
        unsigned int row, col;
        strPositionToCoordinates(row, col, S.str, i-1);
        E.Message.Insert(1, "setting p___3min in text format in row "+inttostr(row)+" and column "+inttostr(col)+": ");
        throw;
    }
}

//---------------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//lee una instancia de brazo en una cadena
void  TArm::readInstance(TArm *A, const AnsiString& S, int& i)
{
    try {
        //comprueba las precondiciones
        if(A == NULL)
            throw EImproperArgument("pointer A shouldpoint to built arm");
        if(i<1 || S.Length()+1<i)
            throw EImproperArgument("index i should indicate a position in the string S");

        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //variables tampón
        double L12, L13, theta__O3, R3;
        double theta___3min, theta___3max, theta___3;

        StrTravelSeparators(S, i);
        StrTravelLabel("L12", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(L12, S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("L13", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(L13, S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("theta__O3", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(theta__O3, S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("R3", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(R3, S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("theta___3min", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(theta___3min, S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("theta___3max", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(theta___3max, S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("theta___3", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(theta___3, S, i);

        //asigna las variables tampón
        A->setTemplate(L12, L13, theta__O3, R3);
        A->setOrientationRadians(theta___3min, theta___3max, theta___3);

    }catch(Exception& E) {
        E.Message.Insert(1, "reading instance of arm: ");
        throw;
    }
}

//--------------------------------------------------------------------------
//MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

//construye un brazo
//con la posición y orientación indicadas
TArm::TArm(TDoublePoint t_P1, double t_thetaO2) :
    //construye las propiedades privadas
    Contour____(8),
//    Contour__(8),
    F(),
    G(), Q(),
    Contour(8),
Contour__(8),
    //construye las propiedades públicas
    theta___3s(2), //las funciones de theta___3s ya están apuntadas por que es de tipo TVector
    Quantify___s(2, NULL, NULL, NULL, StrPrintBool)
{
    //INICIALIZA LA PLANTILLA:

    //inicializa los parámetros de dimensión por defecto
    L12 = MEGARA_L;
    L13 = L12;
    theta__3 = 0; //recuerde que theta__O3 es igual que theta__3
    R3 = 0.75;

    //inicializa el contorno
    setContour____Text(MEGARA_Contour____Text);

    //INICIALIZA LAS PROPIEDADES DE ORIENTACIÓN:

    //inicializa los parámetros y variables de orientación en radianes
    thetaO3 = t_thetaO2 + gettheta__O3();
    theta___3min = MEGARA_theta___3min;
    theta___3max = MEGARA_theta___3max;
    theta___3 = 0;

    //Para poder cuantificar theta___3 debe determinarse antes:
    //      {[theta___3min, theta___3max], F} --> {Q(p___3)}
    //Pero si theta___3 está fuera de [theta___3min,theta___3max]
    //será arrastrado, pudiendo ser cuantificado por Q.
    //Por eso theta___3 dbe inicializarse dentro de su dominio.

    //INICIALIZA LAS PROPIEDADES DE CUANTIFICACIÓN:

    //nombra las funciones
    F.Label = "F2";
    G.Label = "G2";

    //añade los puntos de la función de compresión
    F.Add(-M_2PI, -double(MEGARA_SB2));
    F.Add(0., 0.);
    F.Add(M_2PI, double(MEGARA_SB2));
    F.Add(2*M_2PI, double(2*MEGARA_SB2));
    //elimina las incongruencias del formato texto
    //de la función de compansión
    F.setPointsText(F.getPointsText());

    //F debe estar definida en el intervalo [-M_2PI, 2*M_2PI]
    //para permitir el ajuste olgado de [theta___3min, theta___3max].

    //inicializa las propiedades de cuantificación
    Quantify___ = true;

    //INICIALIZA LAS PROPIEDADES DE LOCACLIZACIÓN:

    //inicializa las propiedades de posición
    P1 = t_P1;

    //initialize the SPM
    SPM = MEGARA_SPMgenPairPPDP_p;


    //------------------------------------------------------------------
    //ASIMILA LAS PROPIEDADES DE LA PLANTILLA:

    //Apartir de:
    //  {L12, L13, theta__3}
    //  {Contour____}
    //Determina:
    //  {P____2, P____1}
    //  {P__2, P__3}
    //  {Contour__}
    //  {V__, L1V}
    processateTemplate();

    //ASIMILA LOS VALORES DE CUANTIFICACIÓN:

    //A partir de:
    //      {F(theta___3)}.
    //Determina:
    //      {G(p___3), SB2, rbs},
    //      {theta___3min, theta___3max}.
    //      {Q(p___3)}
    //Restricciones:
    //      La función F(theta___3) debe ser monótona creciente.
    processateF();

    //ASIMILA LOS VALORES DE ORIENTACIÓN Y LOCACLIZACIÓN:

    //a partir de:
    //      {P__2, P__3}
    //      {Contour__, V__}
    //      {P1, theta2}
    //Determina:
    //      {P2, P3}
    //      {Contour, V},
    calculateImage();

    //RECUERDE: en el constructor de una clase abstracta
    //no se pude invocar a los métodos virtuales de la clase,
    //de modo que los métodos de asimilación no pueden ser abstractos.
}
//clona todas las propiedades de un brazo
void TArm::clone(TArm *Arm)
{
    //el puntero Arm debería apuntar a un brazo contruido
    if(Arm == NULL)
        throw EImproperArgument("pointer Arm should point to built arm");

    //copia las propiedades de plantilla de lectura/escritura
    L12 = Arm->getL12();
    P__2 = Arm->getP__2();
    L13 = Arm->getL13();
    theta__3 = Arm->gettheta__3();
    P__3 = Arm->getP__3();
    R3 = Arm->getR3();
    Contour____.Clone(Arm->getContour____());

    //copia las propiedades de plantilla de solo lectura
    P____2 = Arm->getP____2();
    P____1 = Arm->getP____1();
    Contour__.Clone(Arm->getContour__());
    L1V = Arm->getL1V();
    V__ = Arm->getV__();

    //copia las propiedades de cuanticiación delectura/escritura
    F.Clone(Arm->getF());
    Quantify___ = Arm->getQuantify___();
    Quantify___s.Clone(Arm->Quantify___s);

    //copia las propiedades de cuantificación de solo lectura
    G.Clone(Arm->getG());
    SB2 = Arm->getSB2();
    rbs = Arm->getrbs();
    Q.Clone(Arm->getQ());

    //copia las propiedades de orientación en radianes de lectura/escritura
    thetaO3 = Arm->getthetaO3();
    theta___3 = Arm->gettheta___3();
    theta___3min = Arm->gettheta___3min();
    theta___3max = Arm->gettheta___3max();
    theta___3s.Clone(Arm->theta___3s);

    //copia las propiedades de localización de lectura/escritura
    P1 = Arm->getP1();
    SPM = Arm->getSPM();

    //copia las propiedades de localización de solo lectura
    P2 = Arm->getP2();
    P3 = Arm->getP3();
    V = Arm->getV();
    Contour.Clone(Arm->getContour());
}
//contruye un clon de un brazo
TArm::TArm(TArm *Arm)
{
    //el puntero Arm debería apuntar a un brazo contruido
    if(Arm == NULL)
        throw EImproperArgument("pointer Arm should point to built arm");

    //copia las propiedades del brazo
    clone(Arm);
}
//libera la memoria dinámica
TArm::~TArm()
{
    Destroy(Contour);
    Destroy(Contour__);
    Destroy(Contour____);
}

//--------------------------------------------------------------------------
//MÉTODOS DE TRANSFORMACIÓN ENTRE SISTEMAS DE COORDENADAS:

//transforma la coordenada angular polar de S3 a S2
//      theta__ = theta___ + theta__O3
double TArm::S3thetaToS2theta(double theta___) const
{
    return theta___ + gettheta__O3();
}
//transforma las coordenadas polares en S2
//en coordenadas rectangulares en S2
TDoublePoint TArm::S2polToS2rec(double r__, double theta__) const
{
    //la distancia radial r debe ser no negativa
    if(r__ < 0)
        throw EImproperArgument("radial coordinate r should be nonnegative");

    TDoublePoint P__;
    P__.x = r__*cos(theta__);
    P__.y = r__*sin(theta__);
    return P__;
}

//--------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR LA PERTENENCIA A DOMINIOS DE ORIENTACIÓN:

//determina si un ángulo en radianes está fuera
//del intervalo [theta__3min, theta__3max]
bool TArm::isntInDomaintheta___3(double theta___3)
{
    //si theta___3 está fuera del intervalo [theta___3min, theta___3max]
    if(theta___3<gettheta___3min() || gettheta___3max()<theta___3)
        return true; //indica que no está en el dominio
    else //si no
        return false; //indica que si está en el dominio
}
//determina si un ángulo en radianes está fuera
//del intervalo [theta___2min, theta___2max]
//  IsntInDomaintheta___2(theta___2) =
//      IsntInDomaintheta___3(theta___2 + theta___23)
bool TArm::isntInDomaintheta___2(double theta___2)
{
    //traduce a theta___3 y aplica el método IsntInDomaintheta___3
    return isntInDomaintheta___3(theta___2 + theta__3);
}

//determina si un ángulo en pasos está fuera
//del intervalo [p___3min, p___3max]
//  IsntInDomainp___3(p___3) = IsntInDomaintheta___3(G(p___3))
bool TArm::isntInDomainp___3(double p___3)
{
    //traduce a theta___3 y aplica el método IsntInDomaintheta___3
    return isntInDomaintheta___3(getG().Image(p___3));
}

//--------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR VALORES CUANTIFICADOS DE ORIENTACIONES:

//cuantifica theta___3
double TArm::Qtheta___3(double theta___3)
{
    //comprueba las precondiciones
    if(theta___3<gettheta___3min() || gettheta___3max()<theta___3)
        throw EImproperArgument("orientation angle theta___3 should be in theta___3 domain");

    double p___3 = getF().Image(theta___3);
    p___3 = getQ().Quantifice(p___3);
    return getG().Image(p___3);
}
//cuantifica theta___2
double TArm::Qtheta___2(double theta___2)
{
    //el ángulo de orientación theta___2 debería estar en el dominio de theta___2
    if(theta___2<gettheta___2min() || gettheta___2max()<theta___2)
        throw EImproperArgument("orientation angle theta___2 should be in theta___2 domain");

    return Qtheta___3(theta___2 + theta__3) - theta__3;
}

//--------------------------------------------------------------------------
//MÉTODOS DE CONFIGURACIÓN:

//asigna conjuntamente las propiedades de plantilla
//si no espeficifa argumentos se asignan valores por defecto
void TArm::setTemplate(double t_L12, double t_L13, double t_theta__3, double t_R3)
{
    //comprueba las precondiciones
    if(t_L12 <= 0)
        throw EImproperArgument("length L12 should be upper zero");
    if(t_L13 <= 0)
        throw EImproperArgument("radial coordinate L13 should be nonnegative");
    if(t_R3 <= 0)
        throw EImproperArgument("radio R3 should be upper zero");

    //asigna los nuevos valores
    L12 = t_L12;
    L13 = t_L13;
    theta__3 = t_theta__3;
    R3 = t_R3;

    //asimila los parametros de plantilla
    processateTemplate();
    calculateImage();
}
//asigna conjuntamente las propiedades de orientación en radianes
void TArm::setOrientationRadians(double t_theta___3min, double t_theta___3max,
                                 double t_theta___3)
{
    //comprueba las precondiciones
    if(!F.BelongToDomain(t_theta___3min))
        throw EImproperArgument("angle theta___3min should be in domain F(theta___3)");
    if(!F.BelongToDomain(t_theta___3max))
        throw EImproperArgument("angle theta___3max should be in domain F(theta___3)");
    if(t_theta___3min > t_theta___3max)
        throw EImproperArgument("angle theta___3min should not be upper than angle theta___3max");
    if(isntInDomaintheta___3(t_theta___3))
        throw EImproperArgument("angle theta___3 should be in his domain [theta___3min, theta___3max]");

    //asignalos nuevos límites del intervalo
    theta___3min = t_theta___3min;
    theta___3max = t_theta___3max;

    //configura la función de cuantificación
    Q.Set(1, getp___3min(), getp___3max());

    //ASIGNA theta___3:

    //si la cuantificación está activada
    if(Quantify___)
        //cuantifica el nuevo valor
        t_theta___3 = Qtheta___3(t_theta___3);

    //ADVERTENCIA: la cuantificación podría dar lugar a que theta___3
    //quedara ligeramente fuera de su dominio.

    //constriñe theta___3 a su dominio
    if(t_theta___3 < theta___3min)
        t_theta___3 = theta___3min;
    else if(t_theta___3 > theta___3max)
        t_theta___3 = theta___3max;

    //si el nuevo valor difiere del actual
    if(t_theta___3 != theta___3)
        //asigna el nuevo valor
        theta___3 = t_theta___3;

    //ASIMILA thetaO3 y theta___3:

    //calcula la imagen correspondiente a la nueva posición y orientación
    calculateImage();
}
//asigna conjuntamente las propiedades de cuantificación
void TArm::setQuantification(double SB2)
{
    //DEFINE LA FUNCIÓN F A PARTIR DE SB2:

    //inicializa la función
    F.Clear();
    //añade los puntos de la función de compresión
    F.Add(-M_2PI, -double(SB2));
    F.Add(0., 0.);
    F.Add(M_2PI, double(SB2));
    F.Add(2*M_2PI, double(2*SB2));
    //elimina las incongruencias del formato texto
    //de la función de compansión
    F.setPointsText(getF().getPointsText());

    //asimila F
    processateF();
    calculateImage();
}

//obtiene el contorno del perímetro de seguridad del brazo
void TArm::getSecurityContour(TContourFigureList& securityContour)
{
    getContour().getSecurityContour(securityContour, getSPM());
}

//---------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR SI HAY COLISIÓN:

//determina la distancia mínima con una barrera
double TArm::distanceMin(const TBarrier *Barrier) const
{
    //comprueba las precondiciones
    if(Barrier == NULL)
        throw EImproperArgument("pointer Barrier should point to built barrier");

    //Una manera de aligerar el proceso sería incluir el código siguiente:
    //  //calcula la suma de SPMs
    //  double SPM = Barrier->getSPM() + getSPM();
    //  //si la distancia entre los ejes es al menos la distancia mínima para que no haya colisión
    //  if(Mod(Barrier->getP0() - getP1()) > Barrier->getr_max() + getL1V() + SPM + ERR_NUM)
    //      return DBL_MAX; //indica distancia infinita
    //Pero con este código las distancias mínimas calculadas resultan más confusas.

    //calcula la distancia mínima en cada sentido
    double distanceMin1 = getContour().distanceMin(Barrier->getContour());
    double distanceMin2 = Barrier->getContour().distanceMin(getContour());

    //devuelve la distancia mínima
    return min(distanceMin1, distanceMin2);
}
//determina la distancia mínima con un brazo
double TArm::distanceMin(const TArm *Arm) const
{
    //comprueba las precondiciones
    if(Arm == NULL)
        throw EImproperArgument("pointer Arm should point to built arm");

    //Una manera de aligerar el proceso sería incluir el código siguiente:
    //  //calcula el SPM conjunto
    //  double SPM = getSPM() + Arm->getSPM();
    //  //si la distancia entre los ejes es al menos la distancia mínima para que no haya colisión
    //  if(Mod(Arm->getP1() - getP1()) > Arm->getL1V() + getL1V() + SPM + ERR_NUM)
    //      return DBL_MAX; //indica distancia infinita
    //Pero con este código las distancias mínimas calculadas resultan más confusas.

    //calcula la distancia mínima en cada sentido
    double distanceMin1 = getContour().distanceMin(Arm->getContour());
    double distanceMin2 = Arm->getContour().distanceMin(getContour());

    //devuelve la distancia mínima
    return min(distanceMin1, distanceMin2);
}

//determina si hay colisión con una barrera
bool TArm::collides(const TBarrier *Barrier) const
{
    //comprueba las precondiciones
    if(Barrier == NULL)
        throw EImproperArgument("pointer Barrier should point to built barrier");

    //calcula la suma de SPMs
    double SPM = Barrier->getSPM() + getSPM();

    //si la distancia entre los ejes es al menos la distancia mínima para que no haya colisión
    if(Mod(Barrier->getP0() - getP1()) > Barrier->getr_max() + getL1V() + SPM + ERR_NUM)
        return false; //indica que no hay colisión

    //determina si hay colisión entre los contornos (en ambos sentidos)
    if(getContour().collides(Barrier->getContour(), SPM))
        return true;
    if(Barrier->getContour().collides(getContour(), SPM))
        return true;

    //indica que no ha encontrado colisión en ningún sentido
    return false;
}

//determina si hay colisión con un brazo
bool TArm::collides(const TArm *Arm) const
{
    //comprueba las precondiciones
    if(Arm == NULL)
        throw EImproperArgument("pointer Arm should point to built arm");

    //calcula la suma de SPMs
    double SPM = Arm->getSPM() + getSPM();

    //si la distancia entre los ejes es al menos la distancia mínima para que no haya colisión
    if(Mod(Arm->getP2() - getP1()) > Arm->getL1V() + getL1V() + SPM + ERR_NUM)
        return false; //indica que no hay colisión

    //determina si hay colisión entre los contornos (en ambos sentidos)
    if(getContour().collides(Arm->getContour(), SPM))
        return true;
    if(Arm->getContour().collides(getContour(), SPM))
        return true;

    //indica que no ha encontrado colisión en ningún sentido
    return false;
}

//determina si un punto está sobre el brazo
bool TArm::covers(TDoublePoint P) const
{
    //si la distancia entre el punto y el eje del brazo es mayor que L1V
    if(Mod(P - getP1()) > getL1V() + ERR_NUM)
        return false; //indica que no hay colisión

    //determina si el punto indicado está en el interior del contorno
    return getContour().isInner(P);
}

//---------------------------------------------------------------------------
//MÉTODOS PARA APILAR Y RECUPERAR ORIENTACIONES:

//apila theta___3 en la pila LIFO theta___3s
//  theta___3s.Add(theta___3)
void TArm::pushtheta___3(void)
{
    theta___3s.Add(gettheta___3());
}
//restaura el último theta___3 de la pila LIFO theta___3s
//  theta___3 = theta___3s.Last
//si no hay una posición apilada lanza EImproperCall
void TArm::restoretheta___3(void)
{
    //comprueba las precondiciones
    if(theta___3s.getCount() < 1)
        throw EImproperCall("stack theta___3s should contain some stacking value");

    //asigna el último valor apilado
    settheta___3(theta___3s.getLast());
}
//desempila el último theta___3 de la pila LIFO theta___3s
//  theta___3s.Delete(theta___3.Count - 1)
//si no hay una posición apilada lanza EImproperCall
void TArm::poptheta___3(void)
{
    //comprueba las precondiciones
    if(theta___3s.getCount() < 1)
        throw EImproperCall("stack theta___3s should contain some stacking value");

    //borra el último valor apilado
    theta___3s.Delete(theta___3s.getCount() - 1);
}
//restaura y desempila el último theta___3 de la pila LIFO theta___3s
//  theta___3s.Delete(theta___3.Count - 1)
//si no hay una posición apilada lanza EImproperCall
void TArm::restoreAndPoptheta___3(void)
{
    //comprueba las precondiciones
    if(theta___3s.getCount() < 1)
        throw EImproperCall("stack theta___3s should contain some stacking value");

    //asigna el último valor apilado
    settheta___3(theta___3s.getLast());
    //borra el último valor apilado
    theta___3s.Delete(theta___3s.getCount() - 1);
}

//---------------------------------------------------------------------------
//MÉTODOS PARA APILAR Y RECUPERAR ESTADOS DE CUANTIFICACIÓN:

//apila Quantify___ en la pila LIFO Quantify___s
//  Quantify___s.Add(Quantify___)
void TArm::pushQuantify___(void)
{
    Quantify___s.Add(getQuantify___());
}
//restaura el último Quantify___ de la pila LIFO Quantify___s
//  Quantify___ = Quantify___s.Last
//si no hay una posición apilada lanza EImproperCall
void TArm::restoreQuantify___(void)
{
    //comprueba las precondiciones
    if(Quantify___s.getCount() < 1)
        throw EImproperCall("stack Quantify___s should contain some stacking value");

    //asigna el último valor apilado
    setQuantify___(Quantify___s.getLast());
}
//desempila el último Quantify___ de la pila LIFO Quantify___s
//  Quantify___s.Delete(Quantify___.Count - 1)
//si no hay una posición apilada lanza EImproperCall
void TArm::popQuantify___(void)
{
    //comprueba las precondiciones
    if(Quantify___s.getCount() < 1)
        throw EImproperCall("stack Quantify___s should contain some stacking value");

    //borra el último valor apilado
    Quantify___s.Delete(Quantify___s.getCount() - 1);
}
//restaura y desempila el último Quantify___ de la pila LIFO Quantify___s
//  Quantify___s.Delete(Quantify___.Count - 1)
//si no hay una posición apilada lanza EImproperCall
void TArm::restoreAndPopQuantify___(void)
{
    //comprueba las precondiciones
    if(Quantify___s.getCount() < 1)
        throw EImproperCall("stack Quantify___s should contain some stacking value");

    //asigna el último valor apilado
    setQuantify___(Quantify___s.getLast());
    //borra el último valor apilado
    Quantify___s.Delete(Quantify___s.getCount() - 1);
}

//--------------------------------------------------------------------------
//MÉTODOS DE MOVIMIENTO:

//cambia la posición y orientación
//del origen de coordenadas simultaneamente
void TArm::set(TDoublePoint t_P1, double t_thetaO3)
{
    //si alguno de los nuevos valores difiere del actual
    if(t_P1 != P1 || t_thetaO3 != thetaO3) {
        //asigna los nuevos valores
        P1 = t_P1;
        thetaO3 = t_thetaO3;

        //asimila las propiedades de posición y orientación
        calculateImage();
    }
}

//Cambia simultaneamente:
//  la posición del brazo;
//  la orientación del del origen de coordenadas de S3;
//  la orientación del brazo respecto de su origen de coordenadas.
void TArm::set(TDoublePoint t_P1, double t_thetaO3, double t_theta___3)
{
    //comprueba las precondiciones
    if(isntInDomaintheta___3(t_theta___3))
        throw EImproperArgument("angle theta___3 should be in his domain [theta___3min, theta___3max]");

    //si la cuantificación está activada
    if(getQuantify___())
        //cuantifica el nuevo valor
        t_theta___3 = Qtheta___3(t_theta___3);

    //si alguno de los nuevos valores difiere del actual
    if(t_P1 != P1 || t_thetaO3 != thetaO3 || t_theta___3 != theta___3) {
        //asigna los nuevos valores
        P1 = t_P1;
        thetaO3 = t_thetaO3;
        theta___3 = t_theta___3;
        //asimila las propiedades de posición y orientación
        calculateImage();
    }
}

//genera un valor aleatorio con distribución uniforme en
//[max(0, Qmin), min(floor(SB2/2), Qmax)]
double TArm::randomp___3(void)
{
    return RandomUniform(max(0., getp___3min()), min(floor(getSB2()/2), getp___3max()));
}
//asigna a p___3 un valor aleatorio con distribución uniforme en
//[max(0, Qmin), min(floor(SB2/2), Qmax)]
void TArm::randomizep___3(void)
{
    setp___3(randomp___3());
}

//asigna a theta___3 el primer angulo
//estable menor o igual que el indicado
void TArm::settheta___3FirstStableLessOrEqual(double theta___3)
{
    //comprueba las precondiciones
    if(isntInDomaintheta___3(theta___3))
        throw EImproperArgument("angle theta___3 should be in [theta___3min, theta___3max]");

    //traduce a pasos
    double p___3 = getF().Image(theta___3);

    //asigna el primer entero menor o igual
    setp___3(floor(p___3));
}
//asigna a theta___2 el primer angulo
//estable menor o igual que el indicado
void TArm::settheta___2FirstStableLessOrEqual(double theta___2)
{
    //comprueba las precondiciones
    if(isntInDomaintheta___2(theta___2))
        throw EImproperArgument("angle theta___2 should be in [theta___2min, theta___2max]");

    //calcula theta___3 y lo asigna
    settheta___3FirstStableLessOrEqual(theta___2 + theta__3);
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
