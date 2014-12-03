//---------------------------------------------------------------------------
//Archivo: TargetPointList.h
//Contenido: lista de puntos objetivo
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef TARGETPOINTLIST_H
#define TARGETPOINTLIST_H

#include "TargetPoint.h"
#include "RoboticPositionerList3.h"

//##include <QListWidget>

//---------------------------------------------------------------------------

using namespace Models;

//espacio de nombres del posicionamiento
namespace Positioning {

//---------------------------------------------------------------------------

//clase lista de puntos objetivo
class TTargetPointList : public TItemsList<TTargetPoint*> {
protected:
        TRoboticPositionerList *__RoboticPositionerList;

public:
        //lista de RPs externa adscrito
        TRoboticPositionerList *getRoboticPositionerList(void) const {
            return __RoboticPositionerList;}

        //lista de puntos objetivo
//        TItemsList<TTargetPoint*> TargetPoints;

        //PROPIEDADES DE INTERFAZ:

        //lista de puntos objetivo en formato texto
        AnsiString getTargetPointsText(void);
        void setTargetPointsText(AnsiString&);

        //CONTRUCCIÓN Y DESTRUCCIÓN:

        //construye una lista de puntos objetivo
        //adscrita a una lista de RPs
        TTargetPointList(TRoboticPositionerList *_RoboticPositionerList);

        //borra los puntos objetivo y destruye la lista
        ~TTargetPointList();

        //BÚSQUEDA:

        //busca el punto objetivo adscrito a un posicionador
        int SearchTargetPoint(TRoboticPositioner *RP);
        //busca el punto objetivo adscrito a un posicionador identificado
        int SearchTargetPoint(int Id);

        //OBJETOS INDICADOS:

        //añade un punto objetivo para el posicionador indicado de la lista
        void AddTargetPoint(int i);
        //borra el punto objetivo indicada de la lista
        void DeleteTargetPoint(int i);

        //OBJETOS SELECCIONADOS DE LA LISTA:

        //añade puntos objetivo con la posición del punto P3
        //de los posicionadores seleccionados de la lista
        //e indica el número de posicionadores seleccionados
        int AddP3Selected(void);
        //destruye los puntos objetivo
        //de los posicionadores seleccionados de la lista
        //e indica el número de posicionadores seleccionados
        int DeleteSelected(void);

        //asigna el punto P3o a los puntos objetivo
        //de los posicionadores seleccionados de la lista
        //e indica el número de posicionadores seleccionados
        int SetP3oSelected(void);
        //asigna el punto P3 a los puntos objetivo
        //de los posicionadores seleccionados de la lista
        //e indica el número de posicionadores seleccionados
        int SetP3Selected(void);

        //randomiza los puntos objetivo con distribución uniforme
        //en el dominio del punto P3 de sus posicionadores adscrito
        //de los posicionadores seleccionados de la lista
        //e indica el número de posicionadores seleccionados
        int RandomizeSelected(void);
        //randomiza los puntos objetivo con distribución uniforme
        //en el dominio del punto P3 de sus posicionadores adscritos
        //de los posicionadores seleccionados de la lista
        //e indica el número de posicionadores seleccionados
        int RandomizeWithoutCollisionSelected(void);

        //asigna los puntos objetivo
        //al punto P3 de sus posicionadores adscritos
        //de los posicionadores seleccionados de la lista
        //e indica el número de posicionadores seleccionados
        //si algún punto objetivo no está en el dominio de
        //su posicionador adscrito lanza EImproperCall
        int MoveToTargetP3Selected(void);

        //NOTA: cunado un punto es asignado al punto P3 de un posicionador
        //cuya cuantificación está activada, el posicionador se moverá
        //a la posición estable máspróxima.

        //TODOS LOS OBJETOS DE LA LISTA:

        //añade puntos objetivo con la posición del punto P3
        //de todos los posicionadores de la lista
        void AddP3(void);
        //destruye los puntos objetivo
        //de todos los posicionadores de la lista
        void Delete_(void);

        //El método:
        //      void Delete(int i);
        //Parece enmascarado por el método:
        //      void Delete(void);

        //asigna el punto PO3 a los puntos objetivo
        //de todos los posicionadores de la lista
        void SetPO3(void);
        //asigna el punto P3 a los puntos objetivo
        //de todos los posicionadores de la lista
        void SetP3(void);

        //randomiza los puntos objetivo con distribución uniforme
        //en el dominio del punto P3 de sus posicionadores adscrito
        //de todos los posicionadores de la lista
        void Randomize(void);
        //randomiza los puntos objetivo con distribución uniforme
        //en el dominio del punto P3 de sus posicionadores adscritos
        //de todos los posicionadores de la lista
        void RandomizeWithoutCollision(void);

        //asigna los puntos objetivo
        //al punto P3 de sus posicionadores adscritos
        //si algún punto objetivo no está en el dominio de
        //su posicionador adscrito lanza EImproperCall
        //de todos los posicionadores de la lista
        void MoveToTargetP3(void);

