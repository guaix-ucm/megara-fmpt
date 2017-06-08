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
///@file StrPR.cpp
///@brief functions for print and read text strings
///@author Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Exceptions.h" //EImproperArgument
#include "Strings.h" //StrFill
#include "StrPR.h"

#include <stdio.h> //printf, scanf, sprintf, sscanf
#include <iostream> //ostringstream
#include <iomanip> //put_time
#include <ctime> //strftime

//---------------------------------------------------------------------------

//espacio de nombres de funciones y clases de objetos
//relacionados con cadenas de texto
namespace Strings {

//---------------------------------------------------------------------------

//intenta leer una palabra desde la posición i de una cadena de texto
void StrReadWord(AnsiString &Word, const AnsiString S, int &i)
{
    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //si el índice i indica a la posúltima posición
    if(i > S.Length())
        //indica que no se ha encontrado la palabra
        throw EImproperArgument("word not found");

    //estado de lectura
    //      0: esperando ' ', '\r' o primer caracter de la palabra
    //      1: leyendo palabra y esperando ' ' o '\r\n'
    //      2: palabra leída con éxito
    int status = 0;

    //variables auxiliares
    char c; //S[i]
    AnsiString t_Word; //variable tampón

    do {
        c = S[i];

        switch(status) {
        case 0: //esperando ' ', '\r' o primer caracter de la palabra
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("word not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("word not found");
                break;
            default:
                t_Word += c;
                i++;
                if(i > S.Length())
                    status = 2;
                else
                    status++;
            }
            break;
        case 1: //leyendo palabra y esperando ' ' o '\r\n'
            switch(c) {
            case ' ':
                status++;
                break;
            case '\r':
                status++;
                break;
            default:
                t_Word += c;
                i++;
                if(i > S.Length())
                    status++;
            }
            break;
        }
    } while(status < 2);

    //asigna la variable tampón
    Word = t_Word;
}

//intenta leer una cadena desde la posición i de una cadena de texto
//incluyendo las comillas simples delimitadoras
void StrReadString(AnsiString &String, const AnsiString &S, int &i)
{
    //comprueba la precondición
    if(i<1 || S.Length()<i)
        throw EImproperArgument("String value not found");

    //estado de lectura
    //      0: esperando ' ', '\r' o ''' inicial
    //      1: leyendo cadena y esperando ''' final
    //      2: cadena de texto leída con éxito
    int status = 0;

    //variables auxiliares
    AnsiString t_String; //variable tampón

    do {
        char c = S[i];

        switch(status) {
        case 0: //esperando ' ', '\r' o ''' inicial
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("initial quote not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("initial quote ''' not found");
                break;
            case char(39):
                t_String += c;
                i++;
                if(i > S.Length())
                    throw EImproperArgument("final quote ''' not found");
                status++;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be '''"));
            }
            break;
        case 1: //leyendo cadena y esperando ''' final
            t_String += c;
            i++;
            switch(c) {
            case char(39):
                status++;
                break;
            default:
                if(i >S.Length())
                    throw EImproperArgument("final quote ''' not found");
            }
            break;
        }
    } while(status < 2);

    //asigna la variable tampón
    String = t_String;
}
//intenta leer una cadena desde la posición i de una cadena de texto
//que se encuentra delimitada por dos caracteres determinados
void StrReadStringBetweenChars(AnsiString &String, const AnsiString &S, int &i,
                               char c1, char c2)
{
    //si el índice i no indica a una posicion de la cadena de texto S
    if(i<1 || S.Length()<i)
        //indica que no ha encontrado un acadena
        throw EImproperArgument("String value not found");

    //estado de lectura
    //      0: esperando c1
    //      1: leyendo cadena y esperando c2
    //      2: cadena de texto leída con éxito
    int status = 0;

    //variables tampón
    AnsiString t_String;

    //avanza el índice hasta el próximo caracter no separador
    //o hasta la posúltimaposición de la cadena
    StrTravelSeparatorsIfAny(S, i);
    //si la cadena se ha acabado
    if(i > S.Length())
        //indica que no ha encontrado el caracter c1
        throw EImproperArgument("char c1 not found");

    do {
        //asigna el caracter indicado para facilitar su acceso
        char c = S[i];

        //REACCIONA SEGÚN EL ESTADOY EL CARACTERINDICADO:

        //en caso de que el estado sea
        switch(status) {
        case 0: //esperando c1
            //si el primer caracter es distinto de c1
            if(c != c1)
                //indica que no ha encontrado el pirmer caracter delimitador
                throw EImproperArgument("char c1 not found");

            //si la cadena se ha acabado
            if(i > S.Length())
                //indica que no ha encontrado una cadena después de c1
                throw EImproperArgument("string after c1 not found");

            i++; //indica al próximo caracter
            status++; //pasa a leer la cadena
            break;

        case 1: //leyendo cadena y esperando c2
            //si la cadena se ha acabado
            if(i > S.Length())
                //indica que no ha encontrado c2
                throw EImproperArgument("char c2 not found");

            i++; //indica al próximo caracter

            //si el caracter es c2
            if(c == c2)
                //indica que la cadena ha sido leída con éxito
                status++;
            else //si no (si es otro caractercualquiera)
                //añadeel caracter a la cadena tampón
                t_String += c;
            break;
        }
        //mientras no se haya leido la cadena con éxito
    } while(status < 2);

    //asigna la variable tampón
    String = t_String;
}
//traduce una cadena entre dos caracteres a la misma cadena sin los caracteres
AnsiString StrBetweenCharsToStr(const AnsiString &S,
                                char c1, char c2)
{
    try {
        AnsiString SubS;
        int i = 1;
        StrReadStringBetweenChars(SubS, S, i, c1, c2);

        //avanza el índice i hasta la próxima posición que no contenga un separador
        StrTravelSeparators(S, i);
        //si el índice i indica a algún caracter de la cadena S
        if(i <= S.Length())
            //indica que la cadena S solo debería contener el valor para instancia de realización
            throw EImproperArgument("string S should contain a value to realization instance only");

        //devuelve la subcadena
        return SubS;

    } catch(...) {
        throw;
    }
}
//lee una subcadena SubS desde la posición indicada de una cadena S
//hasta el caracter delimitador (d) que se indique
void StrReadStringToChar(AnsiString &SubS, const AnsiString &S, int &i, char d)
{
    //el índice i debería indicar una posición en la cadena S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("index i should indicate a charactewr of the string S");

    //inicializa la subcadena
    SubS = "";
    //caracter auxiliar
    char c;

    do {
        c = S[i]; //extrae el próximo caarcter de la cadena
        SubS += c; //añade el caracter a la cadena
        i++; //indica a la próxima posición
        //mientras el índice indique una posición de la cadena
        //y el caracter en la posición indicada no sea el delimitador
    } while(i<=S.Length() && c!=d);

    //si el último caracter encontrado es el delimitador
    if(c == d) {
        i--;
        SubS.SetLength(SubS.Length()-1);
    }
}
//intenta recorrer una cadena de texto (entre comillas simples)
//desde la posición i de una cadena de texto
void StrTravelString(const AnsiString &S, int &i)
{
    //comprueba la precondición
    if(i<1 || S.Length()<i)
        throw EImproperArgument("String value not found");

    //estado de lectura
    //      0: esperando ' ', '\r' o ''' inicial
    //      1: leyendo cadena y esperando ''' final
    //      2: cadena de texto leída con éxito
    int status = 0;

    do {
        char c = S[i];

        switch(status) {
        case 0: //esperando ' ', '\r' o ''' inicial
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("initial quote not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("initial quote ''' not found");
                break;
            case char(39):
                i++;
                if(i > S.Length())
                    throw EImproperArgument("final quote ''' not found");
                status++;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be '''"));
            }
            break;
        case 1: //leyendo cadena y esperando ''' final
            switch(c) {
            case char(39):
                status++;
                break;
            default:
                i++;
                if(i >S.Length())
                    throw EImproperArgument("final quote ''' not found");
            }
            break;
        }
    } while(status < 2);
}

//intenta leer una etiqueta desde
//la posición indicada de una cadena de texto
void StrReadLabel(AnsiString &Reading, const AnsiString &Label,
                  const AnsiString &S, int &i)
{
    //si el índice i no indica a un caracter de la cadena S
    if(i<1 || S.Length()<i)
        //indica que no ha encontrado la etiqueta
        throw EImproperArgument(AnsiString("'")+Label+AnsiString("' not found"));

    //la etiqeuta Label debería tener algún caracter
    if(Label.Length() < 1)
        throw EImproperArgument("label Label should contain some character");

    //estado de lectura:
    //      0: esperando ' ', '\r' (cuando la etiqueta no empieza por '\r') o etiqueta
    //      1: etiqueta leída con éxito
    int status = 0;

    //variables auxiliares
    char c; //S[i]
    AnsiString t_Reading; //variable tampón

    do {
        c = S[i];

        //esperando ' ', '\r' o etiqueta
        if(c == ' ') {
            i++;
            if(i > S.Length())
                throw EImproperArgument(AnsiString("'")+Label+AnsiString("' not found"));
        } else if(c=='\r' && Label[1]!='\r') {
            StrTravelLabel("\r\n", S, i);
            if(i > S.Length())
                throw EImproperArgument(AnsiString("'")+Label+AnsiString("' not found"));
        } else {
            char c1, c2;

            for(int j=1; j<=Label.Length(); j++) {
                c1 = Label[j];

                if(i > S.Length())
                    throw EImproperArgument(AnsiString("'")+AnsiString(c1)+AnsiString("' of '")+Label+AnsiString("' not found"));

                c2 = S[i];

                //si los caracteres no coinciden
                if(c1 != c2)
                    throw EImproperArgument(AnsiString("'")+AnsiString(c2)+AnsiString("' should be '")+AnsiString(c1)+AnsiString("'"));
                else {
                    t_Reading += c2;
                    i++;
                }
            }
            //indica que ha leido la etiqueta con éxito
            status++;
        }
    } while(status < 1);

    //asigna la variable tampón
    Reading = t_Reading;
}
//recorre una etiqueta a partir de
//la posición indicada de un acadena de texto
void StrTravelLabel(const AnsiString &Label, const AnsiString &S, int &i)
{
    //si el índice i no indica a un caracter de la cadena S
    if(i<1 || S.Length()<i)
        //indica que no ha encontrado la etiqueta
        throw EImproperArgument(AnsiString("label '")+Label+AnsiString("' not found"));

    //la etiqueta Label debería tener algún caracter
    if(Label.Length() < 1)
        throw EImproperArgument("label Label should contain some character");

    //inicializa el índice de la etiqueta
    int j = 1;
    //estado de lectura de la cadena S
    //  0: esperando separador, retorno de carro o primercar cacter de la etiqueta
    //  1: esperando avance de línea
    //  2: separadores iniciales atravesados con éxito y atravesando la etiqueta
    int status = 0;

    //atraviesa los separadores iniciales de la cadena S y el primer caracter de la etiqueta
    do {
        //leeel caracterindicado de lacadena S para facilitar su acceso
        char c = S[i];

        //en caso de que el estado sea...
        switch(status) {
        case 0: //esperando separador, retorno de carro o primer cacter de la etiqueta
            //si el caracter difiere del primer caracter de la etiqueta
            if(c != Label[j])
                //en caso de que el caracter indicado sea...
                switch(c) {
                case ' ': case '\t': //espacio o tabulador
                    i++; //atraviesa el separador indicado
                    break;
                case '\r': //retorno de carro
                    i++; //atraviesa el caracter indicado
                    status++; //pasa a esperar el avance de linea
                    break;
                default: //si no es un separador
                    //indica que no se ha encontrado la etiqueta
                    throw EImproperArgument(AnsiString("label '")+Label+AnsiString("' not found"));
                }
            //si el caracter coincide con el primer caracter de la etiqueta
            else {
                //atraviesa el caracter indicado
                i++; j++;
                status = 2; //indica que ha empezado a atravesar la etiqueta
            }
            break;

        case 1: //esperando avance de línea
            if(c == '\n') {
                i++; //atraviesa el caracter indicado
                status = 0; //vuelve a esperar un separador o retorno de carro, o primer caracter de etiqueta
            } else
                //indica que no se ha encontrado la etiqueta
                throw EImproperArgument(AnsiString("label '")+Label+AnsiString("' not found"));
            break;
        }

        //mientrasnos e haya empezado a atravesar la etiqueta
    } while(status < 2);

    //mientras queden caracteres por atravesar y los caracteres coincidan
    while(i<=S.Length() && j<=Label.Length() && S[i]==Label[j]) {
        //atraviesa el caracter indicado
        i++;
        j++;
    }

    //si no se han atravesado todos los caracteres de la etiqueta
    if(j <= Label.Length())
        //indica que no se ha encontrado la etiqueta
        throw EImproperArgument(AnsiString("label '")+Label+AnsiString("' not found"));
}

//trabel a label from
//the indicated position of a string
//if it is not possible travel all lavel, return false
bool strTravelLabel_(const string& label, const string& str, unsigned int& i)
{
    //check the precondition
    if(str.length() <= i)
        throw EImproperArgument("index i should indicate a position of the string str");

    //travel the string to achieve the end of some string or find some unequal char
    unsigned int j = 0;
    while(i<str.length() && j<label.length() && str[i]==label[j]) {
        i++;
        j++;
    }

    //indicates the result in funtion if the label was whole traveled
    if(j < label.length())
        return false;
    return true;
}

//trabel a label from
//the indicated position of a string
//if it is not possible travel all lavel, return false
void strTravelLabel(const string& label, const string& str, unsigned int& i)
{
    //check the precondition
    if(str.length() <= i)
        throw EImproperArgument("index i should indicate a position of the string str");

    //travel the string to achieve the end of some string or find some unequal char
    unsigned int j = 0;
    while(i<str.length() && j<label.length() && str[i]==label[j]) {
        i++;
        j++;
    }

    //indicates the result in funtion if the label was whole traveled
    if(j < label.length())
        throw EImproperArgument("label \""+label+"\" not found: '"+str[i]+"' should be '"+label[j]+"'");
}

/*
//busca una sentencia de asignación a partir de
//la posición indicada de una cadena de texto
void StrReadAssign(AnsiString &Ident, const AnsiString &S, int &i)
{
        //estado de lectura
        //      0: esperando ' ', '\r', o primer caracter del identificador distinto de '='
        //      1: leyendo identificador y esperando ' ' o '='
        //      2: asignación leída con éxito
        int status = 0;

        //variables ausiliares
        char c; //S[i]
        AnsiString t_Ident; //variable tampón

        do {
                c = S[i];

                switch(status) {
                        case 0: //esperando ' ', '\r', o primer caracter del identificador distinto de '='
                                switch(c) {
                                        case ' ':
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("identifier not fount");
                                                break;
                                        case '\r':
                                                StrTravelLabel("\r\n", S, i);
                                                if(i > S.Length())
                                                        throw EImproperArgument("identifier not fount");
                                                break;
                                        default:
                                                if(c != '=') {
                                                        t_Ident = c;
                                                        i++;
                                                        if(i > S.Length())
                                                                throw EImproperArgument("'=' not fount");
                                                        status++;
                                                }else
                                                        throw EImproperArgument("identifier not fount");
                                }
                                break;
                        case 1: //leyendo identificador y esperando ' ' o '='
                                switch(c) {
                                        case ' ':
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("'=' not found");
                                                break;
                                        case '=':
                                                //el identificador denería ser una cadena de texto imprimible
                                                if(StrIsntPrintable(_Ident))
                                                        throw EImproperArgument("identifier shoud be a printable string");
                                                i++;
                                                status++;
                                                break;
                                        default:
                                                t_Ident += c;
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("'=' not found");
                                }
                                break;
                }
        } while(status < 2);

        //asigna la variable tampón
        Ident = t_Ident;
}      */
/*
//segrega la subcadena desde la posición indicada hasta el final
//i quedará indicando a la posúltima posición
void StrReadToEnd(AnsiString &SubS, const AnsiString &S, int &i)
{
        //el índice i debería indicar a un caracter de la cadena de texto S
        if(i<1 || S.Length()<i)
                throw EImproperArgument("index i should indicate a character of string S");

        SubS = "";

        while(i<=S.Length()) {
                SubS += S[i];
                i++;
        }
}                */

/*//atraviesa la subcadena desde la posición indicada hasta el final
//i quedará indicando a la posúltima posición
void StrTravelToEnd(const AnsiString &S, int &i)
{
    //el índice i debe indicara una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    //si el índice i está indicando a la posúltima posición
    if(i > S.Length())
        return; //ya ha terminado de leer

    //estado de lectura
    //      0: esperando ' ' o '\r'
    //      1: cadena leída con éxito
    int status = 0;

    //variables auxiliares
    char c;

    do {
        c = S[i];

        switch(status) {
        case 0:
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    status++;
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    status++;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ' ' or '\r'"));
            }
            break;
        }
    } while(status < 1);
}
*/
//recorre una cadena desde la posición indicada
//hasta que encuentra un caracter no separador
void StrTravelSeparatorsIfAny(const AnsiString &S, int &i)
{
    //el índice i debería indicar a una posición de la cadena S
    if(i<1 || S.Length()+1<1)
        throw EImproperArgument("index i should indicate a position in the string S");

    //estado de lectura
    //      0: esperando separador (' ', '\t' o '\r')
    //      1: caracter no separador encontrado
    int status = 0;

    char c; //caracter indicado

    do {
        //si ha llegado al final de la cadena
        if(i > S.Length())
            status++;

        else { //si no ha llegado al final de la cadena
            c = S[i]; //lee el próximo caracter

            switch(c) { //en caso de que el el caracter sea
            case ' ': //epacio
            case '\t': //tabulador
                i++; //recorre el caracter
                break;
            case '\r': //retorno de carro
                try {
                //recorre el retorno de carro y fin de línea
                StrTravelLabel("\r\n", S, i);
            } catch(...) {
                throw;
            }
                break;
            default: //si el caracter no es ' ', '\t' ni '\r'
                //indica que ha encontrado un caracter no separador
                status++;
                break;
            }
        }

        //mientras no haya encontrado un caracter no separador
        //y no haya llegado al final de la cadena
    } while(status < 1);
}
//atraviesa uno más separadores
void StrTravelSeparators(const AnsiString &S, int &i)
{
    //el índice i debería indicar a una posición de la cadena S
    if(i<1 || S.Length()+1<1)
        throw EImproperArgument("index i should indicate a position in the string S");

    //estado de lectura
    //      0: esperando primer separador (' ', '\t' o '\r')
    //      1: esperando separador (' ', '\t' o '\r')
    //      2: caracter no separador encontrado
    int status = 0;

    char c; //caracter indicado

    do {
        //si ha llegado al final de la cadena
        if(i > S.Length())
            status++;

        else { //si no ha llegado al final de la cadena
            c = S[i]; //lee el próximo caracter

            //en caso deque el estado sea
            switch(status) {
            case 0: //esperando primer separador (' ', '\t' o '\r')

                switch(c) { //en caso de que el el caracter sea
                case ' ': //epacio
                case '\t': //tabulador
                    i++; //recorre el caracter
                    status++; //pasa aesperar separadores (en su caso)
                    break;

                case '\r': //retorno de carro
                    try {
                    //recorre el retorno de carro y fin de línea
                    StrTravelLabel("\r\n", S, i);
                } catch(...) {
                    throw;
                }
                    status++; //pasa aesperar separadores (en su caso)
                    break;

                default: //si el caracter no es ' ', '\t' ni '\r'
                    //indica que no hay un primer separador
                    throw EImproperArgument("there isn't a first separator");
                }
                break;

            case 1: //esperando separador (' ', '\t' o '\r')
                switch(c) { //en caso de que el el caracter sea
                case ' ': //epacio
                case '\t': //tabulador
                    i++; //recorre el caracter
                    break;

                case '\r': //retorno de carro
                    try {
                    //recorre el retorno de carro y fin de línea
                    StrTravelLabel("\r\n", S, i);
                } catch(...) {
                    throw;
                }
                    break;

                default: //si el caracter no es ' ', '\t' ni '\r'
                    //indica que ha encontrado un caracter no separador
                    status++;
                }
                break;
            }
        }

        //mientras no haya encontrado un caracter no separador
        //y no haya llegado al final de la cadena
    } while(status < 2);
}

//travel one or more separators
void strTravelSeparators(const string& str, unsigned int& i)
{
    //check the precondition
    if(str.length() < i)
        throw EImproperArgument("index i should indicate a position in the string str");

    //reading status:
    //  0: waiting first ' ', '\t' or '\r'
    //  1: waiting first '\n' (if first separator is "\r\n")
    //  2: waiting ' ', '\t' or '\r' or post-last position
    //  3: waiting '\n'
    //  4: non separator character or post-last position found
    int status = 0;

    do {
        //if the string and has finished
        if(i >= str.length()) {
            //reacts according to the status
            switch(status) {
            case 0: //waiting first ' ', '\t' or '\r'
                throw EImproperArgument("first separator not found");

            case 1: //waiting first '\n' (if first separator is "\r\n")
                throw EImproperArgument("first \\n not found");

            case 2: //waiting ' ', '\t' or '\r' or post-last position
                status++;
                break;

            case 3: //waiting '\n'
                throw EImproperArgument("\\n not found");

            default:
                throw EImpossibleError("lateral effect");
            }
        }

        //if i indicates a char of the strings
        else {
            char c = str[i++]; //gest the char

            //reacts according to the status
            switch(status) {
            case 0: //waiting first ' ', '\t' or '\r'
                if(c==' ' || c=='\t')
                    status = 2;
                else if(c == '\r')
                    status++;
                else
                    throw EImproperArgument("first separator not found");
                break;

            case 1: //waiting first '\n' (if first separator is "\r\n")
                if(c == '\n')
                    status++;
                else
                    throw EImproperArgument("first \\n not found");
                break;

            case 2: //waiting ' ', '\t' or '\r' or post-last position
                if(c==' ' || c=='\t')
                    ; //do nothing (keep the status)
                else if(c == '\r')
                    status++;
                else {
                    i--;
                    status = 4;
                }
                break;

            case 3: //waiting '\n'
                if(c == '\n')
                    status--;
                else
                    throw EImproperArgument("\\n not found");
                break;
            }
        }

        //while non separator character or post-last position not found
    } while(status < 4);
}

//travelstring from the indicated position
//to find a nonseparator character
void strTravelSeparatorsIfAny(const string& str, unsigned int& i)
{
    //Here it is not required that string str is printable.

    if(str.length() < i)
        throw EImproperArgument("index i should indicates a position of the string str");

    //NSC: non separator character.

    //reading status
    //  0: waiting separator (' ', '\t' or '\r') or NSC
    //  1: waiting '\n'
    //  2: string finidhed or NSC found
    int status = 0;

    do {
        //if the string has finished
        if(i >= str.length())
            status = 2; //finish the searching

        //if not string finished
        else {
            //get the next char
            char c = str[i];

            //reacts according the status and the char
            switch(status) {
            case 0: //waiting separator (' ', '\t' or '\r') or NSC
                if(c==' ' || c=='\t') {
                    i++;
                } else if(c=='\r') {
                    i++;
                    status++;
                } else {
                    status = 2;
                }
                break;

            case 1: //waiting '\n'
                if(c == '\n') {
                    i++;
                    status--;
                } else {
                    i--;
                    status = 2;
                }
                break;

            case 2: //string finished or NSC found
                break;

            default:
                EImpossibleError("lateral effect");
            }
        }

        //while:
        //- not not string finished
        //- and NSC not found
    } while(status < 2);
}

//---------------------------------------------------------------------------
//CONVERSION FUNCTIONS:

//read an integer in the indicated position of a string
void strReadIntStr(string& dst, const string& src, unsigned int& i)
{
    //Note that it is not required that source string is printable.

    //check the precondition
    if(i > src.length())
        throw EImproperArgument("index i should indicate a position of the string src");

    //reading status:
    //  0: waiting ' ', '\t', '\r', '+', '-' or first decimal char
    //  1: waiting '\n'
    //  2: waiting first decimal char
    //  3: waiting decimal char or other char
    //  4: value readed successfully
    int status = 0;

    //tampon variable
    string t_dst;

    do {
        //if the index i indicates a char of the source string
        if(i < src.length()) {
            char c = src[i++]; //get the next char

            //reacts according the status and the char
            switch(status) {
            case 0: //waiting ' ', '\t', '\r', '+', '-' or first decimal char
                if(c==' ' || c=='\t')
                    ; //do nothing (keep the status)
                else if(c == '\r')
                    status++;
                else if(c=='+' || c=='-') {
                    t_dst += c;
                    status = 2;
                }
                else if('0'<=c && c<='9') {
                    t_dst += c;
                    status = 3;
                }
                else
                    throw EImproperArgument("integer value not found");
                break;

            case 1: //waiting '\n'
                if(c != '\n')
                    throw EImproperArgument("'\\n' notfound");
                else
                    status--;
                break;

            case 2: //waiting first decimal char
                if(c<'0' || '9'<c)
                    throw EImproperArgument("integer value not found");
                else {
                    t_dst += c;
                    status++;
                }
                break;

            case 3: //waiting decimal char or not decimal char
                if(c<'0' || '9'<c) {
                    i--;
                    status++;
                } else
                    t_dst += c;
                break;

            default:
                throw EImpossibleError("lateral effect");
            }
        }
        //if the source string has finished
        else {
            switch(status) {
            case 0: //waiting ' ', '\t', '\r', '+', '-' or first decimal char
                throw EImproperArgument("integer value not found");
                break;

            case 1: //waiting '\n'
                throw EImproperArgument("'\\n' not found");
                break;

            case 2: //waiting first decimal char
                throw EImproperArgument("fist decinal char not found");
                break;

            case 3: //waiting decimal char or other char
                status++;
                break;

            default:
                throw EImpossibleError("lateral effect");
            }
        }

        //while the integer value has not readed successfully
    } while(status < 4);

    //set the tampon variables
    dst = t_dst;
}
//translate from string to int
int strToInt(const string& str)
{
    try {
        //segregates the floating value in other string
        unsigned int i = 0;
        string dst;
        strReadIntStr(dst, str, i);

        //look for unexpected text
        strTravelSeparatorsIfAny(str, i);
        if(i < str.length())
            throw EImproperArgument("unexpected text: \""+StrFirstChars(str.substr(i, str.length() - i)).str+"\"");

        //convert to double
        istringstream ss(dst);
        int value;
        bool ok = bool(ss >> value);
        if(!ok) {
            throw EImproperArgument(AnsiString("can't convert string to int"));
        }

        //Other way of translate to integer:
        //  int value;
        //  sscanf(dst.c_str(), "%d", &value);

        //Other way of translate to integer:
        //  int value = atoi(dst.c_str());

        return value;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "converting string to int: ");
        throw;
    }
}
//read an integer in the indicated position of a string
void strReadInt(int& value, const string& src, unsigned int& i)
{
    try {
        string dst;
        strReadIntStr(dst, src, i);
        value = strToInt(dst);

    } catch(Exception& E) {
        E.Message.Insert(1, "reading integer: ");
        throw;
    }
}

//read an integer in the indicated position of an AnsiString
void StrReadIntStr(AnsiString& D, const AnsiString& S, int& i)
{
    //check the precondition
    if(i <= 0)
        throw EImproperArgument("index i should be upper zero");

    unsigned int _i = (unsigned int)(i - 1);
    strReadIntStr(D.str, S.str, _i);
    i = int(_i + 1);
}

//read an integer in the indicated position of an AnsiString
void StrReadInt(int& value, const AnsiString& S, int &i)
{
    //check the precondition
    if(i <= 0)
        throw EImproperArgument("index i should be upper zero");

    unsigned int _i = (unsigned int)(i - 1);
    strReadInt(value, S.str, _i);
    i = int(_i + 1);
}

//read an integer in the indicated position of a string
//in hexadecimal format
void strReadHexStr(string& dst, const string& src, unsigned int& i)
{
    //Note that it is not required that source string is printable.

    //check the precondition
    if(i > src.length())
        throw EImproperArgument("index i should indicate a position of the string src");

    //reading status:
    //  0: waiting ' ', '\t', '\r', '+', '-' or first hexadecimal char
    //  1: waiting '\n'
    //  2: waiting first hexadecimal char
    //  3: waiting hexadecimal char or other char
    //  4: value readed successfully
    int status = 0;

    //tampon variable
    string t_dst;

    do {
        //if the index i indicates a char of the source string
        if(i < src.length()) {
            char c = src[i++]; //get the next char

            //reacts according the status and the char
            switch(status) {
            case 0: //waiting ' ', '\t', '\r', '+', '-' or first hexadecimal char
                if(c==' ' || c=='\t')
                    ; //do nothing (keep the status)
                else if(c == '\r')
                    status++;
                else if(c=='+' || c=='-') {
                    t_dst += c;
                    status = 2;
                }
                else if(('0'<=c && c<='9') || c=='a' || c=='b' || c=='c' || c=='d' || c=='e' || c=='f' || c=='A' || c=='B' || c=='C' || c=='D' || c=='E' || c=='F') {
                    t_dst += c;
                    status = 3;
                }
                else
                    throw EImproperArgument("hexadecimal value not found");
                break;

            case 1: //waiting '\n'
                if(c != '\n')
                    throw EImproperArgument("'\\n' notfound");
                else
                    status--;
                break;

            case 2: //waiting first hexadecimal char
                if(!(('0'<=c && c<='9') || c=='a' || c=='b' || c=='c' || c=='d' || c=='e' || c=='f' || c=='A' || c=='B' || c=='C' || c=='D' || c=='E' || c=='F'))
                    throw EImproperArgument("integer value not found");
                else {
                    t_dst += c;
                    status++;
                }
                break;

            case 3: //waiting hexadecimal char or not decimal char
                if(!(('0'<=c && c<='9') || c=='a' || c=='b' || c=='c' || c=='d' || c=='e' || c=='f' || c=='A' || c=='B' || c=='C' || c=='D' || c=='E' || c=='F')) {
                    i--;
                    status++;
                } else
                    t_dst += c;
                break;

            default:
                throw EImpossibleError("lateral effect");
            }
        }
        //if the source string has finished
        else {
            switch(status) {
            case 0: //waiting ' ', '\t', '\r', '+', '-' or first decimal char
                throw EImproperArgument("integer value not found");
                break;

            case 1: //waiting '\n'
                throw EImproperArgument("'\\n' not found");
                break;

            case 2: //waiting first decimal char
                throw EImproperArgument("fist hexadecinal char not found");
                break;

            case 3: //waiting hexadecimal char or other char
                status++;
                break;

            default:
                throw EImpossibleError("lateral effect");
            }
        }

        //while the integer value has not readed successfully
    } while(status < 4);

    //set the tampon variables
    dst = t_dst;
}
//translate from string to int
//in hexadecimal format
int hexToInt(const string& str)
{
    try {
        //segregate the hexadecimal value in other string
        unsigned int i = 0;
        string dst;
        strReadHexStr(dst, str, i);

        //look for unexpected text
        strTravelSeparatorsIfAny(str, i);
        if(i < str.length())
            throw EImproperArgument("unexpected text: \""+StrFirstChars(str.substr(i, str.length() - i)).str+"\"");

        //traslate to numerical type
        int value;
        sscanf(dst.c_str(), "%x", &value);

        //Other way of translate from hexadecimal string to int:
        //  int value = (int)strtol(str.c_str(), NULL, 16);

        //return the value
        return value;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "converting hexadecimal string to int: ");
        throw;
    }
}
//read an integer in the indicated position of a string
//in hexadecimal format
void strReadHex(int& value, const string& src, unsigned int& i)
{
    try {
        string dst;
        strReadHexStr(dst, src, i);
        value = hexToInt(dst);

    } catch(Exception& E) {
        E.Message.Insert(1, "reading hexadecimal integer: ");
        throw;
    }
}

//read an integer in the indicated position of an AnsiString
//in hexadecimal format
void StrReadHexStr(AnsiString& D, const AnsiString& S, int& i)
{
    //check the precondition
    if(i <= 0)
        throw EImproperArgument("index i should be upper zero");

    unsigned int _i = (unsigned int)(i - 1);
    strReadHexStr(D.str, S.str, _i);
    i = int(_i + 1);
}
//read an integer in the indicated position of an AnsiString
//in hexadecimal format
void StrReadHex(int& value, const AnsiString& S, int &i)
{
    //check the precondition
    if(i <= 0)
        throw EImproperArgument("index i should be upper zero");

    unsigned int _i = (unsigned int)(i - 1);
    strReadHex(value, S.str, _i);
    i = int(_i + 1);
}

//read a float in the indicated position of a string
void strReadFloatStr(string& dst, const string& src, unsigned int& i)
{
    //Note that it is not required that source string is printable.

    //check the precondition
    if(i > src.length())
        throw EImproperArgument("index i should indicate a position of the string src");

    //reading status:
    //  0: waiting ' ', '\t', '\r', '+', '-' or first decimal char
    //  1: waiting '\n'
    //  2: waiting first decimal char
    //  3: waiting decimal char or decimal separator or 'e'/'E' or other char
    //  4: waiting decimal char or 'e'/'E' or other char
    //  5: waiting '+', '-' or first decimal char
    //  6: waiting first decimal char
    //  7: waiting decimal char or other char
    //  8: floating point readed successfully
    int status = 0;

    //tampon variable
    string t_dst;

    do {
        //if the index i indicates a char of the source string
        if(i < src.length()) {
            char c = src[i++]; //get the next char

            //reacts according the status and the char
            switch(status) {
            case 0: // waiting ' ', '\t', '\r', '+', '-' or first decimal char
                if(c==' ' || c=='\t')
                    ; //do nothing and keep the status
                else if(c == '\r')
                    status++;
                else if(c=='+' || c=='-') {
                    t_dst += c;
                    status = 2;
                } else if('0'<=c && c<='9') {
                    t_dst += c;
                    status = 3;
                } else
                    throw EImproperArgument("floating point value not found");
                break;

            case 1: //waiting '\n'
                if(c != '\n')
                    throw EImproperArgument("'\\n' not found");
                else
                    status--;
                break;

            case 2: // waiting first decimal char
                if(c<'0' || '9'<c)
                    throw EImproperArgument("floating point value not found");
                else {
                    t_dst += c;
                    status++;
                }
                break;

            case 3: //waiting decimal char or decimal separator or 'e'/'E' or other char
                if('0'<=c && c <='9')
                    t_dst += c;
                else if(c == get_decimal_separator()) {
                    t_dst += c;
                    status++;
                } else if(c=='e' || c=='E') {
                    t_dst += c;
                    status = 5;
                } else {
                    i--;
                    status = 8;
                }
                break;

            case 4: //waiting decimal char or 'e'/'E' or other char
                if('0'<=c && c<='9')
                    t_dst += c;
                else if(c=='e' || c=='E') {
                    t_dst += c;
                    status++;
                } else {
                    i--;
                    status = 8;
                }
                break;

            case 5: //waiting '+', '-' or first decimal char
                if(c=='+' || c=='-') {
                    t_dst += c;
                    status++;
                } else if('0'<=c && c<='9') {
                    t_dst += c;
                    status = 7;
                } else {
                    i--;
                    status = 8;
                }
                break;

            case 6: //waiting first decimal char
                if('0'<=c && c<='9') {
                    t_dst += c;
                    status++;
                } else {
                    while(src[i]<'0' || '9'<src[i])
                        i--;
                    status = 8;
                }
                break;

            case 7: //waiting decimal char or other char
                if(c<'0' || '9'<c) {
                    i--;
                    status = 8;
                } else
                    t_dst += c;
                break;

            default:
                throw EImpossibleError("lateral effect");
            }
        }
        //if the source string has finished
        else {
            //reacts according the status
            switch(status) {
            case 0: // waiting ' ', '\t', '\r', '+', '-' or first decimal char
            case 1: //waiting '\n'
            case 2: //waiting first decimal char
                throw EImproperArgument("floating point value notfound");
                break;

            case 3: //waiting decimal char or decimal separator or 'e'/'E'
            case 4: //waiting decimal char or 'e'/'E'
                status = 8;
                break;

            case 5: //waiting '+', '-' or first decimal char
                i--;
                status = 8;
                break;

            case 6: //waiting first decimal char
                while(src[i]<'0' || '9'<src[i])
                    i--;
                status = 8;
                break;

            case 7: //waiting decimal char or other char
                status = 8;
                break;

            default:
                throw EImpossibleError("lateral effect");
            }
        }

        //while the floating point value has not readed successfully
    } while(status < 8);

    //set the tampon variables
    dst = t_dst;
}
//translate from string to double
double strToFloat(const string& str)
{
    try {
        //segregate the value in other string
        unsigned int i = 0;
        string dst;
        strReadFloatStr(dst, str, i);

        //look for unexpected text
        strTravelSeparatorsIfAny(str, i);
        if(i < str.length())
            throw EImproperArgument("unexpected text: \""+StrFirstChars(str.substr(i, str.length() - i)).str+"\"");

        //translate the value to numerical type
        stringstream ss(dst);
        ss.precision(DBL_DIG);
        double value;
        bool ok = bool(ss >> value);
        if(!ok) {
            throw EImproperArgument(AnsiString("can't convert string to double"));
        }

        //Other way of translate to double:
        //  double value;
        //  sscanf(dst.c_str(), "%lf", &value);

        //Other way of translate to double:
        //  char *pEnd;
        //  double value = strtod(dst.c_str(), &pEnd);

        //WARNING: if precision is upper to DBL_DIG decimals, conversión to
        //string can introduce numerial error for somevalues.Example:
        //  value       ->  string
        //  87.035553	->  "87.03555299999999"
        //  -70.35		->  "-70.34999999999999"

        //return the value
        return value;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "converting string to float: ");
        throw;
    }
}
//read a float in the indicated position of a string
void strReadFloat(double& value, const string& src, unsigned int& i)
{
    try {
        string dst;
        strReadFloatStr(dst, src, i);
        value = strToFloat(dst);

    } catch(Exception& E) {
        E.Message.Insert(1, "reading float: ");
        throw;
    }
}

