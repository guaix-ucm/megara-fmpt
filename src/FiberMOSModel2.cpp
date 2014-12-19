//---------------------------------------------------------------------------
//Archivo: FiberMOSModel.cpp
//Contenido: Fiber MOS Model
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "FiberMOSModel2.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//###########################################################################
//MÉTODOS PÚBLICOS:
//###########################################################################

//---------------------------------------------------------------------------
//MÉTODOS DE CONSTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

//construye un Fiber MOS Model
TFiberMOSModel::TFiberMOSModel(void) :
        TFiberMOSModel1()
{
}

//clona un Fiber MOS Model
void TFiberMOSModel::Clone(const TFiberMOSModel *FMM)
{
        //el puntero FMM debería apuntar a un Fiber MOS Model construido
        if(FMM == NULL)
                throw EImproperArgument("pointer FMM should point to built Fiber MOS Model");

        //clona las listas
        RPL.Clone(&FMM->RPL);
        EAL.Clone(&FMM->EAL);
}

//construye un clon de un Fiber MOS Model
TFiberMOSModel::TFiberMOSModel(const TFiberMOSModel *FMM) :
        TFiberMOSModel1(FMM)
{
}

/*//destruye un Fiber MOS Model
TFiberMOSModel::~TFiberMOSModel()
{
}
  */
//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

