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
//File: RoboticPositionerList1.cpp
//Content: RP list with sizing properties
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "RoboticPositionerList1.h"
#include "Strings.h"
#include "Scalars.h"
#include "VCLemu.h"
#include "TextFile.h"

//---------------------------------------------------------------------------

using namespace Strings;
using namespace Mathematics;

//namespace for models
namespace Models {

//---------------------------------------------------------------------------
//TOLERANCE PROPERTIES:

//margen de error de orientación de S0.
//debe ser no negativo
//valor por defecto: MEGARA_PAem rad
void TRoboticPositionerList1::setPAem(double PAem)
{
    //comprueba las precondiciones
    if(PAem < 0)
        throw EImproperArgument("position angle error margin (PAem) should be nonnegative");

    p_PAem = PAem; //asigna el nuevo valor
    calculateSPMcomponents(); //assimilate the new value
}
//margen de error de apuntado de S0.
//debe ser no negativo
//valor por defecto: MEGARA_Pem mm
void TRoboticPositionerList1::setPem(double Pem)
{
    //comprueba las precondiciones
    if(Pem < 0)
        throw EImproperArgument("position error margin (Pem) should be nonnegative");

    p_Pem = Pem; //asigna el nuevo valor
    calculateSPMcomponents(); //assimilate the new value
}

//---------------------------------------------------------------------------
//TOLERANCE PROPERTIES IN TEXT FORMAT:

AnsiString TRoboticPositionerList1::getPAemText(void) const
{
    return FloatToStr(getPAem());
}
void TRoboticPositionerList1::setPAemText(const AnsiString &S)
{
    try {
        setPAem(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting PAem in text format: ");
        throw;
    }
}
AnsiString TRoboticPositionerList1::getPemText(void) const
{
    return FloatToStr(getPem());
}
void TRoboticPositionerList1::setPemText(const AnsiString &S)
{
    try {
        setPem(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting Pem in text format: ");
        throw;
    }
}

//SIZING PROPERTIES IN TEXT FORMAT:

AnsiString TRoboticPositionerList1::getOText(void) const
{
    return DPointToStr(getO());
}

AnsiString TRoboticPositionerList1::getLO3maxText(void) const
{
    return FloatToStr(getLO3max());
}

AnsiString TRoboticPositionerList1::getx3minText(void) const
{
    return FloatToStr(getx3min());
}
AnsiString TRoboticPositionerList1::getx3maxText(void) const
{
    return FloatToStr(getx3max());
}
AnsiString TRoboticPositionerList1::gety3minText(void) const
{
    return FloatToStr(gety3min());
}
AnsiString TRoboticPositionerList1::gety3maxText(void) const
{
    return FloatToStr(gety3max());
}

//AREA PROPERTIES IN TEXT FORMAT:

AnsiString TRoboticPositionerList1::getSptText(void) const
{
    return FloatToStr(getSpt());
}
AnsiString TRoboticPositionerList1::getSetText(void) const
{
    return FloatToStr(getSet());
}
AnsiString TRoboticPositionerList1::getRetText(void) const
{
    return FloatToStr(getRet());
}
AnsiString TRoboticPositionerList1::getFiberDensityText(void) const
{
    return FloatToStr(getFiberDensity());
}

//SETS OF PROPERTIES IN TEXT FORMAT:

//get the origins table in text format:
//  Id      x0      y0      thetaO1
AnsiString TRoboticPositionerList1::getOriginsTableText(void) const
{
    //guarda el valor de Print
    void ( *PrintBak)(AnsiString&, TRoboticPositioner*);
    PrintBak = Print;

    //apunta la lista de posicionadores con un puntero no constante para facilitar su escritura
    TRoboticPositionerList1 *RPL = (TRoboticPositionerList1*)this;

    //apunta la función de impresión de coordenadas de origen en formato linea de texto
    RPL->Print = TRoboticPositioner::printOriginsRow;
    //obtiene la lista de coordenadas de origen en una cadena de texto
    AnsiString S = RPL->getColumnText();

    //restaura el valor de Print
    RPL->Print = PrintBak;

    return S; //devuelve la cadena de texto
}
//set the origins table in text format:
//  Id      x0      y0      thetaO1
void TRoboticPositionerList1::setOriginsTableText(const AnsiString &S)
{
    //indica al principio de la cadena
    int i = 1;

    try {
        //atraviesa las etiquetas de la cabecera
        TActuator::travelOriginsLabelsRow(S, i);

        //contruye una variable tampón
        TPointersList<TRoboticPositioner> RPL;
        //apunta la función de lectura de coordenadas de origen en formato linea de texto
        RPL.Read = TRoboticPositioner::readOriginsRow;
        //lee la cadena en la variable tampón
        TPointersList<TRoboticPositioner>::ReadSeparated(&RPL, S, i);

        //avanza el índice i hasta la próxima posición que no contenga un separador
        StrTravelSeparatorsIfAny(S, i);
        //si el índice i indica a algún caracter de la cadena S
        if(i <= S.Length())
            //indica que la cadena S solo debería contener el valor para una instancia
            throw EImproperArgument("string S should contain the instance value only");

        //asigna la variable tampón
        Destroy();
        for(int i=0; i<RPL.getCount(); i++)
            Add(RPL.GetPointer(i));
        RPL.ClearWithoutDestroy();

    } catch(Exception& E) {
        unsigned int row, col;
        strPositionToCoordinates(row, col, S.str, i-1);
        E.Message.Insert(1, "setting origins table in text format in row "+inttostr(row)+" and column "+inttostr(col)+": ");
        throw;
    }
}
//get the P3 table in text format:
//  Id      x3      y3
AnsiString TRoboticPositionerList1::getPositionsP3TableText(void) const
{
    //guarda el valor de Print
    void ( *PrintBak)(AnsiString&, TRoboticPositioner*);
    PrintBak = Print;

    //apunta la lista de posicionadores con un puntero no constante para facilitar su escritura
    TRoboticPositionerList1 *RPL = (TRoboticPositionerList1*)this;

    //apunta la función de impresión de coordenadas de posición en formato linea de texto
    RPL->Print = TRoboticPositioner::printPositionP3Row;
    //obtiene la lista de coordenadas de posición en una cadena de texto
    AnsiString S = RPL->getColumnText();

    //restaura el valor de Print
    RPL->Print = PrintBak;

    return S; //devuelve la cadena de texto
}
//set the P3 table in text format
//  Id      x3      y3
void TRoboticPositionerList1::setPositionsP3TableText(const AnsiString &S)
{
    //indica al principio de la cadena
    int i = 1;

    try {
        //atraviesa las etiquetas de la cabecera
        TActuator::travelPositionP3LabelsRow(S, i);

        //contruye una variable tampón
        TRoboticPositionerList1 RPL(this);
        //apunta la función de lectura de coordenadas de posición en formato linea de texto
        RPL.Read = TRoboticPositioner::readPositionP3Row;
        //lee la cadena en la variable tampón
        TRoboticPositionerList1::ReadSeparatedForBuiltItems(&RPL, S, i);

        //avanza el índice i hasta la próxima posición que no contenga un separador
        StrTravelSeparatorsIfAny(S, i);
        //si el índice i indica a algún caracter de la cadena S
        if(i <= S.Length())
            //indica que la cadena S solo debería contener el valor para una instancia
            throw EImproperArgument("string S should contain the instance value only");

        //por cada RP de la lista tampón
        for(int i=0; i<RPL.getCount(); i++) {
            //apunta el RP indicado para facilitar su acceso
            TRoboticPositioner *RP = RPL[i];
            //todos los Ids de la lista tampón deben estar en esta lista
            if(searchId(RP->getActuator()->getId()) >= getCount())
                throw EImproperFileLoadedValue(AnsiString("Id not found: ")+RP->getActuator()->getIdText());
            //ninguno de los Ids de la lista tampón debe estar repetido
            int j;
            for(j=0; j<i; j++)
                if(RPL[j]->getActuator()->getId() == RP->getActuator()->getId())
                    throw EImproperFileLoadedValue(AnsiString("Id repeated: ")+RP->getActuator()->getIdText());
            for(j=i+1; j<RPL.getCount(); j++)
                if(RPL[j]->getActuator()->getId() == RP->getActuator()->getId())
                    throw EImproperFileLoadedValue(AnsiString("Id repeated: ")+RP->getActuator()->getIdText());
        }

        //asigna la variable tampón
        for(int i=0; i<RPL.getCount(); i++) {
            TActuator *A = Items[i]->getActuator();
            TActuator *A_ = RPL[i]->getActuator();
            A->settheta_1(A_->gettheta_1());
            A->getArm()->settheta___3(A_->getArm()->gettheta___3());
        }
        RPL.Destroy();

    } catch(Exception& E) {
        unsigned int row, col;
        strPositionToCoordinates(row, col, S.str, i-1);
        E.Message.Insert(1, "setting positions P3 table in text format in row "+inttostr(row)+" and column "+inttostr(col)+": ");
        throw;
    }
}
//get the P_3 table in text format:
//  Id      x_3      y_3
AnsiString TRoboticPositionerList1::getPositionsP_3TableText(void) const
{
    //guarda el valor de Print
    void ( *PrintBak)(AnsiString&, TRoboticPositioner*);
    PrintBak = Print;

    //apunta la lista de posicionadores con un puntero no constante para facilitar su escritura
    TRoboticPositionerList1 *RPL = (TRoboticPositionerList1*)this;

    //apunta la función de impresión de coordenadas de posición en formato linea de texto
    RPL->Print = TRoboticPositioner::printPositionP_3Row;
    //obtiene la lista de coordenadas de posición en una cadena de texto
    AnsiString S = RPL->getColumnText();

    //restaura el valor de Print
    RPL->Print = PrintBak;

    return S; //devuelve la cadena de texto
}
//get the PPA table in text format:
//  Id      p_1     p___3
AnsiString TRoboticPositionerList1::getPositionsPPATableText(void) const
{
    //save the value of Print
    void ( *PrintBak)(AnsiString&, TRoboticPositioner*);
    PrintBak = Print;

    //point the RPL with a nonconstant pointer to make this writable
    TRoboticPositionerList1 *RPL = (TRoboticPositionerList1*)this;

    //point the required print function
    RPL->Print = TRoboticPositioner::printPositionPPARow;
    //get the PPA table in a text string
    AnsiString S = RPL->getColumnText();

    //restore theprint value
    RPL->Print = PrintBak;

    return S; //return the text string
}

AnsiString TRoboticPositionerList1::getToleranceText(void) const
{
    string str;

    str = commentedLine("PAem = "+getPAemText().str, "orientation error margin of the system S0 (in rad)");
    str += "\r\n"+commentedLine("Pem = "+getPemText().str, "pointing error margin of the system S0 (in mm)");

    return AnsiString(str);
}
AnsiString TRoboticPositionerList1::getSizingText(void) const
{
    AnsiString S;

    S += AnsiString("O = ")+getOText()+AnsiString("\r\n");

    S += AnsiString("LO3max = ")+getLO3maxText();

    return S;
}
AnsiString TRoboticPositionerList1::getAreaText(void) const
{
    AnsiString S;

    S += AnsiString("Spt: ")+getSptText()+AnsiString("\r\n");
    S += AnsiString("Set: ")+getSetText()+AnsiString("\r\n");
    S += AnsiString("Ret: ")+getRetText()+AnsiString("\r\n");
    S += AnsiString("FiberDensity: ")+getFiberDensityText();

    return S;
}

AnsiString TRoboticPositionerList1::getAllText(void) const
{
    AnsiString S;

    S += AnsiString("\r\nTolerances:\r\n")+StrIndent(getToleranceText());
    S += AnsiString("\r\nItemsAddress: ")+getItemsAddressText();
    S += AnsiString("\r\nSizing:\r\n")+StrIndent(getSizingText());
    S += AnsiString("\r\nArea:\r\n")+StrIndent(getAreaText());

    return S;
}
AnsiString TRoboticPositionerList1::getInstanceText(void) const
{
    string str;

    //TOLERANCIAS DE FABRICACIÓN:

    str = commentedLine("Tolerances:", "Tolerances of the Fiber MOS:");
    str += "\r\n"+StrIndent(getToleranceText()).str;

    return AnsiString(str);
}
void TRoboticPositionerList1::setInstanceText(const AnsiString& S)
{
    //indica al principio de la cadena
    int i = 1;

    try {
        //contruye una variable tampón
        TRoboticPositionerList1 RPL(this);
        //lee la cadena en la variable tampón
        TRoboticPositionerList1 *aux = &RPL;
        readInstance((TRoboticPositionerList1*&)aux, S, i);

        //avanza el índice i hasta la próxima posición que no contenga un separador
        StrTravelSeparatorsIfAny(S, i);
        //si el índice i indica a algún caracter de la cadena S
        if(i <= S.Length())
            //indica que la cadena S solo debería contener el valor para una instancia
            throw EImproperArgument("string S should contain the instance value only");

        //asigna la variable tampón
        Clone(aux);

    } catch(Exception& E) {
        unsigned int row, col;
        strPositionToCoordinates(row, col, S.str, i-1);
        E.Message.Insert(1, "setting instance in text format in row "+inttostr(row)+" and column "+inttostr(col)+": ");
        throw;
    }
}

//-------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//lee una instancia en una cadena
void  TRoboticPositionerList1::readInstance(TRoboticPositionerList1* &RPL,
                                            const AnsiString& S, int &i)
{
    //comprueba las precondiciones
    if(RPL == NULL)
        throw EImproperArgument("pointer RPL should point to built robotic positioner list");
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    try {
        //variables tampón
        double PAem, Pem;

        StrTravelLabel("Tolerances:", S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("PAem", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(PAem, S,i);

        StrTravelSeparators(S, i);
        StrTravelLabel("Pem", S, i);
        StrTravelLabel("=", S, i);
        StrReadFloat(Pem, S,i);

        //asigna las variables tampón
        RPL->setTolerance(PAem, Pem);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading instance: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

//construye una lista de posicionadores
TRoboticPositionerList1::TRoboticPositionerList1(void) :
    TItemsList<TRoboticPositioner*>(100),
    //propiedades de seguridad
    p_PAem(MEGARA_PAem), p_Pem(MEGARA_Pem)
{
    //point the default functions
    Compare = TRoboticPositioner::compareIds;
    Print = TRoboticPositioner::printId;

    //inicializa las propiedades de localización
    p_O.x = 0;
    p_O.y = 0;

    //asimila las propiedades inicializadas
    //  AssimilateSizing();
}

//copia las propiedades de seguridad
void TRoboticPositionerList1::copyTolerance(const TRoboticPositionerList1 *RPL)
{
    //comprueba las precondiciones
    if(RPL == NULL)
        throw EImproperArgument("pointer RPL should point to built RP list");

    //copia las propiedades
    p_PAem = RPL->getPAem();
    p_Pem = RPL->getPem();
}
//copia las propiedades de dimensionamiento
void TRoboticPositionerList1::copySizing(const TRoboticPositionerList1 *RPL)
{
    //comprueba las precondiciones
    if(RPL == NULL)
        throw EImproperArgument("pointer RPL should point to built fiber positioner list");

    //copia las propiedades
    p_O = RPL->getO();
    p_LO3max = RPL->getLO3max();
    p_x3min = RPL->getx3min();
    p_x3max = RPL->getx3max();
    p_y3min = RPL->gety3min();
    p_y3max = RPL->gety3max();
}
//copia las propiedades de área
void TRoboticPositionerList1::copyArea(const TRoboticPositionerList1 *RPL)
{
    //comprueba las precondiciones
    if(RPL == NULL)
        throw EImproperArgument("pointer RPL should point to built fiber positioner list");

    //copia las propiedades
    p_Spt = RPL->getSpt();
    p_Set = RPL->getSet();
    p_Ret = RPL->getRet();
    p_FiberDensity = RPL->getFiberDensity();
}

//clone a RP list
//  (Tolerance, Sizing, Area, Graphics)
void TRoboticPositionerList1::Clone(const TRoboticPositionerList1 *RPL)
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
    //    Items.Clone(RPL->Items);
    //This is the reason why the clonation of the list need build
    //the RPs.

    //point the same extern funtions
    Compare = RPL->Compare;
    Evaluate = RPL->Evaluate;
    Assign = RPL->Assign;
    Print = RPL->Print;
    Read = RPL->Read;

    //copy all other properties of the model
    copyTolerance(RPL);
    copySizing(RPL);
    copyArea(RPL);
}

//construye un clon de una lista de posicionadores
TRoboticPositionerList1::TRoboticPositionerList1(const TRoboticPositionerList1 *RPL)
{
    //comprueba las precondiciones
    if(RPL == NULL)
        throw EImproperArgument("pointers RPL should point to built fiber positioner list");

    //clona todas las propiedades
    Clone(RPL);
}

//--------------------------------------------------------------------------
//ADD OR DELETE RPs:

//search and delete a RP of the list
//return the position where the RP was found
int TRoboticPositionerList1::deleteIfFind(const TRoboticPositioner* RP)
{
    //comprueba las precondiciones
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    int i = search(RP);
    if(i < getCount())
        Delete(i);

    //El método Delete no debe asimilar nada, porque el RP borrado sigue existiendo.

    return i;
}

//--------------------------------------------------------------------------
//CONTRUIR POSICIONADORES:

//construye la capa n de posicionadores equidistantes una distancia D
//donde la capa 0 representa el posicionador central
void TRoboticPositionerList1::buildLayer(int &CountId, int n, double D)
{
    //comprueba las precondiciones
    if(n < 0)
        throw EImproperArgument("numer of layer (n) shall not be negative");

    //si tiene 6 vértices
    if(n > 0) {

        //distancia entre el centro del cilidnro central y
        //el centro de los posicionadores de los vértices de la capa n
        const double nD = n*D;
        //un sexto de vuelta en radianes
        const double M_PI3 = M_PI/3;

        //calcula los vértices del exágono
        TDoublePoint V[6];
        for(int i=0; i<6; i++) {
            double aux = i*M_PI3;
            V[i].x = nD*cos(aux);
            V[i].y = nD*sin(aux);
        }

        //calcula los n-1 puntos intermedios entre cada par de vértices
        for(int i=1; i<6; i++) {
            for(int m=0; m<n; m++) {
                //determina P0
                double alfa = m/double(n);
                TDoublePoint P0 = (1-alfa)*V[i-1] + alfa*V[i];
                //añade un nuevo posicionador
                Add(new TRoboticPositioner(CountId++, P0));
            }
        }
        //calcula los n-1 puntos intermedios entre V[5] y V[0]
        for(int m=0; m<n; m++) {
            //determina P0
            double alfa = m/double(n);
            TDoublePoint P0 = (1-alfa)*V[5] + alfa*V[0];
            //añade un nuevo posicionador
            Add(new TRoboticPositioner(CountId++, P0));
        }
    }
    //si solo hay uno en el centro
    else
        //añade un nuevo posicionador con el Id indicado en la posicion (0, 0)
        Add(new TRoboticPositioner(CountId++, TDoublePoint(0, 0)));
}

//construye una colmena circular de posicionadores
//con N capas (más el central) y destruye
//los posicionadores fuera del radio R
void TRoboticPositionerList1::buildCircularHive(int &CountId, double D, int N, double R)
{
    //comprueba las precondiciones
    if(CountId < 0)
        throw EImproperArgument("identification number Id should be nonnegative");
    if(N < 0)
        throw EImproperArgument(AnsiString("number layers '")+IntToStr(N)+AnsiString("' should be nonnegative"));
    if(R <= 0)
        throw EImproperArgument(AnsiString("consrvation radio '")+FloatToStr(R)+AnsiString("' should be upper zero"));

    //construye las capas de posicionadores
    for(int i=0; i<=N; i++)
        buildLayer(CountId, i, D);

    //destruye los posicionadores fuera del círculo
    Destroy(R);
}
//construye una colmena cuadrada de posicionadores
//con N capas
void TRoboticPositionerList1::buildSquareHive(int &CountId, double D, int M, int N)
{
    //comprueba las precondiciones
    if(CountId < 0)
        throw EImproperArgument("identification number Id should be nonnegative");
    if(N < 0)
        throw EImproperArgument(AnsiString("number layers '")+IntToStr(N)+AnsiString("' should be nonnegative"));

    //calcula la componente y de la distancia entre posicionadores
    double Dy = D*cos(M_PI/6);

    //posición de cada posicionador
    TDoublePoint P0;

    //para cada fila
    for(int i=M; i>=-M; i--) {
        //calcula la ordenada de la capa indicada
        P0.y = Dy*i;

        //determina si la línea i es impar
        bool odd = int(i%2);

        //si la linea i es impar
        if(odd) {
            //construye  los posicionadores en j = [-N, N]
            for(int j=-N; j<=N; j++) {
                //calcula las coordenadas del posicionador
                P0.x = D*j;
                //añade un nuevo posicionador
                Add(new TRoboticPositioner(CountId++, P0));
            }
        }
        //si la capa es par
        else {
            //construye  los posicionadores desde en j = [-N-1, N]
            for(int j=-N-1; j<=N; j++) {
                //calcula las coordenadas del posicionador
                P0.x = D*j + D/2;
                //añade un nuevo posicionador
                Add(new TRoboticPositioner(CountId++, P0));
            }
        }
    }
}

//DESTRUIR POSICIONADORES:

//detruye el posicionador indicado de la lista
void TRoboticPositionerList1::Destroy(int i)
{
    //el índice i debe indicar un elemento de la lista
    if(i<0 || getCount()<=i)
        throw EImproperArgument("index i should point to list item");

    //destruye el posicionador indicado
    delete Items[i];

    //borra el puntero de la lista
    Delete(i);
}
//destruye todos los posicionadores y vacia la lista
void TRoboticPositionerList1::Destroy(void)
{
    //por cada posición de la lista
    for(int i=0; i<getCount(); i++)
        //destruye el posicionador indicado
        delete Items[i];

    //vacia la lista
    Clear();
}
//detruye los posicionadores que se encuentren a
//una distancia del centro mayor que la indicada
void TRoboticPositionerList1::Destroy(double rmax)
{
    TRoboticPositioner *RP;
    int i = 0;

    //por cada posicionador de la lista
    while(i < getCount()) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];

        //determina la distancia al origen
        double r = Mod(RP->getActuator()->getP0());

        //determina la distancia apical del dominio
        r += RP->getActuator()->getL03max();

        //si la distancia al origen es mayor que la máxima
        if(r > rmax)
            Delete(i); //destruye rl posicionador indicado
        else //si no
            i++; //indica al próximo posicionador
    }
}

//MÉTODOS DE BÚSQUEDA DE POSICIONADORES:

//busca un posicionador en la lista
int TRoboticPositionerList1::search(const TRoboticPositioner *RP) const
{
    //comprueba las precondiciones
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    //mientras queden posiciones en la lista y
    //el puntero no apunte al mismo posicionador
    int i = 0;
    while(i<getCount() && RP!=Items[i])
        i++; //incrementa el índice

    return i; //devuelve el índice
}
int TRoboticPositionerList1::search(const TActuator *A) const
{
    //comprueba las precondiciones
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built actuator");

    //mientras queden posiciones en la lista y
    //el puntero no apunte al mismo actuador
    int i = 0;
    while(i<getCount() && A!=Items[i]->getActuator())
        i++; //incrementa el índice

    return i; //devuelve el índice
}
//busca el primer posicionador con elidentificador indicado
int TRoboticPositionerList1::searchId(int Id) const
{
    //comprueba las precondiciones
    if(Id < 1)
        throw EImproperArgument("RP identifier number Id should be upper zero");

    int i = 0;
    while(i<getCount() && Items[i]->getActuator()->getId()!=Id)
        i++;

    return i;
}
//devuelve el puntero al primer posicionador
//con el identificador indicado
const TRoboticPositioner *TRoboticPositionerList1::searchIdPointer(int Id) const
{
    //comprueba las precondiciones
    if(Id < 1)
        throw EImproperArgument("identifier Id should be upper zero");

    //busca la posición del posicionador Id
    int i = searchId(Id);

    //el posicionador debe estar en la lista
    if(i >= getCount())
        throw EImproperArgument("missing Id fiber positioner");

    //devuelve el posicionador
    return Items[i];
}

//search the first free identifier starting fron Id
int TRoboticPositionerList1::searchFirstFreeId(int Id) const
{
    do {
        //search the actual Id
        int i = searchId(Id);

        //if has found the Id
        if(i < getCount())
            Id++; //increase the Id
        else //else
            return Id; //return the actual Id

    } while(true); //ever
}

//busca el primer posicionador de la lista
//en cuyo dominio de P3 se encuentra un punto
int TRoboticPositionerList1::searchDomainP3(TDoublePoint P)
{
    int i = 0;
    while(i<getCount() && Items[i]->getActuator()->pointIsOutDomainP3(P.x, P.y))
        i++;

    return i;
}
int TRoboticPositionerList1::searchDomainP3(double x, double y)
{
    int i = 0;
    while(i<getCount() && Items[i]->getActuator()->pointIsOutDomainP3(x, y))
        i++;

    return i;
}

//busca los posicionadores de la lista que tienen
//algún cuantificador desactivado
void TRoboticPositionerList1::searchDisabledQuantificators(TVector<int> &indices)
{
    TRoboticPositioner *RP;

    //limpia la lista de índices en congruencia con sus estado inicial
    indices.Clear();

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];
        //si algún cuantificador del posicionador está desactivado
        if(!RP->getActuator()->getQuantify_() || !RP->getActuator()->getArm()->getQuantify___())
            //añade el índice a la lista
            indices.Add(i);
    }
}

//--------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//A partir de:
//      (Items[i]->Eo, Items[i]->Ep, Items[i]->Tstop_em, Items[i]->Tshif_em, PAem, Pem)
//      {Items[i]->rmax}
//Obtiene y asigna a cada posicionador de la lista:
//      (Items[i]->SPMrec, Items[i]->SPMsta, Items[i]->SPMdyn, Items[i]->SPMmin, Items[i]->SPMoff)
void TRoboticPositionerList1::calculateSPMcomponents(void)
{
    //for each RP of the list:
    //  calculates (SPMrec, SPMsta, SPMdyn)
    //  assigns SPMmin
    //  calculates and assigns the SPMoff
    for(int i=0; i<getCount(); i++) {
        //aspunta el posicionador indicado para facilitar su acceso
        TRoboticPositioner *RP = Items[i];

        //calculates (SPMrec, SPMsta, SPMdyn)
        RP->calculateSPMcomponents();
        //calculates and assigns the SPMoff
        RP->setSPMoff(getPAem(), getPem());

        //SPMmin not needs to be stted because it is independent.
    }
}

//determina las EAs y los RPs que están lo bastante cerca
//de cada posicionador como para invadir su dominio de maniobra
void TRoboticPositionerList1::determineAdjacents(const TExclusionAreaList& EAL)
{
    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        TRoboticPositioner *RPi = Items[i];

        //inicializa la lista de EAs adyacentes
        RPi->getActuator()->AdjacentEAs.Clear();
        //inicializa la lista de RPs adyacentes
        RPi->getActuator()->AdjacentRPs.Clear();

        //para cada una de las EAs
        for(int j=0; j<EAL.getCount(); j++) {
            //apunta el EA indicada para facilitar su acceso
            TExclusionArea *EAj = EAL[j];
            //si están lo bastante cerca como para colisionar
            if(Mod(RPi->getActuator()->getP0() - EAj->Barrier.getP0()) <
                    (RPi->getActuator()->getr_max() + RPi->getActuator()->getSPMall_a() +
                     EAj->Barrier.getr_max() + EAj->Barrier.getSPM()) + ERR_NUM) {
                //añade el EA a la lista de EAs adyacentes
                RPi->getActuator()->AdjacentEAs.Add(new TAdjacentEA(EAj, DBL_MAX, DBL_MAX));
            }
        }
        //para cada uno de los RPs previos al indicado
        for(int j=0; j<i; j++) {
            //apunta el RP indicado para facilitar su acceso
            TRoboticPositioner *RPj = Items[j];
            //si están lo bastante cerca como para colisionar
            if(Mod(RPi->getActuator()->getP0() - RPj->getActuator()->getP0()) <
                    (RPi->getActuator()->getr_max() + RPi->getActuator()->getSPMall_a() +
                     RPj->getActuator()->getr_max() + RPj->getActuator()->getSPMall_a()) + ERR_NUM) {
                //añade el RP a la lista de RPs adyacentes
                //añade una distancia mínima para el RP adyacente
                RPi->getActuator()->AdjacentRPs.Add(new TAdjacentRP(RPj, DBL_MAX, DBL_MAX));
            }
        }
        //para cada uno de los RPs posteriores al indicado
        for(int j=i+1; j<getCount(); j++) {
            //apunta el RP indicado para facilitar su acceso
            TRoboticPositioner *RPj = Items[j];
            //si están lo bastante cerca como para colisionar
            if(Mod(RPi->getActuator()->getP0() - RPj->getActuator()->getP0()) <
                    (RPi->getActuator()->getr_max() + RPi->getActuator()->getSPMall_a() +
                     RPj->getActuator()->getr_max() + RPj->getActuator()->getSPMall_a())) {
                //añade el RP a la lista de RPs adyacentes
                //añade una distancia mínima para el RP adyacente
                RPi->getActuator()->AdjacentRPs.Add(new TAdjacentRP(RPj, DBL_MAX, DBL_MAX));
            }
        }
    }
}
//ordena las listas de EAs adyacentes y RPs adyacentes en
//sentido levógiro empezando por el más próximo a 0
void TRoboticPositionerList1::sortAdjacents(void)
{
    TRoboticPositioner *RP, *RPA;

    //ninguno de los adyacentes debe ser el posicionador de referencia
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];
        //por cada posicionador adyacente
        for(int j=0; j<RP->getActuator()->AdjacentRPs.getCount(); j++) {
            //apunta el posicionador indicado para facilitar su acceso
            RPA = RP->getActuator()->AdjacentRPs[j].RP;
            //si el adyacente es el posicionador de referencia
            if(RPA->getActuator()->getId() == RP->getActuator()->getId())
                throw EImproperCall("adjacent positioner should not be same the reference positioner");
        }
    }

    /*        //define un par (RP, theta) para facilitar la ordenación
        class TPair {
        public:
                //puntero al posicionador adscrito
                //valor por defecto: NULL
                TRoboticPositioner *RP;
                //coordenada angular de la posición del posicionador adscrito
                //respecto al posicionador de referencia
                //valor por defecto: 0
                double theta;

                //compara los ángulos de dos pares
                static int  Comparetheta(const TPair *P1, const TPair *P2) {
                        //el puntero P1 debe apuntar a un par construido
                        if(P1 == NULL)
                                throw EImproperArgument("pointer P1 should point to bult pair");
                        //el puntero P2 debe apuntar a un par construido
                        if(P2 == NULL)
                                throw EImproperArgument("pointer P2 should point to bult pair");

                        //compara los ángulos y devuelve el valor correspondiente
                        if(P1->theta < P2->theta)
                                return -1;
                        if(P1->theta > P2->theta)
                                return 1;
                        return 0;
                }

                //imprime el ángulo al final de una cadena
                static void  Printtheta(AnsiString &S, const TPair *P) {
                        //el puntero P debe apuntar a un par construido
                        if(P == NULL)
                                throw EImproperArgument("pointer P should point to bult pair");

                        //imprime el ámgulo en la cadena
                        S += FloatToStr(P->theta);
                }

                //construye un par (RP, theta)
                TPair(TRoboticPositioner *t_RP=NULL, double t_theta=0) {
                        //asigna los valores de oficio
                        RP = t_RP;
                        theta = t_theta;
                }
        };
                 */
    //ADVERTENCIA: al ser una clase local el depurador no reconocerá
    //las propiedades de los pares.

    TPairTD<TRoboticPositioner> *P;
    TPointersList<TPairTD<TRoboticPositioner> > LP(6, TPairTD<TRoboticPositioner>::Comparex,
                                                   NULL, NULL, TPairTD<TRoboticPositioner>::Printx);

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];

        //vacia la lista de punteros a pares
        LP.Clear();

        //por cada posicionador adyacente
        for(int j=0; j<RP->getActuator()->AdjacentRPs.getCount(); j++) {
            //apunta el posicionador adyacente indicado para facilitar su acceso
            RPA = RP->getActuator()->AdjacentRPs[j].RP;

            //construye el par (RPA, theta)
            //con theta en [0, 2*M_PI)
            P = new TPairTD<TRoboticPositioner>(RPA);
            TDoublePoint V = RPA->getActuator()->getP0() - RP->getActuator()->getP0();
            if(Mod(V) != 0)
                P->x = ArgPos(V);
            else
                P->x = 0;

            //ADVERTENCIA: un posicionador adyacente puede estar
            //ubicado en el mismo lugar que el de referencia.

            //añade el par a la lista
            LP.Add(P);
        }

        AnsiString Saux = LP.getText();

        //ordena la lista de posicionadores de menor a mayor ángulo
        if(LP.getCount() > 1)
            LP.SortInc(0, LP.getCount()-1);

        //transcribe la lista
        RP->getActuator()->AdjacentRPs.Clear();
        for(int j=0; j<LP.getCount(); j++)
            RP->getActuator()->AdjacentRPs.Add(new TAdjacentRP(LP[j].P));
    }
}

