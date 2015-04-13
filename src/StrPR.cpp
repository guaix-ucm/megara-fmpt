// Copyright (c) 2014-2015 Isaac Morales Durán. All rights reserved.
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
//Archivo: _StrPR.cpp
//Contenido: funciones de impresión y lectura de cadenas de texto
//Última actualización: 18/03/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Exceptions.h" //EImproperArgument
#include "Strings.h" //StrFill
#include "StrPR.h"

//---------------------------------------------------------------------------

//espacio de nombres de funciones y clases de objetos
//relacionados con cadenas de texto
namespace Strings {

//---------------------------------------------------------------------------

//intenta leer una palabra desde la posición i de una cadena de texto
void StrReadWord(AnsiString &Word, const AnsiString S, int &i)
{
        //el índice i debería indicar a una posición de la cadena de texto S
        if(i > S.Length()+1)
                throw EImproperArgument("index i should indicate a position in the string S");

        AnsiString _Word; //variable tampón

        //si el índice i indica a la posúltima posición
        if(i > S.Length())
                //indica que no se ha encontrado la palabra
                throw EImproperArgument("word not found");

        //estado de lectura
        //      0: esperando ' ', '\r' o primer caracter de la palabra
        //      1: leyendo palabra y esperando ' ' o '\r\n'
        //      2: palabra leida con éxito
        int status = 0;

        //variables auxiliares
        char c; //S[i]

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
                                                _Word += c;
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
                                                _Word += c;
                                                i++;
                                                if(i > S.Length())
                                                        status++;
                                }
                                break;
                }
        } while(status < 2);

        //asigna la variable tampón
        Word = _Word;
}

//intenta leer una cadena desde la posición i de una cadena de texto
//incluyendo las comillas simples delimitadoras
void StrReadString(AnsiString &String, const AnsiString &S, int &i)
{
        //si el índice i no indica a una posicion de la cadena de texto S
        if(i<1 || S.Length()<i)
                //indica que no ha encontrado un acadena
                throw EImproperArgument("String value not found");

        //estado de lectura
        //      0: esperando ' ', '\r' o ''' inicial
        //      1: leyendo cadena y esperando ''' final
        //      2: cadena de texto leida con éxito
        int status = 0;

        //variables auxiliares
        AnsiString _String; //variable tampón

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
                                                _String += c;
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
                                _String += c;
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
        String = _String;
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
        //      2: cadena de texto leida con éxito
        int status = 0;

        //variables tampón
        AnsiString _String;

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
                                        //indica que la cadena ha sido leida con éxito
                                        status++;
                                else //si no (si es otro caractercualquiera)
                                        //añadeel caracter a la cadena tampón
                                        _String += c;
                                break;
                }
        //mientras no se haya leido la cadena con éxito
        } while(status < 2);

        //asigna la variable tampón
        String = _String;
}
//traduce uan cadena entredoscaracteres a la misma cadena sin los caracteres
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
                throw EImproperArgument("index i should indicate a position in the string S");

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
        //si el índice i no indica a una posicion de la cadena de texto S
        if(i<1 || S.Length()<i)
                //indica que no ha encontrado un acadena
                throw EImproperArgument("String value not found");

        //estado de lectura
        //      0: esperando ' ', '\r' o ''' inicial
        //      1: leyendo cadena y esperando ''' final
        //      2: cadena de texto leida con éxito
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
        //      1: etiqueta leida con éxito
        int status = 0;

        //variables auxiliares
        char c; //S[i]
        AnsiString _Reading; //variable tampón

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
                                                _Reading += c2;
                                                i++;
                                        }
                                }
                                //indica que ha leido la etiqueta con éxito
                                status++;
                        }
        } while(status < 1);

        //asigna la variable tampón
        Reading = _Reading;
}
//recorre una etiqueta a partir de
//la posición indicada de un acadena de texto
void StrTravelLabel(const AnsiString &Label, const AnsiString &S, int &i)
{
    //si el índice i no indica a un caracter de la cadena S
    if(i<1 || S.Length()<i)
        //indica que no ha encontrado la etiqueta
        throw EImproperArgument(AnsiString("'")+Label+AnsiString("' not found"));

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
                    throw EImproperArgument(AnsiString("'")+Label+AnsiString("' not found"));
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
                throw EImproperArgument(AnsiString("'")+Label+AnsiString("' not found"));
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
        throw EImproperArgument(AnsiString("'")+Label+AnsiString("' not found"));
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
        throw EImproperArgument("label not found: \""+label+"\"");
}

