//---------------------------------------------------------------------------
//Archivo: FiberConnectionModel.cpp
//Contenido: modelo de conexión de fibras
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "FiberConnectionModel.h"
#include "TextFile.h"

//---------------------------------------------------------------------------

using namespace Strings;

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//clase conexión de la pseudoslit

//etiquetas
AnsiString TConection::RPIdLabel = "RPId";
AnsiString TConection::SpaxelIdLabel = "SpaxelId";
AnsiString TConection::FiberIdLabel = "FiberId";
AnsiString TConection::sLabel = "s";

void TConection::setRPId(int _RPId)
{
        //RPId debe ser no negativo
        if(_RPId < 0)
                throw EImproperArgument("RPId should be nonnegative");

        __RPId = _RPId;
}
void TConection::setSpaxelId(int _SpaxelId)
{
        //SpaxelId debe estar en [0, 7]
        if(_SpaxelId<0 || 7<_SpaxelId)
                throw EImproperArgument("SpaxelId should be in [0, 7]");

        __SpaxelId = _SpaxelId;
}
void TConection::setFiberId(int _FiberId)
{
        //FiberId debe ser no negativo
        if(_FiberId < 0)
                throw EImproperArgument("FiberId should be nonnegative");

        __FiberId = _FiberId;
}
void TConection::sets(double _s)
{
        __s = _s;
}