//para cada posicionador de la lista calcula:
//      su radio de seguridad Rbsaf;
//      sus propiedades de la zona segura.
void TRoboticPositionerList1::calculateSafeParameters(void)
{
    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++)
        //calcula las propiedades de seguridad
        Items[i]->getActuator()->calculateSafeParameters();
}

//calcula las propiedades de dimensionamiento:
//      (LO3max, x3min, x3max, y3min, y3max)
void TRoboticPositionerList1::calculateSizingParameters(void)
{
    //DETERMINA LO3Max:

    TRoboticPositioner *RP;
    double LO0;
    double LO3;
    p_LO3max = 0; //inicializa la distancia máxima

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];

        //calcula el módulo al centro del posicionador
        LO0 = Mod(RP->getActuator()->getP0() - getO());
        //calcula la distancia máxima a P3
        LO3 = LO0 + RP->getActuator()->getL03max();
        //actualiza la distancia máxima
        if(LO3 > p_LO3max)
            p_LO3max = LO3;

        //actualiza x3min
        if(RP->getActuator()->getx3min() < getx3min())
            p_x3min = RP->getActuator()->getx3min();
        //actualiza x3max
        if(RP->getActuator()->getx3max() > getx3max())
            p_x3max = RP->getActuator()->getx3max();
        //actualiza y3min
        if(RP->getActuator()->gety3min() < gety3min())
            p_y3min = RP->getActuator()->gety3min();
        //actualiza y3max
        if(RP->getActuator()->gety3max() > gety3max())
            p_y3max = RP->getActuator()->gety3max();
    }
}
//calcula todos los parámetros derivados de la geometría del sistema
//      (Spt, Set, Ret, FiberDensity)
void TRoboticPositionerList1::calculateAreaParameters(void)
{
    //calcula el área participativa:
    p_Spt = 0;
    for(int i=0; i<getCount(); i++)
        p_Spt += Items[i]->getActuator()->getSpt();

    //calcula el área exclusiva:
    p_Set = 0;
    for(int i=0; i<getCount(); i++)
        p_Set += Items[i]->getActuator()->getSet();

    //calcula la fracción de área exclusiva:
    if(getSpt() > 0)
        p_Ret = getSet()/getSpt();
    else
        p_Ret = 0;

    //Ret tambien puede calcularse mediante:
    //double p_Ret = 0;
    //for(int i=0; i<getCount(); i++)
    //        p_Ret += RoboticPositioners[i]->Ret*RoboticPositioners[i]->Spt;

    //calcula la densidad de fibras:
    if(getSpt() > 0)
        p_FiberDensity = double(getCount())/getSpt();
    else
        p_FiberDensity = 0;
}

