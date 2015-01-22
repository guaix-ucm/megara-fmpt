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
//Archivo: RoboticPositionerList1.cpp
//Contenido: lista de posicionadores con propiedades dimensionales
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "RoboticPositionerList1.h"
#include "Strings.h"
#include "Scalars.h"
#include "vclemu.h"

//#include <values.h>

//---------------------------------------------------------------------------

using namespace Strings;
using namespace Mathematics;

//espacio de nombres de modelos
namespace Models {

//###########################################################################
//Métodos de TRoboticPositionerList1:
//###########################################################################

//PROPIEDADES DE SEGURIDAD:

void TRoboticPositionerList1::setPAem(double _PAem)
{
        //el valor de PAem debe ser no negativo
        if(_PAem < 0)
                throw EImproperArgument("PAem value should be nonnegative");

        __PAem = _PAem; //asigna el nuevo valor

        //asimila PAem
        CalculateSPMComponents();
}
void TRoboticPositionerList1::setPem(double _Pem)
{
        //el valor de Pem debe ser no negativo
        if(_Pem < 0)
                throw EImproperArgument("Pem value should be nonnegative");

        __Pem = _Pem; //asigna el nuevo valor

        //asimila Pem
        CalculateSPMComponents();
}

//---------------------------------------------------------------------------
//PROPIEDADES DE ÁREA:

//PROPIEDADES MECÁNICAS EN FORMATO TEXTO:

AnsiString TRoboticPositionerList1::getLO3maxText(void) const
{
        return FloatToStr(getLO3max());
}

//PROPIEDADES DE SEGURIDAD EN FORMATO TEXTO:

AnsiString TRoboticPositionerList1::getPAemText(void) const
{
        return FloatToStr(getPAem());
}
void TRoboticPositionerList1::setPAemText(const AnsiString &S)
{
        try {
                setPAem(StrToFloat_(S));
        } catch(...) {
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
                setPem(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}

//PROPIEDADES DE ÁREA EN FORMATO TEXTO:

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

//PROPIEDADES GRÁFICAS EN FORMATO TEXTO:
/*#
AnsiString TRoboticPositionerList1::getLimitDomainColorText(void) const
{
        return ColorToString(LimitDomainColor);
}
void TRoboticPositionerList1::setLimitDomainColorText(const AnsiString &S)
{
        try {
                LimitDomainColor = StringToColor(S);
        } catch(...) {
                throw;
        }
}
*/
AnsiString TRoboticPositionerList1::getPaintActuators_Text(void) const
{
        return BoolToStr_(PaintActuators_);
}
void TRoboticPositionerList1::setPaintActuators_Text(const AnsiString &S)
{
        try {
                PaintActuators_ = StrToBool_(S);
        } catch(...) {
                throw;
        }
}
AnsiString TRoboticPositionerList1::getPaintLimitDomain_Text(void) const
{
        return BoolToStr_(PaintLimitDomain_);
}
void TRoboticPositionerList1::setPaintLimitDomain_Text(const AnsiString &S)
{
        try {
                PaintLimitDomain_ = StrToBool_(S);
        } catch(...) {
                throw;
        }
}
AnsiString TRoboticPositionerList1::getPaintMap_Text(void) const
{
        return BoolToStr_(PaintMap_);
}
void TRoboticPositionerList1::setPaintMap_Text(const AnsiString &S)
{
        try {
                PaintMap_ = StrToBool_(S);
        } catch(...) {
                throw;
        }
}

//PROPIEDADES DE LOCALIZACIÓN EN FORMATO TEXTO:

AnsiString TRoboticPositionerList1::getOText(void) const
{
        return DPointToStr(getO());
}

//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

//get the origins table in text format:
//      Id      x0      y0      thetaO1
AnsiString TRoboticPositionerList1::getOriginsTableText(void) const
{
        //guarda el valor de Print
        void ( *PrintBak)(AnsiString&, TRoboticPositioner*);
        PrintBak = Print;

        //apunta la lista de posicionadores con un puntero no constante para facilitar su escritura
        TRoboticPositionerList1 *RPL = (TRoboticPositionerList1*)this;

        //apunta la función de impresión de coordenadas de origen en formato linea de texto
        RPL->Print = TRoboticPositioner::PrintOriginsRow;
        //obtiene la lista de coordenadas de origen en una cadena de texto
        AnsiString S = RPL->getColumnText();

        //restaura el valor de Print
        RPL->Print = PrintBak;

        return S; //devuelve la cadena de texto
}
//set the origins table in text format:
//      Id      x0      y0      thetaO1
void TRoboticPositionerList1::setOriginsTableText(const AnsiString &S)
{
        try {
                //indica al principio de la cadena
                int i = 1;
                //atraviesa las etiquetas de la cabecera
                TActuator::TravelOriginsLabelsRow(S, i);

                //contruye una variable tampón
                TPointersList<TRoboticPositioner> RPL;
                //apunta la función de lectura de coordenadas de origen en formato linea de texto
                RPL.Read = TRoboticPositioner::ReadOriginsRow;
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

        } catch(...) {
                throw;
        }
}
//get the P3 table in text format:
//      Id      x3      y3
AnsiString TRoboticPositionerList1::getPositionsP3TableText(void) const
{
        //guarda el valor de Print
        void ( *PrintBak)(AnsiString&, TRoboticPositioner*);
        PrintBak = Print;

        //apunta la lista de posicionadores con un puntero no constante para facilitar su escritura
        TRoboticPositionerList1 *RPL = (TRoboticPositionerList1*)this;

        //apunta la función de impresión de coordenadas de posición en formato linea de texto
        RPL->Print = TRoboticPositioner::PrintPositionP3Row;
        //obtiene la lista de coordenadas de posición en una cadena de texto
        AnsiString S = RPL->getColumnText();

        //restaura el valor de Print
        RPL->Print = PrintBak;

        return S; //devuelve la cadena de texto
}
//set the P3 table in text format
//      Id      x3      y3
void TRoboticPositionerList1::setPositionsP3TableText(const AnsiString &S)
{
        try {
                //indica al principio de la cadena
                int i = 1;
                //atraviesa las etiquetas de la cabecera
                TActuator::TravelPositionP3LabelsRow(S, i);

                //contruye una variable tampón
                TRoboticPositionerList1 RPL(this);
                //apunta la función de lectura de coordenadas de posición en formato linea de texto
                RPL.Read = TRoboticPositioner::ReadPositionP3Row;
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
                        if(SearchId(RP->getActuator()->getId()) >= getCount())
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

        } catch(...) {
                throw;
        }
}
//get the PPA table in text format:
//      Id      p_1     p___3
AnsiString TRoboticPositionerList1::getPositionsPPATableText(void) const
{
    //save the value of Print
    void ( *PrintBak)(AnsiString&, TRoboticPositioner*);
    PrintBak = Print;

    //point the RPL with a nonconstant pointer to make this writable
    TRoboticPositionerList1 *RPL = (TRoboticPositionerList1*)this;

    //point the required print function
    RPL->Print = TRoboticPositioner::PrintPositionPPARow;
    //get the PPA table in a text string
    AnsiString S = RPL->getColumnText();

    //restore theprint value
    RPL->Print = PrintBak;

    return S; //return the text string
}

AnsiString TRoboticPositionerList1::getToleranceText(void) const
{
        AnsiString S;

        S += AnsiString("PAem = ")+getPAemText()+AnsiString("\r\n");
        S += AnsiString("Pem = ")+getPemText();

        return S;
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
AnsiString TRoboticPositionerList1::getGraphicsText(void) const
{
        AnsiString S;

        //#S += AnsiString("LimitDomainColor: ")+getLimitDomainColorText()+AnsiString("\r\n");

        S += AnsiString("PaintActuators_: ")+getPaintActuators_Text()+AnsiString("\r\n");
        S += AnsiString("PaintLimitDomain_: ")+getPaintLimitDomain_Text()+AnsiString("\r\n");
        S += AnsiString("PaintMap_: ")+getPaintMap_Text();

        return S;
}

AnsiString TRoboticPositionerList1::getAllText(void) const
{
        AnsiString S;

//        S += "Components:";
        S += AnsiString("\r\nTolerance:\r\n")+StrIndent(getToleranceText());
        S += AnsiString("\r\nItemsAddress: ")+getItemsAddressText();
        S += AnsiString("\r\nSizing:\r\n")+StrIndent(getSizingText());
        S += AnsiString("\r\nArea:\r\n")+StrIndent(getAreaText());
        S += AnsiString("\r\nGraphics:\r\n")+StrIndent(getGraphicsText());

        return S;
}
AnsiString TRoboticPositionerList1::getInstanceText(void) const
{
        AnsiString S;

        //TOLERANCIAS DE FABRICACIÓN:

        S = "Tolerance:\r\n";
        S += StrIndent(getToleranceText());

        return S;
}
void TRoboticPositionerList1::setInstanceText(const AnsiString& S)
{
        try {
                //indica al principio de la cadena
                int i = 1;

                //contruye una variable tampón
                TRoboticPositionerList1 RPL(this);
                //lee la cadena en la variable tampón
                TRoboticPositionerList1 *aux = &RPL;
                ReadInstance((TRoboticPositionerList1*&)aux, S, i);

                //avanza el índice i hasta la próxima posición que no contenga un separador
                StrTravelSeparatorsIfAny(S, i);
                //si el índice i indica a algún caracter de la cadena S
                if(i <= S.Length())
                        //indica que la cadena S solo debería contener el valor para una instancia
                        throw EImproperArgument("string S should contain the instance value only");

                //asigna la variable tampón
                Clone(&RPL);

        } catch(...) {
                throw;
        }
}

//-------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:

//lee una instancia en una cadena
void  TRoboticPositionerList1::ReadInstance(TRoboticPositionerList1* &RPL,
        const AnsiString& S, int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el puntero RPL debería apuntar a una lista de posicionadores construida
        if(RPL == NULL)
                throw EImproperArgument("pointer RPL should point to built robotic positioner list");

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de la máquina de estados de lectura
        //      0: esperando etiqueta "Tolerance:"
        //      1: esperando separador y asignación a PAem
        //      2: esperando separador y asignación a Pem
        //      3: instancia de lista de posicionadores leida con éxito
        int status = 0;

        //variables tampón
        double _PAem, _Pem;

        do {
                switch(status) {
                        case 0: //esperando etiqueta "Tolerance:"
                                try {
                                        StrTravelLabel("Tolerance:", S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 1: //esperando separador y asignación a PAem
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("PAem", S, i);
                                        StrTravelLabel("=", S, i);
                                        StrReadFloat(_PAem, S,i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 2: //esperando separador y asignación a Pem
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("Pem", S, i);
                                        StrTravelLabel("=", S, i);
                                        StrReadFloat(_Pem, S,i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                }
        //mientras no se haya leido la instancia de la lista de posicionadores con éxito
        } while(status < 3);

        //asigna la variable tampón
        try {
                RPL->SetTolerance(_PAem, _Pem);
        }catch(...) {
                throw;
        }
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

//construye una lista de posicionadores
TRoboticPositionerList1::TRoboticPositionerList1(void) :
    TItemsList<TRoboticPositioner*>(100),
    //propiedades de seguridad
    __PAem(0.000001), __Pem(0.0001),
    //propiedades de interfaz
    //#LimitDomainColor(Qt::gray),
    PaintActuators_(true), PaintLimitDomain_(false), PaintMap_(false)
{
    //point the default functions
    Compare = TRoboticPositioner::CompareIds;
    Print = TRoboticPositioner::PrintId;

    //inicializa las propiedades de localización
    __O.x = 0;
    __O.y = 0;

    //asimila las propiedades inicializadas
//        AssimilateSizing();
}

//copia las propiedades de seguridad
void TRoboticPositionerList1::CopyTolerance(const TRoboticPositionerList1 *RPL)
{
        //el puntero RPL debería apuntar a una lista de posicionadores contruida
        if(RPL == NULL)
                throw EImproperArgument("pointer RPL should point to built fiber positioner list");

        __PAem = RPL->getPAem();
        __Pem = RPL->getPem();
}
//copia las propiedades de dimensionamiento
void TRoboticPositionerList1::CopySizing(const TRoboticPositionerList1 *RPL)
{
        //el puntero RPL debería apuntar a una lista de posicionadores contruida
        if(RPL == NULL)
                throw EImproperArgument("pointer RPL should point to built fiber positioner list");

        __O = RPL->getO();

        __LO3max = RPL->getLO3max();

        __x3min = RPL->getx3min();
        __x3max = RPL->getx3max();
        __y3min = RPL->gety3min();
        __y3max = RPL->gety3max();
}
//copia las propiedades de área
void TRoboticPositionerList1::CopyArea(const TRoboticPositionerList1 *RPL)
{
        //el puntero RPL debería apuntar a una lista de posicionadores contruida
        if(RPL == NULL)
                throw EImproperArgument("pointer RPL should point to built fiber positioner list");

        __Spt = RPL->getSpt();
        __Set = RPL->getSet();
        __Ret = RPL->getRet();
        __FiberDensity = RPL->getFiberDensity();
}
//copia las propiedades gráficas
//      (LimitDomainColor, PaintActuators_, PaintLimitDomain_, PaintMap)
void TRoboticPositionerList1::CopyGraphics(const TRoboticPositionerList1 *RPL)
{
        //el puntero RPL debe apuntar a una lista de posicionadores construida
        if(RPL == NULL)
                throw EImproperArgument("pointers RPL should point to built fiber positioner list");

        //#LimitDomainColor = RPL->LimitDomainColor;
        PaintActuators_ =RPL->PaintActuators_;
        PaintLimitDomain_ = RPL->PaintLimitDomain_;
        PaintMap_ = RPL->PaintMap_;
}

//clona una lista de posicionadores
//      (Tolerance, Sizing, Area, Graphics)
void TRoboticPositionerList1::Clone(const TRoboticPositionerList1 *RPL)
{
        //el puntero RPL debe apuntar a una lista de posicionadores construida
        if(RPL == NULL)
                throw EImproperArgument("pointers RPL should point to built robotic positioner list");

        //clona los posicionadores de la lista
        Items.Clone(RPL->Items);
        for(int i=0; i<RPL->getCount(); i++)
//                Add(new TRoboticPositioner(RPL->Get(i)));
                Items[i] = new TRoboticPositioner(RPL->Get(i));

        //apunta a las mismas funciones externas
        Compare = RPL->Compare;
        Evaluate = RPL->Evaluate;
        Assign = RPL->Assign;
        Print = RPL->Print;
        Read = RPL->Read;

        //copia las propiedades de seguridad
        CopyTolerance(RPL);
        //copia las propiedades de dimensionamiento:
        CopySizing(RPL);
        //copia las propiedades de área
        CopyArea(RPL);
        //copia laspropiedades gráficas
        CopyGraphics(RPL);
}

//construye un clon de una lista de posicionadores
TRoboticPositionerList1::TRoboticPositionerList1(const TRoboticPositionerList1 *RPL)
{
        //el puntero RPL debe apuntar a una lista de posicionadores de fibra construida
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
    //pointer RP shall pooint to built robotic positioner
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    int i = Search(RP);
    if(i < getCount())
        Delete(i);

    //El método Delete no debe asimilar nada, porque el RP borrado sigue existiendo.

    return i;
}

//--------------------------------------------------------------------------
//CONTRUIR POSICIONADORES:

//construye la capa n de posicionadores equidistantes una distancia D
//donde la capa 0 representa el posicionador central
void TRoboticPositionerList1::BuildLayer(int &CountId, int n, double D)
{
        //el número de capa n no debe ser negativo
        if(n < 0)
                throw EImproperArgument("numer n shall not be negative");

        if(n > 0) { //si tiene 6 vértices
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

/*                //contador de identificadores correspondientes
                //al punto para los posicionadores
                int Id = 1;
                for(int i=2; i<=n; i++)
                        Id += 6*(i - 1);
   */
                //calcula los n-1 puntos intermedios entre cada par de vértices
                for(int i=1; i<6; i++) {
                        for(int m=0; m<n; m++) {
/*                                //actualiza el identificador
                                Id++;

                                //busca un posicionador con el identificador
                                int j = SearchId(Id);
  */
                                //si no existe un posicionador con el identificador
//                                if(j >= Count) {
                                        //determina P0
                                        double alfa = m/double(n);
                                        TDoublePoint P0 = (1-alfa)*V[i-1] + alfa*V[i];
                                        //añade un nuevo posicionador
                                        Add(new TRoboticPositioner(CountId++, P0));
  //                              }
                        }
                }
                //calcula los n-1 puntos intermedios entre V[5] y V[0]
                for(int m=0; m<n; m++) {
/*                        //actualiza el identificador
                        Id++;
                        //busca un posicionador con el identificador
                        int j = SearchId(Id);
                        //si no existe un posicionador con el identificador
                        if(j >= Count) {*/
                                //determina P0
                                double alfa = m/double(n);
                                TDoublePoint P0 = (1-alfa)*V[5] + alfa*V[0];
                                //añade un nuevo posicionador
                                Add(new TRoboticPositioner(CountId++, P0));
//                        }
                }
        }
        else { //si solo hay uno en el centro
/*                //busca un posicionador con el identificador 1
                int j = SearchId(1);
                //si no existe un posicionador con el identificador
                if(j >= Count) {*/
                        //determina P0
                        TDoublePoint P0(0, 0);
                        //añade un nuevo posicionador
                        Add(new TRoboticPositioner(CountId++, P0));
//                }
        }
}

//construye una colmena circular de posicionadores
//con N capas (más el central) y destruye
//los posicionadores fuera del radio R
void TRoboticPositionerList1::BuildCircularHive(int &CountId, double D, int N, double R)
{
        //el número de identidicación CountId debe ser no negativo
        if(CountId < 0)
                throw EImproperArgument("identification number Id should be nonnegative");

        //el número de capas N debería ser no negativo
        if(N < 0)
                throw EImproperArgument(AnsiString("number layers '")+IntToStr(N)+AnsiString("' should be nonnegative"));

        //el radio de conservación R debería ser mayor que cero
        if(R <= 0)
                throw EImproperArgument(AnsiString("consrvation radio '")+FloatToStr(R)+AnsiString("' should be upper zero"));

        //construye las capas de posicionadores
        for(int i=0; i<=N; i++)
                BuildLayer(CountId, i, D);

        //destruye los posicionadores fuera del círculo
        Destroy(R);
}
//construye una colmena cuadrada de posicionadores
//con N capas
void TRoboticPositionerList1::BuildSquareHive(int &CountId, double D, int M, int N)
{
        //el número de identidicación CountId debe ser no negativo
        if(CountId < 0)
                throw EImproperArgument("identification number Id should be nonnegative");

        //el número de capas N debería ser no negativo
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
//destruye los posicionadores seleccionados de la lista
int TRoboticPositionerList1::DestroySelected(void)
{
        TRoboticPositioner *RP;
        int count = 0;

        //por cada posición de la lista
        for(int i=getCount()-1; i>=0; i--) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];
                //si el posicionador está seleccionado
                if(RP->getActuator()->Selected) {
                        //destruye el posicionador
                        Delete(i);
                        //contabiliza el posicionador seleccionado
                        count++;
                }
        }

        return count;
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
int TRoboticPositionerList1::Search(const TRoboticPositioner *RP) const
{
        //el puntero RP debe apuntar a un posicionador construido
        if(RP == NULL)
                throw EImproperArgument("pointer RP should point to built robotic positioner");

        //mientras queden posiciones en la lista y
        //el puntero no apunte al mismo posicionador
        int i = 0;
        while(i<getCount() && RP!=Items[i])
                i++; //incrementa el índice

        return i; //devuelve el índice
}
int TRoboticPositionerList1::Search(const TActuator *A) const
{
        //elpuntero A debe apuntar a un actuador construido
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
int TRoboticPositionerList1::SearchId(int Id) const
{
        //el número de identidad debe ser mayor que cero
        if(Id < 1)
                throw EImproperArgument("identity number Id should be upper zero");

        int i = 0;
        while(i<getCount() && Items[i]->getActuator()->getId()!=Id)
                        i++;

        return i;
}
//devuelve el puntero al primer posicionador
//con el identificador indicado
const TRoboticPositioner *TRoboticPositionerList1::SearchIdPointer(int Id) const
{
        //el número de identidad debe ser mayor que cero
        if(Id < 1)
                throw EImproperArgument("identifier Id should be upper zero");

        //busca la posición del posicionador Id
        int i = SearchId(Id);

        //el posicionador debe estar en la lista
        if(i >= getCount())
                throw EImproperArgument("missing Id fiber positioner");

        //devuelve el posicionador
        return Items[i];
}

//busca el primer posicionador de la lista
//en cuyo dominio de P3 se encuentra un punto
int TRoboticPositionerList1::SearchDomainP3(TDoublePoint P)
{
        int i = 0;
        while(i<getCount() && Items[i]->getActuator()->PointIsOutDomainP3(P.x, P.y))
                i++;

        return i;
}
int TRoboticPositionerList1::SearchDomainP3(double x, double y)
{
        int i = 0;
        while(i<getCount() && Items[i]->getActuator()->PointIsOutDomainP3(x, y))
                i++;

        return i;
}

//busca los posicionadores de la lista que tienen
//algún cuantificador desactivado
void TRoboticPositionerList1::SearchDisabledQuantificators(TVector<int> &indices)
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
void TRoboticPositionerList1::CalculateSPMComponents(void)
{
        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //aspunta el posicionador indicado para facilitar su acceso
                TRoboticPositioner *RP = Items[i];
                //calcula las componentes de SPM del posicionador indicado
                RP->CalculateSPMComponents();
                //calcula y asigna la componente SPMoff
                RP->SetSPMoff(getPAem(), getPem());
        }
}

//determina los posicionadores que están lo bastante cerca
//de cada posicionador como para invadir su dominio de maniobra
void TRoboticPositionerList1::DetermineAdjacents(void)
{
        TRoboticPositioner *RPi, *RPj;
        int i, j;

        //para cada posicionador de la lista
        for(i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RPi = Items[i];

                //inicializa la lista de adyacentes
                RPi->getActuator()->Adjacents.Clear();

                //para cada uno de los posicionadores previos al indicado
                for(j=0; j<i; j++) {
                        //apunta el posicionador indicado para facilitar su acceso
                        RPj = Items[j];
                        //si están lo bastante cerca como para colisionar
                        if(Mod(RPi->getActuator()->getP0() - RPj->getActuator()->getP0()) <
                                (RPi->getActuator()->getr_max() + RPi->getActuator()->getSPMall_a() +
                                 RPj->getActuator()->getr_max() + RPj->getActuator()->getSPMall_a()))
                                //añade el posicionador indicado a la lista de adyacentes
                                RPi->getActuator()->Adjacents.Add(RPj);
                }
                //para cada uno de los posicionadores posteriores al indicado
                for(j=i+1; j<getCount(); j++) {
                        //apunta el posicionador indicado para facilitar su acceso
                        RPj = Items[j];
                        //si están lo bastante cerca como para colisionar
                        if(Mod(RPi->getActuator()->getP0() - RPj->getActuator()->getP0()) <
                                (RPi->getActuator()->getr_max() + RPi->getActuator()->getSPMall_a() +
                                 RPj->getActuator()->getr_max() + RPj->getActuator()->getSPMall_a()))
                                //añade el posicionador a la lista de adyacentes
                                RPi->getActuator()->Adjacents.Add(RPj);
                }
        }
}
//ordena las listas de posicionadores adyacentes en
//sentido levógiro empezando por el más próximo a 0
void TRoboticPositionerList1::SortAdjacents(void)
{
        TRoboticPositioner *RP, *RPA;

        //ninguno de los adyacentes debe ser el posicionador de referencia
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];
                //por cada posicionador adyacente
                for(int j=0; j<RP->getActuator()->Adjacents.getCount(); j++) {
                        //apunta el posicionador indicado para facilitar su acceso
                        RPA = RP->getActuator()->Adjacents[j];
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
                TPair(TRoboticPositioner *_RP=NULL, double _theta=0) {
                        //asigna los valores de oficio
                        RP = _RP;
                        theta = _theta;
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
                for(int j=0; j<RP->getActuator()->Adjacents.getCount(); j++) {
                        //apunta el posicionador adyacente indicado para facilitar su acceso
                        RPA = RP->getActuator()->Adjacents[j];

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
                RP->getActuator()->Adjacents.Clear();
                for(int j=0; j<LP.getCount(); j++)
                        RP->getActuator()->Adjacents.Add(LP[j].P);
        }
}

//para cada posicionador de la lista calcula:
//      su radio de seguridad Rbsaf;
//      sus propiedades de la zona segura.
void TRoboticPositionerList1::CalculateSafeParameters(void)
{
        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++)
                //calcula las propiedades de seguridad
                Items[i]->getActuator()->CalculateSafeParameters();
}

//calcula las propiedades de dimensionamiento:
//      (LO3max, x3min, x3max, y3min, y3max)
void TRoboticPositionerList1::CalculateSizingParameters(void)
{
        //DETERMINA LO3Max:

        TRoboticPositioner *RP;
        double LO0;
        double LO3;
        __LO3max = 0; //inicializa la distancia máxima

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                //calcula el módulo al centro del posicionador
                LO0 = Mod(RP->getActuator()->getP0() - getO());
                //calcula la distancia máxima a P3
                LO3 = LO0 + RP->getActuator()->getL03max();
                //actualiza la distancia máxima
                if(LO3 > __LO3max)
                        __LO3max = LO3;

                //actualiza x3min
                if(RP->getActuator()->getx3min() < getx3min())
                        __x3min = RP->getActuator()->getx3min();
                //actualiza x3max
                if(RP->getActuator()->getx3max() > getx3max())
                        __x3max = RP->getActuator()->getx3max();
                //actualiza y3min
                if(RP->getActuator()->gety3min() < gety3min())
                        __y3min = RP->getActuator()->gety3min();
                //actualiza y3max
                if(RP->getActuator()->gety3max() > gety3max())
                        __y3max = RP->getActuator()->gety3max();
        }
}
//calcula todos los parámetros derivados de la geometría del sistema
//      (Spt, Set, Ret, FiberDensity)
void TRoboticPositionerList1::CalculateAreaParameters(void)
{
        //calcula el área participativa:
        __Spt = 0;
        for(int i=0; i<getCount(); i++)
                __Spt += Items[i]->getActuator()->getSpt();

        //calcula el área exclusiva:
        __Set = 0;
        for(int i=0; i<getCount(); i++)
                __Set += Items[i]->getActuator()->getSet();

        //calcula la fracción de área exclusiva:
        if(getSpt() > 0)
                __Ret = getSet()/getSpt();
        else
                __Ret = 0;

        //Ret tambien puede calcularse mediante:
        //double __Ret = 0;
        //for(int i=0; i<getCount(); i++)
        //        __Ret += RoboticPositioners[i]->Ret*RoboticPositioners[i]->Spt;

        //calcula la densidad de fibras:
        if(getSpt() > 0)
                __FiberDensity = double(getCount())/getSpt();
        else
                __FiberDensity = 0;
}

//invoca a todos los métodos de asimilación de
//los parámetros de dimensionamiento:
//      DetermineAdjacents
//      SortAdjacents
//      CalculateSafeParameters();
//      CalculateAreaParameters
void TRoboticPositionerList1::AssimilateSizing(void)
{
        DetermineAdjacents();
        SortAdjacents();                //<-----------------18/03/2014
        CalculateSafeParameters();
        CalculateSizingParameters();
        CalculateAreaParameters();
}

//asimila la configurración de posicionadores dada ejecutando:
//      CalculateSPMComponents();
//      AssimilateSizing();
void TRoboticPositionerList1::Assimilate(void)
{
        CalculateSPMComponents();
        AssimilateSizing();                //<-----------------18/03/2014
}

//MÉTODOS DE LECTURA CONJUNTA:

//get the PPA list in steps
void TRoboticPositionerList1::GetPositions(TPairPositionAnglesList& PPAL)
{
        //adjust the length of PPAL
//        PPAL.setCount(getCount());

    //initialize the PPA list
    PPAL.Clear();

    //for each RP
    for(int i=0; i<getCount(); i++) {
        //point the indicated RP to facilitate its access
        TRoboticPositioner *RP = Items[i];
        //builds a PPAattached to the RP
        TPairPositionAngles *PPA = new TPairPositionAngles(RP);
        //assign the position angles of the rotors
        PPA->p_1 = RP->getActuator()->getp_1();
        PPA->p___3 = RP->getActuator()->getArm()->getp___3();
        //add the PPA to the list
        PPAL.Add(PPA);
    }
}

//MÉTODOS DE ASIGNACIÓN CONJUNTA:

/*//asigna una lista de posiciondores
void TRoboticPositionerList1::SetRoboticPositioners(const TPointersList<TRoboticPositioner>& RPs)
{
        //por cada posicionador de la lista
        for(int i=0; i<RPs.Count; i++)
                Add(new TRoboticPositioner(RPs.GetPointer(i)));

        //asimila la lista de posicionadores
        Assimilate();
} */
//asigna las posiciones angulares de los ejes
//este método es atómico
void TRoboticPositionerList1::SetPositions(const TPairPositionAnglesList& PositionList)
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
                if(RP->getActuator()->IsntInDomainp_1(PPA->p_1))
                        throw EImproperArgument("the axis 1 angular position should be in the domain of axis 1");
                //las posiciones angulares del eje 2 debe estar en el dominio del eje 2
                if(RP->getActuator()->getArm()->IsntInDomainp___3(PPA->p___3))
                        throw EImproperArgument("the axis 2 angular position should be in the domain of axis 2");
        }

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                TRoboticPositioner *RP = Items[i];
                //apunta el par indicado para facilitar su acceso
                const TPairPositionAngles *PPA = PositionList.GetPointer(i);
                //asigna las posiciones angulares
                RP->getActuator()->SetAnglesSteps(PPA->p_1, PPA->p___3);
        }
}
//asigna conjuntamente los márgenes de segudidad
//      (PAem, Pem)
void TRoboticPositionerList1::SetTolerance(double _PAem,double _Pem)
{
        //el valor de PAem debe ser no negativo
        if(_PAem < 0)
                throw EImproperArgument("PAem value should be nonnegative");

        //el valor de Pem debe ser no negativo
        if(_Pem < 0)
                throw EImproperArgument("Pem value should be nonnegative");

        __PAem = _PAem; //asigna el nuevo valor
        __Pem = _Pem; //asigna el nuevo valor

        //asimila (PAem, Pem)
        CalculateSPMComponents();
}

//--------------------------------------------------------------------------
//MÉTODOS DE DOMINIO CONJUNTO:

//determina el intervalo cartesiano que contiene
//el dominio de todos los posicionadores
//si el número de posicionadores de la lista es menor que uno
//lanza una excepcion EImproperCall
void TRoboticPositionerList1::GetDomainP3s(double &x3min, double &x3max,
        double &y3min, double &y3max)
{
        //el número de posicionadores de la lista debería ser mayor que cero
        if(getCount() < 1)
                throw EImproperCall("number of robotic positioners in the list should be upper zero");

        TRoboticPositioner *RP;

        //inicializa los límites
        x3min = MAXDOUBLE;
        y3min = MAXDOUBLE;
        x3max = -MAXDOUBLE;
        y3max = -MAXDOUBLE;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                //actualiza los límites
                if(RP->getActuator()->getx3min() < x3min)
                        x3min = RP->getActuator()->getx3min();
                //actualiza los límites
                if(RP->getActuator()->gety3min() < y3min)
                        y3min = RP->getActuator()->gety3min();
                //actualiza los límites
                if(RP->getActuator()->getx3max() > x3max)
                        x3max = RP->getActuator()->getx3max();
                //actualiza los límites
                if(RP->getActuator()->gety3max() > y3max)
                        y3max = RP->getActuator()->gety3max();
        }
}
//determina si un punto se encuentra dentro del círculo
//que contiene el dominio conjunto de los posicionadores
bool TRoboticPositionerList1::IsInCircle(const TDoublePoint &P)
{
        if(Mod(P - getO()) > getLO3max())
                return true;
        return false;
}
//determina si un punto se encuentra dentro del cuadrado
//que contiene el dominio conjunto de los posicionadores
bool TRoboticPositionerList1::IsInSquare(const TDoublePoint &P)
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
void TRoboticPositionerList1::MoveToOrigins(void)
{
        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++)
                //mueve el posicionador al origen
                Items[i]->getActuator()->SetAnglesZeroSteps();
}
//mueve los ejes de los posicionadores seleccionados
//a sus orígenes de coordenadas
int TRoboticPositionerList1::MoveToOriginsSelected(void)
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
                        RP->getActuator()->SetAnglesZeroSteps();
                        //contabiliza el posicionador seleccionado
                        count++;
                }
        }

