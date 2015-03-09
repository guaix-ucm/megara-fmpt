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
//Archivo: Strings.cpp
//Contenido: funciones con cadenas de texto
//Última actualización: 12/06/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Strings.h"
#include "Exceptions.h"
#include "Scalars.h"

//---------------------------------------------------------------------------

using namespace Mathematics;

//espacio de nombres de funciones y clases de objetos
//relacionados con cadenas de texto
namespace Strings {

//---------------------------------------------------------------------------

//determina si un caracter ISO8859-1 es imprimible latino
bool islatinprint(unsigned char c)
{
    if(c<160)
        return false;
    else
        return true;
}

//determina si la cadena contiene un separador de los usualmente escritos
bool StrIsUsualSeparator(AnsiString &S)
{
    if(S==AnsiString(char(9)) || S==AnsiString(char(32)) || S=="\r\n")
        return true;
    else
        return false;}

//busca en S, a partir de i, el próximo caracter imprimible distinto de espacio
//si no lo encuentra devuelve la posúltima posición
int StrNextChar(const AnsiString &S, int i)
{
    //el índice i debería indicar un caracter en la cadena S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("index i should indicate a character in string S");

    char c;

    while(i <= S.Length()) {
        //asigna el caracter para facilitar su acceso
        c = S[i];

        //si es un caracter imprimible no delimitador
        if((isprint(c) || islatinprint(c)) && c!=' ')
            return i;

        //apunta al primer caracter del siguiente símbolo
        i++;
    }

    return i;
}
//busca en S, a partir de i, el caracter previo imprimible distinto de espacio
//si no lo encuentra devuelve la preprimera posición
int StrPreviusChar(const AnsiString &S, int i)
{
    //el índice i debería indicar un caracter en la cadena S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("index i should indicate a character in string S");

    char c;

    while(i >= 1) {
        //asigna el caracter para facilitar su aceso
        c = S[i];

        //si es un caracter imprimible no delimitador
        if((isprint(c) || islatinprint(c)) && c!=' ')
            return i;

        //apunta al último caracter del siguiente símbolo
        i--;
    }

    return i;
}
//busca el caracter en la cadena
int StrSearch(const AnsiString &S, char c, int i)
{
    //el índice i debería indicar un caracter en la cadena S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a character in string S");

    while(i<=S.Length() && S[i]!=c)
        i++;

    return i;
}

//invierte el orden de los elementos de una cadena
void StrFlip(AnsiString &S)
{
    int i = 1;
    int j = S.Length();
    char aux;
    while(i < j) {
        aux = S[i];
        S[i] = S[j];
        S[j] = aux;
        i++;
        j--;
    }
}

//determina si una cadena contiene solo caracteres imprimibles
//y delimitadores usuales
bool StrIsPrintable(const AnsiString &S)
{
    //busca un caracter que no cumpla los requisitos

    int i = 1;
    char c;

    //mientras indique a una posición de la cadena
    while(i <= S.Length()) {
        //asigna el caracter indicado para facilitar su acceso
        c = S[i];

        //si es '\r'
        if(c == '\r') {
            //si es el último
            if(i >= S.Length())
                return false; //indica que no es imprimible
            //pasa al próximo caracter
            i++;
            c = S[i];
            //si el siguiente no es '\n'
            if(c != '\n')
                return false; //indica que no es imprimible
        }
        //si no es '\r' ni imprimible ni separador usual
        else if(!isprint(c) && !islatinprint(c) && c!=' ' && c!=' ')
            //indica que no es imprimible
            return false;

        i++; //indica a la próxima posición
    }

    //indica que si es imprimible
    return true;
}
//determina si una cadena contiene caracteres no imprimibles
//ni delimitadores usuales
bool StrIsntPrintable(const AnsiString &S)
{
    //busca un caracter que no cumpla los requisitos

    int i = 1;
    char c;

    //mientras indique a una posición de la cadena
    while(i <= S.Length()) {
        //asigna el caracter indicado para facilitar su acceso
        c = S[i];

        //si es '\r'
        if(c == '\r') {
            //si es el último
            if(i >= S.Length())
                return true; //indica que no es imprimible
            //pasa al próximo caracter
            i++;
            c = S[i];
            //si el siguiente no es '\n'
            if(c != '\n')
                return true; //indica que no es imprimible
        }
        //si no es '\r' ni imprimible ni separador usual
        else if(!isprint(c) && !islatinprint(c) && c!=' ' && c!=' ')
            //indica que no es imprimible
            return true;

        i++; //indica a la próxima posición
    }

    //indica que si es imprimible
    return false;
}

/*
//sustituye las secuencias de espacios y tabuladores por un solo espacio
AnsiString& StrTighten(AnsiString &S)
{
        //si está vacía no hace nada
        if(S.Length() < 1)
                return S;

        //busca el primer caracter imprimible distinto de espacio
        int i1 = StrNextChar(S, 1);

    //busca el último caracter imprimible distinto de espacio
        int i2 = StrPreviusChar(S, S.Length());

        if(i1>i2)
                S = "";
        else {
                int i = i1; //indica al primer caracter no delimitador
                int j=1; //indica a la primera posición de la nueva cadena
                char c;
                while(i <= i2) {
                        //lee el próximo caraceter de la cadena para facilitar su acceso
                        c = S[i];

                        //si el caracter es imprimible
                        if(isprint(c) || islatinprint(c)) {
                                if(c!=' ' && c!='	') { //si no es delimit.
                                        S[j] = S[i]; //lo asigna
                                        i++; //indica al próximo caracter
                                } else { //si es delimitador
                                    S[j] = ' '; //asigna un solo espacio
                                        //indica al próximo caracter
                                        //imprimible no delimitador
                                        i = StrNextChar(S, i+1);
                                }
                                j++; //indica a la siguiente posición
                        }
                        //si el caracter no es imprimible
                        else
                                //se lo salta
                                i++;
                }
                S.SetLength(j-1); //ajusta la longitud de la cadena
        }

        return S;
}         */
//determina si una cadena no está entre comillas simples
bool StrIsntString(const AnsiString &S)
{
    //si no contiene al menos las comillas marginales
    if(S.Length() < 2)
        return true; //indica que no es una cadena

    //busca el primer caracter imprimible distinto de espacio
    int i1 = StrNextChar(S, 1);
    //si el primer caracter no es una comilla simple
    if(S[i1] != '\'')
        return true; //indica que no es una cadena

    //busca el último caracter imprimible distinto de espacio
    int i2 = StrPreviusChar(S, S.Length());
    //si el último caracter no es una comilla simple
    if(S[i2] != '\'')
        return true; //indica que no es una cadena

    for(int i=i1+1; i<i2; i++)
        if(S[i] == '\'')
            return true; //indica que no es una cadena

    return false; //indica que si es una cadena
}
//Segrega en Value la subcadena que precede al primer '=' en S
//y en Value la subcadena que le sucede; si tiene éxito devuelve true
//y si S no contiene un caracter '=' devuelve false
bool StrSplitAssign(AnsiString &Ident, AnsiString &Value, const AnsiString &S)
{
    //copia en Ident hasta que encuentra '='
    Ident = "";
    int i = 1;
    while(i<=S.Length() && S[i]!='=') {
        Ident += AnsiString(S[i]);
        i++;
    }

    //si se acabó la cadena esque no encontró '='
    if(i > S.Length())
        return false; //indica que no es una asignación

    //copia en Value hasta que se termina S
    Value = "";
    i++;
    while(i<=S.Length()) {
        Value += AnsiString(S[i]);
        i++;
    }

    //descarta los márgenes
    Ident = StrTrim(Ident);
    Value = StrTrim(Value);

    return true; //indica que ha tenido éxito
}
//separa la última extensión de un nombre de archivo
//si no tiene extensión devuelve la cadena vacía
AnsiString StrExtension(const AnsiString &filename)
{
    AnsiString ext; //cadena para copiar la extensión

    //busca el primer caracter distinto de espacio
    //empezando por el final
    int j = filename.Length();
    while(j>0 && filename[j]!='/' && filename[j]==' ')
        j--;

    //la cadena es todo espacios hasta el principio o hasta el '\'
    if(j<1 || filename[j]=='/')
        return "";

    //busca el primer punto
    int i = j;
    while(i>0 && filename[i]!='/' && filename[i]!='.')
        i--;

    //i no se movió o recorrió hasta '\' o la posición cero
    if(i<1 || filename[i]=='/' || i>=j)
        return "";

    //copia la extensión en ext
    int k = 1;
    ext.SetLength(j-i+1);
    for(; i<=j; i++)
        ext[k++] = filename[i];

    //indica que encontró la extensiónd e almenos un caracter
    return ext;
}
//copia el intervalo [i1, i2], de la cadena origen en la cadena destino
//la cadena de destino perderá su contenido original
void StrCopy(AnsiString &D, const AnsiString &S, int i1, int i2)
{
    //el índice i2 no debe ser inferior al índice i1
    if(i2 < i1)
        throw EImproperArgument("index i2 should not be lower index i1");

    //los índices i1 e i2 deben indicar posiciones de la cadena S
    if(i1<1 || S.Length()<i2)
        throw EImproperArgument("índex i1 and i2 should indicate positions of string S");

    D.SetLength(i2-i1+1);
    int j=1;
    for(int i=i1; i<=i2; i++)
        D[j++] = S[i];
}

//sustituye las ocurrencias de una cadena a reemplazar por
//una cadena sustituta
void strreplace(string& str, const string& str_to_replace,
                const string& str_substitute)
{
    size_t pos = 0;
    size_t n = str_to_replace.length();

    //mientras haya posiciones donde buscar
    while(pos < str.length()) {
        //busca la siguiente ocurrencia de la cadena a reemplazar
        pos = str.find(str_to_replace.c_str(), pos, n);

        //si ha encontrado otra ocurrencia
        if(pos < str.length())
            //sustituye la cadena a reemplazar por la cadena sustituta
            str = str.replace(pos, n, str_substitute);
    }
}
//sustituye las ocurrencias de una cadena a reemplazar
//por una cadena sustituta
void StrReplace(AnsiString& S, const AnsiString& S_to_replace,
                const AnsiString& S_substitute)
{
    strreplace(S.str, S_to_replace.str, S_substitute.str);
}
//cuenta las líneas de texto contenidas en una cadena de texto imprimible
//si la cadena String no es imprimible lanza una excepción EImproperArgument
int StrCountLines(const AnsiString &String)
{
    //status:
    //      0: leyendo línea y esperando '\r'
    //      1: '\r' encontrado y esperando '\n'
    char status = 0;

    int i = 1; //inicializa el índice a la primera posición de la cadena
    char c;
    int count; //número de lineas encontradas

    //contabiliza la última línea encontrada
    count = 1;

    //por cada caracter de la cadena
    while(i <= String.Length()) {
        //asigna el caracter indicado para facilitar su acceso
        c = String[i];

        switch(status) {
        case 0: //leyendo línea y esperando '\r'
            if(c == '\r') //si encuentra '\r'
                status = 1; //pasa a estado 1
            break;
        case 1: //'\r' encontrado y esperando '\n'
            if(c != '\n') { //si el que sigue no es '\n'
                status = 0; //vuelve a estado 0
            }
            else { //si el que sigue es '\n'
                count++; //contabiliza la linea
                status = 0; //vuelve al estado 0
            }
            break;
        }
        i++; //indica al próximo caracter
    }

    return count;
}
//determines if there is the substring "\r\n" in the position i of a string
bool thereIsntEndline(const string& str, int i)
{
    if(i<0 || i>str.length())
        throw EImproperArgument("index i should indicates a position of string");

    int remaining = str.length() - 1 - i;

    if(remaining<2 || str[i]!='\r' || str[i+1]!='\n')
        return true;

    return false;
}
//divide una cadena por cada "\r\n" que encuentre
void StrDivideInLines(TStrings *Lines, const AnsiString &S)
{
    if(Lines == NULL)
        throw EImproperArgument("pointerLines shouldpoint to build string list");

    //initialize the output
    Lines->Clear();

    //initialize index to positions of the string
    int i = 1;

    //solve the trivial basic case
    if(i > S.Length())
        return;

    do {
        //actualize index to the first position of the next substring
        int ifirst = i;

        //advances i to the post-last position of the string, or find "\r\n"
        while(i<=S.Length() && thereIsntEndline(S.str, i-1))
            i++;

        //if has reached the post-last position of the string,
        //get the substring in the interval [ifirst, i-1]
        //only if it is not empty
        if(i >= S.Length()) {
            int count = i - ifirst;
            if(count > 0) {
                AnsiString SubString;
                SubString = S.SubString(ifirst, count);
                Lines->Add(SubString);
            }
        }

        //if has found the endline,
        //get the substring in the interval [ifirst, i-1]
        else {
            int count = i - ifirst;
            AnsiString SubString;
            SubString = S.SubString(ifirst, count);
            Lines->Add(SubString);

            //Note that substring [ifirst, i-1] could be empty.

            //advances index i to the first position of the next substring
            //or the post-last position of the string
            i += 2;
        }

    //while there is a remaining position in the string
    } while(i <= S.Length());

    /*    //pointer Lines shall point to built string list
    if(Lines == NULL)
        throw EImproperArgument("pointer Strings shall point to built string list");

    char c;
    char status = 0;

    //status:
    //      0: leyendo línea y esperando '\r'
    //      1: '\r' encontrado y esperando '\n'

    AnsiString S; //cadena tampón
    Lines->Clear(); //reinicializa Lines
    int i = 1; //inicializa el índice a la primera posición de la cadena

    //por cada caracter de la cadena
    while(i <= String.Length()) {
        //asigna el caracter indicado para facilitar su acceso
        c = String[i];

        switch(status) {
        case 0: //leyendo línea y esperando '\r'
            //concatena todo, incluso si es '\r'
            S += c;
            //concatenar '\r' es necesario por si
            //no va seguido de '\n'

            if(c == '\r') //si encuentra '\r'
                status = 1; //pasa a estado 1
            break;
        case 1: //'\r' encontrado y esperando '\n'
            if(c != '\n') { //si el que sigue no es '\n'
                S += c; //lo concatena
                status = 0; //vuelve a estado 0
            }
            else { //si el que sigue es '\n'
                //descarta el '\r' final
                S.SetLength(S.Length() - 1);
                Lines->Add(S); //añade la línea
                S.SetLength(0); //reinicia S
                status = 0; //vuelve al estado 0
            }
            break;
        }
        i++; //indica al próximo caracter
    }

    //añade la última línea
    Lines->Add(S);*/
}
void StrDivideInLines(TStrings& Lines, const AnsiString &S)
{
    //initialize the output
    Lines.Clear();

    //initialize index to positions of the string
    int i = 1;

    //solve the trivial basic case
    if(i > S.Length())
        return;

    do {
        //actualize index to the first position of the next substring
        int ifirst = i;

        //advances i to the post-last position of the string, or find "\r\n"
        while(i<=S.Length() && thereIsntEndline(S.str, i-1))
            i++;

        //if has reached the post-last position of the string,
        //get the substring in the interval [ifirst, i-1]
        //only if it is not empty
        if(i >= S.Length()) {
            int count = i - ifirst;
            if(count > 0) {
                AnsiString SubString;
                SubString = S.SubString(ifirst, count);
                Lines.Add(SubString);
            }
        }

        //if has found the endline,
        //get the substring in the interval [ifirst, i-1]
        else {
            int count = i - ifirst;
            AnsiString SubString;
            SubString = S.SubString(ifirst, count);
            Lines.Add(SubString);

            //Note that substring [ifirst, i-1] could be empty.

            //advances index i to the first position of the next substring
            //or the post-last position of the string
            i += 2;
        }

/*        //search the pos-position of the actual line
        while(i<=S.Length() && S[i]!='\r')
            i++;
        i++;
        if(i <= S.Length()) {
            if(S[i] != '\n')
                throw EImproperArgument("char '\r' should be followed by char '\n'");
            else
                i++;
        }

        //if previously i there isn't "\r\n"
        int count;
        if((i < ifirst+2) || S[i-1]!='\n' || S[i-2]!='\r')
            //add substring [ifirst, i-1]
            count = i - ifirst;
        //else, if previously i there is "\r\n"
        else
            //add substring [ifirst, i-3]
            count = i - ifirst - 2;
        if(count > 0)
            Lines.Add(S.SubString(ifirst, count));
        else
            Lines.Add(AnsiString(""));
*/
    //while there is a remaining position in the string
    } while(i <= S.Length());
}
//divide una cadena en palabras
//obtiene cada palabra hasta cada secuencia de espacios o tabuladores
void StrDivideInWords(TStrings *Words, const AnsiString &String)
{
    //el puntero Words debería apuntar a una lista de cadenas construida
    if(Words == NULL)
        throw EImproperArgument("pointer Words should point to built string list");

    //status:
    //      0: descartando separadores (espacios y tabuladores) y buscando primer caracter de la siguiente palabra
    //      1: leyendo palabra y buscando espacio o final de cadena
    char status = 0;

    int i = 1; //indica a la primera posición de la cadena
    AnsiString word;
    char c;

    //si la posición indica a un caracter de la cadena
    while(i <= String.Length()) {
        //asigna el próximo caracter para facilitar su acceso
        c = String[i];

        //procesa el caracter según el estado
        switch(status) {
        case 0: //buscando primer caracter que no sea espacio ni tabulador
            if(c!=9 && c!=32) {
                word += c;
                status = 1;
            }
            break;
        case 1: //leyendo palabra y buscando espacio o final de palabra
            if(c==9 || c==32)  {
                Words->Add(word);
                word = "";
                status = 0;
            } else
                word += c;
            break;
        }

        //indica a la próxima posición de la cadena
        i++;
    }

    //añade la última palabra leida
    if(word.Length() > 0)
        Words->Add(word);
}
void StrDivideInWords(TStringList *Words, const AnsiString &String)
{
    //el puntero Words debería apuntar a una lista de cadenas construida
    if(Words == NULL)
        throw EImproperArgument("pointer Words should point to built string list");

    StrDivideInWords(&(Words->Strings), String);
}

//separate the words between chars
void StrSplit(TStrings& Strings, const AnsiString& S, char c)
{
    //initialize the list of strings
    Strings.Clear();

    //initialize iprev indicating the first delimiter (the pre-first position)
    int iprev = 0;

    //for each delimiter (S[i]), add to Strings the string in the interval [iprev+1, i-1],
    //and actualice iprev = i.
    for(int i=1; i<=S.Length(); i++) {
        if(S[i] == c) {
            if(i > iprev+1) {
                int count = i - 1 - iprev;
                Strings.Add(S.SubString(iprev+1, count));
            } else
                Strings.Add(AnsiString(""));
            iprev = i;
        }
    }

    //add the string between the last delimiter and poslast position
    if(S.Length() > 0) {
        if(S.Length() > iprev) {
            int count = S.Length() - iprev;
            Strings.Add(S.SubString(iprev+1, count));
        } else
            Strings.Add(AnsiString(""));
    }
}

//elimina los espacios y caracteres de control marginales de una cadena
//la cadena de destino D puede ser la misma cadena fuente S
void StrTrim(AnsiString &D, const AnsiString &S)
{
    //si la cadena está vacía
    if(S.Length() < 1)
        return; //no hace nada

    //busca el último caracter imprimible distinto de espacio
    int i2 = StrPreviusChar(S, S.Length());

    //inicializa la cadena de destino
    D.SetLength(i2);

    //busca el primer caracter imprimible distinto de espacio
    int i1 = StrNextChar(S, 1);

    //inicializa el índice de la cadena de destino
    int j = 1;

    //copia la cadena fuente en la cadena de destino
    //la cadena de destino
    for(int i=i1; i<=i2; i++, j++)
        D[j] = S[i];

    //elimina los caracteres sobrantes de la cadena de destino
    D.SetLength(j-1);
}
//elimina los espacios y caracteres de control marginales de una cadena
AnsiString StrTrim(const AnsiString &S)
{
    AnsiString D;

    //si la cadena está vacía
    if(S.Length() < 1)
        return D; //no hace nada

    //busca el último caracter imprimible distinto de espacio
    int i2 = StrPreviusChar(S, S.Length());

    //inicializa la cadena de destino
    D.SetLength(i2);

    //busca el primer caracter imprimible distinto de espacio
    int i1 = StrNextChar(S, 1);

    //inicializa el índice de la cadena de destino
    int j = 1;

    //copia la cadena fuente en la cadena de destino
    //la cadena de destino
    for(int i=i1; i<=i2; i++, j++)
        D[j] = S[i];

    //elimina los caracteres sobrantes de la cadena de destino
    D.SetLength(j-1);

    return D;
}

//Si la longitud de la cadena es mayor que el número de caracteres indicado,
//sustituye la parte sobrante de la cadena por "...".
//La cadena de destino D, puede ser la misma que la cadena fuente S.
void StrFirstChars(AnsiString &D, const AnsiString &S, int LengthMax)
{
    //el número de caracteres máximo de la cadena resultante debe ser mayor que 3
    if(LengthMax <= 3)
        throw EImproperArgument("maximun number of characters of the resulting string should be upper 3");

    //si la cadena fuente tiene más de LengthMax caracteres
    if(S.Length() > LengthMax) {
        //copia los caracteres de la cadena fuente
        D = S.SubString(1, LengthMax-3);
        //añade los puntos suspensivos
        D += "...";
    } else { //si no
        //asigna la cadena fuente a la cadena destino
        D = S;
    }
}
AnsiString StrFirstChars(const AnsiString &S, int LengthMax)
{
    //el número de caracteres máximo de la cadena resultante debe ser mayor que 3
    if(LengthMax <= 3)
        throw EImproperArgument("maximun number of characters of the resulting string should be upper 3");

    AnsiString D;

    //si la cadena fuente tiene más de LengthMax caracteres
    if(S.Length() > LengthMax) {
        //copia los caracteres de la cadena fuente
        D = S.SubString(1, LengthMax-3);
        //añade los puntos suspensivos
        D += "...";
    } else { //si no
        //asigna la cadena fuente a la cadena destino
        D = S;
    }

    return D; //devuelve la cadena destino
}

//search and return the first non separator char in a text string
int StrSearchFirstNonseparatorChar(const AnsiString& S)
{
    int i = 1;

    if(S.Length() <= 0)
        return i;

    //status:
    //  0: waiting ' ', '\t', '\r' or other
    //  1: '\r' found and waiting '\n'
    //  2: firs nonseparator char found or pos-last position reached
    char status = 0;

    do {
        //assign the indicated char
        char c = S[i];

        //actualize (i, status) according (status, c)
        switch(status) {
        case 0: //waiting ' ', '\t',  '\r', or other
            switch(c) {
            case ' ': case '\t':
                i++;
                break;
            case '\r':
                if(i >= S.Length())
                    throw EImproperArgument("\r should be followed by \n");
                i++;
                status++;
                break;
            default:
                status = 2;
            }
            break;

        case 1: //'\r' found and waiting '\n'
            if(c != '\n')
                throw EImproperArgument("\r should be followed by \n");
            i++;
            status--;
            break;
        }

        //finish if index i indicates the pos-last position
        if(i > S.Length())
            status = 2;

    //while not firs nonseparator char found or pos-last position reached
    } while(status < 2);

    return i;
}

//rellena una cadena con caracteres hasta que alcance la longitud indicada
void StrFill(AnsiString &S, int L, char c)
{
    //la longitud de la cadena L debe ser no negativa
    if(L < 0)
        throw EImproperArgument("string length L should be nonnegative");

    while(S.Length() < L)
        S += c;
}
//insert a char in the indicated position of a text string
//to get the intended length
AnsiString strInsertChar(const AnsiString& S, int length,
                        int i, char c)
{
    //the length to achieve should be upper zero
    if(length <= 0)
        throw EImproperArgument("length to achieve shall be upper zero");

    //index i should indicate a position in the text string S
    if(i<1 || S.Length()<i)
        throw EImproperArgument("index i shall indicate a position of the text string S");

    //clone the text string
    AnsiString S_(S);
    //insert to achieve the intended length
    while(S_.Length() < length)
        S_.Insert(i, c);

    //return the modified string
    return S_;
}

//determina si dos cadenas contienen las mismas palabras
bool StrNotHasSameWords(const AnsiString &S1, const AnsiString &S2)
{
    //ADVERTENCIA: esta función definía TStringList,
    //y mediante autocast se convertía en Strings,
    //al invocar a StrDivideInWords.

    //divide la primera cadena en palabras
    TStrings *Words1 = new TStrings();
    StrDivideInWords(Words1, S1);
    //divide la segunda cadena en palabras
    TStrings *Words2 = new TStrings();
    StrDivideInWords(Words2, S2);

    //determinas si las cadenas tienen el mismo número de palabras
    if(Words1->getCount() != Words2->getCount()) {
        delete Words2;
        delete Words1;
        return true;
    }

    //compara las palabras
    for(int i=0; i<Words1->getCount(); i++)
        if(Words1[i] != Words2[i]) {
            delete Words2;
            delete Words1;
            return true;
        }

    //indica que no se encontraron diferencias
    delete Words2;
    delete Words1;
    return false;
}

//añade la cadena que se indique al comienzo de cada línea de texto
void StrMultiIndent(AnsiString &D, const AnsiString &S, const AnsiString &SubS)
{
    //la cadena de texto SubS debería ser imprimible
    if(StrIsntPrintable(SubS))
        throw EImproperArgument("string SubS should be printable");

    //Si la cadena de texto está vacía se considera
    //que contiene una linea de texto vacía.
    //Incluso una línea vacía debe ser indentada.

    D = SubS;
    int i = 1;
    char c; //S[i]

    //estado de lectura
    //      0: esperando '\r'
    //      1: esperando '\n'
    //      2: cadena terminada
    int status = 1;

    do {
        c = S[i];
        D += c;

        switch(status) {
        case 0: //esperando '\r'
            switch(c) {
            case '\r':
                i++;
                if(i > S.Length())
                    status = 2;
                else
                    status++;
                break;
            default:
                i++;
                if(i > S.Length())
                    status = 2;
            }
            break;
        case 1: //esperando '\n'
            switch(c) {
            case '\n':
                i++;
                D += SubS;
                if(i > S.Length())
                    status = 2;
                else
                    status--;
                break;
            default:
                i++;
                if(i > S.Length())
                    status = 2;
                else
                    status--;
            }
            break;
        }
    } while(status < 2);
}
//añade la cadena que se indique al comienzo de cada línea de texto
AnsiString StrIndent(const AnsiString &S, const AnsiString &SubS)
{
    //la cadena de texto SubS debería ser imprimible
    if(StrIsntPrintable(SubS))
        throw EImproperArgument("string SubS should be printable");

    //Si la cadena de texto está vacía se considera
    //que contiene una linea de texto vacía.
    //Incluso una línea vacía debe ser indentada.

    //Nótese que la cadena detexto SubS puede estar vacía.

    //inserta la primera indentación
    AnsiString D = SubS;
    int i = 1;
    char c; //S[i]

    //Mediante una máquina de estados inserta SubS
    //detrás de cada "\r\n".

    //estado de lectura
    //      0: esperando '\r'
    //      1: esperando '\n'
    //      2: cadena terminada
    int status = 0;

    do {
        //si se ha terminado la cadena
        if(i > S.Length())
            //indica quela cadena se ha terminado
            status = 2;

        //si no, si i está indicando a un nuevo caracter de la cadena S
        else {
            //asigna el caracter indicado para facilitar su acceso
            c = S[i];
            //añade el acaracter ala cadena dedestino
            D += c;
            //indica a la próxima posición de la cadena
            i++;

            //en caso de que el estado sea...
            switch(status) {

            case 0: //esperando '\r'
                if(c == '\r')
                    status++;
                break;

            case 1: //esperando '\n'
                switch(c) {
                case '\n': //si ha encontrado "\r\n"
                    D += SubS; //añade la cadena S
                    status--; //vuelve al estado inicial
                    break;
                default: //si solo ha encontrado '\r' seguido de algún otro caracter distinto de '\n'
                    status--; //vuelve al estado inicial
                }
                break;
            }
        }
        //mientras no haya terminado la cadena
    } while(status < 2);

    return D;
}

//reconoce en una cadena 'r'->real; 'b': binario; 'z'->entero; 'o'->otro
//Devuelve:
//	'r': si el número tiene un punto decimal, aunque se encuentre al final.
//	'b': si no hay punto y todas las cifras se encuentran en [0, 1].
//	'z': si no hay punto y hay alguna cifra mayor que 1.
//	'o': niguna de las anteriores.
char StrFormat(const AnsiString &S)
{
    if(S.Length() == 0)
        return 'o'; //Si ni siquiera es un string vacío ("") tb

    //Si S solo tiene una caracter
    if(S.Length() == 1) {
        if('0'<=S[1] && S[1]<='1') {
            return 'b';
        } else if('2'<=S[1] && S[1]<='9') {
            return 'z';
        } else
            return 'o';
    }

    //Si tiene más de un caracter se obvia el signo '-'
    int i=1; //índice a elementos de S
    if(S[i]=='-')
        i++;

    //Si la primera cifra es un cero deberá ir seguida de '.'
    //ó S deberá terminar
    if(S[i]=='0') {
        if(i!=S.Length()) //si no es el último caracter debe ir seguido de '.'
            if(S[i+1]!='.')
                return 'o';
    } else if(!('1'<=S[i] && S[i]<='9'))
        return 'o';
    i++;
    int findpoint=false;
    if(i<=S.Length()) //si i apunta a un nuevo caracter...
        do {
        if(!findpoint) {
            if(S[i]=='.')
                findpoint = true;
            else if(!('0'<=S[i] && S[i]<='9'))
                return 'o';
            i++;
        } else {
            for(; i<=S.Length(); i++)
                if(!('0'<=S[i] && S[i]<='9'))
                    return 'o';
            i++;
        }
    } while(i<=S.Length());

    if(findpoint)
        return 'r';
    else {
        for(i=1; i<=S.Length(); i++)
            if(S[i]>'1')
                return 'z';

        return 'b';
    }
}
//Limita a n el número de decimales de una cadena que rerpesenta un nº real
void StrCutDecimals(AnsiString &D, const AnsiString &S, int n)
{
    //la cadena S debe representar un número
    if(StrFormat(S)=='o')
        throw EImproperArgument("string S should reprent a number");

    //el número de cifras n no debe ser negativo
    if(n < 0)
        throw EImproperArgument("number figers n should not be negative");

    //Cuenta el nº de caracteres que debe tener la cadena
    //para tener n decimales
    int i; //número de caracteres
    if(StrFormat(S)=='r') { //si es real tendrá un punto decimal
        //Cuenta el nº de caracteres hasta el punto decimal, incluido este
        i = 0;
        do {
            i++;
        } while(S[i]!='.');
        //Ahora i apunta al punto decimal
        if(n>0 && i<S.Length()) {
            int j=0; //nº de decimales que se incorporarán
            do {
                i++; j++;
            }while(i<S.Length() && j<n);
        }
        //Ahora i apunta a la última cifra que se quiere preservar
    } else //no tiene punto decimal
        i = S.Length();

    //Copia la parte de la cadena que se quiere preservar
    D.SetLength(i);
    for(int j=1; j<=i; j++)
        D[j] = S[j];
}

/*comentado el 12/07/2104
//Imprime la etiqueta S en el Canvas, en la posición y distancias p y d,
//relativas a las coordenadas x e y.
//Parámetros de paso:
//	Canvas: lienzo en el que se imprimirá la cadena.
//	S: cadena de caracteres a imprimir.
//	x, y: coordenadas en pixels del punto de referencia en el PaintBox.
//	p: posición relativas al punto de referencia:
//		0=centrado; 1=N; 2=NE; 3=E; ...; 8=NW.
//	d: distancia más corta de la etiqueta al punto de referencia en pixels.
void StrPaint(TCanvas *Canvas, const AnsiString &S,
    int x, int y, char p, int d)
{
        //la posición relativa p debe estar en [0, 8]
    if(p<0 || 8<p)
                throw EImproperArgument("relative position p should be in [0, 8]");

        POINT TopLeft; //esquina sup izda de la cadena
        int width = Canvas->TextWidth(S); //anchura en píx de la cadena
        int height = Canvas->TextHeight("O"); //altura en píx de la cadena
        switch(p) {
                case 0: //centrado
            TopLeft.x = x - width/2;
            TopLeft.y = y - height/2;
                    break;
                case 1:
            TopLeft.x = x - width/2;
            TopLeft.y = y - d - height;
                    break;
                case 2:
            TopLeft.x = x + d;
            TopLeft.y = y - d - height;
                    break;
                case 3:
            TopLeft.x = x + d;
            TopLeft.y = y - height/2;
                    break;
                case 4:
            TopLeft.x = x + d;
            TopLeft.y = y + d;
                    break;
                case 5:
            TopLeft.x = x - width/2;
            TopLeft.y = y + d;
                    break;
                case 6:
            TopLeft.x = x - d - width;
            TopLeft.y = y + d;
                    break;
                case 7:
            TopLeft.x = x - d - width;
            TopLeft.y = y - height/2;
                    break;
                case 8:
            TopLeft.x = x - d - width;
            TopLeft.y = y - d - height;
                    break;
        } //switch(p)

    Canvas->TextOut(TopLeft.x, TopLeft.y, S);
}
*/
//---------------------------------------------------------------------------
//FORMATOS DE PRESENTACIÓN:

//inserta un salto de linea y un tabulador delante de cada elemento de una lista
//delimitada por {} y elementos separados por ','
void StrListToColumnForm(AnsiString &D, const AnsiString &O, int &i)
{
/*
    //la cadena de texto O debería ser imprimible
    if(StrIsntPrintable(O))
        throw EImproperArgument("string O should be printable");

    D = "";

    //estado de lectura
    //      0: esperando '{'
    //      1: esperando ' ' o primer caracter de palabra
    //      2: leyendo palabra y esperando ' ', ',' o '}'
    //      3: lista  con éxito
    int status = 0;

    char c; //O[i]

    do {
        c = O[i];
    } while(status < 3);
  */
}

//---------------------------------------------------------------------------

} //namespace Strings

//---------------------------------------------------------------------------