//invoca a todos los métodos de asimilación de
//los parámetros de dimensionamiento:
//      determineAdjacents
//      sortAdjacents
//      calculateSafeParameters();
//      calculateAreaParameters
void TRoboticPositionerList1::assimilateSizing(const TExclusionAreaList& EAL)
{
    determineAdjacents(EAL);
    sortAdjacents();
    calculateSafeParameters();
    calculateSizingParameters();
    calculateAreaParameters();
}

//asimila la configurración de posicionadores dada ejecutando:
//      calculateSPMComponents();
//      assimilateSizing();
void TRoboticPositionerList1::assimilate(const TExclusionAreaList& EAL)
{
        calculateSPMcomponents();
        assimilateSizing(EAL);
}

//MÉTODOS DE LECTURA CONJUNTA:

//get the PPA list in steps
void TRoboticPositionerList1::getPositions(TPairPositionAnglesList& PPAL) const
{
    //initialize the PPA list
    PPAL.Clear();

    //for each RP
    for(int i=0; i<getCount(); i++) {
        //point the indicated RP to facilitate its access
        TRoboticPositioner *RP = Items[i];
        //builds a PPA attached to the RP
        TPairPositionAngles *PPA = new TPairPositionAngles(RP);
        //assign the position angles of the rotors
        PPA->p_1 = RP->getActuator()->getp_1();
        PPA->p___3 = RP->getActuator()->getArm()->getp___3();
        //add the PPA to the list
        PPAL.Add(PPA);
    }
}
//get the corner 1 list in cartesian coordinates respect S0
void TRoboticPositionerList1::getPositionsCorners1(TItemsList<TDoublePoint>& Corners1) const
{
    //resize the vector
    Corners1.setCount(getCount());

    //for each RP
    for(int i=0; i<getCount(); i++) {
        //point the indicated RP to facilitate its access
        TRoboticPositioner *RP = Items[i];
        //assign the number of figures of the contour
        int N = RP->getActuator()->getArm()->getContour().getCount();
        //assign the position angles of the rotors
        Corners1[i].x = RP->getActuator()->getArm()->getContour()[N - 1]->getPa().x;
        Corners1[i].y = RP->getActuator()->getArm()->getContour()[N - 1]->getPa().y;
    }
}
//get the corner 2 list in cartesian coordinates respect S0
void TRoboticPositionerList1::getPositionsCorners2(TItemsList<TDoublePoint>& Corners2) const
{
    //resize the vector
    Corners2.setCount(getCount());

    //for each RP
    for(int i=0; i<getCount(); i++) {
        //point the indicated RP to facilitate its access
        TRoboticPositioner *RP = Items[i];
        //assign the number of figures of the contour
        int N = RP->getActuator()->getArm()->getContour().getCount();
        //assign the position angles of the rotors
        Corners2[i].x = RP->getActuator()->getArm()->getContour()[N - 1]->getPb().x;
        Corners2[i].y = RP->getActuator()->getArm()->getContour()[N - 1]->getPb().y;
    }
}