/*
//busca una sentencia de asignación a partir de
//la posición indicada de una cadena de texto
void StrReadAssign(AnsiString &Ident, const AnsiString &S, int &i)
{
        //estado de lectura
        //      0: esperando ' ', '\r', o primer caracter del identificador distinto de '='
        //      1: leyendo identificador y esperando ' ' o '='
        //      2: asignación leida con éxito
        int status = 0;

        //variables ausiliares
        char c; //S[i]
        AnsiString _Ident; //variable tampón

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
                                                        _Ident = c;
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
                                                _Ident += c;
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("'=' not found");
                                }
                                break;
                }
        } while(status < 2);

        //asigna la variable tampón
        Ident = _Ident;
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

//atraviesa la subcadena desde la posición indicada hasta el final
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
        //      1: cadena leida con éxito
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

//recorre una cadena desde la posición indicada
//hasta que encuentra un caracter no separador
void StrTravelSeparatorsIfAny(const AnsiString &S, int &i)
{
        //el índice i debería indicar a una posición de la cadena S
        if(i<1 || S.Length()+1<1)
                throw EImproperArgument("index i should indicate a position in string S");

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
                throw EImproperArgument("index i should indicate a position in string S");

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
        throw EImproperArgument("index i should indicate a position in string S");

    //reading status:
    //  0: waiting first ' ', '\t' or '\r'
    //  1: waiting first '\n' (if first separator is "\r\n")
    //  2: waiting ' ', '\t' or '\r' or post-last position
    //  3: waiting '\n'
    //  4: non separator character or post-last position found
    int status = 0;

    do {
        //if and has finished the string
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
        //if string finished
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

//escudriña S a partir de i en busca de un número entero
//i quedará indicando a la posúltima posición
void StrReadIntStr(AnsiString &I, const AnsiString &S, int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caraacter por defecto.

        //el índice i debería apauntar a un caracter de la cadena de texto S
        if(i<1 || S.Length()<i)
                throw EImproperArgument("integer value not found");

        //inicializa los parámetros referidos
        I = "";

        char c; //caracter S[i]
        //estado de la máquina de estados
        //      0: esperando ' ', '\r', '+'. '-' o primer caracter decimal
        //      1: esperando primer caracter decimal
        //      2: esperando caracter decimal posterior al primero
        //      3: número entero segregado con éxito
        int status = 0;

        do {
                //asigna el próximo caracter para facilitar su acceso
                c = S[i];

                //reacciona en función del estado y el caracter
                switch(status) {
                        case 0: //esperando ' ', '\r', '-', '+' o primer carcacter decimal
                                switch(c) {
                                        case ' ':
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("integer value not found");
                                                break;
                                        case '\r':
                                                StrTravelLabel("\r\n", S, i);
                                                if(i > S.Length())
                                                        throw EImproperArgument("integer value not found");
                                                break;
                                        case '+': case '-':
                                                I += c;
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("first decimal figure not found");
                                                status++;
                                                break;
                                        default:
                                                if(c<'0' || '9'<c)
                                                        throw EImproperArgument("integer value not found");
                                                I += c;
                                                i++;
                                                if(i > S.Length())
                                                        status = 3;
                                                else
                                                        status = 2;
                                }
                                break;
                        case 1: //esperando primer caracter decimal
                                if(c<'0' || '9'<c)
                                        throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be a decimal figure"));
                                I += c;
                                i++;
                                if(i > S.Length())
                                        status = 3;
                                else
                                        status++;
                                break;
                        case 2: //esperando caracter decimal posterior al primero
                                if(c<'0' || '9'<c)
                                        status++;
                                else {
                                        I += c;
                                        i++;
                                        if(i > S.Length())
                                                status = 3;
                                }
                                break;
                }
        //mientras no se haya segregado con éxito un número entero
        } while(i<=S.Length() && status<3);
}
//escudriña S a partir de i en busca de un número entero
//e intenta convertirlo a entero mediante StrToInt
void  StrReadInt(int &n, const AnsiString &S, int &i)
{
        //ADVERTENCIA: S puede contener un valor en punto flotante,
        //por lo cual no vale leer solo la mantisa, pues debe advertirse
        //de dicho error.

        AnsiString I;

        try {
                //segrega un número a partir de i
                StrReadFloatStr(I, S, i);

                //En caso de que S no contenga un número entero a partir de i
                //StrReadIntStr lanzará una excepción EImproperArgument.

                //intenta convertir a entero
                n = StrToInt(I);

                //En caso de que I contenga un valor entero demasiado grande
                //StrToInt lanzará una excepción EConvertError con el mensaje
                //'I is not a valid integer value'.

                //Por consiguiente la excepción EConvertError de StrToInt
                //es equivalente a la excepción EOverflow de AtrToFloat,
                //ya que solo es lanzada cuando I contiene un número entero.

                //captura el efecto lateral de la función de conversión
//                int aux = 0; aux + aux;

/*comentado para traducción
        } catch(EConvertError &E) {
//                throw EImproperArgument("'"+I+"' is a integer value out of range");
                throw EImproperArgument(E.Message);*/
        } catch(...) {
                throw;
        }
}
//convierte un valor entero de formato texto a entero
//si S contiene algo más que un valor entero
//lanza una excepción EImproperArgument
int StrToInt_(const AnsiString &S)
{
        try {
                int i = 1;
                int aux;
                StrReadInt(aux, S, i);
                StrTravelToEnd(S, i);
                return aux;
        } catch(...) {
                throw;
        }
}

