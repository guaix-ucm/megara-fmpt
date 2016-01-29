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
//Archivo: Strings.h
//Contenido: funciones con cadenas de texto
//Última actualización: 22/04/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef STRINGS_H
#define STRINGS_H

#include "vclemu.h" //AnsiString

//---------------------------------------------------------------------------

//espacio de nombres de funciones y clases de objetos
//relacionados con cadenas de texto
namespace Strings {

//---------------------------------------------------------------------------

//caracteres imprimibles de ISO8859-1 (ISO Latin 1 ó Alfabeto Latino nº1):
//      anglosajones: [32, 126] (los mismos que en código ASCII)
//      latinos: [160, 255] (lo mismos que en unicode)

//La función isprint(c) determina si c está en [32, 126]

//determina si un caracter ISO8859-1 es imprimible latino
bool islatinprint(unsigned char c);// {if(c<160) return false; else return true;}

//caracteres separadores de ISO8859-1:
//Dec   Hex     Abrev   Repr    AT      Descripción
//9     09      HT      HT      ^I      Tabulación horizontal
//10    0A      LF      LF      ^J      Line feed (salto de línea)
//11    0B      VT      YT      ^K      Tabulación Vertical
//12    0C      FF      FF      ^L      Form feed (salto de página)
//13    0D      CR      CR      ^M      Carriage return (retorno de carro)
//28    1C      FS      FS      ^\      File Separator
//29    1D      GS      GS      ^]      Group Separator
//30    1E      RS      RS      ^^      Record Separator
//31    1F      US      US      ^_      Unit Separator
//32    20                              espacio
//160   A0                              ESPACIO DE "NO RUPTURA"

//caracteres separadores de ISO8859-1
//que usualmente forman parte de un docuemnto de texto:
//Dec   Hex     C. de Form. C   Descripción
//9     09      '       '       Tabulación horizontal
//10    0A      '\n'            Line feed (salto de línea)
//13    0D      '\r'            Carriage return (retorno de carro)
//32    20      ' '             espacio

//el caracter de salto de linea y retorno de carro siempre aparecen
//juntos en un documento de texto escrito en el siguiente orden: "\r\n"

//determina si la cadena contiene un separador de los usualmente escritos
bool StrIsUsualSeparator(AnsiString &S);/* {if(S==AnsiString(char(9)) ||
        S==AnsiString(char(32)) || S=="\r\n") return true; else return false;}*/

//en las siguientes funciones se usa el término imprimible para
//referirse a un caracter imprimible anglosajón o imprimible latino

//busca en S, a partir de i, el próximo caracter imprimible distinto de espacio
//si no lo encuentra devuelve la posúltima posición
int StrNextChar(const AnsiString &S, int i);
//busca en S, a partir de i, el caracter previo imprimible distnto de espacio
//si no lo encuentrsa devuelve la preprimera posición
int StrPreviusChar(const AnsiString &S, int i);
//busca en S la primera ocurrencia de c a partir de i
int StrSearch(const AnsiString &S, char c, int i);

//busca en str, a partir de i, el próximo caracter imprimible distinto de espacio
//si no lo encuentra devuelve la posúltima posición
int strNextChar(const string &str, int i);
//busca en str, a partir de i, el caracter previo imprimible distnto de espacio
//si no lo encuentrsa devuelve la preprimera posición
int strPreviusChar(const string &str, int i);

//invierte el orden de los elementos de una cadena
void StrFlip(AnsiString &S);
//determina si una cadena contiene solo caracteres imprimibles
//y delimitadores usuales
bool StrIsPrintable(const AnsiString &S);
//determina si una cadena contiene caracteres no imprimibles
//ni delimitadores usuales
bool StrIsntPrintable(const AnsiString &S);
//determina si una cadena no está entre comillas simples
bool StrIsntString(const AnsiString &S);
//sustituye las secuencias de espacios y tabuladores por un solo espacio
//AnsiString& StrTighten(AnsiString &S);
//Segrega en Value la subcadena que precede al primer '=' en S
//y en Value la subcadena que le sucede; si tiene éxito devuelve true
//y si S no contiene un caracter '=' devuelve false
bool StrSplitAssign(AnsiString &Ident, AnsiString &Value, const AnsiString &S);
//separa la última extensión de un nombre de archivo
//si no tiene extensión devuelve la cadena vacía
AnsiString StrExtension(const AnsiString &filename);
//copia el intervalo [i1, i2], de la cadena origen en la cadena destino
//la cadena de destino perderá su contenido original
void StrCopy(AnsiString &D, const AnsiString &S, int i1, int i2);

//sustituye las ocurrencias de una cadena a reemplazar por
//una cadena sustituta
void strreplace(string& str, const string& str_to_replace,
                const string& str_substitute);
//sustituye las ocurrencias de una cadena a reemplazar
//por una cadena sustituta
void StrReplace(AnsiString& S, const AnsiString& S_to_replace,
                const AnsiString& S_substitute);
//cuenta las líneas de texto contenidas en una cadena de texto imprimible
//si la cadena String no es imprimible lanza una excepción EImproperArgument
int StrCountLines(const AnsiString &String);
//determines if there is the substring "\r\n" in the position i of a string
bool thereIsntEndline(const string& str, int i);
//divide una cadena por cada "\r\n" que encuentre
void StrDivideInLines(TStrings *Lines, const AnsiString &S);
void StrDivideInLines(TStrings& Lines, const AnsiString &S);
//divide una cadena en palabras
//obtiene cada palabra hasta cada secuencia de espacios o tabuladores
void StrDivideInWords(TStrings *Words, const AnsiString &String);
void StrDivideInWords(TStringList *Words, const AnsiString &String);

//un párrafo puede dividirse en líneas mediante TStrings::SetText(String)
//este método es heredado por la clase hija TStringList
//como es lógico es preferible el uso de dicho método a la función
//StrDivideInLines(Lines, String)

//separate the words between chars
void StrSplit(TStrings&, const AnsiString&, char c);

//elimina los espacios y caracteres de control marginales de una cadena
//la cadena de destino D puede ser la misma cadena fuente S
void StrTrim(AnsiString &D, const AnsiString &S);
//elimina los espacios y caracteres de control marginales de una cadena
AnsiString StrTrim(const AnsiString &S);

//la función Trim(const AnsiString &) elimina los espacios marginales
//de una cadena y quizás también algunos caracteres de control
//sin embargo no elimina al menos el caracter '\n'
//por eso se define la función StrTrim(AnsiString &) que
//si elimina todos los caracteres de control marginales

//elimina los espacios y caracteres de control marginales de una cadena
//la cadena de destino dest puede ser la misma cadena fuente src
void strTrim(AnsiString &des, const string &src);
//elimina los espacios y caracteres de control marginales de una cadena
AnsiString strTrim(const string &src);

//Si la longitud de la cadena es mayor que el número de caracteres indicado,
//sustituye la parte sobrante de la cadena por "...".
//La cadena de destino D, puede ser la misma que la cadena fuente S.
void StrFirstChars(AnsiString &D, const AnsiString &S, int LengthMax=15);
AnsiString StrFirstChars(const AnsiString &S, int LengthMax=15);

//search and return the index to first non separator char in a text string
int StrSearchFirstNonseparatorChar(const AnsiString&);
//search and return the first non separator char in a text string
char strFirstNonseparatorChar(const string&);

//rellena una cadena con caracteres hasta que alcance la longitud indicada
void StrFill(AnsiString &S, int length, char c);
//insert a char in the indicated position of a text string
//to get the intended length
AnsiString strInsertChar(const AnsiString& S, int length,
                        int i=1, char c='0');

//ERROR: usar en StrFill la 'L' como nombre de variable provoca el error:
//      Undefined symbol 'AnsiString'

//determina si dos cadenas contienen las mismas palabras
bool StrNotHasSameWords(const AnsiString &S1, const AnsiString &S2);

//añade la cadena que se indique al comienzo de cada línea de texto
void StrMultiIndent(AnsiString &D, const AnsiString &S,
        const AnsiString &SubS="    ");
//añade la cadena que se indique al comienzo de cada línea de texto
AnsiString StrIndent(const AnsiString &S, const AnsiString &SubS="    ");

//Las siguientes funciones representan con un punto decimal (no una coma)
//los números reales en formato cadena.
//Conviene usar un punto y no una coma para poder representar
//números reales separados por comas.
//Para usar estas funciones debe configurarse el sistema realizando
//la siguiente asignación:
//      DecimalSeparator = '.'
//En Windows XP el valor por defecto de DecimalSeparator es una coma.

//reconoce formatos numéricos en una cadena:
//      'r'->real; 'b': binario; 'z'->entero; 'o'->otro
//Devuelve:
//	'r': si el número tiene un punto decimal, aunque se encuentre al final.
//	'b': si no hay punto y todas las cifras se encuentran en [0, 1].
//	'z': si no hay punto y hay alguna cifra mayor que 1.
//	'o': niguna de las anteriores.
char StrFormat(const AnsiString &S);
//Limita a n el número de decimales de una cadena que rerpesenta un nº real
void StrCutDecimals(AnsiString &D, const AnsiString &S, int n);

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
    int x, int y, char p, int d);
*/
//---------------------------------------------------------------------------
//FORMATOS DE PRESENTACIÓN:

//En congruencia con al notación de MATHEMATICA:
//      TableForm
//      MatrixForm
//se empleará el sufijo 'Form' para referirse a funcioens que
//actúa sobre el formato de presentación de cadenas de texto.

//inserta un salto de linea y un tabulador delante de cada elemento de una lista
//delimitada por {} y elementos separados por ','
//  void StrListToColumnForm(AnsiString &D, const AnsiString &S);
//ERROR: esta función está por definir. No usar.

//---------------------------------------------------------------------------

} //namespace Strings

//---------------------------------------------------------------------------
#endif //STRINGS_H
