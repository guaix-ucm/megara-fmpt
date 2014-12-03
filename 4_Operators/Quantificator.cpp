//---------------------------------------------------------------------------
//Archivo: Quantificator.cpp
//Contenido: operador de cuantificación
//Última actualización: 21-03-2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Quantificator.h"

//---------------------------------------------------------------------------

//espacio de nombres de operadores matemáticos
namespace Operators {

//---------------------------------------------------------------------------
//PARÁMETROS DE CONFIGURACIÓN:

void TQuantificator::setq(double _q)
{
        //el intervalo de cuantificación q debería ser distinto de cero
        if(_q == 0)
                throw EImproperArgument("quantification interval q should be nonnegative");

        //asigna el nuevo valor
        __q = _q;

        //asimila el nuevo valor
        CalculateImageDomain();
}
void TQuantificator::setxmin(double _xmin)
{
        //el límite inferior del intervalo de cuantificación xmin no debe ser mayor que el límite superior xmax
        if(_xmin >= __xmax)
                throw EImproperArgument("lower limit quantification interval xmin should not sceed upper limit xmax");

        //asigna el nuevo valor
        __xmin = _xmin;

        //asimila el nuevo valor
        CalculateImageDomain();
}
void TQuantificator::setxmax(double _xmax)
{
        //el límite superior del intervalo de cuantificación xmax no debe ser menor que el límite inferior xmin
        if(_xmax <= __xmin)
                throw EImproperArgument("upper limit quantification interval xmax should not below lower limit xmin");

        //asigna el nuevo valor
        __xmax = _xmax;

        //asimila el nuevo valor
        CalculateImageDomain();
}

//---------------------------------------------------------------------------
//PROPIEDADES EN FORMATO TEXTO:

void TQuantificator::setqText(AnsiString &S)
{
        try {
                setq(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}
void TQuantificator::setxminText(AnsiString &S)
{
        try {
                setxmin(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}
void TQuantificator::setxmaxText(AnsiString &S)
{
        try {
                setxmax(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}

//---------------------------------------------------------------------------
//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TQuantificator::getRWText(void)
{
        AnsiString S;
        PrintRW(S, this);
        return S;
}

void TQuantificator::setRWText(const AnsiString &S)
{
        try {
                int i = 1;
                ReadRW(this, S, i);
        } catch(...) {
                throw;
        }
}

AnsiString TQuantificator::getRText(void)
{
        AnsiString S = AnsiString("[")+getQminText()+AnsiString(", ")+getQmaxText()+AnsiString("]");
        return S;
}

AnsiString TQuantificator::getAssignsText(void) const
{
        AnsiString S;

        S = AnsiString("q = ")+getqText()+AnsiString("\r\n");
        S += AnsiString("xmin = ")+getxminText()+AnsiString("\r\n");
        S += AnsiString("xmax = ")+getxmaxText()+AnsiString("\r\n");

        S += AnsiString("Qmin = ")+getQminText()+AnsiString("\r\n");
        S += AnsiString("Qmax = ")+getQmaxText();

        return S;
}

void TQuantificator::setAssignsText(const AnsiString &S)
{
        try {
                int i = 1;
                ReadAssigns(this, S, i);
        } catch(...) {
                throw;
        }
}

//---------------------------------------------------------------------------
//MÉTODOS DE ASIMILACIÓN:

//calcula los límties del dominio imagen de la función:
//      (Qmin, Qmax),
//a partir de los parámetros de cuantificación:
//      (q, xmin, xmax)
void TQuantificator::CalculateImageDomain(void)
{
        __Qmin = ceil(__xmin/fabs(__q));
        __Qmax = floor(__xmax/fabs(__q));
}

//---------------------------------------------------------------------------
//FUNCIONES ESTÁTICAS DE IMPRESIÓN Y LECTURA:

//imprime el conjunto de propiedades RW de un cuantificador
//en una cadena de caracteres
void __fastcall TQuantificator::PrintRW(AnsiString &S, const TQuantificator *Q)
{
        //el puntero Q debería apuntar a un cuantificador construido
        if(Q == NULL)
                throw EImproperArgument("pointer Q should point to built quantificator");

    S += AnsiString("(")+Q->getqText()+AnsiString(", [")+Q->getxminText()+AnsiString(", ")+Q->getxmaxText()+AnsiString("])");
}

//intenta leer el conjunto de propiedades RW un cuantificador
//a partir de la posición indicada en una cadena de caracteres
void __fastcall TQuantificator::ReadRW(TQuantificator *Q, const AnsiString &S,
        int &i)
{
        //el puntero Q debería apuntar a un cuantificador construido
        if(Q == NULL)
                throw EImproperArgument("pointer Q should point to built quantificator");

                //NOTA: no se exige que la cadena de texto S sea imprimible,
                //de modo que cuando se quiera imprimir uno de sus caracteres,
                //si no es imprimible saldrá el caraacter por defecto.

                //el índice i debería apauntar a un caracter de la cadena de texto S
                if(i<1 || S.Length()<i)
                        throw EImproperArgument("index i should indicate an character in the string S");

                char c; //caracter S[i]
                //estado de la máquian de estados
                //      0: esperando ' ' o '('
                //      1: esperando ' ' o escalón de cuantificación q
                //      2: esperando ' ' o ':'
                //      3: esperando ' ' o intervalo cerrado '[xinf, xsup]'
                //      4: esperando ' ' o ')'
                //      5: elemento convertido con éxito
                int status = 0;

                //variables tampón
                double q;
                TInterval I;

                do {
                        c = S[i];

                        switch(status) {
                                case 0: //esperando ' ' o '('
                                        switch(c) {
                                                case ' ':
                                                        i++;
                                                        if(i > S.Length())
                                                                throw EImproperArgument("character '(' not found");
                                                        break;
                                                case '(':
                                                        i++;
                                                        status++;
                                                        break;
                                                default:
                                                        throw EImproperArgument(AnsiString("character '")+AnsiString(c)+AnsiString("' should be ' ' or '('"));
                                        }
                                        break;
                                case 1: //esperando ' ' o ecalón de cuantificación q
                                        switch(c) {
                                                case ' ':
                                                        i++;
                                                        if(i > S.Length())
                                                                throw EImproperArgument("quantization step q not found");
                                                        break;
                                                default:
                                                        StrReadFloat(q, S, i);
                                                        status++;
                                        }
                                        break;
                                case 2: //esperando ' ' o ','
                                        switch(c) {
                                                case ' ':
                                                        i++;
                                                        if(i > S.Length())
                                                                throw EImproperArgument("sign puntuation ',' not found");
                                                        break;
                                                case ',':
                                                        i++;
                                                        if(i > S.Length())
                                                                throw EImproperArgument("sign puntuation ',' not found");
                                                        status++;
                                                        break;
                                                default:
                                                        throw EImproperArgument(AnsiString("character '")+AnsiString(c)+AnsiString("' should be ' ' or ','"));
                                        }
                                        break;
                                case 3: //esperando ' ' o intervalo cerrado '[xinf, xsup]'
                                        switch(c) {
                                                case ' ':
                                                        i++;
                                                        if(i > S.Length())
                                                                throw EImproperArgument("closed interval '[xinf, xsup]' not found");
                                                        break;
                                                default:
                                                        TInterval::Read(&I, S, i);
                                                        if(I.IntervalType != itClose)
                                                                throw EImproperArgument(AnsiString("interval '")+I.getText()+AnsiString("' should be closed"));
                                                        status++;
                                        }
                                        break;
                                case 4: //esperando ' ' o ')'
                                        switch(c) {
                                                case ' ':
                                                        i++;
                                                        if(i > S.Length())
                                                                throw EImproperArgument("character ')' not found");
                                                        break;
                                                case ')':
                                                        i++;
                                                        status++;
                                                        break;
                                                default:
                                                        throw EImproperArgument(AnsiString("character '")+AnsiString(c)+AnsiString("' should be ' ', ')'"));

                                                        //NOTA:
                                                        //sería posible determinar si el número de punto flotante contiene un punto decimal o no
                                                        //sustituyento:
                                                        //      StrScanFloat(x, S, i);
                                                        //por:
                                                        //      try {
                                                        //              //segrega un punto flotante a aprtir de i
                                                        //              StrScanFloatStr(ValueStr, S, i);
                                                        //
                                                        //              //intenta convertir a entero
                                                        //              x = StrToFloat(ValueStr);
                                                        //
                                                        //              //captura el efecto lateral de la función de conversión
                                                        //              double aux = 0; aux + aux;
                                                        //
                                                        //      } catch(EOverflow &E) {
                                                        //              throw EImproperArgument("'"+F+"' is a floating point value out of range");
                                                        //      } catch(...) {
                                                        //              throw;
                                                        //      }
                                                        //y buscando el caracter DecimalSeparator en la cadena ValueStr,
                                                        //pero esto resulta demasiado engorroso y se prefiere indicar que el caracter que sigue
                                                        //debería ser el DecimalSeparator si es que ha sido omitido.
                                        }
                                        break;
                        }
                //mientras el elemento no haya sifdo conversito con éxito
                } while(status < 5);

                //asigna el nuevo valor
                Q->Set(q, I.getlmin(), I.getlmax());
}

//-------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//FUNCIONES ESTÁTICAS:

//imprime un cluster de configuración al final de una cadena de texto
void __fastcall TQuantificator::PrintAssigns(AnsiString &S,
        const TQuantificator *Q)
{
        //el puntero Q debería apuntar a un cuantificador contruido
        if(Q == NULL)
                throw EImproperArgument("pointer Q should point to built quantificator");

        S += Q->getAssignsText();
}
//lee un cluster de configuración en una cadena de texto
void __fastcall TQuantificator::ReadAssigns(TQuantificator *Q,
        const AnsiString &S, int &i)
{
        //el puntero Q debería apuntar a un cuantificador contruido
        if(Q == NULL)
                throw EImproperArgument("pointer Q should point to built quantificator");

        //si el índice i no indica un caracter de la cadena de texto S
        if(i<0 || S.Length()<i)
                //indica que no ha encontrado el cluster
                throw EImproperArgument("sizing cluster propertys not found");

        //estado de lectura
        //      0: esperando sentencia de asignación a q
        //      1: esperando sentencia de asignación a xmin
        //      2: esperando sentencia de asignación a xmax
        //
        //      3: esperando sentencia de asignación a Qmin
        //      4: esperando sentencia de asignación a Qmax
        //
        //      5: cluster leido con éxito
        int status = 0;

        //variables auxiliares
        AnsiString Ident, Value;
        double q, xmin, xmax;

        //ADVERTENCIA: las variables tampón con propiedades interdependientes
        //deben ser clones de las variables que se pretenden modificar.

        //ADVERTENCIA: las propiedades enteras deben ser leidas como
        //valores en punto flotante para detectar errores en los cuales
        //sea especificado un valor en punto flotante en vez de un valor entero.

        do {
                switch(status) {
                        case 0: //esperando sentencia de asignación a q
                                try {
                                        StrReadLabel(Ident, "q", S, i);
                                        StrTravelLabel("=", S, i);
                                        StrReadFloatStr(Value, S, i);
                                        q = StrToFloat_(Value);
                                } catch(EImproperArgument &E) {
                                        throw EImproperArgument(E.Message+AnsiString(" for property q"));
                                } catch(...) {
                                        throw;
                                }
                                if(i > S.Length())
                                        throw EImproperArgument("xmin not found");
                                status++;
                                break;
                        case 1: //esperando sentencia de asignación a xmin
                                try {
                                        StrReadLabel(Ident, "xmin", S, i);
                                        StrTravelLabel("=", S, i);
                                        StrReadFloatStr(Value, S, i);
                                        xmin = StrToFloat_(Value);
                                } catch(EImproperArgument &E) {
                                        throw EImproperArgument(E.Message+AnsiString(" for property xmin"));
                                } catch(...) {
                                        throw;
                                }
                                if(i > S.Length())
                                        throw EImproperArgument("xmax not found");
                                status++;
                                break;
                        case 2: //esperando sentencia de asignación a xmax
                                try {
                                        StrReadLabel(Ident, "xmax", S, i);
                                        StrTravelLabel("=", S, i);
                                        StrReadFloatStr(Value, S, i);
                                        xmax = StrToFloat_(Value);
                                } catch(EImproperArgument &E) {
                                        throw EImproperArgument(E.Message+AnsiString(" for property xmax"));
                                } catch(...) {
                                        throw;
                                }
                                if(i > S.Length())
                                        throw EImproperArgument("Qmin not found");
                                status++;
                                break;
                        case 3: //esperando sentencia de asignación a Qmin
                                try {
                                        StrReadLabel(Ident, "Qmin", S, i);
                                        StrTravelLabel("=", S, i);
                                        StrReadFloatStr(Value, S, i);
                                        if(Q->getQminText() != Value)
                                                throw EImproperArgument("write not allowed");
                                } catch(EImproperArgument &E) {
                                        throw EImproperArgument(E.Message+AnsiString(" for property Qmin"));
                                } catch(...) {
                                        throw;
                                }
                                if(i > S.Length())
                                        throw EImproperArgument("Qmax not found");
                                status++;
                                break;
                        case 4: //esperando sentencia de asignación a Qmax
                                try {
                                        StrReadLabel(Ident, "Qmax", S, i);
                                        StrTravelLabel("=", S, i);
                                        StrReadFloatStr(Value, S, i);
                                        if(Q->getQmaxText() != Value)
                                                throw EImproperArgument("write not allowed");
                                } catch(EImproperArgument &E) {
                                        throw EImproperArgument(E.Message+AnsiString(" for property Qmax"));
                                } catch(...) {
                                        throw;
                                }
                                status++;
                                break;
                }
        } while(status < 5);

        //intenta asignar las variables tampón
        try {
                Q->Set(q, xmin, xmax);
        } catch(...) {
                throw;
        }
}

//CONTRUCTORES Y COPIA

//contruye un cuantificador
TQuantificator::TQuantificator(double _q, double _xmin, double _xmax)
{
        //el intervalo de cuantificación q debería ser distinto de cero
        if(_q == 0)
                throw EImproperArgument("quantification interval q should be nonnegative");

        //el límite superior del intervalo de cuantificación xmax no debe ser menor que el límite inferior xmin
        if(_xmax <= _xmin)
                throw EImproperArgument("upper limit quantification interval xmax should not below lower limit xmin");

        //asigna los valores
        __q = _q;
        __xmin = _xmin;
        __xmax = _xmax;

        //asimila los parámetros de configuración
        CalculateImageDomain();
}
//copia todas las propiedades de un cuantificador
void TQuantificator::Copy(const TQuantificator *Q)
{
        //el puntero Q deberí apauntar a un cuantificador contruido
        if(Q == NULL)
                throw EImproperArgument("pointer Q ahould point to built quantificator");

        //copia las propiedades de configuración
        __q = Q->__q;
        __xmin = Q->__xmin;
        __xmax = Q->__xmax;
        //copia las propiedades derivadas
        __Qmin = Q->__Qmin;
        __Qmax = Q->__Qmax;
}
void TQuantificator::Copy(const TQuantificator& Q)
{
        //copia las propiedades de configuración
        __q = Q.__q;
        __xmin = Q.__xmin;
        __xmax = Q.__xmax;
        //copia las propiedades derivadas
        __Qmin = Q.__Qmin;
        __Qmax = Q.__Qmax;
}
TQuantificator& TQuantificator::operator=(const TQuantificator& Q)
{
        //copia las propiedades de configuración
        __q = Q.__q;
        __xmin = Q.__xmin;
        __xmax = Q.__xmax;
        //copia las propiedades derivadas
        __Qmin = Q.__Qmin;
        __Qmax = Q.__Qmax;

        //devuelve una referencia a este cuantificador
        return (*this);
}
//construye un clon del cuantificador
TQuantificator::TQuantificator(const TQuantificator *Q)
{
        //el puntero Q deberí apauntar a un cuantificador contruido
        if(Q == NULL)
                throw EImproperArgument("pointer Q ahould point to built quantificator");

        //copia las propiedades del cuantificador
        Copy(Q);
}

//compara un cuantificador
bool TQuantificator::operator==(const TQuantificator& Q) const
{
        if(__q!=Q.__q)
                return false;
        if(__xmin!=Q.__xmin)
                return false;
        if(__xmax!=Q.__xmax)
                return false;
        return true;
}
bool TQuantificator::operator!=(const TQuantificator& Q) const
{
    if((*this)==Q)
                return false;
        return true;
}

//configura el cuantificador
void TQuantificator::Set(double _q, double _xmin, double _xmax)
{
        //el intervalo de cuantificación q debería ser distinto de cero
        if(_q == 0)
                throw EImproperArgument("quantification interval q should be nonnegative");

        //el límite superior del intervalo de cuantificación xmax no debe ser menor que el límite inferior xmin
        if(_xmax <= _xmin)
                throw EImproperArgument("upper limit quantification interval xmax should not below lower limit xmin");

        //asigna los valores
        __q = _q;
        __xmin = _xmin;
        __xmax = _xmax;

        //asimila los parámetros de configuración
        CalculateImageDomain();
}

//cuantifica un valor
int TQuantificator::Quantifice(double x) const
{
/*  	//limita el dominio de x a [xmin, xmax]
    if(x < xmin)
                x = xmin;
    else if(x > xmax)
                x = xmax;

        return round(x/q);
*/
        //traduce x a coordenadas de pasos y determian el centroide más próximo
        int n = Round(x/__q);

    if(n < __Qmin) //si el centroide rebasa el límite inferior
                return __Qmin; //devuelve el límit einferior

        //si no
    if(n > __Qmax) //si el centroide rebasa el límite superior
                return __Qmax; //devuelve el límite superior

        //si no
    return n; //devuelve el centroide
}
int TQuantificator::operator[](double x) const
{
/*  	//limita el dominio de x a [xmin, xmax]
    if(x < xmin)
                x = xmin;
    else if(x > xmax)
                x = xmax;

        return round(x/q);
*/
        //traduce x a coordenadas de pasos y determian el centroide más próximo
        int n = Round(x/__q);

    if(n < __Qmin) //si el centroide rebasa el límite inferior
                return __Qmin; //devuelve el límit einferior

        //si no
    if(n > __Qmax) //si el centroide rebasa el límite superior
                return __Qmax; //devuelve el límite superior

        //si no
    return n; //devuelve el centroide
}

//reconstruye un valor
double TQuantificator::Reconstruct(int n) const
{
    //el centroide n debe estar en el dominio imagen [Qmin, Qmax]
        if(n<__Qmin || __Qmax<n)
            throw EImproperArgument("centroid n should be in image domain [Qmin, Qmax]");

        //traduce el centroide a unidades de x y lo devuelve
        return n*__q;
}
double TQuantificator::operator[](int n) const
{
    //el centroide n debe estar en el dominio imagen [Qmin, Qmax]
        if(n<__Qmin || __Qmax<n)
            throw EImproperArgument("centroid n should be in image domain [Qmin, Qmax]");

        //traduce el centroide a unidades de x y lo devuelve
        return n*__q;
}

//---------------------------------------------------------------------------

} //namespace Operators

//---------------------------------------------------------------------------