//read an integer in the indicated position of a text string
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
    //  4: integer value readed successfully
    int status = 0;

    //tampon variable
    string _dst;

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
                else if(c=='+' || c=='-' || ('0'<=c && c<='9')) {
                    _dst += c;
                    status = 2;
                } else
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
                    _dst += c;
                    status++;
                }
                break;

            case 3: //waiting decimal char or not decimal char
                if(c<'0' || '9'<c) {
                    i--;
                    status++;
                } else
                    _dst += c;
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
    dst = _dst;
}
//read an integer in the indicated position of a text string
void strReadInt(int& value, const string& src, unsigned int& i)
{
    try {
        string dst;
        strReadIntStr(dst, src, i);
        value = strtoint(dst);

    } catch(Exception& E) {
        E.Message.Insert(0, "reading integer: ");
        throw E;
    }
}

//imprime el valor de una variable double en una cadena de texto
void  StrPrintFloat(AnsiString &S, double x)
{
        S += FloatToStr(x);
}
//escudriña S a partir de i en busca de un número de punto flotante
//i quedará indicando a la posúltima posición
void StrReadFloatStr(AnsiString &F, const AnsiString &S, int &i)
{
        //NOTA: no se exige que la cadena de texto S sea imprimible,
        //de modo que cuando se quiera imprimir uno de sus caracteres,
        //si no es imprimible saldrá el caraacter por defecto.

        //el índice i debería indicar un caracter de la cadena de texto S
        if(i<1 || S.Length()<i)
                throw EImproperArgument("floating point value not found");

        //estado de la máquian de estados
        //      0: esperando ' ', '\t', '\r', '+', '-' o primer caracter decimal
        //      1: esperando primer caracter decimal
        //      2: esperando caracter decimal o separador decimal o 'e'/'E'
        //      3: esperando caracter decimal o 'e'/'E'
        //      4: esperando '+', '-' o primer caracter decimal
        //      5: esperando primer caracter decimal
        //      6: esperando caracter decimal o primer caracter de otra cosa
        //      7: número de punto flotante segregado con éxito
        int status = 0;

        //variables auxiliares
        char c; //caracter S[i]
        AnsiString _F; //variable tampón

        do {
                //asigna el próximo caracter para facilitar su acceso
                c = S[i];

                //reacciona en función del estado y el caracter
                switch(status) {
                        case 0: //esperando ' ', '\t', '\r', '+', '-' o primer carcacter decimal
                                switch(c) {
                                        case ' ': case '\t':
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("floating point value not found");
                                                break;
                                        case '\r':
                                                StrTravelLabel("\r\n", S, i);
                                                if(i > S.Length())
                                                        throw EImproperArgument("floating point value not found");
                                                break;
                                        case '+': case '-':
                                                _F += c;
                                                i++;
                                                if(i > S.Length())
                                                        throw EImproperArgument("first decimal figure not found");
                                                status++;
                                                break;
                                        default:
                                                if(c<'0' || '9'<c)
                                                        throw EImproperArgument("floating point value not found");
                                                _F += c;
                                                i++;
                                                if(i > S.Length())
                                                        status = 7;
                                                else
                                                        status = 2;
                                }
                                break;
                        case 1: //esperando primer caracter decimal
                                if(c<'0' || '9'<c)
                                        throw EImproperArgument("first decimal figure not found");
                                _F += c;
                                i++;
                                if(i > S.Length())
                                        status = 7;
                                else
                                        status++;
                                break;
                        case 2: //esperando caracter decimal o separador decimal o 'e'/'E'
                                if('0'<=c && c<='9') {
                                        _F += c;
                                        i++;
                                        if(i > S.Length())
                                                status = 7;
                                } else if(c == get_decimal_separator()) {
                                        _F += c;
                                        i++;
                                        if(i > S.Length())
                                                status = 7;
                                        else
                                                status++;
                                } else if(c=='e' || c=='E') {
                                        _F += c;
                                        i++;
                                        if(i > S.Length())
                                                throw EImproperArgument("exponent not found");
                                        status = 4;
                                } else
                                        status = 7;
                                break;
                        case 3: //esperando caracter decimal o 'e'/'E'
                                if('0'<=c && c<='9') {
                                        _F += c;
                                        i++;
                                        if(i > S.Length())
                                                status = 7;
                                } else if(c=='e' || c=='E') {
                                        _F += c;
                                        i++;
                                        if(i > S.Length())
                                                throw EImproperArgument("exponent not found");
                                        status++;
                                } else
                                        status = 7;
                                break;
                        case 4: //esperando '+', '-', o primer caracter decimal
                                if(c=='-' || c=='+') {
                                        _F += c;
                                        i++;
                                        if(i > S.Length())
                                                throw EImproperArgument("exponent's first decimal figure not found");
                                        status++;
                                } else if('0'<=c && c<='9') {
                                        _F += c;
                                        i++;
                                        if(i > S.Length())
                                                status = 7;
                                        else
                                                status = 6;
                                } else
                                        throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be '+', '-' or a decimal figure"));
                                break;
                        case 5: //esperando primer caracter decimal
                                if(c<'0' || '9'<c)
                                        throw EImproperArgument(AnsiString("'")+AnsiString(c)+AnsiString("' should be a decimal figure"));
                                else {
                                        _F += c;
                                        i++;
                                        if(i > S.Length())
                                                status = 7;
                                        else
                                                status++;
                                }
                                break;
                        case 6: //esperando caracter decimal o primer caracter de otra cosa
                                if(c<'0' || '9'<c)
                                        status = 7;
                                else {
                                        _F += c;
                                        i++;
                                        if(i > S.Length())
                                                status = 7;
                                }
                                break;
                }
        //mientras no se haya segregado con éxito un número de punto flotante
        } while(status < 7);

        //asigna la variable tampón
        F = _F;
}
//escudriña S a partir de i en busca de un número de punto flotante
//e intenta convertirlo a double mediante StrToFloat
void  StrReadFloat(double &x, const AnsiString &S, int &i)
{
        AnsiString F;

        try {
                //segrega un punto flotante a partir de i
                StrReadFloatStr(F, S, i);

                //intenta convertir a entero
                x = StrToFloat(F);

                //captura el efecto lateral de la función de conversión
//                double aux = 0; aux + aux;

/*comentado para traducción
        } catch(EOverflow &E) {
                throw EImproperArgument(AnsiString("'")+F+AnsiString("' is a floating point value out of range"));*/
        } catch(...) {
                throw;
        }
}
//convierte un valor en punto flotante de formato texto a double
//si S contiene algo más que un valor en punto flotante
//lanza una excepción EImproperArgument
double StrToFloat_(const AnsiString &S)
{
        try {
                //lee Float en la cadena S desde la posición 1
                int i = 1;
                AnsiString FloatText;
                StrReadFloatStr(FloatText, S, i);

                //avanza el índice i hasta la próxima posición que no contenga un separador
                StrTravelSeparatorsIfAny(S, i);
                //si el índice i indica a algún caracter de la cadena S
                if(i <= S.Length())
                        //indica que la cadena S solo debería contener el valor para Id
                        throw EImproperArgument("string S should contain the Id value only");

                //traduce a double y devuelve el valor
                return StrToFloat(FloatText);

        } catch(...) {
                throw;
        }
}

