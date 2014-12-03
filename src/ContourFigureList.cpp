//---------------------------------------------------------------------------
//Archivo: ContourFigureList.cpp
//Contenido: clase lista de figuras de contorno
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "ContourFigureList.h"
#include "Function.h"
#include "Vector.h"
#include "Strings.h"
#include "Geometry.h"
#include "Constants.h"

//---------------------------------------------------------------------------

using namespace Strings;
using namespace Operators;

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TContourFigureList
//--------------------------------------------------------------------------
//MÉTODOS DE CONSTRUCCIÓN,COPIA Y CLONACIÓN:

//construye un contorno
TContourFigureList::TContourFigureList(int _Capacity) :
        TItemsList<TContourFigure*>(_Capacity, NULL, NULL, NULL,
                TContourFigure::ContourFigurePrint, TContourFigure::ContourFigureRead)
{
}

//copia un contorno
void TContourFigureList::Copy(const TContourFigureList &C)
{
        //libera las figuras de la plantilla previa
        for(int i=0; i<getCount(); i++)
                delete Items[i];

        //iguala la longitud de la lista
        setCount(C.getCount());

        //contruye un clon de cada figura
        for(int i=0; i<getCount(); i++) {
                //apunta la figura indicada para facilitar su acceso
                TContourFigure *F = C[i];

                //si la figura es de tipo TSegment
                if(typeid(*F) == typeid(TSegment))
                        Items[i] = new TSegment((TSegment*)F);

                //si la figura es de tipo TArc
                else if(typeid(*F) == typeid(TArc))
                        Items[i] = new TArc((TArc*)F);

                //si F apunta a nulo
                else if(F == NULL)
                        Items[i] = F;

                else {
                        //figura geométrica de tipo desconocido
                        throw EImposibleError("geometrical figure type unknown");
                }
        }
}
TContourFigureList& TContourFigureList::operator=(const TContourFigureList& C)
{
        //libera las figuras de la plantilla previa
        for(int i=0; i<getCount(); i++)
                delete Items[i];

        //iguala la longitud de la lista
        setCount(C.getCount());

        //contruye un clon de cada figura
        for(int i=0; i<getCount(); i++) {
                //apunta la figura indicada para facilitar su acceso
                TContourFigure *F = C[i];

                //si la figura es de tipo TSegment
                if(typeid(*F) == typeid(TSegment))
                        Items[i] = new TSegment((TSegment*)F);

                //si la figura es de tipo TArc
                else if(typeid(*F) == typeid(TArc))
                        Items[i] = new TArc((TArc*)F);

                //si F apunta a nulo
                else if(F == NULL)
                        Items[i] = F;

                else {
                        //figura geométrica de tipo desconocido
                        throw EImposibleError("geometrical figure type unknown");
                }
        }

        //devuelve la referencia a esta lista de figurasde contorno
        //para permitir anidar asignaciones
        return *this;
}

//clona un contorno
TContourFigureList::TContourFigureList(const TContourFigureList &FigureList) :
        TItemsList<TContourFigure*>(FigureList.getCapacity(), FigureList.Compare,
                FigureList.Evaluate, FigureList.Assign,
                FigureList.Print, FigureList.Read)
{
        //copia el contorno
        Copy(FigureList);
}

//--------------------------------------------------------------------------
//MÉTODOS DE GRÁFICOS:
/*#
//asigna un color a todas las figuras del contorno
void TContourFigureList::SetAllColor(QColor Color)
{
        //por cada figura geométrica del contorno
        for(int i=0; i<getCount(); i++)
                //le asigna el color
                Items[i]->Color = Color;
}
*/
//--------------------------------------------------------------------------
//MÉTODOS DE CARACTERIZACIÓN:

//determina si todos los punteros del contorno son nulos
bool TContourFigureList::AreAllNULL(void) const
{
        for(int i=0; i<getCount(); i++)
                if(Items[i] != NULL)
                        return false;

        return true;
}

