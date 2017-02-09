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
//File: TelescopeProjectionModel.cpp
//Content: telescope projection model
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "TelescopeProjectionModel.h"
#include "FiberMOSModelConstants.h" //GTC_R
#include "Geometry.h"
#include "Scalars.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TProjectionCircle
//---------------------------------------------------------------------------

//MÉTODOS PÚBLICOS:

void TProjectionCircle::setR(double R)
{
    //comprueba la precondición
    if(R < 0)
        throw EImproperArgument("radious R should be nonnegative");

    p_R = R; //set the value
}

//contruye un círculo de proyección
TProjectionCircle::TProjectionCircle(void)
{
    //inicializa las propiedades
    O.x = 0;
    O.y = 0;
    p_R = 0;
}
TProjectionCircle::TProjectionCircle(TDoublePoint _O, double R)
{
    O = _O;
    try {
        setR(R);
    } catch(Exception& E) {
        E.Message.Insert(1, "building a projection circle: ");
        throw;
    }
}
//construye un clon de un círculo de proyección
TProjectionCircle::TProjectionCircle(TProjectionCircle *PC)
{
    //comprueba la precondición
    if(PC == NULL)
        throw EImproperArgument("pointer PC should point to built projection circle");

    //copia las propiedades
    O = PC->O;
    p_R = PC->p_R;
}

//determina si un punto está en el dominio del círculo de proyección
bool TProjectionCircle::PointIsInDomain(const TDoublePoint Q)
{
    if(Mod(Q - O) <= getR())
        return true;

    return false;
}
//determina si un punto no está en el dominio del círculo de proyección
bool TProjectionCircle::PointIsntInDomain(const TDoublePoint Q)
{
    //mayor que el radio del círculo
    if(Mod(Q - O) > getR())
        return true;

    return false;
}

TDoublePoint TProjectionCircle::RandomPoint(void)
{
    TDoublePoint Q;

    //determina el intervalo cartesiano que contiene todo el círculo
    double xmin = O.x - getR();
    double xmax = O.x + getR();
    double ymin = O.y - getR();
    double ymax = O.y + getR();

    do {
        //randomiza el punto en el intervalo cartesiano
        Q.x = RandomUniform(xmin, xmax);
        Q.y = RandomUniform(ymin, ymax);
        //mientras no esté dentro del dominio
    } while(PointIsntInDomain(Q));

    return Q;
}

//---------------------------------------------------------------------------
//TFocalPlane
//---------------------------------------------------------------------------

//PROPIEDADES:

void TFocalPlane::setR(double R)
{
    //comprueba las precondiciones
    if(R <= 0)
        throw EImproperArgument("radious R should be upper zero");

    p_R = R; //asigna el valor
}

//MÉTODOS PÚBLICOS:

//contruye un plano focal
//con 331 ángulos de rotación en [-pi/6, pi/6]
//con 331 puntos de traslación equidistantes L/5 mm
//distribuidos en 19 capas exagonales más el central
TFocalPlane::TFocalPlane(void) :
    p_R(GTC_R),
    p_ProjectionCircle(NULL),
    ProjectionPointList(),
    InnerProjectionPointList()
{
    //inicializa el punto origen de coordenadas
    p_O.x = 0;
    p_O.y = 0;
}

//copia las propiedades de un plano focal
void TFocalPlane::CopyFocalPlane(TFocalPlane *FP)
{
    //comprueba las precondiciones
    if(FP == NULL)
        throw EImproperArgument("pointer FP should point to built focal plane");

    //copy simple properties
    p_O = FP->p_O;
    p_R = FP->p_R;

    //clone the projection circle, if any
    if(FP->getProjectionCircle() != NULL)
        p_ProjectionCircle = new TProjectionCircle(FP->getProjectionCircle());
    else
        p_ProjectionCircle = FP->getProjectionCircle();

    //copy the projection point list
    ProjectionPointList = FP->ProjectionPointList;

    //Remember that:
    // class TProjectionPointList : public TPointersList<TProjectionPoint>

    //get the corresponding ListInnerProjectionPoint
    InnerProjectionPointList.Clear();
    for(int i=0; i<FP->InnerProjectionPointList.getCount(); i++) {
        TProjectionPoint *PP = FP->InnerProjectionPointList[i];
        int j = FP->ProjectionPointList.SearchPointer(PP);
        if(j < FP->ProjectionPointList.getCount())
            InnerProjectionPointList.Add(ProjectionPointList.GetPointer(j));
        else
            InnerProjectionPointList.Add(PP);
    }
}

