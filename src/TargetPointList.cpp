//---------------------------------------------------------------------------
//Archivo: TargetPointList.cpp
//Contenido: lista de puntos objetivo
//Última actualización: 26/09/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "TargetPointList.h"
#include "Strings.h"

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres del exposicionamiento
namespace Positioning {

//---------------------------------------------------------------------------

AnsiString TTargetPointList::getTargetPointsText(void)
{
        AnsiString S;

        AnsiString aux;
        TTargetPoint *TP;

        //para cada asignación de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el objeto indicado para facilitar su acceso
                TP = Items[i];

                //imprime el identificador
                aux = IntToStr(TP->getRoboticPositioner()->getActuator()->getId());
                while(aux.Length() < 8)
                        aux += char(32);

                //imprime x
                aux += FloatToStr(TP->TargetP3.x);
                while(aux.Length() < 32)
                        aux += char(32);

                //imprime y
                aux += FloatToStr(TP->TargetP3.y);
                aux += "\r\n";

                S += aux;
        }

        return S;
}
void TTargetPointList::setTargetPointsText(AnsiString &S)
{
    try {
        //indica a la primera posición de la cadena
        int i = 1;
        //atraviesa las etiquetas
        TTargetPoint::TravelLabels(S, i);
        //lee la tabla en una variable tampón
        //TPointersList<TTargetPoint> TPL;
        //TPL.Read = TTargetPoint::ReadRow;
        //TPointersList<TTargetPoint>::ReadSeparated(&TPL, S, i);

        //No puede invocar a TPointersList<TTargetPoint>::ReadSeparated,
        //porque TTargetPoint no tiene un constructor por defecto, de modo que
        //se hará a pelo.

        //-----------------------------------------------------------------------------

        //el índice i debería indicar a alguna posición de la cadena de texto S
        if(i<0 || S.Length()+1<i)
            throw EImproperArgument("index i should indicate a posicion in the string text S");

        //estado de la máquina de estados
        //      0: esperando próximo elemento o fin de cadena
        //      1: fin de cadena encontrada y lista de elementos leida con éxito
        int status = 0;

        int Id;
        double x, y;
        TTargetPoint *Item;
        TSlideArray<TTargetPoint*> Items;

        //avanza el índice hasta el próximo caracter no separador
        StrTravelSeparatorsIfAny(S, i);

        do { //repite
            //si no ha llegado al final de la cadena
            if(i <= S.Length())
                try {
                    //lee los valores de las propiedades próximo elemento
                    TTargetPoint::ReadSeparated(Id, x, y, S, i);
                    //busca el RP en la RPL de la TPL
                    int j = getRoboticPositionerList()->SearchId(Id);
                    //si no ha encontrado el RP
                    if(j >= getRoboticPositionerList()->getCount())
                        //indica que no ha encontrado el RP indicado
                        throw EImproperFileLoadedValue(AnsiString("RP ")+IntToStr(Id)+AnsiString("not found"));
                    //apunta el RP encontrado
                    TRoboticPositioner *RP = getRoboticPositionerList()->Get(j);
                    //contruye el elemento adscrito al RP encontrado
                    Item = new TTargetPoint(RP, x, y);
                    //añade el elemento
                    Items.AddLast(Item);
                    //avanza el índice hasta el próximo caracter no separador
                    StrTravelSeparators(S, i);
                    //si el índice indica a la posúltima posición de la cadena
                    if(i > S.Length())
                        status++; //indica que se ha leido la lista con éxito

                    //ADVERTENCIA: el elemento tiene que ser añadido a la lista tampón
                    //tan pronto como es contruido, para que pueda ser destruido
                    //en caso de error

                } catch(...) {
                    throw;
                }

            else //si ha llegado al final de la cadena
                //indica que ha llegado al final de la cadena
                status++;

        //mientras no se haya acabado la cadena y no se haya leido la lista con éxito
        } while(status < 1);

        //asigna la variable tampón:

        //reinicia la lista de puntos objetivo
        //copia las variables tampón
        Delete_();
        for(int j=0; j<Items.getCount(); j++)
            Add(Items[j]);
        Items.DelLast(Items.getCount());

    } catch(...) {
        //destruye los elementos tampón
        for(int i=0; i<Items.getCount(); i++)
            delete Items[i];
        throw;
    }

/*        TVector<int> Ids; //identificadores
        TVector<double> xs; //abcisas
        TVector<double> ys; //ordenadas

        TStringList *Lines = new TStringList;

        //divide la cadena en líneas
        Lines->setText(TargetPointsText.c_str());

        //-------------------------------------------------------------------
        //intenta traducir la lista a formato numérico

        //la cadena debería contener al menos una linea
        if(Lines->getCount() < 1)
                throw EImproperFileLoadedValue("the string should have almos one line");

        //La primera línea debe contener las palabras de la cabecera:
        //      {"Id", "x", "y"}

        //divide la primera linea en palabras
        TStringList *Words = new TStringList;
        AnsiString S = Lines->Strings[0];
        StrDivideInWords(Words, S);
        //el número de palabras de la primera línea debería ser tres
        if(Words->getCount() != 3)
                throw EImproperFileLoadedValue("number words in the first line should be three");
        //las palabras de la primera línea deben ser: "Id", "x" e "y"
        if(Words->Strings[0]!="Id" || Words->Strings[1]!="x" || Words->Strings[2]!="y")
                throw EImproperFileLoadedValue("the words in the firs line should be: 'Id', 'x' and 'y'");

        int i;

        //para cada fila de la tabla
        for(i=1; i<Lines->getCount(); i++) {
                //descompone la fila en palabras
                Words->Clear();
                S = Lines->Strings[i];
                StrDivideInWords(Words, S);

                //el número de palabras de cada fila debería ser 0 ó 3
                if(Words->getCount()!=0 && Words->getCount()!=3)
                        throw EImproperFileLoadedValue(AnsiString("number words in the file shuld be 0 or 3: ")+IntToStr(i));

                //traduce las palabras a formato numérico
                if(Words->getCount() == 3) {
                        Ids.Add(StrToInt(Words->Strings[0]));
                        xs.Add(StrToFloat(Words->Strings[1]));
                        ys.Add(StrToFloat(Words->Strings[2]));
                }
        }

        //-------------------------------------------------------------------
        //determina la validez de la lista de puntos objetivo

        //el número de puntos objetivo no debería ser
        //mayor que el número de posicionadores
        if(Ids.getCount() > getRoboticPositionerList()->getCount())
               throw EImproperFileLoadedValue("target points number should not be upper RPs number");

        //cada identificador debe ser mayor que cero y aparecer una sola vez
        int Id;
        for(i=0; i<Ids.getCount(); i++) {
                //asigna el identificador para facilitar su acceso
                Id = Ids[i];

                //el identificador debería ser mayor que cero
                if(Id < 1)
                        throw EImproperFileLoadedValue(AnsiString("identifier Id should be upper zero: ")+IntToStr(Id));

                //el identificador debería aparecer una sola vez
                for(int j=i+1; j<Ids.getCount(); j++)
                        if(Ids[j] == Id)
                                throw EImproperFileLoadedValue(AnsiString("identifier Id should appear once: ")+IntToStr(Id));
        }

        //-------------------------------------------------------------------
        //intenta añadir los puntos finales a la lista

        //reinicia la lista de puntos objetivo
        Delete_();

        TRoboticPositioner *FP;
        int j;

        //para cada identificador de la lista
        for(i=0; i<Ids.getCount(); i++) {
                //busca el posicionador
                j = getRoboticPositionerList()->SearchId(Ids[i]);

                //si el posicionador no está en la lista
                if(j >= getRoboticPositionerList()->getCount())
                        //indica que el posicionador identificado Id no está en la lista RoboticPositionerList
                        throw EImproperFileLoadedValue("identified positionr Id isn't in the list RoboticPositionerList");

                //añade un nuevo punto objetivo
                Add(new TTargetPoint(getRoboticPositionerList()->Get(j), xs[i], ys[i]));
        }

        //-------------------------------------------------------------------

        delete Words;
        delete Lines;*/
}