//determina si la lista de figuras constituye un contorno ordenado
//comprobando que:
//- La lista contiene al menos dos figuras.
//- Cada figura se encuentra engarzada con la siguiente.
//- Ningún vértice siguiente coincide con el vértice inicial
//  de otra figura previa, excepto el de la última figura,
//  que coincide con el inicial de la primera figura.
//- Ninguna figura se interseca con las demás.
bool TContourFigureList::IsAContourSorted(void) const
{
        //si la lista contiene menos de dos figuras
        if(getCount() < 2)
                return false; //indica que no es un contorno ordenado

        //RECORRE LA LISTA COMPROBANDO QUE CADA FIGURA
        //ESTÁ ENGARZADA CON SU ADYACENTE:

        //vértice opuesto al que se encontró conectado en la priemra figura
        char Vini;
        //vértice opuesto al que se encontró conectado en la sigueinte figura (a la actual)
        char Vnext;

        //puntero a la figura indicada
        TContourFigure *F1;
        //puntero a otra figura
        TContourFigure *F2;

        //índice a figuras de la lista
        int i = 1;

        //apunta la primera figura para facilitar su acceso
        F1 = getFirst();
        //apunta la segunda figura para facilitar su acceso
        F2 = Items[i];

        //inicializa Vini y Vnext según los primeros vértices que
        //se encuentren conectados en la primera y segunda figuras
        if(F1->getPb() == F2->getPa()) {
                Vini = 'a';
                Vnext = 'b';
        } else if(F1->getPb() == F2->getPb()) {
                Vini = 'a';
                Vnext = 'a';
        } else if(F1->getPa() == F2->getPa()) {
                Vini = 'b';
                Vnext = 'b';
        } else if(F1->getPa() == F2->getPb()) {
                Vini = 'b';
                Vnext = 'a';
        } else {
                //indica que la lista no constituye un conrtorno ordenado
                return false;
        }

        //por cada figura de la lista
        for(i=2; i<getCount(); i++) {
                //apunta la figura indicada para facilitar su acceso
                F1 = Items[i-1];
                //apunta la siguiente figura de la lista
                F2 = Items[i];

                //según sea el vértice que hay que comprobar
                switch(Vnext) {
                        case 'a': //en caso de que F1->getPa(), tenga que estar conectado a algún vértice de F2
                                if(F1->getPa() == F2->getPb()) //si está conectado a Pb
                                        Vnext = 'a'; //sindica que el próximo vértice a comprobar es el a
                                else if(F1->getPa() == F2->getPa()) //si no, si está conectado a Pa
                                        Vnext = 'b'; //sindica que el próximo vértice a comprobar es el a
                                else //si no
                                        //indica que la lista no constituye un conrtorno ordenado
                                        return false;
                                break;
                        case 'b': //en caso de que F1->getPb(), tenga que estar conectado a algún vértice de F2
                                if(F1->getPb() == F2->getPb()) //si está conectado a Pb
                                        Vnext = 'a'; //sindica que el próximo vértice a comprobar es el a
                                else if(F1->getPb() == F2->getPa()) //si no, si está conectado a Pa
                                        Vnext = 'b'; //sindica que el próximo vértice a comprobar es el a
                                else //si no
                                        //indica que la lista no constituye un conrtorno ordenado
                                        return false;
                                break;
                }
        }

        //apunta la última figura indicada para facilitar su acceso
        F1 = getLast();
        //apunta la primera figura indicada para facilitar su acceso
        F2 = getFirst();

        //según sea el vértice que hay que comprobar
        switch(Vnext) {
                case 'a': //en caso de que F1->getPa()...
                        switch(Vini) {
                                case 'a': //...tenga que estar conectado a F2->getPa()
                                        if(F1->getPa() != F2->getPa()) //si no está conectado
                                                return false; //indica que no es un contorno ordenado
                                        break;
                                case 'b': //...tenga que estar conectado a F2->getPb()
                                        if(F1->getPa() != F2->getPb()) //si no está conectado
                                                return false; //indica que no es un contorno ordenado
                                        break;
                        }
                        break;
                case 'b': //en caso de que F1->getPb()...
                        switch(Vini) {
                                case 'a': //...tenga que estar conectado a F2->getPa()
                                        if(F1->getPb() != F2->getPa()) //si no está conectado
                                                return false; //indica que no es un contorno ordenado
                                        break;
                                case 'b': //...tenga que estar conectado a F2->getPb()
                                        if(F1->getPb() != F2->getPb()) //si no está conectado
                                                return false; //indica que no es un contorno ordenado
                                        break;
                        }
                        break;
        }

        //indica que si es un contorno ordenado
        return true;
}
//---------------------------------------------------------------------------
//MÉTODOS DE CÁLCULO DE DISTANCIAS CON EL BRAZO:

//determina la distancia mínima de un punto a este contorno
double TContourFigureList::DistanceMin(TDoublePoint P) const
{
        //este contorno debería contener al menos una figura
        if(AreAllNULL())
                throw EImproperCall("this contour should contain one figure almost");

        double d; //distancia entre el punto y cada figura
        double dmin = MAXDOUBLE; //distancia mínima hasta el momento
        TFigure *F; //puntero a una figura indicada de este contorno

        //calcula la distancia entre las figuras de este contorno
        //y las figuras del otro contorno:

        //por cada figura restante de este contorno
        for(int i=0; i<getCount(); i++) {
                //apunta a la figura indicada de este contorno
                //para facilitar su acceso
                F = Items[i];

                //si el puntero no es nulo
                if(F != NULL) {
                        //calcula la distancia mínima entre el punto y la figura
                        d = F->DistancePoint(P);
                        //actualiza la distancia mínima
                        if(d < dmin)
                                dmin = d;
                }
        }

        return dmin; //devuelve la distancia mínima
}
//determina la distancia máxima de un punto a este contorno
double TContourFigureList::DistanceMax(TDoublePoint P) const
{
        //este contorno debería contener al menos una figura
        if(AreAllNULL())
                throw EImproperCall("this contour should contain one figure almost");

        double d; //distancia entre el punto y cada figura
        double dmax = 0; //distancia máxima hasta el momento
        TFigure *F; //puntero a una figura indicada de este contorno

        //calcula la distancia entre las figuras de este contorno
        //y las figuras del otro contorno:

        //por cada figura de este contorno
        for(int i=0; i<getCount(); i++) {
                //apunta a la figura indicada de este contorno
                //para facilitar su acceso
                F = Items[i];

                //si el puntero no es nulo
                if(F != NULL) {
                        //calcula la distancia entre el punto y la figura
                        d = F->DistancePointMax(P);
                        //actualiza la distancia máxima
                        if(d > dmax)
                                dmax = d;
                }
        }

        return dmax; //devuelve la distancia máxima
}
//determina la distancia mínima de un contorno a este contorno
double TContourFigureList::DistanceMin(const TContourFigureList &C) const
{
        //el contorno C debería contener al menos una figura
        if(C.AreAllNULL())
                throw EImproperArgument("contour C should contain one figure almost");

        //este contorno debería contener al menos una figura
        if(AreAllNULL())
                throw EImproperCall("this contour should contain one figure almost");

        double d; //distancia entre el par de figuras
        double dmin = MAXDOUBLE; //distancia mínima hasta el momento
        TFigure *F; //puntero a una figura indicada de este contorno
        TFigure *Fo; //puntero a una figura indicada del otro contorno
        int i, j;

        //calcula la distancia entre las figuras de este contorno
        //y las figuras del otro contorno:

        //por cada figura restante de este contorno
        for(i=0; i<getCount(); i++) {
                //apunta a la figura indicada de este contorno
                //para facilitar su acceso
                F = Items[i];
                //por cada figura del otro contorno
                for(j=0; j<C.getCount(); j++) {
                        //asigna el puntero indicado del otro contorno
                        //para facilitar su acceso
                        Fo = C[j];
                        //si ambos punteros son no nulos
                        if(F!=NULL && Fo!=NULL) {
                                //calcula la distancia entre figuras
                                d = F->Distance(Fo);
                                //si la distancia no puedo reducirse más
                                if(d <= 0)
                                        return 0; //devuelve cero
                                else //si no
                                        //actualiza la distancia mínima
                                        if(d < dmin)
                                                dmin = d;
                        }
                }
        }

        return dmin; //devuelve la distancia mínima
}