        return count;
}

//mueve los brazos que están fuera del área de seguridad a
//las posiciones de seguridad estables más próximas
void TRoboticPositionerList1::MoveOutsideArmsToSafePositions(void)
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
                        RP->getActuator()->MoveArmToSafePosition();
        }
}
//mueve los brazos de los posicionadores seleccionados que
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
                                RP->getActuator()->MoveArmToSafePosition();
                        //contabiliza el posicionador seleccionado
                        count++;
                }
        }

        return count;
}

//--------------------------------------------------------------------------
//METHODS TO SEGREGATE RPs:

//segrega los posicionadores en dos listas:
//      lista con el brazo dentro del área de seguridad;
//      lista con el brazo fuera del área de seguridad;
void TRoboticPositionerList1::SegregateInOut(TRoboticPositionerList1 &Inners,
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
void TRoboticPositionerList1::segregateCollided(TRoboticPositionerList1& Collided) const
{
    Collided.Clear();
    for(int i=0; i<getCount(); i++) {
        TRoboticPositioner *RP = Items[i];
        if(RP->getActuator()->ThereIsCollisionWithAdjacent())
            Collided.Add(RP);
    }
}

//segrega losposicionadores seleccionados en una lista
void TRoboticPositionerList1::SegregateSelected(TRoboticPositionerList1& RPL) const
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

//MÉTODOS DE APILADO DE POSICIONES ANGULARES:

//apila las posiciones de cada posicionador de la lista
void TRoboticPositionerList1::PushPositions(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];
                //apila la posición del cilindro
                RP->getActuator()->Pushtheta_1();
                //apila la posición del brazo
                RP->getActuator()->getArm()->Pushtheta___3();
        }
}
//restaura la siguiente posición apilada de cada posicionador
//sin desempilarla
void TRoboticPositionerList1::RestorePositions(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                //restaura la posición del cilindro
                RP->getActuator()->Restoretheta_1();
                //restaura la posición del brazo
                RP->getActuator()->getArm()->Restoretheta___3();
        }
}
//desempila la siguiente posición apilada de cada posicionador
void TRoboticPositionerList1::PopPositions(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];
                //desempila la posición del cilindro
                RP->getActuator()->Poptheta_1();
                //desempila la posición del brazo
                RP->getActuator()->getArm()->Poptheta___3();
        }
}
//restaura y desempila la siguiente posición apilada de cad aposicionador
void TRoboticPositionerList1::RestoreAndPopPositions(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                //restaura la posición del cilindro
                RP->getActuator()->Restoretheta_1();
                //restaura la posición del brazo
                RP->getActuator()->getArm()->Restoretheta___3();

                //desempila la posición del cilindro
                RP->getActuator()->Poptheta_1();
                //desempila la posición del brazo
                RP->getActuator()->getArm()->Poptheta___3();
        }
}

