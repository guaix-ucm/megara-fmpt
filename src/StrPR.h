//---------------------------------------------------------------------------
//Archivo: StrPR.h
//Contenido: funciones de impresión y lectura de cadenas de texto
//Última actualización: 18/03/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef STRPR_H
#define STRPR_H

#include "Vectors.h" //TDoublePoint

//##include <QDateTime>

//---------------------------------------------------------------------------

using namespace Mathematics;

//espacio de nombres de funciones y clases de objetos
//relacionados con cadenas de texto
namespace Strings {

//--------------------------------------------------------------------------
//Functiones de conversión entre tipos numéricos y cadena de texto:

//Convierte un valor en punto flotante de double a AnsiString:
//      AnsiString = FloatToStr(double);
//esta función admite valores de x tan altos como se quiera, incluido MAXDOUBLE
//cuyo valor es 1.79769313486232E308

//Convierte un valor en punto flotante de AnsiString a double:
//      double = StrToFloat(AnsiString);
//esta función no admite valores todos los valores más altos para un double,
//de modo que si invoca a la función con la cadena "1.79769313486232E308",
//lanzará una excepción EOverflow. Además admite más decimales en la mantisa
//que el valor devuelto por FloatToStr; el valor "1.797693134862315807E308"
//tiene 5 decimales más y no produce error de desbordamiento.

//Conclusión:
//cuando se invoca a StrToFloat con una adena de texto conteniendo
//un valor en punto flotante mayor que "1,797693134862315807E308":
//      x = StrToFloat("1,79769313486232E308");
//sucede lo siguiente:
//      - La sentencia no llega a devolver ningún valor, con lo que la variable
//        x conservará su valor previo.
//      - Si el programa invoca a alguna de las funciones de conversión
//        StrToFloat o FloatToStr, o reaiza una operación aritmética:
//              · el sistema lanzará un excepción EOverflow en la sentencia,
//                que podrá ser capturada mediante un bloque catch.
//                Dicha excepción, según el debugger, debría contener
//                el mensaje 'Floating point overflow', pero realmente
//                contiene el mensaje 'EOverflow'.
//                Como la sentencia será interrumpida con una excepción no
//                legará a realizarse.
//      - Si el programa no invoca a StrToFloat ni FloatToStr ni realiza
//        ninguna operación aritmética:
//              · cuando termine de ejecutar todo y se devuelva el control
//                al programa principal, el sistema lanzará una excepción
//                EOverflow, esta vez si, con el mensaje
//                'Floating point overflow', qus será capturada por el bloque
//                catch general del programa.

//Como las funciones StrToFloat y FloatToStr no son simétricas, alguna de las
//dos deberá producir una excepción al intentar convertir alguno de los valores
//producidos por la otra.
//Es por esto que es preferible capturar las exepciones en todas
//ls invocaciones de StrToFloat, y no intentar programar una función
//simétrica de FloatToStr que no de nunca error.

//Resumen de la conslusión:
//La función StrToFloat produce efectos laterales que pueden ser capturados
//invoando a StrToFloat, FloatToStr o alguna función aritmética.

//Una posible solución:
//try {
//        x = StrToFloat(S);

//        //captura el efecto lateral cuando S contiene
//        //un valor mayor que "1,797693134862315807E308"
//        double aux = 0;
//        aux + aux;

//        //también podría hacerse invocando FloatToStr(0) pero seguramente
//        //sería más costoso.

//} catch(EOverflow &E) {
//        //indica que: la cadena de texto S contiene
//        //un valor en punto flotante fuera del rango de StrToFloat
//        throw EImproperArgument("string "S" contain a
//              floating point value out of range of StrToFloat");
//} catch(...) {
//        throw;
//}

//---------------------------------------------------------------------------
//Funciones para texto:

//intenta leer una palabra desde la posición i de una cadena de texto
void StrReadWord(AnsiString &Word, const AnsiString S, int &i);

//intenta leer una cadena desde la posición i de una cadena de texto
//incluyendo las comillas simples delimitadoras
void StrReadString(AnsiString &String, const AnsiString &S, int &i);
//intenta leer una cadena desde la posición i de una cadena de texto
//que se encuentra delimitada por dos caracteres determinados
void StrReadStringBetweenChars(AnsiString &String, const AnsiString &S, int &i,
        char c1='\"', char c2='\"');
//traduce uan cadena entredoscaracteres a la misma cadena sin los caracteres
AnsiString StrBetweenCharsToStr(const AnsiString &S,
        char c1='\"', char c2='\"');
//lee una subcadena SubS desde la posición indicada de una cadena S
//hasta el caracter delimitador (d) que se indique
void StrReadStringToChar(AnsiString &SubS, const AnsiString &S, int &i, char d);
//intenta recorrer una cadena de texto (entre comillas simples)
//desde la posición i de una cadena de texto
void StrTravelString(const AnsiString &S, int &i);

//intenta leer una etiqueta desde
//la posición indicada de una cadena de texto
void StrReadLabel(AnsiString &Reading, const AnsiString &Label,
        const AnsiString &S, int &i);
//recorre una etiqueta a partir de
//la posición indicada de una cadena de texto
void StrTravelLabel(const AnsiString &Label, const AnsiString &S, int &i);

//ADVERTENCIA: las funciones de lectura deetiquetas no pueden ser empleadas
//con cadenas que empiecen por caracteres separadores.

//busca una sentencia de asignación a partir de
//la posición indicada de una cadena de texto
//void StrReadAssign(AnsiString &Ident, const AnsiString &S, int &i);
//lee de S una cadena hasta que encuentra ';'
//void StrReadSentence(AnsiString &Sentence, const AnsiString &S, int &i);

//segrega la subcadena desde la posición indicada hasta el final
//si encuentra un caracter no separador lanza una excepción EImproperArgument
//i quedará indicando a la posúltima posición
//void StrReadToEnd(AnsiString &SubS, const AnsiString &S, int &i);
//atraviesa la subcadena desde la posición indicada hasta el final
//si encuentra un caracter no separador lanza una excepción EImproperArgument
//i quedará indicando a la posúltima posición
void StrTravelToEnd(const AnsiString&, int&);
//recorre una cadena desde la posición indicada
//hasta que encuentra un caracter no separador
void StrTravelSeparatorsIfAny(const AnsiString&, int&);
//atraviesa uno o más separadores
void StrTravelSeparators(const AnsiString&, int&);

//---------------------------------------------------------------------------
//Funciones para tipos numéricos:

//escudriña S a partir de i en busca de un número entero
//i quedará indicando a la posúltima posición
void StrReadIntStr(AnsiString &I, const AnsiString &S, int &i);
//escudriña S a partir de i en busca de un número entero
//e intenta convertirlo a int mediante StrToInt
void  StrReadInt(int &n, const AnsiString &S, int &i);
//convierte un valor entero de formato texto a entero
//si S contiene algo más que un valor entero
//lanza una excepción EImproperArgument
int StrToInt_(const AnsiString &S);

//NOTA: StrReadIntStr funciona perfectamente pero ha sido aislada para
//impedir su uso inadvertido.

//imprime el valor de una variable double en una cadena de texto
void  StrPrintFloat(AnsiString &S, double x);
//escudriña S a partir de i en busca de un número de punto flotante
//i quedará indicando a la posúltima posición
void StrReadFloatStr(AnsiString &F, const AnsiString &S, int &i);
//escudriña S a partir de i en busca de un número de punto flotante
//e intenta convertirlo a double mediante StrToFloat
void  StrReadFloat(double &x, const AnsiString &S, int &i);
//convierte un valor en punto flotante de formato texto a double
//si S contiene algo más que un valor en punto flotante
//lanza una excepción EImproperArgument
double StrToFloat_(const AnsiString &S);

//Imprime el valor de una variable lógica al final de una cadena de texto.
//Utiliza los identificadores:
//      TrueBoolStrs[0] en vez de "1", y
//      FalseBoolStrs[0] en vez de "0".
//Si b==true y TrueBoolStrs[0] no está definido
//lanza una excepción EImproperCall.
//Si b==false y FalseBoolStrs[0] no está definido
//lanza una excepción EImproperCall.
void  StrPrintBool(AnsiString &S, bool b);
//Intenta leer un valor lógico en S desde la posición i.
//Deberán estar definidos los identificadores:
//      TrueBoolStrs[0] y
//      FalseBoolStrs[0].
//Si encuentra TrueBoolStrs[0] devuelve B = TrueBoolStrs[0].
//Si encuentra FalseBoolStrs[0] devuelve B = TrueBoolStrs[0].
//El índice i quedará indicando a la posúltima posición
void StrReadBoolStr(AnsiString &B, const AnsiString &S, int &i);
//escudriña S a partir de i en busca de un valor lógico
//e intenta convertirlo a lógico mediante StrToBool
void  StrReadBool(bool &b, const AnsiString &S, int &i);

//ADVERTENCIA: con la función StrToBool no se han observado
//las mismas irregularidades que con StrToInt y StrToFloat,
//pero se considera inapropiado, ya que no realiza la comprobación
//de la condición cuando UseBoolStrs==true:
//      TrueBoolStrs[0] = "True", y
//      FalseBoolStrs[0] = "False".

//Convierte un valor lógico a un AnsiString.
//      Si UseBoolStrs==true, B==true y TrueBoolStrs[0] no está definido
//lanza una excepción EImproperCall.
//Si UseBoolStrs==true, B==false y FalseBoolStrs[0] no está definido
//      lanza una excepción EImproperCall.
AnsiString  BoolToStr_(bool B, bool UseBoolStrs = false);
//Convierte un AnsiString a un valor lógico.
//Si TrueBoolStrs[0] o FalseBoolStrs[0] no están definidos
//lanza una excepción EImproperCall.
bool  StrToBool_(const AnsiString S);

//RECORDATORIO: aunque:
//      TrueBoolStrs[0]=="True" y
//      FalseBoolStrs[0]=="False",
//la función StrToBool no distinguirá entre mayúsculas y minúsculas.

//imprime un punto en una cadena de texto
void  StrPrintPoint(AnsiString &S, TPoint P);
//compara dos punto
//valores devueltos:
//      -1: si x1<x2 || (x1==x2 && y1<y2)
//      1: si x1>x2 || (x1==x2 && y1>y2)
//      0: si x1==x2 && y1==y2
int  CompareDPoints(const TDoublePoint *P1, const TDoublePoint *P2);
//imprime un punto al final de una cadena de texto
void  StrPrintDPoint(AnsiString &S, const TDoublePoint *P);
//intenta leer un punto a partir de la posición i de una cadena de texto
//e intenta convertirlo a TDoublePoint mediante StrToFloat_
void  StrReadDPoint(TDoublePoint *P, const AnsiString &S, int &i);
//convierte un valor punto de formato texto a TDoublePoint
//si S contiene algo más que un valor punto
//lanza una excepción EImproperArgument
TDoublePoint StrToDPoint(const AnsiString &S);

//intenta leer un rectángulo a partir de la posición i de una cadena de texto
//e intenta convertirlo a TDoubleRect mediante StrToFloat_
void  StrReadDRect(TDoubleRect *R, const AnsiString &S, int &i);
//convierte un valor rectángulo de formato texto a TDoubleRect
//si S contiene algo más que un valor rectángulo
//lanza una excepción EImproperArgument
TDoubleRect StrToDRect(const AnsiString &S);

//ADVERTENCIA: para incorporar las funciones de impresión y lectura de
//TDoublePoint a dicha estructura, habría que trasladar dicha estrucutar
//a una posición posterior a la declaración de FloatToStr_.

//imprime dos enteros en una xadena indentados a 12 espacios
AnsiString IntToRowStr(int n1, int n2);

//NOTA: las siguientes funciones funcionan correctamente pero han sido aisladas
//para excluirlas del código posterior:
/*
//imprime en S las palabras de SL entre c1 y c2 separadas por s;
//s puede ser un simple espacio
void StrPrintSecuency (AnsiString &S, TStringList *SL,
    char c1='{', char c2='}', const AnsiString s=", ");
//escudriña S en busca de una lista delimitada por c1 y c2
//cuyas palabras están separadas por d
//si no la encuentra lanza una excepción EImproperArgument
void ReadList (TStringList *SL, const AnsiString &S,
    char c1='{', char c2='}', char d=',');
//escudriña S en busca de una lista delimitada por c1 y c2
//cuyas palabras están separadas por espacios
//si no la encuentra lanza una excepción EImproperArgument
void ReadVector (TStringList *SL, const AnsiString &S,
    char c1='(', char c2=')');  */

//---------------------------------------------------------------------------
//Funciones para fecha-hora

//Para traducir de QString a QDateTime, existen los métodos:
//  QDateTime	fromString(const QString & string, Qt::DateFormat format = Qt::TextDate)
//  QDateTime	fromString(const QString & string, const QString & format)
//Sin embargo, el formato "yyyy/mm/dd hh:mm:ss" especificado mediante una QString,
//no resulta efectivo, devolviendo la función la fecha-hora inválida. Por eso es
//necesario especificar el formato mediante Qt::ISODate, el cual corresponde a:
//  "yyyy-mm-ddThh:mm:ss".

//Por otra parte TDateTime mantenía homología con double.
//En el caso de QDteTime, la homología se mantiene con uint.
//TDateTime era convertido automáticamente a double y viceversa.
//En el caso de QDateTime existen las funciones sigueintes:
//  void	setTime_t(uint seconds)
//  uint	toTime_t() const
//  QDateTime	fromTime_t(uint seconds)
//  QDateTime	fromTime_t(uint seconds, Qt::TimeSpec spec, int offsetSeconds = 0)
//  QDateTime	fromTime_t(uint seconds, const QTimeZone & timeZone)
/*#
//lee la fecha-hora en una cadena de texto a partir de la posición indicada
//en el formato "dd/mm/yyyy hh:mm:ss" en formato de 24h sin AM/PM
//si S no contiene un dato válido, lanza EConvertError
void  StrReadDateTime(QDateTime &DT,
        const AnsiString &S, int &i);

//convierte un valor de fecha-hora de formato texto a QDateTime
//si S contiene algo más que un valor de fehca-hora
//lanza una excepción EImproperArgument
double StrToDateTime_(const AnsiString &S);
*/
//---------------------------------------------------------------------------

} //namespace Strings

//---------------------------------------------------------------------------
#endif //STRPR_H