//print a double in an AnsiString
void StrPrintFloat(AnsiString& S, double value)
{
    S += FloatToStr(value);
}
//read a float in the indicated position of an AnsiString
void StrReadFloatStr(AnsiString& D, const AnsiString& S, int &i)
{
    //check the precondition
    if(i <= 0)
        throw EImproperArgument("index i should be upper zero");

    unsigned int _i = (unsigned int)(i - 1);
    strReadFloatStr(D.str, S.str, _i);
    i = int(_i + 1);
}
//read a float in the indicated position of an AnsiString
void StrReadFloat(double& value, const AnsiString& S, int &i)
{
    try {
        //check the precondition
        if(i <= 0)
            throw EImproperArgument("index i should be upper zero");

        unsigned int _i = (unsigned int)(i - 1);
        strReadFloat(value, S.str, _i);
        i = int(_i + 1);
    }
    catch(...) {
        throw;
    }
}

//read a bool in the indicated position of a string
void strReadBoolStr(string& dst, const string& src, unsigned int& i)
{
    //check the precondition
    if(src.length() <= i)
        throw EImproperArgument("boolean value not found");

    //discard the initial separators if any
    strTravelSeparatorsIfAny(src, i);

    //check if there is any text
    if(i >= src.length())
        throw EImproperArgument("boolean value not found");

    //get the first char
    char c = src[i];

    //if the first char is '0' or '1'
    if(c == '0' || c == '1') {
        dst = c;
        i++;
    }
    //else, if the first char is 'f' or 'F'
    else if(tolower(c) == 'f') {
        //check if there are enough chars for contains the word "false"
        if(src.length() <= i + 5)
            throw EImproperArgument("boolean value not found");

        //check if from position i is the word "false"
        if(src[i + 1] != 'a' || src[i + 2] != 'l' || src[i + 3] != 's' || src[i + 4] != 'e')
            throw EImproperArgument("boolean value not found");

        //copy the word in the dst string
        dst = src.substr(i, 5);
        i += 5; //indicates to the next char
    }
    //else, if the first char is 't' or 'T'
    else if(tolower(c) == 't') {
        //check if there are enough chars for contains the word "true"
        if(src.length() <= i + 4)
            throw EImproperArgument("boolean value not found");

        //check if from position i is the word "true"
        if(src[i + 1] != 'r' || src[i + 2] != 'u' || src[i + 3] != 'e')
            throw EImproperArgument("boolean value not found");

        //copy the word in the dst string
        dst = src.substr(i, 4);
        i += 4; //indicates to the next char
    }
    else
        //indicates that there isnt a boolean value in the position i of the string src
        throw EImproperArgument("boolean value not found");
}
//translate from string to bool
bool strToBool(const string& str)
{
    try {
        //segregates the boolean value in other string
        string dst;
        unsigned int i = 0;
        strReadBoolStr(dst, str, i);

        //look for unexpected text
        strTravelSeparatorsIfAny(str, i);
        if(i < str.length())
            throw EImproperArgument("unexpected text: \""+StrFirstChars(str.substr(i, str.length() - i)).str+"\"");

        //traslate to bool
        bool value;
        if(dst=="0" || dst=="false" || dst=="False")
            value = false;
        else if(dst=="1" || dst=="true" || dst=="True")
            value = true;
        else
            throw EImproperArgument("string should be [\"0\" | \"false\" | \"False\"] or  [\"1\" | \"true\" | \"True\"]: \""+str+"\"");

        //Other way to do this?:
        //  istringstream ss(str);
        //  bool value;
        //  bool ok = (ss >> value);
        //  if(!ok) {
        //      throw EImproperArgument(AnsiString("can't convert string to bool"));
        //  }

        return value;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "converting string to bool: ");
        throw;
    }
}
//read a bool in the indicated position of a string
void strReadBool(bool& value, const string& src, unsigned int& i)
{
    try {
        //segregates the boolean value in other string
        string dst;
        strReadBoolStr(dst, src, i);

        //traslate to bool
        if(dst=="0" || dst=="false" || dst=="False")
            value = false;
        else if(dst=="1" || dst=="true" || dst=="True")
            value = true;
        else
            throw EImproperArgument("string should be [\"0\" | \"false\" | \"False\"] or  [\"1\" | \"true\" | \"True\"]: \""+StrFirstChars(src.substr(i, src.length() - i)).str+"\"");

    }
    catch(Exception& E) {
        E.Message.Insert(1, "converting string to bool: ");
        throw;
    }
}