//---------------------------------------------------------------------------
//MÉTODOS DE DETERMINACIÓN DE INTERSECCIÓN CON EL BRAZO:

//determina si la distancia de un contorno a este controno
//es inferior al margen perimetral de seguridad SPM.
bool TContourFigureList::Collides(const TContourFigureList &C, double SPM) const
{
        //el contorno C debería contener al menos una figura
        if(C.AreAllNULL())
                throw EImproperArgument("contour C should contain one figure almost");

        double d; //distancia entre el par de figuras
        TFigure *F; //puntero a una figura indicada de este contorno
        TFigure *Fo; //puntero a una figura indicada del otro contorno
        int i, j;

        //calcula la distancia entre las figuras de este contorno
        //y las figuras del otro contorno:

        //por cada figura restante de este contorno
        for(i=0; i<getCount(); i++) {
                //apunta a la figura indicada de este contorno
                //para facilitar su acceso
                F = Items[i];
                //por cada figura del otro contorno
                for(j=0; j<C.getCount(); j++) {
                        //asigna el puntero indicado del otro contorno
                        //para facilitar su acceso
                        Fo = C[j];
                        //si ambos punteros son no nulos
                        if(F!=NULL && Fo!=NULL) {
                                //calcula la distancia entre figuras
                                d = F->Distance(Fo);
                                //si la distancia no puedo reducirse más
                                if(d < SPM)
                                        return true; //indica que si colisiona
                        }
                }
        }

        return false; //indica que no colisiona
}

