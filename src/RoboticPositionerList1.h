//---------------------------------------------------------------------------
//Archivo: RoboticPositionerList1.h
//Contenido: lista de posicionadores con propiedades dimensionales
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ROBOTICPOSITIONERLIST1_H
#define ROBOTICPOSITIONERLIST1_H

#include "ExclusionArea.h"
#include "PointersList.h"

//---------------------------------------------------------------------------

using namespace Lists;

//espacio de nombres de modelos
namespace Models {

//###########################################################################
//Métodos de TRoboticPositionerList1:
//###########################################################################

//clase lista de posicionadores
class TRoboticPositionerList1 : public TItemsList<TRoboticPositioner*> {
protected:
        //TOLERANCIAS:

        double __PAem;
        double __Pem;

        //PROPIEDADES DE DIMENSIONAMIENTO:

        TDoublePoint __O;

        double __LO3max;

        double __x3min;
        double __x3max;
        double __y3min;
        double __y3max;

        //PROPIEDADES DE ÁREA:

        double __Spt;
        double __Set;
        double __Ret;
        double __FiberDensity;

        //###################################################################
        //PROPIEDADES EN FORMATO TEXTO
        //###################################################################

        //PROPIEDADES COMPUESTAS EN FORMATO TEXTO:

        //TOLERANCIAS EN FORMATO TEXTO:

        //PROPIEDADES DE DIMENSIONAMIENTO EN FORMATO TEXTO:

        //PROPIEDADES DE ÁREA EN FORMATO TEXTO:

        //PROPIEDADES GRÁFICAS EN FORMATO TEXTO:

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

public:
        //PROPIEDADES COMPUESTAS:

/*        //lista de posicionadores
        TPointersList<TRoboticPositioner> RoboticPositioners;
        //lista de áreas de exclusión
        TPointersList<TRoboticPositioner> ExclusionAreas;
  */
        //PROPIEDADES DE SEGURIDAD:

        //margen de error de orientación de S0.
        //debe ser no negativo
        //valor por defecto: MEGARA_PAem rad
        double getPAem(void) const {return __PAem;}
        void setPAem(double);
        //margen de error de apuntado de S0.
        //debe ser no negativo
        //valor por defecto: MEGARA_Pem mm
        double getPem(void) const {return __Pem;}
        void setPem(double);

        //Los márgenes de tolerancia conjunta del Fiber MOS
        //deben poder ser iguales a cero, para permitir desplazar
        //todo el error a los márgenes de los RPs individuales.
        //Por otra parte, podría desearse que el PAem absorbiera
        //el error de orientación del Fiber MOS, debido al elevado
        //peso de la rueda que lo sostiene.

        //Los márgenes de seguridad serán tenidas en cuenta en el cálculo
        //de las componentes del SPM de cada posicionador. Para facilitar
        //este proceso se definen dos métodos en la lista de posicionadores:
        //      void CalculateSPMComponents(TRoboticPositioner&);
        //      void CalculateSPMComponents();
        //Ý los métodos de cálculo de las componentes de SPM en elposicionador:
        //      double SPMrec(void) const;
        //      double SPMsta(void) const;
        //      double SPMdyn(void) const;
        //      double SPMoff(double PAem, double Pem) const;

        //PROPIEDADES DE DIMENSIONAMIENTO:

        //posición del origen del sistema S0
        //valor constante: (0, 0)
        TDoublePoint getO(void) const {return __O;}

        //radio del círculo mínimo que contiene
        //el dominio de todos los posicionadores
        double getLO3max(void) const {return __LO3max;}

        //límite abcisa inferior del cuadrado que contiene
        //el dominio conjunto de P3
        double getx3min(void) const {return __x3min;}
        //límite abcisa superior del cuadrado que contiene
        //el dominio conjunto de P3
        double getx3max(void) const {return __x3max;}

        //límite ordenada inferior del cuadrado que contiene
        //el dominio conjunto de P3
        double gety3min(void) const {return __y3min;}
        //límite ordenada superior del cuadrado que contiene
        //el dominio conjunto de P3
        double gety3max(void) const {return __y3max;}

        //PROPIEDADES DE ÁREA:

       //NOTA: los parámetros Sp, Se y Re son calculados analíticamente
       //para el caso en que los posicionadores adyacentes cumplen:
       //       radio del dominio igual r_3max;
       //       equidistantes 4*L*cos(M_PI/6).