//---------------------------------------------------------------------------
//CONSTRUCCIÓN Y DESTRUCCIÓN:
//---------------------------------------------------------------------------

//construye una lista de puntos objetivo
//adscrita a una lista de RPs
TTargetPointList::TTargetPointList(TRoboticPositionerList *_RoboticPositionerList) :
//        TargetPoints(1000, TTargetPoint::CompareIds)
        TItemsList<TTargetPoint*>(1000, TTargetPoint::CompareIds)
{
        //el puntero RoboticPositionerList debería apuntar a una lista de RPs contruida
        if(_RoboticPositionerList == NULL)
                throw EImproperArgument("pointer RoboticPositionerList should point to built RP list");

        //apunta el objeto externo
        __RoboticPositionerList = _RoboticPositionerList;
}

//borra los puntos objetivo y destruye la lista
TTargetPointList::~TTargetPointList()
{
        Delete_();
}

//---------------------------------------------------------------------------
//BÚSQUEDA:
//---------------------------------------------------------------------------

//busca el punto objetivo adscrito a un posicionador
int TTargetPointList::SearchTargetPoint(TRoboticPositioner *RP)
{
        //el puntero RP debe apuntar a un RP construido
        if(RP == NULL)
                throw EImproperArgument("pointer RP should point to built RP");

        int i = 0;
        while(i<getCount() && Items[i]->getRoboticPositioner()!=RP)
                i++;

        return i;
}
//busca el punto objetivo adscrito a un posicionador identificado
int TTargetPointList::SearchTargetPoint(int Id)
{
        int i = 0;
        while(i<getCount() && Items[i]->getRoboticPositioner()->getActuator()->getId()!=Id)
                i++;

        return i;
}