//print a bool in an AnsiString
void StrPrintBool(AnsiString& S, bool value)
{
    S += BoolToStr(value);
}
//read a bool in the indicated position of an AnsiString
void StrReadBoolStr(AnsiString& D, const AnsiString& S, int &i)
{
    //check the precondition
    if(i <= 0)
        throw EImproperArgument("index i should be upper zero");

    unsigned int _i = (unsigned int)(i - 1);
    strReadBoolStr(D.str, S.str, _i);
    i = int(_i + 1);
}
//read a bool in the indicated position of an AnsiString
void StrReadBool(bool& value, const AnsiString& S, int &i)
{
    //check the precondition
    if(i <= 0)
        throw EImproperArgument("index i should be upper zero");

    unsigned int _i = (unsigned int)(i - 1);
    strReadBool(value, S.str, _i);
    i = int(_i + 1);
}

//---------------------------------------------------------------------------

//imprime un punto en una cadena de texto
void StrPrintPoint(AnsiString &S, const TPoint& P)
{
    S = "{"+inttostr(P.x)+", "+inttostr(P.y)+"}";
}
//compara dos punto
//valores devueltos:
//      -1: si x1<x2 || (x1==x2 && y1<y2)
//      1: si x1>x2 || (x1==x2 && y1>y2)
//      0: si x1==x2 && y1==y2
int  CompareDPoints(const TDoublePoint *P1, const TDoublePoint *P2)
{
    //el puntero P1 deberí aapuntar a un punto construido
    if(P1 == NULL)
        throw EImproperArgument("pointer P1 should point to built point");

    //el puntero P2 deberí aapuntar a un punto construido
    if(P2 == NULL)
        throw EImproperArgument("pointer P2 should point to built point");

    if(P1->x<P2->x || (P1->x==P2->x && P1->y<P2->y))
        return -1;
    if(P1->x>P2->x || (P1->x==P2->x && P1->y>P2->y))
        return 1;
    return 0;
}
//imprime un punto al final de una cadena de texto
void StrPrintDPoint(AnsiString &S, const TDoublePoint *P)
{
    //el puntero P debería apuntar a un punto construido
    if(P == NULL)
        throw EImproperArgument("pointer P should point to built point");

    S += DPointToStr(*P);
}
//intenta leer un punto a partir de la posición i de una cadena de texto
//e intenta convertirlo a TDoublePoint mediante StrToFloat
void StrReadDPoint(TDoublePoint *P, const AnsiString &S, int &i)
{
    //el puntero P debería apuntar a un punto contruido
    if(P == NULL)
        throw EImproperArgument("pointer P should point to built point");

    //el indice i debería indicar un caracter de la cadena de texto S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("point value not found");

    //estado de lectura
    //      0: espernado ' ', '\r' o '{'
    //      1: esperando valor en punto flotante
    //      2: esperando ' '. '\r' o ','
    //      3: esperando valor en punto flotante
    //      4: esperando ' '. '\r' o '}'
    //      5: valor punto leido con éxito
    int status = 0;

    //variables auxiliares
    char c; //S[i]
    TDoublePoint t_P; //variable tampón
    AnsiString aux;

    do {
        c = S[i];

        switch(status) {
        case 0: //espernado ' ', '\r' o '{'
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
            case '{':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("value to x not found");
                status++;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be '{'"));
            }
            break;
        case 1: //esperando valor en punto flotante
            try {
            StrReadFloat(t_P.x, S, i);
            status++;
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property x"));
            } catch(...) {
            throw;
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
                    throw EImproperArgument("value to y not found");
                status++;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ','"));
            }
            break;
        case 3: //esperando valor en punto flotante
            try {
            StrReadFloat(t_P.y, S, i);
            status++;
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property y"));
            } catch(...) {
            throw;
        }
            break;
        case 4: //esperando ' ', '\r' o '}'
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
            case '}':
                i++;
                status++;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be '}'"));
            }
            break;
        }
    } while(status < 5);

    //asigna la variable tampón
    *P = t_P;
}
//convierte un valor punto de formato texto a TDoublePoint
//si S contiene algo más que un valor punto
//lanza una excepción EImproperArgument
TDoublePoint StrToDPoint(const AnsiString &S)
{
    TDoublePoint P;

    try {
        //lee un punto a a partir de la primera posición
        int i = 1;
        StrReadDPoint(&P, S, i);

        //busca texto inesperado
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text translating string to double point");

        return P; //devuelve el valor leído
    }
    catch(Exception& E) {
        E.Message.Insert(1, "translating stirng to double point");
        throw;
    }
}