       //área de participación del conjunto de posicionadores
        //      Spt = Sum{Spt[i]}
        double getSpt(void) const {return __Spt;}

        //área exclusiva del conjunto de posicionadores
        //      Set = Sum{Set[i]}
        double getSet(void) const {return __Set;}

        //proporción de área exclusiva del conjunto de posicionadores
        //      Ret = Sum{Ret[i]*Spt[i]}/Spt
        double getRet(void) const {return __Ret;}

        //densidad de posicionadores de la lista
        //número de posicionadores por unidad de superficie
        //      FiberDensity = __property double(RoboticPositioners.Count)/Spt
        double getFiberDensity(void) const {return __FiberDensity;}

        //ADVERTENCIA: cuando cambia el radio del dominio del posicionador
        //o de alguno de los adyacentes, o la distancia entre posicionadores
        //no es igual a 4*L*cos(M_PI/6), los valores Spt, Set y Ret no
        //se corresponderán con la realidad.

        //ADVERTENCIA: cuando r_3min!=0 se tiene que parte del área excusiva
        //no forma parte del dominio del posicioandor, con lo que habría que
        //distinguir dos tipos de áreas exclusivas:
        //      área exclusiva total;
        //      área exclusiva accesible.

        //PROPIEDADES GRÁFICAS:

        //color del límite del dominio conjunto
        //valor por defecto: clGray
        //#QColor LimitDomainColor;

        //indica si debe dibujar los actuadores
        //valor por defecto: true
        bool PaintActuators_;
        //interruptor de dibujo del límite del dominio
        //circular conjunto de los posicionadores
        //valor por defecto: false
        bool PaintLimitDomain_;
        //indica si debe dibujar el mapa
        //valor por defecto: false
        bool PaintMap_;

        //###################################################################
        //PROPIEDADES EN FORMATO TEXTO
        //###################################################################

        //PROPIEDADES COMPUESTAS EN FORMATO TEXTO:


        //dirección en memoria de la lista de elementos
        AnsiString getItemsAddressText(void) const {
                return IntToHex(reinterpret_cast<intptr_t>(&Items), 8);}
/*        //dirección en memoria de la lista de posicionadores
        AnsiString getRoboticPositionersAddressText(void) const {
                return IntToHex(reinterpret_cast<intptr_t>(&RoboticPositioners), 8);}
        //dirección en memoria de la lista de áreas de exclusión
        AnsiString getExclusionAreasAddressText(void) const {
                return IntToHex(reinterpret_cast<intptr_t>(&ExclusionAreas), 8);}
  */
        //TOLERANCIAS EN FORMATO TEXTO:

        //TOLERANCIAS EN FORMATO TEXTO:

        AnsiString getPAemText(void) const; void setPAemText(const AnsiString&);
        AnsiString getPemText(void) const; void setPemText(const AnsiString&);

        //PROPIEDADES DE DIMENSIONAMIENTO EN FORMATO TEXTO:

        AnsiString getOText(void) const;

        AnsiString getLO3maxText(void) const;

        AnsiString getx3minText(void) const;
        AnsiString getx3maxText(void) const;
        AnsiString gety3minText(void) const;
        AnsiString gety3maxText(void) const;

        //PROPIEDADES DE ÁREA EN FORMATO TEXTO:

        AnsiString getSptText(void) const;
        AnsiString getSetText(void) const;
        AnsiString getRetText(void) const;
        AnsiString getFiberDensityText(void) const;

        //PROPIEDADES GRÁFICAS EN FORMATO TEXTO:

        AnsiString getLimitDomainColorText(void) const;
        void setLimitDomainColorText(const AnsiString&);

        AnsiString getPaintActuators_Text(void) const;
        void setPaintActuators_Text(const AnsiString&);
        AnsiString getPaintLimitDomain_Text(void) const;
        void setPaintLimitDomain_Text(const AnsiString&);
        AnsiString getPaintMap_Text(void) const;
        void setPaintMap_Text(const AnsiString&);

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //tabla de orígenes de coordenadas:
        //      Id      x0      y0      thetaO1
        AnsiString getOriginsTableText(void) const;
        void setOriginsTableText(const AnsiString&);
        //tabla de coordenadas de posición de P3:
        //      Id      x3      y3
        AnsiString getPositionsP3TableText(void) const;
        void setPositionsP3TableText(const AnsiString&);
        //tabla de PAPs:
        //      Id      p_1     p___3
        AnsiString getPositionsPAPTableText(void) const;