//apila la posición de los brazos de todos los posicionadores
void TRoboticPositionerList1::PushArmtheta___3s()
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];
                //apila la posición del brazo
                RP->getActuator()->getArm()->Pushtheta___3();
        }
}
//restaura la últimaposición apilada del brazo de cada posicionador
void TRoboticPositionerList1::RestoreArmtheta___3s(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                try {
                        //restaura la posición del brazo
                        RP->getActuator()->getArm()->Restoretheta___3();
                } catch(...) {
                        throw;
                }
        }
}
//restaura la posición almacenada del brazo de cada posicionador
void TRoboticPositionerList1::PopArmtheta___3s(void)
{
        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++)
                //restaura la posición apilada del brazo
                Items[i]->getActuator()->getArm()->Poptheta___3();
}

//MÉTODOS DE PILA DE CUANTIFICADORES:

//apila el estado de
//los cuantificadores de cada posicionador de la lista
void TRoboticPositionerList1::PushQuantifys(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                //apila y activa el estado de cuantificación del cilindro
                RP->getActuator()->PushQuantify_();
                //apila y desactiva el estado de cuantificación del brazo
                RP->getActuator()->getArm()->PushQuantify___();
        }
}
//restaura el último estado apilado de
//los cuantificadores de cada posicionador de la lista
void TRoboticPositionerList1::RestoreQuantifys(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                //restaura el estado del cuantificador del eje 1
                RP->getActuator()->RestoreQuantify_();
                //restaura el estado del cuantificador del eje 2
                RP->getActuator()->getArm()->RestoreQuantify___();
        }
}
//desempila el último estado apilado de
//los cuantificadores de cada posicionador de la lista
void TRoboticPositionerList1::PopQuantifys(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                //desempila el estado del cuantificador del eje 1
                RP->getActuator()->PopQuantify_();
                //desempila el estado del cuantificador del eje 2
                RP->getActuator()->getArm()->PopQuantify___();
        }
}
//restaura y desempila el último estado apilado de
//los cuantificadores de cada posicionador de la lista
void TRoboticPositionerList1::RestoreAndPopQuantifys(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                //restaura y desempila el estado del cuantificador del eje 1
                RP->getActuator()->RestoreQuantify_();
                RP->getActuator()->PopQuantify_();

                //restaura y desempila el estado del cuantificador del eje 2
                RP->getActuator()->getArm()->RestoreQuantify___();
                RP->getActuator()->getArm()->PopQuantify___();
        }
}

