//---------------------------------------------------------------------------
//Archivo: ExclusionArea.cpp
//Contenido: área de exclusión compuesta por barrera, posición y orientación
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

//#include <values.h>

#include "ExclusionArea.h"
#include "Strings.h" //StrIndent

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres de modelos
namespace Models {

//###########################################################################
//TExclusionArea
//###########################################################################

//---------------------------------------------------------------------------
//PROPIEDADES ESTÁTICAS:

//lista de objetos construidos
TItemsList<TExclusionArea*> TExclusionArea::Builts;

//---------------------------------------------------------------------------
//PROPIEDADES:

void TExclusionArea::setEo(double _Eo)
{
        //el valor de Eo debe ser no negativo
        if(_Eo < 0)
                throw EImproperArgument("Eo value should be nonnegative");

        __Eo = _Eo; //asignaelnuevo valor

        //asimila Eo
        CalculateSPM();
}
void TExclusionArea::setEp(double _Ep)
{
        //el valor de Ep debe ser no negativo
        if(_Ep < 0)
                throw EImproperArgument("Ep value should be nonnegative");

        __Ep = _Ep; //asignaelnuevo valor

        //asimila Ep
        CalculateSPM();
}

void TExclusionArea::setId(int _Id)
{
        //el número de identificación Id debería ser mayor que cero
        if(_Id <= 0)
                throw EImproperArgument("identification number Id should be upper zero");

        __Id = _Id; //asigna el nuevo valor
}

//PROPIEDADES EN FORMATO TEXTO:

AnsiString TExclusionArea::getEoText(void) const
{
        return FloatToStr(getEo());
}
void TExclusionArea::setEoText(const AnsiString &S)
{
        try {
                setEo(StrToFloat_(S));
        } catch(...) {
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
                setEp(StrToFloat_(S));
        } catch(...) {
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
                setId(StrToInt_(S));
        } catch(...) {
                throw;
        }
}

AnsiString TExclusionArea::getPendingText(void) const
{
        return BoolToStr_(Pending, true);
}
void TExclusionArea::setPendingText(const AnsiString &S)
{
        try {
                Pending = StrToBool_(S);
        } catch(...) {
                throw;
        }
}

//PROPIEDADES GRÁFICAS ENFORMATO TEXTO:
/*#
AnsiString TExclusionArea::getColorText(void) const
{
        return ColorToString(Color);
}
void TExclusionArea::setColorText(const AnsiString &S)
{
        try {
                Color = StringToColor(S);
        } catch(...) {
                throw;
        }
}*/
AnsiString TExclusionArea::getPaintBodyText(void) const
{
        return BoolToStr_(PaintBody, true);
}
void TExclusionArea::setPaintBodyText(const AnsiString &S)
{
        try {
                PaintBody = StrToBool_(S);
        } catch(...) {
                throw;
        }
}
AnsiString TExclusionArea::getSelectedText(void) const
{
        return BoolToStr_(Selected, true);
}
void TExclusionArea::setSelectedText(const AnsiString &S)
{
        try {
                Selected = StrToBool_(S);
        } catch(...) {
                throw;
        }
}

//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TExclusionArea::getAllText(void) const
{
        AnsiString S;

        S += AnsiString("BarrierAddress: ")+getBarrierAddressText()+AnsiString("\r\n");
        S += AnsiString("Eo = ")+getEoText()+AnsiString("\r\n");
        S += AnsiString("Ep = ")+getEpText()+AnsiString("\r\n");
        S += AnsiString("Id = ")+getIdText()+AnsiString("\r\n");
        S += AnsiString("Adjacents: ")+Adjacents.getText()+AnsiString("\r\n");
        S += AnsiString("Pending = ")+getPendingText();

        return S;
}
AnsiString TExclusionArea::getInstanceText(void) const
{
        AnsiString S;

        S += AnsiString("BarrierInstance:\r\n")+StrIndent(Barrier.getInstanceText())+AnsiString("\r\n");
        S += AnsiString("Eo = ")+getEoText()+AnsiString("\r\n");
        S += AnsiString("Ep = ")+getEpText();

        return S;
}
void TExclusionArea::setInstanceText(const AnsiString &S)
{
        try {
                //contruye una variable tampón
                TExclusionArea aux(this);
                TExclusionArea *EA = &aux;
                //lee la instancia y la asigna a la variable tampón
                int i = 1;
                ReadInstance((TExclusionArea*&)EA, S, i);

                //avanza el índice i hasta la próxima posición que no contenga un separador
                StrTravelSeparatorsIfAny(S, i);
                //si el índice i indica a algún caracter de la cadena S
                if(i <= S.Length())
                        //indica que la cadena S solo debería contener el valor para una instancia
                        throw EImproperArgument("string S should contain the instance value only");

                //asigna la variable tampón
                Copy(EA);

        } catch(...) {
                throw;
        }
}

AnsiString TExclusionArea::getOriginsText(void) const
{
        return getIdText()+AnsiString("\t")+Barrier.getP0().getRowText()+AnsiString("\t")+Barrier.getthetaO1Text();
}

//---------------------------------------------------------------------------
//MÉTODOS ESTÁTICOS:


//compara los identificadores de dos EAs
int  TExclusionArea::CompareIds(TExclusionArea *EA1, TExclusionArea *EA2)
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
void  TExclusionArea::PrintId(AnsiString &S, TExclusionArea *EA)
{
        //el puntero EA debería apuntar a un área de exclusión construida
        if(EA == NULL)
                throw EImproperArgument("pointer EA should point to built exclusion area");

        S += EA->getIdText();
}

//obtiene las etiquetas de las propiedades de origen
//en formato fila de texto
AnsiString TExclusionArea::GetOriginsLabelsRow(void)
{
        return "Id\tx0\ty0\tthetaO1";
}
//atraviesa las etiquetas de las propiedades de origen
//en formato fila de texto
void  TExclusionArea::TravelOriginsLabelsRow(const AnsiString& S, int& i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de la máquina de estados de lectura
        //      0: esperando etiqueta "Id"
        //      1: esperando separador y etiqueta "x0"
        //      2: esperando separador y etiqueta "y0"
        //      3: esperando separador y etiqueta "thetaO1"
        //      4: etiquetas de las propiedades de origen atravesadas con éxito
        int status = 0;

        do {
                switch(status) {
                        case 0: //esperando etiqueta "Id"
                                try {
                                        StrTravelLabel("Id", S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 1: //esperando separador y etiqueta "x0"
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("x0", S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 2: //esperando separador y etiqueta "y0"
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("y0", S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 3: //esperando separador y etiqueta "thetaO1"
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("thetaO1", S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                }
        //mientras no se hayan atravesado lasetiquetas de las propiedades de origen con éxito
        } while(status < 4);
}

//imprime los valores de las propiedades de orien de un EA
//al final de una cadena de texto
//en formato fila de texto
void  TExclusionArea::PrintOriginsRow(AnsiString& S,
        TExclusionArea *EA)
{
        //el puntero EA debe apuntar a un área de exclusión construida
        if(EA == NULL)
                throw EImproperArgument("pointer C should bepoint to built exclusion area");

        S += EA->getOriginsText();
}
//lee los valores de las propiedades de orien para un EA
//desde la posición indicada de una cadena de texto
//en formato fila de texto
void  TExclusionArea::ReadOriginsRow(TExclusionArea *EA,
        const AnsiString& S, int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el puntero EA debe apuntar a un área de exclusión construida
        if(EA == NULL)
                throw EImproperArgument("pointer C should bepoint to built exclusion area");

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de la máquina de estados de lectura
        //      0: esperando valor para Id
        //      1: esperando separador y valor para x0
        //      2: esperando separador y valor para y0
        //      3: esperando separador y valor para thetaO1
        //      4: conjunto devalores leido con éxito
        int status = 0;

        //variables tampón
        int _Id;
        double _x0, _y0;
        double _thetaO1;

        do {
                switch(status) {
                        case 0: //esperando valor para Id
                                try {
                                        StrReadInt(_Id, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 1: //esperando separador y valor para x0
                                try {
                                        StrTravelSeparators(S, i);
                                        StrReadFloat(_x0, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 2: //esperando separador y valor para y0
                                try {
                                        StrTravelSeparators(S, i);
                                        StrReadFloat(_y0, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 3: //esperando separador y valor para thetaO1
                                try {
                                        StrTravelSeparators(S, i);
                                        StrReadFloat(_thetaO1, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                }
        //mientras no se haya leido el conjunto de valores con éxito
        } while(status < 4);

        //asigna las variables tampón
        try {
                EA->SetOrigins(_Id, _x0, _y0, _thetaO1);
        }catch(...) {
                throw;
        }
}
//lee una instancia del EA
//desde la posición indicada de una cadena de texto
//en formato de asignaciones
void  TExclusionArea::ReadInstance(TExclusionArea* &EA,
        const AnsiString& S, int &i)
{
        //el puntero EA debe apuntar a una barrera construido
        if(EA == NULL)
                throw EImproperArgument("pointer EA shouldpoint to built exclusion area");

        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de la máquina de estados de lectura
        //      0: esperando etiqueta "BarrierInstance:"
        //      1: esperando instancia de Barrier
        //      2: esperando asignación a Eo
        //      3: esperando asignación a Ep
        //      4: instancia leida con éxito
        int status = 0;

        //variables tampón
        TExclusionArea _EA(EA);

        do {
                switch(status) {
                        case 0: //esperando la etiqueta "BarrierInstance:"
                                try {
                                        StrTravelLabel("BarrierInstance:", S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 1: //esperando instancia de Barrier
                                try {
                                        TBarrier *aux = &_EA.Barrier;
                                        TBarrier::ReadInstance((TBarrier*&)aux, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 2: //esperando asignación a Eo
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("Eo", S, i);
                                        StrTravelLabel("=", S, i);
                                        double _Eo;
                                        StrReadFloat(_Eo, S, i);
                                        _EA.setEo(_Eo);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 3: //esperando asignación a Ep
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel("Ep", S, i);
                                        StrTravelLabel("=", S, i);
                                        double _Ep;
                                        StrReadFloat(_Ep, S, i);
                                        _EA.setEp(_Ep);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                }
        //mientras no se haya leido la instancia con éxito
        } while(status < 4);

        //asigna la variable tampón
        EA->Copy(&_EA);
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

//contruye un área de exclusión
//con los valores por defecto
TExclusionArea::TExclusionArea(void) :
        Barrier(TDoublePoint(0, 0), 0),
        __Eo(MEGARA_Eo), __Ep(MEGARA_Ep),
        __Id(0),
        Adjacents(64, TRoboticPositioner::CompareIds, NULL, NULL, TRoboticPositioner::PrintId, NULL),
        Pending(true),
        /*#Color(Qt::gray), */PaintBody(true), Selected(false)
{
        //asimilalas propiedades de inicialización
        CalculateSPM();

        //añade el objeto a la lista de contruidos
        Builts.Add(this);
}
//construye un área de exclusión
//con los valores indicados
//si el número de identificación es menor que uno
//      lanza una execepción EImproperArgument
TExclusionArea::TExclusionArea(int _Id, TDoublePoint _P0, double _thetaO1) :
        Barrier(_P0, _thetaO1),
        __Eo(MEGARA_Eo), __Ep(MEGARA_Ep),
        Adjacents(6, TRoboticPositioner::CompareIds, NULL, NULL, TRoboticPositioner::PrintId, NULL),
        Pending(true),
        /*#Color(Qt::gray), */PaintBody(true), Selected(false)
{
        //el número de identificación Id debe ser mayor que cero
        if(_Id < 1)
                throw EImproperArgument("identification number Id should be upper zero");

        //ADVERTENCIA: está permitida la duplicidad de números de identificación Id.

        //inicializa las propiedades
        __Id = _Id;

        //asimilalas propiedades de inicialización
        CalculateSPM();

        //añade el objeto a la lista de contruidos
        Builts.Add(this);
}

//copia un área de exclusión
void TExclusionArea::Copy(const TExclusionArea *EA)
{
        //el puntero EA debería apuntar a un área de exclusión contruida
        if(EA == NULL)
                throw EImproperArgument("pointer EA ahould point to built exclusion area");

        Barrier.Copy(&EA->Barrier);
        __Eo = EA->__Eo;
        __Ep = EA->__Ep;
        __Id = EA->__Id;
        Adjacents.Clone(EA->Adjacents);
        Pending = EA->Pending;

        //#Color = EA->Color;
        PaintBody = EA->PaintBody;
        Selected = EA->Selected;
}

//contruye un clon de un área de exclusión
TExclusionArea::TExclusionArea(const TExclusionArea *EA) :
        //construye un clon de la barrera
        Barrier(EA->Barrier)
{
        //el puntero EA debería apuntar a una barrera contruida
        if(EA == NULL)
                throw EImproperArgument("pointer EA ahould point to built barrier");

        //copia todas las propiedades
        Copy(EA);

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
                throw EImproperCall(AnsiString("destruction attempt of non built object: ")+IntToHex(reinterpret_cast<intptr_t>(this), 8));

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
        return getEo()*Barrier.getr_max() + getEp(); //calcula y devuelve el valor
}

//---------------------------------------------------------------------------
//MÉTODOS DE ASIGNACIÓN CONJUNTA:

//asigna las propiedades de origen
void TExclusionArea::SetOrigins(int _Id, double _x0, double _y0, double _thetaO1)
{
        //el número de identificación Id debe ser mayor que cero
        if(_Id <= 0)
                throw EImproperArgument("identificator number Idshould beupper zero");

        //asigna los nuevos valores
        __Id = _Id;

        //asimila (P0, thetaO1)
        Barrier.Set(TDoublePoint(_x0, _y0), _thetaO1);
}

//asigna conjuntamente las tolerancias
//      (Eo, Ep)
void TExclusionArea::SetTolerances(double _Eo, double _Ep)
{
        //el valor de Eo debe ser no negativo
        if(_Eo < 0)
                throw EImproperArgument("Eo value should be nonnegative");

        //el valor de Ep debe ser no negativo
        if(_Ep < 0)
                throw EImproperArgument("Ep value should be nonnegative");

        __Eo = _Eo; //asignaelnuevo valor
        __Ep = _Ep; //asignaelnuevo valor

        //asimila (Eo, Ep)
        CalculateSPM();
}

//---------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//A partir de:
//      (Eo, Ep)
//      r_max
//Determina:
//      (Barrier.SPM)
void TExclusionArea::CalculateSPM(void)
{
        //CALULA LAS COMPONENTES DE SPM:

        Barrier.setSPM(SPMsta());
}

//---------------------------------------------------------------------------
//MÉTODOS DE COLISIÓN:

//determina los RPs adyacentes que pueden colisionar con la barrera
void TExclusionArea::DetermineAdjacents(const TItemsList<TRoboticPositioner*>& RPL)
{
        TRoboticPositioner *RP;
        TActuator *A;

        //por cada posicioandor de la lista de posicioandores
        for(int i=0; i<RPL.getCount(); i++) {
                //apunta el posicionador indicado para facilitar su acceso
                RP = RPL[i];
                //apunta el actuador del posicionador para facilitar su acceso
                A = RP->getActuator();
                //si el posicionador está lo bastante cerca como para colisionar con la barrera
                if(Barrier.getContour().DistanceMin(A->getP0()) < Barrier.getSPM() + A->getr_max() + A->getArm()->getSPM())
                        //apunta el posicionador en la lista de adyacentes
                        Adjacents.Add(RP);
        }
}
//determina los RPs adyacentes en estado de colisión con la barrera
void TExclusionArea::SearchCollinding(TItemsList<TRoboticPositioner*>& RPL)
{
        //inicializa la lista de punteros a actuadores
        RPL.Clear();

        TRoboticPositioner *RP;
        TActuator *A;

        //por cada actuador de la lista de adyacentes
        for(int i=0; i<Adjacents.getCount(); i++) {
                //apunta el posicionador para facilitar su acceso
                RP = Adjacents[i];
                //apunta el actuador del posicionador para facilitar su acceso
                A = RP->getActuator();
                //si el posicionador está lo bastante cerca como para colisionar con la barrera
                if(Barrier.getContour().Collides(A->getArm()->getContour(), Barrier.getSPM() + A->getArm()->getSPM()))
                        RPL.Add(RP); //apunta el posicionador en la lista de actuadores
        }
}

//--------------------------------------------------------------------------
//MÉTODOS PARA DETERMINAR LAS COLISIONES
//CON POSICIONADORES ADYACENTES:

//determina si hay colisión con un axctuador
bool TExclusionArea::ThereIsCollision(const TActuator* AA)
{
/*        //si la posición angular de ambos ejes de este posicionador es conocida
        if(PAkd != kdUnk) {
                //si la posición angular de ambos ejes del posicionador adyacente es conocida
                if(AA->PAkd != kdUnk) {
                        //calcula la distancia a partir de la cual los brazos de los actuadores
                        //están demasiado alejados para que sus brazos puedan colisionar
                        double Dmax = Arm->L1V + Arm->SPM + AA->Arm->L1V + AA->Arm->SPM;
                        //si los brazos de los actuadores no pueden colisionar
                        if(Mod(AA->Arm->P1 - Arm->P1) >= Dmax)
                                //indica que no hay colisión
                                return false;
                        //si los contornos están a una distancia menor que la suma de SPMs
                        if(Arm->Contour.DistanceMin(AA->Arm->Contour) < Arm->SPM + AA->Arm->SPM)
                                //indica que se ha encontrado colisión
                                return true;
                }
                //si la posición angular de algún eje del posicionador adyacente es desconocida
                else {
                        //calcula la distancia a partir de la cual los brazos de los actuadores
                        //están demasiado alejados para que sus brazos puedan colisionar
                        double Dmax = Arm->L1V + Arm->SPM + AA->Barrier->r_max + AA->Barrier->SPM;
                        //si los brazos de los actuadores no pueden colisionar
                        if(Mod(AA->Barrier->P0 - Arm->P1) >= Dmax)
                                //indica que no hay colisión
                                return false;
                        //si los contornos están a una distancia menor que la suma de SPMs
                        if(Arm->Contour.DistanceMin(AA->Barrier->Contour) < Arm->SPM + AA->Barrier->SPM)
                                //indica que se ha encontrado colisión
                                return true;
                }
        }
        //si la posición angular de algún eje de este posicionador es desconocida
        else {
                //si la posición angular de ambos ejes del posicionador adyacente es conocida
                if(AA->PAkd != kdUnk) {
                        //calcula la distancia a partir de la cual los brazos de los actuadores
                        //están demasiado alejados para que sus brazos puedan colisionar
                        double Dmax = Barrier->r_max + Barrier->SPM + AA->Arm->L1V + AA->Arm->SPM;
                        //si los brazos de los actuadores no pueden colisionar
                        if(Mod(AA->Arm->P1 - Barrier->P0) >= Dmax)
                                //indica que no hay colisión
                                return false;
                        //si los contornos están a una distancia menor que la suma de SPMs
                        if(Barrier->Contour.DistanceMin(AA->Arm->Contour) < Barrier->SPM + AA->Arm->SPM)
                                //indica que se ha encontrado colisión
                                return true;
                }
                //si la posición angular de algún eje del posicionador adyacente es desconocida
                else {
                        //calcula la distancia a partir de la cual los brazos de los actuadores
                        //están demasiado alejados para que sus brazos puedan colisionar
                        double Dmax = Barrier->r_max + Barrier->SPM + AA->Barrier->r_max + AA->Barrier->SPM;
                        //si los brazos de los actuadores no pueden colisionar
                        if(Mod(AA->Barrier->P0 - Barrier->P0) >= Dmax)
                                //indica que no hay colisión
                                return false;
                        //si los contornos están a una distancia menor que la suma de SPMs
                        if(Barrier->Contour.DistanceMin(AA->Barrier->Contour) < Barrier->SPM + AA->Barrier->SPM)
                                //indica que se ha encontrado colisión
                                return true;
                }
        }

        //indica que no ha encontrado colisión
        return false;*/
}
//determina si hay colisión con un actuador adyacente
bool TExclusionArea::ThereIsCollisionWithAdjacent(void)
{
/*        TExclusionArea *AA;

        //por cada posicionador adyacente
        for(int i=0; i<Adjacents.Count; i++)
                //si hay colisión
                if(ThereIsCollision(Adjacents[i]->Actuator))
                        //indica que se está invadiendo el espacio de seguridad de algún adyacente
                        return true;

        //indica que no se invade el espacio de seguridad de ningún adyacente
        return false;*/
}

//Busca los posicionadores adyacentes cuyo
//brazo colisiona con la barrera de este área de exclusión.
void TExclusionArea::SearchCollindingAdjacent(TItemsList<TActuator*> &Collindings)
{
/*        //vacia la lista de los que colisionan en congruencia con el estado inicial
        Collindings.Clear();

        TExclusionArea *AA;

        //busca la invasión con alguno de sus adyacentes
        for(int i=0; i<Adjacents.Count; i++) {
                //apunta el posicionador adyacente indicado para facilitar su acceso
                AA = (TExclusionArea*)Adjacents[i];
                //si el posicionador adyacente
                //      colisiona con el posicionador indicado
                if(ThereIsCollision(AA))
                        //añade el índice al posicionador adyacente
                        Collindings.Add(AA);
        }*/
}

//Determina si hay colisión con un actuador adyacente
//con evalauación de colisión pendiente.
bool TExclusionArea::ThereIsCollisionWithPendingAdjacent(void)
{
/*        TExclusionArea *AA;

        //por cada posicionador adyacente
        for(int i=0; i<Adjacents.Count; i++) {
                //apunta el posicionador indicado para facilitar su acceso
                AA = (TExclusionArea*)Adjacents[i];
                //si el posicionador adyacente
                //      tiene pendiente la determinación de colisión,
                //      y colisiona con el posicionador indicado
                if(AA->Pending && ThereIsCollision(AA))
                        //indica que se ha encontrado colisión
                        return true;
        }

        //ADVERTENCIA: se excluirá la condición PendingCollisionDetermine
        //mintras no se demuestre que funciona bien.

        //indica que no se invade el espacio de seguridad de ningún adyacente
        return false;*/
}

//Busca los posicionadores adyacentes
//con evalauación de colisión pendiente
//cuyo brazo colisiona con la barrera de este área de exlusión.
void TExclusionArea::SearchCollindingPendingAdjacent(TItemsList<TActuator*> &Collindings)
{
/*        //vacia la lista de los que colisionan en congruencia con el estado inicial
        Collindings.Clear();

        TExclusionArea *AA;

        //busca la invasión con alguno de sus adyacentes
        for(int i=0; i<Adjacents.Count; i++) {
                //apunta el posicionador adyacente indicado para facilitar su acceso
                AA = (TExclusionArea*)Adjacents[i];
                //si el posicionador adyacente
                //      tiene pendiente la determinación de colisión,
                //      y colisiona con el posicionador indicado
                if(AA->Pending && ThereIsCollision(AA))
                        //añade el índice al posicionador adyacente
                        Collindings.Add(AA);
        }*/
}

//-------------------------------------------------------------------
//MÉTODOS GRÁFICOS:

//indica que parte del EA puede ser agarrada en el punto indicado
//      1: barrera;
//      0: niguna;
int TExclusionArea::Grab(TDoublePoint P)
{
        //si el punto P está dentro del contorno de la barrera
        if(Barrier.Covers(P))
                return 1; //agarra la barrera

        return 0;
}
/*#
//dibuja un EA en un trazador
void TExclusionArea::Paint(TPloterShapes *PS)
{
        //el puntero PS debería apuntar a un trazdor de formas contruido
        if(PS == NULL)
                throw EImproperArgument("pointer PS should point to builts ploter shapes");

        //si el EA está seleccionado
        if(Selected) {
                //configura el color de la pluma
                if(double(Luminance(PS->getBackColor())) < 255.*1.5)
                        PS->setPenColor(Qt::white);
                else
                        PS->setPenColor(Qt::black);

                //si tiene que dibujar el cuerpo
                if(PaintBody)
                        //dibuja la barrera resaltada
                        Barrier.Paint(PS, PS->getPenColor());
        }
        //si el EA no está seleccionado
        else {
                //si tiene que dibujar el cuerpo
                if(PaintBody)
                        //dibuja la barrera con su color correspondiente
                        Barrier.Paint(PS, Color);
        }
}
//dibuja el EA en un trazador
void TExclusionArea::PaintSimplified(TPloterShapes *PS)
{
        //el puntero PS debería apuntar a un trazdor de formas contruido
        if(PS == NULL)
                throw EImproperArgument("pointer PS should point to builts ploter shapes");

        //si el EA está seleccionado
        if(Selected) {
                //configura el color de la pluma
                if(double(Luminance(PS->getBackColor())) < 255.*1.5)
                        PS->setPenColor(Qt::white);
                else
                        PS->setPenColor(Qt::black);

                //si tiene que dibujar el cuerpo
                if(PaintBody)
                        //dibuja la barrera resaltada
                        Barrier.Paint(PS, PS->getPenColor());
        }
        //si el EA no está seleccionado
        else {
                //si tiene que dibujar el cuerpo
                if(PaintBody)
                        //dibuja la barrera con su color correspondiente
                        Barrier.Paint(PS, Color);
        }
}
*/
//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