//read an float in the indicated position of a text string
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
    string _dst;

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
                    _dst += c;
                    status = 2;
                } else if('0'<=c && c<='9') {
                    _dst += c;
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
                    _dst += c;
                    status++;
                }
                break;

            case 3: //waiting decimal char or decimal separator or 'e'/'E' or other char
                if('0'<=c && c <='9')
                    _dst += c;
                else if(c == get_decimal_separator()) {
                    _dst += c;
                    status++;
                } else if(c=='e' || c=='E') {
                    _dst += c;
                    status = 5;
                } else {
                    i--;
                    status = 8;
                }
                break;

            case 4: //waiting decimal char or 'e'/'E' or other char
                if('0'<=c && c<='9')
                    _dst += c;
                else if(c=='e' || c=='E') {
                    _dst += c;
                    status++;
                } else {
                    i--;
                    status = 8;
                }
                break;

            case 5: //waiting '+', '-' or first decimal char
                if(c=='+' || c=='-') {
                    _dst += c;
                    status++;
                } else if('0'<=c && c<='9') {
                    _dst += c;
                    status = 7;
                } else {
                    i--;
                    status = 8;
                }
                break;

            case 6: //waiting first decimal char
                if('0'<=c && c<='9') {
                    _dst += c;
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
                    _dst += c;
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
    dst = _dst;
}
//read an float in the indicated position of a text string
void strReadFloat(double& value, const string& src, unsigned int& i)
{
    try {
        string dst;
        strReadFloatStr(dst, src, i);
        value = strtofloat(dst);

    } catch(Exception& E) {
        E.Message.Insert(0, "reading integer: ");
        throw E;
    }
}