//MÉTODOS DE ASIGNACIÓN CONJUNTA:

//asigna las posiciones angulares de los ejes
//este método es atómico
void TRoboticPositionerList1::setPositions(const TPairPositionAnglesList& PositionList)
{
    //la lista de posiciones debe tener un par por cada posicionador
    if(PositionList.getCount() != getCount())
        throw EImproperArgument("list PositionList should contain a pair by each robotic positioner");

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        TRoboticPositioner *RP = Items[i];
        //apunta el par indicado para facilitar su acceso
        const TPairPositionAngles *PPA = PositionList.GetPointer(i);
        //las posiciones angulares del eje 1 debe estar en el dominio del eje 1
        if(RP->getActuator()->isntInDomainp_1(PPA->p_1))
            throw EImproperArgument("the axis 1 angular position should be in the domain of axis 1");
        //las posiciones angulares del eje 2 debe estar en el dominio del eje 2
        if(RP->getActuator()->getArm()->isntInDomainp___3(PPA->p___3))
            throw EImproperArgument("the axis 2 angular position should be in the domain of axis 2");
    }

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        TRoboticPositioner *RP = Items[i];
        //apunta el par indicado para facilitar su acceso
        const TPairPositionAngles *PPA = PositionList.GetPointer(i);
        //asigna las posiciones angulares
        RP->getActuator()->setAnglesSteps(PPA->p_1, PPA->p___3);
    }
}
//asigna conjuntamente los márgenes de segudidad
//      (PAem, Pem)
void TRoboticPositionerList1::setTolerance(double PAem,double Pem)
{
    //el valor de PAem debe ser no negativo
    if(PAem < 0)
        throw EImproperArgument("PAem value should be nonnegative");

    //el valor de Pem debe ser no negativo
    if(Pem < 0)
        throw EImproperArgument("Pem value should be nonnegative");

    p_PAem = PAem; //asigna el nuevo valor
    p_Pem = Pem; //asigna el nuevo valor

    //asimila (PAem, Pem)
    calculateSPMcomponents();
}

