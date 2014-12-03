//---------------------------------------------------------------------------
//Archivo: MotionFunction.cpp
//Contenido: función de movimiento
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "MotionFunction.h"
#include "..\0_VCL\Exceptions.h"
#include "..\2_Strings\Strings.h"
#include "..\2_Strings\StrPR.h"

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres de funciones de movimiento
namespace MotionFunctions {

//--------------------------------------------------------------------------
//TMotionFunction
//--------------------------------------------------------------------------
//PROPIEDADES EN FORMATO TEXTO:

void TMotionFunction::setLabel(const AnsiString& _Label)
{
        //la cadena Label debe ser una cadena imprimible
        if(!StrIsPrintable(_Label))
                throw EImproperArgument("string Label should be a printable string");

        __Label = _Label;
}

AnsiString TMotionFunction::getvmaxabsText(void) const
{
        return FloatToStr(__vmaxabs);
}
void TMotionFunction::setvmaxabsText(const AnsiString& S)
{
        try {
                setvmaxabs(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}
AnsiString TMotionFunction::getpstaText(void) const
{
        return FloatToStr(__psta);
}
void TMotionFunction::setpstaText(const AnsiString& S)
{
        try {
                setpsta(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}
AnsiString TMotionFunction::getpfinText(void) const
{
        return FloatToStr(__pfin);
}
void TMotionFunction::setpfinText(const AnsiString& S)
{
        try {
                setpfin(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}

AnsiString TMotionFunction::getvmaxText(void) const
{
        return FloatToStr(__vmax);
}
AnsiString TMotionFunction::getTminText(void) const
{
        return FloatToStr(__Tmin);
}

AnsiString TMotionFunction::getDText(void) const
{
        return FloatToStr(__D);
}
void TMotionFunction::setDText(const AnsiString& S)
{
        try {
                setDText(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}
AnsiString TMotionFunction::getTText(void) const
{
        return FloatToStr(__T);
}
void TMotionFunction::setTText(const AnsiString& S)
{
        try {
                setTText(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}

AnsiString TMotionFunction::getDmaxText(void) const
{
        return FloatToStr(__Dmax);
}
AnsiString TMotionFunction::getvcminText(void) const
{
        return FloatToStr(__vcmin);
}
AnsiString TMotionFunction::getvcmaxText(void) const
{
        return FloatToStr(__vcmax);
}

AnsiString TMotionFunction::getvcText(void) const
{
        return FloatToStr(__vc);
}
void TMotionFunction::setvcText(const AnsiString& S)
{
        try {
                setvc(StrToFloat_(S));
        } catch(...) {
                throw;
        }
}

//--------------------------------------------------------------------------
//CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

AnsiString TMotionFunction::getText(void)
{
        AnsiString S;

        //imprime las propiedades heredadas
        S += AnsiString("vmaxabs = ")+getvmaxabsText()+AnsiString("\r\n");
        S += AnsiString("psta = ")+getpstaText()+AnsiString("\r\n");
        S += AnsiString("pfin = ")+getpfinText()+AnsiString("\r\n");
        S += AnsiString("vmax: ")+getvmaxText()+AnsiString("\r\n");
        S += AnsiString("Tmin: ")+getTminText()+AnsiString("\r\n");
        S += AnsiString("D: ")+getDText()+AnsiString("\r\n");
        S += AnsiString("T: ")+getTText()+AnsiString("\r\n");
        S += AnsiString("Dmax: ")+getDmaxText()+AnsiString("\r\n");
        S += AnsiString("vcmin: ")+getvcminText()+AnsiString("\r\n");
        S += AnsiString("vcmax: ")+getvcmaxText()+AnsiString("\r\n");
        S += AnsiString("vc = ")+getvcText()+AnsiString("\r\n");

        return S;
}

//---------------------------------------------------------------------------
//MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN:

//inicializa las propiedades a sus valores por defecto
TMotionFunction::TMotionFunction(void) :
        __vmaxabs(0), __psta(0), __pfin(0), __vmax(0),
        __Tmin(0), __D(0),
        __Dmax(0), __vcmin(0), __vcmax(0), __vc(0)
{
        __T = 0;
}

//contruye un clon de una función de movimiento
TMotionFunction::TMotionFunction(TMotionFunction *MF)
{
        //el puntero MF debe apuntar a una función de movimeinto contruida
        if(MF == NULL)
                throw EImproperArgument("pointer MF should point to built motion function");

        //copia todas las propiedades
        __vmaxabs = MF->__vmaxabs;
        __psta = MF->__psta;
        __pfin = MF->__pfin;
        __vmax = MF->__vmax;
        __Tmin = MF->__Tmin;
        __D = MF->__D;
        __T = MF->__T;
        __Dmax = MF->__Dmax;
        __vcmin = MF->__vcmin;
        __vcmax = MF->__vcmax;
        __vc = MF->__vc;
        __Label = MF->__Label;
}

//destruye una función de movimiento
TMotionFunction::~TMotionFunction()
{
}

//---------------------------------------------------------------------------

} //namespace MotionFunctions

//---------------------------------------------------------------------------

