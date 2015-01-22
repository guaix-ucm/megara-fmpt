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
//Archivo: ExclusionAreaList.cpp
//Contenido: lista de áreas de exclusión
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "ExclusionAreaList.h"
#include "Strings.h"

//#include <values.h>

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres de modelos
namespace Models {

//###########################################################################
//TExclusionAreaList
//###########################################################################

//PROPIEDADES DE LOCALIZACIÓN EN FORMATO TEXTO:

AnsiString TExclusionAreaList::getOText(void) const
{
        return DPointToStr(getO());
}

AnsiString TExclusionAreaList::getrmaxText(void) const
{
        return FloatToStr(getrmax());
}

/*AnsiString TExclusionAreaList::getxminText(void) const
{
        return FloatToStr(xmin);
}
AnsiString TExclusionAreaList::getxmaxText(void) const
{
        return FloatToStr(xmax);
}
AnsiString TExclusionAreaList::getyminText(void) const
{
        return FloatToStr(ymin);
}
AnsiString TExclusionAreaList::getymaxText(void) const
{
        return FloatToStr(ymax);
}
  */
//PROPIEDADES GRÁFICAS EN FORMATO TEXTO:

/*#AnsiString TExclusionAreaList::getLimitDomainColorText(void) const
{
        return ColorToString(LimitDomainColor);
}
void TExclusionAreaList::setLimitDomainColorText(const AnsiString &S)
{
        try {
                LimitDomainColor = StringToColor(S);
        } catch(...) {
                throw;
        }
}
*/
AnsiString TExclusionAreaList::getPaintExclusionAreas_Text(void) const
{
        return BoolToStr_(PaintExclusionAreas_);
}
void TExclusionAreaList::setPaintExclusionAreas_Text(const AnsiString &S)
{
        try {
                PaintExclusionAreas_ = StrToBool_(S);
        } catch(...) {
                throw;
        }
}
AnsiString TExclusionAreaList::getPaintLimitDomain_Text(void) const
{
        return BoolToStr_(PaintLimitDomain_);
}
void TExclusionAreaList::setPaintLimitDomain_Text(const AnsiString &S)
{
        try {
                PaintLimitDomain_ = StrToBool_(S);
        } catch(...) {
                throw;
        }
}

//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TExclusionAreaList::getOriginsTableText(void) const
{
        //guarda el valor de Print
        void ( *PrintBak)(AnsiString&, TExclusionArea*);
        PrintBak = Print;

        //apunta la lista de áreas de exclusión con un puntero no constante para facilitar su escritura
        TExclusionAreaList *EAL = (TExclusionAreaList*)this;

        EAL->Print = TExclusionArea::PrintOriginsRow;
        AnsiString S = EAL->getColumnText();

        //restaura el valor de Print
         EAL->Print = PrintBak;

         return S;
}
void TExclusionAreaList::setOriginsTableText(const AnsiString &S)
{
        try {
                //indica al principio de la cadena
                int i = 1;
                //atraviesa las cabeceras
                TActuator::TravelOriginsLabelsRow(S, i);

                //contruye una variable tampón
                TPointersList<TExclusionArea> EAL;
                //apunta la función de lectura de las propiedades de origen en formato linea de texto
                EAL.Read = TExclusionArea::ReadOriginsRow;
                //lee la cadena en la variable tampón
                TPointersList<TExclusionArea>::ReadSeparated(&EAL, S, i);

                //avanza el índice i hasta la próxima posición que no contenga un separador
                StrTravelSeparatorsIfAny(S, i);
                //si el índice i indica a algún caracter de la cadena S
                if(i <= S.Length())
                        //indica que la cadena S solo debería contener el valor para una instancia
                        throw EImproperArgument("string S should contain the instance value only");

                //asigna la variable tampón
                Destroy();
                for(int i=0; i<EAL.getCount(); i++)
                        Add(EAL.GetPointer(i));
                EAL.ClearWithoutDestroy();

        } catch(...) {
                throw;
        }
/*        try {
                //indica al principio de la cadena
                int i = 1;
                //atraviesa las cabeceras
                TActuator::TravelOriginsLabelsRow(S, i);

                //contruye una variable tampón
                TExclusionAreaList EAL(this);
                //apunta la función de lectura de las propiedades de origen en formato linea de texto
                EAL.Read = TExclusionArea::ReadOriginsRow;
                //lee la cadena en la variable tampón
                TExclusionAreaList::ReadSeparated(&EAL, S, i);

                //avanza el índice i hasta la próxima posición que no contenga un separador
                StrTravelSeparatorsIfAny(S, i);
                //si el índice i indica a algún caracter de la cadena S
                if(i <= S.Length())
                        //indica que la cadena S solo debería contener el valor para una instancia
                        throw EImproperArgument("string S should contain the instance value only");

                //asigna la variable tampón
                Clone(&EAL);

        } catch(...) {
                throw;
        }*/
}

AnsiString TExclusionAreaList::getLocationText(void) const
{
        AnsiString S;

        S += AnsiString("O = ")+getOText()+AnsiString("\r\n");

        S += AnsiString("rmax = ")+getrmaxText()+AnsiString("\r\n");
/*
        S += "xmin = "+xminText+"\r\n";
        S += "xmax = "+xmaxText+"\r\n";
        S += "ymin = "+yminText+"\r\n";
        S += "ymax = "+ymaxText;*/

        return S;
}
AnsiString TExclusionAreaList::getGraphicsText(void) const
{
        AnsiString S;

        //#S += AnsiString("LimitDomainColor: ")+getLimitDomainColorText()+AnsiString("\r\n");

        S += AnsiString("PaintExclusionAreas_: ")+getPaintExclusionAreas_Text()+AnsiString("\r\n");
        S += AnsiString("PaintLimitDomain_: ")+getPaintLimitDomain_Text();

        return S;
}

AnsiString TExclusionAreaList::getAllText(void) const
{
        AnsiString S;

        S += AnsiString("\r\nItemsAddress: ")+getItemsAddressText()+AnsiString("\r\n");
        S += AnsiString("\r\nLocation:\r\n")+StrIndent(getLocationText())+AnsiString("\r\n");
        S += AnsiString("\r\nGraphics:\r\n")+StrIndent(getGraphicsText());

        return S;
}
AnsiString TExclusionAreaList::getInstanceText(void) const
{
        AnsiString S;

        return S;
}
void TExclusionAreaList::setInstanceText(const AnsiString& S)
{
        try {
                //indica al principio de la cadena
                int i = 1;

                //contruye una variable tampón
                TExclusionAreaList EAL(this);
                //lee la cadena en la variable tampón
                TExclusionAreaList *aux = &EAL;
                ReadInstance((TExclusionAreaList*&)aux, S, i);

                //avanza el índice i hasta la próxima posición que no contenga un separador
                StrTravelSeparatorsIfAny(S, i);
                //si el índice i indica a algún caracter de la cadena S
                if(i <= S.Length())
                        //indica que la cadena S solo debería contener el valor para una instancia
                        throw EImproperArgument("string S should contain the instance value only");

                //asigna la variable tampón
                Clone(&EAL);

        } catch(...) {
                throw;
        }
}

//-------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//lee una instancia en una cadena
void  TExclusionAreaList::ReadInstance(TExclusionAreaList* &EAL,
        const AnsiString& S, int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el puntero EAL debería apauntar a una lista de posicionadores construida
        if(EAL == NULL)
                throw EImproperArgument("pointer EAL should point to built exclusion area list");

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de la máquina de estados de lectura
        //      0: instancia de lista de posicionadores leida con éxito
        int status = 0;

        //variables tampón

        do {
                switch(status) {
                }
        //mientras no se haya leido la instancia de la lista de posicionadores con éxito
        } while(status < 0);

        //asigna la variable tampón
        try {
        }catch(...) {
                throw;
        }
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

//construye una lista de áreas de exclusión
TExclusionAreaList::TExclusionAreaList(void) :
        TItemsList<TExclusionArea*>(2),
        //propiedades de interfaz
        //#LimitDomainColor(Qt::gray),
        PaintExclusionAreas_(true), PaintLimitDomain_(false)
{
        //inicializa las propiedades de localización
        __O.x = 0;
        __O.y = 0;

        //asimila las propiedades inicializadas
//        AssimilateSizingAndLocation();
}

//copia las propiedades de localización
//      (O, rmax)
void TExclusionAreaList::CopyLocation(const TExclusionAreaList *EAL)
{
        //el puntero EAL debería apuntar a una lista de áreas de exclusión construida
        if(EAL == NULL)
                throw EImproperArgument("pointer EAL should point to built exclusion area list");

        __O = EAL->getO();

        __rmax = EAL->getrmax();
/*
        __xmin = EAL->xmin;
        __xmax = EAL->xmax;
        __ymin = EAL->ymin;
        __ymax = EAL->ymax;*/
}
//copia las propiedades gráficas
//      (LimitDomainColor, PaintEclusionAreas_, PaintLimitDomain_)
void TExclusionAreaList::CopyGraphics(const TExclusionAreaList *EAL)
{
        //el puntero EAL debería apuntar a una lista de áreas de exclusión construida
        if(EAL == NULL)
                throw EImproperArgument("pointer EAL should point to built exclusion area list");

        //#LimitDomainColor = EAL->LimitDomainColor;
        PaintExclusionAreas_ = EAL->PaintExclusionAreas_;
        PaintLimitDomain_ = EAL->PaintLimitDomain_;
}

//clona una lista de áreas de exclusión
void TExclusionAreaList::Clone(const TExclusionAreaList *EAL)
{
        //el puntero EAL debe apuntar a una lista de áreas de exclusión construida
        if(EAL == NULL)
                throw EImproperArgument("pointers EAL should point to built exclusion area list");

        //inicializa la lista de áreas de exclusión
        Destroy();
        //clona las áreas de exclusión de la lista
//        Items.Clone(EAL->Items);
        for(int i=0; i<EAL->getCount(); i++)
                Add(new TExclusionArea(EAL->Get(i)));

        //apunta a las mismas funciones externas
        Compare = EAL->Compare;
        Evaluate = EAL->Evaluate;
        Assign = EAL->Assign;
        Print = EAL->Print;
        Read = EAL->Read;

        //copia las propiedades de localización
        CopyLocation(EAL);
        //copia las propiedades gráficas
        CopyGraphics(EAL);
}

//construye un clon de una lista de áreas de exclusión
TExclusionAreaList::TExclusionAreaList(const TExclusionAreaList *EAL)
{
        //el puntero EAL debe apuntar a una lista de áreas de exclusión construida
        if(EAL == NULL)
                throw EImproperArgument("pointers EAL should point to built exclusion area list");

        //copia todas las propiedades
        Clone(EAL);
}

//MÉTODOS DE DESTRUCCION DE ÁREAS DE EXCLUSIÓN:

//detruye el área de exclusión indicada de la lista
void TExclusionAreaList::Destroy(int i)
{
        //el índice i debe indicar un elemento de la lista
        if(i<0 || getCount()<=i)
                throw EImproperArgument("index i should point to list item");

        //destruye el área de exclusión indicada
        delete Items[i];

        //borra el puntero de la lista
        Delete(i);
}
//destruye todos las áreas de exclusión y vacia la lista
void TExclusionAreaList::Destroy(void)
{
        //por cada posición de la lista
        for(int i=0; i<getCount(); i++)
                //destruye el área de exclusión indicada
                delete Items[i];

        //vacia la lista
        Clear();
}
//destruye las áreas de exclusión seleccionados de la lista
int TExclusionAreaList::DestroySelected(void)
{
        TExclusionArea *EA;
        int count = 0;

        //por cada posición de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el área de exclusión indicada para facilitar su acceso
                EA = Items[i];
                //si el área de exclusión está seleccionado
                if(EA->Selected) {
                        //destruye el área de exclusión
                        Delete(i);
                        //contabiliza el área de exclusión seleccionado
                        count++;
                }
        }

        return count;
}

//MÉTODOS DE BÚSQUEDA DE ÁREAS DE EXCLUSIÓN:

//busca un área de exclusión en la lista de posicionadores
int TExclusionAreaList::Search(TExclusionArea *EA) const
{
        //debe auntar a un objeto construido
        if(EA == NULL)
                throw EImproperArgument("pointer EA should not be null");

        //mientras queden posiciones en la lista y
        //el puntero no apunte al mismo posicionador
        int i = 0;
        while(i<getCount() && EA!=Items[i])
                i++; //incrementa el índice

        return i; //devuelve el índice
}
//busca el primer posicionador con el identificador indicado
int TExclusionAreaList::SearchId(int Id) const
{
        //el número de identidad debe ser mayor que cero
        if(Id < 1)
                throw EImproperArgument("identity number Id should be upper zero");

        int i = 0;
        while(i<getCount() && Items[i]->getId()!=Id)
                        i++;

        return i;
}
//devuelve el puntero al primer posicionador
//con el identificador indicado
const TExclusionArea *TExclusionAreaList::SearchIdPointer(int Id) const
{
        //el número de identidad debe ser mayor que cero
        if(Id < 1)
                throw EImproperArgument("identifier Id should be upper zero");

        //busca la posición del área de exclusión Id
        int i = SearchId(Id);

        //el áreade exclusión debe estar en la lista
        if(i >= getCount())
                throw EImproperArgument("missing Id exclusion area");

        //devuelve el área de exclusión
        return Items[i];
}

//--------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//calcula el SPM de todas las áreas de exclusión
void TExclusionAreaList::CalculateSPM(void)
{
        //por cada área de exclusión de la lista
        for(int i=0; i<getCount(); i++)
                //calcula el SPM de SPM del área indicada
                Items[i]->CalculateSPM();
}

//determina los posicionadores que están lo bastante cerca
//de cada posicionador como para invadir su dominio de maniobra
void TExclusionAreaList::DetermineAdjacents(const TRoboticPositionerList& RPL)
{
        TExclusionArea *EA;
        const TRoboticPositioner *RP;
        int i, j;

        //para cada área de exclusión de la lista
        for(i=0; i<getCount(); i++) {
                //apunta el área de exclusión indicada para facilitar su acceso
                EA = Items[i];
                //inicializa la lista de adyacentes
                EA->Adjacents.Clear();
                //para cada uno de los posicionadores de la lista
                for(j=0; j<RPL.getCount(); j++) {
                        //apunta el posicionador indicado para facilitar su acceso
                        RP = RPL[j];
                        //si están lo bastante cerca como para poder colisionar
                        double aux1 = EA->Barrier.getContour().DistanceMin(RP->getActuator()->getP0());
                        double aux2 = RP->getActuator()->getr_max();
                        if(aux1 - aux2 < EA->Barrier.getSPM() + RP->getActuator()->getArm()->getSPM())
                                //añade el j-esimo posicionador a la lista de adyacentesdel área de exclusión
                                EA->Adjacents.Add((TRoboticPositioner*)RP);
                }
        }
}
//ordena las listas de posicionadores adyacentes en
//sentido levógiro empezando por el más próximo a 0
void TExclusionAreaList::SortAdjacents(void)
{
/*        TExclusionArea *EA;
        TRoboticPositioner *RPA;
                         +++
        //ninguno de los adyacentes debe ser el posicionador de referencia
        for(int i=0; i<Count; i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];
                //por cada posicionador adyacente
                for(int j=0; j<RP->Actuator->Adjacents.Count; j++) {
                        //apunta el posicionador indicado para facilitar su acceso
                        RPA = RP->Actuator->Adjacents[j];
                        //si el adyacente es el posicionador de referencia
                        if(RPA->Actuator->Id == RP->Actuator->Id)
                                throw EImproperCall("adjacent positioner should not be same the reference positioner");
                }
        }

        //define un par (RP, theta) para facilitar la ordenación
        class TPair {
        public:
                TRoboticPositioner *RP;
                double theta;

                //compara los ángulos de dos pares
                static int  Comparetheta(TPair *P1, TPair *P2) {
                        //debe apuntar a objetos construidos
                        if(P1 == NULL)
                                throw EImproperArgument("pointer P1 should not be null");
                        if(P2 == NULL)
                                throw EImproperArgument("pointer P2 should not be null");

                        if(P1->theta < P2->theta)
                                return -1;
                        if(P1->theta > P2->theta)
                                return 1;
                        return 0;
                }

                //imprime el ángulo al final de una cadena
                static void  Printtheta(AnsiString &S, TPair *P) {
                        S += FloatToStr(P->theta);
                }
        };

        //ADVERTENCIA: al ser una clase local el depurador no reconocerá
        //las propiedades de los pares.

        TPair *P;
        TItemsList<TPair*> LP(6, TPair::Comparetheta, NULL, NULL, TPair::Printtheta);

        //por cada posicionador de la lista
        for(int i=0; i<Count; i++) {
                //apunta el posicionador para facilitar su acceso
                RP = Items[i];

                //vacia la lista de punteros a pares
                LP.Clear();

                //por cada posicionador adyacente
                for(int j=0; j<RP->Actuator->Adjacents.Count; j++) {
                        //apunta el posicionador adyacente para facilitar su acceso
                        RPA = RP->Actuator->Adjacents[j];

                        //construye el par (RPA, theta)
                        //con theta en [PI/2, -3*PI/2)
                        P = new TPair;
                        P->RP = RPA;
                        TDoublePoint V = RPA->Actuator->P0 - RP->Actuator->P0;
                        if(Mod(V) != 0)
                                P->theta = ArgPos(V);
                        else
                                P->theta = 0;

                        //ADVERTENCIA: un posicionador adyacente puede estar
                        //ubicado en elmismo lugar que el de referencia.

                        //añade el par a la lista
                        LP.Add(P);
                }

                AnsiString Saux = LP.Text;

                //ordena la lista de posicionadores de mayor a menor
                if(LP.Count >= 2)
                        LP.SortInc(0, LP.Count-1);

                //transcribe la lista y destruye los pares
                RP->Actuator->Adjacents.Clear();
                for(int j=0; j<LP.Count; j++) {
                        P = LP[j];
                        RP->Actuator->Adjacents.Add(P->RP);
                        delete P;
                }
        }*/
}

//calcula las propiedades de localización:
//      (rmax, xmin, xmax, ymin, ymax)
void TExclusionAreaList::CalculateLocationParameters(void)
{
        //DETERMINA rmax:

        TExclusionArea *EA;
        double LO0;
        double _rmax;
        __rmax = 0; //inicializa la distancia máxima

        //por cada área de exclusión de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el área de exclusión indicada para facilitar su acceso
                EA = Items[i];

                //calcula el módulo al centro del posicionador
                LO0 = Mod(EA->Barrier.getP0() - getO());
                //calcula la distancia máxima a P3
                _rmax = LO0 + EA->Barrier.getr_max();
                //actualiza la distancia máxima
                if(_rmax > __rmax)
                        __rmax = _rmax;
/*
                //actualiza xmin
                if(EA->xmin < xmin)
                        __xmin = EA->xmin;
                //actualiza xmax
                if(EA->xmax > xmax)
                        __xmax = EA->xmax;
                //actualiza ymin
                if(EA->ymin < ymin)
                        __ymin = EA->ymin;
                //actualiza ymax
                if(EA->ymax > ymax)
                        __ymax = EA->ymax;*/
        }
}

