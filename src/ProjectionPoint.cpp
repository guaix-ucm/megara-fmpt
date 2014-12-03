//---------------------------------------------------------------------------
//Archivo: ProjectionPoint.cpp
//Contenido: punto de proyección
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "ProjectionPoint.h"
#include "..\2_Strings\TextFile.h"
#include "..\2_Strings\Strings.h"
#include "..\1_Mathematics\Geometry.h"
#include "..\1_Mathematics\Scalars.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//--------------------------------------------------------------------------
//TProjectionPoint
//--------------------------------------------------------------------------

//PROPIEDADES ESTÁTICAS:

//inicializa el
//color por defecto con el que son contruidos los puntos de proyección
//valor por defecto: clWhite
//#QColor TProjectionPoint::DefaultColor = Qt::white;

//PROPIEDADES ESTÁTICAS:

AnsiString TProjectionPoint::RowIdent = AnsiString("Id          ")+
        AnsiString("x                       ")+AnsiString("y");

//PROPIEDADES:

int TProjectionPoint::getId(void) const
{
    return getSkyPoint()->getId();
}
void TProjectionPoint::setId(int _Id)
{
    try {
        getSkyPoint()->setId(_Id);
    } catch(...) {
        throw;
    }
}

//PROPIEDADES EN FORMATO TEXTO:

AnsiString TProjectionPoint::getIdText(void) const
{
    return IntToStr(getId());
}
void TProjectionPoint::setIdText(const AnsiString &S)
{
    try {
        setId(StrToInt_(S));
    } catch(...) {
        throw;
    }
}

//CONJUNTOS DE PROPIEDADES DEFINITORIAS EN FORMATO TEXTO:

AnsiString TProjectionPoint::getRowText(void) const
{
    AnsiString S;

    S = getIdText();
    StrFill(S, 12, ' ');
    S += P.getxText();
    StrFill(S, 12+24, ' ');
    S += P.getyText();

    return S;
}
void TProjectionPoint::setRowText(const AnsiString &S)
{
    try {
        int i = 1;
        ReadRow(this, S, i);
        StrTravelToEnd(S, i);
    } catch(...) {
        throw;
    }
}
AnsiString TProjectionPoint::getColText(void) const
{
    AnsiString S;

    S = getIdText()+AnsiString("\r\n");
    S += P.getxText()+AnsiString("\r\n");
    S += P.getyText();

    return S;
}
void TProjectionPoint::setColText(const AnsiString &S)
{
    try {
        int i = 1;
        ReadCol(this, S, i);
        StrTravelToEnd(S, i);
    } catch(...) {
        throw;
    }
}
AnsiString TProjectionPoint::getAssignsText(void) const
{
    AnsiString S;

    S = AnsiString("Id = ")+getIdText()+AnsiString("\r\n");
    S += AnsiString("x = ")+P.getxText()+AnsiString("\r\n");
    S += AnsiString("y = ")+P.getyText();

    return S;
}
void TProjectionPoint::setAssignsText(const AnsiString &S)
{
    try {
        int i = 1;
        ReadAssigns(this, S, i);
        StrTravelToEnd(S, i);
    } catch(...) {
        throw;
    }
}

//MÉTODOS ESTÁTICOS DE LISTA:

//imprime las propiedades de un círculo en una cadena
void __fastcall TProjectionPoint::PrintRow(AnsiString &S,
                                           TProjectionPoint *PP)
{
    //el puntero PP debería apuntar a un círculo
    if(PP == NULL)
        throw EImproperArgument("pointer PP should point to built projection point");

    S += PP->getRowText();
}
void __fastcall TProjectionPoint::PrintCol(AnsiString &S,
                                           TProjectionPoint *PP)
{
    //el puntero PP debería apuntar a un círculo
    if(PP == NULL)
        throw EImproperArgument("pointer PP should point to built projection point");

    S += PP->getColText();
}
void __fastcall TProjectionPoint::PrintAssigns(AnsiString &S,
                                               TProjectionPoint *PP)
{
    //el puntero PP debería apuntar a un círculo
    if(PP == NULL)
        throw EImproperArgument("pointer PP should point to built projection point");

    S += PP->getAssignsText();
}