//apila el estado
//del cuantificador del brazo de cada posicionador de la lista
void TRoboticPositionerList1::PushArmQuantify___s(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                //apila y desactiva el estado de cuantificación del brazo
                RP->getActuator()->getArm()->PushQuantify___();
        }
}
//restaura el último estado apilado
//del cuantificador del brazo de cada posicionador de la lista
void TRoboticPositionerList1::RestoreArmQuantify___s(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                //restaura el estado del cuantificador del eje 2
                RP->getActuator()->getArm()->RestoreQuantify___();
        }
}
//desempila el último estado apilado
//del cuantificador del brazo de cada posicionador de la lista
void TRoboticPositionerList1::PopArmQuantify___s(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                //desempila el estado del cuantificador del eje 2
                RP->getActuator()->getArm()->PopQuantify___();
        }
}
//restaura y desempila el último estado apilado
//del cuantificador del brazo de cada posicionador de la lista
void TRoboticPositionerList1::RestoreAndPopArmQuantify___s(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                //restaura y desempila el estado del cuantificador del eje 2
                RP->getActuator()->getArm()->RestoreQuantify___();
                RP->getActuator()->getArm()->PopQuantify___();
        }
}

//--------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR SI HAY COLISIONES:

//levanta las banderas indicadoras de determinación de colisión
//pendiente de todos los posicionadores d ela lista
void TRoboticPositionerList1::EnablePending(void)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->getActuator()->Pending = true;
}

