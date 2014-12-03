//---------------------------------------------------------------------------
//Archivo: FiberMOSModel.h
//Contenido: Fiber MOS Model
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef FIBERMOSMODEL1_H
#define FIBERMOSMODEL1_H

#include "ExclusionAreaList.h"
#include "ExclusionArea.h"

//---------------------------------------------------------------------------

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------

//La clase TFiberMOSModel12:
//      Permite la detección de colisiones con las áreas de exclusión fijas.

//clase Fiber MOS Model
class TFiberMOSModel1 {
protected:
        //CONJUNTOS DE PROPIEDADES EN MODO TEXTO:

public:
        //------------------------------------------------------------------
        //LISTAS DE OBJETOS:

        //lista de robots posicionadores
        TRoboticPositionerList RPL;
        //lista de áreas de exclusión
        TExclusionAreaList EAL;

        //------------------------------------------------------------------
        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //conjunto de propiedades de instancia
        //en formato asignaciones
        AnsiString getInstanceText(void) const;
        void setInstanceText(const AnsiString&);

        //##################################################################
        //MÉTODOS PÚBLICOS:
        //##################################################################

        //------------------------------------------------------------------
        //MÉTODOS ESTÁTICOS:

        //lee una instancia de Fiber MOS Model
        //en una cadena de texto en formato de asignaciones
        static void  ReadInstance(TFiberMOSModel1 *MP,
                const AnsiString &S, int &i);

        //------------------------------------------------------------------
        //MÉTODOS DE CONSTRUCCIÓN, COPIA, CLONACIÓN Y DESTRUCCIÓN:

        //construye un Fiber MOS Model
        TFiberMOSModel1(void);

        //clona un Fiber MOS Model
        void Clone(const TFiberMOSModel1*);

        //construye un clon de un Fiber MOS Model
        TFiberMOSModel1(const TFiberMOSModel1*);

        //destruye un Fiber MOS Model
        ~TFiberMOSModel1();

        //MÉTODOS DE DESTRUCCION DE OBJETOS:

        //destruye todos los objetos de la lista
        void Destroy(void);
        //destruye los objetos seleccionadas de la lista
        int DestroySelected(void);

        //MÉTODOS PARA CONTRUIR Y DESTRUIR POSICIONADORES:

        //contruye una colmena cuadrada y asimilala situación
        void BuildSquareHive(void);
        //borra los posicionadores seleccionados y asimilala situación
        void DeleteSelected(void);

/*        //MÉTODOS DE TRANSFORMACIÓN DE COORDENADAS:

        //transforma una lista de puntos objetivo dados en coordenadas cartesianas en S0,
        //en una lista de puntos objetivo dados en posiciones angulares de los ejes en pasos de sus posicionadores adscritos
        //los puntos objetivo deben estar en el dominio de sus posic. adscritos (en sentido laxo, teniendo en cuenta el error num)
        void S0recToAmgularPositions(TTargetPointAngPosList& TPAPL,
                TTargetPoinRecList& TPRL);
  */
        //MÉTODOS DE ASIMILACIÓN:

        //asimila la configuración de las listas de objetos
        void Assimilate(void);

        //MÉTODOS PARA DETERMINAR SI HAY COLISIONES:

        //determina si algún brazo de algún posicionador
        //colisiona con el brazo de algún posicionador adyacente
        bool ThereIsCollision(void);
        //busca los posicionadores de la lista cuyo brazo colisiona con
        //el brazo de algún otro posicionador adyacente
        void SearchCollinding(TVector<int> &indices);
        //obtiene los conjuntos de posicionadores en colisión en la exposición indicada
        void GetCollisionClusterList(TPointersList<TItemsList<TRoboticPositioner*> > &CCL);

        //MÉTODOS RANDOMIZADO DE LOS EJES:

        //lleva los ejes de los posicionadores a posiciones aleatorias
        //con distribución uniforme en sus dominios
        //en las que no colisionan entre si
        void RandomizeWithoutCollision(void);
        //lleva los ejes de los posicionadores seleccionados a
        //posiciones aleatorias con distribución uniforme en
        //sus dominios en las que no colisionan entre si
        int RandomizeWithoutCollisionSelected(void);

        //MÉTODOS DE RANDOMIZADO DE P3:

        //lleva el punto P3 de los posicionadores a posiciones aleatorias
        //con distribución uniforme en su dominio
        //en las que no colisionan entre si
        void RandomizeP3WithoutCollision(void);
        //lleva el punto P3 de los posicionadores seleccioandos a
        //posiciones aleatorias con distribución uniforme en su dominio
        //en las que no colisionan entre si
        int RandomizeP3WithoutCollisionSelected(void);

/*        //------------------------------------------------------------------
        //MÉTODOS DE ARCHIVO:

        //escribe la instancia de la lista de robots posicionadores
        //en un directorio
        void WriteRPLToDir(const AnsiString& Dir);
        //lee una instancia de la lista de robots posicionadores
        //de un directorio
        void ReadRPLFromDir(const AnsiString& Dir);

        //escribe la instancia de la lista de áreas de exclusión
        //en un directorio
        void WriteEALToDir(const AnsiString& Dir);
        //lee una instancia de la lista de áreas de exclusión
        //de un directorio
        void ReadEALFromDir(const AnsiString& Dir);

        //escribe la instancia del Fiber MOS Model en un directorio
        void WriteInstanceToDir(const AnsiString& Dir);
        //lee una instancia del Fiber MOS Model de un directorio
        void ReadInstanceFromDir(const AnsiString& Dir);

        //Estos métodos de archivo no deben ser incluidos en la FMPT
        //porque la FMPT no debe tener acceso al sistema de archivos.
        //En su lugar se definirán funciones similares formando parte
        //de la FMPT Stand Alone Application:
        //      //escribe la instancia de una lista de robots posicionadores
        //      //en un directorio
        //      void WriteInstanceToDir(const AnsiString& Dir, const TRoboticPositionerList&);
        //      //lee una instancia de la lista de robots posicionadores
        //      //de un directorio
        //      void ReadInstanceFromDir(TRoboticPositionerList&, const AnsiString& Dir);
        //      //escribe la instancia de una lista de áreas de exclusión
        //      //en un directorio
        //      void WriteInstanceToDir(const AnsiString& Dir, const TExclusionAreaList&);
        //      //lee una instancia de la lista de áreas de exclusión
        //      //de un directorio
        //      void ReadInstanceFromDir(TExclusionAreaList&, const AnsiString& Dir,
        //              const TRoboticPositionerList&);
        //      //escribe la instancia de un Fiber MOS Model en un directorio
        //      void WriteInstanceToDir(const AnsiString& Dir, const TMultifiberPositioner1*);
        //      //lee una instancia del Fiber MOS Model de un directorio
        //      void ReadInstanceFromDir(TMultifiberPositioner1*, const AnsiString& Dir);
*/
        //MÉTODOS GRÁFICOS:

        //dibuja los posicionadores y las áreas de exclusión
        //en un trazador de formas
        //#void Paint(TPloterShapes* PS);
};

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // FIBERMOSMODEL1_H