//invoca a todos los métodos de asimilación de
//los parámetros de dimensionamiento y localización:
//      DetermineAdjacents
//      SortAdjacents
void TExclusionAreaList::AssimilateSizingAndLocation(const TRoboticPositionerList& RPL)
{
        DetermineAdjacents(RPL);
        SortAdjacents();
}

//asimila la configurración de posicionadores dada ejecutando:
//      CalculateSPMComponents();
//      AssimilateSizingAndLocation();
void TExclusionAreaList::Assimilate(const TRoboticPositionerList& RPL)
{
        CalculateSPM();
        AssimilateSizingAndLocation(RPL);
}

//--------------------------------------------------------------------------
//MÉTODOS DE DOMINIO CONJUNTO:

/*//determina el intervalo cartesiano que contiene
//el dominio de todos los posicionadores
//si el número de posicionadores de la lista es menor que uno
//lanza una excepcion EImproperCall
void TExclusionAreaList::GetDomain(double &xmin, double &xmax,
        double &ymin, double &ymax)
{
        //el número de posicionadores de la lista debería ser mayor que cero
        if(Count < 1)
                throw EImproperCall("number of positioners in the list should be upper zero");

        TExclusionArea *EA;

        //inicializa los límites
        xmin = MAXDOUBLE;
        ymin = MAXDOUBLE;
        xmax = -MAXDOUBLE;
        ymax = -MAXDOUBLE;

        //por cada posicionador de la lista
        for(int i=0; i<Count; i++) {
                //apunta el área de exclusión indicada para facilitar su acceso
                EA = Items[i];

                //actualiza los límites
                if(EA->xmin < xmin)
                        xmin = EA->xmin;
                //actualiza los límites
                if(EA->ymin < ymin)
                        ymin = EA->ymin;
                //actualiza los límites
                if(EA->xmax > xmax)
                        xmax = EA->xmax;
                //actualiza los límites
                if(EA->ymax > ymax)
                        ymax = EA->ymax;
        }
}                                       */
//determina si un punto se encuentra dentro del círculo
//que contiene el dominio conjunto de los posicionadores
bool TExclusionAreaList::IsInCircle(const TDoublePoint &P)
{
        if(Mod(P - getO()) > getrmax())
                return true;
        return false;
}
/*//determina si un punto se encuentra dentro del cuadrado
//que contiene el dominio conjunto de los posicionadores
bool TExclusionAreaList::IsInSquare(const TDoublePoint &P)
{
        double x = P.x - O.x;
        double y = P.y - O.y;
        if(x<xmin || xmax<x ||y<ymin || ymax<y)
                return false;
        return true;
} */

