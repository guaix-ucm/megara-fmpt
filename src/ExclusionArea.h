//---------------------------------------------------------------------------
//Archivo: ExclusionArea.h
//Contenido: área de exclusión compuesta por barrera, posición y orientación
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef EXCLUSIONAREA_H
#define EXCLUSIONAREA_H

#include "RoboticPositioner.h"
#include "..\0_VCL\vclemu.h"
#include "..\3_Lists\ItemsList.h"
#include "..\A_Arm\Barrier.h"

//Para determinar el estado de colisión de un EA con un RP adyacente,
//se utilizará el método:
//      Barrier.Contour.Collides(A->Arm->Contour, Barrier.SPM + A->Arm->SPM)
//de modo que hay que acceder a algunas propiedades del RP adyacente;
//por eso aquí no basta predeclarar la clase TRoboticPositioner.

//---------------------------------------------------------------------------

//using namespace MoveFunctions;
using namespace Lists;

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TExclusionArea
//---------------------------------------------------------------------------

//clase área de exclusión
class TExclusionArea {
        //PROPIEDADES DEL ÁREA DE EXCLUSIÓN:

        double __Eo;
        double __Ep;

        int __Id;

        //PROPIEDADES EN FORMATO TEXTO:


        //PROPIEDADES GRÁFICAS EN FORMATO TEXTO:


        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:


public:
        //PROPIEDADES ESTÁTICAS:

        //lista de objetos construidos
        static TItemsList<TExclusionArea*> Builts;

        //La lista de objetos contruidos es útil para:
        //- determinar los identificadores que se encuentran repetidos;
        //- obtener identificadores unívocos;
        //- controlar la integridad de memoria.

        //El objeto estático Builts debe ser contruido
        //antes de invocar al contructor mediante:
        //      TItemList<TExclusionArea*> TExclusionArea::Builts;
        //para ello esta sentencia deberá ser insertada en el mismo
        //archivo donde se invoca al constructor, antes de la invocación.
        //Si se insertase en este archivo o en su correspondiente .cpp,
        //sería invocado después de llamar al constructor.

        //PROPIEDADES DEL ÁREA DE EXCLUSIÓN:

        //barrera del área de exclusión
        TBarrier Barrier;

        //margen de error de orientación en S0
        //      Valor por defecto: MEGARA_Eo rad
        double getEo(void) const {return __Eo;} void setEo(double);
        //margen de error de posición en S0
        //      Valor por defecto: MEGARA_Ep mm
        double getEp(void) const {return __Ep;} void setEp(double);

        //número de identificación
        //debe ser mayor que cero
        //valor por defecto: 0
        int getId(void) const {return __Id;} void setId(int);
        //lista de posicionadores de RPs lo bastante cerca
        //para que puedan colisionar con la barrera
        //valor por defecto:
        //      Adjacents.Capacity = 6;
        //      Adjacents.Compare = TRoboticPositioner::CompareIds;
        //      Adjacents.Print = TRoboticPositioner::PrintId;
        TItemsList<TRoboticPositioner*> Adjacents;

        //indica si la barrera del EA será tenida en cuenta
        //en la determinación de distancias y colisiones
        //valor por defecto: true
        bool Pending;

        //La propiedad Pending será usada en los métodos:
        //      ThereIsCollisionWithPendingAdjacent
        //      SearchCollindingPendingAdjacent
        //con objeto de evitar aplicar el método de determinación de colisión
        //más de una vez entre cada par de brazos.

        //PROPIEDADES GRÁFICAS:

        //color del EA
        //valor por defecto: clGray
        //#QColor Color;
        //indica si debe dibujarse el EA
        //valor por defecto: true
        bool PaintBody;
        //indica el estado de selección del EA
        //valor por defecto: false
        bool Selected;

        //cuando el EA está seleccionada, Paint la dibuja resaltada

        //PROPIEDADES EN FORMATO TEXTO:

        //dirección en memoria de la barrera
        //en formato texto
        AnsiString getBarrierAddressText(void) const {
                return IntToHex(int(&Barrier), 8);}

        AnsiString getEoText(void) const; void setEoText(const AnsiString&);
        AnsiString getEpText(void) const; void setEpText(const AnsiString&);

        AnsiString getIdText(void) const;
        void setIdText(const AnsiString&);

        //Adjacents.Text

        AnsiString getPendingText(void) const;
        void setPendingText(const AnsiString&);

        //PROPIEDADES GRÁFICAS EN FORMATO TEXTO:

        AnsiString getColorText(void) const;
        void setColorText(const AnsiString&);
        AnsiString getPaintBodyText(void) const;
        void setPaintBodyText(const AnsiString&);
        AnsiString getSelectedText(void) const;
        void setSelectedText(const AnsiString&);

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjunto de todas las propiedades
        //en formato de asignaciones
        AnsiString getAllText(void) const;

        //instancia del posicionador
        //en formato de asignaciones
        AnsiString getInstanceText(void) const;
        void setInstanceText(const AnsiString&);

        //conjunto de propiedades de origen (Id, x0, y0, thetaO1)
        //en formato línea de texto
        //en formato asignaciones de texto
        AnsiString getOriginsText(void) const;

        //###################################################################
        //MÉTODOS PÚBLICOS:
        //###################################################################

        //-------------------------------------------------------------------
        //MÉTODOS ESTÁTICOS:

        //compara los identificadores de dos EAs
        static int __fastcall CompareIds(TExclusionArea *EA1,
                TExclusionArea *EA2);

        //imprime el identificador de un EA
        static void __fastcall PrintId(AnsiString &S, TExclusionArea *EA);