//determina si algún brazo de algún posicionador
//colisiona con el brazo de algún posicionador adyacente
bool TRoboticPositionerList1::ThereIsCollision(void)
{
        TRoboticPositioner *RP;

        //levanta las banderas de determinación de colisión pendiente
        EnablePending();

        //busca colisión del brazo de cada posicionador con el brazo de
        //sus adyacentes exceptuando aquellos con los que se ha comprobado ya
        for(int i= 0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];
                //si el brazo del posicionador indicado colisiona con
                //el brazo de algún posicionador adyacente con
                //determinación de colisión pendiente
                if(RP->getActuator()->ThereIsCollisionWithPendingAdjacent())
                        //indica que hay colisión
                        return true;
                else
                        //desactiva la bandera para evitar que
                        //se vuelva a determinar la colisión
                        RP->getActuator()->Pending = false;
        }

        return false; //indica que no se ha encontrado invasión
}
//busca los posicionadores de la lista cuyo brazo colisiona con
//el brazo de algún otro posicionador adyacente
void TRoboticPositionerList1::SearchCollinding(TVector<int> &indices)
{
        //ADVERTENCIA: los posicionadores de la lista no tienen
        //por que estar ordenados según su identificador.

        TRoboticPositioner *RP;
        TItemsList<TRoboticPositioner*> Collindings;

        //limpia la lista de índices en congruencia con sus estado inicial
        indices.Clear();

        //levanta las baderas de colisión de todos los posicionadores
        EnablePending();

        //por cada posicionador de la lista
        for(int i= 0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];
                //busca colisiones con los posicionadores adyacentes con los
                //que todavía no hayan determinado su estado de colisión
                RP->getActuator()->SearchCollindingPendingAdjacent(Collindings);
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
                                m = Search(RP);

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
}
//obtiene los conjuntos de posicionadores en colisión en la exposición indicada
void TRoboticPositionerList1::GetCollisionClusterList(TPointersList<TItemsList<TRoboticPositioner*> > &CCL)
{
        TRoboticPositioner *RP;
        TItemsList<TRoboticPositioner*> Collindings;
        TItemsList<TRoboticPositioner*> *CC;

        //inicializa la lista de conjuntos de posicionadores en colisión
        CCL.Clear();

        //levanta las banderas indicadoras de determinación de colisión
        //pendiente de todos los posicionadores de la lista
        EnablePending();

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];

                //busca los posicionadores adyacentes cuyo brazo colisiona con el del posicionador central
                RP->getActuator()->SearchCollindingPendingAdjacent(Collindings);

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
}