//determina si un punto está en el interior del contorno
bool TContourFigureList::IsInner(TDoublePoint P) const
{
        //ADVERTENCIA: si el contorno no delimita un conjunto convexo,
        //no basta con determinar si el punto está al mismo lado de
        //todas las figuras geométricas que lo componen.

        //si la lista de figuras no es un contorno ordenado
        if(!IsAContourSorted())
                //indica que no puede estar dentro por que
                //la lista de figuras no tiene lado interior
                return false;

        //NOTA: para ser un contorno ordenado, debrá haber al menos dos figuras
        //en la lista, ya que un único segmento no tiene lado interior y
        //un único arco tampoco, por que no puede tener una vuelta completa,
        //ya que si sus vértices coinciden se entiende que es un arco de
        //cero radianes.

        //DETERMINA SI EL PUNTO ESTÁ SOBRE LOS VÉRTICES
        //Y SI ES ASÍ DEVUELVE true:

        TContourFigure *F; //puntero a la figura indicada

        //por cada figura del contorno
        for(int i=0; i<getCount(); i++) {
                //apunta la figura indicada para facilitar su acceso
                F = Items[i];

                //si el punto está sobre los vértices
                if(P == F->getPa() || P == F->getPb())
                        //indica que el punto está en la figura
                        return true;
        }

        //RECORRE LAS FIGURAR CONTABILIZANDO EL DESPLAZAMIENTO ANGULAR:

        //bandera indicadora de si el vértice de la figura indicada
        //conectado a la próxima figura es el b
        bool PbIsNext;
        double acum_theta = 0; //acumulador del ángulo de desplazamiento
        TContourFigure *Fnext; //puntero a la figura próxima a la indicada

        //por cada figura adicional de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta la figura indicada para facilitar su acceso
                F = Items[i];
                //puntero a la figura siguiente a la indicada
                Fnext = Items[(i+1)%getCount()];

                //si el vértice Pb de la figura indicada es igual a
                //alguno de los vértices de la próxima figura
                if(F->getPb()==Fnext->getPa() || F->getPb()==Fnext->getPb())
                        //indica que el vértice conectado a la próxima figura es el b
                        PbIsNext = true;
                //si no, si el vértice Pa de la figura indicada es igual a
                //alguno de los vértices de la próxima figura
                else if(F->getPa()==Fnext->getPa() || F->getPa()==Fnext->getPb())
                        //indica que el vértice conectado a la próxima figura es el a
                        PbIsNext = false;
                else {
                        //inidica que se ha producido un efecto lateral por que
                        //debería haber un vértice de la figura indicada conectado a la próxima figura
                        throw EImposibleError("should be a vertext in the actual figure connect to next figure");
                }

                //contabiliza el ángulo recorrido para desplazarse
                //del vértice actual al próximo vértice de la figura F
                //en torno de P
                acum_theta += Rotation(F, PbIsNext, P);
        }

        //si ha dado una vuelta compelta para volver al punto inicial
        if(fabs(acum_theta) >= M_PI)
                return true; //inica que está dentro del contorno

        //Nótese que el umbral para determinar si ha vuelto al mismo sitio
        //o ha dado una vueltacompleta se pone en la mitad.

        //indica que el punto no está en el interior del contorno
        return false;
}
/*//---------------------------------------------------------------------------
//MÉTODOS PARA EL CÁLCULO DE ÁNGULOS DE GIRO:

//determina los ángulos que hay que rotar este contorno
//en torno al punto Q para que quede adyacente al segmento (Pa, Pb)
void TContourFigureList::TurnSegment(TVector<double> &dts,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Q)
{
        //determina la distancia mínima entre el segmento y el brazo
        double da = Mod(Pa - Q);
        double db = Mod(Pb - Q);
        double d = Min(Pa, Pb);
        //si no hay colisión
        if(d-Ra >= SPM) {
                dts.Clear(); //borra todas las soluciones
                return; //no hace nada más
        }

        //vector de soluciones auxiliar
        TVector<double> solutions;

        //arco (PB, PC, P2, R2)
        TurnArcSegment(solutions, PB, PC, P2, R2, Pa, Pb, Q);

        //arco (PC, PF, P1, Ra);
        TurnArcSegment(solutions, PC, PF, P1, Ra, Pa, Pb, Q);

        //arco (PF, PG, P2, R2)
        TurnArcSegment(solutions, PF, PG, P2, R2, Pa, Pb, Q);

        //-----------------

        //segmento (PA, PB)
        TurnSegmentSegment(solutions, PA, PB, Pa, Pb, Q);

        //segmento (PG, PH)
        TurnSegmentSegment(solutions, PG, PH, Pa, Pb, Q);

        //-----------------

        //arco (PH, PI, P1, R1)
        TurnArcSegment(solutions, PH, PI, P1, R1, Pa, Pb, Q);

        //arco (PI, PJ, P2, Rb)
        TurnArcSegment(solutions, PI, PJ, P2, Rb, Pa, Pb, Q);

        //arco (PJ, PA, P1, R1)
        TurnArcSegment(solutions, PJ, PA, P1, R1, Pa, Pb, Q);

        //añade las soluciones mínima y máxima
        if(solutions.Count > 0) {
                double dtmin = solutions.Min;
                if(dts.Search(dtmin) >= dts.Count)
                        dts.Add(dtmin);
        }
        if(solutions.Count > 1) {
                double dtmax = solutions.Max;
                if(dts.Search(dtmax) >= dts.Count)
                        dts.Add(dtmax);
        }
}

//determina los ángulos que hay que rotar este contorno
//en torno al punto Q para que quede adyacente al arco (Pa, Pb, Pc, R)
void TContourFigureList::TurnArc(TVector<double> &dts,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R,
        TDoublePoint Q)
{
        //determina la distancia mínima entre el segmento y el brazo
        double d = Mod(Pc - Q) - R;
        //si no hay colisión
        if(d-Ra >= SPM) {
                dts.Clear(); //borra todas las soluciones
                return; //no hace nada más
        }

        //vector de soluciones auxiliar
        TVector<double> solutions;

        //arco (PB, PC, P2, R2)
        TurnArcArc(solutions, PB, PC, P2, R2, Pa, Pb, Pc, R, Q);

        //arco (PC, PF, P1, Ra);
        TurnArcArc(solutions, PC, PF, P1, Ra, Pa, Pb, Pc, R, Q);

        //arco (PF, PG, P2, R2)
        TurnArcArc(solutions, PF, PG, P2, R2, Pa, Pb, Pc, R, Q);

        //-----------------

        //segmento (PA, PB)
        TurnSegmentArc(solutions, PA, PB, Pa, Pb, Pc, R, Q);

        //segmento (PG, PH)
        TurnSegmentArc(solutions, PG, PH, Pa, Pb, Pc, R, Q);

        //-----------------

        //arco (PH, PI, P1, R1)
        TurnArcArc(solutions, PH, PI, P1, R1, Pa, Pb, Pc, R, Q);

        //arco (PI, PJ, P2, Rb)
        TurnArcArc(solutions, PI, PJ, P2, Rb, Pa, Pb, Pc, R, Q);

        //arco (PJ, PA, P1, R1)
        TurnArcArc(solutions, PJ, PA, P1, R1, Pa, Pb, Pc, R, Q);

        //añade las soluciones mínima y máxima
        if(solutions.Count > 0) {
                double dtmin = solutions.Min;
                if(dts.Search(dtmin) >= dts.Count)
                        dts.Add(dtmin);
        }
        if(solutions.Count > 1) {
                double dtmax = solutions.Max;
                if(dts.Search(dtmax) >= dts.Count)
                        dts.Add(dtmax);
        }
}

//determina los ángulos que hay que rotar este contorno
//en torno al punto Q para que quede adyacente al brazo Arm
void TContourFigureList::TurnArm(TVector<double> &dts, TContourFigureList *Arm_, TDoublePoint Q)
{
        //determina la distancia mínima entre el segmento y el brazo
        double d = Mod(Arm->P1 - Q) - Arm->Ra;
        //si no hay colisión
        if(d-Ra >= SPM) {
                dts.Clear(); //borra todas las soluciones
                return; //no hace nada más
        }

        //apunta el brazo con un puntero específico
        TContourFigureList *Arm = (TContourFigureList*)Arm_;

        //vector de soluciones auxiliar
        TVector<double> solutions;

        //------------------
        //CABEZAL:

        //arco (Arm->PB, Arm->PC, Arm->P2, Arm->R2)
        TurnArc(solutions, Arm->PB, Arm->PC, Arm->P2, Arm->R2, Q);

        //arco (Arm->PC, Arm->PF, Arm->P1, Arm->Ra)
        TurnArc(solutions, Arm->PC, Arm->PF, Arm->P1, Arm->Ra, Q);

        //arco (Arm->PF, Arm->PG, Arm->P2, Arm->R2)
        TurnArc(solutions, Arm->PF, Arm->PG, Arm->P2, Arm->R2, Q);

        //------------------
        //BRAZO:

        //segmento (Arm->PA, Arm->PB)
        TurnSegment(solutions, Arm->PA, Arm->PB, Q);

        //segmento (Arm->PG, Arm->PH)
        TurnSegment(solutions, Arm->PG, Arm->PH, Q);

        //------------------
        //CODO:

        //arco (Arm->PH, Arm->PI, Arm->P1, Arm->R1)
        TurnArc(solutions, Arm->PH, Arm->PI, Arm->P1, Arm->R1, Q);

        //arco (Arm->PI, Arm->PJ, Arm->P2, Arm->Rb)
        TurnArc(solutions, Arm->PI, Arm->PJ, Arm->P2, Arm->Rb, Q);

        //arco (Arm->PJ, Arm->PA, Arm->P1, Arm->R1)
        TurnArc(solutions, Arm->PJ, Arm->PA, Arm->P1, Arm->R1, Q);
//AnsiString aux = solutions.Text;
        //------------------

        //si ha encontrado alguna solución
        if(solutions.Count > 0) {
                //busca las soluciones mínima y máxima
                double dtmin = MAXDOUBLE;
                double dtmax = -MAXDOUBLE;
                double dt;
                for(int i=0; i<solutions.Count; i++) {
                        //asigna el desplazameinto angular para facilitar el acceso
                        dt = solutions[i];
                        //actualiza el mínimo y el máximo
                        if(dt < dtmin)
                                dtmin = dt;
                        if(dt > dtmax)
                                dtmax = dt;
                }

                //añade una corrección del margen de error a las soluciones
                dtmin = dtmin + 0.000000000001*Sign(dtmin);
                dtmax = dtmax + 0.000000000001*Sign(dtmax);

                //si no está en la lista
                if(dts.Search(dtmin) >= dts.Count)
                        //añade la solución mínima
                        dts.Add(dtmin);

                //si no está en la lista
                if(dts.Search(dtmax) >= dts.Count)
                        //añade la solución máxima
                        dts.Add(dtmax);
        }
**//*
        //añade las soluciones mínima y máxima
        if(solutions.Count > 0) {
                double dtmin = solutions.Min;
                if(dts.Search(dtmin) >= dts.Count)
                        dts.Add(dtmin);
        }
        if(solutions.Count > 1) {
                double dtmax = solutions.Max;
                if(dts.Search(dtmax) >= dts.Count)
                        dts.Add(dtmax);
        }*//**
}
**/
//--------------------------------------------------------------------------
//MÉTODOS DE TRANSFORMACIONES GEOMÉTRICAS:

//obtiene la lista de figuras rotada y trasladada.
//si elnúmero de figuras de la lista no coincide:
//      lanza EImproperArgument
//si alguna figuradelalista no es del mimo tipo
//      lanza EImproperArgument
void TContourFigureList::GetRotatedAndTranslated(TContourFigureList &Contour,
        double theta, TDoublePoint V) const
{
        //el número de figuras de la lista debe ser igual
        if(Contour.getCount() != getCount())
                throw EImposibleError("number figures of thelist should be equal");

        //el tipo de figuras dela lista debe coincidir
        for(int i=0; i<Contour.getCount(); i++)
                if(typeid(Contour[i]) != typeid(Items[i]))
                        throw EImposibleError("types figures in thelist should match");
//if(IsAContourSorted())
  //      int aux = 0;
        //rota y traslada cada figura del contorno
        TFigure *F;
        for(int i=0; i<getCount(); i++) { //por cada figura geométrica
                //apunta la figura geométrica indicada para facilitar su acceso
                F = Contour[i];
                //obtiene la figura geométrica rotada
                Items[i]->GetRotated(F, theta);
                //traslada la figura geométrica según el vector V
                F->Translate(V);
        }
//if(Contour.IsAContourSorted())
  //      int xxx = 0;
}

//MÉTODOS GRÁFICOS:
/*#
//dibuja las figuras de una  lista
//en un trazador de formas
void TContourFigureList::Paint(TPloterShapes *PS) const
{
        //el puntero PS debería apuntar a un trazador de formas construido
        if(PS == NULL)
                throw EImproperArgument("pointer PS should point to built ploter shapes");

        //por cada puntero de la lista
        for(int i=0; i<getCount(); i++) {
                //asigna el puntero indicado para facilitar su acceso
                TFigure *F = Items[i];
                //si el puntero apunta a una figura construida
                if(F != NULL) {
                        //configura el color de la pluma
                        PS->setPenColor(F->Color);

                        //si la figura de contorno es del tipo segmento
                        if(typeid(*F) == typeid(TSegment)) {
                                //apunta lafigura con un puntero de la clase
                                TSegment *S = (TSegment*)F;
                                PS->Segment(S->getPa(), S->getPb());
                        }
                        //si no, si la figura de contorno es del tipo arco
                        else if(typeid(*F) == typeid(TArc)) {
                                //apunta lafigura con un puntero de la clase
                                TArc *A = (TArc*)F;
                                PS->Arc(A->getPa(), A->getPb(), A->getPc(), A->getR());
                        }
                }
        }
}
*/
//--------------------------------------------------------------------------
//FUNCIONES RELACIONADAS:

