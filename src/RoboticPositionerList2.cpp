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
//File: RoboticPositionerList2.cpp
//Content: RP list with hash table
//Last update: 13/02/2015
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "RoboticPositionerList2.h"
#include "Scalars.h"

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//ASSIMILATION METHODS:

//calculates the sizing of a cell (qx, qy)
void TRoboticPositionerList2::CalculateSize(void)
{
    __qx = (getxmax() - getxmin())/double(getJ());
    __qy = (getymax() - getymin())/double(getI());
}

//---------------------------------------------------------------------------
//MAPPING INTERVAL:

void TRoboticPositionerList2::setxmin(double _xmin)
{
    //check the precondition
    if(_xmin >= getxmax())
        throw EImproperArgument("lower limit xmin hould be less upper limit xmax");

    //assign and assimilates the new value
    __xmin = _xmin;
    __qx = (getxmax() - getxmin())/double(getJ());
}
void TRoboticPositionerList2::setxmax(double _xmax)
{
    //check the precondition
    if(_xmax <= getxmin())
        throw EImproperArgument("upper limit xmax should be upper to lower limit xmin");

    //assign and assimilates the new value
    __xmax = _xmax;
    __qx = (getxmax() - getxmin())/double(getJ());
}
void TRoboticPositionerList2::setymin(double _ymin)
{
    //el límite inferior ymin debería ser inferior al límite superior ymax
    if(_ymin >= getymax())
        throw EImproperArgument("lower limit ymin should be less upper limit ymax");

    //assign and assimilates the new value
    __ymin = _ymin;
    __qy = (getymax() - getymin())/double(getI());
}
void TRoboticPositionerList2::setymax(double _ymax)
{
    //el límite superior ymin debería ser inferior al límite inferior ymax
    if(_ymax <= getymin())
        throw EImproperArgument("upper limit ymax should be upper to lower limit ymin");

    //assign and assimilates the new value
    __ymax = _ymax;
    __qy = (getymax() - getymin())/double(getI());
}

//---------------------------------------------------------------------------
//CELLS OF THE MAP:

void TRoboticPositionerList2::setI(int _I)
{
    //cheack the precondition
    if(_I < 1)
        throw EImproperArgument("cells matrix's number rows I should be upper zero");

    //capture the original value
    double iini = getI();

    //assign the new value
    Cells.setCount(_I);

    //build the new rows
    for(int i=iini; i<getI(); i++) {
        Cells[i].setCount(getJ());
        for(int j=0; j<getJ(); j++)
            Cells[i][j].Print = TRoboticPositioner::PrintId;
    }

    //assimilates the new value
    __qy = (getymax() - getymin())/double(getI());
}
void TRoboticPositionerList2::setJ(int _J)
{
    //cheack the precondition
    if(_J < 1)
        throw EImproperArgument("cells matrix's number columns J should be upper zero");

    //capture the original value
    double jini = getJ();

    //assign the new value
    __J = _J;

    //build the new columns
    for(int i=0; i<getI(); i++) {
        Cells[i].setCount(getJ());
        for(int j=jini; j<getJ(); j++)
            Cells[i][j].Print = TRoboticPositioner::PrintId;
    }

    //assimilates the new value
    __qx = (getxmax() - getxmin())/double(getJ());
}

//PROPERTIES IN TEXT FORMAT:

AnsiString TRoboticPositionerList2::getxminText(void) const
{
    return FloatToStr(getxmin());
}
void TRoboticPositionerList2::setxminText(const AnsiString &S)
{
    try {
        setxmin(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TRoboticPositionerList2::getxmaxText(void) const
{
    return FloatToStr(getxmax());
}
void TRoboticPositionerList2::setxmaxText(const AnsiString &S)
{
    try {
        setxmax(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TRoboticPositionerList2::getyminText(void) const
{
    return FloatToStr(getymin());
}
void TRoboticPositionerList2::setyminText(const AnsiString &S)
{
    try {
        setymin(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TRoboticPositionerList2::getymaxText(void) const
{
    return FloatToStr(getymax());
}
void TRoboticPositionerList2::setymaxText(const AnsiString &S)
{
    try {
        setymax(StrToFloat_(S));
    } catch(...) {
        throw;
    }
}

AnsiString TRoboticPositionerList2::getIText(void) const
{
    return IntToStr(getI());
}
void TRoboticPositionerList2::setIText(const AnsiString &S)
{
    try {
        setI(StrToInt_(S));
    } catch(...) {
        throw;
    }
}
AnsiString TRoboticPositionerList2::getJText(void) const
{
    return IntToStr(getJ());
}
void TRoboticPositionerList2::setJText(const AnsiString &S)
{
    try {
        setJ(StrToInt_(S));
    } catch(...) {
        throw;
    }
}

AnsiString TRoboticPositionerList2::getqxText(void) const
{
    return FloatToStr(getqx());
}
AnsiString TRoboticPositionerList2::getqyText(void) const
{
    return FloatToStr(getqy());
}

//SET OF PROPERTIES IN TEXT FORMAT:

AnsiString TRoboticPositionerList2::getIntervalText(void) const
{
    AnsiString S;

    S = AnsiString("xmin = ")+getxminText()+AnsiString("\r\n");
    S += AnsiString("xmax = ")+getxmaxText()+AnsiString("\r\n");
    S += AnsiString("ymin = ")+getyminText()+AnsiString("\r\n");
    S += AnsiString("ymax = ")+getymaxText();

    return S;
}
void TRoboticPositionerList2::setIntervalText(const AnsiString &S)
{
    try {
        //build a clone tampon variable
        TRoboticPositionerList2 RPL(this);

        //read instance value from the first position
        int i = 1;
        ReadInterval(&RPL, S, i);

        //string shall contain an instance value only
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("string should contain the instance value only");

        //set the tampon variable
        Copy(&RPL);

    } catch(...) {
        throw;
    }
}

AnsiString TRoboticPositionerList2::getCellsText(void) const
{
    //make a rutinary check
    for(int i=0; i<getI(); i++)
        for(int j=0; j<getJ(); j++)
            if(Cells[i][j].Print == NULL)
                throw EImpossibleError("lateral effect");

    AnsiString S;

    //print the dimensions of the cell matrix
    S = AnsiString("I = ")+getIText()+AnsiString("\r\n");
    S += AnsiString("J = ")+getJText()+AnsiString("\r\n");

    //print the cells of the matrix
    for(int i=0; i<getI(); i++)
        for(int j=0; j<getJ(); j++)
            S += AnsiString("Cells[")+IntToStr(i)+AnsiString("][")+IntToStr(j)+AnsiString("] = ")+Cells[i][j].getText()+AnsiString("\r\n");

    return S;
}
void TRoboticPositionerList2::setCellsText(const AnsiString &S)
{
    try {
        //build a clone tampon variable
        TRoboticPositionerList2 *RPL = new TRoboticPositionerList2(this);

        //read the instance value from the first position
        int i = 1;
        ReadCells(RPL, S, i);

        //string shall contain an instance value only
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("string should contain an instance value only");

        //assign thetampon variable
        Copy(RPL);

        delete RPL;

    } catch(...) {
        throw;
    }
}
/*+++*/
AnsiString TRoboticPositionerList2::getSizeText(void)
{
    AnsiString S;

    S = AnsiString("qx = ")+getqxText()+AnsiString("\r\n");
    S += AnsiString("qy = ")+getqyText();

    return S;
}

AnsiString TRoboticPositionerList2::getInstanceMapText(void) const
{
    AnsiString S;

    //imprime los límites del intervalo
    S += getIntervalText()+AnsiString("\r\n");

    //imprime la matriz de listas de posicionadores
    S += getCellsText();

    return S; //devuelve la cadena
}
void TRoboticPositionerList2::setInstanceMapText(const AnsiString& S)
{
    //TBD
}

//-------------------------------------------------------------------
//STATIC METHODS FOR INDIVIDUAL PROPERTIES:

//read xmin in assignment text format
void  TRoboticPositionerList2::ReadxminText(double &xmin, const AnsiString &S,
                                            int &i)
{
    try {
        AnsiString Ident; //identificador de la asignación
        AnsiString Value; //valor de la asignación

        StrReadLabel(Ident, "xmin", S, i);
        StrTravelLabel("=", S, i);
        StrReadWord(Value, S, i);
        xmin = StrToFloat_(Value);

    } catch(EImproperArgument &E) {
        throw EImproperArgument(E.Message+AnsiString(" for property xmin"));
    } catch(...) {
        throw;
    }
}
//read xmax in assignment text format
void  TRoboticPositionerList2::ReadxmaxText(double &xmax, const AnsiString &S,
                                            int &i)
{
    try {
        AnsiString Ident; //identificador de la asignación
        AnsiString Value; //valor de la asignación

        StrReadLabel(Ident, "xmax", S, i);
        StrTravelLabel("=", S, i);
        StrReadWord(Value, S, i);
        xmax = StrToFloat_(Value);

    } catch(EImproperArgument &E) {
        throw EImproperArgument(E.Message+AnsiString(" for property xmax"));
    } catch(...) {
        throw;
    }
}
//read ymin in assignment text format
void  TRoboticPositionerList2::ReadyminText(double &ymin, const AnsiString &S,
                                            int &i)
{
    try {
        AnsiString Ident; //identificador de la asignación
        AnsiString Value; //valor de la asignación

        StrReadLabel(Ident, "ymin", S, i);
        StrTravelLabel("=", S, i);
        StrReadWord(Value, S, i);
        ymin = StrToFloat_(Value);

    } catch(EImproperArgument &E) {
        throw EImproperArgument(E.Message+AnsiString(" for property ymin"));
    } catch(...) {
        throw;
    }
}
//read umax in assignment text format
void  TRoboticPositionerList2::ReadymaxText(double &ymax, const AnsiString &S,
                                            int &i)
{
    try {
        AnsiString Ident; //identificador de la asignación
        AnsiString Value; //valor de la asignación

        StrReadLabel(Ident, "ymax", S, i);
        StrTravelLabel("=", S, i);
        StrReadWord(Value, S, i);
        ymax = StrToFloat_(Value);

    } catch(EImproperArgument &E) {
        throw EImproperArgument(E.Message+AnsiString(" for property ymax"));
    } catch(...) {
        throw;
    }
}

//read I in assignment text format
void  TRoboticPositionerList2::ReadIText(int &I,
                                         const AnsiString &S, int &i)
{
    try {
        AnsiString Ident; //identificador de la asignación
        AnsiString Value; //valor de la asignación

        StrReadLabel(Ident, "I", S, i);
        StrTravelLabel("=", S, i);
        StrReadWord(Value, S, i);
        I = StrToInt_(Value);

    } catch(EImproperArgument &E) {
        throw EImproperArgument(E.Message+AnsiString(" for property I"));
    } catch(...) {
        throw;
    }
}
//read J in assignment text format
void  TRoboticPositionerList2::ReadJText(int &J,
                                         const AnsiString &S, int &i)
{
    try {
        AnsiString Ident; //identificador de la asignación
        AnsiString Value; //valor de la asignación

        StrReadLabel(Ident, "J", S, i);
        StrTravelLabel("=", S, i);
        StrReadWord(Value, S, i);
        J = StrToInt_(Value);

    } catch(EImproperArgument &E) {
        throw EImproperArgument(E.Message+AnsiString(" for property J"));
    } catch(...) {
        throw;
    }
}

//STATIC METHODS FOR SETS OF PROPERTIES:

//print the mapping interval of a RP map
//in a text string in assign format
void  TRoboticPositionerList2::PrintInterval(AnsiString &S,
                                             const TRoboticPositionerList2 *RPL)
{
    //el puntero RPL debería apuntar a un mapa de posicionadores construido
    if(RPL == NULL)
        throw EImproperArgument("pointer RPL should point to built robotic positioners map");

    S += RPL->getIntervalText();
}
//read the mapping interval of a RP map
//in a text string in assign format
void  TRoboticPositionerList2::ReadInterval(TRoboticPositionerList2 *RPL,
                                            const AnsiString &S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el puntero RPL debería apuntar a un mapa de posicionadores construido
    if(RPL == NULL)
        throw EImproperArgument("pointer RPL shoult point to built celestial object");

    //el índice i debería indicar una posición de la cadena de texto S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("index i should indicate a position in the string text S");

    //estado de lectura
    //      0: esperando asignación para xmin
    //      1: esperando asignación para xmax
    //      2: esperando asignación para ymin
    //      3: esperando asignación para ymax
    //      4: cluster leido con éxito
    int status = 0;

    //variables auxiliares
    double xmin, xmax, ymin, ymax; //variables tampón

    //ADVERTENCIA: las variables tampón con propiedades interdependientes
    //deben ser clones de las variables que se pretenden modificar.

    //NOTA: adviertase que las propiedades enteras son leidas como
    //valores en punto flotante para detectar errores en los cuales
    //sea especificado un valor en punto flotante en vez de un valor entero.

    do {
        switch(status) {
        case 0: //esperando asignación para xmin
            ReadxminText(xmin, S, i);
            if(i > S.Length())
                throw EImproperArgument("assignment xmax not found");
            status++;
            break;
        case 1: //esperando asignación para xmin
            ReadxmaxText(xmax, S, i);
            if(i > S.Length())
                throw EImproperArgument("assignment ymin not found");
            status++;
            break;
        case 2: //esperando asignación para ymin
            ReadyminText(ymin, S, i);
            if(i > S.Length())
                throw EImproperArgument("assignment ymax not found");
            status++;
            break;
        case 3: //esperando asignación para ymin
            ReadymaxText(ymax, S, i);
            status++;
            break;
        }
    } while(status < 4);

    //asigna las variables tampón
    try {
        RPL->SetMapInterval(xmin, xmax, ymin, ymax);
    } catch(...) {
        throw;
    }
}

//print the cells of a RP map
//in a text string in assign format
void  TRoboticPositionerList2::PrintCells(AnsiString &S,
                                          const TRoboticPositionerList2 *RPL)
{
    //el puntero RPL debería apuntar a un mapa de posicionadores construido
    if(RPL == NULL)
        throw EImproperArgument("pointer RPL should point to built robotic positioners list");

    S += RPL->getCellsText();
}
//read the cells of a RP map
//in a text string in assign format
void  TRoboticPositionerList2::ReadCells(TRoboticPositionerList2 *RPL,
                                         const AnsiString &S, int &p)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el puntero RPL debería apauntar a una lista deposicionadores construida
    if(RPL == NULL)
        throw EImproperArgument("pointer RPL should point to built fiberpositioner list");

    //el índice p debería indicar a una posición de la cadena de texto S
    if(p<1 || S.Length()+1<p)
        throw EImproperArgument("index p should indicate a position in the string S");

    int _I; //núnmero de filas de la nueva matriz
    int _J; //número de columnas d el anueva matriz

    try {
        //lee las dimensiones de la matriz
        ReadIText(_I, S, p);
        ReadJText(_J, S, p);
    } catch(...) {
        throw;
    }

    //contruye una matriz de celdas tampón
    TPointersList<TPointersList<TItemsList<TRoboticPositioner*> > > _Cells;
    //redimensiona la matriz
    _Cells.setCount(_I);
    for(int i=0; i<_I; i++)
        _Cells[i].setCount(_J);

    AnsiString Label; //etiqueta de una asignación
    AnsiString Ident; //identificador de una asignación
    TVector<int> Ids; //lista de identificadores de posicionadores
    TItemsList<TRoboticPositioner*> *RPs; //puntero a lista de punteros a posicionadores
    int Id; //identificador indicado
    int l; //índice al posicionador identificado

    try {
        //por cada fila de la matriz de celdas
        for(int i=0; i<_I; i++)
            //por cada columna de la matriz de celdas
            for(int j=0; j<_J; j++) {
                //contruye la etiqueta
                Label = AnsiString("Cells[")+IntToStr(i)+AnsiString("][")+IntToStr(j)+AnsiString("]");
                //lee el identificador de la asignación
                StrReadLabel(Ident, Label, S, p);
                //atraviesa el operador de asignación
                StrTravelLabel("=", S, p);
                //lee la lista de identificadores de posicionador
                TVector<int>::ReadVector(&Ids, S, p);

                //apunta la lista de punteros a posicionador de la célula indicada
                //para facilitar su acceso
                RPs = _Cells[i].GetPointer(j);
                //apunta los vectores identificados en la célula
                for(int k=0; k<Ids.getCount(); k++) {
                    //asigna el identificador indicado para facilitar su acceso
                    Id = Ids[k];
                    //busca el posicionador identificado
                    l = RPL->SearchId(Id);
                    //si no ha encontrado el posicionador
                    if(l >= RPL->getCount())
                        //indica que el posicionador identificado no está en la lista de posicionadores
                        throw EImproperFileLoadedValue(AnsiString("identified fiber positioner '")+IntToStr(Id)+AnsiString("' is not in the robotic positioenr list"));
                    //añade el puntero al posicionador
                    TRoboticPositioner *RP = RPL->Items[l];
                    RPs->Add(RP);
                }
            }
    } catch(...) {
        throw;
    }

    //asigna las variables tampón
    RPL->SetMapCells(_Cells);
}

//-------------------------------------------------------------------
//BUILDING, COPY AND DESTROYING METHODS:

//build a RP list
TRoboticPositionerList2::TRoboticPositionerList2(void) :
    TRoboticPositionerList1(),
    //propiedades del mapa
    __xmin(-106.08), __xmax(106.08),
    __ymin(-102), __ymax(102),
    Cells(50), __J(0)
{
    //calcula el lado máximo de una célula de la regilla
    double qmax = MEGARA_L/sqrt(2.) - ERR_NUM;

    //Nótese que el tamaño de las celdas no es crítico, pues solo influirá
    //en que algunas celdas contengas un número mayor o menor de RPs.
    //Por eso qmax es calculado de modo que la diagonal de cada celda no
    //supere MEGARA_L, que es la distancia mínima nominal entre
    //áreas comunes de tres posicionadores.

    //calcula el número de filas
    int I_ = ceil((getymax() - getymin())/qmax);
    //calcula el número de columnas
    int J_ = ceil((getxmax() - getxmin())/qmax);

    //redimensiona la matriz de celdas
    SetMapDimensions(I_, J_);
}

//copy the cell matrix
void TRoboticPositionerList2::CopyCells(const TRoboticPositionerList2 *RPL)
{
    //el puntero RPL debería apuntar a un mapa de posicionadores construido
    if(RPL == NULL)
        throw EImproperArgument("pointer RPL should point to built robotic positioner map");


    //todos los punteros a posicionador de la matriz de celdas deben estar en la lista
    //        RPL->MapOk

    //redimensiona la matriz
    SetMapDimensions(RPL->getI(), RPL->getJ());

    //inicializa la matriz
    for(int i=0; i<getI(); i++)
        for(int j=0; j<getJ(); j++)
            Cells[i][j].Clear();

    //añade los posiciondores correspondientes a cada celda
    for(int i=0; i<getI(); i++)
        for(int j=0; j<getJ(); j++) {
            const TItemsList<TRoboticPositioner*> *RPs = RPL->Cells[i].GetPointer(j);
            for(int k=0; k<RPs->getCount(); k++) {
                const TRoboticPositioner *RP = RPs->Get(k);
                int l = 0;
                while(l<getCount() && Items[l]->getActuator()->getId()!=RP->getActuator()->getId())
                    l++;
                Cells[i][j].Add(Items[l]);
            }
        }

}
//copy the RP map of a RPL
void TRoboticPositionerList2::CopyMap(const TRoboticPositionerList2 *RPL)
{
    //el puntero RPL debería apuntar a un mapa de posicionadores construido
    if(RPL == NULL)
        throw EImproperArgument("pointer RPL should point to built robotic positioner map");

    //asigna las propiedades de configuración
    __xmin = RPL->getxmin();
    __xmax = RPL->getxmax();
    __ymin = RPL->getymin();
    __ymax = RPL->getymax();

    //clona la matriz de listas de punteros a posicionador
    CopyCells(RPL);
    __J = RPL->getJ();

    //asigna las propiedades derivadas
    __qx = RPL->getqx();
    __qy = RPL->getqy();
}

//clone a RPL including:
//  (Tolerance, Sizing, Area, Graphics, Map)
void TRoboticPositionerList2::Clone(const TRoboticPositionerList2 *RPL)
{
    //check the precondition
    if(RPL == NULL)
        throw EImproperArgument("pointers RPL should point to built robotic positioner list");

    //initialize this list
    Destroy();
    //clona los posicionadores de la lista
    for(int i=0; i<RPL->getCount(); i++) {
        TRoboticPositioner *RP = RPL->Get(i);
        TRoboticPositioner *RP_ = new TRoboticPositioner(RP);
        Add(RP_);
    }

    //WARNING: due to a robotic positioner list is a list of pointers,
    //their method Clone copy only the pointers:
    //        Items.Clone(RPL->Items);
    //This is the reason why the clonation of the list need build
    //the RPs.

    //point the same extern funtions
    Compare = RPL->Compare;
    Evaluate = RPL->Evaluate;
    Assign = RPL->Assign;
    Print = RPL->Print;
    Read = RPL->Read;

    //copy all other properties of the model
    CopyTolerance(RPL);
    CopySizing(RPL);
    CopyArea(RPL);
    CopyMap(RPL);
}
/*TRoboticPositionerList2 &TRoboticPositionerList2::operator=(const TRoboticPositionerList2 &RPL)
{
        //asigna las propiedades de configuración
        __xmin = RPL.xmin;
        __xmax = RPL.xmax;
        __ymin = RPL.ymin;
        __ymax = RPL.ymax;
        //copia las celdas
        Cells = RPL.Cells;
        __J = RPL.J;
        //asigna las propiedades derivadas
        __qx = RPL.qx;
        __qy = RPL.qy;

        return *this;
}                       */


//build a clon of a RPL
TRoboticPositionerList2::TRoboticPositionerList2(const TRoboticPositionerList2 *RPL)
{
    try {
        Clone(RPL);
    } catch(...) {
        throw;
    }
}

//MAP METHODS:

//set the limmits of the mapping interval
void TRoboticPositionerList2::SetMapInterval(double _xmin, double _xmax,
                                             double _ymin, double _ymax)
{
    //el límite superior xmax deberíar mayor que el límite inferior xmin
    if(_xmax <= _xmin)
        throw EImproperArgument("upper limit xmax should be upper to lower limit xmin");

    //el límite superior ymax deberíar mayor que el límite inferior ymin
    if(_ymax <= _ymin)
        throw EImproperArgument("upper limit ymax should be upper to lower limit ymin");

    //asigna los nuevos valores
    __xmin = _xmin;
    __xmax = _xmax;
    __ymin = _ymin;
    __ymax = _ymax;

    //asimila los nuevos valores
    CalculateSize();
}
//set the dimensions of the cell matrix
void TRoboticPositionerList2::SetMapDimensions(int _I, int _J)
{
    //el número de filas de la matriz de celdas debería ser mayor que cero
    if(_I < 1)
        throw EImproperArgument("cells matrix's number rows I should be upper zero");

    //el número de columnas de la matriz de celdas debería ser mayor que cero
    if(_J < 1)
        throw EImproperArgument("cells matrix's number columns J should be upper zero");

    //guarda las dimensiones inciales
    double iini = getI();
    double jini = getJ();

    //asigna el nuevo número de columnas
    __J = _J;

    //apunta la función de impresión de identificador de posicionador
    //en las nuevas columnas
    for(int i=0; i<getI(); i++) {
        Cells[i].setCount(getJ());
        for(int j=jini; j<getJ(); j++)
            Cells[i][j].Print = TRoboticPositioner::PrintId;
    }

    //asigna el nuevo número de filas
    Cells.setCount(_I);

    //apunta la función de impresión de identificador de posicionador
    //en las nuevas filas
    for(int i=iini; i<getI(); i++) {
        Cells[i].setCount(getJ());
        for(int j=0; j<getJ(); j++)
            Cells[i][j].Print = TRoboticPositioner::PrintId;
    }

    //asimila los nuevos valores
    CalculateSize();
}
//set the content of the cell matrix
void TRoboticPositionerList2::SetMapCells(TPointersList<TPointersList<TItemsList<TRoboticPositioner*> > >& _Cells)
{
    //el número de filas de la matriz de celdas debería ser mayor que cero
    if(_Cells.getCount() < 1)
        throw EImproperArgument("cells matrix number rows should be upper zero");

    //todas las filas de la matriz de celdas deberían tener el mismo número de columnas
    for(int i=1; i<_Cells.getCount(); i++)
        if(_Cells[i].getCount() != _Cells.getFirst().getCount())
            throw EImproperArgument("all rows in cells matriz should have the same number column");

    //el número de columnas de la matriz de celdas debería ser mayor que cero
    if(_Cells.getFirst().getCount() < 1)
        throw EImproperArgument("cells matrix number column should be upper zero");

    //todos los punteros a posicionador de la matriz de celdas deben estar en la lista
    for(int i=0; i<_Cells.getCount(); i++)
        for(int j=0; j<_Cells.getFirst().getCount(); j++) {
            TItemsList<TRoboticPositioner*> RPL = _Cells[i][j];
            for(int k=0; k<RPL.getCount(); k++) {
                TRoboticPositioner *RP = RPL.Get(k);
                int l = 0;
                while(l<getCount() && Items[l]!=RP)
                    l++;
                if(l >= getCount())
                    throw EImproperArgument("all pointers to robotic positioners in the cell matrix should be in the list");
            }
        }

    //copia la matriz
    Cells = _Cells;

    //asimila el nuevo valor
    __J = Cells.getFirst().getCount();
    CalculateSize();

    //Nótese que la copia de una matriz de listas no implica
    //la clonación de las listas, sino la copia de sus elementos.

    //apunta la función de impresión en todas las listas de punteros a posicionador
    for(int i=0; i<getI(); i++)
        for(int j=0;  j<getJ(); j++)
            Cells[i][j].Print = TRoboticPositioner::PrintId;
}

//maps the RPs in the mapping interval
void TRoboticPositionerList2::Map(void)
{
    //vértices de una célula
    TItemsList<TDoublePoint> cell;
    cell.setCount(5);

    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int k=0; k<getCount(); k++) {
        //apunta el posicionador para facilitar su acceso
        RP = Items[k];
        //determina el intervalo de celdas que contienen el dominio del posicionador
        int jmin = jfloor(RP->getActuator()->getx3min());
        int imin = ifloor(RP->getActuator()->gety3min());
        int jmax = jceil(RP->getActuator()->getx3max());
        int imax = iceil(RP->getActuator()->gety3max());
        //por cada celda del intervalo
        for(int i=imin; i<imax; i++) {
            for(int j=jmin; j<jmax; j++) {
                //si (i, j) está en el intervalo de definición de Cells
                if(0<=i && i<getI() && 0<=j && j<getJ()) {
                    //calcula los vértices de la celda
                    cell[0].x = getxmin() + j*getqx();
                    cell[0].y = getymin() + i*getqy();
                    cell[1].x = cell[0].x;
                    cell[1].y = cell[0].y + getqy();
                    cell[2].x = cell[1].x + getqx();
                    cell[2].y = cell[1].y;
                    cell[3].x = cell[2].x;
                    cell[3].y = cell[0].y;
                    cell[4].x = cell[0].x;
                    cell[4].y = cell[0].y;
                    //inflacciona la celda para compensar el error numérico
                    cell[0].x -= ERR_NUM;
                    cell[0].y -= ERR_NUM;
                    cell[1].x -= ERR_NUM;
                    cell[1].y += ERR_NUM;
                    cell[2].x += ERR_NUM;
                    cell[2].y += ERR_NUM;
                    cell[3].x += ERR_NUM;
                    cell[3].y -= ERR_NUM;
                    cell[4].x -= ERR_NUM;
                    cell[4].y -= ERR_NUM;
                    //si la celda invade el dominio del posicionador
                    if(RP->getActuator()->PolysegmentInvadeDomainP3(cell))
                        //añade el posicionador a la lista de la celda
                        Cells[i][j].Add(RP);
                }
            }
        }
    }

    /*        //el punto de referencia

        //por cada célula (i, j) del retículo
        for(int i=0; i<I; i++) {
                for(int j=0; j<J; j++) {
                        //calcula los vértices de la célula
                        cell[0].x = xmin + i*qx;
                        cell[0].y = ymin + j*qy;
                        cell[1].x = cell[0].x;
                        cell[1].y = cell[0].y + qy;
                        cell[2].x = cell[1].x + qx;
                        cell[2].y = cell[1].y;
                        cell[3].x = cell[2].x;
                        cell[3].y = cell[0].y;
                        cell[4].x = cell[0].x;
                        cell[4].y = cell[0].y;

                        //por cada posicioandor k de la lista
                        for(int k=0; k<getCount(); k++) {
                                //apunta el posicionador para facilitar su acceso
                                RP = Items[k];
                                //si la célula invade el dominio del posicionador
                                if(RP->getActuator()->PolysegmentInvadeDomainP3(cell))
                                        //añade el posicionador a la lista de la célula
                                        Cells[i][j].Add(RP);
                        }
                }
        }
*/
}
//automatically:
//      1. stablish the appropriate limits of the mapping interval
//      2. stablish the appropriate dimensions of the cell matrix
//      3. maps the RPs in the mapping interval
void TRoboticPositionerList2::GenerateMap(void)
{
    //calcula el lado máximo de una célula de la regilla
    double qmax = MEGARA_L/sqrt(2.) - ERR_NUM;

    //determines the limits of the jointly observing domain of the RPs
    double x3min, x3max, y3min, y3max;
    if(getCount() > 0)
        GetDomainP3s(x3min, x3max, y3min, y3max);
    else {
        x3min = -qmax;
        x3max = qmax;
        y3min = -qmax;
        y3max = qmax;
    }

    //establece los límites del intervalo de mapeo
    SetMapInterval(x3min, x3max, y3min, y3max);

    //caculates the dimensions of the cell matrix
    int I_, J_;
    I_ = ceil((y3max - y3min)/qmax);
    J_ = ceil((x3max - x3min)/qmax);

    //redimensiona la matriz de celdas
    SetMapDimensions(I_, J_);

    //Nótese que el tamaño de las celdas no es crítico, pues solo influirá
    //en que algunas celdas contengas un número mayor o menor de RPs.
    //Por eso qmax es calculado de modo que la diagonal de cada celda no
    //supere MEGARA_L, que es la distancia mínima nominal entre
    //áreas comunes de tres posicionadores.

    //mapea los posicionadores en el intervalo de mapeo
    Map();
}

//assimilates the configuration of RPs executing:
//      CalculateSPMComponents();
//      AssimilateSizing();
//      GenerateMap();
void TRoboticPositionerList2::Assimilate(void)
{

    CalculateSPMComponents();
    AssimilateSizing();
    GenerateMap();
}

//assimilates all RPs of the RPL to a given RP
void TRoboticPositionerList2::Apply(const TRoboticPositioner& RP)
{
    //for all RPs copy all properties of the RP instance except (P0, Id, Id1, Id2)
    for(int i=0; i<getCount(); i++)
        Items[i]->Apply(RP);

    Assimilate();
}

//transform x in j by default
//      x = floor((x - xmin)/qx)
int TRoboticPositionerList2::jfloor(double x)
{
    //la abcisa x debería estar en el intervalo de mapeo [xmin, xmax]
    if(x<getxmin() || getxmax()<x)
        throw EImproperArgument("abcise x should be in [xmin, xmax]");

    return floor((x - getxmin())/getqx());
}
//transform y in i by default
//      y = floor((y - ymin)/qy)
int TRoboticPositionerList2::ifloor(double y)
{
    //la ordenada y debería estar en el intervalo de mapeo [ymin, ymax]
    if(y<getymin() || getymax()<y)
        throw EImproperArgument("ordinate y should be in [ymin, ymax]");

    return floor((y - getymin())/getqy());
}
//transform x in j by excess
//      x = ceil((x - xmin)/qx)
int TRoboticPositionerList2::jceil(double x)
{
    //la abcisa x debería estar en el intervalo de mapeo [xmin, xmax]
    if(x<getxmin() || getxmax()<x)
        throw EImproperArgument("abcise x should be in [xmin, xmax]");

    return ceil((x - getxmin())/getqx());
}
//transform y in i by excess
//      y = ceil((y - ymin)/qy)
int TRoboticPositionerList2::iceil(double y)
{
    //la ordenada y debería estar en el intervalo de mapeo [ymin, ymax]
    if(y<getymin() || getymax()<y)
        throw EImproperArgument("ordinate y should be in [ymin, ymax]");

    return ceil((y - getymin())/getqy());
}

//access to the RP list of the cell which contains a point
TItemsList<TRoboticPositioner*> *TRoboticPositionerList2::RoboticPositionersCell(TDoublePoint P)
{
    //el punto P debería estar en el intervalo de mapeo [xmin, xmax]x[ymin, ymax]
    if(P.x<getxmin() || getxmax()<P.x || P.y<getymin() || getymax()<P.y)
        throw EImproperArgument("point P should be in [xmin, xmax]x[ymin, ymax]");

    //devuelve el contenido de la celda correspondiente
    return Cells[ifloor(P.y)].GetPointer(jfloor(P.x));
}

//search the RPs whose scope is a point
void TRoboticPositionerList2::SearchPositioners(TItemsList<TRoboticPositioner*> &RPs,
                                                TDoublePoint P)
{
    RPs.Clear();

    //si el punto no stá en el intervalo de mapeo
    if(P.x<getxmin() || getxmax()<P.x || P.y<getymin() || getymax()<P.y)
        return; //devuelve la lista vacía

    //apunta la lista de la celda donde está el punto
    TItemsList<TRoboticPositioner*> *RPL = RoboticPositionersCell(P);

    TRoboticPositioner *RP;

    //por cada puntero a posicionador de la lista
    for(int k=0; k<RPL->getCount(); k++) {
        //apunta el posicionador para facilitar su acceso
        RP = RPL->Get(k);
        //i el punto está en el dominio del punto P3 del posicionador
        if(RP->getActuator()->PointIsInDomainP3(P))
            //añade el posicionador a la lista
            RPs.Add(RP);
    }
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
