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
/// @file Instruction.h
/// @brief instruction of a RP
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "Vector.h"

//---------------------------------------------------------------------------

using namespace Lists;

//espacio de nombres de modelos
namespace Models {

//###########################################################################
//TInstruction
//###########################################################################

//Se utilizará el término "instrucción" para referirse a las órdenes
//que se dan a los posicionadores, para evitar confundirlo con el término
//"comando" que se referirá a las órdenes que se dan al proceso a través de
//la consola del programa, de cara a mantener la cogruencia con la denominación
//intérprete de comandos (y no intérprete de instrucciones).
//El término instrucción se asemeja al empleado en procesadores, de modo
//que el posicionador puede entenderse como un procesador con un repertorio de
//instrucciones que es capaz de procesar.

//Semántica:
//      el nombre de instrucción debe ser conocido;
//      número de argumentos de 0 a 2;
//      argumentos reales (pudiendo ser negativos);

//Al definir argumentos reales (incluso negativos) se generaliza para el caso
//de ejes de movimiento no discretizado y posicionadores con instrucciones
//que aceptan argumentos negativos.

//clase instrucción de posicionador
class TInstruction {
    AnsiString p_Name;

public:
    //PROPIEDADES DE LA INTRUCCIÓN:

    //nombre de la instruccion
    //debe ser un nombre conocido:
    //      "": instrucción vacía; sin argumentos;
    //      "MA": establece modo asíncrono; sin argumentos;
    //      "MS": establece modo síncrono; sin argumentos;
    //      "M1": programa o desplaza el eje 1; con un argumento;
    //      "M2": programa o desplaza el eje 2; con un argumento;
    //      "MM": programa o desplaza ambos ejes; con dos argumentos;
    //      "ST": inicia el desplazamiento programado;
    //      "SP": detine el desplazamiento (si se está realizando).
    //valor por defecto: ""
    AnsiString getName(void) const {return p_Name;} void setName(AnsiString);
    //argumentos del comando
    //un argumento puede tener cualquier valor (incluso negativo)
    //valor por defecto: {}
    TVector<double> Args;

    //------------------------------------------------------------------
    //NOTA: estrictamente hablando cuando un posicioandor recibe
    //una  instrucción desconocida, debería responder con un mensaje
    //de error, indicando su desconocimiento, ya que ninguna instrucción
    //desconocida debería ser enviada por el sistema, pues consume resursos
    //y se trata de un error.
    //
    //Hay dos clases de instrucciones desconocidas:
    //      las instrucciónes vacías;
    //      las instrucciónes cuyo nombre es desconocido.
    //
    //Incluir la instrucción vacía en el dominio de definición del nombre
    //de instrucción, permite detectar cuando la instrucción
    //todavía no ha sido escrita.
    //
    //La instrucción vacía se codificaría mediante la cadena de bytes vacia
    //y sería enviada mediante una trama vacía que sería decodificada
    //obteniendo la misma instrucción vacía, la cual podriamos estar
    //tentados de ignorarla.
    //
    //Pero ni siquiera la instrucción vacía debería ser enviada por
    //el bus CAN, ni asignada a los conroladores, pues ha sido concebida
    //para evitar precisamente esos errores.
    //
    //Se recuerda que en realidad, actualmente los controladores han sido
    //programados para ignorar cualquier instrucción desconocida o vacía.
    //------------------------------------------------------------------

    //PROPIEDADES EN FORMATO TEXTO:

    //El nombre del comando ya está en formato texto.

    //argumentos en formato texto
    //      Args.getItemsText(i)

    //instrucción en formato de linea de texto
    //      "Name Arg1 Arg2..."
    AnsiString getText(void) const; void setText(const AnsiString&);
    //instrucción en formato de linea de texto para comentario
    //      "r1 Arg1"           (Name == M1)
    //      "r2 Arg2"           (Name == M2)
    //      "r1 Arg1, r2 Arg2"  (Name == MM)
    AnsiString getTextForComment(void) const;

    //MÉTODOS ESTÁTICOS:

    //imprime una instrucción en una cadena de texto
    static void  Print(AnsiString &S, const TInstruction *I);
    //lee una instrucción en una cadena de texto
    static void  Read(TInstruction *I, const AnsiString &S, int &i);

    //MÉTODOS PÚBLICOS:

    //construye una instrucción de posicionador
    TInstruction(void);

    //asigna una instrucción
    TInstruction& operator=(const TInstruction&);

    //determines if an instruction is different to this instruction
    bool operator!=(const TInstruction&) const;
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // INSTRUCTION_H
