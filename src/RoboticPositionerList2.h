//---------------------------------------------------------------------------
//Archivo: RoboticPositionerList2.h
//Contenido: lista de posicionadores con mapeo hash
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ROBOTICPOSITIONERLIST2_H
#define ROBOTICPOSITIONERLIST2_H

#include "RoboticPositionerList1.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//###########################################################################
//TRoboticPositionerList2
//###########################################################################

//clase mapa de posicionadores
class TRoboticPositionerList2 : public TRoboticPositionerList1 {
protected:
    //INTERVALO DE MAPEO:

    double __xmin;
    double __xmax;
    double __ymin;
    double __ymax;

    //CELDAS DEL MAPA:

    //celdas de la retícula del mapa
    TPointersList<TPointersList<TItemsList<TRoboticPositioner*> > > Cells;

    int __J;

    //PARÁMETROS DERIVADOS:

    double __qx;
    double __qy;

    //PARÁMETROS DE CONFIGURACIÓN EN FORMATO TEXTO:

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    //PROPIEDADES GRÁFICAS:

    bool __PaintMap;

    //MÉTODOS DE ASIMILACIÓN:

    //calcula el tamaño de una celda (qx, qy)
    void CalculateSize(void);

public:
    //INTERVALO DE MAPEO:

    //límite inferior de la abcisa del intervalo de mapeo
    //debe ser menor que el límite superior
    //valor por defecto: -106.08 mm
    double getxmin(void) const {return __xmin;}
    void setxmin(double);
    //límite superior de la abcisa del intervalo de mapeo
    //debe ser mayor que el límite inferior
    //valor por defecto: 106.08 mm
    double getxmax(void) const {return __xmax;}
    void setxmax(double);
    //límite inferior de la ordenada del intervalo de mapeo
    //debe ser menor que el límite superior
    //valor por defecto: -102 mm
    double getymin(void) const {return __ymin;}
    void setymin(double);
    //límite superior de la ordenada del intervalo de mapeo
    //debe ser mayor que el límite inferior
    //valor por defecto: 102 mm
    double getymax(void) const {return __ymax;}
    void setymax(double);

    //ADVERTENCIA: el intervalo de mapeo debe contener completamente
    //al plano focal y no necesariamente el dominio completo
    //del conjunto de posicionadores, ya que solo se habrán de procesar
    //los puntos dentro del plano focal.

    //CELDAS DEL MAPA:

    //número de filas de la retícula
    //debe ser mayor que cero
    //valor por defecto: 50
    int getI(void) const {return Cells.getCount();} void setI(int);
    //número de columnas de la retícula
    //debe ser mayor que cero
    //valor por defecto: 52
    int getJ(void) const {return __J;} void setJ(int);

    //CONSEJO: las dimensiones de la matriz de celdas deberían
    //ser tales que la diagonal de cada celda no supere L,
    //de modo que el lado de cada celda debería ser a lo sumo:
    //      qmax = L/sqrt(2.) - ERR_NUM,
    //ya que:
    //      - por debajo de L el número máximo de posicionadores
    //        por celda es 3;
    //      - cuanto menor sea el tamaño de la celda, más probable
    //        es que un punto del área exclusiva esté en una celda
    //        con un solo posicionador.

    //PARÁMETROS DERIVADOS:

    //base de una celda
    //      qx = (xmax - xmin)/J
    //valor por defecto: 4.08 mm
    double getqx(void) const {return __qx;}
    //altura de una celda
    //      qy = (ymax - ymin)/I
    //valor por defecto: 4.08 mm
    double getqy(void) const {return __qy;}

    //PROPIEDAES EN FORMATO TEXTO:

    AnsiString getxminText(void) const;
    void setxminText(const AnsiString&);
    AnsiString getxmaxText(void) const;
    void setxmaxText(const AnsiString&);
    AnsiString getyminText(void) const;
    void setyminText(const AnsiString&);
    AnsiString getymaxText(void) const;
    void setymaxText(const AnsiString&);

    AnsiString getIText(void) const;
    void setIText(const AnsiString&);
    AnsiString getJText(void) const;
    void setJText(const AnsiString&);

    AnsiString getqxText(void) const;
    AnsiString getqyText(void) const;

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    //propiedades de configuración
    //en formato asignaciones de texto
    AnsiString getIntervalText(void) const;
    void setIntervalText(const AnsiString&);
    //celdas del mapa
    //en formato asignaciones de texto
    AnsiString getCellsText(void) const;
    void setCellsText(const AnsiString&);
    //tamaño de celda
    //en formato asignaciones de texto
    AnsiString getSizeText(void);

    //instancia del mapa del posicionador
    //en formato de asignaciones
    AnsiString getInstanceMapText(void) const;
    void setInstanceMapText(const AnsiString&);

    //PROPIEDADES GRÁFICAS:

    //color de la regilla
    //valor por defecto: clGreen
    //#QColor Color;

    //-------------------------------------------------------------------
    //MÉTODOS ESTÁTICOS DE LECTURA DE PROPIEDADES INDIVIDUALES:

    //lee xmin de una asignación
    static void  ReadxminText(double &xmin, const AnsiString &S,
                                        int &i);
    //lee xmax de una asignación
    static void  ReadxmaxText(double &xmax, const AnsiString &S,
                                        int &i);
    //lee ymin de una asignación
    static void  ReadyminText(double &ymin, const AnsiString &S,
                                        int &i);
    //lee ymax de una asignación
    static void  ReadymaxText(double &ymax, const AnsiString &S,
                                        int &i);

    //lee I de una asignación
    static void  ReadIText(int &I,  const AnsiString &S,
                                     int &i);
    //lee J de una asignación
    static void  ReadJText(int &J, const AnsiString &S,
                                     int &i);

    //MÉTODOS ESTÁTICOS DE CONJUNTOS DE PROPIEDADES:

    //imprime el intervalo de mapeo de un mapa de posicionadores
    //en una cadena de texto en formato de asignaciones
    static void  PrintInterval(AnsiString &S,
                                         const TRoboticPositionerList2 *RPL);
    //le el intervalo de mapeo de un mapa de posicionadores
    //en una cadena de texto en formato de asignaciones
    static void  ReadInterval(TRoboticPositionerList2 *RPL,
                                        const AnsiString &S, int &i);

    //imprime las celdas de un mapa de posicionadores
    //en una cadena de texto en formato de asignaciones
    static void  PrintCells(AnsiString &S,
                                      const TRoboticPositionerList2 *RPL);
    //lee las celdas de un mapa de posicionadores
    //en una cadena de texto en formato de asignaciones
    static void  ReadCells(TRoboticPositionerList2 *RPL,
                                     const AnsiString &S, int &i);

    //MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

    //construye unalista de posicionadores
    TRoboticPositionerList2(void);//TRoboticPositionerList *_ListRoboticPositioners);

    //copia la matriz de celdas
    void CopyCells(const TRoboticPositionerList2*);
    //copia el mapa de posicionadores de una RPL
    void CopyMap(const TRoboticPositionerList2*);

    //clona un lista de posicionadores
    //      (Tolerance, Sizing, Area, Graphics, Map)
    void Clone(const TRoboticPositionerList2*);

    //contruye un clon de una lista de posicionadores
    TRoboticPositionerList2(const TRoboticPositionerList2*);

    //MÉTODOS DEL MAPA:

    //establece los límites del intervalo de mapeo
    void SetMapInterval(double _xmin, double _xmax, double _ymin, double _ymax);
    //establece las dimensiones de la matriz de celdas
    void SetMapDimensions(int _I, int _J);
    //establece el contenido de las celdas
    void SetMapCells(TPointersList<TPointersList<TItemsList<TRoboticPositioner*> > > &_Cells);

    //mapea los posicionadores en el intervalo de mapeo
    void Map(void);
    //automáticamente:
    //      1. establece los límites apropiados del intervalo de mapeo
    //      2. establece las dimensiones apropieadas de la matriz de celda
    //      3. mapea los posicionadores en el intervalo de mapeo
    void GenerateMap(void);

    //transforma x en j por defecto
    //      x = floor((x - xmin)/qx)
    int jfloor(double x);
    //transforma y en i por defecto
    //      y = floor((y - ymin)/qy)
    int ifloor(double y);
    //transforma x en j por exceso
    //      x = ceil((x - xmin)/qx)
    int jceil(double x);
    //transforma y en i por exceso
    //      y = ceil((y - ymin)/qy)
    int iceil(double y);

    //accede a la lista de posicionadores de la célula que contiene un punto
    TItemsList<TRoboticPositioner*> *RoboticPositionersCell(TDoublePoint P);

    //busca los posicionadores en cuyos dominios de P3
    //se encuentre un punto determinado
    void SearchPositioners(TItemsList<TRoboticPositioner*> &RPs,
                           TDoublePoint P);

    //MÉTODOS GRÁFICOS:

    //dibuja el mapa en un trazador de formas
    //#void PaintMap(TPloterShapes*);

    //dibuja la lista de posicioandores en un trazador de formas
    //#void Paint(TPloterShapes*);

};

//Utilización:
//      //contruye el mapa
//      TRoboticPositionerList *RPL = new TRoboticPositionerList(RPL);
//      //ajusta los límites del mapa (ya realizado por defecto para SIDE)
//      RPL->SetLimits(-RPL->R, RPL->R, -RPL->R, RPL->R);
//      //redimensiona la matriz de celdas (ya realizado por defecto para SIDE)
//      RPL->SetDimensions(floor(2*RPL->R/(L/2)), floor(2*RPL->R/(L/2)));
//      //mapea los posicionadores de la lista
//      RPL->Map(RPL);
//      //determina los posicionadores en cuyos cominios está un punto
//      RPL->SearchPositioners(RPs, P);
//      //destruye el mapa
//      delete RPL;

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // ROBOTICPOSITIONERLIST2_H
