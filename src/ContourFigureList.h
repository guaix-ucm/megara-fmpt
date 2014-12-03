//---------------------------------------------------------------------------
//Archivo: FigureList.h
//Contenido: clase lista de figuras de contorno
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef CONTOURFIGURELIST_H
#define CONTOURFIGURELIST_H

#include "Figure.h"
//##include "..\5_Graphics\PloterShapes.h"
#include "PointersList.h"
#include "Vectors.h"

//---------------------------------------------------------------------------

using namespace Mathematics;
using namespace Lists;
//#using namespace Graphics;

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TContourFigureList
//---------------------------------------------------------------------------

//ADVERTENCIA: TContourFigureList no puede derivar de la clase
//TPointersList<TContourFigure>, por que la clase TContourFigure
//es abstracta, y los objetos de la clase TContourFigure no pueden
//ser construidos por defecto.

//clase lista de figuras de contorno
class TContourFigureList : public TItemsList<TContourFigure*> {
public:
        //-------------------------------------------------------------------
        //MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACIÓN:

        //construye una lista de figuras
        TContourFigureList(int _Capacity=8);

        //ADVERTENCIA:
        //al derivar TContourFigureList de TItemsList<TContourFigure*>
        //los objetos a los que apuntan sus punteros deberán ser destruidos
        //manualmente antes de destruir el último puntero que los apunte.

        //copia una lista de figuras
        void Copy(const TContourFigureList&);
        TContourFigureList& operator=(const TContourFigureList&);

        //construye un clon de una lista de figuras
        TContourFigureList(const TContourFigureList &Contour);

        //MÉTODOS DE CONFIGURACIÓN:

        //asigna un color a todas las figuras del contorno
        //#void SetAllColor(QColor Color);

        //Colores recomendados para Color:
        //      clGray: no resaltado.
        //      clWhite: resaltado.

        //MÉTODOS DE CARACTERIZACIÓN:

        //determina si todos los punteros de la lista son nulos
        bool AreAllNULL(void) const;

        //determina si la lista de figuras constituye un contorno ordenado
        //comprobando que:
        //- La lista contiene al menos dos figuras.
        //- Cada figura se encuentra engarzada con la siguiente.
        //- Ningún vértice siguiente coincide con el vértice inicial
        //  de otra figura previa, excepto el de la última figura,
        //  que coincide con el inicial de la primera figura.
        //- Ninguna figura se interseca con las demás.
        bool IsAContourSorted(void) const;

        //ADVERTENCIA: por ahora IsAContourSorted está comprobando solamente
        //que:
        //- La lista contiene al menos dos figuras.
        //- Cada figura se encuentra engarzada con la siguiente.

        //MÉTODOS DE CÁLCULO DE DISTANCIAS CON EL CONTORNO:

        //determina la distancia mínima de un punto a este contorno
        double DistanceMin(TDoublePoint P) const;
        //determina la distancia máxima de un punto a este contorno
        double DistanceMax(TDoublePoint P) const;
        //determina la distancia mínima de un contorno a este contorno
        double DistanceMin(const TContourFigureList&) const;

        //MÉTODOS DE DETERMINACIÓN DE INTERSECCIÓN:

        //determina si la distancia de una lista de figuras
        //a esta lista de figuras es inferior
        //al margen perimetral de seguridad SPM.
        bool Collides(const TContourFigureList &C, double SPM) const;

        //Se advierte que la función que determina el estado de colisión
        //puede tener un tiempo de procesado medio ligeramente inferior
        //a la de determinación de la distancia, por que la colisión se
        //detecta a mayor distancia que la distancia igual a cero.

        //determina si un punto está en el interior del contorno
        bool IsInner(TDoublePoint P) const;

/*        //MÉTODOS PARA EL CÁLCULO DE ÁNGULOS DE GIRO:

        //determina los ángulos que hay que rotar este contorno
        //en torno al punto Q para que quede adyacente al segmento (Pa, Pb)
        void TurnSegment(TVector<double> &dts,
                TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Q);

        //determina los ángulos que hay que rotar este contorno
        //en torno al punto Q para que quede adyacente al arco (Pa, Pb, Pc, R)
        void TurnArc(TVector<double> &dts,
                TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R,
                TDoublePoint Q);

        //determina los ángulos que hay que rotar este contorno
        //en torno al punto Q para que quede adyacente al brazo Arm
        void TurnArm(TVector<double> &dts, TArm *Arm_,
                TDoublePoint Q);
*/
        //MÉTODOS DE TRANSFORMACIONES GEOMÉTRICAS:

        //obtiene la lista de figuras rotada y trasladada.
        //si elnúmero de figuras de la lista no coincide:
        //      lanza EImproperArgument
        //si alguna figuradelalista no es del mimo tipo
        //      lanza EImproperArgument
        void GetRotatedAndTranslated(TContourFigureList &Contour,
                double theta, TDoublePoint V) const;

        //MÉTODOS GRÁFICOS:

        //dibuja las figuras de una  lista
        //en un trazador de formas
        //#void Paint(TPloterShapes *PS) const;
};

//--------------------------------------------------------------------------
//FUNCIONES RELACIONADAS:

//segrega las figuras de una lista en listas de figuras concatenadas
//devuelve falso si encuentra más de dos vértices engarzados entre si
//bool Segregate(TPointersList<TContourFigureList> &LC, TContourFigureList &C);
//reduce un conjunto de listas de figuras concatenadas
//concatenando las listas que se puedan entre si
//void Concatenate(TPointersList<TContourFigureList> &LC);

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // CONTOURFIGURELIST_H
