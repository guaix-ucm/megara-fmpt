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
/// @file ExclusionAreaList.cpp
/// @brief EA list
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#include "ExclusionAreaList.h"
#include "Strings.h"
#include "RoboticPositionerList3.h"

using namespace Strings;

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//CLASE LISTA DE AREAS DE EXCLUSION:

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
        return FloatToStr(getxmin());
}
AnsiString TExclusionAreaList::getxmaxText(void) const
{
        return FloatToStr(getxmax());
}
AnsiString TExclusionAreaList::getyminText(void) const
{
        return FloatToStr(getymin());
}
AnsiString TExclusionAreaList::getymaxText(void) const
{
        return FloatToStr(getymax());
}
*/
//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

//tabla de orígenes de coordenadas:
//    Id      x0      y0      thetaO1
AnsiString TExclusionAreaList::getOriginsTableText(void) const
{
    //print the header
    string str = "# A table for indicate the position and orientation of each EA (Exclusion Area):";
    str += "\r\n#     Id: identifier of the EA (a nonnegative integer number)";
    str += "\r\n#     x0: abscissa of the point P0 of the EA in S0 (in mm)";
    str += "\r\n#     y0: ordinate of the point P0 of the EA in S0 (in mm)";
    str += "\r\n#     thetaO1: orientation of S1 in S0 (in rad)";

    //print the table
    str += "\r\n\r\n";
    str += TExclusionArea::getOriginsLabelsRow().str;
    void ( *PrintBak)(AnsiString&, TExclusionArea*);
    PrintBak = Print;
    TExclusionAreaList *EAL = (TExclusionAreaList*)this;
    EAL->Print = TExclusionArea::printOriginsRow;
    str += "\r\n";
    str += EAL->getColumnText().str;
    EAL->Print = PrintBak;

    return AnsiString(str);
}
void TExclusionAreaList::setOriginsTableText(const AnsiString &S)
{
    //indica al principio de la cadena
    int i = 1;

    try {
        //atraviesa las cabeceras
        TExclusionArea::travelOriginsLabelsRow(S, i);

        //contruye una variable tampón
        TPointersList<TExclusionArea> EAL;
        //apunta la función de lectura de las propiedades de origen en formato linea de texto
        EAL.Read = TExclusionArea::readOriginsRow;
        //lee la cadena en la variable tampón
        TPointersList<TExclusionArea>::ReadSeparated(&EAL, S, i);

        //busca texto inesperado
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text '"+StrFirstChars(S.SubString(i, S.str.length() - i + 1)).str);

        //asigna la variable tampón
        Destroy();
        for(int i=0; i<EAL.getCount(); i++)
            Add(EAL.GetPointer(i));
        EAL.ClearWithoutDestroy();

        //Notese que el codigo:
        //  for(int i=0; i<EAL.getCount(); i++)
        //      Add(EAL.GetPointer(i));
        //difiere del metodo clone en que no copia los punteros a funciones externas.

    } catch(Exception& E) {
        //determine the coordinates (row, col) of the position i of the string S
        unsigned int row, col;
        strPositionToCoordinates(row, col, S.str, i-1);
        //indicates that the name of this function, the row and the column
        E.Message.Insert(1, "setting origins table text, row "+inttostr(row)+" and column "+inttostr(col)+": ");
        throw;
    }
}

//conjunto de propiedades de localización
//en formato asignaciones
AnsiString TExclusionAreaList::getLocationText(void) const
{
    string str;

    str += "O = "+getOText().str+"\r\n";

    str += "rmax = "+getrmaxText().str;

    /*    str += "xmin = "+getxminText().str+"\r\n";
    str += "xmax = "+getxmaxText().str+"\r\n";
    str += "ymin = "+getyminText().str+"\r\n";
    str += "ymax = "+getymaxText().str;
*/
    return AnsiString(str);
}