//---------------------------------------------------------------------------
//OBJETOS INDICADOS:
//---------------------------------------------------------------------------

//añade un punto objetivo para el posicionador indicado de la lista
void TTargetPointList::AddTargetPoint(int i)
{
        //el índice i debería indicar un posicionador de la lista RoboticPositionerList
        if(i<0 || getRoboticPositionerList()->getCount()<=i)
                throw EImproperArgument("index i should indicate a position in the list ListFibeRPositioners");

        //apunta el posicionador indicado para facilitar su acceso
        TRoboticPositioner *RP = getRoboticPositionerList()->Get(i);

        //busca el punto objetivo adscrito al posicionador
        i = SearchTargetPoint(RP);

        //si el posicionador indicado i ya tiene un ponto objetivo adscrito
        if(i > getRoboticPositionerList()->getCount())
                //indica que el posicionador indicado i ya tiene adscrito un punto objetivo
                throw EImproperArgument("indexed positioner i already have assigned a target point");

        //añade el punto objetivo para el posiciondor indicado
        //con ls coordenadas de su punto P3
        Add(new TTargetPoint(RP, RP->getActuator()->getArm()->getP3().x, RP->getActuator()->getArm()->getP3().y));
}
//borra el punto objetivo indicada de la lista
void TTargetPointList::DeleteTargetPoint(int i)
{
        //el índice i debería indicar un punto objetivo de la lista
        if(i<0 || getCount()<=i)
                throw EImproperArgument("index i should indicate a target point in the list");

        //destruye el punto objetivo
        delete Items[i];
        //borra la posición de la lista
        Delete(i);
}

//---------------------------------------------------------------------------
//OBJETOS SELECCIONADOS DE LA LISTA:
//---------------------------------------------------------------------------

//añade puntos objetivo con la posición del punto P3
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
int TTargetPointList::AddP3Selected(void)
{
        TRoboticPositioner *RP;
        int j;
        int acum = 0;

        //para cada posicionador de la lista de posicionadores
        for(int i=0; i<getRoboticPositionerList()->getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = getRoboticPositionerList()->Get(i);

                //si el posicionador está seleccionado
                if(RP->getActuator()->Selected) {
                        //busca el posicionador en la lista de puntos objetivo
                        j = SearchTargetPoint(RP->getActuator()->getId());
                        //si el posicionador ya tiene un punto objetivo asignado
                        if(j < getCount())
                                //asignala posición del posicionador al punto objetivo
                                Items[j]->TargetP3 = RP->getActuator()->getArm()->getP3();
                        else //si el posicionador no tiene un punto objetivo asignado
                                //añade un punto objetivo con sus coordenadas actuales
                                Add(new TTargetPoint(RP, RP->getActuator()->getArm()->getP3()));
                        //contabiliza el posicionador seleccionado
                        acum++;
                }
        }

        return acum;
}

//destruye los puntos objetivo
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
int TTargetPointList::DeleteSelected(void)
{
        int i = 0;
        TTargetPoint *TP;
        int acum = 0;

        //por cada punto objetivo de la lista
        while(i < getCount()) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];
                //si el posicionador adscrito está seleccionado
                if(TP->getRoboticPositioner()->getActuator()->Selected) {
                        //destruye el punto objetivo
                        DeleteTargetPoint(i);
                        //contabilizael posicionador adscrito seleccionado
                        acum++;
                } else  //si elposicionador adscrito no está seleccionado
                        i++; //indica al próximo punto objetivo
        }

        return acum;
}

//asigna el punto P3o a los puntos objetivo
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
int TTargetPointList::SetP3oSelected(void)
{
        TTargetPoint *TP;
        int acum = 0;

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];
                //si el posicionador adscrito está seleccionado
                if(TP->getRoboticPositioner()->getActuator()->Selected) {
                        //asigna el P0 del posicionador adscrito al punto objetivo
                        TP->SetP3o();
                        //contabiliza el posicionador seleccionado
                        acum++;
                }
        }

        return acum;
}

//asigna el punto P3 a los puntos objetivo
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
int TTargetPointList::SetP3Selected(void)
{
        TTargetPoint *TP;
        int acum = 0;

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo para facilitar su acceso
                TP = Items[i];
                //si el posicionador adscrito está seleccionado
                if(TP->getRoboticPositioner()->getActuator()->Selected) {
                        //asigna la posición actual del posicioandor
                        TP->SetP3();
                        //contabiliza el posicionador seleccionado
                        acum++;
                }
        }

        return acum;
}