//lee las propiedades de un círculo en una cadena
void __fastcall TProjectionPoint::ReadRow(TProjectionPoint *PP,
                                          const AnsiString &S, int &i)
{
    //el puntero PP debería apuntar a un círculo construido
    if(PP == NULL)
        throw EImproperArgument("pointer PP shoult point to built projection point");

    //si el índice i no indica a una posición de la cadena
    if(i<1 || S.Length()<i)
        //indica que no se han encontrado los valores del círculo
        throw EImproperArgument("projection point values not founds");

    //estado de lectura
    //      0: esperando asignación a Id
    //      1: esperando asignación a x
    //      2: esperando asignación a y
    //      3: cluster leido con éxito
    int status = 0;

    //variables auxiliares
    TProjectionPoint _PP(PP); //variabletampón
    AnsiString Ident; //identificador de propiedad
    AnsiString Value; //valor de propiedad

    //ADVERTENCIA: las variables tampón con propiedades interdependientes
    //deben ser clones de las variables que se pretenden modificar.

    //NOTA: adviertase que las propiedades enteras son leidas como
    //valores en punto flotante para detectar errores en los cuales
    //sea especificado un valor en punto flotante en vez de un valor entero.

    do {
        switch(status) {
        case 0: //esperando asignación a Id
            try {
            StrReadWord(Value, S, i);
            _PP.setIdText(Value);
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property Id"));
            } catch(...) {
            throw;
        }
            if(i > S.Length())
                throw EImproperArgument("x value not found");
            status++;
            break;
        case 1: //esperando asignación a x
            try {
            StrReadWord(Value, S, i);
            _PP.P.setxText(Value);
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property x"));
            } catch(...) {
            throw;
        }
            if(i > S.Length())
                throw EImproperArgument("y value not found");
            status++;
            break;
        case 2: //esperando asignación a y
            try {
            StrReadWord(Value, S, i);
            _PP.P.setyText(Value);
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property y"));
            } catch(...) {
            throw;
        }
            status++;
            break;
        }
        //mientras no se haya leido el cluster con éxito
    } while(status < 3);

    //asigna la variable tampón
    *PP = _PP;
}
void __fastcall TProjectionPoint::ReadCol(TProjectionPoint *PP,
                                          const AnsiString &S, int &i)
{
}
void __fastcall TProjectionPoint::ReadAssigns(TProjectionPoint *PP,
                                              const AnsiString &S, int &i)
{
}

//MÉTODOS PÚBLICOS:

//contruye un punto de proyección
TProjectionPoint::TProjectionPoint(TSkyPoint *_SkyPoint) :
    TCircle()
{
    //el puntero SkyPoint debería apuntar a un punto de cielo contruido
    if(_SkyPoint == NULL)
        throw EImproperArgument("pointer SkyPoint should point to built sky point");

    //apunta los objetos externos
    __SkyPoint = _SkyPoint;

    //inicializa las propiedades
    __R = 0.75;
    Selected = false;
    //#Color = DefaultColor;
}
//clona un punto de proyección
TProjectionPoint::TProjectionPoint(TProjectionPoint *PP)
{
    try {
        Copy(PP);
    } catch(...) {
        throw;
    }
}

//MÉTODOS DE COPIA:

//copia las propiedades de un punto de proyección
void TProjectionPoint::Copy(TProjectionPoint *PP)
{
    //el puntero PP debería apuntar a un círculo construido
    if(PP == NULL)
        throw EImproperArgument("pointer PP should point to built proyection point");

    //asigna las propiedades
    __SkyPoint = PP->__SkyPoint;
    P = PP->P;
    __R = PP->__R;
    //#Color = PP->Color;
}
TProjectionPoint &TProjectionPoint::operator=(const TProjectionPoint &PP)
{
    //asigna las propiedades
    __SkyPoint = PP.__SkyPoint;
    P = PP.P;
    __R = PP.__R;
    //#Color = PP.Color;

    return *this;
}

//randomiza el punto de proyección
void TProjectionPoint::Randomize(double xmin, double xmax, double ymin, double ymax)
{
    P.x = RandomUniform(xmin, xmax);
    P.y = RandomUniform(ymin, ymax);
}

//MÉTODOS DE INTERFAZ:

//indica que parte del punto objetivo
//puede ser agarrado en el punto indicado
//      1: punto (TargetP3, 0.75)
//      0: niguna;
int TProjectionPoint::Grab(TDoublePoint Q)
{
    //si está en el punto
    if(IntersectionPointCircle(Q, P, getR()))
        return 1; //agarra el punto

    return 0;
}

//--------------------------------------------------------------------------
//TProjectionPointList
//--------------------------------------------------------------------------

//PROPIEDADES DE INTERFAZ:

AnsiString TProjectionPointList::getText(void)
{
    AnsiString S;

    //añade la cabecera
    S = TProjectionPoint::RowIdent+AnsiString("\r\n");
    for(int i=0; i<getCount(); i++)
        S += Items[i]->getRowText()+AnsiString("\r\n");

    return S;
}