AnsiString TConection::getRPIdText(void) const
{
        return IntToStr(getRPId());
}
void TConection::setRPIdText(const AnsiString& S)
{
        try {
                setRPId(StrToInt_(S));
        } catch(...) {
                throw;
        }
}
AnsiString TConection::getSpaxelIdText(void) const
{
        return IntToStr(getSpaxelId());
}
void TConection::setSpaxelIdText(const AnsiString& S)
{
        try {
                setSpaxelId(StrToInt_(S));
        } catch(...) {
                throw;
        }
}
AnsiString TConection::getFiberIdText(void) const
{
        return IntToStr(getFiberId());
}
void TConection::setFiberIdText(const AnsiString& S)
{
        try {
                setFiberId(StrToInt_(S));
        } catch(...) {
                throw;
        }
}
AnsiString TConection::getsText(void) const
{
        return FloatToStr(gets());
}
void TConection::setsText(const AnsiString& S)
{
        try {
                sets(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}

//lee los valores de las propiedades en una fila de texto
void  TConection::ReadRow(TConection *Item,
        const AnsiString& S, int& i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el puntero Item debería apauntar a un elemento construido
        if(Item == NULL)
                throw EImproperArgument("pointer Item should point to built item");

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de la máquina de estados de lectura
        //      0: esperando valor para RPId
        //      1: esperando separador y valor para SpaxelId
        //      2: esperando separador y valor para FiberId
        //      3: esperando separador y valor para s
        //      4: elemento leido con éxito
        int status = 0;

        //variables tampón
        int _RPId;
        int _SpaxelId;
        int _FiberId;
        double _s;

        do {
                switch(status) {
                        case 0: //esperando valor para RPId
                                try {
                                        StrReadInt(_RPId, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 1: //esperando valor para SpaxelId
                                try {
                                        StrReadInt(_SpaxelId, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 2: //esperando valor para FiberId
                                try {
                                        StrReadInt(_FiberId, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 3: //esperando separador y valor para s
                                try {
                                        StrTravelSeparators(S, i);
                                        StrReadFloat(_s, S, i);
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
                Item->Set(_RPId, _SpaxelId, _FiberId, _s);
        }catch(...) {
                throw;
        }
}
//imprime los valores de las propiedades en una fila de texto
void  TConection::PrintRow(AnsiString& S, const TConection *C)
{
        //el puntero C debe apuntar a una conexión construida
        if(C == NULL)
                throw EImproperArgument("pointer C should point to built conection");

        S += C->getRPIdText()+AnsiString("\t")+C->getSpaxelIdText()+AnsiString("\t")+C->getFiberIdText()+AnsiString("\t")+C->getsText();
}
//obtiene las etiqeutas de las propiedades en formato texto
AnsiString TConection::GetLabelsRow(void)
{
        return RPIdLabel+AnsiString("\t")+SpaxelIdLabel+AnsiString("\t")+FiberIdLabel+AnsiString("\t")+sLabel;
}
//atraviesa las etiquetas de las propiedades
void TConection::TravelLabelsRow(const AnsiString& S, int& i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de la máquina de estados de lectura
        //      0: esperando RPIdLabel
        //      1: esperando separador y SpaxelIdLabel
        //      2: esperando separador y FiberIdLabel
        //      3: esperando separador y sLabel
        //      4: etiquetas atravesadas con éxito
        int status = 0;

        do {
                switch(status) {
                        case 0: //esperando RPIdLabel
                                try {
                                        StrTravelLabel(RPIdLabel, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 1: //esperando separador y SpaxelIdLabel
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel(SpaxelIdLabel, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 2: //esperando separador y FiberIdLabel
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel(FiberIdLabel, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                        case 3: //esperando separador y sLabel
                                try {
                                        StrTravelSeparators(S, i);
                                        StrTravelLabel(sLabel, S, i);
                                }catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                }
        //mientras no se hayan atravesado todas las etiquetas con éxito
        } while(status < 4);
}

//asigna las propiedades atómicamente
//si no es posible asignar todas las propiedades:
//      lanza una excepción EImproperArgument
void TConection::Set(int _RPId, int _SpaxelId, int _FiberId, double _s)
{
        //RPId debe ser no negativo
        if(_RPId < 0)
                throw EImproperArgument("RPId should be nonnegative");
        //SpaxelId debe estar en [0, 7]
        if(_SpaxelId<0 || 7<_SpaxelId)
                throw EImproperArgument("SpaxelId should be in [0, 7]");
        //FiberId debe ser no negativo
        if(_FiberId < 0)
                throw EImproperArgument("FiberId should be nonnegative");

        __RPId = _RPId;
        __SpaxelId = _SpaxelId;
        __FiberId = _FiberId;
        __s = _s;
}

//---------------------------------------------------------------------------
//clase modelo del espectrografo

void TFiberConnectionModel::setR(double _R)
{
        //el radio R debe ser mayor que cero
        if(_R <= 0)
                throw EImproperArgument("radious R shoukd be upper zero");

        __R = _R;

        //El valor de R afectará a los métodos GetSpaxelCentre#.
}

AnsiString TFiberConnectionModel::getConnectionsText(void) const
{
        return TConection::GetLabelsRow()+AnsiString("\r\n")+Conections.getColumnText();
}
void TFiberConnectionModel::setConnectionsText(const AnsiString& S)
{
        try {
                //indica a la primera posición de la cadena
                int i = 1;
                //atraviesa la cabecera de la tabla
                TConection::TravelLabelsRow(S, i);

                //lee las conexiones en una variable tampón
                TPointersList<TConection> _Conections;
                _Conections.Read = TConection::ReadRow;
                TPointersList<TConection>::ReadSeparated(&_Conections, S, i);

                //avanza el índice hasta el primer caracter no separador o hasta el final de la cadena
                StrTravelSeparators(S, i);
                //si el índice no indica a la posúltima posición de la cadena
                if(i < S.Length())
                        //indica que la cadena S no contiene una tablña de conexiones
                        throw EImproperArgument("the string S not contains a conection table");

                //asigna la varibale tampón
                Conections.Clear();
                Conections.Add(_Conections);
                _Conections.ClearWithoutDestroy();
/*                while(_Conections.Count > 0) {
                        Conections.Add(_Conections.FirstPointer);
                        _Conections.DeleteWithoutDestroy(0);
                }*/

        } catch(...) {
                throw;
        }
}

//construye un espectrógrafo con los valores por defecto
TFiberConnectionModel::TFiberConnectionModel(void) : __R(0.443),
        Conections(644, NULL, NULL, NULL, TConection::PrintRow, TConection::ReadRow)
{
}

//determina las coordenadas en S0 del centro del spaxel 1
void TFiberConnectionModel::GetSpaxelCenter1(double&x, double& y,
        double x3, double y3, double theta3)
{
        //contruye el vector V orientado en la dirección 0
        TDoublePoint V(getR(), 0);

        //rota el punto V (-PI/6 + theta3) rad
        double a = -M_PI/6;// + theta3;
        x = V.x*cos(a) - V.y*sin(a);
        y = V.x*sin(a) + V.y*cos(a);

        //desplaza (x, y) a (x3, y3)
        x += x3;
        y += y3;
}
//determina las coordenadas en S0 del centro del spaxel 2
void TFiberConnectionModel::GetSpaxelCenter2(double&x, double& y,
        double x3, double y3, double theta3)
{
        //contruye el vector V orientado en la dirección 0
        TDoublePoint V(getR(), 0);

        //rota el punto V (PI/6 + theta3) rad
        double a = M_PI/6;// + theta3;
        x = V.x*cos(a) - V.y*sin(a);
        y = V.x*sin(a) + V.y*cos(a);

        //desplaza (x, y) a (x3, y3)
        x += x3;
        y += y3;
}
//determina las coordenadas en S0 del centro del spaxel 3
void TFiberConnectionModel::GetSpaxelCenter3(double&x, double& y,
        double x3, double y3, double theta3)
{
        //contruye el vector V orientado en la dirección 0
        TDoublePoint V(getR(), 0);

        //rota el punto V (-PI/2 + theta3) rad
        double a = -M_PI/2;// + theta3;
        x = V.x*cos(a) - V.y*sin(a);
        y = V.x*sin(a) + V.y*cos(a);

        //desplaza (x, y) a (x3, y3)
        x += x3;
        y += y3;
}
//determina las coordenadas en S0 del centro del spaxel 4
void TFiberConnectionModel::GetSpaxelCenter4(double&x, double& y,
        double x3, double y3, double theta3)
{
        //desplaza (x, y) a (x3, y3)
        x = x3;
        y = y3;
}
//determina las coordenadas en S0 del centro del spaxel 5
void TFiberConnectionModel::GetSpaxelCenter5(double&x, double& y,
        double x3, double y3, double theta3)
{
        //contruye el vector V orientado en la dirección 0
        TDoublePoint V(getR(), 0);

        //rota el punto V (PI/2 + theta3) rad
        double a = M_PI/2;// + theta3;
        x = V.x*cos(a) - V.y*sin(a);
        y = V.x*sin(a) + V.y*cos(a);

        //desplaza (x, y) a (x3, y3)
        x += x3;
        y += y3;
}
//determina las coordenadas en S0 del centro del spaxel 6
void TFiberConnectionModel::GetSpaxelCenter6(double&x, double& y,
        double x3, double y3, double theta3)
{
        //contruye el vector V orientado en la dirección 0
        TDoublePoint V(getR(), 0);

        //rota el punto V (PI*5/6 + theta3) rad
        double a = M_PI*5./6;// + theta3;
        x = V.x*cos(a) - V.y*sin(a);
        y = V.x*sin(a) + V.y*cos(a);

        //desplaza (x, y) a (x3, y3)
        x += x3;
        y += y3;
}
//determina las coordenadas en S0 del centro del spaxel 7
void TFiberConnectionModel::GetSpaxelCenter7(double&x, double& y,
        double x3, double y3, double theta3)
{
        //contruye el vector V orientado en la dirección 0
        TDoublePoint V(getR(), 0);

        //rota el punto V (-PI*5/6 + theta3) rad
        double a = -M_PI*5./6;// + theta3;
        x = V.x*cos(a) - V.y*sin(a);
        y = V.x*sin(a) + V.y*cos(a);

        //desplaza (x, y) a (x3, y3)
        x += x3;
        y += y3;
}

//Determina (FiberId, s) a partir de (RPId, SpaxelId).
//Si no encuentra (RPId, SpaxelId) en la tabla de posiciones
//relativas de la pseudoslit: devuelve falso.
bool TFiberConnectionModel::Find(int& FiberId, double& s, int RPId, int SpaxelId)
{
        //RPId debe ser no negativo
        if(RPId < 0)
                throw EImproperArgument("RPId should be nonnegative");
        //SpaxelId debe estar en [0, 7]
        if(SpaxelId<0 || 7<SpaxelId)
                throw EImproperArgument("SpaxelId should be in [0, 7]");

        //indica a la primera posición de la lista
        int i = 0;
        //mientras i indque a una conexión, y el par (RId, SpaxelID) no coincida con el de la conexión...
        while(i<Conections.getCount() && (RPId!=Conections[i].getRPId() || SpaxelId!=Conections[i].getSpaxelId())) {
                i++;
        }

        //si no ha encontrado una conexión devuelve falso
        if(i >= Conections.getCount())
                return false;

        //asigna el (FiberId, s) de la conexión encontrada
        FiberId = Conections[i].getFiberId();
        s = Conections[i].gets();
        //indica que ha encontrado la conexión
        return true;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

