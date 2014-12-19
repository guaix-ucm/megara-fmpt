//---------------------------------------------------------------------------
//Archivo: Barrier.h
//Contenido: barrera
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef BARRIER_H
#define BARRIER_H

#include <stdint.h>

#include "Figure.h"
#include "ContourFigureList.h"
//##include "taboutofdialog.h"
//##include "..\5_Graphics\PloterShapes.h"
#include "Function.h"
#include "Quantificator.h"
#include "Vector.h"
#include "Vectors.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TBarrier
//---------------------------------------------------------------------------

//clase barrera
class TBarrier {
        //PROPIEDADES:

        TContourFigureList __Contour_;

        double __r_max;

        double __thetaO1;
        TDoublePoint __P0;

        TContourFigureList __Contour;

        double __SPM;

        //###################################################################
        //PROPIEDADES EN FORMATO TEXTO:
        //###################################################################

        //PROPIEDADES EN FORMATO TEXTO:

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //###################################################################
        //MÉTODOS PRIVADOS:
        //###################################################################

        //MÉTODOS DE ASIMILACIÓN:

        //a partir de:
        //      {Contour_}
        //      {thetaO1, P0}
        //determina:
        //      {Contour}
        void CalculateImage(void);

public:
        //###################################################################
        //PROPIEDADES PÚBLICAS:
        //###################################################################

        //PROPIEDADES:

        //contorno de la pared en S1
        //valor por defecto: {}
        const TContourFigureList &getContour_(void) const {
                return (const TContourFigureList&)__Contour_;}
        void setContour_(const TContourFigureList &_Contour_);

        //distancia entre el origen de coordenadas de S1
        //y el punto más alejado de Contour_
        double getr_max(void) const {return __r_max;}

        //punto origen de S1 en S0 y de referencia de la pared
        //valor por defecto: {0, 0} {mm, mm}
        TDoublePoint getP0(void) const {return __P0;}
        void setP0(TDoublePoint);
        //orientación de S1 en S0
        //valor por defecto: 3./4.*M_2PI rad
        double getthetaO1(void) const {return __thetaO1;}
        void setthetaO1(double);

        //contorno de la barrera en S0
        //valor por defecto: {}
        const TContourFigureList &getContour(void) const {
                return (const TContourFigureList&)__Contour;}
        //margen perimetral de seguridad de la barrera
        //debe ser un valor mayor que cero
        //valor por defecto: MEGARA_SPMsta mm
        double getSPM(void) const {return __SPM;}
        void setSPM(double);

        //###################################################################
        //PROPIEDADES EN FORMATO TEXTO:
        //###################################################################

        //PROPIEDADES EN FORMATO TEXTO:

        AnsiString getContour_AddressText(void) const {
                return IntToHex(reinterpret_cast<intptr_t>(&__Contour_), 8);}
        void setContour_Text(const AnsiString&);
        void setContour_ColumnText(const AnsiString&);

        AnsiString getr_maxText(void) const;

        AnsiString getthetaO1Text(void) const;
        void setthetaO1Text(const AnsiString&);
        AnsiString getP0Text(void) const;
        void setP0Text(const AnsiString&);

        AnsiString getContourAddressText(void) const {
                return IntToHex(reinterpret_cast<intptr_t>(&__Contour), 8);}
        AnsiString getSPMText(void) const;
        void setSPMText(const AnsiString&);

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjunto de todas las propiedades
        //en formato de asignaciones
        AnsiString getAllText(void) const;
        //conjunto de todas las propiedades de instancia
        //en formato de asignaciones
        AnsiString getInstanceText(void) const;
        void setInstanceText(const AnsiString&);

        //###################################################################
        //MÉTODOS PÚBLICOS:
        //###################################################################

        //-------------------------------------------------------------------
        //MÉTODOS ESTÁTICOS:

        //lee una instancia de barrera en una cadena
        static void  ReadInstance(TBarrier* &B,
                const AnsiString& S, int &i);

        //-------------------------------------------------------------------
        //MÉTODOS DE CONTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

        //contruye una barrera
        TBarrier(TDoublePoint _P0, double _thetaO1=1.5*M_PI);
        //copia una barrera
        void Copy(const TBarrier*);
        //contruye un clon de una barrera
        TBarrier(const TBarrier*);
        //libera la memoria dinámica
        ~TBarrier();

        //MÉTODOS DE MOVIMIENTO:

        //cambia la posición y orientación
        //del origen de coordenadas simultaneamente
        void Set(TDoublePoint _P0, double _thetaO1);

/*        //-------------------------------------------------------------------
        //MÉTODOS DE COLISIÓN:

        //determina si la barrera está en colisión con otra barrera
        bool Collides(const TBarrier *Barrier);
        //determina si la barrera está encolisión con un brazo
        bool Collides(const TArm *Arm);
  */
        //determina si el punto indicado está dentro de la barrera
        bool Covers(TDoublePoint);

        //-------------------------------------------------------------------
        //MÉTODOS GRÁFICOS:

        //dibuja la barrera con el color indicado
        //en un trazador
        //#void Paint(TPloterShapes*, QColor Color);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // BARRIER_H