/*//segrega las figuras de esta lista en listas de figuras engarzadas
//devuelve falso si encuentra más de dos vértices engarzados entre si
bool Segregate(TPointersList<TContourFigureList> &LC, TContourFigureList &C)
{
        //puntero a la lista de figuras indicada de LC
        TContourFigureList *C;
        //puntero a la figura a insertar
        TFigure *F;

        //por cada figura de la lista
        for(int i=0; i<Count; i++) {
                //apunta la figura indicada para cafilitar su acceso
                F = Items[i];

                //NOTA: la figura indicada puede insertarse delante o detrás de
                //la primera figura con la que se encuentra engarzada.

                //busca la primera posición de inserción en CL
                int k=0; bool notfound=true;
                while(k<LC.Count && notfound) {
                        //apunta el contorno indicado para facilitar su acceso
                        C = LC.GetPointer(k);
                        //busca la primera posición de inserción en C1
                        int l=0;
                        while(l<C->Count && notfound) {
                                //si la figura a insertar está engarzada con la figura indicada
                                if(AreCrimped(F, C->Get(l))) {
                                        //si la figura indicada está engarzada con la figura previa
                                        if(l>0 && AreCrimped(C->Get(l), C->Get(l-1))) {
                                                //si la figura indicada está engarzada con la figura posterior
                                                if(l+1<C->Count && AreCrimped(C1->Get(l), C->Get(l-1))) {
                                                        //libera las listas de figuras (las figuras no deben ser liberadas por que son las de este contorno)
                                                        LC.Clear();
                                                        //indica que no es un contorno
                                                        return false;
                                                } else { //si no
                                                        //inserta la figura en la posición posterior
                                                        C1->Insert(l+1, F);
                                                        //indica que ha encontrado la posición
                                                        notfound = false;
                                                }
                                        } else { //si no
                                                //inserta la figura en la posición previa
                                                C1->Insert(l, F);
                                                //indica que ha encontrado la posición
                                                notfound = false;
                                        }
                                }
                                //indica a la próxima posición de C1
                                l++;
                        }
                        //indica a la próxima posicion de LC
                        k++;
                }

                //si no ha encontrado una posición de inserción en la que insertar la figura
                if(notfound) {
                        //construye una lista nueva
                        C1 = new TContourFigureList();
                        //añade la figura a la lista
                        C1->Add(F);
                        //añade la lista a la lista de listas
                        LC.Add(C1);
                }
        }

        //indica que no hay más de dos ve´rtices engarzados entre si
        return true;
}
//reduce un conjunto de listas de figuras concatenadas
//concatenando las listas que se puedan entre si
void Concatenate(TPointersList<TContourFigureList> &LC)
{
        //NOTA: no es posible saber cual es el sentido dextrógiro de ordenación
        //de cada lista de figuras, hasta que no se tenga el conjunto de listas
        //de figuras más reducido posible.

        //puntero a la lista indicada de LC
        TContourFigureList *C1;
        //puntero a la lista de figuras posterior a la indicada de LC
        TContourFigureList *C2;

        //por cada lista de figuras
        for(int i=0; i<LC.Count; i++) {
                //apunta la lista de figuras indicada para facilitar su acceso
                C1 = LC.GetPointer(i);

                //por cada lista de figuras posterior
                for(int j=i+1; j<LC.Count; j++) {
                        //apunta la lista de figuras indicada para facilitar su acceso
                        C2 = LC.GetPointer(j);

                        //si la última figura de C1 está crimpada con la primera figura de C2
                        if(AreCrimped(C1->Last, C2->First)) {
                                //concatena C2 al final de C1 manteniendo el orden
                                for(int k=0; k<C2->Count; k++)
                                        C1->Add(C2->Get(k));

                                //destruye la lista C2
                                LC.Delete(j);
                        //si no, si la última figura de C1 está crimpada con la última figura de C2
                        } else if(AreCrimped(C1->Last, C2->First)) {
                                //concatena C2 al final de C1 invirtiendo el orden
                                for(int k=C2->Count-1; k>=0; k--)
                                        C1->Add(C2->Get(k));

                                //destruye la lista C2
                                LC.Delete(j);
                        }
                }
        }
}          */

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