//--------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR SI HAY COLISIONES:

//levanta las banderas indicadoras de determinación de colisión
//pendiente de todos los posicionadores d ela lista
void TExclusionAreaList::EnablePending(void)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->Pending = true;
}
//determina si algún brazo de algún posicionador
//colisiona con el brazo de algún posicionador adyacente
bool TExclusionAreaList::ThereIsCollision(void)
{
        TExclusionArea *EA;

        //levanta las banderas de determinación de colisión pendiente
        EnablePending();

        //busca colisión del brazo de cada posicionador con el brazo de
        //sus adyacentes exceptuando aquellos con los que se ha comprobado ya
        for(int i= 0; i<getCount(); i++) {
                //apunta el áreadeexclusión indicada para facilitar su acceso
                EA = Items[i];
                //si la barrera del área de exclusión colisiona con
                //el brazo de algún posicionador adyacente con
                //determinación de colisión pendiente
                if(EA->ThereIsCollisionWithPendingAdjacent())
                        //indica que hay colisión
                        return true;
                else
                        //desactiva la bandera para evitar que
                        //se vuelva a determinar la colisión
                        EA->Pending = false;
        }

        return false; //indica que no se ha encontrado invasión
}
//busca las áreas de exclusión de la lista cuya barrera colisiona con
//el brazo de algún posicionador adyacente
void TExclusionAreaList::SearchCollinding(TVector<int> &indices)
{
/*        //ADVERTENCIA: los posicionadores de la lista no tienen
        //por que estar ordenados según su identificador.

        TRoboticPositioner *RP;
        TItemsList<TActuator*> Collindings;

        //limpia la lista de índices en congruencia con sus estado inicial
        indices.Clear();

        //levanta las baderas de colisión de todos los posicionadores
        EnablePending();

        //por cada posicionador de la lista
        for(int i= 0; i<Count; i++) {
                //si el posicionador indicado no está en la lista
                if(indices.Search(i) >= indices.Count) {

                        //apunta el posicionador indicado para facilitar su acceso
                        RP = Items[i];
                        //busca colisiones con los posicionadores adyacentes
                        RP->Actuator->SearchCollindingPendingAdjacent(Collindings);
                        //indica que ya ha determinado el estado de colisión del brazo del posicionador
                        RP->Actuator->Pending = false;

                        //si ha encontrado colisión con algún adyacente
                        if(Collindings.Count > 0) {

                                //añade el índice al posicionador adscrito indicado
                                indices.Add(i);

                                int m, n;

                                //añade los índices a los posicionadores adyacentes
                                //que tengan un posicionador adscrito en la lista y
                                //no hayan sido añadidos previamente
                                for(int j=0; j<Collindings.Count; j++) {
                                        //apunta el posicionador indicado para facilitar su acceso
                                        RP = (TRoboticPositioner*)Collindings[j];

                                        //busca la pósición del controlador adscrito
                                        m = Search(RP);

                                        //busca el índice en la lista de añadidos
                                        n = indices.Search(m);

                                        //si el posicionador está en la lista y
                                        //su posición no ha sido previamente agregada
                                        if(m<Count && n>=indices.Count)
                                                //añade el el índice a la lista de añadidos
                                                indices.Add(m);
                                }
                        }
                }
        }

        //orde la lista de índices
        if(indices.Count > 1)
                indices.SortInc(0, indices.Count-1);*/
}
//obtiene los conjuntos de posicionadores en colisión en la exposición indicada
void TExclusionAreaList::GetCollisionClusterList(TPointersList<TItemsList<TRoboticPositioner*> > &CCL)
{
/*        TRoboticPositioner *RP;
        TItemsList<TActuator*> Collindings;
        TItemsList<TRoboticPositioner*> *CC;

        //inicializa la lista de conjuntos de posicionadores en colisión
        CCL.Clear();

        //levanta las banderas indicadoras de determinación de colisión
        //pendiente de todos los posicionadores de la lista
        EnablePending();

        //por cada posicionador de la lista
        for(int i=0; i<Count; i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                //busca los posicionadores adyacentes cuyo brazo colisiona con el del posicionador central
                RP->Actuator->SearchCollindingPendingAdjacent(Collindings);

                //si ha encontrado alguna nueva colisión
                if(Collindings.Count > 0) {
                        //contruye una lista de punteros a posicionadores
                        CC = new TItemsList<TRoboticPositioner*>();
                        //apunta el posicionador y los colindantes en la lista
                        CC->Add(RP);
                        //desactiva la bandera del posicionador
                        RP->Actuator->Pending = false;
                        //por cada posicionador colindante en colisión
                        for(int j=0; j<Collindings.Count; j++) {
                                //apunta el posicionador para facilitar su acceso
                                RP = (TRoboticPositioner*)Collindings[j];
                                //apunta el posicionador en el conjunto
                                CC->Add(RP);
                                //desactiva la bandera del posicionador
                                RP->Actuator->Pending = false;
                        }
                        //añade la nueva lista al resultado
                        CCL.Add(CC);
                }
        }*/
}

