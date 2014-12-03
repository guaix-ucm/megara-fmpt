//---------------------------------------------------------------------------
//Archivo: FiberMOSModel.h
//Contenido: Fiber MOS Model
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FIBERMOSMODEL2_H
#define FIBERMOSMODEL2_H

#include "FiberMOSModel1.h"
#include "ExclusionArea.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------

//La clase TFiberMOSModel2:
//      Permite la detección de colisiones conlas áreas de exclusión fijas.
//      Permitela ejecución simulada por temporizador.

//clase lista de posicionadores
class TFiberMOSModel : public TFiberMOSModel1 {
protected:
public:
        //##################################################################
        //MÉTODOS PÚBLICOS:
        //##################################################################

        //------------------------------------------------------------------
        //MÉTODOS DE CONSTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

        //construye un Fiber MOS Model
        TFiberMOSModel(void);

        //clona un Fiber MOS Model
        void Clone(const TFiberMOSModel*);

        //construye un clon de un Fiber MOS Model
        TFiberMOSModel(const TFiberMOSModel*);

/*        //destruye un Fiber MOS Model
        ~TFiberMOSModel();
  */
};
/*
//Fiber MOS Model en formato Texto
class TFiberMOSModelInstanceText {
public:
        //lista de posicionadores de fibra en formato texto
        TFiberPositionerListText FPLT;
        //lista deñáreas de exclusión en formato texto
        TExclusionAreaListText EALT;

        //construye un Fiber MOS Model en formato texto
        TFiberMOSModelText(void);

        //vacía el Fiber MOS Model en formato texto
        void Clear(void);

        //indica si el objeto no contiene una instancia
        bool IsEmpty(void);

        //aplica la instancia a una lista de posicionadores de fibra
        void GetInstance(TFiberPositionerList& FPL) const;
        //obtiene la instancia de una lista de posicionadores de fibra
        void SetInstance(TFiberPositionerList& FPL);
};*/

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // FIBERMOSMODEL2_H