//--------------------------------------------------------------------------
//MÉTODOS DE DOMINIO CONJUNTO:

//determina el intervalo cartesiano que contiene
//el dominio de todos los posicionadores
//si el número de posicionadores de la lista es menor que uno
//lanza una excepcion EImproperCall
void TRoboticPositionerList1::getDomainP3s(double &x3min, double &x3max,
                                           double &y3min, double &y3max)
{
    //cheack the precondition
    if(getCount() <= 0)
        throw EImproperCall("for determine thejointly domain, the list should contains one RP almost");

    //Here the list has one RP almost.

    //initialize the limits
    x3min = DBL_MAX;
    y3min = DBL_MAX;
    x3max = -DBL_MAX;
    y3max = -DBL_MAX;

    //for each RP of the list, actualize the limits
    for(int i=0; i<getCount(); i++) {
        TRoboticPositioner *RP = Items[i];

        if(RP->getActuator()->getx3min() < x3min)
            x3min = RP->getActuator()->getx3min();
        if(RP->getActuator()->gety3min() < y3min)
            y3min = RP->getActuator()->gety3min();
        if(RP->getActuator()->getx3max() > x3max)
            x3max = RP->getActuator()->getx3max();
        if(RP->getActuator()->gety3max() > y3max)
            y3max = RP->getActuator()->gety3max();
    }
}
//determina si un punto se encuentra dentro del círculo
//que contiene el dominio conjunto de los posicionadores
bool TRoboticPositionerList1::isInCircle(const TDoublePoint &P)
{
    if(Mod(P - getO()) > getLO3max())
        return true;
    return false;
}
//determina si un punto se encuentra dentro del cuadrado
//que contiene el dominio conjunto de los posicionadores
bool TRoboticPositionerList1::isInSquare(const TDoublePoint &P)
{
    double x = P.x - getO().x;
    double y = P.y - getO().y;
    if(x<getx3min() || getx3max()<x ||y<gety3min() || gety3max()<y)
        return false;
    return true;
}

