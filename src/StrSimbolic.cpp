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
//Archivo: StrSimbolic.cpp
//Contenido: funciones de cálculo simbólico
//Última actualización: 16/03/2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Scalars.h" //frexp10
#include "StrSimbolic.h"
#include "Strings.h" //StrFlip

#include <math.h> //pow

//---------------------------------------------------------------------------

using namespace Mathematics; //frexp10

//espacio de nombres de funciones y clases de objetos
//relacionados con cadenas de texto
namespace Strings {

//---------------------------------------------------------------------------

//determina si una cadena contiene un entero;
//a la derecha de la secuencia de cifras puede tener el signo '-'
bool StrIsInteger(const AnsiString &S)
{
    //longitud de la cadena para facilitar la notación
    int N = S.Length();
        char c; //caracter leido de la cadena

    //verifica que la cadena no está vacía
        if (N < 1)
            return false;


    //verifica que las N-1 primeras cifras contienen cifras decimales
        for (int i=1; i<N; i++) {
            c = S[i];
            if (c<'0' || '9'<c)
                return false;
        }

        //verifica que la última posición contiene una cifra decimal
        //o un signo
        c = S[N];
    if ((c<'0' || '9'<c) && c!='-')
            return false;

        //si la última posición contiene un signo
        //la cadena debe tener almenos dos caracteres
    if (c=='-' && N<2)
            return false;

        return true;
}
//---------------------------------------------------------------------------
//copia las cifras significativas de la cadena fuente (S)
//en la cadena destino (D);
//el signo menos no es una cifra significativa
//el cero identificador si es una cifra significativa;
void StrCopySigFig(AnsiString &D, const AnsiString &S)
{
    //determina si se trata de un entero
        if (!StrIsInteger(S))
            throw Exception("S isn't integer");

        //indica a la última cifra no nula
        int i=S.Length();
        if (S[i] == '-') {
            i--;
        }
        while (S[i] == '0')
            i--;

        //determina el nº de caracteres a preservar
        if (i < 1) //si son todo ceros
            i++; //debe preservar el cero identificador

        //reserva los elementos de la cadena destino
        D.SetLength(i);

        //escribe las cifras significativas
    while (i > 0) {
        D[i] = S[i];
                i--;
        }
}
//---------------------------------------------------------------------------
//borra el signo a la derecha de un entero
void StrAbs(AnsiString &S)
{
    //determina si se trata de un entero
        if (!StrIsInteger(S))
            throw Exception("S isn't integer");

        int N = S.Length(); //longitud de la cadena
        if (S[N] == '-') //si el último caracter es un signo
            S.SetLength(--N); //lo borra
}
//---------------------------------------------------------------------------
//función signo devuelve: 1 si S es un entero >=0; y -1 si es un entero <0
int StrSign(const AnsiString &S)
{
    //determina si se trata de un entero
        if (!StrIsInteger(S))
            throw Exception("S isn't integer");

        if (S[S.Length()] == '-') //si el último caracter es un signo menos
            return -1; //devuelve -1

        return 1; //si no es '-' devuelve 1
}
//---------------------------------------------------------------------------
//compara dos enteros devuelve: -1 si S1<S2; 0 si S1==S2; y 1 si S1>S2
int StrCmp(const AnsiString &S1, AnsiString &S2)
{
    //determina que S1 y S2 son enteros
        if (!StrIsInteger(S1))
            throw Exception("S1 isn't integer");
        if (!StrIsInteger(S2))
            throw Exception("S2 isn't integer");

        //indica al último caracter de cada cadena
        int i1 = S1.Length();
        int i2 = S2.Length();

        //determina el signo de cada una de las cadenas
        int sign1;
        i1=='-'? sign1=-1 : sign1=1;
        int sign2;
        i1=='-'? sign2=-1 : sign2=1;

        //realiza una primera comparación de signos
        if (sign1 < sign2)
            return -1;
        else if (sign1 > sign2)
            return 1;

        //a partir de aquí se supone que S1 y S2 tienen el mismo signo

        //indica a la última cifra significativa de cada cadena
        while (S1[i1] == '0')
            i1--;
        while (S2[i2] == '0')
            i2--;

        if (sign1 > 0) { //si ambos signos son positivos
            //realiza una comparación de longitudes
                if (i1 < i2)
                    return -1;
                else if (i1 > i2)
                    return 1;

                //a partir de aquí se supone que i1==i2

                //compara las cifras significativas de derecha a izquierda
                while (i1 > 0) {
                    if (S1[i1] < S2[i1])
                            return -1;
                        else if (S1[i1] > S2[i1])
                            return 1;

                        i1--;
                }
        } else { //si ambos signos son negativos
            //realiza una comparación de longitudes
                if (i1 > i2)
                    return -1;
                else if (i1 < i2)
                    return 1;

                //a partir de aquí se supone que i1==i2

                //compara las cifras significativas de derecha a izquierda
                while (i1 > 0) {
                    if (S1[i1] > S2[i1])
                            return -1;
                        else if (S1[i1] < S2[i1])
                            return 1;

                        i1--;
                }
    }

        return 0; //indica que S1 es igual que S2
}
//---------------------------------------------------------------------------
//suma algebraica de enteros a nivel simbólico
AnsiString StrSumInt(const AnsiString &S1, const AnsiString &S2)
{
    //comprueba que las cadenas contienen números enteros
        if (!StrIsInteger(S1))
            throw Exception("S1 isn't integer");
        if (!StrIsInteger(S2))
            throw Exception("S2 isn't integer");

        //sustrae los signos de las cadenas
        int sign1 = StrSign(S1);
        int sign2 = StrSign(S2);

        //copia las cifras significativas de
        //las cadenas fuente a las cadenas destino
        AnsiString D1, D2;
        StrCopySigFig(D1, S1);
        StrCopySigFig(D2, S2);

        //determina el nº de cifras a sumar
        int N;
        if (D1.Length() > D2.Length())
            N = D1.Length();
        else
            N = D2.Length();

        //reserva la cadena resultante
        AnsiString S;
        S.SetLength(N);

        //determina si Abs(S1) es mayor, igual o menor a Abs(S2)
        //Abs(S1)>Abs(S2) => n=1
        //Abs(S1)==Abs(S2) => n=0
        //Abs(S1)<Abs(S2) => n=-1
        int n = StrCmp(D1, D2);

        //invierte las cadenas, si hace falta,
        //para que al mayor se reste el menor
        if (sign1*sign2<0 && n<0) {
            AnsiString aux = D1;
                D1 = D2;
                D2 = aux;
        }

        int f, f1, f2; //cifras de S, D1 y D2
        int c=0; //acarreo 0, 1

        if (sign1 == sign2) {
            //suma D1+D2 t.q. f p.a. [0, 19]
            for (int i=1; i<=N; i++) {
                //determina f1
                if (D1.Length()<i || D1[i]<'0' || '9'<D1[i])
                        //si D1[i] no es una cifra
                        f1 = 0;
                    else //si D[i] es una cifra
                f1 = int(D1[i]) - 48;

                //determina f2
                if (D2.Length()<i || D2[i]<'0' || '9'<D2[i])
                        //si D2[i] no es una cifra
                        f2 = 0;
                    else //si D[i] es una cifra
                f2 = int(D2[i]) - 48;

                    //suma las cifras
                    f = f1 + f2 + c;

                    //asigna la cifra resultante y determina el acarreo
            if (f > 9) {
                        S[i] = char(f - 10 + 48);
                            c = 1;
                    } else {
                            S[i] = char(f + 48);
                                c = 0;
                        }
            }
        } else {
            //resta D1-D2 t.q. f p.a. [-10, 9]
            for (int i=1; i<=N; i++) {
                //determina f1
                if (D1.Length()<i || D1[i]<'0' || '9'<D1[i])
                        //si D1[i] no es una cifra
                        f1 = 0;
                    else //si D[i] es una cifra
                f1 = int(D1[i]) - 48;

                //determina f2
                if (D2.Length()<i || D2[i]<'0' || '9'<D2[i])
                        //si D2[i] no es una cifra
                        f2 = 0;
                    else //si D[i] es una cifra
                f2 = int(D2[i]) - 48;

                    //suma las cifras
                    f = f1 - f2 - c;

                    //asigna la cifra resultante y determina el acarreo
            if (f < 0) {
                        S[i] = char(f + 10 + 48);
                            c = 1;
                    } else {
                            S[i] = char(f + 48);
                                c = 0;
                        }
            }
        }

        //al restar el menor al mayor,
        //el acarreo final solo puede ser positivo o nulo;

        if (c > 0) ////si hay acarreo final
            //añade la última cifra
                S += char(c+48);

        //borra los ceros a la derecha
        StrCopySigFig (S, S);

        //determina si tiene que añadir el signo
        if ((sign1<0 && sign2<0) || //si S1 y S2 tienen ambos signo
            (n>0 && sign1<0 && sign2>0) || //Abs(S1) es mayor y tiene el s.
                (n<0 && sign1>0 && sign2<0)) //Abs(S2) es mayor y tiene el s.
                S += '-';

        return S; //devuelve el resultado
}
//---------------------------------------------------------------------------
//suma algebraica de dos doubles a nivel simbólico
double SumDouble(double x1, double x2)
{
    //si uno de los sumandos es cero no hace falta sumarlos
    if (x1==0)
            return x2;
    if (x2==0)
            return x1;

    //descompone los números en mantisa y exponente decimal
        int exp1;
        double man1 = frexp10(x1, exp1);
        int exp2;
        double man2 = frexp10(x2, exp2);

        //la mantisa es un nº en [-1, 1]

        //si las mantisas tienen más de 15 cifras significativas,
        //las cifras significativas de peso inferior se perderán
        //en la traducción mediante FloatToStr

        //traduce las mantisas a cadenas de caracteres
        AnsiString S1=FloatToStr(man1);
        AnsiString S2=FloatToStr(man2);

        int N1=S1.Length(); //nº de cifras significativas de S1
        int N2=S2.Length(); //nº de cifras significativas de S2

        //cuenta el nº de caracteres no significativos
        //del principio de las cadenas
        int i1=0;
        while(S1[i1+1]<'1' || '9'<S1[i1+1])
                i1++;
        int i2=0;
        while(S2[i2+1]<'1' || '9'<S2[i2+1])
                i2++;

        //descuenta el nº de caracteres no significativos de las cadenas
        N1 -= i1;
        N2 -= i2;

        //decrementa los caracteres significativos de los exponentes
        exp1 -= N1;
        exp2 -= N2;

        //añadir ceros a la cadena de exponente mayor
        //hasta igualarlo a la de exponente menor
        //(como si hubieras quitado el punto decimal)
    while (exp1 > exp2) {
            S1 += '0';
                exp1--;
        }
    while (exp2 > exp1) {
            S2 += '0';
                exp2--;
        }

        //actualiza el nº de cifras significativas
        N1 = S1.Length() - i1;
        N2 = S2.Length() - i2;

        //voltea las cadenas para que exista relación directa
        //entre el peso y el orden de las cifras
        StrFlip(S1);
        StrFlip(S2);

        //descarta los caracteres no significativos del final
        //obteniendo números enteros (con signo) en S1 y S2
        if (S1[S1.Length()]=='-') {
                S1[N1+1] = '-';
                S1.SetLength(N1+1);
        } else
                S1.SetLength(N1);
        if (S2[S2.Length()]=='-') {
                S2[N2+1] = '-';
                S2.SetLength(N2+1);
        } else
                S2.SetLength(N2);

        //realiza la suma algebraica de las cadenas
        AnsiString S = StrSumInt (S1, S2);

        StrFlip(S); //voltea la cadena para permitir su conversión

        //limita S a 15 cifras significativas
        //para que StrToFloat funcione correctamente
        double exp = exp1;
        if (S[1] == '-') {
            if (S.Length() > 16) {
                        //compensa el dec. de orden de la mant.
                exp += (S.Length() - 16);
                S.SetLength(16); //conserva 15 cifras más el signo
                }
        } else {
            if (S.Length() > 15) {
                        //compensa el dec. de orden de la mant.
            exp += (S.Length() - 15);
                    S.SetLength(15); //conserva 15 cifras
                }
        }

        double man = StrToFloat(S); //convierte a double
        return man*pow(10, exp); //devuelve el resultado
}

//---------------------------------------------------------------------------

} //namespace Strings

//---------------------------------------------------------------------------