void TProjectionPointList::setText(const AnsiString &S)
{
    //descompone la cadena en lineas
    TStringList *SL = new TStringList();
    SL->setText(S);

    //la lista de puntos de proyección debería contener la linea de identificadores
    if(SL->getCount() < 1)
        throw EImproperArgument("row identifiers not found");

    //la primera linea debería tener los identificadores de los campos
    //de la lista de puntos de proyección
    if(/*StrNotHasSameWords(SL->String[0], TProjectionPoint::RowIdent)*/SL->Strings[0] != TProjectionPoint::RowIdent)
        throw EImproperArgument("row identifiers not found");

    //variable tampón
    TPointersList<TProjectionPoint> LPP;
    //variables auxiliares
    TProjectionPoint *PP;

    //COMPRUEBA LA SINTAXIS DE LAS LÍNEAS DE TEXTO:

    try {
        //por cada linea de texto a partir de la segunda
        for(int i=1; i<SL->getCount(); i++) {
            //contruye un punto de proyección
            PP = new TProjectionPoint();
            //asigna la linea de texto al punto de proyección
            PP->setRowText(SL->Strings[i]);
            //añade el punto de proyección a la lista
            LPP.Add(PP);
        }
    } catch(...) {
        throw;
    }

    //ASIGNA LOS NUEVOS VALORES:

    //inicializa la lista
    Clear();
    //traslada los puntos de proyección a la lista
    for(int i=0; i<LPP.getCount(); i++)
        Add(LPP.GetPointer(i));
    //vacia la lista tampón sin destruir los objetos
    LPP.ClearWithoutDestroy();

    //libera la memoria dinámica
    delete SL;
}

//MÉTODOS PÚBLICOS:

/*
//construye un alista de puntos de proyección
TProjectionPointList::TProjectionPointList(void) :
        ProjectionPoints(1003)
{
}   */

//copia una lista de puntos de proyección
TProjectionPointList &TProjectionPointList::operator=(const TProjectionPointList &LPP)
{
    //redimensiona la lista
    if(LPP.getCount() < getCount())
        Items.DelLast(getCount() - LPP.getCount());
    else if(LPP.getCount() > getCount())
        Items.NewLast(LPP.getCount() - getCount());

    //copia los elementos
    for(int i=0; i<getCount(); i++)
        *(Items[i]) = LPP[i];

    return *this;
}

//añade puntos de proyección adscritos a
//los puntos de cielo de una lista
void TProjectionPointList::Build(TSkyPointList &SPL)
{
    for(int i=0; i<SPL.getCount(); i++)
        Add(new TProjectionPoint(SPL[i]));
}

//randomiza los puntos de proyección
void TProjectionPointList::Randomize(double xmin, double xmax, double ymin, double ymax)
{
    try {
        for(int i=0; i<getCount(); i++)
            Items[i]->Randomize(xmin, xmax, ymin, ymax);
    } catch(...) {
        throw;
    }
}

//contruye una capa de nodos de teleraña
//donde la capa 0 representa el punto central
void TProjectionPointList::BuildSpiderwebNodes(int n, double D)
{
    //nodo de la telaraña
    TProjectionPoint *node;

    //n no puede ser menor que cero
    if(n < 0)
        throw EImproperArgument("number's layer n should not be negative");

    if(n > 0) { //si tiene 6 vértices
        //distancia entre el centro del cilindro central y
        //el centro de los cilindros de los vértices de la capa n
        const double nD = n*D;
        //un sexto de vuelta en radianes
        double M_PI3 = M_PI/3;

        //calcula los vértices del exágono
        TDoublePoint V[6];
        for(int i=0; i<6; i++) {
            double aux = (i + 0.5)*M_PI3;
            V[i].x = nD*cos(aux);
            V[i].y = nD*sin(aux);
        }

        //determina los n-1 puntos intermedios entre cada par de vértices
        for(int i=1; i<6; i++) {
            for(int m=0; m<n; m++) {
                //contruye el nodo
                node = new TProjectionPoint();
                //determina el nodo
                double alfa = m/double(n);
                node->P = (1-alfa)*V[i-1] + alfa*V[i];
                //añade el nuevo nodo
                Add(node);
            }
        }
        //determina los n-1 puntos intermedios entre V[5] y V[0]
        for(int m=0; m<n; m++) {
            //contruye el nodo
            node = new TProjectionPoint();
            //determina el nodo
            double alfa = m/double(n);
            node->P = (1-alfa)*V[5] + alfa*V[0];
            //añade el nuevo nodo
            Add(node);
        }
    }
    else { //si solo hay uno en el centro
        //contruye el nodo
        node = new TProjectionPoint();
        //determina el nodo
        node->P.x = 0;
        node->P.y = 0;
        //añade el nuevo nodo
        Add(node);
    }
}
/*#
//imprime los puntos de proyección en una caja de lista
void TProjectionPointList::Print(QListWidget *LB)
{
    //el puntero LB debería apuntar a una caja de lista construida
    if(LB == NULL)
        throw EImproperArgument("pointer LB should point to built list box");

    LB->clear();
    QString QS;
    for(int i=0; i<getCount(); i++) {
        QS = Items[i]->getRowText().c_str();
        LB->addItem(QS);
    }
}
*/
/*//adscribe los posicionadores de fibra en cuyos dominios
//se encuentra cada punto de proyección
void TProjectionPointList::AttachRoboticPositioners(TRoboticPositionerList &RPF)
{
        //por cada punto de proyección de la lista
        for(int i=0; i<Count; i++)
                Items[i]->AttachRoboticPositioners(RPF);
} */

