//---------------------------------------------------------------------------
//Archivo: TelescopeProjectionModel.h
//Contenido: modelo de proyección del telescopio
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "TelescopeProjectionModel.h"
#include "..\A_Arm\FiberMOSModelConstants.h" //GTC_R
#include "..\1_Mathematics\Geometry.h"
#include "..\1_Mathematics\Scalars.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TProjectionCircle
//---------------------------------------------------------------------------

//PROPIEDADES ESTÁTICAS:

//inicializa el color con que son contruidos los círculos de proyección
//#QColor TProjectionCircle::DefaultColor = Qt::white;

//MÉTODOS PÚBLICOS:

//contruye un círculo de proyección
TProjectionCircle::TProjectionCircle(void) : TCircle()
{
    //inicializa las constantes
    __O.x = 0;
    __O.y = 0;

    //inicializa las propiedades heredadas
    __R = 0;
    //#Color = DefaultColor;
}
TProjectionCircle::TProjectionCircle(TDoublePoint _P, double _R) : TCircle(_P, _R)
{
    //inicializa las propiedades heredadas
    //#Color = DefaultColor;
}
//clona un círculo de proyección
TProjectionCircle::TProjectionCircle(TProjectionCircle *PC) : TCircle(PC)
{
    //el puntero PC debería apuntar a un punto d eprotyección contruido
    if(PC == NULL)
        throw EImproperArgument("pointer PC should point to built projection circle");

    //copia las propiedades no heredadas
    __O = PC->__O;
    //#Color = PC->Color;
}

//determina si un punto está en el dominio del círculo de proyección
bool TProjectionCircle::PointIsInDomain(const TDoublePoint Q)
{
    //si el punto está a una distancia del origen
    //mayor que el radio del cñirculo
    if(Mod(Q - P) <= getR())
        return true; //indica que está en el dominio

    return false; //indica que no está en el dominio
}
//determina si un punto no está en el dominio del círculo de proyección
bool TProjectionCircle::PointIsntInDomain(const TDoublePoint Q)
{
    //si el punto está a una distancia del origen
    //mayor que el radio del círculo
    if(Mod(Q - P) > getR())
        return true; //indica que no está en el dominio

    return false; //indica que si está en el dominio
}

TDoublePoint TProjectionCircle::RandomPoint(void)
{
    TDoublePoint P;

    do {
        //randomiza el punto en el intervalo cartesiano
        P.x = RandomUniform(-getR(), getR());
        P.y = RandomUniform(-getR(), getR());
        //mientras no esté dentro del dominio
    } while(PointIsntInDomain(P));

    return P;
}
/*#
//dibuja un círculo de proyección en un trazador de formas
void TProjectionCircle::Paint(TPloterShapes *PS)
{
    //el puntero PS debería apuntar a un trazador de formas contruido
    if(PS == NULL)
        throw EImproperArgument("pointer PS should point to built ploter shapes");

    //establece el color de la pluma
    PS->setPenColor(Color);

    //dibuja el perímetro del círculo de proyección
    PS->Circunference(P, getR());
}
*/
/*//---------------------------------------------------------------------------
//TProjectionCircleList
//---------------------------------------------------------------------------

//dibuja los círculos de proyección en un trazador de formas
void TProjectionCircleList::Paint(TPloterShapes *PS)
{
        //el puntero PS debería apuntar a un trazador de formas contruido
        if(PS == NULL)
                throw EImproperArgument("pointer PS should point to built ploter shapes");

        //por cada círculo de rpoyección de la lista
        for(int i=0; i<Count; i++)
                //dibuja el perímetro del círculo de proyección indicado
                Items[i]->Paint(PS);
}
  */
//---------------------------------------------------------------------------
//TFocalPlane
//---------------------------------------------------------------------------

//PROPIEDADES:

void TFocalPlane::setR(double _R)
{
    //el radio del plano focal R debería ser mayor que cero
    if(_R <= 0)
        throw EImproperArgument(AnsiString("focal plane radio '")+FloatToStr(_R)+AnsiString("' should be upper zero"));

    //asigna el nuevo valor
    __R = _R;
}

//MÉTODOS PÚBLICOS:

//contruye un plano focal
//con 331 ángulos de rotación en [-pi/6, pi/6]
//con 331 puntos de traslación equidistantes L/5 mm
//distribuidos en 19 capas exagonales más el central
TFocalPlane::TFocalPlane(void) :
    __R(GTC_R),
    __ProjectionCircle(NULL),
    ProjectionPointList(),
    ListInnerProjectionPoints(),
    //#Color(Qt::yellow),
    PaintCircunference(false),
    PaintProjectionCircle(true)
{
    try {
        //inicializa el punto origen de coordenadas
        __O.x = 0;
        __O.y = 0;
    } catch(...) {
        throw;
    }
}

