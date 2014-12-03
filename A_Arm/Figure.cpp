//---------------------------------------------------------------------------
//Archivo: Figure.cpp
//Contenido: clases de figuras geométricas
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Figure.h"
#include "..\2_Strings\Strings.h"
#include "..\1_Mathematics\Geometry.h"
#include "..\1_Mathematics\Scalars.h"

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

//PROPIEDADES GRÁFICAS EN FORMATO TEXTO:
/*#
AnsiString TFigure::getColorText(void) const
{
    return ColorToString(Color);
}
void TFigure::setColorText(const AnsiString& S)
{
    try {
        Color = StringToColor(S);
    } catch(...) {
        throw;
    }
}

//MÉTODOS ESTÁTICOS DE COMPARACIÓN:

//compara el color de dos figuras
//      si F1->Color < F2->Color entonces CompareColor = -1
//      si F1->Color > F2->Color entonces CompareColor = 1
//      si F1->Color == F2->Color entonces CompareColor = 0
int __fastcall TFigure::CompareColor(const TFigure *F1, const TFigure *F2)
{
    //el puntero F1 debería apuntar a una figura contruida
    if(F1 == NULL)
        throw EImproperArgument("pointer F1 should point to built figure");

    //el puntero F2 debería apuntar a una figura contruida
    if(F2 == NULL)
        throw EImproperArgument("pointer F2 should point to built figure");

    if(F1->Color.rgba() < F2->Color.rgba())
        return -1;
    if(F1->Color.rgba() > F2->Color.rgba())
        return 1;
    return 0;
}
*/
//MÉTODOS ESTÁTICOS DE IMPRESIÓN Y LECTURA:

//imprime las propiedades de un objeto en una cadena
//en formato texto entre paréntesis
void __fastcall TFigure::FigurePrint(AnsiString& S,
                                     TFigure *F)
{
    //el puntero F debería apuntar a una figura contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //añade el texto a la cadena
    S += F->getText();
}
//imprime las propiedades de un objeto en una cadena
//en formato fila de texto
void __fastcall TFigure::FigurePrintRow(AnsiString& S,
                                        TFigure *F)
{
    //el puntero F debería apuntar a una figura contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //añade el texto a la cadena
    S += F->getRowText();
}

