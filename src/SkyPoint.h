// Copyright (c) 2014-2015 Isaac Morales Durán. All rights reserved.
// Institute of Astrophysics of Andalusia, IAA-CSIC
//
// This file is part of FMPT (Fiber MOS Positioning Tools)
//
// FMPT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//---------------------------------------------------------------------------
//File: SkyPoint.h
//Content: sky point
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef SKYPOINT_H
#define SKYPOINT_H

#include "ItemsList.h"

//##include <QListWidget>
//##include <QTextEdit>

//---------------------------------------------------------------------------

using namespace Lists;

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TPointType:
//---------------------------------------------------------------------------

//type of point
//  ptUNKNOWN: unknown type
//  ptSOURCE: scientific source
//  ptREFERENCE: reference source
//  ptBLANK: sky backgound
enum TPointType {ptUNKNOWN, ptSOURCE, ptREFERENCE, ptBLANK};

void  strPrinPointType(string& str, TPointType value);
void  strReadPointType(TPointType& value, const string &str, unsigned int &i);
string pointTypeToStr(TPointType value);
TPointType strToPointType(const string& str);

//---------------------------------------------------------------------------
//TSkyPoint:
//---------------------------------------------------------------------------

//class sky point
class TSkyPoint {
protected:
    //PROPIEDADES DEFINITORIAS:

    int p_Id;

    double p_RA;
    double p_DEC;

    double p_Tint;
    AnsiString p_Name;

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

    //etiqueta de la propiedad Tint
    //debe ser imprimible
    //valor por defecto: "Tint"
    static AnsiString TintLabel;
    //etiqueta de la propiedad Type
    //debe ser imprimible
    //valor por defecto: "Type"
    static AnsiString TypeLabel;

    //label of the property Priority
    //must be printable
    //default value: "Priority"
    static AnsiString PriorityLabel;

    //PROPIEDADES DEFINITORIAS:

    //número de identificación del objeto
    //debe ser mayor que cero
    //valor por defecto: el indicado en el contructor
    int getId(void) const {return p_Id;} void setId(int);

    //ADVERTENCIA: no se efectuará control de identificadores duplicados,
    //por lo que podrá haber objetos con el mismo identificador.
    //Se prefiere no llevar control de duplicidad por dos motivos:
    //      - las variables tampón pueden clonar los objetos
    //        sin interferir con ellos.
    //      - puede haber catálogos con nombres de objetos duplicados
    //        circunstancia que no debe impedir el funcionamiento
    //        del programa. Tal circunstancia debería ser advertida
    //        mediante una herramienta que no impida la ejecución.

    //ascensión recta del punto de cielo en coordenadas celestes
    //valor por defecto: 0 rad
    double getRA(void) const {return p_RA;} void setRA(double);
    //declinación del punto de cielo en coordenadas celestes
    //debe estar en [-pi/2, pi/2]
    //valor por defecto: 0 rad
    double getDEC(void) const {return p_DEC;} void setDEC(double);

    //NOTA: dos puntos de cielo podrían estar en el mismo punto
    //y enmascararse mutuamente.

    //tiempo de integración del SP
    //debe ser no negativo
    //valor por defecto: 0
    double getTint(void) const {return p_Tint;} void setTint(double);

    //type of point
    //  ptUNKNOWN: unknown type
    //  ptSOURCE: scientific source
    //  ptREFERENCE: reference source
    //  ptBLANK: sky backgound
    //default value: ptUNKNOWN
    TPointType Type;

    //nombre del punto de cielo
    //debe estar entre comillas simples
    //debe ser imprimible
    //debe tener 23 caracteres como máximo
    //no debe estar asignado a otro objeto (excepto la cadena vacía '')
    //valor por defecto: "''"
    AnsiString getName(void) const {return p_Name;}
    void setName(const AnsiString&);

    //NOTA: 23 es precisamente el número máximo de caracteres de un valor
    //en punto flotante para double.

    //allocation Priority
    //devault value: 0
    unsigned int Priority;

    //PROPIEDADES EN FORMATO TEXTO:

    AnsiString getIdText(void) const;
    void setIdText(const AnsiString&);

    AnsiString getRAText(void) const;
    void setRAText(const AnsiString&);
    AnsiString getDECText(void) const;
    void setDECText(const AnsiString&);
    AnsiString getTintText(void) const;
    void setTintText(const AnsiString&);
    AnsiString getTypeText(void) const;
    void setTypeText(const AnsiString&);

    //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

    //todas las propiedades en fila
    AnsiString getRowText(void) const;
    void setRowText(const AnsiString&);
    //todas las propiedades en columna
    AnsiString getColText(void) const;
    void setColText(const AnsiString&);
    //todas las propiedades en asignaciones
    AnsiString getAssignsText(void) const;
    void setAssignsText(const AnsiString&);

    //Formatos de texto:
    //      fila: los valores de las propiedades ordenados
    //              y separados por espacios;
    //      columna: los valores de las propiedades ordenados
    //              y separados por "\r\n";
    //      asignaciones: las asignaciones a las propiedades ordenadas
    //              y separadas por "\r\n".

