//---------------------------------------------------------------------------
//Archivo: MessageInstruction.h
//Contenido: mensaje de instrucción
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef MESSAGEINSTRUCTION_H
#define MESSAGEINSTRUCTION_H

#include "..\3_Lists\Vector.h"
#include "..\B_RoboticPositioner\Instruction.h"

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
        static int __fastcall CompareIds(const TMessageInstruction *MI1,
                const TMessageInstruction *MI2);
        //imprime un mensaje de instrucción en una cadena
        static void __fastcall Print(AnsiString &S,
                const TMessageInstruction *MI);
        //lee un mensaje de itnrucción en una cadena de texto
        static void __fastcall Read(TMessageInstruction *MI,
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