//randomiza los puntos objetivo con distribución uniforme
//en el dominio del punto P3 de sus posicionadores adscrito
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
int TTargetPointList::RandomizeSelected(void)
{
        TTargetPoint *TP;
        int acum = 0;

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];
                //si elposicionador adscrito está seleccionado
                if(TP->getRoboticPositioner()->getActuator()->Selected) {
                        //randomiza el punto en el dominio de su RP adscrito
                        Items[i]->Randomize();
                        //contabiliza el posicionador seleccionado
                        acum++;
                }
        }

        return acum;
}

//randomiza los puntos objetivo con distribución uniforme
//en el dominio del punto P3 de sus posicionadores adscritos
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
int TTargetPointList::RandomizeWithoutCollisionSelected(void)
{
        TTargetPoint *TP;
        TRoboticPositioner *RP;
        int acum = 0;

        //vectores para guardar la configuración inicial
        TVector<char> Quantify_;
        TVector<char> Quantify__;
        TVector<double> theta_1;
        TVector<double> theta__3;

        //bandera de colisión detectada
        bool collision;

        //levanta las baderas de colisión de todos los posicionadores
        getRoboticPositionerList()->EnablePending();

        //ADVERTENCIA: no basta con levantar las banderas de colisión de
        //los posicionadores adscritos  los puntos objetivo de la lista
        //porque la colisión puede darse con posicionadores que no tengan
        //punto objetivo adscrito.

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];
                //apunta el posicionador adscrito para facilitar su acceso
                RP = TP->getRoboticPositioner();

                //sielposicionador adscrito está seleccionado
                if(RP->getActuator()->Selected) {
                        //guarda la configuración inicial del posicionador
                        Quantify_.Add(char(RP->getActuator()->getQuantify_()));
                        Quantify__.Add(char(RP->getActuator()->getArm()->getQuantify___()));
                        theta_1.Add(RP->getActuator()->gettheta_1());
                        theta__3.Add(RP->getActuator()->getArm()->gettheta___3());

                        //desactiva la cuantificación de los ejes
                        RP->getActuator()->setQuantify_(false);
                        RP->getActuator()->getArm()->setQuantify___(false);

                        //repite
                        do {
                                //randomiza el punto en el dominio de su RP adscrito
                                TP->Randomize();
                                //mueve el posicionador al punto objetivo
                                TP->MoveToTargetP3();
                                //determina is hay colisión
                                collision = RP->getActuator()->ThereIsCollisionWithAdjacent();
                        //mientras el brazo del posicionador invada a sus adyacentes
                        } while(collision);

                        //contabiliza el posicionador adscrito seleccionado
                        acum++;
                }
        }

        //restaura las posiciones de los ejes de los posicionadores:

        //si hay posicionadores adscritosseleccionados
        if(acum > 0) {
                int j = 0; //índice a registros de configuración

                //por cada punto objetivo de la lista
                for(int i=0; i<getCount(); i++) {
                        //apunta el posicionador para faciltiar su acceso
                        RP = Items[i]->getRoboticPositioner();

                        //si el posicionador está seleccionado
                        if(RP->getActuator()->Selected) {
                                //restaura la posición de los ejes
                                RP->getActuator()->getArm()->settheta___3(theta__3[j]);
                                RP->getActuator()->settheta_1(theta_1[j]);
                                //restaura la cuantificación de los ejes
                                RP->getActuator()->getArm()->setQuantify___(bool(Quantify__[j]));
                                RP->getActuator()->setQuantify_(bool(Quantify_[j]));

                                //indica al prósimo registro de configuración
                                j++;
                        }
                }
        }

        return acum;
}

//asigna los puntos objetivo
//al punto P3 de sus posicionadores adscritos
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
//si algún punto objetivo no está en el dominio de
//su posicionador adscrito lanza EImproperCall
int TTargetPointList::MoveToTargetP3Selected(void)
{
        TTargetPoint *TP;
        int acum = 0;

        //para cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];
                //si el posicionador adscrito está seleccionado
                if(TP->getRoboticPositioner()->getActuator()->Selected) {
                        //asigna el punto objetivo a su RP adscrito
                        TP->MoveToTargetP3();
                        //contabiliza el posicionador seleccionado
                        acum++;
                }
        }

        return acum;
}

//---------------------------------------------------------------------------
//TODOS LOS OBJETOS DE LA LISTA:
//---------------------------------------------------------------------------

//añade puntos objetivo con la posición del punto P3
//de todos los posicionadores de la lista
void TTargetPointList::AddP3(void)
{
        TRoboticPositioner *RP;
        int j;

        //paracadaposicionadordelalistadeposicionadores
        for(int i=0;i<getRoboticPositionerList()->getCount(); i++){
                //apunta el posicionador indicado para facilitar su acceso
                RP = getRoboticPositionerList()->Get(i);

                //busca el posicionador en la lista de puntos objetivo
                j=SearchTargetPoint(RP->getActuator()->getId());
                //si el posicionador ya tiene un punto objetivo asignado
                if(j < getCount())
                        //asigna la posición del posicionador al punto objetivo
                        Items[j]->TargetP3 = RP->getActuator()->getArm()->getP3();
                else//si el posicionador no tiene un punto objetivo asignado
                        //añade un punto objetivo con sus coordenadas actuales
                        Add(new TTargetPoint(RP, RP->getActuator()->getArm()->getP3()));
        }
}