//RANDOMIZADO DE LOS EJES:

//lleva los ejes de los posicionadores a posiciones aleatorias
//con distribución uniforme en sus dominios
void TRoboticPositionerList1::Randomize(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];
                //randomiza las posiciones de sus ejes
                RP->getActuator()->Randomizep_1();
                RP->getActuator()->getArm()->Randomizep___3();
        }
}
//lleva los ejes de los posicionadores seleccionados a
//posiciones aleatorias con distribución uniforme en sus dominios
int TRoboticPositionerList1::RandomizeSelected(void)
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
                        RP->getActuator()->Randomizep_1();
                        RP->getActuator()->getArm()->Randomizep___3();
                        //contabiliza el posicionador seleccionado
                        count++;
                }
        }

        return count;
}

//lleva los ejes de los posicionadores a posiciones aleatorias
//con distribución uniforme en sus dominios
//en las que no colisionan entre si
void TRoboticPositionerList1::RandomizeWithoutCollision(void)
{
        TRoboticPositioner *RP;

        //para cada posicionador de la lista
        for(int i=0; i<getCount(); i++)
                do {
                        //apunta el posicionador indicado para facilitar su acceso
                        RP = Items[i];
                        //lleva randomiza los ejes del posicionador
                        RP->getActuator()->Randomizep_1();
                        RP->getActuator()->getArm()->Randomizep___3();
                        //mientras colisione con algún adyacente
                } while(RP->getActuator()->ThereIsCollisionWithPendingAdjacent());
}
//lleva los ejes de los posicionadores seleccionados a
//posiciones aleatorias con distribución uniforme en
//sus dominios en las que no colisionan entre si
int TRoboticPositionerList1::RandomizeWithoutCollisionSelected(void)
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
                                RP->getActuator()->Randomizep_1();
                                RP->getActuator()->getArm()->Randomizep___3();
                                //determina si hay colisión
                                collision = RP->getActuator()->ThereIsCollisionWithAdjacent();
                                //si no hay colisión
                                if(!collision)
                                        //contabiliza el posicionador seleccionado
                                        count++;
                        }
                //mientras colisione con algún adyacente
                } while(collision);
        }

        return count;
}