//conjunto de todas las propiedades
//en formato de asignaciones
AnsiString TExclusionAreaList::getAllText(void) const
{
    string str;

    str += "\r\nItemsAddress: "+getItemsAddressText().str+"\r\n";
    str += "\r\nLocation:\r\n"+StrIndent(getLocationText()).str;

    return AnsiString(str);
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

//construye una lista de EAs
TExclusionAreaList::TExclusionAreaList(void) :
    TItemsList<TExclusionArea*>(2),
    p_O(0, 0)
{
    //asimila las propiedades inicializadas
    assimilate();
}

//copia las propiedades de localización
//    (O, rmax, xmin, xmax, ymin, ymax)
void TExclusionAreaList::copyLocation(const TExclusionAreaList *EAL)
{
    //el puntero EAL debería apuntar a una lista de EAs construida
    if(EAL == NULL)
        throw EImproperArgument("pointer EAL should point to built exclusion area list");

    p_O = EAL->getO();

    p_rmax = EAL->getrmax();
    /*
    p_xmin = EAL->getxmin();
    p_xmax = EAL->getxmax();
    p_ymin = EAL->getymin();
    p_ymax = EAL->getymax();*/
}

//clona una lista de EAs
void TExclusionAreaList::Clone(const TExclusionAreaList *EAL)
{
    //el puntero EAL debe apuntar a una lista de EAs construida
    if(EAL == NULL)
        throw EImproperArgument("pointers EAL should point to built exclusion area list");

    //inicializa la lista de EAs
    Destroy();
    //clona las EAs de la lista
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
    copyLocation(EAL);
}

//construye un clon de una lista de EAs
TExclusionAreaList::TExclusionAreaList(const TExclusionAreaList *EAL) :
    TItemsList<TExclusionArea*>(this)
{
    //el puntero EAL debe apuntar a una lista de EAs construida
    if(EAL == NULL)
        throw EImproperArgument("pointers EAL should point to built exclusion area list");

    //clona todas las propiedades
    Clone(EAL);
}

//MÉTODOS DE DESTRUCCION DE EAs:

//detruye el EA indicada de la lista
void TExclusionAreaList::Destroy(int i)
{
    //el índice i debe indicar un elemento de la lista
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i should point to list item");

    //destruye el EA indicada
    delete Items[i];

    //borra el puntero de la lista
    Delete(i);
}
//destruye todas las EAs de la lista
void TExclusionAreaList::Destroy(void)
{
    //por cada posición de la lista
    for(int i=0; i<getCount(); i++)
        //destruye el EA indicada
        delete Items[i];

    //vacia la lista
    Clear();
}

//MÉTODOS DE BÚSQUEDA DE EAs:

//busca el primer puntero a EA
//con la dirección en memoria indicada
int TExclusionAreaList::search(TExclusionArea *EA) const
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
//busca el primer índice a EA
//con el identificador indicado
int TExclusionAreaList::searchId(int Id) const
{
    //el número de identidad debe ser mayor que cero
    if(Id < 1)
        throw EImproperArgument("EA identifier number Id should be upper zero");

    int i = 0;
    while(i<getCount() && Items[i]->getId()!=Id)
        i++;

    return i;
}
//busca el primer puntero a EA
//con el identificador indicado
const TExclusionArea *TExclusionAreaList::searchIdPointer(int Id) const
{
    //el número de identidad debe ser mayor que cero
    if(Id < 1)
        throw EImproperArgument("identifier Id should be upper zero");

    //busca la posición del EA por su Id
    int i = searchId(Id);

    //el áreade exclusión debe estar en la lista
    if(i >= getCount())
        throw EImproperArgument("missing Id exclusion area");

    //devuelve el puntero al EA
    return Items[i];
}

//--------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//calcula el SPM de todas las EAs
void TExclusionAreaList::calculateSPM(void)
{
    //por cada EA de la lista
    for(int i=0; i<getCount(); i++)
        //calcula el SPM de SPM del área indicada
        Items[i]->calculateSPM();
}

//calcula las propiedades de localización:
//    (rmax, xmin, xmax, ymin, ymax)
void TExclusionAreaList::calculateLocationParameters(void)
{
    //DETERMINA rmax:

    TExclusionArea *EA;
    double LO0;
    double rmax;
    p_rmax = 0; //inicializa la distancia máxima

    //por cada EA de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el EA indicada para facilitar su acceso
        EA = Items[i];

        //calcula el módulo al centro del posicionador
        LO0 = Mod(EA->Barrier.getP0() - getO());
        //calcula la distancia máxima a P3
        rmax = LO0 + EA->Barrier.getr_max();
        //actualiza la distancia máxima
        if(rmax > p_rmax)
            p_rmax = rmax;

        /*        //actualiza xmin
        if(EA->xmin < xmin)
            p_xmin = EA->xmin;
        //actualiza xmax
        if(EA->xmax > xmax)
            p_xmax = EA->xmax;
        //actualiza ymin
        if(EA->ymin < ymin)
            p_ymin = EA->ymin;
        //actualiza ymax
        if(EA->ymax > ymax)
            p_ymax = EA->ymax;*/
        /*        getDomain(p_xmin, p_xmax, p_ymin, p_ymax);*/
    }
}

