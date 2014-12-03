//---------------------------------------------------------------------------
//Archivo: Constants.cpp
//Contenido: constantes matemáticas
//Última actualización: 21/03/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef CONSTANTS_H
#define CONSTANTS_H

//---------------------------------------------------------------------------

//El valor de M_PI leido con el depurador es 3.14159265358979
//define un valor textual para 2PI con objeto de evitar
//la discrepancia de los valores cargados de archivo y 2*M_PI
#define M_2PI 6.28318530717959

//máximo valor de una variable de tipo unsigned short int
//const unsigned short int MAXUSHORT = pow(2, sizeof(unsigned short int)*8) - 1
const unsigned short MAXUSHORT = (unsigned short)0xFFFF;

//El valor más grande para tipos sin signo no viene definido
//para tipos con signo si vienen definidos y son:
//      MAXSHORT  	Largest short
//      MAXINT  	Largest int
//      MAXLONG  	Largest long

//número de elementos de un array
#define NELEMS(arr) (sizeof(arr) / sizeof(arr[0]))

//La macro NELEMS no puede ser sustituida por una función, porque para
//la función sizeof, el puntero a un array no es o mismo que el array,
//de modo que la siguiente función simepre devolverá 1:
//      template <class T> int NumberElements(T *arr) {
//              return sizeof(arr)/sizeof(arr[0]);}

//error numérico asumible en las funciones geométricas de giro
#define ERR_NUM 0.00000001

//ADVERTENCIA: si se utiliza un compilador distinto de C++Builder 6.0 puede
//que sea neceario cambiar el valor de la constante ERR_NUM.

//---------------------------------------------------------------------------
#endif // CONSTANTS_H