//determine if all RPs of the list are in secure positions
bool TRoboticPositionerList1::allRPsAreInSecurePosition(void) const
{
    //for each RP of the list
    for(int i=0; i<getCount(); i++)
        //if the RP is out the safe area
        if(Items[i]->getActuator()->ArmIsOutSafeArea())
            //indicate that not all RPsare in securepositions
            return false;

    //indicates that all RPs are in secure positions
    return true;
}

//determine if all operative RPs of the list are in the origin
bool TRoboticPositionerList1::allOperativeRPsAreInTheOrigin(void) const
{
    //for each RP of the list
    for(int i=0; i<getCount(); i++) {
        TRoboticPositioner *RP = Items[i];
        //if the RP is operative and out the safe area
        if(RP->getOperative() && RP->getActuator()->isOutTheOrigin())
            //indicate that not all RPsare in securepositions
            return false;
    }

    //indicates that all operative RPs are in secure positions
    return true;
}

//---------------------------------------------------------------------------
//MÉTODOS DE DESPLAZAMIENTO:

//mueve los ejes de los posicionadores
//a sus orígenes de coordenadas
void TRoboticPositionerList1::moveToOrigins(void)
{
    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++)
        //mueve el posicionador al origen
        Items[i]->getActuator()->setAnglesZeroSteps();
}
/*//mueve los ejes de los posicionadores seleccionados
//a sus orígenes de coordenadas
int TRoboticPositionerList1::moveToOriginsSelected(void)
{
    TRoboticPositioner *RP;
    int count = 0;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];
        //si el posicionador está seleccionado
        if(RP->getActuator()->Selected) {
            //mueve el posicionador al origen
            RP->getActuator()->setAnglesZeroSteps();
            //contabiliza el posicionador seleccionado
            count++;
        }
    }

    return count;
}*/

//mueve los brazos que están fuera del área de seguridad a
//las posiciones de seguridad estables más próximas
void TRoboticPositionerList1::moveOutsideArmsToSafePositions(void)
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];
        //si el brazo está extendido
        if(RP->getActuator()->ArmIsOutSafeArea())
            //lo mueve hasta la posición de seguridad
            //estable más próxima
            RP->getActuator()->moveArmToSafePosition();
    }
}
/*//mueve los brazos de los posicionadores seleccionados que
//están fuera del área de seguridad a las posiciones de
//seguridad estables más próximas
int TRoboticPositionerList1::MoveOutsideArmsToSafePositionsSelected(void)
{
    TRoboticPositioner *RP;
    int count = 0;

    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador inicado para facilitar su acceso
        RP = Items[i];
        //si el posicionador está seleccionado
        if(RP->getActuator()->Selected) {
            //si el brazo está extendido
            if(RP->getActuator()->ArmIsOutSafeArea())
                //mueve el brazo hasta la posición de
                //seguridad estable más próxima
                RP->getActuator()->moveArmToSafePosition();
            //contabiliza el posicionador seleccionado
            count++;
        }
    }

    return count;
}*/

//--------------------------------------------------------------------------
//METHODS TO SEGREGATE RPs:

//segrega los posicionadores en dos listas:
//      lista con el brazo dentro del área de seguridad;
//      lista con el brazo fuera del área de seguridad;
void TRoboticPositionerList1::segregateInOut(TRoboticPositionerList1 &Inners,
                                             TRoboticPositionerList1 &Outsiders) const
{
    //reinicializa las listas
    Inners.Clear();
    Outsiders.Clear();

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        TRoboticPositioner *RP = Items[i];

        //si el posicionador está operativo
        if(RP->getOperative()) {
            //si el brazo del posicionador está dentro del área de seguridad
            if(RP->getActuator()->ArmIsInSafeArea())
                //añade el posicionador a la lista de internos
                Inners.Add(RP);
            //si no (si está fuera)
            else
                //añade el posicionador a la lista de externos
                Outsiders.Add(RP);
        }
    }
}

//segregates the operative RPs in unsecure positions
void TRoboticPositionerList1::segregateOperativeOutsiders(
        TRoboticPositionerList1& Outsiders) const
{
    Outsiders.Clear();
    for(int i=0; i<getCount(); i++) {
        TRoboticPositioner *RP = Items[i];
        if(RP->getOperative() && RP->getActuator()->ArmIsOutSafeArea())
            Outsiders.Add(RP);
    }
}

//segregates the operative RPs in security positions
//out the origins
void TRoboticPositionerList1::segregateOperativeInnersOutTheOrigins(
        TRoboticPositionerList1& Inners) const
{
    Inners.Clear();
    for(int i=0; i<getCount(); i++) {
        TRoboticPositioner *RP = Items[i];
        if(RP->getOperative() && RP->getActuator()->ArmIsInSafeArea() && RP->getActuator()->isOutTheOrigin())
            Inners.Add(RP);
    }
}

//segregates the collided RPs
void TRoboticPositionerList1::segregateCollided(TRoboticPositionerList1& Collided,
                                                TRoboticPositionerList1& notCollided) const
{
    Collided.Clear();
    notCollided.Clear();
    for(int i=0; i<getCount(); i++) {
        TRoboticPositioner *RP = Items[i];
        if(RP->getActuator()->thereIsCollisionWithAdjacent())
            Collided.Add(RP);
        else
            notCollided.Add(RP);
    }
}

/*//segrega losposicionadores seleccionados en una lista
void TRoboticPositionerList1::segregateSelected(TRoboticPositionerList1& RPL) const
{
    //inicializa la lista de posicionadores
    RPL.Clear();

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        TRoboticPositioner *RP = Items[i];

        //si el posicionador está seleccionado
        if(RP->getActuator()->Selected)
            RPL.Add(RP); //apunta el poisicionador en la  lista
    }
}
*/
//MÉTODOS DE APILADO DE POSICIONES ANGULARES:

//apila las posiciones de cada posicionador de la lista
void TRoboticPositionerList1::pushPositions(void)
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];
        //apila la posición del cilindro
        RP->getActuator()->pushtheta_1();
        //apila la posición del brazo
        RP->getActuator()->getArm()->pushtheta___3();
    }
}
//restaura la siguiente posición apilada de cada posicionador
//sin desempilarla
void TRoboticPositionerList1::restorePositions(void)
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];

        //restaura la posición del actuador
        RP->getActuator()->restorethetas();
    }
}
//desempila la siguiente posición apilada de cada posicionador
void TRoboticPositionerList1::popPositions(void)
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];
        //desempila la posición del cilindro
        RP->getActuator()->poptheta_1();
        //desempila la posición del brazo
        RP->getActuator()->getArm()->poptheta___3();
    }
}
//restaura y desempila la siguiente posición apilada de cad aposicionador
void TRoboticPositionerList1::restoreAndPopPositions(void)
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];

        //restaura la posición del cilindro
        RP->getActuator()->restoretheta_1();
        //restaura la posición del brazo
        RP->getActuator()->getArm()->restoretheta___3();

        //desempila la posición del cilindro
        RP->getActuator()->poptheta_1();
        //desempila la posición del brazo
        RP->getActuator()->getArm()->poptheta___3();
    }
}

//apila la posición de los brazos de todos los posicionadores
void TRoboticPositionerList1::pushArmtheta___3s()
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];
        //apila la posición del brazo
        RP->getActuator()->getArm()->pushtheta___3();
    }
}
//restaura la últimaposición apilada del brazo de cada posicionador
void TRoboticPositionerList1::restoreArmtheta___3s(void)
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];

        try {
            //restaura la posición del brazo
            RP->getActuator()->getArm()->restoretheta___3();
        } catch(...) {
            throw;
        }
    }
}
//restaura la posición almacenada del brazo de cada posicionador
void TRoboticPositionerList1::popArmtheta___3s(void)
{
    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++)
        //restaura la posición apilada del brazo
        Items[i]->getActuator()->getArm()->poptheta___3();
}

//MÉTODOS DE PILA DE CUANTIFICADORES:

//apila el estado de
//los cuantificadores de cada posicionador de la lista
void TRoboticPositionerList1::pushQuantifys(void)
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];

        //apila y activa el estado de cuantificación del cilindro
        RP->getActuator()->pushQuantify_();
        //apila y desactiva el estado de cuantificación del brazo
        RP->getActuator()->getArm()->pushQuantify___();
    }
}
//restaura el último estado apilado de
//los cuantificadores de cada posicionador de la lista
void TRoboticPositionerList1::restoreQuantifys(void)
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];

        //restaura el estado del cuantificador del eje 1
        RP->getActuator()->restoreQuantify_();
        //restaura el estado del cuantificador del eje 2
        RP->getActuator()->getArm()->restoreQuantify___();
    }
}
//desempila el último estado apilado de
//los cuantificadores de cada posicionador de la lista
void TRoboticPositionerList1::popQuantifys(void)
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];

        //desempila el estado del cuantificador del eje 1
        RP->getActuator()->popQuantify_();
        //desempila el estado del cuantificador del eje 2
        RP->getActuator()->getArm()->popQuantify___();
    }
}
//restaura y desempila el último estado apilado de
//los cuantificadores de cada posicionador de la lista
void TRoboticPositionerList1::restoreAndPopQuantifys(void)
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];

        //restaura y desempila el estado del cuantificador del eje 1
        RP->getActuator()->restoreQuantify_();
        RP->getActuator()->popQuantify_();

        //restaura y desempila el estado del cuantificador del eje 2
        RP->getActuator()->getArm()->restoreQuantify___();
        RP->getActuator()->getArm()->popQuantify___();
    }
}

//apila el estado
//del cuantificador del brazo de cada posicionador de la lista
void TRoboticPositionerList1::pushArmQuantify___s(void)
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];

        //apila y desactiva el estado de cuantificación del brazo
        RP->getActuator()->getArm()->pushQuantify___();
    }
}
//restaura el último estado apilado
//del cuantificador del brazo de cada posicionador de la lista
void TRoboticPositionerList1::restoreArmQuantify___s(void)
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];

        //restaura el estado del cuantificador del eje 2
        RP->getActuator()->getArm()->restoreQuantify___();
    }
}
//desempila el último estado apilado
//del cuantificador del brazo de cada posicionador de la lista
void TRoboticPositionerList1::popArmQuantify___s(void)
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];

        //desempila el estado del cuantificador del eje 2
        RP->getActuator()->getArm()->popQuantify___();
    }
}
//restaura y desempila el último estado apilado
//del cuantificador del brazo de cada posicionador de la lista
void TRoboticPositionerList1::restoreAndPopArmQuantify___s(void)
{
    TRoboticPositioner *RP;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];

        //restaura y desempila el estado del cuantificador del eje 2
        RP->getActuator()->getArm()->restoreQuantify___();
        RP->getActuator()->getArm()->popQuantify___();
    }
}

//--------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR SI HAY COLISIONES:

//levanta las banderas indicadoras de determinación de colisión pendiente
//de todos los RPs de la lista
void TRoboticPositionerList1::enablePending(void)
{
    for(int i=0; i<getCount(); i++)
        Items[i]->getActuator()->Pending = true;
}

//determina si algún actuador de algún RP colisiona con
//algún actuador o barrera de algún RP o EA adyacente
bool TRoboticPositionerList1::thereIsCollision(void)
{
    //inicializa las banderas de determinación de colisión pendiente
    enablePending();

    //busca colisión del actuador de cada RP con algún actuador o barrera de RP o EA adyacente,
    //exceptuando aquellos con los que se ha comprobado ya
    for(int i= 0; i<getCount(); i++) {
        //apunta el RP indicado para facilitar su acceso
        TRoboticPositioner *RP = Items[i];
        //si el actudor del posicionador indicado colisiona con
        //el actuador o barrera de algún RP o EA adyacente con
        //determinación de colisión pendiente
        if(RP->getActuator()->thereIsCollisionWithPendingAdjacent())
            //indica que hay colisión
            return true;
        else
            //desactiva la bandera para evitar que
            //se vuelva a determinar la colisión
            RP->getActuator()->Pending = false;
    }

    return false; //indica que no se ha encontrado invasión
}
//busca los RPs de la lista cuyo actuador colisiona con
//algún actuador o barrera de algún RP o EA adyacente
void TRoboticPositionerList1::searchCollinding(TVector<int> &indices)
{
/*    //ADVERTENCIA: los RPs de la lista no tienen
    //por que estar ordenados según su identificador.

    TRoboticPositioner *RP;
    TItemsList<TRoboticPositioner*> Collindings;

    //inicializa la salida
    indices.Clear();

    //inicializa las banderas de determinación de colisión pendiente
    enablePending();

    //por cada RP de la lista
    for(int i= 0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];
        //busca colisiones con los RPs o EAs adyacentes con los
        //que todavía no hayan determinado su estado de colisión
        RP->getActuator()->searchCollindingPendingAdjacent(Collindings);
        //indica que ya ha determinado el estado de colisión del posicionador
        RP->getActuator()->Pending = false;

        //si ha encontrado colisión con algún adyacente
        if(Collindings.getCount() > 0) {

            //añade el índice a la lista
            indices.Add(i);

            int m, n;

            //añade los índices de los posicionadores adyacentes
            //que tengan un posicionador adscrito en la lista y
            //no hayan sido añadidos previamente
            for(int j=0; j<Collindings.getCount(); j++) {
                //apunta el actuador indicado para facilitar su acceso
                RP = Collindings[j];

                //busca la pósición del posicionador correspondiente
                m = search(RP);

                //si noha encontrado el actuador en la lista
                if(m >= getCount())
                    //indica efecto lateral
                    throw EImpossibleError("lateral effect");

                //indica que ya ha determinado el estado de colisión del posicionador adyacente
                Items[m]->getActuator()->Pending = false;

                //busca el índice en la lista de añadidos
                n = indices.Search(m);
                //si la posición del posicionador no ha sido previamente agregada
                if(n >= indices.getCount())
                    //añade el el índice a la lista de añadidos
                    indices.Add(m);
            }
        }
    }

    //ordena la lista de índices en sentido creciente
    if(indices.getCount() > 1)
        indices.SortInc(0, indices.getCount()-1);
*/
    //inicializa la salida
    indices.Clear();

    //inicializa las banderas de determinación de colisión pendiente
    enablePending();

    //busca colisión del actuador de cada RP con algún actuador o barrera de RP o EA adyacente,
    //exceptuando aquellos con los que se ha comprobado ya
    for(int i= 0; i<getCount(); i++) {
        //apunta el RP indicado para facilitar su acceso
        TRoboticPositioner *RP = Items[i];
        //si el actudor del posicionador indicado colisiona con
        //el actuador o barrera de algún RP o EA adyacente con
        //determinación de colisión pendiente
        if(RP->getActuator()->thereIsCollisionWithPendingAdjacent()) {
            //busca el índice en la lista de añadidos
            int j = indices.Search(i);
            //si la posición del posicionador no ha sido previamente agregada
            if(j >= indices.getCount())
                //añade el el índice a la lista de añadidos
                indices.Add(j);
        }

        //desactiva la bandera para evitar que
        //se vuelva a determinar la colisión
        RP->getActuator()->Pending = false;
    }

    //ordena la lista de índices en sentido creciente
    if(indices.getCount() > 1)
        indices.SortInc(0, indices.getCount()-1);
}
/*//obtiene los conjuntos de RPs en colisión (Collision Cluster List)
void TRoboticPositionerList1::getCollisionClusterList(TPointersList<TItemsList<TRoboticPositioner*> > &CCL)
{
    TItemsList<TRoboticPositioner*> Collindings;
    TItemsList<TRoboticPositioner*> *CC;

    //inicializa la salida
    CCL.Clear();

    //levanta las banderas indicadoras de determinación de colisión pendiente
    //de todos los RPs de la lista
    enablePending();

    //por cada RP de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el RP indicado para facilitar su acceso
        TRoboticPositioner *RP = Items[i];

        //busca los PRs adyacentes que colisionan
        RP->getActuator()->searchCollindingPendingAdjacent(Collindings);

        //si ha encontrado alguna nueva colisión
        if(Collindings.getCount() > 0) {
            //contruye una lista de punteros a posicionadores
            CC = new TItemsList<TRoboticPositioner*>();
            //apunta el posicionador y los colindantes en la lista
            CC->Add(RP);
            //desactiva la bandera del posicionador
            RP->getActuator()->Pending = false;
            //por cada posicionador colindante en colisión
            for(int j=0; j<Collindings.getCount(); j++) {
                //apunta el posicionador para facilitar su acceso
                RP = Collindings[j];
                //apunta el posicionador en el conjunto
                CC->Add(RP);
                //desactiva la bandera del posicionador
                RP->getActuator()->Pending = false;
            }
            //añade la nueva lista al resultado
            CCL.Add(CC);
        }
    }
}*/

