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
//Archivo: TextFile.h
//Contenido: funciones de cálculo simbólico
//Última actualización: 25/04/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef TEXTFILE_H
#define TEXTFILE_H

#include "vclemu.h" //AnsiString

#include <stdio.h>  //FILE

//---------------------------------------------------------------------------

//espacio de nombres de funciones y clases de objetos
//relacionados con cadenas de texto
namespace Strings {

//---------------------------------------------------------------------------

//class TtextFile {...}; es similar a TRegIniFile, pero controlando las
//situaciones de excepción en los procesos de apertura, lectura y cierre;
//está orientado a la lectura/escritura de sentencias separadas con el
//carácter delimitador ';' permitiendo la ocupación de más de una línea por
//sentencia; además facilita la lectura y escritura directa de sentencias de
//asignación, compuestas de dos miembros (cadenas de caracteres alfanuméricos
//sin espacios) separados por el operador de asignación (signo '=').

//clase archivo de texto
class TTextFile {
    FILE *f; //corriente intermediaria con los archivo de texto

    AnsiString FileName;
    bool IsOpen;

    /*	//Segrega en Ident la subcadena que precede al primer '=' en S
    //y en Value la subcadena que le sucede; si tiene éxito devuelve true
    //y si S no contiene un caracter '=' devuelve false
    friend bool StrSplitAssign (AnsiString &Ident, AnsiString &Value,
            const AnsiString &S);
                                     */
public:
    //PROPIEDADES DE ESTADO:

    //nombre adscrito al archivo abierto
    //valor por defecto: FileName
    AnsiString getFileName(void) const {return FileName;}
    //indica el estado de apertura de la corriente de texto
    //valor por defecto: falso
    bool getIsOpen(void) const {return IsOpen;}

    //MÉTODOS PÚBLICOS:

    //abre el archivo de nombre FileName en el modo indicado
    //si no lo especifica solo construye el TextFile
    //si no lo puede abrir lanza una excepción ECantComplete
    TTextFile(char *FileName=NULL, char *mode=NULL);
    //si el archivo no está cerrado lanza una excepción EImproperCall
    ~TTextFile();

    //MÉTODOS DE APERTURA Y CIERRE:

    //abre un archivo en el modo especificado
    //si no puede abrirlo lanza una excepción ECantComplete
    void Open(const char *FileName, const char *mode);
    //cierra un archivo
    //si ya está cerrado lanza una exceSción EImproperCall
    void Close(void);

    //MÉTODOS DE LECTURA Y ESCRITURA DE TEXTO:

    //lee todo el contenido del archivo
    //si no puede realizar la operación lanza una excepción ECantComplete
    void Read(AnsiString &S);

    //escribe todo el contenido del archivo
    //si no puede realizar la operación lanza una excepción ECantComplete
    void Print(const AnsiString &S);

    //MÉTODOS DE LECTURA Y ESCRITURA DE SENTENCIAS:

    //lee de f una cadena hasta que encuentra ';' o EOF
    //si encuentra ';' devuelve true y si encuentra EOF devuelve false.
    bool ReadSentence(AnsiString &S);

    //busca la clave Ident partir de la posición actual y
    //devuelve el texto asociado
    //si no hay más sentencias lanza una excepción EComplete y
    //si encuentra otra cosa lanza una excepción ECantComplete
    void ReadAssign(char *Ident, AnsiString &Value);
    //almacena la cadena Value asociada a la clave Ident
    //si no tiene éxito lanza una exceptición ECantComplete
    void PrintAssign(char *Ident, const AnsiString &Value);

    //SOBRE EL USO DE LOS MÉTODOS DE SENTENCIAS: es preferible
    //la implementación de un procedimiento que permita leer y escribir
    //las asignaciones de un objeto en formato texto y emplear
    //los métodos de lectura y scritura directa
};

//NOTA: definir métodos de archivo en los objetos tiene las siguientes
//ventajas:
//      1. Se puede indicar el nombre del archivo donde se encuentran
// los errores.
//      2. puede hacerse un acceso directo a las sentencias de archivo
// mejorando la eficiencia.
//Por eso, sin menoscabo de definir métodos de texto,
//es buena idea definir tambien métodos de archivo.
//No obstante es preferible definir primero los métodos de texto,
//ya que los métodos de archivo no son imprescindibles,
//pues pueden ser suplidos mediante las siguientes funciones,
//pasandole las propiedades de texto de los objetos:

//lee un subdirectorio en una lista de cadenas de texto
void ReadDir(TStringList *StringList, const AnsiString &Dir);

//lee una cadena de texto de un archivo
void StrReadFromFile(AnsiString &S, const AnsiString &FileName);
//escribe una cadena de texto en un archivo
void StrWriteToFile(const AnsiString &FileName, const AnsiString &S);
//añade una cadena de texto a un archivo
void StrAddToFile(const AnsiString &FileName, const AnsiString &S);

//---------------------------------------------------------------------------

} //namespace Strings

//---------------------------------------------------------------------------
#endif // TEXTFILE_H
