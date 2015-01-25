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
//Archivo: TextFile.cpp
//Contenido: funciones de cálculo simbólico
//Última actualización: 25/04/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "TextFile.h"
#include "Strings.h"
#include "Exceptions.h"

#include <dirent.h> //opendir, readdir, rewinddir, closedir

//---------------------------------------------------------------------------

//espacio de nombres de funciones y clases de objetos
//relacionados con cadenas de texto
namespace Strings {

//---------------------------------------------------------------------------
//MÉTODOS:

//abre el archivo de nombre FileName en el modo indicado
//si no lo especifica solo construye el TextFile
//si no lo puede abrir lanza una excepción ECantComplete
TTextFile::TTextFile(const char *FileName, const char *mode) :
    IsOpen(false) //inicializa a su valor por defecto
{
    //debe especificar ambos argumentos o ninguno
    if(FileName!=NULL && mode==NULL)
        throw EImproperArgument("must especify also the opening mode");

    //si ha especificado ambos intenta abrir el archivo
    if(FileName!=NULL && mode!=NULL) {
        try {
            //intenta abrir el archivo en el modo indicado
            Open(FileName, mode);
        }
        catch(...) {
            //si no puede abrirlo relanza la excepción
            throw;
        }
    }
}
//si el archivo no está cerrado lanza una excepción EImproperCall
TTextFile::~TTextFile()
{
    /*	//es obligatorio haber cerrado el archivo antes de destruir
    if(IsOpen)
            throw EImproperCall("must close before destroy this TextFile");
  */
    //cierra el archivo antes de destruirlo
    if(IsOpen)
        Close();
}

//---------------------------------------------------------------------------
//MÉTODOS DE APERTURA Y CIERRE:

//abre un archivo en el modo especificado
//si no puede abrirlo lanza una excepción ECantComplete
void TTextFile::Open(const char *_FileName, const char *mode)
{
    //el archivo debe estar cerrado
    if(IsOpen)
        throw EImproperCall("file should be close");

    //intenta abrir el archivo
    f=fopen(_FileName, mode);

    //el archivo debe poder abrirse
    if(f == NULL)
        throw ECantComplete(AnsiString("can't open file: ")+AnsiString(_FileName));

    //indica que el archivo FileName está abierto
    IsOpen = true;
    FileName = AnsiString(_FileName);
}
//cierra un archivo
//si ya está cerrado lanza una excepción EImproperCall
//si no tiene éxito lanza una excepción ECantComplete
void TTextFile::Close(void)
{
    //debe haber un archivo abierto
    if(!IsOpen)
        throw EImproperCall("should be an open file");

    //vacia el buffer
    fflush(f);

    //ADVERTENCIA: no vaciar buiffer de la corriente
    //antes de cerrar el archivo puede provocar
    //la pérdida de datos.

    //debe poder cerrar el archivo
    if(fclose(f) != 0)
        throw EImproperCall(AnsiString("can't close file: ")+FileName);

    //indica que no hay un archivo abierto
    IsOpen = false;
}

//--------------------------------------------------------------------------
//MÉTODOS DE LECTURA Y ESCRITURA DE TEXTO:

//lee todo el contenido del archivo
//si no puede realizar la operación lanza una excepción
void TTextFile::Read(AnsiString &S)
{
    //debe haber un archivo abierto
    if(!IsOpen)
        throw EImproperCall("file should be open");

    //ADVERTENCIA: las funciones de lectura de archivo (como scanf)
    //se saltarán los '\r' leyendo únicamente '\n' al final de cada línea.

    //NOTA: se usará fgetc en vez de scanf, porque scanf no devuelve
    //EOF al llegar al final del archivo.

    //obtiene caracteres de f en S
    S = "";
    char ch;
    ch = (char)fgetc(f);
    while(ch != EOF) {
        //inserta '\r' donde encuentre '\n' para compensar
        //su ausencia debida a las funciones de lectura
        if(ch == '\n')
            S += '\r';

        S += AnsiString(ch);
        ch = (char)fgetc(f);
    }
}

//escribe todo el contenido del archivo
//si no puede realizar la operación lanza una excepción
void TTextFile::Print(const AnsiString &S)
{
    //debe haber un archivo abierto
    if(!IsOpen)
        throw EImproperCall("file should be open");

    char ch;

    //obtiene caracteres de f en S
    for(int i=1; i<=S.Length(); i++) {
        ch = S[i];
        if(ch != '\r')
            fprintf(f, "%c", ch);

        //NOTAL: '\r' no se imprimirá en el archivo, ya que:
        //      '\r' no es leido con la función de lectura
        //      lo cual indica que '\n' hace la función de "\r\n'
        //      por si solo.
    }
}

//--------------------------------------------------------------------------
//MÉTODOS DE LECTURA Y ESCRITURA DE SENTENCIAS:

//lee de f una cadena hasta que encuentra ';' o EOF
//si encuentra ';' devuelve true y si encuentra EOF devuelve false.
bool TTextFile::ReadSentence(AnsiString &S)
{
    //debe haber un archivo abierto
    if(!IsOpen)
        throw EImproperCall("file should be open");

    //copia caracteres de f a S
    S = "";
    char ch = (char)fgetc(f);
    while(ch!=EOF && ch!=';') {
        S += AnsiString(ch);
        ch = (char)fgetc(f);
    }
    StrTrim(S, S); //descarta los márgenes

    //puede encontrar ';' o EOF
    if(ch == EOF)
        return false;
    else
        return true;
}
//busca la clave Ident partir de la posición actual y
//devuelve el texto asociado
//si no hay más sentencias lanza una excepción EComplete y
//si encuentra otra cosa lanza una excepción ECantComplete
void TTextFile::ReadAssign(char *Ident, AnsiString &Value)
{
    //intenta leer la próxima sentencia
    AnsiString S;
    if(ReadSentence(S)) { //encontró ';'
        //intenta descomponer la sentencia
        AnsiString S1, S2;
        if(StrSplitAssign(S1, S2, S)) { //si encontró '='
            //descarta los espacios y caracteres de control marginales
            StrTrim(S1, S1);
            StrTrim(S2, S2);
            if(S1 == AnsiString(Ident)) //si es la clave esperada
                Value = S2; //asigna el texto asociado
            else //la clave no es la esperada
                //indica que la clave no es la esperada
                throw ECantComplete(AnsiString("\"")+S1+AnsiString("\" debería ser \"")+
                                    AnsiString(Ident)+AnsiString("\""));
        } else //no es una asignación
            throw ECantComplete(AnsiString("\"")+S+AnsiString("\" ")+
                                AnsiString("no es una sentencia de asignación"));

    } else //encontró EOF
        if(S.Length() < 1)
            //indica que se acabó el archivo
            throw EComplete(AnsiString("no encuentra la clave: ")+
                             AnsiString(Ident)+AnsiString("\r\narchivo termiando"));
        else
            //indica que encontró basura
            throw ECantComplete(AnsiString("\"")+S+AnsiString("\" ")+
                                 AnsiString("no es una sentencia"));
}
//almacena la cadena Value asociada a la clave Ident
//si no tiene éxito lanza una exceptición ECantComplete
void TTextFile::PrintAssign(char *Ident, const AnsiString &Value)
{
    //debe haber un archivo abierto
    if(!IsOpen)
        throw EImproperCall("there isn't a file open");

    //intenta escribir en el archivo
    if(fprintf(f, "%s=%s;\n", Ident, Value.c_str()) < 1)
        throw ECantComplete(AnsiString("cant write in file ")+FileName);
}

//---------------------------------------------------------------------------

//lee un subdirectorio en una lista de cadenas de texto
void ReadDir(TStringList *StringList, const AnsiString &Dir)
{
    //el puntero StringListdebeapuntar a una lista de cadenas de texto construida
    if(StringList == NULL)
        throw EImproperArgument("pointer StringList shouldpoint to built string list");

    DIR *dir;

    if((dir = opendir(Dir.c_str())) == NULL)
        throw Exception(AnsiString("can't open the directory ")+Dir);

    struct dirent *ent;

    while((ent = readdir(dir)) != NULL)
        StringList->Add(AnsiString(ent->d_name));

    rewinddir(dir);

    if(closedir(dir) != 0)
        throw Exception(AnsiString("can't close the directory ")+Dir);
}

//escribe una cadena de texto en un archivo
void StrReadFromFile(AnsiString &S, const AnsiString &FileName)
{
    TTextFile TF;

    try {
        //abre el archivo para lectura
        TF.Open(FileName.c_str(), (char*)"r");

        //lee el archivo en la cadena
        TF.Read(S);
        //reemplaza las cadenas "\r\r\n" por cadenas "\r\n"
        strreplace(S.str, "\r\r\n", "\r\n");

        //cierra el archivo
        TF.Close();
    }
    catch(...) { //si falla
        //asegura el cierre del archivo para evitar la pérdida de datos
        if(TF.getIsOpen())
            TF.Close();

        //relanza la excepción que deberá ser
        //capturada en la secuencia de guardado
        throw;
    }
}
//escribe una cadena de texto en un archivo
void StrWriteToFile(const AnsiString &FileName, const AnsiString &S)
{
    TTextFile TF;

    try {
        //abre el archivo para escritura
        TF.Open(FileName.c_str(), (char*)"w");

        //imprime la cadena en el archivo
        TF.Print(S);

        //cierra el archivo
        TF.Close();
    }
    catch(Exception &E) { //si falla
        //asegura el cierre del archivo para evitar la pérdida de datos
        if(TF.getIsOpen())
            TF.Close();

        //relanza la excepción que deberá ser
        //capturada en la secuencia de guardado
        throw Exception(E);
    }
}
//añade una cadena de texto a un archivo
void StrAddToFile(const AnsiString &FileName, const AnsiString &S)
{
    TTextFile TF;

    try {
        //abre el archivo para adición
        TF.Open(FileName.c_str(), (char*)"a+");

        //imprime la cadena en el archivo
        TF.Print(S);

        //cierra el archivo
        TF.Close();
    }
    catch(Exception &E) { //si falla
        //asegura el cierre del archivo para evitar la pérdida de datos
        if(TF.getIsOpen())
            TF.Close();

        //relanza la excepción que deberá ser
        //capturada en la secuencia de guardado
        throw Exception(E);
    }
}

//---------------------------------------------------------------------------

} //namespace Strings

//---------------------------------------------------------------------------