//destruye los puntos objetivo
//de los posicionadores seleccionados de la lista
//e indica el número de posicionadores seleccionados
void TTargetPointList::Delete_(void)
{
        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++)
                //destruye el punto objetivo
                delete Items[i];

        //borra las posiciones de la lista
        Clear();
}

//asigna el punto PO3 a los puntos objetivo
//de todos los posicionadores de la lista
void TTargetPointList::SetPO3(void)
{
        //todos los posicionadores adscritos deben estar en la lista
        TVector<int> indices;
        SearchMissingRoboticPositioners(indices);
        if(indices.getCount() > 0)
                throw EImproperCall("there are missing RPs in the target point assignations list");

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++)
                //asigna el P3o del posicionador adscrito al punto objetivo
                Items[i]->SetP3o();
}

//asigna el punto P3 a los puntos objetivo
//de todos los posicionadores de la lista
void TTargetPointList::SetP3(void)
{
        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++)
                //asigna la posición actual del posicioandor adscrito
                Items[i]->SetP3();
}

//randomiza los puntos objetivo con distribución uniforme
//en el dominio del punto P3 de sus posicionadores adscrito
//de todos los posicionadores de la lista
void TTargetPointList::Randomize(void)
{
        //todos los posicionadores de fibra adscritos a
        //los puntos objetivo deben estar en la lista
        TVector<int> indices;
        SearchMissingRoboticPositioners(indices);
        if(indices.getCount() > 0)
                throw EImproperCall("there are missing RPs in the assignaments list");

        //por cada asignación de la lista
        for(int i=0; i<getCount(); i++)
                //randomiza el punto en el dominio de su RP adscrito
                Items[i]->Randomize();
}

//randomiza los puntos objetivo con distribución uniforme
//en el dominio del punto P3 de sus posicionadores adscritos
//de todos los posicionadores de la lista
void TTargetPointList::RandomizeWithoutCollision(void)
{
        //todos los posicionadores de fibra adscritos a
        //los puntos objetivo deben estar en la lista
        TVector<int> indices;
        SearchMissingRoboticPositioners(indices);
        if(indices.getCount() > 0)
                throw EImproperCall("there are missing RPs in the assignaments list");

        TTargetPoint *TP;
        TRoboticPositioner *RP;

        //vectores para guardar la configuración inicial
        TVector<char> Quantify_;
        TVector<char> Quantify__;
        TVector<double> theta_1;
        TVector<double> theta__3;

        //bandera de colisión detectada
        bool collision;

        //levanta las baderas de colisión de todos los posicionadores
        getRoboticPositionerList()->EnablePending();

        //ADVERTENCIA: no basta con levantar las banderas de colisión de
        //los posicionadores adscritos  los puntos objetivo de la lista
        //porque la colisión puede darse con posicionadores que no tengan
        //punto objetivo adscrito.

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];
                //apunta el posicionador adscrito para facilitar su acceso
                RP = TP->getRoboticPositioner();

                //guarda la configuración inicial del posicionador
                Quantify_.Add(char(RP->getActuator()->getQuantify_()));
                Quantify__.Add(char(RP->getActuator()->getArm()->getQuantify___()));
                theta_1.Add(RP->getActuator()->gettheta_1());
                theta__3.Add(RP->getActuator()->getArm()->gettheta___3());

                //desactiva la cuantificación de los ejes
                RP->getActuator()->setQuantify_(false);
                RP->getActuator()->getArm()->setQuantify___(false);

                //repite
                do {
                        //randomiza el punto en el dominio de su RP adscrito
                        TP->Randomize();
                        //mueve el posicionador al punto objetivo
                        TP->MoveToTargetP3();
                        //determina si hay colisión
                        collision = RP->getActuator()->ThereIsCollisionWithAdjacent();
                //mientras el brazo del posicionador invada a sus adyacentes
                } while(collision);
        }

        //restaura las posiciones de los ejes de los posicionadores:

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para faciltiar su acceso
                RP = Items[i]->getRoboticPositioner();

                //restaura la posición de los ejes
                RP->getActuator()->getArm()->settheta___3(theta__3[i]);
                RP->getActuator()->settheta_1(theta_1[i]);

                //restaura la cuantificación de los ejes
                RP->getActuator()->getArm()->setQuantify___(bool(Quantify__[i]));
                RP->getActuator()->setQuantify_(bool(Quantify_[i]));
        }
}