//determina si un punto se encuentra dentro del plano focal
bool TFocalPlane::PointIsInFocalPlane(const TDoublePoint P)
{
    if(Mod(P - getO()) <= getR())
        return true;
    return false;
}

//borra los puntos de proyección y el círculo de proyección
void TFocalPlane::Clear(void)
{
    ProjectionPointList.Clear();
    InnerProjectionPointList.Clear();
    if(p_ProjectionCircle != NULL) {
        delete p_ProjectionCircle;
        p_ProjectionCircle = NULL;
    }
}

//---------------------------------------------------------------------------
//TTelescopeProjectionModel
//---------------------------------------------------------------------------

//INICIALIZA LAS ETIQUETAS DE LAS PROPIEDADES:

AnsiString TTelescopeProjectionModel::RALabel = "RA";
AnsiString TTelescopeProjectionModel::DECLabel = "DEC";
AnsiString TTelescopeProjectionModel::PALabel = "RA";
AnsiString TTelescopeProjectionModel::RLabel = "R";
AnsiString TTelescopeProjectionModel::angviewLabel = "angview";
AnsiString TTelescopeProjectionModel::scaleLabel = "scale";
AnsiString TTelescopeProjectionModel::scaleinvLabel = "scaleinv";

//PROPIDADES DEFINITORIAS:

void TTelescopeProjectionModel::setRA(double RA)
{
    //asigna el nuevo valor
    p_RA = RA;
    //asimila el nuevo valor
    DeterminePlanes();
}
void TTelescopeProjectionModel::setDEC(double DEC)
{
    //la declinación DEC debería estar en [-pi/2, pi/2]
    if(DEC<-M_PI/2 || M_PI/2<DEC)
        throw EImproperArgument(AnsiString("declination '")+FloatToStr(DEC)+AnsiString("' should be in [-pi/2, pi/2]"));

    //asigna el nuevo valor
    p_DEC = DEC;
    //asimila el nuevo valor
    DeterminePlanes();
}
void TTelescopeProjectionModel::setPA(double PA)
{
    //asigna el nuevo valor
    p_PA = PA;
    //asimila el nuevo valor
    DeterminePlanes();
}

void TTelescopeProjectionModel::setR(double R)
{
    //el radio del plano focal debe ser mayor que cero
    if(R == 0)
        throw EImproperArgument("focal plane radio R should be upper zero");

    //asigna el nuevo valor
    p_R = R;
    //asimila el nuevo valor
    DeterminePlanes();
}
void TTelescopeProjectionModel::setangview(double angview)
{
    //el ángulo de visión debe ser mayor que cero
    if(angview == 0)
        throw EImproperArgument("the angle of view angview should be upper zero");

    //asigna el nuevo valor
    p_angview = angview;
    //asimila el nuevo valor
    DeterminePlanes();
}

//PROPIEDADES DEFINITORIAS EN FORMATO TEXTO:

AnsiString TTelescopeProjectionModel::getRAText(void) const
{
    return FloatToStr(getRA());
}
void TTelescopeProjectionModel::setRAText(const AnsiString &S)
{
    try {
        setRA(StrToFloat(S));
    } catch(...) {
        throw;
    }
}
AnsiString TTelescopeProjectionModel::getDECText(void) const
{
    return FloatToStr(getDEC());
}
void TTelescopeProjectionModel::setDECText(const AnsiString &S)
{
    try {
        setDEC(StrToFloat(S));
    } catch(...) {
        throw;
    }
}
AnsiString TTelescopeProjectionModel::getPAText(void) const
{
    return FloatToStr(getPA());
}
void TTelescopeProjectionModel::setPAText(const AnsiString &S)
{
    try {
        setPA(StrToFloat(S));
    } catch(...) {
        throw;
    }
}

AnsiString TTelescopeProjectionModel::getRText(void) const
{
    return FloatToStr(getR());
}
void TTelescopeProjectionModel::setRText(const AnsiString &S)
{
    try {
        setR(StrToFloat(S));
    } catch(...) {
        throw;
    }
}
AnsiString TTelescopeProjectionModel::getangviewText(void) const
{
    return FloatToStr(getangview());
}
void TTelescopeProjectionModel::setangviewText(const AnsiString &S)
{
    try {
        setangview(StrToFloat(S));
    } catch(...) {
        throw;
    }
}

AnsiString TTelescopeProjectionModel::getangview_radText(void) const
{
    return FloatToStr(getangview_rad());
}
AnsiString TTelescopeProjectionModel::getscaleText(void) const
{
    return FloatToStr(getscale());
}
AnsiString TTelescopeProjectionModel::getscaleinvText(void) const
{
    return FloatToStr(getscaleinv());
}

//CONJUNTO DE PROPIEDADES DEFINITORIAS EN FORMATO TEXTO:

AnsiString TTelescopeProjectionModel::getAssignsText(void) const
{
    AnsiString S;

    S += RALabel+AnsiString(" = ")+getRAText()+AnsiString("\r\n");
    S += DECLabel+AnsiString(" = ")+getDECText()+AnsiString("\r\n");
    S += PALabel+AnsiString(" = ")+getPAText()+AnsiString("\r\n");
    S += RLabel+AnsiString(" = ")+getRText()+AnsiString("\r\n");
    S = angviewLabel+AnsiString(" = ")+getangviewText();

    return S;
}
void TTelescopeProjectionModel::setAssignsText(const AnsiString &S)
{
    try {
        int i = 1;
        ReadAssigns(this, S, i);
        StrTravelToEnd(S, i);

    } catch(...) {
        throw;
    }
}

//MÉTODOS DE ASIMILACIÓN

//determina los planos de referencia {X, Y, Z}
//para las coordenadas angulares (RA, DEC, PA)
void TTelescopeProjectionModel::DeterminePlanes(void)
{
    //inicializa tres versores ortogolaes
    //directores de los planos {X, Y, Z}
    //coincidentes con el sistema de referencia (i, j, k)
    TVector3D u(1, 0, 0);
    TVector3D v(0, 1, 0);
    TVector3D w(0, 0, 1);

    TVector3D aux;

    //calcula el versor w normal a u y v
    //y gira u y v RA rad en torno de w
    RotateVectors(u, v, aux, u, v, getRA());
    //calcula el versor v normal a u y w
    //y gira u y w DEC rad en torno de v
    RotateVectors(u, w, aux, u, w, getDEC());
    //calcula el versor u normal a v y w
    //y gira v y w PA rad en torno de u
    RotateVectors(v, w, aux, v, w, getPA());

    //calcula el ángulo de visión en rad
    p_angview_rad = getangview()*M_PI/double(180*3600);
    //calcula la escala y la escala inversa
    p_scale = getR()/getangview();
    p_scaleinv = getangview()/getR();

    //define el plano de proyección X a partir del versor director u
    A1 = u.x;
    B1 = u.y;
    C1 = u.z;
    D1 = -getR()/tan(getangview_rad());

    //define el plano de abcisas Y a partir del versor director v
    A2 = v.x;
    B2 = v.y;
    C2 = v.z;
    D2 = 0;

    //define el plano de proyección Z a partir del versor director w
    A3 = w.x;
    B3 = w.y;
    C3 = w.z;
    D3 = 0;
}

