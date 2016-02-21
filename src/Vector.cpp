// Copyright (c) 2012-2016 Isaac Morales Durán. All rights reserved.
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
//Archivo: Vector.cpp
//Contenido: vector de tipos numéricos
//Última actualización: 20-09-2011
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Vector.h"

//---------------------------------------------------------------------------

//espacio de nombres de funciones y clases de objetos
//relacionados con listas, vectores y matrices
namespace Lists {

//---------------------------------------------------------------------------
//lee una tabla de pares (x, y) en punto flotante
void  StrReadTable(TVector<double> &X, TVector<double> &Y,
        const AnsiString &S, int &i)
{

        //estado
        //      0: esperando separador antes de x,el primer caracter de x
        //         u otro caracter diferente o fin de cadena;
        //      1: esperando número en punto flotante para x;
        //      2: esperando separador antes de y o el primer caracter de y;
        //      3: esperando número en punto flotante para y;
        //      4: esperando separador, salto de linea
        //         u otro caracter diferente;
        //      5: tabla leida con éxito;
        int status = 0;

        //variables tampón
        char c;
        double x, y;
        TVector<double> t_X, t_Y;
        int k = 0; //índice a elementos de X e Y

        if(i > S.Length()) //si se ha acabado la cadena
                status = 5; //indica que ha termiando d leer la tabla con éxito

        do { //repite
                c = S[i];

                //reacciona según el estado y el caracter
                switch(status) {
                        case 0: //esperando separador antes de x, el primer caracter de x u otro caracter diferente
                                switch(c) {
                                        case ' ': case 0x09: //si es separador
                                                i++; //indica al próximo caracter
                                                if(i > S.Length()) //si se ha acabado la cadena
                                                        status = 5; //indica que ha terminado de leer la tabla
                                                break;
                                        case '-':
                                        case'0': case '1': case '2': case '3': case '4':
                                        case '5': case '6': case '7': case '8': case '9': //si es el primer caracter de x
                                                status++; //pasa a leer x
                                                break;
                                        default: //si es otro caracter diferente
                                                status = 5; //pasa a signar las variables tampón
                                }
                                break;
                        case 1: //esperando número en punto flotante para x
                                try {
                                        StrReadFloat(x, S, i); //intenta leer la ordenada
                                        t_X.Add(x);
                                        if(i > S.Length()) //si se ha acabado la cadena
                                                //indica que no encenta la ordenada Y[k]
                                                throw EImproperArgument(AnsiString("ordinate Y[")+IntToStr(k)+AnsiString("] not found"));
                                        status++; //pasa a leer el separador entre x e y
                                } catch(...) {
                                        throw;
                                }
                                break;
                        case 2: //esperando separador antes de y o el primer caracter de y
                                switch(c) {
                                        case ' ': case 0x09: //si es separador
                                                i++; //indica al próximo caracter
                                                if(i > S.Length()) //si se ha acabado la cadena
                                                        status = 5; //indica que ha terminado de leer la tabla con éxito
                                                break;
                                        default: //si es otro caracter no separador
                                                status++; //pasa a leer y
                                                break;
                                }
                                break;
                        case 3: //esperando número en punto flotante para y
                                //intenta leer la abcisa
                                try {
                                        StrReadFloat(y, S, i);
                                        t_Y.Add(y);
                                        if(i > S.Length()) //si se ha acabado la cadena
                                                status = 5; //indica que ha terminado de leer la tabla con éxito
                                        else
                                                status++; //pasa a leer el separador, salto de linea o fin de cadena
                                } catch(...) {
                                        throw;
                                }
                                break;
                        case 4: //esperando separador, salto de linea u otro caracter diferente
                                switch(c) {
                                        case ' ': case 0x09: //si es separador
                                                i++; //indica al próximo caracter
                                                if(i > S.Length()) //si se ha acabado la cadena
                                                        status = 5; //indica que ha terminado de leer la tabla con éxito
                                                break;
                                        case '\r': //si es salto de linea
                                                StrTravelLabel("\r\n", S, i);
                                                if(i > S.Length()) //si se ha acabado la cadena
                                                        status = 5; //indica que ha terminado de leer la tabla con éxito
                                                else {
                                                        k++; //indica a la próxima posición de X e Y
                                                        status = 0; //pasa a leer la siguiente linea
                                                }
                                                break;
                                        default: //si es otro caracter diferente
                                                status = 5; //indica que ha leido la tabla con éxito
                                }
                                break;
                }
        } while(status < 5); //hasta que la tabla se haya leido con éxito

        //asigna las variables tampón
        X = t_X;
        Y = t_Y;
}
//---------------------------------------------------------------------------

} //namespace Lists

//---------------------------------------------------------------------------

