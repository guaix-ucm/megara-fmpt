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
//File: FiberConnectionModel.cpp
//Content: fiber connection model
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "FiberConnectionModel.h"
#include "TextFile.h"

//---------------------------------------------------------------------------

using namespace Strings;

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//clase conexión de la pseudoslit

//etiquetas
AnsiString TConection::RPIdLabel = "RPId";
AnsiString TConection::SpaxelIdLabel = "SpaxelId";
AnsiString TConection::FiberIdLabel = "FiberId";
AnsiString TConection::sLabel = "s";

void TConection::setRPId(int RPId)
{
    //RPId debe ser no negativo
    if(RPId < 0)
        throw EImproperArgument("RPId should be nonnegative");

    p_RPId = RPId;
}
void TConection::setSpaxelId(int SpaxelId)
{
    //SpaxelId debe estar en [0, 7]
    if(SpaxelId<0 || 7<SpaxelId)
        throw EImproperArgument("SpaxelId should be in [0, 7]");

    p_SpaxelId = SpaxelId;
}
void TConection::setFiberId(int FiberId)
{
    //FiberId debe ser no negativo
    if(FiberId < 0)
        throw EImproperArgument("FiberId should be nonnegative");

    p_FiberId = FiberId;
}
void TConection::sets(double s)
{
    p_s = s;
}

