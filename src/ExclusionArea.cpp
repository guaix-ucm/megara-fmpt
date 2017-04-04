// Copyright (c) 2012-2017 Isaac Morales Durán. All rights reserved.
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
//File: ExclusionArea.cpp
//Content: exclusion area (EA) model
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

//#include <values.h>

#include "ExclusionArea.h"
#include "FiberMOSModelConstants.h"
#include "Strings.h" //StrIndent
#include "TextFile.h"
#include "RoboticPositioner.h"

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TExclusionArea:
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//PROPIEDADES ESTÁTICAS:

//lista de objetos construidos
TItemsList<TExclusionArea*> TExclusionArea::Builts;

//---------------------------------------------------------------------------
//PROPIEDADES:

void TExclusionArea::setEo(double Eo)
{
    //el valor de Eo debe ser no negativo
    if(Eo < 0)
        throw EImproperArgument("Eo value should be nonnegative");

    p_Eo = Eo; //asigna el nuevo valor
    calculateSPM(); //asimila el nuevo valor
}
void TExclusionArea::setEp(double Ep)
{
    //el valor de Ep debe ser no negativo
    if(Ep < 0)
        throw EImproperArgument("Ep value should be nonnegative");

    p_Ep = Ep; //asigna el nuevo valor
    calculateSPM(); //asimila el nuevo valor
}

void TExclusionArea::setId(int Id)
{
    //el número de identificación Id debería ser mayor que cero
    if(Id <= 0)
        throw EImproperArgument("identification number Id should be upper zero");

    p_Id = Id; //asigna el nuevo valor
}

//PROPIEDADES EN FORMATO TEXTO:

AnsiString TExclusionArea::getEoText(void) const
{
    return FloatToStr(getEo());
}
void TExclusionArea::setEoText(const AnsiString &S)
{
    try {
        setEo(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting Eo in text format: ");
        throw;
    }
}
AnsiString TExclusionArea::getEpText(void) const
{
    return FloatToStr(getEp());
}
void TExclusionArea::setEpText(const AnsiString &S)
{
    try {
        setEp(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting Ep in text format: ");
        throw;
    }
}

AnsiString TExclusionArea::getIdText(void) const
{
    return IntToStr(getId());
}
void TExclusionArea::setIdText(const AnsiString& S)
{
    try {
        setId(StrToInt(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting Id in text format: ");
        throw;
    }
}

AnsiString TExclusionArea::getPendingText(void) const
{
    return BoolToStr(Pending, true);
}
void TExclusionArea::setPendingText(const AnsiString &S)
{
    try {
        Pending = StrToBool(S);
    } catch(Exception& E) {
        E.Message.Insert(1, "setting Pending in text format: ");
        throw;
    }
}
/*AnsiString TExclusionArea::getCollisionText(void) const
{
    return BoolToStr(Collision, true);
}
void TExclusionArea::setCollisionText(const AnsiString &S)
{
    try {
        Collision = StrToBool(S);
    } catch(Exception& E) {
        E.Message.Insert(1, "setting Collision in text format: ");
        throw;
    }
}*/

//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

//contorno de la barrera
//en formato de asignaciones
AnsiString TExclusionArea::getContour_Text(void) const
{
    string str = "# A list of segments (Pa, Pb) and arcs (Pa, Pb, Pc, R) for describe";
    str += "\r\n# the template of EA.Barrier.Contour_, in S1 (in mm):";
    str += "\r\n\r\n";
    str += Barrier.getContour_().getColumnText().str;

    return AnsiString(str);
}

//conjuntos de propiedades de seguridad
//en formato asignaciones de texto
AnsiString TExclusionArea::getSecurityText(void) const
{
    //PROPIEDADES DE SEGURIDAD:

    string str = "SPM components:";
    str += "\r\n    SPMsta = "+Barrier.getSPMText().str;

    return AnsiString(str);
}

//conjunto de propiedades de tolerancia
//en formato de asignaciones
AnsiString TExclusionArea::getToleranceText(void) const
{
    string str;

    str = commentedLine("Eo = "+getEoText().str, "error margin in theta_ orientation (in rad)");
    str += "\r\n"+commentedLine("Ep = "+getEpText().str, "error margin in P0 position (in mm)");

    return AnsiString(str);
}

//instancia del posicionador
//en formato de asignaciones
AnsiString TExclusionArea::getInstanceText(void) const
{
    string str;

    str = "# Instance properties of the EA (Exclusion Area):";
    str += "\r\n\r\n";
    str += commentedLine("Tolerances:", "Tolerance properties of the EA:");
    str += "\r\n";
    str += StrIndent(getToleranceText()).str;

    return AnsiString(str);
}
void TExclusionArea::setInstanceText(const AnsiString &S)
{
    int i = 1; //index to first char of the string
    try {
        //lee la instancia en una variable tampón
        TExclusionArea EA(this);
        readInstance(&EA, S, i);

        //busca texto inesperado
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text: "+StrFirstChars(S.SubString(i, S.Length() - i + 1)).str);

        //asigna la variable tampón
        clone(&EA);

    } catch(Exception& E) {
        unsigned int row, col;
        strPositionToCoordinates(row, col, S.str, i-1);
        E.Message.Insert(1, "setting instance in text format in row "+inttostr(row)+" and column "+inttostr(col)+": ");
        throw;
    }
}

AnsiString TExclusionArea::getOriginsRowText(void) const
{
    return getIdText()+AnsiString("\t")+Barrier.getP0().getRowText()+AnsiString("\t")+Barrier.getthetaO1Text();
}

//---------------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:


//compara los identificadores de dos EAs
int  TExclusionArea::compareIds(TExclusionArea *EA1, TExclusionArea *EA2)
{
    //el puntero EA1 debería apuntar a un área de exclusión construida
    if(EA1 == NULL)
        throw EImproperArgument("pointer EA1 should point a built exclusion area");

    //el puntero EA2 debería apuntar a un área de exclusión construida
    if(EA2 == NULL)
        throw EImproperArgument("pointer EA2 should point a built exclusion area");

    if(EA1->getId() < EA2->getId())
        return -1;
    if(EA1->getId() > EA2->getId())
        return 1;
    return 0;
}


//imprime el identificador de un EA
void  TExclusionArea::printId(AnsiString &S, TExclusionArea *EA)
{
    //el puntero EA debería apuntar a un área de exclusión construida
    if(EA == NULL)
        throw EImproperArgument("pointer EA should point to built exclusion area");

    S += EA->getIdText();
}

//obtiene las etiquetas de las propiedades de origen
//en formato fila de texto
AnsiString TExclusionArea::getOriginsLabelsRow(void)
{
    return "Id\tx0\ty0\tthetaO1";
}
//atraviesa las etiquetas de las propiedades de origen
//en formato fila de texto
void  TExclusionArea::travelOriginsLabelsRow(const AnsiString& S, int& i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    try {
        StrTravelLabel("Id", S, i);
        StrTravelSeparators(S, i);
        StrTravelLabel("x0", S, i);
        StrTravelSeparators(S, i);
        StrTravelLabel("y0", S, i);
        StrTravelSeparators(S, i);
        StrTravelLabel("thetaO1", S, i);

    } catch(Exception& E) {
        E.Message.Insert(1, "traveling origins labels row: ");
        throw;
    }
}

//imprime los valores de las propiedades de orien de un EA
//al final de una cadena de texto
//en formato fila de texto
void  TExclusionArea::printOriginsRow(AnsiString& S,
                                      TExclusionArea *EA)
{
    //el puntero EA debe apuntar a un área de exclusión construida
    if(EA == NULL)
        throw EImproperArgument("pointer C should bepoint to built exclusion area");

    S += EA->getOriginsRowText();
}
//lee los valores de las propiedades de orien para un EA
//desde la posición indicada de una cadena de texto
//en formato fila de texto
void  TExclusionArea::readOriginsRow(TExclusionArea *EA,
                                     const AnsiString& S, int &i)
{
    //comprueba las precondiciones
    if(EA == NULL)
        throw EImproperArgument("pointer C should bepoint to built exclusion area");
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    try {
        //variables tampón
        int Id;
        double x0, y0;
        double thetaO1;

        //lee los valores en variables tampón
        StrReadInt(Id, S, i);
        StrTravelSeparators(S, i);
        StrReadFloat(x0, S, i);
        StrTravelSeparators(S, i);
        StrReadFloat(y0, S, i);
        StrTravelSeparators(S, i);
        StrReadFloat(thetaO1, S, i);

        //asigna la variable tampón
        EA->setOrigins(Id, x0, y0, thetaO1);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading origins row: ");
        throw;
    }
}
//lee una instancia del EA
//desde la posición indicada de una cadena de texto
//en formato de asignaciones
void  TExclusionArea::readInstance(TExclusionArea *EA,
                                   const AnsiString& S, int &i)
{
    //comprueba las precondiciones
    if(EA == NULL)
        throw EImproperArgument("pointer EA shouldpoint to built exclusion area");
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    try {
        TExclusionArea t_EA(EA); //variables tampón

        //-----------------------------------------------

        StrTravelLabel("Tolerances:", S, i);

        StrTravelSeparators(S, i);
        StrTravelLabel("Eo", S, i);
        StrTravelLabel("=", S, i);
        double Eo;
        StrReadFloat(Eo, S, i);
        t_EA.setEo(Eo);

        StrTravelSeparators(S, i);
        StrTravelLabel("Ep", S, i);
        StrTravelLabel("=", S, i);
        double Ep;
        StrReadFloat(Ep, S, i);
        t_EA.setEp(Ep);

        //-----------------------------------------------

        //asigna la variable tampón
        EA->clone(&t_EA);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading instance: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

//contruye un área de exclusión
//con los valores por defecto
TExclusionArea::TExclusionArea(void) :
    p_Eo(MEGARA_Eo), p_Ep(MEGARA_Ep),
    p_Id(0),
    Barrier(TDoublePoint(0, 0), 0),
    //inicializa las propiedades de estado
    AdjacentRPs(10, TRoboticPositioner::compareIds, NULL, NULL, TRoboticPositioner::printId),
    Pending(true)//, Collision(false)
{
    //asimilalas propiedades de inicialización
    calculateSPM();

    //añade el objeto a la lista de contruidos
    Builts.Add(this);
}
//construye un área de exclusión
//con los valores indicados
//si el número de identificación es menor que uno
//      lanza una execepción EImproperArgument
TExclusionArea::TExclusionArea(int Id, TDoublePoint P0, double thetaO1) :
    p_Eo(MEGARA_Eo), p_Ep(MEGARA_Ep),
    Barrier(P0, thetaO1),
    //inicializa las propiedades de estado
    AdjacentRPs(),
    Pending(true)//, Collision(false)
{
    //inicializa las propiedades
    setId(Id);
    Barrier.setContour_Text(MEGARA_LIFU_Contour_Text);

    //asimilalas propiedades de inicialización
    calculateSPM();

    //añade el objeto a la lista de contruidos
    Builts.Add(this);
}

//clona un área de exclusión
void TExclusionArea::clone(const TExclusionArea *EA)
{
    //el puntero EA debería apuntar a un área de exclusión contruida
    if(EA == NULL)
        throw EImproperArgument("pointer EA should point to built exclusion area");

    p_Eo = EA->p_Eo;
    p_Ep = EA->p_Ep;
    p_Id = EA->p_Id;
    Barrier.clone(&EA->Barrier);
    Pending = EA->Pending;
//    Collision = EA->Collision;
}

//contruye un clon de un área de exclusión
TExclusionArea::TExclusionArea(const TExclusionArea *EA) :
    //construye un clon de la barrera
    Barrier(EA->Barrier)
{
    //check the precondition
    if(EA == NULL)
        throw EImproperArgument("pointer EA should point to built barrier");

    //clona todas las propiedades
    clone(EA);

    //añade el objeto a la lista de contruidos
    Builts.Add(this);
}

//libera la memoria dinámica y borra el objeto de Builts
//si el objeto no está en Builts lanza EImproperCall
TExclusionArea::~TExclusionArea()
{
    //debe haber algún objeto para destruir
    if(Builts.getCount() < 1)
        throw EImproperCall("there isn't an object to destroy");

    //busca el objeto a destruir en la lista
    int i = 0;
    while(i<Builts.getCount() && Builts[i]!=this)
        i++;

    //si no ha encontrado el objeto
    if(i >= Builts.getCount())
        //indica que está intentando destruir un objeto no contruido
        throw EImproperCall("destruction attempt of non built object: "+IntToHex(intptr_t(this)).str);

    //borra el puntero de la lista de construídos
    Builts.Delete(i);
}

//---------------------------------------------------------------------------
//MÉTODOS PARA CALCULAR LOS VALORES RECOMENDADOS DE
//LAS COMPONENTES DE SPM:

//Calcula el márgen perimetral de seguridad para evitar:
//- el error mecánico debido al mecanizado del actuador;
//- el error mecánico de orientación en el anclaje del posicionador;
//- el error mecánico de posicionamiento en el anclaje del posicionador;
//- el error numérico de las operaciones matemáticas del modelo;
//Todos los errores serán absorbidos por Eo y Ep
//en la siguiente fórmula:
//      SPMsta = Eo*r_max + Ep
//Donde:
//      Eo: margen de error de orientación en rad;
//      Ep: margen de error de posición en mm.
double TExclusionArea::SPMsta(void) const
{
    //calcula y devuelve el valor
    return getEo()*Barrier.getr_max() + getEp();
}

//---------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//A partir de:
//      (Eo, Ep)
//      Barrier.r_max
//Determina:
//      Barrier.SPM
void TExclusionArea::calculateSPM(void)
{
    Barrier.setSPM(SPMsta());
}

//---------------------------------------------------------------------------
//MÉTODOS DE ASIGNACIÓN CONJUNTA:

//asigna las propiedades de origen
void TExclusionArea::setOrigins(int Id, double x0, double y0, double thetaO1)
{
    //el número de identificación Id debe ser mayor que cero
    if(Id <= 0)
        throw EImproperArgument("identificator number Idshould beupper zero");

    //asigna los nuevos valores
    p_Id = Id;
    Barrier.set(TDoublePoint(x0, y0), thetaO1);

    //estos valores no necesitan asimilación
}

//asigna conjuntamente las tolerancias y las asimila
//      (Eo, Ep)
void TExclusionArea::setTolerances(double Eo, double Ep)
{
    //comprueba las precondiciones
    if(Eo < 0)
        throw EImproperArgument("Eo value should be nonnegative");
    if(Ep < 0)
        throw EImproperArgument("Ep value should be nonnegative");

    //asigna los nuevos valores
    p_Eo = Eo;
    p_Ep = Ep;

    calculateSPM(); //asimila los nuevos valores
}

//determina si dos areas de exclusión difieren entre si
bool areUnequals(const TExclusionArea *EA1, const TExclusionArea *EA2)
{
    //comprueba las precondiciones
    if(EA1 == NULL)
        throw EImproperArgument("pointer EA1 should point to built exclusion area");
    if(EA2 == NULL)
        throw EImproperArgument("pointer EA2 should point to built exclusion area");

    if(EA1->Barrier != EA2->Barrier)
        return true;

    if(EA1->getEo() != EA2->getEo())
        return true;
    if(EA1->getEp() != EA2->getEp())
        return true;

    if(EA1->getId() != EA2->getId())
        return true;
    if(EA1->Pending != EA2->Pending)
        return true;
//    if(EA1->Collision != EA2->Collision)
  //      return true;

    return false;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

