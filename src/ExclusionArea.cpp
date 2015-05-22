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
//File: ExclusionArea.cpp
//Content: class exclusion area
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

//#include <values.h>

#include "ExclusionArea.h"
#include "Strings.h" //StrIndent

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres de modelos
namespace Models {

//###########################################################################
//TExclusionArea
//###########################################################################

//---------------------------------------------------------------------------
//PROPIEDADES ESTÁTICAS:

//lista de objetos construidos
TItemsList<TExclusionArea*> TExclusionArea::Builts;

//---------------------------------------------------------------------------
//PROPIEDADES:

void TExclusionArea::setEo(double Eo)
{
        //el valor de Eo debe ser no negativo
        if(Eo < 0)
                throw EImproperArgument("Eo value should be nonnegative");

        p_Eo = Eo; //asignaelnuevo valor

        //asimila Eo
        calculateSPM();
}
void TExclusionArea::setEp(double Ep)
{
        //el valor de Ep debe ser no negativo
        if(Ep < 0)
                throw EImproperArgument("Ep value should be nonnegative");

        p_Ep = Ep; //asignaelnuevo valor

        //asimila Ep
        calculateSPM();
}

void TExclusionArea::setId(int Id)
{
        //el número de identificación Id debería ser mayor que cero
        if(Id <= 0)
                throw EImproperArgument("identification number Id should be upper zero");

        p_Id = Id; //asigna el nuevo valor
}

//PROPIEDADES EN FORMATO TEXTO:

AnsiString TExclusionArea::getEoText(void) const
{
        return FloatToStr(getEo());
}
void TExclusionArea::setEoText(const AnsiString &S)
{
        try {
                setEo(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}
AnsiString TExclusionArea::getEpText(void) const
{
        return FloatToStr(getEp());
}
void TExclusionArea::setEpText(const AnsiString &S)
{
        try {
                setEp(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}

AnsiString TExclusionArea::getIdText(void) const
{
        return IntToStr(getId());
}
void TExclusionArea::setIdText(const AnsiString& S)
{
        try {
                setId(StrToInt_(S));
        } catch(...) {
                throw;
        }
}

AnsiString TExclusionArea::getPendingText(void) const
{
        return BoolToStr(Pending, true);
}
void TExclusionArea::setPendingText(const AnsiString &S)
{
        try {
                Pending = StrToBool(S);
        } catch(...) {
                throw;
        }
}

//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TExclusionArea::getAllText(void) const
{
        AnsiString S;

        S += AnsiString("BarrierAddress: ")+getBarrierAddressText()+AnsiString("\r\n");
        S += AnsiString("Eo = ")+getEoText()+AnsiString("\r\n");
        S += AnsiString("Ep = ")+getEpText()+AnsiString("\r\n");
        S += AnsiString("Id = ")+getIdText()+AnsiString("\r\n");
        S += AnsiString("Adjacents: ")+Adjacents.getText()+AnsiString("\r\n");
        S += AnsiString("Pending = ")+getPendingText();

        return S;
}
AnsiString TExclusionArea::getInstanceText(void) const
{
        AnsiString S;

        S += AnsiString("BarrierInstance:\r\n")+StrIndent(Barrier.getInstanceText())+AnsiString("\r\n");
        S += AnsiString("Eo = ")+getEoText()+AnsiString("\r\n");
        S += AnsiString("Ep = ")+getEpText();

        return S;
}
void TExclusionArea::setInstanceText(const AnsiString &S)
{
        try {
                //contruye una variable tampón
                TExclusionArea aux(this);
                TExclusionArea *EA = &aux;
                //lee la instancia y la asigna a la variable tampón
                int i = 1;
                readInstance((TExclusionArea*&)EA, S, i);

                //avanza el índice i hasta la próxima posición que no contenga un separador
                StrTravelSeparatorsIfAny(S, i);
                //si el índice i indica a algún caracter de la cadena S
                if(i <= S.Length())
                        //indica que la cadena S solo debería contener el valor para una instancia
                        throw EImproperArgument("string S should contain the instance value only");

                //asigna la variable tampón
                copy(EA);

        } catch(...) {
                throw;
        }
}

AnsiString TExclusionArea::getOriginsText(void) const
{
        return getIdText()+AnsiString("\t")+Barrier.getP0().getRowText()+AnsiString("\t")+Barrier.getthetaO1Text();
}

//---------------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:


//compara los identificadores de dos EAs
int  TExclusionArea::compareIds(TExclusionArea *EA1, TExclusionArea *EA2)
{
        //el puntero EA1 debería apuntar a un área de exclusión construida
        if(EA1 == NULL)
                throw EImproperArgument("pointer EA1 should point a built exclusion area");

        //el puntero EA2 debería apuntar a un área de exclusión construida
        if(EA2 == NULL)
                throw EImproperArgument("pointer EA2 should point a built exclusion area");

        if(EA1->getId() < EA2->getId())
                return -1;
        if(EA1->getId() > EA2->getId())
                return 1;
        return 0;
}


//imprime el identificador de un EA
void  TExclusionArea::printId(AnsiString &S, TExclusionArea *EA)
{
        //el puntero EA debería apuntar a un área de exclusión construida
        if(EA == NULL)
                throw EImproperArgument("pointer EA should point to built exclusion area");

        S += EA->getIdText();
}

//obtiene las etiquetas de las propiedades de origen
//en formato fila de texto
AnsiString TExclusionArea::getOriginsLabelsRow(void)
{
        return "Id\tx0\ty0\tthetaO1";
}
//atraviesa las etiquetas de las propiedades de origen
//en formato fila de texto
void  TExclusionArea::travelOriginsLabelsRow(const AnsiString& S, int& i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de la máquina de estados de lectura
        //      0: esperando etiqueta "Id"
        //      1: esperando separador y etiqueta "x0"
        //      2: esperando separador y etiqueta "y0"
        //      3: esperando separador y etiqueta "thetaO1"
        //      4: etiquetas de las propiedades de origen atravesadas con éxito
        int status = 0;

        do {
                switch(status) {
                        case 0: //esperando etiqueta "Id"
                                try {
                                        StrTravelLabel("Id", S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 1: //esperando separador y etiqueta "x0"
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("x0", S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 2: //esperando separador y etiqueta "y0"
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("y0", S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 3: //esperando separador y etiqueta "thetaO1"
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("thetaO1", S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                }
        //mientras no se hayan atravesado lasetiquetas de las propiedades de origen con éxito
        } while(status < 4);
}

//imprime los valores de las propiedades de orien de un EA
//al final de una cadena de texto
//en formato fila de texto
void  TExclusionArea::printOriginsRow(AnsiString& S,
        TExclusionArea *EA)
{
        //el puntero EA debe apuntar a un área de exclusión construida
        if(EA == NULL)
                throw EImproperArgument("pointer C should bepoint to built exclusion area");

        S += EA->getOriginsText();
}
//lee los valores de las propiedades de orien para un EA
//desde la posición indicada de una cadena de texto
//en formato fila de texto
void  TExclusionArea::readOriginsRow(TExclusionArea *EA,
        const AnsiString& S, int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el puntero EA debe apuntar a un área de exclusión construida
        if(EA == NULL)
                throw EImproperArgument("pointer C should bepoint to built exclusion area");

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de la máquina de estados de lectura
        //      0: esperando valor para Id
        //      1: esperando separador y valor para x0
        //      2: esperando separador y valor para y0
        //      3: esperando separador y valor para thetaO1
        //      4: conjunto devalores leido con éxito
        int status = 0;

        //variables tampón
        int Id;
        double x0, y0;
        double thetaO1;

        do {
                switch(status) {
                        case 0: //esperando valor para Id
                                try {
                                        StrReadInt(Id, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 1: //esperando separador y valor para x0
                                try {
                                        StrTravelSeparators(S, i);
                                        StrReadFloat(x0, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 2: //esperando separador y valor para y0
                                try {
                                        StrTravelSeparators(S, i);
                                        StrReadFloat(y0, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 3: //esperando separador y valor para thetaO1
                                try {
                                        StrTravelSeparators(S, i);
                                        StrReadFloat(thetaO1, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                }
        //mientras no se haya leido el conjunto de valores con éxito
        } while(status < 4);

        //asigna las variables tampón
        try {
                EA->setOrigins(Id, x0, y0, thetaO1);
        }catch(...) {
                throw;
        }
}
//lee una instancia del EA
//desde la posición indicada de una cadena de texto
//en formato de asignaciones
void  TExclusionArea::readInstance(TExclusionArea* &EA,
        const AnsiString& S, int &i)
{
        //el puntero EA debe apuntar a una barrera construido
        if(EA == NULL)
                throw EImproperArgument("pointer EA shouldpoint to built exclusion area");

        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de la máquina de estados de lectura
        //      0: esperando etiqueta "BarrierInstance:"
        //      1: esperando instancia de Barrier
        //      2: esperando asignación a Eo
        //      3: esperando asignación a Ep
        //      4: instancia leida con éxito
        int status = 0;

        //variables tampón
        TExclusionArea t_EA(EA);

        do {
                switch(status) {
                        case 0: //esperando la etiqueta "BarrierInstance:"
                                try {
                                        StrTravelLabel("BarrierInstance:", S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 1: //esperando instancia de Barrier
                                try {
                                        TBarrier *aux = &t_EA.Barrier;
                                        TBarrier::readInstance((TBarrier*&)aux, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 2: //esperando asignación a Eo
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("Eo", S, i);
                                        StrTravelLabel("=", S, i);
                                        double Eo;
                                        StrReadFloat(Eo, S, i);
                                        t_EA.setEo(Eo);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 3: //esperando asignación a Ep
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("Ep", S, i);
                                        StrTravelLabel("=", S, i);
                                        double Ep;
                                        StrReadFloat(Ep, S, i);
                                        t_EA.setEp(Ep);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                }
        //mientras no se haya leido la instancia con éxito
        } while(status < 4);

        //asigna la variable tampón
        EA->copy(&t_EA);
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

//contruye un área de exclusión
//con los valores por defecto
TExclusionArea::TExclusionArea(void) :
        Barrier(TDoublePoint(0, 0), 0),
        p_Eo(MEGARA_Eo), p_Ep(MEGARA_Ep),
        p_Id(0),
        Adjacents(64, TRoboticPositioner::compareIds, NULL, NULL, TRoboticPositioner::printId, NULL),
        Pending(true)
{
        //asimilalas propiedades de inicialización
        calculateSPM();

        //añade el objeto a la lista de contruidos
        Builts.Add(this);
}
//construye un área de exclusión
//con los valores indicados
//si el número de identificación es menor que uno
//      lanza una execepción EImproperArgument
TExclusionArea::TExclusionArea(int Id, TDoublePoint P0, double thetaO1) :
        Barrier(P0, thetaO1),
        p_Eo(MEGARA_Eo), p_Ep(MEGARA_Ep),
        Adjacents(6, TRoboticPositioner::compareIds, NULL, NULL, TRoboticPositioner::printId, NULL),
        Pending(true)
{
        //el número de identificación Id debe ser mayor que cero
        if(Id < 1)
                throw EImproperArgument("identification number Id should be upper zero");

        //ADVERTENCIA: está permitida la duplicidad de números de identificación Id.

        //inicializa las propiedades
        p_Id = Id;

        //asimilalas propiedades de inicialización
        calculateSPM();

        //añade el objeto a la lista de contruidos
        Builts.Add(this);
}

//copia un área de exclusión
void TExclusionArea::copy(const TExclusionArea *EA)
{
        //el puntero EA debería apuntar a un área de exclusión contruida
        if(EA == NULL)
                throw EImproperArgument("pointer EA ahould point to built exclusion area");

        Barrier.copy(&EA->Barrier);
        p_Eo = EA->p_Eo;
        p_Ep = EA->p_Ep;
        p_Id = EA->p_Id;
        Adjacents.Clone(EA->Adjacents);
        Pending = EA->Pending;
}

//contruye un clon de un área de exclusión
TExclusionArea::TExclusionArea(const TExclusionArea *EA) :
        //construye un clon de la barrera
        Barrier(EA->Barrier)
{
        //el puntero EA debería apuntar a una barrera contruida
        if(EA == NULL)
                throw EImproperArgument("pointer EA ahould point to built barrier");

        //copia todas las propiedades
        copy(EA);

        //añade el objeto a la lista de contruidos
        Builts.Add(this);
}

//libera la memoria dinámica y borra el objeto de Builts
//si el objeto no está en Builts lanza EImproperCall
TExclusionArea::~TExclusionArea()
{
        //debe haber algún objeto para destruir
        if(Builts.getCount() < 1)
                throw EImproperCall("there isn't an object to destroy");

        //busca el objeto a destruir en la lista
        int i = 0;
        while(i<Builts.getCount() && Builts[i]!=this)
                i++;

        //si no ha encontrado el objeto
        if(i >= Builts.getCount())
                //indica que está intentando destruir un objeto no contruido
                throw EImproperCall(AnsiString("destruction attempt of non built object: ")+IntToHex(reinterpret_cast<intptr_t>(this), 8));

        //borra el puntero de la lista de construídos
        Builts.Delete(i);
}

//---------------------------------------------------------------------------
//MÉTODOS PARA CALCULAR LOS VALORES RECOMENDADOS DE
//LAS COMPONENTES DE SPM:

//Calcula el márgen perimetral de seguridad para evitar:
//- el error mecánico debido al mecanizado del actuador;
//- el error mecánico de orientación en el anclaje del posicionador;
//- el error mecánico de posicionamiento en el anclaje del posicionador;
//- el error numérico de las operaciones matemáticas del modelo;
//Todos los errores serán absorbidos por Eo y Ep
//en la siguiente fórmula:
//      SPMsta = Eo*r_max + Ep
//Donde:
//      Eo: margen de error de orientación en rad;
//      Ep: margen de error de posición en mm.
double TExclusionArea::SPMsta(void) const
{
        return getEo()*Barrier.getr_max() + getEp(); //calcula y devuelve el valor
}

//---------------------------------------------------------------------------
//MÉTODOS DE ASIGNACIÓN CONJUNTA:

//asigna las propiedades de origen
void TExclusionArea::setOrigins(int Id, double x0, double y0, double thetaO1)
{
        //el número de identificación Id debe ser mayor que cero
        if(Id <= 0)
                throw EImproperArgument("identificator number Idshould beupper zero");

        //asigna los nuevos valores
        p_Id = Id;

        //asimila (P0, thetaO1)
        Barrier.set(TDoublePoint(x0, y0), thetaO1);
}

//asigna conjuntamente las tolerancias
//      (Eo, Ep)
void TExclusionArea::setTolerances(double Eo, double Ep)
{
        //el valor de Eo debe ser no negativo
        if(Eo < 0)
                throw EImproperArgument("Eo value should be nonnegative");

        //el valor de Ep debe ser no negativo
        if(Ep < 0)
                throw EImproperArgument("Ep value should be nonnegative");

        p_Eo = Eo; //asignaelnuevo valor
        p_Ep = Ep; //asignaelnuevo valor

        //asimila (Eo, Ep)
        calculateSPM();
}

//---------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//A partir de:
//      (Eo, Ep)
//      r_max
//Determina:
//      (Barrier.SPM)
void TExclusionArea::calculateSPM(void)
{
        //CALULA LAS COMPONENTES DE SPM:

        Barrier.setSPM(SPMsta());
}

//---------------------------------------------------------------------------
//MÉTODOS DE COLISIÓN:

//determina los RPs adyacentes que pueden colisionar con la barrera
void TExclusionArea::determineAdjacents(const TItemsList<TRoboticPositioner*>& RPL)
{
        TRoboticPositioner *RP;
        TActuator *A;

        //por cada posicioandor de la lista de posicioandores
        for(int i=0; i<RPL.getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = RPL[i];
                //apunta el actuador del posicionador para facilitar su acceso
                A = RP->getActuator();
                //si el posicionador está lo bastante cerca como para colisionar con la barrera
                if(Barrier.getContour().distanceMin(A->getP0()) < Barrier.getSPM() + A->getr_max() + A->getArm()->getSPM())
                        //apunta el posicionador en la lista de adyacentes
                        Adjacents.Add(RP);
        }
}
//determina los RPs adyacentes en estado de colisión con la barrera
void TExclusionArea::searchCollinding(TItemsList<TRoboticPositioner*>& RPL)
{
        //inicializa la lista de punteros a actuadores
        RPL.Clear();

        TRoboticPositioner *RP;
        TActuator *A;

        //por cada actuador de la lista de adyacentes
        for(int i=0; i<Adjacents.getCount(); i++) {
                //apunta el posicionador para facilitar su acceso
                RP = Adjacents[i];
                //apunta el actuador del posicionador para facilitar su acceso
                A = RP->getActuator();
                //si el posicionador está lo bastante cerca como para colisionar con la barrera
                if(Barrier.getContour().collides(A->getArm()->getContour(), Barrier.getSPM() + A->getArm()->getSPM()))
                        RPL.Add(RP); //apunta el posicionador en la lista de actuadores
        }
}

//--------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR LAS COLISIONES
//CON POSICIONADORES ADYACENTES:

//determina si hay colisión con un axctuador
bool TExclusionArea::thereIsCollision(const TActuator* AA)
{
/*        //si la posición angular de ambos ejes de este posicionador es conocida
        if(PAkd != kdUnk) {
                //si la posición angular de ambos ejes del posicionador adyacente es conocida
                if(AA->PAkd != kdUnk) {
                        //calcula la distancia a partir de la cual los brazos de los actuadores
                        //están demasiado alejados para que sus brazos puedan colisionar
                        double Dmax = Arm->L1V + Arm->SPM + AA->Arm->L1V + AA->Arm->SPM;
                        //si los brazos de los actuadores no pueden colisionar
                        if(Mod(AA->Arm->P1 - Arm->P1) >= Dmax)
                                //indica que no hay colisión
                                return false;
                        //si los contornos están a una distancia menor que la suma de SPMs
                        if(Arm->Contour.DistanceMin(AA->Arm->Contour) < Arm->SPM + AA->Arm->SPM)
                                //indica que se ha encontrado colisión
                                return true;
                }
                //si la posición angular de algún eje del posicionador adyacente es desconocida
                else {
                        //calcula la distancia a partir de la cual los brazos de los actuadores
                        //están demasiado alejados para que sus brazos puedan colisionar
                        double Dmax = Arm->L1V + Arm->SPM + AA->Barrier->r_max + AA->Barrier->SPM;
                        //si los brazos de los actuadores no pueden colisionar
                        if(Mod(AA->Barrier->P0 - Arm->P1) >= Dmax)
                                //indica que no hay colisión
                                return false;
                        //si los contornos están a una distancia menor que la suma de SPMs
                        if(Arm->Contour.DistanceMin(AA->Barrier->Contour) < Arm->SPM + AA->Barrier->SPM)
                                //indica que se ha encontrado colisión
                                return true;
                }
        }
        //si la posición angular de algún eje de este posicionador es desconocida
        else {
                //si la posición angular de ambos ejes del posicionador adyacente es conocida
                if(AA->PAkd != kdUnk) {
                        //calcula la distancia a partir de la cual los brazos de los actuadores
                        //están demasiado alejados para que sus brazos puedan colisionar
                        double Dmax = Barrier->r_max + Barrier->SPM + AA->Arm->L1V + AA->Arm->SPM;
                        //si los brazos de los actuadores no pueden colisionar
                        if(Mod(AA->Arm->P1 - Barrier->P0) >= Dmax)
                                //indica que no hay colisión
                                return false;
                        //si los contornos están a una distancia menor que la suma de SPMs
                        if(Barrier->Contour.DistanceMin(AA->Arm->Contour) < Barrier->SPM + AA->Arm->SPM)
                                //indica que se ha encontrado colisión
                                return true;
                }
                //si la posición angular de algún eje del posicionador adyacente es desconocida
                else {
                        //calcula la distancia a partir de la cual los brazos de los actuadores
                        //están demasiado alejados para que sus brazos puedan colisionar
                        double Dmax = Barrier->r_max + Barrier->SPM + AA->Barrier->r_max + AA->Barrier->SPM;
                        //si los brazos de los actuadores no pueden colisionar
                        if(Mod(AA->Barrier->P0 - Barrier->P0) >= Dmax)
                                //indica que no hay colisión
                                return false;
                        //si los contornos están a una distancia menor que la suma de SPMs
                        if(Barrier->Contour.DistanceMin(AA->Barrier->Contour) < Barrier->SPM + AA->Barrier->SPM)
                                //indica que se ha encontrado colisión
                                return true;
                }
        }

        //indica que no ha encontrado colisión
        return false;*/
}
//determina si hay colisión con un actuador adyacente
bool TExclusionArea::thereIsCollisionWithAdjacent(void)
{
/*        TExclusionArea *AA;

        //por cada posicionador adyacente
        for(int i=0; i<Adjacents.Count; i++)
                //si hay colisión
                if(ThereIsCollision(Adjacents[i]->Actuator))
                        //indica que se está invadiendo el espacio de seguridad de algún adyacente
                        return true;

        //indica que no se invade el espacio de seguridad de ningún adyacente
        return false;*/
}

//Busca los posicionadores adyacentes cuyo
//brazo colisiona con la barrera de este área de exclusión.
void TExclusionArea::searchCollindingAdjacent(TItemsList<TActuator*> &Collindings)
{
/*        //vacia la lista de los que colisionan en congruencia con el estado inicial
        Collindings.Clear();

        TExclusionArea *AA;

        //busca la invasión con alguno de sus adyacentes
        for(int i=0; i<Adjacents.Count; i++) {
                //apunta el posicionador adyacente indicado para facilitar su acceso
                AA = (TExclusionArea*)Adjacents[i];
                //si el posicionador adyacente
                //      colisiona con el posicionador indicado
                if(ThereIsCollision(AA))
                        //añade el índice al posicionador adyacente
                        Collindings.Add(AA);
        }*/
}

//Determina si hay colisión con un actuador adyacente
//con evalauación de colisión pendiente.
bool TExclusionArea::thereIsCollisionWithPendingAdjacent(void)
{
/*        TExclusionArea *AA;

        //por cada posicionador adyacente
        for(int i=0; i<Adjacents.Count; i++) {
                //apunta el posicionador indicado para facilitar su acceso
                AA = (TExclusionArea*)Adjacents[i];
                //si el posicionador adyacente
                //      tiene pendiente la determinación de colisión,
                //      y colisiona con el posicionador indicado
                if(AA->Pending && ThereIsCollision(AA))
                        //indica que se ha encontrado colisión
                        return true;
        }

        //ADVERTENCIA: se excluirá la condición PendingCollisionDetermine
        //mintras no se demuestre que funciona bien.

        //indica que no se invade el espacio de seguridad de ningún adyacente
        return false;*/
}

//Busca los posicionadores adyacentes
//con evalauación de colisión pendiente
//cuyo brazo colisiona con la barrera de este área de exlusión.
void TExclusionArea::searchCollindingPendingAdjacent(TItemsList<TActuator*> &Collindings)
{
/*        //vacia la lista de los que colisionan en congruencia con el estado inicial
        Collindings.Clear();

        TExclusionArea *AA;

        //busca la invasión con alguno de sus adyacentes
        for(int i=0; i<Adjacents.Count; i++) {
                //apunta el posicionador adyacente indicado para facilitar su acceso
                AA = (TExclusionArea*)Adjacents[i];
                //si el posicionador adyacente
                //      tiene pendiente la determinación de colisión,
                //      y colisiona con el posicionador indicado
                if(AA->Pending && ThereIsCollision(AA))
                        //añade el índice al posicionador adyacente
                        Collindings.Add(AA);
        }*/
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