//Imprime el valor de una variable lógica al final de una cadena de texto.
//Utiliza los identificadores:
//      TrueBoolStrs[0] en vez de "1", y
//      FalseBoolStrs[0] en vez de "0".
//Si b==true y TrueBoolStrs[0] no está definido
//lanza una excepción EImproperCall.
//Si b==false y FalseBoolStrs[0] no está definido
//lanza una excepción EImproperCall.
void  StrPrintBool(AnsiString &S, bool b)
{
        //la cadena TrueBoolStrs[0] debería contener al menos un caracter
        if(b && TrueBoolStrs.getCount()<1 && TrueBoolStrs[0].Length()<1)
                throw EImproperCall("string TrueBoolStrs[0] should contain one character almost");

        //la cadena FalseBoolStrs[0] debería contener al menos un caracter
        if(!b && FalseBoolStrs.getCount()<1 && FalseBoolStrs[0].Length()<1)
                throw EImproperCall("string FalseBoolStrs[0] should contain one character almost");

        //imprime el contenido de TrueBoolStrs[0] o FalseBoolStrs[0]
        S += BoolToStr(b, true);
}
//Intenta leer un valor lógico en S desde la posición i.
//Deberán estar definidos los identificadores:
//      TrueBoolStrs[0] y
//      FalseBoolStrs[0].
//Si encuentra TrueBoolStrs[0] devuelve B = TrueBoolStrs[0].
//Si encuentra FalseBoolStrs[0] devuelve B = TrueBoolStrs[0].
//El índice i quedará indicando a la posúltima posición
void StrReadBoolStr(AnsiString &B, const AnsiString &S, int &i)
{
        //la cadena TrueBoolStrs[0] debería contener al menos un caracter
        if(TrueBoolStrs.getCount()<1 && TrueBoolStrs[0].Length()<1)
                throw EImproperCall("strings TrueBoolStrs[0] should contain one character almost");

        //la cadena FalseBoolStrs[0] debería contener al menos un caracter
        if(FalseBoolStrs.getCount()<1 && FalseBoolStrs[0].Length()<1)
                throw EImproperCall("strings FalseBoolStrs[0] should contain one character almost");

        //el indice i debería indicar un caracter de la cadena de texto S
        if(i<1 || S.Length()<i)
                throw EImproperArgument("boolean value not found");

        //estado de lectura
        //      0: espernado ' ', '\r', TrueBoolStrs[0][1] o FalseBoolStrs[0][1]
        //      1: valor lógico leido con éxito
        int status = 0;

        //variables auxiliares
        char c; //S[i]
        AnsiString _B; //variable tampón
        AnsiString aux;

        do {
                c = S[i];

                switch(status) {
                        case 0: //esperando ' ', '\r', TrueBoolStrs[0][1] o FalseBoolStrs[0][1]
                                if(c == ' ') {
                                        i++;
                                        if(i > S.Length())
                                                throw EImproperArgument("boolean value not found");
                                } else if(c == '\r') {
                                        StrTravelLabel("\r\n", S, i);
                                        if(i > S.Length())
                                                throw EImproperArgument("boolean value not found");
                                } else if(c == TrueBoolStrs[0][1]) {
                                        try {
                                                StrReadLabel(_B, TrueBoolStrs[0], S, i);
                                        } catch(...) {
                                                throw;
                                        }
                                        status++;
                                } else if(c == FalseBoolStrs[0][1]) {
                                        try {
                                                StrReadLabel(_B, FalseBoolStrs[0], S, i);
                                        } catch(...) {
                                                throw;
                                        }
                                        status++;
                                } else
                                        throw EImproperArgument("boolean value not found");
                                break;
                }
        } while(status < 1);

        //asigna la variable tampón
        B = _B;
}
//escudriña S a partir de i en busca de un valor lógico
//e intenta convertirlo a lógico mediante StrToBool
void  StrReadBool(bool &b, const AnsiString &S, int &i)
{
        AnsiString F;

        try {
                //segrega un lógico a aprtir de i
                StrReadBoolStr(F, S, i);

                //intenta convertir a lógico
                b = StrToBool(F);

        } catch(...) {
                throw;
        }
}

