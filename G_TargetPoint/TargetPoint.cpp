//---------------------------------------------------------------------------
//Archivo: TargetPoint.cpp
//Contenido: punto objetivo adscrito a un RP
//Última actualización: 27/09/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "TargetPoint.h"
#include "..\1_Mathematics\Geometry.h"
#include "..\2_Strings\Strings.h" //StrTrim
#include "..\2_Strings\StrPR.h" //StrReadInt

using namespace Strings;

//---------------------------------------------------------------------------

//espacio de nombres del programador de esposicionamiento
namespace Positioning {

//---------------------------------------------------------------------------
//TTargetPoint:
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//PROPIEDADES ESTÁTICAS:

//lista de objetos construidos
TItemsList<TTargetPoint*> TTargetPoint::Builts;

//---------------------------------------------------------------------------
//PROPIEDADES DE INTERFAZ:

AnsiString TTargetPoint::getTargetP3Text(void)
{
        return DPointToStr(TargetP3);
}
void TTargetPoint::setTargetP3Text(const AnsiString &S)
{
        try {
                TargetP3 = StrToDPoint(S);
        } catch(...) {
                throw;
        }
}

AnsiString TTargetPoint::getText(void)
{
        AnsiString S;

        S = IntToStr(getRoboticPositioner()->getActuator()->getId())+AnsiString(": ")+getTargetP3Text();

        return S;
}
void TTargetPoint::setText(const AnsiString&)
{
}

//---------------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//compara los identificadores de los posicionadores
//adscritos de dos puntos objetivo
int __fastcall TTargetPoint::CompareIds(TTargetPoint *TPA1, TTargetPoint *TPA2)
{
        //el puntero TPA1 no debe ser nulo
        if(TPA1 == NULL)
                throw EImproperArgument("pointer TPA1 should not be null");

        //el puntero TPA2 no debe ser nulo
        if(TPA2 == NULL)
                throw EImproperArgument("pointer TPA2 should not be null");

        if(TPA1->getRoboticPositioner()->getActuator()->getId() < TPA2->getRoboticPositioner()->getActuator()->getId())
                return -1;
        if(TPA1->getRoboticPositioner()->getActuator()->getId() > TPA2->getRoboticPositioner()->getActuator()->getId())
                return 1;
        return 0;
}
//este método debe ser apuntado en una lista de punteros
//para permitir el funcionamiento de
//los métodos de ordenación y comparación

//obtiene las etiquetas de las propiedades
//en formato fila de texto
AnsiString TTargetPoint::GetIdTargetP3LabelsRow(void)
{
        return AnsiString("Id\t")+TDoublePoint::xLabel+AnsiString("\t")+TDoublePoint::yLabel;
}

//atraviesa las etiquetas de las propiedades
//en una cadena de texto a partir de la posición i
void __fastcall TTargetPoint::TravelLabels(const AnsiString& S, int& i)
{
    try {
        //atraviesa los separadores hasta el próximo caracter no separador
        StrTravelSeparatorsIfAny(S, i);
        //lee la etiqueta de la propiedad Id
        StrTravelLabel(StrTrim(AnsiString("Id")), S, i);
        //atraviesa el separador obligatorio
        StrTravelSeparators(S, i);
        //lee la etiqueta de la propiedad x
        StrTravelLabel(StrTrim(TDoublePoint::xLabel), S, i);
        //atraviesa el separador obligatorio
        StrTravelSeparators(S, i);
        //lee la etiqueta de la propiedad y
        StrTravelLabel(StrTrim(TDoublePoint::yLabel), S, i);

    } catch(...) {
        throw;
    }
}

//lee los valores de las propiedades
//en una cadena de texto a partir de la posición i
void __fastcall TTargetPoint::ReadSeparated(int& Id, double& x, double& y,
                                      const AnsiString& S, int& i)
{
    //estado de lectura:
    //      0: esperando valor para Id
    //      1: esperando separador y valor para x
    //      2: esperando separador y valor para y
    //      3: punto objetivo leido con éxito
    int status = 0;

    //caracter indicado S[i]
    char c;

    //variables tampón en formato conveniente
    int _Id;
    double _x, _y;

    do {
        c = S[i]; //asigna el próximo caracter

        //reacciona según el estado y al caracter
        switch(status) {
        case 0: //esperando valor para Id
            try {
                //avanza el índice hasta el próximo caracter no separador
                StrTravelSeparatorsIfAny(S, i);
                //lee el valor para Id
                StrReadInt(_Id, S, i);
                //pasa a leer el valor para x
                status++;
            } catch(...) {
                throw;
            }
            break;
        case 1: //esperando separador y valor para x
            try {
                //atraviesa el separador obligatorio
                StrTravelSeparators(S, i);
                //lee el valor para x
                StrReadFloat(_x, S, i);
                //pasa a leer el valor para y
                status++;
            } catch(...) {
                throw;
            }
            break;
        case 2: //esperando separador y valor para y
            try {
                //atraviesa el separador obligatorio
                StrTravelSeparators(S, i);
                //lee el valor para y
                StrReadFloat(_y, S, i);
                //pasa a asignar la variable tampón
                status++;
            } catch(...) {
                throw;
            }
            break;
        }
    //mientras nos e haya leido el punto objetivo con éxito
    } while(status < 3);

    //asigna las propiedades
    Id = _Id;
    x = _x;
    y = _y;
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONSTRUCCIÓN Y DESTRUCCIÓN:

//construye un punto objetivo adscrito a un RP
//si el RP ya tiene un punto objetivo contruido
//lanza una excepción EImproperArgument
TTargetPoint::TTargetPoint(TRoboticPositioner *_RoboticPositioner,
        double x, double y) : Selected(false)//#, Color(Qt::red)
{
        //el puntero RoboticPositioner debería apuntar a un RP construido
        if(_RoboticPositioner == NULL)
                throw EImproperArgument("pointer RoboticPositioner should point to built fiber positioner");

        //el RP RoboticPositioner no debería estar adscrito a
        //un punto objetivo previamente construido
        for(int i=0; i<Builts.getCount(); i++)
                if(_RoboticPositioner == Builts[i]->getRoboticPositioner())
                        throw EImproperArgument("Fiber positioner FibeRPositioner should not be assigned to an previously built target point");

        //asigna los valores
        __RoboticPositioner = _RoboticPositioner;
        TargetP3.x = x;
        TargetP3.y = y;

        //apunta el nuevo punto objetivo a la lista de construidos
        Builts.Add(this);
}
TTargetPoint::TTargetPoint(TRoboticPositioner *_RoboticPositioner,
        TDoublePoint _TargetP3) : Selected(false)//#, Color(Qt::red)
{
        //el puntero RoboticPositioner debería apuntar a un RP construido
        if(_RoboticPositioner == NULL)
                throw EImproperArgument("pointer RoboticPositioner should point to built fiber positioner");

        //el RP RoboticPositioner no debería estar adscrito a
        //un punto objetivo previamente construido
        for(int i=0; i<Builts.getCount(); i++)
                if(_RoboticPositioner == Builts[i]->getRoboticPositioner())
                        throw EImproperArgument("Fiber positioner FibeRPositioner should not be assigned to an previously built target point");

        //asigna los valores
        __RoboticPositioner = _RoboticPositioner;
        TargetP3 = _TargetP3;

        //apunta el nuevo punto objetivo a la lista de construidos
        Builts.Add(this);
}
//destruye un punto objetivo
//si no hay ningún punto objetivo construido
//lanza una excepción EImproperCall
TTargetPoint::~TTargetPoint()
{
        //debe haber algún punto objetivo construido
        if(Builts.getCount() < 1)
                throw EImproperCall("should be some constructed target point");

        //busca el posicionador que se va a destruir en la lista de construidos
        int i = 0;
        while(i<Builts.getCount() && this!=Builts[i])
                i++;

        //si no lo encuentra
        if(i >= Builts.getCount())
                //indica que no se encuentra un punto objetivo previamente contruido
                throw EImposibleError("dont find a previously built target point");

        //borra el puntero de la lista
        Builts.Delete(i);
}

//---------------------------------------------------------------------------
//MÉTODOS DE COMPROBACIÓN:

//determina si el punto objetivo está fuera
//del dominio del punto P3 de su posicioandor adscrito
bool TTargetPoint::IsOutDomainP3(void)
{
        if(getRoboticPositioner()->getActuator()->PointIsOutDomainP3(TargetP3))
                return true;

        return false;
}
//determina si el punto objetivo queda dentro
//del área de seguridad del punto P3  de su posicionador adscrito
bool TTargetPoint::IsInSafeAreaP3(void)
{
        //determina si el punto objetivo está en el dominio del posicionador
        //adscrito y calcula las posiciones angulares de los ejes
        double theta_1, theta___3;
        bool isindomain = getRoboticPositioner()->getActuator()->AnglesToGoP3(theta_1, theta___3, TargetP3.x, TargetP3.y);

        //si no está en el dominio o no está dentro del área de seguridad
        if(!isindomain || getRoboticPositioner()->getActuator()->theta___3IsOutSafeArea(theta___3))
                return false; //indica que no está en el área de seguridad

        //indica que está en el área de seguridad
        return true;
}

//---------------------------------------------------------------------------
//MÉTODOS DE DESPLAZAMIENTO:

//randomiza el punto objetivo con distribución uniforme
//en el dominio del punto P3 de su posicioandor adscrito
void TTargetPoint::Randomize(void)
{
/*        double x, y; //coordenadas rectangulares
        TDoublePoint P_;
        double r_, theta_;
        double theta_1, theta__3;
        bool isindomain; //indicador de que el punto está dentro del dominio

        do {
                //radomiza las coordenadas cartesianas con distribución uniforme
                //en el  intervalo cartesiano del posicionador
                x = RandomUniform(RoboticPositioner->Actuator->x3min, RoboticPositioner->Actuator->x3max);
                y = RandomUniform(RoboticPositioner->Actuator->y3min, RoboticPositioner->Actuator->y3max);

                //traduce a coordenadas posicionales de los ejes
                //y determina si el punto está en el dominio
                isindomain = RoboticPositioner->PositionsToGoP3(theta_1, theta__3, x, y);

                //si está en el dominio
                if(isindomain) {
                        //asigna el punto objetivo
                        TargetP3.x = x;
                        TargetP3.y = y;
                }
        } while(!isindomain);*/
        TargetP3 = getRoboticPositioner()->getActuator()->RandomP3();
}

//asigna el punto objetivo al punto P3 de su posicionador adscrito
//si el punto objetivo no está en el dominio de
//su posicionador adscrito lanza EImpropercall
void TTargetPoint::MoveToTargetP3(void)
{
        //determina si el punto objetivo está en el dominio del posicionador
        //adscrito y calcula las posiciones angulares de los ejes
        double theta_1, theta___3;
        bool isindomain = getRoboticPositioner()->getActuator()->AnglesToGoP3(theta_1, theta___3, TargetP3.x, TargetP3.y);

        //el punto objetivo TargetP3 debería estar en el dominio del punto P3 de su RP adscrito
        if(!isindomain)
                throw EImproperCall("target point TargetP3 should be in the point P3 domain of his assigned fiber positioner");

        //asigna las posiciones a los ejes
        getRoboticPositioner()->getActuator()->SetAnglesRadians(theta_1, theta___3);
}

//---------------------------------------------------------------------------
//MÉTODOS DE INTERFAZ:

//indica que parte del punto objetivo
//puede ser agarrado en el punto indicado
//      1: punto (TargetP3, 0.75)
//      0: niguna;
int TTargetPoint::Grab(TDoublePoint P)
{
        //si está en el punto
        if(IntersectionPointCircle(P, TargetP3, 0.75))
                return 1; //agarra el punto

        return 0;
}
/*#
//imprime el puntos objetivo en el lienzo de la fotografía
//de un trazador de formas
void TTargetPoint::Paint(TPloterShapes *PS)
{
        //el puntero PS debería apuntar a un trazador de formas construido
        if(PS == NULL)
                throw EImproperArgument("pointer PS should point to built ploter shapes");

        //si el punto objetivo está seleccionado
        if(Selected) {
                //configura el color de la pluma
                if(double(Luminance(PS->getBackColor())) < 255.*1.5)
                                PS->setPenColor(Qt::white);
                else
                                PS->setPenColor(Qt::black);

                //dibuja el punto objetivo
                PS->Circunference(TargetP3, 0.61);
        }
        //si el punto objetivo no está seleccionado
        else {
                //dibuja el punto objetivo con su color correspondiente
                PS->setPenColor(Color);

                //dibuja el punto objetivo
                PS->Circunference(TargetP3, 0.61);
        }
}
*/
//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
