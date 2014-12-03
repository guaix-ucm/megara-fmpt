//---------------------------------------------------------------------------
//Archivo: Instruction.cpp
//Contenido: instrucción de posicionador
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Instruction.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//###########################################################################
//TInstruction
//###########################################################################

//clase instrucción de posicionador
void TInstruction::setName(AnsiString _Name)
{
    //el nombre de instrucción debería ser conocido
    if(_Name == "") {
        Args.Clear();
    } else if(_Name == "MA") {
        Args.Clear();
    } else if(_Name == "MS") {
        Args.Clear();
    } else if(_Name == "M1") {
        Args.setCount(1);
    } else if(_Name == "M2") {
        Args.setCount(1);
    } else if(_Name == "MM") {
        Args.setCount(2);
    } else if(_Name == "ST") {
        Args.Clear();
    } else if(_Name == "SP") {
        Args.Clear();
    } else { //si no (es que no conoce el nombre de instrucción)
        //indica que el nombre de instrucción no es conocido
        throw EImproperArgument(AnsiString("instruction name '")+_Name+AnsiString("' should be known"));
    }

    //asigna el nuevo valor
    __Name = _Name;
}

//PROPIEDADES EN FORMATO TEXTO:

AnsiString TInstruction::getText(void) const
{
    AnsiString S;

    //asigna el nombre de la instrucción
    S = getName();

    //cncatena los argumentos de la intrucción en formato texto
    for(int i=0; i<Args.getCount(); i++)
        S += AnsiString(" ")+Args.getItemsText(i);

    return S;
}

void TInstruction::setText(const AnsiString &S)
{
    try {
        int i = 1;
        Read(this, S, i);
        StrTravelToEnd(S, i);
    } catch(...) {
        throw;
    }
}

//MÉTODOS ESTÁTICOS:

//imprime una instrucción en una cadena de texto
void _fastcall TInstruction::Print(AnsiString &S, const TInstruction *I)
{
    //el puntro I debería apuntar a una instrucción construida
    if(I == NULL)
        throw EImproperArgument("pointer I should point to built instruction");

    S += I->getText();
}
//lee una instrucción en una cadena de texto
void __fastcall TInstruction::Read(TInstruction *I, const AnsiString &S, int &i)
{
    //el puntro I debería apuntar a una instrucción construida
    if(I == NULL)
        throw EImproperArgument("pointer I should point to built instruction");

    //el índice i debe indicar al primer caracteer de una instrucción
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("instruction not found");

    //ADVERTENCIA: si la cadena S solo contiene espacios o está vacía
    //corresponde a la instrucción por defecto.

    //estado de lectura
    //      0: esperando ' ' o primer caracter de la primera palabra
    //      1: leyendo la primera palabra y esperando ' '
    //      2: esperando ' ' o primer caracter de la segunda palabra
    //      3: esperando ' ' o primer caracter de la tercera palabra
    //      4: instrucción leida con éxito
    int status = 0;

    char c; //caracter indicado S[i]
    AnsiString Word; //palabra
    TInstruction _I; //variable tampón

    //si la cadena a partir de i está vacía
    if(i > S.Length()) {
        *I = _I; //asigna la instrucción por defecto
        return; //no hace nada más
    }

    do {
        c = S[i]; //asigna el próximo caracter para facilitar su acceso

        switch(status) {
        case 0: //esperando ' ' o primer caracter de la primera palabra
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    status = 4;
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    status = 4;
                break;
            case 'M': case 'G': case 'S':
                Word = c;
                i++;
                if(i > S.Length())
                    try {
                    _I.setName(Word);
                    if(_I.Args.getCount() > 0)
                        throw EImproperArgument("first argument not found");
                    status = 4;
                } catch(EImproperArgument &E) {
                    throw EImproperArgument(E.Message+AnsiString(" for property Name"));
                } catch(...) {
                throw;
            }
                else
                status = 1;
                break;
            default:
                status = 4;
            }
            break;
        case 1: //leyendo la primera palabra y esperando ' '
            switch(c) {
            case ' ':
                try {
                _I.setName(Word);
            } catch(EImproperArgument &E) {
                    throw EImproperArgument(E.Message+AnsiString(" for property Name"));
                } catch(...) {
                throw;
            }
                i++;
                if(i > S.Length())
                    if(_I.Args.getCount() > 0)
                        throw EImproperArgument("first argument not found");
                    else
                        status = 4;
                else
                    status = 2;
                break;
            case '\r':
                try {
                _I.setName(Word);
            } catch(...) {
                throw;
            }
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    if(_I.Args.getCount() > 0)
                        throw EImproperArgument("first argument not found");
                    else
                        status = 4;
                else
                    status = 2;
                break;
            default:
                Word += c;
                i++;
                if(i > S.Length())
                    try {
                    _I.setName(Word);
                    if(_I.Args.getCount() > 0)
                        throw EImproperArgument("first argument not found");
                    status = 4;
                } catch(EImproperArgument &E) {
                    throw EImproperArgument(E.Message+AnsiString(" for property Name"));
                } catch(...) {
                throw;
            }
            }
            break;
        case 2: //esperando ' ' o primer caracter de la segunda palabra
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("first argument not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("first argument not found");
                break;
            default:
                try {
                StrReadFloatStr(Word, S, i);
                _I.Args.setItemsText(0, Word);
            } catch(EImproperArgument &E) {
                    throw EImproperArgument(E.Message+AnsiString(" for property Args[0]"));
                } catch(...) {
                throw;
            }

                //reaciona según el número de argumentos
                if(_I.Args.getCount() > 1) {
                    if(i > S.Length())
                        throw EImproperArgument("second argument not found");
                    status = 3;
                } else {
                    status = 4;
                }
            }
            break;
        case 3: //esperando ' ' o primer caracter de la tercera palabra
            switch(c) {
            case ' ':
                i++;
                if(i > S.Length())
                    throw EImproperArgument("second argument not found");
                break;
            case '\r':
                StrTravelLabel("\r\n", S, i);
                if(i > S.Length())
                    throw EImproperArgument("second argument not found");
                break;
            default:
                try {
                StrReadFloatStr(Word, S, i);
                _I.Args.setItemsText(1, Word);
            } catch(EImproperArgument &E) {
                    throw EImproperArgument(E.Message+AnsiString(" for property Args[1]"));
                } catch(...) {
                throw;
            }

                //reaciona según el número de argumentos
                if(_I.Args.getCount() > 2)
                    throw EImposibleError("there isn't instruction with more two arguments");
                else
                    status = 4;
            }
            break;
        }
        //mientras la instrucción no hya sido leida con éxito
    } while(status < 4);

    //asigna la variable tampón
    *I = _I;
}

//MÉTODOS PÚBLICOS:

//contruye una instrucción de posicionador
TInstruction::TInstruction(void) : __Name(""), Args(2)
{
}

//asigna una instrucción
TInstruction& TInstruction::operator=(const TInstruction& Instruction)
{
    __Name = Instruction.getName();
    Args = Instruction.Args;

    return *this;
}

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