        //Los métodos estáticos:
        //      CompareIds
        //      PrintId
        //serán apuntados en la lista de EAs (del tipo TExclusionAreaList),
        //para permitir su ordenacíon en función de los identificadores
        //y permitir la impresión de los mismos.

        //obtiene las etiquetas de las propiedades de origen
        //en formato fila de texto
        static AnsiString GetOriginsLabelsRow(void);
        //atraviesa las etiquetas de las propiedades de origen
        //en formato fila de texto
        static void __fastcall TravelOriginsLabelsRow(const AnsiString&, int&);

        //imprime los valores de las propiedades de orien de un EA
        //al final de una cadena de texto
        //en formato fila de texto
        static void __fastcall PrintOriginsRow(AnsiString& S,
                TExclusionArea *EA);
        //lee los valores de las propiedades de orien para un EA
        //desde la posición indicada de una cadena de texto
        //en formato fila de texto
        static void __fastcall ReadOriginsRow(TExclusionArea *EA,
                const AnsiString& S, int &i);

        //lee una instancia del EA
        //desde la posición indicada de una cadena de texto
        //en formato de asignaciones
        static void __fastcall ReadInstance(TExclusionArea* &B,
                const AnsiString& S, int &i);

        //-------------------------------------------------------------------
        //MÉTODOS DE CONSTRUCCIÓN,COPIA Y CLONACIÓN:

        //contruye un área de exclusión
        //con los valores por defecto
        TExclusionArea(void);
        //construye un área de exclusión
        //con los valores indicados
        //si el número de identificación es menor que uno
        //      lanza una execepción EImproperArgument
        TExclusionArea(int _Id, TDoublePoint _P0, double _thetaO1=0);

        //ADVERTENCIA: para poder contruir clones de áreas de exclusión
        //la duplicidad de números de identificación está permitida.
        //El control de duplicidad de identificadores debe llevarse
        //en todo caso mediante métodos. Al fin y al cabo en el mundo real
        //será posible configurar dos objetos para que tengan el mismo
        //identificador.

        //copia un área de exclusión
        void Copy(const TExclusionArea *EA);

        //contruye un clon de unárea de exclusión
        TExclusionArea(const TExclusionArea *EA);

        //libera la memoria dinámica y borra el objeto de Builts
        //si el objeto no está en Builts lanza EImproperCall
        ~TExclusionArea();

        //-------------------------------------------------------------------
        //MÉTODOS PARA CALCULAR LOS VALORES RECOMENDADOS DE
        //LAS COMPONENTES DE SPM:

        //Calcula el márgen perimetral de seguridad para evitar:
        //- el error mecánico debido al mecanizado del área de exclusión;
        //- el error mecánico de orientación en el anclaje del área de exclusión;
        //- el error mecánico de posicionamiento en el anclaje del área de exclusión;
        //- el error numérico de las operaciones matemáticas del modelo;
        //Todos los errores serán absorbidos por Eo y Ep
        //en la siguiente fórmula:
        //      SPMsta = Eo*r_max + Ep
        //Donde:
        //      Eo: margen de error de orientación en rad;
        //      Ep: margen de error de posición en mm.
        double SPMsta(void) const;

        //-------------------------------------------------------------------
        //MÉTODOS DE ASIGNACIÓN CONJUNTA:

        //asigna las propiedades de origen
        void SetOrigins(int _Id, double _x0, double _y0, double _thetaO1);

        //asigna conjuntamente las tolerancias
        //      (Eo, Ep)
        void SetTolerances(double _Eo, double _Ep);

        //------------------------------------------------------------------
        //MÉTODOS DE ASIMILACIÓN:

        //A partir de:
        //      (Eo, Ep)
        //      r_max
        //Determina:
        //      (Barrier.SPM)
        void CalculateSPM(void);

        //-------------------------------------------------------------------
        //MÉTODOS DE COLISIÓN:

        //determina los RPs adyacentes que pueden colisionar con la barrera
        void DetermineAdjacents(const TItemsList<TRoboticPositioner*>&);
        //determina los RPs adyacentes en estado de colisión con la barrera
        void SearchCollinding(TItemsList<TRoboticPositioner*>&);

        //------------------------------------------------------------------
        //MÉTODOS PARA DETERMINAR LAS COLISIONES CON RPs ADYACENTES:

        //determina si hay colisión con un actuador adyacente
        bool ThereIsCollision(const TActuator*);
        //determina si hay colisión con un actuador adyacente
        bool ThereIsCollisionWithAdjacent(void);
        //Busca los posicionadores adyacentes cuyo
        //brazo colisiona con la barrera de este área de exclusión.
        void SearchCollindingAdjacent(
                TItemsList<TActuator*> &Collindings);

        //Determina si hay colisión con un actuador adyacente
        //con evalauación de colisión pendiente.
        bool ThereIsCollisionWithPendingAdjacent(void);
        //Busca los posicionadores adyacentes
        //con evalauación de colisión pendiente
        //cuyo brazo colisiona con la barrera de este área de exlusión.
        void SearchCollindingPendingAdjacent(
                TItemsList<TActuator*> &Collindings);

        //------------------------------------------------------------------
        //MÉTODOS DE GRÁFICOS:

        //indica que parte del EA puede ser agarrada en el punto indicado
        //      1: barrera;
        //      0: niguna;
        int Grab(TDoublePoint P);

        //dibuja el EA en un trazador
        //#void Paint(TPloterShapes*);
        //dibuja el EA en un trazador
        //#void PaintSimplified(TPloterShapes*);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // EXCLUSIONAREA_H
