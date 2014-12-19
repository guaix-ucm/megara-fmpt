//---------------------------------------------------------------------------
//Archivo: Geometry.cpp
//Contenido: clases y funciones geométricas
//Última actualización: 19/10/2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------


#include "Geometry.h"
#include "Constants.h"
#include "Scalars.h"
#include "Vector.h"
#include "Exceptions.h"

//---------------------------------------------------------------------------

//espacio de nombres de clases y funciones matemáticas
namespace Mathematics {

//---------------------------------------------------------------------------
//Funciones geométricas bidimensionales:
//---------------------------------------------------------------------------

//Obtiene los factores A, B y C de la ecuación canónica
//a partir de dos puntos de la recta.
//Los factores se obtienen noramlizados, de forma que C = -d donde d es
//la distancia del origen de coordenadas a la recta.
void CanonicalEquation(const TDoublePoint& p1, const TDoublePoint& p2,
        double&A, double&B, double&C)
{
    //p1 debe ser distinto de p2
        if (p1.x==p2.x && p1.y==p2.y)
            throw EImproperArgument("p1 should be unequal p2");

    //Los factores se obtienen noramlizados, de forma que C = -d donde d es
        //la distancia del origen de coordenadas a la recta
        double aux = pow(p1.x - p2.x, double(2)) + pow(p1.y - p2.y, double(2));
        A = Sign(p1.y - p2.y) * sqrt(pow(p1.y - p2.y, double(2))/aux);
        B = Sign(p2.x - p1.x) * sqrt(pow(p2.x - p1.x, double(2))/aux);
    C = - p1.x*A - p1.y*B;
}

//Normaliza los parámetros (A, B, C) de tal modo que el vector (A, B)
//se transforma en versor
void Normalize(double& A, double& B, double& C)
{
    double mod = sqrt (A*A + B*B);
    A /= mod;
    B /= mod;
    C /= mod;
}

//---------------------------------------------------------------------------
//INTERSECCIÓN ENTRE FIGURAS ENTERAS:

//Determina si las rectas (A1, B1, C1) y (A2, B2, C2) se intersecan, en cuyo
//caso devuelve el punto de intersección r.
//Parametros de entrada:
//	A1, B1, C1: parámetros de la recta	1
//	A2, B2, C2: parámetros de la recta	2
//Valores de retorno:
//	Intersection: true: se intersecan; false: no se intersecan
//	r: punto de intersección en caso de haberlo
bool Intersection(TDoublePoint& r,
        double A1, double B1, double C1,
        double A2, double B2, double C2)
{
    //Los vectores normales a las rectas deben tener módulo superior a cero
        if ((A1==0 && B1==0) || (A2==0 && B2==0))
            throw EImproperArgument(
                        "(A1, B1) and (A2, B2) dont should be null vectors");

        //Si el denominador de la matriz {{A1, B1}, {A2, B2}} es igual a cero
        //las rectas son paralelas y no se intersecan
        double denom = A1*B2 - B1*A2;
        if (denom==0)
            return false;

        //si no son paralelos
        r.x = (B1*C2 - C1*B2)/denom;
        r.y = (C1*A2 - A1*C2)/denom;

        return true;
}

//determina si el segmento clipeante (q1, q2) interseca al segmento a
//clipear (p1, p2), en cuyo caso devuelve el punto de itnersección r.
//Esta función también devuelve la posición relativa a la recta que
//contiene el segmento clipeante de los puntos del segmento a clipear.
//Parámetros de paso:
//	p1, p2: vértices del segmento a clipear
//	q1, q2: vértices del segmento a clipeante
//Parámetros devueltos:
//	Intersectión: true si los segmentos se intersecan
//	r: punto de intersección en caso de haberlo
//	s: posición relativa de los vñertices p1 y p2 a la recta contiente
//		del segmento clipeante:
//			-2: p1 y p2 ambos a la dcha
//			-1: p1 a la izda y p2 a la dcha
//			0: p1 y p2 sobre la recta continente
//			1: p2 a la izda y p1 a la dcha
//			2: p1 y p2 ambos a la izda
bool Intersection(const TDoublePoint& p1, const TDoublePoint& p2,
        const TDoublePoint& q1, const TDoublePoint& q2,
        TDoublePoint& r, char& s)
{
    double qA, qB, qC; //recta contienente del segmento (p1, p2)
        CanonicalEquation(q1, q2, qA, qB, qC);
    double pA, pB, pC; //recta continente del segmento (q1, q2)
    CanonicalEquation(p1, p2, pA, pB, pC);

        //distancia absoluta de los puntos p1 y p2 a la recta continente
        //del segmento (q1, q2)
    double d1 = Distance(p1, qA, qB, qC);
    double d2 = Distance(p2, qA, qB, qC);

        //posición relativa de los puntos p1 y p2 a la recta continente
        //del segmento (q1, q2)
        if(d1==0 && d2==0)
            s = 0;
        else if(d1>0 && d2<0)
            s = - 1;
        else if(d1<0 && d2>0)
            s = 1;
        else if(d1>=0 && d2>=0)
            s = 2;
        else if(d1<=0 && d2<=0)
                s = -2;

        //si las rectas son paralelas no se intersecan y solo debe determinar
        //la posición relativa de los puntos
        if(d1==d2)
                return false;

        //si hay un punto a cada lado de la recta continente de (q1, q2),
        //debe calcularse el punto de intersección
    Intersection (r, pA, pB, pC, qA, qB, qC);

        //El punto de intersección r pertenecerá al segmento (q1, q2), solo si,
        //almenos una de sus coordenadas se encuentra entre las coordenadas
        //homónimas de q1 y q2. Lo mismo puede decirse del segmento p.
        //Para evitar errores de desprecisión, se observará la coordenada
        //cuyas coordenadas homónimas estén más distantes

        //Comprueba la pertenencia de r al segmento (p1, p2)
        bool belongp=false; //flags de pertenencia de r al segmento (p1, p2)
    if(fabs(p2.x - p1.x)>fabs(p2.y - p1.y)) {
            if(Min(p1.x, p2.x)<=r.x && r.x<=Max(p2.x, p1.x))
                    belongp = true;
        } else
                if(Min(p1.y, p2.y)<=r.y && r.y<=Max(p2.y, p1.y))
                    belongp = true;

        //Comprueba la pertenencia de r al segmento (q1, q2)
        bool belongq=false; //flags de pertenencia de r al segmento (q1, q2)
        if(fabs(q2.x - q1.x)>fabs(q2.y - q1.y)) {
        if(Min(q1.x, q2.x)<=r.x && r.x<=Max(q2.x, q1.x))
            belongq = true;
        } else
            if(Min(q1.y, q2.y)<=r.y && r.y<=Max(q2.y, q1.y))
                    belongq = true;

        //Si pertenece a ambos segmentos esque se intersecan
        if(belongp && belongq)
        return true;
    else
            return false;
}

//calcula los puntos de intersección (P1, P2)
//de la recta (A, B, C) con la circunferencia (Pc, R)
//P1 y P2 son el primer y segundo punto en el sentido de orientación de la recta
//si no hay intersección devuelve falso
bool IntersectionLineCircum(TDoublePoint &P1, TDoublePoint &P2,
        double A, double B, double C, TDoublePoint Pc, double R)
{
        //calcula el módulo del vector normal de la recta
        double mod = sqrt(A*A + B*B);

        //calcula el versor normal de la recta
        TDoublePoint n;
        n.x = A/mod;
        n.y = B/mod;

        //calcula la distancia desde el origen de coordenadas hasta la recta
        //en el sentido de n
        double d = -C/mod;

        //calcula la distancia desde el centro de la circunferencia
        //hasta la recta en la dirección y sentido de n
        double D = d - Dot(Pc, n);

        if(fabs(D) > R) //si la recta está más alejada de Pc que el radio
                return false; //indica que no hay intersección
        else if(fabs(D) == R) { //si la recta es tangente en el sentido de n
                P2 = P1 = Pc + D*n; //calcula el punto de intersección
                //indica que ha encontrado el punto de intersección
                return true;
        }
        else { //si la recta es secante (incluyendo cuando pasa por el centro)
                //calcula el versor dextrógiro de n
                TDoublePoint m = -Cross(n);
                //calcula la distancia desde el centro de la recta
                //hastala circunferencia
                double S = R*sqrt(1 - pow(D/R, 2.));
                //calcula (P1, P2) en el sentido de PA a PB
                P1 = Pc + D*n - S*m;
                P2 = Pc + D*n + S*m;
                //indica que ha encontrado los puntos de intersección
                return true;
        }
}
//calcula los puntos de intersección (P1, P2)
//de la recta que pasa por los puntos (Pa, Pb) con la circunferencia (Pc, R)
//P1 y P2 son el primer y segundo punto en el sentido de orientación de la recta
//si no hay intersección devuelve falso
bool IntersectionLineCircum(TDoublePoint &P1, TDoublePoint &P2,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R)
{
        //los puntos de la recta no deben ser coincidentes
        if(Pa == Pb)
                throw EImproperArgument("Pa should be unequal Pb");

        //se advierte que el radio R puede ser igual a 0

        //calcula el versor dirigido de Pa a Pb
        TDoublePoint m = Pb - Pa;
        m = m/Mod(m);

        //obtiene el versor levógiro de m
        TDoublePoint n = Cross(m);

        //calcula la distancia del centro a la recta
        double D = Dot((Pa - Pc), n);

        if(fabs(D) > R) //si la recta está más alejada de Pc que el radio
                return false; //indica que no hay intersección
        else if(fabs(D) == R) { //si la recta es tangente en el sentido de n
                P2 = P1 = Pc + D*n; //calcula el punto de intersección
                //indica que ha encontrado el punto de intersección
                return true;
        }
        else { //si la recta es secante (incluyendo cuando pasa por el centro)
                //esta situación solo puede darse cuando R > 0

                //calcula la distancia desde el centro de la recta
                //hastala circunferencia
                double S = R*sqrt(1 - pow(D/R, 2.));
                //calcula (P1, P2) en el sentido de PA a PB
                P1 = Pc + D*n - S*m;
                P2 = Pc + D*n + S*m;
                //indica que ha encontrado los puntos de intersección
                return true;
        }
}
bool IntersectionCircumLine(TDoublePoint &P1, TDoublePoint &P2,
        TDoublePoint Pc, double R, TDoublePoint Pa, TDoublePoint Pb)
{
    return IntersectionLineCircum(P1, P2, Pa, Pb, Pc, R);
}
//determina la posición relativa de
//las circunferencias (Pc1, R1) y (Pc2, R2), y
//calcula los puntos de intersección (P1, P2) en caso de haberlos
//P1 y P2 son los puntos superior e inferior
//cuando la circunferencia mayor está a la izquierda
//devuelve:
//      0: exteriores
//      1: coincidentes
//      2: interiores concéntricas
//      3: interiores excéntricas
//      4: tangentes interiormente
//      5: tangentes exteriormente
//      6: secantes
int IntersectionCircumCircum(TDoublePoint &P1, TDoublePoint &P2,
        TDoublePoint Pc1, double R1, TDoublePoint Pc2, double R2)
{
        //el radio R1 debe ser no negativo
        if(R1 < 0)
                throw EImproperArgument("radio R1 should be upper cero");

        //el radio R2 debe ser no negativo
        if(R2 < 0)
                throw EImproperArgument("radio R2 should be upper cero");

        //si el radio R1 es cero
        if(R1 == 0) {
                //si Pc1 está a una distancia R2 de Pc2
                if(Mod(Pc1 - Pc2) == R2) {
                        //los puntos de intersección coinciden en Pc1
                        P2 = P1 = Pc1;
                        //indica que son secantes
                        return 6;
                }
        }

        //si el radio R2 es cero
        if(R2 == 0) {
                //si Pc2 está a una distancia R1 de Pc1
                if(Mod(Pc2 - Pc1) == R1) {
                        //los puntos de intersección coinciden en Pc2
                        P2 = P1 = Pc2;
                        //indica que son secantes
                        return 6;
                }
        }

        double Rmin; //radio de la circunferecnia menor
        double Rmax; //radio de la circunferencia mayor
        TDoublePoint Pcmax; //centro de la circunferencia mayor
        TDoublePoint V; //vector de la circunferencia mayor a la menor
        if(R1 >= R2) { //si la circunferencia 1 es la mayor
                Rmin = R2;
                Rmax = R1;
                Pcmax = Pc1;
                V = Pc2 - Pc1;
        }
        else { //si la circunferencia 2 es la mayor
                Rmin = R1;
                Rmax = R2;
                Pcmax = Pc2;
                V = Pc1 - Pc2;
        }

        //límites del intervalo en el que hay intersección
        double Dmin = Rmax - Rmin;
        double Dmax = R1 + R2;

        double D = Mod(V); //distancia entre centros

        if(D > Dmax)
                return 0; //exteriores
        else if(D == 0) {
                if(R1 == R2)
                        return 1; //coincidentes
                else
                        return 2; //interiores concéntricas
        }
        else if(0<D && D<Dmin) {
                return 3; //interiores excéntricas
        }
        else if(D == Dmin) {
                P2 = P1 = Pcmax + V/D*Rmax;
                return 4; //tangentes interiormente
        }
        else if(D == Dmax) {
                P2 = P1 = Pcmax + V/D*Rmax;
                return 5; //tangentes exteriormente
        }
        else { //Dmin<D && D<Dmax
                double p = (Dmax + D)/2; //semiperímetro del triángulo
                double A = sqrt(p*(p-Rmin)*(p-Rmax)*(p-D)); //área del triángulo
                double H = A*2/D; //altura del triángulo
                if(H > Rmin) { //si la altura supera la máxima teórica
                        if(R1==R2) //si sin de igual rádio
                                return 1; //las toma como coincidentes
                        else //si son de distinto radio
                                H = Min(H, Rmin); //corrige el error numérico
                }
                double D1 = Rmax*sqrt(1 - pow(H/Rmax, 2.));

                TDoublePoint m = V/D; //versor de Cmax a Cmin
                TDoublePoint n = Cross(m); //versor levógiro de m

                P1 = Pcmax + D1*m + H*n;
                P2 = Pcmax + D1*m - H*n;

                return 6; //secantes
        }

        //ERROR: cuando Rmin==Rmax y D tiende a cero,
        //A H tiende a superar el radiod e las circunferencias.
}

//---------------------------------------------------------------------------
//INTERSECCIÓN ENTRE PARTES DE FIGURAS:

//determina si hay intersección entre los segmentos (Pa, Pb), (Qa, Qb)
//si los segmentos se intersecan calcula el punto de intersección P
//si los segmentos no se intersecan devuelve falso
bool IntersectionSegmentSegment(TDoublePoint &P,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Qa, TDoublePoint Qb)
{
        double denom = Det(Pa-Pb, Qa-Qb);

        if(denom == 0) //si el determinante es cero
                return false; //indica que las rectas son paralelas
        else { //si el determinante es distinto de cero
                //calcula los coeficientes (a1, b1)
                double a1 = -Det(Pb-Qb, Qa-Qb)/denom;
                double b1 = Det(Qb-Pb, Pa-Pb)/denom;
/*                double b1 = ((Qb.y-Pb.y)*(Pa.x-Pb.x) - (Qb.x-Pb.x)*(Pa.y-Pb.y))/
                        denom;
                double a1 = (b1*(Qa.x-Qb.x)+Qb.x-Pb.x)/(Pa.x-Pb.x);*/

                if(b1<=0 || 1<=b1 || a1<=0 || 1<=a1) //si los segmentos no se cortan
                        return false; //indica que no se intersecan
                else { //si los segmentos se cortan
                        P = a1*Pa + (1-a1)*Pb; //calcula el punto
                        return true; //indica que se intersecan en un punto
                }
        }
}

//determina si hay intersección entre el segmento (Pa, Pb) y
//el círculo (Pc, R)
//si no hay intersección devuelve falso
bool IntersectionSegmentCircle(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Pc, double R)
{
        //si alguno de los puntos está a una distancia R de Pc, o menor
        if(DistanceSegmentPoint(Pa, Pb, Pc) <= R)
                return true; //indica que hay intersección

        return false; //indica que no hay intersección
}

//determina si hay intersección entre el segmento (Pa, Pb) y
//el arco de circunferencia (Pc, R, Pfin, Pini) en sentido levógiro
//si no hay intersección devuelve falso
bool IntersectionSegmentArc(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Pfin, TDoublePoint Pini, TDoublePoint Pc, double R)
{
        //los puntos del segmento no deben coincidir
        if(Pa == Pb)
                throw EImproperArgument("Pa should be unequal Pb");
        //el radio de la circunferecnia debe ser mayor que cero
        if(R <= 0)
                throw EImproperArgument("R ahould be upper zero");
        //los puntos de las cuerdas del arco no deben
        //coincidir con el centro de la circunferencia
        if(Pfin == Pc)
                throw EImproperArgument("Pfin should be unequal Pc");
        if(Pini == Pc)
                throw EImproperArgument("Pini should be unequal Pc");

        //calcula los puntos de intersección
        TDoublePoint P1, P2;
        bool intersection = IntersectionLineCircum(P1, P2, Pa, Pb, Pc, R);

        if(!intersection) //si no llegan a tocarse
                return false; //indica que no hay intersección

        //determina si el punto P1 está en el segmento y en el arco
        if(PointIsInOpenStrip(P1, Pa, Pb) && PointIsInOpenAngle(P1, Pfin, Pini, Pc))
                return true;

        //determina si el punto P2 está en el segmento y en el arco
        if(PointIsInOpenStrip(P2, Pa, Pb) && PointIsInOpenAngle(P2, Pfin, Pini, Pc))
                return true;

        return false; //indica que no hay intersección
}
//determina si hay intersección entre el arco (Pfin, Pini, Pc, R) y
//el segmento (Pa, Pb) en sentido levógiro
//si no hay intersección devuelve falso
bool IntersectionArcSegment(TDoublePoint Pfin, TDoublePoint Pini,
        TDoublePoint Pv, double R, TDoublePoint Pa, TDoublePoint Pb)
{
    return IntersectionSegmentArc(Pa, Pb, Pfin, Pini, Pv, R);
}

//determina si hay intersección entre
//el círculo (Pc1, R1) y el círculo (Pc2, R2)
bool IntersectionCircleCircle(TDoublePoint Pc1, double R1,
        TDoublePoint Pc2, double R2)
{
        //si la distancia entre los centros es
        //menor o igual a la suma de los radios
        if(Mod(Pc2 - Pc1) <= R1+R2)
                return true; //indica que si hay intersección

        return false; //indica que no hay intersección
}

//determina si hay intersección entre
//el arco de circunferencia (Pa, Pb, Pc, R1)
//la circunferencia (Qc, R2)
bool IntersectionArcCircunference(
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R1,
        TDoublePoint Qc, double R2)
{
        //calcula los puntos de intersección entre las dos circunferencias
        TDoublePoint P1, P2;
        int relpos = IntersectionCircumCircum(P1, P2, Pc, R1, Qc, R2);

        if(relpos < 4) //si no llegan ni a atocarse
                return false; //indica que no hay intersección

        //significados de relpos:
        //      4: tangentes interiormente
        //      5: tangentes exteriormente
        //      6: secantes

        //si el punto P1 está en el arco
        if(PointIsInOpenAngle(P1, Pa, Pb, Pc))
                return true; //indica que si hay intersección

        if(relpos >= 6) //si son secantes, P2 será distinto de P1
                //si el punto P2 está en el arco
                if(PointIsInOpenAngle(P2, Pa, Pb, Pc))
                        return true; //indica que si hay intersección

        return false; //indica que no hay intersección
}
//determina si hay intersección entre
//el arco de circunferencia (Pc1, R1, Pfin1, Pini1) en sentido levógiro y
//el arco de circunferencia (Pc2, R2, Pfin2, Pini2) en sentido levógiro
//si no hay intersección devuelve falso
bool IntersectionArcArc(
        TDoublePoint Pfin1, TDoublePoint Pini1, TDoublePoint Pc1, double R1,
        TDoublePoint Pfin2, TDoublePoint Pini2, TDoublePoint Pc2, double R2)
{
        //calcula los puntos de intersección entre las dos circunferencias
        TDoublePoint P1, P2;
        int relpos = IntersectionCircumCircum(P1, P2, Pc1, R1, Pc2, R2);

        if(relpos < 4) //si no llegan ni a atocarse
                return false; //indica que no hay intersección

        //significados de relpos:
        //      4: tangentes interiormente
        //      5: tangentes exteriormente
        //      6: secantes

        //si el punto P1 está en los dos arcos
        if(PointIsInOpenAngle(P1, Pfin1, Pini1, Pc1) &&
                PointIsInOpenAngle(P1, Pfin2, Pini2, Pc2))
                return true; //indica que si hay intersección

        if(relpos >= 6) //si son secantes, P2 será distinto de P1
                //si el punto P2 está en los dos arcos
                if(PointIsInOpenAngle(P2, Pfin1, Pini1, Pc1) &&
                        PointIsInOpenAngle(P2, Pfin2, Pini2, Pc2))
                        return true; //indica que si hay intersección

        return false; //indica que no hay intersección
}

/*//---------------------------------------------------------------------------
//INTERSECCIÓN ENTRE CIRCUNFERENCIA Y LA FRONTERA DE FIGURAS:

//calcula los puntos de intersección de la circunferencia (P, R)
//con la forntera de seguridad (cuyo margen es sm) del segmento (Pa, Pb)
//puede haber 0, 1, 2, 3 ó 4 puntos de intersección
//los puntos son añadidos a la lista Points
void IntersectionCircumBorderlineSegment(TItemList<TDoublePoint> &Points,
        TDoublePoint P, double R,
        TDoublePoint Pa, TDoublePoint Pb,
        double sm)
{
        //el punto Pb no debe ser igual al punto Pa
        if(Pb == Pa)
                throw EImproperArgument("Pb should not be equal Pa");

        //calcula el versor director de la recta que pasa de Pa a Pb
        TDoublePoint m = Pb - Pa;
        m = m/Mod(m);
        //calcula el versor normal a la recta
        TDoublePoint n = Cross(m);

        //calcula el vector de salto desde los vértices
        TDoublePoint vn = sm*n;

        //calcula los puntos de intersección con la recta del lado positivo
        TDoublePoint P1, P2; //puntos de intersección
        if(IntersectionLineCircum(P1, P2, Pa+vn, Pb+vn, P, R)) {
                //añade los puntos que estén en la franja de Pa a Pb
                if(PointIsInStrip(P1, Pa, Pb))
                        Points.Add(P1);
                if(P2!=P1 && PointIsInStrip(P2, Pa, Pb))
                        Points.Add(P2);
        }

        //calcula los puntos de intersección con la recta del lado negativo
        if(IntersectionLineCircum(P1, P2, Pa-vn, Pb-vn, P, R)) {
                //añade los puntos que estén en la franja de Pa a Pb
                if(PointIsInStrip(P1, Pa, Pb))
                        Points.Add(P1);
                if(P2!=P1 && PointIsInStrip(P2, Pa, Pb))
                        Points.Add(P2);
        }

        //calcula los puntos de intersección con la circunferencia (Pa, sm)
        if(IntersectionCircumCircum(P1, P2, Pa, sm, P, R) >= 4) {
                //añade los puntos que estén en el arco y no estén apuntados
                if(!(Dot(P1, m)>0 || Points.Search(P1)<Points.Count))
                        Points.Add(P1);
                if(!(Dot(P2, m)>0 || Points.Search(P2)<Points.Count))
                        Points.Add(P2);

                //es preferible buscar los puntos en la lista
                //a arriesgarse a descartar algún punto por error numérico
        }

        //calcula los puntos de intersección con la circunferencia (Pb, sm)
        if(IntersectionCircumCircum(P1, P2, Pb, sm, P, R) >= 4) {
                //añade los puntos que estén en el arco y no estén apuntados
                if(!(Dot(P1, m)>0 || Points.Search(P1)<Points.Count))
                        Points.Add(P1);
                if(!(Dot(P2, m)>0 || Points.Search(P2)<Points.Count))
                        Points.Add(P2);

                //es preferible buscar los puntos en la lista
                //a arriesgarse a descartar algún punto por error numérico
        }
}

//calcula los puntos de intersección de la circunferencia (P, R)
//con la forntera de seguridad (cuyo margen es sm) del arco (Pa, Pb, Pc, Rp)
//puede haber 0, 1, 2, 3 ó 4 puntos de intersección
//los puntos son añadidos a la lista Points
void IntersectionCircumBorderlineArc(TItemList<TDoublePoint> &Points,
        TDoublePoint P, double R,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double Rp,
        double sm)
{
        //el punto Pb no debe ser igual al punto Pa
        if(Pb == Pa)
                throw EImproperArgument("Pb should not be equal Pa");

        //calcula los puntos de intersección con la circunferencia exterior
        TDoublePoint P1, P2; //puntos de intersección
        if(IntersectionCircumCircum(P1, P2, P, R, Pc, Rp+sm) >= 4) {
                //añade los puntos que estén en el ángulo (Pa, Pb, Pc)
                if(PointIsInAngle(P1, Pa, Pb, Pc))
                        Points.Add(P1);
                if(P2!=P1 && PointIsInAngle(P2, Pa, Pb, Pc))
                        Points.Add(P2);
        }

        //calcula los puntos de intersección con la circunferencia interior
        if(IntersectionCircumCircum(P1, P2, P, R, Pc, Rp-sm) >= 4) {
                //añade los puntos que estén en el ángulo (Pa, Pb, Pc)
                if(PointIsInAngle(P1, Pa, Pb, Pc))
                        Points.Add(P1);
                if(P2!=P1 && PointIsInAngle(P2, Pa, Pb, Pc))
                        Points.Add(P2);
        }

        //calcula los puntos de intersección con la circunferencia (Pa, sm)
        if(IntersectionCircumCircum(P1, P2, P, R, Pa, sm) >= 4) {
                //añade los puntos fuera del arco no apuntados
                if(PointIsInAngle(P1, Pb, Pa, Pc) && Points.Search(P1)>=Points.Count)
                        Points.Add(P1);
                if(PointIsInAngle(P2, Pb, Pa, Pc) && Points.Search(P2)>=Points.Count)
                        Points.Add(P2);

                //es preferible buscar los puntos en la lista
                //a arriesgarse a descartar algún punto por error numérico
        }

        //calcula los puntos de intersección con la circunferencia (Pb, sm)
        if(IntersectionCircumCircum(P1, P2, P, R, Pb, sm) >= 4) {
                //añade los puntos fuera del arco no apuntados
                if(PointIsInAngle(P1, Pb, Pa, Pc) && Points.Search(P1)>=Points.Count)
                        Points.Add(P1);
                if(PointIsInAngle(P2, Pb, Pa, Pc) && Points.Search(P2)>=Points.Count)
                        Points.Add(P2);

                //es preferible buscar los puntos en la lista
                //a arriesgarse a descartar algún punto por error numérico
        }
}

//---------------------------------------------------------------------------
//DESPLAZAMIENTOS ANGULARES DE FIGURAS INTERFERIDAS
//PARA DISTANCIARSE EL MARGEN DE SEGURIDAD:

//calcula el punto al que se desplaza el punto P
//cuando es rotado a rad en torno de Q
TDoublePoint Rotate(TDoublePoint P, TDoublePoint Q, double a)
{
        TDoublePoint V = P-Q;
        double r = Mod(V);
        double theta = Arg(V);

        //S0: sistema de coordenadas cartesianas en que es dado P y Q
        //S1: sistema de coordenadas cartesianas con origen en Q
        //    y eje de abcisas orientado hacia P

        //calcula el punto en coordenadas relativas a S1
        double R_x = r*cos(a);
        double R_y = r*sin(a);

        //transtribe el punto a S0
        TDoublePoint R;
        R.x = Q.x + R_x*cos(theta) - R_y*sin(theta);
        R.y = Q.x + R_x*sin(theta) + R_y*cos(theta);

        return R;
}

//calcula el ángulo que es necesario rotar el segmento interferido (Qa, Qb)
//para que quede a una distancia sm del segmento interferente (Pa, Pb)
//      a1: ángulo en sentido levógiro
//      a2: ángulo en sentido dextrógiro
//      Q: posición del eje de rotación
//ambos valores son positivos
void Rotation(double a1, double a2,
        TDoublePoint Pa_, TDoublePoint Pb_,
        TDoublePoint Qa_, TDoublePoint Qb_,
        TDoublePoint Q, double sm)
{
        //si no hay interferencia no busca nada
        if(DistanceSegmentSegment(Pa_, Pb_, Qa_, Qb_) >= sm) {
                a2 = a1 = 0; //indica que no tiene que girar
                return; //y no hace nada más
        }

        //a partir de aquí si hay interferencia

        //VARIABLES PARA EL SIGUIENTE PROCESO DE BÚSQUEDA:

        int i = 0; //índice a puntos de Points
        TDoublePoint Ra, Rb; //puntos del segmento (Pa, Pb) desplazado
        double a; //ángulo de depslazamiento para desplazar cada punto

        //BUSCA UN PUNTO SIN COLISIÓN EN LA ÓRBITA DE Pa:

        //calcula los puntos de intersección de la órbita descrita por Pa
        TItemList<TDoublePoint> Points;
        IntersectionCircumBorderlineSegment(Points, Q, Mod(Pa_-Q), Qa_, Qb_, sm);

        //desplaza el segmento (Pa, Pb) t.q. Pa = Points[i]
        //y si no hay intersección entre los segmentos registra la solución
        i = 0; //indica al primer punto
        do {
                //calcula el ángulo de desplazamiento
                a = Angle(Points[i]-Q, Pa_-Q);
                //desplaza el segmento al punto
                Ra = Rotate(Pa_, Q, a);
                Rb = Rotate(Pb_, Q, a);
                //indica al próximo punto
                i++;
        } while(DistanceSegmentSegment(Ra, Rb, Qa_, Qb_) < sm);
        //si se ha encontrado un punto sin colisión
        //registra el desplazamiento
        if(i <= Points.Count) {
                if(a < 0)
                        a1 = -a;
                else
                        a2 = -a;
        }

        //BUSCA UN PUNTO SIN COLISIÓN EN LA ÓRBITA DE Pb:

        //calcula los puntos de intersección de la órbita descrita por Pa
        Points.Clear();
        IntersectionCircumBorderlineSegment(Points, Q, Mod(Pb_-Q), Qa_, Qb_, sm);

        //desplaza el segmento (Pa, Pb) t.q. Pb = Points[i]
        //y si no hay intersección entre los segmentos registra la solución
        i = 0; //indica al primer punto
        do {
                //calcula el ángulo de desplazamiento
                a = Angle(Points[i]-Q, Pb_-Q);
                //desplaza el segmento al punto
                Ra = Rotate(Pa_, Q, a);
                Rb = Rotate(Pb_, Q, a);
                //indica al próximo punto
                i++;
        } while(DistanceSegmentSegment(Ra, Rb, Qa_, Qb_) < sm);
        //si se ha encontrado un punto sin colisión
        //registra el desplazamiento
        if(i <= Points.Count) {
                if(a < 0)
                        a1 = -a;
                else
                        a2 = -a;
        }

        //BUSCA UN PUNTO SIN COLISIÓN EN LA ÓRBITA DE Qa:

        //calcula los puntos de intersección de la órbita descrita por Qa
        Points.Clear();
        IntersectionCircumBorderlineSegment(Points, Q, Mod(Qa_-Q), Pa_, Pb_, sm);

        //desplaza el segmento (Qa, Qb) t.q. Qa = Points[i]
        //y si no hay intersección entre los segmentos registra la solución
        i = 0; //indica al primer punto
        do {
                //calcula el ángulo de desplazamiento
                a = Angle(Points[i]-Q, Qa_-Q);
                //desplaza el segmento al punto
                Ra = Rotate(Qa_, Q, a);
                Rb = Rotate(Qb_, Q, a);
                //indica al próximo punto
                i++;
        } while(DistanceSegmentSegment(Pa_, Pb_, Ra, Rb) < sm);
        //si se ha encontrado un punto sin colisión
        //registra el desplazamiento
        if(i <= Points.Count) {
                if(a >= 0)
                        a1 = a;
                else
                        a2 = a;
        }

        //BUSCA UN PUNTO SIN COLISIÓN EN LA ÓRBITA DE Qb:

        //calcula los puntos de intersección de la órbita descrita por Qb
        Points.Clear();
        IntersectionCircumBorderlineSegment(Points, Q, Mod(Qb_-Q), Pa_, Pb_, sm);

        //desplaza el segmento (Qa, Qb) t.q. Qb = Points[i]
        //y si no hay intersección entre los segmentos registra la solución
        i = 0; //indica al primer punto
        do {
                //calcula el ángulo de desplazamiento
                a = Angle(Points[i]-Q, Qb_-Q);
                //desplaza el segmento al punto
                Ra = Rotate(Qa_, Q, a);
                Rb = Rotate(Qb_, Q, a);
                //indica al próximo punto
                i++;
        } while(DistanceSegmentSegment(Pa_, Pb_, Ra, Rb) < sm);
        //si se ha encontrado un punto sin colisión
        //registra el desplazamiento
        if(i <= Points.Count) {
                if(a >= 0)
                        a1 = a;
                else
                        a2 = a;
        }
}

//calcula el ángulo que es necesario rotar el arco interferido (Qa, Qb, Qc, Rq)
//para que quede a una distancia sm del segmento interferente (Pa, Pb)
//      a1: ángulo en sentido levógiro
//      a2: ángulo en sentido dextrógiro
//      Q: posición del eje de rotación
//ambos valores son positivos
void Rotation(double a1, double a2,
        TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Qa, TDoublePoint Qb, TDoublePoint Qc, double Rq,
        TDoublePoint Q, double sm)
{
        //si la distancia ya mantiene el margen de seguridad
        if(DistanceSegmentArc(Pa, Pb, Qa, Qb, Qc, Rq) >= sm) {
                a2 = a1 = 0; //indica que no tiene que girar
                return; //y no hace nada más
        }

        //calcula el desplazamiento para que
}

//calcula el ángulo que es necesario rotar el arco interferido (Qa, Qb, Qc, Rq)
//para que quede a una distancia sm del arco interferente (Pa, Pb, Pc, Rp)
//      a1: ángulo en sentido levógiro
//      a2: ángulo en sentido dextrógiro
//      Q: posición del eje de rotación
//ambos valores son positivos
void Rotation(double a1, double a2,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double Rp,
        TDoublePoint Qa, TDoublePoint Qb, TDoublePoint Qc, double Rq,
        TDoublePoint Q, double sm)
{
        //si la distancia ya mantiene el margen de seguridad
        if(DistanceArcArc(Pa, Pb, Pc, Rp, Qa, Qb, Qc, Rq) >= sm) {
                a2 = a1 = 0; //indica que no tiene que girar
                return; //y no hace nada más
        }

        //calcula el desplazamiento para que
}
   */
//---------------------------------------------------------------------------
//POSICIONES RELATIVAS DE OBJETOS:

//determina si el punto P está en el subespacio comprendido
//entre las dos rectas (rectas incluidas) que pasan por Pa y Pb
//y son normales a la recta que pasa por Pa y Pb
bool PointIsInClosedStrip(TDoublePoint P, TDoublePoint Pa, TDoublePoint Pb)
{
        //los puntos no deben coincidir ya que deben definir una orientación
        if(Pa == Pb)
                throw EImproperArgument("Pa should be unequal Pb");

        //calcula el versor de PA a PB
        TDoublePoint m = Pb - Pa;
        m = m/Mod(m);

        //calcula la distancia desde el origen de la recta que contiene a Pa
        double da = Dot(Pa, m);

        //calcula la distancia desde el origen de la recta que contiene a Pb
        double db = Dot(Pb, m);

        //calcula la distancia desde el origen de la recta que contiene a P
        double d = Dot(P, m);

        if(d<da || db<d) //si R no está en el intervalo [Ra, Rb]
                return false; //indica que P no está en el intervalo [Ra, Rb]

        return true; //indica que P si está en el intervalo [Ra, Rb]
}
//determina si el punto P está en el ángulo (lados incluidos)
//de vértice Pv y cuyos lados pasan por Pini y Pfin en sentido levógiro
bool PointIsInClosedAngle(TDoublePoint P,
        TDoublePoint Pfin, TDoublePoint Pini, TDoublePoint Pv)
{
        //los puntos de los lados no deben coincidir con el vértice
        if(Pini == Pv)
                throw EImproperArgument("Pini should be unequal Pv");
        if(Pfin == Pv)
                throw EImproperArgument("Pfin should be unequal Pv");

        //si el punto está sobre el vértice
        if(P == Pv)
                return true; //indica que si está en el ángulo

        //Se recuerda que el dominio de la función ArgPos es [0, 2*PI).

        //determina el ángulo del lado inicial
        double thetaini = ArgPos(Pini.x-Pv.x, Pini.y-Pv.y);
        //determina el ángulo del lado final
        double thetafin = ArgPos(Pfin.x-Pv.x, Pfin.y-Pv.y);
        //determina el ángulo del vector (O, P)
        double theta = ArgPos(P.x-Pv.x, P.y-Pv.y);

        //gira los ángulos hasta alinear thetaini con el origen
        thetafin -= thetaini;
        theta -= thetaini;

        //asigna el argumento principal
        if(thetafin < 0)
                thetafin += M_2PI;
        if(theta < 0)
                theta += M_2PI;

        //ya puede alinear thetaini
        //thetaini = 0;

        if(theta<0 || thetafin<theta)
                return false; //indica que no está en el intervalo
        else //thetaini<=theta && theta<=thetafin
                return true; //indica que está en el intervalo
}

//determina si el punto P está en el subespacio comprendido
//entre las dos rectas (rectas excluidas) que pasan por Pa y Pb
//y son normales a la recta que pasa por Pa y Pb
bool PointIsInOpenStrip(TDoublePoint P, TDoublePoint Pa, TDoublePoint Pb)
{
        //los puntos no deben coincidir ya que deben definir una orientación
        if(Pa == Pb)
                throw EImproperArgument("Pa should be unequal Pb");

        //calcula el versor de PA a PB
        TDoublePoint m = Pb - Pa;
        m = m/Mod(m);

        //calcula la distancia desde el origen de la recta que contiene a Pa
        double da = Dot(Pa, m);

        //calcula la distancia desde el origen de la recta que contiene a Pb
        double db = Dot(Pb, m);

        //calcula la distancia desde el origen de la recta que contiene a P
        double d = Dot(P, m);

        if(d<=da || db<=d) //si R no está en el intervalo (Ra, Rb)
                return false; //indica que P no está en el intervalo (Ra, Rb)

        return true; //indica que P si está en el intervalo [Ra, Rb]
}
//determina si el punto P está en el ángulo (lados excluidos)
//de vértice Pv y cuyos lados pasan por Pini y Pfin en sentido levógiro
bool PointIsInOpenAngle(TDoublePoint P,
        TDoublePoint Pfin, TDoublePoint Pini, TDoublePoint Pv)
{
        //los puntos de los lados no deben coincidir con el vértice
        if(Pini == Pv)
                throw EImproperArgument("Pini should be unequal Pv");
        if(Pfin == Pv)
                throw EImproperArgument("Pfin should be unequal Pv");

        //si el punto está sobre el vértice
        if(P == Pv)
                return false; //indica que no está en el ángulo

        //Se recuerda que el dominio de la función ArgPos es [0, 2*PI).

        //determina el ángulo del lado inicial
        double thetaini = ArgPos(Pini.x-Pv.x, Pini.y-Pv.y);
        //determina el ángulo del lado final
        double thetafin = ArgPos(Pfin.x-Pv.x, Pfin.y-Pv.y);
        //determina el ángulo del vector (O, P)
        double theta = ArgPos(P.x-Pv.x, P.y-Pv.y);

        //gira los ángulos hasta alinear thetaini con el origen
        thetafin -= thetaini;
        theta -= thetaini;

        //asigna el argumento principal
        if(thetafin < 0)
                thetafin += M_2PI;
        if(theta < 0)
                theta += M_2PI;

        //ya puede alinear thetaini
        //thetaini = 0;

        if(theta<=0 || thetafin<=theta)
                return false; //indica que no está en el intervalo
        else //thetaini<=theta && theta<=thetafin
                return true; //indica que está en el intervalo
}

//determina la posición relativa del punto P
//respecto la semibanda delimitada por (Pa, Pb)
//devuelve:
//      -1: el punto P está antes de la línea que pasa por Pa
//      0: el punto P está en la banda delimitada por (Pa, Pb) y su distancia
//         a la recta que pasa por (Pa, Pb) es mayor o igual que cero
//      1: el punto P está despues d ela línea que pasa por Pb
//      2: el punto P está en la banda delimitada por (Pa, Pb) y su distancia
//         a la recta que pasa por (Pa, Pb) es menor que cero
int PointInHalfStrip(TDoublePoint P, TDoublePoint Pa, TDoublePoint Pb)
{
        //los puntos no deben coincidir
        if(Pa == Pb)
                throw EImproperArgument("Pa should be unequal Pb");

        TDoublePoint m;
        m.x = Pb.x - Pa.x;
        m.y = Pb.y - Pa.y;
        m = m/Mod(m);

        double d = Dot(P, m); //distancia del origen a P en la dirección de m

        double da = Dot(Pa, m); //distancia del origen a Pa en la dirección de m
        if(d < da) //si P está antes que Pa
                return -1; //lo indica

        double db = Dot(Pb, m); //distancia del origen a Pb en la dirección de m
        if(d > db) //si P está despues que Pb
                return 1; //lo indica

        //calcula la distancia de P a la recta que pasa por (Pa, Pb)
        if(DistanceLineToPoint(Pa, Pb, P) >= 0) //si está en la semitira
                return 0; //indica 0
        else //si está en el otro lado
                return 2; //indica 2
}

//determina si los puntos (Pa, Pb, Pc)
//son los vértices de un trilátero convexo
bool IsTrilateral(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc)
{
        if(Pa == Pb) //si los puntos contuguos coinciden
                return false; //no puede ser cuadrilátero

        //si el próximo punto no está a la derecha
        if(DistanceLineToPoint(Pa, Pb, Pc) >= 0)
                return false; //no puede ser cuadrilátero

        if(Pb == Pc) //si los puntos contuguos coinciden
                return false; //no puede ser cuadrilátero

        //si el próximo punto no está a la derecha
        if(DistanceLineToPoint(Pb, Pc, Pa) >= 0)
                return false; //no puede ser cuadrilátero

        if(Pc == Pa) //si los puntos contuguos coinciden
                return false; //no puede ser cuadrilátero

        //si el próximo punto no está a la derecha
        if(DistanceLineToPoint(Pc, Pa, Pb) >= 0)
                return false; //no puede ser cuadrilátero

        //ningún par de puntos sucesivos coincide y
        //todos los puntos contiguos están a la derecha
        return true;
}
//determina si los puntos (Pa, Pb, Pc, Pd)
//son los vértices de un cuadrilátero convexo
bool IsCuadrilateral(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Pc, TDoublePoint Pd)
{
        if(Pa == Pb) //si los puntos ontuguos coinciden
                return false; //no puede ser cuadrilátero

        //si el próximo punto no está a la derecha
        if(DistanceLineToPoint(Pa, Pb, Pc) >= 0)
                return false; //no puede ser cuadrilátero

        if(Pb == Pc) //si los puntos ontuguos coinciden
                return false; //no puede ser cuadrilátero

        //si el próximo punto no está a la derecha
        if(DistanceLineToPoint(Pb, Pc, Pd) >= 0)
                return false; //no puede ser cuadrilátero

        if(Pc == Pd) //si los puntos ontuguos coinciden
                return false; //no puede ser cuadrilátero

        //si el próximo punto no está a la derecha
        if(DistanceLineToPoint(Pc, Pd, Pa) >= 0)
                return false; //no puede ser cuadrilátero

        if(Pd == Pa) //si los puntos ontuguos coinciden
                return false; //no puede ser cuadrilátero

        //si el próximo punto no está a la derecha
        if(DistanceLineToPoint(Pd, Pa, Pb) >= 0)
                return false; //no puede ser cuadrilátero

        //ningún par de puntos sucesivos coincide y
        //todos los puntos contiguos están a la derecha
        return true;
}
//determina si en (Pa, Pb, Pc)
//Pc es el vértice y Pa y Pb son puntos en los lados de un ángulo
//si Pa==Pc o Pb==Pc devuelve falso
bool IsAngle(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc)
{
        //Pa no debe coincidir con Pc
        if(Pa == Pc)
                return false; //indica que no se cumple la condición

        //Pb no debe coincidir con Pc
        if(Pb == Pc)
                return false; //indica que no se cumple la condición

        return true; //indica que se cumplen las dos condiciones
}
//determina si en (Pa, Pb, Pc, R)
//Pc es el vértice y Pa y Pb son los vértices de un arco
//si Pa==Pc o Pb==Pc o R<=0, devuelve falso
//si Abs(Mod(Pa - Pc)) > ERR_NUM, devuelve falso
//si Abs(Mod(Pb - Pc)) > ERR_NUM, devuelve falso
bool IsArc(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R)
{
        //Pa no debe coincidir con Pc
        if(Pa == Pc)
                return false; //indica que no se cumple la condición

        //Pb no debe coincidir con Pc
        if(Pb == Pc)
                return false; //indica que no se cumple la condición

        //R debe ser mayor que cero
        if(R <= 0)
                return false; //indica que no se cumple la condición

        //Pa debe estar en el entorno de un punto del arco
        double epsilon = Abs(Mod(Pa - Pc) - R);
        if(epsilon > ERR_NUM)
                return false;

        //Pb debe estar en el entorno de un punto del arco
        epsilon = Abs(Mod(Pb - Pc) - R);
        if(epsilon > ERR_NUM)
                return false;

        return true; //indica que se cumplen las dos condiciones
}

//Determina si el punto p es interior al rectángulo R
bool Inner(const TDoublePoint& p, const TDoubleRect &R)
{
    if (p.x<R.Left || R.Right<p.x)
            return false;

    if (p.y<R.Top || R.Bottom<p.y)
            return false;

        return true;
}

//---------------------------------------------------------------------------
//DETERMINACIÓN DEL ÁNGULO QUE CONTIENE A UNA FIGURA:

//Determina los puntos por los que pasa el primer y segundo eje P1 y P2
//del ángulo con vértice en Q que contiene el segmento (Pa, Pb).
//Si el punto P está sobre alguno de los vértices,
//lanza una excepción EImproperArgument.
void AxisAngleSegment(TDoublePoint &P1, TDoublePoint &P2,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Q)
{
        //el vértice del ángulo (Q) no debe estar sobre el vértice del segmento (Pa)
        if(Q == Pa)
                throw EImproperArgument("vertex angle (Q) should not to be on vertex segment (Pa)");
        //el vértice del ángulo (Q) no debe estar sobre el vértice del segmento (Pb)
        if(Q == Pb)
                throw EImproperArgument("vertex angle (Q) should not to be on vertex segment (Pb)");

        //ADVERTENCIA: los puntos Pa y Pb si pueden coincidir.

        //si el ángulo de Pa a Pb es negativo
        if(Angle(Pa-Q, Pb-Q) < 0) {
                //asigna los vértices del segmento en orden inverso
                P1 = Pb;
                P2 = Pa;
        } else { //si no
                //asigna los vértices del segmento en orden directo
                P1 = Pa;
                P2 = Pb;
        }
}
//Determina los puntos por los que pasa el primer y segundo eje P1 y P2
//del ángulo con vértice en Q que contiene el arco (Pa, Pb, Pc, R).
//Si el punto P está sobre alguno de los vértices,
//lanza una excepción EImproperArgument.
//Si el punto está sobre el arco, la solución son los vértices del arco
//(ordenados).
void AxisAngleArc(TDoublePoint &P1, TDoublePoint &P2,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R,
        TDoublePoint Q)
{
        //(Pa, Pb, Pc, R) debe ser un arco
        if(!IsArc(Pa, Pb, Pc, R))
                //indica que (Pa, Pb, Pc, R) debería ser un arco
                throw EImproperArgument("(Pa, Pb, Pc, R) should be an arc");

        //el vértice del ángulo (Q) no debe estar sobre el vértice del arco (Pa)
        if(Q == Pa)
                throw EImproperArgument("vertex angle (Q) should not to be on vertex arc (Pa)");
        //el vértice del ángulo (Q) no debe estar sobre el vértice del arco (Pb)
        if(Q == Pb)
                throw EImproperArgument("vertex angle (Q) should not to be on vertex arc (Pb)");

        //ADVERTENCIA: los puntos Pa y Pb si pueden coincidir.

        //calcula la distancia de Pc a Q
        double d = Mod(Q - Pc);

        //detrmina los vértices del arco por los que pasan
        //los ejes con origen en Q
        TDoublePoint _P1, _P2;
        AxisAngleSegment(_P1, _P2, Pa, Pb, Q);

        //si el punto está dentro de la circunferencia del arco (o sobre ella)
        if(d <= R) {
                //devuelve los vértices del arco
                P1 = _P1;
                P2 = _P2;
                return;
        }

        //------------------------------------------------------------------
        //determina el otro punto de intersección (P) del eje (Q, _P1):

        //determina el versor en la dirección del eje (Q, _P1)
        TDoublePoint m = _P1 - Q;
        m = m/Mod(m);

        //determina el versor dextrógiro de m
        TDoublePoint n = Cross(m);

        //calcula el vector (Pc, Q)
        TDoublePoint V = Q - Pc;

        //calcula la distancia de Pc a la recta (en la dirección de n)
        double D = Dot(V, n);

        //si la distancia es negativa
        if(D < 0) {
                //invierte n y la distancia
                n = -1*n;
                D = -1*D;
        }

        //determina el punto medio entre _P1 y P
        TDoublePoint P = Pc + n*D;

        //determina el punto P
        P = P + (P - _P1);

        //------------------------------------------------------------------

        TDoublePoint trash;

        //si el punto P está en el ángulo abierto (Pa, Pb, Pc)
        if(PointIsInOpenAngle(P, Pa, Pb, Pc))
                //asigna el primer punto de intersección entre
                //la circunferencias (Pc, R) y (Q, Mod(V))
                IntersectionCircumCircum(P1, trash, Pc, R, Q, Mod(V));
        else //si no
                P1 = _P1; //asigna el vértice

        //------------------------------------------------------------------
        //determina el otro punto de intersección (P) del eje (Q, _P2):

        //determina el versor en la dirección del eje (Q, _P1)
        m = _P2 - Q;
        m = m/Mod(m);

        //determina el versor dextrógiro de m
        n = Cross(m);

        //No hace falta calcular el vector (Pc, Q),
        //por que ya lo tenemos de antes:
        //      V = Q - Pc;

        //calcula la distancia de Pc a la recta (en la dirección de n)
        D = Dot(V, n);

        //si la distancia es negativa
        if(D < 0) {
                //invierte n y la distancia
                n = -1*n;
                D = -1*D;
        }

        //determina el punto medio entre _P1 y P
        P = Pc + n*D;

        //determina el punto P
        P = P + (P - _P2);

        //------------------------------------------------------------------

        //si el punto P está en el ángulo abierto (Pa, Pb, Pc)
        if(PointIsInOpenAngle(P, Pa, Pb, Pc))
                //asigna el último punto de intersección entre
                //la circunferencias (Pc, R) y (Q, Mod(V))
                IntersectionCircumCircum(trash, P2, Pc, R, Q, Mod(V));
        else //si no
                P2 = _P2; //asigna el vértice

}

//---------------------------------------------------------------------------
//DISTANCIAS ARITMÉTICAS MÍNIMAS ENTRE PARES DE FIGURAS:

//Distancia entre el punto p y la recta (A, B, C).
//Si el punto P está a la derecha de la recta la distancia es negativa.
double Distance(const TDoublePoint P, double A, double B, double C)
{
    Normalize (A, B, C);
    return P.x*A + P.y*B + C;
}

//Distancia entre el punto p y la recta que pasa por (Pa, Pb).
//Si el punto P está a la derecha de la recta la distancia es negativa.
double DistanceLineToPoint(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint P)
{
        //los puntos de la recta no deben coincidir
        if(Pa == Pb)
                throw EImproperArgument("Pa should be unequal Pb");

        //calcula el versor normal de la recta
        TDoublePoint n;
        n.x = Pb.x - Pa.x;
        n.y = Pb.y - Pa.y;
        n = n/Mod(n);
        n = Cross(n);
        //proyecto el vector P-Pa sobre n
        return Dot(P-Pa, n);
}

//---------------------------------------------------------------------------
//DISTANCIAS MÍNIMAS ENTRE PARES DE FIGURAS:

//determina la distancia mínima entre el segmento (Pa, Pb) y el punto P
double DistanceSegmentPoint(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint P)
{
        //si los puntos del segmento coinciden, este se reduce a un punto
        if(Pa == Pb) {
                if(P == Pa) //si los puntos coinciden
                        return 0; //evita introducir error numérico
                else //si los puntos no coinciden
                        //calcula la distancia punto a punto
                        return Mod(P - Pa);
        }

        //si los puntos no coinciden depende de la posición relativa

        //determina el versor en la dirección Pa a Pb
        TDoublePoint m;
        m.x = Pb.x-Pa.x;
        m.y = Pb.y-Pa.y;
        double module = sqrt(m.x*m.x + m.y*m.y);
        m.x /= module;
        m.y /= module;

        //calcula la distancia de cada punto al origen de coordenadas
        //en la dirección y sentido de m
        double da = Dot(Pa, m);
        double db = Dot(Pb, m);
        double d = Dot(P, m);

        if(d < da) //si está antes de llegar a Pa
                return Mod(P - Pa); //es la distancia a Pa
        else if(d > db) //si está pasado Pb
                return Mod(P - Pb); //es la distancia a Pb
        else { //si está entre Pa y Pb
                //calcula la distancia entre P y la recta que pasa por Pa y Pb
                TDoublePoint n = Cross(m);
                return Abs(Dot(P-Pa, n));
        }
}
//calcula la distancia entre el segmento (Pa, Pb) y el segmento (Qa, Qb)
double DistanceSegmentSegment(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Qa, TDoublePoint Qb)
{
        //determina si los segmentos si intersecan
        TDoublePoint P;
        if(IntersectionSegmentSegment(P, Pa, Pb, Qa, Qb)) //si intersecan
                return 0; //la distancia es cero

        //calcula la distancia de cada punto al otro segmento
        double d1 = DistanceSegmentPoint(Pa, Pb, Qa);
        double d2 = DistanceSegmentPoint(Pa, Pb, Qb);
        double d3 = DistanceSegmentPoint(Qa, Qb, Pa);
        double d4 = DistanceSegmentPoint(Qa, Qb, Pb);

        //al ser l atrayectoria de un vértice a otro lineal,
        //la distancia corresponde a la distancia mínima
        //de alguno de los vértices
        return Min(Min(d1, d2), Min(d3, d4));
}
//calcula la distancia entre el segmento (Pa, Pb) y la circunferencia (Pc, R)
double DistanceSegmentCircunference(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Pc, double R)
{
        //el radio R debería ser no negativo
        if(R < 0)
                throw EImproperArgument("radio R should not be negative");

        double d1 = Mod(Pa - Pc) - R;
        double d2 = Mod(Pb - Pc) - R;

        if(d1*d2 < 0)
                return 0;

        if(d1<0 && d2<0)
                return Max(d1, d2);

        double d = DistanceSegmentPoint(Pa, Pb, Pc);

        if(d <= R)
                return 0;

        return d - R;
}
//calcula la distancia entre el segmento (Qa, Qb) y el arco (Pc, R, Pa, Pb)
double DistanceSegmentArc(TDoublePoint Qa, TDoublePoint Qb,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R)
{
        //(Pa, Pb, Pc, R) debe ser un arco
        if(!IsArc(Pa, Pb, Pc, R))
                throw EImproperArgument("(Pa, PB, PC, R) should be an arc");

        //cuando Qa = Qb el segmento se reduce a un punto

        return DistanceArcSegment(Pa, Pb, Pc, R, Qa, Qb);
}
//calcula la distancia entre el segmento (Pa, Pb) y el círculo (Pc, R)
double DistanceSegmentCircle(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Pc, double R)
{
        //cuando Pa==Pb el segmento se reduce a un punto
        //cuando R==0 el círculo se reduce a un punto

        //el radio R no debe ser negativo
        if(R < 0)
                throw EImproperArgument("radio R should not be negative");

        double D = DistanceSegmentPoint(Pa, Pb, Pc);
        if(D > R) //si no están en contacto
                return D - R; //devuelve la distancia

        return 0; //indica que están en contacto
}

//calcula la distancia entre el arco (Pa, Pb, Pc, R) y el punto P
double DistanceArcPoint(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Pc, double R, TDoublePoint P)
{
        //(Pa, Pb, Pc, R) debe ser un arco
        if(!IsArc(Pa, Pb, Pc, R))
                throw EImproperArgument("(Pa, Pb, Pc, R) shoud be an arc");

        //ADVERTENCIA: los puntos Pa y Pb pueden no estar en los vértices del arco.

        //si P está en el ángulo (Pa, Pb, Pc)
        if(PointIsInClosedAngle(P, Pa, Pb, Pc))
                //devuelve la distancia radial al arco
                return Abs(Mod(P - Pc) - R);
        //si P está fuera del ángulo (Pa, Pb, Pc)
        else {
                //mueve Pa y Pb a los vértices del arco
                TDoublePoint V;
                V = Pa - Pc;
                Pa = Pc + V/Mod(V)*R;
                V = Pb - Pc;
                Pb = Pc + V/Mod(V)*R;

                //devuelve la distancia mínima a los vértices
                return Min(Mod(P - Pa), Mod(P - Pb));
        }
}
//calcula la distancia entre el arco (Pc, R, Pa, Pb) y el segmento (Qa, Qb)
double DistanceArcSegment(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint Pc, double R, TDoublePoint Qa, TDoublePoint Qb)
{
        //(Pa, Pb, Pc, R) debe ser un arco
        if(!IsArc(Pa, Pb, Pc, R))
                throw EImproperArgument("(Pa, PB, PC, R) should be an arc");

        //cuando Qa = Qb el segmento se reduce a un punto

        //calcula el punto de proyección de Pc
        //sobre la recta que pasa por (Qa, Qb)
        TDoublePoint m = Qb - Qa;
        m = m/Mod(m);
        TDoublePoint n = Cross(m);
        double D = Dot(Qa - Pc, n);
        TDoublePoint P = Pc + D*n;

        //si el arco y el segmento "confrontan sus cuerdas"
        if(PointIsInClosedStrip(P, Qa, Qb) && PointIsInClosedAngle(P, Pa, Pb, Pc)) {
                if(fabs(D) >= R)
                        return fabs(D) - R;
        }

        //si el segmento interseca al arco
        if(IntersectionSegmentArc(Qa, Qb, Pa, Pb, Pc, R))
                return 0;

        //si no la distancia será entre algún vértice y la figura homónima

        //calcula la distancia entre los vértices y la figura homónima
        double d1 = DistanceArcPoint(Pa, Pb, Pc, R, Qa);
        double d2 = DistanceArcPoint(Pa, Pb, Pc, R, Qb);
        double d3 = DistanceSegmentPoint(Qa, Qb, Pa);
        double d4 = DistanceSegmentPoint(Qa, Qb, Pb);

        //la distancia entre el segmento y el arco es la mínima
        return Min(Min(d1, d2), Min(d3, d4));
}
//calcula la distancia entre el círculo (Pc, R1) y el arco (Qa, Qb, Qc, R2)
double DistanceArcCircle(
        TDoublePoint Qa, TDoublePoint Qb, TDoublePoint Qc, double R2,
        TDoublePoint Pc, double R1)
{
        TVector<double> Distances;

        //calcula los puntos de intersección de la recta (Pc, Qc)
        //con la circunferecnia (Qc, R2)
        TDoublePoint P1, P2;
        if(IntersectionLineCircum(P1, P2, Pc, Qc, Qc, R2)) { //si hay intersección
                //si el punto P1 está en el arco lo añade
                if(PointIsInOpenAngle(P1, Qa, Qb, Qc))
                        Distances.Add(Mod(P1-Pc) - R1);
                //si el punto P2 está en el arco lo añade
                if(PointIsInOpenAngle(P2, Qa, Qb, Qc))
                        Distances.Add(Mod(P2-Pc) - R1);
        }

        //calcula las distancias de los vértices al círculo
        Distances.Add(Mod(Qa-Pc) - R1);
        Distances.Add(Mod(Qb-Pc) - R1);

        return Distances.getMin(); //devuelve la distancia mínima
}
//calcula la distancia entre el arco (Pa, Pb, Pc, R1)
//y la circunferencia (Qc, R2)
double DistanceArcCircunference(
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R1,
        TDoublePoint Qc, double R2)
{
        //(Pa1, Pb1, Pc1, R1) debe ser un arco
        if(!IsArc(Pa, Pb, Pc, R1))
                throw EImproperArgument("(Pa, Pb, Pc, R1) should be an arc");

        //el radio R2 no debe ser negativo
        if(R2 < 0)
                throw EImproperArgument("radio R2 should not be negative");

        //si el arco interseca a la circunferencia
        if(IntersectionArcCircunference(Pa, Pb, Pc, R1, Qc, R2))
                return 0;

        //determina la circunferecnia mayor y la menor
        TDoublePoint Pcmax;
        double Rmax;
        TDoublePoint Pcmin;
        double Rmin;
        if(R1 > R2) { //si el arco es mayor que la circunferencia
                Pcmax = Pc;
                Rmax = R1;

                Pcmin = Qc;
                Rmin = R2;
        }
        else { //si la circunferencia en mayor que el arco
                Pcmax = Qc;
                Rmax = R2;

                Pcmin = Pc;
                Rmin = R1;
        }

        //calcula el vector del mayor al menor, la distancia entre centros
        //y los límites de los intervalos
        TDoublePoint V = Pcmin - Pcmax;
        double D = Mod(V);
        double Dmin = Rmax - Rmin;
        double Dmax = Rmax + Rmin;

        //si las circunferencias son interiores concéntricas
        if(D <= 0)
                return Dmin; //devuelve la diferencia entre los radios

        //si las circunferencias son interiores excéntricas
        if(0<D && D<Dmin) {
                //calcula los puntos de máxima proximidad
                TDoublePoint m = V/D;
                TDoublePoint Pmin = Pcmin + Rmin*m;
                TDoublePoint Pmax = Pcmax + Rmax*m;
                //y uno de los puntos de máxima proximidad está en el arco
                if((R1>R2 && PointIsInClosedAngle(Pcmax, Pa, Pb, Pc)) || PointIsInClosedAngle(Pcmin, Pa, Pb, Pc))
                        //devuelve la distancia entre los puntos
                        return Mod(Pmax - Pmin);
        }

        //si las circunferencias son exteriores
        if(D > Dmax) {
                //calcula los puntos de máxima proximidad
                TDoublePoint m = V/D;
                TDoublePoint Pmin = Pcmin - Rmin*m;
                TDoublePoint Pmax = Pcmax + Rmax*m;
                //y uno de los puntos de máxima proximidad está en el arco
                if((R1>R2 && PointIsInClosedAngle(Pcmax, Pa, Pb, Pc)) || PointIsInClosedAngle(Pcmin, Pa, Pb, Pc))
                        //devuelve la distancia entre los puntos
                        return Mod(Pmax - Pmin);
        }

        //a partir de aquí la distancia será
        //entre algún vértice y la figura homónima

        //calcula la distancia entre los vértices y la figura homónima
        double d1 = DistanceCircunferencePoint(Qc, R2, Pa);
        double d2 = DistanceCircunferencePoint(Qc, R2, Pb);

        //la distancia entre el segmento y el arco es la mínima
        return Min(d1, d2);
}
//calcula la distancia entre el arco (Pa1, Pb1, Pc1, R1)
//y el arco (Pa2, Pb2, Pc2, R2)
double DistanceArcArc(
        TDoublePoint Pa1, TDoublePoint Pb1, TDoublePoint Pc1, double R1,
        TDoublePoint Pa2, TDoublePoint Pb2, TDoublePoint Pc2, double R2)
{
        //(Pa1, Pb1, Pc1, R1) debe ser un arco
        if(!IsArc(Pa1, Pb1, Pc1, R1))
                throw EImproperArgument("(Pa1, PB1, PC1, R1) should be an arc");

        //(Pa2, Pb2, Pc2, R2) debe ser un arco
        if(!IsArc(Pa2, Pb2, Pc2, R2))
                throw EImproperArgument("(Pa2, PB2, PC2, R2) should be an arc");

        //si los arcos se intersecan
        if(IntersectionArcArc(Pa1, Pb1, Pc1, R1, Pa2, Pb2, Pc2, R2))
                return 0;

        //determina que arco es el mayor y cual es menor
        TDoublePoint Pamax, Pbmax, Pcmax;
        double Rmax;
        TDoublePoint Pamin, Pbmin, Pcmin;
        double Rmin;
        if(R1 > R2) { //si el arco 1 es el mayor
                Pamax = Pa1;
                Pbmax = Pb1;
                Pcmax = Pc1;
                Rmax = R1;

                Pamin = Pa2;
                Pbmin = Pb2;
                Pcmin = Pc2;
                Rmin = R2;
        }
        else { //si el arco 2 es el mayor
                Pamax = Pa2;
                Pbmax = Pb2;
                Pcmax = Pc2;
                Rmax = R2;

                Pamin = Pa1;
                Pbmin = Pb1;
                Pcmin = Pc1;
                Rmin = R1;
        }

        //calcula el vector del mayor al menor, la distancia entre centros
        //y los límites de los intervalos
        TDoublePoint V = Pcmin - Pcmax;
        double D = Mod(V);
        double Dmin = Rmax - Rmin;
        double Dmax = Rmax + Rmin;

        //si las circunferencias son interiores concéntricas
        if(D <= 0)
                return Dmin; //devuelve la diferencia entre los radios

        //si las circunferencias son interiores excéntricas
        if(0<D && D<Dmin) {
                //calcula los puntos de máxima proximidad
                TDoublePoint m = V/D;
                TDoublePoint Pmin = Pcmin + Rmin*m;
                TDoublePoint Pmax = Pcmax + Rmax*m;
                //y los puntos de máxima proximidad están en los arcos
                if(PointIsInClosedAngle(Pmin, Pamin, Pbmin, Pcmin) &&
                        PointIsInClosedAngle(Pmax, Pamax, Pbmax, Pcmax))
                        //devuelve la distancia entre los puntos
                        return Mod(Pmax - Pmin);
        }

        //si las circunferencias son exteriores
        if(D > Dmax) {
                //calcula los puntos de máxima proximidad
                TDoublePoint m = V/D;
                TDoublePoint Pmin = Pcmin - Rmin*m;
                TDoublePoint Pmax = Pcmax + Rmax*m;
                //y los puntos de máxima proximidad están en los arcos
                if(PointIsInClosedAngle(Pmin, Pamin, Pbmin, Pcmin) &&
                        PointIsInClosedAngle(Pmax, Pamax, Pbmax, Pcmax))
                        //devuelve la distancia entre los puntos
                        return Mod(Pmax - Pmin);
        }

        //a partir de aquí la distancia será
        //entre algún vértice y la figura homónima

        //calcula la distancia entre los vértices y la figura homónima
        double d1 = DistanceArcPoint(Pa1, Pb1, Pc1, R1, Pa2);
        double d2 = DistanceArcPoint(Pa1, Pb1, Pc1, R1, Pb2);
        double d3 = DistanceArcPoint(Pa2, Pb2, Pc2, R2, Pa1);
        double d4 = DistanceArcPoint(Pa2, Pb2, Pc2, R2, Pb1);

        //la distancia entre el segmento y el arco es la mínima
        return Min(Min(d1, d2), Min(d3, d4));
}

//determina la distancia entre una circunferencia y un punto
double DistanceCircunferencePoint(TDoublePoint Pc, double R, TDoublePoint Q)
{
        //el radio R debería ser no negativo
        if(R < 0)
                throw EImproperArgument("radio R should be not negative");

        return Abs(Mod(Q-Pc) - R);
}
//calcula la distancia entre la circunferencia (P, R) y el segmento (Pa, Pb)
double DistanceCircunferenceSegment(TDoublePoint Pc, double R,
        TDoublePoint Pa, TDoublePoint Pb)
{
        //el radio R debería ser no negativo
        if(R < 0)
                throw EImproperArgument("radio R should not be negative");

        double d1 = Mod(Pa - Pc) - R;
        double d2 = Mod(Pb - Pc) - R;

        if(d1*d2 < 0)
                return 0;

        if(d1<0 && d2<0)
                return Max(d1, d2);

        double d = DistanceSegmentPoint(Pa, Pb, Pc);

        if(d <= R)
                return 0;

        return d - R;
}
//calcula la distancia entre la circunferencia (P1, R1) y el círculo (P2, R2)
double DistanceCircunferenceCircle(TDoublePoint P1, double R1,
        TDoublePoint P2, double R2)
{
        //el radio R1 debería ser no negativo
        if(R1 < 0)
                throw EImproperArgument("radio R1 should not be negative");

        //el radio R2 debería ser no negativo
        if(R2 < 0)
                throw EImproperArgument("radio R2 should not be negative");

        //calcula la distancia entre los centros
        double d = Mod(P2 - P1);

        //calcula la diferencia entre los radios
        double d1 = Abs(R1 - R2);

        //si la circunferencia es mayor que el círculo
        //y la distancia es menor que la diferencia
        if(R1>R2 && d<d1)
                return d1 - d; //devuelve la diferencia

        //calcula la suma de los radios
        double d2 = R1 + R2;

        //si la distancia es mayor que la suma
        if(d > d2)
                return d - d2; //devuelve la diferencia

        //si no es que las circunferencias se tocan
        return 0;
}
//calcula la distancia entre la circunferencia (P1, R1)
//y la circunferencia (P2, R2)
double DistanceCircunferenceCircunference(TDoublePoint P1, double R1,
        TDoublePoint P2, double R2)
{
        //el radio R1 debería ser no negativo
        if(R1 < 0)
                throw EImproperArgument("radio R1 should not be negative");

        //el radio R2 debería ser no negativo
        if(R2 < 0)
                throw EImproperArgument("radio R2 should not be negative");

        //calcula la distancia entre los centros
        double d = Mod(P2 - P1);

        //calcula la diferencia entre los radios
        double d1 = Abs(R1 - R2);

        //si la distancia es menor que la diferencia
        if(d < d1)
                return d1 - d; //devuelve la diferencia

        //calcula la suma de los radios
        double d2 = R1 + R2;

        //si la distancia es mayor que la suma
        if(d > d2)
                return d - d2; //devuelve la diferencia

        //si no es que las circunferencias se tocan
        return 0;
}
//calcula la distancia entre la circunferencia (Qc, R2)
//y el arco (Pa, Pb, Pc, R1)
double DistanceCircunferenceArc(
        TDoublePoint Qc, double R2,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R1)
{
        //(Pa1, Pb1, Pc1, R1) debe ser un arco
        if(!IsArc(Pa, Pb, Pc, R1))
                throw EImproperArgument("(Pa, Pb, Pc, R1) should be an arc");

        //el radio R2 no debe ser negativo
        if(R2 < 0)
                throw EImproperArgument("radio R2 should not be negative");

        //si el arco interseca a la circunferencia
        if(IntersectionArcCircunference(Pa, Pb, Pc, R1, Qc, R2))
                return 0;

        //determina la circunferecnia mayor y la menor
        TDoublePoint Pcmax;
        double Rmax;
        TDoublePoint Pcmin;
        double Rmin;
        if(R1 > R2) { //si el arco es mayor que la circunferencia
                Pcmax = Pc;
                Rmax = R1;

                Pcmin = Qc;
                Rmin = R2;
        }
        else { //si la circunferencia en mayor que el arco
                Pcmax = Qc;
                Rmax = R2;

                Pcmin = Pc;
                Rmin = R1;
        }

        //calcula el vector del mayor al menor, la distancia entre centros
        //y los límites de los intervalos
        TDoublePoint V = Pcmin - Pcmax;
        double D = Mod(V);
        double Dmin = Rmax - Rmin;
        double Dmax = Rmax + Rmin;

        //si las circunferencias son interiores concéntricas
        if(D <= 0)
                return Dmin; //devuelve la diferencia entre los radios

        //si las circunferencias son interiores excéntricas
        if(0<D && D<Dmin) {
                //calcula los puntos de máxima proximidad
                TDoublePoint m = V/D;
                TDoublePoint Pmin = Pcmin + Rmin*m;
                TDoublePoint Pmax = Pcmax + Rmax*m;
                //y uno de los puntos de máxima proximidad está en el arco
                if((R1>R2 && PointIsInClosedAngle(Pcmax, Pa, Pb, Pc)) || PointIsInClosedAngle(Pcmin, Pa, Pb, Pc))
                        //devuelve la distancia entre los puntos
                        return Mod(Pmax - Pmin);
        }

        //si las circunferencias son exteriores
        if(D > Dmax) {
                //calcula los puntos de máxima proximidad
                TDoublePoint m = V/D;
                TDoublePoint Pmin = Pcmin - Rmin*m;
                TDoublePoint Pmax = Pcmax + Rmax*m;
                //y uno de los puntos de máxima proximidad está en el arco
                if((R1>R2 && PointIsInClosedAngle(Pcmax, Pa, Pb, Pc)) || PointIsInClosedAngle(Pcmin, Pa, Pb, Pc))
                        //devuelve la distancia entre los puntos
                        return Mod(Pmax - Pmin);
        }

        //a partir de aquí la distancia será
        //entre algún vértice y la figura homónima

        //calcula la distancia entre los vértices y la figura homónima
        double d1 = DistanceCircunferencePoint(Qc, R2, Pa);
        double d2 = DistanceCircunferencePoint(Qc, R2, Pb);

        //la distancia entre el segmento y el arco es la mínima
        return Min(d1, d2);
}

//alcula la distancia entre los círculos (Pc1, R1) y (Pc2, R2)
double DistanceCircleCircle(TDoublePoint Pc1, double R1,
        TDoublePoint Pc2, double R2)
{
        //el radio R1 debería ser no negativo
        if(R1 < 0)
                throw EImproperArgument("radio R1 should not be negative");

        //el radio R2 debería ser no negativo
        if(R2 < 0)
                throw EImproperArgument("radio R2 should not be negative");

        //calcula la distancia entre los círculos
        double d = Mod(Pc2 - Pc1);

        //resta los radios
        d -= R1;
        d -= R2;

        //si la distancia sale negativa
        if(d <= 0)
                return 0; //las circunferencias se tocan
        else //si no
                return d; //devuelve la distancia
}
//calcula la distancia entre el círculo (Pc, R) y el segmento (Pa, Pb)
double DistanceCircleSegment(TDoublePoint Pc, double R,
        TDoublePoint Pa, TDoublePoint Pb)
{
        //cuando Pa==Pb el segmento se reduce a un punto
        //cuando R==0 el círculo se reduce a un punto

        //el radio R no debe ser negativo
        if(R < 0)
                throw EImproperArgument("radio R should not be negative");

        return DistanceSegmentCircle(Pa, Pb, Pc, R);
}
//calcula la distancia entre el círculo (P1, R1) y la circunferencia (P2, R2)
double DistanceCircleCircunference(TDoublePoint P1, double R1,
        TDoublePoint P2, double R2)
{
        //el radio R1 debería ser no negativo
        if(R1 < 0)
                throw EImproperArgument("radio R1 should not be negative");

        //el radio R2 debería ser no negativo
        if(R2 < 0)
                throw EImproperArgument("radio R2 should not be negative");

        //calcula la distancia entre los centros
        double d = Mod(P2 - P1);

        //calcula la diferencia entre los radios
        double d1 = Abs(R1 - R2);

        //si el círculo es menor que la circunferencia
        //y la distancia es menor que la diferencia
        if(R1<R2 && d<d1)
                return d1 - d; //devuelve la diferencia

        //calcula la suma de los radios
        double d2 = R1 + R2;

        //si la distancia es mayor que la suma
        if(d > d2)
                return d - d2; //devuelve la diferencia

        //si no es que las circunferencias se tocan
        return 0;
}
//calcula la distancia entre el círculo (Pc, R)
//y el cuadrilátero (Qa, Qb, Qc, Qd)
double DistanceCircleCuadrilateral(TDoublePoint Pc, double R,
        TDoublePoint Qa, TDoublePoint Qb, TDoublePoint Qc, TDoublePoint Qd)
{
        //comprueba que (Qa, Qb, Qc, Qd) son los vértices de un cuadrilátero
        if(!IsCuadrilateral(Qa, Qb, Qc, Qd))
                throw EImproperArgument(
                        "(Qa, Qb, Qc, Qd) isn't a cuadrilateral");

        //determina la posición relativa del punto Pc
        //en la semitira delimitada por (Qa, Qb)
        int p1 = PointInHalfStrip(Pc, Qa, Qb);
        if(p1 == 0) //si está en la semitira calculala distancia desde la recta
                return DistanceLineToPoint(Qa, Qb, Pc) - R;

        //determina la posición relativa del punto Pc
        //en la semitira delimitada por (Qb, Qc)
        int p2 = PointInHalfStrip(Pc, Qb, Qc);
        if(p2 == 0) //si está en la semitira calculala distancia desde la recta
                return DistanceLineToPoint(Qb, Qc, Pc) - R;

        if(p1==1 && p2==-1) //si está en la esquina de B
                return Mod(Pc - Qb) - R; //calcula la distancia desde el punto

        //determina la posición relativa del punto Pc
        //en la semitira delimitada por (Qc, Qd)
        int p3 = PointInHalfStrip(Pc, Qc, Qd);
        if(p3 == 0) //si está en la semitira calculala distancia desde la recta
                return DistanceLineToPoint(Qc, Qd, Pc) - R;

        if(p2==1 && p3==-1) //si está en la esquina de C
                return Mod(Pc - Qc) - R; //calcula la distancia desde el punto

        //determina la posición relativa del punto Pc
        //en la semitira delimitada por (Qd, Qa)
        int p4 = PointInHalfStrip(Pc, Qd, Qa);
        if(p4 == 0) //si está en la semitira calculala distancia desde la recta
                return DistanceLineToPoint(Qd, Qa, Pc) - R;

        if(p3==1 && p4==-1) //si está en la esquina de D
                return Mod(Pc - Qb) - R; //calcula la distancia desde el punto

        if(p4==1 && p1==-1) //si está en la esquina de A
                return Mod(Pc - Qa) - R; //calcula la distancia desde el punto

        return 0;
}
//calcula la distancia entre el círculo (Pc, R1) y el arco (Qa, Qb, Qc, R2)
double DistanceCircleArc(TDoublePoint Pc, double R1,
        TDoublePoint Qa, TDoublePoint Qb, TDoublePoint Qc, double R2)
{
        TVector<double> Distances;

        //calcula los puntos de intersección de la recta (Pc, Qc)
        //con la circunferecnia (Qc, R2)
        TDoublePoint P1, P2;
        if(IntersectionLineCircum(P1, P2, Pc, Qc, Qc, R2)) { //si hay intersección
                //si el punto P1 está en el arco lo añade
                if(PointIsInOpenAngle(P1, Qa, Qb, Qc))
                        Distances.Add(Mod(P1-Pc) - R1);
                //si el punto P2 está en el arco lo añade
                if(PointIsInOpenAngle(P2, Qa, Qb, Qc))
                        Distances.Add(Mod(P2-Pc) - R1);
        }

        //calcula las distancias de los vértices al círculo
        Distances.Add(Mod(Qa-Pc) - R1);
        Distances.Add(Mod(Qb-Pc) - R1);

        return Distances.getMin(); //devuelve la distancia mínima
}

//---------------------------------------------------------------------------
//DISTANCIAS MÁXIMAS ENTRE PARES DE FIGURAS:

double DistanceCirclePointMax(TDoublePoint Pc, double R,
        TDoublePoint P)
{
        double d = Mod(P - Pc);
        if(d <= R)
                return 0;
        else
                return d - R;
}
double DistanceSegmentPointMax(TDoublePoint Pa, TDoublePoint Pb,
        TDoublePoint P)
{
        return Max(Mod(P - Pa), Mod(P - Pb));
}
double DistanceCircunferencePointMax(TDoublePoint Pc, double R,
        TDoublePoint P)
{
        return  Abs(Mod(P - Pc) - R);
}
double DistanceArcPointMax(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc,
        double R, TDoublePoint P)
{
        //(Pa, Pb, Pc, R) debe ser un arco
        if(!IsArc(Pa, Pb, Pc, R))
                throw EImproperArgument("(Pa, Pb, Pc, R) shoud be an arc");

        //ADVERTENCIA: los puntos Pa y Pb pueden no estar en los vértices del arco.

        TDoublePoint Va = Pa - Pc; //calcula el vector (Pc, Pa)
        TDoublePoint Pa_ = Pc - Va; //calcula el punto espejo de Pa (respecto de Pc)

        TDoublePoint Vb = Pb - Pc; //calcula el vector (Pc, Pb)
        TDoublePoint Pb_ = Pc - Vb; //calcula el punto espejo de Pb (respecto de Pc)

        //si el punto P está en el ángulo (Pb_, Pa_, Pc)
        if(PointIsInClosedAngle(P, Pb_, Pa_, Pc)) {
                Va = Va/Mod(Va)*R; //redimensiona Va con longitud R
                Pa = Pc + Va; //mueve Pa al vértice del arco

                Vb = Vb/Mod(Vb)*R; //redimensiona Vb con longitud R
                Pb = Pc + Vb; //mueve Pb al vértice del arco

                //devuelve la distancia máxima a los vértices
                return Max(Mod(P - Pa), Mod(P - Pb));
        }
        //si el punto P está fuera del ángulo (Pb_, Pa_, Pc)
        else
                //devuelve la distancia al punto de intersección más distante
                return Mod(P - Pc) + R;
}

//---------------------------------------------------------------------------
//INTERSECCIÓN ENTRE PUNTO Y FIGURAS:

//para triláteros convexos dados por los vértices en sentido dextrógiro
//determina si hay intersección entre el punto P
//y el trilátero (Pa, Pb, Pc)
//si el punto no toca al trilátero devuelve falso
//si los puntos (Pa, Pb, Pc) no son los vértices del trilátero
//recorrido en sentido dextrógiro, esta función no indica intersección
bool IntersectionPointTrilateral(TDoublePoint P,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc)
{
        //los puntos (Pa, Pb, Pc) deben ser
        //los vértices de un trilátero convexo
        if(!IsTrilateral(Pa, Pb, Pc))
                throw EImproperArgument(
                        "(Pa, Pb, Pc) should be a convex trilateral");

        //si la distancia es mayor que cero
        if(DistanceLineToPoint(Pa, Pb, P) > 0)
                return false; //indica que el punto está fuera del trilátero

        //si la distancia es mayor que cero
        if(DistanceLineToPoint(Pb, Pc, P) > 0)
                return false; //indica que el punto está fuera del trilátero

        //si la distancia es mayor que cero
        if(DistanceLineToPoint(Pc, Pa, P) > 0)
                return false; //indica que el punto está fuera del trilátero

        return true; //indica que el punto pertenece al trilátero
}

//para cuadriláteros convexos dados por los vértices en sentido dextrógiro
//determina si hay intersección entre el punto P
//y el cuadrilátero (Pa, Pb, Pc, Pb)
//si el punto no toca al cuadrilátero devuelve falso
//si los puntos (Pa, Pb, Pc) no son los vértices del cuadrilátero
//recorrido en sentido dextrógiro, esta función no indica intersección
bool IntersectionPointCuadrilateral(TDoublePoint P,
        TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, TDoublePoint Pd)
{
        //los puntos (Pa, Pb, Pc, Pd) deben ser
        //los vértices de un cuadrilátero convexo
        if(!IsCuadrilateral(Pa, Pb, Pc, Pd))
                throw EImproperArgument(
                        "(Pa, Pb, Pc, Pd) should be a convex cuadrilateral");

        //si la distancia es mayor que cero
        if(DistanceLineToPoint(Pa, Pb, P) > 0)
                return false; //indica que el punto está fuera del cuadrilátero

        //si la distancia es mayor que cero
        if(DistanceLineToPoint(Pb, Pc, P) > 0)
                return false; //indica que el punto está fuera del cuadrilátero

        //si la distancia es mayor que cero
        if(DistanceLineToPoint(Pc, Pd, P) > 0)
                return false; //indica que el punto está fuera del cuadrilátero

        //si la distancia es mayor que cero
        if(DistanceLineToPoint(Pd, Pa, P) > 0)
                return false; //indica que el punto está fuera del cuadrilátero

        return true; //indica que el punto pertenece al cuadrilátero
}

//determina si hay intersección entre el punto P y el círculo (Pc, R)
//si el punto no toca al círculo devuelve falso
bool IntersectionPointCircle(TDoublePoint P, TDoublePoint Pc, double R)
{
        //calcula el vector de P a Pc
        double x = P.x - Pc.x;
        double y = P.y - Pc.y;

        if(sqrt(x*x + y*y) <= R) //si la distancia no supera a R
                return true; //indica que el punto pertenece al círculo

        return false; //indica que el punto no pertenece al círculo
}

//determina si hay intersección entre el punto P
//y la porción de círculo (Pc, R, Pfin, Pini) en sentido levógiro
//si el punto no toca a la proción devuelve falso
bool IntersectionPointPie(TDoublePoint P,
        TDoublePoint Pfin, TDoublePoint Pini, TDoublePoint Pc, double R)
{
        //calcula el vector de P a Pc
        double x = P.x - Pc.x;
        double y = P.y - Pc.y;

        //si la distancia no es mayor que R y
        //el punto está en el ángulo (Pc, Pfin, Pini)
        if(sqrt(x*x + y*y)<=R && PointIsInOpenAngle(P, Pfin, Pini, Pc))
                return true; //indica que el punto pertenece a la porción

        return false; //indica que el punto no pertenece a la porción
}

//---------------------------------------------------------------------------
//Funciones geométricas tridimensionales:
//---------------------------------------------------------------------------

/*
//Sea el sistema de versores ortogonales (u, v, w),
//dado el vector A en la dirección de u, y el vector C en el plano (u, w),
//calcular el versor u',
//resultado de girar el vector u un ángulo theta en torno de w.
//Es decir, despeja u' en la ecuación:
//      u x u' = sin(theta)*w
//donde:
//      u = A/Mod(A)
//      w = C/Mod(C)
Geometry::Solve(double &u_x, double &u_y, double &u_z,
        double Ax, double Ay, double Az,
        double Cx, double Cy, double Cz,
        double theta)
{
        //ortogonaliza el vecto C respecto de A
        C = Ortogonalize(A, C);

        //obtiene el sistema de versores
        TVector3D u = A/Mod(A);
        TVector3D w = C/Mod(C);
        TVector3D v = Prod(w, u);

        //calcula u_ respecto de (u, v, w)
        double cost = cos(theta);
        double sint = sin(theta);

        //determina u_ respecto de (i, j, k)
        u_x = cost*u.x + sent*vx;
        u_y = cost*u.y + sent*vy;
        u_z = cost*u.z + sent*vz;
}                   */

//determina la distancia de un punto a un plano en el espacio 3D
double DistancePointPlane(double x, double y, double z,
        double A, double B, double C, double D)
{
        double m = sqrt(A*A + B*B + C*C);

        //el vector director no debería ser nulo
        if(m <= 0)
                throw EImproperArgument("vector director (A, B, C) should not be null");

        //calcula la distancia
        return (A*x + B*y + C*z - D)/m;
}

//---------------------------------------------------------------------------

} //namespace Mathematics

//---------------------------------------------------------------------------