//lee las propiedades de un objeto en una cadena de texto
//en formato texto entre paréntesis
void __fastcall TFigure::FigureRead(TFigure* &F, const AnsiString& S, int& i)
{
    //el índice i debe indicar una posición de la cadena S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("index i should indicate a position in string S");

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
//lee las propiedades de un objeto en una cadena de texto
//en formato fila de texto
void __fastcall TFigure::FigureReadRow(TFigure* &F, const AnsiString& S, int& i)
{
    //el índice i debe indicar una posición de la cadena S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("index i should indicate a position in string S");

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

void TCircle::setR(double _R)
{
    //el radio debe ser mayor que cero
    if(_R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna el nuevo valor
    __R = _R;
}

//CONJUNTOS DE PROPIEDADES DE DEFINICIÓN EN FORMATO TEXTO:

AnsiString TCircle::getRText(void) const
{
    return FloatToStr(__R);
}
void TCircle::setRText(const AnsiString& S)
{
    try {
        setR(StrToFloat_(S));
    } catch(...) {
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
        int i = 1;
        Read(this, S, i);

        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("string S shouldcontain a single circle only");

    } catch(...) {
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
        int i = 1;
        ReadRow(this, S, i);
        StrTravelToEnd(S, i);

        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("string S shouldcontain a single circle only");

    } catch(...) {
        throw;
    }
}
AnsiString TCircle::getAssignsText(void) const
{
    AnsiString S;

    S = AnsiString("x = ")+P.getxText()+AnsiString("\r\n");
    S += AnsiString("y = ")+P.getyText()+AnsiString("\r\n");
    S += AnsiString("R = ")+getRText()+AnsiString("\r\n");

    return S;
}
void TCircle::setAssignsText(const AnsiString& S)
{
    try {
        int i = 1;
        ReadAssigns(this, S, i);
        StrTravelToEnd(S, i);

        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("string S shouldcontain a single circle only");

    } catch(...) {
        throw;
    }
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

int __fastcall TCircle::Comparex(const TCircle *C1,
                                 const TCircle *C2)
{
    //el puntero C1 debería apuntar a un círculo contruido
    if(C1 == NULL)
        throw EImproperArgument("pointer C1 should point to built circle");

    //el puntero C2 debería apuntar a un círculo contruido
    if(C2 == NULL)
        throw EImproperArgument("pointer C2 should point to built circle");

    if(C1->P.x < C2->P.x)
        return -1;
    if(C1->P.x > C2->P.x)
        return 1;
    return 0;
}
int __fastcall TCircle::Comparey(const TCircle *C1,
                                 const TCircle *C2)
{
    //el puntero C1 debería apuntar a un círculo contruido
    if(C1 == NULL)
        throw EImproperArgument("pointer C1 should point to built circle");

    //el puntero C2 debería apuntar a un círculo contruido
    if(C2 == NULL)
        throw EImproperArgument("pointer C2 should point to built circle");

    if(C1->P.y < C2->P.y)
        return -1;
    if(C1->P.y > C2->P.y)
        return 1;
    return 0;
}
int __fastcall TCircle::CompareR(const TCircle *C1,
                                 const TCircle *C2)
{
    //el puntero C1 debería apuntar a un círculo contruido
    if(C1 == NULL)
        throw EImproperArgument("pointer C1 should point to built circle");

    //el puntero C2 debería apuntar a un círculo contruido
    if(C2 == NULL)
        throw EImproperArgument("pointer C2 should point to built circle");

    if(C1->getR() < C2->getR())
        return -1;
    if(C1->getR() > C2->getR())
        return 1;
    return 0;
}

//MÉTODOS ESTÁTICOS DE IMPRESIÓN:

void __fastcall TCircle::Print(AnsiString& S, const TCircle *C)
{
    //el puntero C debería apuntar a un círculo
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    S += C->getText();
}
void __fastcall TCircle::PrintRow(AnsiString& S, const TCircle *C)
{
    //el puntero C debería apuntar a un círculo
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    S += C->getRowText();
}
void __fastcall TCircle::PrintAssigns(AnsiString& S, const TCircle *C)
{
    //el puntero C debería apuntar a un círculo
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    S += C->getAssignsText();
}

//MÉTODOS ESTÁTICOS DE LECTURA:

void __fastcall TCircle::Read(TCircle *C,
                              const AnsiString& S, int& i)
{
    //el puntero C debería apuntar a un círculo construido
    if(C == NULL)
        throw EImproperArgument("pointer C shoult point to built circle");

    //si el índice i no indica a una posición de la cadena
    if(i<1 || S.Length()<i)
        //indica que no se han encontrado los valores del círculo
        throw EImproperArgument("circle values not founds");

    //estado de lectura:
    //      0: esperando separador o '('
    //      1: esperando separador o TDoublePoint P
    //      2: esperando separador o ','
    //      3: esperando separador o double R
    //      4: esperando separador o ')'
    //      5: conjunto leido con éxito
    int status = 0;

    //variables tampón en formato conveniente
    TCircle _C;

    do {
        //reacciona según el estado
        switch(status) {
        case 0: //esperando separador o '('
            try {
            StrTravelLabel("(", S,i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 1: //esperando separador o TDoublePoint P
            try {
            TDoublePoint aux;
            StrReadDPoint(&aux, S, i);
            _C.P = aux;
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 2: //esperando separador o ','
            try {
            StrTravelLabel(",", S,i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 3: //esperando separador o double R
            try {
            double aux;
            StrReadFloat(aux, S, i);
            _C.setR(aux);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 4: //esperando separador o ')'
            try {
            StrTravelLabel(")", S,i);
        } catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientrasel conjunto no se haya leido con éxito
    } while(status < 5);

    //asigna la variable tampón
    *C = _C;
}
void __fastcall TCircle::ReadRow(TCircle *C,
                                 const AnsiString& S, int& i)
{
    //el puntero C debería apuntar a un círculo construido
    if(C == NULL)
        throw EImproperArgument("pointer C shoult point to built circle");

    //si el índice i no indica a una posición de la cadena
    if(i<1 || S.Length()<i)
        //indica que no se han encontrado los valores del círculo
        throw EImproperArgument("circle values not founds");

    //estado de lectura
    //      0: esperando valor para x
    //      1: esperando separador
    //      2: esperando valor para y
    //      3: esperando separador
    //      4: esperando valor para R
    //      5: conjunto leido con éxito
    int status = 0;

    //variables auxiliares
    TCircle _C(C); //variable tampón
    AnsiString Ident; //identificador de propiedad
    AnsiString Value; //valor de propiedad

    //ADVERTENCIA: las variables tampón con propiedades interdependientes
    //deben ser clones de las variables que se pretenden modificar.

    //NOTA: adviertase que las propiedades enteras son leidas como
    //valores en punto flotante para detectar errores en los cuales
    //sea especificado un valor en punto flotante en vez de un valor entero.

    do {
        switch(status) {
        case 0: //esperando valor para x
            try {
            double aux;
            StrReadFloat(aux, S, i);
            _C.P.x = aux;
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 1: //esperando separador
            try {
            StrTravelSeparators(S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 2: //esperando valor para y
            try {
            double aux;
            StrReadFloat(aux, S, i);
            _C.P.y = aux;
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 3: //esperando separador
            try {
            StrTravelSeparators(S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 4: //esperando valor para R
            try {
            double aux;
            StrReadFloat(aux, S, i);
            _C.setR(aux);
        } catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientras el conjunto no se haya leido con éxito
    } while(status < 5);

    //asigna la variable tampón
    *C = _C;
}
void __fastcall TCircle::ReadAssigns(TCircle *C,
                                     const AnsiString& S, int& i)
{
}

//MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACIÓN:

//contruye un objeto
TCircle::TCircle(void) : TFigure(),
    __R(1)
{
    P.x = 0;
    P.y = 0;
}
TCircle::TCircle(double x, double y, double _R)
{
    //el radio debe ser mayor que cero
    if(_R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna el nuevo valor
    P.x = x;
    P.y = y;
    __R = _R;

}
TCircle::TCircle(TDoublePoint _P, double _R)
{
    //el radio debe ser mayor que cero
    if(_R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna el nuevo valor
    P = _P;
    __R = _R;

}

//copia las propiedades de un objeto
void TCircle::Copy(TCircle *C)
{
    //el puntero C debería apuntar a un círculo construido
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built proyection point");

    //asigna las propiedades
    P = C->P;
    __R = C->__R;
    //#Color = C->Color;
}
TCircle& TCircle::operator=(const TCircle &C)
{
    //asigna las propiedades
    P = C.P;
    __R = C.__R;
    //#Color = C.Color;

    return *this;
}

//contruye un clon de un objeto
TCircle::TCircle(TCircle *PP)
{
    try {
        Copy(PP);
    } catch(...) {
        throw;
    }
}

//MÉTODOS DE CONFIGURACIÓN:

//asigna conjuntamente los valores de las propiedades de definición
void TCircle::SetValues(TDoublePoint _P, double _R)
{
    //el radio debe ser mayor que cero
    if(_R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna los nuevos valores
    P = _P;
    __R = _R;
}

//MÉTODOS DE CÁLCULO DE DISTANCIAS:

//calcula la distancia mínima entre la figura y un punto
double TCircle::DistancePoint(TDoublePoint Q)
{
    double d = Mod(Q - P);

    if(d <= __R)
        return 0;
    else
        return d - __R;
}
//calcula la distancia máxima entre la figura y un punto
double TCircle::DistancePointMax(TDoublePoint Q)
{
    return Mod(Q - P) + __R;
}

//calcula la distancia mínima entre la figura y una circulo
double TCircle::DistanceCircle(TCircle *C)
{
    //el puntero C debería apuntar a un círculio contruido
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    return DistanceCircleCircle(P, __R, C->P, C->getR());
}
//calcula la distancia mínima entre la figura y una circunferencia
double TCircle::DistanceCircunference(TCircunference *C)
{
    //el puntero C debería apuntar a una circunferencia contruida
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circunference");

    return DistanceCircleCircunference(P, __R, C->P, C->getR());
}

//calcula la distancia mínima entre la figura y un segmento
double TCircle::DistanceSegment(TSegment *S)
{
    //el puntero S debería apuntar a un segmento contruido
    if(S == NULL)
        throw EImproperArgument("pointer S should point to built segment");

    return DistanceCircleSegment(P, __R, S->getPa(), S->getPb());
}
//calcula la distancia mínima entre la figura y un arco
double TCircle::DistanceArc(TArc *A)
{
    //el puntero A debería apuntar a un punto contruido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built arc");

    return DistanceCircleArc(P, __R, A->getPa(), A->getPb(), A->getPc(), A->getR());
}

//calcula la distancia mínima entre la figura y otra figura
double TCircle::Distance(TFigure *F)
{
    //el puntero F debería apuntar a una figura contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //si no, si la figura es un círculo
    if(typeid(*F) == typeid(TCircle))
        //calcula la distancia mínima al objeto
        return DistanceCircle((TCircle*)F);
    //si no, si la figura es una circunferencia
    else if(typeid(*F) == typeid(TCircunference))
        //calcula la distancia mínima al objeto
        return DistanceCircunference((TCircunference*)F);

    //si la figura es un segmento
    else if(typeid(*F) == typeid(TSegment))
        //calcula la distancia mínima al objeto
        return DistanceSegment((TSegment*)F);
    //si no, si la figura es un arco
    else if(typeid(*F) == typeid(TArc))
        //calcula la distancia mínima al objeto
        return DistanceArc((TArc*)F);
    else
        throw EImposibleError(AnsiString("unknown type: ")+AnsiString(typeid(*F).name()));
}

//MÉTODOS DE ROTACIÓN Y TRASLACIÓN:

//obtiene la figutra geométrica rotada theta radianes
//Figure debe contener una figura geométrica del mismo tipo
void TCircle::GetRotated(TFigure *F, double theta)
{
    //el puntero F debería apuntar a una figura contruida
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
void TCircle::Translate(TDoublePoint V)
{
    //traslada los puntos de la figura
    P.x += V.x;
    P.y += V.y;
}

//MÉTODOS GRÁFICOS:
/*#
//dibuja el círculo
//en un trazador de formas
void TCircle::Paint(TPloterShapes *PS)
{
    //el puntero PS debería apuntar a un trazador de formas construido
    if(PS == NULL)
        throw EImproperArgument("pointer PS should point to built ploter shapes");

    //configura el color de la pluma
    PS->setPenColor(Color);
    //pinta un circulito en el lugar del objeto
    PS->Circle(P, __R);
}
*/
//--------------------------------------------------------------------------
//TCircunference
//--------------------------------------------------------------------------

//PROPIEDADES ESTÁTICAS:

//inicializa las propiedades estáticas
AnsiString TCircunference::RLabel = "R";

//PROPIEDADES DE DEFINICIÓN:

void TCircunference::setR(double _R)
{
    //el radio debe ser mayor que cero
    if(_R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna el nuevo valor
    __R = _R;
}

//CONJUNTOS DE PROPIEDADES DE DEFINICIÓN EN FORMATO TEXTO:

AnsiString TCircunference::getRText(void) const
{
    return FloatToStr(__R);
}
void TCircunference::setRText(const AnsiString& S)
{
    try {
        setR(StrToFloat_(S));
    } catch(...) {
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

    } catch(...) {
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
        int i = 1;
        ReadRow(this, S, i);
        StrTravelToEnd(S, i);

        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("string S shouldcontain a single circle only");

    } catch(...) {
        throw;
    }
}
AnsiString TCircunference::getAssignsText(void) const
{
    AnsiString S;

    S = AnsiString("x = ")+P.getxText()+AnsiString("\r\n");
    S += AnsiString("y = ")+P.getyText()+AnsiString("\r\n");
    S += AnsiString("R = ")+getRText()+AnsiString("\r\n");

    return S;
}
void TCircunference::setAssignsText(const AnsiString& S)
{
    try {
        int i = 1;
        ReadAssigns(this, S, i);
        StrTravelToEnd(S, i);

        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("string S shouldcontain a single circle only");

    } catch(...) {
        throw;
    }
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

int __fastcall TCircunference::Comparex(const TCircunference *C1,
                                        const TCircunference *C2)
{
    //el puntero C1 debería apuntar a un círculo contruido
    if(C1 == NULL)
        throw EImproperArgument("pointer C1 should point to built circle");

    //el puntero C2 debería apuntar a un círculo contruido
    if(C2 == NULL)
        throw EImproperArgument("pointer C2 should point to built circle");

    if(C1->P.x < C2->P.x)
        return -1;
    if(C1->P.x > C2->P.x)
        return 1;
    return 0;
}
int __fastcall TCircunference::Comparey(const TCircunference *C1,
                                        const TCircunference *C2)
{
    //el puntero C1 debería apuntar a un círculo contruido
    if(C1 == NULL)
        throw EImproperArgument("pointer C1 should point to built circle");

    //el puntero C2 debería apuntar a un círculo contruido
    if(C2 == NULL)
        throw EImproperArgument("pointer C2 should point to built circle");

    if(C1->P.y < C2->P.y)
        return -1;
    if(C1->P.y > C2->P.y)
        return 1;
    return 0;
}
int __fastcall TCircunference::CompareR(const TCircunference *C1,
                                        const TCircunference *C2)
{
    //el puntero C1 debería apuntar a un círculo contruido
    if(C1 == NULL)
        throw EImproperArgument("pointer C1 should point to built circle");

    //el puntero C2 debería apuntar a un círculo contruido
    if(C2 == NULL)
        throw EImproperArgument("pointer C2 should point to built circle");

    if(C1->__R < C2->__R)
        return -1;
    if(C1->__R > C2->__R)
        return 1;
    return 0;
}

//MÉTODOS ESTÁTICOS DE IMPRESIÓN:

void __fastcall TCircunference::Print(AnsiString& S, const TCircunference *C)
{
    //el puntero C debería apuntar a un círculo
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    S += C->getText();
}
void __fastcall TCircunference::PrintRow(AnsiString& S, const TCircunference *C)
{
    //el puntero C debería apuntar a un círculo
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    S += C->getRowText();
}
void __fastcall TCircunference::PrintAssigns(AnsiString& S, const TCircunference *C)
{
    //el puntero C debería apuntar a un círculo
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    S += C->getAssignsText();
}

//MÉTODOS ESTÁTICOS DE LECTURA:

void __fastcall TCircunference::Read(TCircunference *C,
                                     const AnsiString& S, int& i)
{
    //el puntero C debería apuntar a un círculo construido
    if(C == NULL)
        throw EImproperArgument("pointer C shoult point to built circle");

    //si el índice i no indica a una posición de la cadena
    if(i<1 || S.Length()<i)
        //indica que no se han encontrado los valores del círculo
        throw EImproperArgument("circle values not founds");

    //estado de lectura:
    //      0: esperando separador o '('
    //      1: esperando separador o TDoublePoint P
    //      2: esperando separador o ','
    //      3: esperando separador o double R
    //      4: esperando separador o ')'
    //      5: conjunto leido con éxito
    int status = 0;

    //variables tampón en formato conveniente
    TCircunference _C;

    do {
        //reacciona según el estado
        switch(status) {
        case 0: //esperando separador o '('
            try {
            StrTravelLabel("(", S,i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 1: //esperando separador o TDoublePoint P
            try {
            TDoublePoint aux;
            StrReadDPoint(&aux, S, i);
            _C.P = aux;
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 2: //esperando separador o ','
            try {
            StrTravelLabel(",", S,i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 3: //esperando separador o double R
            try {
            double aux;
            StrReadFloat(aux, S, i);
            _C.setR(aux);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 4: //esperando separador o ')'
            try {
            StrTravelLabel(")", S,i);
        } catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientrasel conjunto no se haya leido con éxito
    } while(status < 5);

    //asigna la variable tampón
    *C = _C;
}
void __fastcall TCircunference::ReadRow(TCircunference *C,
                                        const AnsiString& S, int& i)
{
    //el puntero C debería apuntar a un círculo construido
    if(C == NULL)
        throw EImproperArgument("pointer C shoult point to built circle");

    //si el índice i no indica a una posición de la cadena
    if(i<1 || S.Length()<i)
        //indica que no se han encontrado los valores del círculo
        throw EImproperArgument("circle values not founds");

    //estado de lectura
    //      0: esperando valor para x
    //      1: esperando separador
    //      2: esperando valor para y
    //      3: esperando separador
    //      4: esperando valor para R
    //      5: conjunto leido con éxito
    int status = 0;

    //variables auxiliares
    TCircunference _C(C); //variable tampón
    AnsiString Ident; //identificador de propiedad
    AnsiString Value; //valor de propiedad

    //ADVERTENCIA: las variables tampón con propiedades interdependientes
    //deben ser clones de las variables que se pretenden modificar.

    //NOTA: adviertase que las propiedades enteras son leidas como
    //valores en punto flotante para detectar errores en los cuales
    //sea especificado un valor en punto flotante en vez de un valor entero.

    do {
        switch(status) {
        case 0: //esperando valor para x
            try {
            double aux;
            StrReadFloat(aux, S, i);
            _C.P.x = aux;
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 1: //esperando separador
            try {
            StrTravelSeparators(S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 2: //esperando valor para y
            try {
            double aux;
            StrReadFloat(aux, S, i);
            _C.P.y = aux;
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 3: //esperando separador
            try {
            StrTravelSeparators(S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 4: //esperando valor para R
            try {
            double aux;
            StrReadFloat(aux, S, i);
            _C.setR(aux);
        } catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientras el conjunto no se haya leido con éxito
    } while(status < 5);

    //asigna la variable tampón
    *C = _C;
}
void __fastcall TCircunference::ReadAssigns(TCircunference *C,
                                            const AnsiString& S, int& i)
{
}

//MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACIÓN:

//contruye un objeto
TCircunference::TCircunference(void) : TFigure(),
    __R(1)
{
    P.x = 0;
    P.y = 0;
}
TCircunference::TCircunference(double x, double y, double _R)
{
    //el radio debe ser mayor que cero
    if(_R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna el nuevo valor
    P.x = x;
    P.y = y;
    __R = _R;

}
TCircunference::TCircunference(TDoublePoint _P, double _R)
{
    //el radio debe ser mayor que cero
    if(_R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna el nuevo valor
    P = _P;
    __R = _R;

}

//copia las propiedades de un objeto
void TCircunference::Copy(TCircunference *C)
{
    //el puntero C debería apuntar a un círculo construido
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built proyection point");

    //asigna las propiedades
    P = C->P;
    __R = C->__R;
    //#Color = C->Color;
}
TCircunference& TCircunference::operator=(const TCircunference &C)
{
    //asigna las propiedades
    P = C.P;
    __R = C.__R;
    //#Color = C.Color;

    return *this;
}

//contruye un clon de un objeto
TCircunference::TCircunference(TCircunference *PP)
{
    try {
        Copy(PP);
    } catch(...) {
        throw;
    }
}

//MÉTODOS DE CONFIGURACIÓN:

//asigna conjuntamente los valores de las propiedades de definición
void TCircunference::SetValues(TDoublePoint _P, double _R)
{
    //el radio debe ser mayor que cero
    if(_R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //asigna los nuevos valores
    P = _P;
    __R = _R;
}

//MÉTODOS DE CÁLCULO DE DISTANCIAS:

//calcula la distancia mínima entre la figura y un punto
double TCircunference::DistancePoint(TDoublePoint Q)
{
    double d = Mod(Q - P);
    if(d < __R)
        return __R - d;
    else if(d > __R)
        return d - __R;
    else
        return 0;
}
//calcula la distancia máxima entre la figura y un punto
double TCircunference::DistancePointMax(TDoublePoint Q)
{
    return Mod(Q - P) + __R;
}

//calcula la distancia mínima entre la figura y una circulo
double TCircunference::DistanceCircle(TCircle *C)
{
    //el puntero C debería apuntar a un círculo contruido
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    return DistanceCircunferenceCircle(P, __R, C->P, C->getR());
}
//calcula la distancia mínima entre la figura y una circunferencia
double TCircunference::DistanceCircunference(TCircunference *C)
{
    //el puntero C debería apuntar a una circunferencia contruida
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circunference");

    return DistanceCircunferenceCircunference(P, __R, C->P, C->getR());
}

//calcula la distancia mínima entre la figura y un segmento
double TCircunference::DistanceSegment(TSegment *S)
{
    //el puntero S debería apuntar a un segmento contruido
    if(S == NULL)
        throw EImproperArgument("pointer S should point to built segment");

    return DistanceCircunferenceSegment(P, __R, S->getPa(), S->getPb());
}
//calcula la distancia mínima entre la figura y un arco
double TCircunference::DistanceArc(TArc *A)
{
    //el puntero A debería apuntar a un punto contruido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built arc");

    return DistanceCircunferenceArc(P, __R, A->getPa(), A->getPb(),
                                    A->getPc(), A->getR());
}

//calcula la distancia mínima entre la figura y otra figura
double TCircunference::Distance(TFigure *F)
{
    //el puntero F debería apuntar a una figura contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //si no, si la figura es un círculo
    if(typeid(*F) == typeid(TCircle))
        //calcula la distancia mínima al objeto
        return DistanceCircle((TCircle*)F);
    //si no, si la figura es una circunferencia
    else if(typeid(*F) == typeid(TCircunference))
        //calcula la distancia mínima al objeto
        return DistanceCircunference((TCircunference*)F);

    //si la figura es un segmento
    else if(typeid(*F) == typeid(TSegment))
        //calcula la distancia mínima al objeto
        return DistanceSegment((TSegment*)F);
    //si no, si la figura es un arco
    else if(typeid(*F) == typeid(TArc))
        //calcula la distancia mínima al objeto
        return DistanceArc((TArc*)F);
    else
        throw EImposibleError(AnsiString("unknown type: ")+AnsiString(typeid(*F).name()));
}

//MÉTODOS DE ROTACIÓN Y TRASLACIÓN:

//obtiene la figutra geométrica rotada theta radianes
//Figure debe contener una figura geométrica del mismo tipo
void TCircunference::GetRotated(TFigure *F, double theta)
{
    //el puntero F debería apuntar a una figura contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //el puntero F debería apauntar a una figura del tipo TCircunference
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
void TCircunference::Translate(TDoublePoint V)
{
    //traslada los puntos de la figura
    P.x += V.x;
    P.y += V.y;
}

//MÉTODOS GRÁFICOS:
/*#
//dibuja la circunferencia
//en un trazador de formas
void TCircunference::Paint(TPloterShapes *PS)
{
    //el puntero PS debería apuntar a un trazador de formas construido
    if(PS == NULL)
        throw EImproperArgument("pointer PS should point to built ploter shapes");

    //configura el color de la pluma
    PS->setPenColor(Color);
    //pinta un circulito en el lugar del objeto
    PS->Circunference(P, __R);
}
*/
//--------------------------------------------------------------------------
//TContourFigure
//--------------------------------------------------------------------------

//FUNCIONES ESTÁTICAS:

//imprime las propiedades de un objeto en una cadena de texto
//en formato texto entre paréntesis
void __fastcall TContourFigure::ContourFigurePrint(AnsiString& S,
                                                   TContourFigure *F)
{
    TFigure::FigurePrint(S, (TFigure*)F);
}
//lee las propiedades de un objeto en una cadena de texto
//en formato texto entre paréntesis
void __fastcall TContourFigure::ContourFigureRead(TContourFigure* &F,
                                                  const AnsiString& S, int& i)
{
    try {
        TFigure::FigureRead((TFigure*&)F, S, i);
    } catch(...) {
        throw;
    }
}

//MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACIÓN:

//inicializa la propiedad Color a su valor por defecto
//Pa y Pb quedarán sin inicializar
TContourFigure::TContourFigure(void) : TFigure()
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
void TSegment::setPa(TDoublePoint _Pa)
{
    __Pa = _Pa;
}
//coordenadas cartesianas del punto final
//valor por defecto: (0, 0)
void TSegment::setPb(TDoublePoint _Pb)
{
    __Pb = _Pb;
}

//CONUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TSegment::getText(void) const
{
    return AnsiString("(")+
            __Pa.getText()+AnsiString(", ")+
            __Pb.getText()+AnsiString(")");
}
void TSegment::setText(const AnsiString& S)
{
    try {
        int i = 1;
        Read(this, S, i);
    } catch(...) {
        throw;
    }
}
AnsiString TSegment::getRowText(void) const
{
    return __Pa.getText()+AnsiString("\t")+__Pb.getText();
}
void TSegment::setRowText(const AnsiString& S)
{
    try {
        int i = 1;
        ReadRow(this, S, i);
    } catch(...) {
        throw;
    }
}

//MÉTODOS ESTÁTICOS DE LISTA:

//imprime las propiedades de un objeto en una cadena de texto
//en formato texto entre paréntesis
void __fastcall TSegment::Print(AnsiString& S, TContourFigure *O)
{
    //el puntero O debería apuntar a un segmento contruido
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    S += O->getText();
}
//imprime las propiedades de un objeto en una cadena de texto
//en formato línea de texto
void __fastcall TSegment::PrintRow(AnsiString& S, TContourFigure *O)
{
    //el puntero O debería apuntar a un segmento contruido
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    S += O->getRowText();
}

//lee las propiedades de un objeto en una cadena de texto
//en formato texto entre paréntesis
void __fastcall TSegment::Read(TContourFigure *O_,
                               const AnsiString& S, int& i)
{
    //convierte a tipo concreto
    TSegment *O = (TSegment*)O_;

    //el puntero O debería apuntar a un segmento contruido
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    //si el índice i no indica a una posición de la cadena
    if(i<1 || S.Length()<i)
        //indica que no se han encontrado los valores del círculo
        throw EImproperArgument("segment values not founds");

    //estado de lectura:
    //      0: esperando separador o '('
    //      1: esperando separador o valor para Pa
    //      2: esperando separador o ','
    //      3: esperando separador o valor para Pb
    //      4: esperando separador o ')'
    //      5: conjunto leido con éxito
    int status = 0;

    //variable tampón en formato conveniente
    TDoublePoint _Pa, _Pb;

    do {
        //reacciona según el estado
        switch(status) {
        case 0: //esperando separador o '('
            try {
            StrTravelLabel("(", S,i);
        }catch(...) {
            throw;
        }
            status++;
            break;

        case 1: //esperando separador o valor para Pa
            try {
            StrReadDPoint(&_Pa, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 2: //esperando separador o ','
            try {
            StrTravelLabel(",", S,i);
        }catch(...) {
            throw;
        }
            status++;
            break;

        case 3: //esperando separador o TDoublePoint Pb
            try {
            StrReadDPoint(&_Pb, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 4: //esperando separador o ')'
            try {
            StrTravelLabel(")", S,i);
        }catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientras el conjunto no se haya leido con éxito
    } while(status < 5);

    //asigna la variable tampón
    try {
        O->SetValues(_Pa,_Pb);
    } catch(...) {
        throw;
    }
}
//lee las propiedades de un objeto en una cadena de texto
//en formato línea de texto
void __fastcall TSegment::ReadRow(TContourFigure *O_,
                                  const AnsiString& S, int& i)
{
    //convierte a tipo concreto
    TSegment *O = (TSegment*)O_;

    //el puntero O debería apuntar a un segmento contruido
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    //si el índice i no indica a una posición de la cadena
    if(i<1 || S.Length()<i)
        //indica que no se han encontrado los valores del círculo
        throw EImproperArgument("segment values not founds");

    //estado de lectura:
    //      0: esperando separador o valor para Pa
    //      1: esperando separador
    //      2: esperando separador o valor para Pb
    //      3: conjunto leido con éxito
    int status = 0;

    //variable tampón en formato conveniente
    TDoublePoint _Pa, _Pb;

    do {
        //reacciona según el estado
        switch(status) {
        case 0: //esperando separador o valor para Pa
            try {
            StrReadDPoint(&_Pa, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 1: //esperando separador
            try {
            StrTravelSeparators(S,i);
        }catch(...) {
            throw;
        }
            status++;
            break;

        case 2: //esperando separador o TDoublePoint Pb
            try {
            StrReadDPoint(&_Pb, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientras el conjunto no se haya leido con éxito
    } while(status < 3);

    //asigna la variable tampón
    try {
        O->SetValues(_Pa,_Pb);
    } catch(...) {
        throw;
    }
}

//MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

//contruye un objeto
TSegment::TSegment(void) : TContourFigure()
{
    __Pa.x = 0;
    __Pa.y = 0;
    __Pb.x = 0;
    __Pb.y = 0;
}
TSegment::TSegment(double ax, double ay, double bx, double by) : TContourFigure()
{
    __Pa.x = ax;
    __Pa.y = ay;
    __Pb.x = bx;
    __Pb.y = by;
}
TSegment::TSegment(TDoublePoint _Pa, TDoublePoint _Pb) : TContourFigure()
{
    __Pa = _Pa;
    __Pb = _Pb;
}

//copia las propiedades de un objeto
void TSegment::Copy(TSegment *O)
{
    //el puntero O debería apuntar a un segmento construido
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    //asigna las propiedades
    __Pa = O->__Pa;
    __Pb = O->__Pb;
    //#Color = O->Color;
}
TSegment& TSegment::operator=(const TSegment &O)
{
    //asigna las propiedades
    __Pa = O.__Pa;
    __Pb = O.__Pb;
    //#Color = O.Color;

    return *this;
}

//contruye un clon de un objeto
TSegment::TSegment(TSegment *O)
{
    try {
        Copy(O);
    } catch(...) {
        throw;
    }
}

//MÉTODOS DE CONFIGURACIÓN:

//asigna conjuntamente los valores de las propiedades de definición
void TSegment::SetValues(TDoublePoint _Pa, TDoublePoint _Pb)
{
    //asigna los nuevos valores
    __Pa = _Pa;
    __Pb = _Pb;
}

//MÉTODOS DE CÁLCULO DE DISTANCIAS:

//calcula la distancia mínima entre la figura y un punto
double TSegment::DistancePoint(TDoublePoint Q)
{
    return DistanceSegmentPoint(__Pa, __Pb, Q);
}
//calcula la distancia máxima entre la figura y un punto
double TSegment::DistancePointMax(TDoublePoint Q)
{
    return DistanceSegmentPointMax(__Pa, __Pb, Q);
}

//calcula la distancia mínima entre la figura y una circulo
double TSegment::DistanceCircle(TCircle *C)
{
    //el puntero C debería apuntar a un círculio contruido
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    return DistanceSegmentCircle(__Pa, __Pb, C->P, C->getR());
}
//calcula la distancia mínima entre la figura y una circunferencia
double TSegment::DistanceCircunference(TCircunference *C)
{
    //el puntero C debería apuntar a una circunferencia contruida
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circunference");

    return DistanceSegmentCircunference(__Pa, __Pb, C->P, C->getR());
}

//calcula la distancia mínima entre la figura y un segmento
double TSegment::DistanceSegment(TSegment *S)
{
    //el puntero S debería apuntar a un segmento contruido
    if(S == NULL)
        throw EImproperArgument("pointer S should point to built segment");

    return DistanceSegmentSegment(__Pa, __Pb, S->__Pa, S->__Pb);
}
//calcula la distancia mínima entre la figura y un arco
double TSegment::DistanceArc(TArc *A)
{
    //el puntero A debería apuntar a un punto contruido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built arc");

    return DistanceSegmentArc(__Pa, __Pb, A->getPa(), A->getPb(),
                              A->getPc(), A->getR());
}

//calcula la distancia mínima entre la figura y otra figura
double TSegment::Distance(TFigure *F)
{
    //el puntero F debería apuntar a una figura contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //si no, si la figura es un círculo
    if(typeid(*F) == typeid(TCircle))
        //calcula la distancia mínima al objeto
        return DistanceCircle((TCircle*)F);
    //si no, si la figura es una circunferencia
    else if(typeid(*F) == typeid(TCircunference))
        //calcula la distancia mínima al objeto
        return DistanceCircunference((TCircunference*)F);

    //si no, si la figura es un segmento
    else if(typeid(*F) == typeid(TSegment))
        //calcula la distancia mínima al objeto
        return DistanceSegment((TSegment*)F);
    //si no, si la figura es un arco
    else if(typeid(*F) == typeid(TArc))
        //calcula la distancia mínima al objeto
        return DistanceArc((TArc*)F);
    else
        throw EImposibleError(AnsiString("unknown type: ")+
                              AnsiString(typeid(*F).name()));
}

//MÉTODOS DE ROTACIÓN Y TRASLACIÓN

//obtiene la figutra geométrica rotada theta radianes
//Figure debe contener una figura geométrica del mismo tipo
void TSegment::GetRotated(TFigure *F, double theta)
{
    //el puntero F debería apuntar a una figura contruida
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
    S->__Pa.x = __Pa.x*COS - __Pa.y*SIN;
    S->__Pa.y = __Pa.x*SIN + __Pa.y*COS;
    S->__Pb.x = __Pb.x*COS - __Pb.y*SIN;
    S->__Pb.y = __Pb.x*SIN + __Pb.y*COS;
}
//traslada la figura geométrica según el vector V
void TSegment::Translate(TDoublePoint V)
{
    //traslada los puntos de la figura
    __Pa.x += V.x;
    __Pa.y += V.y;
    __Pb.x += V.x;
    __Pb.y += V.y;
}

//MÉTODOS GRÁFICOS:
/*#
//dibuja el segmento
//en un trazador de formas
void TSegment::Paint(TPloterShapes *PS)
{
    //el puntero PS debería apuntar a un trazador de formas construido
    if(PS == NULL)
        throw EImproperArgument("pointer PS should point to built ploter shapes");

    //configura el color de la pluma
    PS->setPenColor(Color);
    //pinta un circulito en el lugar del objeto
    PS->Segment(__Pa, __Pb);
}
*/
//--------------------------------------------------------------------------
//TArc
//--------------------------------------------------------------------------

//PROPIEDADES DE DEFINICIÓN:

void TArc::setPa(TDoublePoint _Pa)
{
    //el vértice Pa no debe coincidir con el centro Pc
    if(_Pa == __Pc)
        throw EImproperArgument("vertex Pa should not be equal center Pc");

    //el punto Pa debe estar en el entorno del arco
    if(!IsInRangeArc(_Pa))
        throw EImproperArgument("point Pa should be in range of arc");

    //asigna el nuevo valor
    __Pa = _Pa;
}
void TArc::setPb(TDoublePoint _Pb)
{
    //el vértice Pb no debe coincidir con el centro Pc
    if(_Pb == __Pc)
        throw EImproperArgument("vertex Pb should not be equal center Pc");

    //el punto Pb debe estar en el entorno del arco
    if(!IsInRangeArc(_Pb))
        throw EImproperArgument("point Pb should be in range of arc");

    //asigna el nuevo valor
    __Pb = _Pb;
}
void TArc::setPc(TDoublePoint _Pc)
{
    //el centro Pc no debe coincidir con el vértice Pa
    if(_Pc == __Pa)
        throw EImproperArgument("center Pc should not be equal vertex Pa");
    //el centro Pc no debe coincidir con el vértice Pb
    if(_Pc == __Pb)
        throw EImproperArgument("center Pc should not be equal vertex Pb");

    //el punto Pc debe estar en el entorno del centro del arco
    if(Abs(Mod(__Pa - _Pc) - __R)>ERR_NUM || Abs(Mod(__Pb - _Pc) - __R)>ERR_NUM)
        throw EImproperArgument("point Pa should be in range of a point in the arc");

    //asigna el nuevo valor
    __Pc = _Pc;
}
void TArc::setR(double _R)
{
    //el radio debe ser mayor que cero
    if(_R <= 0)
        throw EImproperArgument("radio R should be upper zero");

    //el radio R debe ser tal que Pa y Pb estén en el entorno del arco
    if(Abs(Mod(__Pa - __Pc) - _R)>ERR_NUM || Abs(Mod(__Pb - __Pc) - _R)>ERR_NUM)
        throw EImproperArgument("radio R should be such that Pa and Pb are in range arc");

    //asigna el nuevo valor
    __R = _R;
}

//PROPIEDADES DE DEFINICIÓN EN FORMATO TEXTO:

AnsiString TArc::getPaText(void) const
{
    return DPointToStr(__Pa);
}
void TArc::setPaText(const AnsiString& S)
{
    try {
        setPa(StrToDPoint(S));
    } catch(...) {
        throw;
    }
}
AnsiString TArc::getPbText(void) const
{
    return DPointToStr(__Pb);
}
void TArc::setPbText(const AnsiString& S)
{
    try {
        setPb(StrToDPoint(S));
    } catch(...) {
        throw;
    }
}
AnsiString TArc::getPcText(void) const
{
    return DPointToStr(__Pc);
}
void TArc::setPcText(const AnsiString& S)
{
    try {
        setPc(StrToDPoint(S));
    } catch(...) {
        throw;
    }
}
AnsiString TArc::getRText(void) const
{
    return FloatToStr(__R);
}
void TArc::setRText(const AnsiString& S)
{
    try {
        setR(StrToFloat(S));
    } catch(...) {
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
        int i = 1;
        Read(this, S, i);
    } catch(...) {
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
        int i = 1;
        ReadRow(this, S, i);
    } catch(...) {
        throw;
    }
}

//FUNCIONES ESTÁTICAS:

//imprime las propiedades de un objeto en una cadena de texto
//en formato texto entre paréntesis
void __fastcall TArc::Print(AnsiString& S,
                            TContourFigure *O)
{
    //el puntero O debería apuntar a un segmento contruido
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    S += O->getText();
}
//imprime las propiedades de un objeto en una cadena de texto
//en formato línea de texto
void __fastcall TArc::PrintRow(AnsiString& S, TContourFigure *O)
{
    //el puntero O debería apuntar a un segmento contruido
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    S += O->getRowText();
}

//lee las propiedades de un objeto en una cadena de texto
//en formato texto entre paréntesis
void __fastcall TArc::Read(TContourFigure *_O,
                           const AnsiString& S, int& i)
{
    //convierte a tipo concreto
    TArc *O = (TArc*)_O;

    //el puntero O debería apuntar a un segmento contruido
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    //si el índice i no indica a una posición de la cadena
    if(i<1 || S.Length()<i)
        //indica que no se han encontrado los valores del círculo
        throw EImproperArgument("arc values not founds");

    //estado de lectura:
    //      0: esperando separador o '('
    //      1: esperando separador o TDoublePoint para Pa
    //      2: esperando separador o ','
    //      3: esperando separador o TDoublePoint para Pb
    //      4: esperando separador o ','
    //      5: esperando separador o TDoublePoint para Pc
    //      6: esperando separador o ','
    //      7: esperando separador o double para R
    //      8: esperando separador o ')'
    //      9: grupo leido con éxito
    int status = 0;

    //variables tampón en formato conveniente
    TDoublePoint _Pa; //punto central
    TDoublePoint _Pb; //punto incial (en sentido dextrógiro)
    TDoublePoint _Pc; //punto final (en sentido dextrógiro)
    double _R; //radio

    do {
        //reacciona según el estado
        switch(status) {
        case 0: //esperando separador o '('
            try {
            StrTravelLabel("(", S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 1: //esperando separador o TDoublePoint Pa
            try {
            StrReadDPoint(&_Pa, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 2: //esperando separador o ','
            try {
            StrTravelLabel(",", S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 3: //esperando separador o TDoublePoint Pb
            try {
            StrReadDPoint(&_Pb, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 4: //esperando separador o ','
            try {
            StrTravelLabel(",", S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 5: //esperando separador o TDoublePoint Pc
            try {
            StrReadDPoint(&_Pc, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 6: //esperando separador o ','
            try {
            StrTravelLabel(",", S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 7: //esperando separador o TDoublePoint R
            try {
            StrReadFloat(_R, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 8: //esperando separador o ')'
            try {
            StrTravelLabel(")", S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientras no se haya leido el grupo con éxito
    } while(status < 9);

    //asigna las variables tampón
    try {
        O->SetValues(_Pa, _Pb, _Pc, _R);
    } catch(...) {
        throw;
    }
}
//lee las propiedades de un objeto en una cadena de texto
//en formato línea de texto
void __fastcall TArc::ReadRow(TContourFigure *_O,
                              const AnsiString& S, int& i)
{
    //convierte a tipo concreto
    TArc *O = (TArc*)_O;

    //el puntero O debería apuntar a un segmento contruido
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    //si el índice i no indica a una posición de la cadena
    if(i<1 || S.Length()<i)
        //indica que no se han encontrado los valores del círculo
        throw EImproperArgument("arc values not founds");

    //estado de lectura:
    //      0: esperando separador o TDoublePoint para Pa
    //      1: esperando separador
    //      2: esperando separador o TDoublePoint para Pb
    //      3: esperando separador
    //      4: esperando separador o TDoublePoint para Pc
    //      5: esperando separador
    //      6: esperando separador o double para R
    //      7: grupo leido con éxito
    int status = 0;

    //variables tampón en formato conveniente
    TDoublePoint _Pa; //punto central
    TDoublePoint _Pb; //punto incial (en sentido dextrógiro)
    TDoublePoint _Pc; //punto final (en sentido dextrógiro)
    double _R; //radio

    do {
        //reacciona según el estado
        switch(status) {
        case 0: //esperando separador o TDoublePoint Pa
            try {
            StrReadDPoint(&_Pa, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 1: //esperando separador
            try {
            StrTravelSeparators(S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 2: //esperando separador o TDoublePoint Pb
            try {
            StrReadDPoint(&_Pb, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 3: //esperando separador
            try {
            StrTravelSeparators(S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 4: //esperando separador o TDoublePoint Pc
            try {
            StrReadDPoint(&_Pc, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 5: //esperando separador
            try {
            StrTravelSeparators(S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;

        case 6: //esperando separador o TDoublePoint R
            try {
            StrReadFloat(_R, S, i);
        } catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientras no se haya leido el grupo con éxito
    } while(status < 7);

    //asigna las variables tampón
    try {
        O->SetValues(_Pa, _Pb, _Pc, _R);
    } catch(...) {
        throw;
    }
}

//MÉTODOS DE CONTRUCCIÓN,COPIA Y CLONACIÓN:

//contruye un objeto
TArc::TArc(void) : TContourFigure(),
    __R(1)
{
    __Pa.x = 1;
    __Pa.y = 0;
    __Pb.x = 1;
    __Pb.y = 0;
    __Pc.x = 0;
    __Pc.y = 0;
}
TArc::TArc(double ax, double ay, double bx, double by, double cx, double cy,
           double _R) : TContourFigure()
{
    //ADVERTENCIA: aquí no debe usarse la función IsArc, por que
    //dicha función no indica la causa de que (Pa, Pb, Pc, R)
    //no sea un arco.

    //asigna las coordenadas a puntos auxiliares
    TDoublePoint _Pa; _Pa.x = ax; _Pa.y = ay;
    TDoublePoint _Pb; _Pb.x = bx; _Pb.y = by;
    TDoublePoint _Pc; _Pc.x = cx; _Pc.y = cy;

    //el vértice Pa no debe coincidir con el centro Pc
    if(_Pa == _Pc)
        throw EImproperArgument("vertex Pa should not be equal center Pc");

    //el vértice Pb no debe coincidir con el centro Pc
    if(_Pb == _Pc)
        throw EImproperArgument("vertex Pb should not be equal center Pc");

    //el radio R debería ser no negativo
    if(_R < 0)
        throw EImproperArgument("radio R should be nonnegative");

    //el punto Pa debe estar en el entorno de un punto del arco
    if(Abs(Mod(_Pa - _Pc) - _R) > ERR_NUM)
        throw EImproperArgument("point Pa should be in range of a point in the arc");

    //el punto Pb debe estar en el entorno de un punto del arco
    if(Abs(Mod(_Pb - _Pc) - _R) > ERR_NUM)
        throw EImproperArgument("point Pb should be in range of a point in the arc");

    //asigna los nuevos valores
    __Pa = _Pa;
    __Pb = _Pb;
    __Pc = _Pc;
    __R = _R;
}
TArc::TArc(TDoublePoint _Pa, TDoublePoint _Pb, TDoublePoint _Pc,
           double _R) :
    TContourFigure()
{
    //ADVERTENCIA: aquí no debe usarse la función IsArc, por que
    //dicha función no indica la causa de que (Pa, Pb, Pc, R)
    //no sea un arco.

    //el vértice Pa no debe coincidir con el centro Pc
    if(_Pa == _Pc)
        throw EImproperArgument("vertex Pa should not be equal center Pc");

    //el vértice Pb no debe coincidir con el centro Pc
    if(_Pb == _Pc)
        throw EImproperArgument("vertex Pb should not be equal center Pc");

    //el radio R debería ser no negativo
    if(_R < 0)
        throw EImproperArgument("radio R should be nonnegative");

    //el punto Pa debe estar en el entorno de un punto del arco
    if(Abs(Mod(_Pa - _Pc) - _R) > ERR_NUM)
        throw EImproperArgument("point Pa should be in range of a point in the arc");

    //el punto Pb debe estar en el entorno de un punto del arco
    if(Abs(Mod(_Pb - _Pc) - _R) > ERR_NUM)
        throw EImproperArgument("point Pb should be in range of a point in the arc");

    //asigna los nuevos valores
    __Pa = _Pa;
    __Pb = _Pb;
    __Pc = _Pc;
    __R = _R;
}

//copia las propiedades de un objeto
void TArc::Copy(TArc *O)
{
    //el puntero O debería apuntar a un segmento construido
    if(O == NULL)
        throw EImproperArgument("pointer O should point to built segment");

    //asigna las propiedades
    __Pa = O->__Pa;
    __Pb = O->__Pb;
    __Pc = O->__Pc;
    __R = O->__R;
    //#Color = O->Color;
}
TArc& TArc::operator=(const TArc &O)
{
    //asigna las propiedades
    __Pa = O.__Pa;
    __Pb = O.__Pb;
    __Pc = O.__Pc;
    __R = O.__R;
    //#Color = O.Color;

    return *this;
}

//contruye un clon de un objeto
TArc::TArc(TArc *O)
{
    try {
        Copy(O);
    } catch(...) {
        throw;
    }
}

//MÉTODOS DE CONFIGURACIÓN:

//determina si un punto está en el entorno del arco
//      Abs(Mod(P - Pc) - R) <= ERR_NUM
bool TArc::IsInRangeArc(TDoublePoint P)
{
    //si el punto P está a una distancia del arco superior a epsilon
    if(Abs(Mod(P - __Pc) - __R) > ERR_NUM)
        return false; //indica que P no está en el entorno del arco

    return true; //indica que P si está en el entorno del arrco
}

//asigna los valores de las propiedades conjuntamente
//Lanza una excepción EImproperArgument si:
//      Pa o Pb es igual a Pc,
//      R no es mayor que cero,
//      Pa o Pb no está en el entorno del arco,
void TArc::SetValues(TDoublePoint _Pa, TDoublePoint _Pb, TDoublePoint _Pc, double _R)
{
    //ADVERTENCIA: aquí no debe usarse la función IsArc, por que
    //dicha función no indica la causa de que (Pa, Pb, Pc, R)
    //no sea un arco.

    //el vértice Pa no debe coincidir con el centro Pc
    if(_Pa == _Pc)
        throw EImproperArgument("vertex Pa should not be equal center Pc");

    //el vértice Pb no debe coincidir con el centro Pc
    if(_Pb == _Pc)
        throw EImproperArgument("vertex Pb should not be equal center Pc");

    //el radio R debería ser no negativo
    if(_R < 0)
        throw EImproperArgument("radio R should be nonnegative");

    //el punto Pa debe estar en el entorno de un punto del arco
    if(Abs(Mod(_Pa - _Pc) - _R) > ERR_NUM)
        throw EImproperArgument("point Pa should be in range of a point in the arc");

    //el punto Pb debe estar en el entorno de un punto del arco
    if(Abs(Mod(_Pb - _Pc) - _R) > ERR_NUM)
        throw EImproperArgument("point Pb should be in range of a point in the arc");

    //asigna los nuevos valores
    __Pa = _Pa;
    __Pb = _Pb;
    __Pc = _Pc;
    __R = _R;
}

//MÉTODOS DE CÁLCULO DE DISTANCIAS:

//calcula la distancia mínima entre la figura y un punto
double TArc::DistancePoint(TDoublePoint Q)
{
    return DistanceArcPoint(__Pa, __Pb, __Pc, __R, Q);
}
//calcula la distancia máxima entre la figura y un punto
double TArc::DistancePointMax(TDoublePoint Q)
{
    return DistanceArcPointMax(__Pa, __Pb, __Pc, __R, Q);
}

//calcula la distancia mínima entre la figura y una circunferencia
double TArc::DistanceCircunference(TCircunference *C)
{
    //el puntero C debería apuntar a una circunferencia contruida
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circunference");

    return DistanceArcCircunference(__Pa, __Pb, __Pc, __R, C->P, C->getR());
}
//calcula la distancia mínima entre la figura y una circulo
double TArc::DistanceCircle(TCircle *C)
{
    //el puntero C debería apuntar a un círculio contruido
    if(C == NULL)
        throw EImproperArgument("pointer C should point to built circle");

    return DistanceArcCircle(__Pa, __Pb, __Pc, __R, C->P, C->getR());
}

//calcula la distancia mínima entre la figura y un segmento
double TArc::DistanceSegment(TSegment *S)
{
    //el puntero S debería apuntar a un segmento contruido
    if(S == NULL)
        throw EImproperArgument("pointer S should point to built segment");

    return DistanceArcSegment(__Pa, __Pb, __Pc, __R, S->getPa(), S->getPb());
}
//calcula la distancia mínima entre la figura y un arco
double TArc::DistanceArc(TArc *A)
{
    //el puntero A debería apuntar a un punto contruido
    if(A == NULL)
        throw EImproperArgument("pointer A should point to built arc");

    return DistanceArcArc(__Pa, __Pb, __Pc, __R,
                          A->__Pa, A->__Pb, A->__Pc, A->__R);
}

//calcula la distancia mínima entre la figura y otra figura
double TArc::Distance(TFigure *F)
{
    //el puntero F debería apuntar a una figura contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //si la figura es un círculo
    if(typeid(*F) == typeid(TCircle))
        //calcula la distancia mínima al objeto
        return DistanceCircle((TCircle*)F);
    //si no, si la figura es una circunferencia
    else if(typeid(*F) == typeid(TCircunference))
        //calcula la distancia mínima al objeto
        return DistanceCircunference((TCircunference*)F);

    //si no, si la figura es un segmento
    else if(typeid(*F) == typeid(TSegment))
        //calcula la distancia mínima al objeto
        return DistanceSegment((TSegment*)F);
    //si no, si la figura es un arco
    else if(typeid(*F) == typeid(TArc))
        //calcula la distancia mínima al objeto
        return DistanceArc((TArc*)F);
    else
        throw EImposibleError(AnsiString("unknown type: ")+AnsiString(typeid(*F).name()));
}

//MÉTODOS DE ROTACIÓN Y TRASLACIÓN

//obtiene la figutra geométrica rotada theta radianes
//Figure debe contener una figura geométrica del mismo tipo
void TArc::GetRotated(TFigure *F, double theta)
{
    //el puntero F debería apuntar a una figura contruida
    if(F == NULL)
        throw EImproperArgument("pointer F should point to built figure");

    //el puntero F debería apauntar a una figura del tipo TArc
    if(typeid(*F) != typeid(TArc))
        throw EImproperArgument("pointer F should point to figure type TArc");

    //apunta la figura con un puntero del tipo TArc
    TArc *A = (TArc*)F;

    //calcula los coeficientes de rotación
    const double COS = cos(theta);
    const double SIN = sin(theta);

    //asigna los puntos rotados
    A->__Pa.x = __Pa.x*COS - __Pa.y*SIN;
    A->__Pa.y = __Pa.x*SIN + __Pa.y*COS;
    A->__Pb.x = __Pb.x*COS - __Pb.y*SIN;
    A->__Pb.y = __Pb.x*SIN + __Pb.y*COS;
    A->__Pc.x = __Pc.x*COS - __Pc.y*SIN;
    A->__Pc.y = __Pc.x*SIN + __Pc.y*COS;
}
//traslada la figura geométrica según el vector V
void TArc::Translate(TDoublePoint V)
{
    //traslada los puntos de la figura
    __Pa.x += V.x;
    __Pb.x += V.x;
    __Pc.x += V.x;
    __Pa.y += V.y;
    __Pb.y += V.y;
    __Pc.y += V.y;
}

//MÉTODOS GRÁFICOS:
/*#
//dibuja el arco
//en un trazador de formas
void TArc::Paint(TPloterShapes *PS)
{
    //el puntero PS debería apuntar a un trazador de formas construido
    if(PS == NULL)
        throw EImproperArgument("pointer PS should point to built ploter shapes");

    //configura el color de la pluma
    PS->setPenColor(Color);
    //pinta un circulito en el lugar del objeto
    PS->Arc(__Pa, __Pb, __Pc, __R);
}
*/
//--------------------------------------------------------------------------
//FUNCIONES RELACIONADAS:

//determina el ángulo entre el vértice inicial de una figura de contorno
//y su vértice final en torno de un punto P
double Rotation(TContourFigure *F, bool PbIsNext, TDoublePoint P)
{
    //el puntero F debería apuntar a una figura contruida
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
            if(DistanceLineToPoint(A->getPa(), A->getPb(), P)>=0 && Mod(P - A->getPc())<=A->getR())
                //devuelve el ángulo invertido
                return -theta;
            //devuelve el ángulo
            return theta;
        } else {
            //calcula el ángulo de desplazamiento de Pb a Pa
            theta = Angle(A->getPb() - P, A->getPa() - P);
            //si el punto P está en el interior del arco
            if(DistanceLineToPoint(A->getPa(), A->getPb(), P)>=0 && Mod(P - A->getPc())<=A->getR())
                //devuelve el ángulo invertido
                return -theta;
            //devuelve el ángulo
            return theta;
        }
    }

    //si no es segmento ni arco

    //indica que el puntero F debería apuntar a un segmento o un arco
    throw EImposibleError("pointer F should point to a segment or an arc");
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