AnsiString TConection::getRPIdText(void) const
{
    return IntToStr(getRPId());
}
void TConection::setRPIdText(const AnsiString& S)
{
    try {
        setRPId(StrToInt(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting RPId in text format: ");
        throw;
    }
}
AnsiString TConection::getSpaxelIdText(void) const
{
    return IntToStr(getSpaxelId());
}
void TConection::setSpaxelIdText(const AnsiString& S)
{
    try {
        setSpaxelId(StrToInt(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting SpaxelId in text format: ");
        throw;
    }
}
AnsiString TConection::getFiberIdText(void) const
{
    return IntToStr(getFiberId());
}
void TConection::setFiberIdText(const AnsiString& S)
{
    try {
        setFiberId(StrToInt(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting FiberId in text format: ");
        throw;
    }
}
AnsiString TConection::getsText(void) const
{
    return FloatToStr(gets());
}
void TConection::setsText(const AnsiString& S)
{
    try {
        sets(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting s in text format: ");
        throw;
    }
}

//lee los valores de las propiedades en una fila de texto
void  TConection::ReadRow(TConection *Item,
                          const AnsiString& S, int& i)
{
    //comprueba las precondiciones
    if(Item == NULL)
        throw EImproperArgument("pointer Item should point to built item");
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    try {
        //lee el valor para RPId
        int RPId;
        StrReadInt(RPId, S, i);

        //lee el valor para SpaxelId
        int SpaxelId;
        StrTravelSeparators(S, i);
        StrReadInt(SpaxelId, S, i);

        //lee el valor para FiberId
        int FiberId;
        StrTravelSeparators(S, i);
        StrReadInt(FiberId, S, i);

        //lee el valor para s
        double s;
        StrTravelSeparators(S, i);
        StrReadFloat(s, S, i);

        //asigna las variables tampón
        Item->Set(RPId, SpaxelId, FiberId, s);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading connection values in row text format: ");
        throw;
    }
}
//imprime los valores de las propiedades en una fila de texto
void  TConection::PrintRow(AnsiString& S, const TConection *C)
{
    //coprueba la precondición
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built connection");

    S += C->getRPIdText()+AnsiString("\t")+C->getSpaxelIdText()+AnsiString("\t")+C->getFiberIdText()+AnsiString("\t")+C->getsText();
}
//obtiene las etiqeutas de las propiedades en formato texto
AnsiString TConection::GetLabelsRow(void)
{
    return RPIdLabel+AnsiString("\t")+SpaxelIdLabel+AnsiString("\t")+FiberIdLabel+AnsiString("\t")+sLabel;
}
//atraviesa las etiquetas de las propiedades
void TConection::TravelLabelsRow(const AnsiString& S, int& i)
{
    //comprueba las precondiciones
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    try {
        StrTravelLabel(RPIdLabel, S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel(SpaxelIdLabel, S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel(FiberIdLabel, S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel(sLabel, S, i);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "traqveling connection labels: ");
        throw;
    }
}


    //copy all properties of a connection
TConection& TConection::operator=(const TConection& C)
{
    p_RPId = C.p_RPId;
    p_SpaxelId = C.p_SpaxelId;
    p_FiberId = C.p_FiberId;
    p_s = C.p_s;

    return *this;
}

//build a clon of a connection
TConection::TConection(const TConection *C)
{
    //check the precondition
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built connection");

    p_RPId = C->p_RPId;
    p_SpaxelId = C->p_SpaxelId;
    p_FiberId = C->p_FiberId;
    p_s = C->p_s;
}

//asigna las propiedades atómicamente
//si no es posible asignar todas las propiedades:
//      lanza una excepción EImproperArgument
void TConection::Set(int RPId, int SpaxelId, int FiberId, double s)
{
    //RPId debe ser no negativo
    if(RPId < 0)
        throw EImproperArgument("RPId should be nonnegative");
    //SpaxelId debe estar en [0, 7]
    if(SpaxelId<0 || 7<SpaxelId)
        throw EImproperArgument("SpaxelId should be in [0, 7]");
    //FiberId debe ser no negativo
    if(FiberId < 0)
        throw EImproperArgument("FiberId should be nonnegative");

    p_RPId = RPId;
    p_SpaxelId = SpaxelId;
    p_FiberId = FiberId;
    p_s = s;
}

//---------------------------------------------------------------------------
//clase modelo del espectrografo

void TFiberConnectionModel::setR(double R)
{
    //el radio R debe ser mayor que cero
    if(R <= 0)
        throw EImproperArgument("radious R shoukd be upper zero");

    p_R = R;

    //El valor de R afectará a los métodos GetSpaxelCentre#.
}

AnsiString TFiberConnectionModel::getConnectionsText(void) const
{
    return TConection::GetLabelsRow()+AnsiString("\r\n")+Conections.getColumnText();
}
void TFiberConnectionModel::setConnectionsText(const AnsiString& S)
{
    //indica a la primera posición de la cadena
    int i = 1;

    try {
        //atraviesa la cabecera de la tabla
        TConection::TravelLabelsRow(S, i);

        //lee las conexiones en una variable tampón
        TPointersList<TConection> t_Conections;
        t_Conections.Read = TConection::ReadRow;
        TPointersList<TConection>::ReadSeparated(&t_Conections, S, i);

        //busca texto inesperado
        StrTravelSeparators(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text: "+StrFirstChars(S.SubString(i, S.Length() - i + 1)).str);

        //asigna la varibale tampón
        Conections.Clear();
        Conections.Add(Conections);
        t_Conections.ClearWithoutDestroy();
    }
    catch(Exception& E) {
        unsigned int row, col;
        strPositionToCoordinates(row, col, S.str, i-1);
        E.Message.Insert(1, "setting connections in row "+inttostr(row)+" and column "+inttostr(col)+": ");
        throw;
    }
}

//construye un Fiber Connection Model con los valores por defecto
TFiberConnectionModel::TFiberConnectionModel(void) : p_R(0.443),
    Conections(644, NULL, NULL, NULL, TConection::PrintRow, TConection::ReadRow)
{
}

//clone a Fiber Connection Model
void TFiberConnectionModel::Clone(const TFiberConnectionModel& FCM)
{
    p_R = FCM.p_R;
    Conections.Clone(FCM.Conections);
}

//determina las coordenadas en S0 del centro del spaxel 1
void TFiberConnectionModel::GetSpaxelCenter1(double&x, double& y,
                                             double x3, double y3, double theta3)
{
    //contruye el vector V orientado en la dirección 0
    TDoublePoint V(getR(), 0);

    //rota el punto V (-PI/6 + theta3) rad
    double a = -M_PI/6 + theta3;
    x = V.x*cos(a) - V.y*sin(a);
    y = V.x*sin(a) + V.y*cos(a);

    //desplaza (x, y) a (x3, y3)
    x += x3;
    y += y3;
}
//determina las coordenadas en S0 del centro del spaxel 2
void TFiberConnectionModel::GetSpaxelCenter2(double&x, double& y,
                                             double x3, double y3, double theta3)
{
    //contruye el vector V orientado en la dirección 0
    TDoublePoint V(getR(), 0);

    //rota el punto V (PI/6 + theta3) rad
    double a = M_PI/6 + theta3;
    x = V.x*cos(a) - V.y*sin(a);
    y = V.x*sin(a) + V.y*cos(a);

    //desplaza (x, y) a (x3, y3)
    x += x3;
    y += y3;
}
//determina las coordenadas en S0 del centro del spaxel 3
void TFiberConnectionModel::GetSpaxelCenter3(double&x, double& y,
                                             double x3, double y3, double theta3)
{
    //contruye el vector V orientado en la dirección 0
    TDoublePoint V(getR(), 0);

    //rota el punto V (-PI/2 + theta3) rad
    double a = -M_PI/2 + theta3;
    x = V.x*cos(a) - V.y*sin(a);
    y = V.x*sin(a) + V.y*cos(a);

    //desplaza (x, y) a (x3, y3)
    x += x3;
    y += y3;
}
//determina las coordenadas en S0 del centro del spaxel 4
void TFiberConnectionModel::GetSpaxelCenter4(double&x, double& y,
                                             double x3, double y3)
{
    //desplaza (x, y) a (x3, y3)
    x = x3;
    y = y3;
}
//determina las coordenadas en S0 del centro del spaxel 5
void TFiberConnectionModel::GetSpaxelCenter5(double&x, double& y,
                                             double x3, double y3, double theta3)
{
    //contruye el vector V orientado en la dirección 0
    TDoublePoint V(getR(), 0);

    //rota el punto V (PI/2 + theta3) rad
    double a = M_PI/2 + theta3;
    x = V.x*cos(a) - V.y*sin(a);
    y = V.x*sin(a) + V.y*cos(a);

    //desplaza (x, y) a (x3, y3)
    x += x3;
    y += y3;
}
//determina las coordenadas en S0 del centro del spaxel 6
void TFiberConnectionModel::GetSpaxelCenter6(double&x, double& y,
                                             double x3, double y3, double theta3)
{
    //contruye el vector V orientado en la dirección 0
    TDoublePoint V(getR(), 0);

    //rota el punto V (PI*5/6 + theta3) rad
    double a = M_PI*5./6 + theta3;
    x = V.x*cos(a) - V.y*sin(a);
    y = V.x*sin(a) + V.y*cos(a);

    //desplaza (x, y) a (x3, y3)
    x += x3;
    y += y3;
}
//determina las coordenadas en S0 del centro del spaxel 7
void TFiberConnectionModel::GetSpaxelCenter7(double&x, double& y,
                                             double x3, double y3, double theta3)
{
    //contruye el vector V orientado en la dirección 0
    TDoublePoint V(getR(), 0);

    //rota el punto V (-PI*5/6 + theta3) rad
    double a = -M_PI*5./6 + theta3;
    x = V.x*cos(a) - V.y*sin(a);
    y = V.x*sin(a) + V.y*cos(a);

    //desplaza (x, y) a (x3, y3)
    x += x3;
    y += y3;
}

//Determina (FiberId, s) a partir de (RPId, SpaxelId).
//Si no encuentra (RPId, SpaxelId) en la tabla de posiciones
//relativas de la pseudoslit: devuelve falso.
bool TFiberConnectionModel::Find(int& FiberId, double& s, int RPId, int SpaxelId)
{
    //RPId debe ser no negativo
    if(RPId < 0)
        throw EImproperArgument("RPId should be nonnegative");
    //SpaxelId debe estar en [0, 7]
    if(SpaxelId<0 || 7<SpaxelId)
        throw EImproperArgument("SpaxelId should be in [0, 7]");

    //indica a la primera posición de la lista
    int i = 0;
    //mientras i indque a una conexión, y el par (RId, SpaxelID) no coincida con el de la conexión...
    while(i<Conections.getCount() && (RPId!=Conections[i].getRPId() || SpaxelId!=Conections[i].getSpaxelId())) {
        i++;
    }

    //si no ha encontrado una conexión devuelve falso
    if(i >= Conections.getCount())
        return false;

    //asigna el (FiberId, s) de la conexión encontrada
    FiberId = Conections[i].getFiberId();
    s = Conections[i].gets();
    //indica que ha encontrado la conexión
    return true;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

