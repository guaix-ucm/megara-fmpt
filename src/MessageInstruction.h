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
//Archivo: MessageInstruction.h
//Contenido: mensaje de instrucción
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef MESSAGEINSTRUCTION_H
#define MESSAGEINSTRUCTION_H

#include "Vector.h"
#include "Instruction.h"

//---------------------------------------------------------------------------

using namespace Lists;

//espacio de nombres de modelos
namespace Models {

//--------------------------------------------------------------------------
//TMessageInstruction:
//--------------------------------------------------------------------------

//mensaje de instrucción
class TMessageInstruction {
        int __Id;

public:
        //PROPIEDADES DEL MENSAJE:

        //número de identificación del puerto CAN destinatario del mensaje
        //valor por defecto: 0
        int getId(void) const {return __Id;} void setId(int);

        //instrucción del mensaje
        //valor por defecto: el de la instrucción
        TInstruction Instruction;

        //Las instrucciones deben escribirse directamente en la propiedad
        //Instruction, primero debe asignarse el nombre de instrucción
        //y a continuación los argumentos. P.e:
        //      Instruction.Name = "G1";
        //      Instruction.Args.First = p1;

        //PROPIEDADES EN FORMATO TEXTO:

        //número de identificación del puerto CAN destinatario del mensaje
        //en formato texto
        //debe ser no negativo
        //valor por defecto: "0"
        AnsiString getIdText(void) const; void setIdText(const AnsiString&);

        //instrucción del mensaje
        //en formato texto:
        //      Instruction.Text

        //Línea de texto con las propiedades del mensaje en formato texto.
        //Si instenta asignar un mensaje inválido
        //lanza una excepcion EImproperArgument.
        //Formato:
        //      IdText+": "+Instruction.Text
        AnsiString getText(void) const; void setText(const AnsiString&);

        //MÉTODOS ESTÁTICOS:

        //compara los identificadores de dos mensajes de instrucción
        static int  CompareIds(const TMessageInstruction *MI1,
                const TMessageInstruction *MI2);
        //imprime un mensaje de instrucción en una cadena
        static void  Print(AnsiString &S,
                const TMessageInstruction *MI);
        //lee un mensaje de itnrucción en una cadena de texto
        static void  Read(TMessageInstruction *MI,
                const AnsiString &S, int &i);

        //MÉTODOS PÚBLICOS:

        //construye un mensaje con los valores por defecto
        TMessageInstruction(void);
        //construye un mensaje con los valores indicados
        TMessageInstruction(int _Id, AnsiString InstructionText);
        //clona un mensaje
        TMessageInstruction(TMessageInstruction *);

        //copia las propiedades de un mensaje
        void Copy(const TMessageInstruction *MI);
        //asigna las propiedades de un mensaje
        TMessageInstruction& operator=(const TMessageInstruction &MI);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // MESSAGEINSTRUCTION_H