    //NOTA: las columnas tendrán un indentado de 24 caracteres ya que
    //el valor en punto flotante más largo tiene 23 caracteres.

    //MÉTODOS ESTÁTICOS:

    //compara una de las propiedades de dos puntos de cielo
    static int  CompareIds(TSkyPoint*, TSkyPoint*);
    static int  CompareRAs(TSkyPoint*, TSkyPoint*);
    static int  CompareDECs(TSkyPoint*, TSkyPoint*);

    //obtiene las etiquetas de las propiedades en una cadena
    static AnsiString GetLabels(void);
    static AnsiString GetLabelsRow(void);

    //imprime las propiedades de un punto de cielo en una cadena
    static void  PrintRow(AnsiString &S, TSkyPoint *SP);
    static void  PrintCol(AnsiString &S, TSkyPoint *SP);
    static void  PrintAssigns(AnsiString &S, TSkyPoint *SP);

    //lee las propiedades de un punto de cielo en una cadena de texto
    static void  ReadRow(TSkyPoint* &SP,
                                   const AnsiString &S, int &i);
    static void  ReadCol(TSkyPoint* &SP,
                                   const AnsiString &S, int &i);
    static void  ReadAssigns(TSkyPoint* &SP,
                                       const AnsiString &S, int &i);

    //MÉTODOS PÚBLICOS:

    //contruye un punto de cielo
    TSkyPoint(void);

    //construye un clon de un punto de cielo
    TSkyPoint(TSkyPoint*);

    //MÉTODOS DE COPIA:

    //clona un punto de cielo
    void Copy(TSkyPoint*);
    TSkyPoint &operator=(const TSkyPoint&);

    //MÉTODOS DE DESPLAZAMIENTO:

    //asigna (DEC, RA)
    void SetPosition(double RA, double DEC);

    //randomiza las coordenadas celestes con distribución uniforme
    //en la esfera celeste
    void Randomize(void);
    //randomiza las coordenadas celestes con distribución uniforme
    //en un intervalo
    void Randomize(double RAmin, double RAmax,
                   double DECmin, double DECmax);

    //DEFINICIÓN: el randomizado de un punto con distribición uniforme
    //en coordenadas polares puede lograrse randomizando la orientación
    //del punto en torno de cada uno de los tres ejes cartesianos (x, y z),
    //como si de un giróscopo se tratara:
    //      1. randomizar en (-pi, pi] en torno del eje z;
    //      2. randomizar en (-pi, pi] en torno del eje y;
    //      3. randomizar en (-pi, pi] en torno del eje x;
    //      4. traducir a coordenadas esféricas.
    //Aunque esto equivale a girar una bóveda no uniforme sobre el último
    //eje, debe pensarse que por simetría la distribución será uniforme,
    //ya que a mayor densidad mayor es el desplazameinto en el último
    //giro.
};

//---------------------------------------------------------------------------
//TSkyPointList:
//---------------------------------------------------------------------------

//clase lista de SPs
class TSkyPointList : public TItemsList<TSkyPoint*> {
    //PROPIEDADES DE INTERFAZ:

public:
    //PROPIEDADES DE INTERFAZ:

    //lista de puntos de cielo en formato tabla
    AnsiString getTableText(void);
    void setTableText(const AnsiString &);

    //ADVERTENCIA: SetText construirá nuevos puntos de cielo con sus
    //propiedades inicialziadas por defecto.
    /*
        //MÉTODOS ESTÁTICOS:
    //imprime una lista de puntos de cielo en una cadena de texto
    static void  PrintTSkyPoints(AnsiString &S,
                const TSkyPointList *L);
        //lee una lista de puntos de cielo en una cedena de texto
    static void  ReadTSkyPoints(
                TSkyPointList *L,
                const AnsiString &S, int &i);   */

    //MÉTODOS PÚBLICOS:

    //construye una lista de puntos de cielo
    TSkyPointList(int Capacity=1840);

    //copia una lista de puntos de cielo
    TSkyPointList &operator=(const TSkyPointList&);

    //busca el objeto cleste con el nombre indicado
    int SearchId(int Id);

    //contruye N puntos de cielo inicializados por defecto
    void Build(int N);
    //destruye los puntos de cielo de la lista
    void Destroy(void);

    //reenumera los puntos de cielo
    void Reidentify(void);

    //randomiza las coordenadas celestes
    //de los puntos de cielo de la lista
    //con distribución uniforme en toda la esfera celeste
    void Randomize(void);
    //randomiza las coordenadas celestes
    //de los puntos de cielo de la lista
    //con distribución uniforme en un intervalo
    void Randomize(double RAmin, double RAmax,
                   double DECmin, double DECmax);

    //borra los puntos de cielo cuyo identificador coincida con el indicado
    void DeleteId(int Id);
    //borra los puntos de cielo correspondientes a
    //los puntos de proyección que han sido asignados
    /*        void DeleteAllocatedObjects(TAllocationPairList *APL);*/

    //INTERFAZ:
/*#
    //imprime los puntos de cielo en una caja de lista
    void Print(QListWidget *LB);
    //imprime los puntos de cielo en un memorando
    void Print(QTextEdit *M);
*/
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // SKYPOINT_H