//copia las propiedades de un plano focal
void TFocalPlane::CopyFocalPlane(TFocalPlane *FP)
{
    //el puntero FP debería apuntar a un trazador de formas construído
    if(FP == NULL)
        throw EImproperArgument("pointer FP should point to built focal plane");

    __R = FP->__R;
    //#Color = FP->Color;
    PaintCircunference = FP->PaintCircunference;
    PaintProjectionCircle = FP->PaintProjectionCircle;
    ProjectionPointList = FP->ProjectionPointList;
    if(FP->getProjectionCircle() != NULL)
        __ProjectionCircle = new TProjectionCircle(FP->getProjectionCircle());
    else
        __ProjectionCircle = NULL;

    ListInnerProjectionPoints.Clear();
    for(int i=0; i<ProjectionPointList.getCount(); i++)
        ListInnerProjectionPoints.Add(ProjectionPointList.GetPointer(i));
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
    ListInnerProjectionPoints.Clear();
    if(__ProjectionCircle != NULL) {
        delete __ProjectionCircle;
        __ProjectionCircle = NULL;
    }
}
/*#
//dibuja el plano focal en un trazador de formas
void TFocalPlane::PaintFocalPlane(TPloterShapes *PS)
{
    //el puntero PS debería apuntar a un trazador de formas contruido
    if(PS == NULL)
        throw EImproperArgument("pointer PS should point to built ploter shapes");

    //establece el color de la pluma
    PS->setPenColor(Color);

    //dibuja el perímetro del plano focal
    if(PaintCircunference)
        PS->Circunference(getO(), getR());

    //dibuja los círculos de proyección
    if(getProjectionCircle()!=NULL && PaintProjectionCircle)
        getProjectionCircle()->Paint(PS);

    //dibuja los puntos de proyección
    ProjectionPointList.Paint(PS);
}
*/
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

void TTelescopeProjectionModel::setRA(double _RA)
{
    //asigna el nuevo valor
    __RA = _RA;
    //asimila el nuevo valor
    DeterminePlanes();
}
void TTelescopeProjectionModel::setDEC(double _DEC)
{
    //la declinación DEC debería estar en [-pi/2, pi/2]
    if(_DEC<-M_PI/2 || M_PI/2<_DEC)
        throw EImproperArgument(AnsiString("declination '")+FloatToStr(_DEC)+AnsiString("' should be in [-pi/2, pi/2]"));

    //asigna el nuevo valor
    __DEC = _DEC;
    //asimila el nuevo valor
    DeterminePlanes();
}
void TTelescopeProjectionModel::setPA(double _PA)
{
    //asigna el nuevo valor
    __PA = _PA;
    //asimila el nuevo valor
    DeterminePlanes();
}

