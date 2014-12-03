//---------------------------------------------------------------------------
//Archivo: FiberConnectionModel.h
//Contenido: modelo de conexión de fibras
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FIBERCONNECTIONMODEL_H
#define FIBERCONNECTIONMODEL_H

#include "..\3_Lists\PointersList.h"

//---------------------------------------------------------------------------

using namespace Lists;

//espacio de nombres de modelos
namespace Models {

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
        int __RPId;
        int __SpaxelId;
        int __FiberId;
        double __s;

public:
        //etiquetas
        static AnsiString RPIdLabel;
        static AnsiString SpaxelIdLabel;
        static AnsiString FiberIdLabel;
        static AnsiString sLabel;

        //identificador único del RP del que proviene la fibra
        //debe ser no negativo
        //valor por defecto: 0 (indefinido)
        int getRPId(void) const {return __RPId;}
        void setRPId(int);
        //identificador único del spaxel de la microlente
        //debe estar en [0, 7]
        //valor por defecto: 0 (indefinido)
        int getSpaxelId(void) const {return __SpaxelId;}
        void setSpaxelId(int);
        //identificador único de la Fibra
        //debe ser no negativo
        //valor por defecto: 0 (indefinido)
        int getFiberId(void) const {return __FiberId;}
        void setFiberId(int);
        //posición relativa en la pseudoslit
        //valor por defecto: 0
        double gets(void) const {return __s;}
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

        //MÉTODOS:

        //lee los valores de las propiedades en una fila de texto
        static void __fastcall ReadRow(TConection *Item,
                const AnsiString& S, int& i);
        //imprime los valores de las propiedades en una fila de texto
        static void __fastcall PrintRow(AnsiString&, const TConection*);
        //obtiene las etiqeutas de las propiedades en formato texto
        static AnsiString GetLabelsRow(void);
        //atraviesa las etiquetas de las propiedades
        static void TravelLabelsRow(const AnsiString& S, int& i);

        //construye una conexión con los valores por defecto
        TConection(void) : __RPId(0), __SpaxelId(0), __FiberId(0), __s(0) {;}

        //asigna las propiedades atómicamente
        //si no es posible asignar todas las propiedades:
        //      lanza una excepción EImproperArgument
        void Set(int _RPId, int _SpaxelId, int _FiberId, double _s);
};

//clase Fiber Connection Model
class TFiberConnectionModel {
        double __R;

public:
        //distancia entre centros de los spaxeles
        //debe ser mayor que cero
        //valor por defecto: 0.443 mm
        double getR(void) const {return __R;} void setR(double);

        //La distancia entre spaxel de la microlente
        //tiene una precisión de +/- 5 um.

        //tabla de conexiones de la pseudoslit
        TPointersList<TConection> Conections;

        //tabla de cnexiones en formato texto
        AnsiString getConnectionsText(void) const;
        void setConnectionsText(const AnsiString&);

        //construye un espectrógrafo con los valores por defecto
        TFiberConnectionModel(void);

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
                double x3, double y3, double theta3);
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