//asigna los puntos objetivo
//al punto P3 de sus posicionadores adscritos
//si algún punto objetivo no está en el dominio de
//su posicionador adscrito lanza EImproperCall
//de todos los posicionadores de la lista
void TTargetPointList::MoveToTargetP3(void)
{
        //todos los puntos objetivo deben estar en
        //el dominio de su posicionador adscrito
        TVector<int> indices;
        SearchOutDomineTargetPoints(indices);
        if(indices.getCount() > 0)
                throw EImproperCall(AnsiString("there are target points out of domain his atached RP ")+indices.getText());

        //para cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++)
                //asigna el punto objetivo a su RP adscrito
                Items[i]->MoveToTargetP3();
}

//---------------------------------------------------------------------------
//VALIDACIÓN:
//---------------------------------------------------------------------------

//busca los puntos objetivo adscritos a posicionadores repetidos
void TTargetPointList::SearchRepeatedRoboticPositioners(TVector<int> &indices)
{
        //inicializa la ista de índices
        indices.Clear();

        TTargetPoint *TP;
        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++)
                //por cada punto objetivo posterior de la lista
                for(int j=i+1; j<getCount(); j++)
                        //si el posicionador adscrito es el mismo
                        if(Items[i]->getRoboticPositioner() == Items[j]->getRoboticPositioner())
                                indices.Add(j); //añade el índice al punto objetivo
}

//busca los puntos objetivo adscritos a posicionadores ausentes
//en la lista de posicionadores RoboticPositionerList
void TTargetPointList::SearchMissingRoboticPositioners(TVector<int> &indices)
{
        TTargetPoint *TP;
        int j;

        //reinicial el vector de índices en congruencia
        //con la situación de partida
        indices.Clear();

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];

                //busca el posicionador adscrito en la lista de posicionadores
                j = getRoboticPositionerList()->Search(TP->getRoboticPositioner());

                //si el posicionador no está en la lista de posicionadores
                if(j >= getRoboticPositionerList()->getCount())
                        //añade el índice a la lista de omitidos
                        indices.Add(i);
        }
}

//busca los puntos objetivo que están fuera del dominio
//de sus posicionadores adscritos
void TTargetPointList::SearchOutDomineTargetPoints(TVector<int> &indices)
{
        TTargetPoint *TP;
        TRoboticPositioner *RP;

        //reinicial el vector de índices en congruencia
        //con la situación de partida
        indices.Clear();

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];

                //apunta el posicionador adscrito para facilitar su acceso
                RP = TP->getRoboticPositioner();

                //traduce el punto a coordenadas angulares de los ejes
                //y determina si está en el dominio del posicionador
                double theta_1, theta___3;
                bool isindomine = RP->getActuator()->AnglesToGoP3(theta_1, theta___3, TP->TargetP3.x, TP->TargetP3.y);

                //si el punto objetivo está fuera del dominio del posicionador
                if(!isindomine)
                        //añade el índice a la lista
                        indices.Add(i);
        }
}

//Determina la invalidez de una lista de puntos objetivo
//para ser programada.
//Valores de retorno:
//      0: lista de puntos objetivo válida;
//      1: posicionadores adscritos de los puntos objetivo indicados
// ausentes;
//      2: puntos objetivo indicados fuera del dominio de
//         sus posicionadores adscritos.
int TTargetPointList::Invalid(TVector<int> &indices)
{
        //se supone que ninguno de los posicionadores adscritos
        //está repetido, y que la congruencia de repetición
        //se logra controlando las adiciones

        //busca posicionadores adscritos ausentes
        SearchMissingRoboticPositioners(indices);
        if(indices.getCount() > 0) //si ha encontrado algún posicionador ausente
                return 1; //indica que ha encontrado posicionadores ausentes

        //busca punto objetivo fuera del dominio de su posicionadores adscritos
        SearchOutDomineTargetPoints(indices);
        if(indices.getCount() > 0) //si ha encontrado algún punto fuera del dominio
                return 2; //indica que ha encontrado puntos fuera del dominio

        return 0; //indica que el punto objetivo de puntos finales es válida
}

//---------------------------------------------------------------------------
//SEGREGACIÓN:
//---------------------------------------------------------------------------