//intenta leer un rectángulo a partir de la posición i de una cadena de texto
//e intenta convertirlo a TDoubleRect mediante StrToFloat
void StrReadDRect(TDoubleRect *R, const AnsiString &S, int &i)
{
    //el punteroR debería apuntar a un rectángulo contruido
    if(R == NULL)
        throw EImproperArgument("pointer R should point to built recttangle");

    //el indice i debería indicar un caracter de la cadena de texto S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("rectangle value not found");

    //estado de lectura
    //      0: espernado ' ', '\r' o '{'
    //      1: esperando valor en punto flotante
    //      2: esperando ' '. '\r' o ','
    //      3: esperando valor en punto flotante
    //      4: esperando ' '. '\r' o '}'
    //      5: esperando valor en punto flotante
    //      6: esperando ' '. '\r' o '}'
    //      7: esperando valor en punto flotante
    //      8: esperando ' '. '\r' o '}'
    //      9: valor rectángulo leido con éxito
    int status = 0;

    //variables auxiliares
    char c; //S[i]
    TDoubleRect t_R; //variable tampón
    AnsiString aux;

    do {
        c = S[i];

        switch(status) {
        case 0: //espernado ' ', '\r' o '{'
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
            case '{':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("value to Left not found");
                status++;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be '{'"));
            }
            break;
        case 1: //esperando valor en punto flotante
            try {
            StrReadFloat(t_R.Left, S, i);
            status++;
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property Left"));
            } catch(...) {
            throw;
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
                    throw EImproperArgument("value to Bottom not found");
                status++;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ','"));
            }
            break;
        case 3: //esperando valor en punto flotante
            try {
            StrReadFloat(t_R.Bottom, S, i);
            status++;
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property Bottom"));
            } catch(...) {
            throw;
        }
            break;
        case 4: //esperando ' ', '\r' o ','
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
                    throw EImproperArgument("value to Right not found");
                status++;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ','"));
            }
            break;
        case 5: //esperando valor en punto flotante
            try {
            StrReadFloat(t_R.Right, S, i);
            status++;
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property Right"));
            } catch(...) {
            throw;
        }
            break;
        case 6: //esperando ' ', '\r' o ','
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
                    throw EImproperArgument("value to Top not found");
                status++;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be ','"));
            }
            break;
        case 7: //esperando valor en punto flotante
            try {
            StrReadFloat(t_R.Top, S, i);
            status++;
        } catch(EImproperArgument &E) {
                throw EImproperArgument(E.Message+AnsiString(" for property Top"));
            } catch(...) {
            throw;
        }
            break;
        case 8: //esperando ' ', '\r' o '}'
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
            case '}':
                i++;
                status++;
                break;
            default:
                throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be '}'"));
            }
            break;
        }
    } while(status < 9);

    //asigna la variable tampón
    *R = t_R;
}
//convierte un valor rectángulo de formato texto a TDoubleRect
//si S contiene algo más que un valor rectángulo
//lanza una excepción EImproperArgument
TDoubleRect StrToDRect(const AnsiString &S)
{
    TDoubleRect R;

    try {
        //lee un punto a a partir de la primera posición
        int i = 1;
        StrReadDRect(&R, S, i);

        //busca texto inesperado
        StrTravelSeparatorsIfAny(S, i);
        if(i <= S.Length())
            throw EImproperArgument("unexpected text translating string to double rect");

        return R; //devuelve el valor leído
    }
    catch(Exception& E) {
        E.Message.Insert(1, "translating stirng to double rect");
        throw;
    }
}