//RANDOMIZADO DE P3:

//lleva el punto P3 de los posicionadores a posiciones aleatorias
//con distribución uniforme en su dominio
void TRoboticPositionerList1::RandomizeP3(void)
{
        TRoboticPositioner *RP;

        //por cada posicionador de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i];
                //randomiza el punto P3
                RP->getActuator()->RandomizeP3();
        }
}
//lleva el punto P3 de los posicionadores seleccionados a
//posiciones aleatorias con distribución uniforme en su dominio
int TRoboticPositionerList1::RandomizeP3Selected(void)
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
                        RP->getActuator()->RandomizeP3();
                        //contabiliza el posicionador seleccionado
                        count++;
                }
        }

        return count;
}

//lleva el punto P3 de los posicionadores a posiciones aleatorias
//con distribución uniforme en su dominio
//en las que no colisionan entre si
void TRoboticPositionerList1::RandomizeP3WithoutCollision(void)
{
        TRoboticPositioner *RP;

        //para cada posicionador de la lista de osicionadores
        for(int i=0; i<getCount(); i++)
                do {
                        //apunta el posicionador indicado para facilitar su acceso
                        RP = Items[i];
                        //randomiza el punto P3
                        RP->getActuator()->RandomizeP3();
                        //mientras colisione con algún adyacente
                } while(RP->getActuator()->ThereIsCollisionWithPendingAdjacent());
}
//lleva el punto P3 de los posicionadores seleccioandos a
//posiciones aleatorias con distribución uniforme en su dominio
//en las que no colisionan entre si
int TRoboticPositionerList1::RandomizeP3WithoutCollisionSelected(void)
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
                                RP->getActuator()->RandomizeP3();
                                //determina si hay colisión
                                collision = RP->getActuator()->ThereIsCollisionWithPendingAdjacent();
                                //si no hay colisión
                                if(!collision)
                                        //contabiliza el posicionador seleccionado
                                        count++;
                        }
                //mientras colisione con algún adyacente
                } while(collision);
        }

        return count;
}