//segrega los posicionadores de los puntos objetivo en dos listas:
//      internos: con el brazo dentro del área de seguridad;
//      externos: con el brazo fuera del área de seguridad;
//si algún punto objetivo está fuera del dominio de
//su posicionador adscrito:
//      lanza EImproperCall
void TTargetPointList::SegregateInOut(TRoboticPositionerList &Inners,
        TRoboticPositionerList &Outsiders)
{
        //COMPRUEBA QUE TODOS LOS PUNTOS OBJETIVO ESTÁN EN
        //EL DOMINIO DE SUS POSICIONADORES ADSCRITOS:

    //busca los puntos objetivo que están fuera del dominio
        //de sus posicionadores adscritos
        TVector<int> indices;
        SearchOutDomineTargetPoints(indices);
    //Si ha encontrado algún punto objetivo
        if(indices.getCount() > 0) //si ha encontrado algún punto fuera del dominio
                throw EImproperCall(AnsiString("there are target points out of domain his atached RP ")+indices.getText());

        //CALCULA LOS ÁNGULOS PARA POSICIONAR CADA FIBRA EN SU PUNTO OBJETIVO
        //Y CLASIFICA EL PUNTO OBJETIVO EN FUNCIÓN DE LA EXTENSIÓN DEL BRAZO:

        TTargetPoint *TP;
        TRoboticPositioner *RP;
        double theta_1, theta___3;
        bool isindomain;

        //reinicializa las listas en congruencia con su estado inicial
        Inners.Clear();
        Outsiders.Clear();

        //para cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];

                //apunta el posicionador adscrito
                //para facilitar su acceso
                RP = TP->getRoboticPositioner();

                //traduce el punto a coordenadas angulares de los ejes
                //y determina si el punto objetivo está en el dominio del posicionador
                isindomain = RP->getActuator()->AnglesToGoP3(theta_1, theta___3, TP->TargetP3.x, TP->TargetP3.y);

                //el punto objetivo no puede estar fuera del dominio de su posicionador adscrito
                if(!isindomain)
                        throw EImposibleError("lateral effects");

                //si el punto objetivo está en el área de seguridad
                if(RP->getActuator()->theta___3IsInSafeArea(theta___3))
                        //apunta el controlador adscrito en la lista de internos
                        Inners.Add(RP);
                else //si está fuera del área de seguridad
                        //apunta el posicionador adscrito en la lista de externos
                        Outsiders.Add(RP);
        }
}

//---------------------------------------------------------------------------
//ALMACENAMIENTO Y RECUPERACIÓN DE POSICIONES:
//---------------------------------------------------------------------------

//apila las posiciones de los posicionadores adscritos
void TTargetPointList::PushPositions(void)
{
        TRoboticPositioner *RP;

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador adscrito inicado para facilitar su acceso
                RP = Items[i]->getRoboticPositioner();

                //apila la posición de sus ejes
                RP->getActuator()->Pushtheta_1();
                RP->getActuator()->getArm()->Pushtheta___3();
        }
}
//restaura las posiciones de los posicionadores adscritos
void TTargetPointList::RestorePositions(void)
{
        TRoboticPositioner *RP;

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador adscrito indicado para facilitar su acceso
                RP = Items[i]->getRoboticPositioner();

                //restaura y desempila la posición de sus ejes
                RP->getActuator()->getArm()->Restoretheta___3();
                RP->getActuator()->Restoretheta_1();
        }
}
//desempila las posiciones de los posicionadores adscritos
void TTargetPointList::PopPositions(void)
{
        TRoboticPositioner *RP;

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador adscrito indicado para facilitar su acceso
                RP = Items[i]->getRoboticPositioner();

                //desempila la posición de sus ejes
                RP->getActuator()->getArm()->Poptheta___3();
                RP->getActuator()->Poptheta_1();
        }
}
//restaura y desempila las posiciones de los posicionadores adscritos
void TTargetPointList::RestoreAndPopPositions(void)
{
        TRoboticPositioner *RP;

        //por cada punto objtivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador adscrito indicado para facilitar su acceso
                RP = Items[i]->getRoboticPositioner();

                //restaura y desempila la posición de sus ejes
                RP->getActuator()->getArm()->Restoretheta___3();
                RP->getActuator()->getArm()->Poptheta___3();
                RP->getActuator()->Restoretheta_1();
                RP->getActuator()->Poptheta_1();
        }
}

//---------------------------------------------------------------------------
//COLISIÓN:
//---------------------------------------------------------------------------

//levanta las banderas indicadoras de determinación de colisión
//pendiente de todos los posicionadores adscritos a los puntos objetivo
void TTargetPointList::EnablePendingCollisionDetermineTargetPoints(void)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->getRoboticPositioner()->getActuator()->Pending = true;
}