//RANDOMIZADO DE LOS EJES:

//lleva los rotores de los RPs a posiciones aleatorias
//con distribución uniforme en sus dominios
void TRoboticPositionerList1::randomize(void)
{
    TRoboticPositioner *RP;

    //por cada RP de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el RP indicado para facilitar su acceso
        RP = Items[i];
        //randomiza las posiciones de sus ejes
        RP->getActuator()->randomizep_1();
        RP->getActuator()->getArm()->randomizep___3();
    }
}
/*//lleva los ejes de los posicionadores seleccionados a
//posiciones aleatorias con distribución uniforme en sus dominios
int TRoboticPositionerList1::randomizeSelected(void)
{
    TRoboticPositioner *RP;
    int count = 0;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];
        //si el posicionador está seleccionado
        if(RP->getActuator()->Selected) {
            //randomiza las posiciones de sus ejes
            RP->getActuator()->randomizep_1();
            RP->getActuator()->getArm()->randomizep___3();
            //contabiliza el posicionador seleccionado
            count++;
        }
    }

    return count;
}*/

//lleva los rotores de los RPs a posiciones aleatorias
//con distribución uniforme en sus dominios
//en las que no colisionan
void TRoboticPositionerList1::randomizeWithoutCollision(void)
{
    TRoboticPositioner *RP;

    //para cada RP de la lista
    for(int i=0; i<getCount(); i++)
        do {
        //apunta el RP indicado para facilitar su acceso
        RP = Items[i];
        //randomiza los ejes del posicionador
        RP->getActuator()->randomizep_1();
        RP->getActuator()->getArm()->randomizep___3();
        //mientras colisione con algún adyacente
    } while(RP->getActuator()->thereIsCollisionWithAdjacent());
}
/*//lleva los ejes de los posicionadores seleccionados a
//posiciones aleatorias con distribución uniforme en
//sus dominios en las que no colisionan entre si
int TRoboticPositionerList1::randomizeWithoutCollisionSelected(void)
{
    TRoboticPositioner *RP;
    bool collision;
    int count = 0;

    //para cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        collision = false;
        do {
            //apunta el posicionador indicado para facilitar su acceso
            RP = Items[i];
            if(RP->getActuator()->Selected) {
                //lleva randomiza los ejes del posicionador
                RP->getActuator()->randomizep_1();
                RP->getActuator()->getArm()->randomizep___3();
                //determina si hay colisión
                collision = RP->getActuator()->thereIsCollisionWithAdjacent();
                //si no hay colisión
                if(!collision)
                    //contabiliza el posicionador seleccionado
                    count++;
            }
            //mientras colisione con algún adyacente
        } while(collision);
    }

    return count;
}*/

//RANDOMIZADO DE P3:

//lleva el punto P3 de los RPs a posiciones aleatorias
//con distribución uniforme en su dominio
void TRoboticPositionerList1::randomizeP3(void)
{
    TRoboticPositioner *RP;

    //por cada RP de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el RP indicado para facilitar su acceso
        RP = Items[i];
        //randomiza el punto P3
        RP->getActuator()->randomizeP3();
    }
}
/*//lleva el punto P3 de los posicionadores seleccionados a
//posiciones aleatorias con distribución uniforme en su dominio
int TRoboticPositionerList1::randomizeP3Selected(void)
{
    TRoboticPositioner *RP;
    int count = 0;

    //por cada posicionador de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta el posicionador indicado para facilitar su acceso
        RP = Items[i];
        //si el posicionador está seleccionado
        if(RP->getActuator()->Selected) {
            //randomiza el punto P3
            RP->getActuator()->randomizeP3();
            //contabiliza el posicionador seleccionado
            count++;
        }
    }

    return count;
}*/

//lleva el punto P3 de los RPs a posiciones aleatorias
//con distribución uniforme en su dominio
//en las que no colisionan
void TRoboticPositionerList1::randomizeP3WithoutCollision(void)
{
    TRoboticPositioner *RP;

    //para cada RP de la lista
    for(int i=0; i<getCount(); i++)
        do {
        //apunta el RP indicado para facilitar su acceso
        RP = Items[i];
        //randomiza el punto P3
        RP->getActuator()->randomizeP3();
        //mientras colisione con algún adyacente
    } while(RP->getActuator()->thereIsCollisionWithAdjacent());
}
/*//lleva el punto P3 de los posicionadores seleccioandos a
//posiciones aleatorias con distribución uniforme en su dominio
//en las que no colisionan entre si
int TRoboticPositionerList1::randomizeP3WithoutCollisionSelected(void)
{
    TRoboticPositioner *RP;
    bool collision;
    int count = 0;

    //para cada posicionador de la lista de osicionadores
    for(int i=0; i<getCount(); i++) {
        collision = false;
        do {
            //apunta el posicionador indicado para facilitar su acceso
            RP = Items[i];
            if(RP->getActuator()->Selected) {
                //randomiza el punto P3
                RP->getActuator()->randomizeP3();
                //determina si hay colisión
                collision = RP->getActuator()->thereIsCollisionWithPendingAdjacent();
                //si no hay colisión
                if(!collision)
                    //contabiliza el posicionador seleccionado
                    count++;
            }
            //mientras colisione con algún adyacente
        } while(collision);
    }

    return count;
}*/

//METHODS RELATED WITH THE MP:

//search the RP with the minimun Dsec
int TRoboticPositionerList1::searchDsecMin(double DsecMin) const
{
    if(getCount() <= 0)
        throw EImproperCall("there aren't RPs for search the minimun Dsec");

    TRoboticPositioner *RP = getFirst();
    DsecMin = RP->getDsec();
    int iMin = 0;
    for(int i=1; i<getCount(); i++) {
        RP = Items[i];
        if(RP->getDsec() < DsecMin) {
            DsecMin = RP->getDsec();
            iMin = i;
        }
    }
    return iMin;
}
//get the minimun Dsec
double TRoboticPositionerList1::getDsecMin() const
{
    double DsecMin = DBL_MAX;

    if(getCount() <= 0)
        return DsecMin;

    searchDsecMin(DsecMin);
    return DsecMin;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
