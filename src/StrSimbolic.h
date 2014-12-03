//---------------------------------------------------------------------------
//Archivo: StrSimbolic.h
//Contenido: funciones de cálculo simbólico
//Última actualización: 16/03/2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef STRSIMBOLIC_H
#define STRSIMBOLIC_H

#include "..\0_VCL\vclemu.h" //AnsiString

//---------------------------------------------------------------------------

//espacio de nombres de funciones y clases de objetos
//relacionados con cadenas de texto
namespace Strings {

//---------------------------------------------------------------------------

//ADVERTENCIA: todos los enteros están escritos en orden inverso al habitual
//para que la cifra de menor peso esté en las posicion de menor orden
//y biceversa; por lo que antes de imprimir un número se debe voltear

//ADVERTENCIA: pasar un AnsiString por valor no es seguro,
//porque al acceder a los elementos del AnsiString argumento,
//puede estar accediendo a los elementos del AnsiString asignado;
//para evitarlo puede asignar la cadena contenida en el AnsiString S.c_str()

//determina si una cadena contiene un entero;
//a la derecha de la secuencia de cifras puede tener el signo '-'
bool StrIsInteger(const AnsiString &S);
//copia las cifras significativas de la cadena fuente (S)
//en la cadena destino (D);
//el signo menos no es una cifra significativa
//el cero identificador si es una cifra significativa;
void StrCopySigFig(AnsiString &D, const AnsiString &S);
//borra el signo a la derecha de un entero
void StrAbs(AnsiString &S);
//función signo devuelve: 1 si S es un entero >=0; y -1 si es un entero <0
int StrSign(const AnsiString &S);
//compara dos enteros devuelve: -1 si S1<S2; 0 si S1==S2; y 1 si S1>S2
int StrCmp(const AnsiString &S1, const AnsiString &S2);
//suma algebraica de enteros a nivel simbólico
AnsiString StrSumInt(const AnsiString &S1, const AnsiString &S2);
//suma algebraica de dos doubles a nivel simbólico
double SumDouble(double x1, double x2);

//ADEVERTENCIA: en SumDouble solo se tienen en cuenta las 15 primeras cifras
//significativas de los sumandos y solo se sonservan las 15 primeras cifras
//significativas del resultado;
//esto es debido al uso de FloatToStr y StrToFloat para traducir

//---------------------------------------------------------------------------

} //namespace Strings

//---------------------------------------------------------------------------
#endif // STRSIMBOLIC_H