//imprime dos enteros en una cadena indentados a 12 espacios
AnsiString IntToRowStr(int n1, int n2)
{
    AnsiString S;

    S = IntToStr(n1);
    StrFill(S, 12, ' ');
    S += IntToStr(n2);

    return S;
}

//---------------------------------------------------------------------------
/*
//lee de S una cadena hasta que encuentra ';' o EOF
void StrReadSentence(AnsiString &Sentence, const AnsiString &S, int &i)
{
        //si el índice i no indica a un caracter de la cadena S
        if(i<1 || S.Length()<i)
                //indica que no ha encontrado una sentencia
                throw EImproperArgument("sentence not found");

        Sentence = "";

        char ch = S[i];
    while(i<=S.Length() && ch!=';') {
            Sentence += S[i];
        i++;
                if(i <= S.Length())
                        ch = S[i];
        }

        i++;

        //si no ha encontrado el caracter terminador de sentencia
        if(ch != ';')
                //indica que no ha encontrado una sentencia
                throw EImproperArgument("sentence not found");
}   */

/*
//imprime en S las palabras de SL entre c1 y c2 y separadas por s;
//s puede ser un simple espacio
void PrintSecuency (AnsiString &S, TStringList *SL,
    char c1, char c2, const AnsiString s)
{
    S += c1;

        if (SL->Count) {
                S += SL->Strings[0];
                for (int i=1; i<SL->Count; i++)
                    S += s+SL->Strings[i];
        }

        S += c2;
}

//escudriña S en busca de una lista delimitada por c1 y c2
//cuyas palabras están separadas por d
//si no la encuentra lanza una excepción EImproperArgument
void ReadList (TStringList *SL, const AnsiString &S, char c1, char c2, char d)
{
        //el puntro SL debería apuntar a una lista de cadenas de texto contruida
        if(SL == NULL)
                throw EImproperArgument("pointer SL should point to built string list");

        //el caracter c1 debería ser imprimible
        if(!isprint(c1))
                throw EImproperArgument("character '"+AnsiString(c1)+"' should be a printable character");

        //el caracter c2 debería ser imprimible
        if(!isprint(c2))
                throw EImproperArgument("character '"+AnsiString(c2)+"' should be a printable character");

        //el caracter d debería ser imprimible
        if(!isprint(d))
                throw EImproperArgument("character '"+AnsiString(d)+"' should be a printable character");

        //incializa SL
        SL->Clear();

    char status = 0;
    //0: esperando c1
    //1: esperando d o c2
    //2: esperando final de cadena
    //10: esperando avance de linea
    char statusbak;

    int i = 1;
    SL->Clear ();
    while (i<=S.Length()) {
        switch (status) {
            case 0: //esperando c1
                if (S[i] == c1) {
                    SL->Add ("");
                    status = 1;
                } else if (S[i]==' ' || S[i]=='	') {
                    //no hace nada
                } else if (S[i]=='\r') {
                    statusbak = status;
                    status = 10;
                } else { //el primer caracter no es c1
                    throw EImproperArgument ("first char should be \'"+
                        AnsiString(c1)+"\'");
                }
                break;
            case 1: //esperando d o c2
                //NOTA: con las cadenas de SL->Strings no funciona
                //el paso por referencia ni el uso del operador +=
                if (S[i]==d) {
                    SL->Strings[SL->Count-1] =
                    StrTrim(AnsiString(SL->Strings[SL->Count-1]));
                    SL->Add ("");
                } else if (S[i]==c2) {
                    SL->Strings[SL->Count-1] =
                    StrTrim(AnsiString(SL->Strings[SL->Count-1]));
                    status = 2;
                } else
                    SL->Strings[SL->Count-1] = SL->Strings[SL->Count-1]+
                    AnsiString(S[i]);
                    break;
            case 2: //esperando final de cadena
                if (S[i]==' ' || S[i]=='	') {
                    //no hace nada
                } else if (S[i] == '\r') {
                    statusbak = 2;
                    status = 10;
                } else
                    throw EImproperArgument ("list in S should be alone");
                    break;
            case 10: //esperando avance de linea
                if (S[i] == '\n')
                    status = statusbak;
                else { //retorno de carro aislado
                    SL->Clear ();
                    throw EImproperArgument ("improper '\r' in S");
                }
                break;
        } //switch(status)

        i++; //indica el siguiente caracter
    } //while

    //si ha llegado al final de la lista debería estar en el estado 2
    if (status != 2)
        switch (status) {
            case 0: //esperando c1
                throw EImproperArgument ("first char should be \'"+
                    AnsiString(c1)+"\'");
            case 1: //esperando d o c2
                throw EImproperArgument ("dont find \'"+
                    AnsiString(d)+AnsiString("\' or \'")+
                    AnsiString(c2)+AnsiString("\'"));
            case 10: //esperando avance de linea
                throw EImproperArgument ("ReadList(SL, S, c1, c2, d):\r\n"+
                AnsiString("improper char '\r' in S"));
    }
}

//escudriña S en busca de una lista delimitada por c1 y c2
//cuyas palabras están separadas por espacios
//si no la encuentra lanza una excepción EImproperArgument
void ReadVector (TStringList *SL, const AnsiString &S, char c1, char c2)
{
        //el puntro SL debería apuntar a una lista de cadenas de texto contruida
        if(SL == NULL)
                throw EImproperArgument("pointer SL should point to built string list");

        //el caracter c1 debería ser imprimible
        if(!isprint(c1))
                throw EImproperArgument("character '"+AnsiString(c1)+"' should be a printable character");

        //el caracter c2 debería ser imprimible
        if(!isprint(c2))
                throw EImproperArgument("character '"+AnsiString(c2)+"' should be a printable character");

        //incializa SL
        SL->Clear();

    char status = 0;
    //0: esperando c1
    //1: esperando inicio de palabra o c2
    //2: esperando final de palabra o c2
    //3: esperando final de cadena
    //10: esperando avance de linea
    char statusbak;

    int i = 1;
    SL->Clear ();
    while (i<=S.Length()) {
        switch (status) {
            case 0: //esperando c1
                if (S[i] == c1) {
                    status = 1;
                } else if (S[i]==' ' || S[i]=='	') {
                    //no hace nada
                } else if (S[i]=='\r') {
                    statusbak = status;
                    status = 10;
                } else { //el primer caracter no es c1
                    throw EImproperArgument ("first char should be \'"+
                        AnsiString(c1)+"\'");
                }
                break;
            case 1: //esperando inicio de palabra o c2
                if (S[i]==' ' || S[i]=='	') {
                    //no hace nada
                } else if (S[i] == '\r') {
                    statusbak = status;
                    status = 10;
                } else if (S[i] == c2) {
                    status = 3;
                } else { //comienza una palabra
                    SL->Add (AnsiString(S[i]));
                    status = 2;
                }
                break;
            case 2: //esperando final de palabra o c2
                if (S[i]==' ' || S[i]=='	') {
                    SL->Strings[SL->Count-1] =
                    StrTrim(AnsiString(SL->Strings[SL->Count-1]));
                    status = 1;
                } else if (S[i] == '\r') {
                    SL->Strings[SL->Count-1] =
                    StrTrim(AnsiString(SL->Strings[SL->Count-1]));
                    statusbak = 1;
                    status = 10;
                } else if (S[i] == c2) {
                    SL->Strings[SL->Count-1] =
                    StrTrim(AnsiString(SL->Strings[SL->Count-1]));
                    status = 3;
                } else //sigue leyendo la palabra
                    SL->Strings[SL->Count-1] = SL->Strings[SL->Count-1]+
                        AnsiString(S[i]);
                break;
            case 3: //esperando final de cadena
                if (S[i]==' ' || S[i]=='	') {
                    //no hace nada
                } else if (S[i] == '\r') {
                    statusbak = 3;
                    status = 10;
                } else
                    throw EImproperArgument ("list in S should be alone");
                    break;
            case 10: //esperando avance de linea
                if (S[i] == '\n')
                    status = statusbak;
                else { //retorno de carro aislado
                    SL->Clear ();
                    throw EImproperArgument ("improper '\r' in S");
                }
                break;
        }

        i++; //indica el siguiente caracter
    }

    //NOTA: las invocaciones a StrTighten(AnsiStrings(SL->Strings[...]))
    //requieren de la construcción y copia de SL->Strings[...],
    //porque esta propiedad no puede ser modificada pro referencia

    //si ha llegado al final de la lista debería estar en el estado 3
    if (status != 3)
        switch (status) {
            case 0: //esperando c1
                throw EImproperArgument ("first char should be \'"+
                    AnsiString(c1)+"\'");
            case 1: //esperando inicio de palabra o c2
                throw EImproperArgument ("dont find item or \'"+
                    AnsiString(c2)+AnsiString("\'"));
            case 2: //esperando final de palabra o c2
                throw EImproperArgument ("dont find \'"+
                    AnsiString(c2)+AnsiString("\'"));
            case 10: //esperando avance de linea
                throw EImproperArgument ("improper char '\r' in S");
        }
}              */