//MÉTODOS ESTÁTICOS:

//imprime las propiedades de configuración en una cadena de texto
//en formato de asignaciones
void  TTelescopeProjectionModel::PrintAssigns(AnsiString &S,
                                              const TTelescopeProjectionModel *TPM)
{
    //el puntero TPM debería apauntar a un Telescope Projection Model construído
    if(TPM == NULL)
        throw EImproperArgument("pointer TPM should point to built Telescope PRojection Model");

    S += TPM->getAssignsText();
}

//lee las propiedades de configuración en una cadena de texto
//en formato de asignaciones
void  TTelescopeProjectionModel::ReadAssigns(TTelescopeProjectionModel *TPM,
                                             const AnsiString &S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //el puntero TPM debería apauntar a un Telescope Projection Model construído
    if(TPM == NULL)
        throw EImproperArgument("pointer TPM should point to built Telescope PRojection Model");

    //estado de lectura:
    //      0: esperando asignación a RA
    //      1: esperando asignación a DEC
    //      2: esperando asignación a PA
    //      3: esperando asignación a R
    //      4: esperando asignación a angview
    //      5: instancia leída con éxito
    int status = 0;

    //variables tampón
    double RA, DEC, PA;
    double R, angview;

    do {
        switch(status) {
        case 0: //esperando asignación a RA
            try {
            //atraviesa la etiqueta de la propiedad
            StrTravelLabel(RALabel, S, i);
            //atraviesa el operador de asignación
            StrTravelLabel("=", S, i);
            //lee la propiedad en la variable tampón
            StrReadFloat(RA, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;
        case 1: //esperando asignación a DEC
            try {
            //atraviesa la etiqueta de la propiedad
            StrTravelLabel(DECLabel, S, i);
            //atraviesa el operador de asignación
            StrTravelLabel("=", S, i);
            //lee la propiedad en la variable tampón
            StrReadFloat(DEC, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;
        case 2: //esperando asignación a PA
            try {
            //atraviesa la etiqueta de la propiedad
            StrTravelLabel(PALabel, S, i);
            //atraviesa el operador de asignación
            StrTravelLabel("=", S, i);
            //lee la propiedad en la variable tampón
            StrReadFloat(PA, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;
        case 3: //esperando asignación a R
            try {
            //atraviesa la etiqueta de la propiedad
            StrTravelLabel(RLabel, S, i);
            //atraviesa el operador de asignación
            StrTravelLabel("=", S, i);
            //lee la propiedad en la variable tampón
            StrReadFloat(R, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;
        case 4: //esperando asignación a angview
            try {
            //atraviesa la etiqueta de la propiedad
            StrTravelLabel(angviewLabel, S, i);
            //atraviesa el operador de asignación
            StrTravelLabel("=", S, i);
            //lee la propiedad en la variable tampón
            StrReadFloat(angview, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientras no se haya leido la instancia con éxito
    } while(status < 5);

    //asigna las variables tampón
    try {
        TPM->Set(RA, DEC, PA, R, angview);
    } catch(...) {
        throw;
    }
}

//MÉTODOS PÚBLICOS:

//contruye un telescopio
TTelescopeProjectionModel::TTelescopeProjectionModel(void) : TFocalPlane(),
    //inicializa las propiedades
    p_Tile(NULL),
    p_RA(0), p_DEC(0), p_PA(0),
    p_R(850), p_angview(1031.305)
{
    //asimila la configuración
    DeterminePlanes();
}
//copia las proopiedades de un telescopio
void TTelescopeProjectionModel::Copy(TTelescopeProjectionModel *T)
{
    //el puntero T debería apuntar a un telescopio contruido
    if(T == NULL)
        throw EImproperArgument("pointer T should point to built telescope");

    //copia las propiedades del plano focal
    CopyFocalPlane(T);

    //copia las demás propiedades
    p_RA = T->p_RA;
    p_DEC = T->p_DEC;
    p_PA = T->p_PA;
    p_R = T->p_R;
    p_angview_rad = T->p_angview_rad;
    p_angview_rad = T->p_angview_rad;
    p_scale = T->p_scale;
    p_scaleinv = T->p_scaleinv;
}
//clona un telescopio
TTelescopeProjectionModel::TTelescopeProjectionModel(TTelescopeProjectionModel *T)
{
    try {
        Copy(T);
    } catch(...) {
        throw;
    }
}

//--------------------------------------------------------------------------

//asigna conjuntamente las propiedades de configuración
void TTelescopeProjectionModel::Set(double RA, double DEC, double PA,
                                    double R, double angview)
{
    //la declinación DEC debería estar en [-pi/2, pi/2]
    if(DEC<-M_PI/2 || M_PI/2<DEC)
        throw EImproperArgument(AnsiString("declination '")+FloatToStr(DEC)+AnsiString("' should be in [-pi/2, pi/2]"));
    //el radio del plano focal debe ser mayor que cero
    if(R == 0)
        throw EImproperArgument("focal plane radio R should be upper zero");
    //el ángulo de visión debe ser mayor que cero
    if(angview == 0)
        throw EImproperArgument("the angle of view angview should be upper zero");

    //asigna los nuevos valores
    p_RA = RA;
    p_DEC = DEC;
    p_PA = PA;
    p_R = R;
    p_angview = angview;
}

//apunta el telescopio en una dirección
void TTelescopeProjectionModel::Point(double RA, double DEC)
{
    //la declinación theta' debería estar en [-pi/2, pi/2]
    if(DEC<-M_PI/2 || M_PI/2<DEC)
        throw EImproperArgument(AnsiString("declination '")+FloatToStr(DEC)+AnsiString("' should be in [-pi/2, pi/2]"));

    //asigna el nuevo valor
    p_RA = RA;
    p_DEC = DEC;
    //asimila el nuevo valor
    DeterminePlanes();
}

//apunta el telescopio y orienta el plano focal
void TTelescopeProjectionModel::PointAndOrientate(double RA, double DEC, double PA)
{
    //la declinación theta' debería estar en [-pi/2, pi/2]
    if(DEC<-M_PI/2 || M_PI/2<DEC)
        throw EImproperArgument(AnsiString("declination '")+FloatToStr(DEC)+AnsiString("' should be in [-pi/2, pi/2]"));

    //asigna el nuevo valor
    p_RA = RA;
    p_DEC = DEC;
    p_PA = PA;
    //asimila el nuevo valor
    DeterminePlanes();
}

//proyecta un punto de la esfera celeste sobre el plano focal
void TTelescopeProjectionModel::Project(TDoublePoint &P, double RA, double DEC)
{
    //Para proyectar se pone el plano X a una distancia (-D, 0, 0)
    //del origen del sistema de coordenadas celestes.

    //calcula el versor en la dirección del punto (DEC, RA)
    double aux = cos(DEC);
    double P_x = aux*cos(RA);
    double P_y = aux*sin(RA);
    double P_z = sin(DEC);

    //calcula el denominador del coeficiente de incremento del versor P_
    aux = A1*P_x + B1*P_y + C1*P_z;

    //Cuando el versor del punto P_ es paralelo al plano X,
    //el denominador del coeficiente de incremento será cero.

    //comprueba las precondiciones
    if(aux == 0)
        throw EImproperArgument("the denominator of increment coeficient should not be zero");

    //Calcula el punto de intesección P de la recta que pasa por (O, P_)
    //con el plano focal X:

    aux = -D1/aux; //coeficiente de incremento para el versor
    double Px = P_x*aux;
    double Py = P_y*aux;
    double Pz = P_z*aux;

    //calcula la distancia al plano de abcisas
    P.x = distancePointPlane(Px, Py, Pz, A2, B2, C2, D2);
    //calcula la distancia al plano de ordenadas
    P.y = distancePointPlane(Px, Py, Pz, A3, B3, C3, D3);
}

//proyecta un punto del plano focal sobre la esfera celeste
void TTelescopeProjectionModel::Deproject(double &RA, double &DEC, TDoublePoint P)
{
    //calcula el punto en el espacio resolviendo:
    //      A1 x + B1 y + C1 z = -D1
    //      A2 x + B2 y + C2 z = P.x sqrt(A2^2 + B2^2 + C2^2) - D2
    //      A3 x + B3 y + C3 z = P.y sqrt(A3^2 + B3^2 + C3^2) - D3
    //donde:
    //      (x, y, z) es el punto en el espacio que queremos obtener;
    //      P es el punto en el plano de proyección.

    double aux = A1*(B2*C3 - C2*B3) + B1*(C2*A3 - A2*C3) + C1*(A2*B3 - B2*A3);

    double s = P.x*sqrt(A2*A2 + B2*B2 + C2*C2) - D2;
    double t = P.y*sqrt(A3*A3 + B3*B3 + C3*C3) - D3;

    double Px = D1*(C2*B3 - B2*C3) + s*(C1*B3 - B1*C3) + t*(B1*C2 - C1*B2);
    double Py = D1*(A2*C3 - C2*A3) + s*(A1*C3 - C1*A3) + t*(C1*A2 - A1*C2);
    double Pz = D1*(B2*A3 - A2*B3) + s*(B1*A3 - A1*B3) + t*(A1*B2 - B1*A2);
    /*
        //(x, y, z) debe corresponder a (-D1, P.x, P.y), pero es calculado
        //por el procecimiento más complicado para evitar los errores numéricos.
        double Px = -D1;
        double Py = P.x;
        double Pz = P.y;
    */
    //proyecta el punto sobre la esfera celeste
    aux = sqrt(Px*Px + Py*Py + Pz*Pz);
    double P_x, P_y, P_z;
    if(getscaleinv() < 0) {
        P_x = -Px/aux;
        P_y = -Py/aux;
        P_z = -Pz/aux;
    } else {
        P_x = Px/aux;
        P_y = Py/aux;
        P_z = Pz/aux;
    }

    //calcula la declinación
    DEC = Arg(sqrt(P_x*P_x + P_y*P_y), P_z);
    //calcula la ascensión recta
    RA = Arg(P_x, P_y);
}

//------------------------------------------------------------------

//adscribe un azulejo al telescopio y
//apunta el telescopio al centro
void TTelescopeProjectionModel::Point(TTile *Tile)
{
    //comprueba las precondiciones
    if(Tile == NULL)
        throw EImproperArgument("pointer Tile should point to built tile");

    //adscribe el azulejo externo
    p_Tile = Tile;
    //apunta el telescopio al centro del azulejo
    Point(getTile()->RA, getTile()->getDEC());

    //NOTA: aquí se podría orientar el plano focal del telescopio según
    //la dirección por defecto del azulejo indicado, pero en principio
    //no tiene mucho sentido hacerlo.
}

//------------------------------------------------------------------

//apunta el telescopio al centro del azulejo adscrito
void TTelescopeProjectionModel::Repoint(void)
{
    //comprueba las precondiciones
    if(getTile() == NULL)
        throw EImproperCall("should have an attached tile");

    //apunta el telescopio al centro del azulejo externo adscrito
    Point(getTile()->RA, getTile()->getDEC());
}

//proyecta una lista de puntos de cielo
//sobre el plano focal del telescopio
void TTelescopeProjectionModel::Project(TSkyPointList& SPL)
{
    //inicializa las listas de puntos de proyección
    ProjectionPointList.Clear();
    InnerProjectionPointList.Clear();

    //por cada punto de cielo de la lista
    for(int i=0; i<SPL.getCount(); i++) {
        //apunta el punto de cielo para facilitar su acceso
        TSkyPoint *SP = SPL[i];
        //contruye el punto de proyección adscrito
        TProjectionPoint *PP = new TProjectionPoint(SP);
        //proyecta la ubicación del punto de cielo
        Project(*PP, SP->getRA(), SP->getDEC());
        //añade el punto de proyección a la lista
        ProjectionPointList.Add(PP);
        //si el punto de proyección está dentro del plano focal
        if(PointIsInFocalPlane(*PP))
            //añade el punto de proyección a la lista de internos
            InnerProjectionPointList.Add(PP);
    }
}

//proyecta el azulejo y los puntos de proyección del azulejo adscrito
void TTelescopeProjectionModel::Project(void)
{
    //comprueba las precondiciones
    if(getTile() == NULL)
        throw EImproperCall("should have an attached tile");

    //si no hay un círculo de proyección
    if(getProjectionCircle() == NULL)
        //contruye el círculo de proyección
        p_ProjectionCircle = new TProjectionCircle();

    //proyecta el centro del azulejo sobre el plano focal
    //y lo asigna al centro del círculo de proyección
    Project(getProjectionCircle()->O, getTile()->RA, getTile()->getDEC());
    //calcula el radio del círculo de proyección
    //y lo asigna al radio del círculo de proyección
    double R = fabs(D1*tan(getTile()->getR_()));
    getProjectionCircle()->setR(R);

    //NOTA: aquí podría orientar el círculo de proyección en la dirección
    //por defecto del azulejo proyectado, pero en principio
    //no tiene mucho sentido hacerlo
    //ProjectionCircle->gamma = Tile->PA - PA;

    //proyecta la lista de puntos de cielo del azulejo
    Project(getTile()->SkyPointList);
}

//randomiza los puntos de proyección con
//distribución uniforme sobre el círculo de proyección
void TTelescopeProjectionModel::RandomizeProjectionPoints(void)
{
    TProjectionPoint *PP;

    //inciializa la lista de puntos de proyección internos
    InnerProjectionPointList.Clear();

    //por cada punto de proyección de la lista
    for(int i=0; i<ProjectionPointList.getCount(); i++) {
        //apunta el punto de proyección indicado para facilitar su acceso
        PP = ProjectionPointList.GetPointer(i);
        //asigna un punto aleatorio del dominio del círculo de proyección
        TDoublePoint aux = getProjectionCircle()->RandomPoint();
        PP->x = aux.x;
        PP->y = aux.y;
        //si el punto de proyección está dentro del plano focal
        if(PointIsInFocalPlane(TDoublePoint(*PP)))
            //añade el punto de proyección a la lista de internos
            InnerProjectionPointList.Add(PP);
    }
}

//deproyecta los puntos de proyección del plano focal
//sobre los puntos de cielo de la lista externa adscrita
void TTelescopeProjectionModel::DeprojectProjectionPoints(void)
{
    TProjectionPoint *PP;
    TSkyPoint *CO;

    //por cada punto de proyección de la lista
    for(int i=0; i<ProjectionPointList.getCount(); i++) {
        //apunta el punto de proyección indicado para facilitar su acceso
        PP = ProjectionPointList.GetPointer(i);

        //si el punto tiene un punto de cielo adscrito
        if(PP->getSkyPoint() != NULL) {
            //apunta el punto de cielo adscrito para facilitar su acceso
            CO = PP->getSkyPoint();
            //proyecta el punto del plano focal sobre la esfera celeste
            double RA, DEC;
            Deproject(RA, DEC, *PP);
            CO->SetPosition(RA, DEC);
        }
    }

    //NOTA: aquí no se deproyectarán el círculo de proyección,
    //ya que este método solo sirve para randomizar las coordenadas
    //de los puntos de cielo, mediante el randomizado de
    //sus puntos de proyección correspondientes.
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

