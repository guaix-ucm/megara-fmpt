//---------------------------------------------------------------------------
//Archivo: Cilinder.cpp
//Contenido: cilindro de un actuador
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Cilinder.h"
#include "Strings.h"
#include "StrPR.h"
#include "TextFile.h"
#include "Geometry.h"

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TCilinder
//---------------------------------------------------------------------------
//PROPIEDADE DE DIMENSIONAMIENTO
//DE LECTURA/ESCRITURA:

void TCilinder::setL01(double _L01)
{
    //el radio del cilindro L01 debe ser mayor que cero
    if(_L01 <= 0)
        throw EImproperArgument("cilinder radio L01 should to be upper zero");

    __L01 = _L01; //asigna el nuevo valor

    //cambiar el radio del posicionador de fibra
    //cambiará la ubicación del brazo pero no su orientación

    //asigna al brazo la nueva posición
    getArm()->setP1(NewP1());
}

//PROPIEDADE DE DIMENSIONAMIENTO
//DE SOLO LECTURA:

//--------------------------------------------------------------------------
//PROPIEDADES DE ORIENTACIÓN EN RADIANES
//DE LECTURA/ESCRITURA:

void TCilinder::setthetaO1(double _thetaO1)
{
    //cambia la orientación del posicionador de fibra
    __thetaO1 = _thetaO1;

    //cambiar la orientación del posicionador de fibra
    //modifica la posición y orientación del origen de coordenadas del brazo

    //asigna al brazo la posición y orientación del origen de coordenadas
    getArm()->Set(NewP1(), getthetaO3());
    //asignaa la barrera la orientación del origen de coordenadas
    getBarrier()->setthetaO1(getthetaO1());
}

void TCilinder::settheta_1min(double _theta_1min)
{
    //el ángulo theta_1min debe estar en el dominio de definición de F(theta_1)
    if(!getF().BelongToDomain(_theta_1min))
        throw EImproperArgument("angle theta_1min should be in domain F(theta_1)");

    //el ángulo theta_1min debe ser menor que theta_1max
    if(_theta_1min >= gettheta_1max())
        throw EImproperArgument("angles theta_1min should be less theta_1max");

    __theta_1min = _theta_1min; //asigna el nuevo valor

    //asimila theta_1min
    __Q.setxmin(getp_1min()); //ajusta el dominio de cuantificación
    if(gettheta_1() < gettheta_1min()) //constriñe theta_1 a su dominio
        settheta_1(gettheta_1min());
}

void TCilinder::settheta_1max(double _theta_1max)
{
    //el ángulo theta_1max debe estar en el domaxio de definición de F(theta_1)
    if(!getF().BelongToDomain(_theta_1max))
        throw EImproperArgument("angle theta_1max should be in domain F(theta_1)");

    //el ángulo theta_1max debe ser mayor que theta_1min
    if(_theta_1max >= gettheta_1max())
        throw EImproperArgument("angles theta_1max should be upper theta_1min");

    __theta_1max = _theta_1max; //asigna el nuevo valor

    //asimila theta_1max
    __Q.setxmax(getp_1max()); //ajusta el dominio de cuantificación
    if(gettheta_1() > gettheta_1max()) //constriñe theta_1 a su dominio
        settheta_1(gettheta_1max());
}

void TCilinder::settheta_1(double _theta_1)
{
    //el ángulo theta_1 debe estar en [theta_1min, theta_1max]
    if(IsntInDomaintheta_1(_theta_1))
        throw EImproperArgument("angle theta_1 should be in [thata_1min, thata_1max]");

    //si la cuantificación está activada
    if(getQuantify_())
        //cuantifica el nuevo valor
        _theta_1 = Qtheta_1(_theta_1);

    //ADVERTENCIA: la cuantificación podría dar lugar a que theta_1
    //quedara ligeramente fuera de su dominio.

    //constriñe theta_1 a su dominio
    if(_theta_1 < gettheta_1min())
        _theta_1 = gettheta_1min();
    else if(_theta_1 > gettheta_1max())
        _theta_1 = gettheta_1max();

    //si el nuevo valor difiere del actual
    if(_theta_1 != gettheta_1()) {
        //asigna el nuevo valor
        __theta_1 = _theta_1;

        //cambiar la orientación del posicionador de fibra
        //modifica la posición y orientación del origen de coordenadas del brazo

        //asigna al brazo la posición y orientación
        //del origen de coordenadas
        getArm()->Set(NewP1(), getthetaO3());
    }
}

void TCilinder::settheta1(double _theta1)
{
    try {
        settheta_1(getthetaO1() - _theta1);
    } catch(...) {
        throw;
    }
}

void TCilinder::settheta_O3o(double _theta_O3o)
{
    //asigna el nuevo valor
    __theta_O3o = _theta_O3o;

    //asimila theta_O3
    getArm()->Set(NewP1(), getthetaO3());
}

//--------------------------------------------------------------------------
//PROPIEDADES DE CUANTIFICACIÓN
//DE LECTURA/ESCRITURA:

void TCilinder::setF(const TFunction &_F)
{
    //la función F(theta_1) debe estar definida en algún punto
    if(_F.getCount() <= 0)
        throw EImproperArgument("the function F should be defined in some point");

    //la función F(theta_1) debe ser monótona creciente
    if(_F.Monotony() != 1)
        throw EImproperArgument("function F(theta_1) should be monotically increasing");

    //F(theta___3) debe ser monótona creciente
    //para poder obtener la función inversa G(p___3).
    //Nótese que no se imponen las siguientes restricciones:
    //      La función F(theta___3) debe estar definida en 0.
    //      F(0) debería ser igual a cero.
    //      La función F(theta___3) debe estar definida en M_2PI.

    //asigna la función F
    __F = _F;

    //asimila F
    ProcessateF();
    settheta_1(gettheta_1());

    //En caso de que la cuantificación esté activada,
    //la asignación de theta_1 a sí mismo,
    //hará que se desplace a la posición estable más próxima
    //la cual será asimilada.
}

void TCilinder::setQuantify_(bool _Quantify_)
{
    //si activa la cuantificación
    if(!getQuantify_() && _Quantify_)
        //establece theta_1 en la posición estable más cercana
        settheta_1(gettheta_1());

    __Quantify_ = _Quantify_; //asigna el nuevo valor
}

//--------------------------------------------------------------------------
//PROPIEDADES ORIENTACIÓN EN PASOS
//DE SOLO LECTURA:

double TCilinder::getp_1min(void) const
{
    //devuelve theta_1min traducido a pasos
    return getF().Image(gettheta_1min());
}
void TCilinder::setp_1min(double _p_1min)
{
    //traduce a radianes
    double _theta_1min = getG().Image(_p_1min);

    //el ángulo p_1min debe estar en el dominio imagen de F(theta_1)
    if(!getF().BelongToDomain(_theta_1min))
        throw EImproperArgument("angle p_1min should be in image domain F(theta_1)");

    //el ángulo p_1min no debe ser mayor que el ángulo p_1max
    if(_theta_1min > gettheta_1max())
        throw EImproperArgument("angle p_1min should not be upper than angle p_1max");

    __theta_1min = _theta_1min; //asigna el nuevo valor

    //asimila theta_1min
    __Q.setxmin(getp_1min());
    if(gettheta_1() < gettheta_1min())
        settheta_1(gettheta_1min());

    //la asignación a theta_1 implicaría su asimilación
}
double TCilinder::getp_1max(void) const
{
    //devuelve theta_1max traducido a pasos
    return getF().Image(gettheta_1max());
}
void TCilinder::setp_1max(double _p_1max)
{
    //traduce a radianes
    double _theta_1max = getG().Image(_p_1max);

    //el ángulo p_1max debe estar en el domaxio imagen de F(theta_1)
    if(!getF().BelongToDomain(_theta_1max))
        throw EImproperArgument("angle p_1max should be in image domain F(theta_1)");

    //el ángulo p_1max no debe ser menor que el ángulo p_1max
    if(_theta_1max < gettheta_1max())
        throw EImproperArgument("angle p_1max should not be less than angle p_1max");

    __theta_1max = _theta_1max; //asigna el nuevo valor

    //asimila theta_1max
    __Q.setxmax(getp_1max());
    if(gettheta_1() > gettheta_1max())
        settheta_1(gettheta_1max());

    //la asignación a theta_1 implicaría su asimilación
}
double TCilinder::getp_1(void) const
{
    //si la cuantificación está activada
    if(getQuantify_())
        //devuelve el valor en pasos cuantificado
        return getQ().Quantifice(getF().Image(gettheta_1()));
    else //si no
        //devuelve el valor en pasos sin cuantificar
        return getF().Image(gettheta_1());

    //En lectura p_1 se cuantifica para evitar el error numérico
    //introducido por la función F(theta_1).
}
void TCilinder::setp_1(double _p_1)
{
    //si la cuantificación está activada
    if(getQuantify_())
        //cuantifica el valor, lo traduce a radianes y lo asigna
        __theta_1 = getG().Image(getQ().Quantifice(_p_1));
    else //si no
        //traduce el valor a radianes y lo asigna
        __theta_1 = getG().Image(_p_1);

    //cambiar la orientación del posicionador de fibra
    //modifica la posición y orientación
    //del origen de coordenadas del brazo

    //asimila theta_1:

    //asigna la posición y orientación del origen de coordenadas del brazo
    getArm()->Set(NewP1(), getthetaO3());
}

//--------------------------------------------------------------------------
//PROPIEDADES DE LOCALIZACIÓN
//DE LECTURA/ESCRITURA:

void TCilinder::setP0(TDoublePoint _P0)
{
    __P0 = _P0; //asigna el nuevo valor

    //desplazar el posicionador cambiará la ubicación del brazo
    //pero no su orientación

    //asimila P0:
    /*        __L0 = P0.Mod();
        if(L0 != 0)
                __theta0 = P0.Arg();
        else
                __theta0 = 0;*/
    getArm()->setP1(NewP1()); //mueve el brazo a la nueva posición
    getBarrier()->setP0(getP0()); //mueve la barrera a la nueva posición
}

double TCilinder::getL0(void) const
{
    return getP0().Mod();
}
void TCilinder::setL0(double _L0)
{
    //la coordenada radial L0 debería ser no negativa
    if(_L0 < 0)
        throw EImproperArgument("radial coordinate L0 should be nonnegative");

    //        __L0 = _L0; //asigna el nuevo valor

    //desplazar el posicionador cambiará la ubicación del brazo
    //pero no su orientación

    //asimila L0
    __P0.x = _L0+cos(gettheta0());
    __P0.y = _L0+sin(gettheta0());
    getArm()->setP1(NewP1()); //mueve el brazo a la nueva posición
    getBarrier()->setP0(getP0()); //mueve la barrera a la nueva posición
}
double TCilinder::gettheta0(void) const
{
    if(getL0() != 0)
        return getP0().Arg();

    return 0;
}
void TCilinder::settheta0(double _theta0)
{
    //Nótese que theta0 admite cualquier valor.

    //        __theta0 = _theta0; //asigna el nuevo valor

    //desplazar el posicionador cambiará la ubicación del brazo
    //pero no su orientación

    //asimila L0
    __P0.x = getL0()+cos(_theta0);
    __P0.y = getL0()+sin(_theta0);
    getArm()->setP1(NewP1()); //mueve el brazo a la nueva posición
    getBarrier()->setP0(getP0()); //mueve la barrera a la nueva posición
}

//PROPIEDADES DE LOCALIZACIÓN
//DE SOLO LECTURA:

double TCilinder::getr_2(void) const
{
    //calcula r_2 mediante la fórmula del coseno
    return sqrt(getL01()*getL01() + getArm()->getL12()*getArm()->getL12() -
                2*getL01()*getArm()->getL12()*
                cos(M_PI - gettheta_O3o() + getArm()->gettheta___2()));

    //Sean a, b y c la longirud de los lados de un triángulo,
    //se demuestra que a^2 + b^2 - 2*a*b*cost(theta) >= 0
    //del siguiente modo: manteniendo constante a^2 + b^2,
    //a*b es máximo cuando a=b, en cuyo caso se verifica que:
    //      2ab = a^2 + b^2;
    //en los demás casos se tiene:
    //      2ab < a^2 + b^2.
    //De modo que el radicando de la función sqrt nunca será negativo.

    //En lugar de (M_PI - thetaO3o + Arm->theta___2) podría ponerse
    //(Arm->theta2 - (theta1 - M_PI)) donde (theta1 - M_PI) representa
    //la orientación del sistema de coordenadas intermedio en S0 (thetaO3o).

    //Nótese que:
    //Arm->theta2 = thetaO3 + theta___2 =
    //              thehaO1 - theta_1 - theta_O3o + Arm->theta___2;
    //     theta1 = thetaO1 - theta_1;
    //Arm->theta2 - (theta1 - M_PI) = M_PI - theta_O3o + Arm->theta___2;
}
double TCilinder::gettheta_12(void) const
{
    double den = 2*getL01()*getr_2();

    if(den != 0) //si no está en el orig
        //al ángulo del eje 1 se le suma un ángulo que
        //puede calcularse por la fórmula del coseno
        return acos(Min(1., (getL01()*getL01() + getr_2()*getr_2() -
                             getArm()->getL12()*getArm()->getL12())/den));
    else //si está en el origen
        //la dirección radial es normal a la orientación del eje 1 (en S1)
        return M_PI/2;

    //Sean a, b y c la longitud de los lados de un triángulo,
    //la demostración de que: (a^2 + b^2 - c^2)/(2*a*b)
    //se mantiene en el intervalo [-1, 1],
    //es la misma que la de la fórmula del coseno.
    //De modo que el argumento de la función acos nunca
    //se saldrá del intervalo [-1, 1].
}
double TCilinder::gettheta_2(void) const
{
    return gettheta_1() + gettheta_12();
}

double TCilinder::getx_2(void) const
{
    return getr_2()*cos(gettheta_2());
}
double TCilinder::gety_2(void) const
{
    return getr_2()*sin(gettheta_2());
}
TDoublePoint TCilinder::getP_2(void) const
{
    TDoublePoint P_2;
    P_2.x = getx_2();
    P_2.y = gety_2();
    return P_2;
}

double TCilinder::getr_3(void) const
{
    return sqrt(getL01()*getL01() + getArm()->getL13()*getArm()->getL13() -
                2*getL01()*getArm()->getL13()*
                cos(M_PI - gettheta_O3o() + getArm()->gettheta___3()));

    //Sean a, b y c las longitudes de los lados de un triángulo,
    //se demuestra que a^2 + b^2 - 2*a*b*cost(theta) >= 0
    //del siguiente modo: manteniendo constante a^2 + b^2,
    //a*b es máximo cuando a=b, en cuyo caso se verifica que:
    //      2ab = a^2 + b^2;
    //en los demás casos se tiene:
    //      2ab < a^2 + b^2.
    //De modo que el radicando de la función sqrt nunca será negativo.

    //En lugar de (M_PI - thetaO3o + theta___3) podría ponerse
    //(Arm->theta3 - (theta1 - M_PI)) donde (theta1 - M_PI) representa
    //la orientación del sistema de coordenadas intermedio en S0 (thetaO3o).

    //Nótese que:
    //Arm->theta3 = thetaO3 + theta___3 =
    //              thehaO1 - theta_1 - theta_O3o + Arm->theta___3;
    //     theta1 = thetaO1 - theta_1;
    //Arm->theta3 - (theta1 - M_PI) = M_PI - theta_O3o + Arm->theta___3;
}
double TCilinder::gettheta_13(void) const
{
    double den = 2*getL01()*getr_3();

    if(den != 0) //si no está en el orig
        //al ángulo del eje 1 se le suma un ángulo que
        //puede calcularse por la fórmula del coseno
        return acos(Min(1., (getL01()*getL01() + getr_3()*getr_3() -
                             getArm()->getL13()*getArm()->getL13())/den));
    else //si está en el origen
        //la dirección radial es normal a la orientación del eje 1 (en S1)
        return M_PI/2;

    //Sean a, b y c la longitud de los lados de un triángulo,
    //la demostración de que: (a^2 + b^2 - c^2)/(2*a*b)
    //se mantiene en el intervalo [-1, 1],
    //es la misma que la de la fórmula del coseno.
    //De modo que el argumento de la función acos nunca
    //se saldrá del intervalo [1, 1].
}
double TCilinder::gettheta_3(void) const
{
    return gettheta_1() + gettheta_13();
}

double TCilinder::getx_3(void) const
{
    return getr_3()*cos(gettheta_3());
}
double TCilinder::gety_3(void) const
{
    return getr_3()*sin(gettheta_3());
}
TDoublePoint TCilinder::getP_3(void) const
{
    return TDoublePoint(getx_3(), gety_3());
}

TDoublePoint TCilinder::getP3o(void) const
{
    //Calcula P3 en S1 cuando theta___3 == 0, por el mismo procedimiento
    //empeado cuando theta___3 no es necesariamente 0, y traduce
    //las coordenadas de S1 a S0:

    double r_3 = sqrt(getL01()*getL01() + getArm()->getL13()*getArm()->getL13() -
                      2*getL01()*getArm()->getL13()*cos(M_PI - gettheta_O3o()));
    double theta_3 = gettheta_1() + gettheta_13();
    double x_3o = r_3*cos(theta_3);
    double y_3o = r_3*sin(theta_3);

    /*
        //en caso de igualdad devuelve directamente P0 para evitar
        //introducir errores numéricos
        if(theta_O3==M_PI && Arm->L13==L01)
                return P0;

        //determina la coordenada angular del punto P3o en S1:
        double theta_3o;
        if(Arm->L13 < L01)
                theta_3o = 0;
        else //Arm->L13 > L01
                theta_3o = -M_PI;
        //la coordenada radial viene dada por r_3min

        //traduce el punto a coordenadas cartesianas
        double x_3o = r_3min*cos(S0thetaToS1theta(theta_3o));
        double y_3o = r_3min*sin(S0thetaToS1theta(theta_3o));
*/
    //traduce a S0
    TDoublePoint P3o = S1recToS0rec(x_3o, y_3o);

    return P3o; //devuelve el punto en S0*/
}

//##########################################################################
//PROPIEDADES EN FORMATO TEXTO:
//##########################################################################

//--------------------------------------------------------------------------
//PROPIEDADES DE DIMENSIONAMIENTO
//DE LECTURA/ESCRITURA EN FORMATO TEXTO:

AnsiString TCilinder::getL01Text(void) const
{
    return FloatToStr(getL01());
}
void TCilinder::setL01Text(AnsiString &S)
{
    try {
        setL01(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

//PROPIEDADES DE DIMENSIONAMIENTO
//DE SOLO LECTURA EN FORMATO TEXTO:

AnsiString TCilinder::getL02maxText(void) const
{
    return FloatToStr(getL02max());
}
AnsiString TCilinder::getL03maxText(void) const
{
    return FloatToStr(getL03max());
}
AnsiString TCilinder::getr_maxText(void) const
{
    return FloatToStr(getr_max());
}

AnsiString TCilinder::getr_3minText(void) const
{
    return FloatToStr(getr_3min());
}
AnsiString TCilinder::getr_3maxText(void) const
{
    return FloatToStr(getr_3max());
}

AnsiString TCilinder::getx3minText(void) const
{
    return FloatToStr(getx3min());
}
AnsiString TCilinder::getx3maxText(void) const
{
    return FloatToStr(getx3max());
}

AnsiString TCilinder::gety3minText(void) const
{
    return FloatToStr(gety3min());
}
AnsiString TCilinder::gety3maxText(void) const
{
    return FloatToStr(gety3max());
}

//--------------------------------------------------------------------------
//PROPIEDADES DE ORIENTACIÓN EN RADIANES
//DE LECTURA/ESCRITURA EN FORMATO TEXTO:

AnsiString TCilinder::getthetaO1Text(void) const
{
    return FloatToStr(getthetaO1());
}
void TCilinder::setthetaO1Text(AnsiString &S)
{
    try {
        setthetaO1(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

AnsiString TCilinder::gettheta_1Text(void) const
{
    return FloatToStr(gettheta_1());
}
void TCilinder::settheta_1Text(AnsiString &S)
{
    try {
        settheta_1(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TCilinder::gettheta_1minText(void) const
{
    return FloatToStr(gettheta_1min());
}
void TCilinder::settheta_1minText(AnsiString &S)
{
    try {
        settheta_1min(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TCilinder::gettheta_1maxText(void) const
{
    return FloatToStr(gettheta_1max());
}
void TCilinder::settheta_1maxText(AnsiString &S)
{
    try {
        settheta_1max(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

AnsiString TCilinder::gettheta1Text(void) const
{
    return FloatToStr(gettheta1());
}
void TCilinder::settheta1Text(AnsiString &S)
{
    try {
        settheta1(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

AnsiString TCilinder::gettheta_O3oText(void) const
{
    return FloatToStr(gettheta_O3o());
}
void TCilinder::settheta_O3oText(const AnsiString &S)
{
    try {
        settheta_O3o(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

//PROPIEDADES DE ORIENTACIÓN EN RADIANES
//DE SOLO LECTURA EN FORMATO TEXTO:

AnsiString TCilinder::getthetaO3Text(void) const
{
    return FloatToStr(getthetaO3());
}

//--------------------------------------------------------------------------
//PROPIEDADES DE CUANTIFICACIÓN
//DE LECTURA/ESCRITURA EN FORMATO TEXTO:

AnsiString TCilinder::getFAddressText(void) const
{
    return IntToHex(reinterpret_cast<intptr_t>(&__F), 8);
}
void TCilinder::setFPointsText(const AnsiString &S)
{
    try {
        //construye una función tampón
        TFunction _F;
        //asigna el nuevo valor a la función tampón
        _F.setPointsText(S);
        //intenta asignar la función tampón a la función
        setF(_F);

        //La asignación a F provocará la asimilación de F mediante:
        //      ProcessateF();

        //No olvidar que la cadena S no no debe asignarse directamente
        //a F.PointsText, por que el método SetF comprubea si F
        //cumple las restricciones.

    } catch(...) {
        throw;
    }
}
void TCilinder::setFTableText(const AnsiString &S)
{
    try {
        //clona la función F
        TFunction _F(__F);
        //asigna el nuevo valor al clon
        _F.setTableText(S);
        //intenta asignar el clon a la función
        setF(_F);

        //La asignación a F provocará la asimilación de F mediante:
        //      CalculateG();
        //      CalculateQ();
        //      theta_1 = theta_1;

        //No olvidar que la cadena S no no debe asignarse directamente
        //a F.PointsText, por que el método SetF comprubea si F
        //cumple las restricciones.
    } catch(...) {
        throw;
    }
}

AnsiString TCilinder::getQuantify_Text(void) const
{
    return BoolToStr_(getQuantify_(), true);
}
void TCilinder::setQuantify_Text(AnsiString &S)
{
    try {
        setQuantify_(StrToBool_(S));
    } catch(...) {
        throw;
    }
}

//PROPIEDADES DE CUANTIFICACIÓN
//DE SOLO LECTURA EN FORMATO TEXTO:

AnsiString TCilinder::getGAddressText(void) const
{
    return IntToHex(reinterpret_cast<intptr_t>(&__G), 8);
}

AnsiString TCilinder::getSB1Text(void) const
{
    return FloatToStr(getSB1());
}
AnsiString TCilinder::getrbsText(void) const
{
    return FloatToStr(getrbs());
}

//--------------------------------------------------------------------------
//PROPIEDADES DE ORIENTACIÓN EN PASOS
//DE SOLO LECTURA EN FORMATO TEXTO:

AnsiString TCilinder::getp_1minText(void) const
{
    return FloatToStr(getp_1min());
}
void TCilinder::setp_1minText(AnsiString &S)
{
    try {
        setp_1min(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TCilinder::getp_1maxText(void) const
{
    return FloatToStr(getp_1max());
}
void TCilinder::setp_1maxText(AnsiString &S)
{
    try {
        setp_1max(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TCilinder::getp_1Text(void) const
{
    return FloatToStr(getp_1());
}
void TCilinder::setp_1Text(AnsiString &S)
{
    try {
        setp_1(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

//--------------------------------------------------------------------------
//PROPIEDADES DE LOCALIZACIÓN
//DE LECTURA/ESCRITURA EN FORMATO TEXTO:

AnsiString TCilinder::getP0Text(void) const
{
    return DPointToStr(getP0());
}
void TCilinder::setP0Text(const AnsiString &S)
{
    try {
        setP0(StrToDPoint(S));
    } catch(...) {
        throw;
    }
}

AnsiString TCilinder::getL0Text(void) const
{
    return FloatToStr(getL0());
}
void TCilinder::setL0Text(const AnsiString &S)
{
    try {
        setL0(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TCilinder::gettheta0Text(void) const
{
    return FloatToStr(gettheta0());
}
void TCilinder::settheta0Text(const AnsiString &S)
{
    try {
        settheta0(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

//PROPIEDADES DE LOCALIZACIÓN
//DE SOLO LECTURA EN FORMATO TEXTO:

AnsiString TCilinder::getrmaxText(void) const
{
    return FloatToStr(getrmax());
}

AnsiString TCilinder::getr_2Text(void) const
{
    return FloatToStr(getr_2());
}
AnsiString TCilinder::gettheta_12Text(void) const
{
    return FloatToStr(gettheta_12());
}
AnsiString TCilinder::gettheta_2Text(void) const
{
    return FloatToStr(gettheta_2());
}

AnsiString TCilinder::getx_2Text(void) const
{
    return FloatToStr(getx_2());
}
AnsiString TCilinder::gety_2Text(void) const
{
    return FloatToStr(gety_2());
}
AnsiString TCilinder::getP_2Text(void) const
{
    return DPointToStr(getP_2());
}

AnsiString TCilinder::getr_3Text(void) const
{
    return FloatToStr(getr_3());
}
AnsiString TCilinder::gettheta_13Text(void) const
{
    return FloatToStr(gettheta_13());
}
AnsiString TCilinder::gettheta_3Text(void) const
{
    return FloatToStr(gettheta_3());
}

AnsiString TCilinder::getx_3Text(void) const
{
    return FloatToStr(getx_3());
}
AnsiString TCilinder::gety_3Text(void) const
{
    return FloatToStr(gety_3());
}
AnsiString TCilinder::getP_3Text(void) const
{
    return DPointToStr(getP_3());
}

AnsiString TCilinder::getP3oText(void) const
{
    return DPointToStr(getP3o());
}

//--------------------------------------------------------------------------
//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TCilinder::getSizingText(void) const
{
    AnsiString S;

    //PROPIEDADES DE DIMENSIONAMIENTO:

    S += "R/W:\r\n";

    S += AnsiString("    L01 = ")+getL01Text()+AnsiString("\r\n");

    //PROPIEDADES DE DIMENSIONAMIENTO DERIVADAS:

    S += "R:\r\n";

    S += AnsiString("    L02max: ")+getL02maxText()+AnsiString("\r\n");
    S += AnsiString("    L03max: ")+getL03maxText()+AnsiString("\r\n");
    S += AnsiString("    r_max: ")+getr_maxText()+AnsiString("\r\n");

    S += AnsiString("    r_3min: ")+getr_3minText()+AnsiString("\r\n");
    S += AnsiString("    r_3max: ")+getr_3maxText()+AnsiString("\r\n");

    S += AnsiString("    x3min: ")+getx3minText()+AnsiString("\r\n");
    S += AnsiString("    x3max: ")+getx3maxText()+AnsiString("\r\n");

    S += AnsiString("    y3min: ")+gety3minText()+AnsiString("\r\n");
    S += AnsiString("    y3max: ")+gety3maxText();

    return S;
}
AnsiString TCilinder::getOrientationRadiansText(void) const
{
    AnsiString S;

    S += "R/W:\r\n";

    S += AnsiString("    thetaO1 = ")+getthetaO1Text()+AnsiString("\r\n");
    S += AnsiString("    theta_1min = ")+gettheta_1minText()+AnsiString("\r\n");
    S += AnsiString("    theta_1max = ")+gettheta_1maxText()+AnsiString("\r\n");
    S += AnsiString("    theta_1 = ")+gettheta_1Text()+AnsiString("\r\n");
    S += AnsiString("    theta_1s = ")+theta_1s.getText()+AnsiString("\r\n");
    S += AnsiString("    theta1: ")+gettheta1Text()+AnsiString("\r\n");
    S += AnsiString("    theta_O3o: ")+gettheta_O3oText()+AnsiString("\r\n");

    S += "R:\r\n";

    S += AnsiString("    thetaO3: ")+getthetaO3Text();

    return S;
}
AnsiString TCilinder::getQuantificationText(void) const
{
    AnsiString S;

    S += "R/W:\r\n";

    S += AnsiString("    FAddress = ")+getFAddressText()+AnsiString("\r\n");
    S += AnsiString("    Quantify_ = ")+getQuantify_Text()+AnsiString("\r\n");
    S += AnsiString("    Quantify_s = ")+Quantify_s.getText()+AnsiString("\r\n");

    S += "R:\r\n";

    S += AnsiString("    GAddress: ")+getGAddressText()+AnsiString("\r\n");
    S += AnsiString("    SB1: ")+getSB1Text()+AnsiString("\r\n");
    S += AnsiString("    rbs: ")+getrbsText()+AnsiString("\r\n");
    S += AnsiString("    Q:\r\n")+StrIndent(getQ().getAssignsText(), "        ");

    return S;
}
AnsiString TCilinder::getOrientationStepsText(void) const
{
    AnsiString S;

    S += "R/W:\r\n";

    S += AnsiString("    p_1min = ")+getp_1minText()+AnsiString("\r\n");
    S += AnsiString("    p_1max = ")+getp_1maxText()+AnsiString("\r\n");
    S += AnsiString("    p_1 = ")+getp_1Text();

    return S;
}
AnsiString TCilinder::getLocationText(void) const
{
    AnsiString S;

    //PARÁMETROS DE POSICIÓN:

    S += "R/W:\r\n";

    S += AnsiString("    P0 = ")+getP0Text()+AnsiString("\r\n");
    S += AnsiString("    L0 = ")+getL0Text()+AnsiString("\r\n");
    S += AnsiString("    theta0 = ")+gettheta0Text()+AnsiString("\r\n");

    S += "R:\r\n";

    S += AnsiString("    rmax = ")+getrmaxText()+AnsiString("\r\n");

    S += AnsiString("    r_2 = ")+getr_2Text()+AnsiString("\r\n");
    S += AnsiString("    theta_12 = ")+gettheta_12Text()+AnsiString("\r\n");
    S += AnsiString("    theta_2 = ")+gettheta_2Text()+AnsiString("\r\n");

    S += AnsiString("    x_2 = ")+getx_2Text()+AnsiString("\r\n");
    S += AnsiString("    y_2 = ")+gety_2Text()+AnsiString("\r\n");
    S += AnsiString("    P_2 = ")+getP_2Text()+AnsiString("\r\n");

    S += AnsiString("    r_3 = ")+getr_3Text()+AnsiString("\r\n");
    S += AnsiString("    theta_13 = ")+gettheta_13Text()+AnsiString("\r\n");
    S += AnsiString("    theta_3 = ")+gettheta_3Text()+AnsiString("\r\n");

    S += AnsiString("    x_3 = ")+getx_3Text()+AnsiString("\r\n");
    S += AnsiString("    y_3 = ")+gety_3Text()+AnsiString("\r\n");
    S += AnsiString("    P_3 = ")+getP_3Text()+AnsiString("\r\n");

    S += AnsiString("    P3o = ")+getP3oText();

    return S;
}

AnsiString TCilinder::getCilinderText(void) const
{
    AnsiString S;

    S += "Sizing:\r\n";
    S += StrIndent(getSizingText())+AnsiString("\r\n");
    S += "OrientationRadians:\r\n";
    S += StrIndent(getOrientationRadiansText())+AnsiString("\r\n");
    S += "Quantification:\r\n";
    S += StrIndent(getQuantificationText())+AnsiString("\r\n");
    S += "OrientationSteps:\r\n";
    S += StrIndent(getOrientationStepsText())+AnsiString("\r\n");
    S += "Location:\r\n";
    S += StrIndent(getLocationText())+AnsiString("\r\n");
    S += AnsiString("ArmAddress: ")+getArmAddressText()+AnsiString("\r\n");
    S += AnsiString("BarrierAddress: ")+getBarrierAddressText();

    return S;
}

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
void TCilinder::ProcessateF(void)
{
    try {
        //determina la función G (inversa de F)
        __G.Inverse(getF());
    } catch(...) {
        throw;
    }

    //ADVERTENCIA: esposible que F no esté definida en 0 y/o en M_2PI.

    //selecciona el intervalo a partir del cual va a calcular
    //el número de pasos de una vuelta completa
    double xmax = Min(M_2PI, getF().getXLast());
    double xmin = Max(double(0), getF().getXFirst());
    //calcula el número de pasos de una vuelta completa
    __SB1 = (getF().Image(xmax) - getF().Image(xmin))/(xmax - xmin)*M_2PI;

    //calcula el escalón de cuantificación promedio en [0, M_2PI]
    __rbs = M_2PI/getSB1();

    //si el dominio de F no contiene todo [theta_1min, theta_1max]
//    if(gettheta_1min()<getF().getXFirst() || getF().getXLast()<gettheta_1max()) {
        //constriñe los límites del dominio de theta_1 según el dominio de F
        if(gettheta_1min() < getF().getXFirst())
            __theta_1min = getF().getXFirst();
        if(gettheta_1max() > getF().getXLast())
            __theta_1max = getF().getXLast();
//    }

    //asimila [theta_1min, theta_1max]
    __Q.Set(1, getp_1min(), getp_1max());

    //constriñe theta_1 a su dominio
    //y/o lo cuantifica en su caso
    if(gettheta_1() < gettheta_1min())
        settheta_1(gettheta_1min());
    else if(gettheta_1() > gettheta_1max())
        settheta_1(gettheta_1max());
    else
        settheta_1(gettheta_1());
}

//calcula P1 según (P0, theta1)
TDoublePoint TCilinder::NewP1(void)
{
    TDoublePoint _P1;
    _P1.x = getP0().x + getL01()*cos(getthetaO1() - gettheta_1());
    _P1.y = getP0().y + getL01()*sin(getthetaO1() - gettheta_1());

    return _P1;
}

//##########################################################################
//MÉTODOS PÚBLICOS:
//##########################################################################

//---------------------------------------------------------------------------
//MÉTODOS DE CONTRUCCION, COPIA Y DESTRUCCIÓN:

//construye un actuador de fibra
//con la posición y orientación indicadas
TCilinder::TCilinder(TDoublePoint _P0, double _thetaO1) :
    __Q(),
    theta_1s(), Quantify_s(2, NULL, NULL, NULL, StrPrintBool)
{
    //inicializa las propiedades de dimensionamiento
    __L01 = MEGARA_L;

    //inicializa las propiedades de orientación en radianes
    __thetaO1 = _thetaO1;
    __theta_1min = MEGARA_theta_1min;
    __theta_1max = MEGARA_theta_1max;
    __theta_1 = 0;
    __theta_O3o = M_PI;

    //Para poder cuantificar theta___3 debe determinarse antes:
    //      {[theta___3min, theta___3max], F} --> {Q(p___3)}
    //Pero si theta___3 está fuera de [theta___3min,theta___3max]
    //será arrastrado, pudiendo ser cuantificado por Q.
    //Por eso theta___3 dbe inicializarse dentro de su dominio.

    //INICIALIZA LAS PROPIEDADES DE CUANTIFICACIÓN:

    //añade los puntos de la función de compresión
    __F.Add(-M_2PI, -double(MEGARA_SB2));
    __F.Add(0., 0.);
    __F.Add(M_2PI, double(MEGARA_SB2));
    __F.Add(2*M_2PI, double(2*MEGARA_SB2));
    //elimina las incongruencias del formato texto
    //de la función de compansión
    __F.setPointsText(getF().getPointsText());

    //F debe estar definida en el intervalo [-M_2PI, M_2PI]
    //para permitir el ajuste olgado de [theta___3min, theta___3max].

    //inicializa las propiedades de cuantificación
    __Quantify_ = true;

    //inicializa las porpiedades de posición y orientación
    __P0 = _P0;
    /*        __L0 = Mod(P0);
        if(L0 > 0)
                __theta0 = ArgPos(P0);
        else
                __theta0 = 0;*/

    //------------------------------------------------------------------
    //ASIMILA LOS VALORES DE INICIALIZACIÓN:

    //A partir de:
    //      {F(theta_1)}.
    //Determina:
    //      {G(p_1), SB1, rbs},
    //      {theta_1min, theta_1max}.
    //      {Q(p_1)}
    //Restricciones:
    //      La función F(theta_1) debe ser monótona creciente.
    ProcessateF();

    //ADVERTENCIA: se necesita conocer L01 para calcular NewP1.

    //construye el brazo del posicionador de fibra con sus propiedades por defecto
    __Arm = new TArm(NewP1(), getthetaO1() - M_PI);

    //construye la barrera del área de exclusión con sus propiedades por defecto
    __Barrier = new TBarrier(getP0(), getthetaO1());
    /*        //asigna el valor por defecto a la barrera del área de exclusión
        TContourFigureList _Contour_;
        TArc *Arc = new TArc(TDoublePoint(r_max, 0), TDoublePoint(-r_max, 0), TDoublePoint(0, 0), r_max);
        _Contour_.Add((TContourFigure*)Arc);
        Arc = new TArc(TDoublePoint(-r_max, 0), TDoublePoint(r_max, 0), TDoublePoint(0, 0), r_max);
        _Contour_.Add((TContourFigure*)Arc);
        Barrier->Contour_Text = _Contour_.Text;
        Destroy(_Contour_);*/
}

//copia un conjunto de propiedades de un cilindro
void TCilinder::CopySizing(const TCilinder *C)
{
    //el puntero C debería apuntar a un cilindro construído
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built cilinder");

    //copia las propiedades
    __L01 = C->__L01;
}
void TCilinder::CopyOrientationRadians(const TCilinder *C)
{
    //el puntero C debería apuntar a un cilindro construído
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built cilinder");

    //copia las propiedades
    __thetaO1 = C->__thetaO1;
    __theta_1min = C->__theta_1min;
    __theta_1max = C->__theta_1max;
    __theta_1 = C->__theta_1;
    __theta_O3o = C->__theta_O3o;
}
void TCilinder::CopyQuantification(const TCilinder *C)
{
    //el puntero C debería apuntar a un cilindro construído
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built cilinder");

    //copia las propiedades
    __F.Clone(C->__F);
    __Quantify_ = C->__Quantify_;
    Quantify_s.Clone(C->Quantify_s);
    __G.Copy(C->__G);
    __SB1 = C->__SB1;
    __rbs = C->__rbs;
    __Q = C->__Q;
}
void TCilinder::CopyLocation(const TCilinder *C)
{
    //el puntero C debería apuntar a un cilindro construído
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built cilinder");

    //copia las propiedades
    __P0 = C->__P0;
    /*        __L0 = C->__L0;
        __theta0 = C->__theta0;*/
}
void TCilinder::CopyCilinder(const TCilinder *C)
{
    //el puntero C debería apuntar a un cilindro construído
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built cilinder");

    //copia las propiedades
    CopySizing(C);
    CopyOrientationRadians(C);
    CopyQuantification(C);
    CopyLocation(C);
    __Arm->Copy(C->__Arm);
    __Barrier->Copy(C->__Barrier);
}

//construye un clon de un cilindro
TCilinder::TCilinder(const TCilinder *C)
{
    //el puntero C debería apuntar a un cilindro construído
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built cilinder");

    //construye el brazo del posicionador de fibra con sus propiedades por defecto
    __Arm = new TArm(NewP1(), getthetaO1() - M_PI);
    //coonstruye la barrera del área de exclusión
    __Barrier = new TBarrier(getP0(), getthetaO1());

    //copia las propiedades
    CopyCilinder(C);
}

//libera la memoria dinámica del posicionador de fibra
TCilinder::~TCilinder()
{
    //destruye los objetos reservados dinámicamente
    delete __Barrier;
    delete __Arm;
}

//---------------------------------------------------------------------------
//MÉTODOS DE TRANSFORMACIÓN ENTRE SISTEMAS DE COORDENADAS:

//transforma un ángulo en radianes
//del sistema S0 al sistema S1
//      theta_ = thetaO1 - theta
double TCilinder::S0thetaToS1theta(double theta) const
{
    return getthetaO1() - theta;
}
//transforma un punto en coordenadas cartesianas
//del sistema S0 al sistema S1
//      P_.x = (x-P0.x)*cos(thetaO1) + (y-P0.y)*sin(thetaO1);
//      P_.y = (x-P0.x)*sin(thetaO1) - (y-P0.y)*cos(thetaO1);
TDoublePoint TCilinder::S0recToS1rec(double x, double y) const
{
    TDoublePoint P_;
    P_.x = (x-getP0().x)*cos(getthetaO1()) + (y-getP0().y)*sin(getthetaO1());
    P_.y = (x-getP0().x)*sin(getthetaO1()) - (y-getP0().y)*cos(getthetaO1());
    return P_;
}
TDoublePoint TCilinder::S0recToS1rec(TDoublePoint P) const
{
    TDoublePoint P_;
    P_.x = (P.x-getP0().x)*cos(getthetaO1()) + (P.y-getP0().y)*sin(getthetaO1());
    P_.y = (P.x-getP0().x)*sin(getthetaO1()) - (P.y-getP0().y)*cos(getthetaO1());
    return P_;
}
//transforma un punto en coordenadas cartesianas
//del sistema S1 al sistema S0
//      P.x = (x_)*cos(thetaO1) + (y_)*sin(thetaO1) + P0.x;
//      P.y = (x_)*sin(thetaO1) - (y_)*cos(thetaO1) + P0.y;
TDoublePoint TCilinder::S1recToS0rec(double x_, double y_) const
{
    TDoublePoint P;
    P.x = (x_)*cos(getthetaO1()) + (y_)*sin(getthetaO1()) + getP0().x;
    P.y = (x_)*sin(getthetaO1()) - (y_)*cos(getthetaO1()) + getP0().y;
    return P;
}

//transforma las coordenadas cartesiaans en S2
//en coordenadas cartesianas en S1
TDoublePoint TCilinder::S2recToS1rec(double x__, double y__) const
{
    TDoublePoint P_;
    P_.x = getL01() - x__;
    P_.y = y__;
    return P_;
}

//--------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR LA PERTENENCIA A DOMIOS DE ORIENTACIÓN:

//determina si un ángulo en radianes
//está fuera del intervalo [theta_1min, theta_1max]
bool TCilinder::IsntInDomaintheta_1(double _theta_1) const
{
    //si está fuera de los límites
    if(_theta_1<gettheta_1min() || gettheta_1max()<_theta_1)
        return true; //indica que está feura del dominio

    //indica que no está fuera del dominio
    return false;
}
//determina si un ángulo pasos
//está fuera del intervalo [p_1min, p_1max]
bool TCilinder::IsntInDomainp_1(double _p_1) const
{
    //el ángulo p_1 debería estar en el dominio de G(p_1)
    if(!getG().BelongToDomain(_p_1))
        throw EImproperArgument("angle p_1 should be in domain G(p_1)");

    //traduce a radianes y determina si theta_1 está en [theta_1min, theta_1max]
    return IsntInDomaintheta_1(getG().Image(_p_1));
}

//determina si un ángulo en radianes
//está dentro del intervalo [theta_1min, theta_1max]
bool TCilinder::IsInDomaintheta_1(double _theta_1) const
{
    //devuelve IsntInDomain(theta_1) negada
    return !IsntInDomaintheta_1(_theta_1);
}
//determina si un ángulo pasos
//está dentro del intervalo [p_1min, p_1max]
bool TCilinder::IsInDomainp_1(double _p_1) const
{
    //devuelve IsntInDomain(p_1) negada
    return !IsntInDomainp_1(_p_1);
}

//---------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR VALORES CUANTIFICADOS DE ORIENTACIONES:

//devuelve el valor de cuantificación en radianes
//más próximo al valor indicado
//en el intervalo [theta_1min, theta_1max]
double TCilinder::Qtheta_1(double theta_1)
{
    //el ángulo theta_1 debería estar en el dominio de theta_1
    if(theta_1<gettheta_1min() || gettheta_1max()<theta_1)
        throw EImproperArgument("angle theta_1 should be in theta_1 domain");

    double p_1 = getF().Image(theta_1);
    p_1 = getQ().Quantifice(p_1);
    return getG().Image(p_1);
}

//determina las posiciones angulares estables que hacen que
//la fibra de este posicionador se ubique lo más cerca posible
//del punto correspondiente a unas posiciones angulares de los ejes
//devuelve la distancia al punto hallado
double TCilinder::GetNearestStablePosition(double &p_1nsp, double &p___3nsp,
                                           double theta_1, double theta___3)
{
    //GUARDA LA CONFIGURACIÓN ORIGINAL DEL ACTUADOR:

    //guarda el estado de habilitación de los cuantificadores de los ejes
    PushQuantify_();
    getArm()->PushQuantify___();
    //guarda las posiciones angulares de los ejes
    Pushtheta_1();
    getArm()->Pushtheta___3();

    //DETERMINALAS COORDENAS CARTESIANAS DEL PUNTO DADO:

    //desactiva la cuantificación de los ejes
    setQuantify_(false);
    getArm()->setQuantify___(false);

    //mueve los ejes a las posiciones indicadas
    SetAnglesRadians(theta_1, theta___3);

    //guarda P
    TDoublePoint P = getArm()->getP3();

    //ADVERTENCIA: las coordenadas de P3 vienen dadas en S0.

    //BUSCA EL PUNTO MÁS PRÓMIMO AL PUNTO P ENTRE LOS
    //CORRESPONDIENTES A LAS POSICIONES ANGULARES ADYACENTES:

    //calcula los límites del intervalo de búsqueda
    double p_1min = floor(getp_1());
    double p_1max = ceil(getp_1());
    double p___3min = floor(getArm()->getp___3());
    double p___3max = ceil(getArm()->getp___3());

    //activa los cuantificadores de los ejes
    setQuantify_(true);
    getArm()->setQuantify___(true);

    //mueve los ejes al punto (min, min)
    SetAnglesSteps(p_1min, p___3min);
    //guarda la solución
    p_1nsp = p_1min;
    p___3nsp = p___3min;
    //calcula la distancia entre puntos
    double R = Mod(getArm()->getP3() - P);

    //si el punto (min, min) coincide exactamente con el punto P
    if(getArm()->getP3() == P)
        return 0; //termina la búsqueda indicando distancia igual a cero

    //mueve los ejes al punto (max, min)
    SetAnglesSteps(p_1max, p___3min);
    //calcula la distancia entre puntos
    double r = Mod(getArm()->getP3() - P);

    //si la distancia es menor que la anterior
    if(r < R) {
        //guarda la solución
        p_1nsp = p_1max;
        p___3nsp = p___3min;
        //actualiza la distancia
        R = r;
    }
    //si el punto (max, min) coincide exactamente con el punto P
    if(getArm()->getP3() == P)
        return 0; //termina la búsqueda indicando distancia igual a cero

    //mueve los ejes al punto (max, max)
    SetAnglesSteps(p_1max, p___3max);
    //calcula la distancia entre puntos
    r = Mod(getArm()->getP3() - P);

    //si la distancia es menor que la anterior
    if(r < R) {
        //guarda la solución
        p_1nsp = p_1max;
        p___3nsp = p___3max;
        //actualiza la distancia
        R = r;
    }
    //si el punto (max, max) coincide exactamente con el punto P
    if(getArm()->getP3() == P)
        return 0; //termina la búsqueda indicando distancia igual a cero

    //mueve los ejes al punto (min, max)
    SetAnglesSteps(p_1min, p___3max);
    //calcula la distancia entre puntos
    r = Mod(getArm()->getP3() - P);

    //si la distancia es menor que la anterior
    if(r < R) {
        //guarda la solución
        p_1nsp = p_1min;
        p___3nsp = p___3max;
        //actualiza la distancia
        R = r;
    }
    //si el punto (min, max) coincide exactamente con el punto P
    if(getArm()->getP3() == P)
        return 0; //termina la búsqueda indicando distancia igual a cero

    //------------------------------------------------------------------
    //CALCULA LOS LÍMITES DEL INTERVALO DE BÚSQUEDA
    //(theta_1min, theta_1max)x(theta___3min, theta___3max):

    //calcula los límites del intervalo para theta___3:

    //calcula la distancia de P0 a P
    r = Mod(P - getP0());

    //calcula theta___3min
    double r_3 = r - R;
    r_3 = Max(r_3, getr_3min());
    double theta___3min;
    bool exist = theta___3ToGor_3(theta___3min, r_3);

    //realiza un control rutinario
    if(!exist)
        throw EImposibleError("possible malfunction of Max or theta___3ToGor_3");

    //calcula theta___3max
    r_3 = r + R;
    r_3 = Min(r_3, getr_3max());
    double theta___3max;
    exist = theta___3ToGor_3(theta___3max, r_3);

    //realiza un control rutinario
    if(!exist)
        throw EImposibleError("possible malfunction of Min or theta___3ToGor_3");

    //calcula los límites del intervalo para theta_1:

    //determina los puntos que se encuentran a una distancia
    //L01 de P0 y Arm->L13-R de P
    TDoublePoint P1, P2;
    int type = IntersectionCircumCircum(P1, P2,
                                        P, getArm()->getL13()-R, getP0(), getL01());

    //las circunferencias deben ser de tipo secantes:
    //      0: exteriores
    //      1: coincidentes
    //      2: interiores concéntricas
    //      3: interiores excéntricas
    //      4: tangentes interiormente
    //      5: tangentes exteriormente
    //      6: secantes
    if(type != 6)
        throw EImposibleError("circunferences (P0, L01) y (P, Arm->L13-R) should be secant");

    //El punto P1 corresponde al punto superior cuando
    //(P, L13-R) esta a la izquierda y (P0, L01) está a la derecha.

    //calcula el argumento positivo del vector de P0 a P1
    settheta_1min(ArgPos(P1 - getP0()));

    //determina los puntos que se encuentran a una distancia
    //L01 de P0 y Arm->L13+R de P
    type = IntersectionCircumCircum(P1, P2,
                                    P, getArm()->getL13()+R, getP0(), getL01());

    //las circunferencias deben ser de tipo secantes:
    //      0: exteriores
    //      1: coincidentes
    //      2: interiores concéntricas
    //      3: interiores excéntricas
    //      4: tangentes interiormente
    //      5: tangentes exteriormente
    //      6: secantes
    if(type != 6)
        throw EImposibleError("circunferences (P0, L01) y (P, Arm->L13+R) should be secant");

    //El punto P1 corresponde al punto superior cuando
    //(P, L13+R) esta a la izquierda y (P0, L01) está a la derecha.

    //calcula el argumento positivo del vector de P0 a P1
    settheta_1max(ArgPos(P1 - getP0()));

    //------------------------------------------------------------------
    //BUSCA EL PUNTO MÁS CERCANO EN EL INTERVALO ABIERTO
    //(p_1min, p_1max)x(p___3min, p___3max):

    //traduce los límites del intervalo a pasos
    p_1min = getF().Image(gettheta_1min());
    p_1max = getF().Image(gettheta_1max());
    p___3min = getArm()->getF().Image(theta___3min);
    p___3max = getArm()->getF().Image(theta___3max);

    //por cada punto estable de las trayectorias que atraviesan el círculo (P, R)
    for(int i=ceil(p_1min); i<p_1max; i++) {
        for(int j=ceil(p___3min); j<p___3max; j++) {
            //transforma los ángulos de los ejes a double
            double p_1 = double(i);
            double p___3 = double(j);

            //asigna las coordenadas correspondientes en pasos
            SetAnglesSteps(p_1, p___3);
            //ADVERTECNIA: como la cuentificación de los ejes está
            //habilitada el error numérico es corregido

            //si el punto P3coincide exactamente con el punto dado
            if(getArm()->getP3() == P) {
                //actualiza las posiciones angulares estables
                p_1nsp = p_1;
                p___3nsp = p___3;
                //termina la búsqueda indicando distancia igual a cero
                return 0;
            }

            //calcula la distancia al punto dado
            r = Mod(getArm()->getP3() - P);

            //si el punto P3 está más próximo al punto dado
            if(r < R) {
                //actualiza las posiciones angulares estables
                p_1nsp = p_1;
                p___3nsp = p___3;
                //actualiza el radio del espacio de búsqueda
                R = r;

                //aquí cabría la posibilidad de indicar que
                //debe reiniciarse el proceso, para que se
                //determine un nuevo intervalo de búsqueda,
                //pero se considera que el núnero de puntos
                //no debe ser tan grande como para justificarlo.
            }
        }
    }

    //------------------------------------------------------------------
    //RESTAURA Y DESCARTA LA CONFIGURACIÓN ORIGINAL DEL ACTUADOR:

    //restaura y descarta las posiciones angulares de los ejes
    getArm()->RestoreAndPoptheta___3();
    RestoreAndPoptheta_1();
    //restaura y desempila el estado de habilitación de
    //los cuantificadores de los ejes
    getArm()->RestoreAndPopQuantify___();
    RestoreAndPopQuantify_();

    //devuelve la distancia al punto hallado
    return R;
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONFIGURACIÓN:

//cambia conjuntamente las propiedades de plantilla
//si no especifica argumentos se asignarán valores por defecto
void TCilinder::SetTemplate(double _L01, double _L12, double _L13,
                            double _theta___3, double _R3)
{
    //el radio del cilindro L01 debe ser mayor que cero
    if(_L01 <= 0)
        throw EImproperArgument("cilinder radio L01 shoud be upper zero");

    //redimensiona el cilindro
    setL01(_L01);
    //redimensiona el brazo
    getArm()->SetTemplate(_L12, _L13, _theta___3, _R3);
}

//desactiva la cuantificación de los ejes del posicionador
void TCilinder::DisableQuantification(void)
{
    //desactiva la cuantificación del eje 1
    setQuantify_(false);
    //desactiva la cuantificación del eje 2
    getArm()->setQuantify___(false);
}
//activa la cuantificación de los ejes del posicionador
void TCilinder::EnableQuantification(void)
{
    //activa la cuantificación del eje 1
    setQuantify_(true);
    //activa la cuantificación del eje 2
    getArm()->setQuantify___(true);
}

//---------------------------------------------------------------------------
//MÉTODOS PARA ALMACENAR Y RECUPERAR ORIENTACIONES:

//apila theta_1 en la pila LIFO theta_1s
//      theta_1s.Add(theta_1)
void TCilinder::Pushtheta_1(void)
{
    theta_1s.Add(gettheta_1());
}
//restaura el último theta_1 de la pila LIFO theta_1s
//      theta_1 = theta_1s.Last
//si no hay una posición apilada lanza EImproperCall
void TCilinder::Restoretheta_1(void)
{
    //la pila theta_1s debería contener algún valor apilado
    if(theta_1s.getCount() < 1)
        throw EImproperCall("stack theta_1s should contain some stacking value");

    //asigna el último valor apilado
    settheta_1(theta_1s.getLast());
}
//desempila el último theta_1 de la pila LIFO theta_1s
//      theta_1s.Delete(theta_1.Count - 1)
//si no hay una posición apilada lanza EImproperCall
void TCilinder::Poptheta_1(void)
{
    //la pila theta_1s debería contener algún valor apilado
    if(theta_1s.getCount() < 1)
        throw EImproperCall("stack theta_1s should contain some stacking value");

    //borra el último valor apilado
    theta_1s.Delete(theta_1s.getCount() - 1);
}
//restaura y desempila el último theta_1 de la pila LIFO theta_1s
//      theta_1s.Delete(theta_1.Count - 1)
//si no hay una posición apilada lanza EImproperCall
void TCilinder::RestoreAndPoptheta_1(void)
{
    //la pila theta_1s debería contener algún valor apilado
    if(theta_1s.getCount() < 1)
        throw EImproperCall("stack theta_1s should contain some stacking value");

    //asigna el último valor apilado
    settheta_1(theta_1s.getLast());
    //borra el último valor apilado
    theta_1s.Delete(theta_1s.getCount() - 1);
}

//---------------------------------------------------------------------------
//MÉTODOS PARA ALMACENAR Y RECUPERAR ESTADOS DE CUNATIFICACIÓN:

//apila Quantify_ en la pila LIFO Quantify_s
//      Quantify_s.Add(Quantify_)
void TCilinder::PushQuantify_(void)
{
    Quantify_s.Add(getQuantify_());
}
//restaura el último Quantify_ de la pila LIFO Quantify_s
//      Quantify_ = Quantify_s.Last
//si no hay una posición apilada lanza EImproperCall
void TCilinder::RestoreQuantify_(void)
{
    //la pila Quantify_s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Quantify_s should contain some stacking value");

    //asigna el último valor apilado
    setQuantify_(Quantify_s.getLast());
}
//desempila el último Quantify_ de la pila LIFO Quantify_s
//      Quantify_s.Delete(Quantify_.Count - 1)
//si no hay una posición apilada lanza EImproperCall
void TCilinder::PopQuantify_(void)
{
    //la pila Quantify_s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Quantify_s should contain some stacking value");

    //borra el último valor apilado
    Quantify_s.Delete(Quantify_s.getCount() - 1);
}
//restaura y desempila el último Quantify_ de la pila LIFO Quantify_s
//      Quantify_s.Delete(Quantify_.Count - 1)
//si no hay una posición apilada lanza EImproperCall
void TCilinder::RestoreAndPopQuantify_(void)
{
    //la pila Quantify_s debería contener algún valor apilado
    if(Quantify_s.getCount() < 1)
        throw EImproperCall("stack Quantify_s should contain some stacking value");

    //asigna el último valor apilado
    setQuantify_(Quantify_s.getLast());
    //borra el último valor apilado
    Quantify_s.Delete(Quantify_s.getCount() - 1);
}

//--------------------------------------------------------------------------
//MÉTODOS DE MOVIMIENTO A ÁNGULOS:

//cambia la posición y orientación
//del origen de coordenadas simultaneamente
void TCilinder::MoveOrigin(TDoublePoint _P0, double _thetaO1)
{
    //asigna los nuevos valores
    __P0 = _P0;
    __thetaO1 = _thetaO1;

    //cambiar la posición y orientación del posicionador de fibra
    //modifica la posición y orientación del origen de coordenadas del brazo

    //cambia la posición y orientación del origen de coordenadas del brazo
    getArm()->Set(NewP1(), getthetaO1() - gettheta_1() - M_PI);
}

//asigna conjuntamente theta_1 y theta___3
void TCilinder::SetAnglesRadians(double _theta_1, double _theta___3)
{
    //el ángulo theta_1 debe estar en [theta_1min, theta_1max]
    if(IsntInDomaintheta_1(_theta_1))
        throw EImproperArgument("angle theta_1 should be in [thata_1min, thata_1max]");

    //el ángulo theta___3 debe estar en [theta___3min, theta___3max]
    if(getArm()->IsntInDomaintheta___3(_theta___3))
        throw EImproperArgument("angle theta___3 should be in [theta___3min, theta___3max]");

    //si la cuantificación está activada
    if(getQuantify_())
        //cuantifica el valor
        _theta_1 = Qtheta_1(_theta_1);

    //si alguno de los nuevos valores difiere del actual
    if(_theta_1!=gettheta_1() || _theta___3!=getArm()->gettheta___3()) {
        //asigna el nuevo valor
        __theta_1 = _theta_1;

        //cambiar la orientación del posicionador de fibra
        //modifica la posición y orientación del origen de coordenadas del brazo

        //mueve el brazo
        getArm()->Set(NewP1(), getthetaO1() - gettheta_1() - M_PI, _theta___3);
    }
}

//asigna conjuntamente p_1 y p___3
void TCilinder::SetAnglesSteps(double _p_1, double _p___3)
{
    //el ángulo p_1 debe estar en [p_1min, p_1max]
    if(IsntInDomainp_1(_p_1))
        throw EImproperArgument("angle p_1 should be in [thata_1min, thata_1max]");

    //el ángulo p___3 debe estar en [p___3min, p___3max]
    if(getArm()->IsntInDomainp___3(_p___3))
        throw EImproperArgument("angle p___3 should be in [p___3min, p___3max]");

    //traduce los ángulos a radianes
    double _theta_1 = getG().Image(_p_1);
    double _theta___3 = getArm()->getG().Image(_p___3);

    //si la cuantificación está activada
    if(getQuantify_())
        //cuantifica el valor
        _theta_1 = Qtheta_1(_theta_1);

    //si alguno de los nuevos valores difiere del actual
    if(_theta_1!=gettheta_1() || _theta___3!=getArm()->gettheta___3()) {
        //asigna el nuevo valor
        __theta_1 = _theta_1;

        //cambiar la orientación del posicionador de fibra
        //modifica la posición y orientación del origen de coordenadas del brazo

        //mueve el brazo
        getArm()->Set(NewP1(), getthetaO1() - gettheta_1() - M_PI, _theta___3);
    }
}

//mueve los ejes hasta el origen de coordenadas
void TCilinder::SetAnglesZeroSteps(void)
{
    setp_1(0);
    getArm()->setp___3(0);
}

//añade conjuntamente 'at_1' y 'at___3' a 'theta_1' y 'theta___3'
void TCilinder::AddAnglesRadians(double at_1, double at__3)
{
    //calcula el ángulo para el eje 1
    double _theta_1 = gettheta_1() + at_1;

    //el ángulo theta_1 debe estar en [theta_1min, theta_1max]
    if(IsntInDomaintheta_1(_theta_1))
        throw EImproperArgument("angle theta_1 should be in [thata_1min, thata_1max]");

    //calcula el ángulo para el eje 2
    double _theta___3 = gettheta_3() + at__3;

    //el ángulo theta___3 debe estar en [theta___3min, theta___3max]
    if(getArm()->IsntInDomaintheta___3(_theta___3))
        throw EImproperArgument("angle theta___3 should be in [theta___3min, theta___3max]");

    //si la cuantificación está activada
    if(getQuantify_())
        //cuantifica el valor
        _theta_1 = Qtheta_1(_theta_1);

    //si alguno de los nuevos valores difiere del actual
    if(_theta_1!=gettheta_1() || _theta___3!=getArm()->gettheta___3()) {
        //asigna el nuevo valor
        __theta_1 = _theta_1;

        //cambiar la orientación del posicionador de fibra
        //modifica la posición y orientación del origen de coordenadas del brazo

        //mueve el brazo
        getArm()->Set(NewP1(), getthetaO1() - gettheta_1() - M_PI, _theta___3);
    }
}
//añade conjuntamente 'a_1' y 'a___3' a 'p_1' y 'p___3'
void TCilinder::AddAnglesSteps(double ap_1, double ap___3)
{
    //calcula el ángulo para el eje 1
    double _p_1 = getp_1() + ap_1;

    //el ángulo p_1 debe estar en [p_1min, p_1max]
    if(IsntInDomainp_1(_p_1))
        throw EImproperArgument("angle p_1 should be in [thata_1min, thata_1max]");

    //calcula el ángulo para el eje 2
    double _p___3 = getArm()->getp___3() + ap___3;

    //el ángulo p___3 debe estar en [p___3min, p___3max]
    if(getArm()->IsntInDomainp___3(_p___3))
        throw EImproperArgument("angle p___3 should be in [p___3min, p___3max]");

    //traduce los ángulos a radianes
    double _theta_1 = getG().Image(_p_1);
    double _theta___3 = getArm()->getG().Image(_p___3);

    //si la cuantificación está activada
    if(getQuantify_())
        //cuantifica el valor
        _theta_1 = Qtheta_1(_theta_1);

    //si alguno de los nuevos valores difiere del actual
    if(_theta_1!=gettheta_1() || _theta___3!=getArm()->gettheta___3()) {
        //asigna el nuevo valor
        __theta_1 = _theta_1;

        //cambiar la orientación del posicionador de fibra
        //modifica la posición y orientación del origen de coordenadas del brazo

        //mueve el brazo
        getArm()->Set(NewP1(), getthetaO1() - gettheta_1() - M_PI, _theta___3);
    }
}

//genera un valor aleatorio con distribución uniforme en:
//      [Max(0, p_1min), Min(floor(SB1), p_1max)]
double TCilinder::Randomp_1(void)
{
    return RandomUniform(Max(0., getp_1min()), Min(floor(getSB1()), getp_1max()));
}
//asigna a p_1 un valor aleatorio con distribución uniforme en
//      [Max(0, p_1min), Min(floor(SB1), p_1max)]
void TCilinder::Randomizep_1(void)
{
    setp_1(Randomp_1());
}

//--------------------------------------------------------------------------
//MÉTODOS DE DETERMINACIÓN DE ÁNGULOS:

//Dado el punto (r_3, theta_3) (en S1)
//calcula (theta_1, theta___3) para que P3 vaya a él;
//si el punto no está dentro del dominio devuelve falso.
//Aunque el punto sea inalcanzable, este método devolverá
//las posiciones límite a la que los ejes pueden ir.
bool TCilinder::AnglesToGoP_3(double &_theta_1, double &_theta___3,
                              double r_3, double theta_3)
{
    //el punto debe estar en el intervalo radial
    if(r_3<getr_3min()-ERR_NUM || getr_3max()+ERR_NUM<r_3)
        return false; //indica que la posición es inalcanzable

    //obtiene el argumento principal de theta_3
    theta_3 = ArgPos(theta_3);

    //si no obtiene el argumento principal
    //pueden obtenerse valores de theta_1 fuera del dominio

    //calcula las posiciones de los ejes
    //como si no tuvieran límites

    double den = 2*getL01()*r_3;
    if(den == 0) { //si P3 está sobre P0
        //el eje 1 debe ir a un cuarto de vuelta antes
        _theta_1 = theta_3 - M_PI/2;
        _theta___3 = 0; //el eje 2 debe permanecer en cero
    }
    else { //si el denominador es mayor que cero
        //calcula la proyección del ángulo 1
        double x_ = (getL01()*getL01() + r_3*r_3 - getArm()->getL13()*getArm()->getL13())/(den);
        //corrige el error numérico
        if(x_ < -1)
            x_ = -1;
        else if(x_ > 1)
            x_ = 1;
        //obtiene la posición del eje 1
        _theta_1 = theta_3 - acos(x_);
        //normaliza la posición del eje 1
        _theta_1 = ArgPos(_theta_1);

        //calcula la proyección del ángulo 2
        double x__ = (getL01()*getL01() +
                      getArm()->getL13()*getArm()->getL13() - r_3*r_3)/
                (2*getL01()*getArm()->getL13());
        //corrige el error numérico
        if(x__ < -1)
            x__ = -1;
        else if(x__ > 1)
            x__ = 1;
        //obtiene la posición del eje 2
        _theta___3 = acos(x__);
    }

    //determina si los posiciones de los ejes están dentro de sus dominios

    bool reachable;
    if(_theta_1<gettheta_1min() || gettheta_1max()<_theta_1)
        reachable = false;
    else if(getArm()->IsntInDomaintheta___3(_theta___3))
        reachable = false;
    else
        reachable = true;

    //constriñe la posición del eje 1 a los límites del dominio
    if(_theta_1 < gettheta_1min())
        _theta_1 = gettheta_1min();
    else if(gettheta_1max() < _theta_1)
        _theta_1 = gettheta_1max();

    //constriñe la posición del eje 2 a los límites del dominio
    if(_theta___3 < getArm()->gettheta___3min())
        _theta___3 = getArm()->gettheta___3min();
    else if(getArm()->gettheta___3max() < _theta___3)
        _theta___3 = getArm()->gettheta___3max();

    return reachable; //indica si la posición es alcanzable
}
//Dado el punto (x3, y3) (en S0)
//calcula (theta_1, theta___3) para que P3 vaya a él;
//si el punto no está dentro del dominio devuelve falso.
//Aunque el punto sea inalcanzable, este método devolverá
//las posiciones límite a la que los ejes pueden ir.
bool TCilinder::AnglesToGoP3(double &_theta_1, double &_theta___3,
                             double x3, double y3)
{
    //traduce a coordenadas relativas a S1
    TDoublePoint P_ = S0recToS1rec(x3, y3);

    //traduce a coordenadas polares relativas a S1
    double r_;
    double theta_;
    if(P_.x!=0 || P_.y!=0) {
        r_ = Mod(P_);
        theta_ = ArgPos(P_);
    } else {
        r_ = 0;
        theta_ = M_PI/2;
    }

    //determina los ángulos para que P3 se posiciones en (r_, theta_)
    return AnglesToGoP_3(_theta_1, _theta___3, r_, theta_);
}

//Dado el ángulo theta_3 (en S1) calcula theta___3 para que P3 vaya a él;
//si el ángulo no está dentro del dominio devuelve falso.
bool TCilinder::theta___3ToGotheta_3(double &_theta___3, double _theta_3)
{
    //si el brazo tiene que retorcerse
    if(_theta_3 < gettheta_1())
        return false; //el punto es inalcanzable

    //en esta caso no hace falta calcular el argumento principal de theta_3
    //ya que solo se usa en cos(_theta_3 - theta_1)

    //CALCULA r_3:

    //resuelve la ecuación L13^2 = L01^2 + r_3^2 - 2*L01*r_3*cos(alpha)

    double aux = getL01()*cos(_theta_3 - gettheta_1());
    double rad = aux*aux - getL01()*getL01() +
            getArm()->getL13()*getArm()->getL13();

    //si el radicando es negativo, el punto es inalcanzable
    if(rad < 0)
        return false;

    double r_3 = aux + sqrt(rad);

    //CALCULA theta___3

    double x_3 = (getL01()*getL01() +
                  getArm()->getL13()*getArm()->getL13() - r_3*r_3)/
            (2*getL01()*getArm()->getL13());

    //corrige el error numérico en caso necesario
    if(x_3 < -1)
        x_3 = -1;
    else if(x_3 > 1)
        x_3 = 1;

    _theta___3 = acos(x_3); //obtiene la solución
    return true; //indica que la posición es alcanzable
}
//Dado el radio r_3 (en S1) calcula theta___3 para que P3 vaya a él;
//si el radio no está dentro del dominio devuelve falso.
bool TCilinder::theta___3ToGor_3(double &_theta___3, double _r_3)
{
    //el punto debe estar en el intervalo radial
    if(_r_3<getr_3min() || getr_3max()<_r_3)
        return false; //indica que el radio no está en el dominio

    if(_r_3 == 0) { //si quiere ir al origen
        _theta___3 = 0;
    }
    else { //si no quiere ir al origen
        //resuelve la ecuación
        //r_3^2 = L01^2 + L13^2 - 2*L01*L13*cos(M_PI - theta_O3o + theta___3)

        //calcula el denominador de la fórmula del coseno
        double den = 2*getL01()*getArm()->getL13();

        //realiza un control rutinario para excluir la situación imposible
        if(den == 0)
            throw EImposibleError("2*L01*Arm->L13 has been zero");

        //calcula el argumento de la función acos
        double x___3 = (getL01()*getL01() + getArm()->getL13()*getArm()->getL13() - _r_3*_r_3)/den;
        //corrige el error numérico del argumento en caso necesario
        if(x___3 < -1)
            x___3 = -1;
        else if(x___3 > 1)
            x___3 = 1;

        //Sean a, b y c la longitud de los lados de un triángulo,
        //la demostración de que: (a^2 + b^2 - c^2)/(2*a*b)
        //se mantiene en el intervalo [-1, 1],
        //es la misma que la de la fórmula del coseno.
        //De modo que el argumento de la función acos nunca
        //se saldrá del intervalo [-1, 1].

        //obtiene la solución
        _theta___3 = gettheta_O3o() - M_PI + acos(x___3);
    }

    return true; //indica que el radio si está en el dominio
}

//MÉTODOS DE PERTENENCIA AL DOMINIO DE P3:

//determina si el punto P está dentro del dominio de P3
bool TCilinder::PointIsInDomainP_3(TDoublePoint P_)
{
    double theta_1, theta___3;

    //traduce el punto a coordenadas polares
    double r_ = Mod(P_);
    double theta_ = Arg(P_.x, P_.y);

    //si está dentro del dominio
    if(AnglesToGoP_3(theta_1, theta___3, r_, theta_))
        return true; //indica que está dentro

    return false; //inica que no está dentro
}
//determina si el punto P está fuera del dominio de P3
bool TCilinder::PointIsOutDomainP_3(TDoublePoint P_)
{
    double theta_1, theta___3;

    //traduce el punto a coordenadas polares
    double r_ = Mod(P_);
    double theta_ = Arg(P_.x, P_.y);

    //si está dentro del dominio
    if(AnglesToGoP_3(theta_1, theta___3, r_, theta_))
        return false; //indica que no está fuera

    return true; //sindica que está fuera
}

//determina si el punto P está dentro del dominio de P3
bool TCilinder::PointIsInDomainP3(TDoublePoint P)
{
    double theta_1, theta___3;

    //si está dentro del dominio
    if(AnglesToGoP3(theta_1, theta___3, P.x, P.y))
        return true; //indica que está dentro

    return false; //inica que no está dentro
}
//determina si el punto P está fuera del dominio de P3
bool TCilinder::PointIsOutDomainP3(TDoublePoint P)
{
    double theta_1, theta___3;

    //si está dentro del dominio
    if(AnglesToGoP3(theta_1, theta___3, P.x, P.y))
        return false; //indica que no está fuera

    return true; //indica que está fuera
}
//determina si el punto (x, y) está fuera del dominio de P3
bool TCilinder::PointIsOutDomainP3(double x, double y)
{
    double theta_1, theta___3;

    //si está dentro del dominio
    if(AnglesToGoP3(theta_1, theta___3, x, y))
        return false; //indica que no está fuera

    return true; //indica que está fuera
}

//determina el arco descrito por P3 al girar el eje 1
//en el intervalo [Max(0., theta_1min), Min(2_MPI, theta_imax)]
//con el brazo totalmente extendido
//si el arco contiene un error numérico significativo hace Pfin = Pini
void TCilinder::GetArc(TDoublePoint &Pini, TDoublePoint &Pfin, TDoublePoint &Pc,
                       double &R)
{
    //el centro del arco ya viene dado por P0
    Pc = getP0();

    //calcula el radio del arco
    R = getL01() + getArm()->getL13();

    //determina el límite inferior angular de P3 en S1
    double theta_ini = Max(0., gettheta_1min());
    //determina el vértice superior del arco en S1
    Pini.x = R*cos(theta_ini);
    Pini.y = R*sin(theta_ini);
    //traduce de S1 a S0
    Pini = S1recToS0rec(Pini.x, Pini.y);

    //determina el límite superior angular de P3 en S1
    double theta_fin = Min(M_2PI, gettheta_1max());
    //determina el vértice inferior del arco en S1
    Pfin.x = R*cos(theta_fin);
    Pfin.y = R*sin(theta_fin);
    //traduce de S1 a S0
    Pfin = S1recToS0rec(Pfin.x, Pfin.y);

    //calcula el ángulo de theta_ini a theta_fin
    double theta_ = theta_fin - theta_ini;
    //calcula el ángulo del arco (Pini, Pfin, Pc)
    double a_ = AnglePos(Pfin-Pc, Pini-Pc);
    //calcula la distancia entre ángulos
    double d_ = a_ - theta_;

    //si A contiene un error numérico significativo
    if(fabs(d_)>=M_PI)
        //corrige el error numérico
        Pfin = Pini;
}
//determina el arco descrito por P3 al plegar el brazo
//con el eje 1 en el ángulo theta_
void TCilinder::GetArc(TDoublePoint &Pini, TDoublePoint &Pfin, TDoublePoint &Pc,
                       double &R, double theta_)
{
    //el ángulo theta_ debería estar en [theta_1min, theta_1max]
    if(theta_<gettheta_1min() || gettheta_1max()<theta_)
        throw EImproperArgument("angle theta_ should be in [theta_1min, theta_1max]");

    //determina el centro del arco en S1
    Pc.x = getL01()*cos(theta_);
    Pc.y = getL01()*sin(theta_);
    //traduce de S1 a S0
    Pc = S1recToS0rec(Pc.x, Pc.y);

    //determina el vértice inicial del arco en S1
    double aux = getL01() + getArm()->getL13();
    Pini.x = aux*cos(theta_);
    Pini.y = aux*sin(theta_);
    //traduce de S1 a S0
    Pini = S1recToS0rec(Pini.x, Pini.y);

    //determina el vértice final del arco en S1
    aux = getL01() - getArm()->getL13();
    Pfin.x = aux*cos(theta_);
    Pfin.y = aux*sin(theta_);
    if(aux < 0) //si el brazo es más largo que el antebrazo
        Pfin = -1*Pfin; //invierte las coordenadas
    //traduce de S1 a S0
    Pfin = S1recToS0rec(Pfin.x, Pfin.y);
}

//Determina si un segmento interseca al arco descrito por P3
//al girar el eje 1 con el brazo totalemnte extendido.
bool TCilinder::IntersectionSegmentArc1(TDoublePoint Pa, TDoublePoint Pb)
{
    //el centro del arco ya viene dado por P0
    TDoublePoint Pc = getP0();

    //calcula el radio del arco
    double R = getL01() + getArm()->getL13();

    //determina el límite inferior angular de P3 en S1
    double theta_ini = Max(0., gettheta_1min());
    //determina el vértice superior del arco en S1
    TDoublePoint Pini;
    Pini.x = R*cos(theta_ini);
    Pini.y = R*sin(theta_ini);
    //traduce de S1 a S0
    Pini = S1recToS0rec(Pini.x, Pini.y);

    //determina el límite superior angular de P3 en S1
    double theta_fin = Min(M_2PI, gettheta_1max());
    //determina el vértice inferior del arco en S1
    TDoublePoint Pfin;
    Pfin.x = R*cos(theta_fin);
    Pfin.y = R*sin(theta_fin);
    //traduce de S1 a S0
    Pfin = S1recToS0rec(Pfin.x, Pfin.y);

    //calcula el ángulo de theta_ini a theta_fin
    double theta_ = theta_fin - theta_ini;
    //calcula el ángulo del arco (Pini, Pfin, Pc)
    double a_ = AnglePos(Pfin-Pc, Pini-Pc);
    //calcula la distancia entre ángulos
    double d_ = a_ - theta_;

    //si A contiene un error numérico significativo
    if(fabs(d_)>=M_PI) {
        //si theta_ está próximo a cero
        if(fabs(theta_) < M_PI) {
            //corrige el error numérico
            Pfin = Pini;
            //determina si hay intersección entre el segmento y el arco
            //y devuelve el resultado
            return IntersectionSegmentArc(Pa, Pb, Pini, Pfin, Pc, R);
            //si theta_ está próximo a un múltiplo de 2*pi distinto de cero
        } else { //fabs(theta_) >= M_PI
            //determina si hay intersección entre el segmento y la circunferencia
            //y devuelve el resultado
            if(DistanceSegmentCircunference(Pa, Pb, Pc, R) <= 0)
                return true;
        }
        //si el error numérico de A es insignificante
    } else {
        //determina si hay intersección entre el segmento y el arco
        //y devuelve el resultado
        return IntersectionSegmentArc(Pa, Pb, Pini, Pfin, Pc, R);
    }

    //indica que no hay intersección entre el segmento y el arco
    return false;
}
//Determina si un segmento interseca al arco descrito por P3
//al plegar el brazo cuando theta_1 = Max(0, theta_1min).
bool TCilinder::IntersectionSegmentArc2(TDoublePoint Pa, TDoublePoint Pb)
{
    //determina el arco en theta_ = Max(0, theta_1min)
    TDoublePoint Pini, Pfin, Pc;
    double R;
    GetArc(Pini, Pfin, Pc, R, Max(0., gettheta_1min()));

    //determina si hay intersección y devulve el resultado
    return IntersectionSegmentArc(Pa, Pb, Pini, Pfin, Pc, R);
}
//Determina si un segmento interseca al arco descrito por P3
//al plegar el brazo cuando theta_1 = Min(M_2PI, theta_1max).
bool TCilinder::IntersectionSegmentArc3(TDoublePoint Pa, TDoublePoint Pb)
{
    //determina el arco en theta_ = Min(M_2PI, theta_1max)
    TDoublePoint Pini, Pfin, Pc;
    double R;
    GetArc(Pini, Pfin, Pc, R, Min(M_2PI, gettheta_1max()));

    //determina si hay intersección y devulve el resultado
    return IntersectionSegmentArc(Pa, Pb, Pini, Pfin, Pc, R);
}

//Determina si un segmento invade el dominio de P3
//teniendo en cuenta los arcos descritos por P3 al girar el eje 1.
bool TCilinder::SegmentInvadeDomainP3(TDoublePoint Pa, TDoublePoint Pb)
{
    double theta_1, theta___3;

    //comprueba el punto Pa
    if(AnglesToGoP3(theta_1, theta___3, Pa.x, Pa.y))
        return true;

    //comprueba el punto Pb
    if(AnglesToGoP3(theta_1, theta___3, Pb.x, Pb.y))
        return true;

    //aunque Pa y Pb estén fuera del dominio una parte interna
    //del segmento puede estar dentro del dominio, en cuyo caso
    //intersecará alguno de los arcos que delimitan el dominio.

    /*        //determina el arco 1
        TDoublePoint Pini, Pfin, Pc;
        double R;
        GetArc(Pini, Pfin, Pc, R);
        //si hay intersección entre el segmneto y el arco
        if(IntersectionSegmentArc(Pa, Pb, Pini, Pfin, Pc, R))
                return true; //indica que el segmento invado el dominio de P3

        //determina el arco 2
        GetArc(Pini, Pfin, Pc, R, Max(0., theta_1min));
        //si hay intersección entre el segmneto y el arco
        if(IntersectionSegmentArc(Pa, Pb, Pini, Pfin, Pc, R))
                return true; //indica que el segmento invado el dominio de P3

        //determina el arco 3
        GetArc(Pini, Pfin, Pc, R, Min(M_2PI, theta_1max));
        //si hay intersección entre el segmneto y el arco
        if(IntersectionSegmentArc(Pa, Pb, Pini, Pfin, Pc, R))
                return true; //indica que el segmento invado el dominio de P3
  */
    if(IntersectionSegmentArc1(Pa, Pb))
        return true;

    if(IntersectionSegmentArc2(Pa, Pb))
        return true;

    if(IntersectionSegmentArc3(Pa, Pb))
        return true;

    //indica que no hay intersección
    return false;
}
//Determina si el segmento (Pa, Pb) no puede ser recorrido
//totalmente mediante un movimeinto continuo del punto P3,
//teniendo en cuenta los arcos descritos por P3 al girar el eje 1.
bool TCilinder::SegmentCantBeFollowedByP3(TDoublePoint Pa, TDoublePoint Pb)
{
    double theta_1, theta___3;
    bool isindomain; //indica la pertenencia al dominio de P3

    //los vértices del segmento deben estar dentro del dominio

    //comprueba el punto Pa
    isindomain = AnglesToGoP3(theta_1, theta___3, Pa.x, Pa.y);
    if(!isindomain)
        return true;

    //comprueba el punto Pb
    isindomain = AnglesToGoP3(theta_1, theta___3, Pb.x, Pb.y);
    if(!isindomain)
        return true;

    //para que el segmento pueda sr recorrido por P3
    //no debe cortar los arcos 2 y 3

    //determina el arco 2
    TDoublePoint Pini, Pfin, Pc;
    double R;
    GetArc(Pini, Pfin, Pc, R, Max(0., gettheta_1min()));
    //si hay intersección entre el segmneto y el arco
    if(IntersectionSegmentArc(Pa, Pb, Pini, Pfin, Pc, R))
        return true; //indica que el segmento no puede ser recorrido

    //determina el arco 3
    GetArc(Pini, Pfin, Pc, R, Min(M_2PI, gettheta_1max()));
    //si hay intersección entre el segmneto y el arco
    if(IntersectionSegmentArc(Pa, Pb, Pini, Pfin, Pc, R))
        return true; //indica que el segmento no puede ser recorrido

    //indica que el segmento si puede ser recorrido
    return false;
}

//Determina si un polisegmento invade el dominio de P3,
//teniendo en cuenta los arcos descritos por P3 al girar el eje 1.
bool TCilinder::PolysegmentInvadeDomainP3(TItemsList<TDoublePoint> &Polysegment)
{
    //para cada segmento del polisegmento
    for(int i=1; i<Polysegment.getCount(); i++)
        //si el segmento invade el dominio de P3
        if(SegmentInvadeDomainP3(Polysegment[i-1], Polysegment[i]))
            return true; //indica que el polisegmento invade en el dominio

    //indica que el polisegmento no invade el dominio
    return false;
}

//Determina si un polisegmento no puede ser recorrido
//totalmente mediante un movimeinto continuo del punto P3,
//teniendo en cuenta los arcos descritos por P3 al girar el eje 1.
bool TCilinder::PolysegmentCantBeFollowedByP3(TItemsList<TDoublePoint> &Polysegment)
{
    //para cada segmento del polisegmento
    for(int i=1; i<Polysegment.getCount(); i++)
        //si el segmento invade el dominio de P3
        if(SegmentCantBeFollowedByP3(Polysegment[i-1], Polysegment[i]))
            return true; //indica que el polisegmento invade en el dominio

    //indica que el polisegmento no invade el dominio
    return false;
    /*        //DETERMINA SI HAY ALGÚN PUNTO DEL POLISEGMENTO FUERA DEL DOMINIO DE P3:

        double theta_1, theta___3; //coordenadas posicionales de los ejes
        bool isindomain; //indica la pertenencia al dominio de P3
        TDoublePoint P; //un punto del polisegmento

        //para cada punto del polisegmento
        for(int i=0; i<Polysegment.Count; i++) {
                //asigna el punto indicado para facilitar su acceso
                P = Polysegment[i];
                //traduce el punto a coordenadas posicionales de los ejes
                //y determina si está dentro del dominio
                isindomain = AnglesToGoP3(theta_1, theta___3, P.x, P.y);
                //si no está en el dominio
                if(!isindomain)
                        return true; //indica que no está en el dominio
        }

        //NOTA: en virtud de la convexidad del arco descrito por P3 cuando
        //el brazo está totalmente extendido, solo podrá haber partes
        //del polisegmento fuera del dominio cuando algún punto esté fuera
        //o cuando un polisegmento atraviese alguno de los arcos descritos
        //por P3 al girar el eje 2.

        //si el eje 1 no llega a dar una vuelta completa
        if(theta_1max-theta_1min < M_2PI) {
                //------------------------------------------------------------------
                //DETERMINA SI HAY INTERSECCIÓN DEL POLISEGMENTO CON
                //EL ARCO DESCRITO POR P3 EN EL LÍMITE INFERIOR DEL EJE 1:

                double theta_; //un ángulo del eje 1
                double theta___; //un ángulo del eje 2

                TDoublePoint Pc; //punto ccentral del arco
                TDoublePoint Pini; //vértice inicial del arco
                TDoublePoint Pfin; //vértice final del arco

                //traduce el centro del arco a coordenadas cartesianas de S1
                theta_ = Max(0., theta_1min);
                Pc.x = L01*cos(theta_);
                Pc.y = L01*sin(theta_);

                //traduce la coordenada limíte superior angular de P3
                //a coordenada angular de S1
                theta___ = Min(M_PI, Arm->theta___3max);
                theta_ = Arm->theta__O3 + theta___;

                //traduce el vértice superior del arco a coordenadas cartesianas de S1
                Pini.x = Arm->P1.x + Arm->L13*cos(theta_);
                Pini.y = Arm->P1.y + Arm->L13*sin(theta_);

                //traduce la coordenada limíte inferior angular de P3
                //a coordenada angular de S1
                theta___ = Max(0., Arm->theta___3min);
                theta_ = Arm->theta__O3 + theta___;

                //traduce el vértice inferior del arco a coordenadas cartesianas de S1
                Pfin.x = Arm->P1.x + Arm->L13*cos(theta_);
                Pfin.y = Arm->P1.y + Arm->L13*sin(theta_);

                //si el dominio cubre el círculo completo
                //no necesita comprobar si hay intersección
                //con el arco en el límite superior
                //ya que coincide con el arco del límite inferior
                //de este modo se evita el error numérico

                TDoublePoint Pa, Pb; //vértices de un segmento

                //por cada segmento del polisegmento
                for(int i=1; i<Polysegment.Count; i++) {
                        //asigna los puntos del segmento indicado para facilitar su acceso
                        Pa = Polysegment[i-1];
                        Pb = Polysegment[i];
                        //si hay intersección entre el segmento y el arco
                        if(IntersectionSegmentArc(Pa, Pb, Pfin, Pini, Pc, Arm->L13))
                                //indica que el polisegmento no está dentro del dominio
                                return true;
                }

                //------------------------------------------------------------------
                //DETERMINA SI HAY INTERSECCIÓN DEL POLISEGMENTO CON
                //EL ARCO DESCRITO POR P3 EN EL LÍMITE SUPERIOR DEL EJE 1:

                //traduce el centro del arco a coordenadas cartesianas de S1
                theta_ = Min(M_2PI, theta_1max);
                Pc.x = L01*cos(theta_);
                Pc.y = L01*sin(theta_);

                //traduce la coordenada limíte superior angular de P3
                //a coordenada angular de S1
                theta___ = Min(M_PI, Arm->theta___3max);
                theta_ = Arm->theta__O3 + theta___;

                //traduce el vértice superior del arco a coordenadas cartesianas de S1
                Pini.x = Arm->P1.x + Arm->L13*cos(theta_);
                Pini.y = Arm->P1.y + Arm->L13*sin(theta_);

                //traduce la coordenada limíte inferior angular de P3
                //a coordenada angular de S1
                theta___ = Max(0., Arm->theta___3min);
                theta_ = Arm->theta__O3 + theta___;

                //traduce el vértice inferior del arco a coordenadas cartesianas de S1
                Pfin.x = Arm->P1.x + Arm->L13*cos(theta_);
                Pfin.y = Arm->P1.y + Arm->L13*sin(theta_);

                //por cada segmento del polisegmento
                for(int i=1; i<Polysegment.Count; i++) {
                        //asigna los puntos del segmento indicado para facilitar su acceso
                        Pa = Polysegment[i-1];
                        Pb = Polysegment[i];
                        //si hay intersección entre el segmento y el arco
                        if(IntersectionSegmentArc(Pa, Pb, Pfin, Pini, Pc, Arm->L13))
                                //indica que el polisegmento no está dentro del dominio
                                return true;
                        }

                //ERROR: IntersectionSegmentArc determina solo la intersección
                //de segmentos abiertos aislados.
                //Pero el polisegmento solo debe tener abiertos los vértices
                //de los segmentos extremos.
        }

        //------------------------------------------------------------------
        //indica que no ha encontrado indicios de
        //estar fuera del dominio o atravesarlo
        return false;   */
}

//MÉTODOS DE MOVIMIENTO A PUNTOS:

//asigna coordenadas polares a P_3
//si el punto no está en el dominio de P_3 lanza una excepción
void TCilinder::SetPolarP_3(double _r_3, double _theta_3)
{
    //trasforma de coordenadas polares a coordenadas angulares en radianes
    //y determina si el punto está en el dominio
    double _theta_1, _theta___3;
    bool isindomine = AnglesToGoP_3(_theta_1, _theta___3, _r_3, _theta_3);

    //el punto (r_3, theta_3) deve estar en el dominio de P_3
    if(!isindomine)
        throw EImproperArgument("point (r_3, theta_3) should be in domain P_3");

    //mueve los ejes a las posiciones correspondientes
    SetAnglesRadians(_theta_1, _theta___3);
}

//asigna coordenadas cartesianas a P_3
//si el punto no está en el dominio de P_3 lanza una excepción
void TCilinder::SetCartesianP_3(double _x_3, double _y_3)
{
    //traduce a coordenadas polares
    double _r_3 = Mod(_x_3, _y_3);
    double _theta_3 = Arg(_x_3, _y_3);

    //trasforma de coordenadas polares a coordenadas angulares en radianes
    //y determina si el punto está en el dominio
    double _theta_1, _theta___3;
    bool isindomine = AnglesToGoP_3(_theta_1, _theta___3, _r_3, _theta_3);

    //el punto (x_3, y_3) deve estar en el dominio de P_3
    if(!isindomine)
        throw EImproperArgument("point (x_3, y_3) should be in domain P_3");

    //mueve los ejes a las posiciones correspondientes
    SetAnglesRadians(_theta_1, _theta___3);
}
void TCilinder::SetCartesianP_3(TDoublePoint _P_3)
{
    //traduce a coordenadas polares
    double _r_3 = Mod(_P_3);
    double _theta_3 = Arg(_P_3);

    //trasforma de coordenadas polares a coordenadas angulares en radianes
    //y determina si el punto está en el dominio
    double _theta_1, _theta___3;
    bool isindomine = AnglesToGoP_3(_theta_1, _theta___3, _r_3, _theta_3);

    //el punto P_3 deve estar en su dominio
    if(!isindomine)
        throw EImproperArgument("point (x_3, y_3) should be in his domain");

    //mueve los ejes a las posiciones correspondientes
    SetAnglesRadians(_theta_1, _theta___3);
}

//genera un punto con distribución uniforme en
//el dominio cartesiano de P3 hasta que dicho punto
//esté dentro del dominio de P3
TDoublePoint TCilinder::RandomP3(void)
{
    TDoublePoint P3;
    TDoublePoint P_3;
    double r_3, theta_3;
    double theta_1, theta___3;

    do {
        //genera un punto en el dominio cartesiano
        P3.x = RandomUniform(getx3min(), getx3max());
        P3.y = RandomUniform(gety3min(), gety3max());

        //traduce a coordenadas cartesianas relativas a S1
        P_3 = S0recToS1rec(P3.x, P3.y);

        //traduce a coordenadas polares
        r_3 = Mod(P_3);
        theta_3 = Arg(P_3);

    } while(!AnglesToGoP_3(theta_1, theta___3, r_3, theta_3));

    //devuelve el punto generado
    return P3;
}

//genera un punto con distribución uniforme en
//el dominio cartesiano de P3 hasta que dicho punto
//esté dentro del dominio de P3 y lo asigna
void TCilinder::RandomizeP3(void)
{
    TDoublePoint P3;
    TDoublePoint P_3;
    double r_3, theta_3;
    double theta_1, theta___3;

    do {
        //genera un punto en el dominio cartesiano
        P3.x = RandomUniform(getx3min(), getx3max());
        P3.y = RandomUniform(gety3min(), gety3max());

        //traduce a coordenadas cartesianas relativas a S1
        P_3 = S0recToS1rec(P3.x, P3.y);

        //traduce a coordenadas polares
        r_3 = Mod(P_3);
        theta_3 = Arg(P_3);

    } while(!AnglesToGoP_3(theta_1, theta___3, r_3, theta_3));

    //asigna las posiciones
    SetAnglesRadians(theta_1, theta___3);
}

//---------------------------------------------------------------------------

} //namespace Models

//--------------------------------------------------------------------------