        //NOTA: cunado un punto es asignado al punto P3 de un posicionador
        //cuya cuantificación está activada, el posicionador se moverá
        //a la posición estable máspróxima.

        //VALIDACIÓN:

        //Los siguientes métodos deben ser invocados antes de
        //desplazar los posicionadores a sus puntos objetivo:

        //busca los puntos objetivo adscritos a posicionadores repetidos
        void SearchRepeatedRoboticPositioners(TVector<int> &indices);

        //busca los puntos objetivo adscritos a posicionadores ausentes
        //en la lista de posicionadores RoboticPositionerList
        void SearchMissingRoboticPositioners(TVector<int> &indices);

        //El método SearchMissigRoboticPositioners es necesario
        //porque la lista de puntos objetivo adscrita (RoboticPositionerList)
        //puede ser manipulada.

        //busca los puntos objetivo que están fuera del dominio
        //de sus posicionadores adscritos
        void SearchOutDomineTargetPoints(TVector<int> &indices);

        //Determina la invalidez de una lista de puntos objetivo
        //para ser programada.
        //Valores de retorno:
        //      0: lista de puntos objetivo válida;
        //      1: posicionadores adscritos de los puntos objetivo indicados
        //         ausentes;
        //      2: puntos objetivo indicados fuera del dominio de
        //         sus posicionadores adscritos.
        int Invalid(TVector<int> &indices);

        //SEGREGACIÓN:

        //segrega los posicionadores de los puntos objetivo en dos listas:
        //      internos: con el brazo dentro del área de seguridad;
        //      externos: con el brazo fuera del área de seguridad;
        //si algún punto objetivo está fuera del dominio de
        //su posicionador adscrito:
        //      lanza EImproperCall
        void SegregateInOut(TRoboticPositionerList &Inners,
                TRoboticPositionerList &Outsiders);

        //ALMACENAMIENTO Y RECUPERACIÓN DE POSICIONES:

        //apila las posiciones de los posicionadores adscritos
        void PushPositions(void);
        //restaura las posiciones de los posicionadores adscritos
        void RestorePositions(void);
        //desempila las posiciones de los posicionadores adscritos
        void PopPositions(void);
        //restaura y desempila las posiciones de los posicionadores adscritos
        void RestoreAndPopPositions(void);

        //COLISIÓN:

        //levanta las banderas indicadoras de determinación de colisión
        //pendiente de todos los posicionadores adscritos a los puntos objetivo
        void EnablePendingCollisionDetermineTargetPoints(void);

        //El siguiente método debe invocarse una vez desplazados
        //los posicionadores a los puntos objetivo:

        //busca los puntos objetivo que colisionan con otros puntos objetivo
        void SearchCollindingTargetPoints(TVector<int> &indices);

        //ADVERTENCIA: cuando la cuantificación de alguno de los ejes
        //de los posicioandores adscritos esta deactivada, SearchCollinding
        //comprobará si hay colisión en las posiciones cuantificadas de
        //los ejes que tengan la cuantificación activada, con lo cual
        //el punto P3 de los posicionadores podría no coincidir exactamente
        //con el punto obejtivo.

        //------------------------------------------------------------------
        //Utilización de los métodos:
        //
        //Para que la solución del programador tenga sentido,
        //se debe partir de una posición inicial sin colisiones.
        //      RoboticPositionerList->SearchCollinding(indices)
        //
        //Comprueba la validez de la lista de puntos objetivo:
        //      Invalid(indices)
        //
        //Segrega los posicionadores internos de los externos:
        //      SegregateInOut(Inssers, Outsiders)
        //
        //Guarda las posiciones originales de los posicionadores adscritos:
        //      PushPositions();
        //
        //Mueve los posicionadores adscritos a sus puntos objetivo:
        //      MoveToTargetP3All()
        //
        //Comprueba que las posiciones finales son compatibles:
        //      Outsiders.SearchCollisions(indices)
        //------------------------------------------------------------------

        //MÉTODOS DE INTERFAZ:

        //selecciona todos los puntos objetivo de la lista
        void SelectAll(void);
        //deselecciona todos los puntos objetivo de la lista
        void DeselectAll(void);

        //imprime los puntos objetivo en una caja de lista
        //#void PrintTargetPoints(QListWidget *LB);
        //imprime los puntos objetivo en el lienzo de la fotografía
        //de un trazador de formas
        //#void PaintTargetPoints(TPloterShapes *PS);

        //busca el primer punto objetivo que está bajo el punto P
        //y que parte del punto objetivo puede ser agarrado en
        //el punto indicado:
        //      i: índice al punto objetivo bajo el punto P;
        //      dominio i: [0, TargetPoints.Count];
        //      n: parte del punto objetivo agarrada;
        //         valores posibles:
        //            0: niguna;
        //            1: punto objetivo.
        //valores de retorno:
        //      false: ninguna parte agarrada
        //      true: alguna parte agarrada
        bool Grab(int &i, int &n, TDoublePoint P);
};

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------
#endif // TARGETPOINTLIST_H