//determina los RPs que están lo bastante cerca
//de cada posicionador como para invadir su perímetro de seguridad
void TExclusionAreaList::determineAdjacents(const TRoboticPositionerList& RPL)
{
    //por cada EA de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el EA indicado para facilitar su acceso
        TExclusionArea *EA = Items[i];

        //inicializa la lista de RPs adyacentes
        EA->AdjacentRPs.Clear();

        //para cada una de los RPs de la lista
        for(int j=0; j<RPL.getCount(); j++) {
            //apunta el RP indicada para facilitar su acceso
            TRoboticPositioner *RP = RPL[j];
            //si están lo bastante cerca como para colisionar
            if(Mod(RP->getActuator()->getP0() - EA->Barrier.getP0()) <
                    (RP->getActuator()->getr_max() + RP->getActuator()->getSPMall_a() +
                     EA->Barrier.getr_max() + EA->Barrier.getSPM()) + ERR_NUM) {
                //añade el RP a la lista de RPs adyacentes
                EA->AdjacentRPs.Add(RP);
            }
        }
    }
}
//ordena las listas de RPs adyacentes en
//sentido levógiro empezando por el más próximo a 0
void TExclusionAreaList::sortAdjacents(void)
{
    TPairTD<TRoboticPositioner> *P;
    TPointersList<TPairTD<TRoboticPositioner> > LP(10, TPairTD<TRoboticPositioner>::Comparex,
                                                   NULL, NULL, TPairTD<TRoboticPositioner>::Printx);

    //por cada EA de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el EA indicado para facilitar su acceso
        TExclusionArea *EA = Items[i];

        //vacia la lista de punteros a pares
        LP.Clear();

        //por cada posicionador adyacente
        for(int j=0; j<EA->AdjacentRPs.getCount(); j++) {
            //apunta el posicionador adyacente indicado para facilitar su acceso
            TRoboticPositioner *RPA = EA->AdjacentRPs[j];

            //construye el par (RPA, theta)
            //con theta en [0, 2*M_PI)
            P = new TPairTD<TRoboticPositioner>(RPA);
            TDoublePoint V = RPA->getActuator()->getP0() - EA->Barrier.getP0();
            if(Mod(V) != 0)
                P->x = ArgPos(V);
            else
                P->x = 0;

            //ADVERTENCIA: un posicionador adyacente puede estar
            //ubicado en el mismo lugar que el de referencia.

            //añade el par a la lista
            LP.Add(P);
        }

        //ordena la lista de posicionadores de menor a mayor ángulo
        if(LP.getCount() > 1)
            LP.SortInc(0, LP.getCount()-1);

        //transcribe la lista
        EA->AdjacentRPs.Clear();
        for(int j=0; j<LP.getCount(); j++)
            EA->AdjacentRPs.Add(LP[j].P);
    }
}

//asimila la configurración de posicionadores dada ejecutando:
//  calculateSPM();
//  calculateLocationParameters();
//  determineAdjacents(RPL)
//  sortAdjacents();
void TExclusionAreaList::assimilate(void)
{
    calculateSPM();
    calculateLocationParameters();
}

//--------------------------------------------------------------------------
//MÉTODOS DE DOMINIO CONJUNTO:

/*//determina el intervalo cartesiano que contiene
//el conjunto de todas las EAs
//si el número de EAs de la lista es menor que uno:
//    lanza una excepcion EImproperCall
void TExclusionAreaList::getDomain(double &xmin, double &xmax,
                                   double &ymin, double &ymax)
{
    //el número de posicionadores de la lista debería ser mayor que cero
    if(getCount() < 1)
        throw EImproperCall("number of positioners in the list should be upper zero");

    TExclusionArea *EA;

    //inicializa los límites
    xmin = DBL_MAX;
    ymin = DBL_MAX;
    xmax = -DBL_MAX;
    ymax = -DBL_MAX;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el EA indicada para facilitar su acceso
        EA = Items[i];

        //actualiza los límites
        if(EA->getxmin() < xmin)
            xmin = EA->getxmin();
        if(EA->getymin() < ymin)
            ymin = EA->getymin();
        if(EA->getxmax() > xmax)
            xmax = EA->getxmax();
        if(EA->getymax() > ymax)
            ymax = EA->getymax();
    }
}
*/
//determina si un punto se encuentra dentro del círculo
//que contiene el dominio conjunto de las EAs
bool TExclusionAreaList::isInCircle(const TDoublePoint &P)
{
    if(Mod(P - getO()) > getrmax())
        return true;
    return false;
}
/*//determina si un punto se encuentra dentro del intervalo carteasiano
//que contiene el dominio conjunto de las EAs
bool TExclusionAreaList::isInSquare(const TDoublePoint &P)
{
        double x = P.x - O.x;
        double y = P.y - O.y;
        if(x<getxmin() || getxmax()<x || y<getymin() || getymax()<y)
                return false;
        return true;
}*/

//--------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR SI HAY COLISIONES:

//levanta las banderas indicadoras de determinación de colisión
//pendiente de todos los posicionadores de la lista
void TExclusionAreaList::enableAllPending(void)
{
    for(int i=0; i<getCount(); i++)
        Items[i]->Pending = true;
}
/*//configura el estado de colisión
//de todos los posicionadores de la lista
void TExclusionAreaList::setAllCollision(bool Collision)
{
    for(int i=0; i<getCount(); i++)
        Items[i]->Collision = Collision;
}*/

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

