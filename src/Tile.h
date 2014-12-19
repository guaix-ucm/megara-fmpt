//---------------------------------------------------------------------------
//Archivo: Tile.h
//Contenido: axulejo
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef TILE_H
#define TILE_H

#include "SkyPoint.h"
#include "PointersList.h"

//#include <dir.h>
#include <sys/stat.h>

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TTile
//---------------------------------------------------------------------------

//azulejo
class TTile {
    int __Id;
    double __DEC;
    double __R_;

public:
    //PROPIEDADES ESTÁTICAS:

    //etiqueta de la propiedad Id
    //debe ser imprimible
    //valor por defecto: "Id"
    static AnsiString IdLabel;

    //etiqueta de la propiedad RA
    //debe ser imprimible
    //valor por defecto: "RA"
    static AnsiString RALabel;
    //etiqueta de la propiedad DEC
    //debe ser imprimible
    //valor por defecto: "DEC"
    static AnsiString DECLabel;

    //etiqueta de la propiedad R
    //debe ser imprimible
    //valor por defecto: "R"
    static AnsiString RLabel;

    //PROPIEDADES DEFINITORIAS:

    //número de identificación del azulejo
    //debe ser no negativo
    //valor por defecto: 0
    int getId(void) const {return __Id;} void setId(int);

    //NOTA: 23 es precisamente el número máximo de caracteres de un valor
    //en punto flotante para double.

    //ADVERTENCIA: no se efectuará control de nombres duplicados,
    //por lo que podrá haber objetos con el mismo nombre.
    //Se prefiere no llevar control de duplicidad por dos motivos:
    //      - las variables tampón pueden clonar los objetos
    //        sin interferir con ellos.
    //      - puede haber catálogos con nombres de objetos duplicados
    //        circunstancia que no debe impedir el funcionamiento
    //        del programa. Tal circunstancia debería ser advertida
    //        mediante una herramienta que no impida la ejecución.

    //ascensión recta del centro del azulejo en coordenadas celestes
    //valor por defecto: 0 rad
    double RA;
    //declinación del centro del azulejo en coordenadas celestes
    //debe estar en [-pi/2, pi/2]
    //valor por defecto: 0 rad
    double getDEC(void) const {return __DEC;} void setDEC(double);

    //radio del azulejo en radianes
    //debe ser mayor que cero
    //valor por defecto: 2.9088902912790199E-3 rad (= 10 arcmin)
    double getR_(void) const {return __R_;} void setR_(double);

    //lista de puntos de cielo del azulejo
    TSkyPointList SkyPointList;

    //ADVERTENCIA: los puntos de cielo del azulejo no serán borrados
    //del azulejo. Los algoritmos de programación de exposiciones
    //deberán sacar una copia de la lista y borrarlos de dicha lista.

    //PROPIEDADES EN FORMATO TEXTO:

    //número de identificación
    AnsiString getIdText(void) const;
    void setIdText(const AnsiString&);
    //ascensión recta
    AnsiString getRAText(void) const;
    void setRAText(const AnsiString&);
    //declinación
    AnsiString getDECText(void) const;
    void setDECText(const AnsiString&);
    //radio
    AnsiString getR_Text(void) const;
    void setR_Text(const AnsiString&);

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    //        AnsiString getRowIdent(void) const;

    //lista de valores de las propiedades en una fila
    AnsiString getRowText(void) const; void setRowText(const AnsiString&);
    //lista de valores de las propiedades en una columna
    AnsiString getColText(void) const; void setColText(const AnsiString&);
    //lista de asignaciones de valores a las propiedades en una columna
    AnsiString getAssignsText(void) const;
    void setAssignsText(const AnsiString&);

    //MÉTODOS ESTÁTICOS:

    //compara una de las propiedades de dos objetos
    static int  CompareIds(const TTile *T1, const TTile *T2);
    static int  CompareRAs(const TTile *T1, const TTile *T2);
    static int  CompareDECs(const TTile *T1, const TTile *T2);
    static int  CompareR_s(const TTile *T1, const TTile *T2);
    static int  CompareQ_s(const TTile *T1, const TTile *T2);

    //obtiene las etiquetas de las propiedades
    static AnsiString GetLabels(void);
    static AnsiString GetLabelsRow(void);

    //imprime las propiedades de un objeto en una cadena
    static void  PrintRow(AnsiString &S, const TTile *T);
    static void  PrintCol(AnsiString &S, const TTile *T);
    static void  PrintAssigns(AnsiString &S, const TTile *T);

    //lee las propiedades de un objeto en una cadena
    static void  ReadRow(TTile *T, const AnsiString &S, int &i);
    static void  ReadCol(TTile *T, const AnsiString &S, int &i);
    static void  ReadAssigns(TTile *T,
                                       const AnsiString &S, int &i);

    //MÉTODOS PÚBLICOS:

    //construye un azulejo
    TTile(void);
    //contruye un clon de un azulejo
    TTile(TTile*);

    //MÉTODOS DE COPIA

    //clona un axulejo
    void Copy(TTile*);
    TTile &operator=(const TTile&);

    //MÉTODOS:

    //mueve el azulejo al punto indicado
    void Move(double _RA, double _DEC);

    //segrega los puntos de cielo del catálogo en el azulejo incluyendo
    //aquellos que estén en el interior del círculo (RA, DEC, 1.2*R_)
    void Segregate(TSkyPointList*);

    //carga el aqzulejo de un archivo de texto
    void LoadFromFile(const AnsiString &FileName);
};

//---------------------------------------------------------------------------
//TTileList
//---------------------------------------------------------------------------

class TTileList : public TPointersList<TTile> {
    //PROPIEDADES EN FORMATO TEXTO:

public:
    //PROPIEDADES DEFINITORIAS:

    //MÉTODOS PÚBLICOS:

    //contruye una lista de azulejos
    TTileList(int _Capacity=64);

    //construye un número de azulejos
    void Build(int N);

    //PROPIEDADES EN FORMATO TEXTO:

    //lista de puntos de cielo del azulejo en formato texto
    AnsiString getTableText(void); void setTableText(const AnsiString &);

    //MÉTODOS:

    //segrega los puntos de cielo del catálogo en los azulejos de una lista
    void Segregate(TSkyPointList *SPL);

    //numera los azulejos
    void Reidentify(void);

    //guarda los azulejos en el directorio correspondiente
    void SaveToFiles(AnsiString FolderName="Tiles");

    //MÉTODOS DE INTERFAZ:
/*#
    //imprime los azulejos en una caja de lista
    void Print(QListWidget *LB);
    //imprime los azulejos en un memorando
    void Print(QTextEdit *M);*/
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // TILE_H
