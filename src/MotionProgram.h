//---------------------------------------------------------------------------
//Archivo: PositioningProgram.h
//Contenido: programa de posicionamiento
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef MOTIONPROGRAM_H
#define MOTIONPROGRAM_H

#include "..\D_ComunicationSystemModel\MessageInstruction.h"
#include "..\3_Lists\PointersList.h"

//##include <QListWidget>
//##include <QTextEdit>

//--------------------------------------------------------------------------

using namespace Lists;
using namespace Models;

//espacio de nombres de posicionamiento
namespace Positioning {

//--------------------------------------------------------------------------
//TMessageList:
//--------------------------------------------------------------------------

//lista de mensajes de instrucción
class TMessageList : public TPointersList<TMessageInstruction> {
//        AnsiString __Label; void SetLabel(const AnsiString&);

public:
/*        //PROPIEDADES EN FORMATO TEXTO:

        //etiqueta de la lista de mensajes
        //debe ser una cadena de texto imprimible
        //valor por defecto: ""
        __property AnsiString Label = {read=__Label, write=SetLabel};

        //NOTA: cada lista de mensajes debe tener una etiqueta
*/
        //MÉTODOS ESTÁTICOS:

    //imprime una lista en una cadena de texto
    static void __fastcall PrintMessageList(AnsiString &S,
                const TMessageList *L);
        //lee una lista de emnsajes en una cadena de texto
    static void __fastcall ReadMessageList(TMessageList *L,
                const AnsiString &S, int &i);

        //construye una lista de mensajes
        TMessageList(int _Capacity=1003) :
                TPointersList<TMessageInstruction>(_Capacity,
                        TMessageInstruction::CompareIds,
                        NULL, NULL,
                        TMessageInstruction::Print,
                        TMessageInstruction::Read) {;}

        //imprime los mensajes de una lista en una caja de lista
        //#void Print(QListWidget*);
};

//--------------------------------------------------------------------------
//TMotionProgram:
//--------------------------------------------------------------------------

//programa de movimiento
class TMotionProgram : public TPointersList<TMessageList> {
public:
        //contruye un programa de posicionamientos
        TMotionProgram(int _Capacity=7) :
                TPointersList<TMessageList>(_Capacity,
                        NULL, NULL, NULL,
                        TMessageList::PrintMessageList,
                        TMessageList::ReadMessageList) {;}
/*#
        //imprime las etiquetas de las listas de mensajes de un programa
        //en una caja de lista
        void Print(QListWidget *LB);

        //imprime el programa de movimiento en un memorando
        void Print(QTextEdit*);*/
};

//--------------------------------------------------------------------------

} //namespace Positiong

//---------------------------------------------------------------------------
#endif // MOTIONPROGRAM_H
