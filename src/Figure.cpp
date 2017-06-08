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
///@file Figure.cpp
///@brief geometric figures
///@author Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Figure.h"
#include "Strings.h"
#include "Geometry.h"
#include "Scalars.h"

//---------------------------------------------------------------------------

using namespace Mathematics;
using namespace Strings;

//espacio de nombres de modelos
namespace Models {

//--------------------------------------------------------------------------
//TFigure
//--------------------------------------------------------------------------

//PROPIEDADES ESTÁTICAS:

//inicializa las propiedades estáticas
AnsiString TFigure::ColorLabel = "Color";

//MÉTODOS ESTÁTICOS DE IMPRESIÓN Y LECTURA:

//imprime las propiedades de un objeto en una cadena
//en formato texto entre paréntesis
void  TFigure::figurePrint(AnsiString& S,
                           TFigure *F)
{
    //comprueba las precondiciones
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //añade el texto a la cadena
    S += F->getText();
}
//imprime las propiedades de un objeto en una cadena
//en formato fila de texto
void  TFigure::figurePrintRow(AnsiString& S,
                              TFigure *F)
{
    //comprueba las precondiciones
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //añade el texto a la cadena
    S += F->getRowText();
}

//lee las propiedades de un objeto en una cadena de texto
//en formato texto entre paréntesis
void  TFigure::figureRead(TFigure* &F, const AnsiString& S, int& i)
{
    try {
        //comprueba las precondiciones
        if(i<1 || S.Length()<i)
            throw EImproperArgument("index i should indicate a char of the string S");

        //guarda la posición inicial de lectura
        int i_ = i;

        TCircunference *Circunference;
        try {
            //intenta leer una circunferencia
            Circunference = new TCircunference();
            TCircunference::Read(Circunference, S, i);
            F = Circunference;
            return;
        } catch(EImproperArgument& E) { //si no puede leerlo
            //vuelve al estado inicial
            delete Circunference;
            i = i_;
        }

        TSegment *Segment;
        try {
            //intenta leer un segmento
            Segment = new TSegment();
            TSegment::Read(Segment, S, i);
            F = Segment;
            return;
        } catch(EImproperArgument& E) { //si no puede leerlo
            //vuelve al estado inicial
            delete Segment;
            i = i_;
        }

        TArc *Arc;
        try {
            //intenta leer un arco
            Arc = new TArc();
            TArc::Read(Arc, S, i);
            F = Arc;
            return;
        } catch(EImproperArgument& E) { //si no puede leerlo
            //vuelve al estado inicial
            delete Arc;
            i = i_;
        }

        //indica que no hay ninguna figura geométrica en la posición i de la cadena S
        throw EImproperArgument(AnsiString("there isn't a figure in position ")+IntToStr(i)+AnsiString(" of string S"));
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading figure in text format: ");
        throw;
    }
}
//lee las propiedades de un objeto en una cadena de texto
//en formato fila de texto
void  TFigure::figureReadRow(TFigure* &F, const AnsiString& S, int& i)
{
    try {
        //comprueba las precondiciones
        if(i<1 || S.Length()<i)
            throw EImproperArgument("index i should indicate a char of the string S");

        //guarda la posición inicial de lectura
        int i_ = i;

        TCircunference *Circunference;
        try {
            //intenta leer una cirunferencia
            Circunference = new TCircunference();
            TCircunference::ReadRow(Circunference, S, i);
            F = Circunference;
            return;
        } catch(EImproperArgument& E) { //si no puede leerlo
            //vuelve al estado inicial
            delete Circunference;
            i = i_;
        }

        TSegment *Segment;
        try {
            //intenta leer un segmento
            Segment = new TSegment();
            TSegment::ReadRow(Segment, S, i);
            F = Segment;
            return;
        } catch(EImproperArgument& E) { //si no puede leerlo
            //vuelve al estado inicial
            delete Segment;
            i = i_;
        }

        TArc *Arc;
        try {
            //intenta leer un arco
            Arc = new TArc();
            TArc::ReadRow(Arc, S, i);
            F = Arc;
            return;
        } catch(EImproperArgument& E) { //si no puede leerlo
            //vuelve al estado inicial
            delete Arc;
            i = i_;
        }

        //indica que no hay ninguna figura geométrica en la posición i de la cadena S
        throw EImproperArgument(AnsiString("there isn't a figure in position ")+IntToStr(i)+AnsiString(" of string S"));
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading figure in row text format: ");
        throw;
    }
}

//destruye una figura
TFigure::~TFigure()
{
}

//--------------------------------------------------------------------------
//TCircle
//--------------------------------------------------------------------------

//PROPIEDADES ESTÁTICAS:

//inicializa las propiedades estáticas
AnsiString TCircle::RLabel = "R";

//PROPIEDADES DE DEFINICIÓN:

void TCircle::setR(double R)
{
    //comprueba las precondiciones
    if(R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna el nuevo valor
    p_R = R;
}

//CONJUNTOS DE PROPIEDADES DE DEFINICIÓN EN FORMATO TEXTO:

AnsiString TCircle::getRText(void) const
{
    return FloatToStr(p_R);
}
void TCircle::setRText(const AnsiString& S)
{
    try {
        setR(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting value to R in text format: ");
        throw;
    }
}

//CONJUNTOS DE PROPIEDADES DE DEFINICIÓN EN FORMATO TEXTO:

AnsiString TCircle::getText(void) const
{
    return AnsiString("(")+P.getText()+AnsiString(", ")+getRText()+AnsiString(")");
}
void TCircle::setText(const AnsiString& S)
{
    try {
        //lee el valor en una variable tampón
        TCircle C;
        int i = 1;
        Read(&C, S, i);

        //busca texto inesperado
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("string S shouldcontain a single circle only");

        //asigna la variable tampón
        this->Copy(&C);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting value to circle in text format: ");
        throw;
    }
}
AnsiString TCircle::getRowText(void) const
{
    return P.getRowText()+AnsiString("\t")+getRText();
}
void TCircle::setRowText(const AnsiString& S)
{
    try {
        //lee el valor en una variable tampón
        TCircle C;
        int i = 1;
        ReadRow(this, S, i);

        //busca texto inesperado
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("string S shouldcontain a single circle only");

        //asigna la variable tampón
        this->Copy(&C);

    } catch(Exception& E) {
        E.Message.Insert(1, "setting value to circle in row text format: ");
        throw;
    }
}
AnsiString TCircle::getAssignsText(void) const
{
    AnsiString str;

    str = "x = "+P.getxText().str+"\r\n";
    str += "y = "+P.getyText().str+"\r\n";
    str += "R = "+getRText().str+"\r\n";

    return AnsiString(str);
}

//FUNCIONES ESTÁTICAS PARA OBTENER LAS ETIQUETAS
//DE LAS PROPIEDADES EN FORMATO TEXTO:

//obtiene las etiquetas de las propiedades de definición
//en formato texto entre paréntesis
AnsiString TCircle::GetLabels(void)
{
    return AnsiString("(")+TDoublePoint::GetLabels()+AnsiString(", ")+RLabel+AnsiString(")");
}
//obtiene las etiquetas de las propiedades de definición
//en formato línea de texto
AnsiString TCircle::GetRowLabels(void)
{
    return TDoublePoint::GetRowLabels()+AnsiString("\t")+RLabel;
}

//MÉTODOS ESTÁTICOS DE COMPARACIÓN:

int  TCircle::comparex(const TCircle *C1,
                       const TCircle *C2)
{
    //comprueba las precondiciones
    if(C1 == NULL)
        throw EImproperArgument("pointer C1 should point to built circle");
    if(C2 == NULL)
        throw EImproperArgument("pointer C2 should point to built circle");

    //compara la propiedad
    if(C1->P.x < C2->P.x)
        return -1;
    if(C1->P.x > C2->P.x)
        return 1;
    return 0;
}
int  TCircle::comparey(const TCircle *C1,
                       const TCircle *C2)
{
    //comprueba las precondiciones
    if(C1 == NULL)
        throw EImproperArgument("pointer C1 should point to built circle");
    if(C2 == NULL)
        throw EImproperArgument("pointer C2 should point to built circle");

    //compara la propiedad
    if(C1->P.y < C2->P.y)
        return -1;
    if(C1->P.y > C2->P.y)
        return 1;
    return 0;
}
int  TCircle::compareR(const TCircle *C1,
                       const TCircle *C2)
{
    //comprueba las precondiciones
    if(C1 == NULL)
        throw EImproperArgument("pointer C1 should point to built circle");
    if(C2 == NULL)
        throw EImproperArgument("pointer C2 should point to built circle");

    //compara la propiedad
    if(C1->getR() < C2->getR())
        return -1;
    if(C1->getR() > C2->getR())
        return 1;
    return 0;
}

//MÉTODOS ESTÁTICOS DE IMPRESIÓN:

void  TCircle::Print(AnsiString& S, const TCircle *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    S += C->getText();
}
void  TCircle::PrintRow(AnsiString& S, const TCircle *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    S += C->getRowText();
}
void  TCircle::PrintAssigns(AnsiString& S, const TCircle *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    S += C->getAssignsText();
}

//MÉTODOS ESTÁTICOS DE LECTURA:

void  TCircle::Read(TCircle *C,
                    const AnsiString& S, int& i)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C shoult point to built circle");
    if(i<1 || S.Length()<i)
        throw EImproperArgument("circle value not found");

    try {
        //lee las propiedades en una variable tampón
        TCircle t_C;
        StrTravelLabel("(", S,i);
        TDoublePoint P;
        StrReadDPoint(&P, S, i);
        t_C.P = P;
        StrTravelLabel(",", S,i);
        double R;
        StrReadFloat(R, S, i);
        t_C.setR(R);
        StrTravelLabel(")", S,i);

        //asigna la variable tampón
        *C = t_C;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading value to circle in text format: ");
        throw;
    }
}
void  TCircle::ReadRow(TCircle *C,
                       const AnsiString& S, int& i)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C shoult point to built circle");
    if(i<1 || S.Length()<i)
        throw EImproperArgument("circle values not found");

    try {
        //lee el valor en una variable tampón
        TCircle t_C; //variable tampón
        double aux;
        StrReadFloat(aux, S, i);
        t_C.P.x = aux;
        StrTravelSeparators(S, i);
        StrReadFloat(aux, S, i);
        t_C.P.y = aux;
        StrTravelSeparators(S, i);
        StrReadFloat(aux, S, i);
        t_C.setR(aux);

        //asigna la variable tampón
        *C = t_C;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading value to circle in row text format: ");
        throw;
    }
}

//MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACIÓN:

//contruye un objeto
TCircle::TCircle(void) : TFigure(),
    p_R(1)
{
    P.x = 0;
    P.y = 0;
}
TCircle::TCircle(double x, double y, double R)
{
    //comprueba las precondiciones
    if(R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna el nuevo valor
    P.x = x;
    P.y = y;
    p_R = R;

}
TCircle::TCircle(TDoublePoint P, double R)
{
    //comprueba las precondiciones
    if(R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna el nuevo valor
    P = P;
    p_R = R;

}

//copia las propiedades de un objeto
void TCircle::Copy(TCircle *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built proyection point");

    //asigna las propiedades
    P = C->P;
    p_R = C->p_R;
}
TCircle& TCircle::operator=(const TCircle &C)
{
    //asigna las propiedades
    P = C.P;
    p_R = C.p_R;

    return *this;
}

//contruye un clon de un objeto
TCircle::TCircle(TCircle *PP)
{
    Copy(PP);
}

//MÉTODOS DE CONFIGURACIÓN:

//asigna conjuntamente los valores de las propiedades de definición
void TCircle::SetValues(TDoublePoint P, double R)
{
    //comprueba las precondiciones
    if(R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna los nuevos valores
    P = P;
    p_R = R;
}

//MÉTODOS DE CÁLCULO DE DISTANCIAS:

//calcula la distancia mínima entre la figura y un punto
double TCircle::distancePoint(TDoublePoint Q)
{
    double d = Mod(Q - P);

    if(d <= p_R)
        return 0;
    else
        return d - p_R;
}
//calcula la distancia máxima entre la figura y un punto
double TCircle::distancePointMax(TDoublePoint Q)
{
    return Mod(Q - P) + p_R;
}

//calcula la distancia mínima entre la figura y una circulo
double TCircle::distanceCircle(TCircle *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    return distanceCircleCircle(P, p_R, C->P, C->getR());
}
//calcula la distancia mínima entre la figura y una circunferencia
double TCircle::distanceCircunference(TCircunference *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circunference");

    return distanceCircleCircunference(P, p_R, C->P, C->getR());
}

//calcula la distancia mínima entre la figura y un segmento
double TCircle::distanceSegment(TSegment *S)
{
    //comprueba las precondiciones
    if(S == NULL)
        throw EImproperArgument("pointer S should point to built segment");

    return distanceCircleSegment(P, p_R, S->getPa(), S->getPb());
}
//calcula la distancia mínima entre la figura y un arco
double TCircle::distanceArc(TArc *A)
{
    //comprueba las precondiciones
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built arc");

    return distanceCircleArc(P, p_R, A->getPa(), A->getPb(), A->getPc(), A->getR());
}

//calcula la distancia mínima entre la figura y otra figura
double TCircle::distance(TFigure *F)
{
    //comprueba las precondiciones
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //si no, si la figura es un círculo
    if(typeid(*F) == typeid(TCircle))
        //calcula la distancia mínima al objeto
        return distanceCircle((TCircle*)F);
    //si no, si la figura es una circunferencia
    else if(typeid(*F) == typeid(TCircunference))
        //calcula la distancia mínima al objeto
        return distanceCircunference((TCircunference*)F);

    //si la figura es un segmento
    else if(typeid(*F) == typeid(TSegment))
        //calcula la distancia mínima al objeto
        return distanceSegment((TSegment*)F);
    //si no, si la figura es un arco
    else if(typeid(*F) == typeid(TArc))
        //calcula la distancia mínima al objeto
        return distanceArc((TArc*)F);
    else
        throw EImpossibleError(AnsiString("unknown type: ")+AnsiString(typeid(*F).name()));
}

//MÉTODOS DE ROTACIÓN Y TRASLACIÓN:

//obtiene la figutra geométrica rotada theta radianes
//Figure debe contener una figura geométrica del mismo tipo
void TCircle::getRotated(TFigure *F, double theta)
{
    //comprueba las precondiciones
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //el puntero F debería apauntar a una figura del tipo TCircle
    if(typeid(*F) != typeid(TCircle))
        throw EImproperArgument("pointer F should point to figure type TCircle");

    //apunta la figura con un puntero del tipo TCircle
    TCircle *C = (TCircle*)F;

    //calcula los coeficientes de rotación
    const double COS = cos(theta);
    const double SIN = sin(theta);

    //asigna los puntos rotados
    C->P.x = P.x*COS - P.y*SIN;
    C->P.y = P.x*SIN + P.y*COS;
}
//traslada la figura geométrica según el vector V
void TCircle::translate(TDoublePoint V)
{
    //traslada los puntos de la figura
    P.x += V.x;
    P.y += V.y;
}

//--------------------------------------------------------------------------
//TCircunference
//--------------------------------------------------------------------------

//PROPIEDADES ESTÁTICAS:

//inicializa las propiedades estáticas
AnsiString TCircunference::RLabel = "R";

//PROPIEDADES DE DEFINICIÓN:

void TCircunference::setR(double R)
{
    //comprueba las precondiciones
    if(R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna el nuevo valor
    p_R = R;
}

//CONJUNTOS DE PROPIEDADES DE DEFINICIÓN EN FORMATO TEXTO:

AnsiString TCircunference::getRText(void) const
{
    return FloatToStr(p_R);
}
void TCircunference::setRText(const AnsiString& S)
{
    try {
        setR(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting value to R in text format");
        throw;
    }
}

//CONJUNTOS DE PROPIEDADES DE DEFINICIÓN EN FORMATO TEXTO:

AnsiString TCircunference::getText(void) const
{
    return AnsiString("(")+P.getText()+AnsiString(", ")+getRText()+AnsiString(")");
}
void TCircunference::setText(const AnsiString& S)
{
    try {
        int i = 1;
        Read(this, S, i);

        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("string S shouldcontain a single circle only");
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting value to circunference in text format: ");
        throw;
    }
}
AnsiString TCircunference::getRowText(void) const
{
    return P.getRowText()+AnsiString("\t")+getRText();
}
void TCircunference::setRowText(const AnsiString& S)
{
    try {
        //read the circunference fron the first position
        int i = 1;
        ReadRow(this, S, i);

        //search unexpected text
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text translating string to circunference");
    }
    catch(Exception& E) {
        E.Message.Insert(1, "translating string to circunference: ");
        throw;
    }
}
AnsiString TCircunference::getAssignsText(void) const
{
    string str;

    str = "x = "+P.getxText().str+"\r\n";
    str += "y = "+P.getyText().str+"\r\n";
    str += "R = "+getRText().str+"\r\n";

    return AnsiString(str);
}

//FUNCIONES ESTÁTICAS PARA OBTENER LAS ETIQUETAS
//DE LAS PROPIEDADES EN FORMATO TEXTO:

//obtiene las etiquetas de las propiedades de definición
//en formato texto entre paréntesis
AnsiString TCircunference::GetLabels(void)
{
    return AnsiString("(")+TDoublePoint::GetLabels()+AnsiString(", ")+RLabel+AnsiString(")");
}
//obtiene las etiquetas de las propiedades de definición
//en formato línea de texto
AnsiString TCircunference::GetRowLabels(void)
{
    return TDoublePoint::GetRowLabels()+AnsiString("\t")+RLabel;
}

//MÉTODOS ESTÁTICOS DE COMPARACIÓN:

int  TCircunference::comparex(const TCircunference *C1,
                              const TCircunference *C2)
{
    //comrpueba las precondiciones
    if(C1 == NULL)
        throw EImproperArgument("pointer C1 should point to built circle");
    if(C2 == NULL)
        throw EImproperArgument("pointer C2 should point to built circle");

    //compara la propiedad
    if(C1->P.x < C2->P.x)
        return -1;
    if(C1->P.x > C2->P.x)
        return 1;
    return 0;
}
int  TCircunference::comparey(const TCircunference *C1,
                              const TCircunference *C2)
{
    //comrpueba las precondiciones
    if(C1 == NULL)
        throw EImproperArgument("pointer C1 should point to built circle");
    if(C2 == NULL)
        throw EImproperArgument("pointer C2 should point to built circle");

    //compara la propiedad
    if(C1->P.y < C2->P.y)
        return -1;
    if(C1->P.y > C2->P.y)
        return 1;
    return 0;
}
int  TCircunference::compareR(const TCircunference *C1,
                              const TCircunference *C2)
{
    //comrpueba las precondiciones
    if(C1 == NULL)
        throw EImproperArgument("pointer C1 should point to built circle");
    if(C2 == NULL)
        throw EImproperArgument("pointer C2 should point to built circle");

    //compara la propiedad
    if(C1->p_R < C2->p_R)
        return -1;
    if(C1->p_R > C2->p_R)
        return 1;
    return 0;
}

//MÉTODOS ESTÁTICOS DE IMPRESIÓN:

void  TCircunference::Print(AnsiString& S, const TCircunference *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    S += C->getText();
}
void  TCircunference::PrintRow(AnsiString& S, const TCircunference *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    S += C->getRowText();
}
void  TCircunference::PrintAssigns(AnsiString& S, const TCircunference *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    S += C->getAssignsText();
}

//MÉTODOS ESTÁTICOS DE LECTURA:

void  TCircunference::Read(TCircunference *C,
                           const AnsiString& S, int& i)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C shoult point to built circle");
    if(i<1 || S.Length()<i)
        throw EImproperArgument("circle values not found");

    try {
        //lee el valor en una variable tampón
        TCircunference t_C;
        StrTravelLabel("(", S,i);
        TDoublePoint P;
        StrReadDPoint(&P, S, i);
        t_C.P = P;
        StrTravelLabel(",", S,i);
        double R;
        StrReadFloat(R, S, i);
        t_C.setR(R);
        StrTravelLabel(")", S,i);

        //asigna la variable tampón
        *C = t_C;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading value to circunference in text format: ");
        throw;
    }
}
void  TCircunference::ReadRow(TCircunference *C,
                              const AnsiString& S, int& i)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C shoult point to built circle");
    if(i<1 || S.Length()<i)
        throw EImproperArgument("circle values not found");

    try {
        //lee el valor en una variable tampón
        TCircunference t_C;
        double aux;
        StrReadFloat(aux, S, i);
        t_C.P.x = aux;
        StrTravelSeparators(S, i);
        StrReadFloat(aux, S, i);
        t_C.P.y = aux;
        StrTravelSeparators(S, i);
        StrReadFloat(aux, S, i);
        t_C.setR(aux);

        //asigna la variable tampón
        *C = t_C;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading value to circunference in row text format: ");
        throw;
    }
}

//MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACIÓN:

//contruye un objeto
TCircunference::TCircunference(void) : TFigure(),
    p_R(1)
{
    P.x = 0;
    P.y = 0;
}
TCircunference::TCircunference(double x, double y, double R)
{
    //comprueba las precondiciones
    if(R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna el nuevo valor
    P.x = x;
    P.y = y;
    p_R = R;

}
TCircunference::TCircunference(TDoublePoint P, double R)
{
    //comprueba las precondiciones
    if(R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna el nuevo valor
    P = P;
    p_R = R;

}

//copia las propiedades de un objeto
void TCircunference::Copy(TCircunference *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built proyection point");

    //asigna las propiedades
    P = C->P;
    p_R = C->p_R;
}
TCircunference& TCircunference::operator=(const TCircunference &C)
{
    //asigna las propiedades
    P = C.P;
    p_R = C.p_R;

    return *this;
}

//contruye un clon de un objeto
TCircunference::TCircunference(TCircunference *C)
{
    Copy(C);
}

//MÉTODOS DE CONFIGURACIÓN:

//asigna conjuntamente los valores de las propiedades de definición
void TCircunference::SetValues(TDoublePoint P, double R)
{
    //comprueba las precondiciones
    if(R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna los nuevos valores
    P = P;
    p_R = R;
}

//MÉTODOS DE CÁLCULO DE DISTANCIAS:

//calcula la distancia mínima entre la figura y un punto
double TCircunference::distancePoint(TDoublePoint Q)
{
    double d = Mod(Q - P);
    if(d < p_R)
        return p_R - d;
    else if(d > p_R)
        return d - p_R;
    else
        return 0;
}
//calcula la distancia máxima entre la figura y un punto
double TCircunference::distancePointMax(TDoublePoint Q)
{
    return Mod(Q - P) + p_R;
}

//calcula la distancia mínima entre la figura y una circulo
double TCircunference::distanceCircle(TCircle *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    return distanceCircunferenceCircle(P, p_R, C->P, C->getR());
}
//calcula la distancia mínima entre la figura y una circunferencia
double TCircunference::distanceCircunference(TCircunference *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circunference");

    return distanceCircunferenceCircunference(P, p_R, C->P, C->getR());
}

//calcula la distancia mínima entre la figura y un segmento
double TCircunference::distanceSegment(TSegment *S)
{
    //comprueba las precondiciones
    if(S == NULL)
        throw EImproperArgument("pointer S should point to built segment");

    return distanceCircunferenceSegment(P, p_R, S->getPa(), S->getPb());
}
//calcula la distancia mínima entre la figura y un arco
double TCircunference::distanceArc(TArc *A)
{
    //comprueba las precondiciones
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built arc");

    return distanceCircunferenceArc(P, p_R, A->getPa(), A->getPb(),
                                    A->getPc(), A->getR());
}

//calcula la distancia mínima entre la figura y otra figura
double TCircunference::distance(TFigure *F)
{
    //comprueba las precondiciones
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //si no, si la figura es un círculo
    if(typeid(*F) == typeid(TCircle))
        //calcula la distancia mínima al objeto
        return distanceCircle((TCircle*)F);
    //si no, si la figura es una circunferencia
    else if(typeid(*F) == typeid(TCircunference))
        //calcula la distancia mínima al objeto
        return distanceCircunference((TCircunference*)F);

    //si la figura es un segmento
    else if(typeid(*F) == typeid(TSegment))
        //calcula la distancia mínima al objeto
        return distanceSegment((TSegment*)F);
    //si no, si la figura es un arco
    else if(typeid(*F) == typeid(TArc))
        //calcula la distancia mínima al objeto
        return distanceArc((TArc*)F);
    else
        throw EImpossibleError(AnsiString("unknown type: ")+AnsiString(typeid(*F).name()));
}

//MÉTODOS DE ROTACIÓN Y TRASLACIÓN:

//obtiene la figutra geométrica rotada theta radianes
//Figure debe contener una figura geométrica del mismo tipo
void TCircunference::getRotated(TFigure *F, double theta)
{
    //comprueba las precondiciones
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");
    if(typeid(*F) != typeid(TCircunference))
        throw EImproperArgument("pointer F should point to figure type TCircunference");

    //apunta la figura con un puntero del tipo TCircunference
    TCircunference *C = (TCircunference*)F;

    //calcula los coeficientes de rotación
    const double COS = cos(theta);
    const double SIN = sin(theta);

    //asigna los puntos rotados
    C->P.x = P.x*COS - P.y*SIN;
    C->P.y = P.x*SIN + P.y*COS;
}
//traslada la figura geométrica según el vector V
void TCircunference::translate(TDoublePoint V)
{
    //traslada los puntos de la figura
    P.x += V.x;
    P.y += V.y;
}

//--------------------------------------------------------------------------
//TContourFigure
//--------------------------------------------------------------------------

//vértice inicial de la figura
//valor por defecto: Pa
TDoublePoint TContourFigure::getPini(void) const
{
    if(inverted)
        return getPb();
    else
        return getPa();
}
void TContourFigure::setPini(const TDoublePoint Pini)
{
    if(inverted)
        setPb(Pini);
    else
        setPa(Pini);

}
//vértice final de la figura
//valor por defecto: Pb
TDoublePoint TContourFigure::getPfin(void) const
{
    if(inverted)
        return getPa();
    else
        return getPb();
}
void TContourFigure::setPfin(const TDoublePoint Pfin)
{
    if(inverted)
        setPa(Pfin);
    else
        setPb(Pfin);
}

//FUNCIONES ESTÁTICAS:

//imprime las propiedades de un objeto en una cadena de texto
//en formato texto entre paréntesis
void  TContourFigure::ContourFigurePrint(AnsiString& S,
                                         TContourFigure *F)
{
    TFigure::figurePrint(S, (TFigure*)F);
}
//lee las propiedades de un objeto en una cadena de texto
//en formato texto entre paréntesis
void  TContourFigure::ContourFigureRead(TContourFigure* &F,
                                        const AnsiString& S, int& i)
{
    try {
        TFigure::figureRead((TFigure*&)F, S, i);
    } catch(Exception& E) {
        E.Message.Insert(1, "reading contour figure: ");
        throw;
    }
}

//MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACIÓN:

//inicializa la propiedad Color a su valor por defecto
//Pa y Pb quedarán sin inicializar
TContourFigure::TContourFigure(void) : TFigure(),
    p_Pa(0, 0), p_Pb(0, 0), inverted(false)
{
}

//destructor virtual
TContourFigure::~TContourFigure()
{
}

//--------------------------------------------------------------------------
//TSegment
//--------------------------------------------------------------------------

//PROPIEDADES DE DEFINICIÓN:

//coordenadas cartesianas del punto inicial
//valor por defecto: (0, 0)
void TSegment::setPa(TDoublePoint Pa)
{
    p_Pa = Pa;
}
//coordenadas cartesianas del punto final
//valor por defecto: (0, 0)
void TSegment::setPb(TDoublePoint Pb)
{
    p_Pb = Pb;
}

//CONUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TSegment::getText(void) const
{
    return AnsiString("(")+
            p_Pa.getText()+AnsiString(", ")+
            p_Pb.getText()+AnsiString(")");
}
void TSegment::setText(const AnsiString& S)
{
    try {
        //read the value in a tanpon variable
        TSegment Segment;
        int i = 1;
        Read(&Segment, S, i);

        //busca texto inesperado
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text: "+StrFirstChars(S.SubString(i, S.Length() - i + 1)).str);

        //asigna la variable tampón
        this->Copy(&Segment);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting segment in text format: ");
        throw;
    }
}
AnsiString TSegment::getRowText(void) const
{
    return p_Pa.getText()+AnsiString("\t")+p_Pb.getText();
}
void TSegment::setRowText(const AnsiString& S)
{
    try {
        //read the value in a tanpon variable
        TSegment Segment;
        int i = 1;
        ReadRow(&Segment, S, i);

        //busca texto inesperado
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text: "+StrFirstChars(S.SubString(i, S.Length() - i + 1)).str);

        //asigna la variable tampón
        this->Copy(&Segment);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading segment in row text format: ");
        throw;
    }
}

//MÉTODOS ESTÁTICOS DE LISTA:

//imprime las propiedades de un objeto en una cadena de texto
//en formato texto entre paréntesis
void  TSegment::Print(AnsiString& S, TContourFigure *O)
{
    //comprueba las precondiciones
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    S += O->getText();
}
//imprime las propiedades de un objeto en una cadena de texto
//en formato línea de texto
void  TSegment::PrintRow(AnsiString& S, TContourFigure *O)
{
    //comprueba las precondiciones
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    S += O->getRowText();
}

//lee las propiedades de un objeto en una cadena de texto
//en formato texto entre paréntesis
void  TSegment::Read(TContourFigure *O_,
                     const AnsiString& S, int& i)
{
    //convierte el puntero a tipo concreto
    TSegment *O = (TSegment*)O_;

    //comprueba las precondiciones
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");
    if(i<1 || S.Length()<i)
        throw EImproperArgument("segment values not found");

    try {
        //lee el valor envariables tampón
        TDoublePoint Pa, Pb;
        StrTravelLabel("(", S,i);
        StrReadDPoint(&Pa, S, i);
        StrTravelLabel(",", S,i);
        StrReadDPoint(&Pb, S, i);
        StrTravelLabel(")", S,i);

        //asigna la variable tampón
        O->SetValues(Pa, Pb);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading value to segment in text format: ");
        throw;
    }
}
//lee las propiedades de un objeto en una cadena de texto
//en formato línea de texto
void  TSegment::ReadRow(TContourFigure *O_,
                        const AnsiString& S, int& i)
{
    //convierte a tipo concreto
    TSegment *O = (TSegment*)O_;

    //comprueba las precondiciones
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");
    if(i<1 || S.Length()<i)
        throw EImproperArgument("segment values not found");

    try {
        //lee el valor envariables tampón
        TDoublePoint Pa, Pb;
        StrReadDPoint(&Pa, S, i);
        StrTravelSeparators(S,i);
        StrReadDPoint(&Pb, S, i);

        //asigna la variable tampón
        O->SetValues(Pa, Pb);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading value to segment in row text format: ");
        throw;
    }
}

//MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

//contruye un objeto
TSegment::TSegment(void) : TContourFigure()
{
    p_Pa.x = 0;
    p_Pa.y = 0;
    p_Pb.x = 0;
    p_Pb.y = 0;
}
TSegment::TSegment(double ax, double ay, double bx, double by) : TContourFigure()
{
    p_Pa.x = ax;
    p_Pa.y = ay;
    p_Pb.x = bx;
    p_Pb.y = by;
}
TSegment::TSegment(TDoublePoint Pa, TDoublePoint Pb) : TContourFigure()
{
    p_Pa = Pa;
    p_Pb = Pb;
}

//copia las propiedades de un objeto
void TSegment::Copy(TSegment *O)
{
    //comprueba las precondiciones
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    //asigna las propiedades
    p_Pa = O->p_Pa;
    p_Pb = O->p_Pb;
    //#Color = O->Color;
}
TSegment& TSegment::operator=(const TSegment &O)
{
    //asigna las propiedades
    p_Pa = O.p_Pa;
    p_Pb = O.p_Pb;
    //#Color = O.Color;

    return *this;
}

//contruye un clon de un objeto
TSegment::TSegment(TSegment *O)
{
    Copy(O);
}

//MÉTODOS DE CONFIGURACIÓN:

//asigna conjuntamente los valores de las propiedades de definición
void TSegment::SetValues(TDoublePoint Pa, TDoublePoint Pb)
{
    //asigna los nuevos valores
    p_Pa = Pa;
    p_Pb = Pb;
}

//MÉTODOS DE CÁLCULO DE DISTANCIAS:

//calcula la distancia mínima entre la figura y un punto
double TSegment::distancePoint(TDoublePoint Q)
{
    return distanceSegmentPoint(p_Pa, p_Pb, Q);
}
//calcula la distancia máxima entre la figura y un punto
double TSegment::distancePointMax(TDoublePoint Q)
{
    return distanceSegmentPointMax(p_Pa, p_Pb, Q);
}

//calcula la distancia mínima entre la figura y una circulo
double TSegment::distanceCircle(TCircle *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    return distanceSegmentCircle(p_Pa, p_Pb, C->P, C->getR());
}
//calcula la distancia mínima entre la figura y una circunferencia
double TSegment::distanceCircunference(TCircunference *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circunference");

    return distanceSegmentCircunference(p_Pa, p_Pb, C->P, C->getR());
}

//calcula la distancia mínima entre la figura y un segmento
double TSegment::distanceSegment(TSegment *S)
{
    //comprueba las precondiciones
    if(S == NULL)
        throw EImproperArgument("pointer S should point to built segment");

    return distanceSegmentSegment(p_Pa, p_Pb, S->p_Pa, S->p_Pb);
}
//calcula la distancia mínima entre la figura y un arco
double TSegment::distanceArc(TArc *A)
{
    //comprueba las precondiciones
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built arc");

    return distanceSegmentArc(p_Pa, p_Pb, A->getPa(), A->getPb(),
                              A->getPc(), A->getR());
}

//calcula la distancia mínima entre la figura y otra figura
double TSegment::distance(TFigure *F)
{
    //comprueba las precondiciones
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //si no, si la figura es un círculo
    if(typeid(*F) == typeid(TCircle))
        //calcula la distancia mínima al objeto
        return distanceCircle((TCircle*)F);
    //si no, si la figura es una circunferencia
    else if(typeid(*F) == typeid(TCircunference))
        //calcula la distancia mínima al objeto
        return distanceCircunference((TCircunference*)F);

    //si no, si la figura es un segmento
    else if(typeid(*F) == typeid(TSegment))
        //calcula la distancia mínima al objeto
        return distanceSegment((TSegment*)F);
    //si no, si la figura es un arco
    else if(typeid(*F) == typeid(TArc))
        //calcula la distancia mínima al objeto
        return distanceArc((TArc*)F);
    else
        throw EImpossibleError(AnsiString("unknown type: ")+
                               AnsiString(typeid(*F).name()));
}

//MÉTODOS DE ROTACIÓN Y TRASLACIÓN

//obtiene la figutra geométrica rotada theta radianes
//Figure debe contener una figura geométrica del mismo tipo
void TSegment::getRotated(TFigure *F, double theta)
{
    //comprueba las precondiciones
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //el puntero F debería apauntar a una figura del tipo TSegment
    if(typeid(*F) != typeid(TSegment))
        throw EImproperArgument("pointer F should point to figure type TSegment");

    //apunta la figura con un puntero del tipo TSegment
    TSegment *S = (TSegment*)F;

    //calcula los coeficientes de rotación
    const double COS = cos(theta);
    const double SIN = sin(theta);

    //asigna los puntos rotados
    S->p_Pa.x = p_Pa.x*COS - p_Pa.y*SIN;
    S->p_Pa.y = p_Pa.x*SIN + p_Pa.y*COS;
    S->p_Pb.x = p_Pb.x*COS - p_Pb.y*SIN;
    S->p_Pb.y = p_Pb.x*SIN + p_Pb.y*COS;
}
//traslada la figura geométrica según el vector V
void TSegment::translate(TDoublePoint V)
{
    //traslada los puntos de la figura
    p_Pa.x += V.x;
    p_Pa.y += V.y;
    p_Pb.x += V.x;
    p_Pb.y += V.y;
}

//--------------------------------------------------------------------------
//TArc
//--------------------------------------------------------------------------

//PROPIEDADES DE DEFINICIÓN:

void TArc::setPa(TDoublePoint Pa)
{
    //comprueba las precondiciones
    if(Pa == p_Pc)
        throw EImproperArgument("vertex Pa should not be equal center Pc");
    if(!IsInRangeArc(Pa))
        throw EImproperArgument("point Pa should be in range of arc");

    //asigna el nuevo valor
    p_Pa = Pa;
}
void TArc::setPb(TDoublePoint Pb)
{
    //comprueba las precondiciones
    if(Pb == p_Pc)
        throw EImproperArgument("vertex Pb should not be equal center Pc");

    //el punto Pb debe estar en el entorno del arco
    if(!IsInRangeArc(Pb))
        throw EImproperArgument("point Pb should be in range of arc");

    //asigna el nuevo valor
    p_Pb = Pb;
}
void TArc::setPc(TDoublePoint Pc)
{
    //comprueba las precondiciones
    if(Pc == p_Pa)
        throw EImproperArgument("center Pc should not be equal vertex Pa");
    if(Pc == p_Pb)
        throw EImproperArgument("center Pc should not be equal vertex Pb");
    if(Abs(Mod(p_Pa - Pc) - p_R)>ERR_NUM || Abs(Mod(p_Pb - Pc) - p_R)>ERR_NUM)
        throw EImproperArgument("point Pa should be in range of a point in the arc");

    //asigna el nuevo valor
    p_Pc = Pc;
}
void TArc::setR(double R)
{
    //comprueba las precondiciones
    if(R <= 0)
        throw EImproperArgument("radio R should be upper zero");
    if(Abs(Mod(p_Pa - p_Pc) - R)>ERR_NUM || Abs(Mod(p_Pb - p_Pc) - R)>ERR_NUM)
        throw EImproperArgument("radio R should be such that Pa and Pb are in range arc");

    //asigna el nuevo valor
    p_R = R;
}

//PROPIEDADES DE DEFINICIÓN EN FORMATO TEXTO:

AnsiString TArc::getPaText(void) const
{
    return DPointToStr(p_Pa);
}
void TArc::setPaText(const AnsiString& S)
{
    try {
        setPa(StrToDPoint(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting value to Pa of an arc in text format: ");
        throw;
    }
}
AnsiString TArc::getPbText(void) const
{
    return DPointToStr(p_Pb);
}
void TArc::setPbText(const AnsiString& S)
{
    try {
        setPb(StrToDPoint(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting value to Pb of an arc in text format: ");
        throw;
    }
}
AnsiString TArc::getPcText(void) const
{
    return DPointToStr(p_Pc);
}
void TArc::setPcText(const AnsiString& S)
{
    try {
        setPc(StrToDPoint(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting value to Pc of an arc in text format: ");
        throw;
    }
}
AnsiString TArc::getRText(void) const
{
    return FloatToStr(p_R);
}
void TArc::setRText(const AnsiString& S)
{
    try {
        setR(StrToFloat(S));
    } catch(Exception& E) {
        E.Message.Insert(1, "setting value to R of an arc in text format: ");
        throw;
    }
}

AnsiString TArc::getText(void) const
{
    return AnsiString("(")+
            getPaText()+AnsiString(", ")+getPbText()+AnsiString(", ")+
            getPcText()+AnsiString(", ")+getRText()+AnsiString(")");
}
void TArc::setText(const AnsiString& S)
{
    try {
        //read the value in a tanpon variable
        TArc A;
        int i = 1;
        Read(&A, S, i);

        //busca texto inesperado
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text: "+StrFirstChars(S.SubString(i, S.Length() - i + 1)).str);

        //asigna la variable tampón
        this->Copy(&A);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting value to an arc in text format: ");
        throw;
    }
}
AnsiString TArc::getRowText(void) const
{
    return AnsiString("(")+
            getPaText()+AnsiString(", ")+getPbText()+AnsiString(", ")+
            getPcText()+AnsiString(", ")+getRText()+AnsiString(")");
}
void TArc::setRowText(const AnsiString& S)
{
    try {
        //read the value in a tanpon variable
        TArc A;
        int i = 1;
        ReadRow(&A, S, i);

        //busca texto inesperado
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text: "+StrFirstChars(S.SubString(i, S.Length() - i + 1)).str);

        //asigna la variable tampón
        this->Copy(&A);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting value to an arc in row text format: ");
        throw;
    }
}

//FUNCIONES ESTÁTICAS:

//imprime las propiedades de un objeto en una cadena de texto
//en formato texto entre paréntesis
void  TArc::Print(AnsiString& S,
                  TContourFigure *O)
{
    //comprueba las precondiciones
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    S += O->getText();
}
//imprime las propiedades de un objeto en una cadena de texto
//en formato línea de texto
void  TArc::PrintRow(AnsiString& S, TContourFigure *O)
{
    //comprueba las precondiciones
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    S += O->getRowText();
}

//lee las propiedades de un objeto en una cadena de texto
//en formato texto entre paréntesis
void  TArc::Read(TContourFigure *_O,
                 const AnsiString& S, int& i)
{
    //convierte a tipo concreto
    TArc *O = (TArc*)_O;

    //comrpueba las precondiciones
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");
    if(i<1 || S.Length()<i)
        throw EImproperArgument("arc values not found");

    try {
        //lee el valor en variables tampón
        StrTravelLabel("(", S, i);
        TDoublePoint Pa; //punto central
        StrReadDPoint(&Pa, S, i);
        StrTravelLabel(",", S, i);
        TDoublePoint Pb; //punto incial (en sentido dextrógiro)
        StrReadDPoint(&Pb, S, i);
        StrTravelLabel(",", S, i);
        TDoublePoint Pc; //punto final (en sentido dextrógiro)
        StrReadDPoint(&Pc, S, i);
        StrTravelLabel(",", S, i);
        double R; //radio
        StrReadFloat(R, S, i);
        StrTravelLabel(")", S, i);

        //asigna las variables tampón
        O->SetValues(Pa, Pb, Pc, R);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading value to an arc in text format: ");
        throw;
    }
}
//lee las propiedades de un objeto en una cadena de texto
//en formato línea de texto
void  TArc::ReadRow(TContourFigure *_O,
                    const AnsiString& S, int& i)
{
    //convierte a tipo concreto
    TArc *O = (TArc*)_O;

    //coprueba las precondiciones
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");
    if(i<1 || S.Length()<i)
        throw EImproperArgument("arc values not found");

    try {
        //lee el valor en variables tampón
        TDoublePoint Pa; //punto central
        StrReadDPoint(&Pa, S, i);
        StrTravelSeparators(S, i);
        TDoublePoint Pb; //punto incial (en sentido dextrógiro)
        StrReadDPoint(&Pb, S, i);
        StrTravelSeparators(S, i);
        TDoublePoint Pc; //punto final (en sentido dextrógiro)
        StrReadDPoint(&Pc, S, i);
        StrTravelSeparators(S, i);
        double R; //radio
        StrReadFloat(R, S, i);

        //asigna las variables tampón
        O->SetValues(Pa, Pb, Pc, R);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading value to an arc in row text format: ");
        throw;
    }

}

//MÉTODOS DE CONTRUCCIÓN,COPIA Y CLONACIÓN:

//contruye un objeto
TArc::TArc(void) : TContourFigure(),
    p_R(1)
{
    p_Pa.x = 1;
    p_Pa.y = 0;
    p_Pb.x = 1;
    p_Pb.y = 0;
    p_Pc.x = 0;
    p_Pc.y = 0;
}
TArc::TArc(double ax, double ay, double bx, double by, double cx, double cy,
           double R) : TContourFigure()
{
    //ADVERTENCIA: aquí no debe usarse la función IsArc, por que
    //dicha función no indica la causa de que (Pa, Pb, Pc, R)
    //no sea un arco.

    //asigna las coordenadas a puntos auxiliares
    TDoublePoint Pa; Pa.x = ax; Pa.y = ay;
    TDoublePoint Pb; Pb.x = bx; Pb.y = by;
    TDoublePoint Pc; Pc.x = cx; Pc.y = cy;

    //comprueba las precondiciones
    if(Pa == Pc)
        throw EImproperArgument("vertex Pa should not be equal center Pc");
    if(Pb == Pc)
        throw EImproperArgument("vertex Pb should not be equal center Pc");
    if(R < 0)
        throw EImproperArgument("radio R should be nonnegative");
    if(Abs(Mod(Pa - Pc) - R) > ERR_NUM)
        throw EImproperArgument("point Pa should be in range of a point in the arc");
    if(Abs(Mod(Pb - Pc) - R) > ERR_NUM)
        throw EImproperArgument("point Pb should be in range of a point in the arc");

    //asigna los nuevos valores
    p_Pa = Pa;
    p_Pb = Pb;
    p_Pc = Pc;
    p_R = R;
}
TArc::TArc(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc,
           double R) :
    TContourFigure()
{
    //ADVERTENCIA: aquí no debe usarse la función IsArc, por que
    //dicha función no indica la causa de que (Pa, Pb, Pc, R)
    //no sea un arco.

    //comprueba las precondiciones
    if(Pa == Pc)
        throw EImproperArgument("vertex Pa should not be equal center Pc");
    if(Pb == Pc)
        throw EImproperArgument("vertex Pb should not be equal center Pc");
    if(R < 0)
        throw EImproperArgument("radio R should be nonnegative");
    if(Abs(Mod(Pa - Pc) - R) > ERR_NUM)
        throw EImproperArgument("point Pa should be in range of a point in the arc");
    if(Abs(Mod(Pb - Pc) - R) > ERR_NUM)
        throw EImproperArgument("point Pb should be in range of a point in the arc");

    //asigna los nuevos valores
    p_Pa = Pa;
    p_Pb = Pb;
    p_Pc = Pc;
    p_R = R;
}

//copia las propiedades de un objeto
void TArc::Copy(TArc *O)
{
    //comprueba las precondiciones
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    //asigna las propiedades
    p_Pa = O->p_Pa;
    p_Pb = O->p_Pb;
    p_Pc = O->p_Pc;
    p_R = O->p_R;
}
TArc& TArc::operator=(const TArc &O)
{
    //asigna las propiedades
    p_Pa = O.p_Pa;
    p_Pb = O.p_Pb;
    p_Pc = O.p_Pc;
    p_R = O.p_R;

    return *this;
}

//contruye un clon de un objeto
TArc::TArc(TArc *O)
{
    Copy(O);
}

//MÉTODOS DE CONFIGURACIÓN:

//determina si un punto está en el entorno del arco
//      Abs(Mod(P - Pc) - R) <= ERR_NUM
bool TArc::IsInRangeArc(TDoublePoint P)
{
    //si el punto P está a una distancia del arco superior a epsilon
    if(Abs(Mod(P - p_Pc) - p_R) > ERR_NUM)
        return false; //indica que P no está en el entorno del arco

    return true; //indica que P si está en el entorno del arrco
}

//asigna los valores de las propiedades conjuntamente
//Lanza una excepción EImproperArgument si:
//      Pa o Pb es igual a Pc,
//      R no es mayor que cero,
//      Pa o Pb no está en el entorno del arco,
void TArc::SetValues(TDoublePoint Pa, TDoublePoint Pb, TDoublePoint Pc, double R)
{
    //ADVERTENCIA: aquí no debe usarse la función IsArc, por que
    //dicha función no indica la causa de que (Pa, Pb, Pc, R)
    //no sea un arco.

    //comprueba las precondiciones
    if(Pa == Pc)
        throw EImproperArgument("vertex Pa should not be equal center Pc");
    if(Pb == Pc)
        throw EImproperArgument("vertex Pb should not be equal center Pc");
    if(R < 0)
        throw EImproperArgument("radio R should be nonnegative");

    //el punto Pa debe estar en el entorno de un punto del arco
    //    double aux = Abs(Mod(Pa - Pc) - R);
    //    if(aux > ERR_NUM)
    //      throw EImproperArgument("point Pa should be in range of a point in the arc");

    //el punto Pb debe estar en el entorno de un punto del arco
    //    aux = Abs(Mod(Pb - Pc) - R);
    //    if(aux > ERR_NUM)
    //      throw EImproperArgument("point Pb should be in range of a point in the arc");

    //corriege el exceso de error numérico introducido por el usuario en Pa
    TDoublePoint Pa_;
    double aux = Abs(Mod(Pa - Pc) - R);
    if(aux > ERR_NUM) {
        TDoublePoint v1 = Pa - Pc;
        Pa_ = Pc + v1/Mod(v1)*R;
    } else
        Pa_ = Pa;

    //corriege el exceso de error numérico introducido por el usuario en Pb
    TDoublePoint Pb_;
    aux = Abs(Mod(Pb - Pc) - R);
    if(aux > ERR_NUM) {
        TDoublePoint v1 = Pb - Pc;
        Pb_ = Pc + v1/Mod(v1)*R;
    } else
        Pb_ = Pb;

    //asigna los nuevos valores
    p_Pa = Pa_;
    p_Pb = Pb_;
    p_Pc = Pc;
    p_R = R;
}

//MÉTODOS DE CÁLCULO DE DISTANCIAS:

//calcula la distancia mínima entre la figura y un punto
double TArc::distancePoint(TDoublePoint Q)
{
    return distanceArcPoint(p_Pa, p_Pb, p_Pc, p_R, Q);
}
//calcula la distancia máxima entre la figura y un punto
double TArc::distancePointMax(TDoublePoint Q)
{
    return distanceArcPointMax(p_Pa, p_Pb, p_Pc, p_R, Q);
}

//calcula la distancia mínima entre la figura y una circunferencia
double TArc::distanceCircunference(TCircunference *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circunference");

    return distanceArcCircunference(p_Pa, p_Pb, p_Pc, p_R, C->P, C->getR());
}
//calcula la distancia mínima entre la figura y una circulo
double TArc::distanceCircle(TCircle *C)
{
    //comprueba las precondiciones
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    return distanceArcCircle(p_Pa, p_Pb, p_Pc, p_R, C->P, C->getR());
}

//calcula la distancia mínima entre la figura y un segmento
double TArc::distanceSegment(TSegment *A)
{
    //comprueba las precondiciones
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built arc");

    return distanceArcSegment(p_Pa, p_Pb, p_Pc, p_R, A->getPa(), A->getPb());
}
//calcula la distancia mínima entre la figura y un arco
double TArc::distanceArc(TArc *A)
{
    //comprueba las precondiciones
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built arc");

    return distanceArcArc(p_Pa, p_Pb, p_Pc, p_R,
                          A->p_Pa, A->p_Pb, A->p_Pc, A->p_R);
}

//calcula la distancia mínima entre la figura y otra figura
double TArc::distance(TFigure *F)
{
    //comprueba las precondiciones
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //si la figura es un círculo
    if(typeid(*F) == typeid(TCircle))
        //calcula la distancia mínima al objeto
        return distanceCircle((TCircle*)F);
    //si no, si la figura es una circunferencia
    else if(typeid(*F) == typeid(TCircunference))
        //calcula la distancia mínima al objeto
        return distanceCircunference((TCircunference*)F);

    //si no, si la figura es un segmento
    else if(typeid(*F) == typeid(TSegment))
        //calcula la distancia mínima al objeto
        return distanceSegment((TSegment*)F);
    //si no, si la figura es un arco
    else if(typeid(*F) == typeid(TArc))
        //calcula la distancia mínima al objeto
        return distanceArc((TArc*)F);
    else
        throw EImpossibleError(AnsiString("unknown type: ")+AnsiString(typeid(*F).name()));
}

//MÉTODOS DE ROTACIÓN Y TRASLACIÓN

//obtiene la figutra geométrica rotada theta radianes
//Figure debe contener una figura geométrica del mismo tipo
void TArc::getRotated(TFigure *F, double theta)
{
    //comprueba las precondiciones
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");
    if(typeid(*F) != typeid(TArc))
        throw EImproperArgument("pointer F should point to figure type TArc");

    //apunta la figura con un puntero del tipo TArc
    TArc *A = (TArc*)F;

    //calcula los coeficientes de rotación
    const double COS = cos(theta);
    const double SIN = sin(theta);

    //asigna los puntos rotados
    A->p_Pa.x = p_Pa.x*COS - p_Pa.y*SIN;
    A->p_Pa.y = p_Pa.x*SIN + p_Pa.y*COS;
    A->p_Pb.x = p_Pb.x*COS - p_Pb.y*SIN;
    A->p_Pb.y = p_Pb.x*SIN + p_Pb.y*COS;
    A->p_Pc.x = p_Pc.x*COS - p_Pc.y*SIN;
    A->p_Pc.y = p_Pc.x*SIN + p_Pc.y*COS;
}
//traslada la figura geométrica según el vector V
void TArc::translate(TDoublePoint V)
{
    //traslada los puntos de la figura
    p_Pa.x += V.x;
    p_Pb.x += V.x;
    p_Pc.x += V.x;
    p_Pa.y += V.y;
    p_Pb.y += V.y;
    p_Pc.y += V.y;
}

//--------------------------------------------------------------------------
//FUNCIONES RELACIONADAS:

//determina el ángulo entre el vértice inicial de una figura de contorno
//y su vértice final en torno de un punto P
double rotation(TContourFigure *F, bool PbIsNext, TDoublePoint P)
{
    //comprueba las precondiciones
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //si el puntero F apunta a un segmento
    if(typeid(*F) == typeid(TSegment)) {
        //si el giro se produce de Pa a Pb
        if(PbIsNext)
            return Angle(F->getPa() - P, F->getPb() - P);
        else
            return Angle(F->getPb() - P, F->getPa() - P);
    }
    //si no, si el puntero F apunta a un arco contruido
    if(typeid(*F) == typeid(TArc)) {
        //apunta la figura con un puntero concreto
        TArc *A = (TArc*)F;
        //variable auxiliar
        double theta;
        //si el giro se produce de Pa a Pb
        if(PbIsNext) {
            //calcula el ángulo de desplazamiento de Pa a Pb
            theta = Angle(A->getPa() - P, A->getPb() - P);
            //si el punto P está en el interior del arco
            if(distanceLineToPoint(A->getPa(), A->getPb(), P)>=0 && Mod(P - A->getPc())<=A->getR())
                //devuelve el ángulo invertido
                return -theta;
            //devuelve el ángulo
            return theta;
        } else {
            //calcula el ángulo de desplazamiento de Pb a Pa
            theta = Angle(A->getPb() - P, A->getPa() - P);
            //si el punto P está en el interior del arco
            if(distanceLineToPoint(A->getPa(), A->getPb(), P)>=0 && Mod(P - A->getPc())<=A->getR())
                //devuelve el ángulo invertido
                return -theta;
            //devuelve el ángulo
            return theta;
        }
    }

    //si no es segmento ni arco

    //indica que el puntero F debería apuntar a un segmento o un arco
    throw EImpossibleError("pointer F should point to a segment or an arc");
}

//determina si dos figuras son diferentes
bool areUnequals(const TFigure *F1, const TFigure *F2)
{
    if(typeid(F1) != typeid(F2))
        return true;

    if(typeid(*F1) == typeid(TCircle)) {
        TCircle *C1 = (TCircle*)F1;
        TCircle *C2 = (TCircle*)F2;
        if(C1->P != C2->P || C1->getR() != C2->getR())
            return true;
    } else if(typeid(*F1) == typeid(TCircunference)) {
        TCircle *C1 = (TCircle*)F1;
        TCircle *C2 = (TCircle*)F2;
        if(C1->P != C2->P || C1->getR() != C2->getR())
            return true;
    } else if(typeid(*F1) == typeid(TSegment)) {
        TSegment *S1 = (TSegment*)F1;
        TSegment *S2 = (TSegment*)F2;
        if(S1->getPa() != S2->getPa() || S1->getPb() != S2->getPb())
            return true;
    } else if(typeid(*F1) == typeid(TArc)) {
        TArc *A1 = (TArc*)F1;
        TArc *A2 = (TArc*)F2;
        if(A1->getPa() != A2->getPa() || A1->getPb() != A2->getPb() || A1->getPc() != A2->getPc())
            return true;
    }
    return false;
}

//determine if there is intersections between cf1 and cf2
bool intersection(TContourFigure *cf1, TContourFigure *cf2)
{
    if(cf1 == NULL)
        throw EImproperArgument("pointer cf1 should point to built contour figure");
    if(cf2 == NULL)
        throw EImproperArgument("pointer cf2 should point to built contour figure");

    if(typeid(*cf1) == typeid(TSegment) && typeid(*cf2) == typeid(TSegment)) {
        TSegment *s1 = (TSegment*)cf1;
        TSegment *s2 = (TSegment*)cf2;
        TDoublePoint p;
        bool aux = intersectionSegmentSegment(p, s1->getPa(), s1->getPb(),
                                                 s2->getPa(), s2->getPb());
        return aux;
    }
    if(typeid(*cf1) == typeid(TSegment) && typeid(*cf2) == typeid(TArc)) {
        TSegment *s1 = (TSegment*)cf1;
        TArc *a2 = (TArc*)cf2;
        bool aux = intersectionSegmentArc(s1->getPa(), s1->getPb(),
                                          a2->getPa(), a2->getPb(), a2->getPc(), a2->getR());
        return aux;
    }
    if(typeid(*cf1) == typeid(TArc) && typeid(*cf2) == typeid(TSegment)) {
        TArc *a1 = (TArc*)cf1;
        TSegment *s2 = (TSegment*)cf2;
        bool aux = intersectionArcSegment(a1->getPa(), a1->getPb(), a1->getPc(), a1->getR(),
                                          s2->getPa(), s2->getPb());
        return aux;
    }
    if(typeid(*cf1) == typeid(TArc) && typeid(*cf2) == typeid(TArc)) {
        TArc *a1 = (TArc*)cf1;
        TArc *a2 = (TArc*)cf2;
        bool aux = intersectionArcArc(a1->getPa(), a1->getPb(), a1->getPc(), a1->getR(),
                                      a2->getPa(), a2->getPb(), a2->getPc(), a2->getR());
        return aux;
    }
    throw EImproperArgument("contour figures cf1 and cf2 should be either segment or arc");
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