/*//borra los puntos de proyección cuyo identificador
//coincida con el indicado
void TProjectionPointList::DeleteId(int Id)
{
        //el número de identificación debería ser no negativo
        if(Id < 0)
                throw EImproperArgument("identifier number Id should be nonnegative");

        //por cada punto de proyección de la lista
        for(int i=0; i<Count; i++)
                //si el identificador del punto de proyección indicado coincide con el indicado
                if(Items[i]->Id == Id)
                        Delete(i); //borra la posición indicada
}
//borra los puntos de proyección que han sido asignados
void TProjectionPointList::DeleteAllocatedPoints(TAllocationPairList *APL)
{
        //el puntero APL debería apuntar a una lista de pares de asignación contruida
        if(APL == NULL)
                throw EImproperArgument("pointer APL should point to built allocation pair list");

        //por cada par de asignación
        for(int i=0; i<APL->Count; i++)
                //borra los puntos de proyección cuyo identificador coincida con el del par
                DeleteId(APL->Get(i).ProjectionPoint->Id);
}
void TProjectionPointList::DeleteAllocatedPoints(TAllocationListList &EP)
{
        TAllocationList *AL;

        //por cada lista de asignaciones
        for(int i=0; i<EP.Count; i++) {
                //apunta la lista de asignaciones indicada para facilitar su acceso
                AL = EP.GetPointer(i);

                //por cada punto de proyección de la lista
                for(int j=0; j<AL->ProjectionPoints.Count; j++)
                        //borra los puntos de cielo cuyo identificador coindida con el indicado
                        DeleteId(AL->ProjectionPoints[j]->Id);
        }
}        */

//MÉTODOS DE INTERFAZ:
/*#
//imprime los puntos de proyección en una caja de lista
void TProjectionPointList::Print(QTextEdit *M)
{
    //el puntero M debería apuntar a un memorando construido
    if(M == NULL)
        throw EImproperArgument("pointer M should point to built memo");

    M->clear();
    QString QS = TProjectionPoint::RowIdent.c_str();
    M->append(QS);
    for(int i=0; i<getCount(); i++) {
        QS = Items[i]->getRowText().c_str();
        M->append(QS);
    }
}

//dibuja los puntos de cielo en la fotografía de un trazador de formas
void TProjectionPointList::Paint(TPloterShapes *PS)
{
    //el puntero PS debe apuntar a un trazador de formas construido
    if(PS == NULL)
        throw EImproperArgument("pointer PS should point to built ploter shapes");

    //por cada punto de cielo de la lista
    for(int i=0; i<getCount(); i++)
        //pinta el punto de proyecciíon indicado
        Items[i]->Paint(PS);
}
*/
//guarda la lista de puntos de proyección en un archivo de texto
void TProjectionPointList::SaveToFile(const AnsiString &FileName)
{
    try {
        StrWriteToFile(FileName, getText());
    } catch(...) {
        throw;
    }
}
//carga los puntos de proyección de un archivo
void TProjectionPointList::LoadFromFile(const AnsiString &FileName)
{
    try {
        AnsiString S;
        StrReadFromFile(S, FileName);
        setText(S);
    } catch(...) {
        throw;
    }
}

//selecciona todos los puntos objetivo de la lista
void TProjectionPointList::SelectAll(void)
{
    for(int i=0; i<getCount(); i++)
        Items[i]->Selected = true;
}
//deselecciona todos los puntos objetivo de la lista
void TProjectionPointList::DeselectAll(void)
{
    for(int i=0; i<getCount(); i++)
        Items[i]->Selected = false;
}

//busca el primer punto de proyección que está bajo el punto P
//y que parte del punto objetivo puede ser agarrado en
//el punto indicado:
//      i: índice al punto de proyección bajo el punto P;
//      dominio i: [0, Count];
//      n: parte del punto de proyección agarrada;
// valores posibles:
//    0: niguna;
//    1: punto objetivo.
//valores de retorno:
//      false: ninguna parte agarrada
//      true: alguna parte agarrada
bool TProjectionPointList::Grab(int &i, int &n, TDoublePoint P)
{
    for(i=0; i<getCount(); i++) {
        n = Items[i]->Grab(P);
        if(n > 0)
            return true;
    }

    return false;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

