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
//File: FiberConnectionModel.h
//Content: fiber connection model
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FIBERCONNECTIONMODEL_H
#define FIBERCONNECTIONMODEL_H

#include "RoboticPositionerList3.h"
//#include "PointersList.h"

//---------------------------------------------------------------------------

using namespace Lists;

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TConection
//---------------------------------------------------------------------------

//Se considera que el espectrógrafo enpieza en las microlentes, de modo que
//del Fiber MOS Model solo se obtendrán los valores de las siguientes
//propiedades, por cada RP: (RPId, x3, y3, theta3). Con estos datos
//se realizará el sigueinte proceso:
//1. La FMPT, obtiene de cada RP del Fiber MOS Model:
//      (RPId, x3, y3, theta3)
//2. La FMPT determina por cada RP los siete valores:
//      (SpaxelId, x, y)
//3. La FMPT determina:
//      (FiberId, s) y (RA, DEC)
//4. La FMPT devuelve por cada RP:
//      (RPId, FiberId, RA, DEC, s).

//clase conexión de la pseudoslit
class TConection {
    //PROPIEDADES DE DEFINICIÓN:

        int p_RPId;
        int p_SpaxelId;
        int p_FiberId;
        double p_s;

public:
        //PROPIEDADES ESTÁTICAS:

        //etiquetas
        static AnsiString RPIdLabel;
        static AnsiString SpaxelIdLabel;
        static AnsiString FiberIdLabel;
        static AnsiString sLabel;

        //PROPIEDADES DE DEFINICIÓN:

        //identificador único del RP del que proviene la fibra
        //debe ser no negativo
        //valor por defecto: 0 (indefinido)
        int getRPId(void) const {return p_RPId;}
        void setRPId(int);
        //identificador único del spaxel de la microlente
        //debe estar en [0, 7]
        //valor por defecto: 0 (indefinido)
        int getSpaxelId(void) const {return p_SpaxelId;}
        void setSpaxelId(int);
        //identificador único de la Fibra
        //debe ser no negativo
        //valor por defecto: 0 (indefinido)
        int getFiberId(void) const {return p_FiberId;}
        void setFiberId(int);
        //posición relativa en la pseudoslit
        //valor por defecto: 0
        double gets(void) const {return p_s;}
        void sets(double);

        //PROPIEDADES EN FORMATO TEXTO:

        AnsiString getRPIdText(void) const;
        void setRPIdText(const AnsiString&);
        AnsiString getSpaxelIdText(void) const;
        void setSpaxelIdText(const AnsiString&);
        AnsiString getFiberIdText(void) const;
        void setFiberIdText(const AnsiString&);
        AnsiString getsText(void) const;
        void setsText(const AnsiString&);

        //MÉTODOS ESTÁTICOS:

        //lee los valores de las propiedades en una fila de texto
        static void  ReadRow(TConection *Item,
                const AnsiString& S, int& i);
        //imprime los valores de las propiedades en una fila de texto
        static void  PrintRow(AnsiString&, const TConection*);
        //obtiene las etiqeutas de las propiedades en formato texto
        static AnsiString GetLabelsRow(void);
        //atraviesa las etiquetas de las propiedades
        static void TravelLabelsRow(const AnsiString& S, int& i);

        //MÉTODOS PÚBLICOS:

        //construye una conexión con los valores por defecto
        TConection(void) : p_RPId(0), p_SpaxelId(0), p_FiberId(0), p_s(0) {;}

        //copy all properties of a connection
        TConection& operator=(const TConection&);

        //build a clon of a connection
        TConection(const TConection*);

        //asigna las propiedades atómicamente
        //si no es posible asignar todas las propiedades:
        //      lanza una excepción EImproperArgument
        void Set(int RPId, int SpaxelId, int FiberId, double s);
};

//---------------------------------------------------------------------------
//TFiberConnectionModel
//---------------------------------------------------------------------------

//clase Fiber Connection Model
class TFiberConnectionModel {
        double p_R;

public:
        //distancia entre centros de los spaxeles
        //debe ser mayor que cero
        //valor por defecto: 0.443 mm
        double getR(void) const {return p_R;} void setR(double);

        //La distancia entre spaxel de la microlente
        //tiene una precisión de +/- 5 um.

        //tabla de conexiones de la pseudoslit
        TPointersList<TConection> Conections;

        //tabla de conexiones en formato texto
        AnsiString getConnectionsText(void) const;
        void setConnectionsText(const AnsiString&);

        //construye un Fiber Connection Model con los valores por defecto
        TFiberConnectionModel(void);

        //clone a Fiber Connection Model
        void Clone(const TFiberConnectionModel&);

        //Definitions:
        //  (x, y): the returned spaxel center
        //  (x3, y3): the P3 of the RP
        //  theta3: the orientation in S0 of the vector from P1 to P3.
        //This is the numeration of the espaxels:
        //                              5
        //                          6   |   2
        //   P1 ------------------------P3
        //                          7   |   1
        //                              3

        //determina las coordenadas en S0 del centro del spaxel 1
        void GetSpaxelCenter1(double&x, double& y,
                double x3, double y3, double theta3);
        //determina las coordenadas en S0 del centro del spaxel 2
        void GetSpaxelCenter2(double&x, double& y,
                double x3, double y3, double theta3);
        //determina las coordenadas en S0 del centro del spaxel 3
        void GetSpaxelCenter3(double&x, double& y,
                double x3, double y3, double theta3);
        //determina las coordenadas en S0 del centro del spaxel 4
        void GetSpaxelCenter4(double&x, double& y,
                double x3, double y3);
        //determina las coordenadas en S0 del centro del spaxel 5
        void GetSpaxelCenter5(double&x, double& y,
                double x3, double y3, double theta3);
        //determina las coordenadas en S0 del centro del spaxel 6
        void GetSpaxelCenter6(double&x, double& y,
                double x3, double y3, double theta3);
        //determina las coordenadas en S0 del centro del spaxel 7
        void GetSpaxelCenter7(double&x, double& y,
                double x3, double y3, double theta3);

        //Determina (FiberId, s) a partir de (RPId, SpaxelId).
        //Si no encuentra (RPId, SpaxelId) en la tabla de posiciones
        //relativas de la pseudoslit: devuelve falso.
        bool Find(int& FiberId, double& s, int RPId, int FiberIn);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // FIBERCONNECTIONMODEL_H
