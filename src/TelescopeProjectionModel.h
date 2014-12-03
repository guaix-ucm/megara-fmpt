//---------------------------------------------------------------------------
//Archivo: TelescopeProjectionModel.h
//Contenido: modelo de proyección del telescopio
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef TELESCOPEPROJECTIONMODEL_H
#define TELESCOPEPROJECTIONMODEL_H

#include "SkyPoint.h"
#include "ProjectionPoint.h"
#include "Tile.h"
#include "..\A_Arm\Figure.h"
#include "..\3_Lists\PointersList.h"

#include <math.h>

using namespace Lists;

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------

//círculo de proyección
class TProjectionCircle : public TCircle {
    TDoublePoint __O;

public:
    //punto origen de coordenadas
    TDoublePoint getO(void) const {return __O;}

    //color con que son contruidos los círculos de proyección
    //valor por defecto: clWhite
    //#static QColor DefaultColor;

    //color del círculo de proyección
    //valor por defecto: DefaultColor
    //#QColor Color;

    //contruye un círculo de proyección
    TProjectionCircle(void);
    TProjectionCircle(TDoublePoint _P, double _R);
    //clona un círculo de proyección
    TProjectionCircle(TProjectionCircle *);

    //determina si un punto está en el dominio del círculo de proyección
    bool PointIsInDomain(const TDoublePoint Q);
    //determina si un punto no está en el dominio del círculo de proyección
    bool PointIsntInDomain(const TDoublePoint Q);
    //genera un punto aleatorio en el cominio del círculo
    TDoublePoint RandomPoint(void);

    //dibuja un círculo de proyección en un trazador de formas
    //#void Paint(TPloterShapes *PS);
};

/*//lista de círculos de proyección
class TProjectionCircleList : public TPointersList<TProjectionCircle> {
public:
        //dibuja los círculos de proyección en un trazador de formas
        void Paint(TPloterShapes *PS);
};
  */
//plano focal
class TFocalPlane {
protected:
    TDoublePoint __O;
    double __R;
    TProjectionCircle *__ProjectionCircle;

public:
    //PROPIEDADES CONSTANTES:

    //punto origen de coordenadas
    TDoublePoint getO(void) const {return __O;}

    //PROPIEDADES:

    //radio del plano focal
    //debe ser un valor mayor que cero
    //valor por defecto: GTC_R mm:
    double getR(void) const {return __R;} void setR(double);

    //CONSEJO: para obtener parámetros estadísticos se aconseja mantener
    //el radio del plano focal igual al radio del dominio del instrumento.

    //lista de puntos de proyección de los puntos de cielo del azulejo
    TProjectionPointList ProjectionPointList;
    //círculo de proyección del azulejo adscrito
    //valor por defecto: NULL
    TProjectionCircle *getProjectionCircle(void) const {
        return __ProjectionCircle;}

    //es conceptualmente más correcto que ProjectionCircle sea un puntero
    //porque una circunferencia de radio nulo puede ser entendida como
    //un punto.

    //lista de puntos de proyección de los puntos de cielo del azulejo
    //dentro del plano focal
    TItemsList<TProjectionPoint*> ListInnerProjectionPoints;

    //color del borde del plano focal
    //valor por defecto: clYellow
    //#QColor Color;

    //indica si debe dibujar el límite del plano focal
    //valor por defecto: false
    bool PaintCircunference;
    //indica si debe dibujar el círculo de proyección
    //valor por defecto: true
    bool PaintProjectionCircle;

    //MÉTODOS PÚBLICOS:

    //contruye un plano focal
    //con 331 ángulos de rotación en [-pi/6, pi/6]
    //con 331 puntos de traslación equidistantes L/5 mm
    //distribuidos en 19 capas exagonales más el central
    TFocalPlane(void);

    //copia las propiedades de un plano focal
    void CopyFocalPlane(TFocalPlane *FP);

    //determina si un punto se encuentra dentro del plano focal
    bool PointIsInFocalPlane(const TDoublePoint P);

    //borra los puntos de proyección y el círculo de proyección
    void Clear(void);

    //dibuja el plano focal en un trazador de formas
    //#void PaintFocalPlane(TPloterShapes*);
};

//clase modelo de proyección del telescopio
class TTelescopeProjectionModel : public TFocalPlane {
    //OBJETOS EXTERNOS ADSCRITOS:

    TTile *__Tile;

    //PROPIDADES DEFINITORIAS:

    double __RA;
    double __DEC;
    double __PA;

    double __R;
    double __angview;
    double __angview_rad;
    double __scale;
    double __scaleinv;

    //PROPIEDADES DEFINITORIAS EN FORMATO TEXTO:

    //CONJUNTO DE PROPIEDADES DEFINITORIAS EN FORMATO TEXTO:

    //PROPIEDADES INTERNAS:

    //plano focal X (ortogonal a u)
    double A1, B1, C1, D1;
    //plano de abcisas Y (ortogonal a v)
    double A2, B2, C2, D2;
    //plano de ordenadas Z (ortogonal a w)
    double A3, B3, C3, D3;

    //MÉTODOS DE ASIMILACIÓN

    //determina los planos de referencia {X, Y, Z}
    //para las coordenadas angulares (RA, DEC, PA)
    void DeterminePlanes(void);

public:
    //ETIQUETAS DE LAS PROPIEDADES:

    //etiqueta de la propiedad RA
    //debe ser imprimible
    //valor por defecto: "RA"
    static AnsiString RALabel;
    //etiqueta de la propiedad DEC
    //debe ser imprimible
    //valor por defecto: "DEC"
    static AnsiString DECLabel;
    //etiqueta de la propiedad PA
    //debe ser imprimible
    //valor por defecto: "PA"
    static AnsiString PALabel;
    //etiqueta de la propiedad R
    //debe ser imprimible
    //valor por defecto: "R"
    static AnsiString RLabel;
    //etiqueta de la propiedad angview
    //debe ser imprimible
    //valor por defecto: "angview"
    static AnsiString angviewLabel;
    //etiqueta de la propiedad scale
    //debe ser imprimible
    //valor por defecto: "scale"
    static AnsiString scaleLabel;
    //etiqueta de la propiedad scaleinv
    //debe ser imprimible
    //valor por defecto: "scaleinv"
    static AnsiString scaleinvLabel;

    //OBJETOS EXTERNOS ADSCRITOS:

    //puntero al azulejo externo adscrito
    //valor por defecto: NULL
    TTile *getTile(void) const {return __Tile;}

    //función de dibujo externa adscrita
    //para actualizar la caja de pintura del plano focal
    //valor por defecto: NULL
    void (__fastcall *PaintAll)(void);

    //PROPIDADES DE CONFIGURACIÓN:

    //ascensión recta del punto de apuntado del telescopio
    //valor por defecto: 0
    double getRA(void) const {return __RA;} void setRA(double);
    //declinación del punto de apuntado del telescopio
    //debe estar en [-pi/2, pi/2]
    //valor por defecto: 0
    double getDEC(void) const {return __DEC;} void setDEC(double);
    //ángulo de orientación del plano focal del telescopio
    double getPA(void) const {return __PA;} void setPA(double);

    //radio del plano focal
    //debe ser mayor que cero
    //valor por defecto: 850 mm
    double getR(void) const {return __R;} void setR(double);
    //ángulo de visión del telescopio
    //debe ser mayor que cero
    //valor por defecto: 1031.305 arcsec
    double getangview(void) const {return __angview;}
    void setangview(double);

    //En este modelo la escala viene determinada por:
    //      D1 = -d = -R/tan(angview_rad);
    //de modo que las sigueintes propiedades no son utilizadas
    //para proyectar o deproyectar puntos, siendo proporcionadas
    //por si resultasen útiles al uauario.

    //ángulo de visión en radianes
    //      angview_rad = angview*M_PI/double(180*3600)
    //valor por defecto: 0.0049999077339667 rad
    double getangview_rad(void) const {return __angview_rad;}
    //factor de escala o aumento del telescopio
    //      scale = R/angview
    //valor por defecto: ? mm/arcsec
    double getscale(void) const {return __scale;}
    //factor de escala inversa o aumento del telescopio
    //      scale = angview/R
    //valor por defecto: 1.2133 arcsec/mm
    double getscaleinv(void) const {return __scaleinv;}

    //En MEGARA-GTC:
    //      R = 850 mm
    //      angview = 1031.305 arcsec = 17.188416666667 arcmin
    //              = 0.28647361111111111 deg
    //
    //      angview_rad = 0.0049999077339667 rad
    //      scale = 0.824198466990851 mm/arcsec
    //      scaleinv = 1.2133 arcsec/mm
    //
    //      tan(angview) = 0.00499994939874339 mm/mm
    //      D1 = -d = -r/tan(angview) = 170001.720460136 mm
    //
    //La escala de GTC es 1.2133 arcsec/mm a 8ºC.
    //El margen de error es de 1/1000 en el el borde del plano focal.

