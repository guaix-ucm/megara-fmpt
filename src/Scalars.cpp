// Copyright (c) 2012-2016 Isaac Morales Durán. All rights reserved.
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
//Archivo: Scalars.cpp
//Contenido: funciones escalares
//Última actualización: 07/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Constants.h"
#include "Scalars.h"
#include "StrPR.h" //StrTravelLabel

#include <math.h> //funciones matemáticas variadas
#include <stdlib.h> //rand
#include <limits> //std::numeric_limits

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres de clases y funciones matemáticas
namespace Mathematics {

//---------------------------------------------------------------------------
//Retardo:
//---------------------------------------------------------------------------

//provoca un retardo de tiempo en clocks ocupando la CPU
//en SOs que no son de tiempo real 1 clock = 1 ms
void Delay(clock_t t)
{
    clock_t start_time;//, cur_time;
    start_time = clock();
    while((clock() - start_time) < t)
    {
    }

    //NOTA: el tipo clock_t representa un entero largo (long)
    //NOTA: la constante CLOCKS_PER_SEC no es usada aquí
    //pero tiene un valor de 1000
}

//---------------------------------------------------------------------------
//Functiones escalares:
//---------------------------------------------------------------------------

//n(n-1)(n-2)···1; 0!=1
int Factorial(int n)
{
    //el factorial de un nº negativo es indefinido
    if(n<0)
        throw EImproperArgument("negative n");

    if(n==0)
        return 1;
    else if(n<=2)
        return n;
    else {
        int acum = n;
        do {
            n--;
            acum = acum*(n);
        } while(n>2);
        return acum;
    }
}
//Binomio de newton
int Binomial(int m, int n)
{
    //m deberá ser mayor o igual que cero
    if(m < 0)
        throw EImproperArgument("m should be not negative");
    //n debe estar en [0, m]
    if(n<0 || m<n)
        throw EImproperArgument("n should be in [0, m]");

    return Factorial(m)/(Factorial(n)*Factorial(m-n));
}
//logaritmo en base a de x
double Log(double a, double x)
{
    //RECUERDE: el logaritmo en base real de un número negativo
    //es un número complejo y el logarimo de cero es menos infinito.

    //la base del logaritmo debe ser mayor que cero
    if(a <= 0)
        throw EImproperArgument("the algorithm base should be upper zero");

    //el argumento de la función logaritmo debe ser mayor que cero
    if(x <= 0)
        throw EImproperArgument("the argument of the function logarithm should be upper zero");

    //calcula y devuelve el logaritmo en base a de x
    return log(x)/log(a);
}
//Logaritmo en base dos
double Log2(double x)
{
    //RECUERDE: el logaritmo en base real de un número negativo
    //es un número complejo y el logarimo de cero es menos infinito.

    //el argumento de la función logaritmo en base 2 debe ser mayor que cero
    if(x <= 0)
        throw EImproperArgument("the argument of the function logarithm should be upper zero");

    //calcula y devuelve el logaritmo en base 2 de x
    return log(x)/log(2.);
}

//Devuelve el entero e más próximo tq [e-0.5<x<e+0.5)
int Round(float x)
{
    if(x-floor(x) <= 0.5)
        return floor(x);
    else
        return ceil(x);
}
//devuelve el entero más próximo al origen de coordenadas
//a una distancia inferior a la unidad del valor indicado
//      Near(x) = Floor(Abs(x))*Sign(x)
double Near(double x)
{
    return floor(Abs(x))*Sign(x);
}
//devuelve el entero más lejano al origen de coordenadas
//a una distancia inferior a la unidad del valor indicado
//      Far(x) = Ceil(Abs(x))*Sign(x)
double Far(double x)
{
    return ceil(Abs(x))*Sign(x);
}

//Genera un número real aleatorio con distribución uniforme en [0, 1]
double RandomReal(void)
{
    return double(rand())/double(RAND_MAX);
}
//Genera un número real aleatorio con distribución uniforme en [xmin, xmax]
double RandomUniform(double x1, double x2)
{
    return (x2 - x1)*RandomReal() + x1;
}
//Genera un número real aleatorio con distribución uniforme en [nmin, nmax]
int RandomUniform(int n1, int n2)
{
    int N = n2 - n1;
    return Sign(N)*random(Abs(N) + 1) + n1;
}
//desplaza theta al intervalo [0, M_2PI)
//una distancia múltiplo de M_2PI
double ArgPos(double theta)
{
    //si ya está dentro del intervalo no calcula nada
    //para evitar la instroducción de errores numéricos
    if(0<=theta && theta<M_2PI)
        return theta;

    //desplaza al intervalo principal
    theta -= floor(theta/M_2PI)*M_2PI;

    //corrige los errores numéricos limítrofes
    if(theta < 0)
        theta = 0;
    if(theta >= M_2PI)
        theta = M_2PI - incmin(M_2PI);

    //devuelve el valor desplazado
    return theta;
}

//determina si un tipo es numérico
bool Numerical(const type_info& t)
{
    if(t==typeid(char) || t==typeid(signed char) ||
            t==typeid(unsigned char) ||
            t==typeid(signed char) ||
            t==typeid(int) || t==typeid(signed int) ||
            t==typeid(unsigned) || t==typeid(unsigned int) ||
            t==typeid(short) || t==typeid(short int) ||
            t==typeid(signed short int) ||
            t==typeid(unsigned short) ||
            t==typeid(unsigned short int) ||
            t==typeid(long) || t==typeid(long int) ||
            t==typeid(signed long int) ||
            t==typeid(unsigned long) || t==typeid(unsigned long int) ||
            t==typeid(float) || t==typeid(double) || t==typeid(long double))
        return true;

    return false;
}
//determina si un tipo es de punto flotante
bool FloatingPoint(const type_info& t)
{
    if(t==typeid(float) || t==typeid(double) || t==typeid(long double))
        return true;

    return false;
}

//---------------------------------------------------------------------------
//Funciones de manipulación de tipos numéricos:

//determina la mantisa y el exponente en base decimal
//de un nº en en punto flotante
double frexp10(double x, int& exponent10)
//x: nº al que se va a calcular la mantisa y el exponente en base 10
//exponent10: exponente decimal
{
    //    int exponent2; //exponente binario
    double mantissa10;


    //retira el signo decimal
    double sign;
    if(x < 0) {
        x *= -1;
        sign = -1;
    } else
        sign = 1;

    if(x >= 1) {//si x[i] tiene parte entera
        //nº de cifras decimales enteras
        exponent10 = ceil(log10(floor(x)+1));
        mantissa10 = x/pow(10, double(exponent10));
    } else if(0<x && x<0.1) {
        //calcula el exponente
        exponent10 = floor(log10(x)) + 1;
        //calcula la mantisa
        mantissa10 = x*pow(10, double(-exponent10));
    } else {
        exponent10 = 0;
        mantissa10 = x;
    }

    mantissa10 *= sign; //restaura el signo

    return mantissa10;
}
//determina el nº de cifras decimales significativas de x
int nsigfig(double x)
{
    AnsiString S = FloatToStr(x);
    int n; //nº de cifras significativas
    bool point; //si hay un punto decimal
    int first; //índice a la primera cifra entre 1 y 9
    int last; //índice a la última cifra entre 1 y 9 antes de la E

    //Busca la primera cifra significativa y un punto decimal
    first = 1;
    while(S[first]<'1' || '9'<S[first])
        first++;

    last = S.Length();

    //Busca una E en las cuatro penúltimas posiciones
    //El éxponente más largo corresponde a -324
    if(S[last-1]=='E')
        last = last-2;
    else if(S[last-2]=='E')
        last = last-3;
    else if(S[last-3]=='E')
        last = last-4;
    else if(S[last-4]=='E')
        last = last-5;
    //Descarta los ceros del final
    while(S[last]=='0')
        last--;
    //Busca un punto decimal
    int i=first;
    point = false;
    do {
        i++;
        if(S[i] == '.')
            point = true;
    } while(!point && i<S.Length());

    n = last-first;
    if(!point)
        n++;

    return n;
}
//determina el incremento mínimo de un double
//correspondiente a incrementar su mantisa una unidad
double incmin(double x)
{
    //determina el exponente de x
    int xexp;
    frexp(x, &xexp);

    //ahora xman es un double cuyo primer decimal corresponde
    //a la cifra significativa de más peso

    //incremento mínimo de la mantisa de x
    //normalmente la mantisa de un double tiene DSIGNIF+1=54 bits
    //y su incremento mínimo corresponde a:
    //      1/pow(2, 54) = 5.55111512312578E-17

    //calcula el incremento mínimo de x
    return pow(2, double(xexp))/pow(2, double(std::numeric_limits<double>::digits+1));
}

//---------------------------------------------------------------------------
/*#
//extrae las componentes RGB de un color
void ColorToRGB(unsigned char &R, unsigned char &G, unsigned char &B,
                QColor Color)
{
    //separa las componentes del color
    //R = (unsigned char)(int(Color) & 0x000000FF);
    //G = (unsigned char)((int(Color) & 0x0000FF00) >>8);
    //B = (unsigned char)((int(Color) & 0x00FF0000) >>16);

    int r, g, b;
    Color.getRgb(&r, &g, &b);
    R = (unsigned char)r;
    G = (unsigned char)g;
    B = (unsigned char)b;
}
//asigna las compnentes RGB a un color
void RGBToColor(QColor &Color,
                unsigned char R, unsigned char G, unsigned char B)
{
        //asigna las componentes a un entero
        //int color = (int(B) <<16);
        //color += (int(G) <<8);
        //color += int(R);

        //asigna el entero al color
        //Color = QColor(color);
    Color.setRgb(int(R), int(G), int(B));
}

//determina la luminancia de un color L = R+G+B
int Luminance(QColor Color)
{
    //descompone el color en sus componentes
    unsigned char R, G, B;
    ColorToRGB(R, G, B, Color);

    //suma las componentes
    return int(R) + int(G) + int(B);
}

//multiplica las coomponentes de un color por un factor de escala
//si alguna de las componentes resultantes excede de 0xFF le asigna 0xFF
QColor HighlightColor(QColor Color, double f)
{
    //el factor de escala f debería ser no negativo
    if(f < 0)
        throw EImproperArgument("scale factor f should be nonnegative");

    //extrae las componentes del color
    unsigned char R, G, B;
    ColorToRGB(R, G, B, Color);

    //multiplica las componentes por el factor de escala
    double r = double(R)*f;
    double g = double(G)*f;
    double b = double(B)*f;

    //limita las componentes al intervalo [0, 255]
    if(r > 255)
        r = 255;
    else if(r < 0)
        r = 0;
    if(g > 255)
        g = 255;
    else if(g < 0)
        g = 0;
    if(b > 255)
        b = 255;
    else if(b < 0)
        b = 0;

    //asigna las nuevas componentes
    R = (unsigned char)Round(r);
    G = (unsigned char)Round(g);
    B = (unsigned char)Round(b);

    //asigna las componentes al color resaltado
    QColor HColor;
    RGBToColor(HColor, R, G, B);

    return HColor;
}

//resalta un color al máximo manteniendo las proporciones entre sus componentes
QColor HighlightColor(QColor Color)
{
    //separa las componentes del color
    unsigned char R;
    unsigned char G;
    unsigned char B;

    //extrae las componentes
    ColorToRGB(R, G, B, Color);

    //determina la componente máxima
    unsigned char max = Max(R, G);
    max = Max(max, B);

    //determina el factor de escala
    double f = double(255)/double(max);

    //multiplica las componentes por el factor de escala
    double r = double(R)*f;
    double g = double(G)*f;
    double b = double(B)*f;

    //asigna las nuevas componentes
    R = (unsigned char)Round(r);
    G = (unsigned char)Round(g);
    B = (unsigned char)Round(b);

    //asigna las componentes
    RGBToColor(Color, R, G, B);

    return Color;
}

//obtiene el color complementario de un color
QColor ComplementaryColor(QColor Color)
{
    //separa las componentes del color
    unsigned char R;
    unsigned char G;
    unsigned char B;

    //extrae las componentes
    ColorToRGB(R, G, B, Color);

    //invierte las componentes
    R = 0xFF - R;
    G = 0xFF - G;
    B = 0xFF - B;

    //asigna las componentes
    QColor CC;
    RGBToColor(CC, R, G, B);

    return CC;
}
*/
//---------------------------------------------------------------------------
//TIdDouble:
//---------------------------------------------------------------------------

//PROPIEDADES DE LA VARIABLE IDENTIFICADA:

void TIdDouble::setId(const int Id)
{
    //el número de identificación Id debería ser no negativo
    if(Id < 0) {
        throw EImproperArgument(AnsiString("identification number '")+IntToStr(Id)+AnsiString("' should be nonnegative"));
    }

    //asigna el nuevo valor
    p_Id = Id;
}

//PROPIEDADES EN FORMATO TEXTO:

AnsiString TIdDouble::getIdText(void) const
{
    return IntToStr(p_Id);
}
void TIdDouble::setIdText(const AnsiString& S)
{
    try {
        p_Id = StrToInt_(S);
    } catch(...) {
        throw;
    }
}
AnsiString TIdDouble::getValueText(void) const
{
    return FloatToStr(Value);
}
void TIdDouble::setValueText(const AnsiString& S)
{
    try {
        Value = StrToFloat_(S);
    } catch(...) {
        throw;
    }
}
AnsiString TIdDouble::getText(void) const
{
    return AnsiString("(")+getIdText()+AnsiString(": ")+getValueText()+AnsiString(")");
}
void TIdDouble::setText(const AnsiString &S)
{
    try {
        int i = 1;
        Read(this, S, i);
    } catch(...) {
        throw;
    }
}

//MÉTODOS ESTÁTICOS PARA LISTAS:

//compara dos elementos
int  TIdDouble::Compare(TIdDouble ID1, TIdDouble ID2)
{
    if(ID1.Value < ID2.Value)
        return -1;

    if(ID1.Value > ID2.Value)
        return 1;

    return 0;
}

//evalúa un elemento
double  TIdDouble::Evaluate(TIdDouble ID)
{
    return ID.Value;
}

//asigna un valor a un elemento
void  TIdDouble::Assign(TIdDouble &ID, double Value)
{
    ID.Value = Value;
}

//imprime un elemento en una cadena
void  TIdDouble::Print(AnsiString &S, TIdDouble ID)
{
    S += AnsiString("(")+IntToStr(ID.getId())+AnsiString(": ")+FloatToStr(ID.Value)+AnsiString(")");
}

//escudriña una cadena en busca de un elemento
void  TIdDouble::Read(TIdDouble &ID, const AnsiString &S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería apauntar a un caracter de la cadena de texto S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("identified double not found");

    //estado de la máquian de estados
    //      0: esperando ' ', '\r' o '('
    //      1: esperando ' ', '\r' o identificador (Id)
    //      2: esperando ' ', '\r' o ':'
    //      3: esperando ' ', '\r' o double (x)
    //      4: esperando ' ', '\r' o ')'
    //      5: double identificado leido con éxito
    int status = 0;

    //variables auxiliares
    TIdDouble t_ID; //variable tampón
    int aux; //variable local para pasar por referencia en vez de t_ID.Id

    do {
        char c = S[i];

        switch(status) {
        case 0: //esperando ' ', '\r' o '('
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("'(' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("'(' not found");
                break;
            case '(':
                i++;
                status++;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ' ' or '('"));
            }
            break;
        case 1: //esperando ' ', '\r' o identificador (Id)
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("number identifier Id not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("number identifier Id not found");
                break;
            default:
                StrReadInt(aux, S, i);
                t_ID.setId(aux);
                status = 2;
            }
            break;
        case 2: //esperando ' ', '\r' o ':'
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("':' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("':' not found");
                break;
            case ':':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("value Value not found");
                status = 3;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ' ' or ':'"));
            }
            break;
        case 3: //esperando ' ', '\r' o double (x)
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("value 'Value' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("value 'Value' not found");
                break;
            default:
                StrReadFloat(t_ID.Value, S, i);
                status = 4;
            }
            break;
        case 4: //esperando ' ', '\r' o ')'
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("'}' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("'}' not found");
                break;
            case ')':
                i++;
                status = 5;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ' ', ')' or '")+AnsiString(get_decimal_separator())+AnsiString("' if missing"));

                //NOTA:
                //sería posible determinar si el número de punto flotante contiene un punto decimal o no
                //sustituyento:
                //      StrReadFloat(ID.Value, S, i);
                //por:
                //      try {
                //              //segrega un punto flotante a aprtir de i
                //              StrReadFloatStr(ValueStr, S, i);
                //
                //              //intenta convertir a entero
                //              x = StrToFloat(ValueStr);
                //
                //              //captura el efecto lateral de la función de conversión
                //              double aux = 0; aux + aux;
                //
                //      } catch(EOverflow &E) {
                //              throw EImproperArgument(AnsiString("'")+F+"' is a floating point value out of range");
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

    //asigna la variable tampón
    ID = t_ID;
}

//MÉTODOS ESTÁTICOS PARA LISTAS DE PUNTEROS:

//compara dos elementos
int  TIdDouble::Compare(const TIdDouble *ID1, const TIdDouble *ID2)
{
    //el puntero ID1 debería apuntar a un double identificado construido
    if(ID1 == NULL)
        throw EImproperArgument("pinter ID1 should point to built identified double");
    //el puntero ID2 debería apuntar a un double identificado construido
    if(ID2 == NULL)
        throw EImproperArgument("pinter ID2 should point to built identified double");

    if(ID1->Value < ID2->Value)
        return -1;
    if(ID1->Value > ID2->Value)
        return 1;
    return 0;
}

//evalúa un elemento
double  TIdDouble::Evaluate(const TIdDouble *ID)
{
    //el puntero ID debería apuntar a un double identificado construido
    if(ID == NULL)
        throw EImproperArgument("pinter ID should point to built identified double");

    return ID->Value;
}

//asigna un valor a un elemento
void  TIdDouble::Assign(TIdDouble *ID, double Value)
{
    //el puntero ID debería apuntar a un double identificado construido
    if(ID == NULL)
        throw EImproperArgument("pinter ID should point to built identified double");

    ID->Value = Value;
}

//imprime un elemento en una cadena
void  TIdDouble::Print(AnsiString &S, const TIdDouble *ID)
{
    //el puntero ID debería apuntar a un double identificado construido
    if(ID == NULL)
        throw EImproperArgument("pinter ID should point to built identified double");

    S += AnsiString("(")+IntToStr(ID->getId())+AnsiString(": ")+FloatToStr(ID->Value)+AnsiString(")");
}

//escudriña una cadena en busca de un elemento
void  TIdDouble::Read(TIdDouble *ID, const AnsiString &S, int &i)
{
    //el puntero ID debería apuntar a un double identificado construido
    if(ID == NULL)
        throw EImproperArgument("pinter ID should point to built identified double");

    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caraacter por defecto.

    //el índice i debería apauntar a un caracter de la cadena de texto S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("index i should indicate an character in the string S");

    //estado de la máquian de estados
    //      0: esperando ' ', '\r' o '('
    //      1: esperando ' ', '\r' o identificador (Id)
    //      2: esperando ' ', '\r' o ':'
    //      3: esperando ' ', '\r' o double (x)
    //      4: esperando ' ', '\r' o ')'
    //      5: elemento convertido con éxito
    int status = 0;

    //variables auxiliares
    TIdDouble t_ID; //variable tampón
    int aux; //variable local para pasar por referencia en vez de t_ID.Id

    do {
        char c = S[i];

        switch(status) {
        case 0: //esperando ' ', '\r' o '('
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("'{' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("'{' not found");
                break;
            case '(':
                i++;
                status = 1;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ' ' or '('"));
            }
            break;
        case 1: //esperando ' ', '\r' o identificador (Id)
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("number identifier Id not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("number identifier Id not found");
                break;
            default:
                StrReadInt(aux, S, i);
                t_ID.setId(aux);
                status = 2;
            }
            break;
        case 2: //esperando ' ', '\r' o ':'
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("':' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("':' not found");
                break;
            case ':':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("':' not found");
                status = 3;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ' ' or ':'"));
            }
            break;
        case 3: //esperando ' ', '\r' o double (x)
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("value Value not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("value Value not found");
                break;
            default:
                StrReadFloat(t_ID.Value, S, i);
                status = 4;
            }
            break;
        case 4: //esperando ' ', '\r' o ')'
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("'}' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("'}' not found");
                break;
            case ')':
                i++;
                status = 5;
                break;
            default:
                throw EImproperArgument(AnsiString("character '")+AnsiString(c)+AnsiString("' should be ' ', ')' or '")+AnsiString(get_decimal_separator())+AnsiString("' if missing"));

                //NOTA:
                //sería posible determinar si el número de punto flotante contiene un punto decimal o no
                //sustituyento:
                //      StrReadFloat(ID.Value, S, i);
                //por:
                //      try {
                //              //segrega un punto flotante a aprtir de i
                //              StrReadFloatStr(ValueStr, S, i);
                //
                //              //intenta convertir a entero
                //              x = StrToFloat(ValueStr);
                //
                //              //captura el efecto lateral de la función de conversión
                //              double aux = 0; aux + aux;
                //
                //      } catch(EOverflow &E) {
                //              throw EImproperArgument(AnsiString("'")+F+"' is a floating point value out of range");
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

    //asigna la variable tampón
    *(ID) = t_ID;
}

//---------------------------------------------------------------------------
//MÉTODOS PÚBLICOS:

//copia un elemento
TIdDouble &TIdDouble::operator=(const TIdDouble &ID)
{
    p_Id = ID.p_Id;
    Value = ID.Value;
    return *this;
}

//---------------------------------------------------------------------------
//TInterval:
//---------------------------------------------------------------------------

void TInterval::setlmin(double lmin)
{
    //el límite inferior del intervalo lmin debe ser menor o igual que el límite superior
    if(lmin > p_lmax)
        throw EImproperArgument("interval lowest limit lmin should be less or equal to upper limit");

    //asigna el nuevo valor
    p_lmin = lmin;
}
void TInterval::setlmax(double lmax)
{
    //el límite superior del intervalo lmax debe ser mayor o igual que el límite inferior
    if(lmax > p_lmin)
        throw EImproperArgument("interval upper limit lmin should be upper or equal to lower limit");

    //asigna el nuevo valor
    p_lmax = lmax;
}

AnsiString TInterval::getText(void)
{
    AnsiString S;
    Print(S, this);
    return S;
}
void TInterval::setText(const AnsiString &S)
{
    try {
        int i = 1;
        Read(this, S, i);
    } catch(...) {
        throw;
    }
}

//MÉTODOS ESTÁTICOS:

//imprime un intervalo en una cadena de texto
void  TInterval::Print(AnsiString &S, const TInterval *I)
{
    //el puntero I debería apuntar a un intervalo construido
    if(I == NULL)
        throw EImproperArgument("pointer I should point to built interval");

    switch(I->IntervalType) {
    case itOpen: S += "("; break;
    case itLeft: S += "["; break;
    case itRight: S += "("; break;
    case itClose: S += "["; break;
    }

    S += FloatToStr(I->getlmin())+AnsiString(", ")+FloatToStr(I->getlmax());

    switch(I->IntervalType) {
    case itOpen: S += ")"; break;
    case itLeft: S += ")"; break;
    case itRight: S += "]"; break;
    case itClose: S += "]"; break;
    }
}
//escanea una cadena de texto en busca de un intervalo
void  TInterval::Read(TInterval *I, const AnsiString &S, int &i)
{
    //el puntero I debería apuntar a un intervalo construido
    if(I == NULL)
        throw EImproperArgument("pointer I should point to built interval");

    //estado de lectura:
    //      0: esperando ' ', '\r' o '(' o '['
    //      1: esperando ' ', '\r' o double lmin
    //      2: esperando ' ', '\r' o ','
    //      3: esperando ' ', '\r' o double lmax
    //      4: esperando ' ', '\r' o ')' o ']'
    //      5: intervalo leido con éxito
    int status = 0;

    //caracter indicado S[i]
    char c;

    //variables tampón en formato conveniente
    char dl; //delimitador izquierdo
    char dr; //delimitador derecho
    double lmin; //límite inferior
    double lmax; //límite superior

    do {
        c = S[i]; //asigna el próximo caracter

        //reacciona según el estado y al caracter
        switch(status) {
        case 0: //esperando ' ', '\r' o '(' o '['
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("'(' or '[' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("'(' or '[' not found");
                break;
            case '(': case '[':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("lower limit lmin not found");
                dl = c;
                status = 1;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be '(' or '['"));
            }
            break;
        case 1: //esperando ' ', '\r' o double lmin
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("lower limit lmin not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("lower limit lmin not found");
                break;
            default:
                try {
                StrReadFloat(lmin, S, i);
            } catch(...) {
                throw;
            }
                status = 2;
            }
            break;
        case 2: //esperando ' ', '\r' o ','
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("',' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("',' not found");
                break;
            case ',':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("upper limit lmax not found");
                status = 3;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ','"));
            }
            break;
        case 3: //esperando ' ', '\r' o double lmax
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("upper limit lmax not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("upper limit lmax not found");
                break;
            default:
                try {
                StrReadFloat(lmax, S, i);
            } catch(...) {
                throw;
            }
                status = 4;
            }
            break;
        case 4: //esperando ' ', '\r' o ')' o ']'
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("')' or ']' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("')' or ']' not found");
                break;
            case ')': case ']':
                i++;
                dr = c;
                status = 5;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ')' or ']'"));
            }
            break;
        }
    } while(status < 5);

    //asigna las propiedades
    try {
        I->SetLimits(lmin, lmax);

        if(dl=='(' && dr==')') {
            I->IntervalType = itOpen;
        } else if(dl=='[' && dr==')') {
            I->IntervalType = itLeft;
        } else if(dl=='(' && dr==']') {
            I->IntervalType = itRight;
        } else {
            I->IntervalType = itClose;
        }

    } catch(...) {
        throw;
    }
}

//MÉTODOS PÚBLICOS:

//construye un intervalo
TInterval::TInterval(TIntervalType t_IntervalType,
                     double lmin, double lmax) : IntervalType(t_IntervalType)
{
    //el límite superior lmax debería ser mayor o igual que el límite inferior lmin
    if(lmax < lmin)
        throw EImproperArgument("upper limit lmax should be upper or equal lower limit lmin");

    //inicializa los valores
    p_lmin = lmin;
    p_lmax = lmax;
}

//asigna los límites del intervalo simultaneamente
void TInterval::SetLimits(double lmin, double lmax)
{
    //el límite superior lmax debería ser mayor o igual que el límite inferior lmin
    if(lmax < lmin)
        throw EImproperArgument("upper limit lmax should be upper or equal lower limit lmin");

    //asigna los nuevos valores
    p_lmin = lmin;
    p_lmax = lmax;
}
/*
//---------------------------------------------------------------------------
//TPairIntegers
//---------------------------------------------------------------------------

//PROPIEDADES EN FORMATO TEXTO:

AnsiString TPairIntegers::GetxText(void) const
{
        return IntToStr(x);
}
void TPairIntegers::SetxText(const AnsiString &S)
{
        try {
                x = StrToInt_(S);
        } catch(...) {
                throw;
        }
}
AnsiString TPairIntegers::GetyText(void) const
{
        return IntToStr(y);
}
void TPairIntegers::SetyText(const AnsiString &S)
{
        try {
                y = StrToInt_(S);
        } catch(...) {
                throw;
        }
}

//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TPairIntegers::GetText(void) const
{
        AnsiString S;

        S = "("+xText+", "+yText+")";

        return S;
}
void TPairIntegers::SetText(const AnsiString &S)
{
        try {
                int i = 1;
                Read((TPairIntegers*)this, S, i);
        } catch(...) {
                throw;
        }
}
AnsiString TPairIntegers::GetRowText(void) const
{
        AnsiString S;

        //NOTA: 0x09 corresponde al caracter tabulador horizontal

        S = xText+char(0x09)+yText;

        return S;
}
void TPairIntegers::SetRowText(const AnsiString &S)
{
        try {
                int i = 1;
                ReadRow((TPairIntegers*)this, S, i);
        } catch(...) {
                throw;
        }
}

//FUNCIONES ESTÁTICAS:

//función de impresión de elementos tipo TPairIntegers
void  TPairIntegers::Print(AnsiString &S, const TPairIntegers *PI)
{
        //el puntero PI debe apuntar a un par de enteros contruido
        if(PI == NULL)
                throw EImproperArgument("pointer PI should point to built integer pair");

        S += PI->Text;
}
//función de lectura de elementos tipo TPairIntegers
void  TPairIntegers::Read(TPairIntegers *PI, const AnsiString &S, int &i)
{
        //FALTA
}

//función de impresión de elementos tipo TPairIntegers
void  TPairIntegers::PrintRow(AnsiString &S, const TPairIntegers *PI)
{
        //el puntero PI debe apuntar a un par de enteros contruido
        if(PI == NULL)
                throw EImproperArgument("pointer PI should point to built integer pair");

        S += PI->RowText;
}
//función de lectura de elementos tipo TPairIntegers
void  TPairIntegers::ReadRow(TPairIntegers *PI, const AnsiString &S, int &i)
{
        //FALTA
}

//MÉTODOS PÚBLICOS:

//construye un par de enteros por defecto
TPairIntegers::TPairIntegers(void) : x(0), y(0)
{
}
//contruye un par de enteros con los valores indicados
TPairIntegers::TPairIntegers(int t_x, int t_y) : x(t_x), y(t_y)
{
}

//copia las propiedades de un par
TPairIntegers& TPairIntegers::operator=(const TPairIntegers &PI)
{
        //copia las propiedades
        x = PI.x;
        y = PI.y;

        //devuelve una referencia a este objeto para permitir anidar las asignaciones
        return *this;
}
*/
//---------------------------------------------------------------------------
//TPair:
//---------------------------------------------------------------------------

AnsiString TPair::getText(void)
{
    AnsiString S;
    Print(S, this);
    return S;
}
void TPair::setText(const AnsiString &S)
{
    try {
        int i = 1;
        Read(this, S, i);
    } catch(...) {
        throw;
    }
}

//MÉTODOS ESTÁTICOS:

//imprime un par en una cadena de texto
void  TPair::Print(AnsiString &S, const TPair *P)
{
    //el puntero P debería apuntar a un par construido
    if(P == NULL)
        throw EImproperArgument("pointer P should point to built pair");

    S += "(";
    S += FloatToStr(P->x)+AnsiString(", ")+FloatToStr(P->y);
    S += ")";
}
//escanea una cadena de texto en busca de un par
void  TPair::Read(TPair *P, const AnsiString &S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el puntero P debería apuntar a un par construido
    if(P == NULL)
        throw EImproperArgument("pointer P should point to built pair");

    //el índice i debería apauntar a un caracter de la cadena de texto S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("identified double not found");

    //estado de lectura:
    //      0: esperando ' ', '\r' o '('
    //      1: esperando ' ', '\r' o double x
    //      2: esperando ' ', '\r' o ','
    //      3: esperando ' ', '\r' o double y
    //      4: esperando ' ', '\r' o ')'
    //      5: par leido con éxito
    int status = 0;

    //caracter indicado S[i]
    char c;

    //variables tampón en formato conveniente
    double t_x; //límite inferior
    double t_y; //límite superior

    do {
        c = S[i]; //asigna el próximo caracter

        //reacciona según el estado y al caracter
        switch(status) {
        case 0: //esperando ' ', '\r' o '('
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("'(' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("'(' not found");
                break;
            case '(':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("first value x not found");
                status = 1;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be '('"));
            }
            break;
        case 1: //esperando ' ', '\r' o double x
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("first value x not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("first value x not found");
                break;
            default:
                try {
                StrReadFloat(t_x, S, i);
            } catch(...) {
                throw;
            }
                status = 2;
            }
            break;
        case 2: //esperando ' ', '\r' o ','
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("',' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("',' not found");
                break;
            case ',':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("second value y not found");
                status = 3;
                break;
            default:
                throw EImproperArgument("',' not found");
            }
            break;
        case 3: //esperando ' ', '\r' o double y
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("second value y not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("second value y not found");
                break;
            default:
                try {
                StrReadFloat(t_y, S, i);
            } catch(...) {
                throw;
            }
                status = 4;
            }
            break;
        case 4: //esperando ' ', '\r' o ')'
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("')' not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("')' not found");
                break;
            case ')':
                i++;
                status = 5;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ')'"));
            }
            break;
        }
    } while(status < 5);

    //asigna las variables tampón
    try {
        P->SetValues(t_x, t_y);
    } catch(...) {
        throw;
    }
}

//MÉTODOS PÚBLICOS:

//construye un par
TPair::TPair(double t_x, double t_y) : x(t_x), y(t_y)
{
}

//asigna simultaneamente los valores al par
void TPair::SetValues(double t_x, double t_y)
{
    //asigna los nuevos valores
    x = t_x;
    y = t_y;
}
/*comentado para evitar tener que traducirlo
//---------------------------------------------------------------------------
//TPairIB
//---------------------------------------------------------------------------
//INICIALIZA LAS PROPIEDADES ESTÁTICAS:

AnsiString TPairIB::xLabel = "x";
AnsiString TPairIB::yLabel = "y";

//---------------------------------------------------------------------------
//PROPIEDADES EN FORMATO TEXTO:

AnsiString TPairIB::GetxText(void) const
{
        return IntToStr(x);
}
void TPairIB::SetxText(const AnsiString &S)
{
        try {
                //traduce y asigna el nuevo valor
                x = StrToInt_(S);

        } catch(...) {
                throw;
        }
}
AnsiString TPairIB::GetyText(void) const
{
        return BoolToStr_(y, true);
}
void TPairIB::SetyText(const AnsiString &S)
{
        try {
                //traduce y asigna el nuevo valor
                y = StrToBool_(S);

        } catch(...) {
                throw;
        }
}

//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TPairIB::GetText(void) const
{
        AnsiString S;

        S = "("+xText+", "+yText+")";

        return S;
}
void TPairIB::SetText(const AnsiString &S)
{
        try {
                int i = 1;
                TPairIB PIB;
                Read(&PIB, S, i);

                //ADVERTENCIA: aunque el método Read() es atómico, conviene
                //usar una variable tampón ya que hay que comprobar que
                //la cadena contenga solo un valor para un par
                //antes de asignar dicho valor.

                //avanza el índice i hasta la próxima posición que no contenga un separador
                StrTravelSeparators(S, i);
                //si el índice i indica a algún caracter de la cadena S
                if(i <= S.Length())
                        //indica que la cadena S solo debería contener el valor para un par GetLabels()
                        throw EImproperArgument("string S should contain a value to pair "+GetLabels()+" only");

                //copia lavariable tampón
                *this = PIB;

        } catch(...) {
                throw;
        }
}
AnsiString TPairIB::GetRowText(void) const
{
        AnsiString S;

        S = xText+'\t';
        S += yText;

        return S;
}
void TPairIB::SetRowText(const AnsiString &S)
{
        try {
                int i = 1;
                TPairIB PIB;
                ReadRow(&PIB, S, i);

                //ADVERTENCIA: aunque el método Read() es atómico, conviene
                //usar una variable tampón ya que hay que comprobar que
                //la cadena contenga solo un valor para un par
                //antes de asignar dicho valor.

                //avanza el índice i hasta la próxima posición que no contenga un separador
                StrTravelSeparators(S, i);
                //si el índice i indica a algún caracter de la cadena S
                if(i <= S.Length())
                        //indica que la cadena S solo debería contener el valor para un par GetLabels()
                        throw EImproperArgument("string S should contain a value to pair "+GetLabels()+" only");

                //copia la variable tampón
                *this = PIB;

        } catch(...) {
                throw;
        }
}

//FUNCIONES ESTÁTICAS DE COMPARACIÓN:

//compara la variable x
//      si P1->x < P2->x devuelve -1
//      si P1->x > P2->x devuelve 1
//      si P1->x == P2->x devuelve 0
int  TPairIB::Comparex(const TPairIB *P1, const TPairIB *P2)
{
        //el puntro P1 debería apuntar a un par GetLabels() contruido
        if(P1 == NULL)
                throw EImproperArgument("pointer P1 shpuld point to built pair "+GetLabels());
        //el puntro P2 debería apuntar a un par GetLabels() contruido
        if(P2 == NULL)
                throw EImproperArgument("pointer P2 shpuld point to built pair "+GetLabels());

        if(P1->x < P2->x)
                return -1;
        else if(P1->x > P2->x)
                return 1;
        else
                return 0;
}

//FUNCIONES ESTÁTICAS DE CONJUNTOS DE PROPIEDADES:

//imprime los valores de las propiedades de la clase
//en formato texto entre paréntesis
void  TPairIB::Print(AnsiString &S, const TPairIB *P)
{
        //el puntero P debe apuntar a un par GetLabels()
        if(P == NULL)
                throw EImproperArgument("pointer P should point to built pair "+GetLabels());

        S += P->Text;
}
//lee los valores de las etiquetas de las propiedades de la clase
//en formato textoentre paréntesis
void  TPairIB::Read(TPairIB *P, const AnsiString &S, int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el puntero P  debería apuntar a un par GetLabels() construido
        if(P == NULL)
                throw EImproperArgument("pointer P should point to built pair "+GetLabels());

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de lectura:
        //      0: esperando "("
        //      1: esperando valor para x
        //      2: esperando ","
        //      3: esperando valor para y
        //      4: esperando ")"
        //      5: par leido con éxito
        int status = 0;

        //variables tampón en formato conveniente
        TPairIB t_P;

        do {
                //reacciona según el estado y el caracter
                switch(status) {
                        case 0: //esperando "("
                                try {
                                        StrTravelLabel("(", S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 1: //esperando valor para x
                                try {
                                        int aux;
                                        StrReadInt(aux, S, i);
                                        t_P.x = aux;
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 2: //esperando ","
                                try {
                                        StrTravelLabel(",", S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 3: //esperando valor para y
                                try {
                                        bool aux;
                                        StrReadBool(aux, S, i);
                                        t_P.y = aux;
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 4: //esperando ")"
                                try {
                                        StrTravelLabel(")", S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;
                }
        //mientras no se haya leido el par con éxito
        } while(status < 5);

        //copia la variable tampón
        try {
                *P = t_P;
        } catch(...) {
                throw;
        }
}

//imprime los valores de las propiedades de la clase
//en formato fila de texto
void  TPairIB::PrintRow(AnsiString &S, const TPairIB *P)
{
        //el puntero P debe apuntar a un par GetLabels() construido
        if(P == NULL)
                throw EImproperArgument("pointer P should point to built pair "+GetLabels());

        S += P->RowText;
}
//lee los valores de las etiquetas de las propiedades de la clase
//en formato fila de texto
void  TPairIB::ReadRow(TPairIB *P, const AnsiString &S, int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el puntero P  debería apuntar a un par GetLabels() construido
        if(P == NULL)
                throw EImproperArgument("pointer P should point to built pair "+GetLabels());

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de lectura:
        //      0: esperando valor para x
        //      1: esperando separador
        //      2: esperando valor para y
        //      3: par leido con éxito
        int status = 0;

        //variables tampón en formato conveniente
        TPairIB t_P;

        do {
                //reacciona según el estado y al caracter
                switch(status) {
                        case 0: //esperando valor para x
                                try {
                                        int aux;
                                        StrReadInt(aux, S, i);
                                        t_P.x = aux;
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 1: //esperando separador
                                try {
                                        StrTravelSeparators(S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 2: //esperando valor para y
                                try {
                                        bool aux;
                                        StrReadBool(aux, S, i);
                                        t_P.y = aux;
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;
                }
        //mientras no se haya leido el par con éxito
        } while(status < 3);

        //copia la variable tampón
        try {
                *P = t_P;
        } catch(...) {
                throw;
        }
}

//obtiene las etiquetas de las propiedades de la clase
//en formato texto entre paréntesis
AnsiString TPairIB::GetLabels(void)
{
        AnsiString S;

        S = "("+xLabel+", "+yLabel+")";

        return S;
}
//obtiene las etiquetas de las propiedades de la clase
//en formato fila de texto
AnsiString TPairIB::GetRowLabels(void)
{
        AnsiString S;

        S = xLabel+'\t';
        S += yLabel;

        return S;
}
//atraviesa las etiquetas de la clase
//en formato fila de texto
void  TPairIB::TravelRowLabels(const AnsiString &S,
        int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de lectura:
        //      0: esperando xLabel
        //      1: esperando separador
        //      2: esperando yLabel
        //      3: etiquetas de las propiedades de recorridas con éxito
        int status = 0;

        do {
                //reacciona según el estado y al caracter
                switch(status) {

                        case 0: //esperando xLabel
                                try {
                                        StrTravelLabel(xLabel, S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 1: //esperando separador
                                try {
                                        StrTravelSeparators(S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 2: //esperando yLabel
                                try {
                                        StrTravelLabel(yLabel, S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;
                }
        //mientras las etiquetas de las propiedades no hayan sido recorridas con éxito
        } while(status < 3);
}

//MÉTODOS PÚBLICOS:

//construye un objeto de la clase
//con los valores por defecto
TPairIB::TPairIB(void) : x(0), y(0.)
{
}
//construye un objeto de la clase
//con los valores indicados
TPairIB::TPairIB(int t_x, int t_y) : x(t_x), y(t_y)
{
}

//copia las propiedades de un objeto de la clase
TPairIB& TPairIB::operator=(const TPairIB &PIB)
{
        //copia las propiedades
        x = PIB.x;
        y = PIB.y;

        //devuelve una referencia a este objeto
        //para permitir anidar las asignaciones
        return *this;
}

//---------------------------------------------------------------------------
//TPairID
//---------------------------------------------------------------------------
//INICIALIZA LAS PROPIEDADES ESTÁTICAS:

AnsiString TPairID::xLabel = "x";
AnsiString TPairID::yLabel = "y";

//---------------------------------------------------------------------------
//PROPIEDADES EN FORMATO TEXTO:

AnsiString TPairID::GetxText(void) const
{
        return IntToStr(x);
}
void TPairID::SetxText(const AnsiString &S)
{
        try {
                //traduce y asigna el nuevo valor
                x = StrToInt_(S);

        } catch(...) {
                throw;
        }
}
AnsiString TPairID::GetyText(void) const
{
        return FloatToStr(y);
}
void TPairID::SetyText(const AnsiString &S)
{
        try {
                //traduce y asigna el nuevo valor
                y = StrToFloat_(S);

        } catch(...) {
                throw;
        }
}

//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TPairID::GetText(void) const
{
        AnsiString S;

        S = "("+xText+", "+yText+")";

        return S;
}
void TPairID::SetText(const AnsiString &S)
{
        try {
                int i = 1;
                TPairID PID;
                Read(&PID, S, i);

                //ADVERTENCIA: aunque el método Read() es atómico, conviene
                //usar una variable tampón ya que hay que comprobar que
                //la cadena contenga solo un valor para un par
                //antes de asignar dicho valor.

                //avanza el índice i hasta la próxima posición que no contenga un separador
                StrTravelSeparators(S, i);
                //si el índice i indica a algún caracter de la cadena S
                if(i <= S.Length())
                        //indica que la cadena S solo debería contener el valor para un par GetLabels()
                        throw EImproperArgument("string S should contain a value to pair "+GetLabels()+" only");

                //asigna el nuevo valor
                *this = PID;

        } catch(...) {
                throw;
        }
}
AnsiString TPairID::GetRowText(void) const
{
        AnsiString S;

        S = xText+'\t';
        S += yText;

        return S;
}
void TPairID::SetRowText(const AnsiString &S)
{
        try {
                int i = 1;
                TPairID PID;
                ReadRow(&PID, S, i);

                //ADVERTENCIA: aunque el método Read() es atómico, conviene
                //usar una variable tampón ya que hay que comprobar que
                //la cadena contenga solo un valor para un par
                //antes de asignar dicho valor.

                //avanza el índice i hasta la próxima posición que no contenga un separador
                StrTravelSeparators(S, i);
                //si el índice i indica a algún caracter de la cadena S
                if(i <= S.Length())
                        //indica que la cadena S solo debería contener el valor para un par GetLabels()
                        throw EImproperArgument("string S should contain a value to pair "+GetLabels()+" only");

                //asigna el nuevo valor
                *this = PID;

        } catch(...) {
                throw;
        }
}

//FUNCIONES ESTÁTICAS DE COMPARACIÓN:

//compara la variable x
//      si P1->x < P2->x devuelve -1
//      si P1->x > P2->x devuelve 1
//      si P1->x == P2->x devuelve 0
int  TPairID::Comparex(const TPairID *P1, const TPairID *P2)
{
        //el puntro P1 debería apuntar a un par GetLabels() contruido
        if(P1 == NULL)
                throw EImproperArgument("pointer P1 shpuld point to built pair "+GetLabels());
        //el puntro P2 debería apuntar a un par GetLabels() contruido
        if(P2 == NULL)
                throw EImproperArgument("pointer P2 shpuld point to built pair "+GetLabels());

        if(P1->x < P2->x)
                return -1;
        else if(P1->x > P2->x)
                return 1;
        else
                return 0;
}

//FUNCIONES ESTÁTICAS DE CONJUNTOS DE PROPIEDADES:

//imprime los valores de las propiedades de la clase
//en formato texto entre paréntesis
void  TPairID::Print(AnsiString &S, const TPairID *P)
{
        //el puntero P debe apuntar a un par GetLabels()
        if(P == NULL)
                throw EImproperArgument("pointer P should point to built pair "+GetLabels());

        S += P->Text;
}
//lee los valores de las etiquetas de las propiedades de la clase
//en formato textoentre paréntesis
void  TPairID::Read(TPairID *P, const AnsiString &S, int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el puntero P  debería apuntar a un par GetLabels() construido
        if(P == NULL)
                throw EImproperArgument("pointer P should point to built pair "+GetLabels());

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de lectura:
        //      0: esperando "("
        //      1: esperando valor para x
        //      2: esperando ","
        //      3: esperando valor para y
        //      4: esperando ")"
        //      5: par leido con éxito
        int status = 0;

        //variables tampón en formato conveniente
        TPairID t_P;

        do {
                //reacciona según el estado y el caracter
                switch(status) {
                        case 0: //esperando "("
                                try {
                                        StrTravelLabel("(", S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 1: //esperando valor para x
                                try {
                                        int aux;
                                        StrReadInt(aux, S, i);
                                        t_P.x = aux;
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 2: //esperando ","
                                try {
                                        StrTravelLabel(",", S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 3: //esperando valor para y
                                try {
                                        double aux;
                                        StrReadFloat(aux, S, i);
                                        t_P.y = aux;
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 4: //esperando ")"
                                try {
                                        StrTravelLabel(")", S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;
                }
        //mientras no se haya leido el par con éxito
        } while(status < 5);

        //copia la variable tampón
        try {
                *P = t_P;
        } catch(...) {
                throw;
        }
}

//imprime los valores de las propiedades de la clase
//en formato fila de texto
void  TPairID::PrintRow(AnsiString &S, const TPairID *P)
{
        //el puntero P debe apuntar a un par GetLabels() construido
        if(P == NULL)
                throw EImproperArgument("pointer P should point to built pair "+GetLabels());

        S += P->RowText;
}
//lee los valores de las etiquetas de las propiedades de la clase
//en formato fila de texto
void  TPairID::ReadRow(TPairID *P, const AnsiString &S, int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el puntero P  debería apuntar a un par GetLabels() construido
        if(P == NULL)
                throw EImproperArgument("pointer P should point to built pair "+GetLabels());

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de lectura:
        //      0: esperando valor para x
        //      1: esperando separador
        //      2: esperando valor para y
        //      3: par leido con éxito
        int status = 0;

        //variables tampón en formato conveniente
        TPairID t_P;

        do {
                //reacciona según el estado y al caracter
                switch(status) {
                        case 0: //esperando valor para x
                                try {
                                        int aux;
                                        StrReadInt(aux, S, i);
                                        t_P.x = aux;
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 1: //esperando separador
                                try {
                                        StrTravelSeparators(S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 2: //esperando valor para y
                                try {
                                        double aux;
                                        StrReadFloat(aux, S, i);
                                        t_P.y = aux;
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;
                }
        //mientras no se haya leido el par con éxito
        } while(status < 3);

        //copia la variable tampón
        try {
                *P = t_P;
        } catch(...) {
                throw;
        }
}

//obtiene las etiquetas de las propiedades de la clase
//en formato texto entre paréntesis
AnsiString TPairID::GetLabels(void)
{
        AnsiString S;

        S = "("+xLabel+", "+yLabel+")";

        return S;
}
//obtiene las etiquetas de las propiedades de la clase
//en formato fila de texto
AnsiString TPairID::GetRowLabels(void)
{
        AnsiString S;

        S = xLabel+'\t';
        S += yLabel;

        return S;
}
//atraviesa las etiquetas de la clase
//en formato fila de texto
void  TPairID::TravelRowLabels(const AnsiString &S,
        int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de lectura:
        //      0: esperando xLabel
        //      1: esperando separador
        //      2: esperando yLabel
        //      3: etiquetas de las propiedades de recorridas con éxito
        int status = 0;

        do {
                //reacciona según el estado y al caracter
                switch(status) {

                        case 0: //esperando xLabel
                                try {
                                        StrTravelLabel(xLabel, S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 1: //esperando separador
                                try {
                                        StrTravelSeparators(S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 2: //esperando yLabel
                                try {
                                        StrTravelLabel(yLabel, S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;
                }
        //mientras las etiquetas de las propiedades no hayan sido recorridas con éxito
        } while(status < 3);
}

//MÉTODOS PÚBLICOS:

//construye un objeto de la clase
//con los valores por defecto
TPairID::TPairID(void) : x(0), y(0.)
{
}
//construye un objeto de la clase
//con los valores indicados
TPairID::TPairID(int t_x, int t_y) : x(t_x), y(t_y)
{
}

//copia las propiedades de un objeto de la clase
TPairID& TPairID::operator=(const TPairID &PID)
{
        //copia las propiedades
        x = PID.x;
        y = PID.y;

        //devuelve una referencia a este objeto
        //para permitir anidar las asignaciones
        return *this;
}

//---------------------------------------------------------------------------
//TPairIS
//---------------------------------------------------------------------------
//INICIALIZA LAS PROPIEDADES ESTÁTICAS:

AnsiString TPairIS::xLabel = "x";
AnsiString TPairIS::yLabel = "y";

//---------------------------------------------------------------------------
//PROPIEDADES EN FORMATO TEXTO:

AnsiString TPairIS::GetxText(void) const
{
        return IntToStr(x);
}
void TPairIS::SetxText(const AnsiString &S)
{
        try {
                //traduce y asigna el nuevo valor
                x = StrToInt_(S);

        } catch(...) {
                throw;
        }
}
AnsiString TPairIS::GetyText(void) const
{
        return y;
}
void TPairIS::SetyText(const AnsiString &S)
{
        try {
                //asigna el nuevo valor
                y = S;

        } catch(...) {
                throw;
        }
}

//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TPairIS::GetText(void) const
{
        AnsiString S;

        S = "("+xText+", "+yText+")";

        return S;
}
void TPairIS::SetText(const AnsiString &S)
{
        try {
                int i = 1;
                TPairIS P;
                Read(&P, S, i);

                //ADVERTENCIA: aunque el método Read() es atómico, conviene
                //usar una variable tampón ya que hay que comprobar que
                //la cadena contenga solo un valor para un par
                //antes de asignar dicho valor.

                //avanza el índice i hasta la próxima posición que no contenga un separador
                StrTravelSeparators(S, i);
                //si el índice i indica a algún caracter de la cadena S
                if(i <= S.Length())
                        //indica que la cadena S solo debería contener el valor para un par GetLabels()
                        throw EImproperArgument("string S should contain a value to pair "+GetLabels()+" only");

                //asigna el nuevo valor
                *this = P;

        } catch(...) {
                throw;
        }
}
AnsiString TPairIS::GetRowText(void) const
{
        AnsiString S;

        S = xText+'\t';
        S += yText;

        return S;
}
void TPairIS::SetRowText(const AnsiString &S)
{
        try {
                int i = 1;
                TPairIS P;
                ReadRow(&P, S, i);

                //ADVERTENCIA: aunque el método Read() es atómico, conviene
                //usar una variable tampón ya que hay que comprobar que
                //la cadena contenga solo un valor para un par
                //antes de asignar dicho valor.

                //avanza el índice i hasta la próxima posición que no contenga un separador
                StrTravelSeparators(S, i);
                //si el índice i indica a algún caracter de la cadena S
                if(i <= S.Length())
                        //indica que la cadena S solo debería contener el valor para un par GetLabels()
                        throw EImproperArgument("string S should contain a value to pair "+GetLabels()+" only");

                //asigna el nuevo valor
                *this = P;

        } catch(...) {
                throw;
        }
}

//FUNCIONES ESTÁTICAS DE COMPARACIÓN:

//compara la variable x
//      si IP1->x < IP2->x devuelve -1
//      si IP1->x > IP2->x devuelve 1
//      si IP1->x == IP2->x devuelve 0
int  TPairIS::Comparex(const TPairIS *P1, const TPairIS *P2)
{
        //el puntro P1 debería apuntar a un par GetLabels() contruido
        if(P1 == NULL)
                throw EImproperArgument("pointer P1 shpuld point to built pair "+GetLabels());
        //el puntro P2 debería apuntar a un par GetLabels() contruido
        if(P2 == NULL)
                throw EImproperArgument("pointer P2 shpuld point to built pair "+GetLabels());

        if(P1->x < P2->x)
                return -1;
        else if(P1->x > P2->x)
                return 1;
        else
                return 0;
}

//FUNCIONES ESTÁTICAS DE CONJUNTOS DE PROPIEDADES:

//obtiene las etiquetas
//en formato (xLabel, yLabel)
AnsiString TPairIS::GetLabels(void)
{
        AnsiString S;

        S = "("+xLabel+", "+yLabel+")";

        return S;
}

//función de impresión de pares
void  TPairIS::Print(AnsiString &S, const TPairIS *P)
{
        //el puntero P debe apuntar a un par GetLabels()
        if(P == NULL)
                throw EImproperArgument("pointer P should point to built pair "+GetLabels());

        S += P->Text;
}
//función de lectura de pares
void  TPairIS::Read(TPairIS *P, const AnsiString &S, int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el puntero P  debería apuntar a un par GetLabels() construido
        if(P == NULL)
                throw EImproperArgument("pointer P should point to built pair "+GetLabels());

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de lectura:
        //      0: esperando "("
        //      1: esperando valor para x
        //      2: esperando ","
        //      3: esperando valor para y
        //      4: esperando ")"
        //      5: par leido con éxito
        int status = 0;

        //variables tampón en formato conveniente
        TPairIS t_P;

        do {
                //reacciona según el estado y el caracter
                switch(status) {
                        case 0: //esperando "("
                                try {
                                        StrTravelLabel("(", S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 1: //esperando valor para x
                                try {
                                        StrReadInt(t_P.x, S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 2: //esperando ","
                                try {
                                        StrTravelLabel(",", S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 3: //esperando valor para y
                                try {
                                        StrReadStringBetweenChars(t_P.y, S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 4: //esperando ")"
                                try {
                                        StrTravelLabel(")", S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;
                }
        //mientras no se haya leido el par con éxito
        } while(status < 5);

        //asigna la variable tampón
        try {
                *P = t_P;
        } catch(...) {
                throw;
        }
}

//obtiene las etiquetas
//en formato fila de texto
AnsiString TPairIS::GetRowLabels(void)
{
        AnsiString S;

        S = xLabel+'\t';
        S += yLabel;

        return S;
}
//función de atravesar las etiquetas
//en formato fila de texto
void  TPairIS::TravelRowLabels(const AnsiString &S,
        int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de lectura:
        //      0: esperando xLabel
        //      1: esperando separador
        //      2: esperando yLabel
        //      3: etiquetas de las propiedades de recorridas con éxito
        int status = 0;

        do {
                //reacciona según el estado y al caracter
                switch(status) {

                        case 0: //esperando xLabel
                                try {
                                        StrTravelLabel(xLabel, S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 1: //esperando separador
                                try {
                                        StrTravelSeparators(S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 2: //esperando yLabel
                                try {
                                        StrTravelLabel(yLabel, S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;
                }
        //mientras las etiquetas de las propiedades no hayan sido recorridas con éxito
        } while(status < 3);
}

//función de impresión de pares
//en formato fila de texto
void  TPairIS::PrintRow(AnsiString &S, const TPairIS *P)
{
        //el puntero P debe apuntar a un par GetLabels() construido
        if(P == NULL)
                throw EImproperArgument("pointer P should point to built pair "+GetLabels());

        S += P->RowText;
}
//función de lectura de pares
//en formato fila de texto
void  TPairIS::ReadRow(TPairIS *P, const AnsiString &S, int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el puntero P  debería apuntar a un par GetLabels() construido
        if(P == NULL)
                throw EImproperArgument("pointer P should point to built pair "+GetLabels());

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //estado de lectura:
        //      0: esperando valor para x
        //      1: esperando separador
        //      2: esperando valor para y
        //      3: par leido con éxito
        int status = 0;

        //variables tampón en formato conveniente
        TPairIS t_P;

        do {
                //reacciona según el estado y al caracter
                switch(status) {
                        case 0: //esperando valor para x
                                try {
                                        StrReadInt(t_P.x, S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 1: //esperando separador
                                try {
                                        StrTravelSeparators(S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;

                        case 2: //esperando valor para y
                                try {
                                        StrReadStringBetweenChars(t_P.y, S, i);
                                        status++;
                                } catch(...) {
                                        throw;
                                }
                                break;
                }
        //mientras no se haya leido el par con éxito
        } while(status < 3);

        //asigna la variable tampón
        try {
                *P = t_P;
        } catch(...) {
                throw;
        }
}

//MÉTODOS PÚBLICOS:

//construye un par por defecto
TPairIS::TPairIS(void) : x(0), y(0.)
{
}
//contruye un par con los valores indicados
TPairIS::TPairIS(int t_x, int t_y) : x(t_x), y(t_y)
{
}

//copia las propiedades de un par
TPairIS& TPairIS::operator=(const TPairIS &PID)
{
        //copia las propiedades
        x = PID.x;
        y = PID.y;

        //devuelve una referencia a este objeto
        //para permitir anidar las asignaciones
        return *this;
}
*/
//---------------------------------------------------------------------------
//TTernIntegers
//---------------------------------------------------------------------------

//INICIALIZA LAS PROTIEDADES ESTÁTICAS:

AnsiString TTernIntegers::xLabel = "x";
AnsiString TTernIntegers::yLabel = "y";
AnsiString TTernIntegers::zLabel = "z";

//PROTIEDADES EN FORMATO TEXTO:

AnsiString TTernIntegers::getxText(void) const
{
    return IntToStr(x);
}
void TTernIntegers::setxText(const AnsiString &S)
{
    try {
        x = StrToInt_(S);
    } catch(...) {
        throw;
    }
}
AnsiString TTernIntegers::getyText(void) const
{
    return IntToStr(y);
}
void TTernIntegers::setyText(const AnsiString &S)
{
    try {
        y = StrToInt_(S);
    } catch(...) {
        throw;
    }
}
AnsiString TTernIntegers::getzText(void) const
{
    return IntToStr(z);
}
void TTernIntegers::setzText(const AnsiString &S)
{
    try {
        z = StrToInt_(S);
    } catch(...) {
        throw;
    }
}

//CONJUNTOS DE PROTIEDADES EN FORMATO TEXTO:

AnsiString TTernIntegers::getText(void) const
{
    AnsiString S;

    S = AnsiString("(")+getxText()+AnsiString(", ")+getyText()+AnsiString(", ")+getzText()+AnsiString(")");

    return S;
}
void TTernIntegers::setText(const AnsiString &S)
{
    try {
        int i = 1;
        Read((TTernIntegers*)this, S, i);
    } catch(...) {
        throw;
    }
}
AnsiString TTernIntegers::getRowText(void) const
{
    AnsiString S;

    //NOTA: 0x09 corresponde al caracter tabulador horizontal

    S = getxText()+AnsiString("\t")+getyText()+AnsiString("\t")+getzText();

    return S;
}
void TTernIntegers::setRowText(const AnsiString &S)
{
    try {
        int i = 1;
        ReadRow((TTernIntegers*)this, S, i);
    } catch(...) {
        throw;
    }
}

//FUNCIONES ESTÁTICAS:

//compara la propiedad x de dos ternas TI1 y TI2
//si TI1->x < TI2.x
//      devuelve -1
//si TI1->x > TI2.x
//      devuelve 1
//si TI1->x == TI2.x
//      devuelve 0
int  TTernIntegers::Comparexs(const TTernIntegers *TI1,
                              const TTernIntegers *TI2)
{
    //el puntero TI1 debe apuntar a una terna construida
    if(TI1 == NULL)
        throw EImproperArgument("pointer TI1 should point to built tern");
    //el puntero TI2 debe apuntar a una terna construida
    if(TI2 == NULL)
        throw EImproperArgument("pointer TI2 should point to built tern");

    //devuelve el valor conrrespondiente
    if(TI1->x < TI2->x)
        return -1;
    if(TI1->x > TI2->x)
        return 1;
    return 0;
}

//obtiene las etiquetas en formato texto:
//      (xLabel, yLabel, zLabel)
AnsiString TTernIntegers::GetLabels(void)
{
    return AnsiString("(")+xLabel+AnsiString(", ")+yLabel+AnsiString(", ")+zLabel+AnsiString(")");
}
//función de impresión de elementos tipo TTernIntegers
void  TTernIntegers::Print(AnsiString &S, const TTernIntegers *TI)
{
    //el puntero TI debe apuntar a un par de enteros contruido
    if(TI == NULL)
        throw EImproperArgument("pointer TI should point to built integer pair");

    S += TI->getText();
}
//función de lectura de elementos tipo TTernIntegers
void  TTernIntegers::Read(TTernIntegers *TI, const AnsiString &S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el puntero TI  debería apuntar a una terna par GetLabels() construida
    if(TI == NULL)
        throw EImproperArgument(AnsiString("pointer P should point to built tern ")+GetLabelsRow());

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //estado de lectura:
    //      0: esperando '(' y valor para x
    //      1: esperando ',' y valor para y
    //      2: esperando ',', valor para z y ')'
    //      3: terna leída con éxito
    int status = 0;

    //variables tampón en formato conveniente
    int x, y, z;

    do {
        //reacciona según el estado y al caracter
        switch(status) {
        case 0: //esperando '(' y valor para x
            try {
            StrTravelLabel("(", S, i);
            StrReadInt(x, S, i);
            status++;
        } catch(...) {
            throw;
        }
            break;

        case 1: //esperando ',' y valor para y
            try {
            StrTravelLabel(",", S, i);
            StrReadInt(y, S, i);
            status++;
        } catch(...) {
            throw;
        }
            break;

        case 2: //esperando ',', valor para z y ')'
            try {
            StrTravelLabel(",", S, i);
            StrReadInt(z, S, i);
            StrTravelLabel(")", S, i);
            status++;
        } catch(...) {
            throw;
        }
            break;

        }
        //mientras no se haya leido la terna con éxito
    } while(status < 3);

    //asigna las variables tampón
    TI->x = x;
    TI->y = y;
    TI->z = z;
}

//obtiene las etiquetas en formato fila de texto:
//      xLabel+"\t"+yLabel+"\t"+zLabel
AnsiString TTernIntegers::GetLabelsRow(void)
{
    return xLabel+AnsiString("\t")+yLabel+AnsiString("\t")+zLabel;
}
//función de impresión de elementos tipo TTernIntegers
void  TTernIntegers::PrintRow(AnsiString &S, const TTernIntegers *TI)
{
    //el puntero TI debe apuntar a un par de enteros contruido
    if(TI == NULL)
        throw EImproperArgument("pointer TI should point to built integer pair");

    S += TI->getRowText();
}
//función de lectura de elementos tipo TTernIntegers
void  TTernIntegers::ReadRow(TTernIntegers *TI, const AnsiString &S, int &i)
{
    //NOTA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el puntero TI  debería apuntar a una terna par GetLabels() construida
    if(TI == NULL)
        throw EImproperArgument(AnsiString("pointer P should point to built tern ")+GetLabelsRow());

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //estado de lectura:
    //      0: esperando valor para x
    //      1: esperando separador y valor para y
    //      2: esperando separador y valor para z
    //      3: terna leída con éxito
    int status = 0;

    //variables tampón en formato conveniente
    int x, y, z;

    do {
        //reacciona según el estado y al caracter
        switch(status) {
        case 0: //esperando valor para x
            try {
            StrReadInt(x, S, i);
            status++;
        } catch(...) {
            throw;
        }
            break;

        case 1: //esperando separador y valor para y
            try {
            StrTravelSeparators(S, i);
            StrReadInt(y, S, i);
            status++;
        } catch(...) {
            throw;
        }
            break;

        case 2: //esperando separador y valor para z
            try {
            StrTravelSeparators(S, i);
            StrReadInt(z, S, i);
            status++;
        } catch(...) {
            throw;
        }
            break;

        }
        //mientras no se haya leido la terna con éxito
    } while(status < 3);

    //asigna las variables tampón
    TI->x = x;
    TI->y = y;
    TI->z = z;
}

//MÉTODOS PÚBLICOS:

//construye un par de enteros por defecto
TTernIntegers::TTernIntegers(void) : x(0), y(0), z(0)
{
}
//contruye un par de enteros con los valores indicados
TTernIntegers::TTernIntegers(int t_x, int t_y, int t_z) : x(t_x), y(t_y), z(t_z)
{
}

//copia las propiedades de un par
TTernIntegers& TTernIntegers::operator=(const TTernIntegers &TI)
{
    //copia las propiedades
    x = TI.x;
    y = TI.y;
    z = TI.z;

    //devuelve una referencia a este objeto para permitir anidar las asignaciones
    return *this;
}

//---------------------------------------------------------------------------

} //namespace Mathematics

//---------------------------------------------------------------------------