//Convierte un valor lógico a un AnsiString.
//Si UseBoolStrs==true, B==true y TrueBoolStrs[0] no está definido
//      lanza una excepción EImproperCall.
//Si UseBoolStrs==true, B==false y FalseBoolStrs[0] no está definido
//      lanza una excepción EImproperCall.
AnsiString  BoolToStr(bool B, bool UseBoolStrs)
{
        //check the precondition
        if(UseBoolStrs) {
                if(B && (TrueBoolStrs.getCount()<=0 || TrueBoolStrs[0].Length()<1))
                        throw EImproperCall("string TrueBoolStrs[0] should contain one character almost");
                if(!B && (FalseBoolStrs.getCount()<=0 || FalseBoolStrs[0].Length()<1))
                        throw EImproperCall("string FalseBoolStrs[0] should contain one character almost");
        }

        if(UseBoolStrs) {
            if(B)
                return TrueBoolStrs[0];
            else
                return FalseBoolStrs[0];
        }

        else {
            if(B)
                return AnsiString("1");
            else
                return AnsiString("0");
        }
}
//Convierte un AnsiString a un valor lógico.
//Si TrueBoolStrs[0] o FalseBoolStrs[0] no están definidos
//lanza una excepción EImproperCall.
bool  StrToBool(const AnsiString S) {
        //check the preconditions
        if(TrueBoolStrs.getCount()<1 || TrueBoolStrs[0].Length()<1)
                throw EImproperArgument("string TrueBoolStrs[0] should contain one character almost");
        if(FalseBoolStrs.getCount()<1 || FalseBoolStrs[0].Length()<1)
                throw EImproperArgument("string FalseBoolStrs[0] should contain one character almost");

        if(S.Length() == 1) {
            if(S == "1")
                return true;
            else if(S == "0")
                return false;
        }

        if(TrueBoolStrs.getCount()>0 && S==TrueBoolStrs[0])
            return true;

        if(FalseBoolStrs.getCount()>0 && S==FalseBoolStrs[0])
            return false;

        //indica que la cadena S debería contener un valor lógico
        throw EImproperArgument("strins S should contains a Boolean value");
}