        //conjunto de todas las propiedades de seguridad
        //en formato asignaciones
        AnsiString getToleranceText(void) const;
        //conjunto de propiedades de dimensionamiento
        //en formato asignaciones
        AnsiString getSizingText(void) const;
        //conjunto de propiedades de área
        //en formato asignaciones
        AnsiString getAreaText(void) const;
        //conjunto de propiedades de gráficas
        //en formato asignaciones
        AnsiString getGraphicsText(void) const;

        //conjunto de todas las propiedades
        //en formato de asignaciones
        AnsiString getAllText(void) const;
        //instancia de la lista de posicionadores
        //en formato de asignaciones
        AnsiString getInstanceText(void) const;
        void setInstanceText(const AnsiString&);

        //-------------------------------------------------------------------
        //MÉTODOS ESTÁTICOS:

        //lee una instancia en una cadena
        static void  ReadInstance(TRoboticPositionerList1* &RPL,
                const AnsiString& S, int &i);

        //-------------------------------------------------------------------
        //MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN

        //construye una lista de posicionadores
        TRoboticPositionerList1(void);

        //copia las propiedades de tolerancia
        //      (PAem, Pem)
        void CopyTolerance(const TRoboticPositionerList1*);
        //copia las propiedades de dimensionamiento
        //      (LO3, x3min, y3min, x3max, y3max)
        void CopySizing(const TRoboticPositionerList1*);
        //copia las propiedades de área
        //      (Spt, Set, Ret, FiberDensity)
        void CopyArea(const TRoboticPositionerList1*);
        //copia las propiedades gráficas
        //      (LimitDomainColor, PaintActuators_, PaintLimitDomain_, PaintMap)
        void CopyGraphics(const TRoboticPositionerList1*);

        //clona un lista de posicionadores
        //      (Tolerance, Sizing, Area, Graphics)
        void Clone(const TRoboticPositionerList1*);

        //construye un clon de una lista de posicionadores
        TRoboticPositionerList1(const TRoboticPositionerList1*);

        //ADVERTENCIA: la clonación de una lista de posicionadores causará
        //la construcción de posicionadores homólogos en la lista clon.

        //ADVERTENCIA: la destrucción de la lista de posicionadores causará
        //la destrucción de los posicionadores de la lista. Si lo que desea
        //es destruir la lista de posicionadores sin destruir los posicionadores
        //deberá vaciarla antes con el método: ClearWithoutDestroy.

        //AÑADIR Y BORAR POSICIONADORES:

        //Añadir un posicionador a la lista:
        //      Add(RP);
        //Destruye y borra un posicionador de la lista
        //      Delete(i);
        //Borra un posicionador de la lista sin destruirlo
        //      DeleteWithoutDestroy(i);

        //MÉTODOS DE CONSTRUCCIÓN DE POSICIONADORES:

        //construye la capa n de posicionadores equidistantes una distancia D
        //donde la capa 0 representa el posicionador central
        void BuildLayer(int &CountId, int n, double D);

        //construye una colmena circular de posicionadores
        //con N capas (más el central) y destruye
        //los posicionadores fuera del radio R
        void BuildCircularHive(int &CountId,
                double D=MEGARA_D, int N=19, double R=500);
        //construye una colmena cuadrada de posicionadores
        //con N capas
        void BuildSquareHive(int &CountId,
                double D=MEGARA_D, int M=5, int N=4);

        //Valores recomendados para BuildCircularHive:
        //      SIDE 85: N=5 pos; R=151 mm;
        //      SIDE 1003: N=19 pos; R=500 mm;
        //Valores recomendados para BuildquareHive:
        //      MEGARA 104: M=5 pos; N=4 pos;

        //MÉTODOS DE DESTRUCCION DE POSICIONADORES:

        //detruye el posicionador indicado de la lista
        void Destroy(int i);
        //destruye todos los posicionadores de la lista
        void Destroy(void);
        //destruye los posicionadores seleccionados de la lista
        int DestroySelected(void);
        //detruye los posicionadores que se encuentren a
        //una distancia del centro mayor que la indicada
        void Destroy(double rmax);

