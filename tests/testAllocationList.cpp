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
//File: testAllocationList.cpp
//Content: test for the module AllocationList
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testAllocationList.h"
#include "../src/AllocationList.h"
//#include "cppunit_assert_emulator.h" //Qt only.

#include <iostream>
#include <limits.h> //INT_MAX
#include <float.h> //DBL_MAX

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestAllocationList);

//overide setUp(), init data, etc
void TestAllocationList::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestAllocationList::tearDown() {
}

//---------------------------------------------------------------------------

void TestAllocationList::test_()
{
    //build a RP list
    TRoboticPositionerList RPL;
    TRoboticPositioner RP2(2, TDoublePoint(-60.3, 87.035553));
    TRoboticPositioner RP4(4, TDoublePoint(-40.2, 87.035553));
    RPL.Add(&RP2);
    RPL.Add(&RP4);
    TExclusionAreaList EAL;
    RPL.assimilate(EAL);

    //build a allocation list, attached to a RP list
    TAllocationList AL(&RPL);
    //add allocations for each RP
    AL.Add(new TAllocation(&RP2, -51.296480, 82.217116));
    AL.Add(new TAllocation(&RP4, -37.372467, 92.504659));

    //PROPERTIES IN TEXT FORMAT:

    //get the allocation list in text format
    AnsiString S1 = AL.getAllocationsText();
    try {
        //try set the text to the allocation list
        AL.setAllocationsText(S1);

        //It is not posssible built more of one allocation for the same RP!

        //indicates error and return
        CPPUNIT_ASSERT(false);
        return;
    }
    catch(...) {
        //do noting
    }
    Destroy(AL);
    AL.setAllocationsText(S1);
    AnsiString S2 = AL.getAllocationsText();
    if(S1 != S2) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //BUILDING AND DESTROYING METHODS:

    //need destroy all allocations for avoid build duplicate allocations
    Destroy(AL);
    try {
        for(int i=0; i<10; i++) {
            //build a allocation list, attached to a RP list
            TAllocationList *AL = new TAllocationList(&RPL);
            //add allocations for each RP
            AL->Add(new TAllocation(&RP2, -51.296480, 82.217116));
            AL->Add(new TAllocation(&RP4, -37.372467, 92.504659));
            //destroy the TAllocation and destroy the list
            delete AL;
        }
    }
    catch(...) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //SEARCHING METHODS:

    //add allocations for each RP
    AL.Add(new TAllocation(&RP2, -51.296480, 82.217116));
    AL.Add(new TAllocation(&RP4, -37.372467, 92.504659));

    //search the allocation attached to a RP
    int i = AL.searchAllocation(&RP2);
    if(i != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }
    i = AL.searchAllocation(&RP4);
    if(i != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //search the allocation attached to an identified RP
    i = AL.searchAllocation(RP2.getActuator()->getId());
    if(i != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }
    i = AL.searchAllocation(RP4.getActuator()->getId());
    if(i != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }
    i = AL.searchAllocation(0);
    if(i < AL.getCount()) {
        CPPUNIT_ASSERT(false);
        return;
    }
    i = AL.searchAllocation(3);
    if(i < AL.getCount()) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //OBJETOS INDICADOS:

    //añade un punto objetivo para el posicionador indicado de la lista
    Destroy(AL);
    AL.AddAllocation(0);
    AL.AddAllocation(1);
    if(AL.getCount() != 2) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(AL[0]->getRP() != &RP2 || AL[0]->PP != RP2.getActuator()->getArm()->getP3()) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(AL[1]->getRP() != &RP4 || AL[1]->PP != RP4.getActuator()->getArm()->getP3()) {
        CPPUNIT_ASSERT(false);
        return;
    }
    //borra el punto objetivo indicada de la lista
    AL.DeleteAllocation(0);
    if(AL.getCount() != 1) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(AL[0]->getRP() != &RP4) {
        CPPUNIT_ASSERT(false);
        return;
    }
    AL.DeleteAllocation(0);
    if(AL.getCount() != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //ALL ITEMS:

    //añade puntos objetivo con la posición del punto P3
    //de todos los posicionadores de la lista
    AL.AddP3();
    if(AL[0]->PP != AL[0]->getRP()->getActuator()->getArm()->getP3() || AL[1]->PP != AL[1]->getRP()->getActuator()->getArm()->getP3()) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //destruye los puntos objetivo
    //de todos los posicionadores de la lista
    AL.Delete_();
    if(AL.getCount() > 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //add allocations for each RP
    AL.Add(new TAllocation(&RP2, -51.296480, 82.217116));
    AL.Add(new TAllocation(&RP4, -37.372467, 92.504659));

    //asigna el punto PO3 a los puntos objetivo
    //de todos los posicionadores de la lista
    AL.SetP3o();
    if(AL[0]->PP != AL[0]->getRP()->getActuator()->getP3o() || AL[1]->PP != AL[1]->getRP()->getActuator()->getP3o()) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //asigna el punto P3 a los puntos objetivo
    //de todos los posicionadores de la lista
    AL.SetP3();
    if(AL[0]->PP != AL[0]->getRP()->getActuator()->getArm()->getP3() || AL[1]->PP != AL[1]->getRP()->getActuator()->getArm()->getP3()) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //randomiza los puntos objetivo con distribución uniforme
    //en el dominio del punto P3 de sus posicionadores adscrito
    //de todos los posicionadores de la lista
    double r_max = 0;
    for(int i=0; i<10; i++) {
        AL.Randomize();
        double r_ = Mod(AL[0]->PP - AL[0]->getRP()->getActuator()->getP0());
        if(r_ > r_max)
            r_max = r_;
    }
    if(r_max <= 0) {
        CPPUNIT_ASSERT(false);
        return;
    }
    if(r_max > AL[0]->getRP()->getActuator()->getr_max()) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //randomiza los puntos objetivo con distribución uniforme
    //en el dominio del punto P3 de sus posicionadores adscritos
    //de todos los posicionadores de la lista
    for(int i=0; i<10; i++) {
        AL.RandomizeWithoutCollision();
        AL.MoveToTargetP3();
        TActuator *A1 = AL[0]->getRP()->getActuator();
        TActuator *A2 = AL[1]->getRP()->getActuator();
        double D = A1->getArm()->getContour().distanceMin(A2->getArm()->getContour());
        if(D < A1->getArm()->getSPM() + A2->getArm()->getSPM()) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    //asigna los puntos objetivo
    //al punto P3 de sus posicionadores adscritos
    //si algún punto objetivo no está en el dominio de
    //su posicionador adscrito lanza EImproperCall
    //de todos los posicionadores de la lista
    AL.MoveToTargetP3();
    double d1 = Mod(AL[0]->getRP()->getActuator()->getArm()->getP3() - AL[0]->PP);
    double d2 = Mod(AL[1]->getRP()->getActuator()->getArm()->getP3() - AL[1]->PP);
    if(d1 > 0.05 || d2 > 0.05) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //NOTA: cunado un punto es asignado al punto P3 de un posicionador
    //cuya cuantificación está activada, el posicionador se moverá
    //a la posición estable máspróxima.

    //VALIDACIÓN:

    //Los siguientes métodos deben ser invocados antes de
    //desplazar los posicionadores a sus puntos objetivo:

    //busca los puntos objetivo adscritos a posicionadores repetidos
    TVector<int> indices;
    AL.SearchRepeatedRPs(indices);
    if(indices.getCount() > 0) {
        CPPUNIT_ASSERT(false);
        return;
    }
    try {
        AL.Add(new TAllocation(&RP2, AL[0]->PP));

        //can't be added a repeated allocation
        //so a exception is throwed

        if(indices.getCount() != 1 || indices[0] != 2) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }
    catch(...) {
        //do nothing
    }

    //busca los puntos objetivo adscritos a posicionadores ausentes
    //en la lista de posicionadores RoboticPositionerList
    RPL.deleteIfFind(&RP2);
    indices.Clear();
    AL.SearchMissingRPs(indices);
    if(indices.getCount() != 1 || indices[0] != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //El método SearchMissigRPs es necesario
    //porque la lista de puntos objetivo adscrita (RoboticPositionerList)
    //puede ser manipulada.

    //busca los puntos objetivo que están fuera del dominio
    //de sus posicionadores adscritos
    AL[0]->PP = TDoublePoint(0, 0);
    indices.Clear();
    AL.SearchOutDomineTAllocations(indices);
    if(indices.getCount() != 1 || indices[0] != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //Determina la invalidez de una lista de puntos objetivo
    //para ser programada.
    //Valores de retorno:
    //      0: lista de puntos objetivo válida;
    //      1: posicionadores adscritos de los puntos objetivo indicados
    //         ausentes;
    //      2: puntos objetivo indicados fuera del dominio de
    //         sus posicionadores adscritos.
    indices.Clear();
    int result = AL.Invalid(indices);
    if(result != 1 || indices.getCount() != 1 || indices[0] != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }
    RPL.Insert(0, &RP2);
    indices.Clear();
    result = AL.Invalid(indices);
    if(result != 2 || indices.getCount() != 1 || indices[0] != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }
    AL[0]->PP = TDoublePoint(-51.296480, 82.217116);
    indices.Clear();
    result = AL.Invalid(indices);
    if(result != 0 || indices.getCount() != 0) {
        CPPUNIT_ASSERT(false);
        return;
    }

    //SEGREGACIÓN:

    //segrega los posicionadores de los puntos objetivo en dos listas:
    //      internos: con el brazo dentro del área de seguridad;
    //      externos: con el brazo fuera del área de seguridad;
    //si algún punto objetivo está fuera del dominio de
    //su posicionador adscrito:
    //      lanza EImproperCall
    AL.MoveToTargetP3();
    TRoboticPositionerList Inners;
    TRoboticPositionerList Outsiders;
    AL.SegregateInOut(Inners, Outsiders);
    if(Inners.getCount() != 0 || Outsiders.getCount() != 2 || Outsiders[0] != &RP2 || Outsiders[1] != &RP4) {
        CPPUNIT_ASSERT(false);
        return;
    }

/*    //ALMACENAMIENTO Y RECUPERACIÓN DE POSICIONES:

    //apila las posiciones de los posicionadores adscritos
    void PushPositions(void);
    //restaura las posiciones de los posicionadores adscritos
    void RestorePositions(void);
    //desempila las posiciones de los posicionadores adscritos
    void PopPositions(void);
    //restaura y desempila las posiciones de los posicionadores adscritos
    void RestoreAndPopPositions(void);

    //COLISIÓN:

    //levanta las banderas indicadoras de determinación de colisión
    //pendiente de todos los posicionadores adscritos a los puntos objetivo
    void EnablePendingCollisionDetermineTAllocations(void);

    //El siguiente método debe invocarse una vez desplazados
    //los posicionadores a los puntos objetivo:

    //busca los puntos objetivo que colisionan con otros puntos objetivo
    void SearchCollindingTAllocations(TVector<int> &indices);

    //ADVERTENCIA: cuando la cuantificación de alguno de los ejes
    //de los posicioandores adscritos esta deactivada, SearchCollinding
    //comprobará si hay colisión en las posiciones cuantificadas de
    //los ejes que tengan la cuantificación activada, con lo cual
    //el punto P3 de los posicionadores podría no coincidir exactamente
    //con el punto obejtivo.

    //COUNT OF ALLOCATION TYPES:

    //count the number of reference sources in the allocation list
    unsigned int countNR(void) const;
    //count the number of blanks in the allocation list
    unsigned int countNB(void) const;
    */
    CPPUNIT_ASSERT(true);
}