void TTelescopeProjectionModel::setR(double _R)
{
    //el radio del plano focal debe ser mayor que cero
    if(_R == 0)
        throw EImproperArgument("focal plane radio R should be upper zero");

    //asigna el nuevo valor
    __R = _R;
    //asimila el nuevo valor
    DeterminePlanes();
}
void TTelescopeProjectionModel::setangview(double _angview)
{
    //el ángulo de visión debe ser mayor que cero
    if(_angview == 0)
        throw EImproperArgument("the angle of view angview should be upper zero");

    //asigna el nuevo valor
    __angview = _angview;
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
        setRA(StrToFloat_(S));
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
        setDEC(StrToFloat_(S));
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
        setPA(StrToFloat_(S));
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
        setR(StrToFloat_(S));
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
        setangview(StrToFloat_(S));
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
    __angview_rad = getangview()*M_PI/double(180*3600);
    //calcula la escala y la escala inversa
    __scale = getR()/getangview();
    __scaleinv = getangview()/getR();

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
void __fastcall TTelescopeProjectionModel::PrintAssigns(AnsiString &S,
                                                        const TTelescopeProjectionModel *TPM)
{
    //el puntero TPM debería apauntar a un Telescope Projection Model construído
    if(TPM == NULL)
        throw EImproperArgument("pointer TPM should point to built Telescope PRojection Model");

    S += TPM->getAssignsText();
}

//lee las propiedades de configuración en una cadena de texto
//en formato de asignaciones
void __fastcall TTelescopeProjectionModel::ReadAssigns(TTelescopeProjectionModel *TPM,
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
    //      5: instancia leida con éxito
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
    //inicializa las propiedades definitorias
    __RA(0), __DEC(0), __PA(0),
    __R(850), __angview(1031.305)
{
    //asimila la configuración
    DeterminePlanes();
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

//copia las proopiedades de un telescopio
void TTelescopeProjectionModel::Copy(TTelescopeProjectionModel *T)
{
    //el puntero T debería apuntar a un telescopio contruido
    if(T == NULL)
        throw EImproperArgument("pointer T should point to built telescope");

    //copia las propiedades del plano focal
    CopyFocalPlane(T);

    //copia las demás propiedades
    __RA = T->__RA;
    __DEC = T->__DEC;
    __PA = T->__PA;
    __R = T->__R;
    __angview_rad = T->__angview_rad;
    __angview_rad = T->__angview_rad;
    __scale = T->__scale;
    __scaleinv = T->__scaleinv;
}

//--------------------------------------------------------------------------

//asigna conjuntamente las propiedades de configuración
void TTelescopeProjectionModel::Set(double _RA, double _DEC, double _PA,
                                    double _R, double _angview)
{
    //la declinación DEC debería estar en [-pi/2, pi/2]
    if(_DEC<-M_PI/2 || M_PI/2<_DEC)
        throw EImproperArgument(AnsiString("declination '")+FloatToStr(_DEC)+AnsiString("' should be in [-pi/2, pi/2]"));
    //el radio del plano focal debe ser mayor que cero
    if(_R == 0)
        throw EImproperArgument("focal plane radio R should be upper zero");
    //el ángulo de visión debe ser mayor que cero
    if(_angview == 0)
        throw EImproperArgument("the angle of view angview should be upper zero");

    //asigna los nuevos valores
    __RA = _RA;
    __DEC = _DEC;
    __PA = _PA;
    __R = _R;
    __angview = _angview;
}

//apunta el telescopio en una dirección
void TTelescopeProjectionModel::Point(double _RA, double _DEC)
{
    //la declinación theta' debería estar en [-pi/2, pi/2]
    if(_DEC<-M_PI/2 || M_PI/2<_DEC)
        throw EImproperArgument(AnsiString("declination '")+FloatToStr(_DEC)+AnsiString("' should be in [-pi/2, pi/2]"));

    //asigna el nuevo valor
    __RA = _RA;
    __DEC = _DEC;
    //asimila el nuevo valor
    DeterminePlanes();
}

//apunta el telescopio y orienta el plano focal
void TTelescopeProjectionModel::PointAndOrientate(double _RA, double _DEC, double _PA)
{
    //la declinación theta' debería estar en [-pi/2, pi/2]
    if(_DEC<-M_PI/2 || M_PI/2<_DEC)
        throw EImproperArgument(AnsiString("declination '")+FloatToStr(_DEC)+AnsiString("' should be in [-pi/2, pi/2]"));

    //asigna el nuevo valor
    __RA = _RA;
    __DEC = _DEC;
    __PA = _PA;
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

    //el denominador del coeficiente de incremento no debe ser cero
    if(aux == 0)
        throw EImproperArgument("the denominator of increment coeficient should not be zero");

    //Calcula el punto de intesección P de la recta que pasa por (O, P_)
    //con el plano focal X:

    aux = -D1/aux; //coeficiente de incremento para el versor
    double Px = P_x*aux;
    double Py = P_y*aux;
    double Pz = P_z*aux;

    //calcula la distancia al plano de abcisas
    P.x = DistancePointPlane(Px, Py, Pz, A2, B2, C2, D2);
    //calcula la distancia al plano de ordenadas
    P.y = DistancePointPlane(Px, Py, Pz, A3, B3, C3, D3);
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
void TTelescopeProjectionModel::Point(TTile *_Tile)
{
    //el puntero Tile debería apuntar a un azulejo contruido
    if(_Tile == NULL)
        throw EImproperArgument("pointer Tile should point to built tile");

    //adscribe el azulejo externo
    __Tile = _Tile;
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
    //debería haber un azulejo adscrito
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
    ListInnerProjectionPoints.Clear();

    TProjectionPoint *PP;
    TSkyPoint *SP;

    //por cada punto de cielo de la lista
    for(int i=0; i<SPL.getCount(); i++) {
        //apunta el punto de cielo para facilitar su acceso
        SP = SPL[i];
        //contruye el punto de proyección adscrito
        PP = new TProjectionPoint(SP);
        //proyecta la ubicación del punto de cielo
        Project(PP->P, SP->getRA(), SP->getDEC());
        //añade el punto de proyección a la lista
        ProjectionPointList.Add(PP);
        //si el punto de proyección está dentro del plano focal
        if(PointIsInFocalPlane(PP->P))
            //añade el punto de proyección a la lista de internos
            ListInnerProjectionPoints.Add(PP);
    }
}

//proyecta el azulejo y los puntos de proyección del azulejo adscrito
void TTelescopeProjectionModel::Project(void)
{
    //debería haber un azulejo adscrito
    if(getTile() == NULL)
        throw EImproperCall("should have an attached tile");

    /*        TProjectionPoint *PP;
        TSkyPoint *SP;

        //inicializa las listas de puntos de proyección
        ProjectionPointList.Clear();
        ListInnerProjectionPoints.Clear();

        //por cada punto de cielo del azulejo
        for(int i=0; i<Tile->SkyPointList.Count; i++) {
                //apunta el punto de cielo para facilitar su acceso
                SP = Tile->SkyPointList[i];
                //contruye el punto de proyección adscrito
                PP = new TProjectionPoint(SP);
                //proyecta la ubicación del punto de cielo
                Project(PP->P, SP->RA, SP->DEC);
                //añade el punto de proyección a la lista
                ProjectionPointList.Add(PP);
                //si el punto de proyección está dentro del plano focal
                if(PointIsInFocalPlane(PP->P))
                        //añade el punto de proyección a la lista de internos
                        ListInnerProjectionPoints.Add(PP);
        }*/

    //proyecta la lilsta de puntos de cielo del azulejo
    Project(getTile()->SkyPointList);

    //si no hay un círculo de proyección
    if(getProjectionCircle() == NULL)
        //contruye el círculo de proyección
        __ProjectionCircle = new TProjectionCircle();

    //proyecta el centro del azulejo sobre el plano focal
    //y lo asigna al centro del círculo de proyección
    Project(getProjectionCircle()->P, getTile()->RA, getTile()->getDEC());
    //calcula el radio del círculo de proyección
    //y los asigna al radio del círculo de proyección
    getProjectionCircle()->setR(fabs(D1*tan(getTile()->getR_())));

    //NOTA: aquí podría orientar el círculo de proyección en la dirección
    //por defecto del azulejo proyectado, pero en principio
    //no tiene mucho sentido hacerlo
    //ProjectionCircle->gamma = Tile->PA - PA;
}

/*//randomiza los puntos de proyección con
//distribución uniforme sobre el círculo de proyección
void TTelescopeProjectionModel::RandomizeProjectionPoints(void)
{
        TProjectionPoint *PP;

        //inciializa la lista de puntos de proyección internos
        ListInnerProjectionPoints.Clear();

        //por cada punto de proyección de la lista
        for(int i=0; i<ProjectionPointList.Count; i++) {
                //apunta el punto de proyección indicado para facilitar su acceso
                PP = ProjectionPointList.GetPointer(i);
                //asigna un punto aleatorio del dominio del círculo de proyección
                PP->P = ProjectionCircle.RandomPoint();
                //si el punto de proyección está dentro del plano focal
                if(PointIsInFocalPlane(PP->P))
                        //añade el punto de proyección a la lista de internos
                        ListInnerProjectionPoints.Add(PP);
        }
}       */

//deproyecta los puntos de proyección del plano focal
//sobre los puntos de cielo de la lista externa adscrita
void TTelescopeProjectionModel::DeprojectProjectionPoints(void)
{
    TProjectionPoint *PP;
    int j;
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
            Deproject(RA, DEC, PP->P);
            CO->SetPosition(RA, DEC);
        }
    }

    //NOTA: aquí no se deproyectarán el círculo de proyección,
    //ya que este método solo sirve para randomizar las coordenadas
    //de los puntos de cielo, mediante el randomizado de
    //sus puntos de proyección correspondientes.
}

//---------------------------------------------------------------------------
//MÉTODOS DE INTERFAZ:
/*#
//dibuja en el lienzo de la fotografía de un trazador de formas:
//      los puntos de proyección
//      el círculo de proyección
//      el plano focal
void TTelescopeProjectionModel::Paint(TPloterShapes *PS)
{
    //el puntero PS debería apauntar a un trazador de formas construído
    if(PS == NULL)
        throw EImproperArgument("pointer PS should point to built ploter shapes");

    //dibuja el plano focal
    PaintFocalPlane(PS);
}
*/
//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