        //MÉTODOS DE BÚSQUEDA DE POSICIONADORES:

        //busca un posicionador en la lista
        int Search(TRoboticPositioner *RP) const;
        int Search(TActuator *A) const;
        //busca el primer posicionador con el identificador indicado
        int SearchId(int Id) const;
        //devuelve el puntero al primer posicionador
        //con el identificador indicado
        const TRoboticPositioner *SearchIdPointer(int Id) const;

        //busca el primer posicionador de la lista
        //en cuyo dominio de P3 se encuentra un punto
        int SearchDomainP3(TDoublePoint P);
        int SearchDomainP3(double x, double y);

        //busca los posicionadores de la lista que tienen
        //algún cuantificador desactivado
        void SearchDisabledQuantificators(TVector<int> &indices);

        //------------------------------------------------------------------
        //MÉTODOS DE ASIMILACIÓN:

        //A partir de:
        //      (Items[i]->Eo, Items[i]->Ep, Items[i]->Tstop_em, Items[i]->Tshif_em, PAem, Pem)
        //      {Items[i]->rmax}
        //Obtiene y asigna a cada posicionador de la lista:
        //      (Items[i]->SPMrec, Items[i]->SPMsta, Items[i]->SPMdyn, Items[i]->SPMmin, Items[i]->SPMoff)
        void CalculateSPMComponents(void);

        //El método CalculateSPMComponents() y los sucesivos
        //deberán ser invocados toda vez que los márgenes de seguridad
        //sean modificados.

        //determina los posicionadores que están lo bastante cerca
        //de cada posicionador como para invadir su dominio de maniobra
        void DetermineAdjacents(void);
        //ordena las listas de posicionadores adyacentes en
        //sentido levógiro empezando por el más próximo a 0
        void SortAdjacents(void);

        //El método DetermineAdjacents() y los sucesivos
        //deberán ser invocados toda vez que se añadan o borren posicionadores,
        //o se modifique algún parámetro de posicionamiento o dimensionamiento,
        //que pudiera afectar a la adyacencia de algún posicionador.

        //calcula las propiedades de seguridad
        //paracada posicionador de la lista:
        //      {r_min, r_saf, r_2saf, theta___2saf, theta_2rad}
        void CalculateSafeParameters(void);

        //el método CalculateSafeParameters() debe ser invicado
        //toda vez que se invoque el método DetermineAdjacets()
        //o se cambie algún parámetro de dimensionameinto o posicionamiento
        //de algún cilindro adyacente

        //la invocación a CalculateSafeParámeters() no es automática
        //quedando bajo responsabilidad del programador su invocación
        //ya que los posicionadores son accesibles individualmente
        //y por consiguientes sus parámetros, de todos modos,
        //podrían ser cambiados de manera inadvertida
        //y definir una función de acceso para cada parámetro
        //de cada posicionador sería demasiado engorroso

        //Los métodos de asimilación:
        //      Deben ser aplicados despues de contruir, destruir posicionadores
        //      o modificar la geometría de alguno de ellos.
        //
        //      Son computacionalmente costosos, de modo que no serán invocados
        //      automáticamente al ajustar cada parámetro independiente,
        //      si no que deben ser invocados manualmente una sola vez
        //      despues de ajustar todos los parámetros.

        //calcula las propiedades de dimensionamiento:
        //      (LO3max, x3min, x3max, y3min, y3max)
        void CalculateSizingParameters(void);

        //calcula todos los parámetros de dimensionamiento derivados
        //      (Spt, Set, Ret, FiberDensity)
        void CalculateAreaParameters(void);

        //invoca a todos los métodos de asimilación de
        //los parámetros de dimensionamiento:
        //      DetermineAdjacents
        //      SortAdjacents
        //      CalculateSafeParameters();
        //      CalculateSizingParameters();
        //      CalculateAreaParameters
        void AssimilateSizing(void);

        //asimila la configurración de posicionadores dada ejecutando:
        //      CalculateSPMComponents();
        //      AssimilateSizing();
        void Assimilate(void);

        //MÉTODOS DE LECTURA CONJUNTA:

        //obtiene las posiciones angulares de los ejes
        //de los posicionadores en radianes
        void GetPositions(TPointersList<TPair>& PositionList);