//busca los puntos objetivo que colisionan con otros puntos objetivo
void TTargetPointList::SearchCollindingTargetPoints(TVector<int> &indices)
{
        TTargetPoint *TP;
        TRoboticPositioner *RP;

        TVector<double> theta_1;
        TVector<double> theta___3;

        //NOTA: la cuantificación de los ejes puede estar activada o no.

        //guarda la configuración original de los posicionadores
        //desactiva las cuantificaciones de los ejes y
        //mueve los posicionadores a sus puntos objetivo adscritos:

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo indicado para facilitar su acceso
                TP = Items[i];
                //apunta el posicionador para facilitar su acceso
                RP = TP->getRoboticPositioner();
                //guarda las posiciones actuales
                theta_1.Add(RP->getActuator()->gettheta_1());
                theta___3.Add(RP->getActuator()->getArm()->gettheta___3());
                //mueve el posicionador a su punto objetivo
                TP->MoveToTargetP3();
        }

        //reinicia el vector de índices en congruencia
        //con la situación de partida
        indices.Clear();

        //levanta las baderas de colisión de todos los posicionadores
        getRoboticPositionerList()->EnablePending();

        //ADVERTENCIA: no basta con levantar las banderas de colisión de
        //los posicionadores adscritos  los puntos objetivo de la lista
        //porque la colisión puede darse con posicionadores que no tengan
        //punto objetivo adscrito.

        TItemsList<TRoboticPositioner*> RPs;
        bool collision;

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //si el posicionador indicado no está en la lista
                if(indices.Search(i) >= indices.getCount()) {
                        //apunta el posicionador adscrito al punto objetivo indicado
                        //para facilitar su acceso
                        RP = Items[i]->getRoboticPositioner();
                        //busca los posicionadores adyacentes que colisionan
                        RP->getActuator()->SearchCollindingPendingAdjacent(RPs);
                        //indica que ya ha determinado el estado de colisión del brazo del posicionador
                        RP->getActuator()->Pending = false;

                        //deshabilita la bandera de detección de colisión
                        collision = false;
                        //añade los índices a los puntos objetivo adscritos a los posicionadores con los que colisiona
                        for(int j=0; j<RPs.getCount(); j++) {
                                //busca un punto objetivo adscrito al posicioandor indicado
                                int k = SearchTargetPoint((TRoboticPositioner*)RPs[j]);
                                //si hay un punto objetivo adscrito al posicionador
                                if(k < getCount()) {
                                        //si el índice no está en la lista
                                        if(indices.Search(k) >= indices.getCount()) {
                                                //añade el índice al posicionador adyacente
                                                indices.Add(k);
                                                //habilita la bandera de detección de colisión
                                                collision = true;
                                        }
                                }
                        }
                        //si ha encontrado colisión con algún adyacente
                        if(collision)
                                //añade el índice al posicionador actual
                                indices.Add(i);
                }
        }

        //restaura la configuración original de los posicionadores:

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = Items[i]->getRoboticPositioner();
                //asigna los valores indicados de las listas
                RP->getActuator()->settheta_1(theta_1[i]);
                RP->getActuator()->getArm()->settheta___3(theta___3[i]);
        }

        //orde la lista de índices
        if(indices.getCount() > 1)
                indices.SortInc(0, indices.getCount()-1);
}

//---------------------------------------------------------------------------
//MÉTODOS DE INTERFAZ:

//selecciona todos los puntos objetivo de la lista
void TTargetPointList::SelectAll(void)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->Selected = true;
}
//deselecciona todos los puntos objetivo de la lista
void TTargetPointList::DeselectAll(void)
{
        for(int i=0; i<getCount(); i++)
                Items[i]->Selected = false;
}
/*#
//imprime los puntos objetivo en una caja de lista
void TTargetPointList::PrintTargetPoints(QListWidget *LB)
{
        //debe apuntar a un objeto construido
        if(LB == NULL)
                throw EImproperArgument("pointer LB should be not null");

        //reinicia la caja de lista para permitir su rellenado
        LB->clear();

        AnsiString S;
        TTargetPoint *TP;

        //por cada asignación de la lista
        for(int i=0; i<getCount(); i++) {
                //apunta el punto objetivo objetivo para facilitar su acceso
                TP = Items[i];

                S = IntToStr(TP->getRoboticPositioner()->getActuator()->getId());
                while(S.Length() < 8)
                        S += char(32);

                S += FloatToStr(TP->TargetP3.x);
                while(S.Length() < 32)
                        S += char(32);

                S += FloatToStr(TP->TargetP3.y);

                QString QS(S.c_str());
                LB->addItem(QS);
        }
}

//imprime los puntos objetivo en el lienzo de la fotografía
//de un trazador de formas
void TTargetPointList::PaintTargetPoints(TPloterShapes *PS)
{
        //el puntero PS debería apuntar a un trazador de formas contruido
        if(PS == NULL)
                throw EImproperArgument("pointer PS should point to Built ploter shapes");

        //por cada punto objetivo de la lista
        for(int i=0; i<getCount(); i++)
                Items[i]->Paint(PS); //dibuja el punto objetivo
}
*/
//busca el primer punto objetivo que está bajo el punto P
//y que parte del punto objetivo puede ser agarrado en
//el punto indicado:
//      i: índice al punto objetivo bajo el punto P;
//      dominio i: [0, Count];
//      n: parte del punto objetivo agarrada;
//         valores posibles:
//            0: niguna;
//            1: punto objetivo.
//valores de retorno:
//      false: ninguna parte agarrada
//      true: alguna parte agarrada
bool TTargetPointList::Grab(int &i, int &n, TDoublePoint P)
{
        for(i=0; i<getCount(); i++) {
                n = Items[i]->Grab(P);
                if(n > 0)
                        return true;
        }

        return false;
}

//---------------------------------------------------------------------------

} //namespace Positioning

//---------------------------------------------------------------------------