//-------------------------------------------------------------------
//MÉTODOS GRÁFICOS:

//establece el estado de la bandera de pintado
//del cuerpo de los posicionadores
void TExclusionAreaList::SetPaintBody(bool b)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->PaintBody = b;
}

/*#//asigna un color a todos los posicionadores de la lista
void TExclusionAreaList::SetAllColors(QColor Color)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->Color = Color;
}*/
//selecciona todos los posicionadores de la lista
void TExclusionAreaList::SelectAll(void)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->Selected = true;
}
//deselecciona todos los posicionadores de la lista
void TExclusionAreaList::DeselectAll(void)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->Selected = false;
}

/*#//imprime la lista de posicionadores en el lienzo de la fotografía
//con los colores indicados
//imprime el lienzo de la fotografía en el lienzo de la caja de pintura
void TExclusionAreaList::PaintExclusionAreas(TPloterShapes *PS)
{
        //el puntero PS debería apuntar a un trazador de formas construido
        if(PS == NULL)
                throw EImproperArgument("pointer PS should point to built ploter shapes");

        //imprime los posicionadores de la lista
        for(int i=0; i<getCount(); i++)
                Items[i]->Paint(PS);
}
//dibuja el límite circular del dominio conjunto de los posicionadores
void TExclusionAreaList::PaintLimitDomain(TPloterShapes *PS)
{
        //configura el color de la pluma
        PS->setPenColor(LimitDomainColor);
        PS->Circunference(getO(), getrmax());

}
*/
//busca el primer área de exclusión que está bajo el punto P
//y que parte del área de exclusión puede ser agarrado en
//el punto indicado:
//      i: índice al área de exclusión bajo el punto P;
//      dominio i: [0, Count];
//      n: parte del área de exclusión agarrada;
//      valores posibles:
//      0: niguna;
//      1: alguna;
//valores de retorno:
//      false: ninguna parte agarrada
//      true: alguna parte agarrada
bool TExclusionAreaList::Grab(int &i, int &n, TDoublePoint P)

{
        for(i=0; i<getCount(); i++) {
                n = Items[i]->Grab(P);
                if(n > 0)
                        return true;
        }

        return false;
}

/*#//dibuja la lista de áreas de exclusión en un trazador de formas
void TExclusionAreaList::Paint(TPloterShapes *PS)
{
        try {
                if(PaintExclusionAreas_)
                        PaintExclusionAreas(PS);
                if(PaintLimitDomain_)
                        PaintLimitDomain(PS);
        } catch(...) {
                throw;
        }
}
*/
//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