//---------------------------------------------------------------------------
//FUNCTIONS FOR DATE-TIME:

//translate from time_t to string in format en_EN
string strfromtime_t(time_t t, string format)
{
    tm lt = *localtime(&t);

    char buffer[32];
    strftime(buffer, 32, format.c_str(), &lt);

    return string(buffer);

    //Other way:
    //  ostringstream ss;
    //  ss.imbue(std::locale("en_EN.utf8"));
    //  ss << put_time(&lt, "%c");
    //  string datetime_str = string(ss.str().c_str());
    //  return datetime_str;
}

/*#
//lee la fecha-hora en una cadena de texto a partir de la posición indicada
//en el formato "dd/mm/yyyy hh:mm:ss" en formato de 24h sin AM/PM
//si S no contiene un dato válido, lanza EConvertError
void StrReadDateTime(QDateTime &DT,
        const AnsiString &S, int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caracter por defecto.

        //el índice i debería indicar a una posición de la cadena de texto S
        if(i<1 || S.Length()+1<i)
                throw EImproperArgument("index i should indicate a position in the string S");

        //La longitud de los valores de la propiedad DateTime son:
        //      strlen("yyyy/mm/dd hh:mm:ss"): 19

        //variable tampón
        AnsiString SubS;
*//*
         //copia el índice en una variable auxiliar
         int j = i;
         //avanza el índice auxiliar hasta el próximo caracter no separador
         StrTravelSeparatorsIfAny(S, j);

         //si desde el primera caracter hay al menos 19 caracteres (con él incluido)
         if(j+18 <= S.Length()) {
                //extrae la subcadena compuesta por los próximos 19 caracteres
                SubS = S.SubString(j, 19);

                try {
                        //traduce de texto a estructura
                        DT = StrToDateTime(SubS);
                        //avanza el índice hasta la posúltima posición de la subcadena
                        i = j + 19;
                        return; //no hace nada más
                } catch(...) {
                        throw;
                }
         }

         //ADVERTENCIA: según la documentación el parámetro de la función
         //StrToDateTime() debe estar en el formato de fecha-hora local,
         //que para USA es: MM/DD/YY.*//*#
}

//convierte un valor de fecha-hora de formato texto a QDateTime
//si S contiene algo más que un valor de fehca-hora
//lanza una excepción EImproperArgument
double StrToDateTime_(const AnsiString &S)
{
        try {
                //lee el valor de fehca-hora en la cadena S desde la posición 1
                int i = 1;
                QDateTime DateTime;
                StrReadDateTime(DateTime, S, i);

                //avanza el índice i hasta la próxima posición que no contenga un separador
                StrTravelSeparatorsIfAny(S, i);
                //si el índice i indica a algún caracter de la cadena S
                if(i <= S.Length())
                        //indica que la cadena S solo debería contener el valor para date-time
                        throw EImproperArgument("string S should contain the date-time value only");

                //devuelve el valor de fehca-hora
                return double(DateTime.toTime_t());

        } catch(...) {
                throw;
        }
}
*/
//---------------------------------------------------------------------------

} //namespace Strings

//---------------------------------------------------------------------------