//imprime un punto en una cadena de texto
void  StrPrintPoint(AnsiString &S, TPoint P)
{
        S = AnsiString("(")+IntToStr(P.x)+AnsiString(" ")+IntToStr(P.y)+AnsiString(")");
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
void  StrPrintDPoint(AnsiString &S, const TDoublePoint *P)
{
        //el puntero P debería apuntar a un punto construido
        if(P == NULL)
                throw EImproperArgument("pointer P should point to built point");

        S += DPointToStr(*P);
}
//intenta leer un punto a partir de la posición i de una cadena de texto
//e intenta convertirlo a TDoublePoint mediante StrToFloat_
void  StrReadDPoint(TDoublePoint *P, const AnsiString &S, int &i)
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
        TDoublePoint _P; //variable tampón
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
                                        StrReadFloat(_P.x, S, i);
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
                                        StrReadFloat(_P.y, S, i);
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
        *P = _P;
}
//convierte un valor punto de formato texto a TDoublePoint
//si S contiene algo más que un valor punto
//lanza una excepción EImproperArgument
TDoublePoint StrToDPoint(const AnsiString &S)
{
        TDoublePoint P;

        try {
                int i = 1;

                //lee un punto a aprtir de i
                StrReadDPoint(&P, S, i);

                //atraviesa la cadena hasta el final
                StrTravelToEnd(S, i);

                return P; //devuelve el valor leído

        } catch(...) {
                throw;
        }
}

//intenta leer un rectángulo a partir de la posición i de una cadena de texto
//e intenta convertirlo a TDoubleRect mediante StrToFloat_
void  StrReadDRect(TDoubleRect *R, const AnsiString &S, int &i)
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
        TDoubleRect _R; //variable tampón
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
                                        StrReadFloat(_R.Left, S, i);
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
                                        StrReadFloat(_R.Bottom, S, i);
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
                                        StrReadFloat(_R.Right, S, i);
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
                                        StrReadFloat(_R.Top, S, i);
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
        *R = _R;
}
//convierte un valor rectángulo de formato texto a TDoubleRect
//si S contiene algo más que un valor rectángulo
//lanza una excepción EImproperArgument
TDoubleRect StrToDRect(const AnsiString &S)
{
        TDoubleRect R;

        try {
                int i = 1;

                //lee un punto a aprtir de i
                StrReadDRect(&R, S, i);

                //atraviesa la cadena hasta el final
                StrTravelToEnd(S, i);

                return R; //devuelve el valor leído

        } catch(...) {
                throw;
        }
}

//imprime dos enteros en una xadena indentados a 12 espacios
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
//Funciones para fecha-hora
/*#
//lee la fecha-hora en una cadena de texto a partir de la posición indicada
//en el formato "dd/mm/yyyy hh:mm:ss" en formato de 24h sin AM/PM
//si S no contiene un dato válido, lanza EConvertError
void  StrReadDateTime(QDateTime &DT,
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