        //MÉTODOS DE ASIGNACIÓN CONJUNTA:

/*        //asigna una lista de posiciondores
        void SetRoboticPositioners(const TPointersList<TRoboticPositioner>&);*/
        //asigna las posiciones angulares de los ejes
        //este método es atómico
        void SetPositions(TPointersList<TPair>& PositionList);
        //asigna conjuntamente las tolerancias
        //      (PAem, Pem)
        void SetTolerance(double _PAem,double _Pem);

        //------------------------------------------------------------------
        //MÉTODOS DE DOMINIO CONJUNTO:

        //determina el intervalo cartesiano que contiene
        //el dominio de todos los posicionadores
        //si el número de posicionadores de la lista es menor que uno
        //lanza una excepcion EImproperCall
        void GetDomainP3s(double &x3min, double &x3max,
                double &y3min, double &y3max);

        //determina si un punto se encuentra dentro del círculo
        //que contiene el dominio conjunto de los posicionadores
        bool IsInCircle(const TDoublePoint&);
        //determina si un punto se encuentra dentro del cuadrado
        //que contiene el dominio conjunto de los posicionadores
        bool IsInSquare(const TDoublePoint&);

        //MÉTODOS DE DESPLAZAMIENTO CONJUNTO:

        //mueve los ejes de los posicionadores
        //a sus orígenes de coordenadas
        void MoveToOrigins(void);
        //mueve los ejes de los posicionadores seleccionados
        //a sus orígenes de coordenadas
        int MoveToOriginsSelected(void);

        //mueve los brazos que están fuera del área de seguridad a
        //las posiciones de seguridad estables más próximas
        void MoveOutsideArmsToSafePositions(void);
        //mueve los brazos de los posicionadores seleccionados que
        //están fuera del área de seguridad a las posiciones de
        //seguridad estables más próximas
        int MoveOutsideArmsToSafePositionsSelected(void);

        //MÉTODOS DE SEGREGACIÓN DE POSICIONADORES:

        //segrega los posicionadores en dos listas:
        //      lista con el brazo dentro del área de seguridad;
        //      lista con el brazo fuera del área de seguridad;
        void SegregateInOut(TRoboticPositionerList1 &Inners,
                TRoboticPositionerList1 &Outsiders);

        //segrega losposicionadores seleccionados en una lista
        void SegregateSelected(TRoboticPositionerList1&);

        //MÉTODOS DE PILA DE POSICIONES ANGULARES:

        //apila las posiciones de cada posicionador de la lista
        void PushPositions(void);
        //restaura la siguiente posición apilada de cada posicionador
        //sin desempilarla
        void RestorePositions(void);
        //desempila la siguiente posición apilada de cada posicionador
        void PopPositions(void);
        //restaura y desempila la siguiente posición apilada de cad aposicionador
        void RestoreAndPopPositions(void);

        //apila la posición de los brazos de todos los posicionadores
        void PushArmtheta___3s();
        //restaura la últimaposición apilada del brazo de cada posicionador
        void RestoreArmtheta___3s(void);
        //restaura la siguiente posición apilada del brazo de cada posicionador
        void PopArmtheta___3s(void);

        //MÉTODOS DE PILA DE CUANTIFICADORES:

        //apila el estado de
        //los cuantificadores de cada posicionador de la lista
        void PushQuantifys(void);
        //restaura el último estado apilado de
        //los cuantificadores de cada posicionador de la lista
        void RestoreQuantifys(void);
        //desempila el último estado apilado de
        //los cuantificadores de cada posicionador de la lista
        void PopQuantifys(void);
        //restaura y desempila el último estado apilado de
        //los cuantificadores de cada posicionador de la lista
        void RestoreAndPopQuantifys(void);

        //apila el estado
        //del cuantificador del brazo de cada posicionador de la lista
        void PushArmQuantify___s(void);
        //restaura el último estado apilado
        //del cuantificador del brazo de cada posicionador de la lista
        void RestoreArmQuantify___s(void);
        //desempila el último estado apilado
        //del cuantificador del brazo de cada posicionador de la lista
        void PopArmQuantify___s(void);
        //restaura y desempila el último estado apilado
        //del cuantificador del brazo de cada posicionador de la lista
        void RestoreAndPopArmQuantify___s(void);

        //------------------------------------------------------------------
        //MÉTODOS PARA DETERMINAR SI HAY COLISIONES:

        //levanta las banderas indicadoras de determinación de colisión
        //pendiente de todos los posicionadores de la lista
        void EnablePending(void);

        //determina si algún brazo de algún posicionador
        //colisiona con el brazo de algún posicionador adyacente
        bool ThereIsCollision(void);
        //busca los posicionadores de la lista cuyo brazo colisiona con
        //el brazo de algún otro posicionador adyacente
        void SearchCollinding(TVector<int> &indices);
        //obtiene los conjuntos de posicionadores en colisión en la exposición indicada
        void GetCollisionClusterList(TPointersList<TItemsList<TRoboticPositioner*> > &CCL);

        //MÉTODOS RANDOMIZADO DE LOS EJES:

        //lleva los ejes de los posicionadores a posiciones aleatorias
        //con distribución uniforme en sus dominios
        void Randomize(void);
        //lleva los ejes de los posicionadores seleccionados a
        //posiciones aleatorias con distribución uniforme en sus dominios
        int RandomizeSelected(void);

        //lleva los ejes de los posicionadores a posiciones aleatorias
        //con distribución uniforme en sus dominios
        //en las que no colisionan entre si
        void RandomizeWithoutCollision(void);
        //lleva los ejes de los posicionadores seleccionados a
        //posiciones aleatorias con distribución uniforme en
        //sus dominios en las que no colisionan entre si
        int RandomizeWithoutCollisionSelected(void);

        //MÉTODOS DE RANDOMIZADO DE P3:

        //lleva el punto P3 de los posicionadores a posiciones aleatorias
        //con distribución uniforme en su dominio
        void RandomizeP3(void);
        //lleva el punto P3 de los posicionadores seleccionados a
        //posiciones aleatorias con distribución uniforme en su dominio
        int RandomizeP3Selected(void);

        //lleva el punto P3 de los posicionadores a posiciones aleatorias
        //con distribución uniforme en su dominio
        //en las que no colisionan entre si
        void RandomizeP3WithoutCollision(void);
        //lleva el punto P3 de los posicionadores seleccioandos a
        //posiciones aleatorias con distribución uniforme en su dominio
        //en las que no colisionan entre si
        int RandomizeP3WithoutCollisionSelected(void);

        //------------------------------------------------------------------
        //MÉTODOS GRÁFICOS:

        //establece el estado de la bandera de pintado
        //del cuerpo de los posicionadores
        void SetPaintBody(bool);
        //establece el estado de la bandera de pintado
        //del límite del dominio del punto P3 de los posicionadores
        void SetPaintLimitDomainP3(bool);
        //establece el estado de la bandera de pintado
        //del límite del dominio de maniobra de los posicionadores
        void SetPaintLimitDomainManeuvering(bool);

        //asigna un color a todos los posicionadores de la lista
//#        void SetAllColors(QColor Color);
        //selecciona todos los posicionadores de la lista
        void SelectAll(void);
        //deselecciona todos los posicionadores de la lista
        void DeselectAll(void);

        //imprime la lista de posicionadores en el lienzo de la fotografía
        //con los colores indicados
        //imprime el lienzo de la fotografía en el lienzo de la caja de pintura
        //#void PaintActuators(TPloterShapes *PS);
        //imprime la lista de posicionadores en el lienzo de la fotografía
        //con los colores indicados en el modelo simplificado
        //imprime el lienzo de la fotografía en el lienzo de la caja de pintura
        //#void PaintActuatorsSimplified(TPloterShapes *PS);
        //dibuja el límite circular del dominio conjunto de los posicionadores
        //#void PaintLimitDomain(TPloterShapes *PS);

        //busca el primer posicionador que está bajo el punto P
        //y que parte del posicionador puede ser agarrado en
        //el punto indicado:
        //      i: índice al posicionador bajo el punto P;
        //      dominio i: [0, Count];
        //      n: parte del posicionador agarrada;
        //      valores posibles:
        //              0: niguna;
        //              1: cilindro (P0, L01);
        //              2: brazo (PA..PH);
        //              3: lente (P2, R2)
        //valores de retorno:
        //      false: ninguna parte agarrada
        //      true: alguna parte agarrada
        bool Grab(int &i, int &n, TDoublePoint P);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // ROBOTICPOSITIONERLIST1_H