//-------------------------------------------------------------------
//MÉTODOS GRÁFICOS:

//establece el estado de la bandera de pintado
//del cuerpo de los posicionadores
void TRoboticPositionerList1::SetPaintBody(bool b)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->getActuator()->PaintBody = b;
}
//establece el estado de la bandera de pintado
//del límite del dominio del punto P3 de los posicionadores
void TRoboticPositionerList1::SetPaintLimitDomainP3(bool b)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->getActuator()->PaintLimitDomainP3 = b;
}
//establece el estado de la bandera de pintado
//del límite del dominio de maniobra de los posicionadores
void TRoboticPositionerList1::SetPaintLimitDomainManeuvering(bool b)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->getActuator()->PaintLimitDomainManeuvering = b;
}

/*#//asigna un color a todos los posicionadores de la lista
void TRoboticPositionerList1::SetAllColors(QColor Color)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->getActuator()->SetAllColors(Color);
}*/
//selecciona todos los posicionadores de la lista
void TRoboticPositionerList1::SelectAll(void)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->getActuator()->Selected = true;
}
//deselecciona todos los posicionadores de la lista
void TRoboticPositionerList1::DeselectAll(void)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->getActuator()->Selected = false;
}
/*#
//imprime la lista de posicionadores en el lienzo de la fotografía
//con los colores indicados
//imprime el lienzo de la fotografía en el lienzo de la caja de pintura
void TRoboticPositionerList1::PaintActuators(TPloterShapes *PS)
{
        //el puntero PS debería apuntar a un trazador de formas construido
        if(PS == NULL)
                throw EImproperArgument("pointer PS should point to built ploter shapes");

        //imprime los posicionadores de la lista
        for(int i=0; i<getCount(); i++)
                Items[i]->getActuator()->Paint(PS);
}
//imprime la lista de posicionadores en el lienzo de la fotografía
//con los colores indicados en el modelo simplificado
//imprime el lienzo de la fotografía en el lienzo de la caja de pintura
void TRoboticPositionerList1::PaintActuatorsSimplified(TPloterShapes *PS)
{
        //el puntero PS debería apuntar a un trazador de formas construido
        if(PS == NULL)
                throw EImproperArgument("pointer PS should point to built ploter shapes");

        //imprime los posicionadores de la lista
        for(int i=0; i<getCount(); i++)
                Items[i]->getActuator()->PaintSimplified(PS);
}
//dibuja el límite circular del dominio conjunto de los posicionadores
void TRoboticPositionerList1::PaintLimitDomain(TPloterShapes *PS)
{
        //configura el color de la pluma
        PS->setPenColor(LimitDomainColor);
        PS->Circunference(getO(), getLO3max());

}

//busca el primer posicionador que está bajo el punto P
//y que parte del posicionador puede ser agarrado en
//el punto indicado:
//      i: índice al posicionador bajo el punto P;
//      dominio i: [0, Count];
//      n: parte del posicionador agarrada;
//      valores posibles:
//      0: niguna;
//      1: cilindro (P0, L01);
//      2: brazo (PA..PH);
//      3: lente (P2, R2)
//valores de retorno:
//      false: ninguna parte agarrada
//      true: alguna parte agarrada
bool TRoboticPositionerList1::Grab(int &i, int &n, TDoublePoint P)
{
        for(i=0; i<getCount(); i++) {
                n = Items[i]->getActuator()->Grab(P);
                if(n > 0)
                        return true;
        }

        return false;
}
*/
//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
