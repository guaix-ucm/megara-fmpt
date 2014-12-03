//---------------------------------------------------------------------------
//Archivo: Quantificator.h
//Contenido: operador de cuantificación
//Última actualización: 15/06/2013
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef QUANTIFICATOR_H
#define QUANTIFICATOR_H

#include "..\3_Lists\Vector.h"

//---------------------------------------------------------------------------

using namespace Lists;

//espacio de nombres de operadores matemáticos
namespace Operators {

//---------------------------------------------------------------------------
//clase TQuantificator
//---------------------------------------------------------------------------

//La clase TQuantificator representa cuantificadores centrados en cero,
//que pueden verse como funciones de transformación de coordenadas,
//donde las coordenadas del dominio imagen vienen dadas en pasos.

//clase cuantificador
class TQuantificator {
protected:
        //PARÁMETROS DE CONFIGURACIÓN:

        double __q;
        double __xmin;
        double __xmax;

        //PARÁMETROS DE CONFIGURACIÓN DERIVADOS:

        int __Qmin;
        int __Qmax;

        //PROPIEDADES EN FORMATO TEXTO:

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //MÉTODOS DE ASIMILACIÓN:

        //calcula los límties del dominio imagen de la función:
        //      (Qmin, Qmax),
        //a partir de los parámetros de cuantificación:
        //      (q, xmin, xmax)
        void CalculateImageDomain(void);

public:
        //PARÁMETROS DE CONFIGURACIÓN:

        //escalón de cuantificación
        //q debe ser distinto de cero
        //q puede ser negativo
        //valor por defecto: 1
        double getq(void) const {return __q;} void setq(double);
        //límite inferior del dominio de definición de la función
        //no debe ser mayor que xmax
        //valor por defecto: -1
        double getxmin(void) const {return __xmin;} void setxmin(double);
        //límite superior del dominio de definición de la función
        //no debe ser menor que xmin
        //valor por defecto: 1
        double getxmax(void) const {return __xmax;} void setxmax(double);

        //PARÁMETROS DE CONFIGURACIÓN DERIVADOS:

        //límite inferior del dominio imagen de la función
        int getQmin(void) const {return __Qmin;}
        //límite superior del dominio imagen de la función
        int getQmax(void) const {return __Qmax;}

        //PROPIEDADES EN FORMATO TEXTO:

        AnsiString getqText(void) const {return FloatToStr(__q);}
        void setqText(AnsiString&);
        AnsiString getxminText(void) const {return FloatToStr(__xmin);}
        void setxminText(AnsiString&);
        AnsiString getxmaxText(void) const {return FloatToStr(__xmax);}
        void setxmaxText(AnsiString&);

        AnsiString getQminText(void) const {return FloatToStr(__Qmin);}
        AnsiString getQmaxText(void) const {return FloatToStr(__Qmax);}

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjunto de propiedades de configuración en formato texto
        //      (q, [xmin, xmax])
        AnsiString getRWText(void); void setRWText(const AnsiString&);
        //conjunto de propiedades de configuración derivadas en formato texto
        //      [Qmin, Qmax]
        AnsiString getRText(void);

        //conjunto de todas las propiedades en formato texto
        //en forma de asignaciones
        AnsiString getAssignsText(void) const;
        void setAssignsText(const AnsiString&);

        //FUNCIONES ESTÁTICAS DE IMPRESIÓN Y LECTURA:

        //imprime el conjunto de propiedades RW de un cuantificador
        //en una cadena de caracteres
        static void __fastcall PrintRW(AnsiString &S, const TQuantificator *Q);
        //intenta leer el conjunto de propiedades RW un cuantificador
        //a partir de la posición indicada en una cadena de caracteres
        static void __fastcall ReadRW(TQuantificator *Q, const AnsiString &S,
                int &i);

        //-------------------------------------------------------------------
        //MÉTODOS PÚBLICOS:

        //imprime un cluster de configuración al final de una cadena de texto
        static void __fastcall PrintAssigns(AnsiString &S,
                const TQuantificator *Q);
        //lee un cluster de configuración en una cadena de texto
        static void __fastcall ReadAssigns(TQuantificator *Q,
                const AnsiString &S, int &i);

        //contruye un cuantificador
        TQuantificator(double _q=1, double _xmin=-1, double _xmax=1);
        //copia todas las propiedades de un cuantificador
        void Copy(const TQuantificator*);
        void Copy(const TQuantificator&);
        TQuantificator& operator=(const TQuantificator& Q);
        //construye un clon del cuantificador
        TQuantificator(const TQuantificator*);

        //compara un cuantificador
        bool operator==(const TQuantificator& Q) const;
        bool operator!=(const TQuantificator& Q) const;

        //configura el cuantificador
        void Set(double _q,  double _xmin, double _xmax);

        //cuantifica un valor
        int Quantifice(double x) const;
        int operator[](double x) const;
        //reconstruye un valor
        double Reconstruct(int n) const;
        double operator[](int n) const;
};

//---------------------------------------------------------------------------

} //namespace Operators

//---------------------------------------------------------------------------
#endif // QUANTIFICATOR_H