    //PROPIEDADES DE CONFIGURACIÓN EN FORMATO TEXTO:

    AnsiString getRAText(void) const;
    void setRAText(const AnsiString&);
    AnsiString getDECText(void) const;
    void setDECText(const AnsiString&);
    AnsiString getPAText(void) const;
    void setPAText(const AnsiString&);

    AnsiString getRText(void) const;
    void setRText(const AnsiString&);
    AnsiString getangviewText(void) const;
    void setangviewText(const AnsiString&);

    AnsiString getangview_radText(void) const;
    AnsiString getscaleText(void) const;
    AnsiString getscaleinvText(void) const;

    //CONJUNTO DE PROPIEDADES DEFINITORIAS EN FORMATO TEXTO:

    //cluster de propiedades deinitorias en formato de asignaciones
    AnsiString getAssignsText(void) const;
    void setAssignsText(const AnsiString&);

    //MÉTODOS ESTÁTICOS:

    //imprime las propiedades de configuración en una cadena de texto
    //en formato de asignaciones
    static void __fastcall PrintAssigns(AnsiString &S,
                                        const TTelescopeProjectionModel *TPM);

    //lee las propiedades de configuración en una cadena de texto
    //en formato de asignaciones
    static void __fastcall ReadAssigns(TTelescopeProjectionModel *TPM,
                                       const AnsiString &S, int &i);

    //------------------------------------------------------------------
    //MÉTODOS PÚBLICOS:

    //contruye un telescopio
    TTelescopeProjectionModel(void);
    //clona un telescopio
    TTelescopeProjectionModel(TTelescopeProjectionModel*);

    //copia las proopiedades de un telescopio
    void Copy(TTelescopeProjectionModel*);

    //------------------------------------------------------------------

    //asigna conjuntamente las propiedades de configuración
    void Set(double _RA, double _DEC, double _PA,
             double _R, double _angview);

    //apunta el telescopio hacia un punto de la esfera celeste
    void Point(double _RA, double _DEC);

    //ADVERTENCIA: al invocar a la función Point debe tenerse cuidado
    //de no invocar a la función global Point(double, double)
    //la cual no hará nada y enmascarará el fallo.

    //apunta el telescopio y orienta el plano focal
    void PointAndOrientate(double _RA, double _DEC, double _PA);

    //proyecta un punto de la esfera celeste sobre el plano focal
    void Project(TDoublePoint &P, double RA, double DEC);
    //proyecta un punto del plano focal sobre la esfera celeste
    void Deproject(double &RA, double &DEC, TDoublePoint P);

    //------------------------------------------------------------------

    //adscribe un azulejo al telescopio y
    //apunta el telescopio al centro
    void Point(TTile *_Tile);

    //apunta el telescopio al centro del azulejo adscrito
    void Repoint(void);

    //proyecta una lista de puntos de cielo
    //sobre el plano focal del telescopio
    void Project(TSkyPointList&);

    //Este método será usado con listas de puntos de cielo
    //cuyos puntos de proyección estén asignados en un CB,
    //para determinar las nuevas coordenadas de los puntos
    //de proyección cuando se produzca un offset.

    //ADVERTENCIA: la proyección de una lista de puntos no implica
    //la proyección del centro del azulejo que el telescopio tenga adscrito.

    //proyecta el azulejo y los puntos de proyección del azulejo adscrito
    void Project(void);

    //ADVERTENCIA: Project construirá nuevos puntos de proyección
    //sin mantener los previamente existentes, de modo que pueden
    //cambiar de ubicación en la memoria.

    //randomiza los puntos de proyección con
    //distribución uniforme sobre el círculo de proyección
    //        void RandomizeProjectionPoints(void);

    //deproyecta los puntos de proyección del plano focal
    //sobre los puntos de cielo del azulejo externo adscrito
    void DeprojectProjectionPoints(void);

    //------------------------------------------------------------------
    //MÉTODOS DE INTERFAZ:

    //dibuja en el lienzo de la fotografía de un trazador de formas:
    //      los puntos de proyección
    //      el círculo de proyección
    //      el plano focal
    //#void Paint(TPloterShapes *PS);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // TELESCOPEPROJECTIONMODEL_H
